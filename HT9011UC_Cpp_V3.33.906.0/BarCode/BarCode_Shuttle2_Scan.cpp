// =============================================================================
//  BarCode/BarCode_Shuttle2_Scan.cpp  --  Shuttle-2 side small scan/trigger/
//                                          float-check functions (TfBarCode)
//
//  See BarCode_Shuttle2_Scan.h for full scope/quirk/gate documentation.
//  Translator: AI(W5-BarCode-Shuttle2Scan) 20260711
// =============================================================================
#include "BarCode_Shuttle2_Scan.h"

#include "aHotPlateSubstrate.h"   // InArmSuck / BLCarryKit / BRCarryKit (TMyKitSuck)
#include "cprod.h"                // Prod / Offset (RUN_OFFSET) / TestIF_File (SYSTEM_TEST_IF)
#include "Config.h"                // IniConfig (HT9045_CONFIG)
#include "cmydef.h"                // NULL_IC / HAS_NULL_IC / asBarCodeErrorSend / K_RETRY / K_SKIP /
                                    // REALLY / DUMMY / MInShuttle2 / iSH1_1BarcodePosition /
                                    // iSH1_2BarcodePosition / iSH2_1BarcodePosition / iSH2_2BarcodePosition
#include "MachineType.h"           // eAMD
#include "Motor/mymotor.h"         // MOT[] (TTrayMotor) / MotorMove / fCanMoveR
#include "canary_support.h"        // ShowMyMessage / ShowErrorMessage / LastSet
#include "atester_shims.h"         // fContact (TfContactShim::IsRun2DCheck)
#include "acarry.h"                // DoInOutARM_SHT_MoveSafe (real W6.5 body)

// ---------------------------------------------------------------------------
//  Shared TfBarCode-member state -- CANONICAL definitions (see .h banner).
// ---------------------------------------------------------------------------
const int iBarCode2_1 = 2;   // golden BarCode.cpp:219 (TfBarCode::FormCreate)
const int iBarCode2_2 = 3;   // golden BarCode.cpp:220 (TfBarCode::FormCreate)

int  iInitialBarcodeInShuttle2Task  = 0;   // golden BarCode.h:787 (0 == not yet initialised)
int  iInitialBarcodeOutShuttle2Task = 0;   // golden BarCode.h:789
int  iShuttleFloatCheck2Task        = 0;   // golden BarCode.h:883

bool bBarcodeStartDelay   [BAR_CODE_COUNT] = {false};   // golden BarCode.h:738
bool bBarcodeDataSaveReady[BAR_CODE_COUNT] = {false};   // golden BarCode.h:740
int  iShtDupilcateFailCnt [2]              = {0, 0};    // golden BarCode.h:812
// bCCDcheckShuttle[2] -- NOT defined here (INTEGRATE FIX 20260711): it is
// ALREADY a real global with external linkage in cmydef.cpp:4358 (extern decl
// cmydef.h:4184), reused across the whole tree.  This unit's own header
// (BarCode_Shuttle2_Scan.h) re-declares it `extern` (harmless -- identical
// `extern` redeclarations of the same global are legal C++), but defining it
// a SECOND time here would be a genuine multiple-definition link error once
// this .cpp joins ht9045_sm alongside cmydef.cpp.  See this unit's own
// translate report item 6 + the W5-BarCode INTEGRATE report for the discovery.

TQPF_Timer BarcodePosDelay      [2];              // golden BarCode.h:871
TQPF_Timer BarcodeExposureDelay [2];              // golden BarCode.h:870
TQPF_Timer BarcodeDelay         [BAR_CODE_COUNT]; // golden BarCode.h:869

std::map<AnsiString, AnsiString> map2DList;                  // golden BarCode.h:852
std::map<AnsiString, AnsiString>::iterator map2DIter;        // golden BarCode.h:853

// ---------------------------------------------------------------------------
//  Gated cross-module leaves.  Golden home noted per function; each is a
//  TfBarCode method (or VCL widget deref) that lives OUTSIDE this wave's
//  scope -- either the still-untranslated shared BarCode.cpp core (~11966
//  golden lines, MIGRATION_ROADMAP DEFERRED "BarCode/{...}.cpp"), the not-yet-
//  translated mymessbox.h/.cpp, or a VCL grid/form (W7).  Each mirrors the
//  golden signature closely enough to drop in later; conservative-default
//  rationale documented per function.  TODO(W5-BarCode-core / W7): replace
//  with real bodies when those TUs land.
// ---------------------------------------------------------------------------
namespace
{
    // golden BarCode.cpp:2097-2362 (TfBarCode::Barcode_StartScan_In) -- starts
    // the TComm serial-reader channel for one site and (if data already
    // buffered) decodes it into cDeviceInf.  Conservative: false ("not read
    // yet this tick") -- keeps the caller's retry/timeout ladder faithful,
    // never claims a false-positive read.
    bool Gated_Barcode_StartScan_In(int /*BarCodeIndex*/, int /*iXpos*/,
                                     AnsiString & /*cDeviceInf*/,
                                     const AnsiString & /*cLastDeviceInf*/)
    {
        return false;
    }

    // golden BarCode.cpp (TfBarCode::Barcode_1StartScan) -- single-channel
    // start used by the OutShuttle scan (simpler than Barcode_StartScan_In,
    // no last-string dup-check).  Same conservative default.
    bool Gated_Barcode_1StartScan(int /*BarCodeIndex*/, int /*MotIndex*/)
    {
        return false;
    }

    // golden BarCode.cpp:2364- (TfBarCode::TurnOffInspection) -- sends a CCD
    // "trigger off" command over TComm.  Pure hardware side effect, no state
    // consumed elsewhere in this SM; no-op.
    void Gated_TurnOffInspection(int /*BarCodeIndex*/)
    {
    }

    // golden BarCode.cpp:5903-5974 (TfBarCode::AddCCDCommunicationLog) --
    // appends to a UI comm-log memo.  Pure log side-channel; no-op.
    void Gated_AddCCDCommunicationLog(int /*index*/, const AnsiString & /*Str*/)
    {
    }

    // golden BarCode.h:919 (TfBarCode::DoCheckSHT2DIDStatus) -- Korea-customer
    // "all sites failed 2DID twice -> alarm" recheck (Isaac 20180329).
    // Conservative: false (no extra Korea-specific rescan requested) -- this
    // customer-specific escalation path is simply not modelled offline.
    bool Gated_DoCheckSHT2DIDStatus(int /*SHT*/)
    {
        return false;
    }

    // golden BarCode.h:858 (TfBarCode::DoBarcodeCount) -- per-channel
    // Barcode-rate tally.  Return value is ignored at every call site in this
    // file (bare statement calls); no state consumed by this SM's own control
    // flow; no-op.
    void Gated_DoBarcodeCount()
    {
    }

    // golden BarCode.h:822 (TfBarCode::InitSht2DCodeComPort) -- resets the
    // TComm com port for a shuttle side on retry.  Hardware side effect only;
    // no-op.
    void Gated_InitSht2DCodeComPort(int /*iStep*/)
    {
    }

    // golden mymessbox.h:54 (ShowMyMessageBox_YES_SKIP) -- blocking VCL
    // Yes/Skip prompt.  Conservative default: NOT "Yes" (take the finish/skip
    // path) so an unattended offline run doesn't loop forever waiting for an
    // operator click.
    int Gated_ShowMyMessageBox_YES_SKIP(const AnsiString & /*S1*/, const AnsiString & /*S2*/)
    {
        return 0;
    }

    // golden BarCode.h:145 mtBarcodeInSh (TTMyTray* grid widget on TfBarCode
    // form) -- pure UI mirror of data already tracked in BLCarryKit.cDeviceInf;
    // no effect on SM control flow.  No-op (VCL form not translated -- W7).
    void Gated_SetBarcodeGridCell(int /*BarCodeIndex*/, int /*iCol1Based*/, const AnsiString & /*Text*/)
    {
    }

    // golden fLotInfo->mtBarcodeInShLotInfo (separate VCL form TfLotInfo) --
    // same UI-mirror rationale.  No-op (W7).
    void Gated_SetLotInfoBarcodeGridCell(int /*iCol1Based*/, const AnsiString & /*Text*/)
    {
    }
}

// =============================================================================
//  BarCode_Sh2_InitialBarcodeScanInShuttle2(bool bClear2DID=true)
//  golden BarCode_Sh2.cpp:31-79                                    //Steven 20160823
// =============================================================================
void BarCode_Sh2_InitialBarcodeScanInShuttle2(bool bClear2DID)
{
    AnsiString Log;
    int iBarCodeRowA = iBarCode2_1;
    int iBarCodeRowB = iBarCode2_2;
    iInitialBarcodeInShuttle2Task = 1;
    iShtDupilcateFailCnt[1] = 0;

    if (InArmSuck.iShtRow == 1)                     //Steven 20240227 : Shuttle使用一排的
    {
        if (TestIF_File.iSelectUseCCDSh2 == 1)
        {
            iBarCodeRowA = iBarCode2_1;
            iBarCodeRowB = iBarCode2_2;
        }
        else
        {
            iBarCodeRowA = iBarCode2_2;
            iBarCodeRowB = iBarCode2_1;
        }
    }

    if (bClear2DID)
    {
        for (int i = 0; i < BLCarryKit.iMaxRow; i++)
        {
            for (int j = 0; j < BLCarryKit.iMaxCol; j++)
            {
                if (BLCarryKit.Item[i][j] != NULL_IC &&
                    BLCarryKit.Item[i][j] != HAS_NULL_IC &&
                    BLCarryKit.cDeviceInf[i][j] != "")
                {
                    map2DIter = map2DList.find(BLCarryKit.cDeviceInf[i][j]);
                    if (map2DList.size() != 0 && map2DIter != map2DList.end())
                    {
                        Log.sprintf("Delete, Shuttle2 %d_%d, Code, %s", i + 1, j + 1, BLCarryKit.cDeviceInf[i][j]);
                        if (i == 0)
                            Gated_AddCCDCommunicationLog(iBarCodeRowA, Log);
                        else
                            Gated_AddCCDCommunicationLog(iBarCodeRowB, Log);

                        map2DList.erase(map2DIter);
                    }
                }
            }
        }
    }
}

// =============================================================================
//  BarCode_Sh2_InitialBarcodeScanOutShuttle2()
//  golden BarCode.cpp:612-616 (NOTE: golden home is BarCode.cpp, the shared
//  core file, not BarCode_Sh2.cpp -- verified by grep; see translate report
//  item 8 "discrepancy from prompt scoping").
// =============================================================================
void BarCode_Sh2_InitialBarcodeScanOutShuttle2()
{
    iInitialBarcodeOutShuttle2Task = 1;
    iShtDupilcateFailCnt[1] = 0;
}

// =============================================================================
//  BarCode_Sh2_DoBarcodeTriggerInShuttle_2()
//  golden BarCode_Sh2.cpp:2474-3038                                //Steven 20151225
// =============================================================================
bool BarCode_Sh2_DoBarcodeTriggerInShuttle_2()
{
    static int iNowCheckStep = 0, iNowMoveStep = 0;
    static int iRetryCount = 0, iStepCount = 0;
    static int i2DIDRetryCTSH2 = 0;                 //Isaac 20200206 : 2DID All site fail recheck twice and alarm
    static bool bBarcodeNum[BAR_CODE_COUNT] = {false};
    static bool bHasError = false;

    int &Task = iInitialBarcodeInShuttle2Task;
    int iSht = 1;
    int pos = 0, ret = 0;
    AnsiString cLastString;

    switch (Task)
    {
        case 1:
            iNowMoveStep = InArmSuck.iShtCol - 1;                  //遞減方式檢查
            iNowCheckStep = iNowMoveStep;                          //一開始移動的位置跟檢查位置是一樣的
            bBarcodeStartDelay[iBarCode2_1] = true;
            bBarcodeStartDelay[iBarCode2_2] = true;

            for (int i = 0; i < BLCarryKit.iMaxCol; i++)           //清空畫面
            {
                Gated_SetBarcodeGridCell(iBarCode2_1, i + 1, "");
                Gated_SetBarcodeGridCell(iBarCode2_2, i + 1, "");
                if (TestIF_File.i2DIDFormat == eAMD)                //JerryYang 20200422 2DID format選項改用下拉選單
                {
                    Gated_SetLotInfoBarcodeGridCell(i + 1, "");
                }
            }
            bBarcodeNum[iBarCode2_1] = true;
            bBarcodeNum[iBarCode2_2] = true;
            BLCarryKit.cDeviceInf[0][iNowMoveStep] = "";
            BLCarryKit.cDeviceInf[1][iNowMoveStep] = "";
            bHasError = false;
            iStepCount = 0;
            iRetryCount = 0;
            i2DIDRetryCTSH2 = 0;

            Task = 1000;
        case 1000:
            if ((BLCarryKit.Item[0][iNowMoveStep] == HAS_NULL_IC || BLCarryKit.Item[0][iNowMoveStep] == NULL_IC) &&
                (BLCarryKit.Item[1][iNowMoveStep] == HAS_NULL_IC || BLCarryKit.Item[1][iNowMoveStep] == NULL_IC))
            {
                Task = 1200;
                break;
            }

            if (InArmSuck.iShtCol == 1)
            {
                pos = Prod.iInSHBarCodeDetectPos1x1[1];
            }
            else if (InArmSuck.iShtCol == 2)
            {
                pos = Prod.iInSHBarCodeDetectPos1x2[1][iNowMoveStep];
            }
            else if (InArmSuck.iShtCol == 3)                       //ChungHung 20140115 add for 2x3_6
            {
                pos = Prod.iInSHBarCodeDetectPos2x3[1][iNowMoveStep];
            }
            else if (InArmSuck.iShtCol == 4)
            {
                pos = Prod.iInSHBarCodeDetectPos1x4[1][iNowMoveStep];
            }
            else if (InArmSuck.iShtCol == 5)                       //Steven 20221027 : Add for 2x5
            {
                pos = Prod.iInSHBarCodeDetectPos2x5[1][iNowMoveStep];
            }
            else if (InArmSuck.iShtCol == 6)                       //Steven 20170111 : fixed for 2x6
            {
                pos = Prod.iInSHBarCodeDetectPos2x6[1][iNowMoveStep];
            }
            else if (InArmSuck.iShtCol == 8)
            {
                pos = Prod.iInSHBarCodeDetectPos2x8[1][iNowMoveStep];
            }
            else
            {
                ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "DoBarcodeTriggerInShuttle_2");
            }

            //AI(W5-BarCode-Shuttle2Scan) 20260711: golden `pos+=Offset.iSHLeft2D[1];`
            //(pos int, iSHLeft2D double) truncates the SUM toward zero, not the
            //offset alone -- written as `pos=(int)(...)` here (not `pos+=(int)x`)
            //so a negative offset with a fractional part truncates identically to
            //golden (e.g. pos=100,offset=-2.7: golden->97, "+=(int)x" would give 98).
            pos = (int)(pos + Offset.iSHLeft2D[1]);

            if (MOT[MInShuttle2].MotorMove(pos))
            {
                if (BLCarryKit.Item[0][iNowCheckStep] == HAS_NULL_IC ||     //判斷要不要讀Code
                    BLCarryKit.Item[0][iNowCheckStep] == NULL_IC)
                {
                    bBarcodeNum[iBarCode2_1] = true;
                }
                else
                {
                    bBarcodeStartDelay[iBarCode2_1] = true;
                    bBarcodeNum[iBarCode2_1] = false;
                }

                if (InArmSuck.iShtRow == 2)
                {
                    if (BLCarryKit.Item[1][iNowCheckStep] == HAS_NULL_IC ||
                        BLCarryKit.Item[1][iNowCheckStep] == NULL_IC)
                    {
                        bBarcodeNum[iBarCode2_2] = true;
                    }
                    else
                    {
                        bBarcodeStartDelay[iBarCode2_2] = true;
                        bBarcodeNum[iBarCode2_2] = false;
                    }
                }
                else
                {
                    bBarcodeNum[iBarCode2_2] = true;
                }

                if (iStepCount == 0)
                {
                    BarcodePosDelay[iSht].SetMSAndOn((DWORD)TestIF_File.iBarCodePos1Delay);
                    iStepCount++;
                }
                else
                {
                    BarcodePosDelay[iSht].SetMSAndOn((DWORD)TestIF_File.iBarCodePosDelay);
                    iStepCount++;
                }
                Task = 1120;
            }
            break;
        case 1120:
            if (BarcodePosDelay[iSht].Off())                          //到位Delay
            {
                BarcodeExposureDelay[iSht].SetMSAndOn((DWORD)TestIF_File.i2DTriggerTime);
                if (bBarcodeNum[iBarCode2_1] == false)
                {
                    if (iNowCheckStep == 0)
                    {
                        cLastString = "(NULL)";
                    }
                    else
                    {
                        cLastString = BLCarryKit.cDeviceInf[0][iNowCheckStep - 1];
                    }
                    iSH2_1BarcodePosition = iNowCheckStep;
                    Gated_Barcode_StartScan_In(iBarCode2_1, iNowCheckStep, BLCarryKit.cDeviceInf[0][iNowCheckStep], cLastString);
                }

                if (bBarcodeNum[iBarCode2_2] == false)
                {
                    if (iNowCheckStep == 0)
                    {
                        cLastString = "(NULL)";
                    }
                    else
                    {
                        cLastString = BLCarryKit.cDeviceInf[1][iNowCheckStep - 1];
                    }
                    iSH2_2BarcodePosition = iNowCheckStep;
                    Gated_Barcode_StartScan_In(iBarCode2_2, iNowCheckStep, BLCarryKit.cDeviceInf[1][iNowCheckStep], cLastString);
                }

                Task = 1150;
            }
            break;
        case 1150:
            if (BarcodeExposureDelay[iSht].Off())                     //拍照Delay
            {
                BarcodeDelay[iSht].SetMSAndOn((DWORD)TestIF_File.iBarCodeDelay);       //這個是讀取Time Out用的
                Task = 1200;
            }
            break;
        case 1200:
            iRetryCount = 0;
            iNowMoveStep--;

            if (iNowMoveStep >= 0 && iNowMoveStep < InArmSuck.iShtCol)
            {
                if ((BLCarryKit.Item[0][iNowMoveStep] == HAS_NULL_IC || BLCarryKit.Item[0][iNowMoveStep] == NULL_IC) &&
                    (BLCarryKit.Item[1][iNowMoveStep] == HAS_NULL_IC || BLCarryKit.Item[1][iNowMoveStep] == NULL_IC))    //直接判斷要不要移動馬達
                {
                    Task = 1200;
                }
                else
                {
                    Task = 1300;
                }
            }
            else
            {
                if (iNowMoveStep != iNowCheckStep)                     //最後一步
                {
                    Task = 1350;
                }
                else
                {
                    Task = 2000;
                }
            }
            break;
        case 1300:                                                     //偷跑
            if (InArmSuck.iShtCol == 1)
            {
                pos = Prod.iInSHBarCodeDetectPos1x1[1];
            }
            else if (InArmSuck.iShtCol == 2)
            {
                pos = Prod.iInSHBarCodeDetectPos1x2[1][iNowMoveStep];
            }
            else if (InArmSuck.iShtCol == 3)                            //ChungHung 20140115 add for 2x3_6
            {
                pos = Prod.iInSHBarCodeDetectPos2x3[1][iNowMoveStep];
            }
            else if (InArmSuck.iShtCol == 4)
            {
                pos = Prod.iInSHBarCodeDetectPos1x4[1][iNowMoveStep];
            }
            else if (InArmSuck.iShtCol == 5)                            //Steven 20221027 : Add for 2x5
            {
                pos = Prod.iInSHBarCodeDetectPos2x5[1][iNowMoveStep];
            }
            else if (InArmSuck.iShtCol == 6)                            //Steven 20170111 : fixed for 2x6
            {
                pos = Prod.iInSHBarCodeDetectPos2x6[1][iNowMoveStep];
            }
            else if (InArmSuck.iShtCol == 8)
            {
                pos = Prod.iInSHBarCodeDetectPos2x8[1][iNowMoveStep];
            }
            else
            {
                ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "DoBarcodeTriggerInShuttle_2");
            }

            pos = (int)(pos + Offset.iSHLeft2D[1]);   // see 1000's note on truncation order

            if (MOT[MInShuttle2].MotorMove(pos))
            {
                Task = 1350;
            }
            break;
        case 1350:
            if (bBarcodeNum[iBarCode2_1] == false)
            {
                if (iNowCheckStep == 0)
                {
                    cLastString = "(NULL)";
                }
                else
                {
                    cLastString = BLCarryKit.cDeviceInf[0][iNowCheckStep - 1];
                }
                bBarcodeNum[iBarCode2_1] = Gated_Barcode_StartScan_In(iBarCode2_1, iNowCheckStep, BLCarryKit.cDeviceInf[0][iNowCheckStep], cLastString);
            }

            if (bBarcodeNum[iBarCode2_2] == false)
            {
                if (iNowCheckStep == 0)
                {
                    cLastString = "(NULL)";
                }
                else
                {
                    cLastString = BLCarryKit.cDeviceInf[1][iNowCheckStep - 1];
                }
                bBarcodeNum[iBarCode2_2] = Gated_Barcode_StartScan_In(iBarCode2_2, iNowCheckStep, BLCarryKit.cDeviceInf[1][iNowCheckStep], cLastString);
            }

            if (bBarcodeNum[iBarCode2_1] == true && bBarcodeNum[iBarCode2_2] == true)     //讀取完成
            {
                if (BLCarryKit.cDeviceInf[0][iNowCheckStep] == asBarCodeErrorSend ||           //wei 20160318 Barcode Error依客戶設定
                    BLCarryKit.cDeviceInf[1][iNowCheckStep] == asBarCodeErrorSend ||
                    BLCarryKit.cDeviceInf[0][iNowCheckStep] == asBarCodeErrorCheckSum ||
                    BLCarryKit.cDeviceInf[1][iNowCheckStep] == asBarCodeErrorCheckSum)     //KaiHuang 20200826 : Add CheckSum
                {
                    bHasError = true;
                }

                Task = 2000;
            }
            else if (BarcodeDelay[iSht].Off())                          //讀取超時
            {
                if (bBarcodeNum[iBarCode2_1] == false)
                {
                    bHasError = true;
                    Gated_TurnOffInspection(iBarCode2_1);
                    BLCarryKit.cDeviceInf[0][iNowCheckStep] = asBarCodeErrorSend; //wei 20160318 Barcode Error依客戶設定
                    Gated_SetBarcodeGridCell(iBarCode2_1, iNowCheckStep + 1, asBarCodeErrorSend);
                    if (TestIF_File.i2DIDFormat == eAMD)                //JerryYang 20200422 2DID format選項改用下拉選單
                    {
                        Gated_SetLotInfoBarcodeGridCell(iNowCheckStep + 1, asBarCodeErrorSend);
                    }
                }

                if (bBarcodeNum[iBarCode2_2] == false)
                {
                    bHasError = true;
                    Gated_TurnOffInspection(iBarCode2_2);
                    BLCarryKit.cDeviceInf[1][iNowCheckStep] = asBarCodeErrorSend; //wei 20160318 Barcode Error依客戶設定
                    Gated_SetBarcodeGridCell(iBarCode2_2, iNowCheckStep + 1, asBarCodeErrorSend);
                }
                Task = 2000;
            }
            break;
        case 2000:
            iNowCheckStep = iNowMoveStep;
            if (iNowMoveStep == -1)
            {
                if (TestIF_File.bNoCodeDeviceAutoSkip == false && bHasError == true)
                {
                    Task = 3000;
                }
                else
                {
                    if (IniConfig.bKoreaFunction == true)               //Isaac 20200206 : 2DID All site fail recheck twice and alarm
                    {
                        if (Gated_DoCheckSHT2DIDStatus(1) == true)
                        {
                            Task = 5000;                                //例外，要重掃
                            break;
                        }
                    }
                    Gated_DoBarcodeCount();                             //wei 20160318
                    Task = 1;
                    return true;
                }
            }
            else
            {
                Task = 1000;
            }
            break;
        case 3000:                                                      //例外處理
            iNowCheckStep = InArmSuck.iShtCol;                          //遞減方式檢查, 故意+1, 因為到下一個Task會先--
            Task = 3100;
        case 3100:
            iRetryCount = 0;
            iNowCheckStep--;

            if (iNowCheckStep >= 0 && iNowCheckStep < InArmSuck.iShtCol)
            {
                if (BLCarryKit.cDeviceInf[0][iNowCheckStep] == asBarCodeErrorSend ||    //直接判斷要不要移動馬達          //wei 20160318 Barcode Error依客戶設定
                    BLCarryKit.cDeviceInf[1][iNowCheckStep] == asBarCodeErrorSend)
                {
                    Task = 3200;
                }
                else
                {
                    Task = 3100;
                }
            }
            else
            {
                if (IniConfig.bKoreaFunction == true)                   //Isaac 20200206 : 2DID All site fail recheck twice and alarm
                {
                    if (Gated_DoCheckSHT2DIDStatus(1) == true)
                    {
                        Task = 5000;                                    //例外，要重掃
                        break;
                    }
                }
                Gated_DoBarcodeCount();                                 //wei 20160318
                Task = 1;
                return true;
            }
            break;
        case 3200: //重置參數
            if (BLCarryKit.cDeviceInf[0][iNowCheckStep] == asBarCodeErrorSend)     //wei 20160318 Barcode Error依客戶設定
            {
                Gated_SetBarcodeGridCell(iBarCode2_1, iNowCheckStep + 1, "");
                if (TestIF_File.i2DIDFormat == eAMD)                    //JerryYang 20200422 2DID format選項改用下拉選單
                {
                    Gated_SetLotInfoBarcodeGridCell(iNowCheckStep + 1, "");
                }
                BLCarryKit.cDeviceInf[0][iNowCheckStep] = "";
                bBarcodeNum[iBarCode2_1] = false;
                bBarcodeStartDelay[iBarCode2_1] = true;
            }

            if (BLCarryKit.cDeviceInf[1][iNowCheckStep] == asBarCodeErrorSend)     //wei 20160318 Barcode Error依客戶設定
            {
                Gated_SetBarcodeGridCell(iBarCode2_2, iNowCheckStep + 1, "");
                BLCarryKit.cDeviceInf[1][iNowCheckStep] = "";
                bBarcodeNum[iBarCode2_2] = false;
                bBarcodeStartDelay[iBarCode2_2] = true;
            }
            Task = 3300;
        case 3300:                                                       //移動馬達
            if (InArmSuck.iShtCol == 1)
            {
                pos = Prod.iInSHBarCodeDetectPos1x1[1];
            }
            else if (InArmSuck.iShtCol == 2)
            {
                pos = Prod.iInSHBarCodeDetectPos1x2[1][iNowMoveStep];
            }
            else if (InArmSuck.iShtCol == 3)                             //ChungHung 20140115 add for 2x3_6
            {
                pos = Prod.iInSHBarCodeDetectPos2x3[1][iNowMoveStep];
            }
            else if (InArmSuck.iShtCol == 4)
            {
                pos = Prod.iInSHBarCodeDetectPos1x4[1][iNowMoveStep];
            }
            else if (InArmSuck.iShtCol == 5)                             //Steven 20221027 : Add for 2x5
            {
                pos = Prod.iInSHBarCodeDetectPos2x5[1][iNowMoveStep];
            }
            else if (InArmSuck.iShtCol == 6)                             //Steven 20170111 : fixed for 2x6
            {
                pos = Prod.iInSHBarCodeDetectPos2x6[1][iNowMoveStep];
            }
            else if (InArmSuck.iShtCol == 8)
            {
                pos = Prod.iInSHBarCodeDetectPos2x8[1][iNowMoveStep];
            }
            else
            {
                ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "DoBarcodeTriggerInShuttle_2");
            }

            pos = (int)(pos + Offset.iSHLeft2D[1]);   // see 1000's note on truncation order

            if (MOT[MInShuttle2].MotorMove(pos))
            {
                BarcodePosDelay[iSht].SetMSAndOn((DWORD)TestIF_File.iBarCodePosDelay);
                Task = 3350;
            }
            break;
        case 3350:
            if (BarcodePosDelay[iSht].Off())
            {
                BarcodeDelay[iSht].SetMSAndOn((DWORD)TestIF_File.iBarCodeDelay);
                Task = 3400;
            }
            break;
        case 3400:                                                       //讀取2D Code
            if (bBarcodeNum[iBarCode2_1] == false)
            {
                //AI(W5-BarCode-Shuttle2Scan) 20260711: golden BarCode_Sh2.cpp:2910
                //writes iSH1_1BarcodePosition (the SHUTTLE-1 position global) here,
                //not iSH2_1BarcodePosition -- inconsistent with case 1120 above
                //(golden :2615) and every other use in this file, which correctly
                //use the SH2 globals.  Looks like a golden copy-paste-from-Shuttle-1
                //bug.  Reproduced verbatim (NOT "fixed") per this project's
                //Front/Rear-asymmetry precedent.
                iSH1_1BarcodePosition = iNowCheckStep;
                if (iNowCheckStep == 0)
                {
                    cLastString = "(NULL)";
                }
                else
                {
                    cLastString = BLCarryKit.cDeviceInf[0][iNowCheckStep - 1];
                }
                bBarcodeNum[iBarCode2_1] = Gated_Barcode_StartScan_In(iBarCode2_1, iNowCheckStep, BLCarryKit.cDeviceInf[0][iNowCheckStep], cLastString);

                if (bBarcodeNum[iBarCode2_1] == true)
                {
                    bBarcodeStartDelay[iBarCode2_1] = true;
                    bBarcodeDataSaveReady[iBarCode2_1] = false;
                }
            }

            if (bBarcodeNum[iBarCode2_2] == false)
            {
                //AI(W5-BarCode-Shuttle2Scan) 20260711: same golden quirk as above
                //(golden :2930) -- writes iSH1_2BarcodePosition, not
                //iSH2_2BarcodePosition.  Reproduced verbatim.
                iSH1_2BarcodePosition = iNowCheckStep;
                if (iNowCheckStep == 0)
                {
                    cLastString = "(NULL)";
                }
                else
                {
                    cLastString = BLCarryKit.cDeviceInf[1][iNowCheckStep - 1];
                }
                bBarcodeNum[iBarCode2_2] = Gated_Barcode_StartScan_In(iBarCode2_2, iNowCheckStep, BLCarryKit.cDeviceInf[1][iNowCheckStep], cLastString);

                if (bBarcodeNum[iBarCode2_2] == true)
                {
                    bBarcodeStartDelay[iBarCode2_2] = true;
                    bBarcodeDataSaveReady[iBarCode2_2] = false;
                }
            }

            if (bBarcodeNum[iBarCode2_1] == true && bBarcodeNum[iBarCode2_2] == true)
            {
                Task = 3100;
            }
            else if (BarcodeDelay[iSht].Off())
            {
                Gated_TurnOffInspection(iBarCode2_1);
                Gated_TurnOffInspection(iBarCode2_2);
                #ifdef SOFT_SIMULTE
                ret = K_SKIP;
                #else
                if (LastSet.iRealDummy == DUMMY)
                    ret = K_SKIP;
                else
                {
                    if (iRetryCount >= TestIF_File.iBarcodeRetryCount)      //Steven 20151221 : 讀不到Code時,自動Retry的次數
                    {
                        if (TestIF_File.bNoCodeDeviceAutoSkip == true &&
                            fContact->IsRun2DCheck() == false)              //JerryYang 20250220 : 2DID硬體順序檢查功能             //Steven 20151221 : 讀不到Code時,自動Skip跳下一顆
                            ret = K_SKIP;
                        else if (fContact->IsRun2DCheck() == true)          //JerryYang 20250220 : 2DID硬體順序檢查功能
                            ret = ShowErrorMessage("JAM0496", K_RETRY, MInShuttle2);      //In Shuttle2 Barcode Error
                        else
                            ret = ShowErrorMessage("JAM0461", K_RETRY | K_SKIP, MInShuttle2);      //In Shuttle2 Barcode Error
                        iRetryCount = -1;
                    }
                    else
                    {
                        ret = K_RETRY;
                    }
                }
                #endif
                if (ret == K_RETRY)
                {
                    iRetryCount++;
                    Task = 3300;
                    return false;
                }
                else
                {
                    iRetryCount = 0;
                    if (bBarcodeNum[iBarCode2_1] == false)
                    {
                        BLCarryKit.cDeviceInf[0][iNowCheckStep] = asBarCodeErrorSend;         //wei 20160318 Barcode Error依客戶設定
                        Gated_SetBarcodeGridCell(iBarCode2_1, iNowCheckStep + 1, asBarCodeErrorSend);
                        if (TestIF_File.i2DIDFormat == eAMD)                //JerryYang 20200422 2DID format選項改用下拉選單
                        {
                            Gated_SetLotInfoBarcodeGridCell(iNowCheckStep + 1, asBarCodeErrorSend);
                        }
                    }

                    if (bBarcodeNum[iBarCode2_2] == false)
                    {
                        BLCarryKit.cDeviceInf[1][iNowCheckStep] = asBarCodeErrorSend;         //wei 20160318 Barcode Error依客戶設定
                        Gated_SetBarcodeGridCell(iBarCode2_2, iNowCheckStep + 1, asBarCodeErrorSend);
                    }
                    Task = 3100;
                }
            }
            break;
        case 5000:                                                        //rescan    //Isaac 20200206 : 2DID All site fail recheck twice and alarm
            if (IniConfig.bF21InOutArmZMotorPrivate)
            {
                if (DoInOutARM_SHT_MoveSafe(1))                            //kevin 20161005 SHUTTLE 1 移動安全保護
                    return false;
            }

            MOT[MInShuttle2].fCanMoveR = true;
            if (MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iLeft))
            {
                i2DIDRetryCTSH2++;
                if (i2DIDRetryCTSH2 > 1)
                {
                    i2DIDRetryCTSH2 = 0;
                    ret = Gated_ShowMyMessageBox_YES_SKIP("All sites 2DID Error On Shuttle 2!! Please check!", "Retry Again?");
                    if (ret == 1)                                          //Yes
                    {
                        Task = 1;
                    }
                    else
                    {
                        Gated_DoBarcodeCount();                            //wei 20160318
                        Task = 1;
                        return true;
                    }
                }
            }
            break;
    }
    return false;
}

// =============================================================================
//  BarCode_Sh2_DoBarcodeScanOutShuttle_2()
//  golden BarCode_Sh2.cpp:4262-4399
// =============================================================================
bool BarCode_Sh2_DoBarcodeScanOutShuttle_2()
{
    static int iNowCheckStep = 0;
    static bool bBarcodeNum[BAR_CODE_COUNT] = {false};

    int &Task = iInitialBarcodeOutShuttle2Task;
    int pos = 0;

    switch (Task)
    {
        case 1:
            iNowCheckStep = InArmSuck.iShtCol - 1;                  //遞減方式檢查
            bBarcodeStartDelay[iBarCode2_1] = true;
            if (InArmSuck.iShtRow == 2)
                bBarcodeStartDelay[iBarCode2_2] = true;
            Task = 1000;
            // golden :4278 falls through without a `break;` (verbatim -- not added here)
        case 1000:
            if (iNowCheckStep >= 0 && iNowCheckStep < InArmSuck.iShtCol)
            {
                if (InArmSuck.iShtCol == 1)
                {
                    pos = Prod.iOutSHBarCodeDetectPos1x1[1];
                }
                else if (InArmSuck.iShtCol == 2)
                {
                    pos = Prod.iOutSHBarCodeDetectPos1x2[1][iNowCheckStep];
                }
                else if (InArmSuck.iShtCol == 3)                     //ChungHung 20140115 add for 2x3_6
                {
                    pos = Prod.iOutSHBarCodeDetectPos2x3[1][iNowCheckStep];
                }
                else if (InArmSuck.iShtCol == 4)
                {
                    pos = Prod.iOutSHBarCodeDetectPos1x4[1][iNowCheckStep];
                }
                else if (InArmSuck.iShtCol == 5)                     //Steven 20221027 : Add for 2x5
                {
                    // golden BarCode_Sh2.cpp:4300 verbatim uses row-index [0] here
                    // (every other iShtCol branch, both here and in the InShuttle
                    // detect-pos ladders above, uses [1]).  Preserved as-is -- looks
                    // like the same class of golden copy-paste inconsistency noted
                    // at case 3400, not "fixed" here.
                    pos = Prod.iOutSHBarCodeDetectPos2x5[0][iNowCheckStep];
                }
                else if (InArmSuck.iShtCol == 6)
                {
                    pos = Prod.iOutSHBarCodeDetectPos2x6[1][iNowCheckStep];
                }
                else if (InArmSuck.iShtCol == 8)
                {
                    pos = Prod.iOutSHBarCodeDetectPos2x8[1][iNowCheckStep];
                }
                else
                {
                    ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "DoBarcodeScanOutShuttle_2");
                }

                if (MOT[MInShuttle2].MotorMove(pos))
                {
                    bBarcodeNum[iBarCode2_1] = false;
                    bBarcodeNum[iBarCode2_2] = false;
                    Task = 1100;
                }
            }
            else
            {
                Task = 2000;
            }
            break;
        case 1100:                                                    //雙排時,Shuttle 2, Row A 使用第4隻CCD; Row B 使用第3隻CCD
            if (InArmSuck.iShtRow == 2)
            {
                if (BRCarryKit.Item[0][iNowCheckStep] == HAS_NULL_IC ||
                    BRCarryKit.Item[0][iNowCheckStep] == NULL_IC)
                {
                    bBarcodeNum[iBarCode2_2] = true;
                }
                else if (bBarcodeNum[iBarCode2_2] == false &&
                         Gated_Barcode_1StartScan(3, MOutShuttle2) == true)
                {
                    bBarcodeStartDelay[iBarCode2_2] = true;
                    bBarcodeDataSaveReady[iBarCode2_2] = false;
                    bBarcodeNum[iBarCode2_2] = true;
                }

                if (BRCarryKit.Item[1][iNowCheckStep] == HAS_NULL_IC ||
                    BRCarryKit.Item[1][iNowCheckStep] == NULL_IC)
                {
                    bBarcodeNum[iBarCode2_1] = true;
                }
                else if (bBarcodeNum[iBarCode2_1] == false &&
                         Gated_Barcode_1StartScan(2, MOutShuttle2) == true)
                {
                    bBarcodeStartDelay[iBarCode2_1] = true;
                    bBarcodeDataSaveReady[iBarCode2_1] = false;
                    bBarcodeNum[iBarCode2_1] = true;
                }
            }
            else
            {
                if (BRCarryKit.Item[0][iNowCheckStep] == HAS_NULL_IC ||     //單排時,Shuttle 2, Row A 使用第3隻CCD;
                    BRCarryKit.Item[0][iNowCheckStep] == NULL_IC)
                {
                    bBarcodeNum[iBarCode2_1] = true;
                }
                else if (bBarcodeNum[iBarCode2_1] == false &&
                         Gated_Barcode_1StartScan(2, MOutShuttle2) == true)
                {
                    bBarcodeStartDelay[iBarCode2_1] = true;
                    bBarcodeDataSaveReady[iBarCode2_1] = false;
                    bBarcodeNum[iBarCode2_1] = true;
                }
                bBarcodeNum[iBarCode2_2] = true;
            }

            if (bBarcodeNum[iBarCode2_1] == true &&
                bBarcodeNum[iBarCode2_2] == true)
            {
                bBarcodeNum[iBarCode2_1] = false;
                bBarcodeNum[iBarCode2_2] = false;
                Task = 1200;
            }
            break;
        case 1200:
            iNowCheckStep--;
            if (iNowCheckStep >= 0 && iNowCheckStep < InArmSuck.iShtCol)
            {
                Task = 1000;
                break;
            }
            else
            {
                Task = 2000;
            }
            // golden :4392 falls through without a `break;` (verbatim -- not added here)
        case 2000:
            Task = 1;
            return true;
    }
    return false;
}

// =============================================================================
//  BarCode_Sh2_InitialShuttleFloatCheck2()
//  golden BarCode_Sh2.cpp:4400-4405                               //Steven 20160920
// =============================================================================
void BarCode_Sh2_InitialShuttleFloatCheck2()
{
    iShuttleFloatCheck2Task = 1;
    bCCDcheckShuttle[1] = true;                     //wei 20161102 IC置偏檢查
}
