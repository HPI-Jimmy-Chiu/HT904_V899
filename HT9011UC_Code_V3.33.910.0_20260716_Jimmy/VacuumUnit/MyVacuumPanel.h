//---------------------------------------------------------------------------
#include "MachineDefine.h"
#ifndef MyVacuumPanelH
#define MyVacuumPanelH

#include <Classes.hpp>
#include "BtnPanelLane.h"

using namespace std;

//---------------------------------------------------------------------------
class PACKAGE TMyVacuumPanel : public TComponent
{
    __published:
          __property AnsiString SuckerName ={read=sSuckerName,write=sSuckerName,default=NULL};
          //__property AnsiString   Path                ={read=HTPath,                  write=SetPath,                  default=NULL};
          __property int OnRing     ={read=iOnRing,write=iOnRing,default=0};
          __property int OnIP       ={read=iOnIP,write=iOnIP,default=0};
          __property int OnPort     ={read=iOnPort,write=iOnPort,default=0};
          __property int OnVCNo     ={read=iOnVCNo,write=iOnVCNo,default=0};
          __property int OnType     ={read=iOnType,write=iOnType,default=0};
          __property int OnISABase  ={read=iOnISABase,write=iOnISABase,default=ePCI1203};

          __property int OffRing    ={read=iOffRing,write=iOffRing,default=0};
          __property int OffIP      ={read=iOffIP,write=iOffIP,default=0};
          __property int OffPort    ={read=iOffPort,write=iOffPort,default=0};
          __property int OffVCNo    ={read=iOffVCNo,write=iOffVCNo,default=0};
          __property int OffType    ={read=iOffType,write=iOffType,default=0};
          __property int OffISABase ={read=iOffISABase,write=iOffISABase,default=ePCI1203};

          __property int SenRing    ={read=iSenRing,write=iSenRing,default=0};
          __property int SenIP      ={read=iSenIP,write=iSenIP,default=0};
          __property int SenPort    ={read=iSenPort,write=iSenPort,default=0};
          __property int SenVCNo    ={read=iSenVCNo,write=iSenVCNo,default=0};
          __property int SenType    ={read=iSenType,write=iSenType,default=0};
          __property int SenISABase ={read=iSenISABase,write=iSenISABase,default=ePCI1203};

    private:    // User declarations
        void __fastcall TMyVacuumPanel::edSVClick(TObject *Sender);
        void __fastcall TMyVacuumPanel::btnSVClick(TObject *Sender);
        void __fastcall TMyVacuumPanel::btnVaccumOnOffOnClick(TObject *Sender);
        int iKind;      //0:IndexArm1 ; 1:IndexArm2 ; 2:InArm ; 3:OutArm
        int iCol;
        int iRow;
        int iVacuOnOffSwap;
        double dCurrectVal;
        double dOldCurrectVal;
        double dThresholdVal;
        double dWriteThresholdVal;
        bool bNeedReadVaccumThreshold;

        AnsiString sSuckerName;

        int iOnRing;
        int iOnIP;
        int iOnPort;
        int iOnVCNo;
        int iOnType;
        int iOnISABase;

        int iOffRing;
        int iOffIP;
        int iOffPort;
        int iOffVCNo;
        int iOffType;
        int iOffISABase;

        int iSenRing;
        int iSenIP;
        int iSenPort;
        int iSenVCNo;
        int iSenType;
        int iSenISABase;

        unsigned short DataSize;                //固定值 128

        unsigned short IO_Index;
        unsigned short IO_SubIndex;

        unsigned short CurValue_Index;
        unsigned short CurValue_SubIndex;

        unsigned short Threshold_Index;
        unsigned short Threshold_SubIndex;
        unsigned short ThresholdMode_SubIndex;

        void ChangeBtnPanelLane(TBtnPanelLane *Ptr);

    public:
        __fastcall TMyVacuumPanel(TComponent* Owner,int _iKind,int _iCol,int _iRow);
        void __fastcall SetBG           (TColor OutLine, TColor BackGround);        //設定背景
        void __fastcall SetEvent        (AnsiString Event,     TColor Color);       //顯示事件
        void __fastcall ShowCurectVal   (AnsiString CurectVal, TColor Color);       //顯示目前氣壓值
        void __fastcall ShowThreshold   (AnsiString ThresholdVal, TColor Color);    //顯示目前氣壓值閥值
        void __fastcall RefreshCurrectVal();        //更新顯示氣壓值
        void __fastcall RefreshThresholdVal();      //更新顯示氣壓閥值
        void __fastcall RefreshVaccumIO();          //更新顯示真空建立狀態
        void __fastcall RefreshDOIO();              //更新顯示 DO 狀態
        void __fastcall RefreshEvent();             //更新顯示 Event
        double __fastcall ReadVaccumCurrect();
        double __fastcall ReadVaccumThreshold();
        bool __fastcall ReadVaccumIO();

        void __fastcall WriteVaccumThreshold(double dKpa);
        void InitialThresholdMode();
        bool bInitialThresholdModeOK;
        __fastcall ~TMyVacuumPanel();
        //double VC8ToKpa(int iVal);
        //int KpaToVC8(double dKpa);
        void Reset();
        AnsiString sEvent;
        AnsiString sOldEvent;

        TGroupBox       *GroupBox;
        TImage          *ImgVacuumPanel;
        TEdit           *edSV;
        TSpeedButton    *btnSV;
        TMyLed          *myld1;
        TBtnPanelLane   *bplOn;
        TBtnPanelLane   *bplOff;
};
#endif
