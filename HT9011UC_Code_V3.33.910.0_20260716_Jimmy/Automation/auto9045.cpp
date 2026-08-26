#include "MachineDefine.h"
#pragma hdrstop

#include "auto9045.h"

#include "cinitial.h"
#include "cMyDef.h"
#include "database.h"
#include "cprod.h"
#include "common.h"
#include "main.h"
#include "cSortCT.h"
#include "cObserver.h"
#include "cContactCT.h"
#include "cSocket.h"
#include "cpublic.h"
#include "csystem.h"
#include "mymessbox.h"
#include "cTemperFrom.h"
#include "cTrayForm.h"
#include "cHotPlate.h"
#include "cCounterClear.h"
#include "cBinSel.h"
#include "uTemp_Set.h"
#include "cShowBinSelect.h"
#include "cTestCategory.h"
#include "cUnitConvert.h"
#include "automation.h"
#include "cSetUp.h"
#include "MyKitSuck.h"
#include "Note.h"
#include "cContact.h"
#include "atester.h"
#include "uLotInfo.h"
#include "ProductionInfo.h"                                                     //AI(ht9045-v899) 20260515: F899-008 SetLotInfoGreatekOEE 需 fProductionInfo OEE_StartLot
#include "MonitorInterface.h"                                                   //AI(ht9045-v899) 20260515: F899-008 SetLotInfoGreatekOEE 需 fMonitor 連線預檢
#include "main.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------
int SetCategory(AnsiString *Data) //OK
{
    if(CheckSystemStart())
        return 2;
    if(CheckNeedCleanOut())
        return 1;

    int PosIndex[TEST_MAX_BIN];

    //Error Category 0~8
    //0=Auto1
    //1=Auto2
    //2=Auto3
    //3=Fix1
    //4=Fix2
    //5=Fix3
    //6=Fix4
    //7=Fix5
    //8=Fix6
    //檢查 IfError SetCategory 資料
    PosIndex[0] = atoi(Data[0].c_str());
    if(PosIndex[0]<0 || PosIndex[0]>8)                                          //Sam 20240227 : 修正 SetCategory 問題
        return 3;

    if(MachineTypeChoice==Type_HT9046_LS && USE_ROTATE_KIT==1)
    {
        if(PosIndex[0]==3 || PosIndex[0]==6)  //不能使用 Fix1 & Fix4
            return 3;
    }

    if(PosIndex[0]>=6 && PosIndex[0]<=8)
    {
        if(TrayForm.LodareType==0 || TrayForm.iFixTrayMode==0)
            return 3;
        else if(PosIndex[0]==6 && TrayForm.bTrayUpDownSet[eFix1]==false)
            return 3;
        else if(PosIndex[0]==7 && TrayForm.bTrayUpDownSet[eFix2]==false)
            return 3;
        else if(PosIndex[0]==8 && TrayForm.bTrayUpDownSet[eFix3]==false)
            return 3;
    }

   //Bin Category 0~9
    //0=Not Use
    //1=Auto1
    //2=Auto2
    //3=Auto3
    //4=Fix1
    //5=Fix2
    //6=Fix3
    //7=Fix4
    //8=Fix5
    //9=Fix6
    //檢查 Bin SetCategory 資料
    for(int i=1; i<=iTestBinCount; i++) //取得資料 檢查資料範圍                 //Sam 20240227 : 修正 SetCategory 問題
    {
        PosIndex[i] = atoi(Data[i].c_str());

        if(PosIndex[i]<0 || PosIndex[i]>=9)
            return 3;

        if(MachineTypeChoice==Type_HT9046_LS && USE_ROTATE_KIT==1)
        {
            if(PosIndex[i]==4 || PosIndex[i]==7)  //不能使用 Fix1 & Fix4
                return 3;
        }

        if(PosIndex[i]>=7 && PosIndex[i]<=9)
        {
            if(TrayForm.LodareType==0 || TrayForm.iFixTrayMode==0) //QQQ
                return 3;
            else if(PosIndex[i]==7 && TrayForm.bTrayUpDownSet[eFix1]==false)
                return 3;
            else if(PosIndex[i]==8 && TrayForm.bTrayUpDownSet[eFix2]==false)
                return 3;
            else if(PosIndex[i]==9 && TrayForm.bTrayUpDownSet[eFix3]==false)
                return 3;
        }
    }

    BinSelect[iTestRunMode].IfErrorT3 = PosIndex[0];
    BinSelect[iTestRunMode].iStackDefFailCate[PosIndex[0]] = 1;

    for(int i=0; i<iTestBinCount; i++)
    {
        //Bin0.....Bin15
        BinSelect[iTestRunMode].iCatDataT3Pos[i]=PosIndex[i+1];
    }

    bOLPSetCategroy=true;
    fBinSel->ChangeActivePageIndex();
    fBinSel->spbSaveClick(fBinSel);    //Sam 20230711 : 修正 OLP SetCategroy 異常
    fShowBinSelect->ShowBinSel();
    bOLPSetCategroy=false;
    return 0;
}
//---------------------------------------------------------------------------
int SetBinDefine(AnsiString *Data)      //Sam 20230803 : 新增OLP指令
{
    if(CheckSystemStart())
        return 2;
    if(CheckNeedCleanOut())
        return 1;

    int iBinPassFail[TEST_MAX_BIN];
    int iPassFail=0;
    int iStackDefFail[e3TrayCount];
    int iStack=0;

    for(int i=0; i<e3TrayCount; i++)
    {
        iStackDefFail[i]=-1;
    }

    for(int i=0; i<=iTestBinCount; i++) //取得資料 檢查資料範圍
    {
        iPassFail=atoi(Data[i].c_str());
        if(iPassFail==1)            //客戶定義 0:Fail ; 1:Pass 。 機台定義 1:Fail ; 0:Pass
            iBinPassFail[i]=0;
        else if(iPassFail==0)
            iBinPassFail[i]=1;
        else
            return 3;               //Undefine
    }

    //if(iBinPassFail[0]!=0)  //iBinPassFail[0] Error 一定是 Fail 不用特別去設定
    //{
    //    return 3;
    //}

    iStackDefFail[Prod.iIfErrorT6]=1;  //設定 Error Stack Fail

    for(int i=0; i<iTestBinCount; i++)
    {
        if(Prod.iT6PosCate[i]>0)
            iStack=Prod.iT6PosCate[i]-1;         //取得 Bin Stack
        else
            iStack=-1;

         if(iStack>=0)
         {
             if(iStackDefFail[iStack]==-1)                          //-1 Stack 沒有被設定過
             {
                iStackDefFail[iStack]=iBinPassFail[i+1];            //Stack 設定 PassFail
             }
             else
             {
                if(iStackDefFail[iStack]!=iBinPassFail[i+1])        //Stack 已經被設定過且 Bin Pass Fail 錯誤重疊
                {
                    return 4;
                }
             }
         }
    }

    for(int i=0; i<e3TrayCount; i++)
    {
        if(iStackDefFail[i]==-1)
        {
            Prod.iIsPassT6[i]=1;
            Prod.iIsFailT6[i]=0;                                    //沒用到的 Stack 塞 0
        }
        else
        {
            Prod.iIsFailT6[i]=iStackDefFail[i];                     //Steven 20240105 : Prod.bIsPass --> Prod.iIsFailT6
            Prod.iIsPassT6[i]=(Prod.iIsFailT6[i]==0)?1:0;
        }
    }

    bOLPSetCategroy=true;
    fBinSel->ChangeActivePageIndex();
    fBinSel->spbSaveClick(fBinSel);
    fShowBinSelect->ShowBinSel();
    bOLPSetCategroy=false;
    return 0;
}
//---------------------------------------------------------------------------
int SetFixTrayDefine(AnsiString *Data)  //Sam 20230921 : 新增 FixTray 指令
{
    if(CheckSystemStart())
        return 2;
    if(CheckNeedCleanOut())
        return 1;
    int iLoaderType=atoi(Data[0].c_str());      //0:Same ; 1:Different
    int iFixTrayMode=atoi(Data[1].c_str());     //0:Full Bin ; 1:Up Down
    int iFixUpDwon1=atoi(Data[2].c_str());      //0:Not use ; 1:Up Down
    int iFixUpDwon2=atoi(Data[3].c_str());
    int iFixUpDwon3=atoi(Data[4].c_str());

    if(iLoaderType==0)
    {
        if(iFixTrayMode>0 || iFixUpDwon1>0 || iFixUpDwon2>0 || iFixUpDwon3>0)
            return 3;
    }

    if(iFixTrayMode==0)
    {
        if(iFixUpDwon1>0 || iFixUpDwon2>0 || iFixUpDwon3>0)
            return 3;
    }

    if(MachineTypeChoice==Type_HT9046_LS && USE_ROTATE_KIT==1)
    {
        if(iFixUpDwon1>0)
            return 3;  //不能使用 Fix1
    }

    AnsiString S=GetLastOpenFN();
    AnsiString szDir="";
    szDir.sprintf("%s%s", DataPath, S);
    szDir+="\\Tray.Data";
    if(fTrayAssignment->fShow)
    {
        fTrayAssignment->rgLoaderType->ItemIndex=iLoaderType;
        if(iLoaderType==1)
        {
            fTrayAssignment->rgFixTrayMode->ItemIndex=iFixTrayMode;
            if(iFixTrayMode==1)
            {
                fTrayAssignment->ckUseFix1->Checked=(iFixUpDwon1==1)?true:false;
                fTrayAssignment->ckUseFix2->Checked=(iFixUpDwon2==1)?true:false;
                fTrayAssignment->ckUseFix3->Checked=(iFixUpDwon3==1)?true:false;
            }
        }
    }
    WriteIniData(szDir, "Flag", "Loader Type",iLoaderType);
    WriteIniData(szDir, "Flag", "Fix Tray Mode",iFixTrayMode);
    WriteIniData(szDir, "Flag", "Use Fix1 Tray",(iFixUpDwon1==1)?true:false);
    WriteIniData(szDir, "Flag", "Use Fix2 Tray",(iFixUpDwon2==1)?true:false);
    WriteIniData(szDir, "Flag", "Use Fix3 Tray",(iFixUpDwon3==1)?true:false);
    fTrayAssignment->ReadFile();
    return 0;
}
//---------------------------------------------------------------------------
int SetDutOnOff(AnsiString *Data) //OK
{
    if(CheckSystemStart())
        return 2;
    if(CheckNeedCleanOut())
        return 1;

    //根據模式選擇
    //TestIF.iTestMode
    int MaxI=0;
    int MaxJ=0;
    switch(TestIF.iTestMode)
    {
      case DualSite://1x2
           MaxI=1;
           MaxJ=2;
           break;
      case SingleSite://1x1
           MaxI=1;
           MaxJ=1;
           break;
      case QualSite1X4://1x4
      case _8Site1X4:  //ChungHung 20150528 add for 海思 _8Site1x4
           MaxI=1;
           MaxJ=4;
           break;
      case QualSite2X2://2x2
           MaxI=2;
           MaxJ=2;
           break;
      case _16Site4X4:  //Sam 20190226 : 16Site4X4
      case _8Site2X4://2x4
           MaxI=2;
           MaxJ=4;
           break;
      case _16Site2X8://2x8
           MaxI=2;
           MaxJ=8;
           break;
    };

    if(fMain->CanChangeSite()==false)
    {
        return 3;
    }

    //AnsiString KeyWord[16]={ "Dut1=" ,"Dut2=" ,"Dut3=" ,"Dut4=" ,"Dut5=" ,"Dut6=" ,"Dut7=" ,"Dut8=" ,
    //                         "Dut9=" ,"Dut10=" ,"Dut11=" ,"Dut12=" ,"Dut13=" ,"Dut14=" ,"Dut15=" ,"Dut16=" };
    //AnsiString Result[2]={ "Close","Open"};

    int index;
    int value;
    bool bDutOnOff[MAX_SOCKET_TOTAL]={false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,
                                      false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};

    for(int i=0; i<MaxI; i++)
    {
        for(int j=0; j<MaxJ; j++)
        {
            index = i*MaxJ+j;
            if(index<8)
            {
                if(TestIF.iSiteMap[index/4][index%4]==0)
                {
                    //ChungHung 20130910 alter for SCK can close site by Index
                    bTestSiteUse[0][index/4][index%4]=false;
                    LastSet.bUseTestSocket[0][index/4][index%4]=false;

                    bTestSiteUse[1][index/4][index%4]=false;
                    LastSet.bUseTestSocket[1][index/4][index%4]=false;
                }
                else
                {
                    //ChungHung 20130910 alter for SCK can close site by Index
                    bTestSiteUse[0][index/4][index%4]=!bTestSiteUse[0][index/4][index%4];
                    LastSet.bUseTestSocket[0][index/4][index%4]=!LastSet.bUseTestSocket[0][index/4][index%4];

                    bTestSiteUse[1][index/4][index%4]=!bTestSiteUse[1][index/4][index%4];
                    LastSet.bUseTestSocket[1][index/4][index%4]=!LastSet.bUseTestSocket[1][index/4][index%4];
                }
            }
            else
            {
                if(TestIF.iSiteMap[index/4-2][index%4+4]==0)
                {
                    //ChungHung 20130910 alter for SCK can close site by Index
                    bTestSiteUse[0][index/4-2][index%4+4]=false;
                    LastSet.bUseTestSocket[0][index/4-2][index%4+4]=false;

                    bTestSiteUse[1][index/4-2][index%4+4]=false;
                    LastSet.bUseTestSocket[1][index/4-2][index%4+4]=false;
                }
                else
                {
                    //ChungHung 20130910 alter for SCK can close site by Index
                    bTestSiteUse[0][index/4-2][index%4+4]=!bTestSiteUse[0][index/4-2][index%4+4];
                    LastSet.bUseTestSocket[0][index/4-2][index%4+4]=!LastSet.bUseTestSocket[0][index/4-2][index%4+4];

                    bTestSiteUse[1][index/4-2][index%4+4]=!bTestSiteUse[1][index/4-2][index%4+4];
                    LastSet.bUseTestSocket[1][index/4-2][index%4+4]=!LastSet.bUseTestSocket[1][index/4-2][index%4+4];
                }
            }

            value =  atoi(Data[index].c_str());
            bDutOnOff[index] = (value==1)? true:false;
        }
    }

    for(int i=0; i<MaxI; i++)
    {
        for(int j=0; j<MaxJ; j++)
        {
          index = i*MaxJ+j;
          //ChungHung 20130910 alter for SCK can close site by Index
          LastSet.bUseTestSocket[0][i][j] = bDutOnOff[index];
          bTestSiteUse[0][i][j] = bDutOnOff[index];

          LastSet.bUseTestSocket[1][i][j] = bDutOnOff[index];
          bTestSiteUse[1][i][j] = bDutOnOff[index];
        }
    }

    fMain->ShowTestHeadComp(false);
    return 0;
}
//---------------------------------------------------------------------------
int SetMapping(AnsiString *Data) //ok
{
    if(CheckSystemStart())
      return 2;
    if(CheckNeedCleanOut())
      return 1;

    //根據模式選擇
    //TestIF.iTestMode
    int MaxI=0;
    int MaxJ=0;
    switch(TestIF.iTestMode)
    {
      case DualSite://1x2
           MaxI=1;
           MaxJ=2;
           break;
      case SingleSite://1x1
           MaxI=1;
           MaxJ=1;
           break;
      case QualSite1X4://1x4
      case _8Site1X4:  //ChungHung 20150528 add for 海思 _8Site1x4
           MaxI=1;
           MaxJ=4;
           break;
      case QualSite2X2://2x2
           MaxI=2;
           MaxJ=2;
           break;
      case _6Site2X3:  //ChungHung 20140115 add for 2x3_6
           MaxI=2;
           MaxJ=3;
           break;
      case _16Site4X4:  //Sam 20190226 : 16Site4X4
      case _8Site2X4://2x4
           MaxI=2;
           MaxJ=4;
           break;
      case _10Site2X5://2x5 //wei 20190614 10 site
      case _12Site2X6://2x8
           MaxI=2;
           MaxJ=8;
           break;
    };

    //AnsiString KeyWord[16]={ "Dut1=" ,"Dut2=" ,"Dut3=" ,"Dut4=" ,"Dut5=" ,"Dut6=" ,"Dut7=" ,"Dut8=" ,
    //                         "Dut9=" ,"Dut10=" ,"Dut11=" ,"Dut12=" ,"Dut13=" ,"Dut14=" ,"Dut15=" ,"Dut16=" };
    //AnsiString Position[16]={ "Ch1" ,"Ch2" , "Ch3", "Ch4", "Ch5", "Ch6", "Ch7", "Ch8",
    //                          "Ch9", "Ch10", "Ch11", "Ch12", "Ch13", "Ch14", "Ch15", "Ch16" };

    AnsiString value;
    int index=0;
    int PosIndex[16]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
                      -1,-1,-1,-1,-1,-1};
    bool bSet[17]={false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};

    for(int i=0; i<MaxI; i++)
    {
        for(int j=0; j<MaxJ; j++)
        {
            index = i*MaxJ+j;
            PosIndex[index] = atoi(Data[index].c_str());

            if(PosIndex[index]<1 || PosIndex[index]>MaxI*MaxJ)
              return 3;

            if(bSet[PosIndex[index]] == false)
                bSet[PosIndex[index]]=true;
            else
                return 3;
        }
    }

    for(int i=0; i<MaxI; i++)
    {
        for(int j=0; j<MaxJ; j++)
        {
            index = i*MaxJ+j;
            TestIF_File.iSiteMap[i][j] = PosIndex[index];
        }
    }

    AnsiString S=GetLastOpenFN();
    AnsiString szDir="";
    AnsiString str;

    szDir.sprintf("%s%s", DataPath, S);
    MyForceDirectories(szDir, "SetMapping");
    szDir+="\\HandlerCondition.Data";

    for(int i=0; i<2; i++)
    {
        for(int j=0; j<8; j++)
        {
            str.sprintf("Site %c%c", i+'A', j+'a');//kevin 20180312 j+'A' ==> j+'a'
            WriteIniData(szDir, "Configuration", str, TestIF_File.iSiteMap[i][j]);
        }
    }

    ChangeSite();

    DoStructUnitConvert();
    fMain->ShowTestHeadComp(false);
    fTestCategory->AdjFormData();
    fContactCT->ShowFormComp();

    return 0;
}
//---------------------------------------------------------------------------
int SetSetUpFileName(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;
    if(CheckNeedCleanOut())
        return 1;

    int index = fMain->cbSetupFileName->Items->IndexOf(Data[0]);
    if(index!=-1)
    {
        RecordProcess("SetSetUpFileName");
        fMain->cbSetupFileName->ItemIndex = index;
        fMain->ChangeSetUpFile(fMain->cbSetupFileName->Text);
        return 0;
    }
    return 3;
}
//---------------------------------------------------------------------------
int SetSoakTime(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;

    Temperature.fSoakTime=atof(Data[0].c_str());
    fMain->edSoakTime->Text     =Temperature.fSoakTime;
    fTemp_Set->edSoakTime->Text =Temperature.fSoakTime;

    AnsiString S="";
    S=GetLastOpenFN();
    AnsiString szDir="";
    szDir.sprintf("%s%s", DataPath, S);
    MyForceDirectories(szDir, "SetSoakTime");
    szDir+="\\Temperature.Data";

    WriteIniData(szDir, "Time",       "Soak",       Temperature.fSoakTime);

    return 0;
}
//---------------------------------------------------------------------------
int SetTemperature(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;

    Temperature.fWorkTemperBase=atof(Data[0].c_str());
    fMain->edWorkTemperBase->Text   = Temperature.fWorkTemperBase;
    fTemp_Set->edWorkTemp->Text     = Temperature.fWorkTemperBase;

    AnsiString S="";

    S=GetLastOpenFN();
    AnsiString szDir="";
    szDir.sprintf("%s%s", DataPath, S);
    MyForceDirectories(szDir, "SetTemperature");
    szDir+="\\Temperature.Data";

    WriteIniData(szDir, "Mode",       "Temperature", Temperature.fWorkTemperBase);

    return 0;
}
//---------------------------------------------------------------------------
int SetTempMode(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;
    if(CheckNeedCleanOut())
        return 1;
    LastSet.iTemperature=atoi(Data[0].c_str());
    return fMain->ChangeTempMode(LastSet.iTemperature, false, bRefreshFunction);
}
//---------------------------------------------------------------------------
int SetTesterConnect(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;
    if(CheckNeedCleanOut())
        return 1;
    LastSet.iTester=atoi(Data[0].c_str());
    return fMain->ChangeTesterConnect(LastSet.iTester,false);
}
//---------------------------------------------------------------------------
int SetTesterMode(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;
    if(CheckNeedCleanOut())
        return 1;
    fAutomation->TestMode=atoi(Data[0].c_str());
    return 0;
}
//---------------------------------------------------------------------------
int SetLowYield(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;

    AnsiString szDir="";
    TestIF_File.bFailAlarmLowYield  =atoi(Data[0].c_str());
    TestIF_File.iLowYieldLimit      =atof(Data[1].c_str());
    TestIF_File.iLowYieldCount      =atoi(Data[2].c_str());

    szDir.sprintf("%s%s\\Tester.Data", DataPath, GetLastOpenFN());
    WriteIniData(szDir, "Low Yield Alarm", "Enable",            int(TestIF_File.bFailAlarmLowYield));
    WriteIniData(szDir, "Low Yield Alarm", "Limit",             TestIF_File.iLowYieldLimit);
    WriteIniData(szDir, "Low Yield Alarm", "Count",             int(TestIF_File.iLowYieldCount));
    return 0;
}
//---------------------------------------------------------------------------
int SetByArmPerSiteDiffYield(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;

    AnsiString szDir="";
    TestIF_File.bFailAlarmSiteYieldDifferent        =atoi(Data[0].c_str());
    TestIF_File.iFailAlarmSiteYield                 =atof(Data[1].c_str());
    TestIF_File.iFailAlarmSiteYieldDifferentCount   =atoi(Data[2].c_str());
    szDir.sprintf("%s%s\\Tester.Data", DataPath, GetLastOpenFN());
    WriteIniData(szDir, "Site Yield Alarm","Site Yield Different",          int(TestIF_File.bFailAlarmSiteYieldDifferent));
    WriteIniData(szDir, "Site Yield Alarm","Site Yield",             TestIF_File.iFailAlarmSiteYield);
    WriteIniData(szDir, "Site Yield Alarm","Site Yield Different Count",    int(TestIF_File.iFailAlarmSiteYieldDifferentCount));
    return 0;
}
//---------------------------------------------------------------------------
int SetConsecutiveFailureAlarmByHead(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;

    AnsiString szDir="";
    TestIF_File.bContsFailByHead        =atoi(Data[0].c_str());
    TestIF_File.iContsFailHeadAlarmCT   =atoi(Data[1].c_str());
    szDir.sprintf("%s%s\\Tester.Data", DataPath, GetLastOpenFN());
    WriteIniData(szDir, "Alarm",     "HeadEnable",      int(TestIF_File.bContsFailByHead));
    WriteIniData(szDir, "Alarm",     "HeadCT",          int(TestIF_File.iContsFailHeadAlarmCT));
    return 0;
}
//---------------------------------------------------------------------------
int SetConsecutiveFailureAlarmBySocket(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;

    AnsiString szDir="";
    TestIF_File.bContsFailBySocket      =atoi(Data[0].c_str());
    TestIF_File.iContsFailSocketAlarmCT =atoi(Data[1].c_str());
    szDir.sprintf("%s%s\\Tester.Data", DataPath, GetLastOpenFN());
    WriteIniData(szDir, "Alarm",     "SocketEnable",    int(TestIF_File.bContsFailBySocket));
    WriteIniData(szDir, "Alarm",     "SocketCT",        int(TestIF_File.iContsFailSocketAlarmCT));
    return 0;
}
//---------------------------------------------------------------------------
int SetAllSiteFailFor9045(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;

    AnsiString szDir="";
    TestIF_File.bAllSiteFail=atoi(Data[0].c_str());
    szDir.sprintf("%s%s\\Tester.Data", DataPath, GetLastOpenFN());
    WriteIniData(szDir, "Alarm4", "AllSiteFail",int(TestIF_File.bAllSiteFail));
    return 0;
}
//---------------------------------------------------------------------------
int SetContactModeFor9045(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;

    AnsiString szDir="";
    DeviceForm_File.ContactMode=atoi(Data[0].c_str());
    szDir.sprintf("%s%s\\Contact.Data", DataPath, GetLastOpenFN());
    WriteIniData(szDir, "Mode", "Contact",        int(DeviceForm_File.ContactMode));
    return 0;
}
//---------------------------------------------------------------------------
int SetContactVacuumMode(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;

    AnsiString szDir="";
    DeviceForm_File.VacuumMode=atoi(Data[0].c_str());
    szDir.sprintf("%s%s\\Contact.Data", DataPath, GetLastOpenFN());
    WriteIniData(szDir, "Mode", "Vacuum",          int(DeviceForm_File.VacuumMode));
    return 0;
}
//---------------------------------------------------------------------------
int SetContactDropWait(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;

    AnsiString szDir="";
    DeviceForm_File.DropWait=atof(Data[0].c_str());
    szDir.sprintf("%s%s\\Contact.Data", DataPath, GetLastOpenFN());
    WriteIniData(szDir, "Wait Time", "Drop Wait",   DeviceForm_File.DropWait);
    return 0;
}
//---------------------------------------------------------------------------
int SetSlowContactSpeed(AnsiString *Data)
{
    AnsiString szDir="";
    if(CheckSystemStart())
        return 2;

    DeviceForm_File.DropSpeed=atoi(Data[0].c_str());
    szDir.sprintf("%s%s\\Contact.Data", DataPath, GetLastOpenFN());
    WriteIniData(szDir, "Wait Time", "Drop Speed",  DeviceForm_File.DropSpeed);
    return 0;
}
//---------------------------------------------------------------------------
int SetShuttleWaitOutSideCamber(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;

    AnsiString szDir="";
    DeviceForm_File.bShuttleWaitingOutSiteChamber=atoi(Data[0].c_str());
    szDir.sprintf("%s%s\\Contact.Data", DataPath, GetLastOpenFN());
    WriteIniData(szDir, "Mode", "Shuttle Waiting Out Site Chamber",  int(DeviceForm_File.bShuttleWaitingOutSiteChamber));
    return 0;
}
//---------------------------------------------------------------------------
int SetPickShuttleDeviceAfterTested(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;

    AnsiString szDir="";
    DeviceForm_File.bSuckShuttleDeviceAfterTested=atoi(Data[0].c_str());
    szDir.sprintf("%s%s\\Contact.Data", DataPath, GetLastOpenFN());
    WriteIniData(szDir, "Mode", "Suck Shuttle Device After Tested",  int(DeviceForm_File.bSuckShuttleDeviceAfterTested));
    return 0;
}
//---------------------------------------------------------------------------
int SetPickShuttleDeviceThenWaitOnShuttle(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;

    AnsiString szDir="";
    DeviceForm_File.bSuckShuttleDeviceWaitOnShuttle=atoi(Data[0].c_str());
    szDir.sprintf("%s%s\\Contact.Data", DataPath, GetLastOpenFN());
    WriteIniData(szDir, "Mode", "Suck Shuttle Device Wait On Shuttle",  int(DeviceForm_File.bSuckShuttleDeviceWaitOnShuttle));
    return 0;
}
//---------------------------------------------------------------------------
int SetPickShuttleDeviceTogetherFor32SiteN(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;

    AnsiString szDir="";
    DeviceForm_File.bPickShuttleDeviceTogether=atoi(Data[0].c_str());
    szDir.sprintf("%s%s\\Contact.Data", DataPath, GetLastOpenFN());
    WriteIniData(szDir, "Mode", "Pick Shuttle Device Together",  int(DeviceForm_File.bPickShuttleDeviceTogether));
    return 0;
}
//---------------------------------------------------------------------------
int SetIndexArm1Height(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;

    AnsiString szDir="";
    if(atof(Data[1].c_str())<atof(Data[0].c_str()))     //PickUp 必須高於 Release
        return 3;
    DeviceForm_File.IndexPlace[0]   =atof(Data[0].c_str());
    DeviceForm_File.IndexArmPick[0] =atof(Data[1].c_str());
    DeviceForm_File.IndexDrop[0]    =atof(Data[2].c_str());
    DeviceForm_File.IndexContact[0] =atof(Data[3].c_str());

    szDir.sprintf("%s%s\\Contact.Data", DataPath, GetLastOpenFN());
    WriteIniData(szDir, "Test Arm1", "Place",      DeviceForm_File.IndexPlace[0]);
    WriteIniData(szDir, "Test Arm1", "Pick Up",    DeviceForm_File.IndexArmPick[0]);
    WriteIniData(szDir, "Test Arm1", "Drop",       DeviceForm_File.IndexDrop[0]);
    WriteIniData(szDir, "Test Arm1", "Contact",    DeviceForm_File.IndexContact[0]);
    return 0;
}
//---------------------------------------------------------------------------
int SetIndexArm2Height(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;

    AnsiString szDir="";
    if(atof(Data[1].c_str())<atof(Data[0].c_str()))     //PickUp 必須高於 Release
        return 3;
    DeviceForm_File.IndexPlace[1]   =atof(Data[0].c_str());
    DeviceForm_File.IndexArmPick[1] =atof(Data[1].c_str());
    DeviceForm_File.IndexDrop[1]    =atof(Data[2].c_str());
    DeviceForm_File.IndexContact[1] =atof(Data[3].c_str());

    szDir.sprintf("%s%s\\Contact.Data", DataPath, GetLastOpenFN());
    WriteIniData(szDir, "Test Arm2", "Place",      DeviceForm_File.IndexPlace[1]);
    WriteIniData(szDir, "Test Arm2", "Pick Up",    DeviceForm_File.IndexArmPick[1]);
    WriteIniData(szDir, "Test Arm2", "Drop",       DeviceForm_File.IndexDrop[1]);
    WriteIniData(szDir, "Test Arm2", "Contact",    DeviceForm_File.IndexContact[1]);
    return 0;
}
//---------------------------------------------------------------------------
int SetTestICCheckMode(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;

    AnsiString szDir="";
    DeviceForm_File.iSocketInitialICCheckPosition=atoi(Data[0].c_str());
    szDir.sprintf("%s%s\\Contact.Data", DataPath, GetLastOpenFN());
    WriteIniData(szDir, "Mode", "iSocketInitialICCheckPosition",  int(DeviceForm_File.iSocketInitialICCheckPosition));
    return 0;
}
//---------------------------------------------------------------------------
int SetAboveSocket(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;

    AnsiString szDir="";
    if(DeviceForm_File.iSocketInitialICCheckPosition!=1)
        return 3;
    DeviceForm_File.fSocketInitialICCheckPositionOffset=atof(Data[0].c_str());
    szDir.sprintf("%s%s\\Contact.Data", DataPath, GetLastOpenFN());
    WriteIniData(szDir, "Mode", "fSocketInitialICCheckPositionOffset",  DeviceForm_File.fSocketInitialICCheckPositionOffset);
    return 0;
}
//---------------------------------------------------------------------------
int SetHotPlate1(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;
    if(CheckNeedCleanOut())
        return 1;
    AnsiString szDir="";
    if(atoi(Data[0].c_str())==0)
    {
        if(HotPlateForm_File.iPlateSelect==1)//HP1 要關閉，不能全關
            return 3;
        else
            HotPlateForm_File.iPlateSelect=2;
        }
    else
    {
        if(HotPlateForm_File.iPlateSelect==2)
            HotPlateForm_File.iPlateSelect=3;
        }
    szDir.sprintf("%s%s\\HotPlate.Data", DataPath, GetLastOpenFN());
    WriteIniData(szDir, "Hotplate Form", "Using Flag",  int(HotPlateForm_File.iPlateSelect));
    return 0;
}
//---------------------------------------------------------------------------
int SetHotPlate2(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;
    if(CheckNeedCleanOut())
        return 1;
    AnsiString szDir="";
    if(atoi(Data[0].c_str())==0)
    {
        if(HotPlateForm_File.iPlateSelect==2)//HP1 要關閉，不能全關
            return 3;
        else
            HotPlateForm_File.iPlateSelect=1;
        }
    else
    {
        if(HotPlateForm_File.iPlateSelect==1)
            HotPlateForm_File.iPlateSelect=3;
        }
    szDir.sprintf("%s%s\\HotPlate.Data", DataPath, GetLastOpenFN());
    WriteIniData(szDir, "Hotplate Form", "Using Flag",  int(HotPlateForm_File.iPlateSelect));
    return 0;
}
//---------------------------------------------------------------------------
int SetTesterInitialMaximumTest(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;

    AnsiString szDir="";
    TestIF_File.iInitialMaxTime=atof(Data[0].c_str());
    szDir.sprintf("%s%s\\Tester.Data", DataPath, GetLastOpenFN());
    WriteIniData(szDir, "Time",      "Initial MAX Time",    TestIF_File.iInitialMaxTime);
    return 0;
}
//---------------------------------------------------------------------------
int SetTesterMaximumTest(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;

    AnsiString szDir="";
    TestIF_File.iMaxTime=atof(Data[0].c_str());
    szDir.sprintf("%s%s\\Tester.Data", DataPath, GetLastOpenFN());
    WriteIniData(szDir, "Time",      "MAX Time",    TestIF_File.iMaxTime);
    return 0;
}
//---------------------------------------------------------------------------
int SetTesterDummyTest(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;
    if(CheckNeedCleanOut())
        return 1;
    AnsiString szDir="";
    TestIF_File.iDummyTime=atof(Data[0].c_str());
    szDir.sprintf("%s%s\\Tester.Data", DataPath, GetLastOpenFN());
    WriteIniData(szDir, "Time",      "Dummy Time",    TestIF_File.iDummyTime);
    return 0;
}
//---------------------------------------------------------------------------
int SetTesterStartDelay(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;

    AnsiString szDir="";
    TestIF_File.dStartDelayTime=atof(Data[0].c_str());
    szDir.sprintf("%s%s\\Tester.Data", DataPath, GetLastOpenFN());
    WriteIniData(szDir, "Time",      "Stary Delay",    TestIF_File.dStartDelayTime);
    return 0;
}
//---------------------------------------------------------------------------
int SetHotZ1Down(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;

    AnsiString szDir="";
    Temperature.bZ2DownSocket=atoi(Data[0].c_str());
    szDir.sprintf("%s%s\\Temperature.Data", DataPath, GetLastOpenFN());
    WriteIniData(szDir, "Time", "bZ2DownSocket",int(Temperature.bZ2DownSocket));
    return 0;
}
//---------------------------------------------------------------------------
int SetHotShuttleSoakMode(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;

    AnsiString szDir="";
    Temperature.iShuttleSoakTimeMode=atoi(Data[0].c_str());
    szDir.sprintf("%s%s\\Temperature.Data", DataPath, GetLastOpenFN());
    WriteIniData(szDir, "Time", "In Shuttle Soak Time Mode",int(Temperature.iShuttleSoakTimeMode));
    return 0;
}
//---------------------------------------------------------------------------
int SetAmbientCheck(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;

    AnsiString szDir="";
    Temperature.bUseAbitCHK=atoi(Data[0].c_str());
    szDir.sprintf("%s%s\\Temperature.Data", DataPath, GetLastOpenFN());
    WriteIniData(szDir, "Mode", "AmbCheck",int(Temperature.bUseAbitCHK));
    return 0;
}
//---------------------------------------------------------------------------
int SetAmbientCheckTemp(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;

    AnsiString szDir="";
    Temperature.fAbitTemp=atof(Data[0].c_str());
    szDir.sprintf("%s%s\\Temperature.Data", DataPath, GetLastOpenFN());
    WriteIniData(szDir, "Ambient",      "Temperature",    Temperature.fAbitTemp);
    return 0;
}
//---------------------------------------------------------------------------
int SetTemperatureOffset(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;

    AnsiString szDir="";
    Temperature.bEnableTempOffsetForInitial=atoi(Data[0].c_str());
    szDir.sprintf("%s%s\\Temperature.Data", DataPath, GetLastOpenFN());
    WriteIniData(szDir, "InitialMode",      "EnableTemperatureOffsetforInitial",    int(Temperature.bEnableTempOffsetForInitial));
    return 0;
}
//---------------------------------------------------------------------------
int SetContactCountForOffsetPeriod(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;

    AnsiString szDir="";
    Temperature.iCintactCntForTempOffsetAtInitial=atoi(Data[0].c_str());
    szDir.sprintf("%s%s\\Temperature.Data", DataPath, GetLastOpenFN());
    WriteIniData(szDir, "InitialMode",      "iCintactCntForTempOffsetAtInitial",    int(Temperature.iCintactCntForTempOffsetAtInitial));
    return 0;
}
//---------------------------------------------------------------------------
int SetContactCountForCoolDown(AnsiString *Data)
{
    if(CheckSystemStart())
        return 2;

    AnsiString szDir="";
    Temperature.iCintactDelayCntForInitTempOffset=atoi(Data[0].c_str());
    szDir.sprintf("%s%s\\Temperature.Data", DataPath, GetLastOpenFN());
    WriteIniData(szDir, "InitialMode",      "iCintactDelayCntForInitTempOffset",    int(Temperature.iCintactDelayCntForInitTempOffset));
    return 0;
}
//---------------------------------------------------------------------------
int SetLotInfo(AnsiString *Data)
{
    if(fMain->CheckCanChangeRealDummy())                                        //Sam 20200618 : FT RT 切換改為 CleanOut
    {
        fLotInfo->edtSysOperatorID->Text=Data[1];
        fLotInfo->SetLotID(Data[0], false);
        fLotInfo->SetLotStart(__FUNC__);
        if(Data[2]=="0")
        {
            fMain->DoFTRTClick(false, false);                                   //RogerYang 20260410 : 整合並區分手動按下還是程式按下
        }
        else
        {
            fMain->DoFTRTClick(true, false);                                    //RogerYang 20260410 : 整合並區分手動按下還是程式按下
        }
        return 0;
    }
    else
    {
        return 2;
    }
}
//---------------------------------------------------------------------------
//AI(JimmyChiu) 20260515: F899-008 OEE_StartLot 失敗 errMsg 關鍵字分流（對齊 SPEC §6.3 / §8.5）
// 實際對盤 OEE_StartLot 6 條 errMsg：
//   - "MO or Machine ID is empty"                                  → 62
//   - "... Set MO Information Fail!" / "Auto Download Setup File by MO ..." /
//     "... AutoDownloadSetupFileByMO() Function" / "... CheckLoadMOInformation() Function" → 61。
// 90 為防禦性 fallback（理論不觸發）。
static int GreatekMapOEEStartLotError(AnsiString asErrorMsg)
{
    if(asErrorMsg.Pos("Machine ID")>0)
        return 62;
    if(asErrorMsg.Pos("MO")>0)
        return 61;
    return 90;
}
//---------------------------------------------------------------------------
//AI(JimmyChiu) 20260515: F899-008 超豐 LotInfo_REQUEST 內嵌 OEE Start Lot（v3）
// 設計依據：SPEC F899-008 §6.3 v3。
// 2026-05-15 客戶二次確認：超豐 Agent 不送 LotID/OPID/RunMode（全部由雲端 MO file 帶入），
//   故本函式忽略 Data[0..2]，僅執行：預檢查 → OEE_StartLot → SetLotID → SetLotStart →
//   SetLotComponents(false) → SetFirstTrayCheckOnUnloader。
// 不重構 sbSECSLotStartClick（見 SPEC §6.1）。
int SetLotInfoGreatekOEE(AnsiString *Data, AnsiString &asErrorMsg)
{
    (void)Data;     // 客戶決定不使用，保留參數僅為呼叫端 signature 一致
    asErrorMsg="";

    // ── (1) 預檢查──
    if(fMain->CheckCanChangeRealDummy()==false)
    {
        asErrorMsg="Machine running or has residual IC";
        return 2;
    }

    // Monitor 連線預檢（對齊 uLotInfo.cpp L8023 手動路徑）。
    if(IniConfig.bC11UseMonitorView && fMonitor->MVCtrl->IsConnect()==false)
    {
        asErrorMsg="Please Check Monitor Connect!!!";
        return 60;
    }

    // ── (2) OEE_StartLot（silent，使用 ProductionInfo.cpp 既有介面）──
    //   超豐由雲端 MO file 帶入 LotID/OPID 等資訊，本函式不再寫 UI 欄位。
    if(fProductionInfo->OEE_StartLot(true, &asErrorMsg)==false)
    {
        if(asErrorMsg=="")
            asErrorMsg="Production Start Lot Fail";
        return GreatekMapOEEStartLotError(asErrorMsg);
    }

    // ── (3) SetLotID + SetLotStart（對齊 §1.2 step 12/13）──
    //   edtSysLotID->Text 由 OEE 雲端下載階段透過 MO 對應流程更新（OEE_StartLot 後讀當下值）。
    fLotInfo->SetLotID(fLotInfo->edtSysLotID->Text, false);
    fLotInfo->SetLotStart(__FUNC__);

    // ── (4) 強制 lock：補 SetLotStart 對空 LotID 走 SetLotComponents(true) 的缺口──
    fLotInfo->SetLotComponents(false);

    // ── (5) First Tray Check（對齊 §1.2 step 15）──
    fLotInfo->SetFirstTrayCheckOnUnloader();

    return 0;
}
//---------------------------------------------------------------------------
//extern void SetRunStartMode(eRunStartMode Mode=rsmNull, AnsiString ModeText="");
int SetStartMode(AnsiString *Data)                                              //Sam 20221212 : 新增 StartMode 指令
{
    int iStartMode=0;
    //if(RunInfo.bLotStart==false)
    if(fMain->CheckCanChangeRealDummy())
    {
        if(Data[0]=="0" || Data[0]=="1" || Data[0]=="2" || Data[0]=="3")
        {
            iStartMode=atoi(Data[0].c_str());
            SetRunStartMode((eRunStartMode)iStartMode);
            return 0;
        }
        else
        {
            return 2;
        }
    }
    else
    {
        return 2;
    }
}
//---------------------------------------------------------------------------
bool CheckInitialOK()
{
    return InitialOK;
}
//---------------------------------------------------------------------------
bool CheckNeedCleanOut()
{
    if(CheckCanChangeRealDummy()==false || HasICUnderMachine())                 //Steven 20240719 : && --> ||
        return true; //需要cleanout
    else
        return false;
}
//---------------------------------------------------------------------------
bool CheckSystemStart()
{
    return SystemStart;
}
//---------------------------------------------------------------------------
int ShowOLPState(int iState)
{
    fMain->ShowOLPState(iState);
    return 0;
}
//---------------------------------------------------------------------------
AnsiString GetMainStatus()
{
    AnsiString S=fMain->palMainStatus->Caption.UpperCase();
    if(S=="RUNNING" ||
       S=="PAUSE" ||
       S=="HALT"  ||
       S=="LOCK")
    {
        if(S=="RUNNING")
        {
            S="RUN";
        }

//        if(S=="LOCK")
//        {
//            S="HALT";
//        }
        return S;
    }
    else
    {
        return "RUN";
    }
}
//---------------------------------------------------------------------------
AnsiString GetDataPath()
{
    return DataPath;
}
//---------------------------------------------------------------------------
AnsiString GetSoftwareVersion()
{
    return fObserver->Memo1->Lines->Strings[0];
}
//---------------------------------------------------------------------------
AnsiString GetActTemp(int i)
{
    return UN150Read[i];
}
//---------------------------------------------------------------------------
AnsiString GetHandlerType()
{
    return "HT9045";
}
//---------------------------------------------------------------------------
AnsiString GetSetUpFileName()
{
    return fMain->cbSetupFileName->Text;
}
//---------------------------------------------------------------------------
AnsiString GetJamCount()
{
    if(CUSTOMER_CODE==CC_MTI || CUSTOMER_CODE==CC_PTI)  //Sam 20190429 : Add CC_PTI_NEWWORK
    {
        return AnsiString(LastSet.iJamCount[0]);
    }
    else
    {
        return AnsiString(LastSet.iJamCount[1]);
    }
}
//---------------------------------------------------------------------------
AnsiString GetSoakTime()
{
    return AnsiString(Temperature.fSoakTime);
}
//---------------------------------------------------------------------------
AnsiString GetTemperature()
{
    return AnsiString(Temperature.fWorkTemperBase);
}
//---------------------------------------------------------------------------
AnsiString GetTempMode()
{
    return AnsiString(LastSet.iTemperature);
}
//---------------------------------------------------------------------------
AnsiString GetTesterConnect()
{
    return AnsiString(LastSet.iTester);
}
//---------------------------------------------------------------------------
AnsiString GetTesterMode()
{
    return AnsiString(fAutomation->TestMode);   //未完成 配合測試流程
}
//---------------------------------------------------------------------------
AnsiString GetLoadCount()
{
    return LastSet.SendCT[1];    // 2009.09.04 , Joye
}
//---------------------------------------------------------------------------
AnsiString GetAutoHeight(int index)
{
    return DeviceForm_File.IndexContact[index];
}
//---------------------------------------------------------------------------
AnsiString GetContactOffset(int index)
{
    return Offset.iIndexArmContact[index];
}
//---------------------------------------------------------------------------
AnsiString GetContactTestMode()
{
    return DeviceForm.ContactMode;
}
//---------------------------------------------------------------------------
AnsiString GetSecondSpeed()
{
    return DeviceForm_File.DropSpeed;
}
//---------------------------------------------------------------------------
AnsiString GetContactWaitHeight()
{
    return 0;
}
//---------------------------------------------------------------------------
AnsiString GetDropHeight(int index)
{
    return DeviceForm_File.IndexDrop[index];
}
//---------------------------------------------------------------------------
AnsiString GetReleaseWait()
{
    return DeviceForm_File.DropWait;
}
//---------------------------------------------------------------------------
AnsiString GetShuttlePickOffset(int index)
{
    return Offset_File.iIndexArmPickUp[index];
}
//---------------------------------------------------------------------------
AnsiString GetShuttlePickHeight(int index)
{
    return DeviceForm_File.IndexArmPick[index];
}
//---------------------------------------------------------------------------
AnsiString GetShuttleReleaseHeight(int index)
{
    return DeviceForm_File.IndexPlace[index];
}
//---------------------------------------------------------------------------
AnsiString GetArmTotalForce(int index)
{
    if(index==0)
        return DeviceForm.dPress;
    else
        return fContact->edAirForceN->Text;
}
//---------------------------------------------------------------------------
AnsiString GetClinderForce(int index)
{
    if(index==0)
        return fContact->edForcePerDeviceKG->Text;
    else
        return fContact->edForcePerDeviceN->Text;
}
//---------------------------------------------------------------------------
AnsiString GetForcePerDevice(int index)
{
    if(index==0)
        return fContact->edForcePerDeviceKG->Text;
    else
        return fContact->edForcePerDeviceN->Text;
}
//---------------------------------------------------------------------------
AnsiString GetSetNoPerPin()
{
    return fContact->edPinCount->Text;
}
//---------------------------------------------------------------------------
AnsiString GetForcePerPin()
{
    return fContact->edForcePerPinG->Text;
}
//---------------------------------------------------------------------------
AnsiString GetContactForcen()
{
    return fContact->edForcePerPinN->Text;
}
//---------------------------------------------------------------------------
AnsiString GetStartPos(int index)
{
    if(index==0)
        return UserDefForm_File[0].XStart;
    else
        return UserDefForm_File[0].YStart;
}
//---------------------------------------------------------------------------
AnsiString GetDivision(int index)
{
    if(index==0)
        return UserDefForm_File[0].XDivision;
    else
        return UserDefForm_File[0].YDivision;
}
//---------------------------------------------------------------------------
int GetDimemsion(AnsiString *Data)
{
    Data[0]=FormatFloat("0.00",DeviceForm_File.XDimension);;
    Data[1]=FormatFloat("0.00",DeviceForm_File.YDimension);;
    return 2;
}
//---------------------------------------------------------------------------
AnsiString GetZThickness()
{
    return UserDefForm_File[0].ZDepth;
}
//---------------------------------------------------------------------------
AnsiString GetTrayType()
{
    return UserDefForm_File[0].Alias;
}
//---------------------------------------------------------------------------
AnsiString GetPitch(int index)
{
    if(index==0)
        return UserDefForm_File[0].XPitch;
    else
        return UserDefForm_File[0].YPitch;
}
//---------------------------------------------------------------------------
//AnsiString GetIP()
//{
//}
//---------------------------------------------------------------------------
//AnsiString GetPort()
//{
//}
//---------------------------------------------------------------------------
//AnsiString GetCusCode()
//{
//}
//---------------------------------------------------------------------------
bool CheckCanChangeRealDummy()
{
    return fMain->CheckCanChangeRealDummy();
}
//---------------------------------------------------------------------------
void DoClearReportRequest(int iSwitchCase)
{
    int iCase=0;
    if(iSwitchCase==1111)   //全清
    {
        fCounterClear->ClearCount(ctTimeData);
        fCounterClear->ClearCount(ctLoadingCounts);
        fCounterClear->ClearCount(ctTraySortCount);
        fCounterClear->ClearCount(ctContactCounts);
        fCounterClear->ClearCount(ctTesterCategory);
    }
    else
    {
        switch(iSwitchCase)
        {
            case 2: iCase=ctTimeData; break;
            case 3: iCase=ctLoadingCounts; break;
            case 4: iCase=ctTraySortCount; break;
            case 5: iCase=ctContactCounts; break;
            case 6: iCase=ctTesterCategory; break;
//          case 9: iCase=0; break;       //GPIB
        }
        fCounterClear->ClearCount(iCase);
    }
}
//---------------------------------------------------------------------------
int GetCategory(AnsiString *Data)
{
    Data[0]= BinSelect[iTestRunMode].IfErrorT3;
    for(int i=0; i<=iTestBinCount; i++)
    {
        Data[i+1]=BinSelect[iTestRunMode].iCatDataT3Pos[i];
    }
    return 17;
}
//---------------------------------------------------------------------------
int GetBindefine(AnsiString *Data)   //Sam 20230803 : 新增OLP指令
{
//    int iStack=0;
    Data[0]="0";
    for(int i=0; i<iTestBinCount; i++)
    {
        if(Prod.iT6CatData[i]>=0)
        {
            if(Prod.bIsPassBin[i]==1)
                Data[i+1]="1";
             else
                Data[i+1]="0";
        }
        else
        {
            Data[i+1]="0";
        }
    }
    return 17;
}
//---------------------------------------------------------------------------
int GetFixTrayDefine(AnsiString *Data)  //Sam 20230921 : 新增 FixTray 指令  //QQQ
{
    Data[0]=IntToStr(TrayForm.LodareType);
    Data[1]=IntToStr(TrayForm.iFixTrayMode);
    Data[2]=(TrayForm.bTrayUpDownSet[eFix1])?"1":"0";
    Data[3]=(TrayForm.bTrayUpDownSet[eFix2])?"1":"0";
    Data[4]=(TrayForm.bTrayUpDownSet[eFix3])?"1":"0";
    return 5;
}
//---------------------------------------------------------------------------
int GetMapping(AnsiString *Data)
{
    //根據模式選擇
    //TestIF.iTestMode
    int MaxI=0;
    int MaxJ=0;
    /*
    AnsiString asY1Site[]={"Aa:","Ba:","Ca:","Da:","Ea:","Fa:","Ga:","Ha:"};
    AnsiString asY2Site[]={"Aa:","Ba:",
                           "Ab:","Bb:",
                           "Ac:","Bc:",
                           "Ad:","Bd:",
                           "Ae:","Be:",
                           "Af:","Bf:",
                           "Ag:","Bg:",
                           "Ah:","Bh:"};
    AnsiString asY4Site[]={"Aa:","Ba:","Ca:","Da:",
                           "Ab:","Bb:","Cb:","Db:",
                           "Ac:","Bc:","Cc:","Dc:",
                           "Ad:","Bd:","Cd:","Dd:",
                           "Ae:","Be:","Ce:","De:",
                           "Af:","Bf:","Cf:","Df:",
                           "Ag:","Bg:","Cg:","Dg:",
                           "Ah:","Bh:","Ch:","Dh:"};
    */
    AnsiString asSite[]={"A:","B:","C:","D:",
                         "E:","F:","G:","H:",
                         "I:","J:","K:","L:",
                         "M:","N:","O:","P:",
                         "Q:","R:","S:","T:",
                         "U:","V:","W:","X:",
                         "Y:","Z:","0:","1:",
                         "2:","3:","4:","5:"};
    switch(TestIF.iTestMode)
    {
        case SingleSite://1x1
             MaxI=1;
             MaxJ=1;
             break;
        case DualSite://1x2
             MaxI=1;
             MaxJ=2;
             break;
        case TriSite1X3:
             MaxI=1;
             MaxJ=3;
             break;
        case QualSite1X4://1x4
        case _8Site1X4:         //ChungHung 20150528 add for 海思 _8Site1x4
             MaxI=1;
             MaxJ=4;
             break;
        case DualSite2x1:
             MaxI=2;
             MaxJ=1;
             break;
        case QualSite2X2://2x2
        case QualSite2X2N:      //Frank 20200520 2X2NN Mode
             MaxI=2;
             MaxJ=2;
             break;
        case _6Site2X3:
        case _6Site2X3N:        //Steven 20220425 : 2X3NN Mode
             MaxI=2;
             MaxJ=3;
             break;
        case _8Site2X4://2x4
        case _8Site2X4N:        //Wei 20231211 : 2X4NN Mode
             MaxI=2;
             MaxJ=4;
             break;
        case _10Site2X5:        //wei 20190614 10 site
             MaxI=2;
             MaxJ=5;
             break;
        case _12Site2X6:
             MaxI=2;
             MaxJ=6;
             break;
        case _16Site2X8://2x8
             MaxI=2;
             MaxJ=8;
             break;
        case _16Site4X4:
             MaxI=4;
             MaxJ=4;
             break;
        case _32Site4X8N:
             MaxI=4;
             MaxJ=8;
             break;
        case _32Site4X8M:
             MaxI=4;
             MaxJ=8;
             break;
    };

    //AnsiString KeyWord[16]={ "Dut1=" ,"Dut2=" ,"Dut3=" ,"Dut4=" ,"Dut5=" ,"Dut6=" ,"Dut7=" ,"Dut8=" ,
    //                         "Dut9=" ,"Dut10=" ,"Dut11=" ,"Dut12=" ,"Dut13=" ,"Dut14=" ,"Dut15=" ,"Dut16=" };
    //AnsiString Position[17]={ "---","Ch1" ,"Ch2" , "Ch3", "Ch4", "Ch5", "Ch6", "Ch7", "Ch8",
    //                          "Ch9", "Ch10", "Ch11", "Ch12", "Ch13", "Ch14", "Ch15", "Ch16" };

    if(CUSTOMER_CODE==CC_MTI || CUSTOMER_CODE==CC_PTI)
    {
        Data[0]=IntToStr(MaxJ);  //Sam 20200414 : 加入 MaxI & MaxJ
        Data[1]=IntToStr(MaxI);  //Sam 20200414 : 加入 MaxI & MaxJ
        for(int j=0; j<MaxJ; j++)
        {
            for(int i=0; i<MaxI; i++)
            {
                /*
                if(MaxI==1)
                {
                    Data[j*MaxI+i]=asY1Site[j*MaxI+i]+TestIF.iSiteMap[i][j];
                }
                else if(MaxI==2)
                {
                    Data[j*MaxI+i]=asY2Site[j*MaxI+i]+TestIF.iSiteMap[i][j];
                }
                else
                {
                    Data[j*MaxI+i]=asY4Site[j*MaxI+i]+TestIF.iSiteMap[i][j];
                }*/
                Data[2+j*MaxI+i]=asSite[j*MaxI+i]+TestIF.iSiteMap[i][j]; //Sam 20200414 : 加入 MaxI & MaxJ
            }
        }
        return 2+(MaxI*MaxJ);    //Sam 20200414 : 加入 MaxI & MaxJ
    }
    else
    {
        for(int i=0; i<MaxI; i++)
        {
            for(int j=0; j<MaxJ; j++)
            {
                Data[i*MaxJ+j] = TestIF.iSiteMap[i][j];
            }
        }
        return MaxI*MaxJ;
    }
}
//---------------------------------------------------------------------------
int GetDutOnOff(AnsiString *Data)
{
    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            if(bUseTwoArm32Site==true)
                Data[i*TestSocket.iShtCol+j]=(LastSet.bUseTestSocket[0][i][j]==true)?1:0;
            else
                Data[i*TestSocket.iShtCol+j]=(LastSet.bUseTestSocket[0][i][j]==true || LastSet.bUseTestSocket[1][i][j]==true)?1:0;
        }
    }
    return TestSocket.iShtCnt;
}
//---------------------------------------------------------------------------
int GetProductivity(AnsiString *Data)
{
    if(CUSTOMER_CODE==CC_MTI || CUSTOMER_CODE==CC_PTI)
    {
        Data[0]=fObserver->labModel->Caption;
        Data[1]=fObserver->labPowerOnTime->Caption;
        Data[2]=fObserver->labRunningTime->Caption;
        Data[3]=fObserver->labProductTime->Caption;
        Data[4]=fObserver->labLoadingCount->Caption;
        Data[5]=fObserver->labMUBA->Caption;                                    //MUBF
        Data[6]=fObserver->labMTBA->Caption;                                    //MTBF
    }
    else
    {
        Data[0]="0";                                                            //Down Time Rate
        Data[1]=fObserver->labMUBA->Caption;                                    //MUBF
        Data[2]=fObserver->labMTBA->Caption;                                    //MTBF
        Data[3]="0";
    }
    return 0;                                                                   //MTTR
}
//---------------------------------------------------------------------------
int GetSortingCount(AnsiString *Data)
{
    Data[0]=AnsiString(LastSet.BinCT[0][e3Auto1]);
    Data[1]=AnsiString(LastSet.BinCT[0][e3Auto2]);
    Data[2]=AnsiString(LastSet.BinCT[0][e3Auto3]);
    Data[3]=AnsiString(LastSet.BinCT[0][e3Fix1 ]);
    Data[4]=AnsiString(LastSet.BinCT[0][e3Fix2 ]);
    Data[5]=AnsiString(LastSet.BinCT[0][e3Fix3 ]);
    if(TrayForm.iFixTrayMode==0)
    {
        Data[6]=0;
        Data[7]=0;
        Data[8]=0;
    }
    else
    {
        Data[6]=AnsiString(LastSet.BinCT[0][e3Fix4]);
        Data[7]=AnsiString(LastSet.BinCT[0][e3Fix5]);
        Data[8]=AnsiString(LastSet.BinCT[0][e3Fix6]);
    }
    return 0;
}
//---------------------------------------------------------------------------
int GetSocketCount(AnsiString *Data)
{
    if(CUSTOMER_CODE==CC_MTI || CUSTOMER_CODE==CC_PTI)
    {
        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                Data[j+i*TestSocket.iShtCol]=ArmData[0]->ArmSKET[i][j]->GetTotal()+ArmData[1]->ArmSKET[i][j]->GetTotal();
            }
        }
        return TestSocket.iShtCnt;
    }
    else
    {
        //HT9045,最多8個Socket
        // Data Format : Bin0,Bin1,Bin2,Bin3,Bin4,Bin5,Bin6,Bin7,Bin8,Bin9,Bin10,Bin11,Bin12,Bin13,Bin14,Bin15,SKIP,TOTAL,PASS
        // 順序: Aa, Ab, Ac, Ad, Ba, Bb, Bc, Bd
        int iIFErrTotal=0, iGategoryTotal[TEST_MAX_BIN]={0}, i_Total=0, i_Pass_Total=0;   //Steven 20121112 : RS232支援32Bin 15 --> TEST_MAX_BIN
        int DutCount=MAX_SOCKET_TOTAL;
        for(int i=0; i<DutCount; i++)
        {
            int j=i/NEW_MAX_Index_Col, k=i%NEW_MAX_Index_Col;

            // Bin 0 Data
            Data[i]="0,";

            // Bin 1 ~ Bin15
            for(int l=0; l<iTestBinCount; l++)                                  //Steven 20121112 : RS232支援32Bin 15 --> iTestBinCount
            {
                Data[i]+=AnsiString(ArmData[0]->ArmSKET[j][k]->GetSelBinCT(l)+ArmData[1]->ArmSKET[j][k]->GetSelBinCT(l))+",";
                iGategoryTotal[l]+=(ArmData[0]->ArmSKET[j][k]->GetSelBinCT(l)+ArmData[1]->ArmSKET[j][k]->GetSelBinCT(l));
            }

            //I/F Error
            Data[i]+=AnsiString(ArmData[0]->ArmSKET[j][k]->GetIFError()+ArmData[1]->ArmSKET[j][k]->GetIFError())+",";
            //Total
            Data[i]+=AnsiString(ArmData[0]->ArmSKET[j][k]->GetTotal()+ArmData[1]->ArmSKET[j][k]->GetTotal())+",";
            //Pass
            Data[i]+=AnsiString(ArmData[0]->ArmSKET[j][k]->GetPassCT()+ArmData[0]->ArmSKET[j][k]->GetPassCT());
            i_Pass_Total+=(ArmData[0]->ArmSKET[j][k]->GetPassCT()+ArmData[1]->ArmSKET[j][k]->GetPassCT());
            iIFErrTotal+=(ArmData[0]->ArmSKET[j][k]->GetIFError()+ArmData[1]->ArmSKET[j][k]->GetIFError());
        }
        Data[DutCount]="0";

        for(int i=0; i<iTestBinCount; i++)                                      //Steven 20121112 : RS232支援32Bin 15 --> iTestBinCount
        {
            Data[DutCount]+=","+AnsiString(iGategoryTotal[i]);
            i_Total+=iGategoryTotal[i];
        }
        Data[DutCount]+=","+AnsiString(iIFErrTotal)+","+AnsiString(i_Total)+","+AnsiString(i_Pass_Total);

        return DutCount+1;
    }
}
//---------------------------------------------------------------------------
int GetHeadCount(AnsiString *Data)
{
    if(CUSTOMER_CODE==CC_MTI || CUSTOMER_CODE==CC_PTI)
    {
        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                Data[j+i*TestSocket.iShtCol]                                      =ArmData[0]->ArmSKET[i][j]->GetTotal();
                Data[j+i*TestSocket.iShtCol+TestSocket.iShtCnt]=ArmData[1]->ArmSKET[i][j]->GetTotal();
            }
        }
        return (TestSocket.iShtCnt)*2;
    }
    else
    {
        int iIFErrTotal=0, iGategoryTotal[TEST_MAX_BIN]={0}, i_Total=0, i_Pass_Total=0;       //Steven 20121112 : RS232支援32Bin 15 --> TEST_MAX_BIN
        // Data Format : Bin0,Bin1,Bin2,Bin3,Bin4,Bin5,Bin6,Bin7,Bin8,Bin9,Bin10,Bin11,Bin12,Bin13,Bin14,Bin15,SKIP,TOTAL,PASS
        // 順序: Aa, Ab, Ac, Ad, Ba, Bb, Bc, Bd 先Arm1再Arm2
        // 尚未加入9046的支援
        for(int x=0; x<2; x++)
        {
            for(int i=0; i<NEW_MAX_Index_Col; i++)
            {
                int j=i/NEW_MAX_Index_Col, k=i%NEW_MAX_Index_Col, y=x*8+i;

                // Bin 0 Data
                Data[y]="0,";
                iIFErrTotal+=ArmData[x]->ArmSKET[j][k]->GetIFError();//category total->IFErr

                // Bin 1 ~ Bin15
                for(int l=0; l<iTestBinCount; l++)     //Steven 20121112 : RS232支援32Bin 15 --> iTestBinCount
                {
                    Data[y]+=AnsiString(ArmData[x]->ArmSKET[j][k]->GetSelBinCT(l))+",";
                    iGategoryTotal[l]+=ArmData[x]->ArmSKET[j][k]->GetSelBinCT(l);
                }

                //I/F Error
                Data[y]+=AnsiString(ArmData[x]->ArmSKET[j][k]->GetIFError())+",";
                //Total
                Data[y]+=AnsiString(ArmData[x]->ArmSKET[j][k]->GetTotal())+",";
                //Pass
                Data[y]+=AnsiString(ArmData[x]->ArmSKET[j][k]->GetPassCT());
                i_Pass_Total+=ArmData[x]->ArmSKET[j][k]->GetPassCT();
            }
        }
        Data[iTestBinCount]="0";
        for(int i=0; i<iTestBinCount; i++)
        {
            Data[iTestBinCount]+=","+AnsiString(iGategoryTotal[i]);
            i_Total+=iGategoryTotal[i];
        }
        Data[iTestBinCount]+=","+AnsiString(iIFErrTotal)+","+AnsiString(i_Total)+","+AnsiString(i_Pass_Total);

        return iTestBinCount+2;
    }
}
//---------------------------------------------------------------------------
int GetTestResult(AnsiString *Data)
{
    if(CUSTOMER_CODE==CC_MTI || CUSTOMER_CODE==CC_PTI)
    {
        for(int i=0; i<TestSocket.iShtCnt; i++)
        {
            Data[i]=TestIF.testBin[i];
        }
        return TestSocket.iShtCnt;
    }
    else
    {
        for(int i=0; i<MAX_Index_Row; i++)
        {
            for(int j=0; j<NEW_MAX_Index_Col; j++)
            {
                if(TestSocket.Item[i][j]!=NULL_IC && TestSocket.Item[i][j]!=HAS_NULL_IC)
                    Data[i*NEW_MAX_Index_Col+j].sprintf("Dut%d=Bin%d", TestIF.iSiteMap[i][j], (TestSocket.Item[i][j]-TEST_PASS+1));
                else
                    Data[i*NEW_MAX_Index_Col+j].sprintf("Dut%d=Bin%d", TestIF.iSiteMap[i][j], 0);
            }
        }
        return MAX_Index_Row*NEW_MAX_Index_Col;
    }
}
//---------------------------------------------------------------------------
int GetHandMode(AnsiString *Data)
{
    Data[0]=TestIF.iTestMode;

    return 1;
}
//---------------------------------------------------------------------------
int GetTrayForm(AnsiString *Data)
{
    int iType=0;

    Data[0]=AnsiString(UserDefForm[iType].XDivision);
    Data[1]=AnsiString(UserDefForm[iType].YDivision);
    Data[2]=AnsiString(UserDefForm[iType].XStart);
    Data[3]=AnsiString(UserDefForm[iType].YStart);
    Data[4]=AnsiString(UserDefForm[iType].XPitch);
    Data[5]=AnsiString(UserDefForm[iType].YPitch);
    return 6;
}
//---------------------------------------------------------------------------
int GetPlateForm(AnsiString *Data)
{
    Data[0]=AnsiString(HotPlateForm.XDivision);
    Data[1]=AnsiString(HotPlateForm.YDivision);
    Data[2]=AnsiString(HotPlateForm.XStart);
    Data[3]=AnsiString(HotPlateForm.YStart);
    Data[4]=AnsiString(HotPlateForm.XPitch);
    Data[5]=AnsiString(HotPlateForm.YPitch);

    return 6;
}
//---------------------------------------------------------------------------
void DoOnLineReply(AnsiString *Data)
{
    Data[0]=IniConfig.sMachineType;
    if(CUSTOMER_CODE==CC_ASE_CL)                                                //Steven 20110902
    {
        Data[1]=AnsiString(PC_NAME);
    }
    else
    {
        Data[1]=IniConfig.SocketHandlerID;
    }

    Data[2]=fObserver->Memo1->Lines->Strings[0];                                //軟體版本
    Data[3]="Ready";
    Data[4]="0";
}
//---------------------------------------------------------------------------
void AlarmMessage(AnsiString enMess, AnsiString chMess)
{
    ShowMyMessage(enMess, chMess);
}
//---------------------------------------------------------------------------
void DoULRequest(AnsiString *Data)                                              //上傳檔案到Host
{
    SetCurrentDirectory(_T("D://"));

    AnsiString str1;
    AnsiString sULFileName=fMain->cbSetupFileName->Text;
    char ddd[10];
    FILE *fs;

    //先刪除舊的壓縮檔
    str1.sprintf("del %s%s.zip", DataPath, sULFileName);
    system(str1.c_str());
    MySleep(50);

    //把7Zip準備好
    if(FileExists( "d:\\HT9045\\7z.exe" ) == false)
    {
        CopyFile( "C:\\Program Files\\7-Zip\\7z.exe" , "d:\\HT9045\\7z.exe" , FALSE);
    }

    //把工作檔壓縮
    str1.sprintf("d:\\HT9045\\7z.exe a -tzip \"%s%s.zip\" \"%s%s\\*.*\"", DataPath, sULFileName, DataPath, sULFileName);
    system(str1.c_str());

    str1="";
    int j=0;
    int size=0;
    char ccc;
    str1.sprintf("%s%s.zip", DataPath, sULFileName);
    fs=fopen(str1.c_str(), "rb");
    //記錄檔案大小
    if(fs!=NULL)    // 2009.07.30 , Joye
    {
        fseek(fs, 0, SEEK_END);
        size=ftell(fs);
    }
    fclose(fs);

    //將工作檔的壓縮檔進行轉換成要傳輸的格式
    fs=fopen(str1.c_str(), "rb");
    if(fs!=NULL)    // 2009.07.30 , Joye
    {
        str1="";
        while(j<=size)
        {
            j++;
            if(fscanf(fs,"%1c",&ccc))
            {
                sprintf(ddd,"%02X",ccc);
                if(ddd[0]=='F' && ddd[1]=='F' && ddd[2]=='F')
                {
                    ddd[0]=ddd[6];
                    ddd[1]=ddd[7];
                    ddd[2]='\0';
                }
                str1=str1+ddd;
            }
            //str1=str1+s;
        }
    }
    fclose(fs); // 2009.08.04

    str1.Delete(str1.Length()-2, 2);
    Data[0]=sULFileName+".zip";
    Data[1]=str1.Length();
    Data[2]=str1;
    Data[3]="0";
}
//---------------------------------------------------------------------------
int DoDLRequest(AnsiString sDLFileName)     //下載檔案,並切換工作檔
{
    SetCurrentDirectory(_T("D://"));

    bool bNeedCover=false;

    AnsiString str1, str2;
    AnsiString sConfigPath=AuthPath+"Security_new.def";
    AnsiString SPath[2]={DataPath, OffsetPath};
    int ret=0;//, ret2=0;
    bool bHasSetUpFile=false;

    //----------------------
    //首先判斷工作檔有沒有
    //----------------------
    if(!DirectoryExists(SPath[0]+sDLFileName))
    {
        MyForceDirectories(SPath[0]+sDLFileName);
        bHasSetUpFile=false;
    }
    else
    {
        bHasSetUpFile=true;
    }

    if(FileExists("d:\\HT9045\\7z.exe")==false)
    {
        CopyFile("C:\\Program Files\\7-Zip\\7z.exe", "d:\\HT9045\\7z.exe", false);
    }

    if(bHasSetUpFile==false)                                                    //沒有的話,就先建立新的資料夾
    {
        for(int i=1; i<2; i++)
        {
            MyForceDirectories(SPath[i]+sDLFileName);
        }
        fMain->cbSetupFileName->Items->Add(sDLFileName);
    }

    //----------------------
        //檢查Offset檔
    //----------------------
    str1.sprintf("%s%s\\Position Offset.Data", OffsetPath, sDLFileName);
    if(!FileExists(str1))    //如果沒有就從當下的工作檔複製過來
    {
        str2.sprintf("%s%s\\Position Offset.Data", OffsetPath, GetLastOpenFN());
        CopyFile(str2.c_str(), str1.c_str(), false);
    }

    str1.sprintf("\"%s%s\\Position Offset Hot.Data\"", OffsetPath, sDLFileName);
    if(!FileExists(str1))
    {
        str2.sprintf("\"%s%s\\Position Offset Hot.Data\"",  OffsetPath, GetLastOpenFN());
        CopyFile(str2.c_str(), str1.c_str(), false);
    }
    MySleep(50);

    //----------------------
    //把要還原的資料先備份
    //----------------------
    double fTempUserOffset[10], fContactHeight[19];
    int iShuttleMode[2];
    int iIndexHeatingMode;

    if(bHasSetUpFile)   //前提是該資料夾已經有資料了
    {
        //----------------------
        //把溫度Offset資料備份
        //----------------------
        str1.sprintf("%s%s\\Temperature.Data", DataPath, sDLFileName);
        for(int i=0; i<10; i++)
        {
            str2.printf("CH%d", i+1);
            fTempUserOffset[i]=ReadIniData(str1, "User OffSet", str2, 0.0);
        }
        iIndexHeatingMode=CheckAndReadIniData(str1, "Index",   "Heating Mode",   0);    //Steven 20180420 (Jou) : JCET吳如春說不覆蓋Index加熱模式
        MySleep(50);

        //----------------------
        //把HotPlate資料備份
        //----------------------
        str1.sprintf("%s%sHotPlate.Data", DataPath, sDLFileName);
        str2.sprintf("%sHotPlate.Data", DataPath);
        ret=CopyFile(str1.c_str(), str2.c_str(), false);
        MySleep(50);

        //----------------------
        //把Contact高度備份
        //----------------------
        str1.sprintf("%s%s\\Contact.Data", DataPath, sDLFileName);
        fContactHeight[0]=ReadIniData(str1, "Test Arm1", "Pick Up", 0.0);
        fContactHeight[1]=ReadIniData(str1, "Test Arm1", "Contact", 0.0);
        fContactHeight[2]=ReadIniData(str1, "Test Arm1", "Drop",    0.0);
        fContactHeight[3]=ReadIniData(str1, "Test Arm1", "Place",   0.0);
        fContactHeight[4]=ReadIniData(str1, "Test Arm2", "Pick Up", 0.0);
        fContactHeight[5]=ReadIniData(str1, "Test Arm2", "Contact", 0.0);
        fContactHeight[6]=ReadIniData(str1, "Test Arm2", "Drop",    0.0);
        fContactHeight[7]=ReadIniData(str1, "Test Arm2", "Place",   0.0);
        fContactHeight[8]=ReadIniData(str1, "Torque Control", "Pin Number",    0.0);
        fContactHeight[9]=ReadIniData(str1, "Torque Control", "Force Per Pin", 0.0);

        fContactHeight[10]=ReadIniData(str1, "Wait Time", "Drop Wait",   1.0);
        fContactHeight[11]=ReadIniData(str1, "Wait Time", "Drop Speed",  1.0);
        fContactHeight[12]=ReadIniData(str1, "Mode", "Contact",                          0.0);
        fContactHeight[13]=ReadIniData(str1, "Mode", "Vacuum",                           0.0);
        fContactHeight[14]=ReadIniData(str1, "Mode", "Dummy Contact",                    0.0);
        fContactHeight[15]=ReadIniData(str1, "Mode", "Head Device Mode",                 0.0);
        fContactHeight[16]=ReadIniData(str1, "Mode", "Kit Diameter",                     3.0);
        fContactHeight[17]=ReadIniData(str1, "Mode", "Suck Shuttle Device After Tested", 0.0);
        fContactHeight[18]=ReadIniData(str1, "Mode", "Shuttle Waiting Out Site Chamber", 0.0);

        //----------------------
        //把Ld/Uld速度備份
        //----------------------
        str1.sprintf("%s%s\\UdUld.Data", DataPath, sDLFileName);
        str2.sprintf("%sUdUld.Data", DataPath);
        ret=CopyFile(str1.c_str(), str2.c_str(), false);
        MySleep(50);

        //----------------------
        //把速度設定備份
        //----------------------
        str1.sprintf("%s%s\\ArmCondition.Data", DataPath, sDLFileName);
        str2.sprintf("%s\\ArmCondition.Data", DataPath);
        ret=CopyFile(str1.c_str(), str2.c_str(), false);
        MySleep(50);

        //----------------------
        //備份開啟單一Shuttle選項
        //----------------------
        str1.sprintf("%s%s\\HandlerCondition.Data", DataPath, sDLFileName);
        iShuttleMode[0]=ReadIniData(str1, "Configuration", "Shuttle Mode",    0);
        iShuttleMode[1]=ReadIniData(str1, "Configuration", "Shuttle1 Cancel", 0);
    }

    //----------------------
    //解壓縮檔案並覆蓋到原本的資料夾
    //----------------------
    if(FileExists(DataPath+sDLFileName+".zip"))     //Steven 20110603
    {
        str1="d:\\HT9045\\7z.exe e \""+DataPath+sDLFileName+".zip\" -o\""+DataPath+sDLFileName+"\\\" -y";
        ret=system(str1.c_str());
        MySleep(100);
    }
    else
    {
        ShowErrorMessage("WAR1684", K_RETRY|K_SKIP, MMSystem, 0, sDLFileName);     //下載 %s.zip 失敗
        return false;
    }

    if(ret==0)  //解壓縮成功的話
    {
        //----------------------
        //把壓縮檔砍掉
        //----------------------
        str1.sprintf("del \"%s%s.zip\"", DataPath, sDLFileName);
        system(str1.c_str());
        MySleep(50);

        if(bHasSetUpFile) //如果不是新的工作檔就要進行資料還原
        {
            //----------------------
            //不覆蓋就是必須要還原
            // 0 : 不覆蓋
            // 1 : 要覆蓋
            //----------------------

            //----------------------
            //把溫度Offset資料還原
            //----------------------
            bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Temp OffSet", true);
            if(bNeedCover==false)   //不覆蓋就要還原
            {
                str1.sprintf("%s%s\\Temperature.Data", DataPath, sDLFileName);
                for(int i=0; i<10; i++)
                {
                    str2.printf("CH%d", i+1);
                    WriteIniData(str1, "User OffSet", str2, fTempUserOffset[i]);
                }
            }

            bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Index Heat Mode", true);  //Steven 20180420 (Jou) : JCET吳如春說不覆蓋Index加熱模式
            if(bNeedCover==false)
            {
                str1.sprintf("%s%s\\Temperature.Data", DataPath, sDLFileName);
                WriteIniData(str1, "Index",   "Heating Mode",   iIndexHeatingMode);
            }

            //----------------------
            //把Contact高度還原
            //----------------------
            bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Test Arm", false);  //預設不覆蓋
            if(bNeedCover==false)   //不覆蓋就要還原
            {
                str1.sprintf("%s%s\\Contact.Data", DataPath, sDLFileName);
                WriteIniData(str1, "Test Arm1", "Pick Up", fContactHeight[0]);
                WriteIniData(str1, "Test Arm1", "Contact", fContactHeight[1]);
                WriteIniData(str1, "Test Arm1", "Drop",    fContactHeight[2]);
                WriteIniData(str1, "Test Arm1", "Place",   fContactHeight[3]);
                WriteIniData(str1, "Test Arm2", "Pick Up", fContactHeight[4]);
                WriteIniData(str1, "Test Arm2", "Contact", fContactHeight[5]);
                WriteIniData(str1, "Test Arm2", "Drop",    fContactHeight[6]);
                WriteIniData(str1, "Test Arm2", "Place",   fContactHeight[7]);
            }

            bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Torque Control", true);
            if(bNeedCover==false)   //不覆蓋就要還原
            {
                str1.sprintf("%s%s\\Contact.Data", DataPath, sDLFileName);
                WriteIniData(str1, "Torque Control", "Pin Number",    fContactHeight[8]);
                WriteIniData(str1, "Torque Control", "Force Per Pin", fContactHeight[9]);
            }

            bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Contact Mode", true);
            if(bNeedCover==false)   //不覆蓋就要還原
            {
                str1.sprintf("%s%s\\Contact.Data", DataPath, sDLFileName);
                WriteIniData(str1, "Wait Time", "Drop Wait",                    fContactHeight[10]);
                WriteIniData(str1, "Wait Time", "Drop Speed",                   fContactHeight[11]);
                WriteIniData(str1, "Mode", "Contact",                           fContactHeight[12]);
                WriteIniData(str1, "Mode", "Vacuum",                            fContactHeight[13]);
                WriteIniData(str1, "Mode", "Dummy Contact",                     fContactHeight[14]);
                WriteIniData(str1, "Mode", "Head Device Mode",                  fContactHeight[15]);
                WriteIniData(str1, "Mode", "Kit Diameter",                      fContactHeight[16]);
                WriteIniData(str1, "Mode", "Suck Shuttle Device After Tested",  fContactHeight[17]);
                WriteIniData(str1, "Mode", "Shuttle Waiting Out Site Chamber",  fContactHeight[18]);
            }

            //----------------------
            //把HotPlate資料還原
            //----------------------
            bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "HotPlate", true);
            if(bNeedCover==false)   //不覆蓋就要還原
            {
                str1.sprintf("%sHotPlate.Data", DataPath);
                str2.sprintf("%s%s\\HotPlate.Data", DataPath, sDLFileName);
                ret=CopyFile(str1.c_str(), str2.c_str(), false);
                MySleep(50);
            }

            str1.sprintf("del \"%sHotPlate.Data\"", DataPath);    //刪除備份
            system(str1.c_str());
            MySleep(50);

            //----------------------
            //把Ld/Uld速度還原
            //----------------------
            bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Load Unload", true);
            if(bNeedCover==false)   //不覆蓋就要還原
            {
                str1.sprintf("%sUdUld.Data", DataPath);
                str2.sprintf("%s%s\\UdUld.Data", DataPath, sDLFileName);
                ret=CopyFile(str1.c_str(), str2.c_str(), false);
                MySleep(50);
            }

            str1.sprintf("del \"%sUdUld.Data\"", DataPath);    //刪除備份
            system(str1.c_str());
            MySleep(50);

            //----------------------
            //把速度設定備份
            //----------------------
            bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "ArmCondition", true);
            if(bNeedCover==false)   //不覆蓋就要還原
            {
                str1.sprintf("%sArmCondition.Data", DataPath);
                str2.sprintf("%s%s\\ArmCondition.Data", DataPath, sDLFileName);
                ret=CopyFile(str1.c_str(), str2.c_str(), false);
                MySleep(50);
            }

            str1.sprintf("del \"%sArmCondition.Data\"", DataPath); //刪除備份
            system(str1.c_str());
            MySleep(50);

            //----------------------
            //備份開啟單一Shuttle選項
            //----------------------
            bNeedCover=CheckAndReadIniData(sConfigPath, "Network", "Shuttle Mode", true);
            if(bNeedCover==false)    //不覆蓋就要還原
            {
                str1.sprintf("%s%s\\HandlerCondition.Data", DataPath, sDLFileName);
                WriteIniData(str1, "Configuration", "Shuttle Mode",    iShuttleMode[0]);
                WriteIniData(str1, "Configuration", "Shuttle1 Cancel", iShuttleMode[1]);
            }
        }

//        ShowErrorMessage("MES1687", 0, MMSystem, 0, sDLFileName); //UnZip %s.zip OK.
//        return true;
    }
    else
    {
        ShowErrorMessage("WAR1686", 0, MMSystem, 0, sDLFileName); //UnZip %s.zip Fail.
        return false;
    }

    bFTPDownloadSetupFile = true;       // 2013.12.03 , Joye , KYEC FTP Download

    //ChungHung 20110906 add
    if(ret==0)
        ret=1;
    else
        ret=0;

    return ret;
}
//---------------------------------------------------------------------------
#ifdef DEBUG_DUTONOFF
void DoHomeAndStart()
{
    bHomeByStart=true;
    fMain->Home("DoHomeAndStart");
}
#endif
//---------------------------------------------------------------------------
void DoOneCycle()
{
    InitOneCycle("DoOneCycle");
    fMain->BtnOneCycle->Down=true;
}
//---------------------------------------------------------------------------
void AmkorUDPGetSiteAndTemp()         //Steven 20120330 : ATK Site Map Monitorning
{
//1Byte         1Byte        1Byte          4Byte      N     1Byte
//Packet Count, Packet Type, Handler Model, Data Size, Data, Check Sum
//Packet Type Define : SITE MAP=0x01, TEMPERATURE=0x02
//Handler Model :
//Check Sum : Packet Count ~ Data N 使用 XOR

//Site MAP Data: Aa Site Num, Aa Enable/Disable, Ab Site Num... Ba Site Num, Ba Enable/Disable...
//Temperature Info Data: Temperature Mode(1Byte), Temperature(1Byte), Soak Time(2Byte)
//Temperature Mode: High=0x00, Ambient=0x01

    AnsiString SendString;
//    char DataTempMode;
    int BCC;

    int MaxI=0;
    int MaxJ=0;
    switch(TestIF.iTestMode)
    {
      case DualSite://1x2
           MaxI=1;
           MaxJ=2;
           break;
      case SingleSite://1x1
           MaxI=1;
           MaxJ=1;
           break;
      case QualSite1X4://1x4
      case _8Site1X4:  //ChungHung 20150528 add for 海思 _8Site1x4
           MaxI=1;
           MaxJ=4;
           break;
      case QualSite2X2://2x2
           MaxI=2;
           MaxJ=2;
           break;
      case _16Site4X4:  //Sam 20190226 : 16Site4X4
      case _8Site2X4://2x4
           MaxI=2;
           MaxJ=4;
           break;
      case _16Site2X8://2x8
           MaxI=2;
           MaxJ=8;
           break;
    };

//    if(LastSet.iTemperature==Tempture_Hot)
//        DataTempMode=0;
//    else
//        DataTempMode=1;

    fAutomation->sATKSendMessage[ 0]=2;                            //count     =2
    fAutomation->sATKSendMessage[ 1]=2;                            //Type      =Temperature
    fAutomation->sATKSendMessage[ 2]=10;                           //Model     =HT9045WH
    fAutomation->sATKSendMessage[ 3]=4;                            //DataSize  =4
    fAutomation->sATKSendMessage[ 4]=0;
    fAutomation->sATKSendMessage[ 5]=0;
    fAutomation->sATKSendMessage[ 6]=0;
    fAutomation->sATKSendMessage[ 7]=(LastSet.iTemperature==Tempture_Hot)?0:1;
    fAutomation->sATKSendMessage[ 8]=int(Temperature.fWorkTemperBase);
    fAutomation->sATKSendMessage[ 9]=int(Temperature.fSoakTime)%256;
    fAutomation->sATKSendMessage[10]=int(Temperature.fSoakTime)/256;
    fAutomation->sATKSendMessage[11]=1;                            //Type      =SiteMap
    fAutomation->sATKSendMessage[12]=10;                           //Model     =HT9045WH
    fAutomation->sATKSendMessage[13]=MaxI*MaxJ*2;
    fAutomation->sATKSendMessage[14]=0;
    fAutomation->sATKSendMessage[15]=0;
    fAutomation->sATKSendMessage[16]=0;
    fAutomation->sATKSendMessageCount=17;
    for(int i=0; i<MaxI; i++)
    {
        for(int j=0; j<MaxJ; j++)
        {
            fAutomation->sATKSendMessage[fAutomation->sATKSendMessageCount]=TestIF_File.iSiteMap[i][j];
            fAutomation->sATKSendMessageCount++;
            fAutomation->sATKSendMessage[fAutomation->sATKSendMessageCount]=LastSet.bUseTestSocket[0][i][j];
            fAutomation->sATKSendMessageCount++;
        }
    }
    BCC=fAutomation->sATKSendMessage[0];

    for(int i=1; i<fAutomation->sATKSendMessageCount; i++)
    {
        BCC^=fAutomation->sATKSendMessage[i];
    }
    fAutomation->sATKSendMessage[fAutomation->sATKSendMessageCount]=BCC;
    fAutomation->sATKSendMessageCount++;
    fAutomation->sATKSendMessage[fAutomation->sATKSendMessageCount]='\0';
}
//---------------------------------------------------------------------------
void AmkorUDPGetSite()         //Steven 20120330 : ATK Site Map Monitorning
{
//1Byte         1Byte        1Byte          4Byte      N     1Byte
//Packet Count, Packet Type, Handler Model, Data Size, Data, Check Sum
//Packet Type Define : SITE MAP=0x01, TEMPERATURE=0x02
//Handler Model :
//Check Sum : Packet Count ~ Data N 使用 XOR

//Site MAP Data: Aa Site Num, Aa Enable/Disable, Ab Site Num... Ba Site Num, Ba Enable/Disable...
//Temperature Info Data: Temperature Mode(1Byte), Temperature(1Byte), Soak Time(2Byte)
//Temperature Mode: High=0x00, Ambient=0x01

    AnsiString SendString;
//    char DataTempMode;
    int BCC;

    int MaxI=2;
    int MaxJ=8; //ChungHung 20140514 alter 4--->8
    /*
    switch(TestIF.iTestMode)
    {
      case DualSite://1x2
           MaxI=1;
           MaxJ=2;
           break;
      case SingleSite://1x1
           MaxI=1;
           MaxJ=1;
           break;
      case QualSite1X4://1x4
           MaxI=1;
           MaxJ=4;
           break;
      case QualSite2X2://2x2
           MaxI=2;
           MaxJ=2;
           break;
      case _8Site2X4://2x4
           MaxI=2;
           MaxJ=4;
           break;
      case _16Site2X8://2x8
           MaxI=2;
           MaxJ=8;
           break;
    };
    */
//    if(LastSet.iTemperature==Tempture_Hot)
//        DataTempMode=0;
//    else
//        DataTempMode=1;

    fAutomation->sATKSendMessage[0]=1;                            //count     =1
    fAutomation->sATKSendMessage[1]=1;                            //Type      =SiteMap
    fAutomation->sATKSendMessage[2]=10;                           //Model     =HT9045WH
    fAutomation->sATKSendMessage[3]=MaxI*MaxJ*2;
    fAutomation->sATKSendMessage[4]=0;
    fAutomation->sATKSendMessage[5]=0;
    fAutomation->sATKSendMessage[6]=0;
    fAutomation->sATKSendMessageCount=7;
    for(int i=0; i<MaxI; i++)
    {
        for(int j=0; j<MaxJ; j++)
        {
            if(TestIF_File.iSiteMap[i][j]!=-1)
            {
                fAutomation->sATKSendMessage[fAutomation->sATKSendMessageCount]=TestIF_File.iSiteMap[i][j];
                fAutomation->sATKSendMessageCount++;
                fAutomation->sATKSendMessage[fAutomation->sATKSendMessageCount]=LastSet.bUseTestSocket[0][i][j];
                fAutomation->sATKSendMessageCount++;
            }
            else
            {
                fAutomation->sATKSendMessage[fAutomation->sATKSendMessageCount]=0;
                fAutomation->sATKSendMessageCount++;
                fAutomation->sATKSendMessage[fAutomation->sATKSendMessageCount]=0;
                fAutomation->sATKSendMessageCount++;
            }
        }
    }
    BCC=fAutomation->sATKSendMessage[0];

    for(int i=1; i<fAutomation->sATKSendMessageCount; i++)
    {
        BCC^=fAutomation->sATKSendMessage[i];
    }
    fAutomation->sATKSendMessage[fAutomation->sATKSendMessageCount]=BCC;
    fAutomation->sATKSendMessageCount++;
    fAutomation->sATKSendMessage[fAutomation->sATKSendMessageCount]='\0';
}
//---------------------------------------------------------------------------
void AmkorUDPGetTemp()         //Steven 20120330 : ATK Site Map Monitorning
{
//1Byte         1Byte        1Byte          4Byte      N     1Byte
//Packet Count, Packet Type, Handler Model, Data Size, Data, Check Sum
//Packet Type Define : SITE MAP=0x01, TEMPERATURE=0x02
//Handler Model :
//Check Sum : Packet Count ~ Data N 使用 XOR

//Site MAP Data: Aa Site Num, Aa Enable/Disable, Ab Site Num... Ba Site Num, Ba Enable/Disable...
//Temperature Info Data: Temperature Mode(1Byte), Temperature(1Byte), Soak Time(2Byte)
//Temperature Mode: High=0x00, Ambient=0x01

    AnsiString SendString;
//    char DataTempMode;
    int BCC;

//    if(LastSet.iTemperature==Tempture_Hot)
//        DataTempMode=0;
//    else
//        DataTempMode=1;

    fAutomation->sATKSendMessage[ 0]=1;                            //count     =1
    fAutomation->sATKSendMessage[ 1]=2;                            //Type      =Temperature
    fAutomation->sATKSendMessage[ 2]=10;                           //Model     =HT9045WH
    fAutomation->sATKSendMessage[ 3]=4;                            //DataSize  =4
    fAutomation->sATKSendMessage[ 4]=0;
    fAutomation->sATKSendMessage[ 5]=0;
    fAutomation->sATKSendMessage[ 6]=0;
    fAutomation->sATKSendMessage[ 7]=(LastSet.iTemperature==Tempture_Hot)?0:1;

    if(LastSet.iTemperature==Tempture_Hot)
    {
        fAutomation->sATKSendMessage[ 8]=int(Temperature.fWorkTemperBase);
        fAutomation->sATKSendMessage[ 9]=int(Temperature.fSoakTime)%256;
        fAutomation->sATKSendMessage[10]=int(Temperature.fSoakTime)/256;
        fAutomation->sATKSendMessageCount=11;
    }
    else
    {
        fAutomation->sATKSendMessage[ 8]=int(Temperature.fAbitTemp);
        fAutomation->sATKSendMessage[ 9]=0x00;
        fAutomation->sATKSendMessage[10]=0x00;
        fAutomation->sATKSendMessageCount=11;
    }

    BCC=fAutomation->sATKSendMessage[0];

    for(int i=1; i<fAutomation->sATKSendMessageCount; i++)
    {
        BCC^=fAutomation->sATKSendMessage[i];
    }
    fAutomation->sATKSendMessage[fAutomation->sATKSendMessageCount]=BCC;
    fAutomation->sATKSendMessageCount++;
    fAutomation->sATKSendMessage[fAutomation->sATKSendMessageCount]='\0';
}
//---------------------------------------------------------------------------
typedef std::pair<int,int> dPair;
bool SiteSort(dPair a,dPair b)
{
    return a.first<b.first;
}
//---------------------------------------------------------------------------
AnsiString GetTestResult_ASE_KR(int row,int col) //ChungHung 20120104 add
{
    dPair temp;
    std::vector<dPair> vBinData;
    vBinData.clear();
    for(int i=0; i<row; i++)
    {
        for(int j=0; j<col; j++)
        {
            if(TestSocket.Item[i][j]!=NULL_IC && TestSocket.Item[i][j]!=HAS_NULL_IC)
            {
                temp.first=TestIF.iSiteMap[i][j];
                temp.second=TestSocket.Item[i][j]-TEST_PASS+1;
            }
            else
            {
                temp.first=TestIF.iSiteMap[i][j];
                temp.second=0;
            }

            vBinData.push_back(temp);
        }
    }
    sort(vBinData.begin(),vBinData.end(),SiteSort);
    AnsiString Data="",SubData="";
    Data.sprintf("%c%c",0x01,row*col);
    AnsiString aSiteNum;
    AnsiString aBinResult;
    for(int i=0; i<row*col; i++)
    {
        temp=vBinData[i];
        aSiteNum=temp.first;
        aBinResult=temp.second;
        SubData.sprintf("%c%c%c%c%s%c%c%s",0x01,0x02,0x41,aSiteNum.Length(),aSiteNum,0x41,aBinResult.Length(),aBinResult);
        Data+=SubData;
    }
    return Data;
}
//---------------------------------------------------------------------------
int GetTotalSite_ASE_KR(int &row,int &col) //ChungHung 20120104 add
{
    int ret=0;
    switch(TestIF.iTestMode)
    {
        case SingleSite:
            row=1;
            col=1;
            ret=1;
            break;
        case DualSite:
            row=1;
            col=2;
            ret=2;
            break;
//        case DualSiteBS:
//            row=1;
//            col=2;
//            ret=2;
//            break;
        case TriSite1X3:
            row=1;
            col=3;
            ret=3;
            break;
        case QualSite1X4:
        case _8Site1X4: //ChungHung 20150528 add for 海思 _8Site1x4
            row=1;
            col=4;
            ret=4;
            break;
        case DualSite2x1:
            row=2;
            col=1;
            ret=2;
            break;
        case QualSite2X2:
        case QualSite2X2N:     //Frank 20200520 2X2NN Mode
            row=2;
            col=2;
            ret=4;
            break;
//        case QualSite2X2BS:
//            row=2;
//            col=2;
//            ret=4;
//            break;
        case _6Site2X3:  //ChungHung 20140115 add for 2x3_6
        case _6Site2X3N: //Steven 20220425 : 2X3NN Mode
            row=2;
            col=3;
            ret=6;
            break;
        case _8Site2X4:
        case _8Site2X4N: //Wei 20231211 : 2X4NN Mode
            row=2;
            col=4;
            ret=8;
            break;
        case _10Site2X5:    //wei 20190614 10 site
            row=2;
            col=5;
            ret=10;
            break;
        case _12Site2X6:
            row=2;
            col=6;
            ret=12;
            break;
        case _16Site2X8:
            row=2;
            col=8;
            ret=16;
            break;
        case _16Site4X4:  //Sam 20190226 : 16Site4X4
            row=4;
            col=4;
            ret=16;
            break;
        case _32Site4X8N:    //單Arm 32 Site
            row=4;
            col=8;
            ret=32;
            break;
        case _32Site4X8M:    //雙Arm 32 Site
            row=4;
            col=8;
            ret=32;
            break;
    }
    return ret;
}
//---------------------------------------------------------------------------
int GetLowYield(AnsiString *Data)
{
    Data[0]=(TestIF_File.bFailAlarmLowYield)?1:0;
    Data[1]=AnsiString(TestIF_File.iLowYieldLimit);
    Data[2]=AnsiString(TestIF_File.iLowYieldCount);
    return 3;
}
//---------------------------------------------------------------------------
int GetByArmPerSiteDiffYield(AnsiString *Data)
{
    Data[0]=(TestIF_File.bFailAlarmSiteYieldDifferent)?1:0;
    Data[1]=AnsiString(TestIF_File.iFailAlarmSiteYield);
    Data[2]=AnsiString(TestIF_File.iFailAlarmSiteYieldDifferentCount);
    return 3;
}
//---------------------------------------------------------------------------
int GetConsecutiveFailureAlarmByHead(AnsiString *Data)
{
    Data[0]=(TestIF_File.bContsFailByHead)?1:0;
    Data[1]=AnsiString(TestIF_File.iContsFailHeadAlarmCT);
    return 2;
}
//---------------------------------------------------------------------------
int GetConsecutiveFailureAlarmBySocket(AnsiString *Data)
{
    Data[0]=(TestIF_File.bContsFailBySocket)?1:0;
    Data[1]=AnsiString(TestIF_File.iContsFailSocketAlarmCT);
    return 2;
}
//---------------------------------------------------------------------------
int GetAllSiteFailFor9045(AnsiString *Data)
{
    Data[0]=(TestIF_File.bAllSiteFail)?1:0;
    return 1;
}
//---------------------------------------------------------------------------
int GetTrayFormTypeInfo(AnsiString *Data,int iType)
{
    if(iType>=1)
    {
        Data[0]=AnsiString(UserDefForm_File[iType-1].XDivision);
        Data[1]=AnsiString(UserDefForm_File[iType-1].YDivision);
        Data[2]=AnsiString(UserDefForm_File[iType-1].XStart);
        Data[3]=AnsiString(UserDefForm_File[iType-1].YStart);
        Data[4]=AnsiString(UserDefForm_File[iType-1].XPitch);
        Data[5]=AnsiString(UserDefForm_File[iType-1].YPitch);
        return 6;
    }
    return 0;
}
//---------------------------------------------------------------------------
int GetTrayFormTypeThickness(AnsiString *Data,int iType)
{
    if(iType>=1)
    {
        Data[0]=AnsiString(UserDefForm_File[iType-1].ZDepth);
        return 1;
    }
    return 0;
}
//---------------------------------------------------------------------------
int GetTrayFormTypePickUp(AnsiString *Data,int iType)
{
    if(iType>=1)
    {
        Data[0]=AnsiString(UserDefForm_File[iType-1].iPickUp);
        return 1;
    }
    return 0;
}
//---------------------------------------------------------------------------
int GetTrayFormTypeName(AnsiString *Data,int iType)
{
    if(iType>=1)
    {
        Data[0]=AnsiString(UserDefForm_File[iType-1].Alias);
        return 1;
    }
    return 0;
}
//---------------------------------------------------------------------------
int GetContactModeFor9045(AnsiString *Data)
{
    Data[0]=DeviceForm_File.ContactMode;
    return 1;
}
//---------------------------------------------------------------------------
int GetContactVacuumMode(AnsiString *Data)
{
    Data[0]=DeviceForm_File.VacuumMode;
    return 1;
}
//---------------------------------------------------------------------------
int GetContactDropWait(AnsiString *Data)
{
    Data[0]=AnsiString(DeviceForm_File.DropWait);
    return 1;
}
//---------------------------------------------------------------------------
int GetSlowContactSpeed(AnsiString *Data)
{
    Data[0]=DeviceForm_File.DropSpeed;
    return 1;
}
//---------------------------------------------------------------------------
int GetShuttleWaitOutSideCamber(AnsiString *Data)
{
    Data[0]=(DeviceForm_File.bShuttleWaitingOutSiteChamber)?1:0;
    return 1;
}
//---------------------------------------------------------------------------
int GetPickShuttleDeviceAfterTested(AnsiString *Data)
{
    Data[0]=(DeviceForm_File.bSuckShuttleDeviceAfterTested)?1:0;
    return 1;
}
//---------------------------------------------------------------------------
int GetPickShuttleDeviceThenWaitOnShuttle(AnsiString *Data)
{
    Data[0]=(DeviceForm_File.bSuckShuttleDeviceWaitOnShuttle)?1:0;
    return 1;
}
//---------------------------------------------------------------------------
int GetPickShuttleDeviceTogetherFor32SiteN(AnsiString *Data)
{
    Data[0]=(DeviceForm_File.bPickShuttleDeviceTogether)?1:0;
    return 1;
}
//---------------------------------------------------------------------------
int GetIndexArm1Height(AnsiString *Data)
{
    Data[0]=AnsiString(DeviceForm_File.IndexPlace[0]);
    Data[1]=AnsiString(DeviceForm_File.IndexArmPick[0]);
    Data[2]=AnsiString(DeviceForm_File.IndexDrop[0]);
    Data[3]=AnsiString(DeviceForm_File.IndexContact[0]);
    return 4;
}
//---------------------------------------------------------------------------
int GetIndexArm2Height(AnsiString *Data)
{
    Data[0]=AnsiString(DeviceForm_File.IndexPlace[1]);
    Data[1]=AnsiString(DeviceForm_File.IndexArmPick[1]);
    Data[2]=AnsiString(DeviceForm_File.IndexDrop[1]);
    Data[3]=AnsiString(DeviceForm_File.IndexContact[1]);
    return 4;
}
//---------------------------------------------------------------------------
int GetTestICCheckMode(AnsiString *Data)
{
    Data[0]=DeviceForm_File.iSocketInitialICCheckPosition;
    return 1;
}
//---------------------------------------------------------------------------
int GetAboveSocket(AnsiString *Data)
{
    Data[0]=AnsiString(DeviceForm_File.fSocketInitialICCheckPositionOffset);
    return 1;
}
//---------------------------------------------------------------------------
int GetContactForceInfo(AnsiString *Data)
{
    Data[ 0]=AnsiString(DeviceForm_File.dPress);
    Data[ 1]=fContact->edAirForceN->Text;
    Data[ 2]=fContact->edAirKPA->Text;
    Data[ 3]=fContact->edSetKg->Text;
    Data[ 4]=DeviceForm_File.iHeadDeviceCT;
    Data[ 5]=fContact->rgOutKitDiameter->ItemIndex;
    Data[ 6]=fContact->edForcePerDeviceKG->Text;
    Data[ 7]=fContact->edForcePerDeviceN->Text;
    Data[ 8]=DeviceForm_File.iPinCT;
    Data[ 9]=AnsiString(DeviceForm_File.ForcePerPinN);
    Data[10]=AnsiString(DeviceForm_File.ForcePerPinG);
    return 11;
}
//---------------------------------------------------------------------------
int GetHotPlateFormName(AnsiString *Data)
{
    Data[0]=HotPlateForm_File.Alias;
    return 1;
}
//---------------------------------------------------------------------------
int GetHotPlate1(AnsiString *Data)
{
    Data[0]=(HotPlateForm_File.iPlateSelect&0x01)?1:0;
    return 1;
}
//---------------------------------------------------------------------------
int GetHotPlate2(AnsiString *Data)
{
    Data[0]=(HotPlateForm_File.iPlateSelect&0x02)?1:0;
    return 1;
}
//---------------------------------------------------------------------------
int GetInterfaceType(AnsiString *Data)
{
    Data[0]=TestIF_File.iTestType;
    return 1;
}
//---------------------------------------------------------------------------
int GetTesterInitialMaximumTest(AnsiString *Data)
{
    Data[0]=AnsiString(TestIF_File.iInitialMaxTime);
    return 1;
}
//---------------------------------------------------------------------------
int GetTesterMaximumTest(AnsiString *Data)
{
    Data[0]=AnsiString(TestIF_File.iMaxTime);
    return 1;
}
//---------------------------------------------------------------------------
int GetTesterDummyTest(AnsiString *Data)
{
    Data[0]=AnsiString(TestIF_File.iDummyTime);
    return 1;
}
//---------------------------------------------------------------------------
int GetTesterStartDelay(AnsiString *Data)
{
    Data[0]=AnsiString(TestIF_File.dStartDelayTime);
    return 1;
}
//---------------------------------------------------------------------------
int GetHotSoakTime(AnsiString *Data)
{
    Data[0]=AnsiString(Temperature.fSoakTime);
    return 1;
}
//---------------------------------------------------------------------------
int GetHotJamSoakTime(AnsiString *Data)
{
    Data[0]=AnsiString(Temperature.fJamSoakTime);
    return 1;
}
//---------------------------------------------------------------------------
int GetHotInitialWaitTime(AnsiString *Data)
{
    Data[0]=AnsiString(Temperature.fInitialWaitTime);
    return 1;
}
//---------------------------------------------------------------------------
int GetHotInitialStart1Time(AnsiString *Data)
{
    Data[0]=Temperature.iInitialStart1Time;
    return 1;
}
//---------------------------------------------------------------------------
int GetHotShuttleSoakTime(AnsiString *Data)
{
    Data[0]=Temperature.iInitialStart2Time;
    return 1;
}
//---------------------------------------------------------------------------
int GetChamberCoolingTemp(AnsiString *Data)
{
    Data[0]=AnsiString(Temperature.fChamberCoolTemp);
    return 1;
}
//---------------------------------------------------------------------------
int GetHotIndexSoakTime(AnsiString *Data)
{
    Data[0]=Temperature.iIndexSoakTime;
    return 1;
}
//---------------------------------------------------------------------------
int GetHotOpenShortTime(AnsiString *Data)
{
    Data[0]=Temperature.iOSTime;
    return 1;
}
//---------------------------------------------------------------------------
int GetHotZ1Down(AnsiString *Data)
{
    Data[0]=(Temperature.bZ2DownSocket)?1:0;
    return 1;
}
//---------------------------------------------------------------------------
int GetHotShuttleSoakMode(AnsiString *Data)
{
    Data[0]=Temperature.iShuttleSoakTimeMode;
    return 1;
}
//---------------------------------------------------------------------------
int GetMachineTempMode(AnsiString *Data)
{
    Data[0]=Temperature.iMachineTempMode;
    return 1;
}
//---------------------------------------------------------------------------
int GetAmbientCheck(AnsiString *Data)
{
    Data[0]=(Temperature.bUseAbitCHK)?1:0;
    return 1;
}
//---------------------------------------------------------------------------
int GetAmbientCheckTemp(AnsiString *Data)
{
    Data[0]=AnsiString(FormatFloat("0.0", Temperature.fAbitTemp));
    return 1;
}
//---------------------------------------------------------------------------
int GetTemperatureOffset(AnsiString *Data)
{
    Data[0]=(Temperature.bEnableTempOffsetForInitial)?1:0;
    return 1;
}
//---------------------------------------------------------------------------
int GetContactCountForOffsetPeriod(AnsiString *Data)
{
    Data[0]=Temperature.iCintactCntForTempOffsetAtInitial;
    return 1;
}
//---------------------------------------------------------------------------
int GetContactCountForCoolDown(AnsiString *Data)
{
    Data[0]=Temperature.iCintactDelayCntForInitTempOffset;
    return 1;
}
//---------------------------------------------------------------------------
int GetInterfaceDIOInfo(AnsiString *Data)
{
    Data[0]=TestIF_File.iDioMode;
    return 1;
}
//---------------------------------------------------------------------------
int GetInterfaceGPIBInfo(AnsiString *Data)
{
    Data[0]=TestIF_File.iGpibMode;
    Data[1]=TestIF_File.iGpibAddress;
    return 2;
}
//---------------------------------------------------------------------------
int GetInterfaceRS232Info(AnsiString *Data)
{
    Data[0]=TestIF_File.iRs232Mode;
    //Data[1]=(TestIF_File.bRS232NeedSendVSOT)?1:0;
    Data[1]=0;
    Data[2]=TestIF_File.Rs232_Data.Baud_Rate;
    Data[3]=TestIF_File.Rs232_Data.Bit_Length;
    Data[4]=TestIF_File.Rs232_Data.Parity;
    Data[5]=TestIF_File.Rs232_Data.Stop_Bit;

    return 6;
}
//---------------------------------------------------------------------------
int GetLotInfo(AnsiString *Data)
{
    Data[0]=fLotInfo->edtSysLotID->Text.c_str();
    Data[1]=fLotInfo->edtSysOperatorID->Text.c_str();
    if(fLotInfo->cbRunMode->Text=="Normal")
    {
        Data[2]="0";
    }
    else
    {
        Data[2]="1";
    }
    return 3;
}
//---------------------------------------------------------------------------
int GetIP(AnsiString *Data)
{
    Data[0]=IniConfig.sN08OlpIP;
    return 1;
}
//---------------------------------------------------------------------------
int GetPort(AnsiString *Data)
{
    Data[0]=IniConfig.sN08OlpPort;
    return 1;
}
//---------------------------------------------------------------------------
int GetCusCode(AnsiString *Data)
{
    Data[0]=IntToStr(CUSTOMER_CODE);
    return 1;
}
//---------------------------------------------------------------------------
int GetStartMode(AnsiString *Data)  //Sam 20221212 : 新增 StartMode 指令
{
    Data[0]=IntToStr(LastSet.iRunStartMode);
    return 1;
}
//---------------------------------------------------------------------------
int GetLotTotal(AnsiString *Data)  //Sam 20230803 : 新增OLP指令     //QQQ
{
    Data[0]=IntToStr(LastSet.SendCT[0]);
    Data[1]=IntToStr(LastSet.iJamCount[1]);
    Data[2]=IntToStr(RunInfo.iUnloadCount);
    for(int i=0; i<iTestBinCount; i++)
    {
        Data[3+i]=IntToStr(LastSet.iBinData32[0][i]);
    }
    return 3+iTestBinCount;
}
//---------------------------------------------------------------------------
