// =============================================================================
//  uYieldMonitoring.cpp  --  FW-3 uYieldMonitoring Wave A: pure yield engine core
//
//  Translation wave: FW-3 uYieldMonitoring Wave A
//  Translator: AI(W906-FW3-YieldMon-WA) 20260818
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/uYieldMonitoring.cpp
//  (5,981 lines) + uYieldMonitoring.h (627 lines), Big5/cp950. Decoded this
//  wave with `python3 -c "open(path,'rb').read().decode('cp950')"` -- 0
//  U+FFFD over both files (measured before any line below was written).
//
//  ROLE
//  ----
//  golden TfYieldMonitoring's PURE yield-engine core: per-tick site/picker/
//  by-arm/total/interval/special low-yield alarm checks, the sliding-window
//  ring buffer (SWRingReset/Push/CheckLowYieldAlarm_SW), the auto-site-off
//  judgement (CanAutoCloseSite, a pure predicate -- the ACTION,
//  DoAutoCloseSite, is out of scope), yield-count housekeeping
//  (ClearYieldCount/ClearAutoSiteOffStatus) and two small pure helpers
//  (CheckSettingNo, SetClosedSiteBin). See forms/fYieldMonitoring.h for the
//  full WAVE SCOPE table, GATE REGISTER and facade shape -- not duplicated
//  here to avoid the two files drifting apart.
//
//  ABSENCE-CLAIM TIMESTAMPS (commands + when run, this wave, before writing
//  the citing code below -- re-run at hand-off per project policy)
//  --------------------------------------------------------------------------
//    fContactCT facade   : `grep -rn "class.*TfContactCT\|fContactCT *;"
//                           --include=*.h .` over the port tree -- 0 hits for
//                           any TfContactCT type/global (20260818). cMyDB.cpp
//                           already carries 4 TODOs citing the same gap.
//    fShowBinSelect facade: `grep -rn "fShowBinSelect" --include=*.h forms/`
//                           -- 0 hits, no forms/fShowBinSelect.h exists
//                           (20260818). Two OTHER TUs (ainarm9045.cpp,
//                           csystem.cpp) carry TU-local macro seams covering
//                           only UPH_StringGrid -- invisible to this TU and
//                           missing every member this wave needs anyway.
//    fLotInfo Label17/18/21/edtAutoCleanLowYield/edtAutoCleanSiteYieldDiff:
//                           `grep -n "Label17\|Label18\|Label21\|
//                           edtAutoCleanLowYield\|edtAutoCleanSiteYieldDiff"
//                           forms/fLotInfo.h` -- 0 hits (fLotInfo.h exists,
//                           just missing these 5 members) (20260818).
//    fMain->CleanOut/ShowTestHeadComp: `grep -n "ShowTestHeadComp\|CleanOut"
//                           forms/fMain.h` -- both ALREADY present as real,
//                           ACTIVE facade methods (lines 156/168) (20260818).
//                           NOT gated -- see forms/fYieldMonitoring.h's own
//                           DESIGN NOTE.
//    Prod.bSlidingWindowYield/iSlidingWindowSize + TestIF_File's matching
//    pair : `grep -n "bSlidingWindowYield\|iSlidingWindowSize" cprod.h` --
//                           BOTH already exist in BOTH structs (cprod.h:
//                           645-646 PROD_INFO_ST, cprod.h:1810-1811
//                           SYSTEM_TEST_IF) -- landed by an earlier,
//                           unrelated wave. NOT a gap, no PORT-ONLY shadow
//                           field added (20260818).
//    Every fContactCT/fShowBinSelect touch's exact golden line number cited
//    in this file's per-site GATE comments was verified this wave by
//    `sed -n '<range>p' <cp950-decoded-and-CRLF-normalized-copy> | grep -n
//    "fContactCT\|fShowBinSelect"` -- absolute line numbers double-checked
//    by re-reading the cited lines directly out of the decoded golden text
//    (20260818).
//
//  DESIGN NOTE -- the (double) cast on TArm/TMySocket Get*() sites
//  --------------------------------------------------------------------------
//  Several sites here assign a `double`-returning Get*PCA()/GetByBin*PCA()
//  call directly into `double dSiteYield[][]`/`dPickerYield[][][]` -- no
//  cast needed (unlike cObserver.cpp's WriteContactKind, which had to cast
//  `unsigned long`-returning GetTotal()/GetPassCT() into AnsiString). Noted
//  here only because that OTHER cast convention (cSocket.cpp:838 precedent)
//  does NOT apply to this file -- nothing here needs it.
//
//  DESIGN NOTE -- golden bare `abs()` on a double -> `fabs()`
//  --------------------------------------------------------------------------
//  Golden calls plain `abs(<double expression>)` six times (CheckBySiteYieldAlarm
//  x2, CheckBySiteByArmYieldAlarm x4). BCB6's runtime library overloads `abs()`
//  for double directly (Borland's own <math.h>/<stdlib.h> both declare it), so
//  golden's own call compiles and returns the correct floating-point
//  magnitude there. Under this tree's MinGW g++ 6.3.0 oracle, unqualified
//  `abs(double)` is AMBIGUOUS: `<cstdlib>` (pulled in transitively by
//  vclcompat/AnsiString.h) brings `int abs(int)`/`std::abs(long)`/
//  `std::abs(long long)` into the overload set, and none of those has a
//  better-or-equal conversion sequence than any other for a `double`
//  argument -- measured this wave (`g++ -fsyntax-only`, first attempt used
//  bare `abs()`, got exactly this ambiguity at all 6 sites, 20260818).
//  `fabs()` (also `<cmath>`, unambiguous, double-only) computes the IDENTICAL
//  IEEE-754 magnitude -- same fix class as cObserver.cpp's `(long)` cast on
//  GetTotal()/GetPassCT() for a different unqualified-overload ambiguity.
//
//  DESIGN NOTE -- "faithfully dead" branches preserved verbatim
//  --------------------------------------------------------------------------
//  A few golden branches are provably unreachable given their own enclosing
//  condition (e.g. CheckLowYieldAlarm's inner `if(Prod.bFailAlarmLowYield &&
//  Prod.dLowYieldLimit!=0){...} else {iYeildCT[1]=RunInfo.iUnloadCount;}`,
//  where the SAME two-term condition already gates the OUTER `if` one level
//  up). Translated verbatim, not simplified or dropped -- same "don't
//  editorialize golden's own redundancy" posture as every prior wave.
// =============================================================================
#include "forms/fYieldMonitoring.h"

#include "MachineType.h"        // MAX_SOCKET_ROW/COL, NN_1Row/NN_2Row, rsmAutoSiteMap, eartInstall,
                                 //   CC_TERAPOWER/CC_HANA_MICRON/CC_JCET/CC_KYEC_LEE/CC_KYEC_XILINX,
                                 //   MAX_Index_Row/Col, eTrayCount/tNotUse, CheckRange<T>, ChangeToFloat/
                                 //   ChangeToFloatNonPcnt
#include "cmydef.h"              // SystemStart/iHome/bRunAutoClean/bZ1PickShuttle/bZ2PickShuttle/
                                 //   bUseTwoArm32Site/bLowYeildAlarm/bLowYeildAlarmSpecial/
                                 //   bLowYeildAlarmSpecial1stPass/dAdaptiveStardardYield/iYeildCT[]/
                                 //   bLowYieldCloseSite[][][]/iAutoClean_IndexContactCount/
                                 //   ContinuousFailSKTCount/SpecialBinContinuousFail*Count/
                                 //   ContinuousFailARMCount/iLoadPersentCT/iLoadCountCT/iYieldSiteBinpass/
                                 //   bIntervalYieldIsPass/bYieldSiteBin*/bYieldTotalBin*/iYieldTotalCount/
                                 //   iYieldTotalBinpass/iYieldSiteCount/bCanAutoCloseSite/
                                 //   NEW_MAX_Index_Col/IndexSuckName/M_SOCKET_ALARM/M_INTERVAL/
                                 //   USE_AUTO_RETEST/iTestBinCount/K_RETRY/K_ONECYCLE/MMInterface/
                                 //   CUSTOMER_CODE
#include "cprod.h"               // Prod/TestIF/TestIF_File/RunInfo, FT, iTo3Unload[]
#include "LastSet.h"             // LastSet (BinCT/iBinData32/BinCT_PTI/bUseTestSocket/iCloseSiteByLowYield/iRunStartMode)
#include "Config.h"              // IniConfig
#include "CosFunction.h"         // CosFunction
#include "aHotPlateSubstrate.h"  // TestSocket / FTestSuck (TMyKitSuck) -- NOT mykitsuck.h, see KNOWLEDGE.md
#include "cSocket.h"             // TArm/TMySocket, ArmData[3]/ArmHistory[3]/ArmData_AutoClean[3]
#include "cinitial.h"            // IsNNMode()
#include "atester_shims.h"       // fContact (TfContactShim)
#include "canary_support.h"      // ShowErrorMessage/RecordProcess
#include "atester_ProcessCount.h" // DoLowYieldAlarm
#include "AutoClean/AutoClean.h" // InitialAutoCleanAllTask
#include "FormsFacade.h"         // fMain (CleanOut -- see DESIGN NOTE, forms/fYieldMonitoring.h)
// AI(W906-FW-YEnable) 20260818: fContactCT (Y1)/fShowBinSelect (Y2) gate
// enablement -- these two facades landed (ecf6154) but this TU never included
// their headers while every touch was still `#if 0`'d. Added here (this
// wave's own write boundary, not forms/fYieldMonitoring.h) so the now-live
// fContactCT->/fShowBinSelect-> call sites below resolve.
#include "forms/fContactCT.h"    // fContactCT (Y1)
#include "forms/fShowBinSelect.h" // fShowBinSelect (Y2)

#include <cstdlib>               // atof
#include <cmath>                 // fabs(double) -- see DESIGN NOTE below
#include <cstring>               // memset

// AI(W906-FW3-YieldMon-WA) 20260818: TU-local forward decl for
// MyDBIProductionData (golden cMyDB.h:87, real body cMyDB.cpp:642) instead of
// `#include "cMyDB.h"`. Discovered this wave: cMyDB.h ALSO redeclares
// RecordProcess (cMyDB.h:122, matching canary_support.h:70's own default
// argument -- illegal to specify the same default twice across two visible
// declarations in one TU) and MyDBIProcessNew (cMyDB.h:82, marked
// `__fastcall`; canary_support.h:210's own declaration of the SAME function
// is NOT marked `__fastcall` -- a real ABI mismatch per vclcompat/
// vcl_compat.h's own "__fastcall is a REAL MinGW calling convention, keep
// declaration/definition pairs in lockstep" audit note, just latent until
// now because no existing TU included both headers together -- checked this
// wave, `grep` for genuine (non-comment) `#include "cMyDB.h"` +
// `#include "canary_support.h"` pairs across the tree: 0 hits before this
// file). Fixing either header is a shared-component change outside this
// wave's write boundary (forms/fYieldMonitoring.h, uYieldMonitoring.cpp,
// tests/test_yieldmon_core.cpp only) -- flagged in this wave's hand-off for
// a future cMyDB.h/canary_support.h audit. This one matching, `__fastcall`-
// consistent-with-the-real-definition forward declaration sidesteps the
// conflict cleanly without touching either shared header.
extern void __fastcall MyDBIProductionData(AnsiString sAction);

// =============================================================================
//  CalculateSiteYield -- golden :3639-3808
// =============================================================================
void TfYieldMonitoring::CalculateSiteYield()
{
    double dYield=0, dSiteCount=0;

    if(TestIF_File.iAutoClean_Function &&
       (TestIF_File.iAutoClean_Mode&M_SOCKET_ALARM ||
        TestIF_File.iAutoClean_Mode&M_INTERVAL))                                //ChungHung 20131223 add for SCK
    {
        // GATE (Y1)+(Y3): fLotInfo->Label17/18/21->Caption + fLotInfo->
        // edtAutoCleanLowYield/edtAutoCleanSiteYieldDiff->Text (golden :3647-3654)
        // -- fLotInfo exists but carries none of these 5 members; that is the
        // ONLY remaining blocker (fContactCT->GetLowYield_AutoClean itself IS
        // ported and ACTIVE, called live elsewhere in this file -- kept inside
        // this gate only because its output has nowhere to go).
        // See forms/fYieldMonitoring.h GATE REGISTER (Y1)/(Y3).
#if 0
        fLotInfo->Label17->Caption = "User set : " + AnsiString(TestIF.iAutoClean_LowYieldLimit) + "%" +
                                    (TestIF.bAutoClean_FailAlarmLowYield?" Enable":" Disable");
        fLotInfo->Label18->Caption =  "User set : " + AnsiString(TestIF.iAutoClean_FailAlarmSiteYield) + "%" +
                                    (TestIF.bAutoClean_FailAlarmSiteYieldDifferent?" Enable":" Disable");
        fLotInfo->Label21->Caption = "User set : " + AnsiString(TestIF.iAutoClean_IntervalContact) +
                                    (TestIF_File.iAutoClean_Mode & M_INTERVAL?"/Contact Enable":"/Contact Disable");
        fLotInfo->edtAutoCleanLowYield->Text = AnsiString(fContactCT->GetLowYield_AutoClean(0));
        fLotInfo->edtAutoCleanSiteYieldDiff->Text = AnsiString(fContactCT->GetLowYield_AutoClean(1));
#endif
    }
    else
    {
        iAutoClean_FailAlarmSiteYieldIntervalCount=0;
        // GATE (Y1)+(Y3): fContactCT->ClearData_AutoClean() + the same
        // fLotInfo block as the `if` arm above (golden :3659-3664).
#if 0
        fContactCT->ClearData_AutoClean();                                      //ChungHung 20131225 add for SCK
        fLotInfo->Label17->Caption = "User set : " + AnsiString(TestIF.iAutoClean_LowYieldLimit) + "%" + " Disable";
        fLotInfo->Label18->Caption = "User set : " + AnsiString(TestIF.iAutoClean_FailAlarmSiteYield) + "%" + " Disable";
        fLotInfo->Label21->Caption = "User set : " + AnsiString(TestIF.iAutoClean_IntervalContact) + " Disable";
        fLotInfo->edtAutoCleanLowYield->Text = AnsiString(fContactCT->GetLowYield_AutoClean(0));
        fLotInfo->edtAutoCleanSiteYieldDiff->Text = AnsiString(fContactCT->GetLowYield_AutoClean(1));
#endif
    }

    if(SystemStart &&
       fContact->fShow==false &&
       iHome==0 &&
       bRunAutoClean==false)
    {                                                                           //KEVIN 20130710 Site Differ Yield% (After 1 min)
        if(IsNNMode()==NN_2Row)                                                 //Steven 20220418 : NN mode Yield alarm
        {
            for(int i=0; i<FTestSuck.iShtRow && i+2<4; i++)               //Steven 20260421 : add boundary guard for bUseTestSocket[0][i+2]
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(Prod.bLowYieldAlarmByBin)                                //Steven 20140828 : By Bin Yield Monitor
                    {
                        dIndexZ1Yield=(LastSet.bUseTestSocket[0][i+2][j])?ArmData[0]->ArmSKET[i][j]->GetByBinSiteYieldPassPCA():0.0;
                        dIndexZ2Yield=(LastSet.bUseTestSocket[0][i  ][j])?ArmData[1]->ArmSKET[i][j]->GetByBinSiteYieldPassPCA():0.0;                            //Sam 20250401 : 修正Yield問題
                    }
                    else
                    {
                        dIndexZ1Yield=(LastSet.bUseTestSocket[0][i+2][j])?ArmData[0]->ArmSKET[i][j]->GetBySitePCA():0.0;
                        dIndexZ2Yield=(LastSet.bUseTestSocket[0][i  ][j])?ArmData[1]->ArmSKET[i][j]->GetBySitePCA():0.0;                                        //Sam 20250401 : 修正Yield問題
                    }
                    dPickerYield[0][i][j]=dIndexZ1Yield;                        //Steven 20230223 : 根據Index吸嘴比較良率
                    dPickerYield[1][i][j]=dIndexZ2Yield;
                    dSiteYield[i+2][j]=dIndexZ1Yield;                           //Steven 20220419 : fixed for nn mode
                    dSiteYield[i  ][j]=dIndexZ2Yield;
                }
            }
        }
        else if(IsNNMode()==NN_1Row)
        {
            for(int j=0; j<FTestSuck.iShtCol; j++)
            {
                if(Prod.bLowYieldAlarmByBin)                                    //Steven 20140828 : By Bin Yield Monitor
                {
                    dIndexZ1Yield=(LastSet.bUseTestSocket[0][0][j])?ArmData[0]->ArmSKET[0][j]->GetByBinArmYieldPassPCA():0.0;                                   //Steven 20260316 : 修正ByArm關site的Yield計算j+1 --> j
                    dIndexZ2Yield=(LastSet.bUseTestSocket[1][0][j])?ArmData[1]->ArmSKET[0][j]->GetByBinArmYieldPassPCA():0.0;
                }
                else
                {
                    dIndexZ1Yield=(LastSet.bUseTestSocket[0][0][j])?ArmData[0]->ArmSKET[0][j]->GetBySitePCA():0.0;
                    dIndexZ2Yield=(LastSet.bUseTestSocket[1][0][j])?ArmData[1]->ArmSKET[0][j]->GetBySitePCA():0.0;
                }
                dSiteYield[1][j]=dIndexZ1Yield;
                dSiteYield[0][j]=dIndexZ2Yield;
                dPickerYield[0][0][j]=dIndexZ1Yield;                            //Steven 20230223 : 根據Index吸嘴比較良率
                dPickerYield[1][0][j]=dIndexZ2Yield;
                dPickerYield[0][1][j]=0.0;
                dPickerYield[1][1][j]=0.0;
            }
        }
        else
        {
            for(int i=0; i<TestSocket.iShtRow; i++)
            {
                for(int j=0; j<TestSocket.iShtCol; j++)
                {
                    if(LastSet.bUseTestSocket[0][i][j] ||                       //Isaac 20210630 : 修正關arm會失效，&&->||
                       LastSet.bUseTestSocket[1][i][j])
                    {
                        if(Prod.bLowYieldAlarmByBin)                            //Steven 20140828 : By Bin Yield Monitor
                        {
                            dIndexZ1Yield=(LastSet.bUseTestSocket[0][i][j])?ArmData[0]->ArmSKET[i][j]->GetByBinSiteYieldPassPCA():0.0;
                            dIndexZ2Yield=(LastSet.bUseTestSocket[1][i][j])?ArmData[1]->ArmSKET[i][j]->GetByBinSiteYieldPassPCA():0.0;
                        }
                        else
                        {
                            dIndexZ1Yield=(LastSet.bUseTestSocket[0][i][j])?ArmData[0]->ArmSKET[i][j]->GetBySitePCA():0.0;                                      //kevin 20130710 by Site 計數
                            dIndexZ2Yield=(LastSet.bUseTestSocket[1][i][j])?ArmData[1]->ArmSKET[i][j]->GetBySitePCA():0.0;                                      //Steven 20230224 : 加上開關Site計算
                        }

                        dPickerYield[0][i][j]=dIndexZ1Yield;                    //Steven 20230223 : 根據Index吸嘴比較良率
                        dPickerYield[1][i][j]=dIndexZ2Yield;

                        if(TestIF.iShuttleMode==0)                              //jou 2014-08-14 Site Compare Low Yield alarm
                        {
                            dYield=0;
                            dSiteCount=0;
                            if(LastSet.bUseTestSocket[0][i][j])                 //Steven 20230224 : 修正ByArm關site的Yield計算
                            {
                                dYield+=dIndexZ1Yield;
                                dSiteCount++;
                            }

                            if(LastSet.bUseTestSocket[1][i][j])
                            {
                                dYield+=dIndexZ2Yield;
                                dSiteCount++;
                            }

                            if(dSiteCount==0)
                                dSiteYield[i][j]=0;
                            else
                                dSiteYield[i][j]=dYield/dSiteCount;
                        }
                        else if(TestIF.iShuttle_Sel==0)
                        {
                            dSiteYield[i][j]=dIndexZ1Yield;
                        }
                        else
                        {
                            dSiteYield[i][j]=dIndexZ2Yield;
                        }
                        //=== Sliding Window Ring Buffer ===                    //Steven 20260331
                        if(Prod.bSlidingWindowYield && Prod.iSlidingWindowSize>0)
                        {
                            for(int arm=0; arm<2; arm++)
                            {
                                unsigned long curT=ArmData[arm]->ArmSKET[i][j]->BySiteTotal;
                                unsigned long curP=ArmData[arm]->ArmSKET[i][j]->BySitePass;
                                if(curT > SW[arm][i][j].iLastBySiteTotal)
                                {
                                    unsigned long dT=curT-SW[arm][i][j].iLastBySiteTotal;
                                    unsigned long dP=(curP>=SW[arm][i][j].iLastBySitePass)?
                                                     curP-SW[arm][i][j].iLastBySitePass:0;
                                    for(unsigned long d=0; d<dT; d++)
                                        SWRingPush(arm, i, j, d<dP, Prod.iSlidingWindowSize);
                                    SW[arm][i][j].iLastBySiteTotal=curT;
                                    SW[arm][i][j].iLastBySitePass =curP;
                                }
                            }
                            int swCnt=SW[0][i][j].iCount+SW[1][i][j].iCount;
                            if(swCnt>0)
                            {
                                int swPas=SW[0][i][j].iPassCT+SW[1][i][j].iPassCT;
                                dSiteYield[i][j]=(double)swPas*100.0/swCnt;
                            }
                        }
                        //=== End Sliding Window ===
                    }
                    else
                    {
                        dSiteYield[i][j]=0.0;
                        dPickerYield[0][i][j]=0.0;                              //Steven 20230223 : 根據Index吸嘴比較良率
                        dPickerYield[1][i][j]=0.0;
                    }
                }
            }
        }
    }
    bFirstCount=false;
}

// =============================================================================
//  CheckBySiteYieldAlarm -- golden :3809-4032 (Site Yield Alarm(%))
// =============================================================================
void TfYieldMonitoring::CheckBySiteYieldAlarm()
{
    static int iCount2=0;

    int sum=0;
    int iSiteCount=0, ret=0;
    int iAlarmTimeInterval=(CosFunction.bLowYieldAlarmIntervalTimeBySetting &&  //JimmyChiu 20220601 : 修改low yield報警時間邏輯，報警間隔時間固定1min改為可以自行設定報警間隔時間
                            IniConfig.bI44_LowYieldAlarmIntervalTimeBySetting)?IniConfig.iI44_LowYieldAlarmIntervalTime:60;

    bool bYieldDiffOver=false, bNeedToCheck=false, bNeedToCloseSite=false;
    // AI(W906-FW-YEnable) 20260818: gate dissolved -- fContactCT landed
    // (ecf6154), `sum`/`dYield` are real reads again (see the un-gated
    // `if(sum>0){...}` block below); the `(void)` suppressions this wave's
    // predecessor needed while that block was `#if 0`'d are gone.
    double dYield=0.0;
    double IndexZ1Yield=0.0, IndexZ2Yield=0.0;                                  //wei 20180709 (steven) BySiteYieldAlarm不作動異常
    AnsiString ErrPart="";
    AnsiString aLowYield="";

    if(bFirstCount)
    {
        iCount2=0;
        return;
    }

    if(SystemStart &&
       fContact->fShow==false &&
       iHome==0 &&
       bRunAutoClean==false &&
       LastSet.iRunStartMode!=rsmAutoSiteMap)                                   //Steven 20230313 : 做Auto Site Map的時候不要檢查Yield
    {
        iCount2++;
        if(bZ1PickShuttle || bZ2PickShuttle)                                    //Steven 20180521 : 避免吸料的時候發Low Yield Alarm  //JerryYang 20180629 (wei) 避免count被歸零
            return;
        if(iCount2>=iAlarmTimeInterval ||                                       //JimmyChiu 20220601 : 修改low yield報警時間邏輯，報警間隔時間固定1min改為可以自行設定報警間隔時間
           CosFunction.bYieldAlarmNoWait1Min)                                   //wei 20150820  Yield Alarm No Wait 1Min
        {
            iCount2=0;
        }
        else
        {
            return;
        }

        if(TestIF_File.iAutoClean_Function &&                                   //Sam 20230104 : 修正 LowYield AutoClean
           (TestIF_File.iAutoClean_Mode&M_SOCKET_ALARM) &&
           TestIF.bAutoClean_FailAlarmSiteYieldDifferent &&
           TestIF.iAutoClean_FailAlarmSiteYield!=0)                             //Steven 20220419 : 往上移動
        {
            if(CUSTOMER_CODE==CC_TERAPOWER)                                     //Sam 20230104 : 晶兆成改用 IC 數量
                sum=ArmData_AutoClean[0]->GetTotalCT()+ArmData_AutoClean[1]->GetTotalCT();
            else
                sum=iAutoClean_FailAlarmSiteYieldIntervalCount;

            // AI(W906-FW-YEnable) 20260818: gate dissolved -- fContactCT/fShowBinSelect landed (ecf6154)
            if(sum>0)
            {
                dYield=fContactCT->GetLowYield_AutoClean(1);
                if(sum>=TestIF.iAutoClean_FailAlarmSiteYieldDifferentCount &&
                   dYield>TestIF.iAutoClean_FailAlarmSiteYield)
                {
                    aLowYield="AutoClean : Site Yield Different : " + AnsiString(dYield) + "%" + " Contact Count : " + iAutoClean_IndexContactCount;
                    RecordProcess(aLowYield);
                    InitialAutoCleanAllTask();                                  //Sam 20230504 : 整理 InitialAutoCleanTask
                    iAutoClean_FailAlarmSiteYieldIntervalCount=0;
                }
            }
        }

        if((TestIF_File.iAutoClean_Function==false &&
            TestIF_File.iAutoClean_Mode&M_SOCKET_ALARM)==0 &&
            TestIF.bAutoClean_FailAlarmSiteYieldDifferent==false)
        {
            iAutoClean_FailAlarmSiteYieldIntervalCount=0;
        }

        if(bUseTwoArm32Site==true ||                                            //Steven 20220419 : NN mode不需要比by arm
           TestIF.iShuttleMode!=0)                                              //ChungHung 20130114 關單Arm 要自動關閉)
        {
            iSiteCount=0;
            bYieldDiffOver=false;
            bNeedToCheck=false;
        }
        else                                                                    //KEVIN 20130710 Site Differ Yield% (After 1 min)
        {
            if(TestIF.iShuttleMode==0 && Prod.dFailAlarmSiteYield>0)
            {
                if(CosFunction.bYieldControlUseEACount)                         //wei 20180606 Yield控制使用EA Count    //Steven 20230223 : 簡化判斷式
                {
                    bNeedToCheck=(RunInfo.iUnloadCount-iYeildCT[0]>=Prod.iFailAlarmSiteYieldDifferentCount);
                }
                else
                {
                    bNeedToCheck=(iFailAlarmSiteYieldIntervalCount>=Prod.iFailAlarmSiteYieldDifferentCount);
                }
            }
            else
            {
                bNeedToCheck=false;
            }

            for(int i=0; i<TestSocket.iShtRow; i++)
            {
                for(int j=0; j<TestSocket.iShtCol; j++)
                {
                    if(LastSet.bUseTestSocket[0][i][j])
                        iSiteCount++;
                    if(LastSet.bUseTestSocket[1][i][j])
                        iSiteCount++;

                    if(LastSet.bUseTestSocket[0][i][j] &&
                       LastSet.bUseTestSocket[1][i][j])
                    {
                        IndexZ1Yield=ArmData[0]->ArmSKET[i][j]->GetBySitePCA();                                         //wei 20180709 (steven) BySiteYieldAlarm不作動異常
                        IndexZ2Yield=ArmData[1]->ArmSKET[i][j]->GetBySitePCA();                                         //wei 20180709 (steven) BySiteYieldAlarm不作動異常

                        if(bNeedToCheck &&                                      //Steven 20230223 : 簡化判斷式
                           (IndexZ1Yield!=0 && IndexZ2Yield!=0) &&
                           (fabs(IndexZ1Yield-IndexZ2Yield)>Prod.dFailAlarmSiteYield))                                   //JerryYang 20160530 LowYieldLimit要能設定到小數點 -- AI(W906-FW3-YieldMon-WA) 20260818: golden bare abs() on a double -- fabs() here, see file-head DESIGN NOTE
                        {
                            iSiteCount--;
                            ErrPart+=IndexSuckName[i][j];
                            bShowSiteYield[j+i*NEW_MAX_Index_Col]=true;         //Steven 20111115 : 8-> NEW_MAX_Index_Col
                            // AI(W906-FW-YEnable) 20260818: gate dissolved -- fContactCT/fShowBinSelect landed (ecf6154)
                            fContactCT->sgYield->Refresh();
                            bYieldDiffOver=true;
                        }
                        else
                        {
                            bShowSiteYield[j+i*NEW_MAX_Index_Col]=false;        //Steven 20111115 : 8-> NEW_MAX_Index_Col
                        }
                    }
                }
            }
        }

        if(CosFunction.bLowYieldAutoSiteOff &&                                  //Sam 20221202 : 修正自動關 Site 關到剩餘設定 Site 數時需要報警。
           TestIF_File.bLowYieldAutoSiteOffByArmSite &&                         //Steven 20230223 : by arm by site, auto site off
           iRunStartMode==FT)
        {
            if(iSiteCount<2)
                bNeedToCloseSite=false;
            else if(iSiteCount<TestIF_File.iAlarmWhenSiteOnCountLess)
                bNeedToCloseSite=false;
            else
                bNeedToCloseSite=true;
        }

        if(iSiteCount<2)                                                        //Steven 20200522 : 改成2, 只剩下一個site就不用比了
        {
            bYieldDiffOver=false;
        }

        if(iRunStartMode==FT &&
           IniConfig.bA09_ByArmCloseSite    &&
           CosFunction.bLowYieldAutoSiteOff &&                                  //Steven 20170905 (wei) : Low Yield Auto Site Off for Ambient
           TestIF_File.bLowYieldAutoSiteOffByArmSite &&                         //Steven 20230223 : by arm by site, auto site off
           iSiteCount>=TestIF_File.iAlarmWhenSiteOnCountLess &&                 //Steven 20200522 : 改成數字比對, 避免參數混用
           bNeedToCloseSite==true)                                              //Sam 20221207 : LowYieldAutoSiteOff 新增 Alarm 幾次後再來關 Site
        {
            for(int i=0; i<TestSocket.iShtRow; i++)
            {
                for(int j=0; j<TestSocket.iShtCol; j++)
                {
                    if(LastSet.bUseTestSocket[0][i][j] &&
                       LastSet.bUseTestSocket[1][i][j])
                    {
                        IndexZ1Yield=ArmData[0]->ArmSKET[i][j]->GetBySitePCA();                                         //wei 20180709 (steven) BySiteYieldAlarm不作動異常
                        IndexZ2Yield=ArmData[1]->ArmSKET[i][j]->GetBySitePCA();                                         //wei 20180709 (steven) BySiteYieldAlarm不作動異常

                        if(bNeedToCheck &&                                      //Steven 20230223 : 簡化判斷式
                           Prod.dFailAlarmSiteYield!=0 &&                       //ChungHung 20130114 關單Arm 要自動關閉
                           (IndexZ1Yield!=0 && IndexZ2Yield!=0) &&
                           (fabs(IndexZ1Yield-IndexZ2Yield)>Prod.dFailAlarmSiteYield))                                   //JerryYang 20160530 LowYieldLimit要能設定到小數點 -- AI(W906-FW3-YieldMon-WA) 20260818: golden bare abs() on a double -- fabs() here, see file-head DESIGN NOTE
                        {
                            if(IndexZ1Yield>IndexZ2Yield)
                                bLowYieldCloseSite[0][i][j]=true;
                            else
                                bLowYieldCloseSite[1][i][j]=true;
                        }
                    }
                }
            }
        }
        else
        {
            if(Prod.bFailAlarmSiteYieldDifferent &&
               Prod.iFailAlarmSiteYieldDifferentCount!=0)
            {
                if(bNeedToCheck)                                                //Steven 20230223 : 簡化判斷式
                {
                    if(bYieldDiffOver==true && TestIF.iShuttleMode==0)          //jou 2012-09-20 修正 Site Yield Different 關單 arm 會一直 alarm
                    {
                        if(bLowYeildAlarm==false)                               //wei 20151116 Yeild Alarm 只能Onecycle
                        {
                            ret=DoLowYieldAlarm("WAR0703", ErrPart);            //Steven 20180627 (wei) : 整合Low Yield Alarm
                            if(ret==K_ONECYCLE)
                            {
                                bLowYeildAlarm=true;
                            }
                            else
                            {
                                if(CosFunction.bYieldControlUseEACount)
                                    iYeildCT[0]=RunInfo.iUnloadCount;
                                else
                                    iFailAlarmSiteYieldIntervalCount=0;
                            }
                        }
                        bYieldDiffOver=false;
                    }
                }
            }
            else
            {
                if(CosFunction.bYieldControlUseEACount)
                    iYeildCT[0]=RunInfo.iUnloadCount;
                else
                    iFailAlarmSiteYieldIntervalCount=0;
            }

            // GATE (Y2) STAYS -- re-verified, NOT dissolved this wave, despite the
            // Y1 fContactCT sites above this one being real. `fShowBinSelect->
            // labArmDiff` (golden :4023) does not exist: forms/fShowBinSelect.h's
            // own ROLE banner (lines 16-18) LISTS `labArmDiff`/`labSiteDiff`/
            // `labLowYield`/`labTotalYield`/`labTotalYieldTotal`/`lblSpeciallYield`/
            // `lblSpeciallYieldTotal`/`IntervalByTotal` as landed, but the actual
            // TfShowBinSelect class body (same file, lines 302-382) declares NONE
            // of those eight -- confirmed this wave by attempting to compile this
            // exact site (`g++ -fsyntax-only`: "class TfShowBinSelect has no member
            // named labArmDiff") and by grepping the header/cpp for each of the
            // eight names (0 hits outside the ROLE banner's own prose). Only
            // `ShowCategoryBin()` and `iLowYieldBinSelectContactCount` are real --
            // this wave dissolves those. This site was ALSO absent from the (Y2)
            // GATE REGISTER's own enumerated function list (which never mentions
            // CheckBySiteYieldAlarm at all) even though its "14 sites total" count
            // already included it. forms/fShowBinSelect.h is out of this wave's
            // write boundary (only uYieldMonitoring.cpp/cContactCT.cpp/
            // cShowBinSelect.cpp/tests/test_yieldmon_core.cpp are) -- adding the
            // missing 8 widgets there is main-loop's call, flagged in hand-off.
#if 0
            if(CosFunction.bYieldControlUseEACount)
                fShowBinSelect->labArmDiff->Caption=RunInfo.iUnloadCount-iYeildCT[0];
#endif
        }
    }
    else
    {
        iCount2=0;
    }
}

// =============================================================================
//  CheckByPickerYieldAlarm -- golden :4033-4255 (根據Index吸嘴比較良率)
// =============================================================================
void TfYieldMonitoring::CheckByPickerYieldAlarm()                               //By Picker Compare Yield
{
    static int iCount2=0;

    int iSiteCount=0, ret=0, iNN=0;
    int iAlarmTimeInterval=(CosFunction.bLowYieldAlarmIntervalTimeBySetting &&  //JimmyChiu 20220601 : 修改low yield報警時間邏輯，報警間隔時間固定1min改為可以自行設定報警間隔時間
                            IniConfig.bI44_LowYieldAlarmIntervalTimeBySetting)?IniConfig.iI44_LowYieldAlarmIntervalTime:60;

    bool bYieldDiffOver=false, bNeedToCheck=false, bNeedToCloseSite=false;
    bool bArm1LowYield=false, bArm2LowYield=false;
    AnsiString ErrPart1="", ErrPart2="";
    AnsiString aLowYield;
    double dLowYield;

    dMaxPickerYield=0;

    if(bFirstCount)
    {
        iCount2=0;
        return;
    }

    if(SystemStart &&
       fContact->fShow==false &&
       iHome==0 &&
       bRunAutoClean==false &&
       LastSet.iRunStartMode!=rsmAutoSiteMap)                                   //Steven 20230313 : 做Auto Site Map的時候不要檢查Yield
    {
        iCount2++;
        if(bZ1PickShuttle || bZ2PickShuttle)                                    //Steven 20180521 : 避免吸料的時候發Low Yield Alarm  //JerryYang 20180629 (wei) 避免count被歸零
            return;
        if(iCount2>=iAlarmTimeInterval ||                                       //JimmyChiu 20220601 : 修改low yield報警時間邏輯，報警間隔時間固定1min改為可以自行設定報警間隔時間
           CosFunction.bYieldAlarmNoWait1Min)                                   //wei 20150820  Yield Alarm No Wait 1Min
        {
            iCount2=0;
        }
        else
        {
            return;
        }

        if(Prod.dLowYieldByPicker>0)
        {
            if(CosFunction.bYieldControlUseEACount)                             //wei 20180606 Yield控制使用EA Count    //Steven 20230223 : 簡化判斷式
            {
                bNeedToCheck=(RunInfo.iUnloadCount-iYeildCT[7]>=Prod.iLowYieldCountByPicker);
            }
            else
            {
                bNeedToCheck=(iPickerYieldIntervalCount>=Prod.iLowYieldCountByPicker);
            }
        }
        else
        {
            bNeedToCheck=false;
        }

        if(bNeedToCheck)
        {
            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(LastSet.bUseTestSocket[0][i][j])
                    {
                        if(dPickerYield[0][i][j]>dMaxPickerYield)
                            dMaxPickerYield=dPickerYield[0][i][j];
                        iSiteCount++;
                    }

                    if(LastSet.bUseTestSocket[1][i][j])
                    {
                        if(dPickerYield[1][i][j]>dMaxPickerYield)
                            dMaxPickerYield=dPickerYield[1][i][j];
                        iSiteCount++;
                    }
                }
            }
        }

        if(Prod.dLowYieldByPicker!=0 &&
           dMaxPickerYield>Prod.dLowYieldByPicker)
            dLowYield=dMaxPickerYield-Prod.dLowYieldByPicker;
        else
            dLowYield=-1;

        if(dLowYield>0 && bNeedToCheck)
        {
            iNN=IsNNMode();
            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(LastSet.bUseTestSocket[0][i][j])
                    {
                        if(dLowYield>dPickerYield[0][i][j])
                        {
                            iSiteCount--;
                            ErrPart1+=IndexSuckName[i+iNN][j];
                            bYieldDiffOver=true;
                            bArm1LowYield=true;
                        }
                    }

                    if(LastSet.bUseTestSocket[1][i][j])
                    {
                        if(dLowYield>dPickerYield[1][i][j])
                        {
                            iSiteCount--;
                            ErrPart2+=IndexSuckName[i][j];
                            bYieldDiffOver=true;
                            bArm2LowYield=true;
                        }
                    }
                }
            }
        }
        else
        {
            bNeedToCheck=false;
        }

        if(CosFunction.bLowYieldAutoSiteOff &&
           TestIF_File.bLowYieldAutoSiteOffByPicker &&                          //Sam 20221202 : 修正自動關 Site 關到剩餘設定 Site 數時需要報警。
           bYieldDiffOver==true)                                                //Steven 20230223 : by arm by site, auto site off
        {
            if(iSiteCount<2)
                bNeedToCloseSite=false;
            else if(iSiteCount>=TestIF_File.iAlarmWhenSiteOnCountLess)
                bNeedToCloseSite=true;
            else
                bNeedToCloseSite=false;
        }

        if(iSiteCount<2)                                                        //Steven 20200522 : 改成2, 只剩下一個site就不用比了
        {
            bYieldDiffOver=false;
        }

        if(bNeedToCheck &&                                                      //Steven 20230223 : 簡化判斷式
           CosFunction.bLowYieldAutoSiteOff &&                                  //Steven 20170905 (wei) : Low Yield Auto Site Off for Ambient
           TestIF_File.bLowYieldAutoSiteOffByPicker &&                          //Steven 20230223 : by arm by site, auto site off
           iSiteCount>=TestIF_File.iAlarmWhenSiteOnCountLess &&                 //Steven 20200522 : 改成數字比對, 避免參數混用
           bNeedToCloseSite==true)                                              //Sam 20221207 : LowYieldAutoSiteOff 新增 Alarm 幾次後再來關 Site
        {
            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(LastSet.bUseTestSocket[0][i][j])
                    {
                        if(dPickerYield[0][i][j]!=0 &&                          //Steven 20230313 : 沒有Yield的時候不比較
                           dLowYield>dPickerYield[0][i][j])
                        {
                            bLowYieldCloseSite[0][i][j]=true;
                        }
                    }

                    if(LastSet.bUseTestSocket[1][i][j])
                    {
                        if(dPickerYield[1][i][j]!=0 &&                          //Steven 20230313 : 沒有Yield的時候不比較
                           dLowYield>dPickerYield[1][i][j])
                        {
                            bLowYieldCloseSite[1][i][j]=true;
                        }
                    }
                }
            }
        }
        else
        {
            if(Prod.bLowYieldByPicker && Prod.iLowYieldCountByPicker!=0)
            {
                if(bNeedToCheck)                                                //Steven 20230223 : 簡化判斷式
                {
                    if(bYieldDiffOver==true)
                    {
                        if(bLowYeildAlarm==false)                               //wei 20151116 Yeild Alarm 只能Onecycle
                        {
                            if(bArm1LowYield)
                            {
                                ret=DoLowYieldAlarm("WAR0726", ErrPart1);       //Steven 20180627 (wei) : 整合Low Yield Alarm
                            }

                            if(bArm2LowYield)
                            {
                                ret=DoLowYieldAlarm("WAR0727", ErrPart2);       //Steven 20180627 (wei) : 整合Low Yield Alarm
                            }

                            if(ret==K_ONECYCLE)
                            {
                                bLowYeildAlarm=true;
                            }
                            else
                            {
                                if(CosFunction.bYieldControlUseEACount)
                                    iYeildCT[7]=RunInfo.iUnloadCount;
                                else
                                    iPickerYieldIntervalCount=0;
                            }
                        }
                        bYieldDiffOver=false;
                    }
                }
            }
            else
            {
                if(CosFunction.bYieldControlUseEACount)
                    iYeildCT[7]=RunInfo.iUnloadCount;
                else
                    iPickerYieldIntervalCount=0;
            }

            // GATE (Y2) STAYS -- `fShowBinSelect->labArmDiff` (golden :4247) does
            // not exist on the landed TfShowBinSelect (see CheckBySiteYieldAlarm's
            // sibling site above for the full re-verification note).
#if 0
            if(CosFunction.bYieldControlUseEACount)
                fShowBinSelect->labArmDiff->Caption=RunInfo.iUnloadCount-iYeildCT[7];
#endif
        }
    }
    else
    {
        iCount2=0;
    }
}

// =============================================================================
//  ClearAutoSiteOffStatus -- golden :4256-4267
// =============================================================================
void TfYieldMonitoring::ClearAutoSiteOffStatus()                                //Steven 20200409 : 修正清除count之後,不能開site的問題
{
    for(int i=0; i<MAX_Index_Row; i++)
    {
        for(int j=0; j<MAX_Index_Col; j++)
        {
            bLowYieldCloseSite[0][i][j]=false;                                  //Steven 20170905 (wei) : Low Yield Auto Site Off for Ambient
            bLowYieldCloseSite[1][i][j]=false;
        }
    }
}

// =============================================================================
//  CheckBySiteByArmYieldAlarm -- golden :4268-4521 (jou 2014-08-14 Site Compare Low Yield alarm)
// =============================================================================
void TfYieldMonitoring::CheckBySiteByArmYieldAlarm()
{
    static int iCount3=0;

    int iSiteCount=0, ret;
    int iAlarmTimeInterval=(CosFunction.bLowYieldAlarmIntervalTimeBySetting &&  //JimmyChiu 20220601 : 修改low yield報警時間邏輯，報警間隔時間固定1min改為可以自行設定報警間隔時間
                            IniConfig.bI44_LowYieldAlarmIntervalTimeBySetting)?IniConfig.iI44_LowYieldAlarmIntervalTime:60;

    bool bYieldCmpOver=false;
    double fYieldMax=0.0;
    AnsiString ErrPart="";

    if(bFirstCount)
    {
        iCount3=0;
        return;
    }

    if(SystemStart &&
       fContact->fShow==false &&
       iHome==0 &&
       bRunAutoClean==false &&
       LastSet.iRunStartMode!=rsmAutoSiteMap)                                   //Steven 20230313 : 做Auto Site Map的時候不要檢查Yield
    {
        if(CosFunction.bSiteCmpYield &&
           Prod.bFailAlarmSiteYieldCmp &&
           Prod.iFailAlarmSiteYieldCmpCount!=0)
        {
            iCount3++;
            if(bZ1PickShuttle || bZ2PickShuttle)                                //Steven 20180521 : 避免吸料的時候發Low Yield Alarm  //JerryYang 20180629 (wei) 避免count被歸零
                return;

            if(iCount3>=iAlarmTimeInterval ||                                   //JimmyChiu 20220601 : 修改low yield報警時間邏輯，報警間隔時間固定1min改為可以自行設定報警間隔時間
               CosFunction.bYieldAlarmNoWait1Min)                               // 1 mimutes  //wei 20150820  Yield Alarm No Wait 1Min
            {
                iCount3=0;
                if(bUseTwoArm32Site==true)                                      //Steven 20220419 : NN mode不需要比by arm
                {
                    for(int i=0; i<TestSocket.iShtRow; i++)
                    {
                        for(int j=0; j<TestSocket.iShtCol; j++)
                        {
                            if(LastSet.bUseTestSocket[0][i][j]==true)
                            {
                                if(fYieldMax<dSiteYield[i][j])
                                    fYieldMax=dSiteYield[i][j];
                            }
                        }
                    }

                    for(int i=0; i<TestSocket.iShtRow; i++)
                    {
                        for(int j=0; j<TestSocket.iShtCol; j++)
                        {
                            if(LastSet.bUseTestSocket[0][i][j])
                            {
                                iSiteCount++;
                                if(fabs(fYieldMax-dSiteYield[i][j])>Prod.dFailAlarmSiteYieldCmp)                         //JerryYang 20160530 LowYieldLimit要能設定到小數點
                                {
                                    if(CosFunction.bLowYieldAutoSiteOff &&      //Steven 20170905 (wei) : Low Yield Auto Site Off for Ambient
                                       TestIF_File.bLowYieldAutoSiteOff &&
                                       iRunStartMode==FT)
                                    {
                                        if(iFailAlarmSiteMaxYieldIntervalCount>=Prod.iFailAlarmSiteYieldCmpCount)
                                        {
                                            iSiteCount--;
                                            if(TestIF_File.bLowYieldAutoSiteOffAlarm)                                   //Sam 20221207 : LowYieldAutoSiteOff 新增 Alarm 幾次後再來關 Site
                                            {
                                                iAlarmSiteYieldCmpCnt[i][j]++;
                                                if(iAlarmSiteYieldCmpCnt[i][j]<=TestIF_File.iLowYieldAutoSiteOffAlarm)
                                                {
                                                    ErrPart+=IndexSuckName[i][j];
                                                }
                                            }
                                            bYieldCmpOver=true;                 //Sam 20250410 : 修正auto site off 關site低於設定值時不會alarm的問題
                                        }
                                    }
                                    else
                                    {
                                        bYieldCmpOver=true;
                                    }
                                    ErrPart+=IndexSuckName[i][j];
                                }
                                else
                                {
                                    if(iFailAlarmSiteMaxYieldIntervalCount>=Prod.iFailAlarmSiteYieldCmpCount)           //Sam 20221207 : LowYieldAutoSiteOff 新增 Alarm 幾次後再來關 Site
                                        iAlarmSiteYieldCmpCnt[i][j]=0;
                                }
                            }
                        }
                    }
                }
                else
                {
                    for(int i=0; i<TestSocket.iShtRow; i++)
                    {
                        for(int j=0; j<TestSocket.iShtCol; j++)
                        {
                            if(LastSet.bUseTestSocket[0][i][j] ||               //Isaac 20210630 : 修正關arm會失效，&&->||
                               LastSet.bUseTestSocket[1][i][j])
                            {
                                if(fYieldMax<dSiteYield[i][j])
                                    fYieldMax=dSiteYield[i][j];
                            }
                        }
                    }

                    for(int i=0; i<FTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<FTestSuck.iShtCol; j++)
                        {
                            if(LastSet.bUseTestSocket[0][i][j] ||               //Isaac 20210630 : 修正關arm會失效，&&->||
                               LastSet.bUseTestSocket[1][i][j])
                            {
                                iSiteCount++;
                                if(fabs(fYieldMax-dSiteYield[i][j])>Prod.dFailAlarmSiteYieldCmp)                         //JerryYang 20160530 LowYieldLimit要能設定到小數點
                                {
                                    if(CosFunction.bLowYieldAutoSiteOff &&      //Steven 20170905 (wei) : Low Yield Auto Site Off for Ambient
                                       TestIF_File.bLowYieldAutoSiteOff &&
                                       iRunStartMode==FT)
                                    {
                                        if(iFailAlarmSiteMaxYieldIntervalCount>=Prod.iFailAlarmSiteYieldCmpCount)
                                        {
                                            iSiteCount--;
                                            if(TestIF_File.bLowYieldAutoSiteOffAlarm)                                   //Sam 20221207 : LowYieldAutoSiteOff 新增 Alarm 幾次後再來關 Site
                                            {
                                                iAlarmSiteYieldCmpCnt[i][j]++;
                                                if(iAlarmSiteYieldCmpCnt[i][j]<=TestIF_File.iLowYieldAutoSiteOffAlarm)
                                                {
                                                    ErrPart+=IndexSuckName[i][j];
                                                }
                                            }
                                        }
                                    }

                                    bYieldCmpOver=true;                         //jou 20221102 : 修正auto site off 關site低於設定值時不會alarm的問題
                                    ErrPart+=IndexSuckName[i][j];
                                }
                                else
                                {
                                    if(iFailAlarmSiteMaxYieldIntervalCount>=Prod.iFailAlarmSiteYieldCmpCount)           //Sam 20221207 : LowYieldAutoSiteOff 新增 Alarm 幾次後再來關 Site
                                        iAlarmSiteYieldCmpCnt[i][j]=0;
                                }
                            }
                        }
                    }
                }

                if(CosFunction.bLowYieldAutoSiteOff &&                          //Steven 20170905 (wei) : Low Yield Auto Site Off for Ambient
                   TestIF_File.bLowYieldAutoSiteOff &&
                   iRunStartMode==FT &&
                   iSiteCount>=TestIF_File.iAlarmWhenSiteOnCountLess)           //Steven 20200522 : 改成數字比對, 避免參數混用
                {
                    if(iFailAlarmSiteMaxYieldIntervalCount>=Prod.iFailAlarmSiteYieldCmpCount)
                    {
                        if(bUseTwoArm32Site==true)                              //Steven 20220419 : NN mode不需要比by arm
                        {
                            for(int i=0; i<TestSocket.iShtRow; i++)
                            {
                                for(int j=0; j<TestSocket.iShtCol; j++)
                                {
                                    if(LastSet.bUseTestSocket[0][i][j]==true)
                                    {
                                        if(fabs(fYieldMax-dSiteYield[i][j])>Prod.dFailAlarmSiteYieldCmp)                 //JerryYang 20160530 LowYieldLimit要能設定到小數點
                                        {
                                            bLowYieldCloseSite[0][i][j]=true;
                                            if(iAlarmSiteYieldCmpCnt[i][j]>TestIF_File.iLowYieldAutoSiteOffAlarm)       //Sam 20221207 : LowYieldAutoSiteOff 新增 Alarm 幾次後再來關 Site
                                                iAlarmSiteYieldCmpCnt[i][j]=0;
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            for(int i=0; i<TestSocket.iShtRow; i++)
                            {
                                for(int j=0; j<TestSocket.iShtCol; j++)
                                {
                                    if(LastSet.bUseTestSocket[0][i][j] ||
                                       LastSet.bUseTestSocket[1][i][j])         //Isaac 20210630 : 修正關arm會失效，&&->||
                                    {
                                        if(fabs(fYieldMax-dSiteYield[i][j])>Prod.dFailAlarmSiteYieldCmp)                 //JerryYang 20160530 LowYieldLimit要能設定到小數點
                                        {
                                            bLowYieldCloseSite[0][i][j]=true;
                                            if(iAlarmSiteYieldCmpCnt[i][j]>TestIF_File.iLowYieldAutoSiteOffAlarm)       //Sam 20221207 : LowYieldAutoSiteOff 新增 Alarm 幾次後再來關 Site
                                                iAlarmSiteYieldCmpCnt[i][j]=0;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                else if(CosFunction.bYieldControlUseEACount)                    //wei 20151111  //wei 20180606 Yield控制使用EA Count
                {
                    if(RunInfo.iUnloadCount-iYeildCT[2]>=Prod.iFailAlarmSiteYieldCmpCount)
                    {
                        if(bYieldCmpOver==true)
                        {
                            if(bLowYeildAlarm==false)                           //wei 20151116 Yeild Alarm 只能Onecycle
                            {
                                ret=DoLowYieldAlarm("WAR0702", ErrPart);        //Steven 20180627 (wei) : 整合Low Yield Alarm
                                if(ret==K_ONECYCLE)
                                {
                                    bLowYeildAlarm=true;
                                }
                                else
                                {
                                    iYeildCT[2]=RunInfo.iUnloadCount;
                                }
                            }
                            bYieldCmpOver=false;
                        }
                    }
                }
                else
                {
                    if(iFailAlarmSiteMaxYieldIntervalCount>=Prod.iFailAlarmSiteYieldCmpCount)
                    {
                        if(bYieldCmpOver==true)
                        {
                            iFailAlarmSiteMaxYieldIntervalCount=0;              //JerryYang 20170406 (Steven) 觸發yield alarm後才清掉
                            ClearYieldCount();                                  //Steven 20140830 : Yield相關的Alarm, 要清掉全部的Ignore的Count重算
                            bYieldCmpOver=false;
                            ret=DoLowYieldAlarm("WAR0702", ErrPart);            //Steven 20180627 (wei) : 整合Low Yield Alarm
                        }
                    }
                }
            }
        }
        else
        {
            if(CosFunction.bYieldControlUseEACount)                             //wei 20151111  //wei 20180606 Yield控制使用EA Count
            {
                iYeildCT[2]=RunInfo.iUnloadCount;
            }
            else
            {
                iFailAlarmSiteMaxYieldIntervalCount=0;
            }
        }

        // GATE (Y2) STAYS -- `fShowBinSelect->labSiteDiff` (golden :4511/:4517)
        // does not exist on the landed TfShowBinSelect (see CheckBySiteYieldAlarm's
        // own labArmDiff site for the full re-verification note).
#if 0
        if(CosFunction.bYieldControlUseEACount)                                 //Steven 20170605 (wei) : 修正畫面顯示 //wei 20151111  //wei 20180606 Yield控制使用EA Count
        {
            fShowBinSelect->labSiteDiff->Caption=RunInfo.iUnloadCount-iYeildCT[2];
        }
        else
        {
            fShowBinSelect->labSiteDiff->Caption=iFailAlarmSiteMaxYieldIntervalCount;
        }
#endif
    }
}

// =============================================================================
//  CheckLowYieldAlarm -- golden :4522-4956
// =============================================================================
void TfYieldMonitoring::CheckLowYieldAlarm()
{
    static int iCount2=0;

    int sum=0, ipass=0, ret=0;
    int iAlarmTimeInterval=(CosFunction.bLowYieldAlarmIntervalTimeBySetting &&  //JimmyChiu 20220601 : 修改low yield報警時間邏輯，報警間隔時間固定1min改為可以自行設定報警間隔時間
                            IniConfig.bI44_LowYieldAlarmIntervalTimeBySetting)?IniConfig.iI44_LowYieldAlarmIntervalTime:60;

    bool bIsLowYield=false;
    double dYield1=0.0;                                                         //JerryYang 20160530 iYield改成dYield
    AnsiString aLowYield="";
    AnsiString SocketErrPart="", s="";
    (void)s;   // golden itself never reads `s` again after declaring it here -- verified end to end this wave

    if(bFirstCount)
    {
        iCount2=0;
        return;
    }

    if(SystemStart && fContact->fShow==false &&
       iHome==0 && bRunAutoClean==false &&
       LastSet.iRunStartMode!=rsmAutoSiteMap)                                   //Steven 20230313 : 做Auto Site Map的時候不要檢查Yield
    {
        if((Prod.bFailAlarmLowYield && Prod.dLowYieldLimit!=0) ||               //JerryYang 20160530 LowYieldLimit要能設定到小數點
           (IniConfig.bE53LowYieldAutoClean && TestIF.iAutoClean_Function==true &&
            TestIF.bAutoClean_FailAlarmLowYield && TestIF.iAutoClean_LowYieldLimit!=0) ||                               //Steven 20220110 : 修正沒開Auto clean跟low yield卻會alarm
           (IniConfig.bE53LowYieldAutoClean && TestIF.iAutoClean_Function==true &&
            Prod.bFailAlarmLowYield_AutoClean && Prod.iLowYieldLimit_AutoClean!=0))
        {
            iCount2++;                                                          //JerryYang 20180629 (wei) 避免count被歸零
            if(bZ1PickShuttle || bZ2PickShuttle)                                //Steven 20180521 : 避免吸料的時候發Low Yield Alarm
                return;

            if(iCount2>=iAlarmTimeInterval ||                                   // 1 mimutes    //JimmyChiu 20220601 : 修改low yield報警時間邏輯，報警間隔時間固定1min改為可以自行設定報警間隔時間
               CosFunction.bYieldAlarmNoWait1Min)                               //wei 20150820  Yield Alarm No Wait 1Min
            {
                iCount2=0;

                //=== Sliding Window Low Yield Check ===                        //Steven 20260331
                if(Prod.bSlidingWindowYield && Prod.iSlidingWindowSize>0 &&
                   Prod.bFailAlarmLowYield   && Prod.dLowYieldLimit!=0)
                {
                    CheckLowYieldAlarm_SW();
                    return;
                }
                //=== End Sliding Window ===                                     //Steven 20260331

                if(IniConfig.bEnableAutoCleanFunction &&                        //ChungHung 20131223 add for SCK start
                   TestIF.iAutoClean_Function==true &&
                   (TestIF_File.iAutoClean_Mode & M_SOCKET_ALARM) &&
                   TestIF.bAutoClean_FailAlarmLowYield)
                {
                    sum=ArmData_AutoClean[0]->GetTotalCT()+ArmData_AutoClean[1]->GetTotalCT();                          //Sam 20230104 : 修正 LowYield AutoClean
                    // AI(W906-FW-YEnable) 20260818: gate dissolved -- fContactCT/fShowBinSelect landed (ecf6154)
                    if(sum>0)
                    {
                        dYield1=fContactCT->GetLowYield_AutoClean(0);
                        if(sum>=TestIF.iAutoClean_LowYieldCount &&  TestIF.iAutoClean_LowYieldLimit>dYield1)
                        {
                            aLowYield="AutoClean : Low Yield Alarm : " + AnsiString(dYield1) + "%" + " Contact Count : " + iAutoClean_IndexContactCount;
                            RecordProcess(aLowYield);
                            InitialAutoCleanAllTask();                          //Sam 20230504 : 整理 InitialAutoCleanTask
                        }
                    }
                }

                if(IniConfig.bLowYieldAlarmSameNS==true &&                      //jou    2011-07-16 : Low Yield Alarm模式與NS機台相同,skip會清除單獨Site.
                   Prod.bFailAlarmLowYield && Prod.dLowYieldLimit!=0)           //Steven 20220110 : 修正沒開Auto clean跟low yield卻會alarm
                {
                    if(CosFunction.bYieldControlUseEACount)                     //wei 20151111  //wei 20180606 Yield控制使用EA Count
                    {
                        if(Prod.bFailAlarmLowYield && Prod.dLowYieldLimit!=0)   //JerryYang 20160530 LowYieldLimit要能設定到小數點
                        {
                            if(RunInfo.iUnloadCount-iYeildCT[1]>=Prod.iLowYieldCount)
                            {
                                // AI(W906-FW-YEnable) 20260818: gate dissolved -- fContactCT/fShowBinSelect landed (ecf6154)
                                if(bUseTwoArm32Site==true)                      //Steven 20220418 : NN mode Yield alarm
                                {
                                    for(int i=0; i<TestSocket.iShtRow; i++)
                                    {
                                        for(int j=0; j<TestSocket.iShtCol; j++)
                                        {
                                            if(LastSet.bUseTestSocket[0][i][j])
                                            {
                                                dYield1=fContactCT->ReturnSiteDataArray(false, i, j);
                                                if(Prod.dLowYieldLimit>dYield1)
                                                {
                                                    bIsLowYield=true;
                                                    SocketErrPart+=IndexSuckName[i][j];
                                                }
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    for(int i=0; i<TestSocket.iShtRow; i++)
                                    {
                                        for(int j=0; j<TestSocket.iShtCol; j++)
                                        {
                                            if(LastSet.bUseTestSocket[0][i][j] ||                                       //Steven 20230831 : 關Arm不會Alarm && --> ||
                                               LastSet.bUseTestSocket[1][i][j])                                         //Frank 20160802 add Low Yield 關Site
                                            {
                                                dYield1=fContactCT->ReturnSiteDataArray(false, i, j);
                                                if(Prod.dLowYieldLimit>dYield1)                                         //JerryYang 20160530 LowYieldLimit要能設定到小數點
                                                {
                                                    bIsLowYield=true;
                                                    SocketErrPart+=IndexSuckName[i][j];
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            iYeildCT[1]=RunInfo.iUnloadCount;
                        }
                        // GATE (Y2) STAYS -- `fShowBinSelect->labLowYield` (golden
                        // :4639) does not exist on the landed TfShowBinSelect (see
                        // CheckBySiteYieldAlarm's own labArmDiff site for the full
                        // re-verification note).
#if 0
                        fShowBinSelect->labLowYield->Caption=RunInfo.iUnloadCount-iYeildCT[1];
#endif
                    }
                    else
                    {
                        // AI(W906-FW-YEnable) 20260818: gate dissolved -- fContactCT/fShowBinSelect landed (ecf6154)
                        if(Prod.bFailAlarmLowYield && Prod.dLowYieldLimit!=0)   //JerryYang 20160530 LowYieldLimit要能設定到小數點
                        {
                            if(bUseTwoArm32Site==true)                          //Steven 20220418 : NN mode Yield alarm
                            {
                                for(int i=0; i<TestSocket.iShtRow; i++)
                                {
                                    for(int j=0; j<TestSocket.iShtCol; j++)
                                    {
                                        if(LastSet.bUseTestSocket[0][i][j])
                                        {
                                            sum=fContactCT->ReturnSiteDataArray(true, i, j);
                                            if(sum>=Prod.iLowYieldCount)
                                            {
                                                dYield1=fContactCT->ReturnSiteDataArray(false, i, j);
                                                if(Prod.dLowYieldLimit>dYield1)                                         //JerryYang 20160530 LowYieldLimit要能設定到小數點
                                                {
                                                    bIsLowYield=true;
                                                    SocketErrPart+=IndexSuckName[i][j];
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            else
                            {
                                for(int i=0; i<TestSocket.iShtRow; i++)
                                {
                                    for(int j=0; j<TestSocket.iShtCol; j++)
                                    {
                                        if(LastSet.bUseTestSocket[0][i][j] ||   //Steven 20230831 : 關Arm不會Alarm
                                           LastSet.bUseTestSocket[1][i][j])     //Frank 20160802 add Low Yield 關Site
                                        {
                                            sum=fContactCT->ReturnSiteDataArray(true, i, j);
                                            if(sum>=Prod.iLowYieldCount)
                                            {
                                                dYield1=fContactCT->ReturnSiteDataArray(false, i, j);
                                                if(Prod.dLowYieldLimit>dYield1)                                         //JerryYang 20160530 LowYieldLimit要能設定到小數點
                                                {
                                                    bIsLowYield=true;
                                                    SocketErrPart+=IndexSuckName[i][j];
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    if(bIsLowYield==true)
                    {
                        if(CosFunction.bYieldControlUseEACount)                 //wei 20151111  //wei 20180606 Yield控制使用EA Count
                        {
                            if(bLowYeildAlarm==false)                           //wei 20151116 Yeild Alarm 只能Onecycle
                            {
                                ret=DoLowYieldAlarm("WAR0701", SocketErrPart);  //Steven 20180627 (wei) : 整合Low Yield Alarm
                                if(ret==K_ONECYCLE)
                                {
                                    bLowYeildAlarm=true;
                                }
                                else
                                {
                                    iYeildCT[1]=RunInfo.iUnloadCount;
                                }
                            }
                            bIsLowYield=false;
                        }
                        else
                        {
                            DoLowYieldAlarm("WAR0701", SocketErrPart);          //Steven 20180627 (wei) : 整合Low Yield Alarm
                            ClearYieldCount();                                  //Steven 20140830 : Yield相關的Alarm, 要清掉全部的Ignore的Count重算

                            if(ret==K_ONECYCLE ||
                               CUSTOMER_CODE==CC_HANA_MICRON)                   //Steven 20210428 : Hana說Low Yield不要清除資料
                            {
                                bIsLowYield=false;
                            }
                            else
                            {
                                // AI(W906-FW-YEnable) 20260818: gate dissolved -- fContactCT/fShowBinSelect landed (ecf6154)
                                for(int i=0; i<TestSocket.iShtRow; i++)
                                {
                                    for(int j=0; j<TestSocket.iShtCol; j++)
                                    {
                                        dYield1=fContactCT->ReturnSiteDataArray(false, i, j);
                                        if(Prod.dLowYieldLimit>dYield1)
                                        {
                                            fContactCT->ClearData(i, j);        //Steven 20220301 : 修正Clear Yield
                                        }
                                    }
                                }
                                bIsLowYield=false;
                            }
                        }

                        // AI(W906-FW-YEnable) 20260818: gate dissolved -- fContactCT/fShowBinSelect landed (ecf6154)
                        for(int i=0; i<TestSocket.iShtRow; i++)
                        {
                            for(int j=0; j<TestSocket.iShtCol; j++)
                            {
                                dYield1=fContactCT->ReturnSiteDataArray(false, i, j);
                                if(Prod.dLowYieldLimit>dYield1 && (CUSTOMER_CODE!=CC_KYEC_LEE && CUSTOMER_CODE!=CC_KYEC_XILINX))
                                {
                                    bShowSiteYield[i*TestSocket.iShtRow+j]=true;
                                }
                                else
                                {
                                    bShowSiteYield[i*TestSocket.iShtRow+j]=false;
                                }
                            }
                        }
                    }

                    //-------------------
                    if(Prod.bLowYieldAlarmByBin)                                //Steven 20140828 : By Bin Yield Monitor
                    {
                        sum=0;
                        ipass=0;

                        for(int i=0; i<6; i++)
                        {
                            sum+=LastSet.BinCT[0][i];
                        }

                        for(int i=0; i<iTestBinCount; i++)
                        {
                            if(Prod.bLowYield[i]==true)
                                ipass+=LastSet.iBinData32[0][i];
                        }

                        if(sum>0)
                        {
                            dYield1=ChangeToFloat((double)ipass, (double)sum);
                            if(CosFunction.bYieldControlUseContactCount)        //Steven 20141212 : Yield控制使用Contact Count
                            {
                                if(iLowYieldContactCount>=Prod.iLowYieldCount &&
                                   Prod.dLowYieldLimit>dYield1)                 //JerryYang 20160530 LowYieldLimit要能設定到小數點
                                {
                                    iLowYieldContactCount=0;
                                    ClearYieldCount();                          //Steven 20140830 : Yield相關的Alarm, 要清掉全部的Ignore的Count重算
                                    DoLowYieldAlarm("WAR0701", SocketErrPart);  //Steven 20180627 (wei) : 整合Low Yield Alarm
                                }
                            }
                            else
                            {
                                if(sum>=Prod.iLowYieldCount &&
                                   Prod.dLowYieldLimit>dYield1)                 //JerryYang 20160530 LowYieldLimit要能設定到小數點
                                {
                                    ClearYieldCount();                          //Steven 20140830 : Yield相關的Alarm, 要清掉全部的Ignore的Count重算
                                    DoLowYieldAlarm("WAR0701", SocketErrPart);  //Steven 20180627 (wei) : 整合Low Yield Alarm
                                }
                            }
                        }
                    }
                }
                else if(IniConfig.bE53LowYieldAutoClean)                        //wei 20141201 Low Yield Auto Clean(%) start
                {
                    // AI(W906-FW-YEnable) 20260818: gate dissolved -- fContactCT/fShowBinSelect landed (ecf6154)
                    if(Prod.bFailAlarmLowYield_AutoClean && Prod.iLowYieldLimit_AutoClean!=0)
                    {
                        if(bUseTwoArm32Site==true)                              //Steven 20220418 : NN mode Yield alarm
                        {
                            for(int i=0; i<TestSocket.iShtRow; i++)
                            {
                                for(int j=0; j<TestSocket.iShtCol; j++)
                                {
                                    if(LastSet.bUseTestSocket[0][i][j])
                                    {
                                        sum=fContactCT->ReturnSiteDataArray(true, i, j);
                                        if(sum>=Prod.iLowYieldCount_AutoClean)
                                        {
                                            dYield1=fContactCT->ReturnSiteDataArray(false, i, j);
                                            if(Prod.iLowYieldLimit_AutoClean>dYield1)
                                            {
                                                bIsLowYield=true;
                                                SocketErrPart+=IndexSuckName[i][j]+ ":" + dYield1 + "%,";
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            for(int i=0; i<TestSocket.iShtRow; i++)
                            {
                                for(int j=0; j<TestSocket.iShtCol; j++)
                                {
                                    if(LastSet.bUseTestSocket[0][i][j] ||
                                       LastSet.bUseTestSocket[1][i][j])
                                    {
                                        sum=fContactCT->ReturnSiteDataArray(true, i, j);
                                        if(sum>=Prod.iLowYieldCount_AutoClean)
                                        {
                                            dYield1=fContactCT->ReturnSiteDataArray(false, i, j);
                                            if(Prod.iLowYieldLimit_AutoClean>dYield1)
                                            {
                                                bIsLowYield=true;
                                                SocketErrPart+=IndexSuckName[i][j]+ ":" + dYield1 + "%,";
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    if(bIsLowYield==true)
                    {
                        aLowYield="AutoClean : Low Yield Site : " + AnsiString(SocketErrPart);
                        RecordProcess(aLowYield);

                        if(CUSTOMER_CODE!=CC_HANA_MICRON)                       //Steven 20210428 : Hana說Low Yield不要清除資料
                        {
                            // AI(W906-FW-YEnable) 20260818: gate dissolved -- fContactCT/fShowBinSelect landed (ecf6154)
                            for(int i=0; i<TestSocket.iShtRow; i++)
                            {
                                for(int j=0; j<TestSocket.iShtCol; j++)
                                {
                                    dYield1=atof(fContactCT->ReturnSiteData(2, i*TestSocket.iShtCol+j).c_str());
                                    if(Prod.iLowYieldLimit_AutoClean>dYield1)
                                    {
                                        fContactCT->ClearData(i, j);            //Steven 20220301 : 修正Clear Yield
                                    }
                                }
                            }
                        }

                        if(IniConfig.bEnableAutoCleanFunction &&
                           TestIF.iAutoClean_Function==true &&
                           Prod.bFailAlarmLowYield_AutoClean)
                        {
                            InitialAutoCleanAllTask();                          //Sam 20230504 : 整理 InitialAutoCleanTask
                        }
                    }
                }                                                               //wei 20141201 Low Yield Auto Clean(%) end
                else if(Prod.bFailAlarmLowYield && Prod.dLowYieldLimit!=0)      //Steven 20220110 : 修正沒開Auto clean跟low yield卻會alarm
                {
                    sum=0;
                    ipass=0;

                    if(CosFunction.bLowYieldUseContactCounts)                   //Sam 20221020 : LowYield 改使用 ContactCounts 的資料來計算
                    {
                        sum=ArmData[0]->GetTotalCT()+ArmData[1]->GetTotalCT();
                        ipass=ArmData[0]->GetPassCT()+ArmData[1]->GetPassCT();
                    }
                    else if(Prod.bLowYieldAlarmByBin)                           //Steven 20140828 : By Bin Yield Monitor
                    {
                        for(int i=0; i<eTrayCount; i++)
                        {
                            if(Prod.iTrayType[i]==tNotUse)
                                continue;
                            sum+=LastSet.BinCT[0][iTo3Unload[i]];
                        }

                        for(int i=0; i<iTestBinCount; i++)
                        {
                            if(Prod.bLowYield[i]==true)
                                ipass+=LastSet.iBinData32[0][i];
                        }
                    }
                    else
                    {
                        for(int i=0; i<eTrayCount; i++)                         //JerryYang 20230925
                        {
                            if(Prod.iTrayType[i]==tNotUse)
                                continue;
                            sum+=LastSet.BinCT[0][iTo3Unload[i]];
                            if(Prod.iIsPassT6[i]==1)                            //Steven 20240105 : Prod.bIsPass --> Prod.iIsPassT6
                                ipass+=LastSet.BinCT[0][iTo3Unload[i]];
                        }
                    }

                    if(sum>0)
                    {
                        dYield1=ChangeToFloat((double)ipass, (double)sum);      //JerryYang 20160530 LowYieldLimit要能設定到小數點

                        if(CosFunction.bYieldControlUseContactCount)            //Steven 20141212 : Yield控制使用Contact Count
                        {
                            if(iLowYieldContactCount>=Prod.iLowYieldCount &&
                               Prod.dLowYieldLimit>dYield1)                     //JerryYang 20160530 LowYieldLimit要能設定到小數點
                            {
                                iLowYieldContactCount=0;
                                ClearYieldCount();                              //Steven 20140830 : Yield相關的Alarm, 要清掉全部的Ignore的Count重算
                                DoLowYieldAlarm("WAR0701", SocketErrPart);      //Steven 20180627 (wei) : 整合Low Yield Alarm
                            }
                        }
                        else
                        {
                            if(CUSTOMER_CODE==CC_JCET)                          //JerryYang 20170418 (Steven) JCET吳如春要求用Ignore count
                                sum=RunInfo.iUnloadCount-iYeildCT[1];

                            if(sum>=Prod.iLowYieldCount &&
                               Prod.dLowYieldLimit>dYield1)                     //JerryYang 20160530 LowYieldLimit要能設定到小數點
                            {
                                ClearYieldCount();                              //Steven 20140830 : Yield相關的Alarm, 要清掉全部的Ignore的Count重算
                                DoLowYieldAlarm("WAR0701", SocketErrPart);      //Steven 20180627 (wei) : 整合Low Yield Alarm
                            }
                        }
                    }
                }
            }
        }
        else
        {
            // GATE (Y2) STAYS -- `fShowBinSelect->labLowYield` (golden :4946)
            // does not exist on the landed TfShowBinSelect (see
            // CheckBySiteYieldAlarm's own labArmDiff site for the full
            // re-verification note).
#if 0
            if(Prod.bFailAlarmLowYield==false)
                fShowBinSelect->labLowYield->Caption=0;
#endif
        }

        // AI(W906-FW-YEnable) 20260818: gate dissolved -- fContactCT/fShowBinSelect landed (ecf6154).
        // SaveTotalYield write-path re-verified this wave by reading cContactCT.cpp's actual
        // body (not just forms/fContactCT.h's WRITE-PATH NOTE prose): it early-returns unless
        // `IniConfig.bI29YieldRecordIntervalIC` is true (cContactCT.cpp:1589-1590 -- the
        // header's own WRITE-PATH NOTE also names a SECOND flag, bI29EnableYieldRecord, that
        // the actual code does not check; only bI29YieldRecordIntervalIC gates it), then
        // appends a CSV under `asYieldRecordPath` (common.cpp:136, defaults to
        // "D:\HT9045_Log\Yield" -- a distinct log tree, NOT the shared production
        // system\Gerneral.ini/teach-data path this repo's known-risk list warns about). Left
        // ACTIVE (not re-gated) per this task's file-write disposition rule -- default-off
        // ini flag, writes outside the shared machine-config tree.
        fContactCT->SaveTotalYield("");                                         //Sam 20231106 : 紀錄 Total yield
    }
    else
    {
        iCount2=0;
    }
}

// =============================================================================
//  CheckLowYieldAlarmByTotal -- golden :4957-5102 (wei 20151116 Low Yield By Total)
// =============================================================================
void TfYieldMonitoring::CheckLowYieldAlarmByTotal()
{
    static int iCount4=0;
    static bool bReflash=false;

    int sumByTotal=0, ipassByTotal=0, ret;
    int iAlarmTimeInterval=(CosFunction.bLowYieldAlarmIntervalTimeBySetting &&  //JimmyChiu 20220601 : 修改low yield報警時間邏輯，報警間隔時間固定1min改為可以自行設定報警間隔時間
                            IniConfig.bI44_LowYieldAlarmIntervalTimeBySetting)?IniConfig.iI44_LowYieldAlarmIntervalTime:60;

    bool bAlarm=false;
    double iYieldByTotal=0.0;
    AnsiString str="";
    AnsiString SocketErrPart="";

    if(bFirstCount)
    {
        iCount4=0;
        return;
    }

    if(SystemStart==false)                                                      //Ifor 20171017 add 強制更新一次避免資料不符
        bReflash=true;

    if((SystemStart || bReflash) &&
       fContact->fShow==false &&
       iHome==0 &&
       bRunAutoClean==false &&
       LastSet.iRunStartMode!=rsmAutoSiteMap)                                   //Steven 20230313 : 做Auto Site Map的時候不要檢查Yield
    {
        bReflash=false;
        // AI(W906-FW-YEnable) 20260818: gate dissolved -- fContactCT/fShowBinSelect landed (ecf6154)
        fShowBinSelect->ShowCategoryBin();
        if(Prod.bFailAlarmLowYieldByTotal &&
           Prod.dLowYieldLimitByTotal!=0 &&                                     //JerryYang 20160530 LowYieldLimit要能設定到小數點
           CosFunction.bLowYeildByTotal)
        {
            iCount4++;
            if(bZ1PickShuttle || bZ2PickShuttle)                                //Steven 20180521 : 避免吸料的時候發Low Yield Alarm  //JerryYang 20180629 (wei) 避免count被歸零
                return;

            if(iCount4>=iAlarmTimeInterval ||                                   //JimmyChiu 20220601 : 修改low yield報警時間邏輯，報警間隔時間固定1min改為可以自行設定報警間隔時間
               CosFunction.bYieldAlarmNoWait1Min)                               //wei 20150820  Yield Alarm No Wait 1Min
            {
                iCount4=0;
                sumByTotal=0;
                ipassByTotal=0;

                for(int i=0; i<eTrayCount; i++)                                 //JerryYang 20230925
                {
                    if(Prod.iTrayType[i]==tNotUse)
                        continue;
                    sumByTotal+=LastSet.BinCT[0][iTo3Unload[i]];                //JerryYang 202240515 : 改成To3陣列            //Steven 20240109 : eTrayCount --> i
                    if(Prod.iIsPassT6[i]==1)                                    //Steven 20240105 : Prod.bIsPass --> Prod.iIsPassT6
                        ipassByTotal+=LastSet.BinCT[0][iTo3Unload[i]];
                }

                if(CosFunction.bYieldControlUseEACount ||                       //Kaichen 20190628 : Low Yield ByTotal 控制使用 Contact Count
                   CUSTOMER_CODE==CC_JCET)                                      //Steven 20231212 : JCET這一項使用的是EA Count
                {
                    if(sumByTotal>0)
                    {
                        iYieldByTotal=ChangeToFloat(double(ipassByTotal), double(sumByTotal));
                        bAlarm=false;                                           //JerryYang 20180302 fix Yield alarm by total失效
                        if(Prod.dLowYieldLimitByTotal>iYieldByTotal)
                        {
                            if(CUSTOMER_CODE==CC_KYEC_LEE)
                            {
                                if(sumByTotal>=Prod.iLowYieldCountByTotal)
                                {
                                    bAlarm=true;
                                }
                            }
                            else
                            {
                                if(sumByTotal-iYeildCT[4]>=Prod.iLowYieldCountByTotal)
                                {
                                    bAlarm=true;
                                }
                            }
                        }

                        if(bAlarm)                                              //JerryYang 20180302 fix Yield alarm by total失效
                        {
                            if(bLowYeildAlarm==false)
                            {
                                ret=DoLowYieldAlarm("WAR0705", SocketErrPart);  //Steven 20180627 (wei) : 整合Low Yield Alarm
                                if(ret==K_ONECYCLE)
                                {
                                    bLowYeildAlarm=true;
                                }
                                else
                                {
                                    iYeildCT[4]=sumByTotal;
                                }
                            }
                        }
                    }
                }
                else
                {
                    if(iLowYieldByTotalContactCount>0)
                    {
                        iYieldByTotal=ChangeToFloat(double(ipassByTotal), double(sumByTotal));
                        bAlarm=false;                                           //JerryYang 20180302 fix Yield alarm by total失效
                        if(Prod.dLowYieldLimitByTotal>iYieldByTotal)
                        {
                            if(iLowYieldByTotalContactCount>=Prod.iLowYieldCountByTotal)
                            {
                                iLowYieldByTotalContactCount=0;
                                bAlarm=true;
                            }
                        }

                        if(bAlarm)                                              //JerryYang 20180302 fix Yield alarm by total失效
                        {
                            if(bLowYeildAlarm==false)
                            {
                                ret=DoLowYieldAlarm("WAR0705", SocketErrPart);  //Steven 20180627 (wei) : 整合Low Yield Alarm
                                if(ret==K_ONECYCLE)
                                {
                                    bLowYeildAlarm=true;
                                }
                            }
                        }
                    }
                }

                // GATE (Y2) STAYS -- `fShowBinSelect->labTotalYield`/
                // `labTotalYieldTotal` (golden :5084/:5087/:5089) do not exist on
                // the landed TfShowBinSelect (see CheckBySiteYieldAlarm's own
                // labArmDiff site for the full re-verification note). `str` is
                // computed only to feed the gated Caption write, so it is gated
                // alongside (same "gate made a real local go quiet" posture as
                // this function's own pre-existing precedent) -- `(void)str;`
                // restored.
#if 0
                str.sprintf("%0.2f", iYieldByTotal);
                fShowBinSelect->labTotalYield->Caption=str.c_str();
                if(CosFunction.bYieldControlUseEACount ||                       //Kaichen 20190628 : Low Yield ByTotal 控制使用 Contact Count
                   CUSTOMER_CODE==CC_JCET)                                      //Steven 20231212 : JCET這一項使用的是EA Count
                    fShowBinSelect->labTotalYieldTotal->Caption=sumByTotal;
                else
                    fShowBinSelect->labTotalYieldTotal->Caption=iLowYieldByTotalContactCount;
#endif
                (void)str;
            }
        }
        else
        {
            // GATE (Y2) STAYS -- `fShowBinSelect->labTotalYield`/
            // `labTotalYieldTotal` (golden :5094/:5095) do not exist on the
            // landed TfShowBinSelect (see CheckBySiteYieldAlarm's own labArmDiff
            // site for the full re-verification note).
#if 0
            fShowBinSelect->labTotalYield->Caption=0;
            fShowBinSelect->labTotalYieldTotal->Caption=0;
#endif
        }
    }
    else
    {
        iCount4=0;
    }
}

// =============================================================================
//  Sliding Window Ring Buffer Methods -- golden :5104-5159
// =============================================================================
void TfYieldMonitoring::SWRingReset(int arm, int row, int col)
{
    TSWRing &buf=SW[arm][row][col];
    buf.iHead=0;
    buf.iCount=0;
    buf.iPassCT=0;
    buf.iLastBySiteTotal=0;
    buf.iLastBySitePass=0;
}

void TfYieldMonitoring::SWRingPush(int arm, int row, int col, bool bIsPass, int N)
{
    TSWRing &buf=SW[arm][row][col];
    if(N<=0 || N>2000) N=400;
    while(buf.iCount>=N)
    {
        int oldest=(buf.iHead-buf.iCount+2000)%2000;
        if(buf.bPass[oldest]) buf.iPassCT--;
        buf.iCount--;
    }
    buf.bPass[buf.iHead]= bIsPass ? 1 : 0;
    if(bIsPass) buf.iPassCT++;
    buf.iHead=(buf.iHead+1)%2000;
    buf.iCount++;
}

void TfYieldMonitoring::CheckLowYieldAlarm_SW()                                 //Steven 20260331 : Sliding Window Low Yield
{
    AnsiString SocketErrPart="";
    bool bIsLowYield=false;
    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            if(LastSet.bUseTestSocket[0][i][j] || LastSet.bUseTestSocket[1][i][j])
            {
                int sw_cnt=SW[0][i][j].iCount+SW[1][i][j].iCount;
                if(sw_cnt>=Prod.iSlidingWindowSize && sw_cnt>0)           //Steven 20260421 : add zero-guard for sw_cnt
                {
                    int    sw_pas=SW[0][i][j].iPassCT+SW[1][i][j].iPassCT;
                    double dYield=(double)sw_pas*100.0/sw_cnt;
                    if(Prod.dLowYieldLimit>dYield)
                    {
                        bIsLowYield=true;
                        SocketErrPart+=IndexSuckName[i][j];
                    }
                }
            }
        }
    }

    if(bIsLowYield)
        DoLowYieldAlarm("WAR0701", SocketErrPart);
}
//=== End Sliding Window ===                                                     //Steven 20260331

// =============================================================================
//  ClearYieldCount -- golden :5161-5200
// =============================================================================
void TfYieldMonitoring::ClearYieldCount()                                       //Steven 20140830 : Yield相關的Alarm, 要清掉全部的Ignore的Count重算
{
    if(CosFunction.bYieldAlarmClearAllCount)
    {
        MyDBIProductionData("Clear yield count");                               //Steven 20140816 : Production Data
        iLowYieldContactCount=0;
        iLowYieldByTotalContactCount=0;                                         //Kaichen 20190628 : Low Yield ByTotal 控制使用 Contact Count
        iFailAlarmSiteYieldIntervalCount=0;
        iPickerYieldIntervalCount=0;                                            //Steven 20230223 : 根據Index吸嘴比較良率
        iFailAlarmSiteMaxYieldIntervalCount=0;
        // AI(W906-FW-YEnable) 20260818: gate dissolved -- fContactCT/fShowBinSelect landed (ecf6154)
        fShowBinSelect->iLowYieldBinSelectContactCount=0;                       //KaiChen 20181115 : BinSelect裡面 Yield控制使用 Contact Count

        for(int i=0; i<MAX_SOCKET_ROW; i++)
        {
            for(int j=0; j<MAX_SOCKET_COL; j++)
            {
                ContinuousFailSKTCount[i][j]=0;
                SpecialBinContinuousFailSKTCount[i][j]=0;
                SpecialBinContinuousFailARMCount[0][i][j]=0;
                SpecialBinContinuousFailARMCount[1][i][j]=0;
                ContinuousFailARMCount[0][i][j]=0;
                ContinuousFailARMCount[1][i][j]=0;
            }
        }

        for(int i=0; i<TEST_MAX_BIN; i++)
        {
            iLoadPersentCT[i]=RunInfo.iUnloadCount;                             //Steven 20140830 : 改成全域變數        //Steven 20140905 : LastSet.SendCT[0] --> RunInfo.iUnloadCount
            iLoadCountCT[i]  =RunInfo.iUnloadCount;                             //Steven 20140830 : 改成全域變數        //Steven 20140905 : LastSet.SendCT[0] --> RunInfo.iUnloadCount
            iYeildCT[i]      =RunInfo.iUnloadCount;                             //wei 20151111
        }
        // Clear Sliding Window Ring Buffer                                       //Steven 20260331
        for(int arm=0; arm<2; arm++)
            for(int i=0; i<MAX_SOCKET_ROW; i++)
                for(int j=0; j<MAX_SOCKET_COL; j++)
                    SWRingReset(arm, i, j);
        // End Sliding Window                                                     //Steven 20260331
        bFirstCount=true;
    }
}

// =============================================================================
//  CanAutoCloseSite -- golden :5300-5334 (pure judgement, no gates)
//  0: Low Yield關, 1: 全開, 2: RT關Site
// =============================================================================
bool TfYieldMonitoring::CanAutoCloseSite(int iAllSiteOn)                        //Steven 20230315 : 整合自動關Site功能的判斷
{
    bool bFlag=false;

    if(iAllSiteOn==1)
    {
        bFlag=TestIF_File.iAllSiteOnAtInitialStart;                             //Steven 20230814 : Initial Start的時候要全開Site
    }
    else if(iAllSiteOn==2)
    {
        if(CosFunction.bAutoCloseSiteWhenRT)
        {
            if(CosFunction.bUseSCKART && USE_AUTO_RETEST==eartInstall && IniConfig.bA10_AutoReTest && TestIF_File.bSCKART_EnableART==true)
                bFlag=false;
            else if(TestIF_File.iAutoCloseSiteWhenRT)
                bFlag=true;
        }
    }
    else
    {
        if(CosFunction.bLowYieldAutoSiteOff && iRunStartMode==FT)
        {
            if((TestIF_File.bLowYieldAutoSiteOff && Prod.bFailAlarmSiteYieldCmp) ||                                     //Steven 20170905 (wei) : Low Yield Auto Site Off for Ambient
               (TestIF_File.bLowYieldAutoSiteOffByContiFail && Prod.bContsFailBySocket) ||                              //Steven 20200420 : Continue fail, auto site off
               (TestIF_File.bLowYieldAutoSiteOffArmContiFail && Prod.bContsFailByHead && IniConfig.bA09_ByArmCloseSite) ||                                      //Steven 20220818 : By Arm Continue fail, auto site off
               (TestIF_File.bLowYieldAutoSiteOffByPicker && Prod.bLowYieldByPicker && IniConfig.bA09_ByArmCloseSite) ||                                         //Steven 20230223 : 根據Index吸嘴比較良率
               (TestIF_File.iAutoSiteOffByGPIB==1 && bGetGPIBAutoSiteOff))      //JimmyChiu 20250715 : Auto site on/off by GPIB
            {
                bFlag=true;
            }
        }
    }
    bCanAutoCloseSite=bFlag;
    return bFlag;
}

// =============================================================================
//  CheckIntervalLowYieldAlarmBySite -- golden :5490-5592 (Interval Low Yield Alarm(%)by Site)
// =============================================================================
void TfYieldMonitoring::CheckIntervalLowYieldAlarmBySite()                      //wei 20180606 Interval Low Yield By Site
{
    static int iCount5=0;

    int iAlarmTimeInterval=(CosFunction.bLowYieldAlarmIntervalTimeBySetting &&  //JimmyChiu 20220601 : 修改low yield報警時間邏輯，報警間隔時間固定1min改為可以自行設定報警間隔時間
                            IniConfig.bI44_LowYieldAlarmIntervalTimeBySetting)?IniConfig.iI44_LowYieldAlarmIntervalTime:60;

    bool bLowYieldbySite=false;
    double dYield1[2][4][8]={0.0};
    AnsiString ErrPart="";

    if(bFirstCount)
    {
        iCount5=0;
        return;
    }

    if(SystemStart &&
       fContact->fShow==false &&
       iHome==0 &&
       bRunAutoClean==false &&
       bZ1PickShuttle==false &&
       bZ2PickShuttle==false)                                                   //Steven 20180521 : 避免吸料的時候發Low Yield Alarm
    {
        memset(iYieldSiteBinpass, 0, sizeof(iYieldSiteBinpass));
        // AI(W906-FW-YEnable) 20260818: gate dissolved -- fContactCT/fShowBinSelect landed (ecf6154)
        fShowBinSelect->ShowCategoryBin();
        if(Prod.bFailAlarmIntervalLowYieldBySite &&
           Prod.dIntervalLowYieldLimitBySite!=0 &&                              //JerryYang 20160530 LowYieldLimit要能設定到小數點
           CosFunction.IntervalYieldCount)
        {
            iCount5++;
            if(iCount5>=iAlarmTimeInterval ||                                   //JimmyChiu 20220601 : 修改low yield報警時間邏輯，報警間隔時間固定1min改為可以自行設定報警間隔時間
               CosFunction.bYieldAlarmNoWait1Min)                               //wei 20150820  Yield Alarm No Wait 1Min
            {
                iCount5=0;

                for(int i=0; i<TestSocket.iShtRow; i++)
                {
                    for(int j=0; j<TestSocket.iShtCol; j++)
                    {
                        for(int l=0; l<2; l++)
                        {
                            for(int k=0; k<Prod.iIntervalLowYieldCountBySite; k++)
                            {
                                if(bIntervalYieldIsPass[l][i][j][k]==true)
                                {
                                    iYieldSiteBinpass[l][i][j]++;
                                }
                            }
                            dYield1[l][i][j]=ChangeToFloatNonPcnt((double)(iYieldSiteBinpass[l][i][j]*100), (double)(Prod.iIntervalLowYieldCountBySite));
                        }

                        if(bUseTwoArm32Site==true)                              //Steven 20220418 : NN mode Yield alarm
                        {
                            dSiteYield[i][j]=dYield1[0][i][j];
                        }
                        else if(TestIF.iShuttleMode==0)
                        {
                            dSiteYield[i][j]=(dYield1[0][i][j]+dYield1[1][i][j])/2.0;
                        }
                        else if(TestIF.iShuttle_Sel==0)
                        {
                            dSiteYield[i][j]=dYield1[0][i][j];
                        }
                        else
                        {
                            dSiteYield[i][j]=dYield1[1][i][j];
                        }

                        if(dSiteYield[i][j]<Prod.dIntervalLowYieldLimitBySite)  //JerryYang 20160530 LowYieldLimit要能設定到小數點
                        {
                            bLowYieldbySite=true;
                            ErrPart+=IndexSuckName[i][j];
                        }
                    }
                }
                // AI(W906-FW-YEnable) 20260818: gate dissolved -- fContactCT/fShowBinSelect landed (ecf6154)
                fContactCT->sgYield->Refresh();

                if((bYieldSiteBin[0] || bYieldSiteBin[1]) && bYieldSiteBinCheck)
                {
                    if(bLowYieldbySite==true)
                    {
                        ShowErrorMessage("WAR0721", K_RETRY, MMInterface, false);
                        bLowYieldbySite=false;
                    }
                    bYieldSiteBinCheck=false;
                }
            }
        }
        else
        {
            for(int i=0; i<2; i++)
            {
                iYieldSiteCount[i]=0;
                bYieldSiteBin[i]=false;
            }
        }
    }
    else
    {
        iCount5=0;
    }
}

// =============================================================================
//  CheckIntervalLowYieldAlarmByTotal -- golden :5627-5701 (Interval Low Yield Alarm(%)by Total)
// =============================================================================
void TfYieldMonitoring::CheckIntervalLowYieldAlarmByTotal()                     //wei 20180718 Interval Low Yield By Total
{
    static int iCount6=0;

    int iAlarmTimeInterval=(CosFunction.bLowYieldAlarmIntervalTimeBySetting &&  //JimmyChiu 20220601 : 修改low yield報警時間邏輯，報警間隔時間固定1min改為可以自行設定報警間隔時間
                            IniConfig.bI44_LowYieldAlarmIntervalTimeBySetting)?IniConfig.iI44_LowYieldAlarmIntervalTime:60;

    bool bLowYieldbyTotal=false;
    double dYield1=0.0;

    if(bFirstCount)
    {
        iCount6=0;
        return;
    }

    if(SystemStart &&
       fContact->fShow==false &&
       iHome==0 &&
       bRunAutoClean==false)                                                    //ChungHung 20131223 add for SCK Autoclean
    {
        iYieldTotalBinpass=0;
        // AI(W906-FW-YEnable) 20260818: gate dissolved -- fContactCT/fShowBinSelect landed (ecf6154)
        fShowBinSelect->ShowCategoryBin();
        if(Prod.bFailAlarmIntervalLowYieldByTotal &&
           Prod.dIntervalLowYieldLimitByTotal!=0 &&
           CosFunction.IntervalYieldCount)                                      //JerryYang 20160530 LowYieldLimit要能設定到小數點
        {
            iCount6++;
            if(bZ1PickShuttle || bZ2PickShuttle)                                //Steven 20180521 : 避免吸料的時候發Low Yield Alarm
                return;
            if(iCount6>=iAlarmTimeInterval ||                                   // 1 mimutes    //JimmyChiu 20220601 : 修改low yield報警時間邏輯，報警間隔時間固定1min改為可以自行設定報警間隔時間
               CosFunction.bYieldAlarmNoWait1Min)                               //wei 20150820  Yield Alarm No Wait 1Min
            {
                iCount6=0;

                for(int k=0; k<Prod.iIntervalLowYieldCountByTotal; k++)
                {
                    if(bYieldTotalBinIsPass[k]==true)
                    {
                        iYieldTotalBinpass++;
                    }
                }
                dYield1=ChangeToFloatNonPcnt((double)(iYieldTotalBinpass*100), (double)(Prod.iIntervalLowYieldCountByTotal));
                // GATE (Y2) STAYS -- `fShowBinSelect->IntervalByTotal` (golden
                // :5670) does not exist on the landed TfShowBinSelect (see
                // CheckBySiteYieldAlarm's own labArmDiff site for the full
                // re-verification note). `dYield1` keeps being read for real below
                // (the bLowYieldbyTotal decision) regardless.
#if 0
                fShowBinSelect->IntervalByTotal->Caption=dYield1;
#endif

                if(dYield1<Prod.dIntervalLowYieldLimitByTotal)                  //JerryYang 20160530 LowYieldLimit要能設定到小數點
                {
                    bLowYieldbyTotal=true;
                }

                if(bYieldTotalBin && bYieldSiteBinCheck)
                {
                    if(bLowYieldbyTotal==true)
                    {
                        ShowErrorMessage("WAR0722", K_RETRY, MMInterface, false);
                        bLowYieldbyTotal=false;
                        bYieldTotalBin=false;
                        iYieldTotalCount=0;
                        memset(bYieldTotalBinIsPass, false, sizeof(bYieldTotalBinIsPass));
                    }
                    bYieldSiteBinCheck=false;
                }
            }
        }
        else
        {
            iYieldTotalCount=0;
            bYieldTotalBin=false;
        }
    }
    else
    {
        iCount6=0;
    }
}

// =============================================================================
//  CheckLowYieldAlarmSpecial -- golden :5703-5805 (Sam 20210505 : PTI 要求的兩段 Low Yeild)
// =============================================================================
void TfYieldMonitoring::CheckLowYieldAlarmSpecial()
{
    static int iCount5=0;

    int iSum=0, iPass=0;
    bool bIsLowYield=false;
    bool bNeedClear=false;
    double dYield1=0.0;
    AnsiString SocketErrPart="",str="";

    if(bFirstCount)
    {
        iCount5=0;
        return;
    }

    if(SystemStart &&
       fContact->fShow==false &&
       iHome==0 &&
       bRunAutoClean==false)
    {
        if(Prod.bFailAlarmLowYieldSpecial &&
           Prod.dLowYieldLimitSpecial!=0 &&
           CosFunction.bSpecailLowYeild)
        {
            iCount5++;
            if(bZ1PickShuttle || bZ2PickShuttle)
                return;

            //if(iCount5>=60 || CosFunction.bYieldAlarmNoWait1Min)              // 1 mimutes  //wei 20150820  Yield Alarm No Wait 1Min
            {
                iCount5=0;
                iSum=0;
                iPass=0;
                bIsLowYield=false;
                bNeedClear=false;

                for(int i=0; i<eTrayCount; i++)                                 //JerryYang 20230925
                {
                    if(Prod.iTrayType[i]==tNotUse)
                        continue;
                    iSum+=LastSet.BinCT_PTI[0][iTo3Unload[i]];
                    if(Prod.iIsPassT6[i]==1)                                    //Steven 20240105 : Prod.bIsPass --> Prod.iIsPassT6
                        iPass+=LastSet.BinCT_PTI[0][iTo3Unload[i]];
                }

                if(iSum>0)
                {
                    dYield1=ChangeToFloat(double(iPass), double(iSum));
                    if(Prod.dLowYieldLimitSpecial>dYield1)
                        bIsLowYield=true;
                }

                if(iSum>Prod.iLowYieldCountSpecial1 &&
                   bLowYeildAlarmSpecial1stPass==false)
                {
                    bNeedClear=true;
                    if(bIsLowYield)
                    {
                        bLowYeildAlarmSpecial=true;
                        fMain->CleanOut("CheckLowYieldAlarmSpecial");           //第一段 Low Yield 做 CleanOut 後報警提示修機
                    }
                    else
                    {
                        bLowYeildAlarmSpecial1stPass=true;                      //第一段 Low Yield 檢查過了
                    }
                }
                else if(iSum>Prod.iLowYieldCountSpecial2)
                {
                    bNeedClear=true;
                    if(bIsLowYield)
                    {
                        DoLowYieldAlarm("WAR0725", SocketErrPart);              //第二段 Low Yield 直接報警
                        //bLowYeildAlarmSpecial1stPass=false;                   //Sam 20211221 : Lot Start 才需要重新第一階段檢查 Mark //重新檢查第一段
                    }
                }

                if(bNeedClear)
                {
                    for(int i=0; i<10; i++)
                    {
                        LastSet.BinCT_PTI[0][i]=0;
                        LastSet.BinCT_PTI[2][i]=0;
                        LastSet.BinCT_PTI[3][i]=0;
                    }
                }

                // GATE (Y2) STAYS -- `fShowBinSelect->lblSpeciallYield`/
                // `lblSpeciallYieldTotal` (golden :5791/:5792) do not exist on the
                // landed TfShowBinSelect (see CheckBySiteYieldAlarm's own
                // labArmDiff site for the full re-verification note). `str` is
                // computed only to feed the gated Caption write, gated alongside
                // -- `(void)str;` restored.
#if 0
                str.sprintf("%0.2f", dYield1);
                fShowBinSelect->lblSpeciallYield->Caption=str.c_str();
                fShowBinSelect->lblSpeciallYieldTotal->Caption=iSum;
#endif
                (void)str;
            }
        }
        else
        {
            // GATE (Y2) STAYS -- `fShowBinSelect->lblSpeciallYield`/
            // `lblSpeciallYieldTotal` (golden :5797/:5798) do not exist on the
            // landed TfShowBinSelect (see CheckBySiteYieldAlarm's own labArmDiff
            // site for the full re-verification note).
#if 0
            fShowBinSelect->lblSpeciallYield->Caption=0;
            fShowBinSelect->lblSpeciallYieldTotal->Caption=0;
#endif
        }
    }
    else
    {
        iCount5=0;
    }
}

// =============================================================================
//  CheckSettingNo -- golden :3220-3231 (Steven 20110506 : 加入範圍保護)
//  DEVIATION: public here, golden private -- see forms/fYieldMonitoring.h banner.
// =============================================================================
void TfYieldMonitoring::CheckSettingNo()
{
//    TestIF_File.iIgnoreIC                   =CheckRange(int(TestIF_File.iIgnoreIC),                    100000, 1);
    TestIF_File.iContinuousPassBin          =CheckRange(int(TestIF_File.iContinuousPassBin),           16, 1);
    TestIF_File.iContinuousPassBin_RT       =CheckRange(int(TestIF_File.iContinuousPassBin_RT),        16, 1);
    TestIF_File.iContinuousPassBinCount     =CheckRange(int(TestIF_File.iContinuousPassBinCount),      iMinCount, iMaxCount);
    TestIF_File.iContinuousPassBinCount_RT  =CheckRange(int(TestIF_File.iContinuousPassBinCount_RT),   iMinCount, iMaxCount);
    TestIF_File.iContinuousLoaderCount      =CheckRange(int(TestIF_File.iContinuousLoaderCount),       iMinCount, iMaxCount);
    TestIF_File.iContinuousLoaderCount_RT   =CheckRange(int(TestIF_File.iContinuousLoaderCount_RT),    iMinCount, iMaxCount);
    TestIF_File.iContinuousContactCount     =CheckRange(int(TestIF_File.iContinuousContactCount),      iMinCount, iMaxCount);
    TestIF_File.iContinuousContactCount_RT  =CheckRange(int(TestIF_File.iContinuousContactCount_RT),   iMinCount, iMaxCount);
}

// =============================================================================
//  SetClosedSiteBin -- golden :5972-5980 (Steven 20240409 : 關site的位置有IC不測試送指定 bin)
// =============================================================================
void TfYieldMonitoring::SetClosedSiteBin()
{
    cbbClosedSiteBin->Clear();
    for(int i=0; i<iTestBinCount; i++)
    {
        cbbClosedSiteBin->Items->Add(i);                                        //Steven 20240409 : 關site的位置有IC不測試送指定 bin
    }
    cbbClosedSiteBin->Items->Add("Error");
}

// =============================================================================
// AI(W906-FW-YMSwap) 20260818: the live global comes HOME (golden
// uYieldMonitoring.h declares `extern PACKAGE TfYieldMonitoring
// *fYieldMonitoring;`; the VCL runtime constructs it in WinMain's CreateForm
// chain). Static-init here is trivially safe: no user ctor, NSDMI only,
// zero config-layer touches. The TfYieldMonitoring_2x4_16 stand-in
// (aHotPlateSubstrate.h / ainarm9045_2x4_16_shims) retired the same commit.
// =============================================================================
TfYieldMonitoring *fYieldMonitoring = new TfYieldMonitoring();

// AI(W906-FW-YMSwap) 20260818: DOCUMENTED NO-OP -- behaviour-neutral swap.
// The REAL body (golden uYieldMonitoring.cpp:5340-5401) actually closes
// sites on low yield; translating it is the user-approved queue's own
// behaviour-change wave with its own gate and commit. Until then the ~26
// live arm-variant call sites keep the exact no-op the retired shim gave
// them.
void TfYieldMonitoring::DoAutoCloseSite(bool /*bRT*/)
{
}
