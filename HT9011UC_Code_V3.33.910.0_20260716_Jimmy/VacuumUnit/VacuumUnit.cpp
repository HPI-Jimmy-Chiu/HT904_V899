#include "MachineDefine.h"
#pragma hdrstop

#include "VacuumUnit.h"
#include "atester.h"
#include "cprod.h"
#include "cmydef.h"
#include "HTEditList.h"
#include "common.h"
#include "main.h"
#include "UsecegemMainFrom.h"
//#include "uHGemClass.h"
#include "uHGemHT9045.h"
#include "MyKitSuck.h"
#include "myQwertyKeyBoard.h"
#include "mymessbox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ALed"
#pragma link "BtnPanelLane"
#pragma link "MyLedLane"
#pragma link "MyLed"
#pragma resource "*.dfm"
TfVacuumUnit *fVacuumUnit;
//---------------------------------------------------------------------------
__fastcall TfVacuumUnit::TfVacuumUnit(TComponent* Owner)
    : TForm(Owner)
{
    iCount=5;
}
//---------------------------------------------------------------------------
void TfVacuumUnit::Initial()
{
    bool bShow=true;//, bNoShow=false;
    bool bEnable=true;//, bDisable=false;
    bool bReadFromFile=true;//, bFixedValue=false;
    AnsiString s="";
    if(MachineTypeChoice==Type_HT9046      ||
       MachineTypeChoice==Type_HT9046_LS   ||
       MachineTypeChoice==Type_HT1032      ||
       USE_46_SUCKER_DB==1)                                                     //HT9046LS sucker_9046.db
    {
        iIndexColMax=TOTAL_VACUUM_UNIT;
    }
    else  //HT9045 sucker.db
    {
        iIndexColMax=TOTAL_VACUUM_UNIT/2;
    }
    iInOutColMax=TOTAL_VACUUM_UNIT/2;

    //initial IndexArm2/IndexArm1 Vacuum unit
    grpIndexArm2->Top       =-3;                                                //IndexArm2
    grpIndexArm2->Left      =0;
    grpIndexArm2->Width     =(VACUUM_UNIT_WIDTH+2)*iIndexColMax;
    grpIndexArm2->Height    =(VACUUM_UNIT_HEIGHT+12)*2;

    grpIndexArm1->Top       =grpIndexArm2->Height;                              //IndexArm1
    grpIndexArm1->Left      =0;
    grpIndexArm1->Width     =(VACUUM_UNIT_WIDTH+2)*iIndexColMax;
    grpIndexArm1->Height    =(VACUUM_UNIT_HEIGHT+12)*2;

    if(iIndexColMax==TOTAL_VACUUM_UNIT)                                         //HT9046LS sucker_9046.db
    {
        for(int iCol=0; iCol<iIndexColMax; iCol++)
        {
            for(int iRow=0; iRow<2; iRow++)
            {
                myPalArm1[iCol][iRow]                     = new TMyVacuumPanel(this,0,iCol,iRow);      //IndexArm1
                myPalArm1[iCol][iRow]->GroupBox->Parent   = scrlbxIndexArm1;

                myPalArm2[iCol][iRow]                     = new TMyVacuumPanel(this,1,iCol,iRow);      //IndexArm2
                myPalArm2[iCol][iRow]->GroupBox->Parent   = scrlbxIndexArm2;
            }
        }
    }
    else    //HT9045 sucker.db
    {
        for(int iCol=0; iCol<iIndexColMax; iCol++)
        {
            for(int iRow=0; iRow<2; iRow++)
            {
                myPalArm1[iCol][iRow]                     = new TMyVacuumPanel(this,4,iCol,iRow);      //IndexArm1
                myPalArm1[iCol][iRow]->GroupBox->Parent   = scrlbxIndexArm1;

                myPalArm2[iCol][iRow]                     = new TMyVacuumPanel(this,5,iCol,iRow);      //IndexArm2
                myPalArm2[iCol][iRow]->GroupBox->Parent   = scrlbxIndexArm2;
            }
        }
    }
    //initial InArm/OutArm Vacuum unit
    grpInarm->Top       =-3;                                                     //InArm
    grpInarm->Left      =0;
    grpInarm->Width     =(VACUUM_UNIT_WIDTH+2)*iInOutColMax;
    grpInarm->Height    =(VACUUM_UNIT_HEIGHT+12)*2;

    grpOutarm->Top      =-3;                                                     //OutArm
    grpOutarm->Left     =0;
    grpOutarm->Width    =(VACUUM_UNIT_WIDTH+2)*iInOutColMax;
    grpOutarm->Height   =(VACUUM_UNIT_HEIGHT+12)*2;
    for(int iCol=0; iCol<iInOutColMax; iCol++)
    {
        for(int iRow=0; iRow<2; iRow++)
        {
            myPalInArm[iCol][iRow]                    = new TMyVacuumPanel(this,2,iCol,iRow);     //InArm
            myPalInArm[iCol][iRow]->GroupBox->Parent  = scrlbxInArm;

            myPalOutArm[iCol][iRow]                   = new TMyVacuumPanel(this,3,iCol,iRow);     //OutArm
            myPalOutArm[iCol][iRow]->GroupBox->Parent = scrlbxOutArm;
        }
    }

    //VacuumUnit
    elVacuumUnit->Add(CheckBox1,        &bTest1,          ECBool,           "Tray",    "CheckBox1",      bShow, bEnable, bReadFromFile, 0);
    elVacuumUnit->Add(Edit1,            &iTest1,          ECInteger,        "Tray",    "Edit1",          bShow, bEnable, bReadFromFile, iTest1,          true,  0,   300);

    for(int iCol=0; iCol<iIndexColMax; iCol++)
    {
        for(int iRow=0; iRow<2; iRow++)
        {
            s.sprintf("IndexArm1_%d_%d", iRow, iCol);
            elVacuumUnit->Add(myPalArm1[iCol][iRow]->edSV,  &TestIF_File.iVaccumThrdIndexArm1[iRow][iCol], ECInteger,   "Vacuum Threshold",  s,    bShow, bEnable, bReadFromFile, TestIF_File.iVaccumThrdIndexArm1[iRow][iCol],    true,  -116.0,   148);
            s.sprintf("IndexArm2_%d_%d", iRow, iCol);
            elVacuumUnit->Add(myPalArm2[iCol][iRow]->edSV,  &TestIF_File.iVaccumThrdIndexArm2[iRow][iCol], ECInteger,   "Vacuum Threshold",  s,    bShow, bEnable, bReadFromFile, TestIF_File.iVaccumThrdIndexArm2[iRow][iCol],    true,  -116.0,   148);
        }
    }
    for(int iCol=0; iCol<iInOutColMax; iCol++)
    {
        for(int iRow=0; iRow<2; iRow++)
        {
            s.sprintf("InArm_%d_%d", iRow, iCol);
            elVacuumUnit->Add(myPalInArm[iCol][iRow]->edSV,  &TestIF_File.iVaccumThrdInArm[iRow][iCol], ECInteger,   "Vacuum Threshold",  s, bShow, bEnable, bReadFromFile, TestIF_File.iVaccumThrdInArm[iRow][iCol],    true,  -116.0,   148);
            s.sprintf("OutArm_%d_%d",iRow,iCol);
            elVacuumUnit->Add(myPalOutArm[iCol][iRow]->edSV, &TestIF_File.iVaccumThrdOutArm[iRow][iCol], ECInteger,  "Vacuum Threshold",  s, bShow, bEnable, bReadFromFile, TestIF_File.iVaccumThrdOutArm[iRow][iCol],    true,  -116.0,   148);
        }
    }
}
//---------------------------------------------------------------------------
void TfVacuumUnit::SetPanelPos(bool bInit)
{
    //Arm2/Arm1
    for(int iCol=0; iCol<iIndexColMax; iCol++)
    {
        for(int iRow=0; iRow<2; iRow++)
        {
            myPalArm2[iCol][iRow]->GroupBox->Left         =VACUUM_UNIT_WIDTH*iCol;
            myPalArm2[iCol][iRow]->GroupBox->Top          =VACUUM_UNIT_HEIGHT*iRow;

            myPalArm1[iCol][iRow]->GroupBox->Left         =VACUUM_UNIT_WIDTH*iCol;
            myPalArm1[iCol][iRow]->GroupBox->Top          =VACUUM_UNIT_HEIGHT*iRow;
        }
    }
    //In/OutArm
    for(int iCol=0; iCol<iInOutColMax; iCol++)
    {
        for(int iRow=0; iRow<2; iRow++)
        {
            myPalInArm[iCol][iRow]->GroupBox->Visible     =bInit;
            myPalInArm[iCol][iRow]->GroupBox->Left        =VACUUM_UNIT_WIDTH*iCol;
            myPalInArm[iCol][iRow]->GroupBox->Top         =VACUUM_UNIT_HEIGHT*iRow;

            myPalOutArm[iCol][iRow]->GroupBox->Visible    =bInit;
            myPalOutArm[iCol][iRow]->GroupBox->Left       =VACUUM_UNIT_WIDTH*iCol;
            myPalOutArm[iCol][iRow]->GroupBox->Top        =VACUUM_UNIT_HEIGHT*iRow;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfVacuumUnit::ShowSuckMode(int iSel)
{
    for(int iRow=0; iRow<FTestSuck.iMaxRow; iRow++)
    {
        for(int iCol=0; iCol<FTestSuck.iMaxCol; iCol++)
        {
            if(iRow<FTestSuck.iShtRow &&
               iCol<FTestSuck.iShtCol)
            {
                myPalArm2[iCol][iRow]->GroupBox->Visible=true;
                myPalArm1[iCol][iRow]->GroupBox->Visible=true;
            }
            else
            {
                myPalArm2[iCol][iRow]->GroupBox->Visible=false;
                myPalArm1[iCol][iRow]->GroupBox->Visible=false;
            }
        }
    }

    switch(iSel)
    {
        case SingleSite:        //1x1 BusyShuttle
        case DualSite:          //1x2
        case QualSite2X2N:      //Frank 20200520 2X2NN Mode
        case TriSite1X3:        //Frank 20160329 add for 1x3_4
        case _6Site2X3N:        //Steven 20220425 : 2X3NN Mode
        case QualSite1X4:       //1x4
        case _8Site1X4:         //ChungHung 20150528 add for 海思 _8Site1x4
        case DualSite2x1:       //2x1
        case QualSite2X2:       //2x2
        case _6Site2X3:         //ChungHung 20140115 add for 2x3_6
        case _16Site4X4:        //Sam 20190226 : 16Site4X4
        case _8Site2X4:         //2x4
        case _16Site2X8:        //2x8  //Steven 20110520 : 換位置
        case _10Site2X5:        //wei 20190614 10 site
        case _12Site2X6:
        case _32Site4X8N:       //2x8
            break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfVacuumUnit::FormShow(TObject *Sender)
{
    AnsiString S="";
    LastFileName=GetLastOpenFN();
    ReadFile();
    S.sprintf("Vacuum Unit Condition  ''%s''  ",LastFileName);
    Caption=S;
    this->Top=0;
    this->Left=110;
    this->Width=690;
    this->Height=1020;
    fShow=true;
    tmr1->Enabled=true;
    SetPanelPos(fShow);     //設定畫面
    iCount=2;
    ShowSuckMode(TestIF.iTestMode);

    //IndexArm12
    for(int iCol=0; iCol<iIndexColMax; iCol++)
    {
        for(int iRow=0; iRow<2; iRow++)
        {
            myPalArm1[iCol][iRow]->RefreshDOIO();

            myPalArm2[iCol][iRow]->RefreshDOIO();
        }
    }

    //In/OutArm
    for(int iCol=0; iCol<iInOutColMax; iCol++)
    {
        for(int iRow=0; iRow<2; iRow++)
        {
            myPalInArm[iCol][iRow]->RefreshDOIO();

            myPalOutArm[iCol][iRow]->RefreshDOIO();
        }
    }
   /*
   edSetInArm->Text     =(int)myPalInArm[0][0]->ReadVaccumThreshold();      //以第一個位置閥值來當初始值。
   edSetIndexArm->Text  =(int)myPalArm1[0][0]->ReadVaccumThreshold();
   edSetOutArm->Text    =(int)myPalOutArm[0][0]->ReadVaccumThreshold();
   */
}
//---------------------------------------------------------------------------
void __fastcall TfVacuumUnit::tmr1Timer(TObject *Sender)
{
    if(InitialOK==false || fShow==false)
        return;
    if(iCount<=0)
    {
        //Arm2/Arm1
        for(int iCol=0; iCol<iIndexColMax; iCol++)
        {
            for(int iRow=0; iRow<2; iRow++)
            {
                myPalArm1[iCol][iRow]->RefreshCurrectVal();
                myPalArm1[iCol][iRow]->RefreshThresholdVal();
                myPalArm1[iCol][iRow]->RefreshVaccumIO();
                myPalArm1[iCol][iRow]->RefreshEvent();
                myPalArm1[iCol][iRow]->InitialThresholdMode();

                myPalArm2[iCol][iRow]->RefreshCurrectVal();
                myPalArm2[iCol][iRow]->RefreshThresholdVal();
                myPalArm2[iCol][iRow]->RefreshVaccumIO();
                myPalArm2[iCol][iRow]->RefreshEvent();
                myPalArm2[iCol][iRow]->InitialThresholdMode();
            }
        }
        //In/OutArm
        for(int iCol=0; iCol<iInOutColMax; iCol++)
        {
            for(int iRow=0; iRow<2; iRow++)
            {
                myPalInArm[iCol][iRow]->RefreshCurrectVal();
                myPalInArm[iCol][iRow]->RefreshThresholdVal();
                myPalInArm[iCol][iRow]->RefreshVaccumIO();
                myPalInArm[iCol][iRow]->InitialThresholdMode();
                myPalInArm[iCol][iRow]->RefreshEvent();

                myPalOutArm[iCol][iRow]->RefreshCurrectVal();
                myPalOutArm[iCol][iRow]->RefreshThresholdVal();
                myPalOutArm[iCol][iRow]->RefreshVaccumIO();
                myPalOutArm[iCol][iRow]->InitialThresholdMode();
                myPalOutArm[iCol][iRow]->RefreshEvent();
            }
        }
    }
    else
    {
        iCount--;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfVacuumUnit::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    ReadFile();
    tmr1->Enabled=false;
    fShow=false;
    DoIniDataToForm();                                                          //JerryYang 20250411 : 離開頁面要刷新一次, 避免誤存檔
}
//---------------------------------------------------------------------------
void __fastcall TfVacuumUnit::FormDestroy(TObject *Sender)
{
    try
    {
        for(int iCol=0; iCol<iIndexColMax; iCol++)
        {
            for(int iRow=0; iRow<2; iRow++)
            {
                if(myPalArm2[iCol][iRow]!=NULL)
                    delete myPalArm2[iCol][iRow];
                myPalArm2[iCol][iRow]=NULL;

                if(myPalArm1[iCol][iRow]!=NULL)
                    delete myPalArm1[iCol][iRow];
                myPalArm1[iCol][iRow]=NULL;
            }
        }

        for(int iCol=0; iCol<iInOutColMax; iCol++)
        {
            for(int iRow=0; iRow<2; iRow++)
            {
                if(myPalInArm[iCol][iRow]!=NULL)
                    delete myPalInArm[iCol][iRow];
                myPalInArm[iCol][iRow]=NULL;
                if(myPalOutArm[iCol][iRow]!=NULL)
                    delete myPalOutArm[iCol][iRow];
                myPalOutArm[iCol][iRow]=NULL;
            }
        }
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfVacuumUnit::FormDestroy");
    }
    //LogSoftwareOffTime("TfVacuumUnit, FormDestroy"); //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
void __fastcall TfVacuumUnit::ReadFile()
{
    AnsiString szDir=GetRecipePath();
    LastFileName=GetLastOpenFN();

    if(elVacuumUnit!=NULL)
    {
        elVacuumUnit->ReadEditTextFromFile(szDir, "HandlerCondition.Data");
    }
    DoIniDataToForm();
}
//---------------------------------------------------------------------------
void __fastcall TfVacuumUnit::DoIniDataToForm()
{
    if(elVacuumUnit!=NULL)
        elVacuumUnit->InitialDataToEdit();
}
//---------------------------------------------------------------------------
void __fastcall TfVacuumUnit::SaveSetupFile(AnsiString szDir, AnsiString S)
{
    MyForceDirectories(szDir);
    if(elVacuumUnit!=NULL)
        elVacuumUnit->SaveEditTextToFile(szDir, "HandlerCondition.Data");

    fMain->BackupSetupFile();                                                   //Ifor 20170620 (wei) add Auto BackUp Setup File & Last Data
}
//---------------------------------------------------------------------------
void __fastcall TfVacuumUnit::spbSaveClick(TObject *Sender)
{
    if(IniConfig.bA02DisableSaveParsWhenSwitchToOp==true &&                     //RogerYang 20260305 張寧說時間到都要登出，且不可被更改任何設定(A01_2)
        AccessLevel==0)
    {
        ShowMyMessage("[A01_2]目前已切換到Operator權限，\r\n請重新登入再做設定!");
        Close();
    }

    AnsiString szDir="";
    szDir.sprintf("%s%s\\", DataPath, LastFileName);
    SaveSetupFile(szDir, LastFileName);
    #ifdef ASE_KaohSiung
       fBuilder->bSaveAsJobFile(LastFileName, "JOBFILE");
    #endif
    ReadFile();
    spbSave->Down=false;
    if(IniConfig.bEnable_SECS_GEM==true)
    {
        bHasSaveSet=true;                                                       //Ifor 20151204 新增判斷機台有無修改設定檔
        EventReport(SECS_EVENT.SaveRecipe);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfVacuumUnit::sbtExitClick(TObject *Sender)
{
    sbtExit->Down=false;
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfVacuumUnit::sbResetClick(TObject *Sender)
{
    //Arm2/Arm1
    for(int iCol=0; iCol<iIndexColMax; iCol++)
    {
        for(int iRow=0; iRow<2; iRow++)
        {
            myPalArm1[iCol][iRow]->Reset();
            myPalArm2[iCol][iRow]->Reset();
        }
    }

    //In/OutArm
    for(int iCol=0; iCol<iInOutColMax; iCol++)
    {
        for(int iRow=0; iRow<2; iRow++)
        {
            myPalInArm[iCol][iRow]->Reset();
            myPalOutArm[iCol][iRow]->Reset();
        }
    }
}
//---------------------------------------------------------------------------
void SetIOTableByECAT_VC8_Sucker()
{
    if(VCCU_UNIT_TYPE==1)
    {
        //由軟體自動編排 2
        /*
        for(int iRow=0; iRow<2; iRow++)
        {
            for(int iCol=0; iCol<fVacuumUnit->iIndexColMax; iCol++)
            {
                VaccumCopyToSuck(*fVacuumUnit->myPalArm1[iCol][iRow],FTestSuck.Suck[iRow][iCol]);
                VaccumCopyToSuck(*fVacuumUnit->myPalArm2[iCol][iRow],BTestSuck.Suck[iRow][iCol]);
            }
        }

        for(int iRow=0; iRow<2; iRow++)
        {
            for(int iCol=0; iCol<fVacuumUnit->iInOutColMax; iCol++)
            {
                VaccumCopyToSuck(*fVacuumUnit->myPalInArm[iCol][iRow],InArmSuck.Suck[iRow][iCol]);
                VaccumCopyToSuck(*fVacuumUnit->myPalOutArm[iCol][iRow],OutArmSuck.Suck[iRow][iCol]);
            }
        }
        */

        //讀 DB 檔 2
        for(int iRow=0; iRow<2; iRow++)
        {
            for(int iCol=0; iCol<fVacuumUnit->iIndexColMax; iCol++)
            {
                SetSuckISABase(FTestSuck.Suck[iRow][iCol],ePCI1203);
                VaccumCopyFormSuck(*fVacuumUnit->myPalArm1[iCol][iRow],FTestSuck.Suck[iRow][iCol]);

                SetSuckISABase(BTestSuck.Suck[iRow][iCol],ePCI1203);
                VaccumCopyFormSuck(*fVacuumUnit->myPalArm2[iCol][iRow],BTestSuck.Suck[iRow][iCol]);
            }
        }
        for(int iRow=0; iRow<2; iRow++)
        {
            for(int iCol=0; iCol<fVacuumUnit->iInOutColMax; iCol++)
            {
                SetSuckISABase(InArmSuck.Suck[iRow][iCol],ePCI1203);
                VaccumCopyFormSuck(*fVacuumUnit->myPalInArm[iCol][iRow],InArmSuck.Suck[iRow][iCol]);

                SetSuckISABase(OutArmSuck.Suck[iRow][iCol],ePCI1203);
                VaccumCopyFormSuck(*fVacuumUnit->myPalOutArm[iCol][iRow],OutArmSuck.Suck[iRow][iCol]);
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall SetSuckISABase(class TMySucker &Source, int iISABase)
{
    Source.OnISABase    =iISABase;
    Source.OffISABase   =iISABase;
    Source.SenISABase   =iISABase;
    Source.ISABase      =iISABase;
}
//---------------------------------------------------------------------------
void __fastcall VaccumCopyFormSuck(class TMyVacuumPanel &Target, class TMySucker &Source)
{
    //將 VaccumUnit 讀取 Suck Database
    AnsiString str="";
    Target.SuckerName=Source.SuckerName;
    Target.OnRing   =Source.OnRing;
    Target.OnIP     =Source.OnIP;
    Target.OnPort   =Source.OnPort;
    Target.OnVCNo   =Source.OnBit;
    Target.OnType   =Source.OnType;
    Target.OnISABase=Source.OnISABase;

    str.sprintf("(Lane:%d,IP:%d,Port:%d,Bit:%d) %s", Target.OnRing, Target.OnIP, Target.OnPort, Target.OnVCNo, Target.SuckerName);
    Target.bplOn->Hint=str;

    Target.OffRing  =Source.OffRing;
    Target.OffIP    =Source.OffIP;
    Target.OffPort  =Source.OffPort;
    Target.OffVCNo  =Source.OffBit;
    Target.OffType  =Source.OffType;
    Target.OffISABase=Source.OffISABase;
    str.sprintf("(Lane:%d,IP:%d,Port:%d,Bit:%d) %s", Target.OffRing, Target.OffIP, Target.OffPort, Target.OffVCNo, Target.SuckerName);
    Target.bplOff->Hint=str;

    Target.SenRing  =Source.SenRing;
    Target.SenIP    =Source.SenIP;
    Target.SenPort  =Source.SenPort;
    Target.SenVCNo  =Source.SenBit;
    Target.SenType  =Source.SenType;
    Target.SenISABase=Source.SenISABase;
    str.sprintf("(Lane:%d,IP:%d,Port:%d,Bit:%d) %s VCNo%d", Target.SenRing,Target.SenIP, Target.SenPort, Target.SenVCNo, Target.SuckerName, Target.SenVCNo);
    Target.myld1->Hint=str;
}
//---------------------------------------------------------------------------
void __fastcall VaccumCopyToSuck(class TMyVacuumPanel &Source, class TMySucker &Target)
{
    //將 VaccumUnit 寫到 Suck Database
    Target.OnRing   =Source.OnRing;
    Target.OnIP     =Source.OnIP;
    Target.OnPort   =Source.OnPort;
    Target.OnBit    =Source.OnVCNo;
    Target.OnType   =Source.OnType;
    Target.OnISABase=Source.OnISABase;

    Target.OnRing   =Source.OffRing;
    Target.OffIP    =Source.OffIP;
    Target.OffPort  =Source.OffPort;
    Target.OffBit   =Source.OffVCNo;
    Target.OffType  =Source.OffType;
    Target.OffISABase=Source.OffISABase;

    Target.SenRing  =Source.SenRing;
    Target.SenIP    =Source.SenIP;
    Target.SenPort  =Source.SenPort;
    Target.SenBit   =Source.SenVCNo;
    Target.SenType  =Source.SenType;
    Target.SenISABase=Source.SenISABase;
    Target.ISABase  =ePCI1203;
}
//---------------------------------------------------------------------------
void __fastcall TfVacuumUnit::btnSetInArmClick(TObject *Sender)
{
    TButton *Ptr;
    double dValue=0.0;

    Ptr=(TButton *)Sender;
    if(Ptr->Tag==0)     //InArm
    {
        dValue=atof(edSetInArm->Text.c_str());
        for(int iRow=0; iRow<2; iRow++)
        {
            for(int iCol=0; iCol<fVacuumUnit->iInOutColMax; iCol++)
            {
                fVacuumUnit->myPalInArm[iCol][iRow]->edSV->Text=(int)dValue;
                fVacuumUnit->myPalInArm[iCol][iRow]->WriteVaccumThreshold(dValue);
            }
        }
    }
    else if(Ptr->Tag==1)    //Index Arm1/2
    {
        dValue=atof(edSetIndexArm->Text.c_str());
        for(int iRow=0; iRow<2; iRow++)
        {
            for(int iCol=0; iCol<fVacuumUnit->iIndexColMax; iCol++)
            {
                fVacuumUnit->myPalArm1[iCol][iRow]->edSV->Text=(int)dValue;
                fVacuumUnit->myPalArm2[iCol][iRow]->edSV->Text=(int)dValue;
                fVacuumUnit->myPalArm1[iCol][iRow]->WriteVaccumThreshold(dValue);
                fVacuumUnit->myPalArm2[iCol][iRow]->WriteVaccumThreshold(dValue);
            }
        }
    }

    if(Ptr->Tag==2)     //OutArm
    {
        dValue=atof(edSetOutArm->Text.c_str());
        for(int iRow=0; iRow<2; iRow++)
        {
            for(int iCol=0; iCol<fVacuumUnit->iInOutColMax; iCol++)
            {
                fVacuumUnit->myPalOutArm[iCol][iRow]->edSV->Text=(int)dValue;
                fVacuumUnit->myPalOutArm[iCol][iRow]->WriteVaccumThreshold(dValue);
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfVacuumUnit::edSetInArmClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_DOUBLE, 0, true, -116.0, 148.0); //小鍵盤
}
//---------------------------------------------------------------------------

