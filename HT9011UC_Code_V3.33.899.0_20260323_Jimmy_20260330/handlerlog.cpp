#include "MachineDefine.h"
#pragma hdrstop
//==============================================================================
// Version & Date  :V1.02 2010/07/08
// Programmer      :Landam
// Objective     :Opration Log
//==============================================================================

#include "handlerlog.h"
#include "cprod.h"
#include "cMyDB.h"
#include "cmydef.h"
#include "cSetUp.h"
#include "Main.h"
#include "atester.h"
#include "LastSet.h"
#include "uLotInfo.h"
#include "common.h"

AnsiString Space_Num(AnsiString S, int n);
//------------------------------------------------------------------------------
_fastcall TMyLog::TMyLog()
{
    fName="";
    sUser="";
    sFolder="";
    Delimiter="\t";
}
//------------------------------------------------------------------------------
void TMyLog::Save_Log()
{
    TDateTime tt= Now();
    Word SystemYear, SystemMonth, SystemDate;
    DecodeDate(tt, SystemYear, SystemMonth, SystemDate);
    AnsiString str1, str2;

    str1.sprintf("%04d_%02d\\", SystemYear, SystemMonth);
    str2.sprintf("log_%04d_%02d_%02d.ini", SystemYear, SystemMonth, SystemDate);

    FILE * pFile;
    AnsiString fp="", s_cFolder;
    if(sFolder=="")
        sFolder.sprintf("%sLog\\", ExtractFilePath(Application->ExeName));

    IncludeTrailingPathDelimiter(sFolder);
    s_cFolder=sFolder+str1;
    MyForceDirectories(s_cFolder);

    fp=s_cFolder+str2;
    pFile=fopen(fp.c_str() ,"a");
    if(pFile!=NULL)
    {
        AnsiString tmps;
        for(unsigned i=0; i<loglist.size(); ++i)
        {
            tmps=loglist[i]+"\n";
            fputs(tmps.c_str(), pFile);
            if(i==loglist.size()-1)
                fputs("\n", pFile);

        }
        fclose(pFile);
    }

    //----------------clear_log folder  ----------------------------------
    if(SystemMonth<=2)
    {
        SystemMonth=SystemMonth + 10;
        SystemYear--;
    }
    else
    {
        SystemMonth=SystemMonth-2;
    }

    str1.sprintf("%04d_%02d", SystemYear, SystemMonth);
    s_cFolder=sFolder+str1;

    if((DirectoryExists(s_cFolder)))
    {
        int len = _tcslen(s_cFolder.c_str());
        TCHAR *pszFrom = new TCHAR[len+2];
        _tcscpy(pszFrom, s_cFolder.c_str());
        pszFrom[len] = 0;
        pszFrom[len+1] = 0;

        SHFILEOPSTRUCT      opStr;

        opStr.hwnd          = NULL;
        opStr.pTo           = NULL;
        opStr.wFunc         = FO_DELETE;
        opStr.fFlags        = FOF_SILENT | FOF_NOCONFIRMATION;
        opStr.pFrom         = pszFrom;
        SHFileOperation(&opStr);
        delete [] pszFrom;
    }
    //-------------end of clear log---------------------------------------------
}
//------------------------------------------------------------------------------
void TMyLog::Save_SiteStatusLog()                                               //JerryYang 20160617 記錄開關site的狀態
{
    TDateTime tt=Now();
    Word SystemYear, SystemMonth, SystemDate;
    DecodeDate(tt, SystemYear, SystemMonth, SystemDate);
    AnsiString str1, str2;

    str1.sprintf("%04d_%02d\\",SystemYear, SystemMonth);
    str2.sprintf("log_%04d_%02d_%02d.ini", SystemYear, SystemMonth, SystemDate);

    FILE * pFile;
    AnsiString fp="", s_cFolder;
    if(sFolder=="")
        sFolder.sprintf("%s", "D:\\HT9045_Log\\ChangeLog\\");

    IncludeTrailingPathDelimiter(sFolder);
    s_cFolder=sFolder+str1;
    MyForceDirectories(s_cFolder);

    fp=s_cFolder+str2;
    pFile=fopen(fp.c_str(), "a");
    if(pFile!=NULL)
    {
        AnsiString tmps, tmps1, tmps2, tmps3, tmps4, Atmps, Atmps1, Atmps2, Atmps3;
        AnsiString asRecordBuffer;

        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                if(i==TestSocket.iShtRow-1 && j==TestSocket.iShtCol-1)
                {
                    tmps1=tmps1+IntToStr(int(LastSet.bUseTestSocket[0][i][j]));
                }
                else
                {
                    tmps1=tmps1+IntToStr(int(LastSet.bUseTestSocket[0][i][j]))+",";
                }

                if(i==TestSocket.iShtRow-1 && j==TestSocket.iShtCol-1)
                    tmps2=tmps2+IntToStr(int(LastSet.bUseTestSocket[1][i][j]));
                else
                    tmps2=tmps2+IntToStr(int(LastSet.bUseTestSocket[1][i][j]))+",";

                tmps3=tmps3+IntToStr(int(iCloseSiteMap[0][i][j]))+",";

                Atmps1=Atmps1+IntToStr(int(TestIF_File.iSiteMap[i][j]))+",";
                Atmps2=Atmps2+IntToStr(int(iRecordSiteMapOrder[i][j]))+",";     //kevin 20161003

                if(iRecordSiteMapOrder[i][j]!=TestIF_File.iSiteMap[i][j])       //Ifor 20161220 (Steven) add Site Mapping change Even
                {
                    asRecordBuffer.sprintf("Site %s Value Change %s -> %s", IndexSuckName[i][j], IntToStr(int(iRecordSiteMapOrder[i][j])), IntToStr(int(TestIF_File.iSiteMap[i][j])));
                    NewRecordProcess("MES2105", "Site Mapping", asRecordBuffer);//Ifor 20170106 需再確認MES號碼
                    iRecordSiteMapOrder[i][j] = TestIF_File.iSiteMap[i][j];     //Ifor 20161220 Site Mapping 變更後需存檔避免紀錄錯誤
                }
            }
        }
        tmps.sprintf("\n(Null)\t\t\tfSiteStatusArm1\t\t\t%s\t\t%s\n(Null)\t\t\tfSiteStatusArm2\t\t\t%s\t\t%s\n", tmps1, AnsiString(Now()), tmps2, AnsiString(Now()));
        Atmps.sprintf("\n(Null)\t\t\tTest Site Assign ->\t\%s\t\t%s\n", Atmps1, AnsiString(Now()));
        tmps4.sprintf("CloseOpenSite : %s  -> %s\n", tmps3, tmps1);

        fputs(tmps.c_str(), pFile);
        fputs(Atmps.c_str(), pFile);
        fclose(pFile);
        Atmps3.sprintf("\t\%s\ -> %s\n", Atmps2, Atmps1);

        if(fSetup->fShow)
        {
            if(Atmps2!=Atmps1)                                                  //Ifor 20161116 Site Map 未修改 不紀錄
                NewRecordProcess("MES2106", "Test Site Assign", Atmps3);

            if(iOldShuttleMode!=TestIF_File.iShuttleMode ||                     //Ifor 20161220 (Steven) add Head Select Mode Even
               iOldSelShuttle!=TestIF_File.iShuttle_Sel)
            {
                if(iOldShuttleMode==TestIF_File.iShuttleMode &&
                   iOldSelShuttle!=TestIF_File.iShuttle_Sel)
                {
                    if(TestIF_File.iShuttle_Sel==0)
                        asRecordBuffer.sprintf("Shuttle 2 only -> Shuttle 1 only");
                    else
                        asRecordBuffer.sprintf("Shuttle 1 only -> Shuttle 2 only");
                }
                else if(iOldShuttleMode!=TestIF_File.iShuttleMode)
                {
                    if(TestIF_File.iShuttleMode==0)
                    {
                        if(iOldSelShuttle==0)
                            asRecordBuffer.sprintf("Shuttle 1 only -> Normal");
                        else
                            asRecordBuffer.sprintf("Shuttle 2 only -> Normal");
                    }
                    else
                    {
                        if(TestIF_File.iShuttle_Sel==0)
                            asRecordBuffer.sprintf("Normal -> Shuttle 1 only");
                        else
                            asRecordBuffer.sprintf("Normal -> Shuttle 2 only");
                    }
                }

                NewRecordProcess("MES2102", "Head Select Value", asRecordBuffer);
                iOldShuttleMode= TestIF_File.iShuttleMode;
                iOldSelShuttle= TestIF_File.iShuttle_Sel;
            }

            if(iUseSuckModeBackup!=TestIF_File.iUseSuckMode)                    //Ifor 20161116 Add Use Suck Mode select Change Even Log 未修改不紀錄
            {
                if(TestIF_File.iTestMode==DualSite ||
                   TestIF_File.iTestMode==QualSite1X4 ||
                   TestIF_File.iTestMode==QualSite2X2)                          //Ifor 20161116 add 判斷是否使用Arm Suck Mode
                {                                                               //Ifor 20161220 (Steven) Use Pick Unit Count Change Even Modify
                    asRecordBuffer.sprintf("Use %s pick unit Pick -> Use %s pick unit Pick", IntToStr(iUseSuckModeBackup), IntToStr(TestIF_File.iUseSuckMode));
                    NewRecordProcess("MES2103", "Use Pick Unit Count Value Change", asRecordBuffer);
                    iUseSuckModeBackup=TestIF_File.iUseSuckMode;
                }
            }

            if(asHandlingMode!=fSetup->Panel1->Caption)                         //Ifor 20161117 add Handler Mode Change Even Log 未修改不紀錄
            {                                                                   //Ifor 20161220 (Steven) Socket Count Change Even Modify
                asRecordBuffer.sprintf("%s -> %s", asHandlingMode, fSetup->Panel1->Caption);
                NewRecordProcess("MES2104", "Socket Count Value Change ", asRecordBuffer);
                asHandlingMode=fSetup->Panel1->Caption;
            }
        }
        else
        {
            //Ifor 20161219 (Steven) 開關 Site Even Log改至mtDutOnOffMouseUp 處理
            //==>
            //if(CUSTOMER_CODE != CC_KYEC_LEE)
            //{
            //    if(tmps3!=tmps1)
            //        RecordProcess(tmps4);                                     //kevin 20161003
            //}
            //<==
            //Ifor 20161219 add Kyec 開關Site Even Log
        }
    }

    //----------------clear_log folder  ----------------------------------
    if(SystemMonth<=2)
    {
        SystemMonth=SystemMonth + 10;
        SystemYear--;
    }
    else
    {
        SystemMonth=SystemMonth-2;
    }

    str1.sprintf("%04d%s%02d", SystemYear, "_", SystemMonth);
    s_cFolder= sFolder + str1;

    if((DirectoryExists(s_cFolder)))
    {
        int len = _tcslen(s_cFolder.c_str());
        TCHAR *pszFrom = new TCHAR[len+2];
        _tcscpy(pszFrom, s_cFolder.c_str());
        pszFrom[len] = 0;
        pszFrom[len+1] = 0;

        SHFILEOPSTRUCT      opStr;

        opStr.hwnd          = NULL;
        opStr.pTo           = NULL;
        opStr.wFunc         = FO_DELETE;
        opStr.fFlags        = FOF_SILENT  | FOF_NOCONFIRMATION;
        opStr.pFrom         = pszFrom;
        SHFileOperation(&opStr);
        delete [] pszFrom;
    }
    //-------------end of clear log---------------------------------------------
}
//------------------------------------------------------------------------------
void TMyLog::Save_AnyName(AnsiString ComponentName,AnsiString AData)            //kevin 20160728 記錄狀態
{
    TDateTime tt=Now();
    Word SystemYear, SystemMonth, SystemDate;
    DecodeDate(tt, SystemYear, SystemMonth, SystemDate);
    AnsiString str1, str2;

    str1.sprintf("%04d%s%02d%s", SystemYear, "_", SystemMonth, "\\");
    str2.sprintf("%s%04d%s%02d%s%02d%s", "log_", SystemYear, "_", SystemMonth, "_", SystemDate, ".ini");

    FILE * pFile;
    AnsiString fp="", s_cFolder;
    if(sFolder=="")
        sFolder.sprintf("%s", "D:\\HT9045_Log\\ChangeLog\\");

    IncludeTrailingPathDelimiter(sFolder);
    s_cFolder=sFolder+str1;
    MyForceDirectories(s_cFolder);

    fp=s_cFolder+str2;

    pFile=fopen(fp.c_str(), "a");
    if(pFile!=NULL)
    {
        AnsiString tmps,tmps1,tmps2,Atmps,Atmps1,Atmps2;
        Atmps.sprintf("\n(Null)\t\t\t%s->\t\%s\t\t%s\n", ComponentName, AData, AnsiString(Now()));
        fputs(tmps.c_str(), pFile);
        fputs(Atmps.c_str(), pFile);
        fclose(pFile);
    }

    //----------------clear_log folder  ----------------------------------
    if(SystemMonth<=2)
    {
        SystemMonth=SystemMonth+10;
        SystemYear--;
    }
    else
    {
        SystemMonth=SystemMonth-2;
    }

    str1.sprintf("%04d%s%02d", SystemYear, "_", SystemMonth);
    s_cFolder=sFolder+str1;

    if((DirectoryExists(s_cFolder)))
    {
        int len=_tcslen(s_cFolder.c_str());
        TCHAR *pszFrom=new TCHAR[len+2];
        _tcscpy(pszFrom, s_cFolder.c_str());
        pszFrom[len]=0;
        pszFrom[len+1]=0;

        SHFILEOPSTRUCT      opStr;

        opStr.hwnd          = NULL;
        opStr.pTo           = NULL;
        opStr.wFunc         = FO_DELETE;
        opStr.fFlags        = FOF_SILENT  | FOF_NOCONFIRMATION;
        opStr.pFrom         = pszFrom;
        SHFileOperation(&opStr);
        delete [] pszFrom;
    }
    //-------------end of clear log---------------------------------------------
}
//------------------------------------------------------------------------------
void TMyLog::Find_Parent(TObject *PCtrl)
{
    TForm          *FrmPtr     = dynamic_cast <TForm          *>(PCtrl);
    TWinControl    *pC         = dynamic_cast <TWinControl    *>(PCtrl);
    if(FrmPtr==NULL && pC!=NULL)
        Find_Parent(pC->Parent);
    else if(FrmPtr!=NULL)
        fName=FrmPtr->Name;
}
//------------------------------------------------------------------------------
void TMyLog::Init_Log(TObject *PCtrl, AnsiString tmpUser, AnsiString tmpFolder)
{
    if(tmpFolder!="")
        sFolder=tmpFolder;

    sUser=tmpUser;
    loglist.clear();
    TForm       *FrmPtr     = dynamic_cast <TForm       *>(PCtrl);

    if(FrmPtr!=NULL)
        fName= FrmPtr->Name;
    else
        Find_Parent(PCtrl);

    if(dynamic_cast <TWinControl *>(PCtrl) != NULL)
    {
        Control_Log_Value((TWinControl *)PCtrl,true);
    }
}
//------------------------------------------------------------------------------
void TMyLog::Comp_Log(TObject *PCtrl)
{
    if(dynamic_cast <TWinControl *>(PCtrl) != NULL)
    {
        Control_Log_Value ((TWinControl *)PCtrl,false);
        Save_Log();
    }
}
//------------------------------------------------------------------------------
void TMyLog::Control_Log_Value(TWinControl  *PCtrl, bool bInit)
{
//    TPanel      *PanelPtr   = dynamic_cast <TPanel      *>(PCtrl);
//    TTabSheet   *ShtPtr     = dynamic_cast <TTabSheet   *>(PCtrl);
//    TForm       *FrmPtr     = dynamic_cast <TForm       *>(PCtrl);
//    TGroupBox   *PGroupBox  = dynamic_cast <TGroupBox   *>(PCtrl);
//    TScrollBox  *ScrBox     = dynamic_cast <TScrollBox  *>(PCtrl);
    TRadioGroup *RadGup     = dynamic_cast <TRadioGroup *>(PCtrl);
    if(RadGup!=NULL)
    {
        AnsiString tmps;
        if(RadGup->ItemIndex!=-1)
            tmps=RadGup->Items->Strings[RadGup->ItemIndex];
        else
            tmps=" ";

        if(bInit)
            mapLogValue[fName+"_"+RadGup->Name+"_"+RadGup->Caption]=AnsiString(RadGup->ItemIndex);
        else
            Compare_Diff(fName+"_"+RadGup->Name+"_"+RadGup->Caption, AnsiString(RadGup->ItemIndex), tmps);
    }
    else
    {
        for(int iP=0; iP<PCtrl->ControlCount; iP++)
        {
            TControl *P=PCtrl->Controls[iP];
            if(dynamic_cast <TWinControl *>(P)!=NULL)
            {
                Control_Log_Value((TWinControl *)P, bInit);      // recursive
            }

            //目前只記錄這些元件內容
            TScrollBar      *PScrollBar   = dynamic_cast <TScrollBar    *>(P);
            TTrackBar       *PTrackBar    = dynamic_cast <TTrackBar     *>(P);
            TCheckBox       *PCheckBox    = dynamic_cast <TCheckBox     *>(P);
            TRadioButton    *PRadioButton = dynamic_cast <TRadioButton  *>(P);
            TComboBox       *PTComboBox   = dynamic_cast <TComboBox     *>(P);
            TEdit           *PTEdit       = dynamic_cast <TEdit         *>(P);
            TLabeledEdit    *PTLEdit      = dynamic_cast <TLabeledEdit  *>(P);  //Steven 20200715 : 補上TLabeledEdit

            AnsiString sValue="NULL", sCaption=" ";
            if(PScrollBar!=NULL)
            {
                sValue=AnsiString(PScrollBar->Position);
            }
            else if(PTrackBar!=NULL)
            {
                sValue=AnsiString(PTrackBar->Position);
            }
            else if(PCheckBox!=NULL)
            {
                sCaption=PCheckBox->Caption.Trim();
                sValue=(PCheckBox->Checked?"1":"0");
            }
            else if(PRadioButton!=NULL)
            {
                sCaption=PRadioButton->Caption.Trim();
                sValue=(PRadioButton->Checked?"1":"0");
            }
            else if(PTComboBox!=NULL)
            {
                sValue=AnsiString (PTComboBox->Text);
            }
            else if(PTEdit!=NULL)
            {
                sValue=PTEdit->Text;
            }
            else if(PTLEdit!=NULL)
            {
                sValue=PTLEdit->Text;
            }

            if(sValue!="NULL")
            {
               if(bInit)
                   mapLogValue[fName+"_"+P->Name]=sValue;
               else
                   Compare_Diff(fName+"_"+P->Name, sValue, sCaption);
            }
        }
    }
}
//------------------------------------------------------------------------------
bool TMyLog::Do_Log(TObject *PCtrl, AnsiString tmpUser, AnsiString tmpFolder)
{
    bool bRetrun=false;

        if(tmpFolder!="")
            sFolder=tmpFolder;

        sUser=tmpUser;
        loglist.clear();
        TForm       *FrmPtr     = dynamic_cast <TForm       *>(PCtrl);

        if(FrmPtr!=NULL)
            fName=FrmPtr->Name;
        else
            Find_Parent(PCtrl);

        if(dynamic_cast <TWinControl *>(PCtrl) != NULL)
        {
            Control_Log_Value((TWinControl *)PCtrl, false);
            if(loglist.size()>0)
            {
    //            Save_Log();                                                   //Steven 20170605 (wei) : 減少LOG數量
                SaveEventLog();
                bRetrun=true;
            }
        }
        return bRetrun;
}
//------------------------------------------------------------------------------
void TMyLog::Compare_Diff(AnsiString Sfind, AnsiString Svalue, AnsiString sCaption)
{
    std::map<AnsiString,AnsiString >::iterator mapIter;
    mapIter=mapLogValue.find(Sfind);
    TStringList *SL;                                                            //Steven 20161115 : EventLog存成文字檔
    SL=new TStringList();
    AnsiString Str;

    if(mapIter!=mapLogValue.end())
    {
        if(Svalue!=mapIter->second)
        {
            AnsiString tmps,s1;
            tmps=mapIter->second+" => "+Svalue;
            s1.sprintf("%-16s%s%-26s%s%-22s%s%-22s%s%-32s", sUser, Delimiter, Sfind, Delimiter, tmps, Delimiter, AnsiString(Now()), Delimiter, sCaption);
            loglist.push_back(s1);

            SL->Clear();
            if(IniConfig.bSPILFunction==true)                                   //Steven 20240604 : SPIL格式的event log
            {
                GetTimeInfo();
                Str.sprintf("%04d-%02d-%02d %02d:%02d:%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);

                SL->Add("ChangeLog");                                           //UnitName
                SL->Add("\t");                                                  //AlarmCode
                SL->Add(Str);                                                   //OccurDateTime
                SL->Add("\t");                                                  //Recovery
                SL->Add("\t");                                                  //StopedTime
                SL->Add("\t");                                                  //Duplicate
                SL->Add(Sfind);                                                 //Message
                SL->Add(tmps+", "+sCaption);                                    //ErrPart
                slEventLog->AddTextWithLineNo(SL->CommaText);
            }
            else
            {
                SL->Add("ChangeLog");
                SL->Add("\t");
                SL->Add(sUser);
                SL->Add("\t");
                SL->Add("\t");
                SL->Add(Sfind);
                SL->Add(tmps+", "+sCaption);
                slEventLog->AddTextWithDateTime(SL->CommaText);
            }
            SaveEventLogInfo("210000000", SL->CommaText, 21, " ");
            mapIter->second=Svalue;
        }
    }
    else
    {
        mapLogValue[Sfind]=Svalue;
    }
    SL->Clear();                                                                //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete SL;
}
//------------------------------------------------------------------------------
AnsiString Space_Num(AnsiString S, int n)
{
    if(S.Length()>n)
        return "";

    AnsiString S1="                                                          ";
    return S1.SubString(1, n-S.Length()+1);
}
//------------------------------------------------------------------------------
