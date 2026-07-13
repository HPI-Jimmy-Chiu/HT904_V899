// =============================================================================
//  BarCode/BarCode_Shuttle2_ScanRemainder1.cpp  --  TfBarCode::DoBarcodeScanInShuttle_2
//
//  See BarCode_Shuttle2_ScanRemainder1.h for the full scope/citation/quirk
//  banner (golden BarCode_Sh2.cpp:3040-4260).
//  Translator: AI(W5-Final-BarCodeShuttle2ScanRemainder1) 20260711.
// =============================================================================
#include "BarCode_Shuttle2_ScanRemainder1.h"

// ---------------------------------------------------------------------------
//  Local gate helpers (file-scope, internal linkage -- cannot collide with any
//  other TU's symbols, including the identically-purposed but differently-
//  named Gated_*/BarSh1_* helpers in the sibling BarCode_Shuttle2_Scan.cpp /
//  BarCode_Shuttle1_Scan.cpp / BarCode_Shuttle2_ScanRemainder2.cpp files).  See
//  the .h banner's GATE MAP for the golden home + conservative-default
//  rationale of each.
// ---------------------------------------------------------------------------
namespace
{
    // golden BarCode.h:145 mtBarcodeInSh (TTMyTray* 2DID result display grid).
    // Offline: no-op set / empty read-back (display-only, never feeds back
    // into the state machine's own Task cursor decisions -- only into UI).
    void Gated_GridSetCell(int /*BarCodeIndex*/, int /*iCol1Based*/, const AnsiString & /*Text*/)
    {
    }
    AnsiString Gated_GridGetCell(int /*BarCodeIndex*/, int /*iCol1Based*/)
    {
        return AnsiString("");
    }

    // fLotInfo->mtBarcodeInShLotInfo (separate not-yet-translated TfLotInfo
    // form's mirror grid; only used under TestIF_File.i2DIDFormat==eAMD).
    void Gated_LotInfoGridSetCell(int /*iRow1Based*/, int /*iCol1Based*/, const AnsiString & /*Text*/)
    {
    }

    // golden BarCode.h fNote->t2DCode (TfNote form's 2D-code alarm-display grid).
    void Gated_Note2DSetCell(int /*row*/, int /*col*/, const AnsiString & /*Text*/)
    {
    }
    void Gated_Note2DSetCellColor(int /*row*/, int /*col*/, int /*colorIndex*/)
    {
    }
    void Gated_Note2DSetXItem(int /*n*/)
    {
    }
    void Gated_Note2DSetYItem(int /*n*/)
    {
    }

    // golden BarCode.h btSaveImage (TButton, Save-Fail-Image trigger).
    void Gated_ClickSaveImage()
    {
    }

    // fMain->ShowNowStatus(TColor, AnsiString) -- FormsFacade has no such
    // method yet (status-bar text only, no SM feedback).
    void Gated_ShowNowStatus(int /*color*/, const AnsiString & /*Text*/)
    {
    }

    // golden BarCode.cpp:2364 (TfBarCode::TurnOffInspection) -- sends a CCD
    // "trigger off" command over TComm.  Pure hardware side effect; no-op.
    void Gated_TurnOffInspection(int /*BarCodeIndex*/)
    {
    }

    // golden BarCode.cpp:5841 (TfBarCode::DoBarcodeCount) -- production-count
    // tally bump; golden call sites discard the bool return.  No-op.
    void Gated_DoBarcodeCount()
    {
    }

    // golden BarCode.cpp:2512 (TfBarCode::InitSht2DCodeComPort) -- resets the
    // CCD serial channel state before a retry.  Offline no-op (this SM's own
    // bBarcodeNum[]/bBarcodeStartDelay[] channel state is reset by the
    // surrounding code at the same call sites anyway).
    void Gated_InitSht2DCodeComPort(int /*iStep*/)
    {
    }

    // golden BarCode.cpp:6622 (TfBarCode::CleanBarcodeError) -- clears the
    // Lot-mismatch alarm display.  Offline no-op (display-only).
    void Gated_CleanBarcodeError(int /*iStep*/)
    {
    }

    // golden BarCode.cpp:2097 (TfBarCode::Barcode_StartScan_In, ~470 lines) --
    // sends the actual 2D-trigger command over BarcodeCOM[BarCodeIndex] and
    // (eventually, async) fills cDeviceInf.  Offline: always "not yet
    // complete" (false) -- every call site already has full Retry/Skip/
    // timeout handling for this case (BarcodeDelay[iSht] timeout ladder), so
    // returning false is safe/faithful.
    bool Gated_Barcode_StartScan_In(int /*BarCodeIndex*/, int /*iXpos*/,
                                     AnsiString & /*cDeviceInf*/, const AnsiString & /*cLastDeviceInf*/)
    {
        return false;
    }

    // fBarCode->DoCheckSHT2DIDStatus(int) -- ANOTHER TfBarCode method outside
    // this unit's 20-method shim surface (Korea-customer 2DID all-site-fail
    // recheck).  Offline: false (no Korea recheck pending).
    bool Gated_DoCheckSHT2DIDStatus(int /*SHT*/)
    {
        return false;
    }

    // fBarCode->RunCheckBarcodeByServerData() -- ANOTHER TfBarCode method
    // (json/2DID server-side duplicate check enable flag).  Offline: false
    // (check disabled), matching golden's SOFT_SIMULTE-compiled-out default.
    bool Gated_RunCheckBarcodeByServerData()
    {
        return false;
    }

    // fMesSystem->IsMatchServerData(AnsiString) -- Mes/json cross-module call,
    // gated per the task brief's explicit instruction.  Offline: true (no
    // mismatch reported) -- consistent with Gated_RunCheckBarcodeByServerData()
    // already gating the whole check off.
    bool Gated_IsMatchServerData(const AnsiString & /*code*/)
    {
        return true;
    }

    // common.h WriteDataToFile/MyForceDirectories -- declared but their golden
    // BODIES are themselves `#if 0`-gated in the already-translated
    // common.cpp (TODO(wave-file), common.cpp:1435).  Calling the real names
    // would be an undefined-reference at link time; stand in with local
    // no-ops (pure logging/dir-creation side effects, no SM feedback).
    void Gated_WriteDataToFile(const AnsiString & /*path*/, const AnsiString & /*data*/)
    {
    }
    void Gated_MyForceDirectories(const AnsiString & /*dir*/)
    {
    }

    // golden mymessbox.h:54 (ShowMyMessageBox_YES_SKIP) -- blocking VCL
    // Yes/Skip prompt.  Conservative default: NOT "Yes" (take the finish/skip
    // path) so an unattended offline run doesn't loop forever waiting for an
    // operator click.  Mirrors the ALREADY-translated sibling
    // BarCode_Shuttle2_Scan.cpp's own Gated_ShowMyMessageBox_YES_SKIP (same
    // golden source file family) rather than Sh1's differently-chosen real
    // global of the same bare name.
    int Gated_ShowMyMessageBox_YES_SKIP(const AnsiString & /*S1*/, const AnsiString & /*S2*/)
    {
        return 0;
    }

    // Pure-dedup helper (NOT a gate -- real logic).  golden repeats this EXACT
    // if/else-if ladder verbatim at 2 call sites within THIS function (case
    // 1100 golden :3213-3244, case 1160 golden :3640-3671), reading
    // Prod.iInSHBarCodeDetectPos1x{1,2,3,4,5,6,8}[1][iStep] (Shuttle-2 row
    // index [1]).  `funcName` reproduces the per-call-site ShowMyMessage tag
    // verbatim (both call sites use the same literal "DoBarcodeScanInShuttle_2"
    // in golden).
    int Gated_GetInSHBarCodeDetectPos(int iShtCol, int iStep, const char *funcName)
    {
        int pos = 0;
        if (iShtCol == 1)      pos = Prod.iInSHBarCodeDetectPos1x1[1];
        else if (iShtCol == 2) pos = Prod.iInSHBarCodeDetectPos1x2[1][iStep];
        else if (iShtCol == 3) pos = Prod.iInSHBarCodeDetectPos2x3[1][iStep];      //ChungHung 20140115 add for 2x3_6
        else if (iShtCol == 4) pos = Prod.iInSHBarCodeDetectPos1x4[1][iStep];
        else if (iShtCol == 5) pos = Prod.iInSHBarCodeDetectPos2x5[1][iStep];      //Steven 20221027 : Add for 2x5
        else if (iShtCol == 6) pos = Prod.iInSHBarCodeDetectPos2x6[1][iStep];      //Steven 20170111 : fixed for 2x6
        else if (iShtCol == 8) pos = Prod.iInSHBarCodeDetectPos2x8[1][iStep];
        else
            ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", funcName);
        return pos;
    }
}

// =============================================================================
//  BarCode_Sh2_DoBarcodeScanInShuttle_2(bool bErrorSkip=false)
//  golden BarCode_Sh2.cpp:3040-4260
// =============================================================================
bool BarCode_Sh2_DoBarcodeScanInShuttle_2(bool bErrorSkip)
{
    static int  iNowCheckStep = 0;
    static int  iRetryCount = 0;
    static int  i2DIDRetryCTSH2 = 0;                //Isaac 20200206 : 2DID All site fail recheck twice and alarm
    static bool bErrorRetry = false;
    static bool bBarcodeNum[BAR_CODE_COUNT] = {false};
    static AnsiString sErrorPart(""), sCheckErrorPart("");

    int &Task = iInitialBarcodeInShuttle2Task;
    int  iSht = 1;
    int  pos = 0, ret = 0;
    int  iBarCodeRowA = iBarCode2_1;                //JerryYang 20250512 : 修正CCD使用位置
    int  iBarCodeRowB = iBarCode2_2;
    bool bMoveOK = false;                           //Ifor 20200421 add:OCR Alarm 不移動回Loader區
    AnsiString DataStr, Path, Data;
    AnsiString cLastString;
    AnsiString asString;

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
    else if (BAR_CODE_INSTALL == ebcUseOCR)
    {
        iBarCodeRowA = iBarCode2_1;
        iBarCodeRowB = iBarCode2_2;
    }

    switch (Task)
    {
        case 1:
            iNowCheckStep = InArmSuck.iShtCol - 1;                    //遞減方式檢查
            Gated_InitSht2DCodeComPort(iSht);                         //Ifor 20160322 Retry 時重置
            if (BAR_CODE_INSTALL == ebctUseCCDMode)                   //JerryYang 20201218 In house版本 shuttle 2 單排使用CCD4
            {
                if (InArmSuck.iShtRow == 2)
                {
                    bBarcodeStartDelay[iBarCodeRowA] = true;
                    bBarcodeStartDelay[iBarCodeRowB] = true;
                }
                else
                {
                    bBarcodeStartDelay[iBarCodeRowB] = true;
                }
            }

            for (int i = 0; i < BLCarryKit.iMaxCol; i++)
            {
                Gated_GridSetCell(iBarCodeRowA, i + 1, "");
                Gated_GridSetCell(iBarCodeRowB, i + 1, "");
                if (TestIF_File.i2DIDFormat == eAMD)                  //JerryYang 20200422 2DID format選項改用下拉選單
                {
                    Gated_LotInfoGridSetCell(1, i + 1, "");
                }
            }

            for (int i = 0; i < InArmSuck.iShtRow; i++)
            {
                for (int j = 0; j < InArmSuck.iShtCol; j++)
                {
                    Gated_Note2DSetCell(i, j, "");
                    Gated_Note2DSetCellColor(i, j, 0);
                }
            }
            bBarcodeNum[iBarCodeRowA] = false;
            bBarcodeNum[iBarCodeRowB] = false;
            bBarcodeFirstAutoRetry[iBarCodeRowA] = false;
            bBarcodeFirstAutoRetry[iBarCodeRowB] = false;
            iRetryCount = 0;
            if (bErrorRetry)
            {
                Task = 1100;
                bErrorRetry = false;
            }
            else
            {
                Task = 1000;
            }
            // golden :3138 falls through without a `break;` (verbatim -- not added here)
        case 1000:
            if (InArmSuck.iShtRow == 2)
            {
                if (BLCarryKit.Item[0][iNowCheckStep] != HAS_NULL_IC &&
                    BLCarryKit.Item[0][iNowCheckStep] != NULL_IC)          //Ifor 20190129 : 修正計數 //wei 20160516 修改需讀取barcode顆數累計
                {
                    if (bBarcodeFirstAutoRetry[iBarCodeRowA] == false)
                        iNeedBarcodeCount[iBarCodeRowA]++;
                }

                if (BLCarryKit.Item[1][iNowCheckStep] != HAS_NULL_IC &&
                    BLCarryKit.Item[1][iNowCheckStep] != NULL_IC)          //wei 20160516 修改需讀取barcode顆數累計
                {
                    if (bBarcodeFirstAutoRetry[iBarCodeRowB] == false)
                        iNeedBarcodeCount[iBarCodeRowB]++;
                }
            }
            else
            {
                if (BLCarryKit.Item[0][iNowCheckStep] != HAS_NULL_IC &&
                    BLCarryKit.Item[0][iNowCheckStep] != NULL_IC)          //Ifor 20190129 : 修正計數 //wei 20160516 修改需讀取barcode顆數累計
                {
                    if (bBarcodeFirstAutoRetry[iBarCodeRowB] == false)
                        iNeedBarcodeCount[iBarCodeRowB]++;
                }
            }
            Task = 1100;
            // golden :3164 falls through without a `break;` (verbatim -- not added here)
        case 1100:
            if (InArmSuck.iShtRow == 2)
            {
                if (bBarcodeNum[iBarCodeRowA] == false)
                {
                    BLCarryKit.cDeviceInf[0][iNowCheckStep] = "";
                    Gated_GridSetCell(iBarCodeRowA, iNowCheckStep + 1, "");
                    if (TestIF_File.i2DIDFormat == eAMD)                  //JerryYang 20200422 2DID format選項改用下拉選單
                    {
                        Gated_LotInfoGridSetCell(1, iNowCheckStep + 1, "");
                    }
                }

                if (bBarcodeNum[iBarCodeRowB] == false)
                {
                    BLCarryKit.cDeviceInf[1][iNowCheckStep] = "";
                    Gated_GridSetCell(iBarCodeRowB, iNowCheckStep + 1, "");
                }
            }
            else
            {
                if (bBarcodeNum[iBarCodeRowB] == false)
                {
                    BLCarryKit.cDeviceInf[0][iNowCheckStep] = "";
                    Gated_GridSetCell(iBarCodeRowB, iNowCheckStep + 1, "");
                }
            }

            if ((BLCarryKit.Item[0][iNowCheckStep] == HAS_NULL_IC || BLCarryKit.Item[0][iNowCheckStep] == NULL_IC) &&
                (BLCarryKit.Item[1][iNowCheckStep] == HAS_NULL_IC || BLCarryKit.Item[1][iNowCheckStep] == NULL_IC))
            {
                Task = 1200;
                break;
            }

            Gated_Note2DSetXItem(InArmSuck.iShtRow);                      //wei 20160824 清除顯示Barcode Alarm
            Gated_Note2DSetYItem(InArmSuck.iShtCol);
            for (int i = 0; i < InArmSuck.iShtRow; i++)
            {
                for (int j = 0; j < InArmSuck.iShtCol; j++)
                {
                    Gated_Note2DSetCell(i, j, "");
                    Gated_Note2DSetCellColor(i, j, 0);
                }
            }

            if (iNowCheckStep >= 0 && iNowCheckStep < InArmSuck.iShtCol)
            {
                pos = Gated_GetInSHBarCodeDetectPos(InArmSuck.iShtCol, iNowCheckStep, "DoBarcodeScanInShuttle_2");

                // see .h banner "FLOATING-POINT TRUNCATION ORDER" -- explicit
                // cast is semantically identical to golden `pos+=Offset.iSHLeft2D[1];`
                pos = (int)(pos + Offset.iSHLeft2D[1]);                   //Steven 20151218 : Offset for 2d reader

                if (TestIF_File.bRetryShiftOffsetMove && iRetryCount > 0)  //wei 20161116 Retry時先退出再進去讀取(前中後)
                {
                    if (iRetryCount % 3 == 1)
                        pos = (int)(pos - TestIF_File.dRetryShiftOffsetMove * 100);
                    else if (iRetryCount % 3 == 2)
                        pos = (int)(pos + TestIF_File.dRetryShiftOffsetMove * 100);
                }

                if (MOT[MInShuttle2].MotorMove(pos))
                {
                    bBarcodeStartDelay[iBarCodeRowB] = true;
                    if (InArmSuck.iShtRow == 2)
                    {
                        bBarcodeStartDelay[iBarCodeRowA] = true;
                    }

                    BarcodePosDelay[iSht].SetMSAndOn((DWORD)TestIF_File.iBarCodePosDelay);
                    Task = 1120;
                }
            }
            else
            {
                Task = 2000;
            }
            break;
        case 1120:
            if (BarcodePosDelay[iSht].Off())
            {
                BarcodeDelay[iSht].SetMSAndOn((DWORD)TestIF_File.iBarCodeDelay);
                //==> Eastsun 20260526 #026-4.P2 reset Pin1 Error :KYEC
                bHasPin1Error[iBarCodeRowA] = false;
                bHasPin1Error[iBarCodeRowB] = false;
                //<== Eastsun 20260526 #026-4.P2
                sErrorPart = "";
                sCheckErrorPart = "";
                Task = 1150;
            }
            break;
        case 1150:                                                        //雙排時,Shuttle 2, Row A 使用第4隻CCD; Row B 使用第3隻CCD
            if (InArmSuck.iShtRow == 2)                                   //JerryYang 20201218 In house版本 shuttle 2 單排使用CCD4
            {
                if (BLCarryKit.Item[0][iNowCheckStep] == HAS_NULL_IC ||
                    BLCarryKit.Item[0][iNowCheckStep] == NULL_IC)
                {
                    bBarcodeNum[iBarCodeRowA] = true;
                }
                else if (bBarcodeNum[iBarCodeRowA] == false)
                {
                    iSH2_2BarcodePosition = iNowCheckStep;
                    if (iNowCheckStep == InArmSuck.iShtCol - 1)
                    {
                        cLastString = "(NULL)";
                    }
                    else
                    {
                        cLastString = BLCarryKit.cDeviceInf[0][iNowCheckStep + 1];
                    }

                    if (Gated_Barcode_StartScan_In(iBarCodeRowA, iNowCheckStep, BLCarryKit.cDeviceInf[0][iNowCheckStep], cLastString) == true)
                    {
                        bBarcodeStartDelay[iBarCodeRowA] = true;
                        bBarcodeDataSaveReady[iBarCodeRowA] = false;
                        bBarcodeNum[iBarCodeRowA] = true;
                    }

                    if ((BAR_CODE_INSTALL == ebctEtherNetCCD || BAR_CODE_INSTALL == ebcUseOCR) && //Steven 20260420 : add OCR HandShake timeout
                        CosFunction.bUseHandShakeCommunication &&
                        TestIF_File.bUseHandShakeCommunication == true)
                    {
                        if (bCCDBarcodeWaitReply[iBarCodeRowA] == false)
                        {
                            BarcodeWaitReplyTimeOut[iBarCodeRowA].SetMSAndOn((DWORD)TestIF_File.i2DHandShakeTimeOut);
                            bCCDBarcodeWaitReply[iBarCodeRowA] = true;
                        }
                    }
                }

                if (BLCarryKit.Item[1][iNowCheckStep] == HAS_NULL_IC ||
                    BLCarryKit.Item[1][iNowCheckStep] == NULL_IC)
                {
                    bBarcodeNum[iBarCodeRowB] = true;
                }
                else if (bBarcodeNum[iBarCodeRowB] == false)
                {
                    iSH2_1BarcodePosition = iNowCheckStep;
                    if (iNowCheckStep == InArmSuck.iShtCol - 1)
                    {
                        cLastString = "(NULL)";
                    }
                    else
                    {
                        cLastString = BLCarryKit.cDeviceInf[1][iNowCheckStep + 1];
                    }

                    if (Gated_Barcode_StartScan_In(iBarCodeRowB, iNowCheckStep, BLCarryKit.cDeviceInf[1][iNowCheckStep], cLastString) == true)
                    {
                        bBarcodeStartDelay[iBarCodeRowB] = true;
                        bBarcodeDataSaveReady[iBarCodeRowB] = false;
                        bBarcodeNum[iBarCodeRowB] = true;
                    }

                    if ((BAR_CODE_INSTALL == ebctEtherNetCCD || BAR_CODE_INSTALL == ebcUseOCR) && //Steven 20260420 : add OCR HandShake timeout
                        CosFunction.bUseHandShakeCommunication &&
                        TestIF_File.bUseHandShakeCommunication == true)
                    {
                        if (bCCDBarcodeWaitReply[iBarCodeRowB] == false)
                        {
                            BarcodeWaitReplyTimeOut[iBarCodeRowB].SetMSAndOn((DWORD)TestIF_File.i2DHandShakeTimeOut);
                            bCCDBarcodeWaitReply[iBarCodeRowB] = true;
                        }
                    }
                }
            }
            else
            {
                if (BLCarryKit.Item[0][iNowCheckStep] == HAS_NULL_IC ||
                    BLCarryKit.Item[0][iNowCheckStep] == NULL_IC)
                {
                    bBarcodeNum[iBarCodeRowB] = true;
                }
                else if (bBarcodeNum[iBarCodeRowB] == false)
                {
                    iSH2_1BarcodePosition = iNowCheckStep;                //Steven 20250808 : for OCR (golden keeps this literal SH2_1 write in both branches here)

                    if (iNowCheckStep == InArmSuck.iShtCol - 1)
                    {
                        cLastString = "(NULL)";
                    }
                    else
                    {
                        cLastString = BLCarryKit.cDeviceInf[0][iNowCheckStep + 1];
                    }

                    if (Gated_Barcode_StartScan_In(iBarCodeRowB, iNowCheckStep, BLCarryKit.cDeviceInf[0][iNowCheckStep], cLastString) == true)
                    {
                        bBarcodeStartDelay[iBarCodeRowB] = true;
                        bBarcodeDataSaveReady[iBarCodeRowB] = false;
                        bBarcodeNum[iBarCodeRowB] = true;
                    }

                    if ((BAR_CODE_INSTALL == ebctEtherNetCCD || BAR_CODE_INSTALL == ebcUseOCR) && //Steven 20260420 : add OCR HandShake timeout
                        CosFunction.bUseHandShakeCommunication &&
                        TestIF_File.bUseHandShakeCommunication == true)
                    {
                        if (bCCDBarcodeWaitReply[iBarCodeRowB] == false)
                        {
                            BarcodeWaitReplyTimeOut[iBarCodeRowB].SetMSAndOn((DWORD)TestIF_File.i2DHandShakeTimeOut);
                            bCCDBarcodeWaitReply[iBarCodeRowB] = true;
                        }
                    }
                }
                bBarcodeNum[iBarCodeRowA] = true;
            }

            if (bBarcodeNum[iBarCodeRowA] == true &&
                bBarcodeNum[iBarCodeRowB] == true)
            {
                Task = 1200;
                bCCDBarcodeWaitReply[iBarCodeRowA] = false;
                bCCDBarcodeWaitReply[iBarCodeRowB] = false;
            }
            else if ((BAR_CODE_INSTALL == ebctEtherNetCCD || BAR_CODE_INSTALL == ebcUseOCR) && //Steven 20260420 : add OCR HandShake timeout
                     CosFunction.bUseHandShakeCommunication &&
                     TestIF_File.bUseHandShakeCommunication == true)
            {
                if (BarcodeWaitReplyTimeOut[iBarCodeRowB].Off())
                {
                    Task = 1180;
                    bCCDBarcodeWaitReply[iBarCodeRowB] = false;
                }

                if (InArmSuck.iShtRow == 2 && BarcodeWaitReplyTimeOut[iBarCodeRowA].Off())
                {
                    Task = 1180;
                    bCCDBarcodeWaitReply[iBarCodeRowA] = false;
                }
                break;
            }
            //==> Eastsun 20260526 #026-4.P1 Pin1 Inspection dispatch :KYEC
            else if (CosFunction.b2DUsePinInspection && TestIF_File.b2DUsePinInspection)     //Ifor 20240624 add: Pin1 mode dispatch Task=1180
            {
                if (bHasPin1Error[iBarCodeRowA] || bHasPin1Error[iBarCodeRowB])
                {
                    Task = 1180;
                }
            }
            //<== Eastsun 20260526 #026-4.P1
            else if (BarcodeDelay[iSht].Off())
            {
                if (iRetryCount < TestIF_File.iBarcodeRetryCount)         //Steven 20151221 : 讀不到Code時,自動Retry的次數
                {
                    if (iRetryCount == 0)
                    {
                        if (bBarcodeNum[iBarCodeRowB] == false &&
                            bBarcodeFirstAutoRetry[iBarCodeRowB] == false)
                        {
                            iBarcodeAutoRetry[iBarCodeRowB]++;
                            bBarcodeFirstAutoRetry[iBarCodeRowB] = true;
                        }

                        if (bBarcodeNum[iBarCodeRowA] == false &&
                            bBarcodeFirstAutoRetry[iBarCodeRowA] == false)
                        {
                            iBarcodeAutoRetry[iBarCodeRowA]++;
                            bBarcodeFirstAutoRetry[iBarCodeRowA] = true;
                        }
                    }

                    Gated_InitSht2DCodeComPort(iSht);                     //Ifor 20160322 Retry 時重置
                    iRetryCount++;
                    iSH2_1BarcodePosition = iNowCheckStep;
                    iSH2_2BarcodePosition = iNowCheckStep;
                    Gated_CleanBarcodeError(2);
                    if (TestIF_File.bRetryOffsetMove)                     //wei 20161116 Retry時先退出再進去讀取
                        Task = 1160;
                    else
                        Task = 1100;
                    return false;
                }
                else if (bBarcodeAutoSkip[iBarCodeRowA] == true ||
                         bBarcodeAutoSkip[iBarCodeRowB] == true)          //wei Barcode Lot 錯誤取出IC，需重新讀取確認是否取出IC
                {
                    if (InArmSuck.iShtRow == 2)
                    {
                        if (bBarcodeAutoSkip[iBarCodeRowA])
                        {
                            BLCarryKit.SetItemData(0, iNowCheckStep, HAS_NULL_IC);     //wei 20160823  Skip就設為NHAS_NULL_IC
                        }
                        bBarcodeAutoSkip[iBarCodeRowA] = false;

                        if (bBarcodeAutoSkip[iBarCodeRowB])
                        {
                            BLCarryKit.SetItemData(1, iNowCheckStep, HAS_NULL_IC);     //wei 20160823  Skip就設為NHAS_NULL_IC
                        }
                        bBarcodeAutoSkip[iBarCodeRowB] = false;
                    }
                    else
                    {
                        if (bBarcodeAutoSkip[iBarCodeRowB])
                        {
                            BLCarryKit.SetItemData(0, iNowCheckStep, HAS_NULL_IC);     //wei 20160823  Skip就設為NHAS_NULL_IC
                        }
                        bBarcodeAutoSkip[iBarCodeRowB] = false;
                    }

                    Gated_InitSht2DCodeComPort(iSht);                     //Ifor 20160322 Retry 時重置
                    iSH2_1BarcodePosition = iNowCheckStep;
                    iSH2_2BarcodePosition = iNowCheckStep;
                    Gated_CleanBarcodeError(2);
                    Task = 1100;
                    return false;
                }
                else
                {
                    Task = 1170;                                         //wei 20160823 Shuttle移出來在alarm    //Frank 20171011 (Steven) 1180-->1170 增加Save Fail Image
                }
            }
            break;
        case 1160:                                                       //wei 20161116 Retry時先退出再進去讀取
            if (iNowCheckStep >= 0 && iNowCheckStep < InArmSuck.iShtCol)
            {
                pos = Gated_GetInSHBarCodeDetectPos(InArmSuck.iShtCol, iNowCheckStep, "DoBarcodeScanInShuttle_2");

                // see .h banner "FLOATING-POINT TRUNCATION ORDER"
                pos = (int)(pos + Offset.iSHLeft2D[1]);                   //Steven 20151218 : Offset for 2d reader
                pos = (int)(pos + TestIF_File.dRetryOffsetMove * 100);

                if (MOT[MInShuttle2].MotorMove(pos))
                {
                    Task = 1100;
                    return false;
                }
            }
            break;
        case 1170:
            if (TestIF_File.bSaveFailImage)                               //Frank 20171011 (Steven) 1180-->1170 增加Save Fail Image
            {
                for (int i = 2; i < 4; i++)
                {
                    if (bBarcodeNum[i] == false)
                        bImageSaveflag[i] = true;

                    if (bImageSaveflag[i] == true)
                        Gated_ClickSaveImage();
                }
                Task = 1175;
            }
            else
            {
                Task = 1180;
            }
            break;
        case 1175:
            if (bImageSaveflag[iBarCodeRowA] == false &&
                bImageSaveflag[iBarCodeRowB] == false)
            {
                Task = 1180;
            }
            else
            {
                Gated_ShowNowStatus(0 /*golden clNavy*/, "Save Fail Image processing...");
            }
            break;
        case 1180:
            bMoveOK = false;
            if (BAR_CODE_INSTALL == ebcUseOCR)                            //Ifor 20200421 add:OCR Alarm 不移動回Loader區
            {
                bMoveOK = true;
            }
            else
            {
                if (MOT[MInShuttle2].MotorMove(Prod.InSHT[iSht].iLeft))
                {
                    bMoveOK = true;
                }
            }

            if (bMoveOK == true)
            {
                Gated_TurnOffInspection(iBarCodeRowA);
                Gated_TurnOffInspection(iBarCodeRowB);
                #ifdef SOFT_SIMULTE
                ret = K_SKIP;
                #else
                if (LastSet.iRealDummy == DUMMY)
                {
                    ret = K_SKIP;
                }
                //==> Eastsun 20260526 #026-4.B2 Pin1 Inspection Sh2 :KYEC
                else if (CosFunction.b2DUsePinInspection && TestIF_File.b2DUsePinInspection)     //Ifor 20240624 add:shuttle 2 Pin 1 inspection Error!
                {
                    if (bHasPin1Error[iBarCodeRowA] == false)
                        sErrorPart += IndexSuckName[0][iNowCheckStep];
                    if (bHasPin1Error[iBarCodeRowB] == false)
                        sErrorPart += IndexSuckName[1][iNowCheckStep];
                    if (bErrorSkip)                                       //Eastsun 20260327 KYEC pin1要求Contact 可以skip 正常做不能skip
                    {
                        ret = ShowErrorMessage("WAR04207", K_RETRY | K_SKIP, MInShuttle2, false, sErrorPart);      //In Shuttle2 Barcode Error
                    }
                    else
                    {
                        ret = ShowErrorMessage("WAR04207", K_RETRY, MInShuttle2, false, sErrorPart);      //In Shuttle2 Barcode Error
                    }
                }
                //<== Eastsun 20260526 #026-4.B2
                else
                {
                    if (TestIF_File.bNoCodeDeviceAutoSkip == true &&
                        fContact->IsRun2DCheck() == false)                 //JerryYang 20250220 : 2DID硬體順序檢查功能                 //Steven 20151221 : 讀不到Code時,自動Skip跳下一顆
                    {
                        ret = K_SKIP;
                    }
                    else if (bBarcodeAutoSkipError[iBarCodeRowA] == true ||
                             bBarcodeAutoSkipError[iBarCodeRowB] == true)  //wei Barcode Lot 錯誤取出IC，需重新讀取確認是否取出IC
                    {
                        sCheckErrorPart = "";
                        if (InArmSuck.iShtRow == 2)
                        {
                            if (bBarcodeAutoSkipError[iBarCodeRowA])       //wei 20160505 Barcode 比對Lot
                            {
                                sCheckErrorPart += IndexSuckName[0][iNowCheckStep];
                            }

                            if (bBarcodeAutoSkipError[iBarCodeRowB])       //wei 20160505 Barcode 比對Lot
                            {
                                sCheckErrorPart += IndexSuckName[1][iNowCheckStep];
                            }
                        }
                        else
                        {
                            if (bBarcodeAutoSkipError[iBarCodeRowB])       //wei 20160505 Barcode 比對Lot
                            {
                                sCheckErrorPart += IndexSuckName[0][iNowCheckStep];
                            }
                        }
                        ret = ShowErrorMessage("WAR0477", K_SKIP, MInShuttle2, false, sCheckErrorPart);      //In Shuttle1 Barcode Error
                    }
                    else if (TestIF_File.bCheckCodeByLot &&
                             (bCheckCodeError[iBarCodeRowA] ||
                              bCheckCodeError[iBarCodeRowB]))
                    {
                        sCheckErrorPart = "";
                        if (InArmSuck.iShtRow == 2)
                        {
                            if (bCheckCodeError[iBarCodeRowA])             //wei 20160505 Barcode 比對Lot
                            {
                                sCheckErrorPart += IndexSuckName[0][iNowCheckStep];
                            }

                            if (bCheckCodeError[iBarCodeRowB])             //wei 20160505 Barcode 比對Lot
                            {
                                sCheckErrorPart += IndexSuckName[1][iNowCheckStep];
                            }
                        }
                        else
                        {
                            if (bCheckCodeError[iBarCodeRowB])             //wei 20160505 Barcode 比對Lot
                            {
                                sCheckErrorPart += IndexSuckName[0][iNowCheckStep];
                            }
                        }
                        ret = ShowErrorMessage("WAR0468", K_RETRY | K_SKIP, MInShuttle2, false, sCheckErrorPart);      //In Shuttle1 Barcode Error  //Alick 20170126 (wei) 拿掉SKIP，避免跑錯LOT //Steven 20250808 : OCR add K_SKIP for WAR0468
                    }
                    else if (TestIF_File.bCheckLotHaveCode &&
                             (bCheckLotError[iBarCodeRowA] ||
                              bCheckLotError[iBarCodeRowB]))
                    {
                        sCheckErrorPart = "";
                        if (InArmSuck.iShtRow == 2)
                        {
                            if (bCheckLotError[iBarCodeRowA])              //wei 20160505 Barcode 比對Lot
                            {
                                sCheckErrorPart += IndexSuckName[0][iNowCheckStep];
                            }

                            if (bCheckLotError[iBarCodeRowB])              //wei 20160505 Barcode 比對Lot
                            {
                                sCheckErrorPart += IndexSuckName[1][iNowCheckStep];
                            }
                        }
                        else
                        {
                            if (bCheckLotError[iBarCodeRowB])              //wei 20160505 Barcode 比對Lot
                            {
                                sCheckErrorPart += IndexSuckName[0][iNowCheckStep];
                            }
                        }

                        sCheckErrorPart += asBarcodeLotNumber;             //wei Barcode Lot Alarm時，顯示出Lot
                        bBarcodePassword = true;                          //wei 20160823  Lot check 錯誤需輸入密碼
                        ret = ShowErrorMessage("WAR0472", K_RETRY, MInShuttle2, false, sCheckErrorPart);      //In Shuttle1 Barcode Error  //Alick 20170202 (wei) remove skip
                    }
                    else
                    {
                        if (InArmSuck.iShtRow == 2)
                        {
                            if (bBarcodeNum[iBarCodeRowA] == false)
                                sErrorPart += IndexSuckName[0][iNowCheckStep];
                            if (bBarcodeNum[iBarCodeRowB] == false)
                                sErrorPart += IndexSuckName[1][iNowCheckStep];
                        }
                        else
                        {
                            if (bBarcodeNum[iBarCodeRowB] == false)
                                sErrorPart += IndexSuckName[0][iNowCheckStep];
                        }

                        if (fContact->IsRun2DCheck() == true)              //JerryYang 20250220 : 2DID硬體順序檢查功能
                        {
                            ret = ShowErrorMessage("JAM0496", K_RETRY, MInShuttle2, false, sErrorPart);      //In Shuttle2 Barcode Error
                        }
                        else
                        {
                            ret = ShowErrorMessage("JAM0461", K_RETRY | K_SKIP, MInShuttle2, false, sErrorPart);      //In Shuttle2 Barcode Error
                        }
                    }
                    iRetryCount = -1;
                }
                #endif
                if (ret == K_RETRY)
                {
                    Gated_InitSht2DCodeComPort(iSht);                     //Ifor 20160322 Retry 時重置
                    iRetryCount = 0;
                    iSH2_1BarcodePosition = iNowCheckStep;
                    iSH2_2BarcodePosition = iNowCheckStep;
                    Gated_CleanBarcodeError(2);
                    Task = 1;
                    bErrorRetry = true;
                    return false;
                }
                else
                {
                    iRetryCount = 0;
                    Path.sprintf("%s\\%04d_%02d_%02d", asBarCodeLogPath.c_str(), SystemYear, SystemMonth, SystemDate);
                    Gated_MyForceDirectories(Path);
                    Data.sprintf("%04d_%02d_%02d  %02d_%02d_%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
                    Path.sprintf("%s\\%04d_%02d_%02d\\%04d_%02d_%02d_%02d.txt", asBarCodeLogPath.c_str(), SystemYear, SystemMonth, SystemDate, SystemYear, SystemMonth, SystemDate, SystemHour);

                    if (TestIF_File.bCheckCodeByLot && (bCheckCodeError[iBarCodeRowA] || bCheckCodeError[iBarCodeRowB]))
                    {
                        if (InArmSuck.iShtRow == 2)
                        {
                            if (bCheckCodeError[iBarCodeRowA])
                            {
                                DataStr.sprintf("%s InShuttle2-1 %d Site%d : %s", Data.c_str(), iSH2_1BarcodePosition, TestIF_File.iSiteMap[0][iSH2_1BarcodePosition], asBarCodeErrorSend.c_str()); //wei 20160325 Barcode 紀錄位置
                                Gated_WriteDataToFile(Path, DataStr);
                                BLCarryKit.cDeviceInf[0][iNowCheckStep] = asBarCodeErrorSend;         //wei 20160318 Barcode Error依客戶設定
                                Gated_GridSetCell(iBarCodeRowA, iNowCheckStep + 1, asBarCodeErrorSend);

                                if (TestIF_File.i2DIDFormat == eAMD)       //JerryYang 20200422 2DID format選項改用下拉選單
                                {
                                    Gated_LotInfoGridSetCell(1, iNowCheckStep + 1, asBarCodeErrorSend);
                                }
                                iBarcodeErrorCount[iBarCodeRowA]++;
                                bBarcodeNum[iBarCodeRowA] = true;
                                bCheckCodeError[iBarCodeRowA] = false;     //Steven 20250808 : for OCR
                            }

                            if (bCheckCodeError[iBarCodeRowB])
                            {
                                DataStr.sprintf("%s InShuttle2-2 %d Site%d : %s", Data.c_str(), iSH2_1BarcodePosition, TestIF_File.iSiteMap[1][iSH2_1BarcodePosition], asBarCodeErrorSend.c_str()); //wei 20160325 Barcode 紀錄位置
                                Gated_WriteDataToFile(Path, DataStr);
                                BLCarryKit.cDeviceInf[1][iNowCheckStep] = asBarCodeErrorSend;         //wei 20160318 Barcode Error依客戶設定
                                Gated_GridSetCell(iBarCodeRowB, iNowCheckStep + 1, asBarCodeErrorSend);

                                iBarcodeErrorCount[iBarCodeRowB]++;
                                bBarcodeNum[iBarCodeRowB] = true;
                                bCheckCodeError[iBarCodeRowB] = false;     //Steven 20250808 : for OCR
                            }
                        }
                        else
                        {
                            if (bCheckCodeError[iBarCodeRowB])
                            {
                                DataStr.sprintf("%s InShuttle2-1 %d Site%d : %s", Data.c_str(), iSH2_1BarcodePosition, TestIF_File.iSiteMap[0][iSH2_1BarcodePosition], asBarCodeErrorSend.c_str()); //wei 20160325 Barcode 紀錄位置
                                Gated_WriteDataToFile(Path, DataStr);
                                BLCarryKit.cDeviceInf[0][iNowCheckStep] = asBarCodeErrorSend;         //wei 20160318 Barcode Error依客戶設定
                                Gated_GridSetCell(iBarCodeRowB, iNowCheckStep + 1, asBarCodeErrorSend);

                                if (TestIF_File.i2DIDFormat == eAMD)       //JerryYang 20200422 2DID format選項改用下拉選單
                                {
                                    Gated_LotInfoGridSetCell(1, iNowCheckStep + 1, asBarCodeErrorSend);
                                }
                                iBarcodeErrorCount[iBarCodeRowB]++;
                                bBarcodeNum[iBarCodeRowB] = true;
                                bCheckCodeError[iBarCodeRowB] = false;     //Steven 20250808 : for OCR
                            }
                        }

                        if (bBarcodeNum[iBarCodeRowA] == false || bBarcodeNum[iBarCodeRowB] == false)
                        {
                            Gated_InitSht2DCodeComPort(iSht);             //Ifor 20160322 Retry 時重置
                            iSH2_1BarcodePosition = iNowCheckStep;
                            iSH2_2BarcodePosition = iNowCheckStep;
                            Gated_CleanBarcodeError(2);
                            Task = 1100;
                            return false;
                        }
                    }
                    else if ((TestIF_File.bCheckLotHaveCode &&
                              (bCheckLotError[iBarCodeRowA] || bCheckLotError[iBarCodeRowB])) ||
                             (bBarcodeAutoSkipError[iBarCodeRowA] || bBarcodeAutoSkipError[iBarCodeRowB]))    //wei Barcode Lot 錯誤取出IC，需重新讀取確認是否取出IC
                    {
                        if (InArmSuck.iShtRow == 2)
                        {
                            if (bCheckLotError[iBarCodeRowA] ||
                                bBarcodeAutoSkipError[iBarCodeRowA])
                            {
                                DataStr.sprintf("%s InShuttle2-1 %d Site%d : %s", Data.c_str(), iSH2_1BarcodePosition, TestIF_File.iSiteMap[0][iSH2_1BarcodePosition], asBarCodeErrorSend.c_str()); //wei 20160325 Barcode 紀錄位置
                                Gated_WriteDataToFile(Path, DataStr);
                                BLCarryKit.cDeviceInf[0][iNowCheckStep] = asBarCodeErrorSend;         //wei 20160318 Barcode Error依客戶設定
                                Gated_GridSetCell(iBarCodeRowA, iNowCheckStep + 1, asBarCodeErrorSend);
                                if (TestIF_File.i2DIDFormat == eAMD)       //JerryYang 20200422 2DID format選項改用下拉選單
                                {
                                    Gated_LotInfoGridSetCell(1, iNowCheckStep + 1, asBarCodeErrorSend);
                                }

                                iBarcodeErrorCount[iBarCodeRowA]++;
                                bBarcodeAutoSkip[iBarCodeRowA] = true;
                            }
                            bBarcodeNum[iBarCodeRowA] = false;
                            bBarcodeAutoSkipError[iBarCodeRowA] = false;

                            if (bCheckLotError[iBarCodeRowB] ||
                                bBarcodeAutoSkipError[iBarCodeRowB])
                            {
                                //AI(W5-Final-BarCodeShuttle2ScanRemainder1) 20260711: golden
                                //BarCode_Sh2.cpp ~3977 uses TestIF_File.iSiteMap[0][...] here
                                //(NOT [1]) even though this is the Row-B branch -- same class
                                //of copy-paste-from-Row-A inconsistency already documented
                                //elsewhere in this file family (see .h banner quirk list).
                                //Reproduced verbatim, not "fixed".
                                DataStr.sprintf("%s InShuttle2-2 %d Site%d : %s", Data.c_str(), iSH2_1BarcodePosition, TestIF_File.iSiteMap[0][iSH2_1BarcodePosition], asBarCodeErrorSend.c_str()); //wei 20160325 Barcode 紀錄位置
                                Gated_WriteDataToFile(Path, DataStr);
                                BLCarryKit.cDeviceInf[0][iNowCheckStep] = asBarCodeErrorSend;         //wei 20160318 Barcode Error依客戶設定
                                Gated_GridSetCell(iBarCodeRowB, iNowCheckStep + 1, asBarCodeErrorSend);
                                iBarcodeErrorCount[iBarCodeRowB]++;
                                bBarcodeAutoSkip[iBarCodeRowB] = true;
                            }
                            bBarcodeNum[iBarCodeRowB] = false;
                            bBarcodeAutoSkipError[iBarCodeRowB] = false;
                        }
                        else
                        {
                            if (bCheckLotError[iBarCodeRowB] ||
                                bBarcodeAutoSkipError[iBarCodeRowB])
                            {
                                DataStr.sprintf("%s InShuttle2-1 %d Site%d : %s", Data.c_str(), iSH2_1BarcodePosition, TestIF_File.iSiteMap[0][iSH2_1BarcodePosition], asBarCodeErrorSend.c_str()); //wei 20160325 Barcode 紀錄位置
                                Gated_WriteDataToFile(Path, DataStr);
                                BLCarryKit.cDeviceInf[0][iNowCheckStep] = asBarCodeErrorSend;         //wei 20160318 Barcode Error依客戶設定
                                Gated_GridSetCell(iBarCodeRowB, iNowCheckStep + 1, asBarCodeErrorSend);
                                if (TestIF_File.i2DIDFormat == eAMD)       //JerryYang 20200422 2DID format選項改用下拉選單
                                {
                                    Gated_LotInfoGridSetCell(1, iNowCheckStep + 1, asBarCodeErrorSend);
                                }

                                iBarcodeErrorCount[iBarCodeRowB]++;
                                bBarcodeAutoSkip[iBarCodeRowB] = true;
                            }
                            bBarcodeNum[iBarCodeRowB] = false;
                            bBarcodeAutoSkipError[iBarCodeRowB] = false;
                        }

                        if (bBarcodeNum[iBarCodeRowA] == false || bBarcodeNum[iBarCodeRowB] == false)
                        {
                            Gated_InitSht2DCodeComPort(iSht);             //Ifor 20160322 Retry 時重置
                            iSH2_1BarcodePosition = iNowCheckStep;
                            iSH2_2BarcodePosition = iNowCheckStep;
                            Gated_CleanBarcodeError(2);
                            Task = 1100;
                            return false;
                        }
                    }
                    else
                    {
                        if (InArmSuck.iShtRow == 2)
                        {
                            if (bBarcodeNum[iBarCodeRowA] == false)
                            {
                                DataStr.sprintf("%s InShuttle2-1 %d Site%d : %s", Data.c_str(), iSH2_1BarcodePosition, TestIF_File.iSiteMap[0][iSH2_1BarcodePosition], asBarCodeErrorSend.c_str()); //wei 20160325 Barcode 紀錄位置
                                Gated_WriteDataToFile(Path, DataStr);
                                BLCarryKit.cDeviceInf[0][iNowCheckStep] = asBarCodeErrorSend;         //wei 20160318 Barcode Error依客戶設定
                                Gated_GridSetCell(iBarCodeRowA, iNowCheckStep + 1, asBarCodeErrorSend);
                                if (TestIF_File.i2DIDFormat == eAMD)       //JerryYang 20200422 2DID format選項改用下拉選單
                                {
                                    Gated_LotInfoGridSetCell(1, iNowCheckStep + 1, asBarCodeErrorSend);
                                }
                                iBarcodeErrorCount[iBarCodeRowA]++;
                                iBarcodeReject++;                         //wei 20160823  Consecutive Failure
                            }

                            if (bBarcodeNum[iBarCodeRowB] == false)
                            {
                                //jou 2016-07-07 修正2D debug log記錄錯誤 TestIF_File.iSiteMap[0][iSH2_2BarcodePosition] -> TestIF_File.iSiteMap[1][iSH2_2BarcodePosition]
                                DataStr.sprintf("%s InShuttle2-2 %d Site%d : %s", Data.c_str(), iSH2_2BarcodePosition, TestIF_File.iSiteMap[1][iSH2_2BarcodePosition], asBarCodeErrorSend.c_str()); //wei 20160325 Barcode 紀錄位置
                                Gated_WriteDataToFile(Path, DataStr);
                                BLCarryKit.cDeviceInf[1][iNowCheckStep] = asBarCodeErrorSend;         //wei 20160318 Barcode Error依客戶設定
                                Gated_GridSetCell(iBarCodeRowB, iNowCheckStep + 1, asBarCodeErrorSend);
                                iBarcodeErrorCount[iBarCodeRowB]++;
                                iBarcodeReject++;                         //wei 20160823  Consecutive Failure
                            }
                        }
                        else
                        {
                            if (bBarcodeNum[iBarCodeRowB] == false)
                            {
                                DataStr.sprintf("%s InShuttle2-1 %d Site%d : %s", Data.c_str(), iSH2_1BarcodePosition, TestIF_File.iSiteMap[0][iSH2_1BarcodePosition], asBarCodeErrorSend.c_str()); //wei 20160325 Barcode 紀錄位置
                                Gated_WriteDataToFile(Path, DataStr);
                                BLCarryKit.cDeviceInf[0][iNowCheckStep] = asBarCodeErrorSend;         //wei 20160318 Barcode Error依客戶設定
                                Gated_GridSetCell(iBarCodeRowB, iNowCheckStep + 1, asBarCodeErrorSend);
                                if (TestIF_File.i2DIDFormat == eAMD)       //JerryYang 20200422 2DID format選項改用下拉選單
                                {
                                    Gated_LotInfoGridSetCell(1, iNowCheckStep + 1, asBarCodeErrorSend);
                                }
                                iBarcodeErrorCount[iBarCodeRowB]++;
                                iBarcodeReject++;                         //wei 20160823  Consecutive Failure
                            }
                        }
                    }

                    if (TestIF_File.bEnableConsecutiveFailure && iBarcodeReject >= TestIF_File.iConsecutiveFailure)
                    {
                        ShowErrorMessage("WAR0475", K_SKIP, MInShuttle2, false);
                        iBarcodeReject = 0;
                    }
                    bBarcodePassword = false;                             //wei 20160823  Lot check 錯誤需輸入密碼
                    Task = 1200;
                }
            }
            break;
        case 1200:
            //==> Eastsun 20260526 #026-4.PinN.Sh2-5 Pin1 skip dup-check :KYEC
            if (CosFunction.b2DUsePinInspection && TestIF_File.b2DUsePinInspection)
            {
                Task = 1300;
                break;
            }
            //<== Eastsun 20260526 #026-4.PinN.Sh2-5
            for (int i = 0; i < InArmSuck.iShtRow; i++)                   //20161123 Frank 同一個Shuttle上有兩個一樣的ID的時候
            {
                for (int j = 0; j < InArmSuck.iShtCol; j++)
                {
                    asString = Gated_GridGetCell(i + 2, j + 1);
                    if (asString != "" &&
                        asString != asBarCodeErrorSend &&
                        asString != asBarCodeErrorCheckSum)               //KaiHuang 20200826 : Add CheckSum
                    {
                        if (InArmSuck.iShtRow == 1)
                        {
                            if (BLCarryKit.cDeviceInf[0][iNowCheckStep] == asString && (i != 0 || j != iNowCheckStep))
                            {
                                bCheckCodeError[iBarCodeRowB] = true;
                            }
                        }
                        else
                        {
                            if (BLCarryKit.cDeviceInf[0][iNowCheckStep] == asString && (i != 0 || j != iNowCheckStep))
                            {
                                bCheckCodeError[iBarCodeRowA] = true;
                            }

                            if (BLCarryKit.cDeviceInf[1][iNowCheckStep] == asString && (i != 1 || j != iNowCheckStep))
                            {
                                bCheckCodeError[iBarCodeRowB] = true;
                            }
                        }
                    }
                }
            }
            #ifndef SOFT_SIMULTE
            if (InArmSuck.iShtRow == 2)
            {
                if (Gated_RunCheckBarcodeByServerData() == true &&                 //Jimmychiu 20230925 : read 2did in json file
                    Gated_IsMatchServerData(BLCarryKit.cDeviceInf[0][iNowCheckStep]) == false)
                {
                    bCheckCodeError[iBarCodeRowA] = true;
                }

                if (Gated_RunCheckBarcodeByServerData() == true &&                 //Jimmychiu 20230925 : read 2did in json file
                    Gated_IsMatchServerData(BLCarryKit.cDeviceInf[1][iNowCheckStep]) == false)
                {
                    bCheckCodeError[iBarCodeRowB] = true;
                }
            }
            else
            {
                if (Gated_RunCheckBarcodeByServerData() == true &&                 //Jimmychiu 20230925 : read 2did in json file
                    Gated_IsMatchServerData(BLCarryKit.cDeviceInf[0][iNowCheckStep]) == false)
                {
                    bCheckCodeError[iBarCodeRowB] = true;
                }
            }
            #endif
            if (bCheckCodeError[iBarCodeRowB] || bCheckCodeError[iBarCodeRowA])
            {
                Task = 1180;
            }
            else
            {
                Task = 1300;
            }
            break;
        case 1300:
            bBarcodeNum[iBarCodeRowA] = false;
            bBarcodeNum[iBarCodeRowB] = false;
            iRetryCount = 0;
            iNowCheckStep--;
            bBarcodeFirstAutoRetry[iBarCodeRowA] = false;
            bBarcodeFirstAutoRetry[iBarCodeRowB] = false;

            if (iNowCheckStep >= 0 && iNowCheckStep < InArmSuck.iShtCol)
            {
                Task = 1000;
                break;
            }
            else
            {
                Task = 2000;
            }
            // golden :4163 has a COMMENTED-OUT `break;` here (verbatim -- see
            // .h banner quirk 3 -- deliberate fall-through into case 2000)
        case 2000:
            if (TestIF_File.bCheckCodeByLot && fContact->IsRun2DCheck() == false)   //把新的值加到List裡面  //JerryYang 20250428 : fix 2DID map
            {
                list2DByLot->Clear();
                for (int i = 0; i < InArmSuck.iShtCol; i++)
                {
                    if (BLCarryKit.Item[0][i] != HAS_NULL_IC && BLCarryKit.Item[0][i] != NULL_IC &&
                        BLCarryKit.cDeviceInf[0][i] != asBarCodeErrorSend && BLCarryKit.cDeviceInf[0][i] != "" &&
                        BLCarryKit.cDeviceInf[0][i] != asBarCodeErrorCheckSum)     //KaiHuang 20200826 : Add CheckSum
                    {
                        list2DByLot->Add(BLCarryKit.cDeviceInf[0][i]);
                        BLCarryKit.PordRec[0][i].Add2DIDRecord(BLCarryKit.cDeviceInf[0][i]);
                    }

                    if (InArmSuck.iShtRow == 2)
                    {
                        if (BLCarryKit.Item[1][i] != HAS_NULL_IC && BLCarryKit.Item[1][i] != NULL_IC &&
                            BLCarryKit.cDeviceInf[1][i] != asBarCodeErrorSend && BLCarryKit.cDeviceInf[1][i] != "" &&
                            BLCarryKit.cDeviceInf[1][i] != asBarCodeErrorCheckSum) //KaiHuang 20200826 : Add CheckSum
                        {
                            list2DByLot->Add(BLCarryKit.cDeviceInf[1][i]);
                            BLCarryKit.PordRec[1][i].Add2DIDRecord(BLCarryKit.cDeviceInf[1][i]);
                        }
                    }
                }
                Gated_WriteDataToFile(asBarCodeLot, list2DByLot->Text);   //Steven 20160429 : 開程式要把2D List讀回來
            }
            else
            {
                for (int i = 0; i < InArmSuck.iShtCol; i++)
                {
                    if (BLCarryKit.Item[0][i] != HAS_NULL_IC && BLCarryKit.Item[0][i] != NULL_IC &&
                        BLCarryKit.cDeviceInf[0][i] != asBarCodeErrorSend && BLCarryKit.cDeviceInf[0][i] != "")
                    {
                        BLCarryKit.PordRec[0][i].Add2DIDRecord(BLCarryKit.cDeviceInf[0][i]);
                    }

                    if (InArmSuck.iShtRow == 2)
                    {
                        if (BLCarryKit.Item[1][i] != HAS_NULL_IC && BLCarryKit.Item[1][i] != NULL_IC &&
                            BLCarryKit.cDeviceInf[1][i] != asBarCodeErrorSend && BLCarryKit.cDeviceInf[1][i] != "")
                        {
                            BLCarryKit.PordRec[1][i].Add2DIDRecord(BLCarryKit.cDeviceInf[1][i]);
                        }
                    }
                }
            }

            Task = 3000;
            break;
        case 3000:
            if (IniConfig.bKoreaFunction == true)
            {
                if (Gated_DoCheckSHT2DIDStatus(1) == true)
                {
                    Task = 5000;                                         //rescan    //Isaac 20200206 : 2DID All site fail recheck twice and alarm
                    break;
                }
            }
            Task = 3100;
            // golden :4223 falls through without a `break;` (verbatim -- not added here)
        case 3100:
            Gated_DoBarcodeCount();                                       //wei 20160318
            Task = 1;
            return true;
        case 5000:                                                        //Isaac 20200206 : 2DID All site fail recheck twice and alarm
            if (IniConfig.bF21InOutArmZMotorPrivate)
            {
                if (DoInOutARM_SHT_MoveSafe(1))                           //kevin 20161005 SHUTTLE 1 移動安全保護
                    return false;
            }

            MOT[MInShuttle2].fCanMoveR = true;
            if (MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iLeft))
            {
                Task = 1;
                i2DIDRetryCTSH2++;
                if (i2DIDRetryCTSH2 > 1)
                {
                    i2DIDRetryCTSH2 = 0;
                    ret = Gated_ShowMyMessageBox_YES_SKIP("All sites 2DID Error On Shuttle 2!! Please check!", "Retry Again?");
                    if (ret == 1)                                        //Yes
                    {
                        Task = 1;
                        break;
                    }
                    else
                    {
                        Gated_DoBarcodeCount();                          //wei 20160318
                        Task = 1;
                        return true;
                    }
                }
            }
            break;
    }
    return false;
}
