#include "MachineDefine.h"
#pragma hdrstop

#include "QAMode.h"

#include "cprod.h"
#include "common.h"
#include "cTesterIF.h"
#include "cTemperFrom.h"
//#include "INPUT.h"
#include "main.h"
#include "cpublic.h"
#include "cmydef.h"
#include "myQwertyKeyBoard.h"
#include "mymessbox.h"
#include "uLotInfo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfQAMode *fQAMode;
//---------------------------------------------------------------------------
__fastcall TfQAMode::TfQAMode(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfQAMode::btnOkClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfQAMode::edQAModeMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 10000, 5);
}
//---------------------------------------------------------------------------
void TfQAMode::DoIniDataToForm()
{
    fLotInfo->edQAMode->Text         =TestIF_File.iQAModeCount;                 //JerryYang 20190701 SPIL要求主畫面可切換EQC mode
    edQAMode->Text                  =TestIF_File.iQAModeCount;                  //Steven 20111026
    rgQARunMode->ItemIndex          =TestIF_File.iQAModeRunType;                //Steven 20120612 : QA做完後的動作
    cbQAModeBin->ItemIndex          =TestIF_File.iQAModeBin;                    //Steven 20141023 : QA做完後的Bin
    cbTrayEndDoQAModeAgain->Checked =TestIF_File.bQAModeAfterTrayEnd;           //Steven 20151125 : QA做完後的TrayEnd要重做QA
    cbQAD22Double->Checked          =TestIF_File.bQAD22DoubleContact;           //Sam 20231117 : 整合到 QA 模式
    cbbQAD22Double->ItemIndex       =TestIF_File.iQAD22DoubleContactCount;      //Sam 20231117 : 整合到 QA 模式
    cbTrayEndCloseYield100Site->Checked=TestIF_File.bQATrayEndCloseYield100Site;//Sam 20231117 : 整合到 QA 模式

    cbTempOffset->Checked           =TestIF_File.bEnableQASampling;             //Steven 20190326 : QA Sampling
    cbbQASampleBin->ItemIndex       =TestIF_File.iQASamplingBin;
//    cbbQASampleTray->ItemIndex      =TestIF_File.iQASamplingT3Pos-1;          //因為0是no-use
    cbbQASampleTray->Text           =TestIF_File.sQASamplingTray;
    edtQASampleCnt->Text            =TestIF_File.iQASamplingCnt;
}
//---------------------------------------------------------------------------
void TfQAMode::DoFormToData()
{
    TestIF_File.iQAModeCount        =CheckRange(atoi(edQAMode->Text.c_str()), 5, 10000);    //Steven 20111026
    TestIF_File.iQAModeRunType      =rgQARunMode->ItemIndex;                                //Steven 20120612 : QA做完後的動作
    TestIF_File.iQAModeBin          =cbQAModeBin->ItemIndex;                                //Steven 20141023 : QA做完後的Bin
    TestIF_File.bQAModeAfterTrayEnd =cbTrayEndDoQAModeAgain->Checked;                       //Steven 20151125 : QA做完後的TrayEnd要重做QA
    TestIF_File.bQAD22DoubleContact =cbQAD22Double->Checked;                                //Sam 20231117 : 整合到 QA 模式
    TestIF_File.iQAD22DoubleContactCount=cbbQAD22Double->ItemIndex;                         //Sam 20231117 : 整合到 QA 模式
    TestIF_File.bQATrayEndCloseYield100Site=cbTrayEndCloseYield100Site->Checked;            //Sam 20231117 : 整合到 QA 模式

    TestIF_File.bEnableQASampling   =cbTempOffset->Checked;                     //Steven 20190326 : QA Sampling
    TestIF_File.iQASamplingBin      =cbbQASampleBin->ItemIndex;
//    TestIF_File.iQASamplingT3Pos     =cbbQASampleTray->ItemIndex+1;           //因為0是no-use
    TestIF_File.sQASamplingTray     =cbbQASampleTray->Text;
    TestIF_File.iQASamplingCnt      =atoi(edtQASampleCnt->Text.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TfQAMode::btnApplyClick(TObject *Sender)
{
    if(IniConfig.bA02DisableSaveParsWhenSwitchToOp==true &&                     //RogerYang 20260305 張寧說時間到都要登出，且不可被更改任何設定(A01_2)
        AccessLevel==0)
    {
        ShowMyMessage("[A01_2]目前已切換到Operator權限，\r\n請重新登入再做設定!");
        Close();
    }

    DoFormToData();

    AnsiString S="";
    S=GetLastOpenFN();
    AnsiString szDir="";
    szDir.sprintf("%s%s", DataPath, S);
    MyForceDirectories(szDir);
    szDir+="\\Tester.Data";

    WriteIniData(szDir, "QA Mode", "Count",                 int(TestIF_File.iQAModeCount));     //Steven 20111026
    WriteIniData(szDir, "QA Mode", "Run Type",              TestIF_File.iQAModeRunType);        //Steven 20120612 : QA做完後的動作
    WriteIniData(szDir, "QA Mode", "Tray Direct",           TestIF_File.iQATrayDirect);
    WriteIniData(szDir, "QA Mode", "Untest Bin",            TestIF_File.iQAModeBin);            //Steven 20141023 : QA做完後的Bin
    WriteIniData(szDir, "QA Mode", "bQAModeAfterTrayEnd",   TestIF_File.bQAModeAfterTrayEnd);   //Steven 20151125 : QA做完後的TrayEnd要重做QA
    WriteIniData(szDir, "QA Mode", "bQAD22DoubleContact",   TestIF_File.bQAD22DoubleContact);   //Sam 20231117 : 整合到 QA 模式
    WriteIniData(szDir, "QA Mode", "iQAD22DoubleContactCount", TestIF_File.iQAD22DoubleContactCount);       //Sam 20231117 : 整合到 QA 模式
    WriteIniData(szDir, "QA Mode", "bQATrayEndCloseYield100Site",   TestIF_File.bQATrayEndCloseYield100Site);   //Sam 20231117 : 整合到 QA 模式

    WriteIniData(szDir, "QA Sampling", "Enable",    TestIF_File.bEnableQASampling);             //Steven 20190326 : QA Sampling
    WriteIniData(szDir, "QA Sampling", "Bin",       TestIF_File.iQASamplingBin);
//    WriteIniData(szDir, "QA Sampling", "Tray",      TestIF_File.iQASamplingTray);
    WriteIniData(szDir, "QA Sampling", "Tray Name", TestIF_File.sQASamplingTray);               //Steven 20230930 : 改用Tray名稱存檔, 避免異常
    WriteIniData(szDir, "QA Sampling", "Count",     TestIF_File.iQASamplingCnt);
    fMain->BackupSetupFile();                                                   //Ifor 20170620 (wei) add Auto BackUp Setup File & Last Data
    ReadFile();                                                                 //JerryYang 20190701 SPIL要求主畫面可切換EQC mode
    DoIniDataToForm();

    fBinSel->ReadFile(false, false, "");                                                        //Steven 20190326 : QA Sampling
}
//---------------------------------------------------------------------------
void __fastcall TfQAMode::FormShow(TObject *Sender)
{
    cbQAModeBin->Clear();                                                       //Steven 20150122 : Fixed for QA Mode
    for(int i=0; i<iTestBinCount; i++)
        cbQAModeBin->Items->Add(i);

    cbbQASampleTray->Clear();
    for(int i=0; i<iFixRight; i++)
    {
        if(Prod.iTrayType[i]==tTrayAuto)
            cbbQASampleTray->Items->Add(s6TrayName[i]);
    }

    Left=(1280-Width)/2;
    Top =(1024-Height)/2;

    ReadFile();
    DoIniDataToForm();
    ShowTrayDirectIMG();
    cbQAModeBin->Enabled=(AccessLevel<LevelSet.AccessLevel[85])?false:true;
    if(CosFunction.bQAModeUseUnloadCnt)                                         //JerryYang 20221004 : Maxim版本QA mode
    {
        rgQARunMode->Enabled=false;
    }
    else
    {
        rgQARunMode->Enabled=(AccessLevel<LevelSet.AccessLevel[85])?false:true;
    }
    gbLoaderDirection->Enabled=(AccessLevel<LevelSet.AccessLevel[85])?false:true;
    edQAMode->Enabled=(AccessLevel<LevelSet.AccessLevel[98])?false:true;
    pnlQASample->Enabled=(AccessLevel<LevelSet.AccessLevel[85])?false:true;     //Steven 20190326 : QA Sampling

    if(CUSTOMER_CODE==CC_SCS)
        rgQARunMode->Enabled=false;                                             // wei  20130103

    if(CUSTOMER_CODE==CC_SIGURD_PeiXing)                                        //Sam 20240122 : 矽格北興俊堯要求鎖死
    {
        edQAMode->Enabled=false;
        rgQARunMode->Enabled=false;
        cbQAModeBin->Enabled=false;
        cbTrayEndDoQAModeAgain->Enabled=false;
    }

    if(IniConfig.bMaximFunction==true ||                                        //JerryYang 20221004 : Maxim版本QA mode
       CUSTOMER_CODE==CC_AnalogDevice_Phil)
    {
        labQAModeBin->Visible=false;
        cbQAModeBin->Visible=false;
        cbTrayEndDoQAModeAgain->Visible=false;
    }

    fShow=true;
    tsQASampling->TabVisible=(CUSTOMER_CODE==CC_AMKOR_Korea &&
                              TestIF_File.bSCKART_EnableART);                   //Steven 20190326 : QA Sampling
    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);                                  //Steven 20100629
}
//---------------------------------------------------------------------------
void __fastcall TfQAMode::FormClose(TObject *Sender, TCloseAction &Action)
{
    fShow=false;
    DoIniDataToForm();                                                          //JerryYang 20250411 : 離開頁面要刷新一次, 避免誤存檔
    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);                                  //Steven 20100629
}
//---------------------------------------------------------------------------
void TfQAMode::ReadFile()
{
    AnsiString S="";
    S=GetLastOpenFN();
    AnsiString szDir="";
    szDir.sprintf("%s%s", DataPath, S);
    MyForceDirectories(szDir);
    szDir+="\\Tester.Data";

    TestIF_File.iQAModeCount    =CheckRange(ReadIniData(szDir, "QA Mode", "Count", 200), 5, 10000);  //Steven 20111026

    if(CUSTOMER_CODE==CC_SCS || CosFunction.bQAModeUseUnloadCnt)                //JerryYang 20221004 : Maxim版本QA mode
        TestIF_File.iQAModeRunType=1;                                           // wei  20130103
    else if(CUSTOMER_CODE==CC_SIGURD_PeiXing)                                   //Sam 20240122 : 矽格北興俊堯要求鎖死
        TestIF_File.iQAModeRunType=3;
    else
        TestIF_File.iQAModeRunType=ReadIniData(szDir, "QA Mode", "Run Type", 0);//Steven 20120612 : QA做完後的動作

    if(CUSTOMER_CODE==CC_AMKOR_Korea)                                           //ChungHung 20141204 add for ATK
        TestIF_File.iQAModeBin=ReadIniData(szDir, "QA Mode", "Untest Bin", 1);
    else
        TestIF_File.iQAModeBin=ReadIniData(szDir, "QA Mode", "Untest Bin", iTestBinCount-1);                    //Steven 20141023 : QA做完後的Bin
    TestIF_File.iQAModeBin=CheckRange(TestIF_File.iQAModeBin, 0, iTestBinCount-1);                              //Steven 20150122 : Fixed for QA Mode

    TestIF_File.iQATrayDirect      =CheckAndReadIniData(szDir, "QA Mode", "Tray Direct", TrayForm.Loader.Direction);
    TestIF_File.bQAModeAfterTrayEnd=CheckAndReadIniData(szDir, "QA Mode", "bQAModeAfterTrayEnd", false);        //Steven 20151125 : QA做完後的TrayEnd要重做QA
    TestIF_File.bQAD22DoubleContact=CheckAndReadIniData(szDir, "QA Mode", "bQAD22DoubleContact", false);        //Sam 20231117 : 整合到 QA 模式
    TestIF_File.iQAD22DoubleContactCount=CheckAndReadIniData(szDir, "QA Mode", "iQAD22DoubleContactCount", 1);              //Sam 20231117 : 整合到 QA 模式
    TestIF_File.bQATrayEndCloseYield100Site=CheckAndReadIniData(szDir, "QA Mode", "bQATrayEndCloseYield100Site", false);        //Sam 20231117 : 整合到 QA 模式

    TestIF_File.bEnableQASampling  =CheckAndReadIniData(szDir, "QA Sampling", "Enable",    false);             //Steven 20190326 : QA Sampling
    TestIF_File.iQASamplingBin     =CheckAndReadIniData(szDir, "QA Sampling", "Bin",       1);

    if(CheckKeyExist(szDir, "QA Sampling", "Tray Name")==false)
    {
        TestIF_File.iQASamplingT3Pos=CheckAndReadIniData(szDir, "QA Sampling", "Tray",      3);

        int iTray=TestIF_File.iQASamplingT3Pos-1;
        if(iTray<0)
            TestIF_File.sQASamplingTray=s3TrayName[0];
        else
            TestIF_File.sQASamplingTray=s3TrayName[iTray];
    }
    else
    {
        TestIF_File.sQASamplingTray=CheckAndReadIniData(szDir, "QA Sampling", "Tray Name", AnsiString("Fix1"));
        TestIF_File.iQASamplingT3Pos=0;                                         //因為0是no-use
        for(int i3=0; i3<e3TrayCount; i3++)
        {
            if(s3TrayName[i3]==TestIF_File.sQASamplingTray)
                TestIF_File.iQASamplingT3Pos=i3+1;
        }
    }
    TestIF_File.iQASamplingCnt     =CheckAndReadIniData(szDir, "QA Sampling", "Count",     100);
}
//---------------------------------------------------------------------------
void __fastcall TfQAMode::Image1Click(TObject *Sender)
{
    AnsiString sDir;

    int dir=TestIF_File.iQATrayDirect;
        dir++;
    if(dir>=8)
        dir=0;
    TestIF_File.iQATrayDirect=dir;
    sDir.sprintf("%stype%d.bmp", BmpPath, dir);
    try
    {
        Image1->Picture->LoadFromFile(sDir);
    }
    catch(...)
    {
        MyDBIProcess("Exception", "Image1Click");
        ShowMyMessage("Path :D:\\HT9045\\IMG\\BMP\\ be delete");
    }
}
//---------------------------------------------------------------------------
void __fastcall TfQAMode::ShowTrayDirectIMG()
{
    AnsiString sDir;
    if(CosFunction.bQAmodeSupplyTrayDir==false)
    {
        gbLoaderDirection->Visible=false;
    }
    else
    {
        if(TestIF_File.iQATrayDirect<0 || TestIF_File.iQATrayDirect>8)
            TestIF_File.iQATrayDirect=0;
        sDir.sprintf("%stype%d.bmp", BmpPath, TestIF_File.iQATrayDirect);
        try
        {
            Image1->Picture->LoadFromFile(sDir);
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TfQAMode::ShowTrayDirectIMG");
            ShowMyMessage("''Path :D:\\HT9045\\IMG\\BMP\\'' be delete");
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfQAMode::edtQASampleCntMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 10000, 0);
}
//---------------------------------------------------------------------------
void QABackupStatus(bool bBackup)                                               //Sam 20231117 : 整合到 QA 模式
{
    IntUnionBit iuTestSiteUse[2], iubUseTestSocket[2];
    AnsiString sPath=AuthPath+"config.ini";
    if(bBackup)
    {
        //備份 SiteMap On/Off 狀態
        for(int Z=0;Z<2;Z++)
        {
            iuTestSiteUse[Z].SetInt(0);
            iubUseTestSocket[Z].SetInt(0);
            for(int Y=0;Y<MAX_SOCKET_ROW;Y++)
            {
                for(int X=0;X<MAX_SOCKET_COL;X++)
                {
                    iuTestSiteUse[Z].SetBit(Y*MAX_SOCKET_COL+X,bTestSiteUse[Z][Y][X]);
                    iubUseTestSocket[Z].SetBit(Y*MAX_SOCKET_COL+X,LastSet.bUseTestSocket[Z][Y][X]);
                }
            }
            WriteIniData(sPath, "Index", "QAMode_BackupTestSiteUse"+IntToStr(Z),    (unsigned long)iuTestSiteUse[Z].UnsignInt);
            WriteIniData(sPath, "Index", "QAMode_BackupUseTestSocket"+IntToStr(Z),  (unsigned long)iubUseTestSocket[Z].UnsignInt);
        }
    }
    else
    {
        //還原 SiteMap On/Off 狀態
        for(int Z=0;Z<2;Z++)
        {
            iuTestSiteUse[Z].SetInt(ReadIniData(sPath, "Index", "QAMode_BackupTestSiteUse"+IntToStr(Z),0));
            iubUseTestSocket[Z].SetInt(ReadIniData(sPath, "Index", "QAMode_BackupUseTestSocket"+IntToStr(Z),0));
            for(int Y=0;Y<MAX_SOCKET_ROW;Y++)
            {
                for(int X=0;X<MAX_SOCKET_COL;X++)
                {
                    bTestSiteUse[Z][Y][X]=iuTestSiteUse[Z].Bit(Y*MAX_SOCKET_COL+X);
                    LastSet.bUseTestSocket[Z][Y][X]=iubUseTestSocket[Z].Bit(Y*MAX_SOCKET_COL+X);
                }
            }
        }
        fMain->ShowTestHeadComp(false);

        AnsiString S="";
        S=GetLastOpenFN();
        AnsiString szDir="";
        szDir.sprintf("%s%s", DataPath, S);
        MyForceDirectories(szDir);
        szDir+="\\Tester.Data";
        WriteIniData(szDir, "QA Mode", "bQAD22DoubleContact", false);           //Sam 20240122 : 北興俊堯要求恢復初始設定
        WriteIniData(szDir, "QA Mode", "bQATrayEndCloseYield100Site", true);    //Sam 20231117 : 整合到 QA 模式
    }
}
//---------------------------------------------------------------------------
