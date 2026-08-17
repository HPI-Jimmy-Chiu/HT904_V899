# GA-4 integration proposal — first-light fMain (agent E deliverable)

> AI(W906-GA4) 20260804: proposal ONLY — per D-GA4-10 every existing file
> (CMakeLists.txt / ui/HT9045App.* / scripts/build_msvc_ui.bat) is edited by
> the main loop by hand. Everything below was desk-checked against the tree
> as of this writing (file:line cites are to current content). New files this
> agent wrote: `ui/forms/FMainFirstLightDlg.{h,cpp}` — nothing else.

---

## 1. CMakeLists.txt — the `if(HT9045_UI)` block (currently lines 1822–1890)

### 1.1 New shared source list (place right after `HT9045_UI_CTRL_SOURCES`, ~line 1855)

```cmake
    # AI(W906-GA4) 20260804: GA-4 -- the four table-driven UI engines, the
    # first-light fMain dialog, and the two GENERATED data tables they bind
    # (layout: D7 pixel authority; uimap: D-GA4-1 symbol->numeric join).
    # Shared between ht9045_app and headless_fmain_probe for the same
    # never-drift reason as HT9045_UI_CTRL_SOURCES above: the probe must
    # exercise the exact TUs the app ships, including the dialog class it
    # does not instantiate (the probe drives CreateDialogParamW itself,
    # D-GA4-9 -- compiling the dialog in keeps it link-checked both places).
    set(HT9045_UI_GA4_SOURCES
        ui/layout/FormRuntime.cpp
        ui/layout/DialogTreeEngine.cpp
        ui/layout/ApplyLayoutEngine.cpp
        ui/layout/CtlColorEngine.cpp
        ui/layout/CustomCtrlAttach.cpp
        ui/forms/FMainFirstLightDlg.cpp
        tools/dfm2rc/layout_out/main_layout.gen.cpp
        tools/dfm2rc/rc_out/main_uimap.gen.cpp
    )

    # GA-4 include roots: generated headers are included by BARE NAME
    # ("main_ids.h", "DfmLayoutTypes.h", "main_uimap.gen.h") per the GA-4
    # include convention, so all three generator output dirs join ui/ on the
    # include path of every UI target.
    set(HT9045_UI_GA4_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/ui
        ${CMAKE_CURRENT_SOURCE_DIR}/ui/layout
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/dfm2rc/layout_out
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/dfm2rc/rc_out
    )
```

### 1.2 `ht9045_app` changes (current lines 1857–1876)

```cmake
    add_executable(ht9045_app WIN32
        ui/HT9045App.cpp
        ui/GateAPlaceholderDlg.cpp          # stays compiled; launch-unreachable after the swap (see §2)
        ${HT9045_UI_CTRL_SOURCES}
        ${HT9045_UI_GA4_SOURCES}
        ui/app.rc
        tools/dfm2rc/rc_out/main.rc         # second .rc: the 91 generated fMain DIALOGEX templates
    )
```

and replace the single-dir include line (current line 1874) with:

```cmake
    target_include_directories(ht9045_app PRIVATE ${HT9045_UI_GA4_INCLUDE_DIRS})
```

**Two .rc files in one target — resource-ID collision check (done, state of
the tree today):**

* `main_ids.h` (generated): 91 `IDD_*` values **8192–8282**, 758 `IDC_*`
  values **1024–1781** (measured from the file, not assumed).
* `ui/resource.h` (placeholder): `IDD_GATEA_PLACEHOLDER 9045`,
  `IDC_GATEA_BANNER 9046`.
* Disjoint on both axes → rc.exe/CVTRES cannot see a duplicate
  (type, id) pair. Note 9045/9046 sit *above* the generated IDD base 8192 —
  today's 91 dialogs stop at 8282, but if a future multi-form .rc round ever
  allocates ≥ 850 more dialog IDs the generated range would reach 9045.
  When placeholder removal happens (integrator's call, §2) this ceases to be
  a live constraint; until then it is a known, measured margin (763 IDs).
* Neither .rc carries VERSIONINFO / manifest resources (grep-verified in
  both), so no singleton-resource clash either. When a version resource is
  added later, put it in **app.rc only**.

### 1.3 New probe target (place after the `ht9045_ctrl_probe` block, ~line 1889)

```cmake
    # AI(W906-GA4) 20260804: GA-4 headless first-light probe (D-GA4-9). Runs
    # the full engine lifecycle over the real generated tables and templates
    # with NO window ever shown: the root template has no WS_VISIBLE
    # (main.rc IDD_MAIN) and the probe never calls ShowWindow -- hiding is
    # by construction, not discipline. A console exe CAN carry dialog
    # resources: .res data is linked into any PE regardless of subsystem,
    # and CreateDialogParamW finds the templates through the module handle
    # (AfxGetInstanceHandle) -- that is why main.rc is a source here too.
    add_executable(headless_fmain_probe
        ui/tests_headless/headless_fmain_probe.cpp
        ${HT9045_UI_CTRL_SOURCES}
        ${HT9045_UI_GA4_SOURCES}
        tools/dfm2rc/rc_out/main.rc
    )
    target_compile_definitions(headless_fmain_probe PRIVATE _AFXDLL)
    target_include_directories(headless_fmain_probe PRIVATE ${HT9045_UI_GA4_INCLUDE_DIRS})
    target_link_libraries(headless_fmain_probe PRIVATE ht9045_public)
```

`ht9045_ctrl_probe` is deliberately **untouched**: it tests the shells in
isolation; adding the GA-4 sources there would add a second consumer of
main.rc without new coverage. (Task brief said "the probes" — desk-check
says only the new probe needs the GA-4 TUs; flagging rather than silently
deciding.)

### 1.4 Link graph

No new libraries. `ht9045_public` (already linked by both targets) exports
the repo root include dir PUBLIC (CMakeLists.txt:352-354) — that is what
resolves `"Public/cBootLog.h"` and `"vclcompat/..."` from ui/forms/ — and
brings `vclcompat` transitively (CMakeLists.txt:356). The engines depend on
nothing beyond that plus the CTRL sources already in both targets.

---

## 2. ui/HT9045App.* swap (minimal, unified-diff style)

### 2.1 `ui/HT9045App.h` — two new flag members (after line 43 `m_bDevPathBypass`)

```diff
     //   --devpath      bypass golden's "exe must live in D:\HT9045\EXE\" check
     //                  (golden HT9045.cpp:151-155) so the build-dir exe can run
     //                  during development. Without it the check is FAITHFUL:
     //                  MessageBox + exit, same as golden.
+    //   --zorder       GA-4 (D-GA4-5): reverse every dialog's sibling chain once
+    //                  after layout -- GA-5's z-order polarity experiment.
+    //   --center       GA-4 (D-GA4-2): CenterWindow() after layout. Dev nicety in
+    //                  --devpath's spirit (golden fMain sits at 1057/489).
     int   m_iSmokeCloseMs;   // 0 = interactive
     bool  m_bDevPathBypass;
+    bool  m_bZOrderFlip;     // AI(W906-GA4) 20260804: --zorder flag storage
+    bool  m_bCenterOnScreen; // AI(W906-GA4) 20260804: --center flag storage
```

### 2.2 `ui/HT9045App.cpp`

```diff
 #include "HT9045App.h"
 #include "GateAPlaceholderDlg.h"
+#include "forms/FMainFirstLightDlg.h"   // AI(W906-GA4) 20260804: GA-4 first-light fMain
 #include "RegisterCustomClasses.h"   // AI(W906-GateA-4-C5) 20260804: HT9045_RegisterAllCustomClasses
```

```diff
 CHT9045App::CHT9045App()
     : m_iSmokeCloseMs(0)
     , m_bDevPathBypass(false)
+    , m_bZOrderFlip(false)
+    , m_bCenterOnScreen(false)
 {
 }
```

```diff
         if (lstrcmpA(__argv[i], "--smoke") == 0 && i + 1 < __argc)
             m_iSmokeCloseMs = atoi(__argv[++i]);
         else if (lstrcmpA(__argv[i], "--devpath") == 0)
             m_bDevPathBypass = true;
+        else if (lstrcmpA(__argv[i], "--zorder") == 0)    //AI(W906-GA4) 20260804: D-GA4-5 experiment switch
+            m_bZOrderFlip = true;
+        else if (lstrcmpA(__argv[i], "--center") == 0)    //AI(W906-GA4) 20260804: D-GA4-2 dev nicety
+            m_bCenterOnScreen = true;
```

```diff
-        CGateAPlaceholderDlg dlg(m_iSmokeCloseMs);
+        // AI(W906-GA4) 20260804: GA-4 swap -- generated fMain replaces the Gate A
+        // placeholder. GateAPlaceholderDlg.{h,cpp} stay compiled and its DIALOGEX
+        // stays in app.rc, but both are now launch-unreachable; removing them is
+        // the integrator's call, not this wave's.
+        CFMainFirstLightDlg dlg(m_iSmokeCloseMs, m_bZOrderFlip, m_bCenterOnScreen);
         m_pMainWnd = &dlg;
         dlg.DoModal();                                    // [DEV-4]
```

**Position invariant (do not reorder):** the dialog block must stay AFTER
`HT9045_RegisterAllCustomClasses()` (HT9045App.cpp:121). IDD_MAIN's child
templates instantiate `HT9045.*` window classes (222 ALed + 56 MyTray + ...);
one unregistered class fails the whole CreateDialog — the exact failure the
registration call's own comment warns about. The current ordering already
satisfies this; the swap keeps the block in place.

**Class name note:** the class is `CFMainFirstLightDlg` (MFC C-prefix house
style, matching `CGateAPlaceholderDlg` / `CLedCtrl`); files are
`FMainFirstLightDlg.{h,cpp}` as commissioned.

---

## 3. scripts/build_msvc_ui.bat hookup

Insert between the ctrl-probe run (ends line 93) and the smoke run (line 95):

```bat
REM AI(W906-GA4) 20260804: GA-4 headless first-light probe (D-GA4-9). Builds
REM the whole fMain dialog tree from the generated tables with no window ever
REM shown (root template lacks WS_VISIBLE; the probe never calls ShowWindow).
REM Runs BEFORE the smoke run: if the tree/layout/attach machinery is broken,
REM fail here with per-check PASS/FAIL lines instead of a mute smoke exit code.
echo [build_msvc_ui] Building headless_fmain_probe...
cmake --build "%BUILD_DIR%" --target headless_fmain_probe
if errorlevel 1 (
    echo [build_msvc_ui] FATAL: headless_fmain_probe build failed.
    exit /b 1
)

echo [build_msvc_ui] Headless fMain first-light probe...
"%REPO_ROOT%\%BUILD_DIR%\headless_fmain_probe.exe"
set "FMAIN_RC=%ERRORLEVEL%"
echo [build_msvc_ui] fmain probe exit code: %FMAIN_RC%
if not "%FMAIN_RC%"=="0" (
    echo [build_msvc_ui] FATAL: headless fMain probe reported failures.
    exit /b %FMAIN_RC%
)
```

Also update the stale banner comment at line 13-14 ("Builds ONLY the
ht9045_app target") — the script now builds three targets.

---

## 4. Everything else the integrator must not forget

1. **`/utf-8` is already global** for MSVC (CMakeLists.txt:126,
   `add_compile_options(/utf-8 /W3)` inside the `if(MSVC)` branch) — no
   per-target flag needed. This matters: `main_uimap.gen.cpp` carries UTF-8
   CJK caption literals (TTabSheet captions, D-GA4-1) and
   FMainFirstLightDlg.cpp assumes the same source charset.

2. **rc.exe include path for `main_ids.h`** (when compiling
   `tools/dfm2rc/rc_out/main.rc` from the build dir): double-covered, but
   verify at configure time.
   * CMake's Ninja generator passes the target's `INCLUDE_DIRECTORIES` to
     the RC compile rule (the `RC` language rule expands the same
     `${INCLUDES}` as C/CXX), so `HT9045_UI_GA4_INCLUDE_DIRS` (which
     contains `tools/dfm2rc/rc_out`) should reach rc.exe as `/I` flags.
   * Independently, rc.exe resolves quoted `#include "main_ids.h"` relative
     to the directory of the including .rc file, and main_ids.h sits next
     to main.rc.
   * If the first build still errors with RC1015 (cannot open include
     file), the explicit escape hatch is:
     `set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} /I \"${CMAKE_CURRENT_SOURCE_DIR}/tools/dfm2rc/rc_out\"")`
     placed inside the `if(HT9045_UI)` block before the targets. I could
     not run a build to prove which of the two mechanisms fires first
     (hard no-build rule this wave), hence stating the flag explicitly.

3. **Charset:** the UI targets build **ANSI MFC** (no `_UNICODE`/`UNICODE`
   definition anywhere in the block — verified). The engines use explicit
   `*W` APIs where wide is required (`CreateDialogParamW`,
   `TCM_INSERTITEMW`, `CreateFontIndirectW` per D-GA4-3/-6). Do NOT add
   `_UNICODE` while integrating; `WriteBootLog`/`AnsiString` and the dialog
   are narrow on purpose.

4. **`main_uimap.gen.{h,cpp}` must exist before configure.** They are the
   parallel agent's `emit_uimap.py` output (D-GA4-1) and are NOT in
   `tools/dfm2rc/rc_out/` at the time of this writing.
   FMainFirstLightDlg.cpp includes `"main_uimap.gen.h"` and binds
   `ht9045_uimap::kmainUiMap` — the name follows the established generated
   convention (`ht9045_layout::kmainLayout`, main_layout.gen.h:22) and the
   commissioning brief's own `&kmainUiMap` wording. If the generator emitted
   a different header/symbol name, reconcile FMainFirstLightDlg.cpp:17/57
   (one include + one identifier).

5. **Visibility semantics of DoModal vs the probe:** IDD_MAIN has no
   `WS_VISIBLE` (main.rc). The interactive exe still shows it — MFC's
   `DoModal` runs `RunModalLoop(MLF_SHOWONIDLE)`, which shows the dialog
   when the queue idles. The probe path (plain `CreateDialogIndirectParam`
   / `CreateDialogParamW`, no ShowWindow, never idle-shown) stays invisible
   by construction. Both behaviours are intended; do not "fix" the template
   by adding WS_VISIBLE.

6. **Smoke oracle lines** now written to `D:\HT9045\Error\BootLog.txt` by a
   `--devpath --smoke 800` run, one line each, pipe-separated (grep-able if
   the .bat later wants content assertions, not just exit codes):
   * `FMain BuildDialogTree OK | dialogs=91|controls=...|tabhosts=...`
   * `FMain ApplyLayoutPass | moved=...|fonts_set=...|hidden=...|rescaled=...|missing=...`
   * `FMain AttachCustomControls | leds=...|btnpanels=...|trays=...|missing=...`
   `missing=0` on both stat lines is the health criterion (ApplyLayoutEngine.h:45,
   CustomCtrlAttach.h:41). On tree-build failure the line is
   `FMain BuildDialogTree FAILED -- aborting first light` and DoModal
   returns IDABORT (distinguishable from a user close, IDCANCEL/IDOK).

7. **Timer id 1** is reused from the placeholder as the smoke-close timer
   (Gate A convention); `CLedCtrl` reserves `0x9045` (CLedCtrl.h:78).
   Disjoint by design; no action needed, just don't allocate id 1 for
   anything else on the root HWND.

8. **`ExitInstance` exit-code contract still holds:** HT9045App.h:47-52's
   override already forces exit code 0 regardless of the modal result, so
   the smoke exit code stays 0 even though first light can now end in
   IDABORT. The smoke run's pass/fail signal is therefore the BootLog stat
   lines (point 6) plus the fmain probe's own exit code — flagging so
   nobody assumes the smoke exit code alone proves the tree built.
