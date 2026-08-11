//---------------------------------------------------------------------------
//  SECSGEM/uHGemHT9045.h -- permanent home for the HT9045Gem container class
//
//  Translation wave: W906-uHGemHT9045-Bucket0 (skeleton swap-back)
//  Translator: AI(W906-uHGemHT9045-Bucket0) 20260721
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/SECSGEM/uHGemHT9045.h
//                 (class HT9045Gem declaration, golden :337-370; that file
//                 also carries a ~9200-line site-override layer combined
//                 with uHGemHT9045_SV.cpp/_EC.cpp -- NOT this file's scope,
//                 see below).
//
//  THIS FILE'S SCOPE (Bucket 0 -- skeleton only, pure rename/relocate)
//  ---------------------------------------------------------------------
//  Mirrors golden's own relative path (SECSGEM/uHGemHT9045.h). This is now
//  the PERMANENT home for the `HT9045Gem` container class, superseding the
//  temporary SECSGEM/uHGemHT9045_Shim.h/.cpp pair (W906-SysModWire 20260720,
//  deleted this wave -- see docs/DEVLOG.md for the swap-back record).
//
//  Declares `class HT9045Gem : public HTGem` with ONE constructor and,
//  deliberately, ZERO of golden's 22 virtual override declarations (golden
//  :346-368: AddSV/AddEC/AddAlarmList/AddCEID/AddReprot/
//  S2F15_CheckNewEquipmentConstant/S2F15_UpdateNewEquipmentConstant/
//  S2F42_Host_Command_Acknowledge/S5F6_ListAlarmData/
//  S7F2_ProcessProgramLoadGrant/S7F4_ProcessProgramAcknowledge/
//  S7F6_ProcessProgramData (x2 overloads)/ProcessS7F23FromatReceipe/
//  S7F24_FormattedProcessProgramSendAcknowledge/ProcessS7F25FromatReceipe/
//  S7F26_FormattedProcessProgramData/S14F4_Get2DID_BinCode/
//  S110F5_RequestCustomerNameList/S125F4_LevelSettingChangeAcknowledge/
//  ReloadParameter/LookForFile/CheckAndExecuteAsyncRCMD) -- not yet
//  translated; see docs/MIGRATION_ROADMAP.md's DEFERRED table entry for
//  SECSGEM/uHGemHT9045.{h,cpp}+_SV.cpp+_EC.cpp (W5-SECSGEM-override, gated
//  on ~22 untranslated site forms) for the per-method translation queue.
//  Every S,F handler HT9045Gem would override in golden instead runs
//  HTGem's own base behavior here -- byte-for-byte identical to the shim
//  this file replaces (see docs/DEVLOG.md's W906-SysModWire entry for the
//  full behavior-difference table; nothing about that table changes this
//  wave, it is a pure container relocate).
//
//  DATA MEMBERS -- deliberately NOT carried over this wave: golden declares
//  two currently-inert members alongside the 22 overrides (`TAsyncRCMDState
//  m_AsyncRCMD;`, golden :340; `AnsiString EventDescription[SECS_EVENT.
//  TotalEvent];`, golden :342). No in-scope method reads or writes either
//  (the ctor that seeds EventDescription[] is itself future-bucket work --
//  see uHGemHT9045.cpp's own file-head note), and the shim this file
//  replaces never had them, so adding either now would be scope creep for
//  a "skeleton only" swap-back. Left as-is for whichever future bucket
//  first needs them (W5-SECSGEM-override).
//---------------------------------------------------------------------------
#ifndef uHGemHT9045H
#define uHGemHT9045H
#include "SECSGEM/uHGemClass.h"   // HTGem base

class THGem;   // forward-decl (ctor parameter uses an incomplete type -- legal)

// AI(W906-uHGemHT9045-Bucket0) 20260721: repeats uHGemEquipment.h:843's
// `extern THGem *HGem;` declaration (same object, same linkage -- a
// duplicate `extern` of the identical declaration is legal C++). Carried
// over verbatim from the deleted uHGemHT9045_Shim.h so database.cpp keeps
// pulling both the HT9045Gem type AND the HGem symbol from this one include.
extern THGem *HGem;

// AI(W906-PT-W5-registration) 20260809: SECS_EVENT is the array bound of
// golden's own `EventDescription[]` member re-added below (golden
// uHGemHT9045.h:342). In GOLDEN, `struct ETypeStruct SECS_EVENT` is declared
// in THIS very header; this port extracted it into a standalone unit
// (SECSGEM/SecsEventType.{h,cpp}), so the include is a PORT-ONLY line that
// restores a name golden already had in scope here -- not a new dependency.
#include "SECSGEM/SecsEventType.h"

// AI(W906-PT-W5-S2F15-S5-S14) 20260809: golden uHGemHT9045.h:328-335, VERBATIM
// (comment text transcribed from the cp950 golden, not retyped). Re-added
// because golden's own CheckAndExecuteAsyncRCMD (golden uHGemHT9045.cpp:1111,
// declared at golden :369, body appended by THIS group) reads
// m_AsyncRCMD.bPending / .sCommand / .sParam -- so the type and the member are
// declarations THIS group's own body needs, not the general "carry golden's
// data members over" work this file's head note defers. The OTHER writer of
// m_AsyncRCMD in golden is S2F42_Host_Command_Acknowledge (golden :1146-4189,
// which sets bPending/sCommand/sParam on the HCACK=4 path) -- NOT translated by
// this group; until it is, bPending is never set true, so
// CheckAndExecuteAsyncRCMD returns on its first line and the async path is
// dormant rather than wrong. Zero static-init exposure: the ctor is a member
// initialiser list over one bool + two AnsiStrings, touches no global, and
// HT9045Gem is only ever heap-built (database.cpp:187).
// 非同步 RCMD 流程 (供 HCACK=4 非同步執行使用)
struct TAsyncRCMDState
{
    bool       bPending;       // 是否有待執行的非同步指令
    AnsiString sCommand;       // 指令名稱 (大寫)
    AnsiString sParam;         // 主要參數 (例如 PPID)
    TAsyncRCMDState() : bPending(false), sCommand(""), sParam("") {}
};

class HT9045Gem : public HTGem
{
    // AI(W906-PT-W5-S2F15-S5-S14) 20260809: golden uHGemHT9045.h:339-340,
    // verbatim (access specifier + member), at golden's own relative position
    // -- golden puts this private section FIRST in the class, ahead of the
    // protected EventDescription[] below. See the struct comment above for why
    // this group needs it.
    private:
        TAsyncRCMDState m_AsyncRCMD;  //Ifor 20260423 add:非同步 RCMD 狀態追蹤
    // AI(W906-PT-W5-registration) 20260809: golden uHGemHT9045.h:341-342,
    // verbatim (access specifier + member). Re-added because AddCEID (golden
    // uHGemHT9045.cpp:445) passes `EventDescription[i]` as SetCEIDContent's
    // CeidAlias, so it is a declaration THIS group's own body needs -- it is
    // NOT the general "carry golden's data members over" work this file's head
    // note defers. golden's OTHER deferred member (`TAsyncRCMDState
    // m_AsyncRCMD;`, golden :340) is still NOT added: no method in this group
    // touches it. NOTE THE SEEDING GAP, which is a real behaviour delta and is
    // documented in full in uHGemHT9045.cpp's own PT-W5 append-block banner:
    // golden's ctor fills all ~292 entries (golden uHGemHT9045.cpp:71-368),
    // this port's ctor does not, so every entry is "" until some future bucket
    // translates that seeding. HT9045Gem is only ever heap-constructed from
    // SYSTEM_MODULAR::SystemModularInitial (database.cpp:187), never during
    // static init (database.cpp:99's `SYSTEM_MODULAR HSys;` gets the implicit
    // default ctor -- golden's real ctor at database.cpp:138 is inside that
    // file's `#if 0 // TODO(wave)` region), so these 292 AnsiString ctors
    // cannot run before main() and touch no global.
    protected:
        AnsiString EventDescription[SECS_EVENT.TotalEvent];
public:
    HT9045Gem(AnsiString Path, THGem *HGemTmp);   // golden uHGemHT9045.h:344 (no __fastcall)
    // AI(W906-PT-W5-registration) 20260809: the "registration" group's five
    // overrides, in golden's OWN wording and golden's OWN relative order.
    // Bodies are appended at the end of SECSGEM/uHGemHT9045.cpp (see that
    // file's PT-W5 banner + GATE REGISTER). golden's `virtual void AddSV();`
    // and `virtual void AddEC();` belong BETWEEN the ctor above and
    // AddAlarmList below (golden uHGemHT9045.h:346-347) -- deliberately NOT
    // added by this wave: reconnecting SECSGEM/uHGemHT9045_{SV,EC}.cpp is the
    // main loop's serialized job, and those two files currently emit
    // free-function heads (HT9045Gem_AddSV/HT9045Gem_AddEC) precisely because
    // this header lacks the declarations.
    // AI(pt-wave) 20260811 PT-W9: the two declarations the block above calls
    //   "the main loop's serialized job", added at golden's own position
    //   (golden uHGemHT9045.h:346-347, between the ctor and AddAlarmList).
    //   Until now SECSGEM/uHGemHT9045_SV.cpp and _EC.cpp emitted their bodies
    //   under free-function heads (HT9045Gem_AddSV / HT9045Gem_AddEC) *because*
    //   this header lacked these two lines, so PT-W4's delivered SV/EC
    //   dictionaries were unreachable as overrides -- a host S1F3/S1F11 saw
    //   HTGem's empty base virtuals instead.  Golden spells both
    //   `virtual void __fastcall`; this tree drops __fastcall everywhere, same
    //   as the ctor above and the five overrides below.
    virtual void AddSV()                    ;   // golden uHGemHT9045.h:346
    virtual void AddEC()                    ;   // golden uHGemHT9045.h:347
    virtual void AddAlarmList()             ;   // golden uHGemHT9045.h:348
    virtual void AddCEID()                  ;   // golden uHGemHT9045.h:349
    virtual void AddReprot()                ;   // golden uHGemHT9045.h:350 (golden's own misspelling of "Report" -- KEPT)
    // golden uHGemHT9045.h:351-365 declare 15 more S,F overrides between
    // AddReprot and ReloadParameter; none of them is this group's scope, so
    // none is declared here.
    // AI(W906-PT-W5-S2F15-S5-S14) 20260809: the "S2F15-S5-S14" group's SIX S,F
    // overrides, in golden's OWN wording and golden's OWN relative order --
    // golden :351/:352/:354 land HERE (ahead of the S7 family below) and
    // golden :363/:364/:365 land after it, so this declaration set is split in
    // two around the sibling S7 block on purpose, not by accident. Bodies are
    // appended at the end of SECSGEM/uHGemHT9045.cpp; see that file's
    // W906-PT-W5-S2F15-S5-S14 banner + GATE REGISTER. The comment three lines
    // above ("none is declared here") was written by the registration sibling
    // and was true FOR ITS OWN GROUP; it predates this append and is left
    // untouched rather than edited.
    // DELIBERATELY NOT DECLARED: golden :353
    // `virtual int S2F42_Host_Command_Acknowledge();` -- golden's body
    // (:1146-4189, ~3000 lines) is not this group's scope. CONSEQUENCE, and it
    // matters for CheckAndExecuteAsyncRCMD below: S2F42 is the ONLY writer of
    // m_AsyncRCMD in golden, so until it is translated bPending stays false and
    // the async path below is dormant. HTGem's own S2F42 (uHGemClass.cpp:2419)
    // keeps handling the message meanwhile.
    virtual int  S2F15_CheckNewEquipmentConstant(); //wei 20170417 (Steven) add S2F15    // golden uHGemHT9045.h:351
    virtual int  S2F15_UpdateNewEquipmentConstant();                                     // golden uHGemHT9045.h:352
    // AI(W906-PT-W5-S2F42) 20260809: golden uHGemHT9045.h:353, at golden's own
    // relative position (between :352 above and :354 below).  The sibling
    // "DELIBERATELY NOT DECLARED" note 4 lines up was written before this
    // append and is left untouched rather than edited, but it is now STALE in
    // both of its claims: this group HAS translated the body (appended at the
    // end of SECSGEM/uHGemHT9045.cpp -- see that file's W906-PT-W5-S2F42
    // banner + its 48-entry GATE REGISTER), and m_AsyncRCMD IS now written --
    // by the SET_RECIPE arm's HCACK=4 path (golden :3979-3981), which this
    // append carries ACTIVE because the sibling had already added the member
    // at :101.  So CheckAndExecuteAsyncRCMD's async path is live from here on:
    // a SET_RECIPE arriving while the machine runs now really does queue.
    virtual int  S2F42_Host_Command_Acknowledge();                                        // golden uHGemHT9045.h:353
    virtual void S5F6_ListAlarmData();                                                   // golden uHGemHT9045.h:354
    // AI(W906-uHGemHT9045-S7program) 20260809: the "S7-program" (recipe)
    // group's seven overrides, in golden's OWN wording and golden's OWN
    // relative order (golden uHGemHT9045.h:355-362). Bodies are appended at
    // the end of SECSGEM/uHGemHT9045.cpp -- see that file's
    // W906-uHGemHT9045-S7program banner + GATE REGISTER for which are ACTIVE
    // and which are gated.
    // DELIBERATELY NOT DECLARED: golden uHGemHT9045.h:358
    // `virtual void S7F6_ProcessProgramData(AnsiString FileName);` (golden
    // :5605-5825, the S7F6 work-file UPLOAD overload) -- that body is not this
    // group's scope, and declaring it without a body would be a link error.
    // CONSEQUENCE, recorded: declaring only the no-arg form HIDES HTGem's
    // 1-arg overload (uHGemClass.h:279) for name lookup through an HT9045Gem
    // (not through an HTGem*/THGem*, where virtual dispatch is unaffected).
    // Verified no call site is affected: the only two 1-arg call sites in the
    // tree are tests/test_uHGemClass.cpp:182, which calls it on an `HTGem g;`,
    // and none exists on an HT9045Gem (grep "S7F6_ProcessProgramData" over
    // *.cpp/*.h, 20260809 16:25).
    virtual int  S7F2_ProcessProgramLoadGrant();                    // golden uHGemHT9045.h:355
    virtual void S7F4_ProcessProgramAcknowledge();                  // golden uHGemHT9045.h:356
    virtual void S7F6_ProcessProgramData();                         // golden uHGemHT9045.h:357
    virtual int  ProcessS7F23FromatReceipe() ;                      // golden uHGemHT9045.h:359 (golden's own misspelling of "Format" -- KEPT)
    virtual int  S7F24_FormattedProcessProgramSendAcknowledge();    // golden uHGemHT9045.h:360
    virtual int  ProcessS7F25FromatReceipe() ;                      // golden uHGemHT9045.h:361 (golden's own misspelling of "Format" -- KEPT)
    virtual int  S7F26_FormattedProcessProgramData();               // golden uHGemHT9045.h:362
    // AI(W906-PT-W5-S2F15-S5-S14) 20260809: second half of this group's
    // declarations -- golden :363/:364/:365, at golden's own relative position
    // (immediately after the S7 family, immediately before ReloadParameter).
    virtual void S14F4_Get2DID_BinCode();                                                // golden uHGemHT9045.h:363
    virtual void S110F5_RequestCustomerNameList();                                       // golden uHGemHT9045.h:364
    virtual void S125F4_LevelSettingChangeAcknowledge();    //Steven 20150605 : S125F3 LevelSettingChangeRequest   // golden uHGemHT9045.h:365
    virtual void ReloadParameter();             // golden uHGemHT9045.h:367
    virtual void LookForFile()              ;   // golden uHGemHT9045.h:368
    // AI(W906-PT-W5-S2F15-S5-S14) 20260809: golden uHGemHT9045.h:369, verbatim
    // (including golden's own trailing comment). NOT virtual in golden -- kept
    // non-virtual. Body appended at the end of SECSGEM/uHGemHT9045.cpp
    // (gates [A1]/[A2] there).
    void CheckAndExecuteAsyncRCMD();// 非同步 RCMD 執行函式 (HCACK=4 後暫存，機台進入 Idle 時觸發)
};
#endif
