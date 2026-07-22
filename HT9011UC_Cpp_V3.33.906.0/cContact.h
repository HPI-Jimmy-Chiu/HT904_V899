// cContact.h
// Standard C++ translation of BCB6 cContact.h (V3.33.906.0_20260618) -- CALC CORE ONLY.
// Source of truth: HT9011UC_Code_V3.33.906.0_20260618/cContact.h (declarations, lines 607-609,
//                   612/dMinForce is actually at line 619; see note below), and
//                   HT9011UC_Code_V3.33.906.0_20260618/cContact.cpp (bodies, lines 18893-19022).
//
// AI(W64b-ForceCalc-Translate) 20260710: new file. Extract-calc-core translation of the two
// pure-math TfContact member functions used by CalculateTotalAirForce (golden :18675-18891,
// itself NOT translated here -- see scope note below).
//
// Translation scope: cContact.cpp is a 22761-line VCL form (TfContact) mixing dense switch-case
// state machines with VCL widgets (TRadioGroup, TEdit, TLabel, ...) and global config reads.  Per
// the project's established extract-calc-core convention (see ContactForce.h/.cpp -- the DIRECT
// PRECEDENT for this file, same SLK/contact-force domain), this wave translates ONLY:
//
//   1. TfContact::GetMaxIndexForceLimit()                    (golden cContact.cpp:18893-18955)
//   2. TfContact::GetMinForce(double dKitDiameter, int iTag) (golden cContact.cpp:18957-19022)
//
// as FREE FUNCTIONS that take every external read (class members, global config, and the one VCL
// widget selection GetMaxIndexForceLimit reads) as explicit parameters, and RETURN the computed
// value instead of mutating class state.  TfContact itself, the global fContactForce instance, and
// the rgKitDiameter VCL radio group do NOT exist in the translated tree; this file does not create
// them.  CalculateTotalAirForce (golden :18675-18891, VCL-widget-heavy caller) and everything else
// in cContact.cpp/.h/.dfm is OUT OF SCOPE for this wave -- see MIGRATION_ROADMAP.md's cContact
// DEFERRED entry (TfContact::DoTestContactFunction's CarlibrationTask + 6 sub-SMs, ~11347 lines).
//
// ---------------------------------------------------------------------------------------------
// DEFERRED note on dMinForce member-persistence semantics (read before wiring up a future
// full-TfContact translation):
//
//   Golden TfContact::GetMinForce mutates AND returns the class member `double dMinForce;`
//   (golden cContact.h:619) in every branch.  Grepping the whole golden 906 tree for
//   `GetMinForce\|GetMaxIndexForceLimit\|dMinForce` shows:
//     * The ONLY call site of GetMinForce anywhere in the golden tree is
//       cContact.cpp:18763 (inside CalculateTotalAirForce): `dMinForce=GetMinForce(dKitDiameter,
//       iTag);` -- i.e. it just re-assigns the SAME member from the function's return value.
//       Treating GetMinForce as a pure function (local variable in, local variable out, no member
//       side-effect) is therefore 100% behaviorally faithful for THAT call site.
//     * The member `dMinForce` is READ again later, independent of any GetMinForce call, at
//       cContact.cpp:5650 and :8414 (`ADAM_WriteVoltage(dMinForce/2.0);` inside the Z1/Z2
//       auto-get-height routines) -- both untranslated/out of scope this wave.
//     * This prompt's scoping guidance also cited a AutoClean/uCleaning.cpp:1741 dMinForce read;
//       that line/reference does NOT exist in this golden 906 snapshot (uCleaning.cpp has zero
//       occurrences of `dMinForce`) -- GOLDEN WINS, so that citation is dropped as a discrepancy
//       (documented in the wave's final report, not fixed here).
//   CONCLUSION: a future wave that translates the REAL TfContact class + CalculateTotalAirForce +
//   the Z1/Z2 auto-get-height routines will still need to wire up `dMinForce` as a persistent
//   member (assigned once per CalculateTotalAirForce call, read later by the unrelated Z1/Z2
//   routines) -- that persistence is NOT modeled by the free function below and is explicitly
//   OUT OF SCOPE / DEFERRED, not "solved" by returning a value instead of a member.
// ---------------------------------------------------------------------------------------------
//
// BCB6 source lines cited inline in cContact.cpp (this translated file).

#ifndef CCONTACT_H
#define CCONTACT_H

#include "MachineType.h"   // eIndexPressType, eTestMode constants (already translated, header-only)

// ---------------------------------------------------------------------------
// AI(W906-cContactLeaf) 20260721: contact-mode / index-down-pos constants the
// leaves below need.  Golden home: cContact.h:20-28 (`extern const int ...`,
// values defined cContact.cpp:73-90) -- TfContact-adjacent free consts, NOT
// class members.
//
// HAZARD (read before adding a #include of this header to any TU that also
// includes BarCode/BarCode_Shuttle2_CCDScan.h): that header ALREADY carries
// its OWN local copy of CONTACT_NORMAL/CONTACT_AUTO_GET_HEIGHT/CONTACT_TEST
// (its :187-189, added while cContact.h/.cpp were still untranslated, "no ODR
// risk" reasoning that assumed no TU would ever see both copies).  Following
// that SAME precedent pattern here (plain internal-linkage `const int`, not
// `extern`) means CONTACT_NORMAL is now defined independently in TWO headers.
// As of this writing NO .cpp includes both (verified by grep across the whole
// target tree); if a future wave ever makes one .cpp include both cContact.h
// and BarCode_Shuttle2_CCDScan.h, that TU will fail to compile with a
// redefinition error on CONTACT_NORMAL -- the fix at that point is to delete
// the local copy in BarCode_Shuttle2_CCDScan.h and have it consume this one
// instead (cContact.h is now the real, translated home). Not fixed proactively
// here to keep this wave's diff minimal.
// ---------------------------------------------------------------------------
const int CONTACT_NORMAL            = 0;   // golden cContact.cpp:74
const int CONTACT_MANUAL_GET_HEIGHT = 2;   // golden cContact.cpp:76 -- added this wave for ComputeAutoHeightMaxKGTorque
const int CONTACT_DEVICE_MAP_CHECK  = 9;   // golden cContact.cpp:83 -- added this wave for ComputeIndexYSpeed/ComputeIndexZSpeed

// golden cContact.cpp:90-92 (file-scope `const double`, TfContact-adjacent, NOT class members).
const double fIndexDownPos_for9045   = -135.0;                            // golden cContact.cpp:90
const double fIndexDownPos_forATC    = -146.0;   //wei 20160303 HT-9045HA -146     // golden cContact.cpp:91
const double fIndexDownPos_for9046LS = -148.0;   //ChungHung 20140604 LS AutoHeight // golden cContact.cpp:92

// ---------------------------------------------------------------------------
// ComputeMaxIndexForceLimit
//   Portable replacement for TfContact::GetMaxIndexForceLimit().
//   BCB6 source: cContact.cpp:18893-18955
//
//   Parameters (every external read the golden body performs, as an explicit param):
//     indexPressType         -- global INDEX_PRESS_TYPE            (cmydef.h/.cpp, eIndexPressType)
//     dDutCount               -- TfContact member double dDutCount  (cContact.h:555)
//     iTestMode               -- global TestIF.iTestMode            (int; compared against eTestMode
//                                 constants SingleSite/DualSite/QualSite2X2N, MachineType.h)
//     bD27UseSingleSite85kg   -- global IniConfig.bD27UseSingleSite85kg (Config.h, bool)
//     iKitDiameterItemIndex   -- VCL rgKitDiameter->ItemIndex (int; TRadioGroup selection index)
//
//   Faithfully preserves the fixed-kg if/else-if ladder over indexPressType, and the else-branch's
//   nested condition + its own inner if/else-if (ItemIndex==1 -> 55, ItemIndex==2 -> 85) with
//   NO trailing else -- this is a REAL golden gap: any other ItemIndex value inside that inner
//   branch leaves dMaxLimit at its prior value, which is 0.0 (the local's initializer), NOT a
//   sensible default.  Do not "fix" this gap; it is preserved on purpose (see inline comment in
//   the .cpp body).
// ---------------------------------------------------------------------------
double ComputeMaxIndexForceLimit(eIndexPressType indexPressType,
                                  double dDutCount,
                                  int iTestMode,
                                  bool bD27UseSingleSite85kg,
                                  int iKitDiameterItemIndex);

// ---------------------------------------------------------------------------
// ComputeMinForce
//   Portable replacement for TfContact::GetMinForce(double dKitDiameter, int iTag).
//   BCB6 source: cContact.cpp:18957-19022
//
//   Parameters:
//     dKitDiameter        -- unchanged golden param (double)
//     dTagContactOffset    -- fContactForce->SLKClass[iTag]->dContactOffset    (already-resolved
//                             scalar; see ContactForce.h's SlkForceData::dContactOffset)
//     dTagContactOffsetNS  -- fContactForce->SLKClass[iTag]->dContactOffset_NS (see
//                             SlkForceData::dContactOffset_NS)
//     bNSKitPress          -- global TestIF_File.bNSKitPress (cprod.h, bool)
//     bD04MinForceByFile   -- global IniConfig.bD04MinForceByFile (Config.h, bool)
//     dD04MinForceByFile{,_20mm,_30mm,_40mm,_60mm,_80mm}
//                          -- global IniConfig.dD04MinForceByFile* family (Config.h, double)
//
//   Faithfully preserves every diameter bucket (20 / 30-or-28 / 40 / 60-or-58 / 80 / else) and the
//   "only override if the file-configured min is LARGER" pattern (`if(dMinForce<X) dMinForce=X`)
//   in every bucket, and the else-branch's bNSKitPress selection (dContactOffset_NS vs
//   dContactOffset) before its own bD04MinForceByFile override.
//
//   NOTE: the golden function mutates+returns the TfContact member dMinForce (golden cContact.h:619,
//   NOT line ~612 as a rough estimate might suggest).  This free function returns a LOCAL value
//   instead of mutating a member -- see the DEFERRED note above this header's include guard for why
//   that is faithful for the one real call site, and what remains deferred for other call sites.
// ---------------------------------------------------------------------------
double ComputeMinForce(double dKitDiameter,
                        double dTagContactOffset,
                        double dTagContactOffsetNS,
                        bool bNSKitPress,
                        bool bD04MinForceByFile,
                        double dD04MinForceByFile,
                        double dD04MinForceByFile_20mm,
                        double dD04MinForceByFile_30mm,
                        double dD04MinForceByFile_40mm,
                        double dD04MinForceByFile_60mm,
                        double dD04MinForceByFile_80mm);

// ===========================================================================
// AI(W906-cContactLeaf) 20260721: W906 cContact leaf-function wave (Step 1 +
// Step 2 of the wave plan).  All 6 functions below follow the SAME
// extract-calc-core / explicit-parameter convention as ComputeMaxIndexForceLimit
// / ComputeMinForce above: every global/member the golden body reads is an
// explicit parameter, no globals are read directly inside cContact.cpp.
// ===========================================================================

// ---------------------------------------------------------------------------
// ComputeIsRun2DCheck
//   Portable replacement for TfContact::IsRun2DCheck().
//   BCB6 source: cContact.cpp:21203-21219 (JerryYang 20250220 : 2DID安裝順序檢查功能)
//
//   Parameters (every external read the golden body performs, as an explicit param):
//     bCheck2DHardware  -- global IniConfig.bF33_Check2DHardware      (Config.h, bool)
//     iBarCodeInstall   -- global BAR_CODE_INSTALL                    (cmydef.h, int; compared
//                          against ebctUseCCDMode/ebctInShtIntel/ebctEtherNetCCD, MachineType.h)
//     bEnableBarCode    -- global TestIF_File.bEnableBarCode          (cprod.h, bool)
//     iInArmShtRow      -- global InArmSuck.iShtRow                   (aHotPlateSubstrate.h, int)
//     iContactMode      -- global iContactMode                        (cmydef.h, int; NOT a
//                          TfContact member -- verified golden cmydef.h:3089 `extern int iContactMode;`)
//     bRun2DCheck       -- global bRun2DCheck                         (cmydef.h, bool; also NOT a
//                          TfContact member -- golden cmydef.h:5817)
//
//   Faithfully preserves the flat 6-term AND (the 3-way BAR_CODE_INSTALL OR-group counts as one
//   term) with no else branch (ret stays false unless every term is true).  Golden also carries a
//   commented-out 7th term `//TestIF_File.bEnableBottom2D==false` (JerryYang 20250428 note "fix
//   contact mode有alarm") -- dead code in golden itself, not translated (nothing to translate).
//
//   CALLER NOTE (behavior-change risk, checked this wave): as of this translation, NOTHING in the
//   translated tree ever assigns bRun2DCheck=true (grep confirms cmydef.cpp's own zero-initializing
//   definition -- un-gated this same wave from the InitialMemory tail block, see the
//   AI(W906-cContactLeaf) comment at its new location -- is the ONLY assignment; the golden setter
//   logic lives in the still-deferred ainarm InArm-suck workflow).  So this function currently
//   evaluates to `false` for every one of its 78 golden call sites across 34 files (26
//   ainarm*.cpp variants incl. ainarm2.cpp/ainarm9045.cpp, csystem.cpp:1812/9577/9583/13012/24260,
//   3 BarCode/BarCode_*.cpp files, AutoClean.cpp, ckernel.cpp, cOffSet.cpp, uhome.cpp -- recounted
//   by AI(W906-cContactLeaf-review) 20260722, correcting this wave's original "40+ ... 18
//   ainarm9045_* ... csystem.cpp:838/2649 ... 8 BarCode" citation, which did not match any real
//   IsRun2DCheck() call site) under every reachable global state in the CURRENT tree --
//   identical to the hardcoded-false stub it replaces.  This is a real behavior change only once a
//   future wave translates whatever sets bRun2DCheck=true.
// ---------------------------------------------------------------------------
bool ComputeIsRun2DCheck(bool bCheck2DHardware,
                          int  iBarCodeInstall,
                          bool bEnableBarCode,
                          int  iInArmShtRow,
                          int  iContactMode,
                          bool bRun2DCheck);

// ---------------------------------------------------------------------------
// kgTranToMitsubishikg
//   Translated VERBATIM: this is already a free function in golden (NOT a TfContact member).
//   BCB6 source: cContact.cpp:5261-5271.  Pure int clamp (kg/3, floor 15, ceil 30), zero deps.
// ---------------------------------------------------------------------------
int kgTranToMitsubishikg(int kg);

// ---------------------------------------------------------------------------
// ComputeAutoHeightMaxKGTorque
//   Portable replacement for TfContact::GetAutoHeightMaxKGTorque().
//   BCB6 source: cContact.cpp:5273-5382 (Steven 20170720 (wei) : for low contact force)
//
//   Parameters:
//     iContactMode                     -- global iContactMode (cmydef.h, int)
//     bD10ManualHeightComptibleWithNS  -- global IniConfig.bD10ManualHeightComptibleWithNS (bool)
//     bD14_AutoHeightUseSetTorque      -- global IniConfig.bD14_AutoHeightUseSetTorque (bool)
//     iD14_AutoHeightUseSetTorque      -- global IniConfig.iD14_AutoHeightUseSetTorque (int)
//     dKitDiameter                     -- global DeviceForm.dKitDiameter (cprod.h, double)
//     indexPressType                   -- global INDEX_PRESS_TYPE (cmydef.h, eIndexPressType)
//     iTestMode                        -- global TestIF.iTestMode (cprod.h, int)
//     iTestSocketShtRow/Col            -- global TestSocket.iShtRow/iShtCol (aHotPlateSubstrate.h, int)
//     iSiteMap                         -- global TestIF_File.iSiteMap[MAX_SOCKET_ROW][MAX_SOCKET_COL]
//                                         (cprod.h, int; first dim decayed per this tree's existing
//                                         2D-array-param convention, see aoutarm9045_2x4_8.cpp:164)
//     iNNMode                          -- return value of global IsNNMode() (atester_shims.h/.cpp,
//                                         int; passed in rather than called here to keep this file
//                                         dependency-free, matching the ComputeMinForce precedent)
//
//   Faithfully preserves every INDEX_PRESS_TYPE branch (e240KG/e260KG open-site-count loop over
//   TestIF_File.iSiteMap, e400KG/e360KG, e500KG, e640KG, e800KG, else) and the low-contact-force
//   dKitDiameter<2.5 branch's e85KG special case.  Uses CONTACT_MANUAL_GET_HEIGHT (golden
//   cContact.cpp:76, value 2 -- added to this header this wave; the header previously only had
//   CONTACT_NORMAL, which Step 1's ComputeIsRun2DCheck needed).
// ---------------------------------------------------------------------------
int ComputeAutoHeightMaxKGTorque(int iContactMode,
                                  bool bD10ManualHeightComptibleWithNS,
                                  bool bD14_AutoHeightUseSetTorque,
                                  int iD14_AutoHeightUseSetTorque,
                                  double dKitDiameter,
                                  eIndexPressType indexPressType,
                                  int iTestMode,
                                  int iTestSocketShtRow,
                                  int iTestSocketShtCol,
                                  const int iSiteMap[][MAX_SOCKET_COL],
                                  int iNNMode);

// ---------------------------------------------------------------------------
// ComputeTestZCompensationHight
//   Portable replacement for TfContact::TestZ_CompensationHight().
//   BCB6 source: cContact.cpp:18453-18540 (Ifor 20200318 : add 針對80mm套件因為手臂Hold的變化需補下壓高度)
//   NOTE: golden's own method name misspells "Height" as "Hight" -- preserved verbatim in this
//   free-function name for grep-back-to-golden fidelity (not silently corrected).
//
//   Parameters:
//     dKitDiameter    -- global DeviceForm_File.dKitDiameter (cprod.h, double)
//     dPress          -- global DeviceForm_File.dPress       (cprod.h, double)
//     dIndexZOffset   -- global dIndexZOffset[3][15]         (cmydef.h, double; first dim decayed
//                        per this tree's 2D-array-param convention)
//     iTotalOffset_1/2 [out] -- golden file-scope globals `double iTotalOffset_1/2` (cContact.cpp:
//                        98-99 -- NOTE: declared `double` despite the `i`-prefix; this is a golden
//                        naming quirk, preserved verbatim, NOT a translation typo).  Passed by
//                        reference instead of being real persistent globals: as of this wave every
//                        real call site that READS iTotalOffset_1/2 (cContact.cpp:6591/7162/7198/
//                        7431/9767/9801/9976) is inside the still-fully-deferred CarlibrationTask
//                        cluster, so there is no live caller needing global persistence yet -- same
//                        "faithful for the one real call site, rest deferred" reasoning as
//                        ComputeMinForce's dMinForce note above.
//
//   GOLDEN QUIRK preserved verbatim: when dKitDiameter is NOT 8 or 40.2, the golden function body
//   does NOTHING -- it does not touch iTotalOffset_1/iTotalOffset_2 at all (they retain whatever
//   value they already had).  This free function mirrors that exactly: the out-params are only
//   ever written inside the `dKitDiameter==8 || dKitDiameter==40.2` branch, never zeroed/touched
//   otherwise.  Do NOT "fix" this into an unconditional reset; it would diverge from golden.
// ---------------------------------------------------------------------------
void ComputeTestZCompensationHight(double dKitDiameter,
                                    double dPress,
                                    const double dIndexZOffset[][15],
                                    double& iTotalOffset_1,
                                    double& iTotalOffset_2);

// ---------------------------------------------------------------------------
// ComputeIndexDownPos
//   Portable replacement for TfContact::SetIndexDownPos().
//   BCB6 source: cContact.cpp:18628-18661 (Steven 20211109 : 改成公用變數)
//
//   Parameters:
//     bUserDefMaxContactHeight  -- global bUserDefMaxContactHeight (cmydef.h, bool)
//     dUserDefMaxContactHeight  -- global dUserDefMaxContactHeight (cmydef.h, double)
//     iMachineTypeChoice        -- global MachineTypeChoice (cmydef.h, int; compared against
//                                  Type_HT9046_LS, MachineType.h)
//     iCustomerCode             -- global CUSTOMER_CODE (cmydef.h, int; compared against
//                                  CC_QUALCOMM/CC_KYEC_LEE/CC_KYEC_CHEN/CC_KYEC_JCTHIU/CC_DL_TEK/
//                                  CC_KYEC_XILINX, MachineType.h)
//     iATCMixMode               -- global ATC_MixMode (cmydef.h, int; compared against
//                                  eMixATC60_ATC33, MachineType.h)
//     iATCSystem                -- global ATC_SYSTEM (cmydef.h, int; compared against
//                                  eATCUninstall, MachineType.h)
//
//   Returns the value golden assigns to the class member/global `float fIndexDownPos` (cmydef.h:
//   5404, `extern double fIndexDownPos;` in this tree) instead of mutating it -- fIndexDownPos IS
//   already a real global in this tree, but this leaf has no live caller yet (see file banner in
//   cContact.cpp), so mutating it here would be an unused side effect; the caller assigns the
//   return value once this function gets wired up.  Faithfully preserves the branch order
//   (bUserDefMaxContactHeight > Type_HT9046_LS > ATC_SYSTEM!=eATCUninstall > else) and the
//   Type_HT9046_LS branch's nested CC_QUALCOMM+eMixATC60_ATC33 -20 adjustment and the else branch's
//   5-customer-code -5 adjustment list.
// ---------------------------------------------------------------------------
double ComputeIndexDownPos(bool bUserDefMaxContactHeight,
                            double dUserDefMaxContactHeight,
                            int iMachineTypeChoice,
                            int iCustomerCode,
                            int iATCMixMode,
                            int iATCSystem);

// ---------------------------------------------------------------------------
// ComputeIndexYSpeed / ComputeIndexZSpeed
//   Portable replacements for TfContact::GotIndexYSpeed(int)/GotIndexZSpeed(int).
//   BCB6 source: cContact.cpp:18663-18667 / 18669-18673 (JimmyChiu 20211028 : All speed can set by
//   speed setting. iContactMode==CONTACT_DEVICE_MAP_CHECK)
//
//   Parameters:
//     iFixValue      -- unchanged golden param (int, the `_fixValue` default speed)
//     iContactMode   -- global iContactMode (cmydef.h, int)
//     iMotGailSpeed  -- golden MOT[MTestY1].GailSpeed / MOT[MTestZ1].GailSpeed (Motor/mymotor.h,
//                       int; MOT[] is real -- Motor/mymotor.h:385 `extern class TTrayMotor
//                       MOT[MAX_TRAY_MOTOR];`).  Passed in rather than indexed here to keep this
//                       file free of a Motor/mymotor.h dependency, matching this file's existing
//                       explicit-parameter convention; caller passes MOT[MTestY1].GailSpeed /
//                       MOT[MTestZ1].GailSpeed respectively.
//
//   NOTE: golden mutates+returns the TfContact member `int itemp` (cContact.h:534, "工具判斷使用" --
//   a scratch/shared temp, NOT a per-call-site-isolated local).  This free function uses a plain
//   local instead and returns it -- itemp has no other reader in golden between the assignment and
//   the return (single-statement mutate-then-return), so this is behaviorally identical; same
//   "local stand-in for the golden member" pattern as ComputeMinForce's dMinForce.
// ---------------------------------------------------------------------------
int ComputeIndexYSpeed(int iFixValue, int iContactMode, int iMotGailSpeed);
int ComputeIndexZSpeed(int iFixValue, int iContactMode, int iMotGailSpeed);

#endif // CCONTACT_H
