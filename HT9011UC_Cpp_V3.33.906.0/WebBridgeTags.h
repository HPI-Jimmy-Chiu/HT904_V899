// =============================================================================
//  WebBridgeTags.h -- the ONE place where the machine and the browser meet.
//
//  AI(W906-WebBridge) 20260806.  NOT in golden.
//
//  WHY IT LIVES HERE AND NOT IN WebBridge/
//  WebBridge/ is deliberately free of vclcompat and of every machine header, so
//  the socket layer can be reasoned about on its own (see WebBridge/Sync.h and
//  the ht9045_webbridge target's comment in CMakeLists.txt). This file is the
//  deliberate exception: it includes BOTH worlds, reads machine globals, and
//  stages them into a webbridge::TagSnapshot. Keeping that mixing confined to
//  one translation unit is the entire point -- if this were inside WebBridge/,
//  that layer's independence would be gone.
//
//  THE RULE THIS FILE EXISTS TO ENFORCE
//  A tag is published with a REAL value only when its source is actually
//  loaded. Everything else is published as NULL, never as 0.
//
//  That is not fussiness. The browser renders null as "---" and 0 as "0.00"
//  (web/README.md, "Formats"), so publishing 0 for a heater zone nobody has
//  read is a screen that states a measurement which was never taken. On a
//  machine that runs at 130 C, "0.00" and "---" mean very different things to
//  the operator standing in front of it.
//
//  WHAT IS ACTUALLY LOADED TODAY -- measured 20260806, not assumed
//  (tests/test_wb_datalayer.cpp and the survey behind it):
//
//    LIVE    CUSTOMER_CODE                      957
//            IniConfig.sMachineType             "HT-9046LS"
//            IniConfig.sGPIBMachineID           "GLY320"
//            IniConfig.RMSTesterID              "HT9046"
//            LastSet.*                          from system\lastdata.dat
//                                               (12,358 non-zero bytes)
//            HSys.*                             420 keys from Gerneral.ini
//
//    DEAD    Temperature.*                      nothing writes it
//            IniConfig numeric/bool flags       loaded by cConfiguration.cpp,
//                                               which is untranslated (7,808
//                                               golden lines).  Only the few
//                                               STRING fields ReadLastSetIni
//                                               sets are live.
//            CosFunction (140 flags)            0 of 488 bytes set --
//                                               CustomerFunctionSelect runs but
//                                               every branch tests an IniConfig
//                                               flag that is not loaded yet
//            UN150Read (all temperature PVs)    0 of 71 zones -- no controller
//                                               polling in the port
//
//  So the live tag set is small on purpose. It grows when the sources do, and
//  every addition has to answer "is this source loaded?" with a measurement.
//
//  THREADING
//  PublishHandlerTags() reads machine globals, so it must be called from the UI
//  thread, on the same tick that owns them -- never from the socket thread.
//  That is the contract in web/docs/ARCHITECTURE.md section 5, and it is the
//  reason the snapshot exists at all.
// =============================================================================
#ifndef WEBBRIDGETAGS_H
#define WEBBRIDGETAGS_H

#include "WebBridge/TagSnapshot.h"

#include <string>

namespace ht9045 {

// Fill `snap` with one full generation of tag values and commit it.
// UI THREAD ONLY. Returns the number of tags staged.
std::size_t PublishHandlerTags(webbridge::TagSnapshot& snap);

// How many of the tags this file knows about currently have a loaded source.
// Reported so "coverage" is a measured number rather than an impression.
struct TagCoverage {
    std::size_t total;   // tags this file publishes at all
    std::size_t live;    // ...of which carry a real value right now
};
TagCoverage HandlerTagCoverage();

}  // namespace ht9045

#endif  // WEBBRIDGETAGS_H
