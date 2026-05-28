//---------------------------------------------------------------------------
#include "MachineDefine.h"
#pragma hdrstop

#include "cShowBinSet.h"

#include "cShowBinSelect.h"
#include "cprod.h"
#include "main.h"
#include "cSetUp.h"
#include "cmydef.h"
#include "ckernel.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "HTray"
#pragma resource "*.dfm"
TfShowBinSet *fShowBinSet;
TPanel      *TestSiteCH_Display[MAX_SOCKET_ROW][MAX_SOCKET_COL];    //JerryYang 20180626 (wei) : MicroChip要求initial要給OP確認site mapping
TLabel      *TestLabCol_Display[MAX_SOCKET_COL];
TLabel      *TestLabRow_Display[MAX_SOCKET_ROW];
//int SiteData_Display[TotalTestMode][2]=  //JerryYang 20190708 mark掉
//    {{1,1},     //1x1
//     {2,1},     //1x2
//     {1,1},     //1x2BS
//     {3,1},     //1x3
//     {4,1},     //1x4
//     {1,2},     //2x1
//     {2,2},     //2x2
//     {1,2},     //2x1BS
//     {3,2},     //2x3
//     {4,2},     //2x4
//     {6,2},     //2x6
//     {8,2},     //2x8
//     {8,4},     //4x8
//     {8,4},     //4x8
//     {4,1}
//    };//Eliot 2009_12_23
int iReturnValue=0;
//---------------------------------------------------------------------------
__fastcall TfShowBinSet::TfShowBinSet(TComponent* Owner)
    : TForm(Owner)
{
    TPanel *tempTestSiteCBox_Dis[MAX_SOCKET_ROW][MAX_SOCKET_COL]=   //JerryYang 20180626 (wei) : MicroChip要求initial要給OP確認site mapping
    {
        {palAa, palAb, palAc, palAd, palAe, palAf, palAg, palAh},
        {palBa, palBb, palBc, palBd, palBe, palBf, palBg, palBh},
        {palCa, palCb, palCc, palCd, palCe, palCf, palCg, palCh},
        {palDa, palDb, palDc, palDd, palDe, palDf, palDg, palDh}
    };
    TLabel *tempTestLabCol_Dis[MAX_SOCKET_COL]=
    {
        labColA, labColB, labColC, labColD, labColE, labColF, labColG, labColH
    };
    TLabel *tempTestLabRow_Dis[MAX_SOCKET_ROW]={labRowA, labRowB, labRowC, labRowD};

    int i, iCol, iRow;
    for(i=0; i<MAX_SOCKET_TOTAL; i++)
    {
        iRow=i/MAX_SOCKET_COL;
        iCol=i%MAX_SOCKET_COL;

        if(i<MAX_SOCKET_ROW)
        {
            TestLabRow_Display[i]=tempTestLabRow_Dis[i];
            TestLabRow_Display[i]->Visible=false;
        }
        if(i<MAX_SOCKET_COL)
        {
            TestLabCol_Display[i]=tempTestLabCol_Dis[i];
            TestLabCol_Display[i]->Visible=false;
        }
        TestSiteCH_Display[iRow][iCol]=tempTestSiteCBox_Dis[iRow][iCol];
        TestSiteCH_Display[iRow][iCol]->Visible=false;
    }
    fShow=false;

    TLabel *tempMyBinSel[]=
    {
        laAuto1, laAuto2, laAuto3, laAuto4, laAuto5, laAuto6,
        laFix1,  laFix2,  laFix3,  laFix4,  laFix5,  laFix6,
        laFix7,  laFix8,  laFix9,  laFix10, laFix11, laFix12, laBinBox,
        laMag1,  laMag2,  laMag3,  laMag4,  laMag5,  laMag6,  laMag7,          //JerryYang 20220909 : add magazine
        laMag8,  laMag9,  laMag10, laMag11, laMag12, laMag13, laMag14
    };

    TLabel *tempMyBinSelLab[]=                                                  //JerryYang 20220909 : add magazine
    {
        labAuto1, labAuto2, labAuto3, labAuto4, labAuto5, labAuto6,
        labFix1,  labFix2,  labFix3,  labFix4,  labFix5,  labFix6,
        labFix7,  labFix8,  labFix9,  labFix10, labFix11, labFix12, labBinBox,
        labMag1,  labMag2,  labMag3,  labMag4,  labMag5,  labMag6,  labMag7,
        labMag8,  labMag9,  labMag10, labMag11, labMag12, labMag13, labMag14
    };

    for(int i=0; i<eTrayCount; i++)
    {
        MyBinSel[i]         =tempMyBinSel[i];
        MyBinSelLab[i]      =tempMyBinSelLab[i];
    }
}
//---------------------------------------------------------------------------
void __fastcall TfShowBinSet::FormShow(TObject *Sender)
{
    for(int i=0; i<eTrayCount; i++)
    {
        MyBinSel[i]->Caption    =fShowBinSelect->MyBinSel[i]->Caption;
        MyBinSel[i]->Visible    =fShowBinSelect->MyBinSel[i]->Visible;
        MyBinSel[i]->Font->Color=fShowBinSelect->MyBinSel[i]->Font->Color;
        MyBinSelLab[i]->Caption =fShowBinSelect->MyBinSelLab[i]->Caption;
        MyBinSelLab[i]->Visible =fShowBinSelect->MyBinSelLab[i]->Visible;
    }

    if(TrayForm.iFixTrayMode)
    {
        Height=660;
    }
    else
    {
        Height=465;
    }
    fShow=true;
//JerryYang 20180626 (wei) : MicroChip要求initial要給OP確認site mapping
//==>
    int i, j, k;
    int iMode=TestIF_File.iTestMode;
    int iTestCHCT=SiteData[iMode].Cnt;

    //jou 2012-11-20 Dual Site supply 4's Channel
    if(IniConfig.bDualSiteSupply4CH==true)
    {
        if(TestIF_File.iTestMode==DualSite)
        {
            iTestCHCT+=2;
        }
    }

    //jou 2015-05-14 Octal Site supply 12 sites channel
    if(CUSTOMER_CODE==CC_ASE_KaohSiung)    //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction  //JerryYang 20230204 : SPIL中科要求改回來一樣顯示8 site
    {
        if((TestIF_File.iTestMode==_8Site2X4 || TestIF.iTestMode==_16Site4X4) && CosFunction.bEnableOctal_12Kit==true && TestIF_File.bOctal_12Kit==true)   //Sam 20190226 : 16Site4X4
        {
            iTestCHCT+=4;
        }
        else if((TestIF_File.iTestMode==_8Site2X4 || TestIF.iTestMode==_16Site4X4) && CosFunction.bEnableOctal_12Kit==true && TestIF_File.bOctal_12Kit==false)  //Sam 20190226 : 16Site4X4
        {
            iTestCHCT=SiteData[iMode].Cnt;
        }
    }

    if(CosFunction.bUse32ChanelSiteMap) //Steven 20170530 (wei) : Use 32CH site map
    {
        iTestCHCT=32;
    }

    for(i=0; i<MAX_SOCKET_ROW; i++)
    {
        TestLabRow_Display[i]->Visible=false;
        for(j=0; j<MAX_SOCKET_COL; j++)
        {
            TestSiteCH_Display[i][j]->Caption="";
            TestSiteCH_Display[i][j]->Visible=false;
            TestLabCol_Display[j]->Visible=false;
        }
    }

    for(i=0; i<SiteData[iMode].XItem; i++)
    {
        TestLabCol_Display[i]->Visible=true;
        for(j=0; j<SiteData[iMode].YItem; j++)
        {
            TestSiteCH_Display[j][i]->Visible=true;
            TestSiteCH_Display[j][i]->Enabled=true;
            for(k=0; k<=iTestCHCT; k++)
            {
                if(k==0)
                    TestSiteCH_Display[j][i]->Caption="- - -";
                else
                    TestSiteCH_Display[j][i]->Caption=("CH "+AnsiString (k));
            }
            TestLabRow_Display[j]->Visible=true;
        }
    }
    for(int i=0; i<MAX_SOCKET_ROW; i++)
    {
        for(int j=0; j<MAX_SOCKET_COL; j++)
        {
            TestSiteCH_Display[i][j]->Caption=("CH "+AnsiString (TestIF_File.iSiteMap[i][j]));
        }
    }
    bAlarmBuzzer=true;  //JerryYang 20190604 蜂鳴器要會叫
//<==
//JerryYang 20180626 (wei) : MicroChip要求initial要給OP確認site mapping
}
//---------------------------------------------------------------------------
void __fastcall TfShowBinSet::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    fShow=false;
}
//---------------------------------------------------------------------------
void __fastcall TfShowBinSet::Panel3Click(TObject *Sender)  //JerryYang 20180626 (wei) : MicroChip要求initial要給OP確認site mapping
{
    iReturnValue=1;
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfShowBinSet::Panel4Click(TObject *Sender)  //JerryYang 20180626 (wei) : MicroChip要求initial要給OP確認site mapping
{
    iReturnValue=2;
    Close();
}
//---------------------------------------------------------------------------
int ShowSiteMapping_YES_NO()    //JerryYang 20180626 (wei) : MicroChip要求initial要給OP確認site mapping
{
    iReturnValue=0;
    fShowBinSet->ShowModal();
    return iReturnValue;
}
//---------------------------------------------------------------------------
void __fastcall TfShowBinSet::Timer1Timer(TObject *Sender)
{
    if(fShow==false)    //JerryYang 20190819 add
        return;

    DoSystemMessage();        //JerryYang 20190604 蜂鳴器要會叫
}
//---------------------------------------------------------------------------
