#include "MachineDefine.h"
#pragma hdrstop

#include "cCounterClear.h"

#include "cprod.h"
#include "common.h"
#include "csocket.h"
#include "cSortCT.h"
#include "cContactCT.h"
#include "cmydef.h"
#include "main.h"
#include "cShowBinSelect.h"
#include "cAuthority.h"
#include "cMyDB.h"
#include "cpublic.h"
#include "uYieldMonitoring.h"
#include "ProductionInfo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfCounterClear *fCounterClear;
//---------------------------------------------------------------------------
__fastcall TfCounterClear::TfCounterClear(TComponent* Owner)
    : TForm(Owner)
{
    bHasFile=true;
}
//---------------------------------------------------------------------------
void __fastcall TfCounterClear::WriteCTInfo()                                   //Steven 20110801 : 改用結構方式存檔
{
    for(int i=0; i<3; i++)                                                      //Steven 20140614 : 改用For迴圈
    {
        ArmData[i]->WriteFile();
        ArmDataLot[i]->WriteFile();
        ArmHistory[i]->WriteFile();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfCounterClear::ReadCTInfo()                                    //Steven 20110801 : 改用結構方式存檔
{
    for(int i=0; i<3; i++)                                                      //Steven 20140614 : 改用For迴圈
    {
        ArmData[i]->ReadFile();
        ArmDataLot[i]->ReadFile();
        ArmHistory[i]->ReadFile();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfCounterClear::cbSelectAllMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(cbSelectAll->Checked)
    {
        if(authCounterClr[0]) cbAlarmData       ->Checked=true;
        if(authCounterClr[1]) cbTestCategory    ->Checked=true;
        if(authCounterClr[2]) cbScanner         ->Checked=true;
        if(authCounterClr[3]) cbLoadingCount    ->Checked=true;
        if(authCounterClr[4]) cbContactCountCurr->Checked=true;
        if(authCounterClr[5]) cbContactCountHis ->Checked=true;
        if(authCounterClr[6]) cbSortingCount    ->Checked=true;
        if(authCounterClr[7]) cbTimeData        ->Checked=true;
        cbSelectAll->Caption="UnSelect All";
    }
    else
    {
        if(authCounterClr[0]) cbAlarmData       ->Checked=false;
        if(authCounterClr[1]) cbTestCategory    ->Checked=false;
        if(authCounterClr[2]) cbScanner         ->Checked=false;
        if(authCounterClr[3]) cbLoadingCount    ->Checked=false;
        if(authCounterClr[4]) cbContactCountCurr->Checked=false;
        if(authCounterClr[5]) cbContactCountHis ->Checked=false;
        if(authCounterClr[6]) cbSortingCount    ->Checked=false;
        if(authCounterClr[7]) cbTimeData        ->Checked=false;
        cbSelectAll->Caption="Select All";
    }
}
//---------------------------------------------------------------------------
void __fastcall TfCounterClear::cbAlarmDataMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TCheckBox *CheckBox=(TCheckBox *)Sender;
    if(!CheckBox->Checked)
    {
        cbSelectAll->Checked=false;
        cbSelectAll->Caption="Select All";
    }
}
//---------------------------------------------------------------------------
//Steven 20090731 : Use Auth to Enable/Disable Component
//---------------------------------------------------------------------------
void __fastcall TfCounterClear::FormShow(TObject *Sender)
{
    GetCountClrAuth();
    cbAlarmData         ->Enabled=authCounterClr[0];
    cbTestCategory      ->Enabled=authCounterClr[1];
    cbScanner           ->Enabled=authCounterClr[2];
    cbLoadingCount      ->Enabled=authCounterClr[3];
    cbContactCountCurr  ->Enabled=authCounterClr[4];
    cbContactCountHis   ->Enabled=authCounterClr[5];
    cbSortingCount      ->Enabled=authCounterClr[6];
    cbTimeData          ->Enabled=authCounterClr[7];
    if(!authCounterClr[0]) cbAlarmData          ->Checked=false;
    if(!authCounterClr[1]) cbTestCategory       ->Checked=false;
    if(!authCounterClr[2]) cbScanner            ->Checked=false;
    if(!authCounterClr[3]) cbLoadingCount       ->Checked=false;
    if(!authCounterClr[4]) cbContactCountCurr   ->Checked=false;
    if(!authCounterClr[5]) cbContactCountHis    ->Checked=false;
    if(!authCounterClr[6]) cbSortingCount       ->Checked=false;
    if(!authCounterClr[7]) cbTimeData           ->Checked=false;
    Top=30;
    Left=300;
    fShow=true;
}
//---------------------------------------------------------------------------
void __fastcall TfCounterClear::ClearCount(int ClearType)
{
    AnsiString BatFile="D:\\HT9045\\system\\BinCount.txt";                      //kevin 20210825 清除Bin 數量統計
    int iCate=0;
    switch(ClearType)
    {
        case ctAlarmData:
            break;
        case ctTesterCategory:
            fProductionInfo->CalculateNowArmSiteBinQty(true);
            for(int i=0; i<MAX_SOCKET_ROW; i++)
            {
                for(int j=0; j<MAX_SOCKET_COL; j++)
                {
                    for(int k=0; k<iTestBinCount; k++)                          //Steven 20121112 : RS232支援32Bin 15 --> iTestBinCount
                    {
                        ArmData[0]->SetBinCT(i, j, k, 0);                       //Steven 20140509 Modify
                        ArmData[1]->SetBinCT(i, j, k, 0);
                        ArmData[2]->SetBinCT(i, j, k, 0);
                        iByBinTotal[k]=0;                                       //kevin 20180703 (wei) bin 數量 Bin total[0]
                    }
                    ArmData[0]->SetIFErr(i, j, 0);
                    ArmData[1]->SetIFErr(i, j, 0);
                    ArmData[2]->SetIFErr(i, j, 0);
                }
            }

            TastCategory.UpdataCount(true);                                     //Steven 20250514 : 統一計算數量
            try
            {
                if(FileExists(BatFile))
                    DeleteFile(BatFile);
            }
            catch(...)
            {
                MyDBIProcess("Process", "Delete File BinCount.txt Error");
            };
            MyDBIProcess("Process", "Tester category has been cleared!!");
            fProductionInfo->UpdateControlBinCount(true);                       //Sam 20200525 : Control Bin
            break;
        case ctLoadingCounts:
            LastSet.iJamCount[0]=0;
            LastSet.iJamCount[1]=0;
            LastSet.SendCT[0]=0;
            LastSet.SendCT[1]=0;                                                //jou 2010-08-13 計數jam rate,改為Tray Feed為一單位
            LastSet.SendCT[2]=0;                                                //jou 2010-11-17 Piggy-Back Functions
            LastSet.SendCT[3]=0;                                                //Steven 20110420 : Piggy-Back Functions - Loading Count
            LastSet.iIndexCount=0;                                              //wei 20141201 Low Yield Auto Clean

            if(USE_AUTO_RETEST==eartInstall && bAutoReTest_ART)                 //Ifor 20171016 (wei) : add Auto Sort Count 計數功能 僅由SECS GEM 下命令清除
            {
                if(LastSet.iRunStartMode>=rsmInitial_ART && LastSet.iRunStartMode<=rsmAutoRetest)
                {
                    LastSet.SendCT_ART[0]=0;
                    LastSet.SendCT_ART[1]=0;
                    LastSet.SendCT_ART[2]=0;
                    LastSet.SendCT_ART[3]=0;
                }
            }
            MyDBIProcess("Process", "Loading Count has been cleared!!");
            break;
        case ctContactCounts:
            for(int i=0; i<MAX_SOCKET_ROW; i++)
            {
                for(int j=0; j<MAX_SOCKET_COL; j++)
                {
                    ArmData[0]->SetFailCT(i, j, 0);                             //Steven 20140509 : Modify
                    ArmData[0]->SetPassCT(i, j, 0);
                    ArmData[1]->SetFailCT(i, j, 0);
                    ArmData[1]->SetPassCT(i, j, 0);
                    ArmData[2]->SetFailCT(i, j, 0);
                    ArmData[2]->SetPassCT(i, j, 0);
                }
            }
            MyDBIProcess("Process", "Contact Count(Total) has been cleared!!");
            break;
        case ctContactCountsHis:
            for(int i=0; i<MAX_SOCKET_ROW; i++)
            {
                for(int j=0; j<MAX_SOCKET_COL; j++)
                {
                    ArmHistory[0]->SetFailCT(i, j, 0);                          //Steven 20140509 : Modify
                    ArmHistory[0]->SetPassCT(i, j, 0);
                    ArmHistory[1]->SetFailCT(i, j, 0);
                    ArmHistory[1]->SetPassCT(i, j, 0);
                    ArmHistory[2]->SetFailCT(i, j, 0);
                    ArmHistory[2]->SetPassCT(i, j, 0);
                }
            }
            MyDBIProcess("Process", "Contact Count(History) has been cleared!!");
            break;
        case ctTraySortCount:
            fProductionInfo->CalculateNowUnloaderTrayQty(true);
            for(int i=0; i<eTrayCount; i++)                                     //JerryYang 20220909 : 10->eTrayCount
            {
                LastSet.RecodeTrayCount[i]=0;                                   //frank 20200814 : 每10盤記錄一次summary log
                LastSet.BinCT[0][iTo3Unload[i]]=0;                              //Steven 20180516 : 1給別人用,所以不能清除
                LastSet.BinCT[2][iTo3Unload[i]]=0;
                LastSet.BinCT[3][iTo3Unload[i]]=0;
                if(USE_AUTO_RETEST==eartInstall && bAutoReTest_ART)             //Ifor 20171016 (wei) : add Auto Sort Count 計數功能 僅由SECS GEM 下命令清除
                {
                    LastSet.BinCT_ART[0][iTo3Unload[i]]=0;
                    LastSet.BinCT_ART[2][iTo3Unload[i]]=0;
                    LastSet.BinCT_ART[3][iTo3Unload[i]]=0;
                }
            }

            if(CosFunction.bSpecailLowYeild)                                    //Sam 20210505 : PTI 要求的兩段 Low Yeild
                LowYieldSpecialInitail();

            for(int i=0; i<TEST_MAX_BIN; i++)
            {
                LastSet.iBinData32[0][i]=0;
                iSVByBinCount[i]=0;                                             //JerryYang 20250120 : add
                LastSet.iBinData32[2][i]=0;
                LastSet.iBinData32[3][i]=0;
                if(USE_AUTO_RETEST==eartInstall && bAutoReTest_ART)             //kevin 20150615
                {
                    LastSet.iBinData32_ART[0][i]=0;
                    LastSet.iBinData32_ART[2][i]=0;
                    LastSet.iBinData32_ART[3][i]=0;
                }
            }

            if(CosFunction.bHWBinBox && iHWFix_BinBox==1)                       //kevin 20160822 error bin box 清除
            {
                LastSet.iBinBoxCount=0;
            }
            break;
        case ctTimeData:
            for(int k=0; k<2; k++)                                              //Steven 20190714 : 4-->3
            {
                for(int i=0; i<8; i++)
                    LastSet.SystemAccSecond[k][i]=0;                            //第三組改成每12Hr計算一次, 第二組為每24Hr計算一次
            }
            MyDBIProcess("Process", "Time Data has been cleared!!");
            break;
        case ctBinCount:
            for(int k=0; k<4; k++)
            {
                for(int i=0; i<TEST_MAX_BIN; i++)
                {
                    LastSet.iBinData32[k][i]=0;
                    iSVByBinCount[i]=0;                                         //JerryYang 20250120 : add
                }
            }
            break;
        case ctIndexCount:                                                      //kevin 20130125 記錄 index吸取ic數量 index放置shuttle 數量
            for(int k=0; k<4; k++)
            {
                LastSet.iIndexInputOutPut[k]=0;
            }
            break;
        case ctAutoRetestCount:                                                 //kevin 20150530 AutoRETEST
            if(USE_AUTO_RETEST==eartInstall &&
               (bAutoReTest_ART || IniConfig.bA10_AutoReTest))                  //Ifor 20171016 (wei) : add Auto Sort Count 計數功能 僅由SECS GEM 下命令清除
            {
                if(CUSTOMER_CODE==CC_KYEC_LEE)
                {
                    if(LastSet.iRunStartMode>=rsmInitial_ART &&
                       LastSet.iRunStartMode<=rsmAutoRetest)
                    {
                        LastSet.SendCT_ART[0]=0;                                //kevin 20150615  ART 計數清除
                    }
                }
                else
                {
                    LastSet.SendCT_ART[0]=0;                                    //kevin 20150615  ART 計數清除
                }
            }

            for(int k=0; k<4; k++)
            {
                if(k==1)                                                        //Steven 20190715 : 第1組為每天24hr清一次
                {
                }
                else
                {
                    for(int i=0; i<eTrayCount; i++)
                    {
                        if(CosFunction.bUseARTSortCount==false ||               //Ifor 20171016 (wei) : add Auto Sort Count 計數功能 僅由SECS GEM 下命令清除
                           (LastSet.iRunStartMode>=rsmInitial_ART &&
                            LastSet.iRunStartMode<=rsmAutoRetest))              //Ifor 20180110 (Steven) : add rsmInitial_ART 需清除Sort Count
                        {
                            LastSet.BinCT_ART[k][iTo3Unload[i]]=0;              //kevin 20150615  ART 計數清除
                        }

                        if(bART_needRT2)                                        //kevin 20150717 退fail tray 清為0
                        {
                            if(k==0 &&(BinSelect[4].iStackDefFailCate[i]>0))    //有設定Fail tray
                            {
                                LastSet.BinCT[k][iTo3Unload[i]]=0;
                                if(USE_AUTO_RETEST==eartInstall &&
                                   (bAutoReTest_ART ||
                                    IniConfig.bA10_AutoReTest))                 //Ifor 20171016 (wei) : add Auto Sort Count 計數功能 僅由SECS GEM 下命令清除
                                {
                                    if(CosFunction.bUseARTSortCount==false ||   //Ifor 20171016 (wei) : add Auto Sort Count 計數功能 僅由SECS GEM 下命令清除
                                       (LastSet.iRunStartMode>=rsmInitial_ART &&
                                        LastSet.iRunStartMode<=rsmAutoRetest))  //Ifor 20180110 (Steven) : add rsmInitial_ART 需清除Sort Count
                                    {
                                        LastSet.BinCT_ART[k][iTo3Unload[i]]=0;
                                    }
                                }
                            }
                        }
                        else if(k==0 && (BinSelect[4].bAutoRetest[i]))          //有設定autoretest
                        {
                            LastSet.BinCT[k][iTo3Unload[i]]=0;
                            if(USE_AUTO_RETEST==eartInstall &&
                               (bAutoReTest_ART ||
                                IniConfig.bA10_AutoReTest))                     //Ifor 20171016 (wei) : add Auto Sort Count 計數功能 僅由SECS GEM 下命令清除
                                LastSet.BinCT_ART[k][iTo3Unload[i]]=0;
                        }
                    }

                    for(int i=0; i<TEST_MAX_BIN; i++)
                    {
                        if(CosFunction.bUseARTSortCount==false  ||
                           (LastSet.iRunStartMode>=rsmInitial_ART &&            //Ifor 20180110 (Steven) : add rsmInitial_ART 需清除Sort Count
                            LastSet.iRunStartMode<=rsmAutoRetest))              //Ifor 20171016 (wei) : add Auto Sort Count 計數功能 僅由SECS GEM 下命令清除
                        {
                            LastSet.iBinData32_ART[k][i]=0;
                        }

                        if(TestIF_File.bPass[i] || Prod.bIsPassBin[i])          //20150706autoRetest 設定pass bin 和原本 PASS TRAY BIN RT中部需清除
                            continue;

                        if(IniConfig.bA60EnableAMR)                             //Sam 20240827 : 新增 AMR 功能
                        {
                            if(k==2)
                            {
                                iCate=BinSelect[iTestRunMode].iCatDataT3Pos[i];
                                if(iCate>=1 && iCate<=9)
                                {
                                    if(BinSelect[iTestRunMode].bCateR[iCate-1])
                                    {
                                        LastSet.iBinData32[k][i]=0;
                                        iSVByBinCount[i]=0;         //JerryYang 20250120 : add
                                    }
                                }
                            }
                            else
                            {
                                LastSet.iBinData32[k][i]=0;
                                iSVByBinCount[i]=0;                 //JerryYang 20250120 : add
                            }
                        }
                        else
                        {
                            LastSet.iBinData32[k][i]=0;
                            iSVByBinCount[i]=0;                     //JerryYang 20250120 : add
                        }
                    }
                }
            }

            if(CUSTOMER_CODE==CC_PTI && IniConfig.bB03_TesterReport)            //Sam 20240809 : PTI ART 模式
            {
                ArmData[2]->ClearALLCT();
                TastCategory.UpdataCount(true);                                 //Steven 20250514 : 統一計算數量
            }
            break;
        case ctFailBinCount:                                                    //Frank 20241114 : Add
            for(int i=eAuto1; i<=iAutoRight; i++)
            {
                if(Prod.iIsFailT6[i]==1)
                {
                    LastSet.BinCT[0][i]=0;
                    LastSet.BinCT[2][i]=0;
                    LastSet.BinCT[3][i]=0;
                }
            }
            break;
    }

    bRefreshCount=true;                                                         //Stteve 20250219 : 清除數量後的重整要delay一下
}
//---------------------------------------------------------------------------
void __fastcall TfCounterClear::spbExeClick(TObject *Sender)
{
    if(CUSTOMER_CODE==CC_KYEC_LEE)                                              //Ifor 20191008 : add KYEC 清除 Count 需刷Barcode & 權限
    {
        ReEnterBarcode[0]=false;
        fMain->cbUserSelect->ItemIndex=0;
        AccessLevel=0;
        fMain->ChangeLevelAttr();
        fMain->cbUserSelect->Text="Operator";
        fMain->spbUserName->Caption="Operator";
        fMain->btLogin->Caption="Login";
        if(FileExists(pwPath))
        {
            fMain->cbUserSelectChange(NULL);
        }
        else
        {
            fMain->stOperatorClick(fMain);
        }

        if(AccessLevel<iDefSupervisorLevel)
        {
            return;
        }
    }

    MyDBIProductionData("Clear Count executed");
    fMain->Clarn_Data(10, "Manual clear count");

    if(cbAlarmData->Checked)
        ClearCount(ctAlarmData);

    if(cbTestCategory->Checked)
        ClearCount(ctTesterCategory);

    if(cbLoadingCount->Checked)
    {
        ClearCount(ctLoadingCounts);
        ClearCount(ctIndexCount);                                               //kevin 20130125 記錄 index吸取ic數量 index放置shuttle 數量
    }

    if(cbContactCountCurr->Checked)
        ClearCount(ctContactCounts);

    if(cbContactCountHis->Checked)
        ClearCount(ctContactCountsHis);

    if(cbSortingCount->Checked)
        ClearCount(ctTraySortCount);

    if(cbTimeData->Checked)
        ClearCount(ctTimeData);

    fMain->Clarn_Data(10, "Manual clear count done");
    MyDBIProcess("Process", "Counter Clear has been executed!!");
    spbExe->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall TfCounterClear::spbExitClick(TObject *Sender)
{
    spbExit->Down=false;
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfCounterClear::AutoClear()
{
    if(IniConfig.bDownLoadAutoCountClear==false)
        return;

    GetCountClrAuth();

    if(authCounterClr[0])
        ClearCount(ctAlarmData);

    if(authCounterClr[1])
        ClearCount(ctTesterCategory);

    if(authCounterClr[3])
        ClearCount(ctLoadingCounts);

    if(authCounterClr[4])
        ClearCount(ctContactCounts);

    if(authCounterClr[5])
        ClearCount(ctContactCountsHis);

    if(authCounterClr[6])
        ClearCount(ctTraySortCount);

    if(authCounterClr[7])
        ClearCount(ctTimeData);
}
//---------------------------------------------------------------------------
void __fastcall TfCounterClear::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    fShow=false;
}
//---------------------------------------------------------------------------
void __fastcall TfCounterClear::LowYieldSpecialInitail()                        //Sam 20210505 : PTI 要求的兩段 Low Yeild
{
    bLowYeildAlarmSpecial=false;
    bLowYeildAlarmSpecial1stPass=false;
    for(int i=0; i<eTrayCount; i++)
    {
        LastSet.BinCT_PTI[0][iTo3Unload[i]]=0;
        LastSet.BinCT_PTI[2][iTo3Unload[i]]=0;
        LastSet.BinCT_PTI[3][iTo3Unload[i]]=0;
    }
}
//---------------------------------------------------------------------------
