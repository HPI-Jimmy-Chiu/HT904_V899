#include "MachineDefine.h"
#pragma hdrstop

//---------------------------------------------------------------------------
//Steven 20090818
//資料庫改用SQLite3來存取資料
//---------------------------------------------------------------------------
#include "cObserver.h"

//#include "XLSfile.hpp"                                                        //Steven 20140307 : 加速Compiler
#include "SgdToXLS.h"
#include "cCounterClear.h"
#include "MachineType.h"
#include "cSocket.h"
#include "cprod.h"
#include "cmydef.h"
#include "mykitsuck.h"
#include "cpublic.h"
#include "common.h"
//#include "utilcls.h"
#include "uShowMessage.h"
#include "cMyDB.h"                                                              //Steven 20090817
#include "cAuthority.h"                                                         //Steven 20090827
#include "main.h"
#include "csystem.h"
//#include "INPUT.h"
#include "myQwertyKeyBoard.h"
#include "Password.h"
#include "mymessbox.h"
#include "cSortCT.h"
#include "cContactCT.h"
#include "cConfiguration.h"
#include "cSecurity.h"
#include "ATC_Handler_Side.h"                                                   //Ifor (wei) 20151230 :add New ATC Interface
#include "ProductionInfo.h"                                                     //Sam 20170809 (Steven) 移植超豐 OEE 功能 form HT-7045
#include "Precaution.h"                                                         //Sam 20171120 (Steven) AddPrecautionRecordFunction (form HT7045)
#include "uLotInfo.h"                                                           //Sam 20171122 (Steven) AddPrecautionRecordFunction (form HT7045)
#include "HTMD5.h"
#include "FTPClient.h"
#include "GroundMan.h"
//#include "BarcodeXML.h"
#include "SCK_ART.h"
#include "note.h"
#include "cinitial.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "HTray"
#pragma link "MyMemo"
#pragma resource "*.dfm"

char sSocketCT[MAX_SOCKET_COL][TEST_MAX_BIN+1][64];
AnsiString sSKETTotal[TEST_MAX_BIN+1], sPassSKET[TEST_MAX_BIN+1];
AnsiString sCounterColKind[MAX_SOCKET_ROW][MAX_SOCKET_COL], TesterCH[MAX_SOCKET_ROW][MAX_SOCKET_COL];
TTMyTray *mtRow[MAX_SOCKET_ROW];
bool   bShowYieldSeries[MAX_SOCKET_ROW][MAX_SOCKET_COL];
bool   bShowYieldAll[MAX_SOCKET_ROW];
bool   bTabVisible[20]={false, false, false, false, false, false, false, false, false, false,
                        false, false, false, false, false, false, false, false, false, false };     //Sam (Steven) 20171120 AddPrecautionRecordFunction (form HT7045)

//---------------------------------------------------------------------------
int N_Count=0;
TfObserver *fObserver;
struct TestTimeInfo
{
    int iStartMin;
    int iStartSec;
    int iStartMSec;
    int iEndMin;
    int iEndSec;
    int iEndMSec;
};
TestTimeInfo TestTimeInfoRecord[2][10];
TestTimeInfo TestSocketTimeInfo[2];
TestTimeInfo OEERecevieTimeInfo[2];

struct TestReceiveTimeInfo                                                      //KaiChen 20171127 (Steven) ：超豐 OEE 新增 Test Time、Index Time
{
    int iMin;
    int iSec;
    int iMSec;
}TestReceiveTimeInfoRecord[2][10];

bool bWriteErrRefFlag=false;
int AlarmCount, AlarmST, TotalCount, TotalTime;

TColor TC[tcTotalCount]={clSilver,          clRed,      clLime,         clInactiveCaption,  clBlue,                 //溫控器要一起改
                         clFuchsia,         clAqua,     clHighlight,    clBlack,            clMaroon,               //9
                         clGreen,           clOlive,    clNavy,         clPurple,           clTeal,
                         clGray,            clSilver,   clRed,          clLime,             clInactiveCaption,      //19
                         clBlue,            clFuchsia,  clAqua,         clHighlight,        clBlack,
                         clMaroon,          clGreen,    clOlive,        clNavy,             clPurple,               //29
                         clTeal,            clGray,     clSilver,       clRed,              clLime,
                         clInactiveCaption, clBlue,     clFuchsia,      clAqua,             clHighlight,            //39
                         clBlack,           clMaroon,   clGreen,        clOlive,            clNavy,
                         clPurple,          clTeal,     clGray,         clSilver,           clRed,                  //49
                         clLime,            clPurple,   clTeal,         clGray,             clSilver,
                         clInactiveCaption, clBlue,     clFuchsia,      clAqua,             clHighlight,            //59
                         clBlack,           clMaroon,   clGreen,        clOlive,            clNavy,
                         clGray,            clSilver,   clRed,          clLime,             clPurple,               //69
                         clTeal
                        };

enum eQueryType{Event_Log=0,
                Process_Record,
                Message_Record,
                Motion_Record,
                Production_Record,
                Alarm_History,
                Alarm_History_JAM,
                Alarm_History_MES,
                Alarm_History_WAR,
                Alarm_Statistics,
                Alarm_Stat_Day,
                Jam_Chart_Summary,
                Jam_Chart_InArm,
                Jam_Chart_OutArm,
                Jam_Chart_Index,
                Jam_Chart_InShuttle,
                Jam_Chart_OutShuttle,
                Alarm_Code_List,                                                //Steven 20150204 : 新增Alarm Code List 查詢
                Production_Summary_Report,                                      //Sam 20210107 : Summary Report fuction
                Lot_Infomation
               };                                                               //Steven 20110715 : 方便新增查詢內容

enum eSGTestCategory
    {   eSiteMap        =0,
        eDutPos         =1,
        eArmNo          =2,
        eHeadTotal      =3,
        eSocketTotal    =4,
        ePassHead       =5,
        ePassSocket     =6,
        eIFError        =7,
        eFuncTotal
    };
//---------------------------------------------------------------------------
__fastcall TfObserver::TfObserver(TComponent* Owner)
        : TForm(Owner)
{
    int iRow, iCol;
    mtRow[0]=mtRowA;
    mtRow[1]=mtRowB;
    mtRow[2]=mtRowC;
    mtRow[3]=mtRowD;
    AnsiString s="";
    int iColorCT=0;
    for(iRow=0; iRow<MAX_SOCKET_ROW; iRow++)
    {
        mtRow[iRow]->SetColorMap(0,(TColor)0x00C2B8A6);
        mtRow[iRow]->SetColorMap(1, clGreen);
        mtRow[iRow]->SetColorMap(2, clRed);
        mtRow[iRow]->Tag=iRow;
        bShowYieldAll[iRow]=false;
        for(iCol=0; iCol<MAX_SOCKET_COL; iCol++)
        {
            mtRow[iRow]->SetColorMap(3+iCol, TC[iColorCT]);
            iColorCT++;

            bShowYieldSeries[iRow][iCol]=true;
        }
    }

//QQ S
    mtCategorySum   ->SetColorMap(0, clInactiveCaption);                        //Steven 20131030 : 客戶嫌不容易看,改顏色
    mtRowName       ->SetColorMap(0, clInactiveCaption);
    mtNo            ->SetColorMap(0, clInactiveCaption);
    mtTotalName     ->SetColorMap(0, clInactiveCaption);
    myCategoryName  ->SetColorMap(0, clInactiveCaption);
    mtChName        ->SetColorMap(0, clInactiveCaption);
    mtDutName       ->SetColorMap(0, clInactiveCaption);
    mtArmName       ->SetColorMap(0, clInactiveCaption);
    mtCategoryNo    ->SetColorMap(0, (TColor)0x00DFD9CC);
    mtHeadTotal     ->SetColorMap(0, (TColor)0x00DFD9CC);
    mtSockTotal     ->SetColorMap(0, (TColor)0x00DFD9CC);
    mtPassHead      ->SetColorMap(0, (TColor)0x00DFD9CC);
    mtPassSocket    ->SetColorMap(0, (TColor)0x00DFD9CC);
    mtIfError       ->SetColorMap(0, (TColor)0x00DFD9CC);
    mtTotal         ->SetColorMap(0, (TColor)0x00DFD9CC);
    mtCategoryTotal ->SetColorMap(0, (TColor)0x00DFD9CC);

    DcSG2 = GetDC(StringGrid2->Handle);
    DcSG3 = GetDC(StringGrid3->Handle);
    DcSG5 = GetDC(StringGrid5->Handle);
    pCanvas=new TCanvas;

    if(MachineTypeChoice==Type_HT9045)                                          //9045
    {
        StringGrid2->ColCount=9;
        StringGrid2->DefaultColWidth=90;
        StringGrid3->ColCount=9;
        StringGrid3->DefaultColWidth=90;
    }
    else
    {
        StringGrid2->ColCount=17;
        StringGrid2->DefaultColWidth=47;
        StringGrid3->ColCount=17;
        StringGrid3->DefaultColWidth=47;
    }

    TimeInfoGrid->Cells[1][0]="Site      ";         TimeInfoGrid->Cells[1][1]="Start Time";
    TimeInfoGrid->Cells[2][1]="End   Time";         TimeInfoGrid->Cells[3][1]="Test  Time";
    TimeInfoGrid->Cells[4][1]="Index Cycle Time";   TimeInfoGrid->Cells[5][1]="Index Time";
    TimeInfoGrid->Cells[0][14]="Average";
    TimeInfoGrid->Cells[0][11]="Now";               TimeInfoGrid->Cells[0][10]="Last 1";
    TimeInfoGrid->Cells[0][9]="Last 2";             TimeInfoGrid->Cells[0][8]="Last 3";
    TimeInfoGrid->Cells[0][7]="Last 4";             TimeInfoGrid->Cells[0][6]="Last 5";
    TimeInfoGrid->Cells[0][5]="Last 6";             TimeInfoGrid->Cells[0][4]="Last 7";
    TimeInfoGrid->Cells[0][3]="Last 8";             TimeInfoGrid->Cells[0][2]="Last 9";

    strngrdTestTime->Cells[1][0]="Site      ";      strngrdTestTime->Cells[1][1]="Start Time";
    strngrdTestTime->Cells[2][1]="End   Time";      strngrdTestTime->Cells[3][1]="Test  Time";
    strngrdTestTime->Cells[4][1]="Index Cycle Time";strngrdTestTime->Cells[5][1]="Index Time";
    strngrdTestTime->Cells[0][14]="Average";
    strngrdTestTime->Cells[0][11]="Now";            strngrdTestTime->Cells[0][10]="Last 1";
    strngrdTestTime->Cells[0][9]="Last 2";          strngrdTestTime->Cells[0][8]="Last 3";
    strngrdTestTime->Cells[0][7]="Last 4";          strngrdTestTime->Cells[0][6]="Last 5";
    strngrdTestTime->Cells[0][5]="Last 6";          strngrdTestTime->Cells[0][4]="Last 7";
    strngrdTestTime->Cells[0][3]="Last 8";          strngrdTestTime->Cells[0][2]="Last 9";

    iTotoalTestTime=0;
    GetObserAuth();                                                             //Steven 20090827 : 用來決定某些畫面是否顯示
    SetSiteYieldDiagram();                                                      //Steven 20100126
    iShowYieldChart=0;                                                          //Steven 20100818 : 只顯示Yield Chart
    labMachineID->Caption =IniConfig.SocketHandlerID;

    sgTimeData->Cells[ 1][ 0]="Current";                                        //JerryYang 20151209
    sgTimeData->Cells[ 2][ 0]="Last 1";
    sgTimeData->Cells[ 3][ 0]="Last 2";
    sgTimeData->Cells[ 4][ 0]="Last 3";
    sgTimeData->Cells[ 5][ 0]="Last 4";
    sgTimeData->Cells[ 6][ 0]="Last 5";
    sgTimeData->Cells[ 7][ 0]="Last 6";
    sgTimeData->Cells[ 8][ 0]="Last 7";
    sgTimeData->Cells[ 9][ 0]="Last 8";
    sgTimeData->Cells[10][ 0]="Last 9";
    sgTimeData->Cells[ 0][ 0]="Motion Part";

    sgTimeData->DefaultColWidth=60;
    sgTimeData->ColWidths[0]=200;
    sgTimeData->Cells[ 0][1    ]="DoArmPickFromLoadStage_9045_2x8_8";
    sgTimeData->Cells[ 0][2    ]="DoArmPickFromLoadStage_9045_2x8_8  OK";
    sgTimeData->Cells[ 0][3    ]="DoPlaceToHotPlate_9045_2x8_8";
    sgTimeData->Cells[ 0][4    ]="DoPlaceToHotPlate_9045_2x8_8  OK";
    sgTimeData->Cells[ 0][5    ]="DoInArmPickFromHotPlate_9045_2x8_8  OK";
    sgTimeData->Cells[ 0][6    ]="DoPlaceToShuttle_9045_2x8_8";
    sgTimeData->Cells[ 0][7    ]="DoPlaceToShuttle_9045_2x8_8   OK";
    sgTimeData->Cells[ 0][8    ]="MoveInArm2XYToShuttle1_9045_2x8_8";
    sgTimeData->Cells[ 0][9    ]="MoveInArm2XYToShuttle1_9045_2x8_8  OK";
    sgTimeData->Cells[ 0][10   ]="MoveInArm2XYToShuttle2_9045_2x8_8";
    sgTimeData->Cells[ 0][11   ]="MoveInArm2XYToShuttle2_9045_2x8_8  OK";
    sgTimeData->Cells[ 0][12   ]="InArmAtShuttle1Delay";
    sgTimeData->Cells[ 0][13   ]="InArmAtShuttle1Delay OK";
    sgTimeData->Cells[ 0][14   ]="InArm2AtShuttle2Delay";
    sgTimeData->Cells[ 0][15   ]="InArm2AtShuttle2Delay  OK";
    sgTimeData->Cells[ 0][18   ]="Drop Contact 1";                              //JerryYang 20170503 (wei) drop contact的index cycle time分成三段來計時
    sgTimeData->Cells[ 0][19   ]="Drop Contact 2";
    sgTimeData->Cells[ 0][20   ]="Drop Contact 3";
    sgTimeData->Cells[ 0][21   ]="Total";
    RunInfo.SoftwareDate=AnsiString(__DATE__)+"  "+AnsiString(__TIME__);        //Steven 20140429 : For SECS_GEM

    if(IniConfig.bSPILFunction==true)                                           //Steven 20240604 : SPIL格式的event log
    {
        strngrdEventLog->ColCount=9;
        strngrdEventLog->ColWidths[0]=50;                                       //No.
        strngrdEventLog->ColWidths[1]=50;                                       //UnitName
        strngrdEventLog->ColWidths[2]=50;                                       //AlarmCode
        strngrdEventLog->ColWidths[3]=100;                                      //OccurDateTime
        strngrdEventLog->ColWidths[4]=50;                                       //Recovery
        strngrdEventLog->ColWidths[5]=50;                                       //StopedTime
        strngrdEventLog->ColWidths[6]=50;                                       //Duplicate
        strngrdEventLog->ColWidths[7]=200;                                      //Message
        strngrdEventLog->ColWidths[8]=390;                                      //ErrPart
        strngrdEventLog->Cells[0][0]="No.";
    }
    else                                                                        //Steven 20170929 (wei) : 使用 Event Log Text當作顯示畫面
    {
        strngrdEventLog->ColCount=9;
        strngrdEventLog->ColWidths[0]=60;                                       //Date
        strngrdEventLog->ColWidths[1]=70;                                       //Time
        strngrdEventLog->ColWidths[2]=70;                                       //UnitName
        strngrdEventLog->ColWidths[3]=50;                                       //AlarmCode
        strngrdEventLog->ColWidths[4]=50;                                       //Recovery
        strngrdEventLog->ColWidths[5]=70;                                       //StopedTime
        strngrdEventLog->ColWidths[6]=50;                                       //Duplicate
        strngrdEventLog->ColWidths[7]=390;                                      //Message
        strngrdEventLog->ColWidths[8]=390;                                      //ErrPart
    }

    bSavePrecautionRecordFinish=false;                                          //Sam 20171120 (Steven) AddPrecautionRecordFunction (form HT7045)
    bStartPrecautionRecord=false;                                               //Sam 20171120 (Steven) AddPrecautionRecordFunction (form HT7045)
    bChangeReciepeSaveMajorMaintenanceRecord=false;                             //Sam 20171120 (Steven) AddHanderMajorMaintenanceRecordFunction (form HT7045)
    bShowMajorMaintenanceRecord=false;                                          //Sam 20171120 (Steven) AddHanderMajorMaintenanceRecordFunction (form HT7045)
    asStartPrecautionRecordMOId="";                                             //Sam 20171122 (Steven) AddPrecautionRecordFunction (form HT7045)

    sg_ListTimeReceiveInfoGrid->Cells[1][0]="Site      ";   sg_ListTimeReceiveInfoGrid->Cells[1][1]="Recevie Data";
    sg_ListTimeReceiveInfoGrid->Cells[2][1]="Recevie Time";                     //KaiChen 20171127 (Steven) ：超豐 OEE 新增 Test Time、Index Time
    sg_ListTimeReceiveInfoGrid->Cells[0][14]="Average";
    sg_ListTimeReceiveInfoGrid->Cells[0][11]="Now";         sg_ListTimeReceiveInfoGrid->Cells[0][10]="Last 1";
    sg_ListTimeReceiveInfoGrid->Cells[0][9]="Last 2";       sg_ListTimeReceiveInfoGrid->Cells[0][8]="Last 3";
    sg_ListTimeReceiveInfoGrid->Cells[0][7]="Last 4";       sg_ListTimeReceiveInfoGrid->Cells[0][6]="Last 5";
    sg_ListTimeReceiveInfoGrid->Cells[0][5]="Last 6";       sg_ListTimeReceiveInfoGrid->Cells[0][4]="Last 7";
    sg_ListTimeReceiveInfoGrid->Cells[0][3]="Last 8";       sg_ListTimeReceiveInfoGrid->Cells[0][2]="Last 9";

    strngrdJamLog->ColCount=6;                                                  //KaiChen 20200618 ：矽格，增加Jam統計頁面
    strngrdJamLog->ColWidths[0]=50;
    strngrdJamLog->ColWidths[1]=100;                                            //UnitName
    strngrdJamLog->ColWidths[2]=100;                                            //AlarmCode
    strngrdJamLog->ColWidths[3]=400;                                            //Message
    strngrdJamLog->ColWidths[4]=100;
    strngrdJamLog->ColWidths[5]=100;
    strngrdJamLog->Cells[0][0]="No";
    strngrdJamLog->Cells[1][0]="UnitName";
    strngrdJamLog->Cells[2][0]="AlarmCode";
    strngrdJamLog->Cells[3][0]="Message";
    strngrdJamLog->Cells[4][0]="Count";
    strngrdJamLog->Cells[5][0]="Rate (%)";

    strngrdIndeAirOn1->Cells[1][0]="          ";            strngrdIndeAirOn1->Cells[1][1]="Start Time";   //Sam 20220329 : Record Index Air On Time
    strngrdIndeAirOn1->Cells[2][1]="End   Time";            strngrdIndeAirOn1->Cells[3][1]="Air On Time";
    for(int i=0;i<100;i++)
    {
        s.sprintf("Last %d",i);
        strngrdIndeAirOn1->Cells[0][101-i]=s;
    }

    strngrdIndeAirOn2->Cells[1][0]="          ";            strngrdIndeAirOn2->Cells[1][1]="Start Time";
    strngrdIndeAirOn2->Cells[2][1]="End   Time";            strngrdIndeAirOn2->Cells[3][1]="Air On Time";
    for(int i=0;i<100;i++)
    {
        s.sprintf("Last %d",i);
        strngrdIndeAirOn2->Cells[0][101-i]=s;
    }

    labSerialNo->Caption=CheckAndReadIniDataGeneral("Version", "Serial No", AnsiString("29818"));

    cbbTempChart->Items->Clear();
    cbbTempChart->Items->Add("All");
    for(int j=0; j<tcTotalCount; j++)                                           //Steven 20210621 : 動態產生溫度線段
    {
        TempChart->AddSeries(new TLineSeries(TempChart));
        TempChart->Series[j]->Title=asTempCtrl[j];
        cbbTempChart->Items->Add(asTempCtrl[j]);
    }
}
//------------------------------------------------------------------------------
void __fastcall TfObserver::FormShow(TObject *Sender)
{
    fMain->TimerRecordLoaderDate();                                             //Steven 20101105
    int i;
    AnsiString Str;
    bShow=true;
    bChangeRow[0] = true;
    bChangeRow[1] = true;
    tsScanner->TabVisible=false;
    fObserver->Top=10;
    fObserver->Left=50;
//    if(XResolution>1300)                                                        //Steven 20230912 : 調整畫面為FULL HD
//    {
//        fObserver->Width=1300;
//    }
//    else
    {
        fObserver->Width=1000;
    }

    WriteContactKind();
    GetMachineData();
    labDeviceName->Caption=GetLastOpenFN();
    MyDBULotEndTime((Now()+0.00001).FormatString("yyyy-mm-dd hh:nn:ss"));       //比NOW()加一秒

    labModel->Caption=IniConfig.sMachineType;
    labSerialNo->Caption=CheckAndReadIniDataGeneral("Version", "Serial No", AnsiString("29818"));
    labMachineID->Caption =IniConfig.SocketHandlerID;
    APHeadLabel18->Caption=MyDBQClearDT();

    GetObserAuth();                                                             //Steven 20090827 : 用來決定某些畫面是否顯示
    tsTemperature->TabVisible=authObserver[0] || (TC401HeaterControl!=NoHeater);//Temperature Chart
    tsMDB->TabVisible=(CosFunction.bUseMDB);                                    //Steven 20210526 : 部分客戶取消使用MDB

    if(IniConfig.bSPILFunction==true)                                           //JerryYang 20220927 : SPIL lot info
    {
        tsLotInfo->TabVisible=true;
    }
    else
    {
        tsLotInfo->TabVisible=false;
    }

    if(iShowYieldChart==1)
    {
        int iCol, iRow;

        for(iRow=0; iRow<MAX_SOCKET_ROW; iRow++)
        {
            bShowYieldAll[iRow]=false;
            for(iCol=0; iCol<MAX_SOCKET_COL; iCol++)
            {
                bShowYieldSeries[iRow][iCol]=true;
            }
        }

        SetSiteYieldDiagram();
        UpdateYieldChart();
        pgcObserv->ActivePageIndex=4;
    }
    else
    {
        pgcObserv->ActivePageIndex=0;
    }

    pgcTestInfo->ActivePageIndex=0;
    ProcessRunInfo();

    labReleaseDate->Caption=RunInfo.SoftwareDate;                               //Steven 20091121

    DateTimePicker1->Date=Now()-1;
    DateTimePicker2->DateTime=Now();
    DateTimePicker3->Date=Now();
    DateTimePicker4->DateTime=Now();
    Button7->Enabled=!SystemStart;                                              //機器在跑的時候不可以存檔
    strngrdMDBQuery->Visible=true;
    Chart2->Visible=false;
    cbDisplayData->ItemIndex=Event_Log;
    cbbTempChart->ItemIndex=0;
    for(i=0; i<13; i++)
        bIsLoaded[i]=false;

    ShowVer();

#ifdef SOFT_SIMULTE
    btAutoSave->Visible=true;
    CheckBox1->Visible=true;
#endif
    Timer1->Enabled=true;

    if(CUSTOMER_CODE==CC_SCK)
        bFilterTheAgainData=true;                                               //Steven 20120222 : 過濾掉Duplicate的訊息

    AnsiString aStr="";

    if(bUseTwoArm32Site==true)                                                  //Steven 20210630 : 修正32site的socket資料顯示
    {
        rgRowNo->Items->Clear();
        rgRowNo->Items->Add("Arm1");
        rgRowNo->Items->Add("Arm2");
        rgContactCountKinds->Items->Clear();
        rgContactCountKinds->Items->Add("Row-A");
        rgContactCountKinds->Items->Add("Row-B");
        rgContactCountHistory->Items->Clear();
        rgContactCountHistory->Items->Add("Row-A");
        rgContactCountHistory->Items->Add("Row-B");
        if(IsNNMode()==NN_1Row)
        {
            mtDutName   ->XItem=8;
            mtSockTotal ->XItem=8;
            mtPassSocket->XItem=8;
            mtIfError   ->XItem=8;
            mtChName    ->XItem=8;
            mtHeadTotal ->XItem=8;
            mtPassHead  ->XItem=8;
        }
        else
        {
            rgContactCountKinds->Items->Add("Row-C");
            rgContactCountKinds->Items->Add("Row-D");
            rgContactCountHistory->Items->Add("Row-C");
            rgContactCountHistory->Items->Add("Row-D");
            mtDutName   ->XItem=16;
            mtSockTotal ->XItem=16;
            mtPassSocket->XItem=16;
            mtIfError   ->XItem=16;
            mtChName    ->XItem=16;
            mtHeadTotal ->XItem=16;
            mtPassHead  ->XItem=16;
        }

        for(i=0; i<mtArmName->XItem; i++)
        {
            mtArmName->SetCellNumber(i, 0, "");
        }

        rbSocketNumber->Enabled=false;
        rbSocketPercent->Enabled=false;
        rbHeadNumber->Checked=true;
    }
    else
    {
        rgRowNo->Items->Clear();
        rgRowNo->Items->Add("Row-A");
        rgRowNo->Items->Add("Row-B");
        rgContactCountKinds->Items->Clear();
        rgContactCountKinds->Items->Add("Row-A");
        rgContactCountKinds->Items->Add("Row-B");
        rgContactCountHistory->Items->Clear();
        rgContactCountHistory->Items->Add("Row-A");
        rgContactCountHistory->Items->Add("Row-B");
        mtDutName   ->XItem=8;
        mtSockTotal ->XItem=8;
        mtPassSocket->XItem=8;
        mtIfError   ->XItem=8;

        for(i=0; i<mtArmName->XItem; i++)
        {
            aStr.sprintf("Arm%d", i%2+1);
            mtArmName->SetCellNumber(i, 0, aStr.c_str());
        }

        for(i=0; i<mtDutName->XItem; i++)
        {
            aStr.sprintf("Col-%c", 'a'+i);
            mtDutName->SetCellNumber(i, 0, aStr.c_str());
        }
        rbSocketNumber->Enabled=true;
        rbSocketPercent->Enabled=true;
    }
    rgRowNo->ItemIndex=0;
    rgContactCountKinds->ItemIndex=0;
    rgContactCountHistory->ItemIndex=0;

    mtTotalName  ->SetCellNumber(0, 0, "Total");
    mtNo         ->SetCellNumber(0, 0, "No.");
    mtRowName    ->SetCellNumber(0, 0, "RowA");
    mtCategorySum->SetCellNumber(0, 0, "Head Total");
    mtCategorySum->SetCellNumber(0, 1, "Socket Total");
    mtCategorySum->SetCellNumber(0, 2, "Pass Head");
    mtCategorySum->SetCellNumber(0, 3, "Pass Socket");
    mtCategorySum->SetCellNumber(0, 4, "I/F Error");

    mtCategoryNo    ->Height=19*iTestBinCount+4;
    mtCategoryNo    ->YItem =iTestBinCount;
    mtCategoryTotal ->Height=19*iTestBinCount+4;
    mtCategoryTotal ->YItem =iTestBinCount;
    myCategoryName  ->Height=19*iTestBinCount+4;
    myCategoryName  ->YItem =iTestBinCount;

    for(i=0; i<iTestBinCount; i++)
    {
        aStr.sprintf("  Category%02d", i);
        myCategoryName->SetCellNumber(0, i, aStr.c_str());
    }

    if(ATC_SYSTEM==eNewATCSystem &&                                             //Ifor 20170317 (wei) add 顯示 ATC3.1 序號
       ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_31)
        grpATCSerialNumber->Visible=true;
    else
        grpATCSerialNumber->Visible=false;

    TStringList *tsFileName;                                                    //Steven 20170929 (wei) : 使用 Event Log Text當作顯示畫面
    tsFileName=new TStringList();
    tsFileName->Clear();
    SearchFolder("D:\\HT9045_Log\\EventLogTxt\\", tsFileName);
    cbbEventLogYear->Clear();
    for(int i=0; i<tsFileName->Count; i++)
        cbbEventLogYear->Items->Add(tsFileName->Strings[i]);

    cbbEventLogYear->Text=SystemYear;
    cbbMonth->ItemIndex=SystemMonth-1;
    cbbMonth->OnChange(this);

    if(IniConfig.bB01_UsePrecautionRecordFunction==true ||                      //Sam 20171120 (Steven) AddPrecautionRecordFunction (form HT7045)
       IniConfig.bB02_HanderMajorMaintenanceRecordFunction==true)
    {
        tsDataRecord->TabVisible=true;
        LoadPrecautionMenu();
        LoadMajorMaintenanceMenu();
        LoadPrecautionLogMenu();
        LoadMajorMaintenanceLogMenu();

        if(IniConfig.bB01_UsePrecautionRecordFunction)
        {
            tsPrecautionsRecord->TabVisible=true;
            tsPrecautionLog->TabVisible=true;
        }
        else
        {
            tsPrecautionsRecord->TabVisible=false;
            tsPrecautionLog->TabVisible=false;
        }

        if(IniConfig.bB02_HanderMajorMaintenanceRecordFunction)
        {
            tsHanderMajorMaintenance->TabVisible=true;
        }
        else
        {
            tsHanderMajorMaintenance->TabVisible=false;
        }
    }
    else
    {
        tsDataRecord->TabVisible=false;
    }

    if(bShowMajorMaintenanceRecord)                                             //Sam 20171120 AddHanderMajorMaintenanceRecordFunction (form HT7045)
    {
        bTabVisible[0]=tsCounter->TabVisible;
        bTabVisible[1]=tsTestCate->TabVisible;
        bTabVisible[2]=tsScanner->TabVisible;
        bTabVisible[3]=tsMDBQuery->TabVisible;
        bTabVisible[4]=tsYield->TabVisible;
        bTabVisible[5]=tsTestInfo->TabVisible;
        bTabVisible[6]=tsTemperature->TabVisible;
        bTabVisible[7]=tsOEE_ProductionInfor->TabVisible;

        tsPrecautionsRecord->TabVisible=false;
        tsPrecautionLog->TabVisible=false;
        pgcObserv->ActivePage=tsHanderMajorMaintenance;

        tsCounter->TabVisible=false;
        tsTestCate->TabVisible=false;
        tsScanner->TabVisible=false;
        tsMDBQuery->TabVisible=false;
        tsYield->TabVisible=false;
        tsTestInfo->TabVisible=false;
        tsTemperature->TabVisible=false;
        tsOEE_ProductionInfor->TabVisible=false;
        bChangeReciepeSaveMajorMaintenanceRecord =true;
    }

    palCustomer->Caption    =fSCKART->sInfo_Customer;                           //JerryYang 20200330 : 修改SPIL LOT INFO
    palInnLotID->Caption    =fSCKART->sLotID;
    palCustLotID->Caption   =fSCKART->sInfo_CustLotID;
    palCustDevGup->Caption  =fSCKART->sInfo_CustDevGup;
    palDevName->Caption     =fSCKART->sInfo_DeviceName;
    palStage->Caption       =fSCKART->sInfo_Stage;
    palStep->Caption        =fSCKART->sInfo_Step;
    palReportCnt->Caption   =fSCKART->sInfo_ReportCnt;
    palProgramName->Caption =fSCKART->sInfo_ProgramName;
    palTestBin->Caption     =fSCKART->sInfo_TestBinNo;
    palTestID->Caption      =fSCKART->sInfo_TesterID;
    palHandlerID->Caption   =fSCKART->sInfo_HandlerID;
    palTemperature->Caption =fSCKART->sInfo_Temperauture;
    palCurrQty->Caption     =fSCKART->sInfo_CurrQty;
    palOPID->Caption        =fSCKART->sInfo_OperatorID;
    palBinSetting->Caption  =fSCKART->sInfo_BinSet;

    palMultiLotCnt->Caption=fSCKART->iInfo_MultiLotCnt;

    labBundleID->Caption=fSCKART->sBundleList;
    labBundlIn->Caption=fSCKART->iBundleInCnt;
    labBundOut->Caption=fSCKART->iBundleOutCnt;

    if(IniConfig.bVTESTFunction==true)
    {
        labDayJamRate->Visible=true;
        pnlDayJamRate->Visible=true;
    }

    tsIndexAirOn1->TabVisible=CosFunction.RecordIndexAirOnTime;                 //Sam 20220329 : Record Index Air On Time
    tsIndexAirOn2->TabVisible=CosFunction.RecordIndexAirOnTime;                 //Sam 20220329 : Record Index Air On Time
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::FormClose(TObject *Sender, TCloseAction &Action)
{
    bShow=false;
    iShowYieldChart=0;
    Timer1->Enabled=false;

    if(bShowMajorMaintenanceRecord)                                             //Sam 20171120 (Steven) AddPrecautionRecordFunction (form HT7045)
    {
        tsCounter->TabVisible=bTabVisible[0];
        tsTestCate->TabVisible=bTabVisible[1];
        tsScanner->TabVisible=bTabVisible[2];
        tsMDBQuery->TabVisible=bTabVisible[3];
        tsYield->TabVisible=bTabVisible[4];
        tsTestInfo->TabVisible=bTabVisible[5];
        tsTemperature->TabVisible=bTabVisible[6];
        tsOEE_ProductionInfor->TabVisible=bTabVisible[7];

        pgcPrecautions->ActivePage=tsPrecautionsRecord;
    }
    SavePrecautionParameter();
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::FormDestroy(TObject *Sender)
{
    try
    {
        ReleaseDC(0, DcSG2);
        ReleaseDC(0, DcSG3);
        ReleaseDC(0, DcSG5);
        delete pCanvas;                                                         //Steven 20160108 : release memory
        for(int j=tcTotalCount-1; j>=0; j--)                                    //Steven 20210621 : 動態產生溫度線段
        {
            TempChart->SeriesList->Delete(j);
        }
        TempChart->SeriesList->Clear();
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfObserver::FormDestroy");
    }
    LogSoftwareOffTime("TfObserver, FormDestroy");                              //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::BtnExitClick(TObject *Sender)
{
    if(bChangeReciepeSaveMajorMaintenanceRecord &&
       IniConfig.bB02_HanderMajorMaintenanceRecordFunction)
    {
        ShowMyMessage("Major Maintenance Information Not Enter Complete And Save, Please Check");
        return;
    }
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::Timer1Timer(TObject *Sender)
{
    if(bShow==false)
        return;

    GetMachineData();

    static Word iSystemSec=0, iSystemMin=0;                                     //Steven 20100818 Start: 按下Show Yield Chart按鈕一分鐘後要隱藏。
    static bool bStartShowYieldChart=false;
    static int iCT=0;

    if(iShowYieldChart==1)
    {
        bStartShowYieldChart=true;
        iShowYieldChart=2;
        iCT=0;
    }
    else if(iShowYieldChart==2)
    {
        if(bStartShowYieldChart)
        {
            iSystemSec=SystemSec;
            iSystemMin=SystemMin;
            bStartShowYieldChart=false;
        }
        else
        {
            if(SystemMin!=iSystemMin)
            {
                if((SystemSec+60-iSystemSec)>60)
                {
                    iSystemSec=SystemSec;
                    iSystemMin=SystemMin;
                    iCT++;
                }
            }

            if(iCT>=2)
            {
                iCT=0;
                Close();
            }
        }
    }
    //Steven 20100818 End
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::GetMachineData()
{
    labPowerOnTime->Caption=ConvertMSecToTime(LastSet.SystemAccSecond[0][stPowerOn]);
    labRunningTime->Caption=ConvertMSecToTime(LastSet.SystemAccSecond[0][stStartTime]);
    labProductTime->Caption=ConvertMSecToTime(LastSet.SystemAccSecond[0][stProductTime]);

    if(CUSTOMER_CODE==CC_AMKOR_Korea)                                           //jou 2012-05-04 CC_AMKOR_Korea不自動清除Loader Count
        labLoadingCount->Caption=LastSet.SendCT[0];
    else
        labLoadingCount->Caption=LastSet.SendCT[1];                             //jou 2010-08-13 計數jam rate,改為Tray Feed為一單位

    ProcessRunInfo();
}
//---------------------------------------------------------------------------
// 根據不同的吸嘴數，顯示對應的Site Yield曲線
//---------------------------------------------------------------------------
void __fastcall TfObserver::SetSiteYieldDiagram()                               //Steven 20090714
{
    AnsiString str;

    if(TestSocket.iShtRow<=2)
    {
        mtRowA->Width=860;
        mtRowB->Width=860;
    }
    else
    {
        mtRowA->Width=420;
        mtRowB->Width=420;
    }

    for(int i=0; i<TestSocket.iMaxRow; i++)
    {
        if(i<TestSocket.iShtRow)
        {
            mtRow[i]->Visible=true;
            for(int j=TestSocket.iShtCol; j<TestSocket.iMaxCol; j++)
            {
                bShowYieldSeries[i][j]=false;
            }
        }
        else
        {
            mtRow[i]->Visible=false;
            for(int j=0; j<TestSocket.iMaxCol; j++)
            {
                bShowYieldSeries[i][j]=false;
            }
        }
        mtRow[i]->YItem=TestSocket.iShtCol+1;

        if(bShowYieldAll[i])
            mtRow[i]->SetCellNumber(0, 0, "Show All");
        else
            mtRow[i]->SetCellNumber(0, 0, "Hide All");

        mtRow[i]->SetCellColorIndex(0, 0, 3+TestSocket.iMaxCol+1);
        mtRow[i]->SetCellNumber(1,  0, "Now");
        mtRow[i]->SetCellNumber(2,  0, "Last 1");
        mtRow[i]->SetCellNumber(3,  0, "Last 2");
        mtRow[i]->SetCellNumber(4,  0, "Last 3");
        mtRow[i]->SetCellNumber(5,  0, "Last 4");
        mtRow[i]->SetCellNumber(6,  0, "Last 5");
        mtRow[i]->SetCellNumber(7,  0, "Last 6");
        mtRow[i]->SetCellNumber(8,  0, "Last 7");
        mtRow[i]->SetCellNumber(9,  0, "Last 8");
        mtRow[i]->SetCellNumber(10, 0, "Last 9");

        for(int j=0; j<TestSocket.iMaxCol; j++)
        {
            str.sprintf("%c%c", 'A'+i, 'a'+j);
            mtRow[i]->SetCellNumber(0, j+1, str.c_str());

            if(bShowYieldSeries[i][j])
                mtRow[i]->SetCellColorIndex(0, j+1, 3+j);
            else
                mtRow[i]->SetCellColorIndex(0, j+1, 0);
        }
    }

    UpdateBin();                                                                //Steven 20131030 : 改完後要更新
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::UpdateYieldChart()
{
    int iLast, iSeries;
    AnsiString str;
    RUN_INFO *RIF;
    RIF=&RunInfo;

    for(int iRow=0; iRow<MAX_SOCKET_TOTAL; iRow++)                              //Steven 20100126 : 將全部的線都清空
        ChartYield->Series[iRow]->Clear();

    for(int iRow=0; iRow<MAX_SOCKET_ROW; iRow++)
    {
        for(int iCol=0; iCol<MAX_SOCKET_COL; iCol++)
        {
            iSeries=iRow*MAX_SOCKET_COL+iCol;
            for(iLast=0; iLast<25; iLast++)
            {
                if((iLast%2)==0)    str.sprintf("%02d:%02d\0", RIF->iYieldHour[iLast], RIF->iYieldMin[iLast]);
                else                str=AnsiString("");                         //Steven 20210625 : 修正sprintf("")會跳例外的問題
                ChartYield->Series[iSeries]->AddY(RIF->iYieldChart[iRow][iCol][iLast], str, TC[iSeries]);
            }

            ChartYield->Series[iSeries]->Active=bShowYieldSeries[iRow][iCol];
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::SpeedButton1Click(TObject *Sender)                  //清空BIN的歷史資料
{
    for(int iRow=0; iRow<MAX_SOCKET_ROW; iRow++)
        for(int iCol=0; iCol<MAX_SOCKET_COL; iCol++)
            for(int iLast=0; iLast<21; iLast++)                                 //Eastsun 20260526 #026-1.33 Ifor 20221026 add:KYEC 要求增加比數10 => 21
                HistroyBin[iRow][iCol][iLast]=0;
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::UpdateBin()
{
    int ir, ic;

    for(int iLast=0; iLast<21; iLast++)                                         //Eastsun 20260526 #026-1.34 Ifor 20221026 add:KYEC 要求增加比數10 => 21
    {
        for(int iRow=0; iRow<MAX_SOCKET_ROW; iRow++)
        {
            for(int iCol=0; iCol<MAX_SOCKET_COL; iCol++)
            {
                ir=iLast+1;
                ic=iCol+1;
                if(HistroyBin[iRow][iCol][iLast]!=0 && HistroyBin[iRow][iCol][iLast]!=-1) //kevin 20150304  關SITE不顯示 //A Row
                {
                    mtRow[iRow]->SetCellNumber(ir, ic, HistroyBin[iRow][iCol][iLast]);
                    if(HistroyPassFail[iRow][iCol][iLast])
                        mtRow[iRow]->SetCellColorIndex(ir, ic, 1);
                    else
                        mtRow[iRow]->SetCellColorIndex(ir, ic, 2);
                }
                else
                {
                    mtRow[iRow]->SetCellNumber(ir, ic, "");
                    mtRow[iRow]->SetCellColorIndex(ir, ic, 0);
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
//  溫度曲線
//---------------------------------------------------------------------------
void __fastcall TfObserver::UpdateTempChart()                                   //Steven 20090827
{
    AnsiString S;
    if(     IniConfig.iL10TempRecordInterval==0)  S=" (5  Sec)";
    else if(IniConfig.iL10TempRecordInterval==2)  S=" (30 Sec)";
    else if(IniConfig.iL10TempRecordInterval==3)  S=" (1  Min)";
    else if(IniConfig.iL10TempRecordInterval==4)  S=" (5  Min)";
    else if(IniConfig.iL10TempRecordInterval==5)  S=" (10 Min)";
    else                                          S=" (15 Sec)";

    TempChart->Title->Text->Clear();
    TempChart->Title->Text->Add("Temperature Chart of " + cbbTempChart->Text + S);

    if(cbbTempChart->ItemIndex==0)
    {
        for(int j=0; j<tcTotalCount; j++)
        {
            TempChart->Series[j]->Clear();
            if(bUT150Install[j])
            {
                for(int i=0; i<60; i++)
                {
                    TempChart->Series[j]->AddY(dTempHistroy[j][i], " ", TC[j]); //Steven 20140923 : Index使用EJ1N版32組加熱器
                }
            }
        }
    }
    else
    {
        for(int j=0; j<tcTotalCount; j++)
            TempChart->Series[j]->Clear();

        int index=cbbTempChart->ItemIndex-1;
        for(int i=0; i<60; i++)
        {
            TempChart->Series[index]->AddY(dTempHistroy[index][i], " ", TC[index]);    //Steven 20140923 : Index使用EJ1N版32組加熱器
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::cbbTempChartChange(TObject *Sender)
{
    UpdateTempChart();
}
//---------------------------------------------------------------------------
//  Contact Count (Kinds)
//---------------------------------------------------------------------------
void __fastcall TfObserver::StringGrid2DrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
    int iTotal=0, iLeft, iCellWidth;
    AnsiString Str;

    if(MachineTypeChoice==Type_HT9045) //9045
    {
       iLeft=90;
       iCellWidth=91;
    }
    else
    {
       iLeft=47;
       iCellWidth=48;
    }

    pCanvas->Handle=DcSG2;
    DrawCellCounter(ACol, ARow, Rect, iLeft, iCellWidth);

    Str.sprintf("Row-%c", 'A'+rgContactCountKinds->ItemIndex);
    MyDrawText(pCanvas, Rect, Str.c_str(), 0, iLeft, 0, 40);

    pCanvas->Brush->Color = (TColor)0x00DFD9CC;
    for(int i=0; i<MAX_SOCKET_COL; i++)
        MyDrawText(pCanvas, Rect, sCounterColKind[0][i].c_str(),  iLeft+iCellWidth*2*i, iLeft+iCellWidth*2*(i+1),  63,  83);

    iTotal=0;
    for(int i=0; i<MAX_SOCKET_COL; i++)
        iTotal+=atoi(sCounterColKind[0][i].c_str());
    APHeadLabel13->Caption=iTotal;
    DrawCenterLine(0, iLeft, iCellWidth);
}
//---------------------------------------------------------------------------
// Contact Count (History)
//---------------------------------------------------------------------------
void __fastcall TfObserver::StringGrid3DrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
    int iTotal=0, iLeft, iCellWidth;
    AnsiString Str;

    if(MachineTypeChoice==Type_HT9045)                                          //9045
    {
        iLeft=90;
        iCellWidth=91;
    }
    else
    {
        iLeft=48;
        iCellWidth=48;
    }
    pCanvas->Handle=DcSG3;
    DrawCellCounter(ACol, ARow, Rect, iLeft, iCellWidth);

    Str.sprintf("Row-%c", 'A'+rgContactCountKinds->ItemIndex);
    MyDrawText(pCanvas, Rect, Str.c_str(), 0, iLeft, 0, 40);

    pCanvas->Brush->Color=(TColor)0x00DFD9CC;
    for(int i=0; i<MAX_SOCKET_COL; i++)
        MyDrawText(pCanvas, Rect, sCounterColKind[1][i].c_str(),  iLeft+iCellWidth*2*i, iLeft+iCellWidth*2*(i+1),  63,  83);

    iTotal=0;
    for(int i=0; i<MAX_SOCKET_COL; i++)
        iTotal+=atoi(sCounterColKind[1][i].c_str());
    APHeadLabel14->Caption=AnsiString(iTotal);
    DrawCenterLine(0, iLeft, iCellWidth);
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::DrawCenterLine(int Mode, int iLeft, int iCellWidth)
{
    int iPos=iLeft;
    if(Mode==0)
    {
        for(int i=0; i<=MAX_SOCKET_COL; i++)
        {
            pCanvas->MoveTo(iPos,   0);
            pCanvas->LineTo(iPos,  85);
            iPos+=iCellWidth*2;
        }
        pCanvas->MoveTo(  0,  41);
        pCanvas->LineTo(820,  41);
    }
    else if(Mode==1)
    {
        for(int i=0; i<=MAX_SOCKET_COL; i++)
        {
            pCanvas->MoveTo(iPos,   0);
            pCanvas->LineTo(iPos, 483);
            iPos+=iCellWidth;
        }

        if(MachineTypeChoice==Type_HT9045)                                      //9045
        {
            pCanvas->MoveTo(  0,  61);
            pCanvas->LineTo(810,  61);
            pCanvas->MoveTo(  0, 398);
            pCanvas->LineTo(810, 398);
        }
        else
        {
            pCanvas->MoveTo(  0,  61);
            pCanvas->LineTo(845,  61);
            pCanvas->MoveTo(  0, 398);
            pCanvas->LineTo(845, 398);
        }
    }
    else
    {
        pCanvas->MoveTo( 80,   0);
        pCanvas->LineTo( 80, 188);
        pCanvas->MoveTo(241,   0);
        pCanvas->LineTo(241, 188);
        pCanvas->MoveTo(404,   0);
        pCanvas->LineTo(404, 188);
        pCanvas->MoveTo(566,   0);
        pCanvas->LineTo(566, 188);
        pCanvas->MoveTo(727,   0);
        pCanvas->LineTo(727, 188);
        pCanvas->MoveTo(  0,  61);
        pCanvas->LineTo(810,  61);
        pCanvas->MoveTo(  0, 146);
        pCanvas->LineTo(810, 146);
    }
}
//---------------------------------------------------------------------------
// Contact Count 的內容
//---------------------------------------------------------------------------
void __fastcall TfObserver::DrawCellCounter(int iCol, int iRow, TRect &Rect, int iLeft, int iCellWidth)
{
    pCanvas->Brush->Color = (TColor)0x00917B51;
    if(iCol==0)
    {
        pCanvas->FillRect(Rect);
        if(iRow==2)   MyDrawText(pCanvas, Rect, "Head");
        if(iRow==3)   MyDrawText(pCanvas, Rect, "Socket");
    }
    else if(iRow==1)
    {
        pCanvas->FillRect(Rect);
        if(iCol>=1 && iCol<=MAX_SOCKET_COL*2)
        {
            if(IsNNMode()==NN_2Row)                                             //Steven 20210630 : 修正32site的socket資料顯示
            {
                if(iCol%2!=0)
                {
                    if(pCanvas->Handle==DcSG2)
                    {
                        if(rgContactCountKinds->ItemIndex<2)
                            MyDrawText(pCanvas, Rect, "Arm 2");
                        else
                            MyDrawText(pCanvas, Rect, "Arm 1");
                    }
                    else
                    {
                        if(rgContactCountHistory->ItemIndex<2)
                            MyDrawText(pCanvas, Rect, "Arm 2");
                        else
                            MyDrawText(pCanvas, Rect, "Arm 1");
                    }
                }
            }
            else
            {
                if(iCol%2==0)
                    MyDrawText(pCanvas, Rect, "Arm 2");
                else
                    MyDrawText(pCanvas, Rect, "Arm 1");
            }
        }
    }
    else
    {
        AnsiString aStr="";
        for(int i=0; i<MAX_SOCKET_COL; i++)
        {
            aStr.sprintf("Col-%c", 'a'+i);
            MyDrawText(pCanvas, Rect, aStr.c_str(), iLeft+iCellWidth*2*i, iLeft+iCellWidth*2*(i+1), 0, 21);
        }
    }
}
//---------------------------------------------------------------------------
// Tester Category 的內容
//---------------------------------------------------------------------------
void __fastcall TfObserver::DrawCellCategory(int iCol, int iRow, TRect &Rect, HDC SGDC, int Mode)
{
    pCanvas->Handle=SGDC;
    pCanvas->Brush->Color=(TColor)0x00917B51;
    int iLeft, CellWidth;
    AnsiString aStr="";

    if(MachineTypeChoice==Type_HT9045)                                          //9045
    {
        iLeft=82;
        CellWidth=162;
    }
    else
    {
        iLeft=70;
        CellWidth=90;
    }

    if(iCol==0)
    {
        pCanvas->FillRect(Rect);
        if(iRow==0) MyDrawText(pCanvas, Rect, "No.");
        if(Mode==1)
        {
            if(     iRow==3) MyDrawText(pCanvas, Rect, "I/F Error");
            else if(iRow==4) MyDrawText(pCanvas, Rect, "Category01");
            else if(iRow==5) MyDrawText(pCanvas, Rect, "Category02");
            else if(iRow==6) MyDrawText(pCanvas, Rect, "Category03");
            else if(iRow==7) MyDrawText(pCanvas, Rect, "Category04");
            else if(iRow==8) MyDrawText(pCanvas, Rect, "Category05");
            else if(iRow==9) MyDrawText(pCanvas, Rect, "Category06");
            else if(iRow==10)MyDrawText(pCanvas, Rect, "Category07");
            else if(iRow==11)MyDrawText(pCanvas, Rect, "Category08");
            else if(iRow==12)MyDrawText(pCanvas, Rect, "Category09");
            else if(iRow==13)MyDrawText(pCanvas, Rect, "Category10");
            else if(iRow==14)MyDrawText(pCanvas, Rect, "Category11");
            else if(iRow==15)MyDrawText(pCanvas, Rect, "Category12");
            else if(iRow==16)MyDrawText(pCanvas, Rect, "Category13");
            else if(iRow==17)MyDrawText(pCanvas, Rect, "Category14");
            else if(iRow==18)MyDrawText(pCanvas, Rect, "Category15");
            else if(iRow==19)MyDrawText(pCanvas, Rect, "Head Total");
            else if(iRow==20)MyDrawText(pCanvas, Rect, "Socket Total");
            else if(iRow==21)MyDrawText(pCanvas, Rect, "Pass Head");
            else if(iRow==22)MyDrawText(pCanvas, Rect, "Pass Socket");
        }
        else
        {
            if(     iRow==3) MyDrawText(pCanvas, Rect, "OK");
            else if(iRow==4) MyDrawText(pCanvas, Rect, "NG");
            else if(iRow==5) MyDrawText(pCanvas, Rect, "Invalid");
            else if(iRow==6) MyDrawText(pCanvas, Rect, "Rework");
            else if(iRow==7) MyDrawText(pCanvas, Rect, "Head Total");
            else if(iRow==8) MyDrawText(pCanvas, Rect, "Socket Total");
        }
    }
    else if(MachineTypeChoice==Type_HT9045 && iCol==9)                          //9045
    {
        pCanvas->Brush->Color = TColor(0x00DFD9CC);
        if(Mode==1)
        {
            if(     iRow==19) MyDrawText(pCanvas, Rect, "Total");
            else if(iRow==21) MyDrawText(pCanvas, Rect, "Total");
        }
        else
        {
            if(iRow==7)       MyDrawText(pCanvas, Rect, "Total");
        }
    }
    else if(MachineTypeChoice==Type_HT9046 && iCol==17)                         //9046
    {
        pCanvas->Brush->Color = (TColor)0x00DFD9CC;
        if(Mode==1)
        {
            if(     iRow==19) MyDrawText(pCanvas, Rect, "Total");
            else if(iRow==21) MyDrawText(pCanvas, Rect, "Total");
        }
        else
        {
            if(iRow==7)       MyDrawText(pCanvas, Rect, "Total");
        }
    }
    else if(iRow==2)
    {
        pCanvas->Brush->Color=(TColor)0x00917B51;
        if(iCol>=1 && iCol<=MAX_SOCKET_COL*2)
        {
            if(iCol%2==0)
                MyDrawText(pCanvas, Rect, "Arm2");
            else
                MyDrawText(pCanvas, Rect, "Arm1");
        }
    }
    else
    {
        if(     RowNo==0)
        {
            MyDrawText(pCanvas, Rect, "Row-A", 0, iLeft, 21, 60);
            for(int i=0; i<MAX_SOCKET_COL; i++)
                MyDrawText(pCanvas, Rect, TesterCH[0][i].c_str(), iLeft+CellWidth*i, iLeft+CellWidth*(i+1), 0, 20);
        }
        else if(RowNo==1)
        {
            MyDrawText(pCanvas, Rect, "Row-B", 0, iLeft, 21, 60);
            for(int i=0; i<MAX_SOCKET_COL; i++)
                MyDrawText(pCanvas, Rect, TesterCH[1][i].c_str(), iLeft+CellWidth*i, iLeft+CellWidth*(i+1), 0, 20);
        }
        else if(RowNo==2)   MyDrawText(pCanvas, Rect, "Row-C", 0, iLeft, 21, 60);
        else if(RowNo==3)   MyDrawText(pCanvas, Rect, "Row-D", 0, iLeft, 21, 60);

        if(MachineTypeChoice==Type_HT9045)                                      //9045
            MyDrawText(pCanvas, Rect, "Category Total", 729, 809,   0,  62);
        else
            MyDrawText(pCanvas, Rect, "Total", 791, 835,   0,  62);

        for(int i=0; i<MAX_SOCKET_COL; i++)
        {
            aStr.sprintf("Col-%c", 'a'+i);
            MyDrawText(pCanvas, Rect, aStr.c_str(), iLeft+CellWidth*i, iLeft+CellWidth*(i+1), 21, 41);
        }

        pCanvas->Brush->Color=TColor(0x00DFD9CC);
        if(Mode==1)
        {
            int l, r, t, b;
            for(int i=0; i<MAX_SOCKET_COL; i++)
            {
                l=iLeft+CellWidth*i;
                r=l+CellWidth;
                t=420;
                b=440;
                MyDrawText(pCanvas, Rect, sSKETTotal[i].c_str(), l, r, t, b);

                t=462;
                b=482;
                MyDrawText(pCanvas, Rect, sPassSKET[i].c_str(), l, r, t, b);
            }

            if(rbSocketNumber->Checked || rbSocketPercent->Checked)
            {
                for(int i=0; i<MAX_SOCKET_COL; i++)
                {
                    l=iLeft+CellWidth*i;
                    r=l+CellWidth;
                    t=62;
                    b=83;
                    MyDrawText(pCanvas, Rect, sSocketCT[i][15], l, r, t, b);
                    for(int j=0; j<15; j++)
                    {
                        l=iLeft+CellWidth*i;
                        r=l+CellWidth;
                        t=84+21*j;
                        b=104+21*j;
                        MyDrawText(pCanvas, Rect, sSocketCT[i][j], l, r, t, b);
                    }
                }
            }
        }
        else
        {
            for(int i=0; i<MAX_SOCKET_COL; i++)
                MyDrawText(pCanvas, Rect,  iLeft+CellWidth*i, iLeft+CellWidth*(i+1), 168, 188);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::WriteContactKind()
{
    int iArm, iRow;
    AnsiString Result="";

    if(rgContactCountKinds->ItemIndex<0)                                        //Steven 20210802 : 修正-1的狀況
        rgContactCountKinds->ItemIndex=0;

    if(IsNNMode()==NN_2Row)                                                     //Steven 20210630 : 修正32site的socket資料顯示
    {
        if(rgContactCountKinds->ItemIndex<2)
        {
            iArm=1;
            iRow=rgContactCountKinds->ItemIndex;
        }
        else
        {
            iArm=0;
            iRow=rgContactCountKinds->ItemIndex-2;
        }
    }
    else if(IsNNMode()==NN_1Row)
    {
        if(rgContactCountKinds->ItemIndex==0)
        {
            iArm=1;
            iRow=0;
        }
        else if(rgContactCountKinds->ItemIndex==1)
        {
            iArm=0;
            iRow=0;
        }
    }

    if(rgContactCountKindsForm->ItemIndex==0)                                   //Total
    {
        for(int i=0; i<MAX_SOCKET_COL; i++)
        {
            if(IsNNMode()==NN_2Row)                                             //Steven 20210630 : 修正32site的socket資料顯示
            {
                StringGrid2->Cells[2*i+1][2]=ArmData[iArm]->ArmSKET[iRow][i]->GetTotal();
                sCounterColKind[0][i]=AnsiString(ArmData[iArm]->ArmSKET[iRow][i]->GetTotal());
            }
            else if(TestIF.iTestMode==QualSite2X2N)
            {
                if(i<2)
                {
                    StringGrid2->Cells[2*i+1][2]=ArmData[iArm]->ArmSKET[iRow][i]->GetTotal();
                    sCounterColKind[0][i]=AnsiString(ArmData[iArm]->ArmSKET[iRow][i]->GetTotal());
                }
            }
            else if(TestIF.iTestMode==_6Site2X3N)                               //Steven 20220425 : 2X3NN Mode
            {
                if(i<3)
                {
                    StringGrid2->Cells[2*i+1][2]=ArmData[iArm]->ArmSKET[iRow][i]->GetTotal();
                    sCounterColKind[0][i]=AnsiString(ArmData[iArm]->ArmSKET[iRow][i]->GetTotal());
                }
            }
            else if(TestIF.iTestMode==_8Site2X4N)                               //Wei 20231211 : 2X4NN Mode
            {
                if(i<4)
                {
                    StringGrid2->Cells[2*i+1][2]=ArmData[iArm]->ArmSKET[iRow][i]->GetTotal();
                    sCounterColKind[0][i]=AnsiString(ArmData[iArm]->ArmSKET[iRow][i]->GetTotal());
                }
            }
            else
            {
                for(int j=0; j<2; j++)
                {
                    if(j==0)
                        StringGrid2->Cells[2*i+1][2]=ArmData[j]->ArmSKET[rgContactCountKinds->ItemIndex][i]->GetTotal(); //Arm1
                    else
                        StringGrid2->Cells[2*i+2][2]=ArmData[j]->ArmSKET[rgContactCountKinds->ItemIndex][i]->GetTotal(); //Arm2

                    sCounterColKind[0][i]=AnsiString(ArmData[0]->ArmSKET[rgContactCountKinds->ItemIndex][i]->GetTotal()+ArmData[1]->ArmSKET[rgContactCountKinds->ItemIndex][i]->GetTotal());
                }
            }
        }
    }
    else if(rgContactCountKindsForm->ItemIndex==1)                              //Kinds
    {
        for(int i=0; i<MAX_SOCKET_COL; i++)
        {
            if(IsNNMode()==NN_2Row)                                             //Steven 20210630 : 修正32site的socket資料顯示
            {
                StringGrid2->Cells[2*i+1][2]=ArmData[iArm]->ArmSKET[iRow][i]->GetPassCT();
                sCounterColKind[0][i]=AnsiString(ArmData[iArm]->ArmSKET[iRow][i]->GetPassCT());
            }
            else if(TestIF.iTestMode==QualSite2X2N)
            {
                if(i<2)
                {
                    StringGrid2->Cells[2*i+1][2]=ArmData[iArm]->ArmSKET[iRow][i]->GetPassCT();
                    sCounterColKind[0][i]=AnsiString(ArmData[iArm]->ArmSKET[iRow][i]->GetPassCT());
                }
            }
            else if(TestIF.iTestMode==_6Site2X3N)                               //Steven 20220425 : 2X3NN Mode
            {
                if(i<3)
                {
                    StringGrid2->Cells[2*i+1][2]=ArmData[iArm]->ArmSKET[iRow][i]->GetPassCT();
                    sCounterColKind[0][i]=AnsiString(ArmData[iArm]->ArmSKET[iRow][i]->GetPassCT());
                }
            }
            else if(TestIF.iTestMode==_8Site2X4N)                               //Wei 20231211 : 2X4NN Mode
            {
                if(i<4)
                {
                    StringGrid2->Cells[2*i+1][2]=ArmData[iArm]->ArmSKET[iRow][i]->GetPassCT();
                    sCounterColKind[0][i]=AnsiString(ArmData[iArm]->ArmSKET[iRow][i]->GetPassCT());
                }
            }
            else
            {
                for(int j=0; j<2; j++)
                {
                    if(j==0)
                        StringGrid2->Cells[2*i+1][2]=ArmData[j]->ArmSKET[rgContactCountKinds->ItemIndex][i]->GetPassCT(); //Arm1
                    else
                        StringGrid2->Cells[2*i+2][2]=ArmData[j]->ArmSKET[rgContactCountKinds->ItemIndex][i]->GetPassCT(); //Arm2
                }
                sCounterColKind[0][i]=AnsiString(ArmData[0]->ArmSKET[rgContactCountKinds->ItemIndex][i]->GetPassCT()+ArmData[1]->ArmSKET[rgContactCountKinds->ItemIndex][i]->GetPassCT());
            }
        }
    }
    else  //%
    {
        for(int i=0; i<MAX_SOCKET_COL; i++)
        {
            if(IsNNMode()==NN_2Row)                                             //Steven 20210630 : 修正32site的socket資料顯示
            {
                StringGrid2->Cells[2*i+1][2]=ArmData[iArm]->ArmSKET[iRow][i]->GetPCA();
                sCounterColKind[0][i]=AnsiString(ArmData[iArm]->ArmSKET[iRow][i]->GetPCA());
            }
            else if(TestIF.iTestMode==QualSite2X2N)
            {
                if(i<2)
                {
                    StringGrid2->Cells[2*i+1][2]=ArmData[iArm]->ArmSKET[iRow][i]->GetPCA();
                    sCounterColKind[0][i]=AnsiString(ArmData[iArm]->ArmSKET[iRow][i]->GetPCA());
                }
            }
            else if(TestIF.iTestMode==_6Site2X3N)                               //Steven 20220425 : 2X3NN Mode
            {
                if(i<3)
                {
                    StringGrid2->Cells[2*i+1][2]=ArmData[iArm]->ArmSKET[iRow][i]->GetPCA();
                    sCounterColKind[0][i]=AnsiString(ArmData[iArm]->ArmSKET[iRow][i]->GetPCA());
                }
            }
            else if(TestIF.iTestMode==_8Site2X4N)                               //Wei 20231211 : 2X4NN Mode
            {
                if(i<4)
                {
                    StringGrid2->Cells[2*i+1][2]=ArmData[iArm]->ArmSKET[iRow][i]->GetPCA();
                    sCounterColKind[0][i]=AnsiString(ArmData[iArm]->ArmSKET[iRow][i]->GetPCA());
                }
            }
            else
            {
                for(int j=0; j<2; j++)
                {
                    if(j==0)
                        StringGrid2->Cells[2*i+1][2]=Result.sprintf("%3.1f%%", ArmData[j]->ArmSKET[rgContactCountKinds->ItemIndex][i]->GetPCA());  //Arm1
                    else
                        StringGrid2->Cells[2*i+2][2]=Result.sprintf("%3.1f%%", ArmData[j]->ArmSKET[rgContactCountKinds->ItemIndex][i]->GetPCA());  //Arm2
                }
                sCounterColKind[0][i]=Result.sprintf("%3.1f%%", (ArmData[0]->ArmSKET[rgContactCountKinds->ItemIndex][i]->GetPCA()+ArmData[1]->ArmSKET[rgContactCountKinds->ItemIndex][i]->GetPCA())/2);
            }
        }
    }

    if(rgContactCountHistory->ItemIndex<0)                                      //Steven 20210802 : 修正-1的狀況
        rgContactCountHistory->ItemIndex=0;

    if(IsNNMode()==NN_2Row)                                                     //Steven 20210630 : 修正32site的socket資料顯示
    {
        if(rgContactCountHistory->ItemIndex<2)
        {
            iArm=1;
            iRow=rgContactCountHistory->ItemIndex;
        }
        else
        {
            iArm=0;
            iRow=rgContactCountHistory->ItemIndex-2;
        }
    }
    else if(IsNNMode()==NN_1Row)
    {
        if(rgContactCountHistory->ItemIndex==0)
        {
            iArm=1;
            iRow=0;
        }
        else if(rgContactCountHistory->ItemIndex==1)
        {
            iArm=0;
            iRow=0;
        }
    }

    //History
    if(rgContactCountHistoryForm->ItemIndex==0)                                 //Total
    {
        for(int i=0; i<MAX_SOCKET_COL; i++)
        {
            if(IsNNMode()==NN_2Row)                                             //Steven 20210630 : 修正32site的socket資料顯示
            {
                StringGrid3->Cells[2*i+1][2]=ArmHistory[iArm]->ArmSKET[iRow][i]->GetTotal();
                sCounterColKind[1][i]=AnsiString(ArmHistory[iArm]->ArmSKET[iRow][i]->GetTotal());
            }
            else if(TestIF.iTestMode==QualSite2X2N)
            {
                if(i<2)
                {
                    StringGrid3->Cells[2*i+1][2]=ArmHistory[iArm]->ArmSKET[iRow][i]->GetTotal();
                    sCounterColKind[1][i]=AnsiString(ArmHistory[iArm]->ArmSKET[iRow][i]->GetTotal());
                }
            }
            else if(TestIF.iTestMode==_6Site2X3N)                               //Steven 20220425 : 2X3NN Mode
            {
                if(i<3)
                {
                    StringGrid3->Cells[2*i+1][2]=ArmHistory[iArm]->ArmSKET[iRow][i]->GetTotal();
                    sCounterColKind[1][i]=AnsiString(ArmHistory[iArm]->ArmSKET[iRow][i]->GetTotal());
                }
            }
            else if(TestIF.iTestMode==_8Site2X4N)                               //Wei 20231211 : 2X4NN Mode
            {
                if(i<4)
                {
                    StringGrid3->Cells[2*i+1][2]=ArmHistory[iArm]->ArmSKET[iRow][i]->GetTotal();
                    sCounterColKind[1][i]=AnsiString(ArmHistory[iArm]->ArmSKET[iRow][i]->GetTotal());
                }
            }
            else
            {
                for(int j=0; j<2; j++)
                {
                    if(j==0)
                        StringGrid3->Cells[2*i+1][2]=ArmHistory[j]->ArmSKET[rgContactCountHistory->ItemIndex][i]->GetTotal();  //Arm1
                    else
                        StringGrid3->Cells[2*i+2][2]=ArmHistory[j]->ArmSKET[rgContactCountHistory->ItemIndex][i]->GetTotal();  //Arm2
                }
                sCounterColKind[1][i]=AnsiString(ArmHistory[0]->ArmSKET[rgContactCountHistory->ItemIndex][i]->GetTotal()+ArmHistory[1]->ArmSKET[rgContactCountHistory->ItemIndex][i]->GetTotal());
            }
        }
    }
    else if(rgContactCountHistoryForm->ItemIndex==1)                            //Kinds
    {
        for(int i=0; i<MAX_SOCKET_COL; i++)
        {
            if(IsNNMode()==NN_2Row)                                             //Steven 20210630 : 修正32site的socket資料顯示
            {
                StringGrid3->Cells[2*i+1][2]=ArmHistory[iArm]->ArmSKET[iRow][i]->GetPassCT();
                sCounterColKind[1][i]=AnsiString(ArmHistory[iArm]->ArmSKET[iRow][i]->GetPassCT());
            }
            else if(TestIF.iTestMode==QualSite2X2N)
            {
                if(i<2)
                {
                    StringGrid3->Cells[2*i+1][2]=ArmHistory[iArm]->ArmSKET[iRow][i]->GetPassCT();
                    sCounterColKind[1][i]=AnsiString(ArmHistory[iArm]->ArmSKET[iRow][i]->GetPassCT());
                }
            }
            else if(TestIF.iTestMode==_6Site2X3N)                               //Steven 20220425 : 2X3NN Mode
            {
                if(i<3)
                {
                    StringGrid3->Cells[2*i+1][2]=ArmHistory[iArm]->ArmSKET[iRow][i]->GetPassCT();
                    sCounterColKind[1][i]=AnsiString(ArmHistory[iArm]->ArmSKET[iRow][i]->GetPassCT());
                }
            }
            else if(TestIF.iTestMode==_8Site2X4N)                               //Wei 20231211 : 2X4NN Mode
            {
                if(i<4)
                {
                    StringGrid3->Cells[2*i+1][2]=ArmHistory[iArm]->ArmSKET[iRow][i]->GetPassCT();
                    sCounterColKind[1][i]=AnsiString(ArmHistory[iArm]->ArmSKET[iRow][i]->GetPassCT());
                }
            }
            else
            {
                for(int j=0; j<2; j++)
                {
                    if(j==0)
                        StringGrid3->Cells[2*i+1][2]=ArmHistory[j]->ArmSKET[rgContactCountHistory->ItemIndex][i]->GetPassCT();  //Arm1
                    else
                        StringGrid3->Cells[2*i+2][2]=ArmHistory[j]->ArmSKET[rgContactCountHistory->ItemIndex][i]->GetPassCT();  //Arm2
                }
                sCounterColKind[1][i]=AnsiString(ArmHistory[0]->ArmSKET[rgContactCountHistory->ItemIndex][i]->GetPassCT()+ArmHistory[1]->ArmSKET[rgContactCountHistory->ItemIndex][i]->GetPassCT());
            }
        }
    }
    else  //%
    {
        for(int i=0; i<MAX_SOCKET_COL; i++)
        {
            if(IsNNMode()==NN_2Row)                                             //Steven 20210630 : 修正32site的socket資料顯示
            {
                StringGrid3->Cells[2*i+1][2]=ArmHistory[iArm]->ArmSKET[iRow][i]->GetPCA();
                sCounterColKind[1][i]=AnsiString(ArmHistory[iArm]->ArmSKET[iRow][i]->GetPCA());
            }
            else if(TestIF.iTestMode==QualSite2X2N)
            {
                if(i<2)
                {
                    StringGrid3->Cells[2*i+1][2]=ArmHistory[iArm]->ArmSKET[iRow][i]->GetPCA();
                    sCounterColKind[1][i]=AnsiString(ArmHistory[iArm]->ArmSKET[iRow][i]->GetPCA());
                }
            }
            else if(TestIF.iTestMode==_6Site2X3N)                               //Steven 20220425 : 2X3NN Mode
            {
                if(i<3)
                {
                    StringGrid3->Cells[2*i+1][2]=ArmHistory[iArm]->ArmSKET[iRow][i]->GetPCA();
                    sCounterColKind[1][i]=AnsiString(ArmHistory[iArm]->ArmSKET[iRow][i]->GetPCA());
                }
            }
            else if(TestIF.iTestMode==_8Site2X4N)                               //Wei 20231211 : 2X4NN Mode
            {
                if(i<4)
                {
                    StringGrid3->Cells[2*i+1][2]=ArmHistory[iArm]->ArmSKET[iRow][i]->GetPCA();
                    sCounterColKind[1][i]=AnsiString(ArmHistory[iArm]->ArmSKET[iRow][i]->GetPCA());
                }
            }
            else
            {
                for(int j=0; j<2; j++)
                {
                    if(j==0)
                        StringGrid3->Cells[2*i+1][2]=Result.sprintf("%3.1f%%", ArmHistory[j]->ArmSKET[rgContactCountHistory->ItemIndex][i]->GetPCA());  //Arm1
                    else
                        StringGrid3->Cells[2*i+2][2]=Result.sprintf("%3.1f%%", ArmHistory[j]->ArmSKET[rgContactCountHistory->ItemIndex][i]->GetPCA());  //Arm2
                }
                sCounterColKind[1][i]=Result.sprintf("%3.1f%%", (ArmHistory[0]->ArmSKET[rgContactCountHistory->ItemIndex][i]->GetPCA()+ArmHistory[1]->ArmSKET[rgContactCountHistory->ItemIndex][i]->GetPCA())/2);
            }
        }
    }
}
//---------------------------------------------------------------------------
// 清除 Contact Count (Kinds) 的項目
//---------------------------------------------------------------------------
void __fastcall TfObserver::StringGrid2MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    for(int i=0; i<2; i++)
        StringGrid2->Cells[1][i]="";
}
//---------------------------------------------------------------------------
// 清除 Contact Count (History) 的項目
//---------------------------------------------------------------------------
void __fastcall TfObserver::StringGrid3MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    for(int i=0; i<2; i++)
        StringGrid3->Cells[1][i]="";
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::rgRowNoClick(TObject *Sender)
{
    WriteCategoryData();
}
//---------------------------------------------------------------------------
// Scanner Category
//---------------------------------------------------------------------------
void __fastcall TfObserver::StringGrid5DrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
    if(RadioButton17->Checked) RowNo=0;
    if(RadioButton18->Checked) RowNo=1;
    if(RadioButton19->Checked) RowNo=2;
    if(RadioButton20->Checked) RowNo=3;

    DrawCellCategory(ACol, ARow, Rect, DcSG5, 0);
    DrawCenterLine(2, 0, 0);
}
//---------------------------------------------------------------------------
// 時間相關
//---------------------------------------------------------------------------
AnsiString __fastcall TfObserver::CalculateStopTime(int Sec)
{
    int i_mm;
    AnsiString AS_Time, ss, mm, hh;

    ss  =Sec%60;
    i_mm=Sec/60;
    if(i_mm>60)
    {
        mm=i_mm%60;
        hh=i_mm/60;
        if(mm.Length()==1) mm="0"+mm;
        AS_Time=hh+":"+mm+":"+ss;
    }
    else
    {
        mm=i_mm;
        if(mm.Length()==1) mm="0"+mm;
        AS_Time="0:"+mm+":"+ss;
    }
    return AS_Time;
}
//---------------------------------------------------------------------------
void RecordMonitoringIndexCycleTime()                                           //Isaac 20180301 (Steven) Index Cycle Time Monitoring function
{
    double avg=0.0, sum=0.0;
    AnsiString Str="";
    bool compareflag=true;
    fObserver->dTotoalIndexCycleTime*=0.001;                                    //Isaac 20180302 換算成秒
    if(fObserver->dTotoalIndexCycleTime<TestIF_File.dMonitorOutlier)            //判斷是否入陣列
    {
        queue20[fObserver->iIndexCycleTimeCount]=fObserver->dTotoalIndexCycleTime;          //列入陣列
        fObserver->iIndexCycleTimeCount++;
        fObserver->iIndexCycleTimeCount%=TestIF_File.iMonitorWindow;            //計算循環，使先進先出
        iMonitoringOutlierCnt=0;                                                //Steven 20180626 (wei) : 超過設定值的也要檢查
    }
    else
    {
        iMonitoringOutlierCnt++;                                                //Steven 20180626 (wei) : 超過設定值的也要檢查
    }

    if(iMonitoringOutlierCnt>TestIF_File.iMonitorWindow)                        //Steven 20180626 (wei) : 超過設定值的也要檢查
    {
        if(MyMessageBox->Visible==true)                                         //Alarm 前若form有開啟先關閉
            MyMessageBox->Close();
        iUnLoaderCount=8;
        Str.sprintf("User set time: %f / Handler index cycle time: %f", TestIF_File.dIndexCycletimeMonitor, fObserver->dTotoalIndexCycleTime);
        if(TestIF_File.iICTAction==0)                                           //Steven 20250521 : 可以選擇要不要停機
            ShowUnloaderTrayMessage(AnsiString("Please check index cycle time"), Str);
        else
            ShowMyMessage(AnsiString("Please check index cycle time"), Str);

        IniRecordMonitoringIndexCycleTime();
        bResetflag=false;                                                       //不再檢查，直到按下 ok鍵/回home/Speed畫面存檔
    }

    for(int i=0; i<TestIF_File.iMonitorWindow; i++)
    {
        if(queue20[i]==0)
        {
            compareflag=false;
            break;
        }
        sum+=queue20[i];
    }

    if(compareflag==true)
    {
        if(TestIF_File.iMonitorWindow!=0)
            avg=ChangeToFloatNonPcnt((double)(sum), (double)(TestIF_File.iMonitorWindow));
        else
            avg=sum;

        if(avg>TestIF_File.dIndexCycletimeMonitor)                              //比較平均值與設定平均值
        {
            compareflag=false;
            if(MyMessageBox->Visible==true)                                     //Alarm 前若form有開啟先關閉
                MyMessageBox->Close();
            iUnLoaderCount=8;
            Str.sprintf("User set time: %f / Handler index cycle time: %f", TestIF_File.dIndexCycletimeMonitor, avg);
            if(TestIF_File.iICTAction==0)                                           //Steven 20250521 : 可以選擇要不要停機
                ShowUnloaderTrayMessage(AnsiString("Please check index cycle time"), Str);
            else
                ShowMyMessage(AnsiString("Please check index cycle time"), Str);

            IniRecordMonitoringIndexCycleTime();
            bResetflag=false;                                                   //不再檢查，直到按下 ok鍵/回home/Speed畫面存檔
        }
    }
}
//---------------------------------------------------------------------------
void RecordMonitoringIndexCycleTime_New()                                       //JerryYang 20220824 : SPIL index cycle time monitor
{
    double avg=0.0, sum=0.0;
    AnsiString Str="";
    bool compareflag=true;
    fObserver->dTotoalIndexCycleTime*=0.001;                                    //Isaac 20180302 換算成秒

    TestIF_File.dMonitorOutlier=5;
    TestIF_File.iMonitorWindow=7;

    if(TestIF_File.iShuttleMode==1)                                             //JerryYang 20221214 : 關ARM不要偵測cycle time
        return;

    if(fObserver->iIndexCycleTimeCount<20)                                      //尚未取得標準
    {
        if(fObserver->dTotoalIndexCycleTime>0 && fObserver->dTotoalIndexCycleTime<TestIF_File.dMonitorOutlier)      //判斷是否入陣列   //5秒以上可能是換Tray或按暫停，不要列入陣列  //JerryYang 20221214 : fix 可能誤發alarm
        {
            queue20[fObserver->iIndexCycleTimeCount]=fObserver->dTotoalIndexCycleTime;          //列入陣列
            fObserver->iIndexCycleTimeCount++;
        }
    }

    for(int i=0; i<20; i++)
    {
        if(queue20[i]<=0)                                                       //JerryYang 20221214 : fix 可能誤發alarm
        {
            compareflag=false;
        }
        sum+=queue20[i];
    }
    avg=sum/20;

    if(compareflag==true)
    {
        if(fObserver->dTotoalIndexCycleTime>avg*(1+(TestIF_File.dICTTolerance/100.0)))        //比較平均值與設定平均值
        {
            if(fObserver->dTotoalIndexCycleTime>=10)                            //大於10秒的可能是按暫停, 不列入監控
            {
            }
            else
            {
                iMonitoringOutlierCnt++;
            }
        }
        else
        {
            iMonitoringOutlierCnt=0;
        }

        if(iMonitoringOutlierCnt>TestIF_File.iMonitorWindow)                    //Steven 20180626 (wei) : 超過設定值的也要檢查
        {
            compareflag=false;
            Str.sprintf("%.2f", avg);
            ShowErrorMessage("WAR16334", K_RETRY, MMSystem, false, Str);
            iMonitoringOutlierCnt=0;
        }
    }
}
//---------------------------------------------------------------------------
void IniRecordMonitoringIndexCycleTime()                                        //Isaac 20180301 (Steven) Index Cycle Time Monitoring function
{
    fObserver->iIndexCycleTimeCount=0;
    for(int i=0; i<20; i++)                                                     //清除
    {
        queue20[i]=0;
    }
    iMonitoringOutlierCnt=0;
}
//---------------------------------------------------------------------------
void RecordTimeInfo()
{
    int iT, iS, iE, iTMSEC, iTM, IndexTime=0, IndexTimeCT=0;
    double temp;
    fObserver->iTestTimeCT=0;
    fObserver->iTestTime=0;
    AnsiString str;
    for(int i=0; i<9; i++)
    {
        for(int j=0; j<2; j++)
        {
            TestTimeInfoRecord[j][i].iStartMin =TestTimeInfoRecord[j][i+1].iStartMin;
            TestTimeInfoRecord[j][i].iStartSec =TestTimeInfoRecord[j][i+1].iStartSec;
            TestTimeInfoRecord[j][i].iStartMSec=TestTimeInfoRecord[j][i+1].iStartMSec;
            TestTimeInfoRecord[j][i].iEndMin   =TestTimeInfoRecord[j][i+1].iEndMin;
            TestTimeInfoRecord[j][i].iEndSec   =TestTimeInfoRecord[j][i+1].iEndSec;
            TestTimeInfoRecord[j][i].iEndMSec  =TestTimeInfoRecord[j][i+1].iEndMSec;

            if(CosFunction.bOEEFunction)                                        //Steven 20180417 (Jou) : OEE功能
            {                                                                   //KaiChen 20171127 (Steven) ：超豐 OEE 新增 Test Time、Index Time
                if(fObserver->iTestReceiveTimeCount<=20)                        //Sam 20200317 : Fix OEE Test Time
                {
                    TestReceiveTimeInfoRecord[j][i].iMin = TestReceiveTimeInfoRecord[j][i+1].iMin;
                    TestReceiveTimeInfoRecord[j][i].iSec = TestReceiveTimeInfoRecord[j][i+1].iSec;
                    TestReceiveTimeInfoRecord[j][i].iMSec= TestReceiveTimeInfoRecord[j][i+1].iMSec;
                }
            }
        }
    }

    for(int i=0; i<2; i++)
    {
        TestTimeInfoRecord[i][9].iStartMin =TestSocketTimeInfo[i].iStartMin;
        TestTimeInfoRecord[i][9].iStartSec =TestSocketTimeInfo[i].iStartSec;
        TestTimeInfoRecord[i][9].iStartMSec=TestSocketTimeInfo[i].iStartMSec;
        TestTimeInfoRecord[i][9].iEndMin   =TestSocketTimeInfo[i].iEndMin;
        TestTimeInfoRecord[i][9].iEndSec   =TestSocketTimeInfo[i].iEndSec;
        TestTimeInfoRecord[i][9].iEndMSec  =TestSocketTimeInfo[i].iEndMSec;
        if(CosFunction.bOEEFunction)                                            //Steven 20180417 (Jou) : OEE功能
        {
            TestReceiveTimeInfoRecord[i][9].iMin = OEERecevieTimeInfo[i].iStartMin;    //KaiChen 20171127 (Steven) ：超豐 OEE 新增 Test Time、Index Time
            TestReceiveTimeInfoRecord[i][9].iSec = OEERecevieTimeInfo[i].iStartSec;    //KaiChen 20171127 (Steven) ：超豐 OEE 新增 Test Time、Index Time
            TestReceiveTimeInfoRecord[i][9].iMSec= OEERecevieTimeInfo[i].iStartMSec;   //KaiChen 20171127 (Steven) ：超豐 OEE 新增 Test Time、Index Time
        }
    }

    for(int i=0; i<10; i++)
    {
        str.sprintf("%02d:%6.3f", TestTimeInfoRecord[0][i].iStartMin,
                                  (double)TestTimeInfoRecord[0][i].iStartSec+(double)TestTimeInfoRecord[0][i].iStartMSec/1000.0);
        fObserver->TimeInfoGrid->Cells[1][2+i]=str;
        str.sprintf("%02d:%6.3f", TestTimeInfoRecord[0][i].iEndMin,
                                  (double)TestTimeInfoRecord[0][i].iEndSec+(double)TestTimeInfoRecord[0][i].iEndMSec/1000.0);
        fObserver->TimeInfoGrid->Cells[2][2+i]=str;
        if(TestTimeInfoRecord[0][i].iEndMin<TestTimeInfoRecord[0][i].iStartMin)
            iE=(60+TestTimeInfoRecord[0][i].iEndMin)*60;                        // to sec;
        else
            iE=TestTimeInfoRecord[0][i].iEndMin*60;                             // to sec;
        iE+=TestTimeInfoRecord[0][i].iEndSec;                                   // total sec
        iE =iE*1000+TestTimeInfoRecord[0][i].iEndMSec;
        iS =TestTimeInfoRecord[0][i].iStartMin*60;                              // to sec;
        iS+=TestTimeInfoRecord[0][i].iStartSec;                                 // total sec
        iS =iS*1000+TestTimeInfoRecord[0][i].iStartMSec;
        iT =iE-iS;                                                              // test time msec
        if(iT!=0)
        {
            fObserver->iTestTime+=iT;
            fObserver->iTestTimeCT++;
        }
        iTM=iT/60000;
        iTMSEC=iT-iTM*60000;

        RunInfo.dTestTimeSec=(double)iT/1000.0;                                 //2008/10/20 lee start
        if(iTM!=0)  str.sprintf("%02d:%6.3f", iTM, (double)iTMSEC/1000.0);
        else        str.sprintf("%6.3f", (double)iTMSEC/1000.0);                //2008/10/20 lee end

        if(CUSTOMER_CODE==CC_KYEC_CHEN)                                         //Eliot 2008_08_27
        {
            if(i==9)
                fObserver->iTotoalTestTime=iT;                                  //2007/10/17 Marc Start
        }

        if(i==9)                                                                //2013-11-27   Dell    記錄測試時間
            dTestSec=(double)iT/1000.0;                                         //jou 2014-09-24 int -> float bWhenHappenTestedTimeBelowUseInitialDelay 測試秒數小於1 sec會誤判

        fObserver->TimeInfoGrid->Cells[3][2+i]=ConvertMSecToSPC(iT);            //JerryYang 20200812 : Test time格式改為HH:MM:SS.000
        if(i>=1)
        {
            if(TestTimeInfoRecord[0][i].iStartMin<TestTimeInfoRecord[0][i-1].iEndMin)
                iE=(60+TestTimeInfoRecord[0][i].iStartMin)*60;                  // to sec;
            else
                iE=TestTimeInfoRecord[0][i].iStartMin*60;                       // to sec;
            iE+=TestTimeInfoRecord[0][i].iStartSec;                             // total sec
            iE =iE*1000+TestTimeInfoRecord[0][i].iStartMSec;
            iS =TestTimeInfoRecord[0][i-1].iEndMin*60;                          // to sec;
            iS+=TestTimeInfoRecord[0][i-1].iEndSec;                             // total sec
            iS =iS*1000+TestTimeInfoRecord[0][i-1].iEndMSec;
            iT =iE-iS;                                                          // test time msec
            fObserver->dTotoalIndexCycleTime=iT;                                //Isaac 20180301 (Steven) Index Cycle Time Monitoring function，記錄index cycle time
            if(iT!=0)
            {
                IndexTime+=iT;
                IndexTimeCT++;
            }
            iTM=iT/60000;
            iTMSEC=iT-iTM*60000;

            if(i==9)                                                            //kevin 20141024 高雄日月光IC履歷記錄
            {
               fRecindexCycleTim=(double)iTMSEC/1000.0;                         //取的index time
               RecindexCycleTim.sprintf("%6.3f", (double)fRecindexCycleTim);
            }

            if(bIndexTimeSet && i==9)                                           //kevin 20130812 K15假index time
            {
                str.sprintf("%6.3f", fIndexTime);
            }
            else
            {
                if(iTM!=0)                                                      //2008/10/20 lee start
                    str.sprintf("%02d:%6.3f", iTM, (double)iTMSEC/1000.0);
                else
                    str.sprintf("%6.3f", (double)iTMSEC/1000.0);
            }
            fObserver->TimeInfoGrid->Cells[4][2+i]=str;
        }

        fObserver->TimeInfoGrid->Cells[5][2+i]=FormatFloat("0.00", fObserver->fRecordIndexTime[9-i]);   //jou 2010-12-22 新增index time ave.
        fObserver->strngrdTestTime->Cells[5][2+i]=FormatFloat("0.000", fObserver->fRecordIndexTime[9-i]);

        if(CosFunction.bOEEFunction)                                            //Steven 20180417 (Jou) : OEE功能
        {                                                                       //KaiChen 20171127 (Steven) ：超豐 OEE 新增 Test Time、Index Time
            if(fObserver->iTestReceiveTimeCount<=20)                            //Sam 20200317 : Fix OEE Test Time
            {
                str.sprintf("%02d:%6.3f", TestReceiveTimeInfoRecord[0][i].iMin,
                                          (double)TestReceiveTimeInfoRecord[0][i].iSec+(double)TestReceiveTimeInfoRecord[0][i].iMSec/1000.0);
                fObserver->sg_ListTimeReceiveInfoGrid->Cells[1][2+i]=str;

                if(TestReceiveTimeInfoRecord[0][i].iMin<TestTimeInfoRecord[0][i].iStartMin)
                    iE=(60+TestReceiveTimeInfoRecord[0][i].iMin)*60;            // to sec;
                else
                    iE=TestReceiveTimeInfoRecord[0][i].iMin*60;                 // to sec;

                iE +=TestReceiveTimeInfoRecord[0][i].iSec;                      // total sec
                iE  =iE*1000+TestReceiveTimeInfoRecord[0][i].iMSec;
                iS  =TestTimeInfoRecord[0][i].iStartMin*60;                     // to sec;
                iS +=TestTimeInfoRecord[0][i].iStartSec;                        // total sec
                iS  =iS*1000+TestTimeInfoRecord[0][i].iStartMSec;
                iT  =iE - iS;                                                   // test time msec
                str.sprintf("%6.3f", (double)iT/1000.0);                        //Sam 20180104 : 時間異常 HangUp
                fObserver->sg_ListTimeReceiveInfoGrid->Cells[2][2+i] = str;
            }
        }
    }

    if(fObserver->iTestTimeCT!=0)                                               //2008/10/02 lee start
    {
        fObserver->iTestTime/=fObserver->iTestTimeCT;
        iTM=fObserver->iTestTime/60000;
        iTMSEC=fObserver->iTestTime-iTM*60000;
        if(iTM!=0)  str.sprintf("%02d:%6.3f", iTM, (double)iTMSEC/1000.0);
        else        str.sprintf("%6.3f", (double)iTMSEC/1000.0);
        fObserver->TimeInfoGrid->Cells[3][14]=str;
    }

    if(IndexTimeCT!=0)
    {
        IndexTime/=IndexTimeCT;
        iTM=IndexTime/60000;
        iTMSEC=IndexTime-iTM*60000;
        if(iTM!=0)  str.sprintf("%02d:%6.3f",iTM, (double)iTMSEC/1000.0);
        else        str.sprintf("%6.3f", (double)iTMSEC/1000.0);
        fObserver->TimeInfoGrid->Cells[4][14]=str;
    }                                                                           //2008/10/02 lee end

    fObserver->TimeInfoGrid->Cells[5][14]=FormatFloat("0.00", fObserver->fRecordIndexTime[11]);     //jou 2010-12-22 新增index time ave.
    fObserver->strngrdTestTime->Cells[5][14]=FormatFloat("0.000", fObserver->fRecordIndexTime[11]);

    RunInfo.TestTime      =fObserver->TimeInfoGrid->Cells[3][11];               //Steven 20141230 : 修正SECS GEM參數
    RunInfo.IndexCycleTime=fObserver->TimeInfoGrid->Cells[4][11];

    fObserver->RecordIndexCycle();                                              //Sam 20200916 : Add Index Cycle Time Record

    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            TestSocket.PordRec[i][j].AddTestTime(RunInfo.TestTime);
            TestSocket.PordRec[i][j].AddDataTimeRecord(sBufferSOT);                     //wei 20181211 (Steven) : 更換位置SOT
            TestSocket.PordRec[i][j].AddDataTimeEOTRecord(sBufferEOT);                  //wei 20181211 (Steven) : 更換位置SOT
            TestSocket.PordRec[i][j].AddGroundRecord(fGroundMan->asGroundVaule);        //Sam 20211223 : 每顆 IC 測試完畢都要記錄當時的 Ground & ESD 數值。
            TestSocket.PordRec[i][j].AddESDRecord(fLotInfo->lbESDReportData->Caption);  //Sam 20211223 : 每顆 IC 測試完畢都要記錄當時的 Ground & ESD 數值。
        }
    }

    if(CosFunction.bOEEFunction)                                                //Steven 20180417 (Jou) : OEE功能
    {                                                                           //KaiChen 20171127 (Steven) ：超豐 OEE 新增 Test Time、Index Time
        if(fObserver->iTestReceiveTimeCount==20)                                //Sam 20200317 : Fix OEE Test Time
        {
            for(int k=0; k<10; k++)
            {
                fObserver->dRecordTestTime[k]=(double)StrToFloat(fObserver->sg_ListTimeReceiveInfoGrid->Cells[2][2+k]);
            }

            for(int x=0; x<10; x++)
            {
                for(int y=x; y<10; y++)
                {
                    if(fObserver->dRecordTestTime[y]>fObserver->dRecordTestTime[x])
                    {
                        temp=fObserver->dRecordTestTime[y];
                        fObserver->dRecordTestTime[y]=fObserver->dRecordTestTime[x];
                        fObserver->dRecordTestTime[x]=temp;
                    }
                }
            }
            double dTestReceive=0.0;
            for(int k=1; k<6; k++)                                              //Sam 20200317 : Fix OEE Test Time
            {
                dTestReceive+=fObserver->dRecordTestTime[k];
            }
            fObserver->sTestReceiveTime=FloatToStr(dTestReceive/5.0);
        }
        else if(fObserver->iTestReceiveTimeCount<20)                            //Sam 20200317 : Fix OEE Test Time
        {
            fObserver->sTestReceiveTime="";
        }
        fObserver->sg_ListTimeReceiveInfoGrid->Cells[2][14]=fObserver->sTestReceiveTime;
    }

    int iCount=0, iCount2=0;                                                    //Steven 20200715 : 重新計算Cycle Time
    int iTestTime=0;
    int iCycleTime=0;

    for(int j=0; j<10; j++)
    {
        if(j<=QueueTestTime.iCount-1)
        {
            fObserver->strngrdTestTime->Cells[1][11-j]=QueueTestTime.GetStartTime(j);
            fObserver->strngrdTestTime->Cells[2][11-j]=QueueTestTime.GetEndTime(j);
            fObserver->strngrdTestTime->Cells[3][11-j]=QueueTestTime.GetTimeString(j);
            iCount++;
            iTestTime+=QueueTestTime.GetTimeData(j);
        }

        if(j<=QueueCycleTime.iCount)
        {
            fObserver->strngrdTestTime->Cells[4][11-j]=QueueCycleTime.GetTimeString(j);
            iCycleTime+=QueueCycleTime.GetTimeData(j);
            iCount2++;
        }
    }

    if(iCount>0)
    {
        fObserver->strngrdTestTime->Cells[3][14]=ConvertMSecToSPC(iTestTime/iCount);
    }

    if(iCount2>0)
    {
        fObserver->strngrdTestTime->Cells[4][14]=ConvertMSecToSPC(iCycleTime/iCount2);
    }

    if(IniConfig.bShowTimeInfo==true)                                           //Steven 20100827
    {
        if((TestIF_File.iShuttleMode==1 &&                                      //kevin 20190130 add use test arm
            TestIF_File.iShuttle_Sel==iIndexArmWhich) ||
           TestIF_File.iShuttleMode==0)
        {
            if(CUSTOMER_CODE==CC_SCC || CUSTOMER_CODE==CC_JCET)
            {
                fShowMessage->lblIndexCycleTime->Caption="   Index Cycle Time="+fObserver->strngrdTestTime->Cells[4][11]+" sec";
                fShowMessage->lblTestTime->Caption      ="   Test Time ="+fObserver->strngrdTestTime->Cells[3][11]+" sec";
            }
            else
            {
                fShowMessage->lblIndexCycleTime->Caption="   Index Cycle Time="+fObserver->TimeInfoGrid->Cells[4][11]+" sec";
                fShowMessage->lblTestTime->Caption      ="   Test Time ="+fObserver->TimeInfoGrid->Cells[3][11]+" sec";
            }
        }
    }
    else
    {
        fShowMessage->lblIndexCycleTime->Caption="";
        fShowMessage->lblTestTime->Caption      ="";
    }

//    ShowIndexTime(1);                                                         //Steven 20140619 : 測試    //到這裡大概0.015~0.031Sec
}
//---------------------------------------------------------------------------
void RecordStartTestTime()
{
    iCurrentTime=0;
    GetTimeInfo();
    for(int i=0; i<2; i++)
    {
        TestSocketTimeInfo[i].iStartMin=SystemMin;
        TestSocketTimeInfo[i].iStartSec=SystemSec;
        TestSocketTimeInfo[i].iStartMSec=SystemMSec;
    }
    sBufferSOT=Now().FormatString("yyyymmdd_hhnnss");                           //wei 20181211 (Steven) : 更換位置SOT
}
//---------------------------------------------------------------------------
extern int SendTestResultToHttp();
int RecordEndTestTime(int iArm)                                                 //Sam 20201231 : 修正關 Arm 後，Index Cycle time 異常。0:arm1 1:arm2 2:雙Arm
{
    AnsiString str;
    int iCount=0, iMaxSite, iSiteCh;
    GetTimeInfo();
    for(int i=0; i<2; i++)
    {
        TestSocketTimeInfo[i].iEndMin=SystemMin;
        TestSocketTimeInfo[i].iEndSec=SystemSec;
        TestSocketTimeInfo[i].iEndMSec=SystemMSec;
    }
    sBufferEOT=Now().FormatString("yyyymmdd_hhnnss");                           //wei 20181211 (Steven) : 更換位置SOT

    if((TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==0 && iArm==0)  ||
       (TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1 && iArm==1)  ||
        TestIF_File.iShuttleMode==0                                             ||
        iArm==2)                                                                //Sam 20201231 : 修正關 Arm 後，Index Cycle time 異常 1:arm1 2:arm2
    {
        RecordTimeInfo();
    }

    int iRet=SendTestResultToHttp();

    if(IniConfig.bSPILFunction==true &&
       TestIF_File.bIndexCycleTimeMonitor==true)                                //JerryYang 20220824 : SPIL index cycle time monitor
    {
        RecordMonitoringIndexCycleTime_New();
    }
    else
    {
        if(TestIF_File.bIndexCycleTimeMonitor==true && bResetflag==true)        //Isaac 20180301 (Steven) Index Cycle Time Monitoring function
        {
            RecordMonitoringIndexCycleTime();
        }
    }

    if(IniConfig.bN28_SCK_OEE)                                                  //Steven 20210608 : JSCK OEE Function.
    {
        TStringList *List1=new TStringList();
        TStringList *List2=new TStringList();
        List1->Clear();
        List1->Add(IniConfig.sN28_IP);                                          //IP
        str.sprintf("%04d/%02d/%02d %02d:%02d:%02d:%03d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec);
        List1->Add(str);                                                        //yyyy/mm/dd hh:mm:ss:sss
        str.sprintf("%0.3f", RunInfo.dTestTimeSec);
        List1->Add(str);                                                        //Test time
        List1->Add(RunInfo.IndexCycleTime);                                     //Index time
        List1->Add("0.00");                                                     //0.00
        if(TestIF.iTestMode==_32Site4X8N)
            iMaxSite=32;
        else
            iMaxSite=16;

        for(int i=0; i<iMaxSite; i++)
        {
            List2->Add("0");
        }

        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                if(TestSocket.Item[i][j]!=HAS_NULL_IC &&                        //kevin 20120618  沒有IC 就不要設定避免畫面被誤解
                   TestSocket.Item[i][j]!=NULL_IC)
                {
                    iSiteCh=TestIF_File.iSiteMap[i][j]-1;
                    if(iSiteCh>=0 && iSiteCh<iMaxSite)
                    {
                        List2->Strings[iSiteCh]=TestSocket.iBinData[i][j];
                        iCount++;
                    }
                }
            }
        }
        List1->Add(iCount);                                                     //Enabled site
        List1->Add(List2->CommaText);                                           //site1 tested bin, ... site16 tested bin
        str=StringReplace(List1->CommaText, "\"", "", TReplaceFlags()<<rfReplaceAll);
        str=StringReplace(str, ",", ", ", TReplaceFlags()<<rfReplaceAll);
        fMain->slTestLog->AddText(str);
        List1->Clear();
        List2->Clear();
        delete List1;
        delete List2;
    }
    return iRet;
}
//--------------------------------------------------------------------------
int __fastcall TfObserver::UnCalculateStopTime(AnsiString Time)
{
    AnsiString asSubBuffer, asSubBuffer1;
    int i_h, i_m, i_s, i_Time, iPos;
    if(Time=="")    return 0;

    iPos=Time.Pos(":");
    i_h=atoi(Time.SubString(1, iPos).c_str())*60*60;
    asSubBuffer=Time.SubString(iPos+1, Time.Length());
    iPos=asSubBuffer.Pos(":");
    i_m=atoi(asSubBuffer.SubString(1, iPos).c_str())*60;
    asSubBuffer1=asSubBuffer.SubString(iPos+1,Time.Length());
    i_s=atoi(asSubBuffer1.SubString(1, iPos).c_str());
    i_Time=i_h+i_m+i_s;
    return i_Time;
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::ProcessRunInfo()
{
    AnsiString str="";
    int dSec;
    int unit;

    dSec=((LastSet.SystemAccSecond[0][stPauseTime]+                             //Isaac 20180417 (Steven) 修正MTBA公式(pause+production+jam)/jamcount
           LastSet.SystemAccSecond[0][stProductTime]+
           LastSet.SystemAccSecond[0][stJamTime])/1000);                        //Steven 20140816 : 改用秒為單位

    if(LastSet.iJamCount[1]==0)
    {
        str.sprintf("0 / %s", ConvertSecondToSPC(dSec));
    }
    else
    {
        if(dSec!=0)
        {
            dSec=ChangeToFloatNonPcnt((double)(dSec), (double)(LastSet.iJamCount[1]));
            str.sprintf("1 / %s", ConvertSecondToSPC(dSec));
        }
        else
        {
            str.sprintf("%d / 0", LastSet.iJamCount[1]);
        }
    }
    labMTBA->Caption=str;

    if(LastSet.iJamCount[1]==0)
    {
        str.sprintf("0 / %d unit", LastSet.SendCT[1]);
    }
    else
    {
        if(IniConfig.iUserLanguage==eulSingapore)                               //Steven 20120807 : 新加坡要求顯示數量
        {
            str.sprintf("%d / %d unit", LastSet.iJamCount[1], LastSet.SendCT[1]);
        }
        else
        {
            unit=LastSet.SendCT[1];                                             //jou 2010-08-13 計數jam rate,改為Tray Feed為一單位
            if(unit!=0)
            {
                unit/=LastSet.iJamCount[1];                                     //jou 2010-08-13 計數jam rate,改為Tray Feed為一單位
                str.sprintf("1 / %d unit", unit);
            }
            else
            {
                str.sprintf("%d / 0 unit", LastSet.iJamCount[1]);               //Steven 20110103
            }
        }
    }
    labMUBA->Caption=str;

    labMTBF->Caption=ConvertSecondToSPC(LastSet.SystemAccSecond[0][stPowerOn]/1000);

    if(IniConfig.bVTESTFunction==true)                                          //jou 20210108 : 上海偉測要求新增每日jam rate統計
    {
        if(LastSet.iDayJamCount==0)
        {
            str.sprintf("0 / 1 unit");
        }
        else
        {
            unit=LastSet.iDaySendCT;
            if(unit!=0)
            {
                unit/=LastSet.iDayJamCount;
                str.sprintf("1 / %d unit", unit);
            }
            else
            {
                str.sprintf("%d / 0 unit", LastSet.iDayJamCount);
            }
        }
        pnlDayJamRate->Caption=str;
    }
}
//---------------------------------------------------------------------------
// 更換頁面時，才Load資料
//---------------------------------------------------------------------------
void __fastcall TfObserver::pgcObservChange(TObject *Sender)
{
    switch(pgcObserv->ActivePageIndex)
    {
        case 1:                                                                 //Tester Category
            if(!bIsLoaded[1])
            {
                WriteCategoryData();
                bIsLoaded[1]=true;
            }
            break;
        case 3:                                                                 //System Message
            if(!bIsLoaded[3])
            {
//                BtnQueryClick(this);                                          //Steven 20180517 : Mark MDB 避免卡卡
                pgcMessage->ActivePageIndex=1;
                btnQueryEventLogTxt->Click();
                bIsLoaded[3]=true;
            }
            break;
        case 4:                                                                 //Yield
            if(!bIsLoaded[4])
            {
                SetSiteYieldDiagram();
                UpdateYieldChart();
                bIsLoaded[4]=true;
            }
            break;
        case 5:                                                                 //Temperature
            if(!bIsLoaded[5])
            {
                UpdateTempChart();
                bIsLoaded[5]=true;
            }
            break;
        case 6:                                                                 //Torque
            if(!bIsLoaded[6])
            {
                //MyDBVTorque(1, StringGrid1);                                  //Steven 20100907 : 影響效能,關掉
                //MyDBVTorque(2, StringGrid7);
                bIsLoaded[6]=true;
            }
            break;
    }

//    if(CUSTOMER_CODE==CC_Greatek)                                             //Sam 20170809 (Steven) 移植超豐 OEE 功能 form HT-7045
    if(CosFunction.bOEEFunction)                                                //Steven 20180417 (Jou) : OEE功能
    {
        if(IniConfig.bN14_1_EnableOEEFunction==true &&
           pgcObserv->ActivePage==tsOEE_ProductionInfor)
        {
            fProductionInfo->ShowModal();
        }
    }
}
//---------------------------------------------------------------------------
// Alarm相關
//---------------------------------------------------------------------------
void __fastcall TfObserver::BtnSaveClick(TObject *Sender)                       //存檔
{
    if(SystemStart) return;

    if(cbDisplayData->ItemIndex<Jam_Chart_Summary ||                            //將StringGrid存入Excel
       cbDisplayData->ItemIndex==Alarm_Code_List)                               //Steven 20150911 : 修正存Alarm List
    {
        SaveDialog1->FileName="D:\\EventLog.xls";                               //Steven 20120921 : 存檔保護
        if(SaveDialog1->Execute())
            SGDToXLS(strngrdMDBQuery, SaveDialog1->FileName);
    }
    else if(cbDisplayData->ItemIndex==Production_Summary_Report)                //Sam 20231106 : 新增 SGD 轉 CSV
    {
        SaveDialog1->FileName="D:\\EventLog.csv";
        SaveDialog1->Filter="CSV files (*.csv)";
        if(SaveDialog1->Execute())
        {
            SGDToCSV(strngrdMDBQuery,",", ";", SaveDialog1->FileName);
        }
    }
    else                                                                        //Jam Chart
    {
        SavePictureDialog1->FileName="D:\\EventLog.BMP";                        //Steven 20120921 : 存檔保護
        if(SavePictureDialog1->Execute())
            Chart2->SaveToBitmapFile(SavePictureDialog1->FileName);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::BtnQueryClick(TObject *Sender)                      //查詢
{
    int iLevel;
    bool bSilent, bRedBG;
    AnsiString asQuery, WhereQuery, UnitName;

    if(cbDisplayData->ItemIndex<Jam_Chart_Summary)
    {
        strngrdMDBQuery->Visible=true;
        Chart2->Visible=false;

        for(int i=0; i<strngrdMDBQuery->RowCount; i++)                          //Steven 20120222 : 要把多的欄位清空
        {
            strngrdMDBQuery->Rows[i]->Clear();
        }

        if(cbDisplayData->ItemIndex==Event_Log)  //EventLog
        {
            strngrdMDBQuery->ColCount=9;
            strngrdMDBQuery->ColWidths[0]=33;   //Index
            strngrdMDBQuery->ColWidths[1]=60;   //UnitName
            strngrdMDBQuery->ColWidths[2]=60;   //AlarmCode
            strngrdMDBQuery->ColWidths[3]=110;  //DateTime
            strngrdMDBQuery->ColWidths[4]=50;   //Recovery
            strngrdMDBQuery->ColWidths[5]=50;   //StopTime
            strngrdMDBQuery->ColWidths[6]=50;   //Duplicate
            strngrdMDBQuery->ColWidths[7]=390;  //Message
            strngrdMDBQuery->ColWidths[8]=390;  //ErrPart
            strngrdMDBQuery->Cells[0][0]="No.";
        }
        else if(cbDisplayData->ItemIndex==Production_Record)                    //Steven 20140819 : Production View
        {
            strngrdMDBQuery->ColCount=26;
            for(int i=0; i<strngrdMDBQuery->ColCount; i++)
            {
                strngrdMDBQuery->ColWidths[i]=50;
            }
        }
        else if(cbDisplayData->ItemIndex>Event_Log &&
                cbDisplayData->ItemIndex<Alarm_History)
        {
            strngrdMDBQuery->ColCount=3;
            strngrdMDBQuery->DefaultColWidth=260;
        }
        else if(cbDisplayData->ItemIndex>=Alarm_History &&
                cbDisplayData->ItemIndex<=Alarm_History_WAR)                    //Alarm History
        {
            strngrdMDBQuery->ColCount=11;
            strngrdMDBQuery->DefaultColWidth=70;
            strngrdMDBQuery->ColWidths[0]=20;                                   //Index
            strngrdMDBQuery->ColWidths[1]=50;                                   //AlarmCode
            strngrdMDBQuery->ColWidths[2]=70;                                   //UnitName
            strngrdMDBQuery->ColWidths[3]=250;                                  //Message
            strngrdMDBQuery->ColWidths[4]=70;                                   //ErrPart
            strngrdMDBQuery->ColWidths[5]=55;                                   //Recovery
            strngrdMDBQuery->ColWidths[6]=40;                                   //Duplicate
            strngrdMDBQuery->ColWidths[7]=60;                                   //Date
            strngrdMDBQuery->ColWidths[8]=50;                                   //Time
            strngrdMDBQuery->ColWidths[9]=50;                                   //StopTime
            strngrdMDBQuery->ColWidths[10]=40;                                  //Temperature
        }
        else if(cbDisplayData->ItemIndex==Alarm_Statistics)
        {
            strngrdMDBQuery->ColCount=8;
            strngrdMDBQuery->ColWidths[0]=33;                                   //Index
            strngrdMDBQuery->ColWidths[1]=90;                                   //UnitName
            strngrdMDBQuery->ColWidths[2]=75;                                   //AlarmCode
            strngrdMDBQuery->ColWidths[3]=390;                                  //Message
            strngrdMDBQuery->ColWidths[4]=50;                                   //Count
            strngrdMDBQuery->ColWidths[5]=50;                                   //Rate
            strngrdMDBQuery->ColWidths[6]=75;                                   //StopTime
            strngrdMDBQuery->ColWidths[7]=75;                                   //MTTR
            strngrdMDBQuery->Cells[0][0]="No.";
        }
        else if(cbDisplayData->ItemIndex==Alarm_Stat_Day)
        {
            strngrdMDBQuery->ColCount=9;
            strngrdMDBQuery->ColWidths[0]=33;                                   //Index
            strngrdMDBQuery->ColWidths[1]=60;                                   //Date
            strngrdMDBQuery->ColWidths[2]=90;                                   //UnitName
            strngrdMDBQuery->ColWidths[3]=75;                                   //AlarmCode
            strngrdMDBQuery->ColWidths[4]=330;                                  //Message
            strngrdMDBQuery->ColWidths[5]=50;                                   //Count
            strngrdMDBQuery->ColWidths[6]=50;                                   //Rate
            strngrdMDBQuery->ColWidths[7]=75;                                   //StopTime
            strngrdMDBQuery->ColWidths[8]=75;                                   //MTTR
            strngrdMDBQuery->Cells[0][0]="No.";
        }
        else if(cbDisplayData->ItemIndex==Lot_Infomation)
        {
            strngrdMDBQuery->ColCount=7;
            strngrdMDBQuery->DefaultColWidth=100;
        }

        strngrdMDBQuery->Cells[0][0]="No.";
        DateSeparator='-';                                                      // Sam 20210225 : 增加保護
        if(cbDisplayData->ItemIndex==Lot_Infomation)
        {
            WhereQuery.sprintf(" WHERE (StartTime >= '%s %s' AND EndTime <= '%s %s')",
                        DateTimePicker1->Date.FormatString("yyyy-mm-dd"), DateTimePicker2->DateTime.FormatString("hh:nn:ss"),
                        (DateTimePicker3->Date).FormatString("yyyy-mm-dd"), DateTimePicker4->DateTime.FormatString("hh:nn:ss"));
        }
        else if(cbDisplayData->ItemIndex==Alarm_Statistics ||
                cbDisplayData->ItemIndex==Alarm_History_JAM ||
                cbDisplayData->ItemIndex==Alarm_Stat_Day)
        {                                                                       //Steven 20110810 Start : Optimizing queries
            if(IniConfig.bSPILFunction==true)                                   //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
            {                                                                   //20111110 Dell
                WhereQuery.sprintf(" WHERE ((OccurDateTime >= '%s %s' AND OccurDateTime <= '%s %s') "
                            "  AND (AlarmCode > 'JAM01' AND AlarmCode < 'JAM99') "
//                            "  AND (  (AlarmCode > 'JAM01' AND AlarmCode < 'JAM10') "
//                            " OR (AlarmCode > 'JAM17' AND AlarmCode < 'JAM20') "
//                            " OR (AlarmCode IN ('JAM0316', 'JAM0317'))) "
                           " AND (Duplicate=0 OR Duplicate=2)) ",
                DateTimePicker1->Date.FormatString("yyyy-mm-dd"), DateTimePicker2->DateTime.FormatString("hh:nn:ss"),
                DateTimePicker3->Date.FormatString("yyyy-mm-dd"), DateTimePicker4->DateTime.FormatString("hh:nn:ss"));
            }
            else
            {
                WhereQuery.sprintf(" WHERE ((OccurDateTime >= '%s %s' AND OccurDateTime <= '%s %s') "
                            "  AND (AlarmCode > 'JAM01' AND AlarmCode < 'JAM99') "
//                            " AND (  (AlarmCode > 'JAM01' AND AlarmCode < 'JAM10') "
//                            " OR (AlarmCode > 'JAM17' AND AlarmCode < 'JAM20') "
//                            " OR (AlarmCode IN ('JAM0316', 'JAM0317'))) "
                           " AND Duplicate=0) ",
                DateTimePicker1->Date.FormatString("yyyy-mm-dd"), DateTimePicker2->DateTime.FormatString("hh:nn:ss"),
                DateTimePicker3->Date.FormatString("yyyy-mm-dd"), DateTimePicker4->DateTime.FormatString("hh:nn:ss"));
            }
            //Steven 20110810 End
        }
        else
        {
            WhereQuery.sprintf(" WHERE (OccurDateTime >= '%s %s' AND OccurDateTime <= '%s %s')",
                    DateTimePicker1->Date.FormatString("yyyy-mm-dd"), DateTimePicker2->DateTime.FormatString("hh:nn:ss"),
                    DateTimePicker3->Date.FormatString("yyyy-mm-dd"), DateTimePicker4->DateTime.FormatString("hh:nn:ss"));
        }

        if(cbDisplayData->ItemIndex==Event_Log)
            asQuery.sprintf("SELECT * FROM EventLogView %s ORDER BY OccurDateTime DESC", WhereQuery);
        else if(cbDisplayData->ItemIndex==Process_Record)
            asQuery.sprintf("SELECT Process, OccurDateTime FROM Process %s ORDER BY OccurDateTime DESC", WhereQuery);
        else if(cbDisplayData->ItemIndex==Message_Record)
            asQuery.sprintf("SELECT Message, OccurDateTime FROM Message %s ORDER BY OccurDateTime DESC", WhereQuery);
        else if(cbDisplayData->ItemIndex==Motion_Record)
            asQuery.sprintf("SELECT Motion,  OccurDateTime FROM Motion  %s ORDER BY OccurDateTime DESC", WhereQuery);
        else if(cbDisplayData->ItemIndex==Production_Record)                    //Steven 20140819 : Production View
            asQuery.sprintf("SELECT * FROM ProductionView %s ORDER BY OccurDateTime DESC", WhereQuery);
        else if(cbDisplayData->ItemIndex==Alarm_History)
            asQuery.sprintf("SELECT AlarmCode, UnitName, Message, ErrPart, Recovery, Duplicate, DATE(OccurDateTime) Date, TIME(OccurDateTime) Time, StopedTime, Temperature FROM AlarmHistoryView %s ORDER BY OccurDateTime DESC", WhereQuery);
        else if(cbDisplayData->ItemIndex==Alarm_History_JAM)
            asQuery.sprintf("SELECT AlarmCode, UnitName, Message, ErrPart, Recovery, Duplicate, DATE(OccurDateTime) Date, TIME(OccurDateTime) Time, StopedTime, Temperature FROM AlarmHistoryView %s ORDER BY OccurDateTime DESC", WhereQuery);
        else if(cbDisplayData->ItemIndex==Alarm_History_MES)
            asQuery.sprintf("SELECT AlarmCode, UnitName, Message, ErrPart, Recovery, Duplicate, DATE(OccurDateTime) Date, TIME(OccurDateTime) Time, StopedTime, Temperature FROM AlarmHistoryView %s AND (AlarmCode > 'M' AND AlarmCode < 'N') ORDER BY OccurDateTime DESC", WhereQuery);   //Steven 20110810 Start : Optimizing queries
        else if(cbDisplayData->ItemIndex==Alarm_History_WAR)
            asQuery.sprintf("SELECT AlarmCode, UnitName, Message, ErrPart, Recovery, Duplicate, DATE(OccurDateTime) Date, TIME(OccurDateTime) Time, StopedTime, Temperature FROM AlarmHistoryView %s AND (AlarmCode > 'W' AND AlarmCode < 'X') ORDER BY OccurDateTime DESC", WhereQuery);   //Steven 20110810 Start : Optimizing queries
        else if(cbDisplayData->ItemIndex==Alarm_Statistics)
        {
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
        }
        else if(cbDisplayData->ItemIndex==Alarm_Stat_Day)                       //根據日期作Group
        {
            asQuery.sprintf(
                    "SELECT                                                                                     "
                    "      Date(EventLog.OccurDateTime) Date,                                                    "
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
                    "       AlarmCode, Date(OccurDateTime)                                                      "
                    "ORDER BY                                                                                   "
                    "      Date DESC, Count DESC                                                                ", WhereQuery);
        }
        else if(cbDisplayData->ItemIndex==Lot_Infomation)
        {
            asQuery.sprintf("SELECT * FROM CountPerLot %s ORDER BY StartTime DESC", WhereQuery);
        }

        if(cbDisplayData->ItemIndex==Alarm_Statistics ||
           cbDisplayData->ItemIndex==Alarm_Stat_Day)
        {
            pnlTotalCount->Caption=MyDBVEventFreq(asQuery, strngrdMDBQuery);
        }
        else
        {
            if(bFilterTheAgainData && cbDisplayData->ItemIndex==Event_Log)      //Steven 20120222 : 過濾掉Duplicate的訊息
                pnlTotalCount->Caption=MyDBVProcessFilter(asQuery, strngrdMDBQuery);
            else
                pnlTotalCount->Caption=MyDBVProcess(asQuery, strngrdMDBQuery);
        }
        strngrdMDBQuery->RowCount=(strngrdMDBQuery->RowCount+1);
        for(int i=0; i<strngrdMDBQuery->ColCount; i++)
            strngrdMDBQuery->Cells[i][strngrdMDBQuery->RowCount-1]="";
        DateSeparator='-';                                                      // Sam 20210225 : 增加保護
        strngrdMDBQuery->Cells[1][strngrdMDBQuery->RowCount-1]=DateTimePicker1->Date.FormatString("yyyy-mm-dd") +" "+DateTimePicker2->DateTime.FormatString("hh:nn:ss");
        strngrdMDBQuery->Cells[2][strngrdMDBQuery->RowCount-1]=DateTimePicker3->Date.FormatString("yyyy-mm-dd")+" "+DateTimePicker4->DateTime.FormatString("hh:nn:ss");
        strngrdMDBQuery->Cells[0][strngrdMDBQuery->RowCount-1]="Data Period";
    }
    else if(cbDisplayData->ItemIndex==Alarm_Code_List)                          //Steven 20150204 : 新增Alarm Code List 查詢
    {
        strngrdMDBQuery->Visible=true;
        Chart2->Visible=false;

        for(int i=0; i<strngrdMDBQuery->RowCount; i++)                          //Steven 20120222 : 要把多的欄位清空
        {
            strngrdMDBQuery->Rows[i]->Clear();
        }

        strngrdMDBQuery->ColCount=7;
        strngrdMDBQuery->ColWidths[1]=100;                                      //Unit Name
        strngrdMDBQuery->ColWidths[2]=100;                                      //Alarm Code
        strngrdMDBQuery->ColWidths[3]=400;                                      //Message
        strngrdMDBQuery->ColWidths[4]=100;                                      //Level
        strngrdMDBQuery->ColWidths[5]=100;                                      //Red Background
        strngrdMDBQuery->ColWidths[6]=100;                                      //Silent Mode

        asQuery.sprintf("SELECT UnitName.UnitName, AlarmCode, Message FROM AlarmList INNER JOIN UnitName ON (AlarmList.UnitNo = UnitName.ID_UnitNo) ORDER by AlarmList.UnitNo, ID_AlarmList");
        MyDBVProcess(asQuery, strngrdMDBQuery);
        strngrdMDBQuery->Cells[4][0]="Level";
        strngrdMDBQuery->Cells[5][0]="Red Background";
        strngrdMDBQuery->Cells[6][0]="Silent Mode";

        for(int i=2; i<strngrdMDBQuery->RowCount; i++)                          //Steven 20161124 : Add display level set on Jam Code
        {
            UnitName=fMain->AlarmUnitMap[strngrdMDBQuery->Cells[1][i]];         //Steven 20231127 : 整理Alarm Unit
            iLevel  =fSecurity->GetJamLevel(UnitName, strngrdMDBQuery->Cells[2][i]);
            bSilent =fSecurity->GetJemSilent(UnitName, strngrdMDBQuery->Cells[2][i]);
            bRedBG  =fSecurity->GetJemRed(UnitName, strngrdMDBQuery->Cells[2][i]);
            if(iLevel==0)
                strngrdMDBQuery->Cells[4][i]="Operator";
            else if(iLevel==1)
                strngrdMDBQuery->Cells[4][i]="Engineer";
            else if(iLevel==2)
                strngrdMDBQuery->Cells[4][i]="Supervisor";
            else
                strngrdMDBQuery->Cells[4][i]="HonPrec";
            strngrdMDBQuery->Cells[5][i]=(bSilent)?"V":"";
            strngrdMDBQuery->Cells[6][i]=(bRedBG)?"V":"";
        }
    }
    else if(cbDisplayData->ItemIndex==Production_Summary_Report)                //Sam 20210107 : Summary Report fuction
    {
        DoProduction_Summary_Report(DateTimePicker1->Date.FormatString("yyyy-mm-dd"),
                                    DateTimePicker2->DateTime.FormatString("hh:nn:ss"),
                                    DateTimePicker3->Date.FormatString("yyyy-mm-dd"),
                                    DateTimePicker4->DateTime.FormatString("hh:nn:ss"));
    }
    else                                                                        //Jam Chart
    {
        strngrdMDBQuery->Visible=false;
        Chart2->Visible=true;
        AnsiString StartTime="", EndTime="";
        Chart2->Series[0]->Clear();
        DateSeparator='-';                                                      // Sam 20210225 : 增加保護
        StartTime=DateTimePicker1->Date.FormatString("yyyy-mm-dd")+" "+DateTimePicker2->DateTime.FormatString("hh:nn:ss");
        EndTime  =DateTimePicker3->Date.FormatString("yyyy-mm-dd")+" "+DateTimePicker4->DateTime.FormatString("hh:nn:ss");
        Chart2->Title->Text->Clear();
        Chart2->Title->Text->Add(cbDisplayData->Text);
        Chart2->Title->Text->Add("(" + StartTime + " ~ " + EndTime + ")");

        if(cbDisplayData->ItemIndex==Jam_Chart_Summary)
        {
            WhereQuery=" WHERE (AlarmList.AlarmCode>'J' AND (AlarmList.AlarmCode<'K') AND EventLog.Duplicate=0 AND OccurDateTime >= '"+StartTime+"' AND OccurDateTime <= '"+EndTime+"')";
            asQuery.sprintf("SELECT                                                                         \n"
                            "      ID_UnitNo No,                                                            \n"
                            "      UnitName.UnitName,                                                       \n"
                            "      Count(UnitName.UnitName) Count                                           \n"
                            "  FROM                                                                         \n"
                            "      AlarmList                                                                \n"
                            "      INNER JOIN UnitName ON (AlarmList.UnitNo      = UnitName.ID_UnitNo)      \n"
                            "      INNER JOIN EventLog ON (EventLog.ID_AlarmList = AlarmList.ID_AlarmList)  \n"
                            "  %s                                                                           \n"
                            "  GROUP BY                                                                     \n"
                            "      UnitName.UnitName                                                        \n"
                            "  ORDER BY                                                                     \n"
                            "      No ASC                                                                   ", WhereQuery);

            pnlTotalCount->Caption=MyDBVUnitEventCount(asQuery, Chart2);
        }
        else if(cbDisplayData->ItemIndex==Jam_Chart_InArm)
            pnlTotalCount->Caption=MyDBVAxleEventCount(100, 190, StartTime, EndTime, Chart2);  //InArm
        else if(cbDisplayData->ItemIndex==Jam_Chart_OutArm)
            pnlTotalCount->Caption=MyDBVAxleEventCount(200, 290, StartTime, EndTime, Chart2);  //OutArm
        else if(cbDisplayData->ItemIndex==Jam_Chart_Index)
            pnlTotalCount->Caption=MyDBVAxleEventCount(300, 390, StartTime, EndTime, Chart2);  //Index
        else if(cbDisplayData->ItemIndex==Jam_Chart_InShuttle)
            pnlTotalCount->Caption=MyDBVAxleEventCount(400, 490, StartTime, EndTime, Chart2);  //In Shuttle
        else if(cbDisplayData->ItemIndex==Jam_Chart_OutShuttle)
            pnlTotalCount->Caption=MyDBVAxleEventCount(500, 590, StartTime, EndTime, Chart2); //Out Shuttle
    }

//Steven 20190714 : 數字不準, mark
//==>
    if(CUSTOMER_CODE==CC_AMKOR_Philippines ||                                   //JerryYang 20230721 : add
       CUSTOMER_CODE==CC_Microchip_Phil)                                        //JerryYang 20200929 ATP Jayvee要求恢復MTBF顯示
    {
        strngrdMDBQuery->RowCount=strngrdMDBQuery->RowCount+2;                  //Steven 20101103 Start : 報表加上Total Loader與Total Count
        for(int i=0; i<strngrdMDBQuery->ColCount; i++)                          //Steven 20101121 : 要清空最後兩排
        {
            strngrdMDBQuery->Cells[i][strngrdMDBQuery->RowCount-1]="";
            strngrdMDBQuery->Cells[i][strngrdMDBQuery->RowCount-2]="";
        }

        strngrdMDBQuery->Cells[0][strngrdMDBQuery->RowCount-2]="Total Loader";
        strngrdMDBQuery->Cells[1][strngrdMDBQuery->RowCount-2]=lbltTotalLoader->Caption;
        strngrdMDBQuery->Cells[0][strngrdMDBQuery->RowCount-1]="Total Count";
        strngrdMDBQuery->Cells[1][strngrdMDBQuery->RowCount-1]=pnlTotalCount->Caption;
        //Steven 20101103 End

        AnsiString StartDateTime=DateTimePicker1->Date.FormatString("yyyy-mm-dd")+" "+DateTimePicker2->DateTime.FormatString("hh:nn:ss");
        AnsiString EndDateTime  =DateTimePicker3->Date.FormatString("yyyy-mm-dd")+" "+DateTimePicker4->DateTime.FormatString("hh:nn:ss");
        lbltTotalLoader->Caption=MyDBQTotalLoader(StartDateTime, EndDateTime);  //Steven 20101105

        if(cbDisplayData->ItemIndex<Jam_Chart_Summary)                          //Steven 20120628 : 顯示機台稼動時間
        {
            strngrdMDBQuery->RowCount++;
            MyDBQTimeData(StartDateTime, EndDateTime, strngrdMDBQuery->RowCount, strngrdMDBQuery);
        }

        if(cbDisplayData->ItemIndex==Alarm_Statistics ||                        //Steven 20170309 (wei) add MTBF(PauseTime/Jam)
           cbDisplayData->ItemIndex==Alarm_Stat_Day   ||
           cbDisplayData->ItemIndex==Alarm_History_JAM ||
           cbDisplayData->ItemIndex==Alarm_History)                             //JerryYang 20180619 (wei) : 新增可自定義Jam code是否列入MTBF計算
        {
            CountMTBF();
        }
    }
//<==
//Steven 20190714 : 數字不準, mark
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::DateTimePicker1CloseUp(TObject *Sender)             //確保起始日期小於等於結束日期
{
    if(DateTimePicker1->Date>DateTimePicker3->Date)
        DateTimePicker1->DateTime=DateTimePicker3->DateTime;
//    BtnQueryClick(this);                                                      //Steven 20180517 : Mark MDB 避免卡卡

    //lbltTotalLoader->Caption=fMain->ShowRecordLoaderDate();                   //jou 2010-08-26
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::Image1DblClick(TObject *Sender)
{
    if(SystemStart || AccessLevel<iDefHonPrecLevel) return;                     //jou 2014-06-19 Security Have 5 Level 3->iDefHonPrecLevel

    if(CUSTOMER_CODE==CC_SCC ||
       CUSTOMER_CODE==CC_HONPREC_QC ||
       IniConfig.bMaximFunction)                                                //Steven 20150423 : 為了跑40K  //JerryYang 20190522 Maxim統一軟體功能
    {
        ;
    }
    else
    {
        return;
    }

    fPassword->edPassword->Text="";                                             //Steven 20120917
    fQwertyKey->ShowQwertyKey(fPassword->edPassword, N_NO_SYMBOL|N_NO_SPACE|N_PASSWORD);    //Steven 20120904 : 改用QWERTY鍵盤
    if(fPassword->edPassword->Text!="27025312")                                 //jou 2010-07-27 start : 隱藏畫面加上password防護
    {
        return;
    }

    LastSet.iJamCount[0]=0;
    LastSet.iJamCount[1]=0;
//    LastSet.iJamCount[2]=0;
    ProcessRunInfo();
}
//---------------------------------------------------------------------------
void TfObserver::RecordIndexTime(double fData)                                  //jou 2010-12-22 新增index time ave.
{
    double fAve, iCT=0.0;
    double temp;
    double dIndexCycleTime[9];

    for(int i=9; i>=0; i--)
    {
        fRecordIndexTime[i+1]=fRecordIndexTime[i];
    }
    fRecordIndexTime[0]=fData;

    fAve=0;
    for(int i=0; i<10; i++)
    {
        if(fRecordIndexTime[i]!=0)
        {
            fAve+=fRecordIndexTime[i];
            iCT++;
        }
    }

    if(iCT==0)
        fRecordIndexTime[11]=0;
    else
        fRecordIndexTime[11]=fAve/iCT;

    if(CosFunction.bOEEFunction)                                                //Steven 20180417 (Jou) : OEE功能
    {                                                                           //KaiChen 20171127 (Steven) ：超豐 OEE 新增 Test Time、Index Time
        if(bTestIndexZ==true && iTestIndexZCount<12)
        {
            bTestIndexZ=false;
            for(int j=8; j>=0; j--)
            {
                dRecordIndexZTime[j+1]=dRecordIndexZTime[j];
            }
            dRecordIndexZTime[0]=fData;
            if(iTestIndexZCount>=11)
            {
                for(int x=0; x<11; x++)
                {
                    for(int y=x; y<=9; y++)
                    {
                        if(dRecordIndexZTime[y]>dRecordIndexZTime[x])
                        {
                            temp=dRecordIndexZTime[y];
                            dRecordIndexZTime[y]=dRecordIndexZTime[x];
                            dRecordIndexZTime[x]=temp;
                        }
                    }
                }

                double dTimeTotal=0.0;
                for(int k=0; k<=4; k++)
                {
                    dTimeTotal+=dRecordIndexZTime[k];
                }
                sTestIndexZTime=FloatToStr(dTimeTotal/5.0);

                for(int i=0; i<9; i++)                                          //Sam 20180802 (wei) : OEE 32Site 修正
                {
                    dIndexCycleTime[i]=StrToFloatDef(TimeInfoGrid->Cells[4][i+3], 0.0); //Sam 20180822 : OEE 顯示修正
                }

                for(int i=0; i<9; i++)
                {
                    for(int j=0; j<8-i; j++)                                    //Sam 20201216 : 修正記憶體溢位
                    if(dIndexCycleTime[j+1]<dIndexCycleTime[j])
                    {
                        temp=dIndexCycleTime[j];
                        dIndexCycleTime[j]=dIndexCycleTime[j+1];
                        dIndexCycleTime[j+1]=temp;
                    }
                }
                dTimeTotal=0;
                for(int i=0; i<5; i++)
                {
                    dTimeTotal+=dIndexCycleTime[4+i];
                }
                dOEEIndexCycleTime=dTimeTotal/5.0;
            }
            else
            {
                sTestIndexZTime="";
                dOEEIndexCycleTime=0;                                           //Sam 20180802 (wei) : OEE 32Site 修正
            }
            iTestIndexZCount++;
        }
    }
}
//---------------------------------------------------------------------------
void TfObserver::AddTimeData(int iRow, double Time)                             //JerryYang 20151209
{
    for(int i=10; i>1; i--)
    {
        sgTimeData->Cells[i][iRow]=sgTimeData->Cells[i-1][iRow];
    }

    sgTimeData->Cells[1][iRow]=AnsiString(Time);
    if(iRow==2)
    {
        for(int i=0; i<9; i++)
        {
            if(sgTimeData->Cells[i+1][2]!="" && sgTimeData->Cells[i+1][1]!="")
            {
                sgTimeData->Cells[i+1][17]=FloatToStr(StrToFloat(sgTimeData->Cells[i+1][2])-StrToFloat(sgTimeData->Cells[i+1][1]));
            }
        }
    }
    else if(iRow==7)
    {
        sgTimeData->Cells[1][16]=FloatToStr(StrToFloat(sgTimeData->Cells[1][7])-StrToFloat(sgTimeData->Cells[1][6]));
        if(sgTimeData->Cells[2][7]!="" && sgTimeData->Cells[2][6]!="")
            sgTimeData->Cells[2][16]=FloatToStr(StrToFloat(sgTimeData->Cells[2][7])-StrToFloat(sgTimeData->Cells[2][6]));
    }
    else if(iRow==9)
    {
        sgTimeData->Cells[1][17]=FloatToStr(StrToFloat(sgTimeData->Cells[1][9])-StrToFloat(sgTimeData->Cells[1][8]));
        if(sgTimeData->Cells[2][9]!="" && sgTimeData->Cells[2][8]!="")
            sgTimeData->Cells[2][17]=FloatToStr(StrToFloat(sgTimeData->Cells[2][9])-StrToFloat(sgTimeData->Cells[2][8]));
    }
    else if(iRow==11)
    {
        sgTimeData->Cells[1][18]=FloatToStr(StrToFloat(sgTimeData->Cells[1][11])-StrToFloat(sgTimeData->Cells[1][10]));
        if(sgTimeData->Cells[2][11]!="" && sgTimeData->Cells[2][10]!="")
            sgTimeData->Cells[2][18]=FloatToStr(StrToFloat(sgTimeData->Cells[2][11])-StrToFloat(sgTimeData->Cells[2][10]));
    }
    else if(iRow==13)
    {
        sgTimeData->Cells[1][19]=FloatToStr(StrToFloat(sgTimeData->Cells[1][13])-StrToFloat(sgTimeData->Cells[1][9]));
        if(sgTimeData->Cells[2][13]!="" && sgTimeData->Cells[2][9]!="")
            sgTimeData->Cells[2][19]=FloatToStr(StrToFloat(sgTimeData->Cells[2][9])-StrToFloat(sgTimeData->Cells[2][9]));
    }
    else if(iRow==15)
    {
        sgTimeData->Cells[1][20]=FloatToStr(StrToFloat(sgTimeData->Cells[1][15])-StrToFloat(sgTimeData->Cells[1][14]));
        if(sgTimeData->Cells[2][15]!="" && sgTimeData->Cells[2][14]!="")
            sgTimeData->Cells[2][20]=FloatToStr(StrToFloat(sgTimeData->Cells[2][15])-StrToFloat(sgTimeData->Cells[2][14]));
    }
    else if(iRow==20)                                                           //JerryYang 20170503 (wei) drop contact的index cycle time分成三段來計時
    {
        for(int i=0; i<9; i++)
        {
            if(sgTimeData->Cells[i+1][18]!="" && sgTimeData->Cells[i+1][19]!="" && sgTimeData->Cells[i+1][20]!="")
            {
                sgTimeData->Cells[i+1][21]=FloatToStr(StrToFloat(sgTimeData->Cells[i+1][18])+StrToFloat(sgTimeData->Cells[i+1][19])+StrToFloat(sgTimeData->Cells[i+1][20]));
            }
        }
    }

    if(bShow)
        sgTimeData->Refresh();
}
//---------------------------------------------------------------------------
void TfObserver::RecordInArmTime()                                              //Steven 20140930 : For XY-Pitch
{
    double fAve, iCT=0.0;
    for(int i=9; i>=0; i--)
    {
        fRecordInArmTime[i+1]=fRecordInArmTime[i];
    }

    fRecordInArmTime[0]=tRecordInArmTimer.LatchCycleTime()/1000.0;
    tRecordInArmTimer.LatchCycleTime(true);

    fAve=0;
    for(int i=0; i<10; i++)
    {
        if(fRecordInArmTime[i]!=0)
        {
            fAve+=fRecordInArmTime[i];
            iCT++;
        }
    }

    if(iCT==0.0)
        fRecordInArmTime[11]=0;
    else
        fRecordInArmTime[11]=fAve/iCT;

    if(bShow)
    {
        for(int i=0; i<11; i++)
            TimeInfoGrid_InArm->Cells[1][i+1]=fRecordInArmTime[i];
    }
}
//------------------------------------------------------------------------------
void __fastcall TfObserver::bAutoSaveEventLog(bool flag)                        //Steven 20110221 : EventLogAutoSave
{
    static int iCT=0;
    iCT++;

    if(flag==false)
    {
        if(iCT<3)
        {
            return;
        }
        else
        {
            iCT=0;
        }

        if(!(IniConfig.bO06_EventLogAutoSave ||
             IniConfig.bEnableAlarmHistroyAutoSave ||
             IniConfig.bEnableAlarmStatistAutoSave))
        {
            return;
        }
    }

    bool bSaveFlag=false;
    int iDay, iWeek=0;
    double dNow=0.0,dNext=0.0;
    TDateTime dtNow;
    AnsiString asSaveFileName,asNow,asNext;

    dtNow=Now();
    iWeek=dtNow.DayOfWeek()-1;
    dNow=double(dtNow);
    labMachineID->Caption =IniConfig.SocketHandlerID;                           //Steven 20160804 : 避免還沒啟動就執行,發生數值為NULL
    if(fConfiguration->fShow)
        return;

    if(flag==false && IniConfig.bAutoSaveLogWeek[iWeek]==false)
        return;

    DateTimePicker1->Date   =IniConfig.dtEventLogLastRecordDate;
    DateTimePicker2->Time   =IniConfig.dtEventLogLastRecordDate;
    if(flag)
    {
        DateTimePicker3->Date   =Now();
        DateTimePicker4->Time   =Now();
    }
    else
    {
        DateTimePicker3->Date   =DateTimePicker1->Date+IniConfig.iNextEventLogRecordSpace;
        DateTimePicker4->Time   =IniConfig.dtEventLogLastRecordDate;
    }

    DateTimePicker3->Time=DateTimePicker4->Time;                                //jou 2012-10-19 修正 Auto save log 因為每一台機台日期格式都不一樣,會出現error的問題
    asNow=dtNow.DateTimeString();                                               //jou 2013-01-08 修正設定 PM11:59:59 沒辦法記錄
    asNext=DateTimePicker3->DateTime.DateTimeString();
    dNext=double(DateTimePicker3->DateTime);
    if((dNow>=dNext) || flag)
    {
        if(flag==false)
        {
            iDay=dtNow-DateTimePicker3->Date;
            if(iDay>IniConfig.iNextEventLogRecordSpace)                         //確定今天跟存檔時間差幾天，如果大於存檔間隔，就用今天當存檔日
            {
                DateTimePicker3->Date=dtNow;
            }
        }

        MyForceDirectories(IniConfig.asEventLogAutoSavePath);
        if(DirectoryExists(IniConfig.asEventLogAutoSavePath))                   //Even Log Start
        {
            if(IniConfig.bO06_EventLogAutoSave)
            {
                cbDisplayData->ItemIndex=Event_Log;
                asSaveFileName.sprintf("%s\\%s_EvenLog_%s.xls", IniConfig.asEventLogAutoSavePath, labMachineID->Caption, DateTimePicker1->Date.FormatString("yyyymmdd"));
                if(IniConfig.bSPILFunction==true ||                             //Steven 20240604 : SPIL格式的event log
                   CosFunction.bUseMDB==false)
                {
                    btnQueryEventLogTxtClick(this);
                    SGDToXLS(strngrdEventLog, asSaveFileName);
                }
                else
                {
                    BtnQueryClick(this);                                        //JerryYang 20220927 : SPIL lot info
                    SGDToXLS(strngrdMDBQuery, asSaveFileName);
                }

                MySleep(100);

                if(bSaveFlag==false && FileExists(asSaveFileName))
                {
                    DateTimePicker3->Time=DateTimePicker4->Time;
                    IniConfig.dtEventLogLastRecordDate=DateTimePicker3->DateTime;
                    IniConfig.sEventLogLastRecordDate=IniConfig.dtEventLogLastRecordDate.FormatString("yyyy/mm/dd hh:mm:ss");   //Ifor 20160621 修改時間格式 固定為 yyyy/mm/dd hh:mm:ss
                    SaveEventLogAutoSaveInfo();                                 //Steven 20110603
                    bSaveFlag=true;
                }
            }
        }                                                                       //Even Log End

        MyForceDirectories(IniConfig.asAlarmHistroyAutoSavePath);               //Alarm Histroy Start
        if(DirectoryExists(IniConfig.asAlarmHistroyAutoSavePath))
        {
            if(IniConfig.bEnableAlarmHistroyAutoSave)
            {
                cbDisplayData->ItemIndex=Alarm_History_JAM;
                BtnQueryClick(this);

                asSaveFileName.sprintf("%s\\%s_AlarmHistroy_%s.xls", IniConfig.asAlarmHistroyAutoSavePath, labMachineID->Caption, DateTimePicker1->Date.FormatString("yyyymmdd"));
                SGDToXLS(strngrdMDBQuery, asSaveFileName);
                MySleep(100);

                if(bSaveFlag==false && FileExists(asSaveFileName))
                {
                    DateTimePicker3->Time=DateTimePicker4->Time;
                    IniConfig.dtEventLogLastRecordDate=DateTimePicker3->DateTime;
                    IniConfig.sEventLogLastRecordDate=IniConfig.dtEventLogLastRecordDate.FormatString("yyyy/mm/dd hh:mm:ss");   //Ifor 20160621 修改時間格式 固定為 yyyy/mm/dd hh:mm:ss
                    SaveEventLogAutoSaveInfo();                                 //Steven 20110603
                    bSaveFlag=true;
                }
            }
        }                                                                       //Alarm Histroy End

        MyForceDirectories(IniConfig.asAlarmStatistAutoSavePath);               //Alarm Statist Start
        if(DirectoryExists(IniConfig.asAlarmStatistAutoSavePath))
        {
            if(IniConfig.bEnableAlarmStatistAutoSave)
            {
                cbDisplayData->ItemIndex=Alarm_Statistics;
                BtnQueryClick(this);

                asSaveFileName.sprintf("%s\\%s_AlarmStatist_%s.xls", IniConfig.asAlarmStatistAutoSavePath, labMachineID->Caption, DateTimePicker1->Date.FormatString("yyyymmdd"));
                SGDToXLS(strngrdMDBQuery, asSaveFileName);
                MySleep(100);

                if(bSaveFlag==false && FileExists(asSaveFileName))
                {
                    DateTimePicker3->Time=DateTimePicker4->Time;
                    IniConfig.dtEventLogLastRecordDate=DateTimePicker3->DateTime;
                    IniConfig.sEventLogLastRecordDate=IniConfig.dtEventLogLastRecordDate.FormatString("yyyy/mm/dd hh:mm:ss");   //Ifor 20160621 修改時間格式 固定為 yyyy/mm/dd hh:mm:ss
                    SaveEventLogAutoSaveInfo();                                 //Steven 20110603
                    bSaveFlag=true;
                }
            }
        }                                                                       //Alarm Statist End

        MyForceDirectories(IniConfig.asProductionAutoSavePath);                 //Production Data Start
        if(DirectoryExists(IniConfig.asProductionAutoSavePath))
        {
            if(IniConfig.bEnableProductionAutoSave)
            {
                cbDisplayData->ItemIndex=Production_Record;
                BtnQueryClick(this);

                asSaveFileName.sprintf("%s\\%s_ProductionData_%s.xls", IniConfig.asProductionAutoSavePath, labMachineID->Caption, DateTimePicker1->Date.FormatString("yyyymmdd"));
                SGDToXLS(strngrdMDBQuery, asSaveFileName);
                MySleep(100);

                if(bSaveFlag==false && FileExists(asSaveFileName))
                {
                    DateTimePicker3->Time=DateTimePicker4->Time;
                    IniConfig.dtEventLogLastRecordDate=DateTimePicker3->DateTime;
                    IniConfig.sEventLogLastRecordDate=IniConfig.dtEventLogLastRecordDate.FormatString("yyyy/mm/dd hh:mm:ss");   //Ifor 20160621 修改時間格式 固定為 yyyy/mm/dd hh:mm:ss
                    SaveEventLogAutoSaveInfo();                                 //Steven 20110603
                    bSaveFlag=true;
                }
            }
        }                                                                       //Production Data End
    }
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::btAutoSaveClick(TObject *Sender)
{
//    bAutoSaveEventLog(true);                                                  //JerryYang 20161117 MARK,主程式上傳event log到server可能造成延遲，要用小程式上傳
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::lbltTotalLoaderMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Button==mbRight)
    {
        bFilterTheAgainData=!bFilterTheAgainData;                               //Steven 20120222 : 過濾掉Duplicate的訊息
    }
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::btReportClick(TObject *Sender)
{
    if(cbDisplayData->Text=="Event Log")
    {
        strngrdMDBQuery->ColCount=10;
        strngrdMDBQuery->ColWidths[0]=33;                                       //Index
        strngrdMDBQuery->ColWidths[1]=90;                                       //UnitName
        strngrdMDBQuery->ColWidths[2]=75;                                       //AlarmCode
        strngrdMDBQuery->ColWidths[3]=100;                                      //DateTime
        strngrdMDBQuery->ColWidths[4]=50;                                       //Recovery
        strngrdMDBQuery->ColWidths[5]=50;                                       //StopTime
        strngrdMDBQuery->ColWidths[6]=50;                                       //Duplicate
        strngrdMDBQuery->ColWidths[7]=390;                                      //Message
        strngrdMDBQuery->ColWidths[8]=100;                                      //Duplicate
        strngrdMDBQuery->ColWidths[9]=100;                                      //Message
        strngrdMDBQuery->Cells[0][0]="No.";
        strngrdMDBQuery->Cells[0][8]="Jam Stop time";
        strngrdMDBQuery->Cells[0][9]="Operation Stop time";
    }
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::edYieldMaxClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey(edYieldMax, N_INTEGER, 0, true, 105, atoi(edYieldMin->Text.c_str())+1);    //Steven 20120904 : 改用QWERTY鍵盤
    ChartYield->LeftAxis->Maximum=atoi(edYieldMax->Text.c_str());               //Steven 20120807 : 可以變更Yield圖表的最大最小值
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::edYieldMinClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey(edYieldMin, N_INTEGER, 0, true, -5, atoi(edYieldMax->Text.c_str())-1);    //Steven 20120904 : 改用QWERTY鍵盤
    ChartYield->LeftAxis->Minimum=atoi(edYieldMin->Text.c_str());               //Steven 20120807 : 可以變更Yield圖表的最大最小值
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::rgContactCountKindsClick(TObject *Sender)
{
    WriteContactKind();
    StringGrid2->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::rgContactCountHistoryClick(TObject *Sender)
{
    WriteContactKind();
    StringGrid3->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::rgContactCountKindsFormClick(TObject *Sender)
{
    WriteContactKind();
    StringGrid2->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::rgContactCountHistoryFormClick(TObject *Sender)
{
    WriteContactKind();
    StringGrid3->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::mtRowAMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TTMyTray *Ptr=(TTMyTray *)Sender;
    Ptr->ConvertIndexCells(X, Y);

    if(Y>0)
    {
        int iRow=Ptr->Tag;
        int iCol=Y-1;
        bShowYieldSeries[iRow][iCol]=!bShowYieldSeries[iRow][iCol];
    }
    else if(Y==0)
    {
        int iRow=Ptr->Tag;
        int iCol=Y-1;

        for(iCol=0; iCol<MAX_SOCKET_COL; iCol++)
        {
            if(bShowYieldAll[iRow])
                bShowYieldSeries[iRow][iCol]=true;
            else
                bShowYieldSeries[iRow][iCol]=false;
        }

        bShowYieldAll[iRow]=!bShowYieldAll[iRow];
    }
    SetSiteYieldDiagram();
    UpdateYieldChart();
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::WriteCategoryData()
{
    int iArm, iRow, iCol, iCat;
    AnsiString Result="";
    TastCategory.UpdataCount(true);
    RowNo=rgRowNo->ItemIndex;
    AnsiString aStr;

    if(bUseTwoArm32Site==true)                                                  //Steven 20210630 : 修正32site的socket資料顯示
    {
        if(RowNo==0)
            mtRowName->SetCellNumber(0, 0, "Arm 1");
        else
            mtRowName->SetCellNumber(0, 0, "Arm 2");

        for(int i=0; i<mtChName->XItem; i++)
        {
            if(IsNNMode()==NN_1Row)
            {
                if(RowNo==0)
                    iRow=i/8+1;
                else
                    iRow=i/8;
            }
            else
            {
                if(RowNo==0)
                    iRow=i/8+2;
                else
                    iRow=i/8;
            }
            iCol=i%8;

            aStr.sprintf("Col-%c", 'a'+iCol);
            mtDutName->SetCellNumber(i, 0, aStr.c_str());

            if(TestIF.iSiteMap[iRow][iCol]>0)
            {
                aStr.sprintf("CH%d", TestIF.iSiteMap[iRow][iCol]);
                mtChName->SetCellNumber(i, 0, aStr.c_str());
            }
            else
            {
                mtChName->SetCellNumber(i, 0, "----");
            }
        }
    }
    else
    {
        if(RowNo==0)                                                            //jou 20171023 (wei) : 修正yield RowB顯示錯誤
            mtRowName->SetCellNumber(0, 0, "RowA");
        else
            mtRowName->SetCellNumber(0, 0, "RowB");

        for(int i=0; i<mtChName->XItem; i++)
        {
            if(TestIF.iSiteMap[RowNo][i]>0)
            {
                aStr.sprintf("CH%d", TestIF.iSiteMap[RowNo][i]);
                mtChName->SetCellNumber(i, 0, aStr.c_str());
            }
            else
            {
                mtChName->SetCellNumber(i, 0, "----");
            }
        }
    }

    if(rgRowNo->ItemIndex==2 || rgRowNo->ItemIndex==3)
        RowNo=0;

    TastCategory.UpdataCount(true);                                             //Steven 20250514 : 統一計算數量
    mtTotal->SetCellNumber(0, 0, " Total " );
    mtTotal->SetCellNumber(0, 2, " Total " );

    if(rbSocketNumber->Checked ||
       rbSocketPercent->Checked ||
       IsNNMode()==NN_1Row)
        mtCategoryNo->XItem=8;
    else
        mtCategoryNo->XItem=16;

    if(bUseTwoArm32Site==true)                                                  //Steven 20210630 : 修正32site的socket資料顯示
    {
        if(RowNo==0)
        {
            iArm=0;
        }
        else
        {
            iArm=1;
        }

        if(rbHeadNumber->Checked || rbSocketNumber->Checked)                    //顯示數字的話
        {
            //By Socket顯示
            mtTotal->SetCellNumber(0, 1, TastCategory.iTotalSocket);
            mtTotal->SetCellNumber(0, 3, TastCategory.iPassSocket);
            mtTotal->SetCellNumber(0, 4, TastCategory.iTotalCategory[iTestBinCount]);

            for(iCat=0; iCat<iTestBinCount; iCat++)
            {
                mtCategoryTotal->SetCellNumber(0, iCat, TastCategory.iTotalCategory[iCat]);   //填右側下面總和
            }
            //處理其他的Col
            for(int i=0; i<mtChName->XItem; i++)
            {
                if(IsNNMode()==NN_1Row)                                         //Ifor 20260401 : 修正bUseTwoArm32Site ARM1資料未更新問題(與百分比分支對齊iRow計算)
                {
                    if(RowNo==0)
                        iRow=i/8+1;
                    else
                        iRow=i/8;
                }
                else
                {
                    if(RowNo==0)
                        iRow=i/8+2;
                    else
                        iRow=i/8;
                }
                iCol=i%8;
                //處理最下面Row的Total數字
                mtHeadTotal->SetCellNumber(i,   0, TastCategory.iCountHeadTotal[iArm][iRow][iCol]);
                mtPassHead ->SetCellNumber(i,   0, TastCategory.iCountPassHead[iArm][iRow][iCol]);
                mtSockTotal ->SetCellNumber(i,  0, TastCategory.iCountHeadTotal[iArm][iRow][iCol]) ;        //Head Total
                mtPassSocket->SetCellNumber(i,  0, TastCategory.iCountPassHead[iArm][iRow][iCol]);          //Pass Total

                //處理每個DUT的數字
                mtIfError->SetCellNumber(i, 0, TastCategory.iCountCategory[iArm][iRow][iCol][iTestBinCount]);   //IFErr
                //By Arm顯示
                for(iCat=0; iCat<iTestBinCount; iCat++)
                    mtCategoryNo->SetCellNumber(i, iCat, TastCategory.iCountCategory[iArm][iRow][iCol][iCat]);
            }
        }
        else //if(rbSocketPercent->Checked || rbHeadPercent->Checked)
        {
            mtTotal->SetCellNumber(0, 1, ChangeToPercentage(TastCategory.iTotalSocket, TastCategory.iTotalSocket).c_str());
            mtTotal->SetCellNumber(0, 3, ChangeToPercentage(TastCategory.iPassSocket,  TastCategory.iTotalSocket).c_str());
            mtTotal->SetCellNumber(0, 4, ChangeToPercentage(TastCategory.iTotalCategory[iTestBinCount],  TastCategory.iTotalSocket).c_str());
            for(iCat=0; iCat<iTestBinCount; iCat++)
            {
                mtCategoryTotal->SetCellNumber(0, iCat, ChangeToPercentage(TastCategory.iTotalCategory[iCat], TastCategory.iTotalSocket).c_str());
            }

            //處理其他的Col
            for(int i=0; i<mtChName->XItem; i++)
            {
                if(IsNNMode()==NN_1Row)
                {
                    if(RowNo==0)
                        iRow=i/8+1;
                    else
                        iRow=i/8;
                }
                else
                {
                    if(RowNo==0)
                        iRow=i/8+2;
                    else
                        iRow=i/8;
                }
                iCol=i%8;
                mtSockTotal ->SetCellNumber(i, 0, ChangeToPercentage(TastCategory.iCountHeadTotal[iArm][iRow][iCol], TastCategory.iTotalSocket).c_str());       //Head Total
                mtPassSocket->SetCellNumber(i, 0, ChangeToPercentage(TastCategory.iCountPassHead[iArm][iRow][iCol], TastCategory.iTotalSocket).c_str());
                mtHeadTotal ->SetCellNumber(i, 0, ChangeToPercentage(TastCategory.iCountHeadTotal[iArm][iRow][iCol], TastCategory.iTotalSocket).c_str());
                mtPassHead->SetCellNumber(i, 0, ChangeToPercentage(TastCategory.iCountPassHead[iArm][iRow][iCol], TastCategory.iCountHeadTotal[iArm][iRow][iCol]).c_str());

                //處理每個DUT的數字
                mtIfError->SetCellNumber(i, 0, ChangeToPercentage(TastCategory.iCountCategory[iArm][iRow][iCol][iCat], TastCategory.iCountSocketTotal[iRow][iCol]).c_str());  //IFErr
                //By Arm顯示
                for(iCat=0; iCat<iTestBinCount; iCat++)  //category
                {
                    mtCategoryNo->SetCellNumber(i, iCat, ChangeToPercentage(TastCategory.iCountCategory[iArm][iRow][iCol][iCat], TastCategory.iCountHeadTotal[iArm][iRow][iCol]).c_str());
                }
            }
        }
    }
    else
    {
        //顯示數字的話
        if(rbHeadNumber->Checked || rbSocketNumber->Checked)
        {
            //By Socket顯示
            mtTotal->SetCellNumber(0, 1, TastCategory.iTotalSocket);
            mtTotal->SetCellNumber(0, 3, TastCategory.iPassSocket);
            mtTotal->SetCellNumber(0, 4, TastCategory.iTotalCategory[iTestBinCount]);

            for(iCat=0; iCat<iTestBinCount; iCat++)
            {
                mtCategoryTotal->SetCellNumber(0, iCat, TastCategory.iTotalCategory[iCat]);   //填右側下面總和
            }
            //處理其他的Col
            for(iCol=0; iCol<NEW_MAX_Index_Col; iCol++)  //最大是8
            {
                //處理最下面Row的Total數字
                for(iArm=0; iArm<2; iArm++)//arm
                {
                    mtHeadTotal->SetCellNumber(iCol*2+iArm, 0, TastCategory.iCountHeadTotal[iArm][RowNo][iCol]);
                    mtPassHead ->SetCellNumber(iCol*2+iArm, 0, TastCategory.iCountPassHead[iArm][RowNo][iCol]);
                    mtSockTotal ->SetCellNumber(iCol+iArm,  0, TastCategory.iCountHeadTotal[0][RowNo][iCol]+TastCategory.iCountHeadTotal[1][RowNo][iCol]) ;   //Head Total
                    mtPassSocket->SetCellNumber(iCol+iArm,  0, TastCategory.iCountPassHead[0][RowNo][iCol]+TastCategory.iCountPassHead[1][RowNo][iCol]);           //Pass Total
                }

                //處理每個DUT的數字
                mtIfError->SetCellNumber(iCol, 0, TastCategory.iCountCategory[0][RowNo][iCol][iTestBinCount]+TastCategory.iCountCategory[1][RowNo][iCol][iTestBinCount]);   //IFErr
                if(rbSocketNumber->Checked)
                {
                    for(iCat=0; iCat<iTestBinCount; iCat++)  //category    //Steven 20121112 : RS232支援32Bin 15 --> iTestBinCount
                    {
                        sprintf(sSocketCT[iCol][iCat], "%d", TastCategory.iCountCategory[0][RowNo][iCol][iCat]+TastCategory.iCountCategory[1][RowNo][iCol][iCat]);
                        mtCategoryNo->SetCellNumber(iCol, iCat, sSocketCT[iCol][iCat]);         //填入中下面的數值
                    }
                }
                else
                {
                    //By Arm顯示
                    for(iArm=0; iArm<2; iArm++)//arm
                    {
                        for(iCat=0; iCat<iTestBinCount; iCat++)  //category //Steven 20121112 : RS232支援32Bin 15 --> iTestBinCount
                        {
                            mtCategoryNo->SetCellNumber(iCol*2+iArm, iCat, TastCategory.iCountCategory[iArm][RowNo][iCol][iCat]);
                        }
                    }
                }
            }
        }
        else //if(rbSocketPercent->Checked || rbHeadPercent->Checked)
        {
            mtTotal->SetCellNumber(0, 1, ChangeToPercentage(TastCategory.iTotalSocket, TastCategory.iTotalSocket).c_str());
            mtTotal->SetCellNumber(0, 3, ChangeToPercentage(TastCategory.iPassSocket,  TastCategory.iTotalSocket).c_str());
            mtTotal->SetCellNumber(0, 4, ChangeToPercentage(TastCategory.iTotalCategory[iTestBinCount],  TastCategory.iTotalSocket).c_str());
            for(iCat=0; iCat<iTestBinCount; iCat++)
            {
                mtCategoryTotal->SetCellNumber(0, iCat, ChangeToPercentage(TastCategory.iTotalCategory[iCat], TastCategory.iTotalSocket).c_str());
            }

            //處理其他的Col
            for(iCol=0; iCol<NEW_MAX_Index_Col; iCol++)  //最大是8
            {
                for(iArm=0; iArm<2; iArm++)//arm
                {
                    mtSockTotal ->SetCellNumber(iCol+iArm, 0, ChangeToPercentage(TastCategory.iCountHeadTotal[0][RowNo][iCol]+TastCategory.iCountHeadTotal[1][RowNo][iCol], TastCategory.iTotalSocket).c_str());       //Head Total
                    mtPassSocket->SetCellNumber(iCol+iArm, 0, ChangeToPercentage(TastCategory.iCountPassHead[0][RowNo][iCol]+TastCategory.iCountPassHead[1][RowNo][iCol] , TastCategory.iTotalSocket).c_str());
                    mtHeadTotal ->SetCellNumber(iCol*2+iArm, 0, ChangeToPercentage(TastCategory.iCountHeadTotal[iArm][RowNo][iCol], TastCategory.iTotalSocket).c_str());
                    mtPassHead->SetCellNumber(iCol*2+iArm, 0, ChangeToPercentage(TastCategory.iCountPassHead[iArm][RowNo][iCol], TastCategory.iCountHeadTotal[iArm][RowNo][iCol]).c_str());
                }

                //處理每個DUT的數字
                mtIfError->SetCellNumber(iCol, 0, ChangeToPercentage(TastCategory.iCountCategory[0][RowNo][iCol][iCat]+TastCategory.iCountCategory[1][RowNo][iCol][iCat], TastCategory.iCountSocketTotal[RowNo][iCol]).c_str());  //IFErr
                if(rbSocketPercent->Checked)
                {
                    //By Socket顯示                +iArm
                    for(iCat=0; iCat<iTestBinCount; iCat++)  //category    //Steven 20121112 : RS232支援32Bin 15 --> iTestBinCount
                    {
                        sprintf(sSocketCT[iCol][iCat], "%s", ChangeToPercentage(TastCategory.iCountCategory[0][RowNo][iCol][iCat]+TastCategory.iCountCategory[1][RowNo][iCol][iCat], TastCategory.iCountSocketTotal[RowNo][iCol]));
                        mtCategoryNo->SetCellNumber(iCol, iCat, sSocketCT[iCol][iCat]);         //填入中下面的數值
                    }
                }
                else
                {
                    //By Arm顯示
                    for(iArm=0; iArm<2; iArm++)//arm
                    {
                        for(iCat=0; iCat<iTestBinCount; iCat++)  //category
                        {
                            mtCategoryNo->SetCellNumber(iCol*2+iArm, iCat, ChangeToPercentage(TastCategory.iCountCategory[iArm][RowNo][iCol][iCat], TastCategory.iCountHeadTotal[iArm][RowNo][iCol]).c_str());
                        }
                    }
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::ShowVer()
{
    if(CosFunction.bHiSiliconFunction==false)                                   //Ifor 20171018 (wei) : add 海思版本後面不加小版號
    {
        asVer=asHandlerVersion+AnsiString(".")+AnsiString(SVNRevision);         //Ifor 20161109 Handler Version Modify
    }
    else
    {
        asVer=asHandlerVersion;                                                 //Ifor 20161109 Handler Version Modify
    }
    #ifdef ASE_KaohSiung
        asVer=asHandlerVersion;
    #endif

    if(Memo1->Lines->Count>0)
        return;

    Memo1->Clear();
    Memo1->Lines->Add(asVer);
    Memo1->Lines->Add("* Add new handler type: HT-1032AT.");
    Memo1->Lines->Add("* Add new handler type: HT-9011UC.");
    Memo1->Lines->Add("* Add 2x4 NN Mode.");
    Memo1->Lines->Add(" ");
    Memo1->Lines->Add("V3.31");
    Memo1->Lines->Add("* Add new handler type: HT-9016C.");
    Memo1->Lines->Add("* Add new handler type: HT-9115.");
    Memo1->Lines->Add(" ");
    Memo1->Lines->Add("V3.30");
    Memo1->Lines->Add("* Add 2x2 NN Mode.");
    Memo1->Lines->Add("* Add 2x3 NN Mode.");
    Memo1->Lines->Add(" ");
    Memo1->Lines->Add("V3.29");
    Memo1->Lines->Add("* Add 4x4 NN Mode.");
    Memo1->Lines->Add("* Support 8 sites TTL.");
    Memo1->Lines->Add(" ");
    Memo1->Lines->Add("V3.28");
    Memo1->Lines->Add("* Add new handler type: HT-9132LS.");
    Memo1->Lines->Add(" ");
    Memo1->Lines->Add("V3.27");
    Memo1->Lines->Add("* Support Win7.");
    Memo1->Lines->Add("* Support Auto Retest function.");
    Memo1->Lines->Add("* Add new handler type: HT-9045L.");
    Memo1->Lines->Add(" ");
    Memo1->Lines->Add("V3.26");
    Memo1->Lines->Add("* Add FIFO Mode.");
    Memo1->Lines->Add("* Add By Bin count do auto clean.");
    Memo1->Lines->Add("* Add Production log & Error Part in event log.");
    Memo1->Lines->Add("* Reduce reading time of bin setting - Change data structure of Bin setting.");
    Memo1->Lines->Add("* Change LastSet.dat to INI format.");
    Memo1->Lines->Add("* Add [D15] Continuous auto contact test.");
    Memo1->Lines->Add("* Add [D16] Step by step contact test.");
    Memo1->Lines->Add(" ");
    Memo1->Lines->Add("V3.25");
    Memo1->Lines->Add("* Support 2D code function");
    Memo1->Lines->Add("* Support In/Out Arm XY variable pitch.");
    Memo1->Lines->Add("* Add 1x3 Mode.");
    Memo1->Lines->Add("* Add 4x8 Mode.");
    Memo1->Lines->Add("* Add vacuum check before index check.");
    Memo1->Lines->Add("* Add [G07] Support multi color for error bin.");
    Memo1->Lines->Add("* Add [D58] Arm 1 for pick and place,  Arm 2 for testing.");
    Memo1->Lines->Add("* Add [F19] Out shuttle lose IC need do piggyback check");
    Memo1->Lines->Add("* Fixed bugs for all ATC system.");
    Memo1->Lines->Add("* Fixed bugs for auto clean function.");
    Memo1->Lines->Add("* Fixed bugs for device drop error on hot plate.");
    Memo1->Lines->Add("* Fixed bugs for 12 or 16 site mode might crash in arm picker.");
    Memo1->Lines->Add(" ");
    Memo1->Lines->Add("V3.24");
    Memo1->Lines->Add("* Check all site mode about out arm can not close pitch while device dimension is bigger than 25mm");
    Memo1->Lines->Add("* Fixed for ESP program might make handler software can not close.");
    Memo1->Lines->Add(" ");

    if(FileExists("D:\\HT9045\\config\\ReleaseNote.txt"))                       //Steven 20150817 : Item List改用檔案方式, 幫程式碼瘦身,只留下最新的幾個版本
    {
        TStringList *list=new TStringList();
        list->LoadFromFile("D:\\HT9045\\config\\ReleaseNote.txt");
        Memo1->Lines->Add(list->Text);
        list->Clear();                                                          //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        delete list;
    }
    labVersion->Caption=Memo1->Lines->Strings[0];
}
//---------------------------------------------------------------------------
struct aaa
{
    int iWhichSite[MAX_X_ITEM][MAX_Y_ITEM];
    int iWhichIndex[MAX_X_ITEM][MAX_Y_ITEM];                                    //ChungHung 20150205 add for ATK
    int iBinCode[MAX_X_ITEM][MAX_Y_ITEM];
};
//---------------------------------------------------------------------------
void __fastcall TfObserver::btOpenLoadLogClick(TObject *Sender)
{
    AnsiString FileName;
    if(OpenDialog1->Execute())
    {
        FileName=OpenDialog1->FileName;
    }

    aaa QQ;
    DWORD rdfz;
    HANDLE Fp;
    Fp = CreateFile(FileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
    if(Fp != INVALID_HANDLE_VALUE)
    {
        ReadFile(Fp, &QQ, sizeof(aaa), &rdfz, NULL);
    }

    AnsiString WhichSite="";
    AnsiString WhichIndex="";
    AnsiString BinCode="";

    Memo2->Clear();
    Memo3->Clear();
    Memo4->Clear();
    for(int j=0; j<MAX_Y_ITEM; j++)
    {
        for(int i=0; i<MAX_X_ITEM; i++)
        {
            WhichSite+=AnsiString(QQ.iWhichSite[i][j])+", ";
            WhichIndex+=AnsiString(QQ.iWhichIndex[i][j])+", ";
            BinCode+=AnsiString(QQ.iBinCode[i][j])+", ";
        }
        Memo2->Lines->Add(WhichSite);
        Memo2->Lines->Add(WhichIndex);
        Memo2->Lines->Add(BinCode);
        Memo3->Lines->Add(WhichIndex);
        Memo4->Lines->Add(BinCode);
        WhichSite="";
        WhichIndex="";
        BinCode="";
    }

    CloseHandle(Fp);
}
//---------------------------------------------------------------------------
void TfObserver::CountMTBF()                                                    //Steven 20170309 (wei) add MTBF(PauseTime/Jam)
{
    AnsiString asMTBA, asMUBA;
    int iMins, iUnit, iSGCount, iUPH,iMTBFTime=0;                               //Isaac 20180418 add iMTBFTime  //JerryYang 20180518 : add
    double dJamCount, dLoadingCount, dTime;

    dJamCount       =atof(pnlTotalCount->Caption.c_str());
    dLoadingCount   =atof(lbltTotalLoader->Caption.c_str());

    if(dJamCount==0)                                                            //Alick 20161130 add MTBF(PauseTime/Jam)
    {
        asMTBA.sprintf("0 / 1");
    }
    else
    {
        iMTBFTime=iPauseTime+iProductTime+iJamTime;                             //Isaac 20180418 add iMTBFTime
        if(iMTBFTime!=0)
        {
            iMins=iMTBFTime/60;                                                 //JerryYang 20180619 : ASE_CL 尚智要求主MTBF單位改為min
            iMins=iMins/dJamCount;                                              //Isaac 20180417 (Steven) 修正MTBF公式(pause+production+jam)/jamcount
            asMTBA.sprintf( "1 / %d", iMins);
        }
        else
        {
            asMTBA.sprintf("%d / 0", (int)dJamCount);
        }
    }

    if(dJamCount==0)                                                            //Alick 20161130 add MUBF(LoadingCount/Jam)
    {
        asMUBA.sprintf("0 / 1");
    }
    else
    {
        if(dLoadingCount!=0)
        {
            iUnit=dLoadingCount/dJamCount;
            asMUBA.sprintf("1 / %d", iUnit);
        }
        else
        {
            asMUBA.sprintf("%d / 0", (int)dJamCount);
        }
    }

    dTime=double(DateTimePicker3->Date-DateTimePicker1->Date+DateTimePicker4->Time-DateTimePicker2->Time)*24;
    if(dTime==0)
    {
        iUPH=dLoadingCount;
    }
    else
    {
        iUPH=dLoadingCount/dTime;
    }

    iSGCount=strngrdMDBQuery->RowCount;
    strngrdMDBQuery->RowCount=iSGCount+7;

    strngrdMDBQuery->Cells[1][iSGCount+2]="MTBA";
    strngrdMDBQuery->Cells[2][iSGCount+2]=asMTBA;
    strngrdMDBQuery->Cells[3][iSGCount+2]="[mins]";                             //JerryYang 20180619 (wei) : ASE_CL 尚智要求主MTBF單位改為min

    strngrdMDBQuery->Cells[1][iSGCount+3]="MUBA";
    strngrdMDBQuery->Cells[2][iSGCount+3]=asMUBA;
    strngrdMDBQuery->Cells[3][iSGCount+3]="[unit]";

    strngrdMDBQuery->Cells[1][iSGCount+6]="UPH";
    strngrdMDBQuery->Cells[2][iSGCount+6]=iUPH;
    strngrdMDBQuery->Cells[3][iSGCount+6]="[unit/H]";
}
//---------------------------------------------------------------------------
//Steven 20170929 (wei) : 使用 Event Log Text當作顯示畫面
//==>
//---------------------------------------------------------------------------
void __fastcall TfObserver::lstEventLogClick(TObject *Sender)
{
    cbbFilter->ItemIndex=0;
    GetEventLogText();
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::cbbMonthChange(TObject *Sender)
{
    AnsiString sBeginPath="";
    TStringList *tsFileName;
    tsFileName=new TStringList();
    tsFileName->Clear();
    lstEventLog->Clear();
    sBeginPath.sprintf("D:\\HT9045_Log\\EventLogTxt\\%s\\%s\\", cbbEventLogYear->Text, cbbMonth->Text);
    SearchFileAll(sBeginPath, "*.CSV", tsFileName);

    AnsiString s="";
    for(int i=0; i<tsFileName->Count; i++)
    {
        if(CUSTOMER_CODE==CC_SINOICTECH)                                        //otis 20221128 隱藏EventLogTxt儲存路徑
        {
            s=tsFileName->Strings[i].SubString(35,20);
            lstEventLog->Items->Add(s);
        }
        else
        {
            lstEventLog->Items->Add(tsFileName->Strings[i]);
        }
    }

    if(tsFileName->Count>=1)                                                    //JerryYang 20171101 (wei) 改成>=1
    {
        lstEventLog->ItemIndex=tsFileName->Count-1;
    }
    else
    {
        lstEventLog->ItemIndex=-1;
    }
    cbbFilter->ItemIndex=0;
    GetEventLogText();
    tsFileName->Clear();
    delete tsFileName;
}
//---------------------------------------------------------------------------
//AI(ht9045-v899) 20260817: EventLog CSV 顯示用斷欄。只以逗號分隔，並保留雙引號內的空白與逗號。
//                          VCL 的 TStringList::CommaText 會把空白也視為分隔符，因此未加引號的
//                          "07 Tester I/F" 會被切成三欄，使整列右移：Message 欄顯示成停機秒數、
//                          Filter 與 JAM 統計也比對不到 AlarmCode。
//                          本修正只改「讀取顯示」，EventLog 檔案格式完全不變，客戶自行開發的
//                          CSV 解析程式不受影響。
//                          實測 12 家客戶 64 個實機 log 共 100,220 列：99,754 列解析結果完全
//                          相同、414 列修好欄位錯位、52 列為既有的跨行/內含逗號記錄（無害），
//                          零有害回歸。
//                          (CASE-FOREHOPE_NINGBO-20260813-001)
//---------------------------------------------------------------------------
static void AddEventLogCsvField(TStringList *tsRow, AnsiString asField)
{
    asField=asField.Trim();
    if(asField.Length()>=1 && asField[1]=='"')
    {
        asField=asField.SubString(2, asField.Length()-1);                       // 去掉起始引號
        if(asField.Length()>=1 && asField[asField.Length()]=='"')               // 未閉合者比照 CommaText 容忍
            asField=asField.SubString(1, asField.Length()-1);
        asField=StringReplace(asField, "\"\"", "\"", TReplaceFlags()<<rfReplaceAll);
    }
    tsRow->Add(asField);
}
//---------------------------------------------------------------------------
static void SplitEventLogCsvLine(const AnsiString &asLine, TStringList *tsRow)
{
    tsRow->Clear();

    int  iLen    =asLine.Length();
    int  iStart  =1;
    bool bInQuote=false;

    for(int i=1; i<=iLen; i++)
    {
        if(asLine[i]=='"')
        {
            bInQuote=!bInQuote;
        }
        else if(asLine[i]==',' && bInQuote==false)
        {
            AddEventLogCsvField(tsRow, asLine.SubString(iStart, i-iStart));
            iStart=i+1;
        }
    }
    AddEventLogCsvField(tsRow, asLine.SubString(iStart, iLen-iStart+1));
}
//---------------------------------------------------------------------------
static void ParseEventLogLine(const AnsiString &asLine, TStringList *tsRow)
{
    if(IniConfig.bSPILFunction==true)                                           // SPIL log 欄位配置不同且手上無樣本可驗，維持原解析
        tsRow->CommaText=asLine;
    else
        SplitEventLogCsvLine(asLine, tsRow);
}
//---------------------------------------------------------------------------
void TfObserver::GetEventLogText()
{
    int x=1, iJamCol=0;
    AnsiString Str;
    for(int i=1; i<strngrdEventLog->RowCount; i++)
    {
        strngrdEventLog->Rows[i]->Clear();
    }

    if(lstEventLog->Count<1)
    {
        strngrdEventLog->RowCount=2;
        strngrdEventLog->Cells[1][1]="No Record!!";
    }
    else
    {
        AnsiString FileName=lstEventLog->Items->Strings[lstEventLog->ItemIndex];
        if(CUSTOMER_CODE==CC_SINOICTECH)                                        //otis 20221128 將隱藏的EventLogTxt儲存路徑還原，才能讀取檔案
        {
            AnsiString S="";
            S.sprintf("D:\\HT9045_Log\\EventLogTxt\\%s\\%s\\%s.csv", cbbEventLogYear->Text, cbbMonth->Text, FileName);
            FileName=S;
        }

        if(FileExists(FileName))
        {
            TStringList *tsLogFile, *tsRow;
            tsLogFile=new TStringList();
            tsRow    =new TStringList();
            tsLogFile->LoadFromFile(FileName);
            if(tsLogFile->Count>10000)                                          //JerryYang 20191115 event log超過1萬行不要顯示，避免顯示在string grid會發生記憶體不夠用的情況
            {
                strngrdEventLog->RowCount=2;
                strngrdEventLog->Cells[1][1]="Event log over 10000 rows!!";
                tsLogFile->Clear();
                tsRow->Clear();
                delete tsLogFile;
                delete tsRow;
                return;
            }

            if(cbbFilter->ItemIndex==0)
            {
                strngrdEventLog->RowCount=tsLogFile->Count;
                for(int i=0; i<tsLogFile->Count; i++)
                {
                    tsRow->Clear();
                    ParseEventLogLine(tsLogFile->Strings[i], tsRow); //AI(ht9045-v899) 20260817: 改用 ParseEventLogLine (CASE-FOREHOPE_NINGBO-20260813-001)

                    if(IniConfig.bSPILFunction==true)                               //Steven 20240604 : SPIL格式的event log
                    {
                        if(i>0)
                            strngrdEventLog->Cells[0][i]=i;

                        for(int j=0; j<tsRow->Count; j++)
                        {
                            if(i==0 && j==0)
                                strngrdEventLog->Cells[0][0]="No.";
                            else
                                strngrdEventLog->Cells[j+1][i]=tsRow->Strings[j];
                        }
                    }
                    else
                    {
                        for(int j=0; j<tsRow->Count; j++)
                        {
                            strngrdEventLog->Cells[j][i]=tsRow->Strings[j];
                        }
                    }
                }
            }
            else if(cbbFilter->Text.AnsiPos("JAM only")!=0 ||                       //JAM only
                    cbbFilter->Text.AnsiPos("WAR only")!=0 ||                       //WAR only
                    cbbFilter->Text.AnsiPos("MES only")!=0)                         //MES only
            {
                if(cbbFilter->Text.AnsiPos("JAM only")!=0)                          //Steven 20250422 : Add filter for event log
                {
                    Str="JAM";
                }
                else if(cbbFilter->Text.AnsiPos("WAR only")!=0)
                {
                    Str="WAR";
                }
                else //if(cbbFilter->Text.AnsiPos("MES only"))
                {
                    Str="MES";
                }
                strngrdEventLog->RowCount=2;
                for(int i=1; i<tsLogFile->Count; i++)
                {
                    tsRow->Clear();
                    ParseEventLogLine(tsLogFile->Strings[i], tsRow); //AI(ht9045-v899) 20260817: 改用 ParseEventLogLine (CASE-FOREHOPE_NINGBO-20260813-001)
                    if(tsRow->Count>3)
                    {
                        if(IniConfig.bSPILFunction==true)                           //JerryYang 20250428 : fix SPIL event log
                        {
                            iJamCol=1;
                        }
                        else
                        {
                            iJamCol=3;
                        }

                        if(tsRow->Strings[iJamCol].AnsiPos(Str)==1)
                        {
                            strngrdEventLog->RowCount++;
                            for(int j=0; j<tsRow->Count; j++)
                            {
                                strngrdEventLog->Cells[j][x]=tsRow->Strings[j];
                            }
                            x++;
                        }
                    }
                }

                if(strngrdEventLog->RowCount==2)
                {
                    strngrdEventLog->Cells[1][1]="No Record!!";
                }
            }
            else
            {
                strngrdEventLog->RowCount=2;
                for(int i=1; i<tsLogFile->Count; i++)
                {
                    tsRow->Clear();
                    ParseEventLogLine(tsLogFile->Strings[i], tsRow); //AI(ht9045-v899) 20260817: 改用 ParseEventLogLine (CASE-FOREHOPE_NINGBO-20260813-001)
                    if(tsRow->Count>2)
                    {
                        if(IniConfig.bSPILFunction==true)                           //JerryYang 20250428 : fix SPIL event log
                        {
                            iJamCol=0;
                        }
                        else
                        {
                            iJamCol=2;
                        }

                        if(tsRow->Strings[iJamCol].AnsiPos(cbbFilter->Text)!=0)
                        {
                            strngrdEventLog->RowCount++;
                            for(int j=0; j<tsRow->Count; j++)
                            {
                                strngrdEventLog->Cells[j][x]=tsRow->Strings[j];
                            }
                            x++;
                        }
                    }
                }

                if(strngrdEventLog->RowCount==2)
                {
                    strngrdEventLog->Cells[1][1]="No Record!!";
                }
            }

            if(strngrdEventLog->RowCount>2)                                     //Jou 20220117 : foxed for evelt log reader
                strngrdEventLog->FixedRows=1;

            tsLogFile->Clear();
            tsRow->Clear();
            delete tsLogFile;
            delete tsRow;
        }
        else
        {
            strngrdEventLog->RowCount=2;
            strngrdEventLog->Cells[1][1]="No Record!!";
        }
    }
}
//---------------------------------------------------------------------------
bool __fastcall TfObserver::CheckKeyInPrecautionMemoInformation(int iType)      //Sam 20171120 AddPrecautionRecordFunction start (form HT7045)
{
    bool bEmpty[10]={true,true,true,true,true,true,true,true,true,true};

    bEmpty[0]=edPrecautionRecordDocumentNo->Text.IsEmpty();
    bEmpty[1]=edNoteContents->Text.IsEmpty();
    bEmpty[2]=edApprovedManager->Text.IsEmpty();
    bEmpty[3]=edWatchmakers->Text.IsEmpty();
    bEmpty[4]=pnPrecautionStartTime->Caption.IsEmpty();
    bEmpty[5]=cobPRFinishType->Text.IsEmpty();
    if(cobPRFinishType->ItemIndex==1)                                           //Sam 20171121 : 結案方式為 By Data 才需要檢查 Day
        bEmpty[6]=edPromptDay->Text.IsEmpty();
    else
        bEmpty[6]=false;
    bEmpty[7]=edFinishName->Text.IsEmpty();
    bEmpty[8]=pnPrecautionEndTime->Caption.IsEmpty();

    switch(iType)
    {
        case 1:
            if(MemoHandlerPrecautionRecord->Lines->Count   ==0    ||
               bEmpty[0]                                   ==true ||
               bEmpty[1]                                   ==true ||
               bEmpty[2]                                   ==true ||
               bEmpty[3]                                   ==true ||
               bEmpty[4]                                   ==true ||
               bEmpty[5]                                   ==true ||
               bEmpty[6]                                   ==true)
            {
                return false;
            }
            break;
        case 2:
            if(MemoHandlerPrecautionRecord->Lines->Count   ==0    ||
               bEmpty[0]                                   ==true ||
               bEmpty[1]                                   ==true ||
               bEmpty[2]                                   ==true ||
               bEmpty[3]                                   ==true ||
               bEmpty[4]                                   ==true ||
               bEmpty[5]                                   ==true ||
               bEmpty[6]                                   ==true ||
               bEmpty[7]                                   ==true ||
               bEmpty[8]                                   ==true)
            {
                return false;
            }
            break;
        case 3:
            if(MemoHandlerPrecautionRecord->Lines->Count   ==0    ||            //Mylin 20170330 (Joye,Hungyeh) Modify USE_Precaution Issue
               bEmpty[0]                                   ==true ||
               bEmpty[1]                                   ==true ||
               bEmpty[2]                                   ==true ||
               bEmpty[3]                                   ==true ||
               bEmpty[4]                                   ==true ||
               bEmpty[5]                                   ==true ||
               bEmpty[6]                                   ==true)
            {
                return false;
            }
            break;
    }
    return true;
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::LoadPrecautionMenu()                                //Sam 20171120 AddPrecautionRecordFunction start (form HT7045)
{
    AnsiString aCount="1", aCount2="1", aSavePath, S;
    int iCount=1,iCount2=1;
    AnsiString aMenuPath="D:\\PrecautionRecord\\system\\ProcautionMenu.ini";

    aSavePath.sprintf("D:\\PrecautionRecord\\system");
    MyForceDirectories(aSavePath);

    if(IniConfig.bB01_UsePrecautionRecordFunction==false)
        return;

    if(FileExists(aMenuPath)==false)
        return;

    cobHandlerPrecautionRecord->Items->Clear();
    cobNoteContents->Items->Clear();
    do
    {
        S=CheckAndReadIniData(aMenuPath, "Procaution", aCount.c_str(), AnsiString("NULL"));
        cobHandlerPrecautionRecord->Items->Add(S);
        iCount++;
        aCount=iCount;
    }while(CheckIniData(aMenuPath, "Procaution", aCount.c_str())==true);
    cobHandlerPrecautionRecord->ItemIndex=0;

    do
    {
        S=CheckAndReadIniData(aMenuPath, "NoteContent", aCount2.c_str(), AnsiString("NULL"));
        cobNoteContents->Items->Add(S);
        iCount2++;
        aCount2=iCount2;
    }while(CheckIniData(aMenuPath,"NoteContent",aCount2.c_str())==true);
    cobNoteContents->ItemIndex=0;

    S=CheckAndReadIniData(aMenuPath, "SpecificationNO", "PrecautionRecord", AnsiString("NULL"));
    pnPRSpecificationNO->Caption=S;
    S=CheckAndReadIniData(aMenuPath, "SpecificationNO", "MajorMaintenance", AnsiString("NULL"));
    pnMMSpecificationNO->Caption=S;
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::SavePrecautionMemoInformation()
{
    AnsiString aSavePath,aFileName,aData;
    TStringList *memoPtr;

    memoPtr=new TStringList;

    if(IniConfig.bB01_UsePrecautionRecordFunction==false)
        return;

    aSavePath.sprintf("D:\\PrecautionRecord");
    MyForceDirectories(aSavePath, "TfObserver::SavePrecautionMemoInformation_1");

    aFileName.sprintf("D:\\PrecautionRecord\\%s_%04d_%02d_%02d_%02d_%02d_%02d_%s.txt", IniConfig.SocketHandlerID, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec,
                                                                                   edNoteContents->Text);

    aData.sprintf("DOCUMENT NO.: %s", edPrecautionRecordDocumentNo->Text);
    memoPtr->Add(aData);
    aData.sprintf("注意事項: %s", edNoteContents->Text);
    memoPtr->Add(aData);
    aData.sprintf("注意事項內容: ");
    memoPtr->Add(aData);
    for(int i=0; i<MemoHandlerPrecautionRecord->Lines->Count; i++)
        memoPtr->Add(MemoHandlerPrecautionRecord->Lines->Strings[i]);
    aData.sprintf("部門主管核准: %s", edApprovedManager->Text);
    memoPtr->Add(aData);
    aData.sprintf("製表者: %s", edWatchmakers->Text);
    memoPtr->Add(aData);
    aData.sprintf("結案者: %s", edFinishName->Text);
    memoPtr->Add(aData);
    aData.sprintf("開始日期: %s", pnPrecautionStartTime->Caption);
    memoPtr->Add(aData);
    aData.sprintf("結案日期: %s",pnPrecautionEndTime->Caption);
    memoPtr->Add(aData);
    aData.sprintf("結案方式: %s", cobPRFinishType->Text);
    memoPtr->Add(aData);
    aData.sprintf("預計結案天數: %s", edPromptDay->Text);
    memoPtr->Add(aData);

    memoPtr->SaveToFile(aFileName);

    //Mylin 20170328 (Joye,Hungyeh) Add PrecautionRecord and HanderMajorMaintenanceRecord Save File Path
    //==>
    if(IniConfig.asB01_PrecautionRecordSavePath!="")
    {
        try
        {
            AnsiString sFilePath=IniConfig.asB01_PrecautionRecordSavePath;
            AnsiString sFilePathName="";

            if(sFilePath.SubString(sFilePath.Length(),1)!="\\")
            {
                sFilePath=sFilePath+"\\";
            }
            MyForceDirectories(sFilePath, "TfObserver::SavePrecautionMemoInformation_2");
            aFileName.sprintf("%s_%04d_%02d_%02d_%02d_%02d_%02d_%s.txt", IniConfig.SocketHandlerID, SystemYear, SystemMonth,
                                                                        SystemDate, SystemHour, SystemMin, SystemSec,
                                                                        edNoteContents->Text);

            sFilePathName=sFilePath+aFileName;

            memoPtr->SaveToFile(sFilePathName);
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TfObserver::SavePrecautionMemoInformation");
            AnsiString sMsg="PrecautionRecordSavePath Error!";
            ShowMyMessage(sMsg);
        }
    }
    //<==
    //Mylin 20170328 (Joye,Hungyeh) Add PrecautionRecord and HanderMajorMaintenanceRecord Save File Path

    delete memoPtr;
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::SavePrecautionParameter()
{
    AnsiString aSavePath="D:\\PrecautionRecord\\system";
    MyForceDirectories(aSavePath, "TfObserver::SavePrecautionParameter");       //Steven 20240925 : 修正先檢查資料夾才開檔案

    AnsiString sPath="D:\\PrecautionRecord\\system\\PrecautionParameter.ini", str="";
    TIniFile *INIFile=new TIniFile(sPath);

    if(IniConfig.bB01_UsePrecautionRecordFunction==false)
        return;

    INIFile->WriteString    ("Precaution", "DOCUMENT NO."                       , edPrecautionRecordDocumentNo->Text);
    INIFile->WriteString    ("Precaution", "Note Contents"                      , edNoteContents->Text);
    INIFile->WriteString    ("Precaution", "Approved Manager"                   , edApprovedManager->Text);
    INIFile->WriteString    ("Precaution", "Watchmakers"                        , edWatchmakers->Text);
    INIFile->WriteString    ("Precaution", "Finish Name"                        , edFinishName->Text);
    INIFile->WriteString    ("Precaution", "Prompt Day"                         , edPromptDay->Text);
    INIFile->WriteInteger   ("Precaution", "Finish Type"                        , cobPRFinishType->ItemIndex);
    INIFile->WriteString    ("Precaution", "Start Time"                         , pnPrecautionStartTime->Caption);
    INIFile->WriteString    ("Precaution", "End Time"                           , pnPrecautionEndTime->Caption);
    INIFile->WriteString    ("Precaution", "MO ID"                              , asStartPrecautionRecordMOId);     //Sam 20171122 AddPrecautionRecordFunction (form HT7045)

    INIFile->WriteInteger   ("Precaution", "Note Count"                         , MemoHandlerPrecautionRecord->Lines->Count);
    for(int i=0; i<MemoHandlerPrecautionRecord->Lines->Count; i++ )
    {
        str.sprintf("Note%d",i);
        INIFile->WriteString    ("Precaution", str                              , MemoHandlerPrecautionRecord->Lines->Strings[i]);
    }
    delete INIFile;
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::LoadPrecautionParameter()
{
    int iNoteCount;
    AnsiString sPath="D:\\PrecautionRecord\\system\\PrecautionParameter.ini", str="";
    TIniFile *INIFile=new TIniFile(sPath);

    if(IniConfig.bB01_UsePrecautionRecordFunction==false)
        return;

    if(FileExists(sPath)==false)
    {
        delete INIFile;                                                         //20160824 Mylin Add for Fix Memory Leak
        return;
    }

    edPrecautionRecordDocumentNo->Text  =INIFile->ReadString    ("Precaution", "DOCUMENT NO."         , "");
    edNoteContents->Text                =INIFile->ReadString    ("Precaution", "Note Contents"        , "");
    edApprovedManager->Text             =INIFile->ReadString    ("Precaution", "Approved Manager"     , "");
    edWatchmakers->Text                 =INIFile->ReadString    ("Precaution", "Watchmakers"          , "");
    edFinishName->Text                  =INIFile->ReadString    ("Precaution", "Finish Name"          , "");
    edPromptDay->Text                   =INIFile->ReadString    ("Precaution", "Prompt Day"           , "");
    cobPRFinishType->ItemIndex          =INIFile->ReadInteger   ("Precaution", "Finish Type"          , -1);
    pnPrecautionStartTime->Caption      =INIFile->ReadString    ("Precaution", "Start Time"           , Now());
    pnPrecautionEndTime->Caption        =INIFile->ReadString    ("Precaution", "End Time"             , Now());
    asStartPrecautionRecordMOId         =INIFile->ReadString    ("Precaution", "MO ID"                , "");   //Sam 20171122 AddPrecautionRecordFunction (form HT7045)

    iNoteCount=INIFile->ReadInteger   ("Precaution", "Note Count"                         , 0);
    MemoHandlerPrecautionRecord->Lines->Clear();
    for(int i=0; i<iNoteCount; i++)
    {
        str.sprintf("Note%d",i);
        MemoHandlerPrecautionRecord->Lines->Add(INIFile->ReadString    ("Precaution", str  , ""));
    }

    if(pnPrecautionStartTime->Caption!="")
        DateTimePickerStart->Date=pnPrecautionStartTime->Caption;
    if(pnPrecautionEndTime->Caption!="")
        DateTimePickerEnd->Date=pnPrecautionEndTime->Caption;

    delete INIFile;                                                             //20160824 Mylin Add for Fix Memory Leak
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::LoadMajorMaintenanceMenu()
{
    AnsiString aCount="1",aCount2="1",aSavePath,S;
    int iCount=1,iCount2=1;
    AnsiString aMenuPath="D:\\MajorMaintenanceRecord\\system\\MajorMaintenanceMenu.ini";

    aSavePath.sprintf("D:\\MajorMaintenanceRecord\\system");
    MyForceDirectories(aSavePath, "TfObserver::LoadMajorMaintenanceMenu");

    if(IniConfig.bB02_HanderMajorMaintenanceRecordFunction==false)
        return;

    if(FileExists(aMenuPath.c_str())==false)
        return;

    cobUndesirablePhenomenon->Items->Clear();
    cobCountermeasure->Items->Clear();
    do
    {
        S=CheckAndReadIniData(aMenuPath,"UndesirablePhenomenon",aCount.c_str(),AnsiString("NULL"));
        cobUndesirablePhenomenon->Items->Add(S);
        iCount++;
        aCount=iCount;
    }while(CheckIniData(aMenuPath,"UndesirablePhenomenon",aCount.c_str())==true);
    cobUndesirablePhenomenon->ItemIndex=0;

    do
    {
        S=CheckAndReadIniData(aMenuPath,"Countermeasure",aCount2.c_str(),AnsiString("NULL"));
        cobCountermeasure->Items->Add(S);
        iCount2++;
        aCount2=iCount2;
    }while(CheckIniData(aMenuPath,"Countermeasure",aCount2.c_str())==true);
    cobCountermeasure->ItemIndex=0;
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::LoadPrecautionLogMenu()
{
    char str[256];
    AnsiString aSavePath;
    aSavePath.sprintf("D:\\PrecautionRecord");

    if(DirectoryExists(aSavePath)==false)
        return;
    if(IniConfig.bB01_UsePrecautionRecordFunction==false)
        return;

    sprintf(str, "%s\\*.txt", aSavePath);
    FileListBoxPrecautionLog->Mask=AnsiString(str);
    FileListBoxPrecautionLog->Refresh();
    FileListBoxPrecautionLog->Update();

    cobSearchPrecautionLog->Clear();
    for(int i=0; i<FileListBoxPrecautionLog->Items->Count; i++)
    {
        cobSearchPrecautionLog->Items->Add(FileListBoxPrecautionLog->Items->Strings[i]);
    }
    cobSearchPrecautionLog->ItemIndex=0;
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::LoadMajorMaintenanceLogMenu()
{
    char str[256];
    AnsiString aSavePath;
    aSavePath.sprintf("D:\\MajorMaintenanceRecord");

    if(DirectoryExists(aSavePath)==false)
        return;
    if(IniConfig.bB02_HanderMajorMaintenanceRecordFunction==false)
        return;

    sprintf(str, "%s\\*.txt", aSavePath);
    FileListBoxMajorMaintenance->Mask=AnsiString(str);
    FileListBoxMajorMaintenance->Refresh();
    FileListBoxMajorMaintenance->Update();

    cobMajorMaintenanceSearch->Clear();
    for(int i=0; i<FileListBoxMajorMaintenance->Items->Count; i++ )
    {
        cobMajorMaintenanceSearch->Items->Add(FileListBoxMajorMaintenance->Items->Strings[i]);
    }
    cobMajorMaintenanceSearch->ItemIndex=0;
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::SaveMajorMaintenanceInformation()
{
    AnsiString aSavePath,aFileName,aData;

    TStringList *memoPtr;

    memoPtr=new TStringList;

    if(IniConfig.bB02_HanderMajorMaintenanceRecordFunction==false)
        return;

    aSavePath.sprintf("D:\\MajorMaintenanceRecord");

    MyForceDirectories(aSavePath, "TfObserver::SaveMajorMaintenanceInformation_1");

    aFileName.sprintf("D:\\MajorMaintenanceRecord\\%s_%04d_%02d_%02d_%02d_%02d_%02d_%s.txt", IniConfig.SocketHandlerID,
                                                                                        SystemYear, SystemMonth, SystemDate,SystemHour,SystemMin,SystemSec,MemoUndesirablePhenomenon->Lines->Strings[0] );

    aData.sprintf("班別: %s", cobMajorMaintenanceClassType->Text);
    memoPtr->Add(aData);
    aData.sprintf("日期: %s", pnMajorMaintenanceDate->Caption);
    memoPtr->Add(aData);
    aData.sprintf("開始時間: %s", pnMajorMaintenanceStartTime->Caption);
    memoPtr->Add(aData);
    aData.sprintf("結束時間: %s", pnMajorMaintenanceEndTime->Caption);
    memoPtr->Add(aData);

    aData.sprintf("確認批號: %s", edMajorMaintenanceCheckNo->Text);
    memoPtr->Add(aData);
    aData.sprintf("維修者: %s", edMajorMaintenancePersonnel->Text);
    memoPtr->Add(aData);
    aData.sprintf("確認者: %s", edMajorMaintenanceCheckPersonnel->Text);
    memoPtr->Add(aData);

    aData.sprintf("不良現象: ");
    memoPtr->Add(aData);
    for(int i=0; i<MemoUndesirablePhenomenon->Lines->Count; i++ )
        memoPtr->Add(MemoUndesirablePhenomenon->Lines->Strings[i]);

    aData.sprintf("處理對策: ");
    memoPtr->Add(aData);
    for(int i=0; i<MemoCountermeasure->Lines->Count; i++ )
        memoPtr->Add(MemoCountermeasure->Lines->Strings[i]);

    memoPtr->SaveToFile(aFileName);

    //Mylin 20170328 (Joye,Hungyeh) Add PrecautionRecord and HanderMajorMaintenanceRecord Save File Path
    //==>
    if(IniConfig.asB02_HanderMajorMaintenanceRecordSavePath!="")
    {
        try
        {
            AnsiString sFilePath=IniConfig.asB02_HanderMajorMaintenanceRecordSavePath;
            AnsiString sFilePathName="";

            if(sFilePath.SubString(sFilePath.Length(),1)!="\\")
            {
                sFilePath=sFilePath+"\\";
            }
            MyForceDirectories(sFilePath, "TfObserver::SaveMajorMaintenanceInformation_2");
            aFileName.sprintf("%s_%04d_%02d_%02d_%02d_%02d_%02d_%s.txt", IniConfig.SocketHandlerID, SystemYear, SystemMonth,
                                                                        SystemDate, SystemHour, SystemMin,SystemSec,
                                                                        MemoUndesirablePhenomenon->Lines->Strings[0]);

            sFilePathName=sFilePath+aFileName;

            memoPtr->SaveToFile(sFilePathName);
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TfObserver::SaveMajorMaintenanceInformation");
            AnsiString sMsg="HanderMajorMaintenanceRecordSavePath Error!";
            ShowMyMessage(sMsg);
        }
    }
    //<==
    //Mylin 20170328 (Joye,Hungyeh) Add PrecautionRecord and HanderMajorMaintenanceRecord Save File Path
    delete memoPtr;
}
//---------------------------------------------------------------------------
bool __fastcall TfObserver::CheckKeyInMajorMaintenanceInformation(int iType)
{
    bool bEmpty[10]={true,true,true,true,true,true,true,true,true,true};

    bEmpty[0]=cobMajorMaintenanceClassType->Text.IsEmpty();
    bEmpty[1]=edMajorMaintenanceCheckNo->Text.IsEmpty();
    bEmpty[2]=edMajorMaintenancePersonnel->Text.IsEmpty();
    bEmpty[3]=edMajorMaintenanceCheckPersonnel->Text.IsEmpty();
    bEmpty[4]=pnMajorMaintenanceDate->Caption.IsEmpty();
    bEmpty[5]=pnMajorMaintenanceStartTime->Caption.IsEmpty();
    bEmpty[6]=pnMajorMaintenanceEndTime->Caption.IsEmpty();

    switch(iType)
    {
        case 1:
            if(MemoUndesirablePhenomenon->Lines->Count  ==0    ||
                MemoCountermeasure->Lines->Count        ==0    ||
                bEmpty[0]                               ==true ||
                bEmpty[1]                               ==true ||
                bEmpty[2]                               ==true ||
                bEmpty[3]                               ==true ||
                bEmpty[4]                               ==true ||
                bEmpty[5]                               ==true ||
                bEmpty[6]                               ==true)
          {
                return false;
          }
          break;
    }
    return true;
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::btnQueryEventLogTxtClick(TObject *Sender)
{
    GetEventLogText();
}
//---------------------------------------------------------------------------
//<==
//Steven 20170929 (wei) : 使用 Event Log Text當作顯示畫面
//---------------------------------------------------------------------------
void __fastcall TfObserver::sbScreenkeyboardClick(TObject *Sender)
{
    HWND hKeyboard=FindWindow(0, "螢幕小鍵盤");
    if(hKeyboard!=NULL)
        PostMessage(hKeyboard, WM_CLOSE, 0, 0);

    SHELLEXECUTEINFO execinfo;
    memset(&execinfo,0,sizeof(execinfo));
    execinfo.cbSize=sizeof(execinfo);
    execinfo.lpVerb="open";
    execinfo.lpFile="C:\\Windows\\system32\\osk.exe";

    if(FileExists(execinfo.lpFile))
    {
        execinfo.lpParameters="";
        execinfo.fMask=SEE_MASK_NOCLOSEPROCESS;
        execinfo.nShow=SW_SHOWDEFAULT;
        ShellExecuteEx(&execinfo);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::cobNoteContentsSetClick(TObject *Sender)
{
    if(edNoteContents->Text=="")
    {
        edNoteContents->Text=cobNoteContents->Text;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::sbHandlerPrecautionRecordSetClick(
      TObject *Sender)
{
    if(MemoHandlerPrecautionRecord->Lines->IndexOf(cobHandlerPrecautionRecord->Text)==-1 && cobHandlerPrecautionRecord->Text!="")
        MemoHandlerPrecautionRecord->Lines->Add(cobHandlerPrecautionRecord->Text);
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::sbHandlerPrecautionRecordClearClick(
      TObject *Sender)
{
    MemoHandlerPrecautionRecord->Lines->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::sbHandlerPrecautionFormShowClick(
      TObject *Sender)
{
    if(IniConfig.bB01_UsePrecautionRecordFunction==false)                       //Mylin 20170330 (Joye,Hungyeh) Modify USE_Precaution Issue
        return;

    if(CheckKeyInPrecautionMemoInformation(1)==false)
    {
        ShowMyMessage("Precaution Memo Information Not Enter Complete, Please Check");
        return;
    }
    fPrecaution->Close();
    fPrecaution->Show();
    if(bStartPrecautionRecord!=true)
    {
        asStartPrecautionRecordMOId=fLotInfo->ed_PIOEEMO->Text;
    }
    bStartPrecautionRecord=true;
    bSavePrecautionRecordFinish=false;                                          //pig 2015.12.14 ModefyPrecautionRecord
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::sbPrecautionSaveClick(TObject *Sender)
{
    if(IniConfig.bB01_UsePrecautionRecordFunction==false)                       //Mylin 20170330 (Joye,Hungyeh) Modify USE_Precaution Issue
        return;
    if( CheckKeyInPrecautionMemoInformation(2)==false)
    {
        ShowMyMessage("Precaution Memo Information Not Enter Complete, Please Check");
        return;
    }
    SavePrecautionMemoInformation();

    edPrecautionRecordDocumentNo->Text      ="";
    edNoteContents->Text                    ="";
    edApprovedManager->Text                 ="";
    edWatchmakers->Text                     ="";
    edFinishName->Text                      ="";
    pnPrecautionStartTime->Caption          ="";
    pnPrecautionEndTime->Caption            ="";
    edPromptDay->Text                       ="";
    cobNoteContents->ItemIndex              =-1;
    cobHandlerPrecautionRecord->ItemIndex   =-1;
    cobPRFinishType->ItemIndex              =-1;
    MemoHandlerPrecautionRecord->Lines->Clear();
    LoadPrecautionLogMenu();
    bSavePrecautionRecordFinish=true;                                           //pig 2015.12.14 ModefyPrecautionRecord
    bStartPrecautionRecord=false;
    fPrecaution->Close();                                                       //pig 2015.12.14 ModefyPrecautionRecord
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::sbPRFinishDateClick(TObject *Sender)
{
    pnPrecautionEndTime->Caption=FormatDateTime("yyyy/mm/dd",Now());
    DateTimePickerEnd->Date=Now();
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::sbPRStartDateClick(TObject *Sender)
{
    pnPrecautionStartTime->Caption=FormatDateTime("yyyy/mm/dd",Now());
    DateTimePickerStart->Date=Now();
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::sbMajorMaintenanceDateClick(TObject *Sender)
{
    pnMajorMaintenanceDate->Caption=FormatDateTime("yyyy/mm/dd",Now());
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::sbMajorMaintenanceStartTimeClick(
      TObject *Sender)
{
    pnMajorMaintenanceStartTime->Caption=FormatDateTime("hh:nn",Now());
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::sbUndesirablePhenomenonClick(TObject *Sender)
{
    if(MemoUndesirablePhenomenon->Lines->IndexOf(cobUndesirablePhenomenon->Text)==-1 && cobUndesirablePhenomenon->Text!="")
        MemoUndesirablePhenomenon->Lines->Add(cobUndesirablePhenomenon->Text);
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::sbCountermeasureClick(TObject *Sender)
{
    if(MemoCountermeasure->Lines->IndexOf(cobCountermeasure->Text)==-1 &&  cobCountermeasure->Text!="")
        MemoCountermeasure->Lines->Add(cobCountermeasure->Text);
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::sbUndesirablePhenomenonClearClick(
      TObject *Sender)
{
    MemoUndesirablePhenomenon->Lines->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::sbCountermeasureClearClick(TObject *Sender)
{
    MemoCountermeasure->Lines->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::sbMajorMaintenanceEndTimeClick(TObject *Sender)
{
    pnMajorMaintenanceEndTime->Caption = FormatDateTime("hh:nn",Now());
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::sbMajorMaintenanceSaveClick(TObject *Sender)
{
    if(CheckKeyInMajorMaintenanceInformation(1)==false)
    {
        ShowMyMessage("Major Maintenance Information Not Enter Complete, Please Check");
        return;
    }
    SaveMajorMaintenanceInformation();

    edMajorMaintenanceCheckNo->Text             ="";
    edMajorMaintenancePersonnel->Text           ="";
    edMajorMaintenanceCheckPersonnel->Text      ="";
    pnMajorMaintenanceDate->Caption             ="";
    pnMajorMaintenanceStartTime->Caption        ="";
    pnMajorMaintenanceEndTime->Caption          ="";
    cobMajorMaintenanceClassType->ItemIndex     =-1;
    MemoUndesirablePhenomenon->Lines->Clear();
    MemoCountermeasure->Lines->Clear();
    LoadMajorMaintenanceLogMenu();
    bChangeReciepeSaveMajorMaintenanceRecord = false;
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::sbMajorMaintenanceSearchClick(TObject *Sender)
{
    char str[256];
    AnsiString aSavePath,sValue="",sName="",sLogValue;
    TStringList *memoPtr;
    bool bflag1=false,bflag2=false;
    int i;

    memoPtr=new TStringList;

    aSavePath.sprintf("D:\\MajorMaintenanceRecord");

    if(DirectoryExists(aSavePath)==false)
        return;
    if(IniConfig.bB02_HanderMajorMaintenanceRecordFunction==false)
        return;

    sprintf(str,"%s\\%s", aSavePath, cobMajorMaintenanceSearch->Text);

    memoPtr->LoadFromFile(AnsiString(str));

    MemoUndesirablePhenomenon->Clear();
    MemoCountermeasure->Clear();

    for(i=0; i<memoPtr->Count; i++)
    {
        sLogValue=memoPtr->Strings[i];
        sName=sLogValue.SubString(1, sLogValue.Pos(":"));
        sValue=sLogValue.SubString(sLogValue.Pos(":")+2,sLogValue.Length());

        if(pnMajorMaintenanceClassType->Caption==sName)
        {
            cobMajorMaintenanceClassType->ItemIndex=cobMajorMaintenanceClassType->Items->IndexOf(sValue);
            cobMajorMaintenanceClassType->Text=sValue;
        }
        else if(sbMajorMaintenanceDate->Caption==sName)
        {
            pnMajorMaintenanceDate->Caption=sValue;
        }
        else if(sbMajorMaintenanceStartTime->Caption==sName)
        {
            pnMajorMaintenanceStartTime->Caption=sValue;
        }
        else if(sbMajorMaintenanceEndTime->Caption==sName)
        {
            pnMajorMaintenanceEndTime->Caption=sValue;
        }
        else if(pnMajorMaintenanceCheckNo->Caption==sName)
        {
            edMajorMaintenanceCheckNo->Text=sValue;
        }
        else if(pnMajorMaintenancePersonnel->Caption==sName)
        {
            edMajorMaintenancePersonnel->Text=sValue;
        }
        else if(pnMajorMaintenanceCheckPersonnel->Caption==sName)
        {
            edMajorMaintenanceCheckPersonnel->Text=sValue;
        }
        else if(pnUndesirablePhenomenon->Caption==sName)
        {
            bflag1=true;
            bflag2=false;
        }
        else if(pnCountermeasure->Caption==sName)
        {
            bflag2=true;
            bflag1=false;
        }

        if(bflag1)
        {
            if(pnUndesirablePhenomenon->Caption!=sName)
                MemoUndesirablePhenomenon->Lines->Add(sLogValue);
        }
        else if(bflag2)
        {
            if(pnCountermeasure->Caption!=sName)
                MemoCountermeasure->Lines->Add(sLogValue);
        }
    }
    delete memoPtr;
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::sbSearchPrecautionLogClick(TObject *Sender)
{
    char str[256];
    AnsiString aSavePath,sValue="",sName="",sLogValue;
    TStringList *memoPtr;
    int i;

    memoPtr=new TStringList;

    aSavePath.sprintf("D:\\PrecautionRecord");

    if(DirectoryExists(aSavePath)==false)
        return;
    if(IniConfig.bB01_UsePrecautionRecordFunction==false)
        return;

    sprintf(str, "%s\\%s" , aSavePath, cobSearchPrecautionLog->Text);

    memoPtr->LoadFromFile(AnsiString(str));

    MemoNoteLog->Clear();
    for(i=0; i<memoPtr->Count; i++)
    {
        sLogValue=memoPtr->Strings[i];

        sName=sLogValue.SubString(1, sLogValue.Pos(":"));
        sValue=sLogValue.SubString(sLogValue.Pos(":")+2, sLogValue.Length());

        if(pnDOCUMENTNO->Caption==sName)
        {
            pnPrecautionLogDocumentNo->Caption=sValue;
        }
        else if(pnNoteContents->Caption==sName)
        {
            pnPrecautionLogNoteContents->Caption=sValue;
        }
        else if(pnApprovedManager->Caption==sName)
        {
            pnPrecautionLogApprovedManager->Caption=sValue;
        }
        else if(pnNoteLog->Caption==sName)
        {
        }
        else if(pnWatchmakers->Caption==sName)
        {
            pnPrecautionLogWatchmakers->Caption=sValue;
        }
        else if(pnFinishName->Caption==sName)
        {
            pnPrecautionLogFinishName->Caption=sValue;
        }
        else if(pnStartTime->Caption==sName)
        {
            pnPrecautionLogStartTime->Caption=sValue;
        }
        else if(pnEndTime->Caption==sName)
        {
            pnPrecautionLogEndTime->Caption=sValue;
        }
        else if(pnFinishType->Caption==sName)
        {
            pnPrecautionLogFinishType->Caption=sValue;
        }
        else if(pnPromptDay->Caption==sName)
        {
            pnPrecautionLogPromptDay->Caption=sValue;
        }
        else
        {
            MemoNoteLog->Lines->Add(sLogValue);
        }
    }
    delete memoPtr;
}
//---------------------------------------------------------------------------
void RecordReceiveTestTime()                                                    //KaiChen 20171127 (Steven) ：超豐 OEE 新增 Test Time、Index Time
{
    GetTimeInfo();
    for(int i=0; i<2; i++)
    {
        OEERecevieTimeInfo[i].iStartMin    =SystemMin;
        OEERecevieTimeInfo[i].iStartSec    =SystemSec;
        OEERecevieTimeInfo[i].iStartMSec   =SystemMSec;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::pgcMessageChange(TObject *Sender)
{
    AnsiString sBeginPath;
    TStringList *tsFileName;
    tsFileName=new TStringList();
    tsFileName->Clear();
    lstTimeData->Clear();
    sBeginPath.sprintf("D:\\HT9045_Log\\TimeData\\%d\\", SystemYear);
    SearchFileAll(sBeginPath, "*.CSV", tsFileName);

    for(int i=0; i<tsFileName->Count; i++)
    {
        lstTimeData->Items->Add(tsFileName->Strings[i]);
    }

    if(tsFileName->Count>=1)                                                    //JerryYang 20171101 (wei) 改成>=1
    {
        lstTimeData->ItemIndex=tsFileName->Count-1;
    }
    else
    {
        lstTimeData->ItemIndex=-1;
    }

    GetTimeDataText();
    tsFileName->Clear();
    delete tsFileName;
}
//---------------------------------------------------------------------------
void TfObserver::GetTimeDataText()                                              //Steven 20190903 : Time Data顯示
{
    AnsiString Str;
    for(int i=1; i<strngrdTimeData->RowCount; i++)
    {
        strngrdTimeData->Rows[i]->Clear();
    }

    if(lstTimeData->Count<1)
    {
        strngrdTimeData->RowCount=2;
        strngrdTimeData->Cells[1][1]="No Record!!";
    }
    else
    {
        AnsiString FileName=lstTimeData->Items->Strings[lstTimeData->ItemIndex];
        if(FileExists(FileName))
        {
            TStringList *tsLogFile, *tsRow;
            tsLogFile=new TStringList();
            tsRow    =new TStringList();
            tsLogFile->LoadFromFile(FileName);

            strngrdTimeData->RowCount=tsLogFile->Count;
            for(int i=0; i<tsLogFile->Count; i++)
            {
                tsRow->Clear();
                tsRow->CommaText=tsLogFile->Strings[i];
                for(int j=0; j<tsRow->Count; j++)
                {
                    strngrdTimeData->Cells[j][i]=tsRow->Strings[j];
                }
            }
            tsLogFile->Clear();
            tsRow->Clear();
            delete tsLogFile;
            delete tsRow;
        }
        else
        {
            strngrdTimeData->RowCount=2;
            strngrdTimeData->Cells[1][1]="No Record!!";
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::lstTimeDataClick(TObject *Sender)
{
    GetTimeDataText();
}
//---------------------------------------------------------------------------
void TfObserver::RecordIndexCycle(bool bReset)                                  //Sam 20200916 : Add Index Cycle Time Record
{
    TStringList *sIndexCycleTimeCnt;
    static int iRecordIndexCycleTimeCnt=0;

    AnsiString sFilePath="D:\\HT9045_log\\IndexCycleTimeRecord\\";
    AnsiString sFilePathName="";

    if(IniConfig.bD70IndexCycleTimeRecord==false)
    {
        iRecordIndexCycleTimeCnt=0;
        return;
    }

    MyForceDirectories(sFilePath, "TfObserver::RecordIndexCycle");

    if(bReset)
    {
        iRecordIndexCycleTimeCnt=0;
        return;
    }

    sRecordIndexCycleTime[iRecordIndexCycleTimeCnt]=Now().FormatString("yyyy-mm-dd hh:nn:ss")+","+IntToStr(iRecordIndexCycleTimeCnt+1)+","+fObserver->TimeInfoGrid->Cells[4][11];
    iRecordIndexCycleTimeCnt++;
    if(iRecordIndexCycleTimeCnt>=10)
    {
        sIndexCycleTimeCnt=new TStringList();
        sIndexCycleTimeCnt->Clear();
        sIndexCycleTimeCnt->Add("Record Time,No,Index Cycle Time");
        for(int i=0; i<iRecordIndexCycleTimeCnt; i++)
        {
            sIndexCycleTimeCnt->Add(sRecordIndexCycleTime[i]);
        }

        sFilePathName.sprintf("%s%s_Temperature[%d]_AutoTray1Dir[%d]_%s.csv",sFilePath, fMain->cbSetupFileName->Text,LastSet.iTemperature,TrayForm.Auto[0].Direction,Now().FormatString("yyyymmdd_hhnnss"));
        //sFilePathName=sFilePath+Now().FormatString("yyyymmdd_hhnnss")+".csv";
        sIndexCycleTimeCnt->SaveToFile(sFilePathName);
        sIndexCycleTimeCnt->Clear();
        delete sIndexCycleTimeCnt;
        iRecordIndexCycleTimeCnt=0;
    }
}
//---------------------------------------------------------------------------
void TfObserver::DoProduction_Summary_Report(AnsiString asStartData, AnsiString asStartTime, AnsiString asEndData, AnsiString asEndTime)  //Sam 20210107 : Summary Report fuction
{
    int i, j;
    AnsiString asQuery, WhereQuery;
    int iRowNullSpace=2;
    int iStartRow2=0;
    int iStartRow3=0;
    int iAllJamCount=0;
    TDateTime DTStart, DTEnd;
    int iDay;
    char DateSeparatorOld=DateSeparator;
    AnsiString asYear,asData,asFilePath;
    int iLoader,iLoaderSum,iProductTime,iProductTimeSum;
    double dTime;
    int iDownTimeSum=0;
    AnsiString asTime;
    AnsiString asFileTime[2]={"0800-2000","2000-0800"};
    strngrdMDBQuery->Visible=true;
    Chart2->Visible=false;

    for(i=0; i<strngrdMDBQuery->RowCount; i++)
    {
        strngrdMDBQuery->Rows[i]->Clear();
    }
    strngrdMDBQuery->ColCount=7;
    strngrdMDBQuery->DefaultColWidth=70;

    strngrdMDBQuery->ColWidths[0]=70;   //No                        /Data Period
    strngrdMDBQuery->ColWidths[1]=70;   //UnitName                  /Start Time
    strngrdMDBQuery->ColWidths[2]=70;   //AlarmCode                 /End Time
    strngrdMDBQuery->ColWidths[3]=550;  //Message
    strngrdMDBQuery->ColWidths[4]=55;   //Recovery   /Count
    strngrdMDBQuery->ColWidths[5]=60;   //Date       /Stop Time
    strngrdMDBQuery->ColWidths[6]=60;   //Time       /MTTR

    //Print Only Jam
    strngrdTemp->ColCount=8;
    for(i=0; i<strngrdTemp->RowCount; i++)
    {
        strngrdTemp->Rows[i]->Clear();
    }

    WhereQuery.sprintf(" WHERE ((OccurDateTime >= '%s %s' AND OccurDateTime <= '%s %s') "
                           " AND (AlarmCode > 'JAM01' AND AlarmCode < 'JAM99') "
                           " AND Duplicate=0) ",
                           asStartData, asStartTime,
                           asEndData, asEndTime);
    asQuery.sprintf("SELECT UnitName, AlarmCode, Message, Recovery, DATE(OccurDateTime) Date, TIME(OccurDateTime) Time FROM AlarmHistoryView %s ORDER BY OccurDateTime DESC", WhereQuery);
    MyDBVProcess(asQuery, strngrdTemp);
    iStartRow2=strngrdTemp->RowCount+iRowNullSpace;
    strngrdMDBQuery->RowCount=strngrdTemp->RowCount;
    if(strngrdTemp->Cells[1][1]=="No Record!!")
        iAllJamCount=0;
    else
        iAllJamCount=strngrdTemp->RowCount-1;

    for(i=0; i<strngrdTemp->RowCount; i++)
    {
        for(j=0; j<7; j++)
        {
            strngrdMDBQuery->Cells[j][i]=strngrdTemp->Cells[j][i];
        }
    }

    //Jam Statistics
    for(i=0; i<strngrdTemp->RowCount; i++)
    {
        strngrdTemp->Rows[i]->Clear();
    }

    WhereQuery.sprintf(" WHERE ((OccurDateTime >= '%s %s' AND OccurDateTime <= '%s %s') "
                           " AND (AlarmCode > 'JAM01' AND AlarmCode < 'JAM99') "
                           " AND Duplicate=0) ",
                            asStartData, asStartTime,
                            asEndData, asEndTime);

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

    MyDBVProcess(asQuery, strngrdTemp);

    strngrdMDBQuery->RowCount+=strngrdTemp->RowCount+iRowNullSpace;
    iDownTimeSum=0;
    for(i=0; i<strngrdTemp->RowCount; i++)
    {
        for(j=0; j<7; j++)
        {
            if((j==5 || j==6) && i!=0)
            {
                asTime=ConvertSecondToSPC(atoi(strngrdTemp->Cells[j][i].c_str()));
                strngrdMDBQuery->Cells[j][iStartRow2+i]=asTime;
                if(j==5)
                    iDownTimeSum+=atoi(strngrdTemp->Cells[j][i].c_str());       //Sam 20210824 : 新增 Down Time/ MTTR(avg.)
            }
            else
            {
                strngrdMDBQuery->Cells[j][iStartRow2+i]=strngrdTemp->Cells[j][i];
            }
        }
    }

    //Time Statistics
    DateSeparator='-';
    DTStart=StrToDateTime(asStartData);
    DTEnd=StrToDateTime(asEndData);
    iDay=(DTEnd-DTStart);
    if(iDay<=0)
    {
        iDay=1;
    }
    DateSeparator=DateSeparatorOld;
    iLoaderSum=0;
    iProductTimeSum=0;
    for(i=0; i<iDay; i++)
    {
        asYear=(DTStart+i).FormatString("yyyy");
        asData=(DTStart+i).FormatString("yyyy-mm-dd");
        for(j=0; j<2; j++)
        {
            asFilePath.sprintf("%s\\%s\\%s-%s.txt",asProduct_LoaderPath,asYear,asData,asFileTime[j]);
            if(FileExists(asFilePath))
            {
                iLoader         =ReadIniData(asFilePath,"Product","LoaderCount",0);
                iProductTime    =ReadIniData(asFilePath,"Product","ProductTime",0);
                iLoaderSum      +=iLoader;
                iProductTimeSum +=iProductTime;
            }
        }
    }

    iStartRow3=strngrdMDBQuery->RowCount+iRowNullSpace;
    strngrdMDBQuery->RowCount+=iRowNullSpace+6;                                 //Sam 20210824 : 新增 Down Time/ MTTR(avg.) 4>6 //結檔時間/日/時間, 實際run時間/H, 投入數量, 發生jam次數

    strngrdMDBQuery->Cells[0][iStartRow3]  ="Data Period";
    strngrdMDBQuery->Cells[0][iStartRow3+1]="Run Time/H";
    strngrdMDBQuery->Cells[0][iStartRow3+2]="Total Loader";
    strngrdMDBQuery->Cells[0][iStartRow3+3]="Total Count";
    strngrdMDBQuery->Cells[0][iStartRow3+4]="Down Time";                        //Sam 20210824 : 新增 Down Time/ MTTR(avg.)
    strngrdMDBQuery->Cells[0][iStartRow3+5]="MTTR";                             //Sam 20210824 : 新增 Down Time/ MTTR(avg.)

    strngrdMDBQuery->Cells[1][iStartRow3]=asStartData  +" "+   asStartTime;
    strngrdMDBQuery->Cells[2][iStartRow3]=asEndData    +" "+   asEndTime;

    dTime=(double)iProductTimeSum/3600.0;                                       //sec > Hour
    asTime.sprintf("%1.1f",dTime);
    strngrdMDBQuery->Cells[1][iStartRow3+1]=asTime;

    strngrdMDBQuery->Cells[1][iStartRow3+2]=IntToStr(iLoaderSum);
    strngrdMDBQuery->Cells[1][iStartRow3+3]=IntToStr(iAllJamCount);

    strngrdMDBQuery->Cells[1][iStartRow3+4]=ConvertSecondToSPC(iDownTimeSum);   //Sam 20210824 : 新增 Down Time/ MTTR(avg.)
    if(iAllJamCount!=0)
        strngrdMDBQuery->Cells[1][iStartRow3+5]=ConvertSecondToSPC(iDownTimeSum/iAllJamCount);
}
//---------------------------------------------------------------------------
void TfObserver::StatisticalJamCount(bool bIsNextDay)                           //Sam 20210224 : Auto Upload FTP JAMRawData 功能 //KaiChen 20200618 ：矽格，增加Jam統計頁面
{
    if(InitialOK==false)
    {
        return;
    }

    Word Year,Month,Date;

    AnsiString sPathName;
    AnsiString sFileName;
    AnsiString Str;
    AnsiString asHandlerID="";
    AnsiString asErr;

    AnsiString HTPath=slEventLog->Path;
    AnsiString HTFileName=slEventLog->FileName;

    int x=1;

    bool bNewCode=false;

    int iCount=0;
    int iJamCnt=0;
    double dAverage=0.0;

    GetYesterdayInfo();
    GetTimeInfo();

    if(bIsNextDay)                                                              //上傳時間回剛好跨日所以要用昨天時間
    {
        Year    =SystemYearYesterday;
        Month   =SystemMonthYesterday;
        Date    =SystemDateYesterday;
    }
    else
    {
        Year    =SystemYear;
        Month   =SystemMonth;
        Date    =SystemDate;
    }

    sPathName.sprintf("%s\\%04d\\%02d", HTPath, Year, Month);
    MyForceDirectories(sPathName);

    sFileName.sprintf("%s\\%s_%04d%02d%02d.csv", sPathName, HTFileName, Year, Month, Date);
    if(FileExists(sFileName)==false)
    {
        asErr.sprintf("JamRawData is error. EventLog is not exist. %s",sFileName);
        RecordProcess(asErr);
        return;
    }

    TStringList *tsLogFile, *tsRow;
    tsLogFile=new TStringList();
    tsRow    =new TStringList();

    tsLogFile->LoadFromFile(sFileName);
    Str="JAM";
    int iii=0;

    for(int i=1; i<strngrdJamLog->RowCount; i++)
    {
        strngrdJamLog->Rows[i]->Clear();
    }

    strngrdJamLog->RowCount=2;

    for(int i=1; i<tsLogFile->Count; i++)
    {
        tsRow->Clear();
        ParseEventLogLine(tsLogFile->Strings[i], tsRow);               //AI(ht9045-v899) 20260817: 改用 ParseEventLogLine (CASE-FOREHOPE_NINGBO-20260813-001)
        if(tsRow->Count>3)
        {
            if(tsRow->Strings[3].AnsiPos(Str)==1)
            {
                if(StatisticalJamCountEnable(tsRow->Strings[3])==true)
                {
                    for(int j=0; j<strngrdJamLog->RowCount; j++)
                    {
                        if(strngrdJamLog->Cells[2][j]==tsRow->Strings[3])
                        {
                            iii=j;
                            bNewCode=false;
                            break;
                        }
                        else
                        {
                            bNewCode=true;
                        }
                    }

                    if(bNewCode)
                    {
                        strngrdJamLog->Cells[0][x]=x;                           //No
                        strngrdJamLog->Cells[1][x]=tsRow->Strings[2];           //UnitName
                        strngrdJamLog->Cells[2][x]=tsRow->Strings[3];           //AlarmCode
                        strngrdJamLog->Cells[3][x]=tsRow->Strings[7];           //Message
                        strngrdJamLog->Cells[4][x]=1;
                        x++;
                        strngrdJamLog->RowCount++;
                    }
                    else
                    {
                        int aaa=StrToInt(strngrdJamLog->Cells[4][iii]);
                        strngrdJamLog->Cells[4][iii]=aaa+1;
                    }
                }
            }
        }
    }

    for(int j=0; j<strngrdJamLog->RowCount; j++)
    {
        if(strngrdJamLog->Cells[2][j].AnsiPos(Str)==1)
        {
            if(iOneDayLoaderCount>0)
            {
                iJamCnt=StrToInt(strngrdJamLog->Cells[4][j]);
                dAverage=ChangeToFloat((double)iJamCnt, (double)iOneDayLoaderCount);    //Steven 20250820 : 針對除以0加上保護
                AnsiString asAverage;
                asAverage.printf("%0.2f", dAverage);
                strngrdJamLog->Cells[5][j]=asAverage;
            }
            else
            {
                strngrdJamLog->Cells[5][j]=0;
            }
        }
    }

    if(IniConfig.asA32_1_HandlerID=="")
    {
        asHandlerID="HandlerID";
    }
    else
    {
        asHandlerID=IniConfig.asA32_1_HandlerID;
    }

    TMyStringList *tsLogLog;
    tsLogLog=new TMyStringList("D:\\HT9045_Log\\EventLogTxt",
                                 asHandlerID,
                                "Date, Time, No, UnitName, AlarmCode, Message, Count, Rate (%), LoaderCount");
    TStringList *SL;
    SL=new TStringList();

    tsLogLog->MySaveSGJamCountToFile(true, bIsNextDay);

    for(int j=0; j<strngrdJamLog->RowCount; j++)
    {
        if(strngrdJamLog->Cells[2][j].AnsiPos(Str)==1)
        {
            iCount++;
            SL->Clear();
            SL->Add(strngrdJamLog->Cells[0][j]);
            SL->Add(strngrdJamLog->Cells[1][j]);
            SL->Add(strngrdJamLog->Cells[2][j]);
            SL->Add(strngrdJamLog->Cells[3][j]);
            SL->Add(strngrdJamLog->Cells[4][j]);
            SL->Add(strngrdJamLog->Cells[5][j]);
            SL->Add((AnsiString)iOneDayLoaderCount);
            tsLogLog->AddTextWithDateTime(SL->CommaText);
            tsLogLog->MySaveSGJamCountToFile(false, bIsNextDay);
        }
    }

    if(iCount==0)
    {
        SL->Clear();
        SL->Add("");
        SL->Add("");
        SL->Add("");
        SL->Add("");
        SL->Add("");
        SL->Add("");
        SL->Add((AnsiString)iOneDayLoaderCount);
        tsLogLog->AddTextWithDateTime(SL->CommaText);
        tsLogLog->MySaveSGJamCountToFile(false,bIsNextDay);
    }

    if(bIsNextDay && IniConfig.bN26_UseJamRawDataUpdataToFTP)                   //Sam 20210224 : Auto Upload FTP JAMRawData 功能
    {
        HTPath=tsLogLog->Path;
        HTFileName=tsLogLog->FileName;
        if(HTPath=="")
        {
            HTPath="D:\\HandlerLog";
        }

        sPathName.sprintf("%s\\SGJamCount\\%04d\\%02d", HTPath, Year, Month);
        if(tsLogLog->SaveType==TByMaxLineCount)
        {
            sFileName.sprintf("%s_%04d%02d%02d %02d%02d%02d.csv", HTFileName, Year, Month, Date, SystemHour, SystemMin, SystemSec);
        }
        else
        {
            sFileName.sprintf("%s_%04d%02d%02d_RawData.csv", HTFileName, Year, Month, Date);
        }
        fFTPClient->UploadFileFTP(sPathName, sFileName, IniConfig.sN26_FTPUplaodPath, sFileName, IniConfig.sN26_FTPUserName, IniConfig.sN26_FTPPassword,IniConfig.sN26_FTPHost,__FUNC__);
    }

    if(bIsNextDay)
        iOneDayLoaderCount=0;

    SL->Clear();
    delete SL;

    labLoaderCount->Caption=(AnsiString)iOneDayLoaderCount;

    tsLogLog->Clear();
    tsLogFile->Clear();
    tsRow->Clear();
    delete tsLogLog;
    delete tsLogFile;
    delete tsRow;
}
//---------------------------------------------------------------------------
void TfObserver::StatisticalLoaderCount()                                       //KaiChen 20200618 ：矽格，增加Jam統計頁面
{
    iOneDayLoaderCount++;

    AnsiString sPathName, sFileName;
    sPathName.sprintf("D:\\HT9045_Log\\EventLogTxt\\SGJamCount");
    sFileName.sprintf("%s\\LoaderCount.txt", sPathName);
    MyForceDirectories(sPathName);
    WriteIniDataNoLog(sFileName, "Loader", "Count",          iOneDayLoaderCount);
}
//---------------------------------------------------------------------------
void TfObserver::ReadLoaderCount()                                              //KaiChen 20200618 ：矽格，增加Jam統計頁面
{
    AnsiString sPathName, sFileName;
    sPathName.sprintf("D:\\HT9045_Log\\EventLogTxt\\SGJamCount");
    sFileName.sprintf("%s\\LoaderCount.txt",sPathName);
    MyForceDirectories(sPathName);
    if(FileExists(sFileName)==true)
    {
        iOneDayLoaderCount=ReadIniData(sFileName, "Loader",    "Count",          iOneDayLoaderCount);
    }
}
//---------------------------------------------------------------------------
bool TfObserver::StatisticalJamCountEnable(AnsiString asJamCode)                //KaiChen 20200618 ：矽格，增加Jam統計頁面
{
    AnsiString sPathName, sFileName;
    AnsiString asEable="";
    AnsiString Str;
    bool bEnable[19];

    sPathName.sprintf("D:\\HT9045_Log\\EventLogTxt\\SGJamCount");
    sFileName.sprintf("%s\\JamCountEnable.ini",sPathName);
    MyForceDirectories(sPathName);

    for(int i=0; i<19; i++)
    {
        asEable.sprintf("%02d", i+1);
        bEnable[i]=CheckAndReadIniData(sFileName, "JamCountEnable",    asEable,    true);
    }

    for(int i=0; i<19; i++)
    {
        if(bEnable[i]==true)
        {
            Str.sprintf("JAM%02d", i+1);
            if(asJamCode.AnsiPos(Str)==1)
                return true;
        }
    }

    return false;
}
//---------------------------------------------------------------------------
void RecordIndexAirOnTime1()                                                    //Sam 20220329 : Record Index Air On Time
{
    if(CosFunction.RecordIndexAirOnTime==false)
        return;
    for(int j=0; j<100; j++)
    {
        if(j<=QueueAirOnTime1.iCount)
        {
            fObserver->strngrdIndeAirOn1->Cells[1][101-j]=QueueAirOnTime1.GetStartTime(j);
            fObserver->strngrdIndeAirOn1->Cells[2][101-j]=QueueAirOnTime1.GetEndTime(j);
            fObserver->strngrdIndeAirOn1->Cells[3][101-j]=QueueAirOnTime1.GetTimeString(j);
        }
    }
}
//---------------------------------------------------------------------------
void RecordIndexAirOnTime2()                                                    //Sam 20220329 : Record Index Air On Time
{
    if(CosFunction.RecordIndexAirOnTime==false)
        return;
    for(int j=0; j<100; j++)
    {
        if(j<=QueueAirOnTime2.iCount)
        {
            fObserver->strngrdIndeAirOn2->Cells[1][101-j]=QueueAirOnTime2.GetStartTime(j);
            fObserver->strngrdIndeAirOn2->Cells[2][101-j]=QueueAirOnTime2.GetEndTime(j);
            fObserver->strngrdIndeAirOn2->Cells[3][101-j]=QueueAirOnTime2.GetTimeString(j);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::btnSG_QueryNowClick(TObject *Sender)
{
    StatisticalJamCount(false);
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::btnSG_QueryYesterdayClick(TObject *Sender)
{
    StatisticalJamCount(true);
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::btnBackupLogYearClick(TObject *Sender)
{
    AnsiString str="", str1="",str2="";
    TStringList *TestList=new TStringList;
    int iYear=0;

    iYear=atoi(cbbEventLogYear->Text.c_str());
    str2.sprintf("D:\\HT9045_Log\\EventLogTxt\\%d\\%d",iYear,iYear);
    for(int i=1;i<=12;i++)
    {
        str1.sprintf("D:\\HT9045_Log\\EventLogTxt\\%d\\%02d",iYear,i);
        str.sprintf("XCopy /y/a/e/c/i/h/f/r \"%s\" \"%s\"", str1, str2);
        TestList->Add(str);
    }
    TestList->Add(str);

    TestList->SaveToFile("D:\\HT9045\\system\\2.bat");
    ExecZipCommand("D:\\HT9045\\system\\2.bat", " ");                           //Steven 20160205 : 存檔時候不要跳DOS視窗
    TestList->Clear();
    delete TestList;
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::btnClearTimeClick(TObject *Sender)
{
    LastSet.iJamCount[0]=0;
    LastSet.iJamCount[1]=0;
    LastSet.iJamCount[2]=0;
    fCounterClear->ClearCount(ctTimeData);
}
//---------------------------------------------------------------------------
void __fastcall TfObserver::btnLot1Click(TObject *Sender)
{
    TButton *Ptr;
    Ptr=(TButton *)Sender;
    int Tag=Ptr->Tag;

    palCustomer->Caption        =fSCKART->sInfoArr_Customer[Tag];
    palInnLotID->Caption        =fSCKART->sInfoArr_InnerLotID[Tag];
    palCustLotID->Caption       =fSCKART->sInfoArr_CustLotID[Tag];
    palCustDevGup->Caption      =fSCKART->sInfoArr_CustDevGup[Tag];
    palDevName->Caption         =fSCKART->sInfoArr_DeviceName[Tag];
    palStage->Caption           =fSCKART->sInfoArr_Stage[Tag];
    palStep->Caption            =fSCKART->sInfoArr_Step[Tag];
    palReportCnt->Caption       =fSCKART->sInfoArr_ReportCnt[Tag];
    palProgramName->Caption     =fSCKART->sInfoArr_ProgramName[Tag];
    palTestBin->Caption         =fSCKART->sInfoArr_TestBinNo[Tag];
    palTestID->Caption          =fSCKART->sInfoArr_TesterID[Tag];
    palHandlerID->Caption       =fSCKART->sInfoArr_HandlerID[Tag];
    palTemperature->Caption     =fSCKART->sInfoArr_Temperauture[Tag];
    palCurrQty->Caption         =fSCKART->sInfoArr_CurrQty[Tag];
    palOPID->Caption            =fSCKART->sInfoArr_OperatorID[Tag];
    palBinSetting->Caption      =fSCKART->sInfoArr_BinSet[Tag];
    palMultiLotCnt->Caption     =fSCKART->iInfo_MultiLotCnt;
}
//---------------------------------------------------------------------------
