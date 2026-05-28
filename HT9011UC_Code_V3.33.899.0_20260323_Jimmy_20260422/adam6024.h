//---------------------------------------------------------------------------
#ifndef adam6024H
#define adam6024H
//---------------------------------------------------------------------------
bool Open_ADAM_6024(AnsiString IP, int Num);                                    //Ifor 20150709 ：加入設備位置
bool Open_ADAM_6024();                                                          //Jimmychiu 20230804 : 整合全部連線檢查
void Close_ADAM_6024();
int ADAM_ReadPA(double *dValue, int iCH=5);                                     //20111217 ChungHung   //Ifor 20190104 :add 選擇 Adam 讀取CH //wei 20220309 Add EP Return Voltage
void ADAM_Rang(int v);                                                          //20111217 ChungHung

bool ADAM_Alarm(int iArm=2);                                                    //20111217 ChungHung
bool ADAM_Alarm_Kg(int iAdd);                                                   //JerryYang 20171024 (wei) add EP check range by kg
bool ADAM_DualAlarm(int iType);                                                 //Ifor 20221228 add:Dual EP Check

//AI(ht9045-v899) 20260526: expose actual EP pressure route so Multi mode follows SwMultiEp state.
bool IsMultiEPPressureRouteActive();
bool IsIndependentEPPressureRouteActive();
bool ADAM_WriteVoltage(double v);
void WriteDigital(double v);
extern bool bADAM6420Install;

double ADAM_ReadVoltage(int Num,int iCH=5);                                     //Ifor 20150709 :選擇裝置順序讀取資料   //Ifor 20190104 :add 選擇 Adam 讀取CH
void ADAM_DirectWriteData(WORD data,int Num, int iAdd=1);                       //Ifor 20150709 :選擇裝置順序寫入資料
void ADAM_WriteMaxData(bool bFullForce);                                        //Steven 20241014 : 整合auto height輸出壓力
double KpaTransferKG(int fKpa, bool bDualForce=false);                          //Ifor 20150826 :新增 Kpa 轉 公斤 Function  //JerryYang 20171023 (wei) int ->double
double MultiTransferKG(int fKpa, bool bDualForce=false);                        //AI(ht9045-v899) 20260504: port from V896 (Ifor 20250416) for Multi EP per-site Kpa->Kg.
extern double iAdamOutValue;
int TransformFuntion(double fInputKG,bool bDualForce=false, bool bSoft=false, int iArm=0);//Ifor 20190107 : add Dual Force Diameter
bool ADAM_ReadAIValue(int iNum, int iChannel, double *dmA, double *dDegree);    //Hmy 20170609 露點計讀AI使用，先寫固定

void APAX_WriteData(bool bDir, WORD wdata, int iArm=0);
bool Open_APAX(char IP[]);
void ADAM_ReturnValueCheck(bool bHome=false);                                   //wei 20220309 Add EP Voltage Error Alarm

bool fCheckConnectStatus_ADAM6024(int Num);                                     //Hmy 20170120 add check Adam6024 Status
int GetModuleConnectionCount(int Num);                                          //Nickliu 20230313 add ADAM6024 Command
AnsiString GetFirmwareName(int Num);                                            //Nickliu 20230313 add ADAM6024 Command
AnsiString GetModuleName(int Num);                                              //Nickliu 20230313 add ADAM6024 Command
bool ClearAllConnection(int Num);                                               //Nickliu 20230313 add ADAM6024 Command
int GetModuleHostIdleTime();                                                    //Nickliu 20230313 add ADAM6024 Command
bool SetModuleHostIdleTime(int ihostIdleTime);                                  //Nickliu 20230313 add ADAM6024 Command
bool fCheckModuleFWISNew_ADAM6024(int Num);                                     //Nickliu 20230314 Add Check Adam Module Name
bool fCheckModuleName_ADAM6024(int Num);                                        //Nickliu 20230314 Add Check Adam FW Is New

void EpSwitch(bool Arm1, bool Arm2);
void EPSwitchOnOff(int iArm);                                                   // 0:全關, 1:Arm1開, 2:Arm2開, 3:全開
enum EPSwOn{eEPSwOff    =0,
            eEPSwArm1   =1,
            eEPSwArm2   =2,
            eEPSwBoth   =3
            };

//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
#include <ComCtrls.hpp>
#include <ScktComp.hpp>
//---------------------------------------------------------------------------
class TfAdam6024 : public TForm
{
__published:    // IDE-managed Components
    TClientSocket *ClientSocket1;
    void __fastcall ClientSocket1Connect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ClientSocket1Error(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
    void __fastcall FormDestroy(TObject *Sender);
private:    // User declarations
public:     // User declarations
    __fastcall TfAdam6024(TComponent* Owner);

    struct cmdbuf
    {
        int status;
        char cmdBuf[512];
        char cmdSize;
    } cmdBuf[8];
    unsigned bufIdx;
    void WriteAO(double AO1=0.0,  double AO2=0.0, double AO3=0.0, double AO4=0.0);
    void OpenSocket(AnsiString IP, int Port);
    void CloseSocket();
    AnsiString ADAMErrorMessage[16];        //JimmyChiu 20230308 : 15 --> 16
};
extern PACKAGE TfAdam6024 *fAdam6024;
WORD TransFuntion(double v);  //JerryYang 20160701 1~16kg建表轉換W

#endif
