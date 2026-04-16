//---------------------------------------------------------------------------

#ifndef cTemperFromH
#define cTemperFromH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
typedef struct
{
    TPanel *UsePanel;
    AnsiString asName;
    bool bShow;
    bool OnOff;
    bool bFlag;
    int  iCount;
}SHOW_YIELD_TYPE;
//---------------------------------------------------------------------------
class TfTemperFrom : public TForm
{
__published:    // IDE-managed Components
    TTimer *Timer1;
    TPanel *palLed;
    TPanel *Panel73;
    TPanel *Panel72;
    TPanel *Panel71;
    TPanel *pnlLeft;
    TGroupBox *gbFunctionWindow;
    TPanel *palErrorRecovery;
    TPanel *palDoubleDevice;
    TPanel *palCGoodBin;
    TPanel *palYieldMonitor;
    TPanel *palConsAlarm;
    TPanel *palIndexStartErr;
    TPanel *palOcrOn;
    TPanel *pnlQAMode;
    TPanel *palTest1;
    TPanel *palTest2;
    TPanel *palNewTempWindow;
    TPanel *pnl16Heater;
    TGroupBox *grpIndex;
    TPanel *hlNameChamber_2;
    TPanel *hlNameDut_2;
    TPanel *hlTempChamber_2;
    TPanel *hlTempDut_2;
    TGroupBox *gbShuttle;
    TPanel *hlNameShuttle1_2;
    TPanel *hlNameShuttle2_2;
    TPanel *hlTempShuttle1_2;
    TPanel *hlTempShuttle2_2;
    TGroupBox *gbPlate;
    TPanel *hlNamePlate1_2;
    TPanel *hlNamePlate2_2;
    TPanel *hlTempPlate1_2;
    TPanel *hlTempPlate2_2;
    TGroupBox *gbArm1;
    TPanel *hlNameAa1;
    TPanel *hlNameAb1;
    TPanel *hlNameAc1;
    TPanel *hlNameAd1;
    TPanel *hlNameBa1;
    TPanel *hlNameBb1;
    TPanel *hlNameBc1;
    TPanel *hlNameBd1;
    TPanel *hlTempAa1;
    TPanel *hlTempAb1;
    TPanel *hlTempAc1;
    TPanel *hlTempAd1;
    TPanel *hlTempBa1;
    TPanel *hlTempBb1;
    TPanel *hlTempBc1;
    TPanel *hlTempBd1;
    TGroupBox *gbArm2;
    TPanel *hlNameAc2;
    TPanel *hlNameAa2;
    TPanel *hlNameAb2;
    TPanel *hlTempAa2;
    TPanel *hlTempAb2;
    TPanel *hlTempAc2;
    TPanel *hlNameAd2;
    TPanel *hlNameBa2;
    TPanel *hlNameBb2;
    TPanel *hlNameBc2;
    TPanel *hlNameBd2;
    TPanel *hlTempAd2;
    TPanel *hlTempBa2;
    TPanel *hlTempBb2;
    TPanel *hlTempBc2;
    TPanel *hlTempBd2;
    TGroupBox *gbHeatGun;
    TPanel *hlNameHeatGun1_2;
    TPanel *hlNameHeatGun2_2;
    TPanel *hlTempHeatGun1_2;
    TPanel *hlTempHeatGun2_2;
    TGroupBox *gbATCHeatGun;
    TPanel *hlNameATCHeatGun1_2;
    TPanel *hlNameATCHeatGun2_2;
    TPanel *hlTempATCHeatGun1_2;
    TPanel *hlTempATCHeatGun2_2;
    TGroupBox *grpOutSht;
    TPanel *hlNameOutShuttle1;
    TPanel *hlNameOutShuttle2;
    TPanel *hlTempOutShuttle1;
    TPanel *hlTempOutShuttle2;
    TGroupBox *grp_Shuttle2;
    TPanel *hlNameShuttle_3;
    TPanel *hlNameShuttle_4;
    TPanel *hlTempShuttle_3;
    TPanel *hlTempShuttle_4;
    TGroupBox *grp_HopPlate2;
    TPanel *hlNamePlate_3;
    TPanel *hlNamePlate_4;
    TPanel *hlTempPlate_3;
    TPanel *hlTempPlate_4;
    TPanel *pnl32Heater;
    TGroupBox *gbArm1_2;
    TPanel *hlNameAe1;
    TPanel *hlNameAf1;
    TPanel *hlNameAg1;
    TPanel *hlNameAh1;
    TPanel *hlNameBe1;
    TPanel *hlNameBf1;
    TPanel *hlNameBg1;
    TPanel *hlNameBh1;
    TPanel *hlTempAe1;
    TPanel *hlTempAf1;
    TPanel *hlTempAg1;
    TPanel *hlTempAh1;
    TPanel *hlTempBe1;
    TPanel *hlTempBf1;
    TPanel *hlTempBg1;
    TPanel *hlTempBh1;
    TGroupBox *gbArm2_2;
    TPanel *hlNameAe2;
    TPanel *hlNameAf2;
    TPanel *hlTempAe2;
    TPanel *hlTempAf2;
    TPanel *hlTempAg2;
    TPanel *hlNameAg2;
    TPanel *hlNameAh2;
    TPanel *hlNameBe2;
    TPanel *hlNameBf2;
    TPanel *hlNameBg2;
    TPanel *hlNameBh2;
    TPanel *hlTempAh2;
    TPanel *hlTempBe2;
    TPanel *hlTempBf2;
    TPanel *hlTempBg2;
    TPanel *hlTempBh2;
    TGroupBox *grp2DID;
    TPanel *hlName2D_2;
    TPanel *hlTemp2D_2;
    TGroupBox *grpLB;
    TPanel *hlNameLB;
    TPanel *hlTempLB;
    TGroupBox *grpESD;
    TPanel *hlNameESD;
    TPanel *hlTempESD;
    TGroupBox *grpChamber;
    TPanel *hlNameChamber_3;
    TPanel *hlTempChamber_3;
    TGroupBox *grpBase;
    TPanel *hlNameBase1;
    TPanel *hlTempBase1;
    TPanel *hlNameBase4;
    TPanel *hlTempBase4;
    TPanel *hlNameBase2;
    TPanel *hlTempBase2;
    TPanel *hlNameBase5;
    TPanel *hlTempBase5;
    TPanel *hlNameBase3;
    TPanel *hlTempBase3;
    TPanel *hlNameBase6;
    TPanel *hlTempBase6;
    TGroupBox *gb_Index;
    TPanel *hlNameDut_A1;
    TPanel *hlTempDut_A1;
    TPanel *hlNameDut_A2;
    TPanel *hlNameDut_A4;
    TPanel *hlTempDut_A2;
    TPanel *hlTempDut_A4;
    TPanel *hlNameDut_A3;
    TPanel *hlTempDut_A3;
    TMemo *Memo1;
    TButton *Button1;
    TPanel *gbOldTempWindow;
    TPanel *pnl4Plate1;
    TPanel *hlNamePlate1;
    TPanel *hlTempPlate1;
    TPanel *pnl4Plate2;
    TPanel *hlNamePlate2;
    TPanel *hlTempPlate2;
    TPanel *pnl4Sht1;
    TPanel *pnl4Sht2;
    TPanel *pnl4Head1;
    TPanel *pnl4Head3;
    TPanel *pnl4Head5;
    TPanel *pnl4Head7;
    TPanel *pnl4Dut;
    TPanel *pnl4Chamber;
    TPanel *pnl4CCD;
    TPanel *pnl4HotGun1;
    TPanel *pnl4HotGun2;
    TPanel *hlNameShuttle1;
    TPanel *hlTempShuttle1;
    TPanel *hlNameShuttle2;
    TPanel *hlTempShuttle2;
    TPanel *hlNameHead12;
    TPanel *hlTempHead12;
    TPanel *hlNameHead34;
    TPanel *hlTempHead34;
    TPanel *hlNameHead56;
    TPanel *hlTempHead56;
    TPanel *hlNameHead78;
    TPanel *hlTempHead78;
    TPanel *hlNameDut;
    TPanel *hlTempDut;
    TPanel *hlNameChamber;
    TPanel *hlTempChamber;
    TPanel *hlNameCCD;
    TPanel *hlTempCCD;
    TPanel *hlNameHeatGun1;
    TPanel *hlTempHeatGun1;
    TPanel *hlNameHeatGun2;
    TPanel *hlTempHeatGun2;
    TButton *Button6;
    TGroupBox *grpLB1;
    TPanel *hlNameLBUp;
    TPanel *hlNameLBDown;
    TPanel *hlTempLBUp;
    TPanel *hlTempLBDown;
    TGroupBox *grpDoor;
    TPanel *hlNameDoor1;
    TPanel *hlNameDoor2;
    TPanel *hlTempDoor1;
    TPanel *hlTempDoor2;
    TGroupBox *grpCCD;
    TPanel *hlNameCCD_3;
    TPanel *hlTempCCD_3;
    TPanel *hlNameCCD_2_2;
    TPanel *hlTempCCD_2_2;
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall Panel73MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall Panel72MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall Panel71MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall Button6Click(TObject *Sender);
private:    // User declarations
    void ShowOffYieldFun();                     //jou 2010-12-04
    void ChangeFormSize();
    bool bStartRecord;
    int iStartMin;
    int iFormWidth;
    void __fastcall RecordTemp();
public:     // User declarations
    __fastcall TfTemperFrom(TComponent* Owner);
    enum eShowYieldType{esytErrorRecovery=0, esytDoubleDevice=1, esytCGoodBin=2,    esytYieldMonitor=3, esytConsAlarm=4,
                        esytIndexStartErr=5, esytOcrOn=6,        esytQAMode=7,  //20141204 ChungHung add for ATK QA mode esytNoUse--->QA mode
                        esytOEE=8,                                              //Steven 20220407 : esytTest1 --> esytOEE
                        esytTest2=9,
                        SHOW_YIELD_TOTAL};
    SHOW_YIELD_TYPE strShowYield[SHOW_YIELD_TOTAL];           //jou 2010-12-04

    bool __fastcall ShowThermo(int Addr);
    bool bShow;
    void __fastcall ShowHotName();
    void __fastcall ArrangeFormWidth();                                         //Steven 20231111 : 重新排列TempForm

    void SetShowYield(eShowYieldType iItem,bool bOnOff);   //jou 2010-12-04
    void ShowYieldFuntion();                    //jou 2010-12-04
    void SetIndex16HeaterPos();                 //Steven 20111208 : 重新排列溫控器位置
//    bool __fastcall TempOverCannotRunShowAlarm();
//    void __fastcall TempOverCannotRunShowAlarmClear();
    void __fastcall SetTempFormPos(); //Steven 20120525 : 熱風槍
    bool __fastcall TempRunShowAlarmHigh();  //kevin 20210112 溫度過高
    bool __fastcall TempRunShowAlarmLow();   //kevin 20210112 溫度過低
    bool Check_Tri_Temp_All_Temperature();//Ztex 2023.04.19 Add HT-1032 TriTemp Function)
};
//---------------------------------------------------------------------------
extern PACKAGE TfTemperFrom *fTemperFrom;
//---------------------------------------------------------------------------
#endif
