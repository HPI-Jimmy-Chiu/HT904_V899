// ===========================================================================
//  tests/test_config_db.cpp
//
//  W3 real-config ORACLE test.
//
//  SCOPE / HONEST CAVEAT
//  ---------------------
//  The production config-table READERS (cinitial.cpp / database.cpp
//  TIOTABLENO::SetIOTableNo / TMOTNO::SetMOTTableNo) are NOT translated yet
//  (scheduled later in W3).  This test therefore does NOT exercise those
//  classes.  What it DOES do, additively, is:
//
//    * Prove the vclcompat layer that W3 will build on (TStringList::CommaText
//      CSV split, AnsiString::AnsiPos name matching, LoadFromFile, file APIs)
//      is fit for purpose by parsing the REAL machine config files with it.
//    * Lock in the ground-truth oracle values the future readers must
//      reproduce, reading the SAME real files the machine uses:
//          D:\HT9045\system\Gerneral.ini   MOTION_CARD_TYPE=1 / IO_CARD_TYPE=2
//          D:\HT9045\system\Mot_Table.csv  45 data rows, all CardModel==SMC
//          D:\HT9045\system\IO_Table.csv   668 data rows, 15 cols, ISABase=0 dominant
//
//  The column resolution here MIRRORS the golden ref's NAME-BASED rule
//  (database.cpp:1993-1996):
//      eioISABase=-1;
//      for(i..) if(SL->Strings[i].AnsiPos("ISABase")!=0) eioISABase=i;
//  i.e. scan the header row, match by column NAME (AnsiPos!=0 == substring
//  found, 1-based), NEVER positional.  Mot_Table physical order != enum order
//  in general, so name-based resolution is mandatory.
//
//  RUNTIME-SKIP: if D:\HT9045\system is absent (e.g. CI without the machine
//  files), the test prints SKIP and exits 0 -- it must not fail the suite on a
//  box that lacks the real config.
//
//  No-Borland-binary limitation: we cannot diff the original binary's parse;
//  the oracle is the real file content + the hand-verified truth values.
// ===========================================================================
#include "vclcompat/vcl_compat.h"

#include <cstdio>
#include <string>

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

// The real machine config directory.  Override via env HT9045_SYSTEM_DIR.
static AnsiString systemDir() {
    const char* env = std::getenv("HT9045_SYSTEM_DIR");
    if (env && *env) return AnsiString(env);
    return AnsiString("D:\\HT9045\\system");
}

// ---------------------------------------------------------------------------
//  Gerneral.ini : find "KEY=VALUE" (Big5 file, but the keys/values we read are
//  pure ASCII).  Returns the value or "" if not found.  Mirrors a simple INI
//  key scan (cinitial.cpp reads these via ReadGeneralIni; we do a line scan).
// ---------------------------------------------------------------------------
static AnsiString iniValue(TStringList* lines, const AnsiString& key) {
    for (int i = 0; i < lines->Count; ++i) {
        AnsiString ln = (AnsiString)lines->Strings[i];
        AnsiString t  = ln.Trim();
        // match "KEY=" at the start (1-based Pos==1).
        AnsiString probe = key + AnsiString("=");
        if (t.Pos(probe) == 1) {
            return t.SubString(probe.Length() + 1, t.Length() - probe.Length()).Trim();
        }
    }
    return AnsiString("");
}

// Resolve a CSV column index BY NAME from a header TStringList (CommaText
// already split).  Mirrors database.cpp: AnsiPos(name)!=0 -> match; -1 if none.
static int resolveColumn(TStringList* header, const AnsiString& name) {
    int idx = -1;
    for (int i = 0; i < header->Count; ++i) {
        AnsiString col = (AnsiString)header->Strings[i];
        if (col.AnsiPos(name) != 0) idx = i;   // last match wins (as golden does)
    }
    return idx;
}

int main() {
    AnsiString sys = systemDir();
    AnsiString iniPath = sys + AnsiString("\\Gerneral.ini");
    AnsiString motPath = sys + AnsiString("\\Mot_Table.csv");
    AnsiString ioPath  = sys + AnsiString("\\IO_Table.csv");

    if (!DirectoryExists(sys) ||
        !FileExists(iniPath) || !FileExists(motPath) || !FileExists(ioPath)) {
        std::printf("SKIP test_config_db: real config not found at \"%s\" "
                    "(set HT9045_SYSTEM_DIR to override). Exiting 0.\n",
                    sys.c_str());
        return 0;
    }

    // =====================================================================
    //  1) Gerneral.ini : MOTION_CARD_TYPE=1, IO_CARD_TYPE=2
    // =====================================================================
    {
        TStringList* ini = new TStringList();
        ini->LoadFromFile(iniPath);
        CHECK(ini->Count > 0);

        AnsiString motion = iniValue(ini, AnsiString("MOTION_CARD_TYPE"));
        AnsiString iocard = iniValue(ini, AnsiString("IO_CARD_TYPE"));
        std::printf("Gerneral.ini: MOTION_CARD_TYPE=\"%s\"  IO_CARD_TYPE=\"%s\"\n",
                    motion.c_str(), iocard.c_str());
        CHECK(motion.ToIntDef(-1) == 1);   // SMC motion card
        CHECK(iocard.ToIntDef(-1) == 2);
        delete ini;
    }

    // =====================================================================
    //  2) Mot_Table.csv : 45 data rows, every data row CardModel == "SMC".
    //     Column resolved BY NAME (CardModel), not by position.
    // =====================================================================
    {
        TStringList* mot = new TStringList();
        mot->LoadFromFile(motPath);
        CHECK(mot->Count >= 2);             // header + >=1 data

        // header -> CommaText split
        TStringList* hdr = new TStringList();
        hdr->CommaText = (AnsiString)mot->Strings[0];
        int colMotorname = resolveColumn(hdr, AnsiString("Motorname"));
        int colCardModel = resolveColumn(hdr, AnsiString("CardModel"));
        std::printf("Mot_Table header: %d cols; Motorname@%d  CardModel@%d\n",
                    hdr->Count, colMotorname, colCardModel);
        CHECK(colMotorname >= 0);
        CHECK(colCardModel >= 0);

        int dataRows = 0, smcRows = 0;
        for (int r = 1; r < mot->Count; ++r) {
            AnsiString line = (AnsiString)mot->Strings[r];
            if (line.Trim().IsEmpty()) continue;     // skip blank trailing line
            TStringList* row = new TStringList();
            row->CommaText = line;
            if (row->Count == hdr->Count) {
                ++dataRows;
                AnsiString cm = ((AnsiString)row->Strings[colCardModel]).Trim();
                if (cm == AnsiString("SMC")) ++smcRows;
            }
            delete row;
        }
        std::printf("Mot_Table: %d data rows, %d SMC\n", dataRows, smcRows);
        CHECK(dataRows == 45);             // 45 axes
        CHECK(smcRows  == 45);             // all SMC
        delete hdr;
        delete mot;
    }

    // =====================================================================
    //  3) IO_Table.csv : 15 columns, 668 data rows, ISABase column (BY NAME)
    //     dominated by value 0 (644 zeros, 24 ones in the real file).
    // =====================================================================
    {
        TStringList* io = new TStringList();
        io->LoadFromFile(ioPath);
        CHECK(io->Count >= 2);

        TStringList* hdr = new TStringList();
        hdr->CommaText = (AnsiString)io->Strings[0];
        std::printf("IO_Table header: %d cols\n", hdr->Count);
        CHECK(hdr->Count == 15);           // eioTotal == 15

        int colISABase = resolveColumn(hdr, AnsiString("ISABase"));
        int colIOType  = resolveColumn(hdr, AnsiString("IOType"));
        CHECK(colIOType  >= 0);
        CHECK(colISABase >= 0);
        // golden default eioISABase==8 (database.cpp:1915) -- real header matches.
        std::printf("IO_Table: ISABase resolved by name @ col %d (golden default 8)\n",
                    colISABase);
        CHECK(colISABase == 8);

        int dataRows = 0, isaZero = 0, isaNonZero = 0;
        for (int r = 1; r < io->Count; ++r) {
            AnsiString line = (AnsiString)io->Strings[r];
            if (line.Trim().IsEmpty()) continue;
            TStringList* row = new TStringList();
            row->CommaText = line;
            if (row->Count == hdr->Count) {
                ++dataRows;
                AnsiString v = ((AnsiString)row->Strings[colISABase]).Trim();
                if (v == AnsiString("0") || v.IsEmpty()) ++isaZero;
                else ++isaNonZero;
            }
            delete row;
        }
        std::printf("IO_Table: %d data rows, ISABase==0: %d, !=0: %d\n",
                    dataRows, isaZero, isaNonZero);
        CHECK(dataRows == 668);                 // 668 rows
        CHECK(isaZero > isaNonZero);            // 0 dominant
        CHECK(isaZero == 644 && isaNonZero == 24); // exact real-file distribution
        delete hdr;
        delete io;
    }

    std::printf("test_config_db: %d/%d checks passed\n", g_total - g_fail, g_total);
    if (g_fail) { std::printf("FAILED: %d checks\n", g_fail); return 1; }
    std::printf("PASS\n");
    return 0;
}
