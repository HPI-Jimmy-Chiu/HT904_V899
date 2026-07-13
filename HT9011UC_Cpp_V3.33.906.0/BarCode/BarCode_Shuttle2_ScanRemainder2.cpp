// =============================================================================
//  BarCode/BarCode_Shuttle2_ScanRemainder2.cpp  --  TfBarCode::DoShuttleFloatCheck_2
//
//  See BarCode_Shuttle2_ScanRemainder2.h for the full scope/citation/quirk
//  banner (golden BarCode_Sh2.cpp:4406-4878).
//  Translator: AI(W5-Final-BarCodeShuttle2ScanRemainder2) 20260711.
// =============================================================================
#include "BarCode_Shuttle2_ScanRemainder2.h"

// ---------------------------------------------------------------------------
//  Local gate helpers (file-scope, internal linkage -- cannot collide with any
//  other TU's symbols, including the identically-purposed but differently-
//  named Gated_*/BarSh1_* helpers in the sibling BarCode_Shuttle2_Scan.cpp /
//  BarCode_Shuttle1_Scan.cpp files).  See the .h banner for the golden home +
//  conservative-default rationale of each.
// ---------------------------------------------------------------------------
namespace
{
    // golden BarCode.cpp:5305-5526 (TfBarCode::SendCCDCommand) -- writes a
    // command string to BarcodeCOM[index] (TComm serial channel, not yet
    // wired for this unit).  Pure hardware side effect; no-op.
    void Gated_SendCCDCommand(int /*index*/, const AnsiString & /*Msg*/, const AnsiString & /*Msg2*/)
    {
    }

    // golden BarCode.cpp:5903-5974 (TfBarCode::AddCCDCommunicationLog) --
    // appends to a UI comm-log memo.  Pure log side-channel; no-op (matches
    // the sibling BarCode_Shuttle2_Scan.cpp's Gated_AddCCDCommunicationLog).
    void Gated_AddCCDCommunicationLog(int /*index*/, const AnsiString & /*Str*/)
    {
    }

    // golden BarCode.h (mtSFC_Shuttle, TTMyTray* SFC result-display grid widget
    // on the TfBarCode form) -- pure UI mirror, never read back into the SM's
    // own Task cursor/state; no-op (VCL form not translated -- W7).
    void Gated_SetSFCGridCell(int /*index*/, int /*iCol1Based*/, const AnsiString & /*Text*/)
    {
    }

    // BarSh2_SetSFCCheckStepCount() -- faithful mirror of golden
    // BarCode.cpp:3128 TfBarCode::SetSFCCheckStepCount() (trivial 2-line pure
    // calc; NOT gated -- see .h banner rationale).  Deliberately a fresh, file-
    // local copy rather than reaching into BarCode_Shuttle1_Scan.cpp's own
    // internal-linkage BarSh1_SetSFCCheckStepCount (unreachable across TUs);
    // harmless duplication of a 2-line pure function, same call semantics.
    void BarSh2_SetSFCCheckStepCount()
    {
        if (TestIF_File.bSFCUse2Photo)                                              //遞減方式檢查
            iSFCTotalMoveStep = InArmSuck.iShtCol * 2;
        else
            iSFCTotalMoveStep = InArmSuck.iShtCol;
    }
}

// =============================================================================
//  BarCode_Sh2_DoShuttleFloatCheck_2()
//  golden BarCode_Sh2.cpp:4406-4878                                //Steven 20160920 : IC置偏檢查
// =============================================================================
bool BarCode_Sh2_DoShuttleFloatCheck_2()
{
    static int  iNowMoveStep = 0;
    static int  iRetryCount = 0;
    static int  iRetryCountExpTimo = 0;
    static bool bHasError = false;
    static AnsiString ErrPart;
    static AnsiString sErrorPart = "";

    int &Task = iShuttleFloatCheck2Task;
    int  pos = 0, ret = 0;
    int  iSht = 1;
    int  MSht = MInShuttle2;
    int  iSFCRowA = iBarCode2_1;
    int  iSFCRowB = iBarCode2_2;
    bool bResult = false;
    AnsiString cLastString, sCCDCommand;
    AnsiString Log;

    switch (Task)
    {
        case 1:
            if (BLCarryKit.HasRealIC() == false)
            {
                bResult = true;
                break;
            }
            Gated_AddCCDCommunicationLog(iSFCRowA, "<<SFC Cycle, Cycle start.------------------");

            BarSh2_SetSFCCheckStepCount();
            iNowMoveStep = iSFCTotalMoveStep - 1;
            iSFCCurrentStep[iSFCRowA] = iNowMoveStep;
            iSFCCurrentStep[iSFCRowB] = iNowMoveStep;
            bSFCStepError[iSFCRowA] = false;
            bSFCStepError[iSFCRowB] = false;
            bSFCExposureOK[iSFCRowA] = true;
            bSFCExposureOK[iSFCRowB] = true;
            bSFCGetResultOK[iSFCRowA] = true;
            bSFCGetResultOK[iSFCRowB] = true;
            iRetryCountExpTimo = 0;

            for (int i = 0; i < 16; i++)
            {
                iSFCCheckResult[iSFCRowA][i] = -1;
                iSFCCheckResult[iSFCRowB][i] = -1;
            }

            bHasError = false;
            if (bCCDcheckShuttle[1])                                                //wei 20161102 IC置偏檢查
            {
                iRetryCount = 0;
                bCCDcheckShuttle[1] = false;
            }
            sCCDCommand.sprintf("E9,1,%d", iSFCTotalMoveStep);                      //清空暫存器
            bSFCGetSE9[iSFCRowA] = false;
            bSFCGetSE9[iSFCRowB] = false;
            Gated_SendCCDCommand(iSFCRowA, "SFC Clear buffer 1", sCCDCommand);       //Reset CCD Shuttle暫存器
            Gated_SendCCDCommand(iSFCRowB, "SFC Clear buffer 1", sCCDCommand);
            SFCStartDelay[iSht].SetMSAndOn(5000);
            Task = 500;
        case 500:                                                                   //確認Buffer清空
            if (bSFCGetSE9[iSFCRowA] && bSFCGetSE9[iSFCRowB])
            {
                Task = 1000;
            }
            else if (SFCStartDelay[iSht].Off())
            {
                ShowErrorMessage("WAR0462", K_RETRY, MSht, false, "SFC Clear buffer");
                Task = 1;
            }
            break;
        case 1000:                                                                  //移動到拍照位置
            if (TestIF_File.bSFCUse2Photo)
            {
                if (InArmSuck.iShtCol == 1)
                {
                    pos = Prod.iInSFCPos1x1_2[iSht][iNowMoveStep];
                }
                else if (InArmSuck.iShtCol == 2)
                {
                    pos = Prod.iInSFCPos1x2_2[iSht][iNowMoveStep];
                }
                else if (InArmSuck.iShtCol == 3)                                     //ChungHung 20140115 add for 2x3_6
                {
                    pos = Prod.iInSFCPos1x3_2[iSht][iNowMoveStep];
                }
                else if (InArmSuck.iShtCol == 4)
                {
                    pos = Prod.iInSFCPos1x4_2[iSht][iNowMoveStep];
                }
                else if (InArmSuck.iShtCol == 5)                                     //Steven 20221107 : Add for 2x5
                {
                    pos = Prod.iInSFCPos1x5_2[iSht][iNowMoveStep];
                }
                else if (InArmSuck.iShtCol == 6)
                {
                    pos = Prod.iInSFCPos1x6_2[iSht][iNowMoveStep];
                }
                else if (InArmSuck.iShtCol == 8)
                {
                    pos = Prod.iInSFCPos1x8_2[iSht][iNowMoveStep];
                }
                else
                {
                    ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "DoShuttleFloatCheck_2");
                }
            }
            else
            {
                if (InArmSuck.iShtCol == 1)
                {
                    pos = Prod.iInSFCPos1x1[iSht];
                }
                else if (InArmSuck.iShtCol == 2)
                {
                    pos = Prod.iInSFCPos1x2[iSht][iNowMoveStep];
                }
                else if (InArmSuck.iShtCol == 3)                                     //ChungHung 20140115 add for 2x3_6
                {
                    pos = Prod.iInSFCPos1x3[iSht][iNowMoveStep];
                }
                else if (InArmSuck.iShtCol == 4)
                {
                    pos = Prod.iInSFCPos1x4[iSht][iNowMoveStep];
                }
                else if (InArmSuck.iShtCol == 5)                                     //Steven 20221107 : Add for 2x5
                {
                    pos = Prod.iInSFCPos1x5[iSht][iNowMoveStep];
                }
                else if (InArmSuck.iShtCol == 6)
                {
                    pos = Prod.iInSFCPos1x6[iSht][iNowMoveStep];
                }
                else if (InArmSuck.iShtCol == 8)
                {
                    pos = Prod.iInSFCPos1x8[iSht][iNowMoveStep];
                }
                else
                {
                    ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "DoShuttleFloatCheck_2");
                }
            }

            pos += Offset.iSHLeft2D[iSht];

            if (MOT[MSht].MotorMove(pos))
            {
                Log.sprintf("SFC Action, Shuttle %d move to step %d", iSht + 1, iNowMoveStep);
                Gated_AddCCDCommunicationLog(iSFCRowA, Log);
                SFCStartDelay[iSht].SetMSAndOn(TestIF_File.iSFCStartDelay);
                Task = 1120;
            }
            break;
        case 1120:
            if (SFCStartDelay[iSht].Off())                                          //到位Delay
            {
                bSFCExposureOK[iSFCRowA] = false;
                if (InArmSuck.iShtRow == 2)
                    bSFCExposureOK[iSFCRowB] = false;
                else
                    bSFCExposureOK[iSFCRowB] = true;

                //--------------------------------------
                //SE   1,        1,     X,   1
                //拍照 偵測置偏, 要拍, 位置, 有無IC
                //--------------------------------------
                if (InArmSuck.iShtRow == 2)
                {
                    if (BLCarryKit.Item[0][iNowMoveStep] == HAS_NULL_IC || BLCarryKit.Item[0][iNowMoveStep] == NULL_IC)
                        sCCDCommand.sprintf("E1,1,%d,0", iNowMoveStep);
                    else
                        sCCDCommand.sprintf("E1,1,%d,1", iNowMoveStep);
                    Gated_SetSFCGridCell(iSFCRowA, iNowMoveStep + 1, "SFC Exposure...");
                    Gated_SendCCDCommand(iSFCRowA, "SFC exposure 1120: ", sCCDCommand);

                    if (BLCarryKit.Item[1][iNowMoveStep] == HAS_NULL_IC || BLCarryKit.Item[1][iNowMoveStep] == NULL_IC)
                        sCCDCommand.sprintf("E1,1,%d,0", iNowMoveStep);
                    else
                        sCCDCommand.sprintf("E1,1,%d,1", iNowMoveStep);
                    Gated_SetSFCGridCell(iSFCRowB, iNowMoveStep + 1, "SFC Exposure...");
                    Gated_SendCCDCommand(iSFCRowB, "SFC exposure 1120: ", sCCDCommand);
                }
                else
                {
                    if (BLCarryKit.Item[0][iNowMoveStep] == HAS_NULL_IC || BLCarryKit.Item[0][iNowMoveStep] == NULL_IC)
                        sCCDCommand.sprintf("E1,1,%d,0", iNowMoveStep);
                    else
                        sCCDCommand.sprintf("E1,1,%d,1", iNowMoveStep);
                    Gated_SetSFCGridCell(iSFCRowB, iNowMoveStep + 1, "SFC Exposure...");
                    Gated_SendCCDCommand(iSFCRowB, "SFC exposure 1120: ", sCCDCommand);
                }

                Task = 1150;
                iRetryCountExpTimo = 0;                                              //Steven 20160425 : 修改拍照Time Out
                SFCExposureDelay[iSht].SetMSAndOn(TestIF_File.iSFCExposureTimeOut);
            }
            break;
        case 1150:
            // AI(W5-Final-BarCodeShuttle2ScanRemainder2) 20260711: golden
            // BarCode_Sh2.cpp:4544-4557 branches ErrPart's IndexSuckName row
            // purely on `InArmSuck.iShtRow==2` -- NO IsNNMode() 3-way split here,
            // unlike the sibling DoShuttleFloatCheck_1 (BarCode_Shuttle1_Scan.cpp
            // case 1150).  Verified against the full golden body (no
            // IsNNMode/NN_1Row/NN_2Row token anywhere in BarCode_Sh2.cpp:4406-4878).
            // Reproduced verbatim (NOT "fixed") -- see .h banner "GOLDEN QUIRK
            // PRESERVED VERBATIM".
            if (bSFCStepError[iSFCRowA] == true || bSFCStepError[iSFCRowB] == true)  //Steven 20160503 : 確認移動位置與拍照位置相同
            {
                ErrPart = "";
                if (InArmSuck.iShtRow == 2)
                {
                    if (bSFCStepError[iSFCRowA])
                        ErrPart += IndexSuckName[0][iNowMoveStep];

                    if (bSFCStepError[iSFCRowB])
                        ErrPart += IndexSuckName[1][iNowMoveStep];
                }
                else
                {
                    if (bSFCStepError[iSFCRowB])
                        ErrPart += IndexSuckName[0][iNowMoveStep];
                }

                Task = 1180;
            }
            else if (bSFCExposureOK[iSFCRowA] == true &&
                     bSFCExposureOK[iSFCRowB] == true)                              //拍照完成
            {
                if (InArmSuck.iShtRow == 2)
                {
                    Gated_SetSFCGridCell(iSFCRowA, iNowMoveStep + 1, "Waiting result..");
                }
                Gated_SetSFCGridCell(iSFCRowB, iNowMoveStep + 1, "Waiting result..");
                Task = 1200;
            }
            else if (SFCExposureDelay[iSht].Off())                                  //拍照time out
            {
                if (InArmSuck.iShtRow == 2)
                {
                    if (bSFCStepError[iSFCRowA] == false)
                    {
                        Gated_SetSFCGridCell(iSFCRowA, iNowMoveStep + 1, "Exposure time out");
                        Log.sprintf("SFC Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                        Gated_AddCCDCommunicationLog(iSFCRowA, Log);
                    }
                    else
                    {
                        Gated_SetSFCGridCell(iSFCRowA, iNowMoveStep + 1, "Waiting result..");
                    }
                }

                if (bSFCStepError[iSFCRowB] == false)
                {
                    Gated_SetSFCGridCell(iSFCRowB, iNowMoveStep + 1, "Exposure time out");
                    Log.sprintf("SFC Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                    Gated_AddCCDCommunicationLog(iSFCRowB, Log);
                }
                else
                {
                    Gated_SetSFCGridCell(iSFCRowB, iNowMoveStep + 1, "Waiting result..");
                }

                iRetryCountExpTimo++;                                               //Steven 20160425 : 修改拍照Time Out
                Task = 1160;
            }
            break;
        case 1160:
            if (iRetryCountExpTimo < 3)
            {
                Task = 1170;
            }
            else
            {
                ret = ShowErrorMessage("WAR0464", K_RETRY | K_SKIP, MSht);          //2D function CCD 3 or 4 Exposure time out!
                if (ret == K_SKIP)
                {
                    Task = 1200;
                }
                else
                {
                    Task = 1170;
                }
                iRetryCountExpTimo = 0;
            }
            break;
        case 1170:
            if (InArmSuck.iShtRow == 2)
            {
                if (bSFCStepError[iSFCRowA] == false)
                {
                    if (BLCarryKit.Item[0][iNowMoveStep] == HAS_NULL_IC || BLCarryKit.Item[0][iNowMoveStep] == NULL_IC)
                        sCCDCommand.sprintf("E1,1,%d,0", iNowMoveStep);
                    else
                        sCCDCommand.sprintf("E1,1,%d,1", iNowMoveStep);

                    Gated_SendCCDCommand(iSFCRowA, "SFC exposure 1170: ", sCCDCommand);
                }

                if (bSFCStepError[iSFCRowB] == false)
                {
                    if (BLCarryKit.Item[1][iNowMoveStep] == HAS_NULL_IC || BLCarryKit.Item[1][iNowMoveStep] == NULL_IC)
                        sCCDCommand.sprintf("E1,1,%d,0", iNowMoveStep);
                    else
                        sCCDCommand.sprintf("E1,1,%d,1", iNowMoveStep);
                    Gated_SendCCDCommand(iSFCRowB, "SFC exposure 1170: ", sCCDCommand);
                }
            }
            else
            {
                if (bSFCStepError[iSFCRowB] == false)
                {
                    if (BLCarryKit.Item[0][iNowMoveStep] == HAS_NULL_IC || BLCarryKit.Item[0][iNowMoveStep] == NULL_IC)
                        sCCDCommand.sprintf("E1,1,%d,0", iNowMoveStep);
                    else
                        sCCDCommand.sprintf("E1,1,%d,1", iNowMoveStep);
                    Gated_SendCCDCommand(iSFCRowB, "SFC exposure 1170: ", sCCDCommand);
                }
            }
            Task = 1150;
            SFCExposureDelay[iSht].SetMSAndOn(TestIF_File.iSFCExposureTimeOut);
            break;
        case 1180:
            Log.sprintf("SFC Alarm, In shuttle %d exposure position error!", iSht + 1);
            Gated_AddCCDCommunicationLog(iSFCRowA, Log);
            ShowErrorMessage("WAR0470", K_RETRY, MSht, false, ErrPart);              //In shuttle 2 exposure position error!
            Task = 1;
            break;
        case 1200:
            iNowMoveStep--;
            iSFCCurrentStep[iSFCRowA] = iNowMoveStep;                               //Steven 20160503 : 確認移動位置與拍照位置相同
            iSFCCurrentStep[iSFCRowB] = iNowMoveStep;

            if (iNowMoveStep >= 0 && iNowMoveStep < iSFCTotalMoveStep)
            {
                Task = 1000;
            }
            else
            {
                Task = 1300;
            }
            break;
        case 1300:
            sCCDCommand = "GV,1";                                                   //Ifor 20151228 取得 CCD 讀取資料
            if (InArmSuck.iShtRow == 2)
            {
                bSFCGetResultOK[iSFCRowA] = false;
                Gated_SendCCDCommand(iSFCRowA, "SFC get result 1130:", sCCDCommand);
            }
            else
            {
                bSFCGetResultOK[iSFCRowA] = true;
            }

            bSFCGetResultOK[iSFCRowB] = false;
            Gated_SendCCDCommand(iSFCRowB, "SFC get result 1130: ", sCCDCommand);

            bHasError = false;
            Task = 1350;
            SFCResultDelay[iSht].SetMSAndOn(TestIF_File.iSFCGetResultTimeOut);       //這個是讀取Time Out用的
            break;
        case 1350:
            if (bSFCGetResultOK[iSFCRowA] == true &&
                bSFCGetResultOK[iSFCRowB] == true)                                  //讀取完成
            {
                sErrorPart = "";
                for (int i = 0; i < iSFCTotalMoveStep; i++)                         //清空畫面
                {
                    if (InArmSuck.iShtRow == 2)
                    {
                        if (iSFCCheckResult[iSFCRowA][i] == 0)
                        {
                            bHasError = true;
                            sErrorPart += IndexSuckName[0][i / 2];
                        }

                        if (iSFCCheckResult[iSFCRowB][i] == 0)
                        {
                            bHasError = true;
                            sErrorPart += IndexSuckName[1][i / 2];
                        }
                    }
                    else
                    {
                        if (iSFCCheckResult[iSFCRowB][i] == 0)
                        {
                            bHasError = true;
                            sErrorPart += IndexSuckName[0][i / 2];
                        }
                    }
                }

                Task = 2000;
            }
            else if (SFCResultDelay[iSht].Off())                                    //讀取超時
            {
                sErrorPart = "";
                if (InArmSuck.iShtRow == 2)
                {
                    if (bSFCGetResultOK[iSFCRowA] == false)
                    {
                        bSFCGetResultOK[iSFCRowA] = true;
                        for (int i = 0; i < iSFCTotalMoveStep; i++)                  //清空畫面
                        {
                            Gated_SetSFCGridCell(iSFCRowA, i + 1, "Inspection time Out");
                        }
                        Gated_AddCCDCommunicationLog(iSFCRowA, "SFC Action 1350, Inspection time out.");
                        bHasError = true;
                    }

                    if (bSFCGetResultOK[iSFCRowB] == false)
                    {
                        bSFCGetResultOK[iSFCRowB] = true;
                        for (int i = 0; i < iSFCTotalMoveStep; i++)                  //清空畫面
                        {
                            Gated_SetSFCGridCell(iSFCRowB, i + 1, "Inspection time Out");
                        }
                        Gated_AddCCDCommunicationLog(iSFCRowB, "SFC Action 1350, Inspection time out.");
                        bHasError = true;
                    }
                }
                else
                {
                    if (bSFCGetResultOK[iSFCRowB] == false)
                    {
                        bSFCGetResultOK[iSFCRowB] = true;
                        for (int i = 0; i < iSFCTotalMoveStep; i++)                  //清空畫面
                        {
                            Gated_SetSFCGridCell(iSFCRowB, i + 1, "Inspection time Out");
                        }
                        Gated_AddCCDCommunicationLog(iSFCRowB, "SFC Action 1350, Inspection time out.");
                        bHasError = true;
                    }
                }
                Task = 2000;
            }
            break;
        case 2000:
            if (bHasError == true)
            {
                if (iRetryCount < TestIF_File.iSFCAutoRetry)                        //Steven 20160421 : 修正判斷方式
                {
                    iRetryCount++;
                    Task = 1;
                }
                else
                {
                    Task = 2500;
                }
                bHasError = false;
            }
            else
            {
                Gated_AddCCDCommunicationLog(iSFCRowA, "SFC >>Cycle 2000, Inspection end.---------------\r\n");
                Task = 5000;
            }
            break;
        case 2500:
            if (MOT[MSht].MotorMove(Prod.InSHT[iSht].iLeft + 1000))
            {
                Log.sprintf("SFC Action 2500, Shuttle %d move to left", iSht + 1);
                Gated_AddCCDCommunicationLog(iSFCRowA, Log);
                fLtcSensor->ClearLtcSensor(iSht);                                   //Sam 20221101 : Latch 清除都要確認是否清清乾淨
                Task = 2600;
            }
            break;
        case 2600:
            Log.sprintf("SFC Alarm 2600, In shuttle %d device floating error", iSht + 1);
            Gated_AddCCDCommunicationLog(iSFCRowA, Log);
            ret = ShowErrorMessage("JAM0479", K_RETRY, MSht, false, sErrorPart);     //In Shuttle 2 IC float error
            iRetryCount = 0;
            bHasError = false;
            Task = 1;
            break;
        case 5000:
            iRetryCount = 0;
            Task = 1;
            return true;
    }

    return bResult;
}
