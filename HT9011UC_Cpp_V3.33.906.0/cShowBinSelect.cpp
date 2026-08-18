// =============================================================================
//  cShowBinSelect.cpp  --  FW-3 queue item 2: TfShowBinSelect, WAVE A subset
//
//  Translation wave: FW-3 ShowBinSelect Wave A
//  Translator: AI(W906-FW3-ShowBinSelect-WA) 20260818
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/cShowBinSelect.cpp
//  (3,016 lines) + cShowBinSelect.h (524 lines), cp950/Big5. Decoded this
//  wave with `python3 -c "open(path,'rb').read().decode('cp950')"` -- 0
//  U+FFFD over both files (measured before any line below was written).
//
//  See forms/fShowBinSelect.h for the full WAVE A / WAVE B split, GATE
//  REGISTER, DEVIATION list (bootstrap ctor) and facade shape -- not
//  duplicated here to avoid the two files drifting apart.
//
//  ABSENCE-CLAIM TIMESTAMPS (commands + when run, this wave, before writing
//  the citing code below -- re-run at hand-off per project policy)
//  --------------------------------------------------------------------------
//    fCounterClear facade : `grep -rn "fCounterClear" --include=*.h .` --
//                            1 hit, Automation/SCK_ART_Remainder.h:71, itself
//                            a comment recording the same gap (20260818).
//    fSecurity facade     : `grep -rn "class TfSecurity" --include=*.h .` --
//                            0 hits anywhere (20260818).
//    fBinSel facade       : `grep -rn "fBinSel" --include=*.h .` -- only
//                            comment-only hits in forms/fMain.h (20260818).
//    fCleaning->btnResetCleanCountClick : `grep -n
//                            "btnResetCleanCountClick" forms/fCleaning.h` --
//                            0 hits (20260818).
//    fFixAICCD->UnloadAICntNG : `grep -n "UnloadAICntNG" forms/fFixAICCD.h`
//                            -- 0 hits (20260818).
//    fMain->StatusBar1    : `grep -n "StatusBar1" forms/fMain.h` -- 0 hits
//                            (20260818).
//    Application->MessageBoxA / MB_OKCANCEL / IDOK : no TApplication surface
//                            anywhere in vclcompat (same absence as
//                            forms/fContactCT.h's GATE (C2); 20260818).
//    ShowMyMessageBox_YES_NO : Automation/AGV_E84.h:58's own comment already
//                            records it "untranslated" (20260818).
//    HSys.BinDisCtrl (opaque): `grep -n BinDisCtrl database.h` -- declared
//                            `TMyBinDispCtrl *BinDisCtrl; // opaque; NULL
//                            until UI wave wires InstallColorBinDisplay`
//                            (database.h:300, 20260818) -- this is WHY
//                            ChangeBinDispStatus/DoShowBinDigital are Wave B,
//                            not gated-in-place: >90% of each body is this
//                            one pointer.
//
//  WAVE B (SBWB2) UPDATE, 20260819 -- three of the above absence claims are now
//  STALE (landed by later waves than the 20260818 date on each original claim,
//  same failure mode as the tree-wide "wave-agents-stale-absence-claims" gotcha
//  -- re-checked fresh this wave, not trusted from the old banner text above):
//    fBinSel      : `grep -n "class TfBinSel" forms/fBinSel.h` -- 1 hit, REAL
//                   (forms/fBinSel.h:396). ShowBinSel() below uses
//                   `fBinSel->chkShow0Xbin->Checked` for real, not gated.
//    fAGV         : `grep -n "IsATK_AMR" forms/fAGV.h` -- REAL (delegates to
//                   Automation/AGV_predicates.cpp, W5-Automation-Integrate).
//                   ShowBinSel() below calls `fAGV->IsATK_AMR()` for real.
//    fCounterClear: `grep -n "class TfCounterClear" forms/fCounterClear.h` --
//                   REAL (landed FW-SecCC, 20260819) -- NOT exercised by
//                   ShowBinSel (this wave's target never calls ClearCount),
//                   so GATE (B1) in ShowCategoryBin above is untouched by this
//                   wave -- flagged here so it is not mistaken for still-
//                   absent by a future reader of this file.
//  NEW absence this wave: fSortCT->myCountPanel[i].pnlYield/.pnlCount -- `grep
//  -n "myCountPanel" forms/fSortCT.h` -- 0 hits, 20260819 (fSortCT itself is
//  real; this ONE member array is not on its facade). See GATE (B9).
// =============================================================================
#include "forms/fShowBinSelect.h"
#include "forms/fSecurity.h"   // AI(W906-FW-SecUnlock) 20260819: fSecurity->Insufficient (B6 permission half dissolved)
#include "forms/fBinSel.h"    // AI(W906-FW-SBWB2) 20260819: fBinSel->chkShow0Xbin (ShowBinSel, real -- see banner UPDATE below)
#include "SECSGEM/SecsEventType.h"     // AI(W906-FW-SBWB2) 20260819: SECS_EVENT.OutputPort1BinCode (ShowBinSel)
#include "SECSGEM/SecsEventReport.h"   // AI(W906-FW-SBWB2) 20260819: EventReport() (ShowBinSel)

#include "MachineType.h"           // enums, CC_* customer codes, ChangeToPercentage<T>/ChangeToFloat<T>
#include "cmydef.h"                  // SystemStart/iHome/InitialOK/iTestBinCount/iByBinCnt[]/
                                       // iSV_ErrBinCnt/iSVByBinCount[]/AccessLevel/iDefHonPrecLevel/
                                       // StipulateInputCount/iNetUPH/iGrossUPH/iPauseTime/bRefreshFunction
#include "cprod.h"                    // Prod/TestIF/TestIF_File/RunInfo/BinSelect/TastCategory
#include "LastSet.h"                   // LastSet
#include "Config.h"                     // IniConfig
#include "CosFunction.h"                 // CosFunction
#include "aHotPlateSubstrate.h"           // TestSocket (TMyKitSuck header choice -- see KNOWLEDGE.md)
#include "cSocket.h"                       // TArm/TMySocket, ArmData[3]/ArmHistory[3]
#include "cinitial.h"                       // IsNNMode()/NN_1Row/NN_2Row
#include "csystem.h"                          // ShuttleHasIC()/IndexHasIC()
#include "common.h"                          // MyTickCount()
#include "cMyDB.h"                            // MyDBIProcess
#include "atester_shims.h"                     // fContact (TfContactShim)
#include "atester_ProcessCount.h"                // DoLowYieldAlarm
#include "AutoClean/AutoClean.h"                  // InitialAutoCleanAllTask
#include "FormsFacade.h"                           // fMain
#include "forms/fYieldMonitoring.h"                 // fYieldMonitoring

#include <cstdlib>   // atoi
#include <algorithm> // (kept for parity; no direct use this wave)

// AI(W906-FW-SBWB2) 20260819: golden Graphics.hpp clGray -- this tree has no
// shared Graphics-compat header (confirmed this wave: `grep -rn "const TColor
// clGray" --include=*.h .` finds it defined LOCALLY, once per consuming TU, in
// ATC/ATCInterface.h, EJ1N/MyOmronPanel.h and VacuumUnit/MyVacuumPanel.h, each
// with the identical value below and each with its own "this file adds its own
// const TColor clGray locally" note) -- ShowBinSel is this TU's first user of
// the name, so it gets its own copy, same established idiom.
const TColor clGray = TColor(0x00808080);   // golden Graphics.hpp clGray

//---------------------------------------------------------------------------
// AI(W906-FW-YEnable) 20260818: homecoming -- the live global is now backed
// by a real instance, BUT this ctor is NOT the same "pure field bootstrap"
// shape as cContactCT.cpp's TfContactCT (re-verified this wave, per task
// brief, by reading the full chain): the ctor calls ShowInitialString(),
// whose own tail statement unconditionally calls ShowCategoryBin(), which
// dereferences `fContact->fShow` (golden :1729) and, on the common
// `LastSet.iTester != _2D_SORT` path, `ArmData[0]/ArmData[1]->GetSelBin()`
// (golden :1802 onward, unconditional inside that path -- not gated behind
// any flag). `fContact` (atester_shims.cpp) and `ArmData[0..2]`
// (cSocket.cpp's own file-scope `ArmDataBootstrap`, see that file's banner)
// are BOTH raw pointers populated by a DIFFERENT translation unit's dynamic
// initializer -- C++ gives no cross-TU dynamic-initialization-order
// guarantee (the exact invariant cSocket.cpp's ArmDataBootstrap banner
// documents: "read from ordinary runtime functions and NEVER from another
// TU's static initialiser"). Unlike golden, where the VCL form is
// constructed inside WinMain strictly after every global already exists,
// this port's `fShowBinSelect` is a plain static-storage global whose own
// dynamic initializer could in principle run before either of those two --
// see the SIOF guard inside ShowInitialString() (this file) added
// specifically for this homecoming, which makes this constructor safe
// regardless of link/init order.
TfShowBinSelect *fShowBinSelect = new TfShowBinSelect();

//---------------------------------------------------------------------------
//  DelDot (free function) -- golden :178-205
//---------------------------------------------------------------------------
AnsiString DelDot(AnsiString asBuffer)
{
    AnsiString asReturn;
    int iBegin = 0;
    for (int i = 0; i < iTestBinCount; i++)   // kevin 20140317: 256 bin
    {
        if (i == 1)
        {
            iBegin = asBuffer.Pos(AnsiString(i) + " ");
            if (iBegin != 1)
                continue;
        }
        else
            iBegin = asBuffer.Pos(" " + AnsiString(i) + " ");
        if (iBegin > 0)
        {
            break;
        }
    }

    if (iBegin == 0)
    {
        iBegin = asBuffer.Pos(AnsiString("E"));
    }

    asReturn = asBuffer.Delete(1, iBegin - 1);
    return asReturn;
}

//---------------------------------------------------------------------------
//  TfShowBinSelect::TfShowBinSelect -- DEVIATION bootstrap ctor.
//  See forms/fShowBinSelect.h CTOR NOTE for why this is NOT a verbatim
//  translation of golden's ctor body (golden :44-161).
//---------------------------------------------------------------------------
TfShowBinSelect::TfShowBinSelect()
{
    for (int i = 0; i < e3TrayCount; i++)
    {
        MyBinSel[i]         = new TfShowBinSelectLabel();   // AI(W906-FW-SBWB2) 20260819: Font->Color (ShowBinSel)
        MyBinSelLab[i]      = new TLabel();
        MyBinSelARTFT[i]    = new TLabel();
        MyBinSelARTFTLab[i] = new TLabel();
        MyBinSelARTRT[i]    = new TLabel();
        MyBinSelARTRTLab[i] = new TLabel();
        grpBinDisp[i]       = new TGroupBox();
        // AI(W906-FW-SBWB2) 20260819: UnLoadPanel[]/UnLoadLabel[] --
        // see forms/fShowBinSelect.h member-declaration comment for why
        // these are member arrays, not ~33 individually-named pointers.
        UnLoadPanel[i] = new TPanel();
        UnLoadLabel[i] = new TfShowBinSelectLabel();   // AI(W906-FW-SBWB2) 20260819: Font->Color (ShowBinSel)
    }
    for (int i = 0; i < eTrayCount; i++)
    {
        EditAi[i] = new TLabeledEdit();
        // GATE (cosmetic): tempAi[i]->Align=alTop; -- golden :132. No Align
        // field exists on vclcompat::TLabeledEdit and nothing reads it back.
    }

    ColorRed = 1;      // 1:red, 2:green, 3:orange
    ColorGreen = 2;
    ColorOrange = 3;

    bShow = false;
    tsUPH = new TStringList();

    for (int i = 0; i < 20; i++)   // Steven 20140621: For SECS GEM
    {
        tsUPH->Add("");
    }

    // AI(W906-FW-YEnable) 20260818: SIOF guard #2 (measured, not speculative) --
    // this ctor now runs as a static initializer in EVERY god-stack exe (the
    // dissolved (Y2) gates in uYieldMonitoring.cpp make the linker pull this
    // TU everywhere), and the first FW-YEnable gate went 64/139 red on it in
    // two link-order-dependent shapes (gdb backtraces, build_fwyeng):
    //   (a) SEGV -- sBinCode_ATK[] lives in cmydef.cpp; assigning another
    //       TU's not-yet-constructed AnsiString reaches std::string::
    //       _M_replace through a null _M_p (test_cUnitConvert).
    //   (b) out_of_range -- when the bootstrap TUs happened to init first,
    //       the tail guard inside ShowInitialString() PASSED and
    //       ShowCategoryBin() indexed default-sized TStringGrid Cells with
    //       config-dependent dimensions still zero (test_contactct_core).
    // golden runs this ctor inside WinMain, strictly after every global AND
    // after config load; INIFileGeneral != 0 reproduces exactly that
    // precondition (nullptr constant-init, set only by LoadMachineConfig,
    // common.cpp:1459 -- the same sentinel cObserver.cpp's ctor already
    // uses). Skipping the priming at static init is self-healing: every
    // dissolved (Y2) call site plus uHGemHT9045.cpp's SECS handlers re-run
    // ShowCategoryBin() from the runtime loop, strictly after static init.
    if (INIFileGeneral != 0)
    {
        for (int i = 0; i < eTrayCount; i++)
        {
            sBinCode_ATK[i] = "";
        }

        ShowInitialString();
    }

    if (SPIL_FOR_QLE == 1)   // JerryYang 20251020: partial device-ejection feature
    {
        palAutoDeviceEjection->Visible = true;
        palAutoDeviceEjection->Top = 230;
    }
    else
    {
        palAutoDeviceEjection->Visible = false;
    }

    // GATE (cosmetic): btReturn->Align=alBottom; -- golden :160. No Align
    // field exists on vclcompat::TButton and nothing reads it back.

    bUpdateBinDigital = false;
    iShowBinDigitalTask = 0;
    iLowYieldBinSelectContactCount = 0;
    Left = 0; Top = 0; Width = 0; Height = 0;
}

//---------------------------------------------------------------------------
//  FormDestroy -- golden :163-176
//---------------------------------------------------------------------------
void TfShowBinSelect::FormDestroy(TObject * /*Sender*/)
{
    try
    {
        TimerAutoCleanCount->Enabled = false;   // Steven 20160727
        tsUPH->Clear();                          // Ifor 20170603 (wei): clear TStringList before deleting
        delete tsUPH;                            // Steven 20160108: release memory
    }
    catch (...)
    {
        // NOTE: explicit 3-arg form disambiguates two overlapping
        // MyDBIProcess declarations both visible in this TU (same
        // pre-existing tree-wide overload shape as cContactCT.cpp's
        // identical FormDestroy fix -- see that file's note).
        MyDBIProcess("Exception", "TfShowBinSelect::FormDestroy", "");
    }
    LogSoftwareOffTime("TfShowBinSelect, FormDestroy");   // Steven 20210526: record software run time
}

//---------------------------------------------------------------------------
//  FormClose -- golden :868-873
//---------------------------------------------------------------------------
void TfShowBinSelect::FormClose(TObject * /*Sender*/)
{
    bShow = false;
}

//---------------------------------------------------------------------------
//  InitShowBinDigital -- golden :874-878
//---------------------------------------------------------------------------
void TfShowBinSelect::InitShowBinDigital()
{
    iShowBinDigitalTask = 1;
    bUpdateBinDigital = true;
}

//---------------------------------------------------------------------------
//  SetLabelVisible -- golden :1425-1434
//---------------------------------------------------------------------------
void TfShowBinSelect::SetLabelVisible(int iTag, bool bVisible)
{
    MyBinSel[iTag]->Visible         = bVisible;
    MyBinSelLab[iTag]->Visible      = bVisible;
    MyBinSelARTFT[iTag]->Visible    = bVisible;
    MyBinSelARTFTLab[iTag]->Visible = bVisible;
    MyBinSelARTRT[iTag]->Visible    = bVisible;
    MyBinSelARTRTLab[iTag]->Visible = bVisible;
    grpBinDisp[iTag]->Visible       = bVisible;
}

//---------------------------------------------------------------------------
//  SetAutoVisible -- golden :1436-1477
//---------------------------------------------------------------------------
void TfShowBinSelect::SetAutoVisible()
{
    gbBinBox->Visible   = (iHWFix_BinBox == 1);
    pnlMag123->Visible  = (AUTO3_IS_MAGAZINE > 0);
    pnlFix789->Visible  = (AUTO_EMPTY_COLOR >= 3);
    pnlAuto456->Visible = (AUTO_EMPTY_COLOR >= 3);
    gbAuto6->Visible    = (AUTO_EMPTY_COLOR >= 4);

    for (int i = eAuto1; i < eTrayCount; i++)
    {
        if (Prod.iTrayType[i] == tNotUse)
        {
            SetLabelVisible(i, false);
        }
        else
        {
            SetLabelVisible(i, true);
        }
    }

    if (AUTO3_IS_MAGAZINE > 0)
        SetLabelVisible(iMagAtAuto, false);

    if (AUTO_EMPTY_COLOR >= 3)   // Steven 20230907: For HT-9011UC
    {
        if (CosFunction.bUseTrayUpDownSet)   // wei 20160224: TSMC fix up/down
        {
            pnlFix789->Visible = true;
        }
        else
        {
            pnlFix789->Visible = false;
        }
    }

    if (iHWFix_BinBox == 1)   // kevin 20160819: error bin box
    {
        MyBinSel[eBulkBox]->Caption      = "E";
        MyBinSelARTFT[eBulkBox]->Caption = "E";
        MyBinSelARTRT[eBulkBox]->Caption = "E";
    }
}

//---------------------------------------------------------------------------
//  ShowInitialString -- golden :1634-1700
//---------------------------------------------------------------------------
void TfShowBinSelect::ShowInitialString()
{
    if (CosFunction.bSortingBy2DList == true &&
        LastSet.iTester == _2D_SORT &&
        TestIF_File.bSortingBy2DIDList == true)   // JerryYang 20230322: 2DID expect count
    {
        StrGrdCategory->ColCount = 4;
        StrGrdCategory->Cells[1][0] = "expect count";
        StrGrdCategory->Cells[2][0] = "Count";
        StrGrdCategory->Cells[3][0] = "Percent";
    }
    else
    {
        StrGrdCategory->Cells[1][0] = "Count";
        StrGrdCategory->Cells[2][0] = "Percent";
    }
    StrGrdCategory->RowCount = iTestBinCount + 2;

    if (LastSet.iTester == _2D_SORT)   // Frank 20221122: 2DID sorting for ATK
    {
        for (int i = 0; i < iTestBinCount; i++)   // JerryYang 20220425: Nvidia Ryan requested "Bin" wording
            StrGrdCategory->Cells[0][1 + i] = "BIN " + AnsiString(i);
    }
    else
    {
        for (int i = 0; i < iTestBinCount; i++)
            StrGrdCategory->Cells[0][1 + i] = "Category " + AnsiString(i);
    }

    StrGrdCategory->Cells[0][iTestBinCount + 1] = "Error Bin";

    if (CosFunction.bCategoryInfoByContactCT)   // Sam 20240131: new CategoryInfo record set using ContactCT
    {
        StrGrdCategoryContCT->Cells[1][0] = "Count";
        StrGrdCategoryContCT->Cells[2][0] = "Percent";
        StrGrdCategoryContCT->RowCount = iTestBinCount + 2;
        for (int i = 0; i < iTestBinCount; i++)
            StrGrdCategoryContCT->Cells[0][1 + i] = "Category " + AnsiString(i);
        StrGrdCategoryContCT->Cells[0][iTestBinCount + 1] = "Error Bin";
    }

    if (USE_AUTO_RETEST == eartInstall || CosFunction.bUseARTSortCount)   // wei 20150923: add ART count
                                                                             // Ifor 20170316 (wei): add MRT Mode
    {
        StrGrdCategoryART->Cells[1][0] = "Count";
        StrGrdCategoryART->Cells[2][0] = "Percent";
        StrGrdCategoryART->RowCount = iTestBinCount + 2;
        for (int i = 0; i < iTestBinCount; i++)
            StrGrdCategoryART->Cells[0][1 + i] = "Category " + AnsiString(i);

        StrGrdCategoryART->Cells[0][iTestBinCount + 1] = "Error Bin";

        StrARTSkipICCount->Cells[1][0] = "Count";   // Frank 20160819
        StrARTSkipICCount->RowCount = 22;
        StrARTSkipICCount->ColCount = 2;

        for (int i = 0; i < 20; i++)
        {
            if (i == 0)
                StrARTSkipICCount->Cells[0][1 + i] = "FT " + AnsiString(1);
            else
                StrARTSkipICCount->Cells[0][1 + i] = "RT " + AnsiString(i);
        }
        StrARTSkipICCount->Cells[0][21] = "Total";
    }

    // AI(W906-FW-YEnable) 20260818: SIOF guard, NOT a golden deviation --
    // PORT-ONLY safety net for the new static-init `fShowBinSelect` global
    // (see this file's homecoming note on that definition for the full
    // reasoning). golden's ctor chain reaches this exact point strictly
    // after WinMain has already brought up every other global, so golden
    // never needed this check. `fContact` and `ArmData[0]/ArmData[1]` are
    // each a raw pointer owned by a DIFFERENT translation unit's own dynamic
    // initializer (atester_shims.cpp / cSocket.cpp respectively); cross-TU
    // dynamic-init order is unspecified, so if this ctor's call happens to
    // run first, `ShowCategoryBin()` would dereference a still-null pointer
    // (`fContact->fShow` at golden :1729 unconditionally, `ArmData[0]/
    // ArmData[1]->GetSelBin()` at golden :1802 on the common
    // `LastSet.iTester != _2D_SORT` path -- verified this wave by reading
    // ShowCategoryBin()'s full body). Skipping this ONE priming call when
    // either isn't bootstrapped yet is self-healing, not a behaviour change:
    // every dissolved fYieldMonitoring (Y2) gate call site
    // (uYieldMonitoring.cpp, this wave) plus uHGemHT9045.cpp's SECS handlers
    // call ShowCategoryBin() again on the next real tick, all from main()'s
    // own runtime loop -- strictly after every TU's static initialization
    // has finished. Every statement ABOVE this guard (the grid header/
    // RowCount setup) touches no cross-TU pointer and is unaffected either
    // way.
    if (fContact != 0 && ArmData[0] != 0 && ArmData[1] != 0)
        ShowCategoryBin();
}

//---------------------------------------------------------------------------
//  ShowCategoryBin -- golden :1701-2052
//  GATE (B1): fCounterClear->ClearCount(ctBinCount) x3, see forms/
//  fShowBinSelect.h GATE REGISTER.
//---------------------------------------------------------------------------
void TfShowBinSelect::ShowCategoryBin()
{
    int i = 0, sum = 0, Sum_ART = 0, iTemp = 0, Sum_ConCT = 0;   // Sam 20240131: ContCT CategoryInfo
    double f = 0.0, fSPBin = 0.0;                                 // Isaac 20171113 (Steven): ATK Special Bin Yield alarm
    AnsiString S1, S2;
    (void)S2;   // golden local, never assigned/read in this function body (verified this wave) -- kept verbatim
    static bool bFirstTime = true;

    if (bFirstTime == true)
    {
        bFirstTime = false;
        for (i = 0; i < TEST_MAX_BIN; i++)
        {
            iLoadPersentCT[i] = 0;
            iLoadCountCT[i] = 0;
            iYeildCT[i] = 0;   // wei 20151111
        }
    }

    if (CUSTOMER_CODE == CC_ASE_CL)
    {
    }
    else
    {
        iSV_ErrBinCnt = LastSet.iBinData32[0][iTestBinCount];
        for (i = 0; i < TEST_MAX_BIN; i++)
            iSVByBinCount[i] = LastSet.iBinData32[0][i];
    }

    if (fContact->fShow == false)
    {
        if (CosFunction.bLowYieldUseContactCounts)   // Sam 20230620: optimise Smart Auto Clean
        {
            for (i = 0; i < iTestBinCount; i++)
                sum += LastSet.iBinData32[0][i];
        }
        else
        {
            for (i = 0; i < eTrayCount; i++)
            {
                sum     += LastSet.BinCT[0][iTo3Unload[i]];
                Sum_ART += LastSet.BinCT_ART[0][iTo3Unload[i]];   // wei 20150923: add ART count
            }
        }

        if (CosFunction.bCategoryInfoByContactCT)   // Sam 20240131: ContCT CategoryInfo
            Sum_ConCT = ArmData[0]->GetTotalCT() + ArmData[1]->GetTotalCT();

        if (LastSet.iTester == _2D_SORT)   // Frank 20221122: 2DID sorting for ATK
        {
            for (i = 0; i < iTestBinCount + 1; i++)   // Steven 20121112: RS232 supports 32Bin
            {
                fShowBinSelect->StrGrdCategory->Cells[0][1 + i] = "";
                fShowBinSelect->StrGrdCategory->Cells[1][1 + i] = "";
                fShowBinSelect->StrGrdCategory->Cells[2][1 + i] = "";
                fShowBinSelect->StrGrdCategory->Cells[3][1 + i] = "";
                if (iByBinCnt[i] > 0 || i == iTestBinCount)
                {
                    if (i == iTestBinCount)
                    {
                        fShowBinSelect->StrGrdCategory->Cells[0][1 + iTemp] = "Error Bin";
                    }
                    else
                    {
                        fShowBinSelect->StrGrdCategory->Cells[0][1 + iTemp] = "BIN " + AnsiString(i);
                    }
                    fShowBinSelect->StrGrdCategory->Cells[2][1 + iTemp] = LastSet.iBinData32[0][i];
                    fShowBinSelect->StrGrdCategoryART->Cells[1][1 + iTemp] = LastSet.iBinData32_ART[0][i];   // wei 20150923: add ART count
                    fShowBinSelect->StrGrdCategory->Cells[1][1 + iTemp] = iByBinCnt[i];
                    if (sum > 0)
                    {
                        f = ChangeToFloat((double)LastSet.iBinData32[0][i], (double)sum);   // Steven 20250820: guard against divide-by-0
                        fShowBinSelect->StrGrdCategory->Cells[3][1 + iTemp] = ChangeToPercentage((double)LastSet.iBinData32[0][i], (double)sum);
                    }
                    else
                    {
                        fShowBinSelect->StrGrdCategory->Cells[3][1 + iTemp] = "0.00%";
                    }

                    if ((USE_AUTO_RETEST == eartInstall && IniConfig.bA10_AutoReTest) ||
                        CosFunction.bUseARTSortCount)   // wei 20150923: add ART count / Ifor 20170316 (wei): add MRT Mode
                    {
                        if (Sum_ART > 0)
                        {
                            fShowBinSelect->StrGrdCategoryART->Cells[2][1 + iTemp] = ChangeToPercentage((double)LastSet.iBinData32_ART[0][i], (double)Sum_ART);
                        }
                        else
                        {
                            fShowBinSelect->StrGrdCategoryART->Cells[2][1 + iTemp] = "0.00%";
                        }
                    }
                    iTemp++;
                }
            }
        }
        else
        {
            for (i = 0; i < iTestBinCount + 1; i++)   // Steven 20121112: RS232 supports 32Bin
            {
                StrGrdCategory->Cells[1][1 + i] = LastSet.iBinData32[0][i];
                StrGrdCategoryART->Cells[1][1 + i] = LastSet.iBinData32_ART[0][i];   // wei 20150923: add ART count
                StrGrdCategoryContCT->Cells[1][1 + i] = (unsigned int)(ArmData[0]->GetSelBin(i) + ArmData[1]->GetSelBin(i));   // Sam 20240131 -- cast disambiguates AnsiString::operator= (unsigned long)

                if (sum > 0)
                {
                    f = ChangeToFloat((double)LastSet.iBinData32[0][i], (double)sum);
                    StrGrdCategory->Cells[2][1 + i] = ChangeToPercentage((double)LastSet.iBinData32[0][i], (double)sum);
                }
                else
                {
                    StrGrdCategory->Cells[2][1 + i] = "0.00%";
                }

                if (CosFunction.bCategoryInfoByContactCT)
                {
                    if (Sum_ConCT > 0)
                    {
                        StrGrdCategoryContCT->Cells[2][1 + i] = ChangeToPercentage((double)(ArmData[0]->GetSelBin(i) + ArmData[1]->GetSelBin(i)), (double)Sum_ConCT);
                    }
                    else
                    {
                        StrGrdCategoryContCT->Cells[2][1 + i] = "0.00%";
                    }
                }

                if ((USE_AUTO_RETEST == eartInstall && IniConfig.bA10_AutoReTest) ||
                    CosFunction.bUseARTSortCount)   // wei 20150923: add ART count / Ifor 20170316 (wei): add MRT Mode
                {
                    if (Sum_ART > 0)
                    {
                        StrGrdCategoryART->Cells[2][1 + i] = ChangeToPercentage((double)LastSet.iBinData32_ART[0][i], (double)Sum_ART);
                    }
                    else
                    {
                        StrGrdCategoryART->Cells[2][1 + i] = "0.00%";
                    }
                }
            }

            if (LastSet.iTester != _2D_SORT)   // JerryYang 20230322: 2DID sort doesn't need this
            {
                for (i = 0; i < iTestBinCount; i++)   // Steven 20121112: RS232 supports 32Bin
                {
                    if (RunInfo.iUnloadCount < iLoadPersentCT[i])   // Steven 20140905: LastSet.SendCT[0] -> RunInfo.iUnloadCount
                        iLoadPersentCT[i] = RunInfo.iUnloadCount;

                    if (RunInfo.iUnloadCount < iLoadCountCT[i])
                        iLoadCountCT[i] = RunInfo.iUnloadCount;

                    if (RunInfo.iUnloadCount < iYeildCT[i])   // wei 20151111
                        iYeildCT[i] = RunInfo.iUnloadCount;

                    StrGrdCategory->Cells[1][1 + i] = LastSet.iBinData32[0][i];
                    StrGrdCategoryART->Cells[1][1 + i] = LastSet.iBinData32_ART[0][i];   // wei 20150923: add ART count
                    StrGrdCategoryContCT->Cells[1][1 + i] = (unsigned int)(ArmData[0]->GetSelBin(i) + ArmData[1]->GetSelBin(i));   // Sam 20240131 -- cast disambiguates AnsiString::operator= (unsigned long)

                    if (sum > 0)
                    {
                        f = ChangeToFloat((double)LastSet.iBinData32[0][i], (double)sum);
                        fSPBin = ChangeToFloat((double)LastSet.iBinData32[0][Prod.iSCKART_SPBinSelect], (double)sum);   // Isaac 20171113 (Steven): ATK Special Bin Yield alarm

                        StrGrdCategory->Cells[2][1 + i] = ChangeToPercentage((double)LastSet.iBinData32[0][i], (double)sum);

                        if (CosFunction.bCategoryInfoByContactCT)   // Sam 20240131
                        {
                            if (Sum_ConCT > 0)
                            {
                                f = ChangeToFloat((double)(ArmData[0]->GetSelBin(i) + ArmData[1]->GetSelBin(i)), (double)Sum_ConCT);
                                StrGrdCategoryContCT->Cells[2][1 + i] = ChangeToPercentage((double)(ArmData[0]->GetSelBin(i) + ArmData[1]->GetSelBin(i)), (double)Sum_ConCT);
                            }
                            else
                            {
                                f = 0.0;
                            }
                        }

                        if ((USE_AUTO_RETEST == eartInstall && IniConfig.bA10_AutoReTest) ||
                            CosFunction.bUseARTSortCount)   // wei 20150923: add ART count / Ifor 20170316 (wei): add MRT Mode
                        {
                            StrGrdCategoryART->Cells[2][1 + i] = ChangeToPercentage((double)LastSet.iBinData32_ART[0][i], (double)Sum_ART);
                        }

                        if (TestIF_File.bSCKART_EnableSPBinAlarm == true &&
                            TestIF_File.bSCKART_EnableART == true)   // Isaac 20171113 (Steven): ATK Special Bin Yield alarm
                        {
                            if (Prod.dSCKART_SPBinAlarmYield != 0.0 && fSPBin >= Prod.dSCKART_SPBinAlarmYield)
                            {
                                bSPBinYieldAlarm = true;
                            }
                            else
                            {
                                bSPBinYieldAlarm = false;
                            }
                        }
                        else
                        {
                            bSPBinYieldAlarm = false;
                        }

                        if (SystemStart && iHome == 0)
                        {
                            if (Prod.bFailure[i] == true)
                            {
                                if (IniConfig.bI30ContFailBin)   // kevin 20160407: add Fail-bin-over-limit warning
                                {
                                    if (Prod.dFailureLimit[i] != 0.0 && f >= Prod.dFailureLimit[i])
                                    {
                                        fYieldMonitoring->ClearYieldCount();

                                        if (CUSTOMER_CODE != CC_KYEC_LEE)   // wei 20151111
                                            iLoadPersentCT[i] = RunInfo.iUnloadCount;

                                        if (IniConfig.bEnableAutoCleanFunction &&   // JerryYang 20161121: fix Yield alarm needing to
                                                                                      // finish ONE CYCLE first, affecting auto clean
                                            TestIF.iAutoClean_Function == true &&
                                            (TestIF.iAutoClean_Mode & M_SOCKET_ALARM))
                                        {
                                            InitialAutoCleanAllTask();   // Sam 20230504: tidy InitialAutoCleanTask
                                        }
                                        S1.sprintf("Category %d count over limit %2.1f%% ", i, Prod.dFailureLimit[i]);
                                        DoLowYieldAlarm("WAR07357", S1);
                                        break;
                                    }
                                }
                                else
                                {
                                    if (CosFunction.bYieldControlBinSelectUseContactCount == true)   // KaiChen 20181115:
                                                                                                         // BinSelect Yield control uses Contact Count
                                    {
                                        if (iLowYieldBinSelectContactCount > Prod.iPersentIgnore[i] &&
                                            Prod.dFailureLimit[i] != 0 &&
                                            f >= Prod.dFailureLimit[i])
                                        {
                                            fShowBinSelect->iLowYieldBinSelectContactCount = 0;   // KaiChen 20181115
                                            fYieldMonitoring->ClearYieldCount();

                                            if (CUSTOMER_CODE != CC_KYEC_LEE)   // wei 20151111
                                                iLoadPersentCT[i] = RunInfo.iUnloadCount;
                                            if (IniConfig.bEnableAutoCleanFunction && TestIF.iAutoClean_Function == true && (TestIF.iAutoClean_Mode & M_SOCKET_ALARM))
                                            {
                                                InitialAutoCleanAllTask();   // Sam 20230504: tidy InitialAutoCleanTask
                                            }
                                            S1.sprintf("Category %d count over limit %2.1f%% ", i, Prod.dFailureLimit[i]);
                                            DoLowYieldAlarm("WAR07357", S1);   // Steven 20180627 (wei): consolidate Low Yield Alarm
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        bool bNeedCheck = false;
                                        if (CosFunction.bLowYieldUseContactCounts &&   // Sam 20230620: optimise Smart Auto Clean
                                            CUSTOMER_CODE != CC_PTI)                    // Sam 20250102: fix PTI AI Clean
                                        {
                                            if (Sum_ConCT > Prod.iPersentIgnore[i])   // Sam 20250102: fix PTI AI Clean
                                                bNeedCheck = true;
                                        }
                                        else
                                        {
                                            if (RunInfo.iUnloadCount - iLoadPersentCT[i] > Prod.iPersentIgnore[i])   // Steven 20140905
                                                bNeedCheck = true;
                                        }

                                        if (bNeedCheck &&
                                            Prod.dFailureLimit[i] != 0 &&
                                            f >= Prod.dFailureLimit[i])
                                        {
                                            fYieldMonitoring->ClearYieldCount();

                                            if (CUSTOMER_CODE != CC_KYEC_LEE)   // wei 20151111
                                                iLoadPersentCT[i] = RunInfo.iUnloadCount;
                                            if (IniConfig.bEnableAutoCleanFunction && TestIF.iAutoClean_Function == true && (TestIF.iAutoClean_Mode & M_SOCKET_ALARM))
                                            {
                                                InitialAutoCleanAllTask();   // Sam 20230504: tidy InitialAutoCleanTask
                                            }
                                            S1.sprintf("Category %d count over limit %2.1f%% ", i, Prod.dFailureLimit[i]);
                                            DoLowYieldAlarm("WAR07357", S1);   // Steven 20180627 (wei): consolidate Low Yield Alarm
                                            if (CosFunction.bSmartAutoClean)
                                            {
                                                // Smart Auto Clean clears the data once it finishes running
                                            }
                                            else
                                            {
                                                // GATE (B1): fCounterClear->ClearCount(ctBinCount); -- see
                                                // forms/fShowBinSelect.h GATE REGISTER (B1).
                                            }
                                            break;
                                        }
                                    }
                                }
                            }
                            else if (Prod.bFailCountEnable[i] == true)
                            {
                                if (IniConfig.bI30ContFailBin)   // kevin 20160407: add Fail-bin-over-limit warning
                                {
                                    if (Prod.iFailCountLimit[i] != 0 && LastSet.iBinData32[0][i] >= Prod.iFailCountLimit[i])
                                    {
                                        fYieldMonitoring->ClearYieldCount();
                                        if (CUSTOMER_CODE != CC_KYEC_LEE)   // wei 20151111
                                            iLoadCountCT[i] = RunInfo.iUnloadCount;
                                        // GATE (B1): fCounterClear->ClearCount(ctBinCount);
                                        if (IniConfig.bEnableAutoCleanFunction && TestIF.iAutoClean_Function == true && (TestIF.iAutoClean_Mode & M_SOCKET_ALARM))
                                        {
                                            InitialAutoCleanAllTask();   // Sam 20230504: tidy InitialAutoCleanTask
                                        }
                                        S1.sprintf("BIN %d over limit count %d ", i, Prod.iFailCountLimit[i]);
                                        DoLowYieldAlarm("WAR07358", S1);   // Steven 20180627 (wei): consolidate Low Yield Alarm
                                        break;
                                    }
                                }
                                else
                                {
                                    bool bNeedCheck = false;
                                    if (CosFunction.bLowYieldUseContactCounts &&   // Sam 20230620: optimise Smart Auto Clean
                                        CUSTOMER_CODE != CC_PTI)                    // Sam 20250102: fix PTI AI Clean
                                    {
                                        if (Sum_ConCT > Prod.iFailCountIgnore[i])   // Sam 20250102: fix PTI AI Clean
                                            bNeedCheck = true;
                                    }
                                    else
                                    {
                                        if (RunInfo.iUnloadCount - iLoadCountCT[i] > Prod.iFailCountIgnore[i])   // Steven 20140905
                                            bNeedCheck = true;
                                    }

                                    if (bNeedCheck &&
                                        Prod.iFailCountLimit[i] != 0 &&
                                        LastSet.iBinData32[0][i] >= Prod.iFailCountLimit[i])
                                    {
                                        fYieldMonitoring->ClearYieldCount();
                                        if (CUSTOMER_CODE != CC_KYEC_LEE)   // wei 20151111
                                            iLoadCountCT[i] = RunInfo.iUnloadCount;
                                        S1.sprintf("BIN %d over limit count %d ", i, Prod.iFailCountLimit[i]);
                                        DoLowYieldAlarm("WAR07358", S1);   // Steven 20180627 (wei): consolidate Low Yield Alarm
                                        if (CosFunction.bSmartAutoClean)
                                        {
                                            // Smart Auto Clean clears the data once it finishes running
                                        }
                                        else
                                        {
                                            // GATE (B1): fCounterClear->ClearCount(ctBinCount);
                                        }
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

//---------------------------------------------------------------------------
//  UPH_StringGridDblClick -- golden :2054-2099 (20090720 Steven: Delete 1 UPH Record)
//  GATE (B2): Application->MessageBoxA(...)==IDOK -- see forms/
//  fShowBinSelect.h GATE REGISTER.
//---------------------------------------------------------------------------
void TfShowBinSelect::UPH_StringGridDblClick(TObject * /*Sender*/)
{
    int iRow = 0, iTotalUPH = 0, iCount = 0;
    if (!SystemStart && AccessLevel >= iDefHonPrecLevel &&
        PageControl1->ActivePageIndex == 2)   // jou 2014-06-19: Security has 5 levels, 3 -> iDefHonPrecLevel
    {
        iRow = UPH_StringGrid->Selection.Top;
        if (iRow > 0 && iRow < 11)   // Row is between 1 and 10
        {
            // GATE (B2): fail-closed -- the destructive row-delete loop below
            // does not run without a real confirm dialog.
#if 0
            if (Application->MessageBoxA("Do you want to delete this record?", "Confirm", MB_OKCANCEL) == IDOK)
            {
                for (int i = iRow; i <= 10; i++)   // shift row[i+1]'s value into row[i]
                {
                    UPH_StringGrid->Cells[0][i] = UPH_StringGrid->Cells[0][i + 1];
                    UPH_StringGrid->Cells[1][i] = UPH_StringGrid->Cells[1][i + 1];
                    UPH_StringGrid->Cells[2][i] = UPH_StringGrid->Cells[2][i + 1];
                    UPH_StringGrid->Cells[3][i] = UPH_StringGrid->Cells[3][i + 1];
                    if (IniConfig.bVTESTFunction == true)   // RogerYang 20250224: Weitest requested 3 more columns
                    {
                        UPH_StringGrid->Cells[4][i] = UPH_StringGrid->Cells[4][i + 1];
                        UPH_StringGrid->Cells[5][i] = UPH_StringGrid->Cells[5][i + 1];
                        UPH_StringGrid->Cells[6][i] = UPH_StringGrid->Cells[6][i + 1];
                    }
                }

                for (int i = 1; i <= 10; i++)   // recompute the UPH average
                {
                    try
                    {
                        if (UPH_StringGrid->Cells[3][i] != "")
                        {
                            iTotalUPH += atoi(AnsiString(UPH_StringGrid->Cells[3][i]).c_str());
                            iCount++;
                        }
                    }
                    catch (...)
                    {
                        MyDBIProcess("Exception", "UPH_StringGridDblClick");
                    }
                }
                RunInfo.iAvgUPH = (iCount == 0) ? 0 : iTotalUPH / iCount;
                UPH_StringGrid->Cells[3][12] = RunInfo.iAvgUPH;
            }
#endif
            (void)iTotalUPH; (void)iCount;
        }
    }
}

//---------------------------------------------------------------------------
//  CaculateUPH -- golden :2279-2333 (Steven 20160727: show immediate UPH)
//  GATE (B3): fMain->StatusBar1->Panels->Items[7]->Text=... -- see forms/
//  fShowBinSelect.h GATE REGISTER. The UPH numbers themselves (iNetUPH/
//  iGrossUPH/iRecordEventLogUPH) are computed and stored regardless.
//---------------------------------------------------------------------------
void TfShowBinSelect::CaculateUPH()
{
    static int iOldSum = -1, PassSum = 0;
    static int UPH = 0, UPH2 = 0;
    static DWORD NowTickCount = (DWORD)-1, LastTimeTick = 1, PassSec = 0, PassSec2 = 0;
    int sum = 0;

    for (int i = 0; i < eTrayCount; i++)
    {
        if (Prod.iTrayType[i] != tNotUse)
            sum += LastSet.BinCT[0][iTo3Unload[i]];
    }

    if ((OutArmSuck.HasIC() || ShuttleHasIC() || IndexHasIC()) && sum > iOldSum)
    {
        NowTickCount = MyTickCount();
        if (NowTickCount < LastTimeTick)
        {
            iOldSum = sum;
            LastTimeTick = MyTickCount();
        }
        else
        {
            PassSec = (NowTickCount - LastTimeTick) / 1000;
            PassSec2 = PassSec - (iPauseTime / 1000);
            PassSum = sum - iOldSum;
            if (PassSec > 0)
            {
                UPH = PassSum * 3600 / PassSec;
                iNetUPH = UPH;

                if (PassSec2 > 0)   // Steven 20260421: add zero-guard for PassSec2
                    UPH2 = PassSum * 3600 / PassSec2;
                iGrossUPH = UPH2;
                // GATE (B3): fMain->StatusBar1->Panels->Items[7]->Text=
                // "Net UPH: "+...  /  "Curr UPH: "+... (golden :2313-2320,
                // both CC_ASE_CL and the general customer-code arms).
            }
        }
    }
    else
    {
        iPauseTime = 0;
        iOldSum = sum;
        LastTimeTick = MyTickCount();
        UPH = 0;
    }
    iRecordEventLogUPH = UPH;
}

//---------------------------------------------------------------------------
//  edSLT01Change -- golden :2334-2337
//  GATE (B3): the RHS itself (fMain->StatusBar1->Panels->Items[6]->Text)
//  needs the same missing StatusBar1 -- whole statement gated, no
//  independently computable value.
//---------------------------------------------------------------------------
void TfShowBinSelect::edSLT01Change(TObject * /*Sender*/)
{
    // GATE (B3): strcpy(LastSet.szJamClearData[0], fMain->StatusBar1->
    // Panels->Items[6]->Text.c_str());  // Alick 20160920 add for JCET
}

//---------------------------------------------------------------------------
//  btnSetInpputCntClick -- golden :2339-2352
//---------------------------------------------------------------------------
void TfShowBinSelect::btnSetInpputCntClick(TObject * /*Sender*/)
{
    if (IniConfig.bG15LoadInputCount)   // kevin 20211106: clean out once input count reached
    {
        try
        {
            StipulateInputCount = StrToInt(EdLoadCount->Text.c_str());   // kevin 20211106
        }
        catch (...)
        {
            StipulateInputCount = 0;
        }
    }
}

//---------------------------------------------------------------------------
//  RefreshAiCnt -- golden :2354-2364 (Sam 20210609: Fix AOI software upgrade)
//  GATE (B4): fFixAICCD->UnloadAICntNG[i] -- see forms/fShowBinSelect.h GATE
//  REGISTER. Safe default: blank text, same as the existing tNotUse else-arm.
//---------------------------------------------------------------------------
void TfShowBinSelect::RefreshAiCnt()
{
    for (int i = 0; i < eTrayCount; i++)
    {
        if (Prod.iTrayType[i] != tNotUse)
            EditAi[i]->Text = "";   // GATE (B4): should be IntToStr(fFixAICCD->UnloadAICntNG[i])
        else
            EditAi[i]->Text = "";
    }
}

//---------------------------------------------------------------------------
//  ed_AutoCleanCountClick -- golden :2216-2223
//  GATE (B5): fSecurity->Insufficient(43) forced false (fail-closed) +
//  fCleaning->btnResetCleanCountClick (moot while the outer gate is closed).
//---------------------------------------------------------------------------
void TfShowBinSelect::ed_AutoCleanCountClick(TObject * /*Sender*/)
{
    // GATE (B5): if(fSecurity->Insufficient(43)) { fCleaning->
    // btnResetCleanCountClick(Owner); ed_AutoCleanCount->Text=0; }
}

//---------------------------------------------------------------------------
//  labAuto1Click -- golden :2225-2242
//  GATE (B7): fBinSel->chkShow0Xbin->Checked forced false (Delphi's own
//  real-VCL checkbox default) -- the `else` arm (Width=331) is taken.
//---------------------------------------------------------------------------
void TfShowBinSelect::labAuto1Click(TObject * /*Sender*/)
{
    fShowBinSelect->Left = 10;
    fShowBinSelect->Top = 130;
    SetAutoVisible();

    if (PageControl1->ActivePageIndex == 0)   // tsTestBin
    {
        fShowBinSelect->Width = 1024;
    }
    else
    {
        // GATE (B7): fBinSel->chkShow0Xbin->Checked -- forced false, so the
        // else arm below always fires.
        fShowBinSelect->Width = 331;   // kevin 20140317: was 280
    }
}

//---------------------------------------------------------------------------
//  btReturnClick -- golden :2244-2251
//  GATE (B8): PageControl1Change(this) -- Wave B (no-op this wave, see below).
//---------------------------------------------------------------------------
void TfShowBinSelect::btReturnClick(TObject * /*Sender*/)
{
    fShowBinSelect->Left = 756;
    fShowBinSelect->Top = 412;
    fShowBinSelect->Left = 756;
    fShowBinSelect->Top = 420;
    PageControl1Change(nullptr);
}

//---------------------------------------------------------------------------
//  btnCleanResetClick -- golden :2253-2260
//  GATE (B5): same shape as ed_AutoCleanCountClick.
//---------------------------------------------------------------------------
void TfShowBinSelect::btnCleanResetClick(TObject * /*Sender*/)
{
    // GATE (B5): if(fSecurity->Insufficient(97)) { fCleaning->
    // btnResetCleanCountClick(Owner); ed_AutoCleanCount->Text=0; }
}

//---------------------------------------------------------------------------
//  btnClearCountClick -- golden :2262-2277
//  GATE (B6): fSecurity->Insufficient(108) forced false (fail-closed) for
//  every non-Greatek customer code; ShowMyMessageBox_YES_NO forced to "No";
//  fCounterClear->ClearCount(ctIndexCount) gated regardless (see (B1)).
//---------------------------------------------------------------------------
void TfShowBinSelect::btnClearCountClick(TObject * /*Sender*/)
{
    if (CUSTOMER_CODE != CC_Greatek)   // Sam 201700915 (Steven): Greatek's clear doesn't need permission
    {
        // AI(W906-FW-SecUnlock) 20260819: GATE (B6) permission half DISSOLVED
        // -- fSecurity real (FW-SecCC); Insufficient(108) is false while SEC1
        // stays closed, so this still returns for every non-Greatek customer
        // today (identical observable). The modal half below stays gated.
        if(fSecurity->Insufficient(108)==false)
            return;
    }

    // GATE (B6): int ret=ShowMyMessageBox_YES_NO(...); if(ret==2) return; --
    // untranslated modal (Automation/AGV_E84.h:58's own comment). Forced to
    // "No"/cancel:
    return;

    // GATE (B1): fCounterClear->ClearCount(ctIndexCount); -- unreachable
    // while the two gates above return early; recorded so it is not missed
    // when either lands.
}

//---------------------------------------------------------------------------
//  PageControl1Change -- GATE (B8): documented no-op, WAVE B (golden
//  :1479-1633). Declared only so btReturnClick's call site compiles/links.
//---------------------------------------------------------------------------
void TfShowBinSelect::PageControl1Change(TObject * /*Sender*/)
{
    // WAVE B: real body (golden :1479-1633) touches ~15 more widgets
    // (pnlShowBin, ScrollBox1, iFixMax-bounded loops, ...) not in this
    // facade yet. See forms/fShowBinSelect.h WAVE B QUEUE.
}

//---------------------------------------------------------------------------
//  ShowBinSel -- golden :388-756 (JerryYang 20220909: BinTrayTotal->eTrayCount)
//  AI(W906-FW-SBWB2) 20260819: FW-3 Wave B, primary target.
//
//  DEVIATION: golden builds two LOCAL arrays right at function entry,
//  `TPanel *UnLoadPanel[]={pnlAuto1,pnlAuto2,...}` / `TLabel *UnLoadLabel[]=
//  {lblAuto1,lblAuto2,...}` (golden :394-404), out of 33 individually-named
//  .dfm widgets. Absence-claim grep this wave (`grep -n "\bpnlAuto1\b"
//  cShowBinSelect.cpp`, `grep -n "\blblAuto1\b" cShowBinSelect.cpp`,
//  20260819): each of the 33 names is referenced in exactly ONE other place
//  in the whole golden class besides this literal-array line -- the still-
//  BLOCKED Wave C `ChangeBinDispStatus`'s OWN local array (golden :210-215,
//  opaque HSys.BinDisCtrl body). No golden method ever names e.g. `pnlAuto1`
//  a THIRD time. Same "array member, not ~33 individually named pointers"
//  shape as this file's own WAVE A ctor DEVIATION note for MyBinSel/
//  MyBinSelLab/etc -- `UnLoadPanel[]`/`UnLoadLabel[]` are therefore
//  TfShowBinSelect MEMBER arrays (forms/fShowBinSelect.h), populated once in
//  the ctor, referenced directly here (no local re-declaration).
//
//  HYDRATION: no per-slot .dfm geometry/Caption/Color hydrated for
//  UnLoadPanel[]/UnLoadLabel[]/pnlEmpty/pnlColor -- see forms/
//  fShowBinSelect.h HYDRATION section for the grep evidence (ShowBinSel
//  itself unconditionally overwrites ->Color for every slot with
//  Prod.iTrayType[i]!=tNotUse before anything in this wave's scope ever
//  reads it back; the tNotUse slots it skips are exactly the slots
//  SetAutoVisible/SetLabelVisible (Wave A, already ACTIVE) hides via
//  grpBinDisp[i]->Visible=false).
//
//  GATE (B9): fSortCT->myCountPanel[i].pnlYield/.pnlCount->Font->Color
//  (golden :640/:642/:648/:650) -- see forms/fShowBinSelect.h GATE REGISTER.
//  GATE (B10)/(B11): ShowBinSel_ARTNor()/ShowBinSel_ARTRT() (golden :752-753)
//  -- each still WAVE B QUEUE, declared below as documented no-ops (same
//  established shape as this file's existing (B8) PageControl1Change).
//  GOLDEN BUG (B12): see forms/fShowBinSelect.h GATE REGISTER -- kept
//  verbatim at its own line below.
//---------------------------------------------------------------------------
void TfShowBinSelect::ShowBinSel()
{
    int Data, iLengh = 0, i2 = 0;
    bool iRecord[100] = {false};
    AnsiString S[eTrayCount], asBuf;

    if (IniConfig.bVTESTFunction == true)   // RogerYang 20250224: Weitest requested 3 more columns
    {
        UPH_StringGrid->ColCount = 7;
    }

    for (int i = 0; i < eTrayCount; i++)   // JerryYang 20220909: BinTrayTotal -> eTrayCount
    {
        S[i] = "";                          // JerryYang 20230512: default blank string
        BinAssign[i] = "";
        bUnloadHasBin[i] = false;           // kevin 20180705: mark whether the tray has a BIN set
        iTrayLastBin[i] = 0;                // kevin 20180705: each tray's last bin
    }

    for (int j = 0; j < eTrayCount; j++)   // JerryYang 20220909: iBinSelCT -> eTrayCount
        iBinTray[j] = false;                // kevin 20170328 (Steven): whether TRAY uses BIN

    for (int i = 0; i < iTestBinCount; i++)
    {
        Data = Prod.iT6CatData[i];
        if (Data < 0)   // kevin 20140317: 256 bin, 0-start
            continue;

        for (int j = 0; j < eTrayCount; j++)
        {
            if (grpBinDisp[j]->Visible == false)
                continue;

            if (j == Data)
            {
                if (fBinSel->chkShow0Xbin->Checked)   // jou 20220719: show 0X bin
                {
                    asBuf.sprintf("%02d ", i);
                    S[j] += asBuf;
                }
                else
                {
                    if (IniConfig.bSPILFunction == true &&
                        CosFunction.bSortingBy2DList == true &&
                        LastSet.iTester == _2D_SORT &&
                        TestIF_File.bSortingBy2DIDList == true)   // JerryYang 20230322: SPIL re-IT requirement
                    {
                        if (S[j] == "")
                        {
                            S[j] = AnsiString(i);   // kevin 20140317: bin 0-start
                        }
                        else
                        {
                            S[j] += " " + AnsiString(i);   // kevin 20140317: bin 0-start
                        }
                    }
                    else
                    {
                        S[j] += AnsiString(i) + " ";   // kevin 20140317: bin 0-start
                    }
                }

                sBinData[j]      = AnsiString(i) + " ";   // kevin 20180202: record bin
                iBinTray[j]      = true;                   // kevin 20170328 (Steven)
                iLengh           = S[j].Length();
                BinAssign[j]    += AnsiString(i) + ",";    // kevin 20180705
                bUnloadHasBin[j] = true;                    // kevin 20180705
                iTrayLastBin[j]  = i;                       // kevin 20180705
                i2               = iLengh / 460;
                if (i2 != 0)
                {
                    if (iRecord[i2] == false)
                    {
                        iRecord[i2]   = true;
                        S[j]         += "\n\r";
                        sBinData[j]   = "\n\r";              // kevin 20180202
                        BinAssign[j] += "\n\r";               // kevin 20180705
                    }
                }
            }
            else
            {
                if (CosFunction.bSortingBy2DList == true &&
                    LastSet.iTester == _2D_SORT &&
                    TestIF_File.bSortingBy2DIDList == true)   // Frank 20221122: 2DID sorting for ATK
                {
                }
                else if (IniConfig.bSPILFunction == true)   // JerryYang 20250320: SPIL shared-tray convention
                {
                }
                else
                {
                    if (iTestBinCount <= 16)   // jou 2014-04-30: 15-bin repeat readability
                    {
                        S[j]         += ". ";
                        BinAssign[j] += ",";     // kevin 20180705
                    }
                }
            }
        }
    }

    Data = Prod.iIfErrorT6;
    if (TestIF_File.iTestType == RS232_MODE &&
        TestIF_File.iRs232Mode == eRs23232Bin)   // Steven 20121112: RS232 supports 32-bin
    {
        if (Data > 0)
        {
            S[Data]             = " E" + S[Data];
            sBinData[Data]      = " E" + S[Data];   // kevin 20180202
            iBinTray[Data]      = true;               // kevin 20170328 (Steven)
            BinAssign[Data]    += "E";                 // kevin 20180705
            bUnloadHasBin[Data] = true;                 // kevin 20180705
            iTrayLastBin[Data]  = 999;                  // kevin 20180705: Error bin
        }
    }
    else
    {
        if (iHWFix_BinBox == 1 || Data == -1)   // kevin 20160819 // Steven 20230929: && -> ||
        {
            S[eBulkBox]            += "E ";
            sBinData[eBulkBox]     += "E ";    // kevin 20180202
            iBinTray[eBulkBox]      = true;     // kevin 20170328 (Steven)
            BinAssign[eBulkBox]    += "E";      // kevin 20180705
            bUnloadHasBin[eBulkBox] = true;     // kevin 20180705
            iTrayLastBin[eBulkBox]  = 999;      // kevin 20180705: Error bin
        }
        else if (Data > 0)
        {
            if (IniConfig.bSPILFunction == true &&
                CosFunction.bSortingBy2DList == true &&
                LastSet.iTester == _2D_SORT &&
                TestIF_File.bSortingBy2DIDList == true)   // JerryYang 20230322: SPIL re-IT requirement
            {
                S[Data] += "ERR";
            }
            else
            {
                S[Data] += "E ";
            }
            sBinData[Data]      += "E ";   // kevin 20180202
            iBinTray[Data]       = true;    // kevin 20170328 (Steven)
            BinAssign[Data]     += "E";     // kevin 20180705
            bUnloadHasBin[Data]  = true;     // kevin 20180705
            iTrayLastBin[Data]   = 999;      // kevin 20180705: Error bin
            // GOLDEN BUG (B12): the very next statement (golden :545, kevin
            // 20220906) immediately overwrites the `=true` two lines above
            // back to `false` -- for this specific "Data>0, non-BulkBox
            // error-bin reroute" arm, bUnloadHasBin[Data] therefore ALWAYS
            // ends up false, making golden :543's `=true` dead code. Kept
            // verbatim (translation-fidelity policy) -- not "fixed" to
            // `=true` since that would change observable behaviour without
            // user sign-off. See forms/fShowBinSelect.h GATE REGISTER (B12).
            bUnloadHasBin[Data] = false;   // kevin 20220906
        }
    }

    if (iTestBinCount <= 16)
    {
        for (int i = 0; i < eTrayCount; i++)   // JerryYang 20220909: BinTrayTotal -> eTrayCount
        {
            S[i] += ". . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . ";
            if (IniConfig.bSPILFunction == true)   // JerryYang 20250320: SPIL shared-tray convention
            {
                AnsiString strTemp = BinAssign[i];
                int len = strTemp.Length();

                if (len > 0 && strTemp[len] == ',')
                {
                    BinAssign[i] = BinAssign[i].SubString(1, len - 1);
                }
            }
            else
            {
                BinAssign[i] += ",";   // kevin 20180705
            }
        }
    }

    for (int i = eFix2; i <= iFixMax; i++)
    {
        if (Prod.bLinkTo6Tray[i] == true)
        {
            if (S[i - 1].AnsiPos("LINK") == 0)
                S[i] = AnsiString("LINK ") + S[i - 1];
            else
                S[i] = S[i - 1];
            BinAssign[i] = BinAssign[i - 1];
        }
    }

    // Ifor 20231122: add Magazine Link
    if (AUTO3_IS_MAGAZINE == 1)
    {
        for (int i = 1; i < 14; i++)
        {
            if (BinSelect[iTestRunMode].bMagazineLink[i] == true)
            {
                for (int j = 0; j < 14; j++)
                {
                    // GOLDEN NOTE (not separately numbered -- see forms/
                    // fShowBinSelect.h GATE REGISTER (B12) note): if
                    // bMagazineLink[i..0] were ALL true, this inner loop
                    // would index bMagazineLink[i-j] with a negative
                    // subscript once j>i (out-of-bounds read). Unreachable in
                    // practice because bMagazineLink[eAuto1] (index 0) is
                    // never set true by any writer this tree ports, so the
                    // loop always breaks at/before j==i. Kept verbatim.
                    if (BinSelect[iTestRunMode].bMagazineLink[i - j] == false)
                    {
                        S[eMag1 + i] = AnsiString("LINK ") + S[eMag1 + (i - j)];
                        BinAssign[eMag1 + i] = BinAssign[eMag1 + (i - j)];
                        break;
                    }
                }
            }
        }
    }

    for (int i = 0; i < eTrayCount; i++)   // JerryYang 20220909: 10 -> eTrayCount // kevin 20160819/20110901: use FIX for 2 trays
    {
        MyBinSel[i]->Caption = S[i];
        if (S[i].Pos("LINK ") > 0)
        {
            UnLoadLabel[i]->Caption = DelDot(MyBinSel[i]->Caption.SubString(6, MyBinSel[i]->Caption.Length()));
        }
        else
        {
            UnLoadLabel[i]->Caption = DelDot(MyBinSel[i]->Caption);
        }

        // GOLDEN ODDITY (B13, integration 20260819): this gray-out branch is DEAD
        // in golden itself -- the 87-char SPACE-led needle below appears ONLY in
        // Pos() checks (golden :628 and both ART siblings :2543/:2762); the only
        // dots writer is the 80-char DOT-led literal (:561), and no code path
        // ever produces a caption BEGINNING with the needle (measured: needle not
        // in write, not in "0 "+write; j==1 unreachable). The colored else-branch
        // always runs for non-tNotUse trays. Translated verbatim, not simplified.
        int j = MyBinSel[i]->Caption.Pos(" . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . ");

        if (iHWFix_BinBox == 1 && i == eBulkBox)   // kevin 20160825
            continue;

        if (Prod.iTrayType[i] == tNotUse)
            continue;

        if (iTestBinCount <= 16 && j == 1)
        {
            UnLoadPanel[i]->Color       = clGray;
            UnLoadLabel[i]->Font->Color = clGray;
            // GATE (B9): fSortCT->myCountPanel[i].pnlYield->Font->Color=clGray;
            MyBinSel[i]->Font->Color    = clGray;
            // GATE (B9): fSortCT->myCountPanel[i].pnlCount->Font->Color=clGray;  // kevin 20150615
        }
        else
        {
            UnLoadPanel[i]->Color       = tcBinColor[Prod.iIsFailT6[i]];
            UnLoadLabel[i]->Font->Color = tcBinColor[Prod.iIsFailT6[i]];
            // GATE (B9): fSortCT->myCountPanel[i].pnlYield->Font->Color=tcBinColor[Prod.iIsFailT6[i]];
            MyBinSel[i]->Font->Color    = tcBinColor[Prod.iIsFailT6[i]];
            // GATE (B9): fSortCT->myCountPanel[i].pnlCount->Font->Color=tcBinColor[Prod.iIsFailT6[i]];  // kevin 20150615
        }

        MyBinSel[i]->Caption = S[i];   // JerryYang 20250320: SPIL shared-tray convention (redundant re-write, same value as above -- kept verbatim)
        if (IniConfig.bSPILFunction == true)
        {
            sSVBinAssign[i] = BinAssign[i];
        }
        else if (fAGV->IsATK_AMR())
        {
            int iLen = BinAssign[i].Length();   // RogerYang 20260403: remove trailing comma
            AnsiString sGetCommas = BinAssign[i].SubString(iLen, iLen);
            if (sGetCommas == ",")
                sSVBinAssign[i] = BinAssign[i].SubString(0, iLen - 1);
            else
                sSVBinAssign[i] = BinAssign[i];

            sSVBinAssign[i] = StringReplace(sSVBinAssign[i], "\r", "", TReplaceFlags() << rfReplaceAll);   // AI(ht9045-atk-amr-flow) 20260423 (RogerYang): strip CR/LF
            sSVBinAssign[i] = StringReplace(sSVBinAssign[i], "\n", "", TReplaceFlags() << rfReplaceAll);

            if (sBinCode_ATK[i] != BinAssign[i])
            {
                sBinCode_ATK[i] = BinAssign[i];
                EventReport(SECS_EVENT.OutputPort1BinCode + i);
            }
        }
        else
        {
            sSVBinAssign[i] = MyBinSel[i]->Caption;
        }
    }

    if (fAGV->IsATK_AMR())   // AI(ht9045-atk-amr-flow) 20260423 (RogerYang): sync BinAssign to Fix(Auto4~6)
    {
        for (int k = 0; k < 3; k++)
            sSVBinAssign[eAuto4 + k] = sSVBinAssign[eFix1 + k];
    }

    if (AUTO3_IS_MAGAZINE != 1)
    {
        for (int i = eMag1; i < eMag1 + 14; i++)
        {
            UnLoadLabel[i]->Caption = ". . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . ";
            UnLoadPanel[i]->Caption = "X";
            if (NUMBER_PANEL_TYPE == 3)   // 3-digit display
            {
                UnLoadPanel[i]->Color       = (TColor)0x000080FF;
                UnLoadLabel[i]->Font->Color = (TColor)0x000080FF;
            }
            else
            {
                UnLoadPanel[i]->Color       = clGray;
                UnLoadLabel[i]->Font->Color = clGray;
            }
        }
    }

    if (TrayForm.iFixTrayMode == false)   // remaining 3 Fix trays only when not split in half
    {
        for (int i = iFixRight + 1; i <= iFixRightHalf; i++)
        {
            UnLoadLabel[i]->Caption = ". . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . ";
            UnLoadPanel[i]->Caption = "X";
            if (NUMBER_PANEL_TYPE == 3 ||   // 3-digit display
                NUMBER_PANEL_TYPE == 4)      // Sam 20240604: new BinDisplay TFT
            {
                UnLoadPanel[i]->Color       = (TColor)0x000080FF;
                UnLoadLabel[i]->Font->Color = (TColor)0x000080FF;
            }
            else
            {
                UnLoadPanel[i]->Color       = clGray;
                UnLoadLabel[i]->Font->Color = clGray;
            }
        }
    }

    if (AUTO_EMPTY_COLOR != 0)   // has Empty/Color trays
    {
        pnlEmpty->Color = (TColor)0x000080FF;
        pnlColor->Color = (TColor)0x000080FF;
    }
    else
    {
        pnlEmpty->Color = clGray;
        pnlColor->Color = clGray;
    }

    if (IniConfig.bAutoTrayLink == true)   // jou 2012-06-14: Auto Tray Link
    {
        for (int i = eAuto2; i <= iAutoRight; i++)
        {
            if (Prod.bLinkTo6Tray[i] == true)
            {
                if (MyBinSel[i - 1]->Caption.AnsiPos("LINK") == 0)
                    MyBinSel[i]->Caption = AnsiString("LINK ") + MyBinSel[i - 1]->Caption;
                else
                    MyBinSel[i]->Caption = MyBinSel[i - 1]->Caption;
            }
        }
    }

    // GATE (B10)/(B11): ShowBinSel_ARTNor()/ShowBinSel_ARTRT() -- each still
    // WAVE B QUEUE (golden :2365-2580 / :2581-2799), declared as documented
    // no-ops below (see forms/fShowBinSelect.h GATE REGISTER).
    ShowBinSel_ARTNor();   // JerryYang 20220331: also show ART bin
    ShowBinSel_ARTRT();

    // S: golden passes `this` (TfShowBinSelect*->TObject* in VCL);
    // TfShowBinSelect has no vclcompat::TObject base (same shape as every
    // other facade in this family -- see forms/fContactCT.h's own note) and
    // PageControl1Change ignores Sender entirely (GATE (B8), documented
    // no-op) -- nullptr is semantics-identical. Same substitution shape as
    // this file's own btReturnClick, which already calls
    // `PageControl1Change(nullptr)` for the identical reason.
    PageControl1Change(nullptr);
}

//---------------------------------------------------------------------------
//  ShowBinSel_ARTNor -- GATE (B10): documented no-op this wave. Real body is
//  WAVE B QUEUE (golden :2365-2580, ~216 lines, guarded by
//  `if(IniConfig.bSPILFunction && bCanRunSCKART)` -- both default false on a
//  non-SPIL/non-ART machine). Declared so ShowBinSel's call site (golden
//  :752) compiles/links; its own widget surface (tsARTNormalBin, palARTNor,
//  s6TrayName[], bCanRunSCKART, Prod.bTo6AutoRetest[], Prod.iTo6CatData[]) is
//  not yet on this facade -- left for a later sub-wave per this wave's
//  "large form, slice it" scoping (see forms/fShowBinSelect.h WAVE B QUEUE).
//---------------------------------------------------------------------------
void TfShowBinSelect::ShowBinSel_ARTNor()
{
    // WAVE B QUEUE: real body is golden :2365-2580. See forms/
    // fShowBinSelect.h GATE REGISTER (B10).
}

//---------------------------------------------------------------------------
//  ShowBinSel_ARTRT -- GATE (B11): documented no-op this wave. Real body is
//  WAVE B QUEUE (golden :2581-2799, ~219 lines), same shape/guard as
//  ShowBinSel_ARTNor. Declared so ShowBinSel's call site (golden :753)
//  compiles/links.
//---------------------------------------------------------------------------
void TfShowBinSelect::ShowBinSel_ARTRT()
{
    // WAVE B QUEUE: real body is golden :2581-2799. See forms/
    // fShowBinSelect.h GATE REGISTER (B11).
}
