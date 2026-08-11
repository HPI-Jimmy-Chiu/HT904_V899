// =============================================================================
//  BarCode/BarCode_Shuttle2_SFCAutoTune.cpp  --  Shuttle-2 SFC Auto-Tune (AT)
//                                                 + manual 2DID-Check cycles
//
//  See BarCode_Shuttle2_SFCAutoTune.h for the full ROLE / WAVE SCOPE / golden
//  quirk register.  Golden source: BarCode/BarCode_Sh2.cpp:4880-5664 (the last
//  3 functions of the file, read with encoding='cp950').
//  Translator: AI(W5-BarCode-Shuttle2-SFCAutoTune) 20260811.
//
//  =========================== GATE REGISTER =================================
//  One entry per gated call site introduced by this unit.  For each:
//  (a) the golden line/home, (b) WHY the offline default is FAITHFUL,
//  (c) how the real machine differs.  All the stand-ins live in the anonymous
//  namespace below, so they have internal linkage and CANNOT collide with the
//  identically-purposed Gated_*/BarSh1_*/CCD1_* helpers in the sibling BarCode
//  TUs, nor with anything the concurrently-running Sh1 counterpart unit writes.
//
//  G-0  ::SendCCDCommand(int,AnsiString,AnsiString)  -- *** NOT GATED ***
//       (a) golden BarCode.cpp:5305-5526; REAL translated body already on disk
//           at BarCode/BarCode_8CCD_Glue.cpp:178, declared
//           BarCode_8CCD_Glue.h:216, and BarCode_8CCD_Glue.cpp is registered in
//           the SAME archive as this file (ht9045_sm, CMakeLists.txt:1953 vs
//           :1920-1953).  Trap 3 applied: a real port exists and is linkable,
//           so the honest answer to "why should this be gated" is "it should
//           not be" -- this unit calls the real one.
//       (b) FAITHFUL because the real body is golden's own: it early-returns on
//           `InitialOK==false`, logs, and dispatches index iBarCode2_1/2_2 to
//           ClientSocket_Shuttle2_A/_B->Socket->SendText only when the socket
//           is ->Active.  Offline the sockets are Sim/inactive, so no I/O
//           happens -- identical observable behaviour to a no-op, PLUS the
//           correct log line.  Its one side effect
//           (`if(Msg2.Pos("LF")) { OCRChangeFileTimeOut...; bChangeSetupFile=true; }`)
//           cannot fire here: every Msg2 this unit passes is "E9,1,%d" /
//           "E2,1,%d,%d" / "E3,1,%d,%d", none of which contains "LF".
//       (c) Real machine: the string actually reaches the CCD controller and
//           the (still-untranslated) ClientSocket_Shuttle*_Read handler
//           eventually sets bSFCGetSE9[]/bSFCExposureOK[]/bSFCStepError[].
//       NOTE for the main loop: this deliberately DIVERGES from the 6 older
//       BarCode TUs that keep their own no-op SendCCDCommand stand-ins.  That
//       divergence is intentional and only forward-looking -- CMakeLists.txt
//       :1934-1950 records the integrate decision NOT to retarget the existing
//       ones (risk of silently changing already-tested behaviour); that
//       reasoning does not apply to a brand-new TU with no prior behaviour.
//
//  G-1  AddCCDCommunicationLog(int,AnsiString) -> SFCAT2_AddCCDCommunicationLog
//       (a) golden BarCode.cpp:5903-5975, declared golden BarCode.h:859.
//       (b) FAITHFUL: golden's body appends to the VCL TMemo `memoCCDCommLog`
//           and to an hourly Big5 log file via MyForceDirectories/
//           WriteDataToFile -- both of which are themselves still `#if 0`
//           "TODO(wave-file)" in the already-translated common.cpp, so the
//           real body is not reachable.  A REAL port body does exist
//           (BarCode_8CCD_Glue.cpp:76 `Glue8CCD_AddCCDCommunicationLog`) but it
//           has INTERNAL linkage (anonymous namespace) and is therefore
//           unreachable from any other TU -- so the trap-3 answer here really
//           is "gate it", and this unit copies that helper's exact chosen
//           behaviour (route to the RecordProcess Sim sink) rather than
//           degrading further to a bare no-op.  Zero control-flow effect: the
//           golden return type is void and no caller in these 3 functions reads
//           anything back.
//       (c) Real machine: the same text lands in the on-screen CCD comm log and
//           in D:\...\ hourly log files.
//
//  G-2  mtSFC_Shuttle->SetCellNumber(int,int,AnsiString) -> SFCAT2_SetSFCGridCell
//       (a) golden BarCode.h:200 (`TTMyTray *mtSFC_Shuttle`), a __published VCL
//           grid widget on the TfBarCode form; call sites golden
//           BarCode_Sh2.cpp:5029/5034/5058/5062/5070/5076/5083/5089 and
//           :5273/5278/5302/5306/5314/5320/5327/5333.
//       (b) FAITHFUL: pure display.  Nothing in either state machine ever reads
//           a cell back (contrast DoBarcodeScanInShuttle_2, which DOES
//           GetCellText -- hence its sibling needed a read stand-in too; this
//           unit does not).  No-op cannot alter any Task transition.
//       (c) Real machine: the operator sees per-step "SFC Exposure..." /
//           "Waiting result.." / "Exposure time out" text in the AT grid.
//
//  G-3  mtBarcodeInSh->SetCellNumber(int,int,AnsiString) -> SFCAT2_GridSetCell
//       (a) golden BarCode.h:145 (`TTMyTray *mtBarcodeInSh`); call sites golden
//           BarCode_Sh2.cpp:5461/5462/5558/5571/5589/5602/5621/5627.
//       (b) FAITHFUL: display only, never read back in this function.  Same
//           rationale + same stand-in shape as the already-integrated
//           BarCode_Shuttle2_ScanRemainder1.cpp:23 `Gated_GridSetCell`.
//       (c) Real machine: the 2DID result grid shows each site's decoded code
//           (or asBarCodeErrorSend on failure).
//
//  G-4  fNote->t2DCode->SetCellNumber / ->SetCellColorIndex
//         -> SFCAT2_Note2DSetCell / SFCAT2_Note2DSetCellColor
//       (a) golden note.h (TfNote VCL form, not translated -- W7); call sites
//           golden BarCode_Sh2.cpp:5469/5470 (the case-1 grid clear).
//       (b) FAITHFUL: display only.  ALSO the trap-4 consideration: `fNote` is
//           one of the not-yet-constructed form pointers; golden new's it in
//           main.cpp, so touching it from a translated TU is exactly the
//           fLaserSensor-style static-init hazard the campaign already paid
//           for.  Gating the CALL SITE (rather than declaring an extern fNote
//           and hoping) is the documented correct handling.  Nothing in this
//           file runs at static-init time anyway -- this TU defines exactly one
//           global, a plain `int`, with constant initialisation and no ctor.
//       (c) Real machine: the Note form's 2D-code board is blanked/decoloured
//           at the start of each 2DID check sweep.
//
//  G-5  TfBarCode::InitSht2DCodeComPort(int) -> SFCAT2_InitSht2DCodeComPort
//       (a) golden BarCode.cpp:2512, declared golden BarCode.h:822; call site
//           golden BarCode_Sh2.cpp:5452.
//       (b) FAITHFUL: it re-opens/resets the CCD serial channel before a retry.
//           With no CCD channel there is nothing to reset, and the surrounding
//           golden code at the same call site re-seeds all of this SM's own
//           channel state (bBarcodeStartDelay[], bBarcodeNum[]) itself.  No-op
//           changes no Task transition.  Same stand-in as
//           BarCode_Shuttle2_ScanRemainder1.cpp:78.
//       (c) Real machine: the COM port is closed+reopened, clearing stale bytes.
//
//  G-6  TfBarCode::Barcode_StartScan_In(int,int,AnsiString&,AnsiString)
//         -> SFCAT2_Barcode_StartScan_In
//       (a) golden BarCode.cpp:2097 (~470 lines), declared golden BarCode.h:796;
//           call sites golden BarCode_Sh2.cpp:5555/5568/5586/5599.
//       (b) FAITHFUL: offline it must report "no code decoded yet" == false.
//           Every call site already carries golden's own full timeout ladder
//           for exactly that case (BarcodeDelay[iSht].Off() -> write
//           asBarCodeErrorSend into cDeviceInf -> WaitManualStepKey()), so
//           returning false walks golden's own documented failure path rather
//           than inventing one.  Returning true would be the unfaithful choice
//           (it would fabricate a decoded barcode).  Same default + same
//           reasoning as BarCode_Shuttle2_ScanRemainder1.cpp:94.
//       (c) Real machine: fires the reader, and on success fills cDeviceInf
//           with the decoded 2DID and returns true.
//
//  NOT GATED, deliberately (real ports exist and are linked in the same
//  archive -- trap 3 checked for each, not assumed):
//    WaitManualStepKey() / WaitManualRetryKey()  -- ckernel.h:97-98, ACTIVE
//        bodies ckernel.cpp:226 / :352, ckernel.cpp registered CMakeLists:1980
//        in the SAME ht9045_sm archive as this file.
//    ShowErrorMessage / ShowMyMessage            -- canary_support.h:66/:80.
//    MOT[].MotorMove / TQPF_Timer                -- Motor/mymotor.h, myTimer.h.
//    SetSFCCheckStepCount()                      -- golden BarCode.cpp:3128 is
//        a trivial 2-line pure calc that sets the very loop bound this SM needs
//        for correctness; every sibling supplies its own faithful file-local
//        mirror (BarCode_Shuttle1_Scan.cpp BarSh1_*, BarCode_Shuttle2_
//        ScanRemainder2.cpp BarSh2_*, BarCode_8CCD_Glue.cpp Glue8CCD_*) because
//        all of those are internal-linkage and thus unreachable across TUs.
//        This unit follows that precedent with SFCAT2_SetSFCCheckStepCount --
//        NOT a gate, a real body.
// =============================================================================
#include "BarCode_Shuttle2_SFCAutoTune.h"

// ---------------------------------------------------------------------------
//  golden BarCode.h:885 -- TfBarCode::iSFCAutoTune2Task.
//  CANONICAL definition (this unit owns it; see the .h banner).  Plain int with
//  constant initialisation -- no dynamic initialiser, no constructor, so it
//  cannot participate in any static-init ordering hazard (trap 4).
//  Golden's TfBarCode ctor leaves it at its zero-initialised value; 0 is not a
//  handled case in DoSFCAutoTune_2's switch, i.e. "idle / not armed", which is
//  exactly golden's own power-on state.
// ---------------------------------------------------------------------------
int iSFCAutoTune2Task = 0;   // golden BarCode.h:885

// ---------------------------------------------------------------------------
//  Local stand-ins + one real helper.  Anonymous namespace == internal linkage.
//  See the GATE REGISTER above for each one's golden home and rationale.
// ---------------------------------------------------------------------------
namespace
{
    // G-1
    void SFCAT2_AddCCDCommunicationLog(int /*index*/, const AnsiString &Str)
    {
        RecordProcess(Str, "BarCode_Sh2_SFCAutoTune");
    }

    // G-2
    void SFCAT2_SetSFCGridCell(int /*index*/, int /*iCol1Based*/, const AnsiString & /*Text*/)
    {
    }

    // G-3
    void SFCAT2_GridSetCell(int /*BarCodeIndex*/, int /*iCol1Based*/, const AnsiString & /*Text*/)
    {
    }

    // G-4
    void SFCAT2_Note2DSetCell(int /*row*/, int /*col*/, const AnsiString & /*Text*/)
    {
    }
    void SFCAT2_Note2DSetCellColor(int /*row*/, int /*col*/, int /*colorIndex*/)
    {
    }

    // G-5
    void SFCAT2_InitSht2DCodeComPort(int /*iStep*/)
    {
    }

    // G-6.  Parameter list mirrors golden BarCode.h:796 exactly, including the
    // by-value 4th parameter.
    bool SFCAT2_Barcode_StartScan_In(int /*BarCodeIndex*/, int /*iXpos*/,
                                     AnsiString & /*cDeviceInf*/, AnsiString /*cLastDeviceInf*/)
    {
        return false;
    }

    // NOT a gate -- faithful mirror of golden BarCode.cpp:3128
    // TfBarCode::SetSFCCheckStepCount().  Deliberately a fresh file-local copy
    // rather than reaching into a sibling TU's internal-linkage twin (those are
    // unreachable across TUs); harmless duplication of a 2-line pure function.
    // int*int stays int*int -- no floating point introduced.
    void SFCAT2_SetSFCCheckStepCount()
    {
        if (TestIF_File.bSFCUse2Photo)                                          //遞減方式檢查
            iSFCTotalMoveStep = InArmSuck.iShtCol * 2;
        else
            iSFCTotalMoveStep = InArmSuck.iShtCol;
    }
}

// =============================================================================
//  BarCode_Sh2_InitialSFCAutoTune2(bool bHasIC)
//  golden BarCode_Sh2.cpp:4880-4886  (TfBarCode::InitialSFCAutoTune2)
//  Arms the Shuttle-2 AT cursor: 1 == "no device" sweep, 10000 == "has device"
//  sweep.  Note golden's polarity -- bHasIC==false selects the NO-device
//  branch, which is the plain reading, but it is the ONLY place the two sweeps
//  are ever distinguished (see DoSFCAutoTune_2's dead bHasIC parameter).
// =============================================================================
void BarCode_Sh2_InitialSFCAutoTune2(bool bHasIC)
{
    if (bHasIC == false)
        iSFCAutoTune2Task = 1;
    else
        iSFCAutoTune2Task = 10000;
}

// =============================================================================
//  BarCode_Sh2_DoSFCAutoTune_2(bool bHasIC)
//  golden BarCode_Sh2.cpp:4888-5417  (TfBarCode::DoSFCAutoTune_2)
//
//  `bHasIC` is DEAD in golden -- the body never reads it (the has-device /
//  no-device split is carried entirely by the Task cursor InitialSFCAutoTune2
//  seeded).  Kept in the signature verbatim, NOT removed and NOT "used".
//
//  See quirks Q1 (iSht==0 on the Shuttle-2 side), Q2 (case 10000 fall-through),
//  Q3 (Task=13000 has no case; case 11300 unreachable) and Q4 (case 1400
//  re-arms Task=1300) in the .h banner -- all preserved below and marked
//  inline.
// =============================================================================
bool BarCode_Sh2_DoSFCAutoTune_2(bool /*bHasIC*/)
{
    static int iNowMoveStep = 0;
    static int iRetryCount = 0;              // golden: assigned, never read
    static int iRetryCountExpTimo = 0;
    static bool bHasError = false;           // golden: assigned, never read
    static AnsiString ErrPart;
    static AnsiString sErrorPart = "";       // golden: declared, never used

    int &Task = iSFCAutoTune2Task;
    int pos = 0, ret = 0;                    // golden `int pos=0, ret;` -- ret is
                                             // always assigned (case 1160/11160)
                                             // before it is read; the explicit 0
                                             // is warning hygiene only, matching
                                             // BarCode_Shuttle2_ScanRemainder2.cpp:69.
    int iSht = 0;                            // GOLDEN QUIRK Q1: 0 on the SHUTTLE-2
                                             // function (its sibling
                                             // DoShuttleFloatCheck_2 uses 1).
                                             // Reproduced, NOT fixed -- every
                                             // iSht-indexed lookup below therefore
                                             // reads Shuttle-1 data.
    int MSht = MInShuttle2;
    int iSFCRowA = iBarCode2_1;
    int iSFCRowB = iBarCode2_2;
    bool bResult = false;
    AnsiString cLastString, sCCDCommand;     // cLastString unused in golden
    AnsiString Log;

    switch (Task)
    {
        //---------------------
        //AT無料的狀況
        //---------------------
        case 1:
            SFCAT2_AddCCDCommunicationLog(iSFCRowA, "<<SFC AT Cycle, Cycle no device start.------------------");

            SFCAT2_SetSFCCheckStepCount();
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
            iRetryCount = 0;
            sCCDCommand.sprintf("E9,1,%d", iSFCTotalMoveStep);                  //清空暫存器
            bSFCGetSE9[iSFCRowA] = false;
            bSFCGetSE9[iSFCRowB] = false;
            SendCCDCommand(iSFCRowA, "SFC AT Clear buffer 1", sCCDCommand);     //Reset CCD Shuttle暫存器
            SendCCDCommand(iSFCRowB, "SFC AT Clear buffer 1", sCCDCommand);
            SFCStartDelay[iSht].SetMSAndOn(5000);
            Task = 500;
            break;
        case 500:                                                               //確認Buffer清空
            if (bSFCGetSE9[iSFCRowA] && bSFCGetSE9[iSFCRowB])
            {
                Task = 1000;
            }
            else if (SFCStartDelay[iSht].Off())
            {
                ShowErrorMessage("WAR0462", K_RETRY, MSht, false, "SFC AT Clear buffer");
                Task = 1;
            }
            break;
        case 1000:
            if (InArmSuck.iShtCol == 1)
            {
                pos = Prod.iInSFCPos1x1[iSht];
            }
            else if (InArmSuck.iShtCol == 2)
            {
                pos = Prod.iInSFCPos1x2[iSht][iNowMoveStep];
            }
            else if (InArmSuck.iShtCol == 3)                                    //ChungHung 20140115 add for 2x3_6
            {
                pos = Prod.iInSFCPos1x3[iSht][iNowMoveStep];
            }
            else if (InArmSuck.iShtCol == 4)
            {
                pos = Prod.iInSFCPos1x4[iSht][iNowMoveStep];
            }
            else if (InArmSuck.iShtCol == 5)                                    //Steven 20221107 : Add for 2x5
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
                ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "DoSFCAutoTune_2");
            }

            if (TestIF_File.bSFCUse2Photo)
            {
                if (iNowMoveStep % 2 == 0)
                    pos += TestIF_File.iSFCUse2PhotoOffset * 100;
                else
                    pos -= TestIF_File.iSFCUse2PhotoOffset * 100;
            }

            // golden `pos+=Offset.iSHLeft2D[iSht];` -- iSHLeft2D is double
            // (cprod.h:250) and pos is int, so C++ compound assignment already
            // computes in double and truncates the SUM on store.  Written with
            // an explicit cast purely to keep this tree's -Wconversion-clean
            // convention (identical semantics), exactly as
            // BarCode_Shuttle2_ScanRemainder1.h:247-261 documented for the same
            // pattern.  NOT a switch to floating-point arithmetic.
            pos = (int)(pos + Offset.iSHLeft2D[iSht]);

            if (MOT[MSht].MotorMove(pos))
            {
                Log.sprintf("SFC AT Action, Shuttle %d move to step %d", iSht + 1, iNowMoveStep);
                SFCAT2_AddCCDCommunicationLog(iSFCRowA, Log);
                SFCStartDelay[iSht].SetMSAndOn(TestIF_File.iSFCStartDelay);
                Task = 1120;
            }
            break;
        case 1120:
            if (SFCStartDelay[iSht].Off())                                      //到位Delay
            {
                bSFCExposureOK[iSFCRowA] = false;
                if (InArmSuck.iShtRow == 2)
                    bSFCExposureOK[iSFCRowB] = false;
                else
                    bSFCExposureOK[iSFCRowB] = true;

                //--------------------------------------
                //SE   1,        X,    1
                //拍照 偵測置偏, 位置, 左中右
                //--------------------------------------
                if (TestIF_File.bSFCUse2Photo)
                {
                    if (iNowMoveStep % 2 == 0)
                        sCCDCommand.sprintf("E2,1,%d,0", iNowMoveStep);
                    else
                        sCCDCommand.sprintf("E2,1,%d,2", iNowMoveStep);
                }
                else
                    sCCDCommand.sprintf("E2,1,%d,1", iNowMoveStep);

                SFCAT2_SetSFCGridCell(iSFCRowA, iNowMoveStep + 1, "SFC Exposure...");
                SendCCDCommand(iSFCRowA, "SFC AT exposure 1120: ", sCCDCommand);

                if (InArmSuck.iShtRow == 2)
                {
                    SFCAT2_SetSFCGridCell(iSFCRowB, iNowMoveStep + 1, "SFC Exposure...");
                    SendCCDCommand(iSFCRowB, "SFC AT exposure 1120: ", sCCDCommand);
                }

                Task = 1150;
                iRetryCountExpTimo = 0;                                         //Steven 20160425 : 修改拍照Time Out
                SFCExposureDelay[iSht].SetMSAndOn(TestIF_File.iSFCExposureTimeOut);
            }
            break;
        case 1150:
            if (bSFCStepError[iSFCRowA] == true || bSFCStepError[iSFCRowB] == true) //Steven 20160503 : 確認移動位置與拍照位置相同
            {
                ErrPart = "";
                if (bSFCStepError[iSFCRowA])
                    ErrPart += IndexSuckName[0][iNowMoveStep];

                if (bSFCStepError[iSFCRowB])
                    ErrPart += IndexSuckName[1][iNowMoveStep];

                Task = 1180;
            }
            else if (bSFCExposureOK[iSFCRowA] == true &&
                     bSFCExposureOK[iSFCRowB] == true)                          //拍照完成
            {
                SFCAT2_SetSFCGridCell(iSFCRowA, iNowMoveStep + 1, "Waiting result..");

                if (InArmSuck.iShtRow == 2)
                {
                    SFCAT2_SetSFCGridCell(iSFCRowB, iNowMoveStep + 1, "Waiting result..");
                }
                Task = 1200;
            }
            else if (SFCExposureDelay[iSht].Off())                              //拍照time out
            {
                if (bSFCStepError[iSFCRowA] == false)
                {
                    SFCAT2_SetSFCGridCell(iSFCRowA, iNowMoveStep + 1, "Exposure time out");
                    Log.sprintf("SFC AT Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                    SFCAT2_AddCCDCommunicationLog(iSFCRowA, Log);
                }
                else
                {
                    SFCAT2_SetSFCGridCell(iSFCRowA, iNowMoveStep + 1, "Waiting result..");
                }

                if (InArmSuck.iShtRow == 2)
                {
                    if (bSFCStepError[iSFCRowB] == false)
                    {
                        SFCAT2_SetSFCGridCell(iSFCRowB, iNowMoveStep + 1, "Exposure time out");
                        Log.sprintf("SFC AT Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                        SFCAT2_AddCCDCommunicationLog(iSFCRowB, Log);
                    }
                    else
                    {
                        SFCAT2_SetSFCGridCell(iSFCRowB, iNowMoveStep + 1, "Waiting result..");
                    }
                }
                iRetryCountExpTimo++;                                           //Steven 20160425 : 修改拍照Time Out
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
                ret = ShowErrorMessage("WAR0464", K_RETRY | K_SKIP, MSht);       //2D function CCD 3 or 4 Exposure time out!
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
            if (TestIF_File.bSFCUse2Photo)
            {
                if (iNowMoveStep % 2 == 0)
                    sCCDCommand.sprintf("E2,1,%d,0", iNowMoveStep);
                else
                    sCCDCommand.sprintf("E2,1,%d,2", iNowMoveStep);
            }
            else
                sCCDCommand.sprintf("E2,1,%d,1", iNowMoveStep);
            if (bSFCStepError[iSFCRowA] == false)
            {
                SendCCDCommand(iSFCRowA, "SFC AT exposure 1170: ", sCCDCommand);
            }

            if (InArmSuck.iShtRow == 2)
            {
                if (bSFCStepError[iSFCRowB] == false)
                {
                    SendCCDCommand(iSFCRowB, "SFC AT exposure 1170: ", sCCDCommand);
                }
            }
            Task = 1150;
            SFCExposureDelay[iSht].SetMSAndOn(TestIF_File.iSFCExposureTimeOut);
            break;
        case 1180:
            Log.sprintf("SFC AT Alarm, In shuttle %d exposure position error!", iSht + 1);
            SFCAT2_AddCCDCommunicationLog(iSFCRowA, Log);
            ShowErrorMessage("WAR0470", K_RETRY, MSht, false, ErrPart);          //In shuttle 2 exposure position error!
            Task = 1;
            break;
        case 1200:
            iNowMoveStep--;
            iSFCCurrentStep[iSFCRowA] = iNowMoveStep;                           //Steven 20160503 : 確認移動位置與拍照位置相同
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
            if (MOT[MSht].MotorMove(Prod.OutSHT[1].iLeft))
            {
                Task = 1400;
            }
            break;
        case 1400:
            SFCAT2_AddCCDCommunicationLog(iSFCRowA, "<<SFC AT Cycle, Cycle no device end.------------------");
            Task = 1300;                                                        // GOLDEN QUIRK Q4: re-arms 1300, so the
                                                                                // no-device cycle ping-pongs 1300<->1400
                                                                                // forever.  Its has-device twin (case
                                                                                // 11400) does NOT reassign Task.
                                                                                // Reproduced, NOT fixed.
            bResult = true;
            break;
        //---------------------
        //AT有料的狀況
        //---------------------
        case 10000:
            SFCAT2_AddCCDCommunicationLog(iSFCRowA, "<<SFC AT Cycle, Cycle has device start.------------------");

            SFCAT2_SetSFCCheckStepCount();
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
            iRetryCount = 0;
            Task = 11000;
            // GOLDEN QUIRK Q2: golden BarCode_Sh2.cpp:5197 has NO `break` here
            // -- case 10000 deliberately FALLS THROUGH into case 11000 so the
            // first move happens in the same pump.  Preserved verbatim.
            // fall through
        case 11000:
            if (InArmSuck.iShtCol == 1)
            {
                pos = Prod.iInSFCPos1x1[iSht];
            }
            else if (InArmSuck.iShtCol == 2)
            {
                pos = Prod.iInSFCPos1x2[iSht][iNowMoveStep];
            }
            else if (InArmSuck.iShtCol == 3)                                    //ChungHung 20140115 add for 2x3_6
            {
                pos = Prod.iInSFCPos1x3[iSht][iNowMoveStep];
            }
            else if (InArmSuck.iShtCol == 4)
            {
                pos = Prod.iInSFCPos1x4[iSht][iNowMoveStep];
            }
            else if (InArmSuck.iShtCol == 5)                                    //Steven 20221107 : Add for 2x5
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
                ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "DoSFCAutoTune_2");
            }

            if (TestIF_File.bSFCUse2Photo)
            {
                if (iNowMoveStep % 2 == 0)
                    pos += TestIF_File.iSFCUse2PhotoOffset * 100;
                else
                    pos -= TestIF_File.iSFCUse2PhotoOffset * 100;
            }

            pos = (int)(pos + Offset.iSHLeft2D[iSht]);   // golden `pos+=Offset.iSHLeft2D[iSht];` -- see case 1000

            if (MOT[MSht].MotorMove(pos))
            {
                Log.sprintf("SFC AT Action, Shuttle %d move to step %d", iSht + 1, iNowMoveStep);
                SFCAT2_AddCCDCommunicationLog(iSFCRowA, Log);
                SFCStartDelay[iSht].SetMSAndOn(TestIF_File.iSFCStartDelay);
                Task = 11120;
            }
            break;
        case 11120:
            if (SFCStartDelay[iSht].Off())                                      //到位Delay
            {
                bSFCExposureOK[iSFCRowA] = false;
                if (InArmSuck.iShtRow == 2)
                    bSFCExposureOK[iSFCRowB] = false;
                else
                    bSFCExposureOK[iSFCRowB] = true;

                //--------------------------------------
                //SE   1,        X,    1
                //拍照 偵測置偏, 位置, 左中右
                //--------------------------------------
                if (TestIF_File.bSFCUse2Photo)
                {
                    if (iNowMoveStep % 2 == 0)
                        sCCDCommand.sprintf("E3,1,%d,0", iNowMoveStep);
                    else
                        sCCDCommand.sprintf("E3,1,%d,2", iNowMoveStep);
                }
                else
                    sCCDCommand.sprintf("E3,1,%d,1", iNowMoveStep);

                SFCAT2_SetSFCGridCell(iSFCRowA, iNowMoveStep + 1, "SFC AT Exposure...");
                SendCCDCommand(iSFCRowA, "SFC AT exposure 1120: ", sCCDCommand); // golden passes the "1120" tag here too,
                                                                                 // in the 11120 case.  Verbatim.

                if (InArmSuck.iShtRow == 2)
                {
                    SFCAT2_SetSFCGridCell(iSFCRowB, iNowMoveStep + 1, "SFC AT Exposure...");
                    SendCCDCommand(iSFCRowB, "SFC AT exposure 1120: ", sCCDCommand);
                }

                Task = 11150;
                iRetryCountExpTimo = 0;                                         //Steven 20160425 : 修改拍照Time Out
                SFCExposureDelay[iSht].SetMSAndOn(TestIF_File.iSFCExposureTimeOut);
            }
            break;
        case 11150:
            if (bSFCStepError[iSFCRowA] == true || bSFCStepError[iSFCRowB] == true) //Steven 20160503 : 確認移動位置與拍照位置相同
            {
                ErrPart = "";
                if (bSFCStepError[iSFCRowA])
                    ErrPart += IndexSuckName[0][iNowMoveStep];

                if (bSFCStepError[iSFCRowB])
                    ErrPart += IndexSuckName[1][iNowMoveStep];

                Task = 11180;
            }
            else if (bSFCExposureOK[iSFCRowA] == true &&
                     bSFCExposureOK[iSFCRowB] == true)                          //拍照完成
            {
                SFCAT2_SetSFCGridCell(iSFCRowA, iNowMoveStep + 1, "Waiting result..");

                if (InArmSuck.iShtRow == 2)
                {
                    SFCAT2_SetSFCGridCell(iSFCRowB, iNowMoveStep + 1, "Waiting result..");
                }
                Task = 11200;
            }
            else if (SFCExposureDelay[iSht].Off())                              //拍照time out
            {
                if (bSFCStepError[iSFCRowA] == false)
                {
                    SFCAT2_SetSFCGridCell(iSFCRowA, iNowMoveStep + 1, "Exposure time out");
                    Log.sprintf("SFC AT Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                    SFCAT2_AddCCDCommunicationLog(iSFCRowA, Log);
                }
                else
                {
                    SFCAT2_SetSFCGridCell(iSFCRowA, iNowMoveStep + 1, "Waiting result..");
                }

                if (InArmSuck.iShtRow == 2)
                {
                    if (bSFCStepError[iSFCRowB] == false)
                    {
                        SFCAT2_SetSFCGridCell(iSFCRowB, iNowMoveStep + 1, "Exposure time out");
                        Log.sprintf("SFC AT Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                        SFCAT2_AddCCDCommunicationLog(iSFCRowB, Log);
                    }
                    else
                    {
                        SFCAT2_SetSFCGridCell(iSFCRowB, iNowMoveStep + 1, "Waiting result..");
                    }
                }
                iRetryCountExpTimo++;                                           //Steven 20160425 : 修改拍照Time Out
                Task = 11160;
            }
            break;
        case 11160:
            if (iRetryCountExpTimo < 3)
            {
                Task = 11170;
            }
            else
            {
                ret = ShowErrorMessage("WAR0464", K_RETRY | K_SKIP, MSht);       //2D function CCD 3 or 4 Exposure time out!
                if (ret == K_SKIP)
                {
                    Task = 11200;
                }
                else
                {
                    Task = 11170;
                }
                iRetryCountExpTimo = 0;
            }
            break;
        case 11170:
            if (TestIF_File.bSFCUse2Photo)
            {
                if (iNowMoveStep % 2 == 0)
                    sCCDCommand.sprintf("E3,1,%d,0", iNowMoveStep);
                else
                    sCCDCommand.sprintf("E3,1,%d,2", iNowMoveStep);
            }
            else
                sCCDCommand.sprintf("E3,1,%d,1", iNowMoveStep);
            if (bSFCStepError[iSFCRowA] == false)
            {
                SendCCDCommand(iSFCRowA, "SFC AT exposure 1170: ", sCCDCommand);
            }

            if (InArmSuck.iShtRow == 2)
            {
                if (bSFCStepError[iSFCRowB] == false)
                {
                    SendCCDCommand(iSFCRowB, "SFC AT exposure 1170: ", sCCDCommand);
                }
            }
            Task = 11150;
            SFCExposureDelay[iSht].SetMSAndOn(TestIF_File.iSFCExposureTimeOut);
            break;
        case 11180:
            Log.sprintf("SFC AT Alarm, In shuttle %d exposure position error!", iSht + 1);
            SFCAT2_AddCCDCommunicationLog(iSFCRowA, Log);
            ShowErrorMessage("WAR0470", K_RETRY, MSht, false, ErrPart);          //In shuttle 2 exposure position error!
            Task = 10000;
            break;
        case 11200:
            iNowMoveStep--;
            iSFCCurrentStep[iSFCRowA] = iNowMoveStep;                           //Steven 20160503 : 確認移動位置與拍照位置相同
            iSFCCurrentStep[iSFCRowB] = iNowMoveStep;

            if (iNowMoveStep >= 0 && iNowMoveStep < iSFCTotalMoveStep)
            {
                Task = 11000;
            }
            else
            {
                Task = 13000;   // GOLDEN QUIRK Q3: there is NO `case 13000` in this
                                // function, so the has-device sweep parks at an
                                // unhandled Task and never reaches 11300/11400.
                                // Reproduced, NOT fixed.
            }
            break;
        case 11300:             // GOLDEN QUIRK Q3 (cont.): UNREACHABLE -- nothing in
                                // this function ever assigns Task=11300.  Kept
                                // verbatim rather than deleted.
            if (MOT[MSht].MotorMove(Prod.OutSHT[1].iLeft))
            {
                Task = 11400;
            }
            break;
        case 11400:
            SFCAT2_AddCCDCommunicationLog(iSFCRowA, "<<SFC AT Cycle, Cycle has device end.------------------");
            bResult = true;
            break;
    }

    return bResult;
}

// =============================================================================
//  BarCode_Sh2_Do2DIDCheckSh2()
//  golden BarCode_Sh2.cpp:5419-5664  (TfBarCode::Do2DIDCheckSh2)
//
//  Operator-driven Shuttle-2 2D-reader verification sweep.  Note it shares the
//  golden TfBarCode member cursor `iInitialBarcodeInShuttle2Task` with
//  DoBarcodeTriggerInShuttle_2 / DoBarcodeScanInShuttle_2 -- golden really does
//  alias the same member across those mutually-exclusive sweeps, so this port
//  reuses BarCode_Shuttle2_Scan.cpp's canonical definition rather than forking.
//
//  See quirks Q5 (iSht==0 timers), Q6 (RowB result written to RowA's grid cell)
//  and Q7 (case 1200 falls through to 1300) in the .h banner.
//
//  OFFLINE BEHAVIOUR (consequence of gate G-6, stated so it is not mistaken for
//  a translation defect): SFCAT2_Barcode_StartScan_In always returns false, so
//  bBarcodeNum[] never latches true from a decode; the sweep therefore always
//  takes golden's OWN documented timeout path -- BarcodeDelay[iSht].Off() ->
//  write asBarCodeErrorSend into BLCarryKit.cDeviceInf -> wait for the operator
//  Step key.  It never escapes its switch(Task) universe.
// =============================================================================
bool BarCode_Sh2_Do2DIDCheckSh2()
{
    int &Task = iInitialBarcodeInShuttle2Task;
    int iSht = 0;                                       // GOLDEN QUIRK Q5: 0 on the
                                                        // Shuttle-2 sweep -- only the
                                                        // two timers below use it (every
                                                        // Prod/Offset lookup is hard-coded
                                                        // [1]).  Reproduced, NOT fixed.
    int pos = 0;
    static int iNowCheckStep = 0;
    static int iRetryCount = 0;                         // golden: assigned, never read
    static bool bBarcodeNum[BAR_CODE_COUNT] = {false};
    AnsiString DataStr, Path, Data;                     // golden: declared, never used
    AnsiString cLastString;
    static AnsiString sErrorPart = "", sCheckErrorPart = "";   // golden: assigned, never read
    AnsiString asString;                                // golden: declared, never used
    int iBarCodeRowA = iBarCode2_1;
    int iBarCodeRowB = iBarCode2_2;

    if (InArmSuck.iShtRow == 1)
    {
        // GOLDEN QUIRK Q8: no `else if(BAR_CODE_INSTALL==ebcUseOCR)` arm here,
        // unlike the sibling DoBarcodeScanInShuttle_2 (golden :3072).  Verbatim.
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

    switch (Task)
    {
        case 1:
            iNowCheckStep = InArmSuck.iShtCol - 1;     //遞減方式檢查
            SFCAT2_InitSht2DCodeComPort(iSht);         //Ifor 20160322 Retry 時重置
            bBarcodeStartDelay[iBarCodeRowA] = true;
            if (InArmSuck.iShtRow == 2)
            {
                bBarcodeStartDelay[iBarCodeRowB] = true;
            }

            for (int i = 0; i < 8; i++)
            {
                SFCAT2_GridSetCell(iBarCodeRowA, i + 1, "");
                SFCAT2_GridSetCell(iBarCodeRowB, i + 1, "");
            }

            for (int i = 0; i < InArmSuck.iShtRow; i++)
            {
                for (int j = 0; j < InArmSuck.iShtCol; j++)
                {
                    SFCAT2_Note2DSetCell(i, j, "");
                    SFCAT2_Note2DSetCellColor(i, j, 0);
                }
            }
            bBarcodeNum[iBarCodeRowA] = false;
            bBarcodeNum[iBarCodeRowB] = false;
            Task = 1000;
            break;
        case 1000:
            if (iNowCheckStep >= 0 && iNowCheckStep < InArmSuck.iShtCol)
            {
                if (InArmSuck.iShtCol == 1)
                {
                    pos = Prod.iInSHBarCodeDetectPos1x1[1];
                }
                else if (InArmSuck.iShtCol == 2)
                {
                    pos = Prod.iInSHBarCodeDetectPos1x2[1][iNowCheckStep];
                }
                else if (InArmSuck.iShtCol == 3)  //ChungHung 20140115 add for 2x3_6
                {
                    pos = Prod.iInSHBarCodeDetectPos2x3[1][iNowCheckStep];
                }
                else if (InArmSuck.iShtCol == 4)
                {
                    pos = Prod.iInSHBarCodeDetectPos1x4[1][iNowCheckStep];
                }
                else if (InArmSuck.iShtCol == 5)  //Steven 20221027 : Add for 2x5
                {
                    pos = Prod.iInSHBarCodeDetectPos2x5[1][iNowCheckStep];
                }
                else if (InArmSuck.iShtCol == 6)  //Steven 20170111 : fixed for 2x6
                {
                    pos = Prod.iInSHBarCodeDetectPos2x6[1][iNowCheckStep];
                }
                else if (InArmSuck.iShtCol == 8)
                {
                    pos = Prod.iInSHBarCodeDetectPos2x8[1][iNowCheckStep];
                }
                else
                {
                    ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "Do2DIDCheckSh2");
                }

                pos = (int)(pos + Offset.iSHLeft2D[1]);  //Steven 20151218 : Offset for 2d reader
                                                         // golden `pos+=Offset.iSHLeft2D[1];` -- explicit
                                                         // cast only, identical semantics (see case 1000
                                                         // of DoSFCAutoTune_2 above).

                if (MOT[MInShuttle2].MotorMove(pos))
                {
                    bBarcodeStartDelay[iBarCodeRowA] = true;
                    if (InArmSuck.iShtRow == 2)
                    {
                        bBarcodeStartDelay[iBarCodeRowB] = true;
                    }
                    BarcodePosDelay[iSht].SetMSAndOn(TestIF_File.iBarCodePosDelay);

                    if (WaitManualRetryKey())
                    {
                        Task = 1000;
                    }
                    else if (WaitManualStepKey())
                    {
                        Task = 1120;
                    }
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
                BarcodeDelay[iSht].SetMSAndOn(TestIF_File.iBarCodeDelay);
                sErrorPart = "";
                sCheckErrorPart = "";
                Task = 1150;
            }
            break;
        case 1150:
            if (BAR_CODE_INSTALL == ebcUseOCR)//Ifor 20211129 OCR Shuttle1 Row A 使用第2隻CCD Row B使用第1隻CCD
            {
                if (bBarcodeNum[iBarCodeRowB] == false)
                {
                    iSH2_1BarcodePosition = iNowCheckStep;

                    if (SFCAT2_Barcode_StartScan_In(iBarCodeRowB, iNowCheckStep, BLCarryKit.cDeviceInf[0][iNowCheckStep], cLastString) == true)
                    {
                        bBarcodeNum[iBarCodeRowB] = true;
                        SFCAT2_GridSetCell(iBarCodeRowB, iNowCheckStep + 1, BLCarryKit.cDeviceInf[0][iNowCheckStep]);
                    }
                }

                if (InArmSuck.iShtRow == 2)
                {
                    if (bBarcodeNum[iBarCodeRowA] == false)
                    {
                        iSH2_2BarcodePosition = iNowCheckStep;

                        if (SFCAT2_Barcode_StartScan_In(iBarCodeRowA, iNowCheckStep, BLCarryKit.cDeviceInf[1][iNowCheckStep], cLastString) == true)
                        {
                            bBarcodeNum[iBarCodeRowA] = true;
                            SFCAT2_GridSetCell(iBarCodeRowA, iNowCheckStep + 1, BLCarryKit.cDeviceInf[1][iNowCheckStep]);
                        }
                    }
                }
                else
                {
                    bBarcodeNum[iBarCodeRowA] = true;
                }
            }
            else    //Shuttle 1, Row A使用第1隻CCD, Row B使用第2隻CCD
            {
                if (bBarcodeNum[iBarCodeRowA] == false)
                {
                    iSH2_1BarcodePosition = iNowCheckStep;

                    if (SFCAT2_Barcode_StartScan_In(iBarCodeRowA, iNowCheckStep, BLCarryKit.cDeviceInf[0][iNowCheckStep], cLastString) == true)
                    {
                        bBarcodeNum[iBarCodeRowA] = true;
                        SFCAT2_GridSetCell(iBarCodeRowA, iNowCheckStep + 1, BLCarryKit.cDeviceInf[0][iNowCheckStep]);
                    }
                }

                if (InArmSuck.iShtRow == 2)
                {
                    if (bBarcodeNum[iBarCodeRowB] == false)
                    {
                        iSH2_2BarcodePosition = iNowCheckStep;

                        if (SFCAT2_Barcode_StartScan_In(iBarCodeRowB, iNowCheckStep, BLCarryKit.cDeviceInf[1][iNowCheckStep], cLastString) == true)
                        {
                            bBarcodeNum[iBarCodeRowB] = true;
                            // GOLDEN QUIRK Q6 (golden :5602): the SUCCESSFUL Row-B
                            // decode is displayed on row iBarCodeRowA, overwriting
                            // Row A's cell.  Every other arm here uses its own row.
                            // Reproduced verbatim, NOT fixed.
                            SFCAT2_GridSetCell(iBarCodeRowA, iNowCheckStep + 1, BLCarryKit.cDeviceInf[1][iNowCheckStep]);
                        }
                    }
                }
                else
                {
                    bBarcodeNum[iBarCodeRowB] = true;
                }
            }

            if (bBarcodeNum[iBarCodeRowA] == true && bBarcodeNum[iBarCodeRowB] == true)
            {
                Task = 1200;
            }
            else if (BarcodeDelay[iSht].Off())
            {
                if (bBarcodeNum[iBarCodeRowA] == false)
                {
                    BLCarryKit.cDeviceInf[0][iNowCheckStep] = asBarCodeErrorSend;
                    SFCAT2_GridSetCell(iBarCodeRowA, iNowCheckStep + 1, asBarCodeErrorSend.c_str());
                }

                if (bBarcodeNum[iBarCodeRowB] == false)
                {
                    BLCarryKit.cDeviceInf[1][iNowCheckStep] = asBarCodeErrorSend;
                    SFCAT2_GridSetCell(iBarCodeRowB, iNowCheckStep + 1, asBarCodeErrorSend.c_str());
                }

                if (WaitManualStepKey())
                {
                    Task = 1200;
                }
            }
            break;
        case 1200:
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
                Task = 1300;
            }
            // GOLDEN QUIRK Q7: golden BarCode_Sh2.cpp:5652 has no `break` on the
            // else path -- it FALLS THROUGH into case 1300 in the same pump.
            // Preserved verbatim.
            // fall through
        case 1300:
            if (MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iLeft))
            {
                Task = 2000;
            }
            break;
        case 2000:
            Task = 0;
            return true;
    }
    return false;
}
