// =============================================================================
//  test_dfm2rc_pipeline.cpp -- W7-B1d ctest wrapper around run_b1d.py.
//
//  AI(W906-W7-B1d) 20260728: new file (docs/W7_UI_ARCHITECTURE_PLAN.md S6
//  W7-B1d "全語料跑 + 閘門分流 + CMake 接線"). All the real gate logic
//  (parsing golden .dfm, emitting .rc/_ids.h/_layout.gen.{h,cpp}, compiling
//  with rc.exe/windres/g++, the G0-G9 cross-checks) lives in Python
//  (tools/dfm2rc/*.py) -- rewriting any of that in C++ here would be a
//  second, divergent implementation of the exact same rules, which is
//  precisely what this project's conventions (and gates.py's own
//  docstring) warn against. This wrapper's only job is to be a real,
//  compiled ctest executable so it (a) inherits ht9045_test_bootstrap
//  automatically via the directory-scoped link_libraries() at the top of
//  tests/CMakeLists.txt (SetErrorMode etc. -- see that file's own
//  banner), and (b) gives ctest a normal PASS/FAIL exit code instead of a
//  bare script invocation.
//
//  One binary, 4 different `add_test(... COMMAND test_dfm2rc_pipeline
//  <mode> ...)` entries in tests/CMakeLists.txt -- ctest natively supports
//  reusing one executable across several tests with different argv, so
//  there is no need for 4 near-identical .cpp files. See run_b1d.py's own
//  module docstring for exactly what each of the 4 modes
//  (fidelity|idempotent|rc_compiles|layout_full) proves.
//
//  AI(W906-W7-B1d) 20260728: uses _spawnvp(), NOT std::system(), to launch
//  python.exe. GOLDEN BUG-class trap found and fixed empirically, not
//  guessed: std::system() on Windows shells the command through
//  `cmd.exe /c <string>`, and cmd.exe's own argument re-parsing has a
//  well-known quirk -- its "preserve quotes" fast path only applies when
//  the WHOLE command line contains exactly 2 quote characters; with more
//  than one quoted path (our exe path + our script path, 4 quotes total)
//  it falls back to "strip the first character, then strip the LAST quote
//  character in the entire line", which severs the closing quote off the
//  wrong (first) argument and glues the two paths into one bogus filename
//  containing a literal embedded quote+space. The observed, reproduced
//  symptom was cmd.exe's own (Big5-locale) "the filename, directory name,
//  or volume label syntax is incorrect" error -- caught via `ctest
//  --output-on-failure` on this exact wrapper before trusting it, not
//  anticipated in advance. _spawnvp() bypasses cmd.exe entirely: each
//  argv element is passed as its own array slot and the CRT applies the
//  standard CreateProcess-compatible quoting per-argument, so this class
//  of double-quoting collision cannot occur regardless of how many
//  arguments contain spaces.
//
//  This process never raises a window: it only ever launches python.exe,
//  which in turn only launches rc.exe/windres.exe/g++.exe -- all pure
//  console tools with no GUI code path (verified: none of them are
//  Debug-CRT MSVC binaries). No additional SetErrorMode guard beyond the
//  inherited bootstrap is needed here for that reason -- unlike a probe
//  that itself does GUI/CRT work.
// =============================================================================
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <string>
#include <vector>

#if defined(_WIN32)
#include <process.h>
#else
#error "test_dfm2rc_pipeline.cpp is Windows-only (spawns rc.exe/windres.exe via _spawnvp)"
#endif

#ifndef HT9045_PYTHON_EXE
#error "HT9045_PYTHON_EXE must be defined by CMake -- see tests/CMakeLists.txt W7-B1d block"
#endif
#ifndef HT9045_DFM2RC_DIR
#error "HT9045_DFM2RC_DIR must be defined by CMake -- see tests/CMakeLists.txt W7-B1d block"
#endif

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr,
                "usage: %s <fidelity|idempotent|rc_compiles|layout_full> [extra-arg...]\n",
                argv[0]);
        return 2;
    }

    const std::string python_exe = HT9045_PYTHON_EXE;
    const std::string script = std::string(HT9045_DFM2RC_DIR) + "/run_b1d.py";

    // Build the argv vector for _spawnvp: [python_exe, script, mode, extra..., NULL].
    // _spawnvp wants char* (not const char*) per its historical C signature,
    // but never actually mutates argv[i] -- the const_cast is the standard,
    // safe idiom for this API (identical to what argv itself already is).
    std::vector<std::string> args;
    args.push_back(python_exe);
    args.push_back(script);
    for (int i = 1; i < argc; ++i)
    {
        args.push_back(argv[i]);
    }

    std::vector<char *> spawn_argv;
    for (size_t i = 0; i < args.size(); ++i)
    {
        spawn_argv.push_back(const_cast<char *>(args[i].c_str()));
    }
    spawn_argv.push_back(nullptr);

    printf("[test_dfm2rc_pipeline] spawning:");
    for (size_t i = 0; i + 1 < spawn_argv.size(); ++i)
    {
        printf(" %s", spawn_argv[i]);
    }
    printf("\n");
    fflush(stdout);

    int rc = static_cast<int>(_spawnvp(_P_WAIT, python_exe.c_str(), spawn_argv.data()));
    if (rc == -1)
    {
        fprintf(stderr, "[test_dfm2rc_pipeline] _spawnvp failed to launch %s: errno=%d\n",
                python_exe.c_str(), errno);
        return 3;
    }

    printf("[test_dfm2rc_pipeline] exit code: %d\n", rc);
    return rc;
}
