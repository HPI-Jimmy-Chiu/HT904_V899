// =============================================================================
//  tools/wb_serve.cpp -- run the web HMI against the REAL handler data layer.
//
//  AI(W906-WebBridge) 20260806.
//
//  This is the WB-2 milestone in runnable form: load the machine config, stand
//  up the bridge, serve D:\HT9045\web, and publish real tag values to whatever
//  browser connects.
//
//      wb_serve.exe                 -> http://127.0.0.1:8045/?src=ws
//      wb_serve.exe 9000            -> different port
//      wb_serve.exe --root <dir>    -> serve a different web root
//      wb_serve.exe --seconds 20    -> exit after N seconds (for scripted runs)
//
//  WHAT IT IS NOT
//  Not the product. The handler proper will own this server on its own UI
//  thread once GA-3 lands the god-stack in HT9045.exe (see LoadMachineConfig in
//  database.h). This exe exists so the whole path can be exercised, and looked
//  at, before that.
//
//  SAFETY POSTURE -- inherited, not re-decided
//    * loopback only, and read-only: WebBridgeConfig defaults both that way
//      because this endpoint can eventually command machine motion
//      (web/docs/ARCHITECTURE.md section 6, questions 2 and 3). This file does
//      not widen either.
//    * LoadMachineConfig() SEEDS missing keys, i.e. it WRITES to asGeneralPath.
//      That is the real system\Gerneral.ini. Running this on a machine is
//      therefore the same class of act as starting the handler -- which is the
//      point, but it is stated here rather than discovered. --dry copies the
//      config to a scratch file first and leaves the real one alone.
//
//  WHAT THE BROWSER WILL SHOW
//  Mostly "---". That is correct, not broken: only ~8 of the tags have a source
//  that the port actually loads today. WebBridgeTags.h carries the measured
//  live/dead inventory and the reason for each.
// =============================================================================
#include "WebBridgeTags.h"

#include "WebBridge/WebBridgeServer.h"
#include "WebBridge/TagSnapshot.h"
#include "WebBridge/CommandQueue.h"   // AI(W906-FW-W1) 20260819: cmd channel e2e (--allow-cmd)
#include "WebAuth.h"                  // AI(W906-FW-W2) 20260819: auth.login verification core
#include "cmydef.h"                   // AccessLevel, pwPath (golden globals the auth commands drive)
// AI(W906-FW-W4) 20260819: first real command family -- counter.clear drives
// the translated TfCounterClear::ClearCount core (test_counterclear_core
// covers it) behind golden's own Security_new.def per-item authorization.
#include "forms/fCounterClear.h"      // fCounterClear global + ClearCount (MachineType.h: eClearType)
#include "forms/fMain.h"              // fMain->Clarn_Data (golden spbExeClick bracket)
#include "cAuthority.h"               // GetCountClrAuth(), authCounterClr[]
#include "cMyDB.h"                    // MyDBIProcess (recording sim in this tree)
#include "LastSet.h"                  // LastSet (post-clear observable printed to the serve log)
#include "forms/fBinSel.h"            // AI(W906-FW-BIN1) 20260820: fBinSel->ReadFile (BinSelect loader)
#include "forms/fShowBinSelect.h"     // AI(W906-FW-BIN1) 20260820: fShowBinSelect->ShowBinSel (MyBinSel captions)
#include "cinitial.h"                 // AI(W906-FW-BIN1) 20260820: SetTechDataToProd_Yield
#include "forms/fTemp_Set.h"          // AI(W906-FW-TEMP2) 20260820: fTemp_Set->ReadTempFile (temp.sv/soak/mode loader)
// AI(W906-FW-W5a) 20260819: ShowMyMessage + its forward hook live in
// canary_support.{h,cpp} -- but that header re-defaults RecordProcess/
// MyDBIProcessNew parameters that common.h (already included above) also
// defaults, which is ill-formed in one TU. Local forward-decls instead,
// the same idiom database.cpp already uses for ShowMyMessage; no defaults
// here, every argument is passed explicitly at the call site.
void ShowMyMessage(AnsiString S1, AnsiString S2, AnsiString S3, bool Ok, bool bServoOff);
extern void (*W906_ShowMyMessage_Hook)(const char* S1, const char* S2);
// AI(W906-FW-W5b) 20260819: the ANSWER-carrying dialog (same local-decl idiom).
int  ShowErrorMessage(AnsiString Code, int KCode, int Pos, bool bDuplicateErr, AnsiString errPart);
extern int (*W906_ShowErrorMessage_Hook)(const char* Code, int KCode, int Pos);

#include <vector>

#include "database.h"
#include "common.h"

#include <windows.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>

// AI(W906-FW-W5a) 20260819: ShowMyMessage -> browser info modal. The hook is a
// raw function pointer (canary_support.h keeps zero includes), so the server
// handle rides a file-scope static. Display-only by design: golden's
// ShowMyMessage returns void, nothing flows back (design doc section 4).
static webbridge::WebBridgeServer* g_modalServer = 0;
static void ForwardShowMyMessage(const char* s1, const char* s2)
{
    if (!g_modalServer) return;
    std::string text(s1 ? s1 : "");
    if (s2 && s2[0]) { text += " | "; text += s2; }
    g_modalServer->PostModal("Message", text);
}

// AI(W906-FW-W5b) 20260819: ShowErrorMessage -> browser query, ANSWER flows
// back. Golden blocks its UI thread in a modal loop until the operator picks
// RETRY / SKIP / CLEAN_OUT; the equivalent here is this pump: it blocks the
// tick thread, drains the queue itself, and refuses every command except the
// matching modal.answer with "modal-pending" -- the transport-level rendering
// of VCL modality (everything behind the dialog is inert until it is
// answered). No timeout, faithfully: golden waits forever. Tag publishing
// also freezes while pumping, exactly as golden's blocked UI thread would.
static webbridge::CommandQueue* g_pumpQueue = 0;
static unsigned long long       g_nextQid  = 1;

static int ForwardShowErrorMessage(const char* code, int kcode, int /*pos*/)
{
    if (!g_modalServer || !g_pumpQueue) return 0;   // unattended -> sim answer
    const unsigned long long qid = g_nextQid++;
    char qidStr[24];
    std::snprintf(qidStr, sizeof(qidStr), "%llu", qid);
    g_modalServer->PostQuery(qid, code ? code : "", kcode);
    std::printf("query qid=%s code=%s kcode=%d -- waiting for modal.answer\n",
                qidStr, code ? code : "", kcode);

    std::vector<webbridge::WebCommand> local;
    for (;;) {
        ::Sleep(100);
        local.clear();
        g_pumpQueue->drain(local);
        for (size_t i = 0; i < local.size(); ++i) {
            const webbridge::WebCommand& wc = local[i];
            if (wc.cmd == "modal.answer" && wc.hasTag && wc.tag == qidStr) {
                const std::string ans = (wc.hasValue && wc.value.isString())
                                        ? wc.value.asString() : std::string();
                int k = 0;
                if      (ans == "RETRY")     k = K_RETRY;
                else if (ans == "SKIP")      k = K_SKIP;
                else if (ans == "CLEAN_OUT") k = K_CLEAN_OUT;
                if (k != 0 && (k & kcode) != 0) {
                    g_modalServer->CompleteCommand((unsigned long long)wc.id, true, std::string());
                    std::printf("query qid=%s answered %s (K=%d)\n", qidStr, ans.c_str(), k);
                    return k;
                }
                g_modalServer->CompleteCommand((unsigned long long)wc.id, false,
                                               "not an offered option");
            } else {
                g_modalServer->CompleteCommand((unsigned long long)wc.id, false, "modal-pending");
            }
        }
    }
}

int main(int argc, char** argv)
{
    std::setvbuf(stdout, 0, _IONBF, 0);

    unsigned short port = 8045;
    std::string    root = "D:\\HT9045\\web";
    int            seconds = 0;      // 0 = run until Ctrl-C
    bool           dry = true;
    // AI(W906-FW-W1) 20260819: write-path channel opt-in. Default stays
    // READ-ONLY (the server refuses every "cmd" frame); --allow-cmd attaches
    // a CommandQueue and flips SetReadOnly(false) -- same explicit-flag
    // safety convention as --real. First dispatch table carries only
    // sys.ping (pure echo); see docs/WEBBRIDGE_WRITEPATH_DESIGN.md section 6.
    bool           allowCmd = false;

    //AI(W906-FW1) 20260817: same two safety reversals as wb_publish, paid for
    // the same evening (see tools/wb_publish.cpp): unknown arguments refuse
    // instead of falling through to atoi-as-port, and touching the REAL
    // system\Gerneral.ini needs an explicit --real -- a config load can WRITE
    // the file it reads, and the TIniFile flush destroys comments and layout.
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--root") == 0 && i + 1 < argc) {
            root = argv[++i];
        } else if (std::strcmp(argv[i], "--seconds") == 0 && i + 1 < argc) {
            seconds = std::atoi(argv[++i]);
        } else if (std::strcmp(argv[i], "--dry") == 0) {
            dry = true;
        } else if (std::strcmp(argv[i], "--real") == 0) {
            dry = false;
        } else if (std::strcmp(argv[i], "--port") == 0 && i + 1 < argc) {
            port = static_cast<unsigned short>(std::atoi(argv[++i]));
        } else if (std::strcmp(argv[i], "--allow-cmd") == 0) {
            allowCmd = true;
        } else {
            std::printf("wb_serve: unknown argument '%s'\n"
                        "usage: wb_serve [--port N] [--root DIR] [--seconds N] [--dry] [--real] [--allow-cmd]\n"
                        "  the config load uses a scratch copy by default; --real opts\n"
                        "  into touching the live system\\Gerneral.ini.\n",
                        argv[i]);
            return 2;
        }
    }

    // --- 1. bring the machine data layer up ---------------------------------
    const AnsiString savedGeneralPath = asGeneralPath;
    AnsiString scratch;

    if (dry) {
        char tmp[MAX_PATH];
        ::GetTempPathA(MAX_PATH, tmp);
        scratch = AnsiString(tmp) + "wb_serve_general.ini";
        if (!::CopyFileA(savedGeneralPath.c_str(), scratch.c_str(), FALSE)) {
            std::printf("--dry: cannot copy %s (err %lu)\n",
                        savedGeneralPath.c_str(), (unsigned long)::GetLastError());
            return 2;
        }
        asGeneralPath = scratch;
        std::printf("--dry: using a scratch copy, real config untouched\n");
    }

    std::printf("loading machine config from %s ...\n", asGeneralPath.c_str());
    if (!LoadMachineConfig()) {
        std::printf("LoadMachineConfig FAILED -- cannot open the ini\n");
        return 1;
    }

    // AI(W906-FW1e) 20260820: golden's startup mirror (main.cpp:9440
    // cbSetupFileName->Text=GetLastOpenFN()) -- the CURRENT recipe name, read
    // once from setup.inf (LastDataPath, read-only). Done here, BEFORE the
    // ShowMyMessage hook is installed, so a missing setup.inf logs once to
    // stdout instead of broadcasting a modal every tick. "Fail Open" on
    // failure is golden's own combo text, published as-is.
    fMain->cbSetupFileName->Text = GetLastOpenFN();
    std::printf("recipe.current = %s\n", fMain->cbSetupFileName->Text.c_str());

    // AI(W906-FW-BIN1) 20260820: the bin.* display chain
    // (docs/RECON_binstar_datasource.md). BinSelect's loader is translated and
    // faithful, but it reads recipe Binasgn*.Data through CheckAndReadIniData,
    // whose missing-key seeding WRITES the file it reads (and ReadFile itself
    // carries one WriteIniData) -- against DataPath, a THIRD hardcoded shared
    // production path family --dry did not yet cover. Same protection pattern
    // as asGeneralPath, extended: copy the recipe folder to scratch and point
    // DataPath there for the whole run. --real keeps golden's true paths, the
    // same explicit-opt-in contract as the Gerneral.ini handling above.
    const AnsiString savedDataPath = DataPath;
    AnsiString recipeScratchRoot;
    bool binSelLoaded = false;
    bool tempLoaded = false;   // AI(W906-FW-TEMP2) 20260820: see the temp.* block below
    {
        const AnsiString recipe = fMain->cbSetupFileName->Text;
        bool pathReady = (recipe.Length() > 0 && recipe != "Fail Open");
        if (pathReady && dry) {
            char tmp[MAX_PATH];
            ::GetTempPathA(MAX_PATH, tmp);
            recipeScratchRoot = AnsiString(tmp) + "wb_serve_recipe\\";
            const AnsiString src = savedDataPath + recipe;
            const AnsiString dst = recipeScratchRoot + recipe;
            ::CreateDirectoryA(recipeScratchRoot.c_str(), 0);
            ::CreateDirectoryA(dst.c_str(), 0);
            WIN32_FIND_DATAA fd;
            HANDLE h = ::FindFirstFileA((src + "\\*").c_str(), &fd);
            int copied = 0;
            if (h != INVALID_HANDLE_VALUE) {
                do {
                    if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
                    if (::CopyFileA((src + "\\" + fd.cFileName).c_str(),
                                    (dst + "\\" + fd.cFileName).c_str(), FALSE)) ++copied;
                } while (::FindNextFileA(h, &fd));
                ::FindClose(h);
            }
            if (copied > 0) {
                DataPath = recipeScratchRoot;
                std::printf("--dry: recipe folder scratch-copied (%d files), DataPath redirected\n", copied);
            } else {
                pathReady = false;
                std::printf("--dry: recipe folder empty/missing (%s) -- bin.* stays null\n", src.c_str());
            }
        }
        if (pathReady) {
            // golden boot shape main.cpp:1014: fBinSel->ReadFile(false,false,"")
            fBinSel->ReadFile(false, false, AnsiString(""));
            // battery member called directly, NOT the SetTechDataToProd
            // orchestrator: the orchestrator is InitialOK-gated AND shadowed
            // by ckernel_shims' no-op #define; _Yield itself is neither
            // (recon section 4, both verified there).
            SetTechDataToProd_Yield();
            fShowBinSelect->ShowBinSel();
            binSelLoaded = true;
            std::printf("bin.* chain loaded: BinSelect -> iT6CatData -> MyBinSel captions\n");

            // AI(W906-FW-TEMP2) 20260820: temp.sv/soak/mode, SAME redirected-
            // DataPath window as bin.* directly above -- main-loop ruling
            // supersedes FW-TEMP1's "no wire" verdict (WebBridgeTags.h's
            // AI(W906-FW-TEMP1) block, part (a), now corrected there too):
            // FW-BIN1's scratch-copy above copies the WHOLE recipe folder,
            // and Temperature.Data lives in that SAME folder, so
            // ReadTempFile's MyForceDirectories()/CheckAndReadIniData()/
            // WriteIniData() calls all land in scratch here -- the identical
            // protection bin.* already relies on, not a new exemption.
            // golden boot shape: main.cpp:8868 (TfMain::DoReadLastData)
            // `fTemp_Set->ReadTempFile(true);` -- called TWICE there (the
            // second time is ordering-dependent on fSetup->ReadFile(), which
            // this tool never brings up), so ONE call is the faithful subset;
            // FormShow's own `ReadTempFile(true);` (uTemp_Set.cpp:872) is the
            // same single-call shape.
            //
            // Init() NOT called -- audited, not assumed (20260820): Init()
            // (uTemp_Set.cpp:281-597) does zero file/hardware I/O and zero
            // fMain/other-form dereferences (grepped its whole body for
            // ReadIniData/WriteIniData/CheckAndReadIniData/MyForceDirectories/
            // FileExists/CopyFile/DeleteFile/"fMain->"/"fLotInfo->"/
            // "ATC_InterfaceForm->": 0 hits), so it WOULD be safe to call --
            // but ReadTempFile does not need it: grepping ReadTempFile's own
            // span (uTemp_Set.cpp:2172-3354) for every global Init() populates
            // (myTempPal[]/listNormal/listArm1/listArm2/ATCOffsetEdit[]/
            // ATCPackageOffsetEdit[]/ATCPackageTempEdit[]/ATC_FFCOffset*Edit
            // [][]/ATC_FFCPointUse[][]/ZoneTempUse[]/ZoneTempSetting[]/
            // MultiSensorOffsetUse[]/ATC_MultiSensorOffsetEdit[]) is 0 hits.
            // Every WIDGET ReadTempFile does touch (rgTemperatureMode,
            // edChillerTemp, cbbATC_RecipeFile, rgIndexHeatMode) is NSDMI'd in
            // forms/fTemp_Set.h (`= new T...()` at the member declaration), so
            // each is already a live object the moment `new TfTemp_Set()`
            // below runs -- Init() is not in the dependency chain for these 3
            // tags. Calling it anyway would be pure unused surface, the
            // opposite of the minimal-footprint bin.* precedent set
            // (SetTechDataToProd_Yield() called directly, not the
            // InitialOK-gated SetTechDataToProd orchestrator).
            //
            // fDynamicTemp stays NULL (never `new`'d) -- ReadTempFile's own
            // `if(fDynamicTemp!=NULL)` guard (uTemp_Set.cpp:2266) skips that
            // block entirely, golden-faithful.
            //
            // THIRD write mechanism found, beyond MyForceDirectories/
            // CheckAndReadIniData (both already on record): a bare
            // `WriteIniData(szDir,"ATC","Chiller Temp",...)` at
            // uTemp_Set.cpp:2795, gated behind
            // `ATC_SYSTEM>eATC30 && ATC_SYSTEM==eNewATCSystem` (:2733/:2746).
            // Whether it fires depends on this box's ATC_SYSTEM config; either
            // way it lands in the SAME redirected scratch folder, so it does
            // not change the go/no-go call -- recorded here for completeness.
            //
            // Liveness: golden's OWN "file missing" signal, iSendChangeTempError
            // (uTemp_Set.cpp:2200, `iSendChangeTempError=1; return;` on the
            // unconditional FileExists() check that gates the WHOLE function
            // before bUpdateAll is even tested) -- reset it first so a stale
            // value from an earlier call cannot be mistaken for this one.
            if (fTemp_Set == NULL) fTemp_Set = new TfTemp_Set();   // ctor is fields-only, uTemp_Set.cpp:272-274
            //AI(W906-FW-TEMP2) 20260820: Init() IS required before ReadTempFile
            // after all -- the wave's audit grepped ReadTempFile's own span only,
            // but ReadTempFile chains into DoIniDataToForm (uTemp_Set.cpp:3355),
            // which reads myTempPal[] (20 sites) and every other Init()-populated
            // array. Measured: gdb bt on the e2e SEGV lands in DoIniDataToForm;
            // Init() itself is pure in-memory widget wiring (the audit's own
            // 0-hit grep for file/hw/global-form calls stands).
            fTemp_Set->Init();
            iSendChangeTempError = 0;
            fTemp_Set->ReadTempFile(true);
            tempLoaded = (iSendChangeTempError != 1);
            std::printf(tempLoaded
                ? "temp.* chain loaded: ReadTempFile -> fWorkTemperBase/fSoakTime/iMachineTempMode\n"
                : "temp.* chain: Temperature.Data missing in the recipe folder -- stays null\n");
        }
    }
    ht9045::SetWebBinSelLoaded(binSelLoaded);
    ht9045::SetWebTempLoaded(tempLoaded);   // AI(W906-FW-TEMP2) 20260820

    webbridge::TagSnapshot snap;
    const std::size_t staged = ht9045::PublishHandlerTags(snap);
    const ht9045::TagCoverage cov = ht9045::HandlerTagCoverage();
    std::printf("published %u tags, %u of %u carry a loaded value\n",
                (unsigned)staged, (unsigned)cov.live, (unsigned)cov.total);

    // --- 2. stand the bridge up ---------------------------------------------
    webbridge::WebBridgeConfig cfg;
    cfg.port         = port;
    cfg.documentRoot = root;
    // bindAddress and readOnly keep their safe defaults on purpose.

    webbridge::WebBridgeServer server(cfg);
    server.SetSnapshot(&snap);

    // AI(W906-FW-W1) 20260819: the command channel. Queue is attached
    // unconditionally (harmless while read-only); the read-only gate is what
    // --allow-cmd actually opens. Dispatch happens on THIS thread's tick
    // below -- the single-process stand-in for "the UI thread drains on its
    // existing timer tick" (design doc section 1).
    webbridge::CommandQueue cmdQueue;
    server.SetCommandQueue(&cmdQueue);
    server.SetReadOnly(!allowCmd);

    std::string err;
    if (!server.Start(&err)) {
        std::printf("server failed to start: %s\n", err.c_str());
        return 1;
    }

    // AI(W906-FW-W5a) 20260819: from here on, every ShowMyMessage anywhere in
    // the linked machine code also reaches the browser as an info modal.
    g_modalServer = &server;
    W906_ShowMyMessage_Hook = &ForwardShowMyMessage;
    // AI(W906-FW-W5b) 20260819: and every ShowErrorMessage becomes a browser
    // query whose K answer flows back (pump above).
    g_pumpQueue = &cmdQueue;
    W906_ShowErrorMessage_Hook = &ForwardShowErrorMessage;

    std::printf("\n  http://127.0.0.1:%u/?src=ws     (live handler data)\n",
                (unsigned)server.BoundPort());
    std::printf("  serving %s\n", root.c_str());
    std::printf(allowCmd ? "  COMMANDS ENABLED (--allow-cmd; dispatch: sys.ping), loopback only\n"
                         : "  read-only, loopback only\n");
    std::printf("  most values will read \"---\": that is the truth, see WebBridgeTags.h\n");
    std::printf(seconds > 0 ? "  exiting after %d s\n\n" : "  Ctrl-C to stop\n\n", seconds);

    // --- 3. republish on a slow tick -----------------------------------------
    // The real handler will do this from its existing UI timer. Nothing here
    // reads machine state off the socket thread; the snapshot is the only seam.
    const DWORD started = ::GetTickCount();
    std::vector<webbridge::WebCommand> drained;
    for (;;) {
        ::Sleep(500);

        // AI(W906-FW-W1) 20260819: drain + dispatch on the tick, ack via
        // CompleteCommand (ticket == WebCommand.id, see QueuePush). FW-W1's
        // dispatch table is deliberately just sys.ping -- proving the
        // browser->ws->queue->tick->ack round trip end to end; real commands
        // land per design doc section 6 (FW-W2+).
        drained.clear();
        cmdQueue.drain(drained);
        for (size_t i = 0; i < drained.size(); ++i) {
            const webbridge::WebCommand& wc = drained[i];
            if (wc.cmd == "sys.ping") {
                server.CompleteCommand((unsigned long long)wc.id, true, std::string());
            } else if (wc.cmd == "sys.echoModal") {
                // AI(W906-FW-W5a) 20260819: probe surface for the modal path,
                // sys.ping's sibling -- drives the REAL ShowMyMessage (capture
                // seam included), whose hook then broadcasts the modal frame.
                // Text rides `value` (free-form JSON string), not `tag` (the
                // server's tag-name charset rejects spaces).
                AnsiString mtext = (wc.hasValue && wc.value.isString())
                                   ? AnsiString(wc.value.asString().c_str()) : AnsiString("");
                ShowMyMessage(mtext, AnsiString(""), AnsiString(""), false, false);
                server.CompleteCommand((unsigned long long)wc.id, true, std::string());
            } else if (wc.cmd == "sys.echoErrorModal") {
                // AI(W906-FW-W5b) 20260819: probe surface for the ANSWER path.
                // tag = alarm code (sane charset), value = golden K button
                // mask (default RETRY|SKIP). This call BLOCKS in the pump
                // until a browser answers -- that is the point.
                AnsiString qcode = wc.hasTag ? AnsiString(wc.tag.c_str()) : AnsiString("WAR0000");
                int qmask = (wc.hasValue && wc.value.isNumber())
                            ? (int)wc.value.asInt(K_RETRY | K_SKIP) : (K_RETRY | K_SKIP);
                const int k = ShowErrorMessage(qcode, qmask, 0, false, AnsiString(""));
                std::printf("sys.echoErrorModal: ShowErrorMessage returned K=%d\n", k);
                server.CompleteCommand((unsigned long long)wc.id, true, std::string());
            } else if (wc.cmd == "modal.answer") {
                // AI(W906-FW-W5b) 20260819: an answer with no query pending --
                // the pump consumes matching answers itself, so reaching the
                // normal dispatch means nobody is asking.
                server.CompleteCommand((unsigned long long)wc.id, false, "no query pending");
            } else if (wc.cmd == "auth.login") {
                // AI(W906-FW-W2) 20260819: tag = user name, value = password
                // (both strings). Verification = golden's password-book arm
                // (WebAuth.cpp); success drives the SAME global golden's
                // btLogin drives: AccessLevel. Book path: golden's pwPath
                // global, test-overridable via W906_PWBOOK_PATH (call-time
                // getenv, the tree's established env-seam shape).
                AnsiString book = getenv("W906_PWBOOK_PATH")
                                  ? AnsiString(getenv("W906_PWBOOK_PATH")) : pwPath;
                AnsiString u = wc.hasTag   ? AnsiString(wc.tag.c_str())   : AnsiString("");
                AnsiString p = (wc.hasValue && wc.value.isString())
                                  ? AnsiString(wc.value.asString().c_str()) : AnsiString("");
                const int level = WebAuthVerify(book, u, p);
                if (level >= 0) {
                    AccessLevel = level;                 // golden: AccessLevel=l;
                    // AI(W906-FW1d) 20260820: golden's login path reaches
                    // ChangeLevelAttr whose FIRST act is DoChangeLevel
                    // (main.cpp:12410) -- mirror the level name the same way.
                    fMain->DoChangeLevel();
                    server.CompleteCommand((unsigned long long)wc.id, true, std::string());
                } else {
                    server.CompleteCommand((unsigned long long)wc.id, false, "bad credentials");
                }
            } else if (wc.cmd == "auth.logout") {
                AccessLevel = 0;                          // golden: back to Operator
                fMain->DoChangeLevel();                   // AI(W906-FW1d) 20260820: same mirror on logout
                server.CompleteCommand((unsigned long long)wc.id, true, std::string());
            } else if (wc.cmd == "counter.clear") {
                // AI(W906-FW-W4) 20260819: tag = counter family, mirroring one
                // checkbox of golden spbExeClick (cCounterClear.cpp golden
                // :394-450). Authorization is golden's OWN gate replayed: the
                // FormShow rule "an unauthorized checkbox can never be checked"
                // becomes a per-family refusal here, read from the same
                // Security_new.def via GetCountClrAuth() (cAuthority.cpp
                // golden :379-384). WRITE CAVEAT, stated not discovered:
                // golden's own readers seed defaults back into that file --
                // CheckFile when the FILE is missing, and CheckAndReadIniData
                // (common.cpp:409) when a KEY is missing. Faithful, but it
                // means this dispatch can write config\Security_new.def on a
                // machine with a stale def; every e2e run MD5-checks the file
                // stayed untouched.
                // Deliberately NOT called (documented deviations):
                //   * MyDBIProductionData -- golden runs it with the sqlite
                //     production DB open; wb_serve never brings the DB layer
                //     up, so its precondition does not exist here (and
                //     MyDBExecSQL on a null dbReadWrite is a crash, not a row).
                //   * the CC_KYEC_LEE re-auth branch -- already GATE CC3
                //     (#if 0) in the translated core itself.
                struct FamilyRow { const char* key; int authIdx; int ct1; int ct2; };
                static const FamilyRow kFamilies[] = {
                    // authIdx = funCounterClr[] order (cAuthority.cpp:203);
                    // ct pairing = golden spbExeClick, incl. loadingCount's
                    // ctIndexCount rider (golden :433 kevin 20130125).
                    { "alarmData",      0, ctAlarmData,       -1           },
                    { "testerCategory", 1, ctTesterCategory,  -1           },
                    { "loadingCount",   3, ctLoadingCounts,   ctIndexCount },
                    { "contactCurr",    4, ctContactCounts,   -1           },
                    { "contactHis",     5, ctContactCountsHis,-1           },
                    { "sortingCount",   6, ctTraySortCount,   -1           },
                    { "timeData",       7, ctTimeData,        -1           },
                };
                const FamilyRow* row = 0;
                for (size_t f = 0; f < sizeof(kFamilies)/sizeof(kFamilies[0]); ++f)
                    if (wc.hasTag && wc.tag == kFamilies[f].key) { row = &kFamilies[f]; break; }
                if (!row) {
                    server.CompleteCommand((unsigned long long)wc.id, false,
                                           "unknown counter family");
                } else {
                    GetCountClrAuth();                       // golden FormShow gate
                    if (!authCounterClr[row->authIdx]) {
                        server.CompleteCommand((unsigned long long)wc.id, false,
                                               "not-authorized");
                    } else {
                        fMain->Clarn_Data(10, "Manual clear count");      // golden :421
                        fCounterClear->ClearCount(row->ct1);
                        if (row->ct2 >= 0) fCounterClear->ClearCount(row->ct2);
                        fMain->Clarn_Data(10, "Manual clear count done"); // golden :448
                        MyDBIProcess("Process", "Counter Clear has been executed!!"); // golden :449
                        std::printf("counter.clear[%s] done (SendCT[0]=%d iIndexCount=%d)\n",
                                    row->key, LastSet.SendCT[0], LastSet.iIndexCount);
                        server.CompleteCommand((unsigned long long)wc.id, true, std::string());
                    }
                }
            } else {
                server.CompleteCommand((unsigned long long)wc.id, false,
                                       "unknown cmd (dispatch: sys.ping, sys.echoModal, sys.echoErrorModal, "
                                       "auth.login, auth.logout, counter.clear, modal.answer)");
            }
        }

        ht9045::SetWebControlOwner(server.ControlOwner());   // AI(W906-FW-W3) 20260819: control.owner tag feed
        ht9045::PublishHandlerTags(snap);
        server.Wake();

        if (seconds > 0 &&
            (::GetTickCount() - started) >= static_cast<DWORD>(seconds) * 1000u) {
            break;
        }
    }

    // AI(W906-FW-W5a) 20260819: unhook before the server object dies.
    W906_ShowMyMessage_Hook = 0;
    W906_ShowErrorMessage_Hook = 0;   // AI(W906-FW-W5b) 20260819
    g_pumpQueue = 0;
    g_modalServer = 0;

    server.Stop();

    if (dry) {
        CloseGeneralIniFile();
        asGeneralPath = savedGeneralPath;
        ::DeleteFileA(scratch.c_str());
        // AI(W906-FW-BIN1) 20260820: restore the recipe-path redirect too.
        // The scratch folder is left for the OS temp cleaner (it may hold
        // seeded keys useful for post-mortem diffing against the real one).
        DataPath = savedDataPath;
    }

    std::printf("stopped\n");
    return 0;
}
