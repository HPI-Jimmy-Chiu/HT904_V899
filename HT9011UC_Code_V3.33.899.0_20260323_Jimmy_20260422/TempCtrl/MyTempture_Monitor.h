//---------------------------------------------------------------------------

#ifndef MyTempture_MonitorH
#define MyTempture_MonitorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class PACKAGE TMyTempMonitor : public TComponent
{
private:    // User declarations
    int iTag;
    bool bEnable;
    TEdit *edTempValue;                                     //溫度數值
    TProgressBar *pbTemp;
    TListBox *lbTempLevel;
public:     // User declarations
    __fastcall TMyTempMonitor::TMyTempMonitor(TComponent* Owner);
    __fastcall TMyTempMonitor::~TMyTempMonitor();
    TGroupBox *gbTempGroup;                                 //整個溫度監控群組
    TCheckBox *cbEnable;                                    //溫度監控開啟或關閉
    TEdit *edTempRange;                                     //溫度的監控範圍
    void SetPosition(int iTop, int iLeft);                  //改變位置
    void SetVisable(bool bVisable);                         //顯示與否
    void SetPName(AnsiString Name);                         //設定名稱
    void SetValue(double Value);                            //設定數字
    void SetLevel(int Max, int Min);                        //設定範圍

    __property int Tag={read=iTag, write=iTag, default=0};
};
//---------------------------------------------------------------------------
#endif
