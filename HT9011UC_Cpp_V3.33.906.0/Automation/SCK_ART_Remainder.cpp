// =============================================================================
//  Automation/SCK_ART_Remainder.cpp  --  SCK/ATK "Auto Retest" (ART) lot-tracking, REMAINDER slice
//
//  Translation wave: W5-Final-SckArtRemainder
//  Translator: AI(W5-Final-SckArtRemainder) 20260711
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/Automation/SCK_ART.cpp (4392 lines, cp950)
//
//  SCOPE: golden lines 183-1646 minus pure-VCL event handlers (see SCK_ART_Remainder.h's file-head
//  "GOLDEN LINE BOUNDARY" comment for the exact per-function split). Continues the sibling
//  Automation/SCK_ART.h/.cpp (W5-SckArt-Translate, 8-function extract, golden :36-56/:639-1359/
//  :4359-4391) WITHOUT editing that file -- see SCK_ART_Remainder.h's "WHY A SEPARATE STATE STRUCT"
//  note for the ODR/coupling rationale.
//
//  Big5 Chinese comments decoded via cp950 (python open(..., encoding='cp950')), verified rendered
//  correctly, and reproduced here as real UTF-8 (not an English gloss) wherever the golden line
//  carries one. ZERO U+FFFD (checked before hand-off).
// =============================================================================

#include "SCK_ART_Remainder.h"

#include "MachineDefine.h"
#include "MachineType.h"      // eAutoRetest (eartInstall), InterfaceType_16BinGS/32BinGS, CC_* codes
#include "cprod.h"             // Prod (bIsPassBin), TestIF_File (SYSTEM_TEST_IF), IniConfig, BinSelect[],
                                // FT/OffT, CustomerFunctionSelect(), WriteLastDataFile()
#include "cmydef.h"            // CUSTOMER_CODE, USE_AUTO_RETEST, OFF_LINE, K_SKIP, bCanRunSCKART, bWaitTSV,
                                // TCP_IP_MODE, _2D_SORT, bReadLotInfoFromART, bQAModeFlag (both real
                                // globals -- AI(W906-DoARTLotStart) 20260721, see SckArtRem_DoARTLotStart)
#include "common.h"            // ReadWriteIni / WriteIniData overloads, AuthPath, DataPath (all REAL,
                                // active bodies in common.cpp -- see gate #1 for the 3 exceptions)
#include "canary_support.h"    // LastSet (LAST_GENERAL_SET shim), RunInfo (RUN_INFO::AddAlarm), __FUNC__
#include "aHotPlateSubstrate.h"// OutArmSuck (TMyKitSuck: iBinData[][]); SetRunStartMode(int) (real,
                                // AI(W906-DoARTLotStart) 20260721)
#include "FormsFacade.h"       // fMain (see gate #3 -- SetStartModeData/LoadTestModePicture/
                                // BackupSetupFile are NOT on it yet); fLotInfo->cbProcess/SetLotID/
                                // SetLotStart (real -- AI(W906-DoARTLotStart) 20260721)
// AI(W906-DoARTLotStart) 20260721: new include, needed ONLY for HasICUnderMachine() (real,
// csystem_predicates.cpp:204). Lightweight header (vclcompat + myTimer.h + cmydef.h only, see its own
// file-head); already #include'd by several other ht9045_sm sources (acatchtray.cpp, aoutarm9045_*.cpp,
// ...) for this exact predicate -- no new circular-link risk (csystem_predicates.cpp and this file
// compile into the SAME CMake target, ht9045_sm; there is no separate "Automation" target to worry
// about).
#include "csystem.h"
// AI(W906-SaveTestSummarySECS) 20260721: new include, needed ONLY for fObserver->memoLotSummary (real,
// atester_shims.h/.cpp -- already #include'd by several other ht9045_sm sources, e.g. atester.cpp,
// acatchtray.cpp, for this same TfObserverShim singleton; same "same CMake target, no circular-link
// risk" posture as csystem.h just above).
#include "atester_shims.h"

// =============================================================================
//  SckArtRemainderState ctor -- golden TfSCKART::TfSCKART(TComponent*) (SCK_ART.cpp:36-120), the
//  subset that initializes the fields this struct mirrors. See header comment for the "no explicit
//  golden ctor line -> BCB6 zero-init" convention (same as the sibling file's SckArtState ctor).
// =============================================================================
SckArtRemainderState::SckArtRemainderState()
    : sSetupFilePath(""),               // golden :40
      sLOTSTATUS("NONE"),               // golden :51
      sLotID(""),                       // golden TForm AnsiString member; zero-init (no explicit ctor line)
      sProcessCode(""),                 // golden TForm AnsiString member; zero-init (no explicit ctor line)
      sLotStartTime(""),                // golden :41
      // AI(W906-SaveTestSummarySECS) 20260721: golden TForm AnsiString member (SCK_ART.h:242); zero-init
      // (no explicit golden ctor line), same convention as sLotStartTime just above.
      sLotEndTime(""),                  // golden :242
      iCurrentStatus(0),                // golden ctor does not set this explicitly; 0==iLOTSTATUS_NONE
      // AI(W906-DoARTLotStart) 20260721: golden TForm int members (SCK_ART.h:263-264); zero-init (no
      // explicit ctor line), same convention as every other "no explicit golden ctor line" field here.
      iCurrent93KARTStep(0),            // golden SCK_ART.h:263
      iCurrentFlexARTStep(0),           // golden SCK_ART.h:264
      iTesterType(0),                   // golden :42 (0: Flex, 1: 93K)
      iInputCount(0),                   // golden TForm int member; zero-init (no explicit ctor line)
      iLotCount(0),                     // golden TForm int member; zero-init (no explicit ctor line)
      iFTRTCount(0),                    // golden TForm int member; zero-init (no explicit ctor line)
      iManualRejectCnt(0),              // golden TForm int member; zero-init (no explicit ctor line)
      iInputJamCnt(0),                  // golden :54
      iOutputJamCnt(0),                 // golden :55
      iNeedRT(0),                       // golden TForm int member; zero-init (no explicit ctor line)
      dCurrYield(0.0),                  // golden TForm double member; zero-init (no explicit ctor line)
      iManualStart(0),                  // golden TForm int member; zero-init (no explicit ctor line)
      bFirstFullSkip(false),            // golden TForm bool member; zero-init (no explicit ctor line)
      sInfo_Customer(""), sInfo_InnerLotID(""), sInfo_CustLotID(""), sInfo_CustDevGup(""),   // golden :60-63
      sInfo_DeviceName(""), sInfo_Stage(""), sInfo_Step(""), sInfo_ReportCnt(""),             // golden :64-67
      sInfo_ProgramName(""), sInfo_TestBinNo(""), sInfo_TesterID(""), sInfo_HandlerID(""),   // golden :68-71
      sInfo_Temperauture(""), sInfo_CurrQty(""), sInfo_OperatorID(""), sInfo_BinSet(""),      // golden :72-75
      iInfo_MultiLotCnt(0),             // golden :77
      sBundleList(""),                  // golden TForm AnsiString member; zero-init (no explicit ctor line)
      iBundleInCnt(0),                  // golden TForm int member; zero-init (no explicit ctor line)
      iBundleOutCnt(0),                 // golden TForm int member; zero-init (no explicit ctor line)
      // AI(W906-SaveTestSummarySECS) 20260721: golden TForm bool/AnsiString members (SCK_ART.h:286-287);
      // zero-init (no explicit golden ctor line), same convention as every other such field above.
      // TSVdelay (TQPF_Timer, golden :303) is NOT listed here -- it default-constructs itself (its own
      // real __fastcall TQPF_Timer() ctor), same "no explicit init needed for a class-typed member"
      // precedent already used tree-wide for plain file-scope `TQPF_Timer` globals (e.g. acarry.cpp).
      bShowTSVMsg(false),               // golden :286
      sTSVMsg("")                       // golden :287
{
    // golden :79-97 loop -- sInfoArr_*[5] all set to "" for i<5.
    for(int i=0; i<5; i++)
    {
        sInfoArr_Customer[i]="";        sInfoArr_InnerLotID[i]="";
        sInfoArr_CustLotID[i]="";       sInfoArr_CustDevGup[i]="";
        sInfoArr_DeviceName[i]="";      sInfoArr_Stage[i]="";
        sInfoArr_Step[i]="";            sInfoArr_ReportCnt[i]="";
        sInfoArr_ProgramName[i]="";     sInfoArr_TestBinNo[i]="";
        sInfoArr_TesterID[i]="";        sInfoArr_HandlerID[i]="";
        sInfoArr_Temperauture[i]="";    sInfoArr_CurrQty[i]="";
        sInfoArr_OperatorID[i]="";      sInfoArr_BinSet[i]="";
    }
    // golden :102-114 (CUSTOMER_CODE==CC_SCK -> widget Visible/Caption only, else iTesterType=1) is
    // pure-VCL / gated by a customer check this remainder does not re-derive at construction time;
    // AccessFile's own ReadWriteIni("iTesterType",...) call (golden :204) is the real, persisted
    // source of truth once AccessFile(true,...) has run, matching how the sibling file's own
    // SckArtState ctor treats this exact field.
}

// =============================================================================
//  GATED DEPENDENCIES -- TU-local stand-ins for substrate not yet in the translated tree.
//  See SCK_ART_Remainder.h's file-head comment for the full rationale; each macro/stub re-cites its
//  golden home.
// =============================================================================

// ---- Gate #1: GetRecipePath()/GetRecipeFileName(AnsiString)/MyForceDirectories(AnsiString) --------
//   golden common.cpp:2039/2044 (bodies exist there) but common.h:287-300 gates them #if 0 in THIS
//   translated tree -- declared, not defined, so calling the real names would be a link error.
//   JUDGMENT CALL stand-in: skip the per-recipe subfolder (GetLastOpenFN(), itself gated) and persist
//   directly under the real DataPath global. See header gate #1 for the full rationale.
static AnsiString W5SckArtRem_GetRecipePath()
{
    return DataPath;                                       // golden common.cpp:2041 minus GetLastOpenFN()
}
static AnsiString W5SckArtRem_GetRecipeFileName(AnsiString FileName)
{
    return DataPath+FileName;                               // golden common.cpp:2046 minus GetLastOpenFN()
}
static int W5SckArtRem_MyForceDirectories(AnsiString /*Directory*/)
{
    return 0;                                                // golden common.cpp:1607-ish (gated) -- no-op
}
#define W5SCKARTREM_GETRECIPEPATH()            W5SckArtRem_GetRecipePath()
#define W5SCKARTREM_GETRECIPEFILENAME(name)    W5SckArtRem_GetRecipeFileName(name)
#define W5SCKARTREM_MYFORCEDIRECTORIES(dir)    W5SckArtRem_MyForceDirectories(dir)

// ---- Gate #2: TFTestIF::ReadTestIFFile() (golden cTesterIF.h, whole untranslated VCL form) --------
#define W5SCKARTREM_FTESTIF_READTESTIFFILE()   do { } while(0)   // golden FTestIF->ReadTestIFFile()

// ---- Gate #3: fMain->SetStartModeData()/LoadTestModePicture()/BackupSetupFile() -------------------
//   [RESOLVED -- AI(W5-Final-Integrate) 20260711] golden main.h (TfMain methods); INTEGRATE added
//   all 3 as offline no-op methods to FormsFacade.h/.cpp's TfMain (see FormsFacade.cpp) -- these
//   macros now just forward to the real facade calls (still no-ops offline, behavior-identical).
#define W5SCKARTREM_FMAIN_SETSTARTMODEDATA()      fMain->SetStartModeData()      // golden fMain->SetStartModeData()
#define W5SCKARTREM_FMAIN_LOADTESTMODEPICTURE()   fMain->LoadTestModePicture()   // golden fMain->LoadTestModePicture()
#define W5SCKARTREM_FMAIN_BACKUPSETUPFILE()       fMain->BackupSetupFile()       // golden fMain->BackupSetupFile() (Ifor 20170620)

// ---- Gate #4: 13 LastSet fields absent from canary_support.h's LAST_GENERAL_SET shim ---------------
//   golden LastSet.h line numbers verified directly (see header comment). TU-local zero-initialized
//   stand-ins. CROSS-UNIT NOTE: sibling SCK_ART.cpp already carries its OWN copies of
//   iSCKART_RTUnitCount / lSCKARTBinCT[256] -- 3-way reconciliation owed when the real LastSet lands
//   (this file / sibling SCK_ART.cpp / csystem.cpp's W7C2_TfSCKARTSeam).
static bool W5SckArtRem_LS_bBreakSCKART       = false;        // golden LastSet.h:420
static int  W5SckArtRem_LS_iSCKART_RTUnitCount= 0;            // golden LastSet.h:410
static long W5SckArtRem_LS_lSCKARTBinCT[256]  = {0};          // golden LastSet.h:507
static int  W5SckArtRem_LS_iHdPass            = 0;            // golden LastSet.h:20
static int  W5SckArtRem_LS_iHdFail             = 0;           // golden LastSet.h:21
static int  W5SckArtRem_LS_iHdRejectATPass     = 0;           // golden LastSet.h:22
static int  W5SckArtRem_LS_iHdRejectATFail     = 0;           // golden LastSet.h:23
static int  W5SckArtRem_LS_iHdRejectBT         = 0;           // golden LastSet.h:24
static int  W5SckArtRem_LS_iTesterMatch        = 0;           // golden LastSet.h:25
static int  W5SckArtRem_LS_iTesterUnmatch      = 0;           // golden LastSet.h:26
static int  W5SckArtRem_LS_iTestIgnore         = 0;           // golden LastSet.h:27
static int  W5SckArtRem_LS_iHdPickUp           = 0;           // golden LastSet.h:28
#define W5SCKARTREM_LS_BREAKSCKART        W5SckArtRem_LS_bBreakSCKART
#define W5SCKARTREM_LS_RTUNITCOUNT        W5SckArtRem_LS_iSCKART_RTUnitCount
#define W5SCKARTREM_LS_BINCT(i)           W5SckArtRem_LS_lSCKARTBinCT[i]
#define W5SCKARTREM_LS_HDPASS             W5SckArtRem_LS_iHdPass
#define W5SCKARTREM_LS_HDFAIL             W5SckArtRem_LS_iHdFail
#define W5SCKARTREM_LS_HDREJECT_ATPASS    W5SckArtRem_LS_iHdRejectATPass
#define W5SCKARTREM_LS_HDREJECT_ATFAIL    W5SckArtRem_LS_iHdRejectATFail
#define W5SCKARTREM_LS_HDREJECT_BT        W5SckArtRem_LS_iHdRejectBT
#define W5SCKARTREM_LS_TESTERMATCH        W5SckArtRem_LS_iTesterMatch
#define W5SCKARTREM_LS_TESTERUNMATCH      W5SckArtRem_LS_iTesterUnmatch
#define W5SCKARTREM_LS_TESTIGNORE         W5SckArtRem_LS_iTestIgnore
#define W5SCKARTREM_LS_HDPICKUP           W5SckArtRem_LS_iHdPickUp

// ---- Gate #5: LotSummary (golden cSocket.h `class TLotSummary`) -----------------------------------
//   cSocket.h/.cpp not translated at all yet. The struct itself (and the observable statics below) now
//   live in SCK_ART_Remainder.h -- see that header's gate #5 [UPDATE] note (AI(W906-SaveTestSummarySECS)
//   20260721) for the full extension rationale (SaveTestSummarySECS is the stub's first real reader).
W5SckArtRem_LotSummaryStub W5SckArtRem_LotSummary = {};
void W5SckArtRem_LotSummaryStub::ClearAllData()
{
    // golden cSocket.cpp:754-763 -- PARTIAL: only the 2 fields this wave's calc-core reads
    // (iCountCategory/iTotalCategory) are zeroed here. Golden's real body ALSO zeroes
    // iLastTotalCategory/iLoadTotal + 3 file-scope E1/E2/E3 error counters -- all still
    // untranslated/unconsumed by any code in this tree (same "not added, no consumer yet" judgment
    // call the ORIGINAL gate #5 note already made for this whole stub). iByLotLoadCount[5] is also
    // left untouched here, matching golden's OWN ClearAllData() body, which never touches it either
    // (golden zeroes iByLotLoadCount[] separately, element-by-element, only from ClearLotInfo :918).
    for(int i=0; i<MAX_SOCKET_ROW*MAX_SOCKET_COL; i++)
        for(int j=0; j<TEST_MAX_BIN; j++)
            iCountCategory[i][j]=0;
    for(int j=0; j<TEST_MAX_BIN; j++)
        iTotalCategory[j]=0;
}
#define W5SCKARTREM_LOTSUMMARY_ITEM(i)                 W5SckArtRem_LotSummary.iByLotLoadCount[i]
#define W5SCKARTREM_LOTSUMMARY_COUNTCATEGORY(site,bin) W5SckArtRem_LotSummary.iCountCategory[site][bin]   // golden LotSummary.iCountCategory[site][bin]
#define W5SCKARTREM_LOTSUMMARY_TOTALCATEGORY(bin)      W5SckArtRem_LotSummary.iTotalCategory[bin]         // golden LotSummary.iTotalCategory[bin]
#define W5SCKARTREM_LOTSUMMARY_CLEARALLDATA()          W5SckArtRem_LotSummary.ClearAllData()              // golden cSocket.cpp:754-763 -- NOW REAL (partial), see gate #5 [UPDATE] above

// ---- Gate #6: fTesterTCP->ProcessOSPrint() (golden Automation/TesterTCP.h/.cpp, untranslated) -----
#define W5SCKARTREM_FTESTERTCP_PROCESSOSPRINT()   do { } while(0)   // golden fTesterTCP->ProcessOSPrint()

// ---- Gate #7: 3 of the 4 giant report-writer bodies (golden :2045-4062) -- deferred to next wave ----
//   SaveTestSummarySECS (golden :1647-2044), the 4th, is now REAL -- see SckArtRem_SaveTestSummarySECS
//   below (AI(W906-SaveTestSummarySECS) 20260721). Its macro stand-in is retired (removed, not just
//   left unused) since SckArtRem_SaveTestSummary's SECS branch now calls the real function directly.
#define W5SCKARTREM_SAVE2DSORTINGSUMMARY(iSaveData)     do { (void)(iSaveData); } while(0)  // golden :3402-4062
#define W5SCKARTREM_SAVETESTSUMMARYTSV(iSaveData)       do { (void)(iSaveData); } while(0)  // golden :2805-3128
#define W5SCKARTREM_SAVESUMMARYTRAYFEED()               do { } while(0)                      // golden :3129-3401

// ---- Gate #11 [NEW -- AI(W906-SaveTestSummarySECS) 20260721]: ShellExecute(NULL,NULL,path,NULL,NULL,
//   SW_SHOW) -- golden SCK_ART.cpp:2027. See SCK_ART_Remainder.h's gate #11 doc comment for the full
//   "why a no-op, why observable" rationale.
AnsiString W5SckArtRem_LastShellExecuteOpenPath="<unset>";
static void W5SckArtRem_ShellExecuteOpen(AnsiString path)
{
    W5SckArtRem_LastShellExecuteOpenPath=path;   // ALWAYS records, even when path=="" -- this IS the
                                                  // golden quirk's observable signature (see the call
                                                  // site's own comment). No real launch (gate #11).
}
#define W5SCKARTREM_SHELLEXECUTE_OPEN(path)   W5SckArtRem_ShellExecuteOpen(path)

// ---- Gate #12 [NEW -- AI(W906-SaveTestSummarySECS) 20260721]: TimerTSV->Enabled=true (golden VCL
//   TTimer arm, SCK_ART.cpp:2038). See SCK_ART_Remainder.h's gate #12 doc comment.
#define W5SCKARTREM_TIMERTSV_ENABLE()   do { } while(0)   // golden TimerTSV->Enabled=true

// ---- Gate #8: WriteLastDataFile()/CustomerFunctionSelect()/RunInfo.AddAlarm() -- DISCOVERED LATE,
//   during this wave's own real link-test (not just -fsyntax-only), see translate report --------------
//   cprod.h DECLARES these (WriteLastDataFile: cprod.h:3237, CustomerFunctionSelect: cprod.h:3280,
//   RUN_INFO::AddAlarm: cprod.h:2755) and cprod.cpp contains their TEXT (WriteLastDataFile:1944-2043,
//   CustomerFunctionSelect:3686-3831, RUN_INFO::AddAlarm:984-1028) -- but ALL of cprod.cpp's function
//   BODIES from line 184 to 4036 are wrapped in a single blanket `#if 0 // TODO(W6)` (cprod.cpp:184/
//   4036, see the file's own "AI(W0-TAIL) 20260626: ==== begin gated function bodies" banner) -- i.e.
//   NONE of these three are actually linkable symbols in this translated tree yet, despite the golden
//   citations elsewhere in this file's history implying otherwise. A first draft of this file called
//   all three directly (compiled clean under -fsyntax-only, which does not check link-time symbol
//   existence) and only failed at an explicit real link+run smoke test performed for this hand-off --
//   see the translate report for why that extra step mattered. TU-local no-op stand-ins, matching the
//   FTestIF/fMain treatment (gate #2/#3): RUN_INFO RunInfo itself (the global instance) is real/active
//   (cprod.cpp:64, outside the gate) -- only its .AddAlarm() METHOD BODY is gated.
#define W5SCKARTREM_WRITELASTDATAFILE()            do { } while(0)   // golden cprod.cpp:1944 (TODO(W6)-gated)
#define W5SCKARTREM_CUSTOMERFUNCTIONSELECT()        do { } while(0)   // golden cprod.cpp:3686 (TODO(W6)-gated)
#define W5SCKARTREM_RUNINFO_ADDALARM(code, message) do { (void)(code); (void)(message); } while(0)  // golden RUN_INFO::AddAlarm, cprod.cpp:984 (TODO(W6)-gated)

// ---- Gate #9 [NEW -- AI(W906-DoARTLotStart) 20260721]: fMain->SetLotState(int) ---------------------
//   golden fMain->SetLotState(int) (SCK_ART.cpp golden :4249). csystem.cpp:2320 carries an EXACT macro
//   precedent for this identical call pattern -- W7C2_FMAIN_SETLOTSTATE(n), itself a pure no-op -- but
//   it is #define'd LOCAL to csystem.cpp, not reachable from this TU. TU-local no-op stand-in here
//   instead (same no-op behavior, same "TU-local macro" idiom as gates #1/#2/#6/#7/#8 above -- see
//   SCK_ART_Remainder.h's gate #9 for the full reasoning, including why gate #3's FormsFacade-forwarding
//   idiom was NOT used here).
#define W5SCKARTREM_FMAIN_SETLOTSTATE(n)            do { (void)(n); } while(0)   // golden fMain->SetLotState(int)

// ---- Gate #10 [NEW -- AI(W906-DoARTLotStart) 20260721]: fMain->tESDError->Add(AnsiString) ----------
//   golden fMain->tESDError->Add("MES07399") (SCK_ART.cpp golden :4251-4252). tESDError (golden
//   main.h:1394, TStringList* ESD-error-code queue) has ZERO hits anywhere in this translated tree
//   (verified by grep) -- a genuinely brand-new gap, no prior gate/macro/FormsFacade member to reuse.
//   TU-local no-op stand-in, same idiom as gate #9.
#define W5SCKARTREM_FMAIN_TESDERROR_ADD(code)       do { (void)(code); } while(0)   // golden fMain->tESDError->Add(AnsiString)

// =============================================================================
//  1. SetSetupFilePath -- golden TfSCKART::SetSetupFilePath() (SCK_ART.cpp:183-188)
// =============================================================================
void SckArtRem_SetSetupFilePath(SckArtRemainderState &st)
{
    st.sSetupFilePath=W5SCKARTREM_GETRECIPEPATH();                              // golden :185 GetRecipePath() -- TODO(wave-path), see gate #1
    W5SCKARTREM_MYFORCEDIRECTORIES(st.sSetupFilePath);                          // golden :186 MyForceDirectories() -- TODO(wave-file), see gate #1
    st.sSetupFilePath=W5SCKARTREM_GETRECIPEFILENAME(AnsiString("Tester.Data"));  // golden :187 GetRecipeFileName("Tester.Data") -- TODO(wave-path), see gate #1
}

// =============================================================================
//  2. AccessFile -- golden TfSCKART::AccessFile(bool,int) (SCK_ART.cpp:190-444)
//     Extract-calc-core: the widget-refresh block (golden :335-403) is NOT reproduced -- see
//     SCK_ART_Remainder.h's function-head comment. Every non-VCL read/write IS preserved.
// =============================================================================
void SckArtRem_AccessFile(SckArtRemainderState &st, bool bRead, int iAccess)
{
    bool bRefreshStartMode=bCanRunSCKART;                                        // golden :193 (bCanRunSCKART: real global, cmydef.cpp)

    if(CosFunction.bUseSCKART==false)                                           //Steven 20161201 : For SCK 93K ART
        return;

    SckArtRem_SetSetupFilePath(st);

    if(iAccess==-1 || iAccess==0 || iAccess==10)
    {
        st.sLOTSTATUS   =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "LOTSTATUS",      st.sLOTSTATUS,     AnsiString("NONE"), bRead);
        st.iCurrentStatus=ReadWriteIni(st.sSetupFilePath, "AutoRetest", "iCurrentStatus", st.iCurrentStatus, 1 /*iLOTSTATUS_W*/, bRead);
        st.iTesterType  =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "iTesterType",     st.iTesterType,    0,                  bRead);
        if(st.iTesterType==1)                                                   //Steven 20161201 (wei) : For SCK 93K ART
            CosFunction.bAutoRetestGPIBmode        =true;
        else
            CosFunction.bAutoRetestGPIBmode        =false;
    }

    if(iAccess==-1 || iAccess==1)
    {
        st.sLotID       =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "sLotID",          st.sLotID,          AnsiString(""), bRead);
        st.sProcessCode =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "sProceddCode",    st.sProcessCode,    AnsiString(""), bRead); //Steven 20190521 : ATK lot count
        st.iLotCount    =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "iLotCount",       st.iLotCount,       0,               bRead);
        st.iInputCount  =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "iInputCount",     st.iInputCount,     0,               bRead);
        st.iFTRTCount   =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "iFTRTCount",      st.iFTRTCount,      0,               bRead);
        st.iInputJamCnt =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "iInputJamCnt",    st.iInputJamCnt,    0,               bRead);
        st.iOutputJamCnt=ReadWriteIni(st.sSetupFilePath, "AutoRetest", "iOutputJamCnt",   st.iOutputJamCnt,   0,               bRead);
        st.iManualRejectCnt=ReadWriteIni(st.sSetupFilePath, "AutoRetest", "iManualRejectCnt", st.iManualRejectCnt, 0,          bRead);  //JerryYang 20220923 : SPIL手動輸入reject數量(要重測的)

        st.sInfo_Customer   =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "sInfoCustomer",     st.sInfo_Customer,   AnsiString(""), bRead);  //JerryYang 20220923 : add Lot Info for SPIL
        st.sInfo_CustLotID  =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "sInfoCustLotID",    st.sInfo_CustLotID,  AnsiString(""), bRead);
        st.sInfo_CustDevGup =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "sInfoCustDevGup",   st.sInfo_CustDevGup, AnsiString(""), bRead);
        st.sInfo_DeviceName =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "sInfoDeviceName",   st.sInfo_DeviceName,AnsiString(""), bRead);
        st.sInfo_Stage      =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "sInfoStage",        st.sInfo_Stage,     AnsiString(""), bRead);
        st.sInfo_Step       =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "sInfoStep",         st.sInfo_Step,      AnsiString(""), bRead);
        st.sInfo_ReportCnt  =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "sInfoReportCnt",    st.sInfo_ReportCnt, AnsiString(""), bRead);
        st.sInfo_ProgramName=ReadWriteIni(st.sSetupFilePath, "AutoRetest", "sInfoProgramName",  st.sInfo_ProgramName,AnsiString(""),bRead);
        st.sInfo_TestBinNo  =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "sInfoTestBinNo",    st.sInfo_TestBinNo, AnsiString(""), bRead);
        st.sInfo_TesterID   =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "sInfoTesterID",     st.sInfo_TesterID,  AnsiString(""), bRead);
        st.sInfo_HandlerID  =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "sInfoHandlerID",    st.sInfo_HandlerID, AnsiString(""), bRead);
        st.sInfo_Temperauture=ReadWriteIni(st.sSetupFilePath, "AutoRetest", "sInfoTemperauture",st.sInfo_Temperauture,AnsiString(""),bRead);
        st.sInfo_CurrQty    =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "sInfoCurrQty",      st.sInfo_CurrQty,   AnsiString(""), bRead);
        st.sInfo_OperatorID =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "sInfoOperatorID",   st.sInfo_OperatorID,AnsiString(""), bRead);
        st.sInfo_BinSet     =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "sInfo_BinSet",      st.sInfo_BinSet,    AnsiString(""), bRead);   //Jerryyang 20250529 : add

        st.iInfo_MultiLotCnt=ReadWriteIni(st.sSetupFilePath, "AutoRetest", "iInfo_MultiLotCnt", st.iInfo_MultiLotCnt, 0, bRead);

        st.sBundleList  =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "sBundleList",     st.sBundleList,     AnsiString(""), bRead);
        st.iBundleInCnt =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "iBundleInCnt",    st.iBundleInCnt,    12, bRead);
        st.iBundleOutCnt=ReadWriteIni(st.sSetupFilePath, "AutoRetest", "iBundleOutCnt",   st.iBundleOutCnt,   12, bRead);

        for(int i=0; i<5; i++)
        {
            AnsiString s1=IntToStr(i);
            st.sInfoArr_InnerLotID[i]  =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "sInfoArr_InnerLotID_"+s1,   st.sInfoArr_InnerLotID[i],   AnsiString(""), bRead);
            st.sInfoArr_Customer[i]    =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "sInfoArr_Customer_"+s1,     st.sInfoArr_Customer[i],     AnsiString(""), bRead);
            st.sInfoArr_CustLotID[i]   =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "sInfoArr_CustLotID_"+s1,    st.sInfoArr_CustLotID[i],    AnsiString(""), bRead);
            st.sInfoArr_CustDevGup[i]  =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "sInfoArr_CustDevGup_"+s1,   st.sInfoArr_CustDevGup[i],   AnsiString(""), bRead);
            st.sInfoArr_DeviceName[i]  =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "sInfoArr_DeviceName_"+s1,   st.sInfoArr_DeviceName[i],   AnsiString(""), bRead);
            st.sInfoArr_Stage[i]       =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "sInfoArr_Stage_"+s1,        st.sInfoArr_Stage[i],        AnsiString(""), bRead);
            st.sInfoArr_Step[i]        =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "sInfoArr_Step_"+s1,         st.sInfoArr_Step[i],         AnsiString(""), bRead);
            st.sInfoArr_ReportCnt[i]   =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "sInfoArr_ReportCnt_"+s1,    st.sInfoArr_ReportCnt[i],    AnsiString(""), bRead);
            st.sInfoArr_ProgramName[i] =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "sInfoArr_ProgramName_"+s1,  st.sInfoArr_ProgramName[i],  AnsiString(""), bRead);
            st.sInfoArr_TestBinNo[i]   =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "sInfoArr_TestBinNo_"+s1,    st.sInfoArr_TestBinNo[i],    AnsiString(""), bRead);
            st.sInfoArr_TesterID[i]    =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "sInfoArr_TesterID_"+s1,     st.sInfoArr_TesterID[i],     AnsiString(""), bRead);
            st.sInfoArr_HandlerID[i]   =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "sInfoArr_HandlerID_"+s1,    st.sInfoArr_HandlerID[i],    AnsiString(""), bRead);
            st.sInfoArr_Temperauture[i]=ReadWriteIni(st.sSetupFilePath, "AutoRetest", "sInfoArr_Temperauture_"+s1, st.sInfoArr_Temperauture[i], AnsiString(""), bRead);
            st.sInfoArr_CurrQty[i]     =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "sInfoArr_CurrQty_"+s1,      st.sInfoArr_CurrQty[i],      AnsiString(""), bRead);
            st.sInfoArr_OperatorID[i]  =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "sInfoArr_OperatorID_"+s1,   st.sInfoArr_OperatorID[i],   AnsiString(""), bRead);
            st.sInfoArr_BinSet[i]      =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "sInfoArr_BinSet_"+s1,       st.sInfoArr_BinSet[i],       AnsiString(""), bRead);      //Jerryyang 20250529 : add
        }

        st.sLotStartTime=ReadWriteIni(st.sSetupFilePath, "AutoRetest", "sLotStartTime", st.sLotStartTime, AnsiString(""), bRead);
        st.iManualStart =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "iManualStart",  st.iManualStart,  1,     bRead);  //RogerYang 20250918 : 瑞薩FT-CT
        st.bFirstFullSkip=ReadWriteIni(st.sSetupFilePath, "AutoRetest", "bFirstFullSkip", st.bFirstFullSkip, false, bRead);  //RogerYang 20251112 : ART數量減少,如果inarm上有IC正走到shuttle
    }

    if(iAccess==-1 || iAccess==10 || iAccess==20)
    {
        if(CUSTOMER_CODE==CC_AMKOR_Japan)                                       //RogerYang 20251108 : 瑞薩FT-CT 設定需要by機台
        {
            if(bRead==true)
            {
                TestIF_File.bSCKART_EnableART           =IniConfig.bA10_AutoReTest;
                TestIF_File.bRENESAS_EnableFTCT         =IniConfig.bA10_7_Renesas_FTCT;

                if(TestIF_File.bSCKART_EnableART==false && TestIF_File.bSCKART_EnableART==true)  //Ifor's note verbatim: 讀取時再檢查 若不對就資料要寫回 (golden :279, tautological condition preserved as-is)
                {
                    TestIF_File.bRENESAS_EnableFTCT=false;
                    IniConfig.bA10_7_Renesas_FTCT=false;
                    TestIF_File.bSCKART_RunARTWithoutCmd=true;

                    AnsiString sPath=AuthPath+"config.ini";
                    WriteIniData(sPath, "Function", "bA10_7_Renesas_FTCT",      IniConfig.bA10_7_Renesas_FTCT);
                }
            }
            TestIF_File.bSCKART_RunARTWithoutCmd=!TestIF_File.bRENESAS_EnableFTCT;   //沒開FTCT要全自動
        }
        else
        {
            TestIF_File.bSCKART_EnableART   =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "Enable ART",                  TestIF_File.bSCKART_EnableART,          IniConfig.bA10_AutoReTest, bRead);     //Steven 20170919 (wei) : Add for SCK ART
            TestIF_File.bSCKART_RunARTWithoutCmd=ReadWriteIni(st.sSetupFilePath, "AutoRetest", "Run ART Without Cmd", TestIF_File.bSCKART_RunARTWithoutCmd,   false,                     bRead);                         //Steven 20180830 : Run ART without tester command
        }

        if(TestIF_File.bSCKART_RunARTWithoutCmd)
        {
            TestIF_File.iSCKART_RTStartMode=1;
        }
        TestIF_File.dSCKART_Yield               =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "Pass Rate",            TestIF_File.dSCKART_Yield,              100.0,  bRead, true, 0.01,  100.0);     //Steven 20170327 (wei) : 預設值改為100
        TestIF_File.iSCKART_TryCnt              =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "Try Count",            int(TestIF_File.iSCKART_TryCnt),        3,      bRead, true, 1,     20);
        TestIF_File.bSCKART_AutoSkip            =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "Auto Skip",            TestIF_File.bSCKART_AutoSkip,           true,   bRead);                         //Steven 20170315 (wei) : For SCK ART Auto Skip
        TestIF_File.iSCKART_AutoSkipCount       =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "Auto Skip Count",      TestIF_File.iSCKART_AutoSkipCount,      5,      bRead, true, 1,     1000);      //Steven 20170315 (wei) : For SCK ART Auto Skip
        TestIF_File.iSCKART_MinAutoCloseSite    =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "Min Auto Close Site",  TestIF_File.iSCKART_MinAutoCloseSite,   2,      bRead, true, 1,     32);        //JerryYang 20220923 : 最小關site數量
        TestIF_File.iSCKART_SortMode            =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "Sort Mode",            TestIF_File.iSCKART_SortMode,           0,      bRead, true, 1,     0);
        TestIF_File.iSCKART_RTStartMode         =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "RT Start Mode",        TestIF_File.iSCKART_RTStartMode,        0,      bRead, true, 1,     0);
        TestIF_File.bSCKART_AutoCleanOut        =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "Auto Clean Out",       TestIF_File.bSCKART_AutoCleanOut,       true,   bRead);
        TestIF_File.bSCKART_AutoSocketOff       =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "Auto Socket Off",      TestIF_File.bSCKART_AutoSocketOff,      false,  bRead);
        TestIF_File.iSCKART_AutoSocketOffMode   =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "Auto Socket Off Mode", TestIF_File.iSCKART_AutoSocketOffMode,  0,      bRead, true,  0, 2);
        TestIF_File.dSCKART_AutoSocketOffYield  =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "Auto Socket Off Yield",TestIF_File.dSCKART_AutoSocketOffYield, 10.0,   bRead, true, 0.0, 100.0);       //Steven 20170327 (wei) : 預設值改為10.0
        TestIF_File.bSCKART_AutoLotEnd          =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "Auto Lot End",         TestIF_File.bSCKART_AutoLotEnd,         true,   bRead);
        TestIF_File.bSCKART_FinalLotEndMsg      =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "Final Lot End Msg",    TestIF_File.bSCKART_FinalLotEndMsg,     false,  bRead);
        TestIF_File.iSCKLdCntLimN               =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "iSCKLdCntLimN",        TestIF_File.iSCKLdCntLimN,              0,      bRead);  //RogerYang 20251224 : Rf360需求 lotcheck加入上下限設定
        TestIF_File.iSCKLdCntLimP               =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "iSCKLdCntLimP",        TestIF_File.iSCKLdCntLimP,              0,      bRead);  //RogerYang 20251224 : Rf360需求 lotcheck加入上下限設定

        if(CUSTOMER_CODE==CC_RF360 ||
           CUSTOMER_CODE==CC_QUALCOMM)                                          //Steven 20251104 : for QTI
            TestIF_File.bSCKART_LotDeviceCheck  =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "Lot Device Check",     TestIF_File.bSCKART_LotDeviceCheck,     false,  bRead);
        else if(W5SCKARTREM_LS_BREAKSCKART)                                     //Sam 20240402 : 修正 BreakSCKART 問題。 -- golden LastSet.bBreakSCKART, TODO(W7-LastSet), see gate #4
            TestIF_File.bSCKART_LotDeviceCheck  =false;
        else
            TestIF_File.bSCKART_LotDeviceCheck  =true;

        TestIF_File.bSCKART_EnableSPBinAlarm    =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "Enable Special Bin Alarm", TestIF_File.bSCKART_EnableSPBinAlarm,   false,  bRead);                  //Isaac 20171113 (Steven) : add ATK Special Bin Yield alarm
        TestIF_File.iSCKART_SPBinSelect         =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "Special Bin Select",       TestIF_File.iSCKART_SPBinSelect,        0,      bRead, true, 1,     255);
        TestIF_File.dSCKART_SPBinAlarmYield     =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "Special Bin Alarm Yield",  TestIF_File.dSCKART_SPBinAlarmYield,    0.00,   bRead, true, 0.00,  100.00);
        TestIF_File.bLowYieldForArtFT           =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "Enable Low Yield Limit",   TestIF_File.bLowYieldForArtFT,          false,  bRead);     //Steven 20220208 : Add for ATK, ART low Yield
        TestIF_File.dLowYieldForArtFT           =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "Yield of Low Yield Limit", TestIF_File.dLowYieldForArtFT,          0.00,   bRead, true, 0.00,  100.00);
        TestIF_File.bAlarmAfterSendSRQKIND2     =ReadWriteIni(st.sSetupFilePath, "AutoRetest", "bAlarmAfterSendSRQKIND2",  TestIF_File.bAlarmAfterSendSRQKIND2,    false,  bRead);     //Steven 20230119 : ATK 要在SRQKIND2執行Alarm

        // golden :332 SetGPIBVersion(edtVersion->Text) -- commented out IN GOLDEN ITSELF (verbatim,
        // not a translation gap): "Jimmychiu 20231205 : Mark SetGPIBVersion function".
    }

    // golden :335-403 widget-refresh block (palXxx->Caption=/edXxx->Text=/cbXxx->Checked=/ItemIndex=,
    // ~35 pure VCL writes, zero consumed elsewhere) intentionally NOT reproduced -- extract-calc-core,
    // see SCK_ART_Remainder.h function-head comment.

    if(USE_AUTO_RETEST==eartInstall       &&
       CosFunction.bUseSCKART==true       &&
       IniConfig.bA10_AutoReTest==true    &&
       IniConfig.bA10TestModeForART==true)
    {
        if(TestIF_File.iGpibMode!=IniConfig.iA10TestModeForART)
        {
            TestIF_File.iGpibMode=IniConfig.iA10TestModeForART;
        }
    }

    if(USE_AUTO_RETEST==eartInstall       &&
       CosFunction.bUseSCKART==true       &&
       IniConfig.bA10_AutoReTest==true    &&
       TestIF_File.bSCKART_EnableART)
    {
        bCanRunSCKART=true;
    }
    else
    {
        bCanRunSCKART=false;
    }

    if(bRefreshStartMode!=bCanRunSCKART)                                        //Steven 20170209 : 修正開啟ART時,要重新整理Start Mode
    {
        W5SCKARTREM_FMAIN_SETSTARTMODEDATA();                                   // golden :429 fMain->SetStartModeData() -- TODO(W7-FormsFacade), see gate #3
    }

    W5SCKARTREM_FTESTIF_READTESTIFFILE();                                       // golden :432 FTestIF->ReadTestIFFile() -- TODO(W7), see gate #2
    W5SCKARTREM_FMAIN_LOADTESTMODEPICTURE();                                    // golden :433 fMain->LoadTestModePicture() -- TODO(W7-FormsFacade), see gate #3

    if(bRead==false)
        W5SCKARTREM_WRITELASTDATAFILE();                                        // golden :436 WriteLastDataFile() -- TODO(W6), see gate #8
    W5SCKARTREM_CUSTOMERFUNCTIONSELECT();                                        // golden :437 CustomerFunctionSelect() -- TODO(W6), see gate #8
    SckArtRem_UpdateCount(st);                                                   // golden :438 UpdateCount()

    if(bRead==false)
    {
        W5SCKARTREM_FMAIN_BACKUPSETUPFILE();                                    // golden :442 fMain->BackupSetupFile() -- TODO(W7-FormsFacade), see gate #3
    }
}

// =============================================================================
//  3. SetGPIBVersion -- golden TfSCKART::SetGPIBVersion(AnsiString) (SCK_ART.cpp:446-475)
// =============================================================================
void SckArtRem_SetGPIBVersion(SckArtRemainderState & /*st*/, AnsiString Str)
{
    if(CosFunction.bAmkorFunction || CUSTOMER_CODE==CC_QUALCOMM)                //Richard 20230428 : Enable sSCKART_VersionCmd
    {
        if(IniConfig.bA10TestModeForART==false)
        {
            WriteIniData(AnsiString("D:\\GPIB9045\\system\\general.ini"), "SystemSetup", "Version",  Str);
        }
        else
        {
            if(TestIF_File.iGpibMode==InterfaceType_16BinGS || TestIF_File.iGpibMode==InterfaceType_32BinGS)
            {
                WriteIniData(AnsiString("D:\\GPIB9045\\system\\general.ini"), "SystemSetup", "Version", "ADVANTEST M4871 Rev.1.1P3 FULLSITEB");
            }
            else
            {
                WriteIniData(AnsiString("D:\\GPIB9045\\system\\general.ini"), "SystemSetup", "Version", "IFUNT200 Version B17");
            }
        }
    }

    if(TestIF_File.iGpibMode==InterfaceType_16BinGS || TestIF_File.iGpibMode==InterfaceType_32BinGS)
    {
        TestIF_File.sSCKART_VersionCmd=ReadWriteIni(AnsiString("D:\\GPIB9045\\system\\general.ini"), AnsiString("SystemSetup"), AnsiString("Version"), TestIF_File.sSCKART_VersionCmd, AnsiString("ADVANTEST M4871 Rev.1.1P3 FULLSITEB"), true);
    }
    else
    {
        TestIF_File.sSCKART_VersionCmd=ReadWriteIni(AnsiString("D:\\GPIB9045\\system\\general.ini"), AnsiString("SystemSetup"), AnsiString("Version"), TestIF_File.sSCKART_VersionCmd, AnsiString("IFUNT200 Version B17"), true);
    }
}

// =============================================================================
//  4. UpdateCount -- golden TfSCKART::UpdateCount() (SCK_ART.cpp:477-591)
//     Extract-calc-core: every palXxx->Caption=/->Visible= VCL write skipped (see header comment).
// =============================================================================
void SckArtRem_UpdateCount(SckArtRemainderState &st)
{
    int iUnloadCnt=0;
    int iTotalUnloadCount=0;

    for(int i=0; i<10; i++)
    {
        iTotalUnloadCount+=W5SCKARTREM_LS_BINCT(i);                              // golden :485 LastSet.lSCKARTBinCT[i] -- TODO(W7-LastSet), see gate #4
    }

    if(LastSet.iTester==OFF_LINE)
    {
        if(BinSelect[OffT].bAutoRetest[0])
            iUnloadCnt+=W5SCKARTREM_LS_BINCT(0);                                //Steven 20170113 : For count unload count
        if(BinSelect[OffT].bAutoRetest[1])
            iUnloadCnt+=W5SCKARTREM_LS_BINCT(1);
        if(BinSelect[OffT].bAutoRetest[2])
            iUnloadCnt+=W5SCKARTREM_LS_BINCT(2);
    }
    else
    {
        if(BinSelect[FT].bAutoRetest[0])
            iUnloadCnt+=W5SCKARTREM_LS_BINCT(0);                                //Steven 20170113 : For count unload count
        if(BinSelect[FT].bAutoRetest[1])
            iUnloadCnt+=W5SCKARTREM_LS_BINCT(1);
        if(BinSelect[FT].bAutoRetest[2])
            iUnloadCnt+=W5SCKARTREM_LS_BINCT(2);
    }

    if(TestIF_File.bRENESAS_EnableFTCT==true)                                   //RogerYang 20250923 : 瑞薩FT-CT
    {
        W5SCKARTREM_LS_HDPASS       =iTotalUnloadCount-iUnloadCnt;              // golden :527 LastSet.iHdPass -- TODO(W7-LastSet), see gate #4
        W5SCKARTREM_LS_HDFAIL       =iUnloadCnt;                                // golden :528 LastSet.iHdFail
        W5SCKARTREM_LS_HDREJECT_BT  =st.iInputJamCnt;                          // golden :530 LastSet.iHdRejectBT
        W5SCKARTREM_LS_HDPICKUP     =LastSet.iSCKARTInputCT;                    // golden :531 LastSet.iHdPickUp
    }

    if(TestIF_File.bSCKART_LotDeviceCheck==false)                               //Steven 20251124 : for disable LotDeviceCheck
    {
        if(st.iInputCount!=0 &&
           LastSet.iSCKARTInputCT!=0 &&                                         //Steven 20250307 : 要有出貨才行
           iUnloadCnt==0)                                                       //Steven 20241002 : 修正ART Yield 100% 誤報alarm
        {
            st.dCurrYield=100.0;
        }
        else if(st.iInputCount!=0 &&
                iUnloadCnt!=0  &&                                               //Steven 20241101 : 要有出貨才行
                LastSet.iSCKARTInputCT-iUnloadCnt==0)                           //Steven 20241002 : 修正ART Yield 100% 誤報alarm
        {
            st.dCurrYield=0.0;
        }
        else
        {
            if(LastSet.iSCKARTInputCT==0)                                       //Steven 20170327 (wei) : Fixed for ART count
            {
                st.dCurrYield=0.0;
            }
            else
            {
                st.dCurrYield=double(LastSet.iSCKARTInputCT-iUnloadCnt)/double(LastSet.iSCKARTInputCT)*100.0;
            }
        }
    }
    else
    {
        if(st.iInputCount!=0 &&
           LastSet.iSCKARTInputCT!=0 &&                                         //Steven 20250307 : 要有出貨才行
           iUnloadCnt==0)                                                       //Steven 20241002 : 修正ART Yield 100% 誤報alarm
        {
            st.dCurrYield=100.0;
        }
        else if(st.iInputCount!=0 &&
                iUnloadCnt!=0  &&                                               //Steven 20241101 : 要有出貨才行
                st.iLotCount-iUnloadCnt==0)                                      //Steven 20241002 : 修正ART Yield 100% 誤報alarm
        {
            st.dCurrYield=0.0;
        }
        else
        {
            if(st.iLotCount==0)                                                 //Steven 20170327 (wei) : Fixed for ART count
            {
                st.dCurrYield=0.0;
            }
            else
            {
                st.dCurrYield=double(st.iLotCount-iUnloadCnt)/double(st.iLotCount)*100.0;
            }
        }
    }

    // golden :586-591 (Str.sprintf+palCurrentRate/palLotStatus/lblHandlerStatus widget writes)
    // intentionally NOT reproduced -- pure VCL, see header comment.
}

// =============================================================================
//  5. AddAlarmCode -- golden TfSCKART::AddAlarmCode(AnsiString,AnsiString,int) (SCK_ART.cpp:593-624)
//     Extract-calc-core: memoAlarmList Add/SaveToFile (VCL alarm-history journal) skipped; the
//     RunInfo.AddAlarm() tail (the only piece OTHER logic consumes) is preserved unconditionally --
//     see SCK_ART_Remainder.h function-head comment (golden's CC_SCK/iTesterType!=0 check only
//     gated the skipped memo branch, never this tail).
// =============================================================================
void SckArtRem_AddAlarmCode(AnsiString Code, AnsiString Message, int iDuplicate)
{
    if(iDuplicate==0 &&
       Code.AnsiPos("JAM")!=0)
    {
        W5SCKARTREM_RUNINFO_ADDALARM(Code, Message);                            // golden :622 RunInfo.AddAlarm(Code,Message) -- TODO(W6), see gate #8
    }
}

// =============================================================================
//  6. ClearLotInfo -- golden TfSCKART::ClearLotInfo() (SCK_ART.cpp:837-923)
// =============================================================================
void SckArtRem_ClearLotInfo(SckArtRemainderState &st, bool *outNeedAccessFileWrite)
{
    st.iNeedRT         =1;
    st.iFTRTCount       =0;
    st.iManualRejectCnt=0;                                                      //JerryYang 20220923 : SPIL手動輸入reject數量(要重測的)

    if(TestIF_File.bRENESAS_EnableFTCT==true)                                   //RogerYang 20250911 : 瑞薩FT-CT 不在這清掉
    {
    }
    else
    {
        st.iLotCount    =0;
        st.iInputCount  =0;
        st.sLotID       ="";
        st.sProcessCode ="";
        st.sLotStartTime="";
        // golden :853 fSCKART->SetLotStatus(iLOTSTATUS_NONE) -- inlined (iLOTSTATUS_NONE==0, the same
        // "NONE"/0 pair the sibling file's SetLotStatus `default:` branch produces for any unmapped
        // iStatus). See SCK_ART_Remainder.h function-head comment for why this does not call a second
        // SetLotStatus implementation.
        st.sLOTSTATUS     ="NONE";
        st.iCurrentStatus =0;
    }
    W5SCKARTREM_LS_RTUNITCOUNT=0;                                                //Steven 20170327 (wei) : Fixed for ART count -- golden :855 LastSet.iSCKART_RTUnitCount, TODO(W7-LastSet), see gate #4
    st.iInputJamCnt =0;
    st.iOutputJamCnt=0;

    for(int i=0; i<10; i++)
    {
        W5SCKARTREM_LS_BINCT(i)=0;                                               // golden :861 LastSet.lSCKARTBinCT[i], TODO(W7-LastSet), see gate #4
    }
    LastSet.iSCKARTInputCT=0;                                                    //Steven 20170327 (wei) : Fixed for ART count
    LastSet.lShuttleCount=0;
    W5SCKARTREM_LS_HDPASS=0;                                                     //RogerYang 20250923 : 瑞薩FT-CT -- golden :865, TODO(W7-LastSet), see gate #4
    W5SCKARTREM_LS_HDFAIL=0;                                                     //RogerYang 20250923 : 瑞薩FT-CT
    W5SCKARTREM_LS_HDREJECT_ATPASS=0;                                            //RogerYang 20250923 : 瑞薩FT-CT
    W5SCKARTREM_LS_HDREJECT_ATFAIL=0;                                            //RogerYang 20250923 : 瑞薩FT-CT
    W5SCKARTREM_LS_HDREJECT_BT=0;                                                //RogerYang 20250923 : 瑞薩FT-CT
    W5SCKARTREM_LS_TESTERMATCH=0;                                                //RogerYang 20250923 : 瑞薩FT-CT
    W5SCKARTREM_LS_TESTERUNMATCH=0;                                              //RogerYang 20250923 : 瑞薩FT-CT
    W5SCKARTREM_LS_TESTIGNORE=0;                                                 //RogerYang 20250923 : 瑞薩FT-CT
    W5SCKARTREM_LS_HDPICKUP=0;                                                   //RogerYang 20250923 : 瑞薩FT-CT

    W5SCKARTREM_LOTSUMMARY_CLEARALLDATA();                                       // golden :875 LotSummary.ClearAllData() -- AI(W906-SaveTestSummarySECS) 20260721: now REAL (partial: iCountCategory/iTotalCategory only), see gate #5 [UPDATE]

    // NOTE: golden :877 also calls `DoAutoSocketOff(true)` here. That function is ALREADY translated
    // by the sibling file as `SckArt_DoAutoSocketOff(SckArtState&, bool)` (Automation/SCK_ART.h/.cpp).
    // This remainder deliberately does NOT call it (it lives on a DIFFERENT state struct --
    // SckArtState, not SckArtRemainderState -- see the "WHY A SEPARATE STATE STRUCT" cross-file note).
    // JUDGMENT CALL, flagged for the integrate agent / human review: the caller of
    // SckArtRem_ClearLotInfo is responsible for ALSO invoking the sibling's
    // `SckArt_DoAutoSocketOff(siblingSt, true)` on whatever SckArtState instance is being kept in sync
    // with this one, until the two state structs are unified onto one FormsFacade TfSCKART.

    st.sInfo_Customer="";                                                       //JerryYang 20220331 : 客戶要求不要Show NA而顯示空白
    st.sInfo_InnerLotID="";
    st.sInfo_CustLotID="";
    st.sInfo_CustDevGup="";
    st.sInfo_DeviceName="";
    st.sInfo_Stage="";
    st.sInfo_Step="";
    st.sInfo_ReportCnt="";
    st.sInfo_ProgramName="";
    st.sInfo_TestBinNo="";
    st.sInfo_TesterID="";
    st.sInfo_HandlerID="";
    st.sInfo_Temperauture="";
    st.sInfo_CurrQty="";
    st.sInfo_OperatorID="";
    st.sInfo_BinSet="";                                                         //Jerryyang 20250529 : add
    st.iInfo_MultiLotCnt=0;

    st.sBundleList="";

    for(int i=0; i<5; i++)
    {
        st.sInfoArr_Customer[i]="";                                             //JerryYang 20220331 : 客戶要求不要Show NA而顯示空白
        st.sInfoArr_InnerLotID[i]="";
        st.sInfoArr_CustLotID[i]="";
        st.sInfoArr_CustDevGup[i]="";
        st.sInfoArr_DeviceName[i]="";
        st.sInfoArr_Stage[i]="";
        st.sInfoArr_Step[i]="";
        st.sInfoArr_ReportCnt[i]="";
        st.sInfoArr_ProgramName[i]="";
        st.sInfoArr_TestBinNo[i]="";
        st.sInfoArr_TesterID[i]="";
        st.sInfoArr_HandlerID[i]="";
        st.sInfoArr_Temperauture[i]="";
        st.sInfoArr_CurrQty[i]="";
        st.sInfoArr_OperatorID[i]="";
        st.sInfoArr_BinSet[i]="";                                               //Jerryyang 20250529 : add
        W5SCKARTREM_LOTSUMMARY_ITEM(i)=0;                                        // golden :918 LotSummary.iByLotLoadCount[i], TODO(W7/cSocket), see gate #5
    }

    // golden :921 fSCKART->AccessFile(false) -- exposed as an out-param rather than called here (see
    // header comment: calling SckArtRem_AccessFile(st,...) recursively from inside this function would
    // re-enter on the SAME st reference mid-mutation; the caller decides ordering instead).
    if(outNeedAccessFileWrite!=0)
        *outNeedAccessFileWrite=true;

    bWaitTSV=false;                                                             // golden :922 (real global, cmydef.cpp)
}

// =============================================================================
//  7. AddOutputJamCnt -- golden TfSCKART::AddOutputJamCnt(int,int,int,int) (SCK_ART.cpp:1361-1389)
// =============================================================================
void SckArtRem_AddOutputJamCnt(SckArtRemainderState &st, int row, int col, int ret, int iBinOnCarryKit)
{
    st.iOutputJamCnt++;
    if(TestIF_File.bRENESAS_EnableFTCT==true && ret==K_SKIP)
    {
        if(iBinOnCarryKit!=0)                                                   //RogerYang 20260226 : 修正計算 //PickUp Error發生在Shuttle上
        {
            if(iBinOnCarryKit==1)   //(bIsPassBinOnCarryKit==true?)1:2;
            {
                W5SCKARTREM_LS_HDREJECT_ATPASS++;                               // golden :1370 LastSet.iHdRejectATPass, TODO(W7-LastSet), see gate #4
            }
            else
            {
                W5SCKARTREM_LS_HDREJECT_ATFAIL++;                               // golden :1374
            }
        }
        else
        {
            if(Prod.bIsPassBin[OutArmSuck.iBinData[row][col]]==true)
            {
                W5SCKARTREM_LS_HDREJECT_ATPASS++;                               // golden :1381
            }
            else
            {
                W5SCKARTREM_LS_HDREJECT_ATFAIL++;                               // golden :1385
            }
        }
    }
}

// =============================================================================
//  8. SaveTestSummary -- golden TfSCKART::SaveTestSummary(int) (SCK_ART.cpp:1619-1645)
//     Dispatch logic verbatim; 3 of the 4 callees remain gated (see gate #7 -- deferred to next wave).
//     AI(W906-SaveTestSummarySECS) 20260721: added the `st` parameter, and the SECS branch now calls
//     the real SckArtRem_SaveTestSummarySECS below instead of the retired W5SCKARTREM_SAVETESTSUMMARYSECS
//     macro -- see this function's own header doc comment for why widening the signature is safe.
// =============================================================================
void SckArtRem_SaveTestSummary(SckArtRemainderState &st, int iSaveData)
{
    if(CosFunction.bSortingBy2DList==true &&                                    //Steven 20240830 : 簡易2D sort summary判斷式
       LastSet.iTester==_2D_SORT &&                                             //JerryYang 20230322 : SPIL 2D SORT summary修改
       TestIF_File.bSortingBy2DIDList==true)
    {
        W5SCKARTREM_SAVE2DSORTINGSUMMARY(iSaveData);                            // golden :1625 Save2DSortingSummary(iSaveData) -- TODO(next wave), see gate #7
    }
    else if(CosFunction.bART_SECSGEM_93K==true)
    {
        SckArtRem_SaveTestSummarySECS(st, iSaveData);                          // golden :1629 SaveTestSummarySECS(iSaveData) -- AI(W906-SaveTestSummarySECS) 20260721: now real, see below
    }
    else if(TestIF_File.iTestType==TCP_IP_MODE)
    {
        if(iSaveData)
            W5SCKARTREM_FTESTERTCP_PROCESSOSPRINT();                            // golden :1634 fTesterTCP->ProcessOSPrint() -- TODO(W5-TesterTCP), see gate #6

        W5SCKARTREM_SAVETESTSUMMARYTSV(iSaveData);                              // golden :1636 SaveTestSummaryTSV(iSaveData) -- TODO(next wave), see gate #7
    }
    else //if(IniConfig.bN09_LotCountAutoFunc)                                  //Steven 20240830 : N09獨立出來 (golden's own commented-out condition, verbatim)
    {
        if(iSaveData==1)
            W5SCKARTREM_SAVESUMMARYTRAYFEED();                                  // golden :1641 SaveSummaryTrayFeed() -- TODO(next wave), see gate #7

        W5SCKARTREM_SAVETESTSUMMARYTSV(iSaveData);                              // golden :1643 SaveTestSummaryTSV(iSaveData) -- TODO(next wave), see gate #7
    }
}

// =============================================================================
//  9. SaveTestSummarySECS -- golden TfSCKART::SaveTestSummarySECS(int) (SCK_ART.cpp:1647-2044)
//     AI(W906-SaveTestSummarySECS) 20260721. See SCK_ART_Remainder.h's own doc comment on this
//     declaration for the full dependency-verification writeup, the golden bugs preserved verbatim,
//     and the 7 dead golden locals dropped (extract-calc-core: mapIPList/mapIPIter/IP/tmps1/Data/
//     plain PathName/lowercase str2+str3 -- none read or written anywhere in golden :1647-2044 beyond
//     their own declaration).
// =============================================================================
void SckArtRem_SaveTestSummarySECS(SckArtRemainderState &st, int iSaveData)     //Steven 20190521 : ATK lot count
{
    AnsiString FileName, PathName2, Str, Str1, Str2, strPath, strFileName="";
    AnsiString sTestBinNo=StringReplace(st.sInfo_TestBinNo, ":", "-", TReplaceFlags()<<rfReplaceAll);
    int iCount=0, iUnloadCount=0;
    bool bResult=false;
    AnsiString sLotID1=st.sLotID;

    if(sLotID1=="" && fLotInfo->edtSysLotID->Text!="")                          //Steven 20230317 : 避免lot ID是空值
        sLotID1=fLotInfo->edtSysLotID->Text;
    if(sLotID1=="")                                                             //Steven 20230814 : 沒有Lot ID, 就帶入日期時間
        sLotID1.sprintf("%04d%02d%02d%02d%02d%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);

    bool bHaveBinData=false;
    int temp;
    AnsiString aUnloader[eTrayCount];
    int iUnloadCnt[eTrayCount]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0};
    GetTimeInfo();

    st.sLotEndTime.sprintf("%04d%02d%02d%02d%02d%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);

    PathName2.sprintf("%s\\%04d\\%02d\\", asSummaryPath, SystemYear, SystemMonth);    //Steven 20230215 : 存檔路徑加上年月
    // AI(W906-SaveTestSummarySECS) 20260721: REAL MyForceDirectories(AnsiString,AnsiString="") (common.h
    // :321/common.cpp:1648, un-gated by a SEPARATE same-day AI(W906-CommonWaveFile) pass) -- NOT gate
    // #1's TU-local stand-in (which SckArtRem_SetSetupFilePath above still uses; retargeting that is out
    // of THIS wave's scope). Golden's own 2-arg call shape preserved verbatim.
    MyForceDirectories(PathName2, "TfSCKART::SaveTestSummarySECS");

    if(st.sProcessCode=="")
        st.sProcessCode="FT1";

    if(IniConfig.bSPILFunction==true)
    {
        // AI(W906-SaveTestSummarySECS) 20260721: golden bug preserved VERBATIM -- this if/else (golden
        // :1682-1691) builds the EXACT SAME FileName.sprintf(...) call in BOTH arms (byte-for-byte
        // identical argument list); a dead branch that differentiates nothing. See this function's own
        // header doc comment ("Golden bug preserved VERBATIM #2"). NOT collapsed into one arm.
        if(IniConfig.bA38_SLT_Summary)
        {
            FileName.sprintf("%s_%s_%s_%s_%s_%s_%s_%s_%s.txt",  st.sInfo_Stage, st.sInfo_Step, st.sInfo_ReportCnt, st.sInfo_CustLotID,
                                                                sLotID1, st.sInfo_TesterID, st.sInfo_HandlerID, sTestBinNo, st.sLotEndTime);
        }
        else
        {
            FileName.sprintf("%s_%s_%s_%s_%s_%s_%s_%s_%s.txt",  st.sInfo_Stage, st.sInfo_Step, st.sInfo_ReportCnt, st.sInfo_CustLotID,
                                                                sLotID1, st.sInfo_TesterID, st.sInfo_HandlerID, sTestBinNo, st.sLotEndTime);
        }
    }
    else
    {
        FileName.sprintf("%s_%s_%s_%s_%s_%s_%s_%s_%04d%02d%02d%02d%02d.txt", st.sInfo_Stage, st.sInfo_Step, st.sInfo_ReportCnt, st.sInfo_CustLotID,
                                                                             sLotID1, st.sInfo_TesterID, st.sInfo_HandlerID, sTestBinNo, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin);
    }

    if(iSaveData==0)
        return;

    TStringList *sList=new TStringList();

    if(IniConfig.bSPILFunction==true)                                           //JerryYang 20240705 : 丁曉冬要求的格式
    {
        Str.sprintf("CUSTOMER:%s", st.sInfo_Customer);
        sList->Add(Str);
        Str.sprintf("STAGE:%s", st.sInfo_Stage);
        sList->Add(Str);
        Str.sprintf("INNER_LOT_ID:%s", sLotID1);
        sList->Add(Str);
        Str.sprintf("CURR_QTY:%s", st.sInfo_CurrQty);
        sList->Add(Str);
        Str.sprintf("TEST_BIN_NO:%s", st.sInfo_TestBinNo);
        sList->Add(Str);
        Str.sprintf("DEVICE_NAME:%s", st.sInfo_DeviceName);
        sList->Add(Str);
        Str.sprintf("STEP:%s", st.sInfo_Step);
        sList->Add(Str);
        Str.sprintf("CUST_LOT_ID:%s", st.sInfo_CustLotID);
        sList->Add(Str);
        Str.sprintf("OPERATOR_ID:%s", st.sInfo_OperatorID);
        sList->Add(Str);
        Str.sprintf("TEMPERATURE:%s", st.sInfo_Temperauture);
        sList->Add(Str);
        Str.sprintf("REPORTCOUNT:%s", st.sInfo_ReportCnt);
        sList->Add(Str);
        Str.sprintf("PROGRAM_NAME:%s", st.sInfo_ProgramName);
        sList->Add(Str);
        Str.sprintf("HANDLER_ID:%s", st.sInfo_HandlerID);
        sList->Add(Str);
        Str.sprintf("TESTER_ID:%s", st.sInfo_TesterID);
        sList->Add(Str);
        Str.sprintf("CUSTOMER_DEVICE_GROUP:%s", st.sInfo_CustDevGup);
        sList->Add(Str);

        if(st.sLotStartTime!="")                                          //Richard 20230111 : Fix fSCKART->sLotStartTime 為NULL。
        {
            Str.sprintf("SUMMARY_START_TIME:%s", st.sLotStartTime);
        }
        else
        {
            Str.sprintf("SUMMARY_START_TIME:%s%s%s%s%s%s", RunInfo.LotStartTime.SubString(1, 4),  RunInfo.LotStartTime.SubString(6, 2),  RunInfo.LotStartTime.SubString(9, 2),
                                                           RunInfo.LotStartTime.SubString(12, 2), RunInfo.LotStartTime.SubString(15, 2), RunInfo.LotStartTime.SubString(18, 2));
        }
        sList->Add(Str);
        Str.sprintf("SUMMARY_END_TIME:%s", st.sLotEndTime);
        sList->Add(Str);
    }
    else
    {
        Str.sprintf("CUSTOMER:%s", st.sInfo_Customer);
        sList->Add(Str);
        Str.sprintf("INNER_LOT_ID:%s", sLotID1);
        sList->Add(Str);
        Str.sprintf("CUST_LOT_ID:%s", st.sInfo_CustLotID);
        sList->Add(Str);
        Str.sprintf("CUSTOMER_DEVICE_GROUP:%s", st.sInfo_CustDevGup);
        sList->Add(Str);
        Str.sprintf("DEVICE_NAME:%s", st.sInfo_DeviceName);
        sList->Add(Str);
        Str.sprintf("STAGE:%s", st.sInfo_Stage);
        sList->Add(Str);
        Str.sprintf("STEP:%s", st.sInfo_Step);
        sList->Add(Str);
        Str.sprintf("REPORTCOUNT:%s", st.sInfo_ReportCnt);
        sList->Add(Str);
        Str.sprintf("PROGRAM_NAME:%s", st.sInfo_ProgramName);
        sList->Add(Str);
        Str.sprintf("TEST_BIN_NO:%s", st.sInfo_TestBinNo);
        sList->Add(Str);
        Str.sprintf("TESTER_ID:%s", st.sInfo_TesterID);
        sList->Add(Str);
        Str.sprintf("HANDLER_ID:%s", st.sInfo_HandlerID);
        sList->Add(Str);
        Str.sprintf("TEMPERATURE:%s", st.sInfo_Temperauture);
        sList->Add(Str);
        Str.sprintf("CURR_QTY:%s", st.sInfo_CurrQty);
        sList->Add(Str);
        Str.sprintf("OPERATOR_ID:%s", st.sInfo_OperatorID);
        sList->Add(Str);
        if(st.sLotStartTime!="")                                          //Richard 20230111 : Fix fSCKART->sLotStartTime 為NULL。
        {
            Str.sprintf("SUMMARY_START_TIME:%s", st.sLotStartTime);
        }
        else
        {
            Str.sprintf("SUMMARY_START_TIME:%s%s%s%s%s%s", RunInfo.LotStartTime.SubString(1, 4), RunInfo.LotStartTime.SubString(6, 2), RunInfo.LotStartTime.SubString(9, 2),
                                                           RunInfo.LotStartTime.SubString(12, 2), RunInfo.LotStartTime.SubString(15, 2), RunInfo.LotStartTime.SubString(18, 2));
        }
        sList->Add(Str);
        Str.sprintf("SUMMARY_END_TIME:%s", st.sLotEndTime);
        sList->Add(Str);
    }

    for(int iBin=0; iBin<=iTestBinCount; iBin++)
    {
        iCount=0;
        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                iUnloadCount+=W5SCKARTREM_LOTSUMMARY_COUNTCATEGORY(iCount,iBin);   // golden :1803 LotSummary.iCountCategory[iCount][iBin]
                iCount++;
            }
        }
    }

    for(int i=0; i<eTrayCount; i++)
    {
        aUnloader[i]=s6ShortTrayName[i]+AnsiString("(");
        bHaveBinData=false;
        if(Prod.iTrayType[i]!=tNotUse)
        {
            for(int j=0; j<=iTestBinCount; j++)
            {
                temp=Prod.iT6PosCate[j];
                if(temp<=0 && j!=iTestBinCount)
                    continue;
                if(i==temp-1)
                {
                    if(bHaveBinData==false)
                        aUnloader[i]+=AnsiString(j);
                    else
                        aUnloader[i]+=AnsiString(",")+AnsiString(j);

                    bHaveBinData=true;
                    iUnloadCnt[i]+=W5SCKARTREM_LOTSUMMARY_TOTALCATEGORY(j);        // golden :1828 LotSummary.iTotalCategory[j]
                }

                if(Prod.iIfErrorT6==i && j==iTestBinCount)
                {
                    iUnloadCnt[i]+=W5SCKARTREM_LOTSUMMARY_TOTALCATEGORY(j);        // golden :1833 LotSummary.iTotalCategory[j]
                }
            }

            if(Prod.iIfErrorT6==i)
            {
                aUnloader[i]+="error";
                bHaveBinData=true;
            }
            else
            {
                if(bHaveBinData==false)
                    aUnloader[i]+="Null";
            }
            aUnloader[i]+=")";
        }
    }

    Str.sprintf("Handler Load Qty:%d", LastSet.iSCKARTInputCT);                 //JerryYang 20200319 fix summary
    sList->Add(Str);
    Str.sprintf("Handler Unload Qty:%d", iUnloadCount);
    sList->Add(Str);
    sList->Add("");
    sList->Add(" ========================================================================");
    sList->Add(" =========================  Group Bin Summary  ===========================");
    sList->Add(" ========================================================================");

    for(int i=0; i<eTrayCount; i++)
    {
        if(Prod.iTrayType[i]!=tNotUse)
        {
            Str.sprintf("%s:%d", aUnloader[i], iUnloadCnt[i]);
            sList->Add(Str);
        }
    }

    sList->Add("");
    sList->Add(" ========================================================================");
    sList->Add(" =========================  Hard Bin Summary  ===========================");
    sList->Add(" ========================================================================");

    iCount=0;
    Str=" Hard P/F ";
    Str2=" ---- ----";
    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            iCount++;
            Str1.sprintf(" Site%d", iCount);
            Str=Str+Str1;
            Str2=Str2+" -----";
        }
    }
    Str=Str+" Total  Yield";
    sList->Add(Str);
    Str2=Str2+" ----- --------";
    sList->Add(Str2);

    for(int iBin=0; iBin<iTestBinCount; iBin++)
    {
        temp=Prod.iT6CatData[iBin];
        if(IniConfig.bSPILFunction==true && (temp<0 || temp>=eTrayCount))
            continue;

            Str.sprintf("%d", iBin);
        while(Str.Length()<5)
        {
            Str=" "+Str;
        }

        if(Prod.bIsPassBin[iBin])
            Str=Str+" PASS";
        else
            Str=Str+" FAIL";

        iCount=0;
        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                Str2.sprintf("%d", W5SCKARTREM_LOTSUMMARY_COUNTCATEGORY(iCount,iBin));   // golden :1914 LotSummary.iCountCategory[iCount][iBin]
                while(Str2.Length()<6)
                {
                    Str2=" "+Str2;
                }
                Str=Str+Str2;
                iCount++;
            }
        }

        if(IniConfig.bSPILFunction==true && W5SCKARTREM_LOTSUMMARY_TOTALCATEGORY(iBin)<=0)   // golden :1924 LotSummary.iTotalCategory[iBin]
            continue;

        Str2.sprintf("%d", W5SCKARTREM_LOTSUMMARY_TOTALCATEGORY(iBin));                      // golden :1927 LotSummary.iTotalCategory[iBin]
        while(Str2.Length()<6)
        {
            Str2=" "+Str2;
        }
        Str=Str+Str2;

        if(IniConfig.bSPILFunction==true)                                       //JerryYang 20220923 : unload數量為0不要存log
        {
            Str2=ChangeToPercentage(W5SCKARTREM_LOTSUMMARY_TOTALCATEGORY(iBin), iUnloadCount);   // golden :1936 LotSummary.iTotalCategory[iBin]
        }
        else
        {
            Str2=ChangeToPercentage(W5SCKARTREM_LOTSUMMARY_TOTALCATEGORY(iBin), st.iLotCount);   // golden :1940 LotSummary.iTotalCategory[iBin]
        }

        while(Str2.Length()<9)
        {
            Str2=" "+Str2;
        }
        Str=Str+Str2;

        sList->Add(Str);
    }

    sList->Add(" ========================================================================");

    if(FileExists(PathName2+FileName)==false)                                   //Steven 20230317 : 避免連續存兩次把資料蓋掉了
    {
        if(IniConfig.bSPILFunction==true)                                       //JerryYang 20220923 : unload數量為0不要存log
        {
            if(iUnloadCount>0)
                sList->SaveToFile(PathName2+FileName);
        }
        else
        {
            sList->SaveToFile(PathName2+FileName);
        }
        fObserver->memoLotSummary->Lines=sList;                                 // golden :1965 fObserver->memoLotSummary->Lines=sList -- whole-list COPY, see atester_shims.h TfObserverMemoLotSummaryLines
    }
    sList->Clear();
    delete sList;

    if(IniConfig.bN17UploadLotSummary)                                          //JerryYang 20220923 : Upload lot summary
    {
        if(DirectoryExists(IniConfig.asN17LotSummaryPath))
        {
            if(IniConfig.bA38_SLT_Summary &&
               st.sLotID!="NA" &&
               st.sLotID!="" &&
               iUnloadCount>0)
            {
                strFileName=PathName2+FileName;                                 //Steven 20230215 : 存檔路徑加上年月
                if(FileExists(strFileName))
                {
                    strPath.sprintf("%s\\%s", IniConfig.asN17LotSummaryPath, FileName);
                    bResult=CopyFile(strFileName.c_str(), strPath.c_str(), true);
                    if(bResult==false)
                    {
                        ShowMyMessage("Uploaded lot summary error\r\nPlease check the path of N-17.","上傳lot summary失敗\r\n請檢查N-17路徑是否存在");
                    }
                    else
                    {
                        RecordProcess("Uploaded lot summary successfully.");    //Steven 20190722 : add TSV log
                    }
                }
            }
            else
            {
                if(st.sInfo_CustLotID=="NA" || st.sInfo_CurrQty=="NA" ||
                   st.sInfo_CustLotID=="" || st.sInfo_CurrQty=="")
                {
                }
                else
                {
                    strFileName=PathName2+FileName;                             //Steven 20230215 : 存檔路徑加上年月
                    if(FileExists(strFileName))
                    {
                        strPath.sprintf("%s\\%s", IniConfig.asN17LotSummaryPath, FileName);
                        bResult=CopyFile(strFileName.c_str(), strPath.c_str(), true);
                        if(bResult==false)
                        {
                            ShowMyMessage("Uploaded lot summary error\r\nPlease check the path of N-17.","上傳lot summary失敗\r\n請檢查N-17路徑是否存在");
                        }
                        else
                        {
                            RecordProcess("Uploaded lot summary successfully.");//Steven 20190722 : add TSV log
                        }
                    }
                }
            }
        }
        else
        {
            ShowMyMessage("Uploaded lot summary error\r\nPlease check the path of N-17.", "上傳lot summary失敗\r\n請檢查N-17路徑是否存在");
        }
    }

    if(IniConfig.bA38_SLT_Summary && FileName!="" && iUnloadCount>0)
    {
        // AI(W906-SaveTestSummarySECS) 20260721: GOLDEN BUG, preserved verbatim (see this function's
        // own header doc comment "Golden bug preserved VERBATIM"): strFileName is only ASSIGNED inside
        // the bN17UploadLotSummary block above (golden :1979/:2002) -- when N17 upload is OFF,
        // strFileName is still its AnsiString default-init "" here, so this fires with an EMPTY path
        // every time this branch's condition is true. Real WinAPI ShellExecute with an empty lpFile
        // synchronously fails (SE_ERR_FNF-class), no process spawned, no crash -- a real, latent
        // no-op-open, NOT "fixed" to skip when empty. See gate #11.
        W5SCKARTREM_SHELLEXECUTE_OPEN(strFileName);                             // golden :2027 ShellExecute(NULL,NULL,strFileName.c_str(),NULL,NULL,SW_SHOW)
    }

    if(CosFunction.bUseTSVFunction &&                                           //Steven 20240904 : for ATK的TSV功能
       IniConfig.bN09_LotCountAutoFunc)
    {
        bWaitTSV=true;
        st.bShowTSVMsg=false;
        st.sTSVMsg="";
        RecordProcess("Lot end and send summary and waiting TSV reply.");       //Steven 20190722 : add TSV log
        st.TSVdelay.SetSecAndOn(IniConfig.dN09_SearchTime);
        W5SCKARTREM_TIMERTSV_ENABLE();                                          // golden :2038 TimerTSV->Enabled=true
    }

    if(iSaveData==1)
        W5SCKARTREM_LOTSUMMARY_CLEARALLDATA();                                  // golden :2043 LotSummary.ClearAllData()
}

// =============================================================================
//  10. DoARTLotStart -- golden TfSCKART::DoARTLotStart(AnsiString,AnsiString,int) (SCK_ART.cpp:4191-4256)
//     AI(W906-DoARTLotStart) 20260721. See SCK_ART_Remainder.h's own doc comment on this declaration
//     for the full dependency-verification writeup (HasICUnderMachine / bReadLotInfoFromART /
//     bQAModeFlag / SetRunStartMode / RecordProcess / fLotInfo->*/ fMain->* gates #9-#10 / the 2 new
//     LastSet fields that turned out to be already-real / the 2 new struct fields).
// =============================================================================
void SckArtRem_DoARTLotStart(SckArtRemainderState &st, AnsiString _sLotID, AnsiString _sProcess, int _iLotCount)
{
    bReadLotInfoFromART=true;                                                   // golden :4193 (real global, cmydef.cpp -- NOT a TfSCKART member; verified against golden SCK_ART.h)
    bool bChangeLotID=false;
    if(HasICUnderMachine()==false)                                              // golden :4195 (real, csystem.h/csystem_predicates.cpp)
    {
        bChangeLotID=true;
    }
    else
    {
        // golden :4201 `if(sLotID==_sLotID && (sLotID!="" && _sLotID!=" "))` -- ASYMMETRIC ON PURPOSE,
        // preserved VERBATIM (not "fixed"): the left half compares st.sLotID against "" (empty string);
        // the right half compares _sLotID against " " (one literal SPACE character), not "". This reads
        // like a BCB6-era typo (both halves plausibly meant "!=\"\""),  but it is golden's real, shipped
        // behavior. Net effect: when a caller passes _sLotID=="" (the common case -- e.g. HANA_ART.cpp's
        // fSCKART->DoARTLotStart(sLotNo,...) call site), "" != " " is TRUE, so the right-hand AND-term is
        // just "st.sLotID!=\"\"" in practice and the whole condition collapses to golden's evident intent
        // (st.sLotID==_sLotID && st.sLotID!=""); only a caller passing the exact one-character string " "
        // as _sLotID would hit the asymmetry and force bChangeLotID=true even when st.sLotID==_sLotID.
        // DO NOT normalize " " to "" here -- see the test file's own case exercising this exact " " input.
        if(st.sLotID==_sLotID && (st.sLotID!="" && _sLotID!=" "))
        {
            bChangeLotID=false;
        }
        else
        {
            bChangeLotID=true;
        }
    }

    if(bChangeLotID==true)
    {
        st.iCurrent93KARTStep=1;                                                // golden :4213
        st.iCurrentFlexARTStep=4;                                               // golden :4214

        // golden :4215 ClearLotInfo() -- per SckArtRem_ClearLotInfo's own out-param contract, its
        // internal `fSCKART->AccessFile(false)` (golden :921, iAccess defaults to -1, ALL groups) is
        // exposed via bNeedAccessFileWrite rather than called recursively inside that function. Invoked
        // here immediately, matching golden's real execution order (ClearLotInfo fully returns --
        // INCLUDING that tail AccessFile(false) call -- before this function's next statement runs).
        bool bNeedAccessFileWrite=false;
        SckArtRem_ClearLotInfo(st, &bNeedAccessFileWrite);
        if(bNeedAccessFileWrite)
            SckArtRem_AccessFile(st, false);                                    // golden :921 (inside ClearLotInfo) fSCKART->AccessFile(false)

        // golden :4216 SetLotStatus(iLOTSTATUS_W) -- inlined (iLOTSTATUS_W==1, golden ctor
        // SCK_ART.cpp:44 -- itself a TfSCKART member golden sets once in its ctor and never reassigns,
        // i.e. a de-facto constant; same "avoid a 2nd SetLotStatus implementation" precedent
        // SckArtRem_ClearLotInfo's own comment already established for iLOTSTATUS_NONE==0). NOTE: because
        // this runs AFTER the AccessFile(false) call just above (which persists the PRE-overwrite
        // "NONE"/0 values), and the LATER AccessFile(false,1) call below only persists the iAccess==1
        // group (sLotID/iLotCount/...), NOT the iAccess==-1||0||10 group SetLotStatus's fields belong to
        // -- the "LOTSTATUS_W"/1 values set here are NEVER actually written to the ini file by this
        // function. Faithfully reproduced golden quirk, not a translation bug.
        st.sLOTSTATUS="LOTSTATUS_W";
        st.iCurrentStatus=1;

        st.sLotID=_sLotID;                                                      // golden :4217
        st.sProcessCode=_sProcess;                                              //Steven 20190521 : ATK lot count -- golden :4218
        if(TestIF_File.bRENESAS_EnableFTCT==false)                              //RogerYang 20250930 : RogerYang 瑞薩FT-CT 指令"20"已更新
            st.sLotStartTime=FormatDateTime("yyyymmdd_hhnnss", Now());          // golden :4220 Now().FormatString(...) -> vclcompat FormatDateTime (ainarm9045.cpp:890 precedent)

        fLotInfo->cbProcess->Text=_sProcess;                                    // golden :4221 (real FormsFacade member)
        fLotInfo->SetLotID(_sLotID, false);                                     // golden :4222 (real FormsFacade member, offline no-op)
        fLotInfo->SetLotStart(__FUNC__);                                        // golden :4223 (real FormsFacade member, offline no-op; __FUNC__ via canary_support.h shim)

        if(bQAModeFlag==true)                                                   //Steven 20170830 (wei) : QA mode for ATK ART -- golden :4225 (real global, cmydef.cpp)
        {
            st.iLotCount  =TestIF_File.iQAModeCount;                           // golden :4227
            st.iInputCount=TestIF_File.iQAModeCount;                           // golden :4228
            SckArtRem_AccessFile(st, false, 1);                                 //必須在轉換模式前先存檔一次, 不然轉模式的裡面會讀取到舊的資料 -- golden :4229
            SetRunStartMode(rsmInitial_ART);                                    // golden :4230 (real, aHotPlateSubstrate.h/.cpp)
            bQAModeFlag=false;                                                  // golden :4231
        }
        else
        {
            st.iLotCount  =_iLotCount;                                         //Steven 20250410 : fixed for ART -- golden :4235
            st.iInputCount=_iLotCount;                                         // golden :4236
            SckArtRem_AccessFile(st, false, 1);                                 // golden :4237
        }

        RecordProcess(AnsiString().sprintf("Lot start: %s, %d", st.sLotID, st.iLotCount));    //Steven 20190722 : add TSV log -- golden :4240
    }

    if(st.iTesterType==1)
    {
        st.iNeedRT=0;                                                           // golden :4245
        LastSet.bEndLotAutoRetestGPIB=false;                                    // golden :4246 -- REAL LastSet field (canary_support.h:145; added ahead of need by a prior integrate pass -- verified, NOT a new gate, see header's [UPDATE] note on gate #4)
        LastSet.bWaitStartLotAutoRetestGPIB=false;                              // golden :4247 -- REAL LastSet field (canary_support.h:144)
        LastSet.bFirstTestAutoRetestGPIB=true;                                  // golden :4248 -- REAL LastSet field (canary_support.h:146)
        W5SCKARTREM_FMAIN_SETLOTSTATE(2);                                       // golden :4249 fMain->SetLotState(2) -- ART FT Start for SCK -- gate #9, TU-local no-op
        if(TestIF_File.bAlarmAfterSendSRQKIND2)                                 //Steven 20230202 : ATK 要在SRQKIND2之後Alarm
        {
            W5SCKARTREM_FMAIN_TESDERROR_ADD("MES07399");                       // golden :4252 fMain->tESDError->Add("MES07399") -- gate #10, TU-local no-op
        }
    }
}
