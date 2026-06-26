cmake_minimum_required(VERSION 3.10)
project(HT9045_Cpp C CXX)   # C needed for Public/cJSON.c

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------------------------------------------------------------------
# Library: vclcompat  (W0 keystone)
#   BCB6 / VCL compatibility layer: recreate ONLY the API surface the V906
#   source actually uses (AnsiString / TStringList / TDateTime / SysUtils),
#   with semantics that mirror BCB6 exactly (1-based AnsiString indexing, etc.).
#   Translated files #include "vclcompat/vcl_compat.h" instead of <vcl.h>.
# ---------------------------------------------------------------------------
add_library(vclcompat
    vclcompat/AnsiString.cpp
    vclcompat/TStringList.cpp
    vclcompat/TDateTime.cpp
    vclcompat/SysUtils.cpp
    vclcompat/IniFiles.cpp
    # --- HAL enabler: SPComm TComm serial shim (Spcomm::TComm) --------------
    #   Faithful stand-in for the SPComm VCL serial component
    #   (D:/HT9045/elec/Component/spcomm.hpp:154-277) used by the temp /
    #   barcode / EJ1N / RS232-tester code.  REAL mode drives \\.\COMx via
    #   Win32 CreateFile/SetCommState/ReadFile/WriteFile + a Win32 reader
    #   thread; SIM mode (auto fallback / SetSimMode) needs no hardware so the
    #   compat layer compiles/links/runs offline.  Lives in the vclcompat lib
    #   because it derives from vclcompat::TComponent and is pulled in via the
    #   vcl_compat.h umbrella.  On Win32 the .cpp uses the kernel32 serial API
    #   (already in the default MinGW link set -- no extra target_link needed).
    vclcompat/Comm.cpp
)

target_include_directories(vclcompat PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# ---------------------------------------------------------------------------
# Library: ht9045_core
#   Collects all ROOT-level translated source files.
#   Mirror the BCB6 source tree: translated .cpp/.h files live at the project
#   root (same relative path as their BCB6 originals).
# ---------------------------------------------------------------------------
add_library(ht9045_core
    ContactForce.cpp
    cUnitConvert.cpp      # W2 partial: iUnitMultiply100/iUnitMultiply1000
    common.cpp            # W3 partial: ini-helper family only; other regions gated #if 0
)

target_include_directories(ht9045_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# ht9045_core links the compat layer (translated logic will depend on it).
target_link_libraries(ht9045_core PUBLIC vclcompat)

# Strict warnings help catch porting errors early.
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(HT9045_WARNFLAGS -Wall -Wextra -Wshadow -Wconversion -Wpedantic)
    target_compile_options(vclcompat   PRIVATE ${HT9045_WARNFLAGS})
    target_compile_options(ht9045_core PRIVATE ${HT9045_WARNFLAGS})
endif()

# ---------------------------------------------------------------------------
# Library: ht9045_public
#   Portable utility modules translated from BCB6 Public/*.
#
#   cJSON.c  -- portable C (Dave Gamble, MIT); compiled as C via set_source_files_properties.
#              No VCL dependency.  #include "Public/cJSON.h" from any C++ TU.
#
#   HTMD5.cpp -- MD5 class + file/folder helpers.  Depends on vclcompat for
#               AnsiString, TStrings/TStringList, FileExists, DirectoryExists.
#               VCL TMask replaced by simple_wildcard_match(); auto_ptr -> unique_ptr.
# ---------------------------------------------------------------------------
# Compile cJSON.c as C (it must not see C++ name mangling on its own symbols).
set_source_files_properties(Public/cJSON.c PROPERTIES LANGUAGE C)

add_library(ht9045_public
    Public/cJSON.c
    Public/HTMD5.cpp
    # --- W1 Public leaf utilities (BCB6 -> standard C++17) ------------------
    Public/ExternFunction.cpp      # Math_AdjustAngle / String_JustNumber /
                                   # String_Erase[LR]Space / RecordTime  (pure logic)
    Public/WinSocketErrorCode.cpp  # GetErrorMsg(Winsock code -> AnsiString); needs ws2_32 headers
    Public/cBootLog.cpp            # Win32 file-based boot tracer (WriteBootLog / Rotate)
    # --- HAL enabler: KeyPro license interface-cut --------------------------
    #   KeyPro_GetLevel(unsigned) -- portable stand-in for the SOLE KeyPro entry
    #   the V906 source calls (KEYPRO_GET_LEVEL(3) at HS_Function.cpp:4933).
    #   Resolves the export from KeyProDLL.dll at RUN TIME (LoadLibrary/
    #   GetProcAddress) and returns 1 (valid dongle) when the DLL is absent
    #   (offline).  Deliberately does NOT static-link KeyProDLL.lib (32-bit OMF
    #   Borland import lib, unusable by MinGW) -- no extra target_link_libraries
    #   entry is added for it.  The std::string-returning KeyPro exports are
    #   NEVER called in 906 and are NOT ported.
    Public/HTKeyProShim.cpp
)

# cJSON.c uses WIN32 FindFirstFile indirectly (via cJSON.h guards); HTMD5.cpp
# uses FindFirstFileA directly.  Both need windows.h available under MinGW --
# it is, but we silence the dllexport chatter by hiding symbols for the static build.
target_compile_definitions(ht9045_public PRIVATE
    CJSON_HIDE_SYMBOLS          # suppress __declspec(dllexport) in cJSON.h
)

target_include_directories(ht9045_public PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ht9045_public PUBLIC vclcompat)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(ht9045_public PRIVATE ${HT9045_WARNFLAGS})
    # HTMD5.cpp contains the original RFC 1321 MD5 algorithm bit-operations and
    # streampos casts that were never ported (intentional faithful translation).
    # Suppress inherited warnings on that TU only; all other TUs keep full strictness.
    set_source_files_properties(Public/HTMD5.cpp PROPERTIES COMPILE_FLAGS
        "-Wno-conversion -Wno-parentheses")
endif()

# ht9045_core also benefits from public utilities.
target_link_libraries(ht9045_core PUBLIC ht9045_public)

# ---------------------------------------------------------------------------
# Library: ht9045_common
#   Translated logic islands under Common/ that are interface-driven and free
#   of Prod/cmydef/MachineType/hardware/VCL-form coupling.
#
#   PickPlanner -- generic enumerate-and-score Arm pick/place planner
#     (cArmPickPlan) plus two production adapters that are interface-driven
#     via IArmPickPlanContext:
#       cInArmLoaderPickPlanner -- Phase 3 scaffold (Search() returns false)
#       cOutArmPlacePlanner     -- Phase 4 scaffold (Search() returns false)
#     The adapters ship as scaffolds: the Phase 3.1 / 4.1 bodies that would
#     pull MOT[]/InArmSuck/OutArmSuck globals are deferred to W6 and remain
#     inline TODOs.  Nothing here reaches an untranslated global, so the whole
#     subtree compiles standalone against vclcompat only.
# ---------------------------------------------------------------------------
add_library(ht9045_common
    Common/PickPlanner/cArmPickPlan.cpp
    Common/PickPlanner/cInArmLoaderPickPlanner.cpp
    Common/PickPlanner/cOutArmPlacePlanner.cpp
)

target_include_directories(ht9045_common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Common/PickPlanner
)

# PickPlanner .cpp files swap <vcl.h> for the vclcompat umbrella header, so the
# compat layer must be on the include path even though no VCL type is used.
target_link_libraries(ht9045_common PUBLIC vclcompat)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(ht9045_common PRIVATE ${HT9045_WARNFLAGS})
endif()

# ---------------------------------------------------------------------------
# Library: ht9045_globals  (W0-TAIL keystone)
#   The de-VCL'd GLOBAL define/type headers + the .cpp regions that define the
#   contract globals. These headers (MachineType / Config / CosFunction / cprod /
#   cpublic / cmydef / myTimer / MachineDefine) gate most downstream logic.
#
#   Each .cpp keeps its active global var DEFINITIONS (Prod / IniConfig /
#   CosFunction / Offset / ARM_OFFSET* arrays / name-tables / TQPF_Timer) while
#   the function bodies that reach untranslated state machines / app headers are
#   gated behind '#if 0 // TODO(Wx)'.  Built as a static library: it must COMPILE
#   standalone (verified), but is NOT linked into an executable here because the
#   active globals construct objects whose ctors live in the gated regions
#   (those resolve in W3/W6/W7).  myTimer.cpp is fully translated this wave.
# ---------------------------------------------------------------------------
add_library(ht9045_globals STATIC
    myTimer.cpp
    cprod.cpp
    cpublic.cpp
    cmydef.cpp
)

target_include_directories(ht9045_globals PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ht9045_globals PUBLIC vclcompat)

# AI(ht9045-v899) 20260626: cpublic.cpp now ungates FindAndKillProcess (psapi) and
# VerInfo::GetAppVersion/m_GetVerInfo (version.lib).  Both ship with MinGW but
# must be listed explicitly -- the linker does not auto-pull them.
if(WIN32)
    target_link_libraries(ht9045_globals PUBLIC psapi version)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # The translated globals carry the original BCB6 idioms (e.g. AnsiString
    # array brace-init with NULL).  Keep them compiling without the strict
    # -Werror-style flags used on freshly-written modules.
    target_compile_options(ht9045_globals PRIVATE
        -Wno-conversion-null -Wno-write-strings)
endif()

# ---------------------------------------------------------------------------
# third_party: sqlite3  (W3 external dependency)
#   The golden ref links a prebuilt sqlite3.lib and ships only sqlite3.h (no
#   sqlite3.c).  Per the migration plan, sqlite3 is an EXTERNAL portable
#   dependency, NOT hand-translated -- cMyDB (the WRAPPER) is what we translate
#   (scheduled W3).  This subdir creates an `sqlite3` target resolved as:
#   vendored amalgamation  ->  system find  ->  header-only stub.
#   No executable links it yet (cMyDB is not translated this wave), so the
#   stub path keeps the build green while documenting the dependency.
# ---------------------------------------------------------------------------
add_subdirectory(third_party/sqlite3)
message(STATUS "sqlite3 resolution: ${SQLITE3_RESOLUTION}")

# ---------------------------------------------------------------------------
# Library: ht9045_db  (W3-cont2 config-table loaders)
#   PARTIAL translation of database.cpp: the Mot_Table.csv / IO_Table.csv
#   parsers (SYSTEM_MODULAR::LoadMotData / LoadIoData + the TMOTDATA / TIODATA
#   row ctors + the name-based column resolvers TMOTNO::SetMOTTableNo /
#   TIOTABLENO::SetIOTableNo).  All other database.cpp regions (TDataModule1,
#   SYSTEM_MODULAR ctor/dtor, ReadGeneralIni, SystemModularInitial body,
#   InstallColorBinDisplay) are gated #if 0 // TODO(wave) inside the .cpp.
#
#   Depends on:
#     * vclcompat       -- AnsiString / TStringList / FileExists / HexStrToInt
#     * ht9045_globals  -- INDEX_MOTION_CARD (cmydef.cpp), the cmydef/MachineType
#                          headers, and TQPF_Timer (myTimer.o) for HSys.SysTimer.
#
#   NOT linked into an executable directly here; the test_config_loaders target
#   links it and supplies the two stubbed externals (MyDBIProcess / ShowMyMessage)
#   that the gated UI/DB waves will provide for real.
# ---------------------------------------------------------------------------
add_library(ht9045_db
    database.cpp
)

target_include_directories(ht9045_db PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ht9045_db PUBLIC vclcompat ht9045_globals)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # database.cpp carries the original BCB6 idioms inherited from the global
    # headers (AnsiString brace-init etc. pulled via cmydef/cprod).  Match the
    # ht9045_globals warning posture so the translated loaders compile without
    # tripping on idioms that live in the (already-accepted) global headers,
    # while still flagging genuine porting mistakes in the loader bodies.
    target_compile_options(ht9045_db PRIVATE
        -Wall -Wextra -Wno-conversion-null -Wno-write-strings)
endif()

# ---------------------------------------------------------------------------
# Library: ht9045_motor  (W4 HAL motor layer)
#
#   HTMotor.{h,cpp}    -- abstract port (virtual base, no vendor SDK)
#   mySimMotor.{h,cpp} -- TMySimMotor : offline sim adapter (default brand)
#   mymotor.{h,cpp}    -- TMyMotor / TTrayMotor (PARTIAL: motion-routing core)
#
#   Sim path (default): compiles + links with NO vendor SDK; no HAVE_xxx macros
#   needed.  The brand drivers (SMC/Galil/MN200/SYNTEK/EtherCAT/MC88X1) are
#   deferred to W4-part2 -- their .cpp files are NOT translated yet because they
#   pull the same W6 god-stack (main.h / cinitial.h / mycylin.h / etc.) as
#   mymotor.cpp.  They will be translated with #if HAVE_xxx vendor guards once
#   the W6 sensor layer lands.
#
#   mymotor.cpp pulls sIC_Type[] (cmydef.h), iYRegNum (cmydef.h), bTestSiteUse[]
#   (cmydef.h), HAS_IC/NULL_IC/HAS_NULL_IC (cprod/cmydef), INDEX_MOTION_CARD
#   (cmydef), MOTION_CARD_TYPE (cmydef), ChangeToFloatNonPcnt (MachineType.h),
#   Prod.* (cprod) -- all resolved by ht9045_globals.
#   mytray.h (TMyTray): NOT yet in Motor/ -- include path is project root.
# ---------------------------------------------------------------------------
add_library(ht9045_motor STATIC
    Motor/HTMotor.cpp
    Motor/mySimMotor.cpp
    Motor/mymotor.cpp
    mytray.cpp          # TMyTray: required by TTrayMotor in mymotor.cpp (W4 partial)
)

target_include_directories(ht9045_motor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ht9045_motor PUBLIC vclcompat ht9045_globals)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(ht9045_motor PRIVATE
        -Wall -Wextra -Wno-conversion-null -Wno-write-strings -Wno-unused-parameter)
endif()

# ---------------------------------------------------------------------------
# Library: ht9045_io  (W4-IO HAL IO layer)
#
#   IOBackend.{h,cpp}   -- abstract port TIOBackend (WriteBit/WriteByte/ReadBit/
#                          ReadByte, int<0=fail) + TSimIOBackend (offline in-memory
#                          default, always active) + real backends TMN200Backend /
#                          TPci1203Backend / TMnetLegacyBackend / TRawPortBackend
#                          (bodies behind #if HAVE_MN200 / HAVE_PCI1203 / HAVE_MNET /
#                          (HAVE_RAWPORT && x86), all OFF by default).
#   MyLaneIo.{h,cpp}    -- TLaneIO facade: keeps the 6 BCB6 method names + signatures
#                          + OutPortData cache + CheckPortRangeErr + ret<0 / SOFT_SIMULTE
#                          bookkeeping; routes vendor calls through TIOBackend* pIO.
#                          Default: pIO = new TSimIOBackend() (offline).
#   myswitch.{h,cpp}    -- TMySwitch On/Off/Status/OnOff/CopySwitch; eMotionNet||
#                          ePCI1203 -> MyLaneIO (active); pad/raw branches gated
#                          #if 0 // TODO(W6).
#   mysensor.{h,cpp}    -- TMySensor Status/IsOn/IsOff/CopySensor via MyLaneIO;
#                          same pad gate; raw branch stub.
#
#   Sim path (default): compiles + links with NO vendor SDK; no HAVE_xxx macros
#   needed.  The real backends compile as stubs (each method {return 0;}) and
#   never link to vendor symbols.
#
#   Depends on:
#     * vclcompat       -- AnsiString / windows.h (byte)
#     * ht9045_globals  -- InitialOK, IO_CARD_TYPE, MyBitMask, MachineTypeChoice,
#                          uiDevhand, iPLCSafetyVer, eMotionNet/ePCI1203/etc.
#                          (cmydef.h / MachineType.h / cprod.h resolved here)
#
#   DEFER to W4-IO-part2 / W6 (NOT compiled now):
#     * TMyCylinder Push/Pop/On/Off/SetSimulateCompoment (Alarm/SystemStart/
#       bHandlerPause/TControl god-stack)
#     * TMySucker Suck()/Destroy() state machines (TQPF_Timer/alarm)
#     * Entire TMyKitSuck data grid (TALed/pLed/TMyProductionRecord/TTrayMotor/
#       cprod/MyDBIProcess)
#     * myio.cpp raw-port free funcs (outportb/IOSetOutport/TTL_CARD_TYPE +
#       2-arg IdleCheckSafeDoorByCylinder)
#     * Real backend wiring/selection in cinitial by IO_CARD_TYPE
#     * MyPLC_IO_Modbus/ModbusTCPClient PLC input state
# ---------------------------------------------------------------------------
add_library(ht9045_io STATIC
    IOBackend.cpp
    MyLaneIo.cpp
    myswitch.cpp
    mysensor.cpp
    # --- W6.0: TMyCylinder HAL piece (the one real substrate gap the W6.1
    #   Empty-tray canary needs).  The W4-IO note above DEFERRED TMyCylinder
    #   Push/Pop/On/Off to "W4-IO-part2 / W6"; this is that wave.  Routes over
    #   the Sim IO HAL (MyLaneIO) exactly like myswitch/mysensor; the alarm /
    #   SmartDiagnostic / VCL-widget god-stack is stubbed/gated in mycylin.cpp.
    mycylin.cpp
)

target_include_directories(ht9045_io PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ht9045_io PUBLIC vclcompat ht9045_globals)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(ht9045_io PRIVATE
        -Wall -Wextra -Wno-conversion-null -Wno-write-strings -Wno-unused-parameter)
endif()

# ---------------------------------------------------------------------------
# Library: ht9045_sm  (W6 state-machine wave -- W6.1 Empty-tray CANARY)
#
#   The first ROOT state machine translated over the W6 substrate, plus the
#   W6 decoupling scaffold that lets it stand alone:
#
#     asendic_Empty.cpp     -- the Empty-tray stack-feed SM (CANARY).  Faithful
#                              switch(Task) steppers; each Do* owns its cursor int.
#     csystem_predicates.cpp-- the frozen csystem.h HasIC predicate interface
#                              (implemented over the Sim HAL; canary uses none,
#                              but they link the contract for later sub-waves).
#     FormsFacade.cpp       -- non-VCL fAGV satellite stub (the canary's only
#                              form-pointer deref: fAGV->IsATK_AMR()).
#     asendic.cpp           -- SIM bodies for CylinderUp/Middle/Lower + TrayMove*
#                              tray-move/lifter helpers (manipulate Cylinder[]/
#                              MOT[]; full lifter SM deferred W6.x).
#     canary_support.cpp    -- minimal LastSet shim + sim bodies for the free
#                              functions the canary calls that have no shared
#                              target header (ShowErrorMessage/RecordProcess/
#                              ShowUnloaderTrayMessage/WhichAutoNeedTray/
#                              ShowMyMessage/ReadWriteTrayID).
#
#   Depends on the full substrate: ht9045_io (Cylinder[]/SW[]/Sen[]/MyLaneIO),
#   ht9045_motor (MOT[]), ht9045_globals (Prod/IniConfig/CosFunction/TrayForm/
#   RunInfo/Ld_UldDelayTime/TrayID/enums), ht9045_db (database.cpp -- pulled by
#   the RESCAN group for cross-archive ctor resolution), vclcompat.
# ---------------------------------------------------------------------------
add_library(ht9045_sm STATIC
    asendic_Empty.cpp
    csystem_predicates.cpp
    FormsFacade.cpp
    asendic.cpp
    canary_support.cpp
    # --- W6.2 (IN-ARM foundation): shared HotPlate pick/place geometry leaves ---
    #   ainarm_SearchPickPlate.cpp  -- HP-pick geometry + DoInArmPickFromHotPlate_9045 SM
    #   ainarm_SearchPlacePlate.cpp -- HP-place geometry + search-planner + data-swap leaves
    #   aHotPlateSubstrate.cpp      -- the W6.2 in-arm substrate the leaves pump
    #                                  (InArmSuck/FLCarryKit/.. TMyKitSuck grid +
    #                                   PickFromHPList uPlateInfo + ainarm2 cursors +
    #                                   in-arm engine helper stubs + tRotate/Laser)
    ainarm_SearchPickPlate.cpp
    ainarm_SearchPlacePlate.cpp
    aHotPlateSubstrate.cpp
    # --- W6.2b (IN-ARM engine): the central pick SM + iInArmType dispatch
    #     ladders + pure pitch/offset geometry + 3 pumpable task SMs.  Variant
    #     dispatch bodies + dense MOT[]/sensor/UI SMs are gated (#if 0) with
    #     active stubs; see the file-head gate documentation.
    ainarm9045.cpp
    # --- W6.2b-canary (IN-ARM site variants): 3 per-site-config variant files
    #     translated faithfully from the golden BCB6 tree ROOT (mirror name/path).
    #       ainarm9045_1x1_1.cpp -- single-site 1x1_1 (THE buildable, live arm;
    #                               defines DoInArm_9045_1x1_1 + _SuckerMap, called
    #                               by the un-gated dispatch arm in ainarm9045.cpp).
    #       ainarm9045_2x4_16.cpp + _shims -- 16-site variant; faithful translation
    #                               but DEAD by the golden engine design (no
    #                               e9045_2x4_16 enum, no dispatch arm; references a
    #                               file-local NN-site map -- golden member absent by
    #                               design).  Registered so it compiles/links; never
    #                               dispatched.
    #       ainarm9045_2x8_32.cpp -- 32-site variant; whole golden body GATED (needs
    #                               Prod.fInArmSuckUse_TwoArm32Site, absent from
    #                               golden cprod.h by design) + the golden engine
    #                               routes e9045_2x8_32 -> 2x8_8 (DEAD).  Active
    #                               offline stubs make the TU link; never dispatched.
    ainarm9045_1x1_1.cpp
    ainarm9045_2x4_16.cpp
    ainarm9045_2x4_16_shims.cpp
    ainarm9045_2x8_32.cpp
    # --- W6.2c batch-1 (5 IN-ARM site variants made ACTIVE; dispatch arms pulled
    #     out of the "#if 0 // TODO(W6.2b variants)" gate in ainarm9045.cpp):
    ainarm9045_1x3_4.cpp
    ainarm9045_1x2_2.cpp
    ainarm9045_1x2_2_14.cpp
    ainarm9045_1x2_4_Hot.cpp
    ainarm9045_1x3_2_14.cpp
    # --- W6.2c batch-2 (6 IN-ARM site variants made ACTIVE; dispatch arms pulled
    #     out of the "#if 0 // TODO(W6.2b variants)" gate in ainarm9045.cpp).
    #     2x2_4 / 2x2_4_14 also drove the substrate add of TMyKitSuck::
    #     ArmDownSideAllTypeIC (golden MyKitSuck.h:335 -> aHotPlateSubstrate).
    ainarm9045_1x4_2.cpp
    ainarm9045_1x4_8_Hot.cpp
    ainarm9045_2x2_4_12.cpp
    ainarm9045_2x1_2.cpp
    ainarm9045_2x2_4.cpp
    ainarm9045_2x2_4_14.cpp
    # --- W6.2c batch-3 (6 IN-ARM site variants made ACTIVE; dispatch arms pulled
    #     out of the "#if 0 // TODO(W6.2b variants)" gate in ainarm9045.cpp).
    #     1x4_4 also drove the substrate add of cSiteUseManager (SiteUseMgr) +
    #     RecordErrorLog (see substrate note).  All_1Pick's DoInArm arm is the
    #     FIRST ladder arm (USE_PICKER_COUNT==ep1Picker), and its *_SuckerMap()
    #     is declared-but-NEVER-dispatched by the golden engine (faithful).
    ainarm9045_1x4_4.cpp
    ainarm9045_1x4_4_Back.cpp
    ainarm9045_2x3_6.cpp
    ainarm9045_2x3_6_14.cpp
    ainarm9045_2x4_4.cpp
    ainarm9045_All_1Pick.cpp
    cSiteUseManager.cpp
    # --- W6.2b-canary (IN-ARM shared helpers): the two ainarm2.cpp leaves the
    #     site-variant SMs call but that had no body yet -- SetInArmUseSuckToHas
    #     NullIC (golden ainarm2.cpp:201) + InitInArmPickFromHotPlateTask (golden
    #     :623), FAITHFUL pure-grid/cursor logic over the Sim HAL.  Also hosts the
    #     single ACTIVE definition of bDestoryOnSht (cmydef.cpp's is gated #if 0).
    ainarm2.cpp
    # --- W6.2c (OUT-ARM engine): the central place-to-Auto SM + iInArmType
    #     dispatch ladders (DoOutArm_9045 / DoPickFromShuttle_9045) + pure
    #     pitch/offset geometry + 4 pumpable task SMs (DoOutArmAdditionalFunction /
    #     DoOutArmPlaceToAuto_9045 / DoOutArmAfterPlaceToAuto / DoOutArmIonFanGiveWay).
    #     Variant dispatch bodies + dense MOT[]/sensor/Magazine/AOI/Rotate SMs are
    #     gated (#if 0) with active stubs; see the file-head gate documentation.
    #     Pumps the same aHotPlateSubstrate (OutArmSuck/FRCarryKit/BRCarryKit/
    #     OutArm2Suck + out-arm cursor ints) as the in-arm engine.
    aoutarm9045.cpp
    # --- W7 (TRAY-ARM engine): the top dispatcher DoCatchTray (flat switch) + 8
    #     pumpable task SM skeletons (DoCatchFromLoader / CatchNewTrayFromBuffer /
    #     DoPlaceTrayToAuto / DoPlaceToBuffer / DoSlapTray / DoCatchUnderTray /
    #     DoSupportUnderTray / DoPlaceBufferTray) + cover-tray/RFID/cassette SMs +
    #     the pure decision helpers (WhichAutoNeedTray/IsOutArmSafe/...).  No
    #     site-variant dispatch in this file.  Hardware behind the Sim HAL
    #     (MOT[]/Cylinder[]/Sen[]/SW[]/CatchTraySuck); cross-module/UI surface
    #     (fTrayMapping/LoadCCD/MyMessageBox/AMR/HSys.BinDisCtrl) behind offline
    #     shims + ACTIVE stubs; see the file-head gate documentation.  OWNS the
    #     real WhichAutoNeedTray() (the canary_support.cpp sim stub was removed).
    acatchtray.cpp
    # --- W6.3 TRAY-ARM cross-module offline shims (fTrayMapping / MyMessageBox /
    #     LoadCCD / AMR / SECS_EVENT+EventReport / Tray-Mapping-laser / AMR-feed /
    #     NewRecordProcess / BoolToStr / iCoverTrayIDTask / iReadCIDAction).
    acatchtray_shims.cpp
    # --- W6.4 (TESTER/INDEX engine): the test-result-DECODE anchor (GetBinaryData /
    #     GetOneByOneData fed by Test_GetTestResulSub/Test_GetTestResul) + its
    #     consumers (Check_TTL_Status / CheckTestSocketIsError / ProcessTestResult)
    #     + the central index SMs DoTestY / DoTestHeadMotor (entry faithful, deep
    #     MOT[]/EP/torque/Front-Rear/32Site bodies gated) + pure site geometry
    #     (ArrayConvertSite / GetRowCol / GetRTCSiteMap).  Pumps the same Sim HAL +
    #     aHotPlateSubstrate (TestSocket/FTestSuck/BTestSuck/InArmSuck/FRCarryKit/
    #     BRCarryKit + iTestYTask/iTestHeadMotorTask cursors).  See the file-head
    #     gate documentation.
    atester.cpp
    # --- W6.4 TESTER/INDEX cross-module offline shims (atester_32Site /
    #     atester_ProcessCount / aTester_Front / aTester_Rear / fContact / ADAM_*).
    atester_shims.cpp
    # --- W6.5 (SHUTTLE/CARRY engine): the two input-shuttle master SMs
    #     Do_Auto_SHT1 / Do_Auto_SHT2 (39 cases each, ALL ACTIVE; case 200->201
    #     fall-through preserved) + the 9046AU sort-shuttle Do_Auto_SHT3 + the
    #     residual/sensor-broken/null-IC/floating checks (switch TestIF.iTestMode)
    #     + the In/Out-arm-vs-shuttle move-safe interlocks + pure pitch geometry +
    #     the uShuttleThread sensor-poll LOGIC (verbatim; the Win32 wrapper is a
    #     thin offline driver).  Pumps the same Sim HAL + aHotPlateSubstrate
    #     (FLCarryKit/BLCarryKit/FRCarryKit/BRCarryKit + AutoSHT1/2/3Task cursors).
    #     NO #if 0 gates -- every golden case body is ACTIVE.  See the file head.
    acarry.cpp
    # --- W6.5 SHUTTLE/CARRY cross-module offline shims (Tech / fLtcSensor /
    #     OutSht3Kit / ATC_InterfaceForm+ATC_Data / CheckShtFloating /
    #     InShtInLF-RT(int) / SortSHTInLF-RT / DoStepShuttleCheck / shuttle
    #     Empty/MustHas/Prominent checks / Check32site* / MoveOutArmXY_To* /
    #     SendHTTPRequest / MoveSortArmToAutoSafe / Win32 thread tokens).
    acarry_shims.cpp
    # --- W6.6 (csystem ORCHESTRATION HUB): the integration SPINE that ties the
    #     five already-translated engines together.  csystem.cpp realizes the
    #     frozen csystem.h spine free-functions: MainProc (slim) + DoAllProcess
    #     (THE per-tick engine driver -- bDoProcess 2-tick alternation + master
    #     guards + the 5 engine calls in documented order + DoLoad/DoCatchTray/
    #     DoAutoReceiveBinTray loop + tail loops) + the alive-instrumentation
    #     (GetMainProc*/IsMainProcAlive) + InitAllProcessTask + CheckMotorHome +
    #     SetInitialICCheck + InitCleanOutFunction/InitOneCycle/InitTrayEndFunction.
    #     The ~24K-line god-file bulk (AutoRetest/OCR/AutoClean/mode-ladder/
    #     DoOneCycleFinishCheck/DoCleanOutFinishCheck/...) is GATED #if 0
    #     // TODO(W7).  Predicates (HasIC family) stay in csystem_predicates.cpp
    #     (ODR-skipped here).  See the file-head documentation.
    csystem.cpp
    # --- W6.6 HUB cross-module offline shims (DoInArm->DoInArm_9045 /
    #     DoOutArm->DoOutArm_9045 / DoLoad / DoSortArm / bShuttleShake +
    #     InitLoadTask / InitOutArmTask / InitialDoLockUnloader /
    #     Initial_Auto_BinTray_Task / InitAutoColorTask / Magazine inits /
    #     DoAuto3Magazine / NewDoAutoTrayEdgeCylinderLoop / DoAuto2 / DoAutoEmpty1 /
    #     DoInArm/OutArmTeachAlignmentProcess / SetFixTrayMiddleDtata /
    #     DoOneCycleFinishCheck / DoCleanOutFinishCheck).
    csystem_shims.cpp
)

target_include_directories(ht9045_sm PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ht9045_sm PUBLIC
    ht9045_io ht9045_motor ht9045_globals vclcompat)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # The SM carries BCB6 idioms inherited from the global headers (AnsiString
    # brace-init etc.).  Match the ht9045_io / ht9045_globals warning posture so
    # the translated stepper compiles without tripping on already-accepted
    # global-header idioms, while still flagging genuine porting mistakes.
    target_compile_options(ht9045_sm PRIVATE
        -Wall -Wextra -Wno-conversion-null -Wno-write-strings -Wno-unused-parameter)
endif()

# W6.6: enable the csystem.cpp tick-sequence ORACLE hook (compiles a tiny
# std::vector trace into DoAllProcess so the W6.6 verify TU can confirm the
# documented per-tick engine ORDER).  The CSYS_TICK macro is a NO-OP unless this
# is defined, so the faithful golden body is untouched in spirit; the trace is
# write-only instrumentation read only by the integration test.
target_compile_definitions(ht9045_sm PRIVATE CSYSTEM_TICK_ORACLE)

# ---------------------------------------------------------------------------
# Tests
# ---------------------------------------------------------------------------
enable_testing()
add_subdirectory(tests)
