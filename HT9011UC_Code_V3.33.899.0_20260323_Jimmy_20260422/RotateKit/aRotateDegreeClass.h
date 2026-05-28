//---------------------------------------------------------------------------

#ifndef aRotateDegreeClassH
#define aRotateDegreeClassH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TfrmRot : public TForm
{
__published:    // IDE-managed Components
    TPanel *pnlRotDegree;
    TRadioGroup *rgRotateDegree;
    TGroupBox *grpRotDegree;
    TImage *imgRotDegree;
    TLabel *lblSiteMap;
    void __fastcall imgRotDegreeClick(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
private:    // User declarations
public:     // User declarations
    __fastcall TfrmRot(TComponent* Owner);
    void SetCaption(AnsiString caption);
    void SetDegree(int iDegree);
    void ReloadPicture();

    void SetTag(bool bIn, int iY, int iX);
    bool bInput;
    int iXPos;
    int iYPos;
    int iSiteMap;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmRot *frmRot;
extern PACKAGE TfrmRot *FrmRotDegree[2][4][8];
//---------------------------------------------------------------------------
#endif
