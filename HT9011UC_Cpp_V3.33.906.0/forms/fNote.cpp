// =============================================================================
//  forms/fNote.cpp  --  definitions for the fNote facade
//
//  AI(W906-W7-F0) 20260728: split out of FormsFacade.cpp by the W7-F0 refactor
//  (docs/W7_UI_ARCHITECTURE_PLAN.md SS6-F0-d).  Body moved VERBATIM.
// =============================================================================
#include "forms/fNote.h"

// AI(W906-W7-L2) 20260803: cmydef.h is the ONLY new include, and it covers all
// three external things golden's IsTestSitICFallDown body reads:
//   * `bool bIsTestSitICFallDown`      -- cmydef.h:2723 (defined cmydef.cpp:2915)
//   * MTestY1/MTestZ1/MTestZ2/MTestY2  -- cmydef.h:2114-2117 (cmydef.cpp:2347-2350)
//     MOutShuttle1/MOutShuttle2        -- cmydef.h:2118/:2120 (cmydef.cpp:2351/2353)
//     MMIndex                          -- cmydef.h:2401 (cmydef.cpp:2695)
//   * `HT9045_CONFIG IniConfig`        -- Config.h:1499, reached transitively
//     (cmydef.h:10 -> cprod.h:6 -> Config.h); defined cprod.cpp:50.
// NO CMakeLists CHANGE IS NEEDED FOR THIS: every one of those symbols lives in
// cmydef.cpp / cprod.cpp, i.e. in ht9045_globals, which ht9045_forms already
// declares as a link dependency -- the statement is `target_link_libraries(
// ht9045_forms PUBLIC vclcompat ht9045_globals)` at CMakeLists.txt:617, and this
// file is already in that target's source list (`forms/fNote.cpp`, :574), both as
// of this pass.  The "DEPENDENCIES -- and why they are acyclic" block that opens
// at :509 records why that edge is safe.  (Line numbers there will shift when the
// integrator edits CMakeLists.txt; the quoted statement text is the stable key.)
// forms/fMain.cpp already sets the precedent of including
// cprod.h/cmydef.h from this library.  No new translation unit is created.
#include "cmydef.h"

// --- W906-AutoCleanFoundation ADD: TfNote (first home, see forms/fNote.h) --
// AI(W906-W7-L2) 20260803: init list extended with the three new members, in
// DECLARATION ORDER (fShow, AlarmType, edErrorCode) so no -Wreorder.  The four
// pre-existing initialisers are unchanged.  fShow(false) is golden note.cpp:164;
// AlarmType(0) is golden note.cpp:218; edErrorCode is allocated because golden's
// is a real TEdit created by the .dfm loader and IsTestSitICFallDown
// dereferences it unconditionally (golden note.cpp:5472) -- a null here would
// be a crash, not a default.  vclcompat::TEdit's own Text defaults to "".
TfNote::TfNote()
    : bMyServoOffInArm(false), iMyServoOffInArmPosX(0), iMyServoOffInArmPosY(0), aJamCodeFilePath(""),
      fShow(false), AlarmType(0), edErrorCode(new vclcompat::TEdit()),
      Edit3(new vclcompat::TEdit()), edUnitName(new vclcompat::TEdit())   // AI(W906-FW-SecUnlock) 20260819: see header
{
}
TfNote *fNote = new TfNote();

// AI(W906-W7-L2) 20260803: golden note.cpp:83 `int iPosition;` -- see the
// declaration comment in forms/fNote.h for who writes it in golden
// (ShowErrorUnit, golden note.cpp:4511/:4514, no ported home) and for what the
// zero value selects.  Written `=0` explicitly rather than relying on static
// zero-init, to make the value reviewable at the point of definition; golden's
// bare `int iPosition;` has the identical initial value.
int iPosition = 0;

// ---------------------------------------------------------------------------
//  AI(W906-W7-L2) 20260803: FAITHFUL translation of golden note.cpp:5462-5492
//  `bool __fastcall TfNote::IsTestSitICFallDown()`.  Structure, operand order,
//  the redundant `else { bReturn=false; }` and the separate second `if` block
//  are all preserved verbatim; only `__fastcall` is dropped (tree-wide idiom)
//  and `edErrorCode` now resolves to the vclcompat TEdit stand-in.  Golden's
//  own comments are carried over on the lines they annotate.
//
//  WHAT ITS RETURN VALUE SELECTS IN ScanPannelKey -- the branch justification
//  this facade owes.  The method is called at golden ckernel.cpp:2009, :2040,
//  :2186 and :2217, always as `if(fNote->IsTestSitICFallDown()==false)`:
//    * false OPENS the plain arms (:2011-2015 FKRetry, :2042-2046 FKSkip,
//      :2188-2192 RKRetry, :2219-2223 RKSkip) -- Retry/Skip are accepted with
//      no extra condition.
//    * true selects the else arms (:2017-2027, :2048-2058, :2194-2204,
//      :2225-2235), where the SAME key is accepted only when
//      IniConfig.bD40IndexICFallDownMustPressFMotorDown==false, i.e. the
//      "after an index drop you must press the motor-down key first" lockout.
//  OFFLINE, this body returns false unless a test drives it, and it does so
//  through a REAL evaluation, not a hard-coded return:
//    - bIsTestSitICFallDown is genuinely live in this tree: a tree-wide grep
//      outside build*/ counts 22 `bIsTestSitICFallDown=true;` writers, incl.
//      ported acarry.cpp:2399, atester.cpp:2753 and AutoClean/AutoClean.cpp:
//      5862, so half of every conjunct really does move;
//    - iPosition is the discriminator and offline stays 0 == MInArmX
//      (ported cmydef.cpp:2334 `const int MInArmX       =0;`; golden
//      cmydef.cpp:2330), which matches none of the seven motor ids tested, so
//      arms 1 and 3 stay closed;
//    - edErrorCode->Text stays "" so arm 2 stays closed.
//  Note the consequence honestly: with iPosition's only golden writer
//  (ShowErrorUnit) untranslated, NOTHING in the ported tree can currently make
//  this return true on its own -- a test must set iPosition (or the Text) to
//  walk the lockout arms.  Both are public/extern precisely so it can.
//  Being false by default is also the behaviour-preserving choice for the
//  common case: when bD40IndexICFallDownMustPressFMotorDown is false the two
//  arms are observationally identical anyway.
// ---------------------------------------------------------------------------
bool TfNote::IsTestSitICFallDown()                                              //jou 2011-12-20 修正曾經發生過掉料的話,note訊息沒辦法操作的問題
{
    bool bReturn=false;
    if(bIsTestSitICFallDown==true &&
       (iPosition==MTestY1 || iPosition==MTestZ1 ||
        iPosition==MTestY2 || iPosition==MTestZ2 ||
        iPosition==MMIndex))
    {
        bReturn=true;
    }
    else if(bIsTestSitICFallDown==true && (edErrorCode->Text=="MES1640" || edErrorCode->Text=="MES1641"))               //JerryYang 20220923 : 掉料後one cycle finish要開門按z1
    {
        bReturn=true;
    }
    else
    {
        bReturn=false;
    }

    if(IniConfig.bOutShLoseNeedOpenChamber==true ||                             //jou 2013-12-12 Out Shuttle Lose Device Need Open Chamber Door and press Z1
       IniConfig.bF24OutShuttleLoseIcOpenIndexDoor)                             //kevin 20180725 add
    {
        if(bIsTestSitICFallDown==true &&
           (iPosition==MOutShuttle1 || iPosition==MOutShuttle2))
        {
            bReturn=true;
        }
    }

    return bReturn;
}

// AI(W906-W7-L2) 20260803: SHIM RETIREMENT DEBT, REPORTED NOT DISCHARGED.
// Two offline stand-ins for fNote->fShow now have a real member to point at,
// but both live in files this wave does not own, so neither is touched here:
//   * atester_32Site.cpp:302  `#define W5_32S_FNOTE_FSHOW (false)`, consumed at
//     atester_32Site.cpp:2835.  Retiring it means `if(fNote->fShow) break;`.
//   * Automation/AGV_PortScan.h:61-70 records TfAGV::Timer2Timer as
//     deliberately untranslated *because* fNote had no home; per that same
//     comment its guard is `if(!fNote->fShow || InitialOK==false) return;` and
//     its golden span is Automation/AGV.cpp:1290-1307.  That span was AGV_
//     PortScan.h's own claim and is now VERIFIED against golden: :1290 is
//     `void __fastcall TfAGV::Timer2Timer(TObject *Sender)`, the guard is split
//     over :1294 `if(!fNote->fShow || InitialOK==false)` + :1295 `return;`, and
//     :1307 is the closing brace.  The stated reason for deferring it has now
//     expired.
// Both are behaviour-identical to the new member's offline value (false), so
// retiring them is a pure cleanup with no behaviour change -- which is exactly
// why it is safe to defer to the owning wave rather than reach across files.

// =============================================================================
//  AI(W906-FW-NOTE-W33) 20260826: note.cpp Wave A -- 6 支唯讀方向 methods。
//  這行以上全部是既有內容（W7-F0 / W7-L2 / FW-SecUnlock），一行未動。
//  波次範圍、分母重量方式、DEVIATION 與完整 GATE REGISTER 都在
//  forms/fNote.h 的檔頭 banner，這裡不複製第二份（避免兩份漂移）。
//
//  唯一新增的 include 是 forms/fQwertyKey.h：class TfQwertyKey 在
//  forms/fQwertyKey.h:294，裸全域 fQwertyKey 宣告在 :406、定義在
//  forms/fQwertyKey.cpp:41，ShowQwertyKey 宣告在 :370。
//  forms/fQwertyKey.cpp 與本檔同屬 ht9045_forms（CMakeLists.txt:699 vs
//  :660），所以沒有新的 link edge，也沒有新的 translation unit。
//  cmydef.h 本檔在 :27 早已 include（W7-L2），PosCodeToStr 的 90 個馬達
//  常數全部由它提供 —— 本波逐一比對 cmydef.h 的 extern 與 cmydef.cpp 的
//  `const int` 定義，90 個全在、0 個缺。
// =============================================================================
#include "forms/fQwertyKey.h"

// ---------------------------------------------------------------------------
//  golden note.cpp:158-159 的兩個 file-scope global，見 forms/fNote.h 的
//  宣告註解。golden 的 `TDateTime LastInputTime;` 沒有初始化子，
//  vclcompat::TDateTime 的預設建構子把 value_ 設 0.0（TDateTime.h:35），
//  與 BCB6 TDateTime 的預設 0 相同。IsScanning 照 golden 原字寫 =false。
// ---------------------------------------------------------------------------
TDateTime LastInputTime;             // golden note.cpp:158
bool      IsScanning = false;        // golden note.cpp:159

// ---------------------------------------------------------------------------
//  golden note.cpp:5764-5767 -- `TObject *Sender` 直接宣告成 TEdit*
//  （golden 自己就是 cast 目標），N-D1。
//  ⚠ ShowQwertyKey 的 offline 語意見 forms/fQwertyKey.h BEHAVIOUR NOTE：
//  ShowModal() 是 no-op，等於「使用者開了鍵盤立刻送出」。這裡
//  bCheckRange=true 且 N_INTEGER，所以淨效果是把 Sender->Text 過一次
//  CheckRange(0..99999999) 再寫回 —— 非數字會變成 0。那是 golden 自己的
//  submit 路徑，不是本移植的發明。
//  ⚠ fQwertyKey 是裸全域指標，全樹唯一建立點是 Public/HTEdit.cpp 的
//  lazy new，所以平時是 NULL；本支今天全樹 0 個 caller（不接線，
//  facade 規則 3），所以到不了。**安全是因為沒接線，不是因為有守衛** ——
//  與 forms/fSetup.h GATE (WA-1) 已開閘的兄弟站點同一曝露。
// ---------------------------------------------------------------------------
void TfNote::edtLotCountClick(vclcompat::TEdit *Sender)
{
    fQwertyKey->ShowQwertyKey(Sender, N_INTEGER, 0, true, 0, 99999999);
}

// golden note.cpp:5769-5772 -- 同上 N-D1 / ShowQwertyKey 說明。
// 這支只帶兩個實參，其餘走 ShowQwertyKey 的預設值
// （iDP=0, bCheckRange=false, min=0, max=0，forms/fQwertyKey.h:370），
// 與 golden 的呼叫形狀完全相同 —— bCheckRange=false 表示文字原字來回，
// 淨效果為無。
void TfNote::edtLotIDClick(vclcompat::TEdit *Sender)
{
    fQwertyKey->ShowQwertyKey(Sender, N_NO_SYMBOL|N_NO_SPACE);
}

// ---------------------------------------------------------------------------
//  golden note.cpp:5856-5862 -- `TObject *Sender, TMouseButton Button,
//  TShiftState Shift, int X, int Y` 全部 dropped（本體從未讀取），N-D1。
//  golden 透過全域 `fNote->` 取自己的成員（不是 this->），照原字保留 ——
//  fNote 就是本檔 :43 定義的那顆全域，語意相同。
//  ⚠ offline：SendToBack() 是 no-op（N-D3，本樹沒有 Z 序），
//  兩個 Visible=false 只是存值，畫面上不會有東西消失 —— 沒有畫面。
// ---------------------------------------------------------------------------
void TfNote::PanelMain6MouseDown()
{
    fNote->strngrdMDBQuery->SendToBack();
    fNote->strngrdMDBQuery->Visible=false;
    fNote->btCloseMDBQuery->Visible=false;
}

// golden note.cpp:5864-5869 -- 與上一支本體逐字相同（golden 自己就寫了兩份
// 一模一樣的），照翻，不合併。`TObject *Sender` dropped（N-D1）。
void TfNote::btCloseMDBQueryClick()
{
    fNote->strngrdMDBQuery->SendToBack();
    fNote->strngrdMDBQuery->Visible=false;
    fNote->btCloseMDBQuery->Visible=false;
}

// ---------------------------------------------------------------------------
//  golden note.cpp:6530-6612 -- PosCodeToStr（宣告 golden note.h:457）
//  **純函式**：吃一個 motor/unit id，回傳人看得懂的區域名稱。
//  不碰 widget、不碰全域狀態、不開檔、不動機台。本波唯一一支大支的交付。
//
//  相依：90 個 `const int` 馬達／單元代號，全部在 cmydef.h（extern）＋
//  cmydef.cpp（定義）。本波逐一比對，90 個全在、0 個缺 ——
//  掃法是把 golden :6530-6612 剝掉字串字面值後抽出所有 `M[A-Za-z_]\w*`
//  識別字（第一次掃忘了剝字串，把 "ManualTray1" 這種**字串內容**誤當符號，
//  多報 7 個假缺口；剝掉之後 0 缺）。
//
//  ⚠ GOLDEN ODDITY，照翻並記錄：第一條 `if(Pos<=MInArmZH) sRet="In Arm";`
//  是**沒有下界**的。MInArmZH==10（cmydef.cpp:2344），所以任何負數 Pos
//  也會被歸成 "In Arm"。golden 這樣寫，本移植不加下界。
//
//  ⚠ 另一個 golden 事實，值得未來波次知道：這 83 行的 else-if 連鎖只有
//  ~90 層，遠低於 MSVC 的巢狀上限，**不會**重演 EJ1N/TextProcess.cpp:404-424
//  的 C1061（那是 127 層）。所以這一支不會擋住 MSVC 次 oracle。
// ---------------------------------------------------------------------------
AnsiString TfNote::PosCodeToStr(int Pos)
{
    AnsiString sRet="Unknown Area";
    if(     Pos<=MInArmZH)                      sRet="In Arm";
    else if(Pos==MInShuttle1)                   sRet="In Shuttle1";
    else if(Pos==MInShuttle2)                   sRet="In Shuttle2";
    else if(Pos==MTestY1 || Pos==MTestZ1)       sRet="Index Arm";
    else if(Pos==MTestY2 || Pos==MTestZ2)       sRet="Index Arm";
    else if(Pos==MOutShuttle1)                  sRet="Out Shuttle1";
    else if(Pos==MOutShuttle2)                  sRet="Out Shuttle2";
    else if(Pos>=MOutArmX && Pos<=MOutArmZH)    sRet="Out Arm";
    else if(Pos==MTrayX)                        sRet="TrayX Car";
    else if(Pos==MMTrayZ  || Pos==MMTrayY_Car)  sRet="TrayZ Car";
    else if(Pos==MMEmptyZ || Pos==MMEmpty_Car)  sRet="Empty Car";
    else if(Pos==MMEmpty1_Car)                  sRet="Empty1 Car";
    else if(Pos==MMColorZ || Pos==MMColor_Car)  sRet="Color Car";
    else if(Pos==MMAuto1Z || Pos==MMAuto1_Car)  sRet="Auto1 Car";
    else if(Pos==MMAuto2Z || Pos==MMAuto2_Car)  sRet="Auto2 Car";
    else if(Pos==MMAuto3Z || Pos==MMAuto3_Car)  sRet="Auto3 Car";
    else if(Pos==MMAuto4Z || Pos==MMAuto4_Car)  sRet="Auto4 Car";
    else if(Pos==MMAuto5Z || Pos==MMAuto5_Car)  sRet="Auto5 Car";
    else if(Pos==MMAuto6Z || Pos==MMAuto6_Car)  sRet="Auto6 Car";
    else if(Pos==MManualTray1)                  sRet="ManualTray1";
    else if(Pos==MManualTray2)                  sRet="ManualTray2";
    else if(Pos==MManualTray3)                  sRet="ManualTray3";
    else if(Pos==MManualTray4)                  sRet="ManualTray4";
    else if(Pos==MManualTray5)                  sRet="ManualTray5";
    else if(Pos==MManualTray6)                  sRet="ManualTray6";
    else if(Pos==MManualTrayAll)                sRet="ManualTrayAll";
    else if(Pos==MMTrayY)                       sRet="TrayY";
    else if(Pos==MMPlate1)                      sRet="Plate 1";
    else if(Pos==MMPlate2)                      sRet="Plate 2";
    else if(Pos==MMAuto1)                       sRet="Auto 1";
    else if(Pos==MMAuto2)                       sRet="Auto 2";
    else if(Pos==MMAuto3)                       sRet="Auto 3";
    else if(Pos==MMAuto4)                       sRet="Auto 4";
    else if(Pos==MMAuto5)                       sRet="Auto 5";
    else if(Pos==MMAuto6)                       sRet="Auto 6";
    else if(Pos==MMEmpty)                       sRet="Empty";
    else if(Pos==MMColor)                       sRet="Color";
    else if(Pos==MMEmpty1)                      sRet="Empty1";
    else if(Pos==MMSystem)                      sRet="System";
    else if(Pos==MMSafeDoor1)                   sRet="Safe Door1";
    else if(Pos==MMSafeDoor2)                   sRet="Safe Door2";
    else if(Pos==MMSafeDoor3)                   sRet="Safe Door3";
    else if(Pos==MMSafeDoor4)                   sRet="Safe Door4";
    else if(Pos==MMSafeDoor5)                   sRet="Safe Door5";
    else if(Pos==MMSafeDoor6)                   sRet="Safe Door6";
    else if(Pos==MMSafeDoor7)                   sRet="Safe Door7";
    else if(Pos==MMSafeDoor8)                   sRet="Safe Door8";
    else if(Pos==MMSafeDoor9)                   sRet="Safe Door9";
    else if(Pos==MMSafeDoor10)                  sRet="Safe Door10";
    else if(Pos==MMIonFan01)                    sRet="Ion Fan1";                //Steven 20130205 : 離子風扇異常顯示
    else if(Pos==MMIonFan02)                    sRet="Ion Fan2";
    else if(Pos==MMIonFan03)                    sRet="Ion Fan3";
    else if(Pos==MMIonFan04)                    sRet="Ion Fan4";
    else if(Pos==MMIonFan05)                    sRet="Ion Fan5";
    else if(Pos==MMIonFan06)                    sRet="Ion Fan6";
    else if(Pos==MMIonFan07)                    sRet="Ion Fan7";
    else if(Pos==MMIonFan08)                    sRet="Ion Fan8";
    else if(Pos==MMIonFan09)                    sRet="Ion Fan9";
    else if(Pos==MMIonFan10)                    sRet="Ion Fan10";
    else if(Pos==MMIonFan11)                    sRet="Ion Fan11";
    else if(Pos==MMIonFan12)                    sRet="Ion Fan12";
    else if(Pos==MMATC_Handler)                 sRet="ATC Handler";
    else if(Pos==MMATC_TCPIP)                   sRet="ATC TCPIP";
    else if(Pos==MMATC_NI)                      sRet="ATC NI";
    else if(Pos==MMATC_ATC)                     sRet="ATC";
    else if(Pos==MMATC_Chiller)                 sRet="ATC Chiller";
    else if(Pos==MMATC_RS232)                   sRet="ATC RS232";
    else if(Pos==MMATC_Head)                    sRet="ATC Head";
    else if(Pos==MMATC_PowerSupply)             sRet="ATC PowerSupply";
    else if(Pos==MMATC_WaterValve)              sRet="ATC WaterValve";
    else if(Pos==MMInShuttle)                   sRet="In Shuttle";
    else if(Pos==MMOutShuttle)                  sRet="Out Shuttle";
    else if(Pos==MMIndex)                       sRet="Index";
    else if(Pos==MMTemperature)                 sRet="Temperature";
    else if(Pos==MMInterface)                   sRet="Interface";
    else if(Pos==MMScanner)                     sRet="Scanner";
    else if(Pos==MMCCD)                         sRet="CCD";
    else if(Pos==MMOCR)                         sRet="OCR";                     //ChungHung 20120830 add OCR Function add
    return sRet;
}

// ---------------------------------------------------------------------------
//  golden note.cpp:6648-6674 -- edBundleIDChange
//  `TObject *Sender` dropped（本體從未讀取），N-D1。
//  純鍵入節奏判別：兩次字元輸入間隔 >100ms 視為人手打字，<=100ms 視為
//  掃碼槍連續輸入；人手打字就把欄位清空。不開檔、不動機台、不送命令。
//  ⚠ DEVIATION (N-D2)：golden 寫 `NowTime.Val` / `LastInputTime.Val`
//  （BCB6 System::TDateTime 的 public 資料成員）；vclcompat::TDateTime 把
//  Val 做成 method（vclcompat/TDateTime.h:44），所以這裡是 `.Val()`。
//  取值語法不同，數值語意相同（都是 OLE 日期序列值）。
//  ⚠ golden :6650 先取一次 Now()，:6654 的早退臂又取第二次 Now()，
//  兩次時間點不同 —— 照翻，不合併成一次。
// ---------------------------------------------------------------------------
void TfNote::edBundleIDChange()
{
    TDateTime NowTime = Now();

    if(edBundleID->Text.Length()<=1)                                            // 一開始輸入第一個字元時，不處理
    {
        LastInputTime=Now();
        return;
    }

    double Interval=(NowTime.Val()-LastInputTime.Val())*24*60*60*1000.0;        // 毫秒
    LastInputTime=NowTime;

    if(Interval>100)                                                            // 超過100毫秒才輸入下一個字元，視為人工手動輸入
    {
        IsScanning=false;
    }
    else                                                                        // 小於100毫秒，視為條碼機掃描輸入
    {
        IsScanning=true;
    }

    if(IsScanning==false)
    {
        edBundleID->Text="";
    }
}

// =============================================================================
//  GATED -- forms/fNote.h 宣告的 27 支在本檔**刻意不定義**，由 linker 當
//  互鎖（forms/fMotorTest.h 先例）：任何未來波次一旦接線呼叫它們，會在
//  link 階段炸出 undefined reference，而不是靜默跑一個空殼。
//  分組理由逐條見 forms/fNote.h 的 GATE REGISTER (N-1)..(N-17)。
//  摘要：
//    (N-1)(N-2)(N-4)  會啟動／重置／移動機台
//    (N-3)(N-5)       呼叫未翻的 modal 路徑或暫停機台 + 寫 log
//    (N-6)(N-7)       對外傳輸／寫檔
//    (N-8)            LogSoftwareOffTime 在 ht9045_sm
//    (N-9)            TTMyTray / TMediaPlayer 無家
//    (N-10)           myBinCodeEdit[256] 無法在 append-only 下配置；Focused() 無意義
//    (N-11)           cDark / clTeal / TColor 無家
//    (N-12)           MOT[] 在 ht9045_motor
//    (N-13)           TBtnPanel 無家，且寫 bLamp* 實體面板燈
//    (N-14)           解鎖路徑，offline 密碼比對退化
//    (N-15)(N-16)     ShellExecute / Canvas / TreeView / MDB 查詢
//    (N-17)           TList 只在 aHotPlateSubstrate.h（兩個 TMyKitSuck ODR 陷阱）
// =============================================================================
