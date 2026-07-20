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
#include "vclcompat/vcl_compat.h"

void MyDBIProcess(AnsiString /*S1*/, AnsiString /*S2*/) {}
void ShowMyMessage(AnsiString /*S1*/, AnsiString /*S2*/, AnsiString /*S3*/, bool /*Ok*/, bool /*bServoOff*/) {}
