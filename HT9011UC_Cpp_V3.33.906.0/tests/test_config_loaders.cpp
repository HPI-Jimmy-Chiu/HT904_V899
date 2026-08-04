// ===========================================================================
//  tests/test_config_loaders.cpp
//
//  W3-cont2 VERIFY: drive the TRANSLATED database.cpp config-table loaders over
//  the REAL machine config files and lock the ground-truth oracle.
//
//  Unlike test_config_db.cpp (which re-implemented the name-based column scan in
//  the test itself to lock the oracle BEFORE the readers were translated), this
//  test exercises the ACTUAL translated production code:
//
//      HSys.LoadMotData()  -> TMOTNO::SetMOTTableNo (name-based col resolve)
//                             -> TMOTDATA(row) ctor  -> HSys.MotTable / mapMotTable
//      HSys.LoadIoData()   -> TIOTABLENO::SetIOTableNo (name-based col resolve)
//                             -> TIODATA(row) ctor   -> HSys.IOTable / mapIOTable
//
//  Both loaders hardcode the real path (database.cpp:1552 / 1620):
//      IoTablePath  = "D:\\HT9045\\System\\IO_Table.csv"
//      MotTablePath = "D:\\HT9045\\System\\Mot_Table.csv"
//  so there is nothing to point them at -- we just guard FileExists and SKIP
//  (exit 0) if the real config is absent (CI box without the machine files).
//
//  GROUND TRUTH (hand-verified against D:\HT9045\system, see report):
//      Mot_Table.csv : 45 data rows; CardModel resolves to "SMC" for ALL 45.
//                      Physical header order != enum order
//                      (CardModel is physical col 22, enum fallback 9) --
//                      proves resolution is NAME-based, not positional.
//                      M00/MInArmX : BoardID==0, Port==0, CardModel=="SMC".
//      IO_Table.csv  : 15 cols; ISABase resolves BY NAME to col 8;
//                      668 data rows; ISABase distribution 644 zero : 24 non-zero.
//
//  NO-BORLAND-BINARY LIMITATION: there is no BCB6 toolchain here, so we cannot
//  diff the original .exe's in-memory parse.  The oracle is (real file content)
//  x (hand-verified truth values) x (faithful translation of the golden logic).
//
//  LINK NOTES: database.cpp references two externals that belong to gated waves
//  (cMyDB / mymessbox).  We provide local stub definitions here so the loader
//  TU links; with the real (fully-populated) config neither is invoked on the
//  happy path, but the symbols must resolve.
// ===========================================================================
#include "vclcompat/vcl_compat.h"
#include "database.h"
#include "cpublic.h"   // TMyQueue10 / TMyStrQueue100 / TMyTimerQueue100 decls (for stub ctors)

#include <cstdio>

// ---------------------------------------------------------------------------
//  ShowMyMessage stub -- database.cpp forward-declares this external
//  (mymessbox.h:58) for its LoadIoData/LoadMotData NULL-cell/error paths.
//  Signature MUST match database.cpp's forward declaration exactly.
//
//  AI(W906-FastcallFix) 20260720: the sibling MyDBIProcess (3-arg,
//  __fastcall) stub that used to live here (matching database.cpp's OTHER
//  forward declaration, cMyDB.h:20) is REMOVED -- as of this wave,
//  SECSGEM/uHGemEquipment.cpp supplies the real, externally-linkable,
//  __fastcall-decorated definition of that overload (moved out of an
//  anonymous namespace and given __fastcall this same wave; see that file's
//  own note), and this target already links ht9045_secsgem (the library
//  that carries it -- see this target's RESCAN link group in
//  tests/CMakeLists.txt) to satisfy database.cpp:75/uHGemClass.cpp:310's own
//  extern+__fastcall declarations of it. A local stub here would now be a
//  duplicate-definition link error, not a missing-symbol fix. (2026-07-20
//  audit, AUDIT_fastcall_tree.md finding 1, confirmed no other production
//  definition of this fastcall-decorated symbol previously existed anywhere
//  in the tree -- this test's own local stub was one of only three places
//  providing it, all now redundant and removed the same way.)
// ---------------------------------------------------------------------------
static int g_msgCalls = 0;   // counts ShowMyMessage hits (NULL-cell / error paths)

// AI(W906-GA1-B2-integrate) 20260804: local ShowMyMessage stub RETIRED -- the full
// RESCAN group now supplies the real definition (was a duplicate).

// AI(W906-SysModWire) 20260720: a THIRD gated external, needed as of this
// wave -- database.cpp's SystemModularInitial is now real (`new
// HT9045Gem(...)`), which pulls uHGemHT9045_Shim.cpp.o -> uHGemClass.cpp.o
// (HTGem base ctor/dtor) -> uHGemEquipment.cpp.o (HTGem's 8 newly un-gated
// methods call real out-of-line THGem methods) into this binary's link for
// the first time. uHGemEquipment.cpp.o's own 2-arg MyDBIProcess extern
// (declared uHGemEquipment.cpp:80, NO __fastcall -- a DIFFERENT overload,
// by arity, from the 3-arg __fastcall one uHGemEquipment.cpp itself now
// defines at file scope, see AI(W906-FastcallFix) 20260720 note above) needs
// a definition too. Same no-op shape as tests/test_uHGemEquipment.cpp's own
// (:173).
// AI(W906-GA1-B2-integrate) 20260804: local MyDBIProcess stub RETIRED -- the full
// RESCAN group now supplies the real definition (was a duplicate).

// ---------------------------------------------------------------------------
//  AI(W906-uHGemClass-Unlock3) 20260723: HasICUnderMachine / HasAnyICInMachine
//  stubs -- this wave un-gated SECSGEM/uHGemClass.cpp's S2F16, which calls
//  these two real csystem.h predicates (implemented in csystem_predicates.cpp,
//  part of ht9045_sm). uHGemClass.cpp.o is unconditionally part of
//  ht9045_secsgem (this test already links it -- see the RESCAN group above,
//  needed for HT9045Gem/HGem), so its now-undefined refs to these two
//  predicates must resolve at THIS test's link too, even though this test
//  never calls S2F16/LoadMotData/LoadIoData through a path that reaches
//  them. Same "avoid the god-stack ht9045_sm for a focused unit test"
//  posture already established by this file's own MyDBIProcess/ShowMyMessage
//  stubs above (rather than adding ht9045_sm to the RESCAN group, which would
//  reintroduce the exact MyDBIProcess/ShowMyMessage duplicate-definition
//  collision those stubs were written to avoid). Conservative `false` here is
//  NOT because the real bodies still have a gated/TODO false-by-default path
//  -- csystem_predicates.cpp:56 has HT9045_KITSUCK_GRID_AVAILABLE=1 (flipped
//  on by the W7 substrate wave, 20260629), so the real HasICUnderMachine()
//  actively evaluates ShuttleHasIC()||IndexHasIC()||HasICUnderHotPlate() on
//  the real (wired) TMyKitSuck grid objects. `false` is simply the correct
//  answer for THIS unlinked stub, which has no grid state to query at all --
//  not a behavioral fork from the real body, just a link-time stand-in for a
//  value this test would get anyway (no IC seeded => false either way).
// AI(W906-GA1-B2-integrate) 20260804: HasICUnderMachine/HasAnyICInMachine local stubs RETIRED (real csystem_predicates bodies now in group)

// ---------------------------------------------------------------------------
//  Link-satisfying stub ctors for the W6/W7-DEFERRED queue classes.
//
//  WHY THESE ARE NEEDED -- not a translation gap in the loaders:
//    cmydef.cpp keeps its ACTIVE global queue objects (QueueTaskList[],
//    QueueGalilCmd, QueueTestTime, ...) whose construction needs these ctors.
//    The ctor *bodies* live in cpublic.cpp, whose ENTIRE body is gated
//    `#if 0 // TODO(W3/W5/W6/W7)` (its bodies reach untranslated cmydef
//    globals).  database.cpp references only the scalar INDEX_MOTION_CARD from
//    cmydef.cpp, but pulling that one symbol drags the whole cmydef.cpp.obj
//    (static-init of the queue globals) in.  ht9045_globals was, by design,
//    only required to COMPILE standalone -- not to be link-complete -- so these
//    three ctors are the deferred tail.  The loaders never touch the queue
//    objects, so trivial (no-op) construction is sufficient and correct for
//    this test.  Replace with the real cpublic.cpp ctors when W6/W7 lands.
// ---------------------------------------------------------------------------
// AI(W906-GA1-B3-integrate) 20260804: the three no-op stub ctors that lived
// here are RETIRED -- cpublic.cpp's real ctors have been active since
// 2026-06-26 and this target links ht9045_globals, so the stubs were a
// latent duplicate-definition trap (found by the GA-1-B3 sweep).

// ---------------------------------------------------------------------------
//  Tiny check harness (same style as the sibling tests).
// ---------------------------------------------------------------------------
static int g_fail = 0;
static int g_total = 0;
static void check(bool cond, const char* expr, const char* file, int line) {
    ++g_total;
    if (!cond) {
        ++g_fail;
        std::printf("FAIL [%s:%d]  %s\n", file, line, expr);
    }
}
#define CHECK(cond) check((cond), #cond, __FILE__, __LINE__)

int main()
{
    // The loaders hardcode these absolute paths; mirror them for the SKIP guard.
    const AnsiString motPath = AnsiString("D:\\HT9045\\System\\Mot_Table.csv");
    const AnsiString ioPath  = AnsiString("D:\\HT9045\\System\\IO_Table.csv");

    if (!FileExists(motPath) || !FileExists(ioPath)) {
        std::printf("SKIP test_config_loaders: real config not found "
                    "(\"%s\" / \"%s\"). Exiting 0.\n",
                    motPath.c_str(), ioPath.c_str());
        return 0;
    }

    // =====================================================================
    //  Mot_Table.csv  -- drive the TRANSLATED LoadMotData()
    // =====================================================================
    {
        HSys.LoadMotData();

        // Name-based column resolution must have run (SetMOTTableNo).  In the
        // real file the physical order differs from the enum fallback order;
        // CardModel sits at physical col 22 (fallback would have been 9).
        std::printf("Mot_Table: SetMOTTableNo resolved CardModel@%d  No@%d  "
                    "BoardID@%d  Port@%d  (enum fallbacks 9/0/3/4)\n",
                    HSys.MotNo.emotCardModel, HSys.MotNo.emotNo,
                    HSys.MotNo.emotBoardID, HSys.MotNo.emotPort);
        CHECK(HSys.MotNo.emotCardModel == 22);   // NAME-based, not positional(9)
        CHECK(HSys.MotNo.emotNo        == 0);
        CHECK(HSys.MotNo.emotBoardID   == 2);    // physical (fallback would be 3)
        CHECK(HSys.MotNo.emotPort      == 3);    // physical (fallback would be 4)
        CHECK(HSys.MotNo.emotSimulateSpeed == 21);

        const int nMot = (int)HSys.MotTable.size();
        std::printf("Mot_Table: LoadMotData parsed %d rows\n", nMot);
        CHECK(nMot == 45);                       // 45 axes

        // Every row's CardModel must resolve to SMC.  For the 4 index motors
        // (MTestY1/Z1/Z2/Y2) the ctor FORCES "SMC" when INDEX_MOTION_CARD==0;
        // for the other 41 it reads the (SMC) cell.  Either way -> SMC.
        int smc = 0;
        for (int i = 0; i < nMot; ++i) {
            if (HSys.MotTable[i]->CardModel == AnsiString("SMC")) ++smc;
        }
        std::printf("Mot_Table: %d / %d rows CardModel==SMC\n", smc, nMot);
        CHECK(smc == 45);

        // Spot-check M00 / MInArmX maps correctly (BoardID 0, Port 0, SMC).
        TMOTDATA* m00 = NULL;
        HSys.mapMotTableIter = HSys.mapMotTable.find(AnsiString("M00"));
        if (HSys.mapMotTableIter != HSys.mapMotTable.end()) {
            int idx = HSys.mapMotTableIter->second.ToIntDef(-1);
            if (idx >= 0 && idx < nMot) m00 = HSys.MotTable[idx];
        }
        CHECK(m00 != NULL);                      // map keyed by Motorname ("M00")
        if (m00) {
            std::printf("Mot_Table: M00 -> Alias=\"%s\" BoardID=%d Port=%d "
                        "CardModel=\"%s\" Enable=%d\n",
                        m00->Alias.c_str(), m00->iBoardID, m00->iPort,
                        m00->CardModel.c_str(), m00->iEnable);
            CHECK(m00->Alias     == AnsiString("MInArmX"));
            CHECK(m00->No        == AnsiString("M00"));
            CHECK(m00->iBoardID  == 0);
            CHECK(m00->iPort     == 0);
            CHECK(m00->CardModel == AnsiString("SMC"));
            CHECK(m00->iEnable   == 1);          // row is enabled, no null cells
        }
    }

    // =====================================================================
    //  IO_Table.csv  -- drive the TRANSLATED LoadIoData()
    // =====================================================================
    {
        HSys.LoadIoData();

        // ISABase resolved BY NAME -> col 8 (matches enum fallback here, but the
        // resolution path is the name-scan SetIOTableNo, verified by the count).
        std::printf("IO_Table: SetIOTableNo resolved ISABase@%d  IOType@%d  "
                    "Alias@%d  (eioTotal=%d)\n",
                    HSys.IoNo.eioISABase, HSys.IoNo.eioType,
                    HSys.IoNo.eioAlias, HSys.IoNo.eioTotal);
        CHECK(HSys.IoNo.eioTotal   == 15);
        CHECK(HSys.IoNo.eioISABase == 8);        // NAME-resolved
        CHECK(HSys.IoNo.eioType    == 0);
        CHECK(HSys.IoNo.eioAlias   == 1);

        const int nIo = (int)HSys.IOTable.size();
        std::printf("IO_Table: LoadIoData parsed %d rows\n", nIo);
        CHECK(nIo == 668);                       // 668 data rows

        // ISABase distribution over the parsed TIODATA rows.  The ctor stores
        // iISABase = eMotionNet(0) when the cell is empty, else atoi(cell);
        // so "empty or 0" both land on iISABase==0 here -> 644, the rest 24.
        int isaZero = 0, isaNonZero = 0;
        for (int i = 0; i < nIo; ++i) {
            if (HSys.IOTable[i]->iISABase == 0) ++isaZero;
            else                                ++isaNonZero;
        }
        std::printf("IO_Table: iISABase==0: %d  !=0: %d\n", isaZero, isaNonZero);
        CHECK(isaZero    == 644);
        CHECK(isaNonZero == 24);
        CHECK(isaZero > isaNonZero);             // 0 dominant
    }

    std::printf("test_config_loaders: %d/%d checks passed "
                "(ShowMyMessage stub hit %d times)\n",
                g_total - g_fail, g_total, g_msgCalls);
    if (g_fail) { std::printf("FAILED: %d checks\n", g_fail); return 1; }
    std::printf("PASS\n");
    return 0;
}
