//---------------------------------------------------------------------------
#include "MachineDefine.h"
#pragma hdrstop

#include "DataCompileForm.h"
#include "common.h"
#include "cmydef.h"
#include "cpublic.h"
#include "myQwertyKeyBoard.h"
#include "MyKitSuck.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDataCompile *DataCompile;
//---------------------------------------------------------------------------
__fastcall TDataCompile::TDataCompile(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TDataCompile::btnCompareClick(TObject *Sender)
{
    iMachType =400;                                                             //kevin 20180131 機型

    AnsiString S, str="", SBin[255]={""}, sSiteMapMachine[4], sSiteMapStand[4], sBuffer="";
    AnsiString szDir="", KeyInData=edtSiteMap->Text, sbuffer[10]={""}, KeyInBin=edtBinMap->Text, sErrorData="";
    int iTemp=0, iSoakTime=0, iPinCount=0;
    int iSitMap=0, ibuffer=0, ik=0;
    int iBinError[10]={0};

    if(iMachType<=400)
    {
        //溫度
        sbuffer[0]="";
        szDir=GetRecipeFileName("HandlerCondition.Data");
        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            sSiteMapMachine[i]="";
            sSiteMapStand[i]="";
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                //if(ik<iSteTotal)
                //{
                    str.sprintf("Site %c%c", i+'A', j+'A');
                    iSitMap=ReadIniData(szDir, "Configuration", str,0);
                    sSiteMapMachine[i]+=IntToStr(iSitMap);
                    ibuffer=GetDataSpecial(KeyInData);
                    sSiteMapStand[i]+= IntToStr(ibuffer);
                    if(iSitMap!=ibuffer)
                    {
                        //Label9->Caption="site Map error";
                        //return;
                        iBinError[0]=1;
                    }
                //}
                //ik++;
            }
        }

        if(iBinError[0]==1)
        {
            if(TestSocket.iShtRow==1)
            {
                sSiteMapStand[1]="00000000";
                sSiteMapStand[2]="00000000";
                sSiteMapStand[3]="00000000";
                sSiteMapMachine[1]="00000000";
                sSiteMapMachine[2]="00000000";
                sSiteMapMachine[3]="00000000";
                sbuffer[0].sprintf("@e020011SiteMap#A:%s;B:%d#A:%s;B:%s",sSiteMapStand[0],sSiteMapStand[1],sSiteMapMachine[0],sSiteMapMachine[1]);
            }

            if(TestSocket.iShtRow==2)
            {
                sSiteMapStand[2]="00000000";
                sSiteMapStand[3]="00000000";
                sSiteMapMachine[2]="00000000";
                sSiteMapMachine[3]="00000000";
                sbuffer[0].sprintf("@e020011SiteMap#A:%s;B:%d#A:%s;B:%s",sSiteMapStand[0],sSiteMapStand[1],sSiteMapMachine[0],sSiteMapMachine[1]);
            }
            else
            {
                sbuffer[0].sprintf("@e020011SiteMap#A:%s;B:%s;C:%s;D:%s#A:%s;B:%s;C:%s;D:%s",sSiteMapStand[0],sSiteMapStand[1],sSiteMapStand[2],sSiteMapStand[3]
                ,sSiteMapMachine[0],sSiteMapMachine[1],sSiteMapMachine[2],sSiteMapMachine[3]);
            }
        }

        sbuffer[1]="";
        szDir=GetRecipeFileName("Binasgn.Data");
        str=ReadIniData(szDir, "Bin Func FT","3617 BinTraySetting",AnsiString("0"));
        iBinError[5]=0;
        int iBinSelCT=6;
        if(TrayForm.iFixTrayMode)
        {
            iBinSelCT=9;    //use up down
        }

        for(int i=0; i<iTestBinCount; i++)           //Bin 別 比對
        {
            if(i<iBinSelCT)
                SBin[i]="";

            ibuffer = GetDataSpecial(KeyInBin);
            iSitMap = GetDataSpecial(str);

            for(int j=0; j<iBinSelCT; j++)
            {
                if(j==ibuffer-1)
                {
                    SBin[j]+=AnsiString(i)+" ";
                }
            }

            if(ibuffer!=iSitMap)
            {
                iBinError[1]=1;
            }
        }

        if(iBinError[1]==1)
        {
            for(int i=0; i<10; i++)
            {
                if(sBinData[i]=="")
                    sBinData[i]="NA";
                if(SBin[i]=="")
                    SBin[i]="NA";
            }

           Label9->Caption="Bin  error";
           sbuffer[1].sprintf("&Binning#Auto1:%s;Auto2:%s;Auto3:%s;Fix1:%s;Fix2:%s;Fix3:%s#Auto1:%s;Auto2:%s;Auto3:%s;Fix1:%s;Fix2:%s;Fix3:%s;",
                              SBin[0],SBin[1],SBin[2],SBin[3],SBin[4],SBin[5],
                              sBinData[0],sBinData[1],sBinData[2],sBinData[3],sBinData[4],sBinData[5]);
        }

        try
        {
            iTemp=StrToInt(edtWorkTemp->Text);
            sbuffer[2]="";
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TDataCompile::Button1Click");
            iTemp=0;
        }

        if(iTemp!=Temperature.fWorkTemperBase)
        {
            Label9->Caption="Tempature error";
            ik= Temperature.fWorkTemperBase;
            sbuffer[2].sprintf("&Temperature:#Temp:%d;#Temp:%d",iTemp,ik);
            iBinError[2]=1;
        }

        //SOAKTIME
        try
        {
            iSoakTime= StrToInt(edtSoakTime->Text);
            sbuffer[3]="";
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TDataCompile::Button1Click");
            iSoakTime=0;
        }

        if(iSoakTime !=Temperature.fSoakTime)
        {
            Label9->Caption="SoakTime error";
            ik= Temperature.fSoakTime;
            sbuffer[3].sprintf("&SoakTime:#Time:%d;#Time:%d",iSoakTime,ik);
            iBinError[3]=1;
        }
        //Pin Force
        try
        {
            S=DeviceForm_File.dPress;
            sbuffer[4]="";
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TDataCompile::Button1Click");
        }

        if(edtForcePerPin->Text!=S)
        {
            Label9->Caption="Pin Force error";
            sbuffer[4].sprintf("&PinForce:#Force:%s;#Force:%s", edtForcePerPin->Text, S);
            iBinError[4]=1;
        }
        //Pin Count
        try
        {
            iPinCount= StrToInt(edtPinCount->Text);
            sbuffer[5]="";
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TDataCompile::Button1Click");
            iPinCount=0;
        }

        if(iPinCount !=DeviceForm_File.iPinCT)
        {
            Label9->Caption="PinCount error";
            sbuffer[5].sprintf("&Pincount:#Count:%d;#Count:%d",iPinCount,DeviceForm_File.iPinCT);
            iBinError[5]=1;
        }
        str.sprintf("@e02011A:");
        S="";
        for(int i=0; i<6; i++)
        {
            if(iBinError[0]==1)
            {
               S+=sbuffer[i];
            }
            else
            {
                if(iBinError[i]==1)
                    str+=sbuffer[i];
            }
        }

        if(iBinError[0]==1)
            sBuffer=S;
        else
            sBuffer=str;
        //sBuffer.sprintf("@e02011A:%s;B:%s#A:%s;B:%s,A2,%d,%d,A3,%d,%d",iArt_AutoTotal[0][0],iArt_AutoTotal[0][1],iArt_AutoTotal[1][0],iArt_AutoTotal[1][1],iArt_AutoTotal[2][0],iArt_AutoTotal[2][1]);                      //kevin 20170830 記錄ART Auto Tray IC數量
        //                    RespondASECom(sBuffer);//kevin 20170830 回應 ase ART Mov Tray finish
        RespondASECom(sBuffer);//kevin 20170830 回應 ase ART Mov Tray finish
    }
}
//---------------------------------------------------------------------------
int __fastcall TDataCompile::GetDataSpecial(AnsiString &SData)
{
    int lengh=0,lengh1=0,ibuffer=0;
    AnsiString buffer="";
    lengh1 = SData.Length();
    lengh = SData.Pos(",");
    if(lengh==0)
        lengh=2;
    buffer = SData.SubString(0,lengh-1);
    try
    {
        ibuffer =  buffer.ToInt();
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TDataCompile::GetDataSpecial");
        ibuffer=0;
    }
    SData= SData.SubString(lengh+1,lengh1);
    return ibuffer;
}
//---------------------------------------------------------------------------
void __fastcall TDataCompile::edtWorkTempClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, dTempMax, dTempMin);
}
//---------------------------------------------------------------------------
void __fastcall TDataCompile::edtForcePerPinClick(TObject *Sender)
{
     fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 4, true, InputLimit.dForcePerpinNHigh+10, InputLimit.dForcePerpinNLow);   //Isaac 20210517 : Pinforce加上上下限
}
//---------------------------------------------------------------------------
void __fastcall TDataCompile::edtPinCountClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 20000, 1);
}
//---------------------------------------------------------------------------

void __fastcall TDataCompile::edtSoakTimeClick(TObject *Sender)
{
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 0, 1000);
}
//---------------------------------------------------------------------------

