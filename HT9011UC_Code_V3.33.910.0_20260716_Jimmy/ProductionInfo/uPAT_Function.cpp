//---------------------------------------------------------------------------
#include "MachineDefine.h"
#pragma hdrstop

#include "uPAT_Function.h"
#include "FileInfo.h"
#include "mymessbox.h"
#include "cprod.h"
#include "cmydef.h"
#include "atester.h"
#include "MyKitSuck.h"
#include "common.h"
#include "uCleaning.h"
#include "main.h"
#include "cSocket.h"
#include "uTimeTool.h"
#include "ProductionInfo.h"
//-------------- -------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
PAT_Function::PAT_Function()
{
    if(CUSTOMER_CODE==CC_PANTHER)
    {
        DoIniDatas();
    }
}
//---------------------------------------------------------------------------
PAT_Function::~PAT_Function()
{
}
//---------------------------------------------------------------------------
void PAT_Function::DoIniDataToForm()
{
    if(CUSTOMER_CODE==CC_PANTHER && IniConfig.bB11UsePATServerFile)
    {
        LoadServerINI(GetPATServerINI());
    }
}
//---------------------------------------------------------------------------
void PAT_Function::DoIniDatas()
{
    patSetup.Clear();
    dStartLotTime=0.0;
    dEndLotTime=0.0;
    sPATMode="";
    iPATModeInd=0;
}
//---------------------------------------------------------------------------
bool PAT_Function::AnalyzePATSetUpText(const AnsiString& text)
{
    int iLen=text.Length();
    if (iLen>=5051)
    {
        patSetup.customerCode = text.SubString(1, 6).Trim();
        patSetup.deviceNo = text.SubString(7, 40).Trim();
        patSetup.cLotNo = text.SubString(47, 25).Trim();
        patSetup.lotNo = text.SubString(72, 24).Trim();
        patSetup.accessoryNo1 = text.SubString(96, 25).Trim();
        patSetup.testQty = text.SubString(121, 10).Trim();
        patSetup.waferId = text.SubString(131, 52).Trim();
        patSetup.temp = text.SubString(183, 12).Trim();
        patSetup.employeeNo = text.SubString(195, 15).Trim();
        patSetup.testerNo = text.SubString(210, 15).Trim();
        patSetup.entityNo = text.SubString(225, 10).Trim();
        patSetup.step = text.SubString(235, 30).Trim();
        patSetup.programName = text.SubString(265, 100).Trim();
        patSetup.pgmPath = text.SubString(365, 120).Trim();
        patSetup.recipe = text.SubString(485, 50).Trim();
        patSetup.programVer = text.SubString(535, 10).Trim();
        patSetup.prtProgram = text.SubString(545, 100).Trim();
        patSetup.rtProgram = text.SubString(645, 100).Trim();
        patSetup.gdProgram = text.SubString(745, 100).Trim();
        patSetup.eqcProgram = text.SubString(845, 100).Trim();
        patSetup.cshProgram = text.SubString(945, 100).Trim();
        patSetup.batchFileName = text.SubString(1045, 50).Trim();
        patSetup.codeName = text.SubString(1095, 30).Trim();
        patSetup.codeVersion = text.SubString(1125, 30).Trim();
        patSetup.reloadProgram = text.SubString(1155, 3).Trim();
        patSetup.writeReadCodeDevice = text.SubString(1158, 3).Trim();
        patSetup.eqcCshProgram = text.SubString(1161, 100).Trim();
        patSetup.cshBatchFileExecution = text.SubString(1261, 3).Trim();
        patSetup.scriptFileName = text.SubString(1264, 100).Trim();
        patSetup.scriptFilePath = text.SubString(1364, 120).Trim();
        patSetup.notchDirection = text.SubString(1484, 6).Trim();
        patSetup.pretestFileName = text.SubString(1490, 100).Trim();
        patSetup.retestFileName = text.SubString(1590, 100).Trim();
        patSetup.goldenFileName = text.SubString(1690, 100).Trim();
        patSetup.eqcFileName = text.SubString(1790, 100).Trim();
        patSetup.dutBoardNo = text.SubString(1890, 25).Trim();
        patSetup.cfgServerPath = text.SubString(1990, 120).Trim();
        patSetup.cfgLocalPath = text.SubString(2110, 120).Trim();
        patSetup.cfgProgram = text.SubString(2230, 100).Trim();
        patSetup.siteNumber = text.SubString(2330, 10).Trim();
        patSetup.waferType = text.SubString(2340, 10).Trim();
        patSetup.cpAutoReTest = text.SubString(2350, 3).Trim();
        patSetup.notRtBin = text.SubString(2353, 220).Trim();
        patSetup.failSiteNumber = text.SubString(2573, 100).Trim();
        patSetup.proberCardCurrentTouchDown = text.SubString(2673, 10).Trim();
        patSetup.proberCardLimitTouchDown = text.SubString(2683, 10).Trim();
        patSetup.curiVersion = text.SubString(2693, 30).Trim();
        patSetup.osVersion = text.SubString(2723, 30).Trim();
        patSetup.waferVersion = text.SubString(2753, 20).Trim();
        patSetup.igExcelVersion = text.SubString(2773, 200).Trim();
        patSetup.downloadPath = text.SubString(2973, 200).Trim();
        patSetup.accessoryNo2 = text.SubString(3173, 25).Trim();
        patSetup.accessoryNo3 = text.SubString(3198, 25).Trim();
        patSetup.accessoryNo4 = text.SubString(3223, 25).Trim();
        patSetup.accessoryNo5 = text.SubString(3248, 25).Trim();
        patSetup.s100OIVersion = text.SubString(3273, 20).Trim();
        patSetup.step2 = text.SubString(3293, 20).Trim();
        patSetup.codeName2 = text.SubString(3313, 20).Trim();
        patSetup.registerBatExecution = text.SubString(3333, 3).Trim();
        patSetup.modelFile = text.SubString(3336, 50).Trim();
        patSetup.type = text.SubString(3386, 30).Trim();
        patSetup.dateCode = text.SubString(3416, 30).Trim();
        patSetup.socketNumber = text.SubString(3446, 120).Trim();
        patSetup.auxDataFile = text.SubString(3566, 60).Trim();
        patSetup.driverPath = text.SubString(3626, 80).Trim();
        patSetup.driverSize1 = text.SubString(3706, 20).Trim();
        patSetup.driverSize2 = text.SubString(3726, 20).Trim();
        patSetup.driverNameDisplayPath = text.SubString(3746, 60).Trim();
        patSetup.cLotNoForSLT = text.SubString(3806, 100).Trim();
        patSetup.passBin = text.SubString(3906, 30).Trim();
        patSetup.checkSun = text.SubString(3936, 20).Trim();
        patSetup.releaseCode = text.SubString(3956, 20).Trim();
        patSetup.pgmVersion = text.SubString(3976, 20).Trim();
        patSetup.autoZOSProgram = text.SubString(3996, 100).Trim();
        patSetup.accessoryNo6 = text.SubString(4096, 300).Trim();
        patSetup.pgmPath2 = text.SubString(4396, 300).Trim();
        patSetup.downloadPath2 = text.SubString(4696, 300).Trim();
        patSetup.autoFixtureCal = text.SubString(4996, 3).Trim();
        patSetup.fdEngineMonitor = text.SubString(4999, 3).Trim();
        patSetup.ecnNo = text.SubString(5002, 30).Trim();
        patSetup.nvtProcess = text.SubString(5032, 10).Trim();
        patSetup.nvtFlowStep = text.SubString(5042, 10).Trim();
        return true;
    }
    else
    {
        return false;
    }
}
//---------------------------------------------------------------------------
bool PAT_Function::LoadSetupFile(const AnsiString FilePath)
{
    DoIniDatas();
    return AnalyzePATSetUpText(FileInfo(FilePath).DecodeASCII(FilePath,0));
}
//---------------------------------------------------------------------------
bool PAT_Function::SaveServerINI(const AnsiString FilePath)
{
    if(FileInfo().IsFilePathExist(FilePath))
    {
        TIniFile* iniFile=new TIniFile(FilePath);
        SetPathConfig(iniFile,pathConf);
        SetTimeConfig(iniFile,timeConf);
        SetTypeConfig(iniFile,typeConf);
        SetPasswordConfig(iniFile,sPATFtpPwd);
        delete iniFile;
        return true;
    }
    else
    {
        return false;
    }
}
//---------------------------------------------------------------------------
bool PAT_Function::LoadServerINI(const AnsiString FilePath)
{
    if(FileInfo().IsFilePathExist(FilePath))
    {
        TIniFile* iniFile=new TIniFile(FilePath);
        GetPathConfig(iniFile,pathConf);
        GetTimeConfig(iniFile,timeConf);
        GetTypeConfig(iniFile,typeConf);
        GetPasswordConfig(iniFile,sPATFtpPwd);
        delete iniFile;
        return true;
    }
    else
    {
        ShowMyMessage(AnsiString().sprintf("PATServer.ini file is missing. Path:%s",FilePath));
        return false;
    }
}
//---------------------------------------------------------------------------
void PAT_Function::SetPathConfig(TIniFile* iniFile,const PathConfig &pathConfig)
{
    if(iniFile==NULL)
        return;
    WriteINIString(iniFile,"Path", "IntervalTime", IntToStr(pathConfig.iIntervalTime));
    WriteINIString(iniFile,"Path", "RealTimePath", pathConfig.sRealTimePath);
    WriteINIString(iniFile,"Path", "LoadProduction", pathConfig.sLoadProduction);
    WriteINIString(iniFile,"Path", "BackupToServer", pathConfig.sBackupToServer);
    WriteINIString(iniFile,"Path", "SockLifeTime", pathConfig.sSockLifeTime);
    WriteINIString(iniFile,"Path", "CleanSocketPath", pathConfig.sCleanSocketPath);
    WriteINIString(iniFile,"Path", "SpecialOriginPath", pathConfig.sSpecialOriginPath);
    WriteINIString(iniFile,"Path", "SpecialSaveToPath", pathConfig.sSpecialSaveToPath);
    WriteINIString(iniFile,"Path", "JobFileUpDownloadPath", pathConfig.sJobFileUpDownloadPath);
    WriteINIString(iniFile,"Path", "RealTimeSubTestPath", pathConfig.sRealTimeSubTestPath);
    WriteINIString(iniFile,"Path", "ServerJobFilePath", pathConfig.sServerJobFilePath);
    WriteINIString(iniFile,"Path", "SendJamLogFilePath", pathConfig.sSendJamLogFilePath);
    WriteINIString(iniFile,"Path", "sRunCorrelationIniPath", pathConfig.sRunCorrelationIniPath);
    WriteINIString(iniFile,"Path", "sConfirmSiteMapIniPath", pathConfig.sConfirmSiteMapIniPath);
    WriteINIString(iniFile,"Path", "EngJobFileUploadPath", pathConfig.sEngJobFileUploadPath);
    WriteINIString(iniFile,"Path", "sRunInfoTesterPath", pathConfig.sRunInfoTesterPath);
    WriteINIString(iniFile,"Path", "sOneCycleReportPath", pathConfig.sOneCycleReportPath);
    WriteINIString(iniFile,"Path", "sRunInfoBackupPath", pathConfig.sRunInfoBackupPath);
}
//---------------------------------------------------------------------------
bool PAT_Function::GetPathConfig(TIniFile* iniFile,PathConfig &pathConfig)
{
    if(iniFile==NULL)
        return false;
    pathConfig.iIntervalTime = atoi(ReadINIString(iniFile,"Path", "IntervalTime").c_str());
    pathConfig.sRealTimePath = ReadINIString(iniFile,"Path", "RealTimePath");
    pathConfig.sLoadProduction = ReadINIString(iniFile,"Path", "LoadProduction");
    pathConfig.sBackupToServer = ReadINIString(iniFile,"Path", "BackupToServer");
    pathConfig.sSockLifeTime = ReadINIString(iniFile,"Path", "SockLifeTime");
    pathConfig.sCleanSocketPath = ReadINIString(iniFile,"Path", "CleanSocketPath");
    pathConfig.sSpecialOriginPath = ReadINIString(iniFile,"Path", "SpecialOriginPath");
    pathConfig.sSpecialSaveToPath = ReadINIString(iniFile,"Path", "SpecialSaveToPath");
    pathConfig.sJobFileUpDownloadPath = ReadINIString(iniFile,"Path", "JobFileUpDownloadPath");
    pathConfig.sRealTimeSubTestPath = ReadINIString(iniFile,"Path", "RealTimeSubTestPath");
    pathConfig.sServerJobFilePath = ReadINIString(iniFile,"Path", "ServerJobFilePath");
    pathConfig.sSendJamLogFilePath = ReadINIString(iniFile,"Path", "SendJamLogFilePath");
    pathConfig.sRunCorrelationIniPath = ReadINIString(iniFile,"Path", "sRunCorrelationIniPath");
    pathConfig.sConfirmSiteMapIniPath = ReadINIString(iniFile,"Path", "sConfirmSiteMapIniPath");
    pathConfig.sEngJobFileUploadPath = ReadINIString(iniFile,"Path", "EngJobFileUploadPath");
    pathConfig.sRunInfoTesterPath = ReadINIString(iniFile,"Path", "sRunInfoTesterPath");
    pathConfig.sOneCycleReportPath = ReadINIString(iniFile,"Path", "sOneCycleReportPath");
    pathConfig.sRunInfoBackupPath = ReadINIString(iniFile,"Path", "sRunInfoBackupPath");
    return true;
}
//---------------------------------------------------------------------------
void PAT_Function::SetTimeConfig(TIniFile* iniFile,const TimeConfig &timeConfig)
{
    if(iniFile==NULL)
        return;
    WriteINIString(iniFile,"Time", "A", timeConfig.A);
    WriteINIString(iniFile,"Time", "B", timeConfig.B);
    WriteINIString(iniFile,"Time", "C", timeConfig.C);
    WriteINIString(iniFile,"Time", "D", timeConfig.D);
    WriteINIString(iniFile,"Time", "E", timeConfig.E);
    WriteINIString(iniFile,"Time", "F", timeConfig.F);
    WriteINIString(iniFile,"Time", "G", timeConfig.G);
    WriteINIString(iniFile,"Time", "H", timeConfig.H);
}
//---------------------------------------------------------------------------
bool PAT_Function::GetTimeConfig(TIniFile* iniFile,TimeConfig &timeConfig)
{
    if(iniFile==NULL)
        return false;
    timeConfig.A=ReadINIString(iniFile,"Time", "A");
    timeConfig.B=ReadINIString(iniFile,"Time", "B");
    timeConfig.C=ReadINIString(iniFile,"Time", "C");
    timeConfig.D=ReadINIString(iniFile,"Time", "D");
    timeConfig.E=ReadINIString(iniFile,"Time", "E");
    timeConfig.F=ReadINIString(iniFile,"Time", "F");
    timeConfig.G=ReadINIString(iniFile,"Time", "G");
    timeConfig.H=ReadINIString(iniFile,"Time", "H");
    return true;
}
//---------------------------------------------------------------------------
void PAT_Function::SetTypeConfig(TIniFile* iniFile,const TypeConfig &typeConfig)
{
    if(iniFile==NULL)
        return;
    WriteINIString(iniFile,"Type", "JamType1", typeConfig.JamType1);
    WriteINIString(iniFile,"Type", "JamType2", typeConfig.JamType2);
    WriteINIString(iniFile,"Type", "JamType3", typeConfig.JamType3);
    WriteINIString(iniFile,"Type", "JamType4", typeConfig.JamType4);
    WriteINIString(iniFile,"Type", "JamType5", typeConfig.JamType5);
}
//---------------------------------------------------------------------------
bool PAT_Function::GetTypeConfig(TIniFile* iniFile,TypeConfig &typeConfig)
{
    if(iniFile==NULL)
        return false;
    typeConfig.JamType1=ReadINIString(iniFile,"Type", "JamType1");
    typeConfig.JamType2=ReadINIString(iniFile,"Type", "JamType2");
    typeConfig.JamType3=ReadINIString(iniFile,"Type", "JamType3");
    typeConfig.JamType4=ReadINIString(iniFile,"Type", "JamType4");
    typeConfig.JamType5=ReadINIString(iniFile,"Type", "JamType5");
    return true;
}
//---------------------------------------------------------------------------
void PAT_Function::SetPasswordConfig(TIniFile* iniFile,const AnsiString &ftppwd)
{
    if(iniFile==NULL)
        return;
    WriteINIString(iniFile,"Password", "PATFtpPwd", ftppwd);
}
//---------------------------------------------------------------------------
bool PAT_Function::GetPasswordConfig(TIniFile* iniFile,AnsiString &ftppwd)
{
    if(iniFile==NULL)
        return false;
    ftppwd=ReadINIString(iniFile,"Password", "PATFtpPwd");
    return true;
}
//---------------------------------------------------------------------------
AnsiString PAT_Function::ReadINIString(TIniFile* iniFile,const AnsiString& section, const AnsiString& key, const AnsiString& defaultValue)
{
    return iniFile->ReadString(section, key, defaultValue);
}
//---------------------------------------------------------------------------
void PAT_Function::WriteINIString(TIniFile* iniFile,const AnsiString& section, const AnsiString& key, const AnsiString& value)
{
    iniFile->WriteString(section, key, value);
}
//---------------------------------------------------------------------------
AnsiString PAT_Function::GetRealTimeReport(const int &iReportType)
{
    ClearRptStruct();
    AnsiString srpt="";
    srpt+=GetBasicData(iReportType);
    //
    srpt+=FileInfo().GetNewLine();
    srpt+=GetRealTimeRpt_FormatRow("-----------------Output Count-----------------------", "");
    srpt+=GetOutputCount();

    srpt+=FileInfo().GetNewLine();
    srpt+=GetRealTimeRpt_FormatRow("-----------------Output Tray Bin Summer-----------------------", "");
    srpt+=GetOutputTrayBinSummer();

    srpt+=FileInfo().GetNewLine();
    srpt+=GetRealTimeRpt_FormatRow("-----------------Output Site Bin Summer-----------------------", "");
    srpt+=GetOutputSiteBinSummer();

    srpt+=FileInfo().GetNewLine();
    srpt+=GetRealTimeRpt_FormatRow("-----------------Output Site Bin Yield Summer-----------------------", "");
    srpt+=GetOutputSiteBinYieldSummer();

    srpt+=FileInfo().GetNewLine();
    srpt+=GetRealTimeRpt_FormatRow("--------------------------------Alarm log----------------------------", "");
    srpt+=GetJamLogList();
    return srpt;
}
//---------------------------------------------------------------------------
AnsiString PAT_Function::GetRealTimeRpt_FormatRow(const AnsiString& sLabel, const AnsiString& sValue)
{
    if(sValue=="")
    {
        return AnsiString().sprintf("%s\n", sLabel.c_str());
    }
    else
        return AnsiString().sprintf("%-26s: %s\n", sLabel.c_str(), sValue.Trim().c_str());
}
//---------------------------------------------------------------------------
AnsiString PAT_Function::GetRealTimeRpt_OutCnt(const AnsiString& sLabel, const AnsiString& sNum, const AnsiString& sPercent)
{
    int labelWidth=5;
    int numWidth=6;
    int percentWidth=6;
    return AnsiString().sprintf(
            "%-*s: %-*s: %-*s %s",
            labelWidth,   sLabel.c_str(),
            numWidth,     sNum,
            percentWidth, sPercent,
            FileInfo().GetNewLine()
        );
}
//---------------------------------------------------------------------------
AnsiString PAT_Function::GetDoubleToStr(double dValue)
{
    return AnsiString().sprintf("%02.2f",dValue);
}
//---------------------------------------------------------------------------
int PAT_Function::GetTotalContactCount()
{
    int iret=(LastSet.iContactCT[0]+LastSet.iContactCT[1]);
    if(iret==0)
        iret=1;
    return iret;
}
//---------------------------------------------------------------------------
void PAT_Function::GetCTbyChannel(int iCh,int iRetDatas[16])
{
    int iRowCounts=TestSocket.iShtRow;
    int iColCounts=TestSocket.iShtCol;
    int iret=0,itotal=0;
    for(int iRow=0; iRow<iRowCounts; iRow++)
    {
        for(int iCol=0; iCol<iColCounts; iCol++)
        {
            if(TestIF.iSiteMap[iRow][iCol]>0 && TestIF.iSiteMap[iRow][iCol]==(iCh+1))
            {
                itotal=0;
                for(int iCat=1; iCat<16; iCat++)
                {
                    iret=0;
                    iret+=TastCategory.iCountCategory[0][iRow][iCol][iCat];
                    iret+=TastCategory.iCountCategory[1][iRow][iCol][iCat];
                    iRetDatas[iCat-1]=iret;
                    itotal+=iret;
                }
                iRetDatas[15]=itotal;
                return;
            }
        }
    }
}
//---------------------------------------------------------------------------
void PAT_Function::ClearRptStruct()
{
    toutputCnt.Clear();
    iTotalSites=0;
    for (int i = 0; i < MAX_SITES; i++)
    {
        Tray[i].Clear();
    }
}
//---------------------------------------------------------------------------
AnsiString PAT_Function::GetAverageTestTime()
{
    double dret=0.0;
    dret=ChangeToFloatNonPcnt((double)(RunInfo.dTestTimeSec), (double)(GetTotalContactCount()));
    return GetDayHourMinSecStr(SecondsToDateTime(dret));
}
//---------------------------------------------------------------------------
AnsiString PAT_Function::GetAveragePauseTime()
{
    double dret=0.0;
    dret=ChangeToFloatNonPcnt((double)(LastSet.SystemAccSecond[0][stPauseTime]), (double)(GetTotalContactCount()));
    return GetDayHourMinSecStr(SecondsToDateTime(dret));
}
//---------------------------------------------------------------------------
AnsiString PAT_Function::GetOffSiteNumber()
{
    int iret=0;
    bool bIncludeCloseSite=true;
    iret=GetSiteCount(bIncludeCloseSite)-GetSiteCount(!bIncludeCloseSite);
    return IntToStr(iret);
}
//---------------------------------------------------------------------------
AnsiString PAT_Function::GetJamRate()
{
    AnsiString str="";
    int sendCT  =LastSet.SendCT[1];
    int jamCount=LastSet.iJamCount[1];
    if(sendCT==0 || jamCount==0)
    {
        str.sprintf("0 / 10000");
    }
    else
    {
        int jamRate=(jamCount*10000)/sendCT;
        str.sprintf("%d / 10000", jamRate);
    }
    return str;
}
//---------------------------------------------------------------------------
AnsiString PAT_Function::GetIndexCycleTime()
{
    AnsiString str="";
    if(RunInfo.IndexTime=="")
    {
        str.sprintf("0");
    }
    else
    {
        str.sprintf("%s", RunInfo.IndexTime);
    }
    return str;
}
//---------------------------------------------------------------------------
AnsiString PAT_Function::GetOutputCount()
{
    toutputCnt.iAuto1=LastSet.BinCT[0][e3Auto1];
    toutputCnt.iAuto2=LastSet.BinCT[0][e3Auto2];
    toutputCnt.iAuto3=LastSet.BinCT[0][e3Auto3];
    toutputCnt.iFix1= LastSet.BinCT[0][e3Fix1];
    toutputCnt.iFix2= LastSet.BinCT[0][e3Fix2];
    toutputCnt.iFix3= LastSet.BinCT[0][e3Fix3];
    toutputCnt.CalculateTotal();
    AnsiString srpt="";
    srpt+=GetRealTimeRpt_OutCnt("Auto1", IntToStr(toutputCnt.iAuto1),GetDoublePercentStr((double)toutputCnt.iAuto1,(double)toutputCnt.iTotal));
    srpt+=GetRealTimeRpt_OutCnt("Auto2", IntToStr(toutputCnt.iAuto2),GetDoublePercentStr((double)toutputCnt.iAuto2,(double)toutputCnt.iTotal));
    srpt+=GetRealTimeRpt_OutCnt("Auto3", IntToStr(toutputCnt.iAuto3),GetDoublePercentStr((double)toutputCnt.iAuto3,(double)toutputCnt.iTotal));
    srpt+=GetRealTimeRpt_OutCnt("Fix1",  IntToStr(toutputCnt.iFix1),GetDoublePercentStr((double)toutputCnt.iFix1,(double)toutputCnt.iTotal));
    srpt+=GetRealTimeRpt_OutCnt("Fix2",  IntToStr(toutputCnt.iFix2),GetDoublePercentStr((double)toutputCnt.iFix2,(double)toutputCnt.iTotal));
    srpt+=GetRealTimeRpt_OutCnt("Fix3",  IntToStr(toutputCnt.iFix3),GetDoublePercentStr((double)toutputCnt.iFix3,(double)toutputCnt.iTotal));
    srpt+=GetRealTimeRpt_OutCnt("Total", IntToStr(toutputCnt.iAuto1)," ");
    return srpt;
}
//---------------------------------------------------------------------------
AnsiString PAT_Function::GetOutputTrayBinSummer()
{
    int labelWidth=6;
    int numBins=15;
    int colWidth=10;
    int numRows=6;
    AnsiString header="Tray :";
    for(int i=0;i<numBins;i++)
    {
        header+=AnsiString().sprintf("%-*s",colWidth,("Bin"+IntToStr(i+1)).c_str());
    }
    header+=FileInfo().GetNewLine();
    AnsiString labels[6]={"Auto1:", "Auto2:", "Auto3:", "Fix1 :", "Fix2 :", "Fix3 :"};
    int data[6][15]={0};
    int iBin=0;
    for(int i=0;i<numRows;i++) //6
    {
        iBin=GetBinByArea(i);
        data[i][iBin-1]=LastSet.BinCT[0][i];
    }
    AnsiString stable=header;
    for(int i=0;i<numRows;i++)
    {
        stable+=AnsiString().sprintf("%-*s",labelWidth,labels[i].c_str());
        for(int j=0;j<numBins;j++)
        {
            stable+=AnsiString().sprintf("%-*d", colWidth, data[i][j]);
        }
        stable+=FileInfo().GetNewLine();
    }
    return stable;
}
//---------------------------------------------------------------------------
AnsiString PAT_Function::GetOutputSiteBinSummer()
{
    int labelWidth=7;
    int colWidth=10;
    AnsiString header="Tray : ";
    for(int i=0;i<MAX_BINS;i++)
    {
        header+=AnsiString().sprintf("%-*s",colWidth,("Bin"+IntToStr(i+1)).c_str());
    }
    header+=AnsiString().sprintf("%-*s",colWidth,AnsiString("Total").c_str());
    header+=FileInfo().GetNewLine();
    int data[16]={0};
    int iRowCounts=TestSocket.iShtRow;
    int iColCounts=TestSocket.iShtCol;
    AnsiString stable=header;
    iTotalSites=0;
    for(int iRow=0; iRow<iRowCounts; iRow++)
    {
        for(int iCol=0; iCol<iColCounts; iCol++)
        {
            if(TestIF.iSiteMap[iRow][iCol]>0)
            {
                if(iTotalSites<MAX_SITES)
                {
                    ZeroMemory(data , sizeof(data));
                    stable+=AnsiString().sprintf("%-*s",labelWidth,("Site"+IntToStr(iTotalSites+1)+":").c_str());//head
                    GetCTbyChannel(iTotalSites,data);
                    for(int idata=0;idata<=MAX_BINS;idata++)
                    {
                        Tray[iTotalSites].Bins[idata]=data[idata];
                        stable+=AnsiString().sprintf("%-*d", colWidth, data[idata]);//datas
                    }
                    Tray[iTotalSites].CalculateTotal();
                    stable+=FileInfo().GetNewLine();
                }
                iTotalSites++;
            }
        }
    }
    return stable;
}
//---------------------------------------------------------------------------
AnsiString PAT_Function::GetOutputSiteBinYieldSummer()
{
    int labelWidth=9;
    int numBins=15;
    int colWidth=10;
    AnsiString header="Tray :   ";
    for(int i=0;i<numBins;i++)
    {
        header+=AnsiString().sprintf("%-*s",colWidth,("Bin"+IntToStr(i+1)).c_str());
    }
    header+=AnsiString().sprintf("%-*s",colWidth,AnsiString("Total").c_str());
    header+=FileInfo().GetNewLine();
    AnsiString stable=header, sRowData="";
    for(int iRow=0; iRow<iTotalSites; iRow++)
    {
        stable+=AnsiString().sprintf("%-*s",labelWidth,("Site"+IntToStr(iRow+1)+":").c_str());//head
        for(int iCol=0; iCol<=MAX_BINS; iCol++)
        {
            sRowData=GetDoublePercentStr((double)Tray[iRow].Bins[iCol],(double)toutputCnt.iTotal);
            stable+=AnsiString().sprintf("%-*s", colWidth, sRowData);
        }
        sRowData=GetDoublePercentStr((double)Tray[iRow].Total,(double)toutputCnt.iTotal);
        stable+=AnsiString().sprintf("%-*s", colWidth, sRowData);//datas
        stable+=FileInfo().GetNewLine();
    }
    return stable;
}
//---------------------------------------------------------------------------
AnsiString PAT_Function::GetBasicData(const int &iReportType)
{
    AnsiString srpt="";
    srpt+=GetRealTimeRpt_FormatRow("Runcard Number",patSetup.lotNo);
    srpt+=GetRealTimeRpt_FormatRow("Cus.Lot Number",patSetup.cLotNo);
    srpt+=GetRealTimeRpt_FormatRow("Part Number",patSetup.deviceNo);
    srpt+=GetRealTimeRpt_FormatRow("Program",GetProgramName(iPATModeInd));
    srpt+=GetRealTimeRpt_FormatRow("Job Name",patSetup.recipe);
    srpt+=GetRealTimeRpt_FormatRow("Op. Name",patSetup.employeeNo);
    srpt+=GetRealTimeRpt_FormatRow("Entity No",patSetup.entityNo);
    srpt+=GetRealTimeRpt_FormatRow("LoadBoard",patSetup.accessoryNo1);
    srpt+=GetRealTimeRpt_FormatRow("Start Time",GetStartLotTimeStr());
    srpt+=GetRealTimeRpt_FormatRow("End Time",GetEndLotTimeStr(iReportType));
    srpt+=GetRealTimeRpt_FormatRow("Test Temperature",patSetup.temp);
    srpt+=GetRealTimeRpt_FormatRow("Soak Time",GetDoubleToStr(Temperature.fSoakTime));
    srpt+=GetRealTimeRpt_FormatRow("Act.Temperature(Plate1)",RunInfo.ShowTempComp[tcHotPlate1]);
    srpt+=GetRealTimeRpt_FormatRow("Act.Temperature(Plate2)",RunInfo.ShowTempComp[tcHotPlate2]);
    srpt+=GetRealTimeRpt_FormatRow("Test Yield", GetTotalYield_Str());
    srpt+=GetRealTimeRpt_FormatRow("Test Time", GetTestTimeStr(iReportType));
    srpt+=GetRealTimeRpt_FormatRow("Pure Test Time", GetDayHourMinSecStr(SecondsToDateTime(LastSet.SystemAccSecond[0][stProductTime]*0.001)));
    srpt+=GetRealTimeRpt_FormatRow("Average Test Time", GetAverageTestTime());
    srpt+=GetRealTimeRpt_FormatRow("Average Pure Test Time", GetAveragePauseTime());
    srpt+=GetRealTimeRpt_FormatRow("Index Time", RunInfo.IndexTime);
    srpt+=GetRealTimeRpt_FormatRow("Idle Time", GetDayHourMinSecStr(SecondsToDateTime(LastSet.SystemAccSecond[0][stPauseTime]*0.001)));
    srpt+=GetRealTimeRpt_FormatRow("Stop Time(Before Test)", GetDayHourMinSecStr(fMain->machineTime.GetTotalPauseTimeBeforeTest()));
    srpt+=GetRealTimeRpt_FormatRow("Stop Time(After Test)", GetDayHourMinSecStr(fMain->machineTime.GetTotalPauseTime()));
    srpt+=GetRealTimeRpt_FormatRow("Total Stop Time", GetDayHourMinSecStr(fMain->machineTime.GetTotalPauseTimeBeforeTest()+fMain->machineTime.GetTotalPauseTime()));
    srpt+=GetRealTimeRpt_FormatRow("Test Frequencey", IntToStr(GetTotalContactCount()));
    srpt+=GetRealTimeRpt_FormatRow("Site Number", IntToStr(GetSiteCount(true)));
    srpt+=GetRealTimeRpt_FormatRow("Off Site Number", GetOffSiteNumber());
    srpt+=GetRealTimeRpt_FormatRow("Off site status", GetClosedSitesName());
    srpt+=GetRealTimeRpt_FormatRow("Oper", patSetup.step);
    srpt+=GetRealTimeRpt_FormatRow("Step", sPATMode);
    srpt+=GetRealTimeRpt_FormatRow("Index Cycle Time", GetIndexCycleTime());
    srpt+=GetRealTimeRpt_FormatRow("UPH", IntToStr(RunInfo.iUPH));
    srpt+=GetRealTimeRpt_FormatRow("Jam Qty", IntToStr(LastSet.iJamCount[1]));
    srpt+=GetRealTimeRpt_FormatRow("Jam rate", GetJamRate());
    return srpt;
}
//---------------------------------------------------------------------------
AnsiString PAT_Function::GetJamLogList()
{
    AnsiString srpt="";
    unsigned int iSize=JamLogs.size();
    for(unsigned int i=0;i<iSize;i++)
    {
        srpt+=AnsiString().sprintf("%s\n", JamLogs[i].c_str());
    }
    return srpt;
}
//---------------------------------------------------------------------------
AnsiString PAT_Function::GetTimeFomate(const TDateTime &datetime)
{
    return datetime.FormatString("yyyy/mm/dd hh:nn:ss");
}
//---------------------------------------------------------------------------
TDateTime PAT_Function::GetEndLotTime(const int &iReportType)
{
    double dretEndLotTime=0.0;
    if(iReportType==(int)REPORT_END_LOT)
    {
        dretEndLotTime=dEndLotTime;
    }
    else
    {
        TDateTime datetime=Now();
        TDateTime updatedDateTime;
        unsigned short year=0, month=0, day=0, hour=0, min=0, sec=0, msec=0;
        datetime.DecodeDate(&year, &month, &day);
        datetime.DecodeTime(&hour, &min, &sec, &msec);
        TDateTime tdDate(year,month,day);
        if(iReportType==(int)REPORT_HOURLY)
        {
            TDateTime tdTime(hour,0,0,0);
            updatedDateTime=tdDate+tdTime;
        }
        else//REPORT_REALTIME
        {
            TDateTime tdTime(hour,min,0,0);
            updatedDateTime=tdDate+tdTime;
        }
        dretEndLotTime=updatedDateTime;
    }
    return dretEndLotTime;
}
//---------------------------------------------------------------------------
AnsiString PAT_Function::GetStartLotTimeStr()
{
    AnsiString sRetTime="";
    if(dStartLotTime>0.0)
    {
        sRetTime=GetTimeFomate(dStartLotTime);
    }
    return sRetTime;
}
//---------------------------------------------------------------------------
AnsiString PAT_Function::GetEndLotTimeStr(const int &iReportType)
{
    AnsiString sEndLotTime="";
    double dGetEndLotTime=GetEndLotTime(iReportType);
    if(dGetEndLotTime>0.0)
    {
        sEndLotTime=GetTimeFomate(dGetEndLotTime);
    }
    return sEndLotTime;
}
//---------------------------------------------------------------------------
AnsiString PAT_Function::GetTestTimeStr(const int &iReportType)
{
    AnsiString str="";
    double dGetEndLotTime=GetEndLotTime(iReportType);
    double dGetStartLotTime=dStartLotTime;
    double difftimeSec=uTimeTool().DiffTimeSec(dGetEndLotTime,dGetStartLotTime);
    str=GetDayHourMinSecStr(SecondsToDateTime(difftimeSec));
    return str;
}
//---------------------------------------------------------------------------
void PAT_Function::GenerateRealTimeReport()
{
    if(IniConfig.bB12UsePATSetup==true)
    {
        GenerateRealTimeReport(IniConfig.sB14RealTimePath);
    }
}
//---------------------------------------------------------------------------
void PAT_Function::GenerateRealTimeReport(const AnsiString &sPath)
{
    FileInfo().EnsureDirectoriesExist(sPath);
    AnsiString sFileName=AnsiString().sprintf("%s.asc",PC_NAME);
    AnsiString sFileNameWithPath=FileInfo().PathCombin(sPath,sFileName);
    FileInfo().SaveAsTxtFile(GetRealTimeReport(REPORT_REALTIME),sFileNameWithPath);
}
//---------------------------------------------------------------------------
void PAT_Function::GenerateHourlyReport()
{
    if(IniConfig.bB12UsePATSetup==true)
    {
        GenerateHourlyReport(IniConfig.sB14RealTimePath);
    }
}
//---------------------------------------------------------------------------
void PAT_Function::GenerateHourlyReport(const AnsiString &sPath)
{
    FileInfo().EnsureDirectoriesExist(sPath);
    AnsiString sRuncardNumber=patSetup.lotNo;
    AnsiString sStartLotTime=uTimeTool().TDatetmeToSavestring(dtStartLot);
    AnsiString sEndLotTime=uTimeTool().TDatetmeToSavestring(GetEndLotTime(REPORT_HOURLY));
    AnsiString sFileName=AnsiString().sprintf("%s_%s_%s_%s.txt",sRuncardNumber,PC_NAME,sStartLotTime,sEndLotTime);
    AnsiString sFileNameWithPath=FileInfo().PathCombin(sPath,sFileName);
    FileInfo().SaveAsTxtFile(GetRealTimeReport(REPORT_HOURLY),sFileNameWithPath);
}
//---------------------------------------------------------------------------
void PAT_Function::GenerateEndLotReport()
{
    if(IniConfig.bB12UsePATSetup==true)
    {
        GenerateEndLotReport(IniConfig.sB14RealTimePath);
    }
}
//---------------------------------------------------------------------------
void PAT_Function::GenerateEndLotReport(const AnsiString &sPath)
{
    FileInfo().EnsureDirectoriesExist(sPath);
    AnsiString sRuncardNumber=patSetup.lotNo;
    AnsiString sStartLotTime=uTimeTool().TDatetmeToSavestring(dtStartLot);
    AnsiString sEndLotTime=uTimeTool().TDatetmeToSavestring(GetEndLotTime(REPORT_END_LOT));
    AnsiString sFileName=AnsiString().sprintf("%s_%s_%s_%s.asc",sRuncardNumber,PC_NAME,sStartLotTime,sEndLotTime);
    AnsiString sFileNameWithPath=FileInfo().PathCombin(sPath,sFileName);
    FileInfo().SaveAsTxtFile(GetRealTimeReport(REPORT_END_LOT),sFileNameWithPath);
}
//---------------------------------------------------------------------------
AnsiString PAT_Function::GetPATServerINI()
{
    AnsiString sFileName=AnsiString().sprintf("PATServer.ini");
    FileInfo().EnsureDirectoriesExist(IniConfig.sB11PATServerPath);
    AnsiString sJobFullFileName=FileInfo().PathCombin(IniConfig.sB11PATServerPath,sFileName);
    return sJobFullFileName;
}
//---------------------------------------------------------------------------
AnsiString PAT_Function::GetPATJobFileDownload()
{
    AnsiString sFileName=GetJobName();
    AnsiString sret=FileInfo().PathCombin(IniConfig.sB13PATJobDownloadPath,sFileName);
    return sret;
}
//---------------------------------------------------------------------------
AnsiString PAT_Function::GetPATJobFileUpload()
{
    AnsiString sFileName=GetJobName();
    AnsiString sret=FileInfo().PathCombin(IniConfig.sB13PATJobUploadPath,sFileName);
    return sret;
}
//---------------------------------------------------------------------------
AnsiString PAT_Function::GetPATSetUpFile()
{
    AnsiString sFileName=AnsiString().sprintf("%s.asc",PC_NAME);
    AnsiString sret=FileInfo().PathCombin(IniConfig.sB12PATSetupPath,sFileName);
    return sret;
}
//---------------------------------------------------------------------------
AnsiString PAT_Function::GetDayHourMinSecStr(const double &dtTime)
{
    double ddata=(dtTime<=0)?0:dtTime;
    int days = static_cast<int>(ddata); // Integer part is the number of days
    double fractionalDay = ddata - days; // Fractional part is the time of day
    int hours = static_cast<int>(fractionalDay * 24);
    double fractionalHour = fractionalDay * 24 - hours;
    int minutes = static_cast<int>(fractionalHour * 60);
    double fractionalMinute = fractionalHour * 60 - minutes;
    int seconds = static_cast<int>(fractionalMinute * 60);
    return AnsiString().sprintf("%d:%d:%d:%d",days,hours,minutes,seconds);
}
//---------------------------------------------------------------------------
bool PAT_Function::CheckMachineStationOnStart()
{
    bool bret=true;
    if(IniConfig.bB12UsePATSetup==true)
    {
        if(patSetup.cLotNo=="")
        {
            ShowMyMessage("【B12】Barcode scanning is required to retrieve data first.");
            return false;
        }
        //temp check
        if(LastSet.iTemperature==Tempture_Hot || LastSet.iTemperature==Tempture_AmbientHot)
        {
            double dNowTemp=Temperature.fWorkTemperBase;
            int iSetTemp=0,iTolerance=0;
            AnalysisTempAndTolerance(patSetup.temp,iSetTemp,iTolerance);
            double dTarTemp=double(iSetTemp);
            if(cMathTool().IsDoubleEqual(dNowTemp,dTarTemp)==false)
            {
                ShowMyMessage("【B12】The actual temperature setting(%f) differs from the target temperature(%f).",dNowTemp,dTarTemp);
                return false;
            }
        }
        //Lot start check
        if(cMathTool().IsDoubleEqual(dStartLotTime,0.0))
        {
            ShowMyMessage("【B12】Not Yet Click Lot Start.");
            return false;
        }
        //Lot end reset
        if(cMathTool().IsDoubleEqual(dEndLotTime,0.0)==false)
        {
            ShowMyMessage("【B12】Not Yet Reset to Clear Lot End.");
            return false;
        }
    }
    return bret;
}
//---------------------------------------------------------------------------
AnsiString PAT_Function::GetClosedSitesName()
{
    AnsiString sret="";
    bool bfirst=true;
    int iRowCounts=TestSocket.iShtRow;
    int iColCounts=TestSocket.iShtCol;
    for(int i=0; i<iRowCounts; i++)
    {
        for(int j=0; j<iColCounts; j++)
        {
            if(TestIF_File.iSiteMap[i][j]==0 ||                                 //Steven 20170302 (wei) : 確認哪個Site有開, 從1開始~32
               bTestSiteUse[0][i][j]==false)
            {
                if(bfirst)
                {
                    bfirst=false;
                }
                else
                {
                    sret+=",";
                }
                sret+=InArmSuck.Suck[i][j].sName;
            }
        }
    }
    return sret;
}
//---------------------------------------------------------------------------
AnsiString PAT_Function::GetProgramName(int iTestIdx)
{
    AnsiString sret="";
    if(iTestIdx==0)     //Normal Test
    {
        sret=patSetup.programName;
    }
    else if(iTestIdx==1)//Pre-Test
    {
        sret=patSetup.prtProgram;
    }
    else if(iTestIdx==2)//Re-Test
    {
        sret=patSetup.rtProgram;
    }
    else if(iTestIdx==3)//GD
    {
        sret=patSetup.gdProgram;
    }
    else//4             //EQC
    {
        sret=patSetup.eqcProgram;
    }
    return sret;
}
//---------------------------------------------------------------------------
AnsiString PAT_Function::GetSoakTime()
{
    AnsiString sret="";
    if((LastSet.iTemperature==Tempture_Hot || LastSet.iTemperature==Tempture_AmbientHot))
    {
        sret=GetDoubleToStr(Temperature.fSoakTime);
    }
    else
    {
        sret="0";
    }
    return sret;
}
//---------------------------------------------------------------------------
void PAT_Function::AnalysisTempAndTolerance(const AnsiString &str,int &iTemp,int &iTolerance)
{
    AnsiString sTemp="", sTolerance="";
    int i=1;
    while(i<=str.Length() && (str[i] >= '0' && str[i] <= '9'))
    {
        sTemp+=str[i];
        i++;
    }
    while (i<=str.Length() && !(str[i] >= '0' && str[i] <= '9'))
    {
        i++;
    }
    while (i<=str.Length() && (str[i] >= '0' && str[i] <= '9'))
    {
        sTolerance+=str[i];
        i++;
    }
    iTemp=atoi(sTemp.c_str());
    iTolerance=atoi(sTolerance.c_str());
}
//---------------------------------------------------------------------------
bool PAT_Function::SaveJobINI(const AnsiString FilePath)
{
    if(FileInfo().IsFilePathExist(FilePath)==false)
    {
        int fileHandle=FileCreate(FilePath);
        if (fileHandle>=0)
        {
            FileClose(fileHandle); // 關閉檔案句柄
        }
        else
        {
            return false;
        }
    }
    //setting
    SetJobTrayToSetting(trayconfig);
    SetJobPlateToSetting(plateconfig);
    SetJobTemperatureToSetting(tempconfig);
    SetJobContactToSetting(contactconfig);
    SetJobAutoCleanToSetting(autocleanconfig);
    SetJobTesterToSetting(testerconfig);
    SetJobVacuumToSetting(vacuumconfig);
    SetJobSpeedToSetting(speedConfig);
    SetJobAlarm3ToSetting(alarm3Config);
    //to ini
    TIniFile* iniFile=new TIniFile(FilePath);
    SetJobTrayConfig(iniFile,trayconfig);
    SetJobPlateConfig(iniFile,plateconfig);
    SetJobTemperatureConfig(iniFile,tempconfig);
    SetJobContactConfig(iniFile,contactconfig);
    SetJobAutoCleanConfig(iniFile,autocleanconfig);
    SetJobTesterConfig(iniFile,testerconfig);
    SetJobVacuumConfig(iniFile,vacuumconfig);
    SetJobSpeedConfig(iniFile,speedConfig);
    SetJobAlarm3Config(iniFile,alarm3Config);
    delete iniFile;
    return true;
}
//---------------------------------------------------------------------------
bool PAT_Function::LoadJobINI(const AnsiString FilePath)
{
    if(FileInfo().IsFilePathExist(FilePath))
    {
        TIniFile* iniFile=new TIniFile(FilePath);
        GetJobTrayConfig(iniFile,trayconfig);
        GetJobPlateConfig(iniFile,plateconfig);
        GetJobTemperatureConfig(iniFile,tempconfig);
        GetJobContactConfig(iniFile,contactconfig);
        GetJobAutoCleanConfig(iniFile,autocleanconfig);
        GetJobTesterConfig(iniFile,testerconfig);
        GetJobVacuumConfig(iniFile,vacuumconfig);
        GetJobSpeedConfig(iniFile,speedConfig);
        GetJobAlarm3Config(iniFile,alarm3Config);
        delete iniFile;
        //setting
        GetJobTrayToSetting(trayconfig);
        GetJobPlateToSetting(plateconfig);
        GetJobTemperatureToSetting(tempconfig);
        GetJobContactToSetting(contactconfig);
        GetJobAutoCleanToSetting(autocleanconfig);
        GetJobTesterToSetting(testerconfig);
        GetJobVacuumToSetting(vacuumconfig);
        GetJobSpeedToSetting(speedConfig);
        GetJobAlarm3ToSetting(alarm3Config);
        return true;
    }
    else
    {
        ShowMyMessage(AnsiString().sprintf("Job file is missing. Path:%s",FilePath));
        return false;
    }
}
//---------------------------------------------------------------------------
void PAT_Function::SetJobTrayConfig(TIniFile* iniFile, const TTrayConfig &tray)
{
    if(iniFile==NULL)
        return;
    iniFile->WriteFloat("Tray", "PitchX",         tray.PitchX);
    iniFile->WriteFloat("Tray", "PitchY",         tray.PitchY);
    iniFile->WriteFloat("Tray", "StartPosX",      tray.StartPosX);
    iniFile->WriteFloat("Tray", "StartPosY",      tray.StartPosY);
    iniFile->WriteInteger("Tray", "DivisionX",      tray.DivisionX);
    iniFile->WriteInteger("Tray", "DivisionY",      tray.DivisionY);
    iniFile->WriteFloat("Tray", "DimemsionX",     tray.DimemsionX);
    iniFile->WriteFloat("Tray", "DimemsionY",     tray.DimemsionY);
    iniFile->WriteFloat("Tray", "ZThickness",     tray.ZThickness);
    iniFile->WriteString("Tray", "UserDefineName", tray.UserDefineName);
}
//---------------------------------------------------------------------------
bool PAT_Function::GetJobTrayConfig(TIniFile* iniFile, TTrayConfig &tray)
{
    if(iniFile==NULL)
        return false;
    tray.PitchX         = iniFile->ReadFloat("Tray", "PitchX", 0.0);
    tray.PitchY         = iniFile->ReadFloat("Tray", "PitchY", 0.0);
    tray.StartPosX      = iniFile->ReadFloat("Tray", "StartPosX", 0.0);
    tray.StartPosY      = iniFile->ReadFloat("Tray", "StartPosY", 0.0);
    tray.DivisionX      = iniFile->ReadInteger("Tray", "DivisionX", 0);
    tray.DivisionY      = iniFile->ReadInteger("Tray", "DivisionY", 0);
    tray.DimemsionX     = iniFile->ReadFloat("Tray", "DimemsionX", 0.0);
    tray.DimemsionY     = iniFile->ReadFloat("Tray", "DimemsionY", 0.0);
    tray.ZThickness     = iniFile->ReadFloat("Tray", "ZThickness", 0.0);
    tray.UserDefineName = iniFile->ReadString("Tray", "UserDefineName", "");
    return true;
}
//---------------------------------------------------------------------------
void PAT_Function::SetJobTrayToSetting(TTrayConfig &tray)
{
    tray.UserDefineName.sprintf("%s",fTrayForm->TrayName1->Text);
    tray.StartPosX      = atof(fTrayForm->XST1->Text.c_str());
    tray.StartPosY      = atof(fTrayForm->YST1->Text.c_str());
    tray.PitchX         = atof(fTrayForm->XPitch1->Text.c_str());
    tray.PitchY         = atof(fTrayForm->YPitch1->Text.c_str());
    tray.DivisionX      = atof(fTrayForm->XCT1->Text.c_str());
    tray.DivisionY      = atof(fTrayForm->YCT1->Text.c_str());
    tray.DimemsionX     = atof(fTrayForm->Tp1TickUp->Text.c_str());
    tray.DimemsionY     = 315;
    tray.ZThickness     = atof(fTrayForm->Tp1Thick->Text.c_str());
}
//---------------------------------------------------------------------------
void PAT_Function::GetJobTrayToSetting(const TTrayConfig &tray)
{
    SetTrayForm_Greatek *stf=&fProductionInfo->sucTrayForm;
    stf->Clear();
    stf->sTRAYPN = tray.UserDefineName;
    stf->sXStartPos = tray.StartPosX;
    stf->sYStartPos = tray.StartPosY;
    stf->sXPitch = tray.PitchX;
    stf->sYPitch = tray.PitchY;
    stf->sColumnsX = tray.DivisionX;
    stf->sRowsY = tray.DivisionY;
    stf->sXWidth = tray.DimemsionX;
    stf->sYHeight = tray.DimemsionY;
    stf->sZThickness = tray.ZThickness;
    stf->Group = "";
    fProductionInfo->SettingTrayFormFromServerOnlyOne();
}
//---------------------------------------------------------------------------
void PAT_Function::SetJobPlateConfig(TIniFile* iniFile, const TPlateConfig &plate)
{
    if(iniFile==NULL)
        return;
    iniFile->WriteFloat("Plate", "PitchX",         plate.PitchX);
    iniFile->WriteFloat("Plate", "PitchY",         plate.PitchY);
    iniFile->WriteFloat("Plate", "StartPositionX", plate.StartPositionX);
    iniFile->WriteFloat("Plate", "StartPositionY", plate.StartPositionY);
    iniFile->WriteInteger("Plate", "DivisionX",      plate.DivisionX);
    iniFile->WriteInteger("Plate", "DivisionY",      plate.DivisionY);
    iniFile->WriteFloat("Plate", "WidthX",         plate.WidthX);
    iniFile->WriteFloat("Plate", "WidthY",         plate.WidthY);
}
//---------------------------------------------------------------------------
bool PAT_Function::GetJobPlateConfig(TIniFile* iniFile, TPlateConfig &plate)
{
    if(iniFile==NULL)
        return false;
    plate.PitchX         = iniFile->ReadFloat("Plate", "PitchX", 0.0);
    plate.PitchY         = iniFile->ReadFloat("Plate", "PitchY", 0.0);
    plate.StartPositionX = iniFile->ReadFloat("Plate", "StartPositionX", 0.0);
    plate.StartPositionY = iniFile->ReadFloat("Plate", "StartPositionY", 0.0);
    plate.DivisionX      = iniFile->ReadInteger("Plate", "DivisionX", 0);
    plate.DivisionY      = iniFile->ReadInteger("Plate", "DivisionY", 0);
    plate.WidthX         = iniFile->ReadFloat("Plate", "WidthX", 0.0);
    plate.WidthY         = iniFile->ReadFloat("Plate", "WidthY", 0.0);
    return true;
}
//---------------------------------------------------------------------------
void PAT_Function::SetJobPlateToSetting(TPlateConfig &plate)
{
    fProductionInfo->OverWritePlateFormData(PlateTablePath,"",plate.StartPositionX,
                                            plate.StartPositionY,plate.PitchX,plate.PitchY,
                                            plate.DivisionX,plate.DivisionY,plate.WidthX,
                                            plate.WidthY,"0","",
                                            "","0","0",
                                            "0","0");
    fProductionInfo->SettingHotPlateFormFromServer();
    plate.StartPositionX=atof(fHotPlate->XST1->Text.c_str());
    plate.StartPositionY=atof(fHotPlate->YST1->Text.c_str());
    plate.PitchX=atof(fHotPlate->XPitch1->Text.c_str());
    plate.PitchY=atof(fHotPlate->YPitch1->Text.c_str());
    plate.DivisionX=atoi(fHotPlate->XCT1->Text.c_str());
    plate.DivisionY=atoi(fHotPlate->YCT1->Text.c_str());
    plate.WidthX=0.0;
    plate.WidthY=0.0;
}
//---------------------------------------------------------------------------
void PAT_Function::GetJobPlateToSetting(const TPlateConfig &plate)
{
    fProductionInfo->OverWritePlateFormData(PlateTablePath,"",plate.StartPositionX,
                                            plate.StartPositionY,plate.PitchX,plate.PitchY,
                                            plate.DivisionX,plate.DivisionY,plate.WidthX,
                                            plate.WidthY,"0","",
                                            "","0","0",
                                            "0","0");
    fProductionInfo->SettingHotPlateFormFromServer();
}
//---------------------------------------------------------------------------
void PAT_Function::SetJobTemperatureConfig(TIniFile* iniFile, const TTemperatureConfig &temp)
{
    if(iniFile==NULL)
        return;
    iniFile->WriteInteger("Temperature", "TemptureMode", temp.TemptureMode);
    iniFile->WriteInteger("Temperature", "Default",      temp.DefaultValue);
    iniFile->WriteInteger("Temperature", "SoakTime",     temp.SoakTime);
}
//---------------------------------------------------------------------------
bool PAT_Function::GetJobTemperatureConfig(TIniFile* iniFile, TTemperatureConfig &temp)
{
    if(iniFile==NULL)
        return false;
    temp.TemptureMode = iniFile->ReadInteger("Temperature", "TemptureMode", 0);
    temp.DefaultValue = iniFile->ReadInteger("Temperature", "Default",      0);
    temp.SoakTime     = iniFile->ReadInteger("Temperature", "SoakTime",     0);
    return true;
}
//---------------------------------------------------------------------------
void PAT_Function::SetJobTemperatureToSetting(TTemperatureConfig &temp)
{
    temp.TemptureMode=LastSet.iTemperature;
    temp.DefaultValue=Temperature.fWorkTemperBase;
    temp.SoakTime=Temperature.fSoakTime;
}
//---------------------------------------------------------------------------
void PAT_Function::GetJobTemperatureToSetting(const TTemperatureConfig &temp)
{
//    fTemp_Set->rgTemperatureMode->ItemIndex=temp.TemptureMode;
    fMain->edWorkTemperBase->Text  =IntToStr(temp.DefaultValue);
    fMain->edSoakTime->Text        =IntToStr(temp.SoakTime);
    Temperature.fWorkTemperBase    =temp.DefaultValue;
    Temperature.fSoakTime          =temp.SoakTime;
    if(atof(fMain->edWorkTemperBase->Text.c_str())>25)
    {
        fMain->ChangeTempMode(1, false, true);
    }
    else
    {
        fMain->ChangeTempMode(0, false, true);
    }
    fMain->SetTemp(false, atof(fMain->edWorkTemperBase->Text.c_str()), atof(fMain->edSoakTime->Text.c_str()));
    fTemp_Set->spbSave->Click();
}
//---------------------------------------------------------------------------
void PAT_Function::SetJobContactConfig(TIniFile* iniFile, const TContactConfig &contact)
{
    if(iniFile==NULL)
        return;
    iniFile->WriteInteger("Contact", "TheNoOfPins", contact.TheNoOfPins);
    iniFile->WriteFloat("Contact", "gf",            contact.gf);
}
//---------------------------------------------------------------------------
bool PAT_Function::GetJobContactConfig(TIniFile* iniFile, TContactConfig &contact)
{
    if(iniFile==NULL)
        return false;
    contact.TheNoOfPins = iniFile->ReadInteger("Contact", "TheNoOfPins", 0);
    contact.gf          = iniFile->ReadFloat("Contact", "gf", 0.0);
    return true;
}
//---------------------------------------------------------------------------
void PAT_Function::SetJobContactToSetting(TContactConfig &contact)
{
    contact.TheNoOfPins=DeviceForm_File.iPinOfDie;
    contact.gf=DeviceForm_File.ForcePerPinG;
}
//---------------------------------------------------------------------------
void PAT_Function::GetJobContactToSetting(const TContactConfig &contact)
{
    fContact->edForcePerPinG->Text=IntToStr(contact.TheNoOfPins);
    fContact->edPinCount->Text=FloatToStr(contact.gf);
    fContact->spbSave->Click();
}
//---------------------------------------------------------------------------
void PAT_Function::SetJobAutoCleanConfig(TIniFile* iniFile, const TAutoCleanConfig &autoClean)
{
    if(iniFile==NULL)
        return;
    iniFile->WriteInteger("AutoClean", "UseAutoClean",         autoClean.UseAutoClean);
    iniFile->WriteInteger("AutoClean", "InitialStart",         autoClean.InitialStart);
    iniFile->WriteInteger("AutoClean", "InitialRetestStart",   autoClean.InitialRetestStart);
    iniFile->WriteInteger("AutoClean", "Finish",               autoClean.Finish);
    iniFile->WriteInteger("AutoClean", "Manual",               autoClean.Manual);
    iniFile->WriteInteger("AutoClean", "SocketAlarmFail",      autoClean.SocketAlarmFail);
    iniFile->WriteInteger("AutoClean", "SocketAlarmFailCount", autoClean.SocketAlarmFailCount);
    iniFile->WriteInteger("AutoClean", "Interval",             autoClean.Interval);
    iniFile->WriteInteger("AutoClean", "IntervalCount",        autoClean.IntervalCount);
    iniFile->WriteInteger("AutoClean", "ContinuousFail",       autoClean.ContinuousFail);
    iniFile->WriteInteger("AutoClean", "ContinuousFailCount",  autoClean.ContinuousFailCount);
    iniFile->WriteInteger("AutoClean", "NumberPices",          autoClean.NumberPices);
    iniFile->WriteInteger("AutoClean", "Tray",                 autoClean.Tray);
    iniFile->WriteInteger("AutoClean", "AlarmCount",           autoClean.AlarmCount);
    iniFile->WriteInteger("AutoClean", "CleanPadDeviation",    autoClean.CleanPadDeviation);
    iniFile->WriteInteger("AutoClean", "ContactMode",          autoClean.ContactMode);
    iniFile->WriteInteger("AutoClean", "ContactTime",          autoClean.ContactTime);
    iniFile->WriteInteger("AutoClean", "ContactCount",         autoClean.ContactCount);
    iniFile->WriteInteger("AutoClean", "PinCount",             autoClean.PinCount);
    iniFile->WriteFloat("AutoClean",   "PinForceN",            autoClean.PinForceN);
    iniFile->WriteFloat("AutoClean",   "PinForceGf",           autoClean.PinForceGf);
    iniFile->WriteInteger("AutoClean", "OutArmSpeed",          autoClean.OutArmSpeed);
    iniFile->WriteInteger("AutoClean", "ShuttleSpeed",         autoClean.ShuttleSpeed);
    iniFile->WriteInteger("AutoClean", "IndexArmSpeed",        autoClean.IndexArmSpeed);
    iniFile->WriteInteger("AutoClean", "InArmSpeed",           autoClean.InArmSpeed);
}
//---------------------------------------------------------------------------
bool PAT_Function::GetJobAutoCleanConfig(TIniFile* iniFile, TAutoCleanConfig &autoClean)
{
    if(iniFile==NULL)
        return false;
    autoClean.UseAutoClean         = iniFile->ReadInteger("AutoClean", "UseAutoClean",         0);
    autoClean.InitialStart         = iniFile->ReadInteger("AutoClean", "InitialStart",         0);
    autoClean.InitialRetestStart   = iniFile->ReadInteger("AutoClean", "InitialRetestStart",   0);
    autoClean.Finish               = iniFile->ReadInteger("AutoClean", "Finish",               0);
    autoClean.Manual               = iniFile->ReadInteger("AutoClean", "Manual",               0);
    autoClean.SocketAlarmFail      = iniFile->ReadInteger("AutoClean", "SocketAlarmFail",      0);
    autoClean.SocketAlarmFailCount = iniFile->ReadInteger("AutoClean", "SocketAlarmFailCount", 0);
    autoClean.Interval             = iniFile->ReadInteger("AutoClean", "Interval",             0);
    autoClean.IntervalCount        = iniFile->ReadInteger("AutoClean", "IntervalCount",        0);
    autoClean.ContinuousFail       = iniFile->ReadInteger("AutoClean", "ContinuousFail",       0);
    autoClean.ContinuousFailCount  = iniFile->ReadInteger("AutoClean", "ContinuousFailCount",  0);
    autoClean.NumberPices          = iniFile->ReadInteger("AutoClean", "NumberPices",          0);
    autoClean.Tray                 = iniFile->ReadInteger("AutoClean", "Tray",                 0);
    autoClean.AlarmCount           = iniFile->ReadInteger("AutoClean", "AlarmCount",           0);
    autoClean.CleanPadDeviation    = iniFile->ReadInteger("AutoClean", "CleanPadDeviation",    0);
    autoClean.ContactMode          = iniFile->ReadInteger("AutoClean", "ContactMode",          0);
    autoClean.ContactTime          = iniFile->ReadInteger("AutoClean", "ContactTime",          0);
    autoClean.ContactCount         = iniFile->ReadInteger("AutoClean", "ContactCount",         0);
    autoClean.PinCount             = iniFile->ReadInteger("AutoClean", "PinCount",             0);
    autoClean.PinForceN            = iniFile->ReadFloat("AutoClean",   "PinForceN",            0.0);
    autoClean.PinForceGf           = iniFile->ReadFloat("AutoClean",   "PinForceGf",           0.0);
    autoClean.OutArmSpeed          = iniFile->ReadInteger("AutoClean", "OutArmSpeed",          0);
    autoClean.ShuttleSpeed         = iniFile->ReadInteger("AutoClean", "ShuttleSpeed",         0);
    autoClean.IndexArmSpeed        = iniFile->ReadInteger("AutoClean", "IndexArmSpeed",        0);
    autoClean.InArmSpeed           = iniFile->ReadInteger("AutoClean", "InArmSpeed",           0);
    return true;
}
//---------------------------------------------------------------------------
void PAT_Function::SetJobAutoCleanToSetting(TAutoCleanConfig &autoClean)
{
    autoClean.UseAutoClean = fCleaning->rgAutoCleanOnOff->ItemIndex;
    autoClean.InitialStart = fCleaning->chkAutoCleanMode1->Checked ? 1 : 0;
    autoClean.InitialRetestStart = fCleaning->chkAutoCleanMode2->Checked ? 1 : 0;
    autoClean.Finish = fCleaning->chkAutoCleanMode3->Checked ? 1 : 0;
    autoClean.Manual = fCleaning->chkAutoCleanMode4->Checked ? 1 : 0;
    autoClean.SocketAlarmFail = fCleaning->chkAutoCleanMode5->Checked ? 1 : 0;
    autoClean.SocketAlarmFailCount = StrToIntDef(fCleaning->edAlarmCount->Text, 0);
    autoClean.Interval = fCleaning->chkAutoCleanMode6->Checked ? 1 : 0;
    autoClean.IntervalCount = StrToIntDef(fCleaning->edIntervalContact->Text, 0);
    autoClean.NumberPices = StrToIntDef(fCleaning->edPinsCount->Text, 0);
    autoClean.Tray = fCleaning->rgCleanKitType->ItemIndex;
    autoClean.CleanPadDeviation = StrToIntDef(fCleaning->edCleanPadDeviation->Text, 0);
    autoClean.ContactMode = fCleaning->ContactMode->ItemIndex;
    autoClean.ContactTime = StrToIntDef(fCleaning->edContactTime->Text, 0);
    autoClean.ContactCount = StrToIntDef(fCleaning->edACContactCount->Text, 0);
    autoClean.PinCount = StrToIntDef(fCleaning->edPinsCount->Text, 0);
    autoClean.PinForceN = StrToFloatDef(fCleaning->edPinSingleN->Text, 0.0);
    autoClean.PinForceGf = StrToFloatDef(fCleaning->edPinSingleGf->Text, 0.0);
    autoClean.OutArmSpeed = StrToIntDef(fCleaning->OutArmSpeed->Text, 0);
    autoClean.ShuttleSpeed = StrToIntDef(fCleaning->ShuttleSpeed->Text, 0);
    autoClean.IndexArmSpeed = StrToIntDef(fCleaning->IndexArmSpeed->Text, 0);
    autoClean.InArmSpeed = StrToIntDef(fCleaning->edtInArmZSpeed->Text, 0);
}
//---------------------------------------------------------------------------
void PAT_Function::GetJobAutoCleanToSetting(const TAutoCleanConfig &autoClean)
{
    fCleaning->rgAutoCleanOnOff->ItemIndex=autoClean.UseAutoClean;
    fCleaning->chkAutoCleanMode1->Checked=(autoClean.InitialStart==1)?true:false;
    fCleaning->chkAutoCleanMode2->Checked=(autoClean.InitialRetestStart==1)?true:false;
    fCleaning->chkAutoCleanMode3->Checked=(autoClean.Finish==1)?true:false;
    fCleaning->chkAutoCleanMode4->Checked=(autoClean.Manual==1)?true:false;
    fCleaning->chkAutoCleanMode5->Checked=(autoClean.SocketAlarmFail==1)?true:false;
    fCleaning->edAlarmCount->Text=IntToStr(autoClean.SocketAlarmFailCount);
    fCleaning->chkAutoCleanMode6->Checked=(autoClean.Interval==1)?true:false;
    fCleaning->edIntervalContact->Text=IntToStr(autoClean.IntervalCount);
    fCleaning->edPinsCount->Text=IntToStr(autoClean.NumberPices);
    fCleaning->rgCleanKitType->ItemIndex=autoClean.Tray;
    fCleaning->edCleanPadDeviation->Text=IntToStr(autoClean.CleanPadDeviation);
    fCleaning->ContactMode->ItemIndex=autoClean.ContactMode;
    fCleaning->edContactTime->Text=IntToStr(autoClean.ContactTime);
    fCleaning->edACContactCount->Text=IntToStr(autoClean.ContactCount);
    fCleaning->edPinsCount->Text=IntToStr(autoClean.PinCount);
    fCleaning->edPinSingleN->Text=FloatToStr(autoClean.PinForceN);
    fCleaning->edPinSingleGf->Text=FloatToStr(autoClean.PinForceGf);
    fCleaning->OutArmSpeed->Text=IntToStr(autoClean.OutArmSpeed);
    fCleaning->ShuttleSpeed->Text=IntToStr(autoClean.ShuttleSpeed);
    fCleaning->IndexArmSpeed->Text=IntToStr(autoClean.IndexArmSpeed);
    fCleaning->edtInArmZSpeed->Text=IntToStr(autoClean.InArmSpeed);
    //
    fCleaning->SaveAutoCleanData();
}
//---------------------------------------------------------------------------
void PAT_Function::SetJobTesterConfig(TIniFile* iniFile, const TTesterConfig &tester)
{
    if(iniFile==NULL)
        return;
    iniFile->WriteInteger("Tester", "TestMode", tester.TestMode);
    iniFile->WriteInteger("Tester", "Rs232_Interface", tester.Rs232_Interface);
    iniFile->WriteInteger("Tester", "AntiSignal", tester.AntiSignal);
    iniFile->WriteInteger("Tester", "Gpib_Interface", tester.Gpib_Interface);
    iniFile->WriteInteger("Tester", "GpibAddress", tester.GpibAddress);
}
//---------------------------------------------------------------------------
bool PAT_Function::GetJobTesterConfig(TIniFile* iniFile, TTesterConfig &tester)
{
    if(iniFile==NULL)
        return false;
    tester.TestMode = iniFile->ReadInteger("Tester", "TestMode", 0);
    tester.Rs232_Interface = iniFile->ReadInteger("Tester", "Rs232_Interface", 0);
    tester.AntiSignal = iniFile->ReadInteger("Tester", "AntiSignal", 0);
    tester.Gpib_Interface = iniFile->ReadInteger("Tester", "Gpib_Interface", 0);
    tester.GpibAddress = iniFile->ReadInteger("Tester", "GpibAddress", 0);
    return true;
}
//---------------------------------------------------------------------------
void PAT_Function::SetJobTesterToSetting(TTesterConfig &tester)
{
    tester.TestMode = FTestIF->rgInterfaceType->ItemIndex;
    tester.Gpib_Interface = FTestIF->cbGPIBType->ItemIndex;
    tester.GpibAddress = StrToIntDef(FTestIF->edGPIBAddress->Text, 0);
}
//---------------------------------------------------------------------------
void PAT_Function::GetJobTesterToSetting(const TTesterConfig &tester)
{
    FTestIF->rgInterfaceType->ItemIndex=tester.TestMode;
    FTestIF->cbRs232Type->ItemIndex=tester.TestMode;
    FTestIF->cbGPIBType->ItemIndex=tester.Gpib_Interface;
    FTestIF->edGPIBAddress->Text=IntToStr(tester.GpibAddress);
    //
    AnsiString S="";
    S=GetLastOpenFN();
    AnsiString szDir="";
    szDir=DataPath+S;
    FTestIF->SaveSetupFile(szDir,S);
}
//---------------------------------------------------------------------------
void PAT_Function::SetJobVacuumConfig(TIniFile* iniFile, const TVacuumConfig &vacuum)
{
    if(iniFile==NULL)
        return;
    iniFile->WriteFloat("Vacuum", "Arm1VacuumWait", vacuum.Arm1VacuumWait);
    iniFile->WriteFloat("Vacuum", "IndexVacuumWait", vacuum.IndexVacuumWait);
    iniFile->WriteFloat("Vacuum", "OutVacuumWait", vacuum.OutVacuumWait);
    iniFile->WriteFloat("Vacuum", "TrayVacuumWait", vacuum.TrayVacuumWait);
    iniFile->WriteFloat("Vacuum", "Arm1DestroyWait", vacuum.Arm1DestroyWait);
    iniFile->WriteFloat("Vacuum", "IndexDestroyWait", vacuum.IndexDestroyWait);
    iniFile->WriteFloat("Vacuum", "OutDestroyWait", vacuum.OutDestroyWait);
    iniFile->WriteFloat("Vacuum", "TrayDestroyWait", vacuum.TrayDestroyWait);
}
//---------------------------------------------------------------------------
bool PAT_Function::GetJobVacuumConfig(TIniFile* iniFile, TVacuumConfig &vacuum)
{
    if(iniFile==NULL)
        return false;
    vacuum.Arm1VacuumWait   = iniFile->ReadFloat("Vacuum", "Arm1VacuumWait", 0.0);
    vacuum.IndexVacuumWait  = iniFile->ReadFloat("Vacuum", "IndexVacuumWait", 0.0);
    vacuum.OutVacuumWait    = iniFile->ReadFloat("Vacuum", "OutVacuumWait", 0.0);
    vacuum.TrayVacuumWait   = iniFile->ReadFloat("Vacuum", "TrayVacuumWait", 0.0);
    vacuum.Arm1DestroyWait  = iniFile->ReadFloat("Vacuum", "Arm1DestroyWait", 0.0);
    vacuum.IndexDestroyWait = iniFile->ReadFloat("Vacuum", "IndexDestroyWait", 0.0);
    vacuum.OutDestroyWait   = iniFile->ReadFloat("Vacuum", "OutDestroyWait", 0.0);
    vacuum.TrayDestroyWait  = iniFile->ReadFloat("Vacuum", "TrayDestroyWait", 0.0);
    return true;
}
//---------------------------------------------------------------------------
void PAT_Function::SetJobVacuumToSetting(TVacuumConfig &vacuum)
{
    vacuum.Arm1VacuumWait = StrToFloatDef(fSpeed->edInVacumCheckTime->Text, 0.0);
    vacuum.IndexVacuumWait = StrToFloatDef(fSpeed->edIndexVacumCheckTime->Text, 0.0);
    vacuum.OutVacuumWait = StrToFloatDef(fSpeed->edOutVacumCheckTime->Text, 0.0);
    vacuum.TrayVacuumWait = StrToFloatDef(fSpeed->edTrayVacumCheckTime->Text, 0.0);
    vacuum.Arm1DestroyWait = StrToFloatDef(fSpeed->edInArmAirOnTime->Text, 0.0);
    vacuum.IndexDestroyWait = StrToFloatDef(fSpeed->edIndexAirOnTime->Text, 0.0);
    vacuum.OutDestroyWait = StrToFloatDef(fSpeed->edOutArmAirOnTime->Text, 0.0);
    vacuum.TrayDestroyWait = StrToFloatDef(fSpeed->edTrayArmAirOnTime->Text, 0.0);
}
//---------------------------------------------------------------------------
void PAT_Function::GetJobVacuumToSetting(const TVacuumConfig &vacuum)
{
    fSpeed->edInVacumCheckTime->Text=FloatToStr(vacuum.Arm1VacuumWait);
    fSpeed->edIndexVacumCheckTime->Text=FloatToStr(vacuum.IndexVacuumWait);
    fSpeed->edOutVacumCheckTime->Text=FloatToStr(vacuum.OutVacuumWait);
    fSpeed->edTrayVacumCheckTime->Text=FloatToStr(vacuum.TrayVacuumWait);
    fSpeed->edInArmAirOnTime->Text=FloatToStr(vacuum.Arm1DestroyWait);
    fSpeed->edIndexAirOnTime->Text=FloatToStr(vacuum.IndexDestroyWait);
    fSpeed->edOutArmAirOnTime->Text=FloatToStr(vacuum.OutDestroyWait);
    fSpeed->edTrayArmAirOnTime->Text=FloatToStr(vacuum.TrayDestroyWait);
    //
    fSpeed->spbSave->Click();
}
//---------------------------------------------------------------------------
void PAT_Function::SetJobSpeedConfig(TIniFile* iniFile, const TSpeedConfig &speed)
{
    if(iniFile==NULL)
        return;
    iniFile->WriteInteger("Speed", "InArm2Speed", speed.InArm2Speed);
    iniFile->WriteInteger("Speed", "ShuttleSpeed", speed.ShuttleSpeed);
    iniFile->WriteInteger("Speed", "IndexSpeed", speed.IndexSpeed);
    iniFile->WriteInteger("Speed", "OutArmSpeed", speed.OutArmSpeed);
    iniFile->WriteInteger("Speed", "CatchTraySpeed", speed.CatchTraySpeed);
    iniFile->WriteInteger("Speed", "TrayLoaderSpeed", speed.TrayLoaderSpeed);
}
//---------------------------------------------------------------------------
bool PAT_Function::GetJobSpeedConfig(TIniFile* iniFile, TSpeedConfig &speed)
{
    if(iniFile==NULL)
        return false;
    speed.InArm2Speed    = iniFile->ReadInteger("Speed", "InArm2Speed", 0);
    speed.ShuttleSpeed   = iniFile->ReadInteger("Speed", "ShuttleSpeed", 0);
    speed.IndexSpeed     = iniFile->ReadInteger("Speed", "IndexSpeed", 0);
    speed.OutArmSpeed    = iniFile->ReadInteger("Speed", "OutArmSpeed", 0);
    speed.CatchTraySpeed = iniFile->ReadInteger("Speed", "CatchTraySpeed", 0);
    speed.TrayLoaderSpeed= iniFile->ReadInteger("Speed", "TrayLoaderSpeed", 0);
    return true;
}
//---------------------------------------------------------------------------
void PAT_Function::SetJobSpeedToSetting(TSpeedConfig &speed)
{
    speed.InArm2Speed = StrToIntDef(fSpeed->edInXYSpd->Text, 0);
    speed.ShuttleSpeed = StrToIntDef(fSpeed->edSht1Spd->Text, 0);
    speed.IndexSpeed = StrToIntDef(fSpeed->edIndexSpeed->Text, 0);
    speed.OutArmSpeed = StrToIntDef(fSpeed->edOutXSpd->Text, 0);
    speed.CatchTraySpeed = StrToIntDef(fSpeed->edTrayXSpd->Text, 0);
    speed.TrayLoaderSpeed = StrToIntDef(fSpeed->edtLoaderSpeed1->Text, 0);
}
//---------------------------------------------------------------------------
void PAT_Function::GetJobSpeedToSetting(const TSpeedConfig &speed)
{
    //inarm
    fSpeed->edInXYSpd->Text=IntToStr(speed.InArm2Speed);
    fSpeed->edInPitchSpd->Text=IntToStr(speed.InArm2Speed);
    //sht
    fSpeed->edSht1Spd->Text=IntToStr(speed.ShuttleSpeed);
    fSpeed->edSht2Spd->Text=IntToStr(speed.ShuttleSpeed);
    //index
    fSpeed->edIndexSpeed->Text=IntToStr(speed.IndexSpeed);
    //outarm
    fSpeed->edOutXSpd->Text=IntToStr(speed.OutArmSpeed);
    fSpeed->edOutPitchSpd->Text=IntToStr(speed.OutArmSpeed);
    //catch tray
    fSpeed->edTrayXSpd->Text=IntToStr(speed.CatchTraySpeed);
    //tray loader
    fSpeed->edtLoaderSpeed1->Text=IntToStr(speed.TrayLoaderSpeed);
    //
    fSpeed->spbSave->Click();
}
//---------------------------------------------------------------------------
void PAT_Function::SetJobAlarm3Config(TIniFile* iniFile, const TAlarm3Config &alarm3)
{
    if(iniFile==NULL)
        return;
    iniFile->WriteInteger("Alarm3", "OpenFunction", alarm3.OpenFunction);
    iniFile->WriteInteger("Alarm3", "ViewMode", alarm3.ViewMode);
    iniFile->WriteInteger("Alarm3", "SiteABinContinuouCount", alarm3.SiteABinContinuouCount);
    iniFile->WriteInteger("Alarm3", "SiteBBinContinuouCount", alarm3.SiteBBinContinuouCount);
    iniFile->WriteInteger("Alarm3", "SiteCBinContinuouCount", alarm3.SiteCBinContinuouCount);
    iniFile->WriteInteger("Alarm3", "SiteDBinContinuouCount", alarm3.SiteDBinContinuouCount);
    iniFile->WriteInteger("Alarm3", "SiteEBinContinuouCount", alarm3.SiteEBinContinuouCount);
    iniFile->WriteInteger("Alarm3", "SiteFBinContinuouCount", alarm3.SiteFBinContinuouCount);
    iniFile->WriteInteger("Alarm3", "SiteGBinContinuouCount", alarm3.SiteGBinContinuouCount);
    iniFile->WriteInteger("Alarm3", "SiteHBinContinuouCount", alarm3.SiteHBinContinuouCount);
}
//---------------------------------------------------------------------------
bool PAT_Function::GetJobAlarm3Config(TIniFile* iniFile, TAlarm3Config &alarm3)
{
    if(iniFile==NULL)
        return false;
    alarm3.OpenFunction              = iniFile->ReadInteger("Alarm3", "OpenFunction", 0);
    alarm3.ViewMode                  = iniFile->ReadInteger("Alarm3", "ViewMode", 0);
    alarm3.SiteABinContinuouCount    = iniFile->ReadInteger("Alarm3", "SiteABinContinuouCount", 0);
    alarm3.SiteBBinContinuouCount    = iniFile->ReadInteger("Alarm3", "SiteBBinContinuouCount", 0);
    alarm3.SiteCBinContinuouCount    = iniFile->ReadInteger("Alarm3", "SiteCBinContinuouCount", 0);
    alarm3.SiteDBinContinuouCount    = iniFile->ReadInteger("Alarm3", "SiteDBinContinuouCount", 0);
    alarm3.SiteEBinContinuouCount    = iniFile->ReadInteger("Alarm3", "SiteEBinContinuouCount", 0);
    alarm3.SiteFBinContinuouCount    = iniFile->ReadInteger("Alarm3", "SiteFBinContinuouCount", 0);
    alarm3.SiteGBinContinuouCount    = iniFile->ReadInteger("Alarm3", "SiteGBinContinuouCount", 0);
    alarm3.SiteHBinContinuouCount    = iniFile->ReadInteger("Alarm3", "SiteHBinContinuouCount", 0);
    return true;
}
//---------------------------------------------------------------------------
void PAT_Function::SetJobAlarm3ToSetting(TAlarm3Config &alarm3)
{
    //
}
//---------------------------------------------------------------------------
void PAT_Function::GetJobAlarm3ToSetting(const TAlarm3Config &alarm3)
{
    //
}
//---------------------------------------------------------------------------
AnsiString PAT_Function::GetPAT_Folder()
{
    AnsiString sRet=AnsiString("D:\\HT9045\\system\\PATFolder");
    FileInfo().EnsureDirectoriesExist(sRet);
    return sRet;
}
//---------------------------------------------------------------------------
AnsiString PAT_Function::GetJobName()
{
    AnsiString sRet=AnsiString().sprintf("%s.ini",patSetup.recipe);
    return sRet;
}
//---------------------------------------------------------------------------
AnsiString PAT_Function::GetDoublePercentStr(const double &dMolecular,const double &dDenominator)
{
    AnsiString sRet="0.00%";
    if(cMathTool().IsDoubleEqual(dDenominator,0.0,0.00999))
    {
        return sRet;
    }
    else
    {
        double dret=(ChangeToFloatNonPcnt((double)(dMolecular), (double)(dDenominator)))*100.0;
        sRet=AnsiString().sprintf("%s%",GetDoubleToStr(dret));
        return sRet;
    }
}
//---------------------------------------------------------------------------
void PAT_Function::SetStartLotTime(const double &dValue)
{
    dStartLotTime=dValue;
}
//---------------------------------------------------------------------------
void PAT_Function::SetEndLotTime(const double &dValue)
{
    dEndLotTime=dValue;
}
//---------------------------------------------------------------------------
void PAT_Function::SetJamLog(const AnsiString& sJamArea,const AnsiString& sMsg)
{
    GetTimeInfo();
    AnsiString sDate=AnsiString().sprintf("%04d/%02d/%02d", SystemYear,SystemMonth,SystemDate);
    AnsiString sTime=AnsiString().sprintf("%02d:%02d:%02d", SystemHour,SystemMin,SystemSec);
    AnsiString sPosition=sJamArea;
    //msg
    AnsiString sSaveMsg=AnsiString().sprintf("%s %s %s : %s",sDate,sTime,sPosition,sMsg);
    JamLogs.push_back(sSaveMsg);
}
//---------------------------------------------------------------------------
int PAT_Function::GetAreaByBin(int iBin)                                        //Bin1=1 ; Bin2=2
{
    int iArea=Prod.iT6PosCate[iBin];
    iArea=iArea-1;
    return iArea;//eLastSetBinCTArea
}
//---------------------------------------------------------------------------
int PAT_Function::GetBinByArea(int iArea)                                       //Bin1=1 ; Bin2=2
{
    iArea=iArea+1;
    for(int i=1;i<=15;i++)//15
    {
        if(Prod.iT6PosCate[i]==iArea)
        {
            return i;
        }
    }
    return 0;
}
//---------------------------------------------------------------------------
TDateTime PAT_Function::SecondsToDateTime(int seconds)
{
    double dnum=(double)seconds/86400.0;
    return dnum;
}
//---------------------------------------------------------------------------
TMachineTimeManager::TMachineTimeManager()
{
    totalRunning = 0;
    totalPause = 0;
    totalPauseBeforeTest=0;
    state = STATE_STOPPED;
}
//---------------------------------------------------------------------------
TMachineTimeManager::~TMachineTimeManager()
{
}
//---------------------------------------------------------------------------
void TMachineTimeManager::UpdateTime()
{
    TDateTime currentTime = Now();
    TDateTime duration = currentTime - lastSwitchTime;
    if (state == STATE_RUNNING)
    {
        totalRunning += duration;
    }
    else if (state == STATE_PAUSED)
    {
        totalPause += duration;
    }
    else if (state == STATE_PAUSED_BEFORE_TEST)
    {
        totalPauseBeforeTest += duration;
    }
    lastSwitchTime = currentTime;
}
//---------------------------------------------------------------------------
void TMachineTimeManager::StartLot()
{
    if (state != STATE_STOPPED)
    {
        EndLot();  // 如果已在運行，先結束上一個 Lot
    }
    totalRunning = 0;
    totalPause = 0;
    totalPauseBeforeTest=0;
    startTime = Now();
    lastSwitchTime = startTime;
    state = STATE_PAUSED_BEFORE_TEST;  // 預設從運作停止
}
//---------------------------------------------------------------------------
void TMachineTimeManager::Pause()
{
    if (state == STATE_RUNNING)
    {
        UpdateTime();  // 記錄運作時間
        state = STATE_PAUSED;
    }
    // 如果已在暫停或停止，無效
}
//---------------------------------------------------------------------------
void TMachineTimeManager::Resume()
{
    if (state == STATE_PAUSED ||
        state == STATE_PAUSED_BEFORE_TEST)
    {
        UpdateTime();  // 記錄暫停時間
        state = STATE_RUNNING;
    }
    // 如果已在運作或停止，無效
}
//---------------------------------------------------------------------------
void TMachineTimeManager::EndLot()
{
    if (state != STATE_STOPPED)
    {
        UpdateTime();  // 記錄最後時間
        state = STATE_STOPPED;
    }
}
//---------------------------------------------------------------------------
double TMachineTimeManager::GetTotalRunningTime() const
{
    return totalRunning;
}
//---------------------------------------------------------------------------
double TMachineTimeManager::GetTotalPauseTime() const
{
    return totalPause;
}
//---------------------------------------------------------------------------
double TMachineTimeManager::GetTotalPauseTimeBeforeTest() const
{
    return totalPauseBeforeTest;
}
//---------------------------------------------------------------------------
