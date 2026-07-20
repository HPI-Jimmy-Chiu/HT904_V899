//---------------------------------------------------------------------------
//  SECSGEM/uHGemHT9045_Shim.h -- THIN SHIM for golden uHGemHT9045.h:337-370
//
//  Translation wave: W906-SysModWire (SystemModularInitial wiring)
//  Translator: AI(W906-SysModWire) 20260720
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/SECSGEM/uHGemHT9045.h
//                 (class HT9045Gem declaration, ~9200-line site-override
//                 layer combined with uHGemHT9045_SV.cpp/_EC.cpp)
//
//  WHY THIS FILE EXISTS
//  ---------------------
//  golden database.cpp:1541's `MyGem=new HT9045Gem("HT9045", HGem);` needs a
//  concrete `HT9045Gem` type to compile. The REAL golden HT9045Gem is a
//  ~9200-line site-specific override layer (uHGemHT9045.h/.cpp +
//  uHGemHT9045_SV.cpp/_EC.cpp) that is explicitly ROADMAP-DEFERRED --
//  blocked on ~22 untranslated forms (see MIGRATION_ROADMAP.md) -- and is
//  NOT part of this wave's scope (see the design brief's Decision A). This
//  shim exists SOLELY so database.cpp's SystemModularInitial translates
//  VERBATIM today, without occupying the real uHGemHT9045.h/.cpp mirror path
//  (SECSGEM/uHGemHT9045.h/.cpp are reserved, untouched, for the future real
//  translation wave).
//
//  BEHAVIOR: NO VIRTUAL OVERRIDES -- this shim's HT9045Gem is byte-for-byte
//  HTGem's own base behavior for every one of the ~20 virtual S,F handlers
//  golden's real HT9045Gem overrides. Behavior-difference table (golden
//  production, HT9045Gem override active, vs. this port with the shim wired
//  -- ProcessReceiceData tail branch : golden production behavior : this
//  port's behavior : note):
//    S9F1/S9F3/S9F5/S9F7/S9F9            : HTGem base : SAME as golden      : no override exists in golden either
//    S2F25->S2F26 loopback               : HTGem base : SAME as golden      : no override exists in golden either (W9 oracle)
//    S2F23->S2F24 (Sub gated->deny)      : HTGem base : SAME as golden      : Sub still gated regardless of shim
//    S1F1->S1F2                          : HTGem base (not overridden)      : SAME as golden      : real reply (MDLN/SOFTREV)
//    S1F13->S1F14                        : HTGem base (not overridden)      : SAME as golden      : real handshake
//    S1F17->S1F18                        : HTGem base (not overridden)      : SAME as golden      :
//    S1F14->Process_S1F14                : HTGem base (not overridden)      : SAME as golden      : DoConnect e2e key path
//    S2F15->S2F16                        : HT9045Gem override (real check)  : HTGem base gated no-op (no reply) : missing SReceiveDataBackup + csystem HasICUnderMachine/HasAnyICInMachine -- deliberately out of scope, see uHGemClass.cpp's own gate comment
//    S2F41->S2F42                        : n/a (not in tail; golden too)    : n/a                  : S2F42 only reachable via direct test call; base = generic HCACK, no site RCMD specialization
//    S5F5->S5F6                          : HT9045Gem override (real alarms) : HTGem base gated no-op (no reply) : SECS alarm family deferred to a future wave
//    S7F1->S7F2                          : HT9045Gem override (site checks) : HTGem base ACTIVE (generic grant/deny) : replies, but with no site-specific validation
//    S7F3/S7F5/S7F19/S7F20/S10F3/S10F5/S14F3/S100F3/S101F1/S101F3/S101F5/S101F7/S103F11/S110F5..8/S120F2/S125F1/S125F3 : various HT9045Gem overrides : HTGem base gated no-op or inline `{}` : no reply, no crash; each gate cites its own specific blocker
//    flag latches S1F2/S7F6/S101F6/S101F8/S110F2 : sets the flag           : SAME as golden (members added this wave) : downstream consumers (DoUploadFileToHost*, etc.) remain gated stubs
//    MoveCheckCallBack short-circuit     : real GemCallBack (TFSECS-installed) : NULL offline (no installer translated) : does not short-circuit; tests can inject one
//    CUSTOMER_CODE==CC_TFME_CHINA gate    : same customer-code branch        : SAME as golden      : W8 verifies both GemControlState<=1 and >1
//  SWAP-BACK PATH (when the real uHGemHT9045 wave lands):
//    1. DELETE both uHGemHT9045_Shim.{h,cpp} (ODR -- HT9045Gem's symbol
//       must have exactly one definition).
//    2. Add the real SECSGEM/uHGemHT9045.{h,cpp} (+ _SV.cpp/_EC.cpp) to the
//       root CMakeLists.txt ht9045_secsgem source list.
//    3. Change database.cpp's include from this header to
//       "SECSGEM/uHGemHT9045.h" -- one line.
//---------------------------------------------------------------------------
#ifndef uHGemHT9045_ShimH
#define uHGemHT9045_ShimH
#include "SECSGEM/uHGemClass.h"   // HTGem base

class THGem;   // forward-decl (ctor parameter uses an incomplete type -- legal)

// AI(W906-SysModWire) 20260720: repeats uHGemEquipment.h:843's `extern THGem
// *HGem;` declaration (same object, same linkage -- a duplicate `extern` of
// the identical declaration is legal C++). This lets database.cpp pull in
// just this header to get both the HT9045Gem type AND the HGem symbol,
// without dragging in uHGemEquipment.h's much larger surface at the include
// site (database.cpp's own .cpp still includes this header only, per
// definitive decision A).
extern THGem *HGem;

class HT9045Gem : public HTGem
{
public:
    HT9045Gem(AnsiString Path, THGem *HGemTmp);   // golden uHGemHT9045.h:344 (no __fastcall)
};
#endif
