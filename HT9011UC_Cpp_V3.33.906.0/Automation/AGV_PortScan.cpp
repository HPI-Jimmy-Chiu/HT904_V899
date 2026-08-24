// =============================================================================
//  Automation/AGV_PortScan.cpp  --  E84 SENSOR-STATUS TELEMETRY + AMR/AGV PORT
//                                   SCAN FAMILY (W5-Automation-cont unit)
//
//  See AGV_PortScan.h for the full scope note (GROUP 1 / GROUP 2 split, the
//  3 new FormsFacade members this unit needed, the deferred "ini-editor
//  sextet", and the golden-quirk index).  Read that file first.
//
//  Translation wave: W5-Automation-cont (AGV_PortScan unit)
//  Translator: AI(W5-Automation-PortScan) 20260713
// =============================================================================
#include "AGV_PortScan.h"

#include "cmydef.h"          // SW/Sen index constants; iTrayEnable/iPortStatus/iLastPortStatus/
                              // bPortIsBusy/bAskStopPort/bForceSendLoaderIsEmpty/iThisPortNo/
                              // iThisPortStatus/asSupplementBin/InitialOK-adjacent globals;
                              // SnFixedTrayDetect[]/iAutoIndex[]/iTempFix[]/iMMAuto[]/iTrayFeed/
                              // AUTO3_IS_MAGAZINE/iMagAtAuto/bCleanOutTrayEnd/CUSTOMER_CODE
#include "MachineType.h"      // ePortLoader../ePortTotal, eLoadError../eLoadHighWIP,
                              // eReadyToUnload/eFullBundle/eUnloadTrayArrived/eUnloadEmpty,
                              // eFixEmpty/eFixFTrayArrived, etLoader../etTrayCount, tNotUse,
                              // CC_MAXIM/CC_MAXIM_THAILAND/CC_KYEC_LEE, rsmInitial_ART../rsmContinuRetest_ART
#include "cprod.h"            // TestIF_File (.iMagTraySource/.iUnloaderTrayCount[]/
                              // .iLoaderUnloaderTrayCount[]), Prod (.iTrayType[]/.iIsPassT6[]/
                              // .iIsFailT6[]), TrayForm (.Auto[].TrayEndRecv), Config.h's IniConfig
                              // (.bA65_BundleIDList/.bSPILFunction/.bP09TrayEndCanSelectTray/.bA10_AutoReTest)
#include "mysensor.h"          // Sen[] (TMySensor Sim HAL) + .Enable
#include "myswitch.h"          // SW[] (TMySwitch Sim HAL)
#include "Motor/mymotor.h"     // MOT[] (TTrayMotor: .fHasTray/.FullIC()/.HowManyDevice())
#include "canary_support.h"    // ShowMyMessage (ACTIVE Sim stand-in) + LastSet.iRunStartMode
#include "FormsFacade.h"       // fMain->ALed1/labAutomation ; fLotInfo->ALedLoader/aLedAuto[]/palRemoveTray
#include "AGV_E84.h"           // iLoaderScanTask[]/iUnloaderScanTask[]/bE84Loaderflag[]/bE84Unloaderflag[] (SHARED, reused not redeclared)
#include "AGV_predicates.h"    // AGV_IsATK_AMR() -- golden ScanLoadPort's unqualified IsATK_AMR() call
#include "SECSGEM/SecsEventType.h"     // SECS_EVENT (ETypeStruct)
#include "SECSGEM/SecsEventReport.h"   // EventReport(unsigned)
#include "forms/fAGV.h"               // AI(W906-FW-QWKEY6) 20260824: TfAGV class (3 MouseDown handlers below)
#include "forms/fQwertyKey.h"         // AI(W906-FW-QWKEY6) 20260824: fQwertyKey (real since FW-QWKEY1 fc08e09)

// =============================================================================
//  GROUP 1a -- E84 sensor-status telemetry (golden :77-169).
// =============================================================================

// ---------------------------------------------------------------------------
//  E84NowStatusFlag[loader/unloader][switch-bank/sensor-bank][index] (golden
//  Automation/AGV.cpp:78, external linkage there).  Verified via whole-golden-
//  tree grep: referenced ONLY inside this same file (NowE84Status writes,
//  E84StatusChange reads) -- tightened to `static` (TU-local) here, same
//  convention as AGV_E84.cpp's E84LoadDelay/E84UnLoadDelay.
// ---------------------------------------------------------------------------
static bool E84NowStatusFlag[2][2][9];

//------------------------------------------------------------------------------
void NowE84Status()
{
    E84NowStatusFlag[0][0][0]=SW[SwE84_1_LREQ].Status();
    E84NowStatusFlag[0][0][1]=SW[SwE84_1_UREQ].Status();
    E84NowStatusFlag[0][0][2]=SW[SwE84_1_VA].Status();
    E84NowStatusFlag[0][0][3]=SW[SwE84_1_READY].Status();
    E84NowStatusFlag[0][0][4]=SW[SwE84_1_VS0].Status();
    E84NowStatusFlag[0][0][5]=SW[SwE84_1_VS1].Status();
    E84NowStatusFlag[0][0][6]=SW[SwE84_1_HOAVBL].Status();
    E84NowStatusFlag[0][0][7]=SW[SwE84_1_ES].Status();

    E84NowStatusFlag[0][1][0]=Sen[SnE84_1_VALID].IsOn();
    E84NowStatusFlag[0][1][1]=Sen[SnE84_1_CS0].IsOn();
    E84NowStatusFlag[0][1][2]=Sen[SnE84_1_CS1].IsOn();
    E84NowStatusFlag[0][1][3]=Sen[SnE84_1_AMAVBL].IsOn();
    E84NowStatusFlag[0][1][4]=Sen[SnE84_1_TRREQ].IsOn();
    E84NowStatusFlag[0][1][5]=Sen[SnE84_1_BUSY].IsOn();
    E84NowStatusFlag[0][1][6]=Sen[SnE84_1_CONT].IsOn();
    E84NowStatusFlag[0][1][7]=Sen[SnE84_1_COMPT].IsOn();
    E84NowStatusFlag[0][1][8]=Sen[SnE84_1_GO].IsOn();

    E84NowStatusFlag[1][0][0]=SW[SwE84_2_LREQ].Status();
    E84NowStatusFlag[1][0][1]=SW[SwE84_2_UREQ].Status();
    E84NowStatusFlag[1][0][2]=SW[SwE84_2_VA].Status();
    E84NowStatusFlag[1][0][3]=SW[SwE84_2_READY].Status();
    E84NowStatusFlag[1][0][4]=SW[SwE84_2_VS0].Status();
    E84NowStatusFlag[1][0][5]=SW[SwE84_2_VS1].Status();
    E84NowStatusFlag[1][0][6]=SW[SwE84_2_HOAVBL].Status();
    E84NowStatusFlag[1][0][7]=SW[SwE84_2_ES].Status();

    E84NowStatusFlag[1][1][0]=Sen[SnE84_2_VALID].IsOn();
    E84NowStatusFlag[1][1][1]=Sen[SnE84_2_CS0].IsOn();
    E84NowStatusFlag[1][1][2]=Sen[SnE84_2_CS1].IsOn();
    E84NowStatusFlag[1][1][3]=Sen[SnE84_2_AMAVBL].IsOn();
    E84NowStatusFlag[1][1][4]=Sen[SnE84_2_TRREQ].IsOn();
    E84NowStatusFlag[1][1][5]=Sen[SnE84_2_BUSY].IsOn();
    E84NowStatusFlag[1][1][6]=Sen[SnE84_2_CONT].IsOn();
    E84NowStatusFlag[1][1][7]=Sen[SnE84_2_COMPT].IsOn();
    E84NowStatusFlag[1][1][8]=Sen[SnE84_2_GO].IsOn();
}
//------------------------------------------------------------------------------
void E84StatusChange()
{
    static bool E84StatusFlag[2][2][9];

    bool bflag;

    // AI(W5-Automation-PortScan) 20260713: FAITHFUL GOLDEN QUIRK, preserved
    // verbatim (golden Automation/AGV.cpp:127-148) -- indices [i][1][6]/[7]
    // ("...CONT"/"...COMPT") are SWAPPED relative to what NowE84Status() (just
    // above) actually stores at those two indices: index 6 holds the CONT
    // sensor's reading but is labelled "...COMPT" below, and index 7 holds
    // COMPT but is labelled "...CONT". A genuine golden mislabel bug (both the
    // Loader i=0 and Unloader i=1 blocks repeat it identically) -- NOT
    // corrected. Impact is cosmetic only: it feeds the diagnostic text
    // E84StatusLog() writes via the (itself-gated) WriteDataToFile sink.
    AnsiString  asE84StatusFlag[2][2][9]={
        {
            {
                "SwE84_1_LREQ", "SwE84_1_UREQ", "SwE84_1_VA", "SwE84_1_READY",
                "SwE84_1_VS0", "SwE84_1_VS1", "SwE84_1_HOAVBL", "SwE84_1_ES", ""
            },
            {
                "SnE84_1_VALID", "SnE84_1_CS0", "SnE84_1_CS1", "SnE84_1_AMAVBL", "SnE84_1_TRREQ",
                "SnE84_1_BUSY", "SnE84_1_COMPT", "SnE84_1_CONT", "SnE84_1_GO"
            }
        },
        {
            {
                "SwE84_2_LREQ", "SwE84_2_UREQ", "SwE84_2_VA", "SwE84_2_READY",
                "SwE84_2_VS0", "SwE84_2_VS1", "SwE84_2_HOAVBL", "SwE84_2_ES", ""
            },
            {
                "SnE84_2_VALID", "SnE84_2_CS0", "SnE84_2_CS1", "SnE84_2_AMAVBL", "SnE84_2_TRREQ",
                "SnE84_2_BUSY", "SnE84_2_COMPT", "SnE84_2_CONT", "SnE84_2_GO"
            }
        }
    };

    NowE84Status();
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<2; j++)
        {
            for(int k=0; k<9; k++)
            {
                if(E84NowStatusFlag[i][j][k]!=E84StatusFlag[i][j][k])
                {
                    bflag=false;
                    if(E84NowStatusFlag[i][j][k])
                        bflag=true;

                    E84StatusLog(asE84StatusFlag[i][j][k], bflag);
                    E84StatusFlag[i][j][k]=E84NowStatusFlag[i][j][k];
                }
            }
        }
    }
}

// =============================================================================
//  GROUP 1b / GROUP 2 -- AMR/AGV port-scan family (golden :1147-1288, :1350-1897).
// =============================================================================

// ---------------------------------------------------------------------------
//  golden Automation/AGV.cpp:1147 (file-scope, external linkage there).
//  Verified: referenced only inside DoE84UnloaderScan (this file) -- tightened
//  to `static` (TU-local), same convention as AGV_E84.cpp's E84LoadDelay.
// ---------------------------------------------------------------------------
static TQPF_Timer ScanUnloaderDelayTime;

//------------------------------------------------------------------------------
void DoE84LoaderScan(int Pos)
{
    static int iOldSec[3];
    static int iCount[3];

    int &Task=iLoaderScanTask[Pos];
    int SenIndex[3]={SnLoaderTrayHasTray_AGV,SnEmptyTrayHasTray_AGV,SnColorTrayHasTray_AGV};

    AnsiString str="", asStr="";

    if(SystemStart==false)
        return;

    switch(Task)
    {
        case 1:
            bE84Loaderflag[Pos]=false;
            Task=100;
            break;
        case 100:
            // AI(W5-Automation-PortScan) 20260713: fMain->labAutomation (golden
            // main.h:802, TPanel*) -- new FormsFacade member this unit added
            // (member (B), see AGV_PortScan.h scope note).
            if(fMain->labAutomation->Caption=="SECS GEM 4:Enable")
            {
                Task=1000;
            }
            break;
        case 1000:   //Check Loader Unloader Tray Count
            if(bE84Loaderflag[Pos]==false)
            {
                if(Sen[SenIndex[Pos]].IsOff()==true)
                {
                    if(iOldSec[Pos]!=SystemSec)
                    {
                        iOldSec[Pos]=SystemSec;
                        iCount[Pos]++;
                    }

                    if(iCount[Pos]>=5)
                    {
                        iCount[Pos]=0;
                        iTrayEnable[Pos]=1;
                        bE84Loaderflag[Pos]=true;
                        Task=2000;
                    }
                }
                else
                {
                    iCount[Pos]=0;
                    Task=100;
                }
            }
            else
            {
                Task=2100;
            }
            break;
        case 2000:
            asSupplementBin.sprintf("P1:%d,P2:%d,P3:%d", iTrayEnable[0], iTrayEnable[1], iTrayEnable[2]);
            // golden :1206 -- commented out in golden itself, SECS_EVENT_K12 is a
            // separate (older/K12) event table not part of this translated tree:
//            EventReport_K12(SECS_EVENT_K12.AGVSupplement);
            iTrayEnable[Pos]=0;
            Task=100;
            break;
        case 2100:
            if(bE84Loaderflag[Pos]==false)
            {
                Task=100;
            }
            break;
    }
}
//------------------------------------------------------------------------------
void DoE84UnloaderScan(int Pos)
{
    static int iCount[3];

    int &Task=iUnloaderScanTask[Pos];
//    int SenIndex[3]={SnLoaderTrayHasTray_AGV, SnEmptyTrayHasTray_AGV, SnColorTrayHasTray_AGV};

    switch(Task)
    {
        case 1:
            bE84Unloaderflag[Pos]=false;
            Task=100;
            break;
        case 100:
            iCount[Pos]=0;
            Task=1000;
            break;
        case 1000:   //Check Loader Unloader Tray Count
            if(bE84Unloaderflag[Pos]==false)
            {
                if(TestIF_File.iUnloaderTrayCount[Pos]>=TestIF_File.iLoaderUnloaderTrayCount[Pos])  // Need Remove Unloader Tray
                {
                    bE84Unloaderflag[Pos]=true;

                    iCount[Pos]=1;
                    TestIF_File.iUnloaderTrayCount[Pos]=0;

                    Task=1050;
                    break;
                }
                Task=100;
            }
            else
            {
                Task=1200;
            }
            break;
        case 1050:
//            if(bUnloaderTrayReceive[Pos])                       // golden: commented out in golden itself
            {
//                fMain->Pause();                                 // golden: commented out in golden itself
                ScanUnloaderDelayTime.SetMSAndOn(2000);
                Task=1100;
            }
            break;
        case 1100:
            // AI(W5-Automation-PortScan) 20260713: fMain->labAutomation (see
            // DoE84LoaderScan case 100 above -- same new FormsFacade member).
            if(fMain->labAutomation->Caption=="SECS GEM 4:Enable")
            {
                asSupplementBin.sprintf("P4:%d,P5:%d,P6:%d",iCount[0],iCount[1],iCount[2]);
//                EventReport_K12(SECS_EVENT_K12.AGVSupplement);   // golden :1268 -- commented out in golden itself
                iCount[Pos]=0;
                Task=100;
            }
            else if(ScanUnloaderDelayTime.Off())
            {
                ShowMyMessage("請檢查SECS是否連線");
                Task=100;
            }
            break;
        case 1200:
            if(bE84Unloaderflag[Pos]==false)
            {
                // AI(W5-Automation-PortScan) 20260713: fLotInfo->palRemoveTray
                // (golden uLotInfo.h:828, TPanel*) -- new FormsFacade member
                // this unit added (member (C), see AGV_PortScan.h scope note).
                fLotInfo->palRemoveTray->Enabled=false;
                fLotInfo->palRemoveTray->Visible=false;

                Task=100;//inital setting
            }
            break;
    }
}
//------------------------------------------------------------------------------
void ScanLoadPort()
{
    if(IniConfig.bA65_BundleIDList==true &&
       IniConfig.bSPILFunction==true)                                           //JerryYang 20250521 : For AMR
    {
        bScanLoadPortState_SPIL();
        bScanUnLoadPortState_SPIL(0);  //Auto1
        bScanUnLoadPortState_SPIL(1);  //Auto2
        bScanUnLoadPortState_SPIL(2);  //Auto3

//        bScanFixPortState_SPIL(0);  //Fix1                                    //JerryYang 20250602 : mark
//        bScanFixPortState_SPIL(1);  //Fix2
//        bScanFixPortState_SPIL(2);  //Fix3
    }
    else if(CUSTOMER_CODE==CC_MAXIM ||
            CUSTOMER_CODE==CC_MAXIM_THAILAND)
    {
        bScanLoadPortState_Analog(0);
        bScanLoadPortState_Analog(1);
        bScanLoadPortState_Analog(2);

        bScanUnLoadPortState_Analog(0);  //Auto1
        bScanUnLoadPortState_Analog(1);  //Auto2
        bScanUnLoadPortState_Analog(2);  //Auto3

        bScanFixPortState_Analog(0);  //Fix1
        bScanFixPortState_Analog(1);  //Fix2
        bScanFixPortState_Analog(2);  //Fix3
    }
    // AI(W5-Automation-PortScan) 20260713: golden `IsATK_AMR()` (unqualified
    // TfAGV member call, AGV.cpp:1379) -> AGV_IsATK_AMR() free-function
    // predicate (Automation/AGV_predicates.cpp) -- same demotion convention
    // AGV_predicates.h documents (FormsFacade's TfAGV::IsATK_AMR() itself just
    // forwards to this same free function).
    else if(AGV_IsATK_AMR())
    {
        bScanLoadPortState_ATK();
    }
}
//==============================================================================
bool bIsStackBusy(int iPortID)                                                  //JerryYang 20250505 : AGV  //JerryYang 20250521 : For AMR
{
    bool bResult=true;
//1: Loader port
//2: Empty port
//3: Color port
//4: Auto1 port
//5: Auto2 port
//6: Auto3 port
//7: Fix1 port
//8: Fix2 port
//9: Fix3 port
//10: Auto4 port
//11: Auto5 port
//12: Auto6 port
//13: Fix4 port
//14: Fix5 port
//15: Fix6 port
    if(iPortID>=1 && iPortID<=15)
    {
        // golden: empty if-body (AGV.cpp:1404-1405) -- a stub reserved for
        // future per-port busy logic, never filled in. Preserved as-is.
    }

    return bResult;
}

// ---------------------------------------------------------------------------
//  golden Automation/AGV.cpp:1409-1410 (file-scope, external linkage there).
//  Verified: tLoadPortTimer referenced only inside bScanLoadPortState_SPIL,
//  tUnloadPortTimer only inside bScanUnLoadPortState_SPIL (both this file) --
//  tightened to `static` (TU-local), same convention as AGV_E84.cpp.
// ---------------------------------------------------------------------------
static TQPF_Timer tLoadPortTimer;
static TQPF_Timer tUnloadPortTimer[3];

//==============================================================================
void bScanLoadPortState_SPIL()
{
    bool bHigh=false, bMiddle=false, bLow=false;

    int currentState=-1;

    static int candidateState=-1;

    // AI(W5-Automation-PortScan) 20260713: fLotInfo->ALedLoader / fMain->ALed1
    // (golden uLotInfo.h:971 / main.h:355, both TALed*) -- new FormsFacade
    // members this unit added (member (A), see AGV_PortScan.h scope note).
    fLotInfo->ALedLoader->Value=bAskStopPort[ePortLoader];

    if(bPortIsBusy[ePortLoader]==true || bAskStopPort[ePortLoader]==true)       //JerryYang 2020529
    {
        return;
    }

    bHigh=Sen[SnLoaderIsFull].IsOn();
    bMiddle=Sen[SnLoaderTrayHasTray_AGV].IsOn();
    bLow=fMain->ALed1->Value;

    if(bHigh==false && bMiddle==true && bLow==false)                            // 0   Error
    {                                                                           // 1
        currentState=eLoadError;                                                // 0
    }
    else if(bHigh==true && bMiddle==false && bLow==false)                       // 1   Error
    {                                                                           // 0
        currentState=eLoadError;                                                // 0
    }
    else if(bHigh==true && bMiddle==true && bLow==false)                        // 1   Error
    {                                                                           // 1
        currentState=eLoadError;
    }
    else if(bHigh==true && bMiddle==false && bLow==true)                        // 1   Error
    {                                                                           // 1
        currentState=eLoadError;                                                // 0
    }
    else if(bHigh==false && bMiddle==false && bLow==false)                      //Empty
    {                                                                           //0
        currentState=eLoadEmpty;                                                //0
    }                                                                           //0
    else if(bHigh==false && bMiddle==false && bLow==true)                       //Low Wip
    {                                                                           //0
        currentState=eLoadLowWIP;                                               //0
    }                                                                           //0
    else if(bHigh==false && bMiddle==true && bLow==true)                        //Low Wip
    {                                                                           //0
        currentState=eLoadTrayArrived;
    }
    else if(bHigh==true && bMiddle==true && bLow==true)                         //Low Wip
    {                                                                           //0
        currentState=eLoadHighWIP;                                              //0
    }                                                                           //0

    if(currentState!=candidateState)
    {
        candidateState=currentState;
        tLoadPortTimer.SetSecAndOn(3);
        return;                                                                 // 還沒穩定，不做事
    }
    else
    {
        if(tLoadPortTimer.Off()==false)                                         //穩定3秒
            return;

        if(iLastPortStatus[ePortLoader]!=currentState)
        {
            iPortStatus[ePortLoader]=currentState;
            iLastPortStatus[ePortLoader]=currentState;
            EventReport(SECS_EVENT.LoadPortStatusChanged);
        }
        else
        {
            if(bForceSendLoaderIsEmpty)                                         //JerryYang 20250618 : add
            {
                bForceSendLoaderIsEmpty=false;
                iPortStatus[ePortLoader]=currentState;
                iLastPortStatus[ePortLoader]=currentState;
                EventReport(SECS_EVENT.LoadPortStatusChanged);
            }
        }
    }
}
//==============================================================================
void bScanUnLoadPortState_SPIL(int iPos)                                        //JerryYang 20250521 : For AMR
{
    bool bPortHasTray=false;
    int currentState[3]={-1, -1, -1};
    static int candidateState[3]={-1, -1, -1};

    // AI(W5-Automation-PortScan) 20260713: fLotInfo->aLedAuto[3] -- golden
    // aLedAuto1/aLedAuto2/aLedAuto3 (uLotInfo.h:965,969,970, three separate
    // TALed* fields) consolidated into ONE array member (member (A), see
    // AGV_PortScan.h scope note) -- aLedAuto1->aLedAuto[0], aLedAuto2->
    // aLedAuto[1], aLedAuto3->aLedAuto[2].
    fLotInfo->aLedAuto[0]->Value=bAskStopPort[ePortAuto1];
    fLotInfo->aLedAuto[1]->Value=bAskStopPort[ePortAuto2];
    fLotInfo->aLedAuto[2]->Value=bAskStopPort[ePortAuto3];

    if(bPortIsBusy[ePortAuto1+iPos]==true ||
       bAskStopPort[ePortAuto1+iPos]==true ||
       iPortStatus[ePortAuto1+iPos]==eReadyToUnload ||
       iPortStatus[ePortAuto1+iPos]==eFullBundle)
    {
        return;
    }

    bPortHasTray=Sen[SnAutoTrayHasTray[iPos]].IsOn();

    if(bPortHasTray==false)
    {
        currentState[iPos]=eUnloadEmpty;
//        iPortStatus[ePortAuto1+iPos]=eUnloadEmpty;
//        if(iLastPortStatus[ePortAuto1+iPos]!=iPortStatus[ePortAuto1+iPos])
//        {
//            iLastPortStatus[ePortAuto1+iPos]=eUnloadEmpty;
//            EventReport(SECS_EVENT.Auto1PortStatusChanged+iPos);
//        }
    }
    else
    {
        currentState[iPos]=eUnloadTrayArrived;
    }

    if(currentState[iPos]!=candidateState[iPos])
    {
        candidateState[iPos]=currentState[iPos];
        tUnloadPortTimer[iPos].SetSecAndOn(3);
        return;                                                                 // 還沒穩定，不做事
    }
    else
    {
        if(tUnloadPortTimer[iPos].Off()==false)                                 //穩定3秒
            return;

        if(iLastPortStatus[ePortAuto1+iPos]!=currentState[iPos])
        {
            iPortStatus[ePortAuto1+iPos]=currentState[iPos];
            iLastPortStatus[ePortAuto1+iPos]=currentState[iPos];
            EventReport(SECS_EVENT.Auto1PortStatusChanged+iPos);
        }
    }
}
//==============================================================================
void bScanFixPortState_SPIL(int iPos)                                           //JerryYang 20250521 : For AMR
{
    bool bPortHasTray=false;

    if(bPortIsBusy[ePortFix1+iPos]==true)
    {
        return;
    }

    int iFix=iAutoIndex[eFix1+iPos];

    bPortHasTray=Sen[SnFixedTrayDetect[iFix]].IsOn();

    if(bPortHasTray==false)
    {
        iPortStatus[ePortFix1+iPos]=eFixEmpty;
        if(iLastPortStatus[ePortFix1+iPos]!=iPortStatus[ePortFix1+iPos])
        {
            iLastPortStatus[ePortFix1+iPos]=eFixEmpty;
            EventReport(SECS_EVENT.Fix1PortStatusChanged+iPos);
        }
    }
    else
    {
        iPortStatus[ePortFix1+iPos]=eFixFTrayArrived;
        if(iLastPortStatus[ePortFix1+iPos]!=iPortStatus[ePortFix1+iPos])
        {
            iLastPortStatus[ePortFix1+iPos]=eFixFTrayArrived;
            EventReport(SECS_EVENT.Fix1PortStatusChanged+iPos);
        }
    }
}
//==============================================================================
void bScanLoadPortState_Analog(int iPos)
{
    int currentState[3][2]={{-1, -1}, {-1, -1}, {-1, -1}};
//    int iCurrent[3]={-1, -1, -1};
    static int LoadcandidateState[3]={-1, -1, -1};

    int iLoSureHasTray[3]  ={SnLoaderSureTray,    SenEmptyHasTray,       SenColorHasTray};
    int iLoTrayIsFull[3]   ={SnLoaderIsFull,      SnEmptyIsFull,         SnColorIsFull};
    int iLoSelectHasTray[3]={SnLoaderTrayHasTray, SenEmptySelectHasTray, SenColorSelectHasTray};

    if(Sen[iLoSureHasTray[iPos]].IsOn())
    {
        currentState[iPos][0]=3;                                                //裡面有料
    }
    else if(Sen[iLoSureHasTray[iPos]].IsOff())
    {
        currentState[iPos][0]=5;                                                //裡面沒料
    }

    if(Sen[iLoTrayIsFull[iPos]].Enable && Sen[iLoTrayIsFull[iPos]].IsOn())
    {
        currentState[iPos][1]=2;                                                //外面滿料
    }
    else if(Sen[iLoSelectHasTray[iPos]].IsOff())
    {
        currentState[iPos][1]=4;                                                //外面沒料
    }
    else if(Sen[iLoSelectHasTray[iPos]].IsOn())
    {
        currentState[iPos][1]=1;                                                //若外面有料，則準備入料
    }

    LoadcandidateState[iPos]=currentState[iPos][0]*10+currentState[iPos][1];

    if(iLastPortStatus[ePortLoader+iPos]!=LoadcandidateState[iPos])
    {
        iPortStatus[ePortLoader+iPos]=LoadcandidateState[iPos];
        iLastPortStatus[ePortLoader+iPos]=LoadcandidateState[iPos];
        EventReport(SECS_EVENT.LoadPortStatusChanged+iPos);
    }
}
//==============================================================================
void bScanUnLoadPortState_Analog(int iPos)                                      //JerryYang 20250521 : For AMR
{
    int currentState[3][2]={{-1, -1}, {-1, -1}, {-1, -1}};
//    int iCurrent[3]={-1, -1, -1};
    static int UnloadcandidateState[3]={-1, -1, -1};

    int iUnSureHasTray[3]  ={SnAuto1_Tray_Car,    SnAuto2_Tray_Car,      SnAuto3_Tray_Car};
    int iUnTrayIsFull[3]   ={SnAuto1IsFull,       SnAuto2IsFull,         SnAuto3IsFull};
    int iUnTrayDetect[3]   ={SnAuto1TrayDetect,   SnAuto2TrayDetect,     SnAuto3TrayDetect};

    if(Sen[iUnTrayDetect[iPos]].IsOn())
    {
        currentState[iPos][0]=3;                                                //裡面有料
    }
    else if(Sen[iUnTrayDetect[iPos]].IsOff())
    {
        currentState[iPos][0]=5;                                                //裡面沒料
    }

    if(Sen[iUnTrayIsFull[iPos]].Enable && Sen[iUnTrayIsFull[iPos]].IsOn())
    {
        currentState[iPos][1]=2;                                                //外面滿料
    }
    else if(Sen[iUnSureHasTray[iPos]].IsOff())
    {
        currentState[iPos][1]=4;                                                //外面沒料
    }
    else if(Sen[iUnSureHasTray[iPos]].IsOn())
    {
        currentState[iPos][1]=1;                                                //若外面有料，則準備入料
    }

    UnloadcandidateState[iPos]=currentState[iPos][0]*10+currentState[iPos][1];

    if(iLastPortStatus[ePortAuto1+iPos]!=UnloadcandidateState[iPos])
    {
        iPortStatus[ePortAuto1+iPos]=UnloadcandidateState[iPos];
        iLastPortStatus[ePortAuto1+iPos]=UnloadcandidateState[iPos];
        EventReport(SECS_EVENT.Auto1PortStatusChanged+iPos);
    }
}
//==============================================================================
void bScanFixPortState_Analog(int iPos)                                         //JerryYang 20250521 : For AMR
{
    static int FixcandidateState[MAX_FIX_TRAY]={-1, -1, -1, -1, -1, -1};

    int iCountFix;
    int currentState;

    if(Sen[SnFixedTrayDetect[iPos]].IsOff())
    {
        currentState=5;                                                         //沒盤
    }
    else if(Sen[SnFixedTrayDetect[iPos]].IsOn())
    {
        currentState=3;                                                         //有盤
    }

    iCountFix=MOT[iTempFix[iPos]].HowManyDevice();
    if(iCountFix==0)
    {
        currentState=1;                                                         //有盤料滿，需下料
    }
    else if(iCountFix>0 &&
            Sen[SnFixedTrayDetect[iPos]].IsOn() &&
            iTrayFeed==1)
    {
        currentState=2;                                                         //有盤料未滿，需下料
    }

    FixcandidateState[iPos]=currentState;

    if(iLastPortStatus[ePortFix1+iPos]!=FixcandidateState[iPos])
    {
        iPortStatus[ePortFix1+iPos]=FixcandidateState[iPos];
        iLastPortStatus[ePortFix1+iPos]=FixcandidateState[iPos];
        EventReport(SECS_EVENT.Fix1PortStatusChanged+iPos);
    }
}
//------------------------------------------------------------------------------
// golden Automation/AGV.cpp:1704-1707 -- KEPT AS GENUINE EXTERNAL-LINKAGE
// GLOBALS (NOT static). See AGV_PortScan.h's comment on the extern
// declarations for the whole-golden-tree grep that justifies this.
int iLoadStateATK[etTrayCount];
int iCurrStateATK[etTrayCount];
AnsiString sDCC_ATK;
AnsiString sOutputBinCode;
enum eATKPortStatus
{
    eATKNoState        =0,                                                      //0: No state (port status unknown)
    eATKTransfer       =1,                                                      //1: Transfer Blocked (Transfer Blocked (tray is loaded in port))
    eATKReadyToLoad    =2,                                                      //2: Ready to Load (port is empty and ready for tray loading)
    eATKReadyToUnload  =3                                                       //3: Ready to Unload (material has been unloaded and can be removed)
};
//------------------------------------------------------------------------------
void bScanLoadPortState_ATK()
{
    static bool bFirstIn=true;
    int iAuto;

    if(bFirstIn)
    {
        bFirstIn=false;
        for(int i=0; i<etTrayCount; i++)
        {
            iLoadStateATK[i]=eATKNoState;
            iCurrStateATK[i]=eATKNoState;
        }
    }

    // AI(W5-Automation-PortScan) 20260713: FAITHFUL GOLDEN QUIRK, preserved
    // verbatim (golden Automation/AGV.cpp:1731-1733) -- `MOT[MMTrayY].fHasTray`
    // is checked TWICE (once here, again immediately below); almost certainly
    // a copy-paste artifact meant to check a third, different loader-side
    // motor once more. NOT corrected (e.g. to a distinct motor index).
    if(MOT[MMTrayZ].fHasTray ||                                                 //Loader
       MOT[MMTrayY].fHasTray ||
       MOT[MMTrayY].fHasTray)
    {
        iCurrStateATK[etLoader]=eATKTransfer;
    }
    else
    {
        iCurrStateATK[etLoader]=eATKReadyToLoad;
    }

    if(iLoadStateATK[etLoader]!=iCurrStateATK[etLoader])
    {
        iThisPortNo=ePortLoader;
        iThisPortStatus=iCurrStateATK[etLoader];
        EventReport(SECS_EVENT.PortStateUpdated);
        iLoadStateATK[etLoader]=iCurrStateATK[etLoader];
    }

    if(Sen[SnEmptyIsFull].IsOn() ||                                             //Empty
       (iTrayFeed && bCleanOutTrayEnd==false))
    {
        iCurrStateATK[etEmpty]=eATKReadyToUnload;
    }
    else if(MOT[MMEmptyZ].fHasTray ||
            MOT[MEmptyY].fHasTray ||
            MOT[MMEmpty_Car].fHasTray)
    {
        iCurrStateATK[etEmpty]=eATKTransfer;
    }
    else
    {
        iCurrStateATK[etEmpty]=eATKReadyToLoad;
    }

    if(iLoadStateATK[etEmpty]!=iCurrStateATK[etEmpty])
    {
        iThisPortNo=ePortEmpty;
        iThisPortStatus=iCurrStateATK[etEmpty];
        EventReport(SECS_EVENT.PortStateUpdated);
        iLoadStateATK[etEmpty]=iCurrStateATK[etEmpty];
    }

    if(Sen[SnColorIsFull].IsOn())                                               //Color
    {
        iCurrStateATK[etColor]=eATKReadyToUnload;
    }
    else if(MOT[MMColorZ].fHasTray ||
            MOT[MColorY].fHasTray ||
            MOT[MMColor_Car].fHasTray ||
            (iTrayFeed && bCleanOutTrayEnd==false))
    {
        iCurrStateATK[etColor]=eATKTransfer;
    }
    else
    {
        iCurrStateATK[etColor]=eATKReadyToLoad;
    }

    if(iLoadStateATK[etColor]!=iCurrStateATK[etColor])
    {
        iThisPortNo=ePortColor;
        iThisPortStatus=iCurrStateATK[etColor];
        EventReport(SECS_EVENT.PortStateUpdated);
        iLoadStateATK[etColor]=iCurrStateATK[etColor];
    }

    for(int i=etAuto1; i<=etAuto6; i++)                                         //Auto Tray
    {
        iAuto=i-etAuto1;
        if(AUTO3_IS_MAGAZINE==1 &&
           TestIF_File.iMagTraySource==0 &&
           iAuto==iMagAtAuto)
        {
            iCurrStateATK[i]=eATKNoState;
        }
        else if(Prod.iTrayType[iAuto]==tNotUse)
        {
            iCurrStateATK[i]=eATKNoState;
        }
        else if(iTrayFeed)
        {
            if(bCleanOutTrayEnd==true)
            {
                if(IniConfig.bP09TrayEndCanSelectTray)                          //Steven 20200317 : CleanOut後,可以選擇Tray End, 且要退的Tray要在工作檔設定
                {
                    if(TrayForm.Auto[iAuto].TrayEndRecv)
                    {
                        iCurrStateATK[i]=eATKReadyToUnload;
                    }
                    else
                    {
                        iCurrStateATK[i]=eATKTransfer;
                    }
                }
                else if((CUSTOMER_CODE==CC_KYEC_LEE &&                          //wei 20150821 KYEC在ART模式退Pass Bin
                         IniConfig.bA10_AutoReTest  &&
                         Prod.iIsPassT6[iAuto]==1     &&                        //Steven 20240105 : Prod.bIsPass --> Prod.iIsPassT6
                         (LastSet.iRunStartMode==rsmInitial_ART ||
                          LastSet.iRunStartMode==rsmContinuStart_ART ||
                          LastSet.iRunStartMode==rsmContinuRetest_ART)) ||
                        (CUSTOMER_CODE!=CC_KYEC_LEE &&
                         Prod.iIsFailT6[iAuto]==1))                             //Steven 20240105 : Prod.bIsPass --> Prod.iIsFailT6
                {
                    iCurrStateATK[i]=eATKReadyToUnload;
                }
                else
                {
                    iCurrStateATK[i]=eATKTransfer;
                }
            }
            else
            {
                iCurrStateATK[i]=eATKReadyToUnload;
            }
        }
        else if(Sen[SnAutoIsFull[iAuto]].IsOn())
        {
            iCurrStateATK[i]=eATKReadyToUnload;
        }
        else if(Sen[SnAutoTrayHasTray[iAuto]].IsOff())
        {
            iCurrStateATK[i]=eATKReadyToLoad;
        }
        else
        {
            iCurrStateATK[i]=eATKTransfer;
        }

        if(iLoadStateATK[i]!=iCurrStateATK[i])
        {
            iThisPortNo=ePortAuto1+iAuto;
            iThisPortStatus=iCurrStateATK[i];
            EventReport(SECS_EVENT.PortStateUpdated);
            iLoadStateATK[i]=iCurrStateATK[i];
        }
    }

    for(int i=etFix1; i<=etFix6; i++)                                           //Fix Tray
    {
        iAuto=i-etAuto1;                                                        //要扣掉Loader, Empty, Color
        if(Prod.iTrayType[iAuto]==tNotUse)
        {
            iCurrStateATK[i]=eATKNoState;
        }
        else if(Sen[SnFixedTrayDetect[iAuto]].IsOff())
        {
            iCurrStateATK[i]=eATKReadyToLoad;
        }
        else //if(Sen[SnFixedTrayDetect[iAuto]].IsOn())
        {
            if(MOT[iMMAuto[iAuto]].FullIC())
                iCurrStateATK[i]=eATKReadyToUnload;
            else
                iCurrStateATK[i]=eATKTransfer;
        }

        if(iLoadStateATK[i]!=iCurrStateATK[i])                                  //AI(general) 20260401 (RogerYang) : add Fix port CEID 284 report
        {
            iThisPortNo=ePortAuto1+iAuto;
            iThisPortStatus=iCurrStateATK[i];
            EventReport(SECS_EVENT.PortStateUpdated);
            iLoadStateATK[i]=iCurrStateATK[i];
        }
    }
}

// ============================================================================
//  APPEND BLOCK -- AI(W906-FW-QWKEY6) 20260824
//  The 3 MouseDown handlers the original wave omitted (see forms/fAGV.h
//  declarations for the trimmed-signature note). Golden AGV.cpp verbatim
//  bodies; N_INTEGER real (cmydef.h). Nothing above this line is edited.
// ============================================================================
void TfAGV::edAuto1CountMouseDown(TObject *Sender)                             // golden AGV.cpp:1133-1137
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 0, 20);
}
//---------------------------------------------------------------------------
void TfAGV::edE84_1_TP1MouseDown(TObject *Sender)                              // golden AGV.cpp:1139-1143
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 0, 300);
}
//---------------------------------------------------------------------------
void TfAGV::edAGVWorryingwattingtimeMouseDown(TObject *Sender)                 // golden AGV.cpp:1309-1313
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 0, 3600);
}
//---------------------------------------------------------------------------
