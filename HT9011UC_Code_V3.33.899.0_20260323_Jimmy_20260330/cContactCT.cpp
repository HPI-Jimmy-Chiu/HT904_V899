#include "MachineDefine.h"
#pragma hdrstop

#include "cContactCT.h"
#include "cprod.h"
#include "common.h"
#include "cSocket.h"
#include "cMyDef.h"
#include "main.h"
#include "cObserver.h"
#include "cSortCT.h"
#include "cSecurity.h"
#include "cMyDB.h"
#include "uYieldMonitoring.h"
#include "ProductionInfo.h"
#include "uLotInfo.h"
#include "uCleaning.h"
#include "cinitial.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfContactCT *fContactCT;
int iMouseX=0, iMouseY=0;
//---------------------------------------------------------------------------
__fastcall TfContactCT::TfContactCT(TComponent* Owner)
    : TForm(Owner)
{
    bShow=false;
    pCanvas = new TCanvas;
    hDC=GetDC(sgYield->Handle);
}
//---------------------------------------------------------------------------
void __fastcall TfContactCT::FormShow(TObject *Sender)
{
    ShowFormComp();
    sgYield->Cells[1][1]="5";
    bShow=true;
    pCanvas->Handle=hDC;
    pCanvas->Font->Style=TFontStyles()<< fsBold;
    pCanvas->Font->Size=8;

    rgYieldType->Items->Clear();
    rgYieldType->Items->Add("History");
    rgYieldType->Items->Add("Total");
    rgYieldType->Items->Add("Kind");
    rgYieldType->Items->Add("Kind(%)");
    rgYieldType->Items->Add("ByHeadYield");
    rgYieldType->Columns=3;
    rgYieldType->Height=50;

    sgYield->Top=palClearCnt->Height+rgYieldType->Height;                       //jou 2014-09-24 50 -> palClearCnt->Height+rgYieldType->Height 修正畫面異常

    if(CosFunction.bUseLowYieldAlarmByBin)                                      //Steven 20140828 : By Bin Yield Monitor
    {
        rgYieldType->Height=70;
        sgYield->Width=370;
        sgYield->Top=102;
        rgYieldType->Items->Add("ByBinPass");
        rgYieldType->Items->Add("ByBinPass(%)");
        rgYieldType->Items->Add("ByBinArmPass");
        rgYieldType->Items->Add("ByBinArmPass(%)");
        rgYieldType->Items->Add("ByBinSitePass");
        rgYieldType->Items->Add("ByBinSitePass(%)");
    }

    if(CosFunction.IntervalYieldCount)                                          //wei 20180606 Interval Low Yield By Site
    {
        rgYieldType->Height=70;
        sgYield->Width=370;
        sgYield->Top=102;
        rgYieldType->Items->Add("IntervalYield(%)");
    }
    rgYieldType->ItemIndex=3;
    fContactCT->Height=palClearCnt->Height+rgYieldType->Height+sgYield->Height; //KEVIN 20141227  ChungHung 20141124 add ATK wnat to autosite
}
//---------------------------------------------------------------------------
void __fastcall TfContactCT::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    bShow=false;
}
//---------------------------------------------------------------------------
void __fastcall TfContactCT::FormDestroy(TObject *Sender)
{
    try
    {
        ReleaseDC(0, hDC);
        delete pCanvas;
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfContactCT::FormDestroy");
    }
    LogSoftwareOffTime("TfContactCT, FormDestroy"); //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
void __fastcall TfContactCT::ShowFormComp()
{
    if(TestIF.iTestMode==SingleSite)
        sgYield->RowCount=2;
    else if(TestIF.iTestMode==DualSite ||
            TestIF.iTestMode==DualSite2x1)
        sgYield->RowCount=3;
    else if(TestIF.iTestMode==QualSite2X2N)                                     //Frank 20200520 2X2NN Mode
        sgYield->RowCount=3;
    else if(TestIF.iTestMode==TriSite1X3 ||                                     //Steven 20160329 add for 1x3_4
            TestIF.iTestMode==_6Site2X3N)                                       //Steven 20220425 : 2X3NN Mode
        sgYield->RowCount=4;
    else if(TestIF.iTestMode==QualSite1X4 ||
            TestIF.iTestMode==QualSite2X2 ||
            TestIF.iTestMode==_8Site1X4   ||                                    //ChungHung 20150528 add for 海思 _8Site1x4
            TestIF.iTestMode==_8Site2X4N)                                       //Wei 20231211 : 2X4NN Mode
        sgYield->RowCount=5;
    else if(TestIF.iTestMode==_6Site2X3)                                        //ChungHung 20140115 add for 2x3_6
        sgYield->RowCount=7;
    else if(TestIF.iTestMode==_10Site2X5)                                       //wei 20190614 10 site
        sgYield->RowCount=11;
    else if(TestIF.iTestMode==_12Site2X6)                                       //ChungHung 20130507 add HT9045 updata for 12site 517
        sgYield->RowCount=13;
    else if(TestIF.iTestMode==_16Site2X8)
        sgYield->RowCount=17;
    else if(TestIF.iTestMode==_16Site4X4)                                       //Sam 20190226 : 16Site4X4
        sgYield->RowCount=9;
    else if(TestIF.iTestMode==_32Site4X8N)
        sgYield->RowCount=17;
    else if(TestIF.iTestMode==_32Site4X8M)
        sgYield->RowCount=33;
    else
        sgYield->RowCount=9;

    int iHeight=0;
    if(sgYield->RowCount<6)
    {
        iHeight=20;
        sgYield->Height=sgYield->RowCount*(iHeight+2);
        Height=palClearCnt->Height+rgYieldType->Height+(sgYield->RowCount+2)*(iHeight+2)+10;
    }
    else
    {
        iHeight=15;
        sgYield->Height=sgYield->RowCount*(iHeight+1.5);
        Height=palClearCnt->Height+rgYieldType->Height+(sgYield->RowCount+2)*(iHeight+1.5)+10;
    }
    for(int i=0; i<sgYield->RowCount; i++)
        sgYield->RowHeights[i]=iHeight;
}
//---------------------------------------------------------------------------
void __fastcall TfContactCT::sgYieldDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
    if(InitialOK==false)                                                        //Jou 20110705
        return;

    AnsiString str;
    AnsiString SCT="";
    int iShowSiteYieldIndex=0;
    AnsiString AYield="";                                                       //kevin 20170816 (Steven) add send Yield data
    AnsiString Arm[3]={"Arm1", "Arm2", "Sum"};
    static AnsiString ArmSite[32];

    if(ARow<16)                                                                 //jou 20180123 (Steven) : 修正low yield (by site) 顯示異常
    {
        if(TestIF.iTestMode==_32Site4X8N)
            iShowSiteYieldIndex=0;
        else
            iShowSiteYieldIndex=ARow-1;
    }

    if(ARow==0)
    {
        if(bUseTwoArm32Site==true)
        {
            if(ACol==1)
            {
                str.sprintf("Arm 2");
                MyDrawText(pCanvas, Rect, str.c_str(), clBtnFace);
            }
            else if(ACol==3)
            {
                str.sprintf("Arm 1");
                MyDrawText(pCanvas, Rect, str.c_str(), clBtnFace);
            }
        }
        else
        {
            if(ACol==1 || ACol==2)
            {
                str.sprintf("Arm %d", ACol);
                MyDrawText(pCanvas, Rect, str.c_str(), clBtnFace);
            }
            else if(ACol==3)
            {
                MyDrawText(pCanvas, Rect, "Sum", clBtnFace);
            }
        }
    }
    else if(ACol==0)
    {
        if(ARow==0)
            return;

        str.sprintf("%c%c", 'A'+(ChangeToFloatNonPcnt((double)((ARow-1)), (double)(TestSocket.iShtCol))), 'a'+((ARow-1)%TestSocket.iShtCol));
        MyDrawText(pCanvas, Rect, str.c_str(), clBtnFace);
    }
    else if(ACol==1)
    {
        if(ARow==0)
            return;
        if(ARow<=sgYield->RowCount)
        {
            if(ARow==1)
                ASE_Yield[1]="";                                                //kevin 20170817 (Steven) add

            if(bUseTwoArm32Site==true)
            {
                SCT=ReturnSiteData(1, ARow);
                MyDrawText(pCanvas, Rect, SCT.c_str(), clWhite, clBlack);
            }
            else
            {
                if(ARow==6)
                    SCT=0;
                SCT=ReturnSiteData(0, ARow);

                if(fYieldMonitoring->bShowSiteYield[iShowSiteYieldIndex])       //Steven 20120423 : Site Yield紅色顯示異常
                    MyDrawText(pCanvas, Rect, SCT.c_str(), clWhite, clRed);
                else
                    MyDrawText(pCanvas, Rect, SCT.c_str(), clWhite, clBlack);
            }
            pCanvas->Font->Color=clBlack;
            AYield.sprintf("%s=%s,", ArmSite[ARow], SCT);                       //kevin 20170816 (Steven) add Arm 1 Yield
            ASE_Yield[1]+= AYield;                                              //kevin 20170816 (Steven) add 傳送YIELD 給ASE
        }
    }
    else if(ACol==2)
    {
        if(ARow==0)
            return;

        if(IsNNMode()==NN_2Row)
        {
            str.sprintf("%c%c", 'C'+(ChangeToFloatNonPcnt((double)((ARow-1)), (double)(TestSocket.iShtCol))), 'a'+((ARow-1)%TestSocket.iShtCol));
            MyDrawText(pCanvas, Rect, str.c_str(), clBtnFace);
        }
        else if(IsNNMode()==NN_1Row)
        {
            str.sprintf("%c%c", 'B'+(ChangeToFloatNonPcnt((double)((ARow-1)), (double)(TestSocket.iShtCol))), 'a'+((ARow-1)%TestSocket.iShtCol));
            MyDrawText(pCanvas, Rect, str.c_str(), clBtnFace);
        }
        else
        {
            if(ARow<=sgYield->RowCount)
            {
                if(ARow==1)
                    ASE_Yield[2]="";                                            //kevin 20170817 (Steven) add

                SCT=ReturnSiteData(1, ARow);
                if(fYieldMonitoring->bShowSiteYield[iShowSiteYieldIndex])       //Steven 20120423 : Site Yield紅色顯示異常
                    MyDrawText(pCanvas, Rect, SCT.c_str(), clWhite, clRed);
                else
                    MyDrawText(pCanvas, Rect, SCT.c_str(), clWhite, clBlack);
                pCanvas->Font->Color=clBlack;

                if(ARow==(sgYield->RowCount-1))
                    AYield.sprintf("%s=%s", ArmSite[ARow],SCT);                 //kevin 20170816 (Steven) add  Arm 2 Yield
                else
                    AYield.sprintf("%s=%s,", ArmSite[ARow],SCT);                //kevin 20170816 (Steven) add
                ASE_Yield[2]+= AYield;                                          //kevin 20170816 (Steven) add 傳送YIELD 給ASE
            }
        }
    }
    else if(ACol==3)
    {
        if(ARow==0)
            return;

        if(bUseTwoArm32Site==true)
        {
            if(ARow<=sgYield->RowCount)
            {
                SCT=ReturnSiteData(0, ARow);
                MyDrawText(pCanvas, Rect, SCT.c_str(), clWhite, clBlack);
                pCanvas->Font->Color=clBlack;
            }
        }
        else
        {
            if(ARow<=sgYield->RowCount)
            {
                if(ARow==1)
                    ASE_Yield[3]="";                                            //kevin 20170817 (Steven) add

                SCT=ReturnSiteData(2, ARow);
                if(fYieldMonitoring->bShowSiteYield[iShowSiteYieldIndex])       //Steven 20120423 : Site Yield紅色顯示異常
                    MyDrawText(pCanvas, Rect, SCT.c_str(), clInfoBk, clRed);
                else
                    MyDrawText(pCanvas, Rect, SCT.c_str(), clInfoBk, clBlack);
                pCanvas->Font->Color=clBlack;
                if(ARow==(sgYield->RowCount-1))
                    AYield.sprintf("%s=%s",ArmSite[ARow],SCT);                  //kevin 20170816 (Steven) add  socket yield
                else
                    AYield.sprintf("%s=%s,",ArmSite[ARow],SCT);                 //kevin 20170816 (Steven) add
                ASE_Yield[3]+= AYield;                                          //kevin 20170816 (Steven) add 傳送YIELD 給ASE
            }
        }
    }
}
//---------------------------------------------------------------------------
bool __fastcall TfContactCT::TestSite2X2Mode()
{
    if(TestIF.iTestMode<=QualSite1X4 || TestIF.iTestMode==_8Site1X4)            //ChungHung 20150528 add for 海思 _8Site1x4
        return false;

    return true;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TfContactCT::ReturnSiteData(int Arm, int ARow)
{
    AnsiString Result="";
    int i, j;
    if(TestIF.iTestMode==DualSite2x1 ||
       TestIF.iTestMode==QualSite2X2)
    {
        i=(ARow-1)/2;
        j=(ARow-1)%2;
    }
    else if(TestIF.iTestMode==QualSite2X2N)                                     //Frank 20200520 2X2NN Mode
    {
        i=(ARow-1)/2;
        j=(ARow-1)%2;
    }
    else if(TestIF.iTestMode==_6Site2X3 ||                                      //ChungHung 20140115 add for 2x3_6
            TestIF.iTestMode==_6Site2X3N)                                       //Steven 20220425 : 2X3NN Mode
    {
        i=(ARow-1)/3;                                                           //ChungHung 20130507 add HT9045 updata for 12site 517
        j=(ARow-1)%3;
    }
    else if(TestIF.iTestMode==_16Site4X4 ||                                     //Sam 20190226 : 16Site4X4
            TestIF.iTestMode==_8Site2X4N)                                       //Wei 20231211 : 2X4NN Mode
    {
        i=(ARow-1)/4;
        j=(ARow-1)%4;
    }
    else if(TestIF.iTestMode==_10Site2X5)                                       //wei 20190614 10 site
    {
        i=(ARow-1)/5;                                                           //ChungHung 20130507 add HT9045 updata for 12site 517
        j=(ARow-1)%5;
    }
    else if(TestIF.iTestMode==_12Site2X6)                                       //Steven 20100113 : 12 Site
    {
        i=(ARow-1)/6;                                                           //ChungHung 20130507 add HT9045 updata for 12site 517
        j=(ARow-1)%6;
    }
    else if(TestIF.iTestMode==_16Site2X8)                                       //Steven 20100113 : 16 Site
    {
        i=(ARow-1)/8;
        j=(ARow-1)%8;
    }
    else if(TestIF.iTestMode==_32Site4X8N)                                      //Steven 20140512 : For HT-9047
    {
        i=(ARow-1)/8;
        j=(ARow-1)%8;
    }

    else if(TestIF.iTestMode==_32Site4X8M)                                      //Steven 20100113 : 32Site
    {
        i=(ARow-1)/8;
        j=(ARow-1)%8;
    }
    else if(TestSite2X2Mode())
    {
        i=(ARow-1)/4;
        j=(ARow-1)%4;
    }
    else
    {
        i=0;
        j=ARow-1;
    }

    if(i<0 || j<0)
    {
        Result="Err";
        return Result;
    }

    if(Arm==2)                                                                  //Total
    {
        int iColA, iColB;
        double dColA, dColB;
        if(rgYieldType->ItemIndex==0)
        {
            iColA=ArmHistory[0]->ArmSKET[i][j]->GetTotal();
            iColB=ArmHistory[1]->ArmSKET[i][j]->GetTotal();
        }
        else if(rgYieldType->ItemIndex==1)
        {
            iColA=ArmData[0]->ArmSKET[i][j]->GetTotal();
            iColB=ArmData[1]->ArmSKET[i][j]->GetTotal();
        }
        else if(rgYieldType->ItemIndex==2)
        {
            iColA=ArmData[0]->ArmSKET[i][j]->GetPassCT();
            iColB=ArmData[1]->ArmSKET[i][j]->GetPassCT();
        }
        else if(rgYieldType->ItemIndex==3)                                      //kevin 20130710 by head  yield
        {
            dColA=double(ArmData[0]->ArmSKET[i][j]->GetPCA());
            dColB=double(ArmData[1]->ArmSKET[i][j]->GetPCA());
        }
        else if(rgYieldType->ItemIndex==4)
        {
            dColA=double(ArmData[0]->ArmSKET[i][j]->GetBySitePCA());
            dColB=double(ArmData[1]->ArmSKET[i][j]->GetBySitePCA());
        }
        else if(rgYieldType->ItemIndex==5)                                      //ByBinPass
        {                                                                       //Steven 20140828 Start: By Bin Yield Monitor
            iColA=ArmData[0]->ArmSKET[i][j]->GetByBinLowYieldPassCT();
            iColB=ArmData[1]->ArmSKET[i][j]->GetByBinLowYieldPassCT();
        }
        else if(rgYieldType->ItemIndex==6)                                      //ByBinPass(%)
        {
            dColA=double(ArmData[0]->ArmSKET[i][j]->GetByBinLowYieldPassPCA());
            dColB=double(ArmData[1]->ArmSKET[i][j]->GetByBinLowYieldPassPCA());
        }
        else if(rgYieldType->ItemIndex==7)                                      //ByBinArmPass
        {
            iColA=ArmData[0]->ArmSKET[i][j]->GetByBinArmYieldPassCT();
            iColB=ArmData[1]->ArmSKET[i][j]->GetByBinArmYieldPassCT();
        }
        else if(rgYieldType->ItemIndex==8)                                      //ByBinArmPass(%)
        {
            dColA=double(ArmData[0]->ArmSKET[i][j]->GetByBinArmYieldPassPCA());
            dColB=double(ArmData[1]->ArmSKET[i][j]->GetByBinArmYieldPassPCA());
        }
        else if(rgYieldType->ItemIndex==9)                                      //ByBinSitePass
        {
            iColA=ArmData[0]->ArmSKET[i][j]->GetByBinSiteYieldPassCT();
            iColB=ArmData[1]->ArmSKET[i][j]->GetByBinSiteYieldPassCT();
        }
        else if(rgYieldType->ItemIndex==10)                                     //ByBinSitePass(%)
        {
            dColA=double(ArmData[0]->ArmSKET[i][j]->GetByBinSiteYieldPassPCA());
            dColB=double(ArmData[1]->ArmSKET[i][j]->GetByBinSiteYieldPassPCA());
        }

        if(rgYieldType->ItemIndex<3  || rgYieldType->ItemIndex==5 ||
           rgYieldType->ItemIndex==7 || rgYieldType->ItemIndex==9)
        {
            Result=(iColA+iColB);
        }
        else
        {
            if(TestIF.iShuttleMode==0)
            {
                Result.sprintf("%3.2f%%", (dColA+dColB)/2);
            }
            else
            {
                if(TestIF.iShuttle_Sel==0)
                {
                    Result.sprintf("%3.2f%%", dColA);
                }
                else
                {
                    Result.sprintf("%3.2f%%", dColB);
                }
            }
        }
    }
    else
    {
        if(rgYieldType->ItemIndex==0)
        {
            Result=ArmHistory[Arm]->ArmSKET[i][j]->GetTotal();
        }
        else if(rgYieldType->ItemIndex==1)
        {
            Result=ArmData[Arm]->ArmSKET[i][j]->GetTotal();
        }
        else if(rgYieldType->ItemIndex==2)
        {
            Result=ArmData[Arm]->ArmSKET[i][j]->GetPassCT();
        }
        else if(rgYieldType->ItemIndex==3)
        {
            Result.sprintf("%3.2f%", double(ArmData[Arm]->ArmSKET[i][j]->GetPCA()));
        }
        else if(rgYieldType->ItemIndex==4)
        {
            Result.sprintf("%3.2f%", double(ArmData[Arm]->ArmSKET[i][j]->GetBySitePCA()));  //kevin 20130710 by head  yield
        }
        else if(rgYieldType->ItemIndex==5)                                      //ByBinPass
        {                                                                       //Steven 20140828 Start: By Bin Yield Monitor
            Result=ArmData[Arm]->ArmSKET[i][j]->GetByBinLowYieldPassCT();
        }
        else if(rgYieldType->ItemIndex==6)                                      //ByBinPass(%)
        {
            Result.sprintf("%3.2f%", double(ArmData[Arm]->ArmSKET[i][j]->GetByBinLowYieldPassPCA()));
        }
        else if(rgYieldType->ItemIndex==7)                                      //ByBinArmPass
        {
            Result=ArmData[Arm]->ArmSKET[i][j]->GetByBinArmYieldPassCT();
        }
        else if(rgYieldType->ItemIndex==8)                                      //ByBinArmPass(%)
        {
            Result.sprintf("%3.2f%", double(ArmData[Arm]->ArmSKET[i][j]->GetByBinArmYieldPassPCA()));
        }
        else if(rgYieldType->ItemIndex==9)                                      //ByBinSitePass
        {
            Result=ArmData[Arm]->ArmSKET[i][j]->GetByBinSiteYieldPassCT();
        }
        else if(rgYieldType->ItemIndex==10)                                     //ByBinSitePass(%)
        {
            Result.sprintf("%3.2f%", double(ArmData[Arm]->ArmSKET[i][j]->GetByBinSiteYieldPassPCA()));
        }
    }
    return Result;
}
//---------------------------------------------------------------------------
//bType
//true = Total
//false = %
double TfContactCT::ReturnSiteDataArray(bool bType, int i, int j)               //JerryYang 20160530 LowYieldLimit要能設定到小數點,int改成double
{
    AnsiString Result="";
    int iColA=0, iColB=0;
    double dResult=0.0, dColA=0.0, dColB=0.0;                                   //JerryYang 20160530 LowYieldLimit要能設定到小數點,int改成double

    if(i<0 || j<0)
    {
        dResult=0;
        return dResult;
    }

    if(bType==true)
    {
        iColA=ArmData[0]->ArmSKET[i][j]->GetTotal();
        iColB=ArmData[1]->ArmSKET[i][j]->GetTotal();
        if(IsNNMode()==NN_2Row)
            dResult=iColA;
        else
            dResult=iColA+iColB;
    }
    else
    {
        dColA=double(ArmData[0]->ArmSKET[i][j]->GetPCA());
        dColB=double(ArmData[1]->ArmSKET[i][j]->GetPCA());

        if(TestIF.iShuttleMode==0)
        {
            if(IniConfig.bA09_ByArmCloseSite==true)                             //jou 2014-07-06 修正16 site low yield顯示異常 & bLowYieldAlarmSameNS function 異常
            {                                                                   //Steven 20171011 (wei) : 修改 Prod.fInArmSuckUse --> LastSet.bUseTestSocket, 否則 2x3mode會一直出現Aa, Ba Low Yield
                if(LastSet.bUseTestSocket[0][i][j]==true &&
                   LastSet.bUseTestSocket[1][i][j]==true)                       //ChungHung 20130910 alter for SCK can close site by Index)
                {
                    Result.sprintf("%3.2f%%", (dColA+dColB)/2);
                }
                else if(LastSet.bUseTestSocket[0][i][j]==true &&
                        LastSet.bUseTestSocket[1][i][j]==false)
                {
                    Result.sprintf("%3.2f%%", dColA);
                }
                else if(LastSet.bUseTestSocket[0][i][j]==false &&
                        LastSet.bUseTestSocket[1][i][j]==true)
                {
                    Result.sprintf("%3.2f%%", dColB);
                }
            }
            else
            {
                if(IsNNMode()==NN_2Row)                                         //Sam 20190226 : 16Site4X4 //Sam 20171212 (Steven) : 32 Site Fix
                    Result.sprintf("%3.2f%%", dColA);
                else
                    Result.sprintf("%3.2f%%", (dColA+dColB)/2);
            }
        }
        else
        {
            if(TestIF.iShuttle_Sel==0)
            {
                Result.sprintf("%3.2f%%", dColA);
            }
            else
            {
                Result.sprintf("%3.2f%%", dColB);
            }
        }
        dResult=atof(Result.c_str());
    }
    return dResult;
}
//---------------------------------------------------------------------------
int TfContactCT::ReturnSiteDataArray_AutoClean(bool bType, int i, int j)        //ChungHung 20131225 add
{
    AnsiString Result="";
    int iResult=0, iColA=0, iColB=0;
    double dColA=0.0, dColB=0.0;

    if(i<0 || j<0)
    {
        iResult=0;
        return iResult;
    }

    if(bType==true)
    {
        iColA=ArmData_AutoClean[0]->ArmSKET[i][j]->GetTotal();
        iColB=ArmData_AutoClean[1]->ArmSKET[i][j]->GetTotal();

        iResult=iColA+iColB;
    }
    else
    {
        dColA=double(ArmData_AutoClean[0]->ArmSKET[i][j]->GetPCA());
        dColB=double(ArmData_AutoClean[1]->ArmSKET[i][j]->GetPCA());

        if(TestIF.iShuttleMode==0)                                              //Steven 20171011 (wei) : 修改 Prod.fInArmSuckUse --> LastSet.bUseTestSocket, 否則 2x3mode會一直出現Aa, Ba Low Yield
        {
            if(LastSet.bUseTestSocket[0][i][j]==true &&
               LastSet.bUseTestSocket[1][i][j]==true && dColA!=0 && dColB!=0)   //ChungHung 20130910 alter for SCK can close site by Index)
                Result.sprintf("%3.2f%%", (dColA+dColB)/2);
            else if(LastSet.bUseTestSocket[0][i][j]==true && LastSet.bUseTestSocket[1][i][j]==false)
                Result.sprintf("%3.2f%%", dColA);
            else if(LastSet.bUseTestSocket[0][i][j]==false && LastSet.bUseTestSocket[1][i][j]==true)
                Result.sprintf("%3.2f%%", dColB);
        }
        else
        {
            if(TestIF.iShuttle_Sel==0)
            {
                Result.sprintf("%3.2f%%", dColA);
            }
            else
            {
                Result.sprintf("%3.2f%%", dColB);
            }
        }
        if(dColA==0 && dColB==0)
            Result.sprintf("%3.2f%%", 100);

        iResult=atoi(Result.c_str());
    }
    return iResult;
}
//---------------------------------------------------------------------------
double TfContactCT::GetLowYield_AutoClean(int iType)                            //iType:0 LowYield  ;1 SiteYieldDifferent by Socket
{
    AnsiString Result=0.0;
    int sum=0, ipass=0;
    double dColA=0, dColB=0, dCol=0;
    double dMax=0.00;
    double dMin=100.0;
    double dYield=100.0;

    sum  =ArmData_AutoClean[0]->GetTotalCT()+ ArmData_AutoClean[1]->GetTotalCT();   //Sam 20230104 : 修正 LowYield AutoClean
    ipass=ArmData_AutoClean[0]->GetPassCT() + ArmData_AutoClean[1]->GetPassCT();

    if(iType==0)
    {
        Result=ChangeToPercentage(ipass, sum);
    }
    else
    {
        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                if(LastSet.bUseTestSocket[0][i][j]==true &&                     //Steven 20171011 (wei) : 修改 Prod.fInArmSuckUse --> LastSet.bUseTestSocket, 否則 2x3mode會一直出現Aa, Ba Low Yield
                   LastSet.bUseTestSocket[1][i][j]==true)
                {
                    if(IsNNMode()==NN_2Row)
                    {
                        if(i<2)
                            dCol=double(ArmData_AutoClean[0]->ArmSKET[i][j]->GetPCA());
                        else
                            dCol=double(ArmData_AutoClean[1]->ArmSKET[i-2][j]->GetPCA());
                    }
                    else if(IsNNMode()==NN_1Row)
                    {
                        if(i<1)
                            dCol=double(ArmData_AutoClean[0]->ArmSKET[i][j]->GetPCA());
                        else
                            dCol=double(ArmData_AutoClean[1]->ArmSKET[i-1][j]->GetPCA());
                    }
                    else
                    {
                        dColA=double(ArmData_AutoClean[0]->ArmSKET[i][j]->GetPCA());
                        dColB=double(ArmData_AutoClean[1]->ArmSKET[i][j]->GetPCA());
                        dCol = (dColA + dColB)/2;
                    }
                    dMax=max(dMax, dCol);
                    dMin=min(dMin, dCol);
                }
                else if(LastSet.bUseTestSocket[0][i][j]==true && LastSet.bUseTestSocket[1][i][j]==false)
                {
                    dColA=double(ArmData_AutoClean[0]->ArmSKET[i][j]->GetPCA());
                    dCol = dColA;

                    dMax=max(dMax, dCol);
                    dMin=min(dMin, dCol);
                }
                else if(LastSet.bUseTestSocket[0][i][j]==false && LastSet.bUseTestSocket[1][i][j]==true)
                {
                    dColB=double(ArmData_AutoClean[1]->ArmSKET[i][j]->GetPCA());
                    dCol =dColB;

                    dMax=max(dMax, dCol);
                    dMin=min(dMin, dCol);
                }
            }
        }

        if(sum==0)
            dYield=0;
        else
            dYield=fabs(dMax-dMin);
        Result.sprintf("%3.2f%%", dYield);
    }

    return atof(Result.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TfContactCT::rgYieldTypeClick(TObject *Sender)
{
    sgYield->Refresh();

    double Sum2=0.0;

    if(TestIF_File.bLowYieldAlarmByBin)
    {
        Sum2=ArmData[0]->GetByBinLowYieldPCA();
        Sum2+=ArmData[1]->GetByBinLowYieldPCA();

        fSortCT->pnlYield->Caption=FormatFloat("0.00%", Sum2/2.0);              //Steven 20141125
        fSortCT->pnlYieldART->Caption=FormatFloat("0.00%", Sum2/2.0);           //kevin 20150615
    }
}
//---------------------------------------------------------------------------
void __fastcall TfContactCT::sgYieldDblClick(TObject *Sender)                   //Steven 20090805 : Clear Contact Kind when Double Click the StringGrid
{
    bool bClear=false;
    if(SystemStart)
        return;
    int iSelCol, iSelRow, i, j;

    sgYield->MouseToCell(iMouseX, iMouseY, iSelCol, iSelRow);

    if(TestIF.iTestMode==DualSite2x1 ||
       TestIF.iTestMode==QualSite2X2)
    {
        i=(iSelRow-1)/2;
        j=(iSelRow-1)%2;
    }
    else if(TestIF.iTestMode==QualSite2X2N)                                     //Frank 20200520 2X2NN Mode
    {
        j=(iSelRow-1)%2;
        if(iSelCol==3)                                                          //Steven 20220223 : 修正2x2 nn mode清除數量
            i=1;
        else
            i=0;
    }
    else if(TestIF.iTestMode==_6Site2X3)                                        //ChungHung 20140115 add for 2x3_6
    {
        i=(iSelRow-1)/3;
        j=(iSelRow-1)%3;
    }
    else if(TestIF.iTestMode==_6Site2X3N)                                       //Steven 20220425 : 2X3NN Mode
    {
        j=(iSelRow-1)%3;
        if(iSelCol==3)
            i=1;
        else
            i=0;
    }
    else if(TestIF.iTestMode==_8Site2X4N)                                       //Wei 20231211 : 2X4NN Mode
    {
        j=(iSelRow-1)%4;
        if(iSelCol==3)
            i=1;
        else
            i=0;
    }
    else if(TestIF.iTestMode==_10Site2X5)                                       //wei 20190614 10 site
    {
       i=(iSelRow-1)/5;                                                         //ChungHung 20130507 add HT9045 updata for 12site 517
       j=(iSelRow-1)%5;
    }
    else if(TestIF.iTestMode==_12Site2X6)                                       //jou 2013-12-05 修正12 site yield異常
    {
        i=(iSelRow-1)/6;
        j=(iSelRow-1)%6;
    }
    else if(TestIF.iTestMode==_16Site2X8)                                       //Steven 20100113 : 16 Site
    {
        i=(iSelRow-1)/8;
        j=(iSelRow-1)%8;
    }
    else if(TestIF.iTestMode==_16Site4X4)                                       //Sam 20190226 : 16Site4X4
    {
        i=(iSelRow-1)/4;
        j=(iSelRow-1)%4;
        if(iSelCol==3)                                                          //Steven 20220223 : 修正4x4site清除數量
            i+=2;
    }
    else if(TestIF.iTestMode==_32Site4X8M ||
            TestIF.iTestMode==_32Site4X8N)                                      //Steven 20100113 : 32Site
    {
        i=(iSelRow-1)/8;
        j=(iSelRow-1)%8;
        if(iSelCol==3)                                                          //Steven 20200227 : 修正32site清除數量
            i+=2;
    }
    else if(TestSite2X2Mode())
    {
        i=(iSelRow-1)/4;
        j=(iSelRow-1)%4;
    }
    else
    {
        i=0;
        j=iSelRow-1;
    }

    if(i<0)
        i=0;

    if(TestIF.iTestMode!=_32Site4X8M &&
       TestIF.iTestMode!=_32Site4X8N &&                                         //Steven 20200227 : 修正32site清除數量
       TestIF.iTestMode!=_16Site4X4)                                            //Steven 20220223 : 修正4x4site清除數量
    {
        if(i>=MAX_Index_Row)
            i=MAX_Index_Row-1;
    }
    if(j<0)
        j=0;
    if(j>=MAX_Index_Col)
        j=MAX_Index_Col-1;

    if(i<0 || j<0)
    {
        return;
    }

    if(CUSTOMER_CODE==CC_HANA_MICRON)                                           //Steven 20200211 : Hana micron want to clear history-data
        bClear=true;
    else if(rgYieldType->ItemIndex!=0)
        bClear=true;

    if(bClear)
    {
        if(Application->MessageBox("Do you want to clear the data?", "Warning!!", MB_YESNO | MB_TOPMOST) == IDYES)
        {
            MyDBIProductionData("Clear site yield");                            //Steven 20140816 : Production Data
            fProductionInfo->CalculateNowArmSiteBinQty(true);
            if(IsNNMode()==NN_2Row)                                             //Steven 20220223 : 修正32site清除數量
            {
                if(iSelCol==3)
                    ArmData[0]->ClearALLCT(i-2, j);
                else
                    ArmData[1]->ClearALLCT(i, j);
                ArmData[2]->ClearALLCT(i, j);
            }
            else if(IsNNMode()==NN_1Row)                                        //Steven 20220223 : 修正2x2 nn mode清除數量
            {
                if(iSelCol==3)
                    ArmData[0]->ClearALLCT(0, j);
                else
                    ArmData[1]->ClearALLCT(0, j);
                ArmData[2]->ClearALLCT(i, j);
            }
            else
            {
                for(int k=0; k<3; k++)
                {
                    ArmData[k]->ClearALLCT(i, j);                               //Steven 20140509 : For Secs GEM
                }
            }
            fProductionInfo->UpdateControlBinCount(true);                       //Sam 20200525 : Control Bin
            for(int k=0; k<32; k++)                                             //Steven 20100113 : 8->16
                fYieldMonitoring->bShowSiteYield[k]=false;                      //jou 20180123 (Steven) : 修正low yield (by site) 顯示異常
        }
        sgYield->Refresh();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfContactCT::ClearData(int iRow, int iCol)
{
    if(iRow<0 || iCol<0)
    {
        return;
    }
    fProductionInfo->CalculateNowArmSiteBinQty(true);
    if(IsNNMode()==NN_2Row)                                                     //Steven 20220223 : 修正4x4site清除數量
    {
        if(iRow<2)
            ArmData[1]->ClearALLCT(iRow, iCol);
        else
            ArmData[0]->ClearALLCT(iRow, iCol);
        ArmData[2]->ClearALLCT(iRow, iCol);
    }
    else if(IsNNMode()==NN_1Row)                                                //Steven 20220223 : 修正2x2 nn mode清除數量
    {
        if(iRow==0)
            ArmData[1]->ClearALLCT(iRow, iCol);
        else
            ArmData[0]->ClearALLCT(iRow, iCol);
        ArmData[2]->ClearALLCT(iRow, iCol);
    }
    else
    {
        for(int k=0; k<3; k++)
        {
            ArmData[k]->ClearALLCT(iRow, iCol);
        }
    }
    fProductionInfo->UpdateControlBinCount(true);                               //Sam 20200525 : Control Bin
    sgYield->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TfContactCT::ClearData_AutoClean()                              //ChungHung 20131225 add
{
    for(int k=0; k<3; k++)
    {
        for(int i=0; i<MAX_Index_Row; i++)
        {
            for(int j=0; j<MAX_Index_Col; j++)
            {
                ArmData_AutoClean[k]->SetPassCT(i, j, 0);                       //Steven 20140510 : Secs Gem
                ArmData_AutoClean[k]->SetFailCT(i, j, 0);                       //Steven 20140510 : Secs Gem
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfContactCT::sgYieldMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{                                                                               //Steven 20090805 : Get which Cell was been Selected.
    iMouseX=X;
    iMouseY=Y;
}
//---------------------------------------------------------------------------
void __fastcall TfContactCT::btClearCountClick(TObject *Sender)
{
    bool bClearData=false;
    if(SystemStart)
        return;

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)
    {
        if(bRefreshFunction ==false &&fSecurity->Insufficient(107)==false)      //kevin 20181012                    //wei 20151022 Count Clear權限設定
            return;
    }
    else if(CUSTOMER_CODE==CC_KYEC_LEE)                                         //Ifor 20191008 : add KYEC 清除 Count 需刷Barcode & 權限
    {
        if(fMain->bHasCleanCount==true)                                         //Ifor 20191016 : add KYEC清除Yield過程中不需再次登入權限
        {
            fMain->bHasCleanCount=false;                                        //Ifor 20191016 : add KYEC清除Yield過程中不需再次登入權限
        }
        else
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
    }
    else if(fSecurity->Insufficient(107)==false)                                //wei 20151022 Count Clear權限設定
    {
        return;
    }

    TSpeedButton *Ptr;                                                          //kevin 20170814 (Steven) add
    Ptr=(TSpeedButton *) Sender;

    if(Ptr->Name=="btClearCount")                                               //kevin 20170814 add
    {
        if(CUSTOMER_CODE==CC_KYEC_LEE)
        {
            bClearData=true;
        }
        else
        {
            if(Application->MessageBox("Do you want to clear the data?", "Warning!!", MB_YESNO|MB_TOPMOST)==IDYES)
            {
                bClearData=true;
            }
        }
        if(bClearData==true)
        {
            MyDBIProductionData("Clear Contact Count");                         //Steven 20140816 : Production Data

            if(IniConfig.bVTESTFunction==true)
            {
                for(int k=0; k<2; k++)
                    ArmData[k]->ClearALLCT();                                   //2012-01-03    Dell fix 當按下Count Clear,在Tester Category的I/F Error數值錯誤
            }
            else
            {
                for(int k=0; k<3; k++)
                    ArmData[k]->ClearALLCT();                                   //2012-01-03    Dell fix 當按下Count Clear,在Tester Category的I/F Error數值錯誤
            }

            for(int i=0; i<32; i++)                                             //Site Yield Alarm(%)
                fYieldMonitoring->bShowSiteYield[i]=false;
            fProductionInfo->UpdateControlBinCount(true);                       //Sam 20200525 : Control Bin
            if(CosFunction.bLowYieldUseContactCounts)                           //Sam 20221020 : LowYield 改使用 ContactCounts 的資料來計算
                fLotInfo->labLowYieldICCount->Caption=IntToStr(ArmData[2]->GetTotalCT());
            /*                                                                  //Sam 20241227 : PTI 不要清
            if(CosFunction.bSmartAutoClean)                                     //Sam 20230620 : 優化 Smart Auto Clean
            {
                for(int i=0; i<TEST_MAX_BIN; i++)
                    LastSet.iBinData32[0][i]=0;
            }
            */
            fCleaning->ResetSmartAutoClean();                                   //Sam 20230111 : Smart Auto Clean
            fSortCT->ShowSortIC();                                              //Sam 20240809 : PTI ART 模式
        }
    }
    else
    {
        MyDBIProductionData("ASE_Clear Contact Count");                         //Steven 20140816 : Production Data
        fProductionInfo->CalculateNowArmSiteBinQty(true);
        for(int k=0; k<3; k++)
        {
            ArmData[k]->ClearALLCT();                                           //2012-01-03    Dell fix 當按下Count Clear,在Tester Category的I/F Error數值錯誤
        }
        fProductionInfo->UpdateControlBinCount(true);                           //Sam 20200525 : Control Bin
        for(int i=0; i<32; i++)                                                 //Site Yield Alarm(%)
            fYieldMonitoring->bShowSiteYield[i]=false;
    }
    sgYield->Refresh();

    fYieldMonitoring->iFailAlarmSiteMaxYieldIntervalCount=0;                    //jou 2014-08-14 Site Compare Low Yield alarm
    fYieldMonitoring->iFailAlarmSiteYieldIntervalCount=0;
    fYieldMonitoring->iAutoClean_FailAlarmSiteYieldIntervalCount=0;
    fYieldMonitoring->ClearYieldCount();                                        //Steven 20140830 : Yield相關的Alarm, 要清掉全部的Ignore的Count重算
    fYieldMonitoring->ClearAutoSiteOffStatus();                                 //Steven 20200409 : 修正清除count之後,不能開site的問題

    LastSet.iIndexCount=0;                                                      //wei 20141201 Low Yield Auto Clean 重置
    iLowYieldCloseCount=0;
    bStandardYield=false;
    for(int i=0; i<4; i++)                                                      //KEVIN 201050424 FIX
    {
        for(int j=0; j<8; j++)
        {
            iStandardYield[i][j]=0;
        }
    }
    LastSet.iAutoTempOfsTriggerCnt=0;                                           //Sam 20220406 : 溫度自動補償功能 By FTP
}
//---------------------------------------------------------------------------
void __fastcall TfContactCT::btYieldChartClick(TObject *Sender)
{
    fObserver->iShowYieldChart=1;
    fObserver->ShowModal();
}
//---------------------------------------------------------------------------
void TfContactCT::SaveSiteYield(AnsiString SaveEvent)                           //Steven 20160822 : 修改存檔的方式
{
    if(IniConfig.bI29EnableYieldRecord==false)
        return;

    AnsiString sFileName;
    String StrYieldName="";                                                     //Yield 檔案名稱
    String NowYieldData="";                                                     //Yield 資料
    AnsiString strReg="";                                                       //字串暫存器

    sFileName.sprintf("%s\\%04d%02d\\", asYieldRecordPath, SystemYear, SystemMonth);    //Ifor 20151221 :新增Yeild Record 目錄
    MyForceDirectories(sFileName);                                              //Ifor 20151221 :新增判斷Yeild Record 目錄是否存在，不存在新增目錄

    NowYieldData=Now().FormatString("yyyy-mm-dd_");                             //取得現在日期
    StrYieldName=sFileName+"\\"+NowYieldData+IntToStr(TestSocket.iShtCnt)+"Site.csv";    //組合 Yield 檔案名稱

    NowYieldData="";
    if(IniConfig.bSPILFunction==true)                                           //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
    {
        if(SaveEvent!="By Interval")
            return;
        if(!FileExists(StrYieldName))                                           //Ifor 20151222 檔案不存在新增並寫入欄位名稱
        {
            AnsiString strSite="";
            NowYieldData="Time,";

            for(int Arm=0; Arm<2; Arm++)
            {
                for(int i=0; i<TestSocket.iShtRow; i++)
                {
                    for(int j=0; j<TestSocket.iShtCol; j++)
                    {
                        strSite.sprintf("%c%c", 'A'+i, 'a'+j);

                        NowYieldData = NowYieldData +
                                       strReg       +
                                       strSite      +
                                       ","          ;
                    }
                }
            }
            WriteDataToFile(StrYieldName, NowYieldData);                        //Steven 20160604 : add protect of fopen
        }
        NowYieldData=Now().FormatString("hh:nn:ss,");

        for(int Arm=0; Arm<2; Arm++)
        {
            for(int i=0; i<TestSocket.iShtRow; i++)
            {
                for(int j=0; j<TestSocket.iShtCol; j++)
                {
                    strReg.sprintf("%3.2f%", double(ArmData[Arm]->ArmSKET[i][j]->GetPCA()));
                    NowYieldData=NowYieldData+strReg+",";
                }
            }
        }
        WriteDataToFile(StrYieldName, NowYieldData);                            //Steven 20160604 : add protect of fopen
    }
    else if(IniConfig.bI29_1SaveYieldBySocketByBin)                             //Steven 20171108 (wei) : By Socket By Bin存檔
    {
        if(!FileExists(StrYieldName))                                           //Ifor 20151222 檔案不存在新增並寫入欄位名稱
        {
            AnsiString strSite="";
            NowYieldData=NowYieldData+"Time,SaveEvent,";
            strReg="";
            for(int k=0; k<iTestBinCount; k++)
            {
                strReg=strReg+
                       AnsiString("BIN")+
                       AnsiString(k)+
                       AnsiString(",");
            }
            NowYieldData=NowYieldData+strReg;
            for(int Arm=0; Arm<2; Arm++)
            {
                for(int i=0; i<TestSocket.iShtRow; i++)
                {
                    for(int j=0; j<TestSocket.iShtCol; j++)
                    {

                        strSite.sprintf("%c%c%d", 'A'+i, 'a'+j, Arm+1);
                        NowYieldData = NowYieldData +
                                       strSite      +
                                       ","          ;
                        for(int k=0; k<iTestBinCount; k++)
                        {
                            NowYieldData=NowYieldData+
                                         AnsiString("BIN")+
                                         AnsiString(k)+
                                         AnsiString(",");
                        }
                    }
                }
            }
            WriteDataToFile(StrYieldName, NowYieldData);                        //Steven 20160604 : add protect of fopen
        }
        NowYieldData=Now().FormatString("hh:nn:ss,")+SaveEvent+",";

        for(int k=0; k<iTestBinCount; k++)
        {
            strReg.sprintf("%d",  TastCategory.iTotalCategory[k]);
            NowYieldData=NowYieldData+strReg+",";
        }

        for(int Arm=0; Arm<2; Arm++)
        {
            for(int i=0; i<TestSocket.iShtRow; i++)
            {
                for(int j=0; j<TestSocket.iShtCol; j++)
                {
                    NowYieldData = NowYieldData + ",";
                    for(int k=0; k<iTestBinCount; k++)
                    {
                        strReg.sprintf("%d",  TastCategory.iCountCategory[Arm][i][j][k]);
                        NowYieldData=NowYieldData+strReg+",";
                    }
                }
            }
        }
        WriteDataToFile(StrYieldName, NowYieldData);                            //Steven 20160604 : add protect of fopen
    }
    else
    {
        if(!FileExists(StrYieldName))                                           //Ifor 20151222 檔案不存在新增並寫入欄位名稱
        {
            AnsiString strSite="";
            NowYieldData=NowYieldData+"Time,SaveEvent,";

            for(int Arm=0; Arm<2; Arm++)
            {
                for(int i=0; i<TestSocket.iShtRow; i++)
                {
                    for(int j=0; j<TestSocket.iShtCol; j++)
                    {
                        strSite.sprintf("%c%c", 'A'+i, 'a'+j);

                        NowYieldData = NowYieldData +
                                       strReg       +
                                       strSite      +
                                       ","          ;
                    }
                }
            }
            WriteDataToFile(StrYieldName, NowYieldData);                        //Steven 20160604 : add protect of fopen
        }
        NowYieldData=Now().FormatString("hh:nn:ss,")+SaveEvent+",";

        for(int Arm=0; Arm<2; Arm++)
        {
            for(int i=0; i<TestSocket.iShtRow; i++)
            {
                for(int j=0; j<TestSocket.iShtCol; j++)
                {
                    strReg.sprintf("%3.2f%", double(ArmData[Arm]->ArmSKET[i][j]->GetPCA()));
                    NowYieldData=NowYieldData+strReg+",";
                }
            }
        }
        WriteDataToFile(StrYieldName, NowYieldData);                            //Steven 20160604 : add protect of fopen
    }
}
//---------------------------------------------------------------------------
void  TfContactCT::ACSmartClearData()                                           //Sam 20230111 : Smart Auto Clean
{
    AnsiString s="";
    int iParam=0;                                                               //Sam 20230620 : 優化 Smart Auto Clean

    if(CosFunction.bSmartAutoClean==false || iRunACSmart==0 || iRunACSmart==2)
    {
        iRunACSmart=0;
        return;
    }

    for(int k=0; k<3; k++)
    {
        ArmData[k]->ClearALLCT();
    }

    for(int i=0; i<16; i++)                                                     //Site Yield Alarm(%)
        fYieldMonitoring->bShowSiteYield[i]=false;

    if(CosFunction.bLowYieldUseContactCounts)
        fLotInfo->labLowYieldICCount->Caption=IntToStr(ArmData[2]->GetTotalCT());

    sgYield->Refresh();

    fYieldMonitoring->iFailAlarmSiteMaxYieldIntervalCount=0;
    fYieldMonitoring->iFailAlarmSiteYieldIntervalCount=0;
    fYieldMonitoring->iAutoClean_FailAlarmSiteYieldIntervalCount=0;
    fYieldMonitoring->ClearYieldCount();
    fYieldMonitoring->ClearAutoSiteOffStatus();
    iAutoClean_IndexContactCount=0;                                             //Sam 20230620 : 優化 Smart Auto Clean
    /*                                                                          //Sam 20241227 : PTI 不要清
    for(int i=0; i<TEST_MAX_BIN; i++)
        LastSet.iBinData32[0][i]=0;
    */
    if(iACUseParam==2)
        iParam=2;
    else
        iParam=1;

    s.sprintf("Done Smart Auto Clean by %d Parameter and Clear Yield Data", iParam);
    NewRecordProcess("", s, "");
    iRunACSmart=0;
}
//---------------------------------------------------------------------------
void TfContactCT::SaveTotalYield(AnsiString SaveEvent)                          //Sam 20231106 : 紀錄 Total yield
{
    if(IniConfig.bI29YieldRecordIntervalIC==false)
        return;
    AnsiString sFileName="", StrYieldName="", NowYieldData="", sYield="";
    double dYield=0.0;
    int sum=0, ipass=0;

    sFileName.sprintf("%s\\%04d%02d\\", asYieldRecordPath, SystemYear, SystemMonth);
    MyForceDirectories(sFileName);

    if(RunInfo.iUnloadCount-iYeildCT[7]>=IniConfig.iI29YieldRecordIntervalIC)
    {
        sum=0;
        ipass=0;
        for(int i=0; i<6; i++)
        {
            sum+=LastSet.BinCT[0][i];
            if(BinSelect[iTestRunMode].iStackDefFailCate[i]==0)
                ipass+=LastSet.BinCT[0][i];
        }
        if(sum>0)
        {
            dYield=(double)ipass*100/sum;
            sYield.sprintf("%2.2f%s", dYield, "%");                             //Sam 20230914 : 自適應性良率監控
        }
        else
        {
            sYield="0.00";
        }

        NowYieldData=Now().FormatString("yyyy-mm-dd");                          //取得現在日期
        StrYieldName.sprintf("%s\\%s_TotalYield.csv",sFileName,NowYieldData);

        if(!FileExists(StrYieldName))
        {
            AnsiString strSite="";
            NowYieldData="Time,Pass,Total,Yiedl%";
            WriteDataToFile(StrYieldName, NowYieldData);
        }

        NowYieldData.sprintf("%s,%d,%d,%2.2f", Now().FormatString("hh:nn:ss"), ipass, sum, dYield);
        WriteDataToFile(StrYieldName, NowYieldData);
        iYeildCT[7]+=IniConfig.iI29YieldRecordIntervalIC;
    }
}
//---------------------------------------------------------------------------
