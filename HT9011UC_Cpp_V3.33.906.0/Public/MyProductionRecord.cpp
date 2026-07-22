// =============================================================================
//  Public/MyProductionRecord.cpp
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/Public/MyProductionRecord.cpp
//  Translation wave: AI(W906-MyProductionRecord) 20260721
//
//  SCOPE THIS WAVE (golden .cpp:218-654, ~34 methods): the ctor,
//  InitialRecord, DeleteProductionRecord, AddArmSiteRecord, and the
//  Add*Record/Get* leaves that purely read/write asBuffer (TStringList*,
//  indexed by eMyProdRec, Public/MyProductionRecord.h). Every external
//  symbol this slice touches was re-verified against the CURRENT target
//  tree (not assumed from an earlier recon) -- see the per-method notes
//  below for anything that turned out different from plan.
//
//  OUT OF SCOPE (golden .cpp:660-1384, left undeclared in the header --
//  see its own file-head banner): SaveRecord/SaveDataForASE/
//  SaveDataForGreatek/SaveGroundESDData/SaveRecordCleanPad/
//  SaveRecordForLeadYo/Save2DIDForJCET/SaveJHT_Record/GetProdLogFileName/
//  GetProdLogFilePath. AddIndexPickVacuum (golden .cpp:1110) also stays out
//  (physically inside that heavy block; see header note).
//
//  AddErrorRecord (golden .cpp:561-573) IS declared in the header (pre-
//  existing) but is DELIBERATELY NOT DEFINED here. Golden's body is:
//      if(bAddPosition) { ...5 asBuffer writes... }
//      SaveRecord(ErrorLog);
//  The position-recording half is pure/in-scope; the tail call is into
//  SaveRecord(), which is the OUT-OF-SCOPE heavy half above. This mirrors
//  the codebase's own established "Hard boundary" precedent for a leaf that
//  calls into a still-deferred sibling -- see atester_32Site.cpp's
//  W5_32S_ADD_TEST_RESULT_RECORD/W5_32S_SEAM ("DECLARED, no body anywhere ->
//  would be undefined-reference") and aTester_Rear.cpp:2022/2307 +
//  BarCode_Bottom2DID8CCD.cpp:1372's "#if 0 // TODO(W7) ... (TMyProductionRecord
//  has no .cpp yet)" call-site gates for this exact same method. Per this
//  wave's own brief, the choice here is the SECOND of the two offered options
//  ("gating just this one method alongside the heavy half") rather than
//  inventing a forward-declared SaveRecord() no-op stub: SaveRecord has ZERO
//  declaration anywhere in the target header today, and adding one (even as
//  a temporary stub) would prematurely stake out the heavy half's surface
//  that this wave's brief explicitly says to leave untouched. Leaving
//  AddErrorRecord itself undefined keeps that boundary exactly as crisp as
//  before (same undefined-symbol status quo for its existing unguarded call
//  sites in acarry.cpp/csystem.cpp/etc. -- no better, no worse). Its sibling
//  AddErrorRecordNoSave (golden .cpp:575-587, no SaveRecord call) has NO such
//  problem and gets a real body below.
//
//  KNOWN CONCURRENT-WAVE COLLISION -- READ BEFORE WIRING INTO ANY CMAKE
//  TARGET: a same-day sibling wave, AI(W906-AutoCleanFoundation) 20260721,
//  independently added its OWN no-op stand-in DEFINITIONS for 9 of these
//  same methods directly inside aHotPlateSubstrate.cpp (verified via git diff
//  while this wave was in flight):
//      TMyProductionRecord::TMyProductionRecord()            (allocates asBuffer only)
//      TMyProductionRecord::AddErrorRecord(...)               (no-op)
//      TMyProductionRecord::AddHPRecord(...)                  (no-op)
//      TMyProductionRecord::AddInArmHotplatePickRecord(...)   (no-op)
//      TMyProductionRecord::AddTestRecord(...)                (no-op -- see below, INTENTIONALLY not redefined here)
//      TMyProductionRecord::GetInRotationAngRecord()          (returns "")
//      TMyProductionRecord::AddPickCleanPad(...)              (no-op)
//      TMyProductionRecord::AddPickCleanPadFormShuttle(...)   (no-op)
//      TMyProductionRecord::AddPlaceCleanPad(...)             (no-op)
//  This TU provides REAL bodies for the ctor and 7 of those 8 remaining
//  methods (AddErrorRecord excepted, see above) because they are genuinely
//  in this wave's scope and behaviourally different from a no-op (e.g.
//  GetInRotationAngRecord must return the recorded angle, not "").  That
//  means linking this .cpp together with aHotPlateSubstrate.cpp WILL produce
//  "multiple definition" errors for those 7 symbols + the ctor until a
//  human/integrate-agent deletes the aHotPlateSubstrate.cpp stand-ins (see
//  that file's :472-491 for the exact lines). This .cpp is therefore NOT
//  wired into any shared CMake target by this wave -- see tests/CMakeLists.txt
//  for the isolated verification target used instead, and the wave's own
//  report for the full reconciliation recommendation.
//  AddTestRecord is the ONE exception left untouched: golden itself never
//  gave it a body (golden .h:19 declares it fully commented-out; golden .cpp
//  :305-309 is the matching commented-out shell) -- an empty no-op IS the
//  faithful translation, so the pre-existing aHotPlateSubstrate.cpp stand-in
//  already is the correct, final answer; redefining it here would be a
//  needless duplicate, not a fix.
//
//  DEPENDENCY VERIFICATION (re-checked against the CURRENT target tree,
//  20260721 -- flagging every place this differs from the task brief's
//  assumptions):
//    * CUSTOMER_CODE/CC_KYEC_LEE/CC_Greatek/CC_HANA_MICRON -- CUSTOMER_CODE
//      is `extern int` in cmydef.h:3181; the CC_* constants are #defines in
//      MachineType.h (NOT cmydef.h/cprod.h as the brief assumed) --
//      MachineType.h:230/292/330.
//    * GetDateInfoByString/GetOnlyTimeInfoByString -- real, cpublic.h:49-50 /
//      cpublic.cpp, matches brief.
//    * TestSocket, TestIF_File.iGpibMode, iTestBinCount -- real.  TestSocket
//      is `extern TMyKitSuck TestSocket;` in aHotPlateSubstrate.h:389 (NOT
//      MyKitSuck.h -- that file does not exist in the target tree; the
//      target's substrate lives in aHotPlateSubstrate.h/.cpp instead).
//      TestIF_File is `extern SYSTEM_TEST_IF TestIF_File;` (cprod.h:2577,
//      iGpibMode member at cprod.h:1673).  iTestBinCount: cmydef.h:3396.
//    * InArmSuck/OutArmSuck .Suck[][].sName -- real (aHotPlateSubstrate.h),
//      matches brief.  .Suck[][].GetIOValue() -- DOES NOT EXIST on the
//      target's TMySucker (aHotPlateSubstrate.h:106-160+; golden MyKitSuck.h
//      :108).  This is a genuine gap the brief's recon missed -- see the
//      per-method "Hard boundary" gate on AddInArmLoaderPickRecord/
//      AddOutArmShuttlePickRecord/AddInArmHotplatePickRecord below (matches
//      atester_32Site.cpp's own already-established W5_32S_ADD_INDEX_PICK_VACUUM
//      workaround for this exact same missing method).
//    * IndexSuckName[][]/iOneTrayPickCount[] -- real, cmydef.h:3239/5147,
//      matches brief.
//    * LoadForm/HotPlateForm -- real.  LoadForm is `extern TRAY_TYPE_PARA
//      *LoadForm;` (a POINTER, cprod.h:1365) -- golden calls it via `->`,
//      matches. HotPlateForm is `extern TRAY_TYPE_PARA HotPlateForm;` (BY
//      VALUE, cprod.h:1361) -- golden calls it via `.`, matches.
//    * CosFunction.bRecordGroundESDByTestIC -- real, CosFunction.h:351
//      (pulled in transitively via cprod.h:7's `#include "CosFunction.h"`).
//    * GetTimeInfo() -- real, cpublic.h:23/cpublic.cpp (fills the SystemHour/
//      SystemMin/SystemSec/SystemMSec globals below via DecodeTime).
//    * SystemHour/SystemMin/SystemSec/SystemMSec -- real, `extern Word ...` in
//      cmydef.h:226 (the brief did not call these out explicitly but the
//      AddPickCleanPad/AddPlaceCleanPad* family needs them).
//    * Now().FormatString("yyyy/mm/dd") -- golden BCB6 TDateTime member call.
//      vclcompat's TDateTime has NO FormatString method; the established,
//      repeatedly-used precedent elsewhere in this tree (ainarm9045.cpp:890,
//      Automation/SCK_ART_Remainder.cpp:1282, SECSGEM/uHGemClass.cpp:2787) is
//      `FormatDateTime(fmt, Now())` -- used the same way below.
// =============================================================================
#include "Public/MyProductionRecord.h"

#include "vclcompat/vcl_compat.h"   // AnsiString, TStringList, Now(), FormatDateTime
#include "MachineType.h"            // CC_KYEC_LEE / CC_Greatek / CC_HANA_MICRON
#include "cprod.h"                  // LoadForm, HotPlateForm, TestIF_File, CosFunction (via CosFunction.h)
#include "cmydef.h"                 // CUSTOMER_CODE, IndexSuckName[][], iOneTrayPickCount[],
                                     // iTestBinCount, SystemHour/Min/Sec/MSec
#include "cpublic.h"                 // GetDateInfoByString/GetOnlyTimeInfoByString/GetTimeInfo
#include "aHotPlateSubstrate.h"     // TMyKitSuck/TMySucker + InArmSuck/OutArmSuck/TestSocket externs

#include <cstdlib>                  // atoi

//------------------------------------------------------------------------------
//Steven 20160113 : 記錄每一顆IC 狀態
//------------------------------------------------------------------------------
TMyProductionRecord::TMyProductionRecord()
{
    asBuffer = new TStringList();
    for(int i=eScheduleName; i<eDataTotal; i++)
    {
        asBuffer->Add("");                                                      //kevin 20180129 add
    }
    InitialRecord();

    asDataTitleGreatek=AnsiString("Schedule name, ")                 +   //eScheduleName         =0         //Sam 20200707 : ProductionRecord for Greatek
                       AnsiString("Start time, ")                    +   //eStartTime            =1
                       AnsiString("Input tray, ")                    +   //eLoadNum              =2
                       AnsiString("Tray Form, ")                     +   //eTrayForm             =41     //Sam 20210907 : Add record
                       AnsiString("In X, ")                          +   //eLoadX                =3
                       AnsiString("In Y, ")                          +   //eLoadY                =4
                       AnsiString("In Time, ")                       +   //eLoadTime             =5
                       AnsiString("In Arm Loader Pick, ")            +   //eInArmLoaderPick      =30
                       AnsiString("Hotplate No,")                    +   //eHotplateNo           =35     //Sam 20200716 : Add record
                       AnsiString("Hotplate Form, ")                 +   //eHotPlateForm         =42     //Sam 20210907 : Add record
                       AnsiString("Hot X, ")                         +   //eHotX                 =6
                       AnsiString("Hot Y, ")                         +   //eHotY                 =7
                       AnsiString("Hot Time, ")                      +   //eHotTime              =8
                       AnsiString("In Arm Hotplate Pick, ")          +   //eInArmHotplatePick    =36     //Sam 20200716 : Add record
                       AnsiString("2DCode, ")                        +   //e2DCode               =9
                       AnsiString("In Arm Place Shuttle Site No, ")  +   //eSiteNO               =10    //1-32 //Site No -> In Arm Place Shuttle Site No
                       AnsiString("Arm No, ")                        +   //eArmNO                =11    //0: arm1  1: arm2
                       AnsiString("Index Pick Shuttle Site No, ")    +   //eIndexPickSHTSiteNo   =31
                       AnsiString("Arm Time, ")                      +   //eArmTime              =12
                       AnsiString("Order of testing, ")              +   //eOrderTest            =13    //touch-down
                       AnsiString("SOT time stamp, ")                +   //eSOTTime              =14
                       AnsiString("Index cycle time, ")              +   //eIndexCycleTime       =15    //第一顆測試 是 0 (1st device)
                       AnsiString("Test category, ")                 +   //eTestCate             =16    //測試 BIN碼
                       AnsiString("Test Mode, ")                     +   //eTestMode             =37
                       AnsiString("Index Place Shuttle Site No,")    +   //eIndexPlaceSHTSiteNo  =32
                       AnsiString("iWhichAuto, ")                    +   //eWhichAuto            =17
                       AnsiString("Out Shuttle Detect Site No,")     +   //eOutSHTDetectSiteNo   =33
                       AnsiString("Out Arm Shuttle Pick, ")          +   //eOutArmShuttlePick    =34
                       AnsiString("Output tray, ")                   +   //eOutTrayNum           =18    //Auto1 :O1-1  Auto2 :2-1  Auto3 :3-1  Fix 1:4-1 Fix 2:5-1 Fix 3:6-1
                       AnsiString("Out X, ")                         +   //eUnloadX              =19
                       AnsiString("Out Y, ")                         +   //eUnloadY              =20
                       AnsiString("Out Time, ")                      +   //eUnloadTime           =21
                       AnsiString("Out Arm X Pos, ")                 +   //eUnloadXPos           =22
                       AnsiString("Out Arm Y Pos, ")                 +   //eUnloadYPos           =23
                       AnsiString("Out Arm Time, ")                  +   //eUnloadYPos           =24
                       AnsiString("Error log, ")                     +   //eErrorCode            =25
                       AnsiString("OCR Code , ")                     +   //eOCRCode              =26
                       AnsiString("Test Time, ")                     +   //eTestTime             =27
                       AnsiString("TSD Time,   ")                    +   //eTSDTime              =28
                       AnsiString("EOT time stamp,")                 +   //eEOTTime              =29
                       AnsiString("Out Arm X pitch Pos, ")           + //eUnloadXpPos            =38  //JerryYang 20210119 : add x,y pitch位置
                       AnsiString("Out Arm X pitch2 Pos, ")          + //eUnloadXp2Pos           =39
                       AnsiString("Out Arm Y pitch Pos, ")           ; //eUnloadYpPos            =40

    //Ifor 20180510 : add 時間資訊
    //==>
    asDataTitle=AnsiString("Schedule name, ")   +       //eScheduleName   =0
                AnsiString("Start time, ")      +       //eStartTime      =1
                AnsiString("Input tray, ")      +       //eLoadNum        =2
                AnsiString("In X, ")            +       //eLoadX          =3
                AnsiString("In Y, ")            +       //eLoadY          =4
                AnsiString("In Time, ")         +       //eLoadTime       =5
                AnsiString("Hot X, ")           +       //eHotX           =6
                AnsiString("Hot Y, ")           +       //eHotY           =7
                AnsiString("Hot Time, ")        +       //eHotTime        =8
                AnsiString("2DCode, ")          +       //e2DCode         =9
                AnsiString("Site No, ")         +       //eSiteNO         =10    //1-32
                AnsiString("Arm No, ")          +       //eArmNO          =11    //0: arm1  1: arm2
                AnsiString("Arm Time, ")        +       //eArmTime        =12
                AnsiString("Order of testing, ")+       //eOrderTest      =13    //touch-down
                AnsiString("SOT time stamp, ")  +       //eSOTTime        =14    //kevin 20140918  yyyymmdd_hhmmss
                AnsiString("Index cycle time, ")+       //eIndexCycleTime =15    //第一顆測試 是 0 (1st device)
                AnsiString("Test category, ")   +       //eTestCate       =16    //測試 BIN碼
                AnsiString("iWhichAuto, ")      +       //eWhichAuto      =17
                AnsiString("Output tray, ")     +       //eOutTrayNum     =18    //Auto1 :O1-1  Auto2 :2-1  Auto3 :3-1  Fix 1:4-1 Fix 2:5-1 Fix 3:6-1
                AnsiString("Out X, ")           +       //eUnloadX        =19
                AnsiString("Out Y, ")           +       //eUnloadY        =20
                AnsiString("Out Time, ")        +       //eUnloadTime     =21
                AnsiString("Out Arm X Pos, ")   +       //eUnloadXPos     =22
                AnsiString("Out Arm Y Pos, ")   +       //eUnloadYPos     =23
                AnsiString("Out Arm Time, ")    +       //eUnloadYPos     =24
                AnsiString("Error log, ")       +       //eErrorCode      =25
                AnsiString("OCR Code , ")       +       //eOCRCode        =26
                AnsiString("Test Time, ")       +       //eTestTime       =27
                AnsiString("TSD Time,   ")      +       //eTSDTime        =28
                AnsiString("EOT time stamp, ")  +       //eEOTTime        =29     //wei 20181211 更換設定SOT
                AnsiString("In Arm Loader Pick, ")+     //eInArmLoaderPick=30     //Sam 20200623 : Add Suck record
                AnsiString("Index Pick Shuttle Site No, ")+ //eIndexPickSHTSiteNo  =31     //Sam 20200623 : Add record
                AnsiString("Index Place Shuttle Site No,")+ //eIndexPlaceSHTSiteNo =32     //Sam 20200623 : Add record
                AnsiString("Out Shuttle Detect Site No,") + //eOutSHTDetectSiteNo  =33     //Sam 20200623 : Add record
                AnsiString("Out Arm Shuttle Pick, ")      + //eOutArmShuttlePick   =34     //Sam 20200623 : Add record
                AnsiString("Hotplate No,")                + //eHotplateNo          =35     //Sam 20200716 : Add record
                AnsiString("In Arm Hotplate Pick, ")      + //eInArmHotplatePick   =36     //Sam 20200716 : Add record
                AnsiString("Test Mode, ")                 + //eTestMode            =37     //Sam 20201216 : Add record
                AnsiString("Out Arm X pitch Pos, ")       + //eUnloadXpPos         =38  //JerryYang 20210119 : add x,y pitch位置
                AnsiString("Out Arm X pitch2 Pos, ")      + //eUnloadXp2Pos        =39
                AnsiString("Out Arm Y pitch Pos, ")       + //eUnloadYpPos         =40
                AnsiString("Tray Form, ")                 + //eTrayForm            =41     //Sam 20210907 : Add record
                AnsiString("Hotplate Form, ")             + //eHotPlateForm        =42     //Sam 20210907 : Add record
                AnsiString("Tray ID, ")                   + //eTrayID              =43     //Steven 20220120 : Amlogic需要輸出SBIN
                AnsiString("SBin, ")                      + //eSBin                =44     //Steven 20220120 : Amlogic需要輸出SBIN
                AnsiString("Ground, ")                    + //eGround              =45     //Sam 20211223 : 每顆 IC 測試完成後要記錄測試當時的 Ground & ESD 數值。
                AnsiString("ESD, ")                       + //eESD                 =46     //Sam 20211223 : 每顆 IC 測試完成後要記錄測試當時的 Ground & ESD 數值。
                AnsiString("In Rotation Angle, ")         + //eInRotationAngle     =47     //Sam 20221103 : 新增In料旋轉角度
                AnsiString("Pick cleanpad data, ")              +       //ePickCleanPadData         =48 //Sam 20230616 : Add Auto Clean Record
                AnsiString("Pick cleanpad time, ")              +       //ePickCleanPadTime         =49
                AnsiString("Pick cleanpad suck, ")              +       //ePickCleanPadSuck         =50
                AnsiString("Pick cleanpad row, ")               +       //ePickCleanPadRow          =51
                AnsiString("Pick cleanpad col, ")               +       //ePickCleanPadCol          =52
                AnsiString("Place cleanpad to shuttle time, ")  +       //ePlaceCleanPadToShtTime   =53
                AnsiString("Place cleanpad to shuttle suck, ")  +       //ePlaceCleanPadToShtSuck   =54
                AnsiString("Place cleanpad to shuttle row, ")   +       //ePlaceCleanPadToShtRow    =55
                AnsiString("Place cleanpad to shuttle col, ")   +       //ePlaceCleanPadToShtCol    =56
                AnsiString("Pick cleanpad form shuttle time, ") +       //ePickCleanPadFormShtTime  =57
                AnsiString("Pick cleanpad form shuttle suck, ") +       //ePickCleanPadFormShtSuck  =58
                AnsiString("Pick cleanpad form shuttle row, ")  +       //ePickCleanPadFormShtRow   =59
                AnsiString("Pick cleanpad form shuttle col, ")  +       //ePickCleanPadFormShtCol   =60
                AnsiString("Place cleanpad time, ")             +       //ePlaceCleanPadTime        =61
                AnsiString("Place cleanpad suck, ")             +       //ePlaceCleanPadSuck        =62
                AnsiString("Place cleanpad row, ")              +       //ePlaceCleanPadRow         =63
                AnsiString("Place cleanpad col, ")              +       //ePlaceCleanPadCol         =64
                AnsiString("InArm Pick From Loader Vacuum, ")   +       //eLoaderPickVacuum         =65     //RogerYang 20251225 : 補上
                AnsiString("InArm Pick From Hotplate Vacuum, ") +       //eHPPickVacuum             =66     //RogerYang 20251225 : 補上
                AnsiString("OutArm Pick From Shuttle Vacuum, ") +       //eOSHTPickVacuum           =67     //RogerYang 20251225 : 補上
                AnsiString("Index Pick Vacuum, ")               +       //eIndexPickVacuum          =68     //RogerYang 20251225 : 補上
                AnsiString("In Arm X Pos, ")                    +       //eLoadXPos                 =69     //Sam 20250211 : Add Load Pos
                AnsiString("In Arm Y Pos, ")                    ;       //eLoadYPos                 =70
    //<==
    //Ifor 20180510 : add 時間資訊

    //Sam 20211223 : 每顆 IC 測試完成後要記錄測試當時的 Ground & ESD 數值。
    //==>
    asDataTitle_SG=AnsiString("EOT time stamp, ")   +       //eEOTTime     =29
                   AnsiString("2DCode, ")           +       //e2DCode      =9
                   AnsiString("Test category, ")    +       //eTestCate    =16    //測試 BIN碼
                   AnsiString("Test result, ")      +       //Pass=P , Fail=F
                   AnsiString("Ground, ")           +       //eGround      =45
                   AnsiString("ESD ")               ;       //eESD         =46
    //<==
    //Sam 20211223 : 每顆 IC 測試完成後要記錄測試當時的 Ground & ESD 數值。

    //Sam 20230616 : Add Auto Clean Record
    //==>
    asDataTitleCleanPad=AnsiString("Pick clean pad data, ")              +       //ePickCleanPadData         =48
                        AnsiString("Pick clean pad time, ")              +       //ePickCleanPadTime         =49
                        AnsiString("Pick clean pad suck, ")              +       //ePickCleanPadSuck         =50
                        AnsiString("Pick clean pad row, ")               +       //ePickCleanPadRow          =51
                        AnsiString("Pick clean pad col, ")               +       //ePickCleanPadCol          =52
                        AnsiString("Place clean pad to shttle time, ")   +       //ePlaceCleanPadToShtTime   =53
                        AnsiString("Place clean pad to shttle suck, ")   +       //ePlaceCleanPadToShtSuck   =54
                        AnsiString("Place clean pad to shttle row, ")    +       //ePlaceCleanPadToShtRow    =55
                        AnsiString("Place clean pad to shttle col, ")    +       //ePlaceCleanPadToShtCol    =56
                        AnsiString("Pick clean pad form shttle time, ")  +       //ePickCleanPadFormShtTime  =57
                        AnsiString("Pick clean pad form shttle suck, ")  +       //ePickCleanPadFormShtSuck  =58
                        AnsiString("Pick clean pad form shttle row, ")   +       //ePickCleanPadFormShtRow   =59
                        AnsiString("Pick clean pad form shttle col, ")   +       //ePickCleanPadFormShtCol   =60
                        AnsiString("Place clean pad time, ")             +       //ePlaceCleanPadTime        =61
                        AnsiString("Place clean pad suck, ")             +       //ePlaceCleanPadSuck        =62
                        AnsiString("Place clean pad row, ")              +       //ePlaceCleanPadRow         =63
                        AnsiString("Place clean pad col, ")              +       //ePlaceCleanPadCol         =64
                        AnsiString("Error log ")                         ;       //eErrorCode            =25
    //<==
    //Sam 20230616 : Add Auto Clean Record

    //RogerYang 20251208 : JCET 2D FT1組名稱/FT2測試碼
    asJCET2DIDList= AnsiString("2DID, ")                            +           //e2DCode           =9
                    AnsiString("SOFTBIN, ")                         +           //eSBin             =44
                    AnsiString("HARDBIN, ")                         +           //eTestCate         =16
                    AnsiString("SubLOTID, ")                        +
                    AnsiString("CustLOTID, ")                       +
                    AnsiString("Modecode, ")                        +
                    AnsiString("StartTime, ")                       +           //eSOTTime          =14
                    AnsiString("EndTime, ")                         +           //eEOTTime          =29
                    AnsiString("SITE, ")                            +           //eSiteNO           =10
                    AnsiString("In X, ")                            +           //eLoadX            =3
                    AnsiString("In Y, ")                            +           //eLoadY            =4
                    AnsiString("Out X, ")                           +           //eUnloadX          =19
                    AnsiString("Out Y, ")                           +           //eUnloadY          =20
                    AnsiString("iWhichAuto, ")                      +           //eWhichAuto        =17
                    AnsiString("PartID, ")                          ;
}
//------------------------------------------------------------------------------
void TMyProductionRecord::DeleteProductionRecord()
{
    if(asBuffer!=0 && asBuffer!=NULL)
    {
        asBuffer->Clear();                                                      //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        delete asBuffer;
        asBuffer=NULL;                                                          //kevin 20170306
    }
}
//------------------------------------------------------------------------------
void TMyProductionRecord::InitialRecord()
{
    if(asBuffer->Count!=eDataTotal)
    {
        DeleteProductionRecord();
        // AI(W906-MyProductionRecord) 20260721: golden quirk, preserved
        // VERBATIM -- `TMyProductionRecord();` here is a bare function-style
        // statement, i.e. it constructs an unnamed TEMPORARY of this class and
        // immediately destroys it. It does NOT reinitialize `this` (a
        // placement-new or `*this = TMyProductionRecord()` would be needed for
        // that). Golden has carried this exact no-op-on-`this` bug since the
        // original Steven 20160113 commit; kept as-is per this wave's
        // "preserve golden bugs verbatim + comment, don't silently fix" rule.
        TMyProductionRecord();
    }

    for(int i=eLoadNum; i<eDataTotal; i++)
    {
        asBuffer->Strings[i]="";
    }
    bUse=false;
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddLoadRecord(int iNum, int iR, int iC, int iXPos, int iYPos, AnsiString Name, AnsiString Time)
{
    AnsiString sTestMode="", asTrayForm="";

    if(CUSTOMER_CODE==CC_KYEC_LEE || CUSTOMER_CODE==CC_Greatek)                 //Sam 20200704 : Add Greatek
    {
        iC=iC+1;
        iR=iR+1;
    }
    AnsiString sTime=GetDateInfoByString("/")+" "+GetOnlyTimeInfoByString(":"); //Ifor 20180510 : add 時間資訊

    sTestMode.sprintf("%dx%d", TestSocket.iShtRow, TestSocket.iShtCol);
    asTrayForm.sprintf("%dx%d", LoadForm->XDivision, LoadForm->YDivision);      //Sam 20210907 : Add record

    asBuffer->Strings[eLoadNum      ]   ="I1-"+AnsiString(iNum);
    asBuffer->Strings[eLoadX        ]   =AnsiString(iC);
    asBuffer->Strings[eLoadY        ]   =AnsiString(iR);
    asBuffer->Strings[eLoadXPos     ]   =AnsiString(iXPos);                     //Sam 20250211 : Add Load Pos
    asBuffer->Strings[eLoadYPos     ]   =AnsiString(iYPos);
    asBuffer->Strings[eLoadTime     ]   =sTime;                                 //Ifor 20180510 : add 時間資訊
    asBuffer->Strings[eScheduleName ]   =Name;
    asBuffer->Strings[eStartTime    ]   =Time;
    asBuffer->Strings[eTestMode     ]   =sTestMode;                             //Sam 20201216 : Add record
    asBuffer->Strings[eTrayForm     ]   =asTrayForm;                            //Sam 20210907 : Add record
    bUse                                =true;
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddHPRecord(int iPlate, int iR, int iC)
{
    AnsiString sHPNo="",asHotPlateForm="";

    if(iPlate==0)
        sHPNo="2";
    else if(iPlate==1)
        sHPNo="1";
    else
        sHPNo="na";

    asHotPlateForm.sprintf("%dx%d",HotPlateForm.XDivision,HotPlateForm.YDivision);  //Sam 20210907 : Add record

    if(CUSTOMER_CODE==CC_KYEC_LEE || CUSTOMER_CODE==CC_Greatek)                 //Sam 20200704 : Add Greatek
    {
        iC=iC+1;
        iR=iR+1;
    }

    AnsiString sTime=GetDateInfoByString("/")+" "+GetOnlyTimeInfoByString(":"); //Ifor 20180510 : add 時間資訊
    asBuffer->Strings[eHotX         ]   =AnsiString(iC);
    asBuffer->Strings[eHotY         ]   =AnsiString(iR);
    asBuffer->Strings[eHotTime      ]   =sTime;                                 //Ifor 20180510 : add 時間資訊
    asBuffer->Strings[eHotplateNo   ]   =sHPNo;                                 //Sam 20210907 : Add record
    asBuffer->Strings[eHotPlateForm ]   =asHotPlateForm;                        //Sam 20210907 : Add record
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddArmSiteRecord(int iArm, int iSite)                 //Steven 20201204 : 修正Site No在非標準Pitch的情況下有問題
{
    asBuffer->Strings[eArmNO]           =AnsiString(iArm);
    asBuffer->Strings[eSiteNO]          =AnsiString(iSite);
    AnsiString sTime=GetDateInfoByString("/")+" "+GetOnlyTimeInfoByString(":"); //Ifor 20180510 : add 時間資訊
    asBuffer->Strings[eArmTime]         =sTime;                                 //Ifor 20180510 : add 時間資訊
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddTestResultRecord(int iBin, AnsiString SBin, AnsiString ErrorLog)
{
    if(TestIF_File.iGpibMode==1)                                                //256 bin    kevin 20141031
    {
        if(iBin>=255)                                                           //JerryYang 20230322 : 最大只能255BIN
            asBuffer->Strings[eTestCate]="E";
        else
            asBuffer->Strings[eTestCate]=AnsiString(iBin);
    }
    else
    {
        if(iBin>=iTestBinCount)                                                 //JerryYang 20200812 : error bin統一記為"E"
        {
            asBuffer->Strings[eTestCate]="E";
        }
        else
        {
            asBuffer->Strings[eTestCate]=AnsiString(iBin);
        }
    }
    asBuffer->Strings[eSBin]            =SBin;                                  //Steven 20220120 : Amlogic需要輸出SBIN
    AddErrorLog(ErrorLog);
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddErrorLog(AnsiString ErrorLog)                      //Steven 20200518 : 避免重複ErrorLog不覆蓋
{
    if(ErrorLog!="")
    {
        if(asBuffer->Strings[eErrorCode]=="" || asBuffer->Strings[eErrorCode]=="na")
        {
            asBuffer->Strings[eErrorCode]=ErrorLog;
        }
        else
        {
            if(ErrorLog=="" || ErrorLog=="na")
            {
            }
            // AI(W906-MyProductionRecord) 20260721: golden `Strings[i].AnsiPos(...)`
            // -- vclcompat's StringsProxy has no passthrough AnsiString methods
            // (read-only via its conversion operator); GetString(i) returns the
            // plain AnsiString by value so AnsiPos chains -- established
            // precedent, see mycylin.cpp:826/844's identical GetString(i).c_str() fix.
            else if(asBuffer->GetString(eErrorCode).AnsiPos(ErrorLog)==0)         //避免記入重複的Error log
            {
                asBuffer->Strings[eErrorCode]=asBuffer->Strings[eErrorCode]+AnsiString("_")+ErrorLog;
            }
        }
    }
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddErrorRecordNoSave(AnsiString ErrorLog, bool bAddPosition, int iXPos, int iYPos, int iXPitch, int iXPitch2, int iYPitch)    //Steven 20190110 : 把Error紀錄拆開,不存檔,只記錄用於Retry
{
    if(bAddPosition)                                                            //Steven 20190110 : Error Log Add Motor Position.
    {
        asBuffer->Strings[eUnloadXPos]  =AnsiString(iXPos);
        asBuffer->Strings[eUnloadYPos]  =AnsiString(iYPos);

        asBuffer->Strings[eUnloadXpPos]  =AnsiString(iXPitch);                  //JerryYang 20210119 : add x,y pitch位置
        asBuffer->Strings[eUnloadXp2Pos]  =AnsiString(iXPitch2);
        asBuffer->Strings[eUnloadYpPos]  =AnsiString(iYPitch);
    }
    AddErrorLog(ErrorLog);
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddIndexCycleTimeRecord(double fIndexCycleTime)
{
    AnsiString IndexCycleTime;
    IndexCycleTime.sprintf("%6.3f", (double)fIndexCycleTime);
    asBuffer->Strings[eIndexCycleTime]=IndexCycleTime;
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddOrderTestRecord(int iNum)
{
    asBuffer->Strings[eOrderTest]=AnsiString(iNum);
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddDataTimeRecord(AnsiString asStr)
{
    asBuffer->Strings[eSOTTime]=asStr;
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddDataTimeEOTRecord(AnsiString asStr)                //wei 20181211 更換設定SOT
{
    asBuffer->Strings[eEOTTime]=asStr;
}
//------------------------------------------------------------------------------
void TMyProductionRecord::Add2DIDRecord(AnsiString s2DID)
{
    asBuffer->Strings[e2DCode]=s2DID;
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddUnloadRecord(int iWhichAuto, int iNum, int iTrayRow, int iTrayCol, int iXPos, int iYPos, AnsiString ascode, int iXPitch, int iXPitch2, int iYPitch, AnsiString TrayID)   //Frank 20160727 add iXpos iYpos
{
    if(CUSTOMER_CODE==CC_KYEC_LEE ||
       CUSTOMER_CODE==CC_Greatek ||                                             //Sam 20200704 : Add Greatek
       CUSTOMER_CODE==CC_HANA_MICRON)
    {
        iTrayRow=iTrayRow+1;
        iTrayCol=iTrayCol+1;
    }
    AnsiString sTime=GetDateInfoByString("/")+" "+GetOnlyTimeInfoByString(":"); //Ifor 20180510 : add 時間資訊
    asBuffer->Strings[eWhichAuto]       =AnsiString(iWhichAuto+1);
    asBuffer->Strings[eOutTrayNum]      ="O"+AnsiString(iWhichAuto+1)+"-"+AnsiString(iNum);        //Auto1 :O1-1  Auto2 :2-1  Auto3 :3-1  Fix 1:4-1 Fix 2:5-1 Fix 3:6-1
    asBuffer->Strings[eUnloadX]         =AnsiString(iTrayCol);
    asBuffer->Strings[eUnloadY]         =AnsiString(iTrayRow);
    asBuffer->Strings[eUnloadTime]      =sTime;
    asBuffer->Strings[eUnloadXPos]      =AnsiString(iXPos);                     //Frank 20160727 add iXpos iYpos
    asBuffer->Strings[eUnloadYPos]      =AnsiString(iYPos);                     //Frank 20160727 add iXpos iYpos
    asBuffer->Strings[eUnloadPosTime]   =sTime;                                 //Ifor 20180510 : add 時間資訊
    asBuffer->Strings[eOCRCode]         =AnsiString(ascode);                    //wei 20161005
    asBuffer->Strings[eUnloadXpPos]     =AnsiString(iXPitch);                   //JerryYang 20210119 : add x,y pitch位置
    asBuffer->Strings[eUnloadXp2Pos]    =AnsiString(iXPitch2);
    asBuffer->Strings[eUnloadYpPos]     =AnsiString(iYPitch);
    asBuffer->Strings[eTrayID]          =AnsiString(TrayID);                    //Steven 20220120 : Amlogic需要輸出SBIN
    iOneTrayPickCount[1+iWhichAuto]++;                                          //kevin 20210623 Count 每輪片數量
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddTestTime(AnsiString asStr)                         //Steven 20161219 : Add Test Time in Production log
{
    asBuffer->Strings[eTestTime]=asStr;
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddTSDTime(AnsiString asStr)                          //Steven 20161219 (jou) : 修改TSD Log方式
{
    asBuffer->Strings[eTSDTime]=asStr;
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddInArmLoaderPickRecord(int iSuckRow, int iSuckCol)  //Sam 20200623 : Add record
{
    asBuffer->Strings[eInArmLoaderPick]=InArmSuck.Suck[iSuckRow][iSuckCol].sName;
    // AI(W906-MyProductionRecord) 20260721: golden :431-436 reads
    // InArmSuck.Suck[][].GetIOValue() (Sam 20230210 VacuumUnit) to populate
    // eLoaderPickVacuum -- TMySucker (aHotPlateSubstrate.h) has NO
    // GetIOValue() member (golden MyKitSuck.h:108; verified absent by grep --
    // a real gap this wave's recon missed). TMySucker is a foreign shared
    // class owned by another wave/file (aHotPlateSubstrate.h) that this unit
    // cannot extend -- "Hard boundary" rule, same precedent already
    // established by atester_32Site.cpp's own W5_32S_ADD_INDEX_PICK_VACUUM
    // no-op (which cites this exact GetIOValue() gap). Gate ONLY this
    // vacuum-read tail; the sName record above is real. eLoaderPickVacuum
    // stays at InitialRecord's "" default until GetIOValue() lands.
#if 0 // TODO(W-VacuumUnit) -- TMySucker::GetIOValue() not real yet (golden MyKitSuck.h:108)
    AnsiString s="";                                                            //Sam 20230210 : 新增 VacuumUnit 通訊模組
    s.sprintf("%2.1f", InArmSuck.Suck[iSuckRow][iSuckCol].GetIOValue());
    if(s=="999.9")
        asBuffer->Strings[eLoaderPickVacuum]="NA";
    else
         asBuffer->Strings[eLoaderPickVacuum]=s;
#endif
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddOutArmShuttlePickRecord(int iSuckRow, int iSuckCol)//Sam 20200623 : Add record
{
    asBuffer->Strings[eOutArmShuttlePick]=OutArmSuck.Suck[iSuckRow][iSuckCol].sName;
    // AI(W906-MyProductionRecord) 20260721: same TMySucker::GetIOValue() gap
    // as AddInArmLoaderPickRecord above -- see that method's comment.
#if 0 // TODO(W-VacuumUnit) -- TMySucker::GetIOValue() not real yet (golden MyKitSuck.h:108)
    AnsiString s="";                                                            //Sam 20230210 : 新增 VacuumUnit 通訊模組
    s.sprintf("%2.1f", OutArmSuck.Suck[iSuckRow][iSuckCol].GetIOValue());
    if(s=="999.9")
        asBuffer->Strings[eOSHTPickVacuum]="NA";
    else
        asBuffer->Strings[eOSHTPickVacuum]=s;
#endif
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddIndexPickShuttleRecord(int iSite, int iSiteRow, int iSiteCol)    //Sam 20200623 : Add record
{
    AnsiString asSite="";
    asSite.sprintf("%s-%d", IndexSuckName[iSiteRow][iSiteCol], iSite);
    asBuffer->Strings[eIndexPickSHTSiteNo]=asSite;
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddIndexPlaceShuttleRecord(int iSite, int iSiteRow, int iSiteCol)    //Sam 20200623 : Add record
{
    AnsiString asSite="";
    asSite.sprintf("%s-%d", IndexSuckName[iSiteRow][iSiteCol], iSite);
    asBuffer->Strings[eIndexPlaceSHTSiteNo]=asSite;
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddOutShuttleDetectRecord(int iSite, int iSiteRow, int iSiteCol)    //Sam 20200623 : Add record
{
    AnsiString asSite="";
    asSite.sprintf("%s-%d", IndexSuckName[iSiteRow][iSiteCol], iSite);
    asBuffer->Strings[eOutSHTDetectSiteNo]=asSite;
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddInArmHotplatePickRecord(int iSuckRow, int iSuckCol)//Sam 20200716 : Add record
{
    asBuffer->Strings[eInArmHotplatePick]=InArmSuck.Suck[iSuckRow][iSuckCol].sName;
    // AI(W906-MyProductionRecord) 20260721: same TMySucker::GetIOValue() gap
    // as AddInArmLoaderPickRecord above. Golden quirk PRESERVED verbatim
    // inside the gate: golden :475 reads OutArmSuck (not InArmSuck) here even
    // though the sName line just above uses InArmSuck -- a genuine golden
    // copy-paste inconsistency (this method is otherwise InArm-only), kept
    // as-is per "preserve golden bugs verbatim + comment" (moot at runtime
    // either way while GetIOValue() itself does not exist).
#if 0 // TODO(W-VacuumUnit) -- TMySucker::GetIOValue() not real yet (golden MyKitSuck.h:108)
    AnsiString s="";                                                            //Sam 20230210 : 新增 VacuumUnit 通訊模組
    s.sprintf("%2.1f", OutArmSuck.Suck[iSuckRow][iSuckCol].GetIOValue());
    if(s=="999.9")
        asBuffer->Strings[eHPPickVacuum]="NA";
    else
        asBuffer->Strings[eHPPickVacuum]=s;
#endif
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddTestModeRecord(int iTestMode)                      //Sam 20201216 : Add record
{
    asBuffer->Strings[eTestMode]=iTestMode;
}
//------------------------------------------------------------------------------
// AI(W906-MyProductionRecord) 20260721: the Get* family below chains
// AnsiString methods (.c_str()/.SubString()/.Length()) straight off
// `asBuffer->Strings[i]` in golden. vclcompat's `Strings[i]` (StringsProxy)
// is read/write property sugar with only a conversion-to-AnsiString operator
// and assignment operators -- it does NOT forward AnsiString's own methods.
// `asBuffer->GetString(i)` (TStringList's plain function accessor, returns
// AnsiString BY VALUE) is used instead so the chained calls compile --
// established precedent, see mycylin.cpp:826/844's identical
// `Strings[i].c_str()` -> `GetString(i).c_str()` fix (that file's own
// AI(W6.0) comment cites the exact same "vclcompat proxy has no c_str" gap).
int TMyProductionRecord::GetAutoX()
{
    return atoi(asBuffer->GetString(eUnloadX).c_str());
}
//------------------------------------------------------------------------------
int TMyProductionRecord::GetAutoY()
{
    return atoi(asBuffer->GetString(eUnloadY).c_str());
}
//------------------------------------------------------------------------------
int TMyProductionRecord::GetWhcihAuto()
{
    return atoi(asBuffer->GetString(eWhichAuto).c_str())-1;
}
//------------------------------------------------------------------------------
int TMyProductionRecord::GetLoaderX()
{
    return atoi(asBuffer->GetString(eLoadX).c_str());
}
//------------------------------------------------------------------------------
int TMyProductionRecord::GetLoaderY()
{
    return atoi(asBuffer->GetString(eLoadY).c_str());
}
//------------------------------------------------------------------------------
int TMyProductionRecord::GetLoaderNum()                                         //Frank 20181121 add iLoaderNum
{
    AnsiString Str;
    AnsiString sLoadNum=asBuffer->GetString(eLoadNum);
    Str=sLoadNum.SubString(4, sLoadNum.Length());
    return atoi(Str.c_str());
}
//------------------------------------------------------------------------------
int TMyProductionRecord::GetSiteNo()
{
    return atoi(asBuffer->GetString(eSiteNO).c_str());
}
//------------------------------------------------------------------------------
int TMyProductionRecord::GetOrderOfContact()
{
    return atoi(asBuffer->GetString(eOrderTest).c_str());
}
//------------------------------------------------------------------------------
AnsiString TMyProductionRecord::GetSOTTime()
{
    if(asBuffer->Strings[eSOTTime]==NULL)
        return "";
    else
        return asBuffer->Strings[eSOTTime];
}
//------------------------------------------------------------------------------
AnsiString TMyProductionRecord::GetEOTTime()
{
    if(asBuffer->Strings[eEOTTime]==NULL)
        return "";
    else
        return asBuffer->Strings[eEOTTime];
}
//------------------------------------------------------------------------------
AnsiString TMyProductionRecord::GetTestTime()
{
    if(asBuffer->Strings[eTestTime]==NULL)
        return "";
    else
        return asBuffer->Strings[eTestTime];
}
//------------------------------------------------------------------------------
AnsiString TMyProductionRecord::GetErrorLog()
{
    if(asBuffer->Strings[eErrorCode]==NULL)
        return "";
    else
        return asBuffer->Strings[eErrorCode];
}
//------------------------------------------------------------------------------
// AI(W906-MyProductionRecord) 20260721: AddErrorRecord is DELIBERATELY NOT
// DEFINED here -- see this file's top-of-file banner ("AddErrorRecord ...
// IS declared in the header ... but is DELIBERATELY NOT DEFINED here") for
// the full rationale (its golden body's only non-leaf action is the
// out-of-scope SaveRecord() tail call).
//------------------------------------------------------------------------------
void TMyProductionRecord::AddGroundRecord(AnsiString sGround)                   //Sam 20211223 : 每顆 IC 測試完成後要記錄測試當時的 Ground & ESD 數值。
{
     asBuffer->Strings[eGround]=(CosFunction.bRecordGroundESDByTestIC==true)?sGround.c_str():"NA";
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddESDRecord(AnsiString sESD)                         //Sam 20211223 : 每顆 IC 測試完成後要記錄測試當時的 Ground & ESD 數值。
{
    asBuffer->Strings[eESD]=(CosFunction.bRecordGroundESDByTestIC==true)?sESD.c_str():"NA";
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddInRotationAngRecord(int iAngle)                    //Sam 20221103 : 新增In料旋轉角度
{
    asBuffer->Strings[eInRotationAngle]=AnsiString(iAngle);
}
//------------------------------------------------------------------------------
AnsiString TMyProductionRecord::GetInRotationAngRecord()                        //Sam 20221103 : 新增In料旋轉角度
{
    return asBuffer->Strings[eInRotationAngle];
}
//------------------------------------------------------------------------------
//Sam 20230616 : Add Auto Clean Record
//==>
void TMyProductionRecord::AddPickCleanPad(int iSuckRow, int iSuckCol, int iKitRow, int iKitCol, int iData)
{
     AnsiString sTime="";
     GetTimeInfo();
     // AI(W906-MyProductionRecord) 20260721: golden :615 `Now().FormatString(...)`
     // (BCB6 TDateTime member call) -> vclcompat `FormatDateTime(fmt, Now())` --
     // established precedent (ainarm9045.cpp:890, SCK_ART_Remainder.cpp:1282,
     // uHGemClass.cpp:2787).
     sTime.sprintf("%s %02d:%02d:%02d.%03d", FormatDateTime("yyyy/mm/dd", Now()), SystemHour, SystemMin, SystemSec, SystemMSec);
     asBuffer->Strings[ePickCleanPadData]=iData;
     asBuffer->Strings[ePickCleanPadTime]=sTime;
     asBuffer->Strings[ePickCleanPadSuck]=InArmSuck.Suck[iSuckRow][iSuckCol].sName;
     asBuffer->Strings[ePickCleanPadRow]=AnsiString(iKitRow);
     asBuffer->Strings[ePickCleanPadCol]=AnsiString(iKitCol);
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddPlaceCleanPadToShuttle(int iSuckRow, int iSuckCol, int iShtRow, int iShtCol)
{
     AnsiString sTime="";
     GetTimeInfo();
     sTime.sprintf("%s %02d:%02d:%02d.%03d", FormatDateTime("yyyy/mm/dd", Now()), SystemHour, SystemMin, SystemSec, SystemMSec);
     asBuffer->Strings[ePlaceCleanPadToShtTime]=sTime;
     asBuffer->Strings[ePlaceCleanPadToShtSuck]=InArmSuck.Suck[iSuckRow][iSuckCol].sName;
     asBuffer->Strings[ePlaceCleanPadToShtRow]=AnsiString(iShtRow);
     asBuffer->Strings[ePlaceCleanPadToShtCol]=AnsiString(iShtCol);
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddPickCleanPadFormShuttle(int iSuckRow, int iSuckCol, int iShtRow, int iShtCol)
{
     AnsiString sTime="";
     GetTimeInfo();
     sTime.sprintf("%s %02d:%02d:%02d.%03d", FormatDateTime("yyyy/mm/dd", Now()), SystemHour, SystemMin, SystemSec, SystemMSec);
     asBuffer->Strings[ePickCleanPadFormShtTime]=sTime;
     asBuffer->Strings[ePickCleanPadFormShtSuck]=InArmSuck.Suck[iSuckRow][iSuckCol].sName;
     asBuffer->Strings[ePickCleanPadFormShtRow]=AnsiString(iShtRow);
     asBuffer->Strings[ePickCleanPadFormShtCol]=AnsiString(iShtCol);
}
//------------------------------------------------------------------------------
void TMyProductionRecord::AddPlaceCleanPad(int iSuckRow, int iSuckCol, int iKitRow, int iKitCol)
{
     AnsiString sTime="";
     GetTimeInfo();
     sTime.sprintf("%s %02d:%02d:%02d.%03d", FormatDateTime("yyyy/mm/dd", Now()), SystemHour, SystemMin, SystemSec, SystemMSec);
     asBuffer->Strings[ePlaceCleanPadTime]=sTime;
     asBuffer->Strings[ePlaceCleanPadSuck]=InArmSuck.Suck[iSuckRow][iSuckCol].sName;
     asBuffer->Strings[ePlaceCleanPadRow]=AnsiString(iKitRow);
     asBuffer->Strings[ePlaceCleanPadCol]=AnsiString(iKitCol);
}
//<==
//Sam 20230616 : Add Auto Clean Record
//------------------------------------------------------------------------------
