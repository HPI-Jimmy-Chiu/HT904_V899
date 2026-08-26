//---------------------------------------------------------------------------
#include "MachineDefine.h"                                                      //Sam 20170603 移植超豐 OEE 功能 form HT-7045
#pragma hdrstop

#include "ProductionInfo.h"

#include "IncludeAllHeader.H"
#include "main.h"
#include "mymessbox.h"
#include "note.h"
#include "common.h"
#include "cprod.h"
#include "cmydef.h"
#include "Command.h"
#include "cShowBinSelect.h"
#include "uLotInfo.h"
#include "FTPClient.h"
#include "cMyDB.h"
#include "cConfiguration.h"
#include "uYieldMonitoring.h"
#include "cBinSel.h"
#include "cContact.h"
#include "cTrayForm.h"
#include "cTrayAssignment.h"
#include "cHotPlate.h"
#include "cSpeed.h"
#include "uTemp_Set.h"
#include "cTesterIF.h"
#include "uCleaning.h"
#include "uESDControl.h"
#include "cSecurity.h"
#include "DIOInterFaceCFG.h"
#include "cUnitConvert.h"
#include "cinitial.h"
#include "cObserver.h"                                                          //KaiChen 20171127 ：超豐 OEE 新增 Test Time、Index Time
#include "iosetview.h"
#include "MyMotor.h"
#include "MyKitSuck.h"
#include "cSocket.h"
#include "atester_ProcessCount.h"                                               //Sam 20200525 : Control Bin
#include "RPDefault.h"
#include "TfFTP.h"
#include "FileInfo.h"
#include "AutoTeach.h"
#include "HTEditList.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfProductionInfo *fProductionInfo;
bool bOEEFTPAction=false;
//---------------------------------------------------------------------------
__fastcall TfProductionInfo::TfProductionInfo(TComponent* Owner)
    : TForm(Owner)
{
    bShow=false;                                                                //Sam 20200225 : OEE 功能防呆保護提示顯示在畫面，不要用 MessageBox。
    bRecFromErrorNote=false;                                                    //Jimmychiu 20220901 加入初始值
    CompareDataList=new TList;
    bAgreeExportConfig=false;
    sIPSCFlag_ShowMessageAndStop="";
    bIPSCFlag_ShowMessageAndStop=false;
    bIPSCFlag_NeedPause=false;
    bIPSCFlag_NeedStart=false;
    asTempstring="";
    bIPSCPause=false;
    cDynaThres=new cDynamicMultiContinualPassBinBySocket;
    ZeroMemory(iInArmZHeightDiff,       sizeof(iInArmZHeightDiff));
    ZeroMemory(iOutArmZHeightDiff,      sizeof(iOutArmZHeightDiff));
    bEnableInarmSuckZAuto=false;
    bEnableOutarmSuckZAuto=false;
    elData     =new HTEditList;
    InitAutoCalSuckZ();
    iInArmSearchStartZ=0;
    iOutArmSearchStartZ=0;
    bN14_21_UsedGDK04=false;
    InitialOEECount();

    iAutoClean_IntervalContact_Server=0;
    InitialOEECount();

    bN14_21_SetUpConfig_Success=false;
}
//---------------------------------------------------------------------------
void __fastcall TfProductionInfo::DoIniDataToForm()
{
    if(CosFunction.bOEEFunction==false)
        return;
    CopyIPSCback2IPSC();                                                        //JimmyChiu 20211227 : 讀取IPSC ini file(第一次執行讀取最後一次ini檔)
    ACM_WriteMsgAndCallExe_sMessage=AnsiString("");                             //Jimmychiu 20220901 降低ACM交握影響主程序效能
    iControlBinTimerCnt                     =0;                                 //Sam 20200525 : Control Bin
    bIsControlBinCounting                   =false;                             //Sam 20200525 : Control Bin
    bNeedLotEndAfterCleanOut=false;
    bPauseTimeNeedSaveAndUpdateOEEFiles=true;                                   //Mylin 20170705 Add OEE Report Field
    dOffsetContactForce=0.0;
    sPI_STime="";                                                               //Jimmychiu 20240119 : ScheduleName = Mo+Flow
    bStartNeedSaveAndUpdateOEEFiles=true;                                       //Sam 20171015 : Bug Fix
    OEE_SetHandlerID(IniConfig.SocketHandlerID);                                //Sam 20170801 HT-7045 變數改為 HT-9045 變數
    InitialOEECount();
    LoadSetting();
    sLocalMOFullFileName="";                                                    //Sam 20190227 : Try Catch Mo Download SetupFile
    if(FileExists("d:\\HT9045\\7z.exe")==false)
    {
        CopyFile("C:\\Program Files\\7-Zip\\7z.exe", "d:\\HT9045\\7z.exe", false);
    }
    bFTPError=false;
    bCycleTimeUp=false;
    InitialIPSCControlCount();
    SetNextRecordDateTime();
    if(_bOEEStartLotSuccess==true)
    {
        #ifndef SOFT_SIMULTE
        if(LoadMOInformation(true)==false)
        {
            AnsiString sMsg="Production Start Lot Fail!#Please Check Load MO File";
            ShowMyMessage(sMsg);
        }
        #endif
        //Mylin 20170331 (Joye,Hungyeh) Modify ESD Control Save To File and Show
        //==>
        AnsiString sSaveESDControlMsgFilePath=sProductionInfoFilePath + "\\" + sLoadMO_MO + "\\" + _sOEE_DirectoryName + "\\"; //Mylin 20170517 Add Load and Check MO Information
        AnsiString sSaveESDControlMsgFileName=_sOEE_MO + "_ESDControlMsg.csv";
        ESDForm->SetSaveESDControlMsgToFile(sSaveESDControlMsgFilePath, sSaveESDControlMsgFileName);
        //<==
        //Mylin 20170331 (Joye,Hungyeh) Modify ESD Control Save To File and Show
    }
    else
    {
        ESDForm->SetSaveESDControlMsgToFile("", "");
    }
    tm_PI->Enabled=true;
    tm_IPSCControl->Enabled=true;
    tIPSCTimer.LatchCycleTimeSec(true);
    tIPSCCycleTimer.LatchCycleTimeSec(true);
    tCheckFTPFileTimer.LatchCycleTimeSec(true);                                 //Sam 20200305 : 增加 FTP 檔案檢查，檢查到 VIE_STOP.txt 就執行 OneCycle
    tControlBinTimer.LatchCycleTimeSec(true);                                   //Sam 20200525 : Control Bin
    #ifndef SOFT_SIMULTE
    if(IniConfig.bN14_14_AlarmCtrlMachine)                                      //Sam 20210412 : 超豐新增軟體開啟時要丟訊息
        fProductionInfo->SetACM_WriteMsgAndCallExe_sMessage("Start");           //Jimmychiu 20220901 降低ACM交握影響主程序效能
    if(IniConfig.bN14_21_ConfigUpdateFromServerExport)
    {
        LoadConfigFromServer();
    }
    #endif
    elData->InitialDataToEdit();
    LoadAutoCalSuckZ();
}
//---------------------------------------------------------------------------
void __fastcall TfProductionInfo::FormDestroy(TObject *Sender)
{
    try
    {
        tm_PI->Enabled=false;
        tm_IPSCControl->Enabled=false;
        SaveSetting();
        delete cDynaThres;
        delete elData;
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfProductionInfo::FormDestroy");
    }
    LogSoftwareOffTime("TfProductionInfo, FormDestroy");                        //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
void __fastcall TfProductionInfo::FormShow(TObject *Sender)
{
    if(bShow==true)
        return;
    else
        bShow=true;                                                             //Sam 20200225 : OEE 功能防呆保護提示顯示在畫面，不要用 MessageBox。
    Left=(1024-Width)/2;
    Top=(768-Height)/2;
    AnsiString sPIChoiseMO="";
    dlb_PIChoiseMO->Directory=sProductionInfoFilePath;
    dlb_PIChoiseMO->Refresh();
    RefreshPIChoiseMO();
    bHadCloseInfo=LoadHaltAndPauseSelectStatusName();
    lbStatus_HALT->Caption="";
    SetInsertOPIDStr("");
    lbStatus_Pause->Caption="";
    edInsertOPID_Pause->Text="";
    if(tsHaltStatusForm->TabVisible==true && PageControl1->ActivePage==tsHaltStatusForm)
    {
        edInsertOPID_HALT->SetFocus();
        tsPauseStatusForm->TabVisible=false;
    }
    else if(tsPauseStatusForm->TabVisible==true && PageControl1->ActivePage==tsPauseStatusForm)
    {
        edInsertOPID_Pause->SetFocus();
        tsHaltStatusForm->TabVisible=false;
    }
    else
    {
        tsPauseStatusForm->TabVisible=false;
        tsHaltStatusForm->TabVisible=false;
    }
    sPauseButtonCaption="";
}
//---------------------------------------------------------------------------
void TfProductionInfo::LoadSetting()
{
    AnsiString sPISetting=GetProInfoFilePath()+"\\PI_Setting.ini";
    if(CosFunction.bOEEFunction==false)
        return;
    _sOEE_DirectoryName =ReadIniData(sPISetting, "Setting", "sOEE_DirectoryName", AnsiString(" ")); //資料夾名稱 YYYYMMDD_HHMMSS_MO
    _sOEE_MO            =ReadIniData(sPISetting, "Setting", "sOEE_MO", AnsiString(" ")); //此Lot的MO
    _bOEEStartLotSuccess=ReadIniData(sPISetting, "Setting", "OEE_StartLotSuccess", false);
    dOffsetContactForce =ReadIniData(sPISetting, "Setting", "dOffsetContactForce", 0.0);
}
//---------------------------------------------------------------------------
void TfProductionInfo::SaveSetting()
{
    AnsiString sPISetting=GetProInfoFilePath()+"\\PI_Setting.ini";
    AnsiString sFilePath=GetProInfoFilePath();
    if(CosFunction.bOEEFunction==false)
        return;
    MyForceDirectories(sFilePath, "TfProductionInfo::SaveSetting");
    WriteIniData(sPISetting, "Setting", "sOEE_DirectoryName",    _sOEE_DirectoryName);  //資料夾名稱 YYYYMMDD_HHMMSS_MO
    WriteIniData(sPISetting, "Setting", "OEE_StartLotTime",      _dtOEE_StartLotTime);
    WriteIniData(sPISetting, "Setting", "OEE_EndLotTime",        _dtOEE_EndLotTime);
    WriteIniData(sPISetting, "Setting", "OEE_PowerTime",         _iOEE_PowerTime);      //Power On時間
    WriteIniData(sPISetting, "Setting", "OEE_RunTime",           _iOEE_RunTime);        //Run時間
    WriteIniData(sPISetting, "Setting", "OEE_SoakTime",          _iOEE_SoakTime);       //Soak時間
    WriteIniData(sPISetting, "Setting", "OEE_StopTime",          _iOEE_StopTime);       //Stop時間
    WriteIniData(sPISetting, "Setting", "OEE_JamCount",          _iOEE_JamCount);       //Jam次數
    WriteIniData(sPISetting, "Setting", "OEE_ServiceCount",      _iOEE_ServiceCount);   //Error次數
    WriteIniData(sPISetting, "Setting", "sOEE_MO",               _sOEE_MO);             //此Lot的MO
    WriteIniData(sPISetting, "Setting", "sOEE_HandlerID",        _sOEE_HandlerID);      //Handler ID
    WriteIniData(sPISetting, "Setting", "OEE_StartLotSuccess",   _bOEEStartLotSuccess);
    WriteIniData(sPISetting, "Setting", "dOffsetContactForce",   dOffsetContactForce);  //JimmyChiu 20220121 add
}
//---------------------------------------------------------------------------
bool TfProductionInfo::OEE_StartLot(bool bSilent, AnsiString *pasErrorMsg)
{
    bN14_21_UsedGDK04=false;
    sLocalMOFullFileName="";                                                    //Sam 20190227 : Try Catch Mo Download SetupFile
    if(SetMOInformation()==false)
    {
        AnsiString sMsg = AnsiString(__FUNC__)+" Set MO Information Fail!";
        _bOEEStartLotSuccess=false;                                             //AI(ht9045-v899) 20260504: failed OEE start must not leave success flag true for remote START_REQUEST
        if(pasErrorMsg!=NULL)
            *pasErrorMsg=sMsg;
        if(bSilent==false)                                                       //AI(ht9045-v899) 20260504: keep manual OEE Start Lot modal behavior
            ShowMyMessage(sMsg);
        return false;
    }
    else
    {
        fLotInfo->ed_PIOEEMO->Text=GetScheduleName();                           //Jimmychiu 20240119 : ScheduleName = Mo+Flow
        if(IniConfig.bN14_4_OEEAutoLoadMOFile==false)
        {
            fLotInfo->ed_PIOEEMO->Text=sLoadMO_MO;                              //Sam 20170603 HT-7045 變數改為 HT-9045 變數
        }
    }

    if(_sOEE_MO=="" || _sOEE_HandlerID=="")
    {
        _bOEEStartLotSuccess=false;
        if(pasErrorMsg!=NULL)                                                    //AI(ht9045-v899) 20260504: expose missing OEE data to LotInfo_REPLY
            *pasErrorMsg="MO or Machine ID is empty";
        return false;
    }

    _dtOEE_StartLotTime=Now();
    AnsiString sFullFileName=sProductionInfoFilePath+"\\"+sLoadMO_MO+"\\"+_sOEE_DirectoryName;
    MyForceDirectories(sFullFileName, "TfProductionInfo::OEE_StartLot");
    _bOEEStartLotSuccess=true;
    cb_PIChoiseMO->Text=_sOEE_DirectoryName;

    AnsiString sSaveESDControlMsgFilePath=sFullFileName+"\\";
    AnsiString sSaveESDControlMsgFileName=_sOEE_MO+"_ESDControlMsg.csv";
    ESDForm->SetSaveESDControlMsgToFile(sSaveESDControlMsgFilePath, sSaveESDControlMsgFileName);
    ClearTrayCnt();
    bNeedLotEndAfterCleanOut=false;
    SaveSetting();

    bN14_21_SetUpConfig_Success=false;
    if(DoSetUpFileFromServer())
    {
        bN14_21_SetUpConfig_Success=true;
    }
    else if(IniConfig.bN14_10_DownFileByMO)    //Sam 20190227 : Try Catch Mo Download SetupFile
    {
        AnsiString sTargetFilePath=GetProInfoFilePath();
        AnsiString sSeparatedValues=":";
        AnsiString sLocalMOSetupFileFullFileName=sTargetFilePath+"\\TempLoadMO\\AutoLoad_TrayForm\\"+_sOEE_HandlerID+"_TrayForm.txt";
        sLoadMO_SetupFileName="";
        if(FileExists(sLocalMOSetupFileFullFileName) == true)
        {
            sLoadMO_SetupFileName=GetStringBySeparatedValues(sLocalMOSetupFileFullFileName,"TrayForm",sSeparatedValues);
            if(sLoadMO_SetupFileName.Length()==0)
            {
                AnsiString sMsg="Auto Download Setup File by MO LineString Error in LoadMOInformation";
                _bOEEStartLotSuccess=false;
                if(pasErrorMsg!=NULL)
                    *pasErrorMsg=sMsg;
                if(bSilent==false)                                               //AI(ht9045-v899) 20260504: suppress modal for Greatek remote LotInfo flow
                    ShowMyMessage(sMsg);
                return false;
            }
        }
        else
        {
            AnsiString sMsg="Auto Download Setup File by MO File Error in LoadMOInformation";
            _bOEEStartLotSuccess=false;
            if(pasErrorMsg!=NULL)
                *pasErrorMsg=sMsg;
            if(bSilent==false)                                                   //AI(ht9045-v899) 20260504: suppress modal for Greatek remote LotInfo flow
                ShowMyMessage(sMsg);
            return false;
        }

        if(LoadMOInformation(true)==true)
        {
            if(sLoadMO_SetupFileName==GetSetUpName())
            {
                //Do Nothing
            }
            else
            {
                if(AutoDownloadSetupFileByMO()==false)
                {
                    AnsiString sMsg="Production Start Lot Fail!#Please Check AutoDownloadSetupFileByMO() Function";
                    _bOEEStartLotSuccess=false;
                    if(pasErrorMsg!=NULL)
                        *pasErrorMsg=sMsg;
                    if(bSilent==false)                                           //AI(ht9045-v899) 20260504: suppress modal for Greatek remote LotInfo flow
                        ShowMyMessage(sMsg);
                    return false;
                }
            }
        }
        else
        {
            AnsiString sMsg = "Production Start Lot Fail!#Please CheckLoadMOInformation() Function";
            _bOEEStartLotSuccess=false;
            if(pasErrorMsg!=NULL)
                *pasErrorMsg=sMsg;
            if(bSilent==false)                                                   //AI(ht9045-v899) 20260504: suppress modal for Greatek remote LotInfo flow
                ShowMyMessage(sMsg);
            return false;
        }
    }
    ApplyHotAirByMOTemperatureMode();                                           //AI(ht9045-v899) 20260819: 換完工作檔後再依 MO Temperature mode 自動開關熱風槍
    SaveInfoFileWhenStart();//Jimmychiu 20211109 : (Upate)Handler 實際運轉Site資料討論  //Jimmychiu 20240110 : change to site close changing
    return true;
}
//---------------------------------------------------------------------------
void TfProductionInfo::OEE_EndLot()
{
    TDateTime dtTemp=_dtOEE_StartDateTime; //Start Lot時間
    _dtOEE_EndLotTime=Now();
    SaveSetupCondition();
    UpdateTrayCntReport();
    SaveProductionSummaryReport();
    SaveProductionDataToPath();
    InitialOEECount();
    _dtOEE_StartDateTime=dtTemp; //Start Lot時間
    _bOEEStartLotSuccess=false;
    bNeedLotEndAfterCleanOut=false;
    ESDForm->SetSaveESDControlMsgToFile("","");
    SaveSetting();
    sLocalMOFullFileName="";     //Sam 20190227 : Try Catch Mo Download SetupFile
}
//---------------------------------------------------------------------------
void TfProductionInfo::OEE_SetMO(AnsiString sMO)
{
    _sOEE_MO=sMO;
}
//---------------------------------------------------------------------------
AnsiString TfProductionInfo::OEE_GetMO()
{
    return _sOEE_MO;
}
//---------------------------------------------------------------------------
void TfProductionInfo::OEE_SetHandlerID(AnsiString sHDID)
{
    _sOEE_HandlerID=sHDID;
}
//---------------------------------------------------------------------------
void TfProductionInfo::SaveAndUpdateOEEFiles(bool bSaveNow)                     //Jimmychiu 20231023 : 非整點製作報表，時間準確到秒
{
    AnsiString sSourcesFilePath="";
    AnsiString sTargetFilePath="";
    AnsiString sFileName="";
    AnsiString sFullFileName="";
    AnsiString sResultOEEReport="";
    TStringList *slOEEReport=new TStringList();

    //Sam 20171018 : OEE Bug 修正 //Sam 20170801 HT-7045 變數改為 HT-9045 變數  //KaiChen 20171127 ：超豐 OEE 新增 Test Time、Index Time
    AnsiString sFileTitle = ",,Power Time(Sec),Run Time,Temp Time,Soak Time,Stop Time,Pause Time,Jam CNT,Service CNT,MTBA,MTBF,HOEE%,FLOW,Status,Jam Code,IN Q'ty,BIN1,BIN2,BIN3,BIN4,BIN5,BIN6,BIN7,BIN8,YIELD,Actural Out,plan out,MOEE%,OEEE%,M.O,T.T,Test Time,Index Time,Activity ID,Test Site";
    //Sam 20171018 : OEE Bug 修正   //KaiChen 20171127 ：超豐 OEE 新增 Test Time、Index Time
    AnsiString sArrayFileTitle[] = {"","","Power Time(Sec)","Run Time","Temp Time","Soak Time","Stop Time","Pause Time",
                                    "Jam CNT","Service CNT",
                                    "MTBA","MTBF","HOEE%",
                                    "FLOW","Status","Jam Code",
                                    "IN Q'ty","BIN1","BIN2","BIN3","BIN4","BIN5","BIN6","BIN7","BIN8",
                                    "YIELD","Actural Out","plan out",
                                    "MOEE%","OEEE%","M.O","T.T","Test Time","Index Time","Activity ID","Test Site"};
    try
    {
        LoadMOInformation(true);
        CalculateOEEReport(sResultOEEReport, bSaveNow);
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfProductionInfo::SaveAndUpdateOEEFiles");
    }
    //Save to Local
    if(_bOEEStartLotSuccess==true)
    {
        sSourcesFilePath=FileInfo().PathCombin(sProductionInfoFilePath, sLoadMO_MO);
        sSourcesFilePath=FileInfo().PathCombin(sSourcesFilePath, _sOEE_DirectoryName);
        sFileName=AnsiString().sprintf("%s_%s_OEE.csv",_sOEE_HandlerID,_sOEE_MO);
        sFullFileName=FileInfo().PathCombin(sSourcesFilePath, sFileName);
        MyForceDirectories(sSourcesFilePath, "TfProductionInfo::SaveAndUpdateOEEFiles_1");
        slOEEReport->Clear();
        if(FileExists(sFullFileName)==true)
        {
            slOEEReport->LoadFromFile(sFullFileName);
        }

        if(FileExists(sFullFileName)==false)
        {
            slOEEReport->Append(sFileTitle);
        }
        slOEEReport->Append(sResultOEEReport);
        slOEEReport->SaveToFile(sFullFileName);
    }

    //Save to FTP
    if(IniConfig.bN14_3_OEEFTPUpload==true)
    {
        if(bFTPError==true)
        {
            slOEEReport->Clear();
            delete slOEEReport;
            return;
        }

        sSourcesFilePath=sProductionInfoFilePath+"\\"+"Temp";                                      //JimmyChiu 20220111 sTargetFilePath=> sSourcesFilePath
        sFileName=_sOEE_HandlerID+"_"+sStartDateTime+".csv";
        sFullFileName=sSourcesFilePath+"\\"+sFileName;                                             //JimmyChiu 20220111 sTargetFilePath=> sSourcesFilePath

        MyForceDirectories(sSourcesFilePath, "TfProductionInfo::SaveAndUpdateOEEFiles_2");
        if(FileExists(sFullFileName)==true)
        {
            slOEEReport->Clear();
            delete slOEEReport;
            return;
        }

        //delete all file in Directory
        AnsiString sFindFile=sSourcesFilePath+"\\*.*";                                             //JimmyChiu 20220111 sTargetFilePath=> sSourcesFilePath
        TSearchRec srFile;
        if(FindFirst(sFindFile, 0, srFile)==0)
        {
            do
            {
                AnsiString sTemp=sSourcesFilePath+"\\"+srFile.Name;                                //JimmyChiu 20220111 sTargetFilePath=> sSourcesFilePath
                DeleteFile(sTemp.c_str());
            }
            while(FindNext(srFile)==0);
        }

        slOEEReport->Clear();
        slOEEReport->Append(sFileTitle);
        slOEEReport->Append(sResultOEEReport);
        slOEEReport->SaveToFile(sFullFileName);
        sTargetFilePath=IniConfig.asN14_3_OEEFTPUploadPath;
        CheckFTPFilePath(sTargetFilePath);
        sTargetFilePath=sTargetFilePath+_sOEE_HandlerID+"//";
        int iRes=K_SKIP;
        try
        {
            if(FTP_OEEFile_Upload(sSourcesFilePath, sTargetFilePath)==false)
            {
                iRes=ShowErrorMessage("WAR1685", K_RETRY|K_SKIP, MMSystem, 0, sFileName);//Upload %s.zip Fail, Retry??
            }
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TfProductionInfo::SaveAndUpdateOEEFiles");
            iRes=ShowErrorMessage("WAR1685", K_RETRY|K_SKIP, MMSystem, 0, sFileName);//Sam 20170801 HT-7045 報警改為 HT-9045 報警
        }

        if(iRes!=K_SKIP)
        {
            bCycleTimeUp=true;
        }
    }
    slOEEReport->Clear();
    delete slOEEReport;
}
//---------------------------------------------------------------------------
//取得CSV格式中的第幾筆資料
AnsiString TfProductionInfo::GetCSVLineData(int iDataNum, AnsiString sCSVLineStr)
{
    AnsiString sRes="";
    for(int i=0; i<iDataNum-1; i++ )
    {
        if(sCSVLineStr.Pos(",")>0)
        {
            sCSVLineStr=sCSVLineStr.SubString((sCSVLineStr.Pos(",")+1), (sCSVLineStr.Length()-sCSVLineStr.Pos(",")));
        }
        else
        {
            sCSVLineStr="";
        }
    }
    sRes=sCSVLineStr;
    if( sCSVLineStr.Pos(",")>0)
    {
        sRes=sCSVLineStr.SubString(1, sCSVLineStr.Pos(",")-1);
    }
    return sRes;
}
//---------------------------------------------------------------------------
void TfProductionInfo::SaveProductionDataToPath()
{
    //這邊要確認
    AnsiString sSourceFilePath="";
    AnsiString sFileName="";
    AnsiString sFullFileName="";
    TStringList *slOEEReport=new TStringList();

    //Save to Path
    if(IniConfig.bN14_2_OEEUseSaveProdData==false || IniConfig.asN14_2_OEESaveProdPath=="")
    {
        delete slOEEReport;
        return;
    }
    sFullFileName=IniConfig.asN14_2_OEESaveProdPath;
    #ifdef SOFT_SIMULTE
    sFullFileName="D:\\ProductionInfoBackup";
    #endif
    MyForceDirectories(sFullFileName, "TfProductionInfo::SaveProductionDataToPath_1");

    if(sFullFileName.SubString(sFullFileName.Length(),2)!="\\")
    {
        sFullFileName=sFullFileName+"\\"+sLoadMO_MO+"\\"+_sOEE_DirectoryName;
    }
    else
    {
        sFullFileName=sFullFileName+sLoadMO_MO+"\\"+_sOEE_DirectoryName;
    }
    MyForceDirectories(sFullFileName, "TfProductionInfo::SaveProductionDataToPath_2");

    //Save all file to Directory
    sSourceFilePath=sProductionInfoFilePath+"\\"+sLoadMO_MO+"\\"+_sOEE_DirectoryName;
    AnsiString sFindFile=sSourceFilePath+"\\*.*";
    TSearchRec srFile;
    if(FindFirst(sFindFile, 0, srFile)==0)
    {
        do
        {
            AnsiString sTemp=sSourceFilePath+"\\"+srFile.Name;
            slOEEReport->Clear();
            slOEEReport->LoadFromFile(sTemp);
            slOEEReport->SaveToFile(sFullFileName+"\\"+srFile.Name);
        }
        while(FindNext(srFile)==0);
    }
    delete slOEEReport;
}
//---------------------------------------------------------------------------
void TfProductionInfo::SetOEEReportMessage()
{
    _sOEE_FLOW=sLoadMO_TestFlow;
    //IN Q'ty
    int iNowINQty=LastSet.SendCT[0];
    if(iNowINQty<iLastINQty)
    {
        iLastINQty=LastSet.SendCT[0];
    }
    _iOEE_sINQty=iNowINQty-iLastINQty; //IN Q'ty
    iLastINQty=iNowINQty;
    int iActuralOut=0;
    int iNowBIN=0;
    std::vector<int> lsBin0_8=GetBin0_8List();
    //BIN0~8
    for(unsigned int i=0; i<lsBin0_8.size(); i++)                                                      //Sam 20171018 : OEE Bug 修正
    {
        iNowBIN=LastSet.BinCT[0][lsBin0_8[i]];                               //Sam 20171018 : OEE Bug 修正
        if(iNowBIN<iLastBIN[i])
        {
            iLastBIN[i]=LastSet.BinCT[0][lsBin0_8[i]];                      //Sam 20171018 : OEE Bug 修正
        }
        _iOEE_sBIN[i]=iNowBIN-iLastBIN[i];
        iLastBIN[i]=iNowBIN;
        iActuralOut=iActuralOut+iNowBIN;
    }
    //Actural Out
    if(iActuralOut<iLastActuralOut)
    {
        iLastActuralOut=iActuralOut;
    }
    _iOEE_ActuralOut=iActuralOut-iLastActuralOut; //Actural Out
    iLastActuralOut=iActuralOut;
}
//---------------------------------------------------------------------------
void TfProductionInfo::CalculateOEEPlanOut()
{
    static int iLastMaxOEEPlanOut=0;
    if(_bOEEStartLotSuccess==false)
    {
        _iOEE_planout=0;
        iLastMaxOEEPlanOut=0;
        RunInfo.TestTime="";                                                    //Sam 20170727先註解掉 系統測試時間 SYS_TestTime? (OK)
        return;
    }

    if(SystemStart==false)
    {
        return;
    }
    double fIndexTime=tIndexTimer.LatchCycleTime()*0.001;
    double fTestTime=atof(sLoadMO_TestTime.c_str()); //Mylin 20170515 Modify OEE Function

    if(fIndexTime>0)
    {
        if(fLastIndexTime>fIndexTime || fLastIndexTime==-1)
        {
            fLastIndexTime=fIndexTime;
        }
    }
    fLastIndexTime=0;
    fLastTestTime=fTestTime;
}
//---------------------------------------------------------------------------
void TfProductionInfo::CalculateOEEReport(AnsiString &sResultOEEReport,bool bSaveNow)//Jimmychiu 20231023 : 非整點製作報表，時間準確到秒
{
    AnsiString sOEEReportMsg = "";
    SetOEEReportMessage();
    _dtOEE_StartDateTime    =Now(); //Start Lot時間
    DateSeparator='/';  // Sam 20210225 : 增加保護
    AnsiString sStartDate   =_dtOEE_StartDateTime.FormatString("yyyy/mm/dd"); //Start Date
    AnsiString sStartTime   =_dtOEE_StartDateTime.FormatString("hh:nn:ss"); //Start Time
    if(bSaveNow==true)                                                          //Jimmychiu 20231023 : 非整點製作報表，時間準確到秒
    {
        sStartDateTime          =_dtOEE_StartDateTime.FormatString("YYYYMMDD_HHNNSS"); //Jimmychiu 20230824 : 符合Greatek資訊部抓資料條件，秒數必須為0
    }
    else
    {
        sStartDateTime          =_dtOEE_StartDateTime.FormatString("YYYYMMDD_HHNN00"); //Jimmychiu 20230824 : 符合Greatek資訊部抓資料條件，秒數必須為0
    }
    AnsiString sPowerTime   =IntToStr(_iOEE_PowerTime); //Power Time(Sec)
    AnsiString sRunTime     =IntToStr(_iOEE_RunTime-_iOEE_SoakTime); //Run Time
    AnsiString sTempTime    =IntToStr(0); //Temp Time
    AnsiString sSoakTime    =IntToStr(_iOEE_SoakTime); //Soak Time
    AnsiString sStopTime    =IntToStr(_iOEE_StopTime); //Stop Time
    AnsiString sPauseTime   =IntToStr(_iOEE_PauseTime);
    AnsiString sJamCNT      =IntToStr(_iOEE_JamCount); //Jam CNT
    AnsiString sServiceCNT  =IntToStr(_iOEE_ServiceCount); //Service CNT
    AnsiString sMTBA        =""; //MTBA
    if(sJamCNT<=0)
    {
        sMTBA=sRunTime;
    }
    else
    {
        if(_iOEE_JamCount>0)
        {
            sMTBA=IntToStr(_iOEE_RunTime/_iOEE_JamCount);
        }
        else
        {
            sMTBA="_iOEE_JamCount = 0 Error";
            sMTBA="0";
        }
    }

    AnsiString sMTBF=""; //MTBF
    if(_iOEE_ServiceCount<=0)
    {
        sMTBF=sRunTime;
    }
    else
    {
        if(_iOEE_ServiceCount>0)
        {
            sMTBF=IntToStr(_iOEE_RunTime/_iOEE_ServiceCount);
        }
        else
        {
            sMTBF="_iOEE_ServiceCount = 0 Error";
            sMTBF="0";
        }
    }

    AnsiString sHOEEPercent=""; //HOEE%
    if(_iOEE_PowerTime>0)
    {
        sHOEEPercent=ChangeToPercentage((double)_iOEE_RunTime-_iOEE_SoakTime, (double)_iOEE_PowerTime); //Steven 20250820 : 針對除以0加上保護
    }
    else
    {
        sHOEEPercent="_iOEE_PowerTime = 0 Error";
        sHOEEPercent="0.00%";
    }

    AnsiString sFLOW    =sLoadMO_TestFlow;
    AnsiString sStatus  =_sOEE_Status;
    AnsiString sJamCode =_sOEE_JamCode;
    AnsiString sINQty   =IntToStr(_iOEE_sINQty); //IN Q'ty
    AnsiString sBIN[8]; //Sam 20171018 : OEE Bug 修正
    int iPassBin=0;
    for(int i=0; i<8; i++)    //Sam 20171018 : OEE Bug 修正                     //QQQ
    {
        sBIN[i]=IntToStr(_iOEE_sBIN[i]);                                        //BIN1
        if(Prod.iIsPassT6[i]==1)                                                //Steven 20240105 : Prod.bIsPass --> Prod.iIsPassT6 //Steven 20240701 : 0 --> 1
        {
            iPassBin=iPassBin+_iOEE_sBIN[i];
        }
    }
    AnsiString sYIELD=""; //YIELD
    if(_iOEE_ActuralOut>0)
    {
        sYIELD=ChangeToPercentage((double)iPassBin, (double)_iOEE_ActuralOut);  //Steven 20250820 : 針對除以0加上保護
    }
    else
    {
        sYIELD="_iOEE_sINQty = 0 Error";
        sYIELD="0.00%";
    }
    AnsiString sActuralOut=IntToStr(_iOEE_ActuralOut); //Actural Out
    AnsiString splanout=""; //IntToStr(_iOEE_planout); //plan out
    if((fLastIndexTime+fLastTestTime)>0.0)
    {
        splanout=IntToStr((int)(ChangeToFloatNonPcnt((double)(atof(sPowerTime.c_str())), (double)(fLastTestTime))));       //Mylin 20170517 Modify OEE Clear Count Issue
    }
    else
    {
        splanout=sActuralOut;
    }
    _iOEE_planout=atoi(splanout.c_str());

    AnsiString sMOEEPercent=""; //MOEE%
    if(_iOEE_planout>0)
    {
        sMOEEPercent=ChangeToPercentage((double)_iOEE_ActuralOut, (double)_iOEE_planout*100.0);
        sMOEEPercent.sprintf("%0.2f%", atof(sMOEEPercent.c_str()));
    }
    else
    {
        sMOEEPercent="_iOEE_planout = 0 Error";
        sMOEEPercent="0.00%";
    }

    AnsiString sOEEEPercent=""; //OEEE%
    if(_iOEE_ActuralOut>0 && _iOEE_planout>0)
    {
        sOEEEPercent=FloatToStr((double)iPassBin/(double)_iOEE_ActuralOut*100.0*(double)_iOEE_ActuralOut/(double)_iOEE_planout);
        sOEEEPercent.sprintf("%0.2f%",atof(sOEEEPercent.c_str()));
    }
    else
    {
        sOEEEPercent="_iOEE_sINQty = 0 or _iOEE_planout = 0 Error";
        sOEEEPercent="0.00%";
    }
    AnsiString sMO=sLoadMO_MO;
    AnsiString sTestTime="0.00";
    if(atof(sLoadMO_TestTime.c_str())!=0)
    {
        sTestTime=sLoadMO_TestTime;
    }

    AnsiString sActivityID=_sOEE_ActivityID;
    int iSiteUse=0;
    for(int i=0; i<4; i++)                                                      //Sam 20170731 HT-7045 變數改為 HT-9045 變數
    {
        for(int j=0; j<8; j++)
        {
            if(LastSet.bUseTestSocket[0][i][j]==true)
            {
                iSiteUse++;
            }
        }
    }
    AnsiString sTestSite=IntToStr(iSiteUse);

    sOEEReportMsg  =sStartDate  +","+
                    sStartTime  +","+
                    sPowerTime  +","+
                    sRunTime    +","+
                    sTempTime   +","+
                    sSoakTime   +","+
                    sStopTime   +","+
                    sPauseTime  +","+
                    sJamCNT     +","+
                    sServiceCNT +","+
                    sMTBA       +","+
                    sMTBF       +","+
                    sHOEEPercent+","+
                    sFLOW       +","+
                    sStatus     +","+
                    sJamCode    +","+
                    sINQty      +",";

    for(int i=0; i<8; i++)    //Sam 20171018 : OEE Bug 修正
    {
        sOEEReportMsg=sOEEReportMsg+sBIN[i]+",";
    }

    //KaiChen 20171127 ：超豐 OEE 新增 Test Time、Index Time
    //==>
    AnsiString sTestRecevieTime =fObserver->sTestReceiveTime;
    AnsiString sIndexTime       =fObserver->sTestIndexZTime;
    //<==
    //KaiChen 20171127 ：超豐 OEE 新增 Test Time、Index Time

    if(bUseTwoArm32Site==true   &&
       sIndexTime!="")                                                          //Sam 20180802 (wei) : OEE 32Site 修正
    {
        sIndexTime=FloatToStr(StrToFloat(sIndexTime)+fObserver->dOEEIndexCycleTime);
    }

    sOEEReportMsg=sOEEReportMsg+sYIELD          +","+
                                sActuralOut     +","+
                                splanout        +","+
                                sMOEEPercent    +","+
                                sOEEEPercent    +","+
                                sMO             +","+
                                sTestTime       +","+
                                sTestRecevieTime+","+      //KaiChen 20171127 ：超豐 OEE 新增 Test Time、Index Time
                                sIndexTime      +","+      //KaiChen 20171127 ：超豐 OEE 新增 Test Time、Index Time
                                sActivityID     +","+
                                sTestSite;
    sResultOEEReport=sOEEReportMsg;
    ClearOEECount();
}
//---------------------------------------------------------------------------
void TfProductionInfo::InitialOEECount()
{
    _sOEE_DirectoryName ="";    //資料夾名稱 YYYYMMDD_HHMMSS_MO
    _dtOEE_StartLotTime =Now(); //Start Lot時間
    _dtOEE_EndLotTime   =Now(); //End Lot時間
    _iOEE_PowerTime     =0;     //Power On時間
    _iOEE_RunTime       =0;     //Run時間
    _iOEE_SoakTime      =0;     //Soak時間
    _iOEE_StopTime      =0;     //Stop時間
    _iOEE_PauseTime     =0;     //Mylin 20170705 Add OEE Report Field
    _iOEE_JamCount      =0;     //Jam次數
    _iOEE_ServiceCount  =0;     //Error次數
    _sOEE_MO            ="";    //此Lot的MO

    iLastINQty          =LastSet.SendCT[0];

    //BIN1~8
    for(int i=0; i<8; i++)                                                      //Sam 20171018 : OEE Bug 修正
    {
        iLastBIN[i]=0;
    }

    //Actural Out
    int iActuralOut=0;
    int iNowBIN=0;
    std::vector<int> lsBin0_8=GetBin0_8List();
    for(unsigned int i=0; i<lsBin0_8.size(); i++)                               //Sam 20171018 : OEE Bug 修正
    {
        iNowBIN=LastSet.BinCT[0][lsBin0_8[i]];                                  //Sam 20171018 : OEE Bug 修正
        iLastBIN[i]=iNowBIN;
        iActuralOut=iActuralOut+iNowBIN;
    }
    iLastActuralOut =iActuralOut;
    fLastIndexTime  =-1.0;
    fLastTestTime   =-1.0;

    sLoadMO_MO              ="";
    sLoadMO_TestTime        ="";
    sLoadMO_Temperature     ="";
    sLoadMO_SoakTime        ="";
    sLoadMO_TestFlow        ="";
    sLoadMO_PASSBIN         ="";
    sLoadMO_InterfaceType   ="";
    sLoadMO_Handler         ="";
    sLoadMO_Tester          ="";
    sLoadMO_Device          ="";
    sLoadMO_AUTOMOTIVE      ="";    //KaiChen 20171116 ：超豐 人員車規認證 (AUTOMOTIVE)

    _sOEE_Status    ="";
    _sOEE_JamCode   ="";
    _sOEE_ActivityID="";
    _sOEE_TestSite  ="";
    bPauseTimeNeedSaveAndUpdateOEEFiles=true;
    ClearOEECount();
}
//---------------------------------------------------------------------------
void TfProductionInfo::ClearOEECount()
{
    _dtOEE_StartDateTime=Now(); //Start Lot時間
    _iOEE_PowerTime     =0;
    _iOEE_RunTime       =0;
    _iOEE_SoakTime      =0; //Soak時間
    _iOEE_StopTime      =0;

    iLastINQty = LastSet.SendCT[0];
    //Actural Out
    int iActuralOut=0;
    int iNowBIN=0;
    std::vector<int> lsBin0_8=GetBin0_8List();
    for(int iBin=0; iBin<8; iBin++)                                                      //Sam 20171018 : OEE Bug 修正
    {
        iNowBIN=LastSet.BinCT[0][lsBin0_8[iBin]];                               //Sam 20171018 : OEE Bug 修正
        iLastBIN[iBin]=iNowBIN;
        iActuralOut=iActuralOut+iNowBIN;
    }
    iLastActuralOut=iActuralOut;

    //plan out
    iplanout            =0;
    _iOEE_JamCount      =0; //Jam次數
    _iOEE_ServiceCount  =0; //Error次數

    _sOEE_Status    ="";
    _sOEE_JamCode   ="";
    _sOEE_ActivityID="";
    _sOEE_TestSite  ="";
    _iOEE_PauseTime =0;
}
//---------------------------------------------------------------------------
void TfProductionInfo::TimeCount()
{
    if(IniConfig.bN14_1_EnableOEEFunction==false ||
       IniConfig.iN14_1_OEERecordCycleTime==0)
    {
        return;
    }
    static int iLastSecond=0;
    int iSecond=atoi(Now().FormatString("ss").c_str());
    iSecond=(iSecond+500)*1000;
    dtNowDateTime=Now();
    if(iSecond!=iLastSecond)
    {
        iLastSecond=iSecond;
        //Power On 開始時間
        if(SystemStart==true)
        {
            _iOEE_RunTime++;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfProductionInfo::tm_PITimer(TObject *Sender)
{
    if(CosFunction.bOEEFunction             ==false ||
       IniConfig.bN14_1_EnableOEEFunction   ==false ||  //Sam 20200306 : 增加功能開關保護。
       InitialOK                            ==false)
    {
        return;
    }
    static bool bIsRunning=false;//Jimmychiu 20220901 新增Timer運算中檢查
    if(bIsRunning==true)
        return;
    else
        bIsRunning=true;
    if(IniConfig.bN14_1_EnableOEEFunction && IniConfig.iN14_1_OEERecordCycleTime>0)
    {
        EachCycleSecondDo_SaveAndUpdateOEEFiles();
    }

    if(IniConfig.bN14_13_ULBinQtyToFTP && IniConfig.iN14_13_ULBinQtyInterval>0)
    {
        UploadBinQtyReport();
    }

    //Sam 20200525 : Control Bin
    //==>
    if(SystemStart)  //運作中才需要檢查
    {
        if(tControlBinTimer.Off() && iControlBinCheckPoint>0)
        {
            //Check Contol Bin
            bCheckControlBinYield();
            tControlBinTimer.LatchCycleTimeSec(true);
            iControlBinTimerCnt=iControlBinCheckPoint;
            NewRecordProcess("","New Start Control Bin Check",IntToStr(iControlBinTimerCnt)+"s");
            bIsControlBinCounting=true;
            tControlBinTimer.SetSecAndOn(iControlBinTimerCnt);
        }
        else if(iControlBinCheckCount>0)    //Sam 20201216 : Add 數量監控
        {
            if(bIsNeedCheckControlBin())
            {
                bCheckControlBinYield();//數量到了 CheckControl Bin
            }
        }
    }
    //<==
    //Sam 20200525 : Control Bin
    FTP_OEEFile_Upload();
    //Sam 20200305 : 增加 FTP 檔案檢查，檢查到 VIE_STOP.txt 就執行 OneCycle
    //==>
    try
    {
        if(tCheckFTPFileTimer.Off())    //Check File every 60 second
        {
            tCheckFTPFileTimer.SetSecAndOn(60);         //reset timer
            if(iHome==1 || SystemStart==0 || iOneCycle==1)
            {
                bIsRunning=false;
                return;
            }
            #ifndef SOFT_SIMULTE         //Jimmychiu 20230406 : 模擬時不執行
            bUseFTPOneCycle=false;
            if(DoCheckFTPFile())
            {
                fMain->BtnOneCycleClick(fMain);
                bUseFTPOneCycle=true;
            }
            #endif
        }
    }
    catch(...)
    {
        ShowMyMessage("DoCheckFTPFile try catch Error");
    }
    //<==
    //Sam 20200305 : 增加 FTP 檔案檢查，檢查到 VIE_STOP.txt 就執行 OneCycle
    bIsRunning=false;
}
//---------------------------------------------------------------------------
void TfProductionInfo::EachCycleSecondDo_SaveAndUpdateOEEFiles(bool bSaveNow)
{
    if(CosFunction.bOEEFunction==false || IniConfig.bN14_1_EnableOEEFunction==false || IniConfig.iN14_1_OEERecordCycleTime==0)
        return;
    static bool bIsRunning=false;
    static int iPauseTimeNeedSaveAndUpdateOEEFiles=0;
    if(bIsRunning==true)
        return;
    else
        bIsRunning=true;

    DecodeTime(Now(), h, n, s, z);
    int iHour   =h;
    int iNowDateTime=GetNowTimeSec();
    static int iLastDateTime=iNowDateTime;
    if(iLastDateTime!=iNowDateTime)
    {
        if(iNowDateTime<iLastDateTime)
            iNowDateTime+=86400;
        _iOEE_PowerTime+=(iNowDateTime-iLastDateTime);
        if(SystemStart==true && fContact->fShow==false)  //Sam 20170815 HT-7045 變數改為 HT-9045 變數
        {
            _iOEE_RunTime+=(iNowDateTime-iLastDateTime);
            if(iHeaterWaitTime!=0)
            {
                _iOEE_SoakTime+=(iNowDateTime-iLastDateTime);
            }
        }
        else
        {
            if(bIsPauseTime == true)
            {
                _iOEE_PauseTime+=(iNowDateTime-iLastDateTime);
            }
            else
            {
                _iOEE_StopTime+=(iNowDateTime-iLastDateTime);
            }
        }

        //Sam 2018227 Fix OEE Time Over 86400
        //==>
        CheckNewDayAndSubtract(_iOEE_PowerTime);    //JimmyChiu 20220115 跨日後重頭計算時間
        CheckNewDayAndSubtract(_iOEE_RunTime);      //JimmyChiu 20220115 跨日後重頭計算時間
        CheckNewDayAndSubtract(_iOEE_StopTime);     //JimmyChiu 20220115 跨日後重頭計算時間
        CheckNewDayAndSubtract(_iOEE_SoakTime);     //JimmyChiu 20220115 跨日後重頭計算時間
        CheckNewDayAndSubtract(_iOEE_PauseTime);    //JimmyChiu 20220115 跨日後重頭計算時間
        //<==
        //Sam 2018227 Fix OEE Time Over 86400

        if(bPauseTimeNeedSaveAndUpdateOEEFiles==false)
        {
            iPauseTimeNeedSaveAndUpdateOEEFiles+=(iNowDateTime-iLastDateTime);
            if(iPauseTimeNeedSaveAndUpdateOEEFiles>IniConfig.iN14_5_PauseIntervalTime)
            {
                bPauseTimeNeedSaveAndUpdateOEEFiles=true;
                iPauseTimeNeedSaveAndUpdateOEEFiles=0;
            }
        }
        iLastDateTime=iNowDateTime;
        CheckNewDayAndSubtract(iLastDateTime);
    }
    CalculateOEEPlanOut();

    if( (iHour==0 && iNextDateTime>=(24*60*60) ||
        (iNextDateTime<=iNowDateTime)
    ))
    {
        SetNextRecordDateTime();
        bCycleTimeUp=true;
    }

    if(bCycleTimeUp==true || bSaveNow==true)
    {
        AnsiString sStatusTemp      =_sOEE_Status;
        AnsiString sJamCodeTemp     =_sOEE_JamCode;
        AnsiString sActivityIDTemp  =_sOEE_ActivityID;

        bCycleTimeUp=false;

        #ifndef SOFT_SIMULTE         //Jimmychiu 20230406 : 模擬時不執行
        SaveAndUpdateOEEFiles(bSaveNow);
        #endif
        if(bSaveNow==false)
        {
            _sOEE_Status    =sStatusTemp;
            _sOEE_JamCode   =sJamCodeTemp;
            _sOEE_ActivityID=sActivityIDTemp;
        }

        #ifndef SOFT_SIMULTE         //Jimmychiu 20230406 : 模擬時不執行
        if(SystemStart==true)
        {
            if(CheckMOInformation()==false)
            {
                AnsiString sMsg="Press Start Fail!#Check Load MO Information Error";
                bStartNeedSaveAndUpdateOEEFiles=true;
                ShowMyMessage(sMsg);
            }
        }
        #endif
    }
    bIsRunning=false;
}
//---------------------------------------------------------------------------
void TfProductionInfo::SetNextRecordDateTime()
{
    int iRecordCycleTime=IniConfig.iN14_1_OEERecordCycleTime;
    iNextDateTime=GetNowTimeSec();
    if(IniConfig.iN14_1_OEERecordCycleTime!=0)
    {
        iNextDateTime=iNextDateTime-iNextDateTime%iRecordCycleTime+iRecordCycleTime;
    }
}
//---------------------------------------------------------------------------
void TfProductionInfo::SaveMessageHistroy(AnsiString sAlarmCode,AnsiString sAlarmMessage,int iType,int iSkipLevel)
{
    AnsiString sFilePath    ="";
    AnsiString sFileName    ="";
    AnsiString sFullFileName="";
    AnsiString sMsgString   ="";
    AnsiString sSaveTime    ="";
    AnsiString sFileTitle   ="AlarmCode,AlarmDate,AlarmTime,ErrMessage,AlarmType,SkipLevel";
    TStringList *slMsgHistroy=new TStringList();

    //Sam 20190128 : 檢查資料夾並自動產生
    MyForceDirectories(sProductionInfoFilePath, "TfProductionInfo::SaveMessageHistroy_1");
    sFilePath       =sProductionInfoFilePath+"\\"+sLoadMO_MO+"\\"+_sOEE_DirectoryName; //Mylin 20170515 Modify OEE Function
    sFileName       =_sOEE_MO+"_History.csv"; //Handler ID_MO_OEE.csv
    sFullFileName   =sFilePath+"\\"+sFileName;

    MyForceDirectories(sFilePath, "TfProductionInfo::SaveMessageHistroy_2");
    slMsgHistroy->Clear();

    if(FileExists(sFullFileName)==true)
    {
        slMsgHistroy->LoadFromFile(sFullFileName);
    }
    else
    {
        slMsgHistroy->Append(sFileTitle);
    }

    sSaveTime.sprintf("%04d/%02d/%02d,%02d:%02d:%02d",SystemYear,SystemMonth,SystemDate,SystemHour,SystemMin,SystemSec);
    sMsgString =sAlarmCode          +","+
                sSaveTime           +","+
                sAlarmMessage       +","+
                AnsiString(iType)   +","+
                AnsiString(iSkipLevel);
    slMsgHistroy->Append(sMsgString);
    slMsgHistroy->SaveToFile(sFullFileName);
    delete slMsgHistroy;
}
//---------------------------------------------------------------------------
bool TfProductionInfo::IsOEEStartLotSuccess()
{
    return _bOEEStartLotSuccess;
}
//---------------------------------------------------------------------------
void TfProductionInfo::SaveSetupCondition(AnsiString sSaveFileName)
{
    AnsiString sFilePath    ="";
    AnsiString sFileName    ="";
    AnsiString sFullFileName="";
    AnsiString sMsgString   ="";
    AnsiString TrayName[3];
    TStringList *slSetupCondition=new TStringList();

    sFilePath       =sProductionInfoFilePath+"\\"+sLoadMO_MO+"\\"+_sOEE_DirectoryName;
    if(sSaveFileName!="")
        sFileName   =sSaveFileName;
    else
        sFileName   =_sOEE_MO+"_SetupCondition.csv";

    sFullFileName   =sFilePath+"\\"+sFileName;

    MyForceDirectories(sFilePath, "TfProductionInfo::SaveSetupCondition");
    slSetupCondition->Clear();
    slSetupCondition->Add("***********[Temperature]***********,***[Value]***");
    sMsgString = "Temperature Mode,Ambient";
    if(LastSet.iTemperature==1)
    {
        sMsgString="Temperature Mode,Hot";
    }
    slSetupCondition->Append(sMsgString);

    sMsgString="Temperature Deg,NULL";
    if(LastSet.iTemperature!=0)
    {
        sMsgString="Temperature Deg,"+FloatToStr(Temperature.fWorkTemperBase);//Sam 20170801 HT-7045 變數改為 HT-9045 變數
    }
    slSetupCondition->Append(sMsgString);

    sMsgString="Soak Time,NULL";
    if(LastSet.iTemperature!=0)
    {
        sMsgString="Soak Time,"+FloatToStr(Temperature.fSoakTime);            //Sam 20170801 HT-7045 變數改為 HT-9045 變數
    }
    slSetupCondition->Append(sMsgString);

    AnsiString sSiteModeMap=fMain->WriteSiteMapData();
    int iPosMapStart    =0;
    iPosMapStart        =sSiteModeMap.Pos("-");
    AnsiString sSiteMode=sSiteModeMap.SubString(1,iPosMapStart-1);
    AnsiString sSiteMap =sSiteModeMap.SubString(iPosMapStart+1,sSiteModeMap.Length()-iPosMapStart-3);
    AnsiString sSiteUse ="";
    for(int i=0; i<MAX_SOCKET_ROW; i++)
    {
        for(int j=0; j<MAX_SOCKET_COL; j++)
        {
            if(LastSet.bUseTestSocket[0][i][j]==true)
            {
                sSiteUse=sSiteUse+"O";
            }
            else
            {
                sSiteUse=sSiteUse+"X";
            }
        }
    }
    slSetupCondition->Add("\r\n***********[Site Mode && Site Map]***********,***[Value]***");
    sMsgString="Site Mode,"+sSiteMode;
    slSetupCondition->Append(sMsgString);
    sMsgString="Site Map,"+sSiteMap;
    slSetupCondition->Append(sMsgString);
    sMsgString="Site Use,"+sSiteUse;
    slSetupCondition->Append(sMsgString);

    slSetupCondition->Add("\r\n***********[Bin]***********,***[Value]***");
    for(int i=0; i<iFixRight; i++)
    {
        AnsiString sPassFail="Error Bin";
        if(fShowBinSelect->MyBinSel[i]->Font->Color==clGreen)
        {
            sPassFail="Pass Bin";
        }
        else if(fShowBinSelect->MyBinSel[i]->Font->Color==clRed)
        {
            sPassFail="Fail Bin";
        }

        sMsgString=fShowBinSelect->MyBinSelLab[i]->Caption+","+sPassFail+"-"+fShowBinSelect->MyBinSel[i]->Caption;
        slSetupCondition->Append(sMsgString);
    }
    sMsgString="Run Mode,"+_sOEE_FLOW;
    slSetupCondition->Append(sMsgString);
    slSetupCondition->Add("\r\n***********[Vacuum&&Purge]***********,***[Value]***");
    AnsiString sVacuumArray[]={"InArm","IndexArm","OutArm","TrayArm"};
    int iVacuumArray=(sizeof(sVacuumArray) / sizeof(AnsiString));          //Sam 20201216 : 修正記憶體溢位 //Sam 20170731 自動算陣列長度
    for(int i=0; i<iVacuumArray; i++)
    {
        sMsgString="Vacuum "+sVacuumArray[i]+" Vacuum Check Wait,"+FloatToStr(ArmSpeed[i].dVacuumTI);
        slSetupCondition->Add(sMsgString);
        sMsgString="Vacuum "+sVacuumArray[i]+" Counter Air On Time,"+FloatToStr(ArmSpeed[i].dCTAirOn);
        slSetupCondition->Add(sMsgString);
    }
    sMsgString="Vacuum Input Arm Vacuum ON timing is after hand down,"+MyBoolToString(fSpeed->chkInArmPreSuck->Checked);
    slSetupCondition->Add(sMsgString);
    sMsgString="Vacuum Index Arm Vacuum ON timing is after hand down,"+MyBoolToString(fSpeed->chkIndexPreSuck->Checked);
    slSetupCondition->Add(sMsgString);
    sMsgString="Vacuum Output Arm Vacuum ON timing is after hand down,"+MyBoolToString(fSpeed->chkOutArmPreSuck->Checked);
    slSetupCondition->Add(sMsgString);

    slSetupCondition->Add("\r\n***********[Contact]***********,***[Value]***");
    sMsgString="Contact Torque Max,"+fContact->edtTorqueMax->Text;
    slSetupCondition->Add(sMsgString);
    sMsgString="Contact Torque Compare,"+fContact->edtTorqueCmp->Text;
    slSetupCondition->Add(sMsgString);
    sMsgString="Contact Test socket IC check Type"+fContact->coD41->Text;
    slSetupCondition->Add(sMsgString);
    sMsgString="Contact Test socket IC check For mm,"+fContact->edD41->Text;
    slSetupCondition->Add(sMsgString);

    sMsgString="Contact Release height Arm1,"+fContact->edReleaseHeight1->Text;
    slSetupCondition->Add(sMsgString);
    sMsgString="Contact Pick up  height Arm1,"+fContact->edPickUp1->Text;
    slSetupCondition->Add(sMsgString);
    sMsgString="Contact Org pick up Arm1,"+fContact->edOrgPick1->Text;
    slSetupCondition->Add(sMsgString);
    sMsgString="Contact Shuttle pick offset Arm1,"+fContact->edShtPickOffset1->Text;
    slSetupCondition->Add(sMsgString);
    sMsgString="Contact Drop offSet Arm1,"+fContact->edDropOffset1->Text;
    slSetupCondition->Add(sMsgString);
    sMsgString="Contact Height Test Arm1,"+fContact->edContactHeight1->Text;
    slSetupCondition->Add(sMsgString);
    sMsgString="Contact Org height Arm1,"+fContact->edContactBackUp1->Text;
    slSetupCondition->Add(sMsgString);
    sMsgString="Contact Contact offset Arm1,"+fContact->edContactOffsetArm1->Text;
    slSetupCondition->Add(sMsgString);

    sMsgString="Contact Release height Arm2,"+fContact->edReleaseHeight2->Text;
    slSetupCondition->Add(sMsgString);
    sMsgString="Contact Pick up  height Arm2,"+fContact->edPickUp2->Text;
    slSetupCondition->Add(sMsgString);
    sMsgString="Contact Org pick up Arm2,"+fContact->edOrgPick2->Text;
    slSetupCondition->Add(sMsgString);
    sMsgString="Contact Shuttle pick offset Arm2,"+fContact->edShtPickOffset2->Text;
    slSetupCondition->Add(sMsgString);
    sMsgString="Contact Drop offSet Arm2,"+fContact->edDropOffset2->Text;
    slSetupCondition->Add(sMsgString);
    sMsgString="Contact Height Test Arm2,"+fContact->edContactHeight2->Text;
    slSetupCondition->Add(sMsgString);
    sMsgString="Contact Org height Arm2,"+fContact->edContactBackUp2->Text;
    slSetupCondition->Add(sMsgString);
    sMsgString="Contact Contact offset Arm2,"+fContact->edContactOffsetArm2->Text;
    slSetupCondition->Add(sMsgString);

    sMsgString="Contact Arm Cylinder Force For KG,"+fContact->edAirForce->Text;
    slSetupCondition->Add(sMsgString);
    sMsgString="Contact The No. Of Pins(Balls),"+fContact->edPinCount->Text;
    slSetupCondition->Add(sMsgString);
    sMsgString="Contact Force Per Device For N,"+fContact->edForcePerDeviceKG->Text;
    slSetupCondition->Add(sMsgString);
    sMsgString="Contact Force Per Device For KG,"+fContact->edForcePerDeviceN->Text;
    slSetupCondition->Add(sMsgString);
    sMsgString="Contact Force Per Pin For N,"+fContact->edForcePerPinN->Text;
    slSetupCondition->Add(sMsgString);
    sMsgString="Contact Force Per Pin For GF,"+fContact->edForcePerPinG->Text;

    slSetupCondition->Add(sMsgString);

    slSetupCondition->Add("\r\n***********[Tray Form]***********,***[Value]***");
    TrayName[0]=fTrayForm->TrayName1->Text;
    TrayName[1]=fTrayForm->TrayName2->Text;
    TrayName[2]=fTrayForm->TrayName3->Text;
    sMsgString="TrayForm Loader Tray Name,"+TrayName[fTrayAssignment->cbLoader->ItemIndex];
    slSetupCondition->Add(sMsgString);
    sMsgString="TrayForm Auto1 Tray Name,"+TrayName[0];
    slSetupCondition->Add(sMsgString);
    sMsgString="TrayForm Auto2 Tray Name,"+TrayName[0];
    slSetupCondition->Add(sMsgString);
    sMsgString="TrayForm Auto3 Tray Name,"+TrayName[0];
    slSetupCondition->Add(sMsgString);
    sMsgString="TrayForm Fix1 Tray Name,"+TrayName[fTrayAssignment->cbFix1->ItemIndex];
    slSetupCondition->Add(sMsgString);
    sMsgString="TrayForm Fix2 Tray Name,"+TrayName[fTrayAssignment->cbFix2->ItemIndex];
    slSetupCondition->Add(sMsgString);
    sMsgString="TrayForm Fix3 Tray Name,"+TrayName[fTrayAssignment->cbFix3->ItemIndex];
    slSetupCondition->Add(sMsgString);
    sMsgString="TrayForm Color Tray Name,"+TrayName[fTrayAssignment->cbColor->ItemIndex];
    slSetupCondition->Add(sMsgString);
    sMsgString="TrayForm Empty Tray Name,"+TrayName[fTrayAssignment->cbEmpty->ItemIndex];
    slSetupCondition->Add(sMsgString);

    slSetupCondition->Add("\r\n***********[Plate Form]***********,***[Value]***");
    sMsgString="PlateForm Hot Plate1 Select,"+MyBoolToString(fHotPlate->cbEnableHP1->Checked);
    slSetupCondition->Add(sMsgString);
    sMsgString="PlateForm Hot Plate2 Select,"+MyBoolToString(fHotPlate->cbEnableHP2->Checked);
    slSetupCondition->Add(sMsgString);

    slSetupCondition->Add("\r\n***********[Temp. Offset]***********,***[Value]***");
    sMsgString="Temp High Base,"+FloatToStr(Temperature.fHighBase);
    slSetupCondition->Add(sMsgString);
    sMsgString="Temp Low Base,"+FloatToStr(Temperature.fLowBase);
    slSetupCondition->Add(sMsgString);
    AnsiString sTempArray[]={"HotPlate1","HotPlate2","Shuttle1","Shuttle2","Head1","Head2","Head3","Head4","Socket","Chamber"};
    int iTempArray=(sizeof(sTempArray) / sizeof(AnsiString));          //Sam 20201216 : 修正記憶體溢位 //Sam 20170731 自動算陣列長度
    for(int i=0; i<iTempArray; i++)
    {
        sMsgString="Temp "+sTempArray[i]+" High Base,"+FloatToStr(Temperature.fTempOffSet[HigBase][i]);
        slSetupCondition->Add(sMsgString);
        sMsgString="Temp "+sTempArray[i]+" Low Base,"+FloatToStr(Temperature.fTempOffSet[LowBase][i]);
        slSetupCondition->Add(sMsgString);
    }

    if(Temperature.iMachineTempMode==0)
        sMsgString="Temp_Temperature Mode,Ambient Mode";
    else if(Temperature.iMachineTempMode==1)
        sMsgString="Temp_Temperature Mode,Hot Mode";
    else if(Temperature.iMachineTempMode==3)
        sMsgString="Temp_Temperature Mode,Ambient Hot Mode";
    slSetupCondition->Add(sMsgString);

    slSetupCondition->Add("\r\n***********[Yield Form]***********,***[Value]***");
    sMsgString="Yields Normal Low Yields Select,"                             +MyBoolToString(TestIF_File.bFailAlarmLowYield);
    slSetupCondition->Add(sMsgString);
    sMsgString="Yields Normal Low Yields,"                                    +FloatToStr(TestIF_File.dLowYieldLimit);
    slSetupCondition->Add(sMsgString);
    sMsgString="Yields Normal Low Yields After Count,"                        +IntToStr(TestIF_File.iLowYieldCount);
    slSetupCondition->Add(sMsgString);
    sMsgString="Yields Normal By Arm Per Site Differ Yield Select,"           +MyBoolToString(TestIF_File.bFailAlarmSiteYieldDifferent);
    slSetupCondition->Add(sMsgString);
    sMsgString="Yields Normal By Arm Per Site Differ Yield,"                  +FloatToStr(TestIF_File.dFailAlarmSiteYield);
    slSetupCondition->Add(sMsgString);
    sMsgString="Yields Normal By Arm Per Site Differ Yield After Count,"      +IntToStr(TestIF_File.iFailAlarmSiteYield);
    slSetupCondition->Add(sMsgString);
    sMsgString="Yields Normal Consecutive Failure Alarm Select( Socket ),"    +MyBoolToString(TestIF_File.bContsFailBySocket);
    slSetupCondition->Add(sMsgString);
    sMsgString="Yields Normal Consecutive Failure Alarm Count( Socket ),"     +IntToStr(TestIF_File.iContsFailSocketAlarmCT);
    slSetupCondition->Add(sMsgString);
    sMsgString="Yields Normal Consecutive Failure Alarm Select( Head ),"      +MyBoolToString(TestIF_File.bContsFailByHead);
    slSetupCondition->Add(sMsgString);
    sMsgString="Yields Normal Consecutive Failure Alarm Count( Head ),"       +IntToStr(TestIF_File.iContsFailHeadAlarmCT);
    slSetupCondition->Add(sMsgString);

    sMsgString="Yields Re-Test Low Yields Select,"                            +MyBoolToString(TestIF_File.bFailAlarmLowYield_RT);
    slSetupCondition->Add(sMsgString);
    sMsgString="Yields Re-Test Low Low Yields,"                               +FloatToStr(TestIF_File.dLowYieldLimit_RT);
    slSetupCondition->Add(sMsgString);
    sMsgString="Yields Re-Test Low Low Yields After Count,"                   +IntToStr(TestIF_File.iLowYieldCount_RT);
    slSetupCondition->Add(sMsgString);
    sMsgString="Yields Re-Test By Arm Per Site Differ Yield Select,"          +MyBoolToString(TestIF_File.bFailAlarmSiteYieldDifferent_RT);
    slSetupCondition->Add(sMsgString);
    sMsgString="Yields Re-Test By Arm Per Site Differ Yield,"                 +FloatToStr(TestIF_File.dFailAlarmSiteYield_RT);
    slSetupCondition->Add(sMsgString);
    sMsgString="Yields Re-Test By Arm Per Site Differ Yield After Count,"     +IntToStr(TestIF_File.iFailAlarmSiteYield_RT);
    slSetupCondition->Add(sMsgString);
    sMsgString="Yields Re-Test Consecutive Failure Alarm Select( Socket ),"   +MyBoolToString(TestIF_File.bContsFailBySocket_RT);
    slSetupCondition->Add(sMsgString);
    sMsgString="Yields Re-Test Consecutive Failure Alarm Count( Socket ),"    +IntToStr(TestIF_File.iContsFailSocketAlarmCT_RT);
    slSetupCondition->Add(sMsgString);
    sMsgString="Yields Re-Test Consecutive Failure Alarm Select( Head ),"     +MyBoolToString(TestIF_File.bContsFailByHead_RT);
    slSetupCondition->Add(sMsgString);
    sMsgString="Yields Re-Test Consecutive Failure Alarm Count( Head ),"      +IntToStr(TestIF_File.iContsFailHeadAlarmCT_RT);
    slSetupCondition->Add(sMsgString);

    sMsgString="Yields Alarm4 Continue Type Select,"                          +MyBoolToString(TestIF_File.bAlarm4ContinueType_Enable);
    slSetupCondition->Add(sMsgString);
    sMsgString="Yields Alarm4 Continue Type Interval Count,"                  +IntToStr(TestIF_File.iAlarm4ContinueType_IntervalCount);
    slSetupCondition->Add(sMsgString);
    sMsgString="Yields Alarm4 Continue Type Continue Count,"                  +IntToStr(TestIF_File.iAlarm4ContinueType_ContinueCount);
    slSetupCondition->Add(sMsgString);
    sMsgString="Yields Alarm4 Interval Total Yield Difference Select,"        +MyBoolToString(TestIF_File.bAlarm4EnableIntervalYield);
    slSetupCondition->Add(sMsgString);
    sMsgString="Yields Alarm4 Interval Total Yield Difference,"               +IntToStr(TestIF_File.iAlarm4IntervalYieldYield);
    slSetupCondition->Add(sMsgString);
    sMsgString="Yields Alarm4 Interval Total Yield Difference Interval Count,"+IntToStr(TestIF_File.iAlarm4IntervalYieldIntervalCount);
    slSetupCondition->Add(sMsgString);
    sMsgString="Yields Alarm4 Interval Total Yield Difference Continue Count,"+IntToStr(TestIF_File.iAlarm4IntervalYieldContinueCount);
    slSetupCondition->Add(sMsgString);
    sMsgString="Yields Alarm4 Site To Site Yield Select,"                     +MyBoolToString(TestIF_File.bSiteToSiteYieldCmp);
    slSetupCondition->Add(sMsgString);
    sMsgString="Yields Alarm4 Site To Site Yield,"                            +IntToStr(TestIF_File.iSiteToSiteYieldCmp);
    slSetupCondition->Add(sMsgString);
    sMsgString="Yields Alarm4 Site To Site Yield Count,"                      +IntToStr(TestIF_File.iSiteToSiteYieldCmpCount);
    slSetupCondition->Add(sMsgString);
    sMsgString="Yields Alarm4 Head To Head Yield Select,"                     +MyBoolToString(TestIF_File.bHeadToHeadYieldCmp);
    slSetupCondition->Add(sMsgString);
    sMsgString="Yields Alarm4 Head To Head Yield,"                            +IntToStr(TestIF_File.iHeadToHeadYieldCmp);
    slSetupCondition->Add(sMsgString);
    sMsgString="Yields Alarm4 Head To Head Yield Count,"                      +IntToStr(TestIF_File.iHeadToHeadYieldCmpCount);
    slSetupCondition->Add(sMsgString);
    sMsgString="Yields Alarm4 Site/Head Yield Over Alert Select,"             + MyBoolToString(TestIF_File.bSiteYieldOverAlert);
    slSetupCondition->Add(sMsgString);
    sMsgString="Yields Alarm4 Site/Head Yield Over Alert,"                    +IntToStr(TestIF_File.iSiteYieldOverAlert);
    slSetupCondition->Add(sMsgString);
    sMsgString="Yields Alarm4 Site/Head Yield Over Alert Count,"              +IntToStr(TestIF_File.iSiteYieldOverAlertCount);
    slSetupCondition->Add(sMsgString);

    slSetupCondition->Add("\r\n***********[Tester]***********,***[Value]***");
    if(TestIF.iTestType==GPIB_MODE)
        sMsgString="Tester Interface for Connection Type,GPIB_MODE";
    else if(TestIF.iTestType==TTL_MODE)
        sMsgString="Tester Interface for Connection Type,TTL_MODE";
    else if(TestIF.iTestType==RS232_MODE)
        sMsgString = "Tester Interface for Connection Type,RS232_MODE";
    slSetupCondition->Add(sMsgString);
    sMsgString="Tester TTL Setting Type,"         +FTestIF->cbDIOType->Text;
    slSetupCondition->Add(sMsgString);
    sMsgString="Tester TTL Anti-Signal Select,"   +MyBoolToString(TestIF.bAntiSignal);
    slSetupCondition->Add(sMsgString);
    sMsgString="Tester GPIB Setting Type,"        +FTestIF->cbGPIBType->Text;
    slSetupCondition->Add(sMsgString);
    sMsgString="Tester Initial Max Time,"         +FloatToStr(TestIF_File.iInitialMaxTime);
    slSetupCondition->Add(sMsgString);
    sMsgString="Tester Wait Max Time,"            +FloatToStr(TestIF_File.iMaxTime);
    slSetupCondition->Add(sMsgString);
    sMsgString="Tester Initial Start Delay Time," +FloatToStr(TestIF_File.dStartDelayTime);
    slSetupCondition->Add(sMsgString);
    sMsgString="Tester Start Signal Delay Time1," +FloatToStr(TestIF_File.dInitStartDelayTime);
    slSetupCondition->Add(sMsgString);
    sMsgString = "Tester Start Signal Delay Time2," + FloatToStr(TestIF_File.dInitialStartDelayDec[0]); //kevin 20180307 add
    slSetupCondition->Add(sMsgString);
    sMsgString = "Tester Start Signal Delay Time3," + FloatToStr(TestIF_File.dInitialStartDelayDec[1]); //kevin 20180307 add
    slSetupCondition->Add(sMsgString);
    sMsgString = "Tester Start Signal Delay Time4," + FloatToStr(TestIF_File.dInitialStartDelayDec[2]); //kevin 20180307 add
    slSetupCondition->Add(sMsgString);
    sMsgString = "Tester Start Signal Delay Time5," + FloatToStr(TestIF_File.dInitialStartDelayDec[3]); //kevin 20180307 add
    slSetupCondition->Add(sMsgString);

    slSetupCondition->Add("\r\n***********[Auto Clean]***********,***[Value]***");
    sMsgString="AutoClean Cleaning Switch,"                       +IntToStr(TestIF_File.iAutoClean_Function);
    slSetupCondition->Add(sMsgString);
    sMsgString="AutoClean Cleaning Mode initial Start,"           +MyBoolToString(TestIF_File.iAutoClean_Mode & M_INIT_START);
    slSetupCondition->Add(sMsgString);
    sMsgString="AutoClean Cleaning Mode Initial Retest Start,"    +MyBoolToString(TestIF_File.iAutoClean_Mode & M_INIT_RESTART);
    slSetupCondition->Add(sMsgString);
    sMsgString="AutoClean Cleaning Mode Finish,"                  +MyBoolToString(TestIF_File.iAutoClean_Mode & M_FINISH);
    slSetupCondition->Add(sMsgString);
    sMsgString="AutoClean Cleaning Mode Manual,"                  +MyBoolToString(TestIF_File.iAutoClean_Mode & M_MANUAL);
    slSetupCondition->Add(sMsgString);
    sMsgString="AutoClean Cleaning Mode Socket Alarm Fail,"       +MyBoolToString(TestIF_File.iAutoClean_Mode & M_SOCKET_ALARM);
    slSetupCondition->Add(sMsgString);
    sMsgString="AutoClean Cleaning Mode Interval Contact,"        +MyBoolToString(TestIF_File.iAutoClean_Mode & M_INTERVAL);
    slSetupCondition->Add(sMsgString);
    sMsgString="AutoClean Cleaning Mode Interval Contact Count,"  +IntToStr(TestIF_File.iAutoClean_IntervalContact);
    slSetupCondition->Add(sMsgString);
    sMsgString="AutoClean Cleaning Alarm Count,"                  +IntToStr(TestIF_File.iAutoClean_AlarmCount);
    slSetupCondition->Add(sMsgString);
    sMsgString="AutoClean Contact Time,"                          +FloatToStr((double)TestIF_File.iAutoClean_ContactTime/10.0);
    slSetupCondition->Add(sMsgString);
    sMsgString="AutoClean Contact Count,"                         +IntToStr(TestIF_File.iAutoClean_ContactCount);
    slSetupCondition->Add(sMsgString);
    sMsgString="AutoClean OutArm Speed,"                          +IntToStr(TestIF_File.iAutoClean_MotorSpeed[0]);
    slSetupCondition->Add(sMsgString);
    sMsgString="AutoClean Shuttle Speed,"                         +IntToStr(TestIF_File.iAutoClean_MotorSpeed[1]);
    slSetupCondition->Add(sMsgString);
    sMsgString="AutoClean IndexArm Speed,"                        +IntToStr(TestIF_File.iAutoClean_MotorSpeed[2]);
    slSetupCondition->Add(sMsgString);
    sMsgString="AutoClean InArm Z Speed,"                         +IntToStr(TestIF_File.iAutoClean_MotorSpeed[3]);
    slSetupCondition->Add(sMsgString);

    slSetupCondition->Add("\r\n***********[Speed]***********,***[Value]***");
    sMsgString="InArm Speed,"                                     +IntToStr(ArmSpeed_File[InArm].iBodySP);
    slSetupCondition->Add(sMsgString);
    sMsgString="InArm Z Speed,"                                   +IntToStr(ArmSpeed_File[InArm].iZSP);
    slSetupCondition->Add(sMsgString);
    sMsgString="IndexArm Speed,"                                  +IntToStr(ArmSpeed_File[IndexArm].iBodySP);
    slSetupCondition->Add(sMsgString);
    sMsgString="IndexArm Z Speed,"                                +IntToStr(ArmSpeed_File[IndexArm].iZSP);
    slSetupCondition->Add(sMsgString);
    sMsgString="OutArm Speed,"                                    +IntToStr(ArmSpeed_File[OutArm].iBodySP);
    slSetupCondition->Add(sMsgString);
    sMsgString="OutArm Z Speed,"                                  +IntToStr(ArmSpeed_File[OutArm].iZSP);
    slSetupCondition->Add(sMsgString);
    sMsgString="TrayArm Speed,"                                   +IntToStr(ArmSpeed_File[TrayArm].iBodySP);
    slSetupCondition->Add(sMsgString);

    slSetupCondition->SaveToFile(sFullFileName);

    delete slSetupCondition;
}
//---------------------------------------------------------------------------
void TfProductionInfo::UploadSetupCondition()
{
    if(IniConfig.bN14_8_ULSetup==false ||
       IniConfig.asN14_8_ULSetupPath=="")
    {
        return;
    }

    AnsiString sSaveFilePath=sProductionInfoFilePath+"\\"+sLoadMO_MO+"\\"+_sOEE_DirectoryName;
    AnsiString sSaveFullFileName;
    AnsiString sSaveFileName;
    sSaveFileName.sprintf("%s_%s_%s.csv", IniConfig.SocketHandlerID, sLoadMO_MO, Now().FormatString("yyyymmdd_hhnnss"));
    sSaveFullFileName.sprintf("%s\\%s", sSaveFilePath, sSaveFileName);
    //delete all file in Directory
    AnsiString sFindFile=sSaveFilePath+"\\*.*";
    TSearchRec srFile;
    if(FindFirst(sFindFile, 0, srFile)==0)
    {
        do
        {
            AnsiString sTemp=sSaveFilePath+"\\"+srFile.Name;
            DeleteFile(sTemp.c_str());
        }
        while(FindNext(srFile)==0);
    }
    SaveSetupCondition(sSaveFileName);
    if(FileExists(sSaveFullFileName)==true)
    {
        FTP_OEEFile_Upload(sSaveFilePath, IniConfig.asN14_8_ULSetupPath, sSaveFileName);
    }
    else
    {
        AnsiString sMsg;
        sMsg.sprintf("File Not Exist => %s", sSaveFullFileName);
        ShowMyMessage(sMsg);
    }
}
//---------------------------------------------------------------------------
AnsiString TfProductionInfo::MyBoolToString(bool b)
{
    if(b==true)
        return "Use";
    else
        return "No Use";
}
//---------------------------------------------------------------------------
void TfProductionInfo::SaveProductionSummaryReport()
{
    AnsiString sFilePath    ="";
    AnsiString sFileName    ="";
    AnsiString sFullFileName="";
    AnsiString sMsgString   ="";
    TStringList *slProductionSummaryReport=new TStringList();
    TStringList *slOEEReport=new TStringList();

    slProductionSummaryReport->Clear();
    slOEEReport->Clear();

    sFilePath=FileInfo().PathCombin(sProductionInfoFilePath, sLoadMO_MO);
    sFilePath=FileInfo().PathCombin(sFilePath, _sOEE_DirectoryName);
    sFileName=AnsiString().sprintf("%s_%s_OEE.csv",_sOEE_HandlerID,_sOEE_MO);
    sFullFileName=FileInfo().PathCombin(sFilePath, sFileName);

    if(FileExists(sFullFileName)==true)
    {
        slOEEReport->LoadFromFile(sFullFileName);
    }
    else
    {
        sMsgString=AnsiString().sprintf("Can Not Find OEE File When SaveProductionSummaryReport! File:%s",sFullFileName);
        ShowMyMessage(sMsgString);
        delete slProductionSummaryReport;
        delete slOEEReport;
        return;
    }

//    sFilePath       =sProductionInfoFilePath+"\\"+sLoadMO_MO+"\\"+_sOEE_DirectoryName; //Mylin 20170515 Modify OEE Function
//    sFileName       =_sOEE_MO+"_ProductionSummaryReport.csv";
    sFileName=AnsiString().sprintf("%s_ProductionSummaryReport.csv",_sOEE_MO);
    sFullFileName=FileInfo().PathCombin(sFilePath, sFileName);

    MyForceDirectories(sFilePath, "TfProductionInfo::SaveProductionSummaryReport");
    AnsiString sStartDateTime=GetCSVLineData(1, slOEEReport->Strings[1])+" "+GetCSVLineData(2,slOEEReport->Strings[1]);

    AnsiString sEndDateTime=GetCSVLineData(1, slOEEReport->Strings[slOEEReport->Count-1])+" "+GetCSVLineData(2, slOEEReport->Strings[slOEEReport->Count-1]);

    AnsiString sSumPowerTime    ="";
    AnsiString sSumRunTime      ="";
    AnsiString sSumSoakTime     ="";
    AnsiString sSumStopTime     ="";
    AnsiString sSumJamCNT       ="";
    AnsiString sSumServiceCNT   ="";
    AnsiString sFLOW            =GetCSVLineData(14,slOEEReport->Strings[1]);    //Sam 20170816 HT-7045 變數改為 HT-9045 變數
    AnsiString sSumINQty        ="";
    AnsiString sSumBIN[8];                                                                        //Sam 20171018 : OEE Bug 修正
    AnsiString sSumActuralOut   ="";
    AnsiString sSumPlanOut      ="";
    AnsiString sMO              =GetCSVLineData(31,slOEEReport->Strings[1]);    //Sam 20171018 : OEE Bug 修正
    AnsiString sIndexTime       ="";
    AnsiString sTestTime        ="";
    sIndexTime.sprintf("%2.3f", fLastIndexTime);
    sTestTime.sprintf("%2.3f", fLastTestTime);

    for(int i=0; i<8; i++)                                                        //Sam 20171018 : OEE Bug 修正
    {
        sSumBIN[i]="";
    }

    try
    {
        for(int i=1; i<slOEEReport->Count; i++)
        {
            AnsiString sResultOEEReport =slOEEReport->Strings[i];
            sSumPowerTime               =AnsiString(atoi(sSumPowerTime.c_str()) + atoi(GetCSVLineData(3, sResultOEEReport).c_str()));
            sSumRunTime                 =AnsiString(atoi(sSumRunTime.c_str())   + atoi(GetCSVLineData(4, sResultOEEReport).c_str()));
            sSumSoakTime                =AnsiString(atoi(sSumSoakTime.c_str())  + atoi(GetCSVLineData(6, sResultOEEReport).c_str()));
            sSumStopTime                =AnsiString(atoi(sSumStopTime.c_str())  + atoi(GetCSVLineData(7, sResultOEEReport).c_str()));
            sSumJamCNT                  =AnsiString(atoi(sSumJamCNT.c_str())    + atoi(GetCSVLineData(9, sResultOEEReport).c_str()));  //Sam 20170801 HT-7045 變數改為 HT-9045 變數
            sSumServiceCNT              =AnsiString(atoi(sSumServiceCNT.c_str())+ atoi(GetCSVLineData(10,sResultOEEReport).c_str()));  //Sam 20170801 HT-7045 變數改為 HT-9045 變數
            sSumINQty                   =AnsiString(atoi(sSumINQty.c_str())     + atoi(GetCSVLineData(17,sResultOEEReport).c_str()));            //Sam 20170801 HT-7045 變數改為 HT-9045 變數
            for(int j=0; j<8; j++)               //Sam 20171018 : OEE Bug 修正
            {
                sSumBIN[j]=AnsiString(atoi(sSumBIN[j].c_str())+atoi(GetCSVLineData(18+j,sResultOEEReport).c_str()));  //Sam 20170801 HT-7045 變數改為 HT-9045 變數
            }
            sSumActuralOut=AnsiString(atoi(sSumActuralOut.c_str())+atoi(GetCSVLineData(27,sResultOEEReport).c_str()));  //Sam 20171018 : OEE Bug 修正
        }
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfProductionInfo::SaveProductionSummaryReport");
        ShowMyMessage("Caculate ProductionSummaryReport Error!!");
    }

    if((fLastIndexTime+fLastTestTime)>0.0)
    {
        sSumPlanOut=IntToStr((int)(ChangeToFloatNonPcnt((double)(atof(sSumPowerTime.c_str())), (double)(fLastTestTime)))); //Sam 20170829 HT-7045 變數改為 HT-9045 變數
    }
    else
    {
        sSumPlanOut=sSumActuralOut;
    }
    _iOEE_planout=atoi(sSumPlanOut.c_str());

    sMsgString="Start Date Time,"   +sStartDateTime;
    slProductionSummaryReport->Append(sMsgString);
    sMsgString="End Date Time,"     +sEndDateTime;
    slProductionSummaryReport->Append(sMsgString);
    sMsgString="Power Time(Sec),"   +sSumPowerTime;
    slProductionSummaryReport->Append(sMsgString);
    sMsgString="Run Time,"          +sSumRunTime;
    slProductionSummaryReport->Append(sMsgString);
    sMsgString="Soak Time,"         +sSumSoakTime;
    slProductionSummaryReport->Append(sMsgString);
    sMsgString="Stop Time,"         +sSumStopTime;
    slProductionSummaryReport->Append(sMsgString);
    sMsgString="Jam CNT,"           +sSumJamCNT;
    slProductionSummaryReport->Append(sMsgString);
    sMsgString="Service CNT,"       +sSumServiceCNT;
    slProductionSummaryReport->Append(sMsgString);
    sMsgString="FLOW,"              +sFLOW;
    slProductionSummaryReport->Append(sMsgString);
    sMsgString="IN Q'ty,"           +sSumINQty;
    slProductionSummaryReport->Append(sMsgString);
    for(int i=0; i<8; i++)                                                      //Sam 20171018 : OEE Bug 修正
    {
        sMsgString="BIN"+IntToStr(i+1)+","+sSumBIN[i];                          //Sam 20171018 : OEE Bug 修正
        slProductionSummaryReport->Append(sMsgString);
    }
    sMsgString="Actural Out,"       +sSumActuralOut;
    slProductionSummaryReport->Append(sMsgString);
    sMsgString="plan out,"          +sSumPlanOut;
    slProductionSummaryReport->Append(sMsgString);
    sMsgString="M.O,"               +sMO;
    slProductionSummaryReport->Append(sMsgString);
    sMsgString="Index Time,"        +sIndexTime;
    slProductionSummaryReport->Append(sMsgString);
    sMsgString="Test Time,"         +sTestTime;
    slProductionSummaryReport->Append(sMsgString);
    slProductionSummaryReport->SaveToFile(sFullFileName);

    delete slProductionSummaryReport;
    delete slOEEReport;
}
//---------------------------------------------------------------------------
void __fastcall TfProductionInfo::bt_PIRefreshMOListClick(TObject *Sender)
{
    AnsiString sFilePath=sProductionInfoFilePath+"\\"+cb_PIChoiseMO->Text+"\\";
    AnsiString sFileName="";
    AnsiString sFilePathName[5]={"", "", "", "", ""};
    AnsiString sIncludFileName[5]={"_OEE.CSV", "_ProductionSummaryReport.csv", "_SetupCondition.csv", "_History.csv", "_ESDControlMsg.csv"};
    int iErr=0;
    std::vector<AnsiString> fileNames =FileInfo().GetAllFileNamesInFolder(sFilePath,iErr);
    if(iErr==1)
    {
        ShowMyMessage("Folder path error \n\r"+sFilePath);
    }
    else
    {
        for(unsigned int i=0;i<fileNames.size();i++)
        {
            sFileName=fileNames[i].UpperCase();
            for(int j=0;j<5;j++)
            {
                if(sFileName.Pos(sIncludFileName[j].UpperCase())>0)
                {
                    sFilePathName[j]=sFilePath+sFileName;
                    break;
                }
            }
        }
    }
    LoadCSVFileToStringGrid(sFilePathName[0], sg_OEEList, 42);                  //Sam 20170801 HT-7045 變數改為 HT-9045 變數
    LoadCSVFileToStringGrid(sFilePathName[1], sg_SummaryReport, 2);
    LoadCSVFileToStringGrid(sFilePathName[2], sg_SetupCondition, 2);
    LoadCSVFileToStringGrid(sFilePathName[3], sg_History, 6);
    LoadCSVFileToStringGrid(sFilePathName[4], sg_ESDControlMsg, 2);
}
//---------------------------------------------------------------------------
void TfProductionInfo::LoadCSVFileToStringGrid(AnsiString sFilePathName,TStringGrid *sgList,int iColCount)
{
    for(int i=0; i<sgList->ColCount; i++)
    {
        for(int j=0; j<sgList->RowCount; j++)
        {
            sgList->Cells[i][j]="";
        }
    }

    if(FileExists(sFilePathName)==false)
    {
        return;
    }

    TStringList *slList=new TStringList();
    slList->LoadFromFile(sFilePathName);
    for(int i=0; i<slList->Count; i++)
    {
        AnsiString sStringList=slList->Strings[i];
        sgList->RowCount=slList->Count;
        for(int j=0; j<iColCount; j++)
        {
            sgList->Cells[j][i]=GetCSVLineData(j+1,sStringList);
        }
    }
    slList->Clear();
    delete slList;
}
//---------------------------------------------------------------------------
void __fastcall TfProductionInfo::cb_PIChoiseMOKeyUp(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
    if(Key == VK_RETURN || Key == VK_ESCAPE ||          //13 || 27
       Key >= 35 && Key <= VK_DOWN)                     //35 && 40
    {
        return;
    }
    RefreshPIChoiseMO();
}
//---------------------------------------------------------------------------
void TfProductionInfo::RefreshPIChoiseMO()
{
    int iSelStart=cb_PIChoiseMO->SelStart;
    AnsiString sTemp=cb_PIChoiseMO->Text;
    dlb_PIChoiseMO->Directory=sProductionInfoFilePath;
    cb_PIChoiseMO->Items->Clear();
    for(int i=3; i<dlb_PIChoiseMO->Items->Count; i++)
    {
        AnsiString sMO=dlb_PIChoiseMO->Items->Strings[i].UpperCase();
        if(sMO.Trim()=="")
        {
            continue;
        }

        if(sMO.Pos(cb_PIChoiseMO->Text.UpperCase())> 0 || cb_PIChoiseMO->Text=="")
        {
            cb_PIChoiseMO->Items->Add(sMO);
        }
    }

    if(PageControl1->ActivePage==tsInformation)
    {
        cb_PIChoiseMO->DroppedDown=true;
    }
    cb_PIChoiseMO->Text=sTemp;
    cb_PIChoiseMO->SelStart=iSelStart;
}
//---------------------------------------------------------------------------
void __fastcall TfProductionInfo::bt_PICloseFormClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
bool TfProductionInfo::CheckMOInformation()
{
    bool bCheckOK=true;
    bool bTempError=false;
    if(IniConfig.bN14_1_EnableOEEFunction==false || IniConfig.bN14_3_OEEFTPUpload==false)
    {
        return true;
    }

    if(_bOEEStartLotSuccess==false)
    {
        return true;
    }

    if(LoadMOInformation(true)==false)
    {
        AnsiString sMsg="Check MO Information Fail!#Please Check Load MO File";
        ShowMyMessage(sMsg);
        return false;
    }

    if(IniConfig.bN14_4_OEEAutoLoadMOFile==false)
    {
        sLoadMO_MO=_sOEE_MO;
        sLoadMO_TestFlow="";
        return true;
    }
    //Check Temperature //Temperature:25
    int iNowTemperature=atoi(sLoadMO_Temperature.c_str());

    bTempError=false;
    if(sLoadMO_TemperatureMode=="Hot")  //Sam 20210423 : MO file 新增 Temperature mode
    {
        if(LastSet.iTemperature!=Tempture_Hot || iNowTemperature!=Temperature.fWorkTemperBase)
            bTempError=true;
    }
    else
    {
        if(iNowTemperature<=IniConfig.iN14_17_AmbientULTemp && LastSet.iTemperature!=Tempture_Ambient)
            bTempError=true;
        else if(iNowTemperature>IniConfig.iN14_17_AmbientULTemp && (LastSet.iTemperature!=Tempture_Hot || iNowTemperature!=Temperature.fWorkTemperBase))
            bTempError=true;
    }

    if(bTempError)
    {
        AnsiString sMsg="Check MO Information Fail!#Check Temperature Error";
        ShowMyMessage(sMsg);
        return false;
    }

    //Check Soak Time //Soak Time:0
    int iNowSoakTime=atoi(sLoadMO_SoakTime.c_str());
    if(LastSet.iTemperature!=Tempture_Ambient && iNowSoakTime!=Temperature.fSoakTime)
    {
        AnsiString sMsg = "Check MO Information Fail!#Check Soak Time Error";
        ShowMyMessage(sMsg);
        return false;
    }

    //Check Test Flow //INDEX:MF
    //Check PassFail //PASSBIN:FFFF <========良品Bin2~Bin5(Bin1已定義為良品,例：Bin1~3為良品:PPFF)
    bool bCheckPassFailOK=true;
    int iLoadMO_PassFail[5];
    int iPassFailCount=sLoadMO_PASSBIN.Length()+1;
    if(iPassFailCount>5)
    {
        bCheckPassFailOK=false;
    }

    iLoadMO_PassFail[0]=0;
    if(bCheckPassFailOK==true)
    {
        for(int i=1; i<iPassFailCount; i++)
        {
            AnsiString sBin=sLoadMO_PASSBIN.SubString(i,1);
            if(sBin=="P")
            {
                iLoadMO_PassFail[i]=0;
            }
            else if(sBin=="F")
            {
                iLoadMO_PassFail[i]=1;
            }
            else
            {
                iLoadMO_PassFail[i]=-1;
                bCheckPassFailOK=false;
            }
        }

        //AI(ht9045-v899) 20260820: PTI 的 Fix1 位置是 rotate module,未指派 Category 時程式會把
        //Pass/Fail 預設為 Fail(避免顯示成好品綠色),此時不可再拿它與 MO 的 PASSBIN 字串比對,
        //否則會誤報 Check PASSBIN Error 擋住 Start。PASSBIN 最多 4 碼,只會涵蓋到 e3Fix2,故只需排除 e3Fix1。
        bool bSkipFix1PassFail=false;
        if(CUSTOMER_CODE==CC_PTI)
        {
            bSkipFix1PassFail=true;
            for(int iChkBin=0; iChkBin<iTestBinCount && bSkipFix1PassFail==true; iChkBin++)
            {
                if(BinSelect[iTestRunMode].iCatDataT3Pos[iChkBin]==e3PosFix1)
                {
                    bSkipFix1PassFail=false;
                }
            }
        }

        for(int i=0; i<iPassFailCount; i++)
        {
            if(bSkipFix1PassFail==true && i==e3Fix1)                             //AI(ht9045-v899) 20260820: 見上方說明
                continue;

            if(BinSelect[iTestRunMode].iStackDefFailCate[i]!=iLoadMO_PassFail[i])
            {
                bCheckPassFailOK=false;
            }
        }
    }

    if(bCheckPassFailOK==false)
    {
        AnsiString sMsg="Check MO Information Fail!#Check PASSBIN Error";
        ShowMyMessage(sMsg);
        return false;
    }

    //Check Interface Type //Interface Type:GPIB
    if(sLoadMO_InterfaceType.Pos("GPIB") ==1 && TestIF.iTestType!=GPIB_MODE ||
       sLoadMO_InterfaceType.Pos("TTL")  ==1 && TestIF.iTestType!=TTL_MODE  ||
       sLoadMO_InterfaceType.Pos("RS232")==1 && TestIF.iTestType!=RS232_MODE)
    {
        AnsiString sMsg="Check MO Information Fail! Please Check Test Type is "+sLoadMO_InterfaceType;
        ShowMyMessage(sMsg);
        return false;
    }
    //Check Handler //Handler:GOANY519
    if(sLoadMO_Handler!=_sOEE_HandlerID)
    {
        AnsiString sMsg="Check MO Information Fail!#Check Handler Error";
        ShowMyMessage(sMsg);
        return false;
    }

    //Sam 20180412 (wei) : Add Check CONTINUE PASS & CONTINUE FAIL Info on FT/QC
    //==>
    AnsiString sTitle=sLoadMO_TestFlow.SubString(1,1);
    if(sTitle=="F" || sTitle=="Q")
    {
        //Sam 20200224 : FT 檢查 ContinuousFail 需要強制被打開且數量正確
        if(Prod.bContsFailBySocket!=true || atoi(sLoadMO_CONTINUE_FAIL.c_str())!=int(Prod.iContsFailSocketAlarmCT))
        {
            AnsiString sMsg="Check MO Information Fail!#Check Yield ContinuousFail OFF Error";
            ShowMyMessage(sMsg);
            return false;
        }
    }
    else
    {
        //Sam 20200224 : RT 檢查 ContinuousFail 需要強制被關閉
        if(Prod.bContsFailBySocket==true)
        {
            AnsiString sMsg="Check MO Information Fail!#Check Yield ContinuousFail On Error";
            ShowMyMessage(sMsg);
            return false;
        }
    }
    //Sam 20200224 : FT 或 RT 都要檢查 ContinuousPass 需要強制被打開且數量正確
    if(Prod.bContinuousPassBySocket!=true || atoi(sLoadMO_CONTINUE_PASS.c_str())!=int(Prod.iContinuousPassBinCountBySocket))
    {
        AnsiString sMsg="Check MO Information Fail!#Check Yield ContinuousPass Error";
        ShowMyMessage(sMsg);
        return false;
    }
    //<==
    //Sam 20180412 (wei) : Add Check CONTINUE PASS & CONTINUE FAIL Info on FT/QC
    return bCheckOK;
}
//---------------------------------------------------------------------------
//AI(ht9045-v899) 20260819: 超豐要求依 MO 工單 Temperature mode 欄位自動開關熱風槍(Active Heater Gun)
//規則(20260819 內部定案):
//  1. Temperature mode 去除前後空白後為空字串(含工單無此欄) -> 不動作, 保留工作檔原設定
//  2. 去空白後轉大寫含 "HOT AIR"                             -> 開啟熱風槍
//  3. 其餘(例如 "Hot" / "Ambient")                            -> 關閉熱風槍
//呼叫點必須在 AutoDownloadSetupFileByMO() 換完工作檔之後, 否則會被 ReadTempFile() 重讀工作檔蓋掉
//只改執行期旗標與畫面勾選, 不回寫工作檔, 避免污染客戶 recipe
void TfProductionInfo::ApplyHotAirByMOTemperatureMode()
{
    if(CosFunction.bHotAirByMOTemperatureMode==false)
        return;
    if(INSTALL_HEAT_GUN<=0)                                                     //AI(ht9045-v899) 20260819: 沒有熱風槍機構就不動作
        return;

    AnsiString asMode=sLoadMO_TemperatureMode.Trim();
    if(asMode=="")                                                              //AI(ht9045-v899) 20260819: 舊工單無此欄或值為空, 保留工作檔原設定
    {
        fLotInfo->WriteFTPSetupFileChangeLog("Hot Air keep recipe setting (MO Temperature mode is empty)");
        return;
    }

    bool bNeedHotAir=(asMode.UpperCase().Pos("HOT AIR")>0);
    if(bNeedHotAir==true && Temperature.bUseCDAOnly==true)                      //AI(ht9045-v899) 20260819: 對齊 ReadTempFile 既有互斥: 熱風槍吹冷風時不開加熱
    {
        bNeedHotAir=false;
        fLotInfo->WriteFTPSetupFileChangeLog("Hot Air OFF forced by Use CDA Only");
    }

    if(Temperature.bActiveHeatGun!=bNeedHotAir)
    {
        Temperature.bActiveHeatGun=bNeedHotAir;
        fTemp_Set->cbHeaterGun->Checked=bNeedHotAir;                            //AI(ht9045-v899) 20260819: 同步畫面勾選, 讓 OP 看得到目前狀態
    }

    AnsiString asLog;
    asLog.sprintf("Hot Air %s by MO Temperature mode = [%s]", (bNeedHotAir ? "ON" : "OFF"), asMode.c_str());
    fLotInfo->WriteFTPSetupFileChangeLog(asLog);
}
//---------------------------------------------------------------------------
bool TfProductionInfo::SetMOInformation()
{
    AnsiString sMsg;
    double dTestTime=0.0;
    int iReleaseSite=0;
    static AnsiString sDevice="";
    AnsiString szDir="";
    if(IniConfig.bN14_1_EnableOEEFunction==false || IniConfig.bN14_3_OEEFTPUpload==false)
    {
        return true;
    }

    if(LoadMOInformation()==false)
    {
        sMsg=AnsiString(__FUNC__)+" Load MO Information Fail!";
        ShowMyMessage(sMsg);
        return false;
    }

    if(IniConfig.bN14_4_OEEAutoLoadMOFile==false)
    {
        sLoadMO_MO=_sOEE_MO;
        sLoadMO_TestFlow="";
        return true;
    }
    //Set Temperature //Temperature:25
    //Set Soak Time //Soak Time:0
    int iNowTemperature =atoi(sLoadMO_Temperature.c_str());
    int iNowSoakTime    =atoi(sLoadMO_SoakTime.c_str());

    if(sLoadMO_TemperatureMode=="Hot")                                          //Sam 20210423 : MO file 新增 Temperature mode
    {
        LastSet.iTemperature=Tempture_Hot;
        fTemp_Set->rgTemperatureMode->ItemIndex=0;
    }
    else
    {
        if(iNowTemperature<=IniConfig.iN14_17_AmbientULTemp)                    //Sam 20200525 : 正賢要求溫度 by Config 以下都是常溫。
        {
            LastSet.iTemperature=Tempture_Ambient;
            fTemp_Set->rgTemperatureMode->ItemIndex=1;                          //Sam 20171018 : OEE Bug 修正
        }
        else
        {
            LastSet.iTemperature=Tempture_Hot;
            fTemp_Set->rgTemperatureMode->ItemIndex=0;                          //Sam 20171018 : OEE Bug 修正
        }
    }
    fMain->edWorkTemperBase->Text=AnsiString(iNowTemperature);                  //Sam 20170815 HT-7045 變數改為 HT-9045 變數
    Temperature.fWorkTemperBase=iNowTemperature;
    fMain->edSoakTime->Text=AnsiString(iNowSoakTime);                           //Sam 20170815 HT-7045 變數改為 HT-9045 變數
    Temperature.fSoakTime=iNowSoakTime;                                         //Sam 20170815 HT-7045 變數改為 HT-9045 變數

    fTemp_Set->spbSaveClick(fTemp_Set);
    fMain->spbSetClick(fMain);                                                  //Sam 20170801 HT-7045 變數改為 HT-9045 變數
    fMain->ChangeTempMode(LastSet.iTemperature,false,false,false);              //Sam 20170925 加熱模式改變

    //Set Test Flow //INDEX:MF
    //Set PassFail //PASSBIN:FFFF <========良品Bin2~Bin5(Bin1已定義為良品,例：Bin1~3為良品:PPFF)
    //Sam 20170925 : 重客戶端討論的定義與 7 系列的註解有認知落差 Set PassFail //PASSBIN:FFFF <========良品Auto1~Fix3(Auto1 已定義為良品,例：Auto1~3為良品:PPFF)
    //MOFile Bin Pass/Fail Setting Only Bin2 ~ Bin5, Bin0 Bin1 always Pass。
    bool bCheckPassFailOK=true;
    int iLoadMO_PassFail[5];
    int iPassFailCount=sLoadMO_PASSBIN.Length()+1;                              //Auto1 Pass + PPFF = 6

    //檢查 MOFile Bin 長度
    if(iPassFailCount>5)
    {
         bCheckPassFailOK=false;
    }

    iLoadMO_PassFail[0]=0;                                                      //Auto1 = Pass
    //檢查 MOFile Bin 字串
    if(bCheckPassFailOK==true)
    {
        for(int i=1; i<iPassFailCount; i++)
        {
            AnsiString sBin=sLoadMO_PASSBIN.SubString(i,1);
            if(sBin=="P")
            {
                iLoadMO_PassFail[i]=0;
            }
            else if(sBin=="F")
            {
                iLoadMO_PassFail[i]=1;
            }
            else
            {
                iLoadMO_PassFail[i]=-1;
                bCheckPassFailOK=false;
            }
        }
    }

    if(bCheckPassFailOK==true)
    {
        for(int i=0; i<iPassFailCount; i++)
        {
            BinSelect[iTestRunMode].iStackDefFailCate[i]=iLoadMO_PassFail[i];
        }
    }
    else
    {
        sMsg.sprintf("Set MO Information Fail!#Check PASSBIN Error, MOFile Data = %s",sLoadMO_PASSBIN);
        ShowMyMessage("Setup Error", sMsg);
        return false;
    }
    bGPIBChangeAlarm=true;
    fYieldMonitoring->btnApplyClick(this);
    //fBinSel->PageControl1->ActivePageIndex=iTestRunMode;                    //切換 Page 才能存地方
    fBinSel->ChangeActivePageIndex();   //Sam 20230711 : 修正 OLP SetCategroy 異常
    fBinSel->spbSaveClick(this);
    fShowBinSelect->ShowBinSel();
    bGPIBChangeAlarm=false;

    //Sam 20171227 : Add CONTINUE Pass/Fail //Sam 20171212 : 超豐良率監控
    //Sam 20180423 (wei) : MOFile of Yeild Download
    //==>
    fYieldMonitoring->Show();
    fYieldMonitoring->cbContinuPassSkt_FT->Checked  =true;      //Sam 20200224 : FT 或 RT 都要檢查 ContinuousPass 需要強制被打開且數量正確
    fYieldMonitoring->cbContinuPassSkt_RT->Checked  =true;      //Sam 20200224 : FT 或 RT 都要檢查 ContinuousPass 需要強制被打開且數量正確
    fYieldMonitoring->rgPiggyBack_FT->ItemIndex     =0;         //Sam 20200225 : Yiedl Continus Pass 條件成立後，強制 Alarm 不要 Index Check。
    fYieldMonitoring->rgPiggyBack_RT->ItemIndex     =0;         //Sam 20200225 : Yiedl Continus Pass 條件成立後，強制 Alarm 不要 Index Check。

    fYieldMonitoring->edContinuPassSkt_FT->Text         =sLoadMO_CONTINUE_PASS; //Sam 20200224 : FT 或 RT 都要檢查 ContinuousPass 需要強制被打開且數量正確
    fYieldMonitoring->edContinuPassSkt_RT->Text         =sLoadMO_CONTINUE_PASS; //Sam 20200224 : FT 或 RT 都要檢查 ContinuousPass 需要強制被打開且數量正確
    fYieldMonitoring->edContsFailSocketAlarmCT_FT->Text =sLoadMO_CONTINUE_FAIL; //Sam 20200224 : FT 檢查 ContinuousFail 需要強制被打開且數量正確
    fYieldMonitoring->edContsFailSocketAlarmCT_RT->Text =sLoadMO_CONTINUE_FAIL; //Sam 20200224 : FT 檢查 ContinuousFail 需要強制被打開且數量正確

    AnsiString sTitle=sLoadMO_TestFlow.SubString(1,1);
    if(sTitle=="F" || sTitle=="Q")
    {
        if(CosFunction.bYieldAlarm5)    //Sam 20200224 : 正賢要求關閉Alarm改由他們自己監控
        {
            fYieldMonitoring->cbAlarm5_BySiteLowYieldEnable->Checked    =true;
            fYieldMonitoring->cbAlarm5_BySiteCmpYieldEnable->Checked    =true;
            fYieldMonitoring->cbAlarm5_BySiteAlarmYieldEnable->Checked  =true;
            fYieldMonitoring->cbAlarm5_BySitePreCmpYieldEnable->Checked =true;
            fYieldMonitoring->edAlarm5_BySiteLowYield->Text             =sLoadMO_YieldLimit;
            fYieldMonitoring->edAlarm5_BySiteAlarmYield->Text           =sLoadMO_AlarmYield;
            fYieldMonitoring->edAlarm5_OSBin->Text                      =sLoadMO_OSBin;
        }
        fYieldMonitoring->rbContsFailBySocket_RTOn->Checked         =true;
        fYieldMonitoring->rbContsFailBySocket_FTOn->Checked         =true;
        fYieldMonitoring->rbContsFailBySocket_FTOff->Checked        =false;
        fYieldMonitoring->rbContsFailBySocket_RTOff->Checked        =false;
    }
    else
    {
        if(CosFunction.bYieldAlarm5)    //Sam 20200224 : 正賢要求關閉Alarm改由他們自己監控
        {
            fYieldMonitoring->cbAlarm5_BySiteLowYieldEnable->Checked    =false;
            fYieldMonitoring->cbAlarm5_BySiteCmpYieldEnable->Checked    =false;
            fYieldMonitoring->cbAlarm5_BySiteAlarmYieldEnable->Checked  =false;
            fYieldMonitoring->cbAlarm5_BySitePreCmpYieldEnable->Checked =false;     //Sam 20180423 (wei) : MOFile of Yeild Download
        }
        fYieldMonitoring->rbContsFailBySocket_RTOn->Checked         =false;
        fYieldMonitoring->rbContsFailBySocket_FTOn->Checked         =false;
        fYieldMonitoring->rbContsFailBySocket_FTOff->Checked        =true;
        fYieldMonitoring->rbContsFailBySocket_RTOff->Checked        =true;
    }

    if(fTrayForm->IsEnableColorSensor()==true)                                  //Jimmychiu 20230630 : add color sensor MU-N in Loader
    {
        if(sTitle=="F" || sTitle=="Q" || sTitle=="C")
        {
            fTrayForm->GetColorSensor("ColorSensor_FT")->SetAllEnable(true);
            fTrayForm->GetColorSensor("ColorSensor_FT")->SaveColorSensorEnable();
            fTrayForm->spbSaveClick(this);
        }
        else
        {
            SetRTColorSensorEnable(sNot_RT_BIN);
        }
    }
    fYieldMonitoring->btnApplyClick(this);
    fYieldMonitoring->btnOkClick(this);
    //<==
    //Sam 20180423 (wei) : MOFile of Yeild Download
    //Sam 20171227 : Add CONTINUE Pass/Fail //Sam 20171212 : 超豐良率監控
    //Set Interface Type //Interface Type:GPIB
    if(sLoadMO_InterfaceType.Pos("GPIB")==1)
    {
        TestIF_File.iTestType=GPIB_MODE;                                        //Sam 20171018 : OEE Bug 修正
    }
    else if(sLoadMO_InterfaceType.Pos("TTL")==1)
    {
        TestIF_File.iTestType=TTL_MODE;                                         //Sam 20171018 : OEE Bug 修正
    }
    else if(sLoadMO_InterfaceType.Pos("RS232")==1)
    {
         TestIF_File.iTestType=RS232_MODE;                                      //Sam 20171018 : OEE Bug 修正
    }
    else
    {
        AnsiString sMsg="Set MO Information Fail!#Set Interface Error";
        ShowMyMessage(sMsg);
        return false;
    }
    //Sam 20171018 : OEE Bug 修正
    //==>
    FTestIF->rgInterfaceType->ItemIndex=TestIF_File.iTestType;
    FTestIF->spbSaveClick(FTestIF);
    FTestIF->ReadTestIFFile();
    AnsiString S="";
    S.sprintf("%s%s.ini", DIOCFGPath, FTestIF->cbDIOType->Text);
    fDIOFrom->LoadData(S);
    DoStructUnitConvert();
    SetWorkParameter();
    fMain->LoadTestModePicture();
    bEnterTestIF=true;
    //<==
    //Sam 20171018 : OEE Bug 修正

    //Sam 20210426 : 增加 Auto Clean Interval Contact
    //==>
    if(sDevice!=sLoadMO_Device)   //Sam 20210728 : Device 有變更的時候才需要重新 Setting
    {
        sDevice=sLoadMO_Device;
        dTestTime=atof(sLoadMO_TestTime.c_str());
        if(dTestTime<=0.0)
            dTestTime=1.0;
        iReleaseSite=atoi(sLoadMO_RELEASE_SITE.c_str());
        if(iReleaseSite==0)
            iReleaseSite=1;
        iAutoClean_IntervalContact_Server=(int)(3600/dTestTime/iReleaseSite);
        TestIF_File.iAutoClean_IntervalContact=iAutoClean_IntervalContact_Server;
        fCleaning->edIntervalContact->Text=TestIF_File.iAutoClean_IntervalContact;

        szDir=GetRecipeFileName("HandlerCondition.Data");
        WriteIniData(szDir, "Configuration", "iAutoClean_IntervalContact",     TestIF_File.iAutoClean_IntervalContact);                 //寫入到 Currect Recipe Value

        szDir="D:\\HT9045\\IniData\\RPDefault.ini";
        WriteIniData(szDir, "Auto Clean", "AutoClean_Cleaning Mode Interval Contact Count", TestIF_File.iAutoClean_IntervalContact);   //寫入到 Default Recipe Value
        SetOffsetContactForce(IniConfig.dD06_ContactOffsetDefaultValue);
    }
    //<==
    //Sam 20210426 : 增加 Auto Clean Interval Contact
    //JimmyChiu 20220114 : MO file 新增 Device Pin、Device Force
    //<==
    //如果沒有讀取到或數值為0，沿用舊數值且不Offset contact
    if(CheckContactForceExist())
    {
        if(dOriginal_Device_Pin_Force==0.0)
        {
            dOriginal_Device_Pin_Force=atof(fContact->edForcePerPinG->Text.c_str());
            dOriginal_Device_Pin_Count=atof(fContact->edPinCount->Text.c_str());
        }
        fContact->edForcePerPinG->Text=sDevice_Pin_Force;
        fContact->edPinCount->Text=sDevice_Pin_Count;
        fContact->spbSaveClick(fContact);
    }
    else
    {
        if(dOriginal_Device_Pin_Force!=0.0)
        {
            fContact->edForcePerPinG->Text=FloatToStr(dOriginal_Device_Pin_Force);
            fContact->edPinCount->Text=FloatToStr(dOriginal_Device_Pin_Count);
            fContact->spbSaveClick(fContact);
        }
    }
    //<==
    //JimmyChiu 20220114 : MO file 新增 Device Pin、Device Force

    if(dTestTime<1)
        iIPSCFlag_CountdownQty=4000;
    else if(dTestTime>=1 && dTestTime<14)
        iIPSCFlag_CountdownQty=ChangeToFloatNonPcnt((double)(7200), (double)(dTestTime));
    else if(dTestTime>=14)
        iIPSCFlag_CountdownQty=500;
    cDynaThres->ResetThresholdNum();                                            //JimmyChiu 20240411 : Dynamic multiplier for Continual Pass Bin( Socket )
    return true;
}
//---------------------------------------------------------------------------
bool TfProductionInfo::SetYieldInformation()                                    //Sam 20180423 (wei) : MOFile of Yeild Download
{
    AnsiString sMsg;
    if(IniConfig.bN14_1_EnableOEEFunction==false || IniConfig.bN14_3_OEEFTPUpload==false)
    {
        return true;
    }

    if(LoadYiedlInformation()==false)
    {
        sMsg="Set Yield Information Fail! Please Check Load Yield File";
//        ShowMyMessage(sMsg);
        NewRecordProcess("", "SetYieldInformation() Error", sMsg);
//        return false;
    }
    fYieldMonitoring->Show();
    fYieldMonitoring->edAlarm5_BySiteLowYield->Text             =sLoadMOYiled_AVG_Alarm_Yield;
    fYieldMonitoring->edAlarm5_BySiteLowYieldRej->Text          =sLoadMOYiled_AVG_Reject_Yield;
    fYieldMonitoring->edAlarm5_BySiteAlarmYield->Text           =sLoadMOYiled_OS_rate;
    fYieldMonitoring->edAlarm5_BySiteAlarmYieldRej->Text        =sLoadMOYiled_OS_rate_Rej;
    fYieldMonitoring->edAlarm5_OSBin->Text                      =sLoadMO_OSBin;
    fYieldMonitoring->edAlarm5_BySiteIntervalContactCnt->Text   =sLoadMOYiled_Qty;
    fYieldMonitoring->edAlarm5_BySiteCmpYield->Text             =sLoadMOYiled_Different_Alarm_Yield;
    fYieldMonitoring->edAlarm5_BySiteCmpYieldRej->Text          =sLoadMOYiled_Different_Alarm_Yield_Rej;
    fYieldMonitoring->edAlarm5_BySitePreCmpYield->Text          =sLoadMOYiled_Yield_Difference;
    fYieldMonitoring->edAlarm5_BySitePreCmpYieldRej->Text       =sLoadMOYiled_Yield_Difference_Rej;
    TestIF_File.dAlarm5_BySiteLowYield                          =atof(sLoadMOYiled_AVG_Alarm_Yield.c_str());
    TestIF_File.dAlarm5_BySiteLowYieldRej                       =atof(sLoadMOYiled_AVG_Reject_Yield.c_str());
    TestIF_File.dAlarm5_BySiteAlarmYield                        =atof(sLoadMOYiled_OS_rate.c_str());
    TestIF_File.dAlarm5_BySiteAlarmYieldRej                     =atof(sLoadMOYiled_OS_rate_Rej.c_str());
    TestIF_File.iAlarm5_OSBin                                   =atoi(sLoadMO_OSBin.c_str());
    TestIF_File.iAlarm5_BySiteIntervalContactCnt                =atoi(sLoadMOYiled_Qty.c_str());
    TestIF_File.dAlarm5_BySiteCmpYield                          =atof(sLoadMOYiled_Different_Alarm_Yield.c_str());
    TestIF_File.dAlarm5_BySiteCmpYieldRej                       =atof(sLoadMOYiled_Different_Alarm_Yield_Rej.c_str());
    TestIF_File.dAlarm5_BySitePreCmpYield                       =atof(sLoadMOYiled_Yield_Difference.c_str());
    TestIF_File.dAlarm5_BySitePreCmpYieldRej                    =atof(sLoadMOYiled_Yield_Difference_Rej.c_str());
    fYieldMonitoring->btnApplyClick(this);
    fYieldMonitoring->btnOkClick(this);

    if(iControlBinCheckPoint>0) //Sam 20201221 : 增加保護
    {
        if(iControlBinTimerCnt==0)//重新計算
        {
            iControlBinTimerCnt=iControlBinCheckPoint;
            UpdateControlBinCount(true);
        }
        else if(iControlBinTimerCnt<0) //Sam 20201221 : 增加保護
        {
            NewRecordProcess("", "Control Bin Check Count Error", IntToStr(iControlBinTimerCnt));
            iControlBinTimerCnt=1;
        }

        if(bIsControlBinCounting==false)
        {
            bIsControlBinCounting=true;
            tControlBinTimer.LatchCycleTimeSec(true);
            NewRecordProcess("", "Start Control Bin Check", IntToStr(iControlBinTimerCnt)+"s");
            tControlBinTimer.SetSecAndOn(iControlBinTimerCnt);
        }
    }
    return true;
}
//---------------------------------------------------------------------------
bool TfProductionInfo::LoadMOInformation(bool bCheckMoInforOnly)
{
    TStringList *TempTajList[5];
    AnsiString S1="",sTempDevice="",sTempHandler="",sTempTaj_offset="";
    bool bFind=false;
    double dMOFileTemperature=0.0;
    if(IniConfig.bN14_1_EnableOEEFunction==false || IniConfig.bN14_3_OEEFTPUpload==false)
    {
        ShowMyMessage("HandlerOEEFTPUpload = false in LoadMOInformation");
        return false;
    }

    if(IniConfig.bN14_4_OEEAutoLoadMOFile==false)
    {
        sLoadMO_MO=_sOEE_MO;
        sLoadMO_TestFlow="";
        OEE_SetMO(fLotInfo->ed_PIOEEMO->Text);                                  //Sam 20170801 HT-7045 變數改為 HT-9045 變數
        OEE_SetHandlerID(IniConfig.SocketHandlerID);
        if(_bOEEStartLotSuccess==false)
        {
            _sOEE_DirectoryName.sprintf("%s_%s_%s",
                                            Now().FormatString("yyyymmdd"),
                                            Now().FormatString("hhnnss"),
                                            _sOEE_MO);
        }
        return true;
    }

    //Sam 20190227 : Try Catch Mo Download SetupFile
    //==>
    if(sLocalMOFullFileName!="")
    {
        if(FileExists(sLocalMOFullFileName)==false)
        {
            sLocalMOFullFileName="";
            ShowMyMessage("Local MO File Lose Error");
            return false;
        }
    }
    //<==
    //Sam 20190227 : Try Catch Mo Download SetupFile
    //FTP Download to HD
    AnsiString sSourcesFilePath=IniConfig.asN14_4_MODownloadPath;
    AnsiString sTargetFilePath=GetProInfoFilePath();
    AnsiString sTempSourcesFilePath=IniConfig.asN14_18_TempOffsetPath;
    #ifdef SOFT_SIMULTE
    sSourcesFilePath="//handler_data//SETUP_FILE//";
    #endif

    //MOFile Source Paht Check
    if(_sOEE_HandlerID.Trim()=="")
    {
        ShowMyMessage("Machine ID is Empty in LoadMOInformation");
        return false;
    }
    CheckFTPFilePath(sSourcesFilePath);       //JimmyChiu 20220111 check FTP Sources File Path
    sSourcesFilePath=sSourcesFilePath+_sOEE_HandlerID;         //JimmyChiu 20220111 Fix FTP Sources File Path
    //MOFile Target Paht Check
    MyForceDirectories(sTargetFilePath+"\\TempLoadMO\\", "TfProductionInfo::LoadMOInformation_1");
    //Sam 20170810  HT-7045 變數改為 HT-9045 變數
    //==>
    if(sLocalMOFullFileName=="")     //Sam 20190227 : Try Catch Mo Download SetupFile
    {
        if(DeleteAllFileInDirectory(sTargetFilePath+"\\TempLoadMO\\")==false)
        {
            return false;
        }
        //Download Load MO File to sTargetFilePath
        CheckFTPFilePath(sSourcesFilePath);         //JimmyChiu 20220111 check FTP Sources File Path
        if (FTP_MOFile_Download(sSourcesFilePath,sTargetFilePath+"\\TempLoadMO\\")==false)
        {
            ShowMyMessage("TempLoadMO Download Error");//Jimmychiu 20220901 修正異常顯示
            return false;
        }

        if(bN14_21_SetUpConfig_Success==true)
        {
            //
        }
        else if(IniConfig.bN14_10_DownFileByMO==true)        //Sam 20190227 : Try Catch Mo Download SetupFile
        {
            MyForceDirectories(sTargetFilePath+"\\TempLoadMO\\AutoLoad_TrayForm\\", "TfProductionInfo::LoadMOInformation_2");
            CheckFTPFilePath(sSourcesFilePath);
            sSourcesFilePath=sSourcesFilePath+"AutoLoad_TrayForm//";
            if(FTP_MOFile_Download(sSourcesFilePath,sTargetFilePath+"\\TempLoadMO\\AutoLoad_TrayForm\\","txt")==false)
            {
                ShowMyMessage("自動下載工單失敗","AutoLoad_TrayForm Download Error");//Jimmychiu 20220901 修正異常顯示
                return false;
            }
        }

        if(IniConfig.bN14_18_EnableTempOffset==true)
        {
            MyForceDirectories(sTargetFilePath+"\\Taj_offset_Tool\\", "TfProductionInfo::LoadMOInformation_3");
            CheckFTPFilePath(sTempSourcesFilePath);        //JimmyChiu 20220111 check FTP Sources File Path
            if(FTP_MOFile_Download(sTempSourcesFilePath,sTargetFilePath+"\\Taj_offset_Tool\\","CSV")==false)
            {
                ShowMyMessage("下載檢查溫度補償失敗","Taj_offset_Tool Download Error");//Jimmychiu 20220901 修正異常顯示
                return false;
            }
        }
    }
    //<==
    //Sam 20170810  HT-7045 變數改為 HT-9045 變數

    //Sam 20200312 :  修改 MOFile 檢查方式
    //==>
    //Read MO Data
    AnsiString sMOFile="";
    sLocalMOFullFileName=sTargetFilePath+"\\TempLoadMO\\"+IniConfig.SocketHandlerID+".TXT";
    AnsiString sSeparatedValues="";
    if(FileExists(sLocalMOFullFileName)==true)
    {
        sMOFile=IniConfig.SocketHandlerID+".TXT";
    }
    else
    {
        ShowMyMessage("Find Load MO File Not .txt Error in LoadMOInformation FilePath="+sLocalMOFullFileName);//Jimmychiu 20220901 修正異常顯示
        return false;
    }
    //<==
    //Sam 20200312 :  修改 MOFile 檢查方式
    sSeparatedValues=":";
    //Sam 20171227 : Add CONTINUE Pass/Fail KaiChen 20171116 ：超豐 人員車規認證 (AUTOMOTIVE)
    //==>
    FileInfo *moFileInfo=new FileInfo(sLocalMOFullFileName);
    moFileInfo->Add(sLoadMO_MO,"MO");
    moFileInfo->Add(sLoadMO_TestTime,"T.T");
    moFileInfo->Add(sLoadMO_Temperature,"Temperature");
    moFileInfo->Add(sLoadMO_SoakTime,"Soak Time");
    moFileInfo->Add(sLoadMO_TestFlow,"INDEX");
    moFileInfo->Add(sLoadMO_PASSBIN,"PASSBIN");
    moFileInfo->Add(sLoadMO_InterfaceType,"Interface Type");
    moFileInfo->Add(sLoadMO_Handler,"Handler");
    moFileInfo->Add(sLoadMO_AUTOMOTIVE,"AUTO MOTIVE");
    moFileInfo->Add(sLoadMO_YieldLimit,"Yield Limit");
    moFileInfo->Add(sLoadMO_OSBin,"O/S Bin");
    moFileInfo->Add(sLoadMO_AlarmYield,"Alarm Yield");
    moFileInfo->Add(sLoadMO_Tester,"Tester");
    moFileInfo->Add(sLoadMO_Device,"Device");
    moFileInfo->Add(sLoadMO_CONTINUE_PASS,"CONTINUE PASS");
    moFileInfo->Add(sLoadMO_CONTINUE_FAIL,"CONTINUE FAIL");
    moFileInfo->Add(sLoadMOYiled_InterValTime,"Interval time");
    moFileInfo->Add(sLoadMOYiled_InterValCount,"Interval count");
    moFileInfo->Add(sLoadMOYiled_HW_BinNumber,"H/W BinNumber");
    moFileInfo->Add(sLoadMO_TemperatureMode,"Temperature mode");
    moFileInfo->Add(sLoadMO_RELEASE_SITE,"RELEASE SITE");
    moFileInfo->Add(sLoadMO_LOT,"LOT");
    moFileInfo->Add(sDevice_Pin_Count,"Device pin count");
    moFileInfo->Add(sDevice_Pin_Force,"Device pin force");
    moFileInfo->Add(sTrayForm,"Tray Form");
    moFileInfo->Add(sSiteMap,"Site Map");           //JimmyChiu 20221226 : SetUpConfiguration HandlerMode splite to SITE MAP and BIN CATEGORY
    moFileInfo->Add(sBinCategory,"Bin Category");   //JimmyChiu 20221226 : SetUpConfiguration HandlerMode splite to SITE MAP and BIN CATEGORY
    moFileInfo->Add(sNot_RT_BIN,"Not RT_BIN");       //JimmyChiu 20231127 : Set color sensor parameter by server
    if(moFileInfo->ReadFile(":"))
    {
    }
    else
    {
        ShowMyMessage("MO Data Error");
        return false;
    }
    delete moFileInfo;
    //<==
    //Sam 20171227 : Add CONTINUE Pass/Fail KaiChen 20171116 ：超豐 人員車規認證 (AUTOMOTIVE)

    #ifdef SOFT_SIMULTE                                                    //JimmyChiu 20220119 : ADD #ifdef SOFT_SIMULTE
    sLoadMO_InterfaceType="GPIB";
    #endif
    SetControlBin(sLocalMOFullFileName,sLoadMOYiled_HW_BinNumber,sLoadMOYiled_InterValTime,sLoadMOYiled_InterValCount);
    sLoadMO_MO=sLoadMO_MO.Trim();
    sLoadMO_YieldLimit=sLoadMO_YieldLimit.SubString(1,sLoadMO_YieldLimit.Pos("|")-1);

    dMOFileTemperature=atof(sLoadMO_Temperature.c_str());
    if(IniConfig.bN14_18_EnableTempOffset==true && dMOFileTemperature>IniConfig.iN14_17_AmbientULTemp)//Sam 20200525 : 正賢要求溫度 by Config 以下都是常溫。
    {
        AnsiString sFileName="",sFilePath=sTargetFilePath+"\\Taj_offset_Tool\\";
        int iErr=0;
        std::vector<AnsiString> fileNames =FileInfo().GetAllFileNamesInFolder(sFilePath,iErr);
        if(iErr==1)
        {
            ShowMyMessage("Folder path error \n\r"+sFilePath);
        }
        else
        {
            if(fileNames.size()==1)
            {
                for(int i=0; i<5; i++)
                {
                    TempTajList[i]=new TStringList();
                    TempTajList[i]->Clear();
                }

                TempTajList[3]->LoadFromFile(fileNames[0]);
                TempTajList[4]->Delimiter=',';
                for(int i=0; i<TempTajList[3]->Count; i++)
                {
                    //Device,Handler,Taj offset,
                    S1=TempTajList[3]->Strings[i].c_str();
                    TempTajList[4]->DelimitedText=S1;
                    if(TempTajList[4]->Count>=3)
                    {
                        sTempDevice=TempTajList[4]->Strings[0].c_str();
                        if(sTempDevice==sLoadMO_Device) //Taj_offset_Tool_Database 裡的 Device 與 MOFile 的 Device 相同的才需要讀出來
                        {
                            sTempHandler=TempTajList[4]->Strings[1].c_str();
                            sTempTaj_offset=TempTajList[4]->Strings[2].c_str();
                            TempTajList[0]->Add(sTempDevice);
                            TempTajList[1]->Add(sTempHandler);
                            TempTajList[2]->Add(sTempTaj_offset);
                        }
                    }
                }
                bFind=false;
                if(TempTajList[1]->Count>0)//有找到 Device 再去找 Handler ID
                {
                    for(int i=0; i<TempTajList[1]->Count; i++)
                    {
                        if(TempTajList[1]->Strings[i]==sLoadMO_Handler)
                        {
                            dTempTajOfs=StrToFloat(TempTajList[2]->Strings[i].c_str());
                            bFind=true;
                            break;
                        }
                    }
                }
                else
                {
                    bFind=true;
                    dTempTajOfs=0; //找不到 Device 就不用補 TajOfs
                }

                if(abs(dTempTajOfs)>10.0)
                {
                     ShowMyMessage("Taj Offset Temputer over setting. Can't production.");
                     return false;
                }

                for(int i=0; i<5; i++)
                {
                    TempTajList[i]->Clear();
                    delete TempTajList[i];
                }

                if(bFind==false)
                {
                    ShowMyMessage("Device model in the file but there is no Handler number of this machine. Can't production.");
                    return false;
                }
            }
            else
            {
                ShowMyMessage("Download taj database file is error in LoadMOInformation");
                return false;
            }
        }
    }
    else
    {
        dTempTajOfs=0.0;
    }

    if(bCheckMoInforOnly==true)
    {
        return true;
    }

    fLastTestTime=atof(sLoadMO_TestTime.c_str()); //Mylin 20170515 Modify OEE Function

    //Back Up File
    if(_bOEEStartLotSuccess==false)
    {
        fLotInfo->ed_PIOEEMO->Text=GetScheduleName();                           //Jimmychiu 20240119 : ScheduleName = Mo+Flow
        OEE_SetMO(fLotInfo->ed_PIOEEMO->Text);                                  //Sam 20170801 HT-7045 變數改為 HT-9045 變數
        OEE_SetHandlerID(IniConfig.SocketHandlerID);                            //Sam 20170801 HT-7045 變數改為 HT-9045 變數
        _sOEE_DirectoryName=Now().FormatString("yyyymmdd")+"_"+Now().FormatString("hhnnss")+"_"+_sOEE_MO; //資料夾名稱 YYYYMMDD_HHMMSS_MO
    }

    TStringList *slCopyFile;
    slCopyFile=new TStringList;
    try
    {
        if(FileExists(sLocalMOFullFileName)==true)
        {
            AnsiString sBackUpFilePath="";
            sBackUpFilePath=sProductionInfoFilePath+"\\"+sLoadMO_MO+"\\"+ _sOEE_DirectoryName+"\\";
            slCopyFile->Clear();
            slCopyFile->LoadFromFile(sLocalMOFullFileName);
            MyForceDirectories(sBackUpFilePath);
            slCopyFile->SaveToFile(sBackUpFilePath+sMOFile);
        }
        else
        {
            ShowMyMessage("Copy Load MO File Error in LoadMOInformation");
            delete slCopyFile;
            return false;
        }
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfProductionInfo::LoadMOInformation");
        ShowMyMessage("Copy Load MO File Error in LoadMOInformation");
        delete slCopyFile;
        return false;
    }

    delete slCopyFile;
    return true;
}
//---------------------------------------------------------------------------
//Sam 20180423 (wei) : MOFile of Yeild Download
//==>
bool TfProductionInfo::LoadYiedlInformation()
{
    AnsiString sFileName="";
    AnsiString sSourcesFilePath="";
    sSourcesFilePath=IniConfig.asN14_4_MODownloadPath;
    #ifdef SOFT_SIMULTE
    sSourcesFilePath="//handler_data//SETUP_FILE//";
    #endif
    AnsiString sTargetFilePath=GetProInfoFilePath()+"\\TempLoadMO\\IPSC\\";

    //MOFile Source Paht Check
    if(_sOEE_HandlerID.Trim()=="")
    {
        ShowMyMessage("Machine ID is Empty in LoadMOInformation");
        return false;
    }

    CheckFTPFilePath(sSourcesFilePath);              //JimmyChiu 20220111 修正 FTP Sources File Path bug "\\" 字串長度為2
    sSourcesFilePath=sSourcesFilePath+_sOEE_HandlerID+"//IPSC";

    //MOFile Target Paht Check
    MyForceDirectories(sTargetFilePath, "TfProductionInfo::LoadYiedlInformation");
    //Delete All File in sTargetFilePath
    if(DeleteAllFileInDirectory(sTargetFilePath)==false)
    {
        return false;
    }
    //Download Load Yiled File to sTargetFilePath
    CheckFTPFilePath(sSourcesFilePath);                                  //JimmyChiu 20220111 check FTP Sources File Path
    if(FTP_MOFile_Download(sSourcesFilePath,sTargetFilePath,"txt")==false)//Jimmychiu 20220901 新增FTP下載判斷條件
    {
        AnsiString sMsg="IPSC Download Error! "+sSourcesFilePath;
        NewRecordProcess("", "LoadYiedlInformation() Error", sMsg);
//        ShowMyMessage("IPSC下載失敗","IPSC Download Error!"+sSourcesFilePath);//Jimmychiu 20220901 修正異常顯示
        return false;
    }

    AnsiString sFindFile=sTargetFilePath+"*.*";
    bool bFindFile=false;
    AnsiString sMOFile="";
    TSearchRec srFile;
    if(FindFirst(sFindFile,0,srFile)==0)
    {
        do
        {
            if(bFindFile==true)
            {
                ShowMyMessage("Find Multiple Load MO Yield File Error in LoadMOInformation");
                return false;
            }
            bFindFile=true;
            sMOFile=srFile.Name;
            if(sMOFile.SubString(sMOFile.Length()-3,4).UpperCase()!=".TXT")
            {
                ShowMyMessage("Find Load MO Yield File Not .txt Error in LoadMOInformation FileName="+sMOFile);//Jimmychiu 20220901 修正異常顯示
                return false;
            }
        }
        while(FindNext(srFile)==0);
    }
    //Read MO Yield Data
    AnsiString sFullFileName=sTargetFilePath+sMOFile;
    FileInfo* moFileInfo=new FileInfo(sFullFileName);
    moFileInfo->Add(sLoadMOYiled_Qty,"IPSC Qty");
    moFileInfo->Add(sLoadMOYiled_AVG_Alarm_Yield,"IPSC AVG alarm Yield");
    moFileInfo->Add(sLoadMOYiled_AVG_Reject_Yield,"IPSC AVG reject Yield");
    moFileInfo->Add(sLoadMOYiled_OS_rate,"IPSC OS rate");
    moFileInfo->Add(sLoadMOYiled_OS_rate_Rej,"IPSC OS rate Rej");
    moFileInfo->Add(sLoadMOYiled_Different_Alarm_Yield,"IPSC Different Alarm Yield");
    moFileInfo->Add(sLoadMOYiled_Different_Alarm_Yield_Rej,"IPSC Different Alarm Yield Rej");
    moFileInfo->Add(sLoadMOYiled_Yield_Difference,"Previous yield difference of IPSC");
    moFileInfo->Add(sLoadMOYiled_Yield_Difference_Rej,"Previous yield difference of IPSC Rej");
    moFileInfo->ReadFile(":");
    delete moFileInfo;
    return true;
}
//<==
//Sam 20180423 (wei) : MOFile of Yeild Download
//---------------------------------------------------------------------------
AnsiString TfProductionInfo::GetStringBySeparatedValues(AnsiString sFullFileName,AnsiString sItemValue,AnsiString sSeparatedValues)
{
    TStringList *slSourceFile;
    AnsiString sDataList="";
    AnsiString sGetString="";

    if(sFullFileName=="" || FileExists(sFullFileName)==false)
    {
        if(bShow)  //Sam 20200225 : OEE 功能防呆保護提示顯示在畫面，不要用 MessageBox。
        {
            pn_ErrorMsg_HALTStatus->Caption = "File Name Error!";
            pn_ErrorMsg_PauseStatus->Caption = "File Name Error!";
        }else
        {
            ShowMyMessage("File Name Error!");
        }
        return sGetString;
    }

    slSourceFile=new TStringList();
    slSourceFile->LoadFromFile(sFullFileName);
    for(int i=0; i<slSourceFile->Count; i++)
    {
        int iPos=-1;
        int iStart=-1;

        sDataList=slSourceFile->Strings[i];
        iPos=sDataList.Pos(sItemValue+sSeparatedValues);
        iStart=sItemValue.Length()+sSeparatedValues.Length()+1;
        if(iPos==1)
        {
            sGetString=sDataList.SubString(iStart,sDataList.Length());
            break;
        }
    }
    delete slSourceFile;
    return sGetString;
}
//---------------------------------------------------------------------------
//Delete All File In Directory
bool TfProductionInfo::DeleteAllFileInDirectory(AnsiString sDirectoryName)
{
    bool bBack=true;
    int iErr=0;
    FileInfo().DeleteFolderContents(sDirectoryName,iErr);
    if(iErr==1)
    {
        ShowMyMessage("Folder path error \n\r"+sDirectoryName);
        bBack=false;
    }
    else if(iErr==2) //delete fail
    {
        ShowMyMessage("Delete file error \n\r"+sDirectoryName);
        bBack=false;
    }
    return bBack;
}
//---------------------------------------------------------------------------
void TfProductionInfo::ShowInformationForm()                                    //JimmyChiu 20230503 : 新增Infomation顯示function
{
    ShowSheet(tsInformation);
}
//---------------------------------------------------------------------------
void TfProductionInfo::ShowHaltStatusForm()
{
    EachCycleSecondDo_SaveAndUpdateOEEFiles(true);
    bStartNeedSaveAndUpdateOEEFiles=true;
    SystemStart=false;
    bIsPauseTime=false;
    ShowSheet(tsHaltStatusForm);
}
//---------------------------------------------------------------------------
void TfProductionInfo::ShowPauseStatusForm()
{
    SystemStart=false;
    bIsPauseTime=true;
    ShowSheet(tsPauseStatusForm);
}
//---------------------------------------------------------------------------
int TfProductionInfo::ShowCompareDataForm()                                    //JimmyChiu 20230504 : 新增資料比對顯示function
{
    bAgreeExportConfig=false;
    return ShowSheet(tsCompareData);
}
//---------------------------------------------------------------------------
void TfProductionInfo::SetStartStatus()
{
    _sOEE_ActivityID="";
    _sOEE_Status="Auto Run";
}
//---------------------------------------------------------------------------
void __fastcall TfProductionInfo::edInsertOPID_HALTClick(TObject *Sender)
{
    edInsertOPID_HALT->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TfProductionInfo::sbScreenkeyboard_HALTClick(TObject *Sender)
{
    ScreenkeyboardShow();                                                       //Sam 20170801 移植 HT-7045 Main 函式到 ProductionInfo
}
//---------------------------------------------------------------------------
void __fastcall TfProductionInfo::Button2Click(TObject *Sender)
{
    if(CheckCloseInfo()==true)
        Close();
}
//---------------------------------------------------------------------------
bool TfProductionInfo::LoadHaltAndPauseSelectStatusName()
{
    bool bHas=true;
    AnsiString sHaltSelectStatusName=GetProInfoFilePath()+"\\PI_HaltSelectStatusName.ini";
    if(FileInfo().IsFilePathExist(sHaltSelectStatusName))
    {
        TSpeedButton *SpeedButton_Halt[]={  SpeedButton_Halt00,SpeedButton_Halt01,SpeedButton_Halt02,SpeedButton_Halt03,
                                            SpeedButton_Halt04,SpeedButton_Halt05,SpeedButton_Halt06,SpeedButton_Halt07,
                                            SpeedButton_Halt08,SpeedButton_Halt09,SpeedButton_Halt10,SpeedButton_Halt11,
                                            SpeedButton_Halt12,SpeedButton_Halt13,SpeedButton_Halt14,SpeedButton_Halt15};
        TStringList *slHaltSelectStatusName=new TStringList();
        slHaltSelectStatusName->Clear();
        slHaltSelectStatusName->LoadFromFile(sHaltSelectStatusName);
        for(int i=0; i<16; i++)
        {
            SpeedButton_Halt[i]->Visible=false;
            SpeedButton_Halt[i]->Hint="";
            SpeedButton_Halt[i]->Caption="";

            if(i<slHaltSelectStatusName->Count)
            {
                AnsiString sName=slHaltSelectStatusName->Strings[i];
                SpeedButton_Halt[i]->Hint=sName;
                SpeedButton_Halt[i]->Caption=sName.SubString(1,sName.Pos(" "))+AnsiString(char(13))+sName.SubString(sName.Pos(" ")+1,sName.Length());
                SpeedButton_Halt[i]->Visible=true;
            }
        }
        slHaltSelectStatusName->Clear();
        delete slHaltSelectStatusName;
    }
    else
    {
        ShowMyMessage("Loss file PI_HaltSelectStatusName.ini",sHaltSelectStatusName);
        bHas=false;
    }

    AnsiString sPauseSelectStatusName=GetProInfoFilePath()+"\\PI_PauseSelectStatusName.ini";
    if(FileInfo().IsFilePathExist(sPauseSelectStatusName))
    {
        TSpeedButton *SpeedButton_Pause[]={ SpeedButton_Pause00,SpeedButton_Pause01,SpeedButton_Pause02,SpeedButton_Pause03,
                                            SpeedButton_Pause04,SpeedButton_Pause05,SpeedButton_Pause06,SpeedButton_Pause07,
                                            SpeedButton_Pause08,SpeedButton_Pause09,SpeedButton_Pause10,SpeedButton_Pause11,
                                            SpeedButton_Pause12,SpeedButton_Pause13,SpeedButton_Pause14,SpeedButton_Pause15};
        TStringList *slPauseSelectStatusName=new TStringList();
        slPauseSelectStatusName->Clear();
        slPauseSelectStatusName->LoadFromFile(sPauseSelectStatusName);
        for(int i=0; i<16; i++)
        {
            SpeedButton_Pause[i]->Visible=false;
            SpeedButton_Pause[i]->Hint="";
            SpeedButton_Pause[i]->Caption="";

            if(i<slPauseSelectStatusName->Count)
            {
                AnsiString sName = slPauseSelectStatusName->Strings[i];
                SpeedButton_Pause[i]->Hint=sName;
                SpeedButton_Pause[i]->Caption=sName.SubString(1,sName.Pos(" "))+AnsiString(char(13))+sName.SubString(sName.Pos(" ")+1,sName.Length());
                SpeedButton_Pause[i]->Visible = true;
            }
        }
        slPauseSelectStatusName->Clear();
        delete slPauseSelectStatusName;
    }
    else
    {
        ShowMyMessage("Loss file PI_PauseSelectStatusName.ini",sPauseSelectStatusName);
        bHas=false;
    }
    return bHas;
}
//---------------------------------------------------------------------------
void __fastcall TfProductionInfo::SpeedButton_Pause00Click(TObject *Sender)
{
    TSpeedButton *SpeedButton_Pause;
    SpeedButton_Pause=(TSpeedButton *)Sender;
    lbStatus_Pause->Caption=SpeedButton_Pause->Hint;
    sPauseButtonCaption=SpeedButton_Pause00->Hint;
}
//---------------------------------------------------------------------------
void __fastcall TfProductionInfo::SpeedButton_Halt00Click(TObject *Sender)
{
    TSpeedButton *SpeedButton_Halt;
    SpeedButton_Halt=(TSpeedButton *)Sender;
    lbStatus_HALT->Caption=SpeedButton_Halt->Hint;
}
//---------------------------------------------------------------------------
void __fastcall TfProductionInfo::PageControl1Change(TObject *Sender)
{
    if(tsHaltStatusForm->TabVisible==true && PageControl1->ActivePage==tsHaltStatusForm)
    {
        edInsertOPID_HALT->SetFocus();
        SetInsertOPIDStr("");
    }

    if(tsPauseStatusForm->TabVisible==true && PageControl1->ActivePage==tsPauseStatusForm)
    {
        edInsertOPID_Pause->SetFocus();
        edInsertOPID_Pause->Text="";
    }
}
//---------------------------------------------------------------------------
void TfProductionInfo::ScreenkeyboardShow()
{
    HWND hKeyboard = FindWindow(NULL, "On-Screen Keyboard"); // 英文系統
    if (hKeyboard == NULL)
    {
        hKeyboard = FindWindow(NULL, "螢幕小鍵盤"); // 中文系統
    }

    if (hKeyboard != NULL)
    {
        SetForegroundWindow(hKeyboard);         // 如果找到鍵盤窗口，將其推送到畫面最上層
    }
    else
    {
        SHELLEXECUTEINFO execinfo;             // 如果沒找到，啟動螢幕小鍵盤
        memset(&execinfo, 0, sizeof(execinfo));
        execinfo.cbSize = sizeof(execinfo);
        execinfo.lpVerb = "open";
        execinfo.lpFile = "C:\\Windows\\system32\\osk.exe";
        if (FileExists(execinfo.lpFile))
        {
            execinfo.lpParameters = "";
            execinfo.fMask = SEE_MASK_NOCLOSEPROCESS;
            execinfo.nShow = SW_SHOWDEFAULT;
            ShellExecuteEx(&execinfo);
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfProductionInfo::OEEFTPList(AnsiString Listing)
{
    fConfiguration->cbbN14_10_OEEFTPDownloadList->Items->Add(Listing);
}
//------------------------------------------------------------------------------
bool __fastcall TfProductionInfo::FTP_OEEFile_Upload(AnsiString sSourcesFilePath, AnsiString sTargetFilePath, AnsiString sULFileName)
{
    AnsiString sMsg="", sUploadFile;
    AnsiString sTempSource="",sTempTarget="",sTempULFuleName="";

    //檢查檔案路徑是否為空值
    if(sSourcesFilePath=="" && sTargetFilePath!="")
    {
        sMsg="Handler OEE Source FilePatch Empty Errot!";
        ShowMyMessage(sMsg);
        return false;
    }
    else if(sSourcesFilePath!="" && sTargetFilePath=="")
    {
        sMsg="Handler OEE Target FilePatch Empty Errot!";
        ShowMyMessage(sMsg);
        return false;
    }
    else if(sSourcesFilePath!="" && sTargetFilePath!="") //有路徑資料放到 List
    {
        if(sSourceList==NULL)
        {
            sSourceList=new TStringList();
        }

        if(sTargetSList==NULL)
        {
            sTargetSList=new TStringList();
        }

        if(sULFileNameList==NULL)
        {
            sULFileNameList=new TStringList();
        }

        CheckFTPFilePath(sTargetFilePath);             //JimmyChiu 20220111 check FTP Sources File Path
        sSourceList->Add(sSourcesFilePath);
        sTargetSList->Add(sTargetFilePath);
        if(sULFileName!="")
        {
            sULFileNameList->Add(sULFileName);
        }
        else
        {
            sULFileNameList->Add("NULL");
        }
        return true;
    }

    if(sSourceList==NULL || sTargetSList==NULL || sULFileNameList==NULL)
    {
        return true;
    }

    if(sSourceList->Count==0 || sTargetSList->Count==0 || sULFileNameList->Count==0)
    {
        sSourceList->Clear();
        sTargetSList->Clear();
        sULFileNameList->Clear();
        return true;
    }

    if(fContact->fShow==true)  //Sam 20191001 : 開啟 Contact 介面時不要上傳 FTP 會死掉。
        return true;

    //防止連續 FTP 連續
    if(bOEEFTPAction==true)
        return true;
    bOEEFTPAction=true;

    if(sSourceList->Count>0 && sTargetSList->Count>0 && sULFileNameList->Count>0)
    {
        sTempSource=sSourceList->Strings[0];
        sSourceList->Delete(0);

        sTempTarget=sTargetSList->Strings[0];
        sTargetSList->Delete(0);

        sTempULFuleName=sULFileNameList->Strings[0];
        sULFileNameList->Delete(0);
    }

    AnsiString asUserID="",asPassword="",asHost="";
    GetFTP_Setting(asUserID,asPassword,asHost);     //Jimmychiu 20240104 : 優化FTP連線Function
    TfFTP fFTP(asUserID,asPassword,asHost);
    if(fFTP.Connect())
    {
        AnsiString asError=AnsiString("");
        fFTP.CheckLocalFilePath(sTempSource);
        if(sTempULFuleName!="NULL" && sTempULFuleName!="") //Steven 20190621 : OEE Function
        {
            AnsiString sFindFile=sTempSource+sTempULFuleName;
            if(FileExists(sFindFile))
                fFTP.Upload(sTempSource,sTempTarget,sTempULFuleName,asError);
            else
                ShowMyMessage("Can not find : "+sFindFile);
        }
        else
        {
            //上傳路徑內所有檔案
            AnsiString sFindFile=sTempSource + "*.*";
            TSearchRec srFile;
            if(FindFirst(sFindFile, 0, srFile)==0)
            {
                do
                {
                    AnsiString sUploadFile=srFile.Name;
                    if(FileExists(sTempSource+sUploadFile))                     //Sam 20180802 (wei) : 增加保護
                        fFTP.Upload(sTempSource,sTempTarget,sUploadFile,asError);
                    else
                        ShowMyMessage("Can not find : "+sTempSource+sUploadFile);
                    MySleep(1);
                }
                while(FindNext(srFile)==0);
            }
            FindClose(srFile);
        }
    }
    else
    {
        MyDBIProcess("Exception", "TfProductionInfo::FTP_OEEFile_Upload");
        ShowMyMessage("FTP_OEEFile_Upload Connect Failed");
    }
    bOEEFTPAction=false;
    return true;
}
//---------------------------------------------------------------------------
bool __fastcall TfProductionInfo::FTP_MOFile_Download(AnsiString sSourcesFilePath,AnsiString sTargetFilePath,AnsiString asFilter)
{
    if(bOEEFTPAction==true)
        return false;
    AnsiString sMsg="";
    bOEEFTPAction=true;
    if(sSourcesFilePath.Trim()=="" || sTargetFilePath.Trim()=="")
    {
        bOEEFTPAction=false;
        sMsg="Handler OEE FilePath or FileName Empty Error!";
        ShowMyMessage(sMsg);
        return false;
    }
    AnsiString asUserID="",asPassword="",asHost="";
    GetFTP_Setting(asUserID,asPassword,asHost);//Jimmychiu 20240104 : 優化FTP連線Function
    TfFTP fFTP(asUserID,asPassword,asHost);
    if(fFTP.Connect())
    {
        AnsiString asError=AnsiString("");
        if(fFTP.DownloadFilterFile(sSourcesFilePath,sTargetFilePath,asFilter,asError))
        {
            bOEEFTPAction=false;
            return true;
        }
    }
    else
    {
        MyDBIProcess("Exception", "TfProductionInfo::FTP_MOFile_Download::Connect Failed");
        ShowMyMessage("FTP Connect Failed Name="+asUserID+" Password="+asPassword+" Host="+asHost);//Jimmychiu 20220901 修正異常顯示
    }
    bOEEFTPAction=false;
    return false;
}
//---------------------------------------------------------------------------
//Sam 20190227 : Try Catch Mo Download SetupFile
//==>
bool TfProductionInfo::AutoDownloadSetupFileByMO()
{
    if(IniConfig.bN14_10_DownFileByMO==false)
    {
        return true;
    }

    if(IniConfig.bEnableFTP==false)
    {
        ShowMyMessage("FTPSetupFileFunction Not Enable in AutoDownloadSetupFileByMO");
        return false;
    }

    if(sLoadMO_SetupFileName.Length() == 0)
    {
        ShowMyMessage("sLoadMO_SetupFileName Empty Error in AutoDownloadSetupFileByMO");
        return false;
    }
    bAutoDownloadSetupFile=true;
    fFTPClient->ShowFTPModal(0);
    if(bAutoDownloadSetupFile==true)
    {
        ShowMyMessage("SetupFileName Can Not Find in Refresh List in AutoDownloadSetupFileByMO");
        return false;
    }
    return true;
}
//<==
//Sam 20190227 : Try Catch Mo Download SetupFile
//---------------------------------------------------------------------------
void TfProductionInfo::SetLastBinQty()          //Steven 20190621 : OEE Function
{
    for(int iCat=0; iCat<iTestBinCount; iCat++)    //每個category
    {
        LastSet.OEEBinCT[iCat]=TastCategory.iTotalCategory[iCat];
    }

    for(int iAuto=0; iAuto<10; iAuto++)
    {
        LastSet.OEETrayCT[iAuto]=LastSet.BinCT[0][iTo3Unload[iAuto]];
    }
}
//------------------------------------------------------------------------------
void TfProductionInfo::SaveBinQTYReport(AnsiString sUploadFileName)     //Steven 20190621 : OEE Function
{
    if(IniConfig.bN14_13_ULBinQtyToFTP==false ||
       IniConfig.iN14_13_ULBinQtyInterval<=0 ||
       IniConfig.asN14_13_ULBinQtyPath=="")
    {
        return;
    }
    AnsiString sTemp="", sTitle="";
    TStringList *slBinQtyReport=new TStringList();
    AnsiString sFilePath="D:\\HT9045_log\\ProductionInfo\\TestBinQuantityReport\\";
    AnsiString sFilePathName="";

    MyForceDirectories(sFilePath, "TfProductionInfo::SaveBinQTYReport");
    if(sFilePath.SubString(sFilePath.Length(), 1)=="\\")
    {
        sFilePathName=sFilePath+sUploadFileName;
    }
    else
    {
        sFilePathName=sFilePath+"\\"+sUploadFileName;
    }
    slBinQtyReport->Clear();
    TastCategory.UpdataCount(true);                                             //Steven 20250514 : 統一計算數量
    sTemp="Tester Bin";

    for(int iCat=0; iCat<32; iCat++)                                            //Sam 20190801 : 強制險顯示32
    {
        sTitle+=",Bin"+AnsiString(iCat);
        if(TastCategory.iTotalCategory[iCat]>LastSet.OEEBinCT[iCat])
        {
            sTemp+=","+AnsiString(TastCategory.iTotalCategory[iCat]-LastSet.OEEBinCT[iCat]);
        }
        else
        {
            sTemp+=",0";
        }
        LastSet.OEEBinCT[iCat]=TastCategory.iTotalCategory[iCat];
    }
    slBinQtyReport->Add(sTitle);
    slBinQtyReport->Add(sTemp);
    sTemp="Handler Bin";
    for(int iAuto=0; iAuto<10; iAuto++)
    {
        if(LastSet.BinCT[0][iTo3Unload[iAuto]]>(unsigned int)LastSet.OEETrayCT[iAuto])
        {
            sTemp+=","+AnsiString(LastSet.BinCT[0][iTo3Unload[iAuto]]-LastSet.OEETrayCT[iAuto]);
        }
        else
        {
            sTemp+=",0";
        }
        LastSet.OEETrayCT[iAuto]=LastSet.BinCT[0][iTo3Unload[iAuto]];
    }
    slBinQtyReport->Add(sTemp);
    slBinQtyReport->SaveToFile(sFilePathName);
    delete slBinQtyReport;
}
//------------------------------------------------------------------------------
void TfProductionInfo::UploadBinQtyReport(bool bSaveNow)        //Steven 20190621 : OEE Function
{
    AnsiString sFLOW="";
    AnsiString sMO="";
    if(IniConfig.bN14_13_ULBinQtyToFTP==false ||
       IniConfig.iN14_13_ULBinQtyInterval<=0  ||
       IniConfig.asN14_13_ULBinQtyPath=="")
    {
        return;
    }

    static bool bTimerStart=false;
    static bool bBinQtyCycleTimeUp=false;

    if(bTimerStart==false)
    {
        bTimerStart=true;
        BinQtyTimer.SetSecAndOn(IniConfig.iN14_13_ULBinQtyInterval);
    }
    else if(bSaveNow ||
            (bTimerStart && BinQtyTimer.Off()))
    {
        bTimerStart=false;
        bBinQtyCycleTimeUp=true;
    }

    AnsiString sFileName="";
    AnsiString sFilePath=sProductionInfoFilePath+"\\TestBinQuantityReport\\";
    if(bBinQtyCycleTimeUp==true)
    {
        if(_sOEE_MO=="")
        {
            _sOEE_MO="_";
        }

        if(_sOEE_FLOW=="")
        {
            _sOEE_FLOW="_";
        }

        sFileName.sprintf("%s_%s_%s_%s.csv",
                                IniConfig.SocketHandlerID,
                                _sOEE_MO,
                                _sOEE_FLOW,
                                Now().FormatString("yyyymmdd_hhnnss"));
        try
        {
            SaveBinQTYReport(sFileName);
            FTP_OEEFile_Upload(sFilePath, IniConfig.asN14_13_ULBinQtyPath, sFileName);
        }
        catch(...)
        {
            bBinQtyCycleTimeUp=false;
        }
        bBinQtyCycleTimeUp=false;
    }
}
//---------------------------------------------------------------------------
void TfProductionInfo::UploadTrayMappingLog()   //Sam 20201209 : 增加資料上傳
{
    AnsiString sTargetLocalFilePath=sProductionInfoFilePath+"\\"+sLoadMO_MO+"\\"+_sOEE_DirectoryName+"\\";
    AnsiString sTargetNetworkFilePath=IniConfig.asN14_19_TrayMappingPath;
    AnsiString sTargetFile="";
    AnsiString sSourceFilePath="D:\\HT9045_Log\\ProductionInfo\\TrayMapping\\";
    AnsiString sSourceFileName="TrayMappingTemp.csv";

    if(IniConfig.bN14_19_TrayMappingToFTP==false ||
       sTargetNetworkFilePath=="")
    {
        return;
    }
    //檢查目標路徑最後必需為 "/"
    if(sTargetNetworkFilePath.SubString(sTargetNetworkFilePath.Length(),1)!="/")
    {
        sTargetNetworkFilePath=sTargetNetworkFilePath+AnsiString("/");
    }
    //機號_MO_yyyymmdd_hhnnss_TrayMap
    sTargetFile.sprintf("%s_%s_%s_%04d%02d%02d_%02d%02d%02d_TrayMap.csv",sLoadMO_Handler,sLoadMO_MO, sLoadMO_TestFlow, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);

    if(FileExists(sSourceFilePath+sSourceFileName))
    {
        CopyFile((sSourceFilePath+sSourceFileName).c_str(),(sTargetLocalFilePath    +sTargetFile).c_str(),false);  //存本機一份
        #ifndef SOFT_SIMULTE
        CopyFile((sSourceFilePath+sSourceFileName).c_str(),(sTargetNetworkFilePath  +sTargetFile).c_str(),false);  //用網路芳鄰上傳
        #endif
        DeleteFile(sSourceFilePath+sSourceFileName);
    }
}
//---------------------------------------------------------------------------
void TfProductionInfo::ClearTrayCnt()
{
    for(int i=0; i<256; i++)
    {
//        LastSet.iN14_9_TrayICCnt[i]         =0;                                 //Tested IC
        LastSet.iN14_9_FullTrayCnt[i]       =0;                                 //Full Tray Tray Qty
        LastSet.iN14_9_PartialTrayCnt[i]    =0;                                 //Not Full Tray Tray Qty
        LastSet.iN14_9_PartialTrayICCnt[i]  =0;                                 //Not Full Tray IC Qty
        LastSet.iN14_9_InHandlerICCnt[i]    =0;                                 //In Machine Tray IC Qty
    }
}
//------------------------------------------------------------------------------
void TfProductionInfo::CalICCountInHandler()
{
    for(int i=0; i<eTrayCount; i++)
    {
        if(MOT[iMMAuto[i]].fHasTray==true)
        {
            LastSet.iN14_9_InHandlerICCnt[i]=MOT[iMMAuto[i]].HowManyDevice();   //In Machine Tray IC Qty
        }
    }
}
//---------------------------------------------------------------------------
void TfProductionInfo::CalTrayICCount(int iWitchTray)
{
    if(iWitchTray<0 || iWitchTray>eTrayCount)
    {
        return;
    }

    int iAuto=iMMAuto[iWitchTray];
    if(IniConfig.bN14_9_ULQtyReport==true)
    {
        if(MOT[iAuto].fHasTray==true)
        {
            if(MOT[iAuto].FullIC()==true)
            {
                LastSet.iN14_9_FullTrayCnt[iWitchTray]++;                       //Full Tray Tray Qty
            }
            else
            {
                if(MOT[iAuto].HowManyDevice()>0)
                {
                    LastSet.iN14_9_PartialTrayCnt[iWitchTray]++;                //Not Full Tray Tray Qty
                }
                LastSet.iN14_9_PartialTrayICCnt[iWitchTray]+=MOT[iAuto].HowManyDevice(); //Not Full Tray IC Qty
            }
        }
    }
}
//---------------------------------------------------------------------------
void TfProductionInfo::UpdateTrayCntReport()
{
    if(IniConfig.bN14_9_ULQtyReport==false ||
       IniConfig.asN14_9_ULQtyReportPath=="")
    {
        return;
    }

    AnsiString sListData = "";
    TStringList *slDQCData = new TStringList;
    int iQty=0;
    int iTotalQty=0;
    int iToErrorBin=0;

    slDQCData->Clear();

    CalICCountInHandler();

    sListData = "<<- Bin Information Start ->>";
    slDQCData->Add(sListData);

    sListData = "<HandlerID>     ";
    sListData = sListData + sLoadMO_Handler; //分類機
    slDQCData->Add(sListData);

    sListData = "<MO>            ";
    sListData = sListData + sLoadMO_MO; //製單
    slDQCData->Add(sListData);

    sListData = "<HandlerType>   ";
    sListData = sListData + sLoadMO_InterfaceType;
    slDQCData->Add(sListData);

    sListData = "<HandlerMode>   ";
    sListData = sListData + "";
    slDQCData->Add(sListData);

//---Qty---------------------------------------------------------------------->>
    std::vector<int> lsBin0_8=GetBin0_8List();
    for(unsigned int iBin=0; iBin<lsBin0_8.size(); iBin++) //<RealBin1>
    {
        iQty=0;
        sListData.sprintf("<RealBin%d>      ", iBin+1);
        if(iBin<6)
        {
            if(iBin==5)
            {
                iToErrorBin=LastSet.BinCT[0][lsBin0_8[iBin]];//Tested IC
            }
            else
            {
                iQty=LastSet.BinCT[0][lsBin0_8[iBin]];       //Tested IC
            }
        }
        iTotalQty+=iQty;
        sListData=sListData+AnsiString(iQty);
        slDQCData->Add(sListData);
    }

    for(int i=0; i<7; i++) //<RealEcode1>
    {
        iQty=0;
        sListData.sprintf("<RealEcode%d>    ", i+1);
        if(i==1)
        {
            iQty=iToErrorBin;
        }
        sListData=sListData+AnsiString(iQty);
        slDQCData->Add(sListData);
    }

    iQty=0;
    sListData="<RealTotal>     ";
    sListData=sListData+AnsiString(iTotalQty);
    slDQCData->Add(sListData);

    for(int i=0; i<8; i++) //<RealFTBin1>
    {
        iQty=0;
        sListData.sprintf("<RealFTBin%d>    ", i+1);
        if(i<6)                                                   //QQQ
        {
            if(i==5)
            {
                iToErrorBin=LastSet.iN14_9_FullTrayCnt[i];
            }
            else
            {
                iQty=LastSet.iN14_9_FullTrayCnt[i]; //Full Tray Tray Qty
            }
        }
        iTotalQty+=iQty;
        sListData=sListData+AnsiString(iQty);
        slDQCData->Add(sListData);
    }

    for(int i=0; i<7; i++) //<RealFTEcode1>
    {
        iQty=0;
        sListData.sprintf("<RealFTEcode%d>  ", i+1);

        if(i==1)
        {
            iQty=iToErrorBin;
        }
        sListData = sListData + AnsiString(iQty);
        slDQCData->Add(sListData);
    }

    for(int i=0; i<8; i++) //<RealTPBin1>
    {
        iQty=0;
        sListData.sprintf("<RealTPBin%d>    ", i+1);
        if(i<6)
        {
            if(i==5)
            {
                iToErrorBin=LastSet.iN14_9_PartialTrayCnt[i];
            }
            else
            {
                iQty=LastSet.iN14_9_PartialTrayCnt[i]; //Not Full Tray Tray Qty
            }
        }
        iTotalQty+=iQty;
        sListData=sListData+AnsiString(iQty);
        slDQCData->Add(sListData);
    }

    for(int i=0; i<7; i++) //<RealTPEcode1>
    {
        iQty=0;
        sListData.sprintf("<RealTPEcode%d>  ", i+1);
        if(i==1)
        {
            iQty=iToErrorBin;
        }
        sListData=sListData+AnsiString(iQty);
        slDQCData->Add(sListData);
    }

    for(int i=0; i<8; i++) //<RealTEABin1>
    {
        iQty=0;
        sListData.sprintf("<RealTEABin%d>    ", i+1);
        if(i<6)
        {
            if(i==5)
            {
                iToErrorBin=LastSet.iN14_9_PartialTrayICCnt[i]; //Tested IC
            }
            else
            {
                iQty=LastSet.iN14_9_PartialTrayICCnt[i]; //Not Full Tray IC Qty
            }
        }
        iTotalQty+=iQty;
        sListData=sListData+AnsiString(iQty);
        slDQCData->Add(sListData);
    }

    for(int i=0; i<7; i++) //<RealTEAEcode1>
    {
        iQty=0;
        sListData.sprintf("<RealTEAEcode%d> ", i+1);
        if(i==1)
        {
            iQty=iToErrorBin;
        }
        sListData=sListData+AnsiString(iQty);
        slDQCData->Add(sListData);
    }

    for(int i=0; i<8; i++) //<RealEABin1>
    {
        iQty = 0;
        sListData.sprintf("<RealEABin%d>    ", i+1);
        if(i<6)
        {
            if(i==5)
            {
                iToErrorBin=LastSet.iN14_9_InHandlerICCnt[i]; //Tested IC
            }
            else
            {
                iQty=LastSet.iN14_9_InHandlerICCnt[i]; //In Machine Tray IC Qty
            }
        }
        iTotalQty+=iQty;
        sListData=sListData+AnsiString(iQty);
        slDQCData->Add(sListData);
    }

    for(int i=0; i<7; i++) //<RealEAEcode1>
    {
        iQty=0;
        sListData.sprintf("<RealEAEcode%d>  ", i+2);
        if(i==1)
        {
            iQty=iToErrorBin;
        }
        sListData=sListData+AnsiString(iQty);
        slDQCData->Add(sListData);
    }
//---Qty----------------------------------------------------------------------<<

    sListData="<UpdateTime>    ";
    sListData=sListData+Now().FormatString("yyyy/mm/dd hh:nn:ss"); //2018/05/28 04:01:25
    slDQCData->Add(sListData);

    sListData="<<-  Bin Information End  ->>";
    slDQCData->Add(sListData);

    AnsiString sFilePath=sProductionInfoFilePath+"\\Handler summary\\";

    AnsiString sFileName;
    sFileName.sprintf("%s_%s.txt",
                            IniConfig.SocketHandlerID,
                            Now().FormatString("yyyymmddhhnnss"));

    AnsiString sFilePathName=sFilePath+sFileName;
    MyForceDirectories(sFilePath);
    slDQCData->SaveToFile(sFilePathName);

    delete slDQCData;
    FTP_OEEFile_Upload(sFilePath, IniConfig.asN14_9_ULQtyReportPath, sFileName);
}
//---------------------------------------------------------------------------
void TfProductionInfo::DoRunExecutFile(AnsiString sExecutFullFileName, AnsiString sParameters) //Mylin 20180911 Add Lead Scan Function Tray Report Defect Code
{
    if(sExecutFullFileName.Trim()=="")
    {
        ShowMyMessage("Do Run Execut File Path Is Empty Error!");
        return;
    }
    try
    {
        SHELLEXECUTEINFO execinfo;
        memset(&execinfo, 0, sizeof(execinfo));
        execinfo.cbSize=sizeof(execinfo);
        execinfo.lpVerb="open";
        execinfo.lpFile=sExecutFullFileName.c_str();
        if(FileExists(execinfo.lpFile)==true)
        {
            MySleep(100);                                                       //Jimmychiu 20220901 降低ACM交握影響主程序效能
            if(sParameters.Trim()!="")
            {
                execinfo.lpParameters=sParameters.c_str();
            }
            else
            {
                execinfo.lpParameters="";
            }
            execinfo.fMask=SEE_MASK_NOCLOSEPROCESS;
            execinfo.nShow=SW_SHOWMINIMIZED;
            ShellExecuteEx(&execinfo);
            MySleep(100);                                                       //Jimmychiu 20220901 降低ACM交握影響主程序效能
        }
        else
        {
            AnsiString sMsg="Run Execut File Is Not Exists! "+sExecutFullFileName;;
            ShowMyMessage(sMsg);
        }
        CloseHandle(execinfo.hProcess);
    }
    catch(...)
    {
        //
    }
}
//---------------------------------------------------------------------------
void TfProductionInfo::SocketLifeTimeUpload()
{
    if(IniConfig.bN14_15_SocketLifeTime==false ||
       IniConfig.asN14_15_ExecutFilePath.Trim()=="" ||
       IniConfig.asN14_15_MessageFilePath.Trim()=="")
    {
        return;
    }

    AnsiString sFilePath=IniConfig.asN14_15_MessageFilePath;
    AnsiString sFileName=IniConfig.SocketHandlerID+"_"+Now().FormatString("yyyymmddhhnnss")+".csv";
    AnsiString sFilePathName="";
    AnsiString sTemp="";
    TStringList *slSLReport = new TStringList();
    int iTotalContactCount=0, iCount=0;

    MyForceDirectories(sFilePath);
    if(sFilePath.SubString(sFilePath.Length(), 1)=="\\")
    {
        sFilePathName=sFilePath+sFileName;
    }
    else
    {
        sFilePathName=sFilePath+"\\"+sFileName;
    }

    slSLReport->Clear();
    //MO
    sTemp.sprintf("MO,%s", OEE_GetMO());
    slSLReport->Add(sTemp);
    //FLOW
    sTemp.sprintf("FLOW,%s", _sOEE_FLOW);
    slSLReport->Add(sTemp);
    //TEST_SITE
    sTemp.sprintf("TEST_SITE,%d", GetSiteCount(true));
    slSLReport->Add(sTemp);

    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            iTotalContactCount+=LastSet.iSocketContactCount[i][j];
        }
    }

    //TOTAL
    sTemp.sprintf("TOTAL,%d", iTotalContactCount);
    slSLReport->Add(sTemp);
    //SITE
    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            iCount++;
            sTemp.sprintf("SITE%d,%d", iCount, LastSet.iSocketContactCount[i][j]);
            slSLReport->Add(sTemp);
        }
    }
    slSLReport->SaveToFile(sFilePathName);
    DoRunExecutFile(IniConfig.asN14_15_ExecutFilePath, OEE_GetMO());
    delete slSLReport;
}
//---------------------------------------------------------------------------
AnsiString TfProductionInfo::ACM_ReadFlag(AnsiString sName)
{
    AnsiString sRes="";
    AnsiString sFullFileName="";
    AnsiString sFilePath=IniConfig.asN14_14_FlagFilePath;

    if(sFilePath.SubString(sFilePath.Length(), 1)!="\\")
    {
        sFilePath=sFilePath+"\\";
    }
    MyForceDirectories(sFilePath);
    sFullFileName.sprintf("%sStart.ini", sFilePath);

    if(FileExists(sFullFileName)==true)
    {
        TIniFile *INIFile;
        try//Jimmychiu 20220901 新增TryCatch避免崩潰
        {
            INIFile=new TIniFile(sFullFileName.c_str());
            sRes=INIFile->ReadString("ConfigItem", sName, "0");
        }
        catch(...)
        {
        }

        if(INIFile!=NULL)
        {
            INIFile->UpdateFile();
            delete INIFile;
        }
    }
    return sRes;
}
//---------------------------------------------------------------------------
void TfProductionInfo::ACM_WriteMsgAndCallExe(AnsiString sMessage)
{
    AnsiString sFullFileName="";
    AnsiString sMsg="";
    AnsiString sFilePath=IniConfig.asN14_14_MessageFilePath;
    TDateTime dtNow=Now();
    TStringList *slFile=new TStringList();
    slFile->Clear();

    if(sFilePath.SubString(sFilePath.Length(), 1)!="\\")
    {
        sFilePath=sFilePath+"\\";
    }
    MyForceDirectories(sFilePath);
    sFullFileName.sprintf("%s%s_%s.txt", sFilePath, IniConfig.SocketHandlerID, dtNow.FormatString("yyyymmddhhnnss"));
    slFile->Add(sMessage);
    //Sam 20201221 : 增加保護
    //==>
    try
    {
        if(FileExists(sFullFileName))//再檢查一次，若檔案還存在就不存了
        {
            sMsg.sprintf("%s is exist",sFullFileName);
            NewRecordProcess("", "ACM_WriteMsgAndCallExe Error", sMsg);
        }
        else
        {
            slFile->SaveToFile(sFullFileName);
            MySleep(100);//Jimmychiu 20220901 降低ACM交握影響主程序效能
        }
        #ifndef SOFT_SIMULTE
        DoRunExecutFile(IniConfig.asN14_14_ExecutFilePath);
        #endif
    }
    catch(...)
    {
        ShowMyMessage("ACM_WriteMsgAndCallExe Error");
    }
    slFile->Clear();
    delete slFile;
    //<==
    //Sam 20201221 : 增加保護
}
//---------------------------------------------------------------------------
void __fastcall TfProductionInfo::tm_IPSCControlTimer(TObject *Sender)
{
    static bool bIsRunning=false;
    int iCount=0;
    double dCount2=0.0;
    int iQty=0;
    AnsiString sIPSCFlag_Action, sIPSCFlag_Message;

    if(InitialOK==false)
        return;

    if(bIsRunning==true)
        return;
    else
        bIsRunning=true;
    ///Jimmychiu 20220901 降低ACM交握影響主程序效能
    if(ACM_WriteMsgAndCallExe_sMessage.Length()>0)
    {
        ACM_WriteMsgAndCallExe(ACM_WriteMsgAndCallExe_sMessage);
        ACM_WriteMsgAndCallExe_sMessage="";
    }
    //<==
    //Jimmychiu 20220901 降低ACM交握影響主程序效能
    //jimmychiu 20220711 reduce loading for ErrorReport
    //<==
    if(bRecFromErrorNote)
    {
        if(IniConfig.bN14_1_EnableOEEFunction && IniConfig.iN14_1_OEERecordCycleTime>0)
        {
            RecordControlBinCount("Pause Control Bin Check");  //Sam 20200525 : Control Bin
            EachCycleSecondDo_SaveAndUpdateOEEFiles(true);
            bStartNeedSaveAndUpdateOEEFiles=true;
            _sOEE_JamCode=AnsiString(fNote->edErrorCode->Text);
            if(fSecurity->GetBit8(fNote->Edit3->Text+" "+fNote->edUnitName->Text, fNote->edErrorCode->Text)==1) //Sam 20170925 自定義 Jam 計數
                _iOEE_ServiceCount++;
            else
                _iOEE_JamCount++;
        }
        bRecFromErrorNote=false;
    }
    //<==
    //jimmychiu 20220711 reduce loading for ErrorReport
    try
    {
        iCount=tIPSCTimer.LatchCycleTime()*0.001;                               //JimmyChiu 20220120 /1000=>*0.001
        if(iCount>=1)    //Check File every second
        {
            tIPSCTimer.LatchCycleTime(true);         //reset timer

            if(bEnableIPSC())
            {
                ReadAndDelIPSCFlagIniFile();
                if(iIPSCFlag_TriggerMode==0)        //CycleTime
                {
                    dCount2=double(tIPSCCycleTimer.LatchCycleTime())/(60.0*1000.0); //Sam 20190808 : 改為分計算
                    if((bIPSCFlag_IntegerTime==false && dCount2>double(IniConfig.iN14_16_IPSCInterval)) ||
                       (bIPSCFlag_IntegerTime==true  && bIntegerTimeCheck())                            ||          //有整點產出資料
                       (iIPSCFlag_CountdownQty>0 && CalculateNowTotalICQty()>iIPSCFlag_CountdownQty))               //收到 Clear 指令做 OneCycle 動作後，達到設定數量產出資料
                    {
                        DoIPSCProcess();
                    }
                }
                else if(iIPSCFlag_TriggerMode==1) //CycleQty
                {
                    iQty=0;
                    if(iIPSCFlag_ClearQty>0)
                        iQty=iIPSCFlag_ClearQty;
                    else
                        iQty=iIPSCFlag_CountdownQty;

                    if(iQty>0 && CalculateNowTotalICQty()>iQty)
                        DoIPSCProcess();
                }
                //JimmyChiu 20230616 : add IPSC Control
                //<==
                else if(bIPSCFlag_NeedPause==true)
                {
                    bIPSCFlag_NeedPause=false;
                    RecordProcess("IPSC Flag trigger Pause");
                    fMain->Pause("IPSC Flag trigger Pause");
                }
                else if(bIPSCFlag_NeedOnecycle==true)
                {
                    bIPSCFlag_NeedOnecycle=false;
                    RecordProcess("IPSC Flag trigger ONE CYCLE");
                    fMain->BtnOneCycleClick(fMain);
                }
                else if(bIPSCFlag_ShowMessageAndStop==true)
                {
                    bIPSCFlag_ShowMessageAndStop=false;
                    ShowMyMessage(AnsiString().sprintf("IPSC Flag trigger ShowMessageAndStop,%s",sIPSCFlag_ShowMessageAndStop));
                    sIPSCFlag_ShowMessageAndStop="";
                }
                else if(bIPSCFlag_NeedStart==true)
                {
                    bIPSCFlag_NeedStart=false;
                    RecordProcess("IPSC Flag trigger Start");
                    //too dangerous
                }
                //<==
                //JimmyChiu 20230616 : add IPSC Control
            }

            if(bEnableRPLog())
            {
                if(bTimerCheck(IniConfig.iN14_20_CycleTime))
                {
                    RPLogRecord=true;
                    fRPDefault->spbSaveClick(fRPDefault);
                    RPLogRecord=false;
                }
            }
        }
    }
    catch(...)
    {
    }
    bIsRunning=false;
}
//---------------------------------------------------------------------------
bool TfProductionInfo::bEnableIPSC()
{
    AnsiString sMsg="";
    if(CosFunction.bOEEFunction==false ||
       IniConfig.bN14_16_EnableIPSC==false ||
       InitialOK==false)
    {
        return false;
    }

        if(IniConfig.asN14_16_ExecutFilePath=="" ||
           IniConfig.asN14_16_FlagFilePath==""   ||
           FileExists(IniConfig.asN14_16_ExecutFilePath)==false ||
           IniConfig.iN14_16_IPSCInterval<=0)
        {
            sMsg="";
            if(IniConfig.asN14_16_ExecutFilePath=="")
            {
                sMsg.sprintf("IPSC Execute File Path Name is NULL!!");
                RecordProcess(sMsg);
            }

            if(FileExists(IniConfig.asN14_16_ExecutFilePath)==false)
            {
                sMsg.sprintf("IPSC Can Not Find Execute File => %s", IniConfig.asN14_16_ExecutFilePath);
                #ifndef SOFT_SIMULTE
                RecordProcess(sMsg);
                #endif
            }

            if(IniConfig.asN14_16_FlagFilePath=="")
            {
                sMsg.sprintf("IPSC Flag Ini File Path Name is NULL!!");
                RecordProcess(sMsg);
            }

        if(IniConfig.iN14_16_IPSCInterval<=0)
            {
                sMsg.sprintf("IPSC Record Cycle Time is less than 1 min");
                RecordProcess(sMsg);
            }
            return false;
        }
    return true;
}
//---------------------------------------------------------------------------
bool TfProductionInfo::bEnableRPLog()
{
    if(CosFunction.bRecipeParameterDefault   ==false ||
       CosFunction.bRecipeParameterDefaultLog==false ||
       InitialOK==false)
    {
        return false;
    }

    if(IniConfig.bN14_20_DefaultRecipeChangeLog             ==false ||
       IniConfig.asN14_20_ChangeLogPath                     ==""    ||
       IniConfig.bN14_20_DefaultRecipeChangeLogCycleRecord  ==false)
    {
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
void TfProductionInfo::SaveAndUpdateIPSCControlFiles()
{
    int ir;
    int x=32-(TestSocket.iShtCnt);
    AnsiString sListData="";
    AnsiString Bin5S="0,";
    int iSiteRowShort[32]={0};
    int iSiteColShort[32]={0};

    int iSiteTotalQty [MAX_SOCKET_ROW][MAX_SOCKET_COL];
    int iSitePassQty  [MAX_SOCKET_ROW][MAX_SOCKET_COL];
    double dSiteYield [MAX_SOCKET_ROW][MAX_SOCKET_COL];
    double dSiteMaxYeild=0.0, dSiteMinYeild=100.0;                              //Sam 20210705 : 計算 By Site Max/Min Yield

    for(int i=0; i<MAX_SOCKET_ROW; i++)
    {
        for(int j=0; j<MAX_SOCKET_COL; j++)
        {
            iSiteTotalQty[i][j]=0;
            iSitePassQty [i][j]=0;
            dSiteYield   [i][j]=0.0;
        }
    }

    TStringList *slIPSCControlData = new TStringList;

    slIPSCControlData->Clear();

    CalculateNowArmSiteBinQty();
    CalculateNowUnloaderTrayQty();

    sTester     =sLoadMO_Tester;                                                //測試機
    sListData   =sListData+sTester;
    sHandler    =sLoadMO_Handler;                                               //分類機
    sListData   =sListData+","+sHandler;
    sMO         =sLoadMO_MO;                                                    //製單
    sListData   =sListData+","+sMO;
    sDevice     =sLoadMO_Device;                                                //型號
    sListData   =sListData+","+sDevice;
    DateSeparator='/';                                                          // Sam 20210225 : 增加保護
    sNowTime    =Now().FormatString("yyyy/mm/dd hh:nn:ss");                     //當下時間
    sListData   =sListData+","+sNowTime;
    sTotalLoadQty=AnsiString(CalculateNowTotalICQty());                         //總進量
    sListData   =sListData+","+AnsiString(sTotalLoadQty);

    for(int i=0; i<8; i++)
    {
        sListData=sListData+","+AnsiString(iNowUnloaderTrayQty[i]);             //Bin1 ~ Bin8  == Auto1~Auto3 Fix1~Fix3
    }

    sListData=sListData+",0,0,0";                                               //Sam 20210514 : 多補三個欄位 mark,lead,purge

    for(int i=0; i<TestSocket.iShtRow; i++)                                     //Sam 20210705 : 計算 By Site Max/Min Yield
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            for(int iArm=0; iArm<2; iArm++)
            {
                for(int iTray=0; iTray<8; iTray++)                              //計算各個 Site 在各軌道的總數，例 Ab-Site 放到 Auto1 數量
                {
                    iSiteTotalQty[i][j]+=iNowArmSiteBinQty[iArm][i][j][iTray];  //各 Site 總測試 IC 數量
                    if(Prod.iIsPassT6[i]==1)                                    //Steven 20240105 : Prod.bIsPass --> Prod.iIsPassT6 //Steven 20240701 : 0 --> 1
                        iSitePassQty[i][j]+=iNowArmSiteBinQty[iArm][i][j][iTray];    //各 Site Pass IC 數量
                }
            }

            if(iSiteTotalQty[i][j]==0)
                iSiteTotalQty[i][j]=1;
            dSiteYield[i][j]=ChangeToFloat((double)iSitePassQty[i][j], (double)iSiteTotalQty[i][j]);  //計算 各 Site Pass Yield

            if(dSiteYield[i][j]>=dSiteMaxYeild)
                dSiteMaxYeild=dSiteYield[i][j];                                 //各SITE良率最高者
            if(dSiteYield[i][j]<=dSiteMinYeild)
                dSiteMinYeild=dSiteYield[i][j];                                 //各SITE良率最低者
        }
    }

    //sMaxSiteYield="0"; //各SITE良率最高者
    sMaxSiteYield.sprintf("%1.2f", dSiteMaxYeild);                              //各SITE良率最高者
    sListData=sListData+","+sMaxSiteYield;

    //sMinSiteYield="0"; //各SITE良率最低者
    sMinSiteYield.sprintf("%1.2f", dSiteMinYeild);                              //各SITE良率最高者
    sListData=sListData+","+sMinSiteYield;

    //sSiteOpenOrClose
    sSiteOpenOrClose="";                                                        //更改為 1101   ( 1為開啟SITE 0 為關閉SITE)

    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            iSiteRowShort[TestIF.iSiteMap[i][j]-1]=i;
            iSiteColShort[TestIF.iSiteMap[i][j]-1]=j;
        }
    }

    for(int i=0; i<TestSocket.iShtCnt; i++)
    {
        if(bTestSiteUse[0][iSiteRowShort[i]][iSiteColShort[i]])
        {
            sSiteOpenOrClose=sSiteOpenOrClose+"1";
        }
        else
        {
            sSiteOpenOrClose=sSiteOpenOrClose+"0";
        }
    }

    sSiteOpenOrClose=sSiteOpenOrClose+AnsiString::StringOfChar('0', x);         //長度固定32Site，後面沒用到的補0
    sListData=sListData+","+sSiteOpenOrClose;
    slIPSCControlData->Add(sListData);

    for(int i=0; i<TestSocket.iShtCnt; i++)
    {
        if(IsNNMode()==NN_2Row)
        {
            ir=GetArmBySiteFor32Site(i+1);                                      //Sam 20200908 : 修正 Site1 資料
            sListData="";
            if(ir==0 || ir==1)
            {
                sListData="0,0,0,0,0,0,0,0,"+Bin5S;                             //Sam 20210701 增加 Bin5S 欄位
                for(int iTray=0; iTray<8; iTray++)                              //計算各個 Site 在各軌道的總數，例 Ab-Site 放到 Auto1 數量
                {
                    sListData=sListData+AnsiString(iNowArmSiteBinQty[0][iSiteRowShort[i]][iSiteColShort[i]][iTray])+",";  //資料 Auto1~3 Fix1~3
                }
                sListData=sListData+Bin5S;                                      //Sam 20210701 增加 Bin5S 欄位
                slIPSCControlData->Add(sListData);
            }
            else if(ir==2 || ir==3)                                             //32Site Arm1
            {
                for(int iTray=0; iTray<8; iTray++)                              //計算各個 Site 在各軌道的總數，例 Ab-Site 放到 Auto1 數量
                {
                    sListData=sListData+AnsiString(iNowArmSiteBinQty[0][iSiteRowShort[i]][iSiteColShort[i]][iTray])+",";  //資料 Auto1~3 Fix1~3
                }
                sListData=sListData+Bin5S+"0,0,0,0,0,0,0,0,"+Bin5S;             //Sam 20210701 增加 Bin5S 欄位
                slIPSCControlData->Add(sListData);
            }
            else
            {
                sListData=Bin5S+"0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"+Bin5S ; //Sam 20210701 增加 Bin5S 欄位
                slIPSCControlData->Add(sListData);
            }
        }
        else
        {
            sListData="";
            for(int iArm=0; iArm<2; iArm++)
            {
                for(int iTray=0; iTray<8; iTray++)                              //計算各個 Site 在各軌道的總數，例 Ab-Site 放到 Auto1 數量
                {
                    sListData=sListData+AnsiString(iNowArmSiteBinQty[iArm][iSiteRowShort[i]][iSiteColShort[i]][iTray])+",";  //資料 Auto1~3 Fix1~3
                }
                sListData=sListData+Bin5S;                                      //Sam 20210701 增加 Bin5S 欄位
            }
            slIPSCControlData->Add(sListData);
        }
    }

    for(int i=0; i<x; i++)                                                      //長度固定32Site，後面沒用到的補0
    {
        sListData="";
        for(int iArm=0; iArm<2; iArm++)
        {
            for(int iTray=0; iTray<8; iTray++)
            {
                sListData=sListData+"0,";
            }
            sListData=sListData+Bin5S;                                          //Sam 20210701 增加 Bin5S 欄位
        }
        slIPSCControlData->Add(sListData);
    }

    ClearArmSiteBinQty();
    ClearUnloaderTrayQty();
    AnsiString sFilePathName=IniConfig.asN14_16_ProductionFilePath+IniConfig.sGPIBMachineID+".csv";
    slIPSCControlData->SaveToFile(sFilePathName);

    delete slIPSCControlData;
}
//---------------------------------------------------------------------------
int TfProductionInfo::GetArmBySiteFor32Site(int iSite)
{
    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            if(TestIF_File.iSiteMap[i][j]==iSite)
            {
                return i;
            }
        }
    }
    return -1;
}
//---------------------------------------------------------------------------
void TfProductionInfo::ReadAndDelIPSCFlagIniFile()
{
    AnsiString sIPSCFlag_Action="", sIPSCFlag_Message="", sIPSCFlag_Mode="", sMsg="",asIPSCINIBackFile="";
    int iIPSCFlag_Qty=0, iIPSCFlag_Value=0;
    if(FileExists(IniConfig.asN14_16_FlagFilePath)==false)
    {
        return;
    }
    else                                                                        //JimmyChiu 20211227 : 讀取IPSC ini file
    {   //file exist
        asIPSCINIBackFile=ExtractFilePath(IniConfig.asN14_16_FlagFilePath)+"IPSC_back.ini";
        if(FileExists(asIPSCINIBackFile))
            DeleteFile(asIPSCINIBackFile);
        CopyFile(IniConfig.asN14_16_FlagFilePath.c_str(), asIPSCINIBackFile.c_str(), false);
    }

    //JimmyChiu 20230616 : add IPSC Control
    //<== //Initial
    sIPSCFlag_ShowMessageAndStop="";
    bIPSCFlag_ShowMessageAndStop=false;
    bIPSCFlag_NeedOnecycle=false;
    bIPSCFlag_NeedPause=false;
    bIPSCFlag_NeedStart=false;
    //<==
    //JimmyChiu 20230616 : add IPSC Control

    sIPSCFlag_Action =CheckAndReadIniData(IniConfig.asN14_16_FlagFilePath, "IPSC", "Action", AnsiString(""));
    iIPSCFlag_Qty    =CheckAndReadIniData(IniConfig.asN14_16_FlagFilePath, "IPSC", "Qty", 0);
    sIPSCFlag_Message=CheckAndReadIniData(IniConfig.asN14_16_FlagFilePath, "IPSC", "Message", AnsiString(""));
    iIPSCFlag_Value  =CheckAndReadIniData(IniConfig.asN14_16_FlagFilePath, "IPSC", "Value", 0);
    sIPSCFlag_Mode   =CheckAndReadIniData(IniConfig.asN14_16_FlagFilePath, "IPSC", "Mode", AnsiString(""));

    if(sIPSCFlag_Action=="PreSiteTest")                                         //待處理
    {
        if(InArmSuck.HasIC()==true)
        {
            return;
        }
    }
    else
    {
        DeleteFile(IniConfig.asN14_16_FlagFilePath);
        if(sIPSCFlag_Action=="")
        {
            sMsg.sprintf("IPSC The Action Flag is NULL.");
            RecordProcess(sMsg);
            return;
        }
        else if(sIPSCFlag_Action=="ShowMessage")
        {
            if(sIPSCFlag_Message=="")
            {
                sMsg.sprintf("Read IPSC Flag Ini Error, ShowMessage = NULL Error");
                RecordProcess(sMsg);
                return;
            }
        }
        else if(sIPSCFlag_Action=="Clear")
        {
            if(iIPSCFlag_Qty==0)
            {
                sMsg.sprintf("Read IPSC Flag Ini Error, Clear Qty = 0 Error");
                RecordProcess(sMsg);
                return;
            }
        }
        else if(sIPSCFlag_Action=="Light_ON")
        {
        }
        else if(sIPSCFlag_Action=="Light_OFF")
        {
        }
        //JimmyChiu 20230616 : add IPSC Control
        //<==
        else if(sIPSCFlag_Action=="ShowMessageAndStop")
        {
            sIPSCFlag_ShowMessageAndStop=sIPSCFlag_Message;
            bIPSCFlag_ShowMessageAndStop=true;
        }
        else if(sIPSCFlag_Action=="DoOnecycle")
        {
            bIPSCFlag_NeedOnecycle=true;
        }
        else if(sIPSCFlag_Action=="DoPause")
        {
            bIPSCFlag_NeedPause = true;
        }
        else if(sIPSCFlag_Action=="DoStart")
        {
            bIPSCFlag_NeedStart=true;
        }
        //<==
        //JimmyChiu 20230616 : add IPSC Control
        else if(sIPSCFlag_Action=="SetCycleTime")
        {
            if(iIPSCFlag_Value<=0 || iIPSCFlag_Value>=1440)
            {
                sMsg.sprintf("IPSC set cycle time not in range=> %d", iIPSCFlag_Value);
                RecordProcess(sMsg);
                return;
            }

            if(sIPSCFlag_Mode=="Dynamic" || sIPSCFlag_Mode=="Static")
            {
            }
            else
            {
                sMsg.sprintf("IPSC The Mode Flag [%s] is not support.", sIPSCFlag_Mode);
                RecordProcess(sMsg);
                return;
            }
        }
        else if(sIPSCFlag_Action=="SetCycleQty")
        {
            if(iIPSCFlag_Qty<=0)
            {
                sMsg.sprintf("IPSC set cycle Qty not in range=> %d", iIPSCFlag_Qty);
                RecordProcess(sMsg);
                return;
            }
        }
        else
        {
            sMsg.sprintf("IPSC The Action Flag [%s] is not support.", sIPSCFlag_Action);
            RecordProcess(sMsg);
            return;
        }
    }

    if(sIPSCFlag_Action=="PreSiteTest")                                         //待處理
    {
        //DoAutoSiteMap
    }
    else if(sIPSCFlag_Action=="ShowMessage")
    {
        bIPSCFlag_IsRedLightON=true;
        //sIPSCFlag_ShowMessage=sIPSCFlag_Message;
        iUnLoaderCount=8;
        ShowUnloaderTrayMessage(sIPSCFlag_Message,"");
    }
    else if(sIPSCFlag_Action=="Clear")
    {
        bIPSCFlag_IsRedLightON=true;
        if(iIPSCFlag_TriggerMode==1)
        {
            iIPSCFlag_ClearQty=iIPSCFlag_Qty;
        }
        else
        {
            iIPSCFlag_CountdownQty =iIPSCFlag_Qty;
        }
        RecordProcess("IPSC Flag trigger ONE CYCLE");
        iOneCycleFinishShowMsg=1;                                               //Sam 20250115 : 矽格湖口 GPIB OneCycle 要強制切 ASM
        sOneCycleFinishShowMsg=sIPSCFlag_Message;
        fMain->BtnOneCycleClick(fMain);
    }
    else if(sIPSCFlag_Action=="Light_ON")
    {
        bIPSCFlag_IsYellowLightON=true;
    }
    else if(sIPSCFlag_Action=="Light_OFF")
    {
        bIPSCFlag_IsYellowLightON=false;
        bIPSCFlag_IsRedLightON=false;
        if(iIPSCFlag_TriggerMode==1)
            iIPSCFlag_CountdownQty=4000;
    }
    else if(sIPSCFlag_Action=="SetCycleTime")
    {
        iIPSCFlag_TriggerMode=0;
        IniConfig.iN14_16_IPSCInterval=iIPSCFlag_Value;
        tIPSCCycleTimer.LatchCycleTimeSec(true);                                //重新計數
        if(sIPSCFlag_Mode=="Static")
        {
            bIPSCFlag_IntegerTime=true;
        }
    }
    else if(sIPSCFlag_Action=="SetCycleQty")
    {
        iIPSCFlag_TriggerMode=1;
        iIPSCFlag_CountdownQty =iIPSCFlag_Qty;
    }
}
//---------------------------------------------------------------------------
void TfProductionInfo::CalculateNowArmSiteBinQty(bool bIsClear)
{
    if(CUSTOMER_CODE==CC_Greatek && IniConfig.bN14_16_EnableIPSC)
    {
        int iSelTrayCT=0, iTarValue=0;
        for(int iCat=0;iCat<TEST_MAX_BIN;iCat++)                                //每個category
        {
            iNowSiteBinTotalQty[iCat]=0;
        }
        for(int iArm=0; iArm<2; iArm++)
        {
            for(int iRow=0; iRow<MAX_SOCKET_ROW; iRow++)
            {
                for(int iCol=0; iCol<MAX_SOCKET_COL; iCol++)
                {
                    for(int iAuto=0; iAuto<9; iAuto++)                          //Auto1~3 Fix1~3
                    {
                        iSelTrayCT=static_cast<int>(ArmData[iArm]->ArmSKET[iRow][iCol]->GetSelTrayCT(iAuto));   //Jimmychiu 20240123 : 防呆-IPSC上傳的資料有負值
                        iTarValue=iSelTrayCT-iLastArmSiteBinQty[iArm][iRow][iCol][iAuto];
                        if(iTarValue<=0)
                        {
                            iTarValue=0;
                        }
                        iNowArmSiteBinQty[iArm][iRow][iCol][iAuto]+=iTarValue;
                        iLastArmSiteBinQty[iArm][iRow][iCol][iAuto]=iSelTrayCT;
                        iNowSiteBinTotalQty[iAuto]+=iNowArmSiteBinQty[iArm][iRow][iCol][iAuto];
                        if(bIsClear)
                        {
                            iLastArmSiteBinQty[iArm][iRow][iCol][iAuto]=0;
                        }
                    }
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
void TfProductionInfo::CalculateNowUnloaderTrayQty(bool bIsClear)
{
    std::vector<int> lsBin0_8=GetBin0_8List();
    for(int iBin=0; iBin<6; iBin++)                                                      //Auto1~3，Fix1~3
    {
        iNowUnloaderTrayQty[iBin]+=LastSet.BinCT[0][lsBin0_8[iBin]]-iLastUnloaderTrayQty[iBin];
        iLastUnloaderTrayQty[iBin]=LastSet.BinCT[0][lsBin0_8[iBin]];
        if(bIsClear)
        {
            iLastUnloaderTrayQty[iBin]=0;
        }
    }
}
//---------------------------------------------------------------------------
int TfProductionInfo::CalculateNowTotalICQty()                                  //測試機測出來的總數
{
    int iArm,iRow,iCol,iBin,iTotalLoadQty;
    CalculateNowArmSiteBinQty();

    iTotalLoadQty=0;
    for(iArm=0; iArm<2; iArm++)
    {
        for(iRow=0; iRow<MAX_SOCKET_ROW; iRow++)
        {
            for(iCol=0; iCol<MAX_SOCKET_COL; iCol++)
            {
                for(iBin=0; iBin<TEST_MAX_BIN; iBin++)
                {
                    iTotalLoadQty=iTotalLoadQty+iNowArmSiteBinQty[iArm][iRow][iCol][iBin];
                }
            }
        }
    }
    return iTotalLoadQty;
}
//---------------------------------------------------------------------------
int TfProductionInfo::CalculateUnloadTotalICQty()                               //放到 Unloader 的總數
{
    int iTotalLoadQty;
    CalculateNowUnloaderTrayQty();

    iTotalLoadQty=0;
    for(int i=0; i<8; i++)
    {
        iTotalLoadQty=iTotalLoadQty+iNowUnloaderTrayQty[i];                     //Bin1 ~ Bin8  == Auto1~Auto3 Fix1~Fix3
    }
    return iTotalLoadQty;
}
//---------------------------------------------------------------------------
void TfProductionInfo::ClearArmSiteBinQty()
{
    for(int iArm=0; iArm<2; iArm++)
    {
        for(int iRow=0; iRow<MAX_SOCKET_ROW; iRow++)
        {
            for(int iCol=0; iCol<MAX_SOCKET_COL; iCol++)
            {
                for(int iAuto=0; iAuto<9; iAuto++)                              //Auto1~3 Fix1~3
                {
                     iNowArmSiteBinQty[iArm][iRow][iCol][iAuto]=0;
                     iLastArmSiteBinQty[iArm][iRow][iCol][iAuto]=ArmData[iArm]->ArmSKET[iRow][iCol]->GetSelTrayCT(iAuto);
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
void TfProductionInfo::ClearUnloaderTrayQty()
{
    for(int i=0; i<8; i++)
    {
        iNowUnloaderTrayQty[i] =0;
        iLastUnloaderTrayQty[i]=LastSet.BinCT[0][iTo3Unload[i]];
    }
}
//---------------------------------------------------------------------------
void TfProductionInfo::InitialIPSCControlCount()
{
    sTester         ="";                                                        //測試機
    sHandler        ="";                                                        //分類機
    sMO             ="";                                                        //製單
    sDevice         ="";                                                        //型號
    sNowTime        ="";                                                        //當下時間
    sTotalLoadQty   ="";                                                        //總進量
    sMaxSiteYield   ="";                                                        //各SITE良率最高者
    sMinSiteYield   ="";                                                        //各SITE良率最低者
    sSiteOpenOrClose="";                                                        //更改為 1101   ( 1為開啟SITE 0 為關閉SITE)

    for(int i=0; i<8; i++)
    {
        iNowUnloaderTrayQty[i]  =0;                                             //Auto1 ~ Auto3 Fix1~Fix3
        iLastUnloaderTrayQty[i] =0;                                             //Auto1 ~ Auto3 Fix1~Fix3
    }
    ZeroMemory(iNowUnloaderTrayQty,     sizeof(iNowUnloaderTrayQty));           //Auto1 ~ Auto3 Fix1~Fix3
    ZeroMemory(iLastUnloaderTrayQty,    sizeof(iLastUnloaderTrayQty));          //Auto1 ~ Auto3 Fix1~Fix3
    ZeroMemory(iNowArmSiteBinQty,       sizeof(iNowArmSiteBinQty));             //BySiteByBin Count
    ZeroMemory(iLastArmSiteBinQty,      sizeof(iLastArmSiteBinQty));
    ClearArmSiteBinQty();
    ClearUnloaderTrayQty();
    bIPSCFlag_NeedOnecycle                  =false;
    bIPSCFlag_IsYellowLightON               =false;
    bIPSCFlag_IsRedLightON                  =false;
    iIPSCFlag_TriggerMode                   =-1;
    iIPSCFlag_CountdownQty                  =0;
    iIPSCFlag_ClearQty                      =0;
    sIPSCFlag_ShowMessage                   ="";
    iIPSCFlag_NeedOnecycleStartCount        =0;
    bIPSCFlag_AfterOnecycleNeedShowMessage  =false;
    bIPSCFlag_IntegerTime                   =false;
    iOneCycleFinishShowMsg=0;                                                   //Sam 20250115 : 矽格湖口 GPIB OneCycle 要強制切 ASM
    sOneCycleFinishShowMsg="";
}
//---------------------------------------------------------------------------
bool TfProductionInfo::bIntegerTimeCheck()
{
    DecodeTime(Now(), h, n, s, z);
    if(IniConfig.iN14_16_IPSCInterval==120)                                     //2Hour
    {
        if(h%2==0)                                                              //0 2 4 6 8 10 12 14 16 18 20 22 24 偶數整點。
            return true;
    }
    else
    {
        if(n==0)                                                                //整點
            return true;
    }
    return false;
}
//---------------------------------------------------------------------------
bool TfProductionInfo::bTimerCheck(int iTimeMode)
{
    static int iOldHour=-1;
    static int iOldmin=-1;
    DecodeTime(Now(), h, n, s, z);
    if((iTimeMode==1 && n==0) ||                                                //整點
       (iTimeMode==2 && n==30))                                                 //半小時
    {
        if(iOldHour!=h || iOldmin!=n)
        {
            iOldHour=h;
            iOldmin=n;
            return true;
        }
    }
    return false;
}
//---------------------------------------------------------------------------
void __fastcall TfProductionInfo::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    bShow=false;                                                                //Sam 20200225 : OEE 功能防呆保護提示顯示在畫面，不要用 MessageBox。
    DoIniDataToForm();                                                          //JerryYang 20250411 : 離開頁面要刷新一次, 避免誤存檔
}
//---------------------------------------------------------------------------
bool TfProductionInfo::DoCheckFTPFile()                                         //Sam 20200305 : 增加 FTP 檔案檢查，檢查到 VIE_STOP.txt 就執行 OneCycle
{
    AnsiString aPathName, sDownloadFile;
    AnsiString asUserID="", asPassword="", asHost="";
    GetFTP_Setting(asUserID, asPassword, asHost);                               //Jimmychiu 20240104 : 優化FTP連線Function
    TfFTP fFTP(asUserID, asPassword, asHost);
    if(fFTP.Connect())
    {
        AnsiString asError=AnsiString("");
        aPathName.sprintf("%s/%s/", IniConfig.asN14_4_MODownloadPath, AnsiString(IniConfig.SocketHandlerID));
        #ifdef SOFT_SIMULTE
        aPathName.sprintf("//handler_data//SETUP_FILE//%s//", AnsiString(IniConfig.SocketHandlerID));
        #endif
        if(fFTP.Delete(aPathName,"VIE_STOP",asError))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        MyDBIProcess("Exception", "TfProductionInfo::DoCheckFTPFile");
        ShowMyMessage("DoCheckFTPFile Connect Failed");
    }
    return false;
}
//---------------------------------------------------------------------------
void __fastcall TfProductionInfo::NMFTPListItem(AnsiString Listing)             //Sam 20200305 : 增加 FTP 檔案檢查，檢查到 VIE_STOP.txt 就執行 OneCycle
{
    coCheckFTPFileList->Items->Add(Listing);
}
//---------------------------------------------------------------------------
bool TfProductionInfo::bIsNeedCheckControlBin()                                 //Sam 20201216 : Add 數量監控
{
    bool bRet=false;
    int iTotal=0;
    UpdateControlBinCount(false);

    for(int iRow=0; iRow<TestSocket.iShtRow; iRow++)
    {
        for(int iCol=0; iCol<TestSocket.iShtCol; iCol++)
        {
            iTotal=NowControlBinCategory.iCountSocketTotal[iRow][iCol]-OldControlBinCategory.iCountSocketTotal[iRow][iCol];
            if(iTotal>iControlBinCheckCount)                                    //其中一個 Site 數量到了需要檢查 Control Bin
                bRet=true;
        }
    }
    return bRet;
}
//---------------------------------------------------------------------------
void TfProductionInfo::bCheckControlBinYield()
{
    int iBin=0;
    AnsiString S1="";
    int iTotal=0;
    double dSiteMaxYield[TEST_MAX_BIN]={0.0};
    int iSiteRowMaxYield[TEST_MAX_BIN]={0};
    int iSiteColMaxYield[TEST_MAX_BIN]={0};
    double dSiteMinYield[TEST_MAX_BIN]={100.0};
    int iSiteRowMinYield[TEST_MAX_BIN]={0};
    int iSiteColMinYield[TEST_MAX_BIN]={0};

    int iRowMaxYield=0;
    int iColMaxYield=0;
    int iRowMinYield=0;
    int iColMinYield=0;
    int iControlBinCount=0;
    double dControlBinPercent=0.0;

    for(int i=0; i<TEST_MAX_BIN; i++)
    {
        dSiteMaxYield   [i]=0.0;
        iSiteRowMaxYield[i]=0;
        iSiteColMaxYield[i]=0;
        dSiteMinYield   [i]=100.0;
        iSiteRowMinYield[i]=0;
        iSiteColMinYield[i]=0;
    }

    if(iControlBinNum<=0)
    {
        return;
    }

    UpdateControlBinCount(false);
    ZeroMemory(dControlBinYield, sizeof(dControlBinYield));
    ZeroMemory(dControlBinCount, sizeof(dControlBinCount));

    for(int iRow=0; iRow<TestSocket.iShtRow; iRow++)                            //Socket total
    {
        for(int iCol=0; iCol<TestSocket.iShtCol; iCol++)                        //By Site total
        {
            iTotal=NowControlBinCategory.iCountSocketTotal[iRow][iCol]-OldControlBinCategory.iCountSocketTotal[iRow][iCol];
            for(int iCat=0; iCat<iTestBinCount; iCat++)
            {
                if(iTotal>0)                                                    //By Site By Bin Yield
                {
                    dControlBinCount[iRow][iCol][iCat]=NowControlBinCategory.iCountCategory[0][iRow][iCol][iCat]+
                                                       NowControlBinCategory.iCountCategory[1][iRow][iCol][iCat]-
                                                       OldControlBinCategory.iCountCategory[0][iRow][iCol][iCat]-
                                                       OldControlBinCategory.iCountCategory[1][iRow][iCol][iCat];
                    dControlBinYield[iRow][iCol][iCat]=ChangeToFloat(dControlBinCount[iRow][iCol][iCat], (double)iTotal);
                }
            }

            for(int i=0; i<iControlBinNum; i++)
            {
                iBin=iControlBin[i];
                dControlBinPercent=dControlBinYield[iRow][iCol][iBin];
                iControlBinCount=dControlBinCount[iRow][iCol][iBin];

                if(dControlPercent[i]>0)
                {
                    if(dControlBinPercent>=dControlPercent[i])
                    {
                        S1.sprintf("ControlBin By Yield, Site_%s category_%d over limit yield %2.1f%% >= %2.1f%%, %dea / Total %dea",
                        IndexSuckName[iRow][iCol],
                        iBin,
                        dControlBinPercent,
                        dControlPercent[i],
                        iControlBinCount,
                        iTotal);
                        DoLowYieldAlarm("WAR07357", S1);
                    }
                    else
                    {
                        S1.sprintf("Site_%s category_%d lower limit yield %2.1f%% < %2.1f%%, %dea / Total %dea",
                        IndexSuckName[iRow][iCol],
                        iBin,
                        dControlBinPercent,
                        dControlPercent[i],
                        iControlBinCount,
                        iTotal);
                        NewRecordProcess("", "Check ControlBin By Yield", S1);
                    }
                }

                if(iControlCount[i]>0)
                {
                    if(iControlBinCount>=iControlCount[i])
                    {
                        S1.sprintf("ControlBin By Count, Site_%s Category_%d over limit count %dea / Total %dea >= %dea",
                        IndexSuckName[iRow][iCol],
                        iBin,
                        iControlBinCount,
                        iTotal,
                        iControlCount[i]);
                        DoLowYieldAlarm("WAR07357", S1);
                    }
                    else
                    {
                        S1.sprintf("Site_%s Category_%d lower limit count %dea / Total %dea < %dea",
                        IndexSuckName[iRow][iCol],
                        iBin,
                        iControlBinCount,
                        iTotal,
                        iControlCount[i]);
                        NewRecordProcess("", "Check ControlBin By Count", S1);
                    }
                }

                if(dControlSiteToSite[i]>0)                                     //Sam 20201209 : Add by site by bin site to site 比較
                {
                    if(dControlBinPercent>=dSiteMaxYield[iBin])
                    {
                        dSiteMaxYield[iBin]=dControlBinPercent;
                        iSiteRowMaxYield[iBin]=iRow;
                        iSiteColMaxYield[iBin]=iCol;
                    }

                    if(dControlBinPercent<=dSiteMinYield[iBin])
                    {
                        dSiteMinYield[iBin]=dControlBinPercent;
                        iSiteRowMinYield[iBin]=iRow;
                        iSiteColMinYield[iBin]=iCol;
                    }
                }
            }
        }
    }

    for(int i=0; i<iControlBinNum; i++)                                         //Sam 20201209 : Add by site by bin site to site 比較
    {
        iBin=iControlBin[i];
        iRowMaxYield=iSiteRowMaxYield[iBin];
        iColMaxYield=iSiteColMaxYield[iBin];
        iRowMinYield=iSiteRowMinYield[iBin];
        iColMinYield=iSiteColMinYield[iBin];
        if(dControlSiteToSite[i]>0)
        {
            if(dSiteMaxYield[iBin]-dSiteMinYield[iBin]>dControlSiteToSite[i])
            {
                S1.sprintf("ControlBin by SiteToSite Yield, Category_%d over limit setting, %s %2.1f%% - %s %2.1f%% >= %2.1f%%",
                iBin,
                IndexSuckName[iRowMaxYield][iColMaxYield],
                dSiteMaxYield[iBin],
                IndexSuckName[iRowMinYield][iColMinYield],
                dSiteMinYield[iBin],
                dControlSiteToSite[i]);
                DoLowYieldAlarm("WAR07357", S1);
            }
            else
            {
                S1.sprintf("Category_%d lower limit setting, %s %2.1f%% - %s %2.1f%% < %2.1f%%",
                iBin,
                IndexSuckName[iRowMaxYield][iColMaxYield],
                dSiteMaxYield[iBin],
                IndexSuckName[iRowMinYield][iColMinYield],
                dSiteMinYield[iBin],
                dControlSiteToSite[i]);
                NewRecordProcess("", "Check ControlBin by SiteToSite Yield", S1);
            }
        }
    }

    memcpy(&OldControlBinCategory.iCountCategory[0][0][0][0], &NowControlBinCategory.iCountCategory[0][0][0][0], sizeof(NowControlBinCategory));
}
//---------------------------------------------------------------------------
void TfProductionInfo::UpdateControlBinCount(bool bClear)                       //Sam 20200525 : Control Bin
{
    if(CUSTOMER_CODE==CC_Greatek && IniConfig.bN14_1_EnableOEEFunction)
    {
        NowControlBinCategory.ClearCount();
        for(int iRow=0; iRow<MAX_SOCKET_ROW; iRow++)                            //Sam 20201209 : 修正資料
        {
            for(int iCol=0; iCol<MAX_SOCKET_COL; iCol++)
            {
                for(int iArm=0; iArm<2; iArm++)
                {
                    NowControlBinCategory.iCountCategory[iArm][iRow][iCol][iTestBinCount]  =ArmData[iArm]->ArmSKET[iRow][iCol]->GetIFError();
                    NowControlBinCategory.iTotalCategory[iTestBinCount]                   +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetIFError();
                    for(int iCat=0; iCat<iTestBinCount; iCat++)                 //每個category
                    {
                        NowControlBinCategory.iCountCategory[iArm][iRow][iCol][iCat]  =ArmData[iArm]->ArmSKET[iRow][iCol]->GetSelBinCT(iCat);
                        NowControlBinCategory.iTotalCategory[iCat]                   +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetSelBinCT(iCat);
                    }
                    NowControlBinCategory.iCountSocketTotal[iRow][iCol]      +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetTotal();
                    NowControlBinCategory.iCountHeadTotal[iArm][iRow][iCol]  +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetTotal();
                    NowControlBinCategory.iTotalSocket                       +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetTotal();
                }
            }
        }

        if(bClear)
        {
            memcpy(&OldControlBinCategory.iCountCategory[0][0][0][0], &NowControlBinCategory.iCountCategory[0][0][0][0], sizeof(NowControlBinCategory));
        }
    }
}
//---------------------------------------------------------------------------
void TfProductionInfo::RecordControlBinCount(AnsiString msg)                    //Sam 20200525 : Control Bin
{
    int iCount=0;
    if(iControlBinCheckPoint<=0)                                                //Sam 20201221 : 增加保護
        return;

    if(bIsControlBinCounting)
    {
        bIsControlBinCounting=false;
        iCount=tControlBinTimer.LatchCycleTime()*0.001;                         //Sam 20200525 : Control Bin
        iControlBinTimerCnt-=iCount;
        NewRecordProcess("","Pause Control Bin Check", IntToStr(iControlBinTimerCnt)+"s");
    }
}
//---------------------------------------------------------------------------
void TfProductionInfo::DoIPSCProcess()
{
    if(CUSTOMER_CODE==CC_Greatek && IniConfig.bN14_16_EnableIPSC)
    {
        bIPSCFlag_IntegerTime=false;
        if(iIPSCFlag_TriggerMode==0)
            iIPSCFlag_CountdownQty=0;
        else                                                                    //iIPSCFlag_TriggerMode==1
            iIPSCFlag_ClearQty=0;

        SaveAndUpdateIPSCControlFiles();
        #ifndef SOFT_SIMULTE
        DoRunExecutFile(IniConfig.asN14_16_ExecutFilePath);
        #endif
        tIPSCCycleTimer.LatchCycleTime(true);                                   //reset timer
    }
}
//---------------------------------------------------------------------------
void TfProductionInfo::DoOneCycleFinishShowMsg()
{
//    if(bEnableIPSC())
//    {
//        if(iOneCycleFinishShowMsg==1)                                           //Sam 20250115 : 矽格湖口 GPIB OneCycle 要強制切 ASM
//        {
//            if(sOneCycleFinishShowMsg!="")
//                ShowMyMessage(sOneCycleFinishShowMsg, sOneCycleFinishShowMsg);
//            iOneCycleFinishShowMsg=2;                                           //Sam 20250115 : 矽格湖口 GPIB OneCycle 要強制切 ASM
//            sOneCycleFinishShowMsg="";
//        }
//    }
}
//---------------------------------------------------------------------------
void TfProductionInfo::SaveInfoFileWhenStart()                                  //Jimmychiu 20211109 : (Upate)Handler 實際運轉Site資料討論
{
    //AI(BVL-3766) 20260420: 本功能為超豐(GTK Loader Agent)專用，非 CC_Greatek 不寫 GTK_Control\flag\info.txt
    if(CUSTOMER_CODE!=CC_Greatek)
        return;
    AnsiString sTemp="";
    TStringList *tslFile=new TStringList();
    AnsiString sFilePath="C:\\GTK_Control\\flag\\";
    AnsiString sFilePathName="info.txt";
    AnsiString sFilePathNameBack="infoBack.txt";                                //Jimmychiu 20230704 : 避免同時讀取造成異常
    sFilePathName=sFilePath+sFilePathName;
    sFilePathNameBack=sFilePath+sFilePathNameBack;
    MyForceDirectories(sFilePath);
    tslFile->Clear();
    //MO
    sTemp="MO:"+sLoadMO_MO;
    tslFile->Add(sTemp);
    //LOT
    sTemp="Lot:"+sLoadMO_LOT;
    tslFile->Add(sTemp);
    //SetupFile
    sTemp="SetupFile:"+GetLastOpenFN();
    tslFile->Add(sTemp);
    //RunSite
    sTemp="RunSite:";
    bool bfirstTime=true;
    //AI(BVL-3766) 20260420: site 編號改用欄交錯(column-major, A奇B偶)，與機台 UI/吸嘴索引慣例(i + j*iShtRow + 1)一致
    //                       原列優先寫法在 2x8 layout 會把客戶關掉的 A,d / A,g 顯示成 4 / 7，並使 B,e 被誤報為 13
    //AI(BVL-3766) 20260428: One Side / Use Shuttle2 模式下只用 Arm2(Z=1)，
    //                       原本只讀 LastSet.bUseTestSocket[0] 會讓 RunSite 全空，
    //                       改成兩臂 OR 並同時參考 runtime 旗標 bTestSiteUse，
    //                       涵蓋 IPSC SiteMap 只更新 runtime / 不寫回 LastSet 的情境。
    //AI(ht9045-v899) 20260519: output RunSite from V812 channel map while keeping two-arm/runtime close-site guard.
    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            bool bUseSite=( bTestSiteUse[0][i][j] || bTestSiteUse[1][i][j] ||
                            LastSet.bUseTestSocket[0][i][j] || LastSet.bUseTestSocket[1][i][j] );
            if(bUseSite)
            {
                int iSiteNo=TestIF_File.iSiteMap[i][j];
                if(bfirstTime)
                {
                    sTemp+=IntToStr(iSiteNo);
                    bfirstTime=!bfirstTime;
                }
                else
                {
                    sTemp+=(","+IntToStr(iSiteNo));
                }
            }
        }
    }
    tslFile->Add(sTemp);
    tslFile->SaveToFile(sFilePathNameBack);
    CopyFile(sFilePathNameBack.c_str(),sFilePathName.c_str(),false);            //Jimmychiu 20230704 : 避免同時讀取造成異常
    delete tslFile;
}
//---------------------------------------------------------------------------
void TfProductionInfo::CopyIPSCback2IPSC()                                      //JimmyChiu 20211227 : 讀取IPSC ini file(第一次執行讀取最後一次ini檔)
{
    if(IniConfig.asN14_16_FlagFilePath=="" ||
       FileExists(IniConfig.asN14_16_FlagFilePath))
    {
        return;
    }
    else
    {
        AnsiString asIPSCINIBackFile=ExtractFilePath(IniConfig.asN14_16_FlagFilePath)+"IPSC_back.ini";
        if(FileExists(asIPSCINIBackFile)==false)
            return;
        CopyFile( asIPSCINIBackFile.c_str(),IniConfig.asN14_16_FlagFilePath.c_str(), false);
    }
}
//---------------------------------------------------------------------------
void TfProductionInfo::CheckFTPFilePath(AnsiString &aspath)                     //JimmyChiu 20220111 check FTP Sources File Path
{
    aspath=StringReplace(aspath, "\\", "//", TReplaceFlags()<<rfReplaceAll);
    aspath=StringReplace(aspath, "////", "//", TReplaceFlags()<<rfReplaceAll);
    if(aspath.SubString(aspath.Length()-1,2)!="//")
        aspath=aspath+AnsiString("//");
}
//---------------------------------------------------------------------------
void TfProductionInfo::CheckNewDayAndSubtract(int &iTimeSec)                    //JimmyChiu 20220115 跨日後重頭計算時間
{
    iTimeSec=iTimeSec%86400;
}
//---------------------------------------------------------------------------
bool TfProductionInfo::CheckContactForceExist()                                 //JimmyChiu 20220117 確認Server是否設定，如無設定依照原設定
{
    if( sDevice_Pin_Force==""  ||
        sDevice_Pin_Force=="0" ||
        sDevice_Pin_Count==""  ||
        sDevice_Pin_Count=="0" ||
        IniConfig.bD06ContactOffsetDefaultValue==false
    )
        return false;
    else
        return true;
}
//---------------------------------------------------------------------------
void TfProductionInfo::SetOffsetContactForce(AnsiString asValue)                //JimmyChiu 20220114 : Index 總壓力 Offset，加總後數值不顯示於Contact Form
{
    if(asValue=="")
        asValue="0";
    dOffsetContactForce=atof(asValue.c_str());
}
//---------------------------------------------------------------------------
void TfProductionInfo::SetControlBin(AnsiString asFullFileName, AnsiString asHW_BinNumber, AnsiString asInterValTime, AnsiString asInterValCount)
{
    //Sam 20201209 : 修改 Control Bin 讀取方式
    //==>
    TStringList *sCondition_List;
    AnsiString sHWControl="";
    AnsiString sControlBinCondition="";
    iControlBinNum=atoi(asHW_BinNumber.c_str());                                //監控幾組 Control Bin
    iControlBinCheckPoint=atoi(asInterValTime.c_str())*60;                      //計算時間區間
    iControlBinCheckCount=atoi(asInterValCount.c_str());                        //計算數量區間
    if(iControlBinNum>0)
    {
        sCondition_List=new TStringList();
        for(int i=0; i<iControlBinNum; i++)
        {
            sCondition_List->Clear();
            sHWControl.sprintf("H/W Control%d", i+1);
            sControlBinCondition=GetStringBySeparatedValues(asFullFileName,sHWControl,":");
            sCondition_List->CommaText=sControlBinCondition;
            if(sCondition_List->Count>=4)
            {
                for(int j=0; j<sCondition_List->Count; j++)
                {
                    if(j==0)                                                    //Control Bin
                    {
                        iControlBin[i]=atoi(sCondition_List->Strings[j].c_str());
                    }
                    else if(j==1)                                               //ControlPercent
                    {
                        if(sCondition_List->Strings[j]!="NA")
                            dControlPercent[i]=atof(sCondition_List->Strings[j].c_str());
                        else
                            dControlPercent[i]=0.0;
                    }
                    else if(j==2)                                               //ControlCount
                    {
                        if(sCondition_List->Strings[j]!="NA")
                        {
                            iControlCount[i]=atoi(sCondition_List->Strings[j].c_str());
                        }
                        else
                        {
                            iControlCount[i]=0;
                        }
                    }
                    else if(j==3)//ControlSiteToSite
                    {
                        if(sCondition_List->Strings[j]!="NA")
                            dControlSiteToSite[i]=atof(sCondition_List->Strings[j].c_str());
                        else
                            dControlSiteToSite[i]=0.0;
                    }
                }
            }
        }
        sCondition_List->Clear();
        delete sCondition_List;
    }
    //<==
    //Sam 20201209 : 修改 Control Bin 讀取方式
}
//---------------------------------------------------------------------------
bool TfProductionInfo::CheckOEE_WhenStart(AnsiString &asErrorMsg)               //JimmyChiu 20220125 add
{
    if(fAutoTeach->IsRun())
        return false;
    //Sam 20180416 (wei) : 超豐新增可以設定權限，權限以下，按『Start』時前檢查 Tester 必須為 『Online』，和 Real/Dummy 必須為 『Normal』。
    //==>
    if(fSecurity->Insufficient(157, false)==false)
    {
        if((LastSet.iTester!=ON_LINE || LastSet.iRealDummy!=REALLY))
        {
            asErrorMsg="Please Check ON_LINE/REALLY!";
            return false;
        }
    }
    //<==
    //Sam 20180416 (wei) : 超豐新增可以設定權限，權限以下，按『Start』時前檢查 Tester 必須為 『Online』，和 Real/Dummy 必須為 『Normal』
    //Sam 20170810 (Steven) 移植超豐 OEE 功能 form HT-7045
    //==>
    if(IniConfig.bN14_14_AlarmCtrlMachine)                                      //Steven 20190724 : [N14-14] Use alarm control machine
    {
        if(ACM_ReadFlag("Start")!="1")                                          //Start=1
        {
            ACM_WriteMsgAndCallExe("Start");
            if(IniConfig.bN14_1_EnableOEEFunction)                              //Sam 20200525 : Control Bin
            {
                RecordControlBinCount("Pause Control Bin Check");
            }
            asErrorMsg="Alarm control machine stop!";
            return false;
        }
    }

    if(IniConfig.bN14_1_EnableOEEFunction==true)
    {
        if(fSecurity->Insufficient(156, false)==false)                          //Sam 20180416 (wei) : 超豐新增可以設定權限，權限以下，才需要檢查 Lot 狀態。
        {
            if(IsOEEStartLotSuccess()==false)
            {
                asErrorMsg="Please Start Lot!";
                return false;
            }

            if(bNeedLotEndAfterCleanOut==true &&
               _bOEEStartLotSuccess==true &&
               iTrayFeed!=1)
            {
                asErrorMsg="Please End Lot!";
                return false;
            }
        }
    }
    //Sam 20180423 (wei) : MOFile of Yeild Download
    //==>
    if(IsOEEStartLotSuccess()==true)
    {
        if(SetYieldInformation()==false)
        {
            asErrorMsg="Set YieldInformation Failed!";
            return false;
        }
    }
    //<==
    //Sam 20180423 (wei) : MOFile of Yeild Download
    if(IniConfig.bN14_8_ULSetup==true)                                          //Steven 20190621 : OEE Function
    {
        UploadSetupCondition();
    }
    //<==
    //Sam 20170724 (Steven) 移植超豐 OEE 功能 form HT-7045。
    //Sam 20170824 (Steven) 移植超豐 ESD Control 功能 form HT-7045
    //==>
    if(bRunAutoClean==false)                                                    //JimmyChiu 20211020 : Auto alignment mode //Sam 20200319 : 自動模式觸發 AutoClean 功能不用檢查 ESD
    {
        if(ESDForm->SearchAndCheckESD()==false)
        {
            asErrorMsg="Check ESD Failed!";
            return false;
        }
    }
    //<==
    //Sam 20170824 (Steven) 移植超豐 ESD Control 功能 form HT-7045
    return true;
}
//---------------------------------------------------------------------------
void TfProductionInfo::ClickPause()                                             //JimmyChiu 20220125 add
{
    if(IniConfig.bN14_1_EnableOEEFunction==true &&
       IniConfig.iN14_1_OEERecordCycleTime>0)
    {
        RecordControlBinCount("Pause Control Bin Check");                       //Sam 20200525 : Control Bin
        if(bPauseTimeNeedSaveAndUpdateOEEFiles==true)
        {
            EachCycleSecondDo_SaveAndUpdateOEEFiles(true);
            bPauseTimeNeedSaveAndUpdateOEEFiles=false;
            bStartNeedSaveAndUpdateOEEFiles=true;
            ShowPauseStatusForm();
        }
        bIsPauseTime=true;
    }
}
//---------------------------------------------------------------------------
int TfProductionInfo::GetNowTimeSec()                                           //JimmyChiu 20220125 add
{
    DecodeTime(Now(), h, n, s, z);
    return h*3600+n*60+s;
}
//---------------------------------------------------------------------------
bool TfProductionInfo::DoSetUpFileFromServer()                                  //JimmyChiu 20220303 : Add class SetUpConfiguration
{
    if(IniConfig.bN14_21_SetUpConfiguration)
    {
        if(sSiteMap=="" && sBinCategory=="" && sTrayForm=="")
        {
           ShowMyMessage(AnsiString(__FUNC__)+" SiteMap、BinCategory and TrayForm are Empty");
           return false;
        }
        else if(sSiteMap=="")
        {
            asTempstring=AnsiString(__FUNC__)+" SiteMap value is Empty";
            ShowMyMessage(asTempstring);
            return false;
        }
        else if(sBinCategory=="")
        {
            asTempstring=AnsiString(__FUNC__)+" BinCategory value is Empty";
            ShowMyMessage(asTempstring);
            return false;
        }
        else if(sTrayForm=="")
        {
            asTempstring=AnsiString(__FUNC__)+" Tray Form value is Empty";
            ShowMyMessage(asTempstring);
            return false;
        }
        else
        {
            AnsiString asSetUpName=sTrayForm+"_"+sSiteMap+"_"+sBinCategory;
            if(asSetUpName!=GetSetUpName())                                     //check set up file is diff
            {
                if(DirectoryExists(DataPath+asSetUpName)==false)                //copy now receipe and create new
                {
                    if(DoSetRecipe(sSiteMap,sBinCategory,sTrayForm)==false)
                    {
                        return false;
                    }

                    if(sSiteMap=="GTK04")
                    {
                        sSiteMap="GTK0D";
                        if(DoSetRecipe(sSiteMap,sBinCategory,sTrayForm)==false)
                        {
                            return false;
                        }
                        fMain->cbSetupFileName->Clear();
                        bN14_21_UsedGDK04=true;
                    }
                }
                else
                {
                    fMain->ChangeSetUpFile(asSetUpName);
                }
                sLoadMO_SetupFileName=asSetUpName;
                #ifndef SOFT_SIMULTE
                fObserver->bShowMajorMaintenanceRecord=true;
                fObserver->ShowModal();
                fObserver->bShowMajorMaintenanceRecord=false;
                if(CosFunction.bManualSteplAutoTeach &&
                   IniConfig.bA56EnableAutoTeachFunciton)                       //JimmyChiu 20211020 : Auto alignment mode
                    fAutoTeach->DoAutoTeachStart();
                #endif
            }
            return true;
        }
    }
    return false;
}
//---------------------------------------------------------------------------
bool TfProductionInfo::DoSetRecipe(AnsiString sSiteMap,AnsiString sBinCategory,AnsiString sTrayForm)                                   //JimmyChiu 20220303 : Add class SetUpConfiguration
{
    if(IniConfig.bN14_21_SetUpConfiguration)
    {
        AnsiString asSetUpName=sTrayForm+"_"+sSiteMap+"_"+sBinCategory;
        if(LoadSiteMapFromServer(sSiteMap)          ==false ||
           LoadBinCategoryFromServer(sBinCategory)  ==false ||
           LoadHotPlateFormFromServer()             ==false ||
           LoadTrayFormFromServer(sTrayForm)    ==false )
        {
            return false;
        }

        if(CopyFolder(DataPath+GetSetUpName()+"\\",DataPath+asSetUpName+"\\")==false)
        {
            asTempstring=AnsiString(__FUNC__)+"【Error】CopyFolder From="+DataPath+GetSetUpName()+"\\  To="+DataPath+asSetUpName+"\\";
            ShowMyMessage(asTempstring);
            return false;
        }
        fMain->ChangeSetUpFile(asSetUpName);
        if(LoadSiteMapFromServer(sSiteMap))
            SettingSiteMapFromServer();
        if(LoadBinCategoryFromServer(sBinCategory))
            SettingBinCategoryFromServer();
        if(LoadHotPlateFormFromServer())
            SettingHotPlateFormFromServer();
        if(LoadTrayFormFromServer(sTrayForm))
            SettingTrayFormFromServer();
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
const int iStackFail_Pass_Length=9;
//---------------------------------------------------------------------------
AnsiString TfProductionInfo::GetBinTraySetting()
{
    AnsiString asBack="";
    TStringList* sBinTraySetT3Pos=new TStringList();
    for(int i=0; i<16; i++)
    {
        sBinTraySetT3Pos->Add("0");
    }
    AnsiString asHARDWARE_BIN[iStackFail_Pass_Length]=
    {
        sucSetUp.sAUTO_1_CAT_A_HARDWARE_BIN, sucSetUp.sAUTO_2_CAT_B_HARDWARE_BIN, sucSetUp.sAUTO_3_CAT_C_HARDWARE_BIN,
        sucSetUp.sFIX_1_CAT_D_HARDWARE_BIN,  sucSetUp.sFIX_2_CAT_E_HARDWARE_BIN,  sucSetUp.sFIX_3_JAM_HARDWARE_BIN,
        sucSetUp.sFIX_4_HARDWARE_BIN,        sucSetUp.sFIX_5_HARDWARE_BIN,        sucSetUp.sFIX_6_HARDWARE_BIN
    };
    for(int i=0; i<iStackFail_Pass_Length; i++)
    {
        if(SetBinTraySetting(sBinTraySetT3Pos, asHARDWARE_BIN[i], IntToStr(i+1)))
            BinSelect[eBinFT].IfErrorT3=i;
    }
    asBack=sBinTraySetT3Pos->CommaText;
    delete sBinTraySetT3Pos;
    return asBack;
}
//---------------------------------------------------------------------------
bool TfProductionInfo::SetBinTraySetting(TStringList* tlBinTray,AnsiString asSource,AnsiString asBin)
{
    bool bGetE=false;
    if(asSource.Trim()=="")
        return bGetE;
    int iTemp=0;
    asSource=asSource.Trim();
    TStringList* tempTL=new TStringList();
    tempTL->CommaText=asSource;
    AnsiString asString="";
    for(int i=0; i<tempTL->Count; i++)
    {
        asString=tempTL->Strings[i];
        if(asString=="E")
        {bGetE=true;}
        else
        {
            iTemp=atoi(tempTL->Strings[i].c_str());
            iTemp=iTemp>15?0:iTemp;
            tlBinTray->Strings[iTemp]=asBin;
        }
    }
    delete tempTL;
    return bGetE;
}
//---------------------------------------------------------------------------
bool TfProductionInfo::LoadHotPlateFormFromServer()                             //JimmyChiu 20220303 : Add class SetUpConfiguration
{
    AnsiString sSourcesFilePath=IniConfig.asN14_21_HP_SetUpConfig;
    AnsiString sTargetFilePath;
    #ifdef SOFT_SIMULTE
//    sSourcesFilePath="//handler_data//SETUP_FILE//";
    #endif
    IncludeTrailingPathDelimiter(sSourcesFilePath);
    CheckFTPFilePath(sSourcesFilePath);
    sTargetFilePath=GetTargetFilePath_HotPlate();
    MyForceDirectories(sTargetFilePath);
    if(FTP_MOFile_Download(sSourcesFilePath,sTargetFilePath, ".csv"))
    {
        return true;
    }
    else
    {
        RecordProcess(AnsiString().sprintf("Func:%s ,Sources File Path:%s", __FUNC__, sSourcesFilePath));
    }
    return false;
}
//---------------------------------------------------------------------------
void TfProductionInfo::SettingHotPlateFormFromServer()                          //JimmyChiu 20220303 : Add class SetUpConfiguration
{
    if(FileExists(PlateTablePath))                                              //Steven 20210629 : HP Form改成CSV
    {
        fConfiguration->sbtReloadHP->Click();
        fHotPlate->ShowTypePage(1);
        fHotPlate->spbSave->Click();
    }
}
//---------------------------------------------------------------------------
void TfProductionInfo::OverWritePlateFormData(AnsiString FilePath, AnsiString TrayPN, AnsiString XStartPos,
                                               AnsiString YStartPos, AnsiString XPitch, AnsiString YPitch,
                                               AnsiString ColumnsX, AnsiString RowsY, AnsiString XWidth,
                                               AnsiString YHeight, AnsiString ZThickness, AnsiString Group,
                                               AnsiString Memo, AnsiString BlockNumberX, AnsiString BlockNumberY,
                                               AnsiString BlockPitchX, AnsiString BlockPitchY)
{
    TStringList *sList = new TStringList();
    AnsiString asTemp = "";
    asTemp = "Package Type,X Start Pos,Y Start Pos,X Pitch,Y Pitch,Columns (X),Rows (Y),X Width,Y Height,Z Tray Tickness,Group,Memo,BlockNumberX,BlockNumberY,BlockPitchX,BlockPitchY,";
    sList->Add(asTemp);
    asTemp = TrayPN + "," + XStartPos + "," + YStartPos + "," + XPitch + "," + YPitch + "," +
             ColumnsX + "," + RowsY + "," + XWidth + "," + YHeight + "," + ZThickness + "," +
             Group + "," + Memo + "," + BlockNumberX + "," + BlockNumberY + "," +
             BlockPitchX + "," + BlockPitchY + ",";
    sList->Add(asTemp);
    sList->SaveToFile(FilePath);
    sList->Clear();
    delete sList;
}
//---------------------------------------------------------------------------
bool TfProductionInfo::LoadTrayFormFromServer(AnsiString asFileName)            //JimmyChiu 20220303 : Add class SetUpConfiguration
{
    AnsiString sSourcesFilePath=IniConfig.asN14_21_TF_SetUpConfig;
    AnsiString sTargetFilePath;
    #ifdef SOFT_SIMULTE
//    sSourcesFilePath="//handler_data//SETUP_FILE//";
    #endif
    IncludeTrailingPathDelimiter(sSourcesFilePath);
    CheckFTPFilePath(sSourcesFilePath);
    sTargetFilePath=GetTargetFilePath_TrayForm();
    MyForceDirectories(sTargetFilePath);
    if(FTP_MOFile_Download(sSourcesFilePath,sTargetFilePath,asFileName+".bin"))
    {
        if(sucTrayForm.ReadFile(sTargetFilePath+asFileName+".bin"))
        {
            return true;
        }
        else
            ShowMyMessage("Load Tray Form File From Server Failed! Server:"+sSourcesFilePath+" FileName:"+asFileName+".bin");
    }
    else
    {
        RecordProcess(AnsiString().sprintf("Func:%s ,Name:%s",__FUNC__,asFileName));
    }
    return false;
}
//---------------------------------------------------------------------------
void TfProductionInfo::SettingTrayFormFromServer()                              //JimmyChiu 20220303 : Add class SetUpConfiguration
{
    int iIndex=sucTrayForm.AddAutoDownloadTrayFormData();
    if(FileExists(TrayTablePath))                                               //Steven 20210629 : HP Form改成CSV
    {
        fConfiguration->sbtReloadTray->Click();
        fTrayForm->ShowTypePage(0, iIndex);
        fTrayForm->spbSave->Click();
    }
}
//---------------------------------------------------------------------------
void TfProductionInfo::SettingTrayFormFromServerOnlyOne()                       //JimmyChiu 20220303 : Add class SetUpConfiguration
{
    sucTrayForm.OverWriteTrayFormData();
    if(FileExists(TrayTablePath)) //Steven 20210629 : HP Form改成CSV
    {
        fConfiguration->sbtReloadTray->Click();
        fTrayForm->ShowTypePage(0,1);
        fTrayForm->spbSave->Click();
    }
}
//---------------------------------------------------------------------------
AnsiString TfProductionInfo::GetSetUpName()                                     //JimmyChiu 20220303 : Add class SetUpConfiguration
{
    return fMain->cbSetupFileName->Text;
}
//---------------------------------------------------------------------------
bool TfProductionInfo::CopyFolder(AnsiString asSource, AnsiString asTarget)
{
    TSearchRec sr;
    if(!DirectoryExists(asSource))
    {
        return false;
    }
    MyForceDirectories(asTarget);
    if(FindFirst(asSource+"*.*", faAnyFile, sr)==0)
    {
        do
        {
            try
            {
               if((sr.Attr & faDirectory)!=0)
               {
                   if (sr.Name!="." && sr.Name!="..")
                   {                                                            //folder
                       CopyFolder(asSource+sr.Name, asTarget+sr.Name);
                   }
               }
               else
               {                                                                //file
                     CopyFile((asSource+sr.Name).c_str(), (asTarget+sr.Name).c_str(), 0);
               }
            }
            catch(...)
            {
                return false;
            }
        }while(FindNext(sr)==0);
        FindClose(sr);
    }
    return true;
}
//---------------------------------------------------------------------------
void TfProductionInfo::SetACM_WriteMsgAndCallExe_sMessage(AnsiString sMessage)
{
    ACM_WriteMsgAndCallExe_sMessage=sMessage;
}
//---------------------------------------------------------------------------
//JimmyChiu 20221226 : SetUpConfiguration HandlerMode splite to SITE MAP and BIN CATEGORY
//<==
bool TfProductionInfo::LoadSiteMapFromServer(AnsiString asFileName)
{
    if(LoadSetUpFileFromServer(IniConfig.asN14_21_SetUpConfiguration,asFileName,GetTargetFilePath_SetSiteMap()))
    {
        return true;
    }
    else
    {
        RecordProcess(AnsiString().sprintf("Func:%s ,Name:%s",__FUNC__,asFileName));
    }
    return false;
}
//---------------------------------------------------------------------------
void TfProductionInfo::SettingSiteMapFromServer()
{
    if(sucSetUp.sHANDLER_TYPE!="HT9046")
    {
        ShowMyMessage("SettingSetUpFileFromServer HANDLER TYPE is not HT9046");
        return;
    }
    //site mode
    if(     sucSetUp.sSITE_MODE=="1X4") TestIF_File.iTestMode=QualSite1X4;
    else if(sucSetUp.sSITE_MODE=="2X2") TestIF_File.iTestMode=QualSite2X2;
    else if(sucSetUp.sSITE_MODE=="1X2") TestIF_File.iTestMode=DualSite;
    else if(sucSetUp.sSITE_MODE=="2X4") TestIF_File.iTestMode=_8Site2X4;
    else if(sucSetUp.sSITE_MODE=="2X8") TestIF_File.iTestMode=_16Site2X8;
    else if(sucSetUp.sSITE_MODE=="4X8") TestIF_File.iTestMode=_32Site4X8N;
    else TestIF_File.iTestMode=_16Site2X8;
    fSetup->ScrollBar1->Position=TestIF.iTestMode=TestIF_File.iTestMode;
    //SiteMap
    TComboBox *tempTestSiteCBox[MAX_SOCKET_ROW][MAX_SOCKET_COL]=
    {
        {fSetup->cbAa, fSetup->cbAb, fSetup->cbAc, fSetup->cbAd, fSetup->cbAe, fSetup->cbAf, fSetup->cbAg, fSetup->cbAh},
        {fSetup->cbBa, fSetup->cbBb, fSetup->cbBc, fSetup->cbBd, fSetup->cbBe, fSetup->cbBf, fSetup->cbBg, fSetup->cbBh},
        {fSetup->cbCa, fSetup->cbCb, fSetup->cbCc, fSetup->cbCd, fSetup->cbCe, fSetup->cbCf, fSetup->cbCg, fSetup->cbCh},
        {fSetup->cbDa, fSetup->cbDb, fSetup->cbDc, fSetup->cbDd, fSetup->cbDe, fSetup->cbDf, fSetup->cbDg, fSetup->cbDh}
    };
    AnsiString asSiteMap[MAX_SOCKET_ROW][MAX_SOCKET_COL]=
    {
        {sucSetUp.sAa, sucSetUp.sAb, sucSetUp.sAc, sucSetUp.sAd, sucSetUp.sAe, sucSetUp.sAf, sucSetUp.sAg, sucSetUp.sAh},
        {sucSetUp.sBa, sucSetUp.sBb, sucSetUp.sBc, sucSetUp.sBd, sucSetUp.sBe, sucSetUp.sBf, sucSetUp.sBg, sucSetUp.sBh},
        {sucSetUp.sCa, sucSetUp.sCb, sucSetUp.sCc, sucSetUp.sCd, sucSetUp.sCe, sucSetUp.sCf, sucSetUp.sCg, sucSetUp.sCh},
        {sucSetUp.sDa, sucSetUp.sDb, sucSetUp.sDc, sucSetUp.sDd, sucSetUp.sDe, sucSetUp.sDf, sucSetUp.sDg, sucSetUp.sDh}
    };
    for(int i=0; i<MAX_SOCKET_ROW; i++)
    {
        for(int j=0; j<MAX_SOCKET_COL; j++)
        {
            tempTestSiteCBox[i][j]->ItemIndex=atoi(asSiteMap[i][j].c_str());
            if(asSiteMap[i][j].Pos("X")>0)
            {
                LastSet.bUseTestSocket[0][i][j]=false;
                LastSet.bUseTestSocket[1][i][j]=false;
            }
            else
            {
                LastSet.bUseTestSocket[0][i][j]=true;
                LastSet.bUseTestSocket[1][i][j]=true;
            }
        }
    }
    //sbUpdate
    fSetup->sbUpdate->Click();
}
//---------------------------------------------------------------------------
bool TfProductionInfo::LoadBinCategoryFromServer(AnsiString asFileName)
{
    if(LoadSetUpFileFromServer(IniConfig.asN14_21_BinCategory, asFileName, GetTargetFilePath_SetBinCategory()))
    {
        return true;
    }
    else
    {
        RecordProcess(AnsiString().sprintf("Func:%s ,Name:%s",__FUNC__,asFileName));
    }
    return false;
}
//---------------------------------------------------------------------------
void TfProductionInfo::SettingBinCategoryFromServer()
{
    if(sucSetUp.sHANDLER_TYPE!="HT9046")
    {
        ShowMyMessage("SettingSetUpFileFromServer HANDLER TYPE is not HT9046");
        return;
    }
    //HARDWARE_BIN
    fBinSel->sBinTraySetT3Pos[eBinFT]->CommaText=GetBinTraySetting();
    //PASS_FAIL
    int iBinSelectLength=sizeof(BinSelect[iTestRunMode].iStackDefFailCate)/sizeof(BinSelect[iTestRunMode].iStackDefFailCate[0]);
    AnsiString asStackFail_Pass[iStackFail_Pass_Length]=
    {
        sucSetUp.sAUTO_1_CAT_A_PASS_FAIL, sucSetUp.sAUTO_2_CAT_B_PASS_FAIL, sucSetUp.sAUTO_3_CAT_C_PASS_FAIL,
        sucSetUp.sFIX_1_CAT_D_PASS_FAIL,  sucSetUp.sFIX_2_CAT_E_PASS_FAIL,  sucSetUp.sFIX_3_JAM_PASS_FAIL,
        sucSetUp.sFIX_4_PASS_FAIL,        sucSetUp.sFIX_5_PASS_FAIL,        sucSetUp.sFIX_6_PASS_FAIL
    };
    for(int i=0;i<iStackFail_Pass_Length && i<iBinSelectLength;i++)             //AI(ht9045-v899) 20260820: bound loop by asStackFail_Pass length(9); iBinSelectLength is eTrayCount(33) and overran the local array
    {
        BinSelect[eBinFT].iStackDefFailCate[i]=(asStackFail_Pass[i]=="Pass")?0:1;
    }
    fBinSel->Save(3617, eBinFT);
    fBinSel->Save(3616, eBinFT);
}
//---------------------------------------------------------------------------
bool TfProductionInfo::LoadSetUpFileFromServer(AnsiString sSourcesFilePath, AnsiString asFileName, AnsiString asTargetPath)
{
    AnsiString sTargetFilePath="";
    #ifdef SOFT_SIMULTE
//    sSourcesFilePath="//handler_data//SETUP_FILE//";
    #endif
    IncludeTrailingPathDelimiter(sSourcesFilePath);
    CheckFTPFilePath(sSourcesFilePath);
    IncludeTrailingPathDelimiter(sSourcesFilePath);
    sTargetFilePath=asTargetPath;
    MyForceDirectories(sTargetFilePath);
    if(FTP_MOFile_Download(sSourcesFilePath,sTargetFilePath,asFileName+".bin"))
    {
        if(sucSetUp.ReadSetUpFile(sTargetFilePath+asFileName+".bin"))
        {
            return true;
        }
        else
            ShowMyMessage(AnsiString(__FUNC__)+" Failed! "+sTargetFilePath+asFileName+".bin");
    }
    return false;
}
//---------------------------------------------------------------------------
//<==
//JimmyChiu 20221226 : SetUpConfiguration HandlerMode splite to SITE MAP and BIN CATEGORY
//JimmyChiu 20230410 : Config update from server
//<==
bool TfProductionInfo::LoadConfigFromServer()
{
    if(DownloadFilterFileToServer(IniConfig.asN14_22_ConfigUpdateFromServerImport, "config_Standard.ini", AuthPath))
    {
        AnsiString asFilepath=AuthPath+"config_Standard.ini";
        ReadWriteIni(asFilepath, "PrecautionRecord","asB01_PrecautionRecordSavePath",               IniConfig.asB01_PrecautionRecordSavePath, "", false);
        ReadWriteIni(asFilepath, "PrecautionRecord","asB02_HanderMajorMaintenanceRecordSavePath",   IniConfig.asB02_HanderMajorMaintenanceRecordSavePath, "", false);
        ReadWriteIni(asFilepath, "ESD_Control",     "N15_ESDControlMachineSaveRecordFilePath",      IniConfig.asN15UserLevelByTxtReadFilePath, "", false);
        ReadWriteIni(asFilepath, "Handler_OEE",     "N14_HandlerOEESaveProductionDataToPath",       IniConfig.asN14_2_OEESaveProdPath, "", false);
        fConfiguration->ReadConfigStandard();
        ReadLastSetIni();
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
bool TfProductionInfo::UploadConfigToServer()
{
    if(DoCompareConfigLocalAndCloud())
    {
        return UploadSingleFileToServer(AuthPath,"config_Standard.ini", IniConfig.asN14_22_ConfigUpdateFromServerExport);
    }
    else
        return false;
}
//---------------------------------------------------------------------------
bool TfProductionInfo::UploadSingleFileToServer(AnsiString sSourcesFilePath, AnsiString asFileName, AnsiString asTargetPath)
{
    bool bReturn=false;
    AnsiString asError="";
    if(sSourcesFilePath.Trim()=="" || asTargetPath.Trim()=="")
    {
        asError="Handler OEE FilePath or FileName Empty Error!";
        ShowMyMessage(asError);
        return false;
    }
    AnsiString sTempSource=sSourcesFilePath;
    AnsiString sTempTarget=asTargetPath;
    AnsiString asUserID="",asPassword="",asHost="";
    GetFTP_Setting(asUserID, asPassword, asHost);                               //Jimmychiu 20240104 : 優化FTP連線Function
    TfFTP fFTP(asUserID,asPassword,asHost);
    if(fFTP.Connect())
    {
        AnsiString asError=AnsiString("");
        fFTP.CheckLocalFilePath(sTempSource);
        if(FileExists(sTempSource+asFileName))                                  //Sam 20180802 (wei) : 增加保護
        {
            fFTP.Upload(sTempSource, sTempTarget, asFileName, asError);
            bReturn=true;
        }
        else
        {
            bReturn=false;
            asError=AnsiString(__FUNC__)+AnsiString().sprintf(" SourcesFilePath=%s,FileName=%s,TargetPath=%s", sSourcesFilePath, asFileName, asTargetPath);
            ShowMyMessage(asError);
        }
    }
    else
    {
        bReturn=false;
        asError=AnsiString(__FUNC__)+AnsiString().sprintf(" Connect Failed,SourcesFilePath=%s,FileName=%s,TargetPath=%s", sSourcesFilePath, asFileName, asTargetPath);
    }
    return bReturn;
}
//---------------------------------------------------------------------------
bool TfProductionInfo::DownloadFilterFileToServer(AnsiString sSourcesFilePath, AnsiString asFileName, AnsiString asTargetPath)
{
    bool bReturn=false;
    AnsiString asError="";
    if(sSourcesFilePath.Trim()=="" || asTargetPath.Trim()=="")
    {
        asError="Handler OEE FilePath or FileName Empty Error!";
        ShowMyMessage(asError);
        return false;
    }
    AnsiString sTempSource=sSourcesFilePath;
    AnsiString sTempTarget=asTargetPath;
    AnsiString asUserID="", asPassword="", asHost="";
    GetFTP_Setting(asUserID, asPassword, asHost);                               //Jimmychiu 20240104 : 優化FTP連線Function
    TfFTP fFTP(asUserID, asPassword, asHost);
    if(fFTP.Connect())
    {
        fFTP.CheckLocalFilePath(sTempSource);
        if(fFTP.DownloadFilterFile(sTempSource, sTempTarget, asFileName, asError))
        {
            bReturn=true;
        }
        else
        {
            bReturn=false;
            asError+=AnsiString(__FUNC__)+AnsiString().sprintf(" SourcesFilePath=%s,FileName=%s,TargetPath=%s", sSourcesFilePath, asFileName, asTargetPath);
            ShowMyMessage(asError);
        }
    }
    else
    {
        bReturn=false;
        asError=AnsiString(__FUNC__)+AnsiString().sprintf(" Connect Failed,SourcesFilePath=%s,FileName=%s,TargetPath=%s", sSourcesFilePath, asFileName, asTargetPath);
    }
    return bReturn;
}
//---------------------------------------------------------------------------
//<==
//JimmyChiu 20230410 : Config update from server
//---------------------------------------------------------------------------
void TfProductionInfo::CloseAllPage()
{
    for(int i=0; i<PageControl1->PageCount; i++)
    {
        PageControl1->Pages[i]->TabVisible=false;
    }
}
//---------------------------------------------------------------------------
int TfProductionInfo::ShowSheet(TTabSheet *tsShow)
{
    CloseAllPage();
    PageControl1->ActivePage=tsShow;
    return ShowModal();
}
//---------------------------------------------------------------------------
void TfProductionInfo::InitialsgDatas()
{
    sgDatas->RowCount=1;
    sgDatas->ColCount=1;
    AddDBColHead("Parameter", 300);
    AddDBColHead("Local", 200);
    AddDBColHead("Cloud", 200);
    sgDatas->ColCount--;                                                        //cuz grid colcount至少為1
    ClearCompareDataList();
}
//---------------------------------------------------------------------------
void TfProductionInfo::AddDBColHead(AnsiString asName,int iColWidth)
{
    sgDatas->Cells[sgDatas->ColCount-1][0]=asName;                              //[col][row]
    sgDatas->ColWidths[sgDatas->ColCount-1]=iColWidth;                          //[col][row]
    sgDatas->ColCount++;
}
//---------------------------------------------------------------------------
void TfProductionInfo::ClearCompareDataList()
{
    for(int i=0;i<CompareDataList->Count;i++)
    {
        TStringList* tempsl=(TStringList*)CompareDataList->Items[i];
        delete tempsl;
    }
    CompareDataList->Clear();
}
//---------------------------------------------------------------------------
bool TfProductionInfo::DoCompareConfigLocalAndCloud()
{
    bool bReturn=false;
    AnsiString asLocal=GetProInfoFilePath()+"\\";
    if(DownloadFilterFileToServer(IniConfig.asN14_22_ConfigUpdateFromServerExport, "config_Standard.ini", asLocal))
    {
        AnsiString asFilepath=AuthPath+"config_Standard.ini";
        AnsiString asError="";
        asLocal+="config_Standard.ini";
        InitialsgDatas();
        bool b1=elConfig->ReadAndCompareDataFromFile(asFilepath, asLocal, CompareDataList, asError);
        if(b1)
        {
            for(int i=0; i<CompareDataList->Count; i++)
            {
                TStringList* tslDiffcontent=(TStringList*)CompareDataList->Items[i];
                if(tslDiffcontent->Count==3)
                {
                    AddData2Grid(tslDiffcontent->Strings[0], tslDiffcontent->Strings[1], tslDiffcontent->Strings[2]);
                }
            }
            ShowCompareDataForm();
            if(bAgreeExportConfig==true)
            {
                bReturn=true;
            }
            else
            {
                bReturn=false;
            }
        }
        else
        {
            ShowMyMessage(AnsiString(__FUNC__)+"_"+asError);
            bReturn=false;
        }
    }
    else
    {
        bReturn=true;                                                           //無資料比較就直接上傳
    }
    return bReturn;
}
//---------------------------------------------------------------------------
void TfProductionInfo::AddData2Grid(AnsiString asContent, AnsiString asLocal, AnsiString asCloud)
{
    sgDatas->RowCount++;
    int inowRow=sgDatas->RowCount;
    sgDatas->Cells[0][inowRow-1]=asContent;                                     //[col][row]
    sgDatas->Cells[1][inowRow-1]=asLocal;                                       //[col][row]
    sgDatas->Cells[2][inowRow-1]=asCloud;                                       //[col][row]
}
//---------------------------------------------------------------------------
void __fastcall TfProductionInfo::btnSummitClick(TObject *Sender)
{
    bAgreeExportConfig=true;
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfProductionInfo::btnCancelClick(TObject *Sender)
{
    bAgreeExportConfig=false;
    Close();
}
//---------------------------------------------------------------------------
bool TfProductionInfo::IsContinueFailAlarm()                                    //Jimmychiu 20230919 : Close Continue Fail In RT mode
{
    if(sLoadMO_TestFlow.SubString(1, 1)=="R")
    {
        return false;
    }
    else
    {
        return true;
    }
}
//---------------------------------------------------------------------------
void TfProductionInfo::SetRTColorSensorEnable(AnsiString snot_RT_BIN)           //JimmyChiu 20231127 : Set color sensor parameter by server
{                                                                               /*0:yellow 1:blue 2:green 3:red 4:None 5:brown 6.orange 7:none 8:none 9:none 10:none*/
    uColorSensorInfo* uCSI=fTrayForm->GetColorSensor("ColorSensor_RT");
    uCSI->SetAllEnable(false);                                                  //None,Brown,Orange,Red,Yellow,White,Green,Blue,Black
    AnsiString sMatchWord="N";
    for(int i=0;i<snot_RT_BIN.Length();i++)
    {
        if(i==0 && snot_RT_BIN.SubString(i+1, 1)==sMatchWord)                   //Yellow
        {
            uCSI->SetColorEnable("Yellow", true);
        }
        else if(i==1 && snot_RT_BIN.SubString(i+1, 1)==sMatchWord)              //Blue
        {
            uCSI->SetColorEnable("Blue", true);
        }
        else if(i==2 && snot_RT_BIN.SubString(i+1, 1)==sMatchWord)              //Green
        {
            uCSI->SetColorEnable("Green", true);
        }
        else if(i==3 && snot_RT_BIN.SubString(i+1, 1)==sMatchWord)              //Red
        {
            uCSI->SetColorEnable("Red", true);
        }
        else if(i==5 && snot_RT_BIN.SubString(i+1, 1)==sMatchWord)              //Brown
        {
            uCSI->SetColorEnable("Brown", true);
        }
        else if(i==6 && snot_RT_BIN.SubString(i+1, 1)==sMatchWord)              //Orange
        {
            uCSI->SetColorEnable("Orange", true);
        }
    }
    uCSI->SaveColorSensorEnable();
    fTrayForm->spbSaveClick(this);
}
//---------------------------------------------------------------------------
void TfProductionInfo::GetFTP_Setting(AnsiString &asUserID, AnsiString &asPassword, AnsiString &asHost) //Jimmychiu 20231223 : add FTP Setting function
{
    #ifdef SOFT_SIMULTE
    asUserID="HONPREC";
    asPassword="27025312";
    asHost="127.0.0.1";
    #else
    asUserID=IniConfig.asN14_3_OEEFTPUserName;
    asPassword=IniConfig.asN14_3_OEEFTPPassword;
    asHost=IniConfig.asN14_3_OEEFTPHost;
    #endif
}
//---------------------------------------------------------------------------
AnsiString TfProductionInfo::GetScheduleName()                                  //Jimmychiu 20240119 : ScheduleName = Mo+Flow
{
    return sLoadMO_MO+"_"+sLoadMO_TestFlow;
}
//---------------------------------------------------------------------------
AnsiString TfProductionInfo::GetNowTime()                                       //Jimmychiu 20231218 : Greatek add error of each position
{
    TDateTime _now=utimetool.GetNow();
    return utimetool.TDatetmeToAnsiLog(_now);
}
//---------------------------------------------------------------------------
void TfProductionInfo::SetPISTime()                                             //Jimmychiu 20231218 : Greatek add error of each position
{
    sPI_STime=GetNowTime();
}
//---------------------------------------------------------------------------
AnsiString TfProductionInfo::GetPISTime()                                       //Jimmychiu 20231218 : Greatek add error of each position
{
    return sPI_STime;
}
//---------------------------------------------------------------------------
void TfProductionInfo::InitAutoCalSuckZ()
{
    bool bShow=true, bEnable=true, bReadFromFile=true;//, bFixedValue=false, bDisable=false;
    InArmAutoCalSuckZPoint.Clear();
    OutArmAutoCalSuckZPoint.Clear();
    //in arm
    elData->Add(cbInarmSuckZAutoEnable,     &bEnableInarmSuckZAuto,       ECBool,     "ArmSuckZAuto", "bEnableInarmSuckZAuto",  bShow, bEnable, bReadFromFile, 0);
    elData->Add(setEditZ1A,                 &iInArmZHeightDiff[0][0],     ECInteger,  "ArmSuckZAuto", "iInArmZHeightDiff0_0",   bShow, bEnable, bReadFromFile, 0,     false, -500, 500);
    elData->Add(setEditZ1B,                 &iInArmZHeightDiff[1][0],     ECInteger,  "ArmSuckZAuto", "iInArmZHeightDiff1_0",   bShow, bEnable, bReadFromFile, 0,     false, -500, 500);
    elData->Add(setEditZ1C,                 &iInArmZHeightDiff[0][1],     ECInteger,  "ArmSuckZAuto", "iInArmZHeightDiff0_1",   bShow, bEnable, bReadFromFile, 0,     false, -500, 500);
    elData->Add(setEditZ1D,                 &iInArmZHeightDiff[1][1],     ECInteger,  "ArmSuckZAuto", "iInArmZHeightDiff1_1",   bShow, bEnable, bReadFromFile, 0,     false, -500, 500);
    elData->Add(setEditZ1E,                 &iInArmZHeightDiff[0][2],     ECInteger,  "ArmSuckZAuto", "iInArmZHeightDiff0_2",   bShow, bEnable, bReadFromFile, 0,     false, -500, 500);
    elData->Add(setEditZ1F,                 &iInArmZHeightDiff[1][2],     ECInteger,  "ArmSuckZAuto", "iInArmZHeightDiff1_2",   bShow, bEnable, bReadFromFile, 0,     false, -500, 500);
    elData->Add(setEditZ1G,                 &iInArmZHeightDiff[0][3],     ECInteger,  "ArmSuckZAuto", "iInArmZHeightDiff0_3",   bShow, bEnable, bReadFromFile, 0,     false, -500, 500);
    elData->Add(setEditZ1H,                 &iInArmZHeightDiff[1][3],     ECInteger,  "ArmSuckZAuto", "iInArmZHeightDiff1_3",   bShow, bEnable, bReadFromFile, 0,     false, -500, 500);

    elData->Add(edInarmTestAreaX,           &InArmAutoCalSuckZPoint.X,    ECInteger,  "ArmSuckZAuto", "InArmAutoCalSuckZPointX",  bShow, bEnable, bReadFromFile, 0);
    elData->Add(edInarmTestAreaY,           &InArmAutoCalSuckZPoint.Y,    ECInteger,  "ArmSuckZAuto", "InArmAutoCalSuckZPointY",  bShow, bEnable, bReadFromFile, 0);
    elData->Add(edInarmSearchStartZ,        &iInArmSearchStartZ,          ECInteger,  "ArmSuckZAuto", "iInArmSearchStartZ",     bShow, bEnable, bReadFromFile, 0);

    //out arm
    elData->Add(cbOutarmSuckZAutoEnable,    &bEnableOutarmSuckZAuto,      ECBool,     "ArmSuckZAuto", "bEnableOutarmSuckZAuto", bShow, bEnable, bReadFromFile, 0);
    elData->Add(setEditZ2A,                 &iOutArmZHeightDiff[0][0],    ECInteger,  "ArmSuckZAuto", "iOutArmZHeightDiff0_0",  bShow, bEnable, bReadFromFile, 0,     false, -500, 500);
    elData->Add(setEditZ2B,                 &iOutArmZHeightDiff[1][0],    ECInteger,  "ArmSuckZAuto", "iOutArmZHeightDiff1_0",  bShow, bEnable, bReadFromFile, 0,     false, -500, 500);
    elData->Add(setEditZ2C,                 &iOutArmZHeightDiff[0][1],    ECInteger,  "ArmSuckZAuto", "iOutArmZHeightDiff0_1",  bShow, bEnable, bReadFromFile, 0,     false, -500, 500);
    elData->Add(setEditZ2D,                 &iOutArmZHeightDiff[1][1],    ECInteger,  "ArmSuckZAuto", "iOutArmZHeightDiff1_1",  bShow, bEnable, bReadFromFile, 0,     false, -500, 500);
    elData->Add(setEditZ2E,                 &iOutArmZHeightDiff[0][2],    ECInteger,  "ArmSuckZAuto", "iOutArmZHeightDiff0_2",  bShow, bEnable, bReadFromFile, 0,     false, -500, 500);
    elData->Add(setEditZ2F,                 &iOutArmZHeightDiff[1][2],    ECInteger,  "ArmSuckZAuto", "iOutArmZHeightDiff1_2",  bShow, bEnable, bReadFromFile, 0,     false, -500, 500);
    elData->Add(setEditZ2G,                 &iOutArmZHeightDiff[0][3],    ECInteger,  "ArmSuckZAuto", "iOutArmZHeightDiff0_3",  bShow, bEnable, bReadFromFile, 0,     false, -500, 500);
    elData->Add(setEditZ2H,                 &iOutArmZHeightDiff[1][3],    ECInteger,  "ArmSuckZAuto", "iOutArmZHeightDiff1_3",  bShow, bEnable, bReadFromFile, 0,     false, -500, 500);

    elData->Add(edOutarmTestAreaX,          &OutArmAutoCalSuckZPoint.X,   ECInteger,  "ArmSuckZAuto", "OutArmAutoCalSuckZPointX",  bShow, bEnable, bReadFromFile, 0);
    elData->Add(edOutarmTestAreaY,          &OutArmAutoCalSuckZPoint.Y,   ECInteger,  "ArmSuckZAuto", "OutArmAutoCalSuckZPointY",  bShow, bEnable, bReadFromFile, 0);
    elData->Add(edOutarmSearchStartZ,       &iOutArmSearchStartZ,         ECInteger,  "ArmSuckZAuto", "iOutArmSearchStartZ",    bShow, bEnable, bReadFromFile, 0);
}
//---------------------------------------------------------------------------
void TfProductionInfo::SaveAutoCalSuckZ()
{
    AnsiString szDir=GetProInfoFilePath();
    MyForceDirectories(szDir);
    szDir+="\\";
    elData->SaveEditTextToFile(szDir, "AutoCalSuckZ.Data");
}
//---------------------------------------------------------------------------
void TfProductionInfo::LoadAutoCalSuckZ()
{
    AnsiString szDir=GetProInfoFilePath();
    MyForceDirectories(szDir);
    szDir+="\\";
    elData->ReadEditTextFromFile(szDir, "AutoCalSuckZ.Data");
}
//---------------------------------------------------------------------------
bool TfProductionInfo::EnableInArmAutoCalSuckZ()
{
    return bEnableInarmSuckZAuto;
}
//---------------------------------------------------------------------------
bool TfProductionInfo::EnableOutArmAutoCalSuckZ()
{
    return bEnableOutarmSuckZAuto;
}
//---------------------------------------------------------------------------
void TfProductionInfo::SetInsertOPIDStr(const AnsiString& s)
{
    edInsertOPID_HALT->Text=FilterAlphanumeric(s);
}
//---------------------------------------------------------------------------
AnsiString TfProductionInfo::GetInsertOPIDStr()
{
    return FilterAlphanumeric(edInsertOPID_HALT->Text);
}
//---------------------------------------------------------------------------
AnsiString TfProductionInfo::FilterAlphanumeric(const AnsiString& s)
{
    AnsiString result;
    for (int i = 1; i <= s.Length(); i++)
    {
        char c = s[i];
        if((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') )
        {
            result += c;
        }
    }
    return result;
}
//---------------------------------------------------------------------------
void TfProductionInfo::ShowAutoCalSuckZForm()
{
    LoadAutoCalSuckZ();
    Left=(1024-Width)/2;
    Top=(768-Height)/2;
    ShowSheet(tsCalibrateSuckZHeight);
}
//---------------------------------------------------------------------------
void TfProductionInfo::InitialStringGrid(TStringGrid *sg)
{
//    sg->Colcount();
//    int iCol=sg->Colcount;
}
//---------------------------------------------------------------------------
int cDynamicMultiContinualPassBinBySocket::GetMultiplierNum()
{
    int iBack=1;
    if(IniConfig.bN14_24_DynaMultiContinuPassSocket)
    {
        iBack=pow(2,iDynamicThresholdNum);
    }
    return iBack;
}
//---------------------------------------------------------------------------
void cDynamicMultiContinualPassBinBySocket::AddThresholdNum()
{
    if(iDynamicThresholdNum<IniConfig.iN14_24_DyMultiPassPower)//max value =5
        iDynamicThresholdNum++;
}
//---------------------------------------------------------------------------
void __fastcall TfProductionInfo::btnAutoCalSuckZSaveClick(TObject *Sender)
{
    SaveAutoCalSuckZ();
    LoadAutoCalSuckZ();
    Close();
}
//---------------------------------------------------------------------------
bool TfProductionInfo::CheckCloseInfo()
{
    bool CanClose=true;
    _sOEE_Status="";
    //
    if(PageControl1->ActivePage==tsHaltStatusForm)
    {
        if(GetInsertOPIDStr().Trim()=="")
        {
            CanClose=false;
            if(bShow)  //Sam 20200225 : OEE 功能防呆保護提示顯示在畫面，不要用 MessageBox。
            {
                pn_ErrorMsg_HALTStatus->Caption = "Please Insert OP ID!";
                pn_ErrorMsg_PauseStatus->Caption = "Please Insert OP ID!";
            }
            else
            {
                ShowMyMessage("Please Insert OP ID!");
            }
        }

        if(lbStatus_HALT->Caption.Trim()=="")
        {
            CanClose=false;
            if(bShow)  //Sam 20200225 : OEE 功能防呆保護提示顯示在畫面，不要用 MessageBox。
            {
                pn_ErrorMsg_HALTStatus->Caption = "Please Select Status!";
                pn_ErrorMsg_PauseStatus->Caption = "Please Select Status!";
            }
            else
            {
                ShowMyMessage("Please Select Status!");
            }
        }
        _sOEE_ActivityID=edInsertOPID_HALT->Text;
        _sOEE_Status=lbStatus_HALT->Caption;
    }
    else if(PageControl1->ActivePage==tsPauseStatusForm)
    {
        if(edInsertOPID_Pause->Text.Trim()=="")
        {
            CanClose=false;
            if(bShow)  //Sam 20200225 : OEE 功能防呆保護提示顯示在畫面，不要用 MessageBox。
            {
                pn_ErrorMsg_HALTStatus->Caption = "Please Insert OP ID!";
                pn_ErrorMsg_PauseStatus->Caption = "Please Insert OP ID!";
            }
            else
            {
                ShowMyMessage("Please Insert OP ID!");
            }
        }

        if(lbStatus_Pause->Caption.Trim()=="")
        {
            CanClose=false;
            if(bShow)  //Sam 20200225 : OEE 功能防呆保護提示顯示在畫面，不要用 MessageBox。
            {
                pn_ErrorMsg_HALTStatus->Caption = "Please Select Status!";
                pn_ErrorMsg_PauseStatus->Caption = "Please Select Status!";
            }
            else
            {
                ShowMyMessage("Please Select Status!");
            }
        }
        _sOEE_ActivityID=edInsertOPID_Pause->Text;
        _sOEE_Status=lbStatus_Pause->Caption;

        if(sPauseButtonCaption!=lbStatus_Pause->Caption)
        {
            bPauseTimeNeedSaveAndUpdateOEEFiles=true;
        }
    }
    _sOEE_ActivityID=FilterAlphanumeric(_sOEE_ActivityID);
    if(bHadCloseInfo==false)
    {
        if(_sOEE_Status=="")
        {
            _sOEE_Status="Lost OEE Status File.";
            CanClose=true;
        }
    }
    //
    return CanClose;
}
//---------------------------------------------------------------------------
std::vector<int> TfProductionInfo::GetBin0_8List()
{
    int iBin0_8[8]= {
                     e3Auto1,  e3Auto2,  e3Auto3,   e3Fix1,
                     e3Fix2,   e3Fix3,   e3Fix4,    e3Fix5,
                    };
    std::vector<int> list;
    for(int iarea=0; iarea<8; iarea++)
    {
        list.push_back(iBin0_8[iarea]);
    }
    return list;
}
//---------------------------------------------------------------------------
