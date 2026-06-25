add_executable(test_ContactForce
    test_ContactForce.cpp
)

target_link_libraries(test_ContactForce PRIVATE ht9045_core)

add_test(NAME ContactForce COMMAND test_ContactForce)

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
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
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
    "$<LINK_GROUP:RESCAN,ht9045_sm,ht9045_io,ht9045_motor,ht9045_db,ht9045_core,ht9045_globals,ht9045_public,vclcompat>"
)

if(WIN32)
    target_link_libraries(test_w6_canary PRIVATE ws2_32)
endif()

add_test(NAME W6_Canary COMMAND test_w6_canary)
