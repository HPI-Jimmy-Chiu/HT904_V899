#include "MachineDefine.h"
#pragma hdrstop

#include "cLd_ULd.h"
#include "cprod.h"
#include "common.h"
//#include "INPUT.h"
#include "cmydef.h"
#include "cSecurity.h"
#include "cBuilder.h"
#include "mycylin.h"
#include "myQwertyKeyBoard.h"
#include "main.h"
#include "HTEditList.h"
#include "uHGemHT9045.h"
#include "mymessbox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfLd_ULd *fLd_ULd;
//---------------------------------------------------------------------------
__fastcall TfLd_ULd::TfLd_ULd(TComponent* Owner)
    : TForm(Owner)
{
    LastFileName="";
    bool bShow=true; //bNoShow=false;
    bool bEnable=true;//, bDisable=false;
    bool bReadFromFile=true;//, bFixedValue=false;

    elUdUld->Add(edtLD_TrayArrivalDely,  &Ld_UldDelayTime.LD_TrayArrivalDely,           ECDouble,  "Loader Wait Time",    "Arrived",            bShow, bEnable, bReadFromFile, 0.2,      true,  10.0,      0.2);
    elUdUld->Add(edtLD_FixTrayDely,      &Ld_UldDelayTime.LD_FixTrayDely,               ECDouble,  "Loader Wait Time",    "Lock",               bShow, bEnable, bReadFromFile, 0.5,      true,  10.0,      0.2);
    elUdUld->Add(edtLD_MiddLockDelay,    &Ld_UldDelayTime.LD_StackMiddLockDelay,        ECDouble,  "Loader Wait Time",    "Close",              bShow, bEnable, bReadFromFile, 2.0,      true,  10.0,      0.2);
    elUdUld->Add(edtLD_BeforeDownDelay,  &Ld_UldDelayTime.LD_BeforeDownDelay,           ECDouble,  "Loader Wait Time",    "Down",               bShow, bEnable, bReadFromFile, 0.0,      true,  10.0,      0.0);    //Steven 20240215 : 下降前Delay
    elUdUld->Add(edtLD_LiftDownDelay,    &Ld_UldDelayTime.LD_LiftDownDelay,             ECDouble,  "Loader Wait Time",    "Check",              bShow, bEnable, bReadFromFile, 1.0,      true,  10.0,      0.2);

    elUdUld->Add(edtULD_TrayArrivalDelay,&Ld_UldDelayTime.ULD_TrayArrivalDelay,         ECDouble,  "UnLoader Wait Time",  "Unlock",             bShow, bEnable, bReadFromFile, 0.2,      true,  10.0,      0.2);
    elUdUld->Add(edtULD_FixTrayDely,     &Ld_UldDelayTime.ULD_FixTrayDely,              ECDouble,  "UnLoader Wait Time",  "Lock",               bShow, bEnable, bReadFromFile, 0.5,      true,  10.0,      0.2);
    if(TRAY_ARM_MODE==eUnderCoveyor)                                                    //Ztex 2024.03.25 Add Reduce tray exit time
        elUdUld->Add(edtULD_TrayBackDelay, &Ld_UldDelayTime.ULD_TrayBackDelay,          ECDouble,  "UnLoader Wait Time",  "Arrived",            bShow, bEnable, bReadFromFile, 1.0,      true,  10.0,      0.1);
    else
        elUdUld->Add(edtULD_TrayBackDelay, &Ld_UldDelayTime.ULD_TrayBackDelay,          ECDouble,  "UnLoader Wait Time",  "Arrived",            bShow, bEnable, bReadFromFile, 1.0,      true,  10.0,      1.0);    //JerryYang 20200408 有翻盤風險改最短1sec
    elUdUld->Add(edtULD_LiftDownDelay,   &Ld_UldDelayTime.ULD_LiftDownDelay,            ECDouble,  "UnLoader Wait Time",  "Check",              bShow, bEnable, bReadFromFile, 0.2,      true,  10.0,      0.2);
    elUdUld->Add(edtULD_TrayArrivalWait, &Ld_UldDelayTime.ULD_TrayArrivalWaitDelay,     ECDouble,  "UnLoader Wait Time",  "TrayArrivedWait",    bShow, bEnable, bReadFromFile, 0.3,      true,  10.0,      0.2);    //kevin 20190326 add auto 退tray 等待時間

    if(CosFunction.bKnockerSetBySetupFile==true && USE_LOADER_VIBRATION!=0)     //JerryYang 20191001 loader震動馬達
    {
        elUdUld->Add(cbF40,              &Ld_UldDelayTime.LD_EnableVibrate,             ECBool,    "Loader Wait Time",    "EnableVibrate",      bShow, bEnable, bReadFromFile, 0);
        elUdUld->Add(edP40_1,            &Ld_UldDelayTime.LD_VibrateLoopCnt,            ECInteger, "Loader Wait Time",    "VibrateLoopCount",   bShow, bEnable, bReadFromFile, 1,        true,  1,         15);
        elUdUld->Add(edP40_2,            &Ld_UldDelayTime.LD_VibrateOnDelay,            ECDouble,  "Loader Wait Time",    "VibrateOn",          bShow, bEnable, bReadFromFile, 10.0,     true,  1.0,       20.0);
        elUdUld->Add(edP40_3,            &Ld_UldDelayTime.LD_VibrateOffDelay,           ECDouble,  "Loader Wait Time",    "VibrateOff",         bShow, bEnable, bReadFromFile, 10.0,     true,  1.0,       15.0);
    }

    if(CosFunction.bKnockerSetBySetupFile==true && ShuttleVibration_INSTALL!=0) //JerryYang 20171006 (wei) Shuttle 震動馬達
    {
        elUdUld->Add(cbF23,              &IniConfig.bF23ShuttleVibration,                   ECBool,    "Tray",    "bF23ShuttleVibration",       bShow, bEnable, bReadFromFile, 0);
        if(CUSTOMER_CODE==CC_TERAPOWER)                                         //Sam 20210520 : TPW 修改震動時間 Range
            elUdUld->Add(edF23,          &IniConfig.iF23ShuttleVibrationTime,           ECInteger, "Shuttle", "iF23ShuttleVibrationTime",       bShow, bEnable, bReadFromFile, 20,       true,  1,         100);
        else
            elUdUld->Add(edF23,          &IniConfig.iF23ShuttleVibrationTime,           ECInteger, "Shuttle", "iF23ShuttleVibrationTime",       bShow, bEnable, bReadFromFile, 20,       true,  20,         100);
    }

    if(CosFunction.bKnockerSetBySetupFile==true)                                //Steven 20160329 : 敲擊汽缸參數調整可搭配工作檔處理
    {
        elUdUld->Add(cbP13,         &IniConfig.bP13EnableAutoTrayEdgePushCylinderLoop,  ECBool,     "Tray",     "bEnableAutoTrayEdgePushCylinderLoop",  bShow, bEnable, bReadFromFile, LastSet.bEnableAutoTrayEdgePushCylinderLoop?"1":"0");
        elUdUld->Add(edP13_1,       &IniConfig.iP13EdgePushCylinderLoopDelay,           ECInteger,  "Tray",     "iP13EdgePushCylinderLoopDelay",        bShow, bEnable, bReadFromFile, LastSet.iEdgePushCylinderLoopDelay,          true,  2,   300);
        elUdUld->Add(edP13_2,       &IniConfig.iP13EdgePushCylinderOnDelay,             ECInteger,  "Tray",     "iP13EdgePushCylinderOnDelay",          bShow, bEnable, bReadFromFile, 10,                                          true,  0,   50);

        elUdUld->Add(cbP14,         &IniConfig.bP14EnableAutoTrayRecevieDelayCount,     ECBool,     "Tray",     "bEnableAutoTrayRecevieDelayCount",     bShow, bEnable, bReadFromFile, LastSet.bEnableAutoTrayRecevieDelayCount?"1":"0");
        elUdUld->Add(edP14_1,       &IniConfig.iP14AutoTrayRecevieDelayCount,           ECInteger,  "Tray",     "iP14AutoTrayRecevieDelayCount",        bShow, bEnable, bReadFromFile, LastSet.iAutoTrayRecevieDelayCount,          true,  1,   1000);
        elUdUld->Add(edP14_2,       &IniConfig.iP14AutoTrayRecevieLoopDelayTime,        ECInteger,  "Tray",     "iP14AutoTrayRecevieLoopDelayTime",     bShow, bEnable, bReadFromFile, LastSet.iAutoTrayRecevieLoopDelayTime,       true,  2,   3000);

        elUdUld->Add(cbP16,         &IniConfig.bP16EnableHotplateEdgePushCylinderLoop,  ECBool,     "Hotplate", "bEnableHotplateEdgePushCylinderLoop",  bShow, bEnable, bReadFromFile, 0);

        if(TRAY_VIBRATION==VibrationMotor)                                      //JerryYang 20170531 (wei) 敲擊方式改成可以分別設定on off時間
        {
            elUdUld->Add(edP16_1,   &IniConfig.iP16HotplateEdgePushCylinderLoopDelay,   ECInteger, "Tray",      "iHotplateEdgePushCylinderLoopDelay",   bShow, bEnable, bReadFromFile, 10,                                          true,  200,   600);
            elUdUld->Add(edP16_2,   &IniConfig.iP16HotplateEdgePushCylinderOnDelay,     ECInteger, "Tray",      "iHotplateEdgePushCylinderOnDelay",     bShow, bEnable, bReadFromFile, 10,                                          true,  0,   50);
        }
        else
        {
            elUdUld->Add(edP16_1,   &IniConfig.iP16HotplateEdgePushCylinderLoopDelay,   ECInteger, "Tray",      "iHotplateEdgePushCylinderLoopDelay",   bShow, bEnable, bReadFromFile, 10,                                          true,  2,   600);
            elUdUld->Add(edP16_2,   &IniConfig.iP16HotplateEdgePushCylinderOnDelay,     ECInteger, "Tray",      "iHotplateEdgePushCylinderOnDelay",     bShow, bEnable, bReadFromFile, 10,                                          true,  2,   50);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLd_ULd::FormShow(TObject *Sender)
{
    AnsiString S;
    LoadImage();
    LastFileName=GetLastOpenFN();
    ReadFile();
    S.sprintf("Loader/Unloader Condition  ''%s''  ",LastFileName);
    Caption=S;
    SetDefaultPos();                                                            //Steven 20240215 : 調整預設位置

    grpLoader->Enabled      =fSecurity->Insufficient(115, false);               //jou 981207 權限控制
    grpUnloader->Enabled    =fSecurity->Insufficient(115, false);
    btnDefaultValue->Enabled=fSecurity->Insufficient(115, false);
    tsKnocker->TabVisible   =(CosFunction.bKnockerSetBySetupFile);              //Steven 20160329 : 敲擊汽缸參數調整可搭配工作檔處理
    palKnocker->Enabled     =fSecurity->Insufficient(116, false);

    if(CosFunction.bKnockerSetBySetupFile==true && TRAY_VIBRATION==VibrationMotor)
    {
        labP13->Caption="[P13-1] Off delay time                   (Unit : 0.1 Sec)";
        labP16->Caption="[P16-1] Off delay time                   (Unit : 0.1 Sec)";
    }

    if(CosFunction.bKnockerSetBySetupFile==false || USE_LOADER_VIBRATION==0)    //JerryYang 20191001 loader震動馬達
    {
        cbF40->Visible=false;
        edP40_1->Visible=false;
        edP40_2->Visible=false;
        edP40_3->Visible=false;
        labF40_1->Visible=false;
        labF40_2->Visible=false;
        labF40_3->Visible=false;
    }

    if(ShuttleVibration_INSTALL==0)                                             //JerryYang 20171006 (wei) Shuttle 震動馬達
    {
        cbF23->Visible=false;
        edF23->Visible=false;                                                   //JerryYang 20171205 (Steven) shuttle震動馬達功能
    }

    fShow=true;
    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);                                  //Steven 20100629
}
//---------------------------------------------------------------------------
void TfLd_ULd::SetDefaultPos()                                                  //Steven 20240215 : 調整預設位置
{
    Left=150;
    Top =10;
}
//---------------------------------------------------------------------------
void __fastcall TfLd_ULd::LoadImage()
{
    Image1->Picture->LoadFromFile(BmpPath+"LoaderCondition.bmp");
    Image2->Picture->LoadFromFile(BmpPath+"UnloaderCondition.bmp");
}
//---------------------------------------------------------------------------
void __fastcall TfLd_ULd::ReadFile()
{
    AnsiString szDir="";
    LastFileName=GetLastOpenFN();
    szDir.sprintf("%s%s\\", DataPath, LastFileName);

    if(elUdUld!=NULL)
    {
        elUdUld->ReadEditTextFromFile(szDir, "UdUld.Data");
        elUdUld->InitialDataToEdit();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLd_ULd::DoIniDataToForm()
{
    if(elUdUld!=NULL)
    {
        elUdUld->InitialDataToEdit();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLd_ULd::edtLD_TrayArrivalDelyKeyPress(TObject *Sender, char &Key)
{
    if(OnlyNumberAndDotInPut(Key)==false)
        Key=NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfLd_ULd::FormClose(TObject *Sender, TCloseAction &Action)
{
    ReadFile();
    rbTemp->SetFocus();                                                         //Steven 20240215 : 避免Enable false的時候跳例外
    fShow=false;
    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);                                  //Steven 20100629
}
//---------------------------------------------------------------------------
void __fastcall TfLd_ULd::spbSaveClick(TObject *Sender)
{
    if(IniConfig.bA02DisableSaveParsWhenSwitchToOp==true &&                     //RogerYang 20260305 張寧說時間到都要登出，且不可被更改任何設定(A01_2)
        AccessLevel==0)
    {
        ShowMyMessage("[A01_2]目前已切換到Operator權限，\r\n請重新登入再做設定!");
        Close();
    }

    AnsiString szDir="";
    szDir.sprintf("%s%s\\", DataPath, LastFileName);                            //Steven 20210716 : Add \\

    SaveSetupFile(szDir, LastFileName);                                         //kevin 20150105  Start 另存 jobfile
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
//------------------------------------------------------------
//kevin 20140105 儲存 檔案另存 jobfile
//------------------------------------------------------------
void __fastcall TfLd_ULd::SaveSetupFile(AnsiString szDir, AnsiString S)
{
    MyForceDirectories(szDir);

    if(elUdUld!=NULL)
    {
        elUdUld->SaveEditTextToFile(szDir, "UdUld.Data");
    }

    fMain->BackupSetupFile();                                                   //Ifor 20170620 (wei) add Auto BackUp Setup File & Last Data
}
//---------------------------------------------------------------------------
void __fastcall TfLd_ULd::sbtExitClick(TObject *Sender)
{
    sbtExit->Down=false;
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfLd_ULd::btnDefaultValueClick(TObject *Sender)
{
    //Loader
    edtLD_TrayArrivalDely->Text     =0.2;                                       //tray Arrival Lock (Sec)
    edtLD_FixTrayDely->Text         =0.5;                                       //Lock-Preparation is completed(Sec)    //jou 2014-05-29 load/unload tray lock wait define 0.2 -> 0.5 sec
    edtLD_MiddLockDelay->Text       =2;                                         //Lifter down to the middle-separator closed(Sec)
    edtLD_LiftDownDelay->Text       =1;                                         //Lifter down-Tray exist/non-exit check(Sec)

    //Unloader
    edtULD_TrayArrivalDelay->Text   =0.2;                                       //tray Arrival Lock (Sec)
    edtULD_FixTrayDely->Text        =0.5;                                       //Lock-Preparation is completed(Sec)    //jou 2014-05-29 load/unload tray lock wait define 0.2 -> 0.5 sec
    edtULD_TrayBackDelay->Text      =1;                                         //Lifter down to the middle-separator closed(Sec)  //JerryYang 20200408 有翻盤風險改最短1sec
    edtULD_LiftDownDelay->Text      =0.2;                                       //Lifter down-Tray exist/non-exit check(Sec)
}
//---------------------------------------------------------------------------
void __fastcall TfLd_ULd::FormShortCut(TWMKey &Msg, bool &Handled)
{
    if(Msg.CharCode==VK_F5)                                                     //Sam 20220725 : 介面新增鍵盤 F5 將視窗恢復初始位置
    {
        SetDefaultPos();                                                        //Steven 20240215 : 調整預設位置
    }
}
//---------------------------------------------------------------------------
//Eastsun 20260525 P40 splice (from KYEC AutoUP) - loader跛動馬達
void __fastcall TfLd_ULd::edP40_1Click(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 1, 10);  //JerryYang 20191001 loader跛動馬達
}
//---------------------------------------------------------------------------
void __fastcall TfLd_ULd::edP40_2DblClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 1, 20);  //JerryYang 20191001 loader跛動馬達
}
//---------------------------------------------------------------------------
void __fastcall TfLd_ULd::edP40_3Click(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 1, 15);  //JerryYang 20191001 loader跛動馬達
}
//---------------------------------------------------------------------------
