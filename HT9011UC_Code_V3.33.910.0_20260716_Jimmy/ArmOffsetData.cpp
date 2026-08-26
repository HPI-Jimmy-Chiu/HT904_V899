#include "MachineDefine.h"
#pragma hdrstop
#include "myQwertyKeyBoard.h"
#include "ArmOffsetData.h"
#include "cprod.h"
#include "INPUT.h"
#include "common.h"
#include "cOffSet.h"
#include "cMyDef.h"
#include "mymessbox.h"
#include "MyKitSuck.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfArmOffsetData *fArmOffsetData;
AnsiString AlxeName[OneByOneOfsTotal]=
{
    "Loader",
    "Hot Plate1",
    "Hot Plate2",
    "Input Shuttle1",
    "Input Shuttle2",

    "Output Shuttle1",
    "Output Shuttle2",
    "Auto1",
    "Auto2",
    "Auto3",
    "Auto4",
    "Auto5",
    "Auto6",
    "Fix1",
    "Fix2",
    "Fix3",
    "Fix4",
    "Fix5",
    "Fix6"
};
//---------------------------------------------------------------------------
__fastcall TfArmOffsetData::TfArmOffsetData(TComponent* Owner)
    : TForm(Owner)
{
    SGUse = new TEdit(this);
    AnsiString Str;

    for(int i=0; i<OneByOneOfsTotal; i++)
    {
        for(int j=0; j<eOfsEditTotal; j++)
        {
            MyEdit[i][j] = new TEdit(this);
            Str.sprintf("edt%02d%d", i, j);
            MyEdit[i][j]->Name=Str;
            if(i<=eOboInSht2)
            {
                MyEdit[i][j]->Parent = pnlInArm;
                MyEdit[i][j]->Left   = 66+i*123;
                MyEdit[i][j]->Top    = 30+j*25;
                MyEdit[i][j]->Width  = 60;
            }
            else
            {
                MyEdit[i][j]->Parent = pnlOutArm;
                MyEdit[i][j]->Left   = 66+(i-eOboOutSht1)*78;
                MyEdit[i][j]->Top    = 30+j*25;
                MyEdit[i][j]->Width  = 60;
            }

            if(j==2 || j==3 || j==7 || j==8) MyEdit[i][j]->OnClick=EditOffsetZClick;
            else                             MyEdit[i][j]->OnClick=EditOffsetXYClick;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfArmOffsetData::EditOffsetXYClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_DOUBLE, 0, true, InputLimit.iOffsetXYHigh, InputLimit.iOffsetXYLow);
}
//---------------------------------------------------------------------------
void __fastcall TfArmOffsetData::EditOffsetZClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_DOUBLE, 0, true, InputLimit.iOffsetZHigh, InputLimit.iOffsetZLow);
}
//---------------------------------------------------------------------------
void __fastcall TfArmOffsetData::FormShow(TObject *Sender)
{
    fShow=true;
    LastFileName=GetLastOpenFN();
    for(int i=0; i<OneByOneOfsTotal; i++)
    {
        if(i<=eOboInSht2)
        {
            MyEdit[i][eOfsEditX      ]->Text = InArmOffSet_File[i]->GetX();
            MyEdit[i][eOfsEditY      ]->Text = InArmOffSet_File[i]->GetY();
            MyEdit[i][eOfsEditPlace  ]->Text = InArmOffSet_File[i]->GetPlace();
            MyEdit[i][eOfsEditPick   ]->Text = InArmOffSet_File[i]->GetPickUp();
            MyEdit[i][eOfsEditPitchX1]->Text = InArmOffSet_File[i]->GetVariable();
            MyEdit[i][eOfsEditPitchX2]->Text = InArmOffSet_File[i]->GetVariable2();
            MyEdit[i][eOfsEditPitchX3]->Text = InArmOffSet_File[i]->GetVariable3();
            MyEdit[i][eOfsEditPitchX4]->Text = InArmOffSet_File[i]->GetVariable4();
            MyEdit[i][eOfsEditPitchY ]->Text = InArmOffSet_File[i]->GetVariableY();
        }
        else
        {
            MyEdit[i][eOfsEditX      ]->Text = OutArmOffSet_File[i-eOboAuto1]->GetX();
            MyEdit[i][eOfsEditY      ]->Text = OutArmOffSet_File[i-eOboAuto1]->GetY();
            MyEdit[i][eOfsEditPlace  ]->Text = OutArmOffSet_File[i-eOboAuto1]->GetPlace();
            MyEdit[i][eOfsEditPick   ]->Text = OutArmOffSet_File[i-eOboAuto1]->GetPickUp();
            MyEdit[i][eOfsEditPitchX1]->Text = OutArmOffSet_File[i-eOboAuto1]->GetVariable();
            MyEdit[i][eOfsEditPitchX2]->Text = OutArmOffSet_File[i-eOboAuto1]->GetVariable2();
            MyEdit[i][eOfsEditPitchX3]->Text = OutArmOffSet_File[i-eOboAuto1]->GetVariable3();
            MyEdit[i][eOfsEditPitchX4]->Text = OutArmOffSet_File[i-eOboAuto1]->GetVariable4();
            MyEdit[i][eOfsEditPitchY ]->Text = OutArmOffSet_File[i-eOboAuto1]->GetVariableY();
        }
    }

    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);    //Steven 20100629
}
//---------------------------------------------------------------------------
void __fastcall TfArmOffsetData::btnOutArmExitClick(TObject *Sender)
{
    Close();
    fOffSet->Show();
    fOffSet->ReadFile();
    fOffSet->DoIniDataToForm(fOffSet->iNowOffsetSel,1);
}
//---------------------------------------------------------------------------
void __fastcall TfArmOffsetData::FormDestroy(TObject *Sender)
{
    try
    {
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfArmOffsetData::FormDestroy");
    }
    LogSoftwareOffTime("TfArmOffsetData, FormDestroy"); //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
void __fastcall TfArmOffsetData::btnSaveClick(TObject *Sender)
{
    if(pgcArmOffset->ActivePageIndex==0)
        SaveOffSet(true);
    else
        SaveOffSet(false);
}
//---------------------------------------------------------------------------
void __fastcall TfArmOffsetData::SaveOffSet(bool ArmType)
{
    AnsiString szDir="";
    AnsiString str, strX, strY;

    szDir=fOffSet->GetOffsetPath(); //Steven 20190109 : 整合Offset路徑
    MyForceDirectories(szDir);
    szDir+="\\Position OffSet.Data";

    if(FileExists(szDir))
    {
        for(int i=0; i<OneByOneOfsTotal; i++)
        {
            if((ArmType==true  && i<=eOboInSht2) ||
               (ArmType==false && i>=eOboOutSht1))
            {
                WriteIniData(szDir, AlxeName[i], "Hand X",    MyEdit[i][eOfsEditX       ]->Text);
                WriteIniData(szDir, AlxeName[i], "Hand Y",    MyEdit[i][eOfsEditY       ]->Text);
                WriteIniData(szDir, AlxeName[i], "PickUp",    MyEdit[i][eOfsEditPick    ]->Text);
                WriteIniData(szDir, AlxeName[i], "Place",     MyEdit[i][eOfsEditPlace   ]->Text);
                WriteIniData(szDir, AlxeName[i], "Variable",  MyEdit[i][eOfsEditPitchX1 ]->Text);
                WriteIniData(szDir, AlxeName[i], "Variable2", MyEdit[i][eOfsEditPitchX2 ]->Text);//Steven 20131002 : XY變距
                WriteIniData(szDir, AlxeName[i], "Variable3", MyEdit[i][eOfsEditPitchX3 ]->Text);
                WriteIniData(szDir, AlxeName[i], "Variable4", MyEdit[i][eOfsEditPitchX4 ]->Text);
                WriteIniData(szDir, AlxeName[i], "VariableY", MyEdit[i][eOfsEditPitchY  ]->Text);
            }
        }

        szDir=fOffSet->GetOffsetPath();                                         //Steven 20190109 : 整合Offset路徑
        if(LastSet.iTemperature==Tempture_Hot)
            szDir+="\\Position Offset Hot.Data";
        else
            szDir+="\\Position Offset.Data";

        for(int i=0; i<OneByOneOfsTotal; i++)
        {
            if(ArmType && (i==eOboInSht1 || i==eOboInSht2))
            {
                WriteIniData(szDir, AlxeName[i], "Hand X",    MyEdit[i][0]->Text);
                WriteIniData(szDir, AlxeName[i], "Hand Y",    MyEdit[i][1]->Text);
            }
            else if(ArmType==false && (i==eOboOutSht1 || i==eOboOutSht2))
            {
                WriteIniData(szDir, AlxeName[i], "Hand X",    MyEdit[i][0]->Text);
                WriteIniData(szDir, AlxeName[i], "Hand Y",    MyEdit[i][1]->Text);
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfArmOffsetData::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);    //Steven 20100629
}
//---------------------------------------------------------------------------
