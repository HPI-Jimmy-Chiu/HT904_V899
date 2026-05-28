//---------------------------------------------------------------------------

#ifndef MyOmronPanelH
#define MyOmronPanelH

#include <Classes.hpp>
using namespace std;

//---------------------------------------------------------------------------
class PACKAGE TMyOmronPanel : public TComponent
{
    __published:
        void __fastcall setEditValueClick(TObject *Sender);
    private:    // User declarations
        void __fastcall GroupBox1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall GroupBox1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall GroupBox1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
        double dPv;//JimmyChiu 20210923 : Index使用DTME08版16組加熱器
        double dSettingSV;      //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
        double dSV;             //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
    public:
        __fastcall TMyOmronPanel(TComponent* Owner);
        void __fastcall SetBG        (TColor OutLine, TColor BackGround); //設定背景
        void __fastcall SetPV        (AnsiString PV, TColor Color);       //顯示量測值
        void __fastcall SetDegree    (AnsiString Degree, TColor Color);   //顯示量測單位
        void __fastcall SetRunStop   (AnsiString RunStop, TColor Color);  //顯示目前運作模式
        void __fastcall SetAT        (AnsiString AT, TColor Color);       //顯示AT是否啟動
        void __fastcall SetInputErr  (AnsiString InputErr, TColor Color); //感溫線異常
        void __fastcall SetEvent     (AnsiString Event, TColor Color);    //警報事件
        void __fastcall SetSP        (AnsiString SP, TColor Color);       //目前設定值
        __fastcall ~TMyOmronPanel();
        TGroupBox *GroupBox;
        TImage    *ImgOmronPanel;
        TEdit     *edSV;        //設定值
        TCheckBox *cbEnableAT;
        int       iStartX;
        int       iStartY;
        int       iPort;
        double    fWeight;          //量測值
        double    fWeightDev;       //差異值
        void      SetVisible(bool bSW);  //Jimmychiu 20221123 add Groupbox visible item
        void      SetSettingSV(double dsv);             //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
        double    GetSettingSV(){return dSettingSV;}    //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
        void      SetSV(double dsv);                    //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
        double    GetSV(){return dSV;}                  //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
        int iMyTag;
        double GetPV(){return dPv;}
        void SetAT(bool bOn){cbEnableAT->Checked=bOn;}
};
#endif
