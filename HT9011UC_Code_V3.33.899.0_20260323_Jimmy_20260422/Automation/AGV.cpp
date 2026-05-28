#include "MachineDefine.h"
#pragma hdrstop

#include "AGV.h"

#include "cmydef.h"
#include "main.h"
#include "database.h"
#include "MyMotor.h"
#include "mymessbox.h"
#include "mysensor.h"
#include "myswitch.h"
#include "MyKitSuck.h"
#include "common.h"
#include "csystem.h"
#include "uLotInfo.h"
#include "cmydef.h"
#include "uHGemHT9045.h"
#include "myQwertyKeyBoard.h"
#include "uLotInfo.h"
#include "mycylin.h"
#include "note.h"
#include "UsecegemMainFrom.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ALed"
#pragma resource "*.dfm"
TfAGV *fAGV;
//---------------------------------------------------------------------------
__fastcall TfAGV::TfAGV(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfAGV::ShowE84Sensor()
{
    ALed1->Value=SW[SwE84_1_LREQ].Status()==true;
    ALed2->Value=SW[SwE84_1_UREQ].Status()==true;
    ALed3->Value=SW[SwE84_1_VA].Status()==true;
    ALed4->Value=SW[SwE84_1_READY].Status()==true;
    ALed5->Value=SW[SwE84_1_VS0].Status()==true;
    ALed6->Value=SW[SwE84_1_VS1].Status()==true;
    ALed7->Value=SW[SwE84_1_HOAVBL].Status()==true;
    ALed8->Value=SW[SwE84_1_ES].Status()==true;
    ALed9->Value=true;

    ALed10->Value=Sen[SnE84_1_VALID].IsOn();
    ALed11->Value=Sen[SnE84_1_CS0].IsOn();
    ALed12->Value=Sen[SnE84_1_CS1].IsOn();
    ALed13->Value=Sen[SnE84_1_AMAVBL].IsOn();
    ALed14->Value=Sen[SnE84_1_TRREQ].IsOn();
    ALed15->Value=Sen[SnE84_1_BUSY].IsOn();
    ALed16->Value=Sen[SnE84_1_CONT].IsOn();
    ALed17->Value=Sen[SnE84_1_COMPT].IsOn();
    ALed18->Value=Sen[SnE84_1_GO].IsOn();


    ALed19->Value=SW[SwE84_2_LREQ].Status()==true;
    ALed20->Value=SW[SwE84_2_UREQ].Status()==true;
    ALed21->Value=SW[SwE84_2_VA].Status()==true;
    ALed22->Value=SW[SwE84_2_READY].Status()==true;
    ALed23->Value=SW[SwE84_2_VS0].Status()==true;
    ALed24->Value=SW[SwE84_2_VS1].Status()==true;
    ALed25->Value=SW[SwE84_2_HOAVBL].Status()==true;
    ALed26->Value=SW[SwE84_2_ES].Status()==true;
    ALed27->Value=true;

    ALed28->Value=Sen[SnE84_2_VALID].IsOn();
    ALed29->Value=Sen[SnE84_2_CS0].IsOn();
    ALed30->Value=Sen[SnE84_2_CS1].IsOn();
    ALed31->Value=Sen[SnE84_2_AMAVBL].IsOn();
    ALed32->Value=Sen[SnE84_2_TRREQ].IsOn();
    ALed33->Value=Sen[SnE84_2_BUSY].IsOn();
    ALed34->Value=Sen[SnE84_2_CONT].IsOn();
    ALed35->Value=Sen[SnE84_2_COMPT].IsOn();
    ALed36->Value=Sen[SnE84_2_GO].IsOn();
}
//------------------------------------------------------------------------------
bool E84NowStatusFlag[2][2][9];
//------------------------------------------------------------------------------
void __fastcall TfAGV::NowE84Status()
{
    E84NowStatusFlag[0][0][0]=SW[SwE84_1_LREQ].Status();
    E84NowStatusFlag[0][0][1]=SW[SwE84_1_UREQ].Status();
    E84NowStatusFlag[0][0][2]=SW[SwE84_1_VA].Status();
    E84NowStatusFlag[0][0][3]=SW[SwE84_1_READY].Status();
    E84NowStatusFlag[0][0][4]=SW[SwE84_1_VS0].Status();
    E84NowStatusFlag[0][0][5]=SW[SwE84_1_VS1].Status();
    E84NowStatusFlag[0][0][6]=SW[SwE84_1_HOAVBL].Status();
    E84NowStatusFlag[0][0][7]=SW[SwE84_1_ES].Status();

    E84NowStatusFlag[0][1][0]=Sen[SnE84_1_VALID].IsOn();
    E84NowStatusFlag[0][1][1]=Sen[SnE84_1_CS0].IsOn();
    E84NowStatusFlag[0][1][2]=Sen[SnE84_1_CS1].IsOn();
    E84NowStatusFlag[0][1][3]=Sen[SnE84_1_AMAVBL].IsOn();
    E84NowStatusFlag[0][1][4]=Sen[SnE84_1_TRREQ].IsOn();
    E84NowStatusFlag[0][1][5]=Sen[SnE84_1_BUSY].IsOn();
    E84NowStatusFlag[0][1][6]=Sen[SnE84_1_CONT].IsOn();
    E84NowStatusFlag[0][1][7]=Sen[SnE84_1_COMPT].IsOn();
    E84NowStatusFlag[0][1][8]=Sen[SnE84_1_GO].IsOn();

    E84NowStatusFlag[1][0][0]=SW[SwE84_2_LREQ].Status();
    E84NowStatusFlag[1][0][1]=SW[SwE84_2_UREQ].Status();
    E84NowStatusFlag[1][0][2]=SW[SwE84_2_VA].Status();
    E84NowStatusFlag[1][0][3]=SW[SwE84_2_READY].Status();
    E84NowStatusFlag[1][0][4]=SW[SwE84_2_VS0].Status();
    E84NowStatusFlag[1][0][5]=SW[SwE84_2_VS1].Status();
    E84NowStatusFlag[1][0][6]=SW[SwE84_2_HOAVBL].Status();
    E84NowStatusFlag[1][0][7]=SW[SwE84_2_ES].Status();

    E84NowStatusFlag[1][1][0]=Sen[SnE84_2_VALID].IsOn();
    E84NowStatusFlag[1][1][1]=Sen[SnE84_2_CS0].IsOn();
    E84NowStatusFlag[1][1][2]=Sen[SnE84_2_CS1].IsOn();
    E84NowStatusFlag[1][1][3]=Sen[SnE84_2_AMAVBL].IsOn();
    E84NowStatusFlag[1][1][4]=Sen[SnE84_2_TRREQ].IsOn();
    E84NowStatusFlag[1][1][5]=Sen[SnE84_2_BUSY].IsOn();
    E84NowStatusFlag[1][1][6]=Sen[SnE84_2_CONT].IsOn();
    E84NowStatusFlag[1][1][7]=Sen[SnE84_2_COMPT].IsOn();
    E84NowStatusFlag[1][1][8]=Sen[SnE84_2_GO].IsOn();
}
//------------------------------------------------------------------------------
void __fastcall TfAGV::E84StatusChange()
{
    static bool E84StatusFlag[2][2][9];

    bool bflag;

    AnsiString  asE84StatusFlag[2][2][9]={
        {
            {
                "SwE84_1_LREQ", "SwE84_1_UREQ", "SwE84_1_VA", "SwE84_1_READY",
                "SwE84_1_VS0", "SwE84_1_VS1", "SwE84_1_HOAVBL", "SwE84_1_ES", ""
            },
            {
                "SnE84_1_VALID", "SnE84_1_CS0", "SnE84_1_CS1", "SnE84_1_AMAVBL", "SnE84_1_TRREQ",
                "SnE84_1_BUSY", "SnE84_1_COMPT", "SnE84_1_CONT", "SnE84_1_GO"
            }
        },
        {
            {
                "SwE84_2_LREQ", "SwE84_2_UREQ", "SwE84_2_VA", "SwE84_2_READY",
                "SwE84_2_VS0", "SwE84_2_VS1", "SwE84_2_HOAVBL", "SwE84_2_ES", ""

            },
            {
                "SnE84_2_VALID", "SnE84_2_CS0", "SnE84_2_CS1", "SnE84_2_AMAVBL", "SnE84_2_TRREQ",
                "SnE84_2_BUSY", "SnE84_2_COMPT", "SnE84_2_CONT", "SnE84_2_GO"
            }
        }
    };

    NowE84Status();
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<2; j++)
        {
            for(int k=0; k<9; k++)
            {
                if(E84NowStatusFlag[i][j][k]!=E84StatusFlag[i][j][k])
                {
                    bflag=false;
                    if(E84NowStatusFlag[i][j][k])
                        bflag=true;

                    E84StatusLog(asE84StatusFlag[i][j][k], bflag);
                    E84StatusFlag[i][j][k]=E84NowStatusFlag[i][j][k];
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
int iE84LoadTask=1;
int iE84UnloadTask=1;
int iLoaderScanTask[3]={1, 1, 1};
int iUnloaderScanTask[3]={1, 1, 1};
//------------------------------------------------------------------------------
void InitialE84LoadTask()
{
    iE84LoadTask=1;
}
//------------------------------------------------------------------------------
void InitialE84UnLoaderTask()
{
    iE84UnloadTask=1;
}
//------------------------------------------------------------------------------
void InitialLoaderScanTask()
{
    iLoaderScanTask[0]=1;
    iLoaderScanTask[1]=1;
    iLoaderScanTask[2]=1;
}
//------------------------------------------------------------------------------
void InitialUnLoaderScanTask()
{
    iUnloaderScanTask[0]=1;
    iUnloaderScanTask[1]=1;
    iUnloaderScanTask[2]=1;
}
//------------------------------------------------------------------------------
void InitialE84LoadSensor()
{
    SW[SwE84_1_LREQ].Off();
    SW[SwE84_1_UREQ].Off();
    SW[SwE84_1_VA].Off();
    SW[SwE84_1_READY].Off();
    SW[SwE84_1_VS0].Off();
    SW[SwE84_1_VS1].Off();
}
//------------------------------------------------------------------------------
void InitialE84UnloadSensor()
{
    SW[SwE84_2_LREQ].Off();
    SW[SwE84_2_UREQ].Off();
    SW[SwE84_2_VA].Off();
    SW[SwE84_2_READY].Off();
    SW[SwE84_2_VS0].Off();
    SW[SwE84_2_VS1].Off();
}
//------------------------------------------------------------------------------
TQPF_Timer E84LoadDelay;
TQPF_Timer E84UnLoadDelay;
int iPlaceWhichBuffer[2];
//------------------------------------------------------------------------------
void __fastcall TfAGV::DoE84Loader()
{
    static bool bCS0=false, bCS1=false;
    static int iCount=-1;

    AnsiString str="", str1="", asStr="";
    int &Task=iE84LoadTask;
    int ret;
    int SenIndex[3]={SnLoaderTrayHasTray_AGV, SnEmptyTrayHasTray_AGV, SnColorTrayHasTray_AGV};
    int C_MiddleIndex[3]={C_Load_Middle, C_Empty_Middle, C_Color_Middle};

    #ifndef SOFT_SIMULTE
    SW[SwE84_1_ES].On();
    SW[SwE84_1_HOAVBL].On();
    #endif

    switch(Task)
    {
        case 1:     //AGV車到位 通訊啟動
            if(SW[SwE84_1_HOAVBL].Status()==true && Sen[SnE84_1_GO].IsOn())
            {
//                if(TestIF_File.bAGVRunningMusic)
//                    SW[SwMusic1].On();

                iPlaceWhichBuffer[0]=10;
                if(Sen[SnE84_1_CS0].IsOn() || Sen[SnE84_1_CS1].IsOn())
                {
                    str.sprintf("Sensor_Go On");
                    ShowE84Log(str,1,1);
                    E84LoadDelay.SetSecAndOn(TestIF_File.iE84TimeOut_K12[0][9]);       //TD
                    bCS0=false;
                    bCS1=false;
                    iCount=-1;
                    Task=50;
                }
                else
                {
                    //未知位置
                }
            }
            break;
        case 50:
            if(Sen[SnE84_1_VALID].IsOn())
            {
                str.sprintf("Sensor_VALID On");
                ShowE84Log(str,50,1);
                Task=100;
            }
            else if(E84LoadDelay.Off())
            {
                str.sprintf("Sensor_VALID Is Not On And TD0 Time Out");
                ShowE84Log(str,50,3);
                Task=5000;  //Auto Recover
            }
            break;
        case 100:
            if(Sen[SnE84_1_CS0].IsOn())
            {
                str.sprintf("Sensor_CS0 On");
                ShowE84Log(str,100,1);
                bCS0=true;
            }

            if(Sen[SnE84_1_CS1].IsOn())
            {
                str.sprintf("Sensor_CS1 On");
                ShowE84Log(str,100,1);
                bCS1=true;
            }
            if(bCS0==true && bCS1==false)
                iPlaceWhichBuffer[0]=0;
            else if(bCS0==false && bCS1==true)
                iPlaceWhichBuffer[0]=1;
            else if(bCS0==true && bCS1==true)
                iPlaceWhichBuffer[0]=2;

            if(iPlaceWhichBuffer[0]==10)
            {
//                E84LoadDelay.SetMSAndOn(500);
                str.sprintf("Sensor_CS/CS1 All Off");
                ShowE84Log(str,100,1);
                Task=100;
                break;
            }

            if(Cylinder[C_MiddleIndex[iPlaceWhichBuffer[0]]].OffStatus()==true)
            {
                bE84LoaderActionflag[iPlaceWhichBuffer[0]]=true;
                if(iPlaceWhichBuffer[0]==0)
                    asStr="Loader";
                else if(iPlaceWhichBuffer[0]==1)
                    asStr="Empty";
                else if(iPlaceWhichBuffer[0]==2)
                    asStr="Color";

                str.sprintf("Place To Buffer : %s", asStr);
                ShowE84Log(str,100,1);
                E84LoadDelay.SetSecAndOn(TestIF_File.iE84TimeOut_K12[0][6]);           //TA1
                Task=200;
            }
            break;
        case 200:
            if(SW[SwE84_1_UREQ].Status()==false)
            {
                SW[SwE84_1_UREQ].On();
                E84LoadDelay.SetSecAndOn(TestIF_File.iE84TimeOut_K12[0][0]);           //TP1
                str.sprintf("Switch_UREQ On");
                ShowE84Log(str,200,1);
                Task=300;
            }
            else if(E84LoadDelay.Off())
            {
                str.sprintf("Switch_UREQ Is Not On And TA1 Time Out");
                ShowE84Log(str,200,3);
                Task=5000;
            }
            break;
        case 300:
            if(SW[SwE84_1_UREQ].Status()==true)
            {
                if(Sen[SnE84_1_TRREQ].IsOn())
                {
                    E84LoadDelay.SetSecAndOn(TestIF_File.iE84TimeOut_K12[0][7]);       //TA2
                    str.sprintf("Sensor_TRREQ On");
                    ShowE84Log(str,300,1);
                    Task=400;
                }
                else if(E84LoadDelay.Off())
                {
                    str.sprintf("Sensor_TRREQ Is Not On And TP1 Time Out");
                    ShowE84Log(str,300,3);
                    Task=5000;  //Auto Recover
                }
            }
            break;
        case 400:
            if(SW[SwE84_1_READY].Status()==false)
            {
                SW[SwE84_1_READY].On();
                E84LoadDelay.SetSecAndOn(TestIF_File.iE84TimeOut_K12[0][1]);           //TP2
                str.sprintf("Switch_READY On");
                ShowE84Log(str,400,1);
                Task=500;
            }
            else if(E84LoadDelay.Off())
            {
                str.sprintf("Switch_READY Is Not On And TA2 Time Out");
                ShowE84Log(str,400,3);
                Task=5000;
            }
            break;
        case 500:
            if(Sen[SnE84_1_BUSY].IsOn())
            {
                E84LoadDelay.SetSecAndOn(TestIF_File.iE84TimeOut_K12[0][2]);           //TP3
                str.sprintf("Sensor_BUSY On");
                ShowE84Log(str,500,1);

                bCS0=false;
                bCS1=false;
                if(Sen[SnE84_1_CS0].IsOn())
                {
                    str.sprintf("Sensor_CS0 On");
                    ShowE84Log(str,500,1);
                    bCS0=true;
                }

                if(Sen[SnE84_1_CS1].IsOn())
                {
                    str.sprintf("Sensor_CS1 On");
                    ShowE84Log(str,500,1);
                    bCS1=true;
                }

                if(bCS0==true && bCS1==false)
                    iCount=0;
                else if(bCS0==false && bCS1==true)
                    iCount=1;
                else if(bCS0==true && bCS1==true)
                    iCount=2;
                else
                    iCount=100;

                Task=600;
            }
            else if(E84LoadDelay.Off())
            {
                str.sprintf("Sensor_BUSY Is Not On And TP2 Time Out");
                ShowE84Log(str,500,3);
                Task=5000;  //Auto Recover
            }
            break;
        case 600:
            if(SW[SwE84_1_UREQ].Status()==true)
            {
                if(Sen[SenIndex[iPlaceWhichBuffer[0]]].IsOn()==true)
                {
                    SW[SwE84_1_UREQ].Off();
                    E84LoadDelay.SetSecAndOn(TestIF_File.iE84TimeOut_K12[0][3]);           //TP4
                    str.sprintf("Switch_UREQ OFF");
                    ShowE84Log(str,600,1);
                    Task=700;
                }  
                else if(E84LoadDelay.Off())
                {
                    if(iPlaceWhichBuffer[0]==0)
                        asStr="Loader";
                    else if(iPlaceWhichBuffer[0]==1)
                        asStr="Empty";
                    else if(iPlaceWhichBuffer[0]==2)
                        asStr="Color";

                    str.sprintf("Sensor %s Has Tray Is Not On And TP3 Time Out",asStr);
                    ShowE84Log(str,600,3);
                    Task=5000;  //Auto Recover
                }
            }
            break;
        case 700:
            if(SW[SwE84_1_UREQ].Status()==false)
            {
                if(Sen[SnE84_1_TRREQ].IsOff() && Sen[SnE84_1_COMPT].IsOn())
                {
                    E84LoadDelay.SetSecAndOn(TestIF_File.iE84TimeOut_K12[0][8]);       //TA3
                    str.sprintf("Sensor_TRREQ OFF And Sensor_COMPT On");
                    ShowE84Log(str,700,1);
                    Task=800;
                }
                else if(E84LoadDelay.Off())
                {
                    str.sprintf("Sensor_TRREQ Is Not OFF Or Sensor_COMPT Is Not On / TP4 Time Out");
                    ShowE84Log(str,700,3);
                    Task=5000;  //Auto Recover
                }
            }
            break;
        case 800:
            if(SW[SwE84_1_READY].Status()==true)
            {
                if(iCount!=100)
                {
                    SW[SwE84_1_READY].Off();
                    E84LoadDelay.SetSecAndOn(TestIF_File.iE84TimeOut_K12[0][4]);           //TP5
                    str.sprintf("Switch_READY OFF");
                    ShowE84Log(str,800,1);
                    Task=900;
                }
                else
                {
                    bCS0=false;
                    bCS1=false;
                    if(Sen[SnE84_1_CS0].IsOn())
                    {
                        str.sprintf("Sensor_CS0 On");
                        ShowE84Log(str,800,1);
                        bCS0=true;
                    }

                    if(Sen[SnE84_1_CS1].IsOn())
                    {
                        str.sprintf("Sensor_CS1 On");
                        ShowE84Log(str,800,1);
                        bCS1=true;
                    }

                    if(bCS0==true && bCS1==false)
                        iCount=0;
                    else if(bCS0==false && bCS1==true)
                        iCount=1;
                    else if(bCS0==true && bCS1==true)
                        iCount=2;
                    else
                        iCount=100;
                }
            }
            else if(E84LoadDelay.Off())
            {
                str.sprintf("Switch_READY Is Not OFF / TA3 Time Out");
                ShowE84Log(str,800,3);
                Task=5000;  //Auto Recover
            }
            break;
        case 900:
            if(SW[SwE84_1_READY].Status()==false)
            {
                if(Sen[SnE84_1_VALID].IsOff() && Sen[SnE84_1_COMPT].IsOff() && Sen[SnE84_1_CS0].IsOff() && Sen[SnE84_1_CS1].IsOff())
                {
                    if(iCount==iPlaceWhichBuffer[0])
                    {
                        str.sprintf("Finish Loader Action");      //Finish
                        ShowE84Log(str,900,1);

//                        if(iPlaceWhichBuffer[0]==0)
//                            iAGVFinishTrayCount++;

                        bE84LoaderActionflag[iPlaceWhichBuffer[0]]=false;
                        bE84Loaderflag[iPlaceWhichBuffer[0]]=false;

                        Task=1000;
                    }
                    else
                    {
                        Task=2000;
                    }

                    if(Sen[SnLoaderTrayHasTray_AGV+iPlaceWhichBuffer[0]].IsOff())
                    {
                        //Loader 區只有一盤tray
                    }
                                     //Finish
                }
                else if(E84LoadDelay.Off())
                {
                    str.sprintf("Sensor_VALID Or Sensor_COMPT Or Sensor_CS0 Or Sensor_CS1 Is Not OFF / TP5 Time Out");
                    ShowE84Log(str,900,3);
                    Task=5000;  //Auto Recover
                }
            }
            break;
        case 1000:
            bE84LoaderActionflag[iPlaceWhichBuffer[0]]=false;
            bE84Loaderflag[iPlaceWhichBuffer[0]]=false;
            Task=1;
            break;
        case 2000:
            if(iCount==0)
                str="Loader";
            else if(iCount==1)
                str="Empty";
            else if(iCount==1)
                str="Color";

            if(iPlaceWhichBuffer[0]==0)
                str1="Loader";
            else if(iPlaceWhichBuffer[0]==1)
                str1="Empty";
            else if(iPlaceWhichBuffer[0]==1)
                str1="Color";

            asStr.sprintf("AGV Place To %s,Start Buffer is %s",str, str1);
            ret=ShowMyMessageBox_YES_NO(asStr, "是否要修改Port狀態?");
            if(ret==1)
            {
                iPlaceWhichBuffer[0]=iCount;
                bE84LoaderActionflag[iPlaceWhichBuffer[0]]=false;
                bE84Loaderflag[iPlaceWhichBuffer[0]]=false;
            }
            Task=2100;
            break;
        case 2100:
            str.sprintf("Finish Loader Action");
            ShowE84Log(str,900,1);
            bE84LoaderActionflag[iPlaceWhichBuffer[0]]=false;
            bE84Loaderflag[iPlaceWhichBuffer[0]]=false;
            Task=1000;                                                  //Finish
            break;
        case 5000:              //Auto Recover
            btInitalLoad->Click();
            break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAGV::DoE84Unloader()
{
    static int iCount=-1;
    static bool bCS0=false, bCS1=false;
    static bool bSensorStatus[3]={false, false, false};

    AnsiString str="", asStr="";
    int &Task=iE84UnloadTask;
    int SenIndex[3]={SnAuto1TrayHasTray, SnAuto2TrayHasTray, SnAuto3TrayHasTray};
    int C_MiddleIndex[3]={C_Auto1_Selector, C_Auto2_Selector, C_Auto3_Selector};
    int ret;

    #ifndef SOFT_SIMULTE
    SW[SwE84_2_ES].On();
    SW[SwE84_2_HOAVBL].On();
    #endif

    switch(Task)
    {
        case 1:     //AGV車到位 通訊啟動
            if(SW[SwE84_2_HOAVBL].Status()==true && Sen[SnE84_2_GO].IsOn())
            {
                iPlaceWhichBuffer[1]=10;
                if(Sen[SnE84_2_CS0].IsOn() || Sen[SnE84_2_CS1].IsOn())
                {
                    str.sprintf("Sensor_Go On");
                    ShowE84Log(str,1,2);
                    E84UnLoadDelay.SetSecAndOn(TestIF_File.iE84TimeOut_K12[0][9]);       //TD0
                    bCS0=false;
                    bCS1=false;
                    iCount=-1;
                    Task=50;
                }
                else
                {
                    //未知位置
                }
            }
            break;
        case 50:
            if(Sen[SnE84_2_VALID].IsOn())
            {
                str.sprintf("Sensor_VALID On");
                ShowE84Log(str,50,2);
                Task=100;
            }
            else if(E84UnLoadDelay.Off())
            {
                str.sprintf("Sensor_VALID Is Not On And TD0 Time Out");
                ShowE84Log(str,50,4);
                Task=5000;  //Auto Recover
            }
            break;
        case 100:
            if(Sen[SnE84_2_CS0].IsOn())
            {
                str.sprintf("Sensor_CS0 On");
                ShowE84Log(str,10,2);
                bCS0=true;
            }

            if(Sen[SnE84_2_CS1].IsOn())
            {
                str.sprintf("Sensor_CS1 On");
                ShowE84Log(str,10,2);
                bCS1=true;
            }

            if(bCS0==true && bCS1==false)
                iPlaceWhichBuffer[1]=0;
            else if(bCS0==false && bCS1==true)
                iPlaceWhichBuffer[1]=1;
            else if(bCS0==true && bCS1==true)
                iPlaceWhichBuffer[1]=2;

            if(iPlaceWhichBuffer[1]==10)
            {
                str.sprintf("Sensor_CS/CS1 All Off");
                ShowE84Log(str,100,2);
                Task=100;
                break;
            }

            for(int i=0; i<3; i++)
            {
                bSensorStatus[i]=false;
                if(Sen[SenIndex[i]].IsOn())
                {
                    bSensorStatus[i]=true;
                }
            }

            if(Cylinder[C_MiddleIndex[iPlaceWhichBuffer[1]]].OffStatus()==true)
            {
                bE84UnloaderActionflag[iPlaceWhichBuffer[1]]=true;
                if(iPlaceWhichBuffer[1]==0)
                    asStr="Auto 1";
                else if(iPlaceWhichBuffer[1]==1)
                    asStr="Auto 2";
                else if(iPlaceWhichBuffer[1]==2)
                    asStr="Auto 3";

                str.sprintf("Pick from Buffer : %s", asStr);
                E84UnLoadDelay.SetSecAndOn(TestIF_File.iE84TimeOut_K12[1][6]);           //TA1
                ShowE84Log(str,100,2);
                Task=200;
            }
            break;
        case 200:
            if(SW[SwE84_2_LREQ].Status()==false)
            {
                SW[SwE84_2_LREQ].On();
                E84UnLoadDelay.SetSecAndOn(TestIF_File.iE84TimeOut_K12[1][0]);           //TP1
                str.sprintf("Switch_LREQ On");
                ShowE84Log(str,200,2);
                Task=300;
            }
            else if(E84UnLoadDelay.Off())
            {
                str.sprintf("Switch_LREQ Is Not On And TA1 Time Out");
                ShowE84Log(str,200,4);
                Task=5000;
            }
            break;
        case 300:
            if(SW[SwE84_2_LREQ].Status()==true)
            {
                if(Sen[SnE84_2_TRREQ].IsOn())
                {
                    E84UnLoadDelay.SetSecAndOn(TestIF_File.iE84TimeOut_K12[1][7]);       //TA2
                    str.sprintf("Sensor_TRREQ On");
                    ShowE84Log(str,300,2);
                    Task=400;
                }
                else if(E84UnLoadDelay.Off())
                {
                    str.sprintf("Sensor_TRREQ Is Not On And TP1 Time Out");
                    ShowE84Log(str,300,4);
                    Task=5000;  //Auto Recover
                }
            }
            break;
        case 400:
            if(SW[SwE84_2_READY].Status()==false)
            {
                SW[SwE84_2_READY].On();
                E84UnLoadDelay.SetSecAndOn(TestIF_File.iE84TimeOut_K12[1][1]);           //TP2
                str.sprintf("Switch_READY On");
                ShowE84Log(str,400,2);
                Task=500;
            }
            else if(E84UnLoadDelay.Off())
            {
                str.sprintf("Switch_READY Is Not On And TA2 Time Out");
                ShowE84Log(str,400,4);
                Task=5000;
            }
            break;
        case 500:
            if(Sen[SnE84_2_BUSY].IsOn())
            {
                E84UnLoadDelay.SetSecAndOn(TestIF_File.iE84TimeOut_K12[1][2]);           //TP3
                str.sprintf("Sensor_BUSY On");
                ShowE84Log(str,500,2);

                bCS0=false;
                bCS1=false;
                if(Sen[SnE84_2_CS0].IsOn())
                {
                    str.sprintf("Sensor_CS0 On");
                    ShowE84Log(str,500,2);
                    bCS0=true;
                }

                if(Sen[SnE84_2_CS1].IsOn())
                {
                    str.sprintf("Sensor_CS1 On");
                    ShowE84Log(str,500,2);
                    bCS1=true;
                }

                if(bCS0==true && bCS1==false)
                    iCount=0;
                else if(bCS0==false && bCS1==true)
                    iCount=1;
                else if(bCS0==true && bCS1==true)
                    iCount=2;
                else
                    iCount=100;

                Task=600;
            }
            else if(E84UnLoadDelay.Off())
            {
                str.sprintf("Sensor_BUSY Is Not On And TP2 Time Out");
                ShowE84Log(str,500,4);
                Task=5000;  //Auto Recover
            }
            break;
        case 600:
            if(SW[SwE84_2_LREQ].Status()==true)
            {
                if(Sen[SenIndex[iPlaceWhichBuffer[1]]].IsOff()==true)
                {
                    SW[SwE84_2_LREQ].Off();
                    E84UnLoadDelay.SetSecAndOn(TestIF_File.iE84TimeOut_K12[1][3]);           //TP4
                    str.sprintf("Switch_UREQ OFF");
                    ShowE84Log(str,600,2);
                    Task=700;
                }
                else if(E84UnLoadDelay.Off())
                {
                    if(iPlaceWhichBuffer[1]==0)
                        asStr="Auto 1";
                    else if(iPlaceWhichBuffer[1]==1)
                        asStr="Auto 2";
                    else if(iPlaceWhichBuffer[1]==2)
                        asStr="Auto 3";

                    str.sprintf("Sensor %s Has Tray Is Not On And TP3 Time Out",asStr);
                    ShowE84Log(str,600,4);
                    Task=5000;  //Auto Recover
                }
            }
            break;
        case 700:
            if(SW[SwE84_2_LREQ].Status()==false)
            {
                if(Sen[SnE84_2_TRREQ].IsOff() && Sen[SnE84_2_COMPT].IsOn())
                {
                    E84UnLoadDelay.SetSecAndOn(TestIF_File.iE84TimeOut_K12[1][8]);       //TA3
                    str.sprintf("Sensor_TRREQ OFF And Sensor_COMPT On");
                    ShowE84Log(str,700,2);
                    Task=800;
                }
                else if(E84UnLoadDelay.Off())
                {
                    str.sprintf("Sensor_TRREQ Is Not OFF Or Sensor_COMPT Is Not On / TP4 Time Out");
                    ShowE84Log(str,700,4);
                    Task=5000;  //Auto Recover
                }
            }
            break;
        case 800:
            if(SW[SwE84_2_READY].Status()==true)
            {
                if(iCount!=100)
                {
                    SW[SwE84_2_READY].Off();
                    E84UnLoadDelay.SetSecAndOn(TestIF_File.iE84TimeOut_K12[1][4]);           //TP5
                    str.sprintf("Switch_READY OFF");
                    ShowE84Log(str,800,2);
                    Task=900;
                }
                else
                {
                    bCS0=false;
                    bCS1=false;
                    if(Sen[SnE84_2_CS0].IsOn())
                    {
                        str.sprintf("Sensor_CS0 On");
                        ShowE84Log(str,800,2);
                        bCS0=true;
                    }

                    if(Sen[SnE84_2_CS1].IsOn())
                    {
                        str.sprintf("Sensor_CS1 On");
                        ShowE84Log(str,800,2);
                        bCS1=true;
                    }

                    if(bCS0==true && bCS1==false)
                        iCount=0;
                    else if(bCS0==false && bCS1==true)
                        iCount=1;
                    else if(bCS0==true && bCS1==true)
                        iCount=2;
                    else
                        iCount=100;
                }
            }
            else if(E84UnLoadDelay.Off())
            {
                str.sprintf("Switch_READY Is Not OFF / TA3 Time Out");
                ShowE84Log(str,800,4);
                Task=5000;  //Auto Recover
            }
            break;
        case 900:
            if(SW[SwE84_2_READY].Status()==false)
            {
                if(Sen[SnE84_2_VALID].IsOff() && Sen[SnE84_2_COMPT].IsOff() && Sen[SnE84_2_CS0].IsOff() && Sen[SnE84_2_CS1].IsOff())
                {
                    if(iCount==iPlaceWhichBuffer[1])
                    {
                        str.sprintf("Finish Unloader Action");
                        ShowE84Log(str,900,2);
                        bE84UnloaderActionflag[iPlaceWhichBuffer[1]]=false;
                        bE84Unloaderflag[iPlaceWhichBuffer[1]]=false;
                        Task=1000;                                                  //Finish
                    }
                    else
                    {
                        Task=2000;
                    }
                }
                else if(E84UnLoadDelay.Off())
                {
                    str.sprintf("Sensor_VALID Or Sensor_COMPT Or Sensor_CS0 Or Sensor_CS1 Is Not OFF / TP5 Time Out");
                    ShowE84Log(str,900,4);
                    Task=5000;  //Auto Recover
                }
            }
            break;
        case 1000:
            bE84UnloaderActionflag[iPlaceWhichBuffer[1]]=false;
            bE84Unloaderflag[iPlaceWhichBuffer[1]]=false;
            Task=1;
            break;
        case 2000:
            asStr.sprintf("AGV Pick Form Auto %d,Start Buffer is Auto %d",iCount+1, iPlaceWhichBuffer[1]+1);
            ret=ShowMyMessageBox_YES_NO(asStr, "是否要修改Port狀態?");
            if(ret==1)
            {
                iPlaceWhichBuffer[1]=iCount;
            }
            Task=2100;
            break;
        case 2100:
            str.sprintf("Finish Unloader Action");
            ShowE84Log(str,900,2);
            Task=1000;                                                  //Finish
            break;
        case 5000:              //Auto Recover
            btInitalUnLoad->Click();
            break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAGV::spbSaveClick(TObject *Sender)
{
    AnsiString szDir="";
    szDir="D:\\HT9045\\config\\AGV.ini";

    WriteIniData(szDir, "Configuration", "E84_1 Time Out 1",                      atoi(edE84_1_TP1->Text.c_str()));
    WriteIniData(szDir, "Configuration", "E84_1 Time Out 2",                      atoi(edE84_1_TP2->Text.c_str()));
    WriteIniData(szDir, "Configuration", "E84_1 Time Out 3",                      atoi(edE84_1_TP3->Text.c_str()));
    WriteIniData(szDir, "Configuration", "E84_1 Time Out 4",                      atoi(edE84_1_TP4->Text.c_str()));
    WriteIniData(szDir, "Configuration", "E84_1 Time Out 5",                      atoi(edE84_1_TP5->Text.c_str()));
    WriteIniData(szDir, "Configuration", "E84_1 Time Out 6",                      atoi(edE84_1_TP6->Text.c_str()));

    WriteIniData(szDir, "Configuration", "E84_1 Time Out 7",                      atoi(edE84_1_TA1->Text.c_str()));
    WriteIniData(szDir, "Configuration", "E84_1 Time Out 8",                      atoi(edE84_1_TA2->Text.c_str()));
    WriteIniData(szDir, "Configuration", "E84_1 Time Out 9",                      atoi(edE84_1_TA3->Text.c_str()));

    WriteIniData(szDir, "Configuration", "E84_1 Time Out 10",                     atoi(edE84_1_TD0->Text.c_str()));
    WriteIniData(szDir, "Configuration", "E84_1 Time Out 11",                     atoi(edE84_1_TD1->Text.c_str()));


    WriteIniData(szDir, "Configuration", "E84_2 Time Out 1",                      atoi(edE84_2_TP1->Text.c_str()));
    WriteIniData(szDir, "Configuration", "E84_2 Time Out 2",                      atoi(edE84_2_TP2->Text.c_str()));
    WriteIniData(szDir, "Configuration", "E84_2 Time Out 3",                      atoi(edE84_2_TP3->Text.c_str()));
    WriteIniData(szDir, "Configuration", "E84_2 Time Out 4",                      atoi(edE84_2_TP4->Text.c_str()));
    WriteIniData(szDir, "Configuration", "E84_2 Time Out 5",                      atoi(edE84_2_TP5->Text.c_str()));
    WriteIniData(szDir, "Configuration", "E84_2 Time Out 6",                      atoi(edE84_2_TP6->Text.c_str()));

    WriteIniData(szDir, "Configuration", "E84_2 Time Out 7",                      atoi(edE84_2_TA1->Text.c_str()));
    WriteIniData(szDir, "Configuration", "E84_2 Time Out 8",                      atoi(edE84_2_TA2->Text.c_str()));
    WriteIniData(szDir, "Configuration", "E84_2 Time Out 9",                      atoi(edE84_2_TA3->Text.c_str()));

    WriteIniData(szDir, "Configuration", "E84_2 Time Out 10",                     atoi(edE84_2_TD0->Text.c_str()));
    WriteIniData(szDir, "Configuration", "E84_2 Time Out 11",                     atoi(edE84_2_TD1->Text.c_str()));

    WriteIniData(szDir, "Configuration", "Auto1 Tray Count",                      atoi(edAuto1Count->Text.c_str()));
    WriteIniData(szDir, "Configuration", "Auto2 Tray Count",                      atoi(edAuto2Count->Text.c_str()));
    WriteIniData(szDir, "Configuration", "Auto3 Tray Count",                      atoi(edAuto3Count->Text.c_str()));

    WriteIniData(szDir, "Configuration", "E84 Enable",                            (cbEnableAGVFunction->Checked)?1:0);

    ReadFile();
    spbSave->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall TfAGV::ReadFile()
{
    AnsiString szDir="";
    szDir="D:\\HT9045\\config\\AGV.ini";

    TestIF_File.iE84TimeOut_K12[0][0]       =ReadIniData(szDir, "Configuration", "E84_1 Time Out 1", 2);
    TestIF_File.iE84TimeOut_K12[0][1]       =ReadIniData(szDir, "Configuration", "E84_1 Time Out 2", 2);
    TestIF_File.iE84TimeOut_K12[0][2]       =ReadIniData(szDir, "Configuration", "E84_1 Time Out 3", 60);
    TestIF_File.iE84TimeOut_K12[0][3]       =ReadIniData(szDir, "Configuration", "E84_1 Time Out 4", 60);
    TestIF_File.iE84TimeOut_K12[0][4]       =ReadIniData(szDir, "Configuration", "E84_1 Time Out 5", 2);
    TestIF_File.iE84TimeOut_K12[0][5]       =ReadIniData(szDir, "Configuration", "E84_1 Time Out 6", 2);
    TestIF_File.iE84TimeOut_K12[0][6]       =ReadIniData(szDir, "Configuration", "E84_1 Time Out 7", 2);
    TestIF_File.iE84TimeOut_K12[0][7]       =ReadIniData(szDir, "Configuration", "E84_1 Time Out 8", 120);
    TestIF_File.iE84TimeOut_K12[0][8]       =ReadIniData(szDir, "Configuration", "E84_1 Time Out 9", 60);
    TestIF_File.iE84TimeOut_K12[0][9]       =ReadIniData(szDir, "Configuration", "E84_1 Time Out 10",60);
    TestIF_File.iE84TimeOut_K12[0][10]      =ReadIniData(szDir, "Configuration", "E84_1 Time Out 11",60);

    TestIF_File.iE84TimeOut_K12[1][0]       =ReadIniData(szDir, "Configuration", "E84_2 Time Out 1", 2);
    TestIF_File.iE84TimeOut_K12[1][1]       =ReadIniData(szDir, "Configuration", "E84_2 Time Out 2", 2);
    TestIF_File.iE84TimeOut_K12[1][2]       =ReadIniData(szDir, "Configuration", "E84_2 Time Out 3", 60);
    TestIF_File.iE84TimeOut_K12[1][3]       =ReadIniData(szDir, "Configuration", "E84_2 Time Out 4", 60);
    TestIF_File.iE84TimeOut_K12[1][4]       =ReadIniData(szDir, "Configuration", "E84_2 Time Out 5", 2);
    TestIF_File.iE84TimeOut_K12[1][5]       =ReadIniData(szDir, "Configuration", "E84_2 Time Out 6", 2);
    TestIF_File.iE84TimeOut_K12[1][6]       =ReadIniData(szDir, "Configuration", "E84_2 Time Out 7", 2);
    TestIF_File.iE84TimeOut_K12[1][7]       =ReadIniData(szDir, "Configuration", "E84_2 Time Out 8", 120);
    TestIF_File.iE84TimeOut_K12[1][8]       =ReadIniData(szDir, "Configuration", "E84_2 Time Out 9", 60);
    TestIF_File.iE84TimeOut_K12[1][9]       =ReadIniData(szDir, "Configuration", "E84_2 Time Out 10",60);
    TestIF_File.iE84TimeOut_K12[1][10]      =ReadIniData(szDir, "Configuration", "E84_2 Time Out 11",60);

    TestIF_File.bEnableE84                  =ReadIniData(szDir, "Configuration", "E84 Enable"   , false);
}
//---------------------------------------------------------------------------
void __fastcall TfAGV::DoIniDataToForm()
{
    cbEnableAGVFunction->Checked      =TestIF_File.bEnableE84;

    edE84_1_TP1->Text                 =AnsiString(TestIF_File.iE84TimeOut_K12[0][0]);
    edE84_1_TP2->Text                 =AnsiString(TestIF_File.iE84TimeOut_K12[0][1]);
    edE84_1_TP3->Text                 =AnsiString(TestIF_File.iE84TimeOut_K12[0][2]);
    edE84_1_TP4->Text                 =AnsiString(TestIF_File.iE84TimeOut_K12[0][3]);
    edE84_1_TP5->Text                 =AnsiString(TestIF_File.iE84TimeOut_K12[0][4]);
    edE84_1_TP6->Text                 =AnsiString(TestIF_File.iE84TimeOut_K12[0][5]);
    edE84_1_TA1->Text                 =AnsiString(TestIF_File.iE84TimeOut_K12[0][6]);
    edE84_1_TA2->Text                 =AnsiString(TestIF_File.iE84TimeOut_K12[0][7]);
    edE84_1_TA3->Text                 =AnsiString(TestIF_File.iE84TimeOut_K12[0][8]);
    edE84_1_TD0->Text                 =AnsiString(TestIF_File.iE84TimeOut_K12[0][9]);
    edE84_1_TD1->Text                 =AnsiString(TestIF_File.iE84TimeOut_K12[0][10]);

    edE84_2_TP1->Text                 =AnsiString(TestIF_File.iE84TimeOut_K12[1][0]);
    edE84_2_TP2->Text                 =AnsiString(TestIF_File.iE84TimeOut_K12[1][1]);
    edE84_2_TP3->Text                 =AnsiString(TestIF_File.iE84TimeOut_K12[1][2]);
    edE84_2_TP4->Text                 =AnsiString(TestIF_File.iE84TimeOut_K12[1][3]);
    edE84_2_TP5->Text                 =AnsiString(TestIF_File.iE84TimeOut_K12[1][4]);
    edE84_2_TP6->Text                 =AnsiString(TestIF_File.iE84TimeOut_K12[1][5]);
    edE84_2_TA1->Text                 =AnsiString(TestIF_File.iE84TimeOut_K12[1][6]);
    edE84_2_TA2->Text                 =AnsiString(TestIF_File.iE84TimeOut_K12[1][7]);
    edE84_2_TA3->Text                 =AnsiString(TestIF_File.iE84TimeOut_K12[1][8]);
    edE84_2_TD0->Text                 =AnsiString(TestIF_File.iE84TimeOut_K12[1][9]);
    edE84_2_TD1->Text                 =AnsiString(TestIF_File.iE84TimeOut_K12[1][10]);

    edAuto1Count->Text                =AnsiString(TestIF_File.iLoaderUnloaderTrayCount[0]);
    edAuto2Count->Text                =AnsiString(TestIF_File.iLoaderUnloaderTrayCount[1]);
    edAuto3Count->Text                =AnsiString(TestIF_File.iLoaderUnloaderTrayCount[2]);
}
//---------------------------------------------------------------------------
void __fastcall TfAGV::sbtExitClick(TObject *Sender)
{
    sbtExit->Down=false;
    Close();
    fShow=false;
}
//---------------------------------------------------------------------------
void __fastcall TfAGV::FormShow(TObject *Sender)
{
    ReadFile();
    DoIniDataToForm();
    fShow=true;
}
//---------------------------------------------------------------------------
void __fastcall TfAGV::FormClose(TObject *Sender, TCloseAction &Action)
{
    fShow=false;
}
//---------------------------------------------------------------------------
void __fastcall TfAGV::btInitalLoadClick(TObject *Sender)
{
    InitialE84LoadTask();
    InitialE84LoadSensor();
}
//---------------------------------------------------------------------------
void __fastcall TfAGV::btInitalUnLoadClick(TObject *Sender)
{
    InitialE84UnLoaderTask();
    InitialE84UnloadSensor();
}
//---------------------------------------------------------------------------
void TfAGV::ShowE84Log(AnsiString str, int iFunction, int iLoader)
{
    AnsiString sFileName="", sMegTime="", asE84Data, asLoader;
    AnsiString asStr="";

    asE84Data="D:\\HT9045_Log\\E84DataTxt\\";
    GetTimeInfo();
    sFileName.sprintf("%s%04d%02d\\%02d%02d", asE84Data, SystemYear, SystemMonth, SystemMonth, SystemDate);
    if(!(DirectoryExists(sFileName)))
        ForceDirectories(sFileName);

    sFileName.sprintf("%s%04d%02d\\%02d%02d\\%04d%02d%02d.txt", asE84Data, SystemYear, SystemMonth, SystemMonth, SystemDate, SystemYear, SystemMonth, SystemDate);

    sMegTime.sprintf("%04d-%02d-%02d %02d:%02d:%02d %03d",SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec);


    if(str!="")
    {
        if(iLoader==1 || iLoader==3)
            asLoader="Loader";
        else if(iLoader==2 || iLoader==4)
            asLoader="Unloader";
        asStr.sprintf("%s Task: %d Message: %s",asLoader ,iFunction ,str);

        str=sMegTime+","+asLoader+str+"Task"+iFunction;
        mmE84Log->Lines->Add(str);
        WriteDataToFile(sFileName.c_str() , str.c_str());

        if(iLoader==3 || iLoader==4)
            ShowMyMessage(str);
    }

    if(mmE84Log->Lines->Count>=500)
        mmE84Log->Lines->Clear();
}
//---------------------------------------------------------------------------
void TfAGV::E84StatusLog(AnsiString str, bool bflag)
{
    AnsiString sFileName="", sMegTime="", asE84Data;
    AnsiString asStr="";

    asE84Data="D:\\HT9045_Log\\E84DataTxt\\";
    GetTimeInfo();
    sFileName.sprintf("%s%04d%02d\\%02d%02d", asE84Data, SystemYear, SystemMonth, SystemMonth, SystemDate);
    if(!(DirectoryExists(sFileName)))
        ForceDirectories(sFileName);

    sFileName.sprintf("%s%04d%02d\\%02d%02d\\%04d%02d%02d_Status.txt", asE84Data, SystemYear, SystemMonth, SystemMonth, SystemDate, SystemYear, SystemMonth, SystemDate);
    sMegTime.sprintf("%04d-%02d-%02d %02d:%02d:%02d %03d",SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec);


    if(str!="")
    {
        if(bflag)
            asStr="Off->On";
        else
            asStr="On->Off";

        str=sMegTime+" "+str+" "+asStr;
        WriteDataToFile(sFileName.c_str() , str.c_str());
    }
}
//---------------------------------------------------------------------------
void TfAGV::CheckSECSValue()
{
//    bool bflag[3]={false, false, false};

}
//---------------------------------------------------------------------------
void __fastcall TfAGV::edAuto1CountMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 0, 20);
}
//---------------------------------------------------------------------------
void __fastcall TfAGV::edE84_1_TP1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 0, 300);
}
//---------------------------------------------------------------------------
TQPF_Timer ScanSensorTimer[3];
TQPF_Timer ScanLoaderDelayTime;
TQPF_Timer ScanUnloaderDelayTime;
//---------------------------------------------------------------------------
void __fastcall TfAGV::DoE84LoaderScan(int Pos)
{
    static int iOldSec[3];
    static int iCount[3];

    int &Task=iLoaderScanTask[Pos];
    int SenIndex[3]={SnLoaderTrayHasTray_AGV,SnEmptyTrayHasTray_AGV,SnColorTrayHasTray_AGV};

    AnsiString str="", asStr="";

    if(SystemStart==false)
        return;

    switch(Task)
    {
        case 1:
            bE84Loaderflag[Pos]=false;
            Task=100;
            break;
        case 100:
            if(fMain->labAutomation->Caption=="SECS GEM 4:Enable")
            {
                Task=1000;
            }
            break;
        case 1000:   //Check Loader Unloader Tray Count
            if(bE84Loaderflag[Pos]==false)
            {
                if(Sen[SenIndex[Pos]].IsOff()==true)
                {
                    if(iOldSec[Pos]!=SystemSec)
                    {
                        iOldSec[Pos]=SystemSec;
                        iCount[Pos]++;
                    }

                    if(iCount[Pos]>=5)
                    {
                        iCount[Pos]=0;
                        iTrayEnable[Pos]=1;
                        bE84Loaderflag[Pos]=true;
                        Task=2000;
                    }
                }
                else
                {
                    iCount[Pos]=0;
                    Task=100;
                }
            }
            else
            {
                Task=2100;
            }
            break;
        case 2000:
            asSupplementBin.sprintf("P1:%d,P2:%d,P3:%d", iTrayEnable[0], iTrayEnable[1], iTrayEnable[2]);
//            EventReport_K12(SECS_EVENT_K12.AGVSupplement);
            iTrayEnable[Pos]=0;
            Task=100;
            break;
        case 2100:
            if(bE84Loaderflag[Pos]==false)
            {
                Task=100;
            }
            break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAGV::DoE84UnloaderScan(int Pos)
{                                           
    static int iCount[3];

    int &Task=iUnloaderScanTask[Pos];
//    int SenIndex[3]={SnLoaderTrayHasTray_AGV, SnEmptyTrayHasTray_AGV, SnColorTrayHasTray_AGV};

    switch(Task)
    {
        case 1:
            bE84Unloaderflag[Pos]=false;
            Task=100;
            break;
        case 100:
            iCount[Pos]=0;
            Task=1000;
            break;
        case 1000:   //Check Loader Unloader Tray Count
            if(bE84Unloaderflag[Pos]==false)
            {
                if(TestIF_File.iUnloaderTrayCount[Pos]>=TestIF_File.iLoaderUnloaderTrayCount[Pos])  // Need Remove Unloader Tray
                {
                    bE84Unloaderflag[Pos]=true;
                    
                    iCount[Pos]=1;
                    TestIF_File.iUnloaderTrayCount[Pos]=0;

                    Task=1050;
                    break;
                }
                Task=100;
            }
            else
            {
                Task=1200;
            }
            break;
        case 1050:
//            if(bUnloaderTrayReceive[Pos])
            {
//                fMain->Pause();
                ScanUnloaderDelayTime.SetMSAndOn(2000);
                Task=1100;
            }
            break;
        case 1100:
            if(fMain->labAutomation->Caption=="SECS GEM 4:Enable")
            {
                asSupplementBin.sprintf("P4:%d,P5:%d,P6:%d",iCount[0],iCount[1],iCount[2]);
//                EventReport_K12(SECS_EVENT_K12.AGVSupplement);
                iCount[Pos]=0;
                Task=100;
            }
            else if(ScanUnloaderDelayTime.Off())
            {
                ShowMyMessage("請檢查SECS是否連線");
                Task=100;
            }
            break;
        case 1200:
            if(bE84Unloaderflag[Pos]==false)
            {
                fLotInfo->palRemoveTray->Enabled=false;
                fLotInfo->palRemoveTray->Visible=false;

                Task=100;//inital setting
            }
            break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAGV::Timer2Timer(TObject *Sender)
{
    static bool bIn=false;

    if(!fNote->fShow || InitialOK==false)
        return;

    if(bIn==true)
    {
        return;
    }
    bIn=true;

    DoE84Loader();
    DoE84Unloader();

    bIn=false;
}
//---------------------------------------------------------------------------
void __fastcall TfAGV::edAGVWorryingwattingtimeMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 0, 3600);
}
//---------------------------------------------------------------------------
bool TfAGV::IsSPIL_AMR()                                                        //JerryYang 20250521 : For AMR
{
    bool bResult=false;
    if(USE_COVER_TRAYID==tCID_Keyence &&
       IniConfig.bA65_BundleIDList==true &&
       IniConfig.bSPILFunction==true)
    {
        bResult=true;
    }
    return bResult;
}
//---------------------------------------------------------------------------
bool TfAGV::IsATK_AMR()                                                         //Steven 20251210 : ATK AMR
{
    bool bResult=false;
    if(USE_COVER_TRAYID==tCID_NFC &&
       IniConfig.bA65_BundleIDList==true &&
       CUSTOMER_CODE==CC_AMKOR_Korea)
    {
        bResult=true;
    }
    return bResult;
}
//---------------------------------------------------------------------------
bool TfAGV::Use_AMR()                                                           //Steven 20251216 : for AMR
{
    bool bResult=false;
    if(USE_COVER_TRAYID!=tCIDNotUse &&
       IniConfig.bA65_BundleIDList==true)
    {
        bResult=true;
    }
    return bResult;
}
//==============================================================================
void TfAGV::ScanLoadPort()
{
    if(IniConfig.bA65_BundleIDList==true &&
       IniConfig.bSPILFunction==true)                                           //JerryYang 20250521 : For AMR
    {
        bScanLoadPortState_SPIL();
        bScanUnLoadPortState_SPIL(0);  //Auto1
        bScanUnLoadPortState_SPIL(1);  //Auto2
        bScanUnLoadPortState_SPIL(2);  //Auto3

//        bScanFixPortState_SPIL(0);  //Fix1                                    //JerryYang 20250602 : mark
//        bScanFixPortState_SPIL(1);  //Fix2
//        bScanFixPortState_SPIL(2);  //Fix3
    }
    else if(CUSTOMER_CODE==CC_MAXIM ||
            CUSTOMER_CODE==CC_MAXIM_THAILAND)
    {
        bScanLoadPortState_Analog(0);
        bScanLoadPortState_Analog(1);
        bScanLoadPortState_Analog(2);

        bScanUnLoadPortState_Analog(0);  //Auto1
        bScanUnLoadPortState_Analog(1);  //Auto2
        bScanUnLoadPortState_Analog(2);  //Auto3

        bScanFixPortState_Analog(0);  //Fix1
        bScanFixPortState_Analog(1);  //Fix2
        bScanFixPortState_Analog(2);  //Fix3
    }
    else if(IsATK_AMR())
    {
        bScanLoadPortState_ATK();
    }
}
//==============================================================================
bool bIsStackBusy(int iPortID)                                                  //JerryYang 20250505 : AGV  //JerryYang 20250521 : For AMR
{
    bool bResult=true;
//1: Loader port
//2: Empty port
//3: Color port
//4: Auto1 port
//5: Auto2 port
//6: Auto3 port
//7: Fix1 port
//8: Fix2 port
//9: Fix3 port
//10: Auto4 port
//11: Auto5 port
//12: Auto6 port
//13: Fix4 port
//14: Fix5 port
//15: Fix6 port
    if(iPortID>=1 && iPortID<=15)
    {

    }

    return bResult;
}
TQPF_Timer tLoadPortTimer;                                                      //JerryYang 20250521 : For AMR
TQPF_Timer tUnloadPortTimer[3];                                                 //JerryYang 20250521 : For AMR
//==============================================================================
void bScanLoadPortState_SPIL()
{
    bool bHigh=false, bMiddle=false, bLow=false;

    int currentState=-1;

    static int candidateState=-1;

    fLotInfo->ALedLoader->Value=bAskStopPort[ePortLoader];

    if(bPortIsBusy[ePortLoader]==true || bAskStopPort[ePortLoader]==true)       //JerryYang 2020529
    {
        return;
    }

    bHigh=Sen[SnLoaderIsFull].IsOn();
    bMiddle=Sen[SnLoaderTrayHasTray_AGV].IsOn();
    bLow=fMain->ALed1->Value;

    if(bHigh==false && bMiddle==true && bLow==false)                            // 0   Error
    {                                                                           // 1
        currentState=eLoadError;                                                // 0
    }
    else if(bHigh==true && bMiddle==false && bLow==false)                       // 1   Error
    {                                                                           // 0
        currentState=eLoadError;                                                // 0
    }
    else if(bHigh==true && bMiddle==true && bLow==false)                        // 1   Error
    {                                                                           // 1
        currentState=eLoadError;
    }
    else if(bHigh==true && bMiddle==false && bLow==true)                        // 1   Error
    {                                                                           // 1
        currentState=eLoadError;                                                // 0
    }
    else if(bHigh==false && bMiddle==false && bLow==false)                      //Empty
    {                                                                           //0
        currentState=eLoadEmpty;                                                //0
    }                                                                           //0
    else if(bHigh==false && bMiddle==false && bLow==true)                       //Low Wip
    {                                                                           //0
        currentState=eLoadLowWIP;                                               //0
    }                                                                           //0
    else if(bHigh==false && bMiddle==true && bLow==true)                        //Low Wip
    {                                                                           //0
        currentState=eLoadTrayArrived;
    }
    else if(bHigh==true && bMiddle==true && bLow==true)                         //Low Wip
    {                                                                           //0
        currentState=eLoadHighWIP;                                              //0
    }                                                                           //0

    if(currentState!=candidateState)
    {
        candidateState=currentState;
        tLoadPortTimer.SetSecAndOn(3);
        return;                                                                 // 還沒穩定，不做事
    }
    else
    {
        if(tLoadPortTimer.Off()==false)                                         //穩定3秒
            return;

        if(iLastPortStatus[ePortLoader]!=currentState)
        {
            iPortStatus[ePortLoader]=currentState;
            iLastPortStatus[ePortLoader]=currentState;
            EventReport(SECS_EVENT.LoadPortStatusChanged);
        }
        else
        {
            if(bForceSendLoaderIsEmpty)                                         //JerryYang 20250618 : add
            {
                bForceSendLoaderIsEmpty=false;
                iPortStatus[ePortLoader]=currentState;
                iLastPortStatus[ePortLoader]=currentState;
                EventReport(SECS_EVENT.LoadPortStatusChanged);
            }
        }
    }
}
//==============================================================================
void bScanUnLoadPortState_SPIL(int iPos)                                        //JerryYang 20250521 : For AMR
{
    bool bPortHasTray=false;
    int currentState[3]={-1, -1, -1};
    static int candidateState[3]={-1, -1, -1};

    fLotInfo->aLedAuto1->Value=bAskStopPort[ePortAuto1];
    fLotInfo->aLedAuto2->Value=bAskStopPort[ePortAuto2];
    fLotInfo->aLedAuto3->Value=bAskStopPort[ePortAuto3];

    if(bPortIsBusy[ePortAuto1+iPos]==true ||
       bAskStopPort[ePortAuto1+iPos]==true ||
       iPortStatus[ePortAuto1+iPos]==eReadyToUnload ||
       iPortStatus[ePortAuto1+iPos]==eFullBundle)
    {
        return;
    }

    bPortHasTray=Sen[SnAutoTrayHasTray[iPos]].IsOn();

    if(bPortHasTray==false)
    {
        currentState[iPos]=eUnloadEmpty;
//        iPortStatus[ePortAuto1+iPos]=eUnloadEmpty;
//        if(iLastPortStatus[ePortAuto1+iPos]!=iPortStatus[ePortAuto1+iPos])
//        {
//            iLastPortStatus[ePortAuto1+iPos]=eUnloadEmpty;
//            EventReport(SECS_EVENT.Auto1PortStatusChanged+iPos);
//        }
    }
    else
    {
        currentState[iPos]=eUnloadTrayArrived;
    }

    if(currentState[iPos]!=candidateState[iPos])
    {
        candidateState[iPos]=currentState[iPos];
        tUnloadPortTimer[iPos].SetSecAndOn(3);
        return;                                                                 // 還沒穩定，不做事
    }
    else
    {
        if(tUnloadPortTimer[iPos].Off()==false)                                 //穩定3秒
            return;

        if(iLastPortStatus[ePortAuto1+iPos]!=currentState[iPos])
        {
            iPortStatus[ePortAuto1+iPos]=currentState[iPos];
            iLastPortStatus[ePortAuto1+iPos]=currentState[iPos];
            EventReport(SECS_EVENT.Auto1PortStatusChanged+iPos);
        }
    }
}
//==============================================================================
void bScanFixPortState_SPIL(int iPos)                                           //JerryYang 20250521 : For AMR
{
    bool bPortHasTray=false;

    if(bPortIsBusy[ePortFix1+iPos]==true)
    {
        return;
    }

    int iFix=iAutoIndex[eFix1+iPos];

    bPortHasTray=Sen[SnFixedTrayDetect[iFix]].IsOn();

    if(bPortHasTray==false)
    {
        iPortStatus[ePortFix1+iPos]=eFixEmpty;
        if(iLastPortStatus[ePortFix1+iPos]!=iPortStatus[ePortFix1+iPos])
        {
            iLastPortStatus[ePortFix1+iPos]=eFixEmpty;
            EventReport(SECS_EVENT.Fix1PortStatusChanged+iPos);
        }
    }
    else
    {
        iPortStatus[ePortFix1+iPos]=eFixFTrayArrived;
        if(iLastPortStatus[ePortFix1+iPos]!=iPortStatus[ePortFix1+iPos])
        {
            iLastPortStatus[ePortFix1+iPos]=eFixFTrayArrived;
            EventReport(SECS_EVENT.Fix1PortStatusChanged+iPos);
        }
    }
}
//==============================================================================
void bScanLoadPortState_Analog(int iPos)
{
    int currentState[3][2]={{-1, -1}, {-1, -1}, {-1, -1}};
//    int iCurrent[3]={-1, -1, -1};
    static int LoadcandidateState[3]={-1, -1, -1};

    int iLoSureHasTray[3]  ={SnLoaderSureTray,    SenEmptyHasTray,       SenColorHasTray};
    int iLoTrayIsFull[3]   ={SnLoaderIsFull,      SnEmptyIsFull,         SnColorIsFull};
    int iLoSelectHasTray[3]={SnLoaderTrayHasTray, SenEmptySelectHasTray, SenColorSelectHasTray};

    if(Sen[iLoSureHasTray[iPos]].IsOn())
    {
        currentState[iPos][0]=3;                                                //裡面有料
    }
    else if(Sen[iLoSureHasTray[iPos]].IsOff())
    {
        currentState[iPos][0]=5;                                                //裡面沒料
    }

    if(Sen[iLoTrayIsFull[iPos]].Enable && Sen[iLoTrayIsFull[iPos]].IsOn())
    {
        currentState[iPos][1]=2;                                                //外面滿料
    }
    else if(Sen[iLoSelectHasTray[iPos]].IsOff())
    {
        currentState[iPos][1]=4;                                                //外面沒料
    }
    else if(Sen[iLoSelectHasTray[iPos]].IsOn())
    {
        currentState[iPos][1]=1;                                                //若外面有料，則準備入料
    }

    LoadcandidateState[iPos]=currentState[iPos][0]*10+currentState[iPos][1];

    if(iLastPortStatus[ePortLoader+iPos]!=LoadcandidateState[iPos])
    {
        iPortStatus[ePortLoader+iPos]=LoadcandidateState[iPos];
        iLastPortStatus[ePortLoader+iPos]=LoadcandidateState[iPos];
        EventReport(SECS_EVENT.LoadPortStatusChanged+iPos);
    }
}
//==============================================================================
void bScanUnLoadPortState_Analog(int iPos)                                      //JerryYang 20250521 : For AMR
{
    int currentState[3][2]={{-1, -1}, {-1, -1}, {-1, -1}};
//    int iCurrent[3]={-1, -1, -1};
    static int UnloadcandidateState[3]={-1, -1, -1};

    int iUnSureHasTray[3]  ={SnAuto1_Tray_Car,    SnAuto2_Tray_Car,      SnAuto3_Tray_Car};
    int iUnTrayIsFull[3]   ={SnAuto1IsFull,       SnAuto2IsFull,         SnAuto3IsFull};
    int iUnTrayDetect[3]   ={SnAuto1TrayDetect,   SnAuto2TrayDetect,     SnAuto3TrayDetect};

    if(Sen[iUnTrayDetect[iPos]].IsOn())
    {
        currentState[iPos][0]=3;                                                //裡面有料
    }
    else if(Sen[iUnTrayDetect[iPos]].IsOff())
    {
        currentState[iPos][0]=5;                                                //裡面沒料
    }

    if(Sen[iUnTrayIsFull[iPos]].Enable && Sen[iUnTrayIsFull[iPos]].IsOn())
    {
        currentState[iPos][1]=2;                                                //外面滿料
    }
    else if(Sen[iUnSureHasTray[iPos]].IsOff())
    {
        currentState[iPos][1]=4;                                                //外面沒料
    }
    else if(Sen[iUnSureHasTray[iPos]].IsOn())
    {
        currentState[iPos][1]=1;                                                //若外面有料，則準備入料
    }

    UnloadcandidateState[iPos]=currentState[iPos][0]*10+currentState[iPos][1];

    if(iLastPortStatus[ePortAuto1+iPos]!=UnloadcandidateState[iPos])
    {
        iPortStatus[ePortAuto1+iPos]=UnloadcandidateState[iPos];
        iLastPortStatus[ePortAuto1+iPos]=UnloadcandidateState[iPos];
        EventReport(SECS_EVENT.Auto1PortStatusChanged+iPos);
    }
}
//==============================================================================
void bScanFixPortState_Analog(int iPos)                                         //JerryYang 20250521 : For AMR
{
    static int FixcandidateState[MAX_FIX_TRAY]={-1, -1, -1, -1, -1, -1};

    int iCountFix;
    int currentState;

    if(Sen[SnFixedTrayDetect[iPos]].IsOff())
    {
        currentState=5;                                                         //沒盤
    }
    else if(Sen[SnFixedTrayDetect[iPos]].IsOn())
    {
        currentState=3;                                                         //有盤
    }

    iCountFix=MOT[iTempFix[iPos]].HowManyDevice();
    if(iCountFix==0)
    {
        currentState=1;                                                         //有盤料滿，需下料
    }
    else if(iCountFix>0 &&
            Sen[SnFixedTrayDetect[iPos]].IsOn() &&
            iTrayFeed==1)
    {
        currentState=2;                                                         //有盤料未滿，需下料
    }

    FixcandidateState[iPos]=currentState;

    if(iLastPortStatus[ePortFix1+iPos]!=FixcandidateState[iPos])
    {
        iPortStatus[ePortFix1+iPos]=FixcandidateState[iPos];
        iLastPortStatus[ePortFix1+iPos]=FixcandidateState[iPos];
        EventReport(SECS_EVENT.Fix1PortStatusChanged+iPos);
    }
}
//------------------------------------------------------------------------------
int iLoadStateATK[etTrayCount];
int iCurrStateATK[etTrayCount];
AnsiString sDCC_ATK;
AnsiString sOutputBinCode;
enum eATKPortStatus
{
    eATKNoState        =0,                                                      //0: No state (port status unknown)
    eATKTransfer       =1,                                                      //1: Transfer Blocked (Transfer Blocked (tray is loaded in port))
    eATKReadyToLoad    =2,                                                      //2: Ready to Load (port is empty and ready for tray loading)
    eATKReadyToUnload  =3                                                       //3: Ready to Unload (material has been unloaded and can be removed)
};
//------------------------------------------------------------------------------
void bScanLoadPortState_ATK()
{
    static bool bFirstIn=true;
    int iAuto;

    if(bFirstIn)
    {
        bFirstIn=false;
        for(int i=0; i<etTrayCount; i++)
        {
            iLoadStateATK[i]=eATKNoState;
            iCurrStateATK[i]=eATKNoState;
        }
    }

    if(MOT[MMTrayZ].fHasTray ||                                                 //Loader
       MOT[MMTrayY].fHasTray ||
       MOT[MMTrayY].fHasTray)
    {
        iCurrStateATK[etLoader]=eATKTransfer;
    }
    else
    {
        iCurrStateATK[etLoader]=eATKReadyToLoad;
    }

    if(iLoadStateATK[etLoader]!=iCurrStateATK[etLoader])
    {
        iThisPortNo=ePortLoader;
        iThisPortStatus=iCurrStateATK[etLoader];
        EventReport(SECS_EVENT.PortStateUpdated);
        iLoadStateATK[etLoader]=iCurrStateATK[etLoader];
    }

    if(Sen[SnEmptyIsFull].IsOn() ||                                             //Empty
       (iTrayFeed && bCleanOutTrayEnd==false))
    {
        iCurrStateATK[etEmpty]=eATKReadyToUnload;
    }
    else if(MOT[MMEmptyZ].fHasTray ||
            MOT[MEmptyY].fHasTray ||
            MOT[MMEmpty_Car].fHasTray)
    {
        iCurrStateATK[etEmpty]=eATKTransfer;
    }
    else
    {
        iCurrStateATK[etEmpty]=eATKReadyToLoad;
    }

    if(iLoadStateATK[etEmpty]!=iCurrStateATK[etEmpty])
    {
        iThisPortNo=ePortEmpty;
        iThisPortStatus=iCurrStateATK[etEmpty];
        EventReport(SECS_EVENT.PortStateUpdated);
        iLoadStateATK[etEmpty]=iCurrStateATK[etEmpty];
    }

    if(Sen[SnColorIsFull].IsOn())                                               //Color
    {
        iCurrStateATK[etColor]=eATKReadyToUnload;
    }
    else if(MOT[MMColorZ].fHasTray ||
            MOT[MColorY].fHasTray ||
            MOT[MMColor_Car].fHasTray ||
            (iTrayFeed && bCleanOutTrayEnd==false))
    {
        iCurrStateATK[etColor]=eATKTransfer;
    }
    else
    {
        iCurrStateATK[etColor]=eATKReadyToLoad;
    }

    if(iLoadStateATK[etColor]!=iCurrStateATK[etColor])
    {
        iThisPortNo=ePortColor;
        iThisPortStatus=iCurrStateATK[etColor];
        EventReport(SECS_EVENT.PortStateUpdated);
        iLoadStateATK[etColor]=iCurrStateATK[etColor];
    }

    for(int i=etAuto1; i<=etAuto6; i++)                                         //Auto Tray
    {
        iAuto=i-etAuto1;
        if(AUTO3_IS_MAGAZINE==1 &&
           TestIF_File.iMagTraySource==0 &&
           iAuto==iMagAtAuto)
        {
            iCurrStateATK[i]=eATKNoState;
        }
        else if(Prod.iTrayType[iAuto]==tNotUse)
        {
            iCurrStateATK[i]=eATKNoState;
        }
        else if(iTrayFeed)
        {
            if(bCleanOutTrayEnd==true)
            {
                if(IniConfig.bP09TrayEndCanSelectTray)                          //Steven 20200317 : CleanOut後,可以選擇Tray End, 且要退的Tray要在工作檔設定
                {
                    if(TrayForm.Auto[iAuto].TrayEndRecv)
                    {
                        iCurrStateATK[i]=eATKReadyToUnload;
                    }
                    else
                    {
                        iCurrStateATK[i]=eATKTransfer;
                    }
                }
                else if((CUSTOMER_CODE==CC_KYEC_LEE &&                          //wei 20150821 KYEC在ART模式退Pass Bin
                         IniConfig.bA10_AutoReTest  &&
                         Prod.iIsPassT6[iAuto]==1     &&                        //Steven 20240105 : Prod.bIsPass --> Prod.iIsPassT6
                         (LastSet.iRunStartMode==rsmInitial_ART ||
                          LastSet.iRunStartMode==rsmContinuStart_ART ||
                          LastSet.iRunStartMode==rsmContinuRetest_ART)) ||
                        (CUSTOMER_CODE!=CC_KYEC_LEE &&
                         Prod.iIsFailT6[iAuto]==1))                             //Steven 20240105 : Prod.bIsPass --> Prod.iIsFailT6
                {
                    iCurrStateATK[i]=eATKReadyToUnload;
                }
                else
                {
                    iCurrStateATK[i]=eATKTransfer;
                }
            }
            else
            {
                iCurrStateATK[i]=eATKReadyToUnload;
            }
        }
        else if(Sen[SnAutoIsFull[iAuto]].IsOn())
        {
            iCurrStateATK[i]=eATKReadyToUnload;
        }
        else if(Sen[SnAutoTrayHasTray[iAuto]].IsOff())
        {
            iCurrStateATK[i]=eATKReadyToLoad;
        }
        else
        {
            iCurrStateATK[i]=eATKTransfer;
        }

        if(iLoadStateATK[i]!=iCurrStateATK[i])
        {
            iThisPortNo=ePortAuto1+iAuto;
            iThisPortStatus=iCurrStateATK[i];
            EventReport(SECS_EVENT.PortStateUpdated);
            iLoadStateATK[i]=iCurrStateATK[i];
        }
    }

    for(int i=etFix1; i<=etFix6; i++)                                           //Fix Tray
    {
        iAuto=i-etAuto1;                                                        //要扣掉Loader, Empty, Color
        if(Prod.iTrayType[iAuto]==tNotUse)
        {
            iCurrStateATK[i]=eATKNoState;
        }
        else if(Sen[SnFixedTrayDetect[iAuto]].IsOff())
        {
            iCurrStateATK[i]=eATKReadyToLoad;
        }
        else //if(Sen[SnFixedTrayDetect[iAuto]].IsOn())
        {
            if(MOT[iMMAuto[iAuto]].FullIC())
                iCurrStateATK[i]=eATKReadyToUnload;
            else
                iCurrStateATK[i]=eATKTransfer;
        }
    }
}
//------------------------------------------------------------------------------
