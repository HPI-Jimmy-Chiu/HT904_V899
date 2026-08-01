// =============================================================================
//  asendic_Auto.h  --  Auto-tray (automatic sort-output) stack-feed SM
//                      prototypes + per-Auto task-cursor arrays
//
//  Mirrored from: HT9011UC_Code_V3.33.906.0_20260618/asendic_Auto.h  (23 lines,
//                 re-counted with `wc -l` on the cp950 decode in this pass)
//  Translation wave: W906-W7-L1 Wave 2 (Auto track)
//  Translator: AI(W906-W7-L1-Auto) 20260802
//
//  EVERY declaration below is golden's, in golden's order, with golden's types.
//  In particular `void DoAutoReceiveBinTray(int Pos)` is VOID (golden
//  asendic_Auto.h:8) -- which is the return type the acatchtray_shims.h stand-in
//  got WRONG (`bool`).  See the RETIREMENT block in asendic_Auto.cpp's banner;
//  the shim BODY is retired and the shim DECLARATION is corrected to void, the
//  same precedent asendic_Color.cpp set for DoAutoColor.
//
//  W6/W7 strategy: each SM owns its own cursor int (the iXxxTask family).  The
//  .cpp DEFINES BinTrayTask / iAutoTrackDetectICFloatingTask /
//  iAuto123TrayToRearTask at file scope and this header externs them, faithful
//  to the golden split (golden asendic_Auto.cpp:52, :2260, :2290).
//
//  ONE GOLDEN DECLARATION THAT IS *NOT* THIS FILE'S SYMBOL
//  -------------------------------------------------------
//  golden asendic_Auto.h:7 `extern void InitUnLoadNewAutoTrayTask(int pos);` is
//  mirrored below for header fidelity, but the DEFINITION lives in
//  asendic_Auto_RT.cpp (golden asendic_Auto_RT.cpp:672) -- the concurrent Wave-2
//  track -- and the ported tree ALREADY declares it once, at asendic_Auto2.h:49,
//  where the banner records the same ownership.  The two declarations are
//  signature-identical (`void (int)`), so seeing both in one TU is legal C++ and
//  neither has to be removed; this note exists so the duplication is deliberate
//  rather than discovered.
//
//  NOT DECLARED HERE, DELIBERATELY -- golden does not declare them either:
//    * int iDoAutoCassetteTrayFeed[2] (golden asendic_Auto.cpp:2481) -- a
//      file-scope cursor with external linkage in golden too; nothing outside
//      the .cpp reads it, and golden exports only the Init/Do pair.
//    * every TQPF_Timer at golden :50, :64-70, :2291-2292, :2299, :2482 and the
//      ULDStackDelay/DetectAutoTime/... family -- all .cpp-internal.
//
//  DoAutoReceiveBinTray IS THIS UNIT'S CROSS-FILE EXPORT: golden
//  asendic_Auto_RT.cpp:483 calls it (re-derived from the cp950 decode in this
//  pass), and so do csystem.cpp / acatchtray.cpp in this tree, through
//  acatchtray_shims.h.  Signature verbatim.
// =============================================================================
//---------------------------------------------------------------------------
#ifndef asendic_AutoH
#define asendic_AutoH
//---------------------------------------------------------------------------

#include "MachineType.h"        // MAX_AUTO_TRAY (golden pulls it via MachineDefine.h)

extern void Initial_Auto_BinTray_Task(int index);                               // golden asendic_Auto.h:6  (body asendic_Auto.cpp:58)
extern void InitUnLoadNewAutoTrayTask(int pos);                                 // golden asendic_Auto.h:7  -- body in asendic_Auto_RT.cpp:672, see banner
extern void DoAutoReceiveBinTray(int Pos);                                      // golden asendic_Auto.h:8  (body asendic_Auto.cpp:71)
extern int BinTrayTask[MAX_AUTO_TRAY];                                          // golden asendic_Auto.h:9  (defined asendic_Auto.cpp:52)
extern bool bAutoIsFullNeedStop[MAX_AUTO_TRAY];                                 //JerryYang 20160810 Unloader滿tray是否需要馬上停機   (golden asendic_Auto.h:10)

extern bool DoAutoTrackDetectICFloating(int Pos);                               //Isaac 20180109 (Steven) : auto123可前進後退        (golden asendic_Auto.h:12)
extern int iAutoTrackDetectICFloatingTask[MAX_AUTO_TRAY];                       // golden asendic_Auto.h:13 (defined asendic_Auto.cpp:2260)
extern int iAuto123TrayToRearTask[MAX_AUTO_TRAY];                               // golden asendic_Auto.h:14 (defined asendic_Auto.cpp:2290)
extern void InitAuto123TrayToRearTask(int Pos);                                 // golden asendic_Auto.h:15
extern void InitAutoTrackDetectICFloatingTask(int Pos);                         // golden asendic_Auto.h:16
extern bool DoAuto123TrayToRear(int Pos);                                       // golden asendic_Auto.h:17
extern void CheckHasErrorBinOnTray(int Pos);                                    //Sam 20240108 : 新增退 Tray 時顯示裡面有多少 Error Bin  (golden asendic_Auto.h:18)

extern void InitAutoCassetteTrayFeedTask(int iPos);                             // golden asendic_Auto.h:20
extern bool DoAutoCassetteTrayFeed(int iPos);                                   //Frank 20251217 add                                   (golden asendic_Auto.h:21)
#endif
