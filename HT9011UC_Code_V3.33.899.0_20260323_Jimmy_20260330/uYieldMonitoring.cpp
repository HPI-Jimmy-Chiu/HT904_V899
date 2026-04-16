#include "MachineDefine.h"
#pragma hdrstop

#include "uYieldMonitoring.h"

#include "cprod.h"
#include "common.h"
#include "cTesterIF.h"
#include "cTemperFrom.h"
#include "cpublic.h"
#include "cmydef.h"
#include "cSecurity.h"
#include "BarcodeReader.h"
#include "myQwertyKeyBoard.h"
#include "cSortCT.h"
#include "cBuilder.h"
#include "uLotInfo.h"
#include "main.h"
#include "cContact.h"
#include "cSocket.h"
#include "note.h"
#include "cShowBinSelect.h"
#include "cContactCT.h"
#include "AutoClean.h"
#include "HS_Function.h"
#include "atester_ProcessCount.h"
#include "uHGemHT9045.h"
#include "RPDefault.h"
#include "uCleaning.h"
#include "cAuthority.h"
#include "cinitial.h"
#include "mymessbox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "HTray"
#pragma link "HTray256"
#pragma resource "*.dfm"
TfYieldMonitoring *fYieldMonitoring;

enum eBinSettingItems
    {   eBinNo           =0,         //Bin名稱的位置
        ePassYield       =1,         //設定pass bin
        eOpenShort       =2,         //設定open short bin
        eRecover         =3,
        eBinSetting      =4,         //Bin設定的起始點
        eBinSetTotal    =(eBinSetting)
    };

const int iRowHeight=24;
const int iColWidth =40;

const int eItemName   =0;           //名稱的位置
enum eTrayNameFunc                  //使用的功能
    {   eItemeEnable   =0,          //功能
        eItemeYield    =1,          //設定 yield
        eItemTotal
    };

enum eTrayColorMap
    {   eCLWhite        =0,
        eCLGreen        =1,
        eCLRed          =2,
        eCLYellow       =3,
        eCLGray         =4,
        eCLSilver       =5,
        eCLBtnFace      =6,
        eCLOlive        =7
    };

const int iBinCount=TEST_MAX_BIN;                                               //使用的Bin數量
bool bHasCloseSite;                                                             //Isaac 20171227 (Steven) : 記錄low yield auto site off log，移到外層
//---------------------------------------------------------------------------
class PACKAGE TMyYieldPanel: public TComponent
{
    public:
        __fastcall TMyYieldPanel(TComponent* Owner, int index, TTabSheet *parent, AnsiString Name);
        __fastcall ~TMyYieldPanel();
        TPanel         *PalYield;
        TTMyTray256    *mtTrayItem;
        TTMyTray256    *mtTrayName;
        TTMyTray256    *mtBinSelectYield;
        TScrollBox     *sbBinSetting;
        TLabel         *laARTLinit;
        TComboBox      *ReTestLimit;

        bool    bEnablePassYieldART;                                            // kevin 20150703 啟用pass yield功能
        double  fPassYieldART;                                                  //啟用 pass yield  94.6%
        bool    bEnableOpenShortART;                                            //啟用Open/short  yield功能
        double  fOpenShortYieldART;                                             //使用 open short yield 94.6%
        bool    bEnableRecoverART;                                              //使用 rt1 當次比對
        double  fRecoverYieldART;                                               //使用 rt1 當次比對

        bool    bPass     [iBinCount];                                          //使用各bin pass yield
        bool    bOpenShort[iBinCount];                                          //使用各bin open short

        void    __fastcall InitmtBinSelectData();
};
vector<TMyYieldPanel*> MyYieldPanel;

//---------------------------------------------------------------------------
__fastcall TMyYieldPanel::TMyYieldPanel(TComponent* Owner, int index, TTabSheet *parent, AnsiString Name) : TComponent(Owner)         //設定元件位置
{
    AnsiString str;
    PalYield                =new TPanel(this);
    mtTrayItem              =new TTMyTray256(this);
    mtTrayName              =new TTMyTray256(this);
    mtBinSelectYield        =new TTMyTray256(this);
    sbBinSetting            =new TScrollBox(this);
    ReTestLimit             =new TComboBox(this);
    laARTLinit              =new TLabel(this);

    str.sprintf("PalYield%s", Name);
    PalYield->Parent        =parent;
    PalYield->Name          =str;
    PalYield->Color         =TColor(0x00C2B8A6);
    PalYield->Align         =alClient;
    PalYield->Caption       ="";

    str.sprintf("sbBinSetting%s", Name);
    sbBinSetting->Parent      =PalYield;
    sbBinSetting->Name        =str;
    sbBinSetting->Height      =iRowHeight*eBinSetTotal;                         //20 是ScrollBar的高度
    sbBinSetting->Left        =265;
    sbBinSetting->Top         =8;
    sbBinSetting->Width       =701;
    sbBinSetting->BorderStyle =bsNone;
    sbBinSetting->Tag         =index;

    str.sprintf("mtBinSelectYield%s", Name);
    mtBinSelectYield->Parent      =sbBinSetting;
    mtBinSelectYield->Name        =str;
    mtBinSelectYield->Left        =0;

    mtBinSelectYield->Height      =iRowHeight*eBinSetTotal;
    mtBinSelectYield->XItem       =iBinCount;
    mtBinSelectYield->YItem       =eBinSetTotal;

    mtBinSelectYield->Top         =0;
    mtBinSelectYield->Width       =iColWidth*iBinCount+10;                      //10是為了留邊
    mtBinSelectYield->SetColorMap(0, clWhite);
    mtBinSelectYield->SetColorMap(1, clGreen);
    mtBinSelectYield->SetColorMap(2, clRed);
    mtBinSelectYield->SetColorMap(3, clYellow);
    mtBinSelectYield->SetColorMap(4, clGray);
    mtBinSelectYield->SetColorMap(5, clSilver);
    mtBinSelectYield->SetColorMap(6, clBtnFace);
    mtBinSelectYield->SetColorMap(7, clOlive);
    mtBinSelectYield->Color       =clWhite;
    mtBinSelectYield->Font->Name  ="Arial";
    mtBinSelectYield->Font->Size  =10;
    mtBinSelectYield->Font->Color =clWhite;
    mtBinSelectYield->DirectWidth =0;
    mtBinSelectYield->EdgeWidth   =2;
    mtBinSelectYield->LineWidth   =-1;
    mtBinSelectYield->Shape       =0;
    mtBinSelectYield->ShowFont    =true;
    mtBinSelectYield->Tag         =index;
    mtBinSelectYield->OnMouseDown =fYieldMonitoring->mtBinSelectYieldMouseDown;

    str.sprintf("mtTrayItem%s", Name);
    mtTrayItem->Parent      =PalYield;
    mtTrayItem->Name        =str;
    mtTrayItem->YItem       =eBinSetTotal;
    mtTrayItem->XItem       =eItemName;
    mtTrayItem->Height      =iRowHeight*eBinSetTotal;
    mtTrayItem->Left        =8;
    mtTrayItem->Top         =8;
    mtTrayItem->Width       =117;
    mtTrayItem->SetColorMap(0, clWhite);
    mtTrayItem->SetColorMap(1, clGreen);
    mtTrayItem->SetColorMap(2, clRed);
    mtTrayItem->SetColorMap(3, clYellow);
    mtTrayItem->SetColorMap(4, clGray);
    mtTrayItem->SetColorMap(5, clSilver);
    mtTrayItem->SetColorMap(6, clBtnFace);
    mtTrayItem->SetColorMap(7, clOlive);
    mtTrayItem->Color       =clWhite;
    mtTrayItem->Font->Name  ="Arial";
    mtTrayItem->Font->Size  =10;
    mtTrayItem->Font->Color =clWhite;
    mtTrayItem->DirectWidth =0;
    mtTrayItem->EdgeWidth   =5;
    mtTrayItem->LineWidth   =-1;
    mtTrayItem->Shape       =0;
    mtTrayItem->ShowFont    =true;
    mtTrayItem->Tag         =index;

    str.sprintf("mtTrayName%s", Name);
    mtTrayName->Parent      =PalYield;
    mtTrayName->Name        =str;
    mtTrayName->YItem       =eBinSetTotal;
    mtTrayName->XItem       =eItemTotal;
    mtTrayName->Height      =iRowHeight*eBinSetTotal;
    mtTrayName->Left        =125;
    mtTrayName->Top         =8;
    mtTrayName->Width       =140;
    mtTrayName->SetColorMap(0, clWhite);
    mtTrayName->SetColorMap(1, clGreen);
    mtTrayName->SetColorMap(2, clRed);
    mtTrayName->SetColorMap(3, clYellow);
    mtTrayName->SetColorMap(4, clGray);
    mtTrayName->SetColorMap(5, clSilver);
    mtTrayName->SetColorMap(6, clBtnFace);
    mtTrayName->SetColorMap(7, clOlive);
    mtTrayName->Color       =clWhite;
    mtTrayName->Font->Name  ="Arial";
    mtTrayName->Font->Size  =10;
    mtTrayName->Font->Color =clBlack;
    mtTrayName->DirectWidth =0;
    mtTrayName->EdgeWidth   =5;
    mtTrayName->LineWidth   =-1;
    mtTrayName->Shape       =0;
    mtTrayName->ShowFont    =true;
    mtTrayName->Tag         =index;
    mtTrayName->OnMouseDown =fYieldMonitoring->mtTrayNameMouseDown;

    //Col Name----
    mtTrayName->SetCellNumber(eItemeEnable, eItemeEnable, "ENABLE");
    mtTrayName->SetCellNumber(eItemeYield,  eItemeEnable, "Yield");

    //Row Name----
    mtTrayItem->SetCellNumber(eItemName,  eBinNo,         "Items");             //Bin名稱的位置
    mtTrayItem->SetCellNumber(eItemName,  ePassYield,     "PassYield");         //設定pass bin
    mtTrayItem->SetCellNumber(eItemName,  eOpenShort,     "OpenShort");         //設定open short bin
    mtTrayItem->SetCellNumber(eItemName,  eRecover,       "Recover");           //設定recover

    str.sprintf("laARTLinit%s", Name.c_str());
    laARTLinit->Parent      =PalYield;
    laARTLinit->Name        =str;
    laARTLinit->Left        =8;
    laARTLinit->Top         =mtTrayName->Height +15;
    laARTLinit->Width       =117;
    laARTLinit->Font->Color =clBlack;
    laARTLinit->Font->Name  ="Arial";
    laARTLinit->Font->Size  =20;
    laARTLinit->Tag         =index;
    laARTLinit->Caption     ="Auto Retest limit";
    laARTLinit->Visible     =true;

    str.sprintf("ReTestLimit%s", Name.c_str());
    ReTestLimit->Parent      =PalYield;
    ReTestLimit->Name        =str;
    ReTestLimit->Left        =laARTLinit->Width+30;
    ReTestLimit->Top         =mtTrayName->Height+15;
    ReTestLimit->Width       =100;
    ReTestLimit->Font->Color =clBlack;
    ReTestLimit->Font->Name  ="Arial";
    ReTestLimit->Font->Size  =10;
    ReTestLimit->Tag         =index;
    ReTestLimit->Text        ="1";
    ReTestLimit->Visible     =true;
    ReTestLimit->Items->Add("1");
    ReTestLimit->Items->Add("2");
    ReTestLimit->Items->Add("3");
    ReTestLimit->Items->Add("4");
    ReTestLimit->Items->Add("5");
    ReTestLimit->Items->Add("6");
    ReTestLimit->Items->Add("7");
    ReTestLimit->Items->Add("8");
    ReTestLimit->Items->Add("9");
    ReTestLimit->Items->Add("10");

    for(int j=0; j<eBinSetTotal; j++)                                           //Row eScanning
    {
        mtTrayItem->SetCellColorIndex(eItemName, j, eCLGray);
    }
    InitmtBinSelectData();
}
//---------------------------------------------------------------------------
void __fastcall TMyYieldPanel::InitmtBinSelectData()
{
    for(int j=0; j<iTestBinCount; j++)
    {
        if(j<mtBinSelectYield->XItem)
        {
            mtBinSelectYield->SetCellNumber    (j, eBinNo, j);                  //kevin 20140317 256 bin 0
            mtBinSelectYield->SetCellColorIndex(j, eBinNo, eCLGray);
        }
    }
}
//---------------------------------------------------------------------------
__fastcall TMyYieldPanel::~TMyYieldPanel()
{
}
//---------------------------------------------------------------------------
__fastcall TfYieldMonitoring::TfYieldMonitoring(TComponent* Owner)
    : TForm(Owner)
{
    bFirstCount=true;

    iMinYield=0;
    iMaxYield=100;
    dMinYield=0.0;
    dMaxYield=100.0;
    iMinCount=1;
    iMaxCount=100000;

//    if(CUSTOMER_CODE==CC_SIGURD_HUKOU)                                          //Sam 20211120 : 卓劭要求能設定為最低 0
//    {
//        iMinYield=0;
//        dMinYield=0.0;
//    }

//    if(CUSTOMER_CODE==CC_KYEC_XILINX)                                           //wei 20151221 XILINX LowYieldCount 最小值為1
//    {
//        iMinCount=1;
//    }

    cbLowYield_FT->Hint                         ="Yield_Normal Low Yield Enable";
    edLowYield_FT->Hint                         ="Yield_Normal Low Yield";      //Sam 20201209 : Default Recipe ChangeLog
    edLowYieldIg_FT->Hint                       ="Yield_Normal Low Yield Count";

    edContsFailSocketAlarmCT_FT->Hint           ="Yield_Normal Consecutive Failure Alarm By Socket";
    edContsFailHeadAlarmCT_FT->Hint             ="Yield_Normal Consecutive Failure Alarm By Head";

    cbAlarm4ContinueType->Hint                  ="Yield_Alarm4 Site To Site Continue Type Select";
    edAlarm4IntervalCount->Hint                 ="Yield_Alarm4 Site To Site Interval Count";
    edAlarm4ContinueCount->Hint                 ="Yield_Alarm4 Site To Site Continue Count";

    cb_Alarm4EnableIntervalYield->Hint          ="Yield_Alarm4 Interval Total Yield Different Select";
    ed_Alarm4IntervalYieldYield->Hint           ="Yield_Alarm4 Interval Total Yield Different";
    ed_Alarm4IntervalYieldIntervalCount->Hint   ="Yield_Alarm4 Interval Total Yield Different Interval Count";
    ed_Alarm4IntervalYieldContinueCount->Hint   ="Yield_Alarm4 Interval Total Yield Different Continue Count";

    ed_SiteToSiteYield->Hint                    ="Yield_Alarm4 Site To Site Yield";
    ed_HeadToHeadYield->Hint                    ="Yield_Alarm4 Head To Head Yield";

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //kevin 20150702 ART 設定
    {
        MyYieldPanel.push_back(new TMyYieldPanel(this, 0, tsAutoRetest, "ART"));
    }
    bGetGPIBAutoSiteOff=false;
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::FormCreate(TObject *Sender)
{
    AnsiString str, StrC;
    int iRowHeight=10;

    for(int i=0; i<TEST_MAX_BIN; i++)
    {
        iRowHeight=30*i+10;
        str.sprintf("Category %d", i);
        StrC.sprintf("% *s%%", -37, str);

        str.sprintf("cbByBinSiteGapCat_FT%03d", i);
        cbByBinSiteGapCat_FT[i]             =new TCheckBox(this);
        cbByBinSiteGapCat_FT[i]->Parent     =scrlbxBinAlarm2_FT;
        cbByBinSiteGapCat_FT[i]->Name       =str;
        cbByBinSiteGapCat_FT[i]->Height     =16;
        cbByBinSiteGapCat_FT[i]->Left       =8;
        cbByBinSiteGapCat_FT[i]->Top        =iRowHeight;
        cbByBinSiteGapCat_FT[i]->Width      =228;
        cbByBinSiteGapCat_FT[i]->OnMouseUp  =cbLowYield_FTMouseUp;
        cbByBinSiteGapCat_FT[i]->Caption    =StrC;
        cbByBinSiteGapCat_FT[i]->Font->Name ="MS Sans Serif";
        cbByBinSiteGapCat_FT[i]->Font->Size =12;

        str.sprintf("cbByBinSiteGapCat_RT%03d", i);
        cbByBinSiteGapCat_RT[i]             =new TCheckBox(this);
        cbByBinSiteGapCat_RT[i]->Parent     =scrlbxBinAlarm2_RT;
        cbByBinSiteGapCat_RT[i]->Name       =str;
        cbByBinSiteGapCat_RT[i]->Height     =16;
        cbByBinSiteGapCat_RT[i]->Left       =8;
        cbByBinSiteGapCat_RT[i]->Top        =iRowHeight;
        cbByBinSiteGapCat_RT[i]->Width      =228;
        cbByBinSiteGapCat_RT[i]->OnMouseUp  =cbLowYield_FTMouseUp;
        cbByBinSiteGapCat_RT[i]->Caption    =StrC;
        cbByBinSiteGapCat_RT[i]->Font->Name ="MS Sans Serif";
        cbByBinSiteGapCat_RT[i]->Font->Size =12;

        str.sprintf("edByBinSiteGapCat_FT%03d", i);
        edByBinSiteGapCat_FT[i]             =new TEdit(this);
        edByBinSiteGapCat_FT[i]->Parent     =scrlbxBinAlarm2_FT;
        edByBinSiteGapCat_FT[i]->Name       =str;
        edByBinSiteGapCat_FT[i]->Height     =28;
        edByBinSiteGapCat_FT[i]->Left       =135;
        edByBinSiteGapCat_FT[i]->Top        =iRowHeight-6;
        edByBinSiteGapCat_FT[i]->Width      =60;
        edByBinSiteGapCat_FT[i]->OnMouseDown=edLowYield_RTMouseDown;
        edByBinSiteGapCat_FT[i]->OnChange   =edContactCountFTChange;

        str.sprintf("edByBinSiteGapCat_RT%03d", i);
        edByBinSiteGapCat_RT[i]             =new TEdit(this);
        edByBinSiteGapCat_RT[i]->Parent     =scrlbxBinAlarm2_RT;
        edByBinSiteGapCat_RT[i]->Name       =str;
        edByBinSiteGapCat_RT[i]->Height     =28;
        edByBinSiteGapCat_RT[i]->Left       =135;
        edByBinSiteGapCat_RT[i]->Top        =iRowHeight-6;
        edByBinSiteGapCat_RT[i]->Width      =60;
        edByBinSiteGapCat_RT[i]->OnMouseDown=edLowYield_RTMouseDown;
        edByBinSiteGapCat_RT[i]->OnChange   =edContactCountFTChange;

        str.sprintf("cbByArmSiteGapCat_FT%03d", i);
        cbByArmSiteGapCat_FT[i]             =new TCheckBox(this);
        cbByArmSiteGapCat_FT[i]->Parent     =scrlbxBinAlarm3_FT;
        cbByArmSiteGapCat_FT[i]->Name       =str;
        cbByArmSiteGapCat_FT[i]->Height     =16;
        cbByArmSiteGapCat_FT[i]->Left       =8;
        cbByArmSiteGapCat_FT[i]->Top        =iRowHeight;
        cbByArmSiteGapCat_FT[i]->Width      =228;
        cbByArmSiteGapCat_FT[i]->OnMouseUp  =cbLowYield_FTMouseUp;
        cbByArmSiteGapCat_FT[i]->Caption    =StrC;
        cbByArmSiteGapCat_FT[i]->Font->Name ="MS Sans Serif";
        cbByArmSiteGapCat_FT[i]->Font->Size =12;

        str.sprintf("cbByArmSiteGapCat_RT%03d", i);
        cbByArmSiteGapCat_RT[i]             =new TCheckBox(this);
        cbByArmSiteGapCat_RT[i]->Parent     =scrlbxBinAlarm3_RT;
        cbByArmSiteGapCat_RT[i]->Name       =str;
        cbByArmSiteGapCat_RT[i]->Height     =16;
        cbByArmSiteGapCat_RT[i]->Left       =8;
        cbByArmSiteGapCat_RT[i]->Top        =iRowHeight;
        cbByArmSiteGapCat_RT[i]->Width      =228;
        cbByArmSiteGapCat_RT[i]->OnMouseUp  =cbLowYield_FTMouseUp;
        cbByArmSiteGapCat_RT[i]->Caption    =StrC;
        cbByArmSiteGapCat_RT[i]->Font->Name ="MS Sans Serif";
        cbByArmSiteGapCat_RT[i]->Font->Size =12;

        str.sprintf("edByArmSiteGapCat_FT%03d", i);
        edByArmSiteGapCat_FT[i]             =new TEdit(this);
        edByArmSiteGapCat_FT[i]->Parent     =scrlbxBinAlarm3_FT;
        edByArmSiteGapCat_FT[i]->Name       =str;
        edByArmSiteGapCat_FT[i]->Height     =28;
        edByArmSiteGapCat_FT[i]->Left       =135;
        edByArmSiteGapCat_FT[i]->Top        =iRowHeight-6;
        edByArmSiteGapCat_FT[i]->Width      =60;
        edByArmSiteGapCat_FT[i]->OnMouseDown=edLowYield_RTMouseDown;
        edByArmSiteGapCat_FT[i]->OnChange   =edContactCountFTChange;

        str.sprintf("edByArmSiteGapCat_RT%03d", i);
        edByArmSiteGapCat_RT[i]             =new TEdit(this);
        edByArmSiteGapCat_RT[i]->Parent     =scrlbxBinAlarm3_RT;
        edByArmSiteGapCat_RT[i]->Name       =str;
        edByArmSiteGapCat_RT[i]->Height     =28;
        edByArmSiteGapCat_RT[i]->Left       =135;
        edByArmSiteGapCat_RT[i]->Top        =iRowHeight-6;
        edByArmSiteGapCat_RT[i]->Width      =60;
        edByArmSiteGapCat_RT[i]->OnMouseDown=edLowYield_RTMouseDown;
        edByArmSiteGapCat_RT[i]->OnChange   =edContactCountFTChange;

        str.sprintf("cbByBinFailureCat_FT%03d", i);
        cbByBinFailureCat_FT[i]             =new TCheckBox(this);
        cbByBinFailureCat_FT[i]->Parent     =scrlbxBinAlarm1_FT;
        cbByBinFailureCat_FT[i]->Name       =str;
        cbByBinFailureCat_FT[i]->Height     =16;
        cbByBinFailureCat_FT[i]->Left       =8;
        cbByBinFailureCat_FT[i]->Top        =iRowHeight;
        cbByBinFailureCat_FT[i]->Width      =228;
        cbByBinFailureCat_FT[i]->OnMouseUp  =cbLowYield_FTMouseUp;
        cbByBinFailureCat_FT[i]->Caption    =StrC;
        cbByBinFailureCat_FT[i]->Font->Name ="MS Sans Serif";
        cbByBinFailureCat_FT[i]->Font->Size =12;

        str.sprintf("cbByBinFailureCat_RT%03d", i);
        cbByBinFailureCat_RT[i]             =new TCheckBox(this);
        cbByBinFailureCat_RT[i]->Parent     =scrlbxBinAlarm1_RT;
        cbByBinFailureCat_RT[i]->Name       =str;
        cbByBinFailureCat_RT[i]->Height     =16;
        cbByBinFailureCat_RT[i]->Left       =8;
        cbByBinFailureCat_RT[i]->Top        =iRowHeight;
        cbByBinFailureCat_RT[i]->Width      =228;
        cbByBinFailureCat_RT[i]->OnMouseUp  =cbLowYield_FTMouseUp;
        cbByBinFailureCat_RT[i]->Caption    =StrC;
        cbByBinFailureCat_RT[i]->Font->Name ="MS Sans Serif";
        cbByBinFailureCat_RT[i]->Font->Size =12;

        str.sprintf("edByBinFailureCat_FT%03d", i);
        edByBinFailureCat_FT[i]             =new TEdit(this);
        edByBinFailureCat_FT[i]->Parent     =scrlbxBinAlarm1_FT;
        edByBinFailureCat_FT[i]->Name       =str;
        edByBinFailureCat_FT[i]->Height     =28;
        edByBinFailureCat_FT[i]->Left       =135;
        edByBinFailureCat_FT[i]->Top        =iRowHeight-6;
        edByBinFailureCat_FT[i]->Width      =60;
        edByBinFailureCat_FT[i]->OnMouseDown=edLowYield_RTMouseDown;
        edByBinFailureCat_FT[i]->OnChange   =edContactCountFTChange;

        str.sprintf("edByBinFailureCat_RT%03d", i);
        edByBinFailureCat_RT[i]             =new TEdit(this);
        edByBinFailureCat_RT[i]->Parent     =scrlbxBinAlarm1_RT;
        edByBinFailureCat_RT[i]->Name       =str;
        edByBinFailureCat_RT[i]->Height     =28;
        edByBinFailureCat_RT[i]->Left       =135;
        edByBinFailureCat_RT[i]->Top        =iRowHeight-6;
        edByBinFailureCat_RT[i]->Width      =60;
        edByBinFailureCat_RT[i]->OnMouseDown=edLowYield_RTMouseDown;
        edByBinFailureCat_RT[i]->OnChange   =edContactCountFTChange;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::FormDestroy(TObject *Sender)
{
    try
    {
        MySleep(100);
        for(vector<TMyYieldPanel *>::iterator iter=MyYieldPanel.begin(); iter!=MyYieldPanel.end(); ++iter)
        {
            delete *iter;
        }
        vec_clr(MyYieldPanel);

        for(int i=0; i<TEST_MAX_BIN; i++)
        {
            delete cbByBinSiteGapCat_FT[i];
            delete cbByBinSiteGapCat_RT[i];
            delete edByBinSiteGapCat_FT[i];
            delete edByBinSiteGapCat_RT[i];
            delete cbByArmSiteGapCat_FT[i];
            delete cbByArmSiteGapCat_RT[i];
            delete edByArmSiteGapCat_FT[i];
            delete edByArmSiteGapCat_RT[i];
            delete cbByBinFailureCat_FT[i];
            delete cbByBinFailureCat_RT[i];
            delete edByBinFailureCat_FT[i];
            delete edByBinFailureCat_RT[i];
        }
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfYieldMonitoring::FormDestroy");
    }
    LogSoftwareOffTime("TfYieldMonitoring, FormDestroy");                       //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
void TfYieldMonitoring::DoIniDataToForm()
{
//Auto Site Off Function
    chkAutoSiteOff->Checked                 =TestIF_File.bLowYieldAutoSiteOff;              //Steven 20170905 (wei) : Low Yield Auto Site Off for Ambient
    chkAutoSiteOffByContiFail->Checked      =TestIF_File.bLowYieldAutoSiteOffByContiFail;   //Steven 20200420 : Continue fail, auto site off
    chkAutoSiteOffByArmBySite->Checked      =TestIF_File.bLowYieldAutoSiteOffByArmSite;     //Steven 20230223 : by arm by site, auto site off
    chkchkAutoSiteOffByPicker->Checked      =TestIF_File.bLowYieldAutoSiteOffByPicker;      //Steven 20230223 : 根據Index吸嘴比較良率
    chkAutoSiteOffByArmContiFail->Checked   =TestIF_File.bLowYieldAutoSiteOffArmContiFail;  //Steven 20220818 : By Arm Continue fail, auto site off
    edtSiteYieldCmp1SiteCnt_FT->Text        =TestIF_File.iAlarmWhenSiteOnCountLess;
    rgCloseSiteOnHPAction->ItemIndex        =TestIF_File.iCloseSiteOnHPDontTest;            //JerryYang 20180726 (wei) 關site的位置有IC不測試送error bin
    cbbClosedSiteBin->ItemIndex             =TestIF_File.iCloseSiteBin;                     //Steven 20240409 : 關site的位置有IC不測試送指定 bin
    chkAutoSiteOffAlarm->Checked            =TestIF_File.bLowYieldAutoSiteOffAlarm;         //Sam 20221207 : LowYieldAutoSiteOff 新增 Alarm 幾次後再來關 Site
    edtAutoSiteOffAlarm->Text               =TestIF_File.iLowYieldAutoSiteOffAlarm;         //Sam 20221207 : LowYieldAutoSiteOff 新增 Alarm 幾次後再來關 Site

//Low Yield Alarm(%) [Socket Yield compare with Total Yield]
    cbLowYield_FT->Checked          =TestIF_File.bFailAlarmLowYield;
    edLowYield_FT->Text             =TestIF_File.dLowYieldLimit;                //JerryYang 20160530 LowYieldLimit要能設定到小數點
    edLowYieldIg_FT->Text           =TestIF_File.iLowYieldCount;
    cbYieldAlarmByBin->Checked      =TestIF_File.bLowYieldAlarmByBin;           //Steven 20140828 : By Bin Yield Monitor

    cbLowYield_RT->Checked          =TestIF_File.bFailAlarmLowYield_RT;
    edLowYield_RT->Text             =TestIF_File.dLowYieldLimit_RT;             //JerryYang 20160530 LowYieldLimit要能設定到小數點
    edLowYieldIg_RT->Text           =TestIF_File.iLowYieldCount_RT;

//Site Yield Alarm(%)   [同一個Socket, 不同Arm比較]
    cbSiteYieldDifferent_FT->Checked=TestIF_File.bFailAlarmSiteYieldDifferent;
    edSiteYieldDifferent_FT->Text   =TestIF_File.dFailAlarmSiteYield;           //JerryYang 20160530 LowYieldLimit要能設定到小數點
    edSiteYieldDifferentIg_FT->Text =TestIF_File.iFailAlarmSiteYieldDifferentCount;

    cbSiteYieldDifferent_RT->Checked=TestIF_File.bFailAlarmSiteYieldDifferent_RT;
    edSiteYieldDifferent_RT->Text   =TestIF_File.dFailAlarmSiteYield_RT;        //JerryYang 20160530 LowYieldLimit要能設定到小數點
    edSiteYieldDifferentIg_RT->Text =TestIF_File.iFailAlarmSiteYieldDifferentCount_RT;

//Site Yield Alarm(%)   [Socket Yield互比]
    cbSiteYieldCmp_FT->Checked      =TestIF_File.bFailAlarmSiteYieldCmp;
    edSiteYieldCmp_FT->Text         =TestIF_File.dFailAlarmSiteYieldCmp;        //JerryYang 20160530 LowYieldLimit要能設定到小數點
    edSiteYieldCmpIg_FT->Text       =TestIF_File.iFailAlarmSiteYieldCmpCount;

    cbSiteYieldCmp_RT->Checked      =TestIF_File.bFailAlarmSiteYieldCmp_RT;
    edSiteYieldCmp_RT->Text         =TestIF_File.dFailAlarmSiteYieldCmp_RT;     //JerryYang 20160530 LowYieldLimit要能設定到小數點
    edSiteYieldCmpIg_RT->Text       =TestIF_File.iFailAlarmSiteYieldCmpCount_RT;

//Low Yield Alarm(By Total)(%)  [總良率]                                        //wei 20151116 Low Yield By Total
    cbLowYieldByTotal_FT->Checked   =TestIF_File.bFailAlarmLowYieldByTotal;
    edLowYieldByTotal_FT->Text      =TestIF_File.dLowYieldLimitByTotal;         //JerryYang 20160530 LowYieldLimit要能設定到小數點
    edLowYieldByTotalIg_FT->Text    =TestIF_File.iLowYieldCountByTotal;

    cbLowYieldByTotal_RT->Checked   =TestIF_File.bFailAlarmLowYieldByTotal_RT;
    edLowYieldByTotal_RT->Text      =TestIF_File.dLowYieldLimitByTotal_RT;      //JerryYang 20160530 LowYieldLimit要能設定到小數點
    edLowYieldByTotalIg_RT->Text    =TestIF_File.iLowYieldCountByTotal_RT;

//By Picker Compare Yield   [每個吸嘴的良率互比]                                //Steven 20230223 : 根據Index吸嘴比較良率
    chkLowYieldByPicker_FT->Checked =TestIF_File.bLowYieldByPicker;
    edtLowYieldByPicker_FT->Text    =TestIF_File.dLowYieldByPicker;
    edtLowYieldByPickerIg_FT->Text  =TestIF_File.iLowYieldCountByPicker;

    chkLowYieldByPicker_RT->Checked =TestIF_File.bLowYieldByPicker_RT;
    edtLowYieldByPicker_RT->Text    =TestIF_File.dLowYieldByPicker_RT;
    edtLowYieldByPickerIg_RT->Text  =TestIF_File.iLowYieldCountByPicker_RT;

//Alarm Action
    rgPiggyBack_FT->ItemIndex       =TestIF_File.iCountAlarmAction;
    rgPiggyBack_RT->ItemIndex       =TestIF_File.iCountAlarmAction_RT;

//Continuous Pass
    cbContinuousPass_FT->Checked    =TestIF_File.bContinuousPass;
    cobContinuousPass_FT->ItemIndex =TestIF_File.iContinuousPassBin-1;
    edContinuousPass_FT->Text       =TestIF_File.iContinuousPassBinCount;

    cbContinuousPass_RT->Checked    =TestIF_File.bContinuousPass_RT;
    cobContinuousPass_RT->ItemIndex =TestIF_File.iContinuousPassBin_RT-1;
    edContinuousPass_RT->Text       =TestIF_File.iContinuousPassBinCount_RT;

//Consecutive Failure Ignore
    cbContsFailIgnore_FT->Checked   =TestIF_File.bContsFailIgnore;              //wei 20160115 銅鑼前幾顆不計算ContsFail
    edContsFailIgnore_FT->Text      =TestIF_File.iContsFailIgnore;              //wei 20160115 銅鑼前幾顆不計算ContsFail

    cbContsFailIgnore_RT->Checked   =TestIF_File.bContsFailIgnore_RT;           //wei 20160115 銅鑼前幾顆不計算ContsFail
    edContsFailIgnore_RT->Text      =TestIF_File.iContsFailIgnore_RT;           //wei 20160115 銅鑼前幾顆不計算ContsFail

    if(IniConfig.bD56YieldPiggyBackEnable)                                      //kevin 20131009 強致 Enable Yield 裡面piggyback 功能
    {
        cbContinuPassSkt_FT->Enabled=false;
        cbContactCountFT->Enabled   =false;
        cbContinuPassSkt_RT->Enabled=false;
        cbContactCountRT->Enabled   =false;

        cbContinuPassSkt_FT->Checked=true;
        edContinuPassSkt_FT->Text   =TestIF_File.iContinuousPassBinCountBySocket;

        cbContinuPassSkt_RT->Checked=true;
        edContinuPassSkt_RT->Text   =TestIF_File.iContinuousPassBinCountBySocket_RT;

        //Continuous Loader
        cbContinuousLoad_FT->Checked=false;
        edContinuousLoad_FT->Text   =TestIF_File.iContinuousLoaderCount;

        cbContinuousLoad_RT->Checked=false;
        edContinuousLoad_RT->Text   =TestIF_File.iContinuousLoaderCount_RT;

        //Continuous Contact Count
        cbContactCountFT->Checked   =true;
        edContactCountFT->Text      =TestIF_File.iContinuousContactCount;

        cbContactCountRT->Checked   =true;
        edContactCountRT->Text      =TestIF_File.iContinuousContactCount_RT;
    }
    else
    {
        cbContinuPassSkt_FT->Enabled=true;
        cbContactCountFT->Enabled   =true;
        cbContinuPassSkt_RT->Enabled=true;
        cbContactCountRT->Enabled   =true;

        cbContinuPassSkt_FT->Checked=TestIF_File.bContinuousPassBySocket;
        edContinuPassSkt_FT->Text   =TestIF_File.iContinuousPassBinCountBySocket;

        cbContinuPassSkt_RT->Checked=TestIF_File.bContinuousPassBySocket_RT;
        edContinuPassSkt_RT->Text   =TestIF_File.iContinuousPassBinCountBySocket_RT;

        //Continuous Loader
        cbContinuousLoad_FT->Checked=TestIF_File.bContinuousLoader;
        edContinuousLoad_FT->Text   =TestIF_File.iContinuousLoaderCount;

        cbContinuousLoad_RT->Checked=TestIF_File.bContinuousLoader_RT;
        edContinuousLoad_RT->Text   =TestIF_File.iContinuousLoaderCount_RT;

        //Continuous Contact Count
        cbContactCountFT->Checked   =TestIF_File.bContinuousContact;
        edContactCountFT->Text      =TestIF_File.iContinuousContactCount;

        cbContactCountRT->Checked   =TestIF_File.bContinuousContact_RT;
        edContactCountRT->Text      =TestIF_File.iContinuousContactCount_RT;
    }
    if(CUSTOMER_CODE==CC_PTI)
    {
        fLotInfo->RefreshOtherTool();
    }

//Consecutive Failure Alarm ( Socket )
    rbContsFailBySocket_FTOn ->Enabled=true;
    rbContsFailBySocket_FTOff->Enabled=true;
    if(IniConfig.bFTContinueON)                                                 //kevin 20121008 FT MODE 不能關連續FAIL
    {
        rbContsFailBySocket_FTOn ->Checked=true;
        rbContsFailBySocket_FTOff->Checked=false;
        if(TestIF_File.iContsFailSocketAlarmCT>(unsigned int)IniConfig.iFTMAXValue)
           TestIF_File.iContsFailSocketAlarmCT=(unsigned int)IniConfig.iFTMAXValue;

       rbContsFailBySocket_FTOn ->Enabled=false;
       rbContsFailBySocket_FTOff->Enabled=false;
    }
    else
    {
        rbContsFailBySocket_FTOn ->Checked=TestIF_File.bContsFailBySocket;
        rbContsFailBySocket_FTOff->Checked=!TestIF_File.bContsFailBySocket;
    }
    edContsFailSocketAlarmCT_FT->Text=TestIF_File.iContsFailSocketAlarmCT;
    edContsFailSocketAlarmCT_RT->Text=TestIF_File.iContsFailSocketAlarmCT_RT;
    chkSpcBinCntAsContinue_FT->Checked=TestIF_File.bCountSpcBinContinuously_FT; //Steven 20230529 : Spc Bin Couont改成連續錯誤
    chkSpcBinCntAsContinue_RT->Checked=TestIF_File.bCountSpcBinContinuously_RT; //Steven 20230529 : Spc Bin Couont改成連續錯誤

    rbContsFailBySocket_RTOn ->Checked=TestIF_File.bContsFailBySocket_RT;
    rbContsFailBySocket_RTOff->Checked=!TestIF_File.bContsFailBySocket_RT;

//Consecutive Failure Alarm ( Head)
    //Consecutive Failure Alarm ( Socket )
    rbContsFailByHead_FTOn ->Enabled=true;
    rbContsFailByHead_FTOff->Enabled=true;
    if(IniConfig.bFTContinueON)                                                 //kevin 20121008 FT MODE 不能關連續FAIL
    {
        rbContsFailByHead_FTOn ->Checked=true;
        rbContsFailByHead_FTOff->Checked=false;
        if(TestIF_File.iContsFailHeadAlarmCT>(unsigned int)IniConfig.iFTMAXValue)
            TestIF_File.iContsFailHeadAlarmCT=(unsigned int)IniConfig.iFTMAXValue;

        rbContsFailByHead_FTOn ->Enabled=false;
        rbContsFailByHead_FTOff->Enabled=false;
    }
    else
    {
        rbContsFailByHead_FTOn ->Checked=TestIF_File.bContsFailByHead;
        rbContsFailByHead_FTOff->Checked=!TestIF_File.bContsFailByHead;
    }
    rbContsFailByHead_RTOn ->Checked    =TestIF_File.bContsFailByHead_RT;
    rbContsFailByHead_RTOff->Checked    =!TestIF_File.bContsFailByHead_RT;
    edContsFailHeadAlarmCT_RT->Text     =TestIF_File.iContsFailHeadAlarmCT_RT;
    edContsFailHeadAlarmCT_FT->Text     =TestIF_File.iContsFailHeadAlarmCT;

    //wei 20141201 Low Yield Auto Clean(%) start
    rgAutoCleanYieldOnOff->ItemIndex    =TestIF_File.bFailAlarmLowYield_AutoClean;
    edAutoCleanLowYieldLimit->Text      =TestIF_File.iLowYieldLimit_AutoClean;
    edAutoCleanLowYieldCount->Text      =TestIF_File.iLowYieldCount_AutoClean;
    eCloseSite->Text                    =IntToStr(TestIF_File.iLowYieldLimit_AutoClean*2);//kevin 20150306
    //wei 20141201 Low Yield Auto Clean(%) end

    rgAutoSiteOffWhenRT->ItemIndex      =TestIF_File.iAutoCloseSiteWhenRT;                          //Steven 20200225 : 切到RT的時候,要關閉Socket
    edtAutoSiteOffWhenRT->Text          =TestIF_File.dAutoCloseSiteYieldWhenRT;                     //Steven 20200225 : 切到RT的時候,要關閉Socket
    rgAutoSiteOn->ItemIndex             =TestIF_File.iAllSiteOnAtInitialStart;                      //Steven 20230814 : Initial Start的時候要全開Site
    rgAutoSiteOnByGPIB->ItemIndex       =TestIF_File.iAutoSiteOffByGPIB;                            //JimmyChiu 20250715 : Auto site on/off by GPIB
    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //kevin 20150529 AutoRetest
    {
        MyYieldPanel[0]->ReTestLimit->ItemIndex=iAutoRetestLimit-1;             //kevin 20150704
        InitDataToEdit(0);
        MyYieldPanel[0]->InitmtBinSelectData();
    }

    //wei 20160314
    cbAlarm4ContinueType->Checked               =TestIF_File.bAlarm4ContinueType_Enable;
    edAlarm4IntervalCount->Text                 =TestIF_File.iAlarm4ContinueType_IntervalCount;
    edAlarm4ContinueCount->Text                 =TestIF_File.iAlarm4ContinueType_ContinueCount;

    cb_Alarm4EnableIntervalYield->Checked       =TestIF_File.bAlarm4EnableIntervalYield;
    ed_Alarm4IntervalYieldIntervalCount->Text   =TestIF_File.iAlarm4IntervalYieldIntervalCount;
    ed_Alarm4IntervalYieldContinueCount->Text   =TestIF_File.iAlarm4IntervalYieldContinueCount;
    ed_Alarm4IntervalYieldYield->Text           =TestIF_File.iAlarm4IntervalYieldYield;

    cb_SiteToSiteYieldEnable->Checked           =TestIF_File.bSiteToSiteYieldCmp;
    ed_SiteToSiteYield->Text                    =TestIF_File.iSiteToSiteYieldCmp;
    ed_SiteToSiteYieldCount->Text               =TestIF_File.iSiteToSiteYieldCmpCount;

    cb_HeadToHeadYieldEnable->Checked           =TestIF_File.bHeadToHeadYieldCmp;
    ed_HeadToHeadYield->Text                    =TestIF_File.iHeadToHeadYieldCmp;
    ed_HeadToHeadYieldCount->Text               =TestIF_File.iHeadToHeadYieldCmpCount;

    cb_SiteYieldOverAlert->Checked              =TestIF_File.bSiteYieldOverAlert;
    ed_SiteYieldOverAlert->Text                 =TestIF_File.iSiteYieldOverAlert;
    ed_SiteYieldOverAlertCount->Text            =TestIF_File.iSiteYieldOverAlertCount;
    cbAllSiteFail->Checked                      =TestIF_File.bAllSiteFail;          //kevin 20170825 (wei) 整支ARM Fail bin
    edAllSiteFailCount->Text                    =TestIF_File.iAllSiteFailCount;     //kevin 20180720 (wei) add all site fail count
    edAllSiteFailCountRT->Text                  =TestIF_File.iAllSiteFailCountRT;   //Steven 20230118 : All site fail RT
    cbAllSiteFail_RT->Checked                   =TestIF_File.bAllSiteFail_RT;       //Isaac 20180305 (Steven) ATK要求，只有FT要alarm，FT/RT分開

    if(CUSTOMER_CODE==CC_SIGURD_HUKOU)                                          //KaiChen 20200603 ：矽格-湖口，Yield 頁面 AllSiteFail 不使用
    {
        cbAllSiteFail->Checked      =false;
        cbAllSiteFail_RT->Checked   =false;
    }

    if(CosFunction.bYieldAlarm5)                                                //Sam 20171213 (Steven) : 超豐良率監控 //Sam 20180423 (wei) : MOFile of Yeild Download
    {
        edAlarm5_BySiteIntervalContactCnt->Text     =TestIF_File.iAlarm5_BySiteIntervalContactCnt;
        cbAlarm5_BySiteLowYieldEnable->Checked      =TestIF_File.bAlarm5_BySiteLowYieldEnable;
        edAlarm5_BySiteLowYield->Text               =TestIF_File.dAlarm5_BySiteLowYield;
        edAlarm5_BySiteLowYieldRej->Text            =TestIF_File.dAlarm5_BySiteLowYieldRej;
        cbAlarm5_BySiteCmpYieldEnable->Checked      =TestIF_File.bAlarm5_BySiteCmpYieldEnable;
        edAlarm5_BySiteCmpYield->Text               =TestIF_File.dAlarm5_BySiteCmpYield;
        edAlarm5_BySiteCmpYieldRej->Text            =TestIF_File.dAlarm5_BySiteCmpYieldRej;
        cbAlarm5_BySiteAlarmYieldEnable->Checked    =TestIF_File.bAlarm5_BySiteAlarmYieldEnable;
        edAlarm5_BySiteAlarmYield->Text             =TestIF_File.dAlarm5_BySiteAlarmYield;
        edAlarm5_BySiteAlarmYieldRej->Text          =TestIF_File.dAlarm5_BySiteAlarmYieldRej;
        edAlarm5_OSBin->Text                        =TestIF_File.iAlarm5_OSBin;
        cbAlarm5_BySitePreCmpYieldEnable->Checked   =TestIF_File.bAlarm5_BySitePreCmpYieldEnable;
        edAlarm5_BySitePreCmpYield->Text            =TestIF_File.dAlarm5_BySitePreCmpYield;
        edAlarm5_BySitePreCmpYieldRej->Text         =TestIF_File.dAlarm5_BySitePreCmpYieldRej;
    }

    cbLoadCellMeasure->Checked             = TestIF_File.bLoadCellMeasure;      //kevin 20190907 Arm 測區次數道量測 功能;
    edLoadCellMeasure->Text                = TestIF_File.iLoadCellCount;        //kevin 20190907 Arm 測區次數道量測

    if(CosFunction.bAdaptiveYield)                                              //Sam 20230914 : 自適應性良率監控
    {
        cbAdaptiveLowYield_FT->Checked  =TestIF_File.bAdaptiveLowYield;
        edlContsLowerAlarmNor_FT->Text  =TestIF_File.iAdaptiveContsLowerAlarmNor;       //Sam 20240726 : AI Clean
        edlContsLowerAlarmMin_FT->Text  =TestIF_File.iAdaptiveContsLowerAlarmMin;
        edAdaptiveYieldMax_FT->Text     =TestIF_File.iAdaptiveYieldMax;
        edAdaptiveYieldMin_FT->Text     =TestIF_File.iAdaptiveYieldMin;

        cbAdaptiveLowYield_RT->Checked  =TestIF_File.bAdaptiveLowYield_RT;
        edlContsLowerAlarmNor_RT->Text  =TestIF_File.iAdaptiveContsLowerAlarmNor_RT;    //Sam 20240726 : AI Clean
        edlContsLowerAlarmMin_RT->Text  =TestIF_File.iAdaptiveContsLowerAlarmMin_RT;
        edAdaptiveYieldMax_RT->Text     =TestIF_File.iAdaptiveYieldMax_RT;
        edAdaptiveYieldMin_RT->Text     =TestIF_File.iAdaptiveYieldMin_RT;
    }

    if(CosFunction.bByBinAlarmFromYieldForm)                                    //jou 20180113 (Steven) : By Site By Bin Percent Compare From Yield form
    {
        for(int i=0; i<TEST_MAX_BIN; i++)
        {
            cbByBinSiteGapCat_FT[i]->Checked=TestIF_File.bSpecBinBySiteCompareEnable[FT][i];
            cbByBinSiteGapCat_RT[i]->Checked=TestIF_File.bSpecBinBySiteCompareEnable[RT][i];
            edByBinSiteGapCat_FT[i]->Text   =TestIF_File.dSpecBinBySiteComparePercent[FT][i];
            edByBinSiteGapCat_RT[i]->Text   =TestIF_File.dSpecBinBySiteComparePercent[RT][i];
            cbByArmSiteGapCat_FT[i]->Checked=TestIF_File.bSpecBinByArmPerSiteCompareEnable[FT][i];
            cbByArmSiteGapCat_RT[i]->Checked=TestIF_File.bSpecBinByArmPerSiteCompareEnable[RT][i];
            edByArmSiteGapCat_FT[i]->Text   =TestIF_File.dSpecBinByArmPerSiteComparePercent[FT][i];
            edByArmSiteGapCat_RT[i]->Text   =TestIF_File.dSpecBinByArmPerSiteComparePercent[RT][i];
            cbByBinFailureCat_FT[i]->Checked=TestIF_File.bByBinFailureEnable[FT][i];
            cbByBinFailureCat_RT[i]->Checked=TestIF_File.bByBinFailureEnable[RT][i];
            edByBinFailureCat_FT[i]->Text   =TestIF_File.dByBinFailurePercent[FT][i];
            edByBinFailureCat_RT[i]->Text   =TestIF_File.dByBinFailurePercent[RT][i];
        }
        edByBinSiteGapCatCT_FT->Text        =TestIF_File.iSpecBinBySiteCompareIgnore[FT];
        edByBinSiteGapCatCT_RT->Text        =TestIF_File.iSpecBinBySiteCompareIgnore[RT];
        edByArmSiteGapCatCT_FT->Text        =TestIF_File.iSpecBinByArmPerSiteCompareIgnore[FT];
        edByArmSiteGapCatCT_RT->Text        =TestIF_File.iSpecBinByArmPerSiteCompareIgnore[RT];
        edByBinFailureCatCT_FT->Text        =TestIF_File.iByBinFailureIgnore[FT];
        edByBinFailureCatCT_RT->Text        =TestIF_File.iByBinFailureIgnore[RT];
    }

    if(CosFunction.bSpecailLowYeild)                                            //Sam 20210505 : PTI 要求的兩段 Low Yeild
    {
        cbLowYield_Special->Checked         =TestIF_File.bFailAlarmLowYieldSpecial;
        edLowYield_Special->Text            =TestIF_File.dLowYieldLimitSpecial;
        edLowYieldIg_Special1->Text         =TestIF_File.iLowYieldCountSpecial1;
        edLowYieldIg_Special2->Text         =TestIF_File.iLowYieldCountSpecial2;
    }

//Interval Low Yield Alarm(By Site)(%)                                          //wei 20180606 Interval Low Yield By Site
    cbIntervalLowYieldBySite_FT->Checked    =TestIF_File.bFailAlarmIntervalLowYieldBySite;
    edIntervalLowYieldBySite_FT->Text       =TestIF_File.iIntervalLowYieldLimitBySite;
    edIntervalLowYieldBySite_FT->Text       =TestIF_File.dIntervalLowYieldLimitBySite;
    edIntervalLowYieldBySiteIg_FT->Text     =TestIF_File.iIntervalLowYieldCountBySite;

    cbIntervalLowYieldBySite_RT->Checked    =TestIF_File.bFailAlarmIntervalLowYieldBySite_RT;
    edIntervalLowYieldBySite_RT->Text       =TestIF_File.iIntervalLowYieldLimitBySite_RT;
    edIntervalLowYieldBySite_RT->Text       =TestIF_File.dIntervalLowYieldLimitBySite_RT;
    edIntervalLowYieldBySiteIg_RT->Text     =TestIF_File.iIntervalLowYieldCountBySite_RT;

//Interval Low Yield Alarm(By Total)(%)                                         //wei 20180718 Interval Low Yield By Total
    cbIntervalLowYieldByTotal_FT->Checked   =TestIF_File.bFailAlarmIntervalLowYieldByTotal;
    edIntervalLowYieldByTotal_FT->Text      =TestIF_File.iIntervalLowYieldLimitByTotal;
    edIntervalLowYieldByTotal_FT->Text      =TestIF_File.dIntervalLowYieldLimitByTotal;
    edIntervalLowYieldByTotalIg_FT->Text    =TestIF_File.iIntervalLowYieldCountByTotal;

    cbIntervalLowYieldByTotal_RT->Checked   =TestIF_File.bFailAlarmIntervalLowYieldByTotal_RT;
    edIntervalLowYieldByTotal_RT->Text      =TestIF_File.iIntervalLowYieldLimitByTotal_RT;
    edIntervalLowYieldByTotal_RT->Text      =TestIF_File.dIntervalLowYieldLimitByTotal_RT;
    edIntervalLowYieldByTotalIg_RT->Text    =TestIF_File.iIntervalLowYieldCountByTotal_RT;

    if(CosFunction.bCreateManualEOCAP)                                          //jou 20221104 : VTest CreateManualEOCAP function;
    {
        chkCreateManualEOCAP->Checked=TestIF_File.bCreateManualEOCAP;
    }
}
//---------------------------------------------------------------------------
void TfYieldMonitoring::ReadFile()
{
    AnsiString szDir=GetRecipeFileName("Tester.Data");
    SetClosedSiteBin();                                                         //Steven 20250424 : Fixed for display

//jou 980716 start : add FT/RT alarm
//FT
    if(CUSTOMER_CODE==CC_ASE_CL)                                                //Alick 20160616 by ASE CL
    {
        TestIF_File.bContsFailBySocket              =ReadIniData(szDir, "Alarm", "SocketEnable", 1);
        TestIF_File.bContsFailByHead                =ReadIniData(szDir, "Alarm", "HeadEnable",   1);
    }
    else
    {
        TestIF_File.bContsFailBySocket              =ReadIniData(szDir, "Alarm", "SocketEnable", 0);
        TestIF_File.bContsFailByHead                =ReadIniData(szDir, "Alarm", "HeadEnable",   0);
    }
    TestIF_File.iContsFailSocketAlarmCT             =ReadIniData(szDir, "Alarm", "SocketCT",     5);
    TestIF_File.iContsFailHeadAlarmCT               =ReadIniData(szDir, "Alarm", "HeadCT",       5);

    if(CUSTOMER_CODE==CC_AMKOR_China ||                                         //jou 2015-05-28 Amkor-China 要求將By Bin Count Fail改成連續Fail才Alarm
       CUSTOMER_CODE==CC_QUALCOMM ||                                            //Sam 20200704 : Add Greatek
       CUSTOMER_CODE==CC_Greatek  ||                                            //JerryYang 20170412 (Steven) add QUALCOMM
       CUSTOMER_CODE==CC_HANA_MICRON)                                           //Steven 20230529 : Hana希望改成continue fail
    {
        TestIF_File.bCountSpcBinContinuously_FT     =ReadIniData(szDir, "Alarm", "bCountSpcBinContinuously_FT", true);      //Steven 20230529 : Spc Bin Couont改成連續錯誤
        TestIF_File.bCountSpcBinContinuously_RT     =ReadIniData(szDir, "Alarm", "bCountSpcBinContinuously_RT", true);
    }
    else
    {
        TestIF_File.bCountSpcBinContinuously_FT     =ReadIniData(szDir, "Alarm", "bCountSpcBinContinuously_FT", false);
        TestIF_File.bCountSpcBinContinuously_RT     =ReadIniData(szDir, "Alarm", "bCountSpcBinContinuously_RT", false);
    }

//Site Yield Alarm(%)
    if(bUseTwoArm32Site==true ||                                                //Steven 20220419 : NN mode不需要比by arm
       CUSTOMER_CODE==CC_SIGURD_HUKOU ||                                        //KaiChen 20200304 ：矽格-湖口，關閉 Low Yields%、By Arm Per Site Differ Yield%
       CUSTOMER_CODE==CC_SIGURD_SUZHOU)
    {
        TestIF_File.bFailAlarmSiteYieldDifferent    =false;
    }
    else
    {
        TestIF_File.bFailAlarmSiteYieldDifferent    =ReadIniData(szDir, "Site Yield Alarm", "Site Yield Different",         false);
    }
    if(CosFunction.bYieldAlarmUseDouble)                                        //JerryYang 20160615 Yield相關alarm設定到小數點
    {
        if(CUSTOMER_CODE==CC_QUALCOMM ||
           CUSTOMER_CODE==CC_AMKOR_China)                                       //JerryYang 20181016 (Steven) : Amkor要求yield改為小數點後，存讀檔的部份要與舊版本相容
        {
            TestIF_File.dFailAlarmSiteYield         =ReadIniData(szDir, "Site Yield Alarm", "Site Yield",                       6.0);
            TestIF_File.dFailAlarmSiteYield_RT      =ReadIniData(szDir, "Site Yield Alarm", "Site Yield RT",                    6.0);
        }
        else
        {
            TestIF_File.dFailAlarmSiteYield         =ReadIniData(szDir, "Site Yield Alarm", "Double Site Yield",                     6.0);     //JerryYang 20160530 LowYieldLimit要能設定到小數點
            TestIF_File.dFailAlarmSiteYield_RT      =ReadIniData(szDir, "Site Yield Alarm", "Double Site Yield RT",                  6.0);     //JerryYang 20160530 LowYieldLimit要能設定到小數點
        }
        TestIF_File.iFailAlarmSiteYield             =0;
        TestIF_File.iFailAlarmSiteYield_RT          =0;
    }
    else                                                                        //JerryYang 20160615 Yield相關alarm設定到整數
    {
        TestIF_File.iFailAlarmSiteYield             =ReadIniData(szDir, "Site Yield Alarm", "Site Yield",                       6);
        TestIF_File.iFailAlarmSiteYield_RT          =ReadIniData(szDir, "Site Yield Alarm", "Site Yield RT",                    6);
        TestIF_File.dFailAlarmSiteYield             =double(TestIF_File.iFailAlarmSiteYield);
        TestIF_File.dFailAlarmSiteYield_RT          =double(TestIF_File.iFailAlarmSiteYield_RT);
    }
    TestIF_File.iFailAlarmSiteYieldDifferentCount   =ReadIniData(szDir, "Site Yield Alarm", "Site Yield Different Count",    1000);

    if(bUseTwoArm32Site==true ||                                                //Steven 20220419 : NN mode不需要比by arm
       CUSTOMER_CODE==CC_SIGURD_HUKOU ||                                        //KaiChen 20200304 ：矽格-湖口，關閉 Low Yields%、By Arm Per Site Differ Yield%
       CUSTOMER_CODE==CC_SIGURD_SUZHOU)
    {
        TestIF_File.bFailAlarmSiteYieldDifferent_RT =false;
    }
    else
    {
        TestIF_File.bFailAlarmSiteYieldDifferent_RT =ReadIniData(szDir, "Site Yield Alarm", "Site Yield Different RT",      false);
    }
    TestIF_File.iFailAlarmSiteYield_RT              =ReadIniData(szDir, "Site Yield Alarm", "Site Yield RT",                    6);
    TestIF_File.iFailAlarmSiteYieldDifferentCount_RT=ReadIniData(szDir, "Site Yield Alarm", "Site Yield Different Count RT", 1000);

    if(CosFunction.bLowYieldAutoSiteOff)                                        //Steven 20170905 (wei) : Low Yield Auto Site Off for Ambient
    {
        TestIF_File.bLowYieldAutoSiteOff            =ReadIniData(szDir, "Site Yield Alarm", "Low Yield Auto Site Off",      false);
        TestIF_File.iAlarmWhenSiteOnCountLess       =ReadIniData(szDir, "Site Yield Alarm", "Alarm When Site On Count Less Than", 2);
        TestIF_File.bLowYieldAutoSiteOffByContiFail =ReadIniData(szDir, "Site Yield Alarm", "bLowYieldAutoSiteOffByContiFail", false);          //Steven 20200420 : Continue fail, auto site off
        TestIF_File.bLowYieldAutoSiteOffArmContiFail=ReadIniData(szDir, "Site Yield Alarm", "bLowYieldAutoSiteOffArmContiFail", false);         //Steven 20220818 : By Arm Continue fail, auto site off

        TestIF_File.bLowYieldAutoSiteOffAlarm       =ReadIniData(szDir, "Site Yield Alarm", "Low Yield Auto Site Off Alarm",      false);       //Sam 20221207 : LowYieldAutoSiteOff 新增 Alarm 幾次後再來關 Site
        TestIF_File.iLowYieldAutoSiteOffAlarm       =ReadIniData(szDir, "Site Yield Alarm", "Low Yield Auto Site Off Alarm Count", 2);          //Sam 20221207 : LowYieldAutoSiteOff 新增 Alarm 幾次後再來關 Site
        if(bUseTwoArm32Site==true)                                              //Steven 20220419 : NN mode不需要比by arm
        {
            TestIF_File.bLowYieldAutoSiteOffByArmSite=false;
            TestIF_File.bLowYieldAutoSiteOffByPicker =false;
        }
        else
        {
            TestIF_File.bLowYieldAutoSiteOffByArmSite=ReadIniData(szDir, "Site Yield Alarm", "By Arm Site Low Yield Auto Site Off ", false);     //Steven 20230223 : by arm by site, auto site off
            TestIF_File.bLowYieldAutoSiteOffByPicker =ReadIniData(szDir, "Site Yield Alarm", "By Picker Low Yield Auto Site Off ", false);       //Steven 20230223 : 根據Index吸嘴比較良率
        }

        if(IniConfig.bI28_OnOffSiteOnTheFly ||
           TestIF_File.bLowYieldAutoSiteOff ||
           TestIF_File.bLowYieldAutoSiteOffByContiFail)                         //Steven 20200420 : Continue fail, auto site off
        {
            TestIF_File.iCloseSiteOnHPDontTest      =ReadIniData(szDir, "Site Yield Alarm", "HP Close Site Do Not Test", 0);                    //JerryYang 20180723 (wei) 關site的位置不測試送error bin
            TestIF_File.iCloseSiteBin               =ReadIniData(szDir, "Site Yield Alarm", "Bin of Closed Site", iTestBinCount);               //Steven 20240409 : 關site的位置有IC不測試送指定 bin
        }
        else
        {
            TestIF_File.iCloseSiteBin               =iTestBinCount;             //Steven 20250604 : 關site的位置有IC不測試送指定 bin
            TestIF_File.iCloseSiteOnHPDontTest      =0;                         //JerryYang 20180726 (wei) 關site的位置有IC不測試送error bin
        }
    }
    else
    {
        TestIF_File.iCloseSiteBin                   =iTestBinCount;             //Steven 20250604 : 關site的位置有IC不測試送指定 bin
        TestIF_File.iCloseSiteOnHPDontTest          =0;                         //JerryYang 20180726 (wei) 關site的位置有IC不測試送error bin
        TestIF_File.bLowYieldAutoSiteOff            =false;
    }

//Site Yield Alarm(%)
    TestIF_File.bFailAlarmSiteYieldCmp              =ReadIniData(szDir, "Site Yield Alarm", "Site Yield Compare",           false);

    TestIF_File.iFailAlarmSiteYieldCmpCount         =ReadIniData(szDir, "Site Yield Alarm", "Site Yield Compare Count",      1000);
    TestIF_File.bFailAlarmSiteYieldCmp_RT           =ReadIniData(szDir, "Site Yield Alarm", "Site Yield Compare RT",        false);

    TestIF_File.iFailAlarmSiteYieldCmpCount_RT      =ReadIniData(szDir, "Site Yield Alarm", "Site Yield Compare Count RT",   1000);

    if(CosFunction.bYieldAlarmUseDouble)                                        //JerryYang 20160615 Yield相關alarm設定到小數點
    {
        if(CUSTOMER_CODE==CC_QUALCOMM ||
           CUSTOMER_CODE==CC_AMKOR_China)                                       //JerryYang 20181016 (Steven) : Amkor要求yield改為小數點後，存讀檔的部份要與舊版本相容
        {
            TestIF_File.dFailAlarmSiteYieldCmp      =ReadIniData(szDir, "Site Yield Alarm", "Site Yield Cmp",                 6.0);     //JerryYang 20160530 LowYieldLimit要能設定到小數點
            TestIF_File.dFailAlarmSiteYieldCmp_RT   =ReadIniData(szDir, "Site Yield Alarm", "Site Yield Cmp RT",              6.0);     //JerryYang 20160530 LowYieldLimit要能設定到小數點
        }
        else
        {
            TestIF_File.dFailAlarmSiteYieldCmp      =ReadIniData(szDir, "Site Yield Alarm", "Double Site Yield Cmp",                 6.0);     //JerryYang 20160530 LowYieldLimit要能設定到小數點
            TestIF_File.dFailAlarmSiteYieldCmp_RT   =ReadIniData(szDir, "Site Yield Alarm", "Double Site Yield Cmp RT",              6.0);     //JerryYang 20160530 LowYieldLimit要能設定到小數點
        }
    }
    else                                                                        //JerryYang 20160615 Yield相關alarm設定到整數
    {
        TestIF_File.iFailAlarmSiteYieldCmp          =ReadIniData(szDir, "Site Yield Alarm", "Site Yield Cmp",                   6);
        TestIF_File.iFailAlarmSiteYieldCmp_RT       =ReadIniData(szDir, "Site Yield Alarm", "Site Yield Cmp RT",                6);
        TestIF_File.dFailAlarmSiteYieldCmp          =double(TestIF_File.iFailAlarmSiteYieldCmp);
        TestIF_File.dFailAlarmSiteYieldCmp_RT       =double(TestIF_File.iFailAlarmSiteYieldCmp_RT);
    }

//Low Yield Alarm(%)
    if(CUSTOMER_CODE==CC_SIGURD_HUKOU ||
       CUSTOMER_CODE==CC_SIGURD_SUZHOU)                                         //KaiChen 20200304 ：矽格-湖口，關閉 Low Yields%、By Arm Per Site Differ Yield%
    {
        TestIF_File.bFailAlarmLowYield              =false;
    }
    else
    {
        TestIF_File.bFailAlarmLowYield              =ReadIniData(szDir, "Low Yield Alarm", "Enable",   0);
    }

    if(CosFunction.bYieldAlarmUseDouble)                                        //JerryYang 20160615 Yield相關alarm設定到小數點
    {
        if(CUSTOMER_CODE==CC_QUALCOMM ||
           CUSTOMER_CODE==CC_AMKOR_China)                                       //JerryYang 20181016 (Steven) : Amkor要求yield改為小數點後，存讀檔的部份要與舊版本相容
        {
            TestIF_File.dLowYieldLimit              =ReadIniData(szDir, "Low Yield Alarm", "Limit",   95.0);        //JerryYang 20160530 LowYieldLimit要能設定到小數點
        }
        else
        {
            TestIF_File.dLowYieldLimit              =ReadIniData(szDir, "Low Yield Alarm", "Double Limit",   95.0);        //JerryYang 20160530 LowYieldLimit要能設定到小數點
        }
    }
    else                                                                        //JerryYang 20160615 Yield相關alarm設定到整數
    {
        TestIF_File.iLowYieldLimit                  =ReadIniData(szDir, "Low Yield Alarm", "Limit",   95);
        TestIF_File.dLowYieldLimit                  =double(TestIF_File.iLowYieldLimit);
    }
    TestIF_File.iLowYieldCount                      =ReadIniData(szDir, "Low Yield Alarm", "Count", 1000);
    TestIF_File.bLowYieldAlarmByBin                 =ReadIniData(szDir, "Low Yield Alarm", "Enable By Bin Setting", false);    //Steven 20140828 : By Bin Yield Monitor
    if(CosFunction.bUseLowYieldAlarmByBin==false)                               //Steven 20140828 : By Bin Yield Monitor
        TestIF_File.bLowYieldAlarmByBin=false;

    TestIF_File.bFailRateMode                       =ReadIniData(szDir, "Alarm", "RateEnable",      0);
//    TestIF_File.iIgnoreIC                           =ReadIniData(szDir, "Alarm", "Ignored IC",   1000);
    TestIF_File.iCountAlarmAction                   =ReadIniData(szDir, "Alarm", "Count Action", 0);   //Steven 20101116
    if(CUSTOMER_CODE==CC_AMKOR_Philippines)
    {
        TestIF_File.iCountAlarmAction               =1;
    }
    TestIF_File.bContinuousPass                     =ReadIniData(szDir, "Alarm", "Continuous Pass",  false);//Eliot 20100708
    TestIF_File.iContinuousPassBin                  =ReadIniData(szDir, "Alarm", "Continuous Pass Bin", 1000);//Eliot 20100708
    TestIF_File.iContinuousPassBinCount             =ReadIniData(szDir, "Alarm", "Continuous Pass Bin Count", 1000);//Eliot 20100708

//Continuous Pass by Socket - Steven 20110915
    TestIF_File.bContinuousPassBySocket             =ReadIniData(szDir, "Alarm", "Continuous Pass By Socket",               false);
    TestIF_File.iContinuousPassBinCountBySocket     =ReadIniData(szDir, "Alarm", "Continuous Pass Bin Count By Socket",     1000);

//RT
    if(CUSTOMER_CODE==CC_ASE_CL)                                                //Alick 20160616 by ASE CL
    {
        TestIF_File.bContsFailBySocket_RT           =ReadIniData(szDir, "Alarm", "SocketEnable RT", 1);
        TestIF_File.bContsFailByHead_RT             =ReadIniData(szDir, "Alarm", "HeadEnable RT",   1);
    }
    else
    {
        TestIF_File.bContsFailBySocket_RT           =ReadIniData(szDir, "Alarm", "SocketEnable RT", 0);
        TestIF_File.bContsFailByHead_RT             =ReadIniData(szDir, "Alarm", "HeadEnable RT",   0);
    }
    TestIF_File.iContsFailSocketAlarmCT_RT          =ReadIniData(szDir, "Alarm", "SocketCT RT",     5);
    TestIF_File.iContsFailHeadAlarmCT_RT            =ReadIniData(szDir, "Alarm", "HeadCT RT",       5);

//Consecutive Failure Ignore                                                    //wei 20160115 銅鑼前幾顆不計算ContsFail
    TestIF_File.bContsFailIgnore                    =ReadIniData(szDir, "Alarm", "ContsFailIgnoreEnable", 0);
    TestIF_File.iContsFailIgnore                    =ReadIniData(szDir, "Alarm", "ContsFailIgnoreCT",     5);

    TestIF_File.bContsFailIgnore_RT                 =ReadIniData(szDir, "Alarm", "ContsFailIgnoreEnable RT", 0);
    TestIF_File.iContsFailIgnore_RT                 =ReadIniData(szDir, "Alarm", "ContsFailIgnoreCT RT",     5);

//Low Yield Alarm(%)
    if(CUSTOMER_CODE==CC_SIGURD_HUKOU ||
       CUSTOMER_CODE==CC_SIGURD_SUZHOU)                                         //KaiChen 20200304 ：矽格-湖口，關閉 Low Yields%、By Arm Per Site Differ Yield%
    {
        TestIF_File.bFailAlarmLowYield_RT           =false;
    }
    else
    {
        TestIF_File.bFailAlarmLowYield_RT           =ReadIniData(szDir, "Low Yield Alarm", "Enable RT",   false);
    }

    if(CosFunction.bYieldAlarmUseDouble)                                        //JerryYang 20160615 Yield相關alarm設定到小數點
    {
        if(CUSTOMER_CODE==CC_QUALCOMM ||
           CUSTOMER_CODE==CC_AMKOR_China)                                       //JerryYang 20181016 (Steven) : Amkor要求yield改為小數點後，存讀檔的部份要與舊版本相容
        {
            TestIF_File.dLowYieldLimit_RT           =ReadIniData(szDir, "Low Yield Alarm", "Limit RT",    95.0);
        }
        else
        {
            TestIF_File.dLowYieldLimit_RT           =ReadIniData(szDir, "Low Yield Alarm", "Double Limit RT",    95.0);
        }
    }
    else                                                                        //JerryYang 20160615 Yield相關alarm設定到整數
    {
        TestIF_File.iLowYieldLimit_RT               =ReadIniData(szDir, "Low Yield Alarm", "Limit RT",    95);
        TestIF_File.dLowYieldLimit_RT               =double(TestIF_File.iLowYieldLimit_RT);
    }

    TestIF_File.iLowYieldCount_RT                   =ReadIniData(szDir, "Low Yield Alarm", "Count RT",  1000);

    TestIF_File.bFailRateMode_RT                    =ReadIniData(szDir, "Alarm", "RateEnable RT",                   false);
//    TestIF_File.iIgnoreIC_RT                        =ReadIniData(szDir, "Alarm", "Ignored IC RT",                   1000);
    TestIF_File.iCountAlarmAction_RT                =ReadIniData(szDir, "Alarm", "Count Action RT",                 0);   //Steven 20101116
    if(CUSTOMER_CODE==CC_AMKOR_Philippines)
    {
        TestIF_File.iCountAlarmAction_RT            =1;
    }
    TestIF_File.bContinuousPass_RT                  =ReadIniData(szDir, "Alarm", "Continuous Pass RT",              false);//Eliot 20100708
    TestIF_File.iContinuousPassBin_RT               =ReadIniData(szDir, "Alarm", "Continuous Pass RT Bin",          1);//Eliot 20100708
    TestIF_File.iContinuousPassBinCount_RT          =ReadIniData(szDir, "Alarm", "Continuous Pass Bin Count RT",    1000);//Eliot 20100708

//Continuous Pass by Socket - Steven 20110915
    TestIF_File.bContinuousPassBySocket_RT          =ReadIniData(szDir, "Alarm", "Continuous Pass RT By Socket",            false);
    TestIF_File.iContinuousPassBinCountBySocket_RT  =ReadIniData(szDir, "Alarm", "Continuous Pass Bin Count RT By Socket",  1000);

//Low Yield Alarm(By Total)(%)                                                  //wei 20151116 Low Yield By Total
    TestIF_File.bFailAlarmLowYieldByTotal           =ReadIniData(szDir, "Low Yield Alarm", "By Total Enable", false);
    TestIF_File.iLowYieldCountByTotal               =ReadIniData(szDir, "Low Yield Alarm", "By Total Count", 1000);
    TestIF_File.bFailAlarmLowYieldByTotal_RT        =ReadIniData(szDir, "Low Yield Alarm", "By Total Enable RT",   false);
    TestIF_File.iLowYieldCountByTotal_RT            =ReadIniData(szDir, "Low Yield Alarm", "By Total Count RT",  1000);
    if(CosFunction.bYieldAlarmUseDouble)                                        //JerryYang 20160615 Yield相關alarm設定到小數點
    {
        if(CUSTOMER_CODE==CC_QUALCOMM ||
           CUSTOMER_CODE==CC_AMKOR_China)                                       //JerryYang 20181016 (Steven) : Amkor要求yield改為小數點後，存讀檔的部份要與舊版本相容
        {
            TestIF_File.dLowYieldLimitByTotal       =ReadIniData(szDir, "Low Yield Alarm", "By Total Limit",          95.0);            //JerryYang 20160530 LowYieldLimit要能設定到小數點
            TestIF_File.dLowYieldLimitByTotal_RT    =ReadIniData(szDir, "Low Yield Alarm", "By Total Limit RT",       95.0);
        }
        else
        {
            TestIF_File.dLowYieldLimitByTotal       =ReadIniData(szDir, "Low Yield Alarm", "Double By Total Limit",          95.0);     //JerryYang 20160530 LowYieldLimit要能設定到小數點
            TestIF_File.dLowYieldLimitByTotal_RT    =ReadIniData(szDir, "Low Yield Alarm", "Double By Total Limit RT",       95.0);
        }
    }
    else                                                                        //JerryYang 20160615 Yield相關alarm設定到整數
    {
        TestIF_File.iLowYieldLimitByTotal           =ReadIniData(szDir, "Low Yield Alarm", "By Total Limit",   95);
        TestIF_File.iLowYieldLimitByTotal_RT        =ReadIniData(szDir, "Low Yield Alarm", "By Total Limit RT",    95);
        TestIF_File.dLowYieldLimitByTotal           =double(TestIF_File.iLowYieldLimitByTotal);
        TestIF_File.dLowYieldLimitByTotal_RT        =double(TestIF_File.iLowYieldLimitByTotal_RT);
    }

//By Picker Compare Yield   [每個吸嘴的良率互比]                                //Steven 20230223 : 根據Index吸嘴比較良率
    TestIF_File.bLowYieldByPicker           =ReadIniData(szDir, "Low Yield Alarm", "By Picker Enable",      false);
    TestIF_File.iLowYieldCountByPicker      =ReadIniData(szDir, "Low Yield Alarm", "By Picker Count",       1000);
    TestIF_File.dLowYieldByPicker           =ReadIniData(szDir, "Low Yield Alarm", "By Picker Limit",       95.0);
    TestIF_File.bLowYieldByPicker_RT        =ReadIniData(szDir, "Low Yield Alarm", "By Picker Enable RT",   false);
    TestIF_File.iLowYieldCountByPicker_RT   =ReadIniData(szDir, "Low Yield Alarm", "By Picker Count RT",    1000);
    TestIF_File.dLowYieldByPicker_RT        =ReadIniData(szDir, "Low Yield Alarm", "By Picker Limit RT",    95.0);

    if(CosFunction.bSpecailLowYeild)                                            //Sam 20210505 : PTI 要求的兩段 Low Yeild
    {
        TestIF_File.bFailAlarmLowYieldSpecial       =ReadIniData(szDir, "Low Yield Alarm", "Special Enable", false);
        TestIF_File.iLowYieldCountSpecial1          =ReadIniData(szDir, "Low Yield Alarm", "Special Count1", 1000);
        TestIF_File.iLowYieldCountSpecial2          =ReadIniData(szDir, "Low Yield Alarm", "Special Count2", 1000);
        if(CosFunction.bYieldAlarmUseDouble)
        {
            TestIF_File.dLowYieldLimitSpecial       =ReadIniData(szDir, "Low Yield Alarm", "Special Limit",          95.0);
        }
        else
        {
            TestIF_File.iLowYieldLimitSpecial       =ReadIniData(szDir, "Low Yield Alarm", "Special Limit",          95);
            TestIF_File.dLowYieldLimitSpecial       =double(TestIF_File.iLowYieldLimitSpecial);
        }
    }

//Interval Low Yield Alarm(By Site)(%)                                          //wei 20180606 Interval Low Yield By Site
    if(CosFunction.IntervalYieldCount==false)                                   //Steven 20230223 : 隱藏沒用到的
    {
        TestIF_File.bFailAlarmIntervalLowYieldBySite=false;
        TestIF_File.bFailAlarmIntervalLowYieldBySite_RT=false;
        TestIF_File.bFailAlarmIntervalLowYieldByTotal=false;
        TestIF_File.bFailAlarmIntervalLowYieldByTotal_RT=false;
    }
    else
    {
        TestIF_File.bFailAlarmIntervalLowYieldBySite            =ReadIniData(szDir, "Low Yield Alarm", "Interval By Site Enable", false);
        TestIF_File.iIntervalLowYieldCountBySite                =ReadIniData(szDir, "Low Yield Alarm", "Interval By Site Count", 300);
        TestIF_File.bFailAlarmIntervalLowYieldBySite_RT         =ReadIniData(szDir, "Low Yield Alarm", "Interval By Site Enable RT",   false);
        TestIF_File.iIntervalLowYieldCountBySite_RT             =ReadIniData(szDir, "Low Yield Alarm", "Interval By Site Count RT",  300);
        if(CosFunction.bYieldAlarmUseDouble)                                    //JerryYang 20160615 Yield相關alarm設定到小數點
        {
            TestIF_File.dIntervalLowYieldLimitBySite            =ReadIniData(szDir, "Low Yield Alarm", "Interval Double By Site Limit",          95.0);                   //JerryYang 20160530 LowYieldLimit要能設定到小數點
            TestIF_File.dIntervalLowYieldLimitBySite_RT         =ReadIniData(szDir, "Low Yield Alarm", "Interval Double By Site Limit RT",       95.0);
        }
        else                                                                    //JerryYang 20160615 Yield相關alarm設定到整數
        {
            TestIF_File.iIntervalLowYieldLimitBySite            =ReadIniData(szDir, "Low Yield Alarm", "Interval By Site Limit",   95);
            TestIF_File.iIntervalLowYieldLimitBySite_RT         =ReadIniData(szDir, "Low Yield Alarm", "Interval By Site Limit RT",    95);
            TestIF_File.dIntervalLowYieldLimitBySite            =double(TestIF_File.iIntervalLowYieldLimitBySite);
            TestIF_File.dIntervalLowYieldLimitBySite_RT         =double(TestIF_File.iIntervalLowYieldLimitBySite_RT);
        }

    //Interval Low Yield Alarm(By Total)(%)                                     //wei 20180718 Interval Low Yield By Total
        TestIF_File.bFailAlarmIntervalLowYieldByTotal           =ReadIniData(szDir, "Low Yield Alarm", "Interval By Total Enable", false);
        TestIF_File.iIntervalLowYieldCountByTotal               =ReadIniData(szDir, "Low Yield Alarm", "Interval By Total Count", 300);
        TestIF_File.bFailAlarmIntervalLowYieldByTotal_RT        =ReadIniData(szDir, "Low Yield Alarm", "Interval By Total Enable RT",   false);
        TestIF_File.iIntervalLowYieldCountByTotal_RT            =ReadIniData(szDir, "Low Yield Alarm", "Interval By Total Count RT",  300);
        if(CosFunction.bYieldAlarmUseDouble)                                    //JerryYang 20160615 Yield相關alarm設定到小數點
        {
            TestIF_File.dIntervalLowYieldLimitByTotal           =ReadIniData(szDir, "Low Yield Alarm", "Interval Double By Total Limit",          95.0);                   //JerryYang 20160530 LowYieldLimit要能設定到小數點
            TestIF_File.dIntervalLowYieldLimitByTotal_RT        =ReadIniData(szDir, "Low Yield Alarm", "Interval Double By Total Limit RT",       95.0);
        }
        else                                                                    //JerryYang 20160615 Yield相關alarm設定到整數
        {
            TestIF_File.iIntervalLowYieldLimitByTotal           =ReadIniData(szDir, "Low Yield Alarm", "Interval By Total Limit",   95);
            TestIF_File.iIntervalLowYieldLimitByTotal_RT        =ReadIniData(szDir, "Low Yield Alarm", "Interval By Total Limit RT",    95);
            TestIF_File.dIntervalLowYieldLimitByTotal           =double(TestIF_File.iIntervalLowYieldLimitByTotal);
            TestIF_File.dIntervalLowYieldLimitByTotal_RT        =double(TestIF_File.iIntervalLowYieldLimitByTotal_RT);
        }
    }

    if(CUSTOMER_CODE==CC_TSI)
    {
        TestIF_File.bFailAlarmIntervalLowYieldBySite=false;
        TestIF_File.bFailAlarmIntervalLowYieldBySite_RT=false;
    }
//jou 980716 end

//Steven 20090821 Start : Alarm Count
    TestIF_File.iACAlarmType                        =ReadIniData(szDir, "Alarm Count", "Alarm Type",    4);
    TestIF_File.iACGroupMethod                      =ReadIniData(szDir, "Alarm Count", "Group Method",  0);
    TestIF_File.iACPeriod                           =ReadIniData(szDir, "Alarm Count", "Period",        10);
    TestIF_File.iACCounts                           =ReadIniData(szDir, "Alarm Count", "Counts",        10);
//Steven 20090821 End

//jou 2010-11-17 Start
    TestIF_File.bContinuousLoader                   =ReadIniData(szDir, "Alarm", "Continuous Loader", false);
    TestIF_File.iContinuousLoaderCount              =ReadIniData(szDir, "Alarm", "Continuous Loader Count", 1000);

    TestIF_File.bContinuousLoader_RT                =ReadIniData(szDir, "Alarm", "Continuous Loader RT", false);
    TestIF_File.iContinuousLoaderCount_RT           =ReadIniData(szDir, "Alarm", "Continuous Loader Count RT", 1000);
//jou 2010-11-17 end

//Steven 20110426 Start
    TestIF_File.bContinuousContact                  =ReadIniData(szDir, "Alarm", "Continuous Contact", false);
    TestIF_File.iContinuousContactCount             =ReadIniData(szDir, "Alarm", "Continuous Contact Count", 1000);

    TestIF_File.bContinuousContact_RT               =ReadIniData(szDir, "Alarm", "Continuous Contact RT", false);
    TestIF_File.iContinuousContactCount_RT          =ReadIniData(szDir, "Alarm", "Continuous Contact Count RT", 1000);

    TestIF_File.bLoadCellMeasure                    =ReadIniData(szDir, "Alarm", "LoadCellMeasure", 0);    //kevin 20190907 Arm 測區次數道量測 功能;
    TestIF_File.iLoadCellCount                      =ReadIniData(szDir, "Alarm", "LoadCellMeasureCount", 1000);   //kevin 20190907 Arm 測區次數道量測

    if(IniConfig.bD56YieldPiggyBackEnable)                                      //kevin 20131101 強致 Enable Yield 裡面piggyback 功能
    {
        TestIF_File.bContinuousContact           =true;
        TestIF_File.bContinuousContact_RT        =true;
        TestIF_File.bContinuousPassBySocket      =true;
        TestIF_File.bContinuousPassBySocket_RT   =true;
        TestIF_File.bContinuousLoader            =true;
        TestIF_File.bContinuousLoader_RT         =true;
        TestIF_File.bContinuousPass              =true;
        TestIF_File.bContinuousPass_RT           =true;
    }
//Steven 20110426 End

    if(CUSTOMER_CODE==CC_SCC)
    {
        TestIF_File.bFailRateMode               =false;
        TestIF_File.bFailRateMode_RT            =false;
//        TestIF_File.bContinuousPass             =false;                       //Steven 20210421 : Mark for JSCC request.
//        TestIF_File.bContinuousPass_RT          =false;
//        TestIF_File.bContinuousPassBySocket     =false;
//        TestIF_File.bContinuousPassBySocket_RT  =false;
//        TestIF_File.bContinuousLoader           =false;
//        TestIF_File.bContinuousLoader_RT        =false;
    }

    if(CosFunction.bPiggyBackForASE==true)                                      //Steven 20131101 : 高雄ASE不要Continual Pass Bin(Total )跟 Continual Loader兩種
    {
        TestIF_File.bContinuousPass             =false;
        TestIF_File.bContinuousPass_RT          =false;
        TestIF_File.bContinuousLoader           =false;
        TestIF_File.bContinuousLoader_RT        =false;
    }

    CheckSettingNo();                                                           //Steven 20110506

    if(IniConfig.bShowFunctionWindow)                                           //jou 2010-12-04 start
    {
        if((LastSet.iRunStartMode==rsmContinuStart ||
            LastSet.iRunStartMode==rsmInitialStart ||
            LastSet.iRunStartMode==rsmAutoSiteMap ||
            LastSet.iRunStartMode==rsmQAMode) ||                                //ChungHung 20120725 add QAMode 使用 Noraml
            (CosFunction.bUseSCKART &&
             (LastSet.iRunStartMode==rsmInitial_ART ||
              LastSet.iRunStartMode==rsmContinuStart_ART)))                     //Steven 20161214 (wei) : For SCK ART
        {
            //ChungHung add 20120730 若其中一個Item 都沒選 則秀OFF
            fTemperFrom->SetShowYield(fTemperFrom->esytDoubleDevice,    (TestIF_File.bContinuousPass   || TestIF_File.bContinuousPassBySocket ||
                                                                         TestIF_File.bContinuousLoader || TestIF_File.bContinuousContact));
            fTemperFrom->SetShowYield(fTemperFrom->esytCGoodBin,        TestIF_File.bContinuousPass==true || TestIF_File.bContinuousPassBySocket==true);   //Steven 20110920
            fTemperFrom->SetShowYield(fTemperFrom->esytYieldMonitor,    TestIF_File.bFailAlarmLowYield || TestIF_File.bFailAlarmSiteYieldCmp || TestIF_File.bFailAlarmSiteYieldDifferent);
            fTemperFrom->SetShowYield(fTemperFrom->esytConsAlarm,       TestIF_File.bContsFailBySocket==true || TestIF_File.bContsFailByHead==true);
        }
        else
        {
            //ChungHung add 20120730 若其中一個Item 都沒選 則秀OFF
            fTemperFrom->SetShowYield(fTemperFrom->esytDoubleDevice,    (TestIF_File.bContinuousPass_RT   || TestIF_File.bContinuousPassBySocket_RT ||
                                                                         TestIF_File.bContinuousLoader_RT || TestIF_File.bContinuousContact_RT));
            fTemperFrom->SetShowYield(fTemperFrom->esytCGoodBin,        TestIF_File.bContinuousPass_RT==true || TestIF_File.bContinuousPassBySocket_RT==true); //Steven 20110920
            fTemperFrom->SetShowYield(fTemperFrom->esytYieldMonitor,    TestIF_File.bFailAlarmLowYield_RT || TestIF_File.bFailAlarmSiteYieldCmp_RT || TestIF_File.bFailAlarmSiteYieldDifferent_RT);
            fTemperFrom->SetShowYield(fTemperFrom->esytConsAlarm,       TestIF_File.bContsFailBySocket_RT==true || TestIF_File.bContsFailByHead_RT==true);

        }
        fTemperFrom->SetShowYield(fTemperFrom->esytTest2, IniConfig.bFTContinueON);
        fTemperFrom->strShowYield[fTemperFrom->esytTest2].bShow=IniConfig.bFTContinueON; //kevin 20121008 RT
        fTemperFrom->ShowYieldFuntion();
    }

    //wei 20141201 Low Yield Auto Clean(%) start
    TestIF_File.bFailAlarmLowYield_AutoClean        =ReadIniData(szDir, "Low Yield Auto Clean", "Enable",   0);
    TestIF_File.iLowYieldLimit_AutoClean            =ReadIniData(szDir, "Low Yield Auto Clean", "Limit",   10);
    TestIF_File.iLowYieldCount_AutoClean            =ReadIniData(szDir, "Low Yield Auto Clean", "Count",  100);
    //wei 20141201 Low Yield Auto Clean(%) end

    if(CosFunction.bAutoCloseSiteWhenRT)                                        //Steven 20200225 : 切到RT的時候,要關閉Socket
    {
        TestIF_File.iAutoCloseSiteWhenRT            =CheckRange(ReadIniData(szDir, "Auto Close Site When RT", "iAutoCloseSiteYieldWhenRT",   0), 0, 1);
        TestIF_File.dAutoCloseSiteYieldWhenRT       =CheckRange(ReadIniData(szDir, "Auto Close Site When RT", "dAutoCloseSiteYieldWhenRT",   0.0), 50.00, 0.01);
        TestIF_File.iAllSiteOnAtInitialStart        =CheckRange(ReadIniData(szDir, "Site On at Initial Start", "iAllSiteOnAtInitialStart",   1), 0, 1);    //Steven 20230814 : Initial Start的時候要全開Site
    }
    TestIF_File.iAutoSiteOffByGPIB                  =CheckRange(ReadIniData(szDir, "AutoSiteOnoff", "iAutoSiteOffByGPIB",   0), 0, 1);
    if(IniConfig.bE53LowYieldAutoClean==false)                                  //kevin 20160802
        TestIF_File.bFailAlarmLowYield_AutoClean=false;

 //---AutoRetest setup  kevin 20150704 start
    if(CUSTOMER_CODE==CC_ASE_KaohSiung)
    {
        TestIF_File.bEnablePassYieldART             =ReadIniData(szDir, "AutoRetest", "bPassART", false);//使用 PASS 比較
        MyYieldPanel[0]->bEnablePassYieldART        =bFirstYieldCmp_ART             =TestIF_File.bEnablePassYieldART;
        TestIF_File.fPassYieldART                   =ReadIniData(szDir, "AutoRetest", "fPassyieldART", 0.00);
        MyYieldPanel[0]->fPassYieldART              =fFirstYieldSet_ART             =TestIF_File.fPassYieldART;

        TestIF_File.bEnableOpenShortART             =ReadIniData(szDir, "AutoRetest", "bOpenshortART", false);
        MyYieldPanel[0]->bEnableOpenShortART        =bOpenShortYieldCmp_ART         =TestIF_File.bEnableOpenShortART;
        TestIF_File.fOpenShortYieldART              =ReadIniData(szDir, "AutoRetest", "fOpenShortYieldART", 0.0);
        MyYieldPanel[0]->fOpenShortYieldART         =fOpenShortYieldSet_ART         =TestIF_File.fOpenShortYieldART;

        TestIF_File.bEnableRecoverART               =ReadIniData(szDir, "AutoRetest", "bRecoverART", false);
        MyYieldPanel[0]->bEnableRecoverART          =bRecoverRateYieldCmp_ART       =TestIF_File.bEnableRecoverART;
        TestIF_File.fRecoverYieldART                =ReadIniData(szDir, "AutoRetest", "fRecoverYieldART", 0.0);
        MyYieldPanel[0]->fRecoverYieldART           =fRecoverRateYieldRT1Set_ART    =TestIF_File.fRecoverYieldART;

        iAutoRetestLimit                            =ReadIniData(szDir, "AutoRetest", "ArtAutoRetestLimit", 0);  //RT 次數
        if(iAutoRetestLimit<=0)
            iAutoRetestLimit=1;
        MyYieldPanel[0]->ReTestLimit->ItemIndex     =iAutoRetestLimit-1;

        AnsiString bBin="", bBin1="";
        AnsiString abuffer="", abuffer1="";                                     //kevin 20170825 (wei) add

        abuffer= CheckAndReadIniData(szDir, "AutoRetest", "bPassBin", AnsiString(""));
        abuffer1=CheckAndReadIniData(szDir, "AutoRetest", "bOpenShortBin", AnsiString(""));  //kevin 20170825 add
        if(abuffer=="" || abuffer1=="")
        {
            for(int i=0; i<iTestBinCount; i++)                                  //Steven 20251104 : iBinCount --> iTestBinCount
            {
                abuffer.sprintf("%s,","0");
                bBin+= abuffer;
                abuffer1.sprintf("%s,","0");
                bBin1+= abuffer1;
            }
            WriteIniData(szDir, "AutoRetest", "bPassBin", bBin);
            WriteIniData(szDir, "AutoRetest", "bOpenShortBin", bBin1);

            abuffer= CheckAndReadIniData(szDir, "AutoRetest", "bPassBin", AnsiString(""));
            abuffer1=CheckAndReadIniData(szDir, "AutoRetest", "bOpenShortBin", AnsiString(""));  //kevin 20170825 add
        }

        for(int i=0; i<iTestBinCount; i++)                                      //Steven 20251104 : iBinCount --> iTestBinCount
        {
            TestIF_File.bPass[i]                    =(abuffer.SubString(i*2+1,1).Trim()==1);
            MyYieldPanel[0]->bPass[i]               =TestIF_File.bPass[i];

            TestIF_File.bOpenShort[i]               =(abuffer1.SubString(i*2+1,1).Trim()==1);
            MyYieldPanel[0]->bOpenShort[i]          =TestIF_File.bOpenShort[i];
        }
    }

    if(CUSTOMER_CODE==CC_KYEC_LEE)                                              //wei 20150825 KYEC 修改成By File
    {
        iAutoRetestLimitFile                        =ReadIniData(szDir, "AutoRetest", "ArtAutoRetestLimitFile", 1);         //RT 次數;       //wei  20150825 做AUTO RETEST最大次數
        iFailYieldRate_ARTFile                      =ReadIniData(szDir, "AutoRetest", "ArtFailYieldRate_ARTFile", 0.0);     //RT 次數;       //wei  20150825

        iAutoLeastRetestLimitFile                   =ReadIniData(szDir, "AutoRetest", "AutoLeastRetestLimitFile", 1);           //wei 20160203
        dFailYieldRate_ARTFTFile[0]                 =ReadIniData(szDir, "AutoRetest", "ArtFailYieldMinRate_ARTFTFile", 0.0);    //wei 20160203
        dFailYieldRate_ARTFTFile[1]                 =ReadIniData(szDir, "AutoRetest", "ArtFailYieldRate_ARTFTFile", 0.0);       //wei 20160203
        dFailYieldRate_ARTFTFile[2]                 =ReadIniData(szDir, "AutoRetest", "ArtFailYieldMaxRate_ARTFTFile", 0.0);    //wei 20160203
        dFailYieldRate_ARTRTFile[0]                 =ReadIniData(szDir, "AutoRetest", "ArtFailYieldMinRate_ARTRTFile", 0.0);    //wei 20160203
        dFailYieldRate_ARTRTFile[1]                 =ReadIniData(szDir, "AutoRetest", "ArtFailYieldRate_ARTRTFile", 0.0);       //wei 20160203
        dFailYieldRate_ARTRTFile[2]                 =ReadIniData(szDir, "AutoRetest", "ArtFailYieldMaxRate_ARTRTFile", 0.0);    //wei 20160203
        bAutoLeastRetestFile                        =ReadIniData(szDir, "AutoRetest", "bAutoLeastRetestFile", false);           //wei 20160203
        bUseFailNoDistinction                       =ReadIniData(szDir, "AutoRetest", "bUseFailNoDistinction", false);          //wei 20160203

        iUseFTFailYield                             =ReadIniData(szDir, "AutoRetest", "ArtUseFTFailYield", 0);                  //wei 20160203
        iUseRTFailYield                             =ReadIniData(szDir, "AutoRetest", "ArtUseRTFailYield", 0);                  //wei 20160203
        iUseFTFailYieldModel                        =ReadIniData(szDir, "AutoRetest", "ArtUseFTFailYieldModel", 0);             //wei 20160203
        iUseRTFailYieldModel                        =ReadIniData(szDir, "AutoRetest", "ArtUseRTFailYieldModel", 0);             //wei 20160203

        edAutoRetestLimitFile->Text                 =iAutoRetestLimitFile;
        edFailYieldRate_ARTFile->Text               =iFailYieldRate_ARTFile;

        edAutoLeastRetestLimitFile->Text            =iAutoLeastRetestLimitFile;                                         //wei 20160203
        edFailYieldMinRate_ARTFTFile->Text          =dFailYieldRate_ARTFTFile[0];                                       //wei 20160203
        edFailYieldRate_ARTFTFile->Text             =dFailYieldRate_ARTFTFile[1];
        edFailYieldMaxRate_ARTFTFile->Text          =dFailYieldRate_ARTFTFile[2];
        edFailYieldMinRate_ARTRTFile->Text          =dFailYieldRate_ARTRTFile[0];
        edFailYieldRate_ARTRTFile->Text             =dFailYieldRate_ARTRTFile[1];
        edFailYieldMaxRate_ARTRTFile->Text          =dFailYieldRate_ARTRTFile[2];
        rgFT_ART->ItemIndex                         =iUseFTFailYield;
        rgRT_ART->ItemIndex                         =iUseRTFailYield;

        cbUseFTFailYieldModel->ItemIndex            =iUseFTFailYieldModel;
        cbUseRTFailYieldModel->ItemIndex            =iUseRTFailYieldModel;

        ckUseLeastRetestTimes->Checked              =bAutoLeastRetestFile;
        ckUseFailNoDistinction->Checked             =bUseFailNoDistinction;
        IniConfig.iAutoRetestLimit                  =iAutoRetestLimitFile;
        IniConfig.iFailYieldRate_ART                =iFailYieldRate_ARTFile;
        pnlARTFailYiel->Visible=false;
    }

    if(CosFunction.bYieldAlarm4)                                                //wei 20160314
    {
        TestIF_File.bAlarm4ContinueType_Enable          =ReadIniData(szDir, "Alarm4", "bAlarm4ContinueType_Enable"                  ,        false);
        TestIF_File.iAlarm4ContinueType_IntervalCount   =ReadIniData(szDir, "Alarm4", "iAlarm4ContinueType_IntervalCount"           ,        1);
        TestIF_File.iAlarm4ContinueType_ContinueCount   =ReadIniData(szDir, "Alarm4", "iAlarm4ContinueType_ContinueCount"           ,        1);

        TestIF_File.bAlarm4EnableIntervalYield          =ReadIniData(szDir, "Alarm4", "bAlarm4EnableIntervalYield"                  ,        false);
        TestIF_File.iAlarm4IntervalYieldIntervalCount   =ReadIniData(szDir, "Alarm4", "iAlarm4IntervalYieldIntervalCount"           ,        1);
        TestIF_File.iAlarm4IntervalYieldContinueCount   =CheckRange(ReadIniData(szDir, "Alarm4", "iAlarm4IntervalYieldContinueCount"           ,        1), iMinYield, iMaxYield);
        TestIF_File.iAlarm4IntervalYieldYield           =ReadIniData(szDir, "Alarm4", "iAlarm4IntervalYieldYield"                   ,        1);

        TestIF_File.bSiteToSiteYieldCmp                 =ReadIniData(szDir, "Alarm4", "SiteToSiteYieldCmp_Enable",              false);
        TestIF_File.iSiteToSiteYieldCmp                 =ReadIniData(szDir, "Alarm4", "SiteToSiteYieldCmp_Yield",                   0);
        TestIF_File.iSiteToSiteYieldCmpCount            =ReadIniData(szDir, "Alarm4", "SiteToSiteYieldCmp_IntervalCount",        1000);

        TestIF_File.bHeadToHeadYieldCmp                 =ReadIniData(szDir, "Alarm4", "HeadToHeadYieldCmp_Enable",              false);
        TestIF_File.iHeadToHeadYieldCmp                 =ReadIniData(szDir, "Alarm4", "HeadToHeadYieldCmp_Yield",                   0);
        TestIF_File.iHeadToHeadYieldCmpCount            =ReadIniData(szDir, "Alarm4", "HeadToHeadYieldCmp_IntervalCount",        1000);

        TestIF_File.bSiteYieldOverAlert                 =ReadIniData(szDir, "Alarm4", "SiteYieldOverAlert_Enable",              false);
        TestIF_File.iSiteYieldOverAlert                 =ReadIniData(szDir, "Alarm4", "SiteYieldOverAlert_Yield",                   0);
        TestIF_File.iSiteYieldOverAlertCount            =ReadIniData(szDir, "Alarm4", "SiteYieldOverAlert_IntervalCount",        1000);
    }
    else
    {
        TestIF_File.bAlarm4ContinueType_Enable          =false;
        TestIF_File.bAlarm4EnableIntervalYield          =false;
        TestIF_File.bSiteToSiteYieldCmp                 =false;
        TestIF_File.bHeadToHeadYieldCmp                 =false;
        TestIF_File.bSiteYieldOverAlert                 =false;
//        TestIF_File.bAllSiteFail                        =false;
//        TestIF_File.bAllSiteFail_RT                     =false;
    }

    //Steven 20231017 : change position
    TestIF_File.bAllSiteFail                            =ReadIniData(szDir, "Alarm4", "AllSiteFail",                            false); //kevin 20170825 (wei) 整支ARM Fail bin
    TestIF_File.iAllSiteFailCount                       =ReadIniData(szDir, "Alarm4", "AllSiteFailCount",                        0);    //kevin 20180720 (wei) all site fail count
    TestIF_File.iAllSiteFailCountRT                     =ReadIniData(szDir, "Alarm4", "AllSiteFailCount_RT",                     0);    //Steven 20230118 : All site fail RT
    TestIF_File.bAllSiteFail_RT                         =ReadIniData(szDir, "Alarm4", "AllSiteFail_RT",                         false); //Isaac 20180305 (Steven) ATK要求，只有FT要alarm，FT/RT分開

    if(CosFunction.bYieldAlarm5)                                                //Sam 20171213 (Steven) : 超豐良率監控 //Sam 20180423 (wei) : MOFile of Yeild Download
    {
        TestIF_File.iAlarm5_BySiteIntervalContactCnt    =ReadIniData(szDir, "Alarm5", "iAlarm5_BySiteIntervalContactCnt",         100);
        TestIF_File.bAlarm5_BySiteLowYieldEnable        =ReadIniData(szDir, "Alarm5", "bAlarm5_BySiteLowYieldEnable",           false);
        TestIF_File.dAlarm5_BySiteLowYield              =ReadIniData(szDir, "Alarm5", "dAlarm5_BySiteLowYield",                  95.0);
        TestIF_File.dAlarm5_BySiteLowYieldRej           =ReadIniData(szDir, "Alarm5", "dAlarm5_BySiteLowYieldRej",               95.0);
        TestIF_File.bAlarm5_BySiteCmpYieldEnable        =ReadIniData(szDir, "Alarm5", "bAlarm5_BySiteCmpYieldEnable",           false);
        TestIF_File.dAlarm5_BySiteCmpYield              =ReadIniData(szDir, "Alarm5", "dAlarm5_BySiteCmpYield",                     5);
        TestIF_File.dAlarm5_BySiteCmpYieldRej           =ReadIniData(szDir, "Alarm5", "dAlarm5_BySiteCmpYieldRej",                 10);
        TestIF_File.bAlarm5_BySiteAlarmYieldEnable      =ReadIniData(szDir, "Alarm5", "bAlarm5_BySiteAlarmYieldEnable",         false);
        TestIF_File.dAlarm5_BySiteAlarmYield            =ReadIniData(szDir, "Alarm5", "dAlarm5_BySiteAlarmYield",                 0.5);
        TestIF_File.dAlarm5_BySiteAlarmYieldRej         =ReadIniData(szDir, "Alarm5", "dAlarm5_BySiteAlarmYieldRej",                1);
        TestIF_File.iAlarm5_OSBin                       =ReadIniData(szDir, "Alarm5", "iAlarm5_OSBin",                              5);
        TestIF_File.bAlarm5_BySitePreCmpYieldEnable     =ReadIniData(szDir, "Alarm5", "bAlarm5_BySitePreCmpYieldEnable",        false);
        TestIF_File.dAlarm5_BySitePreCmpYield           =ReadIniData(szDir, "Alarm5", "dAlarm5_BySitePreCmpYield",                  5);
        TestIF_File.dAlarm5_BySitePreCmpYieldRej        =ReadIniData(szDir, "Alarm5", "dAlarm5_BySitePreCmpYieldRej",              10);
    }
    else
    {
        TestIF_File.bAlarm5_BySiteLowYieldEnable    =false;
        TestIF_File.bAlarm5_BySiteCmpYieldEnable    =false;
        TestIF_File.bAlarm5_BySiteAlarmYieldEnable  =false;
        TestIF_File.bAlarm5_BySitePreCmpYieldEnable =false;
    }

    //---AutoRetest setup  kevin 20150704 end
    if(USE_AUTO_RETEST==eartUninstall)
    {
        bFirstYieldCmp_ART=false;
        bOpenShortYieldCmp_ART=false;
        bRecoverRateYieldCmp_ART=false;
    }

    if(bFirstYieldCmp_ART || bOpenShortYieldCmp_ART || bRecoverRateYieldCmp_ART)  //kevin 20150704
    {
        bAutoReTest_ART=true;                                                   // 起動AUTORETEST
    }
    else
    {
        bAutoReTest_ART=false;
    }
    //kevin 20150601 end
    if(CosFunction.bPiggybackFunctionByHandler==true)                           //Isaac 20170712 :Piggyback function By Handlder(save file to config.ini)
    {
        AnsiString S="";
        S="config.ini";
        AnsiString szDir="";
        szDir=AuthPath+S;

        //Alarm Action
        TestIF_File.iCountAlarmAction          =ReadIniData(szDir, "Alarm", "Count Action",                 0);
        TestIF_File.iCountAlarmAction_RT       =ReadIniData(szDir, "Alarm", "Count Action RT",              0);

        //Continuous Pass
        TestIF_File.bContinuousPass            =ReadIniData(szDir, "Alarm", "Continuous Pass",              false);
        TestIF_File.iContinuousPassBin         =ReadIniData(szDir, "Alarm", "Continuous Pass Bin",          1);
        TestIF_File.iContinuousPassBinCount    =ReadIniData(szDir, "Alarm", "Continuous Pass Bin Count",    1000);
        TestIF_File.bContinuousPass_RT         =ReadIniData(szDir, "Alarm", "Continuous Pass RT",           false);
        TestIF_File.iContinuousPassBin_RT      =ReadIniData(szDir, "Alarm", "Continuous Pass RT Bin",       1);
        TestIF_File.iContinuousPassBinCount_RT =ReadIniData(szDir, "Alarm", "Continuous Pass Bin Count RT", 1000);

        //Continuous Pass by Socket - Steven 20110915
        TestIF_File.bContinuousPassBySocket                 =ReadIniData(szDir, "Alarm", "Continuous Pass By Socket",               false);
        TestIF_File.iContinuousPassBinCountBySocket         =ReadIniData(szDir, "Alarm", "Continuous Pass Bin Count By Socket",     1000);
        TestIF_File.bContinuousPassBySocket_RT              =ReadIniData(szDir, "Alarm", "Continuous Pass RT By Socket",            false);
        TestIF_File.iContinuousPassBinCountBySocket_RT      =ReadIniData(szDir, "Alarm", "Continuous Pass Bin Count RT By Socket",  1000);

        //Continuous Loader
        TestIF_File.bContinuousLoader                       =ReadIniData(szDir, "Alarm", "Continuous Loader",           false);
        TestIF_File.iContinuousLoaderCount                  =ReadIniData(szDir, "Alarm", "Continuous Loader Count",     1000);
        TestIF_File.bContinuousLoader_RT                    =ReadIniData(szDir, "Alarm", "Continuous Loader RT",        false);
        TestIF_File.iContinuousLoaderCount_RT               =ReadIniData(szDir, "Alarm", "Continuous Loader Count RT",  100);

        //Contunous Contact Count
        TestIF_File.bContinuousContact                      =ReadIniData(szDir, "Alarm", "Continuous Contact",          false);
        TestIF_File.iContinuousContactCount                 =ReadIniData(szDir, "Alarm", "Continuous Contact Count",    1000);
        TestIF_File.bContinuousContact_RT                   =ReadIniData(szDir, "Alarm", "Continuous Contact RT",       false);
        TestIF_File.iContinuousContactCount_RT              =ReadIniData(szDir, "Alarm", "Continuous Contact Count RT", 1000);

        if(IniConfig.bD56YieldPiggyBackEnable)                                  //強制Enable Yield 裡面piggyback 功能
        {
            TestIF_File.bContinuousContact           =true;
            TestIF_File.bContinuousContact_RT        =true;
            TestIF_File.bContinuousPassBySocket      =true;
            TestIF_File.bContinuousPassBySocket_RT   =true;
            TestIF_File.bContinuousLoader            =true;
            TestIF_File.bContinuousLoader_RT         =true;
            TestIF_File.bContinuousPass              =true;
            TestIF_File.bContinuousPass_RT           =true;
        }
    }

    if(CosFunction.bAdaptiveYield)                                              //Sam 20230914 : 自適應性良率監控
    {
        TestIF_File.bAdaptiveLowYield               =ReadIniData(szDir, "AdaptiveYield", "Low Yield Alarm",        false);
        TestIF_File.iAdaptiveContsLowerAlarmNor     =ReadIniData(szDir, "AdaptiveYield", "Consecutive Lower Alarm",     3); //Sam 20240726 : AI Clean
        TestIF_File.iAdaptiveContsLowerAlarmMin     =ReadIniData(szDir, "AdaptiveYield", "Consecutive Lower Alarm Min", 3);
        TestIF_File.iAdaptiveYieldMax               =ReadIniData(szDir, "AdaptiveYield", "Yield Max",                   3);
        TestIF_File.iAdaptiveYieldMin               =ReadIniData(szDir, "AdaptiveYield", "Yield Min",                   1);

        TestIF_File.bAdaptiveLowYield_RT            =ReadIniData(szDir, "AdaptiveYield", "Low Yield Alarm RT",        false);
        TestIF_File.iAdaptiveContsLowerAlarmNor_RT  =ReadIniData(szDir, "AdaptiveYield", "Consecutive Lower Alarm RT",  3); //Sam 20240726 : AI Clean
        TestIF_File.iAdaptiveContsLowerAlarmMin_RT  =ReadIniData(szDir, "AdaptiveYield", "Consecutive Lower Alarm Min RT",  3);
        TestIF_File.iAdaptiveYieldMax_RT            =ReadIniData(szDir, "AdaptiveYield", "Yield Max RT",                3);
        TestIF_File.iAdaptiveYieldMin_RT            =ReadIniData(szDir, "AdaptiveYield", "Yield Min RT",                1);
    }

    if(CosFunction.bByBinAlarmFromYieldForm)                                    //jou 20180113 (Steven) : By Site By Bin Percent Compare From Yield form
    {
        AnsiString asSpecBinBySiteCompare;
        for(int i=0; i<iTestBinCount; i++)
        {
            asSpecBinBySiteCompare.printf("bSpecBinBySiteCompareEnable%02d_FT",i);
            TestIF_File.bSpecBinBySiteCompareEnable[FT][i]=ReadIniData(szDir, "Alarm", asSpecBinBySiteCompare,          false);
            asSpecBinBySiteCompare.printf("bSpecBinBySiteCompareEnable%02d_RT",i);
            TestIF_File.bSpecBinBySiteCompareEnable[RT][i]=ReadIniData(szDir, "Alarm", asSpecBinBySiteCompare,          false);

            asSpecBinBySiteCompare.printf("dSpecBinBySiteComparePercent%02d_FT",i);
            TestIF_File.dSpecBinBySiteComparePercent[FT][i]=ReadIniData(szDir, "Alarm", asSpecBinBySiteCompare,          0.0);
            asSpecBinBySiteCompare.printf("dSpecBinBySiteComparePercent%02d_RT",i);
            TestIF_File.dSpecBinBySiteComparePercent[RT][i]=ReadIniData(szDir, "Alarm", asSpecBinBySiteCompare,          0.0);

            asSpecBinBySiteCompare.printf("bSpecBinByArmPerSiteCompareEnable%02d_FT",i);
            TestIF_File.bSpecBinByArmPerSiteCompareEnable[FT][i]=ReadIniData(szDir, "Alarm", asSpecBinBySiteCompare,          false);
            asSpecBinBySiteCompare.printf("bSpecBinByArmPerSiteCompareEnable%02d_RT",i);
            TestIF_File.bSpecBinByArmPerSiteCompareEnable[RT][i]=ReadIniData(szDir, "Alarm", asSpecBinBySiteCompare,          false);

            asSpecBinBySiteCompare.printf("dSpecBinByArmPerSiteComparePercent%02d_FT",i);
            TestIF_File.dSpecBinByArmPerSiteComparePercent[FT][i]=ReadIniData(szDir, "Alarm", asSpecBinBySiteCompare,          0.0);
            asSpecBinBySiteCompare.printf("dSpecBinByArmPerSiteComparePercent%02d_RT",i);
            TestIF_File.dSpecBinByArmPerSiteComparePercent[RT][i]=ReadIniData(szDir, "Alarm", asSpecBinBySiteCompare,          0.0);

            asSpecBinBySiteCompare.printf("bByBinFailureEnable%02d_FT",i);
            TestIF_File.bByBinFailureEnable[FT][i]=ReadIniData(szDir, "Alarm", asSpecBinBySiteCompare,          false);
            asSpecBinBySiteCompare.printf("bByBinFailureEnable%02d_RT",i);
            TestIF_File.bByBinFailureEnable[RT][i]=ReadIniData(szDir, "Alarm", asSpecBinBySiteCompare,          false);

            asSpecBinBySiteCompare.printf("dByBinFailurePercent%02d_FT",i);
            TestIF_File.dByBinFailurePercent[FT][i]=ReadIniData(szDir, "Alarm", asSpecBinBySiteCompare,          0.0);
            asSpecBinBySiteCompare.printf("dByBinFailurePercent%02d_RT",i);
            TestIF_File.dByBinFailurePercent[RT][i]=ReadIniData(szDir, "Alarm", asSpecBinBySiteCompare,          0.0);
        }
        TestIF_File.iSpecBinBySiteCompareIgnore[FT]=ReadIniData(szDir, "Alarm", "iSpecBinBySiteCompareIgnore_FT",          200);
        TestIF_File.iSpecBinBySiteCompareIgnore[RT]=ReadIniData(szDir, "Alarm", "iSpecBinBySiteCompareIgnore_RT",          200);
        TestIF_File.iSpecBinByArmPerSiteCompareIgnore[FT]=ReadIniData(szDir, "Alarm", "iSpecBinByArmPerSiteCompareIgnore_FT",200);
        TestIF_File.iSpecBinByArmPerSiteCompareIgnore[RT]=ReadIniData(szDir, "Alarm", "iSpecBinByArmPerSiteCompareIgnore_RT",200);
        TestIF_File.iByBinFailureIgnore[FT]=ReadIniData(szDir, "Alarm", "iByBinFailureIgnore_FT",200);
        TestIF_File.iByBinFailureIgnore[RT]=ReadIniData(szDir, "Alarm", "iByBinFailureIgnore_RT",200);
    }

    if(CosFunction.bCreateManualEOCAP)                                          //jou 20221104 : VTest CreateManualEOCAP function;
    {
        TestIF_File.bCreateManualEOCAP=ReadIniData(szDir, "Alarm", "bCreateManualEOCAP",          false);
    }

    DoIniDataToForm();
    fSortCT->pnlYield->Visible=TestIF_File.bLowYieldAlarmByBin;                 //Steven 20141125
    fSortCT->pnlYieldART->Visible=TestIF_File.bLowYieldAlarmByBin;              //kevin 20150615
}
//---------------------------------------------------------------------------
void TfYieldMonitoring::DoFormToData()
{
//Low Yield Alarm(%)
    TestIF_File.bFailAlarmLowYield                  =cbLowYield_FT->Checked;
    TestIF_File.iLowYieldLimit                      =CheckRange(atoi(edLowYield_FT->Text.c_str()), iMinYield, iMaxYield);
    TestIF_File.dLowYieldLimit                      =CheckRange(atof(edLowYield_FT->Text.c_str()), dMinYield, dMaxYield);
    TestIF_File.iLowYieldCount                      =CheckRange(atoi(edLowYieldIg_FT->Text.c_str()), iMinCount, iMaxCount);
    TestIF_File.bLowYieldAlarmByBin                 =cbYieldAlarmByBin->Checked;                                                //Steven 20140828 : By Bin Yield Monitor

    TestIF_File.bFailAlarmLowYield_RT               =cbLowYield_RT->Checked;
    TestIF_File.iLowYieldLimit_RT                   =CheckRange(atoi(edLowYield_RT->Text.c_str()), iMinYield, iMaxYield);
    TestIF_File.dLowYieldLimit_RT                   =CheckRange(atof(edLowYield_RT->Text.c_str()), dMinYield, dMaxYield);       //JerryYang 20160530 LowYieldLimit要能設定到小數點
    TestIF_File.iLowYieldCount_RT                   =CheckRange(atoi(edLowYieldIg_RT->Text.c_str()), iMinCount, iMaxCount);

    if(CosFunction.bLowYieldAutoSiteOff)                                        //Steven 20170905 (wei) : Low Yield Auto Site Off for Ambient
    {
        TestIF_File.bLowYieldAutoSiteOff            =chkAutoSiteOff->Checked;
        TestIF_File.bLowYieldAutoSiteOffByArmSite   =chkAutoSiteOffByArmBySite->Checked;    //Steven 20230223 : by arm by site, auto site off
        TestIF_File.bLowYieldAutoSiteOffByContiFail =chkAutoSiteOffByContiFail->Checked;    //Steven 20200420 : Continue fail, auto site off
        TestIF_File.bLowYieldAutoSiteOffArmContiFail=chkAutoSiteOffByArmContiFail->Checked; //Steven 20220818 : By Arm Continue fail, auto site off
        TestIF_File.bLowYieldAutoSiteOffByPicker    =chkchkAutoSiteOffByPicker->Checked;    //Steven 20230223 : 根據Index吸嘴比較良率
        TestIF_File.iAlarmWhenSiteOnCountLess       =CheckRange(atoi(edtSiteYieldCmp1SiteCnt_FT->Text.c_str()), 2, 32);
        TestIF_File.iCloseSiteOnHPDontTest          =rgCloseSiteOnHPAction->ItemIndex;      //JerryYang 20180726 (wei) 關site的位置有IC不測試送error bin
        TestIF_File.iCloseSiteBin                   =cbbClosedSiteBin->ItemIndex;                                   //Steven 20240409 : 關site的位置有IC不測試送指定 bin
        TestIF_File.bLowYieldAutoSiteOffAlarm       =chkAutoSiteOffAlarm->Checked;                                  //Sam 20221207 : LowYieldAutoSiteOff 新增 Alarm 幾次後再來關 Site
        TestIF_File.iLowYieldAutoSiteOffAlarm       =CheckRange(atoi(edtAutoSiteOffAlarm->Text.c_str()), 2, 10);    //Sam 20221207 : LowYieldAutoSiteOff 新增 Alarm 幾次後再來關 Site
    }

//Site Yield Alarm(%)
    TestIF_File.bFailAlarmSiteYieldDifferent        =cbSiteYieldDifferent_FT->Checked;
    TestIF_File.iFailAlarmSiteYield                 =CheckRange(atoi(edSiteYieldDifferent_FT->Text.c_str()), iMinYield, iMaxYield);
    TestIF_File.dFailAlarmSiteYield                 =CheckRange(atof(edSiteYieldDifferent_FT->Text.c_str()), dMinYield, dMaxYield);
    TestIF_File.iFailAlarmSiteYieldDifferentCount   =CheckRange(atoi(edSiteYieldDifferentIg_FT->Text.c_str()), iMinCount, iMaxCount);

    TestIF_File.bFailAlarmSiteYieldDifferent_RT     =cbSiteYieldDifferent_RT->Checked;
    TestIF_File.iFailAlarmSiteYield_RT              =CheckRange(atoi(edSiteYieldDifferent_RT->Text.c_str()), iMinYield, iMaxYield);
    TestIF_File.dFailAlarmSiteYield_RT              =CheckRange(atof(edSiteYieldDifferent_RT->Text.c_str()), dMinYield, dMaxYield);
    TestIF_File.iFailAlarmSiteYieldDifferentCount_RT=CheckRange(atoi(edSiteYieldDifferentIg_RT->Text.c_str()), iMinCount, iMaxCount);

//Site Yield Alarm(%)
    TestIF_File.bFailAlarmSiteYieldCmp              =cbSiteYieldCmp_FT->Checked;

    TestIF_File.iFailAlarmSiteYieldCmp              =CheckRange(atoi(edSiteYieldCmp_FT->Text.c_str()), iMinYield, iMaxYield);
    TestIF_File.dFailAlarmSiteYieldCmp              =CheckRange(atof(edSiteYieldCmp_FT->Text.c_str()), dMinYield, dMaxYield);
    TestIF_File.iFailAlarmSiteYieldCmpCount         =CheckRange(atoi(edSiteYieldCmpIg_FT->Text.c_str()), iMinCount, iMaxCount);

    TestIF_File.bFailAlarmSiteYieldCmp_RT           =cbSiteYieldCmp_RT->Checked;
    TestIF_File.iFailAlarmSiteYieldCmp_RT           =CheckRange(atoi(edSiteYieldCmp_RT->Text.c_str()), iMinYield, iMaxYield);
    TestIF_File.dFailAlarmSiteYieldCmp_RT           =CheckRange(atof(edSiteYieldCmp_RT->Text.c_str()), dMinYield, dMaxYield);
    TestIF_File.iFailAlarmSiteYieldCmpCount_RT      =CheckRange(atoi(edSiteYieldCmpIg_RT->Text.c_str()), iMinCount, iMaxCount);

//Alarm Action
    TestIF_File.iCountAlarmAction                   =CheckRange(rgPiggyBack_FT->ItemIndex, 0, 1);
    TestIF_File.iCountAlarmAction_RT                =CheckRange(rgPiggyBack_RT->ItemIndex, 0, 1);

//Low Yield Alarm(%)                                                            //wei 20151116 Low Yield By Total
    TestIF_File.bFailAlarmLowYieldByTotal           =cbLowYieldByTotal_FT->Checked;
    TestIF_File.iLowYieldLimitByTotal               =CheckRange(atoi(edLowYieldByTotal_FT->Text.c_str()), iMinYield, iMaxYield);
    TestIF_File.dLowYieldLimitByTotal               =CheckRange(atof(edLowYieldByTotal_FT->Text.c_str()), dMinYield, dMaxYield);
    TestIF_File.iLowYieldCountByTotal               =CheckRange(atoi(edLowYieldByTotalIg_FT->Text.c_str()), iMinCount, iMaxCount);

    TestIF_File.bFailAlarmLowYieldByTotal_RT        =cbLowYieldByTotal_RT->Checked;
    TestIF_File.iLowYieldLimitByTotal_RT            =CheckRange(atoi(edLowYieldByTotal_RT->Text.c_str()), iMinYield, iMaxYield);
    TestIF_File.dLowYieldLimitByTotal_RT            =CheckRange(atof(edLowYieldByTotal_RT->Text.c_str()), dMinYield, dMaxYield);
    TestIF_File.iLowYieldCountByTotal_RT            =CheckRange(atoi(edLowYieldByTotalIg_RT->Text.c_str()), iMinCount, iMaxCount);

//By Picker Compare Yield   [每個吸嘴的良率互比]                                //Steven 20230223 : 根據Index吸嘴比較良率
    TestIF_File.bLowYieldByPicker                   =chkLowYieldByPicker_FT->Checked;
    TestIF_File.bLowYieldByPicker_RT                =chkLowYieldByPicker_RT->Checked;
    TestIF_File.iLowYieldCountByPicker              =CheckRange(atoi(edtLowYieldByPickerIg_FT->Text.c_str()), iMinCount, iMaxCount);
    TestIF_File.dLowYieldByPicker                   =CheckRange(atof(edtLowYieldByPicker_FT->Text.c_str()), dMinYield, dMaxYield);
    TestIF_File.iLowYieldCountByPicker_RT           =CheckRange(atoi(edtLowYieldByPickerIg_RT->Text.c_str()), iMinCount, iMaxCount);
    TestIF_File.dLowYieldByPicker_RT                =CheckRange(atof(edtLowYieldByPicker_RT->Text.c_str()), dMinYield, dMaxYield);

//Interval Low Yield Alarm(%)                                                   //wei 20180606 Interval Low Yield By Site
    if(atoi(edIntervalLowYieldBySiteIg_FT->Text.c_str())!=TestIF_File.iIntervalLowYieldCountBySite ||
       atoi(edIntervalLowYieldBySiteIg_RT->Text.c_str())!=TestIF_File.iIntervalLowYieldCountBySite_RT )
    {
        for(int i=0; i<2; i++)
        {
            iYieldSiteCount[i]=0;
            bYieldSiteBin[i]=false;
        }
        memset(bIntervalYieldIsPass, 0, sizeof(bIntervalYieldIsPass));          // 20180629
    }

    if(atoi(edIntervalLowYieldByTotalIg_FT->Text.c_str())!=TestIF_File.iIntervalLowYieldCountByTotal ||
       atoi(edIntervalLowYieldByTotalIg_RT->Text.c_str())!=TestIF_File.iIntervalLowYieldCountByTotal_RT )
    {
        iYieldTotalCount=0;                                                     //wei 20180718 Interval Low Yield By Total
        bYieldTotalBin=false;
        memset(bYieldTotalBinIsPass, 0, sizeof(bYieldTotalBinIsPass));          // 20180629
    }

    TestIF_File.bFailAlarmIntervalLowYieldBySite           =cbIntervalLowYieldBySite_FT->Checked;
    TestIF_File.iIntervalLowYieldLimitBySite               =CheckRange(atoi(edIntervalLowYieldBySite_FT->Text.c_str()), iMinYield, iMaxYield);
    TestIF_File.dIntervalLowYieldLimitBySite               =CheckRange(atof(edIntervalLowYieldBySite_FT->Text.c_str()), dMinYield, dMaxYield);
    TestIF_File.iIntervalLowYieldCountBySite               =CheckRange(atoi(edIntervalLowYieldBySiteIg_FT->Text.c_str()), 1, 300);

    TestIF_File.bFailAlarmIntervalLowYieldBySite_RT        =cbIntervalLowYieldBySite_RT->Checked;
    TestIF_File.iIntervalLowYieldLimitBySite_RT            =CheckRange(atoi(edIntervalLowYieldBySite_RT->Text.c_str()), iMinYield, iMaxYield);
    TestIF_File.dIntervalLowYieldLimitBySite_RT            =CheckRange(atof(edIntervalLowYieldBySite_RT->Text.c_str()), dMinYield, dMaxYield);
    TestIF_File.iIntervalLowYieldCountBySite_RT            =CheckRange(atoi(edIntervalLowYieldBySiteIg_RT->Text.c_str()), 1, 300);

    TestIF_File.bFailAlarmIntervalLowYieldByTotal          =cbIntervalLowYieldByTotal_FT->Checked;
    TestIF_File.iIntervalLowYieldLimitByTotal              =CheckRange(atoi(edIntervalLowYieldByTotal_FT->Text.c_str()), iMinYield, iMaxYield);
    TestIF_File.dIntervalLowYieldLimitByTotal              =CheckRange(atof(edIntervalLowYieldByTotal_FT->Text.c_str()), dMinYield, dMaxYield);
    TestIF_File.iIntervalLowYieldCountByTotal              =CheckRange(atoi(edIntervalLowYieldByTotalIg_FT->Text.c_str()), 1, 300);

    TestIF_File.bFailAlarmIntervalLowYieldByTotal_RT       =cbIntervalLowYieldByTotal_RT->Checked;
    TestIF_File.iIntervalLowYieldLimitByTotal_RT           =CheckRange(atoi(edIntervalLowYieldByTotal_RT->Text.c_str()), iMinYield, iMaxYield);
    TestIF_File.dIntervalLowYieldLimitByTotal_RT           =CheckRange(atof(edIntervalLowYieldByTotal_RT->Text.c_str()), dMinYield, dMaxYield);
    TestIF_File.iIntervalLowYieldCountByTotal_RT           =CheckRange(atoi(edIntervalLowYieldByTotalIg_RT->Text.c_str()), 1, 300);

//Continuous Pass
    if(IniConfig.bD56YieldPiggyBackEnable)
    {
        TestIF_File.bContinuousLoader               =true;
        TestIF_File.bContinuousLoader_RT            =true;
        TestIF_File.bContinuousPassBySocket         =true;
        TestIF_File.bContinuousPassBySocket_RT      =true;
        TestIF_File.bContinuousContact              =true;
        TestIF_File.bContinuousContact_RT           =true;
        TestIF_File.bContinuousPass_RT              =true;
        TestIF_File.bContinuousPass                 =true;
    }
    else
    {
        TestIF_File.bContinuousLoader               =cbContinuousLoad_FT->Checked;
        TestIF_File.bContinuousLoader_RT            =cbContinuousLoad_RT->Checked;
        TestIF_File.bContinuousPassBySocket         =cbContinuPassSkt_FT->Checked;
        TestIF_File.bContinuousPassBySocket_RT      =cbContinuPassSkt_RT->Checked;
        TestIF_File.bContinuousContact              =cbContactCountFT->Checked;
        TestIF_File.bContinuousContact_RT           =cbContactCountRT->Checked;
        TestIF_File.bContinuousPass_RT              =cbContinuousPass_RT->Checked;
        TestIF_File.bContinuousPass                 =cbContinuousPass_FT->Checked;
    }

    if(CosFunction.bPiggyBackForASE==true)                                      //Steven 20131101 : 高雄ASE不要Continual Pass Bin(Total )跟 Continual Loader兩種
    {
        TestIF_File.bContinuousPass                 =false;
        TestIF_File.bContinuousPass_RT              =false;
        TestIF_File.bContinuousLoader               =false;
        TestIF_File.bContinuousLoader_RT            =false;
    }

    TestIF_File.iContinuousPassBin                  =CheckRange(cobContinuousPass_FT->ItemIndex+1, 1, iTestBinCount);     //Steven 20121112 : RS232支援32Bin 15 --> iTestBinCount
    TestIF_File.iContinuousPassBinCount             =CheckRange(atoi(edContinuousPass_FT->Text.c_str()), iMinCount, iMaxCount);
    TestIF_File.iContinuousPassBin_RT               =CheckRange(cobContinuousPass_RT->ItemIndex+1, 1, iTestBinCount);     //Steven 20121112 : RS232支援32Bin 15 --> iTestBinCount
    TestIF_File.iContinuousPassBinCount_RT          =CheckRange(atoi(edContinuousPass_RT->Text.c_str()), iMinCount, iMaxCount);
    TestIF_File.iContinuousPassBinCountBySocket     =CheckRange(atoi(edContinuPassSkt_FT->Text.c_str()), iMinCount, iMaxCount);
    TestIF_File.iContinuousPassBinCountBySocket_RT  =CheckRange(atoi(edContinuPassSkt_RT->Text.c_str()), iMinCount, iMaxCount);
    TestIF_File.iContinuousContactCount             =CheckRange(atoi(edContactCountFT->Text.c_str()), iMinCount, iMaxCount);
    TestIF_File.iContinuousContactCount_RT          =CheckRange(atoi(edContactCountRT->Text.c_str()), iMinCount, iMaxCount);
    TestIF_File.iContinuousLoaderCount              =CheckRange(atoi(edContinuousLoad_FT->Text.c_str()), iMinCount, iMaxCount);
    TestIF_File.iContinuousLoaderCount_RT           =CheckRange(atoi(edContinuousLoad_RT->Text.c_str()), iMinCount, iMaxCount);

//Consecutive Failure Alarm ( Socket )
    TestIF_File.bContsFailBySocket                  =rbContsFailBySocket_FTOn->Checked;
    TestIF_File.iContsFailSocketAlarmCT             =CheckRange(atoi(edContsFailSocketAlarmCT_FT->Text.c_str()), iMinYield, iMaxYield);
    TestIF_File.bCountSpcBinContinuously_FT         =chkSpcBinCntAsContinue_FT->Checked;      //Steven 20230529 : Spc Bin Couont改成連續錯誤
    TestIF_File.bCountSpcBinContinuously_RT         =chkSpcBinCntAsContinue_RT->Checked;      //Steven 20230529 : Spc Bin Couont改成連續錯誤

    TestIF_File.bContsFailBySocket_RT               =rbContsFailBySocket_RTOn->Checked;
    TestIF_File.iContsFailSocketAlarmCT_RT          =CheckRange(atoi(edContsFailSocketAlarmCT_RT->Text.c_str()), iMinYield, iMaxYield);

//Consecutive Failure Alarm ( Head)
    TestIF_File.bContsFailByHead                    =rbContsFailByHead_FTOn->Checked;
    TestIF_File.iContsFailHeadAlarmCT               =CheckRange(atoi(edContsFailHeadAlarmCT_FT->Text.c_str()), iMinYield, iMaxYield);

    TestIF_File.bContsFailByHead_RT                 =rbContsFailByHead_RTOn->Checked;
    TestIF_File.iContsFailHeadAlarmCT_RT            =CheckRange(atoi(edContsFailHeadAlarmCT_RT->Text.c_str()), iMinYield, iMaxYield);

//Consecutive Failure Ignore                                                    //wei 20160115 銅鑼前幾顆不計算ContsFail
    TestIF_File.bContsFailIgnore                    =cbContsFailIgnore_FT->Checked;
    TestIF_File.iContsFailIgnore                    =CheckRange(atoi(edContsFailIgnore_FT->Text.c_str()), iMinCount, iMaxCount);

    TestIF_File.bContsFailIgnore_RT                 =cbContsFailIgnore_RT->Checked;
    TestIF_File.iContsFailIgnore_RT                 =CheckRange(atoi(edContsFailIgnore_RT->Text.c_str()), iMinCount, iMaxCount);

    //wei 20141201 Low Yield Auto Clean(%) start
    TestIF_File.bFailAlarmLowYield_AutoClean        =rgAutoCleanYieldOnOff->ItemIndex;
    TestIF_File.iLowYieldLimit_AutoClean            =CheckRange(atoi(edAutoCleanLowYieldLimit->Text.c_str()), 1, 50);
    TestIF_File.iLowYieldCount_AutoClean            =CheckRange(atoi(edAutoCleanLowYieldCount->Text.c_str()), iMinCount, iMaxCount);
    //wei 20141201 Low Yield Auto Clean(%) end

    TestIF_File.iAutoCloseSiteWhenRT                =rgAutoSiteOffWhenRT->ItemIndex;                                        //Steven 20200225 : 切到RT的時候,要關閉Socket
    TestIF_File.dAutoCloseSiteYieldWhenRT           =CheckRange(atof(edtAutoSiteOffWhenRT->Text.c_str()), 50.00, 0.01);    //Steven 20200225 : 切到RT的時候,要關閉Socket
    TestIF_File.iAllSiteOnAtInitialStart            =rgAutoSiteOn->ItemIndex;                                               //Steven 20230814 : Initial Start的時候要全開Site
    TestIF_File.iAutoSiteOffByGPIB                  =rgAutoSiteOnByGPIB->ItemIndex;                                         //JimmyChiu 20250715 : Auto site on/off by GPIB

    if(IniConfig.bE53LowYieldAutoClean==false)                                  //kevin 20160802
        TestIF_File.bFailAlarmLowYield_AutoClean=false;

    //wei 20160314
    TestIF_File.bAlarm4ContinueType_Enable          =cbAlarm4ContinueType->Checked;
    TestIF_File.iAlarm4ContinueType_IntervalCount   =CheckRange(atoi(edAlarm4IntervalCount->Text.c_str()), iMinCount, iMaxCount);
    TestIF_File.iAlarm4ContinueType_ContinueCount   =atoi(edAlarm4ContinueCount->Text.c_str());

    TestIF_File.bAlarm4EnableIntervalYield          =cb_Alarm4EnableIntervalYield->Checked ;
    TestIF_File.iAlarm4IntervalYieldIntervalCount   =CheckRange(atoi(ed_Alarm4IntervalYieldIntervalCount->Text.c_str()), iMinCount, iMaxCount);
    TestIF_File.iAlarm4IntervalYieldContinueCount   =atoi(ed_Alarm4IntervalYieldContinueCount->Text.c_str());
    TestIF_File.iAlarm4IntervalYieldYield           =CheckRange(atoi(ed_Alarm4IntervalYieldYield->Text.c_str()), iMinYield, iMaxYield);

    TestIF_File.bSiteToSiteYieldCmp                 =cb_SiteToSiteYieldEnable->Checked;
    TestIF_File.iSiteToSiteYieldCmp                 =CheckRange(atoi(ed_SiteToSiteYield->Text.c_str()), iMinYield, iMaxYield);
    TestIF_File.iSiteToSiteYieldCmpCount            =CheckRange(atoi(ed_SiteToSiteYieldCount->Text.c_str()), iMinCount, iMaxCount);

    TestIF_File.bHeadToHeadYieldCmp                 =cb_HeadToHeadYieldEnable->Checked;
    TestIF_File.iHeadToHeadYieldCmp                 =CheckRange(atoi(ed_HeadToHeadYield->Text.c_str()), iMinYield, iMaxYield);
    TestIF_File.iHeadToHeadYieldCmpCount            =CheckRange(atoi(ed_HeadToHeadYieldCount->Text.c_str()), iMinCount, iMaxCount);

    TestIF_File.bSiteYieldOverAlert                 =cb_SiteYieldOverAlert->Checked;
    TestIF_File.iSiteYieldOverAlert                 =CheckRange(atoi(ed_SiteYieldOverAlert->Text.c_str()), iMinYield, iMaxYield);
    TestIF_File.iSiteYieldOverAlertCount            =CheckRange(atoi(ed_SiteYieldOverAlertCount->Text.c_str()), iMinCount, iMaxCount);

    TestIF_File.bAllSiteFail                        =cbAllSiteFail->Checked;    //kevin 20170825 (wei) 整支ARM Fail bin
    TestIF_File.iAllSiteFailCount                   =CheckRange(atoi(edAllSiteFailCount->Text.c_str()), 0, 500);//kevin 20180720 (wei) all site fail count
    TestIF_File.iAllSiteFailCountRT                 =CheckRange(atoi(edAllSiteFailCountRT->Text.c_str()), 0, 500);
    TestIF_File.bAllSiteFail_RT                     =cbAllSiteFail_RT->Checked; //Isaac 20180305 (Steven) ATK要求，只有FT要alarm，FT/RT分開

    if(CUSTOMER_CODE==CC_SIGURD_HUKOU)                                          //KaiChen 20200603 ：矽格-湖口，Yield 頁面 AllSiteFail 不使用
    {
        TestIF_File.bAllSiteFail        =false;
        TestIF_File.bAllSiteFail_RT     =false;
    }
    //kevin 20150529 AutoRetest Start
    if(CUSTOMER_CODE==CC_ASE_KaohSiung)
        iAutoRetestLimit= MyYieldPanel[0]->ReTestLimit->ItemIndex+1;            //kevin 20150704  //kevin 20150601 做AUTO RETEST最大次數

    if(CosFunction.bAdaptiveYield)                                              //Sam 20230914 : 自適應性良率監控
    {
        TestIF_File.bAdaptiveLowYield           =cbAdaptiveLowYield_FT->Checked;
        TestIF_File.iAdaptiveContsLowerAlarmNor     =CheckRange(atoi(edlContsLowerAlarmNor_FT->Text.c_str()), iMinYield, iMaxYield); //Sam 20240726 : AI Clean
        TestIF_File.iAdaptiveContsLowerAlarmMin     =CheckRange(atoi(edlContsLowerAlarmMin_FT->Text.c_str()), iMinYield, iMaxYield);
        TestIF_File.iAdaptiveYieldMax               =CheckRange(atoi(edAdaptiveYieldMax_FT->Text.c_str()), 1, 20);
        TestIF_File.iAdaptiveYieldMin               =CheckRange(atoi(edAdaptiveYieldMin_FT->Text.c_str()), 1, 20);

        TestIF_File.bAdaptiveLowYield_RT        =cbAdaptiveLowYield_RT->Checked ;
        TestIF_File.iAdaptiveContsLowerAlarmNor_RT  =CheckRange(atoi(edlContsLowerAlarmNor_RT->Text.c_str()), iMinYield, iMaxYield); //Sam 20240726 : AI Clean
        TestIF_File.iAdaptiveContsLowerAlarmMin_RT  =CheckRange(atoi(edlContsLowerAlarmMin_RT->Text.c_str()), iMinYield, iMaxYield);
        TestIF_File.iAdaptiveYieldMax_RT            =CheckRange(atoi(edAdaptiveYieldMax_RT->Text.c_str()), 1, 20);
        TestIF_File.iAdaptiveYieldMin_RT            =CheckRange(atoi(edAdaptiveYieldMin_RT->Text.c_str()), 1, 20);
    }

    if(CosFunction.bByBinAlarmFromYieldForm)                                    //jou 20180113 (Steven) : By Site By Bin Percent Compare From Yield form
    {
        for(int i=0; i<TEST_MAX_BIN; i++)
        {
            TestIF_File.bSpecBinBySiteCompareEnable[FT][i]          =(i<iTestBinCount)?cbByBinSiteGapCat_FT[i]->Checked:false;
            TestIF_File.bSpecBinBySiteCompareEnable[RT][i]          =(i<iTestBinCount)?cbByBinSiteGapCat_RT[i]->Checked:false;
            TestIF_File.dSpecBinBySiteComparePercent[FT][i]         =CheckRange(atof(edByBinSiteGapCat_FT[i]->Text.c_str()), dMinYield, dMaxYield);
            TestIF_File.dSpecBinBySiteComparePercent[RT][i]         =CheckRange(atof(edByBinSiteGapCat_RT[i]->Text.c_str()), dMinYield, dMaxYield);
            TestIF_File.bSpecBinByArmPerSiteCompareEnable[FT][i]    =(i<iTestBinCount)?cbByArmSiteGapCat_FT[i]->Checked:false;
            TestIF_File.bSpecBinByArmPerSiteCompareEnable[RT][i]    =(i<iTestBinCount)?cbByArmSiteGapCat_RT[i]->Checked:false;
            TestIF_File.dSpecBinByArmPerSiteComparePercent[FT][i]   =CheckRange(atof(edByArmSiteGapCat_FT[i]->Text.c_str()), dMinYield, dMaxYield);
            TestIF_File.dSpecBinByArmPerSiteComparePercent[RT][i]   =CheckRange(atof(edByArmSiteGapCat_RT[i]->Text.c_str()), dMinYield, dMaxYield);
            TestIF_File.bByBinFailureEnable[FT][i]                  =(i<iTestBinCount)?cbByBinFailureCat_FT[i]->Checked:false;
            TestIF_File.bByBinFailureEnable[RT][i]                  =(i<iTestBinCount)?cbByBinFailureCat_RT[i]->Checked:false;
            TestIF_File.dByBinFailurePercent[FT][i]                 =CheckRange(atof(edByBinFailureCat_FT[i]->Text.c_str()), dMinYield, dMaxYield);
            TestIF_File.dByBinFailurePercent[RT][i]                 =CheckRange(atof(edByBinFailureCat_RT[i]->Text.c_str()), dMinYield, dMaxYield);
        }
        TestIF_File.iSpecBinBySiteCompareIgnore[FT]=CheckRange(atoi(edByBinSiteGapCatCT_FT->Text.c_str()), iMinCount, iMaxCount);
        TestIF_File.iSpecBinBySiteCompareIgnore[RT]=CheckRange(atoi(edByBinSiteGapCatCT_RT->Text.c_str()), iMinCount, iMaxCount);
        TestIF_File.iSpecBinByArmPerSiteCompareIgnore[FT]=CheckRange(atoi(edByArmSiteGapCatCT_FT->Text.c_str()), iMinCount, iMaxCount);
        TestIF_File.iSpecBinByArmPerSiteCompareIgnore[RT]=CheckRange(atoi(edByArmSiteGapCatCT_RT->Text.c_str()), iMinCount, iMaxCount);
        TestIF_File.iByBinFailureIgnore[FT]=CheckRange(atoi(edByBinFailureCatCT_FT->Text.c_str()), iMinCount, iMaxCount);
        TestIF_File.iByBinFailureIgnore[RT]=CheckRange(atoi(edByBinFailureCatCT_RT->Text.c_str()), iMinCount, iMaxCount);
    }

    if(CosFunction.bSpecailLowYeild)                                            //Sam 20210505 : PTI 要求的兩段 Low Yeild
    {
        TestIF_File.bFailAlarmLowYieldSpecial   =cbLowYield_Special->Checked;
        TestIF_File.iLowYieldLimitSpecial       =CheckRange(atoi(edLowYield_Special->Text.c_str()), iMinYield, iMaxYield);
        TestIF_File.dLowYieldLimitSpecial       =CheckRange(atof(edLowYield_Special->Text.c_str()), dMinYield, dMaxYield);
        TestIF_File.iLowYieldCountSpecial1      =CheckRange(atoi(edLowYieldIg_Special1->Text.c_str()), iMinCount, iMaxCount);
        TestIF_File.iLowYieldCountSpecial2      =CheckRange(atoi(edLowYieldIg_Special2->Text.c_str()), iMinCount, iMaxCount);
    }

    if(CosFunction.bCreateManualEOCAP)                                          //jou 20221104 : VTest CreateManualEOCAP function;
    {
        TestIF_File.bCreateManualEOCAP=chkCreateManualEOCAP->Checked;
    }
}
//---------------------------------------------------------------------------
//kevin 20140105 儲存 檔案另存 jobfile
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::SaveSetupFile(AnsiString szDir, AnsiString S)
{
    MyForceDirectories(szDir, "TfYieldMonitoring::SaveSetupFile");
    szDir+="Tester.Data";

//Low Yield Alarm(%)
    WriteIniData(szDir, "Low Yield Alarm", "Enable",     int(TestIF_File.bFailAlarmLowYield));
    if(CosFunction.bYieldAlarmUseDouble)                                        //JerryYang 20160530 LowYieldLimit要能設定到小數點
    {
        if(CUSTOMER_CODE==CC_QUALCOMM ||
           CUSTOMER_CODE==CC_AMKOR_China)                                       //JerryYang 20181016 (Steven) : Amkor要求yield改為小數點後，存讀檔的部份要與舊版本相容
        {
            WriteIniData(szDir, "Low Yield Alarm", "Limit",    TestIF_File.dLowYieldLimit);
            WriteIniData(szDir, "Low Yield Alarm", "Limit RT", TestIF_File.dLowYieldLimit_RT);
        }
        else
        {
            WriteIniData(szDir, "Low Yield Alarm", "Double Limit",    TestIF_File.dLowYieldLimit);
            WriteIniData(szDir, "Low Yield Alarm", "Double Limit RT", TestIF_File.dLowYieldLimit_RT);
        }
    }
    else                                                                        //JerryYang 20160615 Yield相關alarm設定到整數
    {
        WriteIniData(szDir, "Low Yield Alarm", "Limit",      int(TestIF_File.iLowYieldLimit));
        WriteIniData(szDir, "Low Yield Alarm", "Limit RT",   int(TestIF_File.iLowYieldLimit_RT));
    }
    WriteIniData(szDir, "Low Yield Alarm", "Count",      int(TestIF_File.iLowYieldCount));
    WriteIniData(szDir, "Low Yield Alarm", "Enable By Bin Setting", int(TestIF_File.bLowYieldAlarmByBin));    //Steven 20140828 : By Bin Yield Monitor
    WriteIniData(szDir, "Low Yield Alarm", "Enable RT",  int(TestIF_File.bFailAlarmLowYield_RT));

    WriteIniData(szDir, "Low Yield Alarm", "Count RT",   int(TestIF_File.iLowYieldCount_RT));

//    TestIF_File.iIgnoreIC       =CheckRange(int(TestIF_File.iIgnoreIC),    0, 100000);
//    TestIF_File.iIgnoreIC_RT    =CheckRange(int(TestIF_File.iIgnoreIC_RT), 0, 100000);

    if(CosFunction.bLowYieldAutoSiteOff)                                        //Steven 20170905 (wei) : Low Yield Auto Site Off for Ambient
    {
        WriteIniData(szDir, "Site Yield Alarm", "Alarm When Site On Count Less Than",   TestIF_File.iAlarmWhenSiteOnCountLess);
        WriteIniData(szDir, "Site Yield Alarm", "Low Yield Auto Site Off",              TestIF_File.bLowYieldAutoSiteOff);
        WriteIniData(szDir, "Site Yield Alarm", "bLowYieldAutoSiteOffByContiFail",      TestIF_File.bLowYieldAutoSiteOffByContiFail);   //Steven 20200420 : Continue fail, auto site off
        WriteIniData(szDir, "Site Yield Alarm", "bLowYieldAutoSiteOffArmContiFail",     TestIF_File.bLowYieldAutoSiteOffArmContiFail);  //Steven 20220818 : By Arm Continue fail, auto site off
        WriteIniData(szDir, "Site Yield Alarm", "Low Yield Auto Site Off Alarm",        TestIF_File.bLowYieldAutoSiteOffAlarm);         //Sam 20221207 : LowYieldAutoSiteOff 新增 Alarm 幾次後再來關 Site
        WriteIniData(szDir, "Site Yield Alarm", "Low Yield Auto Site Off Alarm Count",  TestIF_File.iLowYieldAutoSiteOffAlarm);         //Sam 20221207 : LowYieldAutoSiteOff 新增 Alarm 幾次後再來關 Site
        WriteIniData(szDir, "Site Yield Alarm", "By Arm Site Low Yield Auto Site Off ", TestIF_File.bLowYieldAutoSiteOffByArmSite);     //Steven 20230223 : by arm by site, auto site off
        WriteIniData(szDir, "Site Yield Alarm", "By Picker Low Yield Auto Site Off ",   TestIF_File.bLowYieldAutoSiteOffByPicker);      //Steven 20230223 : 根據Index吸嘴比較良率

        if(IniConfig.bI28_OnOffSiteOnTheFly ||
           TestIF_File.bLowYieldAutoSiteOff ||
           TestIF_File.bLowYieldAutoSiteOffByContiFail)                         //Steven 20250604 : 關site的位置有IC不測試送指定 bin
        {
            WriteIniData(szDir, "Site Yield Alarm", "HP Close Site Do Not Test",        TestIF_File.iCloseSiteOnHPDontTest);            //JerryYang 20180726 (wei) 關site的位置有IC不測試送error bin
            WriteIniData(szDir, "Site Yield Alarm", "Bin of Closed Site",               TestIF_File.iCloseSiteBin);                     //Steven 20240409 : 關site的位置有IC不測試送指定 bin
        }
    }

//Site Yield Alarm(%)
    WriteIniData(szDir, "Site Yield Alarm","Site Yield Different",          int(TestIF_File.bFailAlarmSiteYieldDifferent));
    if(CosFunction.bYieldAlarmUseDouble)                                        //JerryYang 20160530 LowYieldLimit要能設定到小數點
    {
        if(CUSTOMER_CODE==CC_QUALCOMM ||
           CUSTOMER_CODE==CC_AMKOR_China)                                       //JerryYang 20181016 (Steven) : Amkor要求yield改為小數點後，存讀檔的部份要與舊版本相容
        {
            WriteIniData(szDir, "Site Yield Alarm","Site Yield",    TestIF_File.dFailAlarmSiteYield);
            WriteIniData(szDir, "Site Yield Alarm","Site Yield RT", TestIF_File.dFailAlarmSiteYield_RT);
        }
        else
        {
            WriteIniData(szDir, "Site Yield Alarm","Double Site Yield",    TestIF_File.dFailAlarmSiteYield);
            WriteIniData(szDir, "Site Yield Alarm","Double Site Yield RT", TestIF_File.dFailAlarmSiteYield_RT);
        }
    }
    else                                                                        //JerryYang 20160615 Yield相關alarm設定到整數
    {
        WriteIniData(szDir, "Site Yield Alarm","Site Yield",                int(TestIF_File.iFailAlarmSiteYield));
        WriteIniData(szDir, "Site Yield Alarm","Site Yield RT",             int(TestIF_File.iFailAlarmSiteYield_RT));
    }
    WriteIniData(szDir, "Site Yield Alarm","Site Yield Different Count",    int(TestIF_File.iFailAlarmSiteYieldDifferentCount));
    WriteIniData(szDir, "Site Yield Alarm","Site Yield Different RT",       int(TestIF_File.bFailAlarmSiteYieldDifferent_RT));
    WriteIniData(szDir, "Site Yield Alarm","Site Yield Different Count RT", int(TestIF_File.iFailAlarmSiteYieldDifferentCount_RT));

//Site Yield Alarm(%)
    WriteIniData(szDir, "Site Yield Alarm","Site Yield Compare",            int(TestIF_File.bFailAlarmSiteYieldCmp));
    if(CosFunction.bYieldAlarmUseDouble)                                        //JerryYang 20160530 LowYieldLimit要能設定到小數點
    {
        if(CUSTOMER_CODE==CC_QUALCOMM ||
           CUSTOMER_CODE==CC_AMKOR_China)                                       //JerryYang 20181016 (Steven) : Amkor要求yield改為小數點後，存讀檔的部份要與舊版本相容
        {
            WriteIniData(szDir, "Site Yield Alarm","Site Yield Cmp",     TestIF_File.dFailAlarmSiteYieldCmp);
            WriteIniData(szDir, "Site Yield Alarm","Site Yield Cmp RT",  TestIF_File.dFailAlarmSiteYieldCmp_RT);
        }
        else
        {
            WriteIniData(szDir, "Site Yield Alarm","Double Site Yield Cmp",     TestIF_File.dFailAlarmSiteYieldCmp);
            WriteIniData(szDir, "Site Yield Alarm","Double Site Yield Cmp RT",  TestIF_File.dFailAlarmSiteYieldCmp_RT);
        }
    }
    else                                                                        //JerryYang 20160615 Yield相關alarm設定到整數
    {
        WriteIniData(szDir, "Site Yield Alarm","Site Yield Cmp",            int(TestIF_File.iFailAlarmSiteYieldCmp));
        WriteIniData(szDir, "Site Yield Alarm","Site Yield Cmp RT",         int(TestIF_File.iFailAlarmSiteYieldCmp_RT));
    }
    WriteIniData(szDir, "Site Yield Alarm","Site Yield Compare Count",      int(TestIF_File.iFailAlarmSiteYieldCmpCount));
    WriteIniData(szDir, "Site Yield Alarm","Site Yield Compare RT",         int(TestIF_File.bFailAlarmSiteYieldCmp_RT));
    WriteIniData(szDir, "Site Yield Alarm","Site Yield Compare Count RT",   int(TestIF_File.iFailAlarmSiteYieldCmpCount_RT));

//Low Yield Alarm(By Total)(%)                                                  //wei 20151116 Low Yield By Total
    WriteIniData(szDir, "Low Yield Alarm", "By Total Enable", int(TestIF_File.bFailAlarmLowYieldByTotal));
    if(CosFunction.bYieldAlarmUseDouble)                                        //JerryYang 20160530 LowYieldLimit要能設定到小數點
    {
        if(CUSTOMER_CODE==CC_QUALCOMM ||
           CUSTOMER_CODE==CC_AMKOR_China)                                       //JerryYang 20181016 (Steven) : Amkor要求yield改為小數點後，存讀檔的部份要與舊版本相容
        {
            WriteIniData(szDir, "Low Yield Alarm", "By Total Limit",     TestIF_File.dLowYieldLimitByTotal);
            WriteIniData(szDir, "Low Yield Alarm", "By Total Limit RT",  TestIF_File.dLowYieldLimitByTotal_RT);
        }
        else
        {
            WriteIniData(szDir, "Low Yield Alarm", "Double By Total Limit",     TestIF_File.dLowYieldLimitByTotal);
            WriteIniData(szDir, "Low Yield Alarm", "Double By Total Limit RT",  TestIF_File.dLowYieldLimitByTotal_RT);
        }
    }
    else                                                                        //JerryYang 20160615 Yield相關alarm設定到整數
    {
        WriteIniData(szDir, "Low Yield Alarm", "By Total Limit",            int(TestIF_File.iLowYieldLimitByTotal));
        WriteIniData(szDir, "Low Yield Alarm", "By Total Limit RT",         int(TestIF_File.iLowYieldLimitByTotal_RT));
    }
    WriteIniData(szDir, "Low Yield Alarm", "By Total Count",      int(TestIF_File.iLowYieldCountByTotal));
    WriteIniData(szDir, "Low Yield Alarm", "By Total Enable RT",  int(TestIF_File.bFailAlarmLowYieldByTotal_RT));
    WriteIniData(szDir, "Low Yield Alarm", "By Total Count RT",   int(TestIF_File.iLowYieldCountByTotal_RT));

//By Picker Compare Yield   [每個吸嘴的良率互比]                                //Steven 20230223 : 根據Index吸嘴比較良率
    WriteIniData(szDir, "Low Yield Alarm", "By Picker Enable",      TestIF_File.bLowYieldByPicker        );
    WriteIniData(szDir, "Low Yield Alarm", "By Picker Count",       TestIF_File.iLowYieldCountByPicker   );
    WriteIniData(szDir, "Low Yield Alarm", "By Picker Limit",       TestIF_File.dLowYieldByPicker        );
    WriteIniData(szDir, "Low Yield Alarm", "By Picker Enable RT",   TestIF_File.bLowYieldByPicker_RT     );
    WriteIniData(szDir, "Low Yield Alarm", "By Picker Count RT",    TestIF_File.iLowYieldCountByPicker_RT);
    WriteIniData(szDir, "Low Yield Alarm", "By Picker Limit RT",    TestIF_File.dLowYieldByPicker_RT     );

//Low Yield Alarm(Special)(%)
    if(CosFunction.bSpecailLowYeild)                                            //Sam 20210505 : PTI 要求的兩段 Low Yeild
    {
        WriteIniData(szDir, "Low Yield Alarm", "Special Enable", int(TestIF_File.bFailAlarmLowYieldSpecial));
        if(CosFunction.bYieldAlarmUseDouble)                                    //JerryYang 20160530 LowYieldLimit要能設定到小數點
        {
            WriteIniData(szDir, "Low Yield Alarm", "Special Limit",     TestIF_File.dLowYieldLimitSpecial);
        }
        else
        {
            WriteIniData(szDir, "Low Yield Alarm", "Special Limit",      int(TestIF_File.iLowYieldLimitSpecial));
        }
        WriteIniData(szDir, "Low Yield Alarm", "Special Count1",      int(TestIF_File.iLowYieldCountSpecial1));
        WriteIniData(szDir, "Low Yield Alarm", "Special Count2",      int(TestIF_File.iLowYieldCountSpecial2));
    }

//Alarm Action
    WriteIniData(szDir, "Alarm",     "Count Action",    int(TestIF_File.iCountAlarmAction));   //Steven 20101116
    WriteIniData(szDir, "Alarm",     "Count Action RT", int(TestIF_File.iCountAlarmAction_RT));

//Continuous Pass
    WriteIniData(szDir, "Alarm",     "Continuous Pass",              int(TestIF_File.bContinuousPass));
    WriteIniData(szDir, "Alarm",     "Continuous Pass Bin",          int(TestIF_File.iContinuousPassBin));
    WriteIniData(szDir, "Alarm",     "Continuous Pass Bin Count",    int(TestIF_File.iContinuousPassBinCount));
    WriteIniData(szDir, "Alarm",     "Continuous Pass RT",           int(TestIF_File.bContinuousPass_RT));
    WriteIniData(szDir, "Alarm",     "Continuous Pass RT Bin",       int(TestIF_File.iContinuousPassBin_RT));
    WriteIniData(szDir, "Alarm",     "Continuous Pass Bin Count RT", int(TestIF_File.iContinuousPassBinCount_RT));

//    WriteIniData(szDir, "Alarm",     "iBinAlarmByTotalOrBySite",     int(TestIF_File.iBinAlarmByTotalOrBySite));  //jou 2013-04-25 BIN Alarm By Total or By Site;
//    WriteIniData(szDir, "Alarm",     "iBinAlarmByTotalOrBySite_RT",  int(TestIF_File.iBinAlarmByTotalOrBySite_RT));  //jou 2013-04-25 BIN Alarm By Total or By Site;

//Continuous Pass by Socket - Steven 20110915
    WriteIniData(szDir, "Alarm",     "Continuous Pass By Socket",               int(TestIF_File.bContinuousPassBySocket));
    WriteIniData(szDir, "Alarm",     "Continuous Pass Bin Count By Socket",     int(TestIF_File.iContinuousPassBinCountBySocket));
    WriteIniData(szDir, "Alarm",     "Continuous Pass RT By Socket",            int(TestIF_File.bContinuousPassBySocket_RT));
    WriteIniData(szDir, "Alarm",     "Continuous Pass Bin Count RT By Socket",  int(TestIF_File.iContinuousPassBinCountBySocket_RT));

//Continuous Loader
    //jou 2010-11-17 Start
    WriteIniData(szDir, "Alarm",     "Continuous Loader",           int(TestIF_File.bContinuousLoader));
    WriteIniData(szDir, "Alarm",     "Continuous Loader Count",     int(TestIF_File.iContinuousLoaderCount));
    WriteIniData(szDir, "Alarm",     "Continuous Loader RT",        int(TestIF_File.bContinuousLoader_RT));
    WriteIniData(szDir, "Alarm",     "Continuous Loader Count RT",  int(TestIF_File.iContinuousLoaderCount_RT));
    //jou 2010-11-17 end

//Consecutive Failure Alarm ( Socket )
    WriteIniData(szDir, "Alarm",     "SocketEnable",    int(TestIF_File.bContsFailBySocket));
    WriteIniData(szDir, "Alarm",     "SocketCT",        int(TestIF_File.iContsFailSocketAlarmCT));
    WriteIniData(szDir, "Alarm",     "SocketEnable RT", int(TestIF_File.bContsFailBySocket_RT));
    WriteIniData(szDir, "Alarm",     "SocketCT RT",     int(TestIF_File.iContsFailSocketAlarmCT_RT));
    WriteIniData(szDir, "Alarm", "bCountSpcBinContinuously_FT", TestIF_File.bCountSpcBinContinuously_FT);      //Steven 20230529 : Spc Bin Couont改成連續錯誤
    WriteIniData(szDir, "Alarm", "bCountSpcBinContinuously_RT", TestIF_File.bCountSpcBinContinuously_RT);

//Consecutive Failure Alarm ( Head)
    WriteIniData(szDir, "Alarm",     "HeadEnable",      int(TestIF_File.bContsFailByHead));
    WriteIniData(szDir, "Alarm",     "HeadCT",          int(TestIF_File.iContsFailHeadAlarmCT));
    WriteIniData(szDir, "Alarm",     "HeadEnable RT",   int(TestIF_File.bContsFailByHead_RT));
    WriteIniData(szDir, "Alarm",     "HeadCT RT",       int(TestIF_File.iContsFailHeadAlarmCT_RT));

//Consecutive Failure Ignore                                                    //wei 20160115 銅鑼前幾顆不計算ContsFail
    WriteIniData(szDir, "Alarm",     "ContsFailIgnoreEnable",      int(TestIF_File.bContsFailIgnore));
    WriteIniData(szDir, "Alarm",     "ContsFailIgnoreCT",          int(TestIF_File.iContsFailIgnore));
    WriteIniData(szDir, "Alarm",     "ContsFailIgnoreEnable RT",   int(TestIF_File.bContsFailIgnore_RT));
    WriteIniData(szDir, "Alarm",     "ContsFailIgnoreCT RT",       int(TestIF_File.iContsFailIgnore_RT));

//Steven 20110426 Start
    WriteIniData(szDir, "Alarm",     "Continuous Contact",          int(TestIF_File.bContinuousContact));
    WriteIniData(szDir, "Alarm",     "Continuous Contact Count",    int(TestIF_File.iContinuousContactCount));
    WriteIniData(szDir, "Alarm",     "Continuous Contact RT",       int(TestIF_File.bContinuousContact_RT));
    WriteIniData(szDir, "Alarm",     "Continuous Contact Count RT", int(TestIF_File.iContinuousContactCount_RT));
//Steven 20110426 End

    //wei 20141201 Low Yield Auto Clean(%) start
    WriteIniData(szDir, "Low Yield Auto Clean", "Enable",     int(TestIF_File.bFailAlarmLowYield_AutoClean));
    WriteIniData(szDir, "Low Yield Auto Clean", "Count",      int(TestIF_File.iLowYieldCount_AutoClean));
    WriteIniData(szDir, "Low Yield Auto Clean", "Limit",      int(TestIF_File.iLowYieldLimit_AutoClean));

    if(CosFunction.bAutoCloseSiteWhenRT)                                        //Steven 20200225 : 切到RT的時候,要關閉Socket
    {
        WriteIniData(szDir, "Auto Close Site When RT", "iAutoCloseSiteYieldWhenRT",   TestIF_File.iAutoCloseSiteWhenRT);
        WriteIniData(szDir, "Auto Close Site When RT", "dAutoCloseSiteYieldWhenRT",   TestIF_File.dAutoCloseSiteYieldWhenRT);
        WriteIniData(szDir, "Site On at Initial Start", "iAllSiteOnAtInitialStart",   TestIF_File.iAllSiteOnAtInitialStart);    //Steven 20230814 : Initial Start的時候要全開Site
    }
    WriteIniData(szDir, "AutoSiteOnoff", "iAutoSiteOffByGPIB",   TestIF_File.iAutoSiteOffByGPIB);                               //JimmyChiu 20250715 : Auto site on/off by GPIB
    //---AutoRetest setup  kevin 20150704 start
    if(CUSTOMER_CODE==CC_ASE_KaohSiung)
    {
        WriteIniData(szDir, "AutoRetest", "ArtAutoRetestLimit", iAutoRetestLimit); //RT 次數
        WriteIniData(szDir, "AutoRetest", "bPassART",           MyYieldPanel[0]->bEnablePassYieldART);
        WriteIniData(szDir, "AutoRetest", "fPassyieldART",      MyYieldPanel[0]->fPassYieldART);
        WriteIniData(szDir, "AutoRetest", "bOpenshortART",      MyYieldPanel[0]->bEnableOpenShortART);
        WriteIniData(szDir, "AutoRetest", "fOpenShortYieldART", MyYieldPanel[0]->fOpenShortYieldART);
        WriteIniData(szDir, "AutoRetest", "bRecoverART",        MyYieldPanel[0]->bEnableRecoverART);
        WriteIniData(szDir, "AutoRetest", "fRecoverYieldART",   MyYieldPanel[0]->fRecoverYieldART);

        AnsiString bBin="", bBin1="", abuffer="", abuffer1="";                  //kevin 20170825 (wei) add
        for(int i=0; i<iTestBinCount; i++)                                      //Steven 20251104 : iBinCount --> iTestBinCount
        {
            if(MyYieldPanel[0]->bPass[i])
                abuffer.sprintf("%s,","1");
            else
                abuffer.sprintf("%s,","0");
            bBin+= abuffer;
            if(MyYieldPanel[0]->bOpenShort[i])
                abuffer1.sprintf("%s,","1");
            else
                abuffer1.sprintf("%s,","0");

            bBin1+= abuffer1;
        }
        WriteIniData(szDir, "AutoRetest", "bPassBin", bBin);
        WriteIniData(szDir, "AutoRetest", "bOpenShortBin", bBin1);
    }

    if(CUSTOMER_CODE==CC_KYEC_LEE)                                              //wei 20150825 KYEC 修改成By File
    {
        WriteIniData(szDir, "AutoRetest", "ArtAutoRetestLimitFile",             edAutoRetestLimitFile->Text);           //RT 次數
        WriteIniData(szDir, "AutoRetest", "ArtFailYieldRate_ARTFile",           edFailYieldRate_ARTFile->Text);         //wei 20160203

        WriteIniData(szDir, "AutoRetest", "ArtFailYieldMaxRate_ARTFTFile",      edFailYieldMaxRate_ARTFTFile->Text);    //wei 20160203
        WriteIniData(szDir, "AutoRetest", "ArtFailYieldMinRate_ARTFTFile",      edFailYieldMinRate_ARTFTFile->Text);    //wei 20160203
        WriteIniData(szDir, "AutoRetest", "ArtFailYieldRate_ARTFTFile",         edFailYieldRate_ARTFTFile->Text);       //wei 20160203

        WriteIniData(szDir, "AutoRetest", "ArtFailYieldRate_ARTRTFile",         edFailYieldRate_ARTRTFile->Text);       //wei 20160203
        WriteIniData(szDir, "AutoRetest", "ArtFailYieldMaxRate_ARTRTFile",      edFailYieldMaxRate_ARTRTFile->Text);    //wei 20160203
        WriteIniData(szDir, "AutoRetest", "ArtFailYieldMinRate_ARTRTFile",      edFailYieldMinRate_ARTRTFile->Text);    //wei 20160203

        WriteIniData(szDir, "AutoRetest", "AutoLeastRetestLimitFile",           edAutoLeastRetestLimitFile->Text);      //wei 20160203

        WriteIniData(szDir, "AutoRetest", "bAutoLeastRetestFile",               ckUseLeastRetestTimes->Checked);        //wei 20160203
        WriteIniData(szDir, "AutoRetest", "bUseFailNoDistinction",              ckUseFailNoDistinction->Checked);       //wei 20160203

        WriteIniData(szDir, "AutoRetest", "ArtUseFTFailYield",                  rgFT_ART->ItemIndex);                   //wei 20160203
        WriteIniData(szDir, "AutoRetest", "ArtUseRTFailYield",                  rgRT_ART->ItemIndex);                   //wei 20160203

        WriteIniData(szDir, "AutoRetest", "ArtUseFTFailYieldModel",             cbUseFTFailYieldModel->ItemIndex);                                      //wei 20160203
        WriteIniData(szDir, "AutoRetest", "ArtUseRTFailYieldModel",             cbUseRTFailYieldModel->ItemIndex);                                      //wei 20160203
    }

    //wei 20160314
    if(CosFunction.bYieldAlarm4)                                                //Steven 20230223 : 隱藏沒用到的
    {
        WriteIniData(szDir, "Alarm4", "bAlarm4ContinueType_Enable",             cbAlarm4ContinueType->Checked);
        WriteIniData(szDir, "Alarm4", "iAlarm4ContinueType_IntervalCount",      edAlarm4IntervalCount->Text);
        WriteIniData(szDir, "Alarm4", "iAlarm4ContinueType_ContinueCount",      edAlarm4ContinueCount->Text);

        WriteIniData(szDir, "Alarm4", "bAlarm4EnableIntervalYield",             cb_Alarm4EnableIntervalYield->Checked);
        WriteIniData(szDir, "Alarm4", "iAlarm4IntervalYieldIntervalCount",      ed_Alarm4IntervalYieldIntervalCount->Text);
        WriteIniData(szDir, "Alarm4", "iAlarm4IntervalYieldContinueCount",      ed_Alarm4IntervalYieldContinueCount->Text);
        WriteIniData(szDir, "Alarm4", "iAlarm4IntervalYieldYield",              ed_Alarm4IntervalYieldYield->Text);

        WriteIniData(szDir, "Alarm4", "SiteToSiteYieldCmp_Enable",              cb_SiteToSiteYieldEnable->Checked);
        WriteIniData(szDir, "Alarm4", "SiteToSiteYieldCmp_Yield",               ed_SiteToSiteYield->Text);
        WriteIniData(szDir, "Alarm4", "SiteToSiteYieldCmp_IntervalCount",       ed_SiteToSiteYieldCount->Text);

        WriteIniData(szDir, "Alarm4", "HeadToHeadYieldCmp_Enable",              cb_HeadToHeadYieldEnable->Checked);
        WriteIniData(szDir, "Alarm4", "HeadToHeadYieldCmp_Yield",               ed_HeadToHeadYield->Text);
        WriteIniData(szDir, "Alarm4", "HeadToHeadYieldCmp_IntervalCount",       ed_HeadToHeadYieldCount->Text);

        WriteIniData(szDir, "Alarm4", "SiteYieldOverAlert_Enable",              cb_SiteYieldOverAlert->Checked);
        WriteIniData(szDir, "Alarm4", "SiteYieldOverAlert_Yield",               ed_SiteYieldOverAlert->Text);
        WriteIniData(szDir, "Alarm4", "SiteYieldOverAlert_IntervalCount",       ed_SiteYieldOverAlertCount->Text);
    }

    //Steven 20231017 : change position
    WriteIniData(szDir, "Alarm4", "AllSiteFail",                                cbAllSiteFail->Checked);    //kevin 20170825 (wei) 整支ARM Fail bin
    WriteIniData(szDir, "Alarm4", "AllSiteFailCount",                           edAllSiteFailCount->Text);  //kevin 20180720 (wei) all site fail count
    WriteIniData(szDir, "Alarm4", "AllSiteFailCount_RT",                        edAllSiteFailCountRT->Text);//Steven 20230118 : All site fail RT
    WriteIniData(szDir, "Alarm4", "AllSiteFail_RT",                             cbAllSiteFail_RT->Checked); //Isaac 20180305 (Steven) ATK要求，只有FT要alarm，FT/RT分開

    //Sam 20171213 (Steven) : 超豐良率監控
    //==>
    if(CosFunction.bYieldAlarm5==true)                                          //Sam 20180423 (wei) : MOFile of Yeild Download
    {
        WriteIniData(szDir, "Alarm5", "iAlarm5_BySiteIntervalContactCnt",       edAlarm5_BySiteIntervalContactCnt->Text);
        WriteIniData(szDir, "Alarm5", "bAlarm5_BySiteLowYieldEnable",           cbAlarm5_BySiteLowYieldEnable->Checked);    //Sam 20171213 : 超豐良率監控
        WriteIniData(szDir, "Alarm5", "dAlarm5_BySiteLowYield",                 edAlarm5_BySiteLowYield->Text);             //Sam 20171213 : 超豐良率監控
        WriteIniData(szDir, "Alarm5", "dAlarm5_BySiteLowYieldRej",              edAlarm5_BySiteLowYieldRej->Text);          //Sam 20180423 (wei) : MOFile of Yeild Download
        WriteIniData(szDir, "Alarm5", "bAlarm5_BySiteCmpYieldEnable",           cbAlarm5_BySiteCmpYieldEnable->Checked);    //Sam 20171213 : 超豐良率監控
        WriteIniData(szDir, "Alarm5", "dAlarm5_BySiteCmpYield",                 edAlarm5_BySiteCmpYield->Text);             //Sam 20171213 : 超豐良率監控
        WriteIniData(szDir, "Alarm5", "dAlarm5_BySiteCmpYieldRej",              edAlarm5_BySiteCmpYieldRej->Text);          //Sam 20180423 (wei) : MOFile of Yeild Download
        WriteIniData(szDir, "Alarm5", "bAlarm5_BySiteAlarmYieldEnable",         cbAlarm5_BySiteAlarmYieldEnable->Checked);  //Sam 20171213 : 超豐良率監控
        WriteIniData(szDir, "Alarm5", "dAlarm5_BySiteAlarmYield",               edAlarm5_BySiteAlarmYield->Text);           //Sam 20171213 : 超豐良率監控
        WriteIniData(szDir, "Alarm5", "dAlarm5_BySiteAlarmYieldRej",            edAlarm5_BySiteAlarmYieldRej->Text);        //Sam 20180423 (wei) : MOFile of Yeild Download
        WriteIniData(szDir, "Alarm5", "iAlarm5_OSBin",                          edAlarm5_OSBin->Text);                      //Sam 20171213 : 超豐良率監控
        WriteIniData(szDir, "Alarm5", "bAlarm5_BySitePreCmpYieldEnable",        cbAlarm5_BySitePreCmpYieldEnable->Checked); //Sam 20180423 (wei) : MOFile of Yeild Download
        WriteIniData(szDir, "Alarm5", "dAlarm5_BySitePreCmpYield",              edAlarm5_BySitePreCmpYield->Text);          //Sam 20180423 (wei) : MOFile of Yeild Download
        WriteIniData(szDir, "Alarm5", "dAlarm5_BySitePreCmpYieldRej",           edAlarm5_BySitePreCmpYieldRej->Text);       //Sam 20180423 (wei) : MOFile of Yeild Download
    }

    eCloseSite->Text = IntToStr(TestIF_File.iLowYieldLimit_AutoClean*2);        //kevin 20150306
    bool doAutoYield=false;
    if(TestIF_File.bFailAlarmLowYield_AutoClean==true)
    {
        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                if(bLowYieldCloseSite[0][i][j])
                    doAutoYield=true;                                           //kevin 20150306 儲存時尚未做過AUTO YIELD 將首次記錄更新
            }
        }

        if(doAutoYield==false)
            bStandardYield=false;                                               //kevin 20150306 儲存時將
    }
    //Site Yield Alarm(%)

    if(CosFunction.bAdaptiveYield)                                              //Sam 20230914 : 自適應性良率監控
    {
        WriteIniData(szDir, "AdaptiveYield",     "Low Yield Alarm",           TestIF_File.bAdaptiveLowYield);
        WriteIniData(szDir, "AdaptiveYield",     "Consecutive Lower Alarm",     TestIF_File.iAdaptiveContsLowerAlarmNor);   //Sam 20240726 : AI Clean
        WriteIniData(szDir, "AdaptiveYield",     "Consecutive Lower Alarm Min", TestIF_File.iAdaptiveContsLowerAlarmMin);
        WriteIniData(szDir, "AdaptiveYield",     "Yield Max",                   TestIF_File.iAdaptiveYieldMax);
        WriteIniData(szDir, "AdaptiveYield",     "Yield Min",                   TestIF_File.iAdaptiveYieldMin);

        WriteIniData(szDir, "AdaptiveYield",     "Low Yield Alarm RT",        TestIF_File.bAdaptiveLowYield_RT);
        WriteIniData(szDir, "AdaptiveYield",     "Consecutive Lower Alarm RT",  TestIF_File.iAdaptiveContsLowerAlarmNor_RT);    //Sam 20240726 : AI Clean
        WriteIniData(szDir, "AdaptiveYield",     "Consecutive Lower Alarm Min RT",TestIF_File.iAdaptiveContsLowerAlarmMin_RT);
        WriteIniData(szDir, "AdaptiveYield",     "Yield Max RT",                TestIF_File.iAdaptiveYieldMax_RT);
        WriteIniData(szDir, "AdaptiveYield",     "Yield Min RT",                TestIF_File.iAdaptiveYieldMin_RT);
    }

    if(CosFunction.bByBinAlarmFromYieldForm)                                    //jou 20180113 (Steven) : By Site By Bin Percent Compare From Yield form
    {
        AnsiString asSpecBinBySiteCompare;
        for(int i=0; i<iTestBinCount; i++)
        {
            asSpecBinBySiteCompare.printf("bSpecBinBySiteCompareEnable%02d_FT", i);
            WriteIniData(szDir, "Alarm",     asSpecBinBySiteCompare,           TestIF_File.bSpecBinBySiteCompareEnable[FT][i]);
            asSpecBinBySiteCompare.printf("bSpecBinBySiteCompareEnable%02d_RT", i);
            WriteIniData(szDir, "Alarm",     asSpecBinBySiteCompare,           TestIF_File.bSpecBinBySiteCompareEnable[RT][i]);

            asSpecBinBySiteCompare.printf("dSpecBinBySiteComparePercent%02d_FT", i);
            WriteIniData(szDir, "Alarm",     asSpecBinBySiteCompare,           double(TestIF_File.dSpecBinBySiteComparePercent[FT][i]));
            asSpecBinBySiteCompare.printf("dSpecBinBySiteComparePercent%02d_RT", i);
            WriteIniData(szDir, "Alarm",     asSpecBinBySiteCompare,           double(TestIF_File.dSpecBinBySiteComparePercent[RT][i]));

            asSpecBinBySiteCompare.printf("bSpecBinByArmPerSiteCompareEnable%02d_FT", i);
            WriteIniData(szDir, "Alarm",     asSpecBinBySiteCompare,           TestIF_File.bSpecBinByArmPerSiteCompareEnable[FT][i]);
            asSpecBinBySiteCompare.printf("bSpecBinByArmPerSiteCompareEnable%02d_RT", i);
            WriteIniData(szDir, "Alarm",     asSpecBinBySiteCompare,           TestIF_File.bSpecBinByArmPerSiteCompareEnable[RT][i]);

            asSpecBinBySiteCompare.printf("dSpecBinByArmPerSiteComparePercent%02d_FT", i);
            WriteIniData(szDir, "Alarm",     asSpecBinBySiteCompare,           double(TestIF_File.dSpecBinByArmPerSiteComparePercent[FT][i]));
            asSpecBinBySiteCompare.printf("dSpecBinByArmPerSiteComparePercent%02d_RT", i);
            WriteIniData(szDir, "Alarm",     asSpecBinBySiteCompare,           double(TestIF_File.dSpecBinByArmPerSiteComparePercent[RT][i]));

            asSpecBinBySiteCompare.printf("bByBinFailureEnable%02d_FT", i);
            WriteIniData(szDir, "Alarm",     asSpecBinBySiteCompare,           TestIF_File.bByBinFailureEnable[FT][i]);
            asSpecBinBySiteCompare.printf("bByBinFailureEnable%02d_RT", i);
            WriteIniData(szDir, "Alarm",     asSpecBinBySiteCompare,           TestIF_File.bByBinFailureEnable[RT][i]);

            asSpecBinBySiteCompare.printf("dByBinFailurePercent%02d_FT", i);
            WriteIniData(szDir, "Alarm",     asSpecBinBySiteCompare,           double(TestIF_File.dByBinFailurePercent[FT][i]));
            asSpecBinBySiteCompare.printf("dByBinFailurePercent%02d_RT", i);
            WriteIniData(szDir, "Alarm",     asSpecBinBySiteCompare,           double(TestIF_File.dByBinFailurePercent[RT][i]));
        }
        WriteIniData(szDir, "Alarm",     "iSpecBinBySiteCompareIgnore_FT",       int(TestIF_File.iSpecBinBySiteCompareIgnore[FT]));
        WriteIniData(szDir, "Alarm",     "iSpecBinBySiteCompareIgnore_RT",       int(TestIF_File.iSpecBinBySiteCompareIgnore[RT]));
        WriteIniData(szDir, "Alarm",     "iSpecBinByArmPerSiteCompareIgnore_FT", int(TestIF_File.iSpecBinByArmPerSiteCompareIgnore[FT]));
        WriteIniData(szDir, "Alarm",     "iSpecBinByArmPerSiteCompareIgnore_RT", int(TestIF_File.iSpecBinByArmPerSiteCompareIgnore[RT]));
        WriteIniData(szDir, "Alarm",     "iByBinFailureIgnore_FT", int(TestIF_File.iByBinFailureIgnore[FT]));
        WriteIniData(szDir, "Alarm",     "iByBinFailureIgnore_RT", int(TestIF_File.iByBinFailureIgnore[RT]));
    }

    WriteIniData(szDir, "Alarm",     "LoadCellMeasure",       cbLoadCellMeasure->Checked);      //kevin 20190907 Arm 測區次數道量測 功能;
    WriteIniData(szDir, "Alarm",     "LoadCellMeasureCount",  edLoadCellMeasure->Text);           //kevin 20190907 Arm 測區次數道量測

    if(CosFunction.IntervalYieldCount==true)                                    //Steven 20230223 : 隱藏沒用到的
    {
        //Interval Low Yield Alarm(By Site)(%)                                  //wei 20180606 Interval Low Yield By Site
        WriteIniData(szDir, "Low Yield Alarm",     "Interval By Site Enable",           int(TestIF_File.bFailAlarmIntervalLowYieldBySite));
        WriteIniData(szDir, "Low Yield Alarm",     "Interval By Site Count",            int(TestIF_File.iIntervalLowYieldCountBySite));
        WriteIniData(szDir, "Low Yield Alarm",     "Interval By Site Enable RT",        int(TestIF_File.bFailAlarmIntervalLowYieldBySite_RT));
        WriteIniData(szDir, "Low Yield Alarm",     "Interval By Site Count RT",         int(TestIF_File.iIntervalLowYieldCountBySite_RT));
        if(CosFunction.bYieldAlarmUseDouble)
        {
            WriteIniData(szDir, "Low Yield Alarm", "Interval Double By Site Limit",     TestIF_File.dIntervalLowYieldLimitBySite);
            WriteIniData(szDir, "Low Yield Alarm", "Interval Double By Site Limit RT",  TestIF_File.dIntervalLowYieldLimitBySite_RT);
        }
        else
        {
            WriteIniData(szDir, "Low Yield Alarm", "Interval By Site Limit",            TestIF_File.iIntervalLowYieldLimitBySite);
            WriteIniData(szDir, "Low Yield Alarm", "Interval By Site Limit RT",         TestIF_File.iIntervalLowYieldLimitBySite_RT);

        }
        //Interval Low Yield Alarm(By Total)(%)                                 //wei 20180718 Interval Low Yield By Total
        WriteIniData(szDir, "Low Yield Alarm",     "Interval By Total Enable",          int(TestIF_File.bFailAlarmIntervalLowYieldByTotal));
        WriteIniData(szDir, "Low Yield Alarm",     "Interval By Total Count",           int(TestIF_File.iIntervalLowYieldCountByTotal));
        WriteIniData(szDir, "Low Yield Alarm",     "Interval By Total Enable RT",       int(TestIF_File.bFailAlarmIntervalLowYieldByTotal_RT));
        WriteIniData(szDir, "Low Yield Alarm",     "Interval By Total Count RT",        int(TestIF_File.iIntervalLowYieldCountByTotal_RT));
        if(CosFunction.bYieldAlarmUseDouble)
        {
            WriteIniData(szDir, "Low Yield Alarm", "Interval Double By Total Limit",    TestIF_File.dIntervalLowYieldLimitByTotal);
            WriteIniData(szDir, "Low Yield Alarm", "Interval Double By Total Limit RT", TestIF_File.dIntervalLowYieldLimitByTotal_RT);
        }
        else
        {
            WriteIniData(szDir, "Low Yield Alarm", "Interval By Total Limit",           TestIF_File.iIntervalLowYieldLimitByTotal);
            WriteIniData(szDir, "Low Yield Alarm", "Interval By Total Limit RT",        TestIF_File.iIntervalLowYieldLimitByTotal_RT);
        }
    }

    if(CosFunction.bCreateManualEOCAP)                                          //jou 20221104 : VTest CreateManualEOCAP function;
    {
        WriteIniData(szDir, "Alarm",     "bCreateManualEOCAP",       int(TestIF_File.bCreateManualEOCAP));
    }
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::SaveSetupFileToConfig(AnsiString szDir, AnsiString S)    //Isaac 20170712 :Piggyback function By Handlder(save file to config.ini)
{
    MyForceDirectories(szDir, "TfYieldMonitoring::SaveSetupFileToConfig");
    szDir+=S;

    //Alarm Action
    WriteIniData(szDir, "Alarm",     "Count Action",    int(TestIF_File.iCountAlarmAction));   //Steven 20101116
    WriteIniData(szDir, "Alarm",     "Count Action RT", int(TestIF_File.iCountAlarmAction_RT));

    //Continuous Pass
    WriteIniData(szDir, "Alarm",     "Continuous Pass",              int(TestIF_File.bContinuousPass));
    WriteIniData(szDir, "Alarm",     "Continuous Pass Bin",          int(TestIF_File.iContinuousPassBin));
    WriteIniData(szDir, "Alarm",     "Continuous Pass Bin Count",    int(TestIF_File.iContinuousPassBinCount));
    WriteIniData(szDir, "Alarm",     "Continuous Pass RT",           int(TestIF_File.bContinuousPass_RT));
    WriteIniData(szDir, "Alarm",     "Continuous Pass RT Bin",       int(TestIF_File.iContinuousPassBin_RT));
    WriteIniData(szDir, "Alarm",     "Continuous Pass Bin Count RT", int(TestIF_File.iContinuousPassBinCount_RT));

    //Continuous Pass by Socket - Steven 20110915
    WriteIniData(szDir, "Alarm",     "Continuous Pass By Socket",               int(TestIF_File.bContinuousPassBySocket));
    WriteIniData(szDir, "Alarm",     "Continuous Pass Bin Count By Socket",     int(TestIF_File.iContinuousPassBinCountBySocket));
    WriteIniData(szDir, "Alarm",     "Continuous Pass RT By Socket",            int(TestIF_File.bContinuousPassBySocket_RT));
    WriteIniData(szDir, "Alarm",     "Continuous Pass Bin Count RT By Socket",  int(TestIF_File.iContinuousPassBinCountBySocket_RT));

    //Continuous Loader
    WriteIniData(szDir, "Alarm",     "Continuous Loader",           int(TestIF_File.bContinuousLoader));
    WriteIniData(szDir, "Alarm",     "Continuous Loader Count",     int(TestIF_File.iContinuousLoaderCount));
    WriteIniData(szDir, "Alarm",     "Continuous Loader RT",        int(TestIF_File.bContinuousLoader_RT));
    WriteIniData(szDir, "Alarm",     "Continuous Loader Count RT",  int(TestIF_File.iContinuousLoaderCount_RT));

    WriteIniData(szDir, "Alarm",     "Continuous Contact",          int(TestIF_File.bContinuousContact));
    WriteIniData(szDir, "Alarm",     "Continuous Contact Count",    int(TestIF_File.iContinuousContactCount));
    WriteIniData(szDir, "Alarm",     "Continuous Contact RT",       int(TestIF_File.bContinuousContact_RT));
    WriteIniData(szDir, "Alarm",     "Continuous Contact Count RT", int(TestIF_File.iContinuousContactCount_RT));
}
//---------------------------------------------------------------------------
void TfYieldMonitoring::ChangeData(TWinControl *PCtrl)
{
    for(int iP = 0;iP < PCtrl->ControlCount;iP ++)
    {
        TControl *P = PCtrl->Controls[iP];
        if(dynamic_cast <TPanel       *>(P)!=NULL ||
           dynamic_cast <TPageControl *>(P)!=NULL ||
           dynamic_cast <TTabSheet    *>(P)!=NULL ||
           dynamic_cast <TTabControl  *>(P)!=NULL ||
           dynamic_cast <TForm        *>(P)!=NULL ||
           dynamic_cast <THeader      *>(P)!=NULL ||
           dynamic_cast <TPage        *>(P)!=NULL ||
           dynamic_cast <TGroupBox    *>(P)!=NULL ||
           dynamic_cast <TScrollBox   *>(P)!=NULL)
        {
            ChangeData((TWinControl *) P);                                      // 找該物件附屬的物件(遞迴)
        }

        TCheckBox       *CheckBox   =dynamic_cast<TCheckBox     *>(P);
        TLabeledEdit    *PTLEdit    =dynamic_cast<TLabeledEdit  *>(P);          //Steven 20200715 : 補上TLabeledEdit
        TEdit           *Edit       =dynamic_cast<TEdit         *>(P);
        TComboBox       *ComboBox   =dynamic_cast<TComboBox     *>(P);
        TRadioButton    *RadioButton=dynamic_cast<TRadioButton  *>(P);

        if(CheckBox!=NULL)
        {
            CheckBox->OnClick=edContactCountFTChange;
        }
        if(Edit!=NULL)
        {
            Edit->OnChange=edContactCountFTChange;
        }
        if(PTLEdit!=NULL)
        {
            PTLEdit->OnChange=edContactCountFTChange;
        }
        if(RadioButton!=NULL)
        {
            RadioButton->OnClick=edContactCountFTChange;
        }
        if(ComboBox!=NULL)
        {
            ComboBox->OnChange=edContactCountFTChange;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::FormShow(TObject *Sender)
{
    AnsiString Str;
    //Steven 20120809 Start: 先在最上面Enable全部畫面
    gbYieldAlarm    ->Enabled=true;
    gbPiggyBack     ->Enabled=true;
    gbAlarm         ->Enabled=true;
    gbYieldAlarmRT  ->Enabled=true;
    gbPiggyBackRT   ->Enabled=true;
    gbAlarmRT       ->Enabled=true;
    pgcBySiteByBinPercentCompare_FT->Enabled=true;
    pgcBySiteByBinPercentCompare_RT->Enabled=true;
    //Steven 20120809 End: 先在最上面Enable全部畫面

    for(int i=0; i<TEST_MAX_BIN; i++)
    {
        cbByBinSiteGapCat_FT[i]->Visible=(i<iTestBinCount);
        cbByBinSiteGapCat_RT[i]->Visible=(i<iTestBinCount);
        edByBinSiteGapCat_FT[i]->Visible=(i<iTestBinCount);
        edByBinSiteGapCat_RT[i]->Visible=(i<iTestBinCount);
        cbByArmSiteGapCat_FT[i]->Visible=(i<iTestBinCount);
        cbByArmSiteGapCat_RT[i]->Visible=(i<iTestBinCount);
        edByArmSiteGapCat_FT[i]->Visible=(i<iTestBinCount);
        edByArmSiteGapCat_RT[i]->Visible=(i<iTestBinCount);
        cbByBinFailureCat_FT[i]->Visible=(i<iTestBinCount);
        cbByBinFailureCat_RT[i]->Visible=(i<iTestBinCount);
        edByBinFailureCat_FT[i]->Visible=(i<iTestBinCount);
        edByBinFailureCat_RT[i]->Visible=(i<iTestBinCount);
    }

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //kevin 20150704 ART  bin 設定
    {
        MyYieldPanel[0]->mtBinSelectYield->Width=iColWidth*iTestBinCount+10;
        MyYieldPanel[0]->mtBinSelectYield->XItem=iTestBinCount;
        tsReTest->TabVisible=false;                                             //kevin 20150717
    }
    ReadFile();
    ChangeData(fYieldMonitoring);

    btnApply->Enabled=false;
    Top=50;
    Left=200;

    if(LastSet.iRunStartMode==rsmContinuStart ||
       LastSet.iRunStartMode==rsmInitialStart ||
       LastSet.iRunStartMode==rsmAutoSiteMap ||
       LastSet.iRunStartMode==rsmQAMode)                                        //ChungHung 20120725 add QAMode 使用 Noraml
    {
        pgcMode->ActivePage=tsNormal;
    }
    else if(LastSet.iRunStartMode==rsmInitial_ART ||
            LastSet.iRunStartMode==rsmContinuStart_ART||
            LastSet.iRunStartMode==rsmContinuRetest_ART||
            LastSet.iRunStartMode==rsmAutoRetest)
    {
        if(CUSTOMER_CODE==CC_KYEC_LEE)
        {
            pgcMode->ActivePage=tsAutoRetest1;                                  //wei 20150825 KYEC 修改成By File
        }
        else if(CUSTOMER_CODE==CC_ASE_KaohSiung)
        {
            pgcMode->ActivePage=tsAutoRetest;                                   //kevin 20150717
        }
        else
        {
            pgcMode->ActivePage=tsNormal;
        }
    }
    else
    {
        pgcMode->ActivePage=tsReTest;
    }

    if(CUSTOMER_CODE==CC_SCC)                                                   //Steven 20101221
    {
        if(IniConfig.bEnableRms && AccessLevel<=iDefEngineerLevel)              //jou 2014-06-19 Security Have 5 Level 1->iDefEngineerLevel
        {
            tsNormal->Enabled=false;
            tsReTest->Enabled=false;
        }
        else
        {
            tsNormal->Enabled=true;
            tsReTest->Enabled=true;
        }
        gbPiggyBack->Caption="Piggingback Check";
        gbPiggyBackRT->Caption="Piggingback Check";

    }

    if(CosFunction.bPiggyBackForASE==true)                                      //Steven 20131101 : 高雄ASE不要Continual Pass Bin(Total )跟 Continual Loader兩種
    {
        palContinuousPass_FT  ->Visible=false;
        palContinuousPass_RT  ->Visible=false;
        palContinuousLoader_FT->Visible=false;
        palContinuousLoader_RT->Visible=false;
        tsAutoRetest->TabVisible=true;                                          //kevin 20150601
        tsAutoRetest1->TabVisible=false;                                        //wei 20150825
    }
    else
    {
        tsAutoRetest->TabVisible=false;                                         //kevin 20150601
        if(CUSTOMER_CODE==CC_KYEC_LEE &&
           USE_AUTO_RETEST==eartInstall &&
           IniConfig.bA10_AutoReTest)
            tsAutoRetest1->TabVisible=true;                                     //wei 20150825 KYEC 修改成By File
        else
            tsAutoRetest1->TabVisible=false;                                    //wei 20150825 KYEC 修改成By File
    }
    tsAlarm4->TabVisible        =(CosFunction.bYieldAlarm4 || CosFunction.bYieldAlarm5);//wei 20160406 Yield Alarm4
    grpAlarm4->Visible          =(CosFunction.bYieldAlarm4);
    grpAlarm5->Visible          =(CosFunction.bYieldAlarm5);                    //Sam 20171213 (Steven) : 超豐良率監控
    tsAutoSiteOff->TabVisible   =CosFunction.bLowYieldAutoSiteOff;              //Steven 20200420 : 整合Auto Site Off
    tsYield->TabVisible         =(CosFunction.IntervalYieldCount);              //Steven 20230223 : 隱藏沒用到的

    grpAutoSiteOffWhenRT->Visible=CosFunction.bAutoCloseSiteWhenRT;             //Steven 20200225 : 切到RT的時候,要關閉Socket
    grpSiteYieldCmp_FT  ->Visible=CosFunction.bSiteCmpYield;                    //jou 2014-08-14 Site Compare Low Yield alarm
    grpSiteYieldCmp_RT  ->Visible=CosFunction.bSiteCmpYield;                    //jou 2014-08-14 Site Compare Low Yield alarm
    cbYieldAlarmByBin   ->Visible=CosFunction.bUseLowYieldAlarmByBin;           //Steven 20140828 : By Bin Yield Monitor
//    rgCloseSiteOnHPAction->Visible=(LastSet.iTemperature==Tempture_Hot);      //Steven 20250430 : mark for 常溫也要用  //JerryYang 20180726 (wei) 關site的位置有IC不測試送error bin
//    cbbClosedSiteBin    ->Visible=(LastSet.iTemperature==Tempture_Hot);         //JerryYang 20180726 (wei) 關site的位置有IC不測試送error bin
    rgAutoSiteOn        ->Visible=(CosFunction.bLowYieldAutoSiteOff ||          //Steven 20230814 : Initial Start的時候要全開Site
                                   CosFunction.bAutoCloseSiteWhenRT);

    chkAutoSiteOff              ->Visible=(CosFunction.bLowYieldAutoSiteOff);
    chkAutoSiteOffByArmBySite   ->Visible=(CosFunction.bLowYieldAutoSiteOff);
    chkAutoSiteOffByContiFail   ->Visible=(CosFunction.bLowYieldAutoSiteOff);
    chkAutoSiteOffByArmContiFail->Visible=(CosFunction.bLowYieldAutoSiteOff);
    chkchkAutoSiteOffByPicker   ->Visible=(CosFunction.bLowYieldAutoSiteOff);   //Steven 20230223 : 根據Index吸嘴比較良率

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //kevin 20211222 add Visible
    {
        labSiteYieldCmp1SiteCnt_FT->Visible=false;
        edtSiteYieldCmp1SiteCnt_FT->Visible=false;
        chkAutoSiteOff->Visible=false;
        chkAutoSiteOffByContiFail->Visible=false;
        chkAutoSiteOffByArmBySite->Visible=false;                               //Steven 20230223 : by arm by site, auto site off
    }

    grpAdaptiveYield_FT->Visible=CosFunction.bAdaptiveYield;                    //Sam 20230914 : 自適應性良率監控
    grpAdaptiveYield_RT->Visible=CosFunction.bAdaptiveYield;

    if(gbPiggyBack->Enabled==false)                                             //kevin 20131030 權限不足不使用false
    {
        rgPiggyBack_FT     ->Enabled=false;
        cbContinuPassSkt_FT->Enabled=false;
        cbContactCountFT   ->Enabled=false;
        edContinuPassSkt_FT->Enabled=false;
        edContactCountFT   ->Enabled=false;
    }
    else                                                                        //Steven 20120804 Start: 在最下面決定權限能不能Enable
    {
        rgPiggyBack_FT      ->Enabled=fSecurity->Insufficient(39, false);
        edContinuPassSkt_FT ->Enabled=fSecurity->Insufficient(39, false);
        edContactCountFT    ->Enabled=fSecurity->Insufficient(39, false);
    }

    if(CUSTOMER_CODE!=CC_Greatek)
    {
        gbYieldAlarm    ->Enabled=(AccessLevel<LevelSet.AccessLevel[81])?false:gbYieldAlarm  ->Enabled;
        gbPiggyBack     ->Enabled=(AccessLevel<LevelSet.AccessLevel[82])?false:gbPiggyBack   ->Enabled;
        gbAlarm         ->Enabled=(AccessLevel<LevelSet.AccessLevel[83])?false:gbAlarm       ->Enabled;
        gbYieldAlarmRT  ->Enabled=(AccessLevel<LevelSet.AccessLevel[81])?false:gbYieldAlarmRT->Enabled;
        gbPiggyBackRT   ->Enabled=(AccessLevel<LevelSet.AccessLevel[82])?false:gbPiggyBackRT ->Enabled;
        gbAlarmRT       ->Enabled=(AccessLevel<LevelSet.AccessLevel[83])?false:gbAlarmRT     ->Enabled;

        //jou 20180130 : 新增 BySiteByBinPercentCompare level 管控
        pgcBySiteByBinPercentCompare_FT->Enabled=(AccessLevel<LevelSet.AccessLevel[81])?false:gbYieldAlarm  ->Enabled;
        pgcBySiteByBinPercentCompare_RT->Enabled=(AccessLevel<LevelSet.AccessLevel[81])?false:gbYieldAlarm  ->Enabled;
        if(CosFunction.bLotStartLockCriticalPara && RunInfo.bLotStart)          //JerryYang 20220311 : ATP鎖定Critical parameter
        {
            if(bAuthCriticalPara[22])
            {
                gbAlarm->Enabled=false;
            }
        }
    }

    if(CosFunction.bLotStartLockCriticalPara && RunInfo.bLotStart)              //JerryYang 20220311 : ATP鎖定Critical parameter
    {
        if(bAuthCriticalPara[16])
        {
            gbPiggyBack->Enabled=false;
        }
    }

    if(gbPiggyBackRT     ->Enabled==false)                                      //kevin 20131030 權限不足不使用false
    {
         rgPiggyBack_RT     ->Enabled=false;
         cbContinuPassSkt_RT->Enabled=false;
         cbContactCountRT   ->Enabled=false;
         edContinuPassSkt_RT->Enabled=false;
         edContactCountRT   ->Enabled=false;
    }
    else
    {
        rgPiggyBack_RT      ->Enabled=fSecurity->Insufficient(39, false);
        edContinuPassSkt_RT ->Enabled=fSecurity->Insufficient(39, false);
        edContactCountRT    ->Enabled=fSecurity->Insufficient(39, false);
    }

    if(CUSTOMER_CODE==CC_AMKOR_Philippines)
    {
        rgPiggyBack_RT     ->Enabled=false;
        rgPiggyBack_FT     ->Enabled=false;
    }

    if(CUSTOMER_CODE==CC_KYEC_LEE ||                                            //20140320 wei
       CUSTOMER_CODE==CC_KYEC_XILINX ||
       CUSTOMER_CODE==CC_UTAC_TW)                                               //Sam 20230106 : 聯測廠新增 Yield 權限卡控
    {
        if(AccessLevel<LevelSet.AccessLevel[39])
        {
            tsNormal->Enabled=false;
            tsReTest->Enabled=false;
            tsAutoSiteOff->Enabled=false;                                       //Sam 20230106 : 新增 AutoSiteOff
        }
        else
        {
            tsNormal->Enabled=true;
            tsReTest->Enabled=true;
            tsAutoSiteOff->Enabled=true;                                        //Sam 20230106 : 新增 AutoSiteOff
        }
    }

    if(CUSTOMER_CODE==CC_Greatek)
    {
        cbLowYield_FT                       ->Enabled=fSecurity->Insufficient(118, false);
        cbSiteYieldDifferent_FT             ->Enabled=fSecurity->Insufficient(118, false);
        cbLowYield_RT                       ->Enabled=fSecurity->Insufficient(118, false);
        cbSiteYieldDifferent_RT             ->Enabled=fSecurity->Insufficient(118, false);

        edLowYield_FT                       ->Enabled=fSecurity->Insufficient(81, false);
        edSiteYieldDifferent_FT             ->Enabled=fSecurity->Insufficient(81, false);
        edLowYieldIg_FT                     ->Enabled=fSecurity->Insufficient(81, false);
        edSiteYieldDifferentIg_FT           ->Enabled=fSecurity->Insufficient(81, false);
        edSiteYieldDifferent_RT             ->Enabled=fSecurity->Insufficient(81, false);
        edLowYield_RT                       ->Enabled=fSecurity->Insufficient(81, false);
        edLowYieldIg_RT                     ->Enabled=fSecurity->Insufficient(81, false);
        edSiteYieldDifferentIg_RT           ->Enabled=fSecurity->Insufficient(81, false);

        cbContinuousPass_FT                 ->Enabled=fSecurity->Insufficient(119, false);
        cbContactCountFT                    ->Enabled=fSecurity->Insufficient(119, false);
        cbContinuousLoad_FT                 ->Enabled=fSecurity->Insufficient(119, false);
        cbContinuPassSkt_FT                 ->Enabled=fSecurity->Insufficient(119, false);
        cbContinuPassSkt_RT                 ->Enabled=fSecurity->Insufficient(119, false);
        cbContactCountRT                    ->Enabled=fSecurity->Insufficient(119, false);
        cbContinuousPass_RT                 ->Enabled=fSecurity->Insufficient(119, false);
        cbContinuousLoad_RT                 ->Enabled=fSecurity->Insufficient(119, false);

        edContinuousPass_FT                 ->Enabled=fSecurity->Insufficient(82, false);
        edContinuPassSkt_FT                 ->Enabled=fSecurity->Insufficient(82, false);
        edContinuousLoad_FT                 ->Enabled=fSecurity->Insufficient(82, false);
        edContactCountFT                    ->Enabled=fSecurity->Insufficient(82, false);
        cobContinuousPass_FT                ->Enabled=fSecurity->Insufficient(82, false);
        edContinuPassSkt_RT                 ->Enabled=fSecurity->Insufficient(82, false);
        edContinuousPass_RT                 ->Enabled=fSecurity->Insufficient(82, false);
        edContinuousLoad_RT                 ->Enabled=fSecurity->Insufficient(82, false);
        edContactCountRT                    ->Enabled=fSecurity->Insufficient(82, false);
        cobContinuousPass_RT                ->Enabled=fSecurity->Insufficient(82, false);

        rbContsFailBySocket_FTOn            ->Enabled=fSecurity->Insufficient(120, false);
        rbContsFailBySocket_FTOff           ->Enabled=fSecurity->Insufficient(120, false);
        rbContsFailByHead_FTOn              ->Enabled=fSecurity->Insufficient(120, false);
        rbContsFailByHead_FTOff             ->Enabled=fSecurity->Insufficient(120, false);
        rbContsFailBySocket_RTOn            ->Enabled=fSecurity->Insufficient(120, false);
        rbContsFailBySocket_RTOff           ->Enabled=fSecurity->Insufficient(120, false);
        rbContsFailByHead_RTOn              ->Enabled=fSecurity->Insufficient(120, false);
        rbContsFailByHead_RTOff             ->Enabled=fSecurity->Insufficient(120, false);

        edContsFailSocketAlarmCT_FT         ->Enabled=fSecurity->Insufficient(83, false);
        edContsFailHeadAlarmCT_FT           ->Enabled=fSecurity->Insufficient(83, false);
        edContsFailSocketAlarmCT_RT         ->Enabled=fSecurity->Insufficient(83, false);
        edContsFailHeadAlarmCT_RT           ->Enabled=fSecurity->Insufficient(83, false);
    }

    if(CosFunction.bYieldAlarm4)                                                //Sam 20191109 YieldAlarm4 Fix
    {
        cbAlarm4ContinueType                ->Enabled=fSecurity->Insufficient(121, false);
        cb_Alarm4EnableIntervalYield        ->Enabled=fSecurity->Insufficient(121, false);
        cb_SiteToSiteYieldEnable            ->Enabled=fSecurity->Insufficient(121, false);
        cb_HeadToHeadYieldEnable            ->Enabled=fSecurity->Insufficient(121, false);
        cb_SiteYieldOverAlert               ->Enabled=fSecurity->Insufficient(121, false);

        edAlarm4IntervalCount               ->Enabled=fSecurity->Insufficient(117, false);
        edAlarm4ContinueCount               ->Enabled=fSecurity->Insufficient(117, false);
        ed_Alarm4IntervalYieldIntervalCount ->Enabled=fSecurity->Insufficient(117, false);
        ed_Alarm4IntervalYieldContinueCount ->Enabled=fSecurity->Insufficient(117, false);
        ed_Alarm4IntervalYieldYield         ->Enabled=fSecurity->Insufficient(117, false);
        ed_SiteToSiteYield                  ->Enabled=fSecurity->Insufficient(117, false);
        ed_HeadToHeadYield                  ->Enabled=fSecurity->Insufficient(117, false);
        ed_SiteYieldOverAlert               ->Enabled=fSecurity->Insufficient(117, false);
        ed_SiteToSiteYieldCount             ->Enabled=fSecurity->Insufficient(117, false);
        ed_HeadToHeadYieldCount             ->Enabled=fSecurity->Insufficient(117, false);
        ed_SiteYieldOverAlertCount          ->Enabled=fSecurity->Insufficient(117, false);
    }

    if(CosFunction.bYieldAlarm5)                                                //Sam 20191109 YieldAlarm5 Fix
    {                                                                           //Sam 20171213 (Steven) : 超豐良率監控 //Sam 20180423 (wei) : MOFile of Yeild Download
        cbAlarm5_BySiteLowYieldEnable       ->Enabled=fSecurity->Insufficient(153, false);
        cbAlarm5_BySiteCmpYieldEnable       ->Enabled=fSecurity->Insufficient(153, false);
        cbAlarm5_BySiteAlarmYieldEnable     ->Enabled=fSecurity->Insufficient(153, false);
        cbAlarm5_BySitePreCmpYieldEnable    ->Enabled=fSecurity->Insufficient(153, false);
        edAlarm5_OSBin                      ->Enabled=false;
        edAlarm5_BySiteLowYield             ->Enabled=fSecurity->Insufficient(154, false);
        edAlarm5_BySiteLowYieldRej          ->Enabled=fSecurity->Insufficient(154, false);   //Sam 20180423 (wei) : MOFile of Yeild Download
        edAlarm5_BySiteAlarmYield           ->Enabled=fSecurity->Insufficient(154, false);
        edAlarm5_BySiteAlarmYieldRej        ->Enabled=fSecurity->Insufficient(154, false);   //Sam 20180423 (wei) : MOFile of Yeild Download
        edAlarm5_BySiteIntervalContactCnt   ->Enabled=fSecurity->Insufficient(154, false);
        edAlarm5_BySiteCmpYield             ->Enabled=fSecurity->Insufficient(154, false);
        edAlarm5_BySiteCmpYieldRej          ->Enabled=fSecurity->Insufficient(154, false);   //Sam 20180423 (wei) : MOFile of Yeild Download
        edAlarm5_BySitePreCmpYield          ->Enabled=fSecurity->Insufficient(154, false);   //Sam 20180423 (wei) : MOFile of Yeild Download
        edAlarm5_BySitePreCmpYieldRej       ->Enabled=fSecurity->Insufficient(154, false);   //Sam 20180423 (wei) : MOFile of Yeild Download
    }

    if(CUSTOMER_CODE==CC_ASE_CL)                                                //Alick 20160705 add ASE CL 啟用功能與計數權限追加
    {
        rbContsFailBySocket_FTOn    ->Enabled=fSecurity->Insufficient(125, false);
        rbContsFailBySocket_FTOff   ->Enabled=fSecurity->Insufficient(125, false);
        rbContsFailByHead_FTOn      ->Enabled=fSecurity->Insufficient(125, false);
        rbContsFailByHead_FTOff     ->Enabled=fSecurity->Insufficient(125, false);
        rbContsFailBySocket_RTOn    ->Enabled=fSecurity->Insufficient(125, false);
        rbContsFailBySocket_RTOff   ->Enabled=fSecurity->Insufficient(125, false);
        rbContsFailByHead_RTOn      ->Enabled=fSecurity->Insufficient(125, false);
        rbContsFailByHead_RTOff     ->Enabled=fSecurity->Insufficient(125, false);
        edContsFailSocketAlarmCT_FT ->Enabled=fSecurity->Insufficient(126, false);
        edContsFailHeadAlarmCT_FT   ->Enabled=fSecurity->Insufficient(126, false);
        edContsFailSocketAlarmCT_RT ->Enabled=fSecurity->Insufficient(126, false);
        edContsFailHeadAlarmCT_RT   ->Enabled=fSecurity->Insufficient(126, false);
    }

    if(IniConfig.bE53LowYieldAutoClean)                                         //kevin 20160802   //wei 20141201 Low Yield Auto Clean(%)
        gbLowYieldAutoClean->Visible=true;
    else
        gbLowYieldAutoClean->Visible=false;

    grpSpecialLowYield->Visible=CosFunction.bSpecailLowYeild;                   //Sam 20210505 : PTI 要求的兩段 Low Yeild

    if(IniConfig.bD67LoadCellMeasure)                                           //kevin 20190907 Arm 測區次數道量測 功能;
        gbLoadcell->Visible=true;
    else
        gbLoadcell->Visible=false;

    if(CosFunction.bYieldControlUseContactCount)                                //Steven 20141212 : Yield控制使用Contact Count
    {
        labLowYield1_FT             ->Caption="%  after contact count";
        labSiteYieldDifferent1_FT   ->Caption="%  after contact count";
        labSiteYieldCmp1_FT         ->Caption="%  after contact count";
        labLowYield1_RT             ->Caption="%  after contact count";
        labSiteYieldDifferent1_RT   ->Caption="%  after contact count";
        labSiteYieldCmp1_RT         ->Caption="%  after contact count";
        labLowYieldByTotal1_FT      ->Caption="%  after contact count";         //Kaichen 20190628 : Low Yield ByTotal
        labLowYieldByTotal1_RT      ->Caption="%  after contact count";         //Kaichen 20190628 : Low Yield ByTotal
        lblLowYieldByPicker_FT      ->Caption="%  after contact count";         //Steven 20230223 : 根據Index吸嘴比較良率
        lblLowYieldByPicker_RT      ->Caption="%  after contact count";
    }
    else if(CosFunction.bYieldControlUseEACount)                                //wei 20151111  //wei 20180606 Yield控制使用EA Count
    {
        labLowYield1_FT             ->Caption="%  Ignore IC count";
        labSiteYieldDifferent1_FT   ->Caption="%  Ignore IC count";
        labSiteYieldCmp1_FT         ->Caption="%  Ignore IC count";
        labLowYield1_RT             ->Caption="%  Ignore IC count";
        labSiteYieldDifferent1_RT   ->Caption="%  Ignore IC count";
        labSiteYieldCmp1_RT         ->Caption="%  Ignore IC count";
        labLowYieldByTotal1_FT      ->Caption="%  Ignore IC count";             //Kaichen 20190628 : Low Yield ByTotal
        labLowYieldByTotal1_RT      ->Caption="%  Ignore IC count";             //Kaichen 20190628 : Low Yield ByTotal
        lblLowYieldByPicker_FT      ->Caption="%  Ignore IC count";             //Steven 20230223 : 根據Index吸嘴比較良率
        lblLowYieldByPicker_RT      ->Caption="%  Ignore IC count";

    }
    else if(CUSTOMER_CODE==CC_JCET)                                             //JerryYang 20170420 (Steven) JCET吳如春要求修改名稱
    {
        labLowYield1_FT             ->Caption="%  Ignore IC count";
        labSiteYieldDifferent1_FT   ->Caption="%  after per site count";
        labSiteYieldCmp1_FT         ->Caption="%  after per site count";
        labLowYield1_RT             ->Caption="%  Ignore IC count";
        labSiteYieldDifferent1_RT   ->Caption="%  after per site count";
        labSiteYieldCmp1_RT         ->Caption="%  after per site count";
        lblLowYieldByPicker_FT      ->Caption="%  after per site count";        //Steven 20230223 : 根據Index吸嘴比較良率
        lblLowYieldByPicker_RT      ->Caption="%  after per site count";
    }
    else
    {
        labLowYield1_FT             ->Caption="%  after test count";
        labSiteYieldDifferent1_FT   ->Caption="%  after contact count";
        labSiteYieldCmp1_FT         ->Caption="%  after contact count";
        labLowYield1_RT             ->Caption="%  after test count";
        labSiteYieldDifferent1_RT   ->Caption="%  after contact count";
        labSiteYieldCmp1_RT         ->Caption="%  after contact count";
        lblLowYieldByPicker_FT      ->Caption="%  after contact count";         //Steven 20230223 : 根據Index吸嘴比較良率
        lblLowYieldByPicker_RT      ->Caption="%  after contact count";
    }

    if(CosFunction.bYieldAlarmNoWait1Min)                                       //wei 20150820  Yield Alarm No Wait 1Min
    {
        lblCheckInteval->Caption="";
        grpLowYield_FT              ->Caption="Low Yields%(By Site)";           //Jimmychiu 20230524 : 補回顯示內容
        grpSiteYieldDifferent_FT    ->Caption="By Arm Per Site Differ Yield%";
        grpSiteYieldCmp_FT          ->Caption="By Site Compare Yield%";
        grpLowYield_RT              ->Caption="Low Yields%(By Site)";   //Ifor 20171117 : FT => RT
        grpSiteYieldDifferent_RT    ->Caption="By Arm Per Site Differ Yield%";
        grpSiteYieldCmp_RT          ->Caption="By Site Compare Yield%";
    }
    else                                                                        //Steven 20230223 : 整理Yield顯示畫面
    {
        if(CosFunction.bLowYieldAlarmIntervalTimeBySetting &&
           IniConfig.bI44_LowYieldAlarmIntervalTimeBySetting)
        {
            Str.sprintf("Check Interval : %d Sec.", IniConfig.iI44_LowYieldAlarmIntervalTime);
        }
        else
        {
            Str.sprintf("Check Interval : 1 Min.", IniConfig.iI44_LowYieldAlarmIntervalTime);
        }

        lblCheckInteval->Caption=Str;
        grpLowYield_FT              ->Caption="Low Yields% (1min)";             //Jimmychiu 20230524 : 補回顯示內容
        grpSiteYieldDifferent_FT    ->Caption="By Arm Per Site Differ Yield% (1min)";
        grpSiteYieldCmp_FT          ->Caption="By Site Compare Yield% (1min)";
        grpLowYield_RT              ->Caption="Low Yields% (1min)";             //Ifor 20171117 : FT => RT
        grpSiteYieldDifferent_RT    ->Caption="By Arm Per Site Differ Yield% (1min)";
        grpSiteYieldCmp_RT          ->Caption="By Site Compare Yield% (1min)";
    }

    grpLowYieldByTotal_FT->Visible=(CosFunction.bLowYeildByTotal);              //wei 20151116 Low Yield By Total
    grpLowYieldByTotal_RT->Visible=(CosFunction.bLowYeildByTotal);              //wei 20151116 Low Yield By Total
    palConsFailIgnore_FT ->Visible=(CUSTOMER_CODE==CC_KYEC_CHEN);               //wei 20160115 銅鑼前幾顆不計算ContsFail
    palConsFailIgnore_RT ->Visible=(CUSTOMER_CODE==CC_KYEC_CHEN);               //wei 20160115 銅鑼前幾顆不計算ContsFail
    pgcBySiteByBinPercentCompare_FT->Visible=(CosFunction.bBySiteByBinPercentCompare==true && CosFunction.bByBinAlarmFromYieldForm);   //jou 20180113 (Steven) : By Site By Bin Percent Compare From Yield form
    pgcBySiteByBinPercentCompare_RT->Visible=(CosFunction.bBySiteByBinPercentCompare==true && CosFunction.bByBinAlarmFromYieldForm);   //jou 20180113 (Steven) : By Site By Bin Percent Compare From Yield form

    if(IniConfig.bSIGURDFunction)                                               //KaiChen 20190626 ：矽格要求 LowYieldByTotal & SiteYieldCmp 開關同步
    {
        cbSiteYieldCmp_FT->Enabled=false;
        cbSiteYieldCmp_RT->Enabled=false;
        if(CUSTOMER_CODE!=CC_UTAC_TW)                                           //Sam 20230106 :　聯測孝文要求可以編輯數量
        {
            edSiteYieldCmpIg_FT->Enabled=false;
            edSiteYieldCmpIg_RT->Enabled=false;
        }
    }

    if(bUseTwoArm32Site==true)                                                  //Steven 20200526 : 4x4 & 4x8 不需要比by arm
    {
        grpSiteYieldDifferent_FT->Visible=false;
        grpSiteYieldDifferent_RT->Visible=false;
    }
    else
    {
        grpSiteYieldDifferent_FT->Visible=true;
        grpSiteYieldDifferent_RT->Visible=true;
    }

    if(CUSTOMER_CODE==CC_SIGURD_HUKOU ||
       CUSTOMER_CODE==CC_SIGURD_SUZHOU)                                         //KaiChen 20200304 ：矽格-湖口，關閉 Low Yields%、By Arm Per Site Differ Yield%
    {
        grpSiteYieldDifferent_FT->Visible=false;
        grpSiteYieldDifferent_RT->Visible=false;
        grpLowYield_FT->Visible=false;                                          //Steven 20230224 : Add
        grpLowYield_RT->Visible=false;

        cbAllSiteFail       ->Visible=false;                                    //KaiChen 20200603 ：矽格-湖口，Yield 頁面 AllSiteFail 不使用
        edAllSiteFailCount  ->Visible=false;                                    //KaiChen 20200603 ：矽格-湖口，Yield 頁面 AllSiteFail 不使用
        edAllSiteFailCountRT->Visible=false;
        cbAllSiteFail_RT    ->Visible=false;                                    //KaiChen 20200603 ：矽格-湖口，Yield 頁面 AllSiteFail 不使用
        gbPiggyBack     ->Visible=false;                                        //KaiChen 20201130 ：矽格-湖口，Yield 頁面 Piggy-Back Functions 不顯示
        gbPiggyBackRT   ->Visible=false;                                        //KaiChen 20201130 ：矽格-湖口，Yield 頁面 Piggy-Back Functions 不顯示
    }

    palIntervalLowYeildBySite_FT->Visible =(CosFunction.IntervalYieldCount && CUSTOMER_CODE!=CC_TSI);      //wei 20180606 Interval Low Yield By Site
    palIntervalLowYeildBySite_RT->Visible =(CosFunction.IntervalYieldCount && CUSTOMER_CODE!=CC_TSI);      //wei 20180606 Interval Low Yield By Site
    palIntervalLowYeildByTotal_FT->Visible=(CosFunction.IntervalYieldCount);    //wei 20180606 Interval Low Yield By Total
    palIntervalLowYeildByTotal_RT->Visible=(CosFunction.IntervalYieldCount);    //wei 20180606 Interval Low Yield By Total

    grpCreateManualEOCAP->Visible=CosFunction.bCreateManualEOCAP;               //jou 20221104 : VTest CreateManualEOCAP function;

    fShow=true;
    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);                                  //Steven 20100629
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::btnApplyClick(TObject *Sender)
{
    if(IniConfig.bA02DisableSaveParsWhenSwitchToOp==true &&                     //RogerYang 20260305 張寧說時間到都要登出，且不可被更改任何設定(A01_2)
        AccessLevel==0)
    {
        ShowMyMessage("[A01_2]目前已切換到Operator權限，\r\n請重新登入再做設定!");
        Close();
    }

    AnsiString S="";
    if(bGPIBChangeAlarm==false)                                                 //wei 20151127 GPIB Change Alarm
        DoFormToData();
    CheckSettingNo();                                                           //Steven 20110506

    if(CUSTOMER_CODE==CC_ASE_KaohSiung &&
       LastSet.iRunStartMode==rsmContinuRetest_ART &&
       bART_needRT2==false)                                                     //kevin 20150717    add
    {
        TestIF_File.bContsFailBySocket=false;                                   //kevin 20150812 ART_分tray不卡 連續icfail
        TestIF_File.bContsFailByHead=false;
    }

    SaveSetupFile(GetRecipePath(), GetLastOpenFN());                            //kevin 20150105 Start   另存 jobfile
    #ifdef ASE_KaohSiung
       fBuilder->bSaveAsJobFile(S, "JOBFILE");
    #endif

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //kevin 20150812  add
    {
        ReadFile();                                                             //kevin 20150812  add
        DoIniDataToForm();                                                      //kevin 20150812 add
    }

    if(CosFunction.bPiggybackFunctionByHandler==true)                           //Isaac 20170712 :Piggyback function By Handlder(save file to config.ini)
    {
        SaveSetupFileToConfig(AuthPath, "config.ini");
    }

    if(CosFunction.bRecipeParameterDefault)                                     //Sam 20201209 : Default Recipe ChangeLog
    {
        fRPDefault->RP_Name->Clear()    ;
        fRPDefault->RP_Default->Clear() ;
        fRPDefault->RP_Value->Clear()   ;

        if(CosFunction.bRecipeParameterDefaultLog)
        {
            fSpeed->SearchRecipeParameter(fSpeed->tsAllSpeed, -1);              //Isaac 20171227 (Steven) : AutoSpeed RPD一鍵復原
            fSpeed->SearchRecipeParameter(fSpeed->tsIndex, 0);
            fSpeed->SearchRecipeParameter(fSpeed->tsInArm, 1);
            fSpeed->SearchRecipeParameter(fSpeed->tsOutArm, 2);
            fSpeed->SearchRecipeParameter(fSpeed->tsTrayArm, 3);
            fSpeed->SearchRecipeParameter(fSpeed->tsShuttle, 4);
            fCleaning->SearchRecipeParameter(fCleaning, 0);
            FTestIF->SearchRecipeParameter(FTestIF, 0);
        }
        SearchRecipeParameter(fYieldMonitoring, 0);
        fRPDefault->CompareRPDefaultAndValue(fRPDefault->RP_Name, fRPDefault->RP_Default, fRPDefault->RP_Value);
    }

    btnApply->Enabled=false;
    fMain->BackupSetupFile();                                                   //Ifor 20170620 (wei) add Auto BackUp Setup File & Last Data

    if(IniConfig.bSIGURDFunction &&
       fLotInfo->pgLotinfo->ActivePage==fLotInfo->tsYieldMonitior)              //Sam 20210331 : 北興新增 Yield Monitor
    {
        fLotInfo->RefreshYieldMonitor();
    }

    if(IniConfig.bEnable_SECS_GEM==true)
    {
        bHasSaveSet=true;                                                       //Ifor 20151204 新增判斷機台有無修改設定檔
        EventReport(SECS_EVENT.SaveRecipe);
    }
    fLotInfo->RefreshYieldMonitor();                                            //Sam 20210916 : 新增 Yiled Monitor 到畫面上
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::btnOkClick(TObject *Sender)
{                                                                               //ChungHung add 20120730 若其中一個Item 都沒選 則秀OFF 修正顯示未即時更新
    ReadFile();
    fTemperFrom->ShowYieldFuntion();
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::rgPiggyBack_FTClick(TObject *Sender)
{
    btnApply->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::edContactCountFTMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Barcode_Reader(bcYield)==0)                                              // 20140103 wei KYEC Barcode Reader
    {
        return;
    }
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, iMinCount, iMaxCount);
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::edLowYield_RTMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Barcode_Reader(bcYield)==0)                                              // 20140103 wei KYEC Barcode Reader
    {
        return;
    }

    if(CosFunction.bYieldAlarmUseDouble)                                        //Steven 20170513 (jou) : 不同設定顯示要不一樣
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, dMinYield, dMaxYield);//kevin 20150630 int-> float
    else
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, iMinYield, iMaxYield);
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::edContactCountFTChange(TObject *Sender)
{
    if(IniConfig.bSIGURDFunction &&                                             //KaiChen 20190626 ：矽格要求 LowYieldByTotal & SiteYieldCmp 開關同步
       CUSTOMER_CODE!=CC_UTAC_TW)                                               //Sam 20230106 :　聯測孝文要求可以編輯數量
    {
        TEdit *TempEdit=(TEdit *)Sender;
        if(TempEdit->Name=="edLowYieldByTotalIg_FT")
        {
            edSiteYieldCmpIg_FT->Text=edLowYieldByTotalIg_FT->Text;
        }
        if(TempEdit->Name=="edLowYieldByTotalIg_RT")
        {
            edSiteYieldCmpIg_RT->Text=edLowYieldByTotalIg_RT->Text;
        }
    }

    btnApply->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::edContsFailSocketAlarmCT_FTMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Barcode_Reader(bcYield)==0)                                              // 20140103 wei KYEC Barcode Reader
    {
        return;
    }

    if(IniConfig.bFTContinueON)                                                 //kevin 20121008 FT MODE 不能關連續FAIL
    {
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, (int)IniConfig.iFTMAXValue, 1);       //Steven 20141120 : Modify
    }
    else
    {
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, iMinCount, iMaxCount); //Steven 20141120 : Modify
    }
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::edQAModeMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 10000, 50);  //Steven 20141120 : Modify
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::CheckSettingNo()                             //Steven 20110506 Start: 加入範圍保護
{
//    TestIF_File.iIgnoreIC                   =CheckRange(int(TestIF_File.iIgnoreIC),                    100000, 1);
    TestIF_File.iContinuousPassBin          =CheckRange(int(TestIF_File.iContinuousPassBin),           16, 1);
    TestIF_File.iContinuousPassBin_RT       =CheckRange(int(TestIF_File.iContinuousPassBin_RT),        16, 1);
    TestIF_File.iContinuousPassBinCount     =CheckRange(int(TestIF_File.iContinuousPassBinCount),      iMinCount, iMaxCount);
    TestIF_File.iContinuousPassBinCount_RT  =CheckRange(int(TestIF_File.iContinuousPassBinCount_RT),   iMinCount, iMaxCount);
    TestIF_File.iContinuousLoaderCount      =CheckRange(int(TestIF_File.iContinuousLoaderCount),       iMinCount, iMaxCount);
    TestIF_File.iContinuousLoaderCount_RT   =CheckRange(int(TestIF_File.iContinuousLoaderCount_RT),    iMinCount, iMaxCount);
    TestIF_File.iContinuousContactCount     =CheckRange(int(TestIF_File.iContinuousContactCount),      iMinCount, iMaxCount);
    TestIF_File.iContinuousContactCount_RT  =CheckRange(int(TestIF_File.iContinuousContactCount_RT),   iMinCount, iMaxCount);
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::rgQARunModeClick(TObject *Sender)
{
    btnApply->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    fShow=false;
    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);                                  //Steven 20100629
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::edContsFailSocketAlarmCT_RTMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Barcode_Reader(bcYield)==0)                                              // 20140103 wei KYEC Barcode Reader
    {
        return;
    }
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, iMinCount, iMaxCount);     //Steven 20141120 : Modify
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::edContsFailHeadAlarmCT_FTMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Barcode_Reader(bcYield)==0)                                              // 20140103 wei KYEC Barcode Reader
    {
        return;
    }
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, iMinCount, iMaxCount);
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::edContsFailHeadAlarmCT_RTMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Barcode_Reader(bcYield)==0)                                              // 20140103 wei KYEC Barcode Reader
    {
        return;
    }
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, iMinCount, iMaxCount);
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::rgBinAlarmByClick(TObject *Sender)
{
    btnApply->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::cbLowYield_FTMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Barcode_Reader(bcYield)==0)                                              // 20140103 wei KYEC Barcode Reader
    {
        cbLowYield_FT->Checked=TestIF_File.bFailAlarmLowYield;
        return;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::cbSiteYieldDifferent_FTMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Barcode_Reader(bcYield)==0)                                              // 20140103 wei KYEC Barcode Reader
    {
        cbSiteYieldDifferent_FT->Checked=TestIF_File.bFailAlarmSiteYieldDifferent;
        return;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::rbContsFailBySocket_FTOnMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Barcode_Reader(bcYield)==0)                                              // 20140103 wei KYEC Barcode Reader
    {
        rbContsFailBySocket_FTOn ->Checked=TestIF_File.bContsFailBySocket;
        rbContsFailBySocket_FTOff->Checked=!TestIF_File.bContsFailBySocket;
        return;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::rbContsFailByHead_FTOnMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Barcode_Reader(bcYield)==0)                                              // 20140103 wei KYEC Barcode Reader
    {
        rbContsFailByHead_FTOn->Checked=TestIF_File.bContsFailByHead;
        rbContsFailByHead_FTOff->Checked=!TestIF_File.bContsFailByHead;
        return;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::rbContsFailBySocket_RTOnMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Barcode_Reader(bcYield)==0)                                              // 20140103 wei KYEC Barcode Reader
    {
        rbContsFailBySocket_RTOn ->Checked=TestIF_File.bContsFailBySocket_RT;
        rbContsFailBySocket_RTOff->Checked=!TestIF_File.bContsFailBySocket_RT;
        return;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::rbContsFailByHead_RTOnMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Barcode_Reader(bcYield)==0)                                              // 20140103 wei KYEC Barcode Reader
    {
        rbContsFailByHead_RTOn ->Checked=TestIF_File.bContsFailByHead_RT;
        rbContsFailByHead_RTOff->Checked=!TestIF_File.bContsFailByHead_RT;
        return;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::cbLowYield_RTMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Barcode_Reader(bcYield)==0)                                              // 20140103 wei KYEC Barcode Reader
    {
        cbLowYield_RT->Checked=TestIF_File.bFailAlarmLowYield_RT;
        return;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::cbSiteYieldDifferent_RTMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Barcode_Reader(bcYield)==0)                                              // 20140103 wei KYEC Barcode Reader
    {
        cbSiteYieldDifferent_RT->Checked=TestIF_File.bFailAlarmSiteYieldDifferent_RT;
        return;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::edAutoCleanLowYieldLimitMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Barcode_Reader(bcYield)==0)                                              // 20140103 wei KYEC Barcode Reader
    {
        return;
    }
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 49, 1);      //Steven 20141120 : Modify //kevin 201503606 50->49
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::mtBinSelectYieldMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    TTMyTray *Ptr;
    Ptr=(TTMyTray *)Sender;
    int tag=Ptr->Tag;
    MyYieldPanel[tag]->mtBinSelectYield->ConvertIndexCells(X, Y);               //畫面座標位置換算出  陣列 xy
    btnApply->Enabled=true;                                                     //SAVE
    if(Y==ePassYield)                                                           //pass
    {
        if(MyYieldPanel[tag]->bOpenShort[X]==false)                             //kevin 20150706   fail bin互斥
           MyYieldPanel[tag]->bPass[X]=!MyYieldPanel[tag]->bPass[X];
    }
    else if(Y==eOpenShort)                                                      //OPEN /SHORT
    {
        if(MyYieldPanel[tag]->bPass[X]==false && Prod.bIsPassBin[X]==false)     //kevin 20150706 pass bin  互斥 不是設定pass bin
            MyYieldPanel[tag]->bOpenShort[X]=!MyYieldPanel[tag]->bOpenShort[X];
    }

    InitDataToEdit(tag);
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::mtTrayNameMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TTMyTray *Ptr;
    Ptr=(TTMyTray *)Sender;
    int tag=Ptr->Tag;

    MyYieldPanel[tag]->mtTrayName->ConvertIndexCells(X, Y);

    btnApply->Enabled=true;                                                     //SAVE

    if(X==eItemeEnable)                                                         //Enable
    {
        if(Y==1)
            MyYieldPanel[tag]->bEnablePassYieldART=!MyYieldPanel[tag]->bEnablePassYieldART;
        else if(Y==2)
            MyYieldPanel[tag]->bEnableOpenShortART=!MyYieldPanel[tag]->bEnableOpenShortART;
        else if(Y==3)
            MyYieldPanel[tag]->bEnableRecoverART=!MyYieldPanel[tag]->bEnableRecoverART;
    }
    else
    {
        if(Y==ePassYield)                                                       // pass Yield
        {
            if(MyYieldPanel[tag]->bEnablePassYieldART)
            {
                edInput->Text=AnsiString(MyYieldPanel[tag]->fPassYieldART);
                fQwertyKey->ShowQwertyKey(edInput, N_DOUBLE, 2, true, dMinYield, dMaxYield);
                MyYieldPanel[tag]->fPassYieldART=atof(edInput->Text.c_str());
            }
            else
            {
                MyYieldPanel[tag]->fPassYieldART=0.00;
            }
        }
        else if(Y==eOpenShort)                                                  // Open/short Yield
        {
            if(MyYieldPanel[tag]->bEnableOpenShortART)
            {
                edInput->Text=AnsiString(MyYieldPanel[tag]->fOpenShortYieldART);
                fQwertyKey->ShowQwertyKey(edInput, N_DOUBLE, 2, true, dMinYield, dMaxYield);
                MyYieldPanel[tag]->fOpenShortYieldART=atof(edInput->Text.c_str());
            }
            else
            {
                MyYieldPanel[tag]->fOpenShortYieldART=0.00;
            }
        }
        else                                                                    // Recover Yield
        {
            if(MyYieldPanel[tag]->bEnableRecoverART)
            {
                edInput->Text=AnsiString(MyYieldPanel[tag]->fRecoverYieldART);
                fQwertyKey->ShowQwertyKey(edInput, N_DOUBLE, 2, true, dMinYield, dMaxYield);
                MyYieldPanel[tag]->fRecoverYieldART=atof(edInput->Text.c_str());
            }
            else
            {
                MyYieldPanel[tag]->fRecoverYieldART=0.00;
            }
        }
    }

    InitDataToEdit(tag);
}
//---------------------------------------------------------------------------
// 轉成畫面 V
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::InitDataToEdit(int tag)
{
    int i;
    AnsiString Str="", Temp="";
    tag=0;
    /*
    ePassYield       =1,         //設定pass bin
    eOpenShort       =2,         //設定open short bin
    eRecover         =3,
    eBinSetting
    */
    MyYieldPanel[tag]->mtTrayName->SetCellNumber     (eItemeEnable, ePassYield, (MyYieldPanel[tag]->bEnablePassYieldART)?"V":"");
    MyYieldPanel[tag]->mtTrayName->SetCellColorIndex (eItemeEnable, ePassYield, (MyYieldPanel[tag]->bEnablePassYieldART)?eCLOlive:eCLBtnFace);

    MyYieldPanel[tag]->mtTrayName->SetCellNumber     (eItemeEnable, eOpenShort, (MyYieldPanel[tag]->bEnableOpenShortART)?"V":"");
    MyYieldPanel[tag]->mtTrayName->SetCellColorIndex (eItemeEnable, eOpenShort, (MyYieldPanel[tag]->bEnableOpenShortART)?eCLOlive:eCLBtnFace);

    MyYieldPanel[tag]->mtTrayName->SetCellNumber     (eItemeEnable, eRecover, (MyYieldPanel[tag]->bEnableRecoverART)?"V":"");
    MyYieldPanel[tag]->mtTrayName->SetCellColorIndex (eItemeEnable, eRecover, (MyYieldPanel[tag]->bEnableRecoverART)?eCLOlive:eCLBtnFace);

    if(MyYieldPanel[tag]->bEnablePassYieldART)
    {
        MyYieldPanel[tag]->mtTrayName->SetCellNumber     (eItemeYield, ePassYield,MyYieldPanel[tag]->fPassYieldART);
        MyYieldPanel[tag]->mtTrayName->SetCellColorIndex (eItemeYield, ePassYield, eCLWhite);
    }
    else
    {
        MyYieldPanel[tag]->mtTrayName->SetCellNumber     (eItemeYield, ePassYield, "");
        MyYieldPanel[tag]->mtTrayName->SetCellColorIndex (eItemeYield, ePassYield, eCLBtnFace);
    }
    if(MyYieldPanel[tag]->bEnableOpenShortART)
    {
        MyYieldPanel[tag]->mtTrayName->SetCellNumber     (eItemeYield, eOpenShort,MyYieldPanel[tag]->fOpenShortYieldART);
        MyYieldPanel[tag]->mtTrayName->SetCellColorIndex (eItemeYield, eOpenShort, eCLWhite);
    }
    else
    {
        MyYieldPanel[tag]->mtTrayName->SetCellNumber     (eItemeYield, eOpenShort, "");
        MyYieldPanel[tag]->mtTrayName->SetCellColorIndex (eItemeYield, eOpenShort, eCLBtnFace);
    }
    if(MyYieldPanel[tag]->bEnableRecoverART)
    {
        MyYieldPanel[tag]->mtTrayName->SetCellNumber     (eItemeYield, eRecover,MyYieldPanel[tag]->fRecoverYieldART);
        MyYieldPanel[tag]->mtTrayName->SetCellColorIndex (eItemeYield, eRecover, eCLWhite);
    }
    else
    {
        MyYieldPanel[tag]->mtTrayName->SetCellNumber     (eItemeYield, eRecover, "");
        MyYieldPanel[tag]->mtTrayName->SetCellColorIndex (eItemeYield, eRecover, eCLBtnFace);
    }

//------------  mtBinSelectYield
    for(int i=0; i<iTestBinCount; i++)                                          //Steven 20251104 : iBinCount --> iTestBinCount
    {
        MyYieldPanel[tag]->mtBinSelectYield->SetCellNumber     (i, ePassYield, (MyYieldPanel[tag]->bPass[i])?"V":"");
        MyYieldPanel[tag]->mtBinSelectYield->SetCellColorIndex (i, ePassYield, (MyYieldPanel[tag]->bPass[i])?eCLRed:eCLWhite);

        MyYieldPanel[tag]->mtBinSelectYield->SetCellNumber     (i, eOpenShort, (MyYieldPanel[tag]->bOpenShort[i])?"V":"");
        MyYieldPanel[tag]->mtBinSelectYield->SetCellColorIndex (i, eOpenShort, (MyYieldPanel[tag]->bOpenShort[i])?eCLRed:eCLWhite);
        MyYieldPanel[tag]->mtBinSelectYield->SetCellNumber     (i, eRecover, "");
        MyYieldPanel[tag]->mtBinSelectYield->SetCellColorIndex (i, eRecover, eCLGray);
    }
}
//------------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::edLowYieldIg_FTMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Barcode_Reader(bcYield)==0)                                              // 20140103 wei KYEC Barcode Reader
    {
        return;
    }

    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, iMinCount, iMaxCount);
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::edAutoRetestLimitFileMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    if(Barcode_Reader(bcYield)==0)                                              // 20140103 wei KYEC Barcode Reader
    {
        return;
    }
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 20, 1);
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::edFailYieldRate_ARTFTFileKeyPress(
      TObject *Sender, char &Key)
{
    if(OnlyNumberInPut(Key)==false)
        Key=NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::edFailYieldRate_ARTFTFileMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    if(Barcode_Reader(bcYield)==0)                                              // 20140103 wei KYEC Barcode Reader
    {
        return;
    }

   fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, dMinYield, dMaxYield);   //kevin 20150630 int-> float
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::edContsFailIgnore_FTMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Barcode_Reader(bcYield)==0)                                              // 20140103 wei KYEC Barcode Reader
    {
        return;
    }
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, iMinCount, iMaxCount);     //Steven 20141120 : Modify
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::edAutoLeastRetestLimitFileMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    if(Barcode_Reader(bcYield)==0)                                              // 20140103 wei KYEC Barcode Reader
    {
        return;
    }
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, atoi(edAutoRetestLimitFile->Text.c_str()), 1);   //kevin 20150630 int-> float
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::edFailYieldMaxRate_ARTFTFileMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    if(Barcode_Reader(bcYield)==0)                                              // 20140103 wei KYEC Barcode Reader
    {
        return;
    }
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 100.0, atof(edFailYieldMinRate_ARTFTFile->Text.c_str()));   //kevin 20150630 int-> float
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::edFailYieldMaxRate_ARTRTFileMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    if(Barcode_Reader(bcYield)==0)                                              // 20140103 wei KYEC Barcode Reader
    {
        return;
    }
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 100.0, atof(edFailYieldMinRate_ARTRTFile->Text.c_str()));   //kevin 20150630 int-> float
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::Timer1Timer(TObject *Sender)
{
    if(InitialOK==false)
        return;

    rgFT_ART->Visible=!(ckUseLeastRetestTimes->Checked);
    fLotInfo->palFTRateLmt->Visible=(CUSTOMER_CODE==CC_KYEC_LEE && ckUseLeastRetestTimes->Checked==false);
    fLotInfo->palRTRateLmt->Visible=(CUSTOMER_CODE==CC_KYEC_LEE && ckUseLeastRetestTimes->Checked==false);
    fLotInfo->Label46->Visible=(CUSTOMER_CODE==CC_KYEC_LEE);
    fLotInfo->Edit16->Visible=(CUSTOMER_CODE==CC_KYEC_LEE);
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::edAlarm4ContinueCountMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    if(Barcode_Reader(bcYield)==0)                                              // 20140103 wei KYEC Barcode Reader
    {
        return;
    }
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, iMinYield, iMaxYield);
}
//---------------------------------------------------------------------------
void TfYieldMonitoring::CalculateSiteYield()
{
    double dYield=0, dSiteCount=0;

    if(TestIF_File.iAutoClean_Function &&
       (TestIF_File.iAutoClean_Mode&M_SOCKET_ALARM ||
        TestIF_File.iAutoClean_Mode&M_INTERVAL))                                //ChungHung 20131223 add for SCK
    {
        fLotInfo->Label17->Caption = "User set : " + AnsiString(TestIF.iAutoClean_LowYieldLimit) + "%" +
                                    (TestIF.bAutoClean_FailAlarmLowYield?" Enable":" Disable");
        fLotInfo->Label18->Caption =  "User set : " + AnsiString(TestIF.iAutoClean_FailAlarmSiteYield) + "%" +
                                    (TestIF.bAutoClean_FailAlarmSiteYieldDifferent?" Enable":" Disable");
        fLotInfo->Label21->Caption = "User set : " + AnsiString(TestIF.iAutoClean_IntervalContact) +
                                    (TestIF_File.iAutoClean_Mode & M_INTERVAL?"/Contact Enable":"/Contact Disable");
        fLotInfo->edtAutoCleanLowYield->Text = AnsiString(fContactCT->GetLowYield_AutoClean(0));
        fLotInfo->edtAutoCleanSiteYieldDiff->Text = AnsiString(fContactCT->GetLowYield_AutoClean(1));
    }
    else
    {
        iAutoClean_FailAlarmSiteYieldIntervalCount=0;
        fContactCT->ClearData_AutoClean();                                      //ChungHung 20131225 add for SCK
        fLotInfo->Label17->Caption = "User set : " + AnsiString(TestIF.iAutoClean_LowYieldLimit) + "%" + " Disable";
        fLotInfo->Label18->Caption = "User set : " + AnsiString(TestIF.iAutoClean_FailAlarmSiteYield) + "%" + " Disable";
        fLotInfo->Label21->Caption = "User set : " + AnsiString(TestIF.iAutoClean_IntervalContact) + " Disable";
        fLotInfo->edtAutoCleanLowYield->Text = AnsiString(fContactCT->GetLowYield_AutoClean(0));
        fLotInfo->edtAutoCleanSiteYieldDiff->Text = AnsiString(fContactCT->GetLowYield_AutoClean(1));
    }

    if(SystemStart &&
       fContact->fShow==false &&
       iHome==0 &&
       bRunAutoClean==false)
    {                                                                           //KEVIN 20130710 Site Differ Yield% (After 1 min)
        if(IsNNMode()==NN_2Row)                                                 //Steven 20220418 : NN mode Yield alarm
        {
            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(Prod.bLowYieldAlarmByBin)                                //Steven 20140828 : By Bin Yield Monitor
                    {
                        dIndexZ1Yield=(LastSet.bUseTestSocket[0][i+2][j])?ArmData[0]->ArmSKET[i][j]->GetByBinSiteYieldPassPCA():0.0;
                        dIndexZ2Yield=(LastSet.bUseTestSocket[0][i  ][j])?ArmData[1]->ArmSKET[i][j]->GetByBinSiteYieldPassPCA():0.0;      //Sam 20250401 : 修正Yield問題
                    }
                    else
                    {
                        dIndexZ1Yield=(LastSet.bUseTestSocket[0][i+2][j])?ArmData[0]->ArmSKET[i][j]->GetBySitePCA():0.0;
                        dIndexZ2Yield=(LastSet.bUseTestSocket[0][i  ][j])?ArmData[1]->ArmSKET[i][j]->GetBySitePCA():0.0;    //Sam 20250401 : 修正Yield問題
                    }
                    dPickerYield[0][i][j]=dIndexZ1Yield;                        //Steven 20230223 : 根據Index吸嘴比較良率
                    dPickerYield[1][i][j]=dIndexZ2Yield;
                    dSiteYield[i+2][j]=dIndexZ1Yield;                           //Steven 20220419 : fixed for nn mode
                    dSiteYield[i  ][j]=dIndexZ2Yield;
                }
            }
        }
        else if(IsNNMode()==NN_1Row)
        {
            for(int j=0; j<FTestSuck.iShtCol; j++)
            {
                if(Prod.bLowYieldAlarmByBin)                                    //Steven 20140828 : By Bin Yield Monitor
                {
                    dIndexZ1Yield=(LastSet.bUseTestSocket[0][0][j])?ArmData[0]->ArmSKET[0][j]->GetByBinArmYieldPassPCA():0.0;   //Steven 20260316 : 修正ByArm關site的Yield計算j+1 --> j
                    dIndexZ2Yield=(LastSet.bUseTestSocket[1][0][j])?ArmData[1]->ArmSKET[0][j]->GetByBinArmYieldPassPCA():0.0;
                }
                else
                {
                    dIndexZ1Yield=(LastSet.bUseTestSocket[0][0][j])?ArmData[0]->ArmSKET[0][j]->GetBySitePCA():0.0;
                    dIndexZ2Yield=(LastSet.bUseTestSocket[1][0][j])?ArmData[1]->ArmSKET[0][j]->GetBySitePCA():0.0;
                }
                dSiteYield[1][j]=dIndexZ1Yield;
                dSiteYield[0][j]=dIndexZ2Yield;
                dPickerYield[0][0][j]=dIndexZ1Yield;                            //Steven 20230223 : 根據Index吸嘴比較良率
                dPickerYield[1][0][j]=dIndexZ2Yield;
                dPickerYield[0][1][j]=0.0;
                dPickerYield[1][1][j]=0.0;
            }
        }
        else
        {
            for(int i=0; i<TestSocket.iShtRow; i++)
            {
                for(int j=0; j<TestSocket.iShtCol; j++)
                {
                    if(LastSet.bUseTestSocket[0][i][j] ||                       //Isaac 20210630 : 修正關arm會失效，&&->||
                       LastSet.bUseTestSocket[1][i][j])
                    {
                        if(Prod.bLowYieldAlarmByBin)                            //Steven 20140828 : By Bin Yield Monitor
                        {
                            dIndexZ1Yield=(LastSet.bUseTestSocket[0][i][j])?ArmData[0]->ArmSKET[i][j]->GetByBinSiteYieldPassPCA():0.0;
                            dIndexZ2Yield=(LastSet.bUseTestSocket[1][i][j])?ArmData[1]->ArmSKET[i][j]->GetByBinSiteYieldPassPCA():0.0;
                        }
                        else
                        {
                            dIndexZ1Yield=(LastSet.bUseTestSocket[0][i][j])?ArmData[0]->ArmSKET[i][j]->GetBySitePCA():0.0; //kevin 20130710 by Site 計數
                            dIndexZ2Yield=(LastSet.bUseTestSocket[1][i][j])?ArmData[1]->ArmSKET[i][j]->GetBySitePCA():0.0; //Steven 20230224 : 加上開關Site計算
                        }

                        dPickerYield[0][i][j]=dIndexZ1Yield;                    //Steven 20230223 : 根據Index吸嘴比較良率
                        dPickerYield[1][i][j]=dIndexZ2Yield;

                        if(TestIF.iShuttleMode==0)                              //jou 2014-08-14 Site Compare Low Yield alarm
                        {
                            dYield=0;
                            dSiteCount=0;
                            if(LastSet.bUseTestSocket[0][i][j])                 //Steven 20230224 : 修正ByArm關site的Yield計算
                            {
                                dYield+=dIndexZ1Yield;
                                dSiteCount++;
                            }

                            if(LastSet.bUseTestSocket[1][i][j])
                            {
                                dYield+=dIndexZ2Yield;
                                dSiteCount++;
                            }

                            if(dSiteCount==0)
                                dSiteYield[i][j]=0;
                            else
                                dSiteYield[i][j]=dYield/dSiteCount;
                        }
                        else if(TestIF.iShuttle_Sel==0)
                        {
                            dSiteYield[i][j]=dIndexZ1Yield;
                        }
                        else
                        {
                            dSiteYield[i][j]=dIndexZ2Yield;
                        }
                    }
                    else
                    {
                        dSiteYield[i][j]=0.0;
                        dPickerYield[0][i][j]=0.0;                              //Steven 20230223 : 根據Index吸嘴比較良率
                        dPickerYield[1][i][j]=0.0;
                    }
                }
            }
        }
    }
    bFirstCount=false;
}
//---------------------------------------------------------------------------
void TfYieldMonitoring::CheckBySiteYieldAlarm()                                 //Site Yield Alarm(%)
{
    static int iCount2=0;

    int sum=0;
    int iSiteCount=0, ret=0;
    int iAlarmTimeInterval=(CosFunction.bLowYieldAlarmIntervalTimeBySetting &&  //JimmyChiu 20220601 : 修改low yield報警時間邏輯，報警間隔時間固定1min改為可以自行設定報警間隔時間
                            IniConfig.bI44_LowYieldAlarmIntervalTimeBySetting)?IniConfig.iI44_LowYieldAlarmIntervalTime:60;

    bool bYieldDiffOver=false, bNeedToCheck=false, bNeedToCloseSite=false;
    double dYield=0.0;
    double IndexZ1Yield=0.0, IndexZ2Yield=0.0;                                  //wei 20180709 (steven) BySiteYieldAlarm不作動異常
    AnsiString ErrPart="";
    AnsiString aLowYield="";

    if(bFirstCount)
    {
        iCount2=0;
        return;
    }

    if(SystemStart &&
       fContact->fShow==false &&
       iHome==0 &&
       bRunAutoClean==false &&
       LastSet.iRunStartMode!=rsmAutoSiteMap)                                   //Steven 20230313 : 做Auto Site Map的時候不要檢查Yield
    {
        iCount2++;
        if(bZ1PickShuttle || bZ2PickShuttle)                                    //Steven 20180521 : 避免吸料的時候發Low Yield Alarm  //JerryYang 20180629 (wei) 避免count被歸零
            return;
        if(iCount2>=iAlarmTimeInterval ||                                       //JimmyChiu 20220601 : 修改low yield報警時間邏輯，報警間隔時間固定1min改為可以自行設定報警間隔時間
           CosFunction.bYieldAlarmNoWait1Min)                                   //wei 20150820  Yield Alarm No Wait 1Min
        {
            iCount2=0;
        }
        else
        {
            return;
        }

        if(TestIF_File.iAutoClean_Function &&                                   //Sam 20230104 : 修正 LowYield AutoClean
           (TestIF_File.iAutoClean_Mode&M_SOCKET_ALARM) &&
           TestIF.bAutoClean_FailAlarmSiteYieldDifferent &&
           TestIF.iAutoClean_FailAlarmSiteYield!=0)                             //Steven 20220419 : 往上移動
        {
            if(CUSTOMER_CODE==CC_TERAPOWER)                                     //Sam 20230104 : 晶兆成改用 IC 數量
                sum=ArmData_AutoClean[0]->GetTotalCT()+ArmData_AutoClean[1]->GetTotalCT();
            else
                sum=iAutoClean_FailAlarmSiteYieldIntervalCount;

            if(sum>0)
            {
                dYield=fContactCT->GetLowYield_AutoClean(1);
                if(sum>=TestIF.iAutoClean_FailAlarmSiteYieldDifferentCount &&
                   dYield>TestIF.iAutoClean_FailAlarmSiteYield)
                {
                    aLowYield="AutoClean : Site Yield Different : " + AnsiString(dYield) + "%" + " Contact Count : " + iAutoClean_IndexContactCount;
                    RecordProcess(aLowYield);
                    InitialAutoCleanAllTask();                                  //Sam 20230504 : 整理 InitialAutoCleanTask
                    iAutoClean_FailAlarmSiteYieldIntervalCount=0;
                }
            }
        }

        if((TestIF_File.iAutoClean_Function==false &&
            TestIF_File.iAutoClean_Mode&M_SOCKET_ALARM)==0 &&
            TestIF.bAutoClean_FailAlarmSiteYieldDifferent==false)
        {
            iAutoClean_FailAlarmSiteYieldIntervalCount=0;
        }

        if(bUseTwoArm32Site==true ||                                            //Steven 20220419 : NN mode不需要比by arm
           TestIF.iShuttleMode!=0)                                              //ChungHung 20130114 關單Arm 要自動關閉)
        {
            iSiteCount=0;
            bYieldDiffOver=false;
            bNeedToCheck=false;
        }
        else                                                                    //KEVIN 20130710 Site Differ Yield% (After 1 min)
        {
            if(TestIF.iShuttleMode==0 && Prod.dFailAlarmSiteYield>0)
            {
                if(CosFunction.bYieldControlUseEACount)                         //wei 20180606 Yield控制使用EA Count    //Steven 20230223 : 簡化判斷式
                {
                    bNeedToCheck=(RunInfo.iUnloadCount-iYeildCT[0]>=Prod.iFailAlarmSiteYieldDifferentCount);
                }
                else
                {
                    bNeedToCheck=(iFailAlarmSiteYieldIntervalCount>=Prod.iFailAlarmSiteYieldDifferentCount);
                }
            }
            else
            {
                bNeedToCheck=false;
            }

            for(int i=0; i<TestSocket.iShtRow; i++)
            {
                for(int j=0; j<TestSocket.iShtCol; j++)
                {
                    if(LastSet.bUseTestSocket[0][i][j])
                        iSiteCount++;
                    if(LastSet.bUseTestSocket[1][i][j])
                        iSiteCount++;

                    if(LastSet.bUseTestSocket[0][i][j] &&
                       LastSet.bUseTestSocket[1][i][j])
                    {
                        IndexZ1Yield=ArmData[0]->ArmSKET[i][j]->GetBySitePCA(); //wei 20180709 (steven) BySiteYieldAlarm不作動異常
                        IndexZ2Yield=ArmData[1]->ArmSKET[i][j]->GetBySitePCA(); //wei 20180709 (steven) BySiteYieldAlarm不作動異常

                        if(bNeedToCheck &&                                      //Steven 20230223 : 簡化判斷式
                           (IndexZ1Yield!=0 && IndexZ2Yield!=0) &&
                           (abs(IndexZ1Yield-IndexZ2Yield)>Prod.dFailAlarmSiteYield))                   //JerryYang 20160530 LowYieldLimit要能設定到小數點
                        {
                            iSiteCount--;
                            ErrPart+=IndexSuckName[i][j];
                            bShowSiteYield[j+i*NEW_MAX_Index_Col]=true;         //Steven 20111115 : 8-> NEW_MAX_Index_Col
                            fContactCT->sgYield->Refresh();
                            bYieldDiffOver=true;
                        }
                        else
                        {
                            bShowSiteYield[j+i*NEW_MAX_Index_Col]=false;        //Steven 20111115 : 8-> NEW_MAX_Index_Col
                        }
                    }
                }
            }
        }

        if(CosFunction.bLowYieldAutoSiteOff &&                                  //Sam 20221202 : 修正自動關 Site 關到剩餘設定 Site 數時需要報警。
           TestIF_File.bLowYieldAutoSiteOffByArmSite &&                         //Steven 20230223 : by arm by site, auto site off
           iRunStartMode==FT)
        {
            if(iSiteCount<2)
                bNeedToCloseSite=false;
            else if(iSiteCount<TestIF_File.iAlarmWhenSiteOnCountLess)
                bNeedToCloseSite=false;
            else
                bNeedToCloseSite=true;
        }

        if(iSiteCount<2)                                                        //Steven 20200522 : 改成2, 只剩下一個site就不用比了
        {
            bYieldDiffOver=false;
        }

        if(iRunStartMode==FT &&
           IniConfig.bA09_ByArmCloseSite    &&
           CosFunction.bLowYieldAutoSiteOff &&                                  //Steven 20170905 (wei) : Low Yield Auto Site Off for Ambient
           TestIF_File.bLowYieldAutoSiteOffByArmSite &&                         //Steven 20230223 : by arm by site, auto site off
           iSiteCount>=TestIF_File.iAlarmWhenSiteOnCountLess &&                 //Steven 20200522 : 改成數字比對, 避免參數混用
           bNeedToCloseSite==true)                                              //Sam 20221207 : LowYieldAutoSiteOff 新增 Alarm 幾次後再來關 Site
        {
            for(int i=0; i<TestSocket.iShtRow; i++)
            {
                for(int j=0; j<TestSocket.iShtCol; j++)
                {
                    if(LastSet.bUseTestSocket[0][i][j] &&
                       LastSet.bUseTestSocket[1][i][j])
                    {
                        IndexZ1Yield=ArmData[0]->ArmSKET[i][j]->GetBySitePCA(); //wei 20180709 (steven) BySiteYieldAlarm不作動異常
                        IndexZ2Yield=ArmData[1]->ArmSKET[i][j]->GetBySitePCA(); //wei 20180709 (steven) BySiteYieldAlarm不作動異常

                        if(bNeedToCheck &&                                      //Steven 20230223 : 簡化判斷式
                           Prod.dFailAlarmSiteYield!=0 &&                       //ChungHung 20130114 關單Arm 要自動關閉
                           (IndexZ1Yield!=0 && IndexZ2Yield!=0) &&
                           (abs(IndexZ1Yield-IndexZ2Yield)>Prod.dFailAlarmSiteYield))                   //JerryYang 20160530 LowYieldLimit要能設定到小數點
                        {
                            if(IndexZ1Yield>IndexZ2Yield)
                                bLowYieldCloseSite[0][i][j]=true;
                            else
                                bLowYieldCloseSite[1][i][j]=true;
                        }
                    }
                }
            }
        }
        else
        {
            if(Prod.bFailAlarmSiteYieldDifferent &&
               Prod.iFailAlarmSiteYieldDifferentCount!=0)
            {
                if(bNeedToCheck)                                                //Steven 20230223 : 簡化判斷式
                {
                    if(bYieldDiffOver==true && TestIF.iShuttleMode==0)          //jou 2012-09-20 修正 Site Yield Different 關單 arm 會一直 alarm
                    {
                        if(bLowYeildAlarm==false)                               //wei 20151116 Yeild Alarm 只能Onecycle
                        {
                            ret=DoLowYieldAlarm("WAR0703", ErrPart);            //Steven 20180627 (wei) : 整合Low Yield Alarm
                            if(ret==K_ONECYCLE)
                            {
                                bLowYeildAlarm=true;
                            }
                            else
                            {
                                if(CosFunction.bYieldControlUseEACount)
                                    iYeildCT[0]=RunInfo.iUnloadCount;
                                else
                                    iFailAlarmSiteYieldIntervalCount=0;
                            }
                        }
                        bYieldDiffOver=false;
                    }
                }
            }
            else
            {
                if(CosFunction.bYieldControlUseEACount)
                    iYeildCT[0]=RunInfo.iUnloadCount;
                else
                    iFailAlarmSiteYieldIntervalCount=0;
            }

            if(CosFunction.bYieldControlUseEACount)
                fShowBinSelect->labArmDiff->Caption=RunInfo.iUnloadCount-iYeildCT[0];
        }
    }
    else
    {
        iCount2=0;
    }
}
//---------------------------------------------------------------------------
void TfYieldMonitoring::CheckByPickerYieldAlarm()                               //Steven 20230223 : 根據Index吸嘴比較良率
{                                                                               //By Picker Compare Yield
    static int iCount2=0;

    int iSiteCount=0, ret=0, iNN=0;
    int iAlarmTimeInterval=(CosFunction.bLowYieldAlarmIntervalTimeBySetting &&  //JimmyChiu 20220601 : 修改low yield報警時間邏輯，報警間隔時間固定1min改為可以自行設定報警間隔時間
                            IniConfig.bI44_LowYieldAlarmIntervalTimeBySetting)?IniConfig.iI44_LowYieldAlarmIntervalTime:60;

    bool bYieldDiffOver=false, bNeedToCheck=false, bNeedToCloseSite=false;
    bool bArm1LowYield=false, bArm2LowYield=false;
    AnsiString ErrPart1="", ErrPart2="";
    AnsiString aLowYield;
    double dLowYield;

    dMaxPickerYield=0;

    if(bFirstCount)
    {
        iCount2=0;
        return;
    }

    if(SystemStart &&
       fContact->fShow==false &&
       iHome==0 &&
       bRunAutoClean==false &&
       LastSet.iRunStartMode!=rsmAutoSiteMap)                                   //Steven 20230313 : 做Auto Site Map的時候不要檢查Yield
    {
        iCount2++;
        if(bZ1PickShuttle || bZ2PickShuttle)                                    //Steven 20180521 : 避免吸料的時候發Low Yield Alarm  //JerryYang 20180629 (wei) 避免count被歸零
            return;
        if(iCount2>=iAlarmTimeInterval ||                                       //JimmyChiu 20220601 : 修改low yield報警時間邏輯，報警間隔時間固定1min改為可以自行設定報警間隔時間
           CosFunction.bYieldAlarmNoWait1Min)                                   //wei 20150820  Yield Alarm No Wait 1Min
        {
            iCount2=0;
        }
        else
        {
            return;
        }

        if(Prod.dLowYieldByPicker>0)
        {
            if(CosFunction.bYieldControlUseEACount)                             //wei 20180606 Yield控制使用EA Count    //Steven 20230223 : 簡化判斷式
            {
                bNeedToCheck=(RunInfo.iUnloadCount-iYeildCT[7]>=Prod.iLowYieldCountByPicker);
            }
            else
            {
                bNeedToCheck=(iPickerYieldIntervalCount>=Prod.iLowYieldCountByPicker);
            }
        }
        else
        {
            bNeedToCheck=false;
        }

        if(bNeedToCheck)
        {
            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(LastSet.bUseTestSocket[0][i][j])
                    {
                        if(dPickerYield[0][i][j]>dMaxPickerYield)
                            dMaxPickerYield=dPickerYield[0][i][j];
                        iSiteCount++;
                    }

                    if(LastSet.bUseTestSocket[1][i][j])
                    {
                        if(dPickerYield[1][i][j]>dMaxPickerYield)
                            dMaxPickerYield=dPickerYield[1][i][j];
                        iSiteCount++;
                    }
                }
            }
        }

        if(Prod.dLowYieldByPicker!=0 &&
           dMaxPickerYield>Prod.dLowYieldByPicker)
            dLowYield=dMaxPickerYield-Prod.dLowYieldByPicker;
        else
            dLowYield=-1;

        if(dLowYield>0 && bNeedToCheck)
        {
            iNN=IsNNMode();
            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(LastSet.bUseTestSocket[0][i][j])
                    {
                        if(dLowYield>dPickerYield[0][i][j])
                        {
                            iSiteCount--;
                            ErrPart1+=IndexSuckName[i+iNN][j];
                            bYieldDiffOver=true;
                            bArm1LowYield=true;
                        }
                    }

                    if(LastSet.bUseTestSocket[1][i][j])
                    {
                        if(dLowYield>dPickerYield[1][i][j])
                        {
                            iSiteCount--;
                            ErrPart2+=IndexSuckName[i][j];
                            bYieldDiffOver=true;
                            bArm2LowYield=true;
                        }
                    }
                }
            }
        }
        else
        {
            bNeedToCheck=false;
        }

        if(CosFunction.bLowYieldAutoSiteOff &&
           TestIF_File.bLowYieldAutoSiteOffByPicker &&                          //Sam 20221202 : 修正自動關 Site 關到剩餘設定 Site 數時需要報警。
           bYieldDiffOver==true)                                                //Steven 20230223 : by arm by site, auto site off
        {
            if(iSiteCount<2)
                bNeedToCloseSite=false;
            else if(iSiteCount>=TestIF_File.iAlarmWhenSiteOnCountLess)
                bNeedToCloseSite=true;
            else
                bNeedToCloseSite=false;
        }

        if(iSiteCount<2)                                                        //Steven 20200522 : 改成2, 只剩下一個site就不用比了
        {
            bYieldDiffOver=false;
        }

        if(bNeedToCheck &&                                                      //Steven 20230223 : 簡化判斷式
           CosFunction.bLowYieldAutoSiteOff &&                                  //Steven 20170905 (wei) : Low Yield Auto Site Off for Ambient
           TestIF_File.bLowYieldAutoSiteOffByPicker &&                          //Steven 20230223 : by arm by site, auto site off
           iSiteCount>=TestIF_File.iAlarmWhenSiteOnCountLess &&                 //Steven 20200522 : 改成數字比對, 避免參數混用
           bNeedToCloseSite==true)                                              //Sam 20221207 : LowYieldAutoSiteOff 新增 Alarm 幾次後再來關 Site
        {
            for(int i=0; i<FTestSuck.iShtRow; i++)
            {
                for(int j=0; j<FTestSuck.iShtCol; j++)
                {
                    if(LastSet.bUseTestSocket[0][i][j])
                    {
                        if(dPickerYield[0][i][j]!=0 &&                          //Steven 20230313 : 沒有Yield的時候不比較
                           dLowYield>dPickerYield[0][i][j])
                        {
                            bLowYieldCloseSite[0][i][j]=true;
                        }
                    }

                    if(LastSet.bUseTestSocket[1][i][j])
                    {
                        if(dPickerYield[1][i][j]!=0 &&                          //Steven 20230313 : 沒有Yield的時候不比較
                           dLowYield>dPickerYield[1][i][j])
                        {
                            bLowYieldCloseSite[1][i][j]=true;
                        }
                    }
                }
            }
        }
        else
        {
            if(Prod.bLowYieldByPicker && Prod.iLowYieldCountByPicker!=0)
            {
                if(bNeedToCheck)                                                //Steven 20230223 : 簡化判斷式
                {
                    if(bYieldDiffOver==true)
                    {
                        if(bLowYeildAlarm==false)                               //wei 20151116 Yeild Alarm 只能Onecycle
                        {
                            if(bArm1LowYield)
                            {
                                ret=DoLowYieldAlarm("WAR0726", ErrPart1);       //Steven 20180627 (wei) : 整合Low Yield Alarm
                            }

                            if(bArm2LowYield)
                            {
                                ret=DoLowYieldAlarm("WAR0727", ErrPart2);       //Steven 20180627 (wei) : 整合Low Yield Alarm
                            }

                            if(ret==K_ONECYCLE)
                            {
                                bLowYeildAlarm=true;
                            }
                            else
                            {
                                if(CosFunction.bYieldControlUseEACount)
                                    iYeildCT[7]=RunInfo.iUnloadCount;
                                else
                                    iPickerYieldIntervalCount=0;
                            }
                        }
                        bYieldDiffOver=false;
                    }
                }
            }
            else
            {
                if(CosFunction.bYieldControlUseEACount)
                    iYeildCT[7]=RunInfo.iUnloadCount;
                else
                    iPickerYieldIntervalCount=0;
            }

            if(CosFunction.bYieldControlUseEACount)
                fShowBinSelect->labArmDiff->Caption=RunInfo.iUnloadCount-iYeildCT[7];
        }
    }
    else
    {
        iCount2=0;
    }
}
//---------------------------------------------------------------------------
void TfYieldMonitoring::ClearAutoSiteOffStatus()                                //Steven 20200409 : 修正清除count之後,不能開site的問題
{
    for(int i=0; i<MAX_Index_Row; i++)
    {
        for(int j=0; j<MAX_Index_Col; j++)
        {
            bLowYieldCloseSite[0][i][j]=false;                                  //Steven 20170905 (wei) : Low Yield Auto Site Off for Ambient
            bLowYieldCloseSite[1][i][j]=false;
        }
    }
}
//---------------------------------------------------------------------------
void TfYieldMonitoring::CheckBySiteByArmYieldAlarm()                            //jou 2014-08-14 Site Compare Low Yield alarm
{
    static int iCount3=0;

    int iSiteCount=0, ret;
    int iAlarmTimeInterval=(CosFunction.bLowYieldAlarmIntervalTimeBySetting &&  //JimmyChiu 20220601 : 修改low yield報警時間邏輯，報警間隔時間固定1min改為可以自行設定報警間隔時間
                            IniConfig.bI44_LowYieldAlarmIntervalTimeBySetting)?IniConfig.iI44_LowYieldAlarmIntervalTime:60;

    bool bYieldCmpOver=false;
    double fYieldMax=0.0;
    AnsiString ErrPart="";

    if(bFirstCount)
    {
        iCount3=0;
        return;
    }

    if(SystemStart &&
       fContact->fShow==false &&
       iHome==0 &&
       bRunAutoClean==false &&
       LastSet.iRunStartMode!=rsmAutoSiteMap)                                   //Steven 20230313 : 做Auto Site Map的時候不要檢查Yield
    {
        if(CosFunction.bSiteCmpYield &&
           Prod.bFailAlarmSiteYieldCmp &&
           Prod.iFailAlarmSiteYieldCmpCount!=0)
        {
            iCount3++;
            if(bZ1PickShuttle || bZ2PickShuttle)                                //Steven 20180521 : 避免吸料的時候發Low Yield Alarm  //JerryYang 20180629 (wei) 避免count被歸零
                return;

            if(iCount3>=iAlarmTimeInterval ||                                   //JimmyChiu 20220601 : 修改low yield報警時間邏輯，報警間隔時間固定1min改為可以自行設定報警間隔時間
               CosFunction.bYieldAlarmNoWait1Min)                               // 1 mimutes  //wei 20150820  Yield Alarm No Wait 1Min
            {
                iCount3=0;
                if(bUseTwoArm32Site==true)                                      //Steven 20220419 : NN mode不需要比by arm
                {
                    for(int i=0; i<TestSocket.iShtRow; i++)
                    {
                        for(int j=0; j<TestSocket.iShtCol; j++)
                        {
                            if(LastSet.bUseTestSocket[0][i][j]==true)
                            {
                                if(fYieldMax<dSiteYield[i][j])
                                    fYieldMax=dSiteYield[i][j];
                            }
                        }
                    }

                    for(int i=0; i<TestSocket.iShtRow; i++)
                    {
                        for(int j=0; j<TestSocket.iShtCol; j++)
                        {
                            if(LastSet.bUseTestSocket[0][i][j])
                            {
                                iSiteCount++;
                                if(abs(fYieldMax-dSiteYield[i][j])>Prod.dFailAlarmSiteYieldCmp) //JerryYang 20160530 LowYieldLimit要能設定到小數點
                                {
                                    if(CosFunction.bLowYieldAutoSiteOff &&      //Steven 20170905 (wei) : Low Yield Auto Site Off for Ambient
                                       TestIF_File.bLowYieldAutoSiteOff &&
                                       iRunStartMode==FT)
                                    {
                                        if(iFailAlarmSiteMaxYieldIntervalCount>=Prod.iFailAlarmSiteYieldCmpCount)
                                        {
                                            iSiteCount--;
                                            if(TestIF_File.bLowYieldAutoSiteOffAlarm)   //Sam 20221207 : LowYieldAutoSiteOff 新增 Alarm 幾次後再來關 Site
                                            {
                                                iAlarmSiteYieldCmpCnt[i][j]++;
                                                if(iAlarmSiteYieldCmpCnt[i][j]<=TestIF_File.iLowYieldAutoSiteOffAlarm)
                                                {
                                                    ErrPart+=IndexSuckName[i][j];
                                                }
                                            }
                                            bYieldCmpOver=true;                 //Sam 20250410 : 修正auto site off 關site低於設定值時不會alarm的問題
                                        }
                                    }
                                    else
                                    {
                                        bYieldCmpOver=true;
                                    }
                                    ErrPart+=IndexSuckName[i][j];
                                }
                                else
                                {
                                    if(iFailAlarmSiteMaxYieldIntervalCount>=Prod.iFailAlarmSiteYieldCmpCount)    //Sam 20221207 : LowYieldAutoSiteOff 新增 Alarm 幾次後再來關 Site
                                        iAlarmSiteYieldCmpCnt[i][j]=0;
                                }
                            }
                        }
                    }
                }
                else
                {
                    for(int i=0; i<TestSocket.iShtRow; i++)
                    {
                        for(int j=0; j<TestSocket.iShtCol; j++)
                        {
                            if(LastSet.bUseTestSocket[0][i][j] ||               //Isaac 20210630 : 修正關arm會失效，&&->||
                               LastSet.bUseTestSocket[1][i][j])
                            {
                                if(fYieldMax<dSiteYield[i][j])
                                    fYieldMax=dSiteYield[i][j];
                            }
                        }
                    }

                    for(int i=0; i<FTestSuck.iShtRow; i++)
                    {
                        for(int j=0; j<FTestSuck.iShtCol; j++)
                        {
                            if(LastSet.bUseTestSocket[0][i][j] ||               //Isaac 20210630 : 修正關arm會失效，&&->||
                               LastSet.bUseTestSocket[1][i][j])
                            {
                                iSiteCount++;
                                if(abs(fYieldMax-dSiteYield[i][j])>Prod.dFailAlarmSiteYieldCmp) //JerryYang 20160530 LowYieldLimit要能設定到小數點
                                {
                                    if(CosFunction.bLowYieldAutoSiteOff &&      //Steven 20170905 (wei) : Low Yield Auto Site Off for Ambient
                                       TestIF_File.bLowYieldAutoSiteOff &&
                                       iRunStartMode==FT)
                                    {
                                        if(iFailAlarmSiteMaxYieldIntervalCount>=Prod.iFailAlarmSiteYieldCmpCount)
                                        {
                                            iSiteCount--;
                                            if(TestIF_File.bLowYieldAutoSiteOffAlarm)   //Sam 20221207 : LowYieldAutoSiteOff 新增 Alarm 幾次後再來關 Site
                                            {
                                                iAlarmSiteYieldCmpCnt[i][j]++;
                                                if(iAlarmSiteYieldCmpCnt[i][j]<=TestIF_File.iLowYieldAutoSiteOffAlarm)
                                                {
                                                    ErrPart+=IndexSuckName[i][j];
                                                }
                                            }
                                        }
                                    }

                                    bYieldCmpOver=true;                         //jou 20221102 : 修正auto site off 關site低於設定值時不會alarm的問題
                                    ErrPart+=IndexSuckName[i][j];
                                }
                                else
                                {
                                    if(iFailAlarmSiteMaxYieldIntervalCount>=Prod.iFailAlarmSiteYieldCmpCount)    //Sam 20221207 : LowYieldAutoSiteOff 新增 Alarm 幾次後再來關 Site
                                        iAlarmSiteYieldCmpCnt[i][j]=0;
                                }
                            }
                        }
                    }
                }

                if(CosFunction.bLowYieldAutoSiteOff &&                          //Steven 20170905 (wei) : Low Yield Auto Site Off for Ambient
                   TestIF_File.bLowYieldAutoSiteOff &&
                   iRunStartMode==FT &&
                   iSiteCount>=TestIF_File.iAlarmWhenSiteOnCountLess)           //Steven 20200522 : 改成數字比對, 避免參數混用
                {
                    if(iFailAlarmSiteMaxYieldIntervalCount>=Prod.iFailAlarmSiteYieldCmpCount)
                    {
                        if(bUseTwoArm32Site==true)                              //Steven 20220419 : NN mode不需要比by arm
                        {
                            for(int i=0; i<TestSocket.iShtRow; i++)
                            {
                                for(int j=0; j<TestSocket.iShtCol; j++)
                                {
                                    if(LastSet.bUseTestSocket[0][i][j]==true)
                                    {
                                        if(abs(fYieldMax-dSiteYield[i][j])>Prod.dFailAlarmSiteYieldCmp) //JerryYang 20160530 LowYieldLimit要能設定到小數點
                                        {
                                            bLowYieldCloseSite[0][i][j]=true;
                                            if(iAlarmSiteYieldCmpCnt[i][j]>TestIF_File.iLowYieldAutoSiteOffAlarm)   //Sam 20221207 : LowYieldAutoSiteOff 新增 Alarm 幾次後再來關 Site
                                                iAlarmSiteYieldCmpCnt[i][j]=0;
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            for(int i=0; i<TestSocket.iShtRow; i++)
                            {
                                for(int j=0; j<TestSocket.iShtCol; j++)
                                {
                                    if(LastSet.bUseTestSocket[0][i][j] ||
                                       LastSet.bUseTestSocket[1][i][j])         //Isaac 20210630 : 修正關arm會失效，&&->||
                                    {
                                        if(abs(fYieldMax-dSiteYield[i][j])>Prod.dFailAlarmSiteYieldCmp) //JerryYang 20160530 LowYieldLimit要能設定到小數點
                                        {
                                            bLowYieldCloseSite[0][i][j]=true;
                                            if(iAlarmSiteYieldCmpCnt[i][j]>TestIF_File.iLowYieldAutoSiteOffAlarm)   //Sam 20221207 : LowYieldAutoSiteOff 新增 Alarm 幾次後再來關 Site
                                                iAlarmSiteYieldCmpCnt[i][j]=0;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                else if(CosFunction.bYieldControlUseEACount)                    //wei 20151111  //wei 20180606 Yield控制使用EA Count
                {
                    if(RunInfo.iUnloadCount-iYeildCT[2]>=Prod.iFailAlarmSiteYieldCmpCount)
                    {
                        if(bYieldCmpOver==true)
                        {
                            if(bLowYeildAlarm==false)                           //wei 20151116 Yeild Alarm 只能Onecycle
                            {
                                ret=DoLowYieldAlarm("WAR0702", ErrPart);        //Steven 20180627 (wei) : 整合Low Yield Alarm
                                if(ret==K_ONECYCLE)
                                {
                                    bLowYeildAlarm=true;
                                }
                                else
                                {
                                    iYeildCT[2]=RunInfo.iUnloadCount;
                                }
                            }
                            bYieldCmpOver=false;
                        }
                    }
                }
                else
                {
                    if(iFailAlarmSiteMaxYieldIntervalCount>=Prod.iFailAlarmSiteYieldCmpCount)
                    {
                        if(bYieldCmpOver==true)
                        {
                            iFailAlarmSiteMaxYieldIntervalCount=0;              //JerryYang 20170406 (Steven) 觸發yield alarm後才清掉
                            ClearYieldCount();                                  //Steven 20140830 : Yield相關的Alarm, 要清掉全部的Ignore的Count重算
                            bYieldCmpOver=false;
                            ret=DoLowYieldAlarm("WAR0702", ErrPart);            //Steven 20180627 (wei) : 整合Low Yield Alarm
                        }
                    }
                }
            }
        }
        else
        {
            if(CosFunction.bYieldControlUseEACount)                             //wei 20151111  //wei 20180606 Yield控制使用EA Count
            {
                iYeildCT[2]=RunInfo.iUnloadCount;
            }
            else
            {
                iFailAlarmSiteMaxYieldIntervalCount=0;
            }
        }

        if(CosFunction.bYieldControlUseEACount)                                 //Steven 20170605 (wei) : 修正畫面顯示 //wei 20151111  //wei 20180606 Yield控制使用EA Count
        {
            fShowBinSelect->labSiteDiff->Caption=RunInfo.iUnloadCount-iYeildCT[2];
        }
        else
        {
            fShowBinSelect->labSiteDiff->Caption=iFailAlarmSiteMaxYieldIntervalCount;
        }
    }
}
//---------------------------------------------------------------------------
void TfYieldMonitoring::CheckLowYieldAlarm()
{
    static int iCount2=0;

    int sum=0, ipass=0, ret=0;
    int iAlarmTimeInterval=(CosFunction.bLowYieldAlarmIntervalTimeBySetting &&  //JimmyChiu 20220601 : 修改low yield報警時間邏輯，報警間隔時間固定1min改為可以自行設定報警間隔時間
                            IniConfig.bI44_LowYieldAlarmIntervalTimeBySetting)?IniConfig.iI44_LowYieldAlarmIntervalTime:60;

    bool bIsLowYield=false;
    double dYield1=0.0;                                                         //JerryYang 20160530 iYield改成dYield
    AnsiString aLowYield="";
    AnsiString SocketErrPart="", s="";

    if(bFirstCount)
    {
        iCount2=0;
        return;
    }

    if(SystemStart && fContact->fShow==false &&
       iHome==0 && bRunAutoClean==false &&
       LastSet.iRunStartMode!=rsmAutoSiteMap)                                   //Steven 20230313 : 做Auto Site Map的時候不要檢查Yield
    {
        if((Prod.bFailAlarmLowYield && Prod.dLowYieldLimit!=0) ||               //JerryYang 20160530 LowYieldLimit要能設定到小數點
           (IniConfig.bE53LowYieldAutoClean && TestIF.iAutoClean_Function==true &&
            TestIF.bAutoClean_FailAlarmLowYield && TestIF.iAutoClean_LowYieldLimit!=0) || //Steven 20220110 : 修正沒開Auto clean跟low yield卻會alarm
           (IniConfig.bE53LowYieldAutoClean && TestIF.iAutoClean_Function==true &&
            Prod.bFailAlarmLowYield_AutoClean && Prod.iLowYieldLimit_AutoClean!=0))
        {
            iCount2++;                                                          //JerryYang 20180629 (wei) 避免count被歸零
            if(bZ1PickShuttle || bZ2PickShuttle)                                //Steven 20180521 : 避免吸料的時候發Low Yield Alarm
                return;

            if(iCount2>=iAlarmTimeInterval ||                                   // 1 mimutes    //JimmyChiu 20220601 : 修改low yield報警時間邏輯，報警間隔時間固定1min改為可以自行設定報警間隔時間
               CosFunction.bYieldAlarmNoWait1Min)                               //wei 20150820  Yield Alarm No Wait 1Min
            {
                iCount2=0;

                if(IniConfig.bEnableAutoCleanFunction &&                        //ChungHung 20131223 add for SCK start
                   TestIF.iAutoClean_Function==true &&
                   (TestIF_File.iAutoClean_Mode & M_SOCKET_ALARM) &&
                   TestIF.bAutoClean_FailAlarmLowYield)
                {
                    sum=ArmData_AutoClean[0]->GetTotalCT()+ArmData_AutoClean[1]->GetTotalCT();  //Sam 20230104 : 修正 LowYield AutoClean
                    if(sum>0)
                    {
                        dYield1=fContactCT->GetLowYield_AutoClean(0);
                        if(sum>=TestIF.iAutoClean_LowYieldCount &&  TestIF.iAutoClean_LowYieldLimit>dYield1)
                        {
                            aLowYield="AutoClean : Low Yield Alarm : " + AnsiString(dYield1) + "%" + " Contact Count : " + iAutoClean_IndexContactCount;
                            RecordProcess(aLowYield);
                            InitialAutoCleanAllTask();                          //Sam 20230504 : 整理 InitialAutoCleanTask
                        }
                    }
                }

                if(IniConfig.bLowYieldAlarmSameNS==true &&                      //jou    2011-07-16 : Low Yield Alarm模式與NS機台相同,skip會清除單獨Site.
                   Prod.bFailAlarmLowYield && Prod.dLowYieldLimit!=0)           //Steven 20220110 : 修正沒開Auto clean跟low yield卻會alarm
                {
                    if(CosFunction.bYieldControlUseEACount)                     //wei 20151111  //wei 20180606 Yield控制使用EA Count
                    {
                        if(Prod.bFailAlarmLowYield && Prod.dLowYieldLimit!=0)   //JerryYang 20160530 LowYieldLimit要能設定到小數點
                        {
                            if(RunInfo.iUnloadCount-iYeildCT[1]>=Prod.iLowYieldCount)
                            {
                                if(bUseTwoArm32Site==true)                      //Steven 20220418 : NN mode Yield alarm
                                {
                                    for(int i=0; i<TestSocket.iShtRow; i++)
                                    {
                                        for(int j=0; j<TestSocket.iShtCol; j++)
                                        {
                                            if(LastSet.bUseTestSocket[0][i][j])
                                            {
                                                dYield1=fContactCT->ReturnSiteDataArray(false, i, j);
                                                if(Prod.dLowYieldLimit>dYield1)
                                                {
                                                    bIsLowYield=true;
                                                    SocketErrPart+=IndexSuckName[i][j];
                                                }
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    for(int i=0; i<TestSocket.iShtRow; i++)
                                    {
                                        for(int j=0; j<TestSocket.iShtCol; j++)
                                        {
                                            if(LastSet.bUseTestSocket[0][i][j] ||   //Steven 20230831 : 關Arm不會Alarm && --> ||
                                               LastSet.bUseTestSocket[1][i][j])     //Frank 20160802 add Low Yield 關Site
                                            {
                                                dYield1=fContactCT->ReturnSiteDataArray(false, i, j);
                                                if(Prod.dLowYieldLimit>dYield1)     //JerryYang 20160530 LowYieldLimit要能設定到小數點
                                                {
                                                    bIsLowYield=true;
                                                    SocketErrPart+=IndexSuckName[i][j];
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            iYeildCT[1]=RunInfo.iUnloadCount;
                        }
                        fShowBinSelect->labLowYield->Caption=RunInfo.iUnloadCount-iYeildCT[1];
                    }
                    else
                    {
                        if(Prod.bFailAlarmLowYield && Prod.dLowYieldLimit!=0)   //JerryYang 20160530 LowYieldLimit要能設定到小數點
                        {
                            if(bUseTwoArm32Site==true)                          //Steven 20220418 : NN mode Yield alarm
                            {
                                for(int i=0; i<TestSocket.iShtRow; i++)
                                {
                                    for(int j=0; j<TestSocket.iShtCol; j++)
                                    {
                                        if(LastSet.bUseTestSocket[0][i][j])
                                        {
                                            sum=fContactCT->ReturnSiteDataArray(true, i, j);
                                            if(sum>=Prod.iLowYieldCount)
                                            {
                                                dYield1=fContactCT->ReturnSiteDataArray(false, i, j);
                                                if(Prod.dLowYieldLimit>dYield1) //JerryYang 20160530 LowYieldLimit要能設定到小數點
                                                {
                                                    bIsLowYield=true;
                                                    SocketErrPart+=IndexSuckName[i][j];
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            else
                            {
                                for(int i=0; i<TestSocket.iShtRow; i++)
                                {
                                    for(int j=0; j<TestSocket.iShtCol; j++)
                                    {
                                        if(LastSet.bUseTestSocket[0][i][j] ||   //Steven 20230831 : 關Arm不會Alarm
                                           LastSet.bUseTestSocket[1][i][j])     //Frank 20160802 add Low Yield 關Site
                                        {
                                            sum=fContactCT->ReturnSiteDataArray(true, i, j);
                                            if(sum>=Prod.iLowYieldCount)
                                            {
                                                dYield1=fContactCT->ReturnSiteDataArray(false, i, j);
                                                if(Prod.dLowYieldLimit>dYield1) //JerryYang 20160530 LowYieldLimit要能設定到小數點
                                                {
                                                    bIsLowYield=true;
                                                    SocketErrPart+=IndexSuckName[i][j];
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    if(bIsLowYield==true)
                    {
                        if(CosFunction.bYieldControlUseEACount)                 //wei 20151111  //wei 20180606 Yield控制使用EA Count
                        {
                            if(bLowYeildAlarm==false)                           //wei 20151116 Yeild Alarm 只能Onecycle
                            {
                                ret=DoLowYieldAlarm("WAR0701", SocketErrPart);  //Steven 20180627 (wei) : 整合Low Yield Alarm
                                if(ret==K_ONECYCLE)
                                {
                                    bLowYeildAlarm=true;
                                }
                                else
                                {
                                    iYeildCT[1]=RunInfo.iUnloadCount;
                                }
                            }
                            bIsLowYield=false;
                        }
                        else
                        {
                            DoLowYieldAlarm("WAR0701", SocketErrPart);          //Steven 20180627 (wei) : 整合Low Yield Alarm
                            ClearYieldCount();                                  //Steven 20140830 : Yield相關的Alarm, 要清掉全部的Ignore的Count重算

                            if(ret==K_ONECYCLE ||
                               CUSTOMER_CODE==CC_HANA_MICRON)                   //Steven 20210428 : Hana說Low Yield不要清除資料
                            {
                                bIsLowYield=false;
                            }
                            else
                            {
                                for(int i=0; i<TestSocket.iShtRow; i++)
                                {
                                    for(int j=0; j<TestSocket.iShtCol; j++)
                                    {
                                        dYield1=fContactCT->ReturnSiteDataArray(false, i, j);
                                        if(Prod.dLowYieldLimit>dYield1)
                                        {
                                            fContactCT->ClearData(i, j);        //Steven 20220301 : 修正Clear Yield
                                        }
                                    }
                                }
                                bIsLowYield=false;
                            }
                        }

                        for(int i=0; i<TestSocket.iShtRow; i++)
                        {
                            for(int j=0; j<TestSocket.iShtCol; j++)
                            {
                                dYield1=fContactCT->ReturnSiteDataArray(false, i, j);
                                if(Prod.dLowYieldLimit>dYield1 && (CUSTOMER_CODE!=CC_KYEC_LEE && CUSTOMER_CODE!=CC_KYEC_XILINX))
                                {
                                    bShowSiteYield[i*TestSocket.iShtRow+j]=true;
                                }
                                else
                                {
                                    bShowSiteYield[i*TestSocket.iShtRow+j]=false;
                                }
                            }
                        }
                    }

                    //-------------------
                    if(Prod.bLowYieldAlarmByBin)                                //Steven 20140828 : By Bin Yield Monitor
                    {
                        sum=0;
                        ipass=0;

                        for(int i=0; i<6; i++)
                        {
                            sum+=LastSet.BinCT[0][i];
                        }

                        for(int i=0; i<iTestBinCount; i++)
                        {
                            if(Prod.bLowYield[i]==true)
                                ipass+=LastSet.iBinData32[0][i];
                        }

                        if(sum>0)
                        {
                            dYield1=ChangeToFloat((double)ipass, (double)sum);
                            if(CosFunction.bYieldControlUseContactCount)        //Steven 20141212 : Yield控制使用Contact Count
                            {
                                if(iLowYieldContactCount>=Prod.iLowYieldCount &&
                                   Prod.dLowYieldLimit>dYield1)                 //JerryYang 20160530 LowYieldLimit要能設定到小數點
                                {
                                    iLowYieldContactCount=0;
                                    ClearYieldCount();                          //Steven 20140830 : Yield相關的Alarm, 要清掉全部的Ignore的Count重算
                                    DoLowYieldAlarm("WAR0701", SocketErrPart);  //Steven 20180627 (wei) : 整合Low Yield Alarm
                                }
                            }
                            else
                            {
                                if(sum>=Prod.iLowYieldCount &&
                                   Prod.dLowYieldLimit>dYield1)                 //JerryYang 20160530 LowYieldLimit要能設定到小數點
                                {
                                    ClearYieldCount();                          //Steven 20140830 : Yield相關的Alarm, 要清掉全部的Ignore的Count重算
                                    DoLowYieldAlarm("WAR0701", SocketErrPart);  //Steven 20180627 (wei) : 整合Low Yield Alarm
                                }
                            }
                        }
                    }
                }
                else if(IniConfig.bE53LowYieldAutoClean)                        //wei 20141201 Low Yield Auto Clean(%) start
                {
                    if(Prod.bFailAlarmLowYield_AutoClean && Prod.iLowYieldLimit_AutoClean!=0)
                    {
                        if(bUseTwoArm32Site==true)                              //Steven 20220418 : NN mode Yield alarm
                        {
                            for(int i=0; i<TestSocket.iShtRow; i++)
                            {
                                for(int j=0; j<TestSocket.iShtCol; j++)
                                {
                                    if(LastSet.bUseTestSocket[0][i][j])
                                    {
                                        sum=fContactCT->ReturnSiteDataArray(true, i, j);
                                        if(sum>=Prod.iLowYieldCount_AutoClean)
                                        {
                                            dYield1=fContactCT->ReturnSiteDataArray(false, i, j);
                                            if(Prod.iLowYieldLimit_AutoClean>dYield1)
                                            {
                                                bIsLowYield=true;
                                                SocketErrPart+=IndexSuckName[i][j]+ ":" + dYield1 + "%,";
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            for(int i=0; i<TestSocket.iShtRow; i++)
                            {
                                for(int j=0; j<TestSocket.iShtCol; j++)
                                {
                                    if(LastSet.bUseTestSocket[0][i][j] ||
                                       LastSet.bUseTestSocket[1][i][j])
                                    {
                                        sum=fContactCT->ReturnSiteDataArray(true, i, j);
                                        if(sum>=Prod.iLowYieldCount_AutoClean)
                                        {
                                            dYield1=fContactCT->ReturnSiteDataArray(false, i, j);
                                            if(Prod.iLowYieldLimit_AutoClean>dYield1)
                                            {
                                                bIsLowYield=true;
                                                SocketErrPart+=IndexSuckName[i][j]+ ":" + dYield1 + "%,";
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    if(bIsLowYield==true)
                    {
                        aLowYield="AutoClean : Low Yield Site : " + AnsiString(SocketErrPart);
                        RecordProcess(aLowYield);

                        if(CUSTOMER_CODE!=CC_HANA_MICRON)                       //Steven 20210428 : Hana說Low Yield不要清除資料
                        {
                            for(int i=0; i<TestSocket.iShtRow; i++)
                            {
                                for(int j=0; j<TestSocket.iShtCol; j++)
                                {
                                    dYield1=atof(fContactCT->ReturnSiteData(2, i*TestSocket.iShtCol+j).c_str());
                                    if(Prod.iLowYieldLimit_AutoClean>dYield1)
                                    {
                                        fContactCT->ClearData(i, j);            //Steven 20220301 : 修正Clear Yield
                                    }
                                }
                            }
                        }

                        if(IniConfig.bEnableAutoCleanFunction &&
                           TestIF.iAutoClean_Function==true &&
                           Prod.bFailAlarmLowYield_AutoClean)
                        {
                            InitialAutoCleanAllTask();                          //Sam 20230504 : 整理 InitialAutoCleanTask
                        }
                    }
                }                                                               //wei 20141201 Low Yield Auto Clean(%) end
                else if(Prod.bFailAlarmLowYield && Prod.dLowYieldLimit!=0)      //Steven 20220110 : 修正沒開Auto clean跟low yield卻會alarm
                {
                    sum=0;
                    ipass=0;

                    if(CosFunction.bLowYieldUseContactCounts)                   //Sam 20221020 : LowYield 改使用 ContactCounts 的資料來計算
                    {
                        sum=ArmData[0]->GetTotalCT()+ArmData[1]->GetTotalCT();
                        ipass=ArmData[0]->GetPassCT()+ArmData[1]->GetPassCT();
                    }
                    else if(Prod.bLowYieldAlarmByBin)                           //Steven 20140828 : By Bin Yield Monitor
                    {
                        for(int i=0; i<eTrayCount; i++)
                        {
                            if(Prod.iTrayType[i]==tNotUse)
                                continue;
                            sum+=LastSet.BinCT[0][iTo3Unload[i]];
                        }

                        for(int i=0; i<iTestBinCount; i++)
                        {
                            if(Prod.bLowYield[i]==true)
                                ipass+=LastSet.iBinData32[0][i];
                        }
                    }
                    else
                    {
                        for(int i=0; i<eTrayCount; i++)                         //JerryYang 20230925
                        {
                            if(Prod.iTrayType[i]==tNotUse)
                                continue;
                            sum+=LastSet.BinCT[0][iTo3Unload[i]];
                            if(Prod.iIsPassT6[i]==1)                            //Steven 20240105 : Prod.bIsPass --> Prod.iIsPassT6
                                ipass+=LastSet.BinCT[0][iTo3Unload[i]];
                        }
                    }

                    if(sum>0)
                    {
                        dYield1=ChangeToFloat((double)ipass, (double)sum);      //JerryYang 20160530 LowYieldLimit要能設定到小數點

                        if(CosFunction.bYieldControlUseContactCount)            //Steven 20141212 : Yield控制使用Contact Count
                        {
                            if(iLowYieldContactCount>=Prod.iLowYieldCount &&
                               Prod.dLowYieldLimit>dYield1)                     //JerryYang 20160530 LowYieldLimit要能設定到小數點
                            {
                                iLowYieldContactCount=0;
                                ClearYieldCount();                              //Steven 20140830 : Yield相關的Alarm, 要清掉全部的Ignore的Count重算
                                DoLowYieldAlarm("WAR0701", SocketErrPart);      //Steven 20180627 (wei) : 整合Low Yield Alarm
                            }
                        }
                        else
                        {
                            if(CUSTOMER_CODE==CC_JCET)                          //JerryYang 20170418 (Steven) JCET吳如春要求用Ignore count
                                sum=RunInfo.iUnloadCount-iYeildCT[1];

                            if(sum>=Prod.iLowYieldCount &&
                               Prod.dLowYieldLimit>dYield1)                     //JerryYang 20160530 LowYieldLimit要能設定到小數點
                            {
                                ClearYieldCount();                              //Steven 20140830 : Yield相關的Alarm, 要清掉全部的Ignore的Count重算
                                DoLowYieldAlarm("WAR0701", SocketErrPart);      //Steven 20180627 (wei) : 整合Low Yield Alarm
                            }
                        }
                    }
                }
            }
        }
        else
        {
            if(Prod.bFailAlarmLowYield==false)
                fShowBinSelect->labLowYield->Caption=0;
        }

        fContactCT->SaveTotalYield("");                                         //Sam 20231106 : 紀錄 Total yield
    }
    else
    {
        iCount2=0;
    }
}
//---------------------------------------------------------------------------
void TfYieldMonitoring::CheckLowYieldAlarmByTotal()                             //wei 20151116 Low Yield By Total
{
    static int iCount4=0;
    static bool bReflash=false;

    int sumByTotal=0, ipassByTotal=0, ret;
    int iAlarmTimeInterval=(CosFunction.bLowYieldAlarmIntervalTimeBySetting &&  //JimmyChiu 20220601 : 修改low yield報警時間邏輯，報警間隔時間固定1min改為可以自行設定報警間隔時間
                            IniConfig.bI44_LowYieldAlarmIntervalTimeBySetting)?IniConfig.iI44_LowYieldAlarmIntervalTime:60;

    bool bAlarm=false;
    double iYieldByTotal=0.0;
    AnsiString str="";
    AnsiString SocketErrPart="";

    if(bFirstCount)
    {
        iCount4=0;
        return;
    }

    if(SystemStart==false)                                                      //Ifor 20171017 add 強制更新一次避免資料不符
        bReflash=true;

    if((SystemStart || bReflash) &&
       fContact->fShow==false &&
       iHome==0 &&
       bRunAutoClean==false &&
       LastSet.iRunStartMode!=rsmAutoSiteMap)                                   //Steven 20230313 : 做Auto Site Map的時候不要檢查Yield
    {
        bReflash=false;
        fShowBinSelect->ShowCategoryBin();
        if(Prod.bFailAlarmLowYieldByTotal &&
           Prod.dLowYieldLimitByTotal!=0 &&                                     //JerryYang 20160530 LowYieldLimit要能設定到小數點
           CosFunction.bLowYeildByTotal)
        {
            iCount4++;
            if(bZ1PickShuttle || bZ2PickShuttle)                                //Steven 20180521 : 避免吸料的時候發Low Yield Alarm  //JerryYang 20180629 (wei) 避免count被歸零
                return;

            if(iCount4>=iAlarmTimeInterval ||                                   //JimmyChiu 20220601 : 修改low yield報警時間邏輯，報警間隔時間固定1min改為可以自行設定報警間隔時間
               CosFunction.bYieldAlarmNoWait1Min)                               //wei 20150820  Yield Alarm No Wait 1Min
            {
                iCount4=0;
                sumByTotal=0;
                ipassByTotal=0;

                for(int i=0; i<eTrayCount; i++)                                 //JerryYang 20230925
                {
                    if(Prod.iTrayType[i]==tNotUse)
                        continue;
                    sumByTotal+=LastSet.BinCT[0][iTo3Unload[i]];                //JerryYang 202240515 : 改成To3陣列            //Steven 20240109 : eTrayCount --> i
                    if(Prod.iIsPassT6[i]==1)                                    //Steven 20240105 : Prod.bIsPass --> Prod.iIsPassT6
                        ipassByTotal+=LastSet.BinCT[0][iTo3Unload[i]];
                }

                if(CosFunction.bYieldControlUseEACount ||                       //Kaichen 20190628 : Low Yield ByTotal 控制使用 Contact Count
                   CUSTOMER_CODE==CC_JCET)                                      //Steven 20231212 : JCET這一項使用的是EA Count
                {
                    if(sumByTotal>0)
                    {
                        iYieldByTotal=ChangeToFloat(double(ipassByTotal), double(sumByTotal));
                        bAlarm=false;                                           //JerryYang 20180302 fix Yield alarm by total失效
                        if(Prod.dLowYieldLimitByTotal>iYieldByTotal)
                        {
                            if(CUSTOMER_CODE==CC_KYEC_LEE)
                            {
                                if(sumByTotal>=Prod.iLowYieldCountByTotal)
                                {
                                    bAlarm=true;
                                }
                            }
                            else
                            {
                                if(sumByTotal-iYeildCT[4]>=Prod.iLowYieldCountByTotal)
                                {
                                    bAlarm=true;
                                }
                            }
                        }

                        if(bAlarm)                                              //JerryYang 20180302 fix Yield alarm by total失效
                        {
                            if(bLowYeildAlarm==false)
                            {
                                ret=DoLowYieldAlarm("WAR0705", SocketErrPart);  //Steven 20180627 (wei) : 整合Low Yield Alarm
                                if(ret==K_ONECYCLE)
                                {
                                    bLowYeildAlarm=true;
                                }
                                else
                                {
                                    iYeildCT[4]=sumByTotal;
                                }
                            }
                        }
                    }
                }
                else
                {
                    if(iLowYieldByTotalContactCount>0)
                    {
                        iYieldByTotal=ChangeToFloat(double(ipassByTotal), double(sumByTotal));
                        bAlarm=false;                                           //JerryYang 20180302 fix Yield alarm by total失效
                        if(Prod.dLowYieldLimitByTotal>iYieldByTotal)
                        {
                            if(iLowYieldByTotalContactCount>=Prod.iLowYieldCountByTotal)
                            {
                                iLowYieldByTotalContactCount=0;
                                bAlarm=true;
                            }
                        }
                        if(bAlarm)                                              //JerryYang 20180302 fix Yield alarm by total失效
                        {
                            if(bLowYeildAlarm==false)
                            {
                                ret=DoLowYieldAlarm("WAR0705", SocketErrPart);  //Steven 20180627 (wei) : 整合Low Yield Alarm
                                if(ret==K_ONECYCLE)
                                {
                                    bLowYeildAlarm=true;
                                }

                            }
                        }
                    }
                }

                str.sprintf("%0.2f", iYieldByTotal);
                fShowBinSelect->labTotalYield->Caption=str.c_str();
                if(CosFunction.bYieldControlUseEACount ||                       //Kaichen 20190628 : Low Yield ByTotal 控制使用 Contact Count
                   CUSTOMER_CODE==CC_JCET)                                      //Steven 20231212 : JCET這一項使用的是EA Count
                    fShowBinSelect->labTotalYieldTotal->Caption=sumByTotal;
                else
                    fShowBinSelect->labTotalYieldTotal->Caption=iLowYieldByTotalContactCount;
            }
        }
        else
        {
            fShowBinSelect->labTotalYield->Caption=0;
            fShowBinSelect->labTotalYieldTotal->Caption=0;
        }
    }
    else
    {
        iCount4=0;
    }
}
//---------------------------------------------------------------------------
void TfYieldMonitoring::ClearYieldCount()                                       //Steven 20140830 : Yield相關的Alarm, 要清掉全部的Ignore的Count重算
{
    if(CosFunction.bYieldAlarmClearAllCount)
    {
        MyDBIProductionData("Clear yield count");                               //Steven 20140816 : Production Data
        iLowYieldContactCount=0;
        iLowYieldByTotalContactCount=0;                                         //Kaichen 20190628 : Low Yield ByTotal 控制使用 Contact Count
        iFailAlarmSiteYieldIntervalCount=0;
        iPickerYieldIntervalCount=0;                                            //Steven 20230223 : 根據Index吸嘴比較良率
        iFailAlarmSiteMaxYieldIntervalCount=0;
        fShowBinSelect->iLowYieldBinSelectContactCount=0;                       //KaiChen 20181115 : BinSelect裡面 Yield控制使用 Contact Count

        for(int i=0; i<MAX_SOCKET_ROW; i++)
        {
            for(int j=0; j<MAX_SOCKET_COL; j++)
            {
                ContinuousFailSKTCount[i][j]=0;
                SpecialBinContinuousFailSKTCount[i][j]=0;
                SpecialBinContinuousFailARMCount[0][i][j]=0;
                SpecialBinContinuousFailARMCount[1][i][j]=0;
                ContinuousFailARMCount[0][i][j]=0;
                ContinuousFailARMCount[1][i][j]=0;
            }
        }

        for(int i=0; i<TEST_MAX_BIN; i++)
        {
            iLoadPersentCT[i]=RunInfo.iUnloadCount;                             //Steven 20140830 : 改成全域變數        //Steven 20140905 : LastSet.SendCT[0] --> RunInfo.iUnloadCount
            iLoadCountCT[i]  =RunInfo.iUnloadCount;                             //Steven 20140830 : 改成全域變數        //Steven 20140905 : LastSet.SendCT[0] --> RunInfo.iUnloadCount
            iYeildCT[i]      =RunInfo.iUnloadCount;                             //wei 20151111
        }
        bFirstCount=true;
    }
}
//------------------------------------------------------------------------------
void TfYieldMonitoring::DoRTAutoSocketOff()                                     //Steven 20200205 : 切到RT的時候,要關閉Socket
{
    double dMaxYield=0, dYield[2][MAX_SOCKET_ROW][MAX_SOCKET_COL];
    double dTargetYield;

    if(CosFunction.bUseSCKART &&
       USE_AUTO_RETEST==eartInstall &&
       IniConfig.bA10_AutoReTest &&
       TestIF_File.bSCKART_EnableART==true)
        return;

    if(CosFunction.bAutoCloseSiteWhenRT)
    {
        if(TestIF_File.iAutoCloseSiteWhenRT)
        {
            if(IniConfig.bA09_ByArmCloseSite==0)                                //Auto Head
            {
                for(int i=0; i<TestSocket.iShtRow; i++)                         //沒考慮到NN mode
                {
                    for(int j=0; j<TestSocket.iShtCol; j++)
                    {
                        dYield[0][i][j]=ArmData[0]->ArmSKET[i][j]->GetPCA();
                        if(dYield[0][i][j]>dMaxYield)
                        {
                            dMaxYield=dYield[0][i][j];
                        }

                        dYield[1][i][j]=ArmData[1]->ArmSKET[i][j]->GetPCA();
                        if(dYield[1][i][j]>dMaxYield)
                        {
                            dMaxYield=dYield[1][i][j];
                        }
                    }
                }

                dTargetYield=dMaxYield-TestIF_File.dAutoCloseSiteYieldWhenRT;

                if(dTargetYield>0)
                {
                    for(int k=0; k<2; k++)
                    {
                        for(int i=0; i<TestSocket.iShtRow; i++)
                        {
                            for(int j=0; j<TestSocket.iShtCol; j++)
                            {
                                if(dYield[k][i][j]<dTargetYield)
                                {
                                    bLowYieldCloseSite[k][i][j]==true;
                                }
                            }
                        }
                    }
                }
            }
            else   //Auto Socket
            {
                for(int i=0; i<TestSocket.iShtRow; i++)
                {
                    for(int j=0; j<TestSocket.iShtCol; j++)
                    {
                        dYield[0][i][j]=0;
                        if(LastSet.bUseTestSocket[0][i][j] &&
                           LastSet.bUseTestSocket[1][i][j])
                            dYield[0][i][j]=(ArmData[0]->ArmSKET[i][j]->GetPCA()+ArmData[1]->ArmSKET[i][j]->GetPCA())/2.0;
                        else if(LastSet.bUseTestSocket[0][i][j])
                            dYield[0][i][j]=ArmData[0]->ArmSKET[i][j]->GetPCA();
                        else
                            dYield[1][i][j]=ArmData[1]->ArmSKET[i][j]->GetPCA();

                        if(dYield[0][i][j]>dMaxYield)
                        {
                            dMaxYield=dYield[0][i][j];
                        }
                    }
                }

                dTargetYield=dMaxYield-TestIF_File.dAutoCloseSiteYieldWhenRT;
                if(dTargetYield>0)
                {
                    for(int i=0; i<TestSocket.iShtRow; i++)
                    {
                        for(int j=0; j<TestSocket.iShtCol; j++)
                        {
                            if(dYield[0][i][j]<dTargetYield)
                            {
                                bLowYieldCloseSite[0][i][j]==true;
                                bLowYieldCloseSite[1][i][j]==true;
                            }
                        }
                    }
                }
            }
            fYieldMonitoring->DoAutoCloseSite(2);                               //Steven 20170905 (wei) : Low Yield Auto Site Off for Ambient
            fMain->ShowTestHeadComp(false);
        }
    }
}
//---------------------------------------------------------------------------
bool TfYieldMonitoring::CanAutoCloseSite(int iAllSiteOn)                        //Steven 20230315 : 整合自動關Site功能的判斷
{
    bool bFlag=false;

    if(iAllSiteOn==1)
    {
        bFlag=TestIF_File.iAllSiteOnAtInitialStart;                             //Steven 20230814 : Initial Start的時候要全開Site
    }
    else if(iAllSiteOn==2)
    {
        if(CosFunction.bAutoCloseSiteWhenRT)
        {
            if(CosFunction.bUseSCKART && USE_AUTO_RETEST==eartInstall && IniConfig.bA10_AutoReTest && TestIF_File.bSCKART_EnableART==true)
                bFlag=false;
            else if(TestIF_File.iAutoCloseSiteWhenRT)
                bFlag=true;
        }
    }
    else
    {
        if(CosFunction.bLowYieldAutoSiteOff && iRunStartMode==FT)
        {
            if((TestIF_File.bLowYieldAutoSiteOff && Prod.bFailAlarmSiteYieldCmp) ||                   //Steven 20170905 (wei) : Low Yield Auto Site Off for Ambient
               (TestIF_File.bLowYieldAutoSiteOffByContiFail && Prod.bContsFailBySocket) ||            //Steven 20200420 : Continue fail, auto site off
               (TestIF_File.bLowYieldAutoSiteOffArmContiFail && Prod.bContsFailByHead && IniConfig.bA09_ByArmCloseSite) ||  //Steven 20220818 : By Arm Continue fail, auto site off
               (TestIF_File.bLowYieldAutoSiteOffByPicker && Prod.bLowYieldByPicker && IniConfig.bA09_ByArmCloseSite) ||     //Steven 20230223 : 根據Index吸嘴比較良率
               (TestIF_File.iAutoSiteOffByGPIB==1 && bGetGPIBAutoSiteOff))      //JimmyChiu 20250715 : Auto site on/off by GPIB
            {
                bFlag=true;
            }
        }
    }
    bCanAutoCloseSite=bFlag;
    return bFlag;
}
//---------------------------------------------------------------------------
//0: Low Yield關
//1: 全開
//2: RT關Site
//---------------------------------------------------------------------------
void TfYieldMonitoring::DoAutoCloseSite(int iAllSiteOn)                         //Steven 20170905 (wei) : Low Yield Auto Site Off for Ambient
{
    CanAutoCloseSite(iAllSiteOn);                                               //Steven 20230315 : 整合自動關Site功能的判斷
    if(bCanAutoCloseSite)                                                       //Steven 20210809 : 改成可以強制全開
    {
        if(iAllSiteOn==1)                                                       //Steven 20230315 : 整合自動關Site功能的判斷
        {
            for(int i=0; i<TestSocket.iShtRow; i++)
            {
                for(int j=0; j<TestSocket.iShtCol; j++)
                {
                    if(TestIF_File.iSiteMap[i][j]!=0)
                    {
                        if(LastSet.iCloseSiteByLowYield[0][i][j]==1)            //Steven 20220223 : 紀錄Auto Site Off的位置
                            LastSet.bUseTestSocket[0][i][j]=true;

                        if(LastSet.iCloseSiteByLowYield[1][i][j]==1)
                            LastSet.bUseTestSocket[1][i][j]=true;
                    }
                    LastSet.iCloseSiteByLowYield[0][i][j]=0;
                    LastSet.iCloseSiteByLowYield[1][i][j]=0;
                }
            }
            fMain->ShowTestHeadComp(false);                                     //Steven 20220308 : true --> false
            fYieldMonitoring->ClearAutoSiteOffStatus();                         //Steven 20200409 : 修正清除count之後,不能開site的問題
            RecordProcess("All site on for auto site off function.");
        }
        else
        {
            for(int a=0; a<2; a++)
            {
                for(int i=0; i<TestSocket.iShtRow; i++)
                {
                    for(int j=0; j<TestSocket.iShtCol; j++)
                    {
                        if(LastSet.bUseTestSocket[a][i][j] &&
                           bLowYieldCloseSite[a][i][j]==true)
                        {
                            LastSet.bUseTestSocket[a][i][j]=false;
                            LastSet.iCloseSiteByLowYield[a][i][j]=1;            //Steven 20220223 : 紀錄Auto Site Off的位置
                            bTestSiteUse[a][i][j]=false;
                            bHasCloseSite=true;
                            FormHS->SaveCloseOpenSiteEven(a, i, j, bTestSiteUse[a][i][j]); //Isaac 20171227 (Steven) : 記錄low yield auto site off log
                        }
                    }
                }
            }
            if(bHasCloseSite)
            {
                fMain->ShowTestHeadComp(false);
            }
            bGetGPIBAutoSiteOff=false;                                          //JimmyChiu 20250715 : Auto site on/off by GPIB
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::edtSiteYieldCmp1SiteCnt_FTMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    if(Barcode_Reader(bcYield)==0)                                              // 20140103 wei KYEC Barcode Reader
    {
        return;
    }
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 32, 2);      //Steven 20141120 : Modify
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::edAlarm5_OSBinMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 1, 8);       //Sam 20171213 (Steven) : 超豐良率監控
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::edtAutoSiteOffWhenRTMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    if(Barcode_Reader(bcYield)==0)                                              // 20140103 wei KYEC Barcode Reader
    {
        return;
    }

    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 50.00, 0.01); //kevin 20150630 int-> float
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::cbIntervalLowYieldBySite_FTMouseUp(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    if(Barcode_Reader(bcYield)==0)                                              // 20140103 wei KYEC Barcode Reader
    {
        cbIntervalLowYieldBySite_FT->Checked=TestIF_File.bFailAlarmIntervalLowYieldBySite;
        return;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::cbLowYieldByTotal_FTMouseUp(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    if(Barcode_Reader(bcYield)==0)                                              // 20140103 wei KYEC Barcode Reader
    {
        cbLowYieldByTotal_FT->Checked=TestIF_File.bFailAlarmLowYieldByTotal;
        return;
    }
    if(IniConfig.bSIGURDFunction)                                               //KaiChen 20190626 ：矽格要求 LowYieldByTotal & SiteYieldCmp 開關同步
    {
        cbSiteYieldCmp_FT->Checked=cbLowYieldByTotal_FT->Checked;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::cbSiteYieldCmp_RTMouseUp(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    if(Barcode_Reader(bcYield)==0)                                              // 20140103 wei KYEC Barcode Reader
    {
        cbSiteYieldCmp_RT->Checked=TestIF_File.bFailAlarmSiteYieldCmp_RT;
        return;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::cbLowYieldByTotal_RTMouseUp(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    if(IniConfig.bSIGURDFunction)                                               //KaiChen 20190626 ：矽格要求 LowYieldByTotal & SiteYieldCmp 開關同步
    {
        cbSiteYieldCmp_RT->Checked=cbLowYieldByTotal_RT->Checked;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::cbIntervalLowYieldBySite_RTMouseUp(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    if(Barcode_Reader(bcYield)==0)                                              // 20140103 wei KYEC Barcode Reader
    {
        cbIntervalLowYieldBySite_RT->Checked=TestIF_File.bFailAlarmIntervalLowYieldBySite_RT;
        return;
    }
}
//---------------------------------------------------------------------------
void TfYieldMonitoring::CheckIntervalLowYieldAlarmBySite()                      //Interval Low Yield Alarm(%)by Site //wei 20180606 Interval Low Yield By Site
{
    static int iCount5=0;

    int iAlarmTimeInterval=(CosFunction.bLowYieldAlarmIntervalTimeBySetting &&  //JimmyChiu 20220601 : 修改low yield報警時間邏輯，報警間隔時間固定1min改為可以自行設定報警間隔時間
                            IniConfig.bI44_LowYieldAlarmIntervalTimeBySetting)?IniConfig.iI44_LowYieldAlarmIntervalTime:60;

    bool bLowYieldbySite=false;
    double dYield1[2][4][8]={0.0};
    AnsiString ErrPart="";

    if(bFirstCount)
    {
        iCount5=0;
        return;
    }

    if(SystemStart &&
       fContact->fShow==false &&
       iHome==0 &&
       bRunAutoClean==false &&
       bZ1PickShuttle==false &&
       bZ2PickShuttle==false)                                                   //Steven 20180521 : 避免吸料的時候發Low Yield Alarm
    {
        memset(iYieldSiteBinpass, 0, sizeof(iYieldSiteBinpass));
        fShowBinSelect->ShowCategoryBin();
        if(Prod.bFailAlarmIntervalLowYieldBySite &&
           Prod.dIntervalLowYieldLimitBySite!=0 &&                              //JerryYang 20160530 LowYieldLimit要能設定到小數點
           CosFunction.IntervalYieldCount)
        {
            iCount5++;
            if(iCount5>=iAlarmTimeInterval ||                                   //JimmyChiu 20220601 : 修改low yield報警時間邏輯，報警間隔時間固定1min改為可以自行設定報警間隔時間
               CosFunction.bYieldAlarmNoWait1Min)                               //wei 20150820  Yield Alarm No Wait 1Min
            {
                iCount5=0;

                for(int i=0; i<TestSocket.iShtRow; i++)
                {
                    for(int j=0; j<TestSocket.iShtCol; j++)
                    {
                        for(int l=0; l<2; l++)
                        {
                            for(int k=0; k<Prod.iIntervalLowYieldCountBySite; k++)
                            {
                                if(bIntervalYieldIsPass[l][i][j][k]==true)
                                {
                                    iYieldSiteBinpass[l][i][j]++;
                                }
                            }
                            dYield1[l][i][j]=ChangeToFloatNonPcnt((double)(iYieldSiteBinpass[l][i][j]*100), (double)(Prod.iIntervalLowYieldCountBySite));
                        }

                        if(bUseTwoArm32Site==true)                              //Steven 20220418 : NN mode Yield alarm
                        {
                            dSiteYield[i][j]=dYield1[0][i][j];
                        }
                        else if(TestIF.iShuttleMode==0)
                        {
                            dSiteYield[i][j]=(dYield1[0][i][j]+dYield1[1][i][j])/2.0;
                        }
                        else if(TestIF.iShuttle_Sel==0)
                        {
                            dSiteYield[i][j]=dYield1[0][i][j];
                        }
                        else
                        {
                            dSiteYield[i][j]=dYield1[1][i][j];
                        }

                        if(dSiteYield[i][j]<Prod.dIntervalLowYieldLimitBySite)  //JerryYang 20160530 LowYieldLimit要能設定到小數點
                        {
                            bLowYieldbySite=true;
                            ErrPart+=IndexSuckName[i][j];
                        }
                    }
                }
                fContactCT->sgYield->Refresh();

                if((bYieldSiteBin[0] || bYieldSiteBin[1]) && bYieldSiteBinCheck)
                {
                    if(bLowYieldbySite==true)
                    {
                        ShowErrorMessage("WAR0721", K_RETRY, MMInterface, false);
                        bLowYieldbySite=false;
                    }
                    bYieldSiteBinCheck=false;
                }
            }
        }
        else
        {
            for(int i=0; i<2; i++)
            {
                iYieldSiteCount[i]=0;
                bYieldSiteBin[i]=false;
            }
        }
    }
    else
    {
        iCount5=0;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::edIntervalLowYieldBySiteIg_FTMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    if(Barcode_Reader(bcYield)==0)                                              // 20140103 wei KYEC Barcode Reader
    {
        return;
    }
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 300, 1);     //Steven 20141120 : Modify
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::cbIntervalLowYieldByTotal_FTMouseUp(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    if(Barcode_Reader(bcYield)==0)                                              // 20140103 wei KYEC Barcode Reader
    {
        cbIntervalLowYieldByTotal_FT->Checked=TestIF_File.bFailAlarmIntervalLowYieldByTotal;
        return;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::cbIntervalLowYieldByTotal_RTMouseUp(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    if(Barcode_Reader(bcYield)==0)                                              // 20140103 wei KYEC Barcode Reader
    {
        cbIntervalLowYieldByTotal_RT->Checked=TestIF_File.bFailAlarmIntervalLowYieldByTotal_RT;
        return;
    }
}
//---------------------------------------------------------------------------
void TfYieldMonitoring::CheckIntervalLowYieldAlarmByTotal()                     //Interval Low Yield Alarm(%)by Total   //wei 20180718 Interval Low Yield By Total
{
    static int iCount6=0;

    int iAlarmTimeInterval=(CosFunction.bLowYieldAlarmIntervalTimeBySetting &&  //JimmyChiu 20220601 : 修改low yield報警時間邏輯，報警間隔時間固定1min改為可以自行設定報警間隔時間
                            IniConfig.bI44_LowYieldAlarmIntervalTimeBySetting)?IniConfig.iI44_LowYieldAlarmIntervalTime:60;

    bool bLowYieldbyTotal=false;
    double dYield1=0.0;

    if(bFirstCount)
    {
        iCount6=0;
        return;
    }

    if(SystemStart &&
       fContact->fShow==false &&
       iHome==0 &&
       bRunAutoClean==false)                                                    //ChungHung 20131223 add for SCK Autoclean
    {
        iYieldTotalBinpass=0;
        fShowBinSelect->ShowCategoryBin();
        if(Prod.bFailAlarmIntervalLowYieldByTotal &&
           Prod.dIntervalLowYieldLimitByTotal!=0 &&
           CosFunction.IntervalYieldCount)                                      //JerryYang 20160530 LowYieldLimit要能設定到小數點
        {
            iCount6++;
            if(bZ1PickShuttle || bZ2PickShuttle)                                //Steven 20180521 : 避免吸料的時候發Low Yield Alarm
                return;
            if(iCount6>=iAlarmTimeInterval ||                                   // 1 mimutes    //JimmyChiu 20220601 : 修改low yield報警時間邏輯，報警間隔時間固定1min改為可以自行設定報警間隔時間
               CosFunction.bYieldAlarmNoWait1Min)                               //wei 20150820  Yield Alarm No Wait 1Min
            {
                iCount6=0;

                for(int k=0; k<Prod.iIntervalLowYieldCountByTotal; k++)
                {
                    if(bYieldTotalBinIsPass[k]==true)
                    {
                        iYieldTotalBinpass++;
                    }
                }
                dYield1=ChangeToFloatNonPcnt((double)(iYieldTotalBinpass*100), (double)(Prod.iIntervalLowYieldCountByTotal));
                fShowBinSelect->IntervalByTotal->Caption=dYield1;

                if(dYield1<Prod.dIntervalLowYieldLimitByTotal)                  //JerryYang 20160530 LowYieldLimit要能設定到小數點
                {
                    bLowYieldbyTotal=true;
                }

                if(bYieldTotalBin && bYieldSiteBinCheck)
                {
                    if(bLowYieldbyTotal==true)
                    {
                        ShowErrorMessage("WAR0722", K_RETRY, MMInterface, false);
                        bLowYieldbyTotal=false;
                        bYieldTotalBin=false;
                        iYieldTotalCount=0;
                        memset(bYieldTotalBinIsPass, false, sizeof(bYieldTotalBinIsPass));
                    }
                    bYieldSiteBinCheck=false;
                }
            }
        }
        else
        {
            iYieldTotalCount=0;
            bYieldTotalBin=false;
        }
    }
    else
    {
        iCount6=0;
    }
}
//---------------------------------------------------------------------------
void TfYieldMonitoring::CheckLowYieldAlarmSpecial()                             //Sam 20210505 : PTI 要求的兩段 Low Yeild
{
    static int iCount5=0;

    int iSum=0, iPass=0;
    bool bIsLowYield=false;
    bool bNeedClear=false;
    double dYield1=0.0;
    AnsiString SocketErrPart="",str="";

    if(bFirstCount)
    {
        iCount5=0;
        return;
    }

    if(SystemStart &&
       fContact->fShow==false &&
       iHome==0 &&
       bRunAutoClean==false)
    {
        if(Prod.bFailAlarmLowYieldSpecial &&
           Prod.dLowYieldLimitSpecial!=0 &&
           CosFunction.bSpecailLowYeild)
        {
            iCount5++;
            if(bZ1PickShuttle || bZ2PickShuttle)
                return;

            //if(iCount5>=60 || CosFunction.bYieldAlarmNoWait1Min)              // 1 mimutes  //wei 20150820  Yield Alarm No Wait 1Min
            {
                iCount5=0;
                iSum=0;
                iPass=0;
                bIsLowYield=false;
                bNeedClear=false;

                for(int i=0; i<eTrayCount; i++)                                 //JerryYang 20230925
                {
                    if(Prod.iTrayType[i]==tNotUse)
                        continue;
                    iSum+=LastSet.BinCT_PTI[0][iTo3Unload[i]];
                    if(Prod.iIsPassT6[i]==1)                                    //Steven 20240105 : Prod.bIsPass --> Prod.iIsPassT6
                        iPass+=LastSet.BinCT_PTI[0][iTo3Unload[i]];
                }

                if(iSum>0)
                {
                    dYield1=ChangeToFloat(double(iPass), double(iSum));
                    if(Prod.dLowYieldLimitSpecial>dYield1)
                        bIsLowYield=true;
                }

                if(iSum>Prod.iLowYieldCountSpecial1 &&
                   bLowYeildAlarmSpecial1stPass==false)
                {
                    bNeedClear=true;
                    if(bIsLowYield)
                    {
                        bLowYeildAlarmSpecial=true;
                        fMain->CleanOut("CheckLowYieldAlarmSpecial");           //第一段 Low Yield 做 CleanOut 後報警提示修機
                    }
                    else
                    {
                        bLowYeildAlarmSpecial1stPass=true;                      //第一段 Low Yield 檢查過了
                    }
                }
                else if(iSum>Prod.iLowYieldCountSpecial2)
                {
                    bNeedClear=true;
                    if(bIsLowYield)
                    {
                        DoLowYieldAlarm("WAR0725", SocketErrPart);              //第二段 Low Yield 直接報警
                        //bLowYeildAlarmSpecial1stPass=false;                   //Sam 20211221 : Lot Start 才需要重新第一階段檢查 Mark //重新檢查第一段
                    }
                }

                if(bNeedClear)
                {
                    for(int i=0; i<10; i++)
                    {
                        LastSet.BinCT_PTI[0][i]=0;
                        LastSet.BinCT_PTI[2][i]=0;
                        LastSet.BinCT_PTI[3][i]=0;
                    }
                }

                str.sprintf("%0.2f", dYield1);
                fShowBinSelect->lblSpeciallYield->Caption=str.c_str();
                fShowBinSelect->lblSpeciallYieldTotal->Caption=iSum;
            }
        }
        else
        {
            fShowBinSelect->lblSpeciallYield->Caption=0;
            fShowBinSelect->lblSpeciallYieldTotal->Caption=0;
        }
    }
    else
    {
        iCount5=0;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::DoSetRPDefault(TWinControl *PCtrl,int iSection) //Isaac 20170527 (Steven) defalut值比較功能
{
    AnsiString szDir="D:\\HT9045\\IniData\\RPDefault.ini", aSection="";
    AnsiString aHintName;
    AnsiString sRet;

    if(iSection==0)
        aSection="Yield";

    for(int iP=0; iP<PCtrl->ControlCount; iP++)
    {
        TControl *P=PCtrl->Controls[iP];
        if(dynamic_cast <TWinControl *>(P)!=NULL)
        {
            DoSetRPDefault((TWinControl *) P, iSection);                        // 找該物件附屬的物件(遞迴)
        }
        TEdit           *PEdit      =dynamic_cast <TEdit        *>(P);
        TLabeledEdit    *PLEdit     =dynamic_cast <TLabeledEdit *>(P);          //Steven 20200715 : 補上TLabeledEdit
        TCheckBox       *PTCheckBox =dynamic_cast <TCheckBox    *>(P);

        if(PEdit!=NULL && PEdit->Hint!="")
        {
            aHintName=PEdit->Hint;
            WriteIniData(szDir, aSection, aHintName, PEdit->Text);
        }
        else if(PLEdit!=NULL && PLEdit->Hint!="")
        {
            aHintName=PLEdit->Hint;
            WriteIniData(szDir, aSection, aHintName, PLEdit->Text);
        }
        else if(PTCheckBox!=NULL && PTCheckBox->Hint!="")
        {
            aHintName=PTCheckBox->Hint;
            sRet=(PTCheckBox->Checked)?"1":"0";
            WriteIniData(szDir, aSection, aHintName, sRet);
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::DoReplyDefaultToForm(TWinControl *PCtrl, int iSection)
{
    AnsiString aHintName;
    AnsiString szDir="D:\\HT9045\\IniData\\RPDefault.ini";
    AnsiString aSection="";
    if(iSection==0)
    aSection="Yield";

    AnsiString Stringtemp="";
    for(int iP=0; iP<PCtrl->ControlCount; iP++)
    {
        TControl *P=PCtrl->Controls[iP];
        if(dynamic_cast <TWinControl *>(P)!=NULL)
        {
            DoReplyDefaultToForm((TWinControl *)P, iSection);                   // 找該物件附屬的物件(遞迴)
        }
        TEdit           *PEdit      =dynamic_cast <TEdit         *>(P);
        TLabeledEdit    *PLEdit     =dynamic_cast <TLabeledEdit  *>(P);
        TCheckBox       *PTCheckBox =dynamic_cast <TCheckBox     *>(P);

        if(PEdit!=NULL ||
           PLEdit!=NULL)
        {
            aHintName=PEdit->Hint;
            if(CheckIniData(szDir, aSection, aHintName))
            {
                Stringtemp = ReadIniData(szDir, aSection, aHintName, Stringtemp);
                PEdit->Text= Stringtemp;
            }
        }
        else if(PTCheckBox!=NULL && PTCheckBox->Hint!="")
        {
            aHintName=PTCheckBox->Hint;
            if(CheckIniData(szDir, aSection, aHintName))
            {
                Stringtemp   =ReadIniData(szDir, aSection, aHintName, Stringtemp);
                PTCheckBox->Checked=(Stringtemp=="1")?true:false;
            }
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::SearchRecipeParameter(TWinControl *PCtrl, int iSection)
{
    AnsiString aHintName;
    AnsiString szDir="D:\\HT9045\\IniData\\RPDefault.ini";
    AnsiString aSection="";
    if(iSection==0)
        aSection="Yield";
    AnsiString Stringtemp="";
    AnsiString asDefaultValue="";

    for(int iP=0; iP<PCtrl->ControlCount; iP++)
    {
        TControl *P=PCtrl->Controls[iP];
        if(dynamic_cast <TWinControl *>(P)!=NULL)
        {
            SearchRecipeParameter((TWinControl *)P, iSection);                  // 找該物件附屬的物件(遞迴)
        }
        TEdit           *PEdit      =dynamic_cast <TEdit         *>(P);
        TLabeledEdit    *PLEdit     =dynamic_cast <TLabeledEdit  *>(P);
        TCheckBox       *PTCheckBox =dynamic_cast <TCheckBox     *>(P);

        if(PEdit!=NULL ||
           PLEdit!=NULL)
        {
            aHintName=PEdit->Hint;
            if(CheckIniData(szDir, aSection, aHintName))
            {
                asDefaultValue=CheckAndReadIniData(szDir, aSection, aHintName, Stringtemp);
                if(CosFunction.bRecipeParameterDefaultLog && asDefaultValue=="")  //Sam 20201209 : Default Recipe ChangeLog
                {
                    //Default 數值被手動刪除掉的話，就不要監控了
                }
                else
                {
                    fRPDefault->RP_Name->Add(aHintName);
                    fRPDefault->RP_Value->Add(PEdit->Text);
                    fRPDefault->RP_Default->Add(asDefaultValue);
                }
            }
        }
        else if(PTCheckBox!=NULL)                                               //Sam 20201209 : Default Recipe ChangeLog
        {
            aHintName=PTCheckBox->Hint;
            if(CheckIniData(szDir, aSection, aHintName))
            {
                asDefaultValue=CheckAndReadIniData(szDir, aSection, aHintName, Stringtemp);
                if(CosFunction.bRecipeParameterDefaultLog &&
                   asDefaultValue=="")                                          //Sam 20201209 : Default Recipe ChangeLog
                {
                                                                                //Default 數值被手動刪除掉的話，就不要監控了
                }
                else
                {
                    fRPDefault->RP_Name->Add(aHintName);
                    fRPDefault->RP_Value->Add((PTCheckBox->Checked)?"1":"0");
                    fRPDefault->RP_Default->Add(asDefaultValue);
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::FormShortCut(TWMKey &Msg, bool &Handled)
{
    if(Msg.CharCode==VK_F5)                                                     //Sam 20220725 : 介面新增鍵盤 F5 將視窗恢復初始位置
    {
        Left=200;
        Top=50;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::edtAutoSiteOffAlarmMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 10, 2);      //Sam 20221207 : LowYieldAutoSiteOff 新增 Alarm 幾次後再來關 Site
}
//------------------------------------------------------------------------------
void __fastcall TfYieldMonitoring::btnResetIntervalClick(TObject *Sender)
{
    dAdaptiveStardardYield=-0.01;
    fLotInfo->RefreshYieldMonitor();
}
//------------------------------------------------------------------------------
void TfYieldMonitoring::SetClosedSiteBin()                                      //Steven 20240409 : 關site的位置有IC不測試送指定 bin
{
    cbbClosedSiteBin->Clear();
    for(int i=0; i<iTestBinCount; i++)
    {
        cbbClosedSiteBin->Items->Add(i);                                        //Steven 20240409 : 關site的位置有IC不測試送指定 bin
    }
    cbbClosedSiteBin->Items->Add("Error");
}
//------------------------------------------------------------------------------
