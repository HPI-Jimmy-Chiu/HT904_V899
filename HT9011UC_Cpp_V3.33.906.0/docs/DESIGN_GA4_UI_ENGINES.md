# DESIGN_GA4_UI_ENGINES — GA-4 首燈 UI 引擎設計決策（D-GA4-1 ~ D-GA4-10）

> AI(W906-GA4) 20260804: GATE_A_FIRST_LIGHT_PLAN.md §4-GA4 的落地設計。每項含裁決＋理由＋
> 落地位置。本檔由主迴圈（Fable 5 xhigh）親自撰寫——GA-4 是全計畫唯一不可降級的波
> （無 golden oracle，論證品質是唯一防線）。實作 agent 以本檔＋`ui/layout/*.h` 介面為契約。

## 執行期輸入盤點（動筆當下親自驗證）

| 產物 | 內容 | 缺什麼 |
|---|---|---|
| `tools/dfm2rc/rc_out/main.rc` | 91 DIALOGEX / 946 控件行，rc.exe 實測過（exit 0，46,492B .res） | 模板座標欄位放的是 .dfm 像素值，但 dialog manager 會按 DLU 縮放——模板幾何**不可信** |
| `rc_out/main_ids.h` | 850 個 `#define`（IDD 8192起 / IDC 1024起） | 純 #define，執行期無法由符號字串反查數值 |
| `rc_out/main.rcmeta.json` | 每 dialog 的 idd_value/owner_path/is_root/owner_class + 每控件 symbol/value/source_path/synthesized/rule | 執行期讀不到 JSON |
| `layout_out/main_layout.gen.cpp` | 850 列 DfmControlLayout：幾何（.dfm 像素權威, D7）/字型/顏色/custom props/樹形（kind, parent_path, host_dialog_path） | 無 IDD/IDC 數值；無 ROOT client 尺寸（.dfm 存 ClientWidth/Height，非 Width/Height 四欄）；無 TTabSheet Caption；無 ActivePage |
| `ir_out/main.dfm.ir.json` | 全屬性（ClientWidth/ClientHeight、TTabSheet Caption、pgMain ActivePage=tsMain、TabWidth=80） | 執行期讀不到 |
| `ui/RegisterCustomClasses.cpp` | 16 class 全註冊，wndproc=**DefWindowProc** | 模板建出的自訂控件是死的——必須有 attach pass 事後 SubclassWindow |

kind 統計：ROOT 1 / SUBDLG 90 / TABHOST 5 / LEAF 727 / NONVISUAL 27。
合成控件：SYNTH_CONTAINER_FRAME ×14、SYNTH_RADIOGROUP_ITEM ×12（無 layout 列）。

## D-GA4-1 join 表：新產生器 emit_uimap.py → `<form>_uimap.gen.{h,cpp}`

**裁決**：新增 `tools/dfm2rc/emit_uimap.py`（讀 rcmeta.json + ir.json），輸出每表單一組
`rc_out/<form>_uimap.gen.{h,cpp}`，schema 由手寫 `ui/layout/UiFormMap.h` 定義（比照
DfmLayoutTypes.h 的「schema 手寫、資料生成」慣例）。內容：
- dialogs[]：dfm_path ↔ idd 數值、owner_class、caption(UTF-8，TTabSheet 分頁標題)、is_root
- controls[]：source_path ↔ IDC 數值、synthesized、rule（含合成控件——layout 表沒有它們）
- tabhosts[]：TPageControl 的 active_page_path、tab_width
- 表單層：root client 尺寸（IR ClientWidth/ClientHeight；.dfm ROOT 不寫 Width/Height）

**不採**的替代案：(a) 依賴 IDD=8192+序 的分配規律——隱式耦合產生器內部行為，drift 無檢測；
(b) 手打 91+850 列——違反 data-driven、必錯；(c) 執行期 EnumResourceNames——拿得到數值拿不到名。
rcmeta 本來就是管線欽定的 join 來源（DfmLayoutTypes.h note 1 的 gate_id_symbol_join 同源）。

## D-GA4-2 DLU 陷阱與幾何權威

模板欄位是「像素值寫進 DLU 欄」，CreateDialog 後所有窗口都被 DLU 因子放大（≈1.5×/1.6×）。
**裁決**：D7 像素權威徹底執行——凡有 layout 列且 has_left/top/width/height 的窗口，一律
ApplyLayout 以 `SetWindowPos`（像素、相對 host client）覆蓋；模板座標只當「建立時的初值」。
ROOT：client 尺寸取 uimap 的 root_client_w/h，經 `AdjustWindowRectEx`（真實 style/exstyle）換
外框尺寸；位置取 ROOT 列 left/top（.dfm 值），加 `--center` 開發開關（不影響忠實語意，僅首燈
便利，與 --devpath 同級記錄）。

## D-GA4-3 巢狀掛載規則（91 dialog 樹）

- 父視窗選擇：SUBDLG 列的**字面 parent_path 若是 TABHOST**（TPageControl）→ 視窗父=該 tab
  控件 HWND（VCL 中 TTabSheet 的 WinAPI parent 就是 TPageControl）；否則父=host_dialog_path
  對應 dialog 的 HWND。
- TTabSheet：`TCM_INSERTITEMW` 依 sibling_index 插分頁（caption 取 uimap，UTF-8→UTF-16）；
  幾何=父 tab 控件 client rect 經 `TCM_ADJUSTRECT`（不取 layout 列——.dfm 對 TTabSheet 不寫
  幾何，has_* 全 false，實測確認）；初始顯示頁=tabhosts[].active_page_path（fMain=tsMain），
  其餘 `SW_HIDE`；`TCN_SELCHANGE` 切換 show/hide。
- 其他 SUBDLG（TPanel/TGroupBox/TScrollBox/TRadioGroup）：`CreateDialogParamW` 建立後由
  ApplyLayout 按其 layout 列定位。TScrollBox 首燈不做捲動（記 SKIPPED）。
- 建立順序：按 uimap dialogs[] 順序（= rcmeta 順序 = 樹的前序），父必先於子。
- 共用 child DLGPROC：WM_INITDIALOG 回 FALSE（不搶焦點）；DWLP_USER 存 FormRuntime*；
  WM_CTLCOLOR* 轉 CtlColorEngine；WM_NOTIFY(TCN_SELCHANGE) 轉樹引擎；WM_COMMAND 先
  bubble 到 root（GA-3 binder 接手前僅 BootLog 記錄）。

## D-GA4-4 無 layout 列子窗的比例補正（合成控件）

SYNTH_CONTAINER_FRAME/SYNTH_RADIOGROUP_ITEM 無 layout 列，建立時吃了 DLU 放大、又不被
ApplyLayout 覆蓋。**裁決**：host dialog 被 resize 前記下舊 client，resize 後對「無 layout 列的
直接子窗」按新舊 client 比例縮放。框架控件模板即滿版 → 比例縮放後仍滿版（精確）；
radio item → 近似 VCL 網格（記 W7-UI-SKIPPED：12 個實例，精確網格演算法延後）。

## D-GA4-5 z-order

模板順序=.dfm 宣告順序（產生器保序輸出）。首燈**不重排**，僅提供 `--zorder` 開關把每個
dialog 的兄弟鏈整鏈反轉（SetWindowPos），供 GA-5 目視測極性（W7 §7-10 的既定未知，
計畫書指定用開關一次翻轉實測，不預先猜）。

## D-GA4-6 字型

- cache key=(face,height,style)→HFONT，FormRuntime 擁有、解構釋放。
- `font_height_px` 依 schema 註記**原樣**進 `LOGFONT.lfHeight`（同號約定，不修正）。
- style bits：fsBold→FW_BOLD、fsItalic、fsUnderline、fsStrikeOut。
- face 是 UTF-8（含 CJK）→ `CreateFontIndirectW`。charset 表沒帶 → DEFAULT_CHARSET（記錄）。
- 套用：`WM_SETFONT`（含 SUBDLG dialog 自身）。無 has_font 的控件繼承模板 DS_SETFONT 字型。

## D-GA4-7 顏色（OnCtlColor 引擎）

- TColor→COLORREF：`v<0`（0x80000000 位）→ **paint 時** `GetSysColor(v & 0xFF)`（schema note 4
  明言不可預烘系統色）；否則低 24 位即 COLORREF（同為 0x00BBGGRR）。
- token 非空且 has_*=false（未解析 token）→ 跳過並可 BootLog 一次性記錄。
- brush cache：resolved COLORREF→HBRUSH，FormRuntime 擁有；系統色 brush 用
  `GetSysColorBrush`（不進 cache，OS 擁有）。
- 路由：WM_CTLCOLORSTATIC/BTN/EDIT/LISTBOX → 查 hwndToRow：has_font_color→SetTextColor、
  has_color→SetBkColor+回該色 brush；否則回 NULL 走預設。WM_CTLCOLORDLG → 查該 dialog 的
  SUBDLG/ROOT 列 color（fMain=clBtnFace）。自訂控件自畫不走此路。

## D-GA4-8 自訂控件 attach pass

模板建出的 HT9045.* 窗（fMain：ALed 222/MyTray 56/BtnPanel 11/MyLed 6）wndproc 是
DefWindowProc（RegisterCustomClasses 註記：MFC subclasses later；CLedCtrl banner 註記
PreSubclassWindow 服務 SubclassDlgItem 路徑——attach 是 C5 的既定設計）。
**裁決**：AttachCustomControls 對 shell-backed class 逐一 `new CXxxCtrl` + `SubclassWindow`，
FormRuntime 擁有（解構 UnsubclassWindow+delete）；再按 layout 列推 props：
- LED：true/false/true_font/false_font color（**resolved int 原樣推進 core**——LedCore 是
  aled.pas 忠譯，吃 TColor 語意，系統色由 render 端解析；agent 落地前必須讀 LedCore.h 確認
  setter 簽名，不得臆測）、custom bag 的 LEDStyle/Interval，經 shell forwarder（SetValue/
  SetBlink/SetInterval）或 Core()+SyncFromCore()。
- BtnPanel：TrueColor/FalseColor/TrueFontColor/FalseFontColor + caption（模板已帶）。
- MyTray：XItem/YItem/DirectWidth/DirectStyle/EdgeWidth/LineWidth（bag）。
- placeholder class（StringGrid/Shape/Chart…）：不 attach，維持 inert 透明洞（bring-up 可見性
  是既定選擇，見 RegisterCustomClasses.cpp banner）。

## D-GA4-9 headless 驗證（無 golden 的驗證閘）

root 模板無 WS_VISIBLE → CreateDialogIndirectParam 建出即隱藏，整棵樹永不 ShowWindow ——
「不彈窗」由構造保證（優於 HWND_MESSAGE：tab 幾何/親子關係全真）。新 probe
`ui/tests_headless/headless_fmain_probe.cpp`（比照 headless_ctrl_probe 風格，exit 0=全過）：
1. 91/91 dialog 建立成功；每 dialog GetParent 符合 D-GA4-3 規則
2. LEAF+TABHOST 每列（732）GetDlgItem 命中；NONVISUAL 27 列無 HWND
3. 幾何：全量 has_left 列 client-rect 對表（root 另按 AdjustWindowRectEx 驗）
4. 字型：has_font 列 WM_GETFONT 回讀 face/height 對表
5. attach：shell 數==shell-backed 列數；LED Value/TrueColor 回讀對表
6. tab：pgMain 2 個分頁、caption 對 uimap、初始顯示 tsMain
7. CtlColor：對一個 has_color 的 Static 發 WM_CTLCOLORSTATIC（memory DC）驗 brush/SetBkColor
8. 銷毀無洩漏（字型/brush cache 清空斷言）

## D-GA4-10 檔案與寫入紀律

新檔全在 `ui/layout/`、`ui/forms/`、`tools/dfm2rc/emit_uimap.py`、`rc_out/*_uimap.gen.*`、
`ui/tests_headless/headless_fmain_probe.cpp` —— 零碰撞可平行。**既有檔案（HT9045App.cpp、
CMakeLists.txt、scripts/build_msvc_ui.bat、app.rc、.vscode）一律由主迴圈親手改**，agent 只交
proposal（mutation-agent 與主迴圈同檔衝突的教訓已記憶體化）。生成物（main.rc/main_layout/
main_ids/uimap）除經產生器外不得手改。編碼 UTF-8、EOL=LF、結尾換行；//AI(W906-GA4) 20260804。
