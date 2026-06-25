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
