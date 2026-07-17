add_executable(test_ContactForce
    test_ContactForce.cpp
)

target_link_libraries(test_ContactForce PRIVATE ht9045_core)

add_test(NAME ContactForce COMMAND test_ContactForce)

# --- cContact: ComputeMaxIndexForceLimit / ComputeMinForce (W64b force-calc extract) -----
#   Same simple link shape as test_ContactForce above: cContact.cpp only #includes
#   MachineType.h (header-only) and takes every other external read as an explicit
#   parameter, so it needs no ht9045_globals / RESCAN link group.
add_executable(test_cContact
    test_cContact.cpp
)

target_link_libraries(test_cContact PRIVATE ht9045_core)

add_test(NAME cContact COMMAND test_cContact)

# --- cUnitConvert: iUnitMultiply100 / iUnitMultiply1000 (W2 pure extract) ---
add_executable(test_cUnitConvert
    test_cUnitConvert.cpp
)

target_link_libraries(test_cUnitConvert PRIVATE ht9045_core)

add_test(NAME cUnitConvert COMMAND test_cUnitConvert)

# --- vclcompat semantics harness (W0) --------------------------------------
add_executable(test_vclcompat
    test_vclcompat.cpp
)

target_link_libraries(test_vclcompat PRIVATE vclcompat)

add_test(NAME vclcompat COMMAND test_vclcompat)

# --- HTMD5 + cJSON public library (W0 LeafUtils) ---------------------------
add_executable(test_HTMD5
    test_HTMD5.cpp
)

target_link_libraries(test_HTMD5 PRIVATE ht9045_public)

add_test(NAME HTMD5 COMMAND test_HTMD5)

# --- cJSON round-trip smoke test (W0 LeafUtils) ----------------------------
add_executable(test_cJSON
    test_cJSON.cpp
)

target_link_libraries(test_cJSON PRIVATE ht9045_public)

add_test(NAME cJSON COMMAND test_cJSON)

# ===========================================================================
#  W1 Public leaf utilities
# ===========================================================================

# --- ExternFunction: Math_AdjustAngle / String_JustNumber / Erase* / RecordTime
add_executable(test_ExternFunction
    test_ExternFunction.cpp
)

target_link_libraries(test_ExternFunction PRIVATE ht9045_public)

add_test(NAME ExternFunction COMMAND test_ExternFunction)

# --- WinSocketErrorCode: GetErrorMsg(Winsock code -> AnsiString) ------------
add_executable(test_WinSocketErrorCode
    test_WinSocketErrorCode.cpp
)

target_link_libraries(test_WinSocketErrorCode PRIVATE ht9045_public)
# winsock2 constants come from <winsock2.h>; no functions are called, but link
# ws2_32 defensively in case a future expansion calls a WSA* function.
if(WIN32)
    target_link_libraries(test_WinSocketErrorCode PRIVATE ws2_32)
endif()

add_test(NAME WinSocketErrorCode COMMAND test_WinSocketErrorCode)

# --- cBootLog: WriteBootLog / RotateBootLogIfNeeded (Win32 file tracer) -----
add_executable(test_cBootLog
    test_cBootLog.cpp
)

target_link_libraries(test_cBootLog PRIVATE ht9045_public)

add_test(NAME cBootLog COMMAND test_cBootLog)

# ===========================================================================
#  W2 Common logic islands
# ===========================================================================

# --- PickPlanner: cArmPickPlan engine + Loader/OutArm scaffold adapters -----
#   Stub IArmPickPlanContext drives the generic enumerate-and-score engine;
#   asserts best-plan selection, strict-'>' tie-break, boundary guards, and
#   the shipped scaffolds' zero-side-effect (Search()==false) behaviour.
add_executable(test_PickPlanner
    test_PickPlanner.cpp
)

target_link_libraries(test_PickPlanner PRIVATE ht9045_common)

add_test(NAME PickPlanner COMMAND test_PickPlanner)

# ===========================================================================
#  W0-TAIL global define/type headers compile gate
# ===========================================================================
# Includes all 5 target headers + Config/CosFunction/myTimer/MachineDefine and
# static_asserts a spot-check of preserved enum/#define values. Links against
# vclcompat (IntToHex etc.) + myTimer.o (TQPF_Timer ctor). Does NOT pull the
# cprod/cmydef/cpublic active globals (their ctors are W6-gated).
add_executable(test_w0tail_headers
    w0tail_headers_compile.cpp
)

target_link_libraries(test_w0tail_headers PRIVATE vclcompat)
# myTimer.cpp lives in ht9045_globals; pull just its TQPF_Timer ctor via the lib.
target_link_libraries(test_w0tail_headers PRIVATE ht9045_globals)

add_test(NAME w0tail_headers COMMAND test_w0tail_headers)

# --- W0-TAIL VERIFY: standalone-compile + value/layout spot-check -----------
#   #includes every in-scope translated global header and asserts the
#   define/enum -> value pairs + PROD_INFO_ST sizeof/offsetof layout anchors
#   (static_assert at compile time + a runtime self-check). Links vclcompat
#   (IntToHex / value types) + ht9045_globals (TQPF_Timer ctor in myTimer.o).
add_executable(test_globals
    test_globals.cpp
)

target_link_libraries(test_globals PRIVATE vclcompat ht9045_globals)

add_test(NAME globals COMMAND test_globals)

# ===========================================================================
#  W3 config/DB
# ===========================================================================
# --- test_config_db: REAL machine-config oracle ---------------------------
#   Parses the REAL D:\HT9045\system files (Gerneral.ini / Mot_Table.csv /
#   IO_Table.csv) through the vclcompat layer (TStringList::CommaText name-based
#   column resolution mirroring database.cpp SetIOTableNo/SetMOTTableNo) and
#   asserts the ground-truth oracle values: 45 SMC axes, MOTION_CARD_TYPE=1 /
#   IO_CARD_TYPE=2, IO_Table 15 cols / 668 rows / ISABase=0 dominant (644:24).
#   Runtime-SKIPs (exit 0) when the real config dir is absent.
#   NOTE: the production readers (cinitial/database.cpp) are not translated yet
#   (later W3); this test verifies the vclcompat foundation + locks the oracle.
add_executable(test_config_db
    test_config_db.cpp
)

target_link_libraries(test_config_db PRIVATE vclcompat)

add_test(NAME config_db COMMAND test_config_db)

# --- test_IniFiles: TIniFile / TMemIniFile shim semantics ------------------
#   W3-cont CONFIG-READERS GATEWAY. Verifies BCB6 ini semantics the common.cpp
#   ini-helper family relies on: default fallback, case-insensitive match,
#   integer/float/bool/datetime parse + '%0.4f' double round-trip, section/key
#   existence, TMemIniFile flush-on-UpdateFile, Big5 byte preservation, plus a
#   runtime-SKIP real-config oracle over D:\HT9045\system\Gerneral.ini.
add_executable(test_IniFiles
    test_IniFiles.cpp
)

target_link_libraries(test_IniFiles PRIVATE vclcompat)

add_test(NAME IniFiles COMMAND test_IniFiles)

# --- test_ini_helpers: translated common.cpp *General* helper chain ---------
#   END-TO-END: drives OpenGeneralIniFile() + CheckAndReadIniDataGeneral(int/
#   bool/AnsiString) over a SCRATCH COPY of the REAL D:\HT9045\system\Gerneral.ini
#   (copied so seeding writes never touch the read-only real file) and asserts
#   the known-truth oracle ([System] MOTION_CARD_TYPE=1 / IO_CARD_TYPE=2 /
#   TTL_CARD_TYPE=2 / INDEX_MOTION_CARD=0, [TempCtrl] HEATER_CTRL_TYPE=4) plus
#   missing-key -> supplied-default.  Also a direct TIniFile/TMemIniFile shim
#   round-trip (write-through vs flush-on-UpdateFile, %0.4f double round-trip).
#   Links ht9045_core for the translated common.cpp helpers + asGeneralPath
#   global (ht9045_core PUBLIC-links vclcompat + ht9045_public transitively).
#   The real-config block runtime-SKIPs (exit 0) when the ini is absent.
add_executable(test_ini_helpers
    test_ini_helpers.cpp
)

target_link_libraries(test_ini_helpers PRIVATE ht9045_core)

add_test(NAME ini_helpers COMMAND test_ini_helpers)

# ===========================================================================
#  W3-cont2 config-table LOADERS (translated database.cpp)
# ===========================================================================
# --- test_config_loaders: drive the TRANSLATED LoadMotData / LoadIoData ----
#   Exercises the ACTUAL translated production loaders (not a re-impl):
#     HSys.LoadMotData() -> SetMOTTableNo (name-based) -> TMOTDATA -> MotTable
#     HSys.LoadIoData()  -> SetIOTableNo  (name-based) -> TIODATA  -> IOTable
#   over the REAL D:\HT9045\system\{Mot_Table,IO_Table}.csv and asserts the
#   ground-truth oracle: Mot 45 rows all CardModel==SMC (CardModel NAME-resolved
#   to physical col 22, not enum fallback 9); M00/MInArmX BoardID==0/Port==0;
#   IO 668 rows, 15 cols, ISABase NAME-resolved to col 8, distribution 644:24.
#   Links ht9045_db (database.cpp) which PUBLIC-links vclcompat + ht9045_globals
#   (INDEX_MOTION_CARD, TQPF_Timer).  The TU supplies the two gated externals
#   (MyDBIProcess / ShowMyMessage) as local stubs.  Runtime-SKIPs (exit 0) when
#   the real config files are absent.
add_executable(test_config_loaders
    test_config_loaders.cpp
)

# Link map:
#   ht9045_db      -- the translated database.cpp loaders.
#   ht9045_core    -- defines IoTablePath / MotTablePath (common.cpp), which the
#                     loaders hardcode/assign (database.cpp:1552 / 1620).
#   ht9045_globals -- INDEX_MOTION_CARD (cmydef.cpp) + the queue-object ctors
#                     (cpublic.cpp).  cmydef.cpp.obj's static queue globals
#                     (QueueTaskList[] etc.) need ctors from cpublic.cpp.obj in
#                     the SAME archive; GNU ld's single-pass archive scan can't
#                     back-resolve that, so wrap the libs in a LINK_GROUP to make
#                     ld re-scan the archive set until all refs settle.
target_link_libraries(test_config_loaders PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_db,ht9045_core,ht9045_globals>"
)

add_test(NAME config_loaders COMMAND test_config_loaders)

# ===========================================================================
#  W4 HAL motor layer
# ===========================================================================
# --- test_motor_w4: HTMotor / TMySimMotor / TMyMotor / TTrayMotor / MOT[] ---
#   Exercises the INTERFACE-CUT end-to-end with NO vendor SDK:
#     * TMySimMotor as HTMotor* (polymorphic dispatch, all overridden methods)
#     * TMyMotor routing through the sim (ReadPos / MotorHome state machine /
#       ScanMotorStatus / GetErrorIndex / PCIL132_* / JogP/JogN / Lock*)
#     * TTrayMotor tray data helpers (InitNewTray / HasIC / FullIC /
#       HowManyDevice / MoveTrayAllItem / SetNullIcToHasNullIc)
#     * MOT[] global array wiring (MOT[MInArmX] via sim, boundary check)
#   Links ht9045_motor which RESCAN-links ht9045_globals (cmydef/cprod/cpublic
#   ctors needed by mymotor.cpp's static iEncoderTorence / global MOT[]).
add_executable(test_motor_w4
    test_motor_w4.cpp
)

# ht9045_motor PUBLIC-links ht9045_globals; ht9045_db PUBLIC-links ht9045_globals
# and ht9045_core. The RESCAN group ensures intra-globals cross-references
# (cprod <-> cpublic <-> cmydef) are resolved across archive re-scans.
target_link_libraries(test_motor_w4 PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_motor_w4 PRIVATE ws2_32)
endif()

add_test(NAME motor_w4 COMMAND test_motor_w4)

# --- test_sim_motor: HARDWARE INTERFACE-CUT acceptance proof ----------------
#   Dedicated, self-contained proof that the offline motor HAL works with NO
#   vendor SDK and NO hardware.  Creates a TMySimMotor via HTMotor* (and through
#   TMyMotor) and asserts the exact acceptance contract:
#     InitMotor ok; HomeObject()->HomeFlag() true & ReadPos()==0;
#     MoveToPos(N)->MotionDone() true & ReadPos()==N; GetAlarm()==false; Stop() ok.
#   Plus a full MotorHome() state-machine convergence over the sim.
#   Same RESCAN link group as test_motor_w4 (cprod<->cpublic<->cmydef ctors must
#   resolve across archive re-scans).  The brand drivers are NOT in ht9045_motor,
#   so this links with zero vendor libraries.
add_executable(test_sim_motor
    test_sim_motor.cpp
)

target_link_libraries(test_sim_motor PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_sim_motor PRIVATE ws2_32)
endif()

add_test(NAME sim_motor COMMAND test_sim_motor)

# ===========================================================================
#  W4-IO HAL IO layer
# ===========================================================================
# --- test_lane_io_sim: IO HARDWARE INTERFACE-CUT acceptance proof -----------
#   Dedicated, self-contained proof that the offline IO HAL works with NO
#   vendor SDK and NO hardware.
#   Part A: TSimIOBackend direct round-trip (WriteBit/ReadBit/WriteByte/ReadByte).
#   Part B: TLaneIO facade: IOBitOn->IOOutBitStatus cache; IOByteOut returns true;
#           IOInputBit round-trip through TSimIOBackend; CheckPortRangeErr==0
#           when InitialOK==false or iISABase!=eMotionNet; GetIOErrStr 0..5.
#   Part C: TMySwitch.On() sets OutPortData via MyLaneIO; Status() reads it back.
#   Part D: TMySensor.IsOn()/IsOff() round-trip through TSimIOBackend; Type
#           inversion; Enable==false -> false; CopySensor field copy.
#   Same RESCAN link group as test_motor_w4 (cprod<->cpublic<->cmydef ctors).
#   Links with zero vendor libraries (all HAVE_xxx default OFF).
add_executable(test_lane_io_sim
    test_lane_io_sim.cpp
)

target_link_libraries(test_lane_io_sim PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_lane_io_sim PRIVATE ws2_32)
endif()

add_test(NAME LaneIOSim COMMAND test_lane_io_sim)

# --- test_sim_io: DEDICATED Sim-IO interface-cut acceptance proof -----------
#   Task-spec proof (mirrors test_sim_motor.cpp for the IO layer):
#     [1] select TSimIOBackend; via TLaneIO IOBitOn/IOBitOff then IOOutBitStatus
#         reflects the OutPortData cache (per-bit mask precision verified).
#     [2] IOByteOut / IOInputByte round-trip through the Sim port (IO_CARD_TYPE=1
#         MN200 read path so *Value is used directly, not the IO_CARD_TYPE==0
#         ret-quirk); OutPortData bit pattern cross-checked.
#     [3] IOInputBit returns the Sim value (write Sim bit -> read via facade).
#     [4] range/error path: drives CheckPortRangeErr documented codes 0..5
#         (InitialOK=true + iISABase=eMotionNet + iUseMNetIP device types) and
#         the InitialOK==false / iISABase!=eMotionNet early-return guards, plus
#         GetIOErrStr 0..5 string mapping.
#   Same RESCAN link group as test_lane_io_sim; zero vendor libraries (HAVE_xxx OFF).
add_executable(test_sim_io
    test_sim_io.cpp
)

target_link_libraries(test_sim_io PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_sim_io PRIVATE ws2_32)
endif()

add_test(NAME SimIO COMMAND test_sim_io)

# ===========================================================================
#  HAL enablers: KeyPro license shim + TComm serial shim
# ===========================================================================
# --- test_keypro_tcomm: offline acceptance proof for both HAL enablers ------
#   [1] KeyPro shim (Public/HTKeyProShim.cpp, in ht9045_public): asserts
#       KeyPro_GetLevel(3)==1 via the GENUINE offline fallback -- there is no
#       KeyProDLL.dll in the C++ target tree, so LoadLibraryA fails and the
#       shim returns 1 (valid dongle).  The test first asserts the DLL is
#       absent so the fallback path is the one under test.  KeyProDLL.lib is
#       NOT static-linked (the shim resolves the export at run time).
#   [2] TComm sim (vclcompat/Comm.cpp, in vclcompat): opens in SIM mode with no
#       hardware; WriteCommData buffers into the tx sink (read back + content
#       checked); SimInjectReceive drives OnReceiveData (Sender/len/exact bytes
#       verified, multi-fire + no-handler no-op + Stop/re-Start).
#   Links ht9045_public (KeyPro_GetLevel) which PUBLIC-links vclcompat
#   (Spcomm::TComm); vclcompat listed explicitly for clarity.  On Win32 the
#   shims use kernel32 serial / LoadLibrary APIs that are in the default link
#   set, so no extra libraries are required.
add_executable(test_keypro_tcomm
    test_keypro_tcomm.cpp
)

target_link_libraries(test_keypro_tcomm PRIVATE ht9045_public vclcompat)

add_test(NAME KeyProTComm COMMAND test_keypro_tcomm)

# ===========================================================================
#  cpublic.cpp foundation wave (W4/W5/W6/W7 ungated sections)
# ===========================================================================
# --- test_cpublic_foundation: acceptance proof for all ungated W4 bodies ----
#   [A] DTK4848 frame format via TComm SIM (g_pDTKComm seam):
#       DTK4848_LRC(":0106470100FA")=="B7";
#       DTK4848WordWriteNoSucm(0,250) -> SimTxBuffer==":0106470100FAB7\r\n\0";
#       DTK4848WordReadNoSucm(0) -> SimTxBuffer==":010347000002B3\r\n\0".
#   [B] ByteUnionBit: SetByte(0xA5)->Bit(0..7) assertions; BitCount(true)==4;
#       BitString()=="10100101"; Hex()=="A5"; SetBit/range-guard.
#   [C] Queue method bodies: TMyQueue10 Add/Get/Show/SetAliasAndTask/ring-wrap;
#       TMyQueue100 Add/Get; TMyStrQueue100 Add/Get; TMyTimerQueue100
#       Add/GetTimeString (ConvertMSecToSPC("00:00:01.234")).
#   [D] VerInfo: m_ClearData leaves FixedFileVersion=="0.0.0.0"; bad path
#       -> graceful no-crash; GetAppVersion on own exe no-crash.
#   [E] Foundation free functions: ConvertSecondToSPC/ConvertMSecToSPC;
#       GetFloatFormatString/ConvertToMMType/Get0_01MMType;
#       ConvertDoubleStrToInt; sDataTimelog; Round x2; RotationCoordinates;
#       CRC_Check Modbus-CRC on known vector; GetScreenPos; VC8ToKpa/KpaToVC8
#       round-trip; IsDoubleEqual.
#   Links ht9045_globals (ungated queue/VerInfo/DTK4848 bodies) PUBLIC-links
#   psapi + version (Win32 version API); vclcompat for TComm SIM.
add_executable(test_cpublic_foundation
    test_cpublic_foundation.cpp
)

target_link_libraries(test_cpublic_foundation PRIVATE ht9045_globals ht9045_public vclcompat)

add_test(NAME CpublicFoundation COMMAND test_cpublic_foundation)

# ===========================================================================
#  W6.1 CANARY -- asendic_Empty (Empty-tray stack feed) state machine
# ===========================================================================
# --- test_w6_1_empty_canary: drive the first ROOT SM over the W6 substrate ---
#   Proves the W6 decoupling stack links + RUNS with NO hardware:
#     ht9045_sm  -- the Empty-tray SM (asendic_Empty) + csystem predicates +
#                   FormsFacade (fAGV) + asendic sim helpers + canary_support.
#     ht9045_io  -- Cylinder[] / SW[] / Sen[] / MyLaneIO (TSimIOBackend default).
#     ht9045_motor -- MOT[] (TTrayMotor sim).
#     ht9045_db  -- database.cpp (provides ShowMyMessage's sibling-TU agreement
#                   + pulled into the RESCAN group for cross-archive ctors).
#     ht9045_globals -- Prod/IniConfig/CosFunction/TrayForm/RunInfo/Ld_UldDelayTime/
#                   TrayID + the enum/const universe (DUMMY/NULL_IC/K_RETRY/...).
#   The RESCAN link group lets GNU ld re-scan the archive set so the
#   cprod<->cpublic<->cmydef ctor cross-references settle (same idiom as the
#   W4 motor/IO tests).  Drives DoAutoEmpty/DoAutoEmpty1/DoLoadNewEmptyTrayToCar
#   in DUMMY for hundreds of iterations and asserts no crash + sane cursors.
add_executable(test_w6_1_empty_canary
    test_w6_1_empty_canary.cpp
)

target_link_libraries(test_w6_1_empty_canary PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_w6_1_empty_canary PRIVATE ws2_32)
endif()

add_test(NAME W6_1_EmptyCanary COMMAND test_w6_1_empty_canary)

# ===========================================================================
#  W6 VERIFY -- dedicated predicate-API + canary-stepper acceptance proof
# ===========================================================================
# --- test_w6_canary: the task-mandated W6 verify TU -------------------------
#   Companion to test_w6_1_empty_canary above.  Where that one proves the stack
#   LINKS + does not crash, this one pins two concrete contracts:
#     PART A -- PREDICATE: set a KNOWN Sim HAL state (Sen[] sensors via
#       TSimIOBackend + MOT[] tray flags) and assert the frozen csystem.h
#       HasIC-family predicates return the EXPECTED bool -- the conservative
#       gated-leaf baseline (all false), the LIVE InputShuttleRearHasIC /
#       OutputShuttleRearHasIC IniConfig/TestIF_File guard branches (both ways),
#       the composite OR-aggregation shape, and predicate PURITY (no HAL perturb).
#     PART B -- CANARY: set iAutoEmptyTask to the ENTRY state (1), pump
#       DoAutoEmpty() over the Sim HAL, and assert the stepper ADVANCES out of
#       entry and CONVERGES back to the terminal/recurring state (Task==1) while
#       staying on the documented cursor set; then drives DoLoadNewEmptyTrayToCar()
#       to convergence (lifter + Cylinder[] HAL) and DoAutoEmpty1()/fAGV.
#   Links the SAME orchestration stack as the W6.1 canary: ht9045_sm IS the W6
#   orchestration lib (asendic_Empty + csystem_predicates + FormsFacade + the
#   asendic/canary_support sim helpers); it links the substrate (globals/motor/
#   io/vclcompat) with NO vendor SDK and NO TfMain.  Re-using it (vs. a second
#   lib over the same .cpp set) avoids duplicate-symbol ODR conflicts at link.
#   The RESCAN link group lets GNU ld settle the cprod<->cpublic<->cmydef ctor
#   cross-references (same idiom as the W4 motor/IO tests).
add_executable(test_w6_canary
    test_w6_canary.cpp
)

target_link_libraries(test_w6_canary PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_w6_canary PRIVATE ws2_32)
endif()

add_test(NAME W6_Canary COMMAND test_w6_canary)

# ===========================================================================
#  W6.2 VERIFY -- shared InArm HotPlate pick/place geometry leaves
# ===========================================================================
# --- test_w6_2_inarm_canary: drive the shared HP-geometry leaf library -------
#   Proves ainarm_SearchPickPlate.cpp + ainarm_SearchPlacePlate.cpp +
#   aHotPlateSubstrate.cpp (all in ht9045_sm) LINK + RUN with NO hardware, and
#   pins faithful geometry-leaf values over a KNOWN Sim substrate:
#     GetHotPlateColStep / GetInArmToHPYPitch_9045 / GetHeaterWaitTime /
#     HotPlateYPitchCanPutAll / Row2CanPutHP / ArmRow1AllHasIC, plus the SM
#     DoInArmPickFromHotPlate_9045() converging to entry over the empty offline
#     team list, the W7-gated DoPlaceToHotPlate_9045() stub returning false, and
#     the DoPlaceToHPSwapData / DoPlaceToHPBackupData data-swap leaves running
#     without crash.  Same RESCAN link group as the W6.1 canary so the
#     cprod<->cpublic<->cmydef ctor cross-references settle.
add_executable(test_w6_2_inarm_canary
    test_w6_2_inarm_canary.cpp
)

target_link_libraries(test_w6_2_inarm_canary PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_w6_2_inarm_canary PRIVATE ws2_32)
endif()

add_test(NAME W6_2_InArmCanary COMMAND test_w6_2_inarm_canary)

# --- test_w6_2_inarm_search: drive the Search/Do functions with golden oracle -
#   The task-mandated W6.2 *search* verify TU (companion to the canary above).
#   Sets a KNOWN Sim HAL state and drives the SearchPickPlate/SearchPlacePlate
#   Do*/Search*/geometry functions, asserting the HP-geometry RESULT and the SM
#   stepping against the GOLDEN FORMULA as oracle:
#     GetPlaceToHotPlateCol (XDiv 4/6/8/16 default-picker branches),
#     GetPlaceToHotPlateSuckCol (default + 1x1), GetInArmToHPOffset_9045
#     (AutoAlignment/SameOne/iP branches -> InOfsHP1/HP2/InSh1/-1),
#     GetVariableYInHotPlateData (manual pitch -> TestIF.iARM_Y_PITCH),
#     GetVariableXInHotPlateData (1x1 -> iXpitchMaxX3 + bPitchOver12000=false),
#     ResetHotPlateSearchParameter post-state, the DoInArmPickFromHotPlate_9045
#     SM stepping over the Sim HAL, and SearchPlateToPlace /
#     SearchPlacePlateXItem4_8Suck / CheckHasSpaceToPlace_9045 pumped without
#     crash.  Same RESCAN link group as the W6.1/W6.2 canaries so the
#     cprod<->cpublic<->cmydef ctor cross-references settle.
add_executable(test_w6_2_inarm_search
    test_w6_2_inarm_search.cpp
)

target_link_libraries(test_w6_2_inarm_search PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_w6_2_inarm_search PRIVATE ws2_32)
endif()

add_test(NAME W6_2_InArmSearch COMMAND test_w6_2_inarm_search)

# ===========================================================================
#  W6.2b VERIFY -- the IN-ARM ENGINE core SMs (ainarm9045.cpp)
# ===========================================================================
# --- test_w6_2_inarm_core: pump the engine's central task SMs over Sim HAL ---
#   The task-mandated W6.2b *core engine* verify TU.  Proves ainarm9045.cpp
#   (the in-arm ENGINE, in ht9045_sm) LINKS + its 3 ACTIVE central task state
#   machines PUMP over the Sim HAL with NO hardware:
#     DoInArmAdditionalFunction (iInArmAdditionalFunctionTask, golden:3209) --
#       case 1 falls through to case 100; offline selector flags gated -> the
#       else-path sets bResult=true, cursor settles at 100;
#     DoInArmIonFanGiveWay (iInArmIonFanGiveWayTask, golden:9166) -- entry
#       else-path Task->5; InSHT*InLF false guard -> settles at 5;
#     ProcessSCKARTLoadingCount (iProcessSCKARTLoadingCountTask, golden:7305) --
#       1->100->200 over the destroy/Z-safe stubs, case 200 returns true and
#       clears bPickFromLoader.
#   Each cursor is asserted to stay inside the documented switch(Task) state set
#   and to match the golden entry/early-out (golden line cited per assert), plus
#   the geometry/numeric ORACLEs: GetInArmPitch_9045 interpolation endpoints +
#   float->int TRUNCATION (5000.625 -> 5000), GetJStep default mode == 1,
#   FindCentorPointIndex, GetShtStartPos.
#   Same RESCAN link group as the W6.1/W6.2 canaries so the
#   cprod<->cpublic<->cmydef ctor cross-references settle.
add_executable(test_w6_2_inarm_core
    test_w6_2_inarm_core.cpp
)

target_link_libraries(test_w6_2_inarm_core PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_w6_2_inarm_core PRIVATE ws2_32)
endif()

add_test(NAME W6_2_InArmCore COMMAND test_w6_2_inarm_core)

# ===========================================================================
#  W6.2b-canary VERIFY -- the in-arm SITE-VARIANT dispatch (1x1_1 live arm)
# ===========================================================================
# --- test_w6_2b_inarm_variants: prove the variant DISPATCH over the Sim HAL ---
#   The task-mandated W6.2b-canary *variant dispatch* verify TU.  Proves the
#   in-arm ENGINE's two dispatch ladders (DoInArm_9045 / DoInArm_9045_SuckerMap)
#   route e9045_1x1_1 / e9045_1x4_1_Ac to the translated 1x1_1 variant
#   (ainarm9045_1x1_1.cpp), that the 1x1_1 master pick SM + SuckerMap builder
#   PUMP/RUN over the Sim HAL, and that ONLY the 1x1_1 arm was un-gated (a still-
#   gated iInArmType -- e9045_2x4_8 / e9045_2x8_32 -- hits the Program-Error else
#   and leaves the master cursor UNCHANGED).  Oracles (hand-derived from the
#   golden 1x1_1 source):
#     [D1] e9045_1x1_1, iArmTask=1, no IC -> single-tick 1->10->50->75 fall-through
#          (golden 1x1_1 :1770/:1861/:1871); cursor in documented set.
#     [D2] e9045_1x4_1_Ac -> SAME DoInArm_9045_1x1_1() advance (golden :4563).
#     [D3/D4] gated types -> Program-Error else, iArmTask UNCHANGED.
#     [S1] SuckerMap default branch numeric oracle: seed bUseTestSocket[0][0][0]=
#          true, iSiteMap[0][0]=7 -> Prod.fInArmSuck4x8[0][0][0]/bInSuckUse/iSiteMap
#          written + GetShuttleCol(0,0)==0 (golden :1428-1433/:507-510).
#     [S2/S3] dispatch routing + iSuckCol=1 column-shift bonus.
#   RECONCILIATION: there is NO e9045_2x4_16 enum + the golden routes e9045_2x8_32
#   -> 2x8_8, so ainarm9045_2x4_16.cpp / _2x8_32.cpp are DEAD (registered to
#   compile, never dispatched); no routing oracle is derivable for them -- they
#   serve as the "still-gated type -> Program-Error" witnesses instead.
#   Same RESCAN link group as the W6.1/W6.2 canaries + core TUs.
add_executable(test_w6_2b_inarm_variants
    test_w6_2b_inarm_variants.cpp
)

target_link_libraries(test_w6_2b_inarm_variants PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_w6_2b_inarm_variants PRIVATE ws2_32)
endif()

add_test(NAME W6_2b_InArmVariants COMMAND test_w6_2b_inarm_variants)

# ===========================================================================
#  W6.2c batch-1 VERIFY -- the 5 in-arm SITE-VARIANT batch (active arms)
# ===========================================================================
# --- test_w6_2c_inarm_variants: prove the 5-variant batch over the Sim HAL ---
#   Independently proves the W6.2c batch-1 of 5 in-arm site variants
#   (1x3_4, 1x2_2, 1x2_2_14, 1x2_4_Hot, 1x3_2_14).  Proves:
#     [D] the engine ladders (DoInArm_9045 / DoInArm_9045_SuckerMap) route each
#         of the 5 active iInArmType values to its OWN DoInArm_9045_<v>() /
#         _SuckerMap() arm (cursor advances), while a still-gated type
#         (e9045_2x4_8) hits the Program-Error else (cursor UNCHANGED) -- so only
#         the intended arms were un-gated; all 5 *_SuckerMap symbols defined+callable.
#     [O1] pure const table XPHSuckToSht_1x2_4 (golden 1x2_4_Hot:32; read by the
#          engine at ainarm9045.cpp:513) carries the exact golden values.
#     [O2] TMyKitSuck Arm{Up,Down,Left,Right}SideHaveRealIC Item-grid scans
#          (transcribed verbatim from golden mykitsuck.cpp; drive 1x2_4_Hot) match.
#     [O3] GetShuttleState_1x3_4 pick branch (golden ainarm9045_1x3_4.cpp:2965)
#          returns 0 (all NULL_IC) / 1 (left-top HAS_IC).
#   Same RESCAN link group as the W6.1/W6.2/W6.2b canaries + core TUs.
add_executable(test_w6_2c_inarm_variants
    test_w6_2c_inarm_variants.cpp
)

target_link_libraries(test_w6_2c_inarm_variants PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_w6_2c_inarm_variants PRIVATE ws2_32)
endif()

add_test(NAME W6_2c_InArmVariants COMMAND test_w6_2c_inarm_variants)

# --- test_w6_2c_inarm_variants_batch2: prove the 6-variant batch-2 over Sim HAL
#   Independently proves the W6.2c batch-2 of 6 in-arm site variants
#   (1x4_2, 1x4_8_Hot, 2x2_4_12, 2x1_2, 2x2_4, 2x2_4_14).  Proves:
#     [D]  the engine ladders (DoInArm_9045 / DoInArm_9045_SuckerMap) route each
#          of the 6 active iInArmType values to its OWN DoInArm_9045_<v>() /
#          _SuckerMap() arm (cursor advances), while a still-gated type
#          (e9045_2x4_8) hits the Program-Error else (cursor UNCHANGED); all 6
#          *_SuckerMap symbols defined+callable.
#     [O1] pure-const picker->shuttle permutation maps XPHSuckToSht_2x2_12[8] /
#          XPHSuckToSht_2x2_13[3][8] / XPHSuckToSht_2x2_14[3][8] carry the exact
#          golden values (identity / {0,2,1,3,..} / {0,2,3,1,..}).
#     [O2] DoInArm_9045_2x1_2_SuckerMap via the engine ladder: engine grid-clear
#          (bInSuckUse=false, iSiteMap=-1) + per-variant fill compose -- only
#          cols 0,2 of row 0 used, iSiteMap routed from TestIF.
#     [O3a] DoInArm_9045_2x1_2() advances the SHARED iArmTask global (int &Task).
#     [O3b] GetShuttleState_2x2_4 pick branch == 0/1/2 (golden :2965), driving
#           the substrate ArmUpSideAllTypeIC / ArmDownSideAllTypeIC twins.
#   Same RESCAN link group as the W6.1/W6.2/W6.2b/W6.2c-batch1 canaries + cores.
add_executable(test_w6_2c_inarm_variants_batch2
    test_w6_2c_inarm_variants_batch2.cpp
)

target_link_libraries(test_w6_2c_inarm_variants_batch2 PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_w6_2c_inarm_variants_batch2 PRIVATE ws2_32)
endif()

add_test(NAME W6_2c_InArmVariantsBatch2 COMMAND test_w6_2c_inarm_variants_batch2)

# --- test_w6_2c_inarm_variants_batch3: prove the 6-variant batch-3 over Sim HAL
#   Independently proves the W6.2c batch-3 of 6 in-arm site variants
#   (1x4_4, 1x4_4_Back, 2x3_6, 2x3_6_14, 2x4_4, All_1Pick).  Proves:
#     [D]  the engine ladder (DoInArm_9045) routes each of the 5 iInArmType-routed
#          active variant values to its OWN DoInArm_9045_<v>() arm via the SOUND
#          engine==direct differential witness; all 6 *_SuckerMap symbols (incl.
#          the faithful-never-dispatched All_1Pick_SuckerMap) defined+callable.
#     [D-pick] ORACLE 3 (load-bearing): with USE_PICKER_COUNT!=ep1Picker,
#          e9045_2x4_4_14(=20) enters DoInArm_9045_2x4_4 (NOT _2x4_4_13) and
#          e9045_2x4_4_13(=21) hits the still-GATED Program-Error else (cursor
#          UNCHANGED); with USE_PICKER_COUNT==ep1Picker the FIRST ladder arm enters
#          DoInArm_9045_All_1Pick REGARDLESS of iInArmType (witnessed against a
#          gated type so only the leading picker-count `if` can advance the cursor).
#     [O1] pure-const tables XPHSuckToSht_2x3_6 / _OutArm carry the golden swap
#          (row1 {3,1,0,2,..} in-arm / {0,2,1,3,..} out-arm).
#     [O2] pure-const XPHSuckToSht_2x4_14[3][8] all rows == {0,4,5,1,2,6,7,3}.
#     [O3] GetShuttleState_2x3_6 pick branch == 0/1/2 (golden :3505), driving the
#          substrate ArmUpSideAllTypeIC/ArmDownSideAllTypeIC over the (NULL_IC,0,4)
#          6-site scan range.
#   Same RESCAN link group as the W6.1/W6.2/W6.2b/W6.2c-batch1/batch2 canaries.
add_executable(test_w6_2c_inarm_variants_batch3
    test_w6_2c_inarm_variants_batch3.cpp
)

target_link_libraries(test_w6_2c_inarm_variants_batch3 PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_w6_2c_inarm_variants_batch3 PRIVATE ws2_32)
endif()

add_test(NAME W6_2c_InArmVariantsBatch3 COMMAND test_w6_2c_inarm_variants_batch3)

# --- test_w6_2c_inarm_variants_batch4: prove the 5-variant batch-4 over Sim HAL
#   Independently proves the W6.2c batch-4 of 5 in-arm site variants
#   (2x2_8_Hot, 2x4_8, 2x5_8, 2x6_8, 2x8_8).  Proves:
#     [D]  the engine ladder (DoInArm_9045) routes each of the 5 active variant
#          iInArmType values to its OWN DoInArm_9045_<v>() arm via the SOUND
#          engine==direct differential witness; all 5 *_SuckerMap symbols callable.
#     [D-route] golden routing fact (golden ainarm9045.cpp :4639): e9045_2x8_32
#          routes to the SAME DoInArm_9045_2x8_8 callee as e9045_2x8_8.
#     [D-gate] still-gated e9045_2x4_4_13 / e9045_1x4_4_13 -> Program-Error else,
#          cursor UNCHANGED.
#     [O1] pure-const XPHSuckToSht_2x6_6 (golden ainarm9045_2x6_8.cpp:75) standard /
#          Steven-20250123-fix / close-center-2x4 rows.
#     [O2] pure-const XPHSuckToSht_2x8_8 (golden ainarm9045_2x8_8.cpp:112) identity
#          standard + JerryYang-20190729 STM interleave.
#   Same RESCAN link group as the W6.1/W6.2/W6.2b/W6.2c-batch1/2/3 canaries.
add_executable(test_w6_2c_inarm_variants_batch4
    test_w6_2c_inarm_variants_batch4.cpp
)

target_link_libraries(test_w6_2c_inarm_variants_batch4 PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_w6_2c_inarm_variants_batch4 PRIVATE ws2_32)
endif()

add_test(NAME W6_2c_InArmVariantsBatch4 COMMAND test_w6_2c_inarm_variants_batch4)

# --- test_w6_2c_inarm_Sfamily: prove the 2 S-family in-arm variant files ----
#   Independently proves the LAST 2 in-arm dispatch arms (the former residents of
#   the DoInArm_9045 "#if 0 // TODO(W6.2b variants)" gate, un-gated in batch-5):
#     ainarm9045S_1x4_4.cpp     (e9045_1x4_4_13; IRREGULAR func name DoInArm_9045S_1x4_4 -- S IN func)
#     ainarm9045S_2x4_4_13.cpp  (e9045_2x4_4_13; func DoInArm_9045_2x4_4_13 -- NO S)
#   Proves:
#     [D1/D2] BOTH engine ladders (DoInArm_9045 / DoInArm_9045_SuckerMap) route each
#          S-family iInArmType to its OWN IRREGULAR-named callee via the SOUND
#          engine==direct differential witness; neither hits the Program-Error else.
#     [O2] pure-const XPHSuckToSht_1x4_13 (golden ainarm9045S_1x4_4.cpp:32-33) +
#          XPHSuckToSht_2x4_13 (golden ainarm9045S_2x4_4_13.cpp:32-34).
#     [O3] picker-count witness: SetInOutArmParameter_1x4_4_13 -> SetPickerCount
#          (2,2,1,4,2,0,0); SetInOutArmParameter_2x4_4_13 iShtCol 4 (non-special) /
#          8 (_16Site2X8) -> SetPickerCount(2,2,2,{4|8},2,2,2).
#   Same RESCAN link group as the W6.1/W6.2/W6.2b/W6.2c-batch1..4 canaries.
add_executable(test_w6_2c_inarm_Sfamily
    test_w6_2c_inarm_Sfamily.cpp
)

target_link_libraries(test_w6_2c_inarm_Sfamily PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_w6_2c_inarm_Sfamily PRIVATE ws2_32)
endif()

add_test(NAME W6_2c_InArmSfamily COMMAND test_w6_2c_inarm_Sfamily)

# ===========================================================================
#  W6.2c VERIFY -- the OUT-ARM ENGINE core SMs (aoutarm9045.cpp)
# ===========================================================================
# --- test_w6_2_outarm_core: pump the OUT-ARM central task SMs over Sim HAL ---
#   The task-mandated W6.2c *core engine* verify TU (DIRECT mirror of the in-arm
#   core TU above).  Proves aoutarm9045.cpp (the out-arm ENGINE, in ht9045_sm)
#   LINKS + its ACTIVE central task state machines PUMP over the Sim HAL with NO
#   hardware:
#     DoOutArmAdditionalFunction (iOutArmAdditionalFunctionTask, golden:2246) --
#       case 1 FALLS THROUGH to case 100; the ACTIVE MoveOutArmToAutoSafe() stub
#       returns true and the Rotate/AOI/FixAI branches are W7-gated -> the
#       else-path sets bResult=true on the FIRST tick, cursor settles at 100;
#     DoOutArmIonFanGiveWay (iOutArmIonFanGiveWayTask, golden:4613) --
#       (2a) ESD-off PRE-SWITCH guard returns true + forces cursor==1 (golden
#            1312-1316); (2b) ESD-on entry case 1 with empty carry kits + no real
#            IC -> inner-else Task 1->2 (golden 1332); the full pump stays inside
#            the documented {1,2,3,5} set (no escape / no crash).
#   Each cursor is asserted to stay inside the documented switch(Task) state set
#   and to match the golden entry/early-out (golden line cited per assert), plus
#   the geometry/numeric ORACLEs: GetOutArmPitch_9045 interpolation endpoints +
#   float->int TRUNCATION (5000.125 -> 5000) + GetOutArmPitchY_9045 zero-guard
#   (denom==0 -> iOutArmY15Pitch, no div-by-zero).
#   This TU reaches the IonFan case-1/case-5 OutSHT1InRT/OutSHT2InRT predicates,
#   which had no offline home before this wave; csystem_predicates.cpp now
#   supplies them (delegating to InSHT*InRT, golden csystem.cpp:699/701).
#   Same RESCAN link group as the W6.1/W6.2 canaries so the
#   cprod<->cpublic<->cmydef ctor cross-references settle.
add_executable(test_w6_2_outarm_core
    test_w6_2_outarm_core.cpp
)

target_link_libraries(test_w6_2_outarm_core PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_w6_2_outarm_core PRIVATE ws2_32)
endif()

add_test(NAME W6_2_OutArmCore COMMAND test_w6_2_outarm_core)

# --- test_w6_2_outarm_variants: prove the LIVE out-arm site variants DISPATCH ---
#   W6.2c-OUT batch (1x1_1/1x2_2/1x2_4/1x3_2_14/1x3_4/1x4_2).  Independent witness
#   that the engine dispatch arm for each layout resolves to the REAL translated
#   DoOutArm_9045_<v>() body (in ht9045_sm) -- NOT the removed file-local no-op
#   `static void ...(){}` stub.  Each variant's golden task-1 entry writes the
#   global bSortingAllBinTrayFinish=false and advances OutArmTask off 1 (case
#   1->5->10->50 via the offline MoveOutArmToAutoSafe()/CheckOutArmInitState()
#   stubs); the deleted no-op stub would have left both untouched.  Plus the
#   engine GetOutArmPitch_9045 interpolation oracle as a link-regression guard.
#   Same RESCAN link group as test_w6_2_outarm_core so the cprod<->cpublic<->
#   cmydef ctor cross-references settle and ht9045_sm carries the variant bodies.
add_executable(test_w6_2_outarm_variants
    test_w6_2_outarm_variants.cpp
)

target_link_libraries(test_w6_2_outarm_variants PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_w6_2_outarm_variants PRIVATE ws2_32)
endif()

add_test(NAME W6_2_OutArmVariants COMMAND test_w6_2_outarm_variants)

# --- test_w6_3_catchtray_core: pump the TRAY-ARM central task SMs over Sim HAL ---
#   The task-mandated W6.3 *core engine* verify TU (mirror of the out-arm core TU).
#   Proves acatchtray.cpp (the TrayArm ENGINE, in ht9045_sm) LINKS + its ACTIVE
#   task SMs PUMP over the Sim HAL with NO hardware:
#     DoSlapTray (iSlapTrayTask, golden:5877) -- the simplest fully-pumpable
#       sub-SM; DoSlapTray(true) re-inits to 1; the pump stays inside the
#       documented {1,5,10,20,50,100,200,300,400,450,500} set (golden:5889);
#     DoCatchTray (CatchTrayTask, golden:6000) -- the central dispatcher pumps
#       from entry without crash, cursor stays in a sane range;
#     WhichAutoNeedTray() (golden:410) ORACLE -- the real engine body (NOT the
#       deleted canary stub) returns 0 on the Clean-Out/no-IC path (golden:463)
#       and on the Tray-Feed/no-IC path (golden:417).
#   Same RESCAN link group as the W6.1/W6.2 canaries + core TUs.
add_executable(test_w6_3_catchtray_core
    test_w6_3_catchtray_core.cpp
)

target_link_libraries(test_w6_3_catchtray_core PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_w6_3_catchtray_core PRIVATE ws2_32)
endif()

add_test(NAME W6_3_CatchTrayCore COMMAND test_w6_3_catchtray_core)

# --- test_w6_3_catchtray: TrayArm ENGINE acceptance proof (5 golden oracles) -
#   The task-mandated W6.3 acceptance verify TU (COMPANION to the core TU above;
#   mirror of the in/out-arm core verifies).  Sets a KNOWN Sim HAL state
#   (CatchTraySuck items, the per-SM entry cursors, MOT[].Position/Led via the
#   offline Motor==NULL knob, Prod tray-X geometry, iPauseBackUp) and asserts the
#   FIVE hand-derived golden oracles from the task brief:
#     [O1] DoSlapTray 200->300 FALL-THROUGH in one tick (golden 5925-5929 //break;)
#          + init guard (5883-5887) + case1->5 (5891-5897);
#     [O2] DoCatchFromLoader 1->10 FALL-THROUGH: one call from Task=1 with
#          MTrayXCanSafeMove()==true lands at 50 via case10 else (golden 1499-1539);
#     [O3] WhichAutoNeedTray()==0 on the Tray-Feed (golden 417) + Clean-Out
#          (golden 463) deterministic short-circuit paths;
#     [O4] WhichAutoNeedTray() AMR i+1 path documented as offline-unreachable
#          (TfAGV::Use_AMR()==false, FormsFacade.cpp:24) -- pinned, not faked;
#     [O5] DoCatchTray entry guard: iPauseBackUp!=-1 -> cursor unchanged
#          (golden 6015-6019); iPauseBackUp==-1 -> case1 eUnderCoveyor dispatch
#          Task=50 (golden 6076-6079).
#   Plus BONUS geometry oracles (IsTrayArmAtEmptyOrColor golden 7992-8004 /
#   IsTrayArmMoveAvoidOutArmCrash golden 7979-7990) and DoSlapTray + DoCatchTray
#   cursor-universe pumps (no escape / no crash).
#   Same RESCAN link group as the W6.1/W6.2 canaries + core TUs.
add_executable(test_w6_3_catchtray
    test_w6_3_catchtray.cpp
)

target_link_libraries(test_w6_3_catchtray PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_w6_3_catchtray PRIVATE ws2_32)
endif()

add_test(NAME W6_3_CatchTray COMMAND test_w6_3_catchtray)

# --- test_w6_4_tester: pump the TESTER/INDEX (test-head) engine over Sim HAL ---
#   The task-mandated W6.4 verify TU (mirror of the in/out/tray-arm core TUs).
#   Proves atester.cpp (the TESTER/INDEX ENGINE, in ht9045_sm) LINKS + its ACTIVE
#   core test-head task SMs PUMP over the Sim HAL with NO hardware, and that the
#   geometry/numeric oracles match values hand-derived from the golden formula:
#     [SM1] DoTestY()         iTestYTask 1 -> 50 (golden :4890) -> 100 FALLS
#           THROUGH to 110 in one tick (golden :5180 no-break); cursor stays in
#           the documented switch(Task) set under a 128-tick pump.
#     [SM2] DoTestHeadMotor()  from case 4 (bC02InstallCCD==false) walks
#           4 -> 9 -> 10 -> 15 over Sim HAL (offline Motor==NULL -> Gali Z/Y moves
#           complete + ISNormal()==true); cursor stays in the documented set.
#     [N3]  ArrayConvertSite(1,2)@_8Site2X4==6 / (1,1)@DualSite2x1==3 / (0,2)@DualSite==2.
#     [N4]  GetRowCol@_8Site2X4 -> 2,4 ret4 / @SingleSite -> 1,1 ret0 / @QualSite2X2 -> 2,2 ret3.
#     [N5]  GetIndexZSpeed INTEGER division: !slowdown -> GailSpeed; slowdown scale=30
#           -> 50000*30/100==15000; scale=0 clamp 1 -> 500 (sim HTMotor* attached,
#           then detached to restore the Motor==NULL offline knob).
#   Same RESCAN link group as the W6.1/W6.2/W6.3 canaries + core TUs.
add_executable(test_w6_4_tester
    test_w6_4_tester.cpp
)

target_link_libraries(test_w6_4_tester PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_w6_4_tester PRIVATE ws2_32)
endif()

add_test(NAME W6_4_Tester COMMAND test_w6_4_tester)

# --- test_w6_4_tester_core: the SALVAGED RESULT-DECODE ANCHOR concrete oracle -
#   The task-mandated W6.4a *decode-anchor* verify TU (companion to the SM/
#   geometry W6_4_Tester above).  Pins the high-value fMain-free TTL bin-decode
#   trio that is the reason the partial atester.cpp was salvaged, all ACTIVE
#   (no #if 0): GetBinaryData (golden :670) / GetOneByOneData (golden :621) fed
#   by Test_GetTestResulSub/Test_GetTestResul (golden :759/:789) + ArrayConvertSite
#   (golden :504).  Asserts the CONCRETE ORACLE hand-derived from the golden
#   formula (HexData={1,2,4,8,16}, IOCT[_5Bit=4]=5):
#     [O1] Binary  _5Bit/CHBinary  iSite=0 ScanPort[0]&[2] -> 1+4 == 5;
#     [O2] OneByOne _5Bit/CHOneByOne iSite=0 only ScanPort[2] -> idx2+1 == 3;
#     [O3] OneByOne ScanPort[1]&[2] -> sum!=1 -> iTestBinCount == 16;
#   plus the decode CONTRACT corroborators (Sum==0 -> -1; no-bit -> -1; full
#   weight table -> 31; Data=iSite*5 site offset) and the Test_GetTestResulSub
#   CHBinary/CHOneByOne dispatch driven THROUGH the Sim HAL Sen[] read path
#   (Sen[SenBit0+0..4] mapped onto an injected TSimIOBackend, SOFT_SIMULTE not
#   defined so the #ifndef Sen-repopulate block is live).
#   Same RESCAN link group as the W6.1/W6.2/W6.3/W6.4 canaries + core TUs.
add_executable(test_w6_4_tester_core
    test_w6_4_tester_core.cpp
)

target_link_libraries(test_w6_4_tester_core PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_w6_4_tester_core PRIVATE ws2_32)
endif()

add_test(NAME W6_4_TesterAnchor COMMAND test_w6_4_tester_core)

# --- test_w6_4b_front_rear_destroy: Front (Z1/Arm1) / Rear (Z2/Arm2) "Destroy IC"
#   down-press SM pair -- W64b-Translate wave (aTester_Front.cpp/aTester_Rear.cpp),
#   now real translations (no longer atester_shims.cpp offline stubs).  Asserts:
#     (a) Init*TestDestroyICTask() -> cursor=1, bZ1/2Isdownflag=false (golden :300-305).
#     (b) CheckZ1/2IsDown()==true and TestZ1/2OutRandge()==false under the same
#         MOT[axis].Motor->Enable==false offline convergence knob test_w6_4_tester.cpp
#         uses (sim HTMotor attached, default Enable=false).
#     (c) DoFrontTestDestroyIC()/DoRearTestDestroyIC() PARK at Task=100 (case1
#         OutSHT1/2InLF()==false -> Task=100; case100's Gali_MotMove() stub,
#         mymotor.cpp:920 TODO(W6-Galil), unconditionally returns false so the SM
#         cannot progress further offline -- a pre-existing Galil-stub limitation,
#         not a W64b defect) + a 128-tick pump-sanity check (cursor never escapes
#         the documented switch(Task) label set).
#   Same RESCAN link group as the W6.4 Tester canaries.
add_executable(test_w6_4b_front_rear_destroy
    test_w6_4b_front_rear_destroy.cpp
)

target_link_libraries(test_w6_4b_front_rear_destroy PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_w6_4b_front_rear_destroy PRIVATE ws2_32)
endif()

add_test(NAME W6_4b_FrontRearDestroy COMMAND test_w6_4b_front_rear_destroy)

# --- test_w6_5_shuttle: pump the SHUTTLE/CARRY engine over Sim HAL -----------
#   The task-mandated W6.5 verify TU (mirror of the in/out/tray-arm + tester
#   core TUs).  Proves acarry.cpp (the SHUTTLE/CARRY ENGINE, in ht9045_sm)
#   LINKS + its ACTIVE core shuttle task SM PUMPS over the Sim HAL with NO
#   hardware (136/136 case labels ACTIVE, 0 #if 0 gates), and that the
#   geometry/numeric/zero-guard oracles match values hand-derived from the
#   golden formula:
#     [O1] Do_Auto_SHT1()  AutoSHT1Task 1 -> 10 from entry over the Sim HAL
#          (case 1 final Task=10, golden :3342); cursor stays in the documented
#          switch(Task) universe {1,10,100,105,110,115,120,124,125,130,135,140,
#          200,201,202,203,210,211,212,220,225,230,235,250,251,252,300,400,500,
#          2300,2400,2500,2600,3000,3100,3101,3102,5000,5100} under a 64-tick pump.
#     [O3] GetShuttle1Pitch_9045 double->int TRUNCATION (golden :7540):
#          w=12000 -> 8000; w=18000 -> 14000; i1180=14003,w=15000 -> r=11001.5
#          -> (int)==11001 (toward-zero, NOT 11002).
#     [O4] CheckShuttleOutputHasICError zero-detect-pos guard (golden :2076-2097):
#          Tech.*DetectPos==0 offline -> returns false before the residual switch
#          even with bShuttleHasIC flagged (no spurious residual alarm).
#     [O5] CheckShuttleSensorBroken_1(_,false) left pass: iCheckCount==0 skips the
#          bRight error-aggregation arm -> returns false (golden :7322-7360).
#   Same RESCAN link group as the W6.1/W6.2/W6.3/W6.4 canaries + core TUs.
add_executable(test_w6_5_shuttle
    test_w6_5_shuttle.cpp
)

target_link_libraries(test_w6_5_shuttle PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_w6_5_shuttle PRIVATE ws2_32)
endif()

add_test(NAME W6_5_Shuttle COMMAND test_w6_5_shuttle)

# --- test_w6_6_hub: pump the csystem ORCHESTRATION HUB over the Sim HAL -------
#   The task-mandated W6.6 verify TU.  Proves csystem.cpp (the integration HUB,
#   in ht9045_sm) LINKS against the five engines + its DoAllProcess main loop
#   PUMPS the WHOLE machine cycle over the Sim HAL with NO hardware, ticking the
#   engines in the documented 2-phase order (the bDoProcess interleave), and that
#   the tick-sequence ORACLE (g_csystemTrace, via CSYSTEM_TICK_ORACLE on
#   ht9045_sm) confirms the exact engine order:
#     [O1] TICK A = SensorScan,DoLoad,DoInArm,SHT1,TestHead,CatchTray
#          TICK B = SensorScan,DoLoad,SHT2,TestHead,CatchTray,DoOutArm (golden 9115-10255)
#     [O2] DoInArm only on A, DoOutArm only on B, SHT1/SHT2 mutually exclusive
#     [O3] MainProc alive-instrumentation (CallCount++/IsMainProcAlive) (golden 16677-16729)
#     [O4] 64-cycle pump stability + sustained A/B alternation, no crash
#   Same RESCAN link group as the W6.1..W6.5 canaries + core TUs.
add_executable(test_w6_6_hub
    test_w6_6_hub.cpp
)

target_link_libraries(test_w6_6_hub PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_w6_6_hub PRIVATE ws2_32)
endif()

add_test(NAME W6_6_Hub COMMAND test_w6_6_hub)

# --- test_w6_6_csystem_cycle: the INTEGRATED MACHINE CYCLE pumps over Sim HAL --
#   The task-mandated W6.6 *cycle* verify TU (companion to test_w6_6_hub).  Sets a
#   KNOWN Sim HAL state, pumps the main process loop fn (DoAllProcess/MainProc)
#   for N=64 cycles, and asserts -- against values HAND-DERIVED from the golden
#   csystem.cpp -- that the integrated cycle ticks the FIVE engines in the
#   documented order without crash, the entry/guard behaviour matches the golden,
#   and the numeric/cursor oracles hold:
#     [O1] 4-tick (T,F,T,F) engine-order stream + invariants (golden :9115-10255)
#     [O2] MainProc instrumentation: CallCount 0->5, alive flips (golden :16687-16728)
#     [O3] master-guard early-return emits ONLY [SensorScan], zero engines (golden :10049)
#     [O4] 64-cycle pump: clean A/B each tick + strict bDoProcess toggle, no crash (golden :10254)
#   Same RESCAN link group as test_w6_6_hub (ht9045_sm carries the CSYSTEM_TICK_ORACLE
#   trace; the other archives settle the cprod<->cpublic<->cmydef ctor cross-refs).
add_executable(test_w6_6_csystem_cycle
    test_w6_6_csystem_cycle.cpp
)

target_link_libraries(test_w6_6_csystem_cycle PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_w6_6_csystem_cycle PRIVATE ws2_32)
endif()

add_test(NAME W6_6_CSystemCycle COMMAND test_w6_6_csystem_cycle)

# ===========================================================================
#  W7-A1 VERIFY -- the in-arm shuttle FLOATING + LATCH pair (un-gated)
# ===========================================================================
# --- test_w7_a1_inarm_floating_latch: pump the un-gated PAIR over Sim HAL ----
#   Proves the two W7-A1 un-gated in-arm SMs in ainarm9045.cpp (ht9045_sm) LINK
#   and converge over the Sim HAL with NO hardware:
#     CheckInShuttleSensor_Latch  (golden ainarm9045.cpp:3810-4174) -- the latch
#       interlock; the VERBATIM entry guard (MOTION_CARD_TYPE!=Contec ||
#       iRealDummy!=REALLY) self-disables it to return 1 (Finish) BEFORE any
#       CCLink/Ltc/MOT body runs offline (sim canary iRealDummy==DUMMY).
#     DoInArmCheckShuttleFloating (golden ainarm9045.cpp:3349-3757) -- the
#       place-to-shuttle floating quality SM; with empty Sim grids it converges
#       1 -> 9000 -> (Task=1, bResult=true) over the MoveInArmZToPlateSafe stub +
#       the FLCarryKit/BLCarryKit HasRealIC()/UseSiteHasIC() empty-grid predicates.
#   Oracles (golden-cited): (a) latch self-guard -> 1, no cursor advance/MOT touch;
#   (b) floating convergence + case-9000 side-effects (IndexZCanMove[0/1]=true,
#   bShuttleKnock=false, cInplace->bNo9Action=false); (c) no-floating-no-IC takes
#   the no-alarm branch (never enters the shake/vibrate/knock loop nor the latch).
#   Same RESCAN link group as the other W6 in-arm tests (ht9045_sm carries the
#   floating/latch bodies + the W7-A1 shuttle-motion shims).
add_executable(test_w7_a1_inarm_floating_latch
    test_w7_a1_inarm_floating_latch.cpp
)

target_link_libraries(test_w7_a1_inarm_floating_latch PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_w7_a1_inarm_floating_latch PRIVATE ws2_32)
endif()

add_test(NAME W7_A1_InArmFloatingLatch COMMAND test_w7_a1_inarm_floating_latch)

# --- test_w7_s0_motor_convergence: OFFLINE move-family CONVERGENCE enabler ---
#   Proves the W7 S0 change in Motor/mymotor.cpp makes the offline move family
#   ARRIVE over the Sim HAL (Motor==NULL convergence shortcut):
#     (a) CONVERGE  -- MotorMove(N) -> 1 (Move Success) AND ReadPos()==N (was a
#                      no-op stub returning 0/Moving forever before S0);
#     (b) PICKER    -- MotorMove2SpeedForPicker(FinalPos,NULL) -> true & arrives;
#     (c) HOME      -- MotorHome offline still converges Position->0 (regression
#                      guard; S0 did NOT touch the home path);
#     (d) FAST-PATH -- Gali_Two_ZAxis_Move / ISNormal / GalilTwoY_Move stay true
#                      offline (atester one-liners; NOT part of the move family).
#   Same RESCAN link group as test_sim_motor / test_motor_w4 -- only the motor
#   layer is exercised (no state-machine archive needed), zero vendor libraries.
add_executable(test_w7_s0_motor_convergence
    test_w7_s0_motor_convergence.cpp
)

target_link_libraries(test_w7_s0_motor_convergence PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_w7_s0_motor_convergence PRIVATE ws2_32)
endif()

add_test(NAME W7_S0_MotorConvergence COMMAND test_w7_s0_motor_convergence)

# ===========================================================================
#  W7 SUBSTRATE STAGING VERIFY -- the cross-arm KitSuck occupancy grid (ON)
# ===========================================================================
# --- test_w7_grid_occupancy: prove the flag-flip wired the predicates to grid --
#   Proves the W7 substrate-staging change (csystem_predicates.cpp
#   HT9045_KITSUCK_GRID_AVAILABLE 0->1 + faithful golden UseSiteHasIC()/UseSiteNoIC()
#   Item-scan bodies in aHotPlateSubstrate.cpp) makes the csystem HasIC predicate
#   family consult the REAL (seedable) TMyKitSuck Item grid instead of returning a
#   hard-coded false.
#     PART A -- UNIT: a LOCAL TMyKitSuck: fresh (all NULL_IC) -> UseSiteHasIC false /
#       UseSiteNoIC true / HasRealIC false; seed a use-site (iShtRow x iShtCol) to
#       HAS_IC + widen iMaxRow/iMaxCol -> UseSiteHasIC true / UseSiteNoIC false /
#       HasRealIC true / CountRealIC>=1; ClearAll -> UseSiteHasIC false again.
#     PART B -- END-TO-END flag-flip proof: seed the GLOBAL FLCarryKit use-site that
#       InputShuttleFrontHasIC() reads -> InputShuttleFrontHasIC()==true AND
#       ShuttleHasIC()==true; clear -> false. Second witness: seed FTestSuck ->
#       IndexHasRealIC()==true; clear -> false. Globals are saved/restored so sibling
#       suites are unaffected. With the flag OFF these leaves were a hard-coded
#       `return false`; seeding making them true proves the wiring.
#   The csystem predicates live in ht9045_sm (csystem_predicates.cpp); the TMyKitSuck
#   bodies + the kit globals live in ht9045_sm too (aHotPlateSubstrate.cpp). Same
#   RESCAN link group as the other W6/W7 state-machine tests so the
#   cprod<->cpublic<->cmydef ctor cross-references settle.
add_executable(test_w7_grid_occupancy
    test_w7_grid_occupancy.cpp
)

target_link_libraries(test_w7_grid_occupancy PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_w7_grid_occupancy PRIVATE ws2_32)
endif()

add_test(NAME W7_GridOccupancy COMMAND test_w7_grid_occupancy)

# ===========================================================================
#  W7-C1 VERIFY -- the end-of-lot CLEAN OUT drain finish-check (REAL body)
# ===========================================================================
# --- test_w7_c1_cleanout_finish: prove DoCleanOutFinishCheck does REAL work ---
#   Proves the W7-C1 change (the faithful golden DoCleanOutFinishCheck body,
#   csystem.cpp:14713-15748, now lives in the translated csystem.cpp -- replacing
#   the no-op stub) does REAL work over the now-seedable cross-arm occupancy grid:
#     (a) EMPTY machine (iCleanOut=1/iOneCycle=0/IndexStatus=Z1_Z2_Normal, all
#         kits cleared) -> the body does NOT park on the residual-IC cursor 1001;
#         clean-out can DRAIN toward finish.
#     (b) FALSE-FINISH GUARD (the new capability): seed InArmSuck use-site=HAS_IC
#         -> iCleanOutCycleTask==1001 (golden residual gate :14739) + iCleanOut
#         stays 1 (NO false instant-finish, unlike the old stub).
#     (c) DRAIN: clear the seed -> next call no longer parks on 1001.
#     (d)/(e) NO-REGRESSION: the entry guard (golden :14720-14721) gates the work
#         -- iOneCycle=1 leaves the cursor untouched, and iCleanOut=0 (the W6.6
#         hub posture) sets no clean-out drain cursor.
#   The DoCleanOutFinishCheck body + the iCleanOut/iOneCycle/iCleanOutCycleTask
#   globals + the TMyKitSuck kit globals all live in ht9045_sm (csystem.cpp /
#   cmydef / aHotPlateSubstrate.cpp).  Same RESCAN link group as the other W6/W7
#   state-machine tests so the cprod<->cpublic<->cmydef ctor cross-references
#   settle.  All seeded globals are saved/restored so sibling suites are unaffected.
add_executable(test_w7_c1_cleanout_finish
    test_w7_c1_cleanout_finish.cpp
)

target_link_libraries(test_w7_c1_cleanout_finish PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_w7_c1_cleanout_finish PRIVATE ws2_32)
endif()

add_test(NAME W7_C1_CleanOutFinish COMMAND test_w7_c1_cleanout_finish)

# ===========================================================================
#  W7-C2 VERIFY -- the end-of-lot ONE CYCLE finish-check (REAL body)
# ===========================================================================
# --- test_w7_c2_onecycle_finish: prove DoOneCycleFinishCheck does REAL work ---
#   Proves the W7-C2 change (the faithful golden DoOneCycleFinishCheck body,
#   csystem.cpp:12813-14047, now lives in the translated csystem.cpp -- replacing
#   the no-op stub at csystem_shims.cpp:145) does REAL work over the now-seedable
#   cross-arm occupancy grid:
#     (a) EMPTY machine (iOneCycle=1/IndexStatus=Z1_Z2_Normal/CUSTOMER_CODE=0, all
#         kits cleared) -> the body does NOT park on the residual-IC wait cursor 5;
#         one-cycle can proceed toward finish.
#     (b) FALSE-FINISH GUARD (the new capability): seed InArmSuck use-site=HAS_IC
#         -> iOneCycleTask==5 (golden residual wait gate :12933) + iOneCycle stays
#         1 (NO false instant-finish -- the finish path that clears iOneCycle=0 at
#         golden :13931 is unreached, unlike the old stub).
#     (c) DRAIN: clear the seed -> next call no longer parks on 5.
#     (d) NO-REGRESSION: the entry guard (golden :12820) gates the work -- iOneCycle=0
#         (the W6.6 hub posture) takes the else-branch (golden :14025), leaving the
#         cursor untouched and iOneCycle at 0 (BtnOneCycle->Down offline-false).
#   The DoOneCycleFinishCheck body + the iOneCycle/iOneCycleTask/IndexStatus/
#   CUSTOMER_CODE globals + the TMyKitSuck kit globals all live in ht9045_sm
#   (csystem.cpp / cmydef / aHotPlateSubstrate.cpp).  Same RESCAN link group as
#   the other W6/W7 state-machine tests so the cprod<->cpublic<->cmydef ctor
#   cross-references settle.  All seeded globals are saved/restored so sibling
#   suites are unaffected.
add_executable(test_w7_c2_onecycle_finish
    test_w7_c2_onecycle_finish.cpp
)

target_link_libraries(test_w7_c2_onecycle_finish PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_w7_c2_onecycle_finish PRIVATE ws2_32)
endif()

add_test(NAME W7_C2_OneCycleFinish COMMAND test_w7_c2_onecycle_finish)

# ===========================================================================
#  W5 comms wave -- SECSGEM base / Interface / CCLink / CanBus / MyPLC /
#  EtherCAT-comms (6-unit parallel translate batch, wired by the serial
#  integrate pass, 2026-07-10)
# ===========================================================================

# --- test_uHGemClass: HTGem ctor/dtor/UpdateDataPath + IsCorrectDateFormat +
#   the already-inline virtuals + derived-override polymorphism + the 57 gated
#   stubs' conservative defaults + every SECS_EVENT enumerator's exact golden
#   value.  Zero THGem/uHGemEquipment dependency -- links vclcompat only (via
#   ht9045_secsgem).  Supplies its own local MyDBIProcess(3-arg) stub.
add_executable(test_uHGemClass
    test_uHGemClass.cpp
)

target_link_libraries(test_uHGemClass PRIVATE ht9045_secsgem)

add_test(NAME uHGemClass COMMAND test_uHGemClass)

# --- test_interfacesys: GPIBInterface_Data + outbound builders + inbound
#   decoder + WM_COPYDATA dispatcher, exercised with NO real external program
#   attached (FindWindow legitimately returns NULL -- golden's own safe
#   default).  Interface/InterfaceSYS.cpp lives inside ht9045_sm (see root
#   CMakeLists.txt comment on why it is not part of ht9045_comms); ht9045_sm
#   PUBLIC-links ht9045_secsgem so EventReport()/SECS_EVENT resolve too, but
#   this test does not touch those.  Defines LOCAL stand-in globals
#   bGPIBError/bEcho/bUnderTest/bExist/iBin[4][8] that golden main.cpp owns
#   (main.cpp itself untranslated, W7).
add_executable(test_interfacesys
    test_interfacesys.cpp
)

target_link_libraries(test_interfacesys PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_interfacesys PRIVATE ws2_32)
endif()

add_test(NAME Interface_InterfaceSYS COMMAND test_interfacesys)

# --- test_MyCCLink: offline acceptance proof (HAVE_CCLINK not defined) ------
#   Exercises TMyCCLinkCtrl/TMyCCLinkBoard/UseCanBusOrEtherCAT/UseSensorMod3
#   entirely over the offline Mdfunc stub path.  Links the combined
#   ht9045_comms library (see root CMakeLists.txt); only this unit's own
#   translation units are extracted from that archive.  Supplies local stub
#   definitions for MySleep/MySleepEx/MyDBIProcess so it links standalone.
add_executable(test_MyCCLink
    test_MyCCLink.cpp
)

target_link_libraries(test_MyCCLink PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_comms,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_MyCCLink PRIVATE ws2_32)
endif()

add_test(NAME MyCCLink COMMAND test_MyCCLink)

# --- CanBus cMyNUDN1: TMyNUDN1 NUDN1 station wrapper (W5-CanBus leaf) -------
#   Links the combined ht9045_comms library; cMyNUDN1.cpp itself needs only
#   vclcompat (confirmed leaf), so ht9045_globals here is just along for the
#   ride via ht9045_comms's own PUBLIC link (harmless).
add_executable(test_cMyNUDN1
    test_cMyNUDN1.cpp
)

target_link_libraries(test_cMyNUDN1 PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_comms,ht9045_globals,ht9045_public,vclcompat>"
)

add_test(NAME CanBus_cMyNUDN1 COMMAND test_cMyNUDN1)

# --- test_myplc_modbus: SIM-only acceptance proof, no hardware/network -----
#   [1] vclcompat/ClientSocket Sim-mode connect/disconnect/SendBuf/SimPushReceive.
#   [2] MyPLC::ModbusTCPClient SendData/Connect/DisConnect + byte-level frame-
#       parse oracle driving SocketRead/SocketConnect/SocketDisConnect/SocketError.
#   [3] MyPLC_IO_Modbus InitPLCIO/PLCIOTaskCycle/PLCStatusCheck pumped without
#       crash; TMyPLC_IO_Modbus IsOn()/IsOff(); TPLCIOThread offline-inert.
#   Links the combined ht9045_comms library (MyPLC_IO_Modbus.cpp's real
#   bPLCInData/bPLCIO + local MySleepEx fallback are unconditionally compiled
#   in -- no HAVE_PLC_IO/HAVE_MYSLEEPEX flags needed since ht9045_comms is
#   never linked alongside ht9045_io/ht9045_sm in the SAME executable).
add_executable(test_myplc_modbus
    test_myplc_modbus.cpp
)

target_link_libraries(test_myplc_modbus PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_comms,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_myplc_modbus PRIVATE ws2_32 psapi version)
endif()

add_test(NAME MyPLCModbus COMMAND test_myplc_modbus)

# --- test_clientsocket_ext: Tag/Open()/SendText()/ReceiveText() acceptance --
#   SIM-only proof of the 4 additions ClientSocket.h/.cpp gained for BarCode's
#   8-CCD glue unit + Automation's TesterTCP unit (W5 final-fronts campaign,
#   2026-07-11). AI(W5-Final-Integrate) 20260711: this test file existed on
#   disk but was never wired in by any of the 4 integrate agents in that
#   campaign -- added here after independent verification caught the gap.
add_executable(test_clientsocket_ext
    test_clientsocket_ext.cpp
)

target_link_libraries(test_clientsocket_ext PRIVATE vclcompat)

if(WIN32)
    target_link_libraries(test_clientsocket_ext PRIVATE ws2_32)
endif()

add_test(NAME ClientSocketExt COMMAND test_clientsocket_ext)

# --- test_serversocket: vclcompat/ServerSocket Sim-mode acceptance test ------
#   TServerSocket/TServerWinSocket (Front C of the W906 uHGemClass/AGV_PortScan/
#   ServerSocket triple-front wave, 2026-07-13). Sim-only (no real network I/O),
#   same posture as test_clientsocket_ext.cpp just above. Links vclcompat only
#   (ServerSocket.cpp lives in the vclcompat library, see root CMakeLists.txt).
add_executable(test_serversocket
    test_serversocket.cpp
)

target_link_libraries(test_serversocket PRIVATE vclcompat)

if(WIN32)
    target_link_libraries(test_serversocket PRIVATE ws2_32)
endif()

add_test(NAME ServerSocket COMMAND test_serversocket)

# --- test_MyNUEC1: EtherCAT/MyNUEC1 FS-N12 amplifier slave (W5 comms leaf) --
#   Offline stub path (HAVE_PCI1203 not defined); links the combined
#   ht9045_comms library.
add_executable(test_MyNUEC1
    test_MyNUEC1.cpp
)

target_link_libraries(test_MyNUEC1 PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_comms,ht9045_globals,ht9045_public,vclcompat>"
)

add_test(NAME MyNUEC1 COMMAND test_MyNUEC1)

# ===========================================================================
#  W5 Automation wave (8-unit parallel translate batch, wired by the serial
#  integrate pass, 2026-07-10) -- AMR/AGV predicates, HANA-ART, SCK_ART
#  extract, uRENESAS_Other data shapes, 32-Site engine, ProcessCount leaf
#  subset, Front/Rear "Suck IC" state machines.
# ===========================================================================

# --- test_amr: TeraPower AMR bridge (Automation/AMR.cpp) + the 3 AGV AMR-mode
#   predicates (AGV_predicates.cpp), driven over a KNOWN global-state baseline.
#   Same RESCAN link group as the other ht9045_sm verify TUs (AMR.cpp/HANA_ART.cpp/
#   SCK_ART.cpp are all now part of ht9045_sm -- see root CMakeLists.txt).
add_executable(test_amr
    test_amr.cpp
)

target_link_libraries(test_amr PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_amr PRIVATE ws2_32)
endif()

add_test(NAME AMR COMMAND test_amr)

# --- test_hana_art: uHANA_ART (Automation/HANA_ART.cpp) SRQ protocol helper --
#   black-box coverage via the public API (GetHanaSRQStr/ParseLOTONStr/
#   IsHanaArtAvailable/GetHD_CT/GetHD_Tempset/GetHD_T|J|A-DATA/DoSETUP_INFORM/
#   StartPrimeTest+EndPrimeTest/RecoveryARTStatus/IsContactAvailable).
add_executable(test_hana_art
    test_hana_art.cpp
)

target_link_libraries(test_hana_art PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_hana_art PRIVATE ws2_32)
endif()

add_test(NAME HanaART COMMAND test_hana_art)

# --- test_SCK_ART: Automation/SCK_ART.cpp 8-function ART extract (W5-SckArt-Translate) -------
#   Needs the ht9045_sm substrate (aHotPlateSubstrate.cpp's TestSocket, FormsFacade.cpp's fMain,
#   canary_support.cpp's LastSet/ShowErrorMessage) + ht9045_globals. SCK_ART.cpp is compiled
#   into ht9045_sm (see root CMakeLists.txt) -- do NOT also add it to this executable's own
#   sources (would duplicate-define every SckArt_* symbol at link time).
add_executable(test_SCK_ART
    test_SCK_ART.cpp
)

target_link_libraries(test_SCK_ART PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_SCK_ART PRIVATE ws2_32)
endif()

add_test(NAME SCK_ART COMMAND test_SCK_ART)

# --- test_urenesas_other: RENESAS host-protocol data shapes + cmd/status/err tables ---
#   Pure data shapes, zero global/HAL coupling -- its own small standalone library
#   (ht9045_automation, see root CMakeLists.txt), not part of ht9045_sm.
add_executable(test_urenesas_other
    test_urenesas_other.cpp
)

target_link_libraries(test_urenesas_other PRIVATE ht9045_automation)

add_test(NAME uRENESAS_Other COMMAND test_urenesas_other)

# --- test_atester_32site: 32-Site dual-arm test-head engine (W5-atester32site) -
#   All 8 golden functions real; retires the atester_shims.cpp offline stubs for
#   these symbols (see atester_shims.cpp/.h comments). Same RESCAN link group as
#   the other ht9045_sm verify TUs.
add_executable(test_atester_32site
    test_atester_32site.cpp
)

target_link_libraries(test_atester_32site PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_atester_32site PRIVATE ws2_32)
endif()

add_test(NAME W5_Atester32Site COMMAND test_atester_32site)

# --- test_atester_front_suckic: DoFrontTestSuckIC/InitFrontTestSuckICTask SM --
#   W64b-Translate-Wave2 (aTester_Front.cpp additions). Asserts:
#     (a) InitFrontTestSuckICTask() -> cursor=1, bZ1Isdownflag=false (golden :870-875).
#     (b) DoFrontTestSuckIC() over an empty Sim HAL (no real IC anywhere) PARKS
#         at Task=1 (case 1's only advance is gated by FLCarryKit.UseSiteHasIC(),
#         golden :919 -- genuinely "nothing to pick", not a translation defect)
#         + a 128-tick pump-sanity check (cursor never escapes the documented
#         switch(Task) label set).
#   Same RESCAN link group as the sibling W64b Front/Rear Destroy suite.
add_executable(test_atester_front_suckic
    test_atester_front_suckic.cpp
)

target_link_libraries(test_atester_front_suckic PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_atester_front_suckic PRIVATE ws2_32)
endif()

add_test(NAME W64b_FrontSuckIC COMMAND test_atester_front_suckic)

# --- test_atester_rear_suckic: DoRearTestSuckIC (Wave 2, aTester_Rear.cpp) ----
#   Mirrors test_atester_front_suckic.cpp (sibling parallel unit).  Asserts:
#     (a) InitRearTestSuckICTask() -> cursor=1 (golden :891-895; NOTE golden
#         asymmetry vs Front -- Rear's Init does NOT reset an "isdownflag",
#         unlike InitFrontTestSuckICTask).
#     (b) DoRearTestSuckIC() PARKS at Task=1 over an empty Sim HAL (case 1's
#         only advance is guarded by BLCarryKit.UseSiteHasIC()==false) + a
#         128-tick pump-sanity check (cursor never escapes the documented
#         switch(Task) label set).
#   Same RESCAN link group as the W6.4b Front/Rear Destroy-IC suite.
add_executable(test_atester_rear_suckic
    test_atester_rear_suckic.cpp
)

target_link_libraries(test_atester_rear_suckic PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_atester_rear_suckic PRIVATE ws2_32)
endif()

add_test(NAME W64b_RearSuckIC COMMAND test_atester_rear_suckic)

# ===========================================================================
#  W5-BarCode (BarCode front, 2026-07-11) -- 7 parallel translate units giving
#  real bodies to 18 of the 20 TfBarCode_Shim methods (aHotPlateSubstrate.h:
#  623-664).  All 7 link the same RESCAN group as the other ht9045_sm-linked
#  suites above (cprod<->cpublic<->cmydef ctor cross-references).
# ===========================================================================

# --- test_barcode_helpers: TfHIKROBOTcmd / uCCDUnloaderClip / IsSHT2DIDScanFinish
add_executable(test_barcode_helpers
    test_barcode_helpers.cpp
)

target_link_libraries(test_barcode_helpers PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_barcode_helpers PRIVATE ws2_32)
endif()

add_test(NAME BarCodeHelpers COMMAND test_barcode_helpers)

# --- test_barcode_bottom2did: InitBottom2DIDScan / DoBottom2DIDScan (1-CCD) --
add_executable(test_barcode_bottom2did
    test_barcode_bottom2did.cpp
)

target_link_libraries(test_barcode_bottom2did PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_barcode_bottom2did PRIVATE ws2_32)
endif()

add_test(NAME BarCodeBottom2DID COMMAND test_barcode_bottom2did)

# --- test_barcode_bottom2did8ccd: DoBottom2DID_8CCD_Scan (8-CCD sibling) ----
add_executable(test_barcode_bottom2did8ccd
    test_barcode_bottom2did8ccd.cpp
)

target_link_libraries(test_barcode_bottom2did8ccd PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_barcode_bottom2did8ccd PRIVATE ws2_32)
endif()

add_test(NAME BarCode_Bottom2DID8CCD COMMAND test_barcode_bottom2did8ccd)

# --- test_barcode_shuttle1_scan: 7 Shuttle-1 scan/trigger/float-check methods
add_executable(test_barcode_shuttle1_scan
    test_barcode_shuttle1_scan.cpp
)

target_link_libraries(test_barcode_shuttle1_scan PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_barcode_shuttle1_scan PRIVATE ws2_32)
endif()

add_test(NAME BarCode_Shuttle1_Scan COMMAND test_barcode_shuttle1_scan)

# --- test_barcode_shuttle2_scan: 5 of 7 Shuttle-2 scan/trigger/float-check --
#   (DoBarcodeScanInShuttle_2/DoShuttleFloatCheck_2 handed off, not delivered)
add_executable(test_barcode_shuttle2_scan
    test_barcode_shuttle2_scan.cpp
)

target_link_libraries(test_barcode_shuttle2_scan PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_barcode_shuttle2_scan PRIVATE ws2_32)
endif()

add_test(NAME BarCode_Shuttle2Scan COMMAND test_barcode_shuttle2_scan)

# --- test_barcode_shuttle1_ccdscan: DoBarcodeCCDInShuttle_1 (largest unit) --
add_executable(test_barcode_shuttle1_ccdscan
    test_barcode_shuttle1_ccdscan.cpp
)

target_link_libraries(test_barcode_shuttle1_ccdscan PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_barcode_shuttle1_ccdscan PRIVATE ws2_32)
endif()

add_test(NAME BarCode_Shuttle1_CCDScan COMMAND test_barcode_shuttle1_ccdscan)

# --- test_barcode_shuttle2_ccdscan: DoBarcodeCCDInShuttle_2 -----------------
add_executable(test_barcode_shuttle2_ccdscan
    test_barcode_shuttle2_ccdscan.cpp
)

target_link_libraries(test_barcode_shuttle2_ccdscan PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_barcode_shuttle2_ccdscan PRIVATE ws2_32)
endif()

add_test(NAME W5_BarCode_Shuttle2CCDScan COMMAND test_barcode_shuttle2_ccdscan)

# --- W5-BarCode-Final (BarCode remainder front, 2026-07-11): the last 2/20
#     TfBarCode_Shim methods (hand-off completion) + the 8-CCD ClientSocket
#     connection-lifecycle glue. TfBarCode_Shim is 20/20 real methods as of
#     this integrate -- see CMakeLists.txt's ht9045_sm source-list comment.

# --- test_barcode_shuttle2_scanremainder1: DoBarcodeScanInShuttle_2 ---------
add_executable(test_barcode_shuttle2_scanremainder1
    test_barcode_shuttle2_scanremainder1.cpp
)

target_link_libraries(test_barcode_shuttle2_scanremainder1 PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_barcode_shuttle2_scanremainder1 PRIVATE ws2_32)
endif()

add_test(NAME BarCode_Shuttle2ScanRemainder1 COMMAND test_barcode_shuttle2_scanremainder1)

# --- test_barcode_shuttle2_scanremainder2: DoShuttleFloatCheck_2 ------------
add_executable(test_barcode_shuttle2_scanremainder2
    test_barcode_shuttle2_scanremainder2.cpp
)

target_link_libraries(test_barcode_shuttle2_scanremainder2 PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_barcode_shuttle2_scanremainder2 PRIVATE ws2_32)
endif()

add_test(NAME BarCode_Shuttle2ScanRemainder2 COMMAND test_barcode_shuttle2_scanremainder2)

# --- test_barcode_8ccd_glue: 13 real TClientSocket + SendCCDCommand + the 2
#     Bottom-8CCD timers -----------------------------------------------------
add_executable(test_barcode_8ccd_glue
    test_barcode_8ccd_glue.cpp
)

target_link_libraries(test_barcode_8ccd_glue PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_barcode_8ccd_glue PRIVATE ws2_32)
endif()

add_test(NAME BarCode8CCDGlue COMMAND test_barcode_8ccd_glue)

# ===========================================================================
#  W5-Final -- vclcompat::TList shim (BCB6 VCL.Classes TList, generic void*
#  list) + the SECSGEM wire-codec / SV-EC-registration prerequisite slices
# ===========================================================================

# --- test_tlist: vclcompat::TList shim semantics ----------------------------
#   Add/Count/IndexOf/Items[] (get+set)/Delete/Clear + the non-owning-list
#   contract (TList never frees what it holds -- unlike TObjectList). Header-
#   only shim (vclcompat/TList.h); links vclcompat for the umbrella header
#   (vcl_compat.h) even though no vclcompat .cpp symbol is actually called.
add_executable(test_tlist
    test_tlist.cpp
)

target_link_libraries(test_tlist PRIVATE vclcompat)

add_test(NAME TList COMMAND test_tlist)

# --- test_SecsWireCodec: SecsWireCodec (THGem's wire-codec slice) ----------
#   GetLengthOfType/GetLengthByte/GetSMLLenthByte/DataItemOut x2/DataItemInSub/
#   DataItemIn x2/DataItemInNew/GetDataItemLenAndType(Sub)/StringOut + the
#   shared HType global. Zero THGem/VCL/socket/grid dependency -- links
#   ht9045_secsgem (which now carries SecsWireCodec.cpp) only.
add_executable(test_SecsWireCodec
    test_SecsWireCodec.cpp
)

target_link_libraries(test_SecsWireCodec PRIVATE ht9045_secsgem)

add_test(NAME SecsWireCodec COMMAND test_SecsWireCodec)

# --- test_SecsSvEcRegistration: SecsSvEcRegistration (THGem's SV/EC ---------
#   registration API) -- SetSVDataPointer x4 / SetECDataPointer x4 /
#   GetECDataValue. Links ht9045_secsgem (carries both SecsSvEcRegistration.cpp
#   AND its sibling SecsWireCodec.cpp, needed for the shared HType global that
#   GetECDataValue's Type-dispatch compares against). No THGem/VCL-form/
#   socket/grid dependency.
add_executable(test_SecsSvEcRegistration
    test_SecsSvEcRegistration.cpp
)

target_link_libraries(test_SecsSvEcRegistration PRIVATE ht9045_secsgem)

add_test(NAME SecsSvEcRegistration COMMAND test_SecsSvEcRegistration)

# --- test_uHGemEquipment: THGem's first real slice (W906-uHGemEquipment) ----
#   CEID/Report/Alarm StringGrid "database" family (26 methods) + GemTimer +
#   vclcompat::TStringGrid. Links ht9045_secsgem (which now carries
#   SECSGEM/uHGemEquipment.cpp) -- PUBLIC-links vclcompat transitively (the
#   new vclcompat/StringGrid.cpp + AnsiString/TStringList this file needs) and,
#   as of the W906-uHGemEquipment-ConnLifecycle wave, ht9045_public (GetErrorMsg,
#   see ht9045_secsgem's own target in the root CMakeLists.txt).
#   Supplies its own local MyDBIProcess(2-arg) stand-in (see the test file's
#   own comment) rather than linking ht9045_sm's real aHotPlateSubstrate.cpp
#   definition -- same "avoid the god-stack for a focused unit test" posture
#   already established by test_config_loaders.cpp/test_MyCCLink.cpp.
#   AI(W906-uHGemEquipment-ConnLifecycle) 20260717: THGem now owns a real
#   clientGem/srvGem pair (TClientSocket/TServerSocket, vclcompat/ClientSocket
#   .cpp + ServerSocket.cpp) -- this test is THAT slice's first real consumer,
#   so ws2_32 must be linked (WIN32) exactly like every other test that
#   references vclcompat's socket internals (test_serversocket/
#   test_clientsocket_ext/test_myplc_modbus/... all do the same, see their own
#   entries in this file) -- Real-mode code paths inside ClientSocket.cpp/
#   ServerSocket.cpp reference WSAStartup/socket/connect/send/recv/etc. even
#   though this test only ever exercises Sim mode.
add_executable(test_uHGemEquipment
    test_uHGemEquipment.cpp
)

# AI(W906-uHGemEquipment-ConnLifecycle) 20260717: plain
# `target_link_libraries(... PRIVATE ht9045_secsgem)` (as this line used to
# read) left ws2_32 placed BEFORE libvclcompat.a/libht9045_public.a on the
# actual link line (confirmed via the generated linkLibs.rsp) -- GNU ld's
# single left-to-right pass never re-visits ws2_32 once vclcompat's later
# unresolved WSAStartup/socket/connect/etc. show up, so the link failed.
# Same LINK_GROUP:RESCAN fix already established by this exact multi-hop-
# static-lib-plus-ws2_32 shape at test_myplc_modbus's own entry in this file.
target_link_libraries(test_uHGemEquipment PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_secsgem,ht9045_public,vclcompat>"
)
if(WIN32)
    target_link_libraries(test_uHGemEquipment PRIVATE ws2_32)
endif()

add_test(NAME uHGemEquipment COMMAND test_uHGemEquipment)

# ===========================================================================
#  W5-Final Automation-remainder INTEGRATE: auto9045 / AGV_E84 /
#  SCK_ART_Remainder / Interface/TesterTCP_Socket
# ===========================================================================

# --- test_auto9045: Automation/auto9045.cpp whole-file OLP/GPIB config-accessor translation ---
add_executable(test_auto9045
    test_auto9045.cpp
)
target_link_libraries(test_auto9045 PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)
if(WIN32)
    target_link_libraries(test_auto9045 PRIVATE ws2_32)
endif()
add_test(NAME Auto9045 COMMAND test_auto9045)

# --- test_agv_e84: Automation/AGV_E84.cpp E84 loader/unloader handshake core ---
add_executable(test_agv_e84
    test_agv_e84.cpp
)
target_link_libraries(test_agv_e84 PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)
if(WIN32)
    target_link_libraries(test_agv_e84 PRIVATE ws2_32)
endif()
add_test(NAME AGV_E84 COMMAND test_agv_e84)

# --- test_agv_portscan: Automation/AGV_PortScan.cpp (the second AGV.cpp slice,
#     E84 sensor-status telemetry + AMR/AGV port-scan dispatcher family) -------
add_executable(test_agv_portscan
    test_agv_portscan.cpp
)
target_link_libraries(test_agv_portscan PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)
if(WIN32)
    target_link_libraries(test_agv_portscan PRIVATE ws2_32)
endif()
add_test(NAME AGV_PortScan COMMAND test_agv_portscan)

# --- test_SCK_ART_Remainder: Automation/SCK_ART_Remainder.cpp (next contiguous
#     golden slice after the sibling SCK_ART.cpp 8-function extract) ---------
add_executable(test_SCK_ART_Remainder
    test_SCK_ART_Remainder.cpp
)
target_link_libraries(test_SCK_ART_Remainder PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)
if(WIN32)
    target_link_libraries(test_SCK_ART_Remainder PRIVATE ws2_32)
endif()
add_test(NAME SCK_ART_Remainder COMMAND test_SCK_ART_Remainder)

# --- test_testertcp_socket: Interface/TesterTCP_Socket.cpp (bounded socket-
#     management subset of Interface/TesterTCP.cpp) ---------------------------
add_executable(test_testertcp_socket
    test_testertcp_socket.cpp
)
target_link_libraries(test_testertcp_socket PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)
if(WIN32)
    target_link_libraries(test_testertcp_socket PRIVATE ws2_32)
endif()
add_test(NAME Interface_TesterTCPSocket COMMAND test_testertcp_socket)

# ===========================================================================
#  W906 fire-wave INTEGRATE: Automation/automation.cpp / Interface/TesterTCP.cpp
# ===========================================================================

# --- test_automation: Automation/automation.cpp (TfAutomation OLP host-
#     command engine, "network wiring + small helpers" bucket) --------------
#   Same RESCAN link shape as test_auto9045/test_agv_e84/test_SCK_ART_Remainder
#   above (automation.cpp is compiled directly into ht9045_sm, alongside
#   auto9045.cpp/FormsFacade.cpp/canary_support.cpp it depends on).
add_executable(test_automation
    test_automation.cpp
)
target_link_libraries(test_automation PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)
if(WIN32)
    target_link_libraries(test_automation PRIVATE ws2_32)
endif()
add_test(NAME Automation COMMAND test_automation)

# --- test_TesterTCP: Interface/TesterTCP.cpp (sibling slice of
#     Interface/TesterTCP_Socket.cpp -- CopyRecipeToTester/CopyRecipeFromTester
#     OS-recipe zip-transfer pair + btnSaveClick arm-speed-save handler) -----
#   Same RESCAN link shape as test_testertcp_socket above (TesterTCP.cpp is
#   compiled directly into ht9045_sm, alongside acarry_shims.cpp/
#   canary_support.cpp it depends on for the REAL MySleep/RecordProcess/
#   ShowMyMessage bodies -- see TesterTCP.cpp's own extern-redeclaration
#   comments). INTEGRATE FIX: the front's own test_TesterTCP.cpp originally
#   supplied a LOCAL no-op `void MySleep(DWORD) {}` stand-in (written under the
#   assumption this test would NOT link ht9045_sm, matching the test_
#   FTPClient_Transfer.cpp/test_MyCCLink.cpp "avoid the god-stack" precedent).
#   That stand-in was REMOVED here: linking the RESCAN group (needed anyway,
#   for the real DataPath/sSaveByMachine/WriteIniData/ReadIniData in
#   ht9045_core + CosFunction/IniConfig in ht9045_globals) pulls in
#   acarry_shims.cpp's own real MySleep(DWORD) definition, and a second
#   definition in the test's main() would be a multiple-definition link error.
add_executable(test_TesterTCP
    test_TesterTCP.cpp
)
target_link_libraries(test_TesterTCP PRIVATE
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_secsgem,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)
if(WIN32)
    target_link_libraries(test_TesterTCP PRIVATE ws2_32)
endif()
add_test(NAME Interface_TesterTCP COMMAND test_TesterTCP)

# ===========================================================================
#  W5-Final KYECFTP: MiniFtpEngine (Phase 1) / FTPClient_EventHandlers
#  (Phase 2a) / FTPClient_Transfer (Phase 2b)
#
#  All 3 targets below compile the relevant KYECFTP/*.cpp sources DIRECTLY
#  (not via the production `ht9045_kyecftp` library in the root CMakeLists.txt)
#  -- see each target's own comment for why.  `ht9045_kyecftp` itself still
#  gets built (as part of the default `all` target) and is what proves these
#  files type-check against the REAL cmydef.h/cprod.h/Config.h/CosFunction.h/
#  common.h/canary_support.h declarations; these 3 tests instead verify BEHAVIOR
#  fast and offline, each supplying its own minimal stand-ins per its file's own
#  SCOPE NOTE (same established pattern as tests/test_config_loaders.cpp /
#  tests/test_MyCCLink.cpp).
# ===========================================================================

# --- test_MiniFtpEngine: Nmftp::TNMFTP engine, Sim-mode fake-FTP-server, end to end ---
#   Zero god-stack dependency (MiniFtpEngine.{h,cpp} only touch vclcompat) --
#   compiles MiniFtpEngine.cpp directly and links vclcompat only.
add_executable(test_MiniFtpEngine
    test_MiniFtpEngine.cpp
    ../KYECFTP/MiniFtpEngine.cpp
)
target_link_libraries(test_MiniFtpEngine PRIVATE vclcompat)
if(WIN32)
    target_link_libraries(test_MiniFtpEngine PRIVATE ws2_32)
endif()
add_test(NAME MiniFtpEngine COMMAND test_MiniFtpEngine)

# --- test_FTPClient_EventHandlers: NMFTP1 event-handler bodies --------------
#   Needs the REAL CUSTOMER_CODE (cmydef.cpp, via ht9045_globals) for test
#   [G]'s CC_JSCC_OS gated-dependency case -- links ht9045_globals (+
#   ht9045_public, its own established co-dependency; see test_cpublic_foundation
#   above). Supplies its OWN local MyDBIProcess(2-arg)/ShowMyMessage stand-ins
#   instead of linking ht9045_sm (avoids the aHotPlateSubstrate.cpp/
#   canary_support.cpp god-stack for a focused unit test -- same avoidance as
#   test_config_loaders.cpp/test_MyCCLink.cpp) -- ht9045_sm/ht9045_core are
#   DELIBERATELY NOT linked here (nor is the production ht9045_kyecftp library,
#   which PUBLIC-links both): pulling either in risks the REAL MyDBIProcess/
#   ShowMyMessage bodies (aHotPlateSubstrate.cpp/canary_support.cpp, both inside
#   ht9045_sm) getting extracted alongside this TU's own same-named stand-ins,
#   a multiple-definition link error.
add_executable(test_FTPClient_EventHandlers
    test_FTPClient_EventHandlers.cpp
    ../KYECFTP/MiniFtpEngine.cpp
    ../KYECFTP/FTPClient_EventHandlers.cpp
)
target_link_libraries(test_FTPClient_EventHandlers PRIVATE ht9045_globals ht9045_public vclcompat)
if(WIN32)
    target_link_libraries(test_FTPClient_EventHandlers PRIVATE ws2_32)
endif()
add_test(NAME FTPClient_EventHandlers COMMAND test_FTPClient_EventHandlers)

# --- test_FTPClient_Transfer: the 4 FTP-transfer session functions ----------
#   Deliberately supplies ITS OWN TU-local stand-ins for EVERY extern global
#   FTPClient_Transfer.cpp references (CUSTOMER_CODE/IniConfig/CosFunction/
#   Temperature/DataPath/OffsetPath/DefaultPath/LastDataPath/
#   asATCFileTransferPath/sWhite2DIDListLoc/sATCPath/ATC_SYSTEM/FTPAutomation_*/
#   bSigurd*/iAMD_Function/bHasFTPDownload/asSetupFileCheckList) PLUS
#   ShowMyMessage/RecordProcess/NewRecordProcess/MyDBIProcess(2-arg)/MySleep --
#   see the test file's own SCOPE NOTE. This means it must NOT link
#   ht9045_globals/ht9045_core/ht9045_sm (nor the production ht9045_kyecftp
#   library, which PUBLIC-links all three): the REAL cmydef.cpp/cprod.cpp/
#   common.cpp definitions of these SAME global names would collide with this
#   TU's own the moment anything else in that link forces one of those (very
#   large, densely cross-referenced -- see KNOWLEDGE.md's "全域狀態耦合"
#   gotcha) object files to be extracted from its archive -- a real
#   multiple-definition risk, not just a style preference. Compiles the 3
#   KYECFTP sources directly instead, linking only vclcompat.
add_executable(test_FTPClient_Transfer
    test_FTPClient_Transfer.cpp
    ../KYECFTP/MiniFtpEngine.cpp
    ../KYECFTP/FTPClient_EventHandlers.cpp
    ../KYECFTP/FTPClient_Transfer.cpp
)
target_link_libraries(test_FTPClient_Transfer PRIVATE vclcompat)
if(WIN32)
    target_link_libraries(test_FTPClient_Transfer PRIVATE ws2_32)
endif()
add_test(NAME FTPClient_Transfer COMMAND test_FTPClient_Transfer)
