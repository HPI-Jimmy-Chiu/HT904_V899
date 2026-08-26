//---------------------------------------------------------------------------
#ifndef databaseH
#define databaseH
//---------------------------------------------------------------------------
#include "MyBinDisp.h"
#include <Classes.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <DB.hpp>
#include <map>
#include <vector>
#include "myTimer.h"
using namespace std;

#include "uHGemClass.h"

typedef struct TIODATA
{
    int Tag;
    AnsiString _CommaText;
    AnsiString Type;
    AnsiString Alias;
    int iLane;
    int iModuleType;
    int iIP;
    int iPort;
    int iBit;
    int iInType;
    int iISABase;
    int iEnable;
    int iOnAlarmTime;
    int iOffAlarmTime;
    int iOnDelayTime;
    int iOffDelayTime;
    bool bDelete;

    TIODATA(AnsiString Str=AnsiString(""));
}TIODATA;
//---------------------------------------------------------------------------
typedef struct TIOTABLENO
{
    int eioType;
    int eioAlias;
    int eioLane;
    int eioModuleType;
    int eioIP;
    int eioPort;
    int eioBit;
    int eioInType;
    int eioISABase;
    int eioEnable;
    int eioOnAlarmTime;
    int eioOffAlarmTime;
    int eioOnDelayTime;
    int eioOffDelayTime;
    int eioNote;
    int eioTotal;
    int SetIOTableNo(AnsiString Str);

    TIOTABLENO();
}TIOTABLENO;
//---------------------------------------------------------------------------
typedef struct TMOTDATA
{
    AnsiString _CommaText;
    AnsiString No;
    AnsiString Alias;
    int iEnable;
    int iBoardID;
    int iPort;
    int iIP;
    double dGearRatio;
    int iSoftLimitN;
    int iSoftLimitP;
    AnsiString CardModel;

    int iDirection;
    int iHomeDirectior;
    double dAcc;
    double dDec;
    int iInitSpeed;
    int iHomeHighSpeed;
    int iHomeLowSpeed;
    int iJogHighSpeed;
    int iJogLowSpeed;
    int iRange;
    int iRate;
    int iServoAlarmOn;
    int i1P2P;
    int iSensorType;
    int iEncodeType;
    int iPickLimit;
    int iLimitLogic;
    int iIn1Logic;
    int iSimulateSpeed;

    TMOTDATA(AnsiString Str=AnsiString(""));
}TMOTDATA;
//---------------------------------------------------------------------------
typedef struct TMOTNO
{
    int emotNo;
    int emotAlias;
    int emotEnable;
    int emotBoardID;
    int emotPort;
    int emotIP;
    int emotGearRatio;
    int emotSoftLimitN;
    int emotSoftLimitP;
    int emotCardModel;
    int emotDirection;
    int emotHomeDirectior;
    int emotAcc;
    int emotDec;
    int emotInitSpeed;
    int emotHomeHighSpeed;
    int emotHomeLowSpeed;
    int emotJogHighSpeed;
    int emotJogLowSpeed;
    int emotRange;
    int emotRate;
    int emotServoAlarmOn;
    int emot1P2P;
    int emotSensorType;
    int emotEncodeType;
    int emotPickLimit;
    int emotLimitLogic;
    int emotIn1Logic;
    int emotSimulateSpeed;
    int emotTotal;

    TMOTNO();
    int SetMOTTableNo(AnsiString Str);
}TMOTNO;
//---------------------------------------------------------------------------
//#include "MachineDefine.h"
//---------------------------------------------------------------------------
class TDataModule1 : public TDataModule
{
__published:    // IDE-managed Components
    TTable *MotorTable;
    TTable *CylinderTable;
    TTable *SensorTable;
    TTable *SwitchTable;
    TTable *SuckerTable;
    void __fastcall DataModuleDestroy(TObject *Sender);
private:    // User declarations
public:     // User declarations
    __fastcall TDataModule1(TComponent* Owner);

   //   password part,user level
   void __fastcall ShowUserData();
   void __fastcall NewUser();
   void __fastcall StartNewUser();
   void __fastcall ChoiceCell(int Col,int Row);
   void __fastcall DeleteUser();

   int iNewUserTask;
   int SelectRow;
   int SelectCol;
   int FactUserCT;
   //   function access level
   void __fastcall ShowAccessLevel();
   void __fastcall ShowAccessLevelBmpFile(int);
   void __fastcall ChangeAccessLevel(int);
   // motor parameter
   void __fastcall MotorUp();
   void __fastcall MotorDown();
   void __fastcall MotorDefault();
   void __fastcall MotorPost();
   void __fastcall ReadMotorDirect();
   void __fastcall WriteMotorDirect();
   void __fastcall MotorFirst();
   void __fastcall UpdateMotorPic();
   //communction interface
   void __fastcall UpdateCommSet();

   void __fastcall InputPassword();
   void __fastcall ResetUserAccessLevel();
   void __fastcall ShowMaxLimit();
   void __fastcall SaveMaxLimit();
   void __fastcall ClearAlarmCT();
   void __fastcall RecordSystemRecord(char *s);
};
//---------------------------------------------------------------------------
extern PACKAGE TDataModule1 *DataModule1;
//---------------------------------------------------------------------------
//==============================================================================
// 筿北ンのノ把计栋
// Modify , 2010.03.12 , Joye : ヘ玡Τ放北竟家舱
//==============================================================================
class SYSTEM_MODULAR
{
    private:
        // ノㄓэ礶玂痙ぇ挡篶,
        double dNumberPanelDelay;       //Sam 20240604 : 陪ボ竟近ǖ丁э double
        bool bUseSocketTemp;
    protected:
//        void ChoiceTempController( int iType);
        void InstallColorBinDisplay(int iType);
    public:
//        TMyTempCtrl *TempCtrl;           // 放北竟
        TMyBinDispCtrl  *BinDisCtrl;      // 眒︹琿陪ボ竟
        _fastcall SYSTEM_MODULAR::SYSTEM_MODULAR();
        _fastcall SYSTEM_MODULAR::~SYSTEM_MODULAR();
        TQPF_Timer SysTimer;                //Steven 20190714 : 璸衡╰参丁
        void SystemModularInitial();
        AnsiString sNumberPanelComPort; //Steven 20120217 : Com PortэΘ﹚竡
        AnsiString sNumberPanelComPort2;
        AnsiString sTempComPort;
        AnsiString sTempOmronComPort;
        AnsiString sTempDynamicComPort;
        AnsiString sRTCComPort;
        AnsiString sTorqueComPort;
        AnsiString asATC1ComPort;
        AnsiString asATC2ComPort;
        AnsiString asATC3ComPort;
        AnsiString asATC4ComPort;
        AnsiString TrayStepMotor_ComPort;
        AnsiString sFinePitchComPort;
        AnsiString sFinePitchAdjustmentComPort;
        AnsiString asVisionLightPort;  //RogerYang 20180901 add 打Demo AI CCD Function

        AnsiString asATCSYSTEMIP;
        int asATCSYSTEMPORT;
        int asATCSYSTEMUSEHEAT; //Ifor 20160506 :add New ATC Interface Use Heat Count

        AnsiString asLASER_COM[4];          //Eastsun 20260525 MissF-3 : +OutArm COM    //==> Eastsun 20260525 laser 俱 MissF-3 : asLASER_COM[3] -> [4] for OutArm COM
        AnsiString asBarCodeComPort[4];
        int BarcodeBaudRate;
        int InBarcodeBaudRate;
        int BarcodeByteSize;
        int BarcodeStopBit;
        AnsiString BarcodeParity;

        AnsiString asRFIDCom;
        int  iRFIDBaudRate;              //Steven 20220713 : RFID Reader for SJSEMI
        int  iRFIDByteSize;
        int  iRFIDStopBit;
        AnsiString sRFIDParity;

        AnsiString asOCRComPort;
        AnsiString sTempOmronCom4Port;  //kevin 20130520 4 Dut 放北竟
        AnsiString asOCRwithTesterComPort;
        AnsiString asAirConPort;
        AnsiString asGroundManComPort;  //Steven 20190828 : 硄癟ΑGroundMan
        int iGroundManScanPoint;        //KaiChen 20191005 硄癟ΑGroundMan
        int iGroundManAlarmOhm;         //KaiChen 20191005 硄癟ΑGroundMan
        HTGem *MyGem;

        AnsiString asCCDBarCodeIP[4];       //Ifor 20151224 :add
        AnsiString asCCDBarCodePort[4];     //Ifor 20151224 :add

        map<AnsiString, AnsiString>mapATCAlarmCode;                 //Steven 20160706 : Add ATC alarm code
        map<AnsiString, AnsiString>::iterator mapATCAlarmCodeIter;  //Steven 20160706 : Add ATC alarm code
        void LoadIoData();
        void LoadMotData();
        TIOTABLENO IoNo;
        TMOTNO     MotNo;
        vector <TIODATA *>IOTable;
        map<AnsiString, AnsiString>mapIOTable;
        map<AnsiString, AnsiString>::iterator mapIOTableIter;
        vector <TMOTDATA *>MotTable;
        map<AnsiString, AnsiString>mapMotTable;
        map<AnsiString, AnsiString>::iterator mapMotTableIter;

        void ReadGeneralIni();              //Steven 20190320 : Handler Sys弄郎縒ミΘFunction

        AnsiString asCCDTrayIP[iTotalFunction];          //wei 20161219 Tray Mapping
        AnsiString asCCDTrayPort[iTotalFunction];        //wei 20161219 Tray Mapping

        AnsiString asFix2BGAAICCDIP[2];           //RogerYang 20180901 add 打Demo AI CCD Function
        AnsiString asFix2BGAAICCDPort[2];         //RogerYang 20180901 add 打Demo AI CCD Function

        AnsiString asRFIDComPort[2];        //wei 20180726 RFID
        int RFIDBaudRate;                   //wei 20180726 RFID
        int RFIDByteSize;                   //wei 20180726 RFID
        int RFIDStopBit;                    //wei 20180726 RFID
        AnsiString RFIDParity;              //wei 20180726 RFID
        //Sam 20180428 Auto Alignemnt
        //==>
        AnsiString asCCDAlignIP[4];
        AnsiString asCCDAlignPort[4];
        //<==
        //Sam 20180428 Auto Alignemnt
};
extern SYSTEM_MODULAR HSys;
#endif
