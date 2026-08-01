// =============================================================================
//  asendic_Color.h  --  Color-tray stack-feed SM prototypes + task-int cursors
//
//  Mirrored VERBATIM from: HT9011UC_Code_V3.33.906.0_20260618/asendic_Color.h
//  Translation wave: W906-W7-L1 Wave 1 (asendic_* feed-SM family, Color track)
//  Translator: AI(W906-W7-L1-Color) 20260802
//
//  Every declaration below is golden's, in golden's order, with golden's types --
//  including `void DoAutoColor()` (golden asendic_Color.h:20), which is the
//  return type the acatchtray_shims.h stand-in got WRONG (`bool`).  See the
//  RETIREMENT note in asendic_Color.cpp's banner.
//
//  W6/W7 strategy: each SM owns its own extern cursor int (the iXxxTask family).
//  The .cpp DEFINES iAutoColorReceiveTask (golden :31) and
//  iLoadNewColorTrayToCarTask (golden :32) at file scope AND externs them here --
//  kept faithful to the golden, same shape as the landed asendic_Empty.h.
//
//  NOT DECLARED HERE, DELIBERATELY -- golden does not declare them either, and
//  inventing header surface is how cross-file coupling gets fabricated:
//    * InitLoadNewColorTrayToCarTask / InitColorTrayToFrontTask /
//      DoColorTrayToFront / InitUnLoadNewColorTrayTask / DoUnLoadNewColorToStack
//      -- .cpp-internal, reached only from inside asendic_Color.cpp.
//    * int iColorIdTrayCt (golden :374) -- a non-static file-scope counter with
//      external linkage in golden too; nothing outside the .cpp reads it.
// =============================================================================
//---------------------------------------------------------------------------

#ifndef asendic_ColorH
#define asendic_ColorH
//---------------------------------------------------------------------------

extern int iLoadNewColorTrayToCarTask;
extern int iColorTrayToFrontTask;
extern int iAutoColorReceiveTask;
extern int iAutoColorTask;
extern int iUnLoadNewColorTrayTask;
extern int iColorTrayToRearTask;
extern int iColorTrayReadCIDTask;

extern void InitAutoColorTask();
extern void InitAutoColorReceiveTask();
extern void InitColorTrayReadCIDTask();
extern void InitColorTrayToRearTask();

extern void DoAutoColor();
extern bool DoLoadNewColorTrayToCar();
extern int  DoColorTrayReadCID();
extern void DoAutoColorReceive();
// ForTERAPOWERCheckColorSensor is THE cross-file export of this unit: golden
// asendic_Auto.cpp calls it at :363, :2029 and :2074 (all three re-derived from
// the cp950-decoded golden in this pass) against the definition at golden
// asendic_Color.cpp:1455.  Signature verbatim; iPos indexes SnAutoColorTrayDete[].
extern bool ForTERAPOWERCheckColorSensor(int iPos);
extern bool DoColorTrayToRear();
#endif
