//---------------------------------------------------------------------------
#define ATC_Server_Ver 2016-10-19-1 //Eliot 2016_0816
//---------------------------------------------------------------------------
#ifndef ATC_Handler_SideH
#define ATC_Handler_SideH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ScktComp.hpp>
#include <Buttons.hpp>
#include <vector.h>
//---------------------------------------------------------------------------
#define ATC_MAX_SITE        40 //Ztex 2023.04.19 Add HT-1032 TriTemp Function Max 32 -> 40
//---------------------------------------------------------------------------
#define ATC_TYPE_20         20
#define ATC_TYPE_21         21
#define ATC_TYPE_30         30
#define ATC_TYPE_31         31
#define ATC_TYPE_32         32
#define ATC_TYPE_33         33
#define ATC_TYPE_35         35  //JerryYang 20220408 : add for ATC3.5
#define ATC_TYPE_36         36
#define ATC_TYPE_50         50
#define ATC_TYPE_51         51
#define ATC_TYPE_60         60
#define ATC_TYPE_61         61
#define ATC_TYPE_70         70
#define ATC_TYPE_UNSET      9999
#define iATC_Refrigerator_Num        8   //Ztex 2023.04.19 Add HT-1032 TriTemp Function

#define ATC_MAX_COMMAND  150
//---------------------------------------------------------------------------
class TATC_InterfaceForm : public TForm
{
__published:    // IDE-managed Components
    TPanel *Panel1;
    TPageControl *PageControl;
    TTabSheet *TabSheet20;
    TTabSheet *TabSheet21;
    TTabSheet *TabSheet30;
    TTabSheet *TabSheet31;
    TTabSheet *TabSheet50;
    TTabSheet *TabSheet60;
    TTabSheet *TabSheet70;
    TClientSocket *ClientSocket;
    TTimer *Timer;
    TLabel *A31_TempOffsetLabel;
    TLabel *A31_SetTempLabel;
    TSpeedButton *A31_RunStopsBtn;
    TLabel *Label1;
    TLabel *Label2;
    TEdit *A31_Temp_OFS;
    TButton *A31_Set_Temp_OFS;
    TEdit *A31_Temperature;
    TButton *A31_Set_Temp;
    TButton *ATC_Recipe_File;
    TGroupBox *ChannelGroupBox;
    TButton *ATC_SetChannelEnabled;
    TButton *A31_Read_Temp;
    TComboBox *A31_Single_Temp_OFS_Site;
    TButton *ATC_Set_Single_Temp_OFS;
    TComboBox *A31_Single_Temp_Site;
    TButton *ATC_Set_Single_Temp;
    TButton *Button2;
    TEdit *A31_Single_Temp;
    TEdit *A31_Single_Temp_OFS;
    TSpeedButton *ATC_ConnectBtn;
    TComboBox *A31_Temp_Reading_Mode;
    TButton *A31_Set_Temp_Reading_Mode;
    TPanel *Temp_Panel;
    TPanel *Panel3;
    TPanel *Panel4;
    TPanel *Panel5;
    TPanel *Panel2;
    TPanel *Panel6;
    TPanel *Panel7;
    TPanel *Panel8;
    TPanel *Panel9;
    TButton *A31_Handler_Arm;
    TButton *A31_Test_Start;
    TButton *A31_Initial_Set;
    TButton *A31_Chiller_Status;
    TPanel *Panel10;
    TMemo *A31_Memo;
    TMemo *AlarmMsg;
    TButton *A31_GetAlarmMsg;
    TPanel *Panel11;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;
    TComboBox *cBox_PID;
    TEdit *ed_P;
    TEdit *ed_I;
    TEdit *ed_D;
    TButton *PID_Set;
    TGroupBox *GroupBox1;
    TComboBox *AtcModuleBox;
    TButton *RunModuleBtn;
    TButton *StopModuleBtn;
    TCheckBox *EMG_UP_DOWN;
    TCheckBox *cBox_RecordComm;
    TSpeedButton *SecondSensorFunction;
    TTimer *CommFlagTimer;
    TTabSheet *TabSheet51;
    TComboBox *ATC_RecipeFile;
    TLabel *ATC_Now_RecipeFile;
    TButton *btnGetNowRecipeFile;
    TButton *btnGetRecipeFileList;
    TGroupBox *GroupBox2;
    TLabel *lab_SW;
    TLabel *labFW1;
    TLabel *labFW7;
    TLabel *labFW2;
    TLabel *labFW8;
    TLabel *labFW3;
    TLabel *labFW9;
    TLabel *labFW4;
    TLabel *labFW10;
    TLabel *labFW5;
    TLabel *labFW11;
    TLabel *labFW6;
    TLabel *labFW12;
    TLabel *Label7;
    TGroupBox *GroupBox3;
    TCheckBox *cBox_CH1;
    TCheckBox *cBox_CH2;
    TCheckBox *cBox_CH3;
    TCheckBox *cBox_CH4;
    TButton *BtnSetTSD;
    TSpeedButton *SpeedButton1;
    TEdit *Edit1;
    TEdit *Edit2;
    TSpeedButton *SpeedButton2;
    TEdit *A31_LotID;
    TButton *A31_LotStart;
    TButton *A31_LotEnd;
    TButton *btnGetSelfTestResult;
    TEdit *Edit3;
    TSpeedButton *SpeedButton3;
    TSpeedButton *SpeedButton4;
    TButton *btnFFCEnable;
    TButton *btnFFCTrigger;
    void __fastcall ClientSocketConnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ClientSocketDisconnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ClientSocketRead(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall A31_Set_TempClick(TObject *Sender);
    void __fastcall A31_Set_Temp_OFSClick(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall A31_Set_Temp_Reading_ModeClick(TObject *Sender);
    void __fastcall A31_RunStopsBtnClick(TObject *Sender);
    void __fastcall ATC_Recipe_FileClick(TObject *Sender);
    void __fastcall A31_Initial_SetClick(TObject *Sender);
    void __fastcall ATC_SetChannelEnabledClick(TObject *Sender);
    void __fastcall TimerTimer(TObject *Sender);
    void __fastcall A31_Read_TempClick(TObject *Sender);
    void __fastcall A31_Handler_ArmClick(TObject *Sender);
    void __fastcall ATC_Set_Single_Temp_OFSClick(TObject *Sender);
    void __fastcall ATC_Set_Single_TempClick(TObject *Sender);
    void __fastcall A31_Test_StartClick(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall A31_Chiller_StatusClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall ATC_ConnectBtnClick(TObject *Sender);
    void __fastcall ClientSocketError(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
    void __fastcall A31_GetAlarmMsgClick(TObject *Sender);
    void __fastcall PID_SetClick(TObject *Sender);
    void __fastcall EMG_UP_DOWNClick(TObject *Sender);
    void __fastcall cBox_RecordCommClick(TObject *Sender);
    void __fastcall SecondSensorFunctionClick(TObject *Sender);
    void __fastcall CommFlagTimerTimer(TObject *Sender);
    void __fastcall btnGetNowRecipeFileClick(TObject *Sender);
    void __fastcall btnGetRecipeFileListClick(TObject *Sender);
    void __fastcall BtnSetTSDClick(TObject *Sender);
    void __fastcall SpeedButton1Click(TObject *Sender);
    void __fastcall SpeedButton2Click(TObject *Sender);
    void __fastcall btnGetSelfTestResultClick(TObject *Sender);
    void __fastcall SpeedButton3Click(TObject *Sender);
    void __fastcall SpeedButton4Click(TObject *Sender);
    void __fastcall btnFFCEnableClick(TObject *Sender);
    void __fastcall btnFFCTriggerClick(TObject *Sender);
    void __fastcall A31_TemperatureClick(TObject *Sender);
    void __fastcall A31_Temp_OFSClick(TObject *Sender);
    void __fastcall ed_PClick(TObject *Sender);
private:    // User declarations
    TStringList *slReveiveStringList;
    TCriticalSection *csReceiveString;
    bool IsConnectFlag;
    bool bConnecting;
    AnsiString LastMsg;                                                         //ChungHung 20150826 add fix for continue error message
    FILE *COMM_Log;                                                             //Eliot 2015_1008
    double dCommLogFileSize;                                                    //Eliot 2015_1008

public:     // User declarations
    __fastcall TATC_InterfaceForm(TComponent* Owner);
    void __fastcall SendCommand(int iCommIndex);
    void __fastcall ProcessReceiveString_ATC();

    TStringList *sDataList;
    TStringList *sAlarmMessageList;                                             //Eliot 2016_0722

    int iATC_MODE_TYPE;
    int iDataCount;
    int iHandlerSiteCount;
    int iATC_ModuleCount;
    int  iSendData[ATC_MAX_SITE];
    bool bSendData[ATC_MAX_SITE];
    AnsiString asSendData_PF[ATC_MAX_SITE];                                     //Hmy 20240207 Add Send Pf Par to Atc
    double dSendKP[ATC_MAX_SITE];
    double dSendKI[ATC_MAX_SITE];
    double dSendKD[ATC_MAX_SITE];
    AnsiString asSendData[ATC_MAX_SITE];
    TCheckBox *ATC_Channel[ATC_MAX_SITE];
    TPanel    *ChPanel[ATC_MAX_SITE];
    TPanel    *ChTempTC[ATC_MAX_SITE];
    TPanel    *ChTempTC2[ATC_MAX_SITE];
    TPanel    *ChTempTJ[ATC_MAX_SITE];

    double  dATC_Result[(ATC_MAX_SITE*2)+1];                                    //Eliot 2016_0802  加1是給溫度索引值用
    int  iATC_AlarmCode[ATC_MAX_SITE];
    bool bATC_Power[ATC_MAX_SITE];
    double dTC[ATC_MAX_SITE];
    double dTC2[ATC_MAX_SITE];
    double dTJ[ATC_MAX_SITE];
    double dTC_Temp[ATC_MAX_SITE];
    double dTJ_Temp[ATC_MAX_SITE];

    bool bATCRun;
    bool bGetATC_ModuleStateFlag;                                               //Eliot 2015_1022
    bool bATC_ModuleState[ATC_MAX_SITE];                                        //Eliot 2015_1022

    double dNowATCSetTemp[ATC_MAX_SITE];                                        //kirin 2015_11_02
    double dNowATCSetOffset[ATC_MAX_SITE];                                      //Eliot 2015_1124
    int  flagATCSelfTestResult;                                                 //2016-07-17    Dell    Add //0:動作中 1:+5 2:-5 3:+0 4:exception 5:success
    bool bATC2ndFunctionFlag;                                                   //Eliot 2016_0802 true:Enabled false:Disabled
    int  iATCSelfTestStatus;                                                    //Eliot 2016_0816 0:無self-test狀態 1:+5c 2:-5c 3:+0c

    bool bATCLotStartStatus;                                                    //Ifor(Eliot) 20161121 add ATC Lot Start Status    0:未收到 1:已收到
    bool bATCLotEndStatus;                                                      //Ifor(Eliot) 20161121 add ATC Lot End Status      0:未收到 1:已收到
    bool bReSendATCLotEven;                                                     //Eliot 20161202 Handler的變數移到Interface中,這樣所有機型都可以用

    int     ReadTempInterval;
    double  P_Value[ATC_MAX_SITE];
    double  I_Value[ATC_MAX_SITE];
    double  D_Value[ATC_MAX_SITE];

    bool ATC_Module[ATC_MAX_SITE];                                              //Eliot 2015_0930
    bool bOpenFlag;                                                             //Eliot 2015_1008

    int  iCommCount[ATC_MAX_COMMAND];                                           //Eliot 2015_1027
    bool bCommFlag[ATC_MAX_COMMAND];                                            //Eliot 2015_1027
    AnsiString asCommString[ATC_MAX_COMMAND];                                   //Eliot 2015_1027
    bool bHandlerOnlineFlag;                                                    //Eliot 2015_1027
    bool bGetNowRecipeFlag;                                                     //Eliot 2015_1126
    bool bGetRecipeListFlag;                                                    //Eliot 2015_1126
    bool bGetATC_SW_VerFlag;                                                    //Eliot 2016_0503
    bool bGetATC_SEND_TEMP_READY;                                               //Steven 20160604 : by site TSD
    int iATC_DataCount;                                                         //kevin 20151127 工作檔比數
    AnsiString ATC_WorkFileList[5000];                                          //kevin 20151127 工作檔比數
    AnsiString asLotID;                                                         //Eliot 2016_1020
    int  iPFCPARAMETERData[280];

    AnsiString asData;
    AnsiString asDataTemp;
    AnsiString sATC_COMMAND;
    AnsiString asRecipeFile;
    AnsiString asATC_ErrorMsg[8];
    AnsiString asATC_SW_Ver;                                                    //Eliot 2015_1007
    bool bGetControllerFW_Flag;                                                 //Ifor 20170320 Get ATC power supply firmware number
    AnsiString asATC_FW_Ver[ATC_MAX_SITE];                                      //Eliot 2015_1007
    bool bATCReadData;                                                          //kevin 20151125 開啟讀取ATC TIMER
    AnsiString asHandlerVer;                                                    //Eliot 2015_1126
    AnsiString asATCFilePath;                                                   //Steven 20160707 : 將ATC MODE存檔

    bool bATCCommuateError;                                                     //kevin 20160423
    AnsiString aATCCommuateError;                                               //kevin 20160423

    int iATCTempIndex;                                                          //Eliot 2016_0802
    int iATCTempIndexCount;                                                     //Eliot 2016_0802
    int iSelfTestFailSite[ATC_MAX_SITE];                                        //Eliot 20161130 增加Fail Site資料
    int iSelfTestResult[ATC_MAX_SITE];                                          //Eliot 20170105 詢問Self-Test結果 回傳資料0:失敗 1:成功
    bool bGetControllerSN_Flag;                                                 //Eliot 20170221 Get ATC power supply serial number
    AnsiString asControllerSN[ATC_MAX_SITE];                                    //Eliot 20170221 Get ATC power supply serial number
    AnsiString asHandler2DID;                                                   //ben 20240927 : add CCD Bar Code to ATC
    AnsiString asGetPID[ATC_MAX_SITE][3];                                       //wei 20191024
    AnsiString asSetPFCData;
    AnsiString asSLOPEOFFSET[ATC_MAX_SITE*2];
    AnsiString asTJVoltage[ATC_MAX_SITE];
    AnsiString asQueryTJTemp[ATC_MAX_SITE][3];                                  //Record Tj Temp(Max/Min/Avg)//Eliot 20220411
    int iRecordTjTemp;                                                          //Record Tj Temp(Max/Min/Avg)//Eliot 20220411
    AnsiString asControlMode[ATC_MAX_SITE];
    AnsiString asChangeFile;
    AnsiString asTCWaterValue;
    AnsiString asDynamicPID;

    void __fastcall ShowATC_Page(int iType);
    void __fastcall SetReadTempInterval(int iInterval);
    bool __fastcall SetAddressPort(AnsiString asAddress, int iPost);
    void __fastcall SendReadTempComm();
    void __fastcall SetReadTemp();
    void __fastcall InitialATCSet();
    void __fastcall SetChannelCount(int iCount);
    void __fastcall Connect();
    void __fastcall Disconnet();
    bool __fastcall IsConnect();
    bool __fastcall IsConnecting();
    void __fastcall ChangeRecipe(AnsiString RecipeName);
    void __fastcall ChangeRecipe(AnsiString RecipeName, double dTemperature);
    void __fastcall Run();
    void __fastcall Stop();
    void __fastcall SetAllTemp(double dTemperature);
    void __fastcall SetMultiZoneTemp(double dTemperature);
    void __fastcall SetTemp(int iChCount, double *dTemp);
    void __fastcall SetSingleTemp(int iChannel, double dTemp);
    void __fastcall SetOffset(int iChCount, double *dOffset);
    void __fastcall SetFFCOffset(int iChCount, double *dOffset);                //Ifor 20190730 : add FFC Temperature Offset
    void __fastcall SetSingleOffset(int iChannel, double dOffset);
    void __fastcall EnablesChannel(int iChCount, bool *bEnabled);
    void __fastcall StartTesting();
    void __fastcall TestFinish();
    void __fastcall SiteTesting(int iCount, bool *bTesting);                    //Eliot 2016_0503
    void __fastcall HandlerArm(int iArm);
    void __fastcall ReadTC(int iChCount, double *dTempTC);
    void __fastcall ReadTC2(int iChCount, double *dTempTC2);
    void __fastcall ReadTJ(int iChCount, double *dTempTJ);
    void __fastcall ReadPID(int *kP, double *kI, int *kD);
    void __fastcall SetPID(int kP, double kI, int kD);
    void __fastcall SetMaxTemperature(double dTemperature);
    bool __fastcall HasAlarmMsg();
    AnsiString __fastcall GetAlarmMsg();
    void __fastcall ClearATCMessage();
    void __fastcall PushBackMessage(AnsiString aMsg);                           //ChungHung 20150826 add fix for continue error message

    double __fastcall GetSetTemp(int iChannel=0);                               //ChungHung 20150826 add
    bool   __fastcall GetRunState();                                            //ChungHung 20150826 add
    bool   __fastcall CheckATCSetTemp(double dSettemp);                         //wei 20230202 判斷ATC所有控制器設定溫度，一個溫度不一樣就要重新設定溫度

    void __fastcall SetATC_ModuleCount(int iCount);
    void __fastcall SetModuleRunStop(bool *Enabled);

    void __fastcall Set2ndRange_P(int iCount, double *Range);
    void __fastcall Set2ndRange_N(int iCount, double *Range);
    void __fastcall Set2ndTime(int iTime);
    void __fastcall Set2ndFunction(bool bEnabled);
    void __fastcall UseTSD_Function(int iCount, int *iSiteOn);                  //Steven 20160604 : by site TSD
    void __fastcall EMG_UpDown(bool bDown);                                     //Eliot 2015_1005
    void __fastcall CheckFolderExist();
    void __fastcall SaveCommLog(bool bCloseFile, TMemo *CommMemo);              //Eliot 2015_1008
    void __fastcall GetSW_Ver();                                                //Eliot 2015_1015
    void __fastcall GetFW_Ver();                                                //Eliot 2015_1015
    void __fastcall SetCommandString();                                         //Eliot 2015_1027
    double __fastcall GetSetOffset(int iChannel=0);                             //Eliot 2015_1124
    void __fastcall GetNowRecipeFile();                                         //Eliot 2015_1027
    void __fastcall GetRecipeFileList();                                        //Eliot 2015_1027
    void __fastcall SetHandlerVersion(AnsiString asVersion);                    //Eliot 2015_1126
    void __fastcall AutoCheckList();                                            //Eliot 2016_0106
    void __fastcall SetChillerTemp(double dTemp);                               //kevin 20160425
    void __fastcall ReadATCModeType();                                          //Steven 20160707 : 將ATC MODE存檔
    void __fastcall UseTJ_MODE(int iChCount, int *iMode);                       //Eliot 2015_1005
    void __fastcall SetTempReadMode(int iChCount, int *iMode);                  //2016-02-01    Dell    Add     SetTempReadMode
    void __fastcall SetTJOffset(int iChCount, double *dOffset);
    void __fastcall GetTempReady();
    void __fastcall SetRunSelfTest();                                           //2016/7/17 Dell    Add
    int __fastcall  GetRunSelfTestResult();                                     //ChungHung 20160719 add
    int __fastcall  GetRunSelfTestStatus();                                     //Ifor 20160824 add Get ATC Self Test Status
    void __fastcall GetATC_2ND_Status();                                        //Eliot 2016_0802
    void __fastcall SetManualSelfTest();                                        //Eliot 2016_0816
    void __fastcall QuerySelfTestStatus();                                      //Eliot 2016_0816
    void __fastcall SendHandlerStatus();                                        //Eliot 2016_0816
    void __fastcall LotStart(String sLotID);                                    //Eliot 2016_1020
    void __fastcall LotEnd(String sLotID);                                      //Eliot 2016_1020
    bool __fastcall GetATCLotEndStatus();                                       //Ifor 20161121 Get ATC Lot End 狀態
    bool __fastcall GetATCLotStartStatus();                                     //Ifor 20161121 Get ATC Lot Start 狀態
    void __fastcall QuerySelfTestResult(int iChCount);                          //Eliot 20170105 詢問Self-Test結果 回傳資料0:失敗 1:成功
    void __fastcall SendAbnormalSite(int iChCount, int *AbnormalSite);          //Eliot 20170116 Handler傳送有異常的Site給ATC
    void __fastcall GetControllerSN(int iChCount);                              //Eliot 20170221 Get ATC Controller SN
    void __fastcall SetAirValve(bool bControl, bool bOnOff);
    void __fastcall FFCOffsetEnable( bool Enabled);                             //Ifor 20190730 : add FFC Temperature Offset
    void __fastcall ChannelFFCTrigger(int iChCount,int Arm, bool Enabled);
    void __fastcall AskATCDateToHandle();                                       //kevin 20200416 log ATC Data
    void __fastcall GetChillerStatus();                                         //Ifor 20230515 add:取得Chiller 狀態
    void __fastcall SendHandler2DID(int iArm, bool bStartRecord, AnsiString as2DData="", int iSiteCount=0);                //ben 20240927 : add CCD Bar Code to ATC
    void __fastcall Set_ASIF_TJ_EFUSED(AnsiString Str);                         //Steven 20240903 : for MTK ASIF data
    void __fastcall Get_ASIF_TJ_REQUEST();                                      //Steven 20240903 : for MTK ASIF data
    void __fastcall Get_ASIF_TJ_FB();                                           //Steven 20240903 : for MTK ASIF data

    void __fastcall SendSwitchRefrigerator(int iRefrigeratorNo, int iRefrigeratorOnOff);     //Ztex 2023.04.19 Add HT-1032 TriTemp Function ==>
    int iATC_RefrigeratorUserMode[2][iATC_Refrigerator_Num+1];
    void __fastcall Send_AtcDefrostStatus(int iOpenClose);
    struct stcSend_Rec_AirMachineStatus
    {
        int iH2A_SendStatus;
        int iA2H_RecStatus;
    }Send_Rec_AirMachineStatus, Send_Rec_AirMachineStatus_Index;
    void __fastcall SendAirMachineStatus(int iEnableAirMachine, double dSetSocketTempature, double dSetIndexTempature);
    bool bUseAirMachine;                                                        //kevin 20161230 AirMach hard ware
    void __fastcall SendEnableAtcAtcFreonRecover(int iOpenClose=0);

    int __fastcall Get_ATCRefrigeratorAtcSetMode(int iRefrigeratorNum, int *iTempRefrigeratorUserMode);  //Add ATC Refrigerator Status
    void __fastcall SendAllRefrigeratorFullOpen(int iArm1Status = 0, int iArm2Status = 0);   //Add  Use SingleCenter IndexArm  RefrigerantFullyOpen
    double dATC_RefrigeratorAllStatus[iATC_Refrigerator_Num][9];                //Add Refrigerant machine Status(Status, ValvePos, Hp_1st, Lp_1st, Hp_2nd, Lp_2nd, Lp_2nd,ReadAdjustValveValue)
    double dATC_RefrigerantMachineHighLimit;                                    //Add Refrigerant machine Status(Status, ValvePos, Hp_1st, Lp_1st, Hp_2nd, Lp_2nd, Lp_2nd,ReadAdjustValveValue)
    double dATC_RefrigerantMachineLowLimit;                                     //Add Refrigerant machine Status(Status, ValvePos, Hp_1st, Lp_1st, Hp_2nd, Lp_2nd, Lp_2nd,ReadAdjustValveValue)
    bool bReadRefrigerantMode_AllStatus;                                        //Add Refrigerant machine Status(Status, ValvePos, Hp_1st, Lp_1st, Hp_2nd, Lp_2nd, Lp_2nd,ReadAdjustValveValue)
    AnsiString asRefrigeratorStatus;                                            //Add Refrigerant machine Status(Status, ValvePos, Hp_1st, Lp_1st, Hp_2nd, Lp_2nd, Lp_2nd,ReadAdjustValveValue)
    bool bReadRefrigerantMode_Send;                                             //Add Refrigerant machine Status(Status, ValvePos, Hp_1st, Lp_1st, Hp_2nd, Lp_2nd, Lp_2nd,ReadAdjustValveValue)
    bool bReadRefrigerantMode_Recv;                                             //Add Refrigerant machine Status(Status, ValvePos, Hp_1st, Lp_1st, Hp_2nd, Lp_2nd, Lp_2nd,ReadAdjustValveValue)
    double dATC_Result_Offset[(ATC_MAX_SITE*2)+1];                              //Add offset可以為小數點第一位，原先讀取為整數，目前變更為浮點
    void Check_ATC_Busy_State();
    //add air Machine ->
    struct stcAirMachineInfo
    {
        AnsiString asATC_AirMachineSetTemp;
        AnsiString asATC_AirMachineNowTemp;
        AnsiString asATC_AirMachineSocketBaseTemp;  //2016.11.10 hmy modify
        AnsiString asATC_AirMachineStatus;
        AnsiString asATC_AirMachineAlarm;
        AnsiString asATC_AirMachineAirStatus;
        int        iDefrostSec;    //kevin 20161230 除霜到數
        double        iATC_AirMachineSocketChTemp[4];
        int        iATC_AirVolume;
        AnsiString asATC_AirMachineIndexGetTemp_Ch1;
        AnsiString asATC_AirMachineIndexGetTemp_Ch2;
    }AirMachineInfo,AirMachineInfo_Index;
    void __fastcall Get_ATCRefrigeratorAllStatus(bool bAllStatus);
    void __fastcall Send_ATC_AIRMACHINE_STATUS2(int iEnableAirMachine,double dTemp);   //Add ATC_AIRMACHINE_STATUS2 Command //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    void __fastcall Send_ATC_DewPoint(double dDewPoint_Index,double dDewPoint_InSht,double dDewPointOutSht);
    bool bCloseAirMachine;
    //Ztex 2023.04.19 Add HT-1032 TriTemp Function <==
    void __fastcall Send_ATCSETTJENABLED(int iOpenClose=0);                     //Hmy 20210727 Add handler端設定TJ參數(Slope、Offset)
    void __fastcall Send_ATCSETTJPARAMETER(AnsiString asSlope, AnsiString asOffset);    //Hmy 20210727 Add handler端設定TJ參數(Slope、Offset)
    void __fastcall Send_HANDLERTESTSITEMAPPING(int iChCount, int *iSiteMap);   //Hmy 20210727 Add handler端設定TJ參數(Slope、Offset)  //JerryYang 20231128 : 傳送ATC site map
    void __fastcall Send_ATCWATCHDOG(bool bTjWatchdog, double dDelaySOT, double dInputVLow, double dInputVHigh, double dContinueSec);     //JerryYang 20231205 : ATC Tj WatchDog  //JerryYang 20251201 : ATC連動watchdog新增continue欄位
    void __fastcall SetAllTemp_List(double *dTemperature);                      //Ztex 2024.08.24 Add Tri Temp Set All Temp By List
    void __fastcall Send_ATCSetPFParameter(int iOnOff, int iFullPower, AnsiString asPFSlope, AnsiString asWGain, int iMany2one);  //Hmy 20240207 Add Send Pf Par to Atc

    void __fastcall PFMode(bool bOnOff);
    void __fastcall GetPFCPARAMETER();                                          //讀取單一溫度的PF參數值
    void __fastcall SetPFCPARAMETER(AnsiString asSetData);                      //設定單一溫度的PF參數值
    void __fastcall GetSLOPEOFFSET();                                           //Get the Slope/Offset value   //Eliot 20210412
    void __fastcall SetSLOPEOFFSET(AnsiString asSetData);                       //Set the Slope/Offset value   //Eliot 20210412
    void __fastcall ATCCONTROLMODEMode(AnsiString asSetData);
    void __fastcall GetVOLTAGE();
    void __fastcall RECORDTJTEMP(int iStart);
    void __fastcall QUERYTJTEMP();
    void __fastcall ATCGETCONTROLMODE();
    void __fastcall TesterChangeTemp(int iTemp);    //wei 20230525
    void __fastcall SetTempPID(double dTemp, bool bChange, double dTemp1=0.0);
    void __fastcall SetTCWaterValve(AnsiString asWaterValueGPIB, int iWaterValue);
    void __fastcall ReadTCWaterValue();
    void __fastcall SetDynamicPID(AnsiString asSetData);
    void __fastcall ReadDynamicPID();
    void __fastcall ChangeTJMode();
    void __fastcall SetTC2Offset(int iChCount, double *dOffset);                //KenHsieh 20240311 : add Tc2 Offset
    void __fastcall EnablesMultiSensor(bool *Enabled);                          //Ifor 20251203 add: ATC MultiSensor
    void __fastcall SetMultiSensorOffset(int iChCount, double *dOffset);        //Ifor 20251203 add: ATC MultiSensor
    
};
//---------------------------------------------------------------------------
extern PACKAGE TATC_InterfaceForm *ATC_InterfaceForm;
//---------------------------------------------------------------------------
typedef struct {
    double dChillerSetTemp;                                                     //chililer
    double iPID[8][3];                                                          //PID   Site:0-8   0P 1: I 2:D
//    double iOffset[8];         //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    double iOffset[ATC_MAX_SITE];//Ztex 2023.04.19 Add HT-1032 TriTemp Function
    bool   GET_2ND_STATUS;
    double ATC_GET_GrandBand[16];
    double ATC_READ_WATER_Value[8];
    int ATC_READ_FUNCTION_STATUS;                                               //1: link
    char SWVer[20];                                                             //版號
} TATC_Data ;
extern TATC_Data ATC_Data;                                                      //ATC_Data.ATC_GET_2ND_STATUS;
extern int TriTemp_Ch[ATC_MAX_SITE];                                            //Ztex 2023.04.19 Add HT-1032 TriTemp Function
extern bool bATCBusy;//Ztex 2023.04.19 Add HT-1032 TriTemp Function
#endif
