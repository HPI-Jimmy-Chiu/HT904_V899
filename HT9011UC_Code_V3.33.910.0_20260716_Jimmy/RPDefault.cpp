#include "MachineDefine.h"
#pragma hdrstop

#include "RPDefault.h"
#include "cSpeed.h"
#include "common.h"
#include "uRecipeDefault.h"
#include "uCleaning.h"
#include "cmydef.h"
#include "cprod.h"
#include "main.h"
#include "Password.h"
#include "cSecurity.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfRPDefault *fRPDefault;
//---------------------------------------------------------------------------
__fastcall TfRPDefault::TfRPDefault(TComponent* Owner)
    : TForm(Owner)
{
    RP_Name     = new TStringList;
    RP_Default  = new TStringList;
    RP_Value    = new TStringList;
}
//---------------------------------------------------------------------------
void __fastcall TfRPDefault::FormDestroy(TObject *Sender)
{
    try
    {
        delete RP_Name;
        delete RP_Default;
        delete RP_Value;
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfRPDefault::FormDestroy");
    }
    LogSoftwareOffTime("TfRPDefault, FormDestroy"); //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
void __fastcall TfRPDefault::spbDefaulteViewShowClick(TObject *Sender)
{
    fRecipeDefault->Show();
}
//---------------------------------------------------------------------------
void __fastcall TfRPDefault::spbExitClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfRPDefault::spbSetRPDefaultClick(TObject *Sender)
{
    if(fSecurity->Insufficient(168, false)==false)   //Sam 20210414 : Recipe Parameter Default 功能新增權限控制
        return;

    fSpeed->Show();
    fSpeed->DoSetRPDefault(fSpeed->tsAllSpeed, -1); //Isaac 20170718 (wei) defalut值比較功能:add trackbar default
    fSpeed->DoSetRPDefault(fSpeed->tsIndex,     0);
    fSpeed->DoSetRPDefault(fSpeed->tsInArm,     1);
    fSpeed->DoSetRPDefault(fSpeed->tsOutArm,    2);
    fSpeed->DoSetRPDefault(fSpeed->tsTrayArm,   3);
    fSpeed->DoSetRPDefault(fSpeed->tsShuttle,   4);
    fSpeed->Close();

    fCleaning->Show();
    fCleaning->DoSetRPDefault(fCleaning, 0);
    fCleaning->Close();

    FTestIF->Show();
    FTestIF->DoSetRPDefault(FTestIF, 0);
    FTestIF->Close();

    fYieldMonitoring->Show();
    fYieldMonitoring->DoSetRPDefault(fYieldMonitoring, 0);
    fYieldMonitoring->Close();
}
//---------------------------------------------------------------------------
void __fastcall TfRPDefault::spbReplyRPDefaultClick(TObject *Sender)
{
    AnsiString szDir="D:\\HT9045\\IniData\\RPDefault.ini";  //Isaac 20170718 (wei) defalut值比較功能:add trackbar default

    if(fSecurity->Insufficient(169, false)==false)   //Sam 20210414 : Recipe Parameter Default 功能新增權限控制
        return;

    fSpeed->Show();

    fSpeed->tbAllSpeed->Position      =ReadIniData(szDir, "All", "Speed", 1);//Isaac 20170718 (wei) defalut值比較功能:add trackbar default
    fSpeed->tbAccSpeed->Position      =ReadIniData(szDir, "All", "Accel", 1);
    fSpeed->edAllSpeed->Text          =fSpeed->tbAllSpeed->Position;
    fSpeed->edAllAccSpeed->Text       =fSpeed->tbAccSpeed->Position;

    fSpeed->DoReplyDefaultToForm(fSpeed->tsIndex, 0);
    fSpeed->DoReplyDefaultToForm(fSpeed->tsInArm, 1);
    fSpeed->DoReplyDefaultToForm(fSpeed->tsOutArm, 2);
    fSpeed->DoReplyDefaultToForm(fSpeed->tsTrayArm, 3);
    fSpeed->DoReplyDefaultToForm(fSpeed->tsShuttle, 4);
    fSpeed->spbSaveClick(this);
    fSpeed->Close();

    fCleaning->Show();
    fCleaning->DoReplyDefaultToForm(fCleaning, 0);
    fCleaning->sbCleanSaveClick(this);
    fCleaning->Close();

    FTestIF->Show();
    FTestIF->DoReplyDefaultToForm(FTestIF, 0);
    FTestIF->spbSaveClick(this);
    FTestIF->Close();

    fYieldMonitoring->Show();
    fYieldMonitoring->DoReplyDefaultToForm(fYieldMonitoring, 0);
    fYieldMonitoring->btnApplyClick(this);
    fYieldMonitoring->Close();
}
//---------------------------------------------------------------------------
void __fastcall TfRPDefault::InitialComponentHead()
{
    strngrdReciepeParameterDefault->Cells[0][0]="Parameter";
    strngrdReciepeParameterDefault->Cells[1][0]="Default";
    strngrdReciepeParameterDefault->Cells[2][0]="Value";
}
//---------------------------------------------------------------------------
void __fastcall TfRPDefault::spbSaveClick(TObject *Sender)
{
    fSpeed->Show();
    fCleaning->Show();
    FTestIF->Show();
    fYieldMonitoring->Show();
    RP_Name->Clear();
    RP_Default->Clear();
    RP_Value->Clear();
    fSpeed->SearchRecipeParameter(fSpeed->tsIndex, 0);
    fSpeed->SearchRecipeParameter(fSpeed->tsInArm, 1);
    fSpeed->SearchRecipeParameter(fSpeed->tsOutArm, 2);
    fSpeed->SearchRecipeParameter(fSpeed->tsTrayArm, 3);
    fSpeed->SearchRecipeParameter(fSpeed->tsShuttle, 4);
    fCleaning->SearchRecipeParameter(fCleaning, 0);
    FTestIF->SearchRecipeParameter(FTestIF, 0);
    fYieldMonitoring->SearchRecipeParameter(fYieldMonitoring, 0);

    CompareRPDefaultAndValue(RP_Name, RP_Default, RP_Value);

    fSpeed->Close();
    fCleaning->Close();
    FTestIF->Close();
    fYieldMonitoring->Close();

    fMain->BackupSetupFile();               //Ifor 20170620 (wei) add Auto BackUp Setup File & Last Data
}
//---------------------------------------------------------------------------
void __fastcall TfRPDefault::CompareRPDefaultAndValue(TStringList *aName, TStringList *aDefault, TStringList *aValue)
{
    int iRPCount;
    AnsiString aSection="";
    AnsiString aPath="";
    AnsiString asMsg;
    TStringList *slDefaultRecipeChangeLog = new TStringList;
    slDefaultRecipeChangeLog->Clear();
    static AnsiString asOldValue[256]={""};
    bool bHasChange=false;
    int a;
    int iVisibleCnt[eRPVisibleCnt]={0};

    for(int i=0;i<eRPVisibleCnt;i++)
    {
        iVisibleCnt[i]=0;
    }

    TTabSheet *tsVisible[eRPVisibleCnt]=
    {
        fRecipeDefault->tsIndex,
        fRecipeDefault->tsInputArm,
        fRecipeDefault->tsOutputArm,
        fRecipeDefault->tsShuttle,
        fRecipeDefault->tsTrayArm,
        fRecipeDefault->tsAutoClean,
        fRecipeDefault->tsYield,
        fRecipeDefault->tsTester
    };

    RECEIPE_DEFAULT ReceipeDefault_PARA[]=
    {
        //Index
        { fRecipeDefault->lbIndexunit_Speed                                       , fRecipeDefault->pnlIndexunit_Speed },
        { fRecipeDefault->lbIndexunit_VaccumCheckTime                             , fRecipeDefault->pnlIndexunit_VaccumCheckTime },
        { fRecipeDefault->lbIndexunit_CounterAirOnTime                            , fRecipeDefault->pnlIndexunit_CounterAirOnTime },
        { fRecipeDefault->lbIndexunit_DestoryAgainTime                            , fRecipeDefault->pnlIndexunit_DestoryAgainTime },
        { fRecipeDefault->lbIndexunit_DestoryAgainCount                           , fRecipeDefault->pnlIndexunit_DestoryAgainCount },
        { fRecipeDefault->lblInitialMaxTest                                       , fRecipeDefault->pnlInitialMaxTest },
        { fRecipeDefault->lblMaxTestTime                                          , fRecipeDefault->pnlMaxTestTime },
        { fRecipeDefault->lblStartDelayTime                                       , fRecipeDefault->pnlStartDelayTime },

        //Input
        { fRecipeDefault->lbInput_XYSpeed                                         , fRecipeDefault->pnlInput_XYSpeed },
        { fRecipeDefault->lbInput_ZSpeed                                          , fRecipeDefault->pnlInput_ZSpeed  },
        { fRecipeDefault->lbInput_PitchSpeed                                      , fRecipeDefault->pnlInput_PitchSpeed },
        { fRecipeDefault->lbInput_XYAcc                                           , fRecipeDefault->pnlInput_XYAcc },
        { fRecipeDefault->lbInput_ZAcc                                            , fRecipeDefault->pnlInput_ZAcc },
        { fRecipeDefault->lbInput_PitchAcc                                        , fRecipeDefault->pnlInput_PitchAcc },
        { fRecipeDefault->lbInput_VacumCheckTime                                  , fRecipeDefault->pnlInput_VacumCheckTime },
        { fRecipeDefault->lbInput_ArmAirOnTime                                    , fRecipeDefault->pnlInput_ArmAirOnTime },
        { fRecipeDefault->lbInput_ArmShuttleWaitTime                              , fRecipeDefault->pnlInput_ArmShuttleWaitTime },
        { fRecipeDefault->lbInput_ArmDestroyAgainTime                             , fRecipeDefault->pnlInput_ArmDestroyAgainTime },
        { fRecipeDefault->lbInput_ArmDestroyAgainCount                            , fRecipeDefault->pnlInput_ArmDestroyAgainCount },

        //Output
        { fRecipeDefault->lbOutput_XYSpeed                                        , fRecipeDefault->pnlOutput_XYSpeed },
        { fRecipeDefault->lbOutput_ZSpeed                                         , fRecipeDefault->pnlOutput_ZSpeed  },
        { fRecipeDefault->lbOutput_PitchSpeed                                     , fRecipeDefault->pnlOutput_PitchSpeed },
        { fRecipeDefault->lbOutput_OutXYAcc                                       , fRecipeDefault->pnlOutput_OutXYAcc },
        { fRecipeDefault->lbOutput_ZAcc                                           , fRecipeDefault->pnlOutput_ZAcc },
        { fRecipeDefault->lbOutput_PitchAcc                                       , fRecipeDefault->pnlOutput_PitchAcc },
        { fRecipeDefault->lbAutoClean_InArmSpeed                                  , fRecipeDefault->pnlOutput_VacumCheckTime  },
        { fRecipeDefault->lbOutput_ArmAirOnTime                                   , fRecipeDefault->pnlOutput_ArmAirOnTime },
        { fRecipeDefault->lbOutput_ArmDestroyAgainTime                            , fRecipeDefault->pnlOutput_ArmDestroyAgainTime },
        { fRecipeDefault->lbOutput_ArmDestroyAgainCount                           , fRecipeDefault->pnlOutput_ArmDestroyAgainCount },

        //Shuttle
        { fRecipeDefault->lbShuttle_Shuttle1Speed                                 , fRecipeDefault->pnlShuttle_Shuttle1Speed  },
        { fRecipeDefault->lbShuttle_Shuttle2Speed                                 , fRecipeDefault->pnlShuttle_Shuttle2Speed },
        { fRecipeDefault->lbShuttle_Shuttle1Accel                                 , fRecipeDefault->pnlShuttle_Shuttle1Accel },
        { fRecipeDefault->lbShuttle_Shuttle2Accel                                 , fRecipeDefault->pnlShuttle_Shuttle2Accel },

        //TrayArm
        { fRecipeDefault->lbTrayArm_XSpeed                                        , fRecipeDefault->pnlTrayArm_XSpeed },
        { fRecipeDefault->lbTrayArm_XAcc                                          , fRecipeDefault->pnlTrayArm_XAcc  },
        { fRecipeDefault->lbTrayArm_RetryCount                                    , fRecipeDefault->pnlTrayArm_RetryCount },
        { fRecipeDefault->lbTrayArm_VacuumCheckTime                               , fRecipeDefault->pnlTrayArm_VacuumCheckTime },
        { fRecipeDefault->lbTrayArm_CounterAirONTime                              , fRecipeDefault->pnlTrayArm_CounterAirONTime },
        { fRecipeDefault->lbTrayArm_HandDownTime                                  , fRecipeDefault->pnlTrayArm_HandDownTime },

        //AutoClean
        { fRecipeDefault->lbAutoClean_AlarmCount                                  , fRecipeDefault->pnlAutoClean_AlarmCount },
        { fRecipeDefault->lbAutoClean_OutArmSpeed                                 , fRecipeDefault->pnlAutoClean_OutArmSpeed  },
        { fRecipeDefault->lbAutoClean_ShuttleSpeed                                , fRecipeDefault->pnlAutoClean_ShuttleSpeed },
        { fRecipeDefault->lbAutoClean_RotationStageSpeed                          , fRecipeDefault->pnlAutoClean_RotationStageSpeed },
        { fRecipeDefault->lbAutoClean_ContactTime                                 , fRecipeDefault->pnlAutoClean_ContactTime },
        { fRecipeDefault->lbAutoClean_ContactCount                                , fRecipeDefault->pnlAutoClean_ContactCount },
        { fRecipeDefault->lblAutoCleaning_ModeIntervalContactCount                , fRecipeDefault->pnlAutoCleaning_ModeIntervalContactCount },
        { fRecipeDefault->lblAutoCleaning_ModeSocketAlarmCount                    , fRecipeDefault->pnlAutoCleaning_ModeSocketAlarmCount },

        //Yield
        { fRecipeDefault->lblLowYieldEnable_FT                                    , fRecipeDefault->pnlLowYieldEnable_FT },
        { fRecipeDefault->lblLowYield_FT                                          , fRecipeDefault->pnlLowYield_FT  },
        { fRecipeDefault->lblLowYieldIg                                           , fRecipeDefault->pnlLowYieldIg },
        { fRecipeDefault->lblContsFailSocketAlarmCT_FT                            , fRecipeDefault->pnlContsFailSocketAlarmCT_FT },
        { fRecipeDefault->lblContsFailHeadAlarmCT_FT                              , fRecipeDefault->pnlContsFailHeadAlarmCT_FT },
        { fRecipeDefault->lblAlarm4ContinueType                                   , fRecipeDefault->pnlAlarm4ContinueType },
        { fRecipeDefault->lblAlarm4IntervalCount                                  , fRecipeDefault->pnlAlarm4IntervalCount },
        { fRecipeDefault->lblAlarm4ContinueCount                                  , fRecipeDefault->pnlAlarm4ContinueCount },
        { fRecipeDefault->lblAlarm4EnableIntervalYield                            , fRecipeDefault->pnlAlarm4EnableIntervalYield },
        { fRecipeDefault->lblAlarm4IntervalYieldYield                             , fRecipeDefault->pnlAlarm4IntervalYieldYield },
        { fRecipeDefault->lblAlarm4IntervalYieldIntervalCount                     , fRecipeDefault->pnlAlarm4IntervalYieldIntervalCount },
        { fRecipeDefault->lblAlarm4IntervalYieldContinueCount                     , fRecipeDefault->pnlAlarm4IntervalYieldContinueCount },
        { fRecipeDefault->lblSiteToSiteYield                                      , fRecipeDefault->pnlSiteToSiteYield },
        { fRecipeDefault->lblHeadToHeadYield                                      , fRecipeDefault->pnlHeadToHeadYield },
    };
    strngrdReciepeParameterDefault->RowCount=aName->Count+1;
    InitialComponentHead();
    for(int i=0; i<aName->Count; i++)
    {
        strngrdReciepeParameterDefault->Cells[0][i+1]=aName->Strings[i];
        strngrdReciepeParameterDefault->Cells[1][i+1]=aDefault->Strings[i];
        strngrdReciepeParameterDefault->Cells[2][i+1]=aValue->Strings[i];
    }
    bool bReceipeDefaultDiffShow=false;
    iRPCount=sizeof(ReceipeDefault_PARA)/sizeof(RECEIPE_DEFAULT);

    //Sam 20201209 : Default Recipe ChangeLog
    //==>
    if(CosFunction.bRecipeParameterDefaultLog)
    {
        asMsg.sprintf("USER ID:%s",fPassword->edUserName->Text);
        slDefaultRecipeChangeLog->Add(asMsg);
        for(a=0; a<iRPCount; a++)
        {
            ReceipeDefault_PARA[a].PLabel->Visible=false;
            ReceipeDefault_PARA[a].PPanel->Visible=false;
        }
    }
    //<==
    //Sam 20201209 : Default Recipe ChangeLog

    for(int i=0; i<aName->Count; i++)
    {
        for(int j=0; j<iRPCount; j++)
        {
            if(aName->Strings[i]==ReceipeDefault_PARA[j].PLabel->Caption)
            {
                ReceipeDefault_PARA[j].PPanel->Caption=aValue->Strings[i];

                if(asOldValue[i]!=aValue->Strings[i])
                {
                    asOldValue[i]=aValue->Strings[i];
                    bHasChange=true;
                }

                if(aDefault->Strings[i]==aValue->Strings[i])
                {
                    ReceipeDefault_PARA[j].PPanel->Color=clGreen;
                }
                else
                {
                    ReceipeDefault_PARA[j].PPanel->Color=clRed;
                    bReceipeDefaultDiffShow=true;
                }
                //Sam 20201209 : Default Recipe ChangeLog
                //==>
                if(CosFunction.bRecipeParameterDefaultLog)
                {
                    if(aName->Strings[i].Pos("Index_")==1)
                        iVisibleCnt[eRPVisibleIndex]++;
                    else if(aName->Strings[i].Pos("Input_")==1)
                        iVisibleCnt[eRPVisibleInput]++;
                    else if(aName->Strings[i].Pos("Output_")==1)
                        iVisibleCnt[eRPVisibleOutput]++;
                    else if(aName->Strings[i].Pos("Shuttle_")==1)
                        iVisibleCnt[eRPVisibleShuttle]++;
                    else if(aName->Strings[i].Pos("TrayArm_")==1)
                        iVisibleCnt[eRPVisibleTrayArm]++;
                    else if(aName->Strings[i].Pos("AutoClean_")==1)
                        iVisibleCnt[eRPVisibleAutoClean]++;
                    else if(aName->Strings[i].Pos("Yield_")==1)
                        iVisibleCnt[eRPVisibleYield]++;
                    else if(aName->Strings[i].Pos("Tester_")==1)
                        iVisibleCnt[eRPVisibleTester]++;

                    ReceipeDefault_PARA[j].PLabel->Visible=true;
                    ReceipeDefault_PARA[j].PPanel->Visible=true;
                    asMsg.sprintf("%s,%s,%s",aName->Strings[i],aDefault->Strings[i],aValue->Strings[i]);
                    slDefaultRecipeChangeLog->Add(asMsg);
                }
                //<==
                //Sam 20201209 : Default Recipe ChangeLog
            }
        }
    }
    //Sam 20201209 : Default Recipe ChangeLog
    //==>
    if(CosFunction.bRecipeParameterDefaultLog)
    {
        for(int i=0;i<eRPVisibleCnt;i++)
        {
            if(iVisibleCnt[i]>0)
                tsVisible[i]->TabVisible=true;
            else
                tsVisible[i]->TabVisible=false;
        }

        try
        {
            MyForceDirectories(asDefaultRecipeChangeLogPath);
            aPath.sprintf("%s\\%s_%s.txt", asDefaultRecipeChangeLogPath, IniConfig.SocketHandlerID, Now().FormatString("yyyymmddhhnnss"));

            if(bHasChange || RPLogRecord)  //有改變才需要保存
            {
                slDefaultRecipeChangeLog->SaveToFile(aPath);
                if(IniConfig.bN14_20_DefaultRecipeChangeLog)
                {
                    aPath.sprintf("%s\\%s_%s.txt", IniConfig.asN14_20_ChangeLogPath, IniConfig.SocketHandlerID, Now().FormatString("yyyymmddhhnnss"));
                    slDefaultRecipeChangeLog->SaveToFile(aPath);
                }
            }
        }
        catch(...)
        {
        }
    }
    slDefaultRecipeChangeLog->Clear();
    delete slDefaultRecipeChangeLog;
    //<==
    //Sam 20201209 : Default Recipe ChangeLog

    if(bReceipeDefaultDiffShow)
        fRecipeDefault->Show();
    else if(bReceipeDefaultDiffShow==false)
        fRecipeDefault->Close();
}
//---------------------------------------------------------------------------
void __fastcall TfRPDefault::FormShow(TObject *Sender)  //Isaac 20170718 (wei) defalut值比較功能:按鈕權限管控
{
    if(AccessLevel>=iDefHonPrecLevel)
    {
        spbSetRPDefault->Enabled=true;
    }
    else
    {
        spbSetRPDefault->Enabled=false;
    }
    Left=75;
    Top=75;
}
//---------------------------------------------------------------------------
void __fastcall TfRPDefault::ShowMonitoredParameter()   //Isaac 20210222 : 全智要求把參數拉出來看，方便監控
{
    AnsiString Path=AnsiString().sprintf("D:\\HT9045\\CurrentSetupData.txt");
    AnsiString s="";

    s=AnsiString().sprintf("AutoCleanEnable=%d",TestIF_File.iAutoClean_Function);
    WriteDataToFile(Path, s, true);

    s=AnsiString().sprintf("Setup File Name=%s",GetLastOpenFN());
    WriteDataToFile(Path, s);

    s=AnsiString().sprintf("Socket sensor on/off=%d",TestIF_File.bEnSocketSensor);
    WriteDataToFile(Path, s);

    s=AnsiString().sprintf("Start Mode=%d",LastSet.iRunStartMode);
    WriteDataToFile(Path, s);

    s=AnsiString().sprintf("Shuttle Mode=%d",TestIF_File.iShuttleMode);         //Jimmychiu 20260223 : Shuttle Mode加在CurrentSetupData
    WriteDataToFile(Path, s);
}
//---------------------------------------------------------------------------
