// =============================================================================
//  cinitial.h  --  interface header for cinitial.cpp
//
//  Golden ref: HT9011UC_Code_V3.33.906.0_20260618/cinitial.h (declares the
//  full ~15,242-line cinitial.cpp surface).  THIS HEADER currently declares
//  ONLY the functions landed by GA-2-C1 (golden cinitial.cpp:77-3060):
//
//      GetSHCHKPos, InitialSuckerName, InitSucker, InitialSwitchName,
//      InitialSwitch, InitialSensorName, InitialSensor, InitialSafeDoor,
//      InitialHeaterDoor
//
//  (InitialSwitch() is also forward-declared in myswitch.h -- a pre-existing
//  stub for exactly this definition; repeating an identical declaration here
//  is legal C++ and keeps this header a complete, self-contained interface
//  for cinitial.cpp.)
//
//  GA-2-C2 .. GA-2-C5 (future waves) will append the remaining declarations
//  as they land the rest of cinitial.cpp (InitCylinder, InitialMotorParameter,
//  SetMotorSpeed family, ReadTechData/SetWorkParameter/ChangeRunParameter,
//  InitialMachine, Save/LoadMachineRecord, SetTechDataToProd*, ChangeSite,
//  shuttle-thread parameter setup, etc. -- see golden cinitial.h for the full
//  target list). Each future wave should extend this header in the same
//  ascending, banner-documented style cinitial.cpp itself uses.
//
//  Translator: AI(W906-GA2-C1) 20260804
//  Encoding: UTF-8, bare LF, trailing newline.
// =============================================================================
#ifndef cinitialH
#define cinitialH

//------------------------------------------------------------------------------
//  GA-2-C1  (golden cinitial.cpp:77-3060)
//------------------------------------------------------------------------------
int  GetSHCHKPos(int iSite, int iCenterBase);   // golden cinitial.h (JerryYang 20160728)
void InitialSuckerName();
void InitSucker();
void InitialSwitchName();
void InitialSwitch();
void InitialSensorName();
void InitialSensor();
void InitialSafeDoor();
void InitialHeaterDoor();

//------------------------------------------------------------------------------
//  GA-2-IMP  (golden cinitial.cpp:3392-4101)
//  Declared here because golden cinitial.h:13 declares exactly this.
//  NOTE: InitialMotorName() (golden cinitial.cpp:3109) is deliberately NOT
//  declared -- golden declares it in no header either; its only caller is
//  InitialMotorParameter, same TU, definition-first.
//------------------------------------------------------------------------------
void InitialMotorParameter();                   // golden cinitial.h:13


//------------------------------------------------------------------------------
//  PT-W(n3)  (golden cinitial.cpp:4390-15241 -- 11 functions, see the banner at
//  the end of cinitial.cpp).  ONLY the five that golden cinitial.h itself
//  declares are added here, in golden's own wording and golden's own order:
//      golden cinitial.h:10  void InitCylinder();
//      golden cinitial.h:17  bool SetWorkParameter();
//      golden cinitial.h:19  void SetSuckRetryCount();
//      golden cinitial.h:48  void SetSortArmSpeed(bool bShow);
//      golden cinitial.h:60  int  IsNNMode();
//  NOT added, deliberately:
//   * SaveUnloaderInfo(int) / SetTechDataToProd_Index / _TrayArm / _Shuttle /
//     _AOI -- golden declares NONE of them in ANY header (golden reaches
//     SaveUnloaderInfo through `extern` at asendic_Auto.cpp:44 and
//     csystem.cpp:152, and the SetTechDataToProd_* family purely
//     definition-before-use inside cinitial.cpp).  Inventing header wording
//     golden does not have would be an edit, not a translation.
//   * GetShuttleSize -- golden cinitial.h:57 keeps it COMMENTED OUT, so there
//     is no golden declaration to copy.  The body is landed and live.
//  None of the five carries a default argument, so a sibling appending an
//  identical declaration this wave is legal C++ and harmless.
//------------------------------------------------------------------------------
void InitCylinder();                                                            // golden cinitial.h:10
bool SetWorkParameter();                                                        // golden cinitial.h:17
void SetSuckRetryCount();                                                       // golden cinitial.h:19
void SetSortArmSpeed(bool bShow);                                               //RogerYang 20250515 Add for 9046AU
int IsNNMode();                                                                 //Steven 20240916 : NN mode 判斷

//------------------------------------------------------------------------------
//  PT-W8/n2  --  declarations for the eleven golden functions landed by group
//  n2 (golden cinitial.cpp :4103, :5614, :5623, :6650, :7710, :7722, :7999,
//  :10082, :11232, :13615, :15136).
//
//  ONLY the six that GOLDEN'S OWN cinitial.h declares are declared here, in
//  golden's wording and golden's order of appearance in that header.  The other
//  five are deliberately NOT declared:
//    * InitialCylinderName -- golden declares it in mycylin.h:139, and this
//      tree already mirrors that at mycylin.h:177.  Declaring it here too would
//      be redundant, and mycylin.h is not this slice's to duplicate.
//    * InitialClass, IsSuckerHasIC_NewIO_MN200, SetTechDataToProd_SortArm and
//      SetTechDataToProd_SortShuttle -- golden declares these in NO header at
//      all.  Each is called only from cinitial.cpp itself, below its own
//      definition in golden's line order, so no declaration is needed.
//      Adding one here would invent an interface golden does not have.
//
//  NOTE ON THE DEFAULT ARGUMENT: golden cinitial.h:26 is
//      void LoadMachineRecord(bool bSpare=false);
//  The `=false` therefore lives HERE, exactly once, and the definition in
//  cinitial.cpp does not repeat it.  Do not add a second default for this
//  function anywhere -- C++ makes that a hard error, not a warning.
//  golden cinitial.h:25's SaveMachineRecord(bool bSpare=false) is NOT declared
//  here: that function belongs to another group of this wave, and whoever lands
//  it owns its declaration (and its default argument).
//------------------------------------------------------------------------------
void LoadMachineRecord(bool bSpare=false);                                      // golden cinitial.h:26
void DoSetupSystemToProd();                                                     // golden cinitial.h:36
void SetUnloaderInfoFile(int iUnloader);                                        // golden cinitial.h:52 -- ChungHung 20150205 add for ATK
void LoadUnloaderInfo();                                                        // golden cinitial.h:53 -- ChungHung 20150205 add for ATK
bool CompareTechData();                                                         // golden cinitial.h:56 -- Steven 20220221
void TestTimeSetSpeed();                                                        // golden cinitial.h:63 -- Ztex 2024.08.11 Add Test Time Set Speed

//------------------------------------------------------------------------------
//  PT-W(n4)  (golden cinitial.cpp -- 10 functions, see the banner at the end of
//  cinitial.cpp).  ONLY the seven that golden cinitial.h itself declares are
//  added here, in golden's own wording and golden's own ascending order:
//      golden cinitial.h:23  void InitialMachine();
//      golden cinitial.h:37  void UpdateMyKitSuckDelayTimeToProd();
//      golden cinitial.h:42  void SetOffsetToTech();
//      golden cinitial.h:44  void SetMyKitSuckItemAmount();
//      golden cinitial.h:45  void InitShuttleThreadParameter();
//      golden cinitial.h:47  void SetOutArmSpeed(bool bShow);
//      golden cinitial.h:59  void SetHangupMaxTime();
//  NOT added, deliberately:
//   * InitHontechHardware / SetTechDataToProd_OutArm / SetTechDataToProd_AutoClean
//     -- golden declares NONE of the three in ANY header (golden reaches
//     SetTechDataToProd_AutoClean through a local `extern` at golden
//     AutoClean/AutoClean.cpp:6462, and the other two definition-before-use
//     inside cinitial.cpp).  Inventing header wording golden does not have
//     would be an edit, not a translation.
//   * SetMotorScaleSpeed / SetMotorAccelSpeed (golden cinitial.h:50-51) -- I do
//     not define them; both are already declared in cinitial.cpp itself
//     (:3698 by GA-2-IMP, :4674 by n3) and their only bodies in this tree are
//     the no-op stubs in acatchtray_shims.cpp.
//  None of the seven carries a default argument, so a sibling appending an
//  identical declaration this wave is legal C++ and harmless.
//------------------------------------------------------------------------------
void InitialMachine();
void UpdateMyKitSuckDelayTimeToProd();                                          //Steven 20250319 : 針對OnDelayTime轉換包成function
void SetOffsetToTech();
void SetMyKitSuckItemAmount();
void InitShuttleThreadParameter();                                              //Steven 20110407
void SetOutArmSpeed(bool bShow);
void SetHangupMaxTime();                                                        //Wei 20230224 : 重置死機時間

//------------------------------------------------------------------------------
//  PT-W6/n5  --  declarations for the eleven golden functions landed by group n5
//  (golden cinitial.cpp :4957, :5022, :5855, :8461, :8572, :11636, :13584,
//  :13713, :14267, :15116, :15126).
//
//  ONLY the SEVEN that GOLDEN'S OWN cinitial.h declares AND that are provably
//  safe to publish here are added, in golden's wording and golden's order of
//  appearance in that header.  The other four are deliberately NOT declared:
//
//    * SetSimuScreenPara / SetTechDataToProd_Tray / SetTechDataToProd_InArm --
//      golden declares NONE of them in ANY header (exhaustive grep of the golden
//      tree: cinitial.cpp only).  All three are reached definition-before-use
//      inside cinitial.cpp, exactly as golden reaches them.  Inventing header
//      wording golden does not have would be an edit, not a translation.
//
//    * GetIndexParm -- golden cinitial.h:30 DOES declare it
//      (`extern void GetIndexParm();`) and group n5 lands the body, but putting
//      it here BREAKS A FILE THIS GROUP MAY NOT EDIT: Motor/myGALILmotor.cpp
//      includes "cinitial.h" at :581 and then defines `static void
//      GetIndexParm() {}` at :730 (the #else arm of its own GATE(W4G-5)), which
//      g++ rejects as "declared 'extern' and later 'static'".  It is therefore
//      forward-declared TU-locally inside cinitial.cpp instead.  THE SAME TRAP
//      HAS ALREADY BITTEN: `void SetOutArmSpeed(bool bShow);` was added to this
//      header earlier this wave and aoutarm.cpp:543 (a `static` definition,
//      after its own #include at :493) NO LONGER COMPILES -- measured
//      2026-08-10 15:10.  Reported to the integrator.
//
//  None of the seven below carries a default argument, so an identical
//  declaration appended by a sibling this wave is legal C++ and harmless.
//  Verified before writing (2026-08-10 15:17): none of the seven names has a
//  `static` definition or a `#define` seam in ANY of the four real includers of
//  this header (cinitial.cpp:93, tests/test_ga2_c1_cinitial.cpp:24,
//  aoutarm.cpp:493, Motor/myGALILmotor.cpp:581).
//------------------------------------------------------------------------------
void SetMotorSpeed();                                                           // golden cinitial.h:15
bool ReadTechData();                                                            // golden cinitial.h:18
void SetTechDataToProd();                                                       // golden cinitial.h:32
void SetMotorScaleSpeed(int Index, int ScaleSpeed);                             // golden cinitial.h:50
void InitialHT9045SModule();                                                    //Steven 20161117 : for HT-9045S
int GetInArmSuckBaseHeight(int iRow, int iCol);                                 // golden cinitial.h:61
int GetOutArmSuckBaseHeight(int iRow, int iCol);                                // golden cinitial.h:62

//------------------------------------------------------------------------------
//  PT-W(n1)  (golden cinitial.cpp -- 10 functions; see the BLOCK N1 banner at
//  the end of cinitial.cpp).  ONLY the nine that golden cinitial.h itself
//  declares are added here, in golden's own wording and golden's own ascending
//  order in that header:
//      golden cinitial.h:9   void InitialHandler();
//      golden cinitial.h:25  void SaveMachineRecord(bool bSpare=false);
//      golden cinitial.h:31  void SetAllMotorSpeed(int speed);
//      golden cinitial.h:33  void SetTechDataToProd_Yield();
//      golden cinitial.h:34  bool CheckFix3FullPlaceTechData();
//      golden cinitial.h:39  void ShowMainScreenPresure(int index);
//      golden cinitial.h:40  void ChangeSite();
//      golden cinitial.h:46  void SetInArmSpeed(bool bShow, bool bReset=false);
//      golden cinitial.h:51  void SetMotorAccelSpeed(int Index, int ADCSpeed);
//  NOT added, deliberately:
//   * CheckMotorValue(TTable*, AnsiString, bool) -- golden declares it in NO
//     header at all (it is absent from golden cinitial.h); its only caller is in
//     the same TU.  Its body is landed GATED (N1-G1, TTable/BDE), so there is
//     nothing to expose either way.
//
//  DEFAULT ARGUMENTS -- two of the nine carry one, and a default argument may be
//  given only ONCE per translation unit (a repeat is a HARD ERROR, not a
//  warning).  Both are handled explicitly:
//
//    * SaveMachineRecord(bool bSpare=false)  golden cinitial.h:25 -- plain.
//      cinitial.h:103-105 already reserved this default for this group, and
//      verified 2026-08-10 15:26 that the name is declared in NO other header
//      in this tree.  The definition in cinitial.cpp does NOT repeat the `=`.
//
//    * SetInArmSpeed(bool bShow, bool bReset=false)  golden cinitial.h:46 -- a
//      sibling has ALREADY published this exact declaration, WITH its default,
//      inside cinitial.cpp itself (:13195-:13197), wrapped in its own
//      `#ifndef HT9045_CINITIAL_FWD_SetInArmSpeed` guard for precisely this
//      collision.  cinitial.h is included at cinitial.cpp:93, i.e. BEFORE that
//      block, so declaring it here UNGUARDED would give the default twice and
//      break the build.  The guard is therefore reused below: defining the macro
//      here makes the in-.cpp block a no-op, and the header becomes the single
//      home of the default.  Do NOT unwrap it while cinitial.cpp:13195 stands.
//
//  The other seven carry no default, so a sibling appending an identical
//  declaration is legal C++ and harmless.
//
//  THE `static`-CLASH TRAP, CHECKED FOR ALL NINE (2026-08-10 15:26): publishing a
//  name here breaks any includer that defines the same name `static` after its
//  own #include -- that is what happened to aoutarm.cpp when SetOutArmSpeed was
//  added earlier this wave (see the PT-W6/n5 note below).  cinitial.h has exactly
//  FOUR real includers -- cinitial.cpp:93, aoutarm.cpp:493,
//  Motor/myGALILmotor.cpp:581, tests/test_ga2_c1_cinitial.cpp:24 -- and NONE of
//  them contains a `static` definition or a `#define` seam for ANY of these nine
//  names.  (The other four files that name "cinitial.h" do so only in comments.)
//
//  SetMotorAccelSpeed: note cinitial.cpp:3698 already forward-declares it
//  (GA-2-IMP) and acatchtray_shims.h:417 declares it too, both WITHOUT a default
//  -- three identical declarations are legal.  Unlike n4, THIS group does define
//  it (golden cinitial.cpp:4997), which is why the declaration belongs here now.
//------------------------------------------------------------------------------
void InitialHandler();                                                          // golden cinitial.h:9
void SaveMachineRecord(bool bSpare=false);                                      // golden cinitial.h:25
void SetAllMotorSpeed(int speed);                                               // golden cinitial.h:31
void SetTechDataToProd_Yield();                                                 // golden cinitial.h:33
bool CheckFix3FullPlaceTechData();                                              //Steven 20130126 : Fix3滿盤功能
void ShowMainScreenPresure(int index);                                          // golden cinitial.h:39
void ChangeSite();                                                              // golden cinitial.h:40
#ifndef HT9045_CINITIAL_FWD_SetInArmSpeed
#define HT9045_CINITIAL_FWD_SetInArmSpeed
void SetInArmSpeed(bool bShow, bool bReset=false);                              //Steven 20140909 : Clean Out後要重置Speed
#endif
void SetMotorAccelSpeed(int Index, int ADCSpeed);                               // golden cinitial.h:51
#endif
