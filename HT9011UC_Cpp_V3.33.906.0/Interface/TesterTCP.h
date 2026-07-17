//---------------------------------------------------------------------------
//  Interface/TesterTCP.h -- bounded subset #2 of the golden TfTesterTCP form
//    (Interface/TesterTCP.cpp/.h, golden 1117 lines), a Tester "OS/Open-Short"
//    channel used only by CUSTOMER_CODE==CC_JSCC_OS.
//
//  AI(W906-D-Interface-TesterTCP) 20260716: new file. This is a DIFFERENT,
//  SIBLING file from the already-translated Interface/TesterTCP_Socket.h/.cpp
//  (that unit covers ClientSocket_TCPIP connect/disconnect/error/read +
//  TimerTCPIPConnectTimer + SendTCPIPCommand/AddTCPIPCommunicationLog +
//  btTCPIP_Trigger/Connect/DisconnectClick -- golden TesterTCP.cpp:166-345 +
//  615-633). This file does NOT touch that socket-management subset at all;
//  it covers a completely separate, zero-widget-coupling family: the OS
//  recipe zip-transfer-to/from-Tester pair, plus the per-machine
//  arm-speed-save click handler. Both files together still cover only a
//  fraction of the golden 1117-line TfTesterTCP form -- see the NOT
//  ATTEMPTED list below (this file) and TesterTCP_Socket.h's own list (that
//  file) for the remainder.
//
//  SCOPE (this wave, recon-selected -- 3 functions, all confirmed on a full
//  read to have NO VCL widget reads once the 4 identified inputs are
//  promoted to parameters):
//    CopyRecipeToTester    golden TesterTCP.cpp:1058-1077 (Steven 20250612 "for OS Tester.")
//    CopyRecipeFromTester  golden TesterTCP.cpp:1079-1097 (Steven 20250612 "for OS Tester.")
//    btnSaveClick          golden TesterTCP.cpp:1099-1115 (RogerYang 20260210)
//
//  NOT attempted this wave (per this front's task brief, confirmed by recon's
//  dependency checks, not assumption):
//    - ctor / FormDestroy / FormShow / rgUnloaderClick (~140 widget-member
//      construction cluster, no FormsFacade home, near-zero unique logic).
//    - CopyOSTestResult / PlaceOSTestResultToTray (blocked on a real
//      cross-file gap in Public/MyProductionRecord.cpp's GetSiteNo/
//      GetOrderOfContact storage layer -- a different module's translation
//      debt, not a small addition).
//    - SimulateBin / ProcessOSPrint / ProcessOSTrayData (heavy
//      TRichEdit/TMemo + two-form widget coupling, OS-report generation,
//      not a live protocol path, low urgency).
//    - TimerProcessTCPDataTimer (the file's true protocol-decode core, but
//      genuinely mixed: needs 5 new TfMain FormsFacade members plus an
//      entire untranslated WritePERSITETemperature sibling function --
//      deserves its own dedicated future wave, see TesterTCP_Socket.h's
//      identical assessment).
//    - ClientSocket_TCPIP* / TimerTCPIPConnectTimer / SendTCPIPCommand /
//      AddTCPIPCommunicationLog / btTCPIP_*Click (already covered by the
//      sibling Interface/TesterTCP_Socket.h/.cpp -- do not duplicate here).
//
//  extract-calc-core widget promotions (this wave):
//    btnSaveClick's golden body reads 3 TCheckBox->Checked (chkAuto1_OS/
//    chkAuto2_OS/chkAuto3_OS) and the form member LastFileName -- none of
//    those widgets/members exist yet in any translated form, and this file's
//    own write scope is Interface/TesterTCP.{h,cpp} + tests/test_TesterTCP.cpp
//    only (no FormsFacade edits permitted this wave) -- so, matching this
//    project's established extract-calc-core precedent (e.g.
//    TesterTCP_Socket.h's btTCPIP_TriggerClick promoting edTCPIP_CMD->Text to
//    a parameter), all 4 reads become explicit function parameters. A future
//    real TfTesterTCP UI wave should wire the 3 TCheckBox->Checked reads and
//    the LastFileName member straight through to these same parameters.
//---------------------------------------------------------------------------
#ifndef TesterTCP_MainH
#define TesterTCP_MainH

#include "vclcompat/vcl_compat.h"     // AnsiString

//---------------------------------------------------------------------------
//  golden TfTesterTCP::CopyRecipeToTester(AnsiString FileName), TesterTCP.cpp:1058-1077.
//  Steven 20250612 "for OS Tester."; Steven 20250327/20230710: OS Tester's own
//  work-file must also be uploaded (zips DataPath+FileName into OS_Setting.zip
//  under IniConfig.asN06_TesterPath, gated by IniConfig.bN06_CopyTesterFile).
//---------------------------------------------------------------------------
void TesterTCP_CopyRecipeToTester(AnsiString FileName);

//---------------------------------------------------------------------------
//  golden TfTesterTCP::CopyRecipeFromTester(AnsiString FileName), TesterTCP.cpp:1079-1097.
//  Steven 20250612 "for OS Tester."; the inverse direction of the pair above
//  (re-zips DataPath+FileName's OS_Setting.zip to append the Tester-side
//  <FileName>.ini found under IniConfig.asN06_TesterPath).
//---------------------------------------------------------------------------
void TesterTCP_CopyRecipeFromTester(AnsiString FileName);

//---------------------------------------------------------------------------
//  golden TfTesterTCP::btnSaveClick(TObject *Sender), TesterTCP.cpp:1099-1115.
//  RogerYang 20260210: per-machine arm-speed save request -- if enabled the
//  target directory becomes sSaveByMachine (per-machine shared save), else
//  DataPath+LastFileName (per-recipe save); writes 3 JSCC_OS_Use AutoN flags
//  into <dir>\ArmCondition.Data. extract-calc-core: the 3 golden
//  chkAutoN_OS->Checked widget reads and the golden LastFileName form-member
//  read are promoted to explicit parameters (see file banner).
//---------------------------------------------------------------------------
void TesterTCP_btnSaveClick(bool bChkAuto1_OSChecked,
                             bool bChkAuto2_OSChecked,
                             bool bChkAuto3_OSChecked,
                             AnsiString LastFileName);

#endif // TesterTCP_MainH
