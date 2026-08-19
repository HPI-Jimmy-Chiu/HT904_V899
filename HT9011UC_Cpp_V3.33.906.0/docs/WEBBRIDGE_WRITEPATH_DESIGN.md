# WebBridge Write Path 設計（FW-W）

AI(W906-FW-W-Design) 20260819。依 20260819 使用者裁決成文；本檔是 write path
實作波次的權威設計，取代 DFM2WEB_CAMPAIGN_PLAN.md §7 的「佇列等使用者」條款。

## 0. 使用者裁決（20260819，三條邊界）

1. **瀏覽器要能操作機台**（write path 核可）。
2. **登入與權限比照 BCB6 既有體系**——golden 本來就有登入（fMain 的
   cbUserSelect/btLogin/GetPasswoard 流程）與權限（AccessLevel 全域＋
   fSecurity::Insufficient 檢查）。不發明新權限模型。
3. **同時只允許一個瀏覽器操作**（單一操作權）。
4. **golden 沒有的二次確認不准額外增加**（golden 既有的
   ShowMyMessageBox_YES_NO 確認流程屬 golden 行為，照翻保留）。

## 1. 架構（沿用既有骨架，無新形狀）

```
瀏覽器 ──ws {type:"cmd",id,cmd,tag,value}──▶ wb_gateway ──TCP──▶ handler 行程
                                                                  │
   ◀──ws {type:"ack",id,ok,error}──◀──────────────────────────────┤
                                                       CommandQueue.tryPush()
                                                                  │（socket thread 只入列）
                                                       UI thread 既有 timer tick drain
                                                                  │
                                                       dispatch 表 → 呼叫 golden handler
```

- wire 協定＝web/docs/ARCHITECTURE.md §4 既有 cmd/ack（不改）。
- `WebBridge/CommandQueue.{h,cpp}` 既有（有界、執行緒安全、UI thread 汲取）——
  write path 的執行緒契約已解決，不重做。
- **互鎖的所在**＝dispatch 之後呼叫的 golden handler 本體：忠實翻譯的 handler
  自帶 Insufficient()/狀態檢查（SecCC/SecUnlock 波已落地）。web 層**不再疊**
  一層自己的互鎖——這正是「比照 BCB6」的意思：BCB6 的按鈕也是直接呼叫同一個
  handler，安全檢查在 handler 內。

## 2. 登入與權限（裁決 2 的落地形）

- 新指令 `auth.login {user, password}`：走 golden 的密碼驗證路徑
  （GetPasswoard/Security_new.def 讀取，唯讀），成功後設定該連線 session 的
  身分並把全域 `AccessLevel` 設為對應等級——與 golden btLogin 同一條路。
- `auth.logout`：回 Operator（AccessLevel=0），同 golden。
- 權限檢查點＝golden handler 內建的 `fSecurity->Insufficient(iType)`：
  指令進來 → dispatch → handler 自己拒絕並回 WAR1676（權限不足）→
  ack{ok:false,error:"WAR1676"}。web 層不做第二套權限表。
- tag 增補：`auth.level`（現在的 AccessLevel）、`auth.user`——瀏覽器據此
  灰化按鈕（鏡射 golden 的按鈕 Enabled 邏輯），但灰化只是 UX，authoritative
  的擋在 handler。

### 2.1 FW-W2 recon 補充（20260819，golden main.cpp 實測）

golden 登入真核心在 `TfMain::cbUserSelectChange`（main.cpp:14395-14800+，
2 萬行檔的深水區），結構＝多密碼源選擇（Login.dat／N15 txt-by-FTP／
編碼密碼檔→DecodeStr→PassList）＋fPassword modal 取輸入＋
`edPassword.UpperCase()==PassList[..].UpperCase()` 比對→`AccessLevel=l`
→`ChangeLevelAttr()`。web 版免 modal 的抽取形：

    int WebAuthVerify(user, password)  // 回 level 或 -1
      = golden 的密碼源載入（同一組 PassList 建構邏輯，讀取類 ACTIVE）
      + golden 的 UpperCase 比對語意（逐字）
      + 成功側效果：AccessLevel=level（ChangeLevelAttr 是 UI 鏡射，web 版
        由 auth.level tag 取代）

實作波（FW-W2）交付：WebAuth.cpp（抽取翻譯，golden 行號逐段引用）＋
wb_serve dispatch `auth.login/logout`＋`auth.level` tag＋cmd_probe 擴充。
客製分支（N15/Greatek/LoginDat）逐臂 gate-or-translate 照缺件現況。

## 3. 單一操作權（裁決 3 的落地形）

- 連線分兩級：**viewer**（預設，唯讀）與 **operator**（同時最多 1 個）。
- `control.acquire` → 若無人持有：授予，ack ok；有人持有：ack
  {ok:false,error:"control-held"}。**不做搶佔**（要搶＝對方先
  `control.release` 或斷線）。
- 持有權隨 ws 連線生命週期：斷線即釋放（gateway 偵測）；另設閒置逾時
  （預設 10 分鐘無 cmd 自動釋放，值進 config 可調）。
- 非持有者送任何非 auth/control 指令 → ack{ok:false,error:"not-operator"}。
- tag 增補：`control.owner`（持有者識別，""=無人）——所有瀏覽器看得到
  誰在操作。
- 注意順序：`auth.login`/`control.acquire` 是唯二不需要操作權的指令。

## 4. 二次確認（裁決 4 的落地形）

- **不新增任何確認對話框**。指令送達即入列。
- golden 既有的確認流程（handler 內呼叫 ShowMyMessageBox_YES_NO 才繼續）：
  這是 golden 行為，需保留——設計為 **modal 往返**：handler 走到確認點 →
  發 `{type:"modal",id,text,buttons}` 給 operator 連線 → 瀏覽器彈 golden
  同文案的視窗 → 答案以 `modal.answer` 指令回來 → handler 續行。
  ⚠ 此往返需要 handler 側可等待/續行的形狀（golden 是同步阻塞 modal）——
  **首波不碰含 modal 的 handler**（清單見 §6），modal 往返留 FW-W3 專波。

## 5. 安全底線（開發環境不變的鐵律）

- 開發機沒有真機：dispatch 進的是與 ctest 同一套 SM；馬達/IO 層是
  OFF_LINE/模擬。write path 的「動真機」語意要到真機環境才成立。
- wb_publish/wb_serve 維持 default-dry；共用 config 寫入 gate 政策不因
  write path 開通而放寬——handler 內被 gate 的寫檔仍是 gate（那是另一條
  redirect-seam 設計線）。
- e2e 測試（webprobe）不得與 ctest 並行（WB_TcpLink 撞埠）。

## 6. 波次切分

| 波 | 內容 | 驗收 |
|----|------|------|
| FW-W1 | 指令通道 e2e：gateway ws cmd→TCP→CommandQueue→drain→dispatch→ack 全鏈打通；dispatch 表雛形＋`sys.ping` 測試指令 | webprobe 新 cmd 探針：送 ping 收 ack；雙 gate |
| FW-W2 | `auth.login/logout`＋AccessLevel 對映＋`auth.*` tags | 探針：錯密碼 ack fail、對密碼後 level tag 變化 |
| FW-W3 | `control.acquire/release`＋斷線釋放＋閒置逾時＋`control.owner` tag | 探針：雙連線互斥、斷線釋放 |
| FW-W4 | 第一批真指令：**無 modal、無檔案寫、無馬達**的 handler（候選：fCounterClear 的 ClearCount 族、cbb 類選擇的顯示側） | 指令後 tag 值變化斷言 |
| FW-W5 | modal 往返（§4）＋含確認流程的 handler 解鎖 | 探針模擬答 Yes/No 兩路 |
| 佇列 | 會動馬達/會寫共用 config 的指令 | 逐項過（真機環境才有意義） |

## 7. 與 1203 HAL 線的關係

20260819 同日裁決：906 優先完善 PCIE-1203 HAL（Galil/MN200 後補；
mot_table.csv 決定馬達分類與控制邏輯）。write path 的馬達類指令（佇列段）
最終 dispatch 到 HAL——兩線在 FW-W 佇列段匯合，之前互不阻塞。
