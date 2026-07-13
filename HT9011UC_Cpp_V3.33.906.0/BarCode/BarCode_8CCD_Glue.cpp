// =============================================================================
//  BarCode/BarCode_8CCD_Glue.cpp  --  see BarCode_8CCD_Glue.h for full scope/
//  gating/shared-state notes.
//
//  Translator: AI(W5-Final-BarCode8CCDGlue) 20260711
// =============================================================================
#include "BarCode_8CCD_Glue.h"

#include "cprod.h"           // TestIF_File
#include "cmydef.h"          // BAR_CODE_INSTALL, BOTTOM_2DID, BOTTOM_2DID_CCD, InitialOK
#include "MachineType.h"     // ebctUseCCDMode, ebcUseOCR
#include "aHotPlateSubstrate.h" // InArmSuck (TMyKitSuck)
#include "canary_support.h" // RecordProcess
#include "cpublic.h"         // GetTimeInfo()

// =============================================================================
//  FILE-LOCAL (internal-linkage) helpers + state -- private to this TU, no ODR
//  risk even though several sibling TUs define identically-shaped helpers
//  under different names (see .h banner "PRE-EXISTING GATED STAND-INS").
// =============================================================================
namespace
{
    // -------------------------------------------------------------------
    //  dVisionVer -- golden BarCode.h:720 (TfBarCode class member, double).
    //  Genuinely a SINGLE shared golden field (fBarCode->dVisionVer), but
    //  per this wave's established precedent (BarCode_Bottom2DID.cpp,
    //  BarCode_Bottom2DID8CCD.cpp, BarCode_Shuttle1_CCDScan.cpp each already
    //  keep their OWN anonymous-namespace copy rather than forking the ONE
    //  canonical extern-linkage owner in BarCode_Shuttle2_CCDScan.cpp), this
    //  unit adds a 4th independent TU-local copy.  Initial value matches the
    //  real golden ctor (BarCode.cpp:276 `dVisionVer=1;`, SOFT_SIMULTE not
    //  defined in this build) more closely than the other three units' picks
    //  (2.0 / 1 / 0.0 respectively) -- flagged in the translate report as a
    //  pre-existing, now-4x, state-fragmentation concern, NOT introduced by
    //  this unit's design choice (following established precedent, not
    //  inventing a new pattern).
    // -------------------------------------------------------------------
    double dVisionVer = 1.0;

    // -------------------------------------------------------------------
    //  Glue8CCD_SetSFCCheckStepCount() -- faithful mirror of golden
    //  BarCode.cpp:3128 TfBarCode::SetSFCCheckStepCount() (trivial 2-line
    //  pure calc).  Writes the CANONICAL shared `iSFCTotalMoveStep` extern
    //  (owned by BarCode_Shuttle1_Scan.cpp, reused via BarCode_Bottom2DID.h's
    //  transitive include) -- NOT a forked local copy, since
    //  DoShuttleFloatCheck_1/_2 consume the SAME physical value this unit's
    //  Init-timer Shuttle-Float-Check branch also seeds.  3rd independent
    //  mirror of this same trivial function in the tree (siblings:
    //  BarSh1_SetSFCCheckStepCount / BarSh2_SetSFCCheckStepCount, both
    //  undeclared in their own headers and therefore unreachable from this
    //  TU) -- unique name avoids any ODR question.
    // -------------------------------------------------------------------
    void Glue8CCD_SetSFCCheckStepCount()
    {
        if (TestIF_File.bSFCUse2Photo)                                          //遞減方式檢查
            iSFCTotalMoveStep = InArmSuck.iShtCol * 2;
        else
            iSFCTotalMoveStep = InArmSuck.iShtCol;
    }

    // -------------------------------------------------------------------
    //  Glue8CCD_AddCCDCommunicationLog -- golden BarCode.cpp:5903-5975
    //  (TfBarCode::AddCCDCommunicationLog).  NOT one of this unit's 3
    //  assigned functions (called throughout SendCCDCommand/both Timer
    //  functions purely for state-transition logging -- zero control-flow
    //  effect, return type void).  Real body appends to `memoCCDCommLog`
    //  (VCL TMemo, out of scope) + an hourly Big5 log file via
    //  MyForceDirectories/WriteDataToFile (both themselves gated
    //  `#if 0 TODO(wave-file)` in common.h -- not actually callable yet).
    //  Routed to the already-available RecordProcess Sim sink instead,
    //  matching the EXACT precedent BarCode_Bottom2DID.cpp/
    //  BarCode_Bottom2DID8CCD.cpp already established for this identical
    //  golden method.  Internal linkage (anonymous namespace) -- no
    //  conflict with those TUs' own same-shaped stand-ins.
    // -------------------------------------------------------------------
    void Glue8CCD_AddCCDCommunicationLog(int /*index*/, const AnsiString& Str)
    {
        RecordProcess(Str, "BarCode_8CCD_Glue");
    }

    // -------------------------------------------------------------------
    //  Glue8CCD_LogClientSocketExceptionError -- golden
    //  Public/WinSocketErrorCode.h `LogClientSocketExceptionError` (DEFERRED
    //  tree-wide -- couples to VCL TClientSocket ->Name/Address/Port +
    //  MyDBIProcess, see WinSocketErrorCode.h's own DEFERRED note and
    //  MIGRATION_ROADMAP.md's DEFERRED table).  Only ever reached from a
    //  catch(...) block guarding a real-socket ->Open()/->Close() call; this
    //  shim's TClientSocket::Open()/Close() are documented no-throw (see
    //  vclcompat/ClientSocket.h's Sim/Real BACKING note), so in THIS build
    //  these catch blocks are unreachable dead code -- preserved verbatim
    //  for structural fidelity (a future Real-mode build, or a future
    //  faithful throw-on-failure revision of the shim, would make them live
    //  again).  Routed to RecordProcess as a plain log line (no DB write).
    // -------------------------------------------------------------------
    void Glue8CCD_LogClientSocketExceptionError(void* /*Sender*/, const AnsiString& where)
    {
        RecordProcess("ClientSocketException: " + where, "BarCode_8CCD_Glue");
    }

    // -------------------------------------------------------------------
    //  Glue8CCD_GetSetupFileNameText / Glue8CCD_GetBarcodeRecipeText --
    //  stand-ins for `fMain->cbSetupFileName->Text` (golden main.h, TComboBox*)
    //  and `fLotInfo->edtBarcodeRecipe->Text` (golden uLotInfo.h, TEdit*),
    //  golden BarCode.cpp:9553/9556.  Neither FormsFacade::TfMain nor
    //  FormsFacade::TfLotInfo has these members yet (grep-verified) -- gated
    //  with a conservative "no active recipe name" default (empty
    //  AnsiString), matching this project's established "cosmetic UI read,
    //  offline no-op" gating style.  See translate report for the EXACT
    //  FormsFacade wiring the integrate agent needs to add (reusing the
    //  already-established TfLotInfoEdit {AnsiString Text;} shape for
    //  edtBarcodeRecipe, and an analogous new struct for cbSetupFileName).
    // -------------------------------------------------------------------
#if 0 // TODO(W7-FormsFacade) golden BarCode.cpp:9553 -- fMain->cbSetupFileName->Text (TComboBox*, no FormsFacade member yet)
    AnsiString Glue8CCD_GetSetupFileNameText() { return fMain->cbSetupFileName->Text; }
#endif
    AnsiString Glue8CCD_GetSetupFileNameText() { return AnsiString(""); }

#if 0 // TODO(W7-FormsFacade) golden BarCode.cpp:9553 -- fLotInfo->edtBarcodeRecipe->Text (TEdit*, no FormsFacade member yet)
    AnsiString Glue8CCD_GetBarcodeRecipeText() { return fLotInfo->edtBarcodeRecipe->Text; }
#endif
    AnsiString Glue8CCD_GetBarcodeRecipeText() { return AnsiString(""); }

    // -------------------------------------------------------------------
    //  MakeCcdSocket -- construction helper for the 13 real sockets below.
    //  Mirrors golden BarCode.cpp:190-ish per-instance `Ptr->Tag=i;` seeding
    //  (BarCode.dfm equivalent) done once at "form load" time; here, at
    //  static-init time (same precedent as FormsFacade.cpp's
    //  `TfAGV *fAGV = new TfAGV();`).
    // -------------------------------------------------------------------
    Scktcomp::TClientSocket* MakeCcdSocket(int tag)
    {
        Scktcomp::TClientSocket* p = new Scktcomp::TClientSocket(NULL);
        p->Tag = tag;
        return p;
    }
} // anonymous namespace

// =============================================================================
//  The 13 real TClientSocket instances (golden BarCode.h:127-130,134,300-307).
//  Tag values verified directly against golden BarCode.dfm (see .h banner).
// =============================================================================
Scktcomp::TClientSocket* ClientSocket_Shuttle1_A        = MakeCcdSocket(0);
Scktcomp::TClientSocket* ClientSocket_Shuttle1_B        = MakeCcdSocket(1);
Scktcomp::TClientSocket* ClientSocket_Shuttle2_A        = MakeCcdSocket(2);
Scktcomp::TClientSocket* ClientSocket_Shuttle2_B        = MakeCcdSocket(3);
Scktcomp::TClientSocket* ClientSocket_BarcodeChangeFile = MakeCcdSocket(0);
Scktcomp::TClientSocket* ClientSocket_Bottom_1          = MakeCcdSocket(0);
Scktcomp::TClientSocket* ClientSocket_Bottom_2          = MakeCcdSocket(1);
Scktcomp::TClientSocket* ClientSocket_Bottom_3          = MakeCcdSocket(2);
Scktcomp::TClientSocket* ClientSocket_Bottom_4          = MakeCcdSocket(3);
Scktcomp::TClientSocket* ClientSocket_Bottom_5          = MakeCcdSocket(4);
Scktcomp::TClientSocket* ClientSocket_Bottom_6          = MakeCcdSocket(5);
Scktcomp::TClientSocket* ClientSocket_Bottom_7          = MakeCcdSocket(6);
Scktcomp::TClientSocket* ClientSocket_Bottom_8          = MakeCcdSocket(7);

// =============================================================================
//  NEW SINGLE-OWNER GLOBALS -- see .h banner.  Defaults mirror golden
//  zero-init (bool/int/AnsiString default-ctor) -- none of these are seeded
//  by a golden FormCreate this unit translates (that ctor logic is out of
//  scope; see .h banner "NOT in scope").
// =============================================================================
bool        bSFCGetVR                              = false;
int         iBottomConntectionOkTask1               = 0;
int         iBottomConntectionOkTask2               = 0;
bool        bEnableCCDChannelConnect[BAR_CODE_COUNT] = {false};
TQPF_Timer  tChangeSetupFile;
bool        bGetGF                                  = false;
bool        bGetLF                                  = false;
bool        bSFCGetGF                               = false;
bool        bSFCGetLF                               = false;
AnsiString  sSFCCurrentGF;
bool        bChangeSetupFile                        = false;
TQPF_Timer  OCRChangeFileTimeOut;

// =============================================================================
//  SendCCDCommand -- golden BarCode.cpp:5305-5526.
// =============================================================================
void SendCCDCommand(int index, AnsiString Msg, AnsiString Msg2)
{
    // AI(W5-Final-BarCode8CCDGlue) 20260711: golden's FIRST guard is
    // `if(memoCCDCommLog==NULL || InitialOK==false) return;` -- memoCCDCommLog
    // is a VCL TMemo* (out of scope, no shim built).  Dropped the NULL-guard
    // half (matching the exact precedent BarCode_Bottom2DID.cpp's own
    // SendCCDCommand stand-in already established for this identical golden
    // function), keeping the InitialOK half -- InitialOK==false covers the
    // same "form/system not ready yet" window memoCCDCommLog's NULL-ness
    // would have covered, so no additional gating is needed.
    if (InitialOK == false)
        return;

    AnsiString Log;
    GetTimeInfo();

    if (Msg2.Pos("LF") != 0)
    {
        OCRChangeFileTimeOut.SetSecAndOn(5);
        bChangeSetupFile = true;
    }

    if (Msg2 == "")
        Log.sprintf("Action, %s", Msg);
    else
        Log.sprintf("Send, %s, %s", Msg, Msg2);

    Glue8CCD_AddCCDCommunicationLog(index, Log);

    // AI(W5-Final-BarCode8CCDGlue) 20260711: golden spells each of the 12
    // per-index dispatch arms as `if(Msg2=="") {} else { if(...->Active)
    // ...SendText(...); }` (an empty then-branch).  Collapsed here to
    // `if(Msg2!="" && ...->Active) ...SendText(...);` -- a pure structural
    // simplification with an IDENTICAL truth table/side-effect for every
    // input (not a behavioural "fix"); the golden empty-branch shape is
    // preserved for reference in the still-gated BarCode_Bottom2DID8CCD.cpp
    // stand-in's `#if 0` block.
    if (BOTTOM_2DID_CCD == 1 && BOTTOM_2DID == 1 &&
        TestIF_File.bEnableBottom2D)                                            //KaiChen 20200513 : Bottom 2D 8CCD
    {
        if (index == iBottom_1)
        {
            if (Msg2 != "" && ClientSocket_Bottom_1->Active)
                ClientSocket_Bottom_1->Socket->SendText(Msg2 + "\r\n");
        }

        if (index == iBottom_2)
        {
            if (Msg2 != "" && ClientSocket_Bottom_2->Active)
                ClientSocket_Bottom_2->Socket->SendText(Msg2 + "\r\n");
        }

        if (index == iBottom_3)
        {
            if (Msg2 != "" && ClientSocket_Bottom_3->Active)
                ClientSocket_Bottom_3->Socket->SendText(Msg2 + "\r\n");
        }

        if (index == iBottom_4)
        {
            if (Msg2 != "" && ClientSocket_Bottom_4->Active)
                ClientSocket_Bottom_4->Socket->SendText(Msg2 + "\r\n");
        }

        if (index == iBottom_5)
        {
            if (Msg2 != "" && ClientSocket_Bottom_5->Active)
                ClientSocket_Bottom_5->Socket->SendText(Msg2 + "\r\n");
        }

        if (index == iBottom_6)
        {
            if (Msg2 != "" && ClientSocket_Bottom_6->Active)
                ClientSocket_Bottom_6->Socket->SendText(Msg2 + "\r\n");
        }

        if (index == iBottom_7)
        {
            if (Msg2 != "" && ClientSocket_Bottom_7->Active)
                ClientSocket_Bottom_7->Socket->SendText(Msg2 + "\r\n");
        }

        if (index == iBottom_8)
        {
            if (Msg2 != "" && ClientSocket_Bottom_8->Active)
                ClientSocket_Bottom_8->Socket->SendText(Msg2 + "\r\n");
        }
    }
    else
    {
        if (index == iBarCode1_1)
        {
            if (Msg2 != "" && ClientSocket_Shuttle1_A->Active)
            {
                if (BAR_CODE_INSTALL == ebcUseOCR)
                    ClientSocket_Shuttle1_A->Socket->SendText(Msg2);
                else
                    ClientSocket_Shuttle1_A->Socket->SendText(Msg2 + "\r\n");
            }
        }
        else if (index == iBarCode1_2)
        {
            if (Msg2 != "" && ClientSocket_Shuttle1_B->Active)
            {
                if (BAR_CODE_INSTALL == ebcUseOCR)
                    ClientSocket_Shuttle1_B->Socket->SendText(Msg2);
                else
                    ClientSocket_Shuttle1_B->Socket->SendText(Msg2 + "\r\n");
            }
        }
        else if (index == iBarCode2_1)
        {
            if (Msg2 != "" && ClientSocket_Shuttle2_A->Active)
            {
                if (BAR_CODE_INSTALL == ebcUseOCR)
                    ClientSocket_Shuttle2_A->Socket->SendText(Msg2);
                else
                    ClientSocket_Shuttle2_A->Socket->SendText(Msg2 + "\r\n");
            }
        }
        else if (index == iBarCode2_2)
        {
            if (Msg2 != "" && ClientSocket_Shuttle2_B->Active)
            {
                if (BAR_CODE_INSTALL == ebcUseOCR)
                    ClientSocket_Shuttle2_B->Socket->SendText(Msg2);
                else
                    ClientSocket_Shuttle2_B->Socket->SendText(Msg2 + "\r\n");
            }
        }
    }
}

// =============================================================================
//  TimerBotton8CCDConnectTimer -- golden BarCode.cpp:9192-9536.
//  8 near-identical unrolled blocks (Bottom_1..Bottom_8), faithfully
//  copy-pasted from golden -- including ONE genuine golden copy-paste bug
//  (see the iBottom_2 block below) preserved verbatim, NOT fixed.
// =============================================================================
void TimerBotton8CCDConnectTimer()
{
    static bool bTimerRun = false;
    if (InitialOK == false || bTimerRun == true)                                //Steven 20160512 : 2D Timer加上保護
    {
        return;
    }
    bTimerRun = true;

    if (TestIF_File.bEnableBarCode == false &&
        TestIF_File.bEnableShtFloatChk == false)
    {
        bTimerRun = false;
        return;
    }

    static int Count1 = 90;
    static int Count2 = 90;
    static int Count3 = 90;
    static int Count4 = 90;
    static int Count5 = 90;
    static int Count6 = 90;
    static int Count7 = 90;
    static int Count8 = 90;

    //==========================================================================
    // iBottom_1
    //==========================================================================
    if (ClientSocket_Bottom_1->Active                  &&                       //相同的IP跟Port就不要重新連線
        ClientSocket_Bottom_1->Address == "172.16.8.200" &&
        ClientSocket_Bottom_1->Port == 5011)
    {
    }
    else
    {
        if (ClientSocket_Bottom_1->Active)
        {
            ClientSocket_Bottom_1->Close();
            SendCCDCommand(iBottom_1, "ClientSocket Close", "");
            Count1 = 0;
        }
    }

    if (ClientSocket_Bottom_1->Active == false &&
        bEnableCCDChannelConnect[iBottom_1] == true)
    {
        Count1++;
        if (Count1 > 20)                                                        //kevin 20191004 change
        {
            ClientSocket_Bottom_1->Address = "172.16.8.200";
            ClientSocket_Bottom_1->Port = 5011;
            SendCCDCommand(iBottom_1, "ClientSocket connecting..", "");
            try
            {
                ClientSocket_Bottom_1->Open();
                SendCCDCommand(iBottom_1, "ClientSocket Open Success", "");     //KaiHuang 20200910 Add
            }
            catch (...)
            {
                SendCCDCommand(iBottom_1, "ClientSocket Open Fail", "");        //KaiHuang 20200910 Add
                Glue8CCD_LogClientSocketExceptionError(ClientSocket_Bottom_1, "TfBarCode::TimerDownCCDConnect"); //Steven 20231113 : 紀錄網路連線例外
            }
            Count1 = 0;
        }
    }
    //==========================================================================
    // iBottom_2
    //==========================================================================
    if (ClientSocket_Bottom_2->Active                  &&                       //相同的IP跟Port就不要重新連線
        ClientSocket_Bottom_2->Address == "172.16.8.200" &&
        ClientSocket_Bottom_2->Port == 5012)
    {
    }
    else
    {
        if (ClientSocket_Bottom_2->Active)
        {
            ClientSocket_Bottom_2->Close();
            SendCCDCommand(iBottom_2, "ClientSocket Close", "");
            Count2 = 0;
        }
    }

    if (ClientSocket_Bottom_2->Active == false &&
        bEnableCCDChannelConnect[iBottom_2] == true)
    {
        Count2++;
        if (Count2 > 30)                                                        //kevin 20191004 change
        {
            ClientSocket_Bottom_2->Address = "172.16.8.200";
            ClientSocket_Bottom_2->Port = 5012;
            SendCCDCommand(iBottom_2, "ClientSocket connecting..", "");
            try
            {
                ClientSocket_Bottom_2->Open();
                SendCCDCommand(iBottom_2, "ClientSocket Open Success", "");     //KaiHuang 20200910 Add
            }
            catch (...)
            {
                SendCCDCommand(iBottom_2, "ClientSocket Open Fail", "");        //KaiHuang 20200910 Add
                Glue8CCD_LogClientSocketExceptionError(ClientSocket_Bottom_2, "TfBarCode::TimerDownCCDConnect"); //Steven 20231113 : 紀錄網路連線例外
            }
            // AI(W5-Final-BarCode8CCDGlue) 20260711: golden BarCode.cpp:9264
            // literally resets `Count1=0;` here (NOT `Count2=0;`) -- a
            // genuine copy-paste bug (every other one of the 8 blocks resets
            // its OWN counter).  Preserved VERBATIM, not fixed: this means
            // Count2 keeps incrementing past 30 every subsequent tick once
            // this branch is first taken, so a real reconnect attempt fires
            // on EVERY tick thereafter (not throttled back to a fresh 30-tick
            // wait) until Address/Port next mismatch resets it via the
            // disconnect branch above.  Flagged in translate report item 6.
            Count1 = 0;
        }
    }
    //==========================================================================
    // iBottom_3
    //==========================================================================
    if (ClientSocket_Bottom_3->Active                  &&                       //相同的IP跟Port就不要重新連線
        ClientSocket_Bottom_3->Address == "172.16.8.200" &&
        ClientSocket_Bottom_3->Port == 5013)
    {
    }
    else
    {
        if (ClientSocket_Bottom_3->Active)
        {
            ClientSocket_Bottom_3->Close();
            SendCCDCommand(iBottom_3, "ClientSocket Close", "");
            Count3 = 0;
        }
    }

    if (ClientSocket_Bottom_3->Active == false &&
        bEnableCCDChannelConnect[iBottom_3] == true)
    {
        Count3++;
        if (Count3 > 30)                                                        //kevin 20191004 change
        {
            ClientSocket_Bottom_3->Address = "172.16.8.200";
            ClientSocket_Bottom_3->Port = 5013;
            SendCCDCommand(iBottom_3, "ClientSocket connecting..", "");
            try
            {
                ClientSocket_Bottom_3->Open();
                SendCCDCommand(iBottom_3, "ClientSocket Open Success", "");     //KaiHuang 20200910 Add
            }
            catch (...)
            {
                SendCCDCommand(iBottom_3, "ClientSocket Open Fail", "");        //KaiHuang 20200910 Add
                Glue8CCD_LogClientSocketExceptionError(ClientSocket_Bottom_3, "TfBarCode::TimerDownCCDConnect"); //Steven 20231113 : 紀錄網路連線例外
            }
            Count3 = 0;
        }
    }
    //==========================================================================
    // iBottom_4
    //==========================================================================
    if (ClientSocket_Bottom_4->Active                  &&                       //相同的IP跟Port就不要重新連線
        ClientSocket_Bottom_4->Address == "172.16.8.200" &&
        ClientSocket_Bottom_4->Port == 5014)
    {
    }
    else
    {
        if (ClientSocket_Bottom_4->Active)
        {
            ClientSocket_Bottom_4->Close();
            SendCCDCommand(iBottom_4, "ClientSocket Close", "");
            Count4 = 0;
        }
    }

    if (ClientSocket_Bottom_4->Active == false &&
        bEnableCCDChannelConnect[iBottom_4] == true)
    {
        Count4++;
        if (Count4 > 30)                                                        //kevin 20191004 change
        {
            ClientSocket_Bottom_4->Address = "172.16.8.200";
            ClientSocket_Bottom_4->Port = 5014;
            SendCCDCommand(iBottom_4, "ClientSocket connecting..", "");
            try
            {
                ClientSocket_Bottom_4->Open();
                SendCCDCommand(iBottom_4, "ClientSocket Open Success", "");     //KaiHuang 20200910 Add
            }
            catch (...)
            {
                SendCCDCommand(iBottom_4, "ClientSocket Open Fail", "");        //KaiHuang 20200910 Add
                Glue8CCD_LogClientSocketExceptionError(ClientSocket_Bottom_4, "TfBarCode::TimerDownCCDConnect"); //Steven 20231113 : 紀錄網路連線例外
            }
            Count4 = 0;
        }
    }
    //==========================================================================
    // iBottom_5
    //==========================================================================
    if (ClientSocket_Bottom_5->Active                  &&                       //相同的IP跟Port就不要重新連線
        ClientSocket_Bottom_5->Address == "172.16.8.200" &&
        ClientSocket_Bottom_5->Port == 5015)
    {
    }
    else
    {
        if (ClientSocket_Bottom_5->Active)
        {
            ClientSocket_Bottom_5->Close();
            SendCCDCommand(iBottom_5, "ClientSocket Close", "");
            Count5 = 0;
        }
    }

    if (ClientSocket_Bottom_5->Active == false &&
        bEnableCCDChannelConnect[iBottom_5] == true)
    {
        Count5++;
        if (Count5 > 30)                                                        //kevin 20191004 change
        {
            ClientSocket_Bottom_5->Address = "172.16.8.200";
            ClientSocket_Bottom_5->Port = 5015;
            SendCCDCommand(iBottom_5, "ClientSocket connecting..", "");
            try
            {
                ClientSocket_Bottom_5->Open();
                SendCCDCommand(iBottom_5, "ClientSocket Open Success", "");     //KaiHuang 20200910 Add
            }
            catch (...)
            {
                SendCCDCommand(iBottom_5, "ClientSocket Open Fail", "");        //KaiHuang 20200910 Add
                Glue8CCD_LogClientSocketExceptionError(ClientSocket_Bottom_5, "TfBarCode::TimerDownCCDConnect"); //Steven 20231113 : 紀錄網路連線例外
            }
            Count5 = 0;
        }
    }
    //==========================================================================
    // iBottom_6
    //==========================================================================
    if (ClientSocket_Bottom_6->Active                  &&                       //相同的IP跟Port就不要重新連線
        ClientSocket_Bottom_6->Address == "172.16.8.200" &&
        ClientSocket_Bottom_6->Port == 5016)
    {
    }
    else
    {
        if (ClientSocket_Bottom_6->Active)
        {
            ClientSocket_Bottom_6->Close();
            SendCCDCommand(iBottom_6, "ClientSocket Close", "");
            Count6 = 0;
        }
    }

    if (ClientSocket_Bottom_6->Active == false && bEnableCCDChannelConnect[iBottom_6] == true)
    {
        Count6++;
        if (Count6 > 30)                                                        //kevin 20191004 change
        {
            ClientSocket_Bottom_6->Address = "172.16.8.200";
            ClientSocket_Bottom_6->Port = 5016;
            SendCCDCommand(iBottom_6, "ClientSocket connecting..", "");
            try
            {
                ClientSocket_Bottom_6->Open();
                SendCCDCommand(iBottom_6, "ClientSocket Open Success", "");     //KaiHuang 20200910 Add
            }
            catch (...)
            {
                SendCCDCommand(iBottom_6, "ClientSocket Open Fail", "");        //KaiHuang 20200910 Add
                Glue8CCD_LogClientSocketExceptionError(ClientSocket_Bottom_6, "TfBarCode::TimerDownCCDConnect"); //Steven 20231113 : 紀錄網路連線例外
            }
            Count6 = 0;
        }
    }
    //==========================================================================
    // iBottom_7
    //==========================================================================
    if (ClientSocket_Bottom_7->Active                  &&                       //相同的IP跟Port就不要重新連線
        ClientSocket_Bottom_7->Address == "172.16.8.200" &&
        ClientSocket_Bottom_7->Port == 5017)
    {
    }
    else
    {
        if (ClientSocket_Bottom_7->Active)
        {
            ClientSocket_Bottom_7->Close();
            SendCCDCommand(iBottom_7, "ClientSocket Close", "");
            Count7 = 0;
        }
    }

    if (ClientSocket_Bottom_7->Active == false && bEnableCCDChannelConnect[iBottom_7] == true)
    {
        Count7++;
        if (Count7 > 30)                                                        //kevin 20191004 change
        {
            ClientSocket_Bottom_7->Address = "172.16.8.200";
            ClientSocket_Bottom_7->Port = 5017;
            SendCCDCommand(iBottom_7, "ClientSocket connecting..", "");
            try
            {
                ClientSocket_Bottom_7->Open();
                SendCCDCommand(iBottom_7, "ClientSocket Open Success", "");     //KaiHuang 20200910 Add
            }
            catch (...)
            {
                SendCCDCommand(iBottom_7, "ClientSocket Open Fail", "");        //KaiHuang 20200910 Add
                Glue8CCD_LogClientSocketExceptionError(ClientSocket_Bottom_7, "TfBarCode::TimerDownCCDConnect"); //Steven 20231113 : 紀錄網路連線例外
            }
            Count7 = 0;
        }
    }
    //==========================================================================
    // iBottom_8
    //==========================================================================
    if (ClientSocket_Bottom_8->Active                  &&                       //相同的IP跟Port就不要重新連線
        ClientSocket_Bottom_8->Address == "172.16.8.200" &&
        ClientSocket_Bottom_8->Port == 5018)
    {
    }
    else
    {
        if (ClientSocket_Bottom_8->Active)
        {
            ClientSocket_Bottom_8->Close();
            SendCCDCommand(iBottom_8, "ClientSocket Close", "");
            Count8 = 0;
        }
    }

    if (ClientSocket_Bottom_8->Active == false && bEnableCCDChannelConnect[iBottom_8] == true)
    {
        Count8++;
        if (Count8 > 30)                                                        //kevin 20191004 change
        {
            ClientSocket_Bottom_8->Address = "172.16.8.200";
            ClientSocket_Bottom_8->Port = 5018;
            SendCCDCommand(iBottom_8, "ClientSocket connecting..", "");
            try
            {
                ClientSocket_Bottom_8->Open();
                SendCCDCommand(iBottom_8, "ClientSocket Open Success", "");     //KaiHuang 20200910 Add
            }
            catch (...)
            {
                SendCCDCommand(iBottom_8, "ClientSocket Open Fail", "");        //KaiHuang 20200910 Add
                Glue8CCD_LogClientSocketExceptionError(ClientSocket_Bottom_8, "TfBarCode::TimerDownCCDConnect"); //Steven 20231113 : 紀錄網路連線例外
            }
            Count8 = 0;
        }
    }
    //==========================================================================

    bTimerRun = false;                                                          //最下面
}

// =============================================================================
//  TimerBottom8CCDInitialTimer -- golden BarCode.cpp:9539-9937.               //KaiChen 20200513 : Bottom 2D 8CCD
//  Two near-identical cursor state machines (Task/Bottom_1..4, Task2/
//  Bottom_5..8).  Every intentional golden fall-through (cases with NO
//  `break;`, whether or not golden left a `//break;` comment) is preserved
//  verbatim and flagged inline.
// =============================================================================
void TimerBottom8CCDInitialTimer()
{
    static bool bTimerRun = false;
    if (InitialOK == false || bTimerRun == true)                                //Steven 20160512 : 2D Timer加上保護
    {
        return;
    }
    bTimerRun = true;

    int &Task  = iBottomConntectionOkTask1;
    int &Task2 = iBottomConntectionOkTask2;
    AnsiString Str;
    AnsiString strBarcodeSetupFile = "";
    if (TestIF_File.bEnableBarCode == true &&
        BAR_CODE_INSTALL == ebctUseCCDMode &&
        TestIF_File.bBarCodeMultiRecipe == true)
    {
        strBarcodeSetupFile = Glue8CCD_GetSetupFileNameText() + Glue8CCD_GetBarcodeRecipeText();
    }
    else
    {
        strBarcodeSetupFile = Glue8CCD_GetSetupFileNameText();
    }

    if (ClientSocket_Bottom_1->Active &&                                        //相同的IP跟Port就不要重新連線
        ClientSocket_Bottom_1->Address == "172.16.8.200" &&
        ClientSocket_Bottom_1->Port == 5011 &&
        ClientSocket_Bottom_2->Active &&                                        //KaiHuang 20200910 : Add 2~3
        ClientSocket_Bottom_2->Address == "172.16.8.200" &&
        ClientSocket_Bottom_2->Port == 5012 &&
        ClientSocket_Bottom_3->Active &&
        ClientSocket_Bottom_3->Address == "172.16.8.200" &&
        ClientSocket_Bottom_3->Port == 5013 &&
        ClientSocket_Bottom_4->Active &&
        ClientSocket_Bottom_4->Address == "172.16.8.200" &&
        ClientSocket_Bottom_4->Port == 5014)
    {
        switch (Task)
        {
            case 1:
                Glue8CCD_SetSFCCheckStepCount();
                dVisionVer = 1;
                Str.sprintf("VR");
                SendCCDCommand(iBottom_1, "Get Version", Str);                  //Reset CCD Shuttle暫存器
                bSFCGetVR = false;
                tChangeSetupFile.SetSecAndOn(30);
                Task = 100;
                // FALLTHROUGH intentional (golden has no break here)

            case 100:
                if (bSFCGetVR == true)
                {
                    if (TestIF_File.bEnableBarCode)
                        Task = 1000;
                    else if (TestIF_File.bEnableShtFloatChk)
                        Task = 2000;
                    else
                        Task = 3000;
                }
                else if (tChangeSetupFile.Off())
                {
                    Task = 1;
                }
                break;
            case 1000:                                                          //Reset CCD Shuttle暫存器
                bGetSE9[iBottom_1] = false;
                bGetSE9[iBottom_2] = false;                                     //KaiHuang 20200910 : Add
                bGetSE9[iBottom_3] = false;                                     //KaiHuang 20200910 : Add
                bGetSE9[iBottom_4] = false;                                     //KaiHuang 20200910 : Add

                if (dVisionVer >= 2)                                            //wei 20170119 (Steven) 版本號大於2
                    Str.sprintf("E9,0,%d", InArmSuck.iShtCol);
                else
                    Str.sprintf("SE0,9,%d", InArmSuck.iShtCol);

                SendCCDCommand(iBottom_1, "Clear buffer", Str);
                SendCCDCommand(iBottom_2, "Clear buffer", Str);
                SendCCDCommand(iBottom_3, "Clear buffer", Str);
                SendCCDCommand(iBottom_4, "Clear buffer", Str);
                tChangeSetupFile.SetSecAndOn(1000);
                Task = 1050;
                // FALLTHROUGH intentional (golden has no break here)

            case 1050:
                if (bGetSE9[iBottom_1] && bGetSE9[iBottom_2] && bGetSE9[iBottom_3] && bGetSE9[iBottom_4])   //KaiHuang 20200910 : Add iBottom_2~4
                    Task = 1100;
                else if (tChangeSetupFile.Off())
                    Task = 1000;
                break;
            case 1100:                                                          //取得工作檔名稱
                bGetGF = false;
                if (dVisionVer >= 2)                                            //wei 20170119 (Steven) 版本號大於2
                    SendCCDCommand(iBottom_1, "Get File Name", "GF,0");
                else
                    SendCCDCommand(iBottom_1, "Get File Name", "GF");
                tChangeSetupFile.SetSecAndOn(10);
                Task = 1150;
                // FALLTHROUGH intentional (golden has no break here)

            case 1150:
                if (bGetGF)
                {
                    Task = 1200;
                }
                else if (tChangeSetupFile.Off())
                {
                    Task = 1100;
                }
                break;
            case 1200:
                bGetLF = false;
                if (BOTTOM_2DID && TestIF_File.bEnableBottom2D)                  //Steven 20190524 : Add bottom 2DID
                {
                    if (dVisionVer >= 2)                                        //wei 20170119 (Steven) 版本號大於2
                        Str.sprintf("LF,0,%s,1", strBarcodeSetupFile);
                    else
                        Str.sprintf("LF%s,1", strBarcodeSetupFile);
                }
                else
                {
                    if (dVisionVer >= 2)                                        //wei 20170119 (Steven) 版本號大於2
                        Str.sprintf("LF,0,%s,0", strBarcodeSetupFile);
                    else
                        Str.sprintf("LF%s,0", strBarcodeSetupFile);
                }
                SendCCDCommand(iBottom_1, "Change File", Str);
                tChangeSetupFile.SetSecAndOn(10);
                Task = 1250;
                // FALLTHROUGH intentional (golden has no break here)
            case 1250:
                if (bGetLF)
                {
                    Task = 2000;
                }
                else if (tChangeSetupFile.Off())
                {
                    Task = 1200;
                }
                break;
            case 2000:
                if (TestIF_File.bEnableShtFloatChk)
                    Task = 2100;
                else
                    Task = 3000;
                break;
            case 2100:
                bSFCGetSE9[iBottom_1] = false;
                bSFCGetSE9[iBottom_2] = false;                                  //KaiHuang 20200910 : Add
                bSFCGetSE9[iBottom_3] = false;                                  //KaiHuang 20200910 : Add
                bSFCGetSE9[iBottom_4] = false;                                  //KaiHuang 20200910 : Add
                Str.sprintf("E9,1,%d", iSFCTotalMoveStep);

                SendCCDCommand(iBottom_1, "Clear buffer", Str);
                SendCCDCommand(iBottom_2, "Clear buffer", Str);
                SendCCDCommand(iBottom_3, "Clear buffer", Str);
                SendCCDCommand(iBottom_4, "Clear buffer", Str);
                tChangeSetupFile.SetSecAndOn(1000);
                Task = 2150;
                // FALLTHROUGH intentional (golden left an explicit "//break;" comment here)
            case 2150:
                if (bSFCGetSE9[iBottom_1] && bSFCGetSE9[iBottom_2] && bSFCGetSE9[iBottom_3] && bSFCGetSE9[iBottom_4])
                    Task = 2200;
                else if (tChangeSetupFile.Off())
                    Task = 2100;
                break;
            case 2200:                                                         //取得工作檔名稱
                bSFCGetGF = false;
                SendCCDCommand(iBottom_1, "Get File Name", "GF,1");
                tChangeSetupFile.SetSecAndOn(10);
                Task = 2250;
                // FALLTHROUGH intentional (golden left an explicit "//break;" comment here)
            case 2250:
                if (bSFCGetGF)
                {
                    // AI(W5-Final-BarCode8CCDGlue) 20260711: golden
                    // `strBarcodeSetupFile.UpperCase().AnsiCompare(sSFCCurrentGF)==0`
                    // -- vclcompat's AnsiString has no AnsiCompare() method
                    // (grep-verified tree-wide). Golden only ever tests the
                    // ==0 (equal) case, never inspects sign/ordering, so this
                    // is a pure ordinal-equality check -- substituted with
                    // the already-available `==` free-function operator
                    // (byte-exact compare, identical outcome for every input).
                    if (strBarcodeSetupFile.UpperCase() == sSFCCurrentGF)
                    {
                        Task = 3000;
                        bSFCGetLF = true;
                    }
                    else
                    {
                        Task = 2300;
                    }
                }
                else if (tChangeSetupFile.Off())
                {
                    Task = 2200;
                }
                break;
            case 2300:
                bSFCGetLF = false;
                SendCCDCommand(iBottom_1, "SFC Change File", "LF,1," + strBarcodeSetupFile);
                tChangeSetupFile.SetSecAndOn(10);
                Task = 2350;
                // FALLTHROUGH intentional (golden has no break here)
            case 2350:
                if (bSFCGetLF)
                {
                    Task = 3000;
                }
                else if (tChangeSetupFile.Off())
                {
                    Task = 2300;
                }
                break;
            case 3000:
                break;
        }
    }

    if (Task != 3000)
    {
        bTimerRun = false;
        return;
    }

    if (ClientSocket_Bottom_5->Active &&                                        //相同的IP跟Port就不要重新連線
        ClientSocket_Bottom_5->Address == "172.16.8.200" &&
        ClientSocket_Bottom_5->Port == 5015 &&
        ClientSocket_Bottom_6->Active &&
        ClientSocket_Bottom_6->Address == "172.16.8.200" &&                     //KaiHuang 20200910 : Add 6~8
        ClientSocket_Bottom_6->Port == 5016 &&
        ClientSocket_Bottom_7->Active &&
        ClientSocket_Bottom_7->Address == "172.16.8.200" &&
        ClientSocket_Bottom_7->Port == 5017 &&
        ClientSocket_Bottom_8->Active &&
        ClientSocket_Bottom_8->Address == "172.16.8.200" &&
        ClientSocket_Bottom_8->Port == 5018)
    {
        switch (Task2)
        {
            case 1:
                Glue8CCD_SetSFCCheckStepCount();
                // AI(W5-Final-BarCode8CCDGlue) 20260711: golden asymmetry
                // preserved verbatim -- the FIRST half's case 1 (above) sets
                // `dVisionVer=1;` unconditionally with NO #ifdef at all, while
                // THIS half wraps the identical assignment in
                // `#ifdef SOFT_SIMULTE / #else / #endif`.  SOFT_SIMULTE is
                // not defined in this build (project-wide convention), so
                // both halves evaluate to the same numeric result (1) either
                // way -- NOT "fixed" to match the other half's plain form.
                #ifdef SOFT_SIMULTE
                    dVisionVer = 2;
                #else
                    dVisionVer = 1;
                #endif
                Str.sprintf("VR");
                SendCCDCommand(iBottom_5, "Get Version", Str);                  //Reset CCD Shuttle暫存器
                bSFCGetVR = false;
                tChangeSetupFile.SetSecAndOn(30);
                Task2 = 100;
                // FALLTHROUGH intentional (golden has no break here)

            case 100:
                if (bSFCGetVR == true)
                {
                    if (TestIF_File.bEnableBarCode)
                        Task2 = 1000;
                    else if (TestIF_File.bEnableShtFloatChk)
                        Task2 = 2000;
                    else
                        Task2 = 3000;
                }
                else if (tChangeSetupFile.Off())
                {
                    Task2 = 1;
                }
                break;
            case 1000:                                                         //Reset CCD Shuttle暫存器
                bGetSE9[iBottom_5] = false;
                bGetSE9[iBottom_6] = false;                                    //KaiHuang 20200910 : Add
                bGetSE9[iBottom_7] = false;                                    //KaiHuang 20200910 : Add
                bGetSE9[iBottom_8] = false;                                    //KaiHuang 20200910 : Add

                if (dVisionVer >= 2)                                           //wei 20170119 (Steven) 版本號大於2
                    Str.sprintf("E9,0,%d", InArmSuck.iShtCol);
                else
                    Str.sprintf("SE0,9,%d", InArmSuck.iShtCol);

                SendCCDCommand(iBottom_5, "Clear buffer", Str);
                SendCCDCommand(iBottom_6, "Clear buffer", Str);
                SendCCDCommand(iBottom_7, "Clear buffer", Str);
                SendCCDCommand(iBottom_8, "Clear buffer", Str);
                tChangeSetupFile.SetSecAndOn(1000);
                Task2 = 1050;
                // FALLTHROUGH intentional (golden has no break here)

            case 1050:
                if (bGetSE9[iBottom_5] && bGetSE9[iBottom_6] && bGetSE9[iBottom_7] && bGetSE9[iBottom_8])   //KaiHuang 20200910 : Add 6~8
                    Task2 = 1100;
                else if (tChangeSetupFile.Off())
                    Task2 = 1000;
                break;
            case 1100:                                                         //取得工作檔名稱
                bGetGF = false;
                if (dVisionVer >= 2)                                           //wei 20170119 (Steven) 版本號大於2
                    SendCCDCommand(iBottom_5, "Get File Name", "GF,0");
                else
                    SendCCDCommand(iBottom_5, "Get File Name", "GF");
                tChangeSetupFile.SetSecAndOn(10);
                Task2 = 1150;
                // FALLTHROUGH intentional (golden left an explicit "//break;" comment here)
            case 1150:
                if (bGetGF)
                {
                    Task2 = 1200;
                }
                else if (tChangeSetupFile.Off())
                {
                    Task2 = 1100;
                }
                break;
            case 1200:
                bGetLF = false;
                if (BOTTOM_2DID && TestIF_File.bEnableBottom2D)                 //Steven 20190524 : Add bottom 2DID
                {
                    if (dVisionVer >= 2)                                       //wei 20170119 (Steven) 版本號大於2
                        Str.sprintf("LF,0,%s,1", strBarcodeSetupFile);
                    else
                        Str.sprintf("LF%s,1", strBarcodeSetupFile);
                }
                else
                {
                    if (dVisionVer >= 2)                                       //wei 20170119 (Steven) 版本號大於2
                        Str.sprintf("LF,0,%s,0", strBarcodeSetupFile);
                    else
                        Str.sprintf("LF%s,0", strBarcodeSetupFile);
                }
                SendCCDCommand(iBottom_5, "Change File", Str);
                tChangeSetupFile.SetSecAndOn(10);
                Task2 = 1250;
                // FALLTHROUGH intentional (golden has no break here)
            case 1250:
                if (bGetLF)
                {
                    Task2 = 2000;
                }
                else if (tChangeSetupFile.Off())
                {
                    Task2 = 1200;
                }
                break;
            case 2000:
                if (TestIF_File.bEnableShtFloatChk)
                    Task2 = 2100;
                else
                    Task2 = 3000;
                break;
            case 2100:
                bSFCGetSE9[iBottom_5] = false;
                bSFCGetSE9[iBottom_6] = false;                                 //KaiHuang 20200910 : Add
                bSFCGetSE9[iBottom_7] = false;                                 //KaiHuang 20200910 : Add
                bSFCGetSE9[iBottom_8] = false;                                 //KaiHuang 20200910 : Add
                Str.sprintf("E9,1,%d", iSFCTotalMoveStep);

                SendCCDCommand(iBottom_5, "Clear buffer", Str);
                SendCCDCommand(iBottom_6, "Clear buffer", Str);
                SendCCDCommand(iBottom_7, "Clear buffer", Str);
                SendCCDCommand(iBottom_8, "Clear buffer", Str);
                tChangeSetupFile.SetSecAndOn(10);
                Task2 = 2150;
                // FALLTHROUGH intentional (golden has no break here)

            case 2150:
                if (bSFCGetSE9[iBottom_5] && bSFCGetSE9[iBottom_6] && bSFCGetSE9[iBottom_7] && bSFCGetSE9[iBottom_8])   //KaiHuang 20200910 : Add 6~8
                    Task2 = 2200;
                else if (tChangeSetupFile.Off())
                    Task2 = 2100;
                break;
            case 2200:                                                        //取得工作檔名稱
                bSFCGetGF = false;
                SendCCDCommand(iBottom_5, "Get File Name", "GF,1");
                tChangeSetupFile.SetSecAndOn(10);
                Task2 = 2250;
                // FALLTHROUGH intentional (golden has no break here)

            case 2250:
                if (bSFCGetGF)
                {
                    // See the Task (Bottom_1..4) half's case 2250 above for
                    // why `.AnsiCompare(x)==0` was substituted with `==x`.
                    if (strBarcodeSetupFile.UpperCase() == sSFCCurrentGF)
                    {
                        Task2 = 3000;
                        bSFCGetLF = true;
                    }
                    else
                    {
                        Task2 = 2300;
                    }
                }
                else if (tChangeSetupFile.Off())
                {
                    Task2 = 2200;
                }
                break;
            case 2300:
                bSFCGetLF = false;
                SendCCDCommand(iBottom_5, "SFC Change File", "LF,1," + strBarcodeSetupFile);
                tChangeSetupFile.SetSecAndOn(10);
                Task2 = 2350;
                // FALLTHROUGH intentional (golden has no break here)
            case 2350:
                if (bSFCGetLF)
                {
                    Task2 = 3000;
                }
                else if (tChangeSetupFile.Off())
                {
                    Task2 = 2300;
                }
                break;
            case 3000:
                break;
        }
    }

    bTimerRun = false;                                                          //保持在最下面
}
