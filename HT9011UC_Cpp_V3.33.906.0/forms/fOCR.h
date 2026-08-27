// =============================================================================
//  forms/fOCR.h  --  non-VCL stand-in for golden's fOCR form pointer
//
//  AI(W906-W7-L1-Wave0) 20260801: NEW FILE.  First home for fOCR (golden OCR.h
//  `class TfOCR : public TForm`, global at golden OCR.h:331 `extern PACKAGE
//  TfOCR *fOCR;`).  Verified absent from the ported tree before writing: the only
//  `fOCR` token anywhere was a prose mention in a csystem.cpp comment (:440), no
//  class and no global -- so there is no collision.
//
//  WHY IT IS NEEDED: golden asendic_Loader.cpp calls
//  fOCR->ChangeLightValue(Channel, Value) at exactly 4 sites -- :1301, :1302,
//  :2379, :2380 (all `(1, 0)` / `(2, 0)`, i.e. both ring-light channels off).
//
//  WHY IT LIVES IN forms/ EVEN THOUGH IT IS NOT A WIDGET: golden's TfOCR is a
//  VCL TForm like every other facade member here, and the pointer spelling the
//  SMs use is `fOCR->`.  ChangeLightValue drives OCR ring-light HARDWARE through
//  that form, so this is a hardware sink with a form-pointer shape, not a W7-U
//  widget: the offline body is a no-op because there is no ring light to dim.
//
//  Facade-wide contract: see forms/fMain.h.
//
//  ---------------------------------------------------------------------------
//  AI(W906-FW3-OCR1) 20260827: WAVE FW3-OCR1 -- first real logic members.
//  Golden source of truth: HT9011UC_Code_V3.33.906.0_20260618/OCR.cpp (2,680
//  lines) + OCR.h (333 lines), cp950, decoded with encoding='cp950', 0 U+FFFD.
//
//  DENOMINATOR: census/span_sanity agree golden OCR.cpp defines 88 function
//  bodies (span_sanity: "census 抓 88 個 / 手動解析 88 個 / 缺 0") -- of which
//  82 are `TfOCR::`-qualified members and 6 are file-scope statics
//  (BackupOCRSimulationData/RestoreOCRSimulationData/
//  PrepareOCRSimulationTrayFromTrayForm/BuildOCRSimulationLogFileName/
//  OCRSimLogClear/OCRSimLogWrite -- counted, re-measured 20260827).
//  coverage_probe's PRE-wave port-side match was 100% noise for this file
//  (STRICT 26 golden-lines / loose 188 golden-lines, the loose set entirely
//  VCL-event-handler-NAME collisions in unrelated forms --
//  spbSaveClick/Timer1Timer/FormShow/FormClose/etc. -- NOT prior OCR.cpp
//  work).  The 2 genuine pre-wave STRICT hits, OCRSimLogClear/OCRSimLogWrite,
//  are already ported -- as free functions in OCRInsp.cpp (RETIREMENT DEBT
//  register entry 4 there), not as TfOCR members -- so they are NOT
//  re-added here.
//
//  THIS WAVE DELIVERS 3 of 82 TfOCR:: methods (3.7%; 3 of 88 OCR.cpp function
//  bodies overall, 3.4%): IsOCRCommandTrigger, CheckOCRWordType, InitInsp.
//  Re-run at wave close (20260827): coverage_probe now reports these 3 PLUS
//  the ctor as STRICT (matches this file), confirming the additions are
//  visible to the tool -- STRICT is now 6 golden-functions / 211 golden-lines
//  (the 4 new + the 2 pre-existing OCRInsp.cpp ones), loose is unchanged at
//  13 golden-functions / 188 golden-lines (still 100% unrelated-file noise),
//  NONE is 69 golden-functions (88 - 6 - 13).
//  Each delivered method is a pure predicate / pure state-reset with NO widget, NO
//  hardware (MOT[]/Cylinder[]/SW[]/TMySucker), NO socket/serial comm, NO file
//  I/O, NO alarm (ShowErrorMessage/ShowMyMessage), and NO global
//  machine/production-state write.  Every one of the other 85 is excluded for
//  at least one of those reasons -- see the wave report for the per-function
//  breakdown; this header does not repeat it inline because 82 of the 85 are
//  *Click / *Change / *MouseDown / *Timer event handlers or comm/file-I/O
//  bodies whose exclusion is mechanical (task rule categories), not a judgment
//  call worth a citation each.
//
//  ⚠ THIS FACADE'S TfOCR IS NOT THE ONLY TfOCR SHAPE IN THE TREE.  OCRInsp.cpp
//  (translated 20260807, W906-OCRInsp) carries a TU-local seam class
//  `W906OCR_TfOCRSeam` (OCRInsp.cpp:293-361) with `#define fOCR W906OCR_fOCR`
//  scoped AFTER its own #include block, so `fOCR->` in that file's 34 call
//  sites resolves to the SEAM, not to this class -- no ODR conflict (different
//  class names), but also NO functional benefit flows from this wave's real
//  bodies into OCRInsp.cpp today.  That file's own banner (its register entry
//  1) explicitly names "the wave that lands golden OCR.cpp/OCR.h" as the one
//  that retires the seam + its #define -- which is what THIS wave is, by
//  golden-file scope.  NOT done here: task rule 1 confines this wave to
//  forms/fOCR.{h,cpp}; OCRInsp.cpp is a different agent's file.  Reported as a
//  hand-off, matching the precedent OCRInsp.cpp itself set for
//  asendic_Loader.cpp's InitOCRFlow/CleanOCRData `#define`s (its register
//  entry 5).  The offline VALUES this wave's real bodies compute happen to
//  agree with the seam's hardcoded stand-ins in all 3 cases (see .cpp), which
//  is a coincidence of both currently reading unloaded IniConfig fields, not a
//  guarantee that survives the seam's retirement.
// =============================================================================
#ifndef FORMS_FOCR_H
#define FORMS_FOCR_H

#include "forms/FormWidgets.h"

// ===========================================================================
//  TfOCR -- non-VCL stub (golden OCR.h)
// ===========================================================================
class TfOCR
{
public:
    // golden OCR.h:235-274 -- OCRComIndex, values reproduced VERBATIM (they
    // index bOcr_ReceiveOK[], so the numbering is load-bearing).  Byte-for-byte
    // the same values as the TU-local seam OCRInsp.cpp:298-328
    // (W906OCR_TfOCRSeam::OCRComIndex) -- a DIFFERENT class, so no redefinition;
    // kept identical on purpose so a future integration diff is trivial.
    enum OCRComIndex{ocrVisionOn        =0,
                     ocrVisionOff       =1,
                     ocrStartOk         =2,
                     ocrStartNoReady    =3,

                     ocrRecipeOk        =4,
                     ocrRecipeNoModel   =5,
                     ocrRecipeNoROI     =6,
                     ocrRecipeNoFont    =7,
                     ocrRecipeNoLotID   =8,

                     ocrEnd             =9,
                     ocrFileOk          =10,
                     ocrClear           =11,

                     ocrInspOk          =12,
                     ocrInspNg          =13,
                     ocrInspErr2001     =14,
                     ocrInspErr2002     =15,
                     ocrInspErr2003     =16,
                     ocrInspErr2004     =17,
                     ocrInspErr3001     =18,
                     ocrInspErr3002     =19,

                     ocrTrainOK         =20,
                     ocrTrainNG         =21,

                     ocrLot             =22,

                     OCR_MAX_CMD                 // golden: 這個一定得在最下面,不可以動
                    };

    // golden OCR.h:279 (public DATA member -- the [METHOD]s below read/write
    // it).  Only the INSPECTION subset of indices is ever touched by the
    // methods this wave delivers (InitInsp); the SEND/RECV protocol indices
    // (ocrVisionOn..ocrLot) exist here only so the enum values line up with
    // golden and with the OCRInsp.cpp seam -- no delivered method writes them.
    bool bOcr_ReceiveOK[OCR_MAX_CMD];

    TfOCR();                                                 // ctor: golden OCR.cpp:72-209 -- PORT SPLIT, see .cpp banner
    virtual void ChangeLightValue(int Channel, int Value);    // [METHOD] golden OCR.h:324 -- offline: no-op
    bool IsOCRCommandTrigger();                                // [METHOD] golden OCR.h:310
    bool CheckOCRWordType(AnsiString asOCR);                   // [METHOD] golden OCR.h:323
    void InitInsp();                                           // [METHOD] golden OCR.h:291
    virtual ~TfOCR() {}
};
extern TfOCR *fOCR;     // golden: extern PACKAGE TfOCR *fOCR; (OCR.h:331)

#endif // FORMS_FOCR_H
