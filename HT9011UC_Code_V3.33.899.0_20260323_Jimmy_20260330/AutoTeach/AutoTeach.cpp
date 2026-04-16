//---------------------------------------------------------------------------
#include "MachineDefine.h"
#pragma hdrstop

#include "myQwertyKeyBoard.h"
#include "AutoTeach.h"

#include "aArmHeader.h"
#include "cConfiguration.h"
#include "main.h"
#include "cmydef.h"
#include "mymessbox.h"
#include "ShuttleMove.h"
#include "mymotor.h"
#include "cSecurity.h"
#include "MyKitSuck.h"
#include "LastSet.h"
#include "common.h"
#include "csystem.h"
#include "InOutArmZteach.h"
#include "mysensor.h"
#include "myswitch.h"
#include "mycylin.h"
#include "note.h"
#include "uteach.h"
//#include "SmartSetup.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "butPa1"
#pragma resource "*.dfm"
TfAutoTeach *fAutoTeach;
int iLoadAutoTeachTask=1;   //kevin 20170930 add
int iInArmZTeachTask=1;     //kevin 20171107 (wei) add Arm Z Teach
int iOutArmZTeachTask=1;    //kevin 20171107 (wei) add Arm Z Teach
int iAutoTeachInArmZTask=1;                             //KaiChen 20180117 (Steven) : InArm Auto Hight From Loader Tray
TQPF_Timer DoSupplyNewICTrayDelay_AutoTeach;            //KaiChen 20180117 (Steven) : InArm Auto Hight From Loader Tray
//---------------------------------------------------------------------------
__fastcall TfAutoTeach::TfAutoTeach(TComponent* Owner)
    : TForm(Owner)
{
    bIsFirstTime=true;
    bIsFormShow=false;
}
//---------------------------------------------------------------------------
void __fastcall TfAutoTeach::FormShow(TObject *Sender)
{
    Left=(1024-Width)/2;
    Top =(768 -Height)/2;
    AnsiString sShowImageSize="Size of Image is "+AnsiString(img_ShowImage->Width)+"x"+(img_ShowImage->Height)+"(WxH)";
    img_ShowImage->Hint=sShowImageSize;
    bIsFormShow=true;
}
//---------------------------------------------------------------------------
void __fastcall TfAutoTeach::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    bIsFormShow=false;
    fOffSet->cb_AutoOffsetPositionCheck->Checked=false;
}
//---------------------------------------------------------------------------
void TfAutoTeach::ShowFormAndMoveToCenter()
{
    Left=0;
    Top=0;
    fAutoTeach->Show();
    fAutoTeach->BringToFront();
}
//---------------------------------------------------------------------------
void TfAutoTeach::ShowOneTabSheet(TTabSheet *tsTabSheet)
{
    for(int i=0;i<pc_AutoTeach->PageCount;i++)
    {
        if(pc_AutoTeach->Pages[i]->Name == tsTabSheet->Name)
        {
            pc_AutoTeach->ActivePage=tsTabSheet;
            tsTabSheet->TabVisible  =true;
        }
        else
        {
            pc_AutoTeach->Pages[i]->TabVisible=false;
        }
    }
}
//---------------------------------------------------------------------------
void TfAutoTeach::ShowFormAndActiveTabSheet(TTabSheet *tsTabSheet,AnsiString asFileName)
{
    fOffSet->cb_AutoOffsetPositionCheck->Checked=true;
    pan_AutoOffsetMove->Parent=tsTabSheet;
    if(tsTabSheet==ts_DoHomeing             ||
       tsTabSheet==ts_DoShuttleSensorCheckL ||
       tsTabSheet==ts_DoShuttleSensorCheckR ||
       tsTabSheet==ts_DoAutoHeight          )
    {
        pan_AutoOffsetMove->Visible = false;
    }
    else
    {
        pan_AutoOffsetMove->Visible = true;
    }

    if(tsTabSheet==ts_DoAutoHeight          ||
       tsTabSheet==ts_DoShuttleSensorCheckL ||
       tsTabSheet==ts_DoShuttleSensorCheckR ||
       tsTabSheet==ts_DoHomeing             )
    {
        pn_Information->Visible=false;
    }
    else
    {
        pn_Information->Visible=true;
    }
    mo_ShowMessage->Parent=tsTabSheet;
    img_ShowImage->Parent =tsTabSheet;
    lb_DoingItem->Parent  =tsTabSheet;
    pn_Information->Parent=tsTabSheet;
    ShowOneTabSheet(tsTabSheet);
    ShowFormAndMoveToCenter();
    FormLoadMessageAndImage(asFileName);
    AutoOffsetPitchYCanVisible(false);
}
//---------------------------------------------------------------------------
void TfAutoTeach::FormLoadMessageAndImage(AnsiString asFileName)
{
    AnsiString sFilePath    ="D:\\HT9045\\AutoTeach\\";  //JimmyChiu 20220106 提升客戶更新版本效率  "D:\\HT9045\\system\\AutoTeach\\"=>"D:\\HT9045\\AutoTeach\\"  ，將AutoTeach檔案放在安裝包中
    ed_HintString->Text     =asFileName;
    lb_DoingItem->Caption   =asFileName;
    AnsiString sFilePathName=sFilePath+asFileName;
    AnsiString sTemp="";
    TStringList *ts_Message =new TStringList();
    MyForceDirectories(sFilePath);
    //Load Message File
    mo_ShowMessage->Clear(); //20180504
    sTemp=sFilePathName+".txt";
    if(FileExists(sTemp))
    {
        ts_Message->Clear();
        ts_Message->LoadFromFile(sTemp);
        if(ts_Message->Count>0)
        {
            lb_DoingItem->Caption=ts_Message->Strings[0];
            for(int i=1; i<ts_Message->Count; i++)
            {
                mo_ShowMessage->Lines->Add(ts_Message->Strings[i]);
            }
        }
    }
    //Load Message File
    img_ShowImage->Visible=false; //20180504
    sTemp=sFilePathName+".bmp";
    if(FileExists(sTemp)==true)
    {
        img_ShowImage->Picture->LoadFromFile(sTemp);
        img_ShowImage->Visible = true; //20180504
    }
    delete ts_Message;
}
//---------------------------------------------------------------------------
void __fastcall TfAutoTeach::sb_DoNextClick(TObject *Sender)
{
    if(SystemStart == true)
        return;
    int ret=MessageDlg("Do Next Step?", mtConfirmation, TMsgDlgButtons()<<mbYes<<mbNo, 0);
    if(ret==mrYes)
    {
        //save data
        if(fOffSet->fShow)
            fOffSet->spbSaveClick(fOffSet);
        else if(fContact->fShow)
            fContact->spbSaveClick(fContact);
        DoNext();
    }
    else
    {
        return;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAutoTeach::sb_DoCloseClick(TObject *Sender)
{
    iATStatus=Pause;
    int ret=(GetState()==eATTotalAction)?1:ShowMyMessageBox_YES_NO("Sure To Stop and Exit Auto Teach Flow?","Double Check");
    if(ret==1)
    {
        fMain->BtnPauseClick(fMain);
        StartAutoTeach(false);
        Close();
    }
    iATStatus=Run;
}
//---------------------------------------------------------------------------
void __fastcall TfAutoTeach::sb_AutoOffsetXRightClick(TObject *Sender)
{
    fOffSet->sb_AutoOffsetRightClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfAutoTeach::sb_DoRetryClick(TObject *Sender)
{
    if(ShowMyMessageBox_YES_NO("Sure To Retry?","Double Check")==1)
    {
        iATStatus=Run;
        iTask=0;
        iTaskPlaceToAuto=0;
    }
}
//---------------------------------------------------------------------------
void TfAutoTeach::AutoOffsetPitchYCanVisible(bool bIsCanVisible)
{
    Label14->Visible=bIsCanVisible;
    Label15->Visible=bIsCanVisible;
    sb_AutoOffsetPitchYOpen->Visible =bIsCanVisible;
    sb_AutoOffsetPitchYClose->Visible=bIsCanVisible;

    Label25->Visible=bIsCanVisible;
    ed_YPitchOffsetValue->Visible=bIsCanVisible;
}
//---------------------------------------------------------------------------
bool TfAutoTeach::IsRun()
{
    if(CosFunction.bManualSteplAutoTeach==false || IniConfig.bA56EnableAutoTeachFunciton==false)
    {
        SetState(eATClose);
        iATStatus=Idle;
        return false;
    }
    else
    {
        if(bIsFirstTime)
        {
            SetLoaderCount();
            bIsFirstTime=false;
            SetState(LastSet.iAutoTeachStep);
        }
    }
    if(GetState()==eATClose)
        return false;
    else if(iATStatus!=Pause)
        iATStatus=Run;
    return true;
}
//---------------------------------------------------------------------------
void TfAutoTeach::ShowSetFormPos(TForm *tTForm, bool bOpen, bool bhide=false)
{
    if(tTForm!=NULL)
    {
        if(bOpen)
            tTForm->Show();
        else
            tTForm->Close();
        tTForm->Left=(bhide)?2500:this->Width;
        tTForm->Top=0;
    }
}
//---------------------------------------------------------------------------
void TfAutoTeach::SetLoaderCount()
{
    switch(TestIF.iTestMode)
    {
        case _10Site2X5:
            if(IsShuttle1Enable())
                iLoaderCnt+=10;
            if(IsShuttle2Enable())
                iLoaderCnt+=10;
            break;
        case _12Site2X6:
            if(IsShuttle1Enable())
                iLoaderCnt+=12;
            if(IsShuttle2Enable())
                iLoaderCnt+=12;
            break;
        case _8Site2X4:
            if(IsShuttle1Enable())
                iLoaderCnt+=8;
            if(IsShuttle2Enable())
                iLoaderCnt+=8;
            break;
        case _16Site2X8:
            if(IsShuttle1Enable())
                iLoaderCnt+=16;
            if(IsShuttle2Enable())
                iLoaderCnt+=16;
            break;
        case _32Site4X8N:
            if(IsShuttle1Enable())
                iLoaderCnt+=32;
            if(IsShuttle2Enable())
                iLoaderCnt+=32;
            break;
        default:
            if(IsShuttle1Enable())
                iLoaderCnt+=8;
            if(IsShuttle2Enable())
                iLoaderCnt+=8;
            break;
    }
}
//---------------------------------------------------------------------------
void TfAutoTeach::InitialData()
{
    iTask=0;
    iNextStep=0;
    iTaskPlaceToAuto=0;
    iATStatus=Idle;
    CloseAllForm();
    sb_DoRetry->Visible=true;
    sb_DoNext->Visible=true;
    iLoaderCnt=0;
    SetState(0);
    LastSet.bNeedSetupTeach=false;
    LastSet.iATLoaderCount=0;
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<2; j++)
        {
            LastSet.bNeedInShuttleTeach[i][j]=false;
            LastSet.bNeedOutShuttleTeach[i][j]=false;
        }
    }
    for(int i=0; i<eATTotalAction; i++)
        bStepFinish[i]=false;
}
//---------------------------------------------------------------------------
bool TfAutoTeach::DoAutoTeachProcess()
{
    if(IsRun()==false ||
       CheckGPIBisRun()==false)
        return false;
    if(bIsFormShow==false)
        this->Show();
    switch(GetState())
    {
        case eATClose:              //0  : disable teach
            return false;
        case eATHome:
            DoHome();
            break;
        case eATInShuttleSensor:
            DoInputShuttleDoubleDeviceSensor();
            break;
        case eATOutShuttleSensor:
            DoOutputShuttleSensor();
            break;
        case eATIndexArm:
            DoAutoHeight();
            break;
        case eATLoader:
            DoLoaderPosition();
            break;
        case eATHP1:
            if(HotPlateForm.iPlateSelect==2 || LastSet.iTemperature==Tempture_Ambient)
                DoNext();
            else
                DoPlaceHotPlate();
            break;
        case eATHP2:
            if(HotPlateForm.iPlateSelect==1 || LastSet.iTemperature==Tempture_Ambient)
                DoNext();
            else
                DoPlaceHotPlate();
            break;
        case eATInShuttle1L:
        case eATInShuttle1R:
            if(IsShuttle1Enable())
                DoInputShuttlePosition();
            else
                DoNext();
            break;
        case eATInShuttle2L:
        case eATInShuttle2R:
            if(IsShuttle2Enable()) //only Shuttle 1
                DoInputShuttlePosition();
            else
                DoNext();
            break;
        case eATOutShuttle1L:
        case eATOutShuttle1R:
            if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1) //only Shuttle 2
                DoNext();
            else
                DoOutputShuttlePosition();        //0:outsh1 1:outsh2
            break;
        case eATFix3:
            DoPlaceFix3();
            break;
        case eATAuto1:
            DoPlaceAuto1();
            break;
        case eATAuto2:
            DoPlaceAuto2();
            break;
        case eATAuto3:
            DoPlaceAuto3();
            break;
        case eATFix1:
            DoPlaceFix1();
            break;
        case eATFix2:
            DoPlaceFix2();
            break;
        case eATOutShuttle2L:
        case eATOutShuttle2R:
            if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==0) //only Shuttle 1
                DoNext();
            else
                DoOutputShuttlePosition();        //0:outsh1 1:outsh2
            break;
        case eATActionFinish:
            DoFinish();
            return false;
        default:
            StartAutoTeach(false);
            return false;
    }
    CheckAndAvoidHangUp();
    return true;
}
//---------------------------------------------------------------------------
void TfAutoTeach::DoPlaceFix1()
{
    UpdateInfoOffsetPosition();
    switch(iTask)
    {
        case 0:
            ShowFormAndActiveTabSheet(ts_DoPlaceAutoTray, "DoPlaceAutoTray");
            LastSet.bNeedSetupTeach=true;
            InitialInfoPlaceFixAutoTray("Unloader FIX1 Position", fOffSet->sbFix1, OutOfsFix1, fOffSet->btnSortFix1);
            lb_DoingItem->Caption="DoPlaceAutoTray FIX1";
            fMain->Start("DoPlaceFix1");
            iTask=100;
            break;
        case 100:
            OutArmSetupTeachCtl(OutOfsFix1, true);
            break;
        default:
            DoNext();
            break;
    }
}
//---------------------------------------------------------------------------
void TfAutoTeach::DoPlaceFix2()
{
    UpdateInfoOffsetPosition();
    switch(iTask)
    {
        case 0:
            ShowFormAndActiveTabSheet(ts_DoPlaceAutoTray, "DoPlaceAutoTray");
            LastSet.bNeedSetupTeach=true;
            InitialInfoPlaceFixAutoTray("Unloader FIX2 Position", fOffSet->sbFix2,OutOfsFix2, fOffSet->btnSortFix2);
            fMain->Start("DoPlaceFix2");
            iTask=100;
            break;
        case 100:
            OutArmSetupTeachCtl(OutOfsFix2,true);
            break;
        default:
            DoNext();
            break;
    }
}
//---------------------------------------------------------------------------
void TfAutoTeach::DoPlaceFix3()
{
    UpdateInfoOffsetPosition();
    switch(iTask)
    {
        case 0:
            ShowFormAndActiveTabSheet(ts_DoPlaceAutoTray, "DoPlaceAutoTray");
            LastSet.bNeedSetupTeach=true;
            InitialInfoPlaceFixAutoTray("Unloader FIX3 Position", fOffSet->sbFix3, OutOfsFix3, fOffSet->btnSortFix3);
            lb_DoingItem->Caption="DoPlaceAutoTray FIX3";
            fMain->Start("DoPlaceFix3");
            iTask=100;
            break;
        case 100:
            OutArmSetupTeachCtl(OutOfsFix3, true);
            break;
        default:
            DoNext();
            break;
    }
}
//---------------------------------------------------------------------------
void TfAutoTeach::DoPlaceAuto1()
{
    UpdateInfoOffsetPosition();
    switch(iTask)
    {
        case 0:
            ShowFormAndActiveTabSheet(ts_DoPlaceAutoTray, "DoPlaceAutoTray");
            LastSet.bNeedSetupTeach=true;
            InitialInfoPlaceFixAutoTray("Unloader Auto1 Position", fOffSet->sbAuto1, OutOfsAuto1, fOffSet->btnSortAuto1);
            lb_DoingItem->Caption="DoPlaceAutoTray Auto1";
            fMain->Start("DoPlaceAuto1");
            iTask=100;
            break;
        case 100:
            OutArmSetupTeachCtl(OutOfsAuto1, true);
            break;
        default:
            DoNext();
            break;
    }
}
//---------------------------------------------------------------------------
void TfAutoTeach::DoPlaceAuto2()
{
    UpdateInfoOffsetPosition();
    switch(iTask)
    {
        case 0:
            ShowFormAndActiveTabSheet(ts_DoPlaceAutoTray, "DoPlaceAutoTray");
            LastSet.bNeedSetupTeach=true;
            InitialInfoPlaceFixAutoTray("Unloader Auto2 Position", fOffSet->sbAuto2, OutOfsAuto2, fOffSet->btnSortAuto2);
            lb_DoingItem->Caption="DoPlaceAutoTray Auto2";
            fMain->Start("DoPlaceAuto2");
            iTask=100;
            break;
        case 100:
            OutArmSetupTeachCtl(OutOfsAuto2, true);
            break;
        default:
            DoNext();
            break;
    }
}
//---------------------------------------------------------------------------
void TfAutoTeach::DoPlaceAuto3()
{
    UpdateInfoOffsetPosition();
    switch(iTask)
    {
        case 0:
            ShowFormAndActiveTabSheet(ts_DoPlaceAutoTray, "DoPlaceAutoTray");
            LastSet.bNeedSetupTeach=true;
            InitialInfoPlaceFixAutoTray("Unloader Auto3 Position", fOffSet->sbAuto3, OutOfsAuto3, fOffSet->btnSortAuto3);
            lb_DoingItem->Caption="DoPlaceAutoTray Auto3";
            fMain->Start("DoPlaceAuto3");
            iTask=100;
            break;
        case 100:
            OutArmSetupTeachCtl(OutOfsAuto3, true);
            break;
        default:
            DoNext();
            break;
    }
}
//---------------------------------------------------------------------------
void TfAutoTeach::SetShuttleNeedTeaching()
{
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<2; j++)
        {
            if((TestIF.iTestMode==_10Site2X5 ||
                TestIF.iTestMode==_12Site2X6 ||
                TestIF.iTestMode==_16Site2X8 ||
                TestIF.iTestMode==_32Site4X8N )==false && j==1)
            {   //InOutShuttle 都要吸放1次
                LastSet.bNeedInShuttleTeach[i][j]=false;
                LastSet.bNeedOutShuttleTeach[i][j]=false;
            }
            else//InOutShuttle 都要吸放兩次
            {
                LastSet.bNeedInShuttleTeach[i][j]=true;
                LastSet.bNeedOutShuttleTeach[i][j]=true;
            }
        }
    }
}
//---------------------------------------------------------------------------
void TfAutoTeach::StartAutoTeach(bool _bopen)
{
    InitialData();
    SetState(_bopen?1:0);
    IniConfig.bA30SetupTeachFunction=_bopen?true:false;
    iATStatus=_bopen?Run:Idle;
    if(_bopen)
    {
        SetShuttleNeedTeaching();
        SetLoaderCount();
        LastSet.iATLoaderCount=0;
    }
    else
    {
        if(bIsFormShow)
            Close();
    }
}
//---------------------------------------------------------------------------
void TfAutoTeach::SetState(int _state)
{
    iAutoTeachStep=(_state>eATTotalAction || _state<0)?0:_state;
    LastSet.iAutoTeachStep=iAutoTeachStep;
}
//---------------------------------------------------------------------------
int TfAutoTeach::GetState()
{
    return iAutoTeachStep;
}
//---------------------------------------------------------------------------
void TfAutoTeach::DoNext()
{
    CheckNextStep();
    iTask=0;
    iTaskPlaceToAuto=0;
    SetState(iNextStep);
    ShowInfoMsg("",false);
    CloseAllForm();
}
//---------------------------------------------------------------------------
void TfAutoTeach::DoHome()
{
    switch(iTask)
    {
        case 0:
            LastSet.bNeedSetupTeach=false;
            if(fAllMotorHome)
                DoNext();
            else
                iTask=100;
            break;
        case 100:
            fMain->Start("AutoTeach::DoHome");
            ShowInfoMsg("Do Homing!", false);
            ShowFormAndActiveTabSheet(ts_DoHomeing, "DoHomeing");
            iTask=200;
            break;
        case 200:
            if(fAllMotorHome==false)
                 fMain->Home("AutoTeach");
            iTask=300;
            break;
        case 300:
            if(fAllMotorHome)
                DoNext();
            break;
        default:
            DoNext();
            break;
    }
}
//---------------------------------------------------------------------------
void TfAutoTeach::DoInputShuttleDoubleDeviceSensor()
{
    switch(iTask)
    {
        case 0:
            ShowFormAndActiveTabSheet(ts_DoShuttleSensorCheckL,"DoShuttleSensorCheckL");
            LastSet.bNeedSetupTeach=false;
            iTask=100;
            break;
        //check sh1 is in left S
        case 100:
            ShowSetFormPos(fShuttleMove,true,true);
            iTask=200;     //sh1 not in left
            break;
        case 200:
            fShuttleMove->ShuttleMoveClick(fShuttleMove->btShu1Left);
            iTask=300;
            break;
        case 300:
            if(SystemStart==false)
            {
                ShowSetFormPos(fShuttleMove,false,false);
                iTask=400;
            }
            break;
        //check sh1 is in left E
        //check sh2 is in left S
        case 400:
            ShowSetFormPos(fShuttleMove,true,true);
            iTask=500;
            break;
        case 500:
            fShuttleMove->ShuttleMoveClick(fShuttleMove->btShu2Left);
            iTask=600;
            break;
        case 600:
            if(SystemStart==false)
            {
                ShowSetFormPos(fShuttleMove,false);
                iTask=700;
            }
            break;
        //check sh2 is in left E
        case 700:
            ShowSetFormPos(fCCLink,true);
            iTask=800;
            break;
        case 800:
            ShowInfoMsg("Finish!", false);
            //wait
            break;
        default:
            DoNext();
            break;
    }
}
//---------------------------------------------------------------------------
void TfAutoTeach::DoOutputShuttleSensor()
{
    switch(iTask)
    {
        case 0:
            ShowFormAndActiveTabSheet(ts_DoShuttleSensorCheckR, "DoShuttleSensorCheckR");
            LastSet.bNeedSetupTeach=false;
            iTask=100;
         break;
        //check sh1 is in right S
        case 100:
            ShowSetFormPos(fShuttleMove, true, true);
            iTask=200;
            break;
        case 200:
            fShuttleMove->ShuttleMoveClick(fShuttleMove->btScanOutShu1);  //Sam 20211130 : 改為 btScanOutShu1
            iTask=300;
            break;
        case 300:
            if(SystemStart==false)
            {
                ShowSetFormPos(fShuttleMove, false);
                iTask=400;
            }
            break;
        //check sh1 is in right E
        //check sh2 is in right S
        case 400:
            ShowSetFormPos(fShuttleMove, true, true);
            iTask=500;
            break;
        case 500:
            fShuttleMove->ShuttleMoveClick(fShuttleMove->btScanOutShu2);     //Sam 20211130 : 改為 btScanOutShu2
            iTask=600;
            break;
        case 600:
            if(SystemStart==false)
            {
                ShowSetFormPos(fShuttleMove, false);
                iTask=700;
            }
            break;
        //check sh2 is in right E
        case 700:
            ShowSetFormPos(fShuttleMove, true);   //Sam 20211130 : 改顯示 fShuttleMove
            iTask=800;
            break;
        case 800:
            //wait
            ShowInfoMsg("Finish!", false);
            break;
        default:
            DoNext();
            break;
    }
}
//---------------------------------------------------------------------------
void TfAutoTeach::DoAutoHeight()
{
    UpdateInfoAutoHeight();
    switch(iTask)
    {
        case 0:
            ShowFormAndActiveTabSheet(ts_DoAutoHeight, "DoAutoHeigh");
            ShowSetFormPos(fContact,true, true);
            ShowInfoMsg("Do Auto Height Start!", false);
            LastSet.bNeedSetupTeach=false;
            iTask=100;
            break;
        case 100:
            fContact->rbAutoHeight->Checked=true;
            fMain->Start("fAutoTeach::DoAutoHeight");
            iTask=200;
            break;
        case 200:
            if(SystemStart)
                iTask=250;
            break;
        case 250:
            if(fContact->CarlibrationTask!=1)  //JimmyChiu 20211228 : 確保一定執行Auto Heigh
                iTask=300;
            break;
        case 300:
            if(fContact->CarlibrationTask==1)
            {
                fContact->spbSaveClick(fContact->spbSave);
                ShowSetFormPos(fContact, false);
                ShowInfoMsg("Finish! Please click Next!", true);
                iTask=400;
            }
            else
            {
                ShowInfoMsg("Running!", false);
            }
            break;
        case 400:
            //wait
            break;
        default:
            DoNext();
            break;
    }
}
//---------------------------------------------------------------------------
void TfAutoTeach::DoLoaderPosition()
{
    //read offset info
    UpdateInfoOffsetPosition();
    switch(iTask)
    {
        case 0:
            ShowFormAndActiveTabSheet(ts_DoPickLoaderTray, "DoPickLoaderTray");
            ShowSetFormPos(fOffSet, true, true);
            fOffSet->SpBotSelClick(fOffSet->sbLoader);
            InArmSetupTeachCtl(InOfsLoader, true);
            ShowInfoMsg("Loader Position Offset", false);
            LastSet.bNeedSetupTeach=true;
            fMain->Start("fAutoTeach::DoLoaderPosition");
            iTask=100;
            break;
        case 100:
            ShowInfoMsg("Ready to Tuning!", false);
            iTask=200;
            break;
        case 200:
            //wait
            break;
        default:
            DoNext();
            break;
    }
}
//---------------------------------------------------------------------------
void TfAutoTeach::DoPlaceHotPlate()
{
    //read offset info
    UpdateInfoOffsetPosition();
    switch(iTask)
    {
        case 0:
            if(GetState()==eATHP1)
            {
                fOffSet->SpBotSelClick(fOffSet->sbHp1);
                InArmSetupTeachCtl(InOfsHP1, true);
            }
            else
            {
                fOffSet->SpBotSelClick(fOffSet->sbHp2);
                InArmSetupTeachCtl(InOfsHP2, true);
            }
            ShowFormAndActiveTabSheet(ts_DoPlaceHotPlateTray,"DoPlaceHotPlateTray");
            lb_DoingItem->Caption="DoPlaceHotPlateTray";
            ShowSetFormPos(fOffSet, true, true);
            ShowInfoMsg("Place Hot Plate Offset", false);
            LastSet.bNeedSetupTeach=true;
            fMain->Start("fAutoTeach::DoPlaceHotPlate");
            iTask=100;
            break;
        case 100:
            ShowInfoMsg("Ready to Tuning!", false);
            iTask=200;
            break;
        case 200:
            //wait
            break;
        default:
            DoNext();
            break;
    }
}
//---------------------------------------------------------------------------
void TfAutoTeach::DoInputShuttlePosition()
{
    //read offset info
    UpdateInfoOffsetPosition();
    switch(iTask)
    {
        case 0:
            if(GetState()==eATInShuttle1L)
            {
                fOffSet->SpBotSelClick(fOffSet->sbInSh1);
                InArmSetupTeachCtl(InOfsInSh1, true);
                ShowInfoMsg("DoPlaceShuttle1_Left", false);
            }
            else if(GetState()==eATInShuttle1R)
            {
                fOffSet->SpBotSelClick(fOffSet->sbInSh1RA);
                InArmSetupTeachCtl(InOfsInSh1RA, true);
                ShowInfoMsg("DoPlaceShuttle1_Right", false);
            }
            else if(GetState()==eATInShuttle2L)
            {
                fOffSet->SpBotSelClick(fOffSet->sbInSh2);
                InArmSetupTeachCtl(InOfsInSh2, true);
                ShowInfoMsg("DoPlaceShuttle2_Left", false);
            }
            else if(GetState()==eATInShuttle2R)
            {
                fOffSet->SpBotSelClick(fOffSet->sbInSh2RA);
                InArmSetupTeachCtl(InOfsInSh2RA, true);
                ShowInfoMsg("DoPlaceShuttle2_Right", false);
            }
            else
            {
                iTask=999;
                break;
            }
            ShowFormAndActiveTabSheet(ts_DoPlaceShuttle, "DoPlaceShuttle");
            ShowSetFormPos(fOffSet, true, true);
            LastSet.bNeedSetupTeach=true;
            fMain->Start("fAutoTeach::DoInputShuttlePosition");
            iTask=100;
            break;
        case 100:
            //iTask=200;
            break;
        default:
            DoNext();
            break;
    }
}
//---------------------------------------------------------------------------
void TfAutoTeach::DoOutputShuttlePosition()
{
    //read offset info
    UpdateInfoOffsetPosition();
    switch(iTask)
    {
        case 0:
            if(GetState()==eATOutShuttle1L)
            {
                fOffSet->SpBotSelClick(fOffSet->sbOutSh1);
                OutArmSetupTeachCtl(OutOfsOutSh1,true);
                ShowInfoMsg("DoPickShuttle1_Left",false);
            }
            else if(GetState()==eATOutShuttle1R)
            {
                fOffSet->SpBotSelClick(fOffSet->sbOutSh1RA);
                OutArmSetupTeachCtl(OutOfsOutSh1RA,true);
                ShowInfoMsg("DoPickShuttle1_Right",false);
            }
            else if(GetState()==eATOutShuttle2L)
            {
                fOffSet->SpBotSelClick(fOffSet->sbOutSh2);
                OutArmSetupTeachCtl(OutOfsOutSh2,true);
                ShowInfoMsg("DoPickShuttle2_Left",false);
            }
            else if(GetState()==eATOutShuttle2R)
            {
                fOffSet->SpBotSelClick(fOffSet->sbOutSh2RA);
                OutArmSetupTeachCtl(OutOfsOutSh2RA,true);
                ShowInfoMsg("DoPickShuttle2_Right",false);
            }
            else
            {
                iTask=999;
                break;
            }
            ShowFormAndActiveTabSheet(ts_DoPickShuttle,"DoPickShuttle");
            ShowSetFormPos(fOffSet,true,true);
            LastSet.bNeedSetupTeach=true;
            fMain->Start("fAutoTeach::DoOutputShuttlePosition");
            iTask=100;
            break;
        case 100:
            //iTask=200;
            break;
        default:
            DoNext();
            break;
    }
}
//---------------------------------------------------------------------------
void TfAutoTeach::DoFinish()
{
    switch(iTask)
    {
        case 0:
           if(SystemStart==false)
                fMain->Start("fAutoTeach::DoFinish");
        case 100:
            if(iOneCycle==0)
            {
                ShowInfoMsg("OneCycle Finish.",false);
                iTask=200;
            }
            break;
        case 200:
            InArmSetupTeachCtl(InOfsLoader,false);
            InArmSetupTeachCtl(InOfsHP1,false);
            InArmSetupTeachCtl(InOfsHP2,false);
            InArmSetupTeachCtl(InOfsInSh1,false);
            InArmSetupTeachCtl(InOfsInSh2,false);
            OutArmSetupTeachCtl(OutOfsOutSh1,false);
            OutArmSetupTeachCtl(OutOfsOutSh2,false);
            OutArmSetupTeachCtl(OutOfsAuto1,false);
            OutArmSetupTeachCtl(OutOfsAuto2,false);
            OutArmSetupTeachCtl(OutOfsAuto3,false);
            OutArmSetupTeachCtl(OutOfsFix1,false);
            OutArmSetupTeachCtl(OutOfsFix2,false);
            OutArmSetupTeachCtl(OutOfsFix3,false);
            sb_DoRetry->Visible=false;
            sb_DoNext->Visible=false;
            ShowInfoMsg("Auto Teaching Finish.",true);
            StartAutoTeach(false);
            fMain->BtnPauseClick(fMain);
            Close();
            break;
        default:
            DoNext();
            break;
    }
}
//---------------------------------------------------------------------------
void TfAutoTeach::DoErrorStep()
{
    ShowInfoMsg("Auto Teaching Error.",true);
    StartAutoTeach(false);
    fMain->BtnPauseClick(fMain);
}
//---------------------------------------------------------------------------
bool TfAutoTeach::CheckGPIBisRun()
{
    if(TestIF.iTestType==GPIB_MODE || TestIF.iTestType==RS232_MODE)
        if(fMain->bFind==false)
            return false;
    return true;
}
//---------------------------------------------------------------------------
void TfAutoTeach::UpdateInfoAutoHeight()
{
    //Motor torque (%)
    edTorue0->Caption=fContact->PnlTorue0->Caption;
    edTorue1->Caption=fContact->PnlTorue1->Caption;
    //Actual contact height
    Label321->Caption=fContact->palArm1Height->Caption;
    Label322->Caption=fContact->palArm2Height->Caption;
    //Shuttle pick height
    Label315->Caption=fContact->edPickUp1->Text;
    Label316->Caption=fContact->edPickUp2->Text;
    //Shuttle release height
    Label318->Caption=fContact->edReleaseHeight1->Text;
    Label319->Caption=fContact->edReleaseHeight2->Text;
}
//---------------------------------------------------------------------------
void TfAutoTeach::UpdateInfoOffsetPosition()
{
    ed_XOffsetValue->Text=fOffSet->edArmX->Text;
    ed_YOffsetValue->Text=fOffSet->edArmY->Text;
    ed_XPitchOffsetValue->Text=fOffSet->edPitchX1->Text;
    ed_ZOffsetValue->Text=fOffSet->edPickUp->Text;
//    ed_ZOffsetValue->Text=(GetState()==eATHP1 ||
//                           GetState()==eATHP2 ||
//                           GetState()==eATOutShuttle1L ||
//                           GetState()==eATOutShuttle1R ||
//                           GetState()==eATOutShuttle2L ||
//                           GetState()==eATOutShuttle2R
//                          )?fOffSet->edPickUp->Text:fOffSet->edRelease->Text;
}
//---------------------------------------------------------------------------
void TfAutoTeach::InitialInfoPlaceFixAutoTray(AnsiString asPoiMsg,TObject *Sender,int iPosIndex,TObject *Sortbtn)
{
    lb_DoingItem->Caption=asPoiMsg;
    fOffSet->SpBotSelClick(Sender);
    fOffSet->btnSortAuto1Click(Sortbtn);
    OutArmSetupTeachCtl(iPosIndex,true);
    ShowSetFormPos(fOffSet,true,true);
    ShowInfoMsg(asPoiMsg+" Initial",false);
}
//---------------------------------------------------------------------------
void TfAutoTeach::ShowInfoMsg(AnsiString asMsg,bool bPopMSG)
{
    lb_Message->Caption = asMsg;
    if(bPopMSG)
        ShowMyMessage(asMsg);
}
//---------------------------------------------------------------------------
void TfAutoTeach::ShowErrorMsg(AnsiString asMsg,bool bPopMSG)
{
    iATStatus=Pause;
    ShowInfoMsg(asMsg,bPopMSG);
}
//---------------------------------------------------------------------------
void TfAutoTeach::InArmSetupTeachCtl(int iIndex,bool bStart)
{
    if(bStart)
    {
        bInArmSetupTeach[iIndex]=false;
        iInArmPickPlaceCnt[iIndex]=0;
    }
    else
    {
        bInArmSetupTeach[iIndex]=true;
        iInArmPickPlaceCnt[iIndex]=10;
    }
}
//---------------------------------------------------------------------------
void TfAutoTeach::OutArmSetupTeachCtl(int iIndex,bool bStart)
{
    if(bStart)
    {
        bOutArmSetupTeach[iIndex]=false;
//        iOutArmPickPlaceCnt[iIndex]=0;
    }
    else
    {
        bOutArmSetupTeach[iIndex]=true;
//        iOutArmPickPlaceCnt[iIndex]=10;
    }
}
//---------------------------------------------------------------------------
double TfAutoTeach::GetAjustLength()
{
    return atof(rg_AdjustUnit->Items->Strings[rg_AdjustUnit->ItemIndex].c_str());
}
//---------------------------------------------------------------------------
void __fastcall TfAutoTeach::sb_AutoOffsetYFrontClick(TObject *Sender)
{
    fOffSet->sb_AutoOffsetUpClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfAutoTeach::sb_AutoOffsetXLeftClick(TObject *Sender)
{
    fOffSet->sb_AutoOffsetLeftClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfAutoTeach::sb_AutoOffsetYRearClick(TObject *Sender)
{
    fOffSet->sb_AutoOffsetDownClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfAutoTeach::sb_AutoOffsetZUpClick(TObject *Sender)
{
    if(SystemStart)
        return;
    fOffSet->edPickUp->Text=FormatFloat("0.00", atof(fOffSet->edPickUp->Text.c_str())+GetAjustLength());
    fOffSet->spbSaveClick(this);
    fMain->Start("sb_AutoOffsetZUpClick");
}
//---------------------------------------------------------------------------
void __fastcall TfAutoTeach::sb_AutoOffsetZDownClick(TObject *Sender)
{
    if(SystemStart)
        return;
    fOffSet->edPickUp->Text=FormatFloat("0.00", atof(fOffSet->edPickUp->Text.c_str())-GetAjustLength());
    fOffSet->spbSaveClick(this);
    fMain->Start("sb_AutoOffsetZDownClick");
}
//---------------------------------------------------------------------------
void __fastcall TfAutoTeach::sb_AutoOffsetPitchYOpenClick(TObject *Sender)
{
    if(SystemStart)
        return;
    fOffSet->edPitchY->Text=FormatFloat("0.00", atof(fOffSet->edPitchY->Text.c_str())+GetAjustLength());
    fOffSet->spbSaveClick(this);
    fMain->Start("sb_AutoOffsetPitchYOpenClick");
}
//---------------------------------------------------------------------------
void __fastcall TfAutoTeach::sb_AutoOffsetPitchXOpenClick(TObject *Sender)
{
    if(SystemStart)
        return;
    fOffSet->edPitchX1->Text=FormatFloat("0.00", atof(fOffSet->edPitchX1->Text.c_str())+GetAjustLength());
    fOffSet->spbSaveClick(this);
    fMain->Start("sb_AutoOffsetPitchXOpenClick");
}
//---------------------------------------------------------------------------
void __fastcall TfAutoTeach::sb_AutoOffsetPitchYCloseClick(TObject *Sender)
{
    if(SystemStart)
        return;
    fOffSet->edPitchY->Text=FormatFloat("0.00", atof(fOffSet->edPitchY->Text.c_str())-GetAjustLength());
    fOffSet->spbSaveClick(this);
    fMain->Start("sb_AutoOffsetPitchYCloseClick");
}
//---------------------------------------------------------------------------
void __fastcall TfAutoTeach::sb_AutoOffsetPitchXCloseClick(TObject *Sender)
{
    if(SystemStart)
        return;
    fOffSet->edPitchX1->Text=FormatFloat("0.00", atof(fOffSet->edPitchX1->Text.c_str())-GetAjustLength());
    fOffSet->spbSaveClick(this);
    fMain->Start("sb_AutoOffsetPitchXCloseClick");
}
//---------------------------------------------------------------------------
void TfAutoTeach::CloseAllForm()
{
    ShowSetFormPos(fCCLink, false);
    ShowSetFormPos(fOffSet, false);
    ShowSetFormPos(fShuttleMove, false);
    ShowSetFormPos(fContact, false);
    fAllMotorHome=true;
}
//---------------------------------------------------------------------------
int TfAutoTeach::SetAllICInOutArm2Fix3()
{
    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            OutArmSuck.iWhichAuto[i][j]=eFix3;
        }
    }
    return eFix3;
}
//---------------------------------------------------------------------------
int TfAutoTeach::GetSocketPickUpOffsetWhenAutoTeachOnly()
{
    return IniConfig.iA56SocketPickUpOffsetWhenAutoTeachOnly;
}
//---------------------------------------------------------------------------
int TfAutoTeach::GetShuttlePickUpOffsetWhenAutoTeach()
{
    return IniConfig.iA56ShuttlePickUpOffsetWhenAutoTeach;
}
//---------------------------------------------------------------------------
//iPosition = 0:Front 1:Back 2:both
void TfAutoTeach::SetIndexBinError(int iPosition)
{
    if(IsRun()==false)
        return;
    for(int i=0; i<MAX_Index_Row; i++)
    {
        for(int j=0; j<NEW_MAX_Index_Col; j++)
        {
            if(iPosition==0 || iPosition==2)
            {
                if(FTestSuck.Item[i][j]!=NULL_IC && FTestSuck.Item[i][j]!=HAS_NULL_IC && FTestSuck.Item[i][j]<TEST_PASS)
                {
                    FTestSuck.SetItemData(i, j, TEST_PASS+iTestBinCount);
                    FTestSuck.PordRec[i][j].AddTestResultRecord(iTestBinCount, "AutoTeach"); //Frank 20160505 add
                }
            }
            if(iPosition==1 || iPosition==2)
            {
                if(BTestSuck.Item[i][j]!=NULL_IC && BTestSuck.Item[i][j]!=HAS_NULL_IC && BTestSuck.Item[i][j]<TEST_PASS)
                {
                    BTestSuck.SetItemData(i, j, TEST_PASS+iTestBinCount);
                    BTestSuck.PordRec[i][j].AddTestResultRecord(iTestBinCount, "AutoTeach"); //Frank 20160505 add
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
void TfAutoTeach::CheckLoaderCountNeedOneCycle()
{
    if(CosFunction.bManualSteplAutoTeach==false || IniConfig.bA56EnableAutoTeachFunciton==false || iOneCycle==1 || IsRun()==false)
        return;
    LastSet.iATLoaderCount++;
    if(LastSet.iATLoaderCount>=iLoaderCnt)
    {
        fMain->BtnOneCycleClick(fMain);
    }
}
//---------------------------------------------------------------------------
void TfAutoTeach::SetNextStep(int iNext)
{
    if(iNext>eATActionFinish)
    {
        iNext=eATActionFinish;
        return;
    }
    bStepFinish[iNext]=true;
    iNextStep=iNext;
}
//---------------------------------------------------------------------------
bool TfAutoTeach::CheckNextStep()//false:return   true:donext
{
    switch(GetState())
    {
        case eATHome:
            SetNextStep(eATInShuttleSensor);
            return true;
        case eATInShuttleSensor:
            SetNextStep(eATOutShuttleSensor);
            return true;
        case eATOutShuttleSensor:
            SetNextStep(eATIndexArm);
            return true;
        case eATIndexArm:
            SetNextStep(eATLoader);
            return true;
        case eATLoader:
            if(LastSet.iTemperature==Tempture_Ambient)
            {
                bStepFinish[eATHP1]=true;
                bStepFinish[eATHP2]=true;
                SetNextStep(eATInShuttle1L);
            }
            else if(HotPlateForm.iPlateSelect==1)//only sh1
            {
                bStepFinish[eATHP2]=true;
                SetNextStep(eATHP1);
            }
            else if(HotPlateForm.iPlateSelect==2)//only sh2
            {
                bStepFinish[eATHP1]=true;
                SetNextStep(eATHP2);
            }
            else //Hotplate 1 & 2
            {
                SetNextStep(eATHP1);
            }
            return true;
        case eATHP1:
        case eATHP2:
            if(bStepFinish[eATHP1]==false)
                SetNextStep(eATHP1);
            else if(bStepFinish[eATHP2]==false)
                SetNextStep(eATHP2);
            else //all done
            {
                if(TestIF_File.iShuttleMode==0)     //dual sh
                {
                    SetNextStep(eATInShuttle1L);
                }
                else    //single sh
                {
                    if(TestIF_File.iShuttle_Sel==0) //sh1
                    {
                        LastSet.bNeedInShuttleTeach[1][0]=false;
                        LastSet.bNeedInShuttleTeach[1][1]=false;
                        bStepFinish[eATInShuttle2L]=true;
                        bStepFinish[eATInShuttle2R]=true;
                        LastSet.bNeedOutShuttleTeach[1][0]=false;
                        LastSet.bNeedOutShuttleTeach[1][1]=false;
                        bStepFinish[eATOutShuttle2L]=true;
                        bStepFinish[eATOutShuttle2R]=true;
                        SetNextStep(eATInShuttle1L);
                    }
                    else  //sh2
                    {
                        LastSet.bNeedInShuttleTeach[0][0]=false;
                        LastSet.bNeedInShuttleTeach[0][1]=false;
                        bStepFinish[eATInShuttle1L]=true;
                        bStepFinish[eATInShuttle1R]=true;
                        LastSet.bNeedOutShuttleTeach[0][0]=false;
                        LastSet.bNeedOutShuttleTeach[0][1]=false;
                        bStepFinish[eATOutShuttle1L]=true;
                        bStepFinish[eATOutShuttle1R]=true;
                        SetNextStep(eATInShuttle2L);
                    }
                }
            }
            return true;
        case eATInShuttle1R:
            LastSet.bNeedInShuttleTeach[0][1]=false;
        case eATInShuttle1L:
            LastSet.bNeedInShuttleTeach[0][0]=false;
            if(bStepFinish[eATInShuttle1L]==false && LastSet.bNeedInShuttleTeach[0][0])
            {
                SetNextStep(eATInShuttle1L);
            }
            else if(bStepFinish[eATInShuttle1R]==false && LastSet.bNeedInShuttleTeach[0][1])
            {
                SetNextStep(eATInShuttle1R);
            }
            else
            {   //all sh1 done
                bStepFinish[eATInShuttle1R]=true;
                bStepFinish[eATInShuttle1L]=true;
                //is Fix done?
                SetNextStep(eATInShuttle2L);
            }
            return true;
        case eATInShuttle2R:
            LastSet.bNeedInShuttleTeach[1][1]=false;
        case eATInShuttle2L:
            LastSet.bNeedInShuttleTeach[1][0]=false;
            if(bStepFinish[eATInShuttle2L]==false && LastSet.bNeedInShuttleTeach[1][0])
            {
                SetNextStep(eATInShuttle2L);
            }
            else if(bStepFinish[eATInShuttle2R]==false && LastSet.bNeedInShuttleTeach[1][1])
            {
                SetNextStep(eATInShuttle2R);
            }
            else
            {   //all sh1 done
                bStepFinish[eATInShuttle2R]=true;
                bStepFinish[eATInShuttle2L]=true;
                SetNextStep(eATOutShuttle1L);
            }
            return true;
        case eATOutShuttle1R:
            LastSet.bNeedOutShuttleTeach[0][1]=false;
        case eATOutShuttle1L:
            LastSet.bNeedOutShuttleTeach[0][0]=false;
            if(bStepFinish[eATAuto1]==false)
                SetNextStep(eATAuto1);
            else if(bStepFinish[eATOutShuttle1L]==false && LastSet.bNeedOutShuttleTeach[0][0])
                SetNextStep(eATOutShuttle1L);
            else if(bStepFinish[eATOutShuttle1R]==false && LastSet.bNeedOutShuttleTeach[0][1])
                SetNextStep(eATOutShuttle1R);
            else
            {   //all sh1 done
                bStepFinish[eATOutShuttle1R]=true;
                bStepFinish[eATOutShuttle1L]=true;
                SetNextStep(eATOutShuttle2L);
            }
            return true;
        case eATOutShuttle2R:
            LastSet.bNeedOutShuttleTeach[1][1]=false;
        case eATOutShuttle2L:
            LastSet.bNeedOutShuttleTeach[1][0]=false;
            if(bStepFinish[eATOutShuttle2L]==false && LastSet.bNeedOutShuttleTeach[1][0])
            {
                SetNextStep(eATOutShuttle2L);
            }
            else if(bStepFinish[eATOutShuttle2R]==false && LastSet.bNeedOutShuttleTeach[1][1])
            {
                SetNextStep(eATOutShuttle2R);
            }
            else
            {   //all sh1 done
                bStepFinish[eATOutShuttle2R]=true;
                bStepFinish[eATOutShuttle2L]=true;
                SetNextStep(eATActionFinish);
            }
            return true;
        case eATAuto1:
            SetNextStep(eATAuto2);
            return true;
        case eATAuto2:
            SetNextStep(eATAuto3);
            return true;
        case eATAuto3:
            SetNextStep(eATFix1);
            return true;
        case eATFix1:
            SetNextStep(eATFix2);
            return true;
        case eATFix2:
            SetNextStep(eATFix3);
            return true;
        case eATFix3:
            SetNextStep(eATOutShuttle1R);
            return true;
        case eATActionFinish://finish
        default:
            SetNextStep(eATActionFinish);
            return false;
    }
}
//---------------------------------------------------------------------------
bool TfAutoTeach::IsArmNeedPause(AnsiString asWhich)
{
    if(IsRun()==false)
        return false;
    if(GetState()==eATInShuttle1L               &&
       asWhich.Pos("Input Shuttle1")>0          &&
       InArmSuck.iWhichKit==0                   &&
       LastSet.bNeedInShuttleTeach[0][0]
    )
        return true;

    if(GetState()==eATInShuttle1R               &&
       asWhich.Pos("Input Shuttle1")>0          &&
       InArmSuck.iWhichKit==1                   &&
       LastSet.bNeedInShuttleTeach[0][1]
    )
        return true;

    if(GetState()==eATInShuttle2L               &&
       asWhich.Pos("Input Shuttle2")>0          &&
       InArmSuck.iWhichKit==0                   &&
       LastSet.bNeedInShuttleTeach[1][0]
    )
        return true;

    if(GetState()==eATInShuttle2R               &&
       asWhich.Pos("Input Shuttle2")>0          &&
       InArmSuck.iWhichKit==1                   &&
       LastSet.bNeedInShuttleTeach[1][1]
    )
        return true;

    if(GetState()==eATOutShuttle1L              &&
       asWhich.Pos("Output Shuttle1")>0         &&
       iATOutArmWhichKit==0                     &&
       LastSet.bNeedOutShuttleTeach[0][0]
    )
        return true;

    if(GetState()==eATOutShuttle1R              &&
       asWhich.Pos("Output Shuttle1")>0         &&
       iATOutArmWhichKit==1                     &&
       LastSet.bNeedOutShuttleTeach[0][1]
    )
        return true;

    if(GetState()==eATOutShuttle2L              &&
       asWhich.Pos("Output Shuttle2")>0         &&
       iATOutArmWhichKit==0                     &&
       LastSet.bNeedOutShuttleTeach[1][0]
    )
        return true;

    if(GetState()==eATOutShuttle2R              &&
       asWhich.Pos("Output Shuttle2")>0         &&
       iATOutArmWhichKit==1                     &&
       LastSet.bNeedOutShuttleTeach[1][1]
    )
        return true;

    return false;
}
//---------------------------------------------------------------------------
bool TfAutoTeach::IsShuttle1Enable()
{
    if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1) //only Shuttle 2
        return false;
    else
        return true;
}
//---------------------------------------------------------------------------
bool TfAutoTeach::IsShuttle2Enable()
{
    if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==0) //only Shuttle 1
        return false;
    else
        return true;
}
//---------------------------------------------------------------------------
void TfAutoTeach::CheckAndAvoidHangUp()
{
    if((bStepFinish[eATFix3] &&
        bStepFinish[eATOutShuttle1R] &&
        bStepFinish[eATOutShuttle1L] &&
        bStepFinish[eATOutShuttle2R] &&
        bStepFinish[eATOutShuttle2L] )||
       (LastSet.iATLoaderCount>=iLoaderCnt))
    {
        if(iOneCycle==0)
            iOneCycle=1;
    }
}
//---------------------------------------------------------------------------
bool TfAutoTeach::IsKeyStartEnable()
{
    if(IsRun()==true &&
       (GetState()==eATHome ||
        GetState()==eATInShuttleSensor ||
        GetState()==eATOutShuttleSensor ||
        GetState()==eATIndexArm  ))
    {
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
void TfAutoTeach::DoAutoTeachStart()
{
    IniConfig.bA30SetupTeachFunction=true;
    if(HasICUnderMachine()||HasAnyICInMachine())
    {
        ShowMyMessage("Can't Run Auto Alignment Mode When The Machine Has IC!");
        StartAutoTeach(false);
    }
    else if(bIsFormShow)
    {/*pass*/}
    else
    {
        StartAutoTeach(true);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAutoTeach::ed_XOffsetValueMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_DOUBLE, 2, true, InputLimit.iOffsetXYHigh, InputLimit.iOffsetXYLow);
}
//---------------------------------------------------------------------------
void __fastcall TfAutoTeach::ed_ZOffsetValueMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_DOUBLE, 2, true, InputLimit.iOffsetZHigh, InputLimit.iOffsetZLow);
}
//------------------------------------------------------------------
void bInoutArmTeach()                      //kevin 20210401
{
    Zteach->ShowModal();
}
//------------------------------------------------------------------------------
void InitInArmAutoZTask()
{
    iAutoTeachInArmZTask=1;
}
//------------------------------------------------------------------------------
//KaiChen 20180117 (Steven) : InArm Auto Hight From Loader Tray
//==>
bool AutoTeachInArmZLoaderTray(bool Reset, int iInArm)                          //QQQ
{
    int i=0, j=0, k=0;
    int &iTask=iAutoTeachInArmZTask;
    static int iZVacuumbufferPos[10]={0};   //Z軸吸取高度
    static int iZVacuumPos1[10]={0};        //Z軸吸取高度
    static int iZMot=0;                     //使用 Z軸馬達

    static int iRowCT=0;
    static int iX=0, iY=0;

    if(Reset)
    {
        iAutoTeachInArmZTask=1;
        for(i=0; i<8; i++)
        {
            iZVacuumPos1[i]=0;
        }
        return false;
    }

    switch(iTask)
    {
        case 1:  //初始化參數
            iTask=2;
        case 2:
            if(iInArm==0)
            {
                if(Zteach->OpenSuckOn(InArmSuck, true))//開真空 確認真空是否正常
                {
                    iTask=1;    //真空產生器有問題無法做AUTO Z
                }
                else
                {
                    if(Sen[SnLoaderSureTray].IsOff()==true)          //Frank QQ
                    {
                        InitLoadNewICTrayTask();
                        iTask=10;
                    }
                    else
                    {
                        iTask=20;
                    }
                }
            }
            break;
        case 10:                        //Frank QQ
            if(DoLoadNewICTray())
            {
                InitLoadNewICTrayTask();
                iTask=11;
            }
            break;
        case 11:                        //Frank QQ
            if(DoSupplyNewICTray())     //Supply IC from Loader car
            {
                iTask=20;
            }
            break;
        case 20:                        //Frank QQ
            if(Sen[SnLoaderSureTray].IsOff()==false)//檢查 Loader 是否有 Tray
            {
                if(Cylinder[C_LoaderEdgePush].Enable)
                {
                    Cylinder[C_LoaderEdgePush].On();
                    DoSupplyNewICTrayDelay_AutoTeach.SetSecAndOn(0.15);
                    iTask=25;
                }
            }
            else
            {
                ShowMyMessage("Please Check Loader Has Tray !!! ","請確認 Loader 是否有Tray !!!");
            }
            break;
        case 25:
            if(DoSupplyNewICTrayDelay_AutoTeach.Off())
            {
                Cylinder[C_LoaderEdgePush].Off();
                DoSupplyNewICTrayDelay_AutoTeach.SetSecAndOn(0.15);
                iTask=30;
            }
            break;
        case 30:
            if(DoSupplyNewICTrayDelay_AutoTeach.Off())
            {
                Cylinder[C_LoaderEdgePush].On();
                DoSupplyNewICTrayDelay_AutoTeach.SetSecAndOn(0.2);
                iTask=35;
            }
            break;
        case 35:
            if(DoSupplyNewICTrayDelay_AutoTeach.Off())
            {
                if(Cylinder[C_TrayY_Fixer].Push())
                {
                    iTask=40;
                }
            }
            break;
        case 40:
            MOT[MMTrayY].InitNewTray(HAS_IC, false, __FUNC__);
            MOT[MMTrayY].fHasTray=true;
            iTask=50;

            break;
        case 50:                         //Frank QQ
            iRowCT=0;
            iInArmOrder=0;
            bInArmToLoaderUsage[iRowCT][0]=Prod.fInArmSuck4x8[InArmSuck.iWhichSht][iRowCT][0];
            bInArmToLoaderUsage[iRowCT][1]=Prod.fInArmSuck4x8[InArmSuck.iWhichSht][iRowCT][1];
            bInArmToLoaderUsage[iRowCT][2]=Prod.fInArmSuck4x8[InArmSuck.iWhichSht][iRowCT][2];
            bInArmToLoaderUsage[iRowCT][3]=Prod.fInArmSuck4x8[InArmSuck.iWhichSht][iRowCT][3];
            if(bInArmToLoaderUsage[iRowCT][0]==false && bInArmToLoaderUsage[iRowCT][1]==false && bInArmToLoaderUsage[iRowCT][2]==false && bInArmToLoaderUsage[iRowCT][3]==false)
            {
                iRowCT=1;
                iInArmOrder=1;
                bInArmToLoaderUsage[iRowCT][0]=Prod.fInArmSuck4x8[InArmSuck.iWhichSht][iRowCT][0];
                bInArmToLoaderUsage[iRowCT][1]=Prod.fInArmSuck4x8[InArmSuck.iWhichSht][iRowCT][1];
                bInArmToLoaderUsage[iRowCT][2]=Prod.fInArmSuck4x8[InArmSuck.iWhichSht][iRowCT][2];
                bInArmToLoaderUsage[iRowCT][3]=Prod.fInArmSuck4x8[InArmSuck.iWhichSht][iRowCT][3];
            }

            Find_InArm_Single(4, iY, iX);

            for(j=0; j<MAX_ARM_Col; j++)
            {
                InArmSuckUse[iRowCT][j]=bInArmToLoaderUsage[iRowCT][j];
                if(bInArmToLoaderUsage[iRowCT][j]==true)
                {
                    iZMot=iRowCT*1+j*2;
                    break;
                }
            }

            iTask=60;
            break;
        case 60:
            if(DoMoveInArmXYToLoader_AutoTeach(iRowCT, iY, iX, false))
            {
                iZVacuumbufferPos[iZMot]=iInArmTeachZ;
                MOT[MInArmZA+iZMot].SetSpeed(5);
                iTask=65;
            }
            break;
        case 65:
            if(DoMoveInArmXYToLoader_AutoTeach(iRowCT, iY, iX, true))
            {
                iZVacuumbufferPos[iZMot]=iInArmTeachZ;
                iTask=70;
            }
            break;

        case 70:
            if(MOT[MInArmZA+iZMot].MotorMove(iZVacuumbufferPos[iZMot]))
            {
                iTask=80;
                MOT[MInArmZA+k].SetSpeed(0.3);
            }
            break;
        case 80:
            iZVacuumbufferPos[iZMot]-=1;
            if(iZVacuumbufferPos[iZMot]<-2200) //判斷Z軸是否失步
            {
                MOT[MInArmZA+iZMot].PCIL132_StopMotor();
                iTask=110;
                return false;
            }
            iTask=85;
            break;
        case 85:
            if(MOT[MInArmZA+iZMot].MotorMove(iZVacuumbufferPos[iZMot]))
            {
                if(Zteach->OpenSuckOn(InArmSuck, true))//開真空 確認真空是否正常
                {
                    MOT[MInArmZA+iZMot].PCIL132_StopMotor();
                    iZVacuumPos1[iZMot]= MOT[MInArmZA+iZMot].ReadPos();
                    if(iZVacuumbufferPos[iZMot]!=iZVacuumPos1[iZMot])
                    {
                        iZVacuumbufferPos[iZMot]=0;
                    }
                    MOT[MInArmZA+iZMot].SetSpeed(5);
                    Zteach->OpenSuckOn(InArmSuck, false);
                    iTask=86;
                    break;
                }
                iTask=80;
            }
            break;
        case 86:            //Frank QQ
            if(MOT[MInArmZA+iZMot].MotorMove(iZVacuumPos1[iZMot]+50))
            {
                iTask=87;
            }
            break;
        case 87:             //Frank QQ
            for(int X=0;X<2;X++)
            {
                for(int Y=0;Y<4;Y++)
                {
                    if(InArmSuck.Suck[X][Y].Enable && InArmSuck.Suck[X][Y].Suck())
                    {
                        InArmSuck.Suck[X][Y].Off();
                    }
                }
            }
            DoSupplyNewICTrayDelay_AutoTeach.SetSecAndOn(1);
            iTask=88;
            break;
        case 88:            //Frank QQ
            if(DoSupplyNewICTrayDelay_AutoTeach.Off())
            {
                iTask=90;
            }
            break;
        case 90:
            if(MoveInArmZToPlateSafe(iTask)==true)
            {
                for(int X=0;X<InArmSuck.iMaxRow;X++)
                {
                    for(int Y=0;Y<InArmSuck.iMaxCol;Y++)
                    {
                        InArmSuck.Suck[X][Y].Normal();
                    }
                }
                iTask=150;
            }
            break;
        case 110: //馬達失步必需重來                  //Inarm Z歸零
            Zteach->InOutArmZHome(iInArm); //kevin 20171101
            iTask=115;
            break;
        case 115:
            if(Zteach->DoZHome()==false) //Z軸歸零
            {
                return false;
            }
            iTask=1;
            break;

        case 150:   //完成
            for(j=0; j<8; j++)    // A C E F
            {
                MOT[MInArmZA+j].SetSpeed(ArmSpeed[InArm].iACDCZSP);
            }

            for(i=0; i<InArmSuck.iMaxRow; i++)
            {
                for(j=0; j<InArmSuck.iMaxCol; j++)
                {
                    if((i+j*2)==iZMot)
                    {
                        if(i==iInArmYBase && j==iInArmXBase)
                        {
                            iInArmAutoZHeight=iZVacuumPos1[iZMot];
                        }
                        else
                        {
                            iInArmAutoZHeight=iZVacuumPos1[iZMot]+Tech.iInArmZHeightSub[i][j]+InArmOffSet[InOfsLoader]->GetPickUp(i, j);
                        }
                        InArmOffSet[InOfsLoader]->SetPickUp(0);
                    }
                }
            }

            return true;
    }
    return  false;
}
//<==
//KaiChen 20180117 (Steven) : InArm Auto Hight From Loader Tray
//------------------------------------------------------------------------------
//KaiChen 20180117 (Steven) : InArm Auto Hight From Loader Tray
//==>
bool DoMoveInArmXYToLoader_AutoTeach(int iSelRow, int iRow, int iCol, bool realMove)
{
    int iXPos=0, iYPos=0;
    int iXVariable[X_PITCH_COUNT];
    int iYVariable  =0;
    int iMovePitchX =KIT_PITCH;
    int iMovePitchY =AutoCalculateInArmYClosePitch();
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    int iOffsetPos=-1;
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    for(int i=0; i<4; i++)
    {
        if(InArmSuckUse[iSelRow][i]==true)
        {
            for(int j=(i+1); j<MAX_ARM_Col; j++)
                InArmSuckUse[iSelRow][j]=false;
            iXPos=Prod.XInArm_Tray_Pick[iSelRow][i]+iCol*Prod.LoadForm.iXPitch+(2-i)*KIT_PITCH/3;

            if(IniConfig.bUseTrayBlockMode)                                     //Frank 20160928 add Subtray Function
                InArmAddBlockPitch(iXPos, iYPos, iRow, iCol, 1);

            if(iXPos>(MOT[MInArmX].Motor->PSoftLimitP-100))
            {
                if(Prod.LoadForm.iXPitch*3<=iXpitchMaxX3 && Prod.LoadForm.iXPitch*3>=iXpitchMinX3)  //Steven 20140520 : 大IC使用Fix時,吸嘴B吸Loader最右側可能會Close Pitch導致飛料
                {
                    iMovePitchX=Prod.LoadForm.iXPitch*3;
                }
                else
                {
                    iMovePitchX=6000;
                }

                iXPos=Prod.XInArm_Tray_Pick[iSelRow][i]+iCol*Prod.LoadForm.iXPitch+(2-i)*iMovePitchX/3;       // close pitch
            }
            else
            {
                iMovePitchX=KIT_PITCH;
            }

            iYPos=Prod.YInArm_Tray_Pick[iSelRow][i]-iRow*Prod.LoadForm.iYPitch+Prod.iTrayKitStartY;
            if(IniConfig.bUseTrayBlockMode)                                     //Frank 20160928 add Subtray Function
                InArmAddBlockPitch(iXPos, iYPos, iRow, iCol, 2);

            iXPosition[i]=iCol;
            iYPosition=iRow;
            break;
        }
    }

    if(realMove==false)
    {
        if(iXPos>=MOT[MInArmX].Motor->PSoftLimitP || iXPos<=MOT[MInArmX].Motor->PSoftLimitN)
            return false;
        if(iYPos>=MOT[MInArmY].Motor->PSoftLimitP || iYPos<=MOT[MInArmY].Motor->PSoftLimitN)
            return false;
        return true;
    }

    for(int i=0; i<MAX_ARM_Row; i++)
    {
        for(int j=0; j<MAX_ARM_Col; j++)
        {
            bZFlag[i][j]=InArmSuckUse[i][j];
            iZPos[i][j] =(bZFlag[i][j])?Prod.ZInArm_Tray_Pick[i][j]:ZSafePos;
        }
    }

    iXPos=iXPos+Prod.iINARM_X_PITCH*iInArmOrder;
    if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||                              //ChungHung 20140304 add for AutoYPitch
       USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                            //Ztex 2024.02.24 Add HT-1132
    {
        if(iInArmOrder==0)                                                      //第一排
            iYPos=iYPos-iMovePitchY;
        else
            iYPos=iYPos;
    }
    else
    {
        iYPos=iYPos+iMovePitchY*iInArmOrder;
    }

    iOffsetPos=GetInArmToLoaderOffset_9045(iSelRow);
    iYVariable=GetInArmPitchY_9045(iMovePitchY, iOffsetPos);
    for(int i=0; i<X_PITCH_COUNT; i++)
        iXVariable[i]=GetInArmPitchX_9045(iMovePitchX, i, iOffsetPos);

    TransferLoaderRatio(&iXPos, &iYPos);                                        //Steven 20141029 : Loader的軟體齒輪比

    DoInArmSuckPreOn(iXPos, iYPos);

    if(InArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, ZAxisNotDown, true))
    {
        return true;
    }
    return false;
}
//<==
//KaiChen 20180117 (Steven) : InArm Auto Hight From Loader Tray
//------------------------------------------------------------------------------
//   移動XY位置
//------------------------------------------------------------------------------
bool bMoveInArmXYToTeach(bool Reset, int ZNumber, int iInArm)
{                                //目前使用吸嘴
    int iX, iY, iVari, iYVariable=0, iXVariable2=0;
    int iXPos=0, iYPos=0, iYPitchData=0;
    static bMove[10]={false, false, false, false, false, false, false, false, false, false};        //kevin 20181203 5->10
    static bOutMove[10]={false, false, false, false, false, false, false, false, false, false};     //kevin 20181203 5->10

    if(Reset)
    {
        for(int i=0; i<10; i++)    //kevin 20210210 5->10
        {
            if(iInArm==0)
                bMove[i]=false;
            else
                bOutMove[i]=false;
        }
        return false;
    }

    if(iInArm==0)//INARM
    {
        iVari=GetInArmPitch_9045(9000);
        if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||                          //kevin 20181203 (Steven) : Y變距
           USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                        //Ztex 2024.02.24 Add HT-1132
        {
            iYVariable =GetOutArmPitchY_9045(6000);   //ChungHung 20131231 alter AutoYPitch
            iXVariable2=GetOutArmPitch2_9045(9000);
            iYPitchData=3300;
        }
        else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                     //QQ
                USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                        //Ztex 2023.12.06 Add HT-1032
        {
        }

        if(ZNumber%2==0)//吸嘴 A C E G
        {
            iXPos=29659-3000*(ZNumber/2);//Tech.iInArmTeachX-3000*(ZNumber/2);

            if(MachineTypeChoice==Type_HT9046_LS)
                iYPos=-69249+iInArmAutoYTeachOffset+iYPitchData;//kevin 20181203//kevin 20171115 add    //jou 2014-10-07 Auto Teach Z 新增 HT9046LS 點位
            else
                iYPos=-52130+iInArmAutoYTeachOffset;//kevin 20171115 add//Tech.iInArmTeachY;

        }
        else
        {
            iXPos=29659-3000*(ZNumber/2);//Tech.iInArmTeachX-3000*(ZNumber/2);

            if(MachineTypeChoice==Type_HT9046_LS)
                iYPos=-69249+6000+iInArmAutoYTeachOffset+iYPitchData;//kevin 20181203//kevin 20171115 add   //jou 2014-10-07 Auto Teach Z 新增 HT9046LS 點位
            else
                iYPos=-52130+6000+iInArmAutoYTeachOffset;//kevin 20171115 add
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

        if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||                          //kevin 20181203 add
           USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                        //Ztex 2024.02.24 Add HT-1132
        {
            if(bMove[3]==false)
                bMove[3]=MOT[MInArmPitchY].MotorMove(iYVariable);  //ChungHung 20131231 alter AutoYPitch
            if(bMove[4]==false)
                bMove[4]=MOT[MInArmPitchX2].MotorMove(iXVariable2); //Steven 20131002 : XY變距
        }
        else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                     //QQ
                USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                        //Ztex 2023.12.06 Add HT-1032
        {
        }
        else
        {
            bMove[3]=true;
            bMove[4]=true;
        }

        if(bMove[0] && bMove[1] && bMove[2]&& bMove[3]&&bMove[4])
            return true;
    }
    else
    {
        iVari=GetOutArmPitch_9045(9000);
        if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||                          //kevin 20181203 (Steven) : Y變距
           USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                        //Ztex 2024.02.24 Add HT-1132
        {
            iYVariable =GetOutArmPitchY_9045(6000);   //ChungHung 20131231 alter AutoYPitch
            iXVariable2=GetOutArmPitch2_9045(9000);
            iYPitchData=3300;
        }
        else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                     //QQ
                USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                        //Ztex 2023.12.06 Add HT-1032
        {
        }

        if(ZNumber%2==0)//吸嘴 A C E G
        {
            iXPos=-20193-3000*(ZNumber/2);//Tech.iInArmTeachX-3000*(ZNumber/2);
            if(MachineTypeChoice==Type_HT9046_LS)
                iYPos=-69249+iOutArmAutoYTeachOffset+iYPitchData;//kevin 20181203 kevin 20171115 add     //jou 2014-10-07 Auto Teach Z 新增 HT9046LS 點位
            else
                iYPos=-52526+iOutArmAutoYTeachOffset;//kevin 20171115 add
        }
        else
        {
            iXPos=-20193-3000*(ZNumber/2);//Tech.iInArmTeachX-3000*(ZNumber/2);
            if(MachineTypeChoice==Type_HT9046_LS)
                iYPos=-69249+6000+iOutArmAutoYTeachOffset+iYPitchData;//kevin 20181203//kevin 20171115 add    //jou 2014-10-07 Auto Teach Z 新增 HT9046LS 點位
            else
                iYPos=-52526+6000+iOutArmAutoYTeachOffset;//kevin 20171115 add
        }
        TransferHotPlateRatio(false, &iXPos, &iYPos);    //Steven 20110324

        iX=iXPos;
        iY=iYPos;
        if(bOutMove[0]==false)
            bOutMove[0]=MOT[MOutArmX].MotorMove(iX);

        if(bOutMove[1]==false)
            bOutMove[1]=MOT[MOutArmY].MotorMove(iY);

        if(bOutMove[2]==false)
            bOutMove[2]=MOT[MOutArmPitch].MotorMove(iVari);

        if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||                          //kevin 20181203 add
           USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                        //Ztex 2024.02.24 Add HT-1132
        {
            if(bOutMove[3]==false)
                bOutMove[3]=MOT[MOutArmPitchY].MotorMove(iYVariable);  //ChungHung 20131231 alter AutoYPitch
            if(bOutMove[4]==false)
                bOutMove[4]=MOT[MOutArmPitchX2].MotorMove(iXVariable2); //Steven 20131002 : XY變距
        }
        else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                     //QQ
                USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                        //Ztex 2023.12.06 Add HT-1032
        {
        }
        else
        {
            bOutMove[3]=true;   //kevin 20190508 : Fixed auto teaching hang up
            bOutMove[4]=true;
        }

        if(bOutMove[0] && bOutMove[1] && bOutMove[2] && bOutMove[3] && bOutMove[4])
            return true;
    }
    #ifdef SOFT_SIMULTE
        return true;
    #else
        return false;
    #endif
}
//---------------------------------------------------------------------------
//kevin 20170930  load tray Z AutoHigh
//---------------------------------------------------------------------------
bool AutoTeachLoadTrayZ(bool Reset, int iInArm, int &iAOATask)                  //QQQ
{
    int i=0, j=0, k=0;
    int &iTask=iAOATask;

    static int iZVacuumbufferPos[10]={0};   //Z軸吸取高度
    static int iZVacuumPos1[10]={0};        //Z軸吸取高度
    static bool bUseZ[10]={false};          //目前已確認 吸嘴
    static int iHome2=0;                    //此次使用吸嘴數
    static int iZMot=0;                     //使用 Z軸馬達

    static int iOutZVacuumbufferPos[10]={0};//Z軸吸取高度
    static int iOutZVacuumPos1[10]={0};     //Z軸吸取高度
    static bool bOutUseZ[10]={false};       //目前已確認 吸嘴
    static int iOutHome2=0;                 //此次使用吸嘴數
    static int iOutZMot=0;                  //使用 Z軸馬達
    AnsiString asSuck[8]={"A","B","C","D","E","F","G","H"};  //kevin 20210224
    AnsiString asLog="";                                     //kevin 20210224
    int iStandZ=0;
    //bool bfail=false;
    int iZ[8]={0};
    static int iCheckCount[3]={0};

    if(Reset)
    {
        for(i=0; i<InArmSuck.iMotRow; i++)
        {
            for(j=0; j<InArmSuck.iMotCol; j++)
            {
                iTask=1;
                if(iInArm==0)
                {
                    bUseZ[k]=false;
                    iZVacuumPos1[i]=0;
                    iHome2=0;
                }
                else
                {
                    bOutUseZ[k]=false;
                    iOutZVacuumPos1[i]=0;
                    iOutHome2=0;
                }
                k++;
            }
            return false;
        }
    }

    switch(iTask)
    {
        case 1:     //初始化參數
            iTask=2;
        case 2:     //真空確認是否正常
            if(iInArm==0)//INARM
            {
                if(Zteach->OpenSuckOn(InArmSuck, true))//開真空 確認真空是否正常
                {
                    //真空產生器有問題無法做AUTO Z
                    iTask=1;
                    //Memo1->Lines->Add("Out arm  vaccume On /Off error"); //kevin 20150714
                }
                else
                {
                    iTask=10;
                }
                bMoveInArmXYToTeach(true, iZMot, InArm);
            }
            else
            {       //OutARM
                if(Zteach->OpenSuckOn(OutArmSuck, true))//開真空 確認真空是否正常
                {
                    //真空產生器有問題無法做AUTO Z
                    iTask=31;
                    //Memo1->Lines->Add("Out arm  vaccume On /Off error"); //kevin 20150714
                }
                else
                    iTask=10;
                bMoveInArmXYToTeach(true, iOutZMot, OutArm);
            }
            break;
        case 10:
            k=0;
            for(i=0; i<InArmSuck.iMotRow; i++)
            {
                for(j=0; j<InArmSuck.iMotCol; j++)
                {
                    if(iInArm==0)
                    {
                        if(bUseZ[k]==false)
                        {
                            MOT[MInArmZA+k].SetSpeed(5);
                            bMoveInArmXYToTeach(true, iZMot, InArm);
                            iTask=11;
                            iZMot=k;
                            return false;
                        }
                    }
                    else
                    {
                        if(bOutUseZ[k]==false)
                        {
                            MOT[MOutArmZA+k].SetSpeed(5);
                            bMoveInArmXYToTeach(true, iOutZMot, OutArm);
                            iTask=11;
                            iOutZMot=k;
                            return false;
                        }
                    }
                    k++;
                }
            }
            iTask=30;
            break;
        case 11:
            if(iInArm==0)//INARM
            {
                if(bMoveInArmXYToTeach(false, iZMot, InArm))
                {
                    iZVacuumbufferPos[iZMot]=iInArmTeachZ;//kevin 20170502 (wei) 快拆吸嘴IN/Out Arm 行程影響AutoTeach -1500;
                    MOT[MInArmZA+iZMot].MotorMove(iZVacuumbufferPos[iZMot]);
                    iTask=12;
                }
            }
            else
            {
                if(bMoveInArmXYToTeach(false, iOutZMot, OutArm))
                {
                    iOutZVacuumbufferPos[iOutZMot]=ioutArmTeachZ;//kevin 20170502 (wei) 快拆吸嘴IN/Out Arm 行程影響AutoTeach -1500;
                    MOT[MOutArmZA+iOutZMot].MotorMove(iOutZVacuumbufferPos[iOutZMot]);
                    iTask=12;
                }
            }
            break;
        case 12:
            if(iInArm==0)//INARM
            {
                if(MOT[MInArmZA+iZMot].MotorMove(iZVacuumbufferPos[iZMot]))
                {
                    iTask=13;
                    MOT[MInArmZA+iZMot].SetSpeed(0.3);
                }
            }
            else
            {
                if(MOT[MOutArmZA+iOutZMot].MotorMove(iOutZVacuumbufferPos[iOutZMot]))
                {
                    iTask=13;
                    MOT[MOutArmZA+iZMot].SetSpeed(0.3);
                }
            }
            break;
        case 13:
            if(iInArm==0)//INARM
            {
                iZVacuumbufferPos[iZMot]-=1;
                if(iZVacuumbufferPos[iZMot]<-3200)      //kevin 20220823 9046AT+ -3000 判斷Z軸是否失步
                {
                    MOT[MInArmZA+iZMot].PCIL132_StopMotor();
                    iTask=20;
                    return false;
                }

                MOT[MInArmZA+iZMot].MotorMove(iZVacuumbufferPos[iZMot]);

                if(Zteach->OpenSuckOn(InArmSuck, true))//開真空 確認真空是否正常
                {
                    MOT[MInArmZA+iZMot].PCIL132_StopMotor();
                    iZVacuumPos1[iZMot]= MOT[MInArmZA+iZMot].ReadPos();
                    if(iZVacuumbufferPos[iZMot]!=iZVacuumPos1[iZMot])
                        iZVacuumbufferPos[iZMot]=0;
                    bUseZ[iZMot]=true;
                    MOT[MInArmZA+iZMot].SetSpeed(5);
                    iTask=14;
                }
            }
            else
            {
                iOutZVacuumbufferPos[iOutZMot]-=1;

                if(iOutZVacuumbufferPos[iOutZMot]<-3200)    //kevin 20220823 9046AT+ -3000 //判斷Z軸是否失步
                {
                    MOT[MOutArmZA+iOutZMot].PCIL132_StopMotor();
                    iTask=20;
                    return false;
                }
                MOT[MOutArmZA+iOutZMot].MotorMove(iOutZVacuumbufferPos[iOutZMot]);

                if(Zteach->OpenSuckOn(OutArmSuck, true))//開真空 確認真空是否正常
                {
                    MOT[MOutArmZA+iOutZMot].PCIL132_StopMotor();
                    iOutZVacuumPos1[iOutZMot]= MOT[MOutArmZA+iOutZMot].ReadPos();
                    if(iOutZVacuumbufferPos[iOutZMot]!=iOutZVacuumPos1[iOutZMot])
                        iOutZVacuumbufferPos[iOutZMot]=0;
                    bOutUseZ[iOutZMot]=true;
                    MOT[MOutArmZA+k].SetSpeed(5);
                    iTask=14;
                }
            }
            break;
        //Sam 20210204 : Auto Teach Loader Z Record Vacuum On off time
        //==>
        case 14:
            if(bRecordVacuumTim)                           //kevin 20210224 記錄計算真空產生器壽命
            {
                if(iInArm==0)
                {
                    VacuumOnOffLog("");
                    VacuumOnOffLog("======>");
                    asLog.sprintf("In Arm %s Head Vaccum Test Start", asSuck[iZMot]);
                    VacuumOnOffLog(asLog);
                    iCheckCount[iInArm]=0;
                    iTask=100;
                }
                else if(iInArm==2)
                {
                    VacuumOnOffLog("");
                    VacuumOnOffLog("======>");
                    asLog.sprintf("Out Arm %s Head Vaccum Test Start", asSuck[iOutZMot]);
                    VacuumOnOffLog(asLog);
                    iCheckCount[iInArm]=0;
                    iTask=200;
                }
            }
            else
            {
                iTask=15;
            }
            break;
        //<==
        //Sam 20210204 : Auto Teach Loader Z Record Vacuum On off time
        case 15:
            if(iInArm==0)
            {
                if(MoveInArmZToPlateSafe(iTask))//kevin 20220823 lost case
                    iTask=10;
            }
            else
            {
                if(MoveOutArmToAutoSafe_9045())//kevin 20220823 lost case
                    iTask=10;
            }
            break;
        case 20: //馬達失步必需重來                  //Inarm Z歸零
            Zteach->InOutArmZHome(iInArm); //kevin 20171101
            iTask=21;
            break;
        case 21:
            if(Zteach->DoZHome()==false) //Z軸歸零
                return false;

            iTask=1;
            if(iInArm==0)//INARM
            {
                if(iHome2 >1)
                {
                    iHome2=0;
                    for(i=0; i<InArmSuck.iMotRow; i++)
                    {
                        for(j=0; j<InArmSuck.iMotCol; j++)
                        {
                            MOT[MInArmZA+k].SetSpeed(ArmSpeed[InArm].iACDCZSP);
                            k++;
                        }
                    }
                    ShowErrorMessage("WAR0131", K_RETRY, MInArmX, false); //kevin 20171103
                    return true;
                }
                iHome2++;
            }
            else
            {
                if(iOutHome2>1)
                {
                    iOutHome2=0;
                    for(i=0; i<InArmSuck.iMotRow; i++)
                    {
                        for(j=0; j<InArmSuck.iMotCol; j++)
                        {
                            MOT[MOutArmZA+k].SetSpeed(ArmSpeed[OutArm].iZSP);
                            k++;
                        }
                    }
                    ShowErrorMessage("WAR0231", K_RETRY, OutArm, false); //kevin 20171103
                    return true;
                }
                iOutHome2++;
            }
            break;
        case 30:
           k=0;
           AnsiString szDir;
           //szDir=szFilePath; //kevin 20150105
           //szDir+="\\Position Offset.Data";
           if(iInArm==0)//INARM
            {
                iStandZ=iZVacuumPos1[4];  // 吸嘴 E
                for(j=0; j<InArmSuck.iMotCol; j++)    // A C E F
                {
                    for(i=0; i<InArmSuck.iMotRow; i++)    // B D F H
                    {
                        iZ[k]=iZVacuumPos1[k]-iStandZ;
                        Tech.iInArmZHeightSub[i][j]=iZ[k];
                        MOT[MInArmZA+k].SetSpeed(ArmSpeed[InArm].iACDCZSP);
                        k++;
                        // WriteIniData(szDir, CapStr[0], str.sprintf("PickUp %c", 'A'+(i*2)+j), "0");
                        // WriteIniData(szDir, CapStr[0], str.sprintf("Place %c", 'A'+(i*2)+j),  "0");
                    }
                }
            }
            else
            {
                iStandZ=iOutZVacuumPos1[4];
                for(j=0; j<InArmSuck.iMotCol; j++)
                {
                    for(i=0; i<InArmSuck.iMotRow; i++)
                    {
                        iZ[k]=iOutZVacuumPos1[k]-iStandZ;
                        Tech.iOutArmZHeightSub[i][j]=iZ[k];
                        MOT[MOutArmZA+k].SetSpeed(ArmSpeed[OutArm].iZSP);
                        k++;
                    }
                }
            }
            fTeach->SaveFile(true);                                             //Steven 20240501 : Teach改存成ini
//            WriteData("d:\\HT9045\\system\\tech.dat", (char *)&Tech.iZLoad, sizeof(TECH));
            bOffsetClean();
            return true;
           //break;
    }
    return false;   //kevin 20140515
}
//------------------------------------------------------------------------------
int iAutoTeachOutArmZTask=1;
void InitOutArmAutoZTask()
{
    iAutoTeachOutArmZTask=1;
}
//------------------------------------------------------------------------------
//KaiChen 20180117 (Steven) : InArm Auto Hight From Loader Tray
//==>
TQPF_Timer DoSupplyNewICTrayDelay_AutoTeachOut;      //KaiChen 20180117 (Steven) : InArm Auto Hight From Loader Tray
bool AutoTeachOutArmZAutoTray(bool Reset, int iOutArm)
{
    int &iTask=iAutoTeachOutArmZTask;
    static int iOutArmZVacuumbufferPos[10]={0};     //Z軸吸取高度
    static int iOutArmZVacuumPos[10]={0};           //Z軸吸取高度
    static int iZMot=0, iMot=0;                     //使用 Z軸馬達

    static int iRowCT=0;
    static bool flag[2][8]={{false, false, false, false, false, false, false, false},{false, false, false, false, false, false, false, false}};
    static int iRow=0, iCol=0;
    static int iAutoTray=0;
    AnsiString asStr;

    if(Reset)
    {
        iAutoTeachOutArmZTask=1;
        for(int i=0; i<8; i++)
        {
            iOutArmZVacuumPos[i]=0;
        }
        return false;
    }

    switch(iTask)
    {
        case 1:  //初始化參數
            iTask=2;
        case 2:
            if(iOutArm==1)
            {
                if(Zteach->OpenSuckOn(OutArmSuck, true))//開真空 確認真空是否正常
                {
                    iTask=1;    //真空產生器有問題無法做AUTO Z
                }
                else
                {
                    iTask=20;
                }
            }
            break;
        case 20:                        //Frank QQ
            if(Sen[SnAutoTrayDetect[iAutoTray]].IsOff()==false)//檢查 Loader 是否有 Tray
            {
                if(Cylinder[C_AutoEdgePush[iAutoTray]].Enable)
                {
                    Cylinder[C_AutoEdgePush[iAutoTray]].On();
                    DoSupplyNewICTrayDelay_AutoTeachOut.SetSecAndOn(0.15);
                    iTask=25;
                }
            }
            else
            {
                asStr=asStr.sprintf("Please Check Auto%d Has Tray !!! ", iAutoTray+1);
                ShowMyMessage(asStr);
            }
            break;
        case 25:
            if(DoSupplyNewICTrayDelay_AutoTeachOut.Off())
            {
                Cylinder[C_AutoEdgePush[iAutoTray]].Off();
                DoSupplyNewICTrayDelay_AutoTeachOut.SetSecAndOn(0.15);
                iTask=30;
            }
            break;
        case 30:
            if(DoSupplyNewICTrayDelay_AutoTeachOut.Off())
            {
                Cylinder[C_AutoEdgePush[iAutoTray]].On();
                DoSupplyNewICTrayDelay_AutoTeachOut.SetSecAndOn(0.2);
                iTask=35;
            }
            break;
        case 35:
            if(DoSupplyNewICTrayDelay_AutoTeachOut.Off())
            {
                if(Cylinder[C_AutoSide_Fixer[iAutoTray]].Push())
                {
                    Cylinder[C_AutoUpPress[iAutoTray]].On();                    //JerryYang 20190423 新增unloader壓tray
                    iTask=40;
                }
            }
            break;
        case 40:
            MOT[iMMAuto[iAutoTray]].InitNewTray(NULL_IC, false, __FUNC__);
            MOT[iMMAuto[iAutoTray]].fHasTray=true;
            iTask=50;
            break;
        case 50:                         //Frank QQ
            iRowCT=0;
            iInArmOrder=0;
            flag[iRowCT][0]=Prod.fInArmSuck4x8[InArmSuck.iWhichSht][iRowCT][0];
            flag[iRowCT][1]=Prod.fInArmSuck4x8[InArmSuck.iWhichSht][iRowCT][1];
            flag[iRowCT][2]=Prod.fInArmSuck4x8[InArmSuck.iWhichSht][iRowCT][2];
            flag[iRowCT][3]=Prod.fInArmSuck4x8[InArmSuck.iWhichSht][iRowCT][3];
            if(flag[iRowCT][0]==false && flag[iRowCT][1]==false && flag[iRowCT][2]==false && flag[iRowCT][3]==false)
            {
                iRowCT=1;
                iInArmOrder=1;
                flag[iRowCT][0]=Prod.fInArmSuck4x8[InArmSuck.iWhichSht][iRowCT][0];
                flag[iRowCT][1]=Prod.fInArmSuck4x8[InArmSuck.iWhichSht][iRowCT][1];
                flag[iRowCT][2]=Prod.fInArmSuck4x8[InArmSuck.iWhichSht][iRowCT][2];
                flag[iRowCT][3]=Prod.fInArmSuck4x8[InArmSuck.iWhichSht][iRowCT][3];
            }

            for(int j=0; j<4; j++)
            {
                if(flag[iRowCT][j])
                {
                    OutArmSuck.iWhichAuto[iRowCT][j]=iAutoTray;
                    OutArmSuck.Item[iRowCT][j]=HAS_IC;
                }
            }

            Find_OutArm_Single(4, iRow, iCol);

            for(int j=0; j<MAX_ARM_Col; j++)
            {
                InArmSuckUse[iRowCT][j]=flag[iRowCT][j];
                if(flag[iRowCT][j]==true)
                {
                    iZMot=iRowCT*1+j*2;
                    iMot =MOutArmZA+iZMot;
                    break;
                }
            }

            iTask=60;
            break;
        case 60:
            if(DoMoveOutArmXY_AutoTeach(iRowCT, iRow, iCol, iAutoTray, false))
            {
                iOutArmZVacuumbufferPos[iZMot]=iInArmTeachZ;
                MOT[iMot].SetSpeed(5);
                iTask=65;
            }
            break;
        case 65:
            if(DoMoveOutArmXY_AutoTeach(iRowCT, iRow, iCol, iAutoTray, true))
            {
                iOutArmZVacuumbufferPos[iZMot]=iInArmTeachZ;
                iTask=70;
            }
            break;

        case 70:
            if(MOT[iMot].MotorMove(iOutArmZVacuumbufferPos[iZMot]))
            {
                iTask=80;
                MOT[iMot].SetSpeed(0.3);
            }
            break;
        case 80:
            iOutArmZVacuumbufferPos[iZMot]-=1;
            if(iOutArmZVacuumbufferPos[iZMot]<-2200) //判斷Z軸是否失步
            {
                MOT[iMot].PCIL132_StopMotor();
                iTask=110;
                return false;
            }
            iTask=85;
            break;
        case 85:
            if(MOT[iMot].MotorMove(iOutArmZVacuumbufferPos[iZMot]))
            {
                if(Zteach->OpenSuckOn(OutArmSuck, true))//開真空 確認真空是否正常
                {
                    MOT[iMot].PCIL132_StopMotor();
                    iOutArmZVacuumPos[iZMot]= MOT[iMot].ReadPos();
                    if(iOutArmZVacuumbufferPos[iZMot]!=iOutArmZVacuumPos[iZMot])
                    {
                        iOutArmZVacuumbufferPos[iZMot]=0;
                    }
                    Zteach->OpenSuckOn(OutArmSuck, false);
                    MOT[iMot].SetSpeed(5);
                    iTask=86;
                    break;
                }
                iTask=80;
            }
            break;
        case 86:            //Frank QQ
            if(MOT[iMot].MotorMove(iOutArmZVacuumPos[iZMot]+100))
            {
                iTask=87;
            }
            break;
        case 87:             //Frank QQ
            for(int X=0; X<2; X++)
            {
                for(int Y=0; Y<4; Y++)
                {
                    if(OutArmSuck.Suck[X][Y].Enable && OutArmSuck.Suck[X][Y].Suck())
                    {
                        OutArmSuck.Suck[X][Y].Off();
                    }
                }
            }
            DoSupplyNewICTrayDelay_AutoTeachOut.SetSecAndOn(1);
            iTask=88;
            break;
        case 88:            //Frank QQ
            if(DoSupplyNewICTrayDelay_AutoTeachOut.Off())
            {
                iTask=90;
            }
            break;
        case 90:
            if(MoveOutArmToAutoSafe()==true)
            {
                for(int X=0; X<2; X++)
                {
                    for(int Y=0; Y<4; Y++)
                    {
                        OutArmSuck.Suck[X][Y].Normal();
                    }
                }
                iTask=150;
            }
            break;
        case 110: //馬達失步必需重來                  //Inarm Z歸零
            Zteach->InOutArmZHome(iOutArm); //kevin 20171101
            iTask=115;
            break;
        case 115:
            if(Zteach->DoZHome()==false) //Z軸歸零
            {
                return false;
            }
            iTask=1;
            break;

        case 150:   //完成
            for(int j=0; j<8; j++)
            {
                iMot=MOutArmZA+j;
                MOT[iMot].SetSpeed(ArmSpeed[OutArm].iACDCZSP);
            }

            for(int i=0; i<2; i++)
            {
                for(int j=0; j<4; j++)
                {
                    if((i+j*2)==iZMot)
                    {
                        if(i==iOutArmYBase && j==iOutArmXBase)
                        {
                            iOutArmAutoZHeight=iOutArmZVacuumPos[iZMot];
                        }
                        else
                        {
                            iOutArmAutoZHeight=iOutArmZVacuumPos[iZMot]+Tech.iOutArmZHeightSub[i][j]+OutArmOffSet[iAutoTray+2]->GetPickUp(i, j);
                        }
                        OutArmOffSet[OutOfsAuto1+iAutoTray]->SetPlace(0);
                    }
                }
            }
            return true;
    }
    return  false;
}
//<==
//KaiChen 20180117 (Steven) : InArm Auto Hight From Loader Tray
//------------------------------------------------------------------------------
//KaiChen 20180117 (Steven) : InArm Auto Hight From Loader Tray
//==>
bool DoMoveOutArmXY_AutoTeach(int iSelRow, int iRow, int iCol, int iAutoTray, bool RealMove)
{
    int iVariablePara[]={2, 1, 0, -1};
    if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)                                //ChungHung 20131231 alter AutoYPitch
    {
        iVariablePara[0]=1;
        iVariablePara[1]=0;
        iVariablePara[2]=-1;
        iVariablePara[3]=-2;
    }
    else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                       //Ztex 2024.02.24 Add HT-1132
    {
        iVariablePara[0]=2;
        iVariablePara[1]=1;
        iVariablePara[2]=0;
        iVariablePara[3]=-1;
    }
    else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                         //QQ
            USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                            //Ztex 2023.12.06 Add HT-1032
    {
    }

    int iXVariable[X_PITCH_COUNT];
    bool bZFlag[MAX_ARM_Row][MAX_ARM_Col];
    int iZPos[MAX_ARM_Row][MAX_ARM_Col];
    int iXPos=0, iYPos=0;
    int iYVariable  =0;
    int iMovePitchX =6000;
    int iMovePitchY =AutoCalculateOutArmYClosePitch(iAutoTray);
    int iOffsetPos  =-1;
    ZeroMemory(iXVariable, sizeof(iXVariable));
    ZeroMemory(bZFlag, sizeof(bZFlag));
    ZeroMemory(iZPos, sizeof(iZPos));

    for(int j=0; j<MAX_ARM_Col; j++)
    {
        if(InArmSuckUse[iSelRow][j]==true)
        {
            iXPos=Prod.XStart[iAutoTray][iSelRow][j];
            iYPos=Prod.YStart[iAutoTray][iSelRow][j];
            iXPos+=iCol*AutoForm[iAutoTray]->XPitch+iMovePitchX*iVariablePara[j]/3;

            if(iXPos<(MOT[MOutArmX].Motor->PSoftLimitN+10) || iXPos>(MOT[MOutArmX].Motor->PSoftLimitP-100))  //Steven 20141217 : For Auto 1使用G & H吸嘴
            {
                iXPos=Prod.XStart[iAutoTray][iSelRow][j];
                iXPos+=iCol*AutoForm[iAutoTray]->XPitch+iMovePitchX*iVariablePara[j]/3;
            }

            iYPos-=iRow*AutoForm[iWhichAuto]->YPitch;
                if(USE_OUT_Y_IS_AUTO_PITCH==true)                                      //JerryYang 20251218 : IN/OUT ARM支援不同模組  //ChungHung 20140304 add for AutoYPitch
            {
                if(iSelRow==0)
                    iYPos=iYPos-iMovePitchY;
            }
            else
            {
                if(iSelRow!=0)
                {
                    iYPos+=Prod.iOutARM_Y_PITCH;
                }
            }
        }
    }

    TransferAutoRatio(iWhichAuto, &iXPos, &iYPos);                              //Steven 20141029 : Auto的軟體齒輪比

    if(IniConfig.bUseTrayBlockMode)                                             //Frank 20160928 add Subtray Function
        OutArmAddBlockPitch(iYPos, iXPos, iRow, iCol);

    if(iXPos<(MOT[MOutArmX].Motor->PSoftLimitN+10) ||                           //jou 2010-02-01 fix 3 吸嘴超過 軟體極限時 , 設成 has null ic
       iXPos>(MOT[MOutArmX].Motor->PSoftLimitP-10))
    {
        ShowErrorMessage("WAR16318", 0, MMSystem, 0, "DoMoveOutArmXY_AutoTeach");   //kevin 20210401 超出極限秀ALARM 避免HANG UP
        return false;
    }

    if(IniConfig.bE34InOutArmPitchZOffsetSameOne==true)
    {
        if(iWhichAuto>=0 && iWhichAuto<=iAutoRight)
            iOffsetPos=OutOfsAuto1;
        else
            iOffsetPos=OutOfsFix1;
    }
    else
    {
        iOffsetPos=OutOfsAuto1+iWhichAuto;
    }

    for(int i=0; i<X_PITCH_COUNT; i++)
        iXVariable[i]=GetOutArmPitchX_9045(iMovePitchX, i, iOffsetPos);
    iYVariable=GetOutArmPitchY_9045(iMovePitchY, iOffsetPos);

    if(OutArmContinuousMove_9045(iXPos, iYPos, iXVariable, iYVariable, bZFlag, iZPos, true))
    {
        return true;
    }
    return false;
}
//<==
//KaiChen 20180117 (Steven) : InArm Auto Hight From Loader Tray

//---------------------------------------------------------------------------

