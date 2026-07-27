// tests/test_uHGemClass_link_stubs.cpp
//
// AI(W906-SysModWire) 20260720: SEPARATE stub TU, added so test_uHGemClass.cpp
// itself stays byte-for-byte unchanged (design brief requires its git diff to
// be empty -- see MIGRATION_ROADMAP / design brief §9 item 5).
//
// Why this is needed: this wave un-gated 8 uHGemClass.cpp methods (S1F1/S1F2/
// S1F13/S1F14/Process_S1F14/S1F16/S1F18/S2F18) that call real out-of-line
// THGem methods (CheckSFFormatOnlyHead/OffLine/OnLine/GetOnLineMode/
// GetTimeInfo, defined in SECSGEM/uHGemEquipment.cpp). That pulls
// uHGemEquipment.cpp.o into test_uHGemClass's link for the FIRST time (it had
// ZERO THGem dependency before this wave -- see test_uHGemClass.cpp's own
// file-head scope note). uHGemEquipment.cpp.o's own 2-arg MyDBIProcess extern
// (declared uHGemEquipment.cpp:80, NO __fastcall) and, transitively (via the
// `HSys` global THGem::DoConnect/DoProcessSFNoResponse already reference),
// database.cpp.o's ShowMyMessage extern (declared database.cpp:65, NO
// __fastcall) both need a definition somewhere in the final link. Same exact
// stub shape as tests/test_uHGemEquipment.cpp's own (:173/:190) -- copied
// verbatim into this standalone TU rather than into test_uHGemClass.cpp.
//
// AI(W906-uHGemClass-Unlock3) 20260723: BOTH stubs that used to live here
// (2-arg `MyDBIProcess` and the 5-arg `ShowMyMessage` above) are REMOVED,
// this TU now compiles to empty. This wave un-gated S2F16 (uHGemClass.cpp),
// which needs the real `HasICUnderMachine()`/`HasAnyICInMachine()`
// predicates (csystem_predicates.cpp) -- linking those pulls in ht9045_sm
// (see tests/CMakeLists.txt's test_uHGemClass entry). Confirmed by the
// actual link error (not assumed): ht9045_sm drags in, transitively, TWO
// other object files that each supply a REAL, non-static definition of the
// exact same signature this TU used to stub:
//   * aHotPlateSubstrate.cpp:772 `void MyDBIProcess(AnsiString, AnsiString) {}`
//   * canary_support.cpp:82-89   `void ShowMyMessage(AnsiString, AnsiString,
//                                  AnsiString, bool, bool) { ... }`
// FLAG FOR REVIEW (self-correction): an earlier version of this same
// comment (and the matching tests/CMakeLists.txt note) claimed "every OTHER
// ShowMyMessage definition in the tree is `static`" and kept this file's own
// ShowMyMessage stub on that basis -- that claim was WRONG (a partial-grep
// mistake, not verified against the actual link), caught only by actually
// building this target and reading the real duplicate-definition error
// (`canary_support.cpp.obj: multiple definition of ShowMyMessage(...)`).
// Corrected here per this project's own "never claim more than verified"
// mandate. `HSys` (database.cpp) already resolves via the existing ht9045_db
// link member, unaffected by either removal.
#include "vclcompat/vcl_compat.h"
