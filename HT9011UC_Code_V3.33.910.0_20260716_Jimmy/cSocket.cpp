#include "MachineDefine.h"
#pragma hdrstop

#include "cSocket.h"
#include "cprod.h"
#include "cmydef.h"
#include "common.h"
#include "mymessbox.h"
#include "SCK_ART.h"
#include "cinitial.h"
//------------------------------------------------------------------------------

#pragma package(smart_init)
TArm *ArmData[3];
TArm *ArmDataLot[3];
TArm *ArmHistory[3];
TArm *ArmData_AutoClean[3];                                                     //ChungHung 20131225 add
TLotSummary LotSummary;
TEST_CATEGORY TastCategory;
TEST_CATEGORY OldControlBinCategory;                                            //Sam 20200525 : Control Bin
TEST_CATEGORY NowControlBinCategory;                                            //Sam 20200525 : Control Bin
//------------------------------------------------------------------------------
_fastcall TMySocket::TMySocket()
{
    sBinPassFail=new TStringList();
    sBinCT      =new TStringList();

    for(int i=0; i<TEST_MAX_BIN; i++)
    {
        sBinPassFail->Add("-1");
        sBinCT      ->Add("0");
    }

    ClearALLCT();
    for(int i=0; i<TEST_MAX_BIN; i++)
    {
        SetByBinLowYieldPassFail[i]=false;                                      //Steven 20140828 : By Bin Yield Monitor
        SetByBinArmYieldPassFail[i]=false;                                      //Steven 20140828 : By Bin Arm Yield Monitor
        SetByBinSiteYieldPassFail[i]=false;                                     //Steven 20140828 : By Bin Site Yield Monitor
        SetBinPassFail[i]=-1;
        iBinCT[i]=0;
        iByBinTotal[i]=0;                                                       //kevin 20180705 (wei) bin 數量 Bin total[0]
    }
}
//------------------------------------------------------------------------------
 _fastcall TMySocket::~TMySocket()
{
    try
    {
        sBinPassFail->Clear();                                                  //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        sBinCT->Clear();                                                        //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        delete sBinPassFail;
        delete sBinCT;
    }
    catch(...)
    {
        MyDBIProcess("Exception", "~TMySocket");
    }
}
//------------------------------------------------------------------------------
void _fastcall TMySocket::ClearALLCT()
{
    Pass=0;
    Fail=0;
    Total=0;
    iIFErr=0;

    BySiteFail=0;                                                               //kevin 20130710 by site yield record alarm 後清除}
    BySitePass=0;                                                               //kevin 20130710 by site yield record alarm 後清除}
    BySiteTotal=0;                                                              //kevin 20130710 by site yield record alarm 後清除}

    iByBinLowYieldPass =0;                                                      //Steven 20140828 : By Bin Yield Monitor
    iByBinArmYieldPass =0;                                                      //Steven 20140828 : By Bin Arm Yield Monitor
    iByBinSiteYieldPass=0;                                                      //Steven 20140828 : By Bin Site Yield Monitor

    for(int i=0; i<TEST_MAX_BIN; i++)
    {
        iBinCT[i]=0;
        sBinPassFail->Strings[i]="0";
        sBinCT      ->Strings[i]="0";
    }
}
//------------------------------------------------------------------------------
void _fastcall TMySocket::ClearBySite()
{
    BySiteFail=0;                                                               //kevin 20130710 by site yield record alarm 後清除}
    BySitePass=0;                                                               //kevin 20130710 by site yield record alarm 後清除}
    BySiteTotal=0;                                                              //kevin 20130710 by site yield record alarm 後清除}
}
//------------------------------------------------------------------------------
void _fastcall TMySocket::SetbBinCodeStatus(int Bin, int Status, bool bByBinLowYieldPass, bool bByBinArmYieldPass, bool ByBinSiteYieldPass)//set BinPassFail
{
    SetBinPassFail[Bin]             =Status;
    sBinPassFail->Strings[Bin]      =AnsiString(Status);
    SetByBinLowYieldPassFail[Bin]   =bByBinLowYieldPass;                        //Steven 20140828 : By Bin Yield Monitor
    SetByBinArmYieldPassFail[Bin]   =bByBinArmYieldPass;                        //Steven 20140828 : By Bin Arm Yield Monitor
    SetByBinSiteYieldPassFail[Bin]  =ByBinSiteYieldPass;                        //Steven 20140828 : By Bin Site Yield Monitor
}
//------------------------------------------------------------------------------
void _fastcall TMySocket::SetTesterBin(int BinValue)
{
    if(BinValue>=iTestBinCount || BinValue<0)                                   //Steven 20140509 : Modify
    {
        iIFErr++;
    }
    else
    {
        iBinCT[BinValue]++;
        if(SetByBinLowYieldPassFail[BinValue])                                  //Steven 20140828 : By Bin Yield Monitor
        {
            iByBinLowYieldPass++;
        }

        if(SetByBinArmYieldPassFail[BinValue])                                  //Steven 20140828 : By Bin Arm Yield Monitor
        {
            iByBinArmYieldPass++;
        }

        if(SetByBinSiteYieldPassFail[BinValue])                                 //Steven 20140828 : By Bin Site Yield Monitor
        {
            iByBinSiteYieldPass++;
        }

        sBinCT->Strings[BinValue]=AnsiString(iBinCT[BinValue]);
    }

    if(BinValue>=iTestBinCount || BinValue<0)                                   //Steven 20111026 : 記憶體破壞      //Steven 20140424 : iMaxBin --> iTestBinCount
    {
        Fail++;
        BySiteFail++;                                                           //kevin 20130710 by site yield record alarm 後清除}
    }
    else if(SetBinPassFail[BinValue]==1)                                        //pass //Steven 20240701 : 0 --> 1 (Pass為 1)
    {
        Pass++;
        BySitePass++;                                                           //kevin 20130710 by site yield record alarm 後清除
    }
    else
    {
        Fail++;
        BySiteFail++;                                                           //kevin 20130710 by site yield record alarm 後清除
    }

    Total++;
    BySiteTotal++;                                                              //kevin 20130710 by site yield record alarm 後清除
}
//------------------------------------------------------------------------------
unsigned long _fastcall TMySocket::GetSelBinCT(int Bin)
{
    return iBinCT[Bin];
}
//------------------------------------------------------------------------------
unsigned long _fastcall TMySocket::GetSelTrayCT(int iTray)
{
    int iT6=0;
    int iTrayCnt=0;
    for(int i=0; i<iTestBinCount; i++)
    {
        iT6=(iT6>iTestBinCount-1)?Prod.iIfErrorT6:Prod.iT6CatData[i];
        if(iT6==iTray)
        {
            iTrayCnt+=iBinCT[i];
        }
    }

    if(iTray==Prod.iIfErrorT6)
    {
        iTrayCnt+=iIFErr;
    }

    return iTrayCnt;
}
//------------------------------------------------------------------------------
unsigned long _fastcall TMySocket::GetTotal()
{
    return Pass+Fail;
}
//------------------------------------------------------------------------------
double _fastcall TMySocket::GetPCA()
{
    double dRetrun=ChangeToFloat(double(Pass), double(Total));                  //Steven 20250820 : 針對除以0加上保護
    return dRetrun;
}
//------------------------------------------------------------------------------
double _fastcall TMySocket::GetBySitePCA()                                      //kevin 20130710 by head sit計數
{
    double dRetrun=ChangeToFloat(double(BySitePass), double(BySiteTotal));
    return dRetrun;
}
//------------------------------------------------------------------------------
unsigned long _fastcall TMySocket::GetPassCT()
{
    return Pass;
}
//------------------------------------------------------------------------------
unsigned long _fastcall TMySocket::GetFailCT()
{
    return Fail;
}
//------------------------------------------------------------------------------
unsigned long _fastcall TMySocket::GetIFError()
{
    return iIFErr;
}
//------------------------------------------------------------------------------
void _fastcall TMySocket::SetPassCT(double Value)
{
    Pass=Value;
    Total=Pass+Fail;

    if(Value==0)                                                                //Steven 20140926 : 修正Yield顯示超過100%的問題
    {
        iByBinLowYieldPass =0;                                                  //Steven 20140828 : By Bin Yield Monitor
        iByBinArmYieldPass =0;                                                  //Steven 20140828 : By Bin Arm Yield Monitor
        iByBinSiteYieldPass=0;                                                  //Steven 20140828 : By Bin Site Yield Monitor
    }

    BySitePass=Value;                                                           //kevin 20130710 by HEAD Site
    BySiteTotal=BySitePass+BySiteFail;                                          //kevin 20130710 by Head Site
}
//------------------------------------------------------------------------------
void _fastcall TMySocket::SetFailCT(double Value)
{
    Fail=Value;
    Total=Pass+Fail;

    if(Value==0)                                                                //Steven 20140926 : 修正Yield顯示超過100%的問題
    {
        iByBinLowYieldPass =0;                                                  //Steven 20140828 : By Bin Yield Monitor
        iByBinArmYieldPass =0;                                                  //Steven 20140828 : By Bin Arm Yield Monitor
        iByBinSiteYieldPass=0;                                                  //Steven 20140828 : By Bin Site Yield Monitor
    }

    BySiteFail=Value;                                                           //kevin 20130710 by Head Site
    BySiteTotal=BySitePass+BySiteFail;                                          //kevin 20130710 by Head Site
}
//------------------------------------------------------------------------------
void _fastcall TMySocket::SetIFErr(double Value)                                //Steven 20110801
{
    iIFErr=Value;
}
//------------------------------------------------------------------------------
void _fastcall TMySocket::SetBinCT(int Bin, double Value)
{
    iBinCT[Bin]=Value;
    sBinCT->Strings[Bin]=AnsiString(Value);
}
//------------------------------------------------------------------------------
unsigned long _fastcall TMySocket::GetByBinLowYieldPassCT()                     //Steven 20140828 : By Bin Yield Monitor
{
    return iByBinLowYieldPass;
}
//------------------------------------------------------------------------------
unsigned long _fastcall TMySocket::GetByBinArmYieldPassCT()                     //Steven 20140828 : By Bin Arm Yield Monitor
{
    return iByBinArmYieldPass;
}
//------------------------------------------------------------------------------
unsigned long _fastcall TMySocket::GetByBinSiteYieldPassCT()                    //Steven 20140828 : By Bin Site Yield Monitor
{
    return iByBinSiteYieldPass;
}
//------------------------------------------------------------------------------
void _fastcall TMySocket::SetByBinLowYieldPassCT(unsigned long Value)           //Steven 20140828 : By Bin Yield Monitor
{
    iByBinLowYieldPass=Value;
}
//------------------------------------------------------------------------------
void _fastcall TMySocket::SetByBinArmYieldPassCT(unsigned long Value)           //Steven 20140828 : By Bin Arm Yield Monitor
{
    iByBinArmYieldPass=Value;
}
//------------------------------------------------------------------------------
void _fastcall TMySocket::SetByBinSiteYieldPassCT(unsigned long Value)          //Steven 20140828 : By Bin Site Yield Monitor
{
    iByBinSiteYieldPass=Value;
}
//------------------------------------------------------------------------------
double _fastcall TMySocket::GetByBinLowYieldPassPCA()                           //Steven 20140828 : By Bin Yield Monitor
{
    double dRetrun=ChangeToFloat(double(iByBinLowYieldPass), double(Total));    //Steven 20250820 : 針對除以0加上保護
    return dRetrun;
}
//------------------------------------------------------------------------------
double _fastcall TMySocket::GetByBinArmYieldPassPCA()                           //Steven 20140828 : By Bin Arm Yield Monitor
{
    double dRetrun=ChangeToFloat(double(iByBinArmYieldPass), double(Total));
    return dRetrun;
}
//------------------------------------------------------------------------------
double _fastcall TMySocket::GetByBinSiteYieldPassPCA()                          //Steven 20140828 : By Bin Site Yield Monitor
{
    double dRetrun=ChangeToFloat(double(iByBinSiteYieldPass), double(Total));
    return dRetrun;
}
//------------------------------------------------------------------------------
//this is Class TArm Start
//------------------------------------------------------------------------------
_fastcall TArm::TArm(AnsiString FileName)                                       //Steven 20110801 : 改成初始化後讀檔
{
    sPass =new TStringList();
    sFail =new TStringList();
    sTotal=new TStringList();
    iIFErr=new TStringList();

    Pass=0;
    Fail=0;
    Total=0;
    iSKETInArmCT=0;
    for(int i=0; i<MAX_SOCKET_ROW; i++)
    {
        for(int j=0; j<MAX_SOCKET_COL; j++)
        {
//        for(j=0; j<NEW_MAX_Index_Col; j++)                                    //Steven 20110420 : 用這個會項賽
            sPass ->Add("0");
            sFail ->Add("0");
            sTotal->Add("0");
            iIFErr->Add("0");

            ArmSKET[i][j]=new TMySocket();
        }
    }
    iContactCT=0;
    Name=FileName;
    bHasFile=false;
}
//------------------------------------------------------------------------------
void _fastcall TArm::ReadFile()                                                 //Steven 20110801 : 改成初始化後讀檔
{
    AnsiString asFileName="D:\\HT9045\\system\\"+Name+".dat";
    AnsiString asBackFile="D:\\HT9045\\system\\"+Name+"_backup.dat";
    MyForceDirectories("D:\\HT9045\\system\\");
    bHasFile=FileExists(asFileName);

    unsigned long size=0;
    unsigned long temp[MAX_SOCKET_ROW][MAX_SOCKET_COL][19]={0};                 //jou 2012-09-23 預設值為0
    unsigned long tempMax[4][8][TEST_MAX_BIN]={0};                              //jou 2012-09-23 預設值為0
    unsigned long tempMax4[4][8][TEST_MAX_BIN+4]={0};                           //jou 2014-04-22 因為RS232 100 BIN需要再加4
    unsigned long tempMax5[4][8][104]={0};  //kevin 20140614
    AnsiString asIniFileName="D:\\HT9045\\system\\"+Name+".ini";
    AnsiString Str;

    if(bHasFile)
    {
        FILE *Fp=fopen(asFileName.c_str(), "rb");

        if(Fp!=NULL)
        {
            fseek(Fp, 0L, SEEK_END);
            size=ftell(Fp);
            fseek(Fp, 0L, SEEK_SET);

            if(size==2432)
                fread((char *)&temp[0], sizeof(temp), 1, Fp);
            else if(size==12800)
                fread((char *)&tempMax[0], sizeof(tempMax), 1, Fp);
            else if(size==13312)                                                //kevin 20140614
                fread((char *)&tempMax5[0], sizeof(tempMax5), 1, Fp);
            else
                fread((char *)&tempMax4[0], sizeof(tempMax4), 1, Fp);

            fclose(Fp);

            if(FileExists(asBackFile))
            {
                if(FileDataCompare(asFileName.c_str(), asBackFile.c_str())==false)
                {
                    Fp=fopen(asBackFile.c_str(), "rb");

                    if(Fp!=NULL)
                    {
                        fseek(Fp, 0L, SEEK_END);
                        size=ftell(Fp);
                        fseek(Fp, 0L, SEEK_SET);

                        if(size==2432)
                            fread((char *)&temp[0],sizeof(temp), 1, Fp);
                        else if(size==12800)
                            fread((char *)&tempMax[0],sizeof(tempMax), 1, Fp);
                        else if(size==13312)                                    //kevin 20140614
                            fread((char *)&tempMax5[0], sizeof(tempMax5), 1, Fp);
                        else
                            fread((char *)&tempMax4[0],sizeof(tempMax4), 1, Fp);
                        fclose(Fp);
                    }
                    else
                    {
                        bHasFile=false;
                    }
                }
            }
        }
        else
        {
            Fp=fopen(asBackFile.c_str(), "rb");

            if(Fp!=NULL)
            {
                fseek(Fp, 0L, SEEK_END);
                size=ftell(Fp);
                fseek(Fp, 0L, SEEK_SET);

                if(size==2432)
                    fread((char *)&temp[0], sizeof(temp), 1, Fp);
                else if(size==12800)
                    fread((char *)&tempMax[0], sizeof(tempMax), 1, Fp);
                else if(size==13312)                         //kevin 20140614
                    fread((char *)&tempMax5[0], sizeof(tempMax5), 1, Fp);
                else
                    fread((char *)&tempMax4[0], sizeof(tempMax4), 1, Fp);

                fclose(Fp);
            }
            else
            {
                bHasFile=false;
            }
        }
    }
    else
    {
        bHasFile=false;
    }

    if(bHasFile==true)
    {
        for(int i=0; i<MAX_SOCKET_ROW; i++)
        {
            for(int j=0; j<MAX_SOCKET_COL; j++)
            {                                                                   //Steven 20140509 : Modify Function
                if(size==2432)
                {
                    SetPassCT(i, j, temp[i][j][0]);
                    SetFailCT(i, j, temp[i][j][1]);
                    ArmSKET[i][j]->GetTotal();
                    for(int k=0; k<15; k++)                                     //Steven 20140616 : iTestBinCount --> 15
                        SetBinCT(i, j, k, temp[i][j][3+k]);
                    SetIFErr(i, j, temp[i][j][18]);
                }
                else if(size==12800)
                {
                    SetPassCT(i, j, tempMax[i][j][0]);
                    SetFailCT(i, j, tempMax[i][j][1]);
                    ArmSKET[i][j]->GetTotal();
                    for(int k=0; k<iTestBinCount; k++)                          //Steven 20121112 : RS232支援32Bin 15 --> iTestBinCount
                        SetBinCT(i, j, k, tempMax[i][j][3+k]);
                    SetIFErr(i, j, tempMax[i][j][iTestBinCount+3]);
                }
                else if(size==13312)                                            //kevin 20140614
                {
                    SetPassCT(i, j, tempMax5[i][j][0]);
                    SetFailCT(i, j, tempMax5[i][j][1]);
                    ArmSKET[i][j]->GetTotal();
                    for(int k=0; k<iTestBinCount; k++)                          //Steven 20121112 : RS232支援32Bin 15 --> iTestBinCount
                        SetBinCT(i, j, k, tempMax5[i][j][3+k]);
                    SetIFErr(i, j, tempMax5[i][j][iTestBinCount+3]);
                }
                else
                {
                    SetPassCT(i, j, tempMax4[i][j][0]);
                    SetFailCT(i, j, tempMax4[i][j][1]);
                    ArmSKET[i][j]->GetTotal();
                    for(int k=0; k<iTestBinCount; k++)                          //Steven 20121112 : RS232支援32Bin 15 --> iTestBinCount
                        SetBinCT(i, j, k, tempMax4[i][j][3+k]);
                    SetIFErr(i, j, tempMax4[i][j][iTestBinCount+3]);
                }

                if(TestIF_File.bLowYieldAlarmByBin)
                {
                    Str.sprintf("Site%d-%d", i, j);
                    ArmSKET[i][j]->SetByBinLowYieldPassCT(CheckAndReadIniData(asIniFileName, "iByBinLowYieldPass",  Str, 0));                   //Steven 20140828 : By Bin Yield Monitor
                    ArmSKET[i][j]->SetByBinArmYieldPassCT(CheckAndReadIniData(asIniFileName, "iByBinArmYieldPass",  Str, 0));
                    ArmSKET[i][j]->SetByBinSiteYieldPassCT(CheckAndReadIniData(asIniFileName, "iByBinSiteYieldPass", Str, 0));
                }
            }
        }
    }
    else
    {
        for(int i=0; i<MAX_SOCKET_ROW; i++)
        {
            for(int j=0; j<MAX_SOCKET_COL; j++)
            {
                SetPassCT(i, j, 0);
                SetFailCT(i, j, 0);
                ArmSKET[i][j]->GetTotal();
                for(int k=0; k<iTestBinCount; k++)                              //Steven 20121112 : RS232支援32Bin 15 --> iTestBinCount
                    SetBinCT(i, j, k, 0);
                SetIFErr(i, j, 0);

                Str.sprintf("Site%d-%d", i, j);
                ArmSKET[i][j]->SetByBinLowYieldPassCT(0);                       //Steven 20140828 : By Bin Yield Monitor
                ArmSKET[i][j]->SetByBinArmYieldPassCT(0);
                ArmSKET[i][j]->SetByBinSiteYieldPassCT(0);
            }
        }
    }
}
//------------------------------------------------------------------------------
_fastcall TArm::~TArm()
{
    try
    {
        WriteFile();
        for(int i=0; i<MAX_SOCKET_ROW; i++)
            for(int j=0; j<MAX_SOCKET_COL; j++)
    //        for(j=0; j<NEW_MAX_Index_Col; j++)                                //Steven 20110420 : 用這個會項賽
            {
                if(ArmSKET[i][j]!=NULL)                                         //Steven 20161220 (jou) : 修正delete方式
                    delete ArmSKET[i][j];
            }

        sPass->Clear();                                                         //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        sFail->Clear();                                                         //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        sTotal->Clear();                                                        //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        iIFErr->Clear();                                                        //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        delete sPass;
        delete sFail;
        delete sTotal;
        delete iIFErr;
    }
    catch(...)
    {
        MyDBIProcess("Exception", "~TArm");
    }
}
//------------------------------------------------------------------------------
void _fastcall TArm::WriteFile()                                                //Steven 20110801 : 改成初始化後讀檔
{
    AnsiString asFileName="D:\\HT9045\\system\\"+Name+".dat";
    AnsiString asBackFile="D:\\HT9045\\system\\"+Name+"_backup.dat";

    AnsiString asIniFileName="D:\\HT9045\\system\\"+Name+".ini";
    AnsiString Str;
    MyForceDirectories("D:\\HT9045\\system\\");
    bHasFile=FileExists(asFileName);

    unsigned long temp[MAX_SOCKET_ROW][MAX_SOCKET_COL][TEST_MAX_BIN+4]={0};     //jou 2012-09-23 預設值為0        //Steven 20140122 : 記憶體破壞, +4
    for(int i=0; i<MAX_SOCKET_ROW; i++)
    {
        for(int j=0; j<MAX_SOCKET_COL; j++)
        {
            temp[i][j][0]=ArmSKET[i][j]->GetPassCT();
            temp[i][j][1]=ArmSKET[i][j]->GetFailCT();
            temp[i][j][2]=ArmSKET[i][j]->GetTotal();
            for(int k=0; k<iTestBinCount; k++)
                temp[i][j][3+k]=ArmSKET[i][j]->GetSelBinCT(k);
            temp[i][j][iTestBinCount+3]=ArmSKET[i][j]->GetIFError();

            if(TestIF_File.bLowYieldAlarmByBin)
            {
                Str.sprintf("Site%d-%d", i, j);
                WriteIniData(asIniFileName, "iByBinLowYieldPass",  Str, ArmSKET[i][j]->GetByBinLowYieldPassCT());                   //Steven 20140828 : By Bin Yield Monitor
                WriteIniData(asIniFileName, "iByBinArmYieldPass",  Str, ArmSKET[i][j]->GetByBinArmYieldPassCT());
                WriteIniData(asIniFileName, "iByBinSiteYieldPass", Str, ArmSKET[i][j]->GetByBinSiteYieldPassCT());
            }
        }
    }

    FILE *Fp=fopen(asFileName.c_str(), "wb");
    if(Fp!=NULL)
    {
        fwrite((char *)&temp[0], sizeof(temp), 1, Fp);
        fclose(Fp);
    }

    Fp=fopen(asBackFile.c_str(), "wb");
    if(Fp!=NULL)
    {
        fwrite((char *)&temp[0], sizeof(temp), 1, Fp);
        fclose(Fp);
    }
}
//------------------------------------------------------------------------------
double _fastcall TArm::GetPCA()
{
    Total=Pass+Fail;
    return ChangeToFloat(double(Pass), double(Total));
}
//------------------------------------------------------------------------------
double _fastcall TArm::GetBySitePCA()                                           //kevin 20130710 by sit計數
{
    BySiteTotal=BySitePass+BySiteFail;
    return ChangeToFloat(double(BySitePass), double(BySiteTotal));
}
//------------------------------------------------------------------------------
unsigned long _fastcall TArm::GetPassCT()
{
    Pass=0;
    for(int i=0; i<MAX_SOCKET_ROW; i++)
        for(int j=0; j<MAX_SOCKET_COL; j++)
            Pass+=ArmSKET[i][j]->GetPassCT();
    return Pass;
}
//------------------------------------------------------------------------------
unsigned long _fastcall TArm::GetFailCT()
{
    Fail=0;
    for(int i=0; i<MAX_SOCKET_ROW; i++)
        for(int j=0; j<MAX_SOCKET_COL; j++)
            Fail+=ArmSKET[i][j]->GetFailCT();
    return Fail;
}
//------------------------------------------------------------------------------
unsigned long _fastcall TArm::GetTotalCT()
{
    Total=GetPassCT()+GetFailCT();                                              //Steven 20150518 : 修正Contact Count
    return Total;
}
//------------------------------------------------------------------------------
unsigned long _fastcall TArm::GetSelBin(int iBin)                               //Sam 20240131 : 取得此  Arm 的 Bin 數量
{
    int iSum=0;
    for(int i=0; i<MAX_SOCKET_ROW; i++)
        for(int j=0; j<MAX_SOCKET_COL; j++)
            iSum+=ArmSKET[i][j]->GetSelBinCT(iBin);
    return iSum;
}
//------------------------------------------------------------------------------
void _fastcall TArm::ClearALLCT()
{
    int iDut;
    for(int i=0; i<MAX_SOCKET_ROW; i++)
    {
        for(int j=0; j<MAX_SOCKET_COL; j++)
        {
            ArmSKET[i][j]->ClearALLCT();
            iDut=i*MAX_SOCKET_COL+j;
            sTotal->Strings[iDut]=AnsiString(ArmSKET[i][j]->GetTotal());
            sPass ->Strings[iDut]=AnsiString(ArmSKET[i][j]->GetPassCT());
            sFail ->Strings[iDut]=AnsiString(ArmSKET[i][j]->GetFailCT());
            iIFErr->Strings[iDut]=AnsiString(ArmSKET[i][j]->GetIFError());
        }
    }
}
//------------------------------------------------------------------------------
void _fastcall TArm::ClearALLCT(int ROW, int COL)                               //Steven 20140509 : For Secs GEM
{
    ArmSKET[ROW][COL]->ClearALLCT();
    int iDut=ROW*MAX_SOCKET_COL+COL;
    sTotal->Strings[iDut]=AnsiString(ArmSKET[ROW][COL]->GetTotal());
    sPass ->Strings[iDut]=AnsiString(ArmSKET[ROW][COL]->GetPassCT());
    sFail ->Strings[iDut]=AnsiString(ArmSKET[ROW][COL]->GetFailCT());
    iIFErr->Strings[iDut]=AnsiString(ArmSKET[ROW][COL]->GetIFError());
}
//------------------------------------------------------------------------------
void _fastcall TArm::ClearBySite()                                              //kevin 20130710
{
    int iDut;
    for(int i=0; i<MAX_SOCKET_ROW; i++)
    {
        for(int j=0; j<MAX_SOCKET_COL; j++)
        {
            ArmSKET[i][j]->ClearALLCT();
            iDut=i*MAX_SOCKET_COL+j;
            sTotal->Strings[iDut]=AnsiString(ArmSKET[i][j]->GetTotal());
            sPass ->Strings[iDut]=AnsiString(ArmSKET[i][j]->GetPassCT());
            sFail ->Strings[iDut]=AnsiString(ArmSKET[i][j]->GetFailCT());
            iIFErr->Strings[iDut]=AnsiString(ArmSKET[i][j]->GetIFError());
        }
    }
}
//------------------------------------------------------------------------------
void _fastcall TArm::SetArmBinCodeStatus(int ROW, int COL, int BinValue, int Status, bool bByBinLowYieldPass, bool bByBinArmYieldPass, bool ByBinSiteYieldPass)
{
    ArmSKET[ROW][COL]->SetbBinCodeStatus(BinValue, Status, bByBinLowYieldPass, bByBinArmYieldPass, ByBinSiteYieldPass);
}
//------------------------------------------------------------------------------
void _fastcall TArm::SetArmSKTData(int ROW, int COL, int Data)
{
    ArmSKET[ROW][COL]->SetTesterBin(Data);
    int iDut=ROW*MAX_SOCKET_COL+COL;
    sTotal->Strings[iDut]=AnsiString(ArmSKET[ROW][COL]->GetTotal());
    sPass ->Strings[iDut]=AnsiString(ArmSKET[ROW][COL]->GetPassCT());
    sFail ->Strings[iDut]=AnsiString(ArmSKET[ROW][COL]->GetFailCT());
    iIFErr->Strings[iDut]=AnsiString(ArmSKET[ROW][COL]->GetIFError());
    Total=GetPassCT()+GetFailCT();                                              //Steven 20150518 : 修正Contact Count
}
//------------------------------------------------------------------------------
void _fastcall TArm::InitContactCT()
{
    iContactCT=0;
}
//------------------------------------------------------------------------------
void _fastcall TArm::SetContactCT(int iCT)
{
    iContactCT+=iCT;
}
//------------------------------------------------------------------------------
unsigned long _fastcall TArm::GetContactCT()
{
    return iContactCT;
}
//------------------------------------------------------------------------------
void _fastcall TArm::SetPassCT(int ROW, int COL, double Value)                  //Steven 20140509 : For Secs GEM
{
    ArmSKET[ROW][COL]->SetPassCT(Value);
    int iDut=ROW*MAX_SOCKET_COL+COL;
    sPass ->Strings[iDut]=AnsiString(Value);
    sTotal->Strings[iDut]=AnsiString(ArmSKET[ROW][COL]->GetTotal());
}
//------------------------------------------------------------------------------
void _fastcall TArm::SetFailCT(int ROW, int COL, double Value)                  //Steven 20140509 : For Secs GEM
{
    ArmSKET[ROW][COL]->SetFailCT(Value);
    int iDut=ROW*MAX_SOCKET_COL+COL;
    sFail ->Strings[iDut]=AnsiString(Value);
    sTotal->Strings[iDut]=AnsiString(ArmSKET[ROW][COL]->GetTotal());
}
//------------------------------------------------------------------------------
void _fastcall TArm::SetBinCT(int ROW, int COL, int Bin, double Value)          //Steven 20140509 : For Secs GEM
{
    ArmSKET[ROW][COL]->SetBinCT(Bin, Value);
    int iDut=ROW*MAX_SOCKET_COL+COL;
    sTotal->Strings[iDut]=AnsiString(ArmSKET[ROW][COL]->GetTotal());
    sPass ->Strings[iDut]=AnsiString(ArmSKET[ROW][COL]->GetPassCT());
    sFail ->Strings[iDut]=AnsiString(ArmSKET[ROW][COL]->GetFailCT());
    iIFErr->Strings[iDut]=AnsiString(ArmSKET[ROW][COL]->GetIFError());
}
//------------------------------------------------------------------------------
void _fastcall TArm::SetIFErr(int ROW, int COL, double Value)                   //Steven 20140509 : For Secs GEM
{
    ArmSKET[ROW][COL]->SetIFErr(Value);
    int iDut=ROW*MAX_SOCKET_COL+COL;
    iIFErr->Strings[iDut]=AnsiString(Value);
    sTotal->Strings[iDut]=AnsiString(ArmSKET[ROW][COL]->GetTotal());
}
//------------------------------------------------------------------------------
double _fastcall TArm::GetByBinLowYieldPCA()                                    //Steven 20141125
{
    double Sum=0.0;
    double Sum2=0.0;

    for(int i=0; i<MAX_Index_Row; i++)                                          //Steven 20141125
    {
        for(int j=0; j<NEW_MAX_Index_Col; j++)
        {
            Sum+=ArmSKET[i][j]->GetByBinLowYieldPassCT();
        }
    }
    Sum2=GetTotalCT();

    return ChangeToFloat((double)Sum*100.0, (double)Sum2);
}
//------------------------------------------------------------------------------
_fastcall TLotSummary::TLotSummary()
{
    ClearAllData();
}
//------------------------------------------------------------------------------
_fastcall TLotSummary::~TLotSummary()
{
}
//------------------------------------------------------------------------------
void TLotSummary::ClearAllData()
{
    ZeroMemory(iCountCategory, sizeof(iCountCategory));
    ZeroMemory(iTotalCategory, sizeof(iTotalCategory));
    ZeroMemory(iLastTotalCategory, sizeof(iLastTotalCategory));
    LotSummary.iLoadTotal=0;
    iE1Count=0;                                                                 //JerryYang 20230322 : Lot summary要計算各類型ERR的數量
    iE2Count=0;
    iE3Count=0;
}
//------------------------------------------------------------------------------
void TLotSummary::ClearRTData()
{
    for(int k=0; k<TEST_MAX_BIN; k++)
    {
        if(bIsRTBin[k]==true)
        {
            iTotalCategory[k]=0;
            for(int i=0; i<MAX_SOCKET_ROW*MAX_SOCKET_COL; i++)
            {
                iCountCategory[i][k]=0;
            }
        }
    }
}
//------------------------------------------------------------------------------
void TLotSummary::CleariTotalCategory(bool ClearRetest)                         //Eastsun 20260710整合
{
    if(ClearRetest)
    {
        for(int k=0; k<TEST_MAX_BIN; k++)
        {
            int iBinTray = BinSelect[FT_ART].iCatDataT3Pos[k] - 1;

            for(int t=0; t<3; t++)
            {
                if(BinSelect[FT_ART].bAutoRetest[t] == true && iBinTray == t)
                {
                    iTotalCategory[k] = 0;
                    break;
                }
            }
        }
    }
    else
    {
        for(int k=0; k<TEST_MAX_BIN; k++)
        {
            iTotalCategory[k] = 0;
        }
    }
}
//------------------------------------------------------------------------------
void TLotSummary::SetIsRTBin()
{
    int iBinTray;
    for(int i=0; i<TEST_MAX_BIN; i++)
    {
        iBinTray=Prod.iT6CatData[i];
        if(iBinTray>0 &&
           (iBinTray<=iAutoRight &&
            Prod.bART6Tray[iBinTray]==true) ||
           Prod.bCateRTo6Tray[iBinTray]==true)
        {
            bIsRTBin[i]=true;
        }
        else
        {
            bIsRTBin[i]=false;
        }
    }
}
//------------------------------------------------------------------------------
void TLotSummary::AddCount(int iSite, int iBin)                                 //Steven 20190726 : ATK ART Lot Count
{
    iCountCategory[iSite][iBin]++;
    iTotalCategory[iBin]++;
}
//------------------------------------------------------------------------------
void TLotSummary::AddByLotCount(int iSite, int iBin, AnsiString s2DID)
{
    AnsiString Str, str, str2="", str3="", sFileName;

    sFileName.sprintf("D:\\HT9045_Log\\2D_MappingResult\\MultiLot_VS_Result_%s.csv", sTotalLotID);

    TStringList *File, *list2D;
    File=new TStringList();
    list2D=new TStringList();

    MyForceDirectories("D:\\HT9045_Log\\2D_MappingResult\\");
    if(FileExists(sFileName)==true)
    {
        File->LoadFromFile(sFileName);
    }

    for(int i=0; i<File->Count; i++)
    {
        AnsiString s, s1, s2, s3, s4;
        int iPos1, iPos2;
        s=File->Strings[i];

        iPos1=s.Pos(" ");
        iPos2=s.Pos(",");
        if(iPos1>0 && iPos2>0 && iPos1<iPos2)                                   //表示2D有空格
        {
            s1=s.SubString(1, iPos2-1);                                         //2D
            s3=s.SubString(iPos2+1, s.Length());
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
            if(list2D->Count>=4)
            {
                for(int j=0; j<fSCKART->iInfo_MultiLotCnt; j++)
                {
                    if(list2D->Strings[4]==fSCKART->sInfoArr_InnerLotID[j])
                    {
                        iByLotCountCategory[j][iSite][iBin]++;
                        iByLotTotalCategory[j][iBin]++;
                    }
                }
            }
        }
    }

    File->Clear();
    delete File;

    list2D->Clear();
    delete list2D;
}
//------------------------------------------------------------------------------
void TLotSummary::AddByLotLoadCount(AnsiString s2DID)                           //Steven 20190726 : ATK ART Lot Count
{
    AnsiString Str, str, str2="", str3="", sFileName;

    sFileName.sprintf("D:\\HT9045_Log\\2D_MappingResult\\MultiLot_VS_Result_%s.csv", sTotalLotID);

    TStringList *File, *list2D;
    File=new TStringList();
    list2D=new TStringList();

    MyForceDirectories("D:\\HT9045_Log\\2D_MappingResult\\");

    if(FileExists(sFileName)==true)
    {
        File->LoadFromFile(sFileName);
    }

    for(int i=0; i<File->Count; i++)
    {
        AnsiString s, s1, s2, s3, s4;
        int iPos1,iPos2;
        s=File->Strings[i];

        iPos1=s.Pos(" ");
        iPos2=s.Pos(",");
        if(iPos1>0 && iPos2>0 && iPos1<iPos2)                                   //表示2D有空格
        {
            s1=s.SubString(1, iPos2-1);                                         //2D
            s3=s.SubString(iPos2+1, s.Length());
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
            if(list2D->Count>=4)
            {
                for(int j=0; j<fSCKART->iInfo_MultiLotCnt; j++)
                {
                    if(list2D->Strings[4]==fSCKART->sInfoArr_InnerLotID[j])
                    {
                        LotSummary.iByLotLoadCount[j]++;
                    }
                }
            }
        }
    }

    File->Clear();
    delete File;

    list2D->Clear();
    delete list2D;
}
//------------------------------------------------------------------------------
void _fastcall TLotSummary::ReadFile()                                          //Steven 20190726 : ATK ART Lot Count
{
    AnsiString FileName="D:\\HT9045\\System\\LotSummary.csv";
    TStringList *List =new TStringList();
    TStringList *List2=new TStringList();
    int iCount;

    if(FileExists(FileName))
    {
        List->LoadFromFile(FileName);
        iCount=0;
        for(int i=0; i<MAX_SOCKET_ROW*MAX_SOCKET_COL; i++)
        {
            if(List->Count>iCount)
            {
                List2->CommaText=List->Strings[iCount];
                for(int k=0; k<List2->Count; k++)
                {
                    iCountCategory[i][k]=atoi(List2->Strings[k].c_str());
                }
            }
            iCount++;
        }

        if(List->Count>iCount)                                                  //JerryYang 20210209 : 修正重開程式lot summary遺失的問題
        {
            List2->CommaText=List->Strings[iCount];
            for(int k=0; k<List2->Count; k++)
            {
                iTotalCategory[k]=atoi(List2->Strings[k].c_str());
            }
        }
    }
    else
    {
        ClearAllData();
    }
    delete List;
    delete List2;
}
//------------------------------------------------------------------------------
void _fastcall TLotSummary::WriteFile()                                         //Steven 20190726 : ATK ART Lot Count
{
    AnsiString FileName="D:\\HT9045\\System\\LotSummary.csv";
    TStringList *List=new TStringList();
    AnsiString Str;
    List->Clear();

    for(int i=0; i<MAX_SOCKET_ROW*MAX_SOCKET_COL; i++)
    {
        Str="";
        for(int k=0; k<TEST_MAX_BIN; k++)
        {
            if(k!=0)
                Str+=",";
            Str+=AnsiString(iCountCategory[i][k]);
        }
        List->Add(Str);
    }

    Str="";
    for(int k=0; k<TEST_MAX_BIN; k++)
    {
        if(k!=0)
            Str+=",";
        Str+=AnsiString(iTotalCategory[k]);
    }
    List->Add(Str);

    List->SaveToFile(FileName);
    delete List;
}
//------------------------------------------------------------------------------
void TEST_CATEGORY::ClearCount()
{
    ZeroMemory(iCountCategory, sizeof(iCountCategory));
    ZeroMemory(iCountHeadTotal, sizeof(iCountHeadTotal));
    ZeroMemory(iCountSocketTotal, sizeof(iCountSocketTotal));
    ZeroMemory(iCountPassHead, sizeof(iCountPassHead));
    ZeroMemory(iCountPassSocket, sizeof(iCountPassSocket));
    ZeroMemory(iTotalCategory, sizeof(iTotalCategory));

    ZeroMemory(iBySiteCate, sizeof(iBySiteCate));
    ZeroMemory(iBySiteTotal, sizeof(iBySiteTotal));
    ZeroMemory(iBySitePass, sizeof(iBySitePass));
    ZeroMemory(iBySiteFail, sizeof(iBySiteFail));

    ZeroMemory(dBySiteCate, sizeof(dBySiteCate));
    ZeroMemory(dBySitePass, sizeof(dBySitePass));
    ZeroMemory(dBySiteFail, sizeof(dBySiteFail));

    ZeroMemory(iUnloadCnt, sizeof(iUnloadCnt));

    iTotalSocket=0;
    iPassSocket =0;
    iFailSocket =0;
    iRejectCount=0;
    dPassYield  =0.0;
    dFailYield  =0.0;
}
//==============================================================================
void TEST_CATEGORY::UpdataCount(bool bCheckYield)
{
    int iDut=0, iArm=0;
    ClearCount();

    if(bCheckYield==true)
    {
        if(IsNNMode()==None_NN)
        {
            for(int iRow=0; iRow<TestSocket.iShtRow; iRow++)
            {
                for(int iCol=0; iCol<TestSocket.iShtCol; iCol++)
                {
                    if(TestIF.iSiteMap[iRow][iCol]>0)
                    {
                        iDut=TestIF.iSiteMap[iRow][iCol]-1;
                        for(iArm=0; iArm<2; iArm++)
                        {
                            iCountCategory[iArm][iRow][iCol][iTestBinCount]  =ArmData[iArm]->ArmSKET[iRow][iCol]->GetIFError();
                            iTotalCategory[iTestBinCount]                   +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetIFError();
                            for(int iCat=0; iCat<iTestBinCount; iCat++)             //每個category
                            {
                                iCountCategory[iArm][iRow][iCol][iCat]  =ArmData[iArm]->ArmSKET[iRow][iCol]->GetSelBinCT(iCat);
                                iTotalCategory[iCat]                   +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetSelBinCT(iCat);
                                iBySiteCate[iDut][iCat]                +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetSelBinCT(iCat);
                            }
                            iCountSocketTotal[iRow][iCol]      +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetTotal();
                            iCountHeadTotal[iArm][iRow][iCol]  +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetTotal();
                            iTotalSocket                       +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetTotal();
                            iBySiteTotal[iDut]                 +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetTotal();
                            iCountPassSocket[iRow][iCol]       +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetPassCT();
                            iCountPassHead[iArm][iRow][iCol]   +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetPassCT();
                            iPassSocket                        +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetPassCT();
                            iBySitePass[iDut]                  +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetPassCT();
                        }
                    }
                }
            }
        }
        else if(IsNNMode()==NN_1Row)
        {
            for(int iRow=0; iRow<TestSocket.iShtRow; iRow++)
            {
                for(int iCol=0; iCol<TestSocket.iShtCol; iCol++)
                {
                    if(TestIF.iSiteMap[iRow][iCol]>0)
                    {
                        iArm=(iRow==0)?1:0;
                        iDut=TestIF.iSiteMap[iRow][iCol]-1;
                        iCountCategory[iArm][iRow][iCol][iTestBinCount]  =ArmData[iArm]->ArmSKET[iRow][iCol]->GetIFError();
                        iTotalCategory[iTestBinCount]                   +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetIFError();
                        for(int iCat=0; iCat<iTestBinCount; iCat++)
                        {
                            iCountCategory[iArm][iRow][iCol][iCat]  =ArmData[iArm]->ArmSKET[iRow][iCol]->GetSelBinCT(iCat);
                            iTotalCategory[iCat]                   +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetSelBinCT(iCat);
                            iBySiteCate[iDut][iCat]                +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetSelBinCT(iCat);
                        }
                        iCountSocketTotal[iRow][iCol]      +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetTotal();
                        iCountHeadTotal[iArm][iRow][iCol]  +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetTotal();
                        iTotalSocket                       +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetTotal();
                        iBySiteTotal[iDut]                 +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetTotal();
                        iCountPassSocket[iRow][iCol]       +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetPassCT();
                        iCountPassHead[iArm][iRow][iCol]   +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetPassCT();
                        iPassSocket                        +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetPassCT();
                        iBySitePass[iDut]                  +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetPassCT();
                    }
                }
            }
        }
        else
        {
            for(int iRow=0; iRow<TestSocket.iShtRow; iRow++)
            {
                for(int iCol=0; iCol<TestSocket.iShtCol; iCol++)
                {
                    if(TestIF.iSiteMap[iRow][iCol]>0)
                    {
                        iArm=(iRow==0 || iRow==1)?1:0;
                        iDut=TestIF.iSiteMap[iRow][iCol]-1;
                        iCountCategory[iArm][iRow][iCol][iTestBinCount]  =ArmData[iArm]->ArmSKET[iRow][iCol]->GetIFError();
                        iTotalCategory[iTestBinCount]                   +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetIFError();
                        for(int iCat=0; iCat<iTestBinCount; iCat++)
                        {
                            iCountCategory[iArm][iRow][iCol][iCat]  =ArmData[iArm]->ArmSKET[iRow][iCol]->GetSelBinCT(iCat);
                            iTotalCategory[iCat]                   +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetSelBinCT(iCat);
                            iBySiteCate[iDut][iCat]                +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetSelBinCT(iCat);
                        }
                        iCountSocketTotal[iRow][iCol]      +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetTotal();
                        iCountHeadTotal[iArm][iRow][iCol]  +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetTotal();
                        iTotalSocket                       +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetTotal();
                        iBySiteTotal[iDut]                 +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetTotal();
                        iCountPassSocket[iRow][iCol]       +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetPassCT();
                        iCountPassHead[iArm][iRow][iCol]   +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetPassCT();
                        iPassSocket                        +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetPassCT();
                        iBySitePass[iDut]                  +=ArmData[iArm]->ArmSKET[iRow][iCol]->GetPassCT();
                    }
                }
            }
        }
    }
    else                                                                                //Steven 20250603 : by lot summary
    {
        if(IsNNMode()==None_NN)
        {
            for(int iRow=0; iRow<TestSocket.iShtRow; iRow++)
            {
                for(int iCol=0; iCol<TestSocket.iShtCol; iCol++)
                {
                    if(TestIF.iSiteMap[iRow][iCol]>0)
                    {
                        iDut=TestIF.iSiteMap[iRow][iCol]-1;
                        for(iArm=0; iArm<2; iArm++)
                        {
                            iCountCategory[iArm][iRow][iCol][iTestBinCount]  =ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetIFError();
                            iTotalCategory[iTestBinCount]                   +=ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetIFError();
                            for(int iCat=0; iCat<iTestBinCount; iCat++)             //每個category
                            {
                                iCountCategory[iArm][iRow][iCol][iCat]  =ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetSelBinCT(iCat);
                                iTotalCategory[iCat]                   +=ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetSelBinCT(iCat);
                                iBySiteCate[iDut][iCat]                +=ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetSelBinCT(iCat);
                            }
                            iCountSocketTotal[iRow][iCol]      +=ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetTotal();
                            iCountHeadTotal[iArm][iRow][iCol]  +=ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetTotal();
                            iTotalSocket                       +=ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetTotal();
                            iBySiteTotal[iDut]                 +=ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetTotal();
                            iCountPassSocket[iRow][iCol]       +=ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetPassCT();
                            iCountPassHead[iArm][iRow][iCol]   +=ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetPassCT();
                            iPassSocket                        +=ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetPassCT();
                            iBySitePass[iDut]                  +=ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetPassCT();
                        }
                    }
                }
            }
        }
        else if(IsNNMode()==NN_1Row)
        {
            for(int iRow=0; iRow<TestSocket.iShtRow; iRow++)
            {
                for(int iCol=0; iCol<TestSocket.iShtCol; iCol++)
                {
                    if(TestIF.iSiteMap[iRow][iCol]>0)
                    {
                        iArm=(iRow==0)?1:0;
                        iDut=TestIF.iSiteMap[iRow][iCol]-1;
                        iCountCategory[iArm][iRow][iCol][iTestBinCount]  =ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetIFError();
                        iTotalCategory[iTestBinCount]                   +=ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetIFError();
                        for(int iCat=0; iCat<iTestBinCount; iCat++)
                        {
                            iCountCategory[iArm][iRow][iCol][iCat]  =ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetSelBinCT(iCat);
                            iTotalCategory[iCat]                   +=ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetSelBinCT(iCat);
                            iBySiteCate[iDut][iCat]                +=ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetSelBinCT(iCat);
                        }
                        iCountSocketTotal[iRow][iCol]      +=ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetTotal();
                        iCountHeadTotal[iArm][iRow][iCol]  +=ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetTotal();
                        iTotalSocket                       +=ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetTotal();
                        iBySiteTotal[iDut]                 +=ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetTotal();
                        iCountPassSocket[iRow][iCol]       +=ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetPassCT();
                        iCountPassHead[iArm][iRow][iCol]   +=ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetPassCT();
                        iPassSocket                        +=ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetPassCT();
                        iBySitePass[iDut]                  +=ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetPassCT();
                    }
                }
            }
        }
        else
        {
            for(int iRow=0; iRow<TestSocket.iShtRow; iRow++)
            {
                for(int iCol=0; iCol<TestSocket.iShtCol; iCol++)
                {
                    if(TestIF.iSiteMap[iRow][iCol]>0)
                    {
                        iArm=(iRow==0 || iRow==1)?1:0;
                        iDut=TestIF.iSiteMap[iRow][iCol]-1;
                        iCountCategory[iArm][iRow][iCol][iTestBinCount]  =ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetIFError();
                        iTotalCategory[iTestBinCount]                   +=ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetIFError();
                        for(int iCat=0; iCat<iTestBinCount; iCat++)
                        {
                            iCountCategory[iArm][iRow][iCol][iCat]  =ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetSelBinCT(iCat);
                            iTotalCategory[iCat]                   +=ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetSelBinCT(iCat);
                            iBySiteCate[iDut][iCat]                +=ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetSelBinCT(iCat);
                        }
                        iCountSocketTotal[iRow][iCol]      +=ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetTotal();
                        iCountHeadTotal[iArm][iRow][iCol]  +=ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetTotal();
                        iTotalSocket                       +=ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetTotal();
                        iBySiteTotal[iDut]                 +=ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetTotal();
                        iCountPassSocket[iRow][iCol]       +=ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetPassCT();
                        iCountPassHead[iArm][iRow][iCol]   +=ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetPassCT();
                        iPassSocket                        +=ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetPassCT();
                        iBySitePass[iDut]                  +=ArmDataLot[iArm]->ArmSKET[iRow][iCol]->GetPassCT();
                    }
                }
            }
        }
    }

    iFailSocket=iTotalSocket-iPassSocket;
    for(iDut=0; iDut<MAX_SOCKET_ROW*MAX_SOCKET_COL; iDut++)
    {
        iBySiteFail[iDut]=iBySiteTotal[iDut]-iBySitePass[iDut];
    }

    for(int i=0; i<eTrayCount; i++)
    {
        if(Prod.iTrayType[i]!=tNotUse)
        {
            for(int j=0; j<=iTestBinCount; j++)
            {
                int temp=Prod.iT6PosCate[j];
                if(temp<=0 && j!=iTestBinCount)
                    continue;
                if(i==temp-1)
                {
                    iUnloadCnt[i]+=iTotalCategory[j];
                }

                if(Prod.iIfErrorT6==i && j==iTestBinCount)
                {
                    iUnloadCnt[i]+=iTotalCategory[j];
                }
            }
        }
    }

    UpdataYield();
}
//==============================================================================
void TEST_CATEGORY::UpdataYield()
{
    for(int iDut=0; iDut<MAX_SOCKET_ROW*MAX_SOCKET_COL; iDut++)
    {
        dBySitePass[iDut]=ChangeToFloat((double)iBySitePass[iDut], (double)iBySiteTotal[iDut]);
        dBySiteFail[iDut]=ChangeToFloat((double)iBySiteFail[iDut], (double)iBySiteTotal[iDut]);
        for(int iCat=0; iCat<iTestBinCount; iCat++)
        {
            dBySiteCate[iDut][iCat]=ChangeToFloat((double)iBySiteCate[iDut][iCat], (double)iBySiteTotal[iDut]);
        }
    }

    dPassYield=ChangeToFloat((double)iPassSocket, (double)iTotalSocket);
    dFailYield=ChangeToFloat((double)iFailSocket, (double)iTotalSocket);
}
//==============================================================================
