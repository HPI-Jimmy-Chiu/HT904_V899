#include "MachineDefine.h"
#pragma hdrstop

#include "acatchcassette.h"

#include "MachineType.h"
#include "note.h"
#include "main.h"
#include "mymessbox.h"
#include "asendic.h"
#include "mymotor.h"
#include "mycylin.h"
#include "mysensor.h"
#include "myswitch.h"
#include "MyKitSuck.h"
#include "cprod.h"
#include "cpublic.h"
#include "uHGemHT9045.h"
#include "cmydef.h"
#include "csystem.h"
#include "Cassette.h"
#include "AutoRetest.h"
#include "RFID.h"
#include "uLotInfo.h"
#include "InputForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
int iCatch_Cassette_Push[10]={1};
int iCatch_Cassette_Pop[10]={1};
bool bLoadNeedTray=false;
bool bLoadNeedFailTray=false;
bool bEmptyNeedTray=false;
bool bLoadHasStackedTray=false;
bool bEmptyHasStackedTray=false;
//bool bAutoFullTray=false;
bool bBufferHasNewCassette=false;
bool bBufferHasOldCassette=false;
bool bUnloadRobotHasCassette=false;
bool bBuffer6NeedCassette=false;
bool bBuffer6NoNeedCassette=false;
bool bLoadPortHaveCassette=false;
bool bBuffer1HaveCassette=false;
bool bBuffer6CanUp=false;
bool bLoadPortHasCassette=false;
//int iMRStartMode=0;
bool bBuffer6CombineTray=false;
bool bBuffer6SeparationTray=false;
bool bEmptyFull=false;
bool bMRQAMode=false;
bool bMRQAModePassBin=false;
bool bBuffer6NeedEmpty=false;
bool bCassetteMessage=false;
bool bSTArmMove[3]={false, false, false};
bool bSTMove=false;
AnsiString SenIndexName[6]={"Loader Tray", "Empty Tray", "Tray Bracket",
                            "Auto1 Tray", "Auto2 Tray", "Auto3 Tray"};
//------------------------------------------------------------------------------
void ShowCassetteTrayData()
{
    int iBufferNo=0;
    AnsiString asCassette;
    for(int i=0; i<10; i++)
    {
        iBufferNo=iGetMMCABuffer(i);  //Sam 20190112 LM
        if(MOT[iBufferNo].Tray.Data[0][0]==HAS_CASSETTE_EMPTY)
        {
            asCassette="EMPTY";
        }
        else if(MOT[iBufferNo].Tray.Data[0][0]==HAS_CASSETTE_TRAY)
        {
            asCassette="TRAY";
        }
        else if(MOT[iBufferNo].Tray.Data[0][0]==HAS_CASSETTE_DEVICE)
        {
            asCassette="DEVICE";
        }
        else if(MOT[iBufferNo].Tray.Data[0][0]==HAS_CASSETTE_FULLTRAY)
        {
            asCassette="FULL TRAY";
        }
        else if(MOT[iBufferNo].Tray.Data[0][0]==HAS_CASSETTE_PASS)
        {
            asCassette="PASS BIN";
        }
        else if(MOT[iBufferNo].Tray.Data[0][0]==HAS_CASSETTE_FAIL)
        {
            asCassette="FAIL BIN";
        }
        else
        {
            asCassette="";
        }

        fCassette->sgMRBufferList->Cells[i+1][2]=asCassette;
        fLotInfo->sgMRBufferLotList->Cells[2][i+1]=asCassette;

        iBufferDataType[i]=MOT[iBufferNo].Tray.Data[0][0];

        fCassette->sgMRBufferList->Cells[i+1][ 1]=MOT[iBufferNo].Tray.cCassetteID;
        fLotInfo->sgMRBufferLotList->Cells[1][i+1]=MOT[iBufferNo].Tray.cCassetteID;
        asBufferCassetteID[i]=MOT[iBufferNo].Tray.cCassetteID;

        fCassette->sgMRBufferList->Cells[i+1][ 3]=MOT[iBufferNo].Tray.cCassetteLot;
        fLotInfo->sgMRBufferLotList->Cells[3][i+1]=MOT[iBufferNo].Tray.cCassetteLot;
        asBufferLotID[i]=MOT[iBufferNo].Tray.cCassetteLot;
    }
    fLotInfo->labNowLotID->Caption=asNowLotID;
    fLotInfo->labNextLotID->Caption=asNextLotID;
    fMain->labHP1LotID->Caption=CheckNewLotID1();
    fMain->labHP2LotID->Caption=CheckNewLotID2();
    ShowCassetteData(0);
}
//---------------------------------------------------------------------------
void ShowTrayCountData()
{
    for(int i=0; i<6; i++)
    {
        fCassette->sgMRTrayList->Cells[i+1][1]=iMRTrayCount[i];
    }
}
//---------------------------------------------------------------------------
void ShowCassetteData(int iFunction, AnsiString asCassetteLot)
{
    bool bCheckLotOK=false;

    if(iFunction==0)
    {
        if(asCassetteLot!="")
        {
            for(int i=0; i<4; i++)
            {
                if(asMRLotData[0][i]==asCassetteLot)
                    bCheckLotOK=true;
            }

            if(bCheckLotOK==false)
            {
                for(int i=0; i<4; i++)
                {
                    if(asMRLotData[0][i]=="")
                    {
                        asMRLotData[0][i]=asCassetteLot;
                        iMRLotCount=i;
                        break;
                    }
                }
            }
        }
        for(int i=0; i<4; i++)
        {
            if(asMRLotData[0][i]!="")
            {
                if(asMRLotData[0][i]==asNowLotID)
                {
                    if(iATRFtRtMode==0)
                        asMRLotData[1][i]="FT";
                    else if(iATRFtRtMode==1)
                        asMRLotData[1][i]="RT";
                }
                else if(asMRLotData[0][i]==asNextLotID)
                {
                    asMRLotData[1][i]="Continuous Lot";
                }
                else
                {
                    asMRLotData[1][i]="Buffer Wait";
                }
            }
        }
    }
    else if(iFunction==1)
    {
        for(int i=0; i<4; i++)
        {
            if(i==3)
            {
                asMRLotData[0][i]="";
                asMRLotData[1][i]="";
                asMRLotData[2][i]="";
            }
            else
            {
                asMRLotData[0][i]=asMRLotData[0][i+1];
                asMRLotData[1][i]=asMRLotData[1][i+1];
                asMRLotData[2][i]=asMRLotData[2][i+1];
            }
        }
    }

    for(int i=0; i<4; i++)
    {
        fLotInfo->sgMRStatus->Cells[1][i+1]=asMRLotData[0][i];
        fLotInfo->sgMRStatus->Cells[2][i+1]=asMRLotData[1][i];
        fLotInfo->sgMRStatus->Cells[3][i+1]=asMRLotData[2][i];
    }
}
//---------------------------------------------------------------------------
void SetCassetteTray(int Mot, int Mot1)
{
    MOT[Mot].CopyFromTray(0, 0, 0, 0, MOT[Mot1]);
    ShowCassetteTrayData();
}
//---------------------------------------------------------------------------
AnsiString CheckNewLotID2()
{
    for(int j=0; j<=MOT[MMPlate2].Tray.YItem-1; j++)
    {
        for(int i=0; i<=MOT[MMPlate2].Tray.XItem-1; i++)
        {
            if(MOT[MMPlate2].Tray.Data[j][i]==HAS_IC ||
               MOT[MMPlate2].Tray.Data[j][i]==HAS_HOT_IC)
            {
                 return MOT[MMPlate2].Tray.cDeviceTrayLot[j][i];
            }
        }
    }
    return "";
}
//------------------------------------------------------------------------------
AnsiString CheckNewLotID1()
{
    for(int j=0; j<=MOT[MMPlate1].Tray.YItem-1; j++)
    {
        for(int i=0; i<=MOT[MMPlate1].Tray.XItem-1; i++)
        {
            if(MOT[MMPlate1].Tray.Data[j][i]==HAS_IC ||
               MOT[MMPlate1].Tray.Data[j][i]==HAS_HOT_IC)
            {
                 return MOT[MMPlate1].Tray.cDeviceTrayLot[j][i];
            }
        }
    }
    return "";
}
//------------------------------------------------------------------------------
bool MoveCatchArmZToSafe(int iMotorName)                                        //上升到安全高度
{
    bool OK=true;
    int iAlarmArm=0;
    static int iRetryCount=0;
    static int iRetryFail=0;
    bool bInLedFlag[2]={false,false};

    if(iMotorName==MCasArmZ)
    {
        iAlarmArm=0;
        if(MOT[iMotorName].MotorMove(200)==false)
        {
            OK=false;
        }
    }
    else
    {
        iAlarmArm=1;
        if(MOT[iMotorName].MotorMove(10)==false)
        {
            OK=false;
        }
    }

    if(OK)
    {
        if(MOT[iMotorName].Motor->Enable)
        {
            MOT[iMotorName].ScanMotorStatus();
            bInLedFlag[iAlarmArm]=MOT[iMotorName].Led[iHomeLed];
        }
        else
        {
            bInLedFlag[iAlarmArm]=true;
        }

        if(bInLedFlag[iAlarmArm]==false)
        {
            if(iRetryCount>50)
            {
                iRetryCount=0;
                if(iRetryFail<3)
                {
                    iRetryFail++;
                    InitDoMRArmZHome();
                    bMRNeedArmZHome=true;
                    if(iMotorName==101)
                    {
                        bMRArmZNeedHome[0]=true;
                        bIsMRArmHome[0]=true;
                    }
                    else if(iMotorName==104)
                    {
                        bMRArmZNeedHome[1]=true;
                        bIsMRArmHome[1]=true;
                    }
                }
                else
                {
                    if(iAlarmArm==0)
                        ShowErrorMessage("WAR0157", 0, MCasArmZ, false, MOT[iMotorName].NumberAlias);           //入料手臂 Z 沒有在歸零的位
                    else
                        ShowErrorMessage("WAR0157", 0, MStackedTrayZ, false, MOT[iMotorName].NumberAlias);      //入料手臂 Z 沒有在歸零的位
                    iRetryFail=0;
                }
                return false;
            }
            else
            {
                iRetryCount++;
                return false;
            }
        }
        iRetryFail=0;
        iRetryCount=0;
    }

    if(OK==true && bInLedFlag[iAlarmArm])
    {
        return true;
    }
    else
    {
        return false;
    }

}
//==============================================================================
int WhichBufferCanPlace()//Buffer2~Buffer9
{
    //Cassette2 = 0
    int iBufferOrder[8]={3,5,2,6,1,7,0,8};

    for(int i=0; i<8; i++)
    {
        #ifdef SOFT_SIMULTE
        if(MOT[MMCABuffer2+iBufferOrder[i]].HasIC()==false)
        #else
        if(bCatchArmNoCassette && MOT[MMCABuffer2+iBufferOrder[i]].HasIC()==false && Sen[SnBuffer2HaveCassette+iBufferOrder[i]].IsOn())
        {
            return iBufferOrder[i]+1;
        }
        else if((LastSet.iRealDummy==DUMMY && MOT[MMCABuffer2+iBufferOrder[i]].HasIC()==false) ||
           (LastSet.iRealDummy!=DUMMY && Sen[SnBuffer2HaveCassette+iBufferOrder[i]].IsOn()==false && MOT[MMCABuffer2+iBufferOrder[i]].HasIC()==false))
        #endif
        {
            return iBufferOrder[i]+1;
        }
    }
    return 0;
}
//==============================================================================
int WhichBufferCanCatch(int Data, int iCheckLot)        //Buffer2開始算
{
    //Cassette2 = 0
    int iBufferOrder[8]={3,5,2,6,1,7,0,8};
    AnsiString asNext="";

    for(int i=0; i<8; i++)
    {
        if(MOT[MMCABuffer2+iBufferOrder[i]].Tray.Data[0][0]==Data && CheckHasCassette(iBufferOrder[i]+1))
        {
            if(iCheckLot==1)
            {
                if(MOT[MMCABuffer2+iBufferOrder[i]].Tray.cCassetteLot==fLotInfo->edtSysLotID->Text)
                {
                    return iBufferOrder[i]+1;
                }
            }
            else if(iCheckLot==2)
            {
                if(MOT[MMCABuffer2+iBufferOrder[i]].Tray.cCassetteLot==asNextLotID)
                {
                    return iBufferOrder[i]+1;
                }
            }
            else if(iCheckLot==3)
            {
                for(int i=0; i<4; i++)
                {
//                    if(fLotInfo->edtSysLotID->Text!="")
                    {
                        if(asMRLotData[0][i]!="" && asMRLotData[0][i]!=fLotInfo->edtSysLotID->Text)
                        {
                            asNext=asMRLotData[0][i];
                            break;
                        }
                    }
//                    else
//                    {
//                        asNext=fLotInfo->edtSysLotID->Text;
//                        break;
//                    }
                }
                if(MOT[MMCABuffer2+iBufferOrder[i]].Tray.cCassetteLot==asNext)
                {
                    return iBufferOrder[i]+1;
                }
            }
            else
            {
                return iBufferOrder[i]+1;
            }
        }
    }
    return 0;
}
//==============================================================================
bool WhichEmptyCanCatch(int Data)        //Buffer1開始算
{
    int iMMCABuffer=0;
    for(int i=0; i<9; i++)
    {
        iMMCABuffer=iGetMMCABuffer(i);  //Sam 20190112 LM
        if(MOT[iMMCABuffer].Tray.Data[0][0]==Data && CheckHasCassette(i))
        {
            return true;
        }
    }
    return false;
}
//==============================================================================
bool CheckHasCassette(int Buffer)                                               //Buffer HasCassette 只檢查硬體
{
    #ifndef SOFT_SIMULTE
    //Sam 20190112 LM
    //==>
    int iSensor=0;
    if(USE_MR_SYSTEM==2 && (Buffer==0 || Buffer==9))
    {
        if(Buffer==0)
        {
            iSensor=SnLoadRobotHaveCassette1;
        }
        else
        {
            iSensor=SnUnloadRobotHaveCassette1;
        }
        if(LastSet.iRealDummy==DUMMY ||
          (LastSet.iRealDummy!=DUMMY && Sen[iSensor].IsOn() && Sen[iSensor+1].IsOn()))
        {
            return true;
        }
        else if(Sen[iSensor].IsOn() ^ Sen[iSensor+1].IsOn())
        {
             ShowMyMessage("Cassette is floating!");
        }
    }
    //<==
    //Sam 20190112 LM
    else
    {
        if(LastSet.iRealDummy==DUMMY ||
          (LastSet.iRealDummy!=DUMMY && Sen[SnBuffer1HaveCassette+Buffer].IsOn()))
        {
            return true;
        }
        else
        {
            ShowMyMessage("Cassette Lose!");
        }
    }
    return false;
    #else
    return true;
    #endif
}
//==============================================================================
bool CheckHasCassetteSensor(int Buffer)                                         //Buffer HasCassette 檢查硬體 & 軟體
{
    //Sam 20190112 LM
    //==>
    int iSensor=0;
    int iMot=0;
    if(USE_MR_SYSTEM==2 && (Buffer==0 || Buffer==9))
    {
        if(Buffer==0)
        {
            iSensor=SnLoadRobotHaveCassette1;
            iMot=MLoadRobotZ;
        }
        else
        {
            iSensor=SnUnloadRobotHaveCassette1;
            iMot=MUnloadRobotZ;
        }
        if(MOT[iMot].HasIC() &&
          (LastSet.iRealDummy==DUMMY ||
          (LastSet.iRealDummy!=DUMMY && Sen[iSensor].IsOn() && Sen[iSensor+1].IsOn())))
        {
            return true;
        }
        else if(MOT[iMot].HasIC() && (Sen[iSensor].IsOff() ^ Sen[iSensor+1].IsOn()))
        {
            return false;
        }
    }
    //<==
    //Sam 20190112 LM
    else
    {
        if(MOT[MMCABuffer1+Buffer].HasIC() &&
           (LastSet.iRealDummy==DUMMY ||
           (LastSet.iRealDummy!=DUMMY && Sen[SnBuffer1HaveCassette+Buffer].IsOn())))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}
//==============================================================================
bool CheckNoCassetteSensor(int Buffer)                                          //Buffer NoCassette 檢查硬體 & 軟體
{
    //Sam 20190112 LM
    //==>
    int iSensor=0;
    int iMot=0;
    if(USE_MR_SYSTEM==2 && (Buffer==0 || Buffer==9))
    {
        if(Buffer==0)
        {
            iSensor=SnLoadRobotHaveCassette1;
            iMot=MLoadRobotZ;
        }
        else
        {
            iSensor=SnUnloadRobotHaveCassette1;
            iMot=MUnloadRobotZ;
        }
        if(MOT[iMot].HasIC()==false &&
           (LastSet.iRealDummy==DUMMY ||
           (LastSet.iRealDummy!=DUMMY && Sen[iSensor].IsOn()==false && Sen[iSensor+1].IsOn()==false)))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    //<==
    //Sam 20190112 LM
    else
    {
        if(MOT[MMCABuffer1+Buffer].HasIC()==false &&
           (LastSet.iRealDummy==DUMMY ||
           (LastSet.iRealDummy!=DUMMY && Sen[SnBuffer1HaveCassette+Buffer].IsOn()==false)))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}
//==============================================================================
bool MCasArmXCanSafeMove()                                                      //上升到安全高度  LM Move Buffer1 && Buffer10 用這個就好了
{
    if(MoveCatchArmZToSafe(MCasArmZ))
        return true;
    return false;
}
//------------------------------------------------------------------------------
bool MCasArmXCanSafeMoveBuffer1()
{
    if(MoveCatchArmZToSafe(MCasArmZ) &&                                         //上升到安全高度
       MOT[MCaselevatorZ].ReadPos()<=Prod.iLoadPortZ[1] &&
       Cylinder[C_LoadPortYOff].OnSensor())
       return true;
    return false;
}
//------------------------------------------------------------------------------
int iCACatchFromBufferTask=1;
HTimer CACatchFromBufferDelay;
void InitialCACatchFromBufferTask()
{
    iCACatchFromBufferTask=1;
}
//------------------------------------------------------------------------------
bool DoCACatchFromBuffer(int BufferTarget)
{
    int ret;
    int iMMCABuffer;
    int &Task=iCACatchFromBufferTask;

    switch(Task)
    {
        case 1:
            if(bCatchArmNoCassette)     //wei 20181120
            {
                bCatchArmNoCassette=false;
                Task=400;
            }
            else
            {
                Task=10;
            }
            break;
        case 10:
            if(MCasArmXCanSafeMove()==false)                                    //判斷z在上
            {
                MOT[MCasArmX].PCIL132_StopMotor();
                Task=50;
                break;
            }

            if(CatchArmXMovePos(Prod.iCassetteArmX[BufferTarget]))              //移動到buffer
            {
                Task=100;
            }
            break;
        case 50:
            if(MoveCatchArmZToSafe(MCasArmZ))                                   //上升到安全高度
            {
                Task=1;
            }
            break;
        case 100:
            if(Cylinder[C_CassetteArmCatchOff].OnSensor()==false)
            {
                C_Catch_Cassette_Pop(2, C_CassetteArmCatchOn, true);            //放開 initial
                Task=150;
            }
            else
            {
                if(BufferTarget==5)                                             //要到分離去夾 需確認撐開氣缸狀態
                {
                    C_Catch_Cassette_Pop(4, C_TrayBracketOpenOn, true);         //撐開 initial
                    Task=200;
                }
                else
                {
                    Task=250;
                }
            }
            break;
        case 150:
            if(C_Catch_Cassette_Pop(2, C_CassetteArmCatchOn))                   //放開
            {
                Task=100;
            }
            break;
        case 200:
            if(C_Catch_Cassette_Pop(4, C_TrayBracketOpenOn))                    //撐開
            {
                Task=250;
            }
            break;
        case 250:
            if(MOT[MCasArmZ].MotorMove2SpeedForMR(Prod.iCassetteArmZ[BufferTarget], &MRSpeed[CasArmZ])) //二段速下降
            {
                C_Catch_Cassette_Push(2, C_CassetteArmCatchOn, true);           //夾取 initial
                Task=300;
            }
            break;
        case 300:
            if(C_Catch_Cassette_Push(2, C_CassetteArmCatchOn))                  //夾取
            {
                Task=400;
            }
            break;
        case 400:
            #ifndef SOFT_SIMULTE
            if(LastSet.iRealDummy==DUMMY || Sen[SnCassetteArmHave].IsOn())      //判斷有沒有夾到Cassette
            #endif
            {
                iMMCABuffer=iGetMMCABuffer(BufferTarget);   //Sam 20190112 LM
                MOT[iMMCABuffer].Tray.iCassettePos[0][0]=BufferTarget;    //紀錄夾起位置
                SetCassetteTray(MCasArmZ, iMMCABuffer);

                if(USE_MR_SYSTEM==2 && BufferTarget==0)
                {
                    bBufferHasNewCassette=false;
                }

                Task=700;                                                       //有夾到Cassette
            }
            #ifndef SOFT_SIMULTE
            else
            {
                C_Catch_Cassette_Pop(2, C_CassetteArmCatchOn, true);            //放開 initial
                Task=655;                                                       //錯誤 alarm
            }
            #endif
            break;
        case 655:
            if(C_Catch_Cassette_Pop(2, C_CassetteArmCatchOn))                   //放開
            {
                Task=660;
            }
            break;
        case 660:
            if(MoveCatchArmZToSafe(MCasArmZ))
            {
                ret=ShowErrorMessage("JAM2300", K_RETRY, MCasArmZ, false, "DoCACatchFromBuffer_660");

                if(ret==K_RETRY)
                {
                    Task=100;
                }
            }
            break;
        case 700:
            if(MoveCatchArmZToSafe(MCasArmZ))                                   //上升到安全高度
            {
                Task=800;
            }
            break;
        case 800:
            #ifndef SOFT_SIMULTE
            if(LastSet.iRealDummy==DUMMY || Sen[SnCassetteArmHave].IsOn())      //上升後再檢查是否有夾到
            #endif
            {
                Task=1000;                                                      //有夾到Cassette
            }
            #ifndef SOFT_SIMULTE
            else
            {
                Task=900;                                                       //錯誤 alarm
            }
            #endif
            break;
        case 900:
            ret=ShowErrorMessage("JAM2301", K_SKIP|K_RETRY, MCasArmZ, false, "DoCACatchFromBuffer_900");

//            if(ret==K_SKIP && Sen[SnCassetteArmHave].IsOn()==false)             //skip需再偵測
//            {
//                MOT[MCasArmZ].ClearTray();
//                MOT[MMCABuffer1+BufferTarget].ClearTray();
//                Task=1000;
//            }
//            else
            if(ret==K_RETRY)
            {
                iMMCABuffer=iGetMMCABuffer(BufferTarget);//Sam 20190112 LM
                SetCassetteTray(iMMCABuffer, MCasArmZ);
                Task=100;
            }
            break;
        case 1000:
            Task=1;
            return true;
    }
    return false;
}
//------------------------------------------------------------------------------
int iCAPlaceToBufferTask=1;
HTimer DoCAPlaceToBufferDelay;
//------------------------------------------------------------------------------
void InitialCAPlaceToBufferTask()
{
    iCAPlaceToBufferTask=1;
}
//------------------------------------------------------------------------------
bool DoCAPlaceToBuffer(int BufferTarget)
{
    int ret;
    AnsiString S1="";
    bool bBufferHaveCassette=false;
    int iMMCABuffer;
    int &Task=iCAPlaceToBufferTask;

    switch(Task)
    {
        case 1:
            if(MCasArmXCanSafeMove()==false)                                    //判斷z在上
            {
                MOT[MCasArmX].PCIL132_StopMotor();
                Task=50;
                break;
            }

            #ifndef SOFT_SIMULTE
            if(LastSet.iRealDummy!=DUMMY)
            {
                if(bCatchArmNoCassette==false && MOT[MCasArmZ].fHasTray && Sen[SnCassetteArmHave].IsOn()==false)                        //cassette有沒有在
                {
                    ret=ShowErrorMessage("JAM2302", K_RETRY|K_SKIP, MCasArmZ, false, "DoCAPlaceToBuffer_1");
                    if(ret==K_RETRY)
                    {
                        Task=1;
                        break;
                    }
                    else if(ret==K_SKIP)
                    {
                        MOT[MCasArmZ].ClearTray();
                        Task=1;
                        return true;
                    }
                }
            }
            #endif
            if(bCatchArmNoCassette) //wei 20181120
            {
                bCatchArmNoCassette=false;
                Task=500;
            }
            else
            {
                Task=100;
            }
            break;
        case 50:
            if(MoveCatchArmZToSafe(MCasArmZ))                                   //上升到安全高度
            {
                Task=1;
            }
            break;
        case 100:
            if(MCasArmXCanSafeMove()==false)                                    //判斷z在上
            {
                MOT[MCasArmX].PCIL132_StopMotor();
                Task=200;
                break;
            }

            if(USE_MR_SYSTEM==1)
            {
                if(MOT[MCasArmX].ReadPos()<Prod.iCassetteArmX[2] && MCasArmXCanSafeMoveBuffer1()==false)
                {
                    MOT[MCasArmX].PCIL132_StopMotor();
                    break;
                }
            }
            if(CatchArmXMovePos(Prod.iCassetteArmX[BufferTarget]))              //移動到位置
            {
                Task=300;
            }
            break;
//        case 150:
//            if(MCasArmXCanSafeMoveBuffer1()==true)
//            {
//                Task=100;
//            }
//            break;
        case 200:
            if(MoveCatchArmZToSafe(MCasArmZ))                                   //上升到安全高度
            {
                Task=100;
            }
            break;
        case 300:
            if(MOT[MCasArmZ].MotorMove2SpeedForMR(Prod.iCassetteArmZ[BufferTarget], &MRSpeed[CasArmZ])) //二段速下降
            {
                C_Catch_Cassette_Pop(2, C_CassetteArmCatchOn, true);            //放開 initial
                Task=400;
            }
            break;
        case 400:
            if(C_Catch_Cassette_Pop(2, C_CassetteArmCatchOn))                   //放開cassette
            {
                Task=500;
            }
            break;
        case 500:
            #ifndef SOFT_SIMULTE
            if(BufferTarget==0)
            {
                bBufferHaveCassette=Sen[SnLoadRobotHaveCassette1].IsOn() && Sen[SnLoadRobotHaveCassette2].IsOn();
            }
            else if(BufferTarget==9)
            {
                bBufferHaveCassette=Sen[SnUnloadRobotHaveCassette1].IsOn() && Sen[SnUnloadRobotHaveCassette2].IsOn();
            }
            else
            {
                bBufferHaveCassette=Sen[SnBuffer1HaveCassette+BufferTarget].IsOn();
            }
            if(LastSet.iRealDummy==DUMMY || bBufferHaveCassette)//判斷buffer區 seneor有沒有亮
            #endif
            {

                if(BufferTarget==5)                                             //如果放到buffer5
                {
                    bBuffer6NeedCassette=false;
                }
                if(MOT[MCasArmZ].Tray.iCassettePos[0][0]==5)                    //如果從buffer5夾走
                {
                    bBuffer6NoNeedCassette=false;
                }
                if(USE_MR_SYSTEM==1 && MOT[MCasArmZ].Tray.iCassettePos[0][0]==0)                    //如果從buffer1夾走
                {
                    bBufferHasNewCassette=false;
                }
                iMMCABuffer=iGetMMCABuffer(BufferTarget);
                SetCassetteTray(iMMCABuffer, MCasArmZ);
                if(BufferTarget==9)
                {
                    bUnloadRobotHasCassette=true;
                }
                Task=600;
            }
            #ifndef SOFT_SIMULTE
            else
            {
                Task=650;                                                       //錯誤 alarm
            }
            #endif
            break;
        case 600:
            if(MoveCatchArmZToSafe(MCasArmZ))                                   //上升到安全高度
            {
                Task=1;
                return true;
            }
            break;
        case 650:  //JAM2310 JAM2311 JAM2312 JAM2313 JAM2314 JAM2315 JAM2316 JAM2317 JAM2318 JAM2319                                                              //alarm
            if(MoveCatchArmZToSafe(MCasArmZ))
            {
                S1.sprintf("JAM23%02d", 10+BufferTarget);
                ret=ShowErrorMessage(S1, K_RETRY, MMCABuffer1+BufferTarget, false, "DoCAPlaceToBuffer_650");

    //            if(ret==K_SKIP)
    //            {
    //                MOT[MCasArmZ].ClearTray();
    //                MOT[MMCABuffer1+BufferTarget].ClearTray();
    //                bBuffer6NoNeedCassette=false;
    //                bBufferHasNewCassette=false;
    //                bBuffer6NeedCassette=false;
    //                Task=600;
    //            }
    //            else
                if(ret==K_RETRY)
                {
                    Task=500;
                }
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
int iCatchCassetteTask=1;
void InitialCatchCassetteTask()
{
    iCatchCassetteTask=1;
}
//------------------------------------------------------------------------------
void DoCatchCassette()
{
    static int Target=0;
    int &Task=iCatchCassetteTask;
    int iMMCABuffer;
    int ret=0;
//                    bLoadNeedTray=true;
//    Task=1;
    switch(Task)
    {
        case 1:
            if(MoveCatchArmZToSafe(MCasArmZ))                                   //上升到安全高度
            {
                Task=10;
            }
            break;
        case 10:
            if(MCasArmXCanSafeMove()==false)                                    //判斷z在上
            {
                MOT[MCasArmX].PCIL132_StopMotor();
                Task=1;
                break;
            }

            if(MOT[MCasArmX].ReadPos()>=Prod.iCassetteArmX[2])                  //buffer2安全位置後
            {
                Task=50;                                                        //在安全位置
            }
            else
            {
                Task=30;                                                        //沒在安全位置
            }
            break;
        case 30:
            if(MCasArmXCanSafeMove()==false)                                    //判斷z在上
            {
                MOT[MCasArmX].PCIL132_StopMotor();
                Task=1;
                break;
            }

            if(CatchArmXMovePos(Prod.iCassetteArmX[2]))                         //buffer2安全位置
            {
                Task=10;
            }
            break;
        case 50:
            if(MOT[MCasArmZ].fHasTray && Sen[SnCassetteArmHave].IsOn())         //有cassette就去放
            {
                InitialCAPlaceToBufferTask();                                   //放
                Task=1000;
            }
            else if(MOT[MCasArmZ].fHasTray && Sen[SnCassetteArmHave].IsOn()==false) //wei 20181120
            {
                InitialCAPlaceToBufferTask();                                   //放
                bCatchArmNoCassette=true;
                Task=1000;
            }
            else if(MOT[MCasArmZ].fHasTray==false && Sen[SnCassetteArmHave].IsOn())
            {
                InitialCACatchFromBufferTask();                                 //夾
                bCatchArmNoCassette=true;
                Task=100;
            }
            else
            {
                InitialCACatchFromBufferTask();                                 //夾
                Task=100;
            }
            break;
        case 100:
            iMMCABuffer=iGetMMCABuffer(0);  //Sam 20190112 LM
            if(WhichBufferCanPlace()!=0 && bBufferHasNewCassette &&
               MOT[iMMCABuffer].HasIC()==true)                                  //要去buffer1夾之前先判斷有無空的位置再去夾
            {
                Target=0;
                Task=200;
            }
            else if(MOT[MMCABuffer6].HasIC()==false &&
                    (bLoadNeedTray || bEmptyNeedTray || bLoadNeedFailTray))     //判斷Buffer6是否為空的 並要Device or Empty cassette到Buffer6
            {
                if(bLoadNeedTray)
                {
                    if(bSearchNowLotID)
                    {
                        Target=WhichBufferCanCatch(HAS_CASSETTE_DEVICE, 1);
                    }
                    else if(bSearchNextLotID)
                    {
                        Target=WhichBufferCanCatch(HAS_CASSETTE_DEVICE, 2);
                    }
                    else
                    {
                        Target=WhichBufferCanCatch(HAS_CASSETTE_DEVICE, 3);
                    }
                }
//                else if(bLoadNeedTray && bNewLotHotplate)
//                    Target=WhichBufferCanCatch(HAS_CASSETTE_DEVICE, 2);

                if(bLoadNeedTray==false || WhichBufferCanCatch(HAS_CASSETTE_DEVICE)==0)
                {
                    if(bEmptyNeedTray)
                        Target=WhichBufferCanCatch(HAS_CASSETTE_TRAY);
                }
//                else if(bLoadNeedFailTray)
//                    Target=WhichBufferCanCatch(HAS_CASSETTE_FAIL);
                if(Target!=0)                                                   //如果沒有Device or Empty cassette就重新判斷
                {
                    Task=200;
                }
            }
            else if(MOT[MMCABuffer6].HasIC()==true && bBuffer6NoNeedCassette)  //buffer6不需要cassette 要判斷是否有存在
            {
                Target=5;
                Task=200;
            }
            else if(bBuffer6NeedCassette)                                       //buffer6需要cassette
            {
                Target=WhichBufferCanCatch(HAS_CASSETTE_EMPTY);                 //要判斷是否有存在
//                if(Target==5)
//                {
//                    Target=WhichBufferCanPlace();
//                }
//                else
                if(Target==0)                                                   //QQ待確認
                {
                    Task=100;
                }
                else
                {
                    Task=200;
                }
            }
            else if(bMRAutoMode==false &&
                    MOT[MMCABuffer1].HasIC()==false && MOT[MMLoadPort].HasIC()==false &&
                    iTrayFeed!=1 && bBufferHasOldCassette==false && //判斷buffer1為空 && loadport為空 有pass的cassette先出料
                    (WhichBufferCanCatch(HAS_CASSETTE_PASS)!=0 || WhichBufferCanCatch(HAS_CASSETTE_FAIL)!=0))
            {
                if(WhichBufferCanCatch(HAS_CASSETTE_PASS)!=0)
                    Target=WhichBufferCanCatch(HAS_CASSETTE_PASS);                  //要判斷是否有存在
                else
                    Target=WhichBufferCanCatch(HAS_CASSETTE_FAIL);
                Task=200;
            }
            else if(bMRAutoMode && bMRAutoCassetteOut &&
                    MOT[MMCABuffer1].HasIC()==false && MOT[MMLoadPort].HasIC()==false)
            {
                Target=iMRAutoCassette;
                fLotInfo->labSecsGemStatus->Caption="";
                Task=200;
            }
            break;
        case 200:
            if(CheckHasCassetteSensor(Target) || bCatchArmNoCassette)   //wei 20181120
            {
                Task=250;
            }
            else
            {
                ret=ShowErrorMessage("JAM2332", K_RETRY|K_SKIP, MMLoadPort, false, "DoCatchCassette_200");

                if(ret==K_RETRY)
                {
                    Task=200;
                }
                else
                {
                    MOT[MMCABuffer1+Target].ClearTray();
                    Task=1;
                }
            }
            break;
        case 250:
            if(Target<2)                                                        //如果要移動位置小於buffer2就判斷loadportZ位置
            {
                Task=300;
            }
            else
            {
                Task=400;
            }
            break;
        case 300:
            if(MCasArmXCanSafeMoveBuffer1()==true)                              //判斷z在上 && loadport Z等待位置
            {
                Task=400;
            }
            break;
        case 400:
            if(DoCACatchFromBuffer(Target))                                     //夾
            {
                Task=500;
            }
            break;
        case 500:
            if(MOT[MCasArmZ].fHasTray &&
               ((LastSet.iRealDummy!=DUMMY && Sen[SnCassetteArmHave].IsOn()) ||
                 LastSet.iRealDummy==DUMMY))
            {
                InitialCAPlaceToBufferTask();
                Task=1000;                                                      //有cassette就去放
            }
            else
            {
                Task=1;                                                         //沒有cassette就重新  //QQ待確認
            }
            break;
        case 1000:
            if(iTrayFeed==0 && iCleanOut==0)
            {
                if(MOT[MMTrayLoader].Tray.HasIC()==false && MOT[MCasArmZ].Tray.Data[0][0]==HAS_CASSETTE_DEVICE && bLoadNeedTray==false &&
                   MOT[MMCABuffer6].HasIC()==false &&
                   (LastSet.iRealDummy==DUMMY ||
                    (LastSet.iRealDummy!=DUMMY && Sen[SnLoaderTrayHasTray].IsOn()==false && Sen[SnLoaderCarHasTray].IsOn()==false)))
                {
                    bLoadNeedTray=true;
                }

                if(MOT[MMTrayEmpty].Tray.HasIC()==false && MOT[MCasArmZ].Tray.Data[0][0]==HAS_CASSETTE_TRAY && bEmptyNeedTray==false &&
                   MOT[MMCABuffer6].HasIC()==false &&
                   ((LastSet.iRealDummy==DUMMY && iMRTrayCount[1]==0) ||
                    (LastSet.iRealDummy!=DUMMY && Sen[SenEmptySelectHasTray].IsOn()==false)))
                {
                    bEmptyNeedTray=true;
                }
            }

            if(bLoadNeedTray && MOT[MCasArmZ].Tray.Data[0][0]==HAS_CASSETTE_DEVICE &&
               MOT[MMCABuffer6].HasIC()==false && Sen[SnLoaderTrayHasTray].IsOn()==false)
            {
                if(bSearchNowLotID)
                {
                    if(MOT[MCasArmZ].Tray.cCassetteLot==asNowLotID)
                    {
                        Target=5;
                        bSearchNowLotID=false;
                    }
                    else
                    {
                        Target=WhichBufferCanPlace();
                    }
                }
                else if(bSearchNextLotID)
                {
                    if(MOT[MCasArmZ].Tray.cCassetteLot==asNextLotID)
                    {
                        Target=5;
                        bSearchNextLotID=false;
                    }
                    else
                    {
                        Target=WhichBufferCanPlace();
                    }
                }
                else
                {
                    Target=5;
                }
            }
            else if(bEmptyNeedTray && MOT[MCasArmZ].Tray.Data[0][0]==HAS_CASSETTE_TRAY &&
                MOT[MMCABuffer6].HasIC()==false)
            {
                 if(Sen[SenEmptySelectHasTray].IsOn()==false)
                 {
                    Target=5;
                 }
                 else
                 {
                    bEmptyNeedTray=false;
                    Target=WhichBufferCanPlace();                                   //判斷哪個有空的buffer
                 }
            }
            else if(bMRAutoMode==false &&
                    (MOT[MCasArmZ].Tray.Data[0][0]==HAS_CASSETTE_PASS ||        //20181011
                     MOT[MCasArmZ].Tray.Data[0][0]==HAS_CASSETTE_FAIL)&&
                     MOT[MMCABuffer1].HasIC()==false && MOT[MMLoadPort].HasIC()==false &&
                     (iTrayFeed!=1  || (iTrayFeed==1 && WhichBufferCanCatch(HAS_CASSETTE_DEVICE)==0 && bNewLotHotplate==false && bNewLotLoader==false)))
            {
                 Target=0;
                 bBufferHasOldCassette=true;
            }
            else if(bBuffer6NoNeedCassette &&
                    (MOT[MCasArmZ].Tray.Data[0][0]==HAS_CASSETTE_EMPTY ||
                     MOT[MCasArmZ].Tray.Data[0][0]==HAS_CASSETTE_PASS  ||
                     MOT[MCasArmZ].Tray.Data[0][0]==HAS_CASSETTE_FAIL  ) && MOT[MCasArmZ].Tray.iCassettePos[0][0]!=0)
                Target=WhichBufferCanPlace();
            else if(bBuffer6NeedCassette && MOT[MCasArmZ].Tray.Data[0][0]==HAS_CASSETTE_EMPTY && MOT[MMCABuffer6].HasIC()==false)
                Target=5;
            else if(bMRAutoMode && bMRAutoCassetteOut &&
                    MOT[MMCABuffer1].HasIC()==false && MOT[MMLoadPort].HasIC()==false)
            {
                 Target=0;
                 bBufferHasOldCassette=true;
            }
            else
                Target=WhichBufferCanPlace();                                   //判斷哪個有空的buffer
            Task=1100;
            break;
        case 1100:
            if(CheckNoCassetteSensor(Target) || bCatchArmNoCassette)    //wei 20181120
            {
                Task=1150;
            }
            else
            {
                ret=ShowErrorMessage("JAM2333", K_RETRY|K_SKIP, MMLoadPort, false, "DoCatchCassette_1100");

                if(ret==K_RETRY)
                {
                    Task=1100;
                }
//                else
//                {
//                    MOT[MMCABuffer1+Target].ClearTray();
//                    Task=1;
//                }
//                Task=1000;
            }
            break;
        case 1150:
            if(Target==0 && bBufferHasOldCassette==false)                                                       //如果沒有空的就去等待區域
            {
                Task=1200;
            }
            else if(DoCAPlaceToBuffer(Target))                                  //放
            {
                if(iTrayFeed==0 && iCleanOut==0)
                {
                    if(MOT[MMTrayLoader].Tray.HasIC()==false && MOT[MMTrayConversion].Tray.Data[0][0]!=HAS_CASSETTE_DEVICE &&
                       MOT[MStackedTrayZ].Tray.Data[0][0]!=HAS_CASSETTE_DEVICE &&
                       WhichBufferCanCatch(HAS_CASSETTE_DEVICE)!=0 && bLoadNeedTray==false &&
                       (LastSet.iRealDummy==DUMMY ||
                        (LastSet.iRealDummy!=DUMMY && Sen[SnLoaderTrayHasTray].IsOn()==false)))
                    {
                        bLoadNeedTray=true;
                    }

                    if(MOT[MMTrayEmpty].Tray.HasIC()==false && MOT[MMTrayConversion].Tray.Data[0][0]!=HAS_CASSETTE_TRAY &&
                       MOT[MStackedTrayZ].Tray.Data[0][0]!=HAS_CASSETTE_TRAY &&
                       WhichBufferCanCatch(HAS_CASSETTE_TRAY)!=0 && bEmptyNeedTray==false &&
                       ((LastSet.iRealDummy==DUMMY && iMRTrayCount[1]==0) ||
                        (LastSet.iRealDummy!=DUMMY && Sen[SenEmptySelectHasTray].IsOn()==false)))
                    {
                        bEmptyNeedTray=true;
                    }
                }

                if(bLackCassette)   //wei 20181120
                    bLackCassette=false;

                if(bMRAutoMode && bMRAutoCassetteOut)
                    bMRAutoCassetteOut=false;

                if(Target==5)
                    Task=1190;
                else
                    Task=1200;
            }
            break;
        case 1190:
            if(MOT[MMCABuffer6].Tray.Data[0][0]==HAS_CASSETTE_TRAY || MOT[MMCABuffer6].Tray.Data[0][0]==HAS_CASSETTE_DEVICE)
            {
                #ifndef SOFT_SIMULTE
                if(Sen[SnBuffer6HaveTray].IsOn()==false)
                {
                    ret=ShowErrorMessage("JAM2343", K_RETRY|K_SKIP, MCasArmZ, false, "DoCatchCassette_1190");
                    if(ret==K_RETRY)
                    {
                        bCheckCassetteFinish=false;
                        Task=1190;
                    }
                    else
                    {
                        if(MOT[MMCABuffer6].Tray.Data[0][0]==HAS_CASSETTE_DEVICE)
                            bLoadNeedTray=false;
                        else if(MOT[MMCABuffer6].Tray.Data[0][0]==HAS_CASSETTE_TRAY)
                            bEmptyNeedTray=false;

                        MOT[MMCABuffer6].SetTray(HAS_CASSETTE_EMPTY);
                        MOT[MMCABuffer6].Tray.cCassetteLot="";
                        bBuffer6NoNeedCassette=true;
                        bCheckCassetteFinish=false;
                        Task=1200;
                    }
                }
                else
                #endif
                {
                    bCheckCassetteFinish=true;
                    Task=1200;
                }
            }
            else if(MOT[MMCABuffer6].Tray.Data[0][0]==HAS_CASSETTE_EMPTY)
            {
                if(Sen[SnBuffer6HaveTray].IsOn())
                {
                    ret=ShowErrorMessage("JAM2344", K_RETRY|K_SKIP, MCasArmZ, false, "DoCatchCassette_1190");
                    if(ret==K_RETRY)
                    {
                        bCheckCassetteFinish=false;
                        Task=1190;
                    }
                    else
                    {
                        bCheckCassetteFinish=false;
                        Task=1300;
                    }
                }
                else
                {
                    bCheckCassetteFinish=true;
                    Task=1200;
                }
            }
            break;
        case 1200:
            if(MCasArmXCanSafeMove()==false)                                    //判斷z在上
            {
                MOT[MCasArmX].PCIL132_StopMotor();
                Task=1250;
            }
            else
            {
                Task=10;
            }
            break;
        case 1250:
            if(MoveCatchArmZToSafe(MCasArmZ))                                   //上升到安全高度
            {
                Task=1200;
            }
            break;
        case 1300:
            if(MCasArmXCanSafeMove()==false)                                    //判斷z在上
            {
                MOT[MCasArmX].PCIL132_StopMotor();
                Task=1350;
            }
            else
            {
                ShowMyMessage("Please remove Buffer6 cassette!", "請移除Buffer6卡匣!");
                Task=1400;
            }
            break;
        case 1350:
            if(MoveCatchArmZToSafe(MCasArmZ))                                   //上升到安全高度
            {
                Task=1300;
            }
            break;
        case 1400:
            if(Sen[SnBuffer6HaveCassette].IsOn())
            {
                ShowMyMessage("Please remove Buffer6 cassette!", "請移除Buffer6卡匣!");
            }
            else
            {
                MOT[MMCABuffer6].SetTray(NULL_IC);
                MOT[MMCABuffer6].Tray.cCassetteLot="";
                bBuffer6NeedCassette=true;
                Task=1200;
            }
            break;
    }
}
//------------------------------------------------------------------------------
//Sam 20190112 LM
//==>

//<==
//Sam 20190112 LM
//------------------------------------------------------------------------------
bool C_Catch_Cassette_Push(int Pos, int CylinderPos, bool bInitial)
{
    //Pos 1 C_LoadPortCatch  2 C_LoadPortY 3 C_CassetteArmCatch
    //    4 C_TrayBracketUp  5 C_TrayBracketOpen 6 C_StackedTrayCatch 7 C_StackedTrayLock
    //

    //Sam 20190112 LM
    //==>
    Pos=-1;
    int ipos[]={C_LoadPortCatchOn,      C_LoadPortYOn,          C_CassetteArmCatchOn,
                C_CassetteArmCatchOn,   C_TrayBracketUpOn,      C_TrayBracketOpenOn,
                C_StackedTrayCatchOn,   C_StackedTrayLockOn,    };
    for(int i=0; i<10; i++)
    {
        if(CylinderPos==ipos[i])
        {
            Pos=i;
            break;
        }
    }

    if(Pos==-1)
    {
        return false;
    }
    //<==
    //Sam 20190112 LM

    int &iTask=iCatch_Cassette_Push[Pos];
    static bool bflag[10]={false};
    if(bInitial)
    {
        bflag[Pos]=false;
        iTask=1;
        return true;
    }

    switch(iTask)
    {
        case 1:
            Cylinder[CylinderPos].Reset();
            Cylinder[CylinderPos].Off();
            Cylinder[CylinderPos+1].Off();
            bflag[Pos]=false;
            iTask=100;
        case 100:
            if(bflag[Pos]==false)
                bflag[Pos]=Cylinder[CylinderPos].Push();

            if(bflag[Pos])
                return true;

            break;
    }

    return false;
}
//------------------------------------------------------------------------------
bool C_Catch_Cassette_Pop(int Pos, int CylinderPos, bool bInitial)
{
    //Pos 1 C_LoadPortCatch  2 C_LoadPortY 3 C_CassetteArmCatch
    //    4 C_TrayBracketUp  5 C_TrayBracketOpen 6 C_StackedTrayCatch 7 C_StackedTrayLock
    //
    int &iTask=iCatch_Cassette_Pop[Pos];
    static bool bflag[10]={false};

    //Sam 20190112 LM
    //==>
    Pos=-1;
    int ipos[]={C_LoadPortCatchOn,      C_LoadPortYOn,          C_CassetteArmCatchOn,
                C_CassetteArmCatchOn,   C_TrayBracketUpOn,      C_TrayBracketOpenOn,
                C_StackedTrayCatchOn,   C_StackedTrayLockOn};
    for(int i=0; i<10; i++)
    {
        if(CylinderPos==ipos[i])
        {
            Pos=i;
            break;
        }
    }

    if(Pos==-1)
    {
        return false;
    }
    //<==
    //Sam 20190112 LM

    if(bInitial)
    {
        bflag[Pos]=false;
        iTask=1;
        return true;
    }

    switch(iTask)
    {
        case 1:
            Cylinder[CylinderPos].Off();
            Cylinder[CylinderPos+1].Off();
            Cylinder[CylinderPos+1].Reset();
            bflag[Pos]=false;
            iTask=100;
        case 100:
            if(bflag[Pos]==false)
                bflag[Pos]=Cylinder[CylinderPos+1].Push();

            if(bflag[Pos])
                return true;

            break;
    }

    return false;
}
//------------------------------------------------------------------------------
//Sam 20190112 LM
//==>
void DoLoadCassette()
{
    if(USE_MR_SYSTEM==1)
    {
        DoLoadPortCassette();
    }
    else
    {
        DoLoadRobotCassette();
        DoUnloadRobotCassette();
    }
}
//<==
//Sam 20190112 LM
//------------------------------------------------------------------------------
int iLoadPortCassetteTask=1;
HTimer DoRFIDRead1Delay;
HTimer DoRFIDRead2Delay;
HTimer RFIDDataDelay;
void InitialLoadPortCassetteTask()
{
    iLoadPortCassetteTask=1;
}
//------------------------------------------------------------------------------
void DoLoadPortCassette()
{
    int &Task=iLoadPortCassetteTask;
    static int iRFIDRetry=0;
    int ret=0;
    bool bCheckLotOK=false;

    bNeedDeviceCassette=true;

    switch(Task)
    {
        case 1:
            if(MElevatorCanSafeMove()==false)                                   //判斷是否可以移動
            {
                C_Catch_Cassette_Pop(1, C_LoadPortYOn, true);
                Task=50;
            }
            else
            {
                Task=100;
            }
            break;
        case 50:
            if(C_Catch_Cassette_Pop(1, C_LoadPortYOn))
            {
                Task=1;
            }
            break;
        case 100:
            if(MOT[MCaselevatorZ].MotorMove(Prod.iLoadPortZ[1]))                //移動到安全位置
            {
                Task=150;
            }
            break;
        case 150:
            if(MOT[MCaselevatorZ].fHasTray &&                                        //有cassette就去放
               ((LastSet.iRealDummy!=DUMMY && (Sen[SnLoadPortCatch].IsOn() && Cylinder[C_LoadPortCatchOn].OnSensor()==true)) ||
                 LastSet.iRealDummy==DUMMY))
            {
                if(bBufferHasOldCassette==false)
                {
                    InitialLoadPortElevatorUpTask();
                    Task=1500;                                                  //up
                }
                else
                {
                    InitialLoadPortElevatorDownTask();
                    Task=2000;                                                  //down
                }
            }
            else if(MOT[MCaselevatorZ].fHasTray && Sen[SnLoadPortCatch].IsOn()==false && Cylinder[C_LoadPortCatchOn].OnSensor()==false) //wei 20181120
            {
                bLoadPortNoCassette=true;
                if(bBufferHasOldCassette==false)
                {
                    InitialLoadPortElevatorUpTask();
                    Task=1500;                                                  //up
                }
                else
                {
                    InitialLoadPortElevatorDownTask();
                    Task=2000;                                                  //down
                }
            }
//            else if(MOT[MCaselevatorZ].fHasTray==true && LastSet.iRealDummy!=DUMMY &&
//                    Sen[SnLoadPortCatch].IsOn()==false && Cylinder[C_LoadPortCatchOn].OnSensor()==false)
//            {
//                Task=160;                                                       //cassette遺失
//            }
            else
            {
                Task=200;                                                       //夾
            }
            break;
        case 160:
            //alarm


            break;
        case 200:
            if(bManualCatchCassetteUp[0]==true && bManualCatchCassetteUp[1]==true)
            {
                break;
            }

            if(LastSet.iRealDummy==DUMMY)
            {                                 //MOT[MMCABuffer1].HasIC()==false &&
                if(MOT[MMLoadPort].HasIC() && MOT[MMCABuffer1].HasIC()==false && //iTrayFeed==0 &&
                   bBufferHasOldCassette==false)
                {
                    if(bRFIDReadFinish==false)
                    {
                        if(IniConfig.bEnable_SECS_GEM==true)
                            EventReport(SECS_EVENT.CassetteLoadComplete);               //wei 20181018
                    }

                    if(TestIF_File.bEnableRFID)
                    {
                        if(bMRAutoMode==true && bRFIDReadFinish)
                        {
                            InitialLoadPortElevatorUpTask();
                            bRFIDReadFinish=false;
                            Task=1500;
                        }
                        else
                        {
                            fRFID->bRFIDReaderOK[0]=false;
                            iRFIDRetry=0;
                            fLotInfo->labSecsGemStatus->Caption="Read RFID...";
                            RecordProcess("Read RFID...");
                            Task=1000;
                        }
                    }
                    else
                    {
                        Task=1100;

                    }
                }
                else if(MOT[MMCABuffer1].HasIC() &&   //MOT[MMCABuffer1].HasIC()  &&
                        MOT[MMLoadPort].HasIC()==false &&
                        (bBufferHasOldCassette))           // || iTrayFeed==1
                {
                    InitialLoadPortElevatorDownTask();
                    Task=2000;
                }
                else if(iTrayFeed==1)
                {
                    if(bMRAutoMode==false && WhichBufferCanCatch(HAS_CASSETTE_DEVICE)==0 && bNewLotHotplate==false && bNewLotLoader==false)
                    {
                        if(MOT[MMCABuffer1].HasIC()==false  &&
                           MOT[MMLoadPort].HasIC()==false  &&
                           WhichBufferCanCatch(HAS_CASSETTE_FAIL)==0 &&    //20181011
                           WhichBufferCanCatch(HAS_CASSETTE_PASS)==0 &&    //20181011
                           MOT[MCasArmZ].Tray.Data[0][0]==NULL_IC &&
                           WhichAutoCanCatch()==0)
                         {
                            bTrayCassetteTrayFeed=true;
                         }
                    }
                    else
                    {
                        if(MOT[MMCABuffer1].HasIC()==false  &&
                           MOT[MMLoadPort].HasIC()==false  &&
                           MOT[MCasArmZ].Tray.Data[0][0]==NULL_IC &&
                           WhichAutoCanCatch()==0)
                         {
                            bTrayCassetteTrayFeed=true;
                         }
                    }
                }
            }
            else
            {
                if(Sen[SnLoadPortPresent].IsOn() && Sen[SnLoadPortPlacement1].IsOn() &&
                   Sen[SnLoadPortPlacement2].IsOn() && Sen[SnBuffer1HaveCassette].IsOn()==false &&
                   MOT[MMCABuffer1].HasIC()==false && bBufferHasOldCassette)
                {
                    ShowMyMessage("Cassette unloader to load port, please remove cassette!", "請移除Load port的cassette!");
                }
                else if(Sen[SnLoadPortPresent].IsOn() && Sen[SnLoadPortPlacement1].IsOn() &&
                        Sen[SnLoadPortPlacement2].IsOn() && Sen[SnBuffer1HaveCassette].IsOn()==false &&
                        MOT[MMCABuffer1].HasIC()==false && bBufferHasOldCassette==false)
                {
                    if(Sen[SnSafeDoor11].Enable==false || (Sen[SnSafeDoor11].Enable==true && Sen[SnSafeDoor11].IsOn()==true))
                    {
                        if(bMRAutoMode==false)
                        {
                            if(IniConfig.bEnable_SECS_GEM==true)
                                EventReport(SECS_EVENT.CassetteLoadComplete);               //wei 20181018
                        }

                        if(TestIF_File.bEnableRFID)
                        {
                            if(bMRAutoMode==true && bRFIDReadFinish)
                            {
                                InitialLoadPortElevatorUpTask();
                                bRFIDReadFinish=false;
                                Task=1500;
                            }
                            else
                            {
                                fRFID->bRFIDReaderOK[0]=false;
                                iRFIDRetry=0;
                                #ifndef SOFT_SIMULTE
                                fRFID->RFID_1->StopComm();
                                fRFID->RFID_1->StartComm();
                                #endif
                                Task=1000;
                            }
                        }
                        else
                        {
                            Task=1100;
                        }
                    }
                }
                else if(Sen[SnLoadPortPresent].IsOn()==false && Sen[SnLoadPortPlacement1].IsOn()==false&&
                        Sen[SnLoadPortPlacement2].IsOn()==false && Sen[SnBuffer1HaveCassette].IsOn() &&
                        MOT[MMLoadPort].HasIC()==false && MOT[MMCABuffer1].HasIC() && (bBufferHasOldCassette))
                {
                    InitialLoadPortElevatorDownTask();
                    Task=2000;
                }
                else if(iTrayFeed==1)
                {
                    if(bMRAutoMode==false && bNewLotHotplate==false && bNewLotLoader==false)  //WhichBufferCanCatch(HAS_CASSETTE_DEVICE)==0 &&
                    {
                        if(MOT[MMCABuffer1].HasIC()==false &&
                           MOT[MMLoadPort].HasIC()==false  &&
                           WhichBufferCanCatch(HAS_CASSETTE_FAIL)==0 &&        //20181011
                           WhichBufferCanCatch(HAS_CASSETTE_PASS)==0 &&        //20181011
                           MOT[MCasArmZ].Tray.Data[0][0]==NULL_IC &&
                           WhichAutoCanCatch()==0)
                        {
                            bTrayCassetteTrayFeed=true;
                        }
                    }
                    else
                    {
                        if(MOT[MMCABuffer1].HasIC()==false &&
                           MOT[MMLoadPort].HasIC()==false  &&
                           MOT[MCasArmZ].Tray.Data[0][0]==NULL_IC &&
                           WhichAutoCanCatch()==0)
                        {
                            bTrayCassetteTrayFeed=true;
                        }
                    }
                }
            }
            break;
        case 1000:
            fRFID->DoRFIDReader(0);
            DoRFIDRead1Delay.SetMSAndOn(TestIF_File.iRFIDDelay);
            Task=1050;
            break;
        case 1050:
            if(fRFID->bRFIDReaderOK[0])
            {
                InitialLoadPortElevatorUpTask();
                iRFIDRetry=0;
                if(IniConfig.bEnable_SECS_GEM==true)
                {
                    EventReport(SECS_EVENT.CassetteIDReadComplete);      //wei 20181018
                    if(bMRAutoMode)
                    {
                        fLotInfo->labSecsGemStatus->Caption="RFID read finish,wait EA(Cassette Data)...";
                        RecordProcess("RFID read finish,wait EA(Cassette Data)...");
                        RFIDDataDelay.SetSecAndOn(20.0);
                        bLotDataOK=false;
                        Task=1060;
                    }
                    else
                    {
                        Task=1100;
                    }
                }
                else
                {
                    Task=1100;
                }
            }
            else if(fRFID->cbRFIDSimulate->Checked)
            {
                AnsiString str="";
                str.sprintf("%d",(rand()%1000)+1000);
                MOT[MMLoadPort].Tray.cCassetteID=str;
                asLoadPortCassetteID=str;
                if(IniConfig.bEnable_SECS_GEM==true)
                {
                    EventReport(SECS_EVENT.CassetteIDReadComplete);      //wei 20181018
                    if(bMRAutoMode)
                    {
                        fLotInfo->labSecsGemStatus->Caption="RFID read finish,wait EA(Cassette Data)...";
                        RecordProcess("RFID read finish,wait EA(Cassette Data)...");
                        RFIDDataDelay.SetSecAndOn(20.0);
                        bLotDataOK=false;
                        Task=1060;
                    }
                    else
                    {
                        Task=1100;
                    }
                }
                else
                {
                    Task=1100;
                }
                iRFIDRetry=0;
            }
            else if(DoRFIDRead1Delay.Off())
            {
                if(iRFIDRetry<TestIF_File.iRFIDRetryCount)
                {
                    iRFIDRetry++;
                    Task=1000;
                }
                else
                {
                    ret=ShowErrorMessage("JAM2340", K_RETRY|K_SKIP, MMLoadPort, false, "DoLoadPortCassette_1050");
                    if(ret==K_SKIP)
                    {
                        Task=1100;
                    }
                    else
                    {
                        Task=1000;
                        #ifndef SOFT_SIMULTE
                        fRFID->RFID_1->StopComm();
                        fRFID->RFID_1->StartComm();
                        #endif
                    }
                    iRFIDRetry=0;
                }
            }
            break;
        case 1060:
            if(bSECSGEMConnectionFail==false && bLotDataOK)
            {
                InitialLoadPortElevatorUpTask();
                if(asLoadPortCassetteData=="DEVICE")
                {
                    MOT[MMLoadPort].SetTray(HAS_CASSETTE_DEVICE);
                }
                else if(asLoadPortCassetteData=="EMPTYTRAY")
                {
                    MOT[MMLoadPort].SetTray(HAS_CASSETTE_TRAY);
                }
                else if(asLoadPortCassetteData=="EMPTYCASSETTE")
                {
                    MOT[MMLoadPort].SetTray(HAS_CASSETTE_EMPTY);
                }
                MOT[MMLoadPort].Tray.cCassetteLot=asLoadPortCassetteLotID;
                fLotInfo->labSecsGemStatus->Caption="";
                Task=1500;
            }
            else if(RFIDDataDelay.Off())
            {
                ret=ShowErrorMessage("JAM2342", K_RETRY, MMLoadPort, false, "DoLoadPortCassette_1060");
                if(ret==K_RETRY)
                {
                    fRFID->bRFIDReaderOK[0]=false;
                    iRFIDRetry=0;
                    #ifndef SOFT_SIMULTE
                    fRFID->RFID_1->StopComm();
                    fRFID->RFID_1->StartComm();
                    #endif
                    Task=1000;
                }
            }
            else if(bSECSGEMConnectionFail)
            {
                Task=1100;
            }
            break;
        case 1100:
            InitialLoadPortElevatorUpTask();
            bLoadPortHasCassette=true;
            bCassetteMessage=true;
            bCheckLotOK=false;
            if(fCassette->cbChooseCassetteMode->Checked || (bManualCatchCassetteUp[0]==true && bManualCatchCassetteUp[1]==false))
            {
                ret=ShowMyMessageBox_YES_NO("確認Cassette資料?", "1:Device    2:Empty Tray    3:Empty Cassette");
                if(ret==1)
                {
                    if(MyMessageBox->rgCassetteStatus->ItemIndex==0)
                    {
                        MOT[MMLoadPort].SetTray(HAS_CASSETTE_DEVICE);
                        MOT[MMLoadPort].Tray.cCassetteLot=MyMessageBox->edCassetteLotID->Text;
                        asSameLotID=MyMessageBox->edCassetteLotID->Text;

                        for(int i=0; i<4; i++)
                        {
                            if(asSameLotID==asMRLotData[0][i])
                            {
                                bCheckLotOK=true;
                            }
                        }
//                        ShowCassetteData(0, asSameLotID);

                        if(bCheckLotOK==false)
                        {
                            iLotCount=0;
                            iLotCount=ShowMyInput("請輸入 Lot  數量" , "請輸入數字 : ");
                        }
                    }
                    else if(MyMessageBox->rgCassetteStatus->ItemIndex==1)
                    {
                        MOT[MMLoadPort].SetTray(HAS_CASSETTE_TRAY);
                    }
                    else if(MyMessageBox->rgCassetteStatus->ItemIndex==2)
                    {
                        MOT[MMLoadPort].SetTray(HAS_CASSETTE_EMPTY);
                    }
                    bLoadPortHasCassette=false;
                    bCassetteMessage=false;

                    Task=1500;
                }
                else if(ret==2)
                {
                    bLoadPortHasCassette=false;
                    Task=1200;
                }
            }
            break;
        case 1200:
            if(Sen[SnLoadPortPresent].IsOn() || Sen[SnLoadPortPlacement1].IsOn() ||
               Sen[SnLoadPortPlacement2].IsOn())
            {
                bCassetteMessage=true;
                ShowCassetteMessage("Please remove cassette from load port!!","請移除load port上的Cassette!!");
            }
            else
            {
                MyMessageBox->Close();
                Task=200;
            }
            break;
        case 1500:
            if(DoLoadPortElevatorUp())
            {
                if(IniConfig.bEnable_SECS_GEM==true)
                    EventReport(SECS_EVENT.ReadyToProcessComplete);      //wei 20181018
                if(bManualCatchCassetteUp[0]==true)
                    bManualCatchCassetteUp[1]=true;

                if(MyMessageBox->rgCassetteStatus->ItemIndex==0)
                {
                    ShowCassetteData(0, asSameLotID);
                    if(bCheckLotOK==false)
                    {
                        asMRLotData[2][iMRLotCount]=iLotCount;
                    }
                }
                Task=1;
            }
            break;
        case 2000:
            if(DoLoadPortElevatorDown())
            {
                bCassetteMessage=true;
                ShowCassetteMessage("Please remove cassette from load port!!","請移除load port上的Cassette!!");
                bNeedDeviceCassette=true;
                Task=2100;
            }
            break;
        case 2100:
            if(LastSet.iRealDummy==DUMMY)
            {
                if(IniConfig.bEnable_SECS_GEM==true)
                    EventReport(SECS_EVENT.UnloadComplete);      //wei 20181018
                bBufferHasOldCassette=false;
                bCassetteMessage=false;
                bNeedDeviceCassette=false;
                MOT[MMLoadPort].SetTray(NULL_IC);
                if(bManualCatchCassetteDown[0]==true)
                {
                    bManualCatchCassetteDown[2]=true;
                    bManualCatchCassetteDown[0]=false;
                    fMain->Pause();
                }
                Task=1;
            }
            else
            {
                if(Sen[SnLoadPortPresent].IsOn() || Sen[SnLoadPortPlacement1].IsOn() ||
                   Sen[SnLoadPortPlacement2].IsOn())
                {
                    if(IniConfig.bEnable_SECS_GEM==true)
                        EventReport(SECS_EVENT.ReadyToUnload);      //wei 20181018
                    bCassetteMessage=true;
                    bNeedDeviceCassette=true;
                    if(bManualCatchCassetteDown[0]==true)
                        ShowCassetteMessage("Please remove cassette from load port","請移除load port上的Cassette!!");
                }
                else
                {
                    if(Sen[SnSafeDoor11].Enable==false || (Sen[SnSafeDoor11].Enable==true && Sen[SnSafeDoor11].IsOn()==true))
                    {
                        if(IniConfig.bEnable_SECS_GEM==true)
                            EventReport(SECS_EVENT.UnloadComplete);      //wei 20181018
                        if(MOT[MMCABuffer1].HasIC()==false)
                            bBufferHasOldCassette=false;
                        bCassetteMessage=false;
                        MOT[MMLoadPort].SetTray(NULL_IC);
                        bNeedDeviceCassette=false;
                        MyMessageBox->Close();
                        if(bManualCatchCassetteDown[0]==true)
                        {
                            bManualCatchCassetteDown[2]=true;
                            bManualCatchCassetteDown[0]=false;
                            fMain->Pause();
                        }
                        Task=1;
                    }
                }
            }
            break;
    }
}
//------------------------------------------------------------------------------
//Sam 20190112 LM
//==>
int iLoadRobotCassetteTask=1;
void InitialLoadRobotCassetteTask()
{
    iLoadRobotCassetteTask=1;
}
//------------------------------------------------------------------------------
void DoLoadRobotCassette()
{
    int &Task=iLoadRobotCassetteTask;
    static int iRFIDRetry=0;
    int ret=0;
    bool bCheckLotOK=false;
    static int iWhichPickPort=-1;
    static int iWhichPlacePort=-1;
    static bool bMoveZ=false;

    bNeedDeviceCassette=true;

    switch(Task)
    {
        case 1:
            if(MLoadRobotZCanSafeUpMove()==false)                               //判斷是否可以移動
            {
                Cylinder[C_LoadRobotX].Reset();
                Task=50;
            }
            else
            {
                Task=100;
            }
            break;
        case 50:
            if(Cylinder[C_LoadRobotX].Pop())
            {
                Task=1;
            }
            break;
        case 100:
            //檢查 LoadPort/LoadRobot Cassette 是否置偏
            if(iCheckAllLoadPortSensor()<0)
            {
                ret=ShowErrorMessage("JAM2340", K_RETRY, MMLoadPort, false, "DoLoadRobotCassette_100_1");
            }

            if(MOT[MLoadRobotZ].fHasTray==false)
            {
                if(LastSet.iRealDummy!=DUMMY && Sen[SnLoadRobotHaveCassette1].IsOn() && Sen[SnLoadRobotHaveCassette2].IsOn())
                {
                    if(TestIF_File.bEnableRFID)
                    {
                        Task=500;            //去讀取 RFID 取得 Cassette 資料
                    }
                    else
                    {
                        Task=2000;           //去放 Cassette (不知名 Cassette 放回去)
                    }
                }
                else
                {
                    if(MOT[MLoadRobotZ].ReadPos()!=Prod.iLoadPortZ[0])
                    {
                        Task=300;            //正常速到 Buffer1 待命
                    }
                    else
                    {
                        Task=1000;           //去抓 Cassette
                    }
                }
            }
            else
            {
//                if(bBufferHasNewCassette==true)
//                {
//                    //等待 DoCatchCassetteLM 把 Cassette 抓走，讓 bBufferHasNewCassette=fasle
//                }
                if((LastSet.iRealDummy!=DUMMY && Sen[SnLoadRobotHaveCassette1].IsOn() && Sen[SnLoadRobotHaveCassette2].IsOn()) ||
                    LastSet.iRealDummy==DUMMY)
                {
                    if((MOT[MLoadRobotZ].Tray.Data[0][0]==HAS_CASSETTE_DEVICE && bLoadNeedTray)  ||         //1. 機台需求 HAS_CASSETTE_DEVICE
                       (MOT[MLoadRobotZ].Tray.Data[0][0]==HAS_CASSETTE_TRAY   && bEmptyNeedTray) ||         //2. 機台需求 HAS_CASSETTE_TRAY
                       (MOT[MLoadRobotZ].Tray.Data[0][0]==HAS_CASSETTE_EMPTY)                    ||         //3. 機台需求 HAS_CASSETTE_EMPTY
                       (bCheckBufferIsFull()==false))                                                       //4. 機台沒有需求但 Buffer 還有空位
                    {
                        if(MOT[MLoadRobotZ].ReadPos()!=0)
                        {
                            //Task=200;             //上升到 Buffer1 位置，只要有 Cassette 就要 2 段速
                            Task=300;
                        }
                        else
                        {
                            Task=150;
                            bBufferHasNewCassette=true;
                        }
                    }
                    else//此 Cassette 不需要也不能放到 Buffer 區
                    {
                        Task=1000;              //去放 Cassette (用不到的 Cassette 放回去)
                    }
                }
                else
                {
                    //硬體上沒有 Cassette
                    if(Sen[SnLoadRobotHaveCassette1].IsOn()==false && Sen[SnLoadRobotHaveCassette2].IsOn()==false)
                    {
                        ret=ShowErrorMessage("JAM2340", K_RETRY|K_SKIP, MMLoadPort, false, "DoLoadRobotCassette_100_3");
                        if(ret==K_SKIP)
                        {
                            MOT[MLoadRobotZ].ClearTray();//清除 Casstee 軟體資料
                        }
                    }
                }
            }
            break;
        case 150:    //等待 DoCatchCassetteLM 把 Cassette 抓走，讓 bBufferHasNewCassette=fasle
            if(bBufferHasNewCassette==false &&
             Sen[SnLoadRobotHaveCassette1].IsOn()==false &&
             Sen[SnLoadRobotHaveCassette2].IsOn()==false &&
             MOT[MLoadRobotZ].fHasTray==false)
            {
                  Task=100;
            }
            break;
//        case 200:   //兩段速到 Buffer1
//            bMoveZ=MOT[MLoadRobotZ].MotorMove2SpeedForMR(0, &MRSpeed[LoadRobotZ]);
//            if(bMoveZ)
//            {
//                Task=1;
//            }
//            break;
        case 300:   //正常速到 Buffer1
            bMoveZ=MOT[MLoadRobotZ].MotorMove(0);
            if(bMoveZ)
            {
                Task=1;
            }
            break;
        case 500:
            fRFID->DoRFIDReader(0);
            DoRFIDRead1Delay.SetMSAndOn(TestIF_File.iRFIDDelay);
            break;
        case 600:
            if(fRFID->bRFIDReaderOK[0])
            {
                //有讀到 RFID 的資料後，在跟客戶討論如何取得 Cassette 資料。
                 Task=1;
            }
            else if(DoRFIDRead1Delay.Off())
            {
                if(iRFIDRetry<TestIF_File.iRFIDRetryCount)
                {
                    iRFIDRetry++;
                    Task=500;
                }
                else
                {
                    ret=ShowErrorMessage("JAM2340", K_RETRY|K_SKIP, MMLoadPort, false, "DoLoadRobotCassette_600");
                    if(ret==K_SKIP)
                    {
                        Task=1000;  //讀不到就要放回去 LoadPort
                    }
                    else
                    {
                        Task=500;
                        #ifndef SOFT_SIMULTE
                        fRFID->RFID_1->StopComm();
                        fRFID->RFID_1->StartComm();
                        #endif
                    }
                    iRFIDRetry=0;
                }
            }
            break;

        /*
        case 700:
            預計新增讀取 RFID 後要做的事
            break;
        */
        case 1000:  //去抓 Cassette
            iWhichPickPort=iWhichLoadPortPick();
            if(iWhichPickPort>=0 && iWhichPickPort<=3)
            {
                InitialLoadRobotCassettePickTask();
                Task=1100;
            }
            else if(MOT[MLoadRobotZ].ReadPos()!=Prod.iLoadPortZ[0])  //沒有取 Cassette 就將 LoadRobotZ 移動到待命位置。
            {
                Task=300;
            }
            else if(iTrayFeed==1)
            {
                if(bMRAutoMode==false && WhichBufferCanCatch(HAS_CASSETTE_DEVICE)==0 && bNewLotHotplate==false && bNewLotLoader==false)
                {
                    if(MOT[MLoadRobotZ].HasIC()==false  &&
                       WhichBufferCanCatch(HAS_CASSETTE_FAIL)==0 &&    //20181011
                       WhichBufferCanCatch(HAS_CASSETTE_PASS)==0 &&    //20181011
                       MOT[MCasArmZ].Tray.Data[0][0]==NULL_IC &&
                       WhichAutoCanCatch()==0)
                    {
                        bLoadCassetteTrayFeed=true;
                    }
                }
                else
                {
                    if(MOT[MLoadRobotZ].HasIC()==false  &&
                       MOT[MCasArmZ].Tray.Data[0][0]==NULL_IC &&
                       WhichAutoCanCatch()==0)
                    {
                        bLoadCassetteTrayFeed=true;
                    }
                }
            }
            else
            {
                Task=1;
            }
            break;

        case 1100:
            if(DoLoadRobotCassettePick(iWhichPickPort,true))
            {
                Task=1;
            }
            break;
        case 2000:  //去放 Cassette
            iWhichPlacePort=iWhichLoadPortPlace();
            if(iWhichPlacePort>=0 && iWhichPlacePort<=3)
            {
                InitialLoadRobotCassettePickTask();
                Task=2100;
            }
            else
            {
                //找不到地方放表示，LoadPort 滿了

                ShowCassetteMessage("Please remove cassette from load port!!","請移除 load port上的 cassette!!");
                Task=1;
            }
            break;
        case 2100:
            if(DoLoadRobotCassettePick(iWhichPlacePort,false))
            {
                Task=1;
            }
            break;
    }
}
//------------------------------------------------------------------------------
int iUnloadRobotCassetteTask=1;
void InitialUnloadRobotCassetteTask()
{
    iUnloadRobotCassetteTask=1;
}
//------------------------------------------------------------------------------
void DoUnloadRobotCassette()
{
    int &Task=iUnloadRobotCassetteTask;
    int ret=0;
    static int iWhichPickPort=-1;
    static int iWhichPlacePort=-1;
    static bool bMoveZ=false;
    switch(Task)
    {
        case 1:
            if(MUnloadRobotZCanSafeUpMove()==false)                               //判斷是否可以移動
            {
                Cylinder[C_UnloadRobotX].Reset();
                Task=50;
            }
            else
            {
                Task=100;
            }
            break;
        case 50:
            if(Cylinder[C_UnloadRobotX].Pop())
            {
                Task=1;
            }
            break;
        case 100:
            //檢查 UnloadPort Sensor 是否置偏
            if(iCheckAllUnloadPortSensor()<0)
            {
                ret=ShowErrorMessage("JAM2340", K_RETRY, MMLoadPort, false, "DoUnloadRobotCassette_100_1");
            }
//            if(CheckUnloadRobotSensor()==false)
//            {
//                ret=ShowErrorMessage("JAM2340", K_RETRY, MMLoadPort, false, "DoUnloadRobotCassette_100_2");
//            }

            if(MOT[MUnloadRobotZ].fHasTray==false)
            {

                if(LastSet.iRealDummy!=DUMMY && Sen[SnUnloadRobotHaveCassette1].IsOn() && Sen[SnUnloadRobotHaveCassette2].IsOn() && bUnloadRobotHasCassette)
                {
                    bUnloadRobotHasCassette=false;
                    Task=2000;          //放回去 UnloadPort
                }
                else
                {
                    if(MOT[MUnloadRobotZ].ReadPos()!=0)
                    {
                        Task=300;           //正常速到 Buffer10 待命
                    }
                    else if(iTrayFeed==1)
                    {
                        if(bMRAutoMode==false && WhichBufferCanCatch(HAS_CASSETTE_DEVICE)==0 && bNewLotHotplate==false && bNewLotLoader==false)
                        {
                            if(WhichBufferCanCatch(HAS_CASSETTE_FAIL)==0 &&    //20181011
                               WhichBufferCanCatch(HAS_CASSETTE_PASS)==0 &&    //20181011
                               MOT[MCasArmZ].Tray.Data[0][0]==NULL_IC &&
                               WhichAutoCanCatch()==0)
                            {
                                bUnloadCassetteTrayFeed=true;
                            }
                        }
                        else
                        {
                            if(MOT[MCasArmZ].Tray.Data[0][0]==NULL_IC &&
                               WhichAutoCanCatch()==0)
                            {
                                bUnloadCassetteTrayFeed=true;
                            }
                        }
                    }
                }
            }
            else
            {
                if((LastSet.iRealDummy!=DUMMY && Sen[SnUnloadRobotHaveCassette1].IsOn() && Sen[SnUnloadRobotHaveCassette2].IsOn() && bUnloadRobotHasCassette) ||
                    LastSet.iRealDummy==DUMMY)
                {
                    bUnloadRobotHasCassette=false;
                    Task=2000;            //放回去 UnloadPort
                }
                else
                {
                    if(Sen[SnUnloadRobotHaveCassette1].IsOn()==false && Sen[SnUnloadRobotHaveCassette2].IsOn()==false)
                    {
                        //Casstte 失蹤了
                        ret=ShowErrorMessage("JAM2340", K_RETRY|K_SKIP, MMLoadPort, false, "DoUnloadRobotCassette_100_3");
                        if(ret==K_SKIP)
                        {
                            MOT[MUnloadRobotZ].ClearTray();//清除 Casstee 軟體資料
                        }
                    }
                }
            }
            break;

        case 300:   //正常速到 Buffer10
            bMoveZ=MOT[MUnloadRobotZ].MotorMove(0);
            if(bMoveZ)
            {
                Task=1;
            }
            break;
        case 2000:          //放回去 UnloadPort
            iWhichPlacePort=iWhichUnloadPortPlace();
            if(iWhichPlacePort>=0 && iWhichPlacePort<=3)
            {
                InitialUnloadRobotCassettePickTask();
                Task=2100;
            }
            else
            {
                //UnloadPort 滿了 Cassette 放不回去
                ShowCassetteMessage("Please remove cassette from unload port!!","請移除 Unload Port上的 cassette!!");
                Task=1;
            }
            break;
        case 2100:
            if(DoUnloadRobotCassettePick(iWhichPlacePort,false))
            {
                Task=1;
            }
            break;
    }
}
//<==
//Sam 20190112 LM
//------------------------------------------------------------------------------
bool MElevatorCanSafeUpMove()
{
    if(MOT[MCasArmX].ReadPos()>=Prod.iCassetteArmX[2] &&
       Cylinder[C_LoadPortYOff].OnSensor())//&&
//       Sen[SnSafeDoor11].Enable && Sen[SnSafeDoor11].IsOn()==true)
        return true;
    return false;
}

//------------------------------------------------------------------------------
bool MElevatorCanSafeMove()
{
    if(Cylinder[C_LoadPortYOff].OnSensor())
        return true;
    return false;
}
//------------------------------------------------------------------------------
//Sam 20190112 LM
//==>
bool MLoadRobotZCanSafeUpMove()
{
    if(Cylinder[C_LoadRobotX].OffSensor())//&&
//       Sen[SnSafeDoor11].Enable && Sen[SnSafeDoor11].IsOn()==true)
        return true;
    return false;
}
//------------------------------------------------------------------------------
bool MLoadRobotZCanSafeMove()
{
    if(Cylinder[C_LoadRobotX].OffSensor())
        return true;
    return false;
}
//------------------------------------------------------------------------------
bool HasICUnderLoadPort()
{
    if(USE_MR_SYSTEM==1)
    {
        if(MOT[MMTrayLoader].Tray.HasIC())
        {
           return true;
        }
    }
    else
    {
        for(int i=0; i<4; i++)
        {
            if(MOT[MMLoadPort1+i].HasIC())
            {
                 return true;
            }
        }
    }

    return false;
}


//------------------------------------------------------------------------------
bool MUnloadRobotZCanSafeUpMove()
{
    if(Cylinder[C_UnloadRobotX].OffSensor())//&&
//       Sen[SnSafeDoor11].Enable && Sen[SnSafeDoor11].IsOn()==true)
        return true;
    return false;
}
//------------------------------------------------------------------------------
bool MUnloadRobotZCanSafeMove()
{
    if(Cylinder[C_UnloadRobotX].OffSensor())
        return true;
    return false;
}
//------------------------------------------------------------------------------
bool HasICUnderUnloadPort()
{
    for(int i=0; i<4; i++)
    {
        if(MOT[MMUnloadPort1+i].HasIC())
        {
            return true;
        }
    }
}
//<==
//Sam 20190112 LM
//------------------------------------------------------------------------------
int iLoadPortElevatorUpTask=1;
HTimer DoLoadPortElevatorUpDelay;
//------------------------------------------------------------------------------
void InitialLoadPortElevatorUpTask()
{
    iLoadPortElevatorUpTask=1;
}
//------------------------------------------------------------------------------
bool DoLoadPortElevatorUp()
{
    int ret;
    int &Task=iLoadPortElevatorUpTask;
    static int iAlarmCount=0;

    switch(Task)
    {
        case 1:
            bNeedDeviceCassette=false;
            if(MElevatorCanSafeUpMove()==false)                                 //判斷是否可以移動
            {
                MOT[MCaselevatorZ].PCIL132_StopMotor();
                if(Cylinder[C_LoadPortYOff].OnSensor()==false)
                {
                    C_Catch_Cassette_Pop(1, C_LoadPortYOn, true);
                    Task=50;
                }
                break;
            }

            if(MOT[MCaselevatorZ].MotorMove(Prod.iLoadPortZ[1]))                //移動到安全位置
            {
                if(bLoadPortNoCassette) //wei 20181120
                {
                    Task=860;
                }
                else if(MOT[MCaselevatorZ].fHasTray)
                {
                    Task=600;
                }
                else
                {
                    Task=100;
                }
                bNeedDeviceCassette=true;
            }
            break;
        case 50:
            if(C_Catch_Cassette_Pop(1, C_LoadPortYOn))
            {
                Task=1;
            }
            break;
        case 100:
            if(Cylinder[C_LoadPortCatchOff].OnSensor()==false)                  //判斷夾爪放開
            {
                C_Catch_Cassette_Pop(0, C_LoadPortCatchOn, true);               //inital 放開
                Task=150;
            }
            else
            {
                Task=200;

            }
            break;
        case 150:
            if(C_Catch_Cassette_Pop(0, C_LoadPortCatchOn))                      //放開
            {
                Task=100;
            }
            break;
        case 200:
            bNeedDeviceCassette=false;
            if(MOT[MCaselevatorZ].MotorMove2SpeedForMR(Prod.iLoadPortZ[0], &MRSpeed[CaselevatorZ]))  //二段速下降
            {
                C_Catch_Cassette_Push(0, C_LoadPortCatchOn, true);              //inital 夾取
                bNeedDeviceCassette=true;
                Task=300;
            }
            break;
        case 300:
            if(C_Catch_Cassette_Push(0, C_LoadPortCatchOn))                     //夾取
            {
                iAlarmCount=0;
                Task=500;
            }
            break;
        case 500:
            #ifndef SOFT_SIMULTE
            if(LastSet.iRealDummy==DUMMY || (Sen[SnLoadPortCatch].IsOn() && Sen[SnCassetteCatch].IsOn()))        //判斷seneor有沒有亮
            #endif
            {
                SetCassetteTray(MCaselevatorZ, MMLoadPort);
                iAlarmCount=0;
                Task=600;
            }
            #ifndef SOFT_SIMULTE
            else
            {
                iAlarmCount++;
                if(iAlarmCount>=30)
                {
                    C_Catch_Cassette_Pop(0, C_LoadPortCatchOn, true);           //inital 放開
                    iAlarmCount=0;
                    Task=510;
                }
            }
            #endif
            break;
        case 510:
            if(C_Catch_Cassette_Pop(0, C_LoadPortCatchOn))
            {
                Task=520;
            }
            break;
        case 520:
            bNeedDeviceCassette=false;
            if(MElevatorCanSafeUpMove()==false)                                  //判斷是否可以移動
            {
                MOT[MCaselevatorZ].PCIL132_StopMotor();
                break;
            }

            if(MOT[MCaselevatorZ].MotorMove(Prod.iLoadPortZ[1]))                                 //上升高於buffer1
            {
                bNeedDeviceCassette=true;
                ret=ShowErrorMessage("JAM2320", K_RETRY, MMLoadPort, false, "DoLoadPortElevatorUp_520");

                if(ret==K_RETRY)
                {
                    Task=100;
                }
//                else if(ret==K_SKIP)
//                {
//
//                }
            }
            break;
        case 600:
            bNeedDeviceCassette=false;
            if(MElevatorCanSafeUpMove()==false)                                  //判斷是否可以移動
            {
                MOT[MCaselevatorZ].PCIL132_StopMotor();
                break;
            }

            if(MOT[MCaselevatorZ].MotorMove(50))                                 //上升高於buffer1
            {
                C_Catch_Cassette_Push(1, C_LoadPortYOn, true);                  //inital Loadport y 往前
                bNeedDeviceCassette=true;
                Task=700;
            }
            break;
        case 700:
            if(C_Catch_Cassette_Push(1, C_LoadPortYOn))                         //Loadport y 往前
            {
                Task=760;
            }
            break;
        case 760:
            bNeedDeviceCassette=false;
            if(MOT[MCaselevatorZ].MotorMove2SpeedForMR(Prod.iLoadPortZ[2], &MRSpeed[CaselevatorZ]))
            {
                C_Catch_Cassette_Pop(0, C_LoadPortCatchOn, true);               //inital 放開
                bNeedDeviceCassette=true;
                Task=800;
            }
            break;
        case 800:
            if(C_Catch_Cassette_Pop(0, C_LoadPortCatchOn))                      //放開
            {
                Task=860;
//                fMain->Pause();
            }
            break;
        case 860:
            #ifndef SOFT_SIMULTE
            if(LastSet.iRealDummy==DUMMY || Sen[SnBuffer1HaveCassette].IsOn())  //判斷buffer區 seneor有沒有亮
            #endif
            {
                SetCassetteTray(MMCABuffer1, MCaselevatorZ);
                bBufferHasNewCassette=true;
                if(bLoadPortNoCassette) //wei 20181120
                {
                    C_Catch_Cassette_Pop(1, C_LoadPortYOn, true);                   //inital Loadport y 往後
                    bLoadPortNoCassette=false;
                    Task=1030;
                }
                else
                {
                    Task=1000;
                }
            }
            #ifndef SOFT_SIMULTE
            else
            {
                Task=870;//alarm
            }
            #endif
            break;
        case 870:
            if(MOT[MCaselevatorZ].MotorMove(50))
            {
                ret=ShowErrorMessage("JAM2321", K_RETRY, MMCABuffer1, false, "DoLoadPortElevatorUp_870");

                if(ret==K_RETRY)
                {
                    Task=860;
                }
    //            else if(ret==K_SKIP)
    //            {
    //                Task=860;
    //            }
            }
            break;
        case 1000:
            bNeedDeviceCassette=false;
            if(MOT[MCaselevatorZ].MotorMove(50))                                 //上升高於buffer1
            {
                C_Catch_Cassette_Pop(1, C_LoadPortYOn, true);                   //inital Loadport y 往後
                bNeedDeviceCassette=true;
                Task=1030;
            }
            break;
        case 1030:
            if(C_Catch_Cassette_Pop(1, C_LoadPortYOn))                          //Loadport y 往後
            {
                Task=1100;
            }
            break;
        case 1100:
            bNeedDeviceCassette=false;
            if(MOT[MCaselevatorZ].MotorMove(Prod.iLoadPortZ[1]))                //移動到saft位置
            {
                Task=1;
//                bBufferHasNewCassette=true;
                bNeedDeviceCassette=true;
//                if(iNeedDeviceCassette<2)
//                    bNeedDeviceCassette=true;
//                else
//                    bNeedDeviceCassette=false;
                return true;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
int iLoadPortElevatorDownTask=1;
HTimer DoLoadPortElevatorDownDelay;
//------------------------------------------------------------------------------
void InitialLoadPortElevatorDownTask()
{
    iLoadPortElevatorDownTask=1;
}
//------------------------------------------------------------------------------
bool DoLoadPortElevatorDown()
{
    int ret;
    int &Task=iLoadPortElevatorDownTask;
    static int iAlarmCount=0;

    switch(Task)
    {
        case 1:
            bNeedDeviceCassette=false;
            if(MElevatorCanSafeUpMove()==false)                                 //判斷是否可以移動
            {
                MOT[MCaselevatorZ].PCIL132_StopMotor();
                if(Cylinder[C_LoadPortYOff].OnSensor()==false)                   //inital Loadport 往後
                {
                    C_Catch_Cassette_Pop(1, C_LoadPortYOn, true);
                    Task=50;
                }
                break;
            }

            if(MOT[MCaselevatorZ].MotorMove(Prod.iLoadPortZ[1]))                //移動到安全位置
            {
                bNeedDeviceCassette=true;
                if(bLoadPortNoCassette) //wei 20181120
                {
                    bLoadPortNoCassette=false;
                    Task=1000;
                }
                else if(MOT[MCaselevatorZ].fHasTray)
                {
                    C_Catch_Cassette_Pop(1, C_LoadPortYOn, true);                   //inital Loadport y 往後
                    Task=600;
                }
                else
                {
                    Task=100;
                }
            }
            break;
        case 50:
            if(C_Catch_Cassette_Pop(1, C_LoadPortYOn))                          //Loadport y 往後
            {
                Task=1;
            }
            break;
//        case 80:
//            if(MOT[MCaselevatorZ].fHasTray &&                                        //有cassette就去放
//               ((LastSet.iRealDummy!=DUMMY && (Sen[SnLoadPortCatch].IsOn() && Cylinder[C_LoadPortCatchOn].OnSensor()==true)) ||
//                 LastSet.iRealDummy==DUMMY))         //20181116 QQ
//            {
//                Task=600;
//            }
//            else
//            {
//                Task=100;
//            }
//            break;
        case 100:
            bNeedDeviceCassette=false;
            if(MElevatorCanSafeUpMove()==false)                                 //判斷是否可以移動
            {
                MOT[MCaselevatorZ].PCIL132_StopMotor();
                break;
            }

            if(MOT[MCaselevatorZ].MotorMove(50))                                 //上升高於buffer1
            {
                C_Catch_Cassette_Push(1, C_LoadPortYOn, true);                  //inital Loadport y 往前
                bNeedDeviceCassette=true;
                Task=200;
            }
            break;
        case 200:
            if(C_Catch_Cassette_Push(1, C_LoadPortYOn))                         //Loadport y 往前
            {
                Task=300;
            }
            break;
        case 300:
            if(Cylinder[C_LoadPortCatchOff].OnSensor()==false)                  //判斷夾爪狀態
            {
                C_Catch_Cassette_Pop(0, C_LoadPortCatchOn, true);               //inital 放開
                Task=350;
            }
            else
            {
                Task=400;
            }
            break;
        case 350:
            if(C_Catch_Cassette_Pop(0, C_LoadPortCatchOn))                      //放開
            {
                Task=300;
            }
            break;
        case 400:
            bNeedDeviceCassette=false;
            if(MOT[MCaselevatorZ].MotorMove2SpeedForMR(Prod.iLoadPortZ[2], &MRSpeed[CaselevatorZ]))
            {
                C_Catch_Cassette_Push(0, C_LoadPortCatchOn, true);               //inital 夾取
                bNeedDeviceCassette=true;
                Task=500;
            }
            break;
        case 500:
            if(C_Catch_Cassette_Push(0, C_LoadPortCatchOn))                      //夾取
            {
                Task=550;
            }
            break;
        case 550:
            #ifndef SOFT_SIMULTE
            if(LastSet.iRealDummy==DUMMY || (Sen[SnLoadPortCatch].IsOn() && Sen[SnCassetteCatch].IsOn()))        //判斷seneor有沒有亮
            #endif
            {
                SetCassetteTray(MCaselevatorZ, MMCABuffer1);
                Task=590;
            }
            #ifndef SOFT_SIMULTE
            else
            {
                C_Catch_Cassette_Pop(0, C_LoadPortCatchOn, true);               //inital 放開
                Task=560;  //alarm
            }
            #endif
            break;
        case 560:
            if(C_Catch_Cassette_Pop(0, C_LoadPortCatchOn))                      //放開
            {
                Task=570;
            }
            break;
        case 570:
            if(MOT[MCaselevatorZ].MotorMove(50))                                 //上升高於buffer1
            {
                ret=ShowErrorMessage("JAM2320", K_RETRY, MMCABuffer1, false, "DoLoadPortElevatorDown_570");

                if(ret==K_RETRY)
                {
                    Task=300;
                }
            }
            break;
        case 590:
            bNeedDeviceCassette=false;
            if(MOT[MCaselevatorZ].MotorMove(50))                                //上升高於buffer1
            {
                C_Catch_Cassette_Pop(1, C_LoadPortYOn, true);                   //inital Loadport y 往後
                bNeedDeviceCassette=true;
                Task=600;
            }
            break;
        case 600:
            if(C_Catch_Cassette_Pop(1, C_LoadPortYOn))                          //Loadport y 往後
            {
                Task=700;
            }
            break;
        case 700:
            bNeedDeviceCassette=false;
            if(MOT[MCaselevatorZ].MotorMove2SpeedForMR(Prod.iLoadPortZ[0], &MRSpeed[CaselevatorZ]))
            {
                C_Catch_Cassette_Pop(0, C_LoadPortCatchOn, true);               //inital 放開
                bNeedDeviceCassette=true;
                Task=900;
            }
            break;
        case 800:
            #ifndef SOFT_SIMULTE
            if(LastSet.iRealDummy==DUMMY ||                                     //判斷seneor有沒有亮     //wei 20180911
               (Sen[SnLoadPortPresent].IsOn() ))
            #endif
            {
                Task=900;
            }
            #ifndef SOFT_SIMULTE
            else
            {
                Task=850;
            }
            #endif
            break;
        case 850:
            ret=ShowErrorMessage("JAM2334", K_RETRY|K_SKIP, MMLoadPort, false, "DoLoadPortElevatorDown_850");

            if(ret==K_RETRY)
            {
                Task=800;
            }
            else if(ret==K_SKIP)
            {
                Task=900;
            }
            break;
        case 900:
            if(C_Catch_Cassette_Pop(0, C_LoadPortCatchOn))                      //放開
            {
                Task=1000;
                iAlarmCount=0;
//                fMain->Pause();
            }
            break;
        case 1000:
            #ifndef SOFT_SIMULTE
            if(LastSet.iRealDummy==DUMMY ||                                     //判斷seneor有沒有亮     //wei 20180911
               (Sen[SnLoadPortPresent].IsOn() ))    //&& Sen[SnLoadPortPlacement1].IsOn() && Sen[SnLoadPortPlacement2].IsOn()
            #endif
            {
                SetCassetteTray(MMLoadPort, MCaselevatorZ);

                Task=1100;
                iAlarmCount=0;
            }
            #ifndef SOFT_SIMULTE
            else
            {
                iAlarmCount++;
                if(iAlarmCount>=20)
                {
                    Task=1050;  //alarm
                    iAlarmCount=0;
                }
            }
            #endif
            break;
        case 1050:
            ret=ShowErrorMessage("JAM2322", K_RETRY|K_SKIP, MMLoadPort, false, "DoLoadPortElevatorDown_1050");

            if(ret==K_RETRY)
            {
                Task=1000;
                iAlarmCount=0;
            }
            else if(ret==K_SKIP)
            {
                SetCassetteTray(MMLoadPort, MCaselevatorZ);
                Task=1100;
            }
            break;
        case 1100:
            bNeedDeviceCassette=false;
            if(MOT[MCaselevatorZ].MotorMove(Prod.iLoadPortZ[1]))                //移動到saft位置
            {
                Task=1;
                bNeedDeviceCassette=true;
                return true;
            }
            break;
    }
    return false;
}
//Sam 20190112 LM
//==>
//------------------------------------------------------------------------------
int iLoadRobotCassettePickTask=1;
HTimer DoLoadRobotCassettePickDelay;
//------------------------------------------------------------------------------
void InitialLoadRobotCassettePickTask()
{
    iLoadRobotCassettePickTask=1;
}
//------------------------------------------------------------------------------
bool DoLoadRobotCassettePick(int iPortTarget,bool bPick)     //iPortTarget=0~3
{
    int ret;
    int &Task=iLoadRobotCassettePickTask;
    int iPortSensor=0;
    int iPos=0;
    static bool bMoveZ=false;

    AnsiString asErrMsg="";

    switch(Task)
    {
        case 1:
            if(MLoadRobotZCanSafeUpMove()==false)                               //判斷是否可以移動
            {
                MOT[MLoadRobotZ].PCIL132_StopMotor();
                if(Cylinder[C_LoadRobotX].OnSensor()==false)                 //inital LoadRobotX 往右
                {
                    Cylinder[C_LoadRobotX].Reset();
                    Task=50;
                }
                break;
            }
            bMoveZ=false;
            Task=100;
            break;
        case 50:
            if(Cylinder[C_LoadRobotX].Pop())                         //LoadRobotX 縮
            {
                Task=1;
            }
            break;
        case 100:   //移動到 Target LoadPort 取 Cassette 位置
            if(bPick)
            {
                iPos=Prod.iLoadRobotZ[iPortTarget];
            }
            else
            {
                iPos=Prod.iLoadRobotZ[iPortTarget]+Prod.iLoadRobotUpDownSensor;
            }
            bMoveZ=MOT[MLoadRobotZ].MotorMove(iPos);
            if(bMoveZ)
            {
                Task=200;
            }
            break;
        case 200:   //Check Sensor
            if(bPick)
            {
                #ifndef SOFT_SIMULTE
                //檢查高度是否安全
                if(Sen[SnLoadRobotPosDown].IsOff())
                {
                    //ShowMyMessage("請檢查 LoadRobot 高度位置是否會干涉或檢查 SnLoadRobotXCanOn Sensor");
                    ret=ShowErrorMessage("JAM2320", K_SKIP|K_RETRY, MLoadRobotZ, false, "DoLoadRobotCassettePick_200_Pick1");
                    if(ret==K_SKIP)
                    {
                        Task=5000;
                    }
                    break;
                }
                #endif
                //從 LoadPort 取 Cassette 到 LoadRobot，檢查 LoadRobot 是否有 Cassette
                if(LastSet.iRealDummy!=DUMMY && (Sen[SnLoadRobotHaveCassette1].IsOn() || Sen[SnLoadRobotHaveCassette2].IsOn()))
                {
                    //ShowMyMessage("請手動移除 LoadRobot 上的 Cassette 或檢查 LoadRobot Sensor");
                    ret=ShowErrorMessage("JAM2320", K_SKIP|K_RETRY, MLoadRobotZ, false, "DoLoadRobotCassettePick_200_Pick2");
                    if(ret==K_SKIP)
                    {
                        Task=5000;
                    }
                    break;
                }
            }
            else
            {
                #ifndef SOFT_SIMULTE
                //檢查高度是否安全
                if(Sen[SnLoadRobotPosUp].IsOff())
                {
                    //ShowMyMessage("請檢查 LoadRobot 高度位置是否會干涉或檢查 SnLoadRobotXCanOff Sensor");
                    ret=ShowErrorMessage("JAM2320", K_SKIP|K_RETRY, MLoadRobotZ, false, "DoLoadRobotCassettePick_200_Place1");
                    if(ret==K_SKIP)
                    {
                        Task=5000;
                    }
                    break;
                }
                #endif
                //從 LoadRobot 放 Cassette 到 LoadPort，檢查 LoadPort 是否有 Cassette
                iPortSensor=SnLoadPortHaveCassette1_1+iPortTarget*2;
                if(LastSet.iRealDummy!=DUMMY && (Sen[iPortSensor].IsOn() || Sen[iPortSensor+1].IsOn()))
                {
                    //ShowMyMessage("請手動移除 LoadPort 上的 Cassette 或檢查 LoadPort Sensor");
                    ret=ShowErrorMessage("JAM2320", K_SKIP|K_RETRY, MLoadRobotZ, false, "DoLoadRobotCassettePick_200_Place2");
                    if(ret==K_SKIP)
                    {
                        Task=5000;
                    }
                    break;
                }
            }
            Task=300;
            break;
        case 300:   //準備伸
            Cylinder[C_LoadRobotX].Reset();
            Task=400;
            break;
        case 400:   //伸
            if(Cylinder[C_LoadRobotX].Push())
            {
                bMoveZ=false;
                Task=500;
            }
            break;
        case 500:   //上升到 iPortTarget Cassette 脫離的位置
            if(bPick)
            {
                iPos=Prod.iLoadRobotZ[iPortTarget]+Prod.iLoadRobotUpDownSensor;
            }
            else
            {
                iPos=Prod.iLoadRobotZ[iPortTarget];
            }
            bMoveZ=MOT[MLoadRobotZ].MotorMove2SpeedForMR(iPos, &MRSpeed[LoadRobotZ]);
            //bMoveZ=MOT[MLoadRobotZ].MotorMove(iPos);
            if(bMoveZ)
            {
                DoLoadRobotCassettePickDelay.SetSecAndOn(0.1);//等馬達整定
                Task=550;
            }
            break;
        case 550:
            if(DoLoadRobotCassettePickDelay.Off())
            {
                Task=600;
            }
            break;
        case 600:   //Check Sensor
            if(bPick)
            {
                #ifndef SOFT_SIMULTE
                //檢查高度是否安全
                if(Sen[SnLoadRobotPosUp].IsOn()==false)
                {
                    //ShowMyMessage("請檢查 LoadRobot 高度位置是否會干涉或檢查 SnLoadRobotPosUp Sensor");
                    ret=ShowErrorMessage("JAM2320", K_RETRY, MLoadRobotZ, false, "DoLoadRobotCassettePick_600_Pick1");
                    break;
                }
                #endif
                //檢查在 LoadRobot 上的 Cassette 是否放好
                if(LastSet.iRealDummy!=DUMMY &&
                  (Sen[SnLoadRobotHaveCassette1].IsOff() || Sen[SnLoadRobotHaveCassette2].IsOff()))
                {
                    //ShowMyMessage("請檢查 LoadRobot 上的 Cassette 是否置偏或檢查 LoadRobot Sensor");
                    ret=ShowErrorMessage("JAM2320", K_RETRY, MLoadRobotZ, false, "DoLoadRobotCassettePick_600_Pick2");
                    break;
                }
                SetCassetteTray(MLoadRobotZ,MMLoadPort1+iPortTarget); //Port ===> Robot
            }
            else
            {
                 #ifndef SOFT_SIMULTE
                //檢查高度是否安全
                if(Sen[SnLoadRobotPosDown].IsOn()==false)
                {
                    //ShowMyMessage("請檢查 LoadRobot 高度位置是否會干涉或檢查 SnLoadRobotPosDown Sensor");
                    ret=ShowErrorMessage("JAM2320", K_RETRY, MLoadRobotZ, false, "DoLoadRobotCassettePick_600_Place2");
                    break;
                }
                #endif
                //檢查在 LoadPort 上的 Cassette 是否放好
                iPortSensor=SnLoadPortHaveCassette1_1+iPortTarget*2;
                if(LastSet.iRealDummy!=DUMMY &&
                  (Sen[iPortSensor].IsOff() || Sen[iPortSensor+1].IsOff()))
                {
                    //ShowMyMessage("請檢查 LoadRobot 上的 Cassette 是否置偏或檢查 LoadRobot Sensor");
                    ret=ShowErrorMessage("JAM2320", K_RETRY, MLoadRobotZ, false, "DoLoadRobotCassettePick_600_Place1");
                    break;
                }
                SetCassetteTray(MMLoadPort1+iPortTarget,MLoadRobotZ); //Port ===> Robot
            }
            Task=700;
            break;
        case 700:  //inital LoadRobotX 準備縮
            Cylinder[C_LoadRobotX].Reset();
            Task=800;
            break;
        case 800: //LoadRobotX 往縮
            if(Cylinder[C_LoadRobotX].Pop())
            {
                if(CheckLoadRobotSensor()==false)
                {
                    ret=ShowErrorMessage("JAM2340", K_RETRY, MMLoadPort, false, "DoLoadRobotCassettePick_800");
                    if(ret==K_RETRY)
                    {
                        Task=700;
                        break;
                    }
                }
                Task=5000;
            }
            break;
        case 5000:
            Task=1;
            return true;
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
int iWhichLoadPortPick()   //return 0~3
{
    int ret,i;
    bool bFind;
    int iSn;

    ret=-1;
    bFind=false;

    if(MOT[MLoadRobotZ].fHasTray || (LastSet.iRealDummy!=DUMMY && Sen[SnLoadRobotHaveCassette1].IsOn() && Sen[SnLoadRobotHaveCassette2].IsOn()))
    {
        return ret;
    }

    //由需求來選擇 LoadPort
    //找有 HAS_CASSETTE_DEVICE Cassette 的 LoadPort
    //需要留一組 iBuffer 位置來防止塞車
    if(bCheckBufferIsFull()==false)
    {
        if(bLoadNeedTray)   //機台需要 Decive Tray Cassette
        {
            for(i=0; i<4; i++)
            {
                if(MOT[MMLoadPort1+i].Tray.Data[0][0]==HAS_CASSETTE_DEVICE)
                {
                    ret=i;
                    bFind=true;
                    break;
                }
            }
        }
        //找有 HAS_CASSETTE_TRAY Cassette 的 LoadPort
        if(bFind==false && bEmptyNeedTray)  //機台需要 Empty Tray Cassette
        {
            for(i=0; i<4; i++)
            {
                if(MOT[MMLoadPort1+i].Tray.Data[0][0]==HAS_CASSETTE_TRAY)
                {
                    ret=i;
                    bFind=true;
                    break;
                }
            }
        }
    }
    if(bFind)
    {
        return ret;
    }
    if(TestIF_File.bEnableRFID)
    {
        //檢測 LoadPort 的 Sensor 是否有未知 Cassette
        for(i=0; i<4; i++)
        {
            if(MOT[MMLoadPort1+i].fHasTray==false)
            {
                iSn=SnLoadPortHaveCassette1_1+i*2;
                if(LastSet.iRealDummy!=DUMMY && Sen[iSn].IsOn() && Sen[iSn+1].IsOn())
                {
                    ret=i;
                    bFind=true;
                    break;
                }
            }
        }
    }
    else
    {
        //都沒有需求的話就從 LoadPort1 有 Cassette 就搬上去
        if(bFind==false && bCheckBufferIsFull()==false) //需要留一組 iBuffer 位置來防止塞車
        {
            for(i=0; i<4; i++)
            {
                iSn=SnLoadPortHaveCassette1_1+i*2;
                if(MOT[MMLoadPort1+i].fHasTray &&
                  ((LastSet.iRealDummy!=DUMMY && Sen[iSn].IsOn() && Sen[iSn+1].IsOn()) || LastSet.iRealDummy==DUMMY))
                {
                    ret=i;
                    bFind=true;
                    break;
                }
            }
        }
    }
    return ret;
}
//------------------------------------------------------------------------------
int iWhichLoadPortPlace()   //return 0~3
{
    int i,ret,iPortSensor;
    ret=-1;
    for(i=0;4<i; i++)
    {
        iPortSensor=SnLoadPortHaveCassette1_1+i*2;
        #ifndef SOFT_SIMULTE
        if(LastSet.iRealDummy!=DUMMY && (Sen[iPortSensor].IsOn()==false && Sen[iPortSensor+1].IsOn()==false) ||
          (LastSet.iRealDummy==DUMMY && MOT[MMLoadPort1+i].fHasTray==false))
        {
            ret=i;
        }
        #else
        if(MOT[MMLoadPort1+i].fHasTray==false)
        {
            ret=i;
        }
        #endif
    }
    return ret;
}
//------------------------------------------------------------------------------
bool bCheckBufferIsFull()
{
    int iMMCABuffer,iSensor,i;
    int iBufferHasCassetteCnt=0;

    //檢查計算 MMCABuffer2~9 有 Cassette 的數量
    for(i=0; i<9; i++)
    {
        if(CheckHasCassetteSensor(i))
        {
            iBufferHasCassetteCnt++;
        }
    }
    //檢查 MCasArmZ Cassette
    if(MOT[MCasArmZ].fHasTray)
        iBufferHasCassetteCnt++;

    if(iBufferHasCassetteCnt>=7) //需要留一組 iBuffer 位置來防止塞車
    {
        return true;
    }
    else
    {
        return false;
    }

}
//------------------------------------------------------------------------------
int iUnloadRobotCassettePickTask=1;
HTimer DoUnloadRobotCassettePickDelay;
//------------------------------------------------------------------------------
void InitialUnloadRobotCassettePickTask()
{
    iUnloadRobotCassettePickTask=1;
}
//------------------------------------------------------------------------------
bool DoUnloadRobotCassettePick(int iPortTarget,bool bPick)     //iPortTarget=0~3
{
    int ret;
    int &Task=iUnloadRobotCassettePickTask;
    int iPortSensor=0;
    int iPos=0;
    bool b2Speed;//是否使用2段速
    static bool bMoveZ=false;

    AnsiString asErrMsg="";

    switch(Task)
    {
        case 1:
            //bNeedDeviceCassette=false;
            if(MUnloadRobotZCanSafeUpMove()==false)                             //判斷是否可以移動
            {
                MOT[MUnloadRobotZ].PCIL132_StopMotor();
                if(Cylinder[C_UnloadRobotX].OnSensor()==false)
                {
                    Cylinder[C_UnloadRobotX].Reset();            //inital UnloadRobotX 準備縮
                    Task=50;
                }
                break;
            }
            Task=100;
            bMoveZ=false;
//            if(MOT[MUnloadRobotZ].MotorMove(Prod.iUnloadRobotZ[1]))           //移動到安全位置
//            {
//                //bNeedDeviceCassette=true;
//                if(iPick==false &&                                            //例外處理-軟體有資料但硬體沒有
//                   MOT[MUnloadRobotZ].fHasTray &&
//                   Sen[SnUnloadRobotHaveCassette1].IsOff() &&
//                   Sen[SnUnloadRobotHaveCassette2].IsOff())
//                {
//                    SetCassetteTray(MMUnloadPort1+iPortTarget,MUnloadRobotZ); //Port ===> Robot
//                    Task=700;
//                }
//                else
//                {
//                    Task=100;
//                }
//            }
            break;
        case 50:
            if(Cylinder[C_UnloadRobotX].Pop())                       //UnloadRobotX 縮
            {
                Task=1;
            }
            break;
        case 100:   //移動到 iPortTarget 取 Cassette 位置
            if(bPick)
            {
                iPos=Prod.iUnloadRobotZ[iPortTarget];
            }
            else
            {
                iPos=Prod.iUnloadRobotZ[iPortTarget]+Prod.iUnloadRobotUpDownSensor;
            }
            bMoveZ=MOT[MUnloadRobotZ].MotorMove(iPos);
            if(bMoveZ)
            {
                Task=200;
            }
            break;
        case 200:   //Check Sensor
            if(bPick)
            {
                #ifndef SOFT_SIMULTE
                //檢查高度是否安全
                if(Sen[SnUnloadRobotPosDown].IsOn()==false)
                {
                    //ShowMyMessage("請檢查 UnloadRobot 高度位置是否會干涉或檢查 SnUnloadRobotPosUp Sensor");
                    ret=ShowErrorMessage("JAM2320", K_SKIP|K_RETRY, MUnloadRobotZ, false, "DoUnloadRobotCassettePick_200_Pick1");
                    if(ret==K_SKIP)
                    {
                        Task=5000;
                    }
                    break;
                }
                #endif
                //從 UnloadPort 取 Cassette 到 UnloadRobot，檢查 UnloadRobot 是否有 Cassette
                if(LastSet.iRealDummy!=DUMMY &&
                  (Sen[SnUnloadRobotHaveCassette1].IsOn() || Sen[SnUnloadRobotHaveCassette2].IsOn()))
                {
                    //ShowMyMessage("請手動移除 UnloadRobot 上的 Cassette 或檢查 UnloadRobot Sensor");
                    ret=ShowErrorMessage("JAM2320", K_SKIP|K_RETRY, MUnloadRobotZ, false, "DoUnloadRobotCassettePick_200_Pick2");
                    if(ret==K_SKIP)
                    {
                        Task=5000;
                    }
                    break;
                }
            }
            else
            {

                #ifndef SOFT_SIMULTE
                //檢查高度是否安全
                if(Sen[SnUnloadRobotPosUp].IsOn()==false)
                {
                    //ShowMyMessage("請檢查 UnloadRobot 高度位置是否會干涉或檢查 SnUnloadRobotPosDown Sensor");
                    ret=ShowErrorMessage("JAM2320", K_SKIP|K_RETRY, MUnloadRobotZ, false, "DoUnloadRobotCassettePick_200_Place1");
                    if(ret==K_SKIP)
                    {
                        Task=5000;
                    }
                    break;
                }
                 #endif
                //從 UnloadRobot 放 Cassette 到 UnloadPort，檢查 UnloadPort 是否有 Cassette
                iPortSensor=SnUnloadPortHaveCassette1_1+iPortTarget*2;
                if(LastSet.iRealDummy!=DUMMY &&
                  (Sen[iPortSensor].IsOn() || Sen[iPortSensor+1].IsOn()))
                {
                    //ShowMyMessage("請手動移除 UnloadPort 上的 Cassette 或檢查 UnloadPort Sensor");
                    ret=ShowErrorMessage("JAM2320", K_SKIP|K_RETRY, MUnloadRobotZ, false, "DoUnloadRobotCassettePick_200_Place2");
                    if(ret==K_SKIP)
                    {
                        Task=5000;
                    }
                    break;
                }
            }
            Task=300;
            break;
        case 300:   //inital UnloadRobotX 準備伸
            Cylinder[C_UnloadRobotX].Reset();
            Task=400;
            break;
        case 400:   //UnloadRobotX 伸
            if(Cylinder[C_UnloadRobotX].Push())
            {
                bMoveZ=false;
                Task=500;
            }
            break;
        case 500:   //取放 Cassette
            if(bPick)
            {
                iPos=Prod.iUnloadRobotZ[iPortTarget]+Prod.iUnloadRobotUpDownSensor;
            }
            else
            {
                iPos=Prod.iUnloadRobotZ[iPortTarget];
            }
            //bMoveZ=MOT[MUnloadRobotZ].MotorMove(iPos);
             bMoveZ=MOT[MUnloadRobotZ].MotorMove2SpeedForMR(iPos, &MRSpeed[UnloadRobotZ]);
            if(bMoveZ)
            {
                DoUnloadRobotCassettePickDelay.SetSecAndOn(0.1);
                Task=550;
            }
            break;
        case 550:
            if(DoUnloadRobotCassettePickDelay.Off())
            {
                Task=600;
            }
            break;
        case 600:   //Check Sensor
            if(bPick)
            {
                #ifndef SOFT_SIMULTE
                //檢查高度是否安全
                if(Sen[SnUnloadRobotPosUp].IsOn()==false)
                {
                    //ShowMyMessage("請檢查 UnloadRobot 高度位置是否會干涉或檢查 SnUnloadRobotPosUp Sensor");
                    ret=ShowErrorMessage("JAM2320", K_RETRY, MUnloadRobotZ, false, "DoUnloadRobotCassettePick_600_Pick1");
                    break;
                }
                 #endif
                //檢查在 UnloadRobot 上的 Cassette 是否放好
                if(LastSet.iRealDummy!=DUMMY &&
                  (Sen[SnUnloadRobotHaveCassette1].IsOff() || Sen[SnUnloadRobotHaveCassette2].IsOff()))
                {
                    //ShowMyMessage("請檢查 UnloadRobot 上的 Cassette 是否置偏或檢查 UnloadRobot Sensor");
                    ret=ShowErrorMessage("JAM2320", K_RETRY, MUnloadRobotZ, false, "DoUnloadRobotCassettePick_600_Pick2");
                    break;
                }
                SetCassetteTray(MUnloadRobotZ,MMUnloadPort1+iPortTarget); //Port ===> Robot
            }
            else
            {
                #ifndef SOFT_SIMULTE
                //檢查高度是否安全
                if(Sen[SnUnloadRobotPosDown].IsOn()==false)
                {
                    //ShowMyMessage("請檢查 UnloadRobot 高度位置是否會干涉或檢查 SnUnloadRobotPosDown Sensor");
                    ret=ShowErrorMessage("JAM2320", K_RETRY, MUnloadRobotZ, false, "DoUnloadRobotCassettePick_600_Place1");
                    break;
                }
                #endif
                //檢查在 UnloadPort 上的 Cassette 是否放好
                iPortSensor=SnUnloadPortHaveCassette1_1+iPortTarget*2;
                if(LastSet.iRealDummy!=DUMMY &&
                  (Sen[iPortSensor].IsOff() || Sen[iPortSensor+1].IsOff()))
                {
                    //ShowMyMessage("請檢查 UnloadRobot 上的 Cassette 是否置偏或檢查 UnloadRobot Sensor");
                    ret=ShowErrorMessage("JAM2320", K_RETRY, MUnloadRobotZ, false, "DoUnloadRobotCassettePick_600_Place2");
                    break;
                }
                SetCassetteTray(MMUnloadPort1+iPortTarget,MUnloadRobotZ); //Port ===> Robot
            }
            Task=700;
            break;
        case 700:  //inital UnloadRobotX 準備縮
            Cylinder[C_UnloadRobotX].Reset();
            Task=800;
            break;
        case 800: //UnloadRobotX 縮
            if(Cylinder[C_UnloadRobotX].Pop())
            {
                Task=5000;
            }
            break;
        case 5000:
            Task=1;
            return true;
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
int iWhichUnloadPortPlace()   //return 0~3
{
    int i,ret,iPortSensor;
    ret=-1;
    for(i=0; i<4; i++)
    {

        #ifndef SOFT_SIMULTE
        iPortSensor=SnUnloadPortHaveCassette1_1+i*2;
        if(LastSet.iRealDummy!=DUMMY && (Sen[iPortSensor].IsOn()==false && Sen[iPortSensor+1].IsOn()==false) ||
          (LastSet.iRealDummy==DUMMY && MOT[MMUnloadPort1+i].fHasTray==false))
        {
            return i;
        }
        #else
        if(MOT[MMUnloadPort1+i].fHasTray==false)
        {
            return i;
        }
        #endif
    }
    return ret;
}
//------------------------------------------------------------------------------
int iGetMMCABuffer(int BufferTarget)   // 0~9 MR==>LM   MMCABuffer1==>MLoadRobotZ   MMCABuffer10==>MUnloadRobotZ
{
    int iMMCABuffer;
    iMMCABuffer=MMCABuffer1+BufferTarget;
    if(USE_MR_SYSTEM==2)
    {
        if(BufferTarget==0)
        {
            iMMCABuffer=MLoadRobotZ;
        }
        else if(BufferTarget==9)
        {
            iMMCABuffer=MUnloadRobotZ;
        }
    }
    return iMMCABuffer;
}
//<==
//Sam 20190112 LM
//------------------------------------------------------------------------------
bool MTrayBracketCanSafeMove()
{
    if(MOT[MStackedTrayX].ReadPos()<=(54270) &&
       Cylinder[C_TrayBracketUpOff].OnSensor())
        return true;
    return false;
}
//------------------------------------------------------------------------------
int iTrayBracketTask=1;
void InitialTrayBracketTask()
{
    iTrayBracketTask=1;
}
//------------------------------------------------------------------------------
void DoTrayBracket()
{
    int &Task=iTrayBracketTask;
    static int iRFIDRetry=0;
    int ret;
    switch(Task)
    {
        case 1:
            if(MTrayBracketCanSafeMove()==false)                                //判斷是否可以移動
            {
                if(Cylinder[C_TrayBracketUpOff].OnSensor()==false)
                {
                    C_Catch_Cassette_Pop(3, C_TrayBracketUpOn, true);           //inital 夾取
                    Task=50;
                }
            }
            else
            {
                Task=100;
            }
            break;
        case 50:
            if(C_Catch_Cassette_Pop(3, C_TrayBracketUpOn))                      //inital 夾取
            {
                Task=1;
            }
            break;
        case 100:
            if(MOT[MTrayBracketZ].MotorMove(Prod.iTrayBracketZ[0]))             //等待位置
            {
                Task=150;
            }
            break;
        case 150:
            if(MOT[MTrayBracketZ].fHasTray && Sen[SnTrayBracketHave].IsOn())
            {
                InitialTrayBracketUpTask();                                     //結合
                Task=300;
            }
            else if(MOT[MTrayBracketZ].fHasTray && Sen[SnTrayBracketHave].IsOn()==false &&
                    bBuffer6CombineTray)
            {
                InitialTrayBracketUpTask();                                     //結合
                bTrayBracketNoTray=true;
                Task=300;
            }
            else if(MOT[MTrayBracketZ].fHasTray==false && Sen[SnTrayBracketHave].IsOn() &&
                    bBuffer6SeparationTray)
            {
                InitialTrayBracketDownTask();                                   //分tray
                bTrayBracketNoTray=true;
                Task=200;
            }
            else
            {
                InitialTrayBracketDownTask();                                   //分tray
                Task=200;
            }
            break;
        case 200:
            if(bCheckCassetteFinish)
            {
                if(LastSet.iRealDummy==DUMMY)
                {
                    if(MOT[MMCABuffer6].Tray.HasEmptyCassette() &&
                       (MOT[MMTrayConversion].Tray.Data[0][0]==HAS_CASSETTE_FAIL ||
                        MOT[MMTrayConversion].Tray.Data[0][0]==HAS_CASSETTE_PASS ||
                        (MOT[MMTrayConversion].Tray.Data[0][0]==HAS_CASSETTE_TRAY && bEmptyFull)))
                    {
                        InitialTrayBracketUpTask();                                 //結合
                        bBuffer6CombineTray=true;
                        bCheckCassetteFinish=false;
                        Task=300;
                    }
                    //else if(MOT[MMCABuffer6].Tray.HasICCassette() && MOT[MMTrayConversion].HasIC()==false && (bLoadNeedTray || bEmptyNeedTray))
                    else if(MOT[MMCABuffer6].Tray.HasICCassette() && MOT[MMTrayConversion].HasIC()==false && (MOT[MMCABuffer6].Tray.Data[0][0]==HAS_CASSETTE_DEVICE || MOT[MMCABuffer6].Tray.Data[0][0]==HAS_CASSETTE_TRAY))
                    {
                        InitialTrayBracketDownTask();                               //分tray
                        bBuffer6SeparationTray=true;
                        bCheckCassetteFinish=false;
                        Task=300;
                    }
                }
                else
                {
                    if(Sen[SnBuffer6HaveCassette].IsOn() && MOT[MMCABuffer6].Tray.HasEmptyCassette() &&
                       (MOT[MMTrayConversion].Tray.Data[0][0]==HAS_CASSETTE_FAIL ||
                        MOT[MMTrayConversion].Tray.Data[0][0]==HAS_CASSETTE_PASS ||
                        (MOT[MMTrayConversion].Tray.Data[0][0]==HAS_CASSETTE_TRAY && bEmptyFull)))
                    {
                        InitialTrayBracketUpTask();                                 //結合
                        bBuffer6CombineTray=true;
                        bCheckCassetteFinish=false;
                        Task=300;
                    }
                    else if(Sen[SnBuffer6HaveCassette].IsOn() && MOT[MMCABuffer6].Tray.HasICCassette() && MOT[MMTrayConversion].HasIC()==false &&
                            (bLoadNeedTray || bEmptyNeedTray))
                    {
                        InitialTrayBracketDownTask();                               //分tray
                        bBuffer6SeparationTray=true;
                        bCheckCassetteFinish=false;
                        Task=300;
                    }
                }
            }
            break;
        case 300:
            if(TestIF_File.bEnableRFID)
            {
                fRFID->bRFIDReaderOK[1]=false;
                iRFIDRetry=0;
                #ifndef SOFT_SIMULTE
                fRFID->RFID_2->StopComm();
                fRFID->RFID_2->StartComm();
                #endif
                Task=310;
            }
            else
            {
                Task=500;
            }
            break;
        case 310:
            fRFID->DoRFIDReader(1);
            DoRFIDRead2Delay.SetMSAndOn(TestIF_File.iRFIDDelay);
            Task=320;
            break;
        case 320:
            if(fRFID->bRFIDReaderOK[1])
            {
                iRFIDRetry=0;
//                MOT[MMCABuffer6].Tray.cCassetteLot=fCassette->Edit1->Text;
                Task=500;
            }
            else if(fRFID->cbRFIDSimulate->Checked)
            {
                AnsiString str="";
                str.sprintf("%d",(rand()%1000)+1000);
                asBuffer6CassetteID=str;
                MOT[MMCABuffer6].Tray.cCassetteID=str;
//                MOT[MMCABuffer6].Tray.cCassetteLot=fCassette->Edit1->Text;
                iRFIDRetry=0;
                Task=500;
            }
            else if(DoRFIDRead2Delay.Off())
            {
                if(iRFIDRetry<TestIF_File.iRFIDRetryCount)
                {
                    iRFIDRetry++;
                    Task=310;
                }
                else
                {
                    ret=ShowErrorMessage("JAM2341", K_RETRY|K_SKIP, MMCABuffer6, false, "DoTrayBracket_320");
                    if(ret==K_SKIP)
                    {
                        Task=500;
                    }
                    else
                    {
                        #ifndef SOFT_SIMULTE
                        fRFID->RFID_2->StopComm();
                        fRFID->RFID_2->StartComm();
                        #endif
                        Task=310;
                    }
                    iRFIDRetry=0;
                }
            }
            break;
        case 500:
            if(bSTMove==false)
            {
                Task=600;
            }
            break;
        case 600:
            //if(bCheckCassetteFinish)
            {
                if(bBuffer6CombineTray)
                {
                    Task=1000;

                }
                else
                {
                    Task=2000;
                }
            }
            break;
        case 1000:
            if(DoTrayBracketUp())
            {
                Task=3000;
            }
            break;
        case 2000:
            if(DoTrayBracketDown())
            {
                Task=3000;
            }
            break;
        case 3000:
            if(MOT[MTrayBracketZ].MotorMove(Prod.iTrayBracketZ[0]))             //下降安全高度
            {
                MOT[MStackedTrayX].fCanMove=true;
                if(bBuffer6CombineTray)
                {
                    bBuffer6CombineTray=false;
                    bEmptyFull=false;
                }
                if(bBuffer6SeparationTray)
                    bBuffer6SeparationTray=false;
                Task=1;
            }
            break;
    }
}
//------------------------------------------------------------------------------
int iTrayBracketUpTask=1;
void InitialTrayBracketUpTask()
{
    iTrayBracketUpTask=1;
}
//------------------------------------------------------------------------------
bool DoTrayBracketUp()
{
    int ret;
    int &Task=iTrayBracketUpTask;

    switch(Task)
    {
        case 1:
            if(MTrayBracketCanSafeMove()==false)                                //判斷是否可以移動
            {
                MOT[MTrayBracketZ].PCIL132_StopMotor();
                if(Cylinder[C_TrayBracketUpOff].OnSensor()==false)
                {
                    C_Catch_Cassette_Pop(3, C_TrayBracketUpOn, true);           //inital up下降
                    Task=50;
                    break;
                }
            }
            else
            {
                if(bTrayBracketNoTray)
                {
                    Task=600;
                    bTrayBracketNoTray=false;
                }
                else
                {
                    Task=30;
                }
            }
            break;
        case 30:
            if(MOT[MTrayBracketZ].MotorMove(Prod.iTrayBracketZ[0]))             //移動到安全位置
            {
                MOT[MStackedTrayX].fCanMove=false;
                C_Catch_Cassette_Push(4, C_TrayBracketOpenOn, true);            //inital 撐開 open
                Task=150;
            }
            break;
        case 50:
            if(C_Catch_Cassette_Pop(3, C_TrayBracketUpOn))                      //up下降
            {
                Task=1;
            }
            break;
//        case 100:
//            if(Sen[SnBuffer6HaveTray].IsOn())
//            {
//                ShowMyMessage("Buffer6 Cassette have tray,please check cassette", "Buffer6有Tray，請確認");
//                Task=110;
//            }
//            else
//            {
//                Task=150;
//            }
//            break;
//        case 110:
//            {
//
//
//            }
//            break;
        case 150:
            if(C_Catch_Cassette_Push(4, C_TrayBracketOpenOn))                   //撐開 open
            {
                SetCassetteTray(MTrayBracketZ, MMTrayConversion);
                Task=300;
            }
            break;
        case 300:
            if(MTrayBracketCanSafeMove()==false)                                //判斷是否可以移動
            {
                break;
            }

            if(MOT[MTrayBracketZ].MotorMove(Prod.iTrayBracketZ[1]-1000))        //移動buffer6位置
            {
                C_Catch_Cassette_Push(3, C_TrayBracketUpOn, true);              //inital up升高
                Task=400;
            }
            break;
        case 400:
            if(C_Catch_Cassette_Push(3, C_TrayBracketUpOn))                     //up升高
            {
                Task=450;
            }
            break;
        case 450:
            if(MOT[MTrayBracketZ].MotorMove(Prod.iTrayBracketZ[1]))             //移動buffer6位置
            {
                C_Catch_Cassette_Pop(4, C_TrayBracketOpenOn, true);             //inital 撐開close
                Task=550;
            }
            break;
        case 550:
            if(C_Catch_Cassette_Pop(4, C_TrayBracketOpenOn))                    //撐開close
            {
                C_Catch_Cassette_Pop(3, C_TrayBracketUpOn, true);               //inital up下降
                Task=580;
            }
            break;
        case 580:
            if(C_Catch_Cassette_Pop(3, C_TrayBracketUpOn))                      //up下降
            {
                Task=600;
            }
            break;
        case 600:
            #ifndef SOFT_SIMULTE
            if(LastSet.iRealDummy==DUMMY || Sen[SnBuffer6HaveTray].IsOn())      //判斷seneor有沒有亮
            #endif
            {
                MOT[MTrayBracketZ].Tray.cCassetteID=MOT[MMCABuffer6].Tray.cCassetteID;
                if(IniConfig.bEnable_SECS_GEM==true)
                {
                    if(MOT[MTrayBracketZ].Tray.Data[0][0]==HAS_CASSETTE_PASS)
                        EventReport(SECS_EVENT.ReadyToCombinePass);       //wei 20181018
                    else if(MOT[MTrayBracketZ].Tray.Data[0][0]==HAS_CASSETTE_FAIL)
                        EventReport(SECS_EVENT.ReadyToCombineFail);       //wei 20181018
                }
                SetCassetteTray(MMCABuffer6, MTrayBracketZ);
                bBuffer6NoNeedCassette=true;
//                MOT[MMCABuffer6].SetTray(MOT[MTrayBracketZ].Tray.Data[0][0]);
//                MOT[MTrayBracketZ].ClearTray();
                Task=700;
            }
            #ifndef SOFT_SIMULTE
            else
            {
                Task=610; //alarm
            }
            #endif
            break;
        case 610:
            ret=ShowErrorMessage("JAM2324", K_RETRY|K_SKIP, MMCABuffer6, false, "DoTrayBracketUp_610");       //20180820補alarm code

            if(ret==K_RETRY)
            {
                Task=600;
            }
            else if(ret==K_SKIP)
            {

            }
            break;
        case 700:
            if(MOT[MTrayBracketZ].MotorMove(Prod.iTrayBracketZ[0]))             //下降安全高度
            {
//                bBuffer6NeedCassette=false;
//                bBuffer6NoNeedCassette=true;
                bBuffer6CanUp=false;
                Task=1;
                return true;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
int iTrayBracketDownTask=1;
void InitialTrayBracketDownTask()
{
    iTrayBracketDownTask=1;
}
//------------------------------------------------------------------------------
bool DoTrayBracketDown()
{
    int ret;
    int &Task=iTrayBracketDownTask;
    static int iAlarmCount=0;

    switch(Task)
    {
        case 1:
            if(MTrayBracketCanSafeMove()==false)                                //判斷是否可以移動
            {
                MOT[MTrayBracketZ].PCIL132_StopMotor();
                Task=200;
                break;
            }
            else
            {
                if(bTrayBracketNoTray)
                {
                    Task=550;
                    bTrayBracketNoTray=false;
                }
                else
                {
                    MOT[MStackedTrayX].fCanMove=false;
                    Task=300;
                }
            }
            break;
        case 200:
            if(MOT[MTrayBracketZ].MotorMove(Prod.iTrayBracketZ[0]))             //移動到安全位置
            {
                Task=1;
            }
            break;
        case 300:
            if(MOT[MTrayBracketZ].MotorMove(Prod.iTrayBracketZ[1]-1000))        //移動到抓取位置
            {
                C_Catch_Cassette_Push(3, C_TrayBracketUpOn, true);              //inital up上升
                Task=400;
            }
            break;
        case 400:
            if(C_Catch_Cassette_Push(3, C_TrayBracketUpOn))                     //up上升
            {
                Task=450;
            }
            break;
        case 450:
            if(MOT[MTrayBracketZ].MotorMove(Prod.iTrayBracketZ[1]))             //移動到抓取位置
            {
                C_Catch_Cassette_Push(4, C_TrayBracketOpenOn, true);            //inital 撐開 open
                Task=460;
            }
            break;
        case 460:
            if(C_Catch_Cassette_Push(4, C_TrayBracketOpenOn))                   //撐開 open
            {
                C_Catch_Cassette_Pop(3, C_TrayBracketUpOn, true);               //inital up下降
                Task=500;
            }
            break;
        case 500:
            if(C_Catch_Cassette_Pop(3, C_TrayBracketUpOn))                      //up下降
            {
                Task=550;
                iAlarmCount=0;
            }
            break;
        case 550:
            #ifndef SOFT_SIMULTE
            if(LastSet.iRealDummy==DUMMY || Sen[SnTrayBracketHave].IsOn())      //判斷seneor有沒有亮
            #endif
            {
                if(IniConfig.bEnable_SECS_GEM==true)
                {
                    if(MOT[MMCABuffer6].Tray.Data[0][0]==HAS_CASSETTE_DEVICE)
                        EventReport(SECS_EVENT.ReadyToCarrierOutLot);       //wei 20181018
                    else if(MOT[MMCABuffer6].Tray.Data[0][0]==HAS_CASSETTE_TRAY)
                        EventReport(SECS_EVENT.ReadyToCarrierOutTray);      //wei 20181018
                }
                MOT[MTrayBracketZ].SetTray(MOT[MMCABuffer6].Tray.Data[0][0]);
                MOT[MMCABuffer6].SetTray(HAS_CASSETTE_EMPTY);
                MOT[MTrayBracketZ].Tray.cCassetteLot=MOT[MMCABuffer6].Tray.cCassetteLot;
                MOT[MMCABuffer6].Tray.cCassetteLot="";
                ShowCassetteTrayData();
//                C_Catch_Cassette_Pop(4, C_TrayBracketOpenOn, true);             //inital 撐開 close
                Task=590;
                iAlarmCount=0;
            }
            #ifndef SOFT_SIMULTE
            else
            {
                iAlarmCount++;
                if(iAlarmCount>20)
                {
                    Task=560;  //alarm
                    iAlarmCount=0;
                }
            }
            #endif
            break;
        case 560:
            ret=ShowErrorMessage("JAM2323", K_RETRY, MTrayBracketZ, false, "DoTrayBracketDown_560");              //20180820補alarm code

            if(ret==K_RETRY)
            {
                Task=550;
            }
//            else if(ret==K_SKIP)
//            {
//                Task=550;
//            }
            break;
        case 590:
            if(MOT[MTrayBracketZ].MotorMove(Prod.iTrayBracketZ[0]))
            {
                Task=1000;
            }

            if(MOT[MTrayBracketZ].ReadPos()<=(Prod.iTrayBracketZ[1]/2))
            {
                bBuffer6NoNeedCassette=true;
            }
            break;
        case 1000:
            #ifndef SOFT_SIMULTE
            if(LastSet.iRealDummy==DUMMY || Sen[SnTrayBracketHave].IsOn())      //判斷seneor有沒有亮
            #endif
            {
                SetCassetteTray(MMTrayConversion, MTrayBracketZ);
//                bBuffer6NoNeedCassette=true;
                Task=1100;
            }
            #ifndef SOFT_SIMULTE
            else
            {
                Task=1050;  //alarm
            }
            #endif
            break;
        case 1050:
            ret=ShowErrorMessage("JAM2323", K_RETRY, MMTrayConversion, false, "DoTrayBracketDown_1050");       //20180820補alarm code

            if(ret==K_RETRY)
            {
                Task=1000;
            }
//            else if(ret==K_SKIP)
//            {
//                Task=1000;
//            }
            break;
        case 1100:
            if(MOT[MTrayBracketZ].MotorMove(Prod.iTrayBracketZ[0]))             //移動到安全位置
            {
                if(MOT[MMTrayConversion].Tray.Data[0][0]==HAS_CASSETTE_DEVICE)
                    bLoadNeedTray=false;
                if(MOT[MMTrayConversion].Tray.Data[0][0]==HAS_CASSETTE_TRAY)
                    bEmptyNeedTray=false;

                Task=1;
                return true;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
bool MStackedXCanSafeMove()
{
    if(MoveCatchArmZToSafe(MStackedTrayZ) &&                                    //上升到安全高度
       MOT[MTrayBracketZ].ReadPos()==Prod.iTrayBracketZ[0] &&
       Cylinder[C_TrayBracketUpOff].OnSensor())
        return true;
    return false;
}
//------------------------------------------------------------------------------
int iSTCatchFromBufferTask=1;
void InitialSTCatchFromBufferTask()
{
    iSTCatchFromBufferTask=1;
}
//------------------------------------------------------------------------------
bool DoSTCatchFromBuffer(int BufferTarget)
{
    int ret;
    int &Task=iSTCatchFromBufferTask;

    switch(Task)
    {
        case 1:
            if(MStackedXCanSafeMove()==false)                                   //判斷是否可以移動
            {
                MOT[MStackedTrayX].PCIL132_StopMotor();
                Task=50;
                break;
            }
            else
            {
                if(bBuffer6SeparationTray==false && bBuffer6CombineTray==false)
                {
                    MOT[MTrayBracketZ].fCanMove=false;
                    if(bStackedTrayNoTray)
                    {
                        bStackedTrayNoTray=false;
                        Task=700;
                    }
                    else
                    {
                        Task=10;
                    }
                }
            }
            break;
        case 10:
            if(Sen[SnStackedTrayHave].IsOn() && Cylinder[C_StackedTrayCatchOn].OnSensor())
            {
                Task=600;
            }
            else
            {
                Task=30;
            }
            break;
        case 30:
            if(StackedTrayXMovePos(Prod.iStackedTrayX[BufferTarget]))           //移動到指定buffer
            {
                C_Catch_Cassette_Pop(6, C_StackedTrayLockOn, true);             //initial lock open
//                if(bBuffer6NeedCassette==false && MOT[MMCABuffer6].HasIC()==false && Sen[SnBuffer6HaveCassette].IsOn()==false)
//                    bBuffer6NeedCassette=true;
                Task=100;
            }
            break;
        case 50:
            if(MoveCatchArmZToSafe(MStackedTrayZ))                              //上升到安全高度
            {
                Task=1;
            }
            break;
        case 100:
            if(C_Catch_Cassette_Pop(6, C_StackedTrayLockOn))                    //lock open
            {
                Task=150;
            }
            break;
        case 150:
            if(Cylinder[C_StackedTrayCatchOff].OnSensor()==false)               //判斷Catch是否打開
            {
                C_Catch_Cassette_Pop(5, C_StackedTrayCatchOn, true);            //initial 放開
                Task=200;
            }
            else
            {
                Task=250;
            }
            break;
        case 200:
            if(C_Catch_Cassette_Pop(5, C_StackedTrayCatchOn))                  //放開
            {
                Task=150;
            }
            break;
        case 250:
            if(MOT[MStackedTrayZ].MotorMove(Prod.iStackedTrayZ[BufferTarget]))  //下降到buffer
            {
                C_Catch_Cassette_Push(5, C_StackedTrayCatchOn, true);           //initial 夾取
                Task=300;
            }
            break;
        case 300:
            if(C_Catch_Cassette_Push(5, C_StackedTrayCatchOn))                  //夾取
            {
                Task=400;
            }
            break;
        case 400:
            if(MOT[MStackedTrayZ].MotorMove(Prod.iStackedTrayZ[BufferTarget]+1000))//下降高於buffer
            {
                C_Catch_Cassette_Push(6, C_StackedTrayLockOn, true);             //initial lock close
                Task=500;
            }
            break;
        case 500:
            if(C_Catch_Cassette_Push(6, C_StackedTrayLockOn))                    // lock close
            {
//                if(bBuffer6NeedCassette==false && MOT[MMCABuffer6].HasIC()==false && Sen[SnBuffer6HaveCassette].IsOn()==false)
//                    bBuffer6NeedCassette=true;
                Task=550;
            }
            break;
        case 550:
            #ifndef SOFT_SIMULTE
            if(LastSet.iRealDummy==DUMMY || Sen[SnStackedTrayHave].IsOn())  //判斷buffer區 seneor有沒有亮
            #endif
            {
                Task=600;
            }
            #ifndef SOFT_SIMULTE
            else
            {
                C_Catch_Cassette_Pop(6, C_StackedTrayLockOn, true);             //initial lock close
                Task=800;
            }
            break;
            #endif
        case 600:
            if(MoveCatchArmZToSafe(MStackedTrayZ))                              //上升到安全高度
            {
                Task=700;
            }
            break;
        case 700:
            #ifndef SOFT_SIMULTE
            if(LastSet.iRealDummy==DUMMY || (bCheckSensor(BufferTarget)==false && Sen[SnStackedTrayHave].IsOn()))  //判斷buffer區 seneor有沒有亮
            #endif
            {
                SetCassetteTray(MStackedTrayZ, MMTrayLoader+BufferTarget);
                if(BufferTarget>=3)
                {
                    if(bART_needRT2 && BufferTarget==4)
                        iMRTrayCount[0]=iMRTrayCount[4];
                    iMRTrayCount[BufferTarget]=iMRTrayCount[BufferTarget]-20;
                    if(iMRTrayCount[BufferTarget]<=0)
                        iMRTrayCount[BufferTarget]=0;
                }
                ShowTrayCountData();
                Task=1;
                return true;
            }
            #ifndef SOFT_SIMULTE
            else
            {
                Task=750;
            }
            break;
            #endif
        case 750:
            ret=ShowErrorMessage("JAM2325", K_RETRY, MStackedTrayZ, false, "DoSTCatchFromBuffer_750");       //20180820補alarm code

            if(ret==K_RETRY)
            {
                Task=700;
            }
            break;
        case 800:
            if(C_Catch_Cassette_Pop(6, C_StackedTrayLockOn))                    //lock open
            {
                Task=810;
            }
            break;
        case 810:
            if(MOT[MStackedTrayZ].MotorMove(Prod.iStackedTrayZ[BufferTarget]))  //下降到buffer
            {
                C_Catch_Cassette_Pop(5, C_StackedTrayCatchOn, true);           //initial 夾取
                Task=820;
            }
            break;
        case 820:
            if(C_Catch_Cassette_Pop(5, C_StackedTrayCatchOn))                  //放開
            {
                Task=830;
            }
            break;
        case 830:
            if(MoveCatchArmZToSafe(MStackedTrayZ))
            {
                ret=ShowErrorMessage("JAM2325", K_RETRY, MStackedTrayZ, false, "DoSTCatchFromBuffer_360");       //20180820補alarm code
                if(ret==K_RETRY)
                {
                    Task=30;
                }
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
int iSTPlaceToBufferTask=1;
void InitialSTPlaceToBufferTask()
{
    iSTPlaceToBufferTask=1;
}
//------------------------------------------------------------------------------
bool DoSTPlaceToBuffer(int BufferTarget)
{
    int ret;
    int &Task=iSTPlaceToBufferTask;
    AnsiString S1="";

    switch(Task)
    {
        case 1:
            if(bStackedTrayNoTray)
            {
                bStackedTrayNoTray=false;
                Task=350;
            }
            else
            {
                Task=10;
            }
            break;
        case 10:
            if(MStackedXCanSafeMove()==false)                                   //判斷是否可以移動
            {
                MOT[MStackedTrayX].PCIL132_StopMotor();
                Task=50;
                break;
            }

            if(StackedTrayXMovePos(Prod.iStackedTrayX[BufferTarget]))           //移動到指定buffer
            {
                C_Catch_Cassette_Pop(6, C_StackedTrayLockOn , true);            //inital lock open
                Task=100;
            }
            break;
        case 50:
            if(MoveCatchArmZToSafe(MStackedTrayZ))                              //上升到安全高度
            {
                Task=1;
            }
            break;
        case 100:
//            if(MOT[MStackedTrayZ].MotorMove(Prod.iStackedTrayZ[BufferTarget]+3000))  //下降到位置
//            {
//                C_Catch_Cassette_Pop(6, C_StackedTrayLockOn , true);            //inital lock open
//                Task=150;
//            }
//            break;
//        case 150:
//            if(C_Catch_Cassette_Pop(6, C_StackedTrayLockOn ))                   //lock open
//            {
//                Task=200;
//            }
//            break;
//        case 200:
            if(MOT[MStackedTrayZ].MotorMove(Prod.iStackedTrayZ[BufferTarget]))  //下降到位置
            {
                C_Catch_Cassette_Pop(5, C_StackedTrayCatchOn , true);           //inital lock open
                Task=300;
            }

            if(MOT[MStackedTrayZ].ReadPos()<=Prod.iStackedTrayZ[BufferTarget]+5000)
            {
                C_Catch_Cassette_Pop(6, C_StackedTrayLockOn );
            }
            break;
        case 300:
            if(C_Catch_Cassette_Pop(5, C_StackedTrayCatchOn ))                  //lock open
            {
                Task=350;
            }
            break;
        case 350:
            #ifndef SOFT_SIMULTE
            if(LastSet.iRealDummy==DUMMY || bCheckSensor(BufferTarget))         //判斷軌道seneor有沒有亮
            #endif
            {
                SetCassetteTray(MMTrayLoader+BufferTarget, MStackedTrayZ);
                Task=400;
            }
            #ifndef SOFT_SIMULTE
            else
            {
                Task=500;
            }
            #endif
            break;
        case 400:
            if(MoveCatchArmZToSafe(MStackedTrayZ))                              //上升到安全高度
            {
                Task=1;
                return true;
            }
            break;
        case 500:
            S1.sprintf("JAM23%02d", 26+BufferTarget);   //JAM2326 JAM2327 JAM2328 JAM2329 JAM2330 JAM2331
            ret=ShowErrorMessage(S1, K_RETRY, MMTrayLoader+BufferTarget, false, "DoSTPlaceToBuffer_500");
            if(ret==K_RETRY)
            {
                Task=350;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
bool bCheckSensor(int Target)
{
    int SenIndex[6]={SnLoaderTrayHasTray, SenEmptySelectHasTray, SnTrayBracketHave,
                     SnAuto1TrayHasTray, SnAuto2TrayHasTray, SnAuto3TrayHasTray};

    if(Sen[SenIndex[Target]].IsOn())
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
int WhichAutoCanCatch()
{
    for(int i=0; i<3; i++)
    {
        if(MOT[MMTrayAuto1+i].Tray.Data[0][0]==HAS_CASSETTE_PASS ||
           MOT[MMTrayAuto1+i].Tray.Data[0][0]==HAS_CASSETTE_FAIL)
        {
            return i+3;
        }
    }
    return 0;
}
//==============================================================================
HTimer CheckEmptyDelay;
int iCatchStackedTrayTask=1;
void InitialCatchStackedTrayTask()
{
    iCatchStackedTrayTask=1;
}
//------------------------------------------------------------------------------
void DoCatchStackedTray()
{
    static int Target=0;
    int &Task=iCatchStackedTrayTask;
    AnsiString str1="", str2="", S1="";

    switch(Task)
    {
        case 1:
            if(MoveCatchArmZToSafe(MStackedTrayZ))                              //上升到安全高度
            {
                Task=10;
            }
            break;
        case 10:
            if(MStackedXCanSafeMove()==false)                                   //判斷X可不可以移動
            {
                MOT[MStackedTrayX].PCIL132_StopMotor();
                Task=1;
                break;
            }

            if(StackedTrayXMovePos(54270)) //等待位置
            {
                Task=20;
            }
            break;
        case 20:                                                                //判斷原本上面是否有Tray
            if(MOT[MStackedTrayZ].fHasTray && Sen[SnStackedTrayHave].IsOn())                                  //有Tray就去放
//               ((LastSet.iRealDummy!=DUMMY && Sen[SnStackedTrayHave].IsOn()) ||
//                (LastSet.iRealDummy==DUMMY )))
            {
                InitialSTPlaceToBufferTask();                                   //放
                Task=1000;
            }
            else if(MOT[MStackedTrayZ].fHasTray && Sen[SnStackedTrayHave].IsOn()==false)
            {
                bStackedTrayNoTray=true;
                InitialSTPlaceToBufferTask();                                   //放
                Task=1000;
            }
            else if(MOT[MStackedTrayZ].fHasTray==false && Sen[SnStackedTrayHave].IsOn())
            {
                bStackedTrayNoTray=true;
                InitialSTCatchFromBufferTask();                                   //放
                Task=100;
            }
            else
            {
                InitialSTCatchFromBufferTask();                                 //夾
                Task=100;
            }
            break;
        case 100:
            if(MOT[MMTrayConversion].Tray.Data[0][0]==HAS_CASSETTE_DEVICE)      //有Device
            {
                Target=2;
                Task=300;
            }
            else if(MOT[MMTrayConversion].Tray.Data[0][0]==HAS_CASSETTE_TRAY && bEmptyFull==false)   //有Empty
            {
                Target=2;
                Task=300;
            }
            else if(MOT[MMTrayLoader].Tray.Data[0][0]==HAS_CASSETTE_DEVICE && bMRQAMode) //QA Mode
            {
                Target=0;
                Task=300;
            }
            else if((MOT[MMTrayConversion].Tray.Data[0][0]==NULL_IC && Sen[SnTrayBracketHave].IsOn()==false) &&       //Auto1~3有Tray要出
                    (MOT[MTrayBracketZ].Tray.Data[0][0]==NULL_IC )    &&                  //&& Sen[SnStackedTrayHave].IsOn()==false
                    WhichAutoCanCatch()>=3 && bART_needRT2==false && bLoadNeedTray==false && bEmptyNeedTray==false)
            {
                Target=WhichAutoCanCatch();
                Task=200;
            }
            else if(MOT[MMTrayEmpty].Tray.Data[0][0]==HAS_CASSETTE_TRAY && bEmptyFull &&
                    ((LastSet.iRealDummy!=DUMMY && Sen[SnEmptyIsFull].IsOn()) || LastSet.iRealDummy==DUMMY))   //待確認 empty 滿
            {
                Target=1;
                Task=300;
            }
            break;
        case 200:
            #ifndef SOFT_SIMULTE
            if(PitchCylinderState[Target-3]==0)
            {
                bSTArmMove[Target-3]=true;
                Task=300;
            }
            #else
            Task=300;
            #endif
            break;
        case 300:
//            if(bCheckSensor(Target)==false)
//            {
//                S1.sprintf("JAM23%02d", 35+Target);
//                ret=ShowErrorMessage(S1, K_RETRY|K_SKIP, MStackedTrayZ, false, "DoCatchStackedTray_300");
//                if(ret==K_RETRY)
//                {
//                    Task=300;
//                }
//                else if(ret==K_SKIP)
//                {
//                    MOT[MMTrayLoader+Target].Tray.SetData(NULL_IC);
//                    Task=1;
//                }
//            }
//            else
//            {
//                Task=400;
//            }
//            break;
//        case 400:
            bSTMove=true;
            if(DoSTCatchFromBuffer(Target))
            {
                if(Target==1)
                {
                    iMRTrayCount[1]=0;
                    MOT[MMTrayEmpty].Tray.SetData(HAS_CASSETTE_TRAY);
                }
                else if(Target==0)
                {
                    MOT[MMTrayEmpty].Tray.SetData(NULL_IC);
                }
                else if(Target>=3)
                {
                    bSTArmMove[Target-3]=false;
//                    if(MOT[MMCABuffer6].HasIC()==false)
//                        bBuffer6NeedCassette=true;
                }
                ShowCassetteTrayData();
                ShowTrayCountData();
                Task=500;
            }
            if(Target>=3)
            {
                if(bBuffer6NeedCassette==false && MOT[MMCABuffer6].HasIC()==false && Sen[SnBuffer6HaveCassette].IsOn()==false)
                    bBuffer6NeedCassette=true;
            }
            break;
        case 500:
            if(MOT[MStackedTrayZ].fHasTray &&                                   //有Tray就去放
               ((LastSet.iRealDummy!=DUMMY && Sen[SnStackedTrayHave].IsOn()) ||
                (LastSet.iRealDummy==DUMMY )))
            {
                InitialSTPlaceToBufferTask();
                Task=1000;
            }
            else
            {
                //alarm 遺失
            }
            break;
        case 1000:
            if(MOT[MStackedTrayZ].Tray.Data[0][0]==HAS_CASSETTE_DEVICE && bMRQAMode==false)  //device
            {
                Target=0;
                Task=1050;
            }
            else if(MOT[MStackedTrayZ].Tray.Data[0][0]==HAS_CASSETTE_TRAY && bEmptyFull==false)   //空tray
            {
                Target=1;
                Task=1050;
            }
            else if(MOT[MStackedTrayZ].Tray.Data[0][0]==HAS_CASSETTE_PASS       ||     //PASS OR FAIL
                    MOT[MStackedTrayZ].Tray.Data[0][0]==HAS_CASSETTE_FAIL       )
            {
                Target=2;
                Task=1050;
            }
            else if(MOT[MStackedTrayZ].Tray.Data[0][0]==HAS_CASSETTE_TRAY && bEmptyFull)  //滿空tray
            {
                Target=2;
                Task=1050;
            }
            else if(MOT[MStackedTrayZ].Tray.Data[0][0]==HAS_CASSETTE_DEVICE && bMRQAMode)     //QA mode
            {
                Target=3;
                Task=1050;
                bMRQAMode=false;
            }
            else if(((LastSet.iRealDummy!=DUMMY && Sen[SnStackedTrayHave].IsOn()) || LastSet.iRealDummy==DUMMY) &&
                     MOT[MStackedTrayZ].Tray.Data[0][0]==NULL_IC)
            {
                MOT[MStackedTrayZ].fHasTray=false;
                MOT[MTrayBracketZ].fCanMove=true;
                Task=1;
            }
            break;
        case 1050:
            if(bCheckSensor(Target))
            {
                str1.sprintf("Please remove Tray form %s !", SenIndexName[Target]);
                str2.sprintf("請從%s移除Tray盤!", SenIndexName[Target]);
                ShowMyMessage(str1.c_str(), str2.c_str());
            }
            else
            {
                Task=1100;
            }
            break;
        case 1100:
            if(DoSTPlaceToBuffer(Target))                                       //去指定的buffer放
            {
                if(Target==0)
                {
                    bLoadHasStackedTray=true;
                    #ifdef SOFT_SIMULTE
                    iMRTrayCount[0]=2;
                    #else
                    iMRTrayCount[0]=20;
                    #endif
                    asLoaderLotID=MOT[MMTrayLoader].Tray.cCassetteLot;
                    if(HasICUnderMachine()==false)         // && HasAnyICInMachine()==false        // bInitalForm==false
                    {
                        fLotInfo->edtSysLotID->Text=MOT[MMTrayLoader].Tray.cCassetteLot;
                        asNowLotID=fLotInfo->edtSysLotID->Text;
                    }

                    if(bNewLotHotplate || bNewLotLoader)
                        asNextLotID=MOT[MMTrayLoader].Tray.cCassetteLot;

                    if(USE_MR_SYSTEM && IniConfig.bEnable_SECS_GEM==true)   // && bSECSGEMConnectionFail==false
                    {
                        if((bMRAutoMode || fCassette->cbManualAutoMode->Checked) && bMRAutoFTLotStart==false)
                        {
                            if(bMRAutoFTLotStart==false && iATRFtRtMode==0)
                            {
                                bMRAutoFTLotStart=true;
                                bMRAutoLotStart=true;
                                fLotInfo->spSECSLotCheckClick(fLotInfo);
                                fLotInfo->labSecsGemStatus->Caption="FT Load has tray,wait EA(LOTSTART)";
                                RecordProcess("FT Load has tray,wait EA(LOTSTART)");
                                if(fCassette->cbManualAutoMode->Checked)
                                    bMRAutoLotStart=false;
                            }
                        }
                    }
                }
                else if(Target==1)
                {
                    bEmptyHasStackedTray=true;
                    #ifdef SOFT_SIMULTE
                    iMRTrayCount[1]=10;
                    #else
                    iMRTrayCount[1]=20;
                    #endif
                }
                else if(Target==2)
                {
                    if(bBuffer6NeedCassette==false && MOT[MMCABuffer6].HasIC()==false && Sen[SnBuffer6HaveCassette].IsOn()==false)
                        bBuffer6NeedCassette=true;
                }
                else if(Target==3)
                {
                    iMRTrayCount[3]=iMRTrayCount[8]+iMRTrayCount[3];
                }
                ShowTrayCountData();
                Task=1200;
            }
            break;
        case 1200:
            if(MStackedXCanSafeMove()==false)                                    //判斷X可不可以移動
            {
                MOT[MStackedTrayX].PCIL132_StopMotor();
                Task=1250;
                break;
            }

            if(StackedTrayXMovePos(54270)) //等待區域
            {
                MOT[MTrayBracketZ].fCanMove=true;
                bSTMove=false;
                if(bBuffer6NeedCassette)
                {
                    Task=1300;
                    break;
                }
                Task=1;
            }
            break;
        case 1250:
            if(MoveCatchArmZToSafe(MStackedTrayZ))                              //上升到安全高度
            {
                Task=1200;
            }
            break;
        case 1300:
            if(MOT[MMCABuffer6].Tray.Data[0][0]==HAS_CASSETTE_EMPTY &&
               (MOT[MMTrayConversion].Tray.Data[0][0]==HAS_CASSETTE_PASS ||
                MOT[MMTrayConversion].Tray.Data[0][0]==HAS_CASSETTE_FAIL))
            {
                Task=1;
            }
            else
            {
                if(bBufferHasOldCassette==false &&
                   WhichBufferCanCatch(HAS_CASSETTE_EMPTY)==0 && MOT[MCasArmZ].Tray.Data[0][0]!=HAS_CASSETTE_EMPTY)
                {
//                    ShowMyMessage("Please put empty cassette to Loadport", "請放空卡匣到LoadPort");
                    bCassetteMessage=true;
                    ShowCassetteMessage("Please put empty cassette to Loadport", "請放空卡匣到LoadPort");

                    bBuffer6NeedEmpty=true;
                    Task=1400;
                }
                else if(WhichBufferCanCatch(HAS_CASSETTE_EMPTY)!=0)
                {
                    Task=1;
                }
            }
            break;
        case 1400:
            if((LastSet.iRealDummy!=DUMMY && Sen[SnLoadPortPresent].IsOn() && Sen[SnLoadPortPlacement1].IsOn() &&
               Sen[SnLoadPortPlacement2].IsOn()) || LastSet.iRealDummy==DUMMY)
            {
                bCassetteMessage=false;
                Task=1;
            }
            else
            {
                if(bBufferHasOldCassette==false)
                {
//                    ShowMyMessage("Please put empty cassette to Loadport", "請放空卡匣到LoadPort");
                    bCassetteMessage=true;
                    ShowCassetteMessage("Please put empty cassette to Loadport", "請放空卡匣到LoadPort");
                }

            }
            break;
    }
}
//------------------------------------------------------------------------------
int ATRCatchStackedTrayTask=1;
void InitialATRCatchStackedTrayTask()
{
    ATRCatchStackedTrayTask=1;
}
//------------------------------------------------------------------------------
bool DoATRCatchStackedTray()
{
    static int Target=0;
    int &Task=ATRCatchStackedTrayTask;
    AnsiString str1="", str2="";

    switch(Task)
    {
        case 1:
            if(MoveCatchArmZToSafe(MStackedTrayZ))                              //上升到安全高度
            {
                Task=10;
            }
            break;
        case 10:
            if(MStackedXCanSafeMove()==false)                                   //判斷X可不可以移動
            {
                MOT[MStackedTrayX].PCIL132_StopMotor();
                Task=1;
                break;
            }

            if(StackedTrayXMovePos(54270))             //等待位置
            {
                InitialSTCatchFromBufferTask();
                Task=100;
            }
            break;
        case 100:
            if(MOT[MMTrayAuto1+iNeedATR].Tray.Data[0][0]==HAS_CASSETTE_FAIL)             //ATR
            {
                Target=3+iNeedATR;
                Task=300;
            }
//            if(MOT[MMTrayAuto2].Tray.Data[0][0]==HAS_CASSETTE_FULLDEVICE)
//            {
//                Target=4;
//                Task=300;
//            }
//            else if(MOT[MMTrayAuto1].Tray.Data[0][0]==HAS_CASSETTE_FULLDEVICE)
//            {
//                Target=3;
//                Task=300;
//            }
//            else if(MOT[MMTrayAuto3].Tray.Data[0][0]==HAS_CASSETTE_FULLDEVICE)
//            {
//                Target=5;
//                Task=300;
//            }
            break;
        case 300:
            if(DoSTCatchFromBuffer(Target))
            {
                Task=500;
            }
            break;
        case 500:
            if(MOT[MStackedTrayZ].fHasTray &&                                   //有cassette就去放
               (LastSet.iRealDummy==DUMMY || Sen[SnStackedTrayHave].IsOn()))
            {
                Task=1000;
            }
            else
            {
                //遺失
            }
            break;
        case 1000:
            if(MOT[MMTrayLoader].Tray.HasIC()==false &&
               (LastSet.iRealDummy==DUMMY || Sen[SnLoaderTrayHasTray].IsOn()==false))
            {
                Target=0;
                InitialSTPlaceToBufferTask();
                Task=1100;
            }
            else
            {
                Task=1050;
            }
            break;
        case 1050:
            if(bCheckSensor(Target))
            {
                str1.sprintf("Please remove Tray form %s !", SenIndexName[Target]);
                str2.sprintf("請從%s移除Tray盤!", SenIndexName[Target]);
                ShowMyMessage(str1.c_str(), str2.c_str());
                Task=1000;
            }
            else
            {
                Task=1100;
            }
            break;
        case 1100:
            if(DoSTPlaceToBuffer(Target))                                       //去空的buffer放
            {
                if(Target==0)
                {
                    bLoadHasStackedTray=true;
                    asLoaderLotID=MOT[MMTrayLoader].Tray.cCassetteLot;
                    fLotInfo->edtSysLotID->Text=MOT[MMTrayLoader].Tray.cCassetteLot;
                }
                Task=1200;
            }
            break;
        case 1200:
            if(MStackedXCanSafeMove()==false)                                    //判斷X可不可以移動
            {
                MOT[MStackedTrayX].PCIL132_StopMotor();
                Task=1250;
                break;
            }

            if(StackedTrayXMovePos(54270))                      //等待區域
            {
                MOT[MTrayBracketZ].fCanMove=true;
                Task=1;
                return true;
            }
            break;
        case 1250:
            if(MoveCatchArmZToSafe(MStackedTrayZ))                              //上升到安全高度
            {
                Task=1200;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
int CassetteToUnLoadPortTask=1;
void InitialCassetteToUnLoadPortTask()
{
    CassetteToUnLoadPortTask=1;
}
//------------------------------------------------------------------------------
bool DoCassetteToUnLoadPort()
{
    static int Target=0;
    int &Task=CassetteToUnLoadPortTask;

    switch(Task)
    {
        case 1:
            if(MoveCatchArmZToSafe(MCasArmZ))                                   //上升到安全高度
            {
                Task=10;
            }
            break;
        case 10:
            if(MCasArmXCanSafeMove()==false)                                    //判斷z在上
            {
                MOT[MCasArmX].PCIL132_StopMotor();
                Task=1;
                break;
            }

            if(MOT[MCasArmX].ReadPos()>=Prod.iCassetteArmX[2])
            {
                Task=50;
            }
            else
            {
                Task=30;
            }
            break;
        case 30:
            if(CatchArmXMovePos(Prod.iCassetteArmX[2]))                         //等待位置
            {
                Task=10;
            }
            break;
        case 50:
            if(MOT[MCasArmZ].fHasTray &&                                        //有cassette就去放
               ((LastSet.iRealDummy!=DUMMY && Sen[SnCassetteArmHave].IsOn()) ||
                 LastSet.iRealDummy==DUMMY))
            {
                InitialCAPlaceToBufferTask();
                Task=1000;
            }
            else
            {
                InitialCACatchFromBufferTask();
                Task=100;
            }
            break;
        case 100:
//            if(MOT[MMCABuffer1].HasIC()==false)
            {
                if((bMRAutoMode==false || fCassette->cbManualAutoMode->Checked==true) && bNewLotHotplate==false && bNewLotLoader==false)
                {
                    if(WhichBufferCanCatch(HAS_CASSETTE_FAIL))      //20181011
                    {
                        Target=WhichBufferCanCatch(HAS_CASSETTE_FAIL);
                    }
                    else if(WhichBufferCanCatch(HAS_CASSETTE_PASS))
                    {
                        Target=WhichBufferCanCatch(HAS_CASSETTE_PASS);
                    }
                    else
                    {
                        Target=0;
                    }
                }
                else
                {
                    Target=0;
                }

                if(Target!=0)
                {
//                    if(Target<=1)
//                    {
//
//                    }
//                    else
                    {
                        Task=300;
                    }
                }
                else
                {
                    Task=1;
                    return true;
                }
            }
//            else if(MOT[MMCABuffer1].HasIC())
//            {
//                bBufferHasOldCassette=true;
//                Task=1200;
//            }
            break;
        case 300:
            if(MCasArmXCanSafeMoveBuffer1()==true)                              //判斷z在上
            {
                Task=400;
            }
            break;
        case 400:
            if(DoCACatchFromBuffer(Target))
            {
                Task=500;
            }
            break;
        case 500:
            if(MOT[MCasArmZ].fHasTray &&
               ((LastSet.iRealDummy!=DUMMY && Sen[SnCassetteArmHave].IsOn()) ||
                 LastSet.iRealDummy==DUMMY))                                    //有cassette就去放
            {
                InitialCAPlaceToBufferTask();
                Target=0;
                Task=600;
            }
            break;
        case 600:
            if(MOT[MMCABuffer1].HasIC()==false)
            {
                Task=700;
            }
            break;
        case 700:
            if(MCasArmXCanSafeMoveBuffer1()==true)                              //判斷z在上
            {
                Task=1000;
            }
            break;
        case 1000:
            if(DoCAPlaceToBuffer(Target))                                       //去空的buffer放
            {
//                bBufferHasOldCassette=true;
                Task=1200;
            }
            break;
        case 1200:
            if(MCasArmXCanSafeMove()==false)                                    //判斷z在上
            {
                MOT[MCasArmX].PCIL132_StopMotor();
                Task=1250;
                break;
            }

//            if(CatchArmXMovePos(Prod.iCassetteArmX[5]))                         //等待區域
//            {
//                Task=1500;
//            }
//            break;

            if(MOT[MCasArmX].ReadPos()>=Prod.iCassetteArmX[2])
            {
                bBufferHasOldCassette=true;
                Task=1;
            }
            else
            {
                Task=1230;
            }
            break;
        case 1230:
            if(CatchArmXMovePos(Prod.iCassetteArmX[2]))                         //等待位置
            {
                Task=1200;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
bool DoNotMRStop()
{
    bool bCheckPos[6]={false,false,false,false,false,false};

    if(USE_MR_SYSTEM==1)
    {
        bCheckPos[0]=MOT[MCaselevatorZ].MotorMove(10);
    }
    else
    {
        bCheckPos[0]=true;
    }
    bCheckPos[2]=MoveCatchArmZToSafe(MCasArmZ);                                 //上升到安全高度
    if(bCheckPos[2])
        bCheckPos[1]=MOT[MCasArmX].MotorMove(Prod.iCassetteArmX[5]);

    bCheckPos[3]=MOT[MTrayBracketZ].MotorMove(Prod.iTrayBracketZ[0]);
    if(bART_needRT2)
    {
        bCheckPos[4]=true;
        bCheckPos[5]=true;
    }
    else
    {
        bCheckPos[5]=MoveCatchArmZToSafe(MStackedTrayZ);                        //上升到安全高度
        if(bCheckPos[5])
            bCheckPos[4]=MOT[MStackedTrayX].MotorMove(54270);

    }

    if(bCheckPos[0] && bCheckPos[1] &&
       bCheckPos[2] && bCheckPos[3] &&
       bCheckPos[4] && bCheckPos[5] )
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool CheckMRSafePos()
{
    bool bCheckPos[6]={false};

    bCheckPos[0]=(MOT[MCaselevatorZ].ReadPos()==Prod.iLoadPortZ[1]);

    MOT[MCasArmZ].ScanMotorStatus();                                            //上升到安全高度
    bCheckPos[2]=(MOT[MCasArmZ].ReadPos()>=0 && MOT[MCasArmZ].Led[iHomeLed]==true);
    if(bCheckPos[2])
        bCheckPos[1]=(MOT[MCasArmX].ReadPos()>=Prod.iCassetteArmX[2]);

    bCheckPos[3]=(MOT[MTrayBracketZ].ReadPos()==Prod.iTrayBracketZ[0]);

    MOT[MStackedTrayZ].ScanMotorStatus();                                       //上升到安全高度
    bCheckPos[5]=(MOT[MStackedTrayZ].ReadPos()>=0 && MOT[MStackedTrayZ].Led[iHomeLed]==true);
    if(bCheckPos[5])
        bCheckPos[4]=(MOT[MStackedTrayX].ReadPos()==(54270));


    if(bCheckPos[0] && bCheckPos[1] &&
       bCheckPos[2] && bCheckPos[3] &&
       bCheckPos[4] && bCheckPos[5] )
    {
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
int iManualCatchCassetteTask=1;
void InitialManualCatchCassetteTask()
{
    iManualCatchCassetteTask=1;
}
//------------------------------------------------------------------------------
void DoManualCatchCassette()
{
    static int Target=0;
    int &Task=iManualCatchCassetteTask;
    int ret=0;
    static int iBuffer=0;

    switch(Task)
    {
        case 1:
            if(MoveCatchArmZToSafe(MCasArmZ))                                   //上升到安全高度
            {
                Task=10;
            }
            break;
        case 10:
            if(MCasArmXCanSafeMove()==false)                                    //判斷z在上
            {
                MOT[MCasArmX].PCIL132_StopMotor();
                Task=1;
                break;
            }

            if(MOT[MCasArmX].ReadPos()>=Prod.iCassetteArmX[2])                  //buffer2安全位置後
            {
                Task=50;                                                        //在安全位置
            }
            else
            {
                Task=30;                                                        //沒在安全位置
            }
            break;
        case 30:
            if(MCasArmXCanSafeMove()==false)                                    //判斷z在上
            {
                MOT[MCasArmX].PCIL132_StopMotor();
                Task=1;
                break;
            }

            if(CatchArmXMovePos(Prod.iCassetteArmX[2]))                         //buffer2安全位置
            {
                Task=10;
            }
            break;
        case 50:
            if(MOT[MCasArmZ].fHasTray &&                                        //有cassette就去放
               ((LastSet.iRealDummy!=DUMMY && Sen[SnCassetteArmHave].IsOn()) ||
                 LastSet.iRealDummy==DUMMY))
            {
                InitialCAPlaceToBufferTask();                                   //放
                Task=1000;
            }
            else
            {
                InitialCACatchFromBufferTask();                                   //夾
                Task=100;
            }
            break;
        case 100:
            if(bManualCatchCassetteDown[0]==true && bManualCatchCassetteDown[1]==false)
            {
                if(iDownNumber==0)
                    iBuffer=atoi(fCassette->Edit2->Text.c_str())-1;
                else
                    iBuffer=iDownNumber;

                if(Sen[SnLoadPortPresent].IsOn()==false &&
                   Sen[SnLoadPortPlacement1].IsOn()==false && Sen[SnLoadPortPlacement2].IsOn()==false &&
                   Sen[SnBuffer1HaveCassette].IsOn()==false && MOT[MMCABuffer1].HasIC()==false &&
                   LastSet.iRealDummy!=DUMMY && Sen[SnBuffer1HaveCassette+iBuffer].IsOn())
                {
                    MOT[MMCABuffer1+iBuffer].SetTray(HAS_CASSETTE_EMPTY);
                    Target=iBuffer;
                    Task=200;
                }
                else
                {
                    if(Sen[SnLoadPortPresent].IsOn() || Sen[SnLoadPortPlacement1].IsOn() ||
                       Sen[SnLoadPortPlacement2].IsOn())
                    {
                        ShowMyMessage("Check Load port,Please move cassette!!");
                    }
                    if(Sen[SnBuffer1HaveCassette].IsOn())
                    {
                        ShowMyMessage("Check buffer1,Please move cassette!!");
                    }
                    if(Sen[SnBuffer1HaveCassette+iBuffer].IsOn())
                    {
                        ShowMyMessage("Check buffer no cassette!!");
                    }
                    bManualCatchCassetteDown[0]=false;
                }
            }
            else if(bManualCatchCassetteUp[0]==true && bManualCatchCassetteUp[2]==false)
            {
                if(MOT[MMCABuffer1].HasIC() && bBufferHasNewCassette && WhichBufferCanPlace()!=0 &&
                   ((LastSet.iRealDummy!=DUMMY && Sen[SnBuffer1HaveCassette].IsOn()) ||
                     LastSet.iRealDummy==DUMMY))
                {
                    Target=0;
                    Task=200;
                }
            }
            break;
        case 200:
            if(CheckHasCassetteSensor(Target))
            {
                Task=250;
            }
            else
            {
                ret=ShowErrorMessage("JAM2332", K_RETRY|K_SKIP, MMLoadPort, false, "DoCatchCassette_200");

                if(ret==K_RETRY)
                {
                    Task=200;
                }
                else
                {
                    MOT[MMCABuffer1+Target].ClearTray();
                    Task=1;
                }
            }
            break;
        case 250:
            if(Target<2)                                                        //如果要移動位置小於buffer2就判斷loadportZ位置
            {
                Task=300;
            }
            else
            {
                Task=400;
            }
            break;
        case 300:
            if(MCasArmXCanSafeMoveBuffer1()==true)                              //判斷z在上 && loadport Z等待位置
            {
                Task=400;
            }
            break;
        case 400:
            if(DoCACatchFromBuffer(Target))                                     //夾
            {
                Task=500;
            }
            break;
        case 500:
            if(MOT[MCasArmZ].fHasTray &&
               ((LastSet.iRealDummy!=DUMMY && Sen[SnCassetteArmHave].IsOn()) ||
                 LastSet.iRealDummy==DUMMY))
            {
                Task=1000;                                                      //有cassette就去放
            }
            else
            {
                Task=1;                                                         //沒有cassette就重新  //QQ待確認
            }
            break;
        case 1000:
            if(bManualCatchCassetteDown[0]==true && bManualCatchCassetteDown[1]==false)
            {
                if(MOT[MCasArmZ].Tray.HasIC() &&
                   MOT[MMCABuffer1].HasIC()==false && Sen[SnBuffer1HaveCassette].IsOn()==false &&
                   MOT[MMLoadPort].HasIC()==false && Sen[SnLoadPortPresent].IsOn()==false &&
                   Sen[SnLoadPortPlacement1].IsOn()==false && Sen[SnLoadPortPlacement2].IsOn()==false)
                {
                     Target=0;
                     bBufferHasOldCassette=true;
                }
            }
            else if(bManualCatchCassetteUp[0]==true && bManualCatchCassetteUp[2]==false)
            {
                Target=WhichBufferCanPlace();                                   //判斷哪個有空的buffer
            }
            Task=1100;
            break;
        case 1100:
            if(CheckNoCassetteSensor(Target))
            {
                Task=1150;
            }
            else
            {
                ret=ShowErrorMessage("JAM2333", K_RETRY, MMLoadPort, false, "DoCatchCassette_1100");

                if(ret==K_RETRY)
                {
                    Task=1100;
                }
            }
            break;
        case 1150:
            if(Target==0 && bBufferHasOldCassette==false)                                                       //如果沒有空的就去等待區域
            {
                Task=1200;
            }
            else if(DoCAPlaceToBuffer(Target))                                  //放
            {
                if(bManualCatchCassetteUp[0]==true)
                {
                    bManualCatchCassetteUp[2]=true;
                    bManualCatchCassetteUp[0]=false;
                    fMain->Pause();
                }
                Task=1200;
            }
            break;
        case 1200:
            if(MCasArmXCanSafeMove()==false)                                    //判斷z在上
            {
                MOT[MCasArmX].PCIL132_StopMotor();
                Task=1250;
            }
            else
            {
                bManualCatchCassetteDown[1]=true;
                Task=10;
            }
            break;
        case 1250:
            if(MoveCatchArmZToSafe(MCasArmZ))                                   //上升到安全高度
            {
                Task=1200;
            }
            break;
    }
}
//------------------------------------------------------------------------------
int iRFIDReadWaitDataTask=1;
void InitialRFIDReadWaitDataTask()
{
    iRFIDReadWaitDataTask=1;
}
//------------------------------------------------------------------------------
bool DoRFIDReadWaitData()
{
    int &Task=iRFIDReadWaitDataTask;
    static int iRFIDRetry=0;
    int ret=0;

    switch(Task)
    {
        case 1:
            if(LastSet.iRealDummy==DUMMY)
            {
                if(MOT[MMLoadPort].HasIC() && MOT[MMCABuffer1].HasIC()==false &&
                   bBufferHasOldCassette==false)
                {
                    if(IniConfig.bEnable_SECS_GEM==true)
                    {
                        EventReport(SECS_EVENT.CassetteLoadComplete);               //wei 20181018
                    }

                    if(TestIF_File.bEnableRFID)
                    {
                        fRFID->bRFIDReaderOK[0]=false;
                        iRFIDRetry=0;
                        Task=1000;
                    }
                }
            }
            else
            {
                if(Sen[SnLoadPortPresent].IsOn() && Sen[SnLoadPortPlacement1].IsOn() &&
                   Sen[SnLoadPortPlacement2].IsOn() && Sen[SnBuffer1HaveCassette].IsOn()==false &&
                   MOT[MMCABuffer1].HasIC()==false && bBufferHasOldCassette==false)
                {
                    if(Sen[SnSafeDoor11].Enable==false || (Sen[SnSafeDoor11].Enable==true && Sen[SnSafeDoor11].IsOn()==true))
                    {
                        if(IniConfig.bEnable_SECS_GEM==true)
                        {
                            EventReport(SECS_EVENT.CassetteLoadComplete);               //wei 20181018
                        }
                        if(TestIF_File.bEnableRFID)
                        {
                            fRFID->bRFIDReaderOK[0]=false;
                            iRFIDRetry=0;
                            #ifndef SOFT_SIMULTE
                            fRFID->RFID_1->StopComm();
                            fRFID->RFID_1->StartComm();
                            #endif
                            fLotInfo->labSecsGemStatus->Caption="Read RFID...";
                            RecordProcess("Read RFID...");
                            Task=1000;
                        }
                    }
                }
            }
            break;
        case 1000:
            fRFID->DoRFIDReader(0);
            DoRFIDRead1Delay.SetMSAndOn(TestIF_File.iRFIDDelay);
            Task=1100;
            break;
        case 1100:
            if(fRFID->bRFIDReaderOK[0])
            {
                iRFIDRetry=0;
                if(IniConfig.bEnable_SECS_GEM==true)
                {
                    EventReport(SECS_EVENT.CassetteIDReadComplete);      //wei 20181018
                    RFIDDataDelay.SetSecAndOn(120.0);
                    bLotDataOK=false;
                    fLotInfo->labSecsGemStatus->Caption="RFID read finish,wait EA(Cassette Data)...";
                    RecordProcess("RFID read finish,wait EA(Cassette Data)...");
                    Task=1200;
                }
            }
            else if(fRFID->cbRFIDSimulate->Checked)
            {
                AnsiString str="";
                str.sprintf("%d",(rand()%1000)+1000);
                MOT[MMLoadPort].Tray.cCassetteID=str;
                asLoadPortCassetteID=str;
                if(IniConfig.bEnable_SECS_GEM==true)
                    EventReport(SECS_EVENT.CassetteIDReadComplete);      //wei 20181018
                RFIDDataDelay.SetSecAndOn(600.0);
                bLotDataOK=false;
                fLotInfo->labSecsGemStatus->Caption="RFID read finish,wait EA(Cassette Data)...";
                RecordProcess("RFID read finish,wait EA(Cassette Data)...");
                iRFIDRetry=0;
                Task=1200;
            }
            else if(DoRFIDRead1Delay.Off())
            {
                if(iRFIDRetry<TestIF_File.iRFIDRetryCount)
                {
                    iRFIDRetry++;
                    Task=1000;
                }
                else
                {
                    ret=ShowErrorMessage("JAM2340", K_SKIP|K_RETRY, MMLoadPort, false, "DoRFIDReadWaitData_1100");
                    if(ret==K_SKIP)
                    {
                        Task=1;
                    }
                    else if(ret==K_RETRY)
                    {
                        Task=1000;
                        #ifndef SOFT_SIMULTE
                        fRFID->RFID_1->StopComm();
                        fRFID->RFID_1->StartComm();
                        #endif
                    }
                    iRFIDRetry=0;
                }
            }
            break;
        case 1200:
            if(bSECSGEMConnectionFail==false && bLotDataOK)
            {
                bRFIDReadFinish=true;
                bRFIDReadFirst1=true;
                if(asLoadPortCassetteData=="DEVICE")
                {
                    MOT[MMLoadPort].SetTray(HAS_CASSETTE_DEVICE);
                }
                else if(asLoadPortCassetteData=="EMPTYTRAY")
                {
                    MOT[MMLoadPort].SetTray(HAS_CASSETTE_TRAY);
                }
                else if(asLoadPortCassetteData=="EMPTYCASSETTE")
                {
                    MOT[MMLoadPort].SetTray(HAS_CASSETTE_EMPTY);
                }
                MOT[MMLoadPort].Tray.cCassetteLot=asLoadPortCassetteLotID;

                if(HasICUnderMachine()==false && HasAnyICInMachine()==false)
                {
                    bSecsGemCanStart=true;
                    EventReport(SECS_EVENT.MachineNoStart);                 //wei 20181018
                    fLotInfo->labSecsGemStatus->Caption="Cassette data finish,wait EA(Start)...";
                    RecordProcess("Cassette data finish,wait EA(Start)...");
                }
                else
                {
                    fLotInfo->labSecsGemStatus->Caption="";
//                    RecordProcess("");
                }
                Task=1300;
            }
            else if(RFIDDataDelay.Off())
            {
                ret=ShowErrorMessage("JAM2342", K_RETRY, MMLoadPort, false, "DoRFIDReadWaitData_1200");
                if(ret==K_RETRY)
                {
                    fRFID->bRFIDReaderOK[0]=false;
                    iRFIDRetry=0;
                    #ifndef SOFT_SIMULTE
                    fRFID->RFID_1->StopComm();
                    fRFID->RFID_1->StartComm();
                    #endif
                    Task=1000;
                }
            }
            break;
        case 1300:
            return true;
    }
    return false;
}

//Sam 20190112 LM
//==>
void DoCatchCassetteLM()
{
    static int Target=0;
    int &Task=iCatchCassetteTask;
    int iMMCABuffer;
    static int iWhichCatchBuffer=-1;
    static int iWhichPlaceBuffer=-1;
    int ret=0;
    switch(Task)
    {
        case 1:
            if(MoveCatchArmZToSafe(MCasArmZ))                                   //上升到安全高度
            {
                Task=10;
            }
            break;
        case 10:
            if(MCasArmXCanSafeMove()==false)                                    //判斷z在上
            {
                MOT[MCasArmX].PCIL132_StopMotor();
                Task=1;
                break;
            }
            Task=100;
        case 100:   //由 MCasArmZ 的狀態來決定抓放
            if(MOT[MCasArmZ].fHasTray==false)
            {
                if(CheckCAArmHasCassette()==false || LastSet.iRealDummy==DUMMY)
                {
                    //抓 Cassette 前先判斷是否需要補 Decive/Empry Tray
                    if(iTrayFeed==0 && iCleanOut==0)
                    {
                        if(bLoadNeedTray                         ==false              &&
                           CheckTrayLoaderNoTraySensor()         ==true               &&
                           MOT[MCasArmZ]        .Tray.Data[0][0]!=HAS_CASSETTE_DEVICE &&
                           MOT[MMCABuffer6]     .Tray.Data[0][0]!=HAS_CASSETTE_DEVICE &&
                           MOT[MMTrayConversion].Tray.Data[0][0]!=HAS_CASSETTE_DEVICE &&
                           MOT[MTrayBracketZ]   .Tray.Data[0][0]!=HAS_CASSETTE_DEVICE &&
                           MOT[MStackedTrayZ]   .Tray.Data[0][0]!=HAS_CASSETTE_DEVICE)
                        {
                            bLoadNeedTray=true;
                        }

                        if(bEmptyNeedTray                       ==false             &&
                           CheckTrayEmptyNoTraySensor()         ==true              &&
                           MOT[MCasArmZ]        .Tray.Data[0][0]!=HAS_CASSETTE_TRAY &&
                           MOT[MMCABuffer6]     .Tray.Data[0][0]!=HAS_CASSETTE_TRAY &&
                           MOT[MMTrayConversion].Tray.Data[0][0]!=HAS_CASSETTE_TRAY &&
                           MOT[MTrayBracketZ]   .Tray.Data[0][0]!=HAS_CASSETTE_TRAY &&
                           MOT[MStackedTrayZ]   .Tray.Data[0][0]!=HAS_CASSETTE_TRAY)
                        {
                            bEmptyNeedTray=true;
                        }

//                      if(MOT[MMTrayLoader].Tray.HasIC()==false && MOT[MCasArmZ].Tray.Data[0][0]==HAS_CASSETTE_DEVICE && bLoadNeedTray==false &&
//                         MOT[MMCABuffer6].HasIC()==false &&
//                         (LastSet.iRealDummy==DUMMY ||
//                          (LastSet.iRealDummy!=DUMMY && Sen[SnLoaderTrayHasTray].IsOn()==false && Sen[SnLoaderCarHasTray].IsOn()==false)))
//                      {
//                          bLoadNeedTray=true;
//                      }
//
//                      if(MOT[MMTrayEmpty].Tray.HasIC()==false && MOT[MCasArmZ].Tray.Data[0][0]==HAS_CASSETTE_TRAY && bEmptyNeedTray==false &&
//                         MOT[MMCABuffer6].HasIC()==false &&
//                         ((LastSet.iRealDummy==DUMMY && iMRTrayCount[1]==0) ||
//                          (LastSet.iRealDummy!=DUMMY && Sen[SenEmptySelectHasTray].IsOn()==false)))
//                      {
//                          bEmptyNeedTray=true;
//                      }
                    }
                    Task=1000;   //準備抓 Cassette
                }
                else
                {
                    //有未知的 Cassette 例外處理
                }
            }
            else
            {
                if(CheckCAArmHasCassette())
                {
                      if(MOT[MMTrayLoader].Tray.HasIC()==false && MOT[MCasArmZ].Tray.Data[0][0]==HAS_CASSETTE_DEVICE && bLoadNeedTray==false &&
                         MOT[MMCABuffer6].HasIC()==false &&
                         (LastSet.iRealDummy==DUMMY ||
                         (LastSet.iRealDummy!=DUMMY && Sen[SnLoaderTrayHasTray].IsOn()==false && Sen[SnLoaderCarHasTray].IsOn()==false)))
                      {
                          bLoadNeedTray=true;
                      }

                      if(MOT[MMTrayEmpty].Tray.HasIC()==false && MOT[MCasArmZ].Tray.Data[0][0]==HAS_CASSETTE_TRAY && bEmptyNeedTray==false &&
                         MOT[MMCABuffer6].HasIC()==false &&
                         ((LastSet.iRealDummy==DUMMY && iMRTrayCount[1]==0) ||
                          (LastSet.iRealDummy!=DUMMY && Sen[SenEmptySelectHasTray].IsOn()==false)))
                      {
                          bEmptyNeedTray=true;
                      }
                      Task=2000;  //準備放 Cassette
                }
                else
                {
                    //Cassette 不見例外處理
                }
            }
            break;
        case 1000:     //只能從 Buffer1~9  抓 Cassette
            iWhichCatchBuffer=iWhichCABufferCatch();
            if(iWhichCatchBuffer>=0 && iWhichCatchBuffer<=8)
            {
                InitialCACatchFromBufferTask();
                Task=1100;
            }
            else
            {
                Task=1;
            }
            break;
        case 1100:
            if(DoCACatchFromBuffer(iWhichCatchBuffer))
            {
                Task=1;
            }
            break;
        case 2000:  //只能從 Buffer 1~10 放 Cassette
            iWhichPlaceBuffer=iWhichCABufferPlace();
            if(iWhichPlaceBuffer>=0 && iWhichPlaceBuffer<=9)
            {
                if(iTrayFeed==0 && iCleanOut==0)
                {
//                    if(bLoadNeedTray                         ==false              &&
//                       CheckTrayLoaderNoTraySensor()         ==true               &&
//                       MOT[MCasArmZ]        .Tray.Data[0][0]!=HAS_CASSETTE_DEVICE &&
//                       MOT[MMCABuffer6]     .Tray.Data[0][0]!=HAS_CASSETTE_DEVICE &&
//                       MOT[MMTrayConversion].Tray.Data[0][0]!=HAS_CASSETTE_DEVICE &&
//                       MOT[MTrayBracketZ]   .Tray.Data[0][0]!=HAS_CASSETTE_DEVICE &&
//                       MOT[MStackedTrayZ]   .Tray.Data[0][0]!=HAS_CASSETTE_DEVICE)
//                    {
//                        bLoadNeedTray=true;
//                    }
//
//                    if(bEmptyNeedTray                       ==false             &&
//                       CheckTrayEmptyNoTraySensor()         ==true              &&
//                       MOT[MCasArmZ]        .Tray.Data[0][0]!=HAS_CASSETTE_TRAY &&
//                       MOT[MMCABuffer6]     .Tray.Data[0][0]!=HAS_CASSETTE_TRAY &&
//                       MOT[MMTrayConversion].Tray.Data[0][0]!=HAS_CASSETTE_TRAY &&
//                       MOT[MTrayBracketZ]   .Tray.Data[0][0]!=HAS_CASSETTE_TRAY &&
//                       MOT[MStackedTrayZ]   .Tray.Data[0][0]!=HAS_CASSETTE_TRAY)
//                    {
//                        bEmptyNeedTray=true;
//                    }
                }
                InitialCAPlaceToBufferTask();
                Task=2100;
            }
            else
            {
                Task=1;
            }
            break;
        case 2100:
            if(DoCAPlaceToBuffer(iWhichPlaceBuffer))
            {
                if(iTrayFeed==0 && iCleanOut==0)
                {
                    if(MOT[MMTrayLoader].Tray.HasIC()==false && MOT[MMTrayConversion].Tray.Data[0][0]!=HAS_CASSETTE_DEVICE &&
                       MOT[MStackedTrayZ].Tray.Data[0][0]!=HAS_CASSETTE_DEVICE &&
                       WhichBufferCanCatch(HAS_CASSETTE_DEVICE)!=0 && bLoadNeedTray==false &&
                       (LastSet.iRealDummy==DUMMY ||
                        (LastSet.iRealDummy!=DUMMY && Sen[SnLoaderTrayHasTray].IsOn()==false)))
                    {
                        bLoadNeedTray=true;
                    }

                    if(MOT[MMTrayEmpty].Tray.HasIC()==false && MOT[MMTrayConversion].Tray.Data[0][0]!=HAS_CASSETTE_TRAY &&
                       MOT[MStackedTrayZ].Tray.Data[0][0]!=HAS_CASSETTE_TRAY &&
                       WhichBufferCanCatch(HAS_CASSETTE_TRAY)!=0 && bEmptyNeedTray==false &&
                       ((LastSet.iRealDummy==DUMMY && iMRTrayCount[1]==0) ||
                        (LastSet.iRealDummy!=DUMMY && Sen[SenEmptySelectHasTray].IsOn()==false)))
                    {
                        bEmptyNeedTray=true;
                    }
                }
                if(bLackCassette)   //wei 20181120
                    bLackCassette=false;
                Task=2200;
            }
            break;
        case 2200:
            if(iWhichPlaceBuffer==5)    //放到 Buffer6 需要再檢查 Cassette 內的資料是否正確
            {
                if(MOT[MMCABuffer6].Tray.Data[0][0]==HAS_CASSETTE_DEVICE ||
                   MOT[MMCABuffer6].Tray.Data[0][0]==HAS_CASSETTE_TRAY)
                {
                    if(LastSet.iRealDummy==DUMMY ||
                    (  LastSet.iRealDummy!=DUMMY && Sen[SnBuffer6HaveTray].IsOn()))
                    {
                        bCheckCassetteFinish=true;      //資料正確
                        Task=1;
                    }
                    else
                    {
                        //資料錯誤 例外處理
                        ret=ShowErrorMessage("JAM2343", K_RETRY|K_SKIP, MCasArmZ, false, "DoCatchCassetteLM_2200");
                        if(ret==K_RETRY)
                        {
                            bCheckCassetteFinish=false;
                            Task=2200;
                        }
                        else
                        {
                            //將 Cassette 資料清空為 HAS_CASSETTE_EMPTY
                            if(MOT[MMCABuffer6].Tray.Data[0][0]==HAS_CASSETTE_DEVICE)
                                bLoadNeedTray=false;
                            else if(MOT[MMCABuffer6].Tray.Data[0][0]==HAS_CASSETTE_TRAY)
                                bEmptyNeedTray=false;
                            MOT[MMCABuffer6].SetTray(HAS_CASSETTE_EMPTY);
                            MOT[MMCABuffer6].Tray.cCassetteLot="";
                            bBuffer6NoNeedCassette=true;
                            bCheckCassetteFinish=false;
                            Task=1;
                        }
                    }
                }
                else if(MOT[MMCABuffer6].Tray.Data[0][0]==HAS_CASSETTE_EMPTY)
                {
                    if(Sen[SnBuffer6HaveTray].IsOn()==false)
                    {
                        bCheckCassetteFinish=true;      //資料正確
                        Task=1;
                    }
                    else
                    {
                        //資料錯誤 例外處理
                        ret=ShowErrorMessage("JAM2344", K_RETRY|K_SKIP, MCasArmZ, false, "DoCatchCassette_1190");
                        if(ret==K_RETRY)
                        {
                            bCheckCassetteFinish=false;
                            Task=2200;
                        }
                        else
                        {
                            bCheckCassetteFinish=false;
                            Task=3000;
                        }
                    }
                }
                else
                {
                    ret=ShowErrorMessage("JAM2343",  K_RETRY|K_SKIP, MCasArmZ, false, "DoCatchCassetteLM_1100");
                    if(ret==K_RETRY)
                    {
                        bCheckCassetteFinish=false;
                        Task=2200;
                    }
                    else
                    {
                        if(MOT[MMCABuffer6].Tray.Data[0][0]==HAS_CASSETTE_DEVICE)
                        {
                            bLoadNeedTray=false;
                        }
                        else if(MOT[MMCABuffer6].Tray.Data[0][0]==HAS_CASSETTE_TRAY)
                        {
                            bEmptyNeedTray=false;
                        }
                        MOT[MMCABuffer6].SetTray(HAS_CASSETTE_EMPTY);
                        MOT[MMCABuffer6].Tray.cCassetteLot="";
                        bBuffer6NoNeedCassette=true;
                        bCheckCassetteFinish=false;
                        Task=1;
                    }
                }
            }
            else
            {
                Task=1;
            }
            break;
        case 3000: //例外處理 請移除Buffer6卡匣
            if(CheckHasCassette(5))
            {
                ShowMyMessage("Please remove Buffer6 cassette!", "請移除Buffer6卡匣!");
            }
            else
            {
                MOT[MMCABuffer6].SetTray(NULL_IC);
                MOT[MMCABuffer6].Tray.cCassetteLot="";
                bBuffer6NeedCassette=true;
                Task=1;
            }
            break;
    }
}
//------------------------------------------------------------------------------
int iWhichCABufferPlace()    //0~9   Buffer1~10
{
    int iRet;
    iRet=-1;

    if(CheckCAArmHasCassette())
    {
        if(MOT[MCasArmZ].fHasTray)
        {
            //HAS_CASSETTE_EMPTY 。 HAS_CASSETTE_DEVICE 。  HAS_CASSETTE_TRAY
            if(CheckNoCassetteSensor(5) &&                                      //確認 Buffer6 是空的
              ((MOT[MCasArmZ].Tray.Data[0][0]==HAS_CASSETTE_EMPTY  && bBuffer6NeedCassette) ||
               (MOT[MCasArmZ].Tray.Data[0][0]==HAS_CASSETTE_DEVICE && bLoadNeedTray)        ||
               (MOT[MCasArmZ].Tray.Data[0][0]==HAS_CASSETTE_TRAY   && bEmptyNeedTray)))
            {
                iRet=5;                      //放到 Buffer6 準備抓去機台內使用
            }
            //HAS_CASSETTE_PASS 。 HAS_CASSETTE_FAIL。
            else if(CheckNoCassetteSensor(9) &&                                  //確認 Buffer10 是空的
                   (MOT[MCasArmZ].Tray.Data[0][0]==HAS_CASSETTE_PASS ||
                    MOT[MCasArmZ].Tray.Data[0][0]==HAS_CASSETTE_FAIL))
            {
                iRet=9;                 //放到 Buffer10(MUnloaderZ) Cassette 準備出料
            }
            else
            {
                iRet=WhichBufferCanPlace(); //放到機台上面的 Buffer2~9
            }
        }
        else
        {
            if(TestIF_File.bEnableRFID) //有 RFID 重新讀 RFID
            {
                iRet=0;      //放到 Buffer1(MLoadRobotZ) 或 Buffer6
            }
            else                       //沒有 RFID
            {
                iRet=9;      //未知的 Cassette 放到 Buffer10(MUnloadRobotZ)
            }
        }
    }
    return iRet;
}
//------------------------------------------------------------------------------
int iWhichCABufferCatch()   //0~8   Buffer1~9
{
    int iRet,i;
    int iBufferOrder[9]={4,6,3,7,2,8,1,9,0};

    if(CheckCAArmHasCassetteSensor()==false || LastSet.iRealDummy==DUMMY) //夾爪上是空的
    {
        //1. Buffer6 有 Cassette Pass/Fail Tray要先抓
        if(CheckHasCassetteSensor(5) &&
          (MOT[MMCABuffer6].Tray.Data[0][0]==HAS_CASSETTE_PASS ||
           MOT[MMCABuffer6].Tray.Data[0][0]==HAS_CASSETTE_FAIL  ||
           MOT[MMCABuffer6].Tray.Data[0][0]==HAS_CASSETTE_EMPTY && bBuffer6NoNeedCassette==true))
        {
            return 5;
        }

        //2. 抓有需求的 Cassette
        else if (CheckHasCassetteSensor(5)==false &&       //Buffer6 是完全空才能補需求 Cassette
                (bLoadNeedTray || bEmptyNeedTray || bBuffer6NeedCassette))
        {
            //2.1. 需要補空 Cassette 去接測試完的 Tray
            if(bBuffer6NeedCassette)
            {
                iRet=iMatchBufferType(HAS_CASSETTE_EMPTY);
                if(iRet!=-1)
                {
                    return iRet;
                }
            }
            //2.2. 找有待測 ic 的 Cassette
            if(bLoadNeedTray)
            {
                iRet=iMatchBufferType(HAS_CASSETTE_DEVICE);
                if(iRet!=-1)
                {
                    return iRet;
                }
            }
            //2.3. 找空Tray 的 Cassette
            if(bEmptyNeedTray) //HAS_CASSETTE_DEVICE 要優先補完
            {
                iRet=iMatchBufferType(HAS_CASSETTE_TRAY);
                if(iRet!=-1)
                {
                    return iRet;
                }
            }
        }
        //3. 最後才是去 Buffer1(LoadRobotZ) 抓
        if(CheckHasCassetteSensor(0)                        &&
           bBufferHasNewCassette==true                      &&
           MOT[MLoadRobotZ].ReadPos()==0)
        {
            return 0; //去 Buffer1(LoadRobotZ) 抓 Cassette
        }
    }
    return -1;
}
//------------------------------------------------------------------------------
int iMatchBufferType(int CassetteType) //找尋 Buffer 裡有沒有需要的
{
    int iMMCABuffer,i;
    //Buffer 尋找順序 Buffer4 -> 6 -> 3 -> 7 -> 2 -> 8 -> 1 -> 9 -> 0(MLoadRobotZ)
    //int iBufferOrder[9]={4,6,3,7,2,8,1,9,0};  MLoadRobotZ 不要看
    int iBufferOrder[8]={4,6,3,7,2,8,1,9};
    //從 Buffer 區尋找有沒有需要的 CassetteType
    for(i=0; i<8; i++)
    {
        iMMCABuffer=iGetMMCABuffer(iBufferOrder[i]);
        if(MOT[iMMCABuffer].Tray.Data[0][0]==CassetteType &&
           CheckHasCassetteSensor(iBufferOrder[i]))
        {
            return iBufferOrder[i];
        }
    }
    return -1;//找不到
}
//------------------------------------------------------------------------------
bool CheckLoadRobotHasCassette()                                                //LoadRobot HasCassette 只檢查硬體
{
    if(LastSet.iRealDummy==DUMMY ||
      (LastSet.iRealDummy!=DUMMY && Sen[SnLoadRobotHaveCassette1].IsOn() && Sen[SnLoadRobotHaveCassette2].IsOn()))
    {
        return true;
    }
    else
    {
        return false;
    }
}
//------------------------------------------------------------------------------
bool CheckLoadRobotHasCassetteSensor()                                          //LoadRobot HasCassette 檢查硬體 & 軟體
{
    if(MOT[MLoadRobotZ].fHasTray &&
      (LastSet.iRealDummy==DUMMY ||
      (LastSet.iRealDummy!=DUMMY && Sen[SnLoadRobotHaveCassette1].IsOn() && Sen[SnLoadRobotHaveCassette2].IsOn())))
      {
            return true;
      }
      else
      {
            return false;
      }
}
//------------------------------------------------------------------------------
bool CheckLoadRobotNoCassetteSensor()                                           //LoadRobot NoCassette 檢查硬體 & 軟體
{
    if(MOT[MLoadRobotZ].fHasTray==false &&
      (LastSet.iRealDummy==DUMMY ||
      (LastSet.iRealDummy!=DUMMY && Sen[SnLoadRobotHaveCassette1].IsOn()==false && Sen[SnLoadRobotHaveCassette2].IsOn()==false)))
      {
            return true;
      }
      else
      {
            return false;
      }
}
//------------------------------------------------------------------------------
bool CheckLoadRobotSensor()                                                     //LoadRobot 置偏檢查硬體
{
    if(LastSet.iRealDummy!=DUMMY && Sen[SnLoadRobotHaveCassette1].IsOn() ^ Sen[SnLoadRobotHaveCassette2].IsOn())
    {
        return false;
    }
    else
    {
        return true;
    }
}
//------------------------------------------------------------------------------
bool CheckLoadPortHasCassette(int iPort)                                        //LoadPort HasCassette 只檢查硬體
{
    int iSensor=0;
    int iMot=0;
    iSensor=SnLoadPortHaveCassette1_1+iPort*2;
    iMot=MMLoadPort1+iPort;
    if(LastSet.iRealDummy==DUMMY ||
      (LastSet.iRealDummy!=DUMMY && Sen[iSensor].IsOn() && Sen[iSensor+1].IsOn()))
    {
        return true;
    }
    else
    {
        return false;
    }
}
//------------------------------------------------------------------------------
bool CheckLoadPortHasCassetteSensor(int iPort)                                  //LoadPort HasCassette 檢查硬體 & 軟體
{
    int iSensor=0;
    int iMot=0;
    iSensor=SnLoadPortHaveCassette1_1+iPort*2;
    iMot=MMLoadPort1+iPort;
    if(MOT[iMot].fHasTray &&
      (LastSet.iRealDummy==DUMMY ||
      (LastSet.iRealDummy!=DUMMY && Sen[iSensor].IsOn() && Sen[iSensor+1].IsOn())))
      {
            return true;
      }
      else
      {
            return false;
      }
}
//------------------------------------------------------------------------------
bool CheckLoadPortNoCassetteSensor(int iPort)                                   //LoadPort NoCassette 檢查硬體 & 軟體
{
    int iSensor=0;
    int iMot=0;
    iSensor=SnLoadPortHaveCassette1_1+iPort*2;
    iMot=MMLoadPort1+iPort;
    if(MOT[iMot].fHasTray==false &&
      (LastSet.iRealDummy==DUMMY ||
      (LastSet.iRealDummy!=DUMMY && Sen[iSensor].IsOn()==false && Sen[iSensor+1].IsOn()==false)))
      {
            return true;
      }
      else
      {
            return false;
      }
}
//------------------------------------------------------------------------------
bool CheckLoadPortSensor(int iPort)                                             //LoadPort 置偏檢查硬體
{
    int iSensor;
    iSensor=SnLoadPortHaveCassette1_1+iPort*2;
    if(LastSet.iRealDummy!=DUMMY && Sen[iSensor].IsOn() ^ Sen[iSensor+1].IsOn())
    {
        return false;
    }
    else
    {
        return true;
    }
}
//------------------------------------------------------------------------------
int iCheckAllLoadPortSensor()                                                   //全部 LoadPort 置偏檢查硬體 -1~-4 。
{
    int iPort;
    for(iPort=0;iPort<4;iPort++)
    {
        if(CheckLoadPortSensor(iPort)==false)
        {
            return -(iPort+1);
        }
    }
    return 0;
}
//------------------------------------------------------------------------------
bool CheckUnloadRobotHasCassette()                                              //UnloadRobot HasCassette 只檢查硬體
{
    if(LastSet.iRealDummy==DUMMY ||
      (LastSet.iRealDummy!=DUMMY && Sen[SnUnloadRobotHaveCassette1].IsOn() && Sen[SnUnloadRobotHaveCassette2].IsOn()))
    {
        return true;
    }
    else
    {
        return false;
    }
}
//------------------------------------------------------------------------------
bool CheckUnloadRobotHasCassetteSensor()                                          //UnloadRobot HasCassette 檢查硬體 & 軟體
{
    if(MOT[MUnloadRobotZ].fHasTray &&
      (LastSet.iRealDummy==DUMMY ||
      (LastSet.iRealDummy!=DUMMY && Sen[SnUnloadRobotHaveCassette1].IsOn() && Sen[SnUnloadRobotHaveCassette2].IsOn())))
      {
            return true;
      }
      else
      {
            return false;
      }
}
//------------------------------------------------------------------------------
bool CheckUnloadRobotNoCassetteSensor()                                         //UnloadRobot NoCassette 檢查硬體 & 軟體
{
    if(MOT[MLoadRobotZ].fHasTray==false &&
      (LastSet.iRealDummy==DUMMY ||
      (LastSet.iRealDummy!=DUMMY && Sen[SnUnloadRobotHaveCassette1].IsOn()==false && Sen[SnUnloadRobotHaveCassette2].IsOn()==false)))
      {
            return true;
      }
      else
      {
            return false;
      }
}
//------------------------------------------------------------------------------
bool CheckUnloadRobotSensor()                                                   //UnloadRobot 置偏檢查硬體
{
    if(LastSet.iRealDummy!=DUMMY && Sen[SnUnloadRobotHaveCassette1].IsOn() ^ Sen[SnUnloadRobotHaveCassette2].IsOn())
    {
        return false;
    }
    else
    {
        return true;
    }
}
//------------------------------------------------------------------------------
bool CheckUnloadPortHasCassette(int iPort)                                      //UnloadPort HasCassette 只檢查硬體
{
    int iSensor=0;
    int iMot=0;
    iSensor=SnUnloadPortHaveCassette1_1+iPort*2;
    iMot=MMUnloadPort1+iPort;
    if(LastSet.iRealDummy==DUMMY ||
      (LastSet.iRealDummy!=DUMMY && Sen[iSensor].IsOn() && Sen[iSensor+1].IsOn()))
    {
        return true;
    }
    else
    {
        return false;
    }
}
//------------------------------------------------------------------------------
bool CheckUnloadPortHasCassetteSensor(int iPort)                                //UnloadPort HasCassette 檢查硬體 & 軟體
{
    int iSensor=0;
    int iMot=0;
    iSensor=SnUnloadPortHaveCassette1_1+iPort*2;
    iMot=MMUnloadPort1+iPort;
    if(MOT[iMot].fHasTray &&
      (LastSet.iRealDummy==DUMMY ||
      (LastSet.iRealDummy!=DUMMY && Sen[iSensor].IsOn() && Sen[iSensor+1].IsOn())))
      {
            return true;
      }
      else
      {
            return false;
      }
}
//------------------------------------------------------------------------------
bool CheckUnloadPortNoCassetteSensor(int iPort)                                 //UnloadPort NoCassette 檢查硬體 & 軟體
{
    int iSensor=0;
    int iMot=0;
    iSensor=SnUnloadPortHaveCassette1_1+iPort*2;
    iMot=MMUnloadPort1+iPort;
    if(MOT[iMot].fHasTray==false &&
      (LastSet.iRealDummy==DUMMY ||
      (LastSet.iRealDummy!=DUMMY && Sen[iSensor].IsOn()==false && Sen[iSensor+1].IsOn()==false)))
      {
            return true;
      }
      else
      {
            return false;
      }
}
//------------------------------------------------------------------------------
bool CheckUnloadPortSensor(int iPort)                                           //UnloadPort 置偏檢查硬體
{
    int iSensor;
    iSensor=SnUnloadPortHaveCassette1_1+iPort*2;
    if(LastSet.iRealDummy!=DUMMY && Sen[iSensor].IsOn() ^ Sen[iSensor+1].IsOn())
    {
        return false;
    }
    else
    {
        return true;
    }
}
//------------------------------------------------------------------------------
int iCheckAllUnloadPortSensor()                                                 //全部 UnloadPort 置偏檢查硬體 -1~-4
{
    int iPort;
    for(iPort=0;iPort<4;iPort++)
    {
        if(CheckUnloadPortSensor(iPort)==false)
        {
            return -(iPort+1);
        }
    }
    return 0;
}
//------------------------------------------------------------------------------
bool bCheckUnloadRobotSensor()
{
    if(Sen[SnUnloadRobotHaveCassette1].IsOn() ^ Sen[SnUnloadRobotHaveCassette2].IsOn())    //燈號不同代表有問題
    {
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------
bool CheckTrayLoaderNoTraySensor()                                              //TrayLoader NoTray 檢查硬體 & 軟體
{
    if(MOT[MMTrayLoader].Tray.HasIC()==false &&
      (LastSet.iRealDummy==DUMMY ||
      (LastSet.iRealDummy!=DUMMY && Sen[SnLoaderTrayHasTray].IsOn()==false && Sen[SnLoaderCarHasTray].IsOn()==false)))
    {
        return true;
    }
    else
    {
        return false;
    }
}
//------------------------------------------------------------------------------
bool CheckTrayEmptyNoTraySensor()                                               //TrayEmpty NoTray 檢查硬體 & 軟體
{
    if(MOT[MMTrayEmpty].Tray.HasIC()==false &&
      (LastSet.iRealDummy==DUMMY ||
      (LastSet.iRealDummy!=DUMMY && Sen[SenEmptySelectHasTray].IsOn()==false)))
    {
        return true;
    }
    else
    {
        return false;
    }
}
//------------------------------------------------------------------------------
bool CheckCAArmHasCassette()                                                    //CassetteArm HasCassette 檢查硬體
{
    if(LastSet.iRealDummy==DUMMY ||
      (LastSet.iRealDummy!=DUMMY && Sen[SnCassetteArmHave].IsOn()))
    {
        return true;
    }
    else
    {
        return false;
    }
}
//------------------------------------------------------------------------------
bool CheckCAArmHasCassetteSensor()                                              //CassetteArm HasCassette 檢查硬體 & 軟體
{
    if(MOT[MCasArmZ].fHasTray &&
      (LastSet.iRealDummy==DUMMY ||
      (LastSet.iRealDummy!=DUMMY && Sen[SnCassetteArmHave].IsOn())))
    {
        return true;
    }
    else
    {
        return false;
    }
}
//<==
//Sam 20190112 LM

