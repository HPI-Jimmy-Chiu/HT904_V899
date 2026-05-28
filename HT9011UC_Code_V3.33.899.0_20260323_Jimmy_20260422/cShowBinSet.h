//---------------------------------------------------------------------------

#ifndef cShowBinSetH
#define cShowBinSetH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include "HTray.h"
//---------------------------------------------------------------------------
class TfShowBinSet : public TForm
{
__published:    // IDE-managed Components
        TTimer *Timer1;
    TPageControl *PageControl1;
    TTabSheet *tsBinSettingCheck;

    TPanel *pal1;
    TLabel *lab4;
    TScrollBox *ScrollBox1;
    TLabel *labBinBox;
    TTabSheet *tsSiteMappingCheck;
    TPanel *palSiteMap;
    TPanel *Panel2;
    TLabel *Label1;
    TPanel *Panel4;
    TPanel *Panel3;
    TGroupBox *GroupBox4;
    TLabel *labRowA;
    TLabel *labRowB;
    TLabel *labColA;
    TLabel *labColB;
    TLabel *labColC;
    TLabel *labColD;
    TLabel *labColE;
    TLabel *labColF;
    TLabel *labColG;
    TLabel *labColH;
    TLabel *labRowC;
    TLabel *labRowD;
    TPanel *palAa;
    TPanel *palAb;
    TPanel *palAc;
    TPanel *palAd;
    TPanel *palAg;
    TPanel *palAh;
    TPanel *palAf;
    TPanel *palAe;
    TPanel *palBa;
    TPanel *palBb;
    TPanel *palBc;
    TPanel *palBd;
    TPanel *palBg;
    TPanel *palBh;
    TPanel *palBf;
    TPanel *palBe;
    TPanel *palCa;
    TPanel *palCb;
    TPanel *palCc;
    TPanel *palCd;
    TPanel *palCg;
    TPanel *palCh;
    TPanel *palCf;
    TPanel *palCe;
    TPanel *palDa;
    TPanel *palDb;
    TPanel *palDc;
    TPanel *palDd;
    TPanel *palDg;
    TPanel *palDh;
    TPanel *palDf;
    TPanel *palDe;
    TPanel *pnlShowBin;
    TLabel *laAuto1;
    TLabel *laAuto2;
    TLabel *laAuto3;
    TLabel *laBinBox;
    TLabel *laMag1;
    TLabel *laMag2;
    TLabel *laMag3;
    TLabel *laMag4;
    TLabel *laMag5;
    TLabel *laMag6;
    TLabel *laMag7;
    TLabel *laMag8;
    TLabel *laMag9;
    TLabel *laMag10;
    TLabel *laMag11;
    TLabel *laMag12;
    TLabel *laMag13;
    TLabel *laMag14;
    TLabel *laAuto5;
    TLabel *laAuto6;
    TLabel *laFix4;
    TLabel *laFix5;
    TLabel *laFix6;
    TLabel *laAuto4;
    TLabel *laFix1;
    TLabel *laFix2;
    TLabel *laFix3;
    TLabel *laFix7;
    TLabel *laFix8;
    TLabel *laFix9;
    TLabel *laFix10;
    TLabel *laFix11;
    TLabel *laFix12;
    TLabel *labAuto1;
    TLabel *labAuto2;
    TLabel *labAuto3;
    TLabel *labFix1;
    TLabel *labFix2;
    TLabel *labFix3;
    TLabel *labFix4;
    TLabel *labFix5;
    TLabel *labFix6;
    TLabel *Label2;
    TLabel *labMag1;
    TLabel *labMag2;
    TLabel *labMag3;
    TLabel *labMag4;
    TLabel *labMag5;
    TLabel *labMag6;
    TLabel *labMag7;
    TLabel *labMag8;
    TLabel *labMag9;
    TLabel *labMag10;
    TLabel *labMag11;
    TLabel *labMag12;
    TLabel *labMag13;
    TLabel *labMag14;
    TLabel *labAuto4;
    TLabel *labAuto5;
    TLabel *labAuto6;
    TLabel *labFix7;
    TLabel *labFix8;
    TLabel *labFix9;
    TLabel *labFix10;
    TLabel *labFix11;
    TLabel *labFix12;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall Panel3Click(TObject *Sender);
    void __fastcall Panel4Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall Timer1Timer(TObject *Sender);
private:    // User declarations
public:     // User declarations
    __fastcall TfShowBinSet(TComponent* Owner);
    bool fShow;
    TLabel *MyBinSel[eTrayCount];           //kevin 20160819 add Bult box //kevin 20140305
    TLabel *MyBinSelLab[eTrayCount];        //kevin 20160819 add Bult box //kevin 20140305

};
//---------------------------------------------------------------------------
extern PACKAGE TfShowBinSet *fShowBinSet;
int  ShowSiteMapping_YES_NO();
//---------------------------------------------------------------------------
#endif
