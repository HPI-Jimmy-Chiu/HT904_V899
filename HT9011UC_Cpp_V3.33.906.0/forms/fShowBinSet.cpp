// =============================================================================
//  forms/fShowBinSet.cpp  --  definitions for the fShowBinSet facade
//
//  AI(W906-FW3-HSP1) 20260828: new file, FW wave FW3-HSP1. See forms/
//  fShowBinSet.h for the full GATE REGISTER / ACTIVE evidence / DEVIATIONS /
//  CTOR NOTE (array-index-compatibility proof).
// =============================================================================
#include "forms/fShowBinSet.h"

// AI(W906-FW3-HSP1) 20260828: TfShowBinSet/fShowBinSet were FREE tree-wide --
// same idiom as forms/fCleaning.cpp:45. Ctor touches ONLY its own fields and
// `new`s its own widget stand-ins in a loop (see CTOR NOTE, forms/
// fShowBinSet.h) -- no other global is dereferenced, so this static-init
// `new` carries no SIOF risk (docs/KNOWLEDGE.md "static-init ctor 不可碰
// NULL 全域").
TfShowBinSet *fShowBinSet = new TfShowBinSet();

// golden cShowBinSet.cpp :18-20,38 -- FILE-SCOPE globals, matching golden's
// own linkage (not declared in golden's header either) -- see DEVIATION D-5.
static TPanel *TestSiteCH_Display[MAX_SOCKET_ROW][MAX_SOCKET_COL];
static TLabel *TestLabCol_Display[MAX_SOCKET_COL];
static TLabel *TestLabRow_Display[MAX_SOCKET_ROW];
static int     iReturnValue = 0;

TfShowBinSet::TfShowBinSet()   // golden :40-101, CTOR NOTE (forms/fShowBinSet.h)
{
    // golden :43-75 -- alias 32 palAa..palDh / 8 labColA..H / 4 labRowA..D
    // `.dfm` widgets into TestSiteCH_Display[][]/TestLabCol_Display[]/
    // TestLabRow_Display[]. This port constructs the arrays directly (one
    // heap object per slot) instead of declaring ~44 individually-named
    // facade members none of golden's OWN code ever reads outside this ctor
    // (verified this wave, see CTOR NOTE) -- behaviour-preserving end state
    // (each slot IS a live, independently mutable object, ->Visible=false
    // exactly as golden sets), only the SOURCE of each object differs
    // (fresh `new` vs. aliasing an existing `.dfm` widget -- see BEHAVIOUR
    // DELTA in the header banner).
    for (int i = 0; i < MAX_SOCKET_TOTAL; i++)
    {
        int iRow = i / MAX_SOCKET_COL;
        int iCol = i % MAX_SOCKET_COL;

        if (i < MAX_SOCKET_ROW)
        {
            TestLabRow_Display[i] = new TLabel();
            TestLabRow_Display[i]->Visible = false;
        }

        if (i < MAX_SOCKET_COL)
        {
            TestLabCol_Display[i] = new TLabel();
            TestLabCol_Display[i]->Visible = false;
        }

        TestSiteCH_Display[iRow][iCol] = new TPanel();
        TestSiteCH_Display[iRow][iCol]->Visible = false;
    }
    fShow = false;

    // golden :78-100 -- alias 33 laAuto1..laMag14 / 33 labAuto1..labMag14
    // `.dfm` widgets into MyBinSel[]/MyBinSelLab[], e6TrayName-consecutive
    // order (see CTOR NOTE's INDEX COMPATIBILITY proof). Same "construct
    // directly" deviation as the loop above.
    for (int i = 0; i < eTrayCount; i++)
    {
        MyBinSel[i]    = new TLabel();
        MyBinSelLab[i] = new TLabel();
    }
}

// ---------------------------------------------------------------------------
void TfShowBinSet::FormClose()   // golden :201-205
{
    fShow = false;
}

// ---------------------------------------------------------------------------
void TfShowBinSet::Panel3Click(TObject *Sender)   // golden :207-211
{
    (void)Sender;
    iReturnValue = 1;
    Close();   // DEVIATION D-3 -- port-only no-op
}

// ---------------------------------------------------------------------------
void TfShowBinSet::Panel4Click(TObject *Sender)   // golden :213-217
{
    (void)Sender;
    iReturnValue = 2;
    Close();
}

// ---------------------------------------------------------------------------
int ShowSiteMapping_YES_NO()   // golden cShowBinSet.cpp :219-224 (file-scope)
{
    iReturnValue = 0;
    fShowBinSet->ShowModal();   // DEVIATION D-4 -- port-only no-op
    return iReturnValue;
}

// -- GATE REGISTER -- see forms/fShowBinSet.h for the full reasoning.
// FormShow / Timer1Timer are declared in forms/fShowBinSet.h and
// intentionally have NO definition here -- calling either is a LINK ERROR
// by design (same idiom as forms/fMesSystem.cpp's GATED bucket).
