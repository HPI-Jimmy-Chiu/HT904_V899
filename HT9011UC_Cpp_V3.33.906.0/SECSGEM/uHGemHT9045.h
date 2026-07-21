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

class HT9045Gem : public HTGem
{
public:
    HT9045Gem(AnsiString Path, THGem *HGemTmp);   // golden uHGemHT9045.h:344 (no __fastcall)
};
#endif
