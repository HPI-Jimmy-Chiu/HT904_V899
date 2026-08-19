// =============================================================================
//  WebAuth.cpp -- FW-W2 login verification core. See WebAuth.h for the golden
//  provenance (main.cpp:14618-14643 + PassList comparison arms) and the
//  deliberate exclusions.
//  AI(W906-FW-W2) 20260819: new file.
// =============================================================================
#include "WebAuth.h"

#include "cpublic.h"    // SplitStrByDotSpaceOnly (golden's own splitter)
#include "common.h"     // DecodeStr, CheckAndReadIniDataGeneral, FileExists
#include "Config.h"     // IniConfig.bPasswordSecret

#include <cstring>
#include <cstdlib>

int WebAuthVerify(const AnsiString& bookPath,
                  const AnsiString& user,
                  const AnsiString& password)
{
    if (bookPath == "" || FileExists(bookPath) == false)
        return -1;

    // golden :14620 `PassList[3]->LoadFromFile(pwPath)` -- same TStringList
    // line loading, local instead of the dialog-lifetime lists.
    TStringList* lines = new TStringList();
    lines->LoadFromFile(bookPath);

    // golden's decode precondition, read the same way (the General ini must
    // already be open -- wb_serve's LoadMachineConfig did that):
    //   bChange=CheckAndReadIniDataGeneral("Password", "Change", false);
    const bool bChange = CheckAndReadIniDataGeneral("Password", "Change", false);

    const AnsiString userU = user.UpperCase();
    const AnsiString passU = password.UpperCase();

    int level = -1;
    char str[256];
    char dest[64];

    for (int i = 0; i < lines->Count; i++) {
        // golden :14622 strncpy + three SplitStrByDotSpaceOnly calls.
        std::memset(str, 0, sizeof(str));
        std::strncpy(str, AnsiString(lines->Strings[i]).c_str(), sizeof(str) - 1);

        SplitStrByDotSpaceOnly(str, dest, 20);          // field 1: user
        AnsiString entryUser(dest);
        SplitStrByDotSpaceOnly(str, dest, 20);          // field 2: level
        AnsiString entryLevel(dest);
        SplitStrByDotSpaceOnly(str, dest, 20);          // field 3: password
        AnsiString entryPass(dest);

        if (IniConfig.bPasswordSecret && bChange == true)   // golden :14633-14637
            entryPass = DecodeStr(entryPass);

        // golden's UpperCase comparison arms (user AND password, both sides).
        if (entryUser != "" &&
            userU == entryUser.UpperCase() &&
            passU == entryPass.UpperCase()) {
            level = std::atoi(entryLevel.c_str());          // golden `l=atoi(PassList[1]...)`
            break;
        }
    }

    lines->Clear();
    delete lines;
    return level;
}
