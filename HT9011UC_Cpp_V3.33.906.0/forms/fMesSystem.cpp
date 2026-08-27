// =============================================================================
//  forms/fMesSystem.cpp  --  definitions for the fMesSystem facade
//
//  AI(W906-FW3-MES1) 20260827: new file, FW wave FW3-MES1. See
//  forms/fMesSystem.h for the full safety posture / denominator / gate
//  tables / deviations banner -- not repeated here.
// =============================================================================
#include "forms/fMesSystem.h"
// AI(W906-FW3-MES1) 20260827: every global the 17 bodies below dereference
// lives in one of these (see forms/fMesSystem.h's own banner for the
// per-symbol provenance / CMake-target trace). Same "list what each include
// is for" convention as forms/fCleaning.cpp / forms/fLotInfo.cpp.
#include "cprod.h"        // TestIF_File / BinSelect[8] / TrayForm (cprod.cpp, ht9045_globals)
#include "cmydef.h"        // bNoRTBinFixFlag[3] / iTestRunMode / iFixRightHalf (cmydef.cpp, ht9045_globals)
#include "MachineType.h"   // eAuto1 (enum) / MAX_SOCKET_ROW / MAX_SOCKET_COL / TEST_MAX_BIN / eTrayCount -- header-only
#include "LastSet.h"       // LastSet (LAST_GENERAL_SET, LastSet.cpp, ht9045_globals)
// AI(W906-FW3-MES1) 20260827: RecordProcess is declared via cMyDB.h, NOT
// canary_support.h -- see forms/fMesSystem.h's "LAST_GENERAL_SET ODR TRAP"
// banner section for why: canary_support.h carries its OWN, DIFFERENT
// `struct LAST_GENERAL_SET` (a 66-field shim) that collides with the REAL,
// full struct this file needs from LastSet.h above. cMyDB.h declares the
// same real linker symbol without touching LAST_GENERAL_SET at all.
#include "cMyDB.h"         // RecordProcess(AnsiString,AnsiString="") -- real body canary_support.cpp (ht9045_sm), same exception this task's brief names (forms/fLotInfo.cpp precedent)
#include <windows.h>       // LARGE_INTEGER / QueryPerformanceFrequency / QueryPerformanceCounter / DWORD64 (MyGetTickCount64)
#include <cstdio>          // FILE / fopen / fseek / ftell / fread / fclose (uReadDeviceNumer::Read2DFile)
#include <cstdlib>         // malloc / free (uReadDeviceNumer::Read2DFile)

// =============================================================================
//  file-scope free functions (golden Mes/fVATMesFileSys.cpp, no class) -------
// =============================================================================

// AI(W906-FW3-MES1) 20260827: golden :29-53, ACTIVE -- pure TStringList/
// AnsiString parsing, no globals touched, no I/O.
// ⚠ GOLDEN QUIRK (not fixed, translated verbatim): the early `return 1;`
// path (num<1||num>3) never reaches `delete temp;` below it -- golden leaks
// `temp` on that path. Faithful translation of golden's own bug.
int ValidNoRTBinInfo(AnsiString str, TStringList* result)   //RogerYang 20250626 偉測不可複測bin功能
{
    result->Clear();
    AnsiString trimmed = str.Trim();
    if (trimmed.IsEmpty())
        return 2;

    TStringList* temp = new TStringList();
    temp->Delimiter = ',';
    temp->DelimitedText = trimmed;

    for (int i = 0; i < temp->Count; i++)
    {
        AnsiString token = AnsiString(temp->Strings[i]).Trim();   // explicit StringsProxy->AnsiString cast, vclcompat idiom (ATC/ATCInterface.cpp precedent), not a golden deviation
        int num = token.ToIntDef(-1);

        if (num < 1 || num > 3)                          //其他bin別視為錯誤 (any other bin value counts as an error)
        {
            return 1;                                     // ⚠ golden leaks `temp` here -- verbatim, see banner above
        }
        result->Add(IntToStr(num));
    }
    delete temp;
    return 0;
}

// AI(W906-FW3-MES1) 20260827: golden :1857-1864, ACTIVE -- pure Win32 QPC
// wrapper, no globals.
//===========================================================================
// v1.0 HT3000: Kirin 20200205 替代原本的GetTickCount
//===========================================================================
DWORD64 MyGetTickCount64()
{
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return (DWORD64)(((double)(now.QuadPart) / (double)(freq.QuadPart)) * 1000);   //Kirin 20200414 修改支援後三位. return now.QuadPart / freq.QuadPart * 1000;
}

// AI(W906-FW3-MES1) 20260827: golden :2219-2230, ACTIVE -- MES_JAM_COUNT is
// golden's own file-scope struct (not a class member either); mes_cmp_by_count
// is a pure comparator over it. Golden's own `vector<MES_JAM_COUNT>
// myMesJamCount;` / `myMesWarCount;` file-scope globals right after it
// (golden :2231-2232) are NOT ported here -- their only consumer is
// SendJamCode (GATE W-14), so they would be orphaned declarations with zero
// use in this delivery (minimal-declaration idiom, see forms/fMesSystem.h
// banner).
struct MES_JAM_COUNT
{
    int iCount;
    int iStopTime;
    AnsiString JamCode;
    AnsiString Message;
};
bool mes_cmp_by_count(MES_JAM_COUNT a, MES_JAM_COUNT b)
{
    return a.iCount > b.iCount;
}

// =============================================================================
//  uReadDeviceNumer ------------------------------------------------------------
// =============================================================================

// AI(W906-FW3-MES1) 20260827: golden :55-81, ACTIVE -- pure recursive cJSON
// tree walk, no globals, no I/O.
cJSON* uReadDeviceNumer::SearchObject(cJSON *InNode, AnsiString sFilter)
{
    if (InNode == NULL)
    {
        return NULL;
    }

    if (InNode->type == cJSON_Object)
    {
        cJSON *sortAsNode = cJSON_GetObjectItem(InNode, sFilter.c_str());
        if (sortAsNode != NULL)
        {
            return sortAsNode;
        }
    }
    cJSON *child = InNode->child;
    while (child != NULL)
    {
        cJSON *result = SearchObject(child, sFilter);
        if (result != NULL)
        {
            return result;
        }
        child = child->next;
    }
    return NULL;
}

// AI(W906-FW3-MES1) 20260827: golden :83-118, ACTIVE -- reads a caller-
// supplied local file path (no write, no transmission, no machine action).
// Currently has NO live caller in this delivery (its only consumer,
// Get2DIDFromServer, is GATE W-16), so it is reachable but dormant until a
// future wave un-gates that caller.
// ⚠ GOLDEN QUIRK (not fixed, translated verbatim): `fopen(...,"r")` result
// is used (`fseek(fp,...)`) with NO NULL check -- if the file does not
// exist, golden itself dereferences a NULL FILE*. Faithful translation.
bool uReadDeviceNumer::Read2DFile(AnsiString sFileNameWithPath)
{
    List2DClear();
    //read file
    FILE* fp = fopen(sFileNameWithPath.c_str(), "r");
    fseek(fp, 0, SEEK_END);                                // ⚠ golden: no NULL check on fp, see banner above
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char* file_buf = (char*)malloc(file_size + 1);
    fread(file_buf, file_size, 1, fp);
    file_buf[file_size] = '\0';
    fclose(fp);
    //
    cJSON* loaded_root = cJSON_Parse(file_buf);
    free(file_buf);//    release
    if (!loaded_root)
    {
        return false;
    }
    cJSON *result = SearchObject(loaded_root, "Units");
    if (result != NULL)
    {
        int n = cJSON_GetArraySize(result);
        for (int i = 0; i < n; i++)
        {
            cJSON* UnitID = cJSON_GetArrayItem(result, i);
            cJSON* _obj   = cJSON_GetObjectItem(UnitID, "UnitID");
            List2DoAdd(AnsiString(_obj->valuestring));
        }
        return true;
    }
    else
    {
        return false;
    }
}

// GATE (W-18): uReadDeviceNumer::WriteFile -- raw fopen(...,"w"), declared in
// forms/fMesSystem.h, deliberately NOT defined here. See header banner.

// =============================================================================
//  TfMesSystem -----------------------------------------------------------------
// =============================================================================

// AI(W906-FW3-MES1) 20260827: PORT SPLIT ctor -- golden :127-164. Fields only
// + `new` of this object's own widget stand-ins only, per the static-init
// ctor safety rule (docs/KNOWLEDGE.md "static-init ctor 不可碰 NULL 全域" --
// the SIOF trap that previously cost this tree 88/134 ctest SEGFAULTs).
// `DoInitialLotInfor()` is called at the end, mirroring golden's own
// trailing `bShow=false; DoInitialLotInfor();` -- safe here because every
// widget it touches is `new`'d earlier in THIS SAME ctor body, above,
// exactly mirroring how real VCL streams .dfm-owned components into
// existence before the user ctor body runs. See forms/fMesSystem.h
// DEVIATION D-2 for the full list of golden ctor lines NOT translated
// (LoadStringlRecordReport() [file read+delete], stringlRecordReport alloc,
// dtOEETimeStart=Now(), and five scalar fields whose only consumer is a
// GATED method).
TfMesSystem::TfMesSystem()
{
    bShow = false;
    iJamRateTotalForAlways = 0;                                             // golden :155
    bFormShowJustInitial   = false;                                         // golden :150
    bFirstMaterialsQA      = false;                                         // golden :157
    asGetRcsCheckingResult = "PASS";                                        // golden :159

    LabeledEditLotNo                              = new TfLotInfoEdit();
    LabeledHandlerDevice                          = new TfLotInfoEdit();
    LabeledTestTemp                                = new TfLotInfoEdit();
    LabeledSoakTime                                = new TfLotInfoEdit();
    LabeledEditLotSize                             = new TfLotInfoEdit();
    LabeledEditContFail                            = new TfLotInfoEdit();
    LabeledLeadCount                               = new TfLotInfoEdit();
    LabeledAutoBin1                                = new TfLotInfoEdit();
    LabeledAutoBin2                                = new TfLotInfoEdit();
    LabeledAutoBin3                                = new TfLotInfoEdit();
    LabeledFixBin1                                  = new TfLotInfoEdit();
    LabeledFixBin2                                  = new TfLotInfoEdit();
    LabeledFixBin3                                  = new TfLotInfoEdit();
    LabeledSiteGap                                  = new TfLotInfoEdit();
    LabeledForcePerPin                              = new TfLotInfoEdit();
    lbledtHandlerDeviceLowYield                     = new TfLotInfoEdit();
    lbledtProcess                                   = new TfLotInfoEdit();
    lbledtLabeledFTAutoBin1                         = new TfLotInfoEdit();
    lbledtLabeledFTAutoBin2                         = new TfLotInfoEdit();
    lbledtCustCode                                  = new TfLotInfoEdit();
    lbledtCustPart                                  = new TfLotInfoEdit();
    lbledtInternalLot                               = new TfLotInfoEdit();
    lbledtCustLotNum                                = new TfLotInfoEdit();
    lbledtDevicePassBins                            = new TfLotInfoEdit();
    lbledtDeviceRetestBins                          = new TfLotInfoEdit();
    lbledtHandlerDeviceATCcheck                     = new TfLotInfoEdit();
    lbledtHandlerDeviceSiteCheck                    = new TfLotInfoEdit();
    lbledtHandlerDeviceSOTdelay                     = new TfLotInfoEdit();
    lbledtHandlerDeviceTestTempVar                  = new TfLotInfoEdit();
    lbledtHandlerDeviceGPIBaddress                  = new TfLotInfoEdit();
    lbledtHandlerDeviceOpenBins                     = new TfLotInfoEdit();
    lbledtHandlerID                                 = new TfLotInfoEdit();
    lbledtHandlerDeviceHotplateXDevision            = new TfLotInfoEdit();
    lbledtHandlerDeviceHotplateYDevision            = new TfLotInfoEdit();
    lbledtHandlerDeviceHotplateXStart               = new TfLotInfoEdit();
    lbledtHandlerDeviceHotplateYStart               = new TfLotInfoEdit();
    lbledtHandlerDeviceHotplateXPitch               = new TfLotInfoEdit();
    lbledtHandlerDeviceHotplateYPitch               = new TfLotInfoEdit();
    lbledtHandlerDeviceTrayXDevision                = new TfLotInfoEdit();
    lbledtHandlerDeviceTrayYDevision                = new TfLotInfoEdit();
    lbledtHandlerDeviceTrayXStart                   = new TfLotInfoEdit();
    lbledtHandlerDeviceTrayYStart                   = new TfLotInfoEdit();
    lbledtHandlerDeviceTrayXPitch                   = new TfLotInfoEdit();
    lbledtHandlerDeviceTrayYPitch                   = new TfLotInfoEdit();
    lbledtHandlerDeviceSiteLayouts                  = new TfLotInfoEdit();
    lbledtHandlerDeviceCleanPadLimit                = new TfLotInfoEdit();
    lbledtHandlerDeviceInshuttleSensorCheck         = new TfLotInfoEdit();
    lbledtTestTimePcs                               = new TfLotInfoEdit();
    lbledtC1                                        = new TfLotInfoEdit();
    lbledtTestSites                                 = new TfLotInfoEdit();
    lbledtIndexTime                                 = new TfLotInfoEdit();
    lbledtHandlerDeviceNotRepeatableArea            = new TfLotInfoEdit();
    lbledtHandlerDeviceICNum                        = new TfLotInfoEdit();
    labEocapStatus                                  = new TLabel();
    listbMesFileLog                                 = new TListBox();

    for (int i = 0; i < eTrayCount; i++)
    {
        asFTUnloadBin[i] = new TStringList();
        asUnloadBin[i]   = new TStringList();
    }
    asPassUnloadBin = new TStringList();
    asFailUnloadBin = new TStringList();
    stOEEState12hList = new TStringList();

    DoInitialLotInfor();                                                    // golden :163, safe -- see banner above
}

// AI(W906-FW3-MES1) 20260827: PORT-ONLY destructor (DEVIATION D-3) -- frees
// this wave's OWNED raw pointers. NOT a translation of golden's own dtor
// (golden :2135-2162 calls SaveStringlRecordReport, GATE W-08, as its first
// line -- a write). Same idiom as forms/fCleaning.h / forms/fIoSetView.h.
TfMesSystem::~TfMesSystem()
{
    delete LabeledEditLotNo;
    delete LabeledHandlerDevice;
    delete LabeledTestTemp;
    delete LabeledSoakTime;
    delete LabeledEditLotSize;
    delete LabeledEditContFail;
    delete LabeledLeadCount;
    delete LabeledAutoBin1;
    delete LabeledAutoBin2;
    delete LabeledAutoBin3;
    delete LabeledFixBin1;
    delete LabeledFixBin2;
    delete LabeledFixBin3;
    delete LabeledSiteGap;
    delete LabeledForcePerPin;
    delete lbledtHandlerDeviceLowYield;
    delete lbledtProcess;
    delete lbledtLabeledFTAutoBin1;
    delete lbledtLabeledFTAutoBin2;
    delete lbledtCustCode;
    delete lbledtCustPart;
    delete lbledtInternalLot;
    delete lbledtCustLotNum;
    delete lbledtDevicePassBins;
    delete lbledtDeviceRetestBins;
    delete lbledtHandlerDeviceATCcheck;
    delete lbledtHandlerDeviceSiteCheck;
    delete lbledtHandlerDeviceSOTdelay;
    delete lbledtHandlerDeviceTestTempVar;
    delete lbledtHandlerDeviceGPIBaddress;
    delete lbledtHandlerDeviceOpenBins;
    delete lbledtHandlerID;
    delete lbledtHandlerDeviceHotplateXDevision;
    delete lbledtHandlerDeviceHotplateYDevision;
    delete lbledtHandlerDeviceHotplateXStart;
    delete lbledtHandlerDeviceHotplateYStart;
    delete lbledtHandlerDeviceHotplateXPitch;
    delete lbledtHandlerDeviceHotplateYPitch;
    delete lbledtHandlerDeviceTrayXDevision;
    delete lbledtHandlerDeviceTrayYDevision;
    delete lbledtHandlerDeviceTrayXStart;
    delete lbledtHandlerDeviceTrayYStart;
    delete lbledtHandlerDeviceTrayXPitch;
    delete lbledtHandlerDeviceTrayYPitch;
    delete lbledtHandlerDeviceSiteLayouts;
    delete lbledtHandlerDeviceCleanPadLimit;
    delete lbledtHandlerDeviceInshuttleSensorCheck;
    delete lbledtTestTimePcs;
    delete lbledtC1;
    delete lbledtTestSites;
    delete lbledtIndexTime;
    delete lbledtHandlerDeviceNotRepeatableArea;
    delete lbledtHandlerDeviceICNum;
    delete labEocapStatus;
    delete listbMesFileLog;

    for (int i = 0; i < eTrayCount; i++)
    {
        delete asFTUnloadBin[i];
        delete asUnloadBin[i];
    }
    delete asPassUnloadBin;
    delete asFailUnloadBin;
    delete stOEEState12hList;
}

// AI(W906-FW3-MES1) 20260827: golden :248-318, ACTIVE -- every touched
// symbol is an own widget/own TStringList/own array, no other global. Two
// fields (lbledtInternalLot/lbledtC1) reset to the literal "Unknown" rather
// than "" -- golden's own asymmetry, see forms/fMesSystem.h GOLDEN NOTE G-3.
void TfMesSystem::DoInitialLotInfor()
{
    LabeledHandlerDevice->Text = "";
    LabeledEditLotSize->Text = "";
    LabeledSiteGap->Text = "";
    LabeledEditContFail->Text = "";
    LabeledTestTemp->Text = "";
    LabeledSoakTime->Text = "";
    lbledtLabeledFTAutoBin1->Text = "";
    lbledtLabeledFTAutoBin2->Text = "";
    LabeledAutoBin1->Text = "";
    LabeledAutoBin2->Text = "";
    LabeledAutoBin3->Text = "";
    LabeledFixBin1->Text = "";
    LabeledFixBin2->Text = "";
    LabeledFixBin3->Text = "";
    LabeledLeadCount->Text = "";
    LabeledForcePerPin->Text = "";
    lbledtHandlerDeviceLowYield->Text = "";
    lbledtProcess->Text = "";
    lbledtCustCode->Text = "";
    lbledtCustPart->Text = "";
    lbledtInternalLot->Text = "Unknown";
    lbledtCustLotNum->Text = "";
    lbledtDevicePassBins->Text = "";
    lbledtHandlerDeviceOpenBins->Text = "";
    lbledtDeviceRetestBins->Text = "";
    lbledtHandlerDeviceATCcheck->Text = "";
    lbledtHandlerDeviceSOTdelay->Text = "";
    lbledtHandlerDeviceGPIBaddress->Text = "";
    lbledtHandlerDeviceTestTempVar->Text = "";
    lbledtHandlerDeviceSiteCheck->Text = "";
    lbledtHandlerID->Text = "";
    lbledtHandlerDeviceHotplateXDevision->Text = "";
    lbledtHandlerDeviceHotplateYDevision->Text = "";
    lbledtHandlerDeviceHotplateXStart->Text = "";
    lbledtHandlerDeviceHotplateYStart->Text = "";
    lbledtHandlerDeviceHotplateXPitch->Text = "";
    lbledtHandlerDeviceHotplateYPitch->Text = "";
    lbledtHandlerDeviceTrayXDevision->Text = "";
    lbledtHandlerDeviceTrayYDevision->Text = "";
    lbledtHandlerDeviceTrayXStart->Text = "";
    lbledtHandlerDeviceTrayYStart->Text = "";
    lbledtHandlerDeviceTrayXPitch->Text = "";
    lbledtHandlerDeviceTrayYPitch->Text = "";
    lbledtHandlerDeviceSiteLayouts->Text = "";
    lbledtHandlerDeviceCleanPadLimit->Text = "";
    lbledtHandlerDeviceInshuttleSensorCheck->Text = "";
    lbledtTestTimePcs->Text = "";
    lbledtC1->Text = "Unknown";
    lbledtTestSites->Text = "";
    lbledtIndexTime->Text = "";
    lbledtHandlerDeviceICNum->Text = "";                                    //RogerYang 20250821 : 新增良率中的數量比對
    lbledtHandlerDeviceNotRepeatableArea->Text = "";                        //RogerYang 20250717 偉測更改欄位名稱，為了不跟bin別混淆 //RogerYang 20250604 偉測不可複測bin功能

    for (int i = 0; i < eTrayCount; i++)
    {
        asFTUnloadBin[i]->Clear();
        asUnloadBin[i]->Clear();
    }

    asPassUnloadBin->Clear();
    asFailUnloadBin->Clear();

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 4; j++)
            iSiteLayouts[i][j] = -1;
}

// AI(W906-FW3-MES1) 20260827: golden :2214-2217, ACTIVE -- own member only.
// This is one of the 11 `#if 0`-pre-committed names (cinitial.cpp:9728);
// still `#if 0` after this wave (that file is out of this wave's write
// boundary), see forms/fMesSystem.h banner.
void TfMesSystem::CleaOEEState12hList()
{
    stOEEState12hList->Clear();
}

// AI(W906-FW3-MES1) 20260827: golden :1871-1896, ACTIVE -- LastSet fields
// (LastSet.cpp, ht9045_globals) + own member + header-only enum/const loop
// bounds, no I/O. One of the 11 `#if 0`-pre-committed names
// (ainarm9045.cpp:5235,:5240); still `#if 0` after this wave, see header
// banner.
void TfMesSystem::DoInitailRecordReportByTime()
{
    iJamRateTotalForAlways = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = eAuto1; j <= iFixRightHalf; j++)
        {
            if (i == 2 || i == 3)
                continue;

            LastSet.iBinCTForAlways[i][j] = 0;
        }
    }

    for (int i = 0; i < MAX_SOCKET_ROW; i++)
    {
        for (int j = 0; j < MAX_SOCKET_COL; j++)
        {
            LastSet.iSiteTotalCTForAlways[i][j] = 0;

            for (int k = 0; k < TEST_MAX_BIN; k++)
                LastSet.iSiteBinCTForAlways[i][j][k] = 0;
        }
    }
}

// AI(W906-FW3-MES1) 20260827: golden :2234-2245, ACTIVE -- pure AnsiString
// substring parsing, no globals.
int TfMesSystem::TimeToSec(AnsiString asString)
{
    int iSec = 0;
    AnsiString asStr;
    asStr = asString.SubString(0, 2);
    iSec = asStr.ToIntDef(0) * 3600;
    asStr = asString.SubString(4, 2);
    iSec += asStr.ToIntDef(0) * 60;
    asStr = asString.SubString(7, 2);
    iSec += asStr.ToIntDef(0);
    return iSec;
}

// AI(W906-FW3-MES1) 20260827: golden :2912-2936, ACTIVE -- own widget +
// RecordProcess (real body canary_support.cpp, ht9045_sm exception this
// task's brief names; see header banner). No file write, no transmission.
// ⚠ GOLDEN QUIRK (verbatim): if `asString.Pos("eocapStatus")` returns 0 (not
// found), `SubString(13,1)` still runs unguarded -- see GOLDEN NOTE G-2.
bool TfMesSystem::CheckEocapStatus(AnsiString asString)   //jou 20221104 : VTest CreateManualEOCAP function;
{
    int iStart = 0;
    bool bResult = false;
    AnsiString asResult;
    iStart = asString.Pos("eocapStatus");
    asResult = asString.SubString(iStart + 13, 1);         // ⚠ golden: no guard on iStart==0, see banner above
    if (asResult == "1")
    {
        labEocapStatus->Caption = "eocapStatus : 1 ( 已結案 )";
        bResult = true;
    }
    else if (asResult == "0")
    {
        labEocapStatus->Caption = "eocapStatus : 0 ( 處理中 )";
    }
    else
    {
        labEocapStatus->Caption = "eocapStatus : null";
    }

    if (bShow == true)
        RecordProcess(labEocapStatus->Caption);
    return bResult;
}

// AI(W906-FW3-MES1) 20260827: golden :3062-3066, ACTIVE -- own member only.
// Signature drops `TObject *Sender, TCloseAction &Action` -- TCloseAction
// has no port anywhere in this tree, same established precedent as
// ATC/ATCInterface.h:391 / forms/fIoSetView.h D-7 (DEVIATION D-5).
void TfMesSystem::FormClose()
{
    bShow = false;
}

// AI(W906-FW3-MES1) 20260827: golden :3068-3076, ACTIVE -- pure string
// compare, no globals.
bool TfMesSystem::CheckVTENGmode(AnsiString asLotId)
{
    if (asLotId == "VTENGrecipe" || asLotId == "VTEErecipe")
    {
        return true;
    }
    else
        return false;
}

// AI(W906-FW3-MES1) 20260827: golden :3178-3188, ACTIVE -- own member only.
// ZERO-WRITER FIELD: map2DIDFromServer is populated only by
// Get2DIDFromServer (GATE W-16), so this always reads empty and returns
// false this wave -- see header banner.
bool TfMesSystem::IsMatchServerData(AnsiString as2DID)
{
    for (unsigned int i = 0; i < map2DIDFromServer.size(); i++)
    {
        if (map2DIDFromServer[(int)i].Pos(as2DID) > 0)
        {
            return true;
        }
    }
    return false;
}

// AI(W906-FW3-MES1) 20260827: golden :3190-3197, ACTIVE -- own member only,
// no I/O.
void TfMesSystem::RecordMsg(AnsiString asMsg)
{
    if (listbMesFileLog->Items->Count > 10000)
    {
        listbMesFileLog->Items->Clear();
    }
    listbMesFileLog->Items->Add(asMsg);
}

// AI(W906-FW3-MES1) 20260827: golden :3354-3367, ACTIVE -- TestIF_File /
// BinSelect[] / iTestRunMode (cprod.cpp/cmydef.cpp, ht9045_globals). One of
// the 11 `#if 0`-pre-committed names (atester.cpp:3271); still `#if 0` after
// this wave, see header banner.
bool TfMesSystem::AutoSiteMapPass(int iBin)
{
    if (TestIF_File.iASMPassBin == -1)
    {
        if (BinSelect[iTestRunMode].iStackDefFailCate[BinSelect[iTestRunMode].iCatDataT3Pos[iBin] - 1] == 0)
            return true;
    }
    else
    {
        if (iBin == TestIF_File.iASMPassBin)
            return true;
    }
    return false;
}

// AI(W906-FW3-MES1) 20260827: golden :3369-3382, ACTIVE -- TrayForm
// (cprod.cpp) + bNoRTBinFixFlag[] (cmydef.cpp, ht9045_globals) + own member.
// One of the 11 `#if 0`-pre-committed names (forms/fLotInfo.cpp:2696,:2708 --
// that file's own WB-14 gate already names this exact call site and returns
// a hardcoded conservative `true` pending a real facade; still `#if 0` after
// this wave since forms/fLotInfo.cpp is out of this wave's write boundary,
// see header banner).
bool TfMesSystem::NeedNoRTBinID(int iFix)   //RogerYang 20250626 偉測不可複測bin功能
{
    if (TrayForm.bVTestNoRTBin == false)
    {
        return false;
    }
    else
    {
        if (bNoRTBinFlag[iFix] == false && bNoRTBinFixFlag[iFix] == true)    //ID還沒比對過 && 該Fix需要比對
            return true;
        else
            return false;
    }
}

// AI(W906-FW3-MES1) 20260827: TfMesSystem/fMesSystem globals were FREE
// tree-wide (see forms/fMesSystem.h banner) -- declared per this wave's task
// brief so the 11 `#if 0` call sites elsewhere have something real to bind
// to once each site's OWN file is separately un-gated (not done by this
// wave). Same idiom as forms/fCleaning.cpp:45 / forms/fLotInfo.cpp:4417.
TfMesSystem *fMesSystem = new TfMesSystem();
