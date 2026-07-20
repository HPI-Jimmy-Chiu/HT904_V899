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
    # --- W5 comms enabler: Scktcomp TClientSocket/TCustomWinSocket shim -------
    #   Faithful stand-in for the VCL Scktcomp::TClientSocket (Winsock2 TCP
    #   client) used by MyPLC/ModbusTCPClient.cpp.  Default is SIM mode (no
    #   real connect attempted); Real mode is an explicit composition-root
    #   opt-in (SetSimMode(false)) -- see the file header for why this does
    #   NOT auto-fall-back to Sim on a failed Real connect (Enable_PLCSafety_IO
    #   is a safety-interlock read; masking a real connect failure would be
    #   unsafe).  Lives here (not in MyPLC/ or a new library) because it is
    #   general vclcompat infra, matching Comm.cpp's precedent.
    vclcompat/ClientSocket.cpp
    # --- W906-ServerSocket enabler: Scktcomp TServerSocket/TServerWinSocket
    #   shim (multi-connection server sibling of ClientSocket.cpp above).
    #   Faithful stand-in for the VCL ScktComp::TServerSocket used by
    #   SECSGEM/uHGemEquipment.cpp's srvGem (passive-HSMS role) and
    #   Automation/automation.cpp's OLPServer -- see ServerSocket.h's own
    #   file-head for both golden call-shape citations.  No production
    #   consumer wired yet (prerequisite infra, same shape as the earlier
    #   SecsWireCodec/SecsSvEcRegistration slices); reuses ClientSocket.h's
    #   TCustomWinSocket for its per-connection objects (see that header's
    #   own EXTENSION note), so it must link/compile alongside ClientSocket.cpp
    #   in the same library, not standalone.  Default is Sim mode; Real mode
    #   is an explicit SetSimMode(false) opt-in (WinSock2 listen/accept +
    #   a background acceptor thread + per-connection reader threads).
    vclcompat/ServerSocket.cpp
    # --- W906-uHGemEquipment enabler: headless TStringGrid shim (Grids.hpp) --
    #   Faithful stand-in for the real VCL TStringGrid used by SECSGEM/
    #   uHGemEquipment.h's CEID/Report/Alarm "StringGrid database" family
    #   (strGrdCEID/stdGridReportID/strGrdAlarm/strGrdAlarmOld -- see that
    #   header's own file-head note). Headless: Cells[ACol][ARow] (0-based,
    #   unlike AnsiString's 1-based indexing) + growable RowCount/ColCount
    #   property proxies; no rendering/FixedRows/Options. Zero dependency
    #   beyond vclcompat/AnsiString.h + <vector>. NOT brought into the global
    #   namespace by vcl_compat.h (consumers add their own `using
    #   vclcompat::TStringGrid;` -- see that header's own note on why).
    vclcompat/StringGrid.cpp
    # --- W906-uHGemEquipment-BucketC enabler: headless TMemoryStream shim
    #   (Classes.hpp). Faithful stand-in for the real VCL TMemoryStream used
    #   by SECSGEM/uHGemEquipment.cpp's RecvMemoryBuffer/ProcBuffer/
    #   TempProcBuffer (clientGemRead/ProcessSocketReceiveData) -- see
    #   vclcompat/MemoryStream.h's own file-head note for the exact API
    #   surface + semantics. Zero dependency beyond <vector>/<cstring>.
    #   vclcompat/SyncObjs.h (TCriticalSection) is header-only (inline) --
    #   no matching .cpp, no separate CMake source entry needed for it.
    vclcompat/MemoryStream.cpp
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
    # --- W64b (TESTER/INDEX force-calc extract): TfContact::GetMaxIndexForceLimit /
    #   GetMinForce turned into free functions (ComputeMaxIndexForceLimit / ComputeMinForce).
    #   Only #includes MachineType.h (header-only enum/macro header per KNOWLEDGE.md W0); every
    #   other external read (dDutCount, TestIF.iTestMode, IniConfig.*, fContactForce->SLKClass[i]->
    #   dContactOffset*) is passed in as an explicit scalar parameter, so this TU needs NO link to
    #   ht9045_globals (confirmed empirically: the clean build below links fine without it).
    cContact.cpp
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

    # AI(W64b-ForceCalc-Translate) 20260710: cContact.cpp is the first ht9045_core TU to
    # #include MachineType.h under full -Wpedantic.  MachineType.h (already-accepted W0
    # keystone header) carries several BCB6-preserved `};` stray-semicolon idioms after
    # free-function template bodies (e.g. ChangeToFloatNonPcnt) that ISO C++ pedantically
    # flags as "extra ';'" at namespace scope -- harmless, but not this wave's file to fix.
    # Narrowly suppress -Wpedantic on this ONE translation unit only (same precedent as
    # ht9045_public's HTMD5.cpp / ht9045_globals' idiom carve-outs); every other warning
    # class (-Wall -Wextra -Wshadow -Wconversion) still applies in full to cContact.cpp.
    set_source_files_properties(cContact.cpp PROPERTIES COMPILE_FLAGS "-Wno-pedantic")
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
# Library: ht9045_secsgem  (W5 SECSGEM base layer + Sim EventReport seam)
#
#   uHGemClass.{h,cpp}   -- HTGem: abstract SECS/GEM S,F-handler base (~90
#                           virtuals; the ~33 golden already left fully inline
#                           carry verbatim, the 57 that need THGem/uHGemEquipment
#                           (out of scope) are #if 0-gated with a conservative
#                           ACTIVE stub -- link-complete today).
#   SecsEventType.{h,cpp}-- SECS_EVENT (ETypeStruct): the CEID enum extracted
#                           standalone from uHGemHT9045.h, verbatim values.
#   SecsEventReport.{h,cpp} -- NEW (integrate-agent addition, not a golden
#                           translation): a minimal Sim-first free function
#                           `EventReport(unsigned Ceid)` that gives csystem.cpp's
#                           W7C1_EVENTREPORT_*/W7C2_EVENTREPORT_* seam macros a
#                           REAL, offline-safe call target (records last-CEID +
#                           a running count) instead of a no-op.  Deliberately
#                           does NOT instantiate HTGem -- see its file header:
#                           HTGem::~HTGem() needs a 3-arg MyDBIProcess overload
#                           that only tests/test_uHGemClass.cpp stubs today, and
#                           forcing every ht9045_sm-linked executable to resolve
#                           that too is not worth it for a Sim counter.
#
#   ht9045_sm links this library so csystem.cpp can call EventReport()/read
#   SECS_EVENT directly; only SecsEventType.o + SecsEventReport.o are ever
#   extracted from this archive for that link (uHGemClass.o stays a dormant,
#   unreferenced archive member there -- same "compiled but not pulled in"
#   shape as other inert files elsewhere in this tree). tests/test_uHGemClass
#   links this library directly to exercise HTGem itself.
#
#   SecsWireCodec.{h,cpp} -- W5-Final: THGem's pure byte-level SECS-II item
#     codec slice (GetLengthOfType/GetLengthByte/GetSMLLenthByte/DataItemOut
#     x2/DataItemInSub/DataItemIn x2/DataItemInNew/GetDataItemLenAndType(Sub)/
#     StringOut), plus the shared `HType`/HTypeStruct global (the canonical
#     home for HTypeStruct/STypeStruct/HSMS_Head_Struct -- see its own file
#     header). Zero dependency beyond vclcompat.
#   SecsSvEcRegistration.{h,cpp} -- W5-Final: THGem's SV/EC registration API
#     (SetSVDataPointer x4 / SetECDataPointer x4 / GetECDataValue). A
#     deliberately SEPARATE class from SecsWireCodec (reuses its `HType`
#     global via #include, does not extend it -- see that file's own
#     "DESIGN: SEPARATE CLASS" note); depends on SecsWireCodec.h + vclcompat/
#     TList.h (vclcompat::TList, used qualified -- never brought into the
#     global namespace, per that shim's own ODR note re aHotPlateSubstrate.h).
#   Both slices are prerequisites -- not yet consumers -- of a FUTURE full
#   THGem/uHGemEquipment.cpp translation wave (out of scope here); see each
#   file's own "INTEGRATE-AGENT WIRING POINT" note.
#
#   SECSGEM/uHGemEquipment.{h,cpp} -- W906-uHGemEquipment INTEGRATE: THGem's
#     FIRST real slice (a deliberately partial class -- see that header's own
#     file-head note for exactly what is/isn't modeled yet). This wave's slice
#     is the CEID/Report/Alarm StringGrid "database" family (SetCEIDContent/
#     GetCEIDContent/DeleteReportID/.../CopyStringGridAsTabFormat/SetAlamData/
#     ReadAlamData/WriteAlamData, 26 methods) + the standalone GemTimer utility.
#     Depends on vclcompat + the new vclcompat/StringGrid.h shim (see above) +
#     SECSGEM/SecsEventType.h (SECS_EVENT.TotalEvent array bound). Deliberately
#     does NOT yet embed SecsWireCodec/SecsSvEcRegistration (InitLocalHead/
#     DataItemOut/GetDataItemLenAndType/SetSVDataPointer/... are still OUT of
#     THGem's scope this wave -- a future wave should add them as by-value
#     members, matching HTGem's own already-proven WireCodec/SvEcReg pattern;
#     see uHGemEquipment.h's own "MEMBERS DELIBERATELY NOT PRESENT YET" note).
#     INTEGRATE CHECK (this wave): confirmed zero ODR overlap with
#     uHGemClass.cpp/SecsWireCodec.cpp's own GetDataItemLenAndTypeAndDelete(Sub)
#     -- uHGemEquipment.cpp does not define or call either; THGem does not yet
#     model that method at all. No production consumer wired yet (HTGem's
#     `THGem *HGemPtr` in uHGemClass.h stays a forward declaration -- see that
#     header's own comment on why an #include swap was evaluated and declined
#     this wave: every currently-gated HTGem method needs at least one THGem
#     member/method still absent from this partial slice, so switching to a
#     full #include would add a large new dependency for zero un-gating
#     benefit today).
#
#   AI(W906-uHGemEquipment-BucketB) 20260717: THIRD wave on this same file --
#     ~15 small widget stand-ins (THGemRadioGroup/THGemEdit/THGemCheckBox/
#     THGemComboBox/THGemPanel/THGemSpeedButton/THGemMemo, uHGemEquipment.h) +
#     InitialHGem/SaveSystemDefault/DoUpdateStatus/ProcessShow/FormClose/
#     ManualCreatergRoleClick/FormShow/StringOut(2-arg). DEPENDENCY WIDENING:
#     this slice needs CUSTOMER_CODE/CC_KYEC_LEE/CC_SIGURD_ChungXing/
#     CC_MAXIM_THAILAND/CosFunction/bSECSGEMbyPass/bSECSGEMConnectionFail
#     (cmydef.h -> ht9045_globals) and ReadWriteIni/ReadIniData/WriteIniData
#     (common.h -> ht9045_core) -- see uHGemEquipment.cpp's own top-of-file
#     note for the full reasoning (common.h confirmed to stay lightweight:
#     ini-helper family only, does not itself drag in the rest of golden's
#     common.h). Same "translated unit needs the global/ini-helper families"
#     shape already established by KYECFTP/FTPClient_Transfer.cpp's own
#     ht9045_core+ht9045_globals link (see that library's own comment above).
#     No circular dependency: ht9045_core/ht9045_globals each only link
#     vclcompat (+psapi/version for ht9045_globals on WIN32), neither links
#     back to ht9045_secsgem or ht9045_public.
# ---------------------------------------------------------------------------
add_library(ht9045_secsgem
    SECSGEM/uHGemClass.cpp
    SECSGEM/SecsEventType.cpp
    SECSGEM/SecsEventReport.cpp
    SECSGEM/SecsWireCodec.cpp
    SECSGEM/SecsSvEcRegistration.cpp
    SECSGEM/uHGemEquipment.cpp
    # AI(W906-SysModWire) 20260720: HT9045Gem thin shim (Decision A) -- the
    # ONLY definition of `HT9045Gem`, needed by database.cpp's now-real
    # SystemModularInitial (`new HT9045Gem(...)`). See that header's own
    # file-head note for the full scope/swap-back-path writeup.
    SECSGEM/uHGemHT9045_Shim.cpp
)

target_include_directories(ht9045_secsgem PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/SECSGEM
)

# AI(W906-uHGemEquipment-ConnLifecycle) 20260717: THGem::GetSocketErrorMsg
# (uHGemEquipment.cpp, golden :6842-6845) is a thin forward to the already-real
# GetErrorMsg(TObject*, int) -- its one implementation is
# Public/WinSocketErrorCode.cpp, which lives in the ht9045_public library (see
# that library's own target above). GetErrorMsg only reads <winsock2.h>
# constants (no WSA*/socket function calls -- see that .cpp's own header
# note), so this adds no further transitive link requirement (no ws2_32 etc.).
# AI(W906-uHGemEquipment-BucketB) 20260717: ht9045_core (common.h's
# ReadWriteIni/ReadIniData/WriteIniData) + ht9045_globals (cmydef.h's
# CUSTOMER_CODE/CC_*/CosFunction/bSECSGEMbyPass/bSECSGEMConnectionFail) added
# -- see this target's own comment above for the full reasoning.
# AI(W906-uHGemEquipment-BucketC) 20260717: ht9045_db added -- D3's HSys.MyGem
# seam (`#include "database.h"`, `extern SYSTEM_MODULAR HSys;`) needs
# database.cpp's one definition of `HSys` linked in. ht9045_db's OWN
# target_link_libraries declaration only lists vclcompat+ht9045_globals (see
# that library's own target above) -- no CMake-target-graph cycle.
#
# AI(W906-SysModWire) 20260720: a real (object-file-level, not CMake-target-
# level) circular SYMBOL dependency now exists as of this wave: database.cpp's
# SystemModularInitial calls `new HT9045Gem(...)`, a symbol this library
# (ht9045_secsgem) defines (uHGemHT9045_Shim.cpp.o) -- so ht9045_db's
# database.cpp.o now has an undefined reference back INTO this archive, even
# though ht9045_db's CMake target never declares ht9045_secsgem as a
# dependency. Any executable that ends up needing BOTH archives (this one,
# for HSys via ht9045_db) must link them through a `$<LINK_GROUP:RESCAN,...>`
# (or equivalent --start-group/--end-group) wrapper, not a plain sequential
# link -- see tests/CMakeLists.txt's test_uHGemEquipment/test_uHGemClass/
# test_config_loaders (the three targets that actually pull database.cpp.o;
# independent review verified via nm that only these executables contain
# _HSys -- the motor/io targets linking ht9045_db never pull database.cpp.o,
# so they need no ht9045_secsgem edge) for the applied fix.
# Deliberately NOT making ht9045_db declare
# `target_link_libraries(ht9045_db PUBLIC ht9045_secsgem)` here -- that would
# invert this library's intended layering (ht9045_db is a lower layer than
# SECS/GEM) for a dependency that is a database.cpp implementation detail, not
# ht9045_db's own public interface.
target_link_libraries(ht9045_secsgem PUBLIC vclcompat ht9045_public ht9045_core ht9045_globals ht9045_db)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(ht9045_secsgem PRIVATE ${HT9045_WARNFLAGS})

    # AI(W5-SECSGEM-Integrate) 20260711: uHGemClass.h/.cpp carry 2 golden BCB6
    # idioms (an intentionally-unused virtual-override parameter; a trailing
    # ';' after a ctor body) -- same class of carve-out already accepted for
    # cContact.cpp. Narrowly suppress just these two warning classes on this
    # file; every other warning class (-Wall -Wextra -Wshadow -Wconversion)
    # still applies in full.
    set_source_files_properties(SECSGEM/uHGemClass.cpp PROPERTIES COMPILE_FLAGS
        "-Wno-pedantic -Wno-unused-parameter")
endif()

# ---------------------------------------------------------------------------
# Library: ht9045_automation  (W5 Automation wave -- RENESAS host-protocol data shapes)
#
#   Automation/uRENESAS_Other.{h,cpp} -- ~22 plain-old-data record/reply classes
#     (TRENESAS_OnLine/_ProdInfo/_ModeStart/_HandlerStart/_ModeEnd/_LotEnd/_TestEnd/
#     _SendAlarm/_SendCleanOut + their _Reply counterparts + the aggregate
#     TRENESAS_RECV_CMD_DATA/_Send_CMD_DATA containers), the TRENESAS_CMD /
#     TRENESAS_Status_Code / TRENESAS_Err_Index command/status/error-code constant
#     tables, and TRENESAS_EndFileData (the only class with real logic). Zero
#     hardware/VCL/global coupling -- pure data shapes prerequisite for the FUTURE
#     Automation/uRENESAS_Server.cpp wave (not yet scheduled, not yet a consumer),
#     so this gets its own small standalone library (matching the unit's own
#     translate report) rather than being folded into ht9045_sm's larger link
#     surface for no reason.
# ---------------------------------------------------------------------------
add_library(ht9045_automation STATIC
    Automation/uRENESAS_Other.cpp
)

target_include_directories(ht9045_automation PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ht9045_automation PUBLIC vclcompat)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(ht9045_automation PRIVATE ${HT9045_WARNFLAGS})
endif()

# ---------------------------------------------------------------------------
# Library: ht9045_comms  (W5 comms wave -- CCLink / CanBus / MyPLC / EtherCAT)
#
#   CCLink/MyCCLink.{h,cpp}       -- TMyCCLinkCtrl (top-level comm state machine)
#   CCLink/MyCCLinkBoard.{h,cpp}  -- TMyCCLinkBoard (Mdfunc vendor board wrapper;
#                                    real vendor calls behind #if HAVE_CCLINK,
#                                    default OFF -- mirrors IOBackend.cpp's
#                                    HAVE_MN200/HAVE_PCI1203/HAVE_MNET).
#   CCLink/MyCCLinkSensor_predicates.{h,cpp} -- UseCanBusOrEtherCAT/UseSensorMod3.
#   CanBus/cMyNUDN1.{h,cpp}       -- TMyNUDN1 (NUDN1 DeviceNet amplifier station
#                                    wrapper); real vendor calls behind
#                                    #if HAVE_DNM100, default OFF.
#   MyPLC/ModbusTCPClient.{h,cpp} -- Modbus-TCP-ish framing client over
#                                    vclcompat/ClientSocket.h (above).
#   MyPLC/MyPLC_IO_Modbus.{h,cpp} -- PLC safety-IO polling (InitPLCIO/
#                                    PLCIOTaskCycle/PLCStatusCheck) + TPLCIOThread
#                                    (offline-inert driver, no real OS thread).
#   EtherCAT/MyNUEC1.{h,cpp}      -- TMyNUEC1 (FS-N12 EtherCAT current-sense
#                                    amplifier slave); real vendor calls behind
#                                    #if HAVE_PCI1203 (REUSES the flag
#                                    IOBackend.cpp/MyLaneIo.cpp already
#                                    established for the same Advantech API
#                                    family), default OFF.
#
#   Interface/InterfaceSYS.cpp is DELIBERATELY NOT part of this library: its
#   own translate report found it both depends on (TestSocket/LastSet/
#   FormsFacade) and unblocks (atester.cpp's GetTesterResult/ProcessTesterTimeOut)
#   symbols that live in ht9045_sm -- making it a separate library here would
#   create a circular ht9045_comms<->ht9045_sm CMake target dependency (CMake
#   rejects cycles in the target link graph, unlike the linker's own archive
#   re-scanning). It is instead compiled directly into ht9045_sm's own source
#   list (see that library below) -- the integrate agent's own resolution of
#   this unit's flagged conflict.
#
#   No cross-unit archive-selection conflicts: the four units here never
#   reference each other's symbols (verified by grep during integrate); each
#   unit's own dedicated test below links this whole library, and only that
#   unit's own translation units are ever extracted from the archive.
# ---------------------------------------------------------------------------
add_library(ht9045_comms STATIC
    CCLink/MyCCLink.cpp
    CCLink/MyCCLinkBoard.cpp
    CCLink/MyCCLinkSensor_predicates.cpp
    CanBus/cMyNUDN1.cpp
    MyPLC/ModbusTCPClient.cpp
    MyPLC/MyPLC_IO_Modbus.cpp
    EtherCAT/MyNUEC1.cpp
)

target_include_directories(ht9045_comms PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ht9045_comms PUBLIC vclcompat ht9045_globals)

if(WIN32)
    target_link_libraries(ht9045_comms PUBLIC ws2_32)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # BCB6-idiom-heavy new comms code (narrowing int->short/byte at vendor call
    # boundaries, empty golden event handlers, etc.) -- match the ht9045_io /
    # ht9045_motor relaxed posture rather than ht9045_core's strict one (none of
    # these files is the "genuinely idiom-free" case that earned cContact.cpp
    # its stricter flags).
    target_compile_options(ht9045_comms PRIVATE
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
    # --- W5-comms INTEGRATE: Interface/InterfaceSYS.cpp (WM_COPYDATA IPC bridge
    #     to the GPIB/Tester bridge / ESD / Auto-Update / Event-Log-Analyzer
    #     helper programs).  Lives HERE, not in ht9045_comms, per that unit's own
    #     translate report: it depends on TestSocket/LastSet (aHotPlateSubstrate.
    #     cpp/canary_support.cpp, just above) + fMain/fBarCode (FormsFacade.cpp,
    #     above) and in turn unblocks atester.cpp's still-gated GetTesterResult/
    #     ProcessTesterTimeOut (both also in this library) -- a standalone
    #     library here would create a circular ht9045_comms<->ht9045_sm target
    #     dependency.  See CMakeLists.txt's ht9045_comms comment block.
    Interface/InterfaceSYS.cpp
    # --- W5-Final-TesterTCPSocket INTEGRATE: Interface/TesterTCP_Socket.cpp
    #     (bounded socket-management subset of Interface/TesterTCP.cpp --
    #     Connect/Disconnect/Error/Read handlers, the 30-tick reconnect timer,
    #     SendTCPIPCommand/AddTCPIPCommunicationLog). Same rationale as
    #     InterfaceSYS.cpp just above: compiled directly into ht9045_sm (not a
    #     separate library) since it depends on FormsFacade/canary_support/
    #     common already carried here. Depends on vclcompat/ClientSocket.h's
    #     TClientSocket/TCustomWinSocket/TErrorEvent shim (already present on
    #     disk from an earlier front in this integrate sequence) + the
    #     StringReplace/TReplaceFlags addition this unit made to
    #     vclcompat/SysUtils.h/.cpp (additive only, no hard-boundary file touched).
    Interface/TesterTCP_Socket.cpp
    # --- W6.2b (IN-ARM engine): the central pick SM + iInArmType dispatch
    #     ladders + pure pitch/offset geometry + 3 pumpable task SMs.  Variant
    #     dispatch bodies + dense MOT[]/sensor/UI SMs are gated (#if 0) with
    #     active stubs; see the file-head gate documentation.
    ainarm9045.cpp
    # --- W7-A1 INTEGRATE: offline link stubs for the shuttle-motion engine seam
    #     symbols the un-gated in-arm floating/latch PAIR references (golden
    #     ainarm2.h: DoShakeShuttle/DoKnockShuttle/DoKnockShuttleFirst/
    #     DoVibrateShuttle/RecordShtSuperflous + IndexZCanMove[2]/bShuttleKnock).
    #     IsTestZ1/Z2NotSafe... are NOT here (defined in acarry.cpp).
    ainarm9045_w7_shims.cpp
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
    # --- W6.2c batch-4 (5 IN-ARM site variants made ACTIVE; dispatch arms pulled
    #     out of the "#if 0 // TODO(W6.2b variants)" gate in ainarm9045.cpp).
    #     2x2_8_Hot drove the substrate add of TMyKitSuck::SetNullIcToHasNullIc
    #     (golden MyKitSuck.h:280/.cpp:340) + the engine predicate RowCanDualSite
    #     (golden ainarm2.cpp:1554, offline false).  2x8_8 drove the offline homes
    #     of IsMoveInArm2XYToShuttle2Wait (golden ainarm2.h:142 -> acatchtray_shims)
    #     + XPHSuckToSht_2x8_8_OutArm (golden aoutarm9045_2x8_8.h:7 -> substrate).
    #     The golden engine routes e9045_2x8_32 -> DoInArm_9045_2x8_8 (the LIVE
    #     family), so the DEAD ainarm9045_2x8_32.cpp (line 419) stays compiled-but-
    #     never-dispatched -- do NOT remove it.  iCloseSiteModeFor2x6/2x8 +
    #     iCloseSiteStep_2x8 placeholder definitions in aHotPlateSubstrate.cpp /
    #     acatchtray_shims.cpp were removed (the variant .cpp own the real defs).
    ainarm9045_2x2_8_Hot.cpp
    ainarm9045_2x4_8.cpp
    ainarm9045_2x5_8.cpp
    ainarm9045_2x6_8.cpp
    ainarm9045_2x8_8.cpp
    # --- W6.2c batch-5 (2 S-family HT-9045S in-arm variants made ACTIVE; dispatch
    #     arms un-gated in BOTH ladders of ainarm9045.cpp).  IRREGULAR func names:
    #     ainarm9045S_1x4_4.cpp defines DoInArm_9045S_1x4_4(+_SuckerMap) (S in func);
    #     ainarm9045S_2x4_4_13.cpp defines DoInArm_9045_2x4_4_13(+_SuckerMap) (NO S).
    ainarm9045S_1x4_4.cpp
    ainarm9045S_2x4_4_13.cpp
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
    # --- W6.2c-OUT batch (6 OUT-ARM site variants made ACTIVE; dispatch arms
    #     pulled out of the "#if 0 // TODO(W6.2b variants)" gate in aoutarm9045.cpp
    #     + their 2 static no-op stubs removed).  All 6 route to a LIVE callee per
    #     the golden ENUM->CALLEE map.  These drove the substrate adds of
    #     TMySucker::GetNeedSuckStatus/SetNeedSuck/bNeedSuck (golden MyKitSuck.h:
    #     119/121/123) + TMyKitSuck::SetUnuseAndHasNullICToNullIC (golden
    #     MyKitSuck.cpp:369) in aHotPlateSubstrate.{h,cpp}.
    aoutarm9045_1x1_1.cpp
    aoutarm9045_1x2_2.cpp
    aoutarm9045_1x2_4.cpp
    aoutarm9045_1x3_2_14.cpp
    aoutarm9045_1x3_4.cpp
    aoutarm9045_1x4_2.cpp
    # --- W6.2c-OUT batch-2 (7 more OUT-ARM site variants made ACTIVE; LIVE mode):
    #     1x4_4S/1x4_4/1x4_8/2x1_2/2x2_4/2x2_8/2x3_6_14.  Their dispatch arms were
    #     already active in aoutarm9045.cpp; here their 2 static no-op stubs are
    #     removed + their 2 extern decls un-gated.  All route to a LIVE callee per
    #     the golden ENUM->CALLEE map.  Drove the substrate add of CheckDuplicateBarCode
    #     (golden aoutarm.h:130, offline true) in aoutarm_shims.cpp -- used by 1x4_4.
    aoutarm9045_1x4_4S.cpp
    aoutarm9045_1x4_4.cpp
    aoutarm9045_1x4_8.cpp
    aoutarm9045_2x1_2.cpp
    aoutarm9045_2x2_4.cpp
    aoutarm9045_2x2_8.cpp
    aoutarm9045_2x3_6_14.cpp
    # --- W6.2c-OUT batch-3 (FINAL 7 OUT-ARM site variants made ACTIVE; LIVE mode):
    #     2x3_6/2x4_4/2x4_8/2x5_8/2x6_8/2x8_8/All_1Picker.  This empties BOTH the
    #     gated-extern and static-stub blocks in aoutarm9045.cpp -- all out-arm
    #     variants now resolve to real translated funcs.  Drove the substrate add of
    #     TMyKitSuck::HasRealIC_Left/Right (golden MyKitSuck.h:294-295) used by
    #     2x5_8/2x6_8/2x8_8, and the REMOVAL of the temp MoveOutArmToShuttleIncludeZ_
    #     9045_2x4_4 stub (now defined for real in aoutarm9045_2x4_4.cpp).
    #     All_1Picker is DoOutArm-ONLY (picker-gated; no DoPickFromShuttle callee).
    aoutarm9045_2x3_6.cpp
    aoutarm9045_2x4_4.cpp
    aoutarm9045_2x4_8.cpp
    aoutarm9045_2x5_8.cpp
    aoutarm9045_2x6_8.cpp
    aoutarm9045_2x8_8.cpp
    aoutarm9045_All_1Picker.cpp
    # --- W6.2c-OUT cross-module offline shims: the out-arm ENGINE free-functions
    #     + SM-cursor globals the 6 variants call but whose golden bodies (aoutarm.cpp
    #     / aoutarm9045.cpp engine helpers / Magazine / aRotateKIT_Out / AutoTeach /
    #     FixAICCD) are not translated yet.  Offline-safe non-static defs so an exe
    #     linking ht9045_sm resolves them; the variants forward-declare each.
    aoutarm_shims.cpp
    # --- W6.2c-OUT batch-4 (DEAD): 6 out-arm site variants that are NEVER
    #     dispatched -- compiled as dead weight only (like in-arm 2x4_16/2x8_32).
    #     1x4_4_Back (enum present but DoOutArm ladder arm is an EMPTY //-body and
    #     DoPickFromShuttle routes it to LIVE 1x4_4); 2x2_4_14 (enum present, BOTH
    #     ladders route to LIVE 2x2_4); 2x2_4_23 / 2x4_16 / 2x8_16 (enums ABSENT
    #     from MachineType.h -> no dispatch arm); 2x8_32 (enum present, BOTH ladders
    #     route to LIVE 2x8_8).  Each carries the full _<deadv> suffix on its
    #     DoOutArm_9045_<deadv> + helpers so all symbols are UNIQUE never-called
    #     globals.  CMake-only: NO static-stub removal, NO extern un-gate, NO
    #     dispatch-ladder edit to aoutarm9045.cpp (the ladders are already 100% live).
    aoutarm9045_1x4_4_Back.cpp
    aoutarm9045_2x2_4_14.cpp
    aoutarm9045_2x2_4_23.cpp
    aoutarm9045_2x4_16.cpp
    aoutarm9045_2x8_16.cpp
    aoutarm9045_2x8_32.cpp
    # --- W6.2c-OUT B5 (S-prefix, dead-but-compiled): the LAST out-arm variant
    #     file.  aoutarm9045S_1x4_4.cpp carries the IRREGULAR _9045S_1x4_4 infix
    #     on its top-level export DoOutArm_9045S_1x4_4() AND all helpers.  VERIFIED
    #     DEAD: zero external callers in the golden tree (out-arm dispatch ladders
    #     route to the SUFFIX-S DoOutArm_9045_1x4_4S, defined in the DIFFERENT
    #     already-committed aoutarm9045_1x4_4S.cpp).  Compiled-only, like in-arm
    #     2x8_32: NO static-stub removal, NO extern un-gate, NO dispatch-ladder
    #     edit to aoutarm9045.cpp (dispatch unchanged -- file never dispatched).
    aoutarm9045S_1x4_4.cpp
    # --- W906-Automation INTEGRATE: Automation/automation.cpp -- TfAutomation,
    #     the OLP host-command engine's "network wiring + small helpers" slice
    #     (~34 methods + 6 free functions: CommandProcess/SendClient/SendServer,
    #     OLPServerClient*/OLPClient* socket event handlers, tmrOLPTimer,
    #     DoCommandBuffer, ShowCharHex/ShowRecord/SaveRecord/ShowMSG,
    #     GetHTMLJSONDatas/GetMachineStatus/GetWorkOrder/GetMainTemp/
    #     GetTemperatures, ...). Lives HERE (not a standalone library) because
    #     it depends on FormsFacade.h (fMain->palMainStatus/cbSetupFileName/
    #     edWorkTemperBase) + canary_support.h (RecordProcess) + auto9045.h
    #     (GetHandlerType/GetDataPath/GetMainStatus/...), all already carried
    #     by this library -- same rationale as Interface/InterfaceSYS.cpp /
    #     Interface/TesterTCP_Socket.cpp above. ProcessBuffer/ProcessBuffer1/
    #     SendReportRequest stay safe no-op stubs (deferred to a dedicated
    #     future wave, see the file's own header). INTEGRATE CHECK: golden's
    #     bare `fAutomation` global is already claimed by atester_shims.h's
    #     TfAutomationShim (consumed by atester.cpp/atester_32Site.cpp) -- this
    #     translation names its own real singleton `fAutomationEngine` instead
    #     (see automation.h's own "GLOBAL-NAME COLLISION AVOIDED" note); no ODR
    #     conflict confirmed by grep across the whole tree.
    Automation/automation.cpp
    # --- W906-D-Interface-TesterTCP INTEGRATE: Interface/TesterTCP.cpp -- a
    #     SIBLING slice of Interface/TesterTCP_Socket.cpp above (same golden
    #     TfTesterTCP form, a DISJOINT function subset: CopyRecipeToTester/
    #     CopyRecipeFromTester OS-recipe zip-transfer pair + the per-machine
    #     btnSaveClick arm-speed-save handler; TesterTCP_Socket.cpp keeps sole
    #     ownership of the ClientSocket_TCPIP connect/timer/read/error family --
    #     zero overlap, confirmed by both units' own translate reports). Lives
    #     HERE for the identical reason as its sibling: depends on common.h
    #     (DataPath/sSaveByMachine/WriteIniData, ht9045_core, transitively
    #     needed at final-link time) + canary_support.h (RecordProcess/
    #     ShowMyMessage) + the REAL offline MySleep(DWORD) already defined in
    #     acarry_shims.cpp below (this file's own extern redeclaration now
    #     resolves for real once compiled into this same library, matching the
    #     precedent already established by Automation/auto9045.cpp/
    #     CCLink/MyCCLink.cpp/KYECFTP/FTPClient_Transfer.cpp's identical
    #     extern-redeclaration idiom for this exact symbol).
    Interface/TesterTCP.cpp
    # --- W5 Automation wave (Automation network unit): TeraPower AMR tray-count
    #     bridge (TTeraPowerAMR / global `AMR`) + the 3 pure AGV AMR-mode
    #     predicates (AGV_IsSPIL_AMR/AGV_IsATK_AMR/AGV_Use_AMR, wired into
    #     FormsFacade's TfAGV -- see FormsFacade.cpp) + the HANA-ART SRQ protocol
    #     helper (uHANA_ART) + the 8-function SCK/ATK "Auto Retest" extract
    #     (Automation/SCK_ART.cpp, wired into csystem.cpp's W7C1/W7C2 SCKART
    #     seams -- see csystem.cpp).  Same footprint already carried by
    #     ht9045_sm: ht9045_globals/ht9045_motor/FormsFacade.h/canary_support.h.
    #     NOTE: Automation/AMR.cpp's global `AMR` retires acatchtray_shims.cpp's
    #     TAMRShim/AMR stopgap (ODR: same name, different type) -- see that
    #     file's comment.
    Automation/AGV_predicates.cpp
    Automation/AMR.cpp
    Automation/HANA_ART.cpp
    Automation/SCK_ART.cpp
    # --- W5-Final-AGV_E84 INTEGRATE: Automation/AGV_E84.cpp -- the E84
    #     loader/unloader handshake core (DoE84Loader/DoE84Unloader, 15/15
    #     case labels each, faithful) + ShowE84Log/E84StatusLog file logging.
    #     Same footprint as the AGV_predicates/AMR/HANA_ART/SCK_ART quartet
    #     just above (ht9045_globals/ht9045_motor/FormsFacade.h/
    #     canary_support.h); additionally now uses fAGV->mmE84Log (see
    #     FormsFacade.h/.cpp TfAGV::mmE84Log, added this integrate).
    Automation/AGV_E84.cpp
    # --- W5-Automation-cont INTEGRATE: Automation/AGV_PortScan.cpp -- the
    #     second AGV.cpp slice (after AGV_E84.cpp's E84 handshake core and
    #     AGV_predicates.cpp's 3 AMR predicates, both above): E84 sensor-status
    #     telemetry (NowE84Status/E84StatusChange) + the AMR/AGV port-scan
    #     dispatcher family (ScanLoadPort + 8 bScan*/bIsStackBusy free functions),
    #     13 functions total.  Same footprint as the AGV_E84.cpp/AGV_predicates.cpp
    #     cluster (ht9045_globals/ht9045_motor/FormsFacade.h/canary_support.h),
    #     plus 3 new FormsFacade stand-in members this unit added (TfMain::ALed1/
    #     labAutomation, TfLotInfo::ALedLoader/aLedAuto[3]/palRemoveTray -- see
    #     FormsFacade.h/.cpp "W5-Automation ADD" blocks) and #includes AGV_E84.h
    #     (reuses its iLoaderScanTask[]/iUnloaderScanTask[]/bE84Loaderflag[]/
    #     bE84Unloaderflag[] externs, does not redeclare) + AGV_predicates.h.
    Automation/AGV_PortScan.cpp
    # --- W5-Final-SckArtRemainder INTEGRATE: Automation/SCK_ART_Remainder.cpp --
    #     the next contiguous golden slice (SetSetupFilePath/AccessFile/
    #     SetGPIBVersion/UpdateCount/AddAlarmCode/ClearLotInfo/AddOutputJamCnt/
    #     SaveTestSummary-dispatcher) continuing the sibling SCK_ART.cpp 8-
    #     function extract just above.  Deliberately owns its OWN
    #     SckArtRemainderState (NOT the sibling's SckArtState) -- see the
    #     file's own header for the 3-way reconciliation debt this creates
    #     (sibling SckArtState / this file's SckArtRemainderState / csystem.cpp's
    #     W7C2_TfSCKARTSeam) for dCurrYield/iFTRTCount/iNeedRT/iInputCount/
    #     iManualRejectCnt/iTesterType/sLOTSTATUS/iCurrentStatus -- NOT resolved
    #     by this integrate (would require touching the sibling file's
    #     internals, out of this front's scope; flagged for a future
    #     TfSCKART/FormsFacade unification wave).
    Automation/SCK_ART_Remainder.cpp
    # --- W5-Final-Auto9045 INTEGRATE: Automation/auto9045.cpp -- the whole OLP/
    #     GPIB host-command config-accessor layer (148 golden free functions,
    #     confirmed NOT a VCL TForm). Same footprint as the Automation cluster
    #     above; all VCL-form gaps this file reaches (fBinSel/fShowBinSelect/
    #     fTrayAssignment/fTestCategory/fContactCT/fCounterClear/fMonitor/
    #     fProductionInfo/fTemp_Set + extra fMain/fObserver/fLotInfo/fContact/
    #     fAutomation members) stay as this file's own TU-local `W5FA_`
    #     companion objects -- NOT folded into FormsFacade.h/atester_shims.h by
    #     this integrate (would each need a brand-new shared form class with no
    #     other consumer yet; deferred, see this integrate's own report).
    Automation/auto9045.cpp
    # --- W7 (TRAY-ARM engine): the top dispatcher DoCatchTray (flat switch) + 8
    #     pumpable task SM skeletons (DoCatchFromLoader / CatchNewTrayFromBuffer /
    #     DoPlaceTrayToAuto / DoPlaceToBuffer / DoSlapTray / DoCatchUnderTray /
    #     DoSupportUnderTray / DoPlaceBufferTray) + cover-tray/RFID/cassette SMs +
    #     the pure decision helpers (WhichAutoNeedTray/IsOutArmSafe/...).  No
    #     site-variant dispatch in this file.  Hardware behind the Sim HAL
    #     (MOT[]/Cylinder[]/Sen[]/SW[]/CatchTraySuck); cross-module/UI surface
    #     (fTrayMapping/LoadCCD/MyMessageBox/HSys.BinDisCtrl) behind offline
    #     shims + ACTIVE stubs; AMR is REAL now (Automation/AMR.cpp above); see
    #     the file-head gate documentation.  OWNS the real WhichAutoNeedTray()
    #     (the canary_support.cpp sim stub was removed).
    acatchtray.cpp
    # --- W6.3 TRAY-ARM cross-module offline shims (fTrayMapping / MyMessageBox /
    #     LoadCCD / SECS_EVENT+EventReport / Tray-Mapping-laser / AMR-feed /
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
    # --- W64b (TESTER/INDEX engine): the Front (Z1/Arm1) and Rear (Z2/Arm2)
    #     "Destroy IC" + "Suck IC" down-press state machines (DoFrontTestDestroyIC /
    #     DoRearTestDestroyIC / DoFrontTestSuckIC / DoRearTestSuckIC +
    #     CheckZ1IsDown/CheckZ2IsDown/TestZ1OutRandge/TestZ2OutRandge/
    #     TestZ1SetPos/TestZ2SetPos leaf deps), faithfully translated -- no
    #     longer offline shims for these symbols (see atester_shims.cpp note
    #     below for what of Front/Rear is still shimmed).
    aTester_Front.cpp
    aTester_Rear.cpp
    # --- W5-atester32site / W5-ProcessCount-Translate: the 32-Site dual-arm
    #     test-head engine (all 8 golden functions, 2 vision islands internally
    #     gated -- see the file's own header) + the ProcessCount/low-yield-alarm
    #     leaf subset (14 golden functions: RecordPiggyBackStartEnd/
    #     ProcessPiggyBackFunction/InitialPiggyBackFunction/ProcessSocketPurgeCount/
    #     DoLowYieldAlarm/DoLowYieldAlarmFirst/CheckContinuoussFail/
    #     ProcessContinuoussFailForATC/GetMultiplierNum/CheckContinuoussPassBySocket/
    #     CheckContinuoussPass/ProcessHeadContactCount/ProcessSocketCount/
    #     ProcessAutoloadcellMeasureCount).  Both retire their corresponding
    #     stub entries from atester_shims.cpp/.h (see that file's remaining
    #     scope note below).
    atester_32Site.cpp
    atester_ProcessCount.cpp
    # --- W6.4 TESTER/INDEX cross-module offline shims (fContact / ADAM_* /
    #     CCDInterfaceForm / fAutomation / fObserver / fiosetview / COM2 / IsNNMode /
    #     CheckHeaterOK; aTester_Front/Rear's Destroy+Suck pairs and the whole of
    #     atester_32Site/atester_ProcessCount are real now -- see above -- every
    #     OTHER Front/Rear export, e.g. DoTestYFront/TestZ1OutRandge2, plus the
    #     ProcessCount exports still out of this wave's scope [ProcessShowTestStatus/
    #     ProcessCount/ProcessStartTestData/RecordHistroy/ProcessQASampling], stay
    #     offline shims here).
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
    # --- W5-BarCode (BarCode front, 2026-07-11): real bodies for 18 of the 20
    #     TfBarCode_Shim methods (aHotPlateSubstrate.h:623-664).  7 parallel
    #     translate units extracting SPECIFIC functions from the still-largely-
    #     untranslated ~24,500-line BarCode/{BarCode,BarCode_Sh1,BarCode_Sh2,
    #     BarcodeXML}.cpp VCL-form core (see MIGRATION_ROADMAP.md DEFERRED
    #     table).  DoBarcodeScanInShuttle_2/DoShuttleFloatCheck_2 were NOT
    #     delivered this wave (clean hand-off, BarCode_Shuttle2_Scan's own
    #     report) -- those 2 TfBarCode_Shim methods keep their pre-existing
    #     offline stub.  BarCode_Bottom2DID8CCD.{h,cpp} intentionally live at
    #     project ROOT (not under BarCode/), per that unit's exact task-brief
    #     path -- see its own file banner.
    #     INTEGRATE FIX: resolved 2 real cross-unit ODR conflicts before wiring
    #     (BarCode_Shuttle2_Scan.cpp's own bCCDcheckShuttle[2] vs. the existing
    #     cmydef.cpp global; BarCode_Shuttle2_CCDScan.cpp's independent
    #     redefinition of 8 golden TfBarCode-member globals already owned by
    #     BarCode_Bottom2DID.cpp) -- both units' own translate reports flagged
    #     these; see aHotPlateSubstrate.cpp's include-block comment for the
    #     resolution.
    BarCode/BarCode_Helpers.cpp
    BarCode/BarCode_Bottom2DID.cpp
    BarCode_Bottom2DID8CCD.cpp
    BarCode/BarCode_Shuttle1_Scan.cpp
    BarCode/BarCode_Shuttle2_Scan.cpp
    BarCode/BarCode_Shuttle1_CCDScan.cpp
    BarCode/BarCode_Shuttle2_CCDScan.cpp
    # --- W5-BarCode-Final (BarCode remainder front, 2026-07-11): the last 2/20
    #     TfBarCode_Shim methods (DoBarcodeScanInShuttle_2/DoShuttleFloatCheck_2,
    #     the exact "REMAINING / HANDED OFF" leftover flagged above) + the
    #     8-CCD ClientSocket connection-lifecycle glue (SendCCDCommand's REAL
    #     body + the 13 real TClientSocket globals + the Bottom-8CCD Connect/
    #     Init timer pair).  TfBarCode_Shim is 20/20 real methods as of this
    #     integrate.  See aHotPlateSubstrate.cpp's include-block comment.
    #     INTEGRATE DECISION (BarCode-8CCD-Glue's own flagged question):
    #     the 6 pre-existing per-unit gated Gated_SendCCDCommand-style no-op
    #     stand-ins (BarCode_Bottom2DID.cpp, BarCode_Shuttle1_CCDScan.cpp,
    #     BarCode_Shuttle1_Scan.cpp, BarCode_Shuttle2_CCDScan.cpp,
    #     BarCode_Shuttle2_Scan.cpp, BarCode_Bottom2DID8CCD.cpp) are LEFT
    #     SEPARATE, NOT retargeted onto the new real ::SendCCDCommand -- (1)
    #     those 6 files are already-integrated production code outside this
    #     front's 3-unit scope, and this campaign's whole premise is that
    #     serial integrates avoid touching files another front owns; (2) each
    #     already has a passing oracle test asserting its own no-op/log
    #     behaviour -- retargeting risks a silent behavioural change (real
    #     SendCCDCommand's own log format + Socket->Active/SendText branches)
    #     for zero required benefit, since every gated stand-in is already
    #     faithful to a disconnected-CCD machine; (3) consolidating call sites
    #     is a cleanup, not a correctness fix -- better done as its own
    #     deliberate follow-up wave with full re-verification of all 6 affected
    #     test suites, not a side effect of this hand-off completion.
    BarCode/BarCode_Shuttle2_ScanRemainder1.cpp
    BarCode/BarCode_Shuttle2_ScanRemainder2.cpp
    BarCode/BarCode_8CCD_Glue.cpp
)

target_include_directories(ht9045_sm PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ht9045_sm PUBLIC
    ht9045_io ht9045_motor ht9045_globals vclcompat
    # W5-comms INTEGRATE: EventReport()/SECS_EVENT (csystem.cpp) + link-
    # completeness for Interface/InterfaceSYS.cpp (see source-list comment above).
    ht9045_secsgem)

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
# Library: ht9045_kyecftp  (W5-Final KYECFTP wave -- FTP client engine +
#   TfFTPClient session logic)
#
#   KYECFTP/MiniFtpEngine.{h,cpp}            -- Nmftp::TNMFTP: a from-scratch FTP
#     protocol engine standing in for the closed BCB6 TNMFTP component (no vendor
#     source exists anywhere in this repo -- see the file's own banner for every
#     inferred wire-behavior judgment call: async->sync, PASV-only, Vendor/
#     ParseList inert, CurrentDir's live PWD round-trip, Mode()'s MODE_BYTE->
#     "TYPE L 8" mapping, 502->OnUnSupportedFunction classification). Built on
#     vclcompat/ClientSocket.h (one control-channel connection + a fresh PASV
#     data-channel connection per transfer). Zero dependency beyond vclcompat.
#   KYECFTP/FTPClient_EventHandlers.{h,cpp}  -- TfFTPClient's NMFTP1 event-handler
#     bodies (status/success/failure logging), as free functions matching
#     TNMFTP's std::function callback-slot signatures. Needs CUSTOMER_CODE
#     (cmydef.cpp) + MyDBIProcess (aHotPlateSubstrate.cpp) + ShowMyMessage
#     (canary_support.cpp).
#   KYECFTP/FTPClient_Transfer.{h,cpp}       -- the 4 real FTP-transfer session
#     functions (LoadFileFormServer2/UploadFileToServer2/Download_2DSortingList/
#     Download_2DID_WhiteList), built against both siblings above. Needs
#     CUSTOMER_CODE/IniConfig/CosFunction/Temperature (cprod.cpp/cmydef.cpp) +
#     DataPath/OffsetPath/DefaultPath/LastDataPath/asATCFileTransferPath/
#     sWhite2DIDListLoc (common.cpp) + MyDBIProcess/NewRecordProcess/MySleep/
#     ShowMyMessage/RecordProcess (aHotPlateSubstrate.cpp/acatchtray_shims.cpp/
#     acarry_shims.cpp/canary_support.cpp, all in ht9045_sm).
#
#   PRODUCTION link surface: vclcompat (MiniFtpEngine's transport) + ht9045_sm
#   (CUSTOMER_CODE/IniConfig/CosFunction/Temperature transitively via
#   ht9045_globals, + the 5 real free-function bodies above) + ht9045_core
#   (common.cpp's DataPath family -- ht9045_sm does NOT itself link ht9045_core,
#   see that library's own link line above, so it must be named explicitly here
#   too). NOT exercised end-to-end by this library's own unit tests below --
#   see those targets' own comments in tests/CMakeLists.txt for why: each test
#   supplies TU-local stand-in globals/functions instead, deliberately avoiding
#   the cmydef/cprod/common/canary_support/aHotPlateSubstrate "god-stack" for a
#   fast, focused unit test (same avoidance already established by
#   tests/test_config_loaders.cpp / tests/test_MyCCLink.cpp) -- ht9045_kyecftp
#   itself compiling clean here is what proves these 3 files type-check against
#   the REAL header declarations (cmydef.h/cprod.h/Config.h/CosFunction.h/
#   common.h/canary_support.h), which the units' own narrower manual
#   verification could not check.
#
#   No circular dependency: grepped the whole tree (production + tests) for any
#   existing caller of LoadFileFormServer2/UploadFileToServer2/
#   Download_2DSortingList/Download_2DID_WhiteList/Nmftp:: outside KYECFTP/
#   itself -- zero hits. ht9045_sm/ht9045_core need nothing from ht9045_kyecftp,
#   so this is a clean one-way dependency (unlike Interface/InterfaceSYS.cpp,
#   which had a genuine two-way need and was folded directly into ht9045_sm's
#   own source list instead -- see that library's comment above).
# ---------------------------------------------------------------------------
add_library(ht9045_kyecftp
    KYECFTP/MiniFtpEngine.cpp
    KYECFTP/FTPClient_EventHandlers.cpp
    KYECFTP/FTPClient_Transfer.cpp
)

target_include_directories(ht9045_kyecftp PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ht9045_kyecftp PUBLIC vclcompat ht9045_sm ht9045_core)

if(WIN32)
    target_link_libraries(ht9045_kyecftp PUBLIC ws2_32)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # Same relaxed posture as ht9045_sm/ht9045_comms: FTPClient_EventHandlers.cpp/
    # FTPClient_Transfer.cpp #include the BCB6-idiom-heavy global headers
    # (cmydef.h/cprod.h/Config.h/CosFunction.h/common.h). MiniFtpEngine.cpp itself
    # (zero god-stack dependency) is independently clean under the full strict
    # set per its own unit's report; no per-file carve-out is needed since the
    # relaxed flags here are a strict subset of warnings, not additions.
    target_compile_options(ht9045_kyecftp PRIVATE
        -Wall -Wextra -Wno-conversion-null -Wno-write-strings -Wno-unused-parameter)
endif()

# ---------------------------------------------------------------------------
# Tests
# ---------------------------------------------------------------------------
enable_testing()
add_subdirectory(tests)
