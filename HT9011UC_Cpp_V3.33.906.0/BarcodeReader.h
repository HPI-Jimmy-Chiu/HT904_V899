//---------------------------------------------------------------------------
//  BarcodeReader.h -- faithful headless port of golden BarcodeReader.h
//  (57 lines) + the facade conventions this tree already froze
//  (forms/fQwertyKey.h / forms/fPassword.h precedents).
//
//  AI(W906-FW-BARCODE1) 20260824: WHY THIS WAVE -- Barcode_Reader() is the
//  LAST shared blocker behind every remaining gated keyboard/barcode site
//  (uTemp_Set G-Barcode, fSetup E-B1/E-B2, fLotInfo WB-9/WB-10, MyTempPanel
//  W8-5, HTEdit GATE (6-B)). This wave lands the ENTITY only; opening those
//  gates is the next wave's separate commit (behaviour-change separation).
//
//  DEVIATIONS (all precedented):
//   (D-1) TFormBarcodeReader : public TForm  ->  plain class (fPassword.h
//         posture). ShowModal() is an instant-submit offline no-op and
//         Close() only drops bShow -- the FormShow/FormClose handler pair is
//         translated as callable methods but NOT chained from Show/Close
//         (fQwertyKey.h BEHAVIOUR NOTE precedent): headless, InputBarcode-
//         Number() therefore returns the ctor-empty sBarcodeInfo, i.e. the
//         faithful "user typed nothing" outcome.
//   (D-2) golden ctor body -> explicit Init() is NOT needed here: the ctor
//         only zeroes own fields (iKeyType/sBarcodeInfo/bShow), no widget
//         derefs -- kept as a real ctor (ZeroInitVclFields semantics).
//   (D-3) pmBarcode/miClear/miClose (TPopupMenu/TMenuItem) OMITTED: no
//         translated method derefs the menu POINTERS themselves (only the
//         two mi*Click handlers exist, and they touch edtBarcodeNumber/
//         Close() only) -- fPassword.h "EXCLUDED WIDGETS / unused surface
//         is how facades rot" rule.
//   (D-4) KeyDown/KeyUp keep (unsigned short &Key) -- golden WORD &Key; the
//         TShiftState param has no port and is unread (trimmed, established
//         rule). TimerKeyInTimer/mi*Click/btnEnterClick/FormShow/FormClose
//         drop their unread TObject* Sender likewise.
//   (D-5) TTimer (VCL) has no vclcompat port -- TU-local stand-in class
//         TfBarcodeTimer{bool Enabled;} follows the established TU-local
//         TTimer stand-in pattern (ATC/ATCInterface.h:189,
//         BinDisplay/MyBinDisp.h:231, Automation/uRENESAS_Server.h:168 --
//         each header-local, never shared, so no ODR collision).
//   (D-6) edtBarcodeNumber is a TfBarcodeEdit subclass adding the one
//         member golden writes that vclcompat::TEdit lacks: PasswordChar
//         (fQwertyKey.h TfQwertyKeyEdit precedent).
//
//  GATE REGISTER (narrow, inside BarcodeReader.cpp):
//   (B-W1) RegisterHotKey/UnregisterHotKey(Handle,...) -- real WinAPI on a
//          real HWND; no Handle exists headless. 2 lines gated.
//   (B-F1) fFTPClient->bShow -- TfFTPClient has NO port anywhere in this
//          tree (grep "class TfFTPClient" 20260824, 0 hits). 2 sites: the
//          FormShow bFTPUseBarcodeReader branch and TimerKeyInTimer's
//          CC_CYUEAN test. Gated with the branch's non-FTP shape preserved.
//---------------------------------------------------------------------------
#ifndef BarcodeReaderH
#define BarcodeReaderH
//---------------------------------------------------------------------------
#include "vclcompat/vcl_compat.h"   // AnsiString / TObject at global scope
#include "vclcompat/Controls.h"     // TPanel/TEdit/TButton/TLabel (fQwertyKey.h include pattern)
#include "myTimer.h"                // TQPF_Timer (golden include kept)

#define KEY_NONE        0
#define KEY_ENTER       1
#define KEY_CANCEL      2

//---------------------------------------------------------------------------
// (D-5) TU-local VCL TTimer stand-in -- golden only touches Enabled here.
class TfBarcodeTimer
{
public:
    bool Enabled = false;
};
//---------------------------------------------------------------------------
// (D-6) golden TEdit + the PasswordChar member golden writes (:130/:133).
class TfBarcodeEdit : public vclcompat::TEdit
{
public:
    char PasswordChar = 0;          // golden TCustomEdit::PasswordChar ('*' while Password input)
};
//---------------------------------------------------------------------------
class TFormBarcodeReader            // golden: public TForm -- (D-1)
{
public:
    // -- __published widget set (golden h:22-29), menu trio omitted (D-3) --
    vclcompat::TPanel *pnlBarcodeReader = new vclcompat::TPanel();
    TfBarcodeEdit     *edtBarcodeNumber = new TfBarcodeEdit();
    vclcompat::TButton*btnEnter         = new vclcompat::TButton();
    vclcompat::TLabel *lblInputType     = new vclcompat::TLabel();
    TfBarcodeTimer    *TimerKeyIn       = new TfBarcodeTimer();

    // -- handlers (golden h:30-39; signatures per (D-4)) ---------------------
    void btnEnterClick();                        // golden :32-44
    void FormClose();                            // golden :93-120
    void FormShow();                             // golden :46-91
    void edtBarcodeNumberKeyDown(unsigned short &Key);  // golden :147-157
    void edtBarcodeNumberKeyUp(unsigned short &Key);    // golden :160-180
    void TimerKeyInTimer();                      // golden :182-402
    void miClearClick();                         // golden :404-408
    void miCloseClick();                         // golden :410-413

    // -- private: (golden h:40-41, collapsed public per D-6 of fQwertyKey) --
    TQPF_Timer HDelayTime;                       //RogerYang 20170407 (Steven) KeyUp計時

    // -- public: user declarations (golden h:42-48) --------------------------
    TFormBarcodeReader();                        // fields only, see (D-2)
    AnsiString sBarcodeInfo;
    int iKeyType;
    bool bShow;                                  //Steven 20140508
    AnsiString _sInputType;

    // -- form-level surface golden's bodies touch ----------------------------
    int Left = 0, Top = 0, Width = 0, Height = 0;   // FormShow centring writes/reads
    void ShowModal() {}                          // (D-1) instant-submit offline no-op
    void Close()     { bShow = false; }          // (D-1) window-less close
    void BringToFront() {}                       // TimerKeyInTimer :196 -- offline no-op
};
int Barcode_Reader(int count);    // 20140103 wei KYEC Barcode Reader
//---------------------------------------------------------------------------
extern TFormBarcodeReader *FormBarcodeReader;   // golden: extern PACKAGE ...
//---------------------------------------------------------------------------

AnsiString InputBarcodeNumber(AnsiString sLabelCaption, AnsiString sInputType="");

#endif
