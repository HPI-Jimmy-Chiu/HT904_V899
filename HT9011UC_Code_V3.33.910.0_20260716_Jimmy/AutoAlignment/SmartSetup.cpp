#include "MachineDefine.h"
#pragma hdrstop

#include "SmartSetup.h"
#include "InOutArmZteach.h"
#include "aArmHeader.h"
#include "asendic_Loader.h"
#include "MyMotor.h"
#include "acatchtray.h"
#include "cinitial.h"
#include "main.h"
#include "asendic.h"
#include "uteach.h"
#include "mymessbox.h"
#include "note.h"
#include "cUnitConvert.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfSmartSetup *fSmartSetup;
int iDoSmartSetupProcessTask;
int iDoInOutArmZCalibrationTask;
int iDoInArmZSearchAreaTask;
int iDoOutArmZSearchAreaTask;
int iDoInArmAlignmentTask;
int iDoOutArmAlignmentTask;
int iDoLoadTrayTask;
int iDoAutoHeightTask;

int iInArmPickAndPlcaeTask;
static bool bfinishSetup=false;
//---------------------------------------------------------------------------
__fastcall TfSmartSetup::TfSmartSetup(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TfSmartSetup::MainFormSetting(int iArm)
{
    TEdit *edInArm[]={edInArmZA, edInArmZC, edInArmZE, edInArmZG,
                      edInArmZB, edInArmZD, edInArmZF, edInArmZH};

    TEdit *edOutArm[]={edOutArmZA, edOutArmZC, edOutArmZE, edOutArmZG,
                       edOutArmZB, edOutArmZD, edOutArmZF, edOutArmZH};

    TEdit *teInArm[]={Zteach->AEdit1, Zteach->AEdit3, Zteach->AEdit5, Zteach->AEdit7,
                      Zteach->AEdit2, Zteach->AEdit4, Zteach->AEdit6, Zteach->AEdit8};

    TEdit *teOutArm[]={Zteach->AOutEdit1, Zteach->AOutEdit3, Zteach->AOutEdit5, Zteach->AOutEdit7,
                       Zteach->AOutEdit2, Zteach->AOutEdit4, Zteach->AOutEdit6, Zteach->AOutEdit8};

    TEdit *edIn[]={edInZA, edInZC, edInZE, edInZG,
                   edInZB, edInZD, edInZF, edInZH};

    TEdit *edOut[]={edOutZA, edOutZC, edOutZE, edOutZG,
                    edOutZB, edOutZD, edOutZF, edOutZH};

    TEdit *edOrgInArm[]={edOrgInArmZA, edOrgInArmZC, edOrgInArmZE, edOrgInArmZG,
                         edOrgInArmZB, edOrgInArmZD, edOrgInArmZF, edOrgInArmZH};

    TEdit *edOrgOutArm[]={edOrgOutArmZA, edOrgOutArmZC, edOrgOutArmZE, edOrgOutArmZG,
                          edOrgOutArmZB, edOrgOutArmZD, edOrgOutArmZF, edOrgOutArmZH};

//    TEdit *tInArm[]={fTeach->setEditZ1A, fTeach->setEditZ1B, fTeach->setEditZ1C, fTeach->setEditZ1D,
//                     fTeach->setEditZ1E, fTeach->setEditZ1F, fTeach->setEditZ1G, fTeach->setEditZ1H};
//
//    TEdit *tOutArm[]={fTeach->setEditZ2A, fTeach->setEditZ2B, fTeach->setEditZ2C, fTeach->setEditZ2D,
//                      fTeach->setEditZ2E, fTeach->setEditZ2F, fTeach->setEditZ2G, fTeach->setEditZ2H};

    int tInArm[8]={Tech.iInArmZHeightSub[0][0], Tech.iInArmZHeightSub[0][1], Tech.iInArmZHeightSub[0][2], Tech.iInArmZHeightSub[0][3],
                   Tech.iInArmZHeightSub[1][0], Tech.iInArmZHeightSub[1][1], Tech.iInArmZHeightSub[1][2], Tech.iInArmZHeightSub[1][3]};

    int tOutArm[8]={Tech.iOutArmZHeightSub[0][0], Tech.iOutArmZHeightSub[0][1], Tech.iOutArmZHeightSub[0][2], Tech.iOutArmZHeightSub[0][3],
                    Tech.iOutArmZHeightSub[1][0], Tech.iOutArmZHeightSub[1][1], Tech.iOutArmZHeightSub[1][2], Tech.iOutArmZHeightSub[1][3]};

    TEdit *edDiffInArm[]={edDiffInArmZA, edDiffInArmZC, edDiffInArmZE, edDiffInArmZG,
                          edDiffInArmZB, edDiffInArmZD, edDiffInArmZF, edDiffInArmZH};

    TEdit *edDiffOutArm[]={edDiffOutArmZA, edDiffOutArmZC, edDiffOutArmZE, edDiffOutArmZG,
                           edDiffOutArmZB, edDiffOutArmZD, edDiffOutArmZF, edDiffOutArmZH};

    for(int i=0; i<8; i++)
    {
        if(iArm==0)
        {
            edInArm[i]->Text=teInArm[i]->Text;
            edIn[i]->Text=teInArm[i]->Text;
            edOrgInArm[i]->Text=tInArm[i];
            edDiffInArm[i]->Text=(atoi(teInArm[i]->Text.c_str())-atoi(edOrgInArm[i]->Text.c_str()));
        }
        else if(iArm==1)
        {
            edOutArm[i]->Text=teOutArm[i]->Text;
            edOut[i]->Text=teOutArm[i]->Text;
            edOrgOutArm[i]->Text=tOutArm[i];
            edDiffOutArm[i]->Text=(atoi(teOutArm[i]->Text.c_str())-atoi(edOrgOutArm[i]->Text.c_str()));
        }
        else if(iArm==3)
        {
            edInArm[i]->Text=0;
            edIn[i]->Text=0;
            edOrgInArm[i]->Text=0;
            edDiffInArm[i]->Text=0;
        }
        else if(iArm==4)
        {
            edOutArm[i]->Text=0;
            edOut[i]->Text=0;
            edOrgOutArm[i]->Text=0;
            edDiffOutArm[i]->Text=0;
        }
    }
}
//---------------------------------------------------------------------------
void TfSmartSetup::EachAreaSetting(int iArm)
{
    TEdit *edInCurrentArea[]={edCurrentLoader, edCurrentHP1, edCurrentHP2, edCurrentInSH1, edCurrentInSH2};

    TEdit *edInArmArea[]={edLoader, edHP1, edHP2, edInSH1, edInSH2};

    TEdit *edInOrgArea[]={edOrgLoader, edOrgHP1, edOrgHP2, edOrgInSH1, edOrgInSH2};

    TEdit *edInDiffArea[]={edDiffLoader, edDiffHP1, edDiffHP2, edDiffInSH1, edDiffInSH2};

    TEdit *edInArea[]={fTeach->SetEditPickLoader, fTeach->SetEditPlaceInShuttle, fTeach->SetEditPlaceInShuttle,
                       fTeach->SetEditHP, fTeach->SetEditHP};

    int tInArm[8]={Tech.iInArmLoadStagePickZ2, Tech.iInArmShuttlePlaceZ, Tech.iInArmShuttlePlaceZ,
                   Tech.iInArmPlatePickZ2, Tech.iInArmPlatePickZ2};

    TEdit *edOutCurrentArea[]={edCurrentOutSH1, edCurrentOutSH2, edCurrentAuto1, edCurrentAuto2,
                               edCurrentAuto3, edCurrentFix1, edCurrentFix2, edCurrentFix3};

    TEdit *edOutArmArea[]={edOutSH1, edOutSH2, edAuto1, edAuto2, edAuto3, edFix1, edFix2, edFix3};

    TEdit *edOutOrgArea[]={edOrgOutSH1, edOrgOutSH2, edOrgAuto1, edOrgAuto2,
                           edOrgAuto3, edOrgFix1, edOrgFix2, edOrgFix3};

    TEdit *edOutDiffArea[]={edDiffOutSH1, edDiffOutSH2, edDiffAuto1, edDiffAuto2,
                            edDiffAuto3, edDiffFix1, edDiffFix2, edDiffFix3};

    TEdit *edOutArea[]={fTeach->SetEditPickOutSht, fTeach->SetEditPickOutSht, fTeach->SetEditPlaceAuto, fTeach->SetEditPlaceAuto,
                        fTeach->SetEditPlaceAuto, fTeach->SetEditPlaceFix, fTeach->SetEditPlaceFix, fTeach->SetEditPlaceFix};

    int tOutArm[8]={Tech.iOutArmShuttlePickZ2, Tech.iOutArmShuttlePickZ2, Tech.iOutArmPlaceZ2, Tech.iOutArmPlaceZ2,
                    Tech.iOutArmPlaceZ2, Tech.iOutArmPlaceFixZ1, Tech.iOutArmPlaceFixZ1, Tech.iOutArmPlaceFixZ1};

    int iStandZ = atoi(edDiffInArmZA->Text.c_str());

    if(iArm==0)
    {
        for(int i=0; i<5; i++)
        {
            edInCurrentArea[i]->Text=atoi(edInArmArea[i]->Text.c_str())+iStandZ;
            edInOrgArea[i]->Text=tInArm[i];
            edInDiffArea[i]->Text=(atoi(edInCurrentArea[i]->Text.c_str())-atoi(edInOrgArea[i]->Text.c_str()));
        }
    }
    else
    {
        for(int i=0; i<8; i++)
        {
            edOutCurrentArea[i]->Text=edOutArmArea[i]->Text;
            edOutOrgArea[i]->Text=tOutArm[i];
            edOutDiffArea[i]->Text=(atoi(edOutCurrentArea[i]->Text.c_str())-atoi(edOutOrgArea[i]->Text.c_str()));
        }
    }
}
//---------------------------------------------------------------------------
void TfSmartSetup::RecordAction(AnsiString Str)
{
    Memo1->Lines->Add(Str);
}
//---------------------------------------------------------------------------
void TfSmartSetup::InitalInOutArmCalibrationTask()
{
    iDoInOutArmZCalibrationTask=1;
}
//---------------------------------------------------------------------------
bool TfSmartSetup::DoInOutArmZCalibration()
{
    int &Task=iDoInOutArmZCalibrationTask;

    switch(Task)
    {
        case 1:
            Zteach->AutoTeachZ(true,0,false,0);
            bSmartSetupZCalibration=true;
            if(cbByPassInArmZCalibration->Checked==true)
            {
                RecordAction("By Pass In Arm Z Calibration");
                MainFormSetting(3);
                Task=150;
            }
            else
            {
                RecordAction("Start In Arm Z Calibration");
                Task=100;
            }
            break;
        case 100:
            if(Zteach->AutoTeachZ(false,0,false,0))
            {
                RecordAction("Finish In Arm Z Calibration");
                MainFormSetting(0);
                Task=150;
            }
            break;
        case 150:
            if(cbByPassOutArmZCalibration->Checked==true)
            {
                RecordAction("By Pass Out Arm Z Calibration");
                MainFormSetting(4);
                bSmartSetupZCalibration=false;
                Task=300;
            }
            else
            {
                Zteach->AutoTeachZ(true,1,false,0);
                RecordAction("Start Out Arm Z Calibration");
                Task=200;
            }
            break;
        case 200:
            if(Zteach->AutoTeachZ(false,1,false,0))
            {
                RecordAction("Finish Out Arm Z Calibration");
                MainFormSetting(1);
                bSmartSetupZCalibration=false;
                Task=300;
            }
            break;
        case 300:
            if(MoveOutArmXY_ToFix_Tray_Full())
            {
                return true;
            }
            break;
    }
    return false;
}
//---------------------------------------------------------------------------
void TfSmartSetup::InitalInArmZSearchAreaTask()
{
    iDoInArmZSearchAreaTask=1;
}
//---------------------------------------------------------------------------
TQPF_Timer DoPlaceDeviceDelay;
//---------------------------------------------------------------------------
bool TfSmartSetup::DoInArmPickAndPlace(int iArea, bool bPick, bool bReset)
{
    int &Task=iInArmPickAndPlcaeTask;
//    bool flag[2][4];
//    if(bReset)
//    {
//        iInArmPickAndPlcaeTask=1;
//        return false;
//    }
//
//    switch(Task)
//    {
//        case 1:
//            for(i=0; i<MAX_ARM_Row; i++)
//            {
//                for(j=0; j<MAX_ARM_Col; j++)
//                {
//                    flag[i][j]=false;
//                }
//            }
//            Task=100;
//            break;
//        case 100:
//            if(iArea==3)             //Loader
//            {
//                Task=1000;
//            }
//            break;
//        case 1000:
//            for(i=0; i<MAX_ARM_Row; i++)
//            {
//                for(j=0; j<MAX_ARM_Col; j++)
//                {
//                    if(InArmSuck.Item[i][j]!=NULL_IC)
//                        flag[i*MAX_ARM_Col+j]=false;
//                    else
//
//                }
//            }
//            break;
//    }
    return false;
}
//---------------------------------------------------------------------------
//bool TfSmartSetup::DoInArmZSetValue()
//{
//
//}
//---------------------------------------------------------------------------
bool TfSmartSetup::DoInArmZSearchArea()
{
    int &Task=iDoInArmZSearchAreaTask;
    static int i, j, iCT, iArea[5]={3, 1, 2, 4, 5}; // loader sh1 sh2 hp1 hp2
    int iAreaZ[4]={Prod.ZInArm_Plate1_Place[0][0], Prod.ZInArm_Plate2_Place[0][0],
                   Prod.ZInArm_Shuttle1_Place[0][0],  Prod.ZInArm_Shuttle2_Place[0][0]};
    static bool bArea[5]={false, false, false, false, false};
    static bool bCheckHotPlate=false;
    int iShCount=0;
    static int iSuckNo=100;

    AnsiString Str;
    AnsiString asArea[5]={"Loader", "In Shuttle 1", "In Shuttle 2", "Hot plate 1", "Hot plate 2"};
    int iRow, iCol;
    GetRowCol(iRow, iCol);

    static int iSuckCount;

    if(USE_PICKER_COUNT==ep16Picker)
    {
        iSuckCount=8;
    }
    else
    {
        iSuckCount=4;
    }

    if(bHandlerPause)
        SetAllMotorSpeed(10);

    switch(Task)
    {
        case 1:                                                                 //Homing
            for(i=0; i<5; i++)
                bArea[i]=false;

            if(TestIF_File.iShuttleMode==1)                                     //Ifor 20240424 fix
            {
                if(TestIF_File.iShuttle_Sel==0)
                    bArea[2]=true;
                else
                    bArea[1]=true;
            }

            iCT=0;
            bSmartSetupInOutArmSearchArea=true;
            if(LastSet.iTemperature==Tempture_Hot)
            {
                bCheckHotPlate=true;
                LastSet.iTemperature=Tempture_Ambient;
            }
            else
            {
                bCheckHotPlate=false;
            }
            Task=100;
            break;
        case 100:                                                               //Search Area
            if(MoveInArmZToPlateSafe(Task))
            {
                Str.sprintf("Start %s Area Z Search Action",asArea[iCT]);
                RecordAction(Str);
                if(iCT==0)
                    bLoadInarmAutoHigh=true;
                Zteach->MoveInOutArmXYToTeachPos(iArea[iCT], 0, true);
                Task=200;
            }
            break;
        case 200:
            if(Zteach->MoveInOutArmXYToTeachPos(iArea[iCT],0))
            {
                bLoadInarmAutoHigh=false;
                if(iCT>2)
                {
                    Task=250;
                }
                else
                {
                    iSuckNo=100;
                    Zteach->AutoTeachZ(true,0,true,iArea[iCT]);
                    if(iCT==0)
                    {
                        for(i=0; i<MAX_ARM_Row; i++)
                        {
                            for(j=0; j<MAX_ARM_Col; j++)
                            {
                                if(Prod.fInArmSuckUse[0][i][j] && iSuckNo==100)
                                {
                                    iSuckNo=i*iSuckCount+j;
                                }
                            }
                        }
                    }
                    Task=300;
                }
            }
            break;
        case 250:
            if(MOT[MInArmZA].MotorMove(iAreaZ[iCT-1]))
            {
                InArmSuck.Suck[0][0].Off();
                DoPlaceDeviceDelay.SetMSAndOn(1000);
                Task=260;
            }
            break;
        case 260:
            if(DoPlaceDeviceDelay.Off())
            {
                if(MoveInArmZToPlateSafe(1111)==false)
                    return false;

                InArmSuck.Suck[0][0].Normal();
                Zteach->AutoTeachZ(true,0,true,iArea[iCT]);

                if(iCT==0)
                {
                    iSuckNo=100;
                    for(i=0; i<MAX_ARM_Row; i++)
                    {
                        for(j=0; j<MAX_ARM_Col; j++)
                        {
                            if(Prod.fInArmSuckUse[0][i][j] && iSuckNo==100)
                            {
                                iSuckNo=i*iSuckCount+j;
                            }
                        }
                    }
                }
                Task=300;
            }
            break;
        case 300:
            bSmartSetupUseSingleZ=true;
            if(Zteach->AutoTeachZ(false, 0, bSmartSetupUseSingleZ,iArea[iCT], iSuckNo))
            {
                bSmartSetupUseSingleZ=false;
                Str.sprintf("End %s Area Z Search Action",asArea[iCT]);
                bArea[iArea[iCT]]=true;
                RecordAction(Str);

                iCT++;

                if(iCT>2 && LastSet.iTemperature==Tempture_Ambient)
                {
                    iCT+=2;
                }

                if(iCT<5 && bArea[iArea[iCT]]==false)
                {
                    Str.sprintf("Start %s Area Z Search Action",asArea[iCT]);
                    RecordAction(Str);
                    Task=350;
                }
                else
                {
                    EachAreaSetting(0);
                    Task=400;
                }
            }
            break;
        case 350:
            SetTechDataToProd();
            InitArmPickFromLoadStageTask();
            Task=360;
            break;
        case 360:
            if(DoInArmPickFromLoadStage_9045())
            {
                if(LastSet.iTemperature==Tempture_Hot)
                {
                    InitInArmPlaceToHotPlateTask();
                    Task=370;
                }
                else
                {
                    InitInArmPlaceToShuttleTask();
                    Task=380;
                }
                bNeedManualRemoveTray=true;                                     //Ifor 20240423add: Smart Setup Manual Remove Tray
            }
            break;
        case 370:
            if(DoPlaceToHotPlate())
            {
                Zteach->AutoTeachZ(true,0,true,iArea[iCT]);
                Task=371;
            }
            break;
        case 371:
            bSmartSetupUseSingleZ=true;
            if(Zteach->AutoTeachZ(false,0,bSmartSetupUseSingleZ,iArea[iCT]))
            {
                bSmartSetupUseSingleZ=false;
                InitInArmPickFromHotPlateTask();
                Task=372;
            }
            break;
        case 372:
            if(DoInArmPickFromHotPlate_9045(bDownSpeed))
            {
                InitInArmPlaceToShuttleTask();
                iSuckNo=100;
                for(i=0; i<InArmSuck.iMaxRow; i++)
                {
                    for(j=0; j<InArmSuck.iMaxCol; j++)
                    {
                        if(InArmSuck.Suck[i][j].GetStatus()==true && iSuckNo==100)
                        {
                            iSuckNo=i*iSuckCount+j;
                        }
                    }
                }
                Task=380;
            }
            break;
        case 380:
            if(DoInArmPlaceToShuttle_9045())
            {
                iShCount=0;
                if(iCT==1)
                    ptrInSHT=&FLCarryKit;
                else if(iCT==2)
                    ptrInSHT=&BLCarryKit;

                for(i=0; i<iRow; i++)
                {
                    for(j=0; j<iCol; j++)
                    {
                        if(ptrInSHT->Item[i][j]==NULL_IC)        //Sh1
                        {
                            iShCount++;
                        }
                    }
                }

                if(iShCount>0)
                {
                    InitArmPickFromLoadStageTask();
                    Task=360;
                }
                else
                {
                    Zteach->AutoTeachZ(true,0,true,iArea[iCT]);
                    Task=300;
                }
            }
            break;
        case 400:
            if(MoveInArmZToPlateSafe(1111)==false)
                return false;

            bSmartSetupInOutArmSearchArea=false;
            Task=600;
            MOT[MMTrayY].InitNewTray(NULL_IC);
            break;
        case 600:
            DoCatchTray();
            if(AUTO_EMPTY_COLOR)
            {
                DoAutoEmpty();
                DoAutoColor();
            }

            if(MOT[MTrayX].fHasTray==false && MOT[MMTrayY].fHasTray==false &&
               CatchTraySuck.Item[0][0]==0 && iCatchTrayControlManual==0)
                return true;
            break;
    }
    return false;
}
//---------------------------------------------------------------------------
void TfSmartSetup::InitalOutArmZSearchAreaTask()
{
    iDoOutArmZSearchAreaTask=1;
}
//---------------------------------------------------------------------------
bool TfSmartSetup::DoOutArmZSearchArea()
{
    int &Task=iDoOutArmZSearchAreaTask;
    static int  iCT, iArea[8]={1, 2, 3, 6};   // Sh1 Sh2 Auto1 Auto2 Auto3 Fix1 Fix2 Fix3
    int iAreaZ[7]={Prod.ZOutArm_Shuttle2_Pick[0][0]+50,
                   Prod.ZOutArm_Auto_Place[0][0][0],
                   Prod.ZOutArm_Auto_Place[1][0][0],
                   Prod.ZOutArm_Auto_Place[2][0][0],
                   Prod.ZOutArm_Fix_Place[0][0][0],
                   Prod.ZOutArm_Fix_Place[1][0][0],
                   Prod.ZOutArm_Fix_Place[2][0][0]};

    int iPos[2]={MMAuto1, MManualTray1};
    static bool bArea[8]={false, false, false, false, false, false, false, false};
    AnsiString Str;
    AnsiString asArea[8]={"Out Shuttle 1", "Out Shuttle 2", "Auto 1", "Auto 2", "Auto 3", "Fix 1", "Fix 2", "Fix 3"};
    int i, j, iRow, iCol, iShCount;
    static int iSuckNo=100;
    GetRowCol(iRow, iCol);

    static int iSuckCount;

    if(USE_PICKER_COUNT==ep16Picker)
    {
        iSuckCount=8;
    }
    else
    {
        iSuckCount=4;
    }

    if(bHandlerPause)
        SetAllMotorSpeed(10);

    switch(Task)
    {
        case 1:                                                                 //Homing
            for(i=0; i<8; i++)
                bArea[i]=false;

            if(TestIF_File.iShuttleMode==1)                                     //Ifor 20240424 fix
            {
                if(TestIF_File.iShuttle_Sel==0)
                    bArea[1]=true;
                else
                    bArea[0]=true;
            }

            iCT=0;
            bSmartSetupInOutArmSearchArea=true;
            Task=100;
            break;
        case 100:                                                               //Search Area
            if(MoveOutArmZToPlateSafe())
            {
                Str.sprintf("Start %s Area Z Search Action",asArea[iCT]);
                RecordAction(Str);
                Zteach->MoveInOutArmXYToTeachPos(iArea[iCT],1,true);
                Task=200;
            }
            break;
        case 200:
            if(Zteach->MoveInOutArmXYToTeachPos(iArea[iCT],1))
            {
                Zteach->AutoTeachZ(true,1,true,iArea[iCT]);
                iSuckNo=100;
                for(i=0; i<iRow; i++)
                {
                    for(j=0; j<iCol; j++)
                    {
                        if(iCT==0)
                            ptrInSHT=&FRCarryKit;
                        else if(iCT==1)
                            ptrInSHT=&BRCarryKit;

                        if(ptrInSHT->Item[i][j]!=NULL_IC && iSuckNo==100)        //Sh1
                        {
                            iSuckNo=i*iSuckCount+j;
                        }
                    }
                }

                if(iCT==2 || iCT==3)
                    iSuckNo=0;
                Task=300;
            }
            break;
        case 300:
            bSmartSetupUseSingleZ=true;
            if(Zteach->AutoTeachZ(false,1,bSmartSetupUseSingleZ,iArea[iCT], iSuckNo))
            {
                bSmartSetupUseSingleZ=false;
                bArea[iArea[iCT]]=true;
                Str.sprintf("End %s Area Z Search Action",asArea[iCT]);
                RecordAction(Str);                                              // Sh1 Sh2 Auto1 Auto2 Auto3 Fix1 Fix2 Fix3
                iCT++;

                if(iCT>=2 && iCT<4)
                {
                    if(MOT[iPos[iCT-2]].fHasTray==true)
                    {
                        if(iCT==2)
                        {
                            InitPickFromShuttle1Task();
                            Task=360;
                        }
                        else if(iCT==3)
                        {
                            InitPickFromShuttle2Task();
                            Task=370;
                        }
                        break;
                    }
                    else
                    {
                        Task=350;
                        break;
                    }
                }

                if(iCT<4 && bArea[iArea[iCT]]==false)
                {
                    Task=100;
                }
                else
                {
                    Task=500;
                }
            }
            break;
        case 350:
            iCT++;
            if(iCT>=2 && iCT<4)
            {
                if(MOT[iPos[iCT-2]].fHasTray==false)
                {
                    Task=350;
                    break;
                }
            }

            if(iCT<4 && bArea[iArea[iCT]]==false)
            {
                Task=100;
            }
            else
            {
                Task=500;
            }
            break;
        case 360:
            if(DoPickFromToOutSh1())
            {
                InitPlaceToAutoTask();
                Task=380;
            }
            break;
        case 370:
            if(DoPickFromToOutSh2())
            {
                InitPlaceToAutoTask();
                Task=380;
            }
            break;
        case 380:
            if(DoPlaceToAuto())
            {
                if(OutArmSuck.HasIC()==true)
                {
                    Task=380;
                }
                else
                {
                    iShCount=0;
                    if(iCT==2)
                        ptrInSHT=&FRCarryKit;
                    else if(iCT==3)
                        ptrInSHT=&BRCarryKit;

                    for(i=0; i<iRow; i++)
                    {
                        for(j=0; j<iCol; j++)
                        {
                            if(ptrInSHT->Item[i][j]!=NULL_IC)        //Sh1
                            {
                                iShCount++;
                            }
                        }
                    }

                    if(ptrInSHT->LeftSideNoIC(4))
                    {
                        SmartSetupWhichKit(1);
                    }

                    if(iShCount>0)
                    {
                        if(iCT==2)
                        {
                            InitPickFromShuttle1Task();
                            Task=360;
                        }
                        else if(iCT==3)
                        {
                            InitPickFromShuttle2Task();
                            Task=370;
                        }
                    }
                    else
                    {
                        Zteach->AutoTeachZ(true,1,true,iArea[iCT]);
                        SmartSetupWhichKit(0);
                        Task=100;
                    }
                }
            }
            break;
        case 500:
            if(MoveOutArmToAutoSafe()==false)
                return false;

            bSmartSetupInOutArmSearchArea=false;
            return true;
    }
    return false;
}
//---------------------------------------------------------------------------
void TfSmartSetup::InitalDoInArmAlignmentTask()
{
    iDoInArmAlignmentTask=1;
}
//---------------------------------------------------------------------------
bool TfSmartSetup::DoInArmAlignment()
{
    int &Task=iDoInArmAlignmentTask;
    static int iAlignment;
    int ret;
    AnsiString Str;
    AnsiString asArea[5]={"Loader", "Hot plate 1", "Hot plate 2", "In Shuttle 1", "In Shuttle 2"};

    switch(Task)
    {
        case 1:                                                                 //Homing
            InitInArmXYAutoTeachProcess();
            iAlignment=0;
            Task=100;
            break;
        case 100:                                                               //Search Area
            if(MoveInArmZToPlateSafe(Task))
            {
                Str.sprintf("Start %s Auto Alignment Action",asArea[iAlignment]);
                RecordAction(Str);
                Task=200;
            }
            break;
        case 200:
            ret=DoInArmXYAlignmentTeachProcess(iAlignment,false);
            if(ret==1)
            {
                Str.sprintf("End %s Auto Alignment Action",asArea[iAlignment]);
                RecordAction(Str);
                if(iAlignment==0 && LastSet.iTemperature==Tempture_Ambient)
                    iAlignment=2;
                iAlignment++;
                Task=300;
            }
            break;
        case 300:
            if(iAlignment>4)
            {
                return true;
            }
            else
            {
                InitInArmXYAutoTeachProcess();
                Task=200;
            }
            break;
    }
    return false;
}
//---------------------------------------------------------------------------
void TfSmartSetup::InitalDoOutArmAlignmentTask()
{
    iDoOutArmAlignmentTask=1;
}
//---------------------------------------------------------------------------
bool TfSmartSetup::DoOutArmAlignment()
{
    int &Task=iDoOutArmAlignmentTask;
    static int iAlignment;
    int ret;
    AnsiString Str;
    AnsiString asArea[8]={"Auto 1", "Auto 2", "Auto 3", "Fix 1", "Fix 2", "Fix 3", "Out Shuttle 1", "Out Shuttle 2"};

    switch(Task)
    {
        case 1:                                                                 //Homing
            InitOutArmXYAutoTeachProcess();
            iAlignment=0;
            Task=100;
            break;
        case 100:                                                               //Search Area
            if(MoveOutArmToAutoSafe())
            {
                Str.sprintf("Start %s Auto Alignment Action",asArea[iAlignment]);
                RecordAction(Str);
                Task=200;
            }
            break;
        case 200:
            ret=DoOutArmXYAlignmentTeachProcess(iAlignment,false);
            if(ret==1)
            {
                Str.sprintf("End %s Auto Alignment Action",asArea[iAlignment]);
                RecordAction(Str);

                iAlignment++;
                if(iAlignment<6 && bAutoAlignmentCheckHasTray(iAlignment, false)==false)
                    Task=250;
                else
                    Task=300;
            }
            break;
        case 250:
            iAlignment++;
            if(iAlignment<6)
            {
                if(bAutoAlignmentCheckHasTray(iAlignment, false)==true)
                    Task=300;
            }
            else
            {
                Task=300;
            }
            break;
        case 300:
            if(iAlignment>7)
            {
                return true;
            }
            else
            {
                if(SubMachineType==Type_HT9046LA && (iAlignment>=3 && iAlignment<6))
                {
                    iAlignment=6;
                }

                InitOutArmXYAutoTeachProcess();
                Task=100;
            }
            break;
    }
    return false;
}
//---------------------------------------------------------------------------
void TfSmartSetup::InitalDoLoadTrayTask()
{
    iDoLoadTrayTask=1;
}
//---------------------------------------------------------------------------
bool TfSmartSetup::DoLoadTray()
{
    int &Task=iDoLoadTrayTask;

    switch(Task)
    {
        case 1:
            InitLoadTask();
            InitialCatchTrayTask();
            Task=100;
            break;
        case 100:
            DoLoad();
            if(LoadTask==1 && MOT[MMTrayY].HasIC())
                Task=200;
            break;
        case 200:
            DoCatchTray();
            if(AUTO_EMPTY_COLOR)                                                //ifor 20240422 fix:
            {
                DoAutoEmpty();
                DoAutoColor();
            }

            if(WhichAutoNeedTray()==0 && iCatchTrayControlManual==0)
                return true;
            else if(LastSet.iRealDummy==HAS_TRAY || LastSet.iRealDummy==REALLY)
                IsCatchTrayReadySupplyNewTray();
            break;
    }
    return false;
}
//---------------------------------------------------------------------------
void TfSmartSetup::InitalDoAutoHeightTask()
{
    iDoAutoHeightTask=1;
}
//---------------------------------------------------------------------------
bool TfSmartSetup::DoAutoHeight()
{
    int &Task=iDoAutoHeightTask;
    static int i, iCT, iArea[4]={1, 2, 3, 6};
    static bool bArea[4]={false, false, false, false};

    switch(Task)
    {
        case 1:                                                                 //Homing
            Task=100;
            break;
        case 100:                                                               //Search Area
            if(MoveInArmZToPlateSafe(Task))
            {
                fContact->InitCarlibrationTask();
                RecordAction("Start Auto Height Action");
                iContactMode=CONTACT_AUTO_GET_HEIGHT;
                bSmartSetupAutoHeight=true;
                Task=200;
            }
            break;
        case 200:
            fContact->DoTestContactFunction();

            edReleaseHeight1->Text=fContact->edReleaseHeight1->Text;
            edPickUp1->Text=fContact->edPickUp1->Text;
            edContactHeight1->Text=fContact->edContactHeight1->Text;

            edReleaseHeight2->Text=fContact->edReleaseHeight2->Text;
            edPickUp2->Text=fContact->edPickUp2->Text;
            edContactHeight2->Text=fContact->edContactHeight2->Text;
            if(bSmartSetupAutoHeight==false)
            {
                RecordAction("End Auto Height Action");
                Task=300;
            }
            break;
        case 300:
            for(int i=0; i<MAX_Index_Row; i++)
            {
                for(int j=0; j<NEW_MAX_Index_Col; j++)
                {
                    if(FLCarryKit.Item[i][j]!=NULL_IC)
                    {
                        FRCarryKit.MoveSuckData(FLCarryKit, i, j);
                        if(FRCarryKit.Item[i][j]!=HAS_NULL_IC)
                        {
                            FRCarryKit.SetItemData(i, j, TEST_PASS+iTestBinCount);
                            FRCarryKit.iBinData[i][j]=iTestBinCount;
                        }
                        else
                        {
                            FRCarryKit.SetItemData(i, j, NULL_IC);
                        }
                    }

                    if(BLCarryKit.Item[i][j]!=NULL_IC)
                    {
                        BRCarryKit.MoveSuckData(BLCarryKit, i, j);
                        if(BRCarryKit.Item[i][j]!=HAS_NULL_IC)
                        {
                            BRCarryKit.SetItemData(i, j, TEST_PASS+iTestBinCount);
                            BRCarryKit.iBinData[i][j]=iTestBinCount;
                        }
                        else
                        {
                            BRCarryKit.SetItemData(i, j, NULL_IC);
                        }
                    }
                }
            }
            return true;
    }
    return false;
}
//---------------------------------------------------------------------------
void TfSmartSetup::InitalSmartSetupProcessTask()
{
    iDoSmartSetupProcessTask=1;
}
//---------------------------------------------------------------------------
void TfSmartSetup::DoSmartSetupProcess()
{
    int &Task=iDoSmartSetupProcessTask;
    static bool bMove[2];
    switch(Task)
    {
        case 1:                                                                 //整體Homing
            if(MoveInArmZToPlateSafe(1111)==false)
                break;
            if(MoveOutArmToAutoSafe_9045()==false)
                break;

            InitalInOutArmCalibrationTask();
            bfinishSetup=false;
            Task=100;
            break;
        case 100:                                                               //InArm OutArm Z Calibration
            if(DoInOutArmZCalibration())
            {
                if(bHandlerPause)
                    SetAllMotorSpeed(10);

                RecordAction("Start Loader/Unloader Get Tray");
                InitalDoLoadTrayTask();
                SaveZCalibrationPosition();
                Task=200;
            }
            break;
        case 200:                                                               //Load Device
            if(DoLoadTray())
            {
                RecordAction("End Loader/Unloader Get Tray");
                if(MACHINE_HAS_AUTO_ALIGNMENT_CCD && TestIF.bEnableAutoAlignment==true)
                {
                    RecordAction("Start In Arm Auto Alignment");
                    InitalDoInArmAlignmentTask();
                    Task=250;
                }
                else
                {
                    InitalInArmZSearchAreaTask();
                    Task=300;
                }
            }
            break;
        case 250:                                                               //Auto Alignment
            if(DoInArmAlignment())
            {
                RecordAction("End In Arm Auto Alignment");
                InitalInArmZSearchAreaTask();
                Task=300;
            }
            break;
        case 300:                                                               //InArm Area Z Search
            if(DoInArmZSearchArea())
            {
                InitalDoAutoHeightTask();
                Task=400;
            }
            break;
        case 400:                                                               //Index Auto contact height
            if(DoAutoHeight())
            {
                if(MACHINE_HAS_AUTO_ALIGNMENT_CCD && TestIF.bEnableAutoAlignment==true)
                {
                    RecordAction("Start Out Arm Auto Alignment");
                    bMove[0]=false;
                    bMove[1]=false;
                    Task=450;
                }
                else
                {
                    RecordAction("Start Loader/Unloader Get Tray");
                    InitalOutArmZSearchAreaTask();
                    Task=600;
                }
            }
            break;
        case 450:
            if(bMove[0]==false)
               bMove[0]=MOT[MInShuttle1].MotorMove(Prod.InSHT[0].iRight);

            if(bMove[1]==false)
               bMove[1]=MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iRight);

            if(bMove[0]&& bMove[1])
            {
                InitalDoOutArmAlignmentTask();
                Task=500;
            }
            break;
        case 500:
            if(DoOutArmAlignment())
            {
                RecordAction("End Out Arm Auto Alignment");
                InitalOutArmZSearchAreaTask();
                Task=600;
            }
            break;
        case 600:                                                               //OutArm Area Z Search
            if(DoOutArmZSearchArea())
            {
                RecordAction("Finish All Action");
                tUPH_PauseTime=0;
                iTrayFeed=1;
                iTrayFeedTask=1;
                bCleanOutTrayEnd=false;
                bfinishSetup=true;
                btnSmartSetup->Enabled=true;
            }
            break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfSmartSetup::FormShow(TObject *Sender)
{
    fShow=true;
    Image1->Picture->LoadFromFile(BmpPath+"Contact.bmp");
    cbEnableAutoAlignment->Checked=(TestIF_File.bEnableAutoAlignment)?1:0;
}
//---------------------------------------------------------------------------
void __fastcall TfSmartSetup::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    fShow=false;
    btnSmartSetup->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TfSmartSetup::btnSmartSetupClick(TObject *Sender)
{
    InitalSmartSetupProcessTask();
    btnSmartSetup->Enabled=false;
}
//---------------------------------------------------------------------------
void __fastcall TfSmartSetup::sbtExitClick(TObject *Sender)
{
    int ret;
    if(bfinishSetup)
    {
        bfinishSetup=false;
        ret=ShowMyMessageBox_YES_NO("Sure To Save New Posistion", "確定要存取新的點位？");
        if(ret!=2)
        {
            SavePosition();
        }
    }

    if(!(iDoSmartSetupProcessTask==1 || iDoSmartSetupProcessTask==600))
    {
        ret=ShowMyMessageBox_YES_NO("Action Running Sure To Exit", "動作進行中，確定要離開？");
        if(ret!=2)
        {
            fAllMotorHome=false;
            Close();
        }
    }
    else
    {
        Close();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfSmartSetup::btShowDataClick(TObject *Sender)
{
    static int iCount=0;

    iCount++;
    if(iCount%2==1)
    {
        Panel16->Left=809;
        Panel18->Left=1;
        btShowData->Caption="Show Now Data";
    }
    else
    {
        iCount=0;
        Panel18->Left=809;
        Panel16->Left=1;
        btShowData->Caption="Show All Data";
    }
}
//---------------------------------------------------------------------------
bool TfSmartSetup::DoPlaceToHotPlate()
{
    switch(TestIF.iTestMode)
    {
        case SingleSite:
            if(DoPlaceToHotPlate_9045_1x1_1())
                return true;
            break;
        case DualSite2x1:
            if(DoPlaceToHotPlate_9045_2x1_2())
                return true;
            break;
        case DualSite:
            if(DoPlaceToHotPlate_9045_1x2_2())
                return true;

            if(DoPlaceToHotPlate_9045_1x2_2_14())
                return true;
            break;
        case TriSite1X3:
            if(DoPlaceToHotPlate_9045_1x3_4())
                return true;
            break;
        case QualSite1X4:
        case _8Site1X4:
            if(DoPlaceToHotPlate_9045_1x4_4())
                return true;
            break;
        case QualSite2X2:
            if(DoPlaceToHotPlate_9045_2x2_4())
                return true;

            if(DoPlaceToHotPlate_9045_2x2_4_14())
                return true;
            break;
        case _6Site2X3:
            if(DoPlaceToHotPlate_9045_2x3_6())
                return true;
            break;
        case _8Site2X4:
            if(DoPlaceToHotPlate_9045_2x4_8())
                return true;

//            if(DoPlaceToHotPlate_9045_2x4_4())
//                return true;
            break;
        case _12Site2X6:
            if(DoPlaceToHotPlate_9045_2x6_8())
                return true;
            break;
        case _16Site2X8:
            if(DoPlaceToHotPlate_9045_2x8_8())
                return true;
            break;
        case _16Site4X4:
            if(DoPlaceToHotPlate_9045_2x8_8())
                return true;
            break;
        case _32Site4X8N:
        case _32Site4X8M:
            if(DoPlaceToHotPlate_9045_2x8_32())
                return true;
            break;
    }
    return false;
}
//---------------------------------------------------------------------------
bool TfSmartSetup::DoPickFromToOutSh1()
{
    switch(TestIF.iTestMode)
    {
        case SingleSite:
            break;
        case DualSite2x1:
            break;
        case DualSite:
            break;
        case TriSite1X3:
            break;
        case QualSite1X4:
        case _8Site1X4:
            break;
        case QualSite2X2:
            break;
        case _6Site2X3:
            break;
        case _8Site2X4:
            if(DoPickFromShuttle1_9045_2x4_8(false))
                return true;
            break;
        case _12Site2X6:
            break;
        case _16Site2X8:
            if(DoPickFromShuttle1_9045_2x8_8(false))
                return true;
            break;
        case _16Site4X4:
            break;
        case _32Site4X8N:
        case _32Site4X8M:
            break;
    }
    return false;
}
//---------------------------------------------------------------------------
bool TfSmartSetup::DoPickFromToOutSh2()
{
    switch(TestIF.iTestMode)
    {
        case SingleSite:
            break;
        case DualSite2x1:
            break;
        case DualSite:
            break;
        case TriSite1X3:
            break;
        case QualSite1X4:
        case _8Site1X4:
            break;
        case QualSite2X2:
            break;
        case _6Site2X3:
            break;
        case _8Site2X4:
            if(DoPickFromShuttle2_9045_2x4_8(false))
                return true;
            break;
        case _12Site2X6:
            break;
        case _16Site2X8:
            if(DoPickFromShuttle2_9045_2x8_8(false))
                return true;
            break;
        case _16Site4X4:
            break;
        case _32Site4X8N:
        case _32Site4X8M:
            break;
    }
    return false;
}
//---------------------------------------------------------------------------
bool TfSmartSetup::DoPlaceToAuto()
{
    switch(TestIF.iTestMode)
    {
        case SingleSite:
            break;
        case DualSite2x1:
            break;
        case DualSite:
            break;
        case TriSite1X3:
            break;
        case QualSite1X4:
        case _8Site1X4:
            break;
        case QualSite2X2:
            break;
        case _6Site2X3:
            break;
        case _8Site2X4:
            if(DoPlaceToAuto_9045_2x4_8())
                return true;
            break;
        case _12Site2X6:
            break;
        case _16Site2X8:
            if(DoPlaceToAuto_9045_2x8_8())
                return true;
            break;
        case _16Site4X4:
            break;
        case _32Site4X8N:
        case _32Site4X8M:
            break;
    }
    return false;
}
//---------------------------------------------------------------------------
void IniOffset(int iArea, int iInArm)
{
    int i, j;

    for(i=0; i<InArmSuck.iMaxRow; i++)
    {
        for(j=0; j<InArmSuck.iMaxCol; j++)
        {
            if(iInArm==0)
            {
                InArmOffSet[iArea]->SetPickUp(i, j, 0);
                InArmOffSet[iArea]->SetPlace(i, j, 0);
            }
            else
            {
                OutArmOffSet_File[iArea]->SetPickUp(i, j, 0);
                OutArmOffSet_File[iArea]->SetPlace(i, j, 0);
            }
        }
    }
}
//---------------------------------------------------------------------------
void TfSmartSetup::SaveZCalibrationPosition()
{
    AnsiString S="";
    S=GetLastOpenFN();
    AnsiString szDir="";
    szDir=DataPath+S+"\\";

    TEdit *edInArm[]={edInArmZA, edInArmZB, edInArmZC, edInArmZD,
                      edInArmZE, edInArmZF, edInArmZG, edInArmZH};

    TEdit *edOutArm[]={edOutArmZA, edOutArmZB, edOutArmZC, edOutArmZD,
                       edOutArmZE, edOutArmZF, edOutArmZG, edOutArmZH};

    TEdit *teInArm[]={fTeach->setEditZ1A, fTeach->setEditZ1B, fTeach->setEditZ1C, fTeach->setEditZ1D,
                      fTeach->setEditZ1E, fTeach->setEditZ1F, fTeach->setEditZ1G, fTeach->setEditZ1H};

    TEdit *teOutArm[]={fTeach->setEditZ2A, fTeach->setEditZ2B, fTeach->setEditZ2C, fTeach->setEditZ2D,
                       fTeach->setEditZ2E, fTeach->setEditZ2F, fTeach->setEditZ2G, fTeach->setEditZ2H};

    int i, iStandZ, iBuffer[2], dbTrayThick;

    for(i=0; i<8; i++)
    {
        teInArm[i]->Text=edInArm[i]->Text;
        teOutArm[i]->Text=edOutArm[i]->Text;
    }
    fTeach->IniSuckPara();
    fTeach->UpdateTempTech();
    WriteData("d:\\HT9045\\system\\tech.dat", (char *)&Tech.iZLoad, sizeof(TECH));

    fContact->SaveSetupFile(szDir, S);
}
//---------------------------------------------------------------------------
void TfSmartSetup::SavePosition()
{
    AnsiString S="";
    S=GetLastOpenFN();
    AnsiString szDir="";
    szDir=DataPath+S+"\\";

    TEdit *edInArm[]={edInArmZA, edInArmZB, edInArmZC, edInArmZD,
                      edInArmZE, edInArmZF, edInArmZG, edInArmZH};

    TEdit *edOutArm[]={edOutArmZA, edOutArmZB, edOutArmZC, edOutArmZD,
                       edOutArmZE, edOutArmZF, edOutArmZG, edOutArmZH};

    TEdit *teInArm[]={fTeach->setEditZ1A, fTeach->setEditZ1B, fTeach->setEditZ1C, fTeach->setEditZ1D,
                      fTeach->setEditZ1E, fTeach->setEditZ1F, fTeach->setEditZ1G, fTeach->setEditZ1H};

    TEdit *teOutArm[]={fTeach->setEditZ2A, fTeach->setEditZ2B, fTeach->setEditZ2C, fTeach->setEditZ2D,
                       fTeach->setEditZ2E, fTeach->setEditZ2F, fTeach->setEditZ2G, fTeach->setEditZ2H};

    int i, iStandZ, iBuffer[2], dbTrayThick;

    for(i=0; i<8; i++)
    {
        teInArm[i]->Text=edInArm[i]->Text;
        teOutArm[i]->Text=edOutArm[i]->Text;
    }
    fTeach->IniSuckPara();
    fTeach->UpdateTempTech();
    WriteData("d:\\HT9045\\system\\tech.dat", (char *)&Tech.iZLoad, sizeof(TECH));

    fContact->SaveSetupFile(szDir, S);

    int iInArmTech[5]={Tech.iInArmLoadStagePickZ2, Tech.iInArmPlatePickZ2,
                       Tech.iInArmPlatePickZ2, Tech.iInArmShuttlePlaceZ,
                       Tech.iInArmShuttlePlaceZ};
    AnsiString asInStr[5]={"Load Z Teach.", "Hotplate 1.", "Hotplate 2.",
                           "Shuttle 1 Z Teach.", "Shuttle 2 Z Teach."};
    TEdit *tInArmArea[]={edLoader, edHP1, edHP2, edInSH1, edInSH2};

    double dBuffer[2], iTeachZData1;

    for(i=0; i<5; i++)
    {
        if(tInArmArea[i]->Text!="")
        {
            IniOffset(i, 0);
            iStandZ = atoi(tInArmArea[i]->Text.c_str())+ atoi(fTeach->setEditZ1A->Text.c_str());

            dBuffer[0]=InArmOffSet[i]->GetX()/100;
            dBuffer[1]=InArmOffSet[i]->GetY()/100;

            InArmOffSet[i]->SetX(dBuffer[0]);
            InArmOffSet[i]->SetY(dBuffer[1]);
            if(i==3 || i==4)
            {
                if(DeviceForm_File.XDimension <10 || DeviceForm_File.YDimension <10 )        //kevin 20210607 change 5->9  //kevin 20210408 add IC SIZE compare
                    iTeachZData1 = (double)(iStandZ-iInArmTech[i] +100)/100;    //往下 50
                else
                    iTeachZData1 = (double)(iStandZ-iInArmTech[i] +150)/100;    //往下 50
            }
            else
            {
                if(i==0)
                {
                    iBuffer[0]=iInArmTech[0];
                    if(CosFunction.bUseTrayThickAdjustZHeight==true &&
                    (IniConfig.bE70_UseTrayThickAdjustZHeight==true ||       //Ifor 20221214 add: 使用Tray 厚度自動校正吸嘴高度
                    UserDefForm_File[0].bUseThickTray))                      //JerryYang 20230620 : add厚tray選項
                    {
                        dbTrayThick=UserDefForm_File[TrayForm.Loader.iTrayType].ZDepth*100;
                        if(dbTrayThick<600)
                            dbTrayThick=600.0;

                        iBuffer[0]+=dbTrayThick-635;
                    }
                    iTeachZData1 = (double)(iStandZ-iBuffer[0])/100;
                }
                else
                {
                    iTeachZData1 = (double)(iStandZ-iInArmTech[i])/100;
                }
            }

            if(fabs(iTeachZData1)>3.0)
            {
                ShowErrorMessage("WAR16315", 0, MMSystem, 0, asInStr[i]);
            }
            else
            {
                if(i==0)
                {
                    InArmOffSet[i]->SetPickUp(iTeachZData1-0.5);    // offset 歸0
                }
                else if(i==3 || i==4)
                {
                    InArmOffSet[i]->SetPlace(iTeachZData1);
                }
                else
                {
                    InArmOffSet[i]->SetPickUp(iTeachZData1);
                    InArmOffSet[i]->SetPlace(iTeachZData1+0.5);
                }
                Zteach->SaveFile(i,1, false);
            }
        }
    }

    AnsiString asOutStr[2]={"Out Shuttle 1.", "Out Shuttle 2."};
    TEdit *tOutArmArea[]={edOutSH1, edOutSH2};
    for(i=0; i<2; i++)
    {
        if(tOutArmArea[i]->Text!="")
        {
            IniOffset(i, 1);

            dBuffer[0]=OutArmOffSet_File[i]->GetX()/100;
            dBuffer[1]=OutArmOffSet_File[i]->GetY()/100;

            OutArmOffSet_File[i]->SetX(dBuffer[0]);
            OutArmOffSet_File[i]->SetY(dBuffer[1]);
            iStandZ = atoi(tOutArmArea[i]->Text.c_str())+ atoi(fTeach->setEditZ2A->Text.c_str());

            iTeachZData1 = (double)(iStandZ-Tech.iOutArmShuttlePickZ2)/100;
            if(fabs(iTeachZData1)>3.0)
            {
                ShowErrorMessage("WAR16315", 0, MMSystem, 0, asOutStr[i]);
            }
            else
            {
                OutArmOffSet_File[i]->SetPickUp(iTeachZData1-0.5);    // offset 歸0
                Zteach->SaveFile(5+i,1, false);
            }
        }
    }

    AnsiString asOutAutoStr[3]={"Auto 1.", "Auto 2.", "Auto 3."};
    TEdit *tAuto[]={edAuto1, edAuto2, edAuto3};
    for(i=0; i<3; i++)
    {
        if(tAuto[i]->Text!="")
        {
            IniOffset(OutOfsAuto1+i, 1);

            dBuffer[0]=OutArmOffSet_File[OutOfsAuto1+i]->GetX()/100;
            dBuffer[1]=OutArmOffSet_File[OutOfsAuto1+i]->GetY()/100;

            OutArmOffSet_File[OutOfsAuto1+i]->SetX(dBuffer[0]);
            OutArmOffSet_File[OutOfsAuto1+i]->SetY(dBuffer[1]);

            iStandZ = atoi(tAuto[i]->Text.c_str())+ atoi(fTeach->setEditZ2A->Text.c_str());

            iBuffer[0]=Tech.iOutArmPlaceZ2;

            if(CosFunction.bUseTrayThickAdjustZHeight==true &&
               (IniConfig.bE70_UseTrayThickAdjustZHeight==true ||       //Ifor 20221214 add: 使用Tray 厚度自動校正吸嘴高度
                UserDefForm_File[0].bUseThickTray))                     //JerryYang 20230620 : add厚tray選項
            {
                dbTrayThick=UserDefForm_File[TrayForm.Auto[i].iTrayType].ZDepth*100;
                if(dbTrayThick<600)
                    dbTrayThick=600.0;

                iBuffer[0]+=dbTrayThick-635;
            }

            iTeachZData1 = (double)(iStandZ-iBuffer[0])/100;    //kevin 20210720  change  iStandZ
            if(fabs(iTeachZData1)>3.0)
            {
                ShowErrorMessage("WAR16315", 0, MMSystem, 0, asOutAutoStr[i]);
            }
            else
            {
                OutArmOffSet_File[OutOfsAuto1+i]->SetPickUp(iTeachZData1-0.5);    //kevin 20210720
                Zteach->SaveFile(7+i,1, false);
            }
        }
    }

    AnsiString asOutFixStr[3]={"Fix 1.", "Fix 2.", "Fix 3."};
    TEdit *tFix[]={edFix1, edFix2, edFix3};
    for(i=0; i<3; i++)
    {
        if(tFix[i]->Text!="")
        {
            IniOffset(OfsFix1+i, 1);

            dBuffer[0]=OutArmOffSet_File[OfsFix1+i]->GetX()/100;
            dBuffer[1]=OutArmOffSet_File[OfsFix1+i]->GetY()/100;

            OutArmOffSet_File[OfsFix1+i]->SetX(dBuffer[0]);
            OutArmOffSet_File[OfsFix1+i]->SetY(dBuffer[1]);

            iStandZ = atoi(tFix[i]->Text.c_str())+ atoi(fTeach->setEditZ2A->Text.c_str());

            iBuffer[0]=Tech.iOutArmPlaceFixZ1;
            if(CosFunction.bUseTrayThickAdjustZHeight==true &&
               (IniConfig.bE70_UseTrayThickAdjustZHeight==true ||       //Ifor 20221214 add: 使用Tray 厚度自動校正吸嘴高度
                UserDefForm_File[0].bUseThickTray))                     //JerryYang 20230620 : add厚tray選項
            {
                dbTrayThick=UserDefForm_File[TrayForm.Fix[i].iTrayType].ZDepth*100;
                if(dbTrayThick<600)
                    dbTrayThick=600.0;
                iBuffer[0]+=dbTrayThick-635;
                iBuffer[1]+=dbTrayThick-635;
            }

            iTeachZData1 = (double)(iStandZ-iBuffer[0])/100;    //kevin 20210720  change  iStandZ
            if(fabs(iTeachZData1)>3.0)
            {
                ShowErrorMessage("WAR16315", 0, MMSystem, 0, asOutFixStr[i]);
            }
            else
            {
                OutArmOffSet_File[OfsFix1+i]->SetPickUp(iTeachZData1-0.5);    //kevin 20210720
                Zteach->SaveFile(10+i,1, false);
            }
        }
    }

    fOffSet->ReadFile();
    DoStructUnitConvert();
}
//---------------------------------------------------------------------------

void __fastcall TfSmartSetup::Button1Click(TObject *Sender)
{
    SavePosition();
}
//---------------------------------------------------------------------------

void __fastcall TfSmartSetup::cbEnableAutoAlignmentClick(TObject *Sender)
{
    AnsiString S="";
    S=GetLastOpenFN();
    AnsiString szDir="";
    AnsiString sGroup;

    szDir.sprintf("%s%s\\HandlerCondition.Data", DataPath, S);
    sGroup="AutoAlignmrnt";
                                                                                          WriteIniData(szDir, sGroup, "Enable Auto Alignment", (cbEnableAutoAlignment->Checked)?1:0);
    TestIF_File.bEnableAutoAlignment  =ReadIniData(szDir, sGroup, "Enable Auto Alignment",            false);
}
//---------------------------------------------------------------------------

