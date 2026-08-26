//******************************************************************************
//
//  注意!! cBinSel為Handler Bin設定相關, 修改時要小心!!
//
//******************************************************************************
#include "MachineDefine.h"
#pragma hdrstop

#include "cBinSel.h"

#include "MachineType.h"
#include "cShowBinSelect.h"
//#include "INPUT.h"
#include "cprod.h"
#include "cmydef.h"
#include "MyKitSuck.h"
#include "csystem.h"
#include "MyMotor.h"
#include "cMyDB.h"
#include "database.h"
#include "common.h"
#include "main.h"
#include "mymessbox.h"
#include "cAuthority.h"
#include "cSecurity.h"
#include "BarcodeReader.h"                                                      // 2013.11.29 , Joye , KYEC Barcode Reader  20140103 wei
#include "Password.h"
#include "myQwertyKeyBoard.h"
#include "note.h"
#include "HTray256.h"
#include "cBuilder.h"
#include "systools.h"                                                           //Steven 20140531 : 為了加快ini讀取速度
#include "SCK_ART.h"
#include "cinitial.h"
#include "fRotate.h"
#include "main.h"
#include "QAMode.h"
#include "cSocket.h"
#include "ProductionInfo.h"                                                     //Sam 20200622 : Control Bin 顯示
#include "uLotInfo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "HTray"
#pragma resource "*.dfm"
TfBinSel *fBinSel;

e6TrayName iLinkTrayMin        =eFix2;
e6TrayName iLinkTrayMax        =eFix3;

bool bCanLinkT6[eTrayCount];
enum eBinSettingItems
    {   eBinNo          =0,                                                     //Bin名稱的位置
        eScanning       =1,
        eDoubleContact  =2,
        eConsFail       =3,
        ePersentEnable  =4,                                                     //Steven 20140529 Start: Fail Persent & Count同時存在
        ePersentIgnore  =5,
        ePersentNumber  =6,
        eCountEnable    =7,
        eCountIgnore    =8,
        eCountNumber    =9,

        eSpecialBinByArm            =10,                                        //ChungHung 20140730 add ContinuousFailHaveOneCycle start
        eSpecialBinCountByArm       =11,
        eSpecialBinBySocket         =12,
        eSpecialBinCountBySocket    =13,

        eLowYield       =14,                                                    //Steven 20140828 : By Bin Yield Monitor
        eArmYield       =15,                                                    //Steven 20140828 : By Bin Arm Yield Monitor
        eSiteYield      =16,                                                    //Steven 20140828 : By Bin Site Yield Monitor
        eAutoCleanByBin =17,                                                    //Steven 20160308 : By Bin count do auto clean
        eAutoCleanBySite=18,                                                    //Steven 20160308 : By Bin pre site count do auto clean

        eSpecBinBySiteCompareEnable      =19,                                   //JerryYang 20170712 (Steven) by site by bin compare percent
        eSpecBinBySiteCompareIgnore      =20,
        eSpecBinBySiteComparePercent     =21,
        eSpecBinByArmPerSiteCompareEnable=22,
        eSpecBinByArmPerSiteCompareIgnore=23,
        eSpecBinByArmPerSiteComparePercent=24,

        eBinNotUse      ,                                                       //Bin未使用的位置
        eBinSetting     ,                                                       //Bin設定的起始點
        eBinSetTotal    =(eBinSetting+eTrayCount)
    };

const int iRowHeight=20;                                                        //JerryYang (Steven) 20170712 height24->20
const int iColWidth =40;
bool bEditVisible=false;

const int eItemName   =0;                                                       //名稱的位置
enum eTrayNameFunc                                                              //使用的功能
    {   eItemLink   =0,                                                         //Tray Link功能
        eItemPass   =1,                                                         //Tray為Pass或Fail
        eItemError  =2,                                                         //Tray指定為Error
        eItemCateR  =3,                                                         //Steven 20161221 (wei) : Cate R for ART
        eItemART    =4,                                                         //Tray指定AutoRetest  //ChungHung 20140317 add Auto Retest
        eItemTotal
    };

enum eTrayColorMap
    {   eCLWhite        =0,
        eCLGreen        =1,
        eCLRed          =2,
        eCLYellow       =3,
        eCLPurple       =4,
        eCLBlue         =5,
        eCLGray         =6,
        eCLSilver       =7,
        eCLBtnFace      =8,
        eCLOlive        =9
    };

bool bWriteNewBinFormat=false;                                                  //kevin 20160909 新的 Bin format
const int iBinCount=TEST_MAX_BIN;                                               //使用的Bin數量
//---------------------------------------------------------------------------
class PACKAGE TMyBinPanel : public TComponent
{
    public:
        __fastcall TMyBinPanel(TComponent* Owner, int index, TTabSheet *parent, AnsiString Name);
        __fastcall ~TMyBinPanel();
        TPanel      *Panel;
        TTMyTray256 *mtTrayItem;
        TTMyTray256 *mtTrayName;
        TTMyTray256 *mtBinSelect;
        TTMyTray256 *mtBinSelectBy;                                             //Sam 20200622 : Control Bin 顯示
        TScrollBox  *sbTraySetting;                                             //Steven 20240420 : 調整scrollbar位置
        TScrollBox  *sbBinSetting;
        TScrollBar  *sbBinSetScroll;                                            //Steven 20240420 : 調整scrollbar位置
        TEdit       *edBinSetT3Pos;
        TEdit       *edBinSetT6Error;
        TEdit       *edBinSetT6PassFail;
        TEdit       *edBinSetT6Link;
        TEdit       *edBinSettingEnableScan;
        TEdit       *edBinSetting2Contact;
        TEdit       *edBinSettingConFail;

        TEdit       *edBinSettingFailPersentEnable;                             //Steven 20140529 Start: Fail Persent & Count同時存在
        TEdit       *edBinSettingFailPersentIgnore;
        TEdit       *edBinSettingFailPersentNumber;
        TEdit       *edBinSettingFailCountEnable;
        TEdit       *edBinSettingFailCountIgnore;
        TEdit       *edBinSettingFailCountNumber;

        TEdit       *edSpecialBinByArm;                                         //ChungHung 20140730 add ContinuousFailHaveOneCycle start
        TEdit       *edSpecialBinCountByArm;
        TEdit       *edSpecialBinBySocket;
        TEdit       *edSpecialBinCountBySocket;

        TEdit       *edLowYield;                                                //Steven 20140828 : By Bin Yield Monitor
        TEdit       *edArmYield;                                                //Steven 20140828 : By Bin Arm Yield Monitor
        TEdit       *edSiteYield;                                               //Steven 20140828 : By Bin Site Yield Monitor
        TEdit       *edBySiteClean;                                             //Steven 20160308 : By Bin count do auto clean
        TEdit       *edByBinClean;                                              //Steven 20160308 : By Bin pre site count do auto clean

        TEdit       *edSpecBinBySiteCompareEnable;                              //JerryYang 20170712 (Steven) by site by bin compare percent
        TEdit       *edSpecBinBySiteCompareIgnore;
        TEdit       *edSpecBinBySiteComparePercent;
        TEdit       *edSpecBinByArmPerSiteCompareEnable;
        TEdit       *edSpecBinByArmPerSiteCompareIgnore;
        TEdit       *edSpecBinByArmPerSiteComparePercent;

        TEdit       *edT6BinART;                                                //ChungHung 20140317 add Auto Retest
        TEdit       *edT6CateR;                                                 //Steven 20161221 (wei) : for SCK only Auto 2 has ART

        int     BackT6PosTray[iBinCount][iBinCount];
        int     iErrorT6;
        int     iT6IsFail   [eTrayCount];                                       //Steven 20160310 : 改成有顏色的fail bin bool --> int
        bool    bT6Link     [eTrayCount];
        bool    bScan       [iBinCount];
        int     i2Contact   [iBinCount];                                        //Steven 20231205 : b2Contact --> i2Contact
        bool    bConFail    [iBinCount];

        bool    bPersentEnable[iBinCount];                                      //Steven 20140529 Start: Fail Persent & Count同時存在
        int     iPersentIgnore[iBinCount];
        double  dPersentNumber[iBinCount];
        bool    bCountEnable[iBinCount];
        int     iCountIgnore[iBinCount];
        int     iCountNumber[iBinCount];

        bool    bT6ART      [eTrayCount];                                       //ChungHung 20140317 add Auto Retest
        bool    bT6CateR    [eTrayCount];                                       //Steven 20161221 (wei) : for SCK only Auto 2 has ART

        bool bSpecialBinByArm[iBinCount];                                       //ChungHung 20140730 add ContinuousFailHaveOneCycle
        unsigned int iSpecialBinCountByArm[iBinCount];
        bool bSpecialBinBySocket[iBinCount];
        unsigned int iSpecialBinCountBySocket[iBinCount];

        bool bLowYield[iBinCount];                                              //Steven 20140828 : By Bin Yield Monitor
        bool bArmYield[iBinCount];                                              //Steven 20140828 : By Bin Arm Yield Monitor
        bool bSiteYield[iBinCount];                                             //Steven 20140828 : By Bin Site Yield Monitor

        int iAutoCleanByBin[iBinCount];                                         //Steven 20160308 : By Bin count do auto clean
        int iAutoCleanBySite[iBinCount];                                        //Steven 20160308 : By Bin pre site count do auto clean

        bool bSpecBinBySiteCompareEnable[iBinCount];                            //JerryYang 20170712 (Steven) by site by bin compare percent
        int  iSpecBinBySiteCompareIgnore[iBinCount];
        double dSpecBinBySiteComparePercent[iBinCount];
        bool bSpecBinByArmPerSiteCompareEnable[iBinCount];
        int  iSpecBinByArmPerSiteCompareIgnore[iBinCount];
        double dSpecBinByArmPerSiteComparePercent[iBinCount];
        void __fastcall sbBinSetScrollScroll(TObject *Sender, TScrollCode ScrollCode, int &ScrollPos);
        void __fastcall sbBinSetScrollChange(TObject *Sender);

        void    __fastcall InitmtBinSelectData();
        bool bCancelErrorBin;                                                   //kevin 20160706 取消 ERROR BIN設定
};
vector<TMyBinPanel*> MyBinPanel;
//---------------------------------------------------------------------------
void __fastcall TMyBinPanel::sbBinSetScrollScroll(TObject *Sender,
      TScrollCode ScrollCode, int &ScrollPos)
{
//    sbBinSetting->HorzScrollBar->Position=ScrollPos;
}
//---------------------------------------------------------------------------
void __fastcall TMyBinPanel::sbBinSetScrollChange(TObject *Sender)
{
    sbBinSetting->HorzScrollBar->Position=sbBinSetScroll->Position;
}
//---------------------------------------------------------------------------
__fastcall TMyBinPanel::TMyBinPanel(TComponent* Owner, int index, TTabSheet *parent, AnsiString Name) : TComponent(Owner)         //設定元件位置
{
    int iT6;
    AnsiString str;
    Panel                   =new TPanel(this);
    mtTrayItem              =new TTMyTray256(this);
    mtTrayName              =new TTMyTray256(this);
    mtBinSelect             =new TTMyTray256(this);
    mtBinSelectBy           =new TTMyTray256(this);                             //Sam 20200622 : Control Bin 顯示
    sbTraySetting           =new TScrollBox(this);                              //Steven 20240420 : 調整scrollbar位置
    sbBinSetting            =new TScrollBox(this);
    sbBinSetScroll          =new TScrollBar(this);                              //Steven 20240420 : 調整scrollbar位置
    edBinSetT3Pos           =new TEdit(this);
    edBinSetT6Error         =new TEdit(this);
    edBinSetT6PassFail      =new TEdit(this);
    edBinSetT6Link          =new TEdit(this);
    edBinSettingEnableScan  =new TEdit(this);
    edBinSetting2Contact    =new TEdit(this);
    edBinSettingConFail     =new TEdit(this);

    edBinSettingFailPersentEnable =new TEdit(this);                             //Steven 20140529 Start: Fail Persent & Count同時存在
    edBinSettingFailPersentIgnore =new TEdit(this);
    edBinSettingFailPersentNumber =new TEdit(this);
    edBinSettingFailCountEnable   =new TEdit(this);
    edBinSettingFailCountIgnore   =new TEdit(this);
    edBinSettingFailCountNumber   =new TEdit(this);

    edSpecialBinByArm        =new TEdit(this);                                  //ChungHung 20140730 add ContinuousFailHaveOneCycle start
    edSpecialBinCountByArm   =new TEdit(this);
    edSpecialBinBySocket     =new TEdit(this);
    edSpecialBinCountBySocket=new TEdit(this);

    edLowYield              =new TEdit(this);                                   //Steven 20140828 : By Bin Yield Monitor
    edArmYield              =new TEdit(this);                                   //Steven 20140828 : By Bin Arm Yield Monitor
    edSiteYield             =new TEdit(this);                                   //Steven 20140828 : By Bin Site Yield Monitor
    edBySiteClean           =new TEdit(this);                                   //Steven 20160308 : By Bin count do auto clean
    edByBinClean            =new TEdit(this);                                   //Steven 20160308 : By Bin pre site count do auto clean
    edT6BinART              =new TEdit(this);                                   //ChungHung 20140317 add Auto Retest
    edT6CateR               =new TEdit(this);                                   //Steven 20161221 (wei) : for SCK only Auto 2 has ART

    edSpecBinBySiteCompareEnable =new TEdit(this);                              //JerryYang 20170712 (Steven) by site by bin compare percent
    edSpecBinBySiteCompareIgnore =new TEdit(this);
    edSpecBinBySiteComparePercent=new TEdit(this);
    edSpecBinByArmPerSiteCompareEnable =new TEdit(this);
    edSpecBinByArmPerSiteCompareIgnore =new TEdit(this);
    edSpecBinByArmPerSiteComparePercent=new TEdit(this);

    str.sprintf("Panel_%s", Name.c_str());
    Panel->Parent      =parent;
    Panel->Name        =str;
    Panel->Color       =TColor(0x00C2B8A6);
    Panel->Align       =alClient;
    Panel->Caption     ="";

    str.sprintf("sbTraySetting%s", Name.c_str());
    sbTraySetting->Parent      =Panel;                                          //Steven 20240420 : 調整scrollbar位置
    sbTraySetting->Name        =str;

    sbTraySetting->Height      =iRowHeight*eBinSetTotal+20;                     //20是ScrollBar的高度

    if(sbTraySetting->Height>720)
    {
        sbTraySetting->Height=720;
    }

    sbTraySetting->Left         =0;
    sbTraySetting->Top          =8;
    sbTraySetting->Width        =1050;
    sbTraySetting->BorderStyle  =bsNone;
    sbTraySetting->Tag          =index;
    sbTraySetting->HorzScrollBar->Visible=false;

    str.sprintf("sbBinSetting%s", Name.c_str());
    sbBinSetting->Parent        =sbTraySetting;
    sbBinSetting->Name          =str;

    sbBinSetting->Height        =iRowHeight*eBinSetTotal+20;                    //20是ScrollBar的高度
    sbBinSetting->Left          =350;
    sbBinSetting->Top           =0;
    sbBinSetting->Width         =680;
    sbBinSetting->BorderStyle   =bsNone;
    sbBinSetting->Tag           =index;
    sbBinSetting->VertScrollBar->Visible=false;
//    sbBinSetting->HorzScrollBar->Visible=false;                               //false的話,就不能拖了

    str.sprintf("sbBinSetScroll_%s", Name.c_str());                             //Steven 20240420 : 調整scrollbar位置
    sbBinSetScroll->Parent      =Panel;
    sbBinSetScroll->Name        =str;
    sbBinSetScroll->Height      =25;
    sbBinSetScroll->Kind        =sbHorizontal;
    sbBinSetScroll->Top         =710;
    sbBinSetScroll->Left        =350;
    sbBinSetScroll->Width       =680;
    sbBinSetScroll->PageSize    =600;
    sbBinSetScroll->OnScroll    =sbBinSetScrollScroll;
    sbBinSetScroll->OnChange    =sbBinSetScrollChange;

    str.sprintf("mtBinSelect_%s", Name.c_str());
    mtBinSelect->Parent      =sbBinSetting;
    mtBinSelect->Name        =str;
    mtBinSelect->Height      =iRowHeight*eBinSetTotal;
    mtBinSelect->Left        =0;
    mtBinSelect->Top         =0;
    mtBinSelect->Width       =iColWidth*iBinCount+10;                           //10是為了留邊
    mtBinSelect->SetColorMap(eCLWhite,   clWhite);
    mtBinSelect->SetColorMap(eCLGreen,   clGreen);
    mtBinSelect->SetColorMap(eCLRed,     clRed);
    mtBinSelect->SetColorMap(eCLYellow,  TColor(0x000080FF));
    mtBinSelect->SetColorMap(eCLPurple,  clPurple);
    mtBinSelect->SetColorMap(eCLBlue,    clBlue);
    mtBinSelect->SetColorMap(eCLGray,    clGray);
    mtBinSelect->SetColorMap(eCLSilver,  clSilver);
    mtBinSelect->SetColorMap(eCLBtnFace, clBtnFace);
    mtBinSelect->SetColorMap(eCLOlive,   clOlive);
    mtBinSelect->Color       =clWhite;
    mtBinSelect->Font->Name  ="Arial";
    mtBinSelect->Font->Size  =10;
    mtBinSelect->Font->Color =clWhite;
    mtBinSelect->DirectWidth =0;
    mtBinSelect->EdgeWidth   =2;
    mtBinSelect->LineWidth   =-1;
    mtBinSelect->Shape       =0;
    mtBinSelect->ShowFont    =true;
    mtBinSelect->Tag         =index;
    mtBinSelect->OnMouseDown =fBinSel->mtBinSelectMouseDown;
    mtBinSelect->OnMouseUp   =fBinSel->mtBinSelectMouseUp;
    mtBinSelect->OnMouseMove =fBinSel->mtBinSelectMouseMove;
    mtBinSelect->YItem       =eBinSetTotal;
    mtBinSelect->XItem       =iBinCount;

    str.sprintf("mtTrayName_%s", Name.c_str());
    mtTrayName->Parent      =Panel;
    mtTrayName->Name        =str;
    mtTrayName->Height      =iRowHeight*eBinSetTotal;
    mtTrayName->Left        =131;                                               //JerryYang 20170712 (Steven) 118->131
    mtTrayName->Top         =0;
    mtTrayName->Width       =220;                                               //Steven 20161221 : 171 --> 220
    mtTrayName->SetColorMap(eCLWhite,   clWhite);
    mtTrayName->SetColorMap(eCLGreen,   clGreen);
    mtTrayName->SetColorMap(eCLRed,     clRed);
    mtTrayName->SetColorMap(eCLYellow,  TColor(0x000080FF));
    mtTrayName->SetColorMap(eCLPurple,  clPurple);
    mtTrayName->SetColorMap(eCLBlue,    clBlue);
    mtTrayName->SetColorMap(eCLGray,    clGray);
    mtTrayName->SetColorMap(eCLSilver,  clSilver);
    mtTrayName->SetColorMap(eCLBtnFace, clBtnFace);
    mtTrayName->SetColorMap(eCLOlive,   clOlive);
    mtTrayName->Color       =clWhite;
    mtTrayName->Font->Name  ="Arial";
    mtTrayName->Font->Size  =10;
    mtTrayName->Font->Color =clWhite;
    mtTrayName->DirectWidth =0;
    mtTrayName->EdgeWidth   =5;
    mtTrayName->LineWidth   =-1;
    mtTrayName->Shape       =0;
    mtTrayName->ShowFont    =true;
    mtTrayName->Tag         =index;
    mtTrayName->OnMouseDown =fBinSel->mtTrayNameMouseDown;
    mtTrayName->Parent      =sbTraySetting;
    mtTrayName->YItem       =eBinSetTotal;
    mtTrayName->XItem       =eItemTotal;

    str.sprintf("mtTrayItem_%s", Name.c_str());
    mtTrayItem->Parent      =Panel;
    mtTrayItem->Name        =str;
    mtTrayItem->Height      =iRowHeight*eBinSetTotal;
    mtTrayItem->Left        =8;
    mtTrayItem->Top         =0;
    mtTrayItem->Width       =130;                                               //JerryYang 20170712 (Steven) 117->130
    mtTrayItem->SetColorMap(eCLWhite,   clWhite);
    mtTrayItem->SetColorMap(eCLGreen,   clGreen);
    mtTrayItem->SetColorMap(eCLRed,     clRed);
    mtTrayItem->SetColorMap(eCLYellow,  TColor(0x000080FF));
    mtTrayItem->SetColorMap(eCLPurple,  clPurple);
    mtTrayItem->SetColorMap(eCLBlue,    clBlue);
    mtTrayItem->SetColorMap(eCLGray,    clGray);
    mtTrayItem->SetColorMap(eCLSilver,  clSilver);
    mtTrayItem->SetColorMap(eCLBtnFace, clBtnFace);
    mtTrayItem->SetColorMap(eCLOlive,   clOlive);
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
    mtTrayItem->Parent      =sbTraySetting;
    mtTrayItem->YItem       =eBinSetTotal;
    mtTrayItem->XItem       =eItemName;
    mtTrayItem->OnMouseUp   =fBinSel->mtTrayItemMouseUp;

    //Col Name----
    mtTrayName->SetCellNumber(eItemLink,  eBinNo,       "Link"    );
    mtTrayName->SetCellNumber(eItemPass,  eBinNo,       "Failed"  );
    mtTrayName->SetCellNumber(eItemError, eBinNo,       "Error"   );
    mtTrayName->SetCellNumber(eItemART,   eBinNo,       "Retest"  );            //ChungHung 20140317 add Auto Retest
    mtTrayName->SetCellNumber(eItemCateR, eBinNo,       "CateR"   );            //Steven 20161221 (wei) : Cate R for ART

    //Row Name----
    mtTrayItem->SetCellNumber(eItemName,  eBinNo,           "Items"         );
    mtTrayItem->SetCellNumber(eItemName,  eScanning,        "Scanning"      );
    mtTrayItem->SetCellNumber(eItemName,  eDoubleContact,   "Double Contact");
    mtTrayItem->SetCellNumber(eItemName,  eConsFail,        "Cons. Fail"    );

    mtTrayItem->SetCellNumber(eItemName,  ePersentEnable,   "Yield % Bin");     //Steven 20140925 : SPIL鄭世明說Pass也要設定 Fail --> Yield
    mtTrayItem->SetCellNumber(eItemName,  ePersentIgnore,   "Yield Ignore Cnt");//JerryYang 20160802 修正文字避免誤解,Yield % Ignore改成Yield Ignore Cnt
    mtTrayItem->SetCellNumber(eItemName,  ePersentNumber,   "Yield % Number");
    mtTrayItem->SetCellNumber(eItemName,  eCountEnable,     "Count Bin" );      //Steven 20140925 : SPIL鄭世明說Pass也要設定 Fail --> Count
    mtTrayItem->SetCellNumber(eItemName,  eCountIgnore,     "Count Ignored");
    mtTrayItem->SetCellNumber(eItemName,  eCountNumber,     "Count Number");

    mtTrayItem->SetCellNumber(eItemName,  eSpecialBinByArm,           "Spc. Bin By Arm");
    mtTrayItem->SetCellNumber(eItemName,  eSpecialBinCountByArm,      "Spc. Cnt By Arm") ;
    mtTrayItem->SetCellNumber(eItemName,  eSpecialBinBySocket,        "Spc. Bin By Socket");
    mtTrayItem->SetCellNumber(eItemName,  eSpecialBinCountBySocket,   "Spc. Cnt By Socket");

    mtTrayItem->SetCellNumber(eItemName,  eLowYield,    "Low Yield") ;          //Steven 20140828 : By Bin Yield Monitor
    mtTrayItem->SetCellNumber(eItemName,  eArmYield,    "By Arm Yield");        //Steven 20140828 : By Bin Arm Yield Monitor

    if(CUSTOMER_CODE==CC_Greatek)                                               //Sam 20201221 : 修正 Control Bin 顯示
    {
        mtTrayItem->SetCellNumber(eItemName,    eSiteYield,         "Control Bin Yield%");
        mtTrayItem->SetCellNumber(eItemName,    eAutoCleanByBin,    "Control Bin Count");
        mtTrayItem->SetCellNumber(eItemName,    eAutoCleanBySite,   "Control Bin S/S%");
    }
    else
    {
        mtTrayItem->SetCellNumber(eItemName,    eSiteYield,         "By Site Yield");   //Steven 20140828 : By Bin Site Yield Monitor
        mtTrayItem->SetCellNumber(eItemName,    eAutoCleanByBin,    "By Bin Cleaning"); //Steven 20160308 : By Bin count do auto clean
        mtTrayItem->SetCellNumber(eItemName,    eAutoCleanBySite,   "By Site Cleaning");//Steven 20160308 : By Bin pre site count do auto clean
    }

    mtTrayItem->SetCellNumber(eItemName,  eSpecBinBySiteCompareEnable,       "By Bin Site Gap");
    mtTrayItem->SetCellNumber(eItemName,  eSpecBinBySiteCompareIgnore,       "Count Ignore") ;
    mtTrayItem->SetCellNumber(eItemName,  eSpecBinBySiteComparePercent,      "Site Gap %");
    mtTrayItem->SetCellNumber(eItemName,  eSpecBinByArmPerSiteCompareEnable, "By Arm By Bin Site Gap");
    mtTrayItem->SetCellNumber(eItemName,  eSpecBinByArmPerSiteCompareIgnore, "Count Ignore") ;
    mtTrayItem->SetCellNumber(eItemName,  eSpecBinByArmPerSiteComparePercent,"By Arm Site Gap%");

    mtTrayItem->SetCellNumber(eItemName,  eBinNotUse,    "Not in use");

    for(int j=eBinNo; j<eBinSetting; j++)
    {
        for(int k=eItemLink; k<eItemTotal; k++)
        {
            mtTrayName->SetCellColorIndex(k,  j, eCLGray);
        }
        mtTrayItem->SetCellColorIndex(eItemName, j, eCLGray);
    }

    for(int j=eBinSetting; j<eBinSetTotal; j++)
    {
        iT6=j-eBinSetting;
        mtTrayItem->SetCellNumber    (eItemName,  j, s6TrayName[iT6].c_str());
        mtTrayItem->SetCellColorIndex(eItemName,  j, eCLGray);
        mtTrayName->SetCellNumber    (eItemError, j, "Error");
        mtTrayName->SetCellColorIndex(eItemError, j, eCLWhite);

        if(USE_AUTO_RETEST==eartInstall || IniConfig.bA38_SLT_Summary)          //JerryYang 20220923 : add for SLT lot summary
        {
            mtTrayName->SetCellNumber    (eItemART, j, "Retest");               //ChungHung 20140317 add Auto Retest
            mtTrayName->SetCellColorIndex(eItemART, j, eCLWhite);               //ChungHung 20140317 add Auto Retest

            if(CosFunction.bUseSCKART)                                          //Steven 20161221 (wei) : Cate R for ART
            {
                mtTrayName->SetCellNumber    (eItemCateR, j, "CateR");
                mtTrayName->SetCellColorIndex(eItemCateR, j, eCLWhite);
            }
            else
            {
                mtTrayName->SetCellNumber    (eItemCateR, j, "");
                mtTrayName->SetCellColorIndex(eItemCateR, j, eCLGray);          //Steven 20161221 (wei) : Cate R for ART
            }
        }
        else
        {
            mtTrayName->SetCellNumber    (eItemART, j, "");                     //ChungHung 20140317 add Auto Retest
            mtTrayName->SetCellColorIndex(eItemART, j, eCLGray);                //ChungHung 20140317 add Auto Retest
            mtTrayName->SetCellNumber    (eItemCateR, j, "");
            mtTrayName->SetCellColorIndex(eItemCateR, j, eCLGray);              //Steven 20161221 (wei) : Cate R for ART
        }
    }

    InitmtBinSelectData();

    str.sprintf("edBinSetT3Pos_%s", Name.c_str());
    edBinSetT3Pos->Parent      =Panel;
    edBinSetT3Pos->Name        =str;
    edBinSetT3Pos->Left        =8;
    edBinSetT3Pos->Top         =400;
    edBinSetT3Pos->Width       =500;
    edBinSetT3Pos->Font->Color =clBlack;
    edBinSetT3Pos->Font->Name  ="Arial";
    edBinSetT3Pos->Font->Size  =10;
    edBinSetT3Pos->Tag         =index;
    edBinSetT3Pos->Text        ="";
    edBinSetT3Pos->Visible     =bEditVisible;

    str.sprintf("edBinSetT6Error_%s", Name.c_str());
    edBinSetT6Error->Parent      =Panel;
    edBinSetT6Error->Name        =str;
    edBinSetT6Error->Left        =8;
    edBinSetT6Error->Top         =420;
    edBinSetT6Error->Width       =500;
    edBinSetT6Error->Font->Color =clBlack;
    edBinSetT6Error->Font->Name  ="Arial";
    edBinSetT6Error->Font->Size  =10;
    edBinSetT6Error->Tag         =index;
    edBinSetT6Error->Text        ="";
    edBinSetT6Error->Visible     =bEditVisible;

    str.sprintf("edBinSetT6PassFail_%s", Name.c_str());
    edBinSetT6PassFail->Parent      =Panel;
    edBinSetT6PassFail->Name        =str;
    edBinSetT6PassFail->Left        =8;
    edBinSetT6PassFail->Top         =440;
    edBinSetT6PassFail->Width       =500;
    edBinSetT6PassFail->Font->Color =clBlack;
    edBinSetT6PassFail->Font->Name  ="Arial";
    edBinSetT6PassFail->Font->Size  =10;
    edBinSetT6PassFail->Tag         =index;
    edBinSetT6PassFail->Text        ="";
    edBinSetT6PassFail->Visible     =bEditVisible;

    str.sprintf("edBinSetT6Link_%s", Name.c_str());
    edBinSetT6Link->Parent      =Panel;
    edBinSetT6Link->Name        =str;
    edBinSetT6Link->Left        =8;
    edBinSetT6Link->Top         =460;
    edBinSetT6Link->Width       =500;
    edBinSetT6Link->Font->Color =clBlack;
    edBinSetT6Link->Font->Name  ="Arial";
    edBinSetT6Link->Font->Size  =10;
    edBinSetT6Link->Tag         =index;
    edBinSetT6Link->Text        ="";
    edBinSetT6Link->Visible     =bEditVisible;

    str.sprintf("edBinSettingEnableScan_%s", Name.c_str());
    edBinSettingEnableScan->Parent      =Panel;
    edBinSettingEnableScan->Name        =str;
    edBinSettingEnableScan->Left        =8;
    edBinSettingEnableScan->Top         =480;
    edBinSettingEnableScan->Width       =500;
    edBinSettingEnableScan->Font->Color =clBlack;
    edBinSettingEnableScan->Font->Name  ="Arial";
    edBinSettingEnableScan->Font->Size  =10;
    edBinSettingEnableScan->Tag         =index;
    edBinSettingEnableScan->Text        ="";
    edBinSettingEnableScan->Visible     =bEditVisible;

    str.sprintf("edBinSetting2Contact_%s", Name.c_str());
    edBinSetting2Contact->Parent      =Panel;
    edBinSetting2Contact->Name        =str;
    edBinSetting2Contact->Left        =8;
    edBinSetting2Contact->Top         =500;
    edBinSetting2Contact->Width       =500;
    edBinSetting2Contact->Font->Color =clBlack;
    edBinSetting2Contact->Font->Name  ="Arial";
    edBinSetting2Contact->Font->Size  =10;
    edBinSetting2Contact->Tag         =index;
    edBinSetting2Contact->Text        ="";
    edBinSetting2Contact->Visible     =bEditVisible;

    str.sprintf("edBinSettingConFail_%s", Name.c_str());
    edBinSettingConFail->Parent      =Panel;
    edBinSettingConFail->Name        =str;
    edBinSettingConFail->Left        =8;
    edBinSettingConFail->Top         =520;
    edBinSettingConFail->Width       =500;
    edBinSettingConFail->Font->Color =clBlack;
    edBinSettingConFail->Font->Name  ="Arial";
    edBinSettingConFail->Font->Size  =10;
    edBinSettingConFail->Tag         =index;
    edBinSettingConFail->Text        ="";
    edBinSettingConFail->Visible     =bEditVisible;

    str.sprintf("edBinSettingFailPersentEnable_%s", Name.c_str());              //Steven 20140529 Start: Fail Persent & Count同時存在
    edBinSettingFailPersentEnable->Parent      =Panel;
    edBinSettingFailPersentEnable->Name        =str;
    edBinSettingFailPersentEnable->Left        =8;
    edBinSettingFailPersentEnable->Top         =540;
    edBinSettingFailPersentEnable->Width       =500;
    edBinSettingFailPersentEnable->Font->Color =clBlack;
    edBinSettingFailPersentEnable->Font->Name  ="Arial";
    edBinSettingFailPersentEnable->Font->Size  =10;
    edBinSettingFailPersentEnable->Tag         =index;
    edBinSettingFailPersentEnable->Text        ="";
    edBinSettingFailPersentEnable->Visible     =bEditVisible;

    str.sprintf("edBinSettingFailPersentIgnore_%s", Name.c_str());
    edBinSettingFailPersentIgnore->Parent      =Panel;
    edBinSettingFailPersentIgnore->Name        =str;
    edBinSettingFailPersentIgnore->Left        =8;
    edBinSettingFailPersentIgnore->Top         =560;
    edBinSettingFailPersentIgnore->Width       =500;
    edBinSettingFailPersentIgnore->Font->Color =clBlack;
    edBinSettingFailPersentIgnore->Font->Name  ="Arial";
    edBinSettingFailPersentIgnore->Font->Size  =10;
    edBinSettingFailPersentIgnore->Tag         =index;
    edBinSettingFailPersentIgnore->Text        ="";
    edBinSettingFailPersentIgnore->Visible     =bEditVisible;

    str.sprintf("edBinSettingFailPersentNumber_%s", Name.c_str());
    edBinSettingFailPersentNumber->Parent      =Panel;
    edBinSettingFailPersentNumber->Name        =str;
    edBinSettingFailPersentNumber->Left        =8;
    edBinSettingFailPersentNumber->Top         =580;
    edBinSettingFailPersentNumber->Width       =500;
    edBinSettingFailPersentNumber->Font->Color =clBlack;
    edBinSettingFailPersentNumber->Font->Name  ="Arial";
    edBinSettingFailPersentNumber->Font->Size  =10;
    edBinSettingFailPersentNumber->Tag         =index;
    edBinSettingFailPersentNumber->Text        ="";
    edBinSettingFailPersentNumber->Visible     =bEditVisible;

    str.sprintf("edBinSettingFailCountEnable_%s", Name.c_str());
    edBinSettingFailCountEnable->Parent      =Panel;
    edBinSettingFailCountEnable->Name        =str;
    edBinSettingFailCountEnable->Left        =200;
    edBinSettingFailCountEnable->Top         =540;
    edBinSettingFailCountEnable->Width       =500;
    edBinSettingFailCountEnable->Font->Color =clBlack;
    edBinSettingFailCountEnable->Font->Name  ="Arial";
    edBinSettingFailCountEnable->Font->Size  =10;
    edBinSettingFailCountEnable->Tag         =index;
    edBinSettingFailCountEnable->Text        ="";
    edBinSettingFailCountEnable->Visible     =bEditVisible;

    str.sprintf("edBinSettingFailCountIgnore_%s", Name.c_str());
    edBinSettingFailCountIgnore->Parent      =Panel;
    edBinSettingFailCountIgnore->Name        =str;
    edBinSettingFailCountIgnore->Left        =200;
    edBinSettingFailCountIgnore->Top         =560;
    edBinSettingFailCountIgnore->Width       =500;
    edBinSettingFailCountIgnore->Font->Color =clBlack;
    edBinSettingFailCountIgnore->Font->Name  ="Arial";
    edBinSettingFailCountIgnore->Font->Size  =10;
    edBinSettingFailCountIgnore->Tag         =index;
    edBinSettingFailCountIgnore->Text        ="";
    edBinSettingFailCountIgnore->Visible     =bEditVisible;

    str.sprintf("edBinSettingFailCountNumber_%s", Name.c_str());
    edBinSettingFailCountNumber->Parent      =Panel;
    edBinSettingFailCountNumber->Name        =str;
    edBinSettingFailCountNumber->Left        =200;
    edBinSettingFailCountNumber->Top         =580;
    edBinSettingFailCountNumber->Width       =500;
    edBinSettingFailCountNumber->Font->Color =clBlack;
    edBinSettingFailCountNumber->Font->Name  ="Arial";
    edBinSettingFailCountNumber->Font->Size  =10;
    edBinSettingFailCountNumber->Tag         =index;
    edBinSettingFailCountNumber->Text        ="";
    edBinSettingFailCountNumber->Visible     =bEditVisible;

    str.sprintf("edSpecialBinByArm%s", Name.c_str());                           //ChungHung 20140730 add ContinuousFailHaveOneCycle start
    edSpecialBinByArm->Parent      =Panel;
    edSpecialBinByArm->Name        =str;
    edSpecialBinByArm->Left        =8;
    edSpecialBinByArm->Top         =580;
    edSpecialBinByArm->Width       =500;
    edSpecialBinByArm->Font->Color =clBlack;
    edSpecialBinByArm->Font->Name  ="Arial";
    edSpecialBinByArm->Font->Size  =10;
    edSpecialBinByArm->Tag         =index;
    edSpecialBinByArm->Text        ="";
    edSpecialBinByArm->Visible     =bEditVisible;

    str.sprintf("edSpecialBinCountByArm%s", Name.c_str());
    edSpecialBinCountByArm->Parent      =Panel;
    edSpecialBinCountByArm->Name        =str;
    edSpecialBinCountByArm->Left        =8;
    edSpecialBinCountByArm->Top         =580;
    edSpecialBinCountByArm->Width       =500;
    edSpecialBinCountByArm->Font->Color =clBlack;
    edSpecialBinCountByArm->Font->Name  ="Arial";
    edSpecialBinCountByArm->Font->Size  =10;
    edSpecialBinCountByArm->Tag         =index;
    edSpecialBinCountByArm->Text        ="";
    edSpecialBinCountByArm->Visible     =bEditVisible;

    str.sprintf("edSpecialBinBySocket%s", Name.c_str());
    edSpecialBinBySocket->Parent      =Panel;
    edSpecialBinBySocket->Name        =str;
    edSpecialBinBySocket->Left        =8;
    edSpecialBinBySocket->Top         =580;
    edSpecialBinBySocket->Width       =500;
    edSpecialBinBySocket->Font->Color =clBlack;
    edSpecialBinBySocket->Font->Name  ="Arial";
    edSpecialBinBySocket->Font->Size  =10;
    edSpecialBinBySocket->Tag         =index;
    edSpecialBinBySocket->Text        ="";
    edSpecialBinBySocket->Visible     =bEditVisible;

    str.sprintf("edSpecialBinCountBySocket%s", Name.c_str());
    edSpecialBinCountBySocket->Parent      =Panel;
    edSpecialBinCountBySocket->Name        =str;
    edSpecialBinCountBySocket->Left        =8;
    edSpecialBinCountBySocket->Top         =580;
    edSpecialBinCountBySocket->Width       =500;
    edSpecialBinCountBySocket->Font->Color =clBlack;
    edSpecialBinCountBySocket->Font->Name  ="Arial";
    edSpecialBinCountBySocket->Font->Size  =10;
    edSpecialBinCountBySocket->Tag         =index;
    edSpecialBinCountBySocket->Text        ="";
    edSpecialBinCountBySocket->Visible     =bEditVisible;

    str.sprintf("edLowYield%s", Name.c_str());                                  //Steven 20140828 : By Bin Yield Monitor
    edLowYield->Parent      =Panel;
    edLowYield->Name        =str;
    edLowYield->Left        =8;
    edLowYield->Top         =480;
    edLowYield->Width       =500;
    edLowYield->Font->Color =clBlack;
    edLowYield->Font->Name  ="Arial";
    edLowYield->Font->Size  =10;
    edLowYield->Tag         =index;
    edLowYield->Text        ="";
    edLowYield->Visible     =bEditVisible;

    str.sprintf("edArmYield%s", Name.c_str());                                  //Steven 20140828 : By Bin Arm Yield Monitor
    edArmYield->Parent      =Panel;
    edArmYield->Name        =str;
    edArmYield->Left        =8;
    edArmYield->Top         =530;
    edArmYield->Width       =500;
    edArmYield->Font->Color =clBlack;
    edArmYield->Font->Name  ="Arial";
    edArmYield->Font->Size  =10;
    edArmYield->Tag         =index;
    edArmYield->Text        ="";
    edArmYield->Visible     =bEditVisible;

    str.sprintf("edSiteYield%s", Name.c_str());                                 //Steven 20140828 : By Bin Site Yield Monitor
    edSiteYield->Parent      =Panel;
    edSiteYield->Name        =str;
    edSiteYield->Left        =8;
    edSiteYield->Top         =580;
    edSiteYield->Width       =500;
    edSiteYield->Font->Color =clBlack;
    edSiteYield->Font->Name  ="Arial";
    edSiteYield->Font->Size  =10;
    edSiteYield->Tag         =index;
    edSiteYield->Text        ="";
    edSiteYield->Visible     =bEditVisible;

    str.sprintf("edBySiteClean%s", Name.c_str());                               //Steven 20160308 : By Bin count do auto clean
    edBySiteClean->Parent      =Panel;
    edBySiteClean->Name        =str;
    edBySiteClean->Left        =8;
    edBySiteClean->Top         =480;
    edBySiteClean->Width       =500;
    edBySiteClean->Font->Color =clBlack;
    edBySiteClean->Font->Name  ="Arial";
    edBySiteClean->Font->Size  =10;
    edBySiteClean->Tag         =index;
    edBySiteClean->Text        ="";
    edBySiteClean->Visible     =bEditVisible;

    str.sprintf("edByBinClean%s", Name.c_str());                                //Steven 20160308 : By Bin pre site count do auto clean
    edByBinClean->Parent      =Panel;
    edByBinClean->Name        =str;
    edByBinClean->Left        =8;
    edByBinClean->Top         =480;
    edByBinClean->Width       =500;
    edByBinClean->Font->Color =clBlack;
    edByBinClean->Font->Name  ="Arial";
    edByBinClean->Font->Size  =10;
    edByBinClean->Tag         =index;
    edByBinClean->Text        ="";
    edByBinClean->Visible     =bEditVisible;

    str.sprintf("edT6BinART%s", Name.c_str());                                  //ChungHung 20140317 add Auto Retest
    edT6BinART->Parent      =Panel;
    edT6BinART->Name        =str;
    edT6BinART->Left        =8;
    edT6BinART->Top         =580;
    edT6BinART->Width       =500;
    edT6BinART->Font->Color =clBlack;
    edT6BinART->Font->Name  ="Arial";
    edT6BinART->Font->Size  =10;
    edT6BinART->Tag         =index;
    edT6BinART->Text        ="";
    edT6BinART->Visible     =bEditVisible;

    edT6CateR                 =new TEdit(this);                                 //Steven 20161221 (wei) : for SCK only Auto 2 has ART
    str.sprintf("edT6CateR%s", Name.c_str());
    edT6CateR->Parent         =Panel;
    edT6CateR->Name           =str;
    edT6CateR->Left           =8;
    edT6CateR->Top            =580;
    edT6CateR->Width          =500;
    edT6CateR->Font->Color    =clBlack;
    edT6CateR->Font->Name     ="Arial";
    edT6CateR->Font->Size     =10;
    edT6CateR->Tag            =index;
    edT6CateR->Text           ="";
    edT6CateR->Visible        =bEditVisible;

    str.sprintf("edSpecBinBySiteCompareEnable_%s", Name.c_str());               //JerryYang 20170712 (Steven) by site by bin compare percent
    edSpecBinBySiteCompareEnable->Parent      =Panel;
    edSpecBinBySiteCompareEnable->Name        =str;
    edSpecBinBySiteCompareEnable->Left        =8;
    edSpecBinBySiteCompareEnable->Top         =600;
    edSpecBinBySiteCompareEnable->Width       =500;
    edSpecBinBySiteCompareEnable->Font->Color =clBlack;
    edSpecBinBySiteCompareEnable->Font->Name  ="Arial";
    edSpecBinBySiteCompareEnable->Font->Size  =10;
    edSpecBinBySiteCompareEnable->Tag         =index;
    edSpecBinBySiteCompareEnable->Text        ="";
    edSpecBinBySiteCompareEnable->Visible     =bEditVisible;

    str.sprintf("edSpecBinBySiteCompareIgnore_%s", Name.c_str());
    edSpecBinBySiteCompareIgnore->Parent      =Panel;
    edSpecBinBySiteCompareIgnore->Name        =str;
    edSpecBinBySiteCompareIgnore->Left        =8;
    edSpecBinBySiteCompareIgnore->Top         =620;
    edSpecBinBySiteCompareIgnore->Width       =500;
    edSpecBinBySiteCompareIgnore->Font->Color =clBlack;
    edSpecBinBySiteCompareIgnore->Font->Name  ="Arial";
    edSpecBinBySiteCompareIgnore->Font->Size  =10;
    edSpecBinBySiteCompareIgnore->Tag         =index;
    edSpecBinBySiteCompareIgnore->Text        ="";
    edSpecBinBySiteCompareIgnore->Visible     =bEditVisible;

    str.sprintf("edSpecBinBySiteComparePercent_%s", Name.c_str());
    edSpecBinBySiteComparePercent->Parent      =Panel;
    edSpecBinBySiteComparePercent->Name        =str;
    edSpecBinBySiteComparePercent->Left        =8;
    edSpecBinBySiteComparePercent->Top         =640;
    edSpecBinBySiteComparePercent->Width       =500;
    edSpecBinBySiteComparePercent->Font->Color =clBlack;
    edSpecBinBySiteComparePercent->Font->Name  ="Arial";
    edSpecBinBySiteComparePercent->Font->Size  =10;
    edSpecBinBySiteComparePercent->Tag         =index;
    edSpecBinBySiteComparePercent->Text        ="";
    edSpecBinBySiteComparePercent->Visible     =bEditVisible;

    str.sprintf("edSpecBinByArmPerSiteCompareEnable%s", Name.c_str());
    edSpecBinByArmPerSiteCompareEnable->Parent      =Panel;
    edSpecBinByArmPerSiteCompareEnable->Name        =str;
    edSpecBinByArmPerSiteCompareEnable->Left        =8;
    edSpecBinByArmPerSiteCompareEnable->Top         =600;
    edSpecBinByArmPerSiteCompareEnable->Width       =500;
    edSpecBinByArmPerSiteCompareEnable->Font->Color =clBlack;
    edSpecBinByArmPerSiteCompareEnable->Font->Name  ="Arial";
    edSpecBinByArmPerSiteCompareEnable->Font->Size  =10;
    edSpecBinByArmPerSiteCompareEnable->Tag         =index;
    edSpecBinByArmPerSiteCompareEnable->Text        ="";
    edSpecBinByArmPerSiteCompareEnable->Visible     =bEditVisible;

    str.sprintf("edSpecBinByArmPerSiteCompareIgnore_%s", Name.c_str());
    edSpecBinByArmPerSiteCompareIgnore->Parent      =Panel;
    edSpecBinByArmPerSiteCompareIgnore->Name        =str;
    edSpecBinByArmPerSiteCompareIgnore->Left        =8;
    edSpecBinByArmPerSiteCompareIgnore->Top         =620;
    edSpecBinByArmPerSiteCompareIgnore->Width       =500;
    edSpecBinByArmPerSiteCompareIgnore->Font->Color =clBlack;
    edSpecBinByArmPerSiteCompareIgnore->Font->Name  ="Arial";
    edSpecBinByArmPerSiteCompareIgnore->Font->Size  =10;
    edSpecBinByArmPerSiteCompareIgnore->Tag         =index;
    edSpecBinByArmPerSiteCompareIgnore->Text        ="";
    edSpecBinByArmPerSiteCompareIgnore->Visible     =bEditVisible;

    str.sprintf("edSpecBinByArmPerSiteComparePercent_%s", Name.c_str());
    edSpecBinByArmPerSiteComparePercent->Parent      =Panel;
    edSpecBinByArmPerSiteComparePercent->Name        =str;
    edSpecBinByArmPerSiteComparePercent->Left        =8;
    edSpecBinByArmPerSiteComparePercent->Top         =640;
    edSpecBinByArmPerSiteComparePercent->Width       =500;
    edSpecBinByArmPerSiteComparePercent->Font->Color =clBlack;
    edSpecBinByArmPerSiteComparePercent->Font->Name  ="Arial";
    edSpecBinByArmPerSiteComparePercent->Font->Size  =10;
    edSpecBinByArmPerSiteComparePercent->Tag         =index;
    edSpecBinByArmPerSiteComparePercent->Text        ="";
    edSpecBinByArmPerSiteComparePercent->Visible     =bEditVisible;

    iErrorT6=0;
    for(int i=0; i<eTrayCount; i++)
    {
        iT6IsFail[i]=1;
        bT6Link[i]  =false;
        bT6CateR[i] =false;                                                     //Steven 20161221 (wei) : for SCK only Auto 2 has ART
    }
    for(int i=eAuto1; i<=iAutoRight; i++)
        bT6ART[i]=false;
}
//---------------------------------------------------------------------------
void __fastcall TMyBinPanel::InitmtBinSelectData()
{
    for(int j=0; j<iBinCount; j++)
    {
        if(j<mtBinSelect->XItem)
        {
            mtBinSelect->SetCellNumber    (j, eBinNo, j);                       //kevin 20140317 256 bin 0       j+1);
            mtBinSelect->SetCellColorIndex(j, eBinNo, eCLGray);
            mtBinSelect->SetCellNumber    (j, eScanning, "");
            for(int k=eScanning; k<eBinSetting; k++)
                mtBinSelect->SetCellColorIndex(j, k, eCLBtnFace);

            mtBinSelect->SetCellNumber(j, eBinNotUse, j);

            for(int iT6=0; iT6<eTrayCount; iT6++)
            {
                if((iT6+eBinSetting)>=mtBinSelect->YItem)
                    continue;

                if(Prod.iTrayType[iT6]==tNotUse)
                {
                    mtBinSelect->SetCellNumber(j, iT6+eBinSetting, "");
                }
                else
                {
                    mtBinSelect->SetCellNumber(j, iT6+eBinSetting, j);
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
//ChungHung 20140318 add
__fastcall TMyBinPanel::~TMyBinPanel()
{
//jou 2014-05-05 make code 出現解構異常
//    delete mtTrayItem;
//    delete mtTrayName;
//    delete mtBinSelect;
//    delete sbBinSetting;
//    delete edBinSetT3Pos;
//    delete edBinSetT6Error;
//    delete edBinSetT6PassFail;
//    delete edBinSetT6Link;
//    delete edBinSettingEnableScan;
//    delete edBinSetting2Contact;
//    delete edBinSettingConFail;
//    delete edBinSettingFailPersentEnable;
//    delete edBinSettingFailPersentNumber;
//    delete edT6BinART;                                                        //ChungHung 20140317 add Auto Retest
//    delete Panel;
}
//---------------------------------------------------------------------------
__fastcall TfBinSel::TfBinSel(TComponent* Owner)
    : TForm(Owner)
{
    MyBinPanel.push_back(new TMyBinPanel(this, 0, tsRetest, "RT"));
    MyBinPanel.push_back(new TMyBinPanel(this, 1, tsNormal, "FT"));
    MyBinPanel.push_back(new TMyBinPanel(this, 2, tsOffline, "OffLine"));
    MyBinPanel.push_back(new TMyBinPanel(this, 3, tsArtRT, "RT_ART"));          //ChungHung 20141002 add for KYEC AutoRetest
    MyBinPanel.push_back(new TMyBinPanel(this, 4, tsArtFT, "FT_ART"));          //ChungHung 20141002 add for KYEC AutoRetest
    MyBinPanel.push_back(new TMyBinPanel(this, 5, tsMrtRT, "RT_MRT"));          //Ifor 20170316 (wei) Add MRT Mode
    MyBinPanel.push_back(new TMyBinPanel(this, 6, tsMrtFT, "FT_MRT"));          //Ifor 20170316 (wei) Add MRT Mode

    Panel->Visible=false;
    bMouseDown=false;

    for(int i=0; i<eBinTypeTotal; i++)                                          //Steven 20140510 : Secs Gem
    {
        sBinDoubleContact[i]=new TStringList();
        sBinConsFail[i]     =new TStringList();
        sBinEnableFail[i]   =new TStringList();
        sBinFailPercent[i]  =new TStringList();

        sBinFailIgnore[i]   =new TStringList();                                 //Steven 20140529 Start: Fail Persent & Count同時存在
        sBinCountEnable[i]  =new TStringList();
        sBinCountIgnore[i]  =new TStringList();
        sBinCountNumber[i]  =new TStringList();

        sSpecialBinByArm[i]         =new TStringList();                         //ChungHung 20140730 add ContinuousFailHaveOneCycle start
        sSpecialBinCountByArm[i]    =new TStringList();
        sSpecialBinBySocket[i]      =new TStringList();
        sSpecialBinCountBySocket[i] =new TStringList();

        sLowYield[i]   =new TStringList();                                      //Steven 20140828 : By Bin Yield Monitor
        sArmYield[i]   =new TStringList();                                      //Steven 20140828 : By Bin Arm Yield Monitor
        sSiteYield[i]  =new TStringList();                                      //Steven 20140828 : By Bin Site Yield Monitor

        sBinTraySetT3PosName[i]=new TStringList();                              //Steven 20241205 : Bin to tray增加文字顯示
        sBinTraySetT3Pos[i] =new TStringList();
        sBinType[i]         =new TStringList();
        sT3TrayType[i]      =new TStringList();
        sT6Retest[i]        =new TStringList();
        sBySiteClean[i]     =new TStringList();                                 //Steven 20160308 : By Bin count do auto clean
        sByBinClean[i]      =new TStringList();                                 //Steven 20160308 : By Bin pre site count do auto clean
        sT3CateR[i]         =new TStringList();                                 //Steven 20161221 (wei) : for SCK only Auto 2 has ART

        sSpecBinBySiteCompareEnable[i]       =new TStringList();                //JerryYang 20170712 (Steven) by site by bin compare percent
        sSpecBinBySiteCompareIgnore[i]       =new TStringList();
        sSpecBinBySiteComparePercent[i]      =new TStringList();
        sSpecBinByArmPerSiteCompareEnable[i] =new TStringList();
        sSpecBinByArmPerSiteCompareIgnore[i] =new TStringList();
        sSpecBinByArmPerSiteComparePercent[i]=new TStringList();
        sBinTrayLinked[i]   =new TStringList();                                 //Ifor 20240730 add : BinTrayLinked
        sBinLinked[i]       =new TStringList();                                 //Ifor 20240730 add : BinTrayLinked
        sMagazineSetup[i]   =new TStringList();                                 //Ifor 20240730 add : BinTrayLinked

        for(int j=0; j<iTestBinCount; j++)
        {
            sBinDoubleContact[i]->Add("0");
            sBinConsFail[i]     ->Add("0");
            sBinEnableFail[i]   ->Add("0");
            sBinFailPercent[i]  ->Add("0");

            sBinFailIgnore[i]   ->Add("0");                                     //Steven 20140529 Start: Fail Persent & Count同時存在
            sBinCountEnable[i]  ->Add("0");
            sBinCountIgnore[i]  ->Add("0");
            sBinCountNumber[i]  ->Add("0");

            sSpecialBinByArm[i]         ->Add("0");                             //ChungHung 20140730 add ContinuousFailHaveOneCycle start
            sSpecialBinCountByArm[i]    ->Add("0");
            sSpecialBinBySocket[i]      ->Add("0");
            sSpecialBinCountBySocket[i] ->Add("0");

            sLowYield[i]   ->Add("0");                                          //Steven 20140828 : By Bin Yield Monitor
            sArmYield[i]   ->Add("0");                                          //Steven 20140828 : By Bin Arm Yield Monitor
            sSiteYield[i]  ->Add("0");                                          //Steven 20140828 : By Bin Site Yield Monitor

            sBinTraySetT3PosName[i] ->Add("NotUse");                            //Steven 20241205 : Bin to tray增加文字顯示
            sBinTraySetT3Pos[i]     ->Add("0");
            sBinType[i]             ->Add("0");
            sBySiteClean[i]         ->Add("0");                                 //Steven 20160308 : By Bin count do auto clean
            sByBinClean[i]          ->Add("0");                                 //Steven 20160308 : By Bin pre site count do auto clean

            sSpecBinBySiteCompareEnable[i]->Add("0");                           //JerryYang 20170712 (Steven) by site by bin compare percent
            sSpecBinBySiteCompareIgnore[i]->Add("0");
            sSpecBinBySiteComparePercent[i]->Add("0");
            sSpecBinByArmPerSiteCompareEnable[i]->Add("0");
            sSpecBinByArmPerSiteCompareIgnore[i]->Add("0");
            sSpecBinByArmPerSiteComparePercent[i]->Add("0");

            sBinTrayLinked[i]   ->Add("0");                                     //Ifor 20240730 add : BinTrayLinked
            sBinLinked[i]       ->Add("0");                                     //Ifor 20240730 add : BinTrayLinked
            sMagazineSetup[i]   ->Add("0");                                     //Ifor 20240730 add : BinTrayLinked
        }

        for(int j=e3Auto1; j<eTrayCount; j++)
        {
            sT3TrayType[i]->Add("0");
            sT3CateR[i]->Add("0");                                              //Steven 20161221 (wei) : for SCK only Auto 2 has ART
        }

        for(int j=eAuto1; j<=iAutoRight; j++)
        {
            sT6Retest[i]->Add("0");
        }
    }

    if(CosFunction.bDisableRTBinSet)                                            //wei 20150622 不顯示設定RT Bin set
        cbTestMode->Items->Delete(1);

    if(CosFunction.bOffLineBin || USE_AUTO_RETEST==eartInstall)                 //ChungHung 20141002 add for KYEC AutoRetest
    {
        cbTestMode->Items->Add("Off-Line");
    }
    if(CosFunction.bUseSCKART==false && USE_AUTO_RETEST==eartInstall)           //ChungHung 20141002 add for KYEC AutoRetest
    {
        cbTestMode->Items->Add("ART Re-Test");
        cbTestMode->Items->Add("ART Normal");
    }

    if(CosFunction.bUseMRTMode==true)                                           //Ifor 20170316 (wei) Add MRT Mode
    {
        cbTestMode->Items->Add("MRT Re-Test");
        cbTestMode->Items->Add("MRT Normal");
    }

    for(int tag=0; tag<eBinTypeTotal; tag++)
    {
        MyBinPanel[tag]->mtBinSelect->Width=iColWidth*iTestBinCount+10;
        MyBinPanel[tag]->mtBinSelect->XItem=iTestBinCount;
        MyBinPanel[tag]->InitmtBinSelectData();

        if(CUSTOMER_CODE==CC_KYEC_LEE)                                          //Ifor 20170417 (wei) add Bin 設定頁面底色
        {
            if(tag==eBinRT_ART || tag==eBinFT_ART)                              //Ifor 20170417 add KYEC 喬智要求 ART Mode Bin設定頁面，底色需與7000相同變更修改為0x00B5FFB5
            {
                MyBinPanel[tag]->Panel->Color=TColor(0x00B5FFB5);
            }
            if(tag==eBinRT_MRT || tag==eBinFT_MRT)                              //Ifor 20170417 add KYEC 喬智要求 MRT Mode Bin設定頁面，底色需與7000相同變更修改為0x00E0C1FF
            {
                MyBinPanel[tag]->Panel->Color=TColor(0x00E0C1FF);
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfBinSel::ReadFile(bool bDelOffline, bool bChangeNeme, AnsiString sFileName)//kevin 20160928 新增舊bin 檔名轉換
{
    AnsiString SBuffer1="", SBuffer="";                                         //kevin 20170828 (Steven) add
    AnsiString szDir="", GroupStr, szDirTarget;

    fQAMode->ReadFile();                                                        //Steven 20190326 : QA Sampling
    ReadPrimeDara();                                                            //Steven 20160817 : fixed for Prime & Normal
    AnsiString SavePath[]={"\\BinasgnOff.Data", "\\Binasgn.Data", "\\BinasgnOff-Line.Data", "\\BinasgnOff_ART.Data", "\\Binasgn_ART.Data", "\\Binasgn_MRT_RT.Data", "\\Binasgn_MRT.Data"};  //ChungHung 20141002 add for KYEC AutoRetest  //BinasgnOff is Bin of RT Mode //Ifor 20170316 (wei) Add MRT Mode

    if(IniConfig.bA02BinModelPrime)                                             //Steven 20160623 : 當IniConfig.bFTBin2RTBin==true && 讀檔時, RT Bin要跟FT Bin一樣
    {
        if(IniConfig.bFTBin2RTBin==true &&
           (iBinModelPrime==0 || CosFunction.bDisableRTBinSet))
        {
            SavePath[0]="\\Binasgn.Data";
        }
        else
        {
            SavePath[0]="\\BinasgnOff.Data";
        }
    }
    else
    {
        if(IniConfig.bFTBin2RTBin==true || CosFunction.bDisableRTBinSet)
        {
            SavePath[0]="\\Binasgn.Data";
        }
        else
        {
            SavePath[0]="\\BinasgnOff.Data";
        }
    }

    for(int i=0; i<eTrayCount; i++)
    {
        if(i==eFix1 || i==eAuto1)
        {
            bCanLinkT6[i]=false;
        }
        else if(IniConfig.bAutoTrayLink==false && i<=iAutoRight)
        {
            bCanLinkT6[i]=false;
        }
        else if(AUTO3_IS_MAGAZINE==1 && (i==iMagAtAuto))
        {
            bCanLinkT6[i]=false;
        }
        else if(AUTO_EMPTY_COLOR!=4 && i==eAuto6)
        {
            bCanLinkT6[i]=false;
        }
        else if(i==eBulkBox)
        {
            bCanLinkT6[i]=false;
        }
        else if(Prod.iTrayType[i]==tNotUse)
        {
            bCanLinkT6[i]=false;
        }
        else
        {
            bCanLinkT6[i]=true;
        }
    }

    if(bDelOffline==true)
    {
        szDir=GetRecipeFileName(SavePath[2]);
        DeleteFile(szDir.c_str());
    }

    if(IniConfig.bI32CanCelErrorBin && bChangeNeme==false)                      //kevin 20160928 取消error bin 要手到在 outshuttle 取出
    {
        szDir=GetRecipeFileName("BinasgnOff.Data");
        FormSysTools->OpenFormData(szDir);                                      //Steven 20140531 : 為了加快ini讀取速度
        //I/F Error----------
        bCancelErrorBin=FormSysTools->CheckAndReadIniData("I/F Error", "CancelError", false);  //kevin 20160724 儲存取消BIN 檔案
        FormSysTools->CloseFormData();                                          //Steven 20140531 : 為了加快ini讀取速度
    }
    else
    {
        bCancelErrorBin=false;
        if(IniConfig.bI33ErrorBinBox && bChangeNeme==false)                     //kevin 20160928  //kevin 20160819 error bin 要放到 Bin Box
        {
            szDir=GetRecipeFileName("BinasgnOff.Data");
            FormSysTools->OpenFormData(szDir);                                  //Steven 20140531 : 為了加快ini讀取速度
            //I/F Error----------
            iHWFix_BinBox=FormSysTools->CheckAndReadIniData("I/F Error", "ErrorBinBox", 0); //kevin 20160819 error bin 要放到 Bin Box
            iErrorBinBoxAlarm=FormSysTools->CheckAndReadIniData("I/F Error", "Alarm Count", 0); //kevin 20160819 error bin Alarm 數量
            if(iHWFix_BinBox==1 && iErrorBinBoxAlarm<=2)                        //kevin 20200722
                iErrorBinBoxAlarm=2;
            FormSysTools->CloseFormData();                                      //Steven 20140531 : 為了加快ini讀取速度
        }
        else
        {
            iHWFix_BinBox=0;
        }
    }

    if(CosFunction.bAllSiteSameFailBinShowAlarm)                                //JerryYang 20160913 矽品要求當測試結果中所有site的bin都是所設定一樣的fail bin要跳alarm
    {
        szDir=GetRecipeFileName(SavePath[0]);
        FormSysTools->OpenFormData(szDir);                                      //Steven 20140531 : 為了加快ini讀取速度
        //I/F Error----------
        SBuffer1=FormSysTools->CheckAndReadIniData("AllSiteSameBin", "bSpecificFailBin", AnsiString("5")); //kevin 20170828 (Steven) add
        if(SBuffer1=="5")
        {
            SBuffer1="";
            for(int i=0; i<iTestBinCount; i++)                                  //kevin 20170828 add
            {
                bSpecificBin[i]=FormSysTools->CheckAndReadIniData("AllSiteSameBin", "bSpecificFailBin"+IntToStr(i), false);
                SBuffer.sprintf("%d,", bSpecificBin[i]);
                SBuffer1+=SBuffer;
            }
            FormSysTools->WriteIniData("AllSiteSameBin", "bSpecificFailBin", SBuffer1); //JerryYang 20160913 矽品要求當測試結果中所有site的bin都是所設定一樣的fail bin要跳alarm
            SBuffer1 = FormSysTools->CheckAndReadIniData("AllSiteSameBin", "bSpecificFailBin", AnsiString("0")); //kevin 20170828 add
        }

        for(int i=0; i<iTestBinCount; i++)                                      //kevin 20170828 add
        {
            bSpecificBin[i]=(SBuffer1.SubString(i*2+1, 1)=="1");
        }
        FormSysTools->CloseFormData();                                          //Steven 20140531 : 為了加快ini讀取速度
    }

    for(int tag=0; tag<eBinTypeTotal; tag++)
    {
        if(CosFunction.bUseMRTMode==false)                                      //Steven 20170901 (wei) : 沒用到就不要產生BIN檔案
        {
            if(tag==RT_MRT || tag==FT_MRT)
            {
                continue;
            }
        }

        if(USE_AUTO_RETEST==0)
        {
            if(tag==RT_ART || tag==FT_ART)
            {
                continue;
            }
        }
        else if(USE_AUTO_RETEST==1 && CosFunction.bUseSCKART)
        {
            if(tag==RT_ART || tag==FT_ART)
            {
                continue;
            }
        }

        szDir=GetRecipeFileName(SavePath[tag]);

        FormSysTools->OpenFormData(szDir);                                      //Steven 20140531 : 為了加快ini讀取速度
        if(bChangeNeme)                                                         //kevin 20160928
        {
             if(tag!=1)
                continue;
        }
        //I/F Error----------
        if(CUSTOMER_CODE==CC_TFME_CHINA)                                        //Ifor 20251029 add:通富微電子要求I/F Error BIN存檔與Category相同
        {
            BinSelect[tag].IfErrorT3Define=FormSysTools->CheckAndReadIniData("I/F Error", "BinDefine",  0);
            BinSelect[tag].IfErrorT3=FormSysTools->CheckAndReadIniData("I/F Error", "Bin",  e3Fix2);   //JerryYang 20230926     //Steven 20150413 : I/F Error讀檔預設由-1改為ePosFix2
            if(BinSelect[tag].IfErrorT3Define==0)
            {
                BinSelect[tag].IfErrorT3=BinSelect[tag].IfErrorT3;
                FormSysTools->WriteIniData("I/F Error", "Bin", BinSelect[tag].IfErrorT3+1);
                FormSysTools->WriteIniData("I/F Error", "BinDefine", 1);
            }
            else
            {
                BinSelect[tag].IfErrorT3=BinSelect[tag].IfErrorT3-1;
            }

        }
        else
        {
            BinSelect[tag].IfErrorT3=FormSysTools->CheckAndReadIniData("I/F Error", "Bin",  e3Fix2);   //JerryYang 20230926     //Steven 20150413 : I/F Error讀檔預設由-1改為ePosFix2
        }
        //ChungHung 20111110 檢查檔案是否有IFError

        if(bCancelErrorBin || iHWFix_BinBox==1)                                 //kevin 20160912 20160819 error bin 要放到 Bin Box //kevin 20160724 取消 ERROR BIN
        {
            BinSelect[tag].IfErrorT3=iTo3Unload[eBulkBox];
        }
        else if(IniConfig.bBinBox && AutoForm[iBinBoxAtFix]->iTrayType==3)
        {
            BinSelect[tag].IfErrorT3=iTo3Unload[iBinBoxAtFix];
        }
        else
        {
            if(BinSelect[tag].IfErrorT3<0)                                      //Eastsun 20251224 : 修正設定異常
            //=>if(iTo6Unload[BinSelect[tag].IfErrorT3]<0)                      //Steven 20131018 : Error Bin設定異常 <= --> <
            {
                BinSelect[tag].IfErrorT3=e3Fix2;                                //Steven 20150209 : ePosFix1 --> ePosFix2 for HT9046LS + Rotate
            }

            if(CosFunction.bLoaderTrayToAuto1 && TrayForm.LoaderToEmptyColor[iRunStartMode]==2)
            {
                if(iTo6Unload[BinSelect[tag].IfErrorT3]==eAuto1)
                    BinSelect[tag].IfErrorT3=iTo3Unload[eMag14];
            }

            if(AUTO3_IS_MAGAZINE==1)                                            //JerryYang 20221207 : Magazine not use AUTO3
            {
                if(iTo6Unload[BinSelect[tag].IfErrorT3]==eAuto3)
                    BinSelect[tag].IfErrorT3=iTo3Unload[eMag14];

                if(TestIF_File.iMagFixTrayType==1 &&
                   iTo6Unload[BinSelect[tag].IfErrorT3]>=iFixMin &&
                   iTo6Unload[BinSelect[tag].IfErrorT3]<=iFixRightHalf)
                {
                    BinSelect[tag].IfErrorT3=iTo3Unload[eMag14];
                }
            }
            else
            {
                if(TrayForm.iFixTrayMode==false &&
                   iTo6Unload[BinSelect[tag].IfErrorT3]>=iFixMax &&
                   iTo6Unload[BinSelect[tag].IfErrorT3]<=iFixRightHalf)
                {
                    BinSelect[tag].IfErrorT3=iTo3Unload[iFixMax];
                }
            }

            if(TestIF_File.bEnableQASampling &&                                 //Steven 20190326 : QA Sampling
               iTo6Unload[BinSelect[tag].IfErrorT3]==iTo6Unload[TestIF_File.iQASamplingT3Pos])
            {
                if(iTo6Unload[TestIF_File.iQASamplingT3Pos]==iFixMax)
                    BinSelect[tag].IfErrorT3=TestIF_File.iQASamplingT3Pos-1;
                else if(iTo6Unload[TestIF_File.iQASamplingT3Pos]==iFixRightHalf)
                    BinSelect[tag].IfErrorT3=TestIF_File.iQASamplingT3Pos-1;
                else
                    BinSelect[tag].IfErrorT3=TestIF_File.iQASamplingT3Pos+1;
            }

            if(USE_ROTATE==eCynRot)                                             //kevin 20130722  氣缸版    //Steven 20121001 : 旋轉Kit
            {
                if(iTo6Unload[BinSelect[tag].IfErrorT3]==iRotate_Out_Tray6)
                    BinSelect[tag].IfErrorT3=iTo3Unload[eFix2];                 //Steven 20230930 : ePosFix2 --> eFix2  //Steven 20150209 : ePosFix1 --> ePosFix2 for HT9046LS + Rotate
            }

            if(MachineTypeChoice==Type_HT9045 &&                                //JerryYang 20230608 : HT-9045機型使用汽缸FIX板+Rotate時鎖定不得使用FIX3
               FIX3_FULL_PLACE==Fix3K_UseCylinder &&
               USE_ROTATE==eMotRot &&
               iTo6Unload[BinSelect[tag].IfErrorT3]==e3Fix3)
            {
                BinSelect[tag].IfErrorT3=iTo3Unload[eFix2];
            }
        }

        MyBinPanel[tag]->iErrorT6=iTo6Unload[BinSelect[tag].IfErrorT3];

//      if(bTest==false) //kevin 20151119 不使用256 bin show form
        {
            for(int i=0; i<iTestBinCount; i++)
            {
                //Bin Setting---------
                for(int j=eBinNotUse; j<eBinSetTotal; j++)
                    MyBinPanel[tag]->BackT6PosTray[i][j]=0;
            }

            ReadFunctionData(tag);

            //AI(ht9045-v899) 20260820: PTI HT-90* 的 Fix1 位置是 rotate module,不會指派 Category;
            //原本 Pass/Fail 預設 0(Pass) 會讓未使用的 Fix1 顯示成與好品相同的綠色,造成操作員誤判,
            //故未指派 Category 時預設為 Fail。Fix4 是 Fix1 的下半盤,必須一起設定,
            //否則 GPIB AlarmSetup 會因 Fix 盤上下 Pass/Fail 不一致而擋下設定。
            bool bNoCategoryToFix1=(CUSTOMER_CODE==CC_PTI);
            for(int iChkBin=0; iChkBin<iTestBinCount && bNoCategoryToFix1==true; iChkBin++)
            {
                if(BinSelect[tag].iCatDataT3Pos[iChkBin]==e3PosFix1 ||
                   BinSelect[tag].iCatDataT3Pos[iChkBin]==e3PosFix4)
                {
                    bNoCategoryToFix1=false;
                }
            }

            for(int i=0; i<eTrayCount; i++)
            {
                //Pass Fail-------
                MyBinPanel[tag]->iT6IsFail[i]=FormSysTools->CheckAndReadIniData(s6TrayName[i], "Pass/Fail", 0); //define stack store pass or fail
                if(MyBinPanel[tag]->iT6IsFail[i]==0 &&
                   MyBinPanel[tag]->iErrorT6==i)
                {
                    MyBinPanel[tag]->iT6IsFail[i]=1;
                }

                //AI(ht9045-v899) 20260820: 未指派 Category 的 Fix1/Fix4 預設為 Fail(說明見本函式上方)
                if(bNoCategoryToFix1==true &&
                   (i==eFix1 || i==eFix4) &&
                   MyBinPanel[tag]->iT6IsFail[i]==0)
                {
                    MyBinPanel[tag]->iT6IsFail[i]=1;
                }

                BinSelect[tag].iStackDefFailCate[iTo3Unload[i]]=MyBinPanel[tag]->iT6IsFail[i];

                //Link------------
                if(IniConfig.bAutoTrayLink==true)                               //jou 2012-06-14 Auto Tray Link
                {
                    if((i>=eAuto2 && i<=iAutoRight) ||
                       (AUTO3_IS_MAGAZINE==1 &&                                 //Ifor 20231122 add Magazine Link
                        i>=iMagMin && i<=iMagMax))
                    {
                        MyBinPanel[tag]->bT6Link[i]=FormSysTools->CheckAndReadIniData(s6TrayName[i], "Link", 0);
                        BinSelect[tag].bAutoLink[iTo3Unload[i]]=MyBinPanel[tag]->bT6Link[i];
                    }
                }

                if(i>=eFix2 && i<=iFixRight)
                {
                    MyBinPanel[tag]->bT6Link[i]=FormSysTools->CheckAndReadIniData(s6TrayName[i], "Link", 0);
                    BinSelect[tag].bAutoLink[iTo3Unload[i]]=MyBinPanel[tag]->bT6Link[i];
                }
                //Ifor 20231122 add Magazine Link
                //==>
                if(AUTO3_IS_MAGAZINE==1)
                {
                    if(i>=eMag1 && i<=eMag14)
                    {
                        iLinkTrayMax=eMag14;
                        BinSelect[tag].bMagazineLink[i-eMag1]=FormSysTools->CheckAndReadIniData(s6TrayName[i], "Link", 0);
                        MyBinPanel[tag]->bT6Link[i]=BinSelect[tag].bMagazineLink[i-eMag1];
                    }
                }
                //<==
                //Ifor 20231122 add Magazine Link

                if(CosFunction.bUseSCKART)                                      //Steven 20161214 (wei) : For SCK ART
                {
                    if(i<=iAutoRight)
                    {
                        if(USE_AUTO_RETEST==eartInstall &&                      //Steven 20170313 (wei) : Mark for P170306-SCK-H9-03
                           (tag==FT || tag==OffT))                              //Steven 20170317 (wei) : Off T也要可以ART
                        {
                            MyBinPanel[tag]->bT6ART[i]=FormSysTools->CheckAndReadIniData(s6TrayName[i], "AutoRetest", false);
                            if(MyBinPanel[tag]->bT6ART[i])                      //Steven 20161221 (wei) : Cate R for ART
                                BinSelect[tag].iARTTrack=i;
                        }
                        else
                        {
                            MyBinPanel[tag]->bT6ART[i]=false;
                        }
                    }
                    else
                    {
                        MyBinPanel[tag]->bT6ART[i]=false;
                    }

                    if(USE_AUTO_RETEST==eartInstall &&                          //Steven 20161221 (wei) : Cate R for ART
                       TestIF_File.iSCKART_SortMode==1 &&
                       (tag==FT || tag==OffT) || IniConfig.bA38_SLT_Summary)    //JerryYang 20220923 : add for SLT lot summary
                    {
                        MyBinPanel[tag]->bT6CateR[i]=FormSysTools->CheckAndReadIniData(s6TrayName[i], "Cate R", false);
                    }
                    else
                    {
                        MyBinPanel[tag]->bT6CateR[i]=false;
                    }
                    BinSelect[tag].bAutoRetest[iTo3Unload[i]]=MyBinPanel[tag]->bT6ART[i];
                    BinSelect[tag].bCateR[iTo3Unload[i]]=MyBinPanel[tag]->bT6CateR[i];
                }
                else
                {
                    if(i<=iAutoRight)                                           //ChungHung 20140317 add Auto Retest
                    {
                        if(USE_AUTO_RETEST==eartInstall &&
                           (IniConfig.bA10_AutoReTest ||
                            bAutoReTest_ART) &&
                           tag==FT_ART)                                         //ChungHung 20141002 add for KYEC AutoRetest
                        {
                            MyBinPanel[tag]->bT6ART[i]=FormSysTools->CheckAndReadIniData(s6TrayName[i], "AutoRetest", false);

                            if(CUSTOMER_CODE==CC_TSMC_TAINAN)                   //wei 20170116 (Steven) TSMC強制Auto3設AUTO RETEST
                            {
                                if(BinSelect[tag].bAutoRetest[iTo3Unload[i]]==true && MyBinPanel[tag]->iT6IsFail[i]==0)
                                {
                                    MyBinPanel[tag]->bT6ART[i]=false;
                                }
                                MyBinPanel[tag]->iT6IsFail[eAuto3]=1;
                                BinSelect[tag].bAutoRetest[e3Auto3]=true;
                                MyBinPanel[tag]->bT6ART[eAuto3]=true;
                            }
                            else if(CUSTOMER_CODE!=CC_ASE_KaohSiung &&          //kevin 20150626 強制Auto2設AUTO RETEST
                                    CUSTOMER_CODE!=CC_KYEC_LEE)
                            {
                                MyBinPanel[tag]->iT6IsFail[eAuto2]=1;           //強制Auto2設AUTO RETEST
                                BinSelect[tag].bAutoRetest[e3Auto2]=true;
                                MyBinPanel[tag]->bT6ART[eAuto2]=true;
                            }
                        }
                        else
                        {
                            MyBinPanel[tag]->bT6ART[i]=false;
                        }
                    }
                    else
                    {
                        MyBinPanel[tag]->bT6ART[i]=false;
                    }
                    BinSelect[tag].bAutoRetest[iTo3Unload[i]]=MyBinPanel[tag]->bT6ART[i];
                }
            }
        }
        mtTrayNameSetColor(tag);
        FormSysTools->CloseFormData();                                          //Steven 20140531 : 為了加快ini讀取速度
    }

    ReadWriteMRTMode(0);                                                        //Ifor 20170405 (wei) add MRT Use Mode By Setup File //讀取 MRT Mode
    ReadWriteSpecialFunction(false);                                            //JerryYang 20220923 : bOutShtLoseICSetErrUntilOneCycle改為可By工作檔

    for(int tag=0; tag<eBinTypeTotal; tag++)                                    //ChungHung 20111110 檢查檔案是否有IfErrorT3
    {
        BinSelect[tag].iStackDefFailCate[BinSelect[tag].IfErrorT3]=1;
    }
    CancelErrorBin->Checked = bCancelErrorBin;                                  //kevin 20160724
    rg_FixBinBox->ItemIndex = iHWFix_BinBox;                                    //kevin 20160819 error bin 要放到 Bin Box "\\Binasgn.Data";
    ed_FixBinBoxAlarmCount->Text=iErrorBinBoxAlarm;                             //kevin 20160819

    SetWorkParameter();                                                         //Prod得在更新後才能用

    for(int i=0; i<eBinTypeTotal; i++)                                          //Steven 20161123 : Add protection when read bin data
    {
        sT3TrayType[i]->Clear();
        sT3CateR[i]->Clear();
        sT6Retest[i]->Clear();

        for(int j=e3Auto1; j<eTrayCount; j++)
        {
            sT3TrayType[i]->Add("0");
            sT3CateR[i]->Add("0");
        }

        for(int j=eAuto1; j<=iAutoRight; j++)
        {
            sT6Retest[i]->Add("0");
        }
    }

    for(int tag=0; tag<eBinTypeTotal; tag++)                                    //Steven 20140510 : Secs Gem
    {
        for(int i=0; i<iTestBinCount; i++)
        {
            if(sBinType[tag]->Count<=i)                                         //Steven 20161123 : Add protection when read bin data
                sBinType[tag]->Add(AnsiString(Prod.bIsPassBin[i]?"0":"1"));
            else
                sBinType[tag]->Strings[i]=AnsiString(Prod.bIsPassBin[i]?"0":"1");
        }

        for(int i=0; i<eTrayCount; i++)
        {
            if(iTo3Unload[i]>=0 && iTo3Unload[i]<e3TrayCount)
            {
                sT3TrayType[tag]->Strings[iTo3Unload[i]]=BinSelect[tag].iStackDefFailCate[iTo3Unload[i]];
                sT3CateR[tag]->Strings[iTo3Unload[i]]=(BinSelect[tag].bCateR[iTo3Unload[i]])?"1":"0";
            }
        }

        for(int i=eAuto1; i<=iAutoRight; i++)
        {
            if(iTo3Unload[i]>=0 && iTo3Unload[i]<e3TrayCount)
            {
                sT6Retest[tag]->Strings[i]=(BinSelect[tag].bAutoRetest[iTo3Unload[i]])?"1":"0";
            }
        }
    }

    SetPrimeButton();                                                           //Steven 20160624 : 重新顯示按鈕
    LotSummary.SetIsRTBin();
    ATKRecipeInfo->SaveFile();                                                  //Steven 20170901 (wei) : For ATK要新增工作檔比對用的檔案
}
//---------------------------------------------------------------------------
void TfBinSel::ShowChangeBinMessage()                                           //jou 2010-12-15
{
    if(CosFunction.bOneCycleCanChangeContinuesFailBin)                          //Steven 20151123 : Modified because of CosFunction.bOneCycleCanChangeContinuesFailBin
    {
        labWarning->Caption="";
    }
    else
    {
        labWarning->Caption="* ";
    }
    if(OutArmSuck.HasIC())
        labWarning->Caption=labWarning->Caption+" OutArmSuck ";
    if(ShuttleHasIC())
        labWarning->Caption=labWarning->Caption+" Shuttle ";
    if(IndexHasIC())
        labWarning->Caption=labWarning->Caption+" Index ";
    if(MOT[MMAuto1].HasIC())
        labWarning->Caption=labWarning->Caption+" Auto 1 ";
    if(MOT[MMAuto2].HasIC())
        labWarning->Caption=labWarning->Caption+" Auto 2 ";
    if(MOT[MMAuto3].HasIC())
        labWarning->Caption=labWarning->Caption+" Auto 3 ";
    if(MOT[MMAuto4].HasIC())
        labWarning->Caption=labWarning->Caption+" Auto 4 ";                     //Steven 20230907 : For HT-9011UC
    if(MOT[MMAuto5].HasIC())
        labWarning->Caption=labWarning->Caption+" Auto 5 ";
    if(MOT[MMAuto6].HasIC())
        labWarning->Caption=labWarning->Caption+" Auto 6 ";
    if(MOT[MMAuto1_Car].HasIC())
        labWarning->Caption=labWarning->Caption+" Auto 1 track ";
    if(MOT[MMAuto2_Car].HasIC())
        labWarning->Caption=labWarning->Caption+" Auto 2 track ";
    if(MOT[MMAuto3_Car].HasIC())
        labWarning->Caption=labWarning->Caption+" Auto 3 track ";
    if(MOT[MMAuto4_Car].HasIC())
        labWarning->Caption=labWarning->Caption+" Auto 4 track ";
    if(MOT[MMAuto5_Car].HasIC())
        labWarning->Caption=labWarning->Caption+" Auto 5 track ";
    if(MOT[MMAuto6_Car].HasIC())
        labWarning->Caption=labWarning->Caption+" Auto 6 track ";
    if(MOT[MManualTray1].HasRealIC())
        labWarning->Caption=labWarning->Caption+" Fix 1 ";
    if(MOT[MManualTray2].HasRealIC())
        labWarning->Caption=labWarning->Caption+" Fix 2 ";
    if(MOT[MManualTray3].HasRealIC())
        labWarning->Caption=labWarning->Caption+" Fix 3 ";
    if(MOT[MManualTray4].HasRealIC())
        labWarning->Caption=labWarning->Caption+" Fix 4 ";                      //Steven 20230907 : For HT-9011UC
    if(MOT[MManualTray5].HasRealIC())
        labWarning->Caption=labWarning->Caption+" Fix 5 ";
    if(MOT[MManualTray6].HasRealIC())
        labWarning->Caption=labWarning->Caption+" Fix 6 ";

    if(CosFunction.bOneCycleCanChangeContinuesFailBin)                          //Steven 20151123 : Modified because of CosFunction.bOneCycleCanChangeContinuesFailBin
    {
        if(labWarning->Caption!="")
            labWarning->Caption+"Has IC can't change bin!!";
    }
    else
    {
        labWarning->Caption=labWarning->Caption+"Has IC can't change bin!!";
    }
}
//---------------------------------------------------------------------------
void __fastcall TfBinSel::FormShow(TObject *Sender)
{
    bool bFlag;
    bShow=false;                                                                //Steven 20090713
    bSaveBin=false;
    AnsiString S;
    int iBin=0;//, iT6;                                                         //Sam 20200622 : Control Bin 顯示

    fTrayAssignment->FixCanUse();

    for(int tag=0; tag<eBinTypeTotal; tag++)
    {
        MyBinPanel[tag]->mtBinSelect->Width=iColWidth*iTestBinCount+10;
        MyBinPanel[tag]->mtBinSelect->XItem=iTestBinCount;
        MyBinPanel[tag]->InitmtBinSelectData();
        MyBinPanel[tag]->sbBinSetScroll->Max        =MyBinPanel[tag]->sbBinSetting->HorzScrollBar->Range;   //Steven 20240420 : 調整scrollbar位置
        MyBinPanel[tag]->sbBinSetScroll->PageSize   =600;
    }

    ReadParam();
    ReadPrimeDara();
    ReadFile(false, false, "");                                                 //kevin 20160928

    S.sprintf("Contact Parameter  ''%s''  ",GetLastOpenFN());
    Caption=S;

    bFlag=CanChangeData(false);
    if(bFlag)                                                                   //kevin 20150525
    {
        labWarning->Caption="";                                                 //jou 2010-12-15
        for(int tag=0; tag<eBinTypeTotal; tag++)
            MyBinPanel[tag]->Panel->Enabled=fSecurity->Insufficient(20, false);
    }
    else
    {
        ShowChangeBinMessage();                                                 //jou 2010-12-15
        if(CosFunction.bOneCycleCanChangeContinuesFailBin)                      //ChungHung 20150519 add Continues Failure BIN can select after "OneCycle".
        {
            if(OutArmSuck.HasIC()==false && ShuttleHasIC()==false && IndexHasIC()==false)
            {
                for(int tag=0; tag<eBinTypeTotal; tag++)
                    MyBinPanel[tag]->Panel->Enabled=fSecurity->Insufficient(20, false);
            }
            else
            {
                for(int tag=0; tag<eBinTypeTotal; tag++)
                    MyBinPanel[tag]->Panel->Enabled=false;
            }
        }
        else
        {
            for(int tag=0; tag<eBinTypeTotal; tag++)
                MyBinPanel[tag]->Panel->Enabled=false;
        }
    }

    if(NUMBER_PANEL_TYPE==3 ||
       NUMBER_PANEL_TYPE==4)                                                    //Sam 20240604 : 新增 BinDisplay TFT
        HSys.BinDisCtrl->ProcessStopStart(false);

    if(CUSTOMER_CODE==CC_SCC ||                                                 //Steven 20101221
       CUSTOMER_CODE==CC_SCK)                                                   //ChungHung 20130621 add SCK RMS
    {
        if(IniConfig.bEnableRms && AccessLevel<=iDefEngineerLevel)              //jou 2014-06-19 Security Have 5 Level 1->iDefEngineerLevel
        {
            for(int tag=0; tag<eBinTypeTotal; tag++)
                MyBinPanel[tag]->Panel->Enabled=false;
        }
        else                                                                    //跟上面有無IC衝突,所以不設定!
        {
        }
    }

    GetBinSettingAuth();                                                        //Steven 20120330 : FT跟RT的Enable
    for(int tag=0; tag<eBinTypeTotal; tag++)
    {
        if(MyBinPanel[tag]->Panel->Enabled==true)
            MyBinPanel[tag]->Panel->Enabled=authBinSetting[tag];
    }

    for(int tag=0; tag<eBinTypeTotal; tag++)
    {
        for(int i=eAuto1; i<eTrayCount; i++)
        {
            if(Prod.iTrayType[i]==tNotUse)
            {
                MyBinPanel[tag]->mtTrayItem->SetCellNumber    (eItemName,  i+eBinSetting, "");
                MyBinPanel[tag]->mtTrayItem->SetCellColorIndex(eItemName,  i+eBinSetting, eCLBtnFace);
            }
            else
            {
                MyBinPanel[tag]->mtTrayItem->SetCellNumber    (eItemName,  i+eBinSetting, s6TrayName[i].c_str());
                MyBinPanel[tag]->mtTrayItem->SetCellColorIndex(eItemName,  i+eBinSetting, eCLGray);
            }
        }

        if(AUTO3_IS_MAGAZINE>0)
        {
            MyBinPanel[tag]->mtTrayItem->SetCellColorIndex(eItemName,  iMagAtAuto+eBinSetting, eCLBtnFace);
        }
    }

    iMode=iTestRunMode;
    if(iMode==RT)                                                               //Steven 20090810 : ReTest Bin Select
    {
        if((IniConfig.bA02BinModelPrime && IniConfig.bFTBin2RTBin==true &&      //Steven 20160623 : 當IniConfig.bFTBin2RTBin==true && 讀檔時, RT Bin要跟FT Bin一樣
           (iBinModelPrime==0 || CosFunction.bDisableRTBinSet)) ||
           (IniConfig.bA02BinModelPrime==false &&
           (IniConfig.bFTBin2RTBin==true || CosFunction.bDisableRTBinSet)))
        {
            cbTestMode->Text="Normal";
        }
        else
        {
            cbTestMode->Text="Re-Test";
        }
    }
    else if(iMode==OffT)
    {
        cbTestMode->Text="Off-Line";
    }
    else if(iMode==RT_ART)                                                      //ChungHung 20141002 add for KYEC AutoRetest
    {
        cbTestMode->Text="ART Re-Test";
    }
    else if(iMode==FT_ART)                                                      //ChungHung 20141002 add for KYEC AutoRetest
    {
        cbTestMode->Text="ART Normal";
    }
    else if(iMode==RT_MRT)                                                      //Ifor 20170316 (wei) add MRT Mode
    {
        cbTestMode->Text="MRT Re-Test";
    }
    else if(iMode==FT_MRT)                                                      //Ifor 20170316 (wei) add MRT Mode
    {
        cbTestMode->Text="MRT Normal";
    }
    else
    {
        cbTestMode->Text="Normal";
    }

    if(AccessLevel<LevelSet.AccessLevel[20])
    {
        tsNormal->Enabled=false;
        tsRetest->Enabled=false;
        tsOffline->Enabled=false;
        tsArtFT->Enabled=false;                                                 //ChungHung 20141002 add for KYEC AutoRetest
        tsArtRT->Enabled=false;                                                 //ChungHung 20141002 add for KYEC AutoRetest
        tsMrtFT->Enabled=false;                                                 //Ifor 20170316 (wei) add MRT Mode
        tsMrtRT->Enabled=false;                                                 //Ifor 20170316 (wei) add MRT Mode
    }
    else
    {
        tsNormal->Enabled=true;
        tsRetest->Enabled=true;
        tsOffline->Enabled=true;
        tsArtFT->Enabled=true;                                                  //ChungHung 20141002 add for KYEC AutoRetest
        tsArtRT->Enabled=true;                                                  //ChungHung 20141002 add for KYEC AutoRetest
        tsMrtFT->Enabled=true;                                                  //Ifor 20170316 (wei) add MRT Mode
        tsMrtRT->Enabled=true;                                                  //Ifor 20170316 (wei) add MRT Mode
    }

    if(CosFunction.bLotStartLockCriticalPara && RunInfo.bLotStart)              //JerryYang 20220310 : ATP要求鎖定critical parameter
    {
        if(bAuthCriticalPara[2])
        {
            tsNormal->Enabled=false;
            tsRetest->Enabled=false;
            tsOffline->Enabled=false;
            tsArtFT->Enabled=false;
            tsArtRT->Enabled=false;
            tsMrtFT->Enabled=false;
            tsMrtRT->Enabled=false;
        }
    }
    bCheckOffLineLevel=true;

    fBinSel->Height=920;

    //以下保持在最下面-----
    cbTestModeChange(this);
    bShow=true;
    Left=10;
    Top=10;

    if(IniConfig.bI32CanCelErrorBin)                                            //kevin 20160724 error bin 功能取消
        CancelErrorBin->Visible=true;
    else
        CancelErrorBin->Visible=false;

    if(IniConfig.bI33ErrorBinBox)                                               //kevin 20160912 20160819 error bin 要放到 Bin Box
    {
        if(Prod.iOutArmBinBoxX>-1000 && Prod.iOutArmBinBoxY>-1000)              //kevin 20190706 Set up Teach Bin box pos
        {
            LabBulkBox->Caption= "Set up Teach Bin box pos";
            rg_FixBinBox->Enabled=false;
            LabBulkBox->Color = clRed;
            iHWFix_BinBox=0;
        }
        else
        {
            LabBulkBox->Caption="BulkCount";
            rg_FixBinBox->Enabled=true;
            LabBulkBox->Color =(TColor)0x00C2B8A6;
        }
        BulkBox->Visible=true;
    }
    else
    {
        BulkBox->Visible=false;
    }

    palSpecificBin->Visible=false;                                              //JerryYang 20170301 (wei) 存檔後不顯示
    if(CosFunction.bAllSiteSameFailBinShowAlarm==true &&                        //JerryYang 20170221 (wei) 改成可複選bin
       IniConfig.bI34AllSiteAreSameFailBinShowAlarm==true)                      //JerryYang 20160913 矽品要求當測試結果中所有site的bin都是所設定一樣的fail bin要跳alarm
    {
        btnSettingSpecificBin->Visible=true;
        sgSpecificBin->ColCount =iTestBinCount;                                 //kevin 20170828 (Steven) add 設定 BIN 數量
        for(int i=0; i<iTestBinCount; i++)                                      //kevin 20170828 add
        {
            sgSpecificBin->Cells[i][0]="Bin"+IntToStr(i);
            if(bSpecificBin[i]==true)
                sgSpecificBin->Cells[i][1]="V";
            else
                sgSpecificBin->Cells[i][1]="";
        }
    }
    else
    {
        btnSettingSpecificBin->Visible=false;
    }

    if(CosFunction.bUseMRTMode==true)                                           //Ifor 20170405 (wei) add MRT Use Mode By Setup File
    {
        cbUseMRTMode->Visible=true;
        if(fMain->CheckCanChangeRealDummy()==false)
        {
            cbUseMRTMode->Enabled=false;                                        //Ifor 20170405 add 機台內有IC不可修改MRT 功能開關
        }
    }
    else
    {
        cbUseMRTMode->Visible=false;
    }

    pnlAutoSiteMap->Visible=CosFunction.bAutoSiteMappingSetOpenBIN;             //jou 20200928 : Auto Site Mapping Set Open BIN
    chkShow0Xbin->Visible=CosFunction.bShow0Xbin;                               //jou 20220719 : show 0X bin
    labASMPassBin->Visible=CosFunction.bAutoSiteMappingSetPassBIN;              //jou 20230221 : Auto Site Mapping Set Pass BIN
    cbbASMPassBin->Visible=CosFunction.bAutoSiteMappingSetPassBIN;              //jou 20230221 : Auto Site Mapping Set Pass BIN

    if(CUSTOMER_CODE==CC_Greatek)                                               //Sam 20200622 : Control Bin 顯示
    {
        edControlBinCheckPoint->Visible=true;                                   //Sam 20201221 : 修正 Control Bin 顯示
        edControlBinCheckPoint->Parent=PageControl1;
        edControlBinCheckPoint->BringToFront();
        edControlBinCheckPoint->Left        =183;
        edControlBinCheckPoint->Top         =iRowHeight*19;
        edControlBinCheckPoint->Height      =iRowHeight-2;
        edControlBinCheckPoint->Width       =97;

        mtBinSelectBy->Parent=PageControl1;
        mtBinSelectBy->BringToFront();
        mtBinSelectBy->Left        =357;
        mtBinSelectBy->Top         =iRowHeight*18;
        mtBinSelectBy->Height      =iRowHeight*3;                               //Sam 20201221 : 修正 Control Bin 顯示
        mtBinSelectBy->Width=iColWidth*iTestBinCount+10;
        mtBinSelectBy->XItem=iTestBinCount;
        mtBinSelectBy->YItem=3;                                                 //Sam 20201221 : 修正 Control Bin 顯示
        mtBinSelectBy->Visible=True;

        mtBinSelectBy->SetColorMap(eCLWhite,   clWhite);
        mtBinSelectBy->SetColorMap(eCLGreen,   clGreen);
        mtBinSelectBy->SetColorMap(eCLRed,     clRed);
        mtBinSelectBy->SetColorMap(eCLYellow,  TColor(0x000080FF));
        mtBinSelectBy->SetColorMap(eCLPurple,  clPurple);
        mtBinSelectBy->SetColorMap(eCLBlue,    clBlue);
        mtBinSelectBy->SetColorMap(eCLGray,    clGray);
        mtBinSelectBy->SetColorMap(eCLSilver,  clSilver);
        mtBinSelectBy->SetColorMap(eCLBtnFace, clBtnFace);
        mtBinSelectBy->SetColorMap(eCLOlive,   clOlive);
        mtBinSelectBy->Color       =clWhite;
        mtBinSelectBy->Font->Name  ="Arial";
        mtBinSelectBy->Font->Size  =10;
        mtBinSelectBy->Font->Color =clRed;

        if(fProductionInfo->iControlBinNum>0)
        {
            for(int i=0; i<fProductionInfo->iControlBinNum; i++)
            {
                iBin=fProductionInfo->iControlBin[i];
                mtBinSelectBy->SetCellNumber(iBin, 0, FloatToStr(fProductionInfo->dControlPercent[i]));
                mtBinSelectBy->SetCellNumber(iBin, 1, FloatToStr(fProductionInfo->iControlCount[i]));
                mtBinSelectBy->SetCellNumber(iBin, 2, FloatToStr(fProductionInfo->dControlSiteToSite[i]));
            }
        }

        if(fProductionInfo->iControlBinCheckPoint>0)                            //Sam 20201221 : 增加保護
        {
            edControlBinCheckPoint->Text="Interval time : "+IntToStr(fProductionInfo->iControlBinCheckPoint/60);
        }
        else if(fProductionInfo->iControlBinCheckCount>0)                       //Sam 20201221 : 增加 Control Bin Count 監控
        {
            edControlBinCheckPoint->Text="Interval count : "+IntToStr(fProductionInfo->iControlBinCheckCount);
        }
        else
        {
            edControlBinCheckPoint->Text="Not use";
        }
    }
    else
    {
        edControlBinCheckPoint->Visible=false;
    }
    if(CUSTOMER_CODE==CC_JSSI_Semiconductor)                                    //Jimmychiu 20260120 : 客戶要求關閉全Bin關閉
    {
        btnSetAll2NotUse->Visible=false;
    }
    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);                                  //Steven 20100629
}
//---------------------------------------------------------------------------
void __fastcall TfBinSel::FormDestroy(TObject *Sender)
{
    try
    {
        MySleep(100);
        for(vector<TMyBinPanel *>::iterator iter=MyBinPanel.begin(); iter!=MyBinPanel.end(); ++iter)
        {
            delete *iter;
        }
        vec_clr(MyBinPanel);

        for(int i=eBinTypeTotal-1; i>=0; i--)                                   //Steven 20140510 : Secs Gem
        {
            if(sBinDoubleContact[i]!=NULL)                                      //Steven 20161220 (jou) : 修正delete方式
                delete sBinDoubleContact[i];
            if(sBinConsFail[i]!=NULL)
                delete sBinConsFail[i];
            if(sBinEnableFail[i]!=NULL)
                delete sBinEnableFail[i];
            if(sBinFailPercent[i]!=NULL)
                delete sBinFailPercent[i];

            if(sBinFailIgnore[i]!=NULL)                                         //Steven 20140529 Start: Fail Persent & Count同時存在
                delete sBinFailIgnore[i];
            if(sBinCountEnable[i]!=NULL)
                delete sBinCountEnable[i];
            if(sBinCountIgnore[i]!=NULL)
                delete sBinCountIgnore[i];
            if(sBinCountNumber[i]!=NULL)
                delete sBinCountNumber[i];

            if(sSpecialBinByArm[i]!=NULL)                                       //ChungHung 20140730 add ContinuousFailHaveOneCycle start
                delete sSpecialBinByArm[i];
            if(sSpecialBinCountByArm[i]!=NULL)
                delete sSpecialBinCountByArm[i];
            if(sSpecialBinBySocket[i]!=NULL)
                delete sSpecialBinBySocket[i];
            if(sSpecialBinCountBySocket[i]!=NULL)
                delete sSpecialBinCountBySocket[i];

            if(sLowYield[i]!=NULL)
                delete sLowYield[i];                                            //Steven 20140828 : By Bin Yield Monitor
            if(sArmYield[i]!=NULL)
                delete sArmYield[i];                                            //Steven 20140828 : By Bin Arm Yield Monitor
            if(sSiteYield[i]!=NULL)
                delete sSiteYield[i];                                           //Steven 20140828 : By Bin Site Yield Monitor

            if(sBinTraySetT3PosName[i]!=NULL)                                   //Steven 20241205 : Bin to tray增加文字顯示
                delete sBinTraySetT3PosName[i];
            if(sBinTraySetT3Pos[i]!=NULL)
                delete sBinTraySetT3Pos[i];
            if(sBinType[i]!=NULL)
                delete sBinType[i];
            if(sT3TrayType[i]!=NULL)
                delete sT3TrayType[i];
            if(sT6Retest[i]!=NULL)
                delete sT6Retest[i];
            if(sBySiteClean[i]!=NULL)
                delete sBySiteClean[i];                                         //Steven 20160308 : By Bin count do auto clean
            if(sByBinClean[i]!=NULL)
                delete sByBinClean[i];                                          //Steven 20160308 : By Bin pre site count do auto clean
            if(sT3CateR[i]!=NULL)
                delete sT3CateR[i];                                             //Steven 20161221 (wei) : for SCK only Auto 2 has ART

            if(sSpecBinBySiteCompareEnable[i]!=NULL)                            //JerryYang 20170712 (Steven) by site by bin compare percent
                delete sSpecBinBySiteCompareEnable[i];
            if(sSpecBinBySiteCompareIgnore[i]!=NULL)
                delete sSpecBinBySiteCompareIgnore[i];
            if(sSpecBinBySiteComparePercent[i]!=NULL)
                delete sSpecBinBySiteComparePercent[i];
            if(sSpecBinByArmPerSiteCompareEnable[i]!=NULL)
                delete sSpecBinByArmPerSiteCompareEnable[i];
            if(sSpecBinByArmPerSiteCompareIgnore[i]!=NULL)
                delete sSpecBinByArmPerSiteCompareIgnore[i];
            if(sSpecBinByArmPerSiteComparePercent[i]!=NULL)
                delete sSpecBinByArmPerSiteComparePercent[i];

            if(sBinTrayLinked[i]!=NULL)                                         //Ifor 20240730 add : BinTrayLinked
                delete sBinTrayLinked[i];
            if(sBinLinked[i]!=NULL)                                             //Ifor 20240730 add : BinTrayLinked
                delete sBinLinked[i];
            if(sMagazineSetup[i]!=NULL)                                         //Ifor 20240730 add : BinTrayLinked
                delete sMagazineSetup[i];
        }
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfBinSel::FormDestroy");
    }
    LogSoftwareOffTime("TfBinSel, FormDestroy");                                //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
void __fastcall TfBinSel::ReadParam()
{
    AnsiString szDir=GetRecipeFileName("Tester.Data");
    bUseContinueFail=ReadIniData(szDir, "Alarm", "SocketEnable", 0);
    if(bUseContinueFail!=true)
        bUseContinueFail=ReadIniData(szDir, "Alarm", "HeadEnable", 0);
    bUseFailRate=ReadIniData(szDir, "Alarm", "RateEnable", 0);
}
//---------------------------------------------------------------------------
void __fastcall TfBinSel::FormClose(TObject *Sender, TCloseAction &Action)
{
    bShow=false;                                                                //Steven 20090713
    ReadFile(false, false, "");                                                 //kevin 20160928
    fShowBinSelect->InitShowBinDigital();

    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);                                  //Steven 20100629
}
//---------------------------------------------------------------------------
void __fastcall TfBinSel::cbTestModeChange(TObject *Sender)
{
    bShow=false;

    ReadFile(false, false, "");                                                 //kevin 20160928
    if(cbTestMode->Text=="Re-Test")
    {
        tsNormal->TabVisible=false;
        tsRetest->TabVisible=true;
        tsOffline->TabVisible=false;
        tsArtFT->TabVisible=false;                                              //ChungHung 20141002 add for KYEC AutoRetest
        tsArtRT->TabVisible=false;                                              //ChungHung 20141002 add for KYEC AutoRetest
        tsMrtFT->TabVisible=false;                                              //Ifor 20170316 (wei) add MRT Mode
        tsMrtRT->TabVisible=false;                                              //Ifor 20170316 (wei) add MRT Mode
        PageControl1->ActivePage=tsRetest;
    }
    else if(cbTestMode->Text=="Off-Line")
    {
        tsNormal->TabVisible=false;
        tsRetest->TabVisible=false;
        tsOffline->TabVisible=true;
        tsArtFT->TabVisible=false;                                              //ChungHung 20141002 add for KYEC AutoRetest
        tsArtRT->TabVisible=false;                                              //ChungHung 20141002 add for KYEC AutoRetest
        tsMrtFT->TabVisible=false;                                              //Ifor 20170316 (wei) add MRT Mode
        tsMrtRT->TabVisible=false;                                              //Ifor 20170316 (wei) add MRT Mode
        PageControl1->ActivePage=tsOffline;
    }
    else if(cbTestMode->Text=="ART Re-Test")
    {
        tsNormal->TabVisible=false;
        tsRetest->TabVisible=false;
        tsOffline->TabVisible=false;
        tsArtFT->TabVisible=false;                                              //ChungHung 20141002 add for KYEC AutoRetest
        tsArtRT->TabVisible=true;                                               //ChungHung 20141002 add for KYEC AutoRetest
        tsMrtFT->TabVisible=false;                                              //Ifor 20170316 (wei) add MRT Mode
        tsMrtRT->TabVisible=false;                                              //Ifor 20170316 (wei) add MRT Mode
        PageControl1->ActivePage=tsArtRT;
    }
    else if(cbTestMode->Text=="ART Normal")
    {
        tsNormal->TabVisible=false;
        tsRetest->TabVisible=false;
        tsOffline->TabVisible=false;
        tsArtFT->TabVisible=true;                                               //ChungHung 20141002 add for KYEC AutoRetest
        tsArtRT->TabVisible=false;                                              //ChungHung 20141002 add for KYEC AutoRetest
        tsMrtFT->TabVisible=false;                                              //Ifor 20170316 (wei) add MRT Mode
        tsMrtRT->TabVisible=false;                                              //Ifor 20170316 (wei) add MRT Mode
        PageControl1->ActivePage=tsArtFT;
    }
    else if(cbTestMode->Text=="Normal")
    {
        tsNormal->TabVisible=true;
        tsRetest->TabVisible=false;
        tsOffline->TabVisible=false;
        tsArtFT->TabVisible=false;                                              //ChungHung 20141002 add for KYEC AutoRetest
        tsArtRT->TabVisible=false;                                              //ChungHung 20141002 add for KYEC AutoRetest
        tsMrtFT->TabVisible=false;                                              //Ifor 20170316 (wei) add MRT Mode
        tsMrtRT->TabVisible=false;                                              //Ifor 20170316 (wei) add MRT Mode
        PageControl1->ActivePage=tsNormal;
    }
    else if(cbTestMode->Text=="MRT Re-Test")
    {
        tsNormal->TabVisible=false;
        tsRetest->TabVisible=false;
        tsOffline->TabVisible=false;
        tsArtFT->TabVisible=false;                                              //ChungHung 20141002 add for KYEC AutoRetest
        tsArtRT->TabVisible=false;                                              //ChungHung 20141002 add for KYEC AutoRetest
        tsMrtFT->TabVisible=false;                                              //Ifor 20170316 (wei) add MRT Mode
        tsMrtRT->TabVisible=true;                                               //Ifor 20170316 (wei) add MRT Mode
        PageControl1->ActivePage=tsMrtRT;
    }
    else if(cbTestMode->Text=="MRT Normal")
    {
        tsNormal->TabVisible=false;
        tsRetest->TabVisible=false;
        tsOffline->TabVisible=false;
        tsArtFT->TabVisible=false;                                              //ChungHung 20141002 add for KYEC AutoRetest
        tsArtRT->TabVisible=false;                                              //ChungHung 20141002 add for KYEC AutoRetest
        tsMrtFT->TabVisible=true;                                               //Ifor 20170316 (wei) add MRT Mode
        tsMrtRT->TabVisible=false;                                              //Ifor 20170316 (wei) add MRT Mode
        PageControl1->ActivePage=tsMrtFT;
    }
    bShow=true;
}
//---------------------------------------------------------------------------
void TfBinSel::ChangeActivePageIndex()  //Sam 20230711 : 修正 OLP SetCategroy 異常
{
     if(iTestRunMode==FT)
        PageControl1->ActivePage=tsNormal;
    else if(iTestRunMode==RT)
        PageControl1->ActivePage=tsRetest;
    else if(iTestRunMode==OffT)
        PageControl1->ActivePage=tsOffline;
    else if(iTestRunMode==FT_ART)
        PageControl1->ActivePage=tsArtFT;
    else if(iTestRunMode==RT_ART)
        PageControl1->ActivePage=tsArtRT;
    else if(iTestRunMode==FT_MRT)
        PageControl1->ActivePage=tsMrtFT;
    else if(iTestRunMode==RT_MRT)
        PageControl1->ActivePage=tsMrtRT;
    else
        ShowMyMessage("ChangeActivePageIndex error", "ChangeActivePageIndex 錯誤");
}
//---------------------------------------------------------------------------
void __fastcall TfBinSel::spbSaveClick(TObject *Sender)
{
    if(IniConfig.bA02DisableSaveParsWhenSwitchToOp==true &&                     //RogerYang 20260305 張寧說時間到都要登出，且不可被更改任何設定(A01_2)
        AccessLevel==0)
    {
        ShowMyMessage("[A01_2]目前已切換到Operator權限，\r\n請重新登入再做設定!");
        Close();
    }

    AnsiString szDirTarget="", GroupStr;

    if(CheckFix2Tray())                                                         //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
    {
        ShowMyMessage("己開啟AOI功能，不能使用Fix2 Tray");
        return ;
    }

    Label1->Caption="Saving Data..";

    AnsiString szDir=GetRecipePath();
    SaveOther(szDir);
    #ifdef ASE_KaohSiung
       fBuilder->bSaveAsJobFile(GetLastOpenFN(), "JOBFILE");
    #endif

    Label1->Caption="Refresh data..";
    ReadWriteMRTMode(1);                                                        //寫入MRT Mode  //Ifor 20170417 (wei) add MRT Function 移至 BinSel 內 做開關切換
    ReadWriteSpecialFunction(true);                                             //JerryYang 20220923 : bOutShtLoseICSetErrUntilOneCycle改為可By工作檔
    ReadFile(false, false, "");                                                 //kevin 20160928

    if(CheckOSBin()==false)                                                     //Sam 20250115 : 新增 OSBIN 保護
    {
        ShowMyMessage("SetOSBin Category Error", IntToStr(Prod.iOpenBin));
    }

    if(bSaveBin==false)
    {
        bSaveBin=true;
        RecordProcess("Save Bin");
    }
    if(IniConfig.bEnable_SECS_GEM==true)
    {
        bHasSaveSet=true;                                                       //Ifor 20151204 新增判斷機台有無修改設定檔
    }
    fShowBinSelect->InitShowBinDigital();                                       //20111110  Dell
    palSpecificBin->Visible=false;                                              //JerryYang 20170301 (wei) 存檔後不顯示
    Label1->Caption="Save..OK";
    spbSave->Down=false;
    if(CosFunction.bUseMRTMode==true)                                           //Ifor 20170405 (wei) add 進入Setup 修改設定後要重新整理Start Mode
        fMain->SetStartModeData();
    fMain->BackupSetupFile();                                                   //Ifor 20170620 (wei) add Auto BackUp Setup File & Last Data

    if(IniConfig.bSIGURDFunction &&                                             //Sam 20210331 : 北興新增 Yield Monitor
       fLotInfo->pgLotinfo->ActivePage==fLotInfo->tsYieldMonitior)
    {
        fLotInfo->RefreshYieldMonitor();
    }
}
//------------------------------------------------------------------------------
void __fastcall TfBinSel::SetConFail(int iBinasgnMode)                          //ChungHung 20150519 add Auto Set ContFail After InitialStart.
{
    int tag, iTray;
    AnsiString GroupStr;
    AnsiString szDir=GetRecipePath();
    MyForceDirectories(szDir);

    if(iBinasgnMode==FT)                                                        //tsNormal
    {
        szDir=GetRecipeFileName("Binasgn.Data");
        tag=eBinFT;
    }
    else if(iBinasgnMode==RT)
    {
        szDir=GetRecipeFileName("BinasgnOff.Data");
        tag=eBinRT;
    }
    else if(iBinasgnMode==OffT)                                                 //ChungHung 20141002 add for KYEC AutoRetest  //ChungHung 20111110 add
    {
        szDir=GetRecipeFileName("BinasgnOff-Line.Data");
        tag=eBinOffLine;
    }
    else if(iBinasgnMode==FT_ART)                                               //ChungHung 20141002 add for KYEC AutoRetest
    {
        szDir=GetRecipeFileName("Binasgn_ART.Data");
        tag=eBinFT_ART;
    }
    else if(iBinasgnMode==RT_ART)                                               //ChungHung 20141002 add for KYEC AutoRetest
    {
        szDir=GetRecipeFileName("BinasgnOff_ART.Data");
        tag=eBinRT_ART;
    }
    else if(iBinasgnMode==FT_MRT)                                               //Ifor 20170316 (wei) add MRT Mode
    {
        szDir=GetRecipeFileName("Binasgn_MRT.Data");
        tag=eBinFT_MRT;
    }
    else if(iBinasgnMode==RT_MRT)                                               //Ifor 20170316 (wei) add MRT Mode
    {
        szDir=GetRecipeFileName("Binasgn_MRT_RT.Data");
        tag=eBinRT_MRT;
    }

    FormSysTools->OpenFormData(szDir);                                          //Steven 20140531 : 為了加快ini讀取速度

    bool bT3FailTray[e3TrayCount];
    for(int i=0; i<e3TrayCount; i++)
    {
        //Pass Fail-------
        bT3FailTray[i]=FormSysTools->CheckAndReadIniData(s3TrayName[i], "Pass/Fail", 0); //define stack store pass or fail
    }

    for(int i=0; i<iTestBinCount; i++)
    {
        GroupStr.sprintf("Category%d", i);                                      //kevin 20140317 256 bin  bin0 開始
        iTray=FormSysTools->CheckAndReadIniData(GroupStr, "Bin", (tag==OffT)?e3PosFix2:e3PosNoUse);
        if(iTray!=e3PosNoUse && bT3FailTray[iTray-1]==true)
            FormSysTools->WriteIniData(GroupStr, "Cons.Fail", 1);
    }

    FormSysTools->CloseFormData();                                              //Steven 20140531 : 為了加快ini讀取速度
}
//------------------------------------------------------------------------------
//  kevin 20150105 另存一個 jobfile
//------------------------------------------------------------------------------
void __fastcall TfBinSel::SaveOther(AnsiString szDir)
{
    int tag=0;
    AnsiString szDirTarget="", GroupStr, sBuffer="", sBuffer1="";
    AnsiString FileName="", FileName2="";
    int iResult;
    MyForceDirectories(szDir);

    if(PageControl1->ActivePageIndex==1)
    {
        tag=eBinRT;
        if(IniConfig.bA02BinModelPrime)                                         //Steven 20160623 : 當IniConfig.bFTBin2RTBin==true && 讀檔時, RT Bin要跟FT Bin一樣
        {
            if(IniConfig.bFTBin2RTBin==true && (iBinModelPrime==0 || CosFunction.bDisableRTBinSet))
            {
                FileName=FileInfo().PathCombin(szDir, "Binasgn.Data");
            }
            else
            {
                FileName=FileInfo().PathCombin(szDir, "BinasgnOff.Data");
            }
        }
        else
        {
            if(IniConfig.bFTBin2RTBin==true || CosFunction.bDisableRTBinSet)
            {
                FileName=FileInfo().PathCombin(szDir, "Binasgn.Data");
            }
            else
            {
                FileName=FileInfo().PathCombin(szDir, "BinasgnOff.Data");
            }
        }
    }
    else if(PageControl1->ActivePageIndex==2)                                   //ChungHung 20141002 add for KYEC AutoRetest  //ChungHung 20111110 add
    {
        FileName=FileInfo().PathCombin(szDir, "BinasgnOff-Line.Data");
        tag=eBinOffLine;
    }
    else if(PageControl1->ActivePageIndex==3)                                   //ChungHung 20141002 add for KYEC AutoRetest
    {
        FileName=FileInfo().PathCombin(szDir, "Binasgn_ART.Data");
        tag=eBinFT_ART;
    }
    else if(PageControl1->ActivePageIndex==4)                                   //ChungHung 20141002 add for KYEC AutoRetest
    {
        FileName=FileInfo().PathCombin(szDir, "BinasgnOff_ART.Data");
        tag=eBinRT_ART;
    }
    else if(PageControl1->ActivePageIndex==5)                                   //Ifor 20170316 (wei) add MRT Mode
    {
        FileName=FileInfo().PathCombin(szDir, "Binasgn_MRT.Data");
        tag=eBinFT_MRT;
    }
    else if(PageControl1->ActivePageIndex==6)                                   //Ifor 20170316 (wei) add MRT Mode
    {
        FileName=FileInfo().PathCombin(szDir, "Binasgn_MRT_RT.Data");
        tag=eBinRT_MRT;
    }
    else //if(PageControl1->ActivePageIndex==0)                                 //tsNormal
    {
        FileName=FileInfo().PathCombin(szDir, "Binasgn.Data");
        tag=eBinFT;
    }

    if(IniConfig.bP28Auto1OnlyBin1==true)                                       //Ifor 20171017 (wei) add for Unisem Auto1 Only Bin1
    {
        int iErrCode=0;
        TStringList *sT3List;
        sT3List = new TStringList();
        sT3List->CommaText=sBinTraySetT3Pos[tag]->CommaText;
        if(CUSTOMER_CODE==CC_UNISEM_M && LastSet.iTester==OFF_LINE)             //Ifor 20171213 (Steven) : add UNISEM_M 要求Offline 不卡 Auto1 Only Bin1 功能
        {

        }
        else
        {
            for(int i=0; i<iTestBinCount; i++)
            {
                if(i==1)
                {
                    if(sT3List->Strings[i]!=AnsiString("1"))
                    {
                        iErrCode=1;
                        break;
                    }
                }
                else if(sT3List->Strings[i]==AnsiString("1"))
                {
                    iErrCode=2;
                    break;
                }
            }
        }
        sT3List->Clear();
        delete sT3List;
        if(iErrCode==1)
        {
            ShowMyMessage("Bin1未被設定在Auto1 !! 請確認 !!\n Bin1 is not set to Auto1 !! Please Check !!");
            return;
        }
        else if(iErrCode==2)
        {
            ShowMyMessage("只有Bin 1被允許設為 Auto1 !! 請確認 !!\n Only Bin 1 is allowed to set to Auto 1!!! Please check !!");
            return;
        }
    }

    if(CosFunction.bUsePassBinOnlyCanSetOneBin==true)                           //Ifor 20171213 (Steven) : Auto Tray 設定Pass時僅可設定一個Bin
    {
        int iBinCt=0;
        AnsiString str1, str2;
        TStringList *sT3List;
        sT3List = new TStringList();
        sT3List->CommaText=sBinTraySetT3Pos[tag]->CommaText;
        for(int i=0; i<iAutoRight; i++)                                         //Auto1 ~ Auto3
        {
            if(MyBinPanel[tag]->iT6IsFail[i]==0)                                //判斷 Failed || Pass
            {
                iBinCt=0;
                for(int j=0; j<iTestBinCount; j++)
                {
                    if(sT3List->Strings[j]==AnsiString(iTo3Unload[i]+1))
                    {
                        iBinCt++;
                        if(iBinCt>1)
                        {
                            str1.sprintf("%s Pass Bin Only Can Set One Bin.", s6TrayName[i]);
                            str2.sprintf("%s Pass Bin 僅可設定一個Bin", s6TrayName[i]);
                            ShowMyMessage(str1, str2);
                            return;
                        }
                    }
                }
            }
        }
        sT3List->Clear();
        delete sT3List;
    }

    if(IniConfig.bI32CanCelErrorBin)
    {
        FileName2=FileInfo().PathCombin(szDir, "BinasgnOff.Data");
        FormSysTools->OpenFormData(FileName2);                                  //kevin 20160727: 為了加快ini讀取速度
        FormSysTools->WriteIniData("I/F Error", "CancelError",     CancelErrorBin->Checked);                    //kevin 20160724 儲存取消BIN 檔案 "\\Binasgn.Data";
        FormSysTools->CloseFormData();                                          //Steven 20140531 : 為了加快ini讀取速度
    }
    else if(IniConfig.bI33ErrorBinBox)                                          //kevin 20160909: 為了加快ini讀取速度
    {
        FileName2=FileInfo().PathCombin(szDir, "BinasgnOff.Data");
        FormSysTools->OpenFormData(FileName2);                                  //kevin 20160727: 為了加快ini讀取速度
        FormSysTools->WriteIniData("I/F Error", "Bin",             iTo3PosUnload[MyBinPanel[tag]->iErrorT6]);
        FormSysTools->WriteIniData("I/F Error", "Tray",            s6TrayName[MyBinPanel[tag]->iErrorT6]);      //Steven 20241205 : Bin to tray增加文字顯示
        FormSysTools->WriteIniData("I/F Error", "ErrorBinBox",     rg_FixBinBox->ItemIndex);                    //kevin 20160819 error bin 要放到 Bin Box "\\Binasgn.Data";
        if(rg_FixBinBox->ItemIndex==1)                                          //kevin 20200722 add 保護值
        {
            if(ed_FixBinBoxAlarmCount->Text.ToIntDef(0)<=2)                     //kevin 20200722 add 保護值
                ed_FixBinBoxAlarmCount->Text="2";
        }
        FormSysTools->WriteIniData("I/F Error", "Alarm Count",     ed_FixBinBoxAlarmCount->Text);               //kevin 20160819 error bin 要放到 Bin Box "\\Binasgn.Data";
        FormSysTools->CloseFormData();                                          //Steven 20140531 : 為了加快ini讀取速度
    }

    if(CosFunction.bAllSiteSameFailBinShowAlarm)                                //JerryYang 20160913 矽品要求當測試結果中所有site的bin都是所設定一樣的fail bin要跳alarm
    {
        FileName2=FileInfo().PathCombin(szDir, "BinasgnOff.Data");
        FormSysTools->OpenFormData(FileName2);                                  //kevin 20160727: 為了加快ini讀取速度

        for(int i=0; i<iTestBinCount; i++)                                      //kevin 20170828 (Steven) add   //JerryYang 20170221 (wei) 改成可複選bin
        {
            sBuffer.sprintf("%d,", sgSpecificBin->Cells[i][1]=="V");
            sBuffer1+=sBuffer;
        }

        FormSysTools->WriteIniData("AllSiteSameBin", "bSpecificFailBin", sBuffer1);                 //JerryYang 20160913 矽品要求當測試結果中所有site的bin都是所設定一樣的fail bin要跳alarm
        FormSysTools->CloseFormData();                                          //Steven 20140531 : 為了加快ini讀取速度

        if(CUSTOMER_CODE==CC_ASE_KaohSiung)
        {
            FileName2=FileInfo().PathCombin(szDir, "BinasgnOff.Data");
            FormSysTools->OpenFormData(FileName2);                              //kevin 20160727: 為了加快ini讀取速度
            FormSysTools->WriteIniData("I/F Error", "Bin",             iTo3PosUnload[MyBinPanel[tag]->iErrorT6]);
            FormSysTools->WriteIniData("I/F Error", "Tray",            s6TrayName[MyBinPanel[tag]->iErrorT6]);  //Steven 20241205 : Bin to tray增加文字顯示
            FormSysTools->WriteIniData("I/F Error", "ErrorBinBox",     rg_FixBinBox->ItemIndex);                //kevin 20160819 error bin 要放到 Bin Box "\\Binasgn.Data";
            if(rg_FixBinBox->ItemIndex==1)                                      //kevin 20200722 add 保護值
            {
                if(ed_FixBinBoxAlarmCount->Text.ToIntDef(0)<=2)                 //kevin 20200722 add 保護值
                    ed_FixBinBoxAlarmCount->Text="2";
            }
            FormSysTools->WriteIniData("I/F Error", "Alarm Count",     ed_FixBinBoxAlarmCount->Text);           //kevin 20220916 目前現階段只寫不讀  error bin 要放到 Bin Box "\\Binasgn.Data";
            FormSysTools->CloseFormData();                                      //Steven 20140531 : 為了加快ini讀取速度
        }
    }

    FormSysTools->OpenFormData(FileName);                                       //Steven 20140531 : 為了加快ini讀取速度
    if(bGPIBChangeAlarm)                                                        //wei 20151127 GPIB Change Alarm
    {
        for(int i=0; i<iTestBinCount; i++)                                       //Steven 20231002 :　eTrayCount -->　TEST_MAX_BIN  //Jimmychiu 20251114 : fix overflow
        {
            sBinConsFail[tag]->Strings[i]   =(BinSelect[tag].bConsFail[i])?"1":"0"; //Sam 20210715 : 修正 SetAlarmSetup 寫入異常。
            sBinEnableFail[tag]->Strings[i] =(BinSelect[tag].bFailure[i])?"1":"0";
            sBinFailPercent[tag]->Strings[i]=BinSelect[tag].dFailureLimit[i];   //Sam 20210715 : 修正 SetAlarmSetup 寫入異常。
        }
    }

    if(bTCPIPChangeAlarm)                                                       //Isaac 20170613 (wei) TCP/IP
    {
        for(int i=0; i<iTestBinCount; i++)
        {
            sBinDoubleContact[tag]->Strings[i]=BinSelect[tag].iDBContact[i];
        }
    }

    if(bSetBINCOUNT)
    {
        for(int i=0; i<iTestBinCount; i++)
        {
            sBinFailIgnore[tag]->Strings[i]=BinSelect[tag].iPersentIgnore[i];
        }
    }

    if(CUSTOMER_CODE==CC_Greatek)                                               //Sam 20200429 : IPSC Add Control Bin Alarm
    {
        if(bMOFileControlBinChangeAlarm)
        {
            for(int i=0; i<iTestBinCount; i++)
            {
                sSpecialBinBySocket[tag]->Strings[i]     =(BinSelect[tag].bSpecialBinBySocket[i])?"1":"0";
                sSpecialBinCountBySocket[tag]->Strings[i]=IntToStr(BinSelect[tag].iSpecialBinCountBySocket[i]);
            }
        }
    }

    if(bOLPSetCategroy)                                                         //Sam 20230711 : 修正 OLP SetCategroy 異常
    {
        MyBinPanel[tag]->iErrorT6=iTo6Unload[BinSelect[iTestRunMode].IfErrorT3];
        for(int i=0; i<iTestBinCount; i++)
            sBinTraySetT3Pos[tag]->Strings[i]=BinSelect[tag].iCatDataT3Pos[i];

        for(int i=0; i<eTrayCount; i++)
            MyBinPanel[tag]->iT6IsFail[i]=Prod.iIsFailT6[i];
    }

    SaveFunctionData(tag);

    if(bSetByDLL==true)                                                         //JerryYang 20181126 (Steven) : support Epson DLL function
    {
        MyBinPanel[tag]->iErrorT6=iTo6Unload[BinSelect[iTestRunMode].IfErrorT3];
    }

    if(CUSTOMER_CODE==CC_TFME_CHINA)                                            //Ifor 20251029 add:通富微電子要求I/F Error BIN存檔與Category相同
    {
        if(MyBinPanel[tag]->iErrorT6==2)
        {
            FormSysTools->WriteIniData("I/F Error", "Bin", iTo3Unload[6]);      //JerryYang 20230926
        }
        else if(MyBinPanel[tag]->iErrorT6==11)
        {
            FormSysTools->WriteIniData("I/F Error", "Bin", iTo3Unload[18]);     //JerryYang 20230926
        }
        else
        {
            FormSysTools->WriteIniData("I/F Error", "Bin", iTo3Unload[MyBinPanel[tag]->iErrorT6+1]);      //JerryYang 20230926
        }
    }
    else
    {
        FormSysTools->WriteIniData("I/F Error", "Bin",             iTo3Unload[MyBinPanel[tag]->iErrorT6]);      //JerryYang 20230926
    }
    FormSysTools->WriteIniData("I/F Error", "Tray",            s6TrayName[MyBinPanel[tag]->iErrorT6]);      //Steven 20241205 : Bin to tray增加文字顯示
    FormSysTools->WriteIniData("I/F Error", "Contact",         0);
    FormSysTools->WriteIniData("I/F Error", "Cons.Fail",       0);
    FormSysTools->WriteIniData("I/F Error", "Fail Percent",    0);
    FormSysTools->WriteIniData("I/F Error", "Scan",            0);
    FormSysTools->WriteIniData("I/F Error", "Fail Count",      0);              //ChungHung 20120724 add BinFailCount

    for(int i=0; i<iTestBinCount; i++)                                          //Steven 20160614 : 避免降版導致分Bin異常, 將資料回寫到舊的
    {
        GroupStr.sprintf("Category%d", i);                                      //kevin 20140317 256 bin  bin0 開始
        for(int j=eBinNotUse; j<eBinSetTotal; j++)
        {
            if(MyBinPanel[tag]->BackT6PosTray[i][j])
            {
                FormSysTools->WriteIniData(GroupStr, "Bin", iTo3PosUnload[j-eBinNotUse]);
                if(j==eBinNotUse)
                    FormSysTools->WriteIniData(GroupStr, "Tray", "NotUse");
                else
                {
                    if(j==32)                                                   //Ifor 20251029 Fix:Bin to tray Fix1 顯示變成 Fix7
                    {
                        FormSysTools->WriteIniData(GroupStr, "Tray", s3TrayName[3]);       //Steven 20241205 : Bin to tray增加文字顯示
                    }
                    else
                    {
                        FormSysTools->WriteIniData(GroupStr, "Tray", s3TrayName[iTo3PosUnload[j-eBinNotUse-1]]);       //Steven 20241205 : Bin to tray增加文字顯示
                    }
                }
                break;
            }
        }

        iResult=MyBinPanel[tag]->i2Contact[i];                                  //Steven 20231205 : b2Contact --> i2Contact
        if(Prod.bD22SupportMultiDoubleContact==false)
           //IniConfig.bD22VerifyMode==false)                                     //Sam 20231117 : 整合到 QA 模式 //Sam 20221012 : 新增 VerifyMode 功能
        {
            iResult=0;
        }
        FormSysTools->WriteIniData(GroupStr, "Contact", iResult);

        if(bGPIBChangeAlarm)                                                    //wei 20151127 GPIB Change Alarm
        {
            MyBinPanel[tag]->bConFail[i]=BinSelect[iTestRunMode].bFailure[i];
            MyBinPanel[tag]->bPersentEnable[i]=BinSelect[iTestRunMode].bFailure[i];
            MyBinPanel[tag]->dPersentNumber[i]=Prod.dFailureLimit[i];
        }

        iResult=(MyBinPanel[tag]->bConFail[i])?1:0;
        FormSysTools->WriteIniData(GroupStr, "Cons.Fail", iResult);

        iResult=(MyBinPanel[tag]->bPersentEnable[i])?1:0;                       //Steven 20140529 Start: Fail Persent & Count同時存在
        FormSysTools->WriteIniData(GroupStr, "Fail Percent",        iResult);
        FormSysTools->WriteIniData(GroupStr, "Fail Percent Ignore", MyBinPanel[tag]->iPersentIgnore[i]);
        FormSysTools->WriteIniData(GroupStr, "Fail Percent Limit",  MyBinPanel[tag]->dPersentNumber[i]);

        iResult=(MyBinPanel[tag]->bCountEnable[i])?1:0;
        FormSysTools->WriteIniData(GroupStr, "Fail Count",        iResult);
        FormSysTools->WriteIniData(GroupStr, "Fail Count Ignore", MyBinPanel[tag]->iCountIgnore[i]);
        FormSysTools->WriteIniData(GroupStr, "Fail Count Limit",  MyBinPanel[tag]->iCountNumber[i]);

        iResult=(MyBinPanel[tag]->bSpecialBinByArm[i])?1:0;                     //ChungHung 20140730 add ContinuousFailHaveOneCycle start
        FormSysTools->WriteIniData(GroupStr, "Special Bin By Arm",        iResult);
        FormSysTools->WriteIniData(GroupStr, "Special Bin Count By Arm", MyBinPanel[tag]->iSpecialBinCountByArm[i]);
        iResult=(MyBinPanel[tag]->bSpecialBinBySocket[i])?1:0;
        FormSysTools->WriteIniData(GroupStr, "Special Bin By Socket",        iResult);
        FormSysTools->WriteIniData(GroupStr, "Special Bin Count By Socket",  MyBinPanel[tag]->iSpecialBinCountBySocket[i]);

        FormSysTools->WriteIniData(GroupStr, "By Bin Low Yield",                  MyBinPanel[tag]->bLowYield[i]);     //Steven 20140828 : By Bin Yield Monitor
        FormSysTools->WriteIniData(GroupStr, "By Bin and Site Compare Arm Yield", MyBinPanel[tag]->bArmYield[i]);     //Steven 20140828 : By Bin Arm Yield Monitor
        FormSysTools->WriteIniData(GroupStr, "By Bin Compare Site Yield",         MyBinPanel[tag]->bSiteYield[i]);    //Steven 20140828 : By Bin Site Yield Monitor

        iResult=(MyBinPanel[tag]->bScan[i])?1:0;
        FormSysTools->WriteIniData(GroupStr, "Scan", iResult);
    }

    if(bGPIBChangeAlarm ||                                                      //wei 20151127 GPIB Change Alarm
       bSetByDLL)                                                               //JerryYang 20181126 (Steven) : support Epson DLL function
    {
        for(int i=0; i<eTrayCount; i++)
            MyBinPanel[tag]->iT6IsFail[i]=BinSelect[iTestRunMode].iStackDefFailCate[iTo3Unload[i]]; //Sam 20250303 : 修正 RMS 異常
    }

    for(int i=0; i<eTrayCount; i++)
    {
        FormSysTools->WriteIniData(s6TrayName[i], "Pass/Fail", MyBinPanel[tag]->iT6IsFail[i]);
        FormSysTools->WriteIniData(s6TrayName[i], "Link",      MyBinPanel[tag]->bT6Link[i]);
    }

    if(USE_AUTO_RETEST==eartInstall || IniConfig.bA38_SLT_Summary)              //ChungHung 20140317 add Auto Retest  //JerryYang 20220923 : add for SLT lot summary
    {
        for(int i=eAuto1; i<=iAutoRight; i++)
        {
            if(UNLOADER_ART[i]==eartInstall)                                    //Steven 20161221 (wei) : for SCK only Auto 2 has ART
                FormSysTools->WriteIniData(s6TrayName[i], "AutoRetest", MyBinPanel[tag]->bT6ART[i]);
            else
                FormSysTools->WriteIniData(s6TrayName[i], "AutoRetest", false);
        }

        if(USE_AUTO_RETEST==eartInstall &&                                      //Steven 20161221 (wei) : Cate R for ART
           CosFunction.bUseSCKART &&
           TestIF_File.iSCKART_SortMode==1 &&
           (tag==FT || tag==OffT) || IniConfig.bA38_SLT_Summary)                //Steven 20170317 (wei) : Off T也要可以ART  //JerryYang 20220923 : add for SLT lot summary
        {
            for(int i=0; i<eTrayCount; i++)
            {
                FormSysTools->WriteIniData(s6TrayName[i], "Cate R", MyBinPanel[tag]->bT6CateR[i]);
            }
        }
        else
        {
            for(int i=0; i<eTrayCount; i++)
            {
                FormSysTools->WriteIniData(s6TrayName[i], "Cate R", false);
            }
        }
    }
    else
    {
        for(int i=eAuto1; i<=iAutoRight; i++)
        {
            FormSysTools->WriteIniData(s6TrayName[i], "AutoRetest", false);
        }
    }
    FormSysTools->CloseFormData();                                              //Steven 20140531 : 為了加快ini讀取速度

    FileName2=FileInfo().PathCombin(szDir, "Binasgn.Data");
    FormSysTools->OpenFormData(FileName2);
    FormSysTools->WriteIniData("BinModel", "bPrime", iBinModelPrime);           //ChungHung 20120912 add Bin Setting like Epson

    if(CosFunction.bShow0Xbin)                                                  //jou 20220719 : show 0X bin
    {
        FormSysTools->WriteIniData("Other", "bShow0XBin", chkShow0Xbin->Checked);
    }
    FormSysTools->CloseFormData();                                              //Steven 20140531 : 為了加快ini讀取速度

    AddSpace(szDir);
}
//---------------------------------------------------------------------------
void __fastcall TfBinSel::sbtExitClick(TObject *Sender)
{
    sbtExit->Down=false;
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfBinSel::spbNormalClick(TObject *Sender)
{
    if(iBinModelPrime==0)
        return;

    int ret=ShowMyMessageBox_YES_NO("Change Binning to Normal mode?", "");
    if(ret==1)
    {
        WritePrimeDara(0);
    }
    else
    {
        SetPrimeButton();                                                       //Steven 20160624 : 重新顯示按鈕
        return;
    }

    ReadFile(false, false, "");                                                 //kevin 20160928
    tsRetest->Enabled=false;
    cbTestMode->Enabled=false;
    SetPrimeButton();                                                           //Steven 20160624 : 重新顯示按鈕
}
//---------------------------------------------------------------------------
void __fastcall TfBinSel::spbPrimeClick(TObject *Sender)
{
    if(iBinModelPrime==1)
        return;

    int ret=ShowMyMessageBox_YES_NO("Change Binning to Prime mode?", "");
    if(ret==1)
    {
        WritePrimeDara(1);
    }
    else
    {
        SetPrimeButton();                                                       //Steven 20160624 : 重新顯示按鈕
        return;
    }

    ReadFile(false, false, "");                                                 //kevin 20160928
    tsRetest->Enabled=true;
    cbTestMode->Enabled=true;
    SetPrimeButton();                                                           //Steven 20160624 : 重新顯示按鈕
}
//---------------------------------------------------------------------------
bool TfBinSel::bCheckTrayCanUse(int iT6Tray)
{
    if(iT6Tray<0 || iT6Tray>=eTrayCount)                                        //保護避免超出陣列
        return false;

    if(Prod.iTrayType[iT6Tray]==tNotUse)
        return false;

    if(CosFunction.bLoaderTrayToAuto1 && TrayForm.LoaderToEmptyColor[iRunStartMode]==2)
    {
        if(iT6Tray==eAuto1)
            return false;
    }

    if(AUTO3_IS_MAGAZINE==1)                                                    //JerryYang 20221215 : add Magazine
    {
        if(iT6Tray==iMagAtAuto)
            return false;

        if(TestIF_File.iMagFixTrayType==1)                                      //使用FIX當buffer
        {
            if(iT6Tray>=iFixMin && iT6Tray<=iFixRightHalf)
                return false;
        }
    }

    if(TestIF_File.bEnableQASampling &&                                         //Steven 20190326 : QA Sampling
       (iT6Tray-1)==TestIF_File.iQASamplingT3Pos)
    {
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------
void __fastcall TfBinSel::mtTrayNameMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TTMyTray *Ptr;
    Ptr=(TTMyTray *)Sender;
    int tag=Ptr->Tag;

    MyBinPanel[tag]->mtTrayName->ConvertIndexCells(X, Y);

    Y=Y-eBinSetting;

    if(X<0 || X>=eItemTotal || Y<0 || Y>=eTrayCount)                            //保護避免超出陣列
        return;

    if(bCheckTrayCanUse(Y)==false)
        return;

    if(X==eItemPass)
    {
        if(MyBinPanel[tag]->bT6Link[Y]==false)
        {
            if(MyBinPanel[tag]->iErrorT6==Y)
            {
                MyBinPanel[tag]->iT6IsFail[Y]=1;
            }
            else
            {
                if(IniConfig.bG07MultiColorForFailBin)                          //Steven 20160310 : 改成有顏色的fail bin
                {
                    MyBinPanel[tag]->iT6IsFail[Y]++;
                    if(MyBinPanel[tag]->iT6IsFail[Y]>=5)                        //最多五種顏色
                        MyBinPanel[tag]->iT6IsFail[Y]=0;
                }
                else
                {
                    MyBinPanel[tag]->iT6IsFail[Y]=(MyBinPanel[tag]->iT6IsFail[Y]==0)?1:0;
                }
            }
        }
    }
    else if(X==eItemLink)
    {
        if(bCanLinkT6[Y]==true)
        {
            MyBinPanel[tag]->bT6Link[Y]=!MyBinPanel[tag]->bT6Link[Y];
            if(MyBinPanel[tag]->bT6Link[Y] && MyBinPanel[tag]->iErrorT6==Y && Y>0)
            {
                MyBinPanel[tag]->iErrorT6=Y-1;
                MyBinPanel[tag]->iT6IsFail[Y-1]=MyBinPanel[tag]->iT6IsFail[Y];
            }

            for(int i=0; i<iTestBinCount; i++)
            {
                if(MyBinPanel[tag]->bT6Link[Y] && MyBinPanel[tag]->BackT6PosTray[i][Y+eBinSetting])     //有Link的不顯示
                {
                    MyBinPanel[tag]->BackT6PosTray[i][eBinNotUse]    =1;
                    MyBinPanel[tag]->BackT6PosTray[i][Y+eBinSetting] =0;
                }
            }
        }
    }
    else if(X==eItemError)
    {
        if(MyBinPanel[tag]->bT6Link[Y]==false)
        {
            MyBinPanel[tag]->iT6IsFail[Y]=1;
            MyBinPanel[tag]->iErrorT6=Y;
        }
    }
    else if(X==eItemART)                                                        //ChungHung 20140317 add Auto Retest
    {
        if(USE_AUTO_RETEST==eartInstall)
        {
            if(Y<=iAutoRight && MyBinPanel[tag]->iT6IsFail[Y]>0)                //ChungHung 20140630 add
            {
                if(UNLOADER_ART[Y]==eartInstall)                                //Steven 20161221 (wei) : for SCK only Auto 2 has ART
                {
                    MyBinPanel[tag]->bT6ART[Y]=!MyBinPanel[tag]->bT6ART[Y];
                }
                else
                {
                    MyBinPanel[tag]->bT6ART[Y]=false;
                }
            }
        }
    }
    else if(X==eItemCateR)                                                      //Steven 20161221 (wei) : Cate R for ART
    {
        if((USE_AUTO_RETEST==eartInstall &&
            CosFunction.bUseSCKART &&
            TestIF_File.iSCKART_SortMode==1) ||
           IniConfig.bA38_SLT_Summary)                                          //JerryYang 20220923 : add for SLT lot summary
        {
            if(MyBinPanel[tag]->iT6IsFail[Y]>0)                                 //ChungHung 20140630 add
            {
                MyBinPanel[tag]->bT6CateR[Y]=!MyBinPanel[tag]->bT6CateR[Y];
            }
            else
            {
                MyBinPanel[tag]->bT6CateR[Y]=false;
            }
        }
        else
        {
            MyBinPanel[tag]->bT6CateR[Y]=false;
        }
    }

    for(int i=0; i<MyBinPanel[tag]->mtBinSelect->XItem; i++)                    //ChungHung 20140414 選擇fail bin 預設 continue fial 為 true
    {
        if(MyBinPanel[tag]->BackT6PosTray[i][eBinSetting+Y]==1)
        {
            if(MyBinPanel[tag]->iT6IsFail[Y]>0)
                MyBinPanel[tag]->bConFail[i]=true;
            else
                MyBinPanel[tag]->bConFail[i]=false;
        }
    }

    for(int i=eAuto2; i<eTrayCount; i++)
    {
        if(bCanLinkT6[i]==true &&
           MyBinPanel[tag]->bT6Link[i])
        {
            MyBinPanel[tag]->iT6IsFail[i]=MyBinPanel[tag]->iT6IsFail[i-1];
        }
    }

    mtTrayNameSetColor(tag);
}
//---------------------------------------------------------------------------
void __fastcall TfBinSel::mtBinSelectMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TTMyTray *Ptr;
    Ptr=(TTMyTray *)Sender;
    int tag=Ptr->Tag;

#ifndef SOFT_SIMULTE
    if(tag==OffT &&
       CUSTOMER_CODE==CC_SCC &&                                                 //jou 2014-04-21 start : SCC要求加上password防護
       bCheckOffLineLevel==true)
    {
        AnsiString asOffLinePassword=CheckAndReadIniDataGeneral("VENDER", "Off-Line", AnsiString("HonPrec"));

        fPassword->edPassword->Clear();                                         //kevin  20120914 : 避免密碼未清空
        fQwertyKey->ShowQwertyKey(fPassword->edPassword, N_NO_SYMBOL|N_NO_SPACE|N_PASSWORD);    //Steven 20120904 : 改用QWERTY鍵盤
        if(fPassword->edPassword->Text!=asOffLinePassword)
        {
            ShowErrorMessage("WAR1676", 0, MMSystem);                           //Insufficient privileges
            return;
        }
        bCheckOffLineLevel=false;
        return;
    }
#endif

    MyBinPanel[tag]->mtBinSelect->ConvertIndexCells(X, Y);

    if(CosFunction.bOneCycleCanChangeContinuesFailBin)                          //ChungHung 20150519 add Continues Failure BIN can select after "OneCycle".
    {
        if(labWarning->Caption!="" &&
           (CUSTOMER_CODE==CC_JCET ||
            CUSTOMER_CODE==CC_TERAPOWER))                                       //Sam 20221020 : 晶兆成建勳也要 OneCycle 後可以編輯
        {
        }
        else if(labWarning->Caption!="" &&
                CUSTOMER_CODE!=CC_SIGURD_PeiXing)                               //Alick 20161207 add for 北興不判斷labWarning->Caption(不論AUTO1~3 & FIX1~3是否有IC)可以修改FailBin的相關Alarm設定
        {
            if(Y!=eConsFail)
                return;
        }
    }

    if(X<0 || X>=MyBinPanel[tag]->mtBinSelect->XItem)
    {
        return;
    }

    if(Y>=eBinSetting && bCheckTrayCanUse(Y-eBinSetting)==false)
        return;

    if(Y-eBinSetting==eBulkBox)                                                 //JerryYang 20220909 : bin box卡控
    {
        return;
    }

    if(CosFunction.bBySiteByBinPercentCompare==false ||                         //JerryYang 20170712 (Steven) by site by bin compare percent
       (CosFunction.bBySiteByBinPercentCompare==true &&
        CosFunction.bByBinAlarmFromYieldForm))                                  //jou 20180113 (Steven) : By Site By Bin Percent Compare From Yield form
    {
        if(Y>=eSpecBinBySiteCompareEnable && Y<(eSpecBinBySiteCompareEnable+6))
            return;
    }

    if(CosFunction.bByBinAlarmFromYieldForm)                                    //jou 20180113 (Steven) : By Site By Bin Percent Compare From Yield form
    {
        if(Y>=ePersentEnable && Y<(ePersentNumber))
            return;
    }

    if(CosFunction.bUseYieldControlFunction)                                    //Ifor 20180731 (wei) : add Use Yield Control Function
    {
        if(IniConfig.bKoreaFunction)
        {
            if(Y>0 && Y<eBinNotUse && fSecurity->Insufficient(148)==false)
            {
                return;
            }
            else if(Y>=eBinNotUse && fSecurity->Insufficient(149)==false)
            {
                return;
            }
        }
        else
        {
            if(Y>0 && Y<eBinNotUse && fSecurity->Insufficient(148)==false)
            {
                return;
            }
        }
    }

    if(CUSTOMER_CODE==CC_SCK && IniConfig.bEnableRms)                           //Steven 20210616 : JSCK RMS enabled, can not changed "Site map, temperature & Bin map"
    {
        if(Y>=eBinNotUse)
            return;
    }

    if(Y>0 && Y<eBinNotUse)
    {
        if(CosFunction.bOneCycleCanChangeContinuesFailBin)                      //kevin 20150525
        {
            if(InArmSuck.HasIC()  ||
               OutArmSuck.HasIC() ||
               ShuttleHasIC()     ||
               IndexHasIC())
                return;                                                         //kevin 20150519 有IC不能改變資料
        }

        if(Y==eScanning)
        {
//            if(LastSet.iScanner)
//                MyBinPanel[tag]->bScan[X]=!MyBinPanel[tag]->bScan[X];
        }
        else if(Y==eDoubleContact)
        {
            if(SeteDoubleContact(X, tag)==false)
            {
                return;
            }
        }
        else if(Y==eConsFail)                                                   //ChungHung 20140414 add 必須為fail bin 才能選
        {
            if(SeteConsFail(X, tag)==false)
            {
                return;
            }
        }
        else if(Y==ePersentEnable)                                              //Steven 20140529 Start: Fail Persent & Count同時存在
        {
            if(SetePersentEnable(X, tag)==false)
            {
                return;
            }
        }
        else if(Y==ePersentIgnore)
        {
            if(SetePersentIgnore(X, tag)==false)
            {
                return;
            }
        }
        else if(Y==ePersentNumber)
        {
            if(SetePersentNumber(X, tag)==false)
            {
                return;
            }
        }
        else if(Y==eCountEnable)
        {
            if(SeteCountEnable(X, tag)==false)
            {
                return;
            }
        }
        else if(Y==eCountIgnore)
        {
            if(SeteCountIgnore(X, tag)==false)
            {
                return;
            }
        }
        else if(Y==eCountNumber)
        {
            if(SeteCountNumber(X, tag)==false)
            {
                return;
            }
        }
        else if(Y==eSpecialBinByArm)                                            //ChungHung 20140730 add ContinuousFailHaveOneCycle start
        {
            if(SeteSpecialBinByArm(X, tag)==false)
            {
                return;
            }
        }
        else if(Y==eSpecialBinCountByArm)
        {
            if(SeteSpecialBinCountByArm(X, tag)==false)
            {
                return;
            }
        }
        else if(Y==eSpecialBinBySocket)
        {
            if(SeteSpecialBinBySocket(X, tag)==false)
            {
                return;
            }
        }
        else if(Y==eSpecialBinCountBySocket)
        {
            if(SeteSpecialBinCountBySocket(X, tag)==false)
            {
                return;
            }
        }
        else if(Y==eLowYield)                                                   //Steven 20140828 : By Bin Yield Monitor
        {
            if(SeteLowYield(X, tag)==false)
            {
                return;
            }
        }
        else if(Y==eArmYield)                                                   //Steven 20140828 : By Bin Arm Yield Monitor
        {
            if(SeteArmYield(X, tag)==false)
            {
                return;
            }
        }
        else if(Y==eSiteYield)                                                  //Steven 20140828 : By Bin Site Yield Monitor
        {
            if(SeteSiteYield(X, tag)==false)
            {
                return;
            }
        }
        else if(Y==eAutoCleanByBin)                                             //Steven 20160308 : By Bin count do auto clean
        {
            if(SeteAutoCleanByBin(X, tag)==false)
            {
                return;
            }
        }
        else if(Y==eAutoCleanBySite)                                            //Steven 20160308 : By Bin pre site count do auto clean
        {
            if(SeteAutoCleanBySite(X, tag)==false)
            {
                return;
            }
        }
        else if(Y==eSpecBinBySiteCompareEnable)                                 //JerryYang 20170712 (Steven) by site by bin compare percent
        {
            if(SeteSpecBinBySiteCompareEnable(X, tag)==false)
            {
                return;
            }
        }
        else if(Y==eSpecBinBySiteCompareIgnore)
        {
            if(SeteSpecBinBySiteCompareIgnore(X, tag)==false)
            {
                return;
            }
        }
        else if(Y==eSpecBinBySiteComparePercent)
        {
            if(SeteSpecBinBySiteComparePercent(X, tag)==false)
            {
                return;
            }
        }
        else if(Y==eSpecBinByArmPerSiteCompareEnable)                           //JerryYang 20170712 QQ
        {
            if(SeteSpecBinByArmPerSiteCompareEnable(X, tag)==false)
            {
                return;
            }
        }
        else if(Y==eSpecBinByArmPerSiteCompareIgnore)
        {
            if(SeteSpecBinByArmPerSiteCompareIgnore(X, tag)==false)
            {
                return;
            }
        }
        else if(Y==eSpecBinByArmPerSiteComparePercent)
        {
            if(SeteSpecBinByArmPerSiteComparePercent(X, tag)==false)
            {
                return;
            }
        }
        InitDataToEdit(tag);
    }
    //上下兩段不可以顛倒!!-------

    if(Y<eBinNotUse || Y>=MyBinPanel[tag]->mtBinSelect->YItem)
        return;

    if(Y>eBinSetting)
    {
        if(MyBinPanel[tag]->bT6Link[Y-eBinSetting])                             //有Link的不能按
            return;
    }

    if(CosFunction.bOneCycleCanChangeContinuesFailBin)                          //kevin 20150525 可以修改 fail count
    {
        if(CanChangeData(true))                                                 //kevin 20150523 機台沒
        {
            bMouseDown=true;
            iStartX=X;
            iStartY=Y;
            iEndX=X;
            iEndY=Y;
        }
    }
    else
    {
        bMouseDown=true;
        iStartX=X;
        iStartY=Y;
        iEndX=X;
        iEndY=Y;
    }
}
//---------------------------------------------------------------------------
bool TfBinSel::SeteDoubleContact(int X, int tag)
{
    if(CUSTOMER_CODE==CC_ASE_CL)                                                //JerryYang 20170303 (wei) Double Contact權限
    {
        if(fSecurity->Insufficient(132)==false)
        {
            return false;
        }
        else
        {
            if(Prod.bD22SupportMultiDoubleContact==true)                        //Steven 20170920 : 重新整理Double Contact選項
               //IniConfig.bD22VerifyMode)                                      //Sam 20231117 : 整合到 QA 模式 //Sam 20221012 : 新增 VerifyMode 功能
            {
                fQwertyKey->ShowQwertyKey(edInput, N_INTEGER, 0, true, 0, IniConfig.iD22DoubleContactCount+2);
                if(X>=0)
                {
                    MyBinPanel[tag]->i2Contact[X]=atoi(edInput->Text.c_str());  //Steven 20231205 : b2Contact --> i2Contact
                }
                else
                {
                    int input=atoi(edInput->Text.c_str());
                    for(int i=0; i<iTestBinCount; i++)
                    {
                        MyBinPanel[tag]->i2Contact[i]=input;
                    }
                }
            }
        }
    }
    else
    {
        if(Prod.bD22SupportMultiDoubleContact==true)                            //Steven 20170920 : 重新整理Double Contact選項
           //IniConfig.bD22VerifyMode)                                          //Sam 20231117 : 整合到 QA 模式 //Sam 20221012 : 新增 VerifyMode 功能
        {
            fQwertyKey->ShowQwertyKey(edInput, N_INTEGER, 0, true, 0, IniConfig.iD22DoubleContactCount+2);
            if(X>=0)
            {
                MyBinPanel[tag]->i2Contact[X]=atoi(edInput->Text.c_str());      //Steven 20231205 : b2Contact --> i2Contact
            }
            else
            {
                int input=atoi(edInput->Text.c_str());
                for(int i=0; i<iTestBinCount; i++)
                {
                    MyBinPanel[tag]->i2Contact[i]=input;
                }
            }
        }
    }

    return true;
}
//------------------------------------------------------------------------------
bool TfBinSel::SeteConsFail(int X, int tag)
{
    static bool bEnable=true;
    if(X>=0)
    {
        for(int j=eBinSetting; j<eBinSetTotal; j++)
        {
            if(MyBinPanel[tag]->BackT6PosTray[X][j]==1)
            {
                if(MyBinPanel[tag]->iT6IsFail[j-eBinSetting]>0)
                {
                    if(CUSTOMER_CODE==CC_KYEC_LEE &&                            //wei 20160418 強制打開continue fial
                       bEnablePEModel==false)                                   //Ifor 20160825 add PE模式可修改
                        MyBinPanel[tag]->bConFail[X]=true;
                    else
                        MyBinPanel[tag]->bConFail[X]=!MyBinPanel[tag]->bConFail[X];
                }
                else
                {
                    MyBinPanel[tag]->bConFail[X]=false;
                }
            }
        }
    }
    else
    {
        for(int i=0; i<iTestBinCount; i++)
        {
            for(int j=eBinSetting; j<eBinSetTotal; j++)
            {
                if(MyBinPanel[tag]->BackT6PosTray[i][j]==1)
                {
                    if(MyBinPanel[tag]->iT6IsFail[j-eBinSetting]>0)
                    {
                        MyBinPanel[tag]->bConFail[i]=bEnable;
                    }
                    else
                    {
                        MyBinPanel[tag]->bConFail[i]=false;
                    }
                }
            }
        }
        bEnable=false;
    }

    return true;
}
//------------------------------------------------------------------------------
bool TfBinSel::SetePersentEnable(int X, int tag)
{
    static bool bEnable=true;
    if(X>=0)
    {
        for(int j=eBinSetting; j<eBinSetTotal; j++)                             //ChungHung 20140414 add 必須為fail bin 才能選
        {
            if(MyBinPanel[tag]->BackT6PosTray[X][j]==1)
            {
                MyBinPanel[tag]->bPersentEnable[X]=!MyBinPanel[tag]->bPersentEnable[X];
            }
        }

        if(MyBinPanel[tag]->bPersentEnable[X])
        {
            MyBinPanel[tag]->bCountEnable[X]=false;
            edInput->Text=AnsiString(BinSelect[tag].dFailureLimit[X]);          //Alick 20170102 MyBinPanel[tag]->dPersentNumber[X]改成BinSelect[tag].dFailureLimit[X] 保留上次的設定值
            fQwertyKey->ShowQwertyKey(edInput, N_DOUBLE, 2, true, 100.0, 0.0);
            MyBinPanel[tag]->dPersentNumber[X]=atof(edInput->Text.c_str());
            if(MyBinPanel[tag]->iPersentIgnore[X]==0)
            {
                MyBinPanel[tag]->iPersentIgnore[X]=1;                           //JerryYang 20160728 修改預設ignore顆數0->1
            }
        }
        else
        {
            MyBinPanel[tag]->dPersentNumber[X]=0.0;
        }
    }
    else
    {
        for(int i=0; i<iTestBinCount; i++)
        {
            for(int j=eBinSetting; j<eBinSetTotal; j++)                         //ChungHung 20140414 add 必須為fail bin 才能選
            {
                if(MyBinPanel[tag]->BackT6PosTray[i][j]==1)
                {
                    MyBinPanel[tag]->bPersentEnable[i]=bEnable;
                }
            }
        }

        if(bEnable)
        {
            fQwertyKey->ShowQwertyKey(edInput, N_DOUBLE, 2, true, 100.0, 0.0);
            double input=atof(edInput->Text.c_str());

            for(int i=0; i<iTestBinCount; i++)
            {
                MyBinPanel[tag]->bCountEnable[i]=false;
                MyBinPanel[tag]->dPersentNumber[i]=input;
                if(MyBinPanel[tag]->iPersentIgnore[i]==0)
                {
                    MyBinPanel[tag]->iPersentIgnore[i]=1;                       //JerryYang 20160728 修改預設ignore顆數0->1
                }
            }
        }
        else
        {
            for(int i=0; i<iTestBinCount; i++)
            {
                MyBinPanel[tag]->dPersentNumber[i]=0.0;
            }
        }

        bEnable=false;
    }

    return true;
}
//------------------------------------------------------------------------------
bool TfBinSel::SetePersentIgnore(int X, int tag)
{
    if(X>=0)
    {
        if(MyBinPanel[tag]->bPersentEnable[X])
        {
            edInput->Text=AnsiString(MyBinPanel[tag]->iPersentIgnore[X]);
            fQwertyKey->ShowQwertyKey(edInput, N_INTEGER, 0, true, 1, 100000);
            MyBinPanel[tag]->iPersentIgnore[X]=atoi(edInput->Text.c_str());
        }
        else
        {
            MyBinPanel[tag]->iPersentIgnore[X]=0;
        }
    }

    return true;
}
//------------------------------------------------------------------------------
bool TfBinSel::SetePersentNumber(int X, int tag)
{
    if(X>=0)
    {
        if(MyBinPanel[tag]->bPersentEnable[X])
        {
            edInput->Text=AnsiString(MyBinPanel[tag]->dPersentNumber[X]);
            fQwertyKey->ShowQwertyKey(edInput, N_DOUBLE, 2, true, 100.0, 0.0);
            MyBinPanel[tag]->dPersentNumber[X]=atof(edInput->Text.c_str());
        }
        else
        {
            MyBinPanel[tag]->dPersentNumber[X]=0.0;
        }
    }

    return true;
}
//------------------------------------------------------------------------------
bool TfBinSel::SeteCountEnable(int X, int tag)
{
    if(X>=0)
    {
        for(int j=eBinSetting; j<eBinSetTotal; j++)                             //ChungHung 20140414 add 必須為fail bin 才能選
        {
            if(MyBinPanel[tag]->BackT6PosTray[X][j]==1)
            {
                MyBinPanel[tag]->bCountEnable[X]=!MyBinPanel[tag]->bCountEnable[X];
            }
        }
        if(MyBinPanel[tag]->bCountEnable[X])
        {
            MyBinPanel[tag]->bPersentEnable[X]=false;
            edInput->Text=AnsiString(BinSelect[tag].iFailCountLimit[X]);    //Alick 20170102 MyBinPanel[tag]->iCountNumber[X]改成BinSelect[tag].iFailCountLimit[X]  保留上次的設定值
            fQwertyKey->ShowQwertyKey(edInput, N_INTEGER, 0, true, 1, 100000);
            MyBinPanel[tag]->iCountNumber[X]=atof(edInput->Text.c_str());
        }
        else
        {
            MyBinPanel[tag]->iCountNumber[X]=0.0;
        }
    }

    return true;
}
//------------------------------------------------------------------------------
bool TfBinSel::SeteCountIgnore(int X, int tag)
{
    if(X>=0)
    {
        if(MyBinPanel[tag]->bCountEnable[X])
        {
            edInput->Text=AnsiString(MyBinPanel[tag]->iCountIgnore[X]);
            fQwertyKey->ShowQwertyKey(edInput, N_INTEGER, 0, true, 1, 100000);
            MyBinPanel[tag]->iCountIgnore[X]=atoi(edInput->Text.c_str());
        }
        else
        {
            MyBinPanel[tag]->iCountIgnore[X]=0;
        }
    }

    return true;
}
//------------------------------------------------------------------------------
bool TfBinSel::SeteCountNumber(int X, int tag)
{
    if(X>=0)
    {
        if(MyBinPanel[tag]->bCountEnable[X])
        {
            edInput->Text=AnsiString(MyBinPanel[tag]->iCountNumber[X]);
            fQwertyKey->ShowQwertyKey(edInput, N_INTEGER, 0, true, 1, 100000);
            MyBinPanel[tag]->iCountNumber[X]=atof(edInput->Text.c_str());
        }
        else
        {
            MyBinPanel[tag]->iCountNumber[X]=0.0;
        }
    }

    return true;
}
//------------------------------------------------------------------------------
bool TfBinSel::SeteSpecialBinByArm(int X, int tag)
{
    if(X>=0)
    {
        for(int j=eBinSetting; j<eBinSetTotal; j++)
        {
            if(MyBinPanel[tag]->BackT6PosTray[X][j]==1)
            {
                if(MyBinPanel[tag]->iT6IsFail[j-eBinSetting]>0)
                    MyBinPanel[tag]->bSpecialBinByArm[X]=!MyBinPanel[tag]->bSpecialBinByArm[X];
                else
                    MyBinPanel[tag]->bSpecialBinByArm[X]=false;
            }
        }

        if(MyBinPanel[tag]->bSpecialBinByArm[X])
        {
            edInput->Text=AnsiString(BinSelect[tag].iSpecialBinCountByArm[X]);  //Alick 20170102 MyBinPanel[tag]->iSpecialBinCountByArm[X]改成BinSelect[tag].iSpecialBinCountByArm[X]  保留上次的設定值
            fQwertyKey->ShowQwertyKey(edInput, N_INTEGER, 0, true, 1, 100000);
            MyBinPanel[tag]->iSpecialBinCountByArm[X]=atof(edInput->Text.c_str());
        }
        else
        {
            MyBinPanel[tag]->iSpecialBinCountByArm[X]=0;
        }
    }

    return true;
}
//------------------------------------------------------------------------------
bool TfBinSel::SeteSpecialBinCountByArm(int X, int tag)
{
    if(X>=0)
    {
        if(MyBinPanel[tag]->bSpecialBinByArm[X])
        {
            edInput->Text=AnsiString(MyBinPanel[tag]->iSpecialBinCountByArm[X]);
            fQwertyKey->ShowQwertyKey(edInput, N_INTEGER, 0, true, 1, 100000);
            MyBinPanel[tag]->iSpecialBinCountByArm[X]=atof(edInput->Text.c_str());
        }
        else
        {
            MyBinPanel[tag]->iSpecialBinCountByArm[X]=0;
        }
    }

    return true;
}
//------------------------------------------------------------------------------
bool TfBinSel::SeteSpecialBinBySocket(int X, int tag)
{
    if(X>=0)
    {
        for(int j=eBinSetting; j<eBinSetTotal; j++)                             //ChungHung 20140414 add 必須為fail bin 才能選
        {
            if(MyBinPanel[tag]->BackT6PosTray[X][j]==1)
            {
                if(MyBinPanel[tag]->iT6IsFail[j-eBinSetting]>0)
                    MyBinPanel[tag]->bSpecialBinBySocket[X]=!MyBinPanel[tag]->bSpecialBinBySocket[X];
                else
                    MyBinPanel[tag]->bSpecialBinBySocket[X]=false;
            }
        }
        if(MyBinPanel[tag]->bSpecialBinBySocket[X])
        {
            edInput->Text=AnsiString(BinSelect[tag].iSpecialBinCountBySocket[X]);    //Alick 20170102 MyBinPanel[tag]->iSpecialBinCountBySocket[X]改成BinSelect[tag].iSpecialBinCountBySocket[X]  保留上次的設定值
            fQwertyKey->ShowQwertyKey(edInput, N_INTEGER, 0, true, 1, 100000);
            MyBinPanel[tag]->iSpecialBinCountBySocket[X]=atoi(edInput->Text.c_str());
        }
        else
        {
            MyBinPanel[tag]->iSpecialBinCountBySocket[X]=0;
        }
    }

    return true;    
}
//------------------------------------------------------------------------------
bool TfBinSel::SeteSpecialBinCountBySocket(int X, int tag)
{
    if(X>=0)
    {
        if(MyBinPanel[tag]->bSpecialBinBySocket[X])
        {
            edInput->Text=AnsiString(MyBinPanel[tag]->iSpecialBinCountBySocket[X]);
            fQwertyKey->ShowQwertyKey(edInput, N_INTEGER, 0, true, 1, 100000);
            MyBinPanel[tag]->iSpecialBinCountBySocket[X]=atoi(edInput->Text.c_str());
        }
        else
        {
            MyBinPanel[tag]->iSpecialBinCountBySocket[X]=0;
        }
    }

    return true;
}
//------------------------------------------------------------------------------
bool TfBinSel::SeteLowYield(int X, int tag)
{
    if(X>=0)
    {
        if(MyBinPanel[tag]->BackT6PosTray[X][eBinNotUse]==0)
            MyBinPanel[tag]->bLowYield[X]=!MyBinPanel[tag]->bLowYield[X];
    }

    return true;
}
//------------------------------------------------------------------------------
bool TfBinSel::SeteArmYield(int X, int tag)
{
    if(X>=0)
    {
        if(MyBinPanel[tag]->BackT6PosTray[X][eBinNotUse]==0)
            MyBinPanel[tag]->bArmYield[X]=!MyBinPanel[tag]->bArmYield[X];
    }

    return true;
}
//------------------------------------------------------------------------------
bool TfBinSel::SeteSiteYield(int X, int tag)
{
    if(X>=0)
    {
        if(MyBinPanel[tag]->BackT6PosTray[X][eBinNotUse]==0)
            MyBinPanel[tag]->bSiteYield[X]=!MyBinPanel[tag]->bSiteYield[X];
    }

    return true;
}
//------------------------------------------------------------------------------
bool TfBinSel::SeteAutoCleanByBin(int X, int tag)
{
    if(X>=0)
    {
        if(TestIF_File.iAutoClean_Function)
        {
            edInput->Text=AnsiString(MyBinPanel[tag]->iAutoCleanByBin[X]);
            fQwertyKey->ShowQwertyKey(edInput, N_INTEGER, 0, true, 0, 100000);
            MyBinPanel[tag]->iAutoCleanByBin[X]=atoi(edInput->Text.c_str());
        }
    }

    return true;
}
//------------------------------------------------------------------------------
bool TfBinSel::SeteAutoCleanBySite(int X, int tag)
{
    if(X>=0)
    {
        if(TestIF_File.iAutoClean_Function)
        {
            edInput->Text=AnsiString(MyBinPanel[tag]->iAutoCleanBySite[X]);
            fQwertyKey->ShowQwertyKey(edInput, N_INTEGER, 0, true, 0, 100000);
            MyBinPanel[tag]->iAutoCleanBySite[X]=atoi(edInput->Text.c_str());
        }
    }

    return true;
}
//------------------------------------------------------------------------------
bool TfBinSel::SeteSpecBinBySiteCompareEnable(int X, int tag)
{
    if(X>=0)
    {
        for(int j=eBinSetting; j<eBinSetTotal; j++)
        {
            if(MyBinPanel[tag]->BackT6PosTray[X][j]==1)
            {
                MyBinPanel[tag]->bSpecBinBySiteCompareEnable[X]=!MyBinPanel[tag]->bSpecBinBySiteCompareEnable[X];
            }
        }

        edInput->Text=AnsiString(BinSelect[tag].dSpecBinBySiteComparePercent[X]);
        fQwertyKey->ShowQwertyKey(edInput, N_DOUBLE, 2, true, 1.0, 100.0);
        MyBinPanel[tag]->dSpecBinBySiteComparePercent[X]=atof(edInput->Text.c_str());
    }

    return true;
}
//------------------------------------------------------------------------------
bool TfBinSel::SeteSpecBinBySiteCompareIgnore(int X, int tag)
{
    if(X>=0)
    {
        if(MyBinPanel[tag]->bSpecBinBySiteCompareEnable[X])
        {
            edInput->Text=AnsiString(MyBinPanel[tag]->iSpecBinBySiteCompareIgnore[X]);
            fQwertyKey->ShowQwertyKey(edInput, N_INTEGER, 0, true, 1, 100000);
            MyBinPanel[tag]->iSpecBinBySiteCompareIgnore[X]=atoi(edInput->Text.c_str());
        }
        else
        {
            MyBinPanel[tag]->iSpecBinBySiteCompareIgnore[X]=0;
        }
    }

    return true;
}
//------------------------------------------------------------------------------
bool TfBinSel::SeteSpecBinBySiteComparePercent(int X, int tag)
{
    if(X>=0)
    {
        if(MyBinPanel[tag]->bSpecBinBySiteCompareEnable[X])
        {
            edInput->Text=AnsiString(MyBinPanel[tag]->dSpecBinBySiteComparePercent[X]);
            fQwertyKey->ShowQwertyKey(edInput, N_DOUBLE, 2, true, 0.0, 100.0);
            MyBinPanel[tag]->dSpecBinBySiteComparePercent[X]=atof(edInput->Text.c_str());
        }
        else
        {
            MyBinPanel[tag]->dSpecBinBySiteComparePercent[X]=0.0;
        }
    }

    return true;
}
//------------------------------------------------------------------------------
bool TfBinSel::SeteSpecBinByArmPerSiteCompareEnable(int X, int tag)
{
    if(X>=0)
    {
        for(int j=eBinSetting; j<eBinSetTotal; j++)
        {
            if(MyBinPanel[tag]->BackT6PosTray[X][j]==1)
            {
                MyBinPanel[tag]->bSpecBinByArmPerSiteCompareEnable[X]=!MyBinPanel[tag]->bSpecBinByArmPerSiteCompareEnable[X];
            }
        }

        edInput->Text=AnsiString(BinSelect[tag].dSpecBinByArmPerSiteComparePercent[X]);
        fQwertyKey->ShowQwertyKey(edInput, N_DOUBLE, 2, true, 1.0, 100.0);
        MyBinPanel[tag]->dSpecBinByArmPerSiteComparePercent[X]=atof(edInput->Text.c_str());
    }

    return true;
}
//------------------------------------------------------------------------------
bool TfBinSel::SeteSpecBinByArmPerSiteCompareIgnore(int X, int tag)
{
    if(X>=0)
    {
        if(MyBinPanel[tag]->bSpecBinByArmPerSiteCompareEnable[X])
        {
            edInput->Text=AnsiString(MyBinPanel[tag]->iSpecBinByArmPerSiteCompareIgnore[X]);
            fQwertyKey->ShowQwertyKey(edInput, N_INTEGER, 0, true, 1, 100000);
            MyBinPanel[tag]->iSpecBinByArmPerSiteCompareIgnore[X]=atoi(edInput->Text.c_str());
        }
        else
        {
            MyBinPanel[tag]->iSpecBinByArmPerSiteCompareIgnore[X]=0;
        }
    }

    return true;
}
//------------------------------------------------------------------------------
bool TfBinSel::SeteSpecBinByArmPerSiteComparePercent(int X, int tag)
{
    if(X>=0)
    {
        if(MyBinPanel[tag]->bSpecBinByArmPerSiteCompareEnable[X])
        {
            edInput->Text=AnsiString(MyBinPanel[tag]->dSpecBinByArmPerSiteComparePercent[X]);
            fQwertyKey->ShowQwertyKey(edInput, N_DOUBLE, 2, true, 0.0, 100.0);
            MyBinPanel[tag]->dSpecBinByArmPerSiteComparePercent[X]=atof(edInput->Text.c_str());
        }
        else
        {
            MyBinPanel[tag]->dSpecBinByArmPerSiteComparePercent[X]=0.0;
        }
    }

    return true;
}
//------------------------------------------------------------------------------
void __fastcall TfBinSel::mtBinSelectMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
    TTMyTray *Ptr;
    Ptr=(TTMyTray *)Sender;
    int tag=Ptr->Tag;

    if(CosFunction.bOneCycleCanChangeContinuesFailBin)                          //ChungHung 20150519 add Continues Failure BIN can select after "OneCycle".
    {
        if(labWarning->Caption!="")
        {
            return;
        }
    }

    if(bMouseDown)
    {
        MyBinPanel[tag]->mtBinSelect->ConvertIndexCells(X, Y);
        if(X<0 || X>=MyBinPanel[tag]->mtBinSelect->XItem || Y<eBinNotUse || Y>=MyBinPanel[tag]->mtBinSelect->YItem)
            return;
        iEndX=X;
        iEndY=Y;
        ShowBinTray(tag);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfBinSel::mtBinSelectMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TTMyTray *Ptr;
    Ptr=(TTMyTray *)Sender;
    int tag=Ptr->Tag;
    int iSX, iEX;

    if(CosFunction.bOneCycleCanChangeContinuesFailBin)                          //ChungHung 20150519 add Continues Failure BIN can select after "OneCycle".
    {
        if(labWarning->Caption!="")
        {
            return;
        }
    }

    if(Barcode_Reader(bcBin)==0)                                                // 20140103 wei KYEC Barcode Reader
    {
        if(iStartX>iEndX)
        {
            iSX=iEndX;
            iEX=iStartX;
        }
        else
        {
            iSX=iStartX;
            iEX=iEndX;
        }

        if(iStartY>=eBinNotUse)
        {
            for(int i=0; i<iTestBinCount; i++)
            {
                if(MyBinPanel[tag]->BackT6PosTray[i][iStartY]==1)
                {
                    if(i<MyBinPanel[tag]->mtBinSelect->XItem)
                    {
                        if(iStartY==eBinNotUse)
                        {
                            MyBinPanel[tag]->mtBinSelect->SetCellColorIndex(i, iStartY, eCLSilver);
                        }
                        else
                        {
                            for(int j=eBinNotUse; j<eBinSetTotal; j++)
                            {
                                MyBinPanel[tag]->mtBinSelect->SetCellColorIndex(i, iStartY, tcBinColor[MyBinPanel[tag]->iT6IsFail[iStartY-eBinSetting]]);
                            }
                        }
                    }
                }
                else
                {
                    if(i<MyBinPanel[tag]->mtBinSelect->XItem)
                    {
                        if(i>=iSX && i<=iEX)
                        {
                            MyBinPanel[tag]->mtBinSelect->SetCellColorIndex(i, iStartY, eCLWhite);
                        }
                    }
                }
            }
        }
        bMouseDown=false;
        return;
    }

    if(bMouseDown)
    {
        bMouseDown=false;
        bMovFlag=false;                                                         //Alick 20160729 add for SCC
        SetBinTray(tag);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfBinSel::SetBinTray(int tag)                                   // set tray data //
{
    Change();
    for(int i=iStartX; i<=iEndX; i++)
    {
        if(CosFunction.bBin1CanNotInFix && tag!=OffT)                           //Steven 20150427 : Bin1不能放到Fix盤
        {
            if(iStartX==1 && iStartY>=eBinSetting+ePosFix1-1)
            {
                iStartY=eBinNotUse;
            }
        }
        MyBinPanel[tag]->BackT6PosTray[i][iStartY]=1;

        for(int j=eBinNotUse; j<eBinSetTotal; j++)
        {
            if(j==iStartY)                                                      //如果是同一ROW
            {

            }
            else
            {
                if(MyBinPanel[tag]->BackT6PosTray[i][iStartY]==1)               //如果選定的那一個是True, 同一Col的其他ROW都要變成False
                    MyBinPanel[tag]->BackT6PosTray[i][j]=0;
            }

            if(MyBinPanel[tag]->BackT6PosTray[i][j]==1)
            {
                if(j!=eBinNotUse &&                                             //Steven 20240814 : 針對No Use要直接false
                   MyBinPanel[tag]->iT6IsFail[j-eBinSetting]>0)                 //ChungHung 20140414 選擇fail bin 預設 continue fial 為 true
                    MyBinPanel[tag]->bConFail[i]=true;
                else
                    MyBinPanel[tag]->bConFail[i]=false;
            }
        }
    }

    InitDataToEdit(tag);
}
//---------------------------------------------------------------------------
void __fastcall TfBinSel::ShowBinTray(int tag)
{
    int iSX, iEX;
    if(iStartX>iEndX)
    {
        iSX=iEndX;
        iEX=iStartX;
    }
    else
    {
        iSX=iStartX;
        iEX=iEndX;
    }

    if(iStartY>=eBinNotUse)
    {
        for(int i=0; i<iTestBinCount; i++)
        {
            if(i<MyBinPanel[tag]->mtBinSelect->XItem)
            {
                if(i>=iSX && i<=iEX)
                {
                    MyBinPanel[tag]->mtBinSelect->SetCellColorIndex(i, iStartY, eCLYellow);
                }
                else
                {
                    if(MyBinPanel[tag]->BackT6PosTray[i][iStartY]==1)
                    {
                        if(iStartY==eBinNotUse)                                 //No USE
                        {
                            MyBinPanel[tag]->mtBinSelect->SetCellColorIndex(i, iStartY, eCLSilver);
                        }
                    }
                    else
                    {
                        MyBinPanel[tag]->mtBinSelect->SetCellColorIndex(i, iStartY, eCLWhite);
                    }
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfBinSel::Change()                                              // check change pos //
{
    int iSX, iEX;
    if(iStartX>iEndX)
    {
        iSX=iEndX;
        iEX=iStartX;
    }
    else
    {
        iSX=iStartX;
        iEX=iEndX;
    }
    iStartX=iSX;
    iEndX=iEX;
}
//---------------------------------------------------------------------------
void __fastcall TfBinSel::mtTrayNameSetColor(int tag)
{
    int iT6, iT6Pos;
    for(int j=eBinSetting; j<eBinSetTotal; j++)
    {
        if(j>=MyBinPanel[tag]->mtTrayName->YItem)
            continue;

        iT6   =j-eBinSetting;
        iT6Pos=j-eBinNotUse;

        if(Prod.iTrayType[iT6]==tNotUse)
        {
            MyBinPanel[tag]->mtTrayItem->SetCellNumber    (eItemError,  j, "");
            MyBinPanel[tag]->mtTrayItem->SetCellColorIndex(eItemError,  j, eCLBtnFace);
            MyBinPanel[tag]->mtTrayName->SetCellNumber    (eItemPass,   j, "");
            MyBinPanel[tag]->mtTrayName->SetCellColorIndex(eItemPass,   j, eCLBtnFace);
            MyBinPanel[tag]->mtTrayName->SetCellNumber    (eItemART,    j, "");
            MyBinPanel[tag]->mtTrayName->SetCellColorIndex(eItemART,    j, eCLBtnFace);
            MyBinPanel[tag]->mtTrayName->SetCellNumber    (eItemCateR,  j, "");
            MyBinPanel[tag]->mtTrayName->SetCellColorIndex(eItemCateR,  j, eCLBtnFace);
            MyBinPanel[tag]->mtTrayName->SetCellNumber    (eItemLink,   j, "");
            MyBinPanel[tag]->mtTrayName->SetCellColorIndex(eItemLink,   j, eCLBtnFace);
        }
        else
        {
            //Error----------------
            if(MyBinPanel[tag]->iErrorT6==iT6)                                      //Error     //JerryYang 20230926
            {
                MyBinPanel[tag]->mtTrayName->SetCellNumber    (eItemError, j, "Error");
                MyBinPanel[tag]->mtTrayName->SetCellColorIndex(eItemError, j, eCLRed);
            }
            else
            {
                MyBinPanel[tag]->mtTrayName->SetCellColorIndex(eItemError, j, eCLWhite);
            }

            //Pass/Fail------------
            if(TestIF_File.bEnableQASampling &&                                     //Steven 20190326 : QA Sampling
               iT6Pos==TestIF_File.iQASamplingT3Pos)
            {
                MyBinPanel[tag]->mtTrayName->SetCellNumber    (eItemPass, j, "QA");
                MyBinPanel[tag]->mtTrayName->SetCellColorIndex(eItemPass, j, eCLBlue);
            }
            else if(MyBinPanel[tag]->iT6IsFail[iT6]>0)                              //0=Pass / 1=Fail
            {
                MyBinPanel[tag]->mtTrayName->SetCellNumber    (eItemPass, j, "Failed");
                MyBinPanel[tag]->mtTrayName->SetCellColorIndex(eItemPass, j, MyBinPanel[tag]->iT6IsFail[j-eBinSetting]+1);  //Steven 20240702 : fixed for bin color
            }
            else
            {
                MyBinPanel[tag]->mtTrayName->SetCellNumber    (eItemPass, j, "Pass");
                MyBinPanel[tag]->mtTrayName->SetCellColorIndex(eItemPass, j, eCLGreen);
            }

            //ART------------------
            if(USE_AUTO_RETEST==eartInstall ||                                  //ChungHung 20140317 add Auto Retest
               IniConfig.bA38_SLT_Summary)                                      //JerryYang 20220923 : add for SLT lot summary
            {
                if(iT6<=iAutoRight)
                {
                    mtTrayName->SetCellNumber(eItemART, j, "Retest");
                    if(UNLOADER_ART[iT6]==eartInstall &&                        //Steven 20161221 (wei) : for SCK only Auto 2 has ART
                       MyBinPanel[tag]->bT6ART[iT6])
                    {
                        MyBinPanel[tag]->mtTrayName->SetCellColorIndex(eItemART, j, eCLRed);
                    }
                    else
                    {
                        MyBinPanel[tag]->mtTrayName->SetCellColorIndex(eItemART, j, eCLWhite);
                    }
                }

                mtTrayName->SetCellNumber(eItemCateR, j, "CateR");
                if(CosFunction.bUseSCKART &&
                   (TestIF_File.iSCKART_SortMode==1 ||                          //Steven 20161221 (wei) : Cate R for ART
                    IniConfig.bA38_SLT_Summary) &&                              //JerryYang 20220923 : add for SLT lot summary
                    MyBinPanel[tag]->bT6CateR[iT6])
                {
                    MyBinPanel[tag]->mtTrayName->SetCellColorIndex(eItemCateR, j, eCLRed);
                }
                else
                {
                    MyBinPanel[tag]->mtTrayName->SetCellColorIndex(eItemCateR, j, eCLWhite);
                }
            }
            else
            {
                MyBinPanel[tag]->mtTrayName->SetCellColorIndex(eItemART,   j, eCLWhite);
                MyBinPanel[tag]->mtTrayName->SetCellColorIndex(eItemCateR, j, eCLWhite);
            }

            //Link-----------------
            if(MyBinPanel[tag]->bT6Link[iT6] && bCanLinkT6[iT6]==true)
            {
                MyBinPanel[tag]->mtTrayName->SetCellNumber    (eItemLink, j, "Linked");
                MyBinPanel[tag]->mtTrayName->SetCellColorIndex(eItemLink, j, eCLGreen);
            }
            else
            {
                MyBinPanel[tag]->mtTrayName->SetCellNumber    (eItemLink, j, "");
                MyBinPanel[tag]->mtTrayName->SetCellColorIndex(eItemLink, j, eCLWhite);
            }
        }
    }

    InitDataToEdit(tag);
}
//---------------------------------------------------------------------------
void __fastcall TfBinSel::InitDataToEdit(int tag)
{
    AnsiString Str="", Temp="";
    TStringList *sList=new TStringList();
    TStringList *sLinkedList=new TStringList();
    AnsiString asBinLinked[eTrayCount];
    ZeroMemory(asBinLinked, sizeof(asBinLinked));

    int iT6;

    //Error-----------
    Str.sprintf("%d", MyBinPanel[tag]->iErrorT6);
    MyBinPanel[tag]->edBinSetT6Error->Text=Str;

    //Pass Fail-------
    sList->Clear();
    for(int i=0; i<eTrayCount; i++)
    {
        sList->Add(MyBinPanel[tag]->iT6IsFail[i]);
    }
    MyBinPanel[tag]->edBinSetT6PassFail->Text=sList->CommaText;

    //AutoRetest------
    sList->Clear();
    for(int i=eAuto1; i<=iAutoRight; i++)
    {
        sList->Add((MyBinPanel[tag]->bT6ART[i])?"1":"0");
    }
    MyBinPanel[tag]->edT6BinART->Text=sList->CommaText;

    //AutoRetest Cate R------                                                   //Steven 20161221 (wei) : for SCK only Auto 2 has ART
    sList->Clear();
    for(int i=0; i<eTrayCount; i++)
    {
        sList->Add((MyBinPanel[tag]->bT6CateR[i])?"1":"0");
    }
    MyBinPanel[tag]->edT6CateR->Text=sList->CommaText;

    //Link-------
    sList->Clear();
    for(int i=0; i<eTrayCount; i++)
    {
        if(bCanLinkT6[i]==false)
            sList->Add("0");
        else
            sList->Add((MyBinPanel[tag]->bT6Link[i])?"1":"0");
    }
    MyBinPanel[tag]->edBinSetT6Link->Text=sList->CommaText;

    //Bin-------------
    sList->Clear();
    for(int i=0; i<iTestBinCount; i++)                                          //Steven 20251104 : iBinCount --> iTestBinCount
    {
        for(int j=eBinNotUse; j<eBinSetTotal; j++)
        {
            if(MyBinPanel[tag]->BackT6PosTray[i][j]==1)
            {
                Temp.sprintf("%d", iTo3PosUnload[j-eBinNotUse]);                //第一排不算,所以要-1
                sList->Add(Temp);
            }
        }
    }
    MyBinPanel[tag]->edBinSetT3Pos->Text=sList->CommaText;

    for(int i=0; i<iTestBinCount; i++)                                          //Steven 20251104 : iBinCount --> iTestBinCount
    {
        for(int j=eBinNotUse; j<eBinSetTotal; j++)
        {
            iT6=j-eBinSetting;
            if(j>MyBinPanel[tag]->mtBinSelect->YItem)
                continue;

            if(MyBinPanel[tag]->BackT6PosTray[i][j])
            {
                if(j==eBinNotUse)                                               //No USE
                {
                    MyBinPanel[tag]->mtBinSelect->SetCellColorIndex(i, j, eCLSilver);
                }
                else
                {
                    if(MyBinPanel[tag]->iT6IsFail[iT6]>0)
                    {
                        MyBinPanel[tag]->mtBinSelect->SetCellColorIndex(i, j, eCLRed);
                    }
                    else
                    {
                        MyBinPanel[tag]->mtBinSelect->SetCellColorIndex(i, j, eCLGreen);
                    }
                }
            }
            else if(AUTO3_IS_MAGAZINE==1 && iT6==eAuto3)                        //JerryYang 20221207 : Magazine not use AUTO3
            {
                MyBinPanel[tag]->mtBinSelect->SetCellColorIndex(i, j, eCLBtnFace);
            }
            else if(CosFunction.bLoaderTrayToAuto1 && TrayForm.LoaderToEmptyColor[iRunStartMode]==2 && iT6==eAuto1)
            {
                MyBinPanel[tag]->mtBinSelect->SetCellColorIndex(i, j, eCLBtnFace);
            }
            else if(Prod.iTrayType[iT6]==tNotUse)                               //JerryYang 20221215 : Magazine把fix區當buffer區功能
            {
                MyBinPanel[tag]->mtBinSelect->SetCellColorIndex(i, j, eCLBtnFace);
            }
            else
            {
                MyBinPanel[tag]->mtBinSelect->SetCellColorIndex(i, j, eCLWhite);
            }
        }
    }

    //Scan-------------
    sList->Clear();
    for(int i=0; i<iTestBinCount; i++)                                          //Steven 20251104 : iBinCount --> iTestBinCount
    {
        sList->Add("0");
        MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eScanning, "");
        MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eScanning, eCLGray);
    }
    MyBinPanel[tag]->edBinSettingEnableScan->Text=sList->CommaText;

    //Double Contact---
    sList->Clear();
    for(int i=0; i<iTestBinCount; i++)                                          //Steven 20251104 : iBinCount --> iTestBinCount
    {
        sList->Add(MyBinPanel[tag]->i2Contact[i]);                              //Steven 20231205 : b2Contact --> i2Contact
        if(Prod.bD22SupportMultiDoubleContact)
           //IniConfig.bD22VerifyMode)                                            //Sam 20231117 : 整合到 QA 模式 //Sam 20221012 : 新增 VerifyMode 功能
        {
            MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eDoubleContact, MyBinPanel[tag]->i2Contact[i]); //Steven 20231205 : b2Contact --> i2Contact
            MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eDoubleContact, (MyBinPanel[tag]->i2Contact[i])?eCLOlive:eCLBtnFace);
        }
        else
        {
            MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eDoubleContact, "");
            MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eDoubleContact, eCLGray);
        }
    }
    MyBinPanel[tag]->edBinSetting2Contact->Text=sList->CommaText;

    //Con. Fail--------
    sList->Clear();
    for(int i=0; i<iTestBinCount; i++)                                          //Steven 20251104 : iBinCount --> iTestBinCount
    {
        sList->Add((MyBinPanel[tag]->bConFail[i])?"1":"0");
        MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eConsFail, (MyBinPanel[tag]->bConFail[i])?"V":"");
        MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eConsFail, (MyBinPanel[tag]->bConFail[i])?eCLOlive:eCLBtnFace);
    }
    MyBinPanel[tag]->edBinSettingConFail->Text=sList->CommaText;

    if(CosFunction.bByBinAlarmFromYieldForm)                                    //jou 20180113 (Steven) : By Site By Bin Percent Compare From Yield form
    {
        for(int i=0; i<iTestBinCount; i++)                                      //Steven 20251104 : iBinCount --> iTestBinCount
        {
            MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, ePersentEnable, "");
            MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, ePersentEnable, eCLGray);
            MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, ePersentIgnore, "");
            MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, ePersentIgnore, eCLGray);
            MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, ePersentNumber, "");
            MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, ePersentNumber, eCLGray);
        }
        MyBinPanel[tag]->edBinSettingFailPersentEnable->Text="";
        MyBinPanel[tag]->edBinSettingFailPersentIgnore->Text="";
        MyBinPanel[tag]->edBinSettingFailPersentNumber->Text="";
    }
    else
    {
        //Persent Enable------
        sList->Clear();
        for(int i=0; i<iTestBinCount; i++)                                      //Steven 20251104 : iBinCount --> iTestBinCount
        {
            sList->Add((MyBinPanel[tag]->bPersentEnable[i])?"1":"0");
            MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, ePersentEnable, (MyBinPanel[tag]->bPersentEnable[i])?"V":"");
            MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, ePersentEnable, (MyBinPanel[tag]->bPersentEnable[i])?eCLOlive:eCLBtnFace);
        }
        MyBinPanel[tag]->edBinSettingFailPersentEnable->Text=sList->CommaText;

        //Persent Ignore------
        sList->Clear();
        for(int i=0; i<iTestBinCount; i++)                                      //Steven 20251104 : iBinCount --> iTestBinCount
        {
            sList->Add(MyBinPanel[tag]->iPersentIgnore[i]);

            if(MyBinPanel[tag]->bPersentEnable[i])
            {
                MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, ePersentIgnore, MyBinPanel[tag]->iPersentIgnore[i]);
                MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, ePersentIgnore, eCLOlive);
            }
            else
            {
                MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, ePersentIgnore, "");
                MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, ePersentIgnore, eCLBtnFace);
            }
        }
        MyBinPanel[tag]->edBinSettingFailPersentIgnore->Text=sList->CommaText;

        //Persent Number------
        sList->Clear();
        for(int i=0; i<iTestBinCount; i++)                                      //Steven 20251104 : iBinCount --> iTestBinCount
        {
            sList->Add(MyBinPanel[tag]->dPersentNumber[i]);

            if(MyBinPanel[tag]->bPersentEnable[i])
            {
                MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, ePersentNumber, MyBinPanel[tag]->dPersentNumber[i]);
                MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, ePersentNumber, eCLOlive);
            }
            else
            {
                MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, ePersentNumber, "");
                MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, ePersentNumber, eCLBtnFace);
            }
        }
        MyBinPanel[tag]->edBinSettingFailPersentNumber->Text=sList->CommaText;
    }

    //Count Enable------
    sList->Clear();
    for(int i=0; i<iTestBinCount; i++)                                          //Steven 20251104 : iBinCount --> iTestBinCount
    {
        sList->Add((MyBinPanel[tag]->bCountEnable[i])?"1":"0");
        MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eCountEnable, (MyBinPanel[tag]->bCountEnable[i])?"V":"");
        MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eCountEnable, (MyBinPanel[tag]->bCountEnable[i])?eCLOlive:eCLBtnFace);
    }
    MyBinPanel[tag]->edBinSettingFailCountEnable->Text=sList->CommaText;

    //Count Ignore------
    sList->Clear();
    for(int i=0; i<iTestBinCount; i++)                                          //Steven 20251104 : iBinCount --> iTestBinCount
    {
        sList->Add(MyBinPanel[tag]->iCountIgnore[i]);

        if(MyBinPanel[tag]->bCountEnable[i])
        {
            MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eCountIgnore, MyBinPanel[tag]->iCountIgnore[i]);
            MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eCountIgnore, eCLOlive);
        }
        else
        {
            MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eCountIgnore, "");
            MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eCountIgnore, eCLBtnFace);
        }
    }
    MyBinPanel[tag]->edBinSettingFailCountIgnore->Text=sList->CommaText;

    //Count Number------
    sList->Clear();
    for(int i=0; i<iTestBinCount; i++)                                          //Steven 20251104 : iBinCount --> iTestBinCount
    {
        sList->Add(MyBinPanel[tag]->iCountNumber[i]);

        if(MyBinPanel[tag]->bCountEnable[i])
        {
            MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eCountNumber, MyBinPanel[tag]->iCountNumber[i]);
            MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eCountNumber, eCLOlive);
        }
        else
        {
            MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eCountNumber, "");
            MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eCountNumber, eCLBtnFace);
        }
    }
    MyBinPanel[tag]->edBinSettingFailCountNumber->Text=sList->CommaText;

    //Special Bin By Arm Enable------
    sList->Clear();
    for(int i=0; i<iTestBinCount; i++)                                          //Steven 20251104 : iBinCount --> iTestBinCount
    {
        for(int j=eBinSetting; j<eBinSetTotal; j++)
        {
            if(MyBinPanel[tag]->BackT6PosTray[i][j]==1)
            {
                if(MyBinPanel[tag]->iT6IsFail[j-eBinSetting]==0)
                    MyBinPanel[tag]->bSpecialBinByArm[i]=false;
            }
        }

        sList->Add((MyBinPanel[tag]->bSpecialBinByArm[i])?"1":"0");
        MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eSpecialBinByArm, (MyBinPanel[tag]->bSpecialBinByArm[i])?"V":"");
        MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eSpecialBinByArm, (MyBinPanel[tag]->bSpecialBinByArm[i])?eCLOlive:eCLBtnFace);
    }
    MyBinPanel[tag]->edSpecialBinByArm->Text=sList->CommaText;

    //Special Bin Count by Arm ------
    sList->Clear();
    for(int i=0; i<iTestBinCount; i++)                                          //Steven 20251104 : iBinCount --> iTestBinCount
    {
        sList->Add(MyBinPanel[tag]->iSpecialBinCountByArm[i]);

        if(MyBinPanel[tag]->bSpecialBinByArm[i])
        {
            MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eSpecialBinCountByArm, int(MyBinPanel[tag]->iSpecialBinCountByArm[i]));
            MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eSpecialBinCountByArm, eCLOlive);
        }
        else
        {
            MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eSpecialBinCountByArm, "");
            MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eSpecialBinCountByArm, eCLBtnFace);
        }
    }
    MyBinPanel[tag]->edSpecialBinCountByArm->Text=sList->CommaText;

    //Special Bin By Socket Enable------
    sList->Clear();
    for(int i=0; i<iTestBinCount; i++)                                          //Steven 20251104 : iBinCount --> iTestBinCount
    {
        for(int j=eBinSetting; j<eBinSetTotal; j++)
        {
            if(MyBinPanel[tag]->BackT6PosTray[i][j]==1)
            {
                if(MyBinPanel[tag]->iT6IsFail[j-eBinSetting]==0)
                    MyBinPanel[tag]->bSpecialBinBySocket[i]=false;
            }
        }

        sList->Add((MyBinPanel[tag]->bSpecialBinBySocket[i])?"1":"0");
        MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eSpecialBinBySocket, (MyBinPanel[tag]->bSpecialBinBySocket[i])?"V":"");
        MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eSpecialBinBySocket, (MyBinPanel[tag]->bSpecialBinBySocket[i])?eCLOlive:eCLBtnFace);
    }
    MyBinPanel[tag]->edSpecialBinBySocket->Text=sList->CommaText;

    //Special Bin Count by Socket ------
    sList->Clear();
    for(int i=0; i<iTestBinCount; i++)                                          //Steven 20251104 : iBinCount --> iTestBinCount
    {
        sList->Add(MyBinPanel[tag]->iSpecialBinCountBySocket[i]);

        if(MyBinPanel[tag]->bSpecialBinBySocket[i])
        {
            MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eSpecialBinCountBySocket, int(MyBinPanel[tag]->iSpecialBinCountBySocket[i]));
            MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eSpecialBinCountBySocket, eCLOlive);
        }
        else
        {
            MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eSpecialBinCountBySocket, "");
            MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eSpecialBinCountBySocket, eCLBtnFace);
        }
    }
    MyBinPanel[tag]->edSpecialBinCountBySocket->Text=sList->CommaText;

    sList->Clear();
    for(int i=0; i<iTestBinCount; i++)                                          //Steven 20140828 : By Bin Yield Monitor
    {
        if(Prod.bLowYieldAlarmByBin)
        {
            sList->Add((MyBinPanel[tag]->bLowYield[i])?"1":"0");
            MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eLowYield, (MyBinPanel[tag]->bLowYield[i])?"V":"");
            MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eLowYield, (MyBinPanel[tag]->bLowYield[i])?eCLOlive:eCLBtnFace);
        }
        else
        {
            sList->Add("0");
            MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eLowYield, "");
            MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eLowYield, eCLGray);
        }
    }
    MyBinPanel[tag]->edLowYield->Text=sList->CommaText;

    if(TestIF_File.iAutoClean_Function)
    {
        sList->Clear();
        for(int i=0; i<iTestBinCount; i++)
        {
            sList->Add(MyBinPanel[tag]->iAutoCleanByBin[i]);
            MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eAutoCleanByBin, (MyBinPanel[tag]->iAutoCleanByBin[i]!=0)?AnsiString(MyBinPanel[tag]->iAutoCleanByBin[i]).c_str():"");    //Steven 20160308 : By Bin pre site count do auto clean
            MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eAutoCleanByBin, (MyBinPanel[tag]->iAutoCleanByBin[i]!=0)?eCLOlive:eCLBtnFace);
        }

        MyBinPanel[tag]->edBySiteClean->Text=sList->CommaText;

        sList->Clear();
        for(int i=0; i<iTestBinCount; i++)
        {
            sList->Add(MyBinPanel[tag]->iAutoCleanBySite[i]);
            MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eAutoCleanBySite, (MyBinPanel[tag]->iAutoCleanBySite[i]!=0)?AnsiString(MyBinPanel[tag]->iAutoCleanBySite[i]).c_str():""); //Steven 20160308 : By Bin pre site count do auto clean
            MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eAutoCleanBySite, (MyBinPanel[tag]->iAutoCleanBySite[i]!=0)?eCLOlive:eCLBtnFace);
        }
        MyBinPanel[tag]->edByBinClean->Text=sList->CommaText;
    }
    else
    {
        for(int i=0; i<iTestBinCount; i++)
        {
            MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eAutoCleanByBin, "");
            MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eAutoCleanBySite, "");
            MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eAutoCleanByBin, eCLGray);      //Steven 20160308 : By Bin count do auto clean
            MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eAutoCleanBySite, eCLGray);     //Steven 20160308 : By Bin pre site count do auto clean
        }
    }

    sList->Clear();
    for(int i=0; i<iTestBinCount; i++)                                          //Steven 20140828 : By Bin Arm Yield Monitor
    {
        if(Prod.bLowYieldAlarmByBin)
        {
            sList->Add((MyBinPanel[tag]->bArmYield[i])?"1":"0");
            MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eArmYield, (MyBinPanel[tag]->bArmYield[i])?"V":"");
            MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eArmYield, (MyBinPanel[tag]->bArmYield[i])?eCLOlive:eCLBtnFace);
        }
        else
        {
            sList->Add("0");
            MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eArmYield, "");
            MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eArmYield, eCLGray);
        }
    }
    MyBinPanel[tag]->edArmYield->Text=sList->CommaText;

    sList->Clear();
    for(int i=0; i<iTestBinCount; i++)                                          //Steven 20140828 : By Bin Site Yield Monitor
    {
        if(Prod.bLowYieldAlarmByBin)
        {
            sList->Add((MyBinPanel[tag]->bSiteYield[i])?"1":"0");
            MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eSiteYield, (MyBinPanel[tag]->bSiteYield[i])?"V":"");
            MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eSiteYield, (MyBinPanel[tag]->bSiteYield[i])?eCLOlive:eCLBtnFace);
        }
        else
        {
            sList->Add("0");
            MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eSiteYield, "");
            MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eSiteYield, eCLGray);
        }
    }
    MyBinPanel[tag]->edSiteYield->Text=sList->CommaText;

    if(CosFunction.bBySiteByBinPercentCompare==false ||                         //JerryYang 20170712 (Steven) by site by bin compare percent
       (CosFunction.bBySiteByBinPercentCompare==true &&                         //jou 20180113 (Steven) : By Site By Bin Percent Compare From Yield form
        CosFunction.bByBinAlarmFromYieldForm))
    {
        for(int i=0; i<iTestBinCount; i++)
        {
            MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eSpecBinBySiteCompareEnable, "");
            MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eSpecBinBySiteCompareEnable, eCLGray);
            MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eSpecBinBySiteCompareIgnore, "");
            MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eSpecBinBySiteCompareIgnore, eCLGray);
            MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eSpecBinBySiteComparePercent, "");
            MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eSpecBinBySiteComparePercent, eCLGray);
            MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eSpecBinByArmPerSiteCompareEnable, "");
            MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eSpecBinByArmPerSiteCompareEnable, eCLGray);
            MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eSpecBinByArmPerSiteCompareIgnore, "");
            MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eSpecBinByArmPerSiteCompareIgnore, eCLGray);
            MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eSpecBinByArmPerSiteComparePercent, "");
            MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eSpecBinByArmPerSiteComparePercent, eCLGray);
        }
        MyBinPanel[tag]->edSpecBinBySiteCompareEnable->Text="";
        MyBinPanel[tag]->edSpecBinBySiteCompareIgnore->Text="";
        MyBinPanel[tag]->edSpecBinBySiteComparePercent->Text="";
        MyBinPanel[tag]->edSpecBinByArmPerSiteCompareEnable->Text="";
        MyBinPanel[tag]->edSpecBinByArmPerSiteCompareIgnore->Text="";
        MyBinPanel[tag]->edSpecBinByArmPerSiteComparePercent->Text="";
    }
    else
    {
        sList->Clear();
        for(int i=0; i<iTestBinCount; i++)
        {
            sList->Add((MyBinPanel[tag]->bSpecBinBySiteCompareEnable[i])?"1":"0");
            MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eSpecBinBySiteCompareEnable, (MyBinPanel[tag]->bSpecBinBySiteCompareEnable[i])?"V":"");
            MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eSpecBinBySiteCompareEnable, (MyBinPanel[tag]->bSpecBinBySiteCompareEnable[i])?eCLOlive:eCLBtnFace);
        }
        MyBinPanel[tag]->edSpecBinBySiteCompareEnable->Text=sList->CommaText;
        //Ignore
        sList->Clear();
        for(int i=0; i<iTestBinCount; i++)
        {
            sList->Add(MyBinPanel[tag]->iSpecBinBySiteCompareIgnore[i]);
            if(MyBinPanel[tag]->bSpecBinBySiteCompareEnable[i])
            {
                MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eSpecBinBySiteCompareIgnore, MyBinPanel[tag]->iSpecBinBySiteCompareIgnore[i]);
                MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eSpecBinBySiteCompareIgnore, eCLOlive);
            }
            else
            {
                MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eSpecBinBySiteCompareIgnore, "");
                MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eSpecBinBySiteCompareIgnore, eCLBtnFace);
            }
        }
        MyBinPanel[tag]->edSpecBinBySiteCompareIgnore->Text=sList->CommaText;

        sList->Clear();
        for(int i=0; i<iTestBinCount; i++)
        {
            sList->Add(MyBinPanel[tag]->dSpecBinBySiteComparePercent[i]);

            if(MyBinPanel[tag]->bSpecBinBySiteCompareEnable[i])
            {
                MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eSpecBinBySiteComparePercent, MyBinPanel[tag]->dSpecBinBySiteComparePercent[i]);
                MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eSpecBinBySiteComparePercent, eCLOlive);
            }
            else
            {
                MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eSpecBinBySiteComparePercent, "");
                MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eSpecBinBySiteComparePercent, eCLBtnFace);
            }
        }
        MyBinPanel[tag]->edSpecBinBySiteComparePercent->Text=sList->CommaText;

        //Percent Enable------
        sList->Clear();
        for(int i=0; i<iTestBinCount; i++)
        {
            sList->Add((MyBinPanel[tag]->bSpecBinByArmPerSiteCompareEnable[i])?"1":"0");
            MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eSpecBinByArmPerSiteCompareEnable, (MyBinPanel[tag]->bSpecBinByArmPerSiteCompareEnable[i])?"V":"");
            MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eSpecBinByArmPerSiteCompareEnable, (MyBinPanel[tag]->bSpecBinByArmPerSiteCompareEnable[i])?eCLOlive:eCLBtnFace);
        }
        MyBinPanel[tag]->edSpecBinByArmPerSiteCompareEnable->Text=sList->CommaText;
        //Ignore
        sList->Clear();
        for(int i=0; i<iTestBinCount; i++)
        {
            sList->Add(MyBinPanel[tag]->iSpecBinByArmPerSiteCompareIgnore[i]);

            if(MyBinPanel[tag]->bSpecBinByArmPerSiteCompareEnable[i])
            {
                MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eSpecBinByArmPerSiteCompareIgnore, MyBinPanel[tag]->iSpecBinByArmPerSiteCompareIgnore[i]);
                MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eSpecBinByArmPerSiteCompareIgnore, eCLOlive);
            }
            else
            {
                MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eSpecBinByArmPerSiteCompareIgnore, "");
                MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eSpecBinByArmPerSiteCompareIgnore, eCLBtnFace);
            }
        }
        MyBinPanel[tag]->edSpecBinByArmPerSiteCompareIgnore->Text=sList->CommaText;

        //Percent Number------
        sList->Clear();
        for(int i=0; i<iTestBinCount; i++)
        {
            sList->Add(MyBinPanel[tag]->dSpecBinByArmPerSiteComparePercent[i]);
            if(MyBinPanel[tag]->bSpecBinByArmPerSiteCompareEnable[i])
            {
                MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eSpecBinByArmPerSiteComparePercent, MyBinPanel[tag]->dSpecBinByArmPerSiteComparePercent[i]);
                MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eSpecBinByArmPerSiteComparePercent, eCLOlive);
            }
            else
            {
                MyBinPanel[tag]->mtBinSelect->SetCellNumber     (i, eSpecBinByArmPerSiteComparePercent, "");
                MyBinPanel[tag]->mtBinSelect->SetCellColorIndex (i, eSpecBinByArmPerSiteComparePercent, eCLBtnFace);
            }
        }
        MyBinPanel[tag]->edSpecBinByArmPerSiteComparePercent->Text=sList->CommaText;
    }

    sBinTraySetT3Pos[tag]->CommaText        =MyBinPanel[tag]->edBinSetT3Pos->Text;
    sBinDoubleContact[tag]->CommaText       =MyBinPanel[tag]->edBinSetting2Contact->Text;
    sBinConsFail[tag]->CommaText            =MyBinPanel[tag]->edBinSettingConFail->Text;
    sBinEnableFail[tag]->CommaText          =MyBinPanel[tag]->edBinSettingFailPersentEnable->Text;
    sBinFailPercent[tag]->CommaText         =MyBinPanel[tag]->edBinSettingFailPersentNumber->Text;
    sBinFailIgnore[tag]->CommaText          =MyBinPanel[tag]->edBinSettingFailPersentIgnore->Text;
    sBinCountEnable[tag]->CommaText         =MyBinPanel[tag]->edBinSettingFailCountEnable->Text;
    sBinCountIgnore[tag]->CommaText         =MyBinPanel[tag]->edBinSettingFailCountIgnore->Text;
    sBinCountNumber[tag]->CommaText         =MyBinPanel[tag]->edBinSettingFailCountNumber->Text;
    sSpecialBinByArm[tag]->CommaText        =MyBinPanel[tag]->edSpecialBinByArm->Text;
    sSpecialBinCountByArm[tag]->CommaText   =MyBinPanel[tag]->edSpecialBinCountByArm->Text;
    sSpecialBinBySocket[tag]->CommaText     =MyBinPanel[tag]->edSpecialBinBySocket->Text;
    sSpecialBinCountBySocket[tag]->CommaText=MyBinPanel[tag]->edSpecialBinCountBySocket->Text;
    sLowYield[tag]->CommaText               =MyBinPanel[tag]->edLowYield->Text;
    sArmYield[tag]->CommaText               =MyBinPanel[tag]->edArmYield->Text;
    sSiteYield[tag]->CommaText              =MyBinPanel[tag]->edSiteYield->Text;
    sBySiteClean[tag]->CommaText            =MyBinPanel[tag]->edBySiteClean->Text;
    sByBinClean[tag]->CommaText             =MyBinPanel[tag]->edByBinClean->Text;

    sSpecBinBySiteCompareEnable[tag]->CommaText         =MyBinPanel[tag]->edSpecBinBySiteCompareEnable->Text;
    sSpecBinBySiteCompareIgnore[tag]->CommaText         =MyBinPanel[tag]->edSpecBinBySiteCompareIgnore->Text;
    sSpecBinBySiteComparePercent[tag]->CommaText        =MyBinPanel[tag]->edSpecBinBySiteComparePercent->Text;
    sSpecBinByArmPerSiteCompareEnable[tag]->CommaText   =MyBinPanel[tag]->edSpecBinByArmPerSiteCompareEnable->Text;
    sSpecBinByArmPerSiteCompareIgnore[tag]->CommaText   =MyBinPanel[tag]->edSpecBinByArmPerSiteCompareIgnore->Text;
    sSpecBinByArmPerSiteComparePercent[tag]->CommaText  =MyBinPanel[tag]->edSpecBinByArmPerSiteComparePercent->Text;

    sBinTrayLinked[tag]->CommaText          =MyBinPanel[tag]->edBinSetT6Link->Text;   //Ifor 20240730 add : BinTrayLinked
    sList->Clear();
    sLinkedList->Clear();

    sList->CommaText=sBinTraySetT3Pos[tag]->CommaText;
    sLinkedList->CommaText=sBinTrayLinked[tag]->CommaText;

    int iadd=0;
    for(int i=0; i<eTrayCount; i++)
    {
        if(sLinkedList->Strings[i]!=0)
        {
            Str="";
            for(int j=i; j<eTrayCount; j++)
            {
                 if(sLinkedList->Strings[j]!=0)
                 {
                    if(j+1>=eTrayCount || sLinkedList->Strings[j+1]==0)
                    {
                        Str=Str+IntToStr(j+1)+";";
                        asBinLinked[i]=Str;
                        i=j;
                        break;
                    }
                    else
                    {
                        Str=Str+IntToStr(j+1)+",";
                    }
                 }
                 else
                 {
                    asBinLinked[i]=Str;
                    i=j;
                    break;
                 }
            }
        }
    }

    Str="";
    for(int i=0; i<sList->Count; i++)
    {
        if(sList->Strings[i]!=0)
        {
            iadd=StrToInt(sList->Strings[i]);

            if(iadd>=eTrayCount)
            {
                Str=Str+"0;";
            }
            else if(sLinkedList->Strings[iadd]!=0)
            {
                Str=Str+asBinLinked[iadd];
            }
            else
            {
                Str=Str+"0;";
            }
        }
        else
        {
            Str=Str+"0;";
        }
    }
    sBinLinked[tag]->CommaText          =Str;                                   //Ifor 20240730 add : BinTrayLinked

    sList->Clear();                                                             //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete sList;
    sList=NULL;                                                                 //kevin 20161108
    sLinkedList->Clear();
    delete sLinkedList;
    sLinkedList=NULL;//kevin 20161108
}
//---------------------------------------------------------------------------
bool TfBinSel::CheckFix2Tray()                                                  //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
{
    if(tAOISetup.bEnabledAOI)
    {
        for(int tag=0; tag<3; tag++)
        {
            for(int i=0; i<iTestBinCount; i++)
            {
                for(int j=eBinNotUse; j<eBinSetTotal; j++)
                {
                    if(MyBinPanel[tag]->BackT6PosTray[i][j])
                    {
                        if((j-eBinNotUse)==ePosFix2)
                            return true;
                    }
                }
            }
        }
    }
    return false;
}
//---------------------------------------------------------------------------
void TfBinSel::ReadFunctionData(int tag)
{
    bool bHasNewSetupData=false;
    AnsiString GroupName, GroupStr;
    int iData, iT3, iT6;
    int iNewBinFormat=0;                                                        //kevin 20160928 判斷 Bin format 是否有轉過記錄

    if(CUSTOMER_CODE!=CC_AMKOR_China)                                           //JerryYang 20180207 (Steven) Amkor上海要求使用舊的讀檔方式
    {
        if(tag==eBinRT)
        {
            //Steven 20160623 : 當IniConfig.bFTBin2RTBin==true && 讀檔時, RT Bin要跟FT Bin一樣
            if((IniConfig.bA02BinModelPrime && IniConfig.bFTBin2RTBin==true && (iBinModelPrime==0 || CosFunction.bDisableRTBinSet)) ||
               (IniConfig.bA02BinModelPrime==false && (IniConfig.bFTBin2RTBin==true || CosFunction.bDisableRTBinSet)))
            {
                GroupName="Bin Func FT";
                if(FormSysTools->CheckSectionExist(GroupName))
                {
                    iNewBinFormat=FormSysTools->CheckAndReadIniData(GroupName, "NewBinFormat",77);//kevin 2016028 使用新的bin format
                    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                         //kevin 2016028 add 使用新的bin format
                    {
                        if(iNewBinFormat!=0 &&iNewBinFormat!=77)                //kevin 20161108
                           bHasNewSetupData=true;
                    }
                    else
                    {
                        bHasNewSetupData=true;
                    }

                    if(bHasNewSetupData)                                        //kevin 20161108
                    {
                        sBinTraySetT3Pos        [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "3617 BinTraySetting", AnsiString(""));
                        sBinType                [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "3656 BinType", AnsiString(""));
                        sBinDoubleContact       [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "3636 BinDoubleContac", AnsiString(""));
                        sBinConsFail            [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "3676 BinConsFail", AnsiString(""));
                        sBinEnableFail          [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10641 BinEnableFail", AnsiString(""));
                        sBinFailPercent         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10642 BinFailPercent", AnsiString(""));
                        sBinFailIgnore          [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10643 BinFailIgnore", AnsiString(""));
                        sBinCountEnable         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10644 BinCountEnable", AnsiString(""));
                        sBinCountIgnore         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10646 BinCountIgnore", AnsiString(""));
                        sBinCountNumber         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10645 BinCountNumber", AnsiString(""));
                        sSpecialBinByArm        [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10647 SpecialBinByArm", AnsiString(""));
                        sSpecialBinCountByArm   [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10648 SpecialBinCountByArm", AnsiString(""));
                        sSpecialBinBySocket     [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10649 SpecialBinBySocket", AnsiString(""));
                        sSpecialBinCountBySocket[tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10650 SpecialBinCountBySocket", AnsiString(""));
                        sLowYield               [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10680 LowYield", AnsiString(""));
                        sArmYield               [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10681 ArmYield", AnsiString(""));
                        sSiteYield              [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10682 SiteYield", AnsiString(""));
                        sBySiteClean            [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10683 BySiteClean", AnsiString(""));
                        sByBinClean             [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10684 ByBinClean", AnsiString(""));
                        //JerryYang 20170712 (Steven) by site by bin compare percent
                        //==>
                        sSpecBinBySiteCompareEnable       [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10685 SpecBinBySiteCompareEnable", AnsiString(""));
                        sSpecBinBySiteCompareIgnore       [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10686 SpecBinBySiteCompareIgnore", AnsiString(""));
                        sSpecBinBySiteComparePercent      [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10687 SpecBinBySiteComparePercent",  AnsiString(""));
                        sSpecBinByArmPerSiteCompareEnable [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10688 SpecBinByArmPerSiteCompareEnable", AnsiString(""));
                        sSpecBinByArmPerSiteCompareIgnore [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10689 SpecBinByArmPerSiteCompareIgnore", AnsiString(""));
                        sSpecBinByArmPerSiteComparePercent[tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10690 SpecBinByArmPerSiteComparePercent",  AnsiString(""));
                        //<==
                        //JerryYang 20170712 (Steven) by site by bin compare percent

                        sBinTrayLinked  [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "BinTrayLinked(10698)",  AnsiString("0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"));    //Ifor 20240730 add : BinTrayLinked
                        sBinLinked      [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "BinLinked(10699)",  AnsiString("0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;"));    //Ifor 20240730 add : BinLinked
                        sMagazineSetup  [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "MagazineSetup(10700)", AnsiString("0"));
                    }
                }
            }
            else
            {
                GroupName="Bin Func RT";
                if(FormSysTools->CheckSectionExist(GroupName))
                {
                    iNewBinFormat=FormSysTools->CheckAndReadIniData(GroupName, "NewBinFormat", 77);//kevin 2016028 使用新的bin format
                    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                         //kevin 2016028 add 使用新的bin format
                    {
                        if(iNewBinFormat!=77)                                   //kevin 20160928
                            bHasNewSetupData=true;
                    }
                    else
                    {
                        bHasNewSetupData=true;
                    }

                    if(bHasNewSetupData)                                        //kevin 20161108
                    {
                        sBinTraySetT3Pos        [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "3717 BinTraySetting", AnsiString(""));
                        sBinDoubleContact       [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "3718 BinDoubleContac", AnsiString(""));
                        sBinType                [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "3719 BinType", AnsiString(""));
                        sBinConsFail            [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "3720 BinConsFail", AnsiString(""));
                        sBinEnableFail          [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10741 BinEnableFail", AnsiString(""));
                        sBinFailPercent         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10742 BinFailPercent", AnsiString(""));
                        sBinFailIgnore          [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10743 BinFailIgnore", AnsiString(""));
                        sBinCountEnable         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10744 BinCountEnable", AnsiString(""));
                        sBinCountNumber         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10745 BinCountNumber", AnsiString(""));
                        sBinCountIgnore         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10746 BinCountIgnore", AnsiString(""));
                        sSpecialBinByArm        [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10747 SpecialBinByArm", AnsiString(""));
                        sSpecialBinCountByArm   [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10748 SpecialBinCountByArm", AnsiString(""));
                        sSpecialBinBySocket     [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10749 SpecialBinBySocket", AnsiString(""));
                        sSpecialBinCountBySocket[tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10750 SpecialBinCountBySocket", AnsiString(""));
                        sLowYield               [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10780 LowYield", AnsiString(""));
                        sArmYield               [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10781 ArmYield", AnsiString(""));
                        sSiteYield              [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10782 SiteYield", AnsiString(""));
                        sBySiteClean            [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10783 BySiteClean", AnsiString(""));
                        sByBinClean             [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10784 ByBinClean", AnsiString(""));
                        //JerryYang 20170712 (Steven) by site by bin compare percent
                        //==>
                        sSpecBinBySiteCompareEnable       [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10785 SpecBinBySiteCompareEnable", AnsiString(""));
                        sSpecBinBySiteCompareIgnore       [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10786 SpecBinBySiteCompareIgnore", AnsiString(""));
                        sSpecBinBySiteComparePercent      [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10787 SpecBinBySiteComparePercent",  AnsiString(""));
                        sSpecBinByArmPerSiteCompareEnable [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10788 SpecBinByArmPerSiteCompareEnable", AnsiString(""));
                        sSpecBinByArmPerSiteCompareIgnore [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10789 SpecBinByArmPerSiteCompareIgnore", AnsiString(""));
                        sSpecBinByArmPerSiteComparePercent[tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10790 SpecBinByArmPerSiteComparePercent",  AnsiString(""));
                        //<==
                        //JerryYang 20170712 (Steven) by site by bin compare percent

                        sBinTrayLinked  [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "BinTrayLinked(10798)",  AnsiString("0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"));    //Ifor 20240730 add : BinTrayLinked
                        sBinLinked      [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "BinLinked(10799)",  AnsiString("0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;"));    //Ifor 20240730 add : BinLinked
                        sMagazineSetup  [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "MagazineSetup(10800)", AnsiString("0"));
                    }
                }
            }
        }
        else if(tag==eBinFT)
        {
            GroupName="Bin Func FT";
            if(FormSysTools->CheckSectionExist(GroupName))
            {
                iNewBinFormat=FormSysTools->CheckAndReadIniData(GroupName, "NewBinFormat",77);//kevin 2016028 使用新的bin format
                if(CUSTOMER_CODE==CC_ASE_KaohSiung)                             //kevin 2016028 add 使用新的bin format
                {
                    if(iNewBinFormat!=77)                                       //kevin 20160928
                        bHasNewSetupData=true;
                }
                else
                {
                    bHasNewSetupData=true;
                }

                if(bHasNewSetupData)                                            //kevin 20161108
                {
                    sBinTraySetT3Pos        [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "3617 BinTraySetting", AnsiString(""));
                    sBinType                [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "3656 BinType", AnsiString(""));
                    sBinDoubleContact       [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "3636 BinDoubleContac", AnsiString(""));
                    sBinConsFail            [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "3676 BinConsFail", AnsiString(""));
                    sBinEnableFail          [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10641 BinEnableFail", AnsiString(""));
                    sBinFailPercent         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10642 BinFailPercent", AnsiString(""));
                    sBinFailIgnore          [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10643 BinFailIgnore", AnsiString(""));
                    sBinCountEnable         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10644 BinCountEnable", AnsiString(""));
                    sBinCountIgnore         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10646 BinCountIgnore", AnsiString(""));
                    sBinCountNumber         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10645 BinCountNumber", AnsiString(""));
                    sSpecialBinByArm        [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10647 SpecialBinByArm", AnsiString(""));
                    sSpecialBinCountByArm   [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10648 SpecialBinCountByArm", AnsiString(""));
                    sSpecialBinBySocket     [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10649 SpecialBinBySocket", AnsiString(""));
                    sSpecialBinCountBySocket[tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10650 SpecialBinCountBySocket", AnsiString(""));
                    sLowYield               [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10680 LowYield", AnsiString(""));
                    sArmYield               [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10681 ArmYield", AnsiString(""));
                    sSiteYield              [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10682 SiteYield", AnsiString(""));
                    sBySiteClean            [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10683 BySiteClean", AnsiString(""));
                    sByBinClean             [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10684 ByBinClean", AnsiString(""));
                    //JerryYang 20170712 (Steven) by site by bin compare percent
                    //==>
                    sSpecBinBySiteCompareEnable       [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10685 SpecBinBySiteCompareEnable", AnsiString(""));
                    sSpecBinBySiteCompareIgnore       [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10686 SpecBinBySiteCompareIgnore", AnsiString(""));
                    sSpecBinBySiteComparePercent      [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10687 SpecBinBySiteComparePercent",  AnsiString(""));
                    sSpecBinByArmPerSiteCompareEnable [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10688 SpecBinByArmPerSiteCompareEnable", AnsiString(""));
                    sSpecBinByArmPerSiteCompareIgnore [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10689 SpecBinByArmPerSiteCompareIgnore", AnsiString(""));
                    sSpecBinByArmPerSiteComparePercent[tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10690 SpecBinByArmPerSiteComparePercent", AnsiString(""));
                    //<==
                    //JerryYang 20170712 (Steven) by site by bin compare percent

                    sBinTrayLinked  [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "BinTrayLinked(10698)",  AnsiString("0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"));    //Ifor 20240730 add : BinTrayLinked
                    sBinLinked      [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "BinLinked(10699)",  AnsiString("0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;"));    //Ifor 20240730 add : BinLinked
                    sMagazineSetup  [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "MagazineSetup(10700)", AnsiString("0"));
                }
            }
        }
        else if(tag==eBinOffLine)
        {
            GroupName="Bin Func OffLine";
            if(FormSysTools->CheckSectionExist(GroupName))
            {
                iNewBinFormat=FormSysTools->CheckAndReadIniData(GroupName, "NewBinFormat",77);//kevin 2016028 使用新的bin format
                if(CUSTOMER_CODE==CC_ASE_KaohSiung)                             //kevin 2016028 add 使用新的bin format
                {
                    if(iNewBinFormat!=77)                                       //kevin 20160928
                        bHasNewSetupData=true;
                }
                else
                {
                    bHasNewSetupData=true;
                }

                if(bHasNewSetupData)                                            //kevin 20161108
                {
                    sBinTraySetT3Pos        [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "3722 BinTraySetting", AnsiString(""));
                    sBinDoubleContact       [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "3723 BinDoubleContac", AnsiString(""));
                    sBinType                [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "3724 BinType", AnsiString(""));
                    sBinConsFail            [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "3725 BinConsFail", AnsiString(""));
                    sBinEnableFail          [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10941 BinEnableFail", AnsiString(""));
                    sBinFailPercent         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10942 BinFailPercent", AnsiString(""));
                    sBinFailIgnore          [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10943 BinFailIgnore", AnsiString(""));
                    sBinCountEnable         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10944 BinCountEnable", AnsiString(""));
                    sBinCountNumber         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10945 BinCountNumber", AnsiString(""));
                    sBinCountIgnore         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10946 BinCountIgnore", AnsiString(""));
                    sSpecialBinByArm        [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10947 SpecialBinByArm", AnsiString(""));
                    sSpecialBinCountByArm   [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10948 SpecialBinCountByArm", AnsiString(""));
                    sSpecialBinBySocket     [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10949 SpecialBinBySocket", AnsiString(""));
                    sSpecialBinCountBySocket[tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10950 SpecialBinCountBySocket", AnsiString(""));
                    sLowYield               [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10980 LowYield", AnsiString(""));
                    sArmYield               [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10981 ArmYield", AnsiString(""));
                    sSiteYield              [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10982 SiteYield", AnsiString(""));
                    sBySiteClean            [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10983 BySiteClean", AnsiString(""));
                    sByBinClean             [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10984 ByBinClean", AnsiString(""));
                    //JerryYang 20170712 (Steven) by site by bin compare percent
                    //==>
                    sSpecBinBySiteCompareEnable       [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10985 SpecBinBySiteCompareEnable", AnsiString(""));
                    sSpecBinBySiteCompareIgnore       [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10986 SpecBinBySiteCompareIgnore", AnsiString(""));
                    sSpecBinBySiteComparePercent      [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10987 SpecBinBySiteComparePercent",  AnsiString(""));
                    sSpecBinByArmPerSiteCompareEnable [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10988 SpecBinByArmPerSiteCompareEnable", AnsiString(""));
                    sSpecBinByArmPerSiteCompareIgnore [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10989 SpecBinByArmPerSiteCompareIgnore", AnsiString(""));
                    sSpecBinByArmPerSiteComparePercent[tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "10990 SpecBinByArmPerSiteComparePercent",  AnsiString(""));
                    //<==
                    //JerryYang 20170712 (Steven) by site by bin compare percent

                    sBinTrayLinked  [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "BinTrayLinked(10998)",  AnsiString("0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"));    //Ifor 20240730 add : BinTrayLinked
                    sBinLinked      [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "BinLinked(10999)",  AnsiString("0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;"));    //Ifor 20240730 add : BinLinked
                    sMagazineSetup  [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "MagazineSetup(11000)", AnsiString("0"));
                }
            }
        }
        else if(tag==eBinFT_ART)                                                //Steven 20161123 : eBinRT_ART --> eBinFT_ART
        {
            GroupName="Bin Func ART_FT";                                        //wei 20161124 修改eBinFT_ART存成Bin Func ART_RT問題
            if(FormSysTools->CheckSectionExist(GroupName))
            {
                iNewBinFormat=FormSysTools->CheckAndReadIniData(GroupName, "NewBinFormat",77);//kevin 2016028 使用新的bin format
                if(CUSTOMER_CODE==CC_ASE_KaohSiung)                             //kevin 2016028 add 使用新的bin format
                {
                    if(iNewBinFormat!=77)                                       //kevin 20160928
                        bHasNewSetupData=true;
                }
                else
                {
                    bHasNewSetupData=true;
                }

                if(bHasNewSetupData)                                            //kevin 20161108
                {
                    sBinTraySetT3Pos        [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "3801 BinTraySetting", AnsiString(""));
                    sBinDoubleContact       [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "3802 BinDoubleContac", AnsiString(""));
                    sBinType                [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "3803 BinType", AnsiString(""));
                    sBinConsFail            [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "3804 BinConsFail", AnsiString(""));
                    sBinEnableFail          [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11041 BinEnableFail", AnsiString(""));
                    sBinFailPercent         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11042 BinFailPercent", AnsiString(""));
                    sBinFailIgnore          [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11043 BinFailIgnore", AnsiString(""));
                    sBinCountEnable         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11044 BinCountEnable", AnsiString(""));
                    sBinCountNumber         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11045 BinCountNumber", AnsiString(""));
                    sBinCountIgnore         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11046 BinCountIgnore", AnsiString(""));
                    sSpecialBinByArm        [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11047 SpecialBinByArm", AnsiString(""));
                    sSpecialBinCountByArm   [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11048 SpecialBinCountByArm", AnsiString(""));
                    sSpecialBinBySocket     [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11049 SpecialBinBySocket", AnsiString(""));
                    sSpecialBinCountBySocket[tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11050 SpecialBinCountBySocket", AnsiString(""));
                    sLowYield               [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11080 LowYield", AnsiString(""));
                    sArmYield               [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11081 ArmYield", AnsiString(""));
                    sSiteYield              [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11082 SiteYield", AnsiString(""));
                    sBySiteClean            [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11083 BySiteClean", AnsiString(""));
                    sByBinClean             [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11084 ByBinClean", AnsiString(""));
                    //JerryYang 20170712 (Steven) by site by bin compare percent
                    //==>
                    sSpecBinBySiteCompareEnable       [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11085 SpecBinBySiteCompareEnable", AnsiString(""));
                    sSpecBinBySiteCompareIgnore       [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11086 SpecBinBySiteCompareIgnore", AnsiString(""));
                    sSpecBinBySiteComparePercent      [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11087 SpecBinBySiteComparePercent",  AnsiString(""));
                    sSpecBinByArmPerSiteCompareEnable [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11088 SpecBinByArmPerSiteCompareEnable", AnsiString(""));
                    sSpecBinByArmPerSiteCompareIgnore [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11089 SpecBinByArmPerSiteCompareIgnore", AnsiString(""));
                    sSpecBinByArmPerSiteComparePercent[tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11090 SpecBinByArmPerSiteComparePercent",  AnsiString(""));
                    //<==
                    //JerryYang 20170712 (Steven) by site by bin compare percent

                    sBinTrayLinked  [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "BinTrayLinked(11098)",  AnsiString("0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"));    //Ifor 20240730 add : BinTrayLinked
                    sBinLinked      [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "BinLinked(11099)",  AnsiString("0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;"));    //Ifor 20240730 add : BinLinked
                    sMagazineSetup  [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "MagazineSetup(11100)", AnsiString("0"));
                }
            }
            else
            {
                GroupName="Bin Func ART_RT";
                if(FormSysTools->CheckSectionExist(GroupName))
                {
                    iNewBinFormat=FormSysTools->CheckAndReadIniData(GroupName, "NewBinFormat",77);//kevin 2016028 使用新的bin format
                    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                         //kevin 2016028 add 使用新的bin format
                    {
                        if(iNewBinFormat!=77)                                   //kevin 20160928
                            bHasNewSetupData=true;
                    }
                    else
                    {
                        bHasNewSetupData=true;
                    }

                    if(bHasNewSetupData)                                        //kevin 20161108
                    {
                        sBinTraySetT3Pos        [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "3901 BinTraySetting", AnsiString(""));
                        sBinDoubleContact       [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "3902 BinDoubleContac", AnsiString(""));
                        sBinType                [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "3903 BinType", AnsiString(""));
                        sBinConsFail            [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "3904 BinConsFail", AnsiString(""));
                        sBinEnableFail          [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11141 BinEnableFail", AnsiString(""));
                        sBinFailPercent         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11142 BinFailPercent", AnsiString(""));
                        sBinFailIgnore          [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11143 BinFailIgnore", AnsiString(""));
                        sBinCountEnable         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11144 BinCountEnable", AnsiString(""));
                        sBinCountNumber         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11145 BinCountNumber", AnsiString(""));
                        sBinCountIgnore         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11146 BinCountIgnore", AnsiString(""));
                        sSpecialBinByArm        [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11147 SpecialBinByArm", AnsiString(""));
                        sSpecialBinCountByArm   [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11148 SpecialBinCountByArm", AnsiString(""));
                        sSpecialBinBySocket     [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11149 SpecialBinBySocket", AnsiString(""));
                        sSpecialBinCountBySocket[tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11150 SpecialBinCountBySocket", AnsiString(""));
                        sLowYield               [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11180 LowYield", AnsiString(""));
                        sArmYield               [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11181 ArmYield", AnsiString(""));
                        sSiteYield              [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11182 SiteYield", AnsiString(""));
                        sBySiteClean            [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11183 BySiteClean", AnsiString(""));     //Ifor 20170316 (wei) BinRT_ART BySiteClean 11083 --> 11183
                        sByBinClean             [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11184 ByBinClean", AnsiString(""));      //Ifor 20170316 (wei) BinRT_ART ByBinClean  11084 --> 11184
                        //JerryYang 20170712 (Steven) by site by bin compare percent
                        //==>
                        sSpecBinBySiteCompareEnable       [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11185 SpecBinBySiteCompareEnable", AnsiString(""));
                        sSpecBinBySiteCompareIgnore       [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11186 SpecBinBySiteCompareIgnore", AnsiString(""));
                        sSpecBinBySiteComparePercent      [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11187 SpecBinBySiteComparePercent",  AnsiString(""));
                        sSpecBinByArmPerSiteCompareEnable [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11188 SpecBinByArmPerSiteCompareEnable", AnsiString(""));
                        sSpecBinByArmPerSiteCompareIgnore [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11189 SpecBinByArmPerSiteCompareIgnore", AnsiString(""));
                        sSpecBinByArmPerSiteComparePercent[tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11190 SpecBinByArmPerSiteComparePercent",  AnsiString(""));
                        //<==
                        //JerryYang 20170712 (Steven) by site by bin compare percent

                        sBinTrayLinked  [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "BinTrayLinked(11198)",  AnsiString("0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"));    //Ifor 20240730 add : BinTrayLinked
                        sBinLinked      [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "BinLinked(11199)",  AnsiString("0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;"));    //Ifor 20240730 add : BinLinked
                        sMagazineSetup  [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "MagazineSetup(11200)", AnsiString("0"));
                    }
                }
            }
        }
        else if(tag==eBinRT_ART)                                                //Steven 20161123 : eBinFT_ART --> eBinRT_ART
        {
            GroupName="Bin Func ART_RT";                                        //wei 20161124 修改eBinRT_ART存成Bin Func ART_FT問題
            if(FormSysTools->CheckSectionExist(GroupName))
            {
                iNewBinFormat=FormSysTools->CheckAndReadIniData(GroupName, "NewBinFormat", 77);//kevin 2016028 使用新的bin format
                if(CUSTOMER_CODE==CC_ASE_KaohSiung)                             //kevin 2016028 add 使用新的bin format
                {
                    if(iNewBinFormat!=77)                                       //kevin 20160928
                        bHasNewSetupData=true;
                }
                else
                {
                    bHasNewSetupData=true;
                }

                if(bHasNewSetupData)                                            //kevin 20161108
                {
                    sBinTraySetT3Pos        [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "3901 BinTraySetting", AnsiString(""));
                    sBinDoubleContact       [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "3902 BinDoubleContac", AnsiString(""));
                    sBinType                [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "3903 BinType", AnsiString(""));
                    sBinConsFail            [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "3904 BinConsFail", AnsiString(""));
                    sBinEnableFail          [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11141 BinEnableFail", AnsiString(""));
                    sBinFailPercent         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11142 BinFailPercent", AnsiString(""));
                    sBinFailIgnore          [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11143 BinFailIgnore", AnsiString(""));
                    sBinCountEnable         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11144 BinCountEnable", AnsiString(""));
                    sBinCountNumber         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11145 BinCountNumber", AnsiString(""));
                    sBinCountIgnore         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11146 BinCountIgnore", AnsiString(""));
                    sSpecialBinByArm        [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11147 SpecialBinByArm", AnsiString(""));
                    sSpecialBinCountByArm   [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11148 SpecialBinCountByArm", AnsiString(""));
                    sSpecialBinBySocket     [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11149 SpecialBinBySocket", AnsiString(""));
                    sSpecialBinCountBySocket[tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11150 SpecialBinCountBySocket", AnsiString(""));
                    sLowYield               [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11180 LowYield", AnsiString(""));
                    sArmYield               [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11181 ArmYield", AnsiString(""));
                    sSiteYield              [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11182 SiteYield", AnsiString(""));
                    sBySiteClean            [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11183 BySiteClean", AnsiString(""));    //wei 20161124 11083 --> 11183
                    sByBinClean             [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11184 ByBinClean", AnsiString(""));     //wei 20161124 11084 --> 11184
                    //JerryYang 20170712 (Steven) by site by bin compare percent
                    //==>
                    sSpecBinBySiteCompareEnable       [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11185 SpecBinBySiteCompareEnable", AnsiString(""));
                    sSpecBinBySiteCompareIgnore       [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11186 SpecBinBySiteCompareIgnore", AnsiString(""));
                    sSpecBinBySiteComparePercent      [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11187 SpecBinBySiteComparePercent",  AnsiString(""));
                    sSpecBinByArmPerSiteCompareEnable [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11188 SpecBinByArmPerSiteCompareEnable", AnsiString(""));
                    sSpecBinByArmPerSiteCompareIgnore [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11189 SpecBinByArmPerSiteCompareIgnore", AnsiString(""));
                    sSpecBinByArmPerSiteComparePercent[tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11190 SpecBinByArmPerSiteComparePercent",  AnsiString(""));
                    //<==
                    //JerryYang 20170712 (Steven) by site by bin compare percent

                    sBinTrayLinked  [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "BinTrayLinked(11198)",  AnsiString("0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"));    //Ifor 20240730 add : BinTrayLinked
                    sBinLinked      [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "BinLinked(11199)",  AnsiString("0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;"));    //Ifor 20240730 add : BinLinked
                    sMagazineSetup  [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "MagazineSetup(11200)", AnsiString("0"));
                }
            }
            else
            {
                GroupName="Bin Func ART_FT";
                if(FormSysTools->CheckSectionExist(GroupName))
                {
                    iNewBinFormat=FormSysTools->CheckAndReadIniData(GroupName, "NewBinFormat",77);//kevin 2016028 使用新的bin format
                    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                         //kevin 2016028 add 使用新的bin format
                    {
                        if(iNewBinFormat!=77)                                   //kevin 20160928
                            bHasNewSetupData=true;
                    }
                    else
                    {
                        bHasNewSetupData=true;
                    }

                    if(bHasNewSetupData)                                        //kevin 20161108
                    {
                        sBinTraySetT3Pos        [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "3801 BinTraySetting", AnsiString(""));
                        sBinDoubleContact       [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "3802 BinDoubleContac", AnsiString(""));
                        sBinType                [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "3803 BinType", AnsiString(""));
                        sBinConsFail            [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "3804 BinConsFail", AnsiString(""));
                        sBinEnableFail          [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11041 BinEnableFail", AnsiString(""));
                        sBinFailPercent         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11042 BinFailPercent", AnsiString(""));
                        sBinFailIgnore          [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11043 BinFailIgnore", AnsiString(""));
                        sBinCountEnable         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11044 BinCountEnable", AnsiString(""));
                        sBinCountNumber         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11045 BinCountNumber", AnsiString(""));
                        sBinCountIgnore         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11046 BinCountIgnore", AnsiString(""));
                        sSpecialBinByArm        [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11047 SpecialBinByArm", AnsiString(""));
                        sSpecialBinCountByArm   [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11048 SpecialBinCountByArm", AnsiString(""));
                        sSpecialBinBySocket     [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11049 SpecialBinBySocket", AnsiString(""));
                        sSpecialBinCountBySocket[tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11050 SpecialBinCountBySocket", AnsiString(""));
                        sLowYield               [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11080 LowYield", AnsiString(""));
                        sArmYield               [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11081 ArmYield", AnsiString(""));
                        sSiteYield              [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11082 SiteYield", AnsiString(""));
                        sBySiteClean            [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11083 BySiteClean", AnsiString(""));
                        sByBinClean             [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11084 ByBinClean", AnsiString(""));

                        sBinTrayLinked          [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "BinTrayLinked(11098)",  AnsiString("0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"));    //Ifor 20240730 add : BinTrayLinked
                        sBinLinked              [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "BinLinked(11099)",  AnsiString("0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;"));    //Ifor 20240730 add : BinLinked
                        sMagazineSetup          [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "MagazineSetup(11100)", AnsiString("0"));
                    }
                }
            }
        }
        else if(tag==eBinFT_MRT)                                                //Steven 20161123 : eBinRT_ART --> eBinFT_ART
        {                                                                       //Ifor 20170316 (wei) add KYEC MRT Mode
            GroupName="Bin Func MRT_FT";                                        //wei 20161124 修改eBinFT_ART存成Bin Func ART_RT問題
            if(FormSysTools->CheckSectionExist(GroupName))
            {
                iNewBinFormat=FormSysTools->CheckAndReadIniData(GroupName, "NewBinFormat", 77);//kevin 2016028 使用新的bin format
                if(CUSTOMER_CODE==CC_ASE_KaohSiung)                             //kevin 2016028 add 使用新的bin format
                {
                    if(iNewBinFormat!=77)                                       //kevin 20160928
                        bHasNewSetupData=true;
                }
                else
                {
                    bHasNewSetupData=true;
                }

                if(bHasNewSetupData)                                            //kevin 20161108
                {
                    sBinTraySetT3Pos        [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "4001 BinTraySetting", AnsiString(""));
                    sBinDoubleContact       [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "4002 BinDoubleContac", AnsiString(""));
                    sBinType                [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "4003 BinType", AnsiString(""));
                    sBinConsFail            [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "4004 BinConsFail", AnsiString(""));
                    sBinEnableFail          [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11241 BinEnableFail", AnsiString(""));
                    sBinFailPercent         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11242 BinFailPercent", AnsiString(""));
                    sBinFailIgnore          [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11243 BinFailIgnore", AnsiString(""));
                    sBinCountEnable         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11244 BinCountEnable", AnsiString(""));
                    sBinCountNumber         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11245 BinCountNumber", AnsiString(""));
                    sBinCountIgnore         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11246 BinCountIgnore", AnsiString(""));
                    sSpecialBinByArm        [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11247 SpecialBinByArm", AnsiString(""));
                    sSpecialBinCountByArm   [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11248 SpecialBinCountByArm", AnsiString(""));
                    sSpecialBinBySocket     [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11249 SpecialBinBySocket", AnsiString(""));
                    sSpecialBinCountBySocket[tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11250 SpecialBinCountBySocket", AnsiString(""));
                    sLowYield               [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11280 LowYield", AnsiString(""));
                    sArmYield               [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11281 ArmYield", AnsiString(""));
                    sSiteYield              [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11282 SiteYield", AnsiString(""));
                    sBySiteClean            [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11283 BySiteClean", AnsiString(""));
                    sByBinClean             [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11284 ByBinClean", AnsiString(""));

                    sBinTrayLinked          [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "BinTrayLinked(11298)",  AnsiString("0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"));    //Ifor 20240730 add : BinTrayLinked
                    sBinLinked              [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "BinLinked(11299)",  AnsiString("0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;"));    //Ifor 20240730 add : BinLinked
                    sMagazineSetup          [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "MagazineSetup(11300)", AnsiString("0"));
                }
            }
        }
        else if(tag==eBinRT_MRT)                                                //Steven 20161123 : eBinFT_ART --> eBinRT_ART
        {
            GroupName="Bin Func MRT_RT";                                        //wei 20161124 修改eBinRT_ART存成Bin Func ART_FT問題
            if(FormSysTools->CheckSectionExist(GroupName))
            {
                iNewBinFormat=FormSysTools->CheckAndReadIniData(GroupName, "NewBinFormat",77);//kevin 2016028 使用新的bin format
                if(CUSTOMER_CODE==CC_ASE_KaohSiung)                             //kevin 2016028 add 使用新的bin format
                {
                    if(iNewBinFormat!=77)                                       //kevin 20160928
                        bHasNewSetupData=true;
                }
                else
                {
                    bHasNewSetupData=true;
                }

                if(bHasNewSetupData)                                            //kevin 20161108
                {
                    sBinTraySetT3Pos        [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "4101 BinTraySetting", AnsiString(""));
                    sBinDoubleContact       [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "4102 BinDoubleContac", AnsiString(""));
                    sBinType                [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "4103 BinType", AnsiString(""));
                    sBinConsFail            [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "4104 BinConsFail", AnsiString(""));
                    sBinEnableFail          [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11341 BinEnableFail", AnsiString(""));
                    sBinFailPercent         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11342 BinFailPercent", AnsiString(""));
                    sBinFailIgnore          [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11343 BinFailIgnore", AnsiString(""));
                    sBinCountEnable         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11344 BinCountEnable", AnsiString(""));
                    sBinCountNumber         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11345 BinCountNumber", AnsiString(""));
                    sBinCountIgnore         [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11346 BinCountIgnore", AnsiString(""));
                    sSpecialBinByArm        [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11347 SpecialBinByArm", AnsiString(""));
                    sSpecialBinCountByArm   [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11348 SpecialBinCountByArm", AnsiString(""));
                    sSpecialBinBySocket     [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11349 SpecialBinBySocket", AnsiString(""));
                    sSpecialBinCountBySocket[tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11350 SpecialBinCountBySocket", AnsiString(""));
                    sLowYield               [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11380 LowYield", AnsiString(""));
                    sArmYield               [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11381 ArmYield", AnsiString(""));
                    sSiteYield              [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11382 SiteYield", AnsiString(""));
                    sBySiteClean            [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11383 BySiteClean", AnsiString(""));
                    sByBinClean             [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "11384 ByBinClean", AnsiString(""));

                    sBinTrayLinked          [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "BinTrayLinked(11398)",  AnsiString("0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"));    //Ifor 20240730 add : BinTrayLinked
                    sBinLinked              [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "BinLinked(11399)",  AnsiString("0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;"));    //Ifor 20240730 add : BinLinked
                    sMagazineSetup          [tag]->CommaText=FormSysTools->CheckAndReadIniData(GroupName, "MagazineSetup(11400)", AnsiString("0"));
                }
            }
        }
    }
//    QQ2=MyQQTimer.LatchCycleTime()/1000.0;
//    Memo1->Lines->Add(" ReadFunctionDataB"+AnsiString(tag)+": "+AnsiString(QQ2)+" ="+AnsiString(QQ2-QQ1));
//    QQ1=QQ2;

    if(bHasNewSetupData==false)                                                 //這裡是為了跟舊版的相容, 所以新的功能不需要加在這邊
    {
        for(int i=0; i<iTestBinCount; i++)
        {                                                                       //QQQ
            GroupStr.sprintf("Category%d", i);                                  //kevin 20140317

            //Scan-------------
            BinSelect[tag].bScanner[i]=0;
            MyBinPanel[tag]->bScan[i]=BinSelect[tag].bScanner[i];

            if(IniConfig.bA30SetupTeachFunction && tag==OffT && LastSet.bNeedSetupTeach && (i>=1 && i<=iFixRight))  //JerryYang 20180921 Setup Teach功能
            {
                BinSelect[tag].iCatDataT3Pos[i]=FormSysTools->CheckAndReadIniData(GroupStr, "Bin", i);      //Bin 1..6 to Auto 123 Fix 123
            }
            else
            {
                BinSelect[tag].iCatDataT3Pos[i]=FormSysTools->CheckAndReadIniData(GroupStr, "Bin", (tag==OffT)?e3PosFix2:e3PosNoUse);      //Steven 20150209 : ePosFix1 --> ePosFix2 for HT9046LS + Rotate
            }

            if(BinSelect[tag].iCatDataT3Pos[i]<e3PosNoUse)
                BinSelect[tag].iCatDataT3Pos[i]= e3PosNoUse;

            if(USE_ROTATE==eCynRot && BinSelect[tag].iCatDataT3Pos[i]-1==iRotate_Out_Tray6) //kevin 20130812  氣缸版   //Steven 20121001 : 旋轉Kit
                BinSelect[tag].iCatDataT3Pos[i]= e3PosNoUse;

            if(AUTO3_IS_MAGAZINE==1 && BinSelect[tag].iCatDataT3Pos[i]==e3PosAuto3)         //JerryYang 20221207 : Magazine not use AUTO3
                BinSelect[tag].iCatDataT3Pos[i]=e3PosNoUse;

            if(CosFunction.bLoaderTrayToAuto1 && TrayForm.LoaderToEmptyColor[iRunStartMode]==2 && BinSelect[tag].iCatDataT3Pos[i]==e3PosAuto1)
                BinSelect[tag].iCatDataT3Pos[i]=e3PosNoUse;

            if(AUTO3_IS_MAGAZINE==1 && TestIF_File.iMagFixTrayType==1 && BinSelect[tag].iCatDataT3Pos[i]>=e3PosFix1 && BinSelect[tag].iCatDataT3Pos[i]<=e3PosFix6)  //JerryYang 20221215 : Magazine把fix區當buffer區功能
                BinSelect[tag].iCatDataT3Pos[i]=e3PosNoUse;

            if(TrayForm.iFixTrayMode==false && BinSelect[tag].iCatDataT3Pos[i]>e3PosFix3)   //Fix滿盤
                BinSelect[tag].iCatDataT3Pos[i]=e3PosNoUse;

            if(CosFunction.bUseTrayUpDownSet)                                   //wei 20160224 TSMC FIX UPDOWN
            {
                if(TrayForm.iFixTrayMode && BinSelect[tag].iCatDataT3Pos[i]==e3PosFix4 && TrayForm.bTrayUpDownSet[e3PosFix4-e3PosFix4]==false)
                    BinSelect[tag].iCatDataT3Pos[i]=e3PosNoUse;
                else if(TrayForm.iFixTrayMode && BinSelect[tag].iCatDataT3Pos[i]==e3PosFix5 && TrayForm.bTrayUpDownSet[e3PosFix5-e3PosFix4]==false)
                    BinSelect[tag].iCatDataT3Pos[i]=e3PosNoUse;
                else if(TrayForm.iFixTrayMode && BinSelect[tag].iCatDataT3Pos[i]==e3PosFix6 && TrayForm.bTrayUpDownSet[e3PosFix6-e3PosFix4]==false)
                    BinSelect[tag].iCatDataT3Pos[i]=e3PosNoUse;
            }

            if(TestIF_File.bEnableQASampling &&                                 //Steven 20190326 : QA Sampling
               BinSelect[tag].iCatDataT3Pos[i]==TestIF_File.iQASamplingT3Pos)
            {
                BinSelect[tag].iCatDataT3Pos[i]=e3PosNoUse;
            }

            MyBinPanel[tag]->BackT6PosTray[i][eBinNotUse+BinSelect[tag].iCatDataT3Pos[i]]=1;

            //Double Contact------
            if(Prod.bD22SupportMultiDoubleContact==false)                       //Steven 20170920 : 重新整理Double Contact選項
               //IniConfig.bD22VerifyMode==false)                               //Sam 20231117 : 整合到 QA 模式      //Sam 20221012 : 新增 VerifyMode 功能
            {
                BinSelect[tag].iDBContact[i]    =0;
            }
            else
            {
                BinSelect[tag].iDBContact[i]    =FormSysTools->CheckAndReadIniData(GroupStr, "Contact", 0);
            }
            MyBinPanel[tag]->i2Contact[i]       =BinSelect[tag].iDBContact[i];

            //Con. Fail--------
            BinSelect[tag].bConsFail[i]         =FormSysTools->CheckAndReadIniData(GroupStr, "Cons.Fail", 0);
            MyBinPanel[tag]->bConFail[i]        =BinSelect[tag].bConsFail[i];

            //Persent Enable------                                              //Steven 20140529 Start: Fail Persent & Count同時存在
            BinSelect[tag].bFailure[i]          =FormSysTools->CheckAndReadIniData(GroupStr, "Fail Percent", 0);
            MyBinPanel[tag]->bPersentEnable[i]  =BinSelect[tag].bFailure[i];

            //Persent Ignore------
            BinSelect[tag].iPersentIgnore[i]    =FormSysTools->CheckAndReadIniData(GroupStr, "Fail Percent Ignore", 0);
            MyBinPanel[tag]->iPersentIgnore[i]  =BinSelect[tag].iPersentIgnore[i];

            //Persent Number------
            BinSelect[tag].dFailureLimit[i]     =FormSysTools->CheckAndReadIniData(GroupStr, "Fail Percent Limit", 0.0);
            MyBinPanel[tag]->dPersentNumber[i]  =BinSelect[tag].dFailureLimit[i];

            //Count Enable------
            BinSelect[tag].bFailCountEnable[i]  =FormSysTools->CheckAndReadIniData(GroupStr, "Fail Count", 0);
            MyBinPanel[tag]->bCountEnable[i]    =BinSelect[tag].bFailCountEnable[i];

            //Count Ignore------
            BinSelect[tag].iFailCountIgnore[i]  =FormSysTools->CheckAndReadIniData(GroupStr, "Fail Count Ignore", 0);
            MyBinPanel[tag]->iCountIgnore[i]    =BinSelect[tag].iFailCountIgnore[i];

            //Count Number------
            BinSelect[tag].iFailCountLimit[i]   =FormSysTools->CheckAndReadIniData(GroupStr, "Fail Count Limit", 0.0);
            MyBinPanel[tag]->iCountNumber[i]    =BinSelect[tag].iFailCountLimit[i];

            BinSelect[tag].bSpecialBinByArm[i]          =FormSysTools->CheckAndReadIniData(GroupStr, "Special Bin By Arm", false);
            MyBinPanel[tag]->bSpecialBinByArm[i]        =BinSelect[tag].bSpecialBinByArm[i];

            BinSelect[tag].iSpecialBinCountByArm[i]     =FormSysTools->CheckAndReadIniData(GroupStr, "Special Bin Count By Arm", 0);
            MyBinPanel[tag]->iSpecialBinCountByArm[i]   =BinSelect[tag].iSpecialBinCountByArm[i];

            BinSelect[tag].bSpecialBinBySocket[i]       =FormSysTools->CheckAndReadIniData(GroupStr, "Special Bin By Socket", false);
            MyBinPanel[tag]->bSpecialBinBySocket[i]     =BinSelect[tag].bSpecialBinBySocket[i];

            BinSelect[tag].iSpecialBinCountBySocket[i]  =FormSysTools->CheckAndReadIniData(GroupStr, "Special Bin Count By Socket", 0.0);
            MyBinPanel[tag]->iSpecialBinCountBySocket[i]=BinSelect[tag].iSpecialBinCountBySocket[i];

            BinSelect[tag].bLowYield[i]     =FormSysTools->CheckAndReadIniData(GroupStr, "By Bin Low Yield", false);                    //Steven 20140828 : By Bin Yield Monitor
            MyBinPanel[tag]->bLowYield[i]   =BinSelect[tag].bLowYield[i];

            BinSelect[tag].bArmYield[i]     =FormSysTools->CheckAndReadIniData(GroupStr, "By Bin and Site Compare Arm Yield", false);   //Steven 20140828 : By Bin Arm Yield Monitor
            MyBinPanel[tag]->bArmYield[i]   =BinSelect[tag].bArmYield[i];

            BinSelect[tag].bSiteYield[i]    =FormSysTools->CheckAndReadIniData(GroupStr, "By Bin Compare Site Yield", false);           //Steven 20140828 : By Bin Site Yield Monitor
            MyBinPanel[tag]->bSiteYield[i]  =BinSelect[tag].bSiteYield[i];
        }

        for(int i=0; i<iTestBinCount; i++)
        {
            if(i<sBinDoubleContact[tag]->Count)
                sBinDoubleContact       [tag]->Strings[i]=BinSelect[tag].iDBContact[i];
            if(i<sBinConsFail[tag]->Count)
                sBinConsFail            [tag]->Strings[i]=(BinSelect[tag].bConsFail[i])?"1":"0";
            if(i<sBinEnableFail[tag]->Count)
                sBinEnableFail          [tag]->Strings[i]=(BinSelect[tag].bFailure[i]  )?"1":"0";
            if(i<sBinFailPercent[tag]->Count)
                sBinFailPercent         [tag]->Strings[i]=AnsiString(BinSelect[tag].dFailureLimit[i]);
            if(i<sBinFailIgnore[tag]->Count)
                sBinFailIgnore          [tag]->Strings[i]=AnsiString(BinSelect[tag].iPersentIgnore[i]);
            if(i<sBinCountEnable[tag]->Count)
                sBinCountEnable         [tag]->Strings[i]=(BinSelect[tag].bFailCountEnable[i])?"1":"0";
            if(i<sBinCountIgnore[tag]->Count)
                sBinCountIgnore         [tag]->Strings[i]=AnsiString(BinSelect[tag].iFailCountIgnore[i]);
            if(i<sBinCountNumber[tag]->Count)
                sBinCountNumber         [tag]->Strings[i]=AnsiString(BinSelect[tag].iFailCountLimit[i]);
            if(i<sSpecialBinByArm[tag]->Count)
                sSpecialBinByArm        [tag]->Strings[i]=(BinSelect[tag].bSpecialBinByArm[i]?"1":"0");
            if(i<sSpecialBinCountByArm[tag]->Count)
                sSpecialBinCountByArm   [tag]->Strings[i]=AnsiString(BinSelect[tag].iSpecialBinCountByArm[i]);
            if(i<sSpecialBinBySocket[tag]->Count)
                sSpecialBinBySocket     [tag]->Strings[i]=(BinSelect[tag].bSpecialBinBySocket[i]?"1":"0");
            if(i<sSpecialBinCountBySocket[tag]->Count)
                sSpecialBinCountBySocket[tag]->Strings[i]=AnsiString(BinSelect[tag].iSpecialBinCountBySocket[i]);
            if(i<sLowYield[tag]->Count)                                         //Steven 20140828 : By Bin Yield Monitor
                sLowYield               [tag]->Strings[i]=(BinSelect[tag].bLowYield[i]?"1":"0");
            if(i<sArmYield[tag]->Count)                                         //Steven 20140828 : By Bin Arm Yield Monitor
                sArmYield               [tag]->Strings[i]=(BinSelect[tag].bArmYield[i]?"1":"0");
            if(i<sSiteYield[tag]->Count)                                        //Steven 20140828 : By Bin Site Yield Monitor
                sSiteYield              [tag]->Strings[i]=(BinSelect[tag].bSiteYield[i]?"1":"0");
            if(i<sBinTraySetT3Pos[tag]->Count)
                sBinTraySetT3Pos        [tag]->Strings[i]=AnsiString(BinSelect[tag].iCatDataT3Pos[i]);
        }
        SaveFunctionData(tag);
    }
    else
    {
        for(int i=0; i<iTestBinCount; i++)
        {
            if(IniConfig.bD22SupportMultiDoubleContact==false &&                //Steven 20170920 : 重新整理Double Contact選項
               IniConfig.bD22VerifyMode==false)                                 //Sam 20221012 : 新增 VerifyMode 功能
            {
                sBinDoubleContact[tag]->Clear();
            }

            BinSelect[tag].iDBContact[i]                =(i<sBinDoubleContact[tag]->Count       )?atoi(sBinDoubleContact        [tag]->Strings[i].c_str()):false;
            BinSelect[tag].bConsFail[i]                 =(i<sBinConsFail[tag]->Count            )?atoi(sBinConsFail             [tag]->Strings[i].c_str()):false;
            BinSelect[tag].bFailure[i]                  =(i<sBinEnableFail[tag]->Count          )?atoi(sBinEnableFail           [tag]->Strings[i].c_str()):false;
            BinSelect[tag].dFailureLimit[i]             =(i<sBinFailPercent[tag]->Count         )?atof(sBinFailPercent          [tag]->Strings[i].c_str()):0.0;         //JerryYang 20160613 修正By Bin Yield要能設定到小數點
            BinSelect[tag].iPersentIgnore[i]            =(i<sBinFailIgnore[tag]->Count          )?atoi(sBinFailIgnore           [tag]->Strings[i].c_str()):0;
            BinSelect[tag].bFailCountEnable[i]          =(i<sBinCountEnable[tag]->Count         )?atoi(sBinCountEnable          [tag]->Strings[i].c_str()):false;
            BinSelect[tag].iFailCountIgnore[i]          =(i<sBinCountIgnore[tag]->Count         )?atoi(sBinCountIgnore          [tag]->Strings[i].c_str()):0;
            BinSelect[tag].iFailCountLimit[i]           =(i<sBinCountNumber[tag]->Count         )?atoi(sBinCountNumber          [tag]->Strings[i].c_str()):0;
            BinSelect[tag].bSpecialBinByArm[i]          =(i<sSpecialBinByArm[tag]->Count        )?atoi(sSpecialBinByArm         [tag]->Strings[i].c_str()):false;
            BinSelect[tag].iSpecialBinCountByArm[i]     =(i<sSpecialBinCountByArm[tag]->Count   )?atoi(sSpecialBinCountByArm    [tag]->Strings[i].c_str()):0;
            BinSelect[tag].bSpecialBinBySocket[i]       =(i<sSpecialBinBySocket[tag]->Count     )?atoi(sSpecialBinBySocket      [tag]->Strings[i].c_str()):false;
            BinSelect[tag].iSpecialBinCountBySocket[i]  =(i<sSpecialBinCountBySocket[tag]->Count)?atoi(sSpecialBinCountBySocket [tag]->Strings[i].c_str()):0;
            BinSelect[tag].bLowYield[i]                 =(i<sLowYield[tag]->Count               )?atoi(sLowYield                [tag]->Strings[i].c_str()):false;
            BinSelect[tag].bArmYield[i]                 =(i<sArmYield[tag]->Count               )?atoi(sArmYield                [tag]->Strings[i].c_str()):false;
            BinSelect[tag].bSiteYield[i]                =(i<sSiteYield[tag]->Count              )?atoi(sSiteYield               [tag]->Strings[i].c_str()):false;
            BinSelect[tag].iAutoCleanByBin[i]           =(i<sBySiteClean[tag]->Count            )?atoi(sBySiteClean             [tag]->Strings[i].c_str()):0;
            BinSelect[tag].iAutoCleanBySite[i]          =(i<sByBinClean[tag]->Count             )?atoi(sByBinClean              [tag]->Strings[i].c_str()):0;
            BinSelect[tag].iCatDataT3Pos[i]             =(i<sBinTraySetT3Pos[tag]->Count        )?atoi(sBinTraySetT3Pos         [tag]->Strings[i].c_str()):e3PosNoUse;
            BinSelect[tag].bBinTrayLinked[i]            =(i<sBinTrayLinked[tag]->Count          )?atoi(sBinTrayLinked           [tag]->Strings[i].c_str()):false;

            if(CosFunction.bBySiteByBinPercentCompare==true &&                  //JerryYang 20170712 (Steven) by site by bin compare percent
               CosFunction.bByBinAlarmFromYieldForm==false)                     //jou 20180113 (Steven) : By Site By Bin Percent Compare From Yield form
            {
                BinSelect[tag].bSpecBinBySiteCompareEnable[i]       =(i<sSpecBinBySiteCompareEnable[tag]->Count       )?atoi(sSpecBinBySiteCompareEnable[tag]->Strings[i].c_str()):false;
                BinSelect[tag].iSpecBinBySiteCompareIgnore[i]       =(i<sSpecBinBySiteCompareIgnore[tag]->Count       )?atoi(sSpecBinBySiteCompareIgnore[tag]->Strings[i].c_str()):0;
                BinSelect[tag].dSpecBinBySiteComparePercent[i]      =(i<sSpecBinBySiteComparePercent[tag]->Count      )?atof(sSpecBinBySiteComparePercent[tag]->Strings[i].c_str()):0.0;
                BinSelect[tag].bSpecBinByArmPerSiteCompareEnable[i] =(i<sSpecBinByArmPerSiteCompareEnable[tag]->Count )?atoi(sSpecBinByArmPerSiteCompareEnable[tag]->Strings[i].c_str()):false;
                BinSelect[tag].iSpecBinByArmPerSiteCompareIgnore[i] =(i<sSpecBinByArmPerSiteCompareIgnore[tag]->Count )?atoi(sSpecBinByArmPerSiteCompareIgnore[tag]->Strings[i].c_str()):0;
                BinSelect[tag].dSpecBinByArmPerSiteComparePercent[i]=(i<sSpecBinByArmPerSiteComparePercent[tag]->Count)?atof(sSpecBinByArmPerSiteComparePercent[tag]->Strings[i].c_str()):0.0;
            }

            if(i<sBinTraySetT3Pos[tag]->Count)
            {
                iData=atoi(sBinTraySetT3Pos[tag]->Strings[i].c_str());

                if(iData<e3PosNoUse)
                    BinSelect[tag].iCatDataT3Pos[i]=e3PosNoUse;

                iT3=BinSelect[tag].iCatDataT3Pos[i]-1;
                if(iT3>=e3Auto1)
                {
                    iT6=iTo6Unload[iT3];
                    if(Prod.iTrayType[iT6]==tNotUse)
                        iData=e3PosNoUse;

                    if(CosFunction.bLoaderTrayToAuto1 && TrayForm.LoaderToEmptyColor[iRunStartMode]==2)
                    {
                        if(BinSelect[tag].iCatDataT3Pos[i]==e3PosAuto1)
                            iData=e3PosNoUse;
                    }

                    if(AUTO3_IS_MAGAZINE==1)
                    {
                        if(BinSelect[tag].iCatDataT3Pos[i]==e3PosAuto3)         //JerryYang 20221207 : Magazine not use AUTO3
                            iData=e3PosNoUse;

                        if(TestIF_File.iMagFixTrayType==1 && iT6>=iFixMin && iT6<=iFixRightHalf)
                            iData=e3PosNoUse;
                    }
                }
                else
                {
                    iData=e3PosNoUse;
                }

                BinSelect[tag].iCatDataT3Pos[i]=iData;
                sBinTraySetT3Pos[tag]->Strings[i]=AnsiString(iData);
            }
            else
            {
                BinSelect[tag].iCatDataT3Pos[i]=e3PosNoUse;
            }
        }

        for(int i=0; i<iTestBinCount; i++)
        {
            if(TestIF_File.bEnableQASampling &&                                 //Steven 20190326 : QA Sampling
               BinSelect[tag].iCatDataT3Pos[i]==TestIF_File.iQASamplingT3Pos)
            {
                BinSelect[tag].iCatDataT3Pos[i]=e3PosNoUse;
            }

            int iUnloader=0;
            iUnloader=iTo6PosUnload[BinSelect[tag].iCatDataT3Pos[i]];           //JerryYang 20230915 : test     //JerryYang 20230926

            MyBinPanel[tag]->BackT6PosTray[i][eBinNotUse+iUnloader]=1;
            MyBinPanel[tag]->i2Contact[i]               =BinSelect[tag].iDBContact[i];
            MyBinPanel[tag]->bConFail[i]                =BinSelect[tag].bConsFail[i];
            MyBinPanel[tag]->bPersentEnable[i]          =BinSelect[tag].bFailure[i];
            MyBinPanel[tag]->iPersentIgnore[i]          =BinSelect[tag].iPersentIgnore[i];
            MyBinPanel[tag]->dPersentNumber[i]          =BinSelect[tag].dFailureLimit[i];
            MyBinPanel[tag]->bCountEnable[i]            =BinSelect[tag].bFailCountEnable[i];
            MyBinPanel[tag]->iCountIgnore[i]            =BinSelect[tag].iFailCountIgnore[i];
            MyBinPanel[tag]->iCountNumber[i]            =BinSelect[tag].iFailCountLimit[i];
            MyBinPanel[tag]->bSpecialBinByArm[i]        =BinSelect[tag].bSpecialBinByArm[i];
            MyBinPanel[tag]->iSpecialBinCountByArm[i]   =BinSelect[tag].iSpecialBinCountByArm[i];
            MyBinPanel[tag]->bSpecialBinBySocket[i]     =BinSelect[tag].bSpecialBinBySocket[i];
            MyBinPanel[tag]->iSpecialBinCountBySocket[i]=BinSelect[tag].iSpecialBinCountBySocket[i];
            MyBinPanel[tag]->bLowYield[i]               =BinSelect[tag].bLowYield[i];
            MyBinPanel[tag]->bArmYield[i]               =BinSelect[tag].bArmYield[i];
            MyBinPanel[tag]->bSiteYield[i]              =BinSelect[tag].bSiteYield[i];
            MyBinPanel[tag]->iAutoCleanByBin[i]         =BinSelect[tag].iAutoCleanByBin[i];             //Steven 20160308 : By Bin count do auto clean
            MyBinPanel[tag]->iAutoCleanBySite[i]        =BinSelect[tag].iAutoCleanBySite[i];            //Steven 20160308 : By Bin pre site count do auto clean

            if(CosFunction.bBySiteByBinPercentCompare==true &&                  //JerryYang 20170712 (Steven) by site by bin compare percent
               CosFunction.bByBinAlarmFromYieldForm==false)                     //jou 20180113 (Steven) : By Site By Bin Percent Compare From Yield form
            {
                MyBinPanel[tag]->bSpecBinBySiteCompareEnable[i]       =BinSelect[tag].bSpecBinBySiteCompareEnable[i];
                MyBinPanel[tag]->iSpecBinBySiteCompareIgnore[i]       =BinSelect[tag].iSpecBinBySiteCompareIgnore[i];
                MyBinPanel[tag]->dSpecBinBySiteComparePercent[i]      =BinSelect[tag].dSpecBinBySiteComparePercent[i];
                MyBinPanel[tag]->bSpecBinByArmPerSiteCompareEnable[i] =BinSelect[tag].bSpecBinByArmPerSiteCompareEnable[i];
                MyBinPanel[tag]->iSpecBinByArmPerSiteCompareIgnore[i] =BinSelect[tag].iSpecBinByArmPerSiteCompareIgnore[i];
                MyBinPanel[tag]->dSpecBinByArmPerSiteComparePercent[i]=BinSelect[tag].dSpecBinByArmPerSiteComparePercent[i];
            }
        }

        MyBinPanel[tag]->edBinSetT3Pos->Text                =sBinTraySetT3Pos[tag]->CommaText;
        MyBinPanel[tag]->edBinSetting2Contact->Text         =sBinDoubleContact[tag]->CommaText;
        MyBinPanel[tag]->edBinSettingConFail->Text          =sBinConsFail[tag]->CommaText;
        MyBinPanel[tag]->edBinSettingFailPersentEnable->Text=sBinEnableFail[tag]->CommaText;
        MyBinPanel[tag]->edBinSettingFailPersentIgnore->Text=sBinFailPercent[tag]->CommaText;
        MyBinPanel[tag]->edBinSettingFailPersentNumber->Text=sBinFailIgnore[tag]->CommaText;
        MyBinPanel[tag]->edBinSettingFailCountEnable->Text  =sBinCountEnable[tag]->CommaText;
        MyBinPanel[tag]->edBinSettingFailCountIgnore->Text  =sBinCountIgnore[tag]->CommaText;
        MyBinPanel[tag]->edBinSettingFailCountNumber->Text  =sBinCountNumber[tag]->CommaText;
        MyBinPanel[tag]->edSpecialBinByArm->Text            =sSpecialBinByArm[tag]->CommaText;
        MyBinPanel[tag]->edSpecialBinCountByArm->Text       =sSpecialBinCountByArm[tag]->CommaText;
        MyBinPanel[tag]->edSpecialBinBySocket->Text         =sSpecialBinBySocket[tag]->CommaText;
        MyBinPanel[tag]->edSpecialBinCountBySocket->Text    =sSpecialBinCountBySocket[tag]->CommaText;
        MyBinPanel[tag]->edLowYield->Text                   =sLowYield[tag]->CommaText;
        MyBinPanel[tag]->edArmYield->Text                   =sArmYield[tag]->CommaText;
        MyBinPanel[tag]->edSiteYield->Text                  =sSiteYield[tag]->CommaText;
        MyBinPanel[tag]->edBySiteClean->Text                =sBySiteClean[tag]->CommaText;
        MyBinPanel[tag]->edByBinClean->Text                 =sByBinClean[tag]->CommaText;

        MyBinPanel[tag]->edSpecBinBySiteCompareEnable->Text         =sSpecBinBySiteCompareEnable[tag]->CommaText;
        MyBinPanel[tag]->edSpecBinBySiteCompareIgnore->Text         =sSpecBinBySiteCompareIgnore[tag]->CommaText;
        MyBinPanel[tag]->edSpecBinBySiteComparePercent->Text        =sSpecBinBySiteComparePercent[tag]->CommaText;
        MyBinPanel[tag]->edSpecBinByArmPerSiteCompareEnable->Text   =sSpecBinByArmPerSiteCompareEnable[tag]->CommaText;
        MyBinPanel[tag]->edSpecBinByArmPerSiteCompareIgnore->Text   =sSpecBinByArmPerSiteCompareIgnore[tag]->CommaText;
        MyBinPanel[tag]->edSpecBinByArmPerSiteComparePercent->Text  =sSpecBinByArmPerSiteComparePercent[tag]->CommaText;
        MyBinPanel[tag]->edBinSetT6Link->Text                       =sBinTrayLinked[tag]->CommaText;

        SaveFunctionData(tag);                                                  //wei 20161124 修改存檔錯誤的問題
    }
}
//---------------------------------------------------------------------------
void TfBinSel::SaveFunctionData(int tag)
{
    AnsiString GroupName;
    if(tag==eBinRT)
    {
        if((IniConfig.bA02BinModelPrime==true  && IniConfig.bFTBin2RTBin==true && (iBinModelPrime==0 || CosFunction.bDisableRTBinSet)) ||        //Steven 20160623 : 當IniConfig.bFTBin2RTBin==true && 讀檔時, RT Bin要跟FT Bin一樣
           (IniConfig.bA02BinModelPrime==false && (IniConfig.bFTBin2RTBin==true || CosFunction.bDisableRTBinSet)))
        {
            GroupName="Bin Func FT";
            FormSysTools->WriteIniData(GroupName, "NewBinFormat",               1);   //kevin 20160928 使用新的bin format
            FormSysTools->WriteIniData(GroupName, "3617 BinTraySetting",        sBinTraySetT3Pos        [tag]->CommaText);
            TransferBinTrayStrToName(tag);
            FormSysTools->WriteIniData(GroupName, "3617 BinTraySetName",        sBinTraySetT3PosName    [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "3636 BinDoubleContac",       sBinDoubleContact       [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "3656 BinType",               sBinType                [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "3676 BinConsFail",           sBinConsFail            [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10641 BinEnableFail",        sBinEnableFail          [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10642 BinFailPercent",       sBinFailPercent         [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10643 BinFailIgnore",        sBinFailIgnore          [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10644 BinCountEnable",       sBinCountEnable         [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10646 BinCountIgnore",       sBinCountIgnore         [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10645 BinCountNumber",       sBinCountNumber         [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10647 SpecialBinByArm",      sSpecialBinByArm        [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10648 SpecialBinCountByArm", sSpecialBinCountByArm   [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10649 SpecialBinBySocket",   sSpecialBinBySocket     [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10650 SpecialBinCountBySocket", sSpecialBinCountBySocket[tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10680 LowYield",             sLowYield               [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10681 ArmYield",             sArmYield               [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10682 SiteYield",            sSiteYield              [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10683 BySiteClean",          sBySiteClean            [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10684 ByBinClean",           sByBinClean             [tag]->CommaText);

            if(CosFunction.bBySiteByBinPercentCompare==true &&                  //JerryYang 20170712 (Steven) by site by bin compare percent
               CosFunction.bByBinAlarmFromYieldForm==false)                     //jou 20180113 (Steven) : By Site By Bin Percent Compare From Yield form
            {
                FormSysTools->WriteIniData(GroupName, "10685 SpecBinBySiteCompareEnable",        sSpecBinBySiteCompareEnable         [tag]->CommaText);
                FormSysTools->WriteIniData(GroupName, "10686 SpecBinBySiteCompareIgnore",        sSpecBinBySiteCompareIgnore         [tag]->CommaText);
                FormSysTools->WriteIniData(GroupName, "10687 SpecBinBySiteComparePercent",       sSpecBinBySiteComparePercent        [tag]->CommaText);
                FormSysTools->WriteIniData(GroupName, "10688 SpecBinByArmPerSiteCompareEnable",  sSpecBinByArmPerSiteCompareEnable   [tag]->CommaText);
                FormSysTools->WriteIniData(GroupName, "10689 SpecBinByArmPerSiteCompareIgnore",  sSpecBinByArmPerSiteCompareIgnore   [tag]->CommaText);
                FormSysTools->WriteIniData(GroupName, "10690 SpecBinByArmPerSiteComparePercent", sSpecBinByArmPerSiteComparePercent  [tag]->CommaText);
            }

            FormSysTools->WriteIniData(GroupName, "BinTrayLinked(10698)", sBinTrayLinked        [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "BinLinked(10699)",     sBinLinked            [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "MagazineSetup(10700)", sMagazineSetup        [tag]->CommaText);
        }
        else
        {
            GroupName="Bin Func RT";
            FormSysTools->WriteIniData(GroupName, "NewBinFormat",               1);   //kevin 20160928 使用新的bin format
            FormSysTools->WriteIniData(GroupName, "3717 BinTraySetting",        sBinTraySetT3Pos        [tag]->CommaText);
            TransferBinTrayStrToName(tag);
            FormSysTools->WriteIniData(GroupName, "3717 BinTraySetName",        sBinTraySetT3PosName    [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "3718 BinDoubleContac",       sBinDoubleContact       [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "3719 BinType",               sBinType                [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "3720 BinConsFail",           sBinConsFail            [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10741 BinEnableFail",        sBinEnableFail          [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10742 BinFailPercent",       sBinFailPercent         [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10743 BinFailIgnore",        sBinFailIgnore          [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10744 BinCountEnable",       sBinCountEnable         [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10745 BinCountNumber",       sBinCountNumber         [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10746 BinCountIgnore",       sBinCountIgnore         [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10747 SpecialBinByArm",      sSpecialBinByArm        [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10748 SpecialBinCountByArm", sSpecialBinCountByArm   [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10749 SpecialBinBySocket",   sSpecialBinBySocket     [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10750 SpecialBinCountBySocket", sSpecialBinCountBySocket[tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10780 LowYield",             sLowYield               [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10781 ArmYield",             sArmYield               [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10782 SiteYield",            sSiteYield              [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10783 BySiteClean",          sBySiteClean            [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10784 ByBinClean",           sByBinClean             [tag]->CommaText);

            if(CosFunction.bBySiteByBinPercentCompare==true &&                  //JerryYang 20170712 (Steven) by site by bin compare percent
               CosFunction.bByBinAlarmFromYieldForm==false)                     //jou 20180113 (Steven) : By Site By Bin Percent Compare From Yield form
            {
                FormSysTools->WriteIniData(GroupName, "10785 SpecBinBySiteCompareEnable",        sSpecBinBySiteCompareEnable         [tag]->CommaText);
                FormSysTools->WriteIniData(GroupName, "10786 SpecBinBySiteCompareIgnore",        sSpecBinBySiteCompareIgnore         [tag]->CommaText);
                FormSysTools->WriteIniData(GroupName, "10787 SpecBinBySiteComparePercent",       sSpecBinBySiteComparePercent        [tag]->CommaText);
                FormSysTools->WriteIniData(GroupName, "10788 SpecBinByArmPerSiteCompareEnable",  sSpecBinByArmPerSiteCompareEnable   [tag]->CommaText);
                FormSysTools->WriteIniData(GroupName, "10789 SpecBinByArmPerSiteCompareIgnore",  sSpecBinByArmPerSiteCompareIgnore   [tag]->CommaText);
                FormSysTools->WriteIniData(GroupName, "10790 SpecBinByArmPerSiteComparePercent", sSpecBinByArmPerSiteComparePercent  [tag]->CommaText);
            }

            FormSysTools->WriteIniData(GroupName, "BinTrayLinked(10798)", sBinTrayLinked        [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "BinLinked(10799)",     sBinLinked            [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "MagazineSetup(10800)", sMagazineSetup        [tag]->CommaText);
        }
    }
    else if(tag==eBinFT)
    {
        GroupName="Bin Func FT";
        FormSysTools->WriteIniData(GroupName, "NewBinFormat",               1); //kevin 20160928 使用新的bin format
        FormSysTools->WriteIniData(GroupName, "3617 BinTraySetting",        sBinTraySetT3Pos        [tag]->CommaText);
        TransferBinTrayStrToName(tag);
        FormSysTools->WriteIniData(GroupName, "3617 BinTraySetName",        sBinTraySetT3PosName    [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "3636 BinDoubleContac",       sBinDoubleContact       [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "3656 BinType",               sBinType                [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "3676 BinConsFail",           sBinConsFail            [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "10641 BinEnableFail",        sBinEnableFail          [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "10642 BinFailPercent",       sBinFailPercent         [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "10643 BinFailIgnore",        sBinFailIgnore          [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "10644 BinCountEnable",       sBinCountEnable         [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "10645 BinCountNumber",       sBinCountNumber         [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "10646 BinCountIgnore",       sBinCountIgnore         [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "10647 SpecialBinByArm",      sSpecialBinByArm        [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "10648 SpecialBinCountByArm", sSpecialBinCountByArm   [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "10649 SpecialBinBySocket",   sSpecialBinBySocket     [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "10650 SpecialBinCountBySocket", sSpecialBinCountBySocket[tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "10680 LowYield",             sLowYield               [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "10681 ArmYield",             sArmYield               [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "10682 SiteYield",            sSiteYield              [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "10683 BySiteClean",          sBySiteClean            [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "10684 ByBinClean",           sByBinClean             [tag]->CommaText);

        if(CosFunction.bBySiteByBinPercentCompare==true &&                      //JerryYang 20170712 (Steven) by site by bin compare percent
           CosFunction.bByBinAlarmFromYieldForm==false)                         //jou 20180113 (Steven) : By Site By Bin Percent Compare From Yield form
        {
            FormSysTools->WriteIniData(GroupName, "10685 SpecBinBySiteCompareEnable",        sSpecBinBySiteCompareEnable         [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10686 SpecBinBySiteCompareIgnore",        sSpecBinBySiteCompareIgnore         [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10687 SpecBinBySiteComparePercent",       sSpecBinBySiteComparePercent        [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10688 SpecBinByArmPerSiteCompareEnable",  sSpecBinByArmPerSiteCompareEnable   [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10689 SpecBinByArmPerSiteCompareIgnore",  sSpecBinByArmPerSiteCompareIgnore   [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10690 SpecBinByArmPerSiteComparePercent", sSpecBinByArmPerSiteComparePercent  [tag]->CommaText);
        }

        FormSysTools->WriteIniData(GroupName, "BinTrayLinked(10698)", sBinTrayLinked        [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "BinLinked(10699)",     sBinLinked            [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "MagazineSetup(10700)", sMagazineSetup        [tag]->CommaText);
    }
    else if(tag==eBinOffLine)
    {
        GroupName="Bin Func OffLine";
        FormSysTools->WriteIniData(GroupName, "NewBinFormat",               1); //kevin 20160928 使用新的bin format
        FormSysTools->WriteIniData(GroupName, "3722 BinTraySetting",        sBinTraySetT3Pos        [tag]->CommaText);
        TransferBinTrayStrToName(tag);
        FormSysTools->WriteIniData(GroupName, "3722 BinTraySetName",        sBinTraySetT3PosName    [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "3723 BinDoubleContac",       sBinDoubleContact       [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "3724 BinType",               sBinType                [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "3725 BinConsFail",           sBinConsFail            [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "10941 BinEnableFail",        sBinEnableFail          [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "10942 BinFailPercent",       sBinFailPercent         [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "10943 BinFailIgnore",        sBinFailIgnore          [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "10944 BinCountEnable",       sBinCountEnable         [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "10945 BinCountNumber",       sBinCountNumber         [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "10946 BinCountIgnore",       sBinCountIgnore         [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "10947 SpecialBinByArm",      sSpecialBinByArm        [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "10948 SpecialBinCountByArm", sSpecialBinCountByArm   [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "10949 SpecialBinBySocket",   sSpecialBinBySocket     [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "10950 SpecialBinCountBySocket", sSpecialBinCountBySocket[tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "10980 LowYield",             sLowYield               [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "10981 ArmYield",             sArmYield               [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "10982 SiteYield",            sSiteYield              [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "10983 BySiteClean",          sBySiteClean            [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "10984 ByBinClean",           sByBinClean             [tag]->CommaText);

        if(CosFunction.bBySiteByBinPercentCompare==true &&                      //JerryYang 20170712 (Steven) by site by bin compare percent
           CosFunction.bByBinAlarmFromYieldForm==false)                         //jou 20180113 (Steven) : By Site By Bin Percent Compare From Yield form
        {
            FormSysTools->WriteIniData(GroupName, "10985 SpecBinBySiteCompareEnable",        sSpecBinBySiteCompareEnable         [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10986 SpecBinBySiteCompareIgnore",        sSpecBinBySiteCompareIgnore         [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10987 SpecBinBySiteComparePercent",       sSpecBinBySiteComparePercent        [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10988 SpecBinByArmPerSiteCompareEnable",  sSpecBinByArmPerSiteCompareEnable   [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10989 SpecBinByArmPerSiteCompareIgnore",  sSpecBinByArmPerSiteCompareIgnore   [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "10990 SpecBinByArmPerSiteComparePercent", sSpecBinByArmPerSiteComparePercent  [tag]->CommaText);
        }

        FormSysTools->WriteIniData(GroupName, "BinTrayLinked(10998)", sBinTrayLinked        [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "BinLinked(10999)",     sBinLinked            [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "MagazineSetup(11000)", sMagazineSetup        [tag]->CommaText);
    }
    else if(tag==eBinFT_ART)
    {
        GroupName="Bin Func ART_FT";                                            //wei 20161124 修改eBinFT_ART存成Bin Func ART_RT問題
        FormSysTools->WriteIniData(GroupName, "NewBinFormat",               1); //kevin 20160928 使用新的bin format
        FormSysTools->WriteIniData(GroupName, "3801 BinTraySetting",        sBinTraySetT3Pos        [tag]->CommaText);
        TransferBinTrayStrToName(tag);
        FormSysTools->WriteIniData(GroupName, "3801 BinTraySetName",        sBinTraySetT3PosName    [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "3802 BinDoubleContac",       sBinDoubleContact       [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "3803 BinType",               sBinType                [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "3804 BinConsFail",           sBinConsFail            [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11041 BinEnableFail",        sBinEnableFail          [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11042 BinFailPercent",       sBinFailPercent         [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11043 BinFailIgnore",        sBinFailIgnore          [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11044 BinCountEnable",       sBinCountEnable         [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11045 BinCountNumber",       sBinCountNumber         [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11046 BinCountIgnore",       sBinCountIgnore         [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11047 SpecialBinByArm",      sSpecialBinByArm        [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11048 SpecialBinCountByArm", sSpecialBinCountByArm   [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11049 SpecialBinBySocket",   sSpecialBinBySocket     [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11050 SpecialBinCountBySocket", sSpecialBinCountBySocket[tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11080 LowYield",             sLowYield               [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11081 ArmYield",             sArmYield               [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11082 SiteYield",            sSiteYield              [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11083 BySiteClean",          sBySiteClean            [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11084 ByBinClean",           sByBinClean             [tag]->CommaText);

        if(CosFunction.bBySiteByBinPercentCompare==true &&                      //JerryYang 20170712 (Steven) by site by bin compare percent
           CosFunction.bByBinAlarmFromYieldForm==false)                         //jou 20180113 (Steven) : By Site By Bin Percent Compare From Yield form
        {
            FormSysTools->WriteIniData(GroupName, "11085 SpecBinBySiteCompareEnable",        sSpecBinBySiteCompareEnable         [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "11086 SpecBinBySiteCompareIgnore",        sSpecBinBySiteCompareIgnore         [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "11087 SpecBinBySiteComparePercent",       sSpecBinBySiteComparePercent        [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "11088 SpecBinByArmPerSiteCompareEnable",  sSpecBinByArmPerSiteCompareEnable   [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "11089 SpecBinByArmPerSiteCompareIgnore",  sSpecBinByArmPerSiteCompareIgnore   [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "11090 SpecBinByArmPerSiteComparePercent", sSpecBinByArmPerSiteComparePercent  [tag]->CommaText);
        }

        FormSysTools->WriteIniData(GroupName, "BinTrayLinked(11098)", sBinTrayLinked        [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "BinLinked(11099)",     sBinLinked            [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "MagazineSetup(11100)", sMagazineSetup        [tag]->CommaText);
    }
    else if(tag==eBinRT_ART)
    {
        GroupName="Bin Func ART_RT";                                            //wei 20161124 修改eBinRT_ART存成Bin Func ART_FT問題
        FormSysTools->WriteIniData(GroupName, "NewBinFormat",               1); //kevin 20160928 使用新的bin format
        FormSysTools->WriteIniData(GroupName, "3901 BinTraySetting",        sBinTraySetT3Pos        [tag]->CommaText);
        TransferBinTrayStrToName(tag);
        FormSysTools->WriteIniData(GroupName, "3901 BinTraySetName",        sBinTraySetT3PosName    [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "3902 BinDoubleContac",       sBinDoubleContact       [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "3903 BinType",               sBinType                [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "3904 BinConsFail",           sBinConsFail            [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11141 BinEnableFail",        sBinEnableFail          [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11142 BinFailPercent",       sBinFailPercent         [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11143 BinFailIgnore",        sBinFailIgnore          [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11144 BinCountEnable",       sBinCountEnable         [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11145 BinCountNumber",       sBinCountNumber         [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11146 BinCountIgnore",       sBinCountIgnore         [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11147 SpecialBinByArm",      sSpecialBinByArm        [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11148 SpecialBinCountByArm", sSpecialBinCountByArm   [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11149 SpecialBinBySocket",   sSpecialBinBySocket     [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11150 SpecialBinCountBySocket", sSpecialBinCountBySocket[tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11180 LowYield",             sLowYield               [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11181 ArmYield",             sArmYield               [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11182 SiteYield",            sSiteYield              [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11183 BySiteClean",          sBySiteClean            [tag]->CommaText);   //wei 20161124 11083 --> 11183
        FormSysTools->WriteIniData(GroupName, "11184 ByBinClean",           sByBinClean             [tag]->CommaText);   //wei 20161124 11084 --> 11184

        FormSysTools->WriteIniData(GroupName, "BinTrayLinked(11198)", sBinTrayLinked        [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "BinLinked(11199)",     sBinLinked            [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "MagazineSetup(11200)", sMagazineSetup        [tag]->CommaText);
    }
    else if(tag==eBinFT_MRT)
    {
        GroupName="Bin Func MRT_FT";                                            //Ifor 20170316 (wei) add KYEC MRT Mode
        FormSysTools->WriteIniData(GroupName, "NewBinFormat",               1);
        FormSysTools->WriteIniData(GroupName, "4001 BinTraySetting",        sBinTraySetT3Pos        [tag]->CommaText);
        TransferBinTrayStrToName(tag);
        FormSysTools->WriteIniData(GroupName, "4001 BinTraySetName",        sBinTraySetT3PosName    [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "4002 BinDoubleContac",       sBinDoubleContact       [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "4003 BinType",               sBinType                [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "4004 BinConsFail",           sBinConsFail            [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11241 BinEnableFail",        sBinEnableFail          [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11242 BinFailPercent",       sBinFailPercent         [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11243 BinFailIgnore",        sBinFailIgnore          [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11244 BinCountEnable",       sBinCountEnable         [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11245 BinCountNumber",       sBinCountNumber         [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11246 BinCountIgnore",       sBinCountIgnore         [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11247 SpecialBinByArm",      sSpecialBinByArm        [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11248 SpecialBinCountByArm", sSpecialBinCountByArm   [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11249 SpecialBinBySocket",   sSpecialBinBySocket     [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11250 SpecialBinCountBySocket", sSpecialBinCountBySocket[tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11280 LowYield",             sLowYield               [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11281 ArmYield",             sArmYield               [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11282 SiteYield",            sSiteYield              [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11283 BySiteClean",          sBySiteClean            [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11284 ByBinClean",           sByBinClean             [tag]->CommaText);

        if(CosFunction.bBySiteByBinPercentCompare==true &&                      //JerryYang 20170712 (Steven) by site by bin compare percent
           CosFunction.bByBinAlarmFromYieldForm==false)                         //jou 20180113 (Steven) : By Site By Bin Percent Compare From Yield form
        {
            FormSysTools->WriteIniData(GroupName, "11285 SpecBinBySiteCompareEnable",        sSpecBinBySiteCompareEnable         [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "11286 SpecBinBySiteCompareIgnore",        sSpecBinBySiteCompareIgnore         [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "11287 SpecBinBySiteComparePercent",       sSpecBinBySiteComparePercent        [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "11288 SpecBinByArmPerSiteCompareEnable",  sSpecBinByArmPerSiteCompareEnable   [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "11289 SpecBinByArmPerSiteCompareIgnore",  sSpecBinByArmPerSiteCompareIgnore   [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "11290 SpecBinByArmPerSiteComparePercent", sSpecBinByArmPerSiteComparePercent  [tag]->CommaText);
        }

        FormSysTools->WriteIniData(GroupName, "BinTrayLinked(11298)", sBinTrayLinked        [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "BinLinked(11299)",     sBinLinked            [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "MagazineSetup(11300)", sMagazineSetup        [tag]->CommaText);
    }
    else if(tag==eBinRT_MRT)
    {
        GroupName="Bin Func MRT_RT";
        FormSysTools->WriteIniData(GroupName, "NewBinFormat",               1);
        FormSysTools->WriteIniData(GroupName, "4101 BinTraySetting",        sBinTraySetT3Pos        [tag]->CommaText);
        TransferBinTrayStrToName(tag);
        FormSysTools->WriteIniData(GroupName, "4101 BinTraySetName",        sBinTraySetT3PosName    [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "4102 BinDoubleContac",       sBinDoubleContact       [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "4103 BinType",               sBinType                [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "4104 BinConsFail",           sBinConsFail            [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11341 BinEnableFail",        sBinEnableFail          [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11342 BinFailPercent",       sBinFailPercent         [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11343 BinFailIgnore",        sBinFailIgnore          [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11344 BinCountEnable",       sBinCountEnable         [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11345 BinCountNumber",       sBinCountNumber         [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11346 BinCountIgnore",       sBinCountIgnore         [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11347 SpecialBinByArm",      sSpecialBinByArm        [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11348 SpecialBinCountByArm", sSpecialBinCountByArm   [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11349 SpecialBinBySocket",   sSpecialBinBySocket     [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11350 SpecialBinCountBySocket", sSpecialBinCountBySocket[tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11380 LowYield",             sLowYield               [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11381 ArmYield",             sArmYield               [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11382 SiteYield",            sSiteYield              [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11383 BySiteClean",          sBySiteClean            [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "11384 ByBinClean",           sByBinClean             [tag]->CommaText);

        if(CosFunction.bBySiteByBinPercentCompare==true &&                      //JerryYang 20170712 (Steven) by site by bin compare percent
           CosFunction.bByBinAlarmFromYieldForm==false)                         //jou 20180113 (Steven) : By Site By Bin Percent Compare From Yield form
        {
            FormSysTools->WriteIniData(GroupName, "11385 SpecBinBySiteCompareEnable",        sSpecBinBySiteCompareEnable         [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "11386 SpecBinBySiteCompareIgnore",        sSpecBinBySiteCompareIgnore         [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "11387 SpecBinBySiteComparePercent",       sSpecBinBySiteComparePercent        [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "11388 SpecBinByArmPerSiteCompareEnable",  sSpecBinByArmPerSiteCompareEnable   [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "11389 SpecBinByArmPerSiteCompareIgnore",  sSpecBinByArmPerSiteCompareIgnore   [tag]->CommaText);
            FormSysTools->WriteIniData(GroupName, "11390 SpecBinByArmPerSiteComparePercent", sSpecBinByArmPerSiteComparePercent  [tag]->CommaText);
        }

        FormSysTools->WriteIniData(GroupName, "BinTrayLinked(11398)", sBinTrayLinked        [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "BinLinked(11399)",     sBinLinked            [tag]->CommaText);
        FormSysTools->WriteIniData(GroupName, "MagazineSetup(11400)", sMagazineSetup        [tag]->CommaText);
    }
}
//---------------------------------------------------------------------------
void TfBinSel::SetPrimeButton()
{
    if(IniConfig.bFTBin2RTBin && IniConfig.bA02BinModelPrime)                   //ChungHung 20120912 add Bin Setting like Epson
    {
        spbNormal->Visible=true;
        spbPrime->Visible=true;

        if(iBinModelPrime==1)
        {
            tsRetest->Enabled=true;
            spbPrime->Down=true;
            cbTestMode->Enabled=true;
        }
        else
        {
            tsRetest->Enabled=false;
            spbNormal->Down=true;
            cbTestMode->Enabled=false;
        }
    }
    else
    {
        if(IniConfig.bFTBin2RTBin && IniConfig.bA02BinModelPrime==false)        //jou 2014-08-26 修正 BIN select FT/RT 顯示異常
        {
            tsRetest->Enabled=false;
            cbTestMode->Enabled=false;
        }

        spbNormal->Visible=false;
        spbPrime->Visible=false;
    }
    fMain->SetNormalOrPrime();                                                  //Steven 20160817 : 必須在讀完Bin別設定之後
}
//---------------------------------------------------------------------------
void TfBinSel::ReadPrimeDara()                                                  //ChungHung 20120912 add Bin Setting like Epson
{
    AnsiString S=GetLastOpenFN();
    AnsiString szDir;

    szDir.sprintf("%s%s\\Binasgn.Data", DataPath, S);
    iBinModelPrime=ReadIniData(szDir, "BinModel", "bPrime", 0);
    SetPrimeButton();                                                           //Steven 20160624 : 重新顯示按鈕

    if(CosFunction.bShow0Xbin)                                                  //jou 20220719 : show 0X bin
    {
        chkShow0Xbin->Checked=ReadIniData(szDir, "Other", "bShow0XBin",false);
    }
}
//---------------------------------------------------------------------------
void TfBinSel::WritePrimeDara(int Mode)                                         //ChungHung 20120912 add Bin Setting like Epson
{
    AnsiString S=GetLastOpenFN();
    AnsiString szDir;

    szDir.sprintf("%s%s\\Binasgn.Data", DataPath, S);
    WriteIniData(szDir, "BinModel", "bPrime", Mode);
    iBinModelPrime=Mode;
    SetPrimeButton();                                                           //Steven 20160624 : 重新顯示按鈕
}
//---------------------------------------------------------------------------
void __fastcall TfBinSel::ed_FixBinBoxAlarmCountMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0,true, 20, 2);
}
//---------------------------------------------------------------------------
void __fastcall TfBinSel::CancelErrorBinClick(TObject *Sender)
{
    for(int tag=0; tag<eBinTypeTotal; tag++)
    {
        if(CancelErrorBin->Checked==false)                                      //kevin 20160819 error bin 要放到 Bin Box //kevin 20160724 取消 ERROR BIN
        {
            MyBinPanel[tag]->iErrorT6=iFixRight;
            BinSelect[tag].IfErrorT3 =iTo6Unload[iFixRight];
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfBinSel::rg_FixBinBoxClick(TObject *Sender)
{
    for(int tag=0; tag<eBinTypeTotal; tag++)
    {
        if(rg_FixBinBox->ItemIndex==0)                                          //kevin 20160819 error bin 要放到 Bin Box //kevin 20160724 取消 ERROR BIN
        {
            MyBinPanel[tag]->iErrorT6=iFixRight;
            BinSelect[tag].IfErrorT3 =iTo6Unload[iFixRight];
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfBinSel::sgSpecificBinMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int Column, Row;
    sgSpecificBin->MouseToCell(X, Y, Column, Row);                              //JerryYang 20170221 矽品要求當測試結果中所有site的bin都是所設定一樣的fail bin要跳alarm,改成可複選bin
    if(Row==1)
    {
        if(sgSpecificBin->Cells[Column][Row]=="")
        {
            sgSpecificBin->Cells[Column][Row]="V";
        }
        else
        {
            sgSpecificBin->Cells[Column][Row]="";
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfBinSel::btnSettingSpecificBinClick(TObject *Sender)
{
    if(palSpecificBin->Visible==false)                                          //JerryYang 20170301 (wei) specific bin
    {
        palSpecificBin->Visible=true;
        palSpecificBin->Parent=PageControl1;
        palSpecificBin->BringToFront();
        palSpecificBin->Top=625;                                                //JerryYang 20220923 : 650->625
    }
    else
    {
        palSpecificBin->Visible=false;
    }
}
//---------------------------------------------------------------------------
void TfBinSel::ReadWriteMRTMode(int iMode)                                      //Mode 0:Read 1:Write  //Ifor 20170417 (wei) add MRT Mode Read Write Function
{
    AnsiString S=GetLastOpenFN();
    AnsiString szDir="";
    szDir=DataPath+S;
    szDir.sprintf("%s%s\\HandlerCondition.Data", DataPath, S);

    if(CosFunction.bUseMRTMode==true)
    {
        if(iMode==0)
        {
            TestIF_File.bEnableMRTMode=ReadIniData(szDir, "Configuration", "Enable MRT Mode", false);
            cbUseMRTMode->Checked     =TestIF_File.bEnableMRTMode;
        }
        else
        {
            WriteIniData(szDir, "Configuration", "Enable MRT Mode",(cbUseMRTMode->Checked)?1:0);//Ifor 20170405 add MRT Use Mode By Setup File
            TestIF_File.bEnableMRTMode=cbUseMRTMode->Checked;
        }
    }
    else                                                                        //Ifor 20170504 (wei) add 未開啟MRT Mode 客戶 強制設定EnableMRTMode = false
    {
        TestIF_File.bEnableMRTMode=false;

        if(CosFunction.bAutoSiteMappingSetOpenBIN==true)                        //jou 20200928 : Auto Site Mapping Set Open BIN
        {
            if(iMode==0)
            {
                cbbAutoSiteMap->ItemIndex=ReadIniData(szDir, "Configuration", "ASM Open Bin", 0);
                TestIF_File.iOpenBin=cbbAutoSiteMap->ItemIndex-1;
                Prod.iOpenBin=TestIF_File.iOpenBin;                             //Steven 20230213 : [I21-9]的OS Bin跟著工作檔
                fMain->SetOpenBin();
            }
            else
            {
                WriteIniData(szDir, "Configuration", "ASM Open Bin", cbbAutoSiteMap->ItemIndex);
            }

            if(CosFunction.bAutoSiteMappingSetPassBIN==true)                    //jou 20230221 : Auto Site Mapping Set Pass BIN
            {
                if(iMode==0)
                {
                    cbbASMPassBin->ItemIndex=ReadIniData(szDir, "Configuration", "ASM Pass Bin", 2);
                    TestIF_File.iASMPassBin=cbbASMPassBin->ItemIndex-1;
                }
                else
                {
                    WriteIniData(szDir, "Configuration", "ASM Pass Bin",    cbbASMPassBin->ItemIndex);
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
void TfBinSel::ReadWriteSpecialFunction(bool bReadWrite)                        //Mode 0:Read 1:Write  //JerryYang 20220923 : bOutShtLoseICSetErrUntilOneCycle改為可By工作檔
{
    if(IniConfig.bSPILFunction==true)                                           //JerryYang 20220923 : SPIL Jovie要求改為By工作檔
    {
        AnsiString S=GetLastOpenFN();
        AnsiString szDir="";
        szDir=DataPath+S;
        szDir.sprintf("%s%s\\HandlerCondition.Data", DataPath, S);

        if(CosFunction.bOutShtLoseICSetErrUntilOneCycle)
        {
            if(bReadWrite==false)                                               //Read
            {
                TestIF_File.bOutShtLoseICSetErrUntilOneCycle=ReadIniData(szDir, "Configuration", "bOutShtLoseICSetErrUntilOneCycle", false);
                cbOutShtLoseICSetErrUntilOneCycle->Checked  =TestIF_File.bOutShtLoseICSetErrUntilOneCycle;
            }
            else                                                                //Write
            {
                WriteIniData(szDir, "Configuration", "bOutShtLoseICSetErrUntilOneCycle", cbOutShtLoseICSetErrUntilOneCycle->Checked);
                TestIF_File.bOutShtLoseICSetErrUntilOneCycle=cbOutShtLoseICSetErrUntilOneCycle->Checked;
            }
        }
        else
        {
            TestIF_File.bOutShtLoseICSetErrUntilOneCycle=false;
        }

        if(CosFunction.bIndexDropICSetErrUntilOneCycle)
        {
            if(bReadWrite==false)                                                //Read
            {
                TestIF_File.bIndexDropICSetErrUntilOneCycle=ReadIniData(szDir, "Configuration", "bIndexDropICSetErrUntilOneCycle", false);
                cbIndexDropErrSetErrUntilOneCycle->Checked =TestIF_File.bIndexDropICSetErrUntilOneCycle;
            }
            else                                                                //Write
            {
                WriteIniData(szDir, "Configuration", "bIndexDropICSetErrUntilOneCycle", cbIndexDropErrSetErrUntilOneCycle->Checked);
                TestIF_File.bIndexDropICSetErrUntilOneCycle=cbIndexDropErrSetErrUntilOneCycle->Checked;
            }
        }
        else
        {
            TestIF_File.bIndexDropICSetErrUntilOneCycle=false;
        }
    }
    else
    {
        if(CosFunction.bOutShtLoseICSetErrUntilOneCycle)
        {
            TestIF_File.bOutShtLoseICSetErrUntilOneCycle=true;
        }
        else
        {
            TestIF_File.bOutShtLoseICSetErrUntilOneCycle=false;
        }

        if(CosFunction.bIndexDropICSetErrUntilOneCycle)
        {
            TestIF_File.bIndexDropICSetErrUntilOneCycle=true;
        }
        else
        {
            TestIF_File.bIndexDropICSetErrUntilOneCycle=false;
        }
    }
}
//---------------------------------------------------------------------------
bool TfBinSel::CheckOSBin()                                                     //Sam 20250115 : 新增 OSBIN 保護
{
    bool IsOK=true;
    if(CosFunction.bAutoSiteMappingSetOpenBIN==true)
    {
        if(Prod.iOpenBin>=0)
        {
            if(BinSelect[iTestRunMode].iCatDataT3Pos[Prod.iOpenBin]==0)
                IsOK=false;
        }
    }
    return IsOK;
}
//---------------------------------------------------------------------------
bool TfBinSel::SetOSBin(int iBin)  //-1 : not use ; 0 : Bin0 ; 1 : Bin1.....    //Sam 20250115 : Add GPIB SETOSBIN_
{
    bool IsOK=true;
    if(iBin>=-1 && iBin<16)
    {
        cbbAutoSiteMap->ItemIndex=iBin+1;
        ReadWriteMRTMode(1);
        ReadWriteMRTMode(0);
        if(CheckOSBin()==false)
            IsOK=false;
    }
    else
    {
        IsOK=false;
    }
    return IsOK;
}
//---------------------------------------------------------------------------
//******************************************************************************
//
//  注意!! cBinSel為Handler Bin設定相關, 修改時要小心!!
//
//******************************************************************************
void __fastcall TfBinSel::palSpecificBinClick(TObject *Sender)
{
    palSpecificBin->Visible=false;                                              //JerryYang 20170301 (wei) 存檔後不顯示
}
//----------------------------SECSGEM-----------------------------------------------
void __fastcall TfBinSel::Save(int iECIDCode, int iFileCode)                    //kevin 20181120
{
    AnsiString S="";
    AnsiString SBuffer1="",SBuffer="";                                          //kevin 20170828 (Steven) add
    int tag=iFileCode;

    S=GetLastOpenFN();

    AnsiString szDir="", GroupStr, szDirTarget;

    ReadPrimeDara();                                                            //Steven 20160817 : fixed for Prime & Normal
    AnsiString SavePath[]={"\\BinasgnOff.Data",
                           "\\Binasgn.Data",
                           "\\BinasgnOff_ART.Data",                             //BinasgnOff is Bin of RT Mode
                           "\\Binasgn_ART.Data",                                //ChungHung 20141002 add for KYEC AutoRetest
                           "\\Binasgn_MRT_RT.Data",
                           "\\Binasgn_MRT.Data"};                               //Ifor 20170316 (wei) Add MRT Mode

    if(IniConfig.bA02BinModelPrime)                                             //Steven 20160623 : 當IniConfig.bFTBin2RTBin==true && 讀檔時, RT Bin要跟FT Bin一樣
    {
        if(IniConfig.bFTBin2RTBin==true && (iBinModelPrime==0 || CosFunction.bDisableRTBinSet))
        {
            SavePath[0]="\\Binasgn.Data";
        }
        else
        {
            SavePath[0]="\\BinasgnOff.Data";
        }
    }
    else
    {
        if(IniConfig.bFTBin2RTBin==true || CosFunction.bDisableRTBinSet)
        {
            SavePath[0]="\\Binasgn.Data";
        }
        else
        {
            SavePath[0]="\\BinasgnOff.Data";
        }
    }

    szDir=GetRecipePath()+SavePath[tag];

    FormSysTools->OpenFormData(szDir);                                          //Steven 20140531 : 為了加快ini讀取速度

    switch(iECIDCode)
    {
        case 3616:                                                              //I/F Error----------
            FormSysTools->WriteIniData("I/F Error", "Bin", BinSelect[eBinFT].IfErrorT3);
            FormSysTools->WriteIniData("I/F Error", "Tray", s3TrayName[BinSelect[eBinFT].IfErrorT3]);               //Steven 20241205 : Bin to tray增加文字顯示
            ReadFile(false, false, "");
            fShowBinSelect->ShowBinSel();
            break;
        case 3617:
            FormSysTools->WriteIniData("Bin Func FT", "3617 BinTraySetting", sBinTraySetT3Pos[eBinFT]->CommaText);  //kevin 20181127 add
            TransferBinTrayStrToName(eBinFT);
            FormSysTools->WriteIniData("Bin Func FT", "3617 BinTraySetName", sBinTraySetT3PosName[eBinFT]->CommaText);
            break;
        case 3677:
            for(int i=0; i<eTrayCount; i++)
            {
                FormSysTools->WriteIniData(s3TrayName[i], "Pass/Fail", fBinSel->sT3TrayType[eBinFT]->Strings[i]);   //JerryYang 20230928
            }
            break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfBinSel::FormShortCut(TWMKey &Msg, bool &Handled)
{
    if(Msg.CharCode==VK_F5)                                                     //Sam 20220725 : 介面新增鍵盤 F5 將視窗恢復初始位置
    {
        Left=10;
        Top=10;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfBinSel::btnSetAll2NotUseClick(TObject *Sender)
{
    iStartX=0;                                                                  //JerryYang 20221215 : 一鍵把所有bin設定成Not use
    iEndX=iTestBinCount;
    iStartY=eBinNotUse;
    SetBinTray(iTestRunMode);
}
//---------------------------------------------------------------------------
void TfBinSel::TransferBinTrayStrToName(int iTag)                               //Steven 20241205 : Bin to tray增加文字顯示
{
    int iT3;
    TStringList *sT3List;
    sT3List = new TStringList();
    sT3List->CommaText=sBinTraySetT3Pos[iTag]->CommaText;
    for(int i=0; i<sT3List->Count; i++)
    {
        iT3=atoi(sT3List->Strings[i].c_str());
        if(i<sBinTraySetT3PosName[iTag]->Count)                                 //Steven 20251203 : add protection.
        {
            if(i>=iTestBinCount || iT3==0)
                sBinTraySetT3PosName[iTag]->Strings[i]="NotUse";
            else
                sBinTraySetT3PosName[iTag]->Strings[i]=s3TrayName[iT3-1];
        }
    }
    sT3List->Clear();
    delete sT3List;
}
//---------------------------------------------------------------------------
void __fastcall TfBinSel::mtTrayItemMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TTMyTray *Ptr;
    Ptr=(TTMyTray *)Sender;
    int tag=Ptr->Tag;

    MyBinPanel[tag]->mtTrayName->ConvertIndexCells(X, Y);

    if(X<0 || X>=MyBinPanel[tag]->mtBinSelect->XItem ||
       Y<0 || Y>=MyBinPanel[tag]->mtBinSelect->YItem ||
       iTestBinCount<17)
    {
        return;
    }

    if(CosFunction.bOneCycleCanChangeContinuesFailBin)                          //kevin 20150525
    {
        if(InArmSuck.HasIC()  ||
           OutArmSuck.HasIC() ||
           ShuttleHasIC()     ||
           IndexHasIC())
            return;                                                             //kevin 20150519 有IC不能改變資料
    }

    iStartY=Y;
    iStartX=0;
    iEndX=MyBinPanel[tag]->mtBinSelect->XItem-1;

    if(iStartY>=eBinNotUse)
    {
        for(int i=0; i<iTestBinCount; i++)
        {
            if(MyBinPanel[tag]->BackT6PosTray[i][Y]==1)
            {
                if(iStartY==eBinNotUse)
                {
                    MyBinPanel[tag]->mtBinSelect->SetCellColorIndex(i, iStartY, eCLSilver);
                }
                else
                {
                    for(int j=eBinNotUse; j<eBinSetTotal; j++)
                    {
                        MyBinPanel[tag]->mtBinSelect->SetCellColorIndex(i, iStartY, tcBinColor[MyBinPanel[tag]->iT6IsFail[iStartY-eBinSetting]]);
                    }
                }
            }
            else
            {
                if(i<MyBinPanel[tag]->mtBinSelect->XItem)
                {
                    if(i>=iStartX && i<=iEndX)
                    {
                        MyBinPanel[tag]->mtBinSelect->SetCellColorIndex(i, iStartY, eCLWhite);
                    }
                }
            }
        }

        SetBinTray(tag);
    }
    else
    {
        if(Y==eDoubleContact)
        {
            if(SeteDoubleContact(-1, tag)==false)
            {
                return;
            }
        }
        else if(Y==eConsFail)                                                   //ChungHung 20140414 add 必須為fail bin 才能選
        {
            if(SeteConsFail(-1, tag)==false)
            {
            }
        }
        else if(Y==ePersentEnable)                                              //Steven 20140529 Start: Fail Persent & Count同時存在
        {
            if(SetePersentEnable(-1, tag)==false)
            {
            }
        }
        else if(Y==ePersentIgnore)
        {
            if(SetePersentIgnore(-1, tag)==false)
            {
            }
        }
        else if(Y==ePersentNumber)
        {
            if(SetePersentNumber(-1, tag)==false)
            {
            }
        }
        else if(Y==eCountEnable)
        {
            if(SeteCountEnable(-1, tag)==false)
            {
            }
        }
        else if(Y==eCountIgnore)
        {
            if(SeteCountIgnore(-1, tag)==false)
            {
            }
        }
        else if(Y==eCountNumber)
        {
            if(SeteCountNumber(-1, tag)==false)
            {
            }
        }
        else if(Y==eSpecialBinByArm)                                            //ChungHung 20140730 add ContinuousFailHaveOneCycle start
        {
            if(SeteSpecialBinByArm(-1, tag)==false)
            {
            }
        }
        else if(Y==eSpecialBinCountByArm)
        {
            if(SeteSpecialBinCountByArm(-1, tag)==false)
            {
            }
        }
        else if(Y==eSpecialBinBySocket)
        {
            if(SeteSpecialBinBySocket(-1, tag)==false)
            {
            }
        }
        else if(Y==eSpecialBinCountBySocket)
        {
            if(SeteSpecialBinCountBySocket(-1, tag)==false)
            {
            }
        }
        else if(Y==eLowYield)                                                   //Steven 20140828 : By Bin Yield Monitor
        {
            if(SeteLowYield(-1, tag)==false)
            {
            }
        }
        else if(Y==eArmYield)                                                   //Steven 20140828 : By Bin Arm Yield Monitor
        {
            if(SeteArmYield(-1, tag)==false)
            {
            }
        }
        else if(Y==eSiteYield)                                                  //Steven 20140828 : By Bin Site Yield Monitor
        {
            if(SeteSiteYield(-1, tag)==false)
            {
            }
        }
        else if(Y==eAutoCleanByBin)                                             //Steven 20160308 : By Bin count do auto clean
        {
            if(SeteAutoCleanByBin(-1, tag)==false)
            {
            }
        }
        else if(Y==eAutoCleanBySite)                                            //Steven 20160308 : By Bin pre site count do auto clean
        {
            if(SeteAutoCleanBySite(-1, tag)==false)
            {
            }
        }
        else if(Y==eSpecBinBySiteCompareEnable)                                 //JerryYang 20170712 (Steven) by site by bin compare percent
        {
            if(SeteSpecBinBySiteCompareEnable(-1, tag)==false)
            {
            }
        }
        else if(Y==eSpecBinBySiteCompareIgnore)
        {
            if(SeteSpecBinBySiteCompareIgnore(-1, tag)==false)
            {
            }
        }
        else if(Y==eSpecBinBySiteComparePercent)
        {
            if(SeteSpecBinBySiteComparePercent(-1, tag)==false)
            {
            }
        }
        else if(Y==eSpecBinByArmPerSiteCompareEnable)                           //JerryYang 20170712 QQ
        {
            if(SeteSpecBinByArmPerSiteCompareEnable(-1, tag)==false)
            {
            }
        }
        else if(Y==eSpecBinByArmPerSiteCompareIgnore)
        {
            if(SeteSpecBinByArmPerSiteCompareIgnore(-1, tag)==false)
            {
            }
        }
        else if(Y==eSpecBinByArmPerSiteComparePercent)
        {
            if(SeteSpecBinByArmPerSiteComparePercent(-1, tag)==false)
            {
            }
        }
        InitDataToEdit(tag);
    }
}
//---------------------------------------------------------------------------
