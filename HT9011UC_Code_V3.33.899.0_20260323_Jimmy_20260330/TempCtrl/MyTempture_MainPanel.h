//---------------------------------------------------------------------------

#ifndef MyTempture_MainPanelH
#define MyTempture_MainPanelH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class PACKAGE TMyTempMainPanel : public TComponent
{
private:    // User declarations
    int iTag;
    bool bEnable;
    double dUpperBound;                                     //上極限
    double dLowerBound;                                     //下極限
    bool bChangeColor;                                      //顏色變化
    bool bAlarm;                                            //溫度過高或過低
    TPanel *plTempValue;                                    //溫度數值
public:     // User declarations
    __fastcall TMyTempMainPanel::TMyTempMainPanel(TComponent* Owner);
    __fastcall TMyTempMainPanel::~TMyTempMainPanel();
    TPanel *plTempName;                                     //顯示名稱
    TPanel *plTempMain;                                     //底層畫框, 所有位置與大小控制都由這個決定
    void SetSize(int iWidth, int iHeight);                  //改變大小
    void SetPosition(int iTop, int iLeft);                  //改變位置
    void SetVisable(bool bVisable);                         //顯示與否
    void SetPName(AnsiString Name);                         //設定名稱
    void SetValue(double Value, bool bMonitor=true);        //設定數字
    void SetFontSize(int iSize);                            //設定字型大小
    __property int    Tag={read=iTag, write=iTag, default=0};
    __property double UpperBound={read=dUpperBound, write=dUpperBound, default=0};
    __property double LowerBound={read=dLowerBound, write=dLowerBound, default=0};
    __property bool   HasAlarm={read=bAlarm, default=false};
};

#endif
