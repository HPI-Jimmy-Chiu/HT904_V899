//---------------------------------------------------------------------------
//  SECSGEM/uHGemEquipment.cpp -- THGem, first real slice
//
//  Translation wave: W906-uHGemEquipment
//  Translator: AI(W906-uHGemEquipment) 20260716
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/SECSGEM/uHGemEquipment.cpp
//  (9353 lines total; this file translates a bounded subset -- see
//  uHGemEquipment.h's file-head note for the full scope statement and the
//  list of what is deliberately NOT here yet).
//
//  This TU deliberately does NOT include golden's own include list
//  (MachineDefine.h/cMydef.h/database.h/myQwertyKeyBoard.h/common.h/
//  uHGemHT9045.h/SgdToXLS.h) -- none of those are needed by this wave's
//  StringGrid-only method family; pulling them in would drag in the entire
//  untranslated Handler state-machine graph for no reason (same
//  extract-calc-core discipline the project has used since Public/HTMD5).
//---------------------------------------------------------------------------
#include "SECSGEM/uHGemEquipment.h"

#include <cstdio>
#include <cstdlib>   // atoi
#include <cstring>   // strcpy/strncpy
#include <cstddef>   // NULL

// ---------------------------------------------------------------------------
//  MyDBIProcess -- process-wide diagnostics/log sink (no-op in this
//  translation; see aHotPlateSubstrate.h:576 / aHotPlateSubstrate.cpp:703 for
//  the one real definition already established elsewhere in this tree).
//  Declared locally (matching the SAME established pattern already used by
//  CCLink/MyCCLink.cpp and KYECFTP/FTPClient_EventHandlers.cpp) rather than
//  `#include "aHotPlateSubstrate.h"` -- that header pulls in a large,
//  unrelated hardware-shim surface (TMySucker/TMyKitSuck/TInLaserCheck/...)
//  this StringGrid-only unit has no other need for.
// ---------------------------------------------------------------------------
extern void MyDBIProcess(AnsiString S1, AnsiString S2);

// ---------------------------------------------------------------------------
//  IncludeTrailingPathDelimiter -- golden calls this exact SysUtils name
//  (uHGemEquipment.cpp:6225, inside ReadAlamData). vclcompat/SysUtils.h only
//  exposes IncludeTrailingBackslash (semantically identical: appends the
//  platform path separator iff not already present) -- SysUtils.h/.cpp are
//  NOT in this front's allowed file list, so this is a thin LOCAL wrapper
//  (file-scope, not exported) rather than an addition to that shared shim.
// ---------------------------------------------------------------------------
static AnsiString IncludeTrailingPathDelimiter(const AnsiString &path)
{
    return IncludeTrailingBackslash(path);
}

//===========================================================================
//  GemTimer  (golden uHGemEquipment.cpp:250-309)
//===========================================================================
//---------------------------------------------------------------------------
// V 1.0
//  設定計時器 (gloss: "arm the timer") -- iTime is in units of 0.1s
//---------------------------------------------------------------------------
void GemTimer::TimerSet(int iTime)
{
    iTimeLen = iTime * 100;
}
//---------------------------------------------------------------------------
void GemTimer::TimerSetSecAndOn(double iTime)
{
    // AI(W906-uHGemEquipment) 20260716: golden is `iTimeLen=iTime*1000;`, an
    // implicit double->int narrowing (iTimeLen is `int`). static_cast makes
    // the existing golden truncation explicit (silences -Wconversion);
    // behavior/value is unchanged.
    iTimeLen = static_cast<int>(iTime * 1000);
    ulStartTicks = ::GetTickCount();   // golden: MyTickCount() -- see header note
}
//---------------------------------------------------------------------------
void GemTimer::TimerSetMSAndOn(int iTime)
{
    iTimeLen = iTime;
    ulStartTicks = ::GetTickCount();   // golden: MyTickCount() -- see header note
}
//---------------------------------------------------------------------------
// V 1.0
//  開始計時器 (gloss: "(re)start the timer")
//---------------------------------------------------------------------------
void GemTimer::TimerOn()
{
    ulStartTicks = ::GetTickCount();   // golden: MyTickCount() -- see header note
}
//---------------------------------------------------------------------------
// V 1.0
//  讀取計時器是否時間到 (gloss: "has the armed interval elapsed?")
//---------------------------------------------------------------------------
bool GemTimer::TimerOff()
{
    if (ulStartTicks == 0)
        return false;

    if (iTimeLen <= 0)
    {   // jou 2012-01-04 gloss: "guards against a mis-entered negative/zero
        // duration hanging the caller forever" (golden uHGemEquipment.cpp:284)
        return true;
    }
    DWORD ulLimited = ulStartTicks + iTimeLen;
    DWORD ulNowTicks = ::GetTickCount();
    DWORD ulNowTicksOver = 0;
    if (ulLimited < ulStartTicks)                                          // DWORD wraparound
    {
        ulLimited = 0xFFFFFFFF - ulStartTicks + iTimeLen;
        ulNowTicksOver = 0xFFFFFFFF - ulStartTicks + ulNowTicks;

        if (ulNowTicksOver < ulStartTicks &&                                // if the "now" tick also wrapped
            ulNowTicksOver > ulLimited)                                     // and has passed the limit
        {
            return true;
        }
    }
    else
    {
        if (ulNowTicks >= (ulStartTicks + iTimeLen))
        {
            return true;
        }
    }
    return false;
}

//===========================================================================
//  THGem -- construction / destruction
//===========================================================================
// Grid sizes match golden's .dfm design-time RowCount/ColCount exactly (see
// uHGemEquipment.h's member-declaration comments for the per-grid citation).
// TStringGrid's ctor signature is (initialColCount, initialRowCount).
// AI(W906-fire-verify) 20260716: fire-wave fidelity review flagged (low) that
// allocating all 4 grids directly in the member-initializer list had no
// exception safety net -- if e.g. the 3rd `new` threw (std::bad_alloc), the
// first 2 already-constructed grids would leak (the partially-constructed
// THGem's destructor never runs on a constructor-body exception). golden's
// THGem is DFM/VCL-instantiated so this ctor/dtor pair is new code introduced
// by this wave, not a fidelity concern; fixed here by NULL-initializing the
// members and allocating in the body with an explicit try/catch that cleans
// up whichever grids DID succeed before rethrowing (delete on a NULL pointer
// is a safe no-op, so this is correct regardless of which `new` failed).
THGem::THGem()
    : strGrdAlarmOld(NULL),
      strGrdCEID(NULL),
      stdGridReportID(NULL),
      strGrdAlarm(NULL),
      GemSystemPath("")
{
    try
    {
        strGrdAlarmOld  = new TStringGrid(5, 1);
        strGrdCEID      = new TStringGrid(258, 1025);
        stdGridReportID = new TStringGrid(1026, 257);
        strGrdAlarm     = new TStringGrid(12, 5);
    }
    catch (...)
    {
        delete strGrdAlarmOld;
        delete strGrdCEID;
        delete stdGridReportID;
        delete strGrdAlarm;
        throw;
    }
}

THGem::~THGem()
{
    delete strGrdAlarmOld;
    delete strGrdCEID;
    delete stdGridReportID;
    delete strGrdAlarm;
}

//===========================================================================
//  Alarm-grid siblings (golden uHGemEquipment.cpp:6211-6274)
//===========================================================================
void THGem::SetAlamData(int iRowCount, AnsiString ALID, AnsiString Class, AnsiString ALTX, AnsiString Position)
{
    // golden writes ALTX (Alarm Text) into no cell here -- this is a genuine
    // golden quirk, preserved verbatim: the ALTX parameter is accepted but
    // never used inside SetAlamData's own body (uHGemEquipment.cpp:6211-6217
    // only ever touches cols 7/8/9/10; col 6, "Alarm Text", is populated by
    // other out-of-scope code elsewhere, not by this method). Do NOT "fix"
    // this by writing ALTX somewhere golden doesn't.
    (void)ALTX;
    strGrdAlarm->Cells[7][iRowCount] = 1;
    strGrdAlarm->Cells[8][iRowCount] = ALID;
    strGrdAlarm->Cells[9][iRowCount] = Class;
    strGrdAlarm->Cells[10][iRowCount] = Position;
}
//------------------------------------------------------------------------------
void THGem::ReadAlamData()
{
    // AI(W906-uHGemEquipment) 20260716: GOLDEN QUIRK, preserved verbatim --
    // ReadAlamData loads "AlarmData.def" into `strGrdAlarmOld`, NOT the live
    // `strGrdAlarm` grid that ReportAlarm (out of scope) reads from and that
    // WriteAlamData (below) saves FROM. strGrdAlarmOld appears to serve as a
    // "previous configuration" snapshot for some out-of-scope comparison
    // logic elsewhere in the real THGem -- NOT a bug to "fix" by redirecting
    // this to strGrdAlarm (golden uHGemEquipment.cpp:6219-6260).
    TStringList *memoPtr;
    AnsiString Filename;

    memoPtr = new TStringList;
    Filename = IncludeTrailingPathDelimiter(GemSystemPath) + AnsiString("AlarmData.def");

    if (FileExists(Filename) == false)
    {
        delete memoPtr;
        return;
    }

    memoPtr->LoadFromFile(Filename);

    // Ifor 20170523 (wei) gloss: "guards against a genuinely empty
    // AlarmData.def crashing SECS/GEM startup" -- trims trailing blank lines.
    try
    {
        while (1)
        {
            if (memoPtr->Count == 0)
                break;

            if (memoPtr->Strings[memoPtr->Count - 1] == "")
                memoPtr->Delete(memoPtr->Count - 1);
            else
                break;
        }
        strGrdAlarmOld->RowCount = memoPtr->Count;
        PasteStringGridAsTabFormat(strGrdAlarmOld, memoPtr);
    }
    catch (...)
    {
        MyDBIProcess("Exception", "THGem::ReadAlamData");
        memoPtr->Clear();
        delete memoPtr;
        // AI(W906-uHGemEquipment) 20260716: golden falls through to the
        // (identical) RowCount=/PasteStringGridAsTabFormat pair below EVEN
        // on the exception path, using `memoPtr` AFTER it was just deleted
        // above (uHGemEquipment.cpp:6253-6257) -- a genuine golden
        // use-after-free bug. Preserved: see the duplicated block below,
        // which is reached unconditionally (both normal and catch paths),
        // exactly matching golden's control flow. Do NOT dereference the
        // already-deleted memoPtr a second time here; the duplicate below is
        // executed after `return`-free fallthrough, same as golden.
    }
    // AI(W906-uHGemEquipment) 20260716: GOLDEN BUG, preserved verbatim
    // (uHGemEquipment.cpp:6256-6257): this exact RowCount=/Paste pair is
    // ALSO the last two statements inside the `try` block above, so on the
    // non-exception path it runs twice (harmless: idempotent). On the
    // exception path, golden's `memoPtr` was already `delete`d in the catch
    // block just above -- this is a real use-after-free in golden. We
    // preserve golden's CONTROL FLOW (the statements are textually
    // duplicated, unconditionally reached) but do not attempt to fix or
    // paper over the dangling-pointer read; a build with a hardening
    // allocator (e.g. under ASan) would legitimately trip here on the
    // exception path, exactly as it would against the original BCB6 binary.
    strGrdAlarmOld->RowCount = memoPtr->Count;
    PasteStringGridAsTabFormat(strGrdAlarmOld, memoPtr);
    memoPtr->Clear();
    delete memoPtr;
}
//------------------------------------------------------------------------------
void THGem::WriteAlamData()
{
    TStringList *memoPtr;
    AnsiString Filename;

    memoPtr = new TStringList;
    CopyStringGridAsTabFormat(strGrdAlarm, memoPtr);

    Filename = IncludeTrailingPathDelimiter(GemSystemPath) + AnsiString("AlarmData.def");
    memoPtr->SaveToFile(Filename);
    memoPtr->Clear();
    delete memoPtr;
}

//===========================================================================
//  CEID / Report StringGrid-backed "database" family
//  (golden uHGemEquipment.cpp:7361-7967, 8615-8665)
//===========================================================================
//------------------------------------------------------------------------------
// mode=1 是Handler ,=0 是 Host 定義的 (gloss: "Mode 1 = defined by the
// Handler itself, Mode 0 = defined by the Host" -- golden uHGemEquipment.cpp:7361)
void THGem::SetCEIDContent(unsigned iCeid, AnsiString CeidAlias, unsigned iReportCount, unsigned *iReportIDData, int Mode)
{
    // AI(W906-fire-verify) 20260716: `pos` is zero-initialized here; golden
    // leaves `int pos;` uninitialized (uHGemEquipment.cpp:7363). Behaviorally
    // inert either way -- every read of `pos` below is guarded by
    // `bSearchOK`, which is only ever true after `pos` was itself just
    // assigned a few lines above -- but flagged per fire-wave fidelity review
    // as a literal (if harmless) deviation from verbatim translation.
    int pos = 0;
    bool bSearchOK = false;
    unsigned int iBackID[SECS_EVENT.TotalEvent], ct = 0;
    AnsiString S;

    for (int y = 1; y < strGrdCEID->RowCount; y++)
    {
        if (strGrdCEID->Cells[0][y] == iCeid)
        {
            bSearchOK = true;
            pos = y;
            break;
        }
    }

    if (bSearchOK == false)
    {
        for (int y = 1; y < strGrdCEID->RowCount; y++)
        {
            if (strGrdCEID->Cells[0][y] == "")
            {
                strGrdCEID->Cells[0][y] = iCeid;
                strGrdCEID->Cells[1][y] = Mode;
                strGrdCEID->Cells[2][y] = CeidAlias;

                for (int i = 3; i < strGrdCEID->ColCount; i++)
                    strGrdCEID->Cells[i][y] = "";
                for (unsigned int x = 3; x < (iReportCount + 3); x++)
                    strGrdCEID->Cells[x][y] = iReportIDData[x - 3];
                break;
            }
        }
    }
    else
    {
        strGrdCEID->Cells[2][pos] = CeidAlias;
        for (int i = 3; i < strGrdCEID->ColCount; i++)
        {
            S = strGrdCEID->Cells[i][pos];
            if (S == "")
                continue;
            if (GetReportIDType(S) == (1 - Mode))
            {
                iBackID[ct] = atoi(S.c_str());
                ct++;
            }
        }
        for (int i = 3; i < strGrdCEID->ColCount; i++)
            strGrdCEID->Cells[i][pos] = "";
        for (unsigned int x = 0; x < iReportCount; x++)
            strGrdCEID->Cells[x + 3][pos] = iReportIDData[x];
        for (unsigned int x = 0; x < ct; x++)
            strGrdCEID->Cells[x + iReportCount + 3][pos] = iBackID[x];
    }
    SaveEventReportData();
}
//------------------------------------------------------------------------------
// mode=1 是Handler ,=0 是 Host 定義的
void THGem::SetCEIDContent(unsigned iCeid, unsigned iReportCount, unsigned *iReportIDData, int Mode)
{
    SetCEIDContent(iCeid, "", iReportCount, iReportIDData, Mode);
}
//------------------------------------------------------------------------------
unsigned THGem::GetCEIDContent(unsigned iCeid, unsigned *iReportIDData, int Mode)
{
    (void)Mode;   // golden also never reads Mode inside this method's body
    int ct = 0;
    for (int y = 1; y < strGrdCEID->RowCount; y++)
    {
        if (strGrdCEID->Cells[0][y] == iCeid)
        {
            // AI(W906-uHGemEquipment) 20260716: GOLDEN BUG, preserved
            // verbatim (uHGemEquipment.cpp:7432): loop bound is
            // `strGrdCEID->RowCount`, not `->ColCount` as every other sibling
            // in this family uses (e.g. SetCEIDContent's own equivalent loop
            // just above uses `->ColCount`). strGrdCEID is 1025 rows x 258
            // cols, so RowCount(1025) > ColCount(258) here -- harmless in
            // practice ONLY because the loop always `break`s on the first ""
            // cell, which (per SetCEIDContent's own writer logic) is never
            // later than column 257. If a CEID row were ever fully packed
            // with report IDs across all 255 usable slots, this would walk
            // past the real column count and hit vclcompat::TStringGrid's
            // out-of-range check (std::out_of_range) -- matching real VCL's
            // own ERangeError in that scenario. NOT fixed here.
            for (int x = 3; x < strGrdCEID->RowCount; x++)
            {
                if (strGrdCEID->Cells[x][y] == "")
                    break;
                iReportIDData[ct] = static_cast<unsigned>(atoi(strGrdCEID->Cells[x][y].c_str()));
                ct++;
            }
            return static_cast<unsigned>(ct);
        }
    }
    return 0;
}
//------------------------------------------------------------------------------
// mode=1 是Handler ,=0 是 Host 定義的
bool THGem::SetReportIDContent(unsigned iCeid, unsigned iReportCount, unsigned *iReportIDData, int Mode)
{
    // AI(W906-fire-verify) 20260716: same zero-init-vs-golden-uninitialized
    // `pos` deviation as SetCEIDContent above (golden uHGemEquipment.cpp:7447)
    // -- behaviorally inert, see that function's comment for the full note.
    int pos = 0;
    bool bSearchOK = false;

    if ((int)(iReportCount + 2) > stdGridReportID->ColCount)
        return false;

    for (int y = 1; y < stdGridReportID->RowCount; y++)
    {
        if (stdGridReportID->Cells[0][y] == iCeid)
        {
            bSearchOK = true;
            pos = y;
            break;
        }
    }

    if (bSearchOK == false)
    {
        for (int y = 1; y < stdGridReportID->RowCount; y++)
        {
            if (stdGridReportID->Cells[0][y] == "")
            {
                stdGridReportID->Cells[0][y] = iCeid;
                stdGridReportID->Cells[1][y] = Mode;
                for (int x = 2; x < stdGridReportID->ColCount; x++)
                    stdGridReportID->Cells[x][y] = "";
                for (unsigned int i = 2; i < (iReportCount + 2); i++)
                    stdGridReportID->Cells[i][y] = iReportIDData[i - 2];
                bSearchOK = true;
                break;
            }
        }
    }
    else
    {
        for (int x = 2; x < stdGridReportID->ColCount; x++)
            stdGridReportID->Cells[x][pos] = "";
        for (unsigned int i = 2; i < (iReportCount + 2); i++)
            stdGridReportID->Cells[i][pos] = iReportIDData[i - 2];
    }
    SaveEventReportData();
    return bSearchOK;
}
//------------------------------------------------------------------------------
// 2013/11/20 lee
bool THGem::DeleteReportID(unsigned iCeid, int Type)
{
    // AI(W906-fire-verify) 20260716: same zero-init-vs-golden-uninitialized
    // `pos` deviation as SetCEIDContent above (golden uHGemEquipment.cpp:7493)
    // -- behaviorally inert, see that function's comment for the full note.
    int pos = 0, iY;
    bool bSearchOK = false;

    for (int y = 1; y < stdGridReportID->RowCount; y++)
    {
        if (stdGridReportID->Cells[0][y] == iCeid &&
            stdGridReportID->Cells[1][y] == Type)
        {
            pos = y;
            bSearchOK = true;
            break;
        }
    }

    if (bSearchOK == true)
    {
        for (int y = pos; y < (stdGridReportID->RowCount - 1); y++)
        {
            for (int x = 0; x < stdGridReportID->ColCount; x++)
                stdGridReportID->Cells[x][y] = stdGridReportID->Cells[x][y + 1];
        }

        iY = stdGridReportID->RowCount - 1;
        for (int x = 0; x < stdGridReportID->ColCount; x++)
            stdGridReportID->Cells[x][iY] = "";
        return true;
    }
    else
    {
        return false;
    }
}
//------------------------------------------------------------------------------
bool THGem::DeleteReportIDOfCeid(unsigned iCeid)
{
    int ct;
    AnsiString S[1024];

    for (int y = 1; y < strGrdCEID->RowCount; y++)
    {
        for (int x = 3; x < strGrdCEID->ColCount; x++)
        {
            if (strGrdCEID->Cells[x][y] == iCeid)
                strGrdCEID->Cells[x][y] = "";
        }
        ct = 0;
        for (int x = 3; x < strGrdCEID->ColCount; x++)
        {
            if (strGrdCEID->Cells[x][y] != "")
            {
                S[ct] = strGrdCEID->Cells[x][y];
                ct++;
            }
        }

        for (int x = 3; x < strGrdCEID->ColCount; x++)
            strGrdCEID->Cells[x][y] = "";

        for (int i = 0; i < ct; i++)
            strGrdCEID->Cells[3 + i][y] = S[i];
    }
    return true;
}
//------------------------------------------------------------------------------
// 2013/11/20 lee
bool THGem::DeleteAllHostDefineReportID()
{
    unsigned iReportID;
    bool bSearchOK;
    do
    {
        bSearchOK = false;
        for (int y = 1; y < stdGridReportID->RowCount; y++)
        {
            if (stdGridReportID->Cells[1][y] == "0")
            {
                iReportID = static_cast<unsigned>(atoi(stdGridReportID->Cells[0][y].c_str()));
                DeleteReportID(iReportID, 0);
                DeleteReportIDOfCeid(iReportID);
                bSearchOK = true;
                break;
            }
        }
    } while (bSearchOK == true);

    return true;
}
//------------------------------------------------------------------------------
unsigned THGem::GetReportIDContent(unsigned iCeid, unsigned *iReportIDData, int Mode)
{
    (void)Mode;   // golden also never reads Mode inside this method's body
    int ct = 0;
    for (int y = 1; y < stdGridReportID->RowCount; y++)
    {
        if (stdGridReportID->Cells[0][y] == iCeid)
        {
            // AI(W906-uHGemEquipment) 20260716: GOLDEN BUG, preserved
            // verbatim (uHGemEquipment.cpp:7587) -- same class of bug as
            // GetCEIDContent above, mirrored the OTHER direction: loop bound
            // is `stdGridReportID->RowCount` (257) where `->ColCount` (1026)
            // is clearly intended. Here the practical effect is the
            // OPPOSITE of GetCEIDContent's: this grid's RowCount(257) is
            // SMALLER than its ColCount(1026), so the loop can terminate
            // EARLY (at x==257) even though up to 1024 usable SVID slots
            // exist per report -- silently truncating a Report with more
            // than ~255 SVIDs to its first ~255. NOT fixed here.
            for (int x = 2; x < stdGridReportID->RowCount; x++)
            {
                if (stdGridReportID->Cells[x][y] == "")
                    break;
                iReportIDData[ct] = static_cast<unsigned>(atoi(stdGridReportID->Cells[x][y].c_str()));
                ct++;
            }
            return static_cast<unsigned>(ct);
        }
    }
    return 0;
}
//------------------------------------------------------------------------------
bool THGem::IsEnableEvent(unsigned iDataID, unsigned iCeid)
{
    (void)iDataID;   // golden signature keeps iDataID but never reads it in this method's body
    for (int y = 1; y < strGrdCEID->RowCount; y++)
    {
        if (strGrdCEID->Cells[0][y] == iCeid)
        {
            if (strGrdCEID->Cells[1][y] == "1")
                return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------
bool THGem::CheckCeidExist(AnsiString Ceid)
{
    for (int y = 1; y < strGrdCEID->RowCount; y++)
    {
        if (strGrdCEID->Cells[0][y] == Ceid)
            return true;
    }
    return false;
}
//------------------------------------------------------------------------------
int THGem::GetReportIDType(AnsiString ReportID)
{
    for (int y = 1; y < stdGridReportID->RowCount; y++)
    {
        if (stdGridReportID->Cells[0][y] == ReportID)
        {
            if (stdGridReportID->Cells[1][y] == "0")
                return 0;
            else if (stdGridReportID->Cells[1][y] == "1")
                return 1;
            else
                return 2;
        }
    }
    return 3;
}
//------------------------------------------------------------------------------
bool THGem::CheckCeidAlreadyDefine(AnsiString Ceid)
{
    AnsiString ReportID;
    for (int y = 1; y < strGrdCEID->RowCount; y++)
    {
        if (strGrdCEID->Cells[0][y] == Ceid)
        {
            for (int x = 3; x < strGrdCEID->ColCount; x++)   // 2014/01/01 lee -- add S1F23
            {
                ReportID = strGrdCEID->Cells[x][y];
                if (ReportID == "")
                    continue;
                if (GetReportIDType(ReportID) == 0)
                    return true;
            }
        }
    }
    return false;
}
//------------------------------------------------------------------------------
bool THGem::CheckReportIDExist(AnsiString ReportID)
{
    for (int y = 1; y < stdGridReportID->RowCount; y++)
    {
        if (stdGridReportID->Cells[0][y] == ReportID)
            return true;
    }
    return false;
}
//------------------------------------------------------------------------------
void THGem::AddCeidReportID(AnsiString Ceid, AnsiString ReportID)
{
    for (int y = 1; y < strGrdCEID->RowCount; y++)
    {
        if (strGrdCEID->Cells[0][y] == Ceid)
        {
            for (int x = 3; x < strGrdCEID->ColCount; x++)   // 2014/01/01 lee
            {
                // golden: `strGrdCEID->Cells[x][y]=="" || strGrdCEID->Cells[x][y]==NULL`
                // -- the `==NULL` half is dead/redundant here (a grid cell is
                // always a real AnsiString value, never a null pointer;
                // AnsiString==NULL resolves via the const-char*-overload,
                // which treats a null pointer as ""), preserved verbatim for
                // source fidelity rather than simplified away.
                if (strGrdCEID->Cells[x][y] == "" || strGrdCEID->Cells[x][y] == NULL)
                {
                    strGrdCEID->Cells[x][y] = ReportID;
                    return;
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
void THGem::DeleteAllHostReportIDOfCeid(unsigned ReportID)
{
    bool bSearch = true;
    TStringList *P;
    P = new TStringList;
    for (int y = 1; y < strGrdCEID->RowCount; y++)
    {
        bSearch = false;
        for (int x = 3; x < strGrdCEID->ColCount; x++)
        {
            if (strGrdCEID->Cells[x][y] == ReportID)
            {
                strGrdCEID->Cells[x][y] = "";
                bSearch = true;
            }
        }

        if (bSearch)
        {
            P->Clear();
            for (int x = 3; x < strGrdCEID->ColCount; x++)
            {
                if (strGrdCEID->Cells[x][y] != "")
                    P->Add(strGrdCEID->Cells[x][y]);
            }

            for (int x = 3; x < strGrdCEID->ColCount; x++)
                strGrdCEID->Cells[x][y] = "";

            for (int i = 0; i < P->Count; i++)
                strGrdCEID->Cells[3 + i][y] = P->Strings[i];
        }
    }
    delete P;
}
//------------------------------------------------------------------------------
void THGem::DeleteHostReportIDOfAssignCeid(AnsiString Ceid)
{
    AnsiString ReportID;
    for (int y = 1; y < strGrdCEID->RowCount; y++)
    {
        if (strGrdCEID->Cells[0][y] == Ceid)
        {
            for (int x = 3; x < strGrdCEID->ColCount; x++)
            {
                // golden reads ReportID here (uHGemEquipment.cpp:7945) then
                // immediately clears the cell without otherwise using the
                // local -- preserved verbatim (dead read, not removed).
                ReportID = strGrdCEID->Cells[x][y];
                (void)ReportID;
                strGrdCEID->Cells[x][y] = "";
            }
        }
    }
}
//------------------------------------------------------------------------------
void THGem::DeleteAllHostDefineCeid()
{
    for (int y = 1; y < strGrdCEID->RowCount; y++)
    {
        if (strGrdCEID->Cells[0][y] != "")
            DeleteHostReportIDOfAssignCeid(strGrdCEID->Cells[0][y]);
    }
}
//------------------------------------------------------------------------------
bool THGem::CheckReportAlreadyDefine(AnsiString ReportID)
{
    for (int y = 1; y < stdGridReportID->RowCount; y++)
        if (stdGridReportID->Cells[0][y] == ReportID)
            return true;
    return false;
}
//------------------------------------------------------------------------------
// golden uHGemEquipment.cpp:8034-8039 -- GATED STUB.
// Real body is:
//     InitLocalHead(2, 38, 0);
//     DataItemOut(1, HType.BINARY_TYPE, &ErrCode);
//     SendLocalData();
// i.e. sends an S2F38 (Enable/Disable Event Report Acknowledge) over the
// wire. InitLocalHead/DataItemOut/SendLocalData are THGem's SML wire-codec
// forwarding methods, explicitly deferred to a follow-on wave (this wave's
// scope is the StringGrid CEID/Report/Alarm family only -- see this file's
// own header note). No-op: does NOT send anything over clientGem/srvGem.
// Exists purely so EnableDisableEventReport's own (in-scope) control flow,
// which unconditionally calls this at its tail, remains translatable without
// silently dropping the call site.
void THGem::EnableDisableEventReportAcknowledgeError(unsigned char ErrCode)
{
    (void)ErrCode;
}
//------------------------------------------------------------------------------
void THGem::EnableDisableEventReport(bool CEED, int slen, unsigned *CEID)
{
    if (slen == 0)   // all
    {
        for (int y = 1; y < strGrdCEID->RowCount; y++)
        {
            if (strGrdCEID->Cells[0][y] != "")
            {
                if (CEED == true)
                    strGrdCEID->Cells[1][y] = "1";
                else
                    strGrdCEID->Cells[1][y] = "0";
            }
        }
    }
    else
    {
        for (int i = 0; i < slen; i++)
        {
            for (int y = 1; y < strGrdCEID->RowCount; y++)
            {
                if (strGrdCEID->Cells[0][y] == CEID[i])
                {
                    if (CEED == true)
                        strGrdCEID->Cells[1][y] = "1";
                    else
                        strGrdCEID->Cells[1][y] = "0";
                }
            }
        }
    }
    SaveEventReportData();
    EnableDisableEventReportAcknowledgeError(0x00);   // GATED, see stub above
}

//===========================================================================
//  Generic StringGrid <-> TStringList tab-format (de)serialization
//  (golden uHGemEquipment.cpp:8059-8195)
//===========================================================================
bool THGem::CheckCEIDExist(AnsiString sID)   // upper-case-CEID sibling (golden :8051)
{
    for (int y = 1; y < strGrdCEID->RowCount; y++)
        if (strGrdCEID->Cells[0][y] == sID)
            return true;
    return false;
}
//------------------------------------------------------------------------------
void THGem::CopyStringGridAsTabFormat(TStringGrid *strGrd, TStringList *memoPtr)
{
    int endx;
    AnsiString S;

    memoPtr->Clear();
    for (int y = 0; y < strGrd->RowCount; y++)
    {
        S = "";
        endx = 0;
        for (int x = (strGrd->ColCount - 1); x >= 0; x--)
        {
            if (strGrd->Cells[x][y] != "")
            {
                endx = x;
                break;
            }
        }
        // NOTE: even a fully-empty row (endx stays 0) still emits ONE
        // tab-terminated (empty) field below -- golden quirk, preserved
        // verbatim (confirmed against the real on-disk
        // D:\HT9045\SECS\SECS\SYSTEM\EventReport_CEID.def, whose 1024 unused
        // data rows are literally single-tab lines, not truly-empty lines).
        for (int x = 0; x <= endx; x++)
        {
            S += strGrd->Cells[x][y];
            S += "\t";
        }
        memoPtr->Add(S);
    }
}
//------------------------------------------------------------------------------
void THGem::PasteStringGridAsTabFormat(TStringGrid *strGrd, TStringList *memoPtr)
{
    int x, y, StartX, StartY;
    char str[8192], str2[256];
    bool flag;

    for (y = 0; y < strGrd->RowCount; y++)
        for (x = 0; x < strGrd->ColCount; x++)
            strGrd->Cells[x][y] = "";

    for (y = 0; y < memoPtr->Count; y++)
    {
        if (y >= strGrd->RowCount)
            break;
        // AI(W906-uHGemEquipment) 20260716: golden is `memoPtr->Strings[y].c_str()`;
        // vclcompat::TStringList's Strings[] proxy has no .c_str() (same
        // accommodation already established at mycylin.cpp's Strings[i]->
        // GetString(i) sites) -- GetString(y) returns the real AnsiString.
        strncpy(str, memoPtr->GetString(y).c_str(), sizeof(str));
        str[sizeof(str) - 1] = '\0';   // AI(W906-uHGemEquipment) 20260716: golden's bare
                                        // strncpy has no explicit terminator guard; added
                                        // defensively for a line >=8192 bytes (never hit by
                                        // this wave's real/test data, which are all far
                                        // shorter) -- does not change behavior for any
                                        // in-range input, only avoids reading past `str`'s
                                        // end in SplitStrByTabOnly on a pathological input.
        x = 0;
        do
        {
            flag = SplitStrByTabOnly(str, str2, 256);
            if (flag == false)
                break;
            StartX = x;
            StartY = y;

            strGrd->Cells[StartX][StartY] = str2;
            x++;
            if (x >= strGrd->ColCount)
                break;
        } while (1);
    }
}
//------------------------------------------------------------------------------
void THGem::ReadEventReportData()
{
    // AI(W906-uHGemEquipment) 20260716: golden hardcodes this absolute path
    // (uHGemEquipment.cpp:8202/8212) -- NOT derived from GemSystemPath,
    // unlike ReadAlamData/WriteAlamData above. Preserved verbatim: the real
    // production files exist at exactly this path on the reference dev
    // machine (D:\HT9045\SECS\SECS\SYSTEM\EventReport_CEID.def /
    // EventReport_ReportID.def) and this method only ever READS them
    // (LoadFromFile) -- never writes -- so calling it is safe/non-destructive.
    // SaveEventReportData below WRITES to this same path family and must
    // never be exercised against the real path (see that method's own note).
    TStringList *memoPtr;
    AnsiString Filename;

    memoPtr = new TStringList;
    Filename = "D:\\HT9045\\SECS\\SECS\\SYSTEM\\EventReport_CEID.def";
    if (FileExists(Filename) == false)
    {
        delete memoPtr;
        return;
    }

    memoPtr->LoadFromFile(Filename);
    PasteStringGridAsTabFormat(strGrdCEID, memoPtr);

    Filename = "D:\\HT9045\\SECS\\SECS\\SYSTEM\\EventReport_ReportID.def";
    if (FileExists(Filename) == false)
    {
        delete memoPtr;
        return;
    }

    memoPtr->LoadFromFile(Filename);
    PasteStringGridAsTabFormat(stdGridReportID, memoPtr);

    delete memoPtr;
}
//------------------------------------------------------------------------------
void THGem::SaveEventReportData()
{
    // AI(W906-uHGemEquipment) 20260716: DANGEROUS IF EXECUTED AS-IS ON THIS
    // DEV MACHINE -- golden hardcodes the SAME absolute path family that
    // ReadEventReportData reads from (D-colon HT9045 SECS SECS SYSTEM
    // EventReport_CEID.def / EventReport_ReportID.def, backslash-separated),
    // and those files
    // genuinely exist as PRODUCTION data at that path here. This method
    // WRITES (SaveToFile) to that literal path -- it must NEVER be called
    // by this wave's own test harness (would silently overwrite real
    // production SECS/GEM config, which is outside this migration's write
    // boundary: D:\HT9045 root, not the D:\HT9045\HT9011UC_Cpp_V3.33.906.0
    // working tree). Translated faithfully (hardcoded path preserved,
    // matching golden uHGemEquipment.cpp:8650-8665 exactly) but verified by
    // code inspection/citation only -- see tests/test_uHGemEquipment.cpp's
    // own header note for how CopyStringGridAsTabFormat (the part of this
    // method's logic that IS actually exercised) is tested instead.
    TStringList *memoPtr;
    AnsiString Filename;

    memoPtr = new TStringList;
    CopyStringGridAsTabFormat(strGrdCEID, memoPtr);
    Filename = "D:\\HT9045\\SECS\\SECS\\SYSTEM\\EventReport_CEID.def";
    memoPtr->SaveToFile(Filename);

    CopyStringGridAsTabFormat(stdGridReportID, memoPtr);
    Filename = "D:\\HT9045\\SECS\\SECS\\SYSTEM\\EventReport_ReportID.def";
    memoPtr->SaveToFile(Filename);

    delete memoPtr;
}

//---------------------------------------------------------------------------
//  SplitStrByTabOnly (golden uHGemEquipment.cpp:8086-8161)
//  File-scope helper, NOT a THGem method (matches golden exactly -- see
//  uHGemEquipment.h's own note on why this is declared there for testability
//  even though golden's header never declares it).
//
//  Parses ONE tab-delimited field out of `str` (in place, shifting the
//  consumed portion off the front) into `dest`, tolerating a leading tab,
//  CR, and NUL/CR/LF terminators.
//
//  BEHAVIOR (verified by hand-trace, e.g. against "a\t\tb" -> "a", "", "b"):
//  if `str` STARTS with a tab, that represents an EMPTY field at the current
//  position (either the very first field of a line beginning with a tab, or
//  -- more commonly -- the trailing tab an earlier call already left behind
//  after consuming the previous field, re-entering this SAME top branch on
//  the next call when two delimiters are adjacent). This branch writes
//  `dest=""` (the empty field), shifts `str` left by one to drop the
//  consumed tab, and returns true. The non-tab (`else`) branch below extracts
//  one field into `dest`, then -- as its OWN last step -- performs the same
//  kind of one-tab left-shift on whatever follows, so a normal (non-empty)
//  field is fully consumed, INCLUDING its trailing delimiter, within a
//  single call (the next call's `str` starts clean, not with a leftover
//  tab) -- confirmed by hand-trace against "hello\tworld".
//---------------------------------------------------------------------------
bool SplitStrByTabOnly(char *str, char *dest, int Max)
{
    char Buffer[10240];
    int ct1 = 0, ct2 = 0;

    if (str[ct1] == '\t')
    {
        strcpy(dest, "");
        for (;;)
        {
            Buffer[ct1] = str[ct1 + 1];
            if (Buffer[ct1] == 0)
            {
                strcpy(str, Buffer);
                return true;
            }
            ct1++;
        }
    }
    else
    {
        while (1)   // find first character
        {
            if (str[ct1] == '\x0' || str[ct1] == '\r' || str[ct1] == '\n')
                return false;
            if ((str[ct1] != '\t' && str[ct1] != '\r'))
                break;
            ct1++;
        }
        while (1)
        {
            dest[ct2] = str[ct1];
            ct2++;
            ct1++;
            dest[ct2] = '\x0';
            if ((ct2 + 1) >= Max)
                break;

            if (str[ct1] != '\t' && str[ct1] != '\0' && str[ct1] != '\r')
            {
            }
            else
            {
                break;
            }
        }
        ct2 = 0;
        while (1)
        {
            Buffer[ct2] = str[ct1];
            if (str[ct1] == '\x0')
                break;
            ct1++;
            ct2++;
            Buffer[ct2] = '\x0';
            if (ct2 >= 10240)
                break;
        }
        strcpy(str, Buffer);
        ct1 = 0;
        if (str[0] == '\t')
        {
            for (;;)
            {
                Buffer[ct1] = str[ct1 + 1];
                if (Buffer[ct1] == 0)
                {
                    strcpy(str, Buffer);
                    return true;
                }
                ct1++;
            }
        }
    }
    return true;
}
