//---------------------------------------------------------------------------

#ifndef MyTempture_OffsetH
#define MyTempture_OffsetH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class PACKAGE TMyTempOffset : public TComponent
{
private:    // User declarations
    int iTag;
    TPanel *pMyPanel;
    bool bEnable;
public:     // User declarations
    __fastcall TMyTempOffset::TMyTempOffset(TComponent* Owner);
    __fastcall TMyTempOffset::~TMyTempOffset();
    TGroupBox *gbTempOffsetGroup;
    TEdit *edTempLowBase;
    TEdit *edTempMiddBase;
    TEdit *edTempHighBase;
    TEdit *edTempUserBase;

    void SetPosition(int iTop, int iLeft);                  //改變位置
    void SetVisable(bool bVisable);                         //顯示與否
    void SetPName(AnsiString Name);                         //設定名稱

    __property int Tag={read=iTag, write=iTag, default=0};
};
//---------------------------------------------------------------------------
#endif
