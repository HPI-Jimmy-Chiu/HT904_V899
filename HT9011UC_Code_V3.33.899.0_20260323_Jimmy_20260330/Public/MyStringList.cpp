#include "MachineDefine.h"
#pragma hdrstop

#include "MyStringList.h"
#include "cmydef.h"
#include "cObserver.h"
//#include "MemoryAlarm.h"
//#include "HonNewDel.h"
#include "common.h"
#include "uLotInfo.h"
#include "SCK_ART.h"
#include "FileInfo.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TMyStringList::TMyStringList()
{
    MaxLineCount        =1000;
    Path                ="D:\\HandlerLog";
    FirstRow            ="";
    SaveType            =TByDay;
    AutoSave            =true;
    bUseFTRT            =false;
    bFilePathWithDate   =true;                                                  //Steven 20210623 : 預設存檔要有日期當資料夾
    bHanaTrayMap        =false;
    HTSaveFixedFile     =false;
    MyList              =new TStringList();
    sPrevFileName       ="";                                                    //Steven 20250809 : 紀錄前一個的檔名
    bChangeFile         =false;
//    MyList              =HonNew(MyList, __FILE__, __FUNC__, __LINE__);
}
//---------------------------------------------------------------------------
__fastcall TMyStringList::TMyStringList(AnsiString sPath, AnsiString sFileName, AnsiString sFirstRow)
{
    MaxLineCount        =1;
    Path                =sPath;
    FileName            =sFileName;
    FirstRow            =sFirstRow;

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)
        SaveType            =TByHour;                                           //kevin 20210518 Data too large    TByHour=TByDay;
    else
        SaveType            =TByDay;

    AutoSave            =true;
    bUseFTRT            =false;
    bFilePathWithDate   =true;                                                  //Steven 20210623 : 預設存檔要有日期當資料夾
    bHanaTrayMap        =false;
    HTSaveFixedFile     =false;
    MyList              =new TStringList();
    sPrevFileName       ="";                                                    //Steven 20250809 : 紀錄前一個的檔名
    bChangeFile         =false;
//    MyList              =HonNew(MyList, __FILE__, __FUNC__, __LINE__);
}
//---------------------------------------------------------------------------
__fastcall TMyStringList::~TMyStringList()
{
    try
    {
        MySaveToFile();
        MyList->Clear();                                                        //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        delete MyList;
//        HonDelete(MyList);
    }
    catch(...)
    {
        MyDBIProcess("Exception", "~TMyStringList");
    }
}
//---------------------------------------------------------------------------
void __fastcall TMyStringList::SetPath(AnsiString P)
{
    HTPath=P;
}
//---------------------------------------------------------------------------
void __fastcall TMyStringList::SetFileName(AnsiString P)
{
    HTFileName=P;
}
//---------------------------------------------------------------------------
void __fastcall TMyStringList::SetFirstRow(AnsiString P)
{
    HTFirstRow=P;
}
//---------------------------------------------------------------------------
void __fastcall TMyStringList::SetMaxLineCount(int Cnt)
{
    HTMaxLineCount=Cnt;
}
//---------------------------------------------------------------------------
void __fastcall TMyStringList::SetSaveType(TSaveType Type)
{
    HTSaveType=Type;
}
//---------------------------------------------------------------------------
void __fastcall TMyStringList::SetAutoSave(bool P)
{
    HTAutoSave=P;
}
//---------------------------------------------------------------------------
void __fastcall TMyStringList::SetSaveSameFolder(bool P)                        //KaiChen 20180322 ：矽格-湖口 新增 將 Event Log 放在同個資料夾
{
    HTSaveSameFolder=P;
}
//---------------------------------------------------------------------------
void __fastcall TMyStringList::SetSaveByLotID(bool P)                           //KaiChen 20181121 ：矽格-北興 Save Event Log by Lot ID
{
    HTSaveByLotID=P;
}
//---------------------------------------------------------------------------
void __fastcall TMyStringList::SetSaveFixedFile(bool P)                         //Steven 20250716 : 固定檔名存檔
{
    HTSaveFixedFile=P;
}
//---------------------------------------------------------------------------
void TMyStringList::SetLotData(AnsiString ID, AnsiString Time, AnsiString LotFileName)
{
    HTLotID=ID;                                                                 //KaiChen 20181121 ：矽格-北興 Save Event Log by Lot ID
    HTLotStartTime=Time;                                                        //Steven 20250520 : 加入By Lot存檔

    if(ID=="")
    {
        sLotFileName="";
    }
    else if(LotFileName=="")                                                    //Steven 20250809 : 修正event log上傳
    {
        LotFileName.sprintf("%s\\ByLotID\\%04d\\%02d\\%02d", HTPath, SystemYear, SystemMonth, SystemDate);
        MyForceDirectories(LotFileName, "TMyStringList::SetLotData");
        sLotFileName.sprintf("%s\\%s_%s_%s_%s_ByLotEventLog.csv", LotFileName, IniConfig.sMachineType, IniConfig.SocketHandlerID, HTLotID, HTLotStartTime);
    }
    else
    {
        sLotFileName=LotFileName;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMyStringList::AddText(AnsiString Msg)
{
    GetTimeInfo();

    if(FileName=="")
        FileName="";

    MyList->Add(Msg);

    if(MyList->Count>=HTMaxLineCount)
    {
        MySaveToFile();
        MyList->Clear();
    }
}
//------------------------------------------------------------------------------
void __fastcall TMyStringList::AddTextWithLineNo(AnsiString Msg)
{
    AnsiString Str;
    Str.sprintf("%s", Msg);
    AddText(Str);
}
//------------------------------------------------------------------------------
void __fastcall TMyStringList::AddTextWithDateTime(AnsiString Msg)
{
    AnsiString Str;
    GetTimeInfo();
    if(IniConfig.bSIGURDFunction)                                               //Sam 矽格北興俊堯 要求不要留空格。
        Str.sprintf("%04d-%02d-%02d, %02d:%02d:%02d.%03d,%s", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec, Msg);
    else
        Str.sprintf("%04d-%02d-%02d, %02d:%02d:%02d.%03d, %s", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec, Msg);

    if(FileName=="")
        FileName="";

    if(MyList->Count>HTMaxLineCount)
    {
        MySaveToFile();
        MyList->Clear();
    }
    MyList->Add(Str);
}
//------------------------------------------------------------------------------
AnsiString __fastcall TMyStringList::AddTextWithDateTime2(AnsiString Msg)
{
    AnsiString Str;
    GetTimeInfo();
    Str.sprintf("%04d/%02d/%02d %02d:%02d:%02d,%s", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, Msg);

    if(FileName=="")
        FileName="";

    if(MyList->Count>HTMaxLineCount)
    {
        MySaveToFile();
        MyList->Clear();
    }
    MyList->Add(Str);
    return Str;
}
//------------------------------------------------------------------------------
void __fastcall TMyStringList::AddTextWithDateTime3(AnsiString Msg, AnsiString Msg2)
{
    AnsiString Str;
    GetTimeInfo();
    Str.sprintf("%04d-%02d-%02d, %02d:%02d:%02d, %03d, %s, %s", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec, Msg, Msg2);

    if(FileName=="")
        FileName="";

    if(MyList->Count>HTMaxLineCount)
    {
        MySaveToFile();
        MyList->Clear();
    }
    MyList->Add(Str);
}
//------------------------------------------------------------------------------
void TMyStringList::GetTimeInfo()
{
    static TDateTime dtPresent;
    dtPresent=Now();
    DecodeDate(dtPresent, SystemYear, SystemMonth, SystemDate);
    DecodeTime(dtPresent, SystemHour, SystemMin, SystemSec, SystemMSec);
}
//------------------------------------------------------------------------------
void TMyStringList::MySaveToFile()
{
    MySaveToFileShareMode();
}
//------------------------------------------------------------------------------
void TMyStringList::MySaveToFileShareMode()
{
    AnsiString Str;
    AnsiString sPathName_Same;
    AnsiString sFileName_Same;
    GetTimeInfo();

    if(HTAutoSave==false || MyList==NULL || MyList->Count==0)                   //沒資料就不用存檔
        return;

//    Str=StringReplace(Str, "\r\n", "\n", TReplaceFlags()<<rfReplaceAll);

    if(bHanaTrayMap)                                                            //Steven 20250414 : HANA ART Function
    {
        if(fSCKART->palLotNumber->Caption=="" ||
           fSCKART->pnlProcessCode->Caption=="")
            return;

        sPathName_Same.sprintf("D:\\HT9045_Log\\Hana_TrayMap\\%04d\\%s", SystemYear, fSCKART->palLotNumber->Caption);
        MyForceDirectories(sPathName_Same, "TMyStringList::MySaveToFile_2");
        sLastPathName=sPathName_Same;

        sFileName_Same.sprintf("%s_%s_%s_%s_%s_%s.txt", IniConfig.sGPIBMachineID,
                                                        fSCKART->pnlProcessCode->Caption,
                                                        fSCKART->palLotNumber->Caption,
                                                        HTFileName,
                                                        RunInfo.LotStartTime,
                                                        fSCKART->palTestCnt->Caption);
        sLastNameNoPath=sFileName_Same;
        sLastFileName=FileInfo().PathCombin(sPathName_Same, sLastNameNoPath);

        if(FileExists(sLastFileName)==false && HTFirstRow!="")
        {
            Str=HTFirstRow+"\r\n"+MyList->Text;
        }
        else
        {
            Str=MyList->Text;
        }

        HANDLE hFile = CreateFile(
            sLastFileName.c_str(),         // 檔案名稱
            GENERIC_WRITE,                  // 寫入權限
            FILE_SHARE_READ,                // 允許其他程式讀取
            NULL,                           // 安全性屬性
            OPEN_ALWAYS,                    // 如果檔案不存在則建立，已存在則開啟
            FILE_ATTRIBUTE_NORMAL,          // 一般檔案屬性
            NULL                            // 無模板檔案
        );

        if(hFile!=INVALID_HANDLE_VALUE)                                         //JimmyChiu : 20250401 : 變更開檔案方式
        {
            SetFilePointer(hFile, 0, NULL, FILE_END);                           // 移動到檔案結尾，模擬追加模式
            DWORD bytesWritten;
            WriteFile(hFile, Str.c_str(), Str.Length(), &bytesWritten, NULL);
            CloseHandle(hFile);                                                 // 寫入完成後立即關閉
        }
    }
    else
    {
        Str=GetFileName();                                                      //Steven 20250809 : 修正event log上傳
        if(sLastFileName!="" &&
           sLastFileName!=Str)
        {
            sPrevFileName       =sLastFileName;                                 //Steven 20250809 : 紀錄前一個的檔名
            bChangeFile         =true;
        }
        sLastFileName=Str;

        if(FileExists(sLastFileName)==false && HTFirstRow!="")
        {
            Str=HTFirstRow+"\r\n"+MyList->Text;
        }
        else
        {
            Str=MyList->Text;
        }

        HANDLE hFile = CreateFile(
            sLastFileName.c_str(),       // 檔案名稱
            GENERIC_WRITE,               // 寫入權限
            FILE_SHARE_READ,             // 允許其他程式讀取
            NULL,                        // 安全性屬性
            OPEN_ALWAYS,                 // 如果檔案不存在則建立，已存在則開啟
            FILE_ATTRIBUTE_NORMAL,       // 一般檔案屬性
            NULL                         // 無模板檔案
        );

        if(hFile!=INVALID_HANDLE_VALUE)                                         //JimmyChiu : 20250401 : 變更開檔案方式
        {
            SetFilePointer(hFile, 0, NULL, FILE_END);                           // 移動到檔案結尾，模擬追加模式
            DWORD bytesWritten;
            WriteFile(hFile, Str.c_str(), Str.Length(), &bytesWritten, NULL);
            CloseHandle(hFile);                                                 // 寫入完成後立即關閉
        }

        if(HTSaveSameFolder==true &&                                            //KaiChen 20180322 ：矽格-湖口 新增 將 Event Log 放在同個資料夾
           HTSaveFixedFile==false)
        {
            sPathName_Same.sprintf("%s\\AllEventLog", HTPath);
            MyForceDirectories(sPathName_Same, "TMyStringList::MySaveToFile_1");

            if(HTSaveType==TByMaxLineCount)
                sFileName_Same.sprintf("%s\\%s_%04d%02d%02d %02d%02d%02d.csv", sPathName_Same, HTFileName, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
            else
                sFileName_Same.sprintf("%s\\%s_%04d%02d%02d.csv", sPathName_Same, HTFileName, SystemYear, SystemMonth, SystemDate);

            if(FileExists(sFileName_Same)==false && HTFirstRow!="")
                Str=HTFirstRow+"\r\n"+MyList->Text;
            else
                Str=MyList->Text;

            Str=StringReplace(Str, "\r\n", "\n", TReplaceFlags()<<rfReplaceAll);
            hFile = CreateFile(
                sFileName_Same.c_str(),
                GENERIC_WRITE,
                FILE_SHARE_READ,
                NULL,
                OPEN_ALWAYS,
                FILE_ATTRIBUTE_NORMAL,
                NULL
            );

            if (hFile != INVALID_HANDLE_VALUE)                                      //JimmyChiu : 20250401 : 變更開檔案方式
            {
                SetFilePointer(hFile, 0, NULL, FILE_END);
                DWORD bytesWritten;
                WriteFile(hFile, Str.c_str(), Str.Length(), &bytesWritten, NULL);
                CloseHandle(hFile);
            }
        }

        if(HTSaveByLotID==true &&                                               //KaiChen 20181121 ：矽格-北興 Save Event Log by Lot ID
           HTSaveFixedFile==false)
        {
            if(HTLotID!="")
            {
//                sPathName_Lot.sprintf("%s\\ByLotID\\%04d\\%02d\\%02d", HTPath, SystemYear, SystemMonth, SystemDate);
//                MyForceDirectories(sPathName_Lot, "TMyStringList::MySaveToFile_2");
//                sFileName_Lot.sprintf("%s\\%s_%s.csv", sPathName_Lot, HTLotID, HTLotStartTime);

                if(FileExists(sLotFileName)==false && HTFirstRow!="")           //Steven 20250809 : 修正event log上傳
                    Str=HTFirstRow+"\r\n"+MyList->Text;
                else
                    Str=MyList->Text;

                Str=StringReplace(Str, "\r\n", "\n", TReplaceFlags()<<rfReplaceAll);
                hFile = CreateFile(
                    sLotFileName.c_str(),
                    GENERIC_WRITE,
                    FILE_SHARE_READ,
                    NULL,
                    OPEN_ALWAYS,
                    FILE_ATTRIBUTE_NORMAL,
                    NULL
                );

                if(hFile!=INVALID_HANDLE_VALUE)                                 //JimmyChiu : 20250401 : 變更開檔案方式
                {
                    SetFilePointer(hFile, 0, NULL, FILE_END);
                    DWORD bytesWritten;
                    WriteFile(hFile, Str.c_str(), Str.Length(), &bytesWritten, NULL);
                    CloseHandle(hFile);
                }
            }
        }
    }
    MyList->Clear();
}
//------------------------------------------------------------------------------
void TMyStringList::MySaveFileByFileNameAndType(AnsiString asFileName, AnsiString asLotId, AnsiString asFileType) //RogerYang 20170406 (Steven)
{
    AnsiString sPathName;
    AnsiString sFileName;
    AnsiString Str;
    FILE *pFile;
    GetTimeInfo();

    if(HTAutoSave==false || MyList->Count==0)
        return;

    sPathName.sprintf("D:\\HT9045_Log\\JamAlarmLogTxt\\%s", asLotId);
    MyForceDirectories(sPathName, "TMyStringList::MySaveFileByFileNameAndType");
    sFileName.sprintf("%s\\%s_%04d%02d%02d_%s_%s.csv",
                        sPathName, asFileName,
                        SystemYear, SystemMonth, SystemDate,
                        asLotId,                                                //Sam 20170428 (wei) 帶入 LotID
                        asFileType);                                            //Sam 20171215 (wei) : 檔名尾巴增加JAM/INPUT

    if(FileExists(sFileName)==false && HTFirstRow!="")
    {
        Str=HTFirstRow+"\r\n"+MyList->Text;
    }
    else
    {
        Str=MyList->Text;
    }

    Str=StringReplace(Str, "\r\n", "\n", TReplaceFlags()<<rfReplaceAll);
    pFile=fopen(sFileName.c_str(), "a");
    if(pFile!=NULL)
    {
        fputs(Str.c_str(), pFile);
        fclose(pFile);
    }
    pFile=NULL;
    MyList->Clear();
}
//------------------------------------------------------------------------------
void TMyStringList::MySaveFileByFileName(AnsiString asPathName, AnsiString asFileName)   //Sam 20170516 (wei) 力成 SocketID Count 保存
{
    AnsiString Str;
    FILE *pFile;

    MyForceDirectories(asPathName, "TMyStringList::MySaveFileByFileName");
    asFileName.sprintf("%s\\%s", asPathName, asFileName);

    if(FileExists(asFileName)==false && HTFirstRow!="")
    {
        Str=HTFirstRow+"\r\n"+MyList->Text;
    }
    else
    {
        Str=MyList->Text;
    }

    Str=StringReplace(Str, "\r\n", "\n", TReplaceFlags()<<rfReplaceAll);
    pFile=fopen(asFileName.c_str(), "a");
    if(pFile!=NULL)
    {
        fputs(Str.c_str(), pFile);
        fclose(pFile);
    }
    pFile=NULL;
    MyList->Clear();
}
//------------------------------------------------------------------------------
AnsiString TMyStringList::GetFileName()
{
    AnsiString sPathName;
    AnsiString sFileName="";
    AnsiString Str;
    int iHour;
    GetTimeInfo();

    if(HTPath=="")
        Path="D:\\HandlerLog";

    if(FileName=="")
        FileName="";

    if(HTSaveFixedFile)
    {
        sPathName=HTPath;
    }
    else if(bFilePathWithDate)                                                  //Steven 20210623 : 預設存檔要有日期當資料夾
    {
        if(HTSaveType>=TByMonth)                                                //使用年月存檔的話,就By年分類
        {
            sPathName.sprintf("%s\\%04d", HTPath, SystemYear);
        }
        else if(HTSaveType>=TByDay)                                             //使用每日存檔的話,就By月分類
        {
            sPathName.sprintf("%s\\%04d\\%02d", HTPath, SystemYear, SystemMonth);
        }
        else                                                                    //其他就每年存成365個資料夾
        {
            if(IniConfig.iN10UploadProductMethod==0)                            //JerryYang 20190131 上傳production log可選擇00:00 or 08:00
            {
                sPathName.sprintf("%s\\%04d\\%02d\\%02d", HTPath, SystemYear, SystemMonth, SystemDate);
            }
            else
            {
                if(SystemHour<8)
                {
                    GetYesterdayInfo();
                    sPathName.sprintf("%s\\%04d\\%02d\\%02d", HTPath, SystemYearYesterday, SystemMonthYesterday, SystemDateYesterday);
                }
                else
                {
                    sPathName.sprintf("%s\\%04d\\%02d\\%02d", HTPath, SystemYear, SystemMonth, SystemDate);
                }
            }
        }
    }
    else
    {
        sPathName=HTPath;
    }

    MyForceDirectories(sPathName, "TMyStringList::GetFileName");
    if(HTSaveFixedFile)
    {
        sFileName.sprintf("%s\\%s.csv", sPathName, HTFileName);
    }
    else if(HTSaveType==TByMaxLineCount)
    {
        sFileName.sprintf("%s\\%s_%04d%02d%02d %02d%02d%02d.csv", sPathName, HTFileName, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    }
    else
    {
        if(HTSaveType==TByHour)
        {
            if(bUseFTRT)                                                        //Steven 20220414 : JSCK OEE檔名要加上FT RT
            {
                if(LastSet.iRunStartMode==rsmContinuRetest ||
                   LastSet.iRunStartMode==rsmCInitialRetest)
                    sFileName.sprintf("%s\\%s_%04d%02d%02d %02d_RT.csv", sPathName, HTFileName, SystemYear, SystemMonth, SystemDate, SystemHour);
                else
                    sFileName.sprintf("%s\\%s_%04d%02d%02d %02d_FT.csv", sPathName, HTFileName, SystemYear, SystemMonth, SystemDate, SystemHour);
            }
            else                                                                //Steven 20220513 : For ChipMos FTP
            {
                sFileName.sprintf("%s\\%s_%04d%02d%02d %02d.csv", sPathName, HTFileName, SystemYear, SystemMonth, SystemDate, SystemHour);
            }
        }
        else if(HTSaveType==TBy2Hour)
        {
            iHour=SystemHour-SystemHour%2;
            sFileName.sprintf("%s\\%s_%04d%02d%02d %02d.csv", sPathName, HTFileName, SystemYear, SystemMonth, SystemDate, iHour);
        }
        else if(HTSaveType==TBy4Hour)
        {
            iHour=SystemHour-SystemHour%4;
            sFileName.sprintf("%s\\%s_%04d%02d%02d %02d.csv", sPathName, HTFileName, SystemYear, SystemMonth, SystemDate, iHour);
        }
        else if(HTSaveType==TBy6Hour)
        {
            iHour=SystemHour-SystemHour%6;
            sFileName.sprintf("%s\\%s_%04d%02d%02d %02d.csv", sPathName, HTFileName, SystemYear, SystemMonth, SystemDate, iHour);
        }
        else if(HTSaveType==TBy8Hour)
        {
            iHour=SystemHour-SystemHour%8;
            sFileName.sprintf("%s\\%s_%04d%02d%02d %02d.csv", sPathName, HTFileName, SystemYear, SystemMonth, SystemDate, iHour);
        }
        else if(HTSaveType==TBy12Hour)
        {
            if(IniConfig.iN10UploadProductMethod==0)                            //JerryYang 20190131 上傳production log可選擇00:00 or 08:00
            {
                iHour=SystemHour-SystemHour%12;
                sFileName.sprintf("%s\\%s_%04d%02d%02d %02d.csv", sPathName, HTFileName, SystemYear, SystemMonth, SystemDate, iHour);
            }
            else
            {
                if(SystemHour<8)
                {
                    GetYesterdayInfo();
                    sFileName.sprintf("%s\\%s_%04d%02d%02d%02d00.csv", sPathName, HTFileName, SystemYearYesterday, SystemMonthYesterday, SystemDateYesterday, 20);
                }
                else if(SystemHour>=20)
                {
                    sFileName.sprintf("%s\\%s_%04d%02d%02d%02d00.csv", sPathName, HTFileName, SystemYear, SystemMonth, SystemDate, 20);
                }
                else
                {
                    sFileName.sprintf("%s\\%s_%04d%02d%02d%02d00.csv", sPathName, HTFileName, SystemYear, SystemMonth, SystemDate, 8);
                }
            }
        }
        else if(HTSaveType==TByDay)
        {
            if(CUSTOMER_CODE==CC_CYPRESS &&
               LastSet.iTester==OFF_LINE)                                       //JerryYang 20230717 : Cypress要求區分Online/Offline
            {
                sFileName.sprintf("%s\\%s_%04d%02d%02d_TesterOffline.csv", sPathName, HTFileName, SystemYear, SystemMonth, SystemDate);
            }
            else
            {
                if(bUseFTRT)
                {
                    if(LastSet.iRunStartMode==rsmContinuRetest ||
                       LastSet.iRunStartMode==rsmCInitialRetest)
                        sFileName.sprintf("%s\\%s_%04d%02d%02d_RT.csv", sPathName, HTFileName, SystemYear, SystemMonth, SystemDate);
                    else
                        sFileName.sprintf("%s\\%s_%04d%02d%02d_FT.csv", sPathName, HTFileName, SystemYear, SystemMonth, SystemDate);
                }
                else
                {
                    sFileName.sprintf("%s\\%s_%04d%02d%02d.csv", sPathName, HTFileName, SystemYear, SystemMonth, SystemDate);
                }
            }
        }
        else if(HTSaveType==TByMonth)
        {
            sFileName.sprintf("%s\\%s_%04d%02d.csv", sPathName, HTFileName, SystemYear, SystemMonth);
        }
        else if(HTSaveType==TByMin)
        {
            if(bUseFTRT)
            {
                if(LastSet.iRunStartMode==rsmContinuRetest ||
                   LastSet.iRunStartMode==rsmCInitialRetest)
                    sFileName.sprintf("%s\\%s_%04d%02d%02d_%02d%02d_RT.TXT", sPathName, HTFileName, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin);
                else
                    sFileName.sprintf("%s\\%s_%04d%02d%02d_%02d%02d_FT.TXT", sPathName, HTFileName, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin);
            }
            else
            {
                sFileName.sprintf("%s\\%s_%04d%02d%02d_%02d%02d.TXT", sPathName, HTFileName, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin);
            }
        }
        else //if(HTSaveType==TByYear)
        {
            sFileName.sprintf("%s\\%s_%04d.csv", sPathName, HTFileName, SystemYear);
        }
    }

    return sFileName;
}
//---------------------------------------------------------------------------
int TMyStringList::GetLastLine()                                                //Steven 20191016 : 取得目前檔案的行數
{
    AnsiString sFileName=GetFileName();
    int iCount=0;
    GetTimeInfo();
    if(FileExists(sFileName))                                                   //Steven 20200221 : 修正沒有檔案就不要讀檔
    {
        HANDLE hFile = CreateFile(sFileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile!=INVALID_HANDLE_VALUE)                                        //JimmyChiu : 20250401 : 變更開檔案方式
        {
            CloseHandle(hFile);
            std::ifstream file(sFileName.c_str());
            if (file.is_open())
            {
                std::string line;
                while (std::getline(file, line))
                {
                    iCount++;
                }
                file.close();
            } else
            {
                iCount=0;
            }
        }
    }
    sLastFileName=sFileName;
    return iCount;
}
//---------------------------------------------------------------------------
void SaveTryCatchLog(AnsiString asMessage, AnsiString Func)                     //KaiChen 20200706 ：增加 Try Catch 抓問題
{
    AnsiString sPathName, sFileName, sMegTime, asLog;

    sPathName.sprintf("D:\\HT9045_Log\\EventLogTxt\\TryCatchLog\\%04d", SystemYear);
    sFileName.sprintf("%s\\TryCatchLog.txt", sPathName);
    MyForceDirectories(sPathName);
    sMegTime.sprintf("%04d-%02d-%02d %02d:%02d:%02d %03d",
                        SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec);

    asLog=sMegTime+", "+Func+", "+asMessage;
    WriteDataToFile(sFileName.c_str() , asLog.c_str());
}
//---------------------------------------------------------------------------
void TMyStringList::MyInsertToFile(AnsiString Msg, int iCount)
{
//    if(HTAutoSave==false || MyList->Count==0)                                   //沒資料就不用存檔
//        return;

    TStringList *File;
    File=new TStringList();

    try                                                                         //KaiChen 20200706 ：增加 Try Catch 抓問題
    {
        if(FileExists(sLastFileName)==false)
        {
            sLastFileName=GetFileName();
        }
        else                                                                    //Steven 20200221 : 修正沒有檔案就不要讀檔
        {
            HANDLE hFile = CreateFile(
                sLastFileName.c_str(),       // 檔案名稱
                GENERIC_WRITE,               // 寫入權限
                FILE_SHARE_READ,             // 允許其他程式讀取
                NULL,                        // 安全性屬性
                OPEN_ALWAYS,                 // 如果檔案不存在則建立，已存在則開啟
                FILE_ATTRIBUTE_NORMAL,       // 一般檔案屬性
                NULL                         // 無模板檔案
            );

            if(hFile!=INVALID_HANDLE_VALUE)
            {
                CloseHandle(hFile);
                File->LoadFromFile(sLastFileName);
//                Str=MyList->Text.SubString(0, MyList->Text.Length()-2);       //Steven 20251107 : 修正event log存檔
//                if(IniConfig.bSPILFunction)                                     //JerryYang 20250428 : fix SPIL event log
//                {
//                }
//                else
//                {
//                    Msg.Insert(' ', 12);
//                    Msg.Insert(' ', 26);
//                }

                if(FileExists(sLastFileName) && File->Count>iCount && iCount>0) //Steven 20200215 : 加上保護機制
                    File->Insert(iCount, Msg);
                else
                    File->Add(Msg);
                File->SaveToFile(sLastFileName);                                //Steven 20191107 : 紀錄現在的檔名
            }
        }
    }
    catch(...)
    {
        SaveTryCatchLog(slEventLog->Text, AnsiString("ShowErrorMessage_MyInsertToFile"));
    }
    File->Clear();
    MyList->Clear();
    delete File;
}
//---------------------------------------------------------------------------
void TMyStringList::MySaveSGJamCountToFile(bool bDelete, bool bflag)            //KaiChen 20200618 ：矽格，增加Jam統計頁面
{
    AnsiString Str;
    FILE *pFile;
    Word Year,Month,Date;
    GetTimeInfo();
    GetYesterdayInfo();

    if(HTAutoSave==false && bDelete==false)                                     //沒資料就不用存檔
        return;

    if(HTPath=="")
        Path="D:\\HandlerLog";

    AnsiString sPathName;
    AnsiString sFileName;

    if(bflag)
    {
        Year    =SystemYearYesterday;
        Month   =SystemMonthYesterday;
        Date    =SystemDateYesterday;
    }
    else
    {
        Year    =SystemYear;
        Month   =SystemMonth;
        Date    =SystemDate;
    }

    sPathName.sprintf("%s\\SGJamCount\\%04d\\%02d", HTPath, Year, Month);
    MyForceDirectories(sPathName, "TMyStringList::MySaveSGJamCountToFile");

    if(HTSaveType==TByMaxLineCount)
        sFileName.sprintf("%s\\%s_%04d%02d%02d %02d%02d%02d.csv", sPathName, HTFileName, Year, Month, Date, SystemHour, SystemMin, SystemSec);
    else
        sFileName.sprintf("%s\\%s_%04d%02d%02d_RawData.csv", sPathName, HTFileName, Year, Month, Date);

    if(bDelete)
    {
        DeleteFile(sFileName);
    }
    else
    {
        if(FileExists(sFileName)==false && HTFirstRow!="")
        {
            Str=HTFirstRow+"\r\n"+MyList->Text;
        }
        else
        {
            Str=MyList->Text;
        }

        Str=StringReplace(Str, "\r\n", "\n", TReplaceFlags()<<rfReplaceAll);
        pFile=fopen(sFileName.c_str(), "a");
        if(pFile!=NULL)
        {
            fputs(Str.c_str(), pFile);
            fclose(pFile);
        }
    }
    MyList->Clear();
}
//------------------------------------------------------------------------------
void TMyStringList::MyInsert2DMappingToFile(AnsiString s2DID, int iPlaceToUnload)  //JerryYang 20230322 : add 2D mapping result
{
    if(TestIF_File.bEnableBarCode    ==false &&                                 //Nickliu 20230810 retrun for debug
       TestIF_File.bSortingBy2DIDList==false)
    {
        return ;
    }

    AnsiString sTestCate;                                                       //RogerYang 20260120 : Add for RF360 2DID sorting
    if(CUSTOMER_CODE==CC_QUALCOMM)
    {
        if(fLotInfo->edtSysLotID->Text=="")
        {
            return;
        }
        else
        {
            if(iPlaceToUnload>=0)
                sTestCate=AnsiString(iPlaceToUnload);
            else
                sTestCate="E";
        }
    }
    else if(fLotInfo->edtSysLotID->Text =="" ||
       fLotInfo->edtCusLotID->Text =="" ||
       fLotInfo->edtCusDevGrp->Text=="")
    {
        return;
    }

    AnsiString Str, str, str2="", str3="", sFileName;

    sFileName.sprintf("D:\\HT9045_Log\\2D_MappingResult\\%s_%s_%s_VS_Result.csv", fLotInfo->edtSysLotID->Text, fLotInfo->edtCusLotID->Text, fLotInfo->edtCusDevGrp->Text);

    int iCol=0;
    TStringList *File, *list2D;
    File=new TStringList();
    list2D=new TStringList();

    MyForceDirectories("D:\\HT9045_Log\\2D_MappingResult\\", "TMyStringList::MyInsert2DMappingToFile");

    if(FileExists(sFileName))
    {
        File->LoadFromFile(sFileName);
        for(int i=0; i<File->Count; i++)
        {
            AnsiString s,s1,s2,s3,s4;
            int iPos1,iPos2;
            s=File->Strings[i];

            iPos1=s.Pos(" ");
            iPos2=s.Pos(",");
            if(iPos1>0 && iPos2>0 && iPos1<iPos2)                               //表示2D有空格
            {
                s1=s.SubString(1, iPos2-1);                                     //2D
                s3=s.SubString(iPos2+1, s.Length());                            //2D
                s2=StringReplace(s1, " ", "_", TReplaceFlags()<<rfReplaceAll);
                s4=s2+","+s3;
                list2D->CommaText=s4;
                list2D->Strings[0]=s1;
            }
            else
            {
                list2D->CommaText=File->Strings[i];
            }

            if(list2D->Strings[0]==s2DID)
            {
                iCol=i;

                if(CUSTOMER_CODE==CC_QUALCOMM)                                  //RogerYang 20260121 : Add for RF360 2DID sorting 塞bin結果
                {
                    list2D->Strings[10]=sTestCate;
                    break;

                }

                if(fLotInfo->cbRunMode->Text=="VS")
                {
                    if(list2D->Count>=11)
                    {
                        if(iPlaceToUnload>=0)
                        {
                            list2D->Strings[6]=iPlaceToUnload+1;
                        }
                        else
                        {
                            list2D->Strings[5]="V";
                        }
                    }
                }
                else if(fLotInfo->cbRunMode->Text=="RC1")
                {
                    if(list2D->Count>=11)
                    {
                        if(iPlaceToUnload>=0)
                        {
                            list2D->Strings[8]=iPlaceToUnload+1;
                        }
                        else
                        {
                            list2D->Strings[7]="V";
                        }
                    }
                }
                else
                {
                    if(list2D->Count>=11)
                    {
                        if(iPlaceToUnload>=0)
                        {
                            list2D->Strings[10]=iPlaceToUnload+1;
                        }
                        else
                        {
                            list2D->Strings[9]="V";
                        }
                    }
                }
                break;
            }
        }
        str3="";
        for(int i=0; i<list2D->Count; i++)
        {
            str=list2D->Strings[i];
            if(i==0)
            {
                str=StringReplace(str, '"', "", TReplaceFlags()<<rfReplaceAll);
                str3=str;
            }
            else
            {
                str3=str3+","+str;
            }
        }
    }

    if(FileExists(sFileName))
    {
        if(iCol>0)
        {
            File->Delete(iCol);
            File->Insert(iCol, str3);
            File->SaveToFile(sFileName);
        }
        else if(CUSTOMER_CODE==CC_QUALCOMM)                                     //RogerYang 20260121 : Add for RF360 2DID sorting 沒有出現在LIST，需新增資料
        {
            File->Add(s2DID+",,,,,,,,,,"+sTestCate);
            File->SaveToFile(sFileName);
        }
    }
    else
    {
        File->SaveToFile(sFileName);
    }

    File->Clear();
    MyList->Clear();
    delete File;

    list2D->Clear();
    delete list2D;
}
//------------------------------------------------------------------------------
