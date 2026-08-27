// =============================================================================
//  forms/fWinway.h  --  non-VCL facade for golden's TfWinway / fWinway
//                        (AMD-M Winway 4-site ATC comm-port setup form)
//
//  AI(W906-FW3-OWM1) 20260828: new file. Wave FW3-OWM1 (golden
//  ATC/WinWaySetting.cpp, HT9011UC_Code_V3.33.906.0_20260618, cp950-decoded
//  with `python -c "open(p,encoding='cp950').read()"`, 0 U+FFFD).
//  Denominator (tools/census/wave_preflight.py, 20260828): 17 TRUE bodies
//  (16 distinct names -- `OpenCommPort` is overloaded, defined TWICE at
//  golden :156 int-arg and :164 no-arg, which census's name-keyed dict
//  collapses to one slot; wave_preflight's own overload check catches this),
//  206 span lines.
//
//  WHY THIS FILE MATTERS BEYOND ITS OWN LOGIC: TfWinway/fWinway were BOTH
//  completely absent from the port before this wave (class name + global
//  name both "clear" per wave_preflight's shim-occupation scan, 20260828 --
//  the 4 `(b)` hits it found are all `ATC_WinWay::` methods on the UNRELATED,
//  already-ported `ATC_WinWay` class, or `TfLotInfo::FormShow`, neither an
//  occupation). 3 predetermined names are referenced by #if 0 call sites
//  elsewhere: `OpenCommPort` and `SetTempratureAll` (forms/fLotInfo.cpp
//  :4256/:4272, inside GATE WC-24, itself `#if 0` since 20260819) and
//  `arrATC_Site` (bthermo.cpp:4609/4611/4625 and others). All 3 are declared
//  below with matching name+type; un-gating those call sites is a separate,
//  safety-relevant decision left to whoever owns fLotInfo.cpp/bthermo.cpp.
//
//  THE SINGLE REASON THIS FACADE IS THIN: every golden method except the
//  ctor's field-only prefix, FormShow's geometry lines, cbbWinwayATCIndexChange's
//  first line and edtSetTempClick reaches `arrATC_Site[n]`, an `ATC_WinWay*`.
//  ATC_WinWay is ALREADY translated (ATC/ATC_WinWay.h/.cpp, PT-W2) but its
//  .cpp body lives in ht9045_comms (CMakeLists.txt:1432); ht9045_forms links
//  only vclcompat+ht9045_globals+ht9045_core (CMakeLists.txt:859) -- NOT one
//  of the 4 verified ht9045_forms->ht9045_sm exceptions either (RecordProcess/
//  ShowMyMessage/ShowErrorMessage/MyDBIProcess; ATC_WinWay is a different
//  class in a different archive, ht9045_comms, not ht9045_sm). This wave does
//  NOT add a CMake edge (outside the write boundary), so every arrATC_Site[n]
//  METHOD CALL is left permanently unreachable. WORSE THAN A LINK ERROR: this
//  wave's ctor (see PORT SPLIT below) leaves `arrATC_Site[n]` explicitly
//  NULL (golden's own `new ATC_WinWay(...)` is gated, not merely deferred),
//  so even a FIELD-only read through `arrATC_Site[n]->` would compile clean
//  and NULL-DEREF the instant anything actually called it. That is why
//  ShowCommData/LoadCommData/SaveCommData/btnUpdateClick/CloseCommPort/
//  SetST/SetTemprature/btnGetPVClick/WinWayATCComm1ReceiveData are EXCLUDED
//  ENTIRELY below rather than partially translated: a field-only body that
//  compiles clean but null-derefs on first call is a worse deliverable than
//  no body at all ("缺符號比錯答案好，不要寫退化版本").
//
//  WAVE SCOPE (17 bodies; golden line numbers, this file's own numbering)
//  --------------------------------------------------------------------------
//    TfWinway ctor              golden :12-32   PORT SPLIT (see below)
//    FormShow                   golden :34-41   ACTIVE, 1 line gated (GATE W-1)
//    ShowCommData                golden :43-76   EXCLUDED (see below)
//    btnUpdateClick               golden :78-114  EXCLUDED (see below)
//    LoadCommData                 golden :116-133 EXCLUDED (see below)
//    SaveCommData                 golden :135-148 EXCLUDED (see below)
//    cbbWinwayATCIndexChange      golden :150-154 ACTIVE, 1 line gated (GATE W-1)
//    OpenCommPort(int)            golden :156-162 EXCLUDED (see below)
//    OpenCommPort()                golden :164-168 DECLARED, body fully gated
//                                                  (GATE W-2 -- predetermined name)
//    SetST()                       golden :170-174 EXCLUDED (see below)
//    CloseCommPort(int)            golden :176-182 EXCLUDED (see below)
//    btnSendTempClick               golden :184-188 EXCLUDED (see below)
//    SetTemprature(double)          golden :190-192 EXCLUDED (see below)
//    SetTempratureAll(double)       golden :195-201 DECLARED, body fully gated
//                                                  (GATE W-2 -- predetermined name)
//    btnGetPVClick                   golden :203-205 EXCLUDED (see below)
//    WinWayATCComm1ReceiveData        golden :208-233 EXCLUDED (see below)
//    edtSetTempClick                  golden :235-238 ACTIVE verbatim
//  Total: 4 ACTIVE (2 with one gated line each) + 2 DECLARED-but-fully-gated
//  (predetermined names, 0 live lines) + 11 EXCLUDED ENTIRELY = 17/17.
//
//  GATE REGISTER
//  --------------------------------------------------------------------------
//  (W-1) FormShow golden :39 / cbbWinwayATCIndexChange golden :39(again, via
//      the shared call) -- `ShowCommData(iWinwayATCIndex);`. ShowCommData
//      itself is EXCLUDED (not declared -- see below), so the call cannot
//      resolve; gated in place, one line each, rest of both functions ACTIVE
//      (SmartDiagnostic SD-5 "gate one line, rest ACTIVE" precedent).
//  (W-2) OpenCommPort() / SetTempratureAll(double) -- BOTH on the
//      predetermined-name contract (forms/fLotInfo.cpp:4256/:4272) so BOTH
//      are declared with golden's exact name+signature; their ENTIRE golden
//      bodies loop over `arrATC_Site[i]->...` (OpenCommPort(int)/SetST, both
//      independently EXCLUDED -- see WHY THIS FILE IS THIN above), so there
//      is no line left to keep live. Declared empty (parameter explicitly
//      cast to void where otherwise unused) so a future un-gate of
//      fLotInfo.cpp's GATE WC-24 binds to the right name+type; the body
//      itself stays a documented no-op until ATC_WinWay is reachable from
//      ht9045_forms (a CMake decision, out of this wave's scope) AND the
//      null-arrATC_Site problem above is separately resolved.
//
//  EXCLUDED ENTIRELY (not declared as class members -- nothing here calls
//  them, and none besides the two GATE W-2 names is on the predetermined-name
//  list, so there is no future-linking contract to honour by declaring an
//  undefined member):
//    * ShowCommData(int)         golden :43-76   -- arrATC_Site[iIndex]->
//        GetPT()/GetST() are ATC_WinWay:: METHOD CALLS (ht9045_comms, not
//        reachable); every other line reads through the same permanently-
//        NULL pointer (see WHY THIS FILE IS THIN). Not on the predetermined
//        list.
//    * btnUpdateClick             golden :78-114  -- SAFETY (opens/re-opens a
//        real COM port with new settings) AND arrATC_Site[..]->WinwayCOM
//        writes/CloseCommPort()/OpenCommPort() calls, AND calls the
//        write-path-excluded SaveCommData. Not on the predetermined list.
//    * LoadCommData(TComm*,int)   golden :116-133 -- SAFETY (calls
//        ATC_WinWay::SetST -- a hardware setpoint command) AND
//        arrATC_Site[iIndex] null-deref AND cross-archive method call. Not on
//        the predetermined list (only `OpenCommPort`/`SetTempratureAll`/
//        `arrATC_Site` are).
//    * SaveCommData(TComm*,int)   golden :135-148 -- WRITE PATH:
//        `IniFile->WriteString/WriteInteger/WriteFloat` (vclcompat::TIniFile
//        write-through = immediate disk write) AND the same null-deref/
//        cross-archive problem via `tempATC_WinWay->GetST()`.
//    * OpenCommPort(int _idx)      golden :156-162 -- SAFETY (opens a real COM
//        port) AND `arrATC_Site[_idx]->OpenCommPort()` null-deref/cross-
//        archive. NOT itself on the predetermined list (only the NO-ARG
//        overload is, per forms/fLotInfo.cpp:4256's exact call shape,
//        verified this wave -- `fWinway->OpenCommPort();`, zero arguments).
//    * SetST()                     golden :170-174 -- SAFETY (hardware
//        setpoint via ATC_WinWay::SetST) AND null-deref/cross-archive. This
//        is TfWinway's OWN no-arg SetST, unrelated to (and not colliding
//        with, different class) ATC_WinWay::SetST. Not predetermined.
//    * CloseCommPort(int _idx)     golden :176-182 -- SAFETY (closes a real
//        COM port) AND null-deref/cross-archive. Not predetermined (only
//        `OpenCommPort` is named at the call site).
//    * btnSendTempClick             golden :184-188 -- calls the excluded
//        SetTemprature(double) (a DIFFERENT, non-predetermined member -- see
//        next bullet) plus an unported `ShowMessage(...)`.
//    * SetTemprature(double)        golden :190-192 -- SAFETY (hardware
//        setpoint, single-site) AND null-deref/cross-archive via
//        `arrATC_Site[iWinwayATCIndex]->SetST(_value)`. NOT predetermined
//        (`SetTempratureAll`, the ALL-sites sibling, is -- these are two
//        distinct golden methods, not an overload of each other).
//    * btnGetPVClick                 golden :203-205 -- null-deref/cross-
//        archive via `arrATC_Site[iWinwayATCIndex]->GetPT()`.
//    * WinWayATCComm1ReceiveData      golden :208-233 -- needs `TComm::Name`
//        (golden `tempTComm->Name`), which vclcompat::Spcomm::TComm does NOT
//        carry (vclcompat/Comm.h's own "SCOPED API SURFACE" list has no
//        `Name` entry -- grepped this wave, 0 hits under vclcompat/Comm.h)
//        -- a genuine missing-symbol gap, not merely a boundary issue --
//        PLUS `_atc->SetPT((char*)Buffer)` is the same null-deref/cross-
//        archive problem as everything above.
//
//  DEPENDENCY AUDIT (this wave, 20260828) -- all grepped before translating:
//    ATC_WinWay                REAL type, ht9045_comms (ATC/ATC_WinWay.cpp,
//                               CMakeLists.txt:1432) -- forward-declared
//                               ONLY below (pointer field, no method call, no
//                               `new` -- does NOT create a link edge; see
//                               forms/fSmartDiagnostic.h's mycylin.h
//                               precedent for the same "type visible, symbol
//                               never used" distinction).
//    fQwertyKey / ShowQwertyKey  REAL  forms/fQwertyKey.h:406/:370, compiled
//                               into ht9045_forms (CMakeLists.txt:699) --
//                               same archive, no new edge. Global itself
//                               stays NULL until a wiring wave constructs it
//                               (fQwertyKey.h D-8/GATE precedent) -- this
//                               wave adds no NEW caller of it beyond the one
//                               line edtSetTempClick already had in golden.
//    N_DOUBLE                    REAL  cmydef.h:290 (extern const int),
//                               body cmydef.cpp:351, ht9045_globals
//    dTempMax                    REAL  cmydef.h:3437 (extern double), body
//                               cmydef.cpp:3649, ht9045_globals
//    TEdit / TComboBox / TObject  REAL  vclcompat/Controls.h /
//                               vclcompat/vcl_compat.h (both global)
//    SiteNum (=4)                 golden #define, unclaimed tree-wide
//                               (grepped this wave, 0 other hits) -- kept as
//                               the golden macro name+value, guards the
//                               `arrATC_Site` array size the predetermined
//                               name needs.
//
//  CMAKE STATUS -- NOT WIRED by this wave (CMakeLists.txt is outside the
//  write boundary). Suggested landing: forms/fWinway.cpp -> the ht9045_forms
//  source list (CMakeLists.txt:654 block) -- diet-clean AS TRANSLATED: no
//  ht9045_comms/sm/io/motor/db symbol is touched by any COMPILED
//  (non-#if-0) line in this file.
// =============================================================================
#ifndef FORMS_FWINWAY_H
#define FORMS_FWINWAY_H

#include "vclcompat/vcl_compat.h"   // AnsiString/TObject at global scope
#include "vclcompat/Controls.h"     // TEdit/TComboBox (global via `using vclcompat::...;`)

// golden ATC/WinWaySetting.h:16 `#define SiteNum 4` -- unclaimed tree-wide
// (grepped 20260828), kept verbatim as the array size for arrATC_Site below.
#define SiteNum 4

// Forward-declared ONLY -- see DEPENDENCY AUDIT above. No method of
// ATC_WinWay is ever called from this file, and it is never `new`'d here
// (golden's `new ATC_WinWay(...)` is GATED, see the .cpp ctor), so the
// pointer-only field below does not need (and must not pull in) the full
// class definition / its ht9045_comms link edge.
class ATC_WinWay;

// ===========================================================================
//  TfWinway -- non-VCL facade (golden ATC/WinWaySetting.h:20-64,
//  TfWinway : public TForm)
// ===========================================================================
class TfWinway
{
public:
    TfWinway();   // golden ctor :12-32, PORT SPLIT -- see .cpp

    // -- golden __published (h:24-45) -- only what a translated (ACTIVE or
    // GATE-W-1-adjacent) body dereferences by name is declared, per
    // forms/fDynamicTemp.h "only what's dereferenced" convention. Skipped:
    // grpcps/lbl1-7/cbbBaudRate/cbbByteSize/cbbStopBit/cbbParity/cbbDevice/
    // lblWinwayNumber/btnUpdate/grpcommu/btnSendTemp/lblShowPT/
    // WinWayATCComm1-4/btnGetPV -- every one of them is touched ONLY inside
    // an EXCLUDED body above.
    TEdit     *edtSetTemp         = new TEdit();
    TComboBox *cbbWinwayATCIndex  = new TComboBox();

    // -- golden public: User declarations (h:47-64) --------------------------
    int iWinwayATCIndex = 0;

    // GATE-REGISTER predetermined name (bthermo.cpp:4609/:4611/:4625 and
    // others, all inside `#if 0` GATE WC-24 -- forms/fLotInfo.cpp:4248-4252).
    // Left permanently NULL by this wave's ctor -- see WHY THIS FILE IS THIN
    // in the header banner above; every member that would populate or read
    // through it is EXCLUDED for exactly that reason.
    ATC_WinWay *arrATC_Site[SiteNum];

    bool bShow = false;

    // Form-level plain-int fields (golden TForm::Width/Height/Left/Top),
    // same D-7-class deviation as forms/fQwertyKey.h/forms/fGroundMan.h:
    // this facade does not derive TForm, so these are ordinary data members
    // the translated ctor/FormShow read and write exactly like golden does.
    int Width  = 0;
    int Height = 0;
    int Left   = 0;
    int Top    = 0;

    void FormShow();                          // golden :34-41 (GATE W-1)
    void cbbWinwayATCIndexChange();           // golden :150-154 (GATE W-1); unread
                                               // `TObject *Sender` dropped
    void OpenCommPort();                      // golden :164-168 (GATE W-2)
    void SetTempratureAll(double _value);     // golden :195-201 (GATE W-2)
    void edtSetTempClick(TObject *Sender);    // golden :235-238, ACTIVE verbatim
};

// AI(W906-FW3-OWM1) 20260828: golden ATC/WinWaySetting.h:66 `extern PACKAGE
// TfWinway *fWinway;` -- PACKAGE glue dropped. Class name and global name
// were BOTH free tree-wide (wave_preflight, 20260828), so this facade
// declares the global -- same idiom as forms/fCleaning.cpp:56 /
// forms/fMesSystem.cpp:597 / forms/fSmartDiagnostic.cpp:37.
extern TfWinway *fWinway;

#endif // FORMS_FWINWAY_H
