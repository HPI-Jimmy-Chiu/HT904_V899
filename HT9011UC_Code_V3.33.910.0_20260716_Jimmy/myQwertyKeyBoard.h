//---------------------------------------------------------------------------

#ifndef myQwertyKeyBoardH
#define myQwertyKeyBoardH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <vector>

using namespace std;
//---------------------------------------------------------------------------
enum eKeyType
{
    eKeyNumAndSymbol=0,
    eKeyNumOnly     =1,
    eKeySymbol      =2,
    eKeyalphabet    =3
};
//---------------------------------------------------------------------------
class TQwertyKeyClass
{
    public :
        TQwertyKeyClass(AnsiString sUpper, AnsiString sLower, TSpeedButton *SPB, eKeyType Type);
        AnsiString UpperCase;
        AnsiString LowerCase;
        TSpeedButton *spbKey;
        eKeyType     KeyType;
};
//---------------------------------------------------------------------------
class TfQwertyKey : public TForm
{
__published:    // IDE-managed Components
    TPanel *palContent;
    TEdit *edQwertyContent;
    TSpeedButton *SpeedButton63;
    TSpeedButton *SpeedButton64;
    TSpeedButton *SpeedButton65;
    TSpeedButton *SpeedButton66;
    TPanel *palNumKey;
    TSpeedButton *speedbNo17;
    TSpeedButton *speedbNo18;
    TSpeedButton *speedbNo19;
    TSpeedButton *speedbNo14;
    TSpeedButton *speedbNo15;
    TSpeedButton *speedbNo16;
    TSpeedButton *speedbNo11;
    TSpeedButton *speedbNo12;
    TSpeedButton *speedbNo13;
    TSpeedButton *speedbNo10;
    TSpeedButton *spbDP;
    TSpeedButton *spbMinus;
    TSpeedButton *spbCancel2;
    TSpeedButton *spbSummit2;
    TSpeedButton *spbClear2;
    TSpeedButton *spbBackSpace2;
    TSpeedButton *spbPercent;
    TSpeedButton *spbAdd1;
    TSpeedButton *spbMinus1;
    TSpeedButton *spbAdd10;
    TSpeedButton *spbMinus10;
    TSpeedButton *spbAdd100;
    TSpeedButton *spbMinus100;
    TPanel *palQwertyKey;
    TSpeedButton *speedbSyb3;
    TSpeedButton *speedbNo01;
    TSpeedButton *speedbNo02;
    TSpeedButton *speedbNo03;
    TSpeedButton *speedbNo04;
    TSpeedButton *speedbNo05;
    TSpeedButton *speedbNo06;
    TSpeedButton *speedbNo07;
    TSpeedButton *speedbNo08;
    TSpeedButton *speedbNo09;
    TSpeedButton *speedbNo00;
    TSpeedButton *speedbSyb_;
    TSpeedButton *speedbSyPl;
    TSpeedButton *speedbChrQ;
    TSpeedButton *speedbChrW;
    TSpeedButton *speedbChrE;
    TSpeedButton *speedbChrR;
    TSpeedButton *speedbChrT;
    TSpeedButton *speedbChrY;
    TSpeedButton *speedbChrU;
    TSpeedButton *speedbChrI;
    TSpeedButton *speedbChrO;
    TSpeedButton *speedbChrP;
    TSpeedButton *speedbSybL;
    TSpeedButton *speedbSybR;
    TSpeedButton *speedbSyOr;
    TSpeedButton *speedbChrA;
    TSpeedButton *speedbChrS;
    TSpeedButton *speedbChrD;
    TSpeedButton *speedbChrF;
    TSpeedButton *speedbChrG;
    TSpeedButton *speedbChrH;
    TSpeedButton *speedbChrJ;
    TSpeedButton *speedbChrK;
    TSpeedButton *speedbChrL;
    TSpeedButton *speedbSyb1;
    TSpeedButton *speedbSyb2;
    TSpeedButton *speedbChrZ;
    TSpeedButton *speedbChrX;
    TSpeedButton *speedbChrC;
    TSpeedButton *speedbChrV;
    TSpeedButton *speedbChrB;
    TSpeedButton *speedbChrN;
    TSpeedButton *speedbChrM;
    TSpeedButton *speedbSyb4;
    TSpeedButton *speedbSyb5;
    TSpeedButton *speedbSyb6;
    TSpeedButton *speedbSpac;
    TSpeedButton *spbChangeCase;
    TSpeedButton *spbBackSpace;
    TSpeedButton *spbSummit;
    TSpeedButton *spbClear;
    TSpeedButton *spbCancel;
    TPanel *palValueLimit;
    TEdit *edCurrValue;
    TLabel *Label1;
    TLabel *labMaxValue;
    TEdit *edMaxValue;
    TLabel *labMinValue;
    TEdit *edMinValue;
    void __fastcall spbChangeCaseClick(TObject *Sender);
    void __fastcall spbBackSpaceClick(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall spbSummitClick(TObject *Sender);
    void __fastcall spbClearClick(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall spbCancelClick(TObject *Sender);
    void __fastcall spbPercentClick(TObject *Sender);
    void __fastcall spbMinusClick(TObject *Sender);
    void __fastcall spbAdd1Click(TObject *Sender);
    void __fastcall spbDPClick(TObject *Sender);
    void __fastcall edQwertyContentKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall edQwertyContentKeyPress(TObject *Sender, char &Key);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:    // User declarations
    void __fastcall spbKeyClick(TObject *Sender);
    void __fastcall ChangeDecimalPoint();
    bool bIntegerOnly;
    int  KeyCode;
    AnsiString sBackup;
public:     // User declarations
    __fastcall TfQwertyKey(TComponent* Owner);
    vector<TQwertyKeyClass *> QwertyKey;
    void __fastcall ShowQwertyKey(TWinControl *Ptr, int iFunction, int iDP=0, bool bCheckRange=false, double min=0, double max=0);
    bool bUpperCase;
    bool bNoSymbol;
    int  iDecimalPoint;
    bool bShow;
    bool bKey2;                                                                 //Steven 20150505 : 剛好在輸入時,Alarm會卡死
};
//---------------------------------------------------------------------------
extern PACKAGE TfQwertyKey *fQwertyKey;
extern PACKAGE TfQwertyKey *fQwertyKey2;                                        //Steven 20150505 : 剛好在輸入時,Alarm會卡死
//---------------------------------------------------------------------------
#endif
