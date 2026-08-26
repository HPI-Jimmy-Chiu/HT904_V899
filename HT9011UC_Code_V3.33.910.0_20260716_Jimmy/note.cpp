#include "MachineDefine.h"
#pragma hdrstop

#include "note.h"

#include "MachineType.h"
#include "database.h"
#include "main.h"
#include "mysensor.h"
#include "myswitch.h"
#include "ckernel.h"
#include "mymotor.h"
#include "csystem.h"
#include "csetup.h"
//#include "ascanner.h"                                                         //Steven 20140409 : Mark Scanner
#include "uTrayEditForm.h"
#include "uhome.h"
#include "mycylin.h"
#include "cObserver.h"
#include "mymessbox.h"
#include "cMyDB.h"
#include "uLotInfo.h"
#include "atester.h"
#include "bthermo.h"
#include "MyCCLinkSensor.h"
#include "Password.h"
#include "iosetview.h"
#include "INPUT.h"
#include "myQwertyKeyBoard.h"
#include "automation.h"
#include "cContact.h"
#include "common.h"
#include "rs232.h"
#include "cprod.h"
#include "cinitial.h"
//#include "Fingerprint.h"
//#include "SECSGEM.h"
#include "Socket_ASE_KR.h"
#include "LtcSensor.h"
#include "FTPClient.h"                                                          //ChungHung 20140108 add FTP unload jam code
#include "cSecurity.h"
#include "uHGemHT9045.h"
#include "InterfaceSYS.h"
#include "atester_ProcessCount.h"
#include "uHeaterThread.h"
#include "cpublic.h"
#include "OCR.h"
#include "cmydef.h"
#include "InputForm.h"
#include "aoutarm.h"
#include "HS_Function.h"
#include "ATCInterface.h"
#include "ATC_Handler_Side.h"
#include "cShowBinSelect.h"
#include "SCK_WebService.h"
#include "SCK_ART.h"
#include "BarcodeReader.h"
#include "ProductionInfo.h"                                                     //Sam 20170809 (Steven) 移植超豐 OEE 功能 form HT-7045
#include "uESDControl.h"                                                        //Sam 20171018 (Steven) : OEE Bug 修正
#include "MyKitSuck.h"
#include "MN200.h"                                                              //Isaac 20181212 (Steven) : Baud Rate防呆功能
#include "fVATMesFileSys.h"                                                     //jou 20200409 : VTest Mes system
#include "uPadInterface.h"                                                      //Ken 20210702 AddPadInterface
#include "cTrayMapping.h"
#include "MyPLC_IO_Modbus.h"
#include "OmronEJ1N.h"
#include "AGV.h"
//==============================================================================
#pragma package(smart_init)
#pragma link "butPa1"
#pragma link "HTray"
#pragma resource "*.dfm"
TfNote *fNote;
//==============================================================================
extern bool bWriteErrRefFlag, bSupplyNewICTrayPause;
extern bool bHangTimePause;                                                     //Steven 20090827 : Hang Up dectector
static bool bAutoCount_Reset=false;

static DWORD PassTime;
static AnsiString BackRecordLastHappenTimeString, BackRecordLastHappenErrorCode ,err;

int iEventID;
int iPosition;
int iDuplicateError=0;
int mess_jam=0, mess_err=1, mess_fuc=2;
int iLogTorque1=-1;                                                             //kevin 20211207 MCU Stop
int iStopLogTorque1=-1;                                                         //kevin 20211207 MCU Stop

bool bShowBigDescription=false;
bool bShowSpecialPan    =false;                                                 //ChungHung 20110829
bool bErrPan_err        =false;                                                 //Richard 2011/2/22 SpecialPanel //2011.02.21 Q_Q 新增
char ErrorCode[256];

int iNoRTBinIdx=0;                                                              //RogerYang 20250604 偉測不可複測bin功能

DWORD RecordHappenTime,BackRecordHappenTime;
TPanel *FlushPanel=NULL;
TColor cDark =(TColor)0x00917B51;
TColor cLight=(TColor)0x00DFD9CC;

static bool IsBinCodeEditScanEnabled()
{
    return LastSet.iTester==ON_LINE &&
           (IniConfig.bP53_ForcedScanBinCodeOfUnloader ||
            IniConfig.bA69_ScanBinLabelToBinCodeEdit);
}

//==============================================================================
static bool ShouldAutoScanBinLabelToEdit()
{
    return USE_COVER_TRAYID &&
           (IniConfig.bA69_ScanBinLabelToBinCodeEdit ||
            (IniConfig.bA65_BundleIDList &&
             IniConfig.bP53_ForcedScanBinCodeOfUnloader));
}

AnsiString RecordLastHappenTimeString, RecordLastHappenErrorCode;
AnsiString Recovery;
AnsiString Pwd;                                                                 //Richard 2011/2/22 SpecialPanel //2011.02.21 Q_Q 新增
AnsiString ASE_aJamCode="";                                                     //kevin 20150818 ASE alarm send  war011
AnsiString ASE_aMessage="";                                                     //kevin 20150818 ASE alarm send  No tray
AnsiString ASE_aMode="";                                                        //kevin 20150818 ASE alarm send   Load tray
AnsiString ASE_aDate="";                                                        //kevin 20150818 ASE alarm send   20150818 15:00
AnsiString strBackUpCode=0;
AnsiString JamArea[]={"Input Arm",
                      "Output Arm",
                      "Index Unit",
                      "Input Shuttle",
                      "Output Shuttle",
                      "Empty Tray Arm",
                      "Tester I/F",
                      "Scanner",
                      "Tray Loader",
                      "Empty Tray",
                      "Tray Unloader 1",
                      "Tray Unloader 2",
                      "Tray Unloader 3",
                      "Color Tray",
                      "Temp. Controller",
                      "System",
                      "Fix Tray 1",
                      "Fix Tray 2",
                      "Fix Tray 3",
                      "ESD",
                      "Process",
                      "Motion",
                      "Cassette",
                      "Motor",
                      "Tray Unloader 4",
                      "Tray Unloader 5",
                      "Tray Unloader 6",
                      "Fix Tray 4",
                      "Fix Tray 5",
                      "Fix Tray 6",
                     };
TEdit *myBinCodeEdit[TEST_MAX_BIN];                                             //JerryYang 20240111 : add P53 function
bool bCheckBinOK=false;
TDateTime LastInputTime;                                                        //JerryYang 20250429 : fix Auto In/Out
bool IsScanning=false;
//==============================================================================
__fastcall TfNote::TfNote(TComponent* Owner)
    : TForm(Owner)
{
    fShow=false;
    fShowMessage            =false;                                             //Chunghung 20120110 add
    pCanvas                 =new TCanvas;
    bNeedPassWord           =false;
    bCloseShowMsg           =false;                                             //Sam 20230426 : 通知系統 Handler 已經密碼鎖定
    bMyServoOffInArm        =false;
    bMyServoOffOutArm       =false;
    iMyServoOffInArmPosX    =0;
    iMyServoOffInArmPosY    =0;
    iMyServoOffOutArmPosX   =0;
    iMyServoOffOutArmPosY   =0;
    palRedNotice[ 0]        =palCheckLoader;
    palRedNotice[ 1]        =palSGCheckList;
    palRedNotice[ 2]        =palCheckSht;
    palRedNotice[ 3]        =PanSpecialNote;
    palRedNotice[ 4]        =pnlIndex1Error;
    palRedNotice[ 5]        =pnlIndex2Error;
    palRedNotice[ 6]        =pnlContact;
    palRedNotice[ 7]        =pnlCleanSocket;
    palRedNotice[ 8]        =palWrongPW;
    palRedNotice[ 9]        =pnlContactOver;
    palRedNotice[10]        =pnlOutArmDrop;
    palRedNotice[11]        =pnlPan_TriMachineSpecialNote;
    for(int i=0; i<12; i++)
    {
        palRedNotice[i]->Top=290;
        if(AUTO_EMPTY_COLOR>=3)
            palRedNotice[i]->Left=2;
        else
            palRedNotice[i]->Left=160;
    }
    palWrongPW->Top=130;

    bMyServoOffOutShuttle1   =false;                                            //ChungHung 20140522 add OutShuttle lose devices can servo off
    bMyServoOffOutShuttle2   =false;
    iMyServoOffOutShuttle1Pos=0;
    iMyServoOffOutShuttle2Pos=0;

    TMyTray1->SetColorMap(0, clWhite);
    TMyTray1->SetColorMap(1, cLight);
    TMyTray1->SetColorMap(2, clGreen);
    TMyTray1->SetColorMap(3, clRed);

    t2DCode->SetColorMap(0, clWhite);
    t2DCode->SetColorMap(1, cLight);
    t2DCode->SetColorMap(2, clGreen);
    t2DCode->SetColorMap(3, clRed);

    bOpenChamberDoor            =true;
    bOpenLeftDoor               =false;
    bOpenSixDoor                =false;                                         //kevin 20160725 bin error 需開右後方門確認
    bPLCFlag=false;                                                             //KenHsieh 20250307 : fix PLC safedoor 通訊延遲問題

    KeyCode                     =0;
    AlarmType                   =0;
    iSameAlarmCT                =0;                                             //jou 2014-09-04 Continuous Same Alarm N time Need KeyIn Password
    fMain->Timer1->Enabled      =true;                                          //kevin 20120718      Form 尚未建立 不能馬上使用
    bOpenAllDoor                =true;                                          //wei 20160407 Alarm 後需要開門確認
    pnlCorrectionCount->Visible =false;
    btPrintSummary->Visible     =false;

    for(int i=0; i<pgcNote->PageCount; i++)
    {
        pgcNote->Pages[i]->TabVisible=false;
    }
    pgcNote->ActivePageIndex=0;
    pgcNote->ActivePage=tsHandler;

    for(int i=0; i<TEST_MAX_BIN; i++)                                           //JerryYang 20240111 : add P53 function
    {
        myBinCodeEdit[i]=new TEdit(this);
        myBinCodeEdit[i]->Parent=sbBinEdit;
//        myBinCodeEdit[i]->Align=alLeft;
        myBinCodeEdit[i]->Width=150;                                            //JerryYang 20241118 : fix
        myBinCodeEdit[i]->Height=20;
        myBinCodeEdit[i]->Top=20;
        myBinCodeEdit[i]->Left=0;
        myBinCodeEdit[i]->Visible=false;
        myBinCodeEdit[i]->OnKeyPress=edBinCodeEditKeyPress;                     //JerryYang 20241118 : fix
        myBinCodeEdit[i]->Enabled=true;
    }

    if(USE_COVER_TRAYID==tCID_Keyence)
        timerKeyence->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TfNote::FormDestroy(TObject *Sender)
{
    delete pCanvas;                                                             //Steven 20160108 : release memory
    listO17->Clear();
    listContAlarmTimer->Clear();                                                //Sam 20231116 : 連續 Alarm 不要上傳伺服器
    listContAlarmList->Clear();                                                 //Sam 20231116 : 連續 Alarm 不要上傳伺服器
    delete listO17;
    delete listContAlarmTimer;                                                  //Sam 20231116 : 連續 Alarm 不要上傳伺服器
    delete listContAlarmList;                                                   //Sam 20231116 : 連續 Alarm 不要上傳伺服器
    LogSoftwareOffTime("TNote, FormDestroy");                                   //Steven 20210526 : 紀錄軟體執行時間
}
//------------------------------------------------------------------------------
void __fastcall TfNote::Reset()
{
    pgcNote->ActivePageIndex=0;
    pgcNote->ActivePage     =tsHandler;

    for(int i=0; i<12; i++)
    {
        palRedNotice[i]->Parent=tsRedAlarm;
    }

    t2DCode->Parent =ts2DID;                                                    //Steven 20240512 : fixed for Note display
    pnlLotInfo->Parent  =tsLotInfo;

    palInArm    ->Color=cDark;                                                  //jou 980316 FlushPanel before set and after set null error
    palHead0    ->Color=cDark;
    palHead1    ->Color=cDark;
    palInSh1    ->Color=cDark;
    palInSh2    ->Color=cDark;
    palOutSh2   ->Color=cDark;
    palOutSh1   ->Color=cDark;
    palOutArm   ->Color=cDark;
    palTrayArm  ->Color=cDark;
    palSys      ->Color=cDark;
    palIF       ->Color=cDark;
    palEmpty    ->Color=cDark;
    palColor    ->Color=cDark;
    palLoad     ->Color=cDark;
    palAuto1    ->Color=cDark;
    palAuto2    ->Color=cDark;
    palAuto3    ->Color=cDark;
    palFix1     ->Color=cDark;
    palFix2     ->Color=cDark;
    palFix3     ->Color=cDark;
    palLoad_Car ->Color=cDark;
    palEmpty_Car->Color=cDark;
    palColor_Car->Color=cDark;
    palAuto1_Car->Color=cDark;
    palAuto2_Car->Color=cDark;
    palAuto3_Car->Color=cDark;
    palSafeDoor1->Color=cDark;
    palSafeDoor2->Color=cDark;
    palSafeDoor3->Color=cDark;
    palSafeDoor4->Color=cDark;
    palSafeDoor5->Color=cDark;
    palSafeDoor6->Color=cDark;
    palSafeDoor7->Color=cDark;
    palSafeDoor8->Color=cDark;
    palSafeDoor9->Color=cDark;
    palManualAll->Color=cDark;                                                  //Steven 20120131 : Fix Tray置偏偵測
    palHead     ->Color=clBlue;
    palOutSh    ->Visible=false;
    palInSh     ->Visible=false;
    palTemp     ->Visible=false;
    palScan     ->Visible=false;
    palInSh1    ->Visible=true;
    palInSh2    ->Visible=true;
    palHead     ->Left   =229;

    palIonFan01->Visible=false;                                                 //Steven 20130205 : 離子風扇異常顯示
    palIonFan02->Visible=false;
    palIonFan03->Visible=false;
    palIonFan04->Visible=false;
    palIonFan05->Visible=false;
    palIonFan06->Visible=false;
    palIonFan07->Visible=false;
    palIonFan08->Visible=false;
    palIonFan09->Visible=false;
    palIonFan10->Visible=false;
    palIonFan11->Visible=false;
    palIonFan12->Visible=false;

    pnlATC_Handler->Color        =clTeal;                                       //Ifor 20160830 ATC 異常顯示
    pnlATC_TCPIP->Color          =clTeal;
    pnlATC_NI->Color             =clTeal;
    pnlATC_ATC->Color            =clTeal;
    pnlATC_Chiller->Color        =clTeal;
    pnlATC_RS232->Color          =clTeal;
    pnlATC_Head->Color           =clTeal;
    pnlATC_PowerSupply->Color    =clTeal;
    pnlATC_WaterValve->Color     =clTeal;
}
//------------------------------------------------------------------------------
bool CheckRecordJamType(AnsiString asJamCode, bool bCheckOnly=false)
{
    int iUnitNo;
    bool bResult=false, bAddCount=false;
    AnsiString CodeBuffer;

    CodeBuffer=asJamCode.UpperCase();
    iUnitNo=atoi(AnsiString(asJamCode.SubString(4, 2)).c_str());

    bool bIncludAllUnit=(CUSTOMER_CODE==CC_AnalogDevice_Phil);                  //JerryYang 20230721 : Analog要求修改

    if(CosFunction.bIncludeMTBA)
    {
        if(LastSet.iRealDummy==REALLY)
            bAddCount=fSecurity->GetJemIncludeMTBA(fNote->sJamArea, fNote->sJamCode);
    }
    else
    {
        if(CodeBuffer.Pos("JAM")>0 &&
           (iUnitNo<9 || bIncludAllUnit==true) &&                               //Steven 20101102 : 改用UnitNo來判別要不要記入Jam Rate
           LastSet.iRealDummy==REALLY)                                          //JerryYang 20230721 : Analog要求修改
        {
            if(bCheckOnly==false)                                               //Steven 20140528 : Secs Gem
            {
                bAddCount=true;
            }
        }
    }

    if(bAddCount)
    {
        for(int i=0; i<3; i++)
            LastSet.iJamCount[i]++;

        LastSet.iDayJamCount++;                                                 //jou 20210108 : 上海偉測要求新增每日jam rate統計
        iRecordJamRateByTime_JamCount++;                                        // 2015.11.11 , Joye , Add Jam Rate Record

        if(IniConfig.bVTESTFunction==true && LastSet.iTester==ON_LINE &&        //marvin 20200424 (Kirin) Added always record report by time.
           RunInfo.bLotStart==true && IniConfig.bCheckFile==true)
        {
            fMesSystem->iJamRateTotalForAlways++;
        }
        bResult=true;
    }
    return bResult;
}
//---------------------------------------------------------------------------
void CheckRecordJamDrivingRecord(AnsiString asJamCode)                          //wei 2013-12-09 JAM偵測檢查
{
    AnsiString Code;

    Code=asJamCode.UpperCase();
    if(Code.Pos("JAM")>0)
    {
        if(IniConfig.bC09_CarRecord)
        {
            SW[SwCarRecord].On();
            bCarRecordTimeStart=true;
        }
        else if(CUSTOMER_CODE==CC_KYEC_LEE &&
                (USE_AUTO_RETEST==eartInstall ||
                 CosFunction.bUseMRTMode==true ||                               //wei 20160407 Alarm Message
                 CosFunction.bUseARTSortCount==true))                           //Ifor 20170322 add Jam Skip (改到下個版本)
        {
            if(Code=="JAM0109" || Code=="JAM0112" || Code=="JAM0126" || Code=="JAM0128" ||
               Code=="JAM0201" || Code=="JAM0202" || Code=="JAM0203" || Code=="JAM0210" ||
               Code=="JAM0301" || Code=="JAM0302" || Code=="JAM0303" || Code=="JAM0304" ||
               Code=="JAM0312" || Code=="JAM0313" || Code=="JAM0314" || Code=="JAM0315" ||
               Code=="JAM0508" || Code=="JAM0509" || Code=="JAM0407")           //JerryYang 20160516 add JAM0312~0315 JAM0128
               {
                    if(Code=="JAM0303" || Code=="JAM0304" || Code=="JAM0508" || Code=="JAM0509")
                    {
                        bAutoCount_Reset=true;
                    }
                    bAutoRetestJam=true;                                        //wei 20160302 Jam Skip輸入顆數
                    #ifndef SOFT_SIMULTE
                    bOpenAllDoor=false;                                         //wei 20160407 Alarm 後需要開門確認
                    #endif
               }
        }
    }
    else if(CUSTOMER_CODE==CC_KYEC_JCTHIU || CUSTOMER_CODE==CC_KYEC_LEE)        //Frank 20260710 ADD//Eastsun 20260710整合
    {
        if(Code=="JAM0303" || Code=="JAM0304")
        {
            if(fContact->fShow==true)
                iHome=1;
            else
                iResetStopMachine=1;
        }
    }
    else if(Code=="MES0101")                                                    //Ifor 20171228 : add MES0101 Loader Pick Up Error 處置
    {
        if(CUSTOMER_CODE==CC_KYEC_LEE &&
           (USE_AUTO_RETEST==eartInstall ||
            CosFunction.bUseMRTMode==true ||
            CosFunction.bUseARTSortCount==true))
        {
            bNeedKeyInSkipIC=true;
            bAutoRetestJam=true;
        }
    }
}
//---------------------------------------------------------------------------
void DoCheckTemperatureDuringIndexTesting(AnsiString asJamCode)                 //Jimmychiu 20240916 : Check the temperature during index arm testing
{
    if(CosFunction.bCheckTempDuringIndexArmTesting==false ||
       IniConfig.bI54_Enable==false &&
       myIAR_Test.iStartTest==0)                                                //Steven 20250102 : fixed for [I54]
    {
        return;
    }

    int iSuckRow=0, iSuckCol=0;
    bool bHasErr=false;
    AnsiString sCode=asJamCode.UpperCase();
    AnsiString sOverLow[MAX_SOCKET_ROW][MAX_SOCKET_COL]={{"WAR1511", "WAR1512", "WAR1513", "WAR1514", "WAR1533", "WAR1534", "WAR1535", "WAR1536"},
                                                         {"WAR1515", "WAR1516", "WAR1517", "WAR1518", "WAR1533", "WAR1538", "WAR1539", "WAR1540"},
                                                         {"WAR1519", "WAR1520", "WAR1521", "WAR1522", "WAR1541", "WAR1542", "WAR1543", "WAR1544"},
                                                         {"WAR1523", "WAR1524", "WAR1525", "WAR1526", "WAR1545", "WAR1546", "WAR1547", "WAR1548"}};

    AnsiString sOverHigh[MAX_SOCKET_ROW][MAX_SOCKET_COL]={{"WAR15111", "WAR15112", "WAR15113", "WAR15114", "WAR15133", "WAR15134", "WAR15135", "WAR15136"},
                                                          {"WAR15115", "WAR15116", "WAR15117", "WAR15118", "WAR15133", "WAR15138", "WAR15139", "WAR15140"},
                                                          {"WAR15119", "WAR15120", "WAR15121", "WAR15122", "WAR15141", "WAR15142", "WAR15143", "WAR15144"},
                                                          {"WAR15123", "WAR15124", "WAR15125", "WAR15126", "WAR15145", "WAR15146", "WAR15147", "WAR15148"}};

    for(int iRow=0; iRow<MAX_SOCKET_ROW; iRow++)
    {
        for(int iCol=0; iCol<MAX_SOCKET_COL; iCol++)
        {
            if(sOverLow[iRow][iCol]==sCode ||
               sOverHigh[iRow][iCol]==sCode)
            {
                bHasErr=false;
                if(IsNNMode()==NN_1Row)
                {
                    if(myIAR_Test.iStartTest==3)
                        bHasErr=true;

                    if(iRow==1)
                        iSuckRow=0;
                    else
                        iSuckRow=1;
                }
                else if(IsNNMode()==NN_2Row)
                {
                    if(myIAR_Test.iStartTest==3)
                        bHasErr=true;

                    if(iRow==2 || iRow==3)
                        iSuckRow=iSuckRow-2;
                    else
                        iSuckRow=iSuckRow+2;
                }
                else
                {
                    if(myIAR_Test.iStartTest==2)
                    {
                        if(iRow==2 || iRow==3)
                        {
                            bHasErr=true;
                            iSuckRow=iRow-2;
                        }
                    }
                    else if(myIAR_Test.iStartTest==1)
                    {
                        if(iRow==0 || iRow==1)
                        {
                            bHasErr=true;
                            iSuckRow=iRow;
                        }
                    }
                }

                if(bHasErr)
                {
                    if(TestIF_File.bUse32Heater==false &&
                       (TestIF_File.iTestMode==_10Site2X5 ||
                        TestIF_File.iTestMode==_12Site2X6 ||
                        TestIF_File.iTestMode==_16Site2X8 ||
                        TestIF_File.iTestMode==_32Site4X8N ||
                        TestIF_File.iTestMode==_32Site4X8M))
                    {
                        iSuckCol=iCol*2;
                        myIAR_Test.SetTempErr(iSuckRow, iSuckCol);
                        myIAR_Test.SetTempErr(iSuckRow, iSuckCol+1);
                    }
                    else
                    {
                        iSuckCol=iCol;
                        myIAR_Test.SetTempErr(iSuckRow, iSuckCol);
                    }
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
int ShowErrorMessage(AnsiString Code, int KCode, int Pos, bool bDuplicateErr, AnsiString errPart)                       //Steven 20091231 顯示整合的錯誤訊息
{
    AnsiString Str;
    bool bShowATCAlarm=false;
    int iSendAlarmCode=0;

    if(InitialOK==false)                                                        //Steven 20250310 : Add protection
    {
        MyDBIProcess("Exception", Code, errPart);
        return 0;
    }

    if(TestIF_File.iTestMode==_16Site2X8 && TestIF_File.bUse32Heater==false &&  //JerryYang 20251201 : 優化alarm顯示
       TestIF_File.iSiteMap[0][1]==0 && TestIF_File.iSiteMap[0][3]==0 && TestIF_File.iSiteMap[0][5]==0 && TestIF_File.iSiteMap[0][7]==0 &&
       TestIF_File.iSiteMap[1][0]==0 && TestIF_File.iSiteMap[1][2]==0 && TestIF_File.iSiteMap[1][4]==0 && TestIF_File.iSiteMap[1][6]==0)
    {
        if(Code=="WAR15111")                                                    //加熱點Aa -> Site Aa
        {
            Code="WAR15111";
        }
        else if(Code=="WAR15112")                                               //加熱點Ab -> Site Ac
        {
            Code="WAR15113";
        }
        else if(Code=="WAR15113")                                               //加熱點Ac -> Site Ac
        {
            Code="WAR15133";
        }
        else if(Code=="WAR15114")                                               //加熱點Ad -> Site Ag
        {
            Code="WAR15135";
        }
        else if(Code=="WAR15115")                                               //加熱點Ba -> Site Bb
        {
            Code="WAR15116";
        }
        else if(Code=="WAR15116")                                               //加熱點Bb -> Site Bd
        {
            Code="WAR15118";
        }
        else if(Code=="WAR15117")                                               //加熱點Bc -> Site Bf
        {
            Code="WAR15138";
        }
        else if(Code=="WAR15118")                                               //加熱點Bd -> Site Bh
        {
            Code="WAR15140";
        }

        if(Code=="WAR15119")                                                    //加熱點Aa -> Site Aa    //Arm1 low
        {
            Code="WAR15119";
        }
        else if(Code=="WAR15120")                                               //加熱點Ab -> Site Ac
        {
            Code="WAR15121";
        }
        else if(Code=="WAR15121")                                               //加熱點Ac -> Site Ae
        {
            Code="WAR15141";
        }
        else if(Code=="WAR15122")                                               //加熱點Ad -> Site Ag
        {
            Code="WAR15143";
        }
        else if(Code=="WAR15123")                                               //加熱點Ba -> Site Bb
        {
            Code="WAR15124";
        }
        else if(Code=="WAR15124")                                               //加熱點Bb -> Site Bd
        {
            Code="WAR15126";
        }
        else if(Code=="WAR15125")                                               //加熱點Bc -> Site Bf
        {
            Code="WAR15146";
        }
        else if(Code=="WAR15126")                                               //加熱點Bd -> Site Bh
        {
            Code="WAR15148";
        }
    }

    if(IniConfig.bA32EnableFTPAutomation==true &&
       FTPAutomation_Up_ErrorCodePath!="")                                      //KaiChen 20190530 ：Sigurd FTP Automation
    {
        if(CUSTOMER_CODE==CC_UTAC_TW)                                           //Sam 20231205 : 聯測先不要上傳
        {
        }
        else if(Code.Pos("JAM")>0 || Code.Pos("WAR")>0)
        {
            #ifdef SOFT_SIMULTE
            fLotInfo->AlarmCodeUpload(Code);
            #endif
        }
    }

    if(Code.Length()==8)
    {
        bool bATCAlarmNeedDisconnet=true;
        iSendAlarmCode=StrToFloat(Code.SubString(4,Code.Length()-3));           //Ifor 20160720 取得Alarm Code 數字
        if(iSendAlarmCode>=15000 && iSendAlarmCode<=15400)                      //Ifor 20160729 Handler所有溫度Alarm
        {
            if(CUSTOMER_CODE==CC_KYEC_LEE &&
               (CosFunction.bHiSiliconFunction==true || USE_BU5_Function==true)) //Eastsun 20260710 Merge                            //Ifor 20181219 : add KYEC 要求溫度相關Alarm 測試中需先Index 上升才Alarm
            {
                if(iSendAlarmCode>=15200 && iSendAlarmCode<=15354)              //Ifor 20160726 add 所有ATC Alarm
                {
                    if(bInitStartDelayNotFinish==false)                         //Ifor 20181220 : add Init Start Delay Time Not Finish
                    {
                        if(bNeedWaitATCRunSelfTestFinish==true)                 //Ifor 20190201 add :ATC Self Test 執行中不Alarm
                            return 0;
                        if(bFinshTest==false)
                        {
                            bATCHasAlarmBinNeedToError=true;
                        }
                    }
                }
            }
            else if(CosFunction.bTempAlarmBinNeedToError &&                     //Steven 20251022 : Temp alarm need put to error bin
                    Temperature.bTempAlarmBinNeedToError)
            {
                if(iSendAlarmCode>=15000 &&
                   iSendAlarmCode<=15354 &&
                   bFinshTest==false)
                {
                    bATCHasAlarmBinNeedToError=true;
                }
            }
            else
            {
                if(iSendAlarmCode>=15200 && iSendAlarmCode<=15354)              //Ifor 20160726 add 所有ATC Alarm
                {
                    #ifdef HiSilicon
                        if(CUSTOMER_CODE==CC_ASE_KaohSiung &&
                           bFinshTest==false)                                   //kevin 20181009  add 發生 ATC 溫度異常時需將測中IC放至Error Bin for KYEC
                            bATCHasAlarmBinNeedToError=true;
                    #endif
                    if((CUSTOMER_CODE==CC_ASE_SG ||                             //Ifor 20250926 add:ASE SG要求測試中ATC 溫度異常需將測中IC放至Error Bin
                        CosFunction.bTempAlarmBinNeedToError) &&                //Steven 20251022 : Temp alarm need put to error bin
                       bFinshTest==false)
                    {
                        bATCHasAlarmBinNeedToError=true;
                    }
                }
            }

            if(iSendAlarmCode>=15310 && iSendAlarmCode<=15322)                  //Ifor 20160729 判斷ATC Alarm 是否需要斷線
            {
                if(iSendAlarmCode!=15319)                                       //ATC alarm: ATC temperature Index always same error
                {
                    bATCAlarmNeedDisconnet=false;                               //Ifor 20160728 ATC 非溫度Alarm 不斷線
                }
                bNeedWaitATCRunSelfTestFinish=false;                            //Ifor 20160726 ATC Self Test 清除time Out 計數時間
            }
            else
            {
                if(iSendAlarmCode>=15200 && iSendAlarmCode<=15299)              //Ifor 20160729 判斷是否為ATC Alarm
                {
                    bNeedWaitATCRunSelfTestFinish=false;
                }
                else
                {
                    if(bNeedWaitATCRunSelfTestFinish==true)                     //Ifor 20160729 非 ATC Alarm 一律卡掉
                        return 0;
                }
            }

            if(CosFunction.bHiSiliconFunction==true)                            //Ifor 20160907 海思版本ATC Alarm 需要斷線
            {
                if(iSendAlarmCode!=15309 && iSendAlarmCode!=15310 &&            //Ifor 20160729 工作擋Alarm & 斷線Alarm & Self Test OK 不做 Self test
                   iSendAlarmCode!=15311 && iSendAlarmCode!=15312 &&            //WAR15309~WAR15312 WAR15317 WAR15320~WAR15322不需要做Self Test
                   iSendAlarmCode!=15317 && iSendAlarmCode!=15320 &&
                   iSendAlarmCode!=15321 && iSendAlarmCode!=15322)
                {
                    if(Temperature.bATCActiveCooling==true &&                   //Ifor 20160802 無開啟ATC功能不需做 Self Test 功能
                       iSendAlarmCode>=15200)                                   //Ifor 20160808 非ATC Alarm 不做Self Test
                        bNeedSendATCRunSelfTest=true;                           //Ifor 20160720 add for ATC Safe Test
                }

                if(bATCAlarmNeedDisconnet)                                      //Ifor 20160728 整合 ATC Alarm 需要斷線
                {
                    if(ATC_SYSTEM==eATCHonPrecType)
                    {
                        //ATCInterfaceForm->OffLine();
                    }
                    else if(ATC_SYSTEM==eNewATCSystem)
                    {
                        //ATC_InterfaceForm->Disconnet();
                        ATC_InterfaceForm->bHandlerOnlineFlag=false;
                    }
                    fLotInfo->aldATCPower->Value=false;
                    fLotInfo->aldATCChillerStatus->Value=false;
                    bRunATC=false;
                    bATCTempAdjustmentOffset=false;
                    bSetTempChange=true;

                    fLotInfo->pl_ATC_Online->Caption="ATC Off Line";
                    fLotInfo->pl_ATC_Online->Color=clRed;
                    bStartATCRun=false;
                    bReSendATCLotEven=true;                                     //Ifor 20161118 ATC Alarm 斷線後需重新送Lot命令給ATC系統
                }

                if(iSendAlarmCode==15317)
                    FormHS->ShowATCSelfTestSatus(true,false);
                else
                    FormHS->ShowATCSelfTestSatus(true,true);
            }

            if(iSendAlarmCode>=15200 && iSendAlarmCode<=15400)                  //Ifor 20160829 add 判斷是否顯示ATC Alarm 畫面
            {
                bShowATCAlarm=true;
            }

            if((iSendAlarmCode>=15200 && iSendAlarmCode<15300) ||               //Ifor 20170331 (wei) Add 判斷式加一層(
               iSendAlarmCode==15319)                                           //Ifor 20160830 add ATC Alarm 位置顯示
                Pos=FormHS->ShowATCAlarmPosition(iSendAlarmCode);
            if(IniConfig.bL46_AStreamErrorCompressOnecycle)                     //Ztex 2024.10.01 Add AStream Error Compress Onecycle
            {
                if(iSendAlarmCode==15393)
                {
                    iAStreamErrorCompressOnecycle=1;
                    fMain->BtnOneCycleClick(fMain);
                }

                if(iSendAlarmCode==15394)
                {
                    iAStreamErrorCompressOnecycle=3;
                }
            }
        }
    }
    //JerryYang 20250220 : Mark掉, 移到下面
    //==>
    //if(bShowATCAlarm==true)                                                     //Ifor 20160830 Add 顯示ATC Alarm 畫面
    //{
    //    if(bShowTriTempAlarm_Msg==true)                                         //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    //    {
    //        bShowTriTempAlarm_Msg=false;
    //    }
    //    else
    //    {
    //        fNote->pgcNote->ActivePageIndex =6;
    //        fNote->pgcNote->ActivePage      =fNote->tsATC;
    //    }
    //}
    //if(Code=="MES1712" || Code=="MES1812" || Code=="MES1912")
    //{
    //    fNote->edBundleID->Text="";
    //    fNote->pgcNote->ActivePage=fNote->tsFixBundleID;
    //}
    //<==
    //JerryYang 20250220 : Mark掉, 移到下面
//    fNote->TempCode=Code;                                                       //JerryYang 20250311 : 移到下面,避免同時多個ALARM的時候TempCode被刷掉  //ChungHung 20111230 add CleanOut no music
    if(bStartRecordAseError && IniConfig.bASE_Report)                           //kevin 20140918 高雄日月光IC履歷記錄
    {
        if(aRecordAseError.Pos("na"))
            aRecordAseError="";
        aRecordAseError+=Code+";";                                              //kevin 20141028 紀錄發生 alarm
    }
    strBackUpCode=Code;
    bShowBigDescription=false;
    bOpenAllDoor=true;                                                          //wei 20160407 Alarm 後需要開門確認
    if(Code!="WAR1635")                                                         //Steven 20220309 : 避免Galil Command Error時, 不能Alarm
        StopAllMotor();
    else
        StopAllMotor(false);
    SoftStop=false;
    SoftStart=false;
    SystemStart=false;
    if(bDuplicateErr==true)
        iDuplicateError=1;
    #ifndef SOFT_SIMULTE
    else if(LastSet.iTester==OFF_LINE)                                          //Steven 20110121 : 不要把Offline的Jam也給列入報表中
        iDuplicateError=2;
    #endif
    else if(fContact->fShow==true)                                              //Steven 20110209 : Auto Height時也不算
        iDuplicateError=3;
    else if(bMaintanceMode)                                                     //Steven 20251007 : maintance mode for Hana
        iDuplicateError=4;
    else
        iDuplicateError=0;
    if(fNote->fShow)
    {
        MyDBIProcess("Exception", "Alarm at same time: "+Code, errPart);        //Steven 20250310 : Add protection
        return 0;
    }
    fNote->TempCode=Code;                                                       //JerryYang 20250311 : 移到下面,避免同時多個ALARM的時候TempCode被刷掉
    if(Code=="MES1712" || Code=="MES1812" || Code=="MES1912" ||                 //JerryYang 20250220 : 移到下面
       Code=="MES1713" || Code=="MES1813" || Code=="MES1913")                   //RogerYang 20250604 偉測不可複測bin功能
    {
        fNote->edBundleID->Text="";
        fNote->pgcNote->ActivePage=fNote->tsFixBundleID;
    }

    if(bShowATCAlarm==true)                                                     //JerryYang 20250226 : 機台已經Alarm了, 不要蓋ˇ
    {
        if(bShowTriTempAlarm_Msg==true)                                         //Ztex 2023.04.19 Add HT-1032 TriTemp Function
        {
            bShowTriTempAlarm_Msg=false;
        }
        else
        {
            fNote->pgcNote->ActivePageIndex =6;
            fNote->pgcNote->ActivePage      =fNote->tsATC;
        }
    }
    AnsiString Message, UnitName;
    int AlarmID, UnitNo, AxleNo;
    Code=Code.UpperCase();
    if(IniConfig.bC09_CarRecord || CUSTOMER_CODE==CC_KYEC_LEE)
    {
        CheckRecordJamDrivingRecord(Code);                                      //wei 2013-12-09
    }
    iEventID=MyDBIEvent(Code, 0, &AlarmID, &UnitNo, &AxleNo, &fNote->AlarmType, &Message, &UnitName, fMain->edWorkTemperBase->Text, iDuplicateError, errPart);
    ProductionLog(Message, true);                                               //JerryYang 20160217 for 矽品蘇州 Process Record也要存成文字檔
    if(IniConfig.bSPILFunction==true)                                           //Ifor 20160408 矽品客戶要求，發生Alarm上傳一次Log
    {
        bSysLotStart=false;
        FormHS->RecordLog_HS(true);
    }

    if(AlarmID==41)
    {
        MyDBIProcess("Message", "Unknow Alarm Code: "+Code);
    }

    if(errPart==" ")
    {
        if(iDuplicateError==1)  fNote->ErrShowToForm(Code, UnitName, Message+" (Again!!)", -1);
        else                    fNote->ErrShowToForm(Code, UnitName, Message, -1);
    }
    else
    {
        if(iDuplicateError==1)  fNote->ErrShowToForm(Code, UnitName, Message+" : "+errPart+" (Again!!)", -1);
        else                    fNote->ErrShowToForm(Code, UnitName, Message+" : "+errPart, -1);
    }

    if(Code!="WAR16125")                                                        //KenHsieh 20250307 : fix PLC safedoor 通訊延遲問題
        ShowErrorUnit(Pos);
    if(UnitNo>0 && UnitNo<=30)
        Str.sprintf("%02d %s", UnitNo, JamArea[UnitNo-1]);                      //Steven 20240614 : Fixed for JAM Area顯示
    else
        Str="";
    fNote->sJamArea=Str;
    fNote->sJamCode=Code;
    if((UnitNo>=1 && UnitNo<=5) || UnitNo==7)                                   //Steven 20110701
       // || UnitNo==15)                                                        //Steven 20251023 : mark
        fNote->ShowErrSite(Code, UnitNo, AxleNo, errPart);
    else
        fNote->TMyTray1->Visible=false;

    if(CosFunction.bOLPFunction)                                                //Steven 20141229 : OLP功能
    {
        bool bUpload=true;
        if(fSecurity->GetContAlarmNotUpload(fNote->sJamArea, fNote->sJamCode))  //Sam 20231116 : 連續 Alarm 不要上傳伺服器
        {
            bUpload=fNote->bCheckContAlarmNotUpload(fNote->sJamCode);
        }

        if(bUpload)
            fAutomation->DoCommandBuffer("ALARM_REQUEST", "", AnsiString(UnitName+":"+Message), fNote->AlarmType, Code);                                        //Steven 20110210
    }
    else if(CosFunction.bCreateManualEOCAP==true &&
            TestIF_File.bCreateManualEOCAP==true)                               //jou 20221104 : VTest CreateManualEOCAP function;
    {
        fNote->asMesMessage=Message;
    }

    fNote->KeyCode=KCode;
    AnsiString SData="";

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //kevin 20150818 20150724 add remote
    {
        ASE_aDate=FormatDateTime("yyyy/mm/dd hh:mm:ss", Now());
        ASE_aJamCode=Code;                                                      //kevin 20150818 ASE alarm send
        ASE_aMessage=Message+errPart;                                           //kevin 20170905 (Steven) ASE alarm send
        ASE_aMode=UnitName;                                                     //kevin 20150818 ASE alarm send
        if(fNote->sJamCode=="MES0101")                                          //kevin 202107015 add pick error + IC count
        {
            SData.sprintf("@e02002%s,%s,%s:%d,%s", ASE_aMode, ASE_aJamCode, ASE_aMessage, iOneTrayPickCount[0], ASE_aDate);
        }
        else
        {
            SData="@e02002"+ASE_aMode+","+ASE_aJamCode+","+ASE_aMessage+","+ASE_aDate;
        }
        RespondASECom(SData);                                                   //kevin 20150818 20150610 回應 ase Reset finish
        if(AlarmID==16042)
            bCleanOutFinish=true;                                               //kevin 20210406 clean out open close Site
    }

    fNote->aJamCodeFilePath=fFTPClient->SaveJamCodeFile(IniConfig.SocketHandlerID, Now(), Code, AnsiString(UnitName+":"+Message));
    fNote->AlarmLogXmlToFTP(Code, Message);

    if(CosFunction.bCheckTempDuringIndexArmTesting)                             //Jimmychiu 20240916 : Check the temperature during index arm testing
    {
        DoCheckTemperatureDuringIndexTesting(Code);
    }

    if(CosFunction.bOEEFunction)                                                //Steven 20180417 (Jou) : OEE功能
    {
        if(IniConfig.bN14_1_EnableOEEFunction==true &&                          //Sam 20171018 (wei) : OEE Bug 修正 Sam 20170724 移植超豐 OEE 功能 form HT-7045
           IniConfig.iN14_1_OEERecordCycleTime>0)
        {
            fProductionInfo->RecordControlBinCount("Pause Control Bin Check");  //Sam 20200525 : Control Bin
            fProductionInfo->EachCycleSecondDo_SaveAndUpdateOEEFiles(true);
            fProductionInfo->bStartNeedSaveAndUpdateOEEFiles=true;
            fProductionInfo->_sOEE_JamCode=AnsiString(fNote->edErrorCode->Text);
            if(fSecurity->GetBit8(fNote->Edit3->Text+" "+fNote->edUnitName->Text, fNote->edErrorCode->Text)==1)         //Sam 20170925 自定義 Jam 計數
                fProductionInfo->_iOEE_ServiceCount++;
            else
                fProductionInfo->_iOEE_JamCount++;
        }

        if(IniConfig.bN14_1_EnableOEEFunction==true &&
           fProductionInfo->IsOEEStartLotSuccess()==true)
        {
            fProductionInfo->SaveMessageHistroy(Code, Message, fNote->AlarmType, 0);                                    //Sam 20170724 uAlarmCode->SearchSkipLevel() 先用 0 頂著
        }
    }

    if(CUSTOMER_CODE==CC_KYEC_LEE && IniConfig.bN07_EnableEmployeeIdCheak)      //Ifor 20180911 (Steven) : Add 啟動 Employee ID Check
    {
        if(bSECSGEMAlarm==false)                                                //OMS Alarm 後的Note Alarm 不卡控 工號檢查流程
        {
            bEnableEmployeeIDCheck=true;
            bWaitSecsGemReply=true;
        }
    }

    double dNowDT=Now();
    SaveEventLog();

    if(CUSTOMER_CODE==CC_FOREHOPE_NINGBO)                                       //Jimmychiu 20250426 : 甬矽要求異常發生當下紀錄
    {
        AnsiString sPassTime=AnsiString().sprintf("%lu", PassTime);
        AnsiString sDuplicateError=AnsiString().sprintf("%d", iDuplicateError);
        AnsiString sRecipeName=GetLastOpenFN();
        fNote->SaveErrEventLog(dNowDT, fNote->sJamArea, fNote->sJamCode, "", sPassTime, sDuplicateError,
                        Message, errPart, sRecipeName);
    }
    else
    {
    }

    fNote->iAlarmLine=slEventLog->GetLastLine();                                //Steven 20191016 : 紀錄目前Alarm在檔案裡面的行數
    if(TestIF_File.bRENESAS_EnableFTCT==true)                                   //TrayFeed完成can not解鎖，當下命令可能在41
    {
        if(fNote->edErrorCode->Text=="MES1643")
        {
            fSCKART->iManualStart=false;                                        //40 41 要上鎖
        }
        else
        {
            fMain->RENESAS_Server->FTCTManStartUnlock();
        }
        bAutoRestartAfterFTCTAlarm=false;
    }

    fNote->ShowModal();

    if(CUSTOMER_CODE==CC_FOREHOPE_NINGBO)
    {
        //
    }
    else
    {
        AnsiString sPassTime=AnsiString().sprintf("%lu", PassTime);
        AnsiString sDuplicateError=AnsiString().sprintf("%d", iDuplicateError);
        AnsiString sRecipeName=GetLastOpenFN();
        fNote->SaveErrEventLog(dNowDT, fNote->sJamArea, fNote->sJamCode, Recovery, sPassTime, sDuplicateError,
                               Message, errPart, sRecipeName);
    }

    if(CosFunction.bNeedAlarmAfterUnloaderFull &&
       fSecurity->GetAlarmAfterUnloaderFull(fNote->sJamArea, fNote->sJamCode))  //Jimmychiu 20240902 : Need Alarm After Unloader Full
    {
        MOT[iWhichAuto].sUnloaderAlarmMsg=AnsiString().sprintf("Need to check the tray device status,the alarm is caused by %s",fNote->sJamCode);
    }

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //kevin 20150818 20150724 add remote
    {                                                                           //ASE_aDate=FormatDateTime("yyyy/mm/dd hh:mm:ss", Now())
        if(KCode==0)                                                            //Steven : 沒有KeyCode的話,不用Recovery
            SData="@e02002"+ASE_aMode+","+ASE_aJamCode+","+ASE_aMessage+","+ASE_aDate+",Pause,"+PassTime;
        else
            SData="@e02002"+ASE_aMode+","+ASE_aJamCode+","+ASE_aMessage+","+ASE_aDate+","+Recovery+","+PassTime;
        RespondASECom(SData);                                                   //kevin 20150818 20150610 回應 ase Reset finish
        ASE_aDate="";
        ASE_aJamCode="";                                                        //kevin 20150818 ASE alarm send
        ASE_aMessage="";                                                        //kevin 20150818 ASE alarm send
        ASE_aMode="";                                                           //kevin 20150818 ASE alarm send
    }
    bIndexDropVacuumError=false;                                                //kevin 20190418 避免 inarm 來回跑

    if(fNote->ReturnCode==K_SKIP && SoftStop==true)                             //Jimmychiu 20260410: V899 add SoftStop guard to prevent false MES2111 on Start-close
        fMain->Pause("ShowErrorMessage");

    bCleanOutFinish=false;                                                      //kevin 20210406 clean out open close Site

    if(FormHS->Visible==true &&                                                 //Ifor 20160829 Alarm 後需關閉HSForm
       (iSendAlarmCode==15318 ||                                                //Steven 20190714 : Fixed for Self test hang up.
        iSendAlarmCode==15313 ||
        iSendAlarmCode==15314 ||
        iSendAlarmCode==15315 ||
        iSendAlarmCode==15316 ||
        iSendAlarmCode==15317))
    {
        FormHS->Close();
    }
    bSendRealCCDSendStart=true;
    if(CUSTOMER_CODE==CC_PANTHER)                                               //Jimmychiu 20250916 : 新增機台運作狀態紀錄
    {
        fMain->machineTime.Pause();
    }

    return fNote->ReturnCode;
}
//------------------------------------------------------------------------------
//Steven 20190412 : 修改Motor Alarm Message --> WAR24xxxo (xxx為馬達號碼, 0為Alarm Type)
//------------------------------------------------------------------------------
void ShowMotorErrorMessage(AnsiString Code, int MotorAlarmNo, AnsiString errPart)
{
    SoftStop=false;
    SoftStart=false;
    StopAllMotor();
    MOT[MTestY1].Gali_Command("ST", errPart);
    IndexMotorBreakerOFF();
    fAllMotorHome=false;

    if(InitialOK==false)                                                        //Steven 20250310 : Add protection
    {
        MyDBIProcess("Exception", Code, AnsiString(MotorAlarmNo));
        return;
    }

    if(Code=="WAR")                                                             //Steven 20100830
    {
        ShowErrorMessage("WAR16101", 0, MMSystem, false, AnsiString(MotorAlarmNo));                                     //Motor error !!
        return;
    }

    int Pos=0;
    AnsiString Message, UnitName, AxleName, Str;
    int AlarmID, UnitNo, AxleNo;

    Code=Code+AnsiString(MotorAlarmNo);
    Code=Code.UpperCase();

    iEventID=MyDBIEvent(Code, 0, &AlarmID, &UnitNo, &AxleNo, &fNote->AlarmType, &Message, &UnitName, fMain->edWorkTemperBase->Text);
    if(AlarmID==41)
    {
        MyDBIProcess("Message", "Unknow Alarm Code: "+Code);
    }

    if(UnitNo>0 && UnitNo<=30)                                                  //Steven 20140222 Start: Alarm Code設定權限
        fNote->sJamArea=JamArea[UnitNo-1];
    else
        fNote->sJamArea="";

    fNote->sJamCode=Code;
    ProductionLog(Message, true);                                               //JerryYang 20160217 for 矽品蘇州 EventLog也要存成文字檔

    UnitNo=atoi(Code.SubString(6, 3).c_str());

    if(UnitNo>=MInArmX && UnitNo<=MInArmZH)             Pos=MInArmX;
    else if(UnitNo>=MOutArmX && UnitNo<=MOutArmZH)      Pos=MOutArmX;
    else if(UnitNo<=MTrayX)                             Pos=UnitNo;
    else                                                Pos=MMSystem;

    ShowErrorUnit(Pos);
    fNote->ErrShowToForm(Code, MOT[UnitNo].Alias, Message, MotorAlarmNo);

    if(CosFunction.bOLPFunction)                                                //Steven 20141229 : OLP功能
    {
        fAutomation->DoCommandBuffer("ALARM_REQUEST", "", AnsiString(UnitName+":"+Message), fNote->AlarmType, Code);    //Steven 20110210
    }
    ASE_aDate=Now();
    fNote->aJamCodeFilePath=fFTPClient->SaveJamCodeFile(IniConfig.SocketHandlerID, ASE_aDate, Code, AnsiString(UnitName+":"+Message));
    fNote->KeyCode=0;                                                           //ChungHung 20120927 強制只能秀pause  <----原本未指定任何顯示的組合建所以會依照上次所設定的

    TStringList *SL;                                                            //Steven 20161115 : EventLog存成文字檔
    SL=new TStringList();
    if(IniConfig.bSPILFunction==true)                                           //Steven 20240604 : SPIL格式的event log
    {
        Str.sprintf("%04d-%02d-%02d %02d:%02d:%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    }
    else
    {
        Str.sprintf("%04d-%02d-%02d", SystemYear, SystemMonth, SystemDate);
        SL->Add(Str);
        Str.sprintf("%02d:%02d:%02d.%03d", SystemHour, SystemMin, SystemSec, SystemMSec);
        SL->Add(Str);
    }

    fNote->iAlarmLine=slEventLog->GetLastLine();                                //Steven 20191016 : 紀錄目前Alarm在檔案裡面的行數
    SaveEventLog();
    if(TestIF_File.bRENESAS_EnableFTCT==true)                                   //RogerYang 20251002 : RogerYang 瑞薩FT-CT 解綁manualstart
    {
        bAutoRestartAfterFTCTAlarm=false;
        fMain->RENESAS_Server->FTCTManStartUnlock();
    }
    fNote->ShowModal();

    if(IniConfig.bSPILFunction==true)                                           //Steven 20240604 : SPIL格式的event log
    {
        SL->Add(fNote->sJamArea);                                               //UnitName
        SL->Add(fNote->sJamCode);                                               //AlarmCode
        SL->Add(Str);                                                           //OccurDateTime
        SL->Add(Recovery);                                                      //Recovery
        SL->Add(PassTime);                                                      //StopedTime
        SL->Add(iDuplicateError);                                               //Duplicate
        SL->Add(Message);                                                       //Message
        SL->Add(MotorAlarmNo);                                                  //ErrPart
    }
    else
    {
        SL->Add(fNote->sJamArea);
        SL->Add(fNote->sJamCode);
        SL->Add(Recovery);
        SL->Add(PassTime);
        SL->Add(iDuplicateError);
        SL->Add(Message);
        SL->Add(MotorAlarmNo);
        SL->Add(GetLastOpenFN());
    }
    SaveEventLog();
//    if(IniConfig.bSPILFunction==true)                                           //Steven 20240604 : SPIL格式的event log
//        slEventLog->AddTextWithLineNo(SL->CommaText);
//    else
//        slEventLog->AddText(SL->CommaText);

    slEventLog->MyInsertToFile(SL->CommaText, fNote->iAlarmLine-1);             //Steven 20191016 : 紀錄目前Alarm在檔案裡面的行數
    SL->Clear();                                                                //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete SL;

    bIndexDropVacuumError=false;                                                //kevin 20190418 避免 inarm 來回跑
    iHome=1;                                                                    //ChungHung 20120927 修改做contact height 時Index馬達錯誤會照成撞機
}
//------------------------------------------------------------------------------
void __fastcall TfNote::RTCalarmImageUnload()                                   //jou 20170210 (Steven) : RTC alarm image unload
{
    AnsiString asDateTime;
    AnsiString asSourcePath,asTarPath;
    int iMax=8;
    if(CosFunction.bRTCalarmUnload==true && IniConfig.bN05_RTCalarmUnload==true &&
       REAL_TIME_CCD==true && !COM2->bCCDDummyRum)
    {
        if(DirectoryExists(IniConfig.asN05_RTCalarmUnload))
        {
            asDateTime=FormatDateTime("yyyymmdd_hh_mm_ss", Now());
            if(edErrorCode->Text=="WAR0343" || edErrorCode->Text=="WAR0346")    //RTC FullView Socket Have Devices Error!
            {
                //Handler 開始執行之前若Full View NG
                //(2相機)丟image5.bmp, image6.bmp
                //(4相機)丟image5.bmp, image6.bmp, image7.bmp, image8.bmp
                if(MachineTypeChoice==Type_HT9046_LS)
                    iMax=8;
                else
                    iMax=6;

                for(int i=5; i<=iMax; i++)
                {
                    asSourcePath.sprintf("D:\\VisionNG\\image%d.jpg",i);
                    asTarPath.sprintf("%s\\%s_FullView_Image%d.jpg", IniConfig.asN05_RTCalarmUnload,asDateTime, i-4);
                    if(FileExists(asSourcePath))
                    {
                        CopyFile(asSourcePath.c_str(), asTarPath.c_str(), false);
                        MySleep(100);
                    }
                }
            }
            else if(edErrorCode->Text=="WAR0342")                               //RTC HalfView Socket Have Devices Error!
            {
                //Handler running中 若double device 造成 Half View NG
                //(2相機)丟image1.bmp, image2.bmp, image3.bmp, image4.bmp
                //(4相機)丟image1.bmp, image2.bmp, image3.bmp, image4.bmp
                for(int i=1; i<=4; i++)
                {
                    asSourcePath.sprintf("D:\\VisionNG\\image%d.jpg",i);
                    asTarPath.sprintf("%s\\%s_HalfView_Image%d.jpg", IniConfig.asN05_RTCalarmUnload, asDateTime, i);
                    if(FileExists(asSourcePath))
                    {
                        CopyFile(asSourcePath.c_str(), asTarPath.c_str(), false);
                        MySleep(100);
                    }
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfNote::FormShow(TObject *Sender)
{
    int iO17Count=0;
    int iO17Sec;
    bool bRedFlag=false;
    bool bSilent=false;
    bool bAlarmPassWord=false;
    int Level=0, ct=0;
    bool bSCCNeedAlarm=false, bSetFocus=false;                                  //JerryYang 20240111 : add P53 function
    AnsiString sMessage, sTCPSendMes="";                                        //Sam 20230426 : 通知系統 Handler 已經密碼鎖定

    TBtnPanel *Ptr[]={BtnSkip, BtnRetry, BtnTrayFeed, BtnTrayEnd, BtnCleanOut, BtnReset, BtnHome,BtnTrain,BtnOneCycle};                                         //ChungHung 20140730 add ContinuousFailHaveOneCycle //kevin 20130722 cancel BtnFix //kevin 20130218 add FIX//ChungHung HT9045 2011/12/13 //Input pickup device error時,按"retry"鍵,機台都會自動home add BtnHome
    int KeyComp[]={K_SKIP, K_RETRY, K_TRAY_FEED, K_TRAY_END, K_CLEAN_OUT, K_RESET, K_HOME, K_TRAIN, K_ONECYCLE};        //ChungHung 20140730 add ContinuousFailHaveOneCycle //kevin 20130722 cancel K_FIX //kevin 20130218 add FIX//ChungHung HT9045 2011/12/13 //Input pickup device error時,按"retry"鍵,機台都會自動home add K_HOME
    ExString="";                                                                //Steven 20151111 : 改成全域變數,避免Alarm與Message按的時候沒紀錄

    bHandlerPause=true;                                                         //JerryYang 20200407 show alarm以及message也要把flag設true
    iHandlerStartCount=0;
    palSGCheckList->Visible=false;                                              //Sam 20240425 : 修正 [A32] 功能報警未顯示問題

    if(edErrorCode->Text=="MES1640" ||
       edErrorCode->Text=="MES1641")                                            //JerryYang 20220923 : 掉料後one cycle finish要開門按z1
    {
        if(bOutShtLoseICNeedSetErrBin || bIndexDropICNeedSetErrBin)
        {
            bOutShtLoseICNeedSetErrBin=false;
            bIndexDropICNeedSetErrBin=false;
            for(int i=0; i<MAX_Index_Row; i++)
            {
                for(int j=0; j<MAX_Index_Col; j++)
                {
                    bTestSiteNeedSetErrBin[i][j]=false;
                }
            }
            bIsTestSitICFallDown=true;
        }
    }

    if(MyMessageBox->Visible==true)                                             //Ifor 20170424 (wei) add Note Alarm 顯示前若form有開啟先關閉避免卡死
    {
        if(CUSTOMER_CODE==CC_KYEC_LEE &&
           bSECSGEMAlarm==true &&
           fNote->fShow==false)                                                 //Ifor 20170615 (wei) KYEC add SECS GEM Alarm 未解除時發生NoteAlarm旗標
        {
            bSECSGEM_NoteAlarm=true;
        }
        else
        {
            if(iUnLoaderCount!=0)                                               //Ifor 20170906 (wei) add 避免 PreAlarm -> Alarm -> SECS GEM Alarm 同時發生造成當機問題
                bAlarmAfterPreAlarm=true;
            MyMessageBox->Close();
        }
    }

    if(CUSTOMER_CODE==CC_KYEC_LEE &&
       FormBarcodeReader->bShow)                                                //Ifor 20170615 (wei) KYEC add 判斷BarCode 畫面若有顯示，若有顯示關閉畫面
    {
        FormBarcodeReader->Close();
    }

    if(CUSTOMER_CODE==CC_AnalogDevice_Phil &&                                   //JerryYang 20230721 : Analog要求WAR07322要開門按Z1
       edErrorCode->Text=="WAR07322")
    {
        bIsTestSitICFallDown=true;
    }

    if(edErrorCode->Text=="MES16112" ||
       edErrorCode->Text=="MES16111")                                           //JerryYang 20220923 : MES16111手動輸入EQC數量
    {
        if(fSCKART->sInfo_Step=="CORR")                                         //JerryYang 20220923 : 手動輸入EQC數量
        {
            lblLotID->Visible=false;
            edtLotID->Visible=false;
        }
        else
        {
            lblLotID->Visible=true;
            edtLotID->Visible=true;
        }
        lblLotQty->Visible=true;
        edtLotCount->Visible=true;

        lblQCQty->Visible=false;
        edEQCQty->Visible=false;
        if(edErrorCode->Text=="MES16111")
        {
            lblLotID->Visible=false;
            lblLotQty->Visible=false;
            edtLotCount->Visible=false;
            edtLotID->Visible=false;
            lblQCQty->Visible=true;
            edEQCQty->Visible=true;
        }
        pnlLotInfo->Visible =true;                                              //Sam 20240804 : 修正顯示
        pnlLotInfo->Parent  =tsHandler;
        pnlLotInfo->Top     =pgcNote->Height/2-pnlLotInfo->Height/2;
        pnlLotInfo->Left    =pgcNote->Width/2-pnlLotInfo->Width/2;
        pnlLotInfo->BringToFront();
        edtLotCount->Text=0;
        edtLotID->Text="";
        edEQCQty->Text=0;
    }

    if(Prod.bAfterShowAlarmMessageUseInitialDelay)
    {
        if(bUseInitTempOffset)                                                  //Steven 20141117 : 起測時溫度要補Offset   //Steven 20150415 : 換位置
        {
            RecordProcess("After show alarm message trigger initial offset function.");
            iInitContactCount=0;                                                //Steven 20141117 : 起測時溫度要補Offset
            fHeaterOK=false;
        }

        if(bUseInitDelay &&                                                     //Ifor 20180116 (Steven) : add KYEC 常溫使用 Initial start delay
           edErrorCode->Text!="WAR07352")
        {
            if(CUSTOMER_CODE==CC_SCK)                                           //Isaac 20171106 (Steven) : SCK IndexArm在壓測位置發生alarm後，不等Initial delay
            {
                if(MOT[MTestZ1].Gali_ReadEncoderPos()<(Prod.TestZ1_Test+10) ||  //Steven 20171226 (Wei) : 修改不補償溫度的高度
                   MOT[MTestZ2].Gali_ReadEncoderPos()<(Prod.TestZ2_Test+10))
                {
                    bDoAfterShowAlarmMessageUseInitialDelay=false;
                }
                else
                {
                    bDoAfterShowAlarmMessageUseInitialDelay=true;
                }
            }
            else
            {
                bDoAfterShowAlarmMessageUseInitialDelay=true;
            }
        }
        else
        {
            bDoAfterShowAlarmMessageUseInitialDelay=false;                      //ChungHung 20140105 add for SCK have order
        }
    }

    bPauseInMotor=true;
    bPauseOutMotor=true;
    bPauseSortMotor=true;                                                       //RogerYang 20250510 Add for 9046AU
    bAlarmReset=false;                                                          //Steven 20140905 : 紀錄有被按下Alarm Reset
    AnsiString str;                                                             //ChungHung 02120720 add

    if(CUSTOMER_CODE==CC_AMKOR_China ||
       CUSTOMER_CODE==CC_QUALCOMM)                                              //JerryYang 20170412 (Steven) add QUALCOMM   //Steven 20101123
    {
        bIsTestSitICFallDown=false;
    }

    if(IniConfig.bIndexJamInArmAway==true &&
       IsTestSitICFallDown())                                                   //jou 2011-10-14 start : 掉料需開啟Chamber門
    {
        bOpenChamberDoor=false;
    }

    if((iHWFix_BinBox==1 || bCancelErrorBin) &&
        edErrorCode->Text=="WAR07356" &&
       (bBinError[0] || bBinError[1]))                                          //kevin 20160819 add error bin box
    {
        palCheckSht->Parent=tsHandler;
        palCheckSht->BringToFront();
        bOpenSixDoor=true;                                                      //kevin 20160725 bin error 需開右後方門確認
    }
    else
    {
        palCheckSht->Parent=tsRedAlarm;
    }

    if(bOutArmDropICSkip)                                                       //kevin 20171005 (wei) Out Arm Drop IC Skip 開門確認
    {
        pnlOutArmDrop->Parent=tsHandler;
        lblOutArmDropEN->Caption="Please check Out arm drop device takse out device and open 6 door.";
        lblOutArmDropCN->Caption="請開6 號門(右後方),取出 掉落 ic out Arm drop ic";
        pnlOutArmDrop->BringToFront();
        bOpenSixDoor=true;                                                      //kevin 20160725 bin error 需開右後方門確認
    }

    BackRecordLastHappenTimeString=RecordLastHappenTimeString;
    BackRecordLastHappenErrorCode=RecordLastHappenErrorCode;
    BackRecordHappenTime=RecordHappenTime;

//    if(XResolution>1300)
    if(AUTO_EMPTY_COLOR>=3)                                                     //Steven 20230907 : For HT-9011UC
    {
        Width=1200;
        palOutArm->Width=475;
        if(USE_OUT_SORT_ARM!=eartUninstall)                                     //Steven 20240822 : For HT-9046AU
        {
            palAuto1    ->Left=706;
            palAuto2    ->Left=781;
            palAuto3    ->Left=856;
            palAuto4    ->Left=480;
            palAuto5    ->Left=555;
            palAuto6    ->Left=630;
            palAuto1_Car->Left=706;
            palAuto2_Car->Left=781;
            palAuto3_Car->Left=856;
            palAuto4_Car->Left=480;
            palAuto5_Car->Left=555;
            palAuto6_Car->Left=630;
        }
        else
        {
        }
    }
    else
    {
        Width=980;
        palOutArm   ->Width  =255;
        palFix4     ->Visible=false;
        palFix5     ->Visible=false;
        palFix6     ->Visible=false;
        palAuto4    ->Visible=false;
        palAuto5    ->Visible=false;
        palAuto6    ->Visible=false;
        palAuto4_Car->Visible=false;
        palAuto5_Car->Visible=false;
        palAuto6_Car->Visible=false;
    }

    palOutSh3       ->Visible=(USE_OUT_SORT_ARM!=eartUninstall);                //Steven 20240822 : For HT-9046AU;
    palOutArm2      ->Visible=(USE_OUT_SORT_ARM!=eartUninstall);
    palLoad2        ->Visible=(USE_2nd_LOADER!=eartUninstall);
    palLoad2_Car    ->Visible=(USE_2nd_LOADER!=eartUninstall);

    ShowMessageEdit1->Width=pnlMsg->Width-8;
    reDescription   ->Width=pnlMsg->Width-8;
    RichEdit1       ->Width=pnlMsg->Width-16;

    Left=(XResolution-Width)/2;                                                 //Steven 20230912 : 調整畫面為FULL HD
    Top =(YResolution-Height)/2;

    bHangTimePause=true;                                                        //Steven 20090827 : Hang Up dectector
    bSupplyNewICTrayPause=true;

    for(int i=0; i<7; i++)                                                      //jou 2011-02-21 start : magazine沒動作會alarm
    {
        bLifterPause[i]=true;
        bAuto2Pause[i]=true;                                                    //kevin 20120718 for auto2
    }

    AnsiString Str, SourceIndexMessage, SourceIndexMessage1;
    if(bChangeCleanPad==false)                                                  //kevin 20120801  Autoclean 更換CLEAN PAD
    {
        SourceIndexMessage ="Please call shift Engineer";                       //kevin 20120801  Autoclean 更換CLEAN PAD
        SourceIndexMessage1="請Call輪班工程師來協助處理";
    }
    else
    {
        SourceIndexMessage ="Please call shift Engineer to change clean pad。";  //kevin 20120801  Autoclean 更換CLEAN PAD
        SourceIndexMessage1="請Call輪班工程師來協助處理，更換Clean Pad。";
    }

    StopAllMotor();
    if(fHome->fShow)
        fHome->sbAbortHomeClick(fHome);
    fMemoPos=false;
    bSilent=fSecurity->GetJemSilent(sJamArea, sJamCode);                        //Steven 20150423 : SCK要求可以自訂Alarm是否要有蜂鳴器
    if(bSilent)
        bAlarmBuzzer=false;
    else
        bAlarmBuzzer=true;

    if(CosFunction.bUseAlarmUnlockPassWord==true)                               //Ifor 20170214 (wei) add 解除Alarm 需要獨立密碼
    {
        bAlarmPassWord=fSecurity->GetJemUnlockPassWord(sJamArea, sJamCode);     //Ifor 20170214 add 可以自訂Alarm是否要解除密碼
        if(bAlarmPassWord)
            bAlarmUnlockPassWord=true;
        else
            bAlarmUnlockPassWord=false;
    }

    bLampAlarmReset=true;

    if(MachineTypeChoice==Type_HT9046_LS)                                       //2013-01-15    Dell
    {
        Image1->Left    = 66;
        Image1->Top     = 84;
        Image1->Width   = 300;

        Image2->Left    = 66;
        Image2->Top     = 320;
        Image2->Width   = 300;
        Image2->Center  = false;

        Image3->Left    = 376;
        Image3->Top     = 84;
        Image3->Width   = 300;

        Image4->Left    = 376;
        Image4->Top     = 320;
        Image4->Width   = 300;
    }

    if(CosFunction.bTrayOCR)                                                    //wei 20160805
    {
        Image2->Left    = 66;
        Image2->Top     = 10;
        Image2->Width   = 650;
        Image2->Height  = 500;
        Image2->Center  = true;
    }

    tNoteTimer.LatchCycleTimeSec(true);
    for(int i=0; i<9; i++)                                                      //ChungHung 20140730 add ContinuousFailHaveOneCycle 8--->9 //kevin 20130722 cancel K_FIX //kevin 20130312  7->9 ChungHung HT9045 2011/12/13 //Input pickup device error時,按"retry"鍵,機台都會自動home
    {
        Select[i]=false;
        Ptr[i]->FalseColor=(TColor)0x00804000;
        if(KeyCode & KeyComp[i])
        {
            Ptr[i]->Visible=true;
            ct++;
        }
        else
        {
            Ptr[i]->Visible=false;
        }
    }

    if(KeyCode==0)
    {
        BtnStart->Visible=false;
    }
    else
    {
        BtnStart->Visible=true;
    }

    if(EmptySocketCheckModeBeUse() &&                                           //Steven 20201022 : For RFMD Empty Socket Check Funstion.
       IniConfig.bI41_4_AfterContactorJam)
    {
        if(edErrorCode->Text=="JAM0301" || edErrorCode->Text=="JAM0302" ||
           edErrorCode->Text=="JAM0303" || edErrorCode->Text=="JAM0304" ||
           edErrorCode->Text=="JAM0305" || edErrorCode->Text=="JAM0306" ||
           edErrorCode->Text=="JAM0312" || edErrorCode->Text=="JAM0313" ||
           edErrorCode->Text=="JAM0314" || edErrorCode->Text=="JAM0315")
        {
            fMain->ResetForESC("Start Empty Socket OneCycle by Index Happen Jam");
        }
    }

    if(IniConfig.bIndexDropNeedPwdByIni ||                                      //jou 2012-08-28 Tester Time up Error Need Password
       IniConfig.bTesterTimeUpErrorNeedPassword)                                //Steven 20110627
    {
        if(IniConfig.bIndexDropNeedPwdByIni &&                                  //Index掉料
           (edErrorCode->Text=="JAM0303" || edErrorCode->Text=="JAM0304" ||
            edErrorCode->Text=="JAM0305" || edErrorCode->Text=="JAM0306" ||
            edErrorCode->Text=="JAM0314" || edErrorCode->Text=="JAM0315"))      //JerryYang 20160516 add JAM0314 JAM0315
        {
            bShowSpecialPan=true;
        }
        else if(IniConfig.bTesterTimeUpErrorNeedPassword &&
                edErrorCode->Text=="WAR07352")                                  //Tester Time up error
        {
            bShowSpecialPan=true;
        }
        else
        {
            bShowSpecialPan=false;
        }

        if(bShowSpecialPan==true)                                               //2011.01.20 Q_Q V207E {
        {
            PanSpecialNote->Parent=tsHandler;
            PanSpecialNote->BringToFront();
            bErrPan_err=true;                                                   //Richard 2011/2/22 SpecialPanel  //Q_Q 2011.02.21
        }

        if(FileExists(asErrNotePath) && 1==ReadIniData(asErrNotePath, "SUCK", "TestSuck",0))
        {
            //Message
            AnsiString MessageEn, MessageCh;
            MessageEn=ReadIniData(asErrNotePath, "MESSAGE", "En", AnsiString(""));
            MessageCh=ReadIniData(asErrNotePath, "MESSAGE", "Ch", AnsiString(""));
            lblSpecialNoteEn->Caption=MessageEn;
            lblSpecialNoteCh->Caption=MessageCh;
            //Password
            Pwd=ReadIniData(asErrNotePath, "PASSWORD", "Pwd", AnsiString(""));
        }
        else
        {
            Pwd="";
            bErrPan_err=false;
        }
    }

    if(CUSTOMER_CODE==CC_ASE_CL &&
       edErrorCode->Text=="MES1640")                                            //JerryYang 20250728 : Thomas要求只有跳出ONE CYCLE FINISH視窗才發報Event
    {
        if(IniConfig.bEnable_SECS_GEM==true)                                    //Steven 20140528 : Secs Gem
            EventReport(SECS_EVENT.OneCycleFinish);                             //41     One Cycle Finish
    }

    if(IniConfig.bSPILFunction &&                                               //JerryYang 20220923 : 矽品版本ART
       (edErrorCode->Text=="MES1641" ||
        edErrorCode->Text=="MES1640") &&
       (LastSet.iRunStartMode!=rsmInitial_ART && LastSet.iRunStartMode!=rsmContinuStart_ART &&
        LastSet.iRunStartMode!=rsmAutoRetest && LastSet.iRunStartMode!=rsmContinuRetest_ART))
    {
        btPrintSummary->Visible=true;
        btPrintSummary->BringToFront();
    }

    if(CosFunction.bUseSCKART &&                                                //Steven 20170324 (wei) : Modify
       (LastSet.iRunStartMode==rsmInitial_ART      ||
        LastSet.iRunStartMode==rsmContinuStart_ART ||
        LastSet.iRunStartMode==rsmAutoRetest       ||
        LastSet.iRunStartMode==rsmContinuRetest_ART))
    {
        if(fSCKART->iTesterType==0 &&
           (edErrorCode->Text=="MES1641" || edErrorCode->Text=="MES1640"))      //JerryYang 20220923 : add
        {
            btPrintSummary->Visible=true;
            btPrintSummary->BringToFront();
            if(CUSTOMER_CODE==CC_TERAPOWER)                                     //Sam 20202015 : TCP ART 增加 OneCycle 可手動強制中斷流程
            {
                btPrintSummary->Caption="Break ART";
            }
        }

        if(IniConfig.bA10_5SCKART_AutoCorrection &&                             //Steven 20171211 (Wei) : Auto correction for SCK ART
           (edErrorCode->Text=="WAR0119" || edErrorCode->Text=="WAR0120" ||
            edErrorCode->Text=="MES0921" || edErrorCode->Text=="WAR0121"))
        {
            pgcNote->ActivePageIndex =5;                                        //Steven 20240614 : 修正ART畫面顯示
            pgcNote->ActivePage      =tsCorrectionCount;

            btCorrectCounting_Auto->Caption="Auto Correct Count";
            btCorrectCounting_StopART->Visible=false;
            btContinuousTest->Visible=false;
            pnlAutoCorrCnt->Visible=false;
            if(IniConfig.bSPILFunction && edErrorCode->Text=="WAR0120")         //JerryYang 20220923 : 矽品版本ART
            {
            }
            else
            {
                pnlCorrectionCount->Visible=true;
                if(IniConfig.bSPILFunction)                                     //JerryYang 20220923 : 矽品版本ART
                {
                   pnlAutoCorrCnt->Visible=true;
                   SetWindowLong(btCorrectCounting_Auto->Handle, GWL_STYLE, GetWindowLong(btCorrectCounting_Auto->Handle, GWL_STYLE) | BS_MULTILINE);
                   SetWindowLong(btCorrectCounting_StopART->Handle, GWL_STYLE, GetWindowLong(btCorrectCounting_StopART->Handle, GWL_STYLE) | BS_MULTILINE);
                   SetWindowLong(btContinuousTest->Handle, GWL_STYLE, GetWindowLong(btContinuousTest->Handle, GWL_STYLE) | BS_MULTILINE);
                    btCorrectCounting->Visible=false;
                    if(edErrorCode->Text=="WAR0119")
                    {
                        btCorrectCounting->Visible=false;
                        btCorrectCounting_Auto->Caption="平帳+執行ART搬運\r\n(Auto Corr cnt + Run ART)";
                        btCorrectCounting_StopART->Visible=true;
                        btCorrectCounting_StopART->Caption="平帳+中斷ART\r\n(Auto Corr cnt + Stop ART)";
                        btContinuousTest->Visible=true;
                        btContinuousTest->Caption="續測未測品\r\n(Continue test untest IC)";
                    }
                    else
                    {
                        btCorrectCounting_StopART->Visible=false;
                    }
                }
                else
                {
                    btCorrectCounting->Visible=true;
                    btCorrectCounting_StopART->Visible=false;
                }

                palInputJamCount->Caption=fSCKART->palInputJamCount->Caption;
                palOutputJamCount->Caption=fSCKART->palOutputJamCount->Caption;
                if(atoi(fSCKART->palInputCount->Caption.c_str())>atoi(fSCKART->palInputCnt->Caption.c_str()))           //Steven 20171129 (Wei) : If Input Q'ty < Lot Q'ty condition, press auto correction and input miss match count goes to Reject merge count
                    pnlMissMatchQty->Caption=atoi(fSCKART->palInputCount->Caption.c_str())-atoi(fSCKART->palInputCnt->Caption.c_str());
                else
                    pnlMissMatchQty->Caption=0;
                pnlCorrectionCount->BringToFront();
            }
        }
    }

    if((edErrorCode->Text=="MES0923" && TrayForm.iManualRemoveLoader==2) ||     //Steven 20130819 : 加入開門檢查的功能
       (edErrorCode->Text=="MES0101" && (TrayForm.iManualRemoveLoader==2 || IniConfig.bE87PickupErrorAtLoaderNeedOpenDoor) && CosFunction.bPickupErrorAtLoaderNeedOpenDoor) ||  //JerryYang 20160220 add for Amkor-Philippine,In ARM至Loader吸取異常要開門確認後才能繼續
       (edErrorCode->Text=="MES0923" && CosFunction.bOpenDoorCheckLoaderAfterTrayEnd))                                  //Jimmychiu 20250826 : Open Door Check Loader After TrayEnd
    {
        palCheckLoader->Parent=tsHandler;
        lblCheckLoadEN->Caption="Please open left door check if there has any device on loader tray";
        lblCheckLoadCH->Caption="請打開左側門並確認入料盤上是否有產品";
        palCheckLoader->BringToFront();
        bOpenLeftDoor=false;
    }
    else
    {
        bOpenLeftDoor=true;
    }

    if(CUSTOMER_CODE==CC_KYEC_XILINX &&
       edErrorCode->Text=="MES0923" &&
       TrayForm.iManualRemoveLoader==2)                                         //Frank 20170516 (Steven) add Xilinx不須開門檢查
    {
        palCheckLoader->Parent=tsRedAlarm;
        bOpenLeftDoor=true;
    }

    if(bOpenAllDoor==false)                                                     //wei 20160407 Alarm 後需要開門確認
    {
        palCheckLoader->Parent=tsHandler;
        lblCheckLoadEN->Caption="Please Open Door Check";
        lblCheckLoadCH->Caption="請開門確認";
        palCheckLoader->BringToFront();
    }

    if(bChangeCleanPad || bAutoCleanCheckOpenDoor)                              //kevin 20130312  Autoclean 更換CLEAN PAD
    {
        PanSpecialNote->Parent=tsHandler;
        lblSpecialNoteEn->Caption=SourceIndexMessage;
        lblSpecialNoteCh->Caption=SourceIndexMessage1;
        PanSpecialNote->BringToFront();
    }

    if(CUSTOMER_CODE==CC_SCC)                                                   //Ifor 20181024 add :SCC針對特定Alarm Code解除時需權限密碼與紅底
    {
        if(sJamCode=="WAR07301" ||                                              //Socket consecutive failure
           sJamCode=="WAR07321" ||                                              //Arm1: consecutive failure
           sJamCode=="WAR07329" ||                                              //Arm2: consecutive failure
           sJamCode=="JAM0540"  || sJamCode=="JAM0550" ||                       //Device lose at Output Shuttle
           sJamCode=="JAM0303"  || sJamCode=="JAM0304" ||                       //Device drop error (Arm 1) & Device drop error (Arm 2)  //Jou 20140604 Add for SCC
           sJamCode=="WAR0310"  ||                                              //Socket has IC error!                                   //Jou 20140604 Add for SCC
           sJamCode=="JAM0314"  || sJamCode=="JAM0315")                         //JerryYang 20160516 add JAM0314 JAM0315 "Clean pad drop error"
        {
            bSCCNeedAlarm=true;
        }
        else
        {
            bSCCNeedAlarm=false;
        }
    }
    else
    {
        bSCCNeedAlarm=false;
    }

    if(IniConfig.bAlarmMustRedColor==true)                                      //Steven 20111116 : 特殊Alarm需要改紅底
    {
        bRedFlag=fSecurity->GetJemRed(sJamArea, sJamCode);                      //Steven 20140222 Start: Alarm Code設定權限
        if(CUSTOMER_CODE==CC_SCC && bSCCNeedAlarm==true)                        //Ifor 20181024 add :SCC針對特定Alarm Code解除時需權限密碼與紅底
        {
            bRedFlag=true;
        }

        if(bRedFlag==true)
            fNote->Color=clRed;
        else
            fNote->Color=(TColor)0x00C2B8A6;
    }

    if(CosFunction.bIndexCheckCanTurnOff &&
       IniConfig.iD71IndexCheckOnOffMode==1 &&                                  //Isaac 20211019 : 可選擇做index check的時機，index相關alarm後，要做index check
       (edErrorCode->Text=="JAM0301" || edErrorCode->Text=="JAM0302" ||         //Device pick-up error at Arm 1               ;Device pick-up error at Arm 2
        edErrorCode->Text=="JAM0303" || edErrorCode->Text=="JAM0304" ||         //Device drop error (Arm 1)                   ;Device drop error (Arm 1)")
        edErrorCode->Text=="JAM0305" || edErrorCode->Text=="JAM0306" ||         //Device drop to Shuttle error (Arm 1)!!      ;Device drop to Shuttle error (Arm 2)!!
        edErrorCode->Text=="JAM0316" || edErrorCode->Text=="JAM0317" ||         //Handler hang!! Try one cycle, home, reset, or reset program
        edErrorCode->Text=="JAM0327" || edErrorCode->Text=="JAM0350"))          //Index vacuum sensor OFF error               ;By Head yield different
    {
        bIndexJamNeedIndexcheck=true;
        iWhoTriggerPiggyBack=pbtIndexArmPickUpErr;
        ProcessPiggyBackFunction();
    }

    if(IniConfig.bF35OutShtLoseNeedSetErrBin)                                   //F33功能跟F19衝突
    {
    }
    else if(IniConfig.bF19OutShuttleLoseICNeedPiggyback)                        //Steven 20150709 : Out shuttle lose IC要做Piggyback  //JerryYang 20160813 移到外面
    {
        if(edErrorCode->Text=="JAM0508" ||
           edErrorCode->Text=="JAM0509")
        {
            iWhoTriggerPiggyBack=pbtOutShuttleLose;
            ProcessPiggyBackFunction();
        }
    }

    if(IniConfig.bD61IndexArmVacOffErrNeedPiggyBack)                            //JerryYang 20160815 Index arm 發生Vaccum off error要做piggy back
    {
        if(edErrorCode->Text=="JAM0327")
        {
            iWhoTriggerPiggyBack=pbtIndexArmVacOffErr;
            ProcessPiggyBackFunction();
        }
    }

    Level=fSecurity->GetJamLevel(sJamArea, sJamCode);                           //Steven 20140222 Start: Alarm Code設定權限
    bNeedHighLevelPassword=fSecurity->AddJamCount(sJamCode);                    //jou 20171201 (Steven) : 新增統計jam code alarm次數,達到設定數量後提高一階權限才能解開alarm
    if(bNeedHighLevelPassword==true)
    {
        PanSpecialNote->Parent=tsHandler;
        PanSpecialNote->BringToFront();
    }

    if(CUSTOMER_CODE==CC_SCC && bSCCNeedAlarm==true)                            //Ifor 20181024 add :SCC針對特定Alarm Code解除時需權限密碼與紅底
    {
        Level=1;
    }

    if(CosFunction.bEnableHandlerResultServer)                                  //Sam 20230426 : 通知系統 Handler 已經密碼鎖定
    {
        bNeedTCPAlarm=fSecurity->GetJemTCPAlarm(sJamArea, sJamCode);
        if(bNeedTCPAlarm)
        {
            if(Level==0)
                Level=1;
        }
    }

    if(edErrorCode->Text=="WAR07352")
    {
        BtnHome->Caption="HOME";
    }
    else
    {
        BtnHome->Caption="HOME && RETRY";
    }

    if(Level!=0)                                                                //Steven 20140606 : Fix密碼問題
        bNeedPassWord=true;
    else
        bNeedPassWord=false;

    if(CosFunction.bOutShuttleLoseICNeedHome &&                                 //Steven 20140222 End: Alarm Code設定權限
       (edErrorCode->Text=="JAM0508" || edErrorCode->Text=="JAM0509"))          //JerryYang 20160225 OutputShuttle lose IC要回home 為了檢查index arm殘料
    {
        RecordProcess("Output shuttle lose IC need home");                      //JerryYang 20160328 for 矽格北興,觸發回home的地方補上log
    }

    if(CosFunction.bConAlarmNeedKeyInPassword==true &&                          //jou 2014-09-04 Continuous Same Alarm N time Need KeyIn Password
       IniConfig.bO16ConAlarmNeedKeyInPasswordCT)                               //Steven 20200513 : 改成可以設定[O16]
    {
        if(fSecurity->GetJemContiAlarm(sJamArea, sJamCode))                     //Steven 20200513 : 連續alarm輸入密碼的alarm要可以自訂義
        {
            if(CUSTOMER_CODE==CC_KYEC_XILINX && sJamCode.Pos("JAM")!=0)         //wei 20150421 KYEC XILINX Alarm要輸入密碼
            {
                if(Level<=0)
                    bNeedPassWord=false;
                else
                    bNeedPassWord=true;
            }
            else if(sJamCode.Pos("JAM")!=0)
            {
                if(asSameAlarm!=sJamCode)
                {
                    asSameAlarm=sJamCode;
                    iSameAlarmCT=1;
                }
                else
                {
                    iSameAlarmCT++;
                    if(iSameAlarmCT>=IniConfig.iO16ConAlarmNeedKeyInPasswordCT)
                    {
                        iSameAlarmCT=0;
                        bNeedPassWord=true;
                    }
                }
            }
        }
    }
    else if(CosFunction.bConAlarmInTimeLevelUp &&                               //Steven 20210127 : 逸昌要求在單位時間內相同Alarm發生多次,提昇解除alarm權限
            IniConfig.bO17EnableLevelUpWhenContiAlarm)
    {
        if(sJamCode!="")
        {
            if(fSecurity->GetO17ContiAlarm(sJamArea, sJamCode))                 //Steven 20200513 : 連續alarm輸入密碼的alarm要可以自訂義
            {
                iO17Sec=IniConfig.iO17LevelUpWhenContiAlarmTime*60;
                if(listO17->Count==0)
                    listO17->Clear();

                iO17Count=0;
                listO17->Add(new TQPF_Timer());
                TQPF_Timer *temTimer;
                temTimer=(TQPF_Timer *)listO17->Items[listO17->Count-1];
                temTimer->LatchCycleTimeSec(true);

                for(int i=0; i<listO17->Count; i++)
                {
                    temTimer=(TQPF_Timer *)listO17->Items[i];
                    if(temTimer->LatchCycleTimeSec()<iO17Sec)
                        iO17Count++;
                }

                if(iO17Count>=IniConfig.iO17LevelUpWhenContiAlarmCount)
                    bNeedPassWord=true;

                for(int i=listO17->Count-1; i>=0; i--)
                {
                    temTimer=(TQPF_Timer *)listO17->Items[i];
                    if(temTimer->LatchCycleTimeSec()>iO17Sec)
                    {
                        delete temTimer;
                        listO17->Delete(i);
                    }
                }
            }
        }
    }

    if(CUSTOMER_CODE==CC_KYEC_LEE && IniConfig.bN07_EnableEmployeeIdCheak)      //Ifor 20180925 :add 工號檢查完成或失敗都需登入權限
    {
        bNeedPassWord=true;
    }

    if(TestIF_File.bEnableBarCode==true &&
       TestIF_File.bEnableBarcodeCSVCompare==true &&
       edErrorCode->Text=="WAR04217")
    {
        bNeedPassWord=true;
    }

    if(bChangeCleanPad || bAutoCleanCheckOpenDoor)                              //kevin 20130226  Autoclean 更換CLEAN PAD
    {
        PanSpecialNote->Parent=tsHandler;
        lblSpecialNoteEn->Caption=SourceIndexMessage;
        lblSpecialNoteCh->Caption=SourceIndexMessage1;
        PanSpecialNote->BringToFront();
    }

    if(bFrontPadActive) SW[SwFKAlarmReset].OnOff(bLampAlarmReset);
    else                SW[SwRKAlarmReset].OnOff(bLampAlarmReset);

    bHangTimePause=true;                                                        //Steven 20090827 : Hang Up dectector

    if(IniConfig.bE49_LoaderOnlyRetryAndCleanOut && bInArmSuckErr)              //Steven 20141105 : Loader吸取異常只能Retry與CleanOut
        bInArmSuckErr=false;

    TrayEdit->Visible=bInArmSuckErr;
    bInArmSuckErr=false;

    if(ShowMessageEdit1->Text=="One Cycle Finish\r\n")
        UpdateButtonStatus(BtnRetry);                                           //kevin 20170905 (Steven) 換成Edit 可以看後面字串

    if(IsTestSitICFallDown() ||                                                 //ChungHung 20111206
       ((CUSTOMER_CODE==CC_SIGURD_HUKOU) &&
        ((edErrorCode->Text=="JAM0508") ||
         (edErrorCode->Text=="JAM0509"))))                                      //Frank20141027 : Output Shuttle Jam (IC遺失)時, 若按下SKIP時, 需要加入訊息提示.    //wei 20141203
    {
        if((edErrorCode->Text=="JAM0301") ||                                    //JerryYang 20250723 : fix error訊息錯誤
           (edErrorCode->Text=="JAM0302") ||
           (edErrorCode->Text=="JAM0312") ||                                    //JerryYang 20160516 add JAM0312 JAM0313
           (edErrorCode->Text=="JAM0313"))
        {
            lblIndex2ErrorEN->Caption="Open index door to check shuttle IC.\nPress Z1 to continue";
            lblIndex2ErrorCH->Caption="打開index門檢查Shuttle上IC後，並按 Z1 開始";
            pnlIndex2Error->Parent=tsHandler;
        }
        else if(IniConfig.bD40IndexICFallDownMustPressFMotorDown)
        {
            lblIndex2ErrorEN->Caption="Open index door 2 to take out IC.\nPress Z1 and RETRY to continue";
            lblIndex2ErrorCH->Caption="打開index側門將掉落在測試座上的材料取出並按 Z1, RETRY 開始";
            pnlIndex2Error->Parent=tsHandler;
        }
        else
        {
            pnlIndex1Error->Parent=tsHandler;
        }
        pnlIndex1Error->BringToFront();
        pnlIndex2Error->BringToFront();
        if((CUSTOMER_CODE==CC_SIGURD_HUKOU) &&                                  //KaiChen 20200108 ：矽格-湖口，發生 OutShuttle Loss IC 時，檢查Index區提示加大
           ((edErrorCode->Text=="JAM0508") ||
            (edErrorCode->Text=="JAM0509")))
        {
            pnlIndex1Error->Parent=tsRedAlarm;
            pnlIndex2Error->Parent=tsRedAlarm;
        }
    }

    if(bContactCTOverCHK)
    {
        pnlContactOver->Parent=tsHandler;
        pnlContactOver->BringToFront();
    }

    if(bShowNoteCleanSocket && CosFunction.bSmartAutoClean)                     //Sam 20230111 : Smart Auto Clean
    {
        lblSmartCleanEN->Left=24;
        lblSmartCleanEN->Width=725;
        lblSmartCleanEN->Caption="Alarm Smart Auto clean null!open Rear door and offline clean socket";                 //Sam 20230913 : 客戶建勳要求這樣寫。
        lblSmartCleanCH->Left=119;
        lblSmartCleanCH->Width=522;
        lblSmartCleanCH->Caption="auto clean 無效!請打開後門下機清潔Socket";
        pnlCleanSocket->Parent=tsHandler;
        pnlCleanSocket->BringToFront();
    }

    if(AUTO_EMPTY_COLOR==0)
    {
        palEmpty_Car->Visible=false;
        palColor_Car->Visible=false;
    }

    if(bIsContactforce==true ||                                                 //kevin 20130418 Contactforce
       IniConfig.bC08_SocketSensor && bIsSocketSensor)                          //kevin 20130504 socket sensor detect error
    {
        pnlContact->Parent=tsHandler;
        pnlContact->BringToFront();
        lblContactEN->Caption="Open rear door to check socket and contact high\nPress RETRY to continue";
        lblContactCH->Caption="打開後門檢查Socket上是否為空，確認CONTRACT高度\n按 Retry 開始";
        if(IniConfig.bC08_SocketSensor && bIsSocketSensor)                      //kevin 20130504 socket sensor detect error
        {
            lblContactEN->Caption="Open rear door check socket have IC or check socket sensor\nPress Z1 to continue";
            lblContactCH->Caption="打開後門檢查Socket上是否有IC，確認socket sensor\n按 Z1 開始";

            if(CUSTOMER_CODE==CC_JCET)                                          //Frank 20170221 (wei) add for JCET
            {
                lblContactEN->Caption="Open Chamber door check socket have IC or check socket sensor\nPress Z1 to continue";
                lblContactCH->Caption="打開Index區側門檢查Socket上是否有IC，確認socket sensor\n按 Z1 開始";
            }
        }
    }
    else if(IniConfig.bG06HomeinitialCheckZ1 && bHomeinitialCheckPushZ1)        //kevin 20131218
    {
        pnlContact->Parent=tsHandler;
        pnlContact->BringToFront();
        lblContactEN->Caption="Clear debris on hot plate and press Z1 to start home";
        lblContactCH->Caption="檢查機台 hotplate 是否有tray或雜物會造成歸Home時機構損壞";
    }

    if(IniConfig.bAlarmNeedServoOff)                                            //Steven 20110802
    {
        if(edUnitName->Text=="Input Arm"      ||                                //In Shuttle異常, In Arm讓開
           edUnitName->Text=="Input Shuttle"  ||
           edUnitName->Text=="Output Shuttle" ||                                //Steven 20220624 : Out shuttle & Index Jam, In arm servo off
           edUnitName->Text=="Index Unit")
        {
            if(bMyServoOffInArm==false &&                                       //ChungHung 20120912 Hangup 解除
               (MOT[MInArmX].IsCanMove() &&
                MOT[MInArmY].IsCanMove()))
            {
                if(InArmZSafe(DETECT_ALL_FLAG)==-1)                             //如果Z軸在上才可以推
                {
                    iMyServoOffInArmPosX=MOT[MInArmX].ReadEncoderPos();
                    iMyServoOffInArmPosY=MOT[MInArmY].ReadEncoderPos();         //要在ServoOff之前
                    MySleep(200);
                    MOT[MInArmX].ServoOnOff(false);
                    MOT[MInArmY].ServoOnOff(false);
                    bMyServoOffInArm=true;
                    str.sprintf("In Arm Servo Off, X=%d, Y=%d", iMyServoOffInArmPosX, iMyServoOffInArmPosY);
                    RecordProcess(str);
                }
            }
        }

        if(edUnitName->Text=="Output Arm" ||                                    //Out Shuttle異常,Out Arm讓開
           edUnitName->Text=="Output Shuttle")                                  //ChungHung 20120723 Output Shuttle Alarm InArm Servo off
        {
            if(bMyServoOffOutArm==false &&                                      //ChungHung 20120912 Hangup 解除
               (MOT[MOutArmX].IsCanMove() &&
                MOT[MOutArmY].IsCanMove()))
            {
                if(OutArmZSafe(DETECT_ALL_FLAG)==-1)                            //如果Z軸在上才可以推
                {
                    iMyServoOffOutArmPosX=MOT[MOutArmX].ReadEncoderPos();
                    iMyServoOffOutArmPosY=MOT[MOutArmY].ReadEncoderPos();       //要在ServoOff之前
                    MySleep(200);
                    MOT[MOutArmX].ServoOnOff(false);
                    MOT[MOutArmY].ServoOnOff(false);
                    bMyServoOffOutArm=true;
                    str.sprintf("Out Arm Servo Off, X=%d, Y=%d", iMyServoOffOutArmPosX, iMyServoOffOutArmPosY);
                    RecordProcess(str);
                }
            }
        }

        if(IniConfig.bE44EnableLoseDeviceOutShuttleServoOff)                    //ChungHung 20140522 add OutShuttle lose devices can servo off start
        {
            if(edErrorCode->Text=="JAM0508")
            {
                if(bMyServoOffOutShuttle1==false &&
                   (MOT[MOutArmX].IsCanMove() &&
                    MOT[MOutArmY].IsCanMove() &&
                    MOT[MInArmX].IsCanMove() &&
                    MOT[MInArmY].IsCanMove() &&
                    MOT[MTestZ1].IsCanMove()))
                {
                    if(OutArmZSafe(DETECT_ALL_FLAG)==-1 &&                      //如果Z軸在上才可以推
                       InArmZSafe(DETECT_ALL_FLAG)==-1 &&
                       MOT[MTestZ1].Gali_ReadEncoderInRandge(Prod.TestZ1_Safe))
                    {
                        iMyServoOffOutShuttle1Pos=MOT[MInShuttle1].ReadEncoderPos();
                        MySleep(200);
                        MOT[MInShuttle1].ServoOnOff(false);
                        bMyServoOffOutShuttle1=true;
                        str.sprintf("Out Shuttle 1 Servo Off, X=%d", iMyServoOffOutShuttle1Pos);
                        RecordProcess(str);
                    }
                }
            }
            else if(edErrorCode->Text=="JAM0509")
            {
                if(bMyServoOffOutShuttle2==false &&
                   (MOT[MOutArmX].IsCanMove() &&
                    MOT[MOutArmY].IsCanMove() &&
                    MOT[MInArmX].IsCanMove() &&
                    MOT[MInArmY].IsCanMove() &&
                    MOT[MTestZ2].IsCanMove()))
                {
                    if(OutArmZSafe(DETECT_ALL_FLAG)==-1 &&                      //如果Z軸在上才可以推
                       InArmZSafe(DETECT_ALL_FLAG)==-1 &&
                       MOT[MTestZ2].Gali_ReadEncoderInRandge(Prod.TestZ2_Safe))
                    {
                        iMyServoOffOutShuttle2Pos=MOT[MInShuttle2].ReadEncoderPos();
                        MySleep(200);
                        MOT[MInShuttle2].ServoOnOff(false);
                        bMyServoOffOutShuttle2=true;
                        str.sprintf("Out Shuttle 2 Servo Off, X=%d", iMyServoOffOutShuttle2Pos);
                        RecordProcess(str);
                    }
                }
            }
        }
    }

    Recovery="";                                                                //Steven 20120209 : 每次進來都要初始化
    if(IniConfig.bIndexJamInArmAway==true &&                                    //jou 2012-02-24 index drop error,have button can move arm Y front or Rear
       IniConfig.bIndexDropErrCanMove==true &&
       (edErrorCode->Text=="JAM0303" || edErrorCode->Text=="JAM0304" ||
        edErrorCode->Text=="JAM0305" || edErrorCode->Text=="JAM0306" ||
        edErrorCode->Text=="JAM0314" || edErrorCode->Text=="JAM0315" ||         //JerryYang 20160516
        edErrorCode->Text=="JAM0320"))                                          //Sam 20191120 Fix Issue
    {
        btnMoveToFront->Visible=true;
        btnMoveToRear->Visible=true;
    }
    else
    {
        btnMoveToFront->Visible=false;
        btnMoveToRear->Visible=false;
    }
    bIndexMoveToFrontRear=false;

    if(CosFunction.bOEEFunction)                                                //Steven 20180417 (Jou) : OEE功能      //Sam 20210412 : 超豐新增軟體開啟時要丟訊息
    {
        if(IniConfig.bN14_14_AlarmCtrlMachine)                                  //Sam 20171212 (Steven) : 超豐良率監控
        {
            AnsiString sMessage=AnsiString().sprintf("Alarm,%s,%s", fNote->edErrorCode->Text, fNote->ShowMessageEdit1->Text);
            fProductionInfo->SetACM_WriteMsgAndCallExe_sMessage(sMessage);      //jimmychiu 20220622 add
        }

        if(edErrorCode->Text=="WAR0718" || edErrorCode->Text=="WAR0719" ||
           edErrorCode->Text=="WAR0722" || edErrorCode->Text=="WAR0723")        //Sam 20180423 (wei) : MOFile of Yeild Download
        {
            reDescription->Color=clRed;
        }
        else
        {
            reDescription->Color=clWindow;
        }
    }

    fShow=true;                                                                 //Steven 20140526 : 移到最下面
    if(IniConfig.bEnable_SECS_GEM==true)                                        //Steven 20140528 : Secs Gem
    {
        fMain->ShowNowStatus(clRed, "Alarm");

        if(sMachineState!=fMain->palMainStatus->Caption)
        {
            sMachinePreState=sMachineState;                                     // 2013.11.27 , Joye , KYEC SECS/GEM
            for(int i=0; i<30; i++)
            {
                if(sMachinePreState==sMacStatus[i])
                {
                    iSECSGEMMachinePreState=i+1;                                //Ifor 20221019 add: Machine PreS tate改成int
                    break;
                }
            }
            EventReport(SECS_EVENT.RunStatus);
            sMachineState=fMain->palMainStatus->Caption;
        }

        bool bIsJam=CheckRecordJamType(edErrorCode->Text, true);
        if(CUSTOMER_CODE==CC_ASE_CL && edErrorCode->Text=="MES0101")            //JerryYang 20250120 : add
        {
        }
        else
        {
            HGem->ReportAlarm(edErrorCode->Text, bIsJam, iDuplicateError, ShowMessageEdit1->Text);                      //kevin 20170905 (Steven) 換成Edit 可以看後面字串
        }
    }

    if(CUSTOMER_CODE==CC_SCK && IniConfig.bN05SCKWebService)                    //Steven 20161101 : SCK SOAP
    {
        //Type|Jam_code|handler_Id|Jam_description
        Str.sprintf("HT|%s|%s|%s", edErrorCode->Text, IniConfig.SocketHandlerID, ShowMessageEdit1->Text);               //kevin 20170905 (Steven) 換成Edit 可以看後面字串
        try
        {
            fWebService->SIPService->InputJam(Str);
        }
        catch(...)
        {
        }
    }
    else if(CosFunction.bCreateManualEOCAP==true &&
            TestIF_File.bCreateManualEOCAP==true)                               //jou 20221104 : VTest CreateManualEOCAP function;
    {
        if(bRedFlag==true)
        {
            BtnStart->Visible=false;
            Str.sprintf("%s:%s", edErrorCode->Text,asMesMessage);               //kevin 20170905 (Steven) 換成Edit 可以看後面字串
            fMesSystem->SendJamCode(Str);
        }
        else
        {
            fMesSystem->bCreateManualEOCAP_OK=false;
        }
    }

    if(IniConfig.bSPILFunction==true &&
       TrayForm.iManualRemoveLoader==1 &&                                       //JerryYang 20200317 當發生以下Alarm後，要求人員手動取出loader tray
      (edErrorCode->Text=="MES0101" || edErrorCode->Text=="JAM0114" ||
       edErrorCode->Text=="JAM0126" || edErrorCode->Text=="JAM0127" ||
       edErrorCode->Text=="JAM0160" || edErrorCode->Text=="JAM0161" ||
       edErrorCode->Text=="JAM0162" || edErrorCode->Text=="JAM0163"))           //JerryYang 20160516 add JAM0128
    {
        bLoaderHasSkip=true;
        if(CosFunction.bNextTrayNeedManualRemoveTray==true)                     //Sam 20190731 : 當 Load 發生 Pickup Error Skip 時，當前這盤除了要手動移除，下一盤也要手動移除。
        {
            MaualRemoveTray3Alarm.SetTakePickUpError();
        }
    }

    if(IniConfig.bF34OutShtPickErrSetErrBin)                                    //F33功能跟F19衝突
    {
        if(edErrorCode->Text=="JAM0201" || edErrorCode->Text=="JAM0202")
        {
            fMain->BtnResetClick(fMain);
        }
    }

//    if(CosFunction.bUseSCKART)                                                  //Steven 20161201 (wei) : For SCK 93K ART
//    {
        fSCKART->AddAlarmCode(edErrorCode->Text, fNote->ShowMessageEdit1->Text, iDuplicateError);
//    }

    if((IniConfig.bSPILFunction==true) &&                                       //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
        IniConfig.bFTPJamCodeUpload  &&
        IniConfig.bEnableFTP)                                                   //Steven 20140526 : bSendJamCodeToFTP 改為Timer處理
        bSendJamCodeToFTP=true;

    if(CUSTOMER_CODE==CC_SIGURD_PeiXing || CUSTOMER_CODE==CC_TERAPOWER)         //Alick 20160825 add for SIGURD PeiXing 要求NOTE畫面增加ALARMRESET按鈕功能
        BtnAlarmReset->Visible=true;
    else
        BtnAlarmReset->Visible=false;

    palOutShuttleLossIC->Visible=false;                                         //KaiChen 20200108 ：矽格-湖口，發生 OutShuttle Loss IC 時，檢查Index區提示加大
    if(CUSTOMER_CODE==CC_SIGURD_HUKOU ||                                        //KaiChen 20200108 ：矽格-湖口，發生 OutShuttle Loss IC 時，檢查Index區提示加大
       CUSTOMER_CODE==CC_SIGURD_SUZHOU)
    {
        if(edErrorCode->Text=="JAM0508" ||
           edErrorCode->Text=="JAM0509")
        {
            if(edErrorCode->Text=="JAM0508")
            {
                labOutShuttleLossIEN->Caption="Check the Index Arm 1 nozzle for IC";
                labOutShuttleLossICH->Caption="請檢查 Index Arm 1 吸嘴，是否殘留 IC";
            }
            else if(edErrorCode->Text=="JAM0509")
            {
                labOutShuttleLossIEN->Caption="Check the Index Arm 2 nozzle for IC";
                labOutShuttleLossICH->Caption="請檢查 Index Arm 2 吸嘴，是否殘留 IC";
            }
            palOutShuttleLossIC->Visible=true;
        }
    }

    if(IniConfig.bP59UnloaderICFloattingAlarmAfterExit)                         //Sam 20250415 : Unloader 偵測到置偏 IC 退出後再報警
    {
        iICFloattingCheckStep=0;
        iICFloattingCheckPos=0;
        if(edErrorCode->Text=="JAM1110")
        {
            iICFloattingCheckStep=3;
            iICFloattingCheckPos=0;
        }
        else if(edErrorCode->Text=="JAM1210")
        {
            iICFloattingCheckStep=3;
            iICFloattingCheckPos=1;
        }
        else if(edErrorCode->Text=="JAM1310")
        {
            iICFloattingCheckStep=3;
            iICFloattingCheckPos=2;
        }

        if(iICFloattingCheckStep==3)
            ShowUnloaderChkMsg(iICFloattingCheckStep);
    }

    if(IniConfig.bSIGURDFunction==true)                                         //KaiChen 20210127 ：矽格全開 //KaiChen 20200108 ：矽格-CheckList 增加顯示異常數值
    {
        if(edErrorCode->Text=="WAR16333")
        {
            palSGCheckList->Visible=true;                                       //Sam 20240425 : 修正 [A32] 功能報警未顯示問題
            labSG_CheckList_Check   ->Visible=true;                             //Sam 20221103 : OneCycle 完後顯示訊息
            labSG_CheckList_Current ->Visible=true;
            labSG_OneCycleMsg       ->Visible=false;
            labSG_CheckList_Check   ->Caption="CheckList  -->"+CheckListValue_Check;
            labSG_CheckList_Current ->Caption="Current      -->"+CheckListValue_Current;
            palSGCheckList->Parent=tsHandler;
            palSGCheckList->BringToFront();
        }
    }

    if(iOneCycleFinishShowMsg==1)                                               //Sam 20250115 : 矽格湖口 GPIB OneCycle 要強制切 ASM //Sam 20221103 : OneCycle 完後顯示訊息
    {
        if(edErrorCode->Text=="MES1640")
        {
            palSGCheckList->Visible=true;                                       //Sam 20240425 : 修正 [A32] 功能報警未顯示問題
            iOneCycleFinishShowMsg=2;                                           //Sam 20250115 : 矽格湖口 GPIB OneCycle 要強制切 ASM
            labSG_CheckList_Check   ->Visible=false;
            labSG_CheckList_Current ->Visible=false;
            labSG_OneCycleMsg       ->Visible=true;
            labSG_OneCycleMsg       ->Caption=sOneCycleFinishShowMsg;
            labSG_OneCycleMsg->Left=8;                                          //Sam 20230809 : 聯測需要多行顯示
            labSG_OneCycleMsg->Top=8;
            labSG_OneCycleMsg->Alignment=taLeftJustify;
            labSG_OneCycleMsg->Height=105;
            labSG_OneCycleMsg->Width=633;
            labSG_OneCycleMsg->WordWrap=true;
            palSGCheckList->Parent=tsHandler;
            palSGCheckList->BringToFront();
        }
    }
    pnlStopTime->Visible=CosFunction.bShowHandlerStopTime;                      //jou 2014-09-21 Show Handler Stop Time
    lHandlerStopTime.LatchCycleTime(true);                                      //jou 2014-09-21 Show Handler Stop Time
    iOldKey=-1;                                                                 //Steven 20151015 : To avoid double click

    fLotInfo->bEventLogAlarm=true;

    if(CUSTOMER_CODE==CC_GIGAS)                                                 //Isaac 20191225 : add alarm後，丟alarm code給客戶指定的ELOAD.exe
    {
        #ifndef SOFT_SIMULTE
        if(FileExists("c:\\ECLOAD.exe")==true)
            ShellExecute(NULL,"open","c:\\ECLOAD.exe",(edErrorCode->Text.c_str()),NULL, SW_HIDE);
        else
            ShowMyMessage("'ECLOAD.exe' not exist, please check on C:\ ", "'ECLOAD.exe'不存在，請確認C槽是否有此檔案");
        #endif
    }

    if(CosFunction.bAfterOneCycleShowTrayQty)                                   //Sam 20221101 : OneCycle 完成後顯示所有 Tray 各別的IC數量
    {
        if(edErrorCode->Text=="MES1640")
            ShowTrayQty();
    }

    int iLeft=10;
    if(IsBinCodeEditScanEnabled())
    {
        for(int i=0; i<TEST_MAX_BIN; i++)
        {
            myBinCodeEdit[i]->Parent=sbBinEdit;
            myBinCodeEdit[i]->Width=150;                                        //JerryYang 20241118 : fix
            myBinCodeEdit[i]->Height=20;
            myBinCodeEdit[i]->Top=20;
            myBinCodeEdit[i]->Font->Size=12;
            myBinCodeEdit[i]->Left=0;
            myBinCodeEdit[i]->Visible=false;
            myBinCodeEdit[i]->Text="";
            myBinCodeEdit[i]->Enabled=true;
        }

        int iBinCntMax=17;
        if(edErrorCode->Text=="MES1120" || edErrorCode->Text=="MES1220" || edErrorCode->Text=="MES1320" ||
           edErrorCode->Text=="MES1720" || edErrorCode->Text=="MES1820" || edErrorCode->Text=="MES1920" ||
           edErrorCode->Text=="MES1124" || edErrorCode->Text=="MES1224" || edErrorCode->Text=="MES1324" ||
           edErrorCode->Text=="MES1724" || edErrorCode->Text=="MES1824" || edErrorCode->Text=="MES1924" ||
           edErrorCode->Text=="MES1712" || edErrorCode->Text=="MES1812" || edErrorCode->Text=="MES1912")                //JerryYang 20250429 : fix Auto In/Out
        {
            bCheckBinOK=false;
            labCheckBin->Caption="Please keyin the bin code label.";
//            edtCheckBin->Text="";
            sbBinEdit->Visible=true;
            sbBinEdit->Parent=fNote;
            sbBinEdit->BringToFront();

            if(ShouldAutoScanBinLabelToEdit())                      //JerryYang 20240821 : 自動讀取BIN LABEL
            {
                if(edErrorCode->Text=="MES1120" || edErrorCode->Text=="MES1124" ||
                   edErrorCode->Text=="MES1220" || edErrorCode->Text=="MES1224" ||
                   edErrorCode->Text=="MES1320" || edErrorCode->Text=="MES1324")                                        //JerryYang 20250429 : fix Auto In/Out
                {
                    fNote->bScanBinLabel=true;
                }
            }
//            edtCheckBin->SetFocus();

            for(int i=0; i<=iTestBinCount; i++)
            {
                if(i==iTestBinCount)
                {
                    if(((edErrorCode->Text=="MES1120" || edErrorCode->Text=="MES1124") && Prod.iIfErrorT6==eAuto1) ||   //JerryYang 20240821 : modify
                       ((edErrorCode->Text=="MES1220" || edErrorCode->Text=="MES1224") && Prod.iIfErrorT6==eAuto2) ||
                       ((edErrorCode->Text=="MES1320" || edErrorCode->Text=="MES1324") && Prod.iIfErrorT6==eAuto3) ||
                       ((edErrorCode->Text=="MES1720" || edErrorCode->Text=="MES1724" || edErrorCode->Text=="MES1712") && Prod.iIfErrorT6==eFix1) ||
                       ((edErrorCode->Text=="MES1820" || edErrorCode->Text=="MES1824" || edErrorCode->Text=="MES1812") && Prod.iIfErrorT6==eFix2) ||
                       ((edErrorCode->Text=="MES1920" || edErrorCode->Text=="MES1924" || edErrorCode->Text=="MES1912") && Prod.iIfErrorT6==eFix3))              //JerryYang 20250429 : fix Auto In/Out
                    {
                        myBinCodeEdit[i]->Visible=true;
                        myBinCodeEdit[i]->Left=iLeft;
                        if(bSetFocus==false)
                        {
                            bSetFocus=true;
                            myBinCodeEdit[i]->SetFocus();
                        }
                    }
                }
                else if(i<iBinCntMax &&                                         //JerryYang 20241118 : fix
                        (((edErrorCode->Text=="MES1120" || edErrorCode->Text=="MES1124") && Prod.iT6CatData[i]==eAuto1) ||                                      //JerryYang 20250303 : fix
                         ((edErrorCode->Text=="MES1220" || edErrorCode->Text=="MES1224") && Prod.iT6CatData[i]==eAuto2) ||
                         ((edErrorCode->Text=="MES1320" || edErrorCode->Text=="MES1324") && Prod.iT6CatData[i]==eAuto3) ||
                         ((edErrorCode->Text=="MES1720" || edErrorCode->Text=="MES1724" || edErrorCode->Text=="MES1712") && Prod.iT6CatData[i]==eFix1) ||
                         ((edErrorCode->Text=="MES1820" || edErrorCode->Text=="MES1824" || edErrorCode->Text=="MES1812") && Prod.iT6CatData[i]==eFix2) ||
                         ((edErrorCode->Text=="MES1920" || edErrorCode->Text=="MES1924" || edErrorCode->Text=="MES1912") && Prod.iT6CatData[i]==eFix3)))        //JerryYang 20250429 : fix Auto In/Out
                {
                    myBinCodeEdit[i]->Visible=true;
                    myBinCodeEdit[i]->Left=iLeft;
                    iLeft=iLeft+myBinCodeEdit[i]->Width+20;
                    if(bSetFocus==false)
                    {
                        bSetFocus=true;
                        myBinCodeEdit[i]->SetFocus();
                    }
                }
                else
                {
                    myBinCodeEdit[i]->Visible=false;
                }
            }
        }
        else
        {
            bCheckBinOK=true;
            sbBinEdit->Visible=false;
        }
    }
    else
    {
        sbBinEdit->Visible=false;
        bCheckBinOK=true;
    }
    lblBundleIDErr->Visible=false;
    lblBunIDNotInList->Visible=false;
    lblBundleUnloadIDuplicted->Visible=false;

    iLogTorque1=-1;
    iStopLogTorque1=-1;                                                         //kevin 20211207 MCU Stop

    if((TempCode=="MES1712" || TempCode=="MES1812" || TempCode=="MES1912") &&
       bSetFocus==false)
    {
        bSetFocus=true;
        fNote->edBundleID->SetFocus();                                          //RogerYang 20250304
    }
    else if(TrayForm.bVTestNoRTBin &&                                           //RogerYang 20250626 偉測不可複測bin功能
            (TempCode=="MES1713"   ||
             TempCode=="MES1813"   ||
             TempCode=="MES1913")  &&
             bSetFocus==false)
    {
        if(TempCode=="MES1713")
            iNoRTBinIdx=0;
        else if(TempCode=="MES1813")
            iNoRTBinIdx=1;
        else if(TempCode=="MES1913")
            iNoRTBinIdx=2;

        Str.sprintf("Please keyin the NoRTBin ID for Fix%d!", iNoRTBinIdx+1);
        lblBundleText->Caption=Str;
        Str.sprintf("Fix%d ID", iNoRTBinIdx+1);
        lblBundleID->Caption=Str;
        lblBunIDNotInList->Caption="This ID is not match "+Str+" Setting!!";

        bSetFocus=true;
        fNote->edBundleID->SetFocus();
    }

    if(IsSafePLCIOInstall() &&
       edErrorCode->Text=="WAR16125")                                           //KenHsieh 20250307 : fix PLC safedoor 通訊延遲問題
    {
        bPLCFlag=true;
    }
}
//---------------------------------------------------------------------------
extern HAlarm *Alarm;
extern TQPF_Timer tGalilTwoYMoveDelay;
void __fastcall TfNote::FormClose(TObject *Sender, TCloseAction &Action)
{
    bool *bPtr[9]={&bLampSkip, &bLampRetry, &bLampTrayFeed, &bLampTrayEnd, &bLampCleanOut, &bLampReset, &bLampHome, &bLampTrain,&bLampFix};                     //kevin 20130311  ChungHung HT9045 2011/12/13 //Input pickup device error時,按"retry"鍵,機台都會自動home

    fSCKART->palARTCount->Parent=fSCKART->palARTLeft;                           //Steven 20170111 (wei) : Add button for SCK ART
    fSCKART->palARTCount->Align=alTop;
    fSCKART->SettingPanelOnOff(false);
    btPrintSummary->Visible=false;
    pnlCorrectionCount->Visible=false;
    AnsiString sMessage="";
    bShowNoteCleanSocket=false;                                                 //Sam 20230111 : Smart Auto Clean
    if(CosFunction.bOEEFunction)                                                //Steven 20180417 (Jou) : OEE功能
    {
        fProductionInfo->bFTPError = false;
    }

    fShow=false;
    bSendJamCodeToFTP=false;
    bNeedPassWord=false;
    bAlarmReset=false;                                                          //Steven 20140905 : 紀錄有被按下Alarm Reset
    for(int i=0; i<9; i++)
        *bPtr[i]=Select[i];
    bSECSGEM_NoteAlarm=false;                                                   //Ifor 20170616 (wei) add note From Close時清除SECSGEM Note Alarm 旗標
    bAlarmAfterPreAlarm=false;                                                  //Ifor 20170906 (wei) add 避免 PreAlarm -> Alarm -> SECS GEM Alarm 同時發生造成當機問題

    if(AlarmType==1 && LastSet.iRealDummy==REALLY && !bLampTrayEnd && iDuplicateError!=1)
        CheckRecordJamType(edErrorCode->Text);

    Alarm->Clear();
    if(iGali_VsSpeed<=0)    iGali_VsSpeed=30000;
    if(iGali_SpSpeed<=0)    iGali_SpSpeed=10000;
    bLampSkip=false;
    bLampRetry=false;
    bLampOneCycle=false;
    bLampCleanOut=false;
    bLampTrayFeed=false;
    bLampTrayEnd=false;
    bLampReset=false;
    bLampHome=false;                                                            //ChungHung HT9045 2011/12/13 //Input pickup device error時,按"retry"鍵,機台都會自動home
    bLampTrain=false;
    bLampFix=false;                                                             //kevin 20130312
    SW[SwManualZ1].Off();
    PassTime=tNoteTimer.LatchCycleTimeSec();
    bScanBinLabel=false;                                                        //JerryYang 20250429 : fix Auto In/Out

    if(KeyCode==0)                                                              //Steven : 沒有KeyCode的話,不用Recovery
        Recovery="";

    if(CUSTOMER_CODE!=CC_KYEC_LEE && IniConfig.bEnable_SECS_GEM==true)          //JerryYang 20170504 (Steven) SPIL騰清說解除alarm也需上報alarm report
    {
        bool bIsJam=CheckRecordJamType(edErrorCode->Text, true);
        HGem->ReportAlarm(edErrorCode->Text, bIsJam, iDuplicateError, ShowMessageEdit1->Text, true, Recovery);          //kevin 20170905 (Steven) 換成Edit 可以看後面字串
    }

    MyDBUEventRecover(iEventID, Recovery, PassTime);                            //Steven 20090819
    fNote->Reset();                                                             //Steven 20090909
    StopMovie();
    palShtSensorSOP->Visible=false;                                             //Steven 20100319
    bShowSpecialPan=false;                                                      // 2011.04.11 , Q_Q
    bEnterTestIF=true;

    lHandlerStopTime.LatchCycleTime(true);                                      //jou 2014-09-21 Show Handler Stop Time

    AnsiString Str="";
    int iSkipICCount=0;

    if(IniConfig.bA67TriggerOneCycleWhenAlarm)                                  //JerryYang 20241028 : 矽品彰化要求 特定ALARM要觸發ONE CYCLE
    {
        if(fNote->ReturnCode==K_SKIP)
        {
            if(edErrorCode->Text=="JAM0126" || edErrorCode->Text=="JAM0203" || edErrorCode->Text=="MES0101" ||
                edErrorCode->Text=="JAM0301" || edErrorCode->Text=="JAM0302" ||
                edErrorCode->Text=="JAM0303" || edErrorCode->Text=="JAM0304" ||
                edErrorCode->Text=="JAM0508" || edErrorCode->Text=="JAM0509" ||
                edErrorCode->Text=="JAM0305" || edErrorCode->Text=="JAM0306" ||
                edErrorCode->Text=="JAM0401" || edErrorCode->Text=="JAM0404" ||
                edErrorCode->Text=="JAM0201" || edErrorCode->Text=="JAM0202")
            {
                fMain->BtnOneCycleClick(fMain);
            }
        }
    }

    if(CosFunction.bOEEFunction &&
       IniConfig.bN14_14_AlarmCtrlMachine)                                      //Sam 20210412 : 超豐新增軟體開啟時要丟訊息
    {
        sMessage.sprintf("Alarm,%s,%s", edErrorCode->Text, ShowMessageEdit1->Text);
        if(fNote->ReturnCode==K_SKIP)
            sMessage+=",Action:Skip";
        else if(fNote->ReturnCode==K_RETRY)
            sMessage+=",Action:Retry";
        else if(fNote->ReturnCode==K_TRAY_FEED)
            sMessage+=",Action:TrayFeed";
        else if(fNote->ReturnCode==K_TRAY_END)
            sMessage+=",Action:TrayEnd";
        else if(fNote->ReturnCode==K_CLEAN_OUT)
            sMessage+=",Action:CleanOut";
        else if(fNote->ReturnCode==K_ONECYCLE)
            sMessage+=",Action:OneCycle";
        else if(fNote->ReturnCode==K_RESET)
            sMessage+=",Action:Reset";
        fProductionInfo->ACM_WriteMsgAndCallExe(sMessage);
    }

    if(CUSTOMER_CODE==CC_KYEC_LEE)                                              //Ifor 20160310 海思版本不顯示
    {
        if(bAutoRetestJam &&
           (fNote->ReturnCode==K_SKIP ||
            bNeedKeyInSkipIC ||
            bAutoCount_Reset==true))                                            //wei 20160302 Jam Skip輸入顆數
        {
            bAutoCount_Reset=false;
            if(fNote->ReturnCode==K_SKIP ||
               fNote->ReturnCode==K_RESET)                                      //Ifor 20171228 : add 非 SKIP與RESET按鈕不累加 SKIP COUNT
            {
                if(bNeedKeyInSkipIC)
                {
                    Str=ShowMyInputSkip("請輸入Tray上遺漏IC顆數","Skip IC Count:");
                    if(Str=="NA")
                        return;
                    else
                        bNeedKeyInSkipIC=false;
                }

                Str.printf("%s Skip IC count : %d", sAlarmMes, iJamSkipICCount);
                RecordProcess(Str.c_str());

                iJamSkipIC=iJamSkipICCount;
                EventReport(SECS_EVENT.JamSkipICCount);                         //wei 20160503 Jam Skip IC Count

                if(CUSTOMER_CODE==CC_KYEC_LEE &&
                   (CosFunction.bUseMRTMode ||                                  //Ifor 20170413 add MRT Mode Use ART Skip IC Count顯示 改到下個版本
                    (USE_AUTO_RETEST==eartInstall && IniConfig.bA10_AutoReTest && bCleanSkipICCount==false) ||
                    (CosFunction.bUseARTSortCount==true && bCleanSkipICCount==false)))
                {
                    iJamSkipICCount=iJamSkipICCount+atoi(fShowBinSelect->StrARTSkipICCount->Cells[1][LastSet.iAutoRetestCount_ART+1].c_str());
                    fShowBinSelect->StrARTSkipICCount->Cells[1][LastSet.iAutoRetestCount_ART+1]=iJamSkipICCount;
                }

                for(int j=1; j<21; j++)
                {
                    iSkipICCount=iSkipICCount+atoi(fShowBinSelect->StrARTSkipICCount->Cells[1][j].c_str());
                }
                fShowBinSelect->StrARTSkipICCount->Cells[1][21]=iSkipICCount;
            }
            bNeedKeyInSkipIC=false;                                             //Ifor 20171228 : add 避免下次Alarm 出現 輸入IC的視窗
        }
    }
    iJamSkipICCount=0;
    sAlarmMes="";                                                               //wei 20160407 Alarm Message
    bAutoRetestJam=false;
    bOpenAllDoor=true;
    for(int i=0; i<InArmSuck.iShtRow; i++)
    {
        for(int j=0; j<InArmSuck.iShtCol; j++)
        {
            fNote->t2DCode->SetCellNumber(i, j, "");
            fNote->t2DCode->SetCellColorIndex(i, j, 0);
        }
    }

    bInArmNeedToSafePos=false;                                                  //Steven 20171204 : 加上保護,避免Flag沒轉換造成Hang Up

    if(CosFunction.bDownloadUpdateAutomatically &&
       IniConfig.bN32_CheckAtTrayFeedFinish)                                    //Sam 20220824 : FTP 自動下載安裝更新包
    {
        if(edErrorCode->Text=="MES1644")
            fFTPClient->DownloadUpdateAutomatically();
    }

    edErrorCode->Text="";

    if(bCloseShowMsg)                                                           //Sam 20230426 : 通知系統 Handler 已經密碼鎖定
    {
        ShowMyMessage("!!!Please follow the SOP to deal with the found IC. and it is strictly forbiddento dispose of the found IC without permission!!!<Private handling will cause mixing problems> ", "!!!拾獲IC請遵循SOP處理，嚴禁私自處理拾獲IC!!!<私自處理會有混料問題產生>");
        bCloseShowMsg=false;
    }

    if(fNote->TempCode=="MES1712" || fNote->TempCode=="MES1812" || fNote->TempCode=="MES1912")
    {
        if(fNote->TempCode=="MES1712")                                          //Bundle 12= 10 + 1 black +1 2D cover tray
        {
            sFixBundleID[0]=fNote->edBundleID->Text;
            if(iFixTrayCountCal[0]>=fSCKART->iBundleOutCnt-2 || bUnloading==true)
            {
                sUnloadBundleID=fNote->edBundleID->Text;
//                sFixBundleID[0]=fNote->edBundleID->Text;
                sBundleEndInfo=GetBundleInfo(6);
                EventReport(SECS_EVENT.BundleEnd_Fix1);
                EventReport(SECS_EVENT.BundleEnd_IDREAD_Fix1);
                iFixTrayCountCal[0]=0;

                slDupUnloadBundlID->Clear();
                slDupUnloadBundlID->LoadFromFile(aslDupUnloadBundlID);
                slDupUnloadBundlID->Add(sUnloadBundleID);
                slDupUnloadBundlID->SaveToFile(aslDupUnloadBundlID);

                sFixBundleID[0]="";

                RecordProcess("Fix1 Bundle End event report finished.");

                bNeedReportBundleID[eFix1]=false;                               //JerryYang 20250220 : fix AUTO IN OUT
            }
        }
        else if(fNote->TempCode=="MES1812")
        {
            sFixBundleID[1]=fNote->edBundleID->Text;
            if(iFixTrayCountCal[1]>=fSCKART->iBundleOutCnt-2 || bUnloading==true)
            {
                sUnloadBundleID=fNote->edBundleID->Text;
//                sFixBundleID[1]=fNote->edBundleID->Text;
                sBundleEndInfo=GetBundleInfo(7);
                EventReport(SECS_EVENT.BundleEnd_Fix2);
                EventReport(SECS_EVENT.BundleEnd_IDREAD_Fix2);

                slDupUnloadBundlID->Clear();
                slDupUnloadBundlID->LoadFromFile(aslDupUnloadBundlID);
                slDupUnloadBundlID->Add(sUnloadBundleID);
                slDupUnloadBundlID->SaveToFile(aslDupUnloadBundlID);

                sFixBundleID[1]="";
                RecordProcess("Fix2 Bundle End event report finished.");
                bNeedReportBundleID[eFix2]=false;                               //JerryYang 20250220 : fix AUTO IN OUT
            }
        }
        else if(fNote->TempCode=="MES1912")
        {
            sFixBundleID[2]=fNote->edBundleID->Text;
            if(iFixTrayCountCal[2]>=fSCKART->iBundleOutCnt-2 || bUnloading==true)
            {
                sUnloadBundleID=fNote->edBundleID->Text;
//                sFixBundleID[2]=fNote->edBundleID->Text;
                sBundleEndInfo=GetBundleInfo(8);
                EventReport(SECS_EVENT.BundleEnd_Fix3);
                EventReport(SECS_EVENT.BundleEnd_IDREAD_Fix3);

                slDupUnloadBundlID->Clear();
                slDupUnloadBundlID->LoadFromFile(aslDupUnloadBundlID);
                slDupUnloadBundlID->Add(sUnloadBundleID);
                slDupUnloadBundlID->SaveToFile(aslDupUnloadBundlID);

                sFixBundleID[2]="";
                RecordProcess("Fix3 Bundle End event report finished.");
                bNeedReportBundleID[eFix3]=false;                               //JerryYang 20250220 : fix AUTO IN OUT
            }
        }
    }

    if(IniConfig.bL46_AStreamErrorCompressOnecycle)                             //Ztex 2024.10.01 Add AStream Error Compress Onecycle
    {
        if(iAStreamErrorCompressOnecycle==3)
        {
            iAStreamErrorCompressOnecycle=0;
            fMain->Start("bL46_AStreamErrorCompressOnecycle");
        }
    }
    hAutoCleanHangUp.SetSecAndOn(Prod.iHangupMaxTime);                          //Steven 20220823 : 機台有暫停就要重新計算
    tGalilTwoYMoveDelay.SetSecAndOn(60);
    bPLCFlag=false;                                                             //KenHsieh 20250307 : fix PLC safedoor 通訊延遲問題
}
//---------------------------------------------------------------------------
void __fastcall TfNote::UpdateButtonStatus(TObject *Sender)
{
    TBtnPanel *Ptr[]={BtnSkip, BtnRetry, BtnTrayFeed, BtnTrayEnd, BtnCleanOut, BtnReset, BtnHome, BtnTrain,BtnOneCycle};                                        //ChungHung 20140730 add ContinuousFailHaveOneCycle //kevin 20130722 cancel K_FIX //kevin 20130226 add FIX   //ChungHung HT9045 2011/12/13 Input pickup device error時,按"retry"鍵,機台都會自動home add Home
    TBtnPanel *Ptr2;
    AnsiString KeyName[]={"SKIP", "RETRY", "TRAY_FEED", "TRAY_END", "CLEAN_OUT", "RESET", "HOME","Train","ONECYCLE"};   //ChungHung 20140730 add ContinuousFailHaveOneCycle //kevin 20130722 cancel K_FIX //kevin 20130226 add FIX  ChungHung HT9045 2011/12/13 Input pickup device error時,按"retry"鍵,機台都會自動home add Home
    int i, ret=0;                                                               //JerryYang 20151202
    Ptr2=(TBtnPanel *) Sender;
    Tag=Ptr2->Tag-BtnSkip->Tag;
    if(IsSafeLockCheck())
        return;
    #ifndef SOFT_SIMULTE

    if(IsTestSitICFallDown())                                                   //jou 2011-10-31 start : 有掉料的話，要開Chamber門和按Z1才能接收按鍵
        return;
    #endif

    if(CUSTOMER_CODE!=CC_SCK &&
       IniConfig.bIndexJamInArmAway==true &&
       IniConfig.bD40IndexICFallDownMustPressFMotorDown==true)                  //jou 2011-10-14 start : 掉料需開啟Chamber門
    {
        if(bOpenChamberDoor==false && bIsTestSitICFallDown==true)               //wei 20151030 add  bIsTestSitICFallDown==true
        {
            return;
        }
    }

    if(bAutoRetestJam && bOpenAllDoor==false)                                   //wei 20160407 Alarm 後需要開門確認
        return;

    if((iHWFix_BinBox==1 || bCancelErrorBin) &&
        bOpenSixDoor && (bBinError[0] || bBinError[1]))                         //kevin 20160819 add error bin box
    {
        return;                                                                 //kevin 20160725 bin error 需開右後方門確認
    }

    if(fNote->TempCode=="MES0923" && CUSTOMER_CODE==CC_KYEC_LEE)
    {
        bAutoRetestJam=true;
        bNeedKeyInSkipIC=true;
    }

    for(i=0; i<9; i++)                                                          //ChungHung 20140730 add ContinuousFailHaveOneCycle 8--->9  //kevin 20130722 cancel K_FIX //kevin 20130312
    {
        if(i!=Tag)
        {
            Select[i]=false;
            Ptr[i]->FalseColor=(TColor)0x00804000;
        }
        else
        {
            Select[i]=true;
            Ptr[i]->FalseColor=clRed;
            Recovery = KeyName[i];
            if(IniConfig.bG08VisibleAreyousure &&                               //wei 20160413 顯示Are you sure
               CUSTOMER_CODE==CC_TSMC_TAINAN &&
               (Recovery=="SKIP" || Recovery=="RETRY" ))                        //JerryYang 20151202 Add for TSMC 點SKIP或是RETRY都要再跳一次確認
            {
                ret=ShowMyMessageBox_YES_NO("Are you sure?", "確定嗎?");
                if(ret==2)
                {
                    Select[i]=false;
                    Ptr[i]->FalseColor=(TColor)0x00804000;
                    Recovery="";
                    bScanKeyNo=true;
                    return;
                }
                else if(ret==1)
                {
                    bScanKeyNo=false;
                }
            }
            else
            {
                 bScanKeyNo=false;
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfNote::BtnSkipClick(TObject *Sender)
{
    if(bErrPan_err==true && Pwd!="")
        return;                                                                 //Richard 2011/2/22 SpecialPanel
    #ifdef SOFT_SIMULTE
        if(IsSafeLockCheck())
            return ;
    #else
        if(IsTestSitICFallDown())
            return ;                                                            //jou 2011-12-20 修正曾經發生過掉料的話,note訊息沒辦法操作的問題
        if(bContactCTOverCHK)
            return ;

        if(bContactCTOverCHK ||
           bAutoCleanCheckOpenDoor ||                                           //Steven 20121015 : Auto Clean有Alarm要開後門
           bChangeCleanPad)                                                     //kevin 20121020 add
            return;

        if(IniConfig.bP59UnloaderICFloattingAlarmAfterExit &&                   //Sam 20250415 : Unloader 偵測到置偏 IC 退出後再報警
           iICFloattingCheckStep!=0)
            return;
    #endif

    UpdateButtonStatus(Sender);
    #ifdef SOFT_SIMULTE
        BtnPauseClick(this);
    #endif
}
//---------------------------------------------------------------------------
void __fastcall TfNote::ScanKey()
{
    TBtnPanel *Ptr[]  ={BtnSkip, BtnRetry, BtnTrayFeed, BtnTrayEnd, BtnCleanOut, BtnReset, BtnHome, BtnTrain, BtnOneCycle};                                     //ChungHung 20140730 add ContinuousFailHaveOneCycle //kevin 20130722 cancel K_FIX //kevin 20130226 add FIX     //ChungHung HT9045 2011/12/13 Input pickup device error時,按"retry"鍵,機台都會自動home add Home
    int Index2[]={SnFKSkip, SnFKRetry, SnFKTrayFeed, SnFKTrayEnd, SnFKCleanOut, SnFKReset, SnFKHome, K_TRAIN, SnFKOneCycle};                                    //ChungHung 20140730 add ContinuousFailHaveOneCycle //kevin 20130722 cancel K_FIX   //ChungHung HT9045 2011/12/13 Input pickup device error時,按"retry"鍵,機台都會自動home add Home
    int Key, i;

    Key=ScanPannelKey();

    if(Key==-1)
        return;

    if(CUSTOMER_CODE==CC_ASE_SG && bTesterSendPause==true && Key==SnFKStart)
        return;

    if(bErrPan_err==true &&
       Key!=SnFKAlarmReset &&
       Key!=SnRKAlarmReset &&
       Pwd!="")
    {
        return;                                                                 //Richard 2011/2/22 SpecialPanel
    }

    if(bAutoRetestJam && bOpenAllDoor==false)                                   //wei 20160407 Alarm 後需要開門確認
    {
        return;
    }

    if(CosFunction.bOpenDoorCheckLoaderAfterTrayEnd &&                          //Jimmychiu 20250826 : Open Door Check Loader After TrayEnd
       bOpenLeftDoor==false &&                                                  //Steven 20130819 : 加入開門檢查的功能
       Key!=SnFKAlarmReset &&
       Key!=SnRKAlarmReset)
    {
        return;
    }

    if(TrayForm.iManualRemoveLoader==2 &&
       bOpenLeftDoor==false &&                                                  //Steven 20130819 : 加入開門檢查的功能
       Key!=SnFKAlarmReset &&
       Key!=SnRKAlarmReset)
    {
        return;
    }

    if(CosFunction.bPickupErrorAtLoaderNeedOpenDoor &&
       (TrayForm.iManualRemoveLoader==2 ||
        IniConfig.bE87PickupErrorAtLoaderNeedOpenDoor) &&                       //JerryYang 20250120 : add
       bOpenLeftDoor==false &&
       Key!=SnFKAlarmReset &&
       Key!=SnRKAlarmReset &&
       Key!=SnRKRetry &&
       Key!=SnFKRetry)                                                          //JerryYang 20160220 add for Amkor每次In Arm至loader吸取異常都要開門檢查
    {
        return;
    }

    if(CosFunction.bEnableHandlerResultServer &&
       bNeedTCPAlarm &&
       Key!=SnFKAlarmReset &&
       Key!=SnRKAlarmReset)                                                     //Sam 20230620 : 面板已經鎖定需要 IT 下命令解鎖
    {
        return;
    }

    if(IniConfig.bP59UnloaderICFloattingAlarmAfterExit &&                       //Sam 20250415 : Unloader 偵測到置偏 IC 退出後再報警
       iICFloattingCheckStep!=0)
        return;

    if(iOldKey==Key)                                                            //Steven 20151015 : To avoid double click //Steven 20191205 : 移到下面
        return;
    iOldKey=Key;

    for(i=0; i<9; i++)                                                          //ChungHung 20140730 add ContinuousFailHaveOneCycle 8--->9  //kevin 20130722 cancel K_FIX
    {                                                                           //JerryYang 20161130 10->9,修正超出Ptr陣列大小
        if(Ptr[i]->Visible &&  Key==Index2[i])
        {
            UpdateButtonStatus(Ptr[i]);
            if(bScanKeyNo==true)                                                //JerryYang 20151202 Add for TSMC 點SKIP或是RETRY都要再跳一次確認
            {
                return;
            }
            bAlarmBuzzer=false;
            bLampAlarmReset=false;
            if(i==0)
            {
                NewRecordProcess("MES2120", "SKIP pressed", "Note_ScanKey");
                if(IniConfig.bEnable_SECS_GEM==true)                            //Steven 20140528 : Secs Gem
                    EventReport(SECS_EVENT.DoSkip);                             //29     按下 Skip
            }
            else if(i==1)
            {
                NewRecordProcess("MES2121", "RETRY pressed", "Note_ScanKey");
                if(IniConfig.bEnable_SECS_GEM==true)                            //Steven 20140528 : Secs Gem
                    EventReport(SECS_EVENT.DoRetry);                            //28     按下 Retry
            }
            else if(i==2)
            {
                NewRecordProcess("MES2118", "TRAY FEED pressed", "Note_ScanKey");
                if(IniConfig.bEnable_SECS_GEM==true)                            //Steven 20140528 : Secs Gem
                    EventReport(SECS_EVENT.DoTrayFeed);                         //32     按下 Tray Feed
            }
            else if(i==3)
            {
                NewRecordProcess("MES2119", "TRAY END pressed", "Note_ScanKey");
                if(Prod.bAfterShowAlarmMessageUseInitialDelay)                  //ChungHung Add for SCK request Loader Tray End 不用Initial delay time
                {
                    if(LastSet.iTemperature==Tempture_Hot)
                    {
                        if(edErrorCode->Text=="MES0101")
                            bDoAfterShowAlarmMessageUseInitialDelay=false;
                    }
                }

                if(IniConfig.bEnable_SECS_GEM==true)                            //Steven 20140528 : Secs Gem
                    EventReport(SECS_EVENT.DoTrayEnd);                          //31     按下 Tray End
            }
            else if(i==4)
            {
                NewRecordProcess("MES2114", "CLEAN OUT pressed", "Note_ScanKey");
                if(IniConfig.bEnable_SECS_GEM==true)                            //Steven 20140528 : Secs Gem
                    EventReport(SECS_EVENT.DoCleanOut);                         // 4     按下 Clean Out
            }
            else if(i==5)
            {
                if(bRunAutoClean)                                               //Jou 2014-03-11 : Auto Clean時不能Reset
                    return;

                NewRecordProcess("MES2113", "RESET pressed", "Note_ScanKey");   //jou 2010-11-18 功能鍵不能放在外面，會誤判

                if(CUSTOMER_CODE==CC_AMKOR_Philippines &&
                   (edErrorCode=="JAM0302" || edErrorCode=="JAM0301" ||
                    edErrorCode=="JAM0303" || edErrorCode=="JAM0304"))          //Frank 20171219 modify
                {
                }
                else
                {
                    if(CUSTOMER_CODE==CC_KYEC_JCTHIU)                          //Frank 20260710 ADD//Eastsun 20260710整合
                    {
                        if(edErrorCode->Text=="JAM0303" || edErrorCode->Text=="JAM0304")
                        {
                            iResetStopMachine=1;
                        }
                    }
                    BtnResetClick(this);
                }
            }
            else if(i==6)
            {
                NewRecordProcess("HOME & Retry pressed", "Note_ScanKey", "MES2122");
                if(IniConfig.bEnable_SECS_GEM==true)                            //Steven 20140528 : Secs Gem
                    EventReport(SECS_EVENT.DoHome);                             //25     按下 Home
            }
            else if(i==8)
            {
                NewRecordProcess("MES2115", "ONE CYCLE pressed", "Note_ScanKey");
                bManualOneCycle=true;                                           //Sam 20230309 : 避免觸發 OneCycle 後，OneCycle 未完成時又觸發 AutoClean 動作，AutoClean 做完並不會提示 OneCycle Finish
            }
        }
    }

    if(Key==SnFKStart)
    {
        if(BtnStart->Visible==false)
        {
            return;
        }

        if(IniConfig.bG14UseStartSoundAlarm && bStartMoveSpeed)                 //kevin 20201116  motor move speed 10 % 移鄧10 sec 系統暫停 恢復速度  //kevin 20201116
        {
            return;
        }

        if(IniConfig.bEnableAutoCleanFunction && TestIF_File.iAutoClean_Function)
        {
            if(TestIF_File.iAutoClean_Tray==eCKPos_CleanKit)                    //Clean Kit
            {
                if(TestIF_File.iShuttleMode==1 &&
                   TestIF_File.iShuttle_Sel==1 &&                               //jou 2012-07-31 Clean kit使用shuttle 1
                   TestIF.dSiteYPitch>6350 &&
                   IniConfig.bE43AutoCleanUseHotplate==false)
                {
                    ShowErrorMessage("WAR16102", K_RETRY, MMAutoClean);         //AutoClean Must Use ARM1
                    return;
                }
            }
            else
            {
                if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==0)  //kevin 20120518使用AUTOCLEAN 但只使用arm1要發alarm警告
                {
                    ShowErrorMessage("WAR16103", K_RETRY, MMAutoClean);         //AutoClean Must Use ARM2
                    return;
                }
            }
        }

        NewRecordProcess("MES2110", "START pressed", "Note_ScanKey");
        Start();
    }
    else if(Key==SnFKPause)
    {
        NewRecordProcess("MES2111", "PAUSE pressed", "Note_ScanKey");
        BtnPauseClick(this);
    }
    else if(Key==SnFKAlarmReset && bAlarmBuzzer)
    {
        NewRecordProcess("MES2116", "ALARM RESET pressed", "Note_ScanKey");
        bAlarmBuzzer=false;
        bLampAlarmReset=false;
        SECS_GEM_PPMUSIC_CONTROL_flag=false;
        SECS_GEM_PPSIGNALTOWER_CONTROL_flag=false;
        bAlarmReset=true;                                                       //Steven 20140905 : 紀錄有被按下Alarm Reset
        SW[SwFKAlarmReset].Off();
        SW[SwRKAlarmReset].Off();
        if(IniConfig.bEnable_SECS_GEM==true)                                    //Steven 20140528 : Secs Gem
            EventReport(SECS_EVENT.DoAlarmReset);                               //30     按下 Alarm Reset

        RespondASECom("@e02111Done");                                           //kevin 20150415 回應 ase AlarmReset finish
    }
}
//------------------------------------------------------------------------------
void __fastcall TfNote::FlushLabel()
{
    TBtnPanel *Ptr[]={BtnSkip, BtnRetry, BtnTrayFeed, BtnTrayEnd, BtnCleanOut, BtnReset, BtnHome, BtnTrain, BtnOneCycle, BtnAlarmReset};                        //ChungHung 20140730 add ContinuousFailHaveOneCycle //kevin 20130722 cancel K_FIX //kevin 20130226 add FIX  //ChungHung HT9045 2011/12/13 Input pickup device error時,按"retry"鍵,機台都會自動home add Home  //Alick 20160907 Add AlarmReset for 矽格北興
    bool      *bPtr[10]={&bLampSkip, &bLampRetry, &bLampTrayFeed, &bLampTrayEnd, &bLampCleanOut, &bLampReset, &bLampHome, &bLampTrain, &bLampOneCycle, &bLampAlarmReset};  //ChungHung 20140730 add ContinuousFailHaveOneCycle  //kevin 20130722 cancel K_FIX //kevin 20130311 ChungHung HT9045 2011/12/13 Input pickup device error時,按"retry"鍵,機台都會自動home add Home  //Alick 20160907 Add AlarmReset for 矽格北興
    static bool OldFlushFlag, flag=false;

    if(OldFlushFlag==FlushFlag)
        return;
    OldFlushFlag=FlushFlag;
    flag=false;
    for(int i=0; i<10; i++)                                                     //ChungHung 20140730 add ContinuousFailHaveOneCycle 8--->9  //kevin 20130312 7->9
    {
        if(Select[i])
        {
            flag=true;
            *bPtr[i]=true;
        }
        else
        {
            if(OldFlushFlag)
            {
                Ptr[i]->FalseColor=clBlue;
                if(Ptr[i]->Visible) *bPtr[i]=true;
            }
            else
            {
                Ptr[i]->FalseColor=(TColor)0x00804000;
                *bPtr[i]=false;
            }
        }
    }

    if((flag ||(KeyCode==0))&& OldFlushFlag)
    {
        BtnStart->FalseColor=clBlue;
        BtnPause->FalseColor=clBlue;
        if(IniConfig.bG14UseStartSoundAlarm && bStartMoveSpeed)                 //kevin 20201116  motor move speed 10 % 移鄧10 sec 系統暫停 恢復速度
            bLampStart=false;
        else
            bLampStart=true;
        bLampPause=true;
    }
    else
    {
        BtnStart->FalseColor=(TColor)0x00804000;
        BtnPause->FalseColor=(TColor)0x00804000;
        bLampStart=false;
        bLampPause=false;
    }

    if(bAlarmBuzzer)bLampAlarmReset=OldFlushFlag;
    else            bLampAlarmReset=false;

    if(bAlarmReset)                                                             //Alick 20160907 Modify for 矽格北興
        Ptr[9]->FalseColor=(TColor)0x00804000;
}
//------------------------------------------------------------------------------
TQPF_Timer OPCheckSocketTime;
TQPF_Timer OPCheckUnloaderTime;                                                 //Sam 20250415 : Unloader 偵測到置偏 IC 退出後再報警
void __fastcall TfNote::Timer1Timer(TObject *Sender)
{
    if(!fShow)
        return;
    if(bSECSGEMAlarm && bSECSGEM_NoteAlarm==false)                              //wei 20150817 S10F3 按鍵不能按//Ifor 20170616 (wei) add S10F3 後發生的Note Alarm 要恢復按鍵掃描解除Note後再鎖定按鍵
        return;
    if(InitialOK==false)                                                        //Ztex 2023.12.13 Add
        return;

    TBtnPanel *Ptr[]={BtnSkip, BtnRetry, BtnTrayFeed, BtnTrayEnd, BtnCleanOut, BtnReset, BtnHome, BtnTrain, BtnOneCycle};                                       //ChungHung 20140730 add ContinuousFailHaveOneCycle //kevin 20130722 cancel K_FIX //kevin 20130226 add FIX   //ChungHung HT9045 2011/12/13 Input pickup device error時,按"retry"鍵,機台都會自動home add Home
    static int i, kct[10]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0};                       //ChungHung 20140730 add ContinuousFailHaveOneCycle 9--->10
    int SenIndex[3]={SnAuto1TrayHasTray,SnAuto2TrayHasTray,SnAuto3TrayHasTray};
    AnsiString asSafedoor="";
    int iC_DoorLock[]={C_SafeDoor1Lock,C_SafeDoor2Lock,C_SafeDoor3Lock,C_SafeDoor4Lock,C_SafeDoor6Lock,C_SafeDoor7Lock,C_SafeDoor8Lock};                        //Ztex 2024.08.14 Add Open Door Check IC
    int iS_DoorLock[]={SnSafeDoor1,SnSafeDoor2,SnSafeDoor3,SnSafeDoor4,SnSafeDoor6,SnSafeDoor7,SnSafeDoor8};            //Ztex 2024.08.14 Add Open Door Check IC

    if(SystemStart ==false && TorqueUseHPComCard)                               //kevin 20211216 add MCU TORQUE
    {
        if(iLogTorque1!=0 && iStopLogTorque1!=1)                                //kevin 20211207 MCU Stop
        {
            COM2->iReadTorque_HPCardTask=1;                                     //Steven 20211116 : 改成一送一收
            iStopLogTorque1=COM2->StopReadTorque_HPCard();
            if( iStopLogTorque1==1)
            {
                iLogTorque1=0;
                iStopLogTorque1=0;
            }
        }
    }

    if(IniConfig.bIndexJamInArmAway==true &&                                    //jou 2011-10-14 start : 掉料需開啟Chamber門
       bOpenChamberDoor==false)
    {
        OPCheckSocketTime.SetSecAndOn(10);
        if(Sen[SnHeaterDoor2].Enable==true || Sen[SnHeaterDoor].Enable==true)
        {
            if((Sen[SnHeaterDoor2].Enable==true &&
                Sen[SnHeaterDoor2].IsOff()==true) ||
               (Sen[SnHeaterDoor].Enable==true &&
                Sen[SnHeaterDoor].IsOff()==true))                               //kevin 20130906 雲董講開任一個門,掉料需開啟Chamber門
            {
                bOpenChamberDoor=true;
                RecordProcess("Chamber Door Open!!");
            }
        }
        else
        {
            bOpenChamberDoor=true;
        }
    }

    if(IsSafePLCIOInstall()==true)                                              //kevin 20250407 check PLC
        bPLCStatusCheck();

    if(bAutoRetestJam && bOpenAllDoor==false)                                   //wei 20160407 Alarm 後需要開門確認
    {
        for(int i=0; i<MAX_SAFE_DOOR_CNT; i++)                                  //JerryYang 20230704 : 整合安全門15->MAX_SAFE_DOOR_CNT
        {
            if(Sen[iSafeDoor[i]].Enable==true)
            {
                if(Sen[iSafeDoor[i]].IsOff()==true)
                {
                    bOpenAllDoor=true;
                }
            }
        }

        if(Tri_Temp_Machine==1)                                                 //Ztex 2024.08.14 Add Open Door Check IC
        {
            for(int i=0; i<7; i++)
            {
                if(IsSafePLCIOInstall()==1)//所有PLC都要的機制
                    Cylinder[iC_DoorLock[i]].Off();
                else
                    Cylinder[iC_DoorLock[i]].On();
            }

            for(int i=0; i<MAX_HATCH_DOOR_CNT; i++)
            {
                if(Sen[iSafeDoorHatchway[i]].Enable==true)
                {
                    if(Sen[iSafeDoorHatchway[i]].IsOff()==true)
                        bOpenAllDoor=true;
                }
            }
            pnl_OpenDoor->Caption="Please Open Door Check";
            pnl_OpenDoor->Visible=true;
        }                                                                       //Ztex 2024.08.14 Add Open Door Check IC
    }
    else
    {
        bOpenAllDoor=true;
        if(Tri_Temp_Machine==1)                                                 //Ztex 2024.08.14 Add Open Door Check IC
        {
            for(int i=0; i<7; i++)
            {
                if(Sen[iS_DoorLock[i]].IsOn()==true)
                {
                    if(IsSafePLCIOInstall()==1)//所有PLC都要的機制
                        Cylinder[iC_DoorLock[i]].On();
                    else
                        Cylinder[iC_DoorLock[i]].Off();
                }
            }
            pnl_OpenDoor->Visible=false;
        }                                                                       //Ztex 2024.08.14 Add Open Door Check IC
    }

    if((iHWFix_BinBox==1 || bCancelErrorBin) &&
       bOpenSixDoor && (bBinError[0] || bBinError[1]))                          //kevin 20160819 add error bin box Bin error out shuttle 後需要開門確認
    {
        if(Sen[iSafeDoor[5]].Enable==true)
        {
            if(Sen[iSafeDoor[5]].IsOff()==true)
            {
                bOpenSixDoor=false;
                palCheckSht->Parent=tsRedAlarm;
            }
        }
        #ifdef SOFT_SIMULTE                                                     //kevin 20160912
            bOpenSixDoor=false;
            palCheckSht->Parent=tsRedAlarm;
        #endif
    }
    else if((bOutArmDropICSkip && bOpenSixDoor))                                //kevin 20171005 (wei) Out Arm Drop IC Skip 開門確認
    {
        if(Sen[iSafeDoor[5]].Enable==true)
        {
            if(Sen[iSafeDoor[5]].IsOff()==true ||
              (Tri_Temp_Machine==1 &&
               Sen[iSafeDoorHatchway[11]].Enable==true  &&
                Sen[iSafeDoorHatchway[11]].IsOff()==true))                      //Ztex 2024.12.17 Add Out Arm Drop IC Skip Show Message
            {
                bOpenSixDoor=false;
                palCheckSht->Parent=tsRedAlarm;
                Pwd="";
                bErrPan_err=false;
                bOutArmDropICSkip=false;                                        //kevin 20220125 clean need open drop alarm
                RecordProcess("open 6 safe door take outarm device!!");
                pnl_OpenDoor->Visible=false;                                    //Ztex 2024.12.17 Add Out Arm Drop IC Skip Show Message
            }
            else
            {
                bErrPan_err=true;
                Pwd="123";
                if(Tri_Temp_Machine==1)                                         //Ztex 2024.12.17 Add Out Arm Drop IC Skip Show Message
                {
                    pnl_OpenDoor->Caption="Please Open Door 6 Check";
                    pnl_OpenDoor->Visible=true;
                }
            }
        }
        #ifdef SOFT_SIMULTE                                                     //kevin 20160912
            bOpenSixDoor=false;
            palCheckSht->Parent=tsRedAlarm;
        #endif
    }
    else
    {
        bOpenSixDoor=false;
    }

    if(Sen[SnSafeDoor1].Enable==true)                                           //Steven 20130819 : 加入開門檢查的功能
    {
        if(Sen[SnSafeDoor1].IsOff()==true)
        {
            bOpenLeftDoor=true;
        }
    }
    else if(Sen[SnSafeDoor2].Enable==true)
    {
        if(Sen[SnSafeDoor2].IsOff()==true)
        {
            bOpenLeftDoor=true;
        }
    }
    else
    {
        bOpenLeftDoor=true;
    }

    if(IniConfig.bP59UnloaderICFloattingAlarmAfterExit)                         //Sam 20250415 : Unloader 偵測到置偏 IC 退出後再報警
    {
        if(iICFloattingCheckStep==3)
        {
            if(Sen[SenIndex[iICFloattingCheckPos]].IsOn()==false)
            {
                OPCheckUnloaderTime.SetSecAndOn(5.0);
                iICFloattingCheckStep=2;
                ShowUnloaderChkMsg(iICFloattingCheckStep);
            }
        }
        else if(iICFloattingCheckStep==2)
        {
            if(Sen[SenIndex[iICFloattingCheckPos]].IsOn())
                OPCheckUnloaderTime.SetSecAndOn(5.0);
            else if (OPCheckUnloaderTime.Off())
            {
                iICFloattingCheckStep=1;
                ShowUnloaderChkMsg(iICFloattingCheckStep);
            }
        }
        else if(iICFloattingCheckStep==1)
        {
            if(Sen[SenIndex[iICFloattingCheckPos]].IsOff()==false)
            {
                 iICFloattingCheckStep=0;
                 ShowUnloaderChkMsg(iICFloattingCheckStep);
            }
        }
    }

    fMain->Timer1Timer(fMain);                                                  //BtnSkipClick(BtnRetry); Steven 測試連續按Retry Start會不會出錯
    fMain->Timer7Timer(fMain);
    ProcessStatrDigital();

    if(Top>=YResolution-100 || Top<100-YResolution ||
       Left<400-XResolution || Left>XResolution-100)                            //Steven 20120206 : 畫面解析度
    {
        Left=0;
        Top=0;
    }

    for(i=0; i<9; i++)                                                          //ChungHung 20140730 add ContinuousFailHaveOneCycle 8--->9 //kevin 20130722 cancel K_FIX
    {
        if(Ptr[i]->Down==false)
            kct[i]=0;
        else
        {
            kct[i]++;
            if(kct[i]>10)
                Ptr[i]->Down=false;
        }
    }
    FlushLabel();
    if(iControlPanelMode)                                                       //Ken 20210812 : 解決Alarm無法按按鈕
        fPadInterface->Main232();
    ScanKey();
    CheckSafeDoorForICFallDown();
    DoSystemMessage();
    RecordSafeDoorStates();                                                     //Steven 20150128 : 加入安全門紀錄

    static bool oldFlushFlag;
    if(oldFlushFlag!=FlushFlag)
    {
        oldFlushFlag=FlushFlag;
        if(FlushPanel!=NULL)
        {
            if(FlushFlag)   FlushPanel->Color=clRed;
            else            FlushPanel->Color=cDark;
        }
    }

    if(bWriteErrRefFlag && FlushPanel!=NULL)    FlushPanel->Color=clRed;

    ScanTrayStatus();
    DoAvoidIndexMotorFallDown();
#ifdef SOFT_SIMULTE
    bOpenChamberDoor=true;
    bOpenLeftDoor=true;
    bOpenAllDoor=true;
    bIsTestSitICFallDown=false;
#else

    if(IniConfig.bIndexJamInArmAway==true &&                                    //jou 2011-10-14 start : 掉料需開啟Chamber門
       IniConfig.bD40IndexICFallDownMustPressFMotorDown==true)
    {
        if(bOpenChamberDoor==true && bIsTestSitICFallDown==false &&
           pnlIndex1Error->Parent==tsHandler)
        {
            if(CUSTOMER_CODE==CC_SCC)
            {
                if(OPCheckSocketTime.Off()==false)
                {
                    bIsTestSitICFallDown=true;
                    lblIndex1ErrorCH->Caption="安全門開啟時間小於10秒，請仔細確認Socket有沒有殘留IC";
                    lblIndex1ErrorCH->Visible=true;
                    return;
                }
            }

            pnlIndex1Error->Parent=tsRedAlarm;
        }
    }
    else
    {
        if(bIsTestSitICFallDown==false &&  pnlIndex1Error->Parent==tsHandler)
             pnlIndex1Error->Parent=tsRedAlarm;
    }

    if(TrayForm.iManualRemoveLoader==2 ||                                       //JerryYang 20250120 : add
       IniConfig.bE87PickupErrorAtLoaderNeedOpenDoor)                           //Steven 20130819 : 加入開門檢查的功能
    {
        palCheckLoader->Parent=(bOpenLeftDoor==true)?tsRedAlarm:tsHandler;
    }

    if(bAutoRetestJam==true)                                                    //wei 20160407 Alarm 後需要開門確認
    {
        palCheckLoader->Parent=(bOpenAllDoor==true)?tsRedAlarm:tsHandler;
    }

#endif

    static unsigned int iCT=0;
    iCT++;
    if(iCT>(unsigned int)(ChangeToFloatNonPcnt((double)(1000), (double)(Timer1->Interval))))  //jou 2011-08-01 start : 避免訊息被蓋在main form下面
    {
        iCT=0;
        if(TrayEditForm->fShow==true)                                           // kevin 20110824
        {
            TrayEditForm->BringToFront();
        }
        else if(fInput->fShow==true)                                            //Hung 20110827
        {
            fInput->BringToFront();
        }
        else if(FormBarcodeReader->bShow==true)                                 //Ifor 20170804 add 避免Offset 小鍵盤開啟時Alarm後視窗一直跳到fQwertyKey
        {
            FormBarcodeReader->BringToFront();
        }
        else if(bSECSGEM_NoteAlarm==true && fNote->fShow==true)                 //Ifor 20170616 (wei) add S10F3 後跳出Note Alarm 需顯示上層避免卡死
        {
            fNote->BringToFront();
        }
        else if(fQwertyKey2!=NULL && fQwertyKey2->bShow)                        //kevin 20120912 //jou 2012-10-24 修正密碼輸入會被password蓋住
        {
            fQwertyKey2->BringToFront();                                        //Steven 20150505 : 剛好在輸入時,Alarm會卡死
        }
        else if(fQwertyKey->bShow && fQwertyKey2->bShow==false)                 //Steven 20150505 : 剛好在輸入時,Alarm會卡死
        {
            fQwertyKey->BringToFront();
        }
        else if(fPassword2->bShow==true)                                        //Steven 20150505 : 剛好在輸入時,Alarm會卡死
        {
            fPassword2->BringToFront();
        }
        else if(fPassword->bShow && fPassword2->bShow==false)                   //Steven 20150505 : 剛好在輸入時,Alarm會卡死
        {
            fPassword->BringToFront();
        }
        else if(fCCLink->bShow)                                                 //Steven 20121015 : Alarm時要到前面
        {
            fCCLink->BringToFront();
        }
        else if(MyMessageBox->fShow==false)
        {
            fNote->BringToFront();
        }
    }

    if(IniConfig.bIndexDropErrCanMove==true)
    {
        IndexMoveToFrontRear();
    }

    if(InitialOK)                                                               //kevin 20130226
    {
        DoThermo();                                                             //jou 981209 any time need detect heater state
        fOmron->Main232();                                                      //Steven 20250424 : add protection for Omren EJ1N
        fOmron->Timer1Timer();
        HeaterDoorIsOpen();                                                     //jou 981013 : unify check heater doop sensor
        CheckHeater();                                                          //jou 981209 any time need detect heater state
    }

    if(IsSafePLCIOInstall() && bPLCFlag)                                        //KenHsieh 20250307 : fix PLC safedoor 通訊延遲問題
    {
        for(int i=0; i<MAX_SAFE_DOOR_CNT; i++)
        {
            if(iSafeDoor[i]>MAX_SENSOR_ITEM || iSafeDoor[i]<SnSafeDoor1)
            {
                ;
            }
            else if(Sen[iSafeDoor[i]].IsOff()==true)
            {
                asSafedoor.printf("%s is open.", Sen[iSafeDoor[i]].Name);
                RecordProcess(asSafedoor);
                ShowErrorUnit(iSafeDoorPosition[i]);
                bPLCFlag=false;
                break;
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfNote::Start()
{
    int KeyComp[]={K_SKIP, K_RETRY, K_TRAY_FEED, K_TRAY_END, K_CLEAN_OUT, K_RESET, K_HOME, K_TRAIN, K_ONECYCLE};        //kevin 20130722 cancel K_FIX //kevin 20130312
    int i, iTest;
    bool bSetFocus=false;
    if(bWaitSecsGemReply==true)                                                 //Ifor 20180227 (Steven) add SECS GEM Confirm the Employee ID
    {
        return;
    }

    if(pnlLotInfo->Parent==tsHandler)
    {
        if(fNote->TempCode=="MES16112" && atoi(edtLotCount->Text.c_str())==0)
        {
            return;
        }
        else if(fNote->TempCode=="MES16111" && atoi(edEQCQty->Text.c_str())==0)
        {
            return;
        }
    }

    BtnStart->Down=false;
    for(i=0; i<9; i++)                                                          //kevin 20130312
    {
        if(Select[i])
        {
            if(IniConfig.bF15OutShuttleLoseICNeedPWD)                           //ChungHung 20120912 Amkor 需求Shuttle lose ic need password Only Skip
            {
                if((TempCode=="JAM0508" || TempCode=="JAM0509") && Select[0]!=true)
                    bNeedPassWord=false;
            }

            if(fNote->TempCode=="MES0923" && CUSTOMER_CODE==CC_KYEC_LEE)        //Ifor 20171227 : addfNote->TempCode=="MES0923" && CUSTOMER_CODE==CC_KYEC_LEE)
            {
                bAutoRetestJam=true;
                bNeedKeyInSkipIC=true;
            }

            if(DoUnlockPassword()==false)                                       //Ifor 20170214 (wei) add 解除Alarm 需要獨立密碼
            {
                return;
            }

            if(DoPassword()==false)                                             //Steven 20101124
            {
                return;
            }

            if(IniConfig.bP53_ForcedScanBinCodeOfUnloader &&                    //JerryYang 20240111 : add P53 function
               LastSet.iTester==ON_LINE)
            {
                if((edErrorCode->Text=="MES1120" ||
                   edErrorCode->Text=="MES1220" ||
                   edErrorCode->Text=="MES1320" ||
                   edErrorCode->Text=="MES1720" ||
                   edErrorCode->Text=="MES1820" ||
                   edErrorCode->Text=="MES1920" ||
                   edErrorCode->Text=="MES1124" ||
                   edErrorCode->Text=="MES1224" ||
                   edErrorCode->Text=="MES1324" ||
                   edErrorCode->Text=="MES1724" ||
                   edErrorCode->Text=="MES1824" ||
                   edErrorCode->Text=="MES1924" ||
                   edErrorCode->Text=="MES1712" ||
                   edErrorCode->Text=="MES1812" ||
                   edErrorCode->Text=="MES1912") && bCheckBinOK==false)         //JerryYang 20250429 : fix Auto In/Out
                {
                    if(CheckBinCode()==true)
                    {
                        labCheckBin->Caption="Check bin pass!";
                        labCheckBin->Font->Color=clBlack;
                        bCheckBinOK=true;
                    }
                    else
                    {
                        for(int j=0; j<TEST_MAX_BIN; j++)
                        {
                            myBinCodeEdit[j]->Text="";
                            myBinCodeEdit[j]->Enabled=true;                     //JerryYang 20241118 : fix
                            if(myBinCodeEdit[j]->Enabled==true && myBinCodeEdit[j]->Visible==true && myBinCodeEdit[j]->Text=="" && bSetFocus==false)
                            {
                                bSetFocus=true;
                                myBinCodeEdit[j]->SetFocus();
                            }
                        }

                        labCheckBin->Caption="Check bin fail! please keyin the bin again.";
                        labCheckBin->Font->Color=clRed;
                        bCheckBinOK=false;
                        return;
                    }
                }
            }

            if(edErrorCode->Text=="MES1712" || edErrorCode->Text=="MES1812" || edErrorCode->Text=="MES1912")            //JerryYang 2025045 : fix 沒卡到Bundle ID不能是空白
            {
                if((fNote->edBundleID->Text.Length()!=12 || (fNote->edBundleID->Text.Length()>3 && fNote->edBundleID->Text[3]!='T')) ||                         //RogerYang 20250613 index 2->3
                  (bUnloading==false &&
                  ((iFixTrayCountCal[0]<fSCKART->iBundleOutCnt-2 &&edErrorCode->Text=="MES1712" && sFixBundleID[0]!="" && fNote->edBundleID->Text!=sFixBundleID[0]) ||
                  (iFixTrayCountCal[1]<fSCKART->iBundleOutCnt-2 &&edErrorCode->Text=="MES1812" && sFixBundleID[1]!="" && fNote->edBundleID->Text!=sFixBundleID[1]) ||
                  (iFixTrayCountCal[2]<fSCKART->iBundleOutCnt-2 &&edErrorCode->Text=="MES1912" && sFixBundleID[2]!="" && fNote->edBundleID->Text!=sFixBundleID[2]))))
                {
                    lblBundleIDErr->Visible=true;
                    return;
                }
                slDupUnloadBundlID->Clear();
                slDupUnloadBundlID->LoadFromFile(aslDupUnloadBundlID);
                if(slDupUnloadBundlID->Find(fNote->edBundleID->Text, iTest))
                {
                    lblBundleUnloadIDuplicted->Visible=true;
                    return;
                }
            }
            else if(TrayForm.bVTestNoRTBin &&                                   //RogerYang 20250626 偉測不可複測bin功能
                    (TempCode=="MES1713"   ||
                     TempCode=="MES1813"   ||
                     TempCode=="MES1913")  &&
                     bSetFocus==false)
            {
                bool bTmp=edBundleID->Text==""?false:edBundleID->Text==TrayForm.asNoRTBinFix[iNoRTBinIdx];
                fMesSystem->bNoRTBinFlag[iNoRTBinIdx]=bTmp;
                if(bTmp==false)                                                 //not Skip，且數值錯誤，不允許離開
                {
                    if(bSetFocus==false)
                    {
                        bSetFocus=true;
                        edBundleID->SetFocus();
                    }

                    if(Select[0]==false)
                    {
                        lblBunIDNotInList->Visible=true;
                        return;
                    }
                }
            }

            ReturnCode=KeyComp[i];

//            SoftStart=true;
//            SoftStop=false;
            fMain->Start("fNote::Start 1");                                     //JerryYang 20241118 : fix
            fMain->bStartKeyPressCheck=true;                                    //Steven 20110309 : for 安全門未關按Start時IndexArm會先動作

//            if(IniConfig.bEnable_SECS_GEM==true)                              //Steven 20140528 : Secs Gem
//            {
//               fMain->RunCheckStart();                                        //Ifor 20151208 : Run check 新增判斷
//            }

            SendCommand_ESD(ESD_SYSTEM_START);                                  //Steven 20140722
            Close();
            return;
        }
    }

    if(KeyCode==0)
    {
        if(IniConfig.bF15OutShuttleLoseICNeedPWD)                               //ChungHung 20120912 Amkor 需求Shuttle lose ic need password Only Skip
        {
            if((TempCode=="JAM0508" || TempCode=="JAM0509") && Select[0]!=true)
                bNeedPassWord=false;
        }

        if(DoPassword()==false)                                                 //Steven 20101124
        {
            return;
        }

        if(IniConfig.bP53_ForcedScanBinCodeOfUnloader &&                        //JerryYang 20240111 : add P53 function
           LastSet.iTester==ON_LINE)
        {
            if((edErrorCode->Text=="MES1120" ||
               edErrorCode->Text=="MES1220" ||
               edErrorCode->Text=="MES1320" ||
               edErrorCode->Text=="MES1720" ||
               edErrorCode->Text=="MES1820" ||
               edErrorCode->Text=="MES1920" ||
               edErrorCode->Text=="MES1124" ||
               edErrorCode->Text=="MES1224" ||
               edErrorCode->Text=="MES1324" ||
               edErrorCode->Text=="MES1724" ||
               edErrorCode->Text=="MES1824" ||
               edErrorCode->Text=="MES1924" ||
               edErrorCode->Text=="MES1712" ||
               edErrorCode->Text=="MES1812" ||
               edErrorCode->Text=="MES1912") && bCheckBinOK==false)             //JerryYang 20250429 : fix Auto In/Out
            {
                if(CheckBinCode()==true)
                {
                    labCheckBin->Caption="Check bin pass!";
                    labCheckBin->Font->Color=clBlack;
                    bCheckBinOK=true;
                }
                else
                {
                    for(int j=0; j<TEST_MAX_BIN; j++)
                    {
                        myBinCodeEdit[j]->Text="";
                        myBinCodeEdit[j]->Enabled=true;                         //JerryYang 20241118 : fix
                        if(myBinCodeEdit[j]->Enabled==true && myBinCodeEdit[j]->Visible==true && myBinCodeEdit[j]->Text=="" && bSetFocus==false)
                        {
                            bSetFocus=true;
                            myBinCodeEdit[j]->SetFocus();
                        }
                    }

                    labCheckBin->Caption="Check bin fail! please keyin the bin again.";
                    labCheckBin->Font->Color=clRed;
                    bCheckBinOK=false;
                    return;
                }
            }
        }

        if(edErrorCode->Text=="MES1712" || edErrorCode->Text=="MES1812" || edErrorCode->Text=="MES1912")
        {
            if((fNote->edBundleID->Text.Length()!=12 || (fNote->edBundleID->Text.Length()>3 && fNote->edBundleID->Text[3]!='T')) ||                             //RogerYang 20250613 index 2->3
              (bUnloading==false &&
              ((iFixTrayCountCal[0]<fSCKART->iBundleOutCnt-2 && edErrorCode->Text=="MES1712" && sFixBundleID[0]!="" && fNote->edBundleID->Text!=sFixBundleID[0]) ||
               (iFixTrayCountCal[1]<fSCKART->iBundleOutCnt-2 && edErrorCode->Text=="MES1812" && sFixBundleID[1]!="" && fNote->edBundleID->Text!=sFixBundleID[1]) ||
               (iFixTrayCountCal[2]<fSCKART->iBundleOutCnt-2 && edErrorCode->Text=="MES1912" && sFixBundleID[2]!="" && fNote->edBundleID->Text!=sFixBundleID[2]))))
            {
                lblBundleIDErr->Visible=true;
                return;
            }
            slDupUnloadBundlID->Clear();
            slDupUnloadBundlID->LoadFromFile(aslDupUnloadBundlID);
            if(slDupUnloadBundlID->Find(fNote->edBundleID->Text, iTest))
            {
                lblBundleUnloadIDuplicted->Visible=true;
                return;
            }
        }
        else if(TrayForm.bVTestNoRTBin &&                                       //RogerYang 20250626 偉測不可複測bin功能
                (TempCode=="MES1713"   ||
                 TempCode=="MES1813"   ||
                 TempCode=="MES1913")  &&
                 bSetFocus==false)
        {
            bool bTmp=edBundleID->Text==""?false:edBundleID->Text==TrayForm.asNoRTBinFix[iNoRTBinIdx];
            fMesSystem->bNoRTBinFlag[iNoRTBinIdx]=bTmp;
            if(bTmp==false)                                                     //not Skip，且數值錯誤，不允許離開
            {
                if(bSetFocus==false)
                {
                    bSetFocus=true;
                    edBundleID->SetFocus();
                }

                if(Select[0]==false)
                {
                    lblBunIDNotInList->Visible=true;
                    return;
                }
            }
        }

        if(CosFunction.bRMSNoNeedToDownloadEveryTime==false &&                  //Steven 20240926 : RMS不要每次下載包成功能
            IniConfig.bShowLotInfo && IniConfig.bEnableRms==true &&             //jou 2012-02-21 雖然沒有顯示Start button，但是按面板Start機台也是會跑
            LastSet.bHasDownloadFile==false)
        {
            ShowErrorMessage("MES1680", 0, MMSystem);                           //Please enter the device name and download set up file first!!
            return;
        }

        fMain->bStartKeyPressCheck=true;                                        //Steven 20110309 : for 安全門未關按Start時IndexArm會先動作
        fMain->Start("fNote::Start 2");                                         //JerryYang 20241118 : fix

//        if(IniConfig.bEnable_SECS_GEM==true)                                  //Steven 20140528 : Secs Gem
//        {
//            fMain->RunCheckStart();                                           //Ifor 20151208 : Run check 新增判斷
//        }

        SendCommand_ESD(ESD_SYSTEM_START);                                      //Steven 20140722
        Close();
    }
}
//------------------------------------------------------------------------------
void __fastcall TfNote::BtnStartClick(TObject *Sender)
{
    if(CosFunction.bEnableHandlerResultServer && bNeedTCPAlarm)                 //Sam 20230620 : 面板已經鎖定需要 IT 下命令解鎖
        return;
    #ifndef SOFT_SIMULTE
        if(CUSTOMER_CODE==CC_SIGURD_PeiXing)                                    //Alick 20160825 add for SIGURD PeiXing 要求NOTE畫面開啟START按鈕功能
        {
            Start();
            NewRecordProcess("MES2110", "START pressed", "Note_BtnStartClick");
        }
        else
        {
            if(bErrPan_err==true && Pwd!="")
                return;                                                         //Richard 2011/2/22 SpecialPanel
        }
    #else
        Start();
    #endif
}
//---------------------------------------------------------------------------
void __fastcall TfNote::BtnPauseClick(TObject *Sender)
{
    if(CosFunction.bEnableHandlerResultServer && bNeedTCPAlarm)                 //Sam 20230620 : 面板已經鎖定需要 IT 下命令解鎖
        return;
    if(bErrPan_err==true && Pwd!="")
        return;                                                                 //Richard 2011/2/22 SpecialPanel

     if(CUSTOMER_CODE==CC_ASE_SG && bTesterSendPause==true)
        return;

    int KeyComp[]={K_SKIP, K_RETRY, K_TRAY_FEED, K_TRAY_END, K_CLEAN_OUT, K_RESET, K_HOME, K_TRAIN,K_ONECYCLE};         //ChungHung 20140730 add ContinuousFailHaveOneCycle //kevin 20130312  ChungHung HT9045 2011/12/13 //Input pickup device error時,按"retry"鍵,機台都會自動home add K_HOME
    bool bSetFocus=false;
    int iTest;
    BtnPause->Down=false;
    bStartMoveSpeed=false;                                                      //Steven 20231018 : Fixed for G14

    if(pnlLotInfo->Parent==tsHandler)
    {
        if(fNote->TempCode=="MES16112" && atoi(edtLotCount->Text.c_str())==0)
        {
            return;
        }
        else if(fNote->TempCode=="MES16111" && atoi(edEQCQty->Text.c_str())==0)
        {
            return;
        }
    }

    if(fNote->TempCode=="MES1712" || fNote->TempCode=="MES1812" || fNote->TempCode=="MES1912")
    {
        if((fNote->edBundleID->Text.Length()!=12 || (fNote->edBundleID->Text.Length()>3 && fNote->edBundleID->Text[3]!='T')))                                   //RogerYang 20250613 index 2->3
        {
            return;
        }
        else
        {
        }
    }

    for(int i=0; i<9; i++)                                                      //kevin 20130312
    {
        if(Select[i])
        {
            if(IniConfig.bF15OutShuttleLoseICNeedPWD)                           //ChungHung 20120912 Amkor 需求Shuttle lose ic need password Only Skip
            {
                if((TempCode=="JAM0508" || TempCode=="JAM0509") && Select[0]!=true)
                    bNeedPassWord=false;
            }

            if(fNote->TempCode=="MES0923" && CUSTOMER_CODE==CC_KYEC_LEE)        //Ifor 20171227 : addfNote->TempCode=="MES0923" && CUSTOMER_CODE==CC_KYEC_LEE)
            {
                bAutoRetestJam=true;
                bNeedKeyInSkipIC=true;
            }

            if(DoUnlockPassword()==false)                                       //Ifor 20170214 (wei) add 解除Alarm 需要獨立密碼
            {
                return;
            }

            if(DoPassword()==false)                                             //Steven 20101124
            {
                return;
            }

            if(IniConfig.bP53_ForcedScanBinCodeOfUnloader &&                    //JerryYang 20240111 : add P53 function
               LastSet.iTester==ON_LINE)
            {
                if((edErrorCode->Text=="MES1120" ||
                   edErrorCode->Text=="MES1220" ||
                   edErrorCode->Text=="MES1320" ||
                   edErrorCode->Text=="MES1720" ||
                   edErrorCode->Text=="MES1820" ||
                   edErrorCode->Text=="MES1920" ||
                   edErrorCode->Text=="MES1124" ||
                   edErrorCode->Text=="MES1224" ||
                   edErrorCode->Text=="MES1324" ||
                   edErrorCode->Text=="MES1724" ||
                   edErrorCode->Text=="MES1824" ||
                   edErrorCode->Text=="MES1924" ||
                   edErrorCode->Text=="MES1712" ||
                   edErrorCode->Text=="MES1812" ||
                   edErrorCode->Text=="MES1912") && bCheckBinOK==false)         //JerryYang 20250429 : fix Auto In/Out
                {
                    if(CheckBinCode()==true)
                    {
                        labCheckBin->Caption="Check bin pass!";
                        labCheckBin->Font->Color=clBlack;
                        bCheckBinOK=true;
                    }
                    else
                    {
                        for(int j=0; j<TEST_MAX_BIN; j++)
                        {
                            myBinCodeEdit[j]->Text="";
                            myBinCodeEdit[j]->Enabled=true;                     //JerryYang 20241118 : fix
                            if(myBinCodeEdit[j]->Enabled==true && myBinCodeEdit[j]->Visible==true && myBinCodeEdit[j]->Text=="" && bSetFocus==false)
                            {
                                bSetFocus=true;
                                myBinCodeEdit[j]->SetFocus();
                            }
                        }

                        labCheckBin->Caption="Check bin fail! please keyin the bin again.";
                        labCheckBin->Font->Color=clRed;
                        bCheckBinOK=false;
                        return;
                    }
                }
            }

            if(edErrorCode->Text=="MES1712" || edErrorCode->Text=="MES1812" || edErrorCode->Text=="MES1912")
            {
                if((fNote->edBundleID->Text.Length()!=12 || (fNote->edBundleID->Text.Length()>3 && fNote->edBundleID->Text[3]!='T')) ||                         //RogerYang 20250613 index 2->3
                  (bUnloading==false &&
                  ((iFixTrayCountCal[0]<fSCKART->iBundleOutCnt-2 &&edErrorCode->Text=="MES1712" && sFixBundleID[0]!="" && fNote->edBundleID->Text!=sFixBundleID[0]) ||
                  (iFixTrayCountCal[1]<fSCKART->iBundleOutCnt-2 &&edErrorCode->Text=="MES1812" && sFixBundleID[1]!="" && fNote->edBundleID->Text!=sFixBundleID[1]) ||
                  (iFixTrayCountCal[2]<fSCKART->iBundleOutCnt-2 &&edErrorCode->Text=="MES1912" && sFixBundleID[2]!="" && fNote->edBundleID->Text!=sFixBundleID[2]))))
                {
                    lblBundleIDErr->Visible=true;
                    return;
                }
                slDupUnloadBundlID->Clear();
                slDupUnloadBundlID->LoadFromFile(aslDupUnloadBundlID);
                if(slDupUnloadBundlID->Find(fNote->edBundleID->Text, iTest))
                {
                    lblBundleUnloadIDuplicted->Visible=true;
                    return;
                }
            }
            else if(TrayForm.bVTestNoRTBin &&                                   //RogerYang 20250626 偉測不可複測bin功能
                    (TempCode=="MES1713"   ||
                     TempCode=="MES1813"   ||
                     TempCode=="MES1913")  &&
                     bSetFocus==false)
            {
                bool bTmp=edBundleID->Text==""?false:edBundleID->Text==TrayForm.asNoRTBinFix[iNoRTBinIdx];
                fMesSystem->bNoRTBinFlag[iNoRTBinIdx]=bTmp;
                if(bTmp==false)                                                 //not Skip，且數值錯誤，不允許離開
                {
                    if(bSetFocus==false)
                    {
                        bSetFocus=true;
                        edBundleID->SetFocus();
                    }

                    if(Select[0]==false)
                    {
                        lblBunIDNotInList->Visible=true;
                        return;
                    }
                }
            }

            ReturnCode=KeyComp[i];
            SoftStop=true;
            SoftStart=false;
            #ifdef SOFT_SIMULTE
                if(     i==0)
                {
                    NewRecordProcess("MES2120", "SKIP pressed", "Note_BtnPauseClick");
                    if(IniConfig.bEnable_SECS_GEM==true)                        //Steven 20140528 : Secs Gem
                        EventReport(SECS_EVENT.DoSkip);                         //29     按下 Skip
                }
                else if(i==1)
                {
                    NewRecordProcess("MES2121", "RETRY pressed", "Note_BtnPauseClick");
                    if(IniConfig.bEnable_SECS_GEM==true)                        //Steven 20140528 : Secs Gem
                        EventReport(SECS_EVENT.DoRetry);                        //28     按下 Retry
                }
                else if(i==2)
                {
                    NewRecordProcess("MES2118", "TRAY FEED pressed", "Note_BtnPauseClick");
                    if(IniConfig.bEnable_SECS_GEM==true)                        //Steven 20140528 : Secs Gem
                        EventReport(SECS_EVENT.DoTrayFeed);                     //32     按下 Tray Feed
                }
                else if(i==3)
                {
                    NewRecordProcess("MES2119", "TRAY END pressed", "Note_BtnPauseClick");
                    if(IniConfig.bEnable_SECS_GEM==true)                        //Steven 20140528 : Secs Gem
                        EventReport(SECS_EVENT.DoTrayEnd);                      //31     按下 Tray End
                }
                else if(i==4)
                {
                    NewRecordProcess("MES2114", "CLEAN OUT pressed", "Note_BtnPauseClick");
                    if(IniConfig.bEnable_SECS_GEM==true)                        //Steven 20140528 : Secs Gem
                        EventReport(SECS_EVENT.DoCleanOut);                     // 4     按下 Clean Out
                }
                else if(i==5)
                {
                    NewRecordProcess("MES2113", "RESET pressed", "Note_BtnPauseClick");
                    if(IniConfig.bEnable_SECS_GEM==true)                        //Steven 20140528 : Secs Gem
                        EventReport(SECS_EVENT.DoReset);                        //33     按下 Reset
                }
                else if(i==6)
                {
                    NewRecordProcess("MES2122", "HOME & Retry pressed", "Note_BtnPauseClick");                          //ChungHung HT9045 2011/12/13 //Input pickup device error時,按"retry"鍵,機台都會自動home
                    if(IniConfig.bEnable_SECS_GEM==true)                        //Steven 20140528 : Secs Gem
                        EventReport(SECS_EVENT.DoHome);                         //25     按下 Home
                }
                else if(i==7)
                {
                    NewRecordProcess("MES2123", "Auto Training", "Note_BtnPauseClick");
                }
                else if(i==8)
                {
                    NewRecordProcess("MES2124", "Fix pressed", "Note_BtnPauseClick");
                }
            #endif

            if(IniConfig.bEnable_SECS_GEM==true)                                //Steven 20140528 : Secs Gem
                EventReport(SECS_EVENT.DoPause);                                // 2     按下 Pause
            SendCommand_ESD(ESD_SYSTEM_STOP);                                   //Steven 20140722
            Close();
            return;
        }
    }

    if(KeyCode==0)
    {
        if(IniConfig.bF15OutShuttleLoseICNeedPWD)                               //ChungHung 20120912 Amkor 需求Shuttle lose ic need password Only Skip
        {
            if((TempCode=="JAM0508" || TempCode=="JAM0509") && Select[0]!=true)
                bNeedPassWord=false;
        }

        if(DoPassword()==false)                                                 //Steven 20101124
        {
            return;
        }

        if(IniConfig.bP53_ForcedScanBinCodeOfUnloader &&                        //JerryYang 20240111 : add P53 function
           LastSet.iTester==ON_LINE)
        {
            if((edErrorCode->Text=="MES1120" ||
               edErrorCode->Text=="MES1220" ||
               edErrorCode->Text=="MES1320" ||
               edErrorCode->Text=="MES1720" ||
               edErrorCode->Text=="MES1820" ||
               edErrorCode->Text=="MES1920" ||
               edErrorCode->Text=="MES1124" ||
               edErrorCode->Text=="MES1224" ||
               edErrorCode->Text=="MES1324" ||
               edErrorCode->Text=="MES1724" ||
               edErrorCode->Text=="MES1824" ||
               edErrorCode->Text=="MES1924" ||
               edErrorCode->Text=="MES1712" ||
               edErrorCode->Text=="MES1812" ||
               edErrorCode->Text=="MES1912") && bCheckBinOK==false)             //JerryYang 20250429 : fix Auto In/Out
            {
                if(CheckBinCode()==true)
                {
                    labCheckBin->Caption="Check bin pass!";
                    labCheckBin->Font->Color=clBlack;
                    bCheckBinOK=true;
                }
                else
                {
                    for(int j=0; j<TEST_MAX_BIN; j++)
                    {
                        myBinCodeEdit[j]->Text="";
                        myBinCodeEdit[j]->Enabled="";                           //JerryYang 20241118 : fix
                        if(myBinCodeEdit[j]->Enabled==true && myBinCodeEdit[j]->Visible==true && myBinCodeEdit[j]->Text=="" && bSetFocus==false)
                        {
                            bSetFocus=true;
                            myBinCodeEdit[j]->SetFocus();
                        }
                    }

                    labCheckBin->Caption="Check bin fail! please keyin the bin again.";
                    labCheckBin->Font->Color=clRed;
                    bCheckBinOK=false;
                    return;
                }
            }
        }

        if(edErrorCode->Text=="MES1712" || edErrorCode->Text=="MES1812" || edErrorCode->Text=="MES1912")
        {
            if((fNote->edBundleID->Text.Length()!=12 || (fNote->edBundleID->Text.Length()>3 && fNote->edBundleID->Text[3]!='T')) ||                             //RogerYang 20250613 index 2->3
              (bUnloading==false &&
              ((iFixTrayCountCal[0]<fSCKART->iBundleOutCnt-2 &&edErrorCode->Text=="MES1712" && sFixBundleID[0]!="" && fNote->edBundleID->Text!=sFixBundleID[0]) ||
              (iFixTrayCountCal[1]<fSCKART->iBundleOutCnt-2 &&edErrorCode->Text=="MES1812" && sFixBundleID[1]!="" && fNote->edBundleID->Text!=sFixBundleID[1]) ||
              (iFixTrayCountCal[2]<fSCKART->iBundleOutCnt-2 &&edErrorCode->Text=="MES1912" && sFixBundleID[2]!="" && fNote->edBundleID->Text!=sFixBundleID[2]))))
            {
                lblBundleIDErr->Visible=true;
                return;
            }
            slDupUnloadBundlID->Clear();
            slDupUnloadBundlID->LoadFromFile(aslDupUnloadBundlID);
            if(slDupUnloadBundlID->Find(fNote->edBundleID->Text, iTest))
            {
                lblBundleUnloadIDuplicted->Visible=true;
                return;
            }
        }
        else if(TrayForm.bVTestNoRTBin &&                                       //RogerYang 20250626 偉測不可複測bin功能
                (TempCode=="MES1713"   ||
                 TempCode=="MES1813"   ||
                 TempCode=="MES1913")  &&
                 bSetFocus==false)
        {
            bool bTmp=edBundleID->Text==""?false:edBundleID->Text==TrayForm.asNoRTBinFix[iNoRTBinIdx];
            fMesSystem->bNoRTBinFlag[iNoRTBinIdx]=bTmp;
            if(bTmp==false)                                                     //not Skip，且數值錯誤，不允許離開
            {
                if(bSetFocus==false)
                {
                    bSetFocus=true;
                    edBundleID->SetFocus();
                }

                if(Select[0]==false)
                {
                    lblBunIDNotInList->Visible=true;
                    return;
                }
            }
        }

        SoftStop=true;
        ReturnCode=0;
        SoftStart=false;
        if(IniConfig.bEnable_SECS_GEM==true)                                    //Steven 20140528 : Secs Gem
            EventReport(SECS_EVENT.DoPause);                                    // 2     按下 Pause
        SendCommand_ESD(ESD_SYSTEM_STOP);                                       //Steven 20140722
        fNote->Close();
    }
}
//---------------------------------------------------------------------------
AnsiString CylinderIndexToJamCode(int Code, int *Pos)                           //Steven 20231127 : 氣缸Alarm改成自動生成, 分類31
{
    int CylinderIndex=Code-31000;
    AnsiString S;

    for(int i=0; i<6; i++)
        fNote->Select[i]=false;

    if(CylinderIndex==C_TrayX_UpDown        ||
       CylinderIndex==C_CatchTray_FixOn     ||                                  //ChungHung 20140624 add AutoRetest catch Tray use 2 Output
       CylinderIndex==C_TurnTrayArm         ||                                  //ChungHung 20140701 add AutoRetest Turn Tray Arm
       CylinderIndex==C_CatchTray_FixOff    ||
       CylinderIndex==C_CatchTray_Fix)
    {
        *Pos=MTrayX;
    }
    else if(CylinderIndex==C_TrayY_Fixer    ||
            CylinderIndex==C_LoaderEdgePush)
    {
        *Pos=MMTrayY;
    }
    else if(CylinderIndex==C_Load_Up        ||
            CylinderIndex==C_Load_Middle    ||
            CylinderIndex==C_TrayZ_Selector)
    {
        *Pos=MMTrayY_Car;
    }
    else if(CylinderIndex==C_Empty_Fix)
    {
        *Pos=MMEmpty;
    }
    else if(CylinderIndex==C_Empty_Up           ||
            CylinderIndex==C_Empty_Middle       ||
            CylinderIndex==C_EmptyLoaderZ_Select)
    {
        *Pos=MMEmpty_Car;
    }
    else if(CylinderIndex==C_Auto1EdgePush  ||
            CylinderIndex==C_Auto1Side_Fixer)
    {
        *Pos=MMAuto1;
    }
    else if(CylinderIndex==C_Auto1_Selector     ||
            CylinderIndex==C_Auto1_Up           ||                              //Steven 20140409 : Auto Retest
            CylinderIndex==C_Auto1LoaderZ_Select)                               //Steven 20140409 : Auto Retest
    {
        *Pos=MMAuto1_Car;
    }
    else if(CylinderIndex==C_Auto2Side_Fixer    ||
            CylinderIndex==C_Auto2EdgePush      )
    {
        *Pos=MMAuto2;
    }
    else if(CylinderIndex==C_Auto2_Selector     ||
            CylinderIndex==C_Auto2_Up           ||                              //Steven 20140409 : Auto Retest
            CylinderIndex==C_Auto2LoaderZ_Select)                               //Steven 20140409 : Auto Retest
    {
        *Pos=MMAuto2_Car;
    }
    else if(CylinderIndex==C_Auto3Side_Fixer    ||
            CylinderIndex==C_Auto3EdgePush      )
    {
        *Pos=MMAuto3;
    }
    else if(CylinderIndex==C_Auto3_Selector     ||
            CylinderIndex==C_Auto3_Up           ||                              //Steven 20140409 : Auto Retest
            CylinderIndex==C_Auto3LoaderZ_Select)                               //Steven 20140409 : Auto Retest
    {
        *Pos=MMAuto3_Car;
    }
    else if(CylinderIndex==C_Auto4EdgePush  ||
            CylinderIndex==C_Auto4Side_Fixer)
    {
        *Pos=MMAuto4;
    }
    else if(CylinderIndex==C_Auto4_Selector     ||
            CylinderIndex==C_Auto4_Up           ||
            CylinderIndex==C_Auto4LoaderZ_Select)
    {
        *Pos=MMAuto4_Car;
    }
    else if(CylinderIndex==C_Auto5EdgePush  ||
            CylinderIndex==C_Auto5Side_Fixer)
    {
        *Pos=MMAuto5;
    }
    else if(CylinderIndex==C_Auto5_Selector     ||
            CylinderIndex==C_Auto5_Up           ||
            CylinderIndex==C_Auto5LoaderZ_Select)
    {
        *Pos=MMAuto5_Car;
    }
    else if(CylinderIndex==C_Auto6EdgePush  ||
            CylinderIndex==C_Auto6Side_Fixer)
    {
        *Pos=MMAuto6;
    }
    else if(CylinderIndex==C_Auto6_Selector     ||
            CylinderIndex==C_Auto6_Up           ||
            CylinderIndex==C_Auto6LoaderZ_Select)
    {
        *Pos=MMAuto6_Car;
    }
    else if(CylinderIndex==C_Color_Fix)
    {
        *Pos=MMColor;
    }
    else if(CylinderIndex==C_Color_Up           ||
            CylinderIndex==C_Color_Middle       ||
            CylinderIndex==C_ColorLoaderZ_Select)
    {
        *Pos=MMColor_Car;
    }
    else if(CylinderIndex==C_TesterSidePush)                                    //Richard 20220321 : 渠梁Side Push
    {
        *Pos=MMSystem;
    }
    else if(CylinderIndex==C_FixTray_FullPlace)
    {
        *Pos=MManualTray3;
    }
    else                                                                        //ChungHung 20140731 add fix Cylinder make UnknowMessage
    {
        *Pos=MMSystem;
    }

    S.sprintf("JAM%d", Code);
    return S;
}
//------------------------------------------------------------------------------
bool NowNoteIsShow()                                                            //ChungHung 20130110 add 防止訊息重複進入
{
    return fNote->fShow;
}
//------------------------------------------------------------------------------
AnsiString MotorIndexToJamCode(int MotNo)
{
    AnsiString S;
    S.sprintf("WAR24%03d", MotNo);
    return S;
}
//---------------------------------------------------------------------------
void __fastcall TfNote::TrayEditClick(TObject *Sender)
{
    if(CosFunction.bUseEditLDTrayNeedManualRemoveTray==true)                    //Sam 20230204 : 修正編輯 Loader 無法手動移除問題
    {
        iLDTrayNeedManualRemoveTray=1;                                          //0:Normal 1:Edit Loader Tray 2:Data Change
    }
    EditTray(MMTrayY);
}
//---------------------------------------------------------------------------
void __fastcall TfNote::ErrShowToForm(AnsiString Code, AnsiString UName, AnsiString Mes, int iMotorErr)
{
#ifdef DEBUG_NEW_ALARM_DESCRIPTION
    bool bAddToMemo;
    TStringList *strList = new TStringList;
    AnsiString Language="", LanguageEng=Code+"_English", MoviePath;
    strList->LoadFromFile("D:\\HT9045\\Error\\AlarmDescription.ini");

//    _di_IXMLDocument

    MoviePath="D:\\Movie\\"+Code+".avi";                                        //Steven 20100914

    PlayMovie(MoviePath);
    reDescription->Clear();
    edErrorCode->Text="";
    Edit3->Text="";
    edUnitName->Text="";

    edErrorCode->Text=Code;
    Edit3->Text=Code.SubString(4, 2);
    edUnitName->Text=UName;
    ShowMessageEdit1->Text=Mes;                                                 //kevin 20170905 (Steven) add

    if(IniConfig.iUserLanguage==eulKorea)                                       //Steven 20120203 : 分開語言包
    {
        Language=Code+"_Korea";
    }
    else if(IniConfig.iUserLanguage==eulJapan)
    {
        Language=Code+"_Japan";
    }
    else if(IniConfig.iUserLanguage==eulSingapore)                              //Steven 20120607 : 新加坡版
    {
        Language=Code+"_Singapore";
    }
    else if(IniConfig.iUserLanguage==eulEnglish)                                //Steven 20120202 : 純英文版
    {
        Language="";
    }
    else
    {
        Language=Code+"_Chinese";
    }

    bAddToMemo=false;
    for(int i=0; i<strList->Count; i++)
    {
        if(Language!="")
        {
            if(strList->Strings[i].Pos(Language)!=0)
            {
                bAddToMemo=true;
                continue;
            }
        }

        if(strList->Strings[i].Pos(LanguageEng)!=0)
        {
            bAddToMemo=true;
            continue;
        }

        if(bAddToMemo)
        {
            if(strList->Strings[i].Pos("[")!=1)
            {
                reDescription->Lines->Add(strList->Strings[i]);
            }
            else
            {
                bAddToMemo=false;
            }
        }
    }

    if(IniConfig.iUserLanguage==eulKorea)                                       //Steven 20120203 : 分開語言包
    {
        reDescription->Font->Name="Batang";
        reDescription->Font->Charset=HANGEUL_CHARSET;
    }
    else if(IniConfig.iUserLanguage==eulJapan)
    {
        reDescription->Font->Name="Tahoma";
        reDescription->Font->Charset=SHIFTJIS_CHARSET;
    }
    else
    {
        reDescription->Font->Name="Arial";
        reDescription->Font->Charset=DEFAULT_CHARSET;
    }
    strList->Clear();                                                           //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete strList;
#else
    AnsiString Language, TextPath="", MoviePath="";

    if(CUSTOMER_CODE==CC_KYEC_LEE)                                              //Ifor 20160907 KYEC 要求顯示中英文 Alarm
    {
         Language="Chinese";
    }
    else if(IniConfig.iUserLanguage==eulKorea)                                  //Steven 20120203 : 分開語言包
    {
        Language="Korea";
    }
    else if(IniConfig.iUserLanguage==eulEnglish)                                //Steven 20120202 : 純英文版
    {
        Language="English";
    }
    else if(IniConfig.iUserLanguage==eulSingapore)                              //Steven 20120607 : 新加坡版
    {
        Language="Singapore";
    }
    else
    {
        if(LastSet.iLanguageCountry == 0)                                       //kevin 20170923 (wei) add 中英切換
            Language="English";
        else
            Language="Chinese";
    }

    if(iMotorErr==-1)
        TextPath="D:\\HT9045\\Error\\"+Language+"\\"+Code+".dat";
    else
        TextPath="D:\\HT9045\\Error\\"+Language+"\\MOT"+iMotorErr+".dat";

    MoviePath="D:\\Movie\\"+Code+".avi";                                        //Steven 20100914

    PlayMovie(MoviePath);
    reDescription->Clear();
    fNote->reBigDescription->Clear();                                           //Ifor 20200331 : add
    edErrorCode->Text="";
    Edit3->Text="";
    edUnitName->Text="";

    edErrorCode->Text=Code;
    Edit3->Text=Code.SubString(4,2);
    edUnitName->Text=UName;
    sAlarmMes=Mes;                                                              //wei 20160407 Alarm Message
    ShowMessageEdit1->Text=Mes;                                                 //kevin 20170905 (Steven) add

    if(FileExists(TextPath))
    {
        reDescription->Lines->LoadFromFile(TextPath);                           //Steven 20100127 : 新的Alarm訊息專用
        fNote->reBigDescription->Lines->LoadFromFile(TextPath);                 //Ifor 20200331 : add

        if(IniConfig.iUserLanguage==eulKorea)                                   //Steven 20120203 : 分開語言包
        {
            reDescription->Font->Name="Batang";
            reDescription->Font->Charset=DEFAULT_CHARSET;
            fNote->reBigDescription->Font->Name="Batang";                       //Ifor 20200331 : add
            fNote->reBigDescription->Font->Charset=DEFAULT_CHARSET;             //Ifor 20200331 : add
//            reDescription->Font->Charset=HANGEUL_CHARSET;
        }
        else if(IniConfig.iUserLanguage==eulJapan)
        {
            reDescription->Font->Name="Tahoma";
            reDescription->Font->Charset=SHIFTJIS_CHARSET;
            fNote->reBigDescription->Font->Name="Tahoma";                       //Ifor 20200331 : add
            fNote->reBigDescription->Font->Charset=SHIFTJIS_CHARSET;            //Ifor 20200331 : add
        }
        else if(IniConfig.iUserLanguage==eulEnglish)                            //Steven 20120202 : 純英文版
        {
            reDescription->Font->Name="Arial";
            reDescription->Font->Charset=DEFAULT_CHARSET;
            fNote->reBigDescription->Font->Name="Arial";                        //Ifor 20200331 : add
            fNote->reBigDescription->Font->Charset=DEFAULT_CHARSET;             //Ifor 20200331 : add
        }
        else
        {
            reDescription->Font->Charset=DEFAULT_CHARSET;
            fNote->reBigDescription->Font->Charset=DEFAULT_CHARSET;             //Ifor 20200331 : add
        }
    }
    else
    {
        Language="English";
        if(iMotorErr==-1)
            TextPath="D:\\HT9045\\Error\\"+Language+"\\"+Code+".dat";
        else
            TextPath="D:\\HT9045\\Error\\"+Language+"\\MOT"+iMotorErr+".dat";

        if(FileExists(TextPath))
        {
            reDescription->Lines->LoadFromFile(TextPath);                       //Steven 20100127 : 新的Alarm訊息專用
            fNote->reBigDescription->Lines->LoadFromFile(TextPath);             //Ifor 20200331 : add
        }

        reDescription->Font->Name="Arial";
        fNote->reBigDescription->Font->Name="Arial";                            //Ifor 20200331 : add
    }

    if(CUSTOMER_CODE==CC_Greatek && bUseFTPOneCycle && Code=="MES1640")         //Sam 20200305 : 增加 FTP 檔案檢查，檢查到 VIE_STOP.txt 就執行 OneCycle
    {
        reDescription->Clear();
        bUseFTPOneCycle=false;
        reDescription->Lines->Add("Please confirm Tester VIE information");
        reDescription->Font->Color=clRed;
    }
    else
    {
        reDescription->Font->Color=clBlack;
    }
#endif
}
//---------------------------------------------------------------------------
void ShowErrorUnit(int Pos)                                                     //Steven 20090920 : For Flush Alarm Panel
{
    fNote->palCCD->Visible=false;                                               //Steven 20120315
    iPosition=Pos;
    fNote->palShtSensorSOP->Top=200;
    if(     Pos<=MInArmZH)                      FlushPanel=fNote->palInArm;
    else if(Pos==MInShuttle1)
    {
        FlushPanel=fNote->palInSh1;
    }
    else if(Pos==MInShuttle2)
    {
        FlushPanel=fNote->palInSh2;
    }
    else if(Pos==MTestY1 || Pos==MTestZ1)       FlushPanel=fNote->palHead0;
    else if(Pos==MTestY2 || Pos==MTestZ2)       FlushPanel=fNote->palHead1;
    else if(Pos==MOutShuttle1)
    {
        FlushPanel=fNote->palOutSh1;
        if(SHUTTLE_SENSOR_TYPE==eSensorLine)                                    //Line Sensor, show SOP button
            fNote->palShtSensorSOP->Visible=true;                               //Steven 20100319
    }
    else if(Pos==MOutShuttle2)
    {
        FlushPanel=fNote->palOutSh2;
        if(SHUTTLE_SENSOR_TYPE==eSensorLine)                                    //Line Sensor, show SOP button
            fNote->palShtSensorSOP->Visible=true;                               //Steven 20100319
    }
    else if(Pos>=MOutArmX && Pos<=MOutArmZH)    FlushPanel=fNote->palOutArm;
    else if(Pos==MTrayX)                        FlushPanel=fNote->palTrayArm;
    else if(Pos==MMTrayZ  || Pos==MMTrayY_Car)  FlushPanel=fNote->palLoad_Car;
    else if(Pos==MMEmptyZ || Pos==MMEmpty_Car)  FlushPanel=fNote->palEmpty_Car;
    else if(Pos==MMEmpty1_Car)                  FlushPanel=fNote->palColor_Car;
    else if(Pos==MMColorZ || Pos==MMColor_Car)  FlushPanel=fNote->palColor_Car;
    else if(Pos==MMAuto1Z || Pos==MMAuto1_Car)  FlushPanel=fNote->palAuto1_Car;
    else if(Pos==MMAuto2Z || Pos==MMAuto2_Car)  FlushPanel=fNote->palAuto2_Car;                                         //Steven 20140331 : Add MMAuto2Z
    else if(Pos==MMAuto3Z || Pos==MMAuto3_Car)  FlushPanel=fNote->palAuto3_Car;                                         //Steven 20140331 : Add MMAuto3Z
    else if(Pos==MMAuto4Z || Pos==MMAuto4_Car)  FlushPanel=fNote->palAuto4_Car;
    else if(Pos==MMAuto5Z || Pos==MMAuto5_Car)  FlushPanel=fNote->palAuto5_Car;
    else if(Pos==MMAuto6Z || Pos==MMAuto6_Car)  FlushPanel=fNote->palAuto6_Car;
    else if(Pos==MManualTray1)                  FlushPanel=fNote->palFix1;
    else if(Pos==MManualTray2)                  FlushPanel=fNote->palFix2;
    else if(Pos==MManualTray3)                  FlushPanel=fNote->palFix3;
    else if(Pos==MManualTray4)                  FlushPanel=fNote->palFix4;
    else if(Pos==MManualTray5)                  FlushPanel=fNote->palFix5;
    else if(Pos==MManualTray6)                  FlushPanel=fNote->palFix6;
    else if(Pos==MManualTrayAll)                FlushPanel=fNote->palManualAll;                                         //Steven 20120131 : Fix Tray置偏偵測
    else if(Pos==MMTrayY)                       FlushPanel=fNote->palLoad;
    else if(Pos==MMPlate1)                      FlushPanel=fNote->palPlate1;
    else if(Pos==MMPlate2)                      FlushPanel=fNote->palPlate2;
    else if(Pos==MMAuto1)                       FlushPanel=fNote->palAuto1;
    else if(Pos==MMAuto2)                       FlushPanel=fNote->palAuto2;
    else if(Pos==MMAuto3)                       FlushPanel=fNote->palAuto3;
    else if(Pos==MMAuto4)                       FlushPanel=fNote->palAuto4;
    else if(Pos==MMAuto5)                       FlushPanel=fNote->palAuto5;
    else if(Pos==MMAuto6)                       FlushPanel=fNote->palAuto6;
    else if(Pos==MMEmpty)                       FlushPanel=fNote->palEmpty;
    else if(Pos==MMColor)                       FlushPanel=fNote->palColor;
    else if(Pos==MMEmpty1)                      FlushPanel=fNote->palColor;
    else if(Pos==MMSystem)                      FlushPanel=fNote->palSys;
    else if(Pos==MMSafeDoor1)                   FlushPanel=fNote->palSafeDoor1;
    else if(Pos==MMSafeDoor2)                   FlushPanel=fNote->palSafeDoor2;
    else if(Pos==MMSafeDoor3)                   FlushPanel=fNote->palSafeDoor3;
    else if(Pos==MMSafeDoor4)                   FlushPanel=fNote->palSafeDoor4;
    else if(Pos==MMSafeDoor5)                   FlushPanel=fNote->palSafeDoor5;
    else if(Pos==MMSafeDoor6)                   FlushPanel=fNote->palSafeDoor6;
    else if(Pos==MMSafeDoor7)                   FlushPanel=fNote->palSafeDoor7;
    else if(Pos==MMSafeDoor8)                   FlushPanel=fNote->palSafeDoor8;
    else if(Pos==MMSafeDoor9)                   FlushPanel=fNote->palSafeDoor9;
    else if(Pos==MMSafeDoor10)                  FlushPanel=fNote->palSafeDoor10;
    //Steven 20130205 : 離子風扇異常顯示
    else if(Pos==MMIonFan01)                    FlushPanel=fNote->palIonFan01;
    else if(Pos==MMIonFan02)                    FlushPanel=fNote->palIonFan02;
    else if(Pos==MMIonFan03)                    FlushPanel=fNote->palIonFan03;
    else if(Pos==MMIonFan04)                    FlushPanel=fNote->palIonFan04;
    else if(Pos==MMIonFan05)                    FlushPanel=fNote->palIonFan05;
    else if(Pos==MMIonFan06)                    FlushPanel=fNote->palIonFan06;
    else if(Pos==MMIonFan07)                    FlushPanel=fNote->palIonFan07;
    else if(Pos==MMIonFan08)                    FlushPanel=fNote->palIonFan08;
    else if(Pos==MMIonFan09)                    FlushPanel=fNote->palIonFan09;
    else if(Pos==MMIonFan10)                    FlushPanel=fNote->palIonFan10;
    else if(Pos==MMIonFan11)                    FlushPanel=fNote->palIonFan11;
    else if(Pos==MMIonFan12)                    FlushPanel=fNote->palIonFan12;
    else if(Pos==MMATC_Handler)                 FlushPanel=fNote->pnlATC_Handler;
    else if(Pos==MMATC_TCPIP)                   FlushPanel=fNote->pnlATC_TCPIP;
    else if(Pos==MMATC_NI)                      FlushPanel=fNote->pnlATC_NI;
    else if(Pos==MMATC_ATC)                     FlushPanel=fNote->pnlATC_ATC;
    else if(Pos==MMATC_Chiller)                 FlushPanel=fNote->pnlATC_Chiller;
    else if(Pos==MMATC_RS232)                   FlushPanel=fNote->pnlATC_RS232;
    else if(Pos==MMATC_Head)                    FlushPanel=fNote->pnlATC_Head;
    else if(Pos==MMATC_PowerSupply)             FlushPanel=fNote->pnlATC_PowerSupply;
    else if(Pos==MMATC_WaterValve)              FlushPanel=fNote->pnlATC_WaterValve;
    else if(Pos==MMInShuttle)
    {
        fNote->palInSh ->Visible=true;
        fNote->palInSh1->Visible=false;
        fNote->palInSh2->Visible=false;
        fNote->palHead ->Left=301;
        FlushPanel=fNote->palInSh;
    }
    else if(Pos==MMOutShuttle)
    {
        fNote->palOutSh ->Visible=true;
        fNote->palOutSh1->Visible=false;
        fNote->palOutSh2->Visible=false;
        fNote->palHead  ->Left=145;
        FlushPanel=fNote->palOutSh;
    }
    else if(Pos==MMIndex)
    {
        FlushPanel=fNote->palHead;

        fNote->RTCalarmImageUnload();                                           //jou 20170210 (Steven) : RTC alarm image unload
    }
    else if(Pos==MMTemperature)
    {
        fNote->palTemp->Visible=true;
        FlushPanel=fNote->palTemp;
    }
    else if(Pos==MMInterface)
    {
        FlushPanel=fNote->palIF;
    }
    else if(Pos==MMScanner)
    {
        FlushPanel=fNote->palScan;
        fNote->palScan->Visible=true;
    }
    else if(Pos==MMCCD)
    {
        FlushPanel=fNote->palCCD;
        fNote->palCCD->Visible=true;
        TDateTime nowT = Now();
        TDateTime rangeT = EncodeTime(0, 0, 30 ,0);
        if(REAL_TIME_CCD)                                                       //Steven 20110830 : Real Time CCD - Socket有IC要顯示畫面
        {
            fNote->palRTCFullView->Parent=fNote->tsHandler;
            fNote->palRTCFullView->BringToFront();
            fNote->Label8->Visible=true;
            if(MachineTypeChoice==Type_HT9046_LS)                               //2013-01-15    Dell
            {
                if(FileExists(FULLVIEWIMAGEPATHNG1))
                {                                                               //2013-09-18    Dell    檢查VisionNG檔案有沒有更新
                    TDateTime fTime=FileDateToDateTime(FileAge(FULLVIEWIMAGEPATHNG1));
                    if(nowT>(fTime+rangeT))
                    {
                        FlushPanel->Visible=true;
                        return;
                    }

                    fNote->Image1->Picture->LoadFromFile(FULLVIEWIMAGEPATHNG1);
                }

                if(FileExists(FULLVIEWIMAGEPATHNG3))
                {
                    fNote->Image2->Picture->LoadFromFile(FULLVIEWIMAGEPATHNG3);
                }

                if(FileExists(FULLVIEWIMAGEPATHNG2))
                {
                    fNote->Image3->Picture->LoadFromFile(FULLVIEWIMAGEPATHNG2);
                }

                if(FileExists(FULLVIEWIMAGEPATHNG4))
                {
                    fNote->Image4->Picture->LoadFromFile(FULLVIEWIMAGEPATHNG4);
                }
            }
            else
            {
                if(FileExists(FULLVIEWIMAGEPATHNG1))
                {
                    fNote->Image1->Picture->LoadFromFile(FULLVIEWIMAGEPATHNG1);
                }

                if(FileExists(FULLVIEWIMAGEPATHNG2))
                {
                    fNote->Image2->Picture->LoadFromFile(FULLVIEWIMAGEPATHNG2);
                }
            }

            if(CUSTOMER_CODE==CC_SCC)                                           //jou 20180420 : SCC要求RTC alarm要做piggyback
            {
                iWhoTriggerPiggyBack=pbtRTCAlarm;                               //Steven 20111207 : 誰觸發了Piggy Back
                ProcessPiggyBackFunction();
            }

            fNote->RTCalarmImageUnload();                                       //jou 20170210 (Steven) : RTC alarm image unload
        }
    }
    else if(Pos==MMOCR)                                                         //ChungHung 20120830 add OCR Function add
    {
        FlushPanel=fNote->palOCR;
        fNote->palOCR->Visible=true;
        fNote->Label8->Visible=false;
        if(INSTALL_OCR!=eocrUninstal)
        {
            fNote->palRTCFullView->Parent=fNote->tsHandler;
            fNote->palRTCFullView->BringToFront();
            fNote->Image1->Visible=false;                                       //ChungHung 20130122 add

            if(CosFunction.bTrayOCR)                                            //wei 20160805
            {
                AnsiString OrgPath="D:\\HT9045_Log\\OCR\\Alarm\\Bad\\";
                if(asOCRImageName!="")
                {
                    OrgPath+=asOCRImageName;
                    if(FileExists(OrgPath))
                    {
                        fInputForm->Image1->Picture->LoadFromFile(OrgPath);
                        fNote->Image2->Picture->LoadFromFile(OrgPath);
                    }
                }
            }
            else
            {
                if(FileExists(OCRINSPNG))
                {
                    fNote->Image2->Picture->LoadFromFile(OCRINSPNG);
                    fNote->Image1->Canvas->TextOutA(10, 10, "InspNF");
                }
            }
        }
    }
    else
    {
        FlushPanel=fNote->palSys;
    }
    FlushPanel->Visible=true;
}
//---------------------------------------------------------------------------
void __fastcall TfNote::PlayMovie(AnsiString asPathFile)
{
    if(FileExists(asPathFile))
    {
        pnlMovie->Visible=true;
        MediaPlayer1->FileName=asPathFile;
        MediaPlayer1->DeviceType=dtAVIVideo;                                    //set Device compatibility to AVI
        MediaPlayer1->Open();
        MediaPlayer1->DisplayRect=Rect(2, 2, pnlMovie->Width-8, pnlMovie->Height-8);                                    //Set Screen Size
        MediaPlayer1->Display=pnlMovie;                                         //Set display device to a TPanel
        MediaPlayer1->AutoRewind=true;
        MediaPlayer1->Play();
        MovieToBack();
    }
    else
    {
        pnlMovie->Visible=false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfNote::StopMovie()
{
    if(pnlMovie->Visible)
    {
        MediaPlayer1->FileName="";
        MediaPlayer1->Stop();
        MovieToBack();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfNote::MediaPlayer1Notify(TObject *Sender)
{
    if(MediaPlayer1->NotifyValue==nvSuccessful)
        MediaPlayer1->Play();
}
//---------------------------------------------------------------------------
void __fastcall TfNote::palShtSensorSOPClick(TObject *Sender)
{
    if(SHUTTLE_SENSOR_TYPE==eSensorLine)                                        //Line Sensor, show SOP button
    {
        ShellExecute(NULL, "Open", "D:\\HT9045\\Error\\SOP\\Shuttle.pdf", NULL, NULL, SW_MAXIMIZE);
    }
    else if(SHUTTLE_SENSOR_TYPE==eSensorCCLink  ||                              //Steven 20131008 : for HT9046AH
            SHUTTLE_SENSOR_TYPE==eSensorCCLink3 ||                              //CC-Link調整
            UseCanBusOrEtherCAT())                                              //Sam 20230707 : EtherCAT Shuttle sensor
    {
        fCCLink->ShowUseSensor(TestIF.iTestMode, TestIF.dSiteXPitch);
        fCCLink->Show();
    }
}
//---------------------------------------------------------------------------
//extern AnsiString asTempCtrl[tcTotalCount];
void __fastcall TfNote::ShowErrSite(AnsiString Code, int UnitNo, int AxleNo, AnsiString errPart)
{
    int i, j, x, y;

    TMyTray1->Visible=true;
    if(UnitNo==1 || UnitNo==2)                                                  //In Out Arm
    {
        TMyTray1->XItem=InArmSuck.iMaxCol;                                      //Steven for HT1032
        TMyTray1->YItem=InArmSuck.iMaxRow;

        for(i=0; i<TMyTray1->YItem; i++)
        {
            for(j=0; j<TMyTray1->XItem; j++)
            {
                TMyTray1->SetCellNumber(j, i, InArmSuck.Suck[i][j].sName.c_str());
            }
        }

        if(Code=="MES0101" || Code=="JAM0109" || Code=="JAM0110" || Code=="JAM0111" || Code=="JAM0127" ||               //kevin 20130115 jam 0114
           Code=="JAM0203" || Code=="JAM0201" || Code=="JAM0202" || Code=="JAM0128" ||                                  //JerryYang 20160516 add JAM0128
           Code=="WAR0132" || Code=="WAR0226" || Code=="WAR0204" || Code=="JAM0126" || Code=="JAM0114")
        {
            if(errPart.Pos("DoCatchTray")>0)                                    //JerryYang 20250723 : fix error part 錯誤
            {
                return;
            }

            for(i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(j=0; j<InArmSuck.iMaxCol; j++)
                {
                    if(errPart.Pos(InArmSuck.Suck[i][j].sName)>0)
                    {
                        TMyTray1->SetCellColorIndex(j, i, 3);
                        iJamSkipICCount++;
                    }
                    else
                    {
                        TMyTray1->SetCellColorIndex(j, i, 0);
                    }
                }
            }
        }
        else
        {
            switch(AxleNo)
            {
                case 101: case 201: y=0; x=0; break;                            //A
                case 102: case 202: y=1; x=0; break;                            //B
                case 103: case 203: y=0; x=1; break;                            //C
                case 104: case 204: y=1; x=1; break;                            //D
                case 105: case 205: y=0; x=2; break;                            //E
                case 106: case 206: y=1; x=2; break;                            //F
                case 107: case 207: y=0; x=3; break;                            //G
                case 108: case 208: y=1; x=3; break;                            //H
                default: y=2; x=9; break;
            }

            for(i=0; i<TMyTray1->YItem; i++)
            {
                for(j=0; j<TMyTray1->XItem; j++)
                {
                    if(j==x && i==y)
                    {
                        TMyTray1->SetCellColorIndex(j, i, 3);
                        iJamSkipICCount++;
                    }
                    else
                    {
                        TMyTray1->SetCellColorIndex(j, i, 0);
                    }
                }
            }
        }
    }
    else if(UnitNo==3)                                                          //Index
    {
        TMyTray1->XItem=TestSocket.iShtCol;
        TMyTray1->YItem=TestSocket.iShtRow;

        if(errPart!="")                                                         //Steven 20101123
        {
            for(i=0; i<TestSocket.iShtRow; i++)
            {
                for(j=0; j<TestSocket.iShtCol; j++)
                {
                    TMyTray1->SetCellNumber(j, i, IndexSuckName[i][j].c_str());
                    if(errPart.Pos(IndexSuckName[i][j])>0)
                    {
                        TMyTray1->SetCellColorIndex(j, i, 3);
                        iJamSkipICCount++;
                    }
                    else
                    {
                        TMyTray1->SetCellColorIndex(j, i, 0);
                    }
                }
            }
        }
        else
        {
            switch(AxleNo)
            {
                case 311: case 321: y=0; x=0; break;                            //Aa
                case 312: case 322: y=0; x=1; break;                            //Ab
                case 313: case 323: y=0; x=2; break;                            //Ac
                case 314: case 324: y=0; x=3; break;                            //AD
                case 315: case 325: y=1; x=0; break;                            //Ba
                case 316: case 326: y=1; x=1; break;                            //Bb
                case 317: case 327: y=1; x=2; break;                            //Bc
                case 318: case 328: y=1; x=3; break;                            //Bd
                case 331: case 341: y=0; x=4; break;                            //Ae
                case 332: case 342: y=0; x=5; break;                            //Af
                case 333: case 343: y=0; x=6; break;                            //Ag
                case 334: case 344: y=0; x=7; break;                            //Ah
                case 335: case 345: y=1; x=4; break;                            //Be
                case 336: case 346: y=1; x=5; break;                            //Bf
                case 337: case 347: y=1; x=6; break;                            //Bg
                case 338: case 348: y=1; x=7; break;                            //Bh
                default: x=9; break;
            }

            for(i=0; i<TMyTray1->YItem; i++)
            {
                for(j=0; j<TMyTray1->XItem; j++)
                {
                    TMyTray1->SetCellNumber(j, i, IndexSuckName[i][j].c_str());

                    if(j==x && i==y)
                    {
                        TMyTray1->SetCellColorIndex(j, i, 3);
                        iJamSkipICCount++;
                    }
                    else
                    {
                        TMyTray1->SetCellColorIndex(j, i, 0);
                    }
                }
            }
        }

        if(CUSTOMER_CODE==CC_KYEC_LEE &&                                        //Ifor 20171228 (Steven) : add Index Drop Error 強制RESET 需加上 In SH 上的IC數量
           (Code=="JAM0303" || Code=="JAM0304"))
        {
            iJamSkipICCount=iJamSkipICCount+ FLCarryKit.CountRealIC()+ BLCarryKit.CountRealIC();
        }
    }
    else if(UnitNo==4)                                                          //In Shuttle
    {
        if(MachineTypeChoice==Type_HT9046 || MachineTypeChoice==Type_HT9046_LS)                                         //9046
            TMyTray1->XItem=9;
        else
            TMyTray1->XItem=7;
        TMyTray1->YItem=1;

        if(Code=="JAM0401" || Code=="JAM0402" || Code=="JAM0403" || Code=="JAM0404" ||
           Code=="JAM0405" || Code=="JAM0406" || Code=="JAM0452" || Code=="JAM0407" || Code=="JAM0408")
        {
            if(SHUTTLE_SENSOR_TYPE==eSensorLine)                                //Line Sensor, show SOP button
            {
                fNote->labShtSensorSOP->Caption="Shuttle Sensor Adjusting SOP";
                fNote->labShtSensorSOP2->Caption="感應器調整方法";
                fNote->palShtSensorSOP->Visible=true;                           //Steven 20100319
            }
            else if(SHUTTLE_SENSOR_TYPE==eSensorCCLink  ||                      //CC-Link調整
                    SHUTTLE_SENSOR_TYPE==eSensorCCLink3 ||                      //Steven 20131008 : for HT9046AH
                    UseCanBusOrEtherCAT())                                      //Sam 20230707 : EtherCAT Shuttle sensor
            {
                fNote->labShtSensorSOP->Caption="Shuttle Sensor Adjustment";
                fNote->labShtSensorSOP2->Caption="感應器調整";
                fNote->palShtSensorSOP->Visible=true;                           //Steven 20100610
            }
            else
            {
                fNote->palShtSensorSOP->Visible=false;
            }
        }
        else
        {
            fNote->palShtSensorSOP->Visible=false;
        }

        if(Code=="JAM0459" || Code=="JAM0460" || Code=="JAM0461" ||             //kevin 20190920 JAM0459
           Code=="WAR0465" || Code=="WAR0466" ||
           Code=="WAR0467" || Code=="WAR0468" ||
           Code=="WAR0471" || Code=="WAR0472" )                                 //Steven 20160428 : 檢查2D重複碼
        {
            if(BAR_CODE_INSTALL!=ebctInShtIntel ||
               ((BAR_CODE_INSTALL==ebctInShtIntel ||
                 BAR_CODE_INSTALL==ebctEtherNetCCD) &&
                (Code!="JAM0460" && Code!="JAM0461")))                          //Ifor 20180104 : add Cognex EtherNet 通訊
            {
                t2DCode->Parent =tsHandler;
                t2DCode->Top    =pgcNote->Height/2-pnlLotInfo->Height/2;
                t2DCode->Left   =pgcNote->Width/2-pnlLotInfo->Width/2;
                t2DCode->BringToFront();                                        //Sam 20220301 : 修正 2D Alarm 未顯示。
                t2DCode->Left=206;                                              //Steven 20160503 : 修正Tray顯示方式
            }
        }

        if(Code=="JAM0401" || Code=="JAM0404" ||                                //Steven 20110408
           Code=="JAM0403" || Code=="JAM0406" ||                                //Isaac 20170418 (Steven) 用Y Latch檢測Inshuttle有無IC
           Code=="JAM0481" || Code=="JAM0473")                                  //kevin 20180504 add JAM0481" || Code=="JAM0471"
        {
            for(i=0; i<TMyTray1->XItem; i++)
            {
                TMyTray1->SetCellNumber(i, 0, ShuttleSensorName[i].c_str());
                if(errPart.Pos(ShuttleSensorName[i])>0)
                {
                    TMyTray1->SetCellColorIndex(i, 0, 3);
                    iJamSkipICCount++;
                }
                else
                {
                    TMyTray1->SetCellColorIndex(i, 0, 0);
                }
            }
        }
        else if(Code=="JAM0411" || Code=="JAM0412" || Code=="JAM0460" || Code=="JAM0461" ||                             //Steven 20140228 : 雷射測距功能  //Steven 20160123 : For 2D function
                Code=="WAR0465" || Code=="WAR0466" || Code=="WAR0467" || Code=="WAR0468" ||                             //Steven 20160428 : 檢查2D重複碼
                Code=="WAR0469" || Code=="WAR0470" || Code=="WAR0471" || Code=="WAR0472" ||                             //Steven 20160503 : 確認移動位置與拍照位置相同
                Code=="WAR0476" || Code=="WAR0477" )                            //wei 20160922 增加 WAR0476 WAR0477
        {
            TMyTray1->XItem=FLCarryKit.iShtCol;
            TMyTray1->YItem=FLCarryKit.iShtRow;

            for(i=0; i<FLCarryKit.iShtRow; i++)
            {
                for(j=0; j<FLCarryKit.iShtCol; j++)
                {
                    TMyTray1->SetCellNumber(j, i, IndexSuckName[i][j].c_str());
                    if(errPart.Pos(IndexSuckName[i][j])>0)
                    {
                        TMyTray1->SetCellColorIndex(j, i, 3);
                        iJamSkipICCount++;
                    }
                    else
                    {
                        TMyTray1->SetCellColorIndex(j, i, 0);
                    }
                }
            }
        }
        else
        {
            switch(AxleNo)
            {
                case 411: case 421: case 511: case 521: x=0; break;
                case 412: case 422: case 512: case 522: x=1; break;
                case 413: case 423: case 513: case 523: x=2; break;
                case 414: case 424: case 514: case 524: x=3; break;
                case 415: case 425: case 515: case 525: x=4; break;
                case 416: case 426: case 516: case 526: x=5; break;
                case 417: case 427: case 517: case 527: x=6; break;
                case 418: case 428: x=7; break;
                case 419: case 429: x=8; break;
                default: x=9; break;
            }

            for(j=0; j<TMyTray1->XItem; j++)
            {
                TMyTray1->SetCellNumber(j, 0, ShuttleSensorName[j].c_str());

                if(j==x)
                {
                    TMyTray1->SetCellColorIndex(j, 0, 3);
                    iJamSkipICCount++;
                }
                else
                {
                    TMyTray1->SetCellColorIndex(j, 0, 0);
                }
            }
        }
    }
    else if(UnitNo==5)                                                          //Out Shuttle
    {
        if(Code=="JAM0560" || Code=="JAM0570" ||                                //殘料偵測
           Code=="JAM0508" || Code=="JAM0509" ||                                //IC Lose
           Code=="JAM0552")
        {
            if(Code=="JAM0560" || Code=="JAM0508")
            {
                fLtcSensor->SetLtcSensor(0);                                    //JerryYang 20230406 : 只重置異常的shuttle就好    //2013-11-19    Dell    for LTC 可能失效,重新Set 試試看
            }
            else if(Code=="JAM0570" || Code=="JAM0509")
            {
                fLtcSensor->SetLtcSensor(1);                                    //JerryYang 20230406 : 只重置異常的shuttle就好    //2013-11-19    Dell    for LTC 可能失效,重新Set 試試看
            }

            TMyTray1->XItem=FLCarryKit.iShtCol;
            TMyTray1->YItem=FLCarryKit.iShtRow;

            for(i=0; i<FLCarryKit.iShtRow; i++)
            {
                for(j=0; j<FLCarryKit.iShtCol; j++)
                {
                    TMyTray1->SetCellNumber(j, i, IndexSuckName[i][j].c_str());
                    if(errPart.Pos(IndexSuckName[i][j])>0)
                    {
                        TMyTray1->SetCellColorIndex(j, i, 3);
                        iJamSkipICCount++;
                    }
                    else
                    {
                        TMyTray1->SetCellColorIndex(j, i, 0);
                    }
                }
            }
        }
        else
        {
            if(MachineTypeChoice==Type_HT9046)                                  //9046
                TMyTray1->XItem=9;
            else
                TMyTray1->XItem=7;

            TMyTray1->YItem=1;

            if(Code=="JAM0501" || Code=="JAM0504" ||
               Code=="JAM0508" || Code=="JAM0509")
            {
                for(i=0; i<TMyTray1->XItem; i++)
                {
                    TMyTray1->SetCellNumber(i, 0, ShuttleSensorName[i].c_str());
                    if(errPart.Pos(ShuttleSensorName[i])>0)
                    {
                        TMyTray1->SetCellColorIndex(i, 0, 3);
                        iJamSkipICCount++;
                    }
                    else
                    {
                        TMyTray1->SetCellColorIndex(i, 0, 0);
                    }
                }
            }
            else
            {
                switch(AxleNo)
                {
                    case 511: case 521: x=0; break;
                    case 512: case 522: x=1; break;
                    case 513: case 523: x=2; break;
                    case 514: case 524: x=3; break;
                    case 515: case 525: x=4; break;
                    case 516: case 526: x=5; break;
                    case 517: case 527: x=6; break;
                    default: x=9; break;
                }

                for(j=0; j<TMyTray1->XItem; j++)
                {
                    TMyTray1->SetCellNumber(j, 0, ShuttleSensorName[j].c_str());

                    if(j==x)
                    {
                        TMyTray1->SetCellColorIndex(j, 0, 3);
                        iJamSkipICCount++;
                    }
                    else
                    {
                        TMyTray1->SetCellColorIndex(j, 0, 0);
                    }
                }
            }
        }
    }
    else if(UnitNo==7)                                                          //Tester I/F     //Steven 20110603
    {
        TMyTray1->XItem=TestSocket.iShtCol;
        TMyTray1->YItem=TestSocket.iShtRow;

        //Steven 20120423 : Site Yield紅色顯示異常 加入WAR0702
        //"WAR0701"     //Low Yield Alarm!
        //"WAR0702"     //Site Yield Different over
        //"WAR0703"     //Arm Site Yield Different over setting
        //"WAR07321"    //Arm1: consecutive failure
        //"WAR07329"    //Arm2: consecutive failure
        //"WAR07301"    //Socket consecutive
        //"WAR07322"    //Consecutive Pass Error!
        if(Code=="WAR0703" || Code=="WAR0702" || Code=="WAR0705" ||             //Steven 20150709 : Add for Amkor China
           Code=="WAR07321" || Code=="WAR07329" || Code=="WAR07301" ||
           Code=="WAR07322" || Code=="WAR0701" || Code=="WAR07334")             //kevin 20170831 (Steven) add WAR07334
        {
            for(i=0; i<TestSocket.iShtRow; i++)
            {
                for(j=0; j<TestSocket.iShtCol; j++)
                {
                    TMyTray1->SetCellNumber(j, i, IndexSuckName[i][j].c_str());
                    if(errPart.Pos(IndexSuckName[i][j])>0)
                    {
                        TMyTray1->SetCellColorIndex(j, i, 3);
                        iJamSkipICCount++;
                    }
                    else
                    {
                        TMyTray1->SetCellColorIndex(j, i, 0);
                    }
                }
            }
        }
        else
        {
            TMyTray1->Visible=false;
        }
    }
    else if(UnitNo==15)                                                         //溫控器
    {
    }
}
//---------------------------------------------------------------------------
void __fastcall TfNote::BtnResetClick(TObject *Sender)
{
    if(IniConfig.bIndexDropOnlyReset==false)                                    //2014-02-13 Dell 修改Reset 流程
    {
        if(bRunAutoClean)                                                       //Jou 2014-03-11 : Auto Clean時不能Reset
            return;

        fMain->BtnResetClick(fMain);
    }
    else                                                                        //IniConfig.bIndexDropOnlyReset==true                                  //JimmyChiu 20230429 : Index區掉料異常只能Reset清除非盤內資料
    {
        if(bRunAutoClean)                                                       //Jou 2014-03-11 : Auto Clean時不能Reset
            return;
        fMain->BtnResetClick(fMain);
    }

    if(bErrPan_err==true && Pwd!="")
        return;                                                                 //Richard 2011/2/22 SpecialPanel
    #ifdef SOFT_SIMULTE
        if(IsSafeLockCheck())
            return;
    #else
        if(IsTestSitICFallDown())                                               //jou 2011-12-20 修正曾經發生過掉料的話,note訊息沒辦法操作的問題
            return;
        if(bContactCTOverCHK)
            return;

        if(bContactCTOverCHK || bAutoCleanCheckOpenDoor || bChangeCleanPad)     //kevin 20121020 add //Steven 20121015 : Auto Clean有Alarm要開後門
            return;
    #endif
}
//---------------------------------------------------------------------------
bool __fastcall TfNote::DoPassword()                                            //Steven 20101124
{
    bool bFlag=true;

    AnsiString aspwPath="";
    if(IniConfig.bN15UserLevelByTxt==true &&
       CUSTOMER_CODE==CC_Greatek)                                               //Sam 20171130 (Steven) : 超豐密碼檔 FTP 下載
    {
        aspwPath=ESDForm->FTP_PasswordFile_Download(IniConfig.asN15UserLevelByTxtReadFilePath);                         //JimmyChiu 20220219 : FTP_ESDFile_Download==>FTP_PasswordFile_Download
        if(FileExists(aspwPath))
            pwPath=aspwPath;
    }

    bool bTechComExist=FileExists(pwPath);                                      //2012-01-03    Dell modify
    int iLevel=0;
    static bool bEnter=false;                                                   //Steven 20151015 : To avoid double click

    if(bWaitSecsGemReply==true)                                                 //Ifor 20180227 (Steven) add SECS GEM Confirm the Employee ID
    {
        return bFlag;
    }

    iLevel=fSecurity->GetJamLevel(sJamArea, sJamCode);

    if(CosFunction.bConAlarmNeedKeyInPassword==true &&                          //jou 2014-09-04 Continuous Same Alarm N time Need KeyIn Password
       IniConfig.bO16ConAlarmNeedKeyInPasswordCT)
    {
        if(bNeedPassWord==true && iLevel<=0)
        {
            iLevel=LevelSet.AccessLevel[35];
        }
    }
    else if(CosFunction.bConAlarmInTimeLevelUp &&                               //Steven 20210127 : 逸昌要求在單位時間內相同Alarm發生多次,提昇解除alarm權限
            IniConfig.bO17EnableLevelUpWhenContiAlarm)
    {
        if(bNeedPassWord==true && iLevel<=0)
        {
            iLevel=LevelSet.AccessLevel[166];
        }
    }

    if(TestIF_File.bEnableBarCode==true &&
       TestIF_File.bEnableBarcodeCSVCompare==true &&
       edErrorCode->Text=="WAR04217")
    {
        iLevel=1;
    }

    if(CUSTOMER_CODE==CC_SCC)                                                   //Ifor 20181024 add :SCC針對特定Alarm Code解除時需權限密碼
    {
        if(sJamCode=="WAR07301" ||                                              //Socket consecutive failure
           sJamCode=="WAR07321" ||                                              //Arm1: consecutive failure
           sJamCode=="WAR07329" ||                                              //Arm2: consecutive failure
           sJamCode=="JAM0540"  || sJamCode=="JAM0550" ||                       //Device lose at Output Shuttle
           sJamCode=="JAM0303"  || sJamCode=="JAM0304" ||                       //Device drop error (Arm 1) & Device drop error (Arm 2)  //Jou 20140604 Add for SCC
           sJamCode=="WAR0310"  ||                                              //Socket has IC error!                                   //Jou 20140604 Add for SCC
           sJamCode=="JAM0314"  || sJamCode=="JAM0315")                         //JerryYang 20160516 add JAM0314 JAM0315 "Clean pad drop error"
        {
            if(iLevel==0)
                iLevel=1;
        }
    }
    else if(IniConfig.bVTESTFunction==true)
    {
        if(sJamCode=="WAR16123" && Select[1]==true)
            return true;

        if(sJamCode=="WAR07460" && iLevel<1)                                    //AI(ht9045-config) 20260507 (RogerYang) : VTEST銦片超壽命解除需工程師密碼
            iLevel=1;
    }

    if(CosFunction.bStatisticsJamCount==true)                                   //jou 20171201 (Steven) : 新增統計jam code alarm次數,達到設定數量後提高一階權限才能解開alarm
    {
        if(bNeedHighLevelPassword==true)
        {
            iLevel++;
            if(CosFunction.bSecurityHave5Level==true)
            {
                if(iLevel>=4)
                    iLevel=4;
            }
            else
            {
                if(iLevel>=3)
                    iLevel=3;
            }
        }
    }

    if(CosFunction.bEnableHandlerResultServer &&
       fSecurity->GetJemTCPAlarm(sJamArea, sJamCode))                           //Sam 20230620 : 面板已經鎖定需要 IT 下命令解鎖
    {
        iLevel=1;
    }
    //jou 2012-11-28 Bin Yield Failure 的百分比值要可以輸入到小數點一位,另外也要紅底+密碼
    //jou 2016-11-17 make code修改Security password 設定不卡客戶碼
//    if(CUSTOMER_CODE==CC_SCS || CUSTOMER_CODE==CC_SCC || CUSTOMER_CODE==CC_AMKOR_China || IniConfig.bKoreaFunction || IniConfig.bIndexDropNeedPwdByIni || CUSTOMER_CODE==CC_KYEC_XILINX || CUSTOMER_CODE==CC_TSMC_TAINAN)    //wei 20150421 KYEC XILINX Alarm要輸入密碼
    {
        if(bNeedPassWord==true && fInput->fShow==false && bEnter==false)
        {
            bEnter=true;                                                        //Steven 20151015 : To avoid double click
            if(bTechComExist)
                fMain->cbUserSelectChange(NULL);
            else
                fMain->stOperatorClick(fMain);
            bEnter=false;                                                       //Steven 20151015 : To avoid double click
            if(AccessLevel<iLevel)
            {
                bFlag=false;
                if(bTechComExist)
                {
                    fPassword2->Label3->Visible=true;
                    fPassword2->Label4->Visible=true;
                }
                else
                {
                    palWrongPW->Parent=tsHandler;                               //Steven 20111026
                    palWrongPW->BringToFront();
                }
            }
            else
            {
                RecordProcess("==Login for unlock alarm.==", AccessLevel);      //Steven 20220615 : Log for unlock alarm
                if(bTechComExist)
                {
                    fPassword2->Label3->Visible=false;
                    fPassword2->Label4->Visible=false;
                }
                else
                {
                    palWrongPW->Parent=tsRedAlarm;
                }
            }

            if(CUSTOMER_CODE==CC_PTI)                                           //Sam 20230112 : 力成呂其名要求解除 Alarm 後要維持權限。
            {
            }
            else if(bTechComExist)
            {
                fMain->btLogin->Caption="Login";
                fMain->spbUserName->Caption="Operator";
                fMain->cbUserSelect->ItemIndex=0;
                AccessLevel=0;
                fMain->ChangeLevelAttr();
            }
        }
        else if(bEnter==true)                                                   //Steven 20151015 : To avoid double click
        {
            bFlag=false;
        }
    }
    return bFlag;
}
//---------------------------------------------------------------------------
bool __fastcall TfNote::DoUnlockPassword()                                      //Ifor 20170214 (wei) add 解除Alarm 需要獨立密碼
{
    bool bFlag=true;
    if(CosFunction.bUseAlarmUnlockPassWord==true && bAlarmUnlockPassWord==true)
    {
        fPassword->edPassword->Text="";
        fQwertyKey->ShowQwertyKey(fPassword->edPassword, N_NO_SYMBOL|N_NO_SPACE|N_PASSWORD);
        if(asUnlockPassword==fPassword->edPassword->Text)
            bAlarmUnlockPassWord=false;
    }

    if(bAlarmUnlockPassWord==true)
        bFlag=false;

    return  bFlag;
}
//---------------------------------------------------------------------------
void __fastcall TfNote::TMyTray1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(pnlMovie->Visible==true)
    {
        if(pnlMovie->Top!=PanelMain6->Top)
        {
            pnlMovie->Parent    =PanelMain6;
            pnlMovie->Top       =PanelMain6->Top;
            pnlMovie->Left      =PanelMain6->Left;
            pnlMovie->Width     =PanelMain6->Width;
            pnlMovie->Height    =PanelMain6->Height;
            pnlMovie->BringToFront();

            MediaPlayer1->DisplayRect=Rect(2, 2, pnlMovie->Width-8, pnlMovie->Height-8);                                //Set Screen Size
            MediaPlayer1->Previous();
            MediaPlayer1->Play();
        }
        else
        {
            MovieToBack();
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfNote::MovieToBack()
{
    pnlMovie->Top   =TMyTray1->Top;
    pnlMovie->Left  =TMyTray1->Left;
    pnlMovie->Width =TMyTray1->Width;
    pnlMovie->Height=TMyTray1->Height;
    pnlMovie->Parent=pnlPicker;
    TMyTray1->BringToFront();
}
//---------------------------------------------------------------------------
void __fastcall TfNote::pnlMovieMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    MovieToBack();
}
//---------------------------------------------------------------------------
void __fastcall TfNote::PanSpecialNoteClick(TObject *Sender)
{
    if(Pwd=="")
        return;

    fPassword2->edPassword->Text="";                                            //Steven 20120917
    fQwertyKey2->ShowQwertyKey(fPassword2->edPassword, N_NO_SYMBOL|N_NO_SPACE|N_PASSWORD);                              //Steven 20120904 : 改用QWERTY鍵盤    //Steven 20150505 : 剛好在輸入時,Alarm會卡死
    if(fPassword2->edPassword->Text==Pwd)
    {
        bErrPan_err=false;
        PanSpecialNote->Parent=tsRedAlarm;

        if(IniConfig.bTesterTimeUpErrorNeedPassword==true &&
           CUSTOMER_CODE==CC_LINGSEN)                                           //jou 2012-08-28 菱生要求 Test Time Up Error 之後的兩次測試, 不管結果如何都排到R
        {
            iTestTimeUpErrContinueR=2;
        }
    }
}
//---------------------------------------------------------------------------
bool __fastcall TfNote::IsTestSitICFallDown()                                   //jou 2011-12-20 修正曾經發生過掉料的話,note訊息沒辦法操作的問題
{
    bool bReturn=false;
    if(bIsTestSitICFallDown==true &&
       (iPosition==MTestY1 || iPosition==MTestZ1 ||
        iPosition==MTestY2 || iPosition==MTestZ2 ||
        iPosition==MMIndex))
    {
        bReturn=true;
    }
    else if(bIsTestSitICFallDown==true && (edErrorCode->Text=="MES1640" || edErrorCode->Text=="MES1641"))               //JerryYang 20220923 : 掉料後one cycle finish要開門按z1
    {
        bReturn=true;
    }
    else
    {
        bReturn=false;
    }

    if(IniConfig.bOutShLoseNeedOpenChamber==true ||                             //jou 2013-12-12 Out Shuttle Lose Device Need Open Chamber Door and press Z1
       IniConfig.bF24OutShuttleLoseIcOpenIndexDoor)                             //kevin 20180725 add
    {
        if(bIsTestSitICFallDown==true &&
           (iPosition==MOutShuttle1 || iPosition==MOutShuttle2))
        {
            bReturn=true;
        }
    }

    return bReturn;
}
//---------------------------------------------------------------------------
void __fastcall TfNote::IndexMoveToFrontRear()                                  //jou 2012-02-24 index drop error,have button can move arm Y front or Rear
{
    bool bCheckSafeDoor=true;
    bool bScanIndexMotorCanMove=true;

    if(bIndexMoveToFrontRear==false)
    {
        return;
    }

    bCheckSafeDoor=CheckSafeDoorIsClosed();
    bScanIndexMotorCanMove=ScanIndexMotorCanMove();
    if(bCheckSafeDoor==false || bScanIndexMotorCanMove==false)
    {
        bIndexMoveToFrontRear=false;
        StopAllMotor();

        if(bCheckSafeDoor==false)
        {
            ShowMyMessage("Safe Door Close!!", "安全門未關");
        }

        if(bScanIndexMotorCanMove==false)
        {
            ShowMyMessage("Index Motor Error Alarm!!", "Index馬達異常錯誤");
        }
        return;
    }

    int &Task=iIndexMoveToFrontRearTask;
    switch(Task)
    {
        case 1:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "IndexMoveToFrontRear 1"))
            {
                Task=100;
            }
            break;
        case 100:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Front, Prod.TestY2_Middle, iSpeedY, __FUNC__))
            {
                bIndexMoveToFrontRear=false;
                Task=9999;
            }
            break;
        case 1000:
            if(MOT[MTestZ1].Gali_Two_ZAxis_Move(Prod.TestZ1_Safe, iSpeedSlow, "IndexMoveToFrontRear 1000"))
            {
                Task=1100;
            }
            break;
        case 1100:
            if(MOT[MTestY1].GalilTwoY_Move(Prod.TestY1_Middle, Prod.TestY2_Rear, iSpeedY, __FUNC__))
            {
                bIndexMoveToFrontRear=false;
                Task=9999;
            }
            break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfNote::btnMoveToFrontClick(TObject *Sender)
{
    bIndexMoveToFrontRear=true;
    iIndexMoveToFrontRearTask=1;
}
//---------------------------------------------------------------------------
void __fastcall TfNote::btnMoveToRearClick(TObject *Sender)
{
    bIndexMoveToFrontRear=true;
    iIndexMoveToFrontRearTask=1000;
}
//---------------------------------------------------------------------------
void __fastcall TfNote::TimerFTPTimer(TObject *Sender)
{
    static bool bRun=false;
    static int iTime=0;

    if(!fShow)
    {
        bRun=false;
        return;
    }

    if(bRun)
        return;

    if((IniConfig.bP53_ForcedScanBinCodeOfUnloader &&                           //JerryYang 20240111 : add P53 function
        LastSet.iTester==ON_LINE) &&
       (edErrorCode->Text=="MES1120" || edErrorCode->Text=="MES1220" || edErrorCode->Text=="MES1320" ||
       edErrorCode->Text=="MES1720" || edErrorCode->Text=="MES1820" || edErrorCode->Text=="MES1920" ||
       edErrorCode->Text=="MES1124" || edErrorCode->Text=="MES1224" || edErrorCode->Text=="MES1324" ||
       edErrorCode->Text=="MES1724" || edErrorCode->Text=="MES1824" || edErrorCode->Text=="MES1924" ||
       edErrorCode->Text=="MES1712" || edErrorCode->Text=="MES1812" || edErrorCode->Text=="MES1912"))                   //JerryYang 20250429 : fix Auto In/Out
    {
        iTime++;
        if(iTime>10)
        {
            iTime=0;
            sbBinEdit->Visible=true;
            sbBinEdit->Left=200;
            sbBinEdit->Height=100;
            sbBinEdit->Parent=fNote;
            sbBinEdit->BringToFront();
        }
    }

    bRun=true;

    if(bSendJamCodeToFTP)
    {
        if((IniConfig.bSPILFunction==true) &&                                   //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
            IniConfig.bFTPJamCodeUpload && IniConfig.bEnableFTP)                //ChungHung 20140108 add FTP unload jam code
        {
            fFTPClient->UploadFileToServer2(IniConfig.FtpUplaodPath, aJamCodeFilePath);                                 //Steven 20140513 : 移動到Form Show之前
            bSendJamCodeToFTP=false;                                            //Steven 20140526 : bSendJamCodeToFTP 改為Timer處理
        }
    }

    bRun=false;
    if(bCleanOutFinish && beKeepCloseSite)                                      //kevin 20210406 clean out open close Site
    {
        beKeepCloseSite =false;
        ReadTestMode();
        fMain->ShowTestHeadComp(false);
        BtnSkipClick(BtnRetry);
        BtnPauseClick(this);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfNote::BtnAlarmResetClick(TObject *Sender)
{
    NewRecordProcess("MES2116", "ALARM RESET pressed", "Note_BtnAlarmResetClick");
    bAlarmBuzzer=false;                                                         //Alick 20160825 add for SIGURD PeiXing 要求NOTE畫面增加ALARMRESET按鈕功能
    bLampAlarmReset=false;
    bAlarmReset=true;                                                           //Alick 20160907 add for SIGURD PeiXing 記憶MotionView按過AlarmReset
    SW[SwFKAlarmReset].Off();
    SW[SwRKAlarmReset].Off();
}
//---------------------------------------------------------------------------
void __fastcall TfNote::btPrintSummaryClick(TObject *Sender)
{
    if(CUSTOMER_CODE==CC_TERAPOWER)                                             //Sam 20202015 : TCP ART 增加 OneCycle 可手動強制中斷流程
    {
        fMain->cbUserSelectChange(this);
        if(fSecurity->Insufficient(159)==false)
            return;

        RecordProcess("Manually Break ART.");
        LastSet.bBreakSCKART=true;                                              //Sam 20200311 : Fix TCP ART MODE
        TestIF_File.bSCKART_LotDeviceCheck=false;
        UpdateButtonStatus(BtnCleanOut);
        TrayForm.bAutoFeed=true;                                                //Sam 20200709 : 修正 Break ART Loader 殘留 IC 問題。
        BtnPauseClick(this);
    }
    else
    {
        fSCKART->SetLotStatus(fSCKART->iLOTSTATUS_A);                           //Steven 20161025 (wei) : SCK ART function
        if(IniConfig.bSPILFunction==true)
        {
            fMain->SetLotState(8);                                              //JerryYang 20220923 : 矽品版本ART
            fSCKART->SaveTestSummary(1);
            fSCKART->ClearLotInfo();
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfNote::btCorrectCountingClick(TObject *Sender)
{
    fSCKART->SettingPanelOnOff(true);                                           //Steven 20170111 (wei) : Add button for SCK ART
    fSCKART->palARTCount->Align=alRight;
    fSCKART->palARTCount->Parent=fNote;
    pnlCorrectionCount->Visible=false;                                          //Steven 20171128 (Wei) : If user pressed "Auto Correction count" button, pop up message closed automatically
}
//---------------------------------------------------------------------------
void __fastcall TfNote::btCorrectCounting_AutoClick(TObject *Sender)
{
    int ret;
    if(IniConfig.bSPILFunction==true && edErrorCode->Text=="WAR0119")           //JerryYang 20220923 : 矽品版本ART
    {
        bBigMyMessage=true;
        ret=ShowMyMessageBox_YES_NO("你選擇的是執行ART搬運，請再次確認是否需執行ART搬運\r\n1.執行ART搬運 : 請按下確定按鍵\r\n2.不執行ART搬運 : 請按下取消按鍵回到平帳選單重新選擇\r\n3.選擇錯誤: 請按下取消回到平帳選單重新選擇",
                                    "You select Run ART, Please double check if you need to Run ART\r\n1. Need to Run ART : Click YES button to Run ART\r\n2. No need to Run ART : Click NO button to return the Auot correct count menu select again.");

        if(ret==2)
        {
            return;
        }
        ret=ShowMyMessageBox_YES_NO("確定執行ART搬運?", "Are you sure to do Run ART?");
        if(ret==2)
        {
            return;
        }
    }

    if(IniConfig.bSPILFunction==true)                                           //JerryYang 20220923 : 矽品版本ART
    {
        fSCKART->palInputJamCnt->Caption=0;
        fSCKART->palInRemoveCnt->Caption=fSCKART->palInputJamCount->Caption;
        fSCKART->palOutputJamCnt->Caption=0;
        fSCKART->palOutRemoveCnt->Caption=fSCKART->palOutputJamCount->Caption;
    }
    else
    {
        fSCKART->palInputJamCnt->Caption=fSCKART->palInputJamCount->Caption;
        fSCKART->palInRemoveCnt->Caption=0;
        fSCKART->palOutputJamCnt->Caption=fSCKART->palOutputJamCount->Caption;
        fSCKART->palOutRemoveCnt->Caption=0;
    }                                                                           //Richard 20230427 : btnApplyCountClick會將=>  iSCKARTInputCT-=fSCKART->palInputJamCnt->Caption
                                                                                //                   然後會在UpdateCount() 內將 =>palInputCnt->Caption=iSCKARTInputCT
    fSCKART->btnApplyCount->Click();                                            //                   此時palInputCount 跟 palInputCnt 差值則為 iMissMatchQty。

    int iMissMatchQty=atoi(fSCKART->palInputCount->Caption.c_str())-atoi(fSCKART->palInputCnt->Caption.c_str());

    if(IniConfig.bSPILFunction==true)                                           //JerryYang 20220923 : 矽品版本ART
    {
        fSCKART->iInputCount=atoi(fSCKART->palInputCnt->Caption.c_str());
    }
    else
    {
        fSCKART->palInputCnt->Caption=fSCKART->palInputCount->Caption;
        LastSet.iSCKARTInputCT+=iMissMatchQty;
        LastSet.lShuttleCount+=iMissMatchQty;
        LastSet.iSCKART_RTUnitCount+=iMissMatchQty;
        if(LastSet.iTester==OFF_LINE)
        {
            if(BinSelect[OffT].bAutoRetest[eAuto1])
                LastSet.lSCKARTBinCT[0]+=iMissMatchQty;
            else if(BinSelect[OffT].bAutoRetest[eAuto2])
                LastSet.lSCKARTBinCT[1]+=iMissMatchQty;
            else
                LastSet.lSCKARTBinCT[2]+=iMissMatchQty;
        }
        else
        {
            if(BinSelect[FT].bAutoRetest[eAuto1])
                LastSet.lSCKARTBinCT[0]+=iMissMatchQty;
            else if(BinSelect[FT].bAutoRetest[eAuto3])
                LastSet.lSCKARTBinCT[1]+=iMissMatchQty;
            else
                LastSet.lSCKARTBinCT[2]+=iMissMatchQty;
        }
        AnsiString sLog;                                                        //AI(ht9045-staterecord-analysis) 20260519 (RogerYang) : log Auto Correct Count force-fill
        sLog.sprintf("ART Auto Correct Count force-fill: +%d (NewInputPickCount: %d, NewShuttleCount: %d, NewRTUnitCount: %d, BinCT: %d/%d/%d)",
                     iMissMatchQty,
                     LastSet.iSCKARTInputCT,
                     LastSet.lShuttleCount,
                     LastSet.iSCKART_RTUnitCount,
                     LastSet.lSCKARTBinCT[0],
                     LastSet.lSCKARTBinCT[1],
                     LastSet.lSCKARTBinCT[2]);
        RecordProcess(sLog);
    }
    pnlMissMatchQty->Caption=0;                                                 //Steven 20171129 (Wei) : If Input Q'ty < Lot Q'ty condition, press auto correction and input miss match count goes to Reject merge count
    pnlCorrectionCount->Visible=false;                                          //Steven 20171128 (Wei) : If user pressed "Auto Correction count" button, pop up message closed automatically
}
//---------------------------------------------------------------------------
void __fastcall TfNote::FormCreate(TObject *Sender)                             //Sam 20170828 (Steven) : 移植超豐 ESD Control 功能 form HT-7045
{
    listO17=new TList();
    listContAlarmList=new TStringList();                                        //Sam 20231116 : 連續 Alarm 不要上傳伺服器
    listContAlarmTimer=new TList();                                             //Sam 20231116 : 連續 Alarm 不要上傳伺服器
    iContAlarmTime=CheckAndReadIniDataGeneral("System","ContAlarmTime",900);    //Sam 20231116 : 連續 Alarm 不要上傳伺服器
    if(CheckIniData("D:\\HT9045\\system\\SpecialErrNote.ini","PASSWORD","GeneralUserID") == false)
    {
        WriteIniData("D:\\HT9045\\system\\SpecialErrNote.ini","PASSWORD","GeneralUserID","27025312");
    }
    sESDControlMachineGeneralUserID = ReadIniData("D:\\HT9045\\system\\SpecialErrNote.ini","PASSWORD","GeneralUserID",AnsiString("27025312"));
}
//---------------------------------------------------------------------------
void __fastcall TfNote::edtLotCountClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 0, 99999999);
}
//---------------------------------------------------------------------------
void __fastcall TfNote::edtLotIDClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_NO_SYMBOL|N_NO_SPACE);
}
//---------------------------------------------------------------------------
void __fastcall TfNote::tvMNetCustomDrawItem(TCustomTreeView *Sender,
      TTreeNode *Node, TCustomDrawState State, bool &DefaultDraw)
{
    if(Node->Text.AnsiPos("BaudRate")!=0)                                       //Isaac 20181212 (Steven) : Baud Rate防呆功能，和選擇鮑率不同的，顯示紅色
    {
        if((MOTIONNET_SPEED==COMMSPEED_2_5M && Node->Text.AnsiPos(":2.5M")==0) ||
           (MOTIONNET_SPEED==COMMSPEED_5M && Node->Text.AnsiPos(":5M")==0)  ||
           (MOTIONNET_SPEED==COMMSPEED_10M && Node->Text.AnsiPos(":10M")==0) ||
           (MOTIONNET_SPEED==COMMSPEED_20M && Node->Text.AnsiPos(":20M")==0))
        {
            Sender->Canvas->Font->Color=clRed;
        }
    }
}
//==============================================================================
int iDataAdd=0;
//---------------------------------------------------------------------------
void __fastcall TfNote::palInArmClick(TObject *Sender)
{
    int Tag;
    AnsiString strAlarmStart="";
    AnsiString strAlarmEnd="";
    AnsiString asQuery, WhereQuery, UnitName;
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;

    Tag=Ptr->Tag;

    if(FlushPanel->Tag!=Tag)
        return;

    fNote->strngrdMDBQuery->BringToFront();
    fNote->strngrdMDBQuery->Visible=true;
    fNote->strngrdMDBQuery->Top=242;                                            //Steven 20221116 : 調整Note畫面的位置
    fNote->strngrdMDBQuery->Left=2;
    fNote->btCloseMDBQuery->Visible=true;

    strAlarmStart=(strBackUpCode.SubString(4, 2).c_str());
    strAlarmEnd.sprintf("%02d", atoi(strAlarmStart.c_str())+1);

    WhereQuery.sprintf(" WHERE (OccurDateTime >= datetime('now', '-%d days') "
                       " AND (AlarmCode > 'JAM%s' AND AlarmCode < 'JAM%s') "
                       " AND Duplicate=0) ", iDBQueryDays,strAlarmStart, strAlarmEnd);

    asQuery.sprintf(
            "SELECT                                                                                     "
            "      UnitName.UnitName,                                                                   "
            "      AlarmList.AlarmCode,                                                                 "
            "      (AlarmList.Message || ' ' ||  MotorAlarmList.MotMess) Message,                       "
            "      Count(AlarmList.AlarmCode) Count,                                                    "
            "      Sum(EventLog.StopedTime) StopTime,                                                   "
            "      AVG(EventLog.StopedTime) MTTR                                                        "
            "FROM                                                                                       "
            "      EventLog                                                                             "
            "      INNER JOIN AlarmList ON (EventLog.ID_AlarmList = AlarmList.ID_AlarmList)             "
            "      INNER JOIN MotorAlarmList ON (EventLog.ID_MotorList = MotorAlarmList.ID_MotorList)   "
            "      INNER JOIN UnitName ON (AlarmList.UnitNo = UnitName.ID_UnitNo)                       "
            "%s                                                                                         "
            "GROUP BY                                                                                   "
            "       AlarmCode                                                                           "
            "ORDER BY                                                                                   "
            "      Count DESC                                                                           ", WhereQuery);

    MyDBVEventFreq(asQuery, fNote->strngrdMDBQuery);
    fNote->strngrdMDBQuery->RowCount=(fNote->strngrdMDBQuery->RowCount+1);

    for(int j=0; j<fNote->strngrdMDBQuery->RowCount; j++)
    {
        if(fNote->strngrdMDBQuery->Cells[2][j]==strBackUpCode)
        {
            iDataAdd=j;
            break;
        }
    }

    for(int i=0; i<fNote->strngrdMDBQuery->ColCount; i++)
    {
        fNote->strngrdMDBQuery->Cells[i][fNote->strngrdMDBQuery->RowCount-1]="";
    }
    RecordProcess("OCAP DB Show");
}
//---------------------------------------------------------------------------
void __fastcall TfNote::PanelMain6MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fNote->strngrdMDBQuery->SendToBack();
    fNote->strngrdMDBQuery->Visible=false;
    fNote->btCloseMDBQuery->Visible=false;
}
//---------------------------------------------------------------------------
void __fastcall TfNote::btCloseMDBQueryClick(TObject *Sender)
{
    fNote->strngrdMDBQuery->SendToBack();
    fNote->strngrdMDBQuery->Visible=false;
    fNote->btCloseMDBQuery->Visible=false;
}
//---------------------------------------------------------------------------
void __fastcall TfNote::strngrdMDBQueryDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
    if(ARow==iDataAdd)
    {
        fNote->strngrdMDBQuery->Canvas->Font->Color=clRed;                      //字體顏色
        fNote->strngrdMDBQuery->Canvas->Brush->Color=clYellow;                  //背景顏色
        fNote->strngrdMDBQuery->Canvas->FillRect(Rect);
        fNote->strngrdMDBQuery->Canvas->TextOut(Rect.Left, Rect.Top,fNote->strngrdMDBQuery->Cells[ACol][ARow]);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfNote::reDescriptionMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(bShowBigDescription==false)
    {
        bShowBigDescription=true;
        fNote->reBigDescription->Parent=tsHandler;
        fNote->reBigDescription->Align=alClient;                                //Steven 20221116 : 調整Note畫面的位置
        fNote->reBigDescription->BringToFront();
        RecordProcess("OCAP Instructions Show");
    }
    else
    {
        bShowBigDescription=false;
        fNote->reBigDescription->Parent=tsBigDescription;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfNote::AlarmLogXmlToFTP(AnsiString sAlarmCode, AnsiString sMsg)
{
    if(IniConfig.bN27_UseAlarmLogXmlUpdataToFTP==false)
        return;
    char Filename[256], str[1024];
    bool bAddBoard=false;
    AnsiString sXML="", sTemp="", sFileName="";
    MyForceDirectories(asAlarmLogXmlPath, "AlarmLogXmlToFTP");
    GetTimeInfo();
    sprintf(Filename, "%s\\%s_Test_gating_alarms_%04d%02d%02d%02d%02d%02d.xml", asAlarmLogXmlPath, IniConfig.sN27_TesterID, SystemYear,SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    if(FileExists(Filename))
    {
        SystemSec++;
        sprintf(Filename, "%s\\%s_Test_gating_alarms_%04d%02d%02d%02d%02d%02d.xml", asAlarmLogXmlPath, IniConfig.sN27_TesterID, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    }
    sFileName.sprintf("%s_Test_gating_alarms_%04d%02d%02d%02d%02d%02d.xml", IniConfig.sN27_TesterID, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);

    FILE *Fp = fopen(Filename, "a+");

    //<TEST_ALARMS>\n
    //<TesterID>E750-45</TesterID>\n
    //<Alarm_Start_Date>2020/05/25</Alarm_Start_Date>\n
    //<Alarm_Start_Time>22:27</Alarm_Start_Time>\n
    //<Alarm_message>Continuous_fail</Alarm_message>\n
    //<Boardcast_message>Continuous_fail</Boardcast_message>\n
    //</TEST_ALARMS>\n

    sXML += "<TEST_ALARMS>\n";
    sTemp = IniConfig.sN27_TesterID;
    sXML += "<TesterID>" + sTemp + "</TesterID>\n";
    sTemp.printf("%04d/%02d/%02d", SystemYear, SystemMonth, SystemDate);
    sXML += "<Alarm_Start_Date>" + sTemp + "</Alarm_Start_Date>\n";
    sTemp.printf("%02d:%02d", SystemHour, SystemMin);
    sXML += "<Alarm_Start_Time>" + sTemp + "</Alarm_Start_Time>\n";
    sTemp = sMsg;
    sXML += "<Alarm_message>" + sTemp + "</Alarm_message>\n";

    bAddBoard=false;
    if(CUSTOMER_CODE==CC_SIGURD_ChungXing)                                      //Sam 20210911 :  矽格中興廠 Alarm Message Board 是否顯示增加開關
    {
        if(fSecurity->GetN27AddBoard(sJamArea, sJamCode))
        {
            bAddBoard=true;
        }
    }
    else
    {
        bAddBoard=true;
    }

    if(bAddBoard)
    {
        sTemp = sMsg;
        sXML += "<Boardcast_message>" + sTemp + "</Boardcast_message>\n";
    }

    sXML += "</TEST_ALARMS>";

    strncpy(str, sXML.c_str(), sizeof(str));
    strcat(str, "\n");
    if(Fp!=NULL)
    {
        fputs(str, Fp);
        fclose(Fp);
    }

    if(fSecurity->GetN27AlarmSel(sJamArea, "") ||
       fSecurity->GetN27AlarmSel(sJamArea, sJamCode))
        fFTPClient->UploadFileFTP(asAlarmLogXmlPath, sFileName, IniConfig.sN27_FTPUplaodPath,sFileName, IniConfig.sN27_FTPUserName, IniConfig.sN27_FTPPassword, IniConfig.sN27_FTPHost, __FUNC__);
}
//---------------------------------------------------------------------------
void __fastcall TfNote::btCorrectCounting_StopARTClick(TObject *Sender)
{
    int ret;
    if(IniConfig.bSPILFunction==true &&                                         //JerryYang 20220923 : 矽品版本ART
       edErrorCode->Text=="WAR0119")
    {
        bBigMyMessage=true;
        ret=ShowMyMessageBox_YES_NO("你選擇的是中斷ART,請確認是否中斷ART\r\n1.中斷ART : 請按下確定按鍵\r\n2.不執行中斷ART : 請按下取消按鍵回到平帳選單重新選擇\r\n3.選擇錯誤 : 請按下取消回到平帳選單重新選擇",
            "You select Stop ART, Please double check if you need to Stop ART\r\n1. Need to Stop ART: Click YES button to Stop ART\r\n2. No need to Stop ART : Click NO button to return to Auot correct count menu select again.\r\n");
        if(ret==2)
        {
            return;
        }
        ret=ShowMyMessageBox_YES_NO("確定中斷ART?", "Are you sure to Stop ART?");
        if(ret==2)
        {
            return;
        }
    }

    if(IniConfig.bSPILFunction==true)
    {
        bStopART=true;
        fSCKART->palInputJamCnt->Caption=0;
        fSCKART->palInRemoveCnt->Caption=fSCKART->palInputJamCount->Caption;
        fSCKART->palOutputJamCnt->Caption=0;
        fSCKART->palOutRemoveCnt->Caption=fSCKART->palOutputJamCount->Caption;
    }
    else
    {
        fSCKART->palInputJamCnt->Caption=fSCKART->palInputJamCount->Caption;
        fSCKART->palInRemoveCnt->Caption=0;
        fSCKART->palOutputJamCnt->Caption=fSCKART->palOutputJamCount->Caption;
        fSCKART->palOutRemoveCnt->Caption=0;
    }

    fSCKART->btnApplyCount->Click();

    int iMissMatchQty=atoi(fSCKART->palInputCount->Caption.c_str())-atoi(fSCKART->palInputCnt->Caption.c_str());

    if(IniConfig.bSPILFunction==true)
    {
        fSCKART->iInputCount=atoi(fSCKART->palInputCnt->Caption.c_str());
    }
    else
    {
        fSCKART->palInputCnt->Caption=fSCKART->palInputCount->Caption;
        LastSet.iSCKARTInputCT+=iMissMatchQty;
        LastSet.lShuttleCount+=iMissMatchQty;
        LastSet.iSCKART_RTUnitCount+=iMissMatchQty;
        if(LastSet.iTester==OFF_LINE)
        {
            if(BinSelect[OffT].bAutoRetest[eAuto1])
                LastSet.lSCKARTBinCT[0]+=iMissMatchQty;
            else if(BinSelect[OffT].bAutoRetest[eAuto2])
                LastSet.lSCKARTBinCT[1]+=iMissMatchQty;
            else
                LastSet.lSCKARTBinCT[2]+=iMissMatchQty;
        }
        else
        {
            if(BinSelect[FT].bAutoRetest[eAuto1])
                LastSet.lSCKARTBinCT[0]+=iMissMatchQty;
            else if(BinSelect[FT].bAutoRetest[eAuto2])
                LastSet.lSCKARTBinCT[1]+=iMissMatchQty;
            else
                LastSet.lSCKARTBinCT[2]+=iMissMatchQty;
        }
        AnsiString sLog;                                                        //AI(ht9045-staterecord-analysis) 20260519 (RogerYang) : log StopART force-fill
        sLog.sprintf("ART StopART force-fill: +%d (NewInputPickCount: %d, NewShuttleCount: %d, NewRTUnitCount: %d, BinCT: %d/%d/%d)",
                     iMissMatchQty,
                     LastSet.iSCKARTInputCT,
                     LastSet.lShuttleCount,
                     LastSet.iSCKART_RTUnitCount,
                     LastSet.lSCKARTBinCT[0],
                     LastSet.lSCKARTBinCT[1],
                     LastSet.lSCKARTBinCT[2]);
        RecordProcess(sLog);
    }
    pnlMissMatchQty->Caption=0;                                                 //Steven 20171129 (Wei) : If Input Q'ty < Lot Q'ty condition, press auto correction and input miss match count goes to Reject merge count
    pnlCorrectionCount->Visible=false;                                          //Steven 20171128 (Wei) : If user pressed "Auto Correction count" button, pop up message closed automatically
}
//---------------------------------------------------------------------------
void __fastcall TfNote::btContinuousTestClick(TObject *Sender)
{
    if(IniConfig.bSPILFunction==true &&
       edErrorCode->Text=="WAR0119")                                            //JerryYang 20220923 : 矽品版本ART
    {
        bBigMyMessage=true;
        ShowMyMessage("續測未測品步驟:\r\n1.請將未測品上料至Loader後按下確定按鍵\r\n2.操作分類機按下Retry + start 進行未測品測試",
         "Continue test untest IC:\r\n1.Put the untest IC to the loader and click YES button\r\n2.Handler click Retry + start to do the test the untest IC.");
    }
}
//---------------------------------------------------------------------------
void __fastcall TfNote::ShowTrayQty()                                           //Sam 20221101 : OneCycle 完成後顯示所有 Tray 各別的IC數量
{
    pgcNote->ActivePageIndex=2;
    pgcNote->ActivePage =tsTrayQty;
    pnlTrayQty->Visible=true;                                                   //Sam 20240804 : 修正顯示
    edQtyLoader->Text   =MOT[MMTrayY].Tray.HowManyIC();
    edQtyAuto1->Text    =MOT[MMAuto1].Tray.HowManyIC();
    edQtyAuto2->Text    =MOT[MMAuto2].Tray.HowManyIC();
    edQtyAuto3->Text    =MOT[MMAuto3].Tray.HowManyIC();

    if(AUTO_EMPTY_COLOR>=3)                                                     //Steven 20230907 : For HT-9011UC
    {
        pnlFix4->Left       =365;
        pnlFix4->Top        =4;
        pnlFix4->Visible    =true;

        pnlFix789->Left     =100;
        pnlFix789->Top      =64;
        pnlFix789->Visible  =(TrayForm.bTrayUpDownSet[eFix1] ||
                              TrayForm.bTrayUpDownSet[eFix2] ||
                              TrayForm.bTrayUpDownSet[eFix3]);

        pnlFix10->Visible   =(TrayForm.bTrayUpDownSet[eFix4] ||
                              TrayForm.bTrayUpDownSet[eFix5] ||
                              TrayForm.bTrayUpDownSet[eFix6]);

        if(TrayForm.bTrayUpDownSet[eFix1])
        {
            edQtyFix1->Text =MOT[MManualTray1].Tray.HowManyUpperHalfIC();
            edQtyFix7->Text =MOT[MManualTray1].Tray.HowManyLowerHalfIC();
        }
        else
        {
            edQtyFix1->Text =MOT[MManualTray1].Tray.HowManyIC();
        }

        if(TrayForm.bTrayUpDownSet[eFix2])
        {
            edQtyFix2->Text =MOT[MManualTray2].Tray.HowManyUpperHalfIC();
            edQtyFix8->Text =MOT[MManualTray2].Tray.HowManyLowerHalfIC();
        }
        else
        {
            edQtyFix2->Text =MOT[MManualTray2].Tray.HowManyIC();
        }

        if(TrayForm.bTrayUpDownSet[eFix3])
        {
            edQtyFix3->Text =MOT[MManualTray3].Tray.HowManyUpperHalfIC();
            edQtyFix9->Text =MOT[MManualTray3].Tray.HowManyLowerHalfIC();
        }
        else
        {
            edQtyFix3->Text =MOT[MManualTray3].Tray.HowManyIC();
        }

        if(TrayForm.bTrayUpDownSet[eFix4])
        {
            edQtyFix4->Text =MOT[MManualTray4].Tray.HowManyUpperHalfIC();
            edQtyFix10->Text=MOT[MManualTray4].Tray.HowManyLowerHalfIC();
        }
        else
        {
            edQtyFix4->Text =MOT[MManualTray4].Tray.HowManyIC();
        }

        if(TrayForm.bTrayUpDownSet[eFix5])
        {
            edQtyFix5->Text =MOT[MManualTray5].Tray.HowManyUpperHalfIC();
            edQtyFix11->Text=MOT[MManualTray5].Tray.HowManyLowerHalfIC();
        }
        else
        {
            edQtyFix5->Text =MOT[MManualTray5].Tray.HowManyIC();
        }

        if(TrayForm.bTrayUpDownSet[eFix6])
        {
            edQtyFix6->Text =MOT[MManualTray6].Tray.HowManyUpperHalfIC();
            edQtyFix12->Text=MOT[MManualTray6].Tray.HowManyLowerHalfIC();
        }
        else
        {
            edQtyFix6->Text =MOT[MManualTray6].Tray.HowManyIC();
        }
    }
    else
    {
        pnlFix4->Left       =100;
        pnlFix4->Top        =64;
        pnlFix4->Visible    =true;
        labQtyFix4->Visible =TrayForm.bTrayUpDownSet[eFix1];
        edQtyFix4->Visible  =TrayForm.bTrayUpDownSet[eFix1];
        labQtyFix5->Visible =TrayForm.bTrayUpDownSet[eFix2];
        edQtyFix5->Visible  =TrayForm.bTrayUpDownSet[eFix2];
        labQtyFix6->Visible =TrayForm.bTrayUpDownSet[eFix3];
        edQtyFix6->Visible  =TrayForm.bTrayUpDownSet[eFix3];

        pnlFix789->Visible  =false;
        pnlFix10->Visible   =false;

        if(TrayForm.bTrayUpDownSet[eFix1])
        {
            edQtyFix1->Text=MOT[MManualTray1].Tray.HowManyUpperHalfIC();
            edQtyFix4->Text=MOT[MManualTray1].Tray.HowManyLowerHalfIC();
        }
        else
        {
            edQtyFix1->Text=MOT[MManualTray1].Tray.HowManyIC();
        }

        if(TrayForm.bTrayUpDownSet[eFix2])
        {
            edQtyFix2->Text=MOT[MManualTray2].Tray.HowManyUpperHalfIC();
            edQtyFix5->Text=MOT[MManualTray2].Tray.HowManyLowerHalfIC();
        }
        else
        {
            edQtyFix2->Text=MOT[MManualTray2].Tray.HowManyIC();
        }

        if(TrayForm.bTrayUpDownSet[eFix3])
        {
            edQtyFix3->Text=MOT[MManualTray3].Tray.HowManyUpperHalfIC();
            edQtyFix6->Text=MOT[MManualTray3].Tray.HowManyLowerHalfIC();
        }
        else
        {
            edQtyFix3->Text=MOT[MManualTray3].Tray.HowManyIC();
        }
    }
}
//---------------------------------------------------------------------------
bool TfNote::bCheckContAlarmNotUpload(AnsiString sJamCode)                      //Sam 20231116 : 連續 Alarm 不要上傳伺服器
{
    bool bUpload=false, bFind=false;
    int iTime=0;
    AnsiString sMsg="";
    iContAlarmTime=CheckAndReadIniDataGeneral("System", "ContAlarmTime", 900);
    if(listContAlarmList->Count==0 ||
       listContAlarmTimer->Count==0)
    {
        listContAlarmList->Clear();
        listContAlarmTimer->Clear();
    }

    TQPF_Timer *temTimer;

    if(listContAlarmList->Count==0)
    {
        bFind=false;
    }
    else
    {
        bFind=false;
        for(int i=0; i<listContAlarmList->Count; i++)
        {
            if(listContAlarmList->Strings[i]==sJamCode)
            {
                bFind=true;
                temTimer=(TQPF_Timer *)listContAlarmTimer->Items[i];
                if(temTimer->Off())
                {
                    bUpload=true;                                               //超過時間要上傳
                }
                else
                {
                    iTime=temTimer->LatchCycleTimeSec();
                    sMsg.sprintf("Continuous %s not uploading to server. reset timer", sJamCode, iTime);
                    NewRecordProcess("", sMsg, IntToStr(iTime));
                    bUpload=false;                                              //沒有超過時間再重複發生要重新計數
                }
                temTimer->LatchCycleTimeSec(true);
                temTimer->SetSecAndOn(iContAlarmTime);
            }
        }
    }

    if(bFind==false)                                                            //新增新 Alarm 計時
    {
        bUpload=true;
        listContAlarmList->Add(sJamCode);
        listContAlarmTimer->Add(new TQPF_Timer());
        temTimer=(TQPF_Timer *)listContAlarmTimer->Items[listContAlarmTimer->Count-1];
        temTimer->LatchCycleTimeSec(true);
        temTimer->SetSecAndOn(iContAlarmTime);
    }

    if(listContAlarmTimer->Count!=0)                                            //Sam 20240506 : 修正溢位異常問題
    {
        for(int i=listContAlarmTimer->Count-1; i>=0; i--)
        {
            temTimer=(TQPF_Timer *)listContAlarmTimer->Items[i];
            if(temTimer->Off())
            {
                delete temTimer;
                listContAlarmList->Delete(i);
                listContAlarmTimer->Delete(i);
            }
        }
    }
    return bUpload;
}
//---------------------------------------------------------------------------
void  TfNote::ClearContAlarmList()                                              //Sam 20231116 : 連續 Alarm 不要上傳伺服器
{
    if(CosFunction.bOLPFunction==false)
        return;

   TQPF_Timer *temTimer;
    if(listContAlarmTimer->Count!=0)
    {
        for(int i=listContAlarmTimer->Count-1; i>=0; i--)                       //Sam 20240506 : 修正溢位異常問題
        {
            temTimer=(TQPF_Timer *)listContAlarmTimer->Items[i];
            delete temTimer;
            listContAlarmTimer->Delete(i);
        }
    }
    listContAlarmList->Clear();
    listContAlarmTimer->Clear();
}
//---------------------------------------------------------------------------
bool NoteFormShow()
{
    return fNote->fShow;
}
//---------------------------------------------------------------------------
void ShowNoteMessage_1032(AnsiString sMsg)                                      //顯示TriTemp 用的Note //Ztex 2023.04.19 Add HT-1032 TriTemp Function <==
{
    fNote->lbl_TriMachineSpecialNote1->Caption = sMsg;
//    fNote->pnlPan_TriMachineSpecialNote->Parent=fNote->tsHandler;
    fNote->pnlPan_TriMachineSpecialNote->Visible=true;
    fNote->pnlIndex1Error->Visible=false;
    fNote->pnlPan_TriMachineSpecialNote->BringToFront();
    fNote->lbl_TriMachineSpecialNote1->Visible=true;
    fNote->lbl_TriMachineSpecialNote1->BringToFront();
    fNote->pnlMovie->Visible=false;
    fNote->pgcNote->ActivePageIndex=1;
    bShowTriTempAlarm_Msg=true;
    fNote->ShowModal();
    RecordProcess(sMsg);
}
//---------------------------------------------------------------------------
bool __fastcall TfNote::CheckBinCode()                                          //JerryYang 20240111 : add P53 function
{
    AnsiString str="", str1="";
    int iBinCntMax=17;                                                          //JerryYang 20241118 : fix

    for(int i=0; i<iTestBinCount-1; i++)
    {
        for (int j=i+1; j<=iTestBinCount; j++)                                  //JerryYang 20241118 : fix
        {
            if(myBinCodeEdit[i]->Text==myBinCodeEdit[j]->Text &&                // 如果找到相同的數值，表示有重複
               myBinCodeEdit[i]->Text!="")
            {
                return false;
            }
        }
    }

    for(int i=0; i<=iTestBinCount; i++)
    {
        if(myBinCodeEdit[i]->Visible==true)
        {
            str=myBinCodeEdit[i]->Text;

            str=str.UpperCase();

            int j=0;

            if(str.Length()>=10 &&
               (str.Pos("PASS_BIN_")>0 ||
                str.Pos("FAIL_BIN_")>0))
            {
                str1=str.SubString(10, str.Length()-10+1);

                if(str1=="E")
                {
                    j=iTestBinCount;
                }
                else
                {
                    j=StrToIntDef(str1, 0);
                }

                if(str.Pos("BIN_E")>0)
                {
                }
                else if(str.Pos("PASS_BIN_")>0)
                {
                    if(j<iBinCntMax && Prod.bIsPassBin[j]==false)               //fail
                    {
                        return false;
                    }
                }
                else
                {
                    if(j<iBinCntMax && Prod.bIsPassBin[j]==true)                //pass
                    {
                        return false;
                    }
                }
            }
            else                                                                //沒有刷條碼
            {
                return false;
            }

            if(j>=0 && j<=iTestBinCount)
            {
                if(j==iTestBinCount)
                {
                    if(((edErrorCode->Text=="MES1120" || edErrorCode->Text=="MES1124") && Prod.iIfErrorT6==eAuto1) ||   //JerryYang 20250303 : fix
                       ((edErrorCode->Text=="MES1220" || edErrorCode->Text=="MES1224") && Prod.iIfErrorT6==eAuto2) ||
                       ((edErrorCode->Text=="MES1320" || edErrorCode->Text=="MES1324") && Prod.iIfErrorT6==eAuto3) ||
                       ((edErrorCode->Text=="MES1720" || edErrorCode->Text=="MES1724" || edErrorCode->Text=="MES1712") && Prod.iIfErrorT6==eFix1) ||
                       ((edErrorCode->Text=="MES1820" || edErrorCode->Text=="MES1824" || edErrorCode->Text=="MES1812") && Prod.iIfErrorT6==eFix2) ||
                       ((edErrorCode->Text=="MES1920" || edErrorCode->Text=="MES1924" || edErrorCode->Text=="MES1912") && Prod.iIfErrorT6==eFix3))              //JerryYang 20250429 : fix Auto In/Out
                    {
                    }
                    else
                    {
                        return false;
                    }
                }
                else if(((edErrorCode->Text=="MES1120" || edErrorCode->Text=="MES1124") && Prod.iT6PosCate[j]==ePosAuto1) ||                                    //JerryYang 20250303 : fix
                        ((edErrorCode->Text=="MES1220" || edErrorCode->Text=="MES1224") && Prod.iT6PosCate[j]==ePosAuto2) ||
                        ((edErrorCode->Text=="MES1320" || edErrorCode->Text=="MES1324") && Prod.iT6PosCate[j]==ePosAuto3) ||
                        ((edErrorCode->Text=="MES1720" || edErrorCode->Text=="MES1724" || edErrorCode->Text=="MES1712") && Prod.iT6PosCate[j]==ePosFix1) ||
                        ((edErrorCode->Text=="MES1820" || edErrorCode->Text=="MES1824" || edErrorCode->Text=="MES1812") && Prod.iT6PosCate[j]==ePosFix2) ||
                        ((edErrorCode->Text=="MES1920" || edErrorCode->Text=="MES1924" || edErrorCode->Text=="MES1912") && Prod.iT6PosCate[j]==ePosFix3))       //JerryYang 20250429 : fix Auto In/Out
                {
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
    }

    if(fNote->edErrorCode->Text=="MES1124")
    {
        sStackBinTemp[0]=str;
    }
    else if(fNote->edErrorCode->Text=="MES1224")
    {
        sStackBinTemp[1]=str;
    }
    else if(fNote->edErrorCode->Text=="MES1324")
    {
        sStackBinTemp[2]=str;
    }
    else if(fNote->edErrorCode->Text=="MES1724" ||
            fNote->edErrorCode->Text=="MES1712")                                //JerryYang 20250429 : fix Auto In/Out
    {
        sStackBinTemp[3]=str;
    }
    else if(fNote->edErrorCode->Text=="MES1824" ||
            fNote->edErrorCode->Text=="MES1812")
    {
        sStackBinTemp[4]=str;
    }
    else if(fNote->edErrorCode->Text=="MES1924" ||
            fNote->edErrorCode->Text=="MES1912")
    {
        sStackBinTemp[5]=str;
    }

    return true;
}
//---------------------------------------------------------------------------
void __fastcall TfNote::tmrKeyInTimer(TObject *Sender)
{
    if(!fShow)
        return;
    int iBinCntMax=17;                                                          //JerryYang 20241118 : fix

    for(int i=0; i<=iBinCntMax; i++)
    {
        if(myBinCodeEdit[i]->Text.Length()>0 &&
           myBinCodeEdit[i]->Text.Length()<10 &&
           myBinCodeEdit[i]->Focused())                                         //Rogeryang 20250613 判斷是否擁有游標
        {
            myBinCodeEdit[i]->Text="";
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfNote::FormShortCut(TWMKey &Msg, bool &Handled)
{
    if(Msg.CharCode==VK_F4)                                                     //F4 關閉視窗
    {                                                                           //Sam 20230620 : 面板已經鎖定需要 IT 下命令解鎖
        MyDBIProcess("Message","Manual Unlock TCP Lock Password!!");
        bNeedTCPAlarm=false;
        bNeedPassWord=false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfNote::timerKeyenceTimer(TObject *Sender)
{
    static int Pos=0;
    static bool bTimerIn=false;

    if(bTimerIn)
    {
        return;
    }
    bTimerIn=true;

    if(bScanBinLabel)                                                           //JerryYang 20240821 : 自動讀取BIN LABEL
    {
        switch(iScanTask)
        {
            case 1:
                if(fAGV->Use_AMR() || ShouldAutoScanBinLabelToEdit())                                             //Steven 20251216 : for AMR
                {
                    if(edErrorCode->Text=="MES1120" || edErrorCode->Text=="MES1124")
                    {
                        InitialCoverTrayIDTask(iKeyenceCoverTrayID_AUTO1);
                        Pos=iKeyenceCoverTrayID_AUTO1;
                        bAutoIsScanBinLabel[0]=true;                            //JerryYang 20250429 : fix Auto In/Out
                        iScanTask=100;
                    }
                    else if(edErrorCode->Text=="MES1220" || edErrorCode->Text=="MES1224")
                    {
                        InitialCoverTrayIDTask(iKeyenceCoverTrayID_AUTO2);
                        Pos=iKeyenceCoverTrayID_AUTO2;
                        iScanTask=100;
                        bAutoIsScanBinLabel[1]=true;
                    }
                    else if(edErrorCode->Text=="MES1320" || edErrorCode->Text=="MES1324")
                    {
                        InitialCoverTrayIDTask(iKeyenceCoverTrayID_AUTO3);
                        Pos=iKeyenceCoverTrayID_AUTO3;
                        iScanTask=100;
                        bAutoIsScanBinLabel[2]=true;
                    }
                }
                break;
            case 100:                                                           //JerryYang 20240318
                if(fTrayMapping->DoCoverTrayID(Pos, true)==true)
                {
                    iScanTask=1;
                    bScanBinLabel=false;
                    bAutoIsScanBinLabel[0]=false;                               //JerryYang 20250429 : fix Auto In/Out
                    bAutoIsScanBinLabel[1]=false;
                    bAutoIsScanBinLabel[2]=false;
                }
        }
    }
    else
    {
        iScanTask=1;
    }

    bTimerIn=false;
}
//---------------------------------------------------------------------------
AnsiString TfNote::PosCodeToStr(int Pos)
{
    AnsiString sRet="Unknown Area";
    if(     Pos<=MInArmZH)                      sRet="In Arm";
    else if(Pos==MInShuttle1)                   sRet="In Shuttle1";
    else if(Pos==MInShuttle2)                   sRet="In Shuttle2";
    else if(Pos==MTestY1 || Pos==MTestZ1)       sRet="Index Arm";
    else if(Pos==MTestY2 || Pos==MTestZ2)       sRet="Index Arm";
    else if(Pos==MOutShuttle1)                  sRet="Out Shuttle1";
    else if(Pos==MOutShuttle2)                  sRet="Out Shuttle2";
    else if(Pos>=MOutArmX && Pos<=MOutArmZH)    sRet="Out Arm";
    else if(Pos==MTrayX)                        sRet="TrayX Car";
    else if(Pos==MMTrayZ  || Pos==MMTrayY_Car)  sRet="TrayZ Car";
    else if(Pos==MMEmptyZ || Pos==MMEmpty_Car)  sRet="Empty Car";
    else if(Pos==MMEmpty1_Car)                  sRet="Empty1 Car";
    else if(Pos==MMColorZ || Pos==MMColor_Car)  sRet="Color Car";
    else if(Pos==MMAuto1Z || Pos==MMAuto1_Car)  sRet="Auto1 Car";
    else if(Pos==MMAuto2Z || Pos==MMAuto2_Car)  sRet="Auto2 Car";
    else if(Pos==MMAuto3Z || Pos==MMAuto3_Car)  sRet="Auto3 Car";
    else if(Pos==MMAuto4Z || Pos==MMAuto4_Car)  sRet="Auto4 Car";
    else if(Pos==MMAuto5Z || Pos==MMAuto5_Car)  sRet="Auto5 Car";
    else if(Pos==MMAuto6Z || Pos==MMAuto6_Car)  sRet="Auto6 Car";
    else if(Pos==MManualTray1)                  sRet="ManualTray1";
    else if(Pos==MManualTray2)                  sRet="ManualTray2";
    else if(Pos==MManualTray3)                  sRet="ManualTray3";
    else if(Pos==MManualTray4)                  sRet="ManualTray4";
    else if(Pos==MManualTray5)                  sRet="ManualTray5";
    else if(Pos==MManualTray6)                  sRet="ManualTray6";
    else if(Pos==MManualTrayAll)                sRet="ManualTrayAll";
    else if(Pos==MMTrayY)                       sRet="TrayY";
    else if(Pos==MMPlate1)                      sRet="Plate 1";
    else if(Pos==MMPlate2)                      sRet="Plate 2";
    else if(Pos==MMAuto1)                       sRet="Auto 1";
    else if(Pos==MMAuto2)                       sRet="Auto 2";
    else if(Pos==MMAuto3)                       sRet="Auto 3";
    else if(Pos==MMAuto4)                       sRet="Auto 4";
    else if(Pos==MMAuto5)                       sRet="Auto 5";
    else if(Pos==MMAuto6)                       sRet="Auto 6";
    else if(Pos==MMEmpty)                       sRet="Empty";
    else if(Pos==MMColor)                       sRet="Color";
    else if(Pos==MMEmpty1)                      sRet="Empty1";
    else if(Pos==MMSystem)                      sRet="System";
    else if(Pos==MMSafeDoor1)                   sRet="Safe Door1";
    else if(Pos==MMSafeDoor2)                   sRet="Safe Door2";
    else if(Pos==MMSafeDoor3)                   sRet="Safe Door3";
    else if(Pos==MMSafeDoor4)                   sRet="Safe Door4";
    else if(Pos==MMSafeDoor5)                   sRet="Safe Door5";
    else if(Pos==MMSafeDoor6)                   sRet="Safe Door6";
    else if(Pos==MMSafeDoor7)                   sRet="Safe Door7";
    else if(Pos==MMSafeDoor8)                   sRet="Safe Door8";
    else if(Pos==MMSafeDoor9)                   sRet="Safe Door9";
    else if(Pos==MMSafeDoor10)                  sRet="Safe Door10";
    else if(Pos==MMIonFan01)                    sRet="Ion Fan1";                //Steven 20130205 : 離子風扇異常顯示
    else if(Pos==MMIonFan02)                    sRet="Ion Fan2";
    else if(Pos==MMIonFan03)                    sRet="Ion Fan3";
    else if(Pos==MMIonFan04)                    sRet="Ion Fan4";
    else if(Pos==MMIonFan05)                    sRet="Ion Fan5";
    else if(Pos==MMIonFan06)                    sRet="Ion Fan6";
    else if(Pos==MMIonFan07)                    sRet="Ion Fan7";
    else if(Pos==MMIonFan08)                    sRet="Ion Fan8";
    else if(Pos==MMIonFan09)                    sRet="Ion Fan9";
    else if(Pos==MMIonFan10)                    sRet="Ion Fan10";
    else if(Pos==MMIonFan11)                    sRet="Ion Fan11";
    else if(Pos==MMIonFan12)                    sRet="Ion Fan12";
    else if(Pos==MMATC_Handler)                 sRet="ATC Handler";
    else if(Pos==MMATC_TCPIP)                   sRet="ATC TCPIP";
    else if(Pos==MMATC_NI)                      sRet="ATC NI";
    else if(Pos==MMATC_ATC)                     sRet="ATC";
    else if(Pos==MMATC_Chiller)                 sRet="ATC Chiller";
    else if(Pos==MMATC_RS232)                   sRet="ATC RS232";
    else if(Pos==MMATC_Head)                    sRet="ATC Head";
    else if(Pos==MMATC_PowerSupply)             sRet="ATC PowerSupply";
    else if(Pos==MMATC_WaterValve)              sRet="ATC WaterValve";
    else if(Pos==MMInShuttle)                   sRet="In Shuttle";
    else if(Pos==MMOutShuttle)                  sRet="Out Shuttle";
    else if(Pos==MMIndex)                       sRet="Index";
    else if(Pos==MMTemperature)                 sRet="Temperature";
    else if(Pos==MMInterface)                   sRet="Interface";
    else if(Pos==MMScanner)                     sRet="Scanner";
    else if(Pos==MMCCD)                         sRet="CCD";
    else if(Pos==MMOCR)                         sRet="OCR";                     //ChungHung 20120830 add OCR Function add
    return sRet;
}
//---------------------------------------------------------------------------
void __fastcall TfNote::edBinCodeEditKeyPress(TObject *Sender, char &Key)
{
    TEdit *Buffer;                                                              //JerryYang 20241118 : fix
    Buffer=(TEdit *)Sender;

    if(Buffer->Text.Length()<=1)
    {
        tmrKeyIn->Enabled=true;
    }

    if(Key=='\r')                                                               //JerryYang 20251022 : 優化BIN LABEL刷條碼
    {
        if(Buffer->Text.Length()<10)
        {
            Buffer->Text="";
        }
        else
        {
            tmrKeyIn->Enabled=false;
            Buffer->Enabled=false;
            for(int i=0; i<=iTestBinCount; i++)
            {
                if(myBinCodeEdit[i]->Enabled==true &&
                   myBinCodeEdit[i]->Visible==true &&
                   myBinCodeEdit[i]->Text=="")
                {
                    myBinCodeEdit[i]->SetFocus();
                    break;
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfNote::edBundleIDChange(TObject *Sender)
{
    TDateTime NowTime = Now();

    if(edBundleID->Text.Length()<=1)                                            // 一開始輸入第一個字元，不處理
    {
        LastInputTime=Now();
        return;
    }

    double Interval=(NowTime.Val-LastInputTime.Val)*24*60*60*1000.0;            // 毫秒
    LastInputTime=NowTime;

    if(Interval>100)                                                            // 超過100毫秒才輸入下一個字元，當作人工輸入
    {
        IsScanning=false;
    }
    else                                                                        // 小於100毫秒，推測為掃描槍輸入
    {
        IsScanning=true;
    }

    if(IsScanning==false)
    {
        edBundleID->Text="";
    }
}
//---------------------------------------------------------------------------
void __fastcall TfNote::edBundleIDKeyPress(TObject *Sender, char &Key)
{
    if(IniConfig.bA65_BundleIDList &&                                           //JerryYang 20250430 : 矽品文棋要求修改
       IniConfig.bP53_ForcedScanBinCodeOfUnloader==false &&
       TrayForm.bSpecTrayCnt==true)
    {
        if(Key=='\r')
        {
            NewRecordProcess("MES2111", "PAUSE pressed", "Note_edBundleIDKeyPress");
            BtnSkipClick(BtnRetry);
            BtnPauseClick(this);
        }
    }
}
//---------------------------------------------------------------------------
void TfNote::SaveErrEventLog(double dt, AnsiString sJamArea, AnsiString sJamCode, AnsiString sRecovery, AnsiString sPassTime, AnsiString sDuplicateError,
                             AnsiString sMessage, AnsiString sErrPart, AnsiString sRecipeName)
{
    Word y, m, d, h, n, s, ms;
    DecodeDate(dt, y, m, d);
    DecodeTime(dt, h, n, s, ms);
    AnsiString Str="";
    TStringList *SL;                                                            //Steven 20161115 : EventLog存成文字檔
    SL=new TStringList();

    if(IniConfig.bSPILFunction==true)                                           //Steven 20240604 : SPIL格式的event log
    {
        Str.sprintf("%04d-%02d-%02d %02d:%02d:%02d", y, m, d, h, n, s);
        SL->Add(sJamArea);                                                      //UnitName
        SL->Add(sJamCode);                                                      //AlarmCode
        SL->Add(Str);                                                           //OccurDateTime
        SL->Add(sRecovery);                                                     //Recovery
        SL->Add(sPassTime);                                                     //StopedTime
        SL->Add(sDuplicateError);                                               //Duplicate
        SL->Add(sMessage);                                                      //Message
        SL->Add(sErrPart);
        Str=SL->CommaText;                                                      //ErrPart
    }
    else if(CUSTOMER_CODE==CC_FOREHOPE_NINGBO)
    {   //AI(ht9045-v899) 20260429: 甬矽要求 EventLog 時間欄不加雙引號與前綴空格 (CASE-20260429-001)
        Str=AnsiString().sprintf("%04d-%02d-%02d", y, m, d);
        Str+=","+AnsiString().sprintf("%02d:%02d:%02d.%03d", h, n, s, ms);
        Str+=","+sJamArea;
        Str+=","+sJamCode;
        Str+=","+sRecovery;
        Str+=","+sPassTime;
        Str+=","+sDuplicateError;
        Str+=","+sMessage;
        Str+=","+sErrPart;
        Str+=","+sRecipeName;
    }
    else //if(CUSTOMER_CODE==CC_FOREHOPE_NINGBO)                                //Steven 20251107 : 修正event log存檔
    {   //AI(ht9045-v899) 20260429: 其他客戶維持 Steven 20251107 新方法 (CASE-20260429-001)
        Str=AnsiString().sprintf("%04d-%02d-%02d", y, m, d);
        Str+=",\""+AnsiString().sprintf(" %02d:%02d:%02d.%03d", h, n, s, ms)+"\"";
        Str+=",\""+sJamArea+"\"";
        Str+=","+sJamCode;
        Str+=","+sRecovery;
        Str+=","+sPassTime;
        Str+=","+sDuplicateError;
        Str+=",\""+sMessage+"\"";
        Str+=",\""+sErrPart+"\"";
        Str+=","+sRecipeName;
    }
//    else
//    {
//        Str.sprintf("%04d-%02d-%02d", y, m, d);
//        SL->Add(Str);
//        Str.sprintf("%02d:%02d:%02d.%03d", h, n, s, ms);
//        SL->Add(Str);
//        SL->Add(sJamArea);
//        SL->Add(sJamCode);
//        SL->Add(sRecovery);
//        SL->Add(sPassTime);
//        SL->Add(sDuplicateError);
//        SL->Add(sMessage);
//        SL->Add(sErrPart);
//        SL->Add(sRecipeName);
//        Str=SL->CommaText;
//    }
    SaveEventLog();

//    try                                                                         //KaiChen 20200706 ：增加 Try Catch 抓問題
//    {
//        if(IniConfig.bSPILFunction==true)                                       //Steven 20240604 : SPIL格式的event log
//            slEventLog->AddTextWithLineNo(Str);
//        else
//            slEventLog->AddText(Str);
//    }
//    catch(...)
//    {
//        SaveTryCatchLog(Str, "ShowErrorMessage_AddText");
//    }

    if(CUSTOMER_CODE==CC_FOREHOPE_NINGBO)
        slEventLog->AddText(Str);
    else if(IniConfig.bSPILFunction==true)
        slEventLog->MyInsertToFile(SL->CommaText, fNote->iAlarmLine);           //Steven 20191016 : 紀錄目前Alarm在檔案裡面的行數
    else
        slEventLog->MyInsertToFile(Str, fNote->iAlarmLine);                     //Steven 20191016 : 紀錄目前Alarm在檔案裡面的行數

    if(CUSTOMER_CODE==CC_PTI &&
       (fNote->AlarmType==1 ||                                                  //RogerYang 20170405 (Steven) : Jam Alarm Log
        fSecurity->GetAddAlarmLog(sJamArea, sJamCode)))                         //Sam 20210611 : Alarm Log 可以自定義哪個需要記 Log 上報
    {
        if(LastSet.iTemperature==Tempture_Ambient)                              //RogerYang 20170417 (wei) 沒設定加熱就不寫入溫度設定值
            SL->Add("Disable");
        else
            SL->Add(fMain->edWorkTemperBase->Text);
        SL->Add(fLotInfo->edtSysLotID->Text);
        SL->Add(fLotInfo->edtSysOperatorID->Text);
        SL->Add(fMain->cbSetupFileName->Text);
        SL->Add(fLotInfo->cbRunMode->Text);
        fMain->slJamAlarmLog->AddText(SL->CommaText);                           //Sam 20210611 : AddTextWithDateTime --> AddText
        fMain->slJamAlarmLog->MySaveFileByFileNameAndType(IniConfig.asN06_FileName, fLotInfo->edtSysLotID->Text, "JAM");                                        //Sam 20171215 (wei) : 檔名尾巴增加JAM/INPUT //Sam 20170428 (wei) 增加帶入 LotID
    }
    SL->Clear();                                                                //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete SL;
}
//---------------------------------------------------------------------------
void TfNote::ShowUnloaderChkMsg(int iStep)                                      //Sam 20250415 : Unloader 偵測到置偏 IC 退出後再報警
{
    AnsiString sCh="",  sEn="";

    if(iStep==3)
    {
        lblCleanSocketEN->Left=192;
        lblCleanSocketEN->Width=585;
        lblCleanSocketCH->Left=231;
        lblCleanSocketCH->Width=522;
        sEn.sprintf("Please move all Trays on Auto%d", iICFloattingCheckPos+1);
        lblCleanSocketEN->Caption=sEn;
        sCh.sprintf("請搬移 Auto%d 上所有 Tray", iICFloattingCheckPos+1);
        lblCleanSocketCH->Caption=sCh;
        pnlContactOver->Parent=tsHandler;
        pnlContactOver->BringToFront();
    }
    else if(iStep==2)
    {
        lblCleanSocketEN->Left=104;
        lblCleanSocketEN->Width=585;
        lblCleanSocketCH->Left=179;
        lblCleanSocketCH->Width=522;
        sEn.sprintf("Please check the Tray IC floatting on the Auto%d track", iICFloattingCheckPos+1);
        lblCleanSocketEN->Caption=sEn;
        sCh.sprintf("請檢查 Auto%d 軌道上的 Tray IC 置偏", iICFloattingCheckPos+1);
        lblCleanSocketCH->Caption=sCh;
        pnlContactOver->Parent=tsHandler;
        pnlContactOver->BringToFront();
    }
    else if(iStep==1)
    {
        lblCleanSocketEN->Left=192;
        lblCleanSocketEN->Width=585;
        lblCleanSocketCH->Left=231;
        lblCleanSocketCH->Width=522;
        sEn.sprintf("Please move back all Trays on Auto%d", iICFloattingCheckPos+1);
        lblCleanSocketEN->Caption=sEn;
        sCh.sprintf("請搬回 Auto%d 上所有的 Tray", iICFloattingCheckPos+1);
        lblCleanSocketCH->Caption=sCh;
        pnlContactOver->Parent=tsHandler;
        pnlContactOver->BringToFront();
    }
    else
    {
        pnlContactOver->Visible=false;
    }
}
//---------------------------------------------------------------------------
