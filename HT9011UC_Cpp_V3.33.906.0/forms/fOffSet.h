// =============================================================================
//  forms/fOffSet.h  --  non-VCL stand-in for golden's fOffSet form pointer
//
//  AI(W906-W7-F0) 20260728: split out of FormsFacade.h by the W7-F0 refactor
//  (docs/W7_UI_ARCHITECTURE_PLAN.md SS6-F0-d).  Content moved VERBATIM apart
//  from the F0-e virtualisation.  See forms/fMain.h for the facade-wide
//  contract.
// =============================================================================
#ifndef FORMS_FOFFSET_H
#define FORMS_FOFFSET_H

#include "forms/FormWidgets.h"

// =============================================================================
//  AI(W906-FW-NOTE-W33) 20260826: cOffSet Wave A -- 22 支唯讀方向 methods。
//  本檔既有內容一行未動（append-only，沿用 fLotInfo Wave B 規則）；
//  這一段以下全部是新增。
//
//  GOLDEN 參照：HT9011UC_Code_V3.33.906.0_20260618/cOffSet.cpp（4,179 行，
//  cp950，本波以 open(p,'rb').read().decode('cp950') 解碼，0 U+FFFD）
//  ＋ cOffSet.h（512 行）。
//
//  分母（自行重量，不引用上游數字）
//  --------------------------------------------------------------------------
//  golden cOffSet.cpp 有 62 支 DISTINCT TfOffSet:: 定義 / 4,179 行。
//  量法：字元狀態機剝掉 block 註解、行註解、字串與字元字面值後再掃
//  TfOffSet::NAME(，並用 #if 0 追蹤器標記死區。結果 62 hits、live 62、
//  dead 0（本樹上千個 #if 0 區塊，cOffSet.cpp 內沒有任何一個包住方法定義）。
//  ⚠ 這個 62 與主迴圈給的 62 相符 —— 但那是重量之後的結論，不是採信。
//
//  本波交付：22 支 / 228 golden 行（占 62 支的 35.5%、4,179 行的 5.5%）。
//  剩餘 40 支未交付：其中 12 支列入下方 GATE REGISTER（O-1..O-9，O-5 含
//  4 支），其餘 28 支本波未評估。
//
//  取批標準（唯讀方向）
//  --------------------------------------------------------------------------
//  只收「不動機台、不寫檔／持久化、不送對外命令」，且相依全部落在
//  ht9045_forms 可連的 archive（vclcompat / ht9045_globals / ht9045_core）
//  之內的 method。每一支都**實際開 golden 讀完整本體**後才翻，沒有一支是
//  靠 regex 訊號掃描過關的 —— 見 GATE REGISTER (O-5)：正是逐行讀才抓到
//  那四支會啟動機台，而它們在 regex 掃描下全是乾淨的。
//
//  DEPENDENCIES ADDED（全部只加在 forms/fOffSet.cpp，本 header 不新增 include）
//    cmydef.h      -- CUSTOMER_CODE / USE_PICKER_COUNT /
//                     USE_Scanner_AOI_Inspection / Tempture_Hot
//                     （⚠ iSortUnloadT6 **不在**本波的相依裡：唯一會碰它的
//                       btnSortAuto1Click 已整支退出，見 GATE (O-9)）
//    cprod.h       -- InputLimit / Temperature / TrayForm / UserDefForm_File / DeviceForm
//    Config.h      -- IniConfig（bA30SetupTeachFunction）
//    LastSet.h     -- LastSet（iTester / bNeedSetupTeach / iTemperature）
//    common.h      -- OnlyNumberAndDotInPut / ReadIniData
//    MachineType.h -- CC_KYEC_LEE / CC_ASE_CL / ep16Picker / eBtnAOI_*
//    forms/fQwertyKey.h -- fQwertyKey + ShowQwertyKey（同 target，無新 link edge）
//    forms/fMain.h      -- fMain->cbSetupFileName（同 target）
//  本 header 維持只 include forms/FormWidgets.h，所以既有 includer 的
//  compile graph 完全沒變。
//
//  DEVIATIONS
//  --------------------------------------------------------------------------
//  (D-1) TObject *Sender 未被讀時整支 drop；被 cast 使用時直接宣告成 cast
//        後的型別（TEdit *Sender / TButton *Sender），C-style cast 因此消失。
//        同 forms/fSetup.cpp:72/:84/:94/:188 的既有慣例。
//        TMouseButton Button, TShiftState Shift, int X, int Y 與
//        WORD &Key, TShiftState Shift 在本波每一支都逐支確認 golden 本體
//        從未讀取，故 drop。
//  (D-2) Close() 是永久 offline no-op（無視窗）。既有前例：
//        forms/fContact.h:1601、forms/fMotorTest.h:976、forms/fSpeed.h:1027、
//        forms/fQwertyKey.h:366、forms/fTrayAssignment.h:683。
//  (D-3) PageControl1->ActivePage —— vclcompat::TPageControl 只有
//        ActivePageIndex，沒有 ActivePage。照既有五處前例
//        （forms/fConfiguration.h:280、fLotInfo.h:1326、fObserver.h:448、
//        fTeach.h:600、fTrayAssignment.h:534）用 facade-local 子類別補
//        TTabSheet *ActivePage。基底沒有同名成員，**不構成 shadow**。
//  (D-4) Pnl_ScanAOI->Height —— vclcompat::TControl 有 Left/Top 但沒有
//        Height。照 forms/fContact.h:946 / fContactCT.h:254 / fSetup.h:395
//        的前例，在 facade-local 子類別補 int Height。基底無同名成員。
//        ⚠ 只存值，不排版 —— 本樹沒有視窗，設 Height 不會有任何東西改變大小。
//
//  GATE REGISTER（一律整支退出：header 宣告、.cpp 不定義，讓 linker 當互鎖）
//  --------------------------------------------------------------------------
//  (O-1) CheckBox1Click            golden :2787-2790
//        本體只有一句 ShowOneByOneOffSet();，而該支（golden :884-1400，
//        517 行）本波沒翻。翻一支只為呼叫一支沒翻的，等於空殼。
//  (O-2) btnOffsetListClick        golden :3369-3374
//        同上：ShowOffSetList();（golden :3388-3524，137 行）未翻。
//  (O-3) FormDestroy               golden :3094-3098
//        LogSoftwareOffTime() 的本體在 acarry_shims.cpp:255，屬
//        **ht9045_sm**（CMakeLists.txt:1353）。ht9045_forms 不能 link
//        ht9045_sm（CMakeLists.txt:559-560、:612-646 記錄的 cycle）。
//        規則 5：不自建跨檔缺口的 shim。
//  (O-4) GetOffsetPath             golden :1426-1462
//        FileInfo().PathCombin() 的本體在 ProductionInfo/FileInfo.cpp，
//        同樣屬 ht9045_sm。理由同 (O-3)。
//  (O-5) sb_AutoOffsetUpClick      golden :2888-2899
//        sb_AutoOffsetDownClick    golden :2901-2912
//        sb_AutoOffsetRightClick   golden :2914-2925
//        sb_AutoOffsetLeftClick    golden :2927-2938
//        ⚠ **會啟動機台**。四支本體都是
//          edArmX/Y->Text=...; spbSaveClick(this);
//          if(cb_AutoOffsetPositionCheck->Checked) fMain->Start("...");
//        spbSaveClick（golden :2803-2879）寫 offset 檔案，fMain->Start()
//        是開始運轉。兩條都踩硬退出線。
//        ⚠ 這四支在純 regex 訊號掃描下是乾淨的（本體沒有 MotorMove/Write*
//        字樣），是逐行讀 golden 本體才抓到的。
//  (O-6) ClearIndexOffset          golden :1817-1853
//        ⚠ **寫教導／偏移值**。:1837-1839 三個
//        WriteIniData(szDir, str, "Pick Up"/"Place"/"Contact", 0)
//        直接把 "Position OffSet.Data" 的 Index arm offset 歸零，並同步清
//        Offset / Offset_File 兩組全域。本專案已有「升版靜默毀掉教導值」的
//        代價，這條路一律退出。
//  (O-7) edOffsetContactForceChange golden :3352-3361
//        需要 fProductionInfo->CheckContactForceExist() /
//        SetOffsetContactForce() / GetOffsetContactForce()，三者**全樹皆無**
//        （重跑 20260826，排除 build*）；forms/fProductionInfo.h 目前只有
//        sLoadMO_TestFlow 與 CalTrayICCount。另需 fContact->dDutCount，
//        而 fContact 住在 forms/fContact.cpp（ht9045_sm）。
//        往別人的 facade 加方法就是跨檔 shim，規則 5 禁止。
//  (O-8) UseOutArmSetupTeach       golden :2976-3005
//        本體本身不動機台也不寫檔（純讀 AUTO_EMPTY_COLOR / OutOfs* /
//        LastSet，再 latch bOutArmStop[iArea]=true），相依全部可達。
//        退出理由是**對稱性**：同族的 UseInArmSetupTeach 在本檔既有行是
//        return false 的 stub，append-only 不准改。只交付 out-arm 側會
//        造成 golden 沒有的 in／out 臂不對稱行為（in 臂永不 setup-teach、
//        out 臂會）。兩支要一起處理，交給後續能改既有行的波次。
//  (O-9) btnSortAuto1Click         golden :3207-3217
//        ⚠ **值的來源不存在，翻了會選錯行為**。本體是
//          Ptr=(TButton *)Sender;
//          if(bA30SetupTeachFunction && iTester==OFF_LINE && bNeedSetupTeach)
//              iSortUnloadT6=Ptr->Tag;
//        vclcompat::TControl 確實有 Tag（Controls.h），但那個 Tag 的
//        **provenance 是 .dfm 設計期屬性**，本 port 從來不載入 .dfm 的 Tag
//        （見 vclcompat/Controls.h TControl::Tag 的 VALUE PROVENANCE 段：
//        dfm2rc 把 Tag 留在 stage-1 IR 與 web/forms/*.layout.json，
//        .rc/layout/uimap 三個 emitter 都丟掉，沒有任何 C++ 讀 layout）。
//        本波開 golden cOffSet.dfm 逐一確認：btnSortAuto1 Tag=1、
//        btnSortAuto2 Tag=2、btnSortAuto3 Tag=3、btnSortAuto4 Tag=4、
//        btnSortAuto5 Tag=5、btnSortAuto6 Tag=... 等六顆全都掛
//        OnClick=btnSortAuto1Click 並各帶不同的設計期 Tag。
//        在本樹 Ptr->Tag 會**一律讀 0**，於是 iSortUnloadT6 被寫成 0 而不是
//        按鈕自己的編號。iSortUnloadT6 的消費者是 aoutarm.cpp:4172-4174
//        （`OutArmSuck.iWhichAuto[...]=iSortUnloadT6;`），初值 -1
//        （cmydef.cpp:5126）配 `iSortUnloadT6>=0` 守衛 —— 也就是說寫 0
//        會把守衛從關打開，並把出料一律導到 Auto1。
//        0 不是中性的 else，是**主動命中一個錯的分支**。整支退出。
//
//  ⚠ 既有兩支 stub 與 golden 的落差 —— 明講不隱藏
//  --------------------------------------------------------------------------
//  本檔既有的 UseAutoOffsetFunction / UseInArmSetupTeach 是 W6.2 留下的
//  return false stub，**與 golden 本體不同**：
//    * golden UseAutoOffsetFunction（:2940-2953）第一件事是
//      if(fAutoTeach->IsArmNeedPause(asWhich)) return true;
//      —— 20260826 收工重跑（字元狀態機剝註解／字串 ＋ #if 0 追蹤，
//      排除 build*）：`fAutoTeach` 的**活碼命中 0**、`IsArmNeedPause`
//      活碼命中 0；且全樹沒有 `class TfAutoTeach`、沒有任何
//      `extern ... fAutoTeach` 宣告。
//      ⚠ 說清楚免得下一波誤判：`fAutoTeach` 這個字串在樹上有 97 個
//      文字命中，但**每一個都在 `#if 0` GATE 區塊裡**
//      （例：ainarm9045.cpp:5071 在 `#if 0 // GATE W7D-K2-G02` 內、
//      aoutarm.cpp:4152 在 `#if 0 // GATE G13` 內）。
//      grep 找得到字串不代表編譯器看得到。
//      所以就算允許改既有行，這支現在也翻不完整。
//    * golden UseInArmSetupTeach（:2955-2974）相依都在，見 (O-8)。
//  本波受 append-only 約束不動這兩行，記錄在此供後續波次接手。
// =============================================================================

// ---------------------------------------------------------------------------
//  Facade-local widget subclasses（DEVIATION D-3 / D-4）
// ---------------------------------------------------------------------------
class TfOffSetPageControl : public vclcompat::TPageControl
{
public:
    // golden PageControl1->ActivePage=<TTabSheet*>（:3366/:3372/:3379/
    // :3385/:3548）。vclcompat::TPageControl 只有 ActivePageIndex。
    // 與 forms/fConfiguration.h:280 同形 —— 只存值，不切頁。
    vclcompat::TTabSheet *ActivePage = nullptr;
    virtual ~TfOffSetPageControl() {}
};

class TfOffSetPanel : public vclcompat::TPanel
{
public:
    // golden Pnl_ScanAOI->Height=81/33（:3271/:3275）。
    // vclcompat::TControl 帶 Left/Top 但沒有 Height —— 只存值。
    int Height = 0;
    virtual ~TfOffSetPanel() {}
};

// ===========================================================================
//  TfOffSet -- non-VCL stub (golden cOffSet.h).  Both methods return false
//  offline (no auto-offset / no setup-teach configured).
// ===========================================================================
class TfOffSet
{
public:
    virtual bool UseAutoOffsetFunction(AnsiString sName);  // [METHOD] golden cOffSet.h:488 -> false
    virtual bool UseInArmSetupTeach(int iWhich);           // [METHOD] golden cOffSet.h:489 -> false
    virtual ~TfOffSet() {}

    // =======================================================================
    //  AI(W906-FW-NOTE-W33) 20260826 ADD -- cOffSet Wave A.
    //  Everything from here to the closing brace is new; the two lines above
    //  are UNTOUCHED W6.2 content (see the file-head banner for their status).
    //
    //  WIDGET FIELDS -- golden cOffSet.h 宣告的型別逐一對照，只放本波
    //  22 支真的碰到的。命名＝dfm leaf name（facade 規則 4）。
    //  用 in-class initialiser 配置（forms/fSetup.h:477-499 慣例），所以
    //  既有的隱式預設建構子照用，不需要新增或修改 ctor。
    // =======================================================================
    TSpeedButton *sbtExit           = new TSpeedButton();   // golden cOffSet.h:26
    TSpeedButton *btnBack           = new TSpeedButton();   // golden cOffSet.h:211
    TSpeedButton *btnToIndexOffset  = new TSpeedButton();   // golden cOffSet.h:31
    TSpeedButton *btnToArmOffset    = new TSpeedButton();   // golden cOffSet.h:174
    TSpeedButton *sbZcalibration    = new TSpeedButton();   // golden cOffSet.h:396

    TfOffSetPageControl *PageControl1 = new TfOffSetPageControl();  // golden cOffSet.h:19 (TPageControl) -- D-3
    TTabSheet *tsInOutArmOffset     = new TTabSheet();      // golden cOffSet.h:20
    TTabSheet *tsIndexOffset        = new TTabSheet();      // golden cOffSet.h:21
    TTabSheet *tsZCalibration       = new TTabSheet();      // golden cOffSet.h:393

    TEdit *IndexArmOffSet1          = new TEdit();          // golden cOffSet.h:196
    TEdit *IndexArmOffSet3          = new TEdit();          // golden cOffSet.h:195

    TCheckBox     *ck_ScanAOI       = new TCheckBox();      // golden cOffSet.h:154
    TfOffSetPanel *Pnl_ScanAOI      = new TfOffSetPanel();  // golden cOffSet.h:152 (TPanel) -- D-4
    TLabel *lbFinalAirForce         = new TLabel();         // golden cOffSet.h:207

    // SetXYPitchVCLVisible (golden :856-882) 用到的十一顆 widget
    TEdit  *edPitchX1               = new TEdit();          // golden cOffSet.h:56
    TEdit  *edPitchX2               = new TEdit();          // golden cOffSet.h:111
    TEdit  *edPitchX3               = new TEdit();          // golden cOffSet.h:266
    TEdit  *edPitchX4               = new TEdit();          // golden cOffSet.h:265
    TEdit  *edPitchY                = new TEdit();          // golden cOffSet.h:112
    TLabel *lblPitchX2              = new TLabel();         // golden cOffSet.h:109
    TLabel *lblPitchX3              = new TLabel();         // golden cOffSet.h:263
    TLabel *lblPitchX4              = new TLabel();         // golden cOffSet.h:264
    TLabel *lblPitchY               = new TLabel();         // golden cOffSet.h:107
    TLabel *lblPitchX2Range         = new TLabel();         // golden cOffSet.h:110
    TLabel *lblPitchYRange          = new TLabel();         // golden cOffSet.h:108

    //  [DATA] golden cOffSet.h:477 int iIndexChange; -- kevin 20211211
    //  "index socket value change" 旗標。golden 的 ctor（cOffSet.cpp:186-440）
    //  沒有初始化它；VCL 的 TComponent 是 zero-filled 配置，所以 golden 的
    //  初值同樣是 0。這裡把 0 寫明，語意與 golden 相同。
    //  0 選到什麼：golden 只在 cOffSet.cpp 內部寫它（:2885 =3、:3236 =1、
    //  :3261 =2），讀它的是 spbSaveClick / ShowOneByOneOffSet 等本波未翻的
    //  支，所以 offline 只有本波三支寫入者會動它，沒有消費者。
    int iIndexChange = 0;

    // DEVIATION (D-2): golden TForm::Close() -- 永久 offline no-op（無視窗）。
    // 唯一到得了的呼叫點是本波的 sbtExitClick。
    void Close() {}

    // -----------------------------------------------------------------------
    //  METHODS -- 22 支，全部唯讀方向。每一支的 golden span 寫在行末。
    //  event handler 本體有翻但**不接線**（facade 規則 3）：本樹沒有事件
    //  來源，這 22 支今天全樹 0 個 caller（重跑 20260826）。
    // -----------------------------------------------------------------------
    void       SetXYPitchVCLVisible(bool bVisible);              // golden cOffSet.cpp:856-882
    void       edArmXKeyPress(char &Key);                        // golden cOffSet.cpp:2792-2795
    void       sbtExitClick();                                   // golden cOffSet.cpp:2797-2801
    void       IndexArmOffSet3MouseDown(TEdit *Sender);          // golden cOffSet.cpp:2881-2886
    void       IndexArmOffSet5MouseDown(TEdit *Sender);          // golden cOffSet.cpp:3049-3053
    void       edShtFor2DMouseDown(TEdit *Sender);               // golden cOffSet.cpp:3055-3086
    void       edPreciserOpenMouseDown(TEdit *Sender);           // golden cOffSet.cpp:3088-3092
    void       IndexArmOffSet1KeyDown();                         // golden cOffSet.cpp:3220-3227
    void       IndexArmOffSet3KeyDown();                         // golden cOffSet.cpp:3229-3237
    void       IndexArmOffSet1KeyUp();                           // golden cOffSet.cpp:3239-3247
    void       IndexArmOffSet3KeyUp();                           // golden cOffSet.cpp:3249-3257
    void       IndexArmOffSet3Change();                          // golden cOffSet.cpp:3259-3262
    void       ck_ScanAOIClick();                                // golden cOffSet.cpp:3264-3278
    void       edOffsetContactForceMouseDown(TEdit *Sender);     // golden cOffSet.cpp:3341-3345
    void       ShowFinalAirForce();                              // golden cOffSet.cpp:3347-3350
    void       btnBackClick();                                   // golden cOffSet.cpp:3363-3367
    void       btnToIndexOffsetClick();                          // golden cOffSet.cpp:3376-3380
    void       btnToArmOffsetClick();                            // golden cOffSet.cpp:3382-3386
    AnsiString Tri_Position_Offset();                            // golden cOffSet.cpp:3526-3543
    void       sbZcalibrationClick();                            // golden cOffSet.cpp:3545-3549
    int        LoadASECLOffsetIndexValues(int iType, int iPos);  // golden cOffSet.cpp:3956-3994
    void       edLodXClick(TEdit *Sender);                       // golden cOffSet.cpp:3997-4000

    // -----------------------------------------------------------------------
    //  GATED -- 宣告在此、forms/fOffSet.cpp **不定義**，由 linker 當互鎖
    //  （forms/fMotorTest.h 先例）。理由逐支見檔頭 GATE REGISTER。
    // -----------------------------------------------------------------------
    void       CheckBox1Click();                                 // GATE (O-1) golden :2787-2790
    void       btnOffsetListClick();                             // GATE (O-2) golden :3369-3374
    void       FormDestroy();                                    // GATE (O-3) golden :3094-3098
    AnsiString GetOffsetPath(AnsiString FileFolder = "");        // GATE (O-4) golden :1426-1462
    void       sb_AutoOffsetUpClick();                           // GATE (O-5) golden :2888-2899
    void       sb_AutoOffsetDownClick();                         // GATE (O-5) golden :2901-2912
    void       sb_AutoOffsetRightClick();                        // GATE (O-5) golden :2914-2925
    void       sb_AutoOffsetLeftClick();                         // GATE (O-5) golden :2927-2938
    void       ClearIndexOffset();                               // GATE (O-6) golden :1817-1853
    void       edOffsetContactForceChange();                     // GATE (O-7) golden :3352-3361
    bool       UseOutArmSetupTeach(int iArea);                   // GATE (O-8) golden :2976-3005
    void       btnSortAuto1Click(TButton *Sender);               // GATE (O-9) golden :3207-3217
};
extern TfOffSet *fOffSet;

// AI(W906-FW-NOTE-W33) 20260826: bhasKeyDown 是 golden 的 **file-scope
// global**（cOffSet.cpp:3219 bool bhasKeyDown=false;，就寫在
// IndexArmOffSet1KeyDown 的上一行），不是 TfOffSet 的成員 —— 本波開
// golden cOffSet.h 逐行確認過（cOffSet.h 的 private/public 區塊都沒有它）。
// forms/fOffSet.cpp 是本樹對 golden cOffSet.cpp 的 stand-in，所以
// cOffSet.cpp 的 file-scope 資料歸在這裡；真正的 cOffSet.cpp port 落地時，
// 這個定義的所有權隨之搬走（同 forms/fNote.cpp 對 iPosition 的處置）。
// 宣告成 extern（而不是只定義在 .cpp）是因為它是四支 KeyDown/KeyUp 的
// 唯一判別子，測試要能驅動它。
// 全樹唯一的寫入者就是那四支（golden :3225/:3234/:3245/:3255），
// 而四支都 gated 在 CUSTOMER_CODE==CC_KYEC_LEE 之後，所以非 KYEC 機台上
// 它永遠是 false。
extern bool bhasKeyDown;

#endif // FORMS_FOFFSET_H
