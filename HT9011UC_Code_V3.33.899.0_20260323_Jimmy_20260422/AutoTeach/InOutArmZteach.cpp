#include "MachineDefine.h"
#pragma hdrstop

#include "InOutArmZteach.h"

#include "main.h"
#include "aArmHeader.h"
#include "cmydef.h"
#include "uhome.h"
#include "mymotor.h"
#include "uteach.h"
#include "csystem.h"
#include "MyKitSuck.h"
#include "common.h"
#include "mysensor.h"
#include "note.h"
#include "mymessbox.h"
#include "math.h"
#include "myQwertyKeyBoard.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TZteach *Zteach;
int iUseArm=0;//0:  inarm    1:Outarm
int iAutoTeachTask=0;//kevin 20131024 AutoTeach
int iAutoPosTask=0;//kevin 20140505 AutoTeach
int iVaccumOnOff=0;  //kevin 20210322 add Vacuum on off

bool bMove[10]={false, false, false, false, false, false, false, false, false, false};
int iZVacuumPos1[10]={0};//Z軸吸取高度
bool bTestIndexVaccum=false;
int iUseVacuum=0;
int iZ=0;
int iRecordXYPos[2][5][6]={0,0,0};    //kevin 20210629 [2]: 0:X  1:Y   [5]: 0 Inarm 1 Outarm 2:Arm1  3: Arm2 [6]: load shuttle hotplate
int iUseMotor=0;

//Sam 20210204 : Auto Teach Loader Z Record Vacuum On off time
//==>
TQPF_Timer InVacuumTimer;
TQPF_Timer InTimer;

TQPF_Timer OutVacuumTimer;
TQPF_Timer OutTimer;
//<==
//Sam 20210204 : Auto Teach Loader Z Record Vacuum On off time
//---------------------------------------------------------------------------
__fastcall TZteach::TZteach(TComponent* Owner)
        : TForm(Owner)
{
    fShow=false;
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<4; j++)
        {
            bInArmZHome[i][j]=true;
            bOutArmZHome[i][j]=true;
        }
    }
    pCanvasScale=new TCanvas;
    DcSGScale = GetDC(sgThreePointTeach->Handle);
}
//---------------------------------------------------------------------------
void __fastcall TZteach::SpeedButton11Click(TObject *Sender)
{
    /*
    if(rgTeachMode->ItemIndex==0)  //JerryYang 20161012 是否使用3點校正
        b3PointTeach=false;
    else
        b3PointTeach=true;
     */
    b3PointTeach=false;
    bZTeachOffset=false;             //kevin 20210204 Z Teach Offset
    sbFix2->Visible=true;
    sbFix3->Visible=true;
    sbAuto2->Visible=true;
    sbAuto3->Visible=true;
    rgTeachMode->Visible=true;
    AutoTeachSingle->Visible=true;
    sbInSht1->Visible=true;
    sbOutSht1->Visible=true;
    sbHotPlate1->Visible=true;
    bEnableUseAreaZCalbration=false;
    Panel2->Enabled = false;            //kevin 20210225
    if(LastSet.iRealDummy!=REALLY && iTeachMode == 2)     //kevin 20210331 add Teach mode
    {
        ShowErrorMessage("WAR16316", 0, MMSystem, 0, "Autoteach Z.");
        return;
    }
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<5; j++)
        {
            for(int k=0;k<6;k++)
            {
                iRecordXYPos[i][j][k]=0;    //kevin 20210629 [2]: 0:X  1:Y   [5]: 0 Inarm 1 Outarm 2:Arm1  3: Arm2 [6]: load shuttle hotplate
            }
        }
    }

    switch(iTeachMode)                  //kevin 20210204 Auto Teach mode
    {
        case 0:
            b3PointTeach=false;
            break;
        case 1:
            b3PointTeach=true;
            break;
        case 2:
            if(CUSTOMER_CODE !=CC_ASE_KaohSiung)  //kevin 20210205 add
            {
                sbFix2->Visible=false;
                sbFix3->Visible=false;
                sbAuto2->Visible=false;
                sbAuto3->Visible=false;
                rgTeachMode->Visible=false;
                AutoTeachSingle->Visible=false;
                sbInSht1->Visible=false;
                sbOutSht1->Visible=false;
                sbHotPlate1->Visible=false;
            }
            bEnableUseAreaZCalbration=true;
            break;
    }
    bRecordVacuumTim = CbEnRecordVacuum->Checked;     //kevin 20210224 記錄計算真空產生器壽命

    if(AutoTeachSingle->ItemIndex==0)
    {
        SingleTeach=true;   //使用單一選項TEACH
    }
    else
    {
        iTeachPos=1;
        SingleTeach=false;
    }
#ifndef SOFT_SIMULTE
    //jou 2014-06-17 修正沒裝sensor時不能做Z軸高度校正
    if(Sen[SnAutoTeach].Enable==false && (AutoTeachSingle->ItemIndex!=0 || iTeachPos!=0))
    {
        Memo1->Lines->Add("No Install SnAutoTeach sensor for database.");
        return;
    }
#endif

#ifdef SOFT_SIMULTE
    fMain->BtnStartClick(fMain);
#endif
}
//---------------------------------------------------------------------------
TQPF_Timer DoPlaceDeviceDelay;      //Frank QQQ
//---------------------------------------------------------------------------
bool TZteach::AutoTeachZ(bool Reset,int InArm, bool bUseSingleZDoCalibration, int TeachPos)
{
    int i=0, j=0, k=0;
    static int &iTask=iAutoTeachTask;
    static bool bflag[8]={false};
    static int iZVacuumbufferPos[10]={0};//Z軸吸取高度
    static bool bUseZ[10]={false}; //目前已確認 吸嘴
    static int iZMot=0; //使用 Z軸馬達
    AnsiString aStringbuffer="";
    static bool bFlag=false;
    int iStandZ;
    int iLimtZ;
    int iZ[8];
    double dBuffer=0.0;
    AnsiString asString, asStr;
    AnsiString sBuffer="";
    //Sam 20210204 : Auto Teach Loader Z Record Vacuum On off time
    //==>

//    double iTimerCount={0};//真空建立時間毫秒
//    double iTimerCount1={0};
    AnsiString asSuck[8]={"A", "B", "C", "D", "E", "F", "G", "H"};
    AnsiString asLog={""};
    //static int iCheckCount[3]={0};

    double iTeachZData1=0;  //kevin 20210310
    int iTeachZData=0;
    AnsiString szDir;
//    static int iVacuumCount=0;
    static int iPickAlarm[8]={0};
    if(Reset)
    {
        iAutoTeachTask=1;
        return false;
    }
    //<==
    //Sam 20210204 : Auto Teach Loader Z Record Vacuum On off time

    TEdit *teInArm[]={AEdit1, AEdit2, AEdit3, AEdit4,
                      AEdit5, AEdit6, AEdit7, AEdit8};

    TEdit *teOutArm[]={AOutEdit1, AOutEdit2, AOutEdit3, AOutEdit4,
                       AOutEdit5, AOutEdit6, AOutEdit7, AOutEdit8};

    int MotorData[2][2][4]=
    {
        {
            {MInArmZA,MInArmZC,MInArmZE,MInArmZG},
            {MInArmZB,MInArmZD,MInArmZF,MInArmZH}
        },
        {
            {MOutArmZA,MOutArmZC,MOutArmZE,MOutArmZG},
            {MOutArmZB,MOutArmZD,MOutArmZF,MOutArmZH}
        }
    };

    int OutMotorData[2][2][4]=
    {
        {
            {MInArmZA,MInArmZC,MInArmZE,MInArmZG},
            {MInArmZB,MInArmZD,MInArmZF,MInArmZH}
        },
        {
            {MOutArmZA,MOutArmZC,MOutArmZE,MOutArmZG},
            {MOutArmZB,MOutArmZD,MOutArmZF,MOutArmZH}
        }
    };

    switch(iTask)
    {
        case 1:   //初始化參數
            for(i=0; i<2; i++)
            {
                for(j=0; j<4; j++)
                {
                    iPickAlarm[k]=0;
                    iZVacuumPos1[k]=0;
                    iZVacuumbufferPos[k]=0;
                    if(bUseSingleZDoCalibration)
                    {
                        if(InArm==1 && TestIF.iTestMode == SingleSite)
                        {
                        if(k==6 )
                            bUseZ[k]=false;
                        else
                            bUseZ[k]=true;
                        }
                        else
                        {
                            if(k==0 )
                                bUseZ[k]=false;
                            else
                                bUseZ[k]=true;
                        }
                    }
                    else
                    {
                        bUseZ[k]=false;
                    }
                    bflag[k]=false;
                    k++;
                }
            }
            InArm=iUseArm;
            sVacuumData="";
            iTask++;
            break;
        case 2:
            if(InArm==0)//INARM
            {
                //Inarm Z歸零
                for(int i=0; i<2; i++)
                {
                    for(int j=0; j<4; j++)
                    {
                        bInArmZHome[i][j]=false;
                        InitProcessSingleMotorTask( MotorData[0][i][j]);
                    }
                }
            }
            else
            {
                 //Outarm Z歸零
                for(int i=0; i<2; i++)
                {
                    for(int j=0; j<4; j++)
                    {
                        bOutArmZHome[i][j]=false;
                        InitProcessSingleMotorTask( OutMotorData[1][i][j]);
                    }
                }
                 //OutArm
            }
            iTask=10;
            break;
        case 10:   //真空確認是否正常
            if(InArm==0)//INARM
            {
                if(OpenSuckOn(InArmSuck, true))//開真空 確認真空是否正常
                {
                    //真空產生器有問題無法做AUTO Z
                    iTask=1;
                    Memo1->Lines->Add("Out arm  vaccume On /Off error"); //kevin 20150714
                }
                else
                    iTask++;
            }
            else
            {       //OutARM
                if(OpenSuckOn(OutArmSuck, true))//開真空 確認真空是否正常
                {
                    //真空產生器有問題無法做AUTO Z
                    iTask=31;
                    Memo1->Lines->Add("Out arm  vaccume On /Off error"); //kevin 20150714
                }
                else
                    iTask++;
            }
            break;
         case 11:
            if(InArm==0)//INARM
            {
                if(OpenSuckOn(InArmSuck,false))//開真空 確認真空是否正常
                {
                    //真空產生器有問題無法做AUTO Z
                    iTask=1;
                }
                else
                    iTask++;
            }
            else
            {
                if(OpenSuckOn(OutArmSuck,false))//開真空 確認真空是否正常
                {
                    //真空產生器有問題無法做AUTO Z
                    iTask=1;
                }
                else
                    iTask++;
            }
            if(bUseSingleZDoCalibration)                     //Frank QQQ
            {
                iTask=14;
            }
            break;
        case 12:
            if(DoZHome()==false) //Z軸歸零
                return false;

            if(iHome==0)   //kevin 20210312 add home ok
            {
                iTask=14;
                return false;
            }
            if(InArm==0)//INARM
            {
                if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||
                   USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                //Ztex 2024.02.24 Add HT-1132
                {
                    InitProcessSingleMotorTask(MInArmPitchX2);
                    InitProcessSingleMotorTask(MInArmPitchY);
                }
                else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||             //QQ
                        USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                //Ztex 2023.12.06 Add HT-1032
                {
                }
                InitProcessSingleMotorTask(MInArmPitch);
                InitProcessSingleMotorTask(MInArmX);
                InitProcessSingleMotorTask(MInArmY); //XY HOME
                iTask++;
            }
            else
            {
                if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||
                   USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                //Ztex 2024.02.24 Add HT-1132
                {
                    InitProcessSingleMotorTask(MOutArmPitchX2);
                    InitProcessSingleMotorTask(MOutArmPitchY);
                }
                else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||             //QQ
                        USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                //Ztex 2023.12.06 Add HT-1032
                {
                }
                InitProcessSingleMotorTask(MOutArmPitch);
                InitProcessSingleMotorTask(MOutArmX);
                InitProcessSingleMotorTask(MOutArmY); //XY HOME
                iTask++;
            }
            break;
        case 13: //X y motor  home
            if(InArm==0)//INARM
            {
                OpenSuckOn(InArmSuck,true);

                if(bflag[0]==false)
                    bflag[0]= ProcessSingleMotorHome(MInArmX);
                if(bflag[1]==false)
                    bflag[1]= ProcessSingleMotorHome(MInArmY);
                if(bflag[2]==false)
                    bflag[2]= ProcessSingleMotorHome(MInArmPitch);

                if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||
                   USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                //Ztex 2024.02.24 Add HT-1132
                {
                    if(bflag[3]==false)
                        bflag[3]= ProcessSingleMotorHome(MInArmPitchX2);

                    if(bflag[4]==false)
                        bflag[4]= ProcessSingleMotorHome(MInArmPitchY);
                }
                else
                {
                    bflag[3]= true;
                    bflag[4]= true;
                }

                if(bflag[0]&&bflag[1]&& bflag[2] && bflag[3]&& bflag[4])
                    iTask++;
            }
            else
            {
                OpenSuckOn(OutArmSuck,true);
                if(bflag[0]==false)
                    bflag[0]= ProcessSingleMotorHome(MOutArmX);
                if(bflag[1]==false)
                    bflag[1]= ProcessSingleMotorHome(MOutArmY);
                if(bflag[2]==false)
                    bflag[2]= ProcessSingleMotorHome(MOutArmPitch);

                if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||                  //kevin 20220125 add
                   USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                //Ztex 2024.02.24 Add HT-1132
                {
                    if(bflag[3]==false)
                        bflag[3]= ProcessSingleMotorHome(MOutArmPitchX2);

                    if(bflag[4]==false)
                        bflag[4]= ProcessSingleMotorHome(MOutArmPitchY);
                }
                else
                {
                    bflag[3]= true;
                    bflag[4]= true;
                }

                if(bflag[0]&&bflag[1]&& bflag[2] && bflag[3]&& bflag[4])
                    iTask++;
            }
            break;
        case 14:
            k=0;
            bMove[0]=false;
            bMove[1]=false;
            bMove[2]=false;
            bMove[3]=false;
            bMove[4]=false;
            bMove[5]=false;
            bMove[6]=false; //kevin 20210210 add XY PITCH
            /*
            for(i=0; i<2; i++)
            {
                for(j=0; j<4; j++)
                {
                    if(bUseZ[k]==false)
                    {
                        MOT[MInArmX].SetSpeed(10);
                        MOT[MInArmY].SetSpeed(10);
                        MOT[MInArmZA+k].SetSpeed(5);
                        MOT[MInArmPitch].SetSpeed(5);


                        MOT[MOutArmX].SetSpeed(10);
                        MOT[MOutArmY].SetSpeed(10);
                        MOT[MOutArmZA+k].SetSpeed(5);
                        MOT[MOutArmPitch].SetSpeed(5);

                        iTask++;
                        iZMot=k;
                        return false;
                    }
                    k++;
                }
            }*/
            MOT[MInArmX].SetSpeed(10);
            MOT[MInArmY].SetSpeed(10);
            MOT[MInArmPitch].SetSpeed(5);
            MOT[MOutArmX].SetSpeed(10);
            MOT[MOutArmY].SetSpeed(10);
            MOT[MOutArmPitch].SetSpeed(5);
            if(bZTeachOffset)            //kevin 20210317 Z Teach Offset  OK  開始做各別 Z TEACH
            {
                bUseZ[0]=false;         //A
                bUseZ[1]=true;         //B
                bUseZ[2]=true;         //C
                bUseZ[3]=true;         //D
                bUseZ[4]=true;         //E
                bUseZ[5]=true;         //F
                bUseZ[6]=true;         //G
                bUseZ[7]=true;         //H
            }

            for(i=0; i<8; i++)
            {
                    if(bUseZ[k]==false)
                    {
                        MOT[MInArmZA+k].SetSpeed(5);
                        MOT[MOutArmZA+k].SetSpeed(5);
                        iTask++;
                        iZMot=k;
                        return false;
                    }
                    k++;

            }

            iTask=30;
            break;
        case 15:
            if(bUseSingleZDoCalibration)                                   //Frank QQQ
            {
                if(InArm==0)//INARM
                {
                    iZVacuumbufferPos[iZMot]=iInArmTeachZ;//kevin 20170502 (wei) 快拆吸嘴IN/Out Arm 行程影響AutoTeach -1500;
                    if(iTeachPos==4 || iTeachPos==5)//kevin 20150724 hotplate
                       iZVacuumbufferPos[iZMot]=10;
                    else  if(iTeachPos==1)
                            iZVacuumbufferPos[iZMot]=-350;
                    MOT[MInArmZA+iZMot].MotorMove(iZVacuumbufferPos[iZMot]);
                }
                else
                {
                    iZVacuumbufferPos[iZMot]=ioutArmTeachZ;//kevin 20170502 (wei) 快拆吸嘴IN/Out Arm 行程影響AutoTeach -1500;
                    MOT[MOutArmZA+iZMot].MotorMove(iZVacuumbufferPos[iZMot]);
                }
                iTask++;
            }
            else  //Frank 20171213 (Steven) modify
            {
                if(MoveInArmXYToTeach(iZMot,InArm))
                {
                    if(InArm==0)//INARM
                    {
                        iZVacuumbufferPos[iZMot]=iInArmTeachZ;//kevin 20170502 (wei) 快拆吸嘴IN/Out Arm 行程影響AutoTeach -1500;
                        if(iTeachPos==4 || iTeachPos==5)//kevin 20150724 hotplate
                           iZVacuumbufferPos[iZMot]=10;
                        else  if(iTeachPos==1)
                                iZVacuumbufferPos[iZMot]=-350;
                        MOT[MInArmZA+iZMot].MotorMove(iZVacuumbufferPos[iZMot]);
                    }
                    else
                    {
                        iZVacuumbufferPos[iZMot]=ioutArmTeachZ;//kevin 20170502 (wei) 快拆吸嘴IN/Out Arm 行程影響AutoTeach -1500;
                        if(TeachPos==6)    //Fix 1                              //Frank QQQ
                            iZVacuumbufferPos[iZMot]=-800;
                        MOT[MOutArmZA+iZMot].MotorMove(iZVacuumbufferPos[iZMot]);
                    }
                    iTask++;
                }
            }
            break;
        case 16:
            if(InArm==0)//INARM
            {
                if(MOT[MInArmZA+iZMot].MotorMove(iZVacuumbufferPos[iZMot]))
                {
                    iTask++;
                    MOT[MInArmZA+iZMot].SetSpeed(5); //kevin 20210316 0.3->5
                }
            }
            else
            {
                if(MOT[MOutArmZA+iZMot].MotorMove(iZVacuumbufferPos[iZMot]))
                {
                    iTask++;
                    MOT[MOutArmZA+iZMot].SetSpeed(5);//kevin 20210316 0.3->5
                }
            }
            break;
        case 17:
            iLimtZ=3000;                 //kevin 20210310 2200=>  3000                  //Frank QQQ
            /*
            if(bUseSingleZDoCalibration)
            {
                if(TeachPos==1 || TeachPos==2)         //shuttl 2
                    iLimtZ=2500;    //2300
            } */

            if(InArm==0)//INARM
            {
                iZVacuumbufferPos[iZMot]-=1;

                if(iZVacuumbufferPos[iZMot]<-iLimtZ) //判斷Z軸是否失步
                {
                    MOT[MInArmZA+iZMot].PCIL132_StopMotor();
                    //aStringbuffer =  "M"+IntToStr(MInArmZA+iZMot)+" Z pos over check Vacuum";
                    //Memo1->Lines->Add(aStringbuffer);
                    iPickAlarm[iZMot]++;
                    iTask=20;
                    return false;
                }

                MOT[MInArmZA+iZMot].MotorMove(iZVacuumbufferPos[iZMot]);

                if(OpenSuckOn(InArmSuck,true))//開真空 確認真空是否正常
                {
                    MOT[MInArmZA+iZMot].PCIL132_StopMotor();
                    iZVacuumPos1[iZMot] = MOT[MInArmZA+iZMot].ReadPos();
                    if(iZVacuumbufferPos[iZMot] != iZVacuumPos1[iZMot])
                           iZVacuumbufferPos[iZMot]=0;
                    bUseZ[iZMot]=true;
                    MOT[MInArmZA+iZMot].SetSpeed(5);
                    iTask++;
                }
            }
            else
            {
                iZVacuumbufferPos[iZMot]-=1;

                if(iZVacuumbufferPos[iZMot]<-iLimtZ) //判斷Z軸是否失步
                {
                    MOT[MOutArmZA+iZMot].PCIL132_StopMotor();
                    iPickAlarm[iZMot]++;
                    //aStringbuffer =  "M"+IntToStr(MOutArmZA+iZMot)+" Z pos over check Vacuum";
                    //Memo1->Lines->Add(aStringbuffer);
                    iTask=20;
                    return false;
                }

                MOT[MOutArmZA+iZMot].MotorMove(iZVacuumbufferPos[iZMot]);

                if(OpenSuckOn(OutArmSuck,true))//開真空 確認真空是否正常
                {
                    MOT[MOutArmZA+iZMot].PCIL132_StopMotor();
                    iZVacuumPos1[iZMot]= MOT[MOutArmZA+iZMot].ReadPos();
                    if(iZVacuumbufferPos[iZMot] != iZVacuumPos1[iZMot])
                        iZVacuumbufferPos[iZMot]=0;
                    bUseZ[iZMot]=true;
                    MOT[MOutArmZA+iZMot].SetSpeed(5);
                    iTask++;
                }
            }
            break;
        //Sam 20210204 : Auto Teach Loader Z Record Vacuum On off time
        //==>
        case 18:
            if(bRecordVacuumTim)                           //kevin 20210224 記錄計算真空產生器壽命
            {
                iVaccumOnOff=1;  //kevin 20210322 add Vacuum on off
                if(InArm==0)
                {
                    //VacuumOnOffLog("");
                    //VacuumOnOffLog("======>");
                   // asLog.sprintf("Time,In Arm %s Head Vaccum Test Start", asSuck[iZMot]);
                    //VacuumOnOffLog(asLog);
                    //iCheckCount[InArm]=0;
                    iTask=100;
                }
                //else if(InArm==2)
                else if(InArm==1)   //KEVIN 20210629
                {
                    //VacuumOnOffLog("");
                    //VacuumOnOffLog("======>");
                    //asLog.sprintf("Out Arm %s Head Vaccum Test Start", asSuck[iZMot]);
                    //VacuumOnOffLog(asLog);
                    //iCheckCount[InArm]=0;
                    //iTask=200;
                    iTask=100;
                }
            }
            else
            {
                iTask=19;
            }
            break;
        //<==
        //Sam 20210204 : Auto Teach Loader Z Record Vacuum On off time
        case 19:
            if(bUseSingleZDoCalibration==true)             //Frank QQQ
            {
                bFlag=false;
                iTask=1000;
                break;
            }
            if(InArm==0)//INARM
            {
                if(MoveInArmZToPlateSafe(iTask)==false)
                    return false;
            }
            else
            {
                if(MoveOutArmToAutoSafe_9045()==false)
                    return false;
            }
            iTask=14;
            break;
        case 20: //馬達失步必需重來
            if(InArm==0)//INARM
            {
                //Inarm Z歸零
                for(int i=0; i<2; i++)
                {
                    for(int j=0; j<4; j++)
                    {
                        bInArmZHome[i][j]=false;
                        InitProcessSingleMotorTask( MotorData[0][i][j]);
                    }
                }
                iTask++;
            }
            else
            {
                //Outarm Z歸零
                for(int i=0; i<2; i++)
                {
                    for(int j=0; j<4; j++)
                    {
                        bOutArmZHome[i][j]=false;
                        InitProcessSingleMotorTask( OutMotorData[1][i][j]);
                    }
                }
                iTask++;//OutArm
                //OutArm
            }
            break;
        case 21:
            if(DoZHome()==false) //Z軸歸零
                return false;
            if(iPickAlarm[iZMot] >2)
            {
                aStringbuffer =  "M"+IntToStr(MOutArmZA+iZMot)+" Z pos over check Vacuum";
                Memo1->Lines->Add(aStringbuffer);
                fMain->Pause("AutoTeachZ 21");
            }

            iTask=14;  //Frank 20171213 (Steven) modify
            break;
        case 30:
            if(InArm==0)//INARM
            {
                for(i=0; i<8; i++)
                    teInArm[i]->Text=iZVacuumPos1[i];
                //------------------------------------------
                iStandZ=atoi(teInArm[4]->Text.c_str());
                for(i=0; i<8; i++)
                {
                    iZ[i]=atoi(teInArm[i]->Text.c_str())-iStandZ;
                    teInArm[i]->Text=iZ[i];
                }
            }
            else
            {
                for(i=0; i<8; i++)
                    teOutArm[i]->Text=iZVacuumPos1[i];
                //------------------------------------------
                iStandZ=atoi(teOutArm[4]->Text.c_str());
                for(i=0; i<8; i++)
                {
                    iZ[i]=atoi(teOutArm[i]->Text.c_str())-iStandZ;
                    teOutArm[i]->Text=iZ[i];
                }
            }
            iTask++;
            break;
        case 31:
            InitAutoPosTask();
            if(bEnableUseAreaZCalbration) //kevin 20210205 add 繼續 校正
            {
                bRecordVacuumTim=false;
                return true;
            }
            else
            {
                fMain->Pause("AutoTeachZ 31");
            }
            break;
        //Sam 20210204 : Auto Teach Loader Z Record Vacuum On off time
        //==>
        case 100:
            if(bVacuumOnOff(InArm,iZMot))  // 0: inarm 1: out arm 3: Index  //kevin 20210322 add Vacuum on off
            {
                iTask=19;
                //return true;
            }
            break;
       /*
        case 100:       //先將吸嘴內的真空破壞    0  2  4  6      0,0   0,1
            iInRow=iSuckI[iZMot%2];            // 1  3  5  7      1,0   1,1
            iInCol=iSuckJ[iZMot];
            InArmSuck.Suck[iInRow][iInCol].Off();
            InVacuumTimer.SetSecAndOn(0.5);
            iTask=110;
            break;
        case 110:
             if(InVacuumTimer.Off())
             {
                InArmSuck.Suck[iInRow][iInCol].On();
                iTask=120;
             }
            break;
        case 120:
            //InArmSuck.Suck[iInRow][iInCol].On();
            asLog.sprintf("%s,InArm %s,%d",Now().FormatString("yyyy/mm/dd hh:nn:ss"), asSuck[iZMot], iCheckCount[InArm]+1);
            sVacuumData=asLog;
            //VacuumOnOffLog(asLog);
            InTimer.LatchCycleTimeUS(true);
            iTask=130;
            //break;
        case 130:
            #ifdef SOFT_SIMULTE
                bStatus=true;
            #else
                bStatus=InArmSuck.Suck[iInRow][iInCol].Sensor();//GetStatus();
            #endif

            if(bStatus)
            {
                InArmSuck.Suck[iInRow][iInCol].Normal();
                iTimerCount=InTimer.LatchCycleTimeUS(false);
                //asLog.sprintf("In Arm %s Head Vaccum On :(%d ms)", asSuck[iZMot], (double)iTimerCount/100000);
                iTimerCount1 = (double)iTimerCount/100000;
                asLog.sprintf(",%f",iTimerCount1);
                sVacuumData+= asLog;
                //VacuumOnOffLog(asLog);
                iTask=140;
            }
            else if(InTimer.LatchCycleTimeUS(false)>1000000)    //真空建立超出1s
            {
                asLog.sprintf("In Arm %s Head Vaccum On 測試異常", asSuck[iZMot]);
                VacuumOnOffLog(asLog);
                VacuumOnOffLog("<======");
                ShowMyMessage(asLog);
                InArmSuck.Suck[iInRow][iInCol].Normal();
                iTask=19;
            }
            break;
        case 140:
            //InArmSuck.Suck[iInRow][iInCol].Off();
            InTimer.LatchCycleTimeUS(true);
            iTask=150;
            //break;
        case 150:
            bStatus=true;
            #ifdef SOFT_SIMULTE
                bStatus=false;
            #else
                bStatus=InArmSuck.Suck[iInRow][iInCol].Sensor();//GetStatus();
            #endif

            if(bStatus ==false)
            {
                iTimerCount=InTimer.LatchCycleTimeUS(false);
                //asLog.sprintf("In Arm %s Head Vaccum Off:(%d ms)", asSuck[iZMot],(double)iTimerCount/100000);
                iTimerCount1 = (double)iTimerCount/100000;
                asLog.sprintf(",%f",iTimerCount1);
                sVacuumData+= asLog;
                VacuumOnOffLog(sVacuumData);
                iTask=160;
            }
            else if(InTimer.LatchCycleTimeUS(false)>1000000)    //真空破壞超出1s
            {
                InArmSuck.Suck[iInRow][iInCol].Normal();
                asLog.sprintf("In Arm %s Head Vaccum Off 測試異常", asSuck[iZMot]);
                VacuumOnOffLog(asLog);
                VacuumOnOffLog("<======");
                ShowMyMessage(asLog);
                iTask=100;
            }
            break;
        case 160:
            iCheckCount[InArm]++;
            if(iCheckCount[InArm]>=iCheckMax)
            {
                //asLog.sprintf(",In Arm %s Head Vaccum 測試完成", asSuck[iZMot]);
                //sVacuumData+= asLog;
                //VacuumOnOffLog(sVacuumData);
                iTask=19;
            }
            else
            {

                iTask=100;
            }
            sVacuumData="";
            break;
        case 200:       //先將吸嘴內的真空破壞
            iOutRow=iSuckI[iZMot];
            iOutCol=iSuckJ[iZMot];
            OutArmSuck.Suck[iOutRow][iOutCol].Off();
            OutVacuumTimer.SetSecAndOn(0.2);
            iTask=210;
            break;
        case 210:
            if(OutVacuumTimer.Off())
            {
                iTask=220;
            }
            break;
        case 220:
            asLog.sprintf("Out Arm %s Head Vaccum Test %d", asSuck[iZMot], iCheckCount[InArm]+1);
            VacuumOnOffLog(asLog);
            OutTimer.LatchCycleTimeUS(true);
            OutArmSuck.Suck[iOutRow][iOutCol].On();
            iTask=230;
            //break;
        case 230:
            #ifdef SOFT_SIMULTE
                bStatus=true;
            #else
                bStatus=OutArmSuck.Suck[iOutRow][iOutCol].GetStatus();
            #endif
            if(bStatus)
            {
                iTimerCount=OutTimer.LatchCycleTimeUS(false);
                //asLog.sprintf("Out Arm %s Head Vaccum On :(%d ms)", asSuck[iZMot], iTimerCount);
                iTimerCount1 = (double)iTimerCount/100000;
                asLog.sprintf("Out Arm %s Head Vaccum On :(%f ms)", asSuck[iZMot], iTimerCount1);
                VacuumOnOffLog(asLog);
                iTask=240;
            }
            else if(OutTimer.LatchCycleTimeUS(false)>1000)    //真空建立超出1s
            {
                OutArmSuck.Suck[iOutRow][iOutCol].Normal();
                asLog.sprintf("Out Arm %s Head Vaccum On 測試異常", asSuck[iZMot]);
                VacuumOnOffLog(asLog);
                VacuumOnOffLog("<======");
                ShowMyMessage(asLog);
                iTask=19;
            }
            break;
        case 240:
            OutTimer.LatchCycleTimeUS(true);
            OutArmSuck.Suck[iOutRow][iOutCol].Off();
            iTask=250;
            //break;
        case 250:
            if(OutArmSuck.Suck[iOutRow][iOutCol].GetStatus()==false)
            {
                iTimerCount=OutTimer.LatchCycleTimeUS(false);
                //asLog.sprintf("Out Arm %s Head Vaccum Off:(%d ms)", asSuck[iZMot],iTimerCount);
                iTimerCount1 = (double)iTimerCount/100000;
                asLog.sprintf("Out Arm %s Head Vaccum Off :(%f ms)", asSuck[iZMot], iTimerCount1);
                VacuumOnOffLog(asLog);
                iTask=260;
            }
            else if(OutTimer.LatchCycleTimeUS(false)>1000)    //真空破壞超出1s
            {
                OutArmSuck.Suck[iOutRow][iOutCol].Normal();
                asLog.sprintf("Out Arm %s Head Vaccum Off 測試異常", asSuck[iZMot]);
                VacuumOnOffLog(asLog);
                VacuumOnOffLog("<======");
                ShowMyMessage(asLog);
                iTask=19;
            }
            break;
        case 260:
            iCheckCount[InArm]++;
            if(iCheckCount[InArm]>=iCheckMax)
            {
                asLog.sprintf("Out Arm %s Head Vaccum 測試完成", asSuck[iZMot]);
                VacuumOnOffLog(asLog);
                VacuumOnOffLog("<======");
                bRecordVacuumTim=false;
            }
            iTask=19;
            break;
        //<==
        //Sam 20210204 : Auto Teach Loader Z Record Vacuum On off time
        */
        //Frank QQQ
        case 1000:
            if(InArm==0)
            {
                if(bEnableUseAreaZCalbration) //kevin 20210205 add 繼續 校正
                    bFlag=MOT[MInArmZA+iZMot].MotorMove(iZVacuumPos1[iZMot]);
                else
                    bFlag=MOT[MInArmZA+iZMot].MotorMove(iZVacuumPos1[iZMot]+50);
            }
            else
            {
                if(bEnableUseAreaZCalbration) //kevin 20210205 add 繼續 校正
                    bFlag=MOT[MOutArmZA+iZMot].MotorMove(iZVacuumPos1[iZMot]);
                else
                    bFlag=MOT[MOutArmZA+iZMot].MotorMove(iZVacuumPos1[iZMot]+50);
            }

            if(bFlag==true)
            {
                iTask=1100;
            }
            break;
        case 1100:
            if(InArm==0)
            {
                for(int X=0;X<2;X++)
                {
                    for(int Y=0;Y<4;Y++)
                    {
                        InArmSuck.Suck[X][Y].Off();
                    }
                }
            }
            else
            {
                for(int X=0;X<2;X++)
                {
                    for(int Y=0;Y<4;Y++)
                    {
                        OutArmSuck.Suck[X][Y].Off();
                    }
                }
            }
            DoPlaceDeviceDelay.SetSecAndOn(1);
            iTask=1200;
            break;
        case 1200:
            if(DoPlaceDeviceDelay.Off())
            {
                if(InArm==0)//INARM
                {
                    if(MoveInArmZToPlateSafe(1111)==false)
                       return false;
                }
                else
                {
                    if(MoveOutArmToAutoSafe_9045()==false)
                       return false;
                }

                for(int X=0;X<2;X++)
                {
                    for(int Y=0;Y<4;Y++)
                    {
                        InArmSuck.Suck[X][Y].Normal();
                    }
                }
                iTask=1300;
            }
            break;
        case 1300:
            k=0;
            iTeachZData = iZVacuumPos1[iZMot];
            if(InArm==0)//INARM
            {
                if(bEnableUseAreaZCalbration) //kevin 20210205 add 繼續 校正
                {
                    if(TeachPos==1)         //shuttl 1
                    {
                        for(int j=0; j<4; j++)
                        {
                            for(int i=0; i<2; i++)
                            {
                                InArmOffSet[3]->SetPickUp(i, j, 0);
                                InArmOffSet[3]->SetPlace(i, j, 0);
                            }
                        }
                        iStandZ = atoi(teInArm[0]->Text.c_str());

                        InArmOffSet[3]->GetX();

                        iTeachZData1= (double)(MOT[0].ReadEncoderPos()-iRecordXYPos[0][iUseArm][TeachPos])/100;    //kevin 20210629 Shuttle
                        dBuffer=iTeachZData1+(InArmOffSet[3]->GetX()/100);  //原本Offset  + 新 X Offset

                        if(fabs(dBuffer)>5.0)
                        {
                            ShowErrorMessage("WAR16319", 0, MMSystem, 0, "In Shuttle 1 X.");
                        }
                        else
                        {
                            InArmOffSet[3]->SetX(dBuffer);
                        }

                        iTeachZData1= (double)(MOT[1].ReadEncoderPos()-iRecordXYPos[1][iUseArm][TeachPos])/100;    //kevin 20210708 Shuttle
                        dBuffer=iTeachZData1+(double)(InArmOffSet[3]->GetY()/100);  //原本Offset  + 新 X Offset

                        if(fabs(dBuffer)>5.0)
                        {
                            ShowErrorMessage("WAR16319", 0, MMSystem, 0, "In Shuttle 1 Y.");
                        }
                        else
                        {
                            InArmOffSet[3]->SetY(dBuffer);
                        }

                        if(DeviceForm_File.XDimension <10 || DeviceForm_File.YDimension <10 )        //kevin 20210607 change 5->9  //kevin 20210408 add IC SIZE compare
                            iTeachZData1 = (double)(iTeachZData-Tech.iInArmShuttlePlaceZ +100)/100;   //往下 50
                        else
                            iTeachZData1 = (double)(iTeachZData-Tech.iInArmShuttlePlaceZ +150)/100;    //往下 50
                        if(fabs(iTeachZData1)>3.0)
                        {
                            ShowErrorMessage("WAR16315", 0, MMSystem, 0, "Shuttle 1 Z Teach.");
                        }
                        else
                        {
                            InArmOffSet[3]->SetPlace(iTeachZData1);    // offset 歸0
                            SaveFile(3,1, false);
                            sBuffer.sprintf("%d",iTeachZData);
                            StringGrid1->Cells[6][1]= sBuffer;   //kevin 20210205
                            sBuffer.sprintf("%3.2f",(iTeachZData1));
                            StringGrid1->Cells[7][1]= sBuffer;   //kevin 20210205 z OFFSET
                        }
                    }
                    else if(TeachPos==2)         //shuttl 2
                    {
                        for(int j=0; j<4; j++)
                            for(int i=0; i<2; i++)
                            {

                                InArmOffSet[4]->SetPickUp(i, j, 0);
                                InArmOffSet[4]->SetPlace(i, j, 0);
                            }
                        iStandZ = atoi(teInArm[k]->Text.c_str());

                        iTeachZData1= (double)(MOT[0].ReadEncoderPos()-iRecordXYPos[0][iUseArm][TeachPos])/100;    //kevin 20210629 Shuttle
                        dBuffer=iTeachZData1+(InArmOffSet[4]->GetX()/100);  //原本Offset  + 新 X Offset
                        if(fabs(dBuffer)>5.0)
                        {
                            ShowErrorMessage("WAR16319", 0, MMSystem, 0, "In Shuttle 2 X.");
                        }
                        else
                        {
                            InArmOffSet[4]->SetX(dBuffer);
                        }

                        iTeachZData1= (double)(MOT[1].ReadEncoderPos()-iRecordXYPos[1][iUseArm][TeachPos])/100;    //kevin 20210629 Shuttle
                        dBuffer=iTeachZData1+(InArmOffSet[4]->GetY()/100);  //原本Offset  + 新 X Offset
                        if(fabs(dBuffer)>5.0)
                        {
                            ShowErrorMessage("WAR16319", 0, MMSystem, 0, "In Shuttle 2 Y.");
                        }
                        else
                        {
                            InArmOffSet[4]->SetY(dBuffer);
                        }


                        if(DeviceForm_File.XDimension <10 || DeviceForm_File.YDimension <10 )        //kevin 20210607 change 5->10  //kevin 20210607 change IC SIZE compare
                            iTeachZData1 = (double)(iTeachZData-Tech.iInArmShuttlePlaceZ +100)/100; //往下 50
                        else
                            iTeachZData1 = (double)(iTeachZData-Tech.iInArmShuttlePlaceZ +150)/100;  //往下 50
                        if(fabs(iTeachZData1)>3.0)
                        {
                            ShowErrorMessage("WAR16315", 0, MMSystem, 0, "Shuttle 2 Z Teach.");
                        }
                        else
                        {
                            InArmOffSet[4]->SetPlace(iTeachZData1);     // offset 歸0
                            SaveFile(4,1, false);
                            sBuffer.sprintf("%d",iTeachZData);
                            StringGrid1->Cells[6][2]= sBuffer;   //kevin 20210205
                            sBuffer.sprintf("%3.2f",(iTeachZData1));
                            StringGrid1->Cells[7][2]= sBuffer;   //kevin 20210205 z OFFSET
                        }
                    }
                    else if(TeachPos==3)    //LOAD
                    {
                        for(int j=0; j<4; j++)
                            for(int i=0; i<2; i++)
                            {
                                InArmOffSet[0]->SetPickUp(i, j, 0);
                                InArmOffSet[0]->SetPlace(i, j, 0);
                            }
                        iStandZ = atoi(teInArm[0]->Text.c_str());
                        iTeachZData1= (double)(MOT[0].ReadEncoderPos()-iRecordXYPos[0][iUseArm][TeachPos])/100;    //kevin 20210629 Shuttle
                        dBuffer=iTeachZData1+(InArmOffSet[0]->GetX()/100);  //原本Offset  + 新 X Offset
                        if(fabs(dBuffer)>5.0)
                        {
                            ShowErrorMessage("WAR16319", 0, MMSystem, 0, "Load X.");
                        }
                        else
                        {
                            InArmOffSet[0]->SetX(dBuffer);
                        }

                        iTeachZData1= (double)(MOT[1].ReadEncoderPos()-iRecordXYPos[1][iUseArm][TeachPos])/100;    //kevin 20210629 Shuttle
                        dBuffer=iTeachZData1+(InArmOffSet[0]->GetY()/100);  //原本Offset  + 新 X Offset
                        if(fabs(dBuffer)>5.0)
                        {
                            ShowErrorMessage("WAR16319", 0, MMSystem, 0, "Load Y.");
                        }
                        else
                        {
                            InArmOffSet[0]->SetY(dBuffer);           //kevin 20210629 Sh
                        }

                        iTeachZData1 = (double)(iTeachZData-Tech.iInArmLoadStagePickZ2)/100;     //kevin 20210607 change  iStandZ
                        if(fabs(iTeachZData1)>3.0)
                        {
                            ShowErrorMessage("WAR16315", 0, MMSystem, 0, "Load Z Teach.");
                        }
                        else
                        {
                            InArmOffSet[0]->SetPickUp(iTeachZData1-0.5);    // offset 歸0
                            SaveFile(0,1, false);
                            sBuffer.sprintf("%d",iTeachZData);
                            StringGrid1->Cells[6][3]= sBuffer;   //kevin 20210310
                            sBuffer.sprintf("%3.2f",(iTeachZData1));
                            StringGrid1->Cells[7][3]= sBuffer;   //kevin 20210205 z OFFSET
                        }
                    }
                    else if(TeachPos==4)    //Hotplate 1
                    {
                        for(int j=0; j<4; j++)
                            for(int i=0; i<2; i++)
                            {
                                InArmOffSet[1]->SetPickUp(i, j, 0);
                                InArmOffSet[1]->SetPlace(i, j, 0);
                            }
                        iStandZ = atoi(teInArm[0]->Text.c_str());

                        iTeachZData1 = (double)(iTeachZData-Tech.iInArmPlatePickZ2)/100;   //kevin 20210607 change  iStandZ
                        iTeachZData1= (double)(MOT[0].ReadEncoderPos()-iRecordXYPos[0][iUseArm][TeachPos])/100;    //kevin 20210629 Shuttle
                        dBuffer=iTeachZData1+(InArmOffSet[1]->GetX()/100);  //原本Offset  + 新 X Offset
                        if(fabs(dBuffer)>5.0)
                        {
                            ShowErrorMessage("WAR16319", 0, MMSystem, 0, "Hotplate 1 X.");
                        }
                        else
                        {
                            InArmOffSet[1]->SetX(dBuffer);
                        }

                        iTeachZData1= (double)(MOT[1].ReadEncoderPos()-iRecordXYPos[1][iUseArm][TeachPos])/100;    //kevin 20210629 Shuttle
                        dBuffer=iTeachZData1+(InArmOffSet[1]->GetY()/100);  //原本Offset  + 新 X Offset
                        if(fabs(dBuffer)>5.0)
                        {
                            ShowErrorMessage("WAR16319", 0, MMSystem, 0, "Hotplate 1 Y.");
                        }
                        else
                        {
                            InArmOffSet[1]->SetY(dBuffer);
                        }

                        if(fabs(iTeachZData1)>3.0)
                        {
                            ShowErrorMessage("WAR16315", 0, MMSystem, 0, "Hotplate 1.");
                        }
                        else
                        {
                            InArmOffSet[1]->SetPickUp(iTeachZData1-0.5);    // offset 歸0
                            if(DeviceForm_File.XDimension <10 || DeviceForm_File.YDimension <10 )          //kevin 20210607 change 5->10//kevin 20210408 add IC SIZE compare
                                InArmOffSet[1]->SetPlace(iTeachZData1-1);    //PLACE  offset +2
                            else
                                InArmOffSet[1]->SetPlace(iTeachZData1-0.5);    //PLACE  offset +2
                            SaveFile(1,1, false);
                            //sBuffer.sprintf("%d",iTeachZData);
                            sBuffer.sprintf("%d",iTeachZData);        //kevin 20210310 add i teach Z offset hogh
                            StringGrid1->Cells[6][4]= sBuffer;   //kevin 20210205
                            sBuffer.sprintf("%3.2f",(iTeachZData1));
                            StringGrid1->Cells[7][4]= sBuffer;   //kevin 20210205 z OFFSET
                        }
                    }
                    else if(TeachPos==5)    //Hotplate 2
                    {

                        for(int j=0; j<4; j++)
                            for(int i=0; i<2; i++)
                            {
                                //iStandZ = atoi(teInArm[k]->Text.c_str());
                                //iTeachZData1 = (double)(Tech.iInArmPlatePickZ2 - iTeachZData+ iStandZ)/100;
                                InArmOffSet[2]->SetPickUp(i, j, 0);   //kevin 20210323
                                InArmOffSet[2]->SetPlace(i, j, 0);
                                //k++;
                            }
                        iStandZ = atoi(teInArm[0]->Text.c_str());

                        iTeachZData1= (double)(MOT[0].ReadEncoderPos()-iRecordXYPos[0][iUseArm][TeachPos])/100;    //kevin 20210629 Shuttle
                        dBuffer=iTeachZData1+(InArmOffSet[2]->GetX()/100);  //原本Offset  + 新 X Offset
                        if(fabs(dBuffer)>5.0)
                        {
                            ShowErrorMessage("WAR16319", 0, MMSystem, 0, "Hotplate 2X.");
                        }
                        else
                        {
                            InArmOffSet[2]->SetX(dBuffer);
                        }

                        iTeachZData1= (double)(MOT[1].ReadEncoderPos()-iRecordXYPos[1][iUseArm][TeachPos])/100;    //kevin 20210629 Shuttle
                        dBuffer=iTeachZData1+(InArmOffSet[2]->GetY()/100);  //原本Offset  + 新 X Offset
                        if(fabs(dBuffer)>5.0)
                        {
                            ShowErrorMessage("WAR16319", 0, MMSystem, 0, "Hotplate 2 Y.");
                        }
                        else
                        {
                            InArmOffSet[2]->SetY(dBuffer);
                        }
                        iTeachZData1 = (double)(iTeachZData-Tech.iInArmPlatePickZ2)/100;//kevin 20210607 change  iStandZ
                        if(fabs(iTeachZData1)>3.0)
                        {
                            ShowErrorMessage("WAR16315", 0, MMSystem, 0, "Hotplate 2.");
                        }
                        else
                        {
                            InArmOffSet[2]->SetPickUp(iTeachZData1-0.5);    // offset 歸0
                            if(DeviceForm_File.XDimension <10 || DeviceForm_File.YDimension <10 )          //kevin 20210607 change 5->10 add IC SIZE compare
                                InArmOffSet[2]->SetPlace(iTeachZData1-1);    //PLACE  offset +2
                            else
                                InArmOffSet[2]->SetPlace(iTeachZData1-0.5);    //PLACE  offset +2
                            SaveFile(2,1, false);
                            sBuffer.sprintf("%d",iTeachZData);
                            StringGrid1->Cells[6][5]= sBuffer;   //kevin 20210205
                            sBuffer.sprintf("%3.2f",(iTeachZData1));
                            StringGrid1->Cells[7][5]= sBuffer;   //kevin 20210205 z OFFSET
                        }
                    }
                }
                else
                {
                    if(TeachPos==2)         //shuttl 2
                        fTeach->SetEditPlaceInShuttle->Text=iZVacuumPos1[iZMot];
                    else if(TeachPos==3)    //LOAD
                        fTeach->SetEditPickLoader->Text=iZVacuumPos1[iZMot];
                    else if(TeachPos==5)    //Hotplate 2
                        fTeach->SetEditHP->Text=iZVacuumPos1[iZMot];
                }
            }
            else
            {
                if(bEnableUseAreaZCalbration) //kevin 20210205 add 繼續 校正
                {
                    if(TeachPos==1)         //shuttl 1
                    {
                        for(int j=0; j<4; j++)
                            for(int i=0; i<2; i++)
                            {
                                OutArmOffSet_File[0]->SetPickUp(i, j, 0);     //kevin 20210720 change data
                                OutArmOffSet_File[0]->SetPlace(i, j, 0);      //kevin 20210720 change data
                                //k++;
                            }
                        iStandZ = atoi(teOutArm[0]->Text.c_str());

                        iTeachZData1= (double)(MOT[19].ReadEncoderPos()-iRecordXYPos[0][iUseArm][TeachPos])/100;    //kevin 20210629 Shuttle
                        dBuffer=iTeachZData1+(OutArmOffSet_File[OutOfsOutSh1]->GetX()); //kevin 20210720  //原本Offset  + 新 X Offset
                        if(fabs(dBuffer)>5.0)
                        {
                            ShowErrorMessage("WAR16319", 0, MMSystem, 0, "Out Shuttle 1.");
                        }
                        else
                        {
                            OutArmOffSet_File[OutOfsOutSh1]->SetX(dBuffer);
                        }

                        iTeachZData1= (double)(MOT[20].ReadEncoderPos()-iRecordXYPos[1][iUseArm][TeachPos])/100;    //kevin 20210629 Shuttle
                        dBuffer=iTeachZData1+(OutArmOffSet_File[OutOfsOutSh1]->GetY());  //原本Offset  + 新 X Offset
                        if(fabs(dBuffer)>5.0)
                        {
                            ShowErrorMessage("WAR16319", 0, MMSystem, 0, "Out Shuttle 1.");
                        }
                        else
                        {
                            OutArmOffSet_File[OutOfsOutSh1]->SetY(dBuffer);
                        }

                        //iTeachZData1 = (double)(iTeachZData-Tech.iOutArmShuttlePickZ2+ iStandZ)/100;
                        iTeachZData1 = (double)(iTeachZData-Tech.iOutArmShuttlePickZ2)/100;    //kevin 20210607 change  iStandZ
                        if(fabs(iTeachZData1)>3.0)
                        {
                            ShowErrorMessage("WAR16315", 0, MMSystem, 0, "Out Shuttle 1.");
                        }
                        else
                        {
                            OutArmOffSet_File[0]->SetPickUp(iTeachZData1-0.5);    // offset 歸0
                            SaveFile(5,1, false);
                            sBuffer.sprintf("%d",iTeachZData);
                            StringGrid1->Cells[6][1]= sBuffer;   //kevin 20210205
                            sBuffer.sprintf("%3.2f",(iTeachZData1));
                            StringGrid1->Cells[7][1]= sBuffer;   //kevin 20210205 z OFFSET
                        }
                    }
                    else     //shuttl 2
                    {
                        for(int j=0; j<4; j++)
                            for(int i=0; i<2; i++)
                            {
                                OutArmOffSet_File[OutOfsOutSh2]->SetPickUp(i, j, 0);   //kevin 20210720 change
                                OutArmOffSet_File[OutOfsOutSh2]->SetPlace(i, j, 0);    //kevin 20210720 change
                            }
                        iStandZ = atoi(teOutArm[k]->Text.c_str());

                        iTeachZData1= (double)(MOT[19].ReadEncoderPos()-iRecordXYPos[0][iUseArm][TeachPos])/100;    //kevin 20210629 Shuttle
                        dBuffer=iTeachZData1+(OutArmOffSet_File[OutOfsOutSh2]->GetX()); //kevin 20210720   //原本Offset  + 新 X Offset
                        if(fabs(dBuffer)>5.0)
                        {
                            ShowErrorMessage("WAR16319", 0, MMSystem, 0, "Out Shuttle 1.");
                        }
                        else
                        {
                            OutArmOffSet_File[OutOfsOutSh2]->SetX(dBuffer);
                        }

                        iTeachZData1= (double)(MOT[20].ReadEncoderPos()-iRecordXYPos[1][iUseArm][TeachPos])/100;    //kevin 20210629 Shuttle
                        dBuffer=iTeachZData1+(OutArmOffSet_File[OutOfsOutSh2]->GetY()); //kevin 20210720  //原本Offset  + 新 X Offset
                        if(fabs(dBuffer)>5.0)
                        {
                            ShowErrorMessage("WAR16319", 0, MMSystem, 0, "Out Shuttle 1.");
                        }
                        else
                        {
                            OutArmOffSet_File[OutOfsOutSh2]->SetY(dBuffer);          //kevin 20210720
                        }

                        iTeachZData1 = (double)(iTeachZData-Tech.iOutArmShuttlePickZ2)/100;    //kevin 20210720  change  iStandZ
                        if(fabs(iTeachZData1)>3.0)
                        {
                            ShowErrorMessage("WAR16315", 0, MMSystem, 0, "Out Shuttle 2.");
                        }
                        else
                        {
                            OutArmOffSet_File[OutOfsOutSh2]->SetPickUp(iTeachZData1-0.5);    //kevin 20210720
                            SaveFile(6,1, false);
                            sBuffer.sprintf("%d",iTeachZData);
                            StringGrid1->Cells[6][2]= sBuffer;   //kevin 20210205
                            sBuffer.sprintf("%3.2f",(iTeachZData1));
                            StringGrid1->Cells[7][2]= sBuffer;   //kevin 20210205 z OFFSET
                        }
                    }
                }
                else
                {
                    if(TeachPos==2)         //shuttl 2
                        fTeach->SetEditPickOutSht->Text=iZVacuumPos1[iZMot];
                    else if(TeachPos==3)    //Auto 1
                        fTeach->SetEditPlaceAuto->Text=iZVacuumPos1[iZMot];
                    else if(TeachPos==6)    //Fix 1
                        fTeach->SetEditPlaceFix->Text=iZVacuumPos1[iZMot];
                }
            }
            iTask=1400;
            break;
        case 1400:
            if(InArm==0)
            {
                if(TeachPos==2)         //shuttl 2
                    asString="Shuttle 2";
                else if(TeachPos==3)    //LOAD
                    asString="Loader ";
                else if(TeachPos==5)    //Hotplate 2
                    asString="HotPlate 2";
                asStr.sprintf("InArm Z Calibration in %s Area Finish",asString);
            }
            else
            {
                if(TeachPos==2)         //shuttl 2
                    asString="Shuttle 2";
                else if(TeachPos==3)    //Auto 1
                    asString="Auto 1 ";
                else if(TeachPos==6)    //Fix 1
                    asString="Fix 1";
                asStr.sprintf("OutArm Z Calibration in %s Area Finish",asString);
            }

            Memo1->Lines->Add(asStr); //kevin 20150728
            InitAutoPosTask();
            return true;
    }
    return false;   //kevin 20140515
}
//------------------------------------------------------------------------------
//   移動XY位置
//------------------------------------------------------------------------------
bool TZteach::MoveInArmXYToTeach(int ZNumber, int iInArm)
{                                //目前使用吸嘴
    int iX, iY, iVari, iYVariable=0, iXVariable2=0;
    int iXPos=0, iYPos=0, iYPitchData=0;
    int iInOutArmMoveYpos= -52730;   //kevin 20210312 add

    if(iInArm==0)
    {
        if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||                          //kevin 20181203 : Y變距
           USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                        //Ztex 2024.02.24 Add HT-1132
        {
            iInOutArmMoveYpos=-69700;                                           //kevin 20210318 add
        }
        else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                     //QQ
                USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                        //Ztex 2023.12.06 Add HT-1032
        {
        }
        else if(MachineTypeChoice==Type_HT9046_LS)
        {
            iInOutArmMoveYpos=-69500;                                           //kevin 20210312 add-69349
        }
    }
    else
    {
        iInOutArmMoveYpos= -52926;   //kevin 20210312 add
        if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||                          //kevin 20181203 : Y變距
           USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                        //Ztex 2024.02.24 Add HT-1132
            iInOutArmMoveYpos= -69700;   //kevin 20210318add
        else if(MachineTypeChoice==Type_HT9046_LS)
            iInOutArmMoveYpos= -69500;   //kevin 20210312 add-69349

    }
    if(iInArm==0)//INARM
    {
        iVari=GetInArmPitch_9045(9000);
        if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||                          //kevin 20181203 : Y變距
           USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                        //Ztex 2024.02.24 Add HT-1132
        {
            iYVariable =GetInArmPitchY_9045(6000);   //ChungHung 20131231 alter AutoYPitch
            iXVariable2=GetInArmPitch2_9045(9000);
            iYPitchData=3300;
        }
        if(ZNumber%2==0)//吸嘴 A C E G
        {
            iXPos=29659-3000*(ZNumber/2);//Tech.iInArmTeachX-3000*(ZNumber/2);
            iYPos=iInOutArmMoveYpos+iInArmAutoYTeachOffset+iYPitchData;//kevin 20210312 change data//kevin 20210210 (wei) add       //jou 2014-10-07 Auto Teach Z 新增 HT9046LS 點位
        }
        else
        {
            iXPos=29659-3000*(ZNumber/2);//Tech.iInArmTeachX-3000*(ZNumber/2);
            iYPos=iInOutArmMoveYpos+6000+iInArmAutoYTeachOffset+iYPitchData;//kevin 20210312  change data
        }

        TransferHotPlateRatio(false, &iXPos, &iYPos);    //Steven 20110324

        iX=iXPos;
        iY=iYPos;
        if(bMove[0]==false)
           bMove[0]=MOT[MInArmX].MotorMove(iX);

        if(bMove[1]==false)
           bMove[1]=MOT[MInArmY].MotorMove(iY);

        if(bMove[2]==false)
           bMove[2]=MOT[MInArmPitch].MotorMove(iVari);

        if(bMove[3]==false)
           bMove[3]=MOT[MInShuttle1].MotorMove(Tech.iInShuttle1Left );

        if(bMove[4]==false)
           bMove[4]=MOT[MInShuttle2].MotorMove(Tech.iInShuttle2Left);

        if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||                          //kevin 20181203 add
           USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                        //Ztex 2024.02.24 Add HT-1132
        {
            if(bMove[5]==false)
                bMove[5]=MOT[MInArmPitchY].MotorMove(iYVariable);  //ChungHung 20131231 alter AutoYPitch
            if(bMove[6]==false)
                bMove[6]=MOT[MInArmPitchX2].MotorMove(iXVariable2); //Steven 20131002 : XY變距
        }
        else
        {
           bMove[5]=true;
           bMove[6]=true;
        }
    }
    else
    {
        iVari=GetOutArmPitch_9045(9000);
        if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||                          //kevin 20181203 : Y變距
           USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                        //Ztex 2024.02.24 Add HT-1132
        {
            iYVariable =GetOutArmPitchY_9045(6000);   //ChungHung 20131231 alter AutoYPitch
            iXVariable2=GetOutArmPitch2_9045(9000);
            iYPitchData=3300;
        }

        if(ZNumber%2==0)//吸嘴 A C E G
        {
            iXPos=-20193-3000*(ZNumber/2);//Tech.iInArmTeachX-3000*(ZNumber/2);
            iYPos=iInOutArmMoveYpos+iOutArmAutoYTeachOffset+iYPitchData; //kevin 20210312 change    //jou 2014-10-07 Auto Teach Z 新增 HT9046LS 點位

        }
        else
        {
            iXPos=-20193-3000*(ZNumber/2);//Tech.iInArmTeachX-3000*(ZNumber/2);
            iYPos=iInOutArmMoveYpos+6000+iOutArmAutoYTeachOffset+iYPitchData;//kevin 20210312 change //kevin 20171115 (wei) add  //jou 2014-10-07 Auto Teach Z 新增 HT9046LS 點位
        }

        TransferHotPlateRatio(false, &iXPos, &iYPos);    //Steven 20110324

        iX=iXPos;
        iY=iYPos;
        if(bMove[0]==false)
            bMove[0]=MOT[MOutArmX].MotorMove(iX);

        if(bMove[1]==false)
            bMove[1]=MOT[MOutArmY].MotorMove(iY);

        if(bMove[2]==false)
            bMove[2]=MOT[MOutArmPitch].MotorMove(iVari);

        if(bMove[3]==false)
           bMove[3]=MOT[MOutShuttle1].MotorMove(Tech.iInShuttle1Right );

        if(bMove[4]==false)
           bMove[4]=MOT[MOutShuttle2].MotorMove(Tech.iInShuttle2Right );

        if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||                          //kevin 20181203 add
           USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                        //Ztex 2024.02.24 Add HT-1132
        {
            if(bMove[5]==false)
                bMove[5]=MOT[MOutArmPitchY].MotorMove(iYVariable);  //ChungHung 20131231 alter AutoYPitch
            if(bMove[6]==false)
                bMove[6]=MOT[MOutArmPitchX2].MotorMove(iXVariable2); //Steven 20131002 : XY變距
        }
        else
        {
           bMove[5]=true;
           bMove[6]=true;
        }
    }

    if(bMove[0] && bMove[1] && bMove[2] && bMove[3] && bMove[4] && bMove[5] && bMove[6])
        return true;
    return false;
}
//------------------------------------------------------------------------------
//   移動XY位置
//------------------------------------------------------------------------------
bool TZteach::MoveInArmXYToTeachPos(int iInArm,int SendXpos,int SendYpos)
{                                //目前使用吸嘴
    int iX,iY,iVari;
    int iXPos=0,iYPos=0;

    if(iInArm==0)//INARM
    {
        iVari=GetInArmPitch_9045(iXpitchMaxX3);     //Isaac 20171204 (Steven) : Xpitch40->50mm, 12000->iXpitchMaxX3

        iXPos=SendXpos;//Tech.iInArmTeachX-3000*(ZNumber/2);
        iYPos=SendYpos;//Tech.iInArmTeachY;

        TransferHotPlateRatio(false, &iXPos, &iYPos);    //Steven 20110324

        iX=iXPos;
        iY=iYPos;
        if(bMove[0]==false)
            bMove[0]=MOT[MInArmX].MotorMove(iX);

        if(bMove[1]==false)
            bMove[1]=MOT[MInArmY].MotorMove(iY);

        if(bMove[2]==false)
            bMove[2]=MOT[MInArmPitch].MotorMove(iVari);

        if(bMove[3]==false)
           bMove[3]=MOT[MInShuttle1].MotorMove(Tech.iInShuttle1Left );

        if(bMove[4]==false)
           bMove[4]=MOT[MInShuttle2].MotorMove(Tech.iInShuttle2Left);
    }
    else
    {
        iVari=GetOutArmPitch_9045(iXpitchMaxX3);    //Isaac 20171204 (Steven) : Xpitch40->50mm, 12000->iXpitchMaxX3

        iXPos=SendXpos;//Tech.iInArmTeachX-3000*(ZNumber/2);
        iYPos=SendYpos;//Tech.iInArmTeachY;

        TransferHotPlateRatio(false, &iXPos, &iYPos);    //Steven 20110324

        iX=iXPos;
        iY=iYPos;
        if(bMove[0]==false)
            bMove[0]=MOT[MOutArmX].MotorMove(iX);

        if(bMove[1]==false)
            bMove[1]=MOT[MOutArmY].MotorMove(iY);

        if(bMove[2]==false)
            bMove[2]=MOT[MOutArmPitch].MotorMove(iVari);

        if(bMove[3]==false)
           bMove[3]=MOT[MInShuttle1].MotorMove(Tech.iInShuttle1Right );

        if(bMove[4]==false)
           bMove[4]=MOT[MInShuttle2].MotorMove(Tech.iInShuttle2Right );
    }

    if(bMove[0] && bMove[1]&&bMove[2]&& bMove[3]&&bMove[4])
        return true;
    return false;
}
//---------------------------------------------------------------------------
//  開真空
//------------------------------------------------------------------------------
bool TZteach::OpenSuckOn(class TMyKitSuck &Ptr,bool CheckSuckOn)
{
    int X=0,Y=0;
    for(X=0;X<2;X++)
    {
        for(Y=0;Y<4;Y++)
        {
            if(Ptr.Suck[X][Y].Enable &&
               Ptr.Suck[X][Y].SenUsing!="")
            {
                if(CheckSuckOn)
                    Ptr.Suck[X][Y].On();
                else
                    Ptr.Suck[X][Y].Normal();
            }
        }
    }
    MySleep(10);
    for(X=0;X<2;X++)
    {
        for(Y=0;Y<4;Y++)
        {
            if(Ptr.Suck[X][Y].Enable &&
               Ptr.Suck[X][Y].SenUsing!="")
            {
                if(Ptr.Suck[X][Y].GetStatus()==true)
                   return true;
            }
        }
    }

    return false;
}
//------------------------------------------------------------------------------
//  真空偵測
//------------------------------------------------------------------------------
bool TZteach::CheckSuckOn(class TMyKitSuck &Ptr,int X,int Y)
{
    if(Ptr.Suck[X][Y].Enable &&
       Ptr.Suck[X][Y].SenUsing!="" )
    {
        if(Ptr.Suck[X][Y].GetStatus()==true)
            return true;
    }
    return false;
}
//---------------------------------------------------------------------------
bool TZteach::DoZHome()
{
    int MotorData[2][2][4]=
    {
        {
            {MInArmZA,MInArmZC,MInArmZE,MInArmZG},
            {MInArmZB,MInArmZD,MInArmZF,MInArmZH}
        },
        {
            {MOutArmZA,MOutArmZC,MOutArmZE,MOutArmZG},
            {MOutArmZB,MOutArmZD,MOutArmZF,MOutArmZH}
        }
    };

    int i, j;
    bool bFlag[2]={false,false};
    for(i=0; i<2; i++)
    {
        for(j=0; j<4; j++)
        {
            if(bInArmZHome[i][j]==false)
            {
                bInArmZHome[i][j]=ProcessSingleMotorHome(MotorData[0][i][j]);
                bFlag[0]=true;
            }
            if(bOutArmZHome[i][j]==false)
            {
                bOutArmZHome[i][j]=ProcessSingleMotorHome(MotorData[1][i][j]);
                bFlag[1]=true;
            }
        }
    }

    if(bFlag[0]==false && bFlag[1]==false)
        return true;
    else
        return false;
}
//---------------------------------------------------------------------------
void __fastcall TZteach::FormShow(TObject *Sender)
{
    fShow=true;
//    InitAutoPosTask();
    StringGrid1->ColCount=8;       //kevin 20210205
    StringGrid1->Cells[1][0]="X";
    StringGrid1->Cells[2][0]="Y";
    StringGrid1->Cells[3][0]="Enable Write";
    StringGrid1->Cells[4][0]="Teach X";
    StringGrid1->Cells[5][0]="Teach Y";
    StringGrid1->Cells[6][0]="Z pos"; //kevin 20210205
    StringGrid1->Cells[7][0]="Z OFFSET"; //kevin 20210205

    sgThreePointTeach->ColCount=5;
    sgThreePointTeach->RowCount=7;
    sgThreePointTeach->ColWidths[0]=20;
    sgThreePointTeach->ColWidths[1]=90;
    sgThreePointTeach->Height=161;
    sgThreePointTeach->Cells[2][0]="Distance";
    sgThreePointTeach->Cells[3][0]="Scale";
    sgThreePointTeach->Cells[1][1]="Jig";
    sgThreePointTeach->Cells[1][2]="Actual";
    sgThreePointTeach->Cells[1][3]="Difference";
    sgThreePointTeach->Cells[1][4]="Jig";
    sgThreePointTeach->Cells[1][5]="Actual";
    sgThreePointTeach->Cells[1][6]="Difference";
    sgThreePointTeach->Cells[4][0]="Angel";

    if(CosFunction.bInOutArmZCalibration)  //Frank 20171213 (Steven) modify
    {
        cbEnableUseAreaZCalbration->Visible=true;
    }
    else
    {
        cbEnableUseAreaZCalbration->Visible=false;
    }

    if(CosFunction.bRecordVacuumOnOffTime)      //kevin 20210224 -Sam 20210204 : Auto Teach Loader Z Record Vacuum On off time
    {
        CbEnRecordVacuum->Visible=true;
    }
    else
    {
        CbEnRecordVacuum->Visible=false;
    }

    if(USE_ROTATE_KIT==0)
    {
        sbInRotate->Visible=false;
        sbOutRotate->Visible=false;
    }

    if(iUseArm==0)
    {
        StringGrid1->RowCount=7;
        StringGrid1->Height=151;
        StringGrid1->Cells[0][1]="In Shuttle 1";
        StringGrid1->Cells[0][2]="In Shuttle 2";
        StringGrid1->Cells[0][3]="Load     ";
        StringGrid1->Cells[0][4]="Hotplate1";
        StringGrid1->Cells[0][5]="Hotplate2";
        StringGrid1->Cells[0][6]="Rotate   ";
        StringGrid1->Cells[4][1]= fTeach->setEditInSht1X->Text;
        StringGrid1->Cells[5][1]= fTeach->setEditInSht1Y->Text;
        StringGrid1->Cells[4][2]= fTeach->setEditInSht2X->Text;
        StringGrid1->Cells[5][2]= fTeach->setEditInSht2Y->Text;
        StringGrid1->Cells[4][3]= fTeach->setEditLoaderX->Text;
        StringGrid1->Cells[5][3]= fTeach->setEditLoaderY->Text;
        StringGrid1->Cells[4][4]= fTeach->setEditHP1X->Text;
        StringGrid1->Cells[5][4]= fTeach->setEditHP1Y->Text;
        StringGrid1->Cells[4][5]= fTeach->setEditHP2X->Text;
        StringGrid1->Cells[5][5]= fTeach->setEditHP2Y->Text;
        StringGrid1->Cells[4][6]= fTeach->setEditRotateX->Text;
        StringGrid1->Cells[5][6]= fTeach->setEditRotateY->Text;
    }
    else
    {
        StringGrid1->RowCount=10;
        StringGrid1->Height=214;
        StringGrid1->Cells[0][1]="Out Shuttle 1";
        StringGrid1->Cells[0][2]="Out Shuttle 2";
        StringGrid1->Cells[0][3]="Auto 1   ";
        StringGrid1->Cells[0][4]="Auto 2";
        StringGrid1->Cells[0][5]="Auto 3";
        StringGrid1->Cells[0][6]="Fix 1";
        StringGrid1->Cells[0][7]="Fix 2";
        StringGrid1->Cells[0][8]="Fix 3";
        StringGrid1->Cells[0][9]="Rotate   ";

        StringGrid1->Cells[4][1]= fTeach->setEditOutSht1X->Text;
        StringGrid1->Cells[5][1]= fTeach->setEditOutSht1Y->Text;
        StringGrid1->Cells[4][2]= fTeach->setEditOutSht2X->Text;
        StringGrid1->Cells[5][2]= fTeach->setEditOutSht2Y->Text;
        StringGrid1->Cells[4][3]= fTeach->setEditAuto1X->Text;
        StringGrid1->Cells[5][3]= fTeach->setEditAuto1Y->Text;
        StringGrid1->Cells[4][4]= fTeach->setEditAuto2X->Text;
        StringGrid1->Cells[5][4]= fTeach->setEditAuto2Y->Text;
        StringGrid1->Cells[4][5]= fTeach->setEditAuto3X->Text;
        StringGrid1->Cells[5][5]= fTeach->setEditAuto3Y->Text;
        StringGrid1->Cells[4][6]= fTeach->setEditFix1X->Text;
        StringGrid1->Cells[5][6]= fTeach->setEditFix1X->Text;
        StringGrid1->Cells[4][7]= fTeach->setEditFix2X->Text;
        StringGrid1->Cells[5][7]= fTeach->setEditFix2Y->Text;
        StringGrid1->Cells[4][8]= fTeach->setEditFix3X->Text;
        StringGrid1->Cells[5][8]= fTeach->setEditFix3Y->Text;
        StringGrid1->Cells[4][9]= fTeach->setEditRotateOutX->Text;
        StringGrid1->Cells[5][9]= fTeach->setEditRotateOutY->Text;
    }
#ifndef SOFT_SIMULTE
    //SpeedButton11->Visible=false;
    //SpeedButton14->Visible=false;
#endif

}
//---------------------------------------------------------------------------
void __fastcall TZteach::FormClose(TObject *Sender, TCloseAction &Action)
{
    fShow=false;
    fAllMotorHome=false;
    bLoadInarmAutoHigh=false;  //kevin 20171107 (wei) Auto teach open vacumm
    cbEnableUseAreaZCalbration->Enabled=true;         //Frank QQQ
}
//---------------------------------------------------------------------------
void __fastcall TZteach::BitBtn1Click(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TZteach::SendInarmClick(TObject *Sender)
{
  int i;
    TEdit *teAutoInArm[]={AEdit1, AEdit2, AEdit3, AEdit4,
                          AEdit5, AEdit6, AEdit7, AEdit8};

    TEdit *teInArm[]={fTeach->setEditZ1A, fTeach->setEditZ1B, fTeach->setEditZ1C, fTeach->setEditZ1D,
                      fTeach->setEditZ1E, fTeach->setEditZ1F, fTeach->setEditZ1G, fTeach->setEditZ1H};

    for(i=0; i<8; i++)
    {
        teInArm[i]->Text=teAutoInArm[i]->Text;
    }
}
//---------------------------------------------------------------------------
void __fastcall TZteach::SendOutarmClick(TObject *Sender)
{
    int i;
    TEdit *teAutoOutArm[]={AOutEdit1, AOutEdit2, AOutEdit3, AOutEdit4,
                           AOutEdit5, AOutEdit6, AOutEdit7, AOutEdit8};

    TEdit *teOutArm[]={fTeach->setEditZ2A, fTeach->setEditZ2B, fTeach->setEditZ2C, fTeach->setEditZ2D,
                       fTeach->setEditZ2E, fTeach->setEditZ2F, fTeach->setEditZ2G, fTeach->setEditZ2H};

    for(i=0; i<8; i++)
    {
        teOutArm[i]->Text=teAutoOutArm[i]->Text;
    }
}
//---------------------------------------------------------------------------
//  Auto teach
//TeachPos:    INARM                   OUTARM
//Z Calibration   0                     Z Calibration  0
//Shuttle 1       1                     Shuttle 1      1
//Shuttle 2       2                     Shuttle 2      2
//Rotate          3                     Rotate         3
//Load            4                     Auto 1         4
//Hotplate1       5                     Auto 2         5
//Hotplate2       6                     Auto 3         6
//                                      Fix 1          7
//                                      Fix 2          8
//                                      Fix 3          9
//---------------------------------------------------------------------------
void TZteach::AutoTeachPos()
{
    static int &iTask=iAutoPosTask;
    static int XMovPos[2][10][6]={0}, iXPos=0, X_NewTechPOS, X_ShiftPOSX=0, X_ShiftPOSY=0, X_Dis, XPOSSource, ZPos=0;
    static int YMovPos[2][10][6]={0}, iYPos=0, Y_NewTechPOS, Y_ShiftPOSX=0, Y_ShiftPOSY=0, Y_Dis, YPOSSource;
    static bool bflag[8]={false};
    static int iTeach3Point=0;
    bool biflag=false;
    int i=0, j=0;
    int iOffset=1; //每次位移位置
    static int InArm=0, TeachPos=0, iJigXDis=0, iJigYDis=0;
    static AnsiString sStr3Point="";
    AnsiString sMode="Find Pos ", sString="", sTemp, sTemp1, asStr="";
    double dAngleX=0.0, dAngleY=0.0, dXScale=1.0, dYScale=1.0;

    int MotorData[2][2][4]=
    {
        {
            {MInArmZA,MInArmZC,MInArmZE,MInArmZG},
            {MInArmZB,MInArmZD,MInArmZF,MInArmZH}
        },
        {
            {MOutArmZA,MOutArmZC,MOutArmZE,MOutArmZG},
            {MOutArmZB,MOutArmZD,MOutArmZF,MOutArmZH}
        }
    };
    int OutMotorData[2][2][4]=
    {
        {
            {MInArmZA,MInArmZC,MInArmZE,MInArmZG},
            {MInArmZB,MInArmZD,MInArmZF,MInArmZH}
        },
        {
            {MOutArmZA,MOutArmZC,MOutArmZE,MOutArmZG},
            {MOutArmZB,MOutArmZD,MOutArmZF,MOutArmZH}
        }
    };

     switch(iTask)
    {
        case 1:   //初始化參數
            for(i=0; i<10; i++)
            {
                for(j=0; j<6; j++)
                {
                    XMovPos[0][i][j]=0;
                    YMovPos[0][i][j]=0;
                    XMovPos[1][i][j]=0;
                    YMovPos[1][i][j]=0;
                }
            }
            SetAutoTeachMode();
            InArm=iUseArm;
            TeachPos=iTeachPos;
            DisplayAutoTeachPos(InArm, TeachPos);
            iTeach3Point=0;
            iXPos=0,iYPos=0;
            X_ShiftPOSX=0, X_ShiftPOSY=0, Y_ShiftPOSX=0, Y_ShiftPOSY=0;
            bMove[0]=false;
            bMove[1]=false;
            bMove[2]=false;
            bMove[3]=false;
            bMove[4]=false;

            cbEnableUseAreaZCalbration->Enabled=false;         //Frank QQQ

            iTask++;
            break;
        case 2:
            if(InArm==0)//INARM
            {
                //Inarm Z歸零
                for(int i=0; i<2; i++)
                {
                    for(int j=0; j<4; j++)
                    {
                        bInArmZHome[i][j]=false;
                        InitProcessSingleMotorTask( MotorData[0][i][j]);
                        bflag[j+i*4]=false;
                    }
                }
            }
            else
            {
                //Outarm Z歸零
                for(int i=0; i<2; i++)
                {
                    for(int j=0; j<4; j++)
                    {
                        bOutArmZHome[i][j]=false;
                        InitProcessSingleMotorTask( OutMotorData[1][i][j]);
                        bflag[j+i*4]=false;
                    }
                }
                //OutArm
            }
            Memo1->Lines->Add("Z motor homeing ...."); //kevin 20150728
            iTask++;
            break;
        case 3:
            if(DoZHome()==false) //Z軸歸零
                return;

            if(iHome==0)   //kevin 20210312 add home ok
            {
                iTask=5;
                return;
            }
            if(InArm==0)//INARM
            {
                InitProcessSingleMotorTask(MInArmPitch);
                InitProcessSingleMotorTask(MInArmX);
                InitProcessSingleMotorTask(MInArmY); //XY HOME

                InitProcessSingleMotorTask(MInShuttle1);
                InitProcessSingleMotorTask(MInShuttle2); //shuttle HOME
            }
            else
            {
                InitProcessSingleMotorTask(MOutArmPitch);
                InitProcessSingleMotorTask(MOutArmX);
                InitProcessSingleMotorTask(MOutArmY); //XY HOME

                InitProcessSingleMotorTask(MInShuttle1);
                InitProcessSingleMotorTask(MInShuttle2); //shuttle HOME

            }
            Memo1->Lines->Add("X Y Pitch shuttle motor homeing ...."); //kevin 20150728
            iTask++;
            //kevin 20150728
            break;
        case 4: //X y motor  home
            if(InArm==0)//INARM
            {
                OpenSuckOn(InArmSuck,true);
                if(bflag[0]==false)
                          bflag[0]= ProcessSingleMotorHome(MInArmX);
                if(bflag[1]==false)
                          bflag[1]= ProcessSingleMotorHome(MInArmY);
                if(bflag[2]==false)
                          bflag[2]= ProcessSingleMotorHome(MInArmPitch);

                if(bflag[3]==false)
                          bflag[3]= ProcessSingleMotorHome(MInShuttle1);
                if(bflag[4]==false)
                          bflag[4]= ProcessSingleMotorHome(MInShuttle2);

                if(bflag[0]&&bflag[1]&& bflag[2]&&bflag[3]&& bflag[4])
                    iTask++;
            }
            else
            {
                OpenSuckOn(OutArmSuck,true);
                if(bflag[0]==false)
                          bflag[0]= ProcessSingleMotorHome(MOutArmX);
                if(bflag[1]==false)
                          bflag[1]= ProcessSingleMotorHome(MOutArmY);
                if(bflag[2]==false)
                          bflag[2]= ProcessSingleMotorHome(MOutArmPitch);
                if(bflag[3]==false)
                          bflag[3]= ProcessSingleMotorHome(MInShuttle1);
                if(bflag[4]==false)
                          bflag[4]= ProcessSingleMotorHome(MInShuttle2);

                if(bflag[0]&&bflag[1]&& bflag[2]&&bflag[3]&& bflag[4])
                    iTask++;

            }
            break;
        case 5:
            bMove[0]=false;
            bMove[1]=false;
            bMove[2]=false;
            bMove[3]=false;
            bMove[4]=false;
            DisplayAutoTeachPos(InArm, TeachPos);
            if(InArm==0)// inarm
            {
                if(TeachPos==0  || (bEnableUseAreaZCalbration && CUSTOMER_CODE==CC_ASE_KaohSiung)) //kevin 20210205
                {
                    AutoTeachZ(true,iUseArm,false,TeachPos);//INARM Z 水平校正
                    Memo1->Lines->Add("Inarm Z motor teach Start"); //kevin 20150728
                    iTask=10;
                    break;
                }
                else if(bEnableUseAreaZCalbration)  //Frank 20171213 (Steven) modify
                {
                    if(TeachPos==2)         //shuttl 2
                        sString="Shuttle 2";
                    else if(TeachPos==3)    //LOAD
                        sString="Loader ";
                    else if(TeachPos==5)    //Hotplate 2
                        sString="HotPlate 2";

                    asStr.sprintf("Inarm Z Calibration in %s Area Start",sString);
                    Memo1->Lines->Add(asStr); //kevin 20150728
                    iTask=1000;
                    break;
                }
                else if(TeachPos==1)    //shuttl 1
                {
                    if(b3PointTeach==false || iTeach3Point==0)
                    {
                        XMovPos[0][0][0]=Tech.iInArmShuttle1X;  //每一個圓X 左邊 位置
                        YMovPos[0][0][0]=Tech.iInArmShuttle1Y; //每一個圓Y 左邊 位置
                        ZPos=-690;
                        sString= "Inarm Shuttle 1  motor teach Start"; //kevin 20150728
                    }
                    else
                    {
                        sStr3Point="Inarm Shuttle 1";
                        if(iTeach3Point==1)
                        {
                            iJigXDis=6000;  //JerryYang 20161026 三點校正治具,x方向兩點的距離
                            XMovPos[0][0][0]=Tech.iInArmShuttle1X-iJigXDis;  //每一個圓X 左邊 位置
                            YMovPos[0][0][0]=Tech.iInArmShuttle1Y; //每一個圓Y 左邊 位置
                            ZPos=-690;
                            sString="Inarm Shuttle 1 X-Shift motor teach Start"; //kevin 20150728
                        }
                        else if(iTeach3Point==2)
                        {
                            iJigYDis=6000;   //JerryYang 20161026 三點校正治具,Y方向兩點的距離
                            XMovPos[0][0][0]= Tech.iInArmShuttle1X;  //每一個圓X 左邊 位置
                            YMovPos[0][0][0]=Tech.iInArmShuttle1Y-iJigYDis; //每一個圓Y 左邊 位置
                            ZPos=-690;
                            sString= "Inarm Shuttle 1 Y-Shift motor teach Start"; //kevin 20150728
                        }
                    }
                }
                else if(TeachPos==2)  //shuttl 2
                {
                    if(b3PointTeach==false || iTeach3Point==0)
                    {
                        XMovPos[0][1][0]=Tech.iInArmShuttle2X;  //每一個圓X 左邊 位置
                        YMovPos[0][1][0]=Tech.iInArmShuttle2Y; //每一個圓Y 左邊 位置
                        ZPos=-690;
                        sString= "Inarm Shuttle 2  motor teach Start"; //kevin 20150728
                    }
                    else
                    {
                        sStr3Point="Inarm Shuttle 2";
                        if(iTeach3Point==1)
                        {
                            iJigXDis=6000; //JerryYang 20161026 三點校正治具,X方向兩點的距離

                            XMovPos[0][1][0]=Tech.iInArmShuttle2X-iJigXDis;  //每一個圓X 左邊 位置
                            YMovPos[0][1][0]=Tech.iInArmShuttle2Y; //每一個圓Y 左邊 位置
                            ZPos=-690;
                            sString= "Inarm Shuttle 2 X-Shift motor teach Start";
                        }
                        else if(iTeach3Point==2)
                        {

                            iJigYDis=6000; //JerryYang 20161026 三點校正治具,Y方向兩點的距離
                            XMovPos[0][1][0]=Tech.iInArmShuttle2X;  //每一個圓X 左邊 位置
                            YMovPos[0][1][0]=Tech.iInArmShuttle2Y-iJigYDis; //每一個圓Y 左邊 位置
                            ZPos=-690;
                            sString= "Inarm Shuttle 2 Y-Shift motor teach Start";
                        }

                    }
                }
                else if(TeachPos==3) //LOAD
                {
                    if(b3PointTeach==false || iTeach3Point==0)
                    {
                        XMovPos[0][2][0]=Tech.iInArmLoadStageX;  //每一個圓X 左邊 位置
                        YMovPos[0][2][0]=Tech.iInArmLoadStageY; //每一個圓Y 左邊 位置
                        ZPos=-350;
                        sString= "Inarm Load motor teach Start"; //kevin 20150728
                    }
                    else
                    {
                        sStr3Point="Inarm Loader";
                        if(iTeach3Point==1)    // X-Shift
                        {
                            iJigXDis=11000; //JerryYang 20161026 三點校正治具,X方向兩點的距離
                            XMovPos[0][2][0]=Tech.iInArmLoadStageX+iJigXDis;  //JerryYang 20161012 三點校正Teaching點的右邊校正點  //每一個圓X 左邊 位置
                            YMovPos[0][2][0]=Tech.iInArmLoadStageY; //每一個圓Y 左邊 位置
                            ZPos=-350;
                            sString= "Inarm Load X-Shift motor teach ...";
                        }
                        else if(iTeach3Point==2)    // Y-Shift
                        {
                            iJigYDis=27000; //JerryYang 20161026 三點校正治具,Y方向兩點的距離
                            XMovPos[0][2][0]=Tech.iInArmLoadStageX;  //每一個圓X 左邊 位置
                            YMovPos[0][2][0]=Tech.iInArmLoadStageY-iJigYDis; //JerryYang 20161012 三點校正Teaching點的下面校正點  //每一個圓Y 左邊 位置
                            ZPos=-350;
                            sString= "Inarm Load Y-Shift motor teach ...";
                        }
                    }
                }
                else if(TeachPos==4)  //Hotplate 1
                {
                    if(b3PointTeach==false || iTeach3Point==0)
                    {

                        XMovPos[0][3][0]=Tech.iInArmPlate1X;  //每一個圓X 左邊 位置
                        YMovPos[0][3][0]=Tech.iInArmPlate1Y; //每一個圓Y 左邊 位置
                        ZPos=10;
                        sString= "Inarm Hotplate 1  motor teach Start"; //kevin 20150728
                    }
                    else
                    {
                        sStr3Point="Inarm Hotplate 1";
                        if(iTeach3Point==1)
                        {
                            if(HOT_PLATE_POSITION==0)  //pin在左邊
                            {
                               iJigXDis=3000; //JerryYang 20161026 三點校正治具,X方向兩點的距離
                            }
                            else
                            {
                               iJigXDis=11000;
                            }
                            XMovPos[0][3][0]=Tech.iInArmPlate1X+iJigXDis;  //每一個圓X 左邊 位置
                            YMovPos[0][3][0]=Tech.iInArmPlate1Y; //每一個圓Y 左邊 位置
                            ZPos=10;
                            sString= "Inarm Hotplate 1  motor teach Start"; //kevin 20150728
                        }
                        else if(iTeach3Point==2)
                        {
                            iJigYDis=27000;
                            XMovPos[0][3][0]=Tech.iInArmPlate1X;  //每一個圓X 左邊 位置
                            if(HOT_PLATE_POSITION==0)  //pin在左邊
                            {
                                YMovPos[0][3][0]=Tech.iInArmPlate1Y-iJigYDis;  //每一個圓X 左邊 位置
                            }
                            else
                            {
                                YMovPos[0][3][0]=Tech.iInArmPlate1Y+iJigYDis;  //每一個圓X 左邊 位置
                            }
                            ZPos=10;
                            sString= "Inarm Hotplate 1  motor teach Start"; //kevin 20150728
                        }
                    }
                }
                else if(TeachPos==5 )  //Hotplate 2
                {
                    if(b3PointTeach==false || iTeach3Point==0)
                    {
                        XMovPos[0][4][0]=Tech.iInArmPlate2X;  //每一個圓X 左邊 位置
                        YMovPos[0][4][0]=Tech.iInArmPlate2Y; //每一個圓Y 左邊 位置
                        ZPos=10;
                        sString= "Inarm Hotplate 2  motor teach Start"; //kevin 20150728
                    }
                    else
                    {
                        sStr3Point="Inarm Hotplate 2";
                        if(iTeach3Point==1)    // X-Shift
                        {
                            iJigXDis=3000; //JerryYang 20161026 三點校正治具,X方向兩點的距離
                            if(HOT_PLATE_POSITION==0)  //pin在左邊
                            {
                               iJigXDis=3000; //JerryYang 20161026 三點校正治具,X方向兩點的距離
                            }
                            else
                            {
                               iJigXDis=11000;
                            }
                            XMovPos[0][4][0]=Tech.iInArmPlate2X+iJigXDis;  //每一個圓X 左邊 位置
                            YMovPos[0][4][0]=Tech.iInArmPlate2Y; //每一個圓Y 左邊 位置
                            ZPos=10;
                            sString= "Inarm Hotplate 2  X-Shift motor teach ...";
                        }
                        else if(iTeach3Point==2)    // Y-Shift
                        {

                            iJigYDis=27000; //JerryYang 20161026 三點校正治具,Y方向兩點的距離
                            XMovPos[0][4][0]=Tech.iInArmPlate2X;  //每一個圓X 左邊 位置
                            if(HOT_PLATE_POSITION==0)  //pin在左邊
                            {
                                YMovPos[0][4][0]=Tech.iInArmPlate2Y-iJigYDis;  //每一個圓X 左邊 位置
                            }
                            else
                            {
                                YMovPos[0][4][0]=Tech.iInArmPlate2Y+iJigYDis;  //每一個圓X 左邊 位置
                            }
                            ZPos=10;
                            sString= "Inarm Hotplate 2 Y-Shift motor teach ...";
                        }
                    }

                }
                else if(TeachPos==6 ) //Rotate
                {
                    if(USE_ROTATE_KIT==0) //沒有安裝Rotate就跳過
                    {
                        iTask=121;
                        break;
                    }
                    XMovPos[0][5][0]=Tech.M_In_iRotateX;  //每一個圓X 左邊 位置
                    YMovPos[0][5][0]=Tech.M_In_iRotateY; //每一個圓Y 左邊 位置
                    ZPos=10;
                    sString= "Inarm Rotate  motor teach Start"; //kevin 20150728
                }
                iXPos=XMovPos[0][TeachPos-1][0];
                XPOSSource=iXPos;
                iYPos=YMovPos[0][TeachPos-1][0];
                YPOSSource=iYPos;
            }
            else if(InArm==1)//OUTarm
            {
                if(TeachPos==0 || bEnableUseAreaZCalbration) //kevin 20210205
                {
                    AutoTeachZ(true,iUseArm,false,TeachPos);//INARM Z 水平校正
                    Memo1->Lines->Add("Outarm Z motor teach Start"); //kevin 20150728
                    iTask=10;
                    break;
                }
                else if(bEnableUseAreaZCalbration)  //Frank 20171213 (Steven) modify
                {
                    if(TeachPos==2)         //shuttl 2
                        sString="Shuttle 2";
                    else if(TeachPos==3)    //Auto 1
                        sString="Auto 1 ";
                    else if(TeachPos==6)    //Fix 1
                        sString="Fix 1";

                    asStr.sprintf("Outarm Z Calibration in %s Area Start",sString);
                    Memo1->Lines->Add(asStr); //kevin 20150728
                    iTask=1000;
                    break;
                }
                else if(TeachPos==1) //shuttle1
                {
                    if(b3PointTeach==false || iTeach3Point==0)
                    {
                        XMovPos[1][0][0]=Tech.iOutArmShuttle1X;  //每一個圓X 左邊 位置
                        YMovPos[1][0][0]=Tech.iOutArmShuttle1Y;  //每一個圓Y 左邊 位置
                        ZPos=-610;
                        sString="Outarm shuttle 2  motor teach Start"; //kevin 20150728
                    }
                    else
                    {
                        sStr3Point="Outarm shuttle 1";
                        if(iTeach3Point==1)    // X-Shift
                        {
                            iJigXDis=6000; //JerryYang 20161026 三點校正治具,X方向兩點的距離
                            XMovPos[1][0][0]=Tech.iOutArmShuttle1X+iJigXDis;  //每一個圓X 左邊 位置
                            YMovPos[1][0][0]=Tech.iOutArmShuttle1Y; //每一個圓Y 左邊 位置
                            ZPos=-610;
                            sString= "Outarm shuttle 1 X-Shift motor teach Start"; //kevin 20150728
                        }
                        else if(iTeach3Point==2)    // Y-Shift
                        {
                            iJigYDis=6000; //JerryYang 20161026 三點校正治具,Y方向兩點的距離
                            XMovPos[1][0][0]=Tech.iOutArmShuttle1X;  //每一個圓X 左邊 位置
                            YMovPos[1][0][0]=Tech.iOutArmShuttle1Y-iJigYDis; //JerryYang 20161012 三點校正Teaching點的下面校正點  //每一個圓Y 左邊 位置
                            ZPos=-610;
                            sString= "Outarm shuttle 1 Y-Shift motor teach Start ...";
                        }
                    }

                }
                else if(TeachPos==2 )  //shuttle2
                {
                    if(b3PointTeach==false || iTeach3Point==0)
                    {
                        XMovPos[1][1][0]=Tech.iOutArmShuttle2X;  //每一個圓X 左邊 位置
                        YMovPos[1][1][0]=Tech.iOutArmShuttle2Y; //每一個圓Y 左邊 位置
                        ZPos=-610;
                        sString= "Outarm shuttle 2  motor teach Start"; //kevin 20150728
                    }
                    else
                    {
                        sStr3Point="Outarm shuttle 2";
                        if(iTeach3Point==1)    // X-Shift
                        {
                            iJigXDis=6000; //JerryYang 20161026 三點校正治具,X方向兩點的距離
                            XMovPos[1][1][0]=Tech.iOutArmShuttle2X+iJigXDis;  //每一個圓X 左邊 位置
                            YMovPos[1][1][0]=Tech.iOutArmShuttle2Y; //每一個圓Y 左邊 位置
                            ZPos=-610;
                            sString= "Outarm shuttle 2 X-Shift motor teach Start ..."; //kevin 20150728
                        }
                        else if(iTeach3Point==2)    // Y-Shift
                        {
                            iJigYDis=6000; //JerryYang 20161026 三點校正治具,Y方向兩點的距離
                            XMovPos[1][1][0]=Tech.iOutArmShuttle2X;  //每一個圓X 左邊 位置
                            YMovPos[1][1][0]=Tech.iOutArmShuttle2Y-iJigYDis; //JerryYang 20161012 三點校正Teaching點的下面校正點  //每一個圓Y 左邊 位置
                            ZPos=-610;
                            sString= "Outarm shuttle 2 Y-Shift motor teach Start ...";
                        }
                    }
                }
                else if(TeachPos==3 )   //AUTO 1
                {
                    if(b3PointTeach==false || iTeach3Point==0)
                    {
                        XMovPos[1][2][0]=Tech.iOutArmAuto1X;  //每一個圓X 左邊 位置
                        YMovPos[1][2][0]=Tech.iOutArmAuto1Y ; //每一個圓Y 左邊 位置
                        ZPos=-610;
                        sString= "Outarm Auto 1 motor teach Start"; //kevin 20150728
                    }
                    else
                    {
                        sStr3Point="Outarm Auto 1";
                        if(iTeach3Point==1)    // X-Shift
                        {
                            iJigXDis=11000; //JerryYang 20161026 三點校正治具,X方向兩點的距離
                            XMovPos[1][2][0]=Tech.iOutArmAuto1X+iJigXDis;  //每一個圓X 左邊 位置
                            YMovPos[1][2][0]=Tech.iOutArmAuto1Y; //每一個圓Y 左邊 位置
                            ZPos=-610;
                            sString= "Outarm Auto 1 X-Shift motor teach Start ..."; //kevin 20150728
                        }
                        else if(iTeach3Point==2)    // Y-Shift
                        {
                            iJigYDis=27000; //JerryYang 20161026 三點校正治具,Y方向兩點的距離
                            XMovPos[1][2][0]=Tech.iOutArmAuto1X;  //每一個圓X 左邊 位置
                            YMovPos[1][2][0]=Tech.iOutArmAuto1Y-iJigYDis; //JerryYang 20161012 三點校正Teaching點的下面校正點  //每一個圓Y 左邊 位置
                            ZPos=-610;
                            sString= "Outarm Auto 1 Y-Shift motor teach Start ...";
                        }
                    }
                }
                else if(TeachPos==4 ) //AUTO 2
                {
                    if(b3PointTeach==false || iTeach3Point==0)
                    {
                        XMovPos[1][3][0]=Tech.iOutArmAuto2X;  //每一個圓X 左邊 位置
                        YMovPos[1][3][0]=Tech.iOutArmAuto2Y; //每一個圓Y 左邊 位置
                        ZPos=-610;
                        sString= "Outarm Auto 2 motor teach Start"; //kevin 20150728
                    }
                    else
                    {
                        sStr3Point="Outarm Auto 2";
                        if(iTeach3Point==1)    // X-Shift
                        {
                            iJigXDis=11000; //JerryYang 20161026 三點校正治具,X方向兩點的距離
                            XMovPos[1][3][0]=Tech.iOutArmAuto2X+iJigXDis;  //每一個圓X 左邊 位置
                            YMovPos[1][3][0]=Tech.iOutArmAuto2Y; //每一個圓Y 左邊 位置
                            ZPos=-610;
                            sString= "Outarm Auto 2 X-Shift motor teach Start ..."; //kevin 20150728
                        }
                        else if(iTeach3Point==2)    // Y-Shift
                        {
                            iJigYDis=27000; //JerryYang 20161026 三點校正治具,Y方向兩點的距離
                            XMovPos[1][3][0]=Tech.iOutArmAuto2X;  //每一個圓X 左邊 位置
                            YMovPos[1][3][0]=Tech.iOutArmAuto2Y-iJigYDis; //JerryYang 20161012 三點校正Teaching點的下面校正點  //每一個圓Y 左邊 位置
                            ZPos=-610;
                            sString= "Outarm Auto 2 Y-Shift motor teach Start ...";
                        }
                    }
                }
                else if(TeachPos==5 ) //AUTO 3
                {
                    if(b3PointTeach==false || iTeach3Point==0)
                    {
                        XMovPos[1][4][0]=Tech.iOutArmAuto3X;  //每一個圓X 左邊 位置
                        YMovPos[1][4][0]=Tech.iOutArmAuto3Y; //每一個圓Y 左邊 位置
                        ZPos=-610;
                        sString= "Outarm Auto 3 motor teach Start"; //kevin 20150728
                    }
                    else
                    {
                        sStr3Point="Outarm Auto 3";
                        if(iTeach3Point==1)    // X-Shift
                        {
                            iJigXDis=11000; //JerryYang 20161026 三點校正治具,X方向兩點的距離
                            XMovPos[1][4][0]=Tech.iOutArmAuto3X+iJigXDis;  //每一個圓X 左邊 位置
                            YMovPos[1][4][0]=Tech.iOutArmAuto3Y; //每一個圓Y 左邊 位置
                            ZPos=-610;
                            sString= "Outarm Auto 3 X-Shift motor teach Start ..."; //kevin 20150728
                        }
                        else if(iTeach3Point==2)    // Y-Shift
                        {
                            iJigYDis=27000; //JerryYang 20161026 三點校正治具,Y方向兩點的距離
                            XMovPos[1][4][0]=Tech.iOutArmAuto3X;  //每一個圓X 左邊 位置
                            YMovPos[1][4][0]=Tech.iOutArmAuto3Y-iJigYDis; //JerryYang 20161012 三點校正Teaching點的下面校正點  //每一個圓Y 左邊 位置
                            ZPos=-610;
                            sString= "Outarm Auto 3 Y-Shift motor teach Start ...";
                        }
                    }
                }
                else if(TeachPos==6 )//FIX 1
                {
                    if(b3PointTeach==false || iTeach3Point==0)
                    {
                        XMovPos[1][5][0]=Tech.iOutArmFix1X;  //每一個圓X 左邊 位置
                        YMovPos[1][5][0]=Tech.iOutArmFix1Y; //每一個圓Y 左邊 位置
                        ZPos=10;
                        sString= "Outarm FIX 1 motor teach Start"; //kevin 20150728
                    }
                    else
                    {
                        sStr3Point="Outarm FIX 1";
                        if(iTeach3Point==1)    // X-Shift
                        {
                            iJigXDis=11000; //JerryYang 20161026 三點校正治具,X方向兩點的距離
                            XMovPos[1][5][0]=Tech.iOutArmFix1X+iJigXDis;  //每一個圓X 左邊 位置
                            YMovPos[1][5][0]=Tech.iOutArmFix1Y; //每一個圓Y 左邊 位置
                            ZPos=10;
                            sString= "Outarm FIX 1 X-Shift motor teach Start ..."; //kevin 20150728
                        }
                        else if(iTeach3Point==2)    // Y-Shift
                        {
                            iJigYDis=27000; //JerryYang 20161026 三點校正治具,Y方向兩點的距離
                            XMovPos[1][5][0]=Tech.iOutArmFix1X;  //每一個圓X 左邊 位置
                            YMovPos[1][5][0]=Tech.iOutArmFix1Y-iJigYDis; //JerryYang 20161012 三點校正Teaching點的下面校正點  //每一個圓Y 左邊 位置
                            ZPos=10;
                            sString= "Outarm FIX 1 Y-Shift motor teach Start ...";
                        }
                    }
                }
                else if(TeachPos==7 )//FIX 2
                {
                    if(b3PointTeach==false || iTeach3Point==0)
                    {
                        XMovPos[1][6][0]=Tech.iOutArmFix2X;  //每一個圓X 左邊 位置
                        YMovPos[1][6][0]=Tech.iOutArmFix2Y; //每一個圓Y 左邊 位置
                        ZPos=10;
                        sString= "Outarm FIX 2 motor teach Start"; //kevin 20150728
                    }
                    else
                    {
                        sStr3Point="Outarm FIX 2";
                        if(iTeach3Point==1)    // X-Shift
                        {
                            iJigXDis=11000; //JerryYang 20161026 三點校正治具,X方向兩點的距離
                            XMovPos[1][6][0]=Tech.iOutArmFix2X+iJigXDis;  //每一個圓X 左邊 位置
                            YMovPos[1][6][0]=Tech.iOutArmFix2Y; //每一個圓Y 左邊 位置
                            ZPos=10;
                            sString= "Outarm FIX 2 X-Shift motor teach Start ..."; //kevin 20150728
                        }
                        else if(iTeach3Point==2)    // Y-Shift
                        {
                            iJigYDis=27000; //JerryYang 20161026 三點校正治具,Y方向兩點的距離
                            XMovPos[1][6][0]=Tech.iOutArmFix2X;  //每一個圓X 左邊 位置
                            YMovPos[1][6][0]=Tech.iOutArmFix2Y-iJigYDis; //JerryYang 20161012 三點校正Teaching點的下面校正點  //每一個圓Y 左邊 位置
                            ZPos=10;
                            sString= "Outarm FIX 2 Y-Shift motor teach Start ...";
                        }
                    }
                }
                else if(TeachPos==8 )//FIX 3
                {
                    if(b3PointTeach==false || iTeach3Point==0)
                    {
                        XMovPos[1][7][0]=Tech.iOutArmFix3X;  //每一個圓X 左邊 位置
                        YMovPos[1][7][0]=Tech.iOutArmFix3Y; //每一個圓Y 左邊 位置
                        ZPos=10;
                        sString= "Outarm FIX 3 motor teach Start"; //kevin 20150728
                    }
                    else
                    {
                        sStr3Point="Outarm FIX 3";
                        if(iTeach3Point==1)    // X-Shift
                        {
                            iJigXDis=11000; //JerryYang 20161026 三點校正治具,X方向兩點的距離
                            XMovPos[1][7][0]=Tech.iOutArmFix3X+iJigXDis;  //每一個圓X 左邊 位置
                            YMovPos[1][7][0]=Tech.iOutArmFix3Y; //每一個圓Y 左邊 位置
                            ZPos=10;
                            sString= "Outarm FIX 3 X-Shift motor teach Start ..."; //kevin 20150728
                        }
                        else if(iTeach3Point==2)    // Y-Shift
                        {
                            iJigYDis=27000; //JerryYang 20161026 三點校正治具,Y方向兩點的距離
                            XMovPos[1][7][0]=Tech.iOutArmFix3X;  //每一個圓X 左邊 位置
                            YMovPos[1][7][0]=Tech.iOutArmFix3Y-iJigYDis; //JerryYang 20161012 三點校正Teaching點的下面校正點  //每一個圓Y 左邊 位置
                            ZPos=10;
                            sString= "Outarm FIX 3 Y-Shift motor teach Start ...";
                        }
                    }
                }
                else if(TeachPos==9 )//Rotate
                {
                    if(USE_ROTATE_KIT==0)
                    {
                        iTask=121;
                        break;
                    }
                    XMovPos[1][8][0]=Tech.M_Out_iRotateX;  //每一個圓X 左邊 位置
                    YMovPos[1][8][0]=Tech.M_Out_iRotateY; //每一個圓Y 左邊 位置
                    ZPos=10;
                    sString= "Outarm Rotate  motor teach Start"; //kevin 20150728
                }
                iXPos=XMovPos[1][TeachPos-1][0];
                XPOSSource=iXPos;
                iYPos=YMovPos[1][TeachPos-1][0];
                YPOSSource=iYPos;
            }
            MOT[MInArmX].SetSpeed(20);
            MOT[MInArmY].SetSpeed(20);
            MOT[MInArmPitch].SetSpeed(10);
            MOT[MInShuttle1].SetSpeed(50);
            MOT[MInShuttle2].SetSpeed(50);
            for(i=0; i<8; i++)
            {
                MOT[MInArmZA+i].SetSpeed(10);
                MOT[MOutArmZA+i].SetSpeed(10);
            }
            MOT[MOutArmX].SetSpeed(10);
            MOT[MOutArmY].SetSpeed(10);
            MOT[MOutArmPitch].SetSpeed(10);
            MOT[MOutShuttle1].SetSpeed(50);   //kevin 20210312 add
            MOT[MOutShuttle1].SetSpeed(50);   //kevin 20210312 add
            Memo1->Lines->Add(sString); //kevin 20150728
            iTask=100;
            break;
        case 10:
            if(AutoTeachZ(false, iUseArm,false,TeachPos))  //Frank 20171213 (Steven) modify
            {
                if(bEnableUseAreaZCalbration) //kevin 20210205
                {
                    bZTeachOffset=true;             //kevin 20210317 Z Teach Offset  OK  開始做各別 Z TEACH
                    if(InArm==0)// inarm
                    {
                        if(TeachPos==1)         //shuttl 1      kevin 20210317 add Autoteach Z
                            sString="Shuttle 1";
                        else if(TeachPos==2)         //shuttl 2
                            sString="Shuttle 2";
                        else if(TeachPos==3)    //LOAD
                            sString="Loader ";
                        else if(TeachPos==4)         //shuttl 1      kevin 20210317 add Autoteach Z
                            sString="HotPlate 1";
                        else if(TeachPos==5)    //Hotplate 2
                            sString="HotPlate 2";
                        asStr.sprintf("Inarm Z Calibration in %s Area Start",sString);
                    }
                    else
                    {
                        if(TeachPos==1)         //shuttl 1
                            sString="Shuttle 1";
                        else if(TeachPos==2)    //shuttl 2
                            sString="Shuttle 2";

                        asStr.sprintf("Inarm Z Calibration in %s Area Start",sString);
                    }
                    Memo1->Lines->Add(asStr); //kevin 20150728
                    iTask=1000;
                }
                else
                {
                    iTask=131;
                }
            }
            break;
        case 100:    //移到檢測位置
            if(InArm==0)// inarm
            {
                if(MoveInArmXYToTeachPos(InArm,iXPos,iYPos))
                    iTask++;
            }
            else
            {
                if(MoveInArmXYToTeachPos(OutArm,iXPos,iYPos))
                    iTask++;
            }
            break;
        case 101:  //移到檢測高度位置
            if(InArm==0)// inarm
            {
                if(MOT[MInArmZE].MotorMove(ZPos))
                    iTask++;
                MOT[MInArmX].SetSpeed(5);
                MOT[MInArmY].SetSpeed(5);
            }
            else
            {
                 if(MOT[MOutArmZE].MotorMove(ZPos))
                    iTask++;
                MOT[MInArmX].SetSpeed(5);
                MOT[MInArmY].SetSpeed(5);
            }
            break;
        case 102:  //X位置右移   找黑地方  sensor set d-On
            if(InArm==0)// inarm
                MOT[MInArmX].MotorMove(iXPos);
            else
                MOT[MOutArmX].MotorMove(iXPos);

            if(Sen[SnAutoTeach].IsOn())
            {
                if(InArm==0)
                    MOT[MInArmX].PCIL132_StopMotor();
                else
                    MOT[MOutArmX].PCIL132_StopMotor();
                iTask++;
                break;
            }
            else
            {
                if(InArm==0)// inarm
                    iXPos+=iOffset;
                else
                    iXPos-=iOffset;
            }

            if(abs(XPOSSource-iXPos)>3000)// inarm   超出搜尋範圍
            {
                Memo1->Lines->Add("No Install SnAutoTeach sensor for database.");
                iTask=131;
            }
            break;
        case 103:  //X位置左移   找白地方
            if(InArm==0)// inarm
                MOT[MInArmX].MotorMove(iXPos);
            else
                MOT[MOutArmX].MotorMove(iXPos);

            if(Sen[SnAutoTeach].IsOff()) //找到左邊位置
            {
                if(InArm==0)
                {
                    MOT[MInArmX].PCIL132_StopMotor();
                    XMovPos[0][TeachPos-1][1]= MOT[MInArmX].ReadEncoderPos();  //記錄右邊位置
                }
                else
                {
                    MOT[MOutArmX].PCIL132_StopMotor();
                    XMovPos[1][TeachPos-1][1]= MOT[MOutArmX].ReadEncoderPos();  //記錄左邊位置
                }
                iTask++;
                break;
            }
            else
            {
                if(InArm==0)// inarm
                    iXPos-=iOffset;
                else
                    iXPos+=iOffset;
            }

            if(abs(XPOSSource-iXPos)>1500)// inarm   超出搜尋範圍
            {
                MOT[MInArmX].PCIL132_StopMotor();
                Memo1->Lines->Add("Left Find Over 1500.");
                iTask=131;
            }
            break;
        case 104:  //X位置左移 找黑地方
            if(InArm==0)// inarm
                MOT[MInArmX].MotorMove(iXPos);
            else
                MOT[MOutArmX].MotorMove(iXPos);

            if(Sen[SnAutoTeach].IsOn())//左邊位置
            {
                if(InArm==0)
                    MOT[MInArmX].PCIL132_StopMotor();
                else
                    MOT[MOutArmX].PCIL132_StopMotor();

                iTask++;
                break;
            }
            else
            {
                if(InArm==0)// inarm
                    iXPos-=iOffset;
                else
                    iXPos+=iOffset;
            }

            if(abs(XPOSSource-iXPos)>1500)// inarm   超出搜尋範圍
            {
                MOT[MInArmX].PCIL132_StopMotor();
                Memo1->Lines->Add("Left Find Over 1500.");
                iTask=131;
                break;
            }
            break;
        case 105:  //X位置右移 找白地方
            if(InArm==0)// inarm
                MOT[MInArmX].MotorMove(iXPos);
            else
                MOT[MOutArmX].MotorMove(iXPos);

            if(Sen[SnAutoTeach].IsOff())
            {                          //左邊位置
                if(InArm==0)
                {
                    MOT[MInArmX].PCIL132_StopMotor();
                    XMovPos[0][TeachPos-1][2]= MOT[MInArmX].ReadEncoderPos();  //記錄左邊位置
                    XMovPos[0][TeachPos-1][3] =(XMovPos[0][TeachPos-1][1]-XMovPos[0][TeachPos-1][2])/2;  //記錄中間位置
                    iXPos= XMovPos[0][TeachPos-1][2]+XMovPos[0][TeachPos-1][3];//X中間位置
                    if(iTeach3Point==1)
                    {
                        X_ShiftPOSX=iXPos;
                        StringGrid1->Cells[6][TeachPos]=iXPos;  //3點校正, X-Shift點位
                    }
                    else if(iTeach3Point==2)
                    {
                        Y_ShiftPOSX=iXPos;
                    }
                    else if(iTeach3Point==0)
                    {
                        X_NewTechPOS=iXPos;
                        StringGrid1->Cells[1][TeachPos]=iXPos;
                    }
                }
                else
                {
                    MOT[MOutArmX].PCIL132_StopMotor();
                    XMovPos[1][TeachPos-1][2]= MOT[MOutArmX].ReadEncoderPos();  //記錄左邊位置
                    XMovPos[1][TeachPos-1][3] =(XMovPos[1][TeachPos-1][1]-XMovPos[1][TeachPos-1][2])/2;  //記錄中間位置
                    iXPos= XMovPos[1][TeachPos-1][2]+XMovPos[1][TeachPos-1][3];//X中間
                    if(iTeach3Point==1)
                    {
                        X_ShiftPOSX=iXPos;
                        StringGrid1->Cells[6][TeachPos]=iXPos;  //3點校正, X-Shift點位
                    }
                    else if(iTeach3Point==2)
                    {
                        Y_ShiftPOSX=iXPos;
                    }
                    else if(iTeach3Point==0)
                    {
                        X_NewTechPOS=iXPos;
                        StringGrid1->Cells[1][TeachPos]=iXPos;
                    }
                }
                iTask++;
            }
            else
            {
                if(InArm==0)// inarm
                    iXPos+=iOffset;
                else
                    iXPos-=iOffset;
            }

            if(InArm==0 && abs(XPOSSource-iXPos)>1500)// inarm   超出搜尋範圍
            {
                MOT[MInArmX].PCIL132_StopMotor();
                Memo1->Lines->Add("Right Find Over 1500.");
                iTask=131;
                break;
            }
            else if(InArm==1 && abs(XPOSSource-iXPos)>1500)// outarm   超出搜尋範圍
            {
                MOT[MInArmX].PCIL132_StopMotor();
                Memo1->Lines->Add("Right Find Over 1500.");
                iTask=131;
                break;
            }
            break;
        case 106:  //X 圓中心
            if(InArm==0)// inarm
                biflag=MOT[MInArmX].MotorMove(iXPos);
            else
                biflag=MOT[MOutArmX].MotorMove(iXPos);

            if(biflag)
                iTask++;
            break;
        case 107:
            if(InArm==0)// inarm  Y往上找 白色
                MOT[MInArmY].MotorMove(iYPos);
            else
                MOT[MOutArmY].MotorMove(iYPos);

            if(Sen[SnAutoTeach].IsOn())
            {
                if(InArm==0)
                    MOT[MInArmY].PCIL132_StopMotor();
                else
                    MOT[MOutArmY].PCIL132_StopMotor();

                iTask++;
            }
            else
                iYPos+=iOffset;

            if(abs(YPOSSource-iYPos)>1500)// inarm   超出搜尋範圍
            {
                MOT[MInArmY].PCIL132_StopMotor();
                Memo1->Lines->Add("UP Find Over 1500.");
                iTask=131;
            }
            break;
        case 108:  //記錄Y上面位置
            if(InArm==0)// inarm  Y往下找 黑色
                MOT[MInArmY].MotorMove(iYPos);
            else
                MOT[MOutArmY].MotorMove(iYPos);

            if(Sen[SnAutoTeach].IsOff()) //找到上面位置
            {
                if(InArm==0)
                {
                    MOT[MInArmY].PCIL132_StopMotor();
                    YMovPos[0][TeachPos-1][1]= MOT[MInArmY].ReadEncoderPos();  //記錄Y上面位置
                }
                else
                {
                    MOT[MOutArmY].PCIL132_StopMotor();
                    YMovPos[1][TeachPos-1][1]= MOT[MOutArmY].ReadEncoderPos();  //記錄Y上面位置
                }
                iTask++;
            }
            else
               iYPos-=iOffset;

            if(abs(YPOSSource-iYPos)>1500)// inarm   超出搜尋範圍
            {
                MOT[MInArmY].PCIL132_StopMotor();
                Memo1->Lines->Add("Down Find Over 1500.");
                iTask=131;
            }
            break;
        case 109: //找Y 軸 下方
            if(InArm==0)// inarm  Y往下找 白色
                MOT[MInArmY].MotorMove(iYPos);
            else
                MOT[MOutArmY].MotorMove(iYPos);

            if(Sen[SnAutoTeach].IsOn())
            {
                if(InArm==0)
                {
                    MOT[MInArmY].PCIL132_StopMotor();
                }
                else
                {
                    MOT[MOutArmY].PCIL132_StopMotor();
                }
                iTask++;
            }
            else
                iYPos-=iOffset;

            if(abs(YPOSSource-iYPos)>1500)// inarm   超出搜尋範圍
            {
                MOT[MInArmY].PCIL132_StopMotor();
                Memo1->Lines->Add("Down Find Over 1500.");
                iTask=131;
            }
            break;
        case 110:
            if(InArm==0)// inarm  Y往上找 黑色
                MOT[MInArmY].MotorMove(iYPos);
            else
                MOT[MOutArmY].MotorMove(iYPos);

            if(Sen[SnAutoTeach].IsOff())
            {
                if(InArm==0)
                {
                    MOT[MInArmY].PCIL132_StopMotor();
                    YMovPos[0][TeachPos-1][2]= MOT[MInArmY].ReadEncoderPos();  //記錄Y下面位置
                    YMovPos[0][TeachPos-1][3]=  (YMovPos[0][TeachPos-1][1]-YMovPos[0][TeachPos-1][2])/2;  //記錄Y上面位置
                    iYPos=YMovPos[0][TeachPos-1][2]+YMovPos[0][TeachPos-1][3];//Y中間
                    if(iTeach3Point==2)
                    {
                        Y_ShiftPOSY=iYPos;
                        StringGrid1->Cells[7][TeachPos]=iYPos;  //3點校正, X-Shift點位
                    }
                    else if(iTeach3Point==1)
                    {
                        X_ShiftPOSY=iYPos;
                    }
                    else if(iTeach3Point==0)
                    {
                        Y_NewTechPOS=iYPos;
                        StringGrid1->Cells[2][TeachPos]=iYPos;
                    }
                    switch(TeachPos)
                    {
                        case  0:
                            sMode+="Z Calibration";
                            break;
                        case  1:
                            sMode+="Shuttle 1";
                            break;
                        case  2:
                            sMode+="Shuttle 2";
                            break;
                        case  3:
                            sMode+="Load";
                            break;
                        case  4:
                            sMode+="Hotplate 1";
                            break;
                        case  5:
                            sMode+="Hotplate 2";
                            break;
                        case  6:
                            sMode+="Rotate";
                            break;
                    }
                }
                else
                {
                    MOT[MOutArmY].PCIL132_StopMotor();
                    YMovPos[1][TeachPos-1][2]= MOT[MOutArmY].ReadEncoderPos();  //記錄Y下面位置
                    YMovPos[1][TeachPos-1][3]=  (YMovPos[1][TeachPos-1][1]-YMovPos[1][TeachPos-1][2])/2;  //記錄Y上面位置
                    iYPos=YMovPos[1][TeachPos-1][2]+YMovPos[1][TeachPos-1][3];//Y中間
                    if(iTeach3Point==2)
                    {
                        Y_ShiftPOSY=iYPos;
                        StringGrid1->Cells[7][TeachPos]=iYPos;  //3點校正, X-Shift點位
                    }
                    else if(iTeach3Point==1)
                    {
                        X_ShiftPOSY=iYPos;
                    }
                    else if(iTeach3Point==0)
                    {
                        Y_NewTechPOS=iYPos;
                        StringGrid1->Cells[2][TeachPos]=iYPos;
                    }

                    switch(TeachPos)
                    {
                        case  0:
                            sMode+="Z Calibration";
                            break;
                        case  1:
                            sMode+="Shuttle 1";
                            break;
                        case  2:
                            sMode+="Shuttle 2";
                            break;
                        case  3:
                            sMode+="Auto 1";
                            break;
                        case  4:
                            sMode+="Auto 2";
                            break;
                        case  5:
                            sMode+="Auto 3";
                            break;
                        case  6:
                            sMode+="Fix 1";
                            break;
                        case  7:
                            sMode+="Fix 2";
                            break;
                        case  8:
                            sMode+="Fix 3";
                            break;
                        case  9:
                            sMode+="Rotate";
                            break;
                    }
                }
                Memo1->Lines->Add(sMode);
                iTask=120;
                break;
            }
            else
                iYPos+=iOffset;

            if(abs(YPOSSource-iYPos)>1500)// inarm   超出搜尋範圍
            {
                MOT[MInArmY].PCIL132_StopMotor();
                Memo1->Lines->Add("UP Find Over 1500.");
                iTask=131;
            }
            break;
        case 120:  //X 圓中心
            if(InArm==0)// inarm
                biflag=MOT[MInArmY].MotorMove(iYPos);
            else
                biflag=MOT[MOutArmY].MotorMove(iYPos);

            if(biflag)
                 iTask++;
            break;
        case 121: //升到高點準備移到下一個位置
            if(InArm==0)// inarm
            {
                if(MOT[MInArmZE].MotorMove(0))
                {
                    if(SingleTeach==false)
                    {
                        if(b3PointTeach==true)
                        {
                            if(iTeach3Point<2)  //繼續做3點校正
                            {
                                iTeach3Point++;
                                iTask=5;
                            }
                            else
                            {
                                iTask=130;
                            }
                        }
                        else
                        {
                            AutoTeachiTeachPos++;    //全部要校正
                            TeachPos=AutoTeachiTeachPos;//移到下一個校正位置
                            if(AutoTeachiTeachPos>6)
                            {
                                iTask++;
                                break;         //inarm  全部校正完
                            }
                            sgThreePointTeach->Cells[1][0]="";   //清除紀錄
                            for(int i=0; i<5; i++)
                            {
                                for(int j=0; j<7; j++)
                                {
                                    if(i>=2 && j>=1)
                                        sgThreePointTeach->Cells[i][j]="";
                                }
                            }
                            iTask=5;
                        }
                        break;
                    }
                    else
                    {
                        if(b3PointTeach==true)
                        {

                            if(iTeach3Point<2)  //繼續做3點校正
                            {
                                iTeach3Point++;
                                iTask=5;
                            }
                            else
                            {
                                iTask=130;
                            }
                        }
                        else
                        {
                            iTask++;
                        }
                    }
                }
            }
            else
            {
               if(MOT[MOutArmZE].MotorMove(0))
                {
                    if(SingleTeach==false)
                    {
                        if(b3PointTeach==true)
                        {
                            if(iTeach3Point<2)  //繼續做3點校正
                            {
                                iTeach3Point++;
                                iTask=5;
                            }
                            else
                            {
                                iTask=130;
                            }
                        }
                        else
                        {
                            AutoTeachiTeachPos++;
                            TeachPos=AutoTeachiTeachPos;//移到下一個校正位置
                            if(InArm==1  && AutoTeachiTeachPos >9)
                            {
                                iTask++;
                                break;         //Outarm  全部校正完
                            }
                            iTask=5;
                        }
                        break;
                    }
                    else
                    {
                        if(b3PointTeach==true)
                        {

                            if(iTeach3Point<2)  //繼續做3點校正
                            {
                                iTeach3Point++;
                                iTask=5;
                            }
                            else
                            {
                                iTask=130;
                            }
                        }
                        else
                        {
                            iTask++;
                        }
                    }
                }
            }
            break;
        case 130:
            for(int i=0; i<5; i++)
            {
                for(int j=0; j<7; j++)
                {
                    if(i>=2 && j>=1)
                        sgThreePointTeach->Cells[i][j]="";
                }
            }
            sgThreePointTeach->Cells[1][0]=sStr3Point;   //清除紀錄
            sgThreePointTeach->Cells[2][1]=iJigXDis;
            sgThreePointTeach->Cells[2][4]=iJigYDis;
            X_Dis=abs(X_ShiftPOSX-X_NewTechPOS);
            Y_Dis=abs(Y_ShiftPOSY-Y_NewTechPOS);
            dAngleX=Angle(X_NewTechPOS, Y_NewTechPOS, X_ShiftPOSX, X_ShiftPOSY,0);
            dAngleY=Angle(X_NewTechPOS, Y_NewTechPOS, Y_ShiftPOSX, Y_ShiftPOSY,1);
            sTemp.printf("%2.5f", dAngleX);
            sTemp1.printf("%2.5f", dAngleY);
            sgThreePointTeach->Cells[4][2]=sTemp;
            sgThreePointTeach->Cells[4][5]=sTemp1;
            if(X_Dis==0 || Y_Dis==0)
            {
                iTask=131;
                ShowMyMessage("三點校正點位錯誤");
            }
            dXScale=double(iJigXDis)/X_Dis;
            dYScale=ChangeToFloatNonPcnt((double)(double(iJigYDis)), (double)(Y_Dis));

            sgThreePointTeach->Cells[4][3]=FloatToStr(dAngleX);
            sgThreePointTeach->Cells[4][6]=FloatToStr(dAngleY);

            if(abs(dAngleX)>=1.0 || abs(dAngleY) >=1.0)
            {
                iTask=131;
                ShowMyMessage("角度過大, 請確認jig是否有擺正");
            }
            sgThreePointTeach->Cells[4][1]=0.0;
            sgThreePointTeach->Cells[4][2]=sTemp;
            sgThreePointTeach->Cells[4][4]=sTemp1;
            sgThreePointTeach->Cells[4][5]=0.0;

            if(InArm==0)//INARM
            {
                if(TeachPos==1)    //shuttl 1
                {
                    LastSet.fInShuttleXScale[0]=CheckRange(dXScale, 0.95, 1.05);
                    LastSet.fInShuttleYScale[0]=CheckRange(dYScale, 0.95, 1.05);
                }
                else if(TeachPos==2 )  //shuttl 2
                {
                    LastSet.fInShuttleXScale[1]=CheckRange(dXScale, 0.95, 1.05);
                    LastSet.fInShuttleYScale[1]=CheckRange(dYScale, 0.95, 1.05);
                }
                else if(TeachPos==3 ) //LOAD         LastSet.fOutShuttleXScale[0]
                {
                    LastSet.fLoaderTrayXScale=CheckRange(dXScale, 0.95, 1.05);
                    LastSet.fLoaderTrayYScale=CheckRange(dYScale, 0.95, 1.05);
                }
                else if(TeachPos==4 )  //Hotplate 1
                {
                    LastSet.fHotPlateXScale[0]=CheckRange(dXScale, 0.95, 1.05);
                    LastSet.fHotPlateYScale[0]=CheckRange(dYScale, 0.95, 1.05);
                }
                else if(TeachPos==5 )  //Hotplate 2
                {
                    LastSet.fHotPlateXScale[1]=CheckRange(dXScale, 0.95, 1.05);
                    LastSet.fHotPlateYScale[1]=CheckRange(dYScale, 0.95, 1.05);
                }
                else if(TeachPos==6 ) //Rotate
                {

                }
            }
            else
            {
                if(TeachPos==1) //shuttle1
                {
                    dXScale=ChangeToFloatNonPcnt((double)(6000.0), (double)(X_Dis));
                    dYScale=ChangeToFloatNonPcnt((double)(6000.0), (double)(Y_Dis));
                    LastSet.fOutShuttleXScale[0]=CheckRange(dXScale, 0.95, 1.05);
                    LastSet.fOutShuttleYScale[0]=CheckRange(dYScale, 0.95, 1.05);
                }
                else if(TeachPos==2 )  //shuttle2
                {
                    dXScale=ChangeToFloatNonPcnt((double)(6000.0), (double)(X_Dis));
                    dYScale=ChangeToFloatNonPcnt((double)(6000.0), (double)(Y_Dis));
                    LastSet.fOutShuttleXScale[1]=CheckRange(dXScale, 0.95, 1.05);
                    LastSet.fOutShuttleYScale[1]=CheckRange(dYScale, 0.95, 1.05);
                }
                else if(TeachPos==3 )   //AUTO 1
                {
                    dXScale=ChangeToFloatNonPcnt((double)(11000.0), (double)(X_Dis));
                    dYScale=ChangeToFloatNonPcnt((double)(27000.0), (double)(Y_Dis));
                    IniConfig.dTrayXScale[eAuto1]=CheckRange(dXScale, 0.95, 1.05);
                    IniConfig.dTrayXScale[eAuto1]=CheckRange(dYScale, 0.95, 1.05);
                }
                else if(TeachPos==4 ) //AUTO 2
                {
                    dXScale=ChangeToFloatNonPcnt((double)(11000.0), (double)(X_Dis));
                    dYScale=ChangeToFloatNonPcnt((double)(27000.0), (double)(Y_Dis));
                    IniConfig.dTrayXScale[eAuto2]=CheckRange(dXScale, 0.95, 1.05);
                    IniConfig.dTrayXScale[eAuto2]=CheckRange(dYScale, 0.95, 1.05);
                }
                else if(TeachPos==5 ) //AUTO 3
                {
                    dXScale=ChangeToFloatNonPcnt((double)(11000.0), (double)(X_Dis));
                    dYScale=ChangeToFloatNonPcnt((double)(27000.0), (double)(Y_Dis));
                    IniConfig.dTrayXScale[eAuto3]=CheckRange(dXScale, 0.95, 1.05);
                    IniConfig.dTrayXScale[eAuto3]=CheckRange(dYScale, 0.95, 1.05);
                }
                else if(TeachPos==6 )//FIX 1
                {
                    dXScale=ChangeToFloatNonPcnt((double)(11000.0), (double)(X_Dis));
                    dYScale=ChangeToFloatNonPcnt((double)(27000.0), (double)(Y_Dis));
                    IniConfig.dTrayXScale[eFix1]=CheckRange(dXScale, 0.95, 1.05);
                    IniConfig.dTrayYScale[eFix1]=CheckRange(dYScale, 0.95, 1.05);
                }
                else if(TeachPos==7 )//FIX 2
                {
                    dXScale=ChangeToFloatNonPcnt((double)(11000.0), (double)(X_Dis));
                    dYScale=ChangeToFloatNonPcnt((double)(27000.0), (double)(Y_Dis));
                    IniConfig.dTrayXScale[eFix2]=CheckRange(dXScale, 0.95, 1.05);
                    IniConfig.dTrayYScale[eFix2]=CheckRange(dYScale, 0.95, 1.05);
                }
                else if(TeachPos==8 )//FIX 3
                {
                    dXScale=ChangeToFloatNonPcnt((double)(11000.0), (double)(X_Dis));
                    dYScale=ChangeToFloatNonPcnt((double)(27000.0), (double)(Y_Dis));
                    IniConfig.dTrayXScale[eFix3]=CheckRange(dXScale, 0.95, 1.05);
                    IniConfig.dTrayYScale[eFix3]=CheckRange(dYScale, 0.95, 1.05);
                }
                else if(TeachPos==9 )//Rotate
                {

                }
            }
            sgThreePointTeach->Cells[2][2]=X_Dis;
            sgThreePointTeach->Cells[2][5]=Y_Dis;
            sgThreePointTeach->Cells[2][3]=X_Dis-iJigXDis;
            sgThreePointTeach->Cells[2][6]=Y_Dis-iJigYDis;
            sgThreePointTeach->Cells[3][3]=dXScale;
            sgThreePointTeach->Cells[3][6]=dYScale;
            if(SingleTeach==false)
            {
                iTeach3Point=0;
                AutoTeachiTeachPos++;    //全部要校正
                TeachPos=AutoTeachiTeachPos;//移到下一個校正位置
                if(InArm==0 && AutoTeachiTeachPos>6)
                {
                    iTask++;
                    Memo1->Lines->Add("Full Auto Teach Finish!");
                    break;         //Outarm  全部校正完
                }
                else if(InArm==1 && AutoTeachiTeachPos>9)
                {
                    iTask++;
                    Memo1->Lines->Add("Full Auto Teach Finish!");
                    break;         //inarm  全部校正完
                }
                iTask=5;
                break;
            }
            else
            {
                Memo1->Lines->Add("Auto Teach Finish!");
                iTask++;
            }
            break;
        case 131:
            InitAutoPosTask();
            fMain->Pause("AutoTeachPos 131");
            break;
        case 1000:  //Frank 20171213 (Steven) modify
            MOT[MInArmX].SetSpeed(20);
            MOT[MInArmY].SetSpeed(20);
            MOT[MInArmPitch].SetSpeed(10);
            MOT[MInShuttle1].SetSpeed(50);
            MOT[MInShuttle2].SetSpeed(50);
            for(i=0; i<8; i++)
            {
                MOT[MInArmZA+i].SetSpeed(10);
                MOT[MOutArmZA+i].SetSpeed(10);
            }
            MOT[MOutArmX].SetSpeed(10);
            MOT[MOutArmY].SetSpeed(10);
            MOT[MOutArmPitch].SetSpeed(10);
            MOT[MOutShuttle1].SetSpeed(50);   //kevin 20210312 add
            MOT[MOutShuttle1].SetSpeed(50);   //kevin 20210312 add

            iTask=1100;
            break;
        case 1100:
            if(TeachPos==3)             //kevin 20210303 add load Zteach
            {
                bLoadAutoTeachInarm=true;
            }
            else
            {
                bLoadAutoTeachInarm=false;
            }

            if(MoveInOutArmXYToTeachPos(TeachPos, iUseArm))
            {
                if(iUseArm==0)//kevin 20210629 Inarm
                {
                    if(iRecordXYPos[0][iUseArm][TeachPos]==0)   //kevin 20210629
                        iRecordXYPos[0][iUseArm][TeachPos]=MOT[0].ReadEncoderPos();    //kevin 20210629 record X pos

                    if(iRecordXYPos[1][iUseArm][TeachPos]==0)   //kevin 20210629
                        iRecordXYPos[1][iUseArm][TeachPos]=MOT[1].ReadEncoderPos();    //kevin 20210629 record Y pos
                }
                else
                {
                    if(iRecordXYPos[0][iUseArm][TeachPos]==0)   //kevin 20210629
                        iRecordXYPos[0][iUseArm][TeachPos]=MOT[19].ReadEncoderPos();    //kevin 20210629 record X pos

                    if(iRecordXYPos[1][iUseArm][TeachPos]==0)   //kevin 20210629
                        iRecordXYPos[1][iUseArm][TeachPos]=MOT[20].ReadEncoderPos();    //kevin 20210629 record Y pos
                }

                bLoadAutoTeachInarm=false;
                iTask=1500;
                AutoTeachZ(true, iUseArm,true,TeachPos);
                iAutoZXYMove=false;                        //kevin 20210223 Auto Teach X Y offset
                Panel2->Enabled = true;            //kevin 20210225
                if(CUSTOMER_CODE==CC_ASE_KaohSiung)  //kevin 20210317
                    iTask=1200;
            }
            break;
        case 1200:
            Panel2->Enabled=true;            //kevin 20210225
            if(InArm==0)//INARM
            {
                MOT[MInArmZA].MotorMove(-1000);

            }
            else
            {
                if(TestIF.iTestMode == SingleSite)
                    MOT[MOutArmZG].MotorMove(-1000);
                else
                     MOT[MOutArmZA].MotorMove(-1000);
            }

            if(iAutoZXYMove)                        //kevin 20210223 Auto Teach X Y offset
            {
                if(InArm==0)//INARM
                {
                    MOT[MInArmZA].MotorMove(ZSafePos);
                }
                else
                {
                    if(TestIF.iTestMode==SingleSite) //kevin 20210312
                        MOT[MOutArmZG].MotorMove(ZSafePos);
                    else
                        MOT[MOutArmZA].MotorMove(ZSafePos);
                }
                iTask=1300;
            }
            break;
        case 1300:
            if(InArm==0)//INARM
            {
                if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||                  //kevin 20210318 Y PITCH
                   USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                //Ztex 2024.02.24 Add HT-1132
                {
                    if(MOT[MInArmZA].MotorMove(ZSafePos))
                    {
                        iTask=1500;
                    }
                }
                else
                {
                    if(MOT[MInArmZA].MotorMove(ZSafePos))
                    {
                        iTask=1500;
                    }
                }
            }
            else
            {
                if(TestIF.iTestMode == SingleSite) //kevin 20210312
                {
                    if(MOT[MOutArmZG].MotorMove(ZSafePos))
                    {
                        iTask=1500;
                    }
                }
                else
                {
                    if(MOT[MOutArmZA].MotorMove(ZSafePos))
                    {
                        iTask=1500;
                    }
                }
            }
            break;
        case 1500:
            if(AutoTeachZ(false, iUseArm,true,TeachPos))
            {
                AutoTeachZ(true, iUseArm,true,TeachPos);       //kevin 20210302 add
                if(SingleTeach==false)
                {
                    if((InArm==0 && TeachPos <5) || (InArm==1 && TeachPos<2)) //INARM
                    {                                               //out arm sh 1 sh2
                       TeachPos++;
                       iTask=1000;
                       break;;
                    }
                    else
                    {
                        iTask=131;
                    }
                }
                else
                {
                    iTask=131;
                }

                if(InArm==0)
                    SendInarmClick(this);  //kevin 20210319 inarm 傳送資料
                else
                   SendOutarmClick(this);  //kevin 20210319 inarm 傳送資料

                fTeach->UpdateTempTech();
                fTeach->SaveFile(true);
//                WriteData("d:\\HT9045\\system\\tech.dat", (char *)&Tech.iZLoad, sizeof(TECH));

            }
            break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TZteach::SendDataClick(TObject *Sender)
{
    int j=9;
    if(iUseArm==0)
        j=6;

    for(int i=0; i<j; i++)
    {
        if(bSendDataToTeach[iUseArm][i]==true)//勾選要送資料
        {
            InoutArmPos[iUseArm][0][i]= atof(StringGrid1->Cells[1][i].c_str());//X 0: inarm  1:outarm     0:X 1: Y   0-9 : shuttle1 2
            InoutArmPos[iUseArm][1][i]= atof(StringGrid1->Cells[2][i].c_str());//Y 0: inarm  1:outarm     0:X 1: Y   0-9 : shuttle1 2
        }
    }

    if(iUseArm==0)
    {
        if(bSendDataToTeach[iUseArm][1]==true)//勾選要送資料
        {
            fTeach->setEditInSht1X->Text=StringGrid1->Cells[1][1];
            fTeach->setEditInSht1Y->Text=StringGrid1->Cells[2][1];
        }
        if(bSendDataToTeach[iUseArm][2]==true)//勾選要送資料
        {
            fTeach->setEditInSht2X->Text=StringGrid1->Cells[1][2];
            fTeach->setEditInSht2Y->Text=StringGrid1->Cells[2][2];
        }
        if(bSendDataToTeach[iUseArm][3]==true)//勾選要送資料
        {
            fTeach->setEditLoaderX->Text=StringGrid1->Cells[1][3];
            fTeach->setEditLoaderY->Text=StringGrid1->Cells[2][3];
        }
        if(bSendDataToTeach[iUseArm][4]==true)//勾選要送資料
        {
            fTeach->setEditHP1X->Text=StringGrid1->Cells[1][4];
            fTeach->setEditHP1Y->Text=StringGrid1->Cells[2][4];
        }
        if(bSendDataToTeach[iUseArm][5]==true)//勾選要送資料
        {
            fTeach->setEditHP2X->Text=StringGrid1->Cells[1][5];
            fTeach->setEditHP2Y->Text=StringGrid1->Cells[2][5];
        }
        if(bSendDataToTeach[iUseArm][6]==true)//勾選要送資料
        {
            fTeach->setEditRotateX->Text=StringGrid1->Cells[1][6];
            fTeach->setEditRotateY->Text=StringGrid1->Cells[2][6];
        }
    }
    else
    {
        if(bSendDataToTeach[iUseArm][1]==true)//勾選要送資料
            fTeach->setEditOutSht1X->Text=StringGrid1->Cells[4][1];
        if(bSendDataToTeach[iUseArm][1]==true)//勾選要送資料
            fTeach->setEditOutSht1Y->Text=StringGrid1->Cells[5][1];
        if(bSendDataToTeach[iUseArm][2]==true)//勾選要送資料
            fTeach->setEditOutSht2X->Text=StringGrid1->Cells[4][2];
        if(bSendDataToTeach[iUseArm][2]==true)//勾選要送資料
            fTeach->setEditOutSht2Y->Text=StringGrid1->Cells[5][2];
        if(bSendDataToTeach[iUseArm][3]==true)//勾選要送資料
            fTeach->setEditAuto1X->Text=StringGrid1->Cells[4][3];
        if(bSendDataToTeach[iUseArm][3]==true)//勾選要送資料
            fTeach->setEditAuto1Y->Text=StringGrid1->Cells[5][3];
        if(bSendDataToTeach[iUseArm][4]==true)//勾選要送資料
            fTeach->setEditAuto2X->Text=StringGrid1->Cells[4][4];
        if(bSendDataToTeach[iUseArm][4]==true)//勾選要送資料
            fTeach->setEditAuto2Y->Text=StringGrid1->Cells[5][4];
        if(bSendDataToTeach[iUseArm][5]==true)//勾選要送資料
            fTeach->setEditAuto3X->Text=StringGrid1->Cells[4][5];
        if(bSendDataToTeach[iUseArm][5]==true)//勾選要送資料
            fTeach->setEditAuto3Y->Text=StringGrid1->Cells[5][5];
        if(bSendDataToTeach[iUseArm][6]==true)//勾選要送資料
            fTeach->setEditFix1X->Text=StringGrid1->Cells[4][6];
        if(bSendDataToTeach[iUseArm][6]==true)//勾選要送資料
            fTeach->setEditFix1Y->Text=StringGrid1->Cells[5][6];
        if(bSendDataToTeach[iUseArm][7]==true)//勾選要送資料
            fTeach->setEditFix2X->Text=StringGrid1->Cells[4][7];
        if(bSendDataToTeach[iUseArm][7]==true)//勾選要送資料
            fTeach->setEditFix2Y->Text=StringGrid1->Cells[5][7];
        if(bSendDataToTeach[iUseArm][8]==true)//勾選要送資料
            fTeach->setEditFix3X->Text=StringGrid1->Cells[4][8];
        if(bSendDataToTeach[iUseArm][8]==true)//勾選要送資料
            fTeach->setEditFix3Y->Text=StringGrid1->Cells[5][8];
        if(bSendDataToTeach[iUseArm][9]==true)//勾選要送資料
            fTeach->setEditRotateOutX->Text=StringGrid1->Cells[4][9];
        if(bSendDataToTeach[iUseArm][9]==true)//勾選要送資料
            fTeach->setEditRotateOutY->Text=StringGrid1->Cells[5][9];
    }
}
//---------------------------------------------------------------------------
void __fastcall TZteach::StringGrid1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int ARow, ACol;
    TStringGrid *P;
    P=(TStringGrid *)Sender;
    P->MouseToCell(X, Y, ACol, ARow);
    if(ACol==3 && ARow>=1)    //JerryYang 20160920 ACol>=3改成==,避免Teaching位置被改掉
    {
        bSendDataToTeach[InArm][ARow] = !bSendDataToTeach[InArm][ARow];
        if(bSendDataToTeach[InArm][ARow])
        {
            StringGrid1->Cells[ACol][ARow]="V";
        }
        else
            StringGrid1->Cells[ACol][ARow]="";
    }
}
//---------------------------------------------------------------------------
void __fastcall TZteach::AutoTeachSingleClick(TObject *Sender)
{
    if(AutoTeachSingle->ItemIndex==1)
    {
        iINArmTeach= 1;
    }
}
//---------------------------------------------------------------------------

void __fastcall TZteach::sbLoaderClick(TObject *Sender)
{
    int Tag;
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;
    Tag=Ptr->Tag;
    Ptr->Down=false;
    if(Tag>=10)
        iUseArm=1;
    else
        iUseArm=0;
    iINArmTeach=Tag;
    iTeachPos=iINArmTeach%10;
    InitAutoPosTask();
    if(iUseArm==0)
    {
        StringGrid1->RowCount=7;
        StringGrid1->Height=151;
        StringGrid1->Cells[0][1]="In Shuttle 1";
        StringGrid1->Cells[0][2]="In Shuttle 2";
        StringGrid1->Cells[0][3]="Load     ";
        StringGrid1->Cells[0][4]="Hotplate1";
        StringGrid1->Cells[0][5]="Hotplate2";
        StringGrid1->Cells[0][6]="Rotate   ";
        StringGrid1->Cells[4][1]= fTeach->setEditInSht1X->Text;
        StringGrid1->Cells[5][1]= fTeach->setEditInSht1Y->Text;
        StringGrid1->Cells[4][2]= fTeach->setEditInSht2X->Text;
        StringGrid1->Cells[5][2]= fTeach->setEditInSht2Y->Text;
        StringGrid1->Cells[4][3]= fTeach->setEditLoaderX->Text;
        StringGrid1->Cells[5][3]= fTeach->setEditLoaderY->Text;
        StringGrid1->Cells[4][4]= fTeach->setEditHP1X->Text;
        StringGrid1->Cells[5][4]= fTeach->setEditHP1Y->Text;
        StringGrid1->Cells[4][5]= fTeach->setEditHP2X->Text;
        StringGrid1->Cells[5][5]= fTeach->setEditHP2Y->Text;
        StringGrid1->Cells[4][6]= fTeach->setEditRotateX->Text;
        StringGrid1->Cells[5][6]= fTeach->setEditRotateY->Text;

        switch(iTeachPos)
        {
          case  0:
              PanelTeachPosSelect->Caption="[In Arm] Z Calibration";
              break;
          case  1:
              PanelTeachPosSelect->Caption="[In Arm] Shuttle 1";
              break;
          case  2:
              PanelTeachPosSelect->Caption="[In arm] Shuttle 2";
              break;
          case  3:
              PanelTeachPosSelect->Caption="[In arm] Load";
              break;
          case  4:
              PanelTeachPosSelect->Caption="[In Arm] Hotplate 1";
              break;
          case  5:
              PanelTeachPosSelect->Caption="[In Arm] Hotplate 2";
              break;
          case  6:
              PanelTeachPosSelect->Caption="[In Arm] Rotate";
              break;
        }
    }
    else
    {
        StringGrid1->RowCount=10;
        StringGrid1->Height=214;
        StringGrid1->Cells[0][1]="Out Shuttle 1";
        StringGrid1->Cells[0][2]="Out Shuttle 2";
        StringGrid1->Cells[0][3]="Auto 1   ";
        StringGrid1->Cells[0][4]="Auto 2";
        StringGrid1->Cells[0][5]="Auto 3";
        StringGrid1->Cells[0][6]="Fix 1";
        StringGrid1->Cells[0][7]="Fix 2";
        StringGrid1->Cells[0][8]="Fix 3";
        StringGrid1->Cells[0][9]="Rotate   ";

        StringGrid1->Cells[4][1]= fTeach->setEditOutSht1X->Text;
        StringGrid1->Cells[5][1]= fTeach->setEditOutSht1Y->Text;
        StringGrid1->Cells[4][2]= fTeach->setEditOutSht2X->Text;
        StringGrid1->Cells[5][2]= fTeach->setEditOutSht2Y->Text;
        StringGrid1->Cells[4][3]= fTeach->setEditAuto1X->Text;
        StringGrid1->Cells[5][3]= fTeach->setEditAuto1Y->Text;
        StringGrid1->Cells[4][4]= fTeach->setEditAuto2X->Text;
        StringGrid1->Cells[5][4]= fTeach->setEditAuto2Y->Text;
        StringGrid1->Cells[4][5]= fTeach->setEditAuto3X->Text;
        StringGrid1->Cells[5][5]= fTeach->setEditAuto3Y->Text;
        StringGrid1->Cells[4][6]= fTeach->setEditFix1X->Text;
        StringGrid1->Cells[5][6]= fTeach->setEditFix1Y->Text;
        StringGrid1->Cells[4][7]= fTeach->setEditFix2X->Text;
        StringGrid1->Cells[5][7]= fTeach->setEditFix2Y->Text;
        StringGrid1->Cells[4][8]= fTeach->setEditFix3X->Text;
        StringGrid1->Cells[5][8]= fTeach->setEditFix3Y->Text;
        StringGrid1->Cells[4][9]= fTeach->setEditRotateOutX->Text;
        StringGrid1->Cells[5][9]= fTeach->setEditRotateOutY->Text;
        switch(iTeachPos)
        {
            case  0:
                PanelTeachPosSelect->Caption="[Out Arm] Z Calibration";
                break;
            case  1:
                PanelTeachPosSelect->Caption="[Out Arm] Shuttle 1";
                break;
            case  2:
                PanelTeachPosSelect->Caption="[Out Arm] Shuttle 2";
                break;
            case  3:
                PanelTeachPosSelect->Caption="[Out Arm] Auto 1";
                break;
            case  4:
                PanelTeachPosSelect->Caption="[Out Arm] Auto 2";
                break;
            case  5:
                PanelTeachPosSelect->Caption="[Out Arm] Auto 3";
                break;
            case  6:
                PanelTeachPosSelect->Caption="[Out Arm] Fix 1";
                break;
            case  7:
                PanelTeachPosSelect->Caption="[Out Arm] Fix 2";
                break;
            case  8:
                PanelTeachPosSelect->Caption="[Out Arm] Fix 3";
                break;
            case  9:
                PanelTeachPosSelect->Caption="[Out Arm] Rotate";
                break;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TZteach::FormDestroy(TObject *Sender)
{
    try
    {
        ReleaseDC(0, DcSGScale);
        delete pCanvasScale;                                                    //Steven 20160108 : release memory
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TZteach::FormDestroy");
    }
    LogSoftwareOffTime("TZteach, FormDestroy");                                 //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
void __fastcall TZteach::sgThreePointTeachDrawCell(TObject *Sender,
      int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
    pCanvasScale->Handle=DcSGScale;
    pCanvasScale->Brush->Color=clBtnFace;
    MyDrawText(pCanvasScale, Rect, "X", 0, 19, 23, 77);
    MyDrawText(pCanvasScale, Rect, "Y", 0, 19, 85, 137);
}
//---------------------------------------------------------------------------
double __fastcall TZteach::Angle(double X0, double Y0, double X1, double Y1, bool bCompareXY)
{
    double angle_temp=0.0;

    if(X1!=X0 && X1>X0 && Y1>Y0)           //第一象限
    {
        if(bCompareXY==false)
            angle_temp=ChangeToFloatNonPcnt((double)(atan((Y1-Y0)/(X1-X0))*180), (double)(M_PI));
        else
            angle_temp=-90+ChangeToFloatNonPcnt((double)(atan((Y1-Y0)/(X1-X0))*180), (double)(M_PI));    //與Y軸的夾角
    }
    else if(X1!=X0 && X0>X1 && Y1>Y0)      //第二象限
    {
        if(bCompareXY==false)
            angle_temp=ChangeToFloatNonPcnt((double)(atan((Y1-Y0)/(X1-X0))*180), (double)(M_PI));
        else
            angle_temp=90+ChangeToFloatNonPcnt((double)(atan((Y1-Y0)/(X1-X0))*180), (double)(M_PI));
    }
    else if(X1!=X0 && X0>X1 && Y0>Y1)      //第三象限
    {
        if(bCompareXY==false)
            angle_temp=ChangeToFloatNonPcnt((double)(atan((Y1-Y0)/(X1-X0))*180), (double)(M_PI));
        else
            angle_temp=-90+ChangeToFloatNonPcnt((double)(atan((Y1-Y0)/(X1-X0))*180), (double)(M_PI));
    }
    else if(X1!=X0 && X1>X0 && Y0>Y1)      //第四象限
    {
        if(bCompareXY==false)
            angle_temp=ChangeToFloatNonPcnt((double)(atan((Y1-Y0)/(X1-X0))*180), (double)(M_PI));
        else
            angle_temp=90+ChangeToFloatNonPcnt((double)(atan((Y1-Y0)/(X1-X0))*180), (double)(M_PI));
    }
    else if(X1==X0 && Y0>Y1)             //90度
    {
        angle_temp=0;
    }
    else if(X1==X0 && Y1>Y0)             //270度
    {
        angle_temp=0;
    }
    return (angle_temp);
}
//--------------------------------------------------------------------
void __fastcall TZteach::InitAutoPosTask()
{
    iAutoPosTask=1;
    iAutoTeachTask=1;
}
//--------------------------------------------------------------------
void __fastcall TZteach::SetAutoTeachMode()
{
    if(rgTeachMode->ItemIndex==0)  //JerryYang 20161012 是否使用3點校正
        b3PointTeach=false;
    else
        b3PointTeach=true;

    if(AutoTeachSingle->ItemIndex==0)
    {
        SingleTeach=true;   //使用單一選項TEACH
    }
    else
    {
        iTeachPos=1;
        SingleTeach=false;
    }
#ifndef SOFT_SIMULTE
    //jou 2014-06-17 修正沒裝sensor時不能做Z軸高度校正
    if(Sen[SnAutoTeach].Enable==false && (AutoTeachSingle->ItemIndex!=0 || iTeachPos!=0))
    {
        Memo1->Lines->Add("No Install SnAutoTeach sensor for database.");
        return;
    }
#endif
    AutoTeachiTeachPos=1;
}
//----------------------------------------------------------------------
void __fastcall TZteach::DisplayAutoTeachPos(int iInArm=0, int iTeachPos=0)
{
    if(iInArm==0)
    {
        switch(iTeachPos)
        {
          case  0:
              PanelTeachPosSelect->Caption="[In Arm] Z Calibration";
              break;
          case  1:
              PanelTeachPosSelect->Caption="[In Arm] Shuttle 1";
              break;
          case  2:
              PanelTeachPosSelect->Caption="[In arm] Shuttle 2";
              break;
          case  3:
              PanelTeachPosSelect->Caption="[In arm] Load";
              break;
          case  4:
              PanelTeachPosSelect->Caption="[In Arm] Hotplate 1";
              break;
          case  5:
              PanelTeachPosSelect->Caption="[In Arm] Hotplate 2";
              break;
          case  6:
              PanelTeachPosSelect->Caption="[In Arm] Rotate";
              break;
        }
    }
    else
    {
        switch(iTeachPos)
        {
            case  0:
                PanelTeachPosSelect->Caption="[Out Arm] Z Calibration";
                break;
            case  1:
                PanelTeachPosSelect->Caption="[Out Arm] Shuttle 1";
                break;
            case  2:
                PanelTeachPosSelect->Caption="[Out Arm] Shuttle 2";
                break;
            case  3:
                PanelTeachPosSelect->Caption="[Out Arm] Auto 1";
                break;
            case  4:
                PanelTeachPosSelect->Caption="[Out Arm] Auto 2";
                break;
            case  5:
                PanelTeachPosSelect->Caption="[Out Arm] Auto 3";
                break;
            case  6:
                PanelTeachPosSelect->Caption="[Out Arm] Fix 1";
                break;
            case  7:
                PanelTeachPosSelect->Caption="[Out Arm] Fix 2";
                break;
            case  8:
                PanelTeachPosSelect->Caption="[Out Arm] Fix 3";
                break;
            case  9:
                PanelTeachPosSelect->Caption="[Out Arm] Rotate";
                break;
        }
    }
}
//----------------------------------------------------------------------
void __fastcall TZteach::InOutArmZHome(int iInArm) //kevin 20171101
{
    int MotorData[2][2][4]=
    {
        {
            {MInArmZA,MInArmZC,MInArmZE,MInArmZG},
            {MInArmZB,MInArmZD,MInArmZF,MInArmZH}
        },
        {
            {MOutArmZA,MOutArmZC,MOutArmZE,MOutArmZG},
            {MOutArmZB,MOutArmZD,MOutArmZF,MOutArmZH}
        }
    };

    int OutMotorData[2][2][4]=
    {
        {
            {MInArmZA,MInArmZC,MInArmZE,MInArmZG},
            {MInArmZB,MInArmZD,MInArmZF,MInArmZH}
        },
        {
            {MOutArmZA,MOutArmZC,MOutArmZE,MOutArmZG},
            {MOutArmZB,MOutArmZD,MOutArmZF,MOutArmZH}
        }
    };

    if(iInArm==0)//INARM
    {
        //Inarm Z歸零
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<4; j++)
            {
                bInArmZHome[i][j]=false;
                InitProcessSingleMotorTask( MotorData[0][i][j]);
            }
        }
        //Inarm Z歸零
    }
    else
    {
        //Outarm Z歸零
        for(int i=0; i<2; i++)
        {
            for(int j=0; j<4; j++)
            {
                bOutArmZHome[i][j]=false;
                InitProcessSingleMotorTask( OutMotorData[1][i][j]);
            }
        }
        //OutArm
    }
}
//---------------------------------------------------------------------------
//kevin 20210208  Hotplate
//---------------------------------------------------------------------------
bool MoveInArmXYPickHotPlateAutoTeach(int iHotPlate)
{
    bool bResult=MoveInArmXYToHotPlatePlace(iPickHP, false);
    return bResult;
}
//------------------------------------------------------------------------------
//kevin 20210208
//------------------------------------------------------------------------------
bool MoveInArmXYPickShuttleAutoTeach(int iSht, int iArm)                        //kevin 20210312
{
    if(iArm==InArm)
    {
        if(MoveInArmXYToShuttle_9045(iSht, 0, false, true))                     //Steven 20240512 : 整合MoveInArmXYToShuttle
            return true;
    }
    else
    {
        switch(TestIF.iTestMode)
        {
            case SingleSite:
                    if(MoveOutArmToShuttleIncludeZ_9045_1x1_1(iSht, false))
                        return true;
                break;
            case DualSite2x1:
                    if(MoveOutArmToShuttleIncludeZ_9045_2x1_2(iSht, false))
                        return true;
                break;
            case DualSite:
                    if(MoveOutArmToShuttleIncludeZ_9045_1x2_2(iSht, false))
                        return true;
                break;
            case TriSite1X3:
                    if(MoveOutArmToShuttleIncludeZ_9045_1x3_4(iSht, false))
                        return true;
                break;
            case QualSite1X4:
            case _8Site1X4:
                    if(MoveOutArmToShuttleIncludeZ_9045_1x4_4(iSht, false))
                        return true;
                break;
            case QualSite2X2:
                    if(MoveOutArmToShuttleIncludeZ_9045_2x2_4(iSht))
                        return true;
                break;
            case _6Site2X3:
                    if(MoveOutArmToShuttleIncludeZ_9045_2x4_4(iSht, 0, false))
                        return true;
                break;
            case _8Site2X4:
            case _16Site4X4:
                    if(MoveOutArmToShuttleIncludeZ_9045_2x4_8(iSht))
                        return true;
                break;
            case _12Site2X6:
                    if(MoveOutArmToShuttleIncludeZ_9045_2x6_8(iSht, 0, false))
                         return true;
                break;
            case _16Site2X8:
            case _32Site4X8N:
            case _32Site4X8M:
                    if(MoveOutArmToShuttleIncludeZ_9045_2x8_8(iSht, 0, false))  //kevin 20210315 add shuttle
                        return true;
                break;
        }
    }
    return false;
}
//---------------------------------------------------------------------------
//kevin 20210208
//---------------------------------------------------------------------------
bool MoveInArmXYPickLoadAutoTeach(int iData)
{
    return SearchAndMoveInArmXYToLoad_9045();
}
//---------------------------------------------------------------------------
bool TZteach::MoveInOutArmXYToTeachPos(int TeachPos,int iInArm)
{
    int iX, iY, iVari, iMovePitchX=0;
    int iXPos=0, iYPos=0;
    int iShuttle=0;
    if(iInArm==0)//INARM
    {
        if(CUSTOMER_CODE==CC_ASE_KaohSiung)   //kevin 20210205 add autoteach Z
        {
            if(TeachPos==1 || TeachPos==2)         //shuttl 2
            {
                if(TeachPos==2) //kevin 20210223
                    iShuttle=1;
                 if(MoveInArmXYPickShuttleAutoTeach(iShuttle,iInArm))
                    return true;
                else
                    return false;
            }
            else if(TeachPos==3)    //LOAD
            {
                MOT[MMTrayY].Tray.Data[0][0]=HAS_IC;//kevin 20210220

                if(MoveInArmXYPickLoadAutoTeach(HAS_IC))
                    return true;
                else
                    return false;
            }
            else if(TeachPos==4 || TeachPos==5)    //Hotplate 2
            {
                if(TeachPos==4)    //Hotplate 1
                    iPickPlate[0]=1;
                else
                    iPickPlate[0]=0;

                if(MoveInArmXYPickHotPlateAutoTeach(iPickPlate[0]))
                    return true;
                else
                    return false;
            }
        }
        else
        {
            if(TeachPos==1)         //shuttl 1
            {
                //iXPos=Prod.XInArm_Shuttle1_Place[iInArmYBase][iInArmXBase]-2000+6000-TestIF.dSiteXPitch/2;
                iXPos=Prod.XInArm_Shuttle1_Place[iInArmYBase][iInArmXBase]-2000; //kevin 20210319
                iYPos=Prod.YInArm_Shuttle1_Place[iInArmYBase][iInArmXBase];
                if(XPitchIsStand()) //X Pitch is 4000mm
                    iMovePitchX=Prod.LoadForm.iXPitch*3;
                else
                    iMovePitchX=TestIF.dSiteXPitch+TestIF.dSiteXPitch/2;
            }
            else if(TeachPos==2)    //shuttl 2
            {
                //iXPos=Prod.XInArm_Shuttle2_Place[iInArmYBase][iInArmXBase]-2000+6000-TestIF.dSiteXPitch/2;
                iXPos=Prod.XInArm_Shuttle2_Place[iInArmYBase][iInArmXBase]-2000;
                iYPos=Prod.YInArm_Shuttle2_Place[iInArmYBase][iInArmXBase];
                if(XPitchIsStand()) //X Pitch is 4000mm
                    iMovePitchX=TestIF.dSiteXPitch*3;
                else
                    iMovePitchX=TestIF.dSiteXPitch+TestIF.dSiteXPitch/2;
            }
            else if(TeachPos==3)    //LOAD
            {
                iXPos=Prod.XInArm_Tray_Pick[iInArmYBase][iInArmXBase]+6000;
                iYPos=Prod.YInArm_Tray_Pick[iInArmYBase][iInArmXBase]-TestIF.dSiteYPitch; //kevin 20210318 add Y pitch
                if(XPitchIsStand()) //X Pitch is 4000mm
                    iMovePitchX=TestIF.dSiteXPitch*3;
                else
                    iMovePitchX=TestIF.dSiteXPitch+TestIF.dSiteXPitch/2;
            }
            else if(TeachPos==4)    //Hotplate 1
            {
                iXPos=Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase]+6000;
                if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||                  //kevin 20210318 Y PITCH
                   USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                //Ztex 2024.02.24 Add HT-1132
                    iYPos=Prod.YInArm_Plate1_Pick[iInArmYBase][iInArmXBase]-TestIF.dSiteYPitch; //kevin 20210318 add Y pitch
                else
                    iYPos=Prod.YInArm_Plate1_Pick[iInArmYBase][iInArmXBase]; //kevin 20210318 add Y pitch

                if(XPitchIsStand()) //X Pitch is 4000mm
                    iMovePitchX=TestIF.dSiteXPitch*3;
                else
                    iMovePitchX=TestIF.dSiteXPitch+TestIF.dSiteXPitch/2;
            }
            else if(TeachPos==5)    //Hotplate 2
            {
                iXPos=Prod.XInArm_Plate2_Pick[iInArmYBase][iInArmXBase]+6000;
                if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||                  //kevin 20210318 Y PITCH
                   USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                //Ztex 2024.02.24 Add HT-1132
                    iYPos=Prod.YInArm_Plate2_Pick[iInArmYBase][iInArmXBase]-TestIF.dSiteYPitch; //kevin 20210318 add Y pitch
                else
                    iYPos=Prod.YInArm_Plate2_Pick[iInArmYBase][iInArmXBase]; //kevin 20210318 add Y pitch

                if(XPitchIsStand()) //X Pitch is 4000mm
                    iMovePitchX=TestIF.dSiteXPitch*3;
                else
                    iMovePitchX=TestIF.dSiteXPitch+TestIF.dSiteXPitch/2;
            }


            iVari=GetInArmPitch_9045(iMovePitchX);

            TransferHotPlateRatio(false, &iXPos, &iYPos);    //Steven 20110324

            iX=iXPos;
            iY=iYPos;
            if(bMove[0]==false)
               bMove[0]=MOT[MInArmX].MotorMove(iX);

            if(bMove[1]==false)
               bMove[1]=MOT[MInArmY].MotorMove(iY);

            if(bMove[2]==false)
               bMove[2]=MOT[MInArmPitch].MotorMove(iVari);

            if(bMove[3]==false)
               bMove[3]=MOT[MInShuttle1].MotorMove(Tech.iInShuttle1Left );

            if(bMove[4]==false)
               bMove[4]=MOT[MInShuttle2].MotorMove(Tech.iInShuttle2Left);
        }
    }
    else
    {
        if(CUSTOMER_CODE==CC_ASE_KaohSiung &&(TeachPos==2 || TeachPos==1) )   //kevin 20210312 add autoteach Z
        {
            if(TeachPos==2) //kevin 20210223
                iShuttle=1;

            if(bMove[1]==false)
               bMove[1] = MOT[MInShuttle1].MotorMove(Tech.iInShuttle1Right );

            if(bMove[2]==false)
               bMove[2] = MOT[MInShuttle2].MotorMove(Tech.iInShuttle2Right );

            if(bMove[0] == false)
               bMove[0] = MoveInArmXYPickShuttleAutoTeach(iShuttle,iInArm);

            if(bMove[0] && bMove[1]&&bMove[2])
                return true;
            else
                return false;
        }
        //<=====================================  kevin 20210312 add autoteach Z
        if(TeachPos==1)         //shuttl 1
        {
            iXPos=Prod.XOutArm_Shuttle2_Pick[iOutArmYBase][iOutArmXBase]-2000+6000-TestIF.dSiteXPitch/2;
            iYPos=Prod.YOutArm_Shuttle2_Pick[iOutArmYBase][iOutArmXBase];
        }
        else if(TeachPos==2)         //shuttl 2
        {
            iXPos=Prod.XOutArm_Shuttle2_Pick[iOutArmYBase][iOutArmXBase]-2000+6000-TestIF.dSiteXPitch/2;
            iYPos=Prod.YOutArm_Shuttle2_Pick[iOutArmYBase][iOutArmXBase];
        }
        else if(TeachPos==3)    //Auto 1
        {
            iXPos=Prod.XOutArm_Auto_Place[0][iInArmYBase][iInArmXBase]+6000;
            iYPos=Prod.YOutArm_Auto_Place[0][iInArmYBase][iInArmXBase];
        }
        else if(TeachPos==6)    //Fix 1
        {
            iXPos=Prod.XOutArm_Fix_Place[0][iInArmYBase][iInArmXBase]+6000;
            iYPos=Prod.YOutArm_Fix_Place[0][iInArmYBase][iInArmXBase];
        }

        iVari=GetOutArmPitch_9045(9000);
        TransferHotPlateRatio(false, &iXPos, &iYPos);    //Steven 20110324

        iX=iXPos;
        iY=iYPos;
        if(bMove[0]==false)
            bMove[0]=MOT[MOutArmX].MotorMove(iX);

        if(bMove[1]==false)
            bMove[1]=MOT[MOutArmY].MotorMove(iY);

        if(bMove[2]==false)
            bMove[2]=MOT[MOutArmPitch].MotorMove(iVari);

        if(bMove[3]==false)
           bMove[3]=MOT[MInShuttle1].MotorMove(Tech.iInShuttle1Right );

        if(bMove[4]==false)
           bMove[4]=MOT[MInShuttle2].MotorMove(Tech.iInShuttle2Right );
    }

    if(bMove[0] && bMove[1]&&bMove[2]&& bMove[3]&&bMove[4])
        return true;
    return false;
}
//---------------------------------------------------------------------------
void __fastcall TZteach::rgTeachModeClick(TObject *Sender)
{
    iTeachMode = rgTeachMode->ItemIndex;                  //kevin 20210204 Auto Teach mode
    if(CUSTOMER_CODE==CC_ASE_KaohSiung &&  iTeachMode==2)      //kevin 20210505 add for ASE KH
    {
        sbFix1->Visible=false;
        sbFix2->Visible=false;
        sbFix3->Visible=false;
        sbAuto1->Visible=false;
        sbAuto2->Visible=false;
        sbAuto3->Visible=false;
        sbOutRotate->Visible=false;
    }
    else
    {
        sbFix1->Visible=true;
        sbFix2->Visible=true;
        sbFix3->Visible=true;
        sbAuto1->Visible=true;
        sbAuto2->Visible=true;
        sbAuto3->Visible=true;
        sbOutRotate->Visible=true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TZteach::cbEnableUseAreaZCalbrationClick(TObject *Sender)
{
    if(cbEnableUseAreaZCalbration->Checked==true)
    {
        sbFix2->Visible=false;
        sbFix3->Visible=false;
        sbAuto2->Visible=false;
        sbAuto3->Visible=false;
        rgTeachMode->Visible=false;
        AutoTeachSingle->Visible=false;
        sbInSht1->Visible=false;
        sbOutSht1->Visible=false;
        sbHotPlate1->Visible=false;
        bEnableUseAreaZCalbration=true;
    }
    else
    {
        sbFix2->Visible=true;
        sbFix3->Visible=true;
        sbAuto2->Visible=true;
        sbAuto3->Visible=true;
        rgTeachMode->Visible=true;
        AutoTeachSingle->Visible=true;
        sbInSht1->Visible=true;
        sbOutSht1->Visible=true;
        sbHotPlate1->Visible=true;
        bEnableUseAreaZCalbration=false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TZteach::UpdateMotorTeachMonitor(int Index)
{
    if(Index==-1)
        return;

    unsigned int Speed=10;

    MOT[Index].SetSpeed(Speed);
    if(Speed>100)
        Speed=100;
    if(Speed<1)
        Speed=1;
    Panel3->Caption=MOT[Index].NumberAlias;
}
//-----------------------------------------------------------------------------
void __fastcall TZteach::MotorInArmXClick(TObject *Sender)
{
    int Tag;
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;
    Tag=Ptr->Tag;

    if(Tag ==0)
        iUseMotor=0;
    else if(Tag ==1)//kevin 20210227 add out arm  Y
        iUseMotor=1;

    if(iUseArm==1)    //kevin 20210227 add out arm  X
    {
       if(Tag ==0)
          Tag =19;
       else if(Tag ==1)//kevin 20210227 add out arm  Y
          Tag =20;
    }
    ActiveMotorIndex=Tag;//TechMotorAxle[Tag]->MotorSelect;
    UpdateMotorTeachMonitor(ActiveMotorIndex);
}
//---------------------------------------------------------------------------
void __fastcall TZteach::SpeedButton13Click(TObject *Sender)
{
    int P1,P2;
    SpeedButton13->Down=false;
    if(ActiveMotorIndex==-1)
        return;

    if(Panel3->Caption=="")
    {
        ShowMyMessage("Choice X Y motor!", "選取控制 XY 馬達移動!!");
        return;
    }
    P1=atoi(edtPosition->Text.c_str());
    P2=atoi(ComboBox1->Text.c_str());

    if(abs(P1)>999999)  //Steven 20100831 : 溢位保護
    {
        ShowMyMessage("Position over limitation!", "移動位置超過限制!!");
        return;
    }

    MOT[ActiveMotorIndex].fCMD=false;
    MOT[ActiveMotorIndex].MotorMove(P1-P2);
}
//---------------------------------------------------------------------------
void __fastcall TZteach::SpeedButton12Click(TObject *Sender)
{
        SpeedButton12->Down=false;
    int P1,P2;

    P1=atoi(edtPosition->Text.c_str());
    P2=atoi(ComboBox1->Text.c_str());

    if(abs(P1)>999999)  //Steven 20100831 : 溢位保護
    {
        ShowMyMessage("Position over limitation!", "移動位置超過限制!!");
        return;
    }
    if(Panel3->Caption=="")
    {
        ShowMyMessage("Choice X Y motor!", "選取控制 XY 馬達移動!!");
        return;
    }

    MOT[ActiveMotorIndex].fCMD=false;
    MOT[ActiveMotorIndex].MotorMove(P1+P2);
}
//---------------------------------------------------------------------------
void __fastcall TZteach::AutoTimerTimer(TObject *Sender)
{
    double dPos=0;
    if(fShow==false)
        return;

    if(ActiveMotorIndex==-1)
        return;

//    long HOME_MOT_DATA;
    MOT[ActiveMotorIndex].ScanMotorStatus();
        edtPosition->Text=MOT[ActiveMotorIndex].ReadPos();
    if(MOT[ActiveMotorIndex].Motor->MotorType==1 || MOT[ActiveMotorIndex].Motor->MotorType==3 || MOT[ActiveMotorIndex].Motor->MotorType==4)
        pnlEncoderPos->Caption= MOT[ActiveMotorIndex].ReadEncoderPos();
    else
    {
        pnlEncoderPos->Caption= MOT[ActiveMotorIndex].ReadPos();

    }
    dPos= MOT[ActiveMotorIndex].ReadEncoderPos();

    if(iRecordXYPos[iUseMotor][iUseArm][iTeachPos]==0)   //kevin 20210629
    switch(ActiveMotorIndex)
    {
        case 0:
             if(iUseArm ==0 &&iUseMotor==0)      //In arm  Load  Offset
                AutoTeachOffset.dXPos[0]=dPos;
             else
                AutoTeachOffset.dYPos[0]=dPos;
            break;
        case 1:
             if(iUseArm ==0 &&iUseMotor==0)      //In arm  Shuttle 1  Offset
                AutoTeachOffset.dXPos[1]=dPos;
             else
                AutoTeachOffset.dYPos[1]=dPos;
            break;
        case 2:
             if(iUseArm ==0 &&iUseMotor==0)      //In arm  Shuttle 2  Offset
                AutoTeachOffset.dXPos[2]=dPos;
             else
                AutoTeachOffset.dYPos[2]=dPos;
            break;
        case 3:
             if(iUseArm ==0 &&iUseMotor==0)      //In arm  Load  Offset
                AutoTeachOffset.dXPos[3]=dPos;
             else
                AutoTeachOffset.dYPos[3]=dPos;
            break;
        case 4:
             if(iUseArm ==0 &&iUseMotor==0)      //In arm  HP1  Offset
                AutoTeachOffset.dXPos[4]=dPos;
             else
                AutoTeachOffset.dYPos[4]=dPos;
            break;
        case 5:
             if(iUseArm ==0 &&iUseMotor==0)      //In arm  HP2  Offset
                AutoTeachOffset.dXPos[5]=dPos;
             else
                AutoTeachOffset.dYPos[5]=dPos;
            break;
        case 6:
             if(iUseArm ==0 &&iUseMotor==0)      //In arm  Rotate  Offset
                AutoTeachOffset.dXPos[6]=dPos;
             else
                AutoTeachOffset.dYPos[6]=dPos;
            break;
    }

   if(bTestIndexVaccum)
   {
        if(bVacuumOnOff(iUseVacuum,iZ )==5)
            iZ++;

        if(iUseVacuum >=2 )//index Arm1 Arm2
        {
            if(iZ >15)
            {
                iZ=0;
                iUseVacuum++;
                iVaccumOnOff=1;
            }
        }
        else
        {
            if(iZ >8)    //in out arm
            {
                iZ=0;
                iUseVacuum++;
                iVaccumOnOff=1;
            }
        }

        if(iUseVacuum>=4)
            bTestIndexVaccum=false;
   }
}
//---------------------------------------------------------------------------
void __fastcall TZteach::SpeedButton1Click(TObject *Sender)
{
        iAutoZXYMove=true;
}
//---------------------------------------------------------------------------
void __fastcall TZteach::SaveFile(int iSelPartData, int SpecialMode, bool bReset)
{
    if(iSelPartData<0 || iSelPartData>OfsTotal)
        return;
    AnsiString szDir="", szFilePath="";

    szDir=fOffSet->GetOffsetPath();
    szFilePath=szDir;

    //kevin 20150105  Start 另存 jobfile
    SaveSetupFile(szDir, szFilePath, iSelPartData, SpecialMode, bReset);//kevin 20150105
    /*
    #ifdef ASE_KaohSiung
        fBuilder->bSaveAsJobFile(fOffSet->LastFileName, "JOBFILE");
    #endif
    */
    //kevin 20150105 end
}
//------------------------------------------------------------
//kevin 20210311 oFFSET  儲存 檔案另存 jobfile
//------------------------------------------------------------
void __fastcall TZteach::SaveSetupFile(AnsiString szDir, AnsiString szFilePath, int iSelPartData, int SpecialMode, bool bReset)
{
//    bool FilePathErr=false;
    int i, j;//, iNum;
    AnsiString str;
    int iOffsetUnit=0;                          //kevin 20210720 add log

    AnsiString CapStr[OfsTotal]=
    {
        "Loader",              //0
        "Hot Plate1",          //1
        "Hot Plate2",          //2
        "Input Shuttle1",      //3
        "Input Shuttle2",      //4
        "Output Shuttle1",     //5
        "Output Shuttle2",     //6
    };

    MyForceDirectories(szDir);
    szDir+="\\Position Offset.Data";

//    if(!FileExists(szDir))
//        FilePathErr=true;

    if(SpecialMode & iSaveStander)
    {
        szDir=szFilePath; //kevin 20150105
        szDir=szFilePath; //kevin 20150105
        if(LastSet.iTemperature==Tempture_Hot)       //kevin 20210720
            szDir+="\\Position Offset Hot.Data";
        else
            szDir+="\\Position Offset.Data";

         if(iSelPartData==5)               //kevin 20210720 add ootSHT
             iOffsetUnit=0;
         else if(iSelPartData==6)
             iOffsetUnit=1;

        if(iSelPartData==5 ||iSelPartData==6)  // OUT SHUTTLE 1 2
        {
            WriteIniData(szDir, CapStr[iSelPartData], "Hand X",   OutArmOffSet_File[iOffsetUnit]->GetX());        //kevin 20210720 add ootSHT
            WriteIniData(szDir, CapStr[iSelPartData], "Hand Y",   OutArmOffSet_File[iOffsetUnit]->GetY());        //kevin 20210720 add ootSHT
            WriteIniData(szDir, CapStr[iSelPartData], "PickUp",   OutArmOffSet_File[iOffsetUnit]->GetPickUp());    //kevin 20210720 add ootSHT
        }
        else
        {
            WriteIniData(szDir, CapStr[iSelPartData], "Hand X",   InArmOffSet[iSelPartData]->GetX());        //kevin 20210629 add
            WriteIniData(szDir, CapStr[iSelPartData], "Hand Y",   InArmOffSet[iSelPartData]->GetY());       //kevin 20210629 add
            WriteIniData(szDir, CapStr[iSelPartData], "PickUp",   InArmOffSet[iSelPartData]->GetPickUp());
            WriteIniData(szDir, CapStr[iSelPartData], "Place",   InArmOffSet[iSelPartData]->GetPlace());
        }

        for(i=0; i<4; i++)//8=In/out arm suction
        {
            for(j=0; j<2; j++)//8=In/out arm suction
            {
                if(iSelPartData==5 ||iSelPartData==6)
                {
                    AutoTeachOffset.dPick[iSelPartData][j][i] = CheckRange(double(OutArmOffSet_File[iOffsetUnit]->GetPickUp(j,i)),double(InputLimit.iOffsetZHigh), double(InputLimit.iOffsetZLow));   //JerryYang 20190328 Offset上下限保護
                    WriteIniData(szDir, CapStr[iSelPartData], str.sprintf("PickUp %c", 'A'+(i*2)+j), AutoTeachOffset.dPick[iSelPartData][j][i]);      //kevin 20210720 add ootSHT

                    AutoTeachOffset.dPick[iSelPartData][j][i] = CheckRange(double(OutArmOffSet_File[iOffsetUnit]->GetPlace(j,i)),double(InputLimit.iOffsetZHigh), double(InputLimit.iOffsetZLow));   //JerryYang 20190328 Offset上下限保護
                    WriteIniData(szDir, CapStr[iSelPartData], str.sprintf("Place %c", 'A'+(i*2)+j), AutoTeachOffset.dPick[iSelPartData][j][i]);
                }
                else
                {
                    AutoTeachOffset.dPick[iSelPartData][j][i] = CheckRange(double(InArmOffSet[iSelPartData]->GetPickUp(j,i)),double(InputLimit.iOffsetZHigh), double(InputLimit.iOffsetZLow));   //JerryYang 20190328 Offset上下限保護
                    WriteIniData(szDir, CapStr[iSelPartData], str.sprintf("PickUp %c", 'A'+(i*2)+j), AutoTeachOffset.dPick[iSelPartData][j][j]);
                    AutoTeachOffset.dPick[iSelPartData][j][i] = CheckRange(double(InArmOffSet[iSelPartData]->GetPlace(j,i)),double(InputLimit.iOffsetZHigh), double(InputLimit.iOffsetZLow));   //JerryYang 20190328 Offset上下限保護
                    WriteIniData(szDir, CapStr[iSelPartData], str.sprintf("Place %c", 'A'+(i*2)+j), AutoTeachOffset.dPick[iSelPartData][j][j]);
                }
            }
        }
    }

    AddSpace(szDir);
}
//==============================================================================
void __fastcall TZteach::SpeedButton14Click(TObject *Sender)
{
    fMain->Pause("Zteach");
}
//---------------------------------------------------------------------------
int TZteach::bVacuumOnOff(int iInArm, int iZMot)  // 0: inarm 1: out arm 3: Index
{
    int &iTask =iVaccumOnOff;//,iY=0,iX=0;  //kevin 20210322 add Vacuum on off
    double iTimerCount=0.0,iTimerCount1=0.0;
    int iSuckI[16]={0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1};
    int iSuckJ[16]={0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7};
    static int iInRow=0,iInCol=0;
    static int iOutRow=0,iOutCol=0;
    AnsiString asSuck[16]={"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P"};
    AnsiString asLog={""};
    static int iCheckCount[5]={0};
    bool bStatus=false;
    int iCheckMax=10;//檢查五次

    switch(iTask)
    {
        case 1:
             iCheckCount[iInArm]=0;
            iTask=100;
            break;
       case 100:
        //Sam 20210204 : Auto Teach Loader Z Record Vacuum On off time
        //==>
            //先將吸嘴內的真空破壞    0  2  4  6      0,0   0,1
            iInRow=iSuckI[iZMot%2];// 1  3  5  7      1,0   1,1
            iInCol=iSuckJ[iZMot];
            if(iInArm ==0)
               iTask=105;
            else if(iInArm ==1)
                 iTask=200;
            else if(iInArm ==2)
                 iTask=300;
            else if(iInArm ==3)
                 iTask=400;
            break;
        case 105:
            InArmSuck.Suck[iInRow][iInCol].Off();
            InVacuumTimer.SetSecAndOn(0.5);
            iTask=110;
            break;
        case 110:
             if(InVacuumTimer.Off())
             {
                InArmSuck.Suck[iInRow][iInCol].On();
                iTask=120;
             }
            break;
        case 120:
            //asLog.sprintf("%s,InArm %s,%d",Now().FormatString("yyyy/mm/dd hh:nn:ss"), asSuck[iZMot], iCheckCount[InArm]+1);
            asLog.sprintf("%s,InArm %s,%d",Now().FormatString("yyyy/mm/dd hh:nn:ss"), asSuck[iZMot], iCheckCount[iInArm]+1);
            sVacuumData=asLog;
            InTimer.LatchCycleTimeUS(true);
            iTask=130;
            //break;
        case 130:
            #ifdef SOFT_SIMULTE
                bStatus=true;
            #else
                bStatus=InArmSuck.Suck[iInRow][iInCol].Sensor();//GetStatus();
            #endif

            if(bStatus)
            {
                InArmSuck.Suck[iInRow][iInCol].Normal();
                iTimerCount=InTimer.LatchCycleTimeUS(false);
                iTimerCount1 = (double)iTimerCount/100000;
                asLog.sprintf(",%f",iTimerCount1);
                sVacuumData+= asLog;
                iTask=140;
            }
            else if(InTimer.LatchCycleTimeUS(false)>1000000)    //真空建立超出1s
            {
                asLog.sprintf("In Arm %s Head Vaccum On 測試異常", asSuck[iZMot]);
                VacuumOnOffLog(asLog);
                VacuumOnOffLog("<======");
                ShowMyMessage(asLog);
                InArmSuck.Suck[iInRow][iInCol].Normal();
                iTask=100;
            }
            break;
        case 140:
            InTimer.LatchCycleTimeUS(true);
            iTask=150;
            //break;
        case 150:
            bStatus=true;
            #ifdef SOFT_SIMULTE
                bStatus=false;
            #else
                bStatus=InArmSuck.Suck[iInRow][iInCol].Sensor();//GetStatus();
            #endif

            if(bStatus==false)
            {
                iTimerCount=InTimer.LatchCycleTimeUS(false);
                iTimerCount1 = (double)iTimerCount/100000;
                asLog.sprintf(",%f",iTimerCount1);
                sVacuumData+= asLog;
                VacuumOnOffLog(sVacuumData);
                iTask=160;
            }
            else if(InTimer.LatchCycleTimeUS(false)>1000000)    //真空破壞超出1s
            {
                InArmSuck.Suck[iInRow][iInCol].Normal();
                asLog.sprintf("In Arm %s Head Vaccum Off 測試異常", asSuck[iZMot]);
                VacuumOnOffLog(asLog);
                VacuumOnOffLog("<======");
                ShowMyMessage(asLog);
                iTask=100;
            }
            break;
        case 160:
            iCheckCount[iInArm]++;
            if(iCheckCount[iInArm]>=iCheckMax)
            {
                iTask=1;
                return 1;
            }
            else
            {
                iTask=100;
            }
            sVacuumData="";
            return 5;
        case 200:       //先將吸嘴內的真空破壞
            iOutRow=iSuckI[iZMot];
            iOutCol=iSuckJ[iZMot];
            OutArmSuck.Suck[iOutRow][iOutCol].Off();
            OutVacuumTimer.SetSecAndOn(0.2);
            iTask=210;
            break;
        case 210:
            if(OutVacuumTimer.Off())
            {
                iTask=220;
            }
            break;
        case 220:
            asLog.sprintf("Out Arm %s Head Vaccum Test %d", asSuck[iZMot], iCheckCount[iInArm]+1);
            VacuumOnOffLog(asLog);
            OutTimer.LatchCycleTimeUS(true);
            OutArmSuck.Suck[iOutRow][iOutCol].On();
            iTask=230;
            //break;
        case 230:
            #ifdef SOFT_SIMULTE
                bStatus=true;
            #else
                bStatus=OutArmSuck.Suck[iOutRow][iOutCol].GetStatus();
            #endif
            if(bStatus)
            {
                iTimerCount=OutTimer.LatchCycleTimeUS(false);
                iTimerCount1 = (double)iTimerCount/1000000;
                asLog.sprintf("Out Arm %s Head Vaccum On :(%f ms)", asSuck[iZMot], iTimerCount1);
                VacuumOnOffLog(asLog);
                iTask=240;
            }
            else if(OutTimer.LatchCycleTimeUS(false)>1000000)    //真空建立超出1s
            {
                OutArmSuck.Suck[iOutRow][iOutCol].Normal();
                asLog.sprintf("Out Arm %s Head Vaccum On 測試異常", asSuck[iZMot]);
                VacuumOnOffLog(asLog);
                VacuumOnOffLog("<======");
                ShowMyMessage(asLog);
                iTask=100;
            }
            break;
        case 240:
            OutTimer.LatchCycleTimeUS(true);
            OutArmSuck.Suck[iOutRow][iOutCol].Off();     //KEVIN 20210629
            iTask=250;
            //break;
        case 250:
            if(OutArmSuck.Suck[iOutRow][iOutCol].GetStatus()==false)
            {
                iTimerCount=OutTimer.LatchCycleTimeUS(false);
                iTimerCount1 = (double)iTimerCount/1000000;
                asLog.sprintf("Out Arm %s Head Vaccum Off :(%f ms)", asSuck[iZMot], iTimerCount1);
                VacuumOnOffLog(asLog);
                iTask=260;
            }
            else if(OutTimer.LatchCycleTimeUS(false)>1000000)    //真空破壞超出1s
            {
                OutArmSuck.Suck[iOutRow][iOutCol].Normal();
                asLog.sprintf("Out Arm %s Head Vaccum Off 測試異常", asSuck[iZMot]);
                VacuumOnOffLog(asLog);
                VacuumOnOffLog("<======");
                ShowMyMessage(asLog);
                iTask=100;
            }
            break;
        case 260:
            iCheckCount[iInArm]++;
            if(iCheckCount[iInArm]>=iCheckMax)
            {
                asLog.sprintf("Out Arm %s Head Vaccum 測試完成", asSuck[iZMot]);
                VacuumOnOffLog(asLog);
                VacuumOnOffLog("<======");
                bRecordVacuumTim=false;
                iTask=1;
                return 2;
            }
            iTask=100;
            return 5;
       case 300:       //先將吸嘴內的真空破壞    0  2  4  6      0,0   0,1
            iInRow=iSuckI[iZMot%2];            // 1  3  5  7      1,0   1,1
            iInCol=iSuckJ[iZMot];
            FTestSuck.Suck[iInRow][iInCol].Off();
            InVacuumTimer.SetSecAndOn(0.5);
            iTask=310;
            break;
        case 310:
            if(InVacuumTimer.Off())
            {
                FTestSuck.Suck[iInRow][iInCol].On();
                iTask=320;
            }
            break;
        case 320:
            //asLog.sprintf("%s,InArm %s,%d",Now().FormatString("yyyy/mm/dd hh:nn:ss"), asSuck[iZMot], iCheckCount[InArm]+1);
            asLog.sprintf("%s,Arm1 %s,%d",Now().FormatString("yyyy/mm/dd hh:nn:ss"), asSuck[iZMot], iCheckCount[iInArm]+1);
            sVacuumData=asLog;
            InTimer.LatchCycleTimeUS(true);
            iTask=330;
            //break;
        case 330:
            #ifdef SOFT_SIMULTE
                bStatus=true;
            #else
                bStatus=FTestSuck.Suck[iInRow][iInCol].Sensor();//GetStatus();
            #endif

            if(bStatus)
            {
                FTestSuck.Suck[iInRow][iInCol].Normal();
                iTimerCount=InTimer.LatchCycleTimeUS(false);
                iTimerCount1 = (double)iTimerCount/1000000;
                asLog.sprintf(",%f",iTimerCount1);
                sVacuumData+= asLog;
                iTask=340;
            }
            else if(InTimer.LatchCycleTimeUS(false)>1000000)    //真空建立超出1s
            {
                asLog.sprintf("Arm1 %s Head Vaccum On 測試異常", asSuck[iZMot]);
                VacuumOnOffLog(asLog);
                VacuumOnOffLog("<======");
                ShowMyMessage(asLog);
                FTestSuck.Suck[iInRow][iInCol].Normal();
                iTask=100;
            }
            break;
        case 340:
            InTimer.LatchCycleTimeUS(true);
            iTask=350;
            //break;
        case 350:
            bStatus=true;
            #ifdef SOFT_SIMULTE
                bStatus=false;
            #else
                bStatus=FTestSuck.Suck[iInRow][iInCol].Sensor();//GetStatus();
            #endif

            if(bStatus==false)
            {
                iTimerCount=InTimer.LatchCycleTimeUS(false);
                iTimerCount1 = (double)iTimerCount/1000000;
                asLog.sprintf(",%f",iTimerCount1);
                sVacuumData+= asLog;
                VacuumOnOffLog(sVacuumData);
                iTask=360;
            }
            else if(InTimer.LatchCycleTimeUS(false)>1000000)    //真空破壞超出1s
            {
                FTestSuck.Suck[iInRow][iInCol].Normal();
                asLog.sprintf("Arm1 %s Head Vaccum Off 測試異常", asSuck[iZMot]);
                VacuumOnOffLog(asLog);
                VacuumOnOffLog("<======");
                ShowMyMessage(asLog);
                iTask=100;
            }
            break;
        case 360:
            iCheckCount[iInArm]++;
            if(iCheckCount[iInArm]>=iCheckMax)
            {
                iCheckCount[iInArm]=0;
                iTask=100;
                return 5;
            }
            else
            {
                iTask=100;
            }
            sVacuumData="";
            break;

        case 400:       //先將吸嘴內的真空破壞    0  2  4  6      0,0   0,1
            iInRow=iSuckI[iZMot%2];            // 1  3  5  7      1,0   1,1
            iInCol=iSuckJ[iZMot];
            BTestSuck.Suck[iInRow][iInCol].Off();
            InVacuumTimer.SetSecAndOn(0.5);
            iTask=410;
            break;
        case 410:
            if(InVacuumTimer.Off())
            {
                BTestSuck.Suck[iInRow][iInCol].On();
                iTask=420;
            }
            break;
        case 420:
            //asLog.sprintf("%s,InArm %s,%d",Now().FormatString("yyyy/mm/dd hh:nn:ss"), asSuck[iZMot], iCheckCount[InArm]+1);
            asLog.sprintf("%s,Arm2 %s,%d",Now().FormatString("yyyy/mm/dd hh:nn:ss"), asSuck[iZMot], iCheckCount[iInArm]+1);
            sVacuumData=asLog;
            InTimer.LatchCycleTimeUS(true);
            iTask=430;
            //break;
        case 430:
            #ifdef SOFT_SIMULTE
                bStatus=true;
            #else
                bStatus=BTestSuck.Suck[iInRow][iInCol].Sensor();//GetStatus();
            #endif

            if(bStatus)
            {
                BTestSuck.Suck[iInRow][iInCol].Normal();
                iTimerCount=InTimer.LatchCycleTimeUS(false);
                iTimerCount1 = (double)iTimerCount/100000;
                asLog.sprintf(",%f",iTimerCount1);
                sVacuumData+= asLog;
                iTask=440;
            }
            else if(InTimer.LatchCycleTimeUS(false)>1000000)    //真空建立超出1s
            {
                asLog.sprintf("Arm2 %s Head Vaccum On 測試異常", asSuck[iZMot]);
                VacuumOnOffLog(asLog);
                VacuumOnOffLog("<======");
                ShowMyMessage(asLog);
                BTestSuck.Suck[iInRow][iInCol].Normal();
                iTask=1;
            }
            break;
        case 440:
            InTimer.LatchCycleTimeUS(true);
            iTask=450;
            //break;
        case 450:
            bStatus=true;
            #ifdef SOFT_SIMULTE
                bStatus=false;
            #else
                bStatus=BTestSuck.Suck[iInRow][iInCol].Sensor();//GetStatus();
            #endif

            if(bStatus==false)
            {
                iTimerCount=InTimer.LatchCycleTimeUS(false);
                iTimerCount1 = (double)iTimerCount/100000;
                asLog.sprintf(",%f",iTimerCount1);
                sVacuumData+= asLog;
                VacuumOnOffLog(sVacuumData);
                iTask=460;
            }
            else if(InTimer.LatchCycleTimeUS(false)>1000000)    //真空破壞超出1s
            {
                BTestSuck.Suck[iInRow][iInCol].Normal();
                asLog.sprintf("Arm 2 %s Head Vaccum Off 測試異常", asSuck[iZMot]);
                VacuumOnOffLog(asLog);
                VacuumOnOffLog("<======");
                ShowMyMessage(asLog);
                iTask=1;
            }
            break;
        case 460:
            iCheckCount[iInArm]++;
            if(iCheckCount[iInArm]>=iCheckMax)
            {
                iCheckCount[iInArm]=0;
                iTask=100;
                return 5;
            }
            else
            {
                iTask=100;
            }
            sVacuumData="";
            break;
        //<==
    }
    return 0;
}
//---------------------------------------------------------------------------
void __fastcall TZteach::Button1Click(TObject *Sender)
{
    iVaccumOnOff=1;
    bTestIndexVaccum=true;
    iUseVacuum=2;        //arm1
    iZ=0;
}
//---------------------------------------------------------------------------
void __fastcall TZteach::AEdit1Click(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 100, -3000);
}
//---------------------------------------------------------------------------
void __fastcall TZteach::edtPositionClick(TObject *Sender)
{
    if(ActiveMotorIndex==-1)
        return;

    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, MOT[ActiveMotorIndex].Motor->PSoftLimitP, MOT[ActiveMotorIndex].Motor->PSoftLimitN);
}
//---------------------------------------------------------------------------