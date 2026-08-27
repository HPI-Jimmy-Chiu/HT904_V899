// =============================================================================
//  forms/fAirCon.h  --  non-VCL facade for golden's TfAirCon (cAirCon.h)
//
//  AI(W906-FW3-TIF1) 20260828: new file, FW wave FW3-TIF1 (facade wave 1 of
//  cAirCon -- read-only direction, no write path). Golden source of truth:
//  HT9011UC_Code_V3.33.906.0_20260618/cAirCon.h (69 lines) + cAirCon.cpp
//  (197 lines). Decoded this wave with `io.open(p, encoding='cp950')`
//  (0 U+FFFD asserted on the whole file before any quoting), written out
//  UTF-8 / bare-LF.
//
//  ROLE
//  ----
//  TfAirCon drives the chamber AIR-CONDITIONER power supply over a serial
//  port (SPComm `TComm`): it builds SCPI-ish text commands
//  (":OUTPut:STATe 1" / ":CHAN1:VOLT 00.00;CURR %5.2f" / "*CLS" /
//  "CHAN1:CURR ?"), writes them to the COM port, mirrors both directions
//  into an on-screen memo, and -- via PowerUp()/PowerDown() -- ALSO flips the
//  machine's own `SW[SwAirConditioner]` output switch. Everything this form
//  does is therefore either an outbound serial command or a hardware switch,
//  which is why the ACTIVE set below is deliberately small.
//
//  `python tools/census/wave_preflight.py cAirCon.cpp` (20260828, re-run at
//  closeout -- see ABSENCE-CLAIM note at the bottom): 16 bodies = 16
//  `TfAirCon::` members + 0 file-scope, 164 span lines, 0 brace-swallow
//  short-fall, 0 overload collapse. THIS WAVE: 5 of 16 ACTIVE (31.3%) + 11
//  GATED. BY LINES: the 5 ACTIVE bodies' golden spans total 36 of 164 lines
//  (22.0%). Quote both numbers or neither.
//
//  ============================================================================
//  *** THE GLOBALS `TfAirCon` / `fAirCon` ARE FREE -- THIS WAVE CLAIMS THEM ***
//  Measured 20260828 05:14 over the 1,199 git-tracked .cpp/.h/.hpp files of
//  this tree (`git ls-files -- '*.cpp' '*.h' '*.hpp'` + per-file regex scan;
//  build_*/ excluded because they are not tracked):
//      `TfAirCon`  -- 1 hit, tools/dfm2rc/layout_out/cAirCon_layout.gen.cpp:12,
//                     a GENERATED layout row (a string literal naming the dfm
//                     root class), not a C++ class definition.
//      `fAirCon`   -- 19 hits, ALL of them either the same generated layout
//                     row, prose in a banner, or one of the 6 PRE-COMMITTED
//                     `#if 0` call sites listed below. 0 class definitions,
//                     0 global definitions.
//  `.h` declares `extern TfAirCon *fAirCon;`, `.cpp` defines
//  `TfAirCon *fAirCon = new TfAirCon();` -- same idiom as
//  forms/fPrecaution.cpp:14 / forms/fHotPlate.cpp:24 / forms/fCleaning.cpp:45.
//
//
//  CMakeLists.txt LINE NUMBERS WERE RE-MEASURED AFTER REGISTRATION.
//  Every `CMakeLists.txt:N` / `ht9045_xxx:N` citation in this file was first
//  taken at 20260828 05:10, then the integrating wave registered this facade's
//  .cpp into `add_library(ht9045_forms ...)` -- a 53-line insertion (46 lines, plus a later 7-line
//  correction by the integrator) at
//  CMakeLists.txt:924-976 -- which shifted EVERY citation from :935 onward by
//  +53 in total. They were re-measured TWICE -- after the registration, and
//  again after that 7-line correction -- each time READ OFF the file rather
//  than derived by adding an offset, because deriving is how off-by-N gets in.
//  All were
//  corrected. Citations at :272 / :485 / :699 sit ABOVE the insertion point and
//  are unchanged. Adding sources to ht9045_forms moves these numbers again --
//  re-measure, do not trust them blind.
//  ============================================================================
//  PRE-COMMITTED `#if 0` CALL SITES ELSEWHERE IN THE TREE (6, all in
//  csystem.cpp's OWN gates H1-09a..H1-09f). NAMES AND SIGNATURES ARE
//  PRE-COMMITTED -- NOT THIS WAVE'S CHOICE.
//  ============================================================================
//    PowerUp(5.0)   csystem.cpp:26877  (GATE H1-09a, bCCDOverTemp fast-cool)
//    PowerDown()    csystem.cpp:26898  (GATE H1-09b, hot / ambient-hot)
//    PowerUp(5.5)   csystem.cpp:26966  (GATE H1-09c, CC_ASE_KaohSiung fast-cool)
//    PowerUp(0.5)   csystem.cpp:26982  (GATE H1-09d, CC_ASE_KaohSiung idle)
//    PowerUp(5.5)   csystem.cpp:27004  (GATE H1-09e, bHALTing fast-cool)
//    PowerUp(0.5)   csystem.cpp:27020  (GATE H1-09f, idle)
//  Every one passes ONE double, so this facade keeps golden's
//  `void PowerUp(double dCurrent=0.1);` signature verbatim. All 6 csystem.cpp
//  gates are STILL `#if 0` after this wave -- declaring the members does NOT
//  un-gate them (that is csystem.cpp's own separate, deliberate edit, outside
//  this wave's write boundary).
//
//  ============================================================================
//  GATE REGISTER (11)
//  ============================================================================
//  Every entry below is DECLARED here and its golden body is translated in
//  forms/fAirCon.cpp inside `#if 0` -- so the text exists for a future wave
//  but calling one of these is a LINK ERROR by design (same net effect as
//  forms/fHotPlate.h's declared-not-defined bucket, plus the translation).
//
//  (A-1) SendCommand          golden :67-77 -- SENDS ON THE WIRE. Line :76
//        `CommAirCon->WriteCommData(cStr, strlen(cStr));`. `CommAirCon` is a
//        golden `TComm*` (SPComm) and this tree DOES have a real one
//        (vclcompat/Comm.h:126 `class TComm : public TComponent`, body
//        vclcompat/Comm.cpp, target `vclcompat` -- reachable from
//        ht9045_forms), whose `StartComm()` opens an actual Win32 COM handle
//        and whose `WriteCommData()` writes real bytes unless SetSimMode() was
//        called first. So this is NOT a missing-symbol gate: it would link and
//        it would transmit. Gated by the wave's SAFETY POLICY (唯讀方向:
//        對外送指令一律不得是 live 的), not by the link boundary.
//  (A-2) InitialAirConCom     golden :96-120 -- OPENS THE PORT. :108
//        `CommAirCon->StartComm();` inside try/catch. Same "real TComm"
//        finding as (A-1): opening a COM port is a hardware action. Also
//        depends on `GetCOMPortStatus` (:104) whose body is
//        EJ1N/TextProcess.cpp:770 -- target `ht9045_globals`
//        (CMakeLists.txt:485), i.e. actually REACHABLE from ht9045_forms; and
//        on `MyDBIProcess` (:112, aHotPlateSubstrate.cpp, ht9045_sm) which is
//        one of the FOUR verified forms->sm exceptions. So, again, the gate is
//        policy, not reachability -- recorded explicitly so a future wave does
//        not "discover" it links and un-gate it by accident.
//  (A-3) StartOutput          golden :139-145 -- TRANSITIVE on (A-1);
//        both arms are `SendCommand(":OUTPut:STATe 1"/"0")`.
//  (A-4) SetCurrent           golden :147-153 -- TRANSITIVE on (A-1) at :152.
//        `CheckRange` (:150) is the MachineType.h:1525 template (header-only,
//        reachable) -- it is not what gates this.
//  (A-5) SetCLS               golden :155-158 -- TRANSITIVE on (A-1).
//  (A-6) GetStatus            golden :160-163 -- TRANSITIVE on (A-1).
//  (A-7) GetCurrent           golden :165-168 -- TRANSITIVE on (A-1).
//  (A-8) PowerUp              golden :170-179 -- TWO INDEPENDENT GATES.
//        (a) SAFETY/HARDWARE: :178 `SW[SwAirConditioner].On();` energises the
//            machine's air-conditioner output switch.
//        (b) LINK BOUNDARY: `SW[]` is `extern class TMySwitch SW[MAX_SWITCH_ITEM]`
//            (myswitch.h:43); its DEFINING TU is myswitch.cpp:46, whose
//            `add_library` block is `ht9045_io` (CMakeLists.txt:1240, located
//            this wave by walking up to the enclosing add_library). This
//            facade's landing is `ht9045_forms`, which links only
//            vclcompat + ht9045_globals + ht9045_core (CMakeLists.txt:988-995)
//            -- it does NOT link ht9045_io.
//        Plus TRANSITIVE on (A-4)/(A-3)/(A-5) inside the USE_AIR_CONDITIONER==2
//        arm. This is the more consequential of the two PRE-COMMITTED names.
//  (A-9) PowerDown            golden :181-189 -- same (a)+(b)+transitive as
//        (A-8); :188 `SW[SwAirConditioner].Off();`.
//  (A-10) sbUpdateClick       golden :35-65 -- TRANSITIVE. Its `switch` on
//        `rgAirConCommand->ItemIndex` dispatches to SetCLS / StartOutput(true)
//        / StartOutput(false) / GetStatus / SetCurrent / GetCurrent -- i.e.
//        EVERY arm lands in a gated, wire-writing method. The prologue
//        (`IniConfig.bA02DisableSaveParsWhenSwitchToOp` Config.cpp/
//        ht9045_globals, `AccessLevel` cmydef.cpp/ht9045_globals,
//        `ShowMyMessage` canary_support.cpp/ht9045_sm but one of the four
//        verified exceptions, `Close()` this facade's own no-op) is entirely
//        reachable -- it is the switch body that gates the whole function.
//        NOT split: dropping the dispatch would leave an operator-permission
//        check that does nothing, i.e. an invented degenerate behaviour.
//  (A-11) FormDestroy          golden :122-137 -- THREE reasons, any one
//        sufficient. (a) TRANSITIVE on (A-9) `PowerDown();` (:128).
//        (b) HARDWARE: `CommAirCon->StopComm();` (:129) closes a real port.
//        (c) LINK BOUNDARY: `LogSoftwareOffTime("TfAirCon, FormDestroy")`
//            (:136) is declared cmydef.h:5032 but its ONLY definition is
//            acarry_shims.cpp:255, target `ht9045_sm` (CMakeLists.txt:2107).
//            `LogSoftwareOffTime` is explicitly NOT one of the four verified
//            forms->sm exceptions (those are RecordProcess / ShowMyMessage /
//            ShowErrorMessage in canary_support.cpp and MyDBIProcess in
//            aHotPlateSubstrate.cpp) -- same finding forms/fMonitor.h GATE
//            (M-3) and forms/fNote.h (N-8) already record independently.
//
//  ============================================================================
//  ACTIVE (5) -- reachability evidence, symbol by symbol
//  ============================================================================
//   TfAirCon()                golden :17-20 -- body is EMPTY past
//        `: TForm(Owner)`. Fields-only, no global deref, no function call ->
//        SIOF-safe for the static-init `new` in the .cpp (docs/KNOWLEDGE.md
//        "static-init ctor 不可碰 NULL 全域").
//   AddMessage(AnsiString)    golden :22-28 -- `Memo1->Lines->Count` /
//        `Memo1->Clear()` / `Memo1->Lines->Add(Str)`. `Memo1` is this
//        facade's OWN `vclcompat::TMemo` (Controls.h:367) whose `Lines` is a
//        REAL `TStringList` (vclcompat/TStringList.cpp, target `vclcompat`)
//        and whose `Clear()` forwards to `Lines->Clear()` -- i.e. golden's
//        1024-line flush is genuinely reproduced, not a no-op.
//   sbtExitClick(TObject*)    golden :30-33 -- `Close();` only (DEVIATION D-2,
//        this facade's own port-only no-op).
//   CommAirConReceiveData(TObject*, void*, Word)
//                             golden :79-94 -- pure RX-side formatting into a
//        local buffer plus `AddMessage`. Touches no global, no file, no port.
//        See DEVIATION D-3 for the parameter types and GOLDEN BUGS below for
//        the two defects preserved verbatim.
//   edOutputCurrentMouseDown(TObject*, TMouseButton, TShiftState, int, int)
//                             golden :191-195 -- `fQwertyKey->ShowQwertyKey(
//        (TEdit *)Sender, N_DOUBLE, 1, true, 5.8, 0.0);`. `fQwertyKey` is
//        REAL and in the SAME target (forms/fQwertyKey.cpp, ht9045_forms,
//        CMakeLists.txt:699); its signature is
//        `ShowQwertyKey(TControl*, int, int=0, bool=false, double=0, double=0)`
//        (forms/fQwertyKey.h:370) -- golden's 6 arguments bind exactly.
//        `N_DOUBLE` is `extern const int` (cmydef.h:290, body cmydef.cpp,
//        ht9045_globals).
//
//  ============================================================================
//  GOLDEN BUGS PRESERVED VERBATIM (in ACTIVE CommAirConReceiveData)
//  ============================================================================
//  (B-1) golden :87 `LogMsg.sprintf("Receive (%d): %s", BufferLength, Buffer);`
//        passes a `void*` to a `%s` conversion. That is undefined behaviour in
//        golden and here alike (it reads the bytes at Buffer as a NUL-terminated
//        C string, which is what golden intends but not what `%s` on a void*
//        is defined to do). Kept verbatim -- changing it would be a behaviour
//        decision that belongs to the user, not to a translation wave. It does
//        NOT trip -Wformat here because vclcompat AnsiString::sprintf is a
//        variadic TEMPLATE (vclcompat/AnsiString.h:150) with no format
//        attribute, so the compiler cannot see the mismatch either.
//  (B-2) golden :92-93 formats `"Read  : %s"` into the local `cStr` and then
//        calls `AddMessage(cPtr)` -- i.e. the formatted string is BUILT AND
//        THROWN AWAY and the raw buffer is logged instead, without the
//        "Read  : " prefix. Also kept verbatim.
//
//  ============================================================================
//  DEVIATIONS (port-only; each with why)
//  ============================================================================
//  (D-1) `__fastcall` dropped from every signature -- tree-wide convention.
//  (D-2) `Close()` is a PORT-ONLY no-op (`{}`) -- golden inherits
//      `TForm::Close` and this facade has no TForm base (D-6). Needed by
//      ACTIVE sbtExitClick and by GATED sbUpdateClick. Same shape as
//      forms/fPrecaution.h D-2 / forms/fHotPlate.h D-3.
//  (D-3) `CommAirConReceiveData(TObject*, Pointer, WORD)` ->
//      `(TObject*, void*, Word)`. There is NO `Pointer` typedef anywhere in
//      this tree (`Pointer` scan over the 1,199 tracked sources, 20260828:
//      0 typedef hits) and `WORD` would drag <windows.h> into a forms header.
//      `void*`/`Word` are the SAME types and are exactly what
//      vclcompat/Comm.h:120-121 already chose for its own
//      `TReceiveDataEvent` -- that header's banner states the equivalence in
//      as many words ("consumers spell the 2nd/3rd params `Pointer Buffer,
//      WORD BufferLength`, which are the same types"). `Word` comes in via
//      vclcompat/vcl_compat.h:262.
//  (D-4) `InitialAirConCom(AnsiString, TParity=None, unsigned=9600,
//      TByteSize=_8, bool=false, TStopBits=_1)` keeps golden's defaults
//      verbatim; the three enums are the real Spcomm ones
//      (vclcompat/Comm.h:110-112, byte-identical ordering to spcomm.hpp).
//      Declared even though the body is gated, because the enum values are
//      the load-bearing part of the contract.
//  (D-5) `FormDestroy(TObject*)` keeps its parameter (unlike
//      forms/fPrecaution.h D-5's FormClose, no unportable type is involved).
//  (D-6) `TfAirCon` has NO base class (matches forms/fPrecaution.h D-4 /
//      forms/fHotPlate.h D-2 / forms/fCleaning.h).
//  (D-7) golden's private `AnsiString LogMsg;` (cAirCon.h:35, "JimmyChiu
//      20210922 Avoid overflow") is declared PUBLIC here. Golden's own
//      visibility is private and only golden's own members touch it; making
//      it public costs nothing and matches this tree's facade practice of not
//      reproducing VCL access sections. Flagged rather than silently done.
//
//  ============================================================================
//  FIELD LIST (dfm leaf names, per the FW rule "widget 存取改成 UI-state 欄位")
//  ============================================================================
//    Memo1            (TMemo*)        golden :18 -- ACTIVE AddMessage
//    rgAirConCommand  (TRadioGroup*)  golden :20 -- GATED sbUpdateClick
//    sbUpdate         (TSpeedButton*) golden :21 -- declared for completeness
//    sbtExit          (TSpeedButton*) golden :22 -- ACTIVE sbtExitClick reads
//                                     nothing off it (golden's own body does
//                                     NOT set ->Down, unlike fHotPlate's), so
//                                     it is declared but currently unread.
//    edOutputCurrent  (TEdit*)        golden :23 -- GATED sbUpdateClick
//    Label1           (TLabel*)       golden :24
//    Panel1           (TPanel*)       golden :19
//    LogMsg           (AnsiString)    golden :35 (see D-7)
//  NOT declared: `CommAirCon` (golden :17, `TComm*`). A real
//  `vclcompat::TComm` EXISTS and would compile -- but every one of its four
//  golden uses (StartComm/StopComm/WriteCommData/the property block) is
//  inside a gated body, and allocating one in this facade's NSDMI would
//  construct a serial-port object (and its Impl) at static-init time in every
//  executable that links ht9045_forms, for zero live benefit. Declared inside
//  the `#if 0` region's own commentary instead. THIS IS A DELIBERATE
//  OMISSION, not an oversight: a future wave that un-gates (A-1)/(A-2) must
//  add the member AND decide SetSimMode() policy first.
//
//  ============================================================================
//  ZERO-WRITER FIELDS -- observable consequence, flagged per task rule 7
//  ============================================================================
//  `rgAirConCommand->ItemIndex` and `edOutputCurrent->Text` are read ONLY by
//  the GATED sbUpdateClick, and NOTHING in this tree writes them, so they
//  stay at their `new`-defaults (0 / "") for as long as (A-10) is gated.
//  `Memo1` is written by ACTIVE AddMessage and read by nothing else in this
//  tree -- so the memo is a real, growing sink with no consumer. Both are
//  consequences of the gate boundary, not defects in the ACTIVE bodies.
//
//  ============================================================================
//  ABSENCE-CLAIM PROVENANCE (trap #2: these expire)
//  ============================================================================
//  Every "0 hits" above was measured at 20260828 05:14 with
//      git ls-files -- '*.cpp' '*.h' '*.hpp'   (1,199 files)
//  plus a per-file Python regex scan (build_*/ is untracked and therefore
//  excluded). Re-run at closeout -- a sibling wave landing forms/fAirCon.*-
//  adjacent files could falsify the `TfAirCon`/`fAirCon` freedom claim.
// =============================================================================
#ifndef FORMS_FAIRCON_H
#define FORMS_FAIRCON_H

#include "forms/FormWidgets.h"     // AnsiString / Word / TMemo / TRadioGroup / TEdit / TLabel / TPanel / TSpeedButton
#include "vclcompat/ShiftState.h"  // TMouseButton / TShiftState (edOutputCurrentMouseDown)
#include "vclcompat/Comm.h"        // Spcomm::TParity / TByteSize / TStopBits (InitialAirConCom's golden defaults, D-4)

// ===========================================================================
//  TfAirCon -- non-VCL facade (golden cAirCon.h:14-65,
//  `class TfAirCon : public TForm`; NO base class here, see DEVIATION D-6).
// ===========================================================================
class TfAirCon
{
public:
    TfAirCon();

    // -- golden __published widgets (dfm leaf names) ------------------------
    TMemo        *Memo1           = new TMemo();          // golden :18
    TPanel       *Panel1          = new TPanel();         // golden :19
    TRadioGroup  *rgAirConCommand = new TRadioGroup();    // golden :20
    TSpeedButton *sbUpdate        = new TSpeedButton();   // golden :21
    TSpeedButton *sbtExit         = new TSpeedButton();   // golden :22
    TEdit        *edOutputCurrent = new TEdit();          // golden :23
    TLabel       *Label1          = new TLabel();         // golden :24

    // -- golden private: User declarations (public here, DEVIATION D-7) -----
    AnsiString LogMsg;                                     // golden :35, JimmyChiu 20210922 Avoid overflow

    // -- golden's own command enum, values verbatim ------------------------
    enum eAirConCMD{eCLS   =0, //致能輸出電壓
                    ePwrOn =1, //開啟輸出電壓
                    ePwrOff=2, //關閉輸出電壓
                    ePwrSts=3, //回傳power supply狀態
                    eSetCur=4, //設定電壓電流值
                    eGetCur=5, //讀取電流
                    eCmdTotal
                   };

    // -- ACTIVE this wave (defined in forms/fAirCon.cpp) --------------------
    void AddMessage(AnsiString Str);                        // golden :22-28   ACTIVE
    void sbtExitClick(TObject *Sender);                     // golden :30-33   ACTIVE
    void CommAirConReceiveData(TObject *Sender,
                               void *Buffer, Word BufferLength);  // golden :79-94  ACTIVE, D-3
    void edOutputCurrentMouseDown(TObject *Sender, TMouseButton Button,
                                  TShiftState Shift, int X, int Y);// golden :191-195 ACTIVE
    void Close() {}                                          // DEVIATION D-2, port-only no-op

    // -- GATED this wave -- declared here, body translated in
    //    forms/fAirCon.cpp inside `#if 0`. Calling one is a LINK ERROR by
    //    design (see GATE REGISTER above). --------------------------------
    void sbUpdateClick(TObject *Sender);                     // golden :35-65   GATE (A-10)
    void FormDestroy(TObject *Sender);                       // golden :122-137 GATE (A-11), D-5
    void InitialAirConCom(AnsiString ComPort,
                          Spcomm::TParity   Parity      = Spcomm::None,
                          unsigned          Rate        = 9600,
                          Spcomm::TByteSize ByteSize    = Spcomm::_8,
                          bool              ParityCheck = false,
                          Spcomm::TStopBits StopBits    = Spcomm::_1);  // golden :96-120 GATE (A-2), D-4

    void StartOutput(bool bEnable);     //開啟輸出電壓                   // golden :139-145 GATE (A-3)
    void SetCurrent(double dCurrent);   //設定電流                       // golden :147-153 GATE (A-4)
    void SetCLS();                      //致能輸出電壓                   // golden :155-158 GATE (A-5)
    void GetStatus();                   //讀取Power狀態                  // golden :160-163 GATE (A-6)
    void GetCurrent();                  //讀取電流                       // golden :165-168 GATE (A-7)

    void PowerUp(double dCurrent=0.1);  //啟動電源   // golden :170-179 GATE (A-8), pre-committed csystem.cpp:26877/26966/26982/27004/27020
    void PowerDown();                   //關閉電源   // golden :181-189 GATE (A-9), pre-committed csystem.cpp:26898

private:
    void SendCommand(AnsiString Str);                        // golden :67-77   GATE (A-1), private (matches golden :33)
};

// AI(W906-FW3-TIF1) 20260828: TfAirCon/fAirCon were FREE tree-wide (see
// banner's measured provenance) -- declared here so the 6 pre-committed
// csystem.cpp `#if 0` sites have something real to bind to once csystem.cpp's
// OWN gates are separately lifted (NOT done by this wave).
extern TfAirCon *fAirCon;

#endif // FORMS_FAIRCON_H
