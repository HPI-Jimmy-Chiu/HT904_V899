// =============================================================================
//  forms/fProductionInfo.h  --  non-VCL stand-in for golden's fProductionInfo
//                               form pointer
//
//  AI(W906-W7-L1-Wave0) 20260801: NEW FILE.  First real home for fProductionInfo
//  (golden ProductionInfo/ProductionInfo.h:23 `class TfProductionInfo : public
//  TForm`, global at :572 `extern PACKAGE TfProductionInfo *fProductionInfo;`).
//
//  IT MERGES TWO INDEPENDENT REQUESTS FOR ONE GOLDEN OBJECT: the Loader recon
//  asked for a new forms/ file carrying sLoadMO_TestFlow while the Auto recon
//  independently asked for a TU-LOCAL no-op macro for CalTrayICCount -- two
//  mechanisms for the same object.  One file, both members.
//
//  WHERE THEY ARE DEREFERENCED IN GOLDEN (re-derived this pass):
//    * sLoadMO_TestFlow -- asendic_Loader.cpp:1880 (`.SubString(1,2)!="RT"`).
//    * CalTrayICCount   -- asendic_Auto.cpp:405, :455, :1325, :1405, :2114,
//      :2363.  Several of those are MAIN-PATH, so the method is mandatory, not
//      optional.
//
//  PRE-EXISTING STATE THIS FILE DOES *NOT* TOUCH, RECORDED SO IT IS NOT MISSED.
//  Before this file existed, the symbol `fProductionInfo` had NO definition
//  anywhere in the ported tree, yet two out-arm variants declare their own
//  differently-typed extern for it and dereference it on live code paths:
//    * aoutarm9045_1x4_4_Back.cpp:180-181 (`TfProductionInfo_1x4_4_Back_stub`),
//      used at :2594;
//    * aoutarm9045_2x2_4_14.cpp:181-182   (`TfProductionInfo_2x2_4_14_stub`),
//      used at :2618.
//  Those objects simply are not pulled out of libht9045_sm.a by any current test
//  executable, which is why the missing definition has never surfaced as an
//  undefined reference.  Defining the global here can only IMPROVE that (an
//  undefined symbol becomes defined); it cannot duplicate anything.  Their
//  declarations still name a different TYPE for the same symbol, which is a
//  pre-existing ODR problem in files owned by W7-A2 -- repointing them is A2's
//  call, not this pass's, and their stub CalTrayICCount bodies are inline no-ops
//  that never touch the object, so nothing breaks meanwhile.
//
//  ALSO NOT ADDED: ClickPause() (golden ProductionInfo.h:464).  forms/fMain.h's
//  BtnPauseClick comment records golden's `#ifndef SOFT_SIMULTE
//  fProductionInfo->ClickPause()` second line as an untranslated gap.  That gap
//  belongs to whichever wave translates BtnPauseClick's second line; adding the
//  method here without a caller would be inventing surface.
//
//  Facade-wide contract: see forms/fMain.h.
// =============================================================================
#ifndef FORMS_FPRODUCTIONINFO_H
#define FORMS_FPRODUCTIONINFO_H

#include "forms/FormWidgets.h"
// AI(W906-FW3-PI1) 20260827: 2 includes added for the FW3-PI1 read-only batch
// (see the member/method block below).  PMAlarm/uTimeTool.h -- the
// `utimetool` member's type (golden ProductionInfo.h:542), already a landed
// port (PMAlarm/uTimeTool.cpp, registered in ht9045_globals -- see the
// LINK-BOUNDARY note below for why that target matters).  <vector> --
// GetBin0_8List()'s golden return type (golden :569 `std::vector<int>`).
// MachineType.h (e3Auto1.../e3Fix5 for GetBin0_8List's body) is included from
// the .cpp instead -- nothing in this header needs it after the link-boundary
// trim below removed the MAX_SOCKET_ROW/MAX_SOCKET_COL/TEST_MAX_BIN-sized
// array fields.
#include "PMAlarm/uTimeTool.h"
#include <vector>

// ===========================================================================
//  TfProductionInfo -- non-VCL stub (golden ProductionInfo/ProductionInfo.h)
// ===========================================================================
class TfProductionInfo
{
public:
    // Concrete AnsiString storage (facade contract rule 2).  Offline "" means
    // golden asendic_Loader.cpp:1880's `SubString(1,2)!="RT"` is TRUE, i.e. the
    // handler is treated as not running an RT-mode MO -- the faithful "no MO
    // downloaded yet" state for an offline handler.
    AnsiString sLoadMO_TestFlow;                    // [DATA]   golden ProductionInfo.h:281
    // Offline no-op: the real body updates the production-info form's per-tray IC
    // count display, which has no headless equivalent.
    virtual void CalTrayICCount(int iWitchTray);    // [METHOD] golden ProductionInfo.h:377 -- offline: no-op
                                    //   (parameter spelling `iWitchTray` is golden's own, kept verbatim)

    // =======================================================================
    // AI(W906-FW3-PI1) 20260827: FW3-PI1 wave -- read-only batch translated
    // from golden ProductionInfo/ProductionInfo.cpp.  Every member and method
    // below is cited against its golden line.  None of them write a file,
    // touch a socket/FTP, exec an external program, or drive a
    // timer/*Click event -- those stay untranslated this wave (see the wave
    // report).
    //
    // LINK-BOUNDARY EXCLUSIONS (discovered this wave, not in the original
    // suggested list): forms/fProductionInfo.cpp is registered in
    // ht9045_forms, which links ONLY vclcompat + ht9045_globals + ht9045_core
    // (CMakeLists.txt:714-721) -- NOT ht9045_sm or ht9045_motor, and per
    // CMakeLists.txt:2249-2295 that direction cannot be added (it would be a
    // CMake target cycle; forms/fContact.cpp, fTrayAssignment.cpp,
    // fMotorTest.cpp and fTeach.cpp were all placed in ht9045_sm's OWN source
    // list instead of ht9045_forms for exactly this reason). Several golden
    // functions in the suggested batch reference symbols that live only in
    // ht9045_sm (ShowMyMessage/RecordProcess -- canary_support.cpp; ArmData/
    // TArm/TMySocket/NowControlBinCategory/OldControlBinCategory --
    // cSocket.cpp; TestSocket -- aHotPlateSubstrate.cpp) or ht9045_motor
    // (MOT[] -- Motor/mymotor.cpp). This wave's write boundary forbids
    // touching CMakeLists.txt, so those functions are NOT translated here:
    // bEnableIPSC, GetStringBySeparatedValues, GetArmBySiteFor32Site,
    // CalculateNowArmSiteBinQty, CalculateNowTotalICQty (calls the previous),
    // ClearArmSiteBinQty, CalICCountInHandler, UpdateControlBinCount,
    // bIsNeedCheckControlBin (calls UpdateControlBinCount). See the wave
    // report for the per-symbol breakdown.
    // =======================================================================

    // ---- new UI-state field (dfm leaf name, golden __published :74) ----
    TfLotInfoEdit *edInsertOPID_HALT;   // [DATA] golden :74 (TEdit*) -- SetInsertOPIDStr/GetInsertOPIDStr

    // ---- new plain-data fields (golden private/public, :197-570) ----
    AnsiString sLoadMO_MO;              // [DATA] golden :277 -- GetScheduleName's Mo half
    AnsiString sPI_STime;               // [DATA] golden :200 (private) -- SetPISTime/GetPISTime cache
    uTimeTool  utimetool;               // [DATA] golden :542 -- GetNowTime
    AnsiString _sOEE_MO;                // [DATA] golden :346 -- OEE_GetMO (OEE_SetMO's write side stays untranslated)
    bool _bOEEStartLotSuccess = false;  // [DATA] golden :265 -- IsOEEStartLotSuccess (golden has no ctor initializer for
                                         //   it either; `bool` default-constructs indeterminate in golden too -- false
                                         //   here is the deterministic "no Start Lot happened yet" offline reading)
    Word h = 0, n = 0, s = 0, z = 0;    // [DATA] golden :197 (private) -- DecodeTime() scratch, reused verbatim by
                                         //   GetNowTimeSec/bIntegerTimeCheck/bTimerCheck (golden keeps them as instance
                                         //   fields even though every use is write-then-immediately-read; kept faithful)
    AnsiString sDevice_Pin_Force;       // [DATA] golden :314 -- CheckContactForceExist
    AnsiString sDevice_Pin_Count;       // [DATA] golden :313 -- CheckContactForceExist

    int iNowUnloaderTrayQty[8]  = {};   // golden :430
    int iLastUnloaderTrayQty[8] = {};   // golden :431

    // ---- read-only methods (golden .cpp span cited; golden .h decl cited) ----
    AnsiString MyBoolToString(bool b);                                       // golden .cpp:1592-1598  .h:244
    AnsiString GetNowTime();                                                 // golden .cpp:5839-5843  .h:543
    int GetNowTimeSec();                                                     // golden .cpp:5119-5123  .h:465
    void SetPISTime();                                                       // golden .cpp:5845-5848  .h:544
    AnsiString GetPISTime();                                                 // golden .cpp:5850-5853  .h:545
    AnsiString OEE_GetMO();                                                  // golden .cpp:369-372    .h:241
    bool IsOEEStartLotSuccess();                                             // golden .cpp:1175-1178  .h:234
    AnsiString GetSetUpName();                                               // golden .cpp:5382-5385  .h:493
    AnsiString GetScheduleName();                                            // golden .cpp:5834-5837  .h:541
    void SetInsertOPIDStr(const AnsiString& s);                              // golden .cpp:5917-5920  .h:563
    AnsiString GetInsertOPIDStr();                                           // golden .cpp:5922-5925  .h:564
    AnsiString FilterAlphanumeric(const AnsiString& s);                      // golden .cpp:5927-5939  .h:565
    void CheckNewDayAndSubtract(int &iTimeSec);                              // golden .cpp:4931-4934  .h:458
    bool CheckContactForceExist();                                          // golden .cpp:4936-4947  .h:384
    bool bIntegerTimeCheck();                                                // golden .cpp:4506-4520  .h:415
    bool bTimerCheck(int iTimeMode);                                         // golden .cpp:4522-4538  .h:416
    bool IsContinueFailAlarm();                                              // golden .cpp:5773-5783  .h:539
    AnsiString GetCSVLineData(int iDataNum, AnsiString sCSVLineStr);         // golden .cpp:495-515    .h:242
    std::vector<int> GetBin0_8List();                                        // golden .cpp:6065-6077  .h:569
    void CalculateNowUnloaderTrayQty(bool bIsClear=false);                   // golden .cpp:4393-4405  .h:442
    int CalculateUnloadTotalICQty();                                         // golden .cpp:4429-4440  .h:444
    void ClearUnloaderTrayQty();                                             // golden .cpp:4460-4467  .h:446
    void ClearTrayCnt();                                                     // golden .cpp:3329-3339  .h:375

    // =======================================================================
    // AI(W906-FW3-PI2) 20260827: FW3-PI2 wave -- second read-only batch.
    // The 7 priority names the wave brief asked for FIRST (LoadMOInformation,
    // CalculateOEEReport, bCheckControlBinYield, CheckMOInformation,
    // CheckCloseInfo, CheckOEE_WhenStart, LoadYiedlInformation) were all read
    // in full and ALL 7 are excluded this wave -- see the wave report for the
    // per-function reason (FTP/file I/O, external-exec, or ht9045_sm link-
    // boundary symbols: fObserver/TastCategory/ArmData/ShowMyMessage all live
    // in ht9045_sm TUs cObserver.cpp/cSocket.cpp/canary_support.cpp, which
    // ht9045_forms cannot link -- same CMakeLists.txt:2249-2295 cycle FW3-PI1
    // already hit). The 12 functions below were self-picked from
    // coverage_probe's NONE list instead, each individually verified against
    // this same link-boundary + no-file/exec/hw rule.
    //
    // FLAGGED PER WAVE BRIEF RULE 4 (writes global machine/production state,
    // even though not on the brief's own exclusion list): NONE of the 12
    // below write global state -- SetBinTraySetting only mutates the
    // caller-owned TStringList* passed in, everything else touches only
    // TfProductionInfo's own fields. (GetBinTraySetting, SetBinTraySetting's
    // golden *caller*, was deliberately NOT translated: it writes
    // BinSelect[eBinFT].IfErrorT3 -- a global bin-routing config array in
    // cprod.cpp -- so it is not "read-only" despite the Get* name, the exact
    // naming trap this wave's brief warned about. SetBinTraySetting itself
    // has zero golden-global references -- see its own citation below.)
    // =======================================================================

    // ---- new plain-data fields (golden private, .h line cited) ----
    AnsiString _sOEE_HandlerID;         // [DATA] golden :347 -- OEE_SetHandlerID
    TDateTime  dtNowDateTime;            // [DATA] golden :195 -- TimeCount (write-only in this wave's scope; no
                                         //   translated getter reads it yet, faithful default-construct = 0.0)
    int _iOEE_RunTime = 0;              // [DATA] golden :247 -- TimeCount.  Golden's own ctor does not zero this
                                         //   either (real init is InitialOECount(), not translated this wave); 0 is
                                         //   the deterministic "no lot running yet" offline reading, same rationale
                                         //   as _bOEEStartLotSuccess above.
    int iNextDateTime = 0;               // [DATA] golden :262 -- SetNextRecordDateTime
    AnsiString _sOEE_ActivityID;        // [DATA] golden :350 -- SetStartStatus (CheckCloseInfo's other writer stays
                                         //   untranslated -- ShowMyMessage link-boundary, see exclusion list)
    AnsiString _sOEE_Status;            // [DATA] golden :348 -- SetStartStatus (CheckCloseInfo's other writer stays
                                         //   untranslated, same reason as _sOEE_ActivityID above)
    double dOffsetContactForce = 0.0;   // [DATA] golden :196 -- SetOffsetContactForce/GetOffsetContactForce

    // ---- read-only / field-only-write methods (golden .cpp span; golden .h decl) ----
    void OEE_SetMO(AnsiString sMO);                                          // golden .cpp:364-367    .h:205
    void OEE_SetHandlerID(AnsiString sHDID);                                 // golden .cpp:374-377    .h:206
    void TimeCount();                                                        // golden .cpp:920-940    .h:207
    void SetNextRecordDateTime();                                            // golden .cpp:1126-1134  .h:217
    void SetStartStatus();                                                   // golden .cpp:2818-2822  .h:228
    bool bEnableRPLog();                                                     // golden .cpp:3958-3974  .h:454
    void SetOffsetContactForce(AnsiString asValue);                          // golden .cpp:4949-4954  .h:459
    // golden .h:460-461 declare these 2 INLINE (one-liners) -- kept inline here too,
    // same precedent as cDynamicMultiContinualPassBinBySocket::ResetThresholdNum below.
    void SetOffsetContactForce(double dValue) { dOffsetContactForce = dValue; }   // golden .h:460
    double GetOffsetContactForce() { return dOffsetContactForce; }               // golden .h:461
    // GATE (PI2-G1) -- AI(W906-FW3-PI2-fix) 20260827: EnableInArmAutoCalSuckZ
    // (golden .cpp:5907-5910 / .h:556)、EnableOutArmAutoCalSuckZ (.cpp:5912-5915 / .h:557)
    // 與組合它們的 inline EnableAutoCalSuckZ (.h:555) **本波交付後被主迴圈移除**。
    // 理由是連結期缺符號：bEnableInarmSuckZAuto / bEnableOutarmSuckZAuto 雖然在
    // cmydef.cpp:6102-6103 有定義，但整段在 cmydef.cpp:6011 的 `#if 0 // TODO(W6)` 內
    // ——**文字存在，編譯器看不到**；cmydef.h:5806-5807 只有 extern 宣告，
    // 所以 -fsyntax-only 全過，是全新 build dir 的 link 期才炸出來。
    // 解它要先解 cmydef.cpp:6011 那個 #if 0（行為變更，單獨一波）。
    // 見 fProductionInfo.cpp 內同編號的 GATE 註記。
    void InitialStringGrid(TStringGrid *sg);                                 // golden .cpp:5949-5953  .h:560 -- golden's
                                    //   OWN body is fully commented out (dead/no-op in golden itself); translated verbatim
                                    //   as an empty body, not a facade-invented stub.
    void GetFTP_Setting(AnsiString &asUserID, AnsiString &asPassword, AnsiString &asHost);  // golden .cpp:5821-5832  .h:540
    // golden's caller-facing counterpart, GetBinTraySetting, is NOT translated (see the
    // block comment above) -- this helper has zero golden-global references of its own.
    bool SetBinTraySetting(TStringList* tlBinTray, AnsiString asSource, AnsiString asBin);  // golden .cpp:5254-5278  .h:492

    TfProductionInfo();
    virtual ~TfProductionInfo() {}
};
extern TfProductionInfo *fProductionInfo;   // golden: extern PACKAGE TfProductionInfo *fProductionInfo; (:572)

// ===========================================================================
//  cDynamicMultiContinualPassBinBySocket -- golden ProductionInfo.h:574-593
//  AI(W906-FW3-PI1) 20260827: added for AddThresholdNum()/GetMultiplierNum()
//  (both read-only: one increments a private in-memory counter bounded by an
//  IniConfig limit, the other reads it back through a power-of-two formula --
//  no I/O either way).  ResetThresholdNum() is golden's own inline ctor/dtor
//  helper, translated verbatim.  NOT wired to TfProductionInfo::cDynaThres
//  this wave -- no translated caller dereferences that pointer yet, and
//  adding a member with no consumer is exactly the "inventing surface" this
//  facade's own contract (forms/FormWidgets.h) warns against.  Add cDynaThres
//  to TfProductionInfo when a caller needs it.
// ===========================================================================
class cDynamicMultiContinualPassBinBySocket
{
private:
    int iDynamicThresholdNum;
public:
    cDynamicMultiContinualPassBinBySocket()
    {
        ResetThresholdNum();
    }
    ~cDynamicMultiContinualPassBinBySocket()
    {
        ResetThresholdNum();
    }
    int GetMultiplierNum();     // golden ProductionInfo.cpp:5955-5963
    void AddThresholdNum();     // golden ProductionInfo.cpp:5965-5969
    void ResetThresholdNum()
    {
        iDynamicThresholdNum=0;
    }
};

#endif // FORMS_FPRODUCTIONINFO_H
