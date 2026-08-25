# tools/wavescan — 開波前的五個檢查

這幾支不是「輔助工具」，是**五次付過代價之後被固定下來的開波程序**。
20260825 一天之內，靠這幾件事擋下五個安全項與兩個會靜默出錯的抽取缺陷。

順序就是下面的順序。

---

## 0. `goldenscan.py` —— 掃 golden 前一律先過這支

golden 是 cp950。這支負責兩件會靜默出錯的事：

```python
from goldenscan import load, code_only
lines, live = load('D:/HT9045/HT9011UC_Code_V3.33.906.0_20260618/cXxx.cpp')
```

- `code_only(line)` 剝掉字串常值與 `//`，**再**數括號。
  **陷阱一（FW-CFG-W2）**：golden `cConfiguration.cpp:1950` 的行尾註解裡有一個 `}`，
  天真的括號配對因此判定 `InitConfigEdtList_ItemE` 結束於 `:1951`，實際到 `:2111`，
  **少抓 160 行**。那次是編譯器抓到的——但截斷點若剛好括號平衡就會安靜地少翻一段。
- `live[i]` 標出第 i 行是否落在 `/* */` 區塊註解內（字串裡的 `/*` 不會誤判）。
  **陷阱二（FW-TF-W1）**：`cTemperFrom.cpp:1904-1915` 與 `:1941-1948` 兩個函式
  整段包在區塊註解裡，golden 根本沒編譯；但 `TfXxx::Name(` 這種 regex 照樣命中，
  census 也算成「缺的方法」。照抽下去就是**把 golden 停用的碼加進 port**。

單獨執行會列出該檔所有落在區塊註解內的函式定義：

```
python tools/wavescan/goldenscan.py D:/HT9045/HT9011UC_Code_V3.33.906.0_20260618/cTemperFrom.cpp
```

---

## 1. 先讀該檔對應 header 的 `EXPLICITLY EXCLUDED` / `GATE REGISTER` 段落

**沒有工具，因為這一步只能用讀的。**

**census 的「缺」欄位不是待辦清單。** `forms/fTemperFrom.h` 把該檔全部 15 個「缺」的
方法列為刻意排除並附理由，**其中三個是安全項**（`Timer1Timer` 驅動硬體開關
`SW[SwCCDCooling]`、`Panel71/72/73MouseDown` 帶 `HandlerSystem->ShowModal()` 這個
StopAllMotor 側門、`Check_Tri_Temp_All_Temperature` 進 StopAllMotor 領域）。

同一天另外兩個候選也是被自己的 header 擋下的：
`forms/fSCKART.h` 的 RECONCILIATION DEBT（golden 一個物件、port 五個宣告點、
兩處已被測試釘住的行為分歧，「這是分析工作不是機械式對齊」）、
`Interface/TesterTCP.h` 的 NOT-attempted 清單（剩餘明確是 UI 軸）。

## 2. `survey_file.py` —— 這個檔還缺什麼

```
python tools/wavescan/survey_file.py cConfiguration.cpp TfConfiguration
```

列出 golden 的方法、port 有沒有、缺的行數。**這是規模資訊，不是可做性資訊。**

## 3. `screen_methods.py` —— 逐方法安全篩選

```
python tools/wavescan/screen_methods.py cConfiguration.cpp TfConfiguration [方法名...]
```

對每個方法的本體掃這幾類風險：IO/繼電器、馬達、氣缸、硬體指令、寫檔/寫ini、
緒參數初始化、警報/對話框、golden 自帶的 `AI(safety` 標記、**跨表單呼叫**、送命令/上傳。

⚠ **「乾淨」的意思是「這些樣式沒命中」，不是「安全」。**
第一版少了「跨表單呼叫」那一類，於是 `btnN31_ManualClick`（觸發 FTP 溫度補償上傳）、
`btnN25_3_ManualClick`（送 EventLog 上傳命令）、`btnN35_TestClick`（存檔並上傳）、
`btnA71ManuallyClick`（批次複製 recipe）全被判成乾淨。補上那一類之後，
同一個檔的「乾淨」從 110 個掉到 68 個。

**名字裡有 Manual / Test / Send / Update 的，一律親眼開 golden 看過再決定。**

## 4. `wave_extract.py` —— 抽出本體並列出缺的識別字

```
python tools/wavescan/wave_extract.py <TAG> <golden檔> <類別> <方法名...>
```

## 5. `owner_report.py` —— widget 的擁有者是誰

```
python tools/wavescan/owner_report.py <TAG> <golden header> <port header> <port cpp>
```

把缺的識別字分成「本表單自己的」「屬於別的物件」「裸全域」。

**陷阱三（FW-CFG-W5 第一次嘗試）**：成員產生器看到 `fPassword->Label3` 就把 `Label3`
加成了 `TfConfiguration` 的成員——**它屬於另一個表單**。那一波因此整個退掉重做。

---

## 收工自檢（在 gate 之前）

抽完、套完之後，對每個方法跑一次「LIVE 敘述逐句回 golden 找逐字對應」，
沒命中的**每一條**都必須對應到已記錄的 deviation／substitution。
FW-CFG 各波的數字：W1 533/0、W2 1,003/2、W3 1,076/0、W4b 108/1、W5 455/2、W6 172/17
（W6 那 17 是 17 個函式的簽章行）。

注意：這個檢查**抓不到「少抓一段」**（少的那段根本不在 port 裡），
所以它不能取代第 0 步的範圍檢查——兩個都要做。
