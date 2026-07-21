//---------------------------------------------------------------------------
//  SECSGEM/uHGemHT9045.cpp -- permanent home for HT9045Gem::HT9045Gem ctor
//
//  Translation wave: W906-uHGemHT9045-Bucket0 (skeleton swap-back)
//  Translator: AI(W906-uHGemHT9045-Bucket0) 20260721
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/SECSGEM/uHGemHT9045.cpp
//                 (ctor's first two lines only -- see uHGemHT9045.h's own
//                 file-head note for why the rest of this ~9200-line file
//                 remains out of scope for this wave).
//
//  This ctor body is copied VERBATIM from the now-deleted
//  uHGemHT9045_Shim.cpp (W906-SysModWire 20260720), NOT reconstructed from
//  golden -- golden's own ctor (uHGemHT9045.cpp:66-69) lacks the ActiveWire
//  rebind below (a deliberate port-specific addition, see uHGemClass.h:
//  139-151) and instead seeds a ~300-line EventDescription[] string table
//  (golden :71-368) this class does not carry members for (see uHGemHT9045.h's
//  own "DATA MEMBERS" note -- that population remains future-bucket work,
//  not this wave's, regardless of that data-member decision).
//
//  golden ctor body (uHGemHT9045.cpp:66-69), for reference only -- NOT what
//  is implemented below:
//    HT9045Gem::HT9045Gem(AnsiString Path, THGem *HGemTmp)   // Tmp
//    {
//        HGemPtr=HGemTmp;
//        HandlerPath=Path;
//        ... ~300 lines of EventDescription[...] string-table seeding ...
//    }
//---------------------------------------------------------------------------
#include "SECSGEM/uHGemHT9045.h"
#include "SECSGEM/uHGemEquipment.h"   // THGem complete type (needed for HGemTmp->WireCodec)

HT9045Gem::HT9045Gem(AnsiString Path, THGem *HGemTmp)
{
    HGemPtr=HGemTmp;
    HandlerPath=Path;
    // AI(W906-SysModWire) 20260720: PORT-ONLY -- bind the receive/send codec
    // to the live THGem engine when one is supplied (golden's single-codec
    // reality; golden re-points HGemPtr the same way in AddSV/AddEC,
    // uHGemHT9045_SV.cpp:61 / _EC.cpp:54, once the real override layer
    // lands). NULL-tolerant: golden's own static-init call site passes a
    // still-NULL HGem (SYSTEM_MODULAR is a global -> its ctor runs during
    // static init, before THGem HGem is ever assigned -- see design brief
    // §1.1) -- ProcessReceiceData's dispatch entry re-binds ActiveWire
    // defensively on every dispatch (see that method's own comment) so this
    // is not the only re-bind point, just the earliest opportunistic one.
    if (HGemTmp != NULL)
        ActiveWire = &HGemTmp->WireCodec;
}
