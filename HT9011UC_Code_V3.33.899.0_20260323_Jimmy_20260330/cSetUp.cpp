#include "MachineDefine.h"
#pragma hdrstop

#include "cSetUp.h"

#include "common.h"
#include "cprod.h"
#include "cinitial.h"
#include "uCleaning.h"
#include "mymessbox.h"
#include "cMyDef.h"
#include "main.h"
#include "cUnitConvert.h"
#include "cAuthority.h"
#include "rs232.h"
#include "MyKitSuck.h"
#include "uLotInfo.h"
#include "Password.h"
#include "cMyDB.h"
#include "uTemp_Set.h"
#include "csystem.h"
#include "cTemperFrom.h"                                                        //Steven 20111208
#include "cContact.h"                                                           //20111212  Dell
#include "automation.h"
#include "OCR.h"
//#include "AutoClean_ASEKH.h"
//#include "uCleaning_ASEKH.h"
#include "cSecurity.h"
#include "BarcodeReader.h"                                                      // 2013.11.29 , Joye , KYEC Barcode Reader  20140103 wei
#include "MessageDef.h"
#include "cHotPlate.h"
#include "uHGemHT9045.h"
#include "ATCInterface.h"
#include "ATC_Handler_Side.h"                                                   //Ifor 20151230 :add New ATC Interface
#include "cBuilder.h"
#include "MyMotor.h"
#include "aArmHeader.h"
#include "myQwertyKeyBoard.h"
#include "handlerlog.h"                                                         //JerryYang 20160617
#include "mysensor.h"
#include "RPDefault.h"                                                          //Isaac 20210601 : 全智要把指定參數存出來
#include "adam6024.h"
#include "Command.h"
#include "main.h"
#include "cStartCondition.h"
#include "cConfiguration.h"
//#include "ainarm2.h"

TComboBox   *TestSiteCH[MAX_SOCKET_ROW][MAX_SOCKET_COL];
TLabel      *TestLabCol[MAX_SOCKET_COL];
TLabel      *TestLabRow[MAX_SOCKET_ROW];
int  iTestSiteCh[MAX_SOCKET_ROW][MAX_SOCKET_COL];
int iASMTestMode;                                                               //Steven 20120927 : for auto site mapping
int iASMSiteMap[MAX_SOCKET_ROW][MAX_SOCKET_COL];                                //Steven 20120927 : for auto site mapping
bool bNeedEnterPassword=false;                                                  //JerryYang 20160425 修改Site map需要密碼
AnsiString sSigPassword;                                                        //Alick 20160602 矽格Password用
TRadioGroup *MyTempRGBox[iSnSocketCnt];                                         //Steven 20200420 : Socket Sensor功能可以選
//------------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfSetup *fSetup;
//AnsiString TestSiteFileName[2][TotalTestMode]=                                //JerryYang 20201125 : TestSiteFileName移到cmydef
//{
//    {"Single Site",                     //SingleSite
//     "2-Site",                          //DualSite
////     "2-Site Busy Shuttle",           //DualSiteBS
//     "3-Site (1x3)",                    //TriSite1X3
//     "In-Line 4-Site(1X4)",             //QualSite1X4
//     "2-Site (2x1)",                    //DualSite2x1
//     "Square 4-Site(2X2)",              //QualSite2X2
//     "Square 4-Site(2X2) NN Mode",      //QualSite2X2N                        //Frank 20200520 2X2NN Mode
////     "Square 4-Site(2X2) Busy Shuttle",  //QualSite2X2BS
//     "6-Site",                          //_6Site2X3                           //ChungHung 20140115 add for 2x3_6
//     "8-Site",                          //_8Site2X4
//     "10-Site",                         //_10Site2X5
//     "12-Site",                         //_12Site2X6
//     "16-Site",                         //_16Site2X8
//     "16-Site (4X4)",                   //_16Site4X4                          //Sam 20190226 : 16Site4X4
//     "32-Site N Mode",                  //_32Site4X8N
//     "32-Site M Mode",                  //_32Site4X8M
//     "8-Site Pop"                       //_8Site1X4                           //ChungHung 20150528 add for 海思 _8Site1x4
//    },
//    {"1site.bmp",                       //SingleSite
//     "2site.bmp",                       //DualSite
////     "2siteBusy.bmp",                 //DualSiteBS
//     "3site.bmp",                       //TriSite1X3
//     "4site.bmp",                       //QualSite1X4
//     "2site2x1.bmp",                    //DualSite2x1
//     "4siteRow.bmp",                    //QualSite2X2
//     "2x2siteRow_NN.bmp",               //QualSite2X2N                        //Frank 20200520 2X2NN Mode  2x2siteRow_NN.bmp
////     "4siteBusy.bmp",                 //QualSite2X2BS
//     "6Site.bmp",                       //_6Site2X3                           //ChungHung 20140115 add for 2x3_6
//     "2x3site_NN.bmp",                  //_6Site2X3N                          //Steven 20220425 : 2x3 NN mode
//     "8Site.bmp",                       //_8Site2X4
//     "10Site.bmp",                      //_10Site2X5
//     "12Site.bmp",                      //_12Site2X6
//     "16Site.bmp",                      //_16Site2X8
//     "16Site4X4.bmp",                   //_16Site4X4                          //Sam 20190226 : 16Site4X4
//     "32SiteN.bmp",                     //_32Site4X8N
//     "32SiteM.bmp",                     //_32Site4X8M
//     "8-Site Pop.bmp"                   //_8Site1X4                           //ChungHung 20150528 add for 海思 _8Site1x4
//    }
//};
//int SiteData[TotalTestMode][2]=                                               //JerryYang 20181005 Mark掉,SiteData改成全域變數
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
//    };
int OrgTestMode=0;
//------------------------------------------------------------------------------
__fastcall TfSetup::TfSetup(TComponent* Owner)
    : TForm(Owner)
{
    bSiteMapHasChange=true;                                                     //Steven 20250102 : 紀錄test mode與site map是否有變更
    iTestMode=TotalTestMode;
    AnsiString Str;
    TComboBox *tempTestSiteCBox[MAX_SOCKET_ROW][MAX_SOCKET_COL]=
    {
        {cbAa, cbAb, cbAc, cbAd, cbAe, cbAf, cbAg, cbAh},
        {cbBa, cbBb, cbBc, cbBd, cbBe, cbBf, cbBg, cbBh},
        {cbCa, cbCb, cbCc, cbCd, cbCe, cbCf, cbCg, cbCh},
        {cbDa, cbDb, cbDc, cbDd, cbDe, cbDf, cbDg, cbDh}
    };
    TLabel *tempTestLabCol[MAX_SOCKET_COL]=
    {
        labColA, labColB, labColC, labColD, labColE, labColF, labColG, labColH
    };
    TLabel *tempTestLabRow[MAX_SOCKET_ROW]={labRowA, labRowB, labRowC, labRowD};

    int iCol, iRow;
    for(int i=0; i<MAX_SOCKET_TOTAL; i++)
    {
        iRow=i/MAX_SOCKET_COL;
        iCol=i%MAX_SOCKET_COL;

        if(i<MAX_SOCKET_ROW)
        {
            TestLabRow[i]=tempTestLabRow[i];
            TestLabRow[i]->Visible=false;
        }

        if(i<MAX_SOCKET_COL)
        {
            TestLabCol[i]=tempTestLabCol[i];
            TestLabCol[i]->Visible=false;
        }
        iTestSiteCh[iRow][iCol]=-1;
        TestSiteCH[iRow][iCol]=tempTestSiteCBox[iRow][iCol];
        TestSiteCH[iRow][iCol]->Visible=false;
    }
    fShow=false;

    if(MachineTypeChoice==Type_HT9045)
    {
        ScrollBar1->Max=_8Site2X4;
    }
    else if(MachineTypeChoice==Type_HT9046_LS ||
            MachineTypeChoice==Type_HT1032)
    {
        ScrollBar1->Max=_32Site4X8N;
    }
    else if(MachineTypeChoice==Type_HT9045_12Site)                              //ChungHung 20130507 add HT9045 updata for 12site 517
    {
        ScrollBar1->Max=_12Site2X6;
    }
    else
    {
        ScrollBar1->Max=_16Site2X8;
    }

    iTestModeOcr=-1;                                                            //Steven 20120716 : OCR

    tSiteMap=new TStringList();                                                 //Steven 20140510 : Secs Gem
    for(int i=0; i<MAX_SOCKET_ROW; i++)
    {
        for(int j=0; j<MAX_SOCKET_COL; j++)
        {
            tSiteMap->Add("0");
        }
    }

    if(USE_IN_OUT_ARM_Y_PITCH==iXPitchManual360)                                //Steven 20140819 : Y-Pitch 36mm
    {
        rgInOutArmYPitch->Items->Clear();
        rgInOutArmYPitch->Items->Add("60.0 mm");
        rgInOutArmYPitch->Items->Add("36.0 mm");
    }

    for(int i=0; i<iSnSocketCnt; i++)                                           //Steven 20200610 : Socket sensor 改成16顆
    {
        MyTempRGBox[i]=new TRadioGroup(this);
        MyTempRGBox[i]->Parent=scrlbxSocketSensor;
        MyTempRGBox[i]->Name=AnsiString("rgSensor")+AnsiString(i+1);
        MyTempRGBox[i]->Height=36;
        Str.sprintf("Sensor %d usage", i+1);
        MyTempRGBox[i]->Caption=Str;
        MyTempRGBox[i]->Columns=3;
        MyTempRGBox[i]->Items->Add("No use");
        MyTempRGBox[i]->Items->Add("Has IC");
        MyTempRGBox[i]->Items->Add("Floating");
        MyTempRGBox[i]->Align=alTop;
        MyTempRGBox[i]->OnClick=rgSensor1Click;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::FormDestroy(TObject *Sender)
{
    try
    {
        tSiteMap->Clear();                                                      //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        delete tSiteMap;                                                        //Steven 20140510 : Secs Gem
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfSetup::FormDestroy");
    }
    LogSoftwareOffTime("TfSetup, FormDestroy");                                 //Steven 20210526 : 紀錄軟體執行時間
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::ScrollBar1Change(TObject *Sender)
{
    static bool bFirstRead=true;
    gbShuttleMode->Visible=true;                                                //kevin 20210813 add close arm display
    if(CosFunction.bDisableOpenAllSiteWhenChangeShtMod)                         //Jimmychiu 20241203 : Disable opening all sites when changing shuttle mode.
    {
        shtMode.Clear();
    }

    if(ScrollBar1->Position!=TestIF_File.iTestMode &&
       Barcode_Reader(bcSetup)==0)                                              // 20140103 wei KYEC Barcode Reader 20140314
    {
        ScrollBar1->Position=TestIF_File.iTestMode;
        for(int i=0; i<MAX_SOCKET_ROW; i++)
        {
            for(int j=0; j<MAX_SOCKET_COL; j++)
            {
                TestSiteCH[i][j]->ItemIndex=TestIF_File.iSiteMap[i][j];
            }
        }
        return;
    }

    if(IniConfig.bIndexArm2SupplyLight==true ||                                 //jou 2012-10-19 Index Arm 2 供應光源 for CMOS
       TestIF_File.bForEgisTecTest==true ||                                     //Steven 20140922 : Arm2當作指紋測試
       (TestIF_File.bArm1PickPlaceArm2Test &&                                   //kevin 20150127 Arm1 下壓 arm2 測試
        IniConfig.bD58UseArm1PickPlaceArm2Test==true))                          //Ifor 20191122 : 避免D58功能關閉無法開ARM問題
    {
        gbShuttleMode->Visible=false;                                           //only use shuttle 1
    }
    else
    {
        gbShuttleMode->Visible=IniConfig.bD30EnableSiteModeSelect;
    }

    if(bGPIBChangeSiteMap)                                                      //wei 20151127 GPIB Change Site Map
    {
        ScrollBar1->Position=TestIF_File.iTestMode;
        for(int i=0; i<MAX_SOCKET_ROW; i++)
            for(int j=0; j<MAX_SOCKET_COL; j++)
                TestSiteCH[i][j]->ItemIndex=TestIF.iSiteMap[i][j];
    }
    int iPos=ScrollBar1->Position;
    int iCheckPos;                                                              //wei 20150314 ScrollBar1->Position改變時，會有問題。  iPos-->iCheckPos

    int iMax;

    bool bShow32Site=(Sen[SnNegativePressureAir2].Enable==true);
#ifdef SOFT_SIMULTE
    bShow32Site=true;
#endif
//    if(CUSTOMER_CODE!=CC_HONPREC_QC  &&
//       USE_ROTATE_KIT==1             &&
//       iRotate_Type==eInOutArm1Motor &&
//       USE_PICKER_COUNT==ep1Picker)                                             //Frank 20250812 : modify InOutArm1Motor
//    {
//        iMax=SingleSite;
//    }
//    else
    if(MachineTypeChoice==Type_HT9045)
    {
        iMax=_8Site2X4;
    }
    else if((MachineTypeChoice==Type_HT9046_LS ||
             MachineTypeChoice==Type_HT1032) &&
             bShow32Site==true)                                                 //Ifor 20171226 (Steven) add 需安裝兩顆才可以跑32Site
    {
        iMax=_32Site4X8N;
    }
    else if(MachineTypeChoice==Type_HT9045_12Site)                              //ChungHung 20130507 add HT9045 updata for 12site 517
    {
        iMax=_12Site2X6;
    }
    else
    {
        iMax=_16Site2X8;
    }

    if(USE_PICKER_COUNT==0 && bHT9045S_USE2x4==false)                           //Ifor 20170308 (wei) add 鎖住 HT9045S 僅可跑2*2
    {
        iMax=QualSite2X2;
    }

    if(iPos>=iMax)
    {
        ScrollBar1->Position=iMax;
        OrgTestMode=iPos;
    }

    if(((!(MachineTypeChoice==Type_HT9046_LS ||
           MachineTypeChoice==Type_HT1032) ||                                   //ChungHung 20150528 add
        (MachineTypeChoice==Type_HT9046_LS && SubMachineType==Type_HT9016C) ||  //Jimmychiu 20230207 : HT9016C不可以跑NN Mode
        (MachineTypeChoice==Type_HT9046_LS && SubMachineType==Type_HT9046LA)) &&//Steven 20220425 : 9046LA不可以跑NN Mode
        (iPos==_32Site4X8N  ||
         iPos==_32Site4X8M  ||
         iPos==_16Site4X4   ||
         iPos==QualSite2X2N ||
         iPos==_6Site2X3N   ||
         iPos==_8Site2X4N)) ||                                                  //Steven 20220425 : 2x3 NN mode
       (MachineTypeChoice==Type_HT9045 && iPos>=_10Site2X5)         ||
       (MachineTypeChoice==Type_HT9045_12Site && iPos>=_16Site2X8)  ||
       (iPos==DualSite2x1  && CosFunction.bEnable2x1Site==false)    ||
       (iPos==_12Site2X6   && CosFunction.bEnable12Site==false)     ||
       (iPos==_6Site2X3    && CosFunction.bEnable6Site==false)      ||          //ChungHung 20140115 add for 2x3_6
       (iPos==_6Site2X3    && USE_PICKER_COUNT==0)                  ||          //Steven 20161117 : for HT-9045S
       (iPos==TriSite1X3   && USE_PICKER_COUNT==0)                  ||          //Steven 20161117 : for HT-9045S
       (iPos==QualSite2X2N && CosFunction.bCanUse2x2NNMode==false)  ||          //Steven 20200722 : 2x2 NN mode
       (iPos==_6Site2X3N   && CosFunction.bCanUse2x3NNMode==false)  ||          //Steven 20220425 : 2x3 NN mode
       (iPos==_8Site2X4N   && CosFunction.bCanUse2x4NNMode==false))             //Wei 20231211 : 2X4NN Mode
    {
        if(OrgTestMode<iPos)
            iPos++;
        else
            iPos--;

        if(iPos<0)                                                              //Steven 20120816 : 修正小於1的Error
        {
            iPos=1;
        }
        ScrollBar1->Position=iPos;
        OrgTestMode=iPos;
        bGPIBChangeSiteMap=false;                                               //JerryYang 20150105 GPIB Change Site Map
        return;                                                                 //ChungHung 20150528 modify ScrollBar1->Position改變時，會在觸發一次事件。
    }

    iCheckPos=ScrollBar1->Position;                                             //避免ScrollBar1->Position改變時，會有問題。
    OrgTestMode=iCheckPos;                                                      //ChungHung 20150528 modify ScrollBar1->Position改變時，會在觸發一次事件。

    chkOffCenterkitClick(this);
    Panel1->Caption=TestSiteFileName[0][iCheckPos];                             //wei 20150314 ScrollBar1->Position改變時，會有問題。  iPos-->iCheckPos
    CompChange(iCheckPos);                                                      //wei 20150314 ScrollBar1->Position改變時，會有問題。  iPos-->iCheckPos

    if(bFirstRead==true)                                                        //Ifor 20161117 add 取得程式開啟時 Handler Mode
    {
        asHandlingMode=Panel1->Caption;                                         //Ifor 20161117 add Handler Mode Backup
        bFirstRead=false;
    }

    cbAa->Enabled=true;
    XPitch                  ->Visible=true;                                     //Steven 20120604 : XPitch 顯示異常
    lblXPitch               ->Visible=true;
    lblXPMM                 ->Visible=true;
    cbNS8000H               ->Visible=false;
    chkOctal80              ->Visible=false;
    cb2CableLayoutKit       ->Visible=false;                                    //Steven 20150724 : 16溫控器 2條線版本
    cb1CableLayoutKit       ->Visible=false;                                    //jou 2015-10-15  : 16溫控器 1條線版本
    cb6CableLayoutKit       ->Visible=false;                                    //Steven 20230613 : ATC6.1 16Site SLK
    cbOctal16Site           ->Visible=false;                                    //Steven 20140312 : 8Site使用16Site Kit
    chk12SiteUse2x8SLK      ->Visible=false;                                    //Steven 20240807 : 12Site使用16Site Kit
    cbSquareOctalLayout     ->Visible=false;                                    //Steven 20141224 : 2x2Site使用8Site Kit
    chk2x2Use16siteSLK      ->Visible=false;                                    //Steven 20191113 : 2x2Site使用16Site Kit
    cb1x2Use1x4siteSLK      ->Visible=false;                                    //Isaac 20200311 : 1x2Site使用1x4Site Kit
    cbUse1x3siteSLK         ->Visible=false;                                    //KevinC 20260109 : 1x2Site and 2x2 NN mode 使用1x3Site Kit
    cbOctal12Site           ->Visible=false;                                    //ChungHung 20140508 add for SCK
    labYOffset              ->Visible=false;                                    //Steven 20140512 : For HT-9047
    edYOffset               ->Visible=false;                                    //Steven 20140512 : For HT-9047
    rgYOffset               ->Visible=false;                                    //Steven 20200715 : for Tinton
    cb16DirectHeater        ->Visible=false;                                    //Steven 20140923 : Index使用EJ1N版32組加熱器
    cb12Site10DirectHeater  ->Visible=false;                                    //Steven 20140923 : Index使用EJ1N版32組加熱器
    cbHotechLayoutKit2x2    ->Visible=false;                                    //jou 2015-12-10 SCS 要求 Hontech Layout kit要選擇Hontech.
    cbQualSite2X2Shift      ->Visible=false;                                    //wei 20160226 TSMC X Shift
    paQualSite2X2Shift      ->Visible=false;                                    //wei 20160226 TSMC X Shift
    cbSingleSiteSingleHeater->Visible=false;                                    //JerryYang 20161013 新增Single site一支加熱棒模式
    gbInUseBackRow          ->Visible=false;                                    //jou 20161122 (Steven) In & Out arm use back row suck
    gbOutUseBackRow         ->Visible=false;                                    //jou 20161122 (Steven) In & Out arm use back row suck
    cbQualSite2X2Shift      ->Checked=false;                                    //kevin 20170513 (wei) add
    grpUseXCenterPitch->Visible=(CosFunction.b2x4SupportCenterPitch==true &&
                                 iCheckPos==_8Site2X4);                         //Steven 20170706 (wei) : 2x4中間的Pitch不同 for SCC
    bUseTwoArm32Site=false;                                                     //kevin 20190322 add two arm tester 32SITE 16SITE(4X4)
    cb16change12DirectHeater->Visible=false;                                    //kevin 20191104 add 16 Site Direct Heater use 12 Site shuttle
    cb16change12DirectHeater->Checked=false;                                    //kevin 20191104 add 16 Site Direct Heater use 12 Site shuttle
    cb16change8DirectHeater ->Visible=false;                                    //Steven 20210322 : 16 Site Direct Heater use 8 Site shuttle
    cb16change8DirectHeater ->Checked=false;                                    //Steven 20210322 : 16 Site Direct Heater use 8 Site shuttle
    rgYPitchOffsetMode      ->Visible=false;                                    //Steven 20201013 : NN mode add Y-Pitch offset selection
    cbIndSLK                ->Visible=false;
    cbSingleUseOtherSuck    ->Visible=false;                                    //wei 20220905 Single Use Other Suck(IN C OUT E)
    chkOffCenterkit         ->Checked=false;                                    //kevin 20201211 add for ASE KH
    chkOffCenterkit         ->Visible=false;
    cbUseRotateForHT7000HPKit->Visible=false;                                   //Sam 20210416 : 新增特殊模式 For Rotate Function HT7000 HP Kit
    palVisibleIndex         ->Visible=false;                                    //KenHsieh 20231227 : add Y Center Pitch mode
    cbSingleInArmUseOtherSuck->Visible=false;                                   //Ifor 20240430 add: Single Site InArm Use Other Suck(IN C)
    switch(iCheckPos)                                                           //wei 20150314 ScrollBar1->Position改變時，會有問題。  iPos-->iCheckPos
    {
        case SingleSite:                                                        //1x1
            gbArm1PickArm2Test->Visible=IniConfig.bD58UseArm1PickPlaceArm2Test; //ChungHung 20150528 add for 海思 _8Site1x4
            cbAa->Enabled=false;
            cbAa->ItemIndex=1;
            rgSelectSearchLast->Visible=false;
            rgUseSuckMode->Visible=false;

            chkNS7000CS->Visible=false;
            chkOffCenterkit->Visible=CosFunction.bCanUseBias;                   //ChungHung 20140519 add Singe Site use NS7000 kit  //Steven 20190801 : SCC李國旗說要取消BIAS選項

            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                if(CUSTOMER_CODE!=CC_SCS)                                       //jou 20170518 (Steven) : SCS 要求沒用到不顯示
                    cb2CableLayoutKit->Visible=true;                            //JerryYang 20160826 Single site兩條線版本
            }
            else if(USE_16_HEATER==eht4Heater)
            {
                cbSingleSiteSingleHeater->Visible=true;                         //JerryYang 20161013 新增Single site一支加熱棒模式
            }
            lblXPitch->Visible=false;
            lblXPMM->Visible=false;
            lblYPitch->Visible=false;
            YPitch ->Visible=false;
            XPitch ->Visible=false;
            XPitch ->Text   ="1";
            YPitch ->Text   ="0";
            iSiteTotal=1;                                                       //kevin 20160125 SITE 數設定
            cb16DirectHeater->Visible=(USE_16_HEATER==eht32HeaterEJ1N  ||       //kevin 20191202 add 12 以下 direct //Steven 20140923 : Index使用EJ1N版32組加熱器
                                       USE_16_HEATER==eht32HeaterKT4H  ||       //Steven 20150211 : Index使用KT4H版32組加熱器
                                      (ATC_SYSTEM==eNewATCSystem &&
                                       iATC_Use_Heat_Count>=16) ||              //Ifor 20170814 (Steven) Add HT9046AT Direct Heater開關
                                       USE_16_HEATER==eht32HeaterDTME08);       //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            cbSingleUseOtherSuck    ->Visible=true;                             //wei 20220905 Single Use Other Suck(IN C OUT E)
            cbSingleInArmUseOtherSuck->Visible=true;                            //Ifor 20240430 add: Single Site InArm Use Other Suck(IN C)
            break;
        case DualSite:                                                          //1x2
            gbArm1PickArm2Test->Visible=IniConfig.bD58UseArm1PickPlaceArm2Test; //ChungHung 20150528 add for 海思 _8Site1x4
            rgSelectSearchLast->Visible=false;
            rgUseSuckMode->Visible=true;
            rgUseSuckMode->Items->Strings[0]="Use 2 pick unit";
            rgUseSuckMode->Items->Strings[1]="Use 4 pick unit";
            if(rgUseSuckMode->ItemIndex==1)
            {
                if(IniConfig.bDisableSelectSearchLast==true)                    //jou 2012-01-10 取消Setup，Search Last Mode功能。
                {
                    rgSelectSearchLast->ItemIndex=0;
                }
                else
                {
                    rgSelectSearchLast->Visible=true;
                }
            }

            if(USE_16_HEATER==eht16Heater       ||                              //Steven 20120606 : 16溫控器 8Site使用Hontech頭
               USE_16_HEATER==eht16HeaterEJ1N   ||
               USE_16_HEATER==eht32HeaterEJ1N   ||                              //Steven 20140923 : Index使用EJ1N版32組加熱器
               USE_16_HEATER==eht32HeaterKT4H   ||                              //Steven 20150211 : Index使用KT4H版32組加熱器
               USE_16_HEATER==eht16HeaterDTME08 ||                              //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
               USE_16_HEATER==eht32HeaterDTME08 )                               //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            {
                cb2CableLayoutKit->Visible=true;                                //JerryYang 20160826 Dual site兩條線版本
                if(IniConfig.bL30Use1CableLayoutKitByConfig==false)
                    cb1CableLayoutKit->Visible=true;
            }
            chkNS7000CS->Visible=false;                                         //Steven 20100119
            chkOffCenterkit->Visible=CosFunction.bCanUseBias;                   //Steven 20190801 : SCC李國旗說要取消BIAS選項

            if(CosFunction.bEnable_1x3Kit)                                      //KevinC 20260109 : 1x2Site and 2x2 NN mode 使用1x3Site Kit
            {
                cbUse1x3siteSLK->Visible=true;
                cbUse1x3siteSLK->Caption="Dual Site use 1x3 SLK";
                cbUse1x3siteSLK->Visible=CosFunction.bEnable_1x3Kit;
            }

            if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                 //kevin 20170415 (wei) KaohSiung add 2x2 Shift
            {
                cbQualSite2X2Shift->Visible=true;
                paQualSite2X2Shift->Visible=cbQualSite2X2Shift->Checked;        //kevin 20170513 X Shift
            }
            if(INSTALL_DOUBLE_EP==2)
            {
                cbIndSLK->Visible=true;
            }
            lblYPitch->Visible=false;
            YPitch->Visible=false;
            XPitch->Text=80;                                                    //Steven 20100827
            YPitch->Text="0";                                                   //Steven 20100827
            iSiteTotal=2;                                                       //kevin 20160125 SITE 數設定
            cb16DirectHeater->Visible=(USE_16_HEATER==eht32HeaterEJ1N  ||       //kevin 20191202 add 12 以下 direct //Steven 20140923 : Index使用EJ1N版32組加熱器
                                       USE_16_HEATER==eht32HeaterKT4H  ||       //Steven 20150211 : Index使用KT4H版32組加熱器
                                      (ATC_SYSTEM==eNewATCSystem &&
                                       iATC_Use_Heat_Count>=16) ||              //Ifor 20170814 (Steven) Add HT9046AT Direct Heater開關
                                       USE_16_HEATER==eht32HeaterDTME08);       //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            cb1x2Use1x4siteSLK->Visible=CosFunction.bEnableDual_1x4Kit;         //Isaac 20200311 : 1x2Site使用1x4Site Kit
            cbUseRotateForHT7000HPKit->Visible=(CosFunction.bRotateUseHT7000HPKit &&
                                                USE_ROTATE_KIT);                //Sam 20210416 : 新增特殊模式 For Rotate Function HT7000 HP Kit
            break;
        case DualSite2x1:                                                       //2x1
            gbArm1PickArm2Test->Visible=IniConfig.bD58UseArm1PickPlaceArm2Test; //ChungHung 20150528 add for 海思 _8Site1x4
            rgSelectSearchLast->Visible=false;
            rgUseSuckMode->Visible=false;

            chkNS7000CS->Visible=true;                                          //Steven 20140312 : For HT9045WA

            lblXPitch->Visible=false;
            lblXPMM->Visible=false;
            lblYPitch->Visible=true;
            YPitch ->Visible=true;
            XPitch ->Visible=false;
            XPitch ->Text   ="0";
            YPitch ->Text   ="";
            iSiteTotal=2;                                                       //kevin 20160125 SITE 數設定
            cb16DirectHeater->Visible=(USE_16_HEATER==eht32HeaterEJ1N  ||       //kevin 20191202 add 12 以下 direct //Steven 20140923 : Index使用EJ1N版32組加熱器
                                       USE_16_HEATER==eht32HeaterKT4H  ||       //Steven 20150211 : Index使用KT4H版32組加熱器
                                      (ATC_SYSTEM==eNewATCSystem &&
                                       iATC_Use_Heat_Count>=16) ||              //Ifor 20170814 (Steven) Add HT9046AT Direct Heater開關
                                       USE_16_HEATER==eht32HeaterDTME08);       //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            break;
        case TriSite1X3:                                                        //Frank 20160329 add for 1x3_4
            Arm1PickArm2Test->Checked=false;                                    //ChungHung 20150528 add for 海思 _8Site1x4
            gbArm1PickArm2Test->Visible=false;

            if(IniConfig.bDisableSelectSearchLast==true)                        //jou 2012-01-10 取消Setup，Search Last Mode功能。
            {
                rgSelectSearchLast->Visible=false;
                rgSelectSearchLast->ItemIndex=0;
            }
            else
            {
                rgSelectSearchLast->Visible=true;
            }
            rgUseSuckMode->Visible=false;

            if(ATC_SYSTEM>eATC30)                                               //20141204 ChungHung add for ATC3.0  //2014-05-30    Dell    for ATC6.0
            {
                chkNS7000CS->Visible=true;
            }
            else
            {
                chkNS7000CS->Visible=false;                                     //Steven 20100119
            }

            lblYPitch->Visible=false;
            YPitch->Visible=false;
            XPitch->Text="40";                                                  //Steven 20100827
            YPitch->Text="0";                                                   //Steven 20100827
            iSiteTotal=3;                                                       //kevin 20160125 SITE 數設定
            cb16DirectHeater->Visible=(USE_16_HEATER==eht32HeaterEJ1N  ||       //kevin 20191202 add 12 以下 direct //Steven 20140923 : Index使用EJ1N版32組加熱器
                                       USE_16_HEATER==eht32HeaterKT4H  ||       //Steven 20150211 : Index使用KT4H版32組加熱器
                                      (ATC_SYSTEM==eNewATCSystem &&
                                       iATC_Use_Heat_Count>=16) ||              //Ifor 20170814 (Steven) Add HT9046AT Direct Heater開關
                                       USE_16_HEATER==eht32HeaterDTME08);       //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            break;
        case _8Site1X4:                                                         //ChungHung 20150528 add for 海思 _8Site1x4
            Arm1PickArm2Test->Checked=false;                                    //ChungHung 20150528 add for 海思 _8Site1x4
            gbArm1PickArm2Test->Visible=false;

            if(IniConfig.bDisableSelectSearchLast==true)                        //jou 2012-01-10 取消Setup，Search Last Mode功能。
            {
                rgSelectSearchLast->Visible=false;
                rgSelectSearchLast->ItemIndex=0;
            }
            else
            {
                rgSelectSearchLast->Visible=true;
            }
            rgUseSuckMode->Visible=false;

            if(ATC_SYSTEM>eATC30)                                               //20141204 ChungHung add for ATC3.0  //2014-05-30    Dell    for ATC6.0
            {
                chkNS7000CS->Visible=true;
            }
            else
            {
                chkNS7000CS->Visible=false;                                     //Steven 20100119
            }

            lblYPitch->Visible=true;
            YPitch->Visible=true;
            XPitch->Text=40;                                                    //Steven 20100827
            YPitch->Text=60;                                                    //Steven 20100827
            iSiteTotal=4;                                                       //kevin 20160125 SITE 數設定
            cb16DirectHeater->Visible=(USE_16_HEATER==eht32HeaterEJ1N  ||       //kevin 20191202 add 12 以下 direct //Steven 20140923 : Index使用EJ1N版32組加熱器
                                       USE_16_HEATER==eht32HeaterKT4H  ||       //Steven 20150211 : Index使用KT4H版32組加熱器
                                      (ATC_SYSTEM==eNewATCSystem &&
                                       iATC_Use_Heat_Count>=16) ||              //Ifor 20170814 (Steven) Add HT9046AT Direct Heater開關
                                       USE_16_HEATER==eht32HeaterDTME08);       //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            break;
        case QualSite1X4:                                                       //1x4
            gbArm1PickArm2Test->Visible=IniConfig.bD58UseArm1PickPlaceArm2Test; //ChungHung 20150528 add for 海思 _8Site1x4

            rgSelectSearchLast->Visible=false;
            if(USE_PICKER_COUNT!=0)                                             //Steven 20161117 : for HT-9045S
            {
                rgUseSuckMode->Visible=true;
                rgUseSuckMode->Items->Strings[0]="Use 4 pick unit";
                rgUseSuckMode->Items->Strings[1]="Use 8 pick unit";
                if(rgUseSuckMode->ItemIndex==1)
                {
                    if(IniConfig.bDisableSelectSearchLast==true)                //jou 2012-01-10 取消Setup，Search Last Mode功能。
                    {
                        rgSelectSearchLast->ItemIndex=0;
                    }
                    else
                    {
                        rgSelectSearchLast->Visible=true;
                    }
                }
            }

//            if(IniConfig.bKoreaFunction==false &&                             //Steven 20240401 : Mark 1x4 SLK 2 Connector for Korea
            if(CUSTOMER_CODE!=CC_SCS)                                           //jou 2015-08-23 SCS remove Use 2 cable layout kit in Tool>setup
            {
                if(USE_16_HEATER==eht16Heater       ||                          //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                   USE_16_HEATER==eht16HeaterEJ1N   ||
                   USE_16_HEATER==eht32HeaterEJ1N   ||                          //Steven 20140923 : Index使用EJ1N版32組加熱器
                   USE_16_HEATER==eht32HeaterKT4H   ||                          //Steven 20150211 : Index使用KT4H版32組加熱器
                   USE_16_HEATER==eht16HeaterDTME08 ||                          //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                   USE_16_HEATER==eht32HeaterDTME08 )                           //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                {
                    cb2CableLayoutKit->Visible=true;                            //Steven 20150724 : 16溫控器 2條線版本
                    if(IniConfig.bL30Use1CableLayoutKitByConfig==false)         //Steven 20141022 : 1x4 Layout Kit with 2 Connector
                        cb1CableLayoutKit->Visible=true;                        //jou 2015-10-15  : 16溫控器 1條線版本
                }
            }

            if(CosFunction.bInOutArmUseBackRowSuck==true)                       //jou 20161122 (Steven) In & Out arm use back row suck
            {
                gbInUseBackRow->Visible=true;
                gbOutUseBackRow->Visible=true;
            }

            chkNS7000CS->Visible=true;                                          //kevin 20140829 使用偏心氣孔

            if(CUSTOMER_CODE==CC_ASE_KaohSiung)
            {
                chkOffCenterkit->Visible=true;
                chkOffCenterkit->Checked=true;                                  //kevin 20200924 add for ASE KH
            }
            else
            {
                chkOffCenterkit->Visible=CosFunction.bCanUseBias;               //Steven 20190801 : SCC李國旗說要取消BIAS選項
            }

            if(CosFunction.bNonCenterModeCanUseShtOffset &&
               IniConfig.bA50Enable1x4BiasYOffset &&
               chkOffCenterkit->Checked)                                        //Steven 20200715 : for Tinton
            {
                rgYOffset->Visible=true;
                rgYOffset->ItemIndex=0;
            }

            lblYPitch->Visible=false;
            YPitch->Visible=false;
            XPitch->Text=40;                                                    //Steven 20100827
            YPitch->Text="0";                                                   //Steven 20100827
            iSiteTotal=4;                                                       //kevin 20160125 SITE 數設定
            cb16DirectHeater->Visible=(USE_16_HEATER==eht32HeaterEJ1N  ||       //kevin 20191202 add 12 以下 direct //Steven 20140923 : Index使用EJ1N版32組加熱器
                                       USE_16_HEATER==eht32HeaterKT4H  ||       //Steven 20150211 : Index使用KT4H版32組加熱器
                                      (ATC_SYSTEM==eNewATCSystem &&
                                       iATC_Use_Heat_Count>=16) ||              //Ifor 20170814 (Steven) Add HT9046AT Direct Heater開關
                                       USE_16_HEATER==eht32HeaterDTME08);       //JimmyChiu 20210923 : Index使用DTME08版32組加熱器

            break;
        case QualSite2X2N:                                                      //Frank 20200520 2X2NN Mode
            gbArm1PickArm2Test->Visible=IniConfig.bD58UseArm1PickPlaceArm2Test; //ChungHung 20150528 add for 海思 _8Site1x4
            if(IniConfig.bDisableSelectSearchLast==true)                        //jou 2012-01-10 取消Setup，Search Last Mode功能。
            {
                rgSelectSearchLast->Visible=false;
                rgSelectSearchLast->ItemIndex=0;
            }
            else
            {
                rgSelectSearchLast->Visible=true;
            }

            rgUseSuckMode->Visible=false;

            if(CUSTOMER_CODE==CC_SCS)
            {
                cbHotechLayoutKit2x2->Visible=true;                             //jou 2015-12-10 SCS 要求 Hontech Layout kit要選擇Hontech.
            }
            else
            {
                chkOffCenterkit->Visible=CosFunction.bCanUseBias;               //Steven 20190801 : SCC李國旗說要取消BIAS選項
            }

            if(CosFunction.bEnable_1x3Kit)                                      //KevinC 20260109 : 1x2Site and 2x2 NN mode 使用1x3Site Kit
            {
                cbUse1x3siteSLK->Visible=true;
                cbUse1x3siteSLK->Caption="2x2 NN Mode use 1x3 SLK";
                cbUse1x3siteSLK->Visible=CosFunction.bEnable_1x3Kit;
            }

            rgYPitchOffsetMode->ItemIndex=0;                                    //KenHsieh 20231227 : add Y Center Pitch mode
            labYOffset   ->Visible=true;                                        //Steven 20140512 : For HT-9047
            edYOffset    ->Visible=true;                                        //Steven 20140512 : For HT-9047
            edYOffset->Text=80;                                                 //Steven 20140512 : For HT-9047 //jou 2016-05-06 32 Sites Y Offset 統一填入LoaderBoard尺寸 10 -> 80
            gbShuttleMode->Visible=false;
            lblYPitch->Visible=false;
            YPitch->Visible=false;

            XPitch->Text=80;                                                    //Steven 20100827
            YPitch->Text="0";                                                   //Steven 20100827
            iSiteTotal=4;                                                       //kevin 20160125 SITE 數設定
            break;
        case QualSite2X2: //2x2
            gbArm1PickArm2Test->Visible=IniConfig.bD58UseArm1PickPlaceArm2Test; //ChungHung 20150528 add for 海思 _8Site1x4
            if(IniConfig.bDisableSelectSearchLast==true)                        //jou 2012-01-10 取消Setup，Search Last Mode功能。
            {
                rgSelectSearchLast->Visible=false;
                rgSelectSearchLast->ItemIndex=0;
            }
            else
            {
                rgSelectSearchLast->Visible=true;
            }

            if(CUSTOMER_CODE!=CC_SCS)                                           //jou 20170518 (Steven) : SCS 要求沒用到不顯示
            {
                cbSquareOctalLayout->Visible=true;                              //Steven 20141224 : 2x2Site使用8Site Kit
                if(CUSTOMER_CODE==CC_HANA_MICRON)
                    chk2x2Use16siteSLK->Visible=true;                           //Steven 20191113 : 2x2Site使用16Site Kit
            }

            if(USE_PICKER_COUNT!=0)                                             //Steven 20161117 : for HT-9045S
            {
                rgUseSuckMode->Visible=true;
                rgUseSuckMode->Items->Strings[0]="Use 4 pick unit";
                rgUseSuckMode->Items->Strings[1]="Use 8 pick unit";
            }
            if(IniConfig.bL30Use1CableLayoutKitByConfig==false)
                cb1CableLayoutKit->Visible=true;                                //JerryYang 20181207 2x2新增一條線版本

            chkNS7000CS->Visible=true;                                          //Steven 20100119

            if(CUSTOMER_CODE==CC_TSMC_TAINAN ||
               CUSTOMER_CODE==CC_ASE_KaohSiung)                                 //kevin 20170415 (wei) KaohSiung add 2x2 Shift
            {
                cbQualSite2X2Shift->Visible=true;                               //wei 20160226 TSMC X Shift
                paQualSite2X2Shift->Visible=(TestIF_File.bQualSite2X2Shift);    //wei 20160226 TSMC X Shift
            }

            if(CosFunction.bCanUse2x2Bias)                                      //Steven 20190724 : 2x2偏心開關
            {
                chkOffCenterkit->Visible=true;                                  //Steven 20190801 : SCC李國旗說要取消BIAS選項
            }
            else                                                                //jou 2015-08-23 SCS remove 2x2 NS7000 bias kit in Tool>setup
            {
                chkOffCenterkit->Visible=false;
            }

            if(CUSTOMER_CODE==CC_SCS)
            {
                cbHotechLayoutKit2x2->Visible=true;                             //jou 2015-12-10 SCS 要求 Hontech Layout kit要選擇Hontech.
            }
            if(CUSTOMER_CODE==CC_ATEC && DeviceForm_File.iHeadDeviceCT==3)      //JerryYang 20191003 for 艾科 支援7000的2個感溫點SLK, Mars說用兩條線的選項
            {
                cb2CableLayoutKit->Visible=true;
            }
            lblYPitch->Visible=true;
            YPitch->Visible=true;

            XPitch->Text=80;                                                    //Steven 20100827
            YPitch->Text=60;                                                    //Steven 20100827
            iSiteTotal=4;                                                       //kevin 20160125 SITE 數設定
            cb16DirectHeater->Visible=(USE_16_HEATER==eht32HeaterEJ1N  ||       //kevin 20191202 add 12 以下 direct //Steven 20140923 : Index使用EJ1N版32組加熱器
                                       USE_16_HEATER==eht32HeaterKT4H  ||       //Steven 20150211 : Index使用KT4H版32組加熱器
                                      (ATC_SYSTEM==eNewATCSystem &&
                                       iATC_Use_Heat_Count>=16) ||              //Ifor 20170814 (Steven) Add HT9046AT Direct Heater開關
                                       USE_16_HEATER==eht32HeaterDTME08);       //JimmyChiu 20210923 : Index使用DTME08版32組加熱器

            break;
        case _6Site2X3:                                                         //ChungHung 20140115 add for 2x3_6
            gbArm1PickArm2Test->Visible=IniConfig.bD58UseArm1PickPlaceArm2Test; //ChungHung 20150528 add for 海思 _8Site1x4
            if(IniConfig.bDisableSelectSearchLast==true)                        //jou 2012-01-10 取消Setup，Search Last Mode功能。
            {
                rgSelectSearchLast->Visible=false;
                rgSelectSearchLast->ItemIndex=0;
            }
            else
            {
                rgSelectSearchLast->Visible=true;
            }
            rgUseSuckMode->Visible=false;

            chkNS7000CS->Visible=false;                                         //Steven 20100119
            lblYPitch->Visible=true;
            YPitch->Visible=true;
            XPitch->Text=40;                                                    //Steven 20100827
            YPitch->Text=60;                                                    //Steven 20100827
            iSiteTotal=6;                                                       //kevin 20160125 SITE 數設定
            cb16DirectHeater->Visible=(USE_16_HEATER==eht32HeaterEJ1N  ||       //kevin 20191202 add 12 以下 direct //Steven 20140923 : Index使用EJ1N版32組加熱器
                                       USE_16_HEATER==eht32HeaterKT4H  ||       //Steven 20150211 : Index使用KT4H版32組加熱器
                                      (ATC_SYSTEM==eNewATCSystem &&
                                       iATC_Use_Heat_Count>=16) ||              //Ifor 20170814 (Steven) Add HT9046AT Direct Heater開關
                                       USE_16_HEATER==eht32HeaterDTME08);       //JimmyChiu 20210923 : Index使用DTME08版32組加熱器

            break;
        case _6Site2X3N:                                                        //Steven 20220425 : 2x3 NN mode
            gbArm1PickArm2Test->Visible=false;                                  //ChungHung 20150528 add for 海思 _8Site1x4
            rgSelectSearchLast->Visible=false;
            rgSelectSearchLast->ItemIndex=0;

            rgUseSuckMode->Visible=false;

            chkNS7000CS->Visible=false;
            cbHotechLayoutKit2x2->Visible=false;

            rgYPitchOffsetMode->ItemIndex=0;                                    //KenHsieh 20231227 : add Y Center Pitch mode
            labYOffset   ->Visible=true;                                        //Steven 20140512 : For HT-9047
            edYOffset    ->Visible=true;                                        //Steven 20140512 : For HT-9047
            edYOffset->Text=80;                                                 //Steven 20140512 : For HT-9047 //jou 2016-05-06 32 Sites Y Offset 統一填入LoaderBoard尺寸 10 -> 80
            gbShuttleMode->Visible=false;
            lblYPitch->Visible=false;
            YPitch->Visible=false;

            XPitch->Text=120;
            YPitch->Text="0";
            iSiteTotal=6;                                                       //kevin 20160125 SITE 數設定
            break;
        case _8Site2X4N:                                                        //Wei 20231211 : 2X4NN Mode
            gbArm1PickArm2Test->Visible=false;                                  //ChungHung 20150528 add for 海思 _8Site1x4
            rgSelectSearchLast->Visible=false;
            if(USE_PICKER_COUNT!=0)                                             //Steven 20161117 : for HT-9045S
            {
                rgUseSuckMode->Visible=true;
                rgUseSuckMode->Items->Strings[0]="Use 4 pick unit";
                rgUseSuckMode->Items->Strings[1]="Use 8 pick unit";
                if(rgUseSuckMode->ItemIndex==1)
                {
                    if(IniConfig.bDisableSelectSearchLast==true)                //jou 2012-01-10 取消Setup，Search Last Mode功能。
                    {
                        rgSelectSearchLast->ItemIndex=0;
                    }
                    else
                    {
                        rgSelectSearchLast->Visible=true;
                    }
                }
            }

            chkNS7000CS->Visible=false;
            cbHotechLayoutKit2x2->Visible=false;

            rgYPitchOffsetMode->ItemIndex=0;                                    //KenHsieh 20231227 : add Y Center Pitch mode
            labYOffset   ->Visible=false;                                       //Steven 20140512 : For HT-9047
            edYOffset    ->Visible=false;                                       //Steven 20140512 : For HT-9047
            edYOffset->Text=80;                                                 //Steven 20140512 : For HT-9047 //jou 2016-05-06 32 Sites Y Offset 統一填入LoaderBoard尺寸 10 -> 80
            gbShuttleMode->Visible=false;
            lblYPitch->Visible=false;
            YPitch->Visible=false;

            XPitch->Text=80;
            YPitch->Text="0";
            iSiteTotal=8;                                                       //kevin 20160125 SITE 數設定
            break;
        case _8Site2X4:                                                         //2x4
            gbArm1PickArm2Test->Visible=IniConfig.bD58UseArm1PickPlaceArm2Test; //ChungHung 20150528 add for 海思 _8Site1x4
            if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                      //Steven 20231207 : 2x4 16 picker
               USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)
            {
                rgUseSuckMode->Items->Strings[0]="Use 8 pick unit";
                rgUseSuckMode->Items->Strings[1]="Use 16 pick unit";
                rgUseSuckMode->Visible=true;
            }
            else
            {
                rgUseSuckMode->Visible=false;
            }

            if(IniConfig.bDisableSelectSearchLast==true)                        //jou 2012-01-10 取消Setup，Search Last Mode功能。
            {
                rgSelectSearchLast->Visible=false;
                rgSelectSearchLast->ItemIndex=0;
            }
            else
            {
                rgSelectSearchLast->Visible=true;
            }
            rgUseSuckMode->Visible=false;
            chkNS7000CS->Visible=false;                                         //Steven 20100119

            if(CUSTOMER_CODE==CC_ATEC && DeviceForm_File.iHeadDeviceCT==4)      //JerryYang 20191003 for 艾科 支援7000的2個感溫點SLK, Mars說用兩條線的選項
            {
                cb2CableLayoutKit->Visible=true;
            }

            if(IniConfig.bL30Use1CableLayoutKitByConfig==false)                 //Sam 20210715 : Use 1CableLayoutKit By Config //Sam 20210524 2x4 新增一條線版本
                cb1CableLayoutKit->Visible=true;

            lblYPitch->Visible=true;
            YPitch->Visible=true;
            XPitch->Text=40;                                                    //Steven 20100827
            YPitch->Text=60;                                                    //Steven 20100827
            iSiteTotal=8;                                                       //kevin 20160125 SITE 數設定

            if(CUSTOMER_CODE!=CC_AMKOR_China &&
               CUSTOMER_CODE!=CC_QUALCOMM &&                                    //JerryYang 20170412 (Steven) add QUALCOMM
               IniConfig.bKoreaFunction==false)                                 //ChungHung 20121031 add
            {
                if(USE_16_HEATER==eht16Heater       ||                          //Steven 20120606 : 16溫控器 8Site使用Hontech頭
                   USE_16_HEATER==eht16HeaterEJ1N   ||
                   USE_16_HEATER==eht32HeaterEJ1N   ||                          //Steven 20140923 : Index使用EJ1N版32組加熱器
                   USE_16_HEATER==eht32HeaterKT4H   ||                          //Steven 20150211 : Index使用KT4H版32組加熱器
                   USE_16_HEATER==eht16HeaterDTME08 ||                          //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
                   USE_16_HEATER==eht32HeaterDTME08 )                           //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
                {
                    cbNS8000H->Visible=true;
                }
            }

            if(MachineTypeChoice==Type_HT9046 ||
               MachineTypeChoice==Type_HT9046_LS)                               //2013-03-05    Dell    for ATK pitch 80mm kit
            {
                if(CUSTOMER_CODE==CC_SCK)                                       //2013-03-05    Dell    for ATK pitch 80mm kit
                {
                    chkOctal80->Visible=true;
                }
                cbOctal12Site->Visible=CosFunction.bEnableOctal_12Kit;          //ChungHung 20140508 add for SCK
                cbOctal16Site->Caption="Octal site use 16 Site Layout Kit";
                cbOctal16Site->Visible=CosFunction.bEnableOctal_16Kit;          //Steven 20140312 : 8Site使用16Site Kit
            }

            cb16DirectHeater->Visible=(USE_16_HEATER==eht32HeaterEJ1N  ||       //kevin 20191202 add 12 以下 direct //Steven 20140923 : Index使用EJ1N版32組加熱器
                                       USE_16_HEATER==eht32HeaterKT4H  ||       //Steven 20150211 : Index使用KT4H版32組加熱器
                                      (ATC_SYSTEM==eNewATCSystem &&
                                       iATC_Use_Heat_Count>=16) ||              //Ifor 20170814 (Steven) Add HT9046AT Direct Heater開關
                                       USE_16_HEATER==eht32HeaterDTME08);       //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            break;
        case _16Site2X8:                                                        //2x8
            gbArm1PickArm2Test->Visible=IniConfig.bD58UseArm1PickPlaceArm2Test; //ChungHung 20150528 add for 海思 _8Site1x4
            if(IniConfig.bDisableSelectSearchLast==true)                        //jou 2012-01-10 取消Setup，Search Last Mode功能。
            {
                rgSelectSearchLast->Visible=false;
                rgSelectSearchLast->ItemIndex=0;
            }
            else
            {
                rgSelectSearchLast->Visible=true;
            }
            rgUseSuckMode->Visible=false;

            chkNS7000CS->Visible=false;                                         //Steven 20100119
            cb16change12DirectHeater->Visible=true;                             //kevin 20191104 add 16 Site Direct Heater use 12 Site shuttle
            cb16change8DirectHeater ->Visible=true;                             //Steven 20210322 : 16 Site Direct Heater use 8 Site shuttle
            lblYPitch->Visible=true;
            YPitch->Visible=true;
            XPitch->Text=30;                                                    //Steven 20100827
            YPitch->Text=60;                                                    //Steven 20100827
            iSiteTotal=16;                                                      //kevin 20160125 SITE 數設定
            cb6CableLayoutKit->Visible=(MachineTypeChoice==Type_HT1032);        //Steven 20230613 : ATC6.1 16Site SLK
            cb16DirectHeater->Visible=(USE_16_HEATER==eht32HeaterEJ1N  ||       //kevin 20191202 add 12 以下 direct //Steven 20140923 : Index使用EJ1N版32組加熱器
                                       USE_16_HEATER==eht32HeaterKT4H  ||       //Steven 20150211 : Index使用KT4H版32組加熱器
                                      (ATC_SYSTEM==eNewATCSystem &&
                                       iATC_Use_Heat_Count>=16) ||              //Ifor 20170814 (Steven) Add HT9046AT Direct Heater開關
                                       USE_16_HEATER==eht32HeaterDTME08);       //JimmyChiu 20210923 : Index使用DTME08版32組加熱器

            break;
         case _12Site2X6:                                                       //2x6  //ChungHung 20130507 add HT9045 updata for 12site 517
            gbArm1PickArm2Test->Visible=IniConfig.bD58UseArm1PickPlaceArm2Test; //ChungHung 20150528 add for 海思 _8Site1x4
            if(IniConfig.bDisableSelectSearchLast==true)                        //jou 2012-01-10 取消Setup，Search Last Mode功能。
            {
                rgSelectSearchLast->Visible=false;
                rgSelectSearchLast->ItemIndex=0;
            }
            else
            {
                rgSelectSearchLast->Visible=true;
            }
            rgUseSuckMode->Visible=false;

            if(CUSTOMER_CODE==CC_SIGURD_PeiXing)                                //Ifor 20170420 (wei) add SIGURD PeiXing Hisi 2x6 常溫 無感溫線
                chkNS7000CS->Visible=true;                                      //Steven 20100119
            else
                chkNS7000CS->Visible=false;                                     //Steven 20100119

            chk12SiteUse2x8SLK->Visible=(CosFunction.bEnable12SiteUse16SLK);    //Steven 20240807 : 12Site使用16Site Kit
            lblYPitch->Visible=true;
            YPitch->Visible=true;
            XPitch->Text=40;
            YPitch->Text=63.5;
            cb16DirectHeater->Visible=(USE_16_HEATER==eht32HeaterEJ1N  ||       //kevin 20191202 add 12 以下 direct //Steven 20140923 : Index使用EJ1N版32組加熱器
                                       USE_16_HEATER==eht32HeaterKT4H  ||       //Steven 20150211 : Index使用KT4H版32組加熱器
                                      (ATC_SYSTEM==eNewATCSystem &&
                                       iATC_Use_Heat_Count>=16) ||              //Ifor 20170814 (Steven) Add HT9046AT Direct Heater開關
                                       USE_16_HEATER==eht32HeaterDTME08);       //JimmyChiu 20210923 : Index使用DTME08版32組加熱器

            if(CUSTOMER_CODE==CC_ASE_KaohSiung)
            {
                cb12Site10DirectHeater->Visible=false;                          //kevin 20161102
            }
            else
            {
                cb12Site10DirectHeater->Visible=(USE_16_HEATER==eht32HeaterEJ1N ||    //Steven 20140923 : Index使用EJ1N版32組加熱器
                                                 USE_16_HEATER==eht32HeaterKT4H ||    //Steven 20150211 : Index使用KT4H版32組加熱器
                                                 USE_16_HEATER==eht32HeaterDTME08);   //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
            }
            iSiteTotal=12;                                                      //kevin 20160125 SITE 數設定
            break;
         case _10Site2X5:                                                       //2x5   //wei 20190614 10 site
            gbArm1PickArm2Test->Visible=IniConfig.bD58UseArm1PickPlaceArm2Test; //ChungHung 20150528 add for 海思 _8Site1x4
            if(IniConfig.bDisableSelectSearchLast==true)                        //jou 2012-01-10 取消Setup，Search Last Mode功能。
            {
                rgSelectSearchLast->Visible=false;
                rgSelectSearchLast->ItemIndex=0;
            }
            else
            {
                rgSelectSearchLast->Visible=true;
            }
            rgUseSuckMode->Visible=false;

            if(CUSTOMER_CODE==CC_SIGURD_PeiXing)                                //Ifor 20170420 (wei) add SIGURD PeiXing Hisi 2x6 常溫 無感溫線
                chkNS7000CS->Visible=true;                                      //Steven 20100119
            else
                chkNS7000CS->Visible=false;                                     //Steven 20100119

            cb6CableLayoutKit->Visible=(MachineTypeChoice==Type_HT1032);        //Steven 20230613 : ATC6.1 16Site SLK

            lblYPitch->Visible=true;
            YPitch->Visible=true;
            XPitch->Text=60;
            YPitch->Text=60;
            cb16DirectHeater->Visible=(USE_16_HEATER==eht32HeaterEJ1N ||        //Steven 20140923 : Index使用EJ1N版32組加熱器
                                       USE_16_HEATER==eht32HeaterKT4H ||        //Steven 20150211 : Index使用KT4H版32組加熱器
                                       (ATC_SYSTEM==eNewATCSystem &&
                                        iATC_Use_Heat_Count>=16));              //Ifor 20170814 (Steven) Add HT9046AT Direct Heater開關

            if(CUSTOMER_CODE==CC_ASE_KaohSiung)
            {
                cb12Site10DirectHeater->Visible=false;                          //kevin 20161102
            }
            else
            {
                cb12Site10DirectHeater->Visible=(USE_16_HEATER==eht32HeaterEJ1N ||   //Steven 20140923 : Index使用EJ1N版32組加熱器
                                                 USE_16_HEATER==eht32HeaterKT4H);    //Steven 20150211 : Index使用KT4H版32組加熱器
            }
            iSiteTotal=10;                                                      //kevin 20160125 SITE 數設定
            break;
        case _16Site4X4:                                                        //Sam 20190226 : 16Site4X4
            if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                      //Steven 20231207 : 2x4 16 picker
               USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)
            {
                rgUseSuckMode->Items->Strings[0]="Use 8 pick unit";
                rgUseSuckMode->Items->Strings[1]="Use 16 pick unit";
                rgUseSuckMode->Visible=true;
            }
            else
            {
                rgUseSuckMode->Visible=false;
            }
            cbOctal16Site->Caption="4x4 site use 4x8 site layout kit";
            cbOctal16Site->Visible=CosFunction.bEnableOctal_16Kit;              //Steven 20140312 : 8Site使用16Site Kit

        case _32Site4X8N:                                                       //4x8
            gbArm1PickArm2Test->Visible=IniConfig.bD58UseArm1PickPlaceArm2Test; //ChungHung 20150528 add for 海思 _8Site1x4
            if(IniConfig.bDisableSelectSearchLast==true)                        //jou 2012-01-10 取消Setup，Search Last Mode功能。
            {
                rgSelectSearchLast->Visible=false;
                rgSelectSearchLast->ItemIndex=0;
            }
            else
            {
                rgSelectSearchLast->Visible=true;
            }

            if(iCheckPos==_32Site4X8N)                                          //Steven 20231207 : 2x4 16 picker
            {
                rgUseSuckMode->Visible=false;
            }

            chkNS7000CS->Visible=false;                                         //Steven 20100119
            cb6CableLayoutKit->Visible=(MachineTypeChoice==Type_HT1032);        //Steven 20230613 : ATC6.1 16Site SLK

            lblYPitch->Visible=true;
            YPitch->Visible=true;
            XPitch->Text=40;
            YPitch->Text=60;
            rgYPitchOffsetMode->Visible=true;                                   //KenHsieh 20231227 : add Y Center Pitch mode
            rgYPitchOffsetMode->OnClick(this);
            labYOffset   ->Visible=true;                                        //Steven 20140512 : For HT-9047
            edYOffset    ->Visible=true;                                        //Steven 20140512 : For HT-9047
            edYOffset->Text=80;                                                 //Steven 20140512 : For HT-9047 //jou 2016-05-06 32 Sites Y Offset 統一填入LoaderBoard尺寸 10 -> 80
            gbShuttleMode->Visible=false;                                       //Steven 20140512 : For HT-9047
            cb16DirectHeater->Visible=(USE_16_HEATER==eht32HeaterEJ1N  ||       //kevin 20191202 add 12 以下 direct //Steven 20140923 : Index使用EJ1N版32組加熱器
                                       USE_16_HEATER==eht32HeaterKT4H  ||       //Steven 20150211 : Index使用KT4H版32組加熱器
                                      (ATC_SYSTEM==eNewATCSystem &&
                                       iATC_Use_Heat_Count>=16) ||              //Ifor 20170814 (Steven) Add HT9046AT Direct Heater開關
                                       USE_16_HEATER==eht32HeaterDTME08);       //JimmyChiu 20210923 : Index使用DTME08版32組加熱器

            if(iCheckPos==_16Site4X4)                                           //Sam 20190226 : 16Site4X4
            {
                iSiteTotal=16;
            }
            else
            {
                iSiteTotal=32;                                                  //kevin 20160125 SITE 數設定
                if(CosFunction.b32SiteYOffsetMode)                              //Steven 20201013 : NN mode add Y-Pitch offset selection
                    palVisibleIndex->Visible=true;                              //KenHsieh 20231227 : add Y Center Pitch mode
            }
            break;
    }

    if(iCheckPos==QualSite2X2N ||
       iCheckPos==_6Site2X3N   ||
       iCheckPos==_8Site2X4N   ||                                               //Wei 20231211 : 2X4NN Mode
       iCheckPos==_16Site4X4   ||
       iCheckPos==_32Site4X8N)
    {
        bUseTwoArm32Site=true;                                                  //kevin 20190322 add two arm tester 32SITE 16SITE(4X4)
    }
    else
    {
        bUseTwoArm32Site=false;
    }

    if(bGPIBChangeSiteMap)                                                      //JerryYang 20160113 由GPIB改setup 若YPitch為0則使用預設值
    {
        if(iCheckPos==_8Site1X4   ||
           iCheckPos==QualSite2X2 ||
           iCheckPos==_6Site2X3   ||
           iCheckPos==_8Site2X4   ||
           iCheckPos==_16Site2X8  ||
           iCheckPos==_16Site4X4  ||                                            //Sam 20190226 : 16Site4X4
           iCheckPos==_32Site4X8N)
        {
            if(TestIF_File.dSiteYPitch==0)
                TestIF_File.dSiteYPitch=60;
        }
        else if(iCheckPos==_10Site2X5 ||                                        //wei 20190614 10 site
                iCheckPos==_12Site2X6)
        {
            if(TestIF_File.dSiteYPitch==0)
                TestIF_File.dSiteYPitch=63.5;
        }
        bGPIBChangeSiteMap=false;                                               //JerryYang 20160105
    }

    if(gbArm1PickArm2Test->Visible==false &&
       gbOcr->Visible==false &&
       gbSocketClamp->Visible==false &&
       grpSocketSensor->Visible==false)
    {
        fSetup->Width=818;
        grpFunction->Visible=false;
    }
    else
    {
        fSetup->Width=1090;
        grpFunction->Visible=true;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::CompChange(int iMode)
{
    int iTestCHCT=SiteData[iMode].Cnt;

    if(IniConfig.bDualSiteSupply4CH==true)                                      //jou 2012-11-20 Dual Site supply 4's Channel
    {
        if(iMode==DualSite)
        {
            iTestCHCT+=2;
        }
    }

    if(IniConfig.bSPILFunction==true ||                                         //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
       CUSTOMER_CODE==CC_ASE_KaohSiung)
    {
        if(iMode==_8Site2X4 &&
           CosFunction.bEnableOctal_12Kit==true &&
           cbOctal12Site->Checked==true)
        {
            iTestCHCT+=4;
        }
        else if(iMode==_8Site2X4 &&
                CosFunction.bEnableOctal_12Kit==true &&
                cbOctal12Site->Checked==false)
        {
            iTestCHCT=SiteData[iMode].Cnt;
        }
    }

    if(CosFunction.bUse32ChanelSiteMap)                                         //Steven 20170530 (wei) : Use 32CH site map
    {
        iTestCHCT=32;
    }

    for(int i=0; i<MAX_SOCKET_ROW; i++)
    {
        TestLabRow[i]->Visible=false;
        for(int j=0; j<MAX_SOCKET_COL; j++)
        {
            TestSiteCH[i][j]->Clear();
            TestSiteCH[i][j]->Visible=false;
            TestLabCol[j]->Visible=false;
        }
    }

    for(int i=0; i<SiteData[iMode].XItem; i++)
    {
        TestLabCol[i]->Visible=true;
        for(int j=0; j<SiteData[iMode].YItem; j++)
        {
            TestSiteCH[j][i]->Visible=true;
            TestSiteCH[j][i]->Enabled=true;
            for(int k=0; k<=iTestCHCT; k++)
            {
                if(k==0)
                    TestSiteCH[j][i]->Items->Add("- - -");
                else
                    TestSiteCH[j][i]->Items->Add("CH "+AnsiString (k));
            }
            TestLabRow[j]->Visible=true;
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::rgShtModeNormalClick(TObject *Sender)
{
    if(rgShtModeOneSide->Checked==false)
    {
        rgUseSht1->Checked=false;
        rgUseSht2->Checked=false;
        rgUseSht1->Enabled=false;
        rgUseSht2->Enabled=false;
        rgUseSuckMode->Enabled=true;                                            //Steven 20101013
    }
    else
    {
        rgUseSht1->Checked=true;
        rgUseSht1->Enabled=true;
        rgUseSht2->Enabled=true;
        rgUseSuckMode->ItemIndex=0;                                             //Steven 20101013 : 單Arm使用多吸嘴會Hang Up
        rgUseSuckMode->Enabled=false;                                           //Steven 20101013
    }
}
//------------------------------------------------------------------------------
bool __fastcall TfSetup::CHSetError()
{
    bool bSelOne=false;
    int iXpitch, iYpitch;
    int iRow1, iCol1, iRow2, iCol2;

    for(int i=0; i<MAX_SOCKET_TOTAL; i++)
    {
        for(int j=i+1; j<MAX_SOCKET_TOTAL; j++)
        {
            iRow1=i/MAX_SOCKET_COL;
            iCol1=i%MAX_SOCKET_COL;
            iRow2=j/MAX_SOCKET_COL;
            iCol2=j%MAX_SOCKET_COL;
            if(TestSiteCH[iRow1][iCol1]->ItemIndex==-1 &&
               TestSiteCH[iRow1][iCol1]->Visible)
            {
                ShowMyMessage("Test Site Not Select");
                return true;
            }
            if(TestSiteCH[iRow1][iCol1]->Visible &&
               TestSiteCH[iRow2][iCol2]->Visible)
            {
                if(TestSiteCH[iRow1][iCol1]->ItemIndex==TestSiteCH[iRow2][iCol2]->ItemIndex &&
                   TestSiteCH[iRow1][iCol1]->ItemIndex!=0)
                {
                    ShowMyMessage("Test Site Set Same CH");
                    return true;
                }

                if(TestSiteCH[iRow1][iCol1]->ItemIndex>=1 ||
                   TestSiteCH[iRow2][iCol2]->ItemIndex>=1)
                {
                    bSelOne=true;
                }
            }
        }
    }

    if(ScrollBar1->Position==SingleSite)
    {
        TestSiteCH[0][0]->ItemIndex=1;
        bSelOne=true;
    }

    if(bSelOne==false)
    {
        ShowMyMessage("Test Site Not Select");
        return true;
    }

    iXpitch=atoi(XPitch->Text.c_str());                                         //jou 980725 start : set over limit
    iYpitch=atoi(YPitch->Text.c_str());

    if(ScrollBar1->Position!=SingleSite &&
       ScrollBar1->Position!=DualSite2x1)
    {
        if(iXpitch<20)
        {
            ShowMyMessage("X Pitch can not < 20mm!!");
            return true;
        }
    }

    switch(ScrollBar1->Position)
    {
        case SingleSite:
            break;
        case DualSite2x1:
            if(iYpitch>80)
            {
                ShowMyMessage("Y Pitch can not over 80mm");
                return true;
            }
            if(iYpitch<20)
            {
                ShowMyMessage("Y Pitch can not < 20mm");
                return true;
            }
            break;
        case DualSite:                                                          //1x2
            if(MachineTypeChoice==Type_HT9046_LS)                               //Steven 20200114 : SCK新的SLK
            {
                if(iXpitch<40 || iXpitch>240)
                {
                    ShowMyMessage("X Pitch Must Between 40~240mm");
                    return true;
                }
            }
            else if(USE_IN_OUT_ARM_X_PITCH==iXPitch40mm)                        //Isaac 20171204 (Steven) : Xpitch40->50mm 可選擇X Pitch機構模式
            {
                if(iXpitch<40 || iXpitch>120)                                   //Steven 20150505 : 1x2加大支援X-Pitch 120mm
                {
                    ShowMyMessage("X Pitch Must Between 40~120mm");
                    return true;
                }
            }
            else
            {
                if(iXpitch<40 || iXpitch>150)
                {
                    ShowMyMessage("X Pitch Must Between 40~150mm");
                    return true;
                }
            }
            break;
        case QualSite1X4:                                                       //1x4
        case _8Site1X4:                                                         //ChungHung 20150528 add for 海思 _8Site1x4
            if(USE_IN_OUT_ARM_X_PITCH==iXPitch40mm)                             //Isaac 20171204 (Steven) : Xpitch40->50mm 可選擇X Pitch機構模式
            {
                if(iXpitch>80)                                                  //Steven 20100617 : 60 -> 80
                {
                    ShowMyMessage("X Pitch can not over 80mm");
                    return true;
                }
            }
            else
            {
                if(iXpitch>100)
                {
                    ShowMyMessage("X Pitch can not over 100mm");
                    return true;
                }
            }
            break;
        case QualSite2X2:                                                       //2x2
            if(iXpitch>200)                                                     //Steven 20150224 : 韓國Danny說要180的Pitch
            {
                ShowMyMessage("X Pitch can not over 200mm");
                return true;
            }
            if(iYpitch>80)
            {
                ShowMyMessage("Y Pitch can not over 80mm");
                return true;
            }
            if(iYpitch<20)
            {
                ShowMyMessage("Y Pitch can not < 20mm");
                return true;
            }
            break;
        case _6Site2X3:                                                         //ChungHung 20140115 add for 2x3_6
            if(USE_IN_OUT_ARM_X_PITCH==iXPitch40mm)                             //Isaac 20171204 (Steven) : Xpitch40->50mm 可選擇X Pitch機構模式
            {
                if(iXpitch>120)                                                 //Steven 20200211 : 80 --> 120 for SCK
                {
                    ShowMyMessage("X Pitch can not over 120mm");
                    return true;
                }
            }
            else
            {
                if(iXpitch>100)
                {
                    ShowMyMessage("X Pitch can not over 100mm");
                    return true;
                }
            }
            if(iYpitch>80)
            {
                ShowMyMessage("Y Pitch can not over 80mm");
                return true;
            }
            if(iYpitch<20)
            {
                ShowMyMessage("Y Pitch can not < 20mm");
                return true;
            }
            break;
        case _8Site2X4:                                                         //2x4
        case _16Site4X4:                                                        //Sam 20190226 : 16Site4X4
            if(USE_IN_OUT_ARM_X_PITCH==iXPitch40mm)                             //Isaac 20171204 (Steven) : Xpitch40->50mm 可選擇X Pitch機構模式
            {
                if(iXpitch>80)                                                  //Steven 20100825 : 60 -> 80
                {
                    ShowMyMessage("X Pitch can not over 80mm");
                    return true;
                }
            }
            else
            {
                if(iXpitch>100)
                {
                    ShowMyMessage("X Pitch can not over 100mm");
                    return true;
                }
            }

            if(iYpitch>80)                                                      //Steven 20100617 : 60 -> 80
            {
                ShowMyMessage("Y Pitch can not over 80mm");
                return true;
            }
            if(iYpitch<20)
            {
                ShowMyMessage("Y Pitch can not < 20mm");
                return true;
            }
            break;
        case _10Site2X5:                                                        //wei 20190614 10 site
        case _12Site2X6:
            if(USE_IN_OUT_ARM_X_PITCH==iXPitch40mm)                             //Isaac 20171204 (Steven) : Xpitch40->50mm 可選擇X Pitch機構模式
            {
                if(iXpitch>80)
                {
                    ShowMyMessage("X Pitch can not over 80mm");
                    return true;
                }
            }
            else
            {
                if(iXpitch>100)
                {
                    ShowMyMessage("X Pitch can not over 100mm");
                    return true;
                }
            }

            if(iYpitch>80)
            {
                ShowMyMessage("Y Pitch can not over 80mm");
                return true;
            }
            if(iYpitch<30)                                                      //Steven 20170530 (wei) : 35 --> 30
            {
                ShowMyMessage("Y Pitch can not < 30mm");
                return true;
            }
            break;
        case _16Site2X8:                                                        //2x8
            if(USE_IN_OUT_ARM_X_PITCH==iXPitch50mm)                             //Isaac 20171204 (Steven) : Xpitch40->50mm 可選擇X Pitch機構模式
            {
                if(iXpitch>50)
                {
                    ShowMyMessage("X Pitch can not over 50mm");
                    return true;
                }
            }
            else
            {
                if(MachineTypeChoice==Type_HT9046_LS ||
                   SubMachineType==Type_HT9046LA)                               //Steven 20190307 : Add for HT-9046LA
                {
                    if(iXpitch>45)                                              //JerryYang 20171101 (wei) 16 site X-pitch最大值40->45
                    {
                        ShowMyMessage("X Pitch can not over 45mm");
                        return true;
                    }
                }
                else if(MachineTypeChoice==Type_HT1032)
                {
                    if(iXpitch>40)
                    {
                        ShowMyMessage("X Pitch can not over 40mm");
                        return true;
                    }
                }
                else
                {
                    if(iXpitch>30)
                    {
                        ShowMyMessage("X Pitch can not over 30mm");
                        return true;
                    }
                }
            }
            if(iYpitch>63.5)
            {
                ShowMyMessage("Y Pitch can not over 63.5mm");
                return true;
            }
            if(iYpitch<20)
            {
                ShowMyMessage("Y Pitch can not < 20mm");
                return true;
            }
            break;
        case _32Site4X8M:
            if(USE_IN_OUT_ARM_X_PITCH==iXPitch40mm ||                           //Isaac 20171204 (Steven) : Xpitch40->50mm 可選擇X Pitch機構模式
               MachineTypeChoice==Type_HT1032)
            {
                if(iXpitch>40)
                {
                    ShowMyMessage("X Pitch can not over 40mm");
                    return true;
                }
            }
            else
            {
                if(iXpitch>50)
                {
                    ShowMyMessage("X Pitch can not over 50mm");
                    return true;
                }
            }
            if(iYpitch>63.5)
            {
                ShowMyMessage("Y Pitch can not over 63.5mm");
                return true;
            }
            if(iYpitch<35)
            {
                ShowMyMessage("Y Pitch can not < 35mm");
                return true;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::FormShow(TObject *Sender)
{
    GetSetupAuth();
    gbSocketClamp->Visible=INSTALL_SOCKET_CLAMP;                                //JerryYang 20160602 有改機為分離式才顯示
    rgseparabilityTest->Visible=INSTALL_SOCKET_CLAMP;                           //JerryYang 20160602 有改機為分離式才顯示
    gbShuttleMode->Visible=true;
    cbI21->Visible=(CosFunction.bI21EnableASMByRecipe);                         //Steven 20210518 : 吳如春希望Auto site map從工作檔開關

    if(InArmSuck.HasIC()==true || OutArmSuck.HasIC()==true ||                   //Isaac 20211019 : fixed for auto site map
        ShuttleHasIC()==true   || IndexHasIC()==true)
        cbI21->Enabled=false;
    else
        cbI21->Enabled=(AccessLevel<LevelSet.AccessLevel[167])?false:true;      //Isaac 20210714 : JECT要求獨立權限設定

    if(CUSTOMER_CODE==CC_JCET)                                                  //JerryYang 20170426 (Steven) 江蘇長電吳如春要求不顯示
    {
        cbdisibleinitialcheck->Visible=false;
    }

    if(fMain->CheckCanChangeRealDummy()==false)                                 //機台內有IC
    {
        ScrollBar1          ->Visible=false;
        XPitch              ->Enabled=false;
        YPitch              ->Enabled=false;
        rgSelectSearchLast  ->Enabled=false;
        rgUseSuckMode       ->Enabled=false;
        grpSiteMap           ->Enabled=false;
        chkOffCenterkit     ->Enabled=false;
        rgInOutArmYPitch    ->Enabled=false;
        grpIndexOption      ->Enabled=false;                                    //ChungHung 20150528 add
        gbSocketClamp->Enabled      =false;                                     //JerryYang 20160602
        rgseparabilityTest->Enabled =false;
        grpUseXCenterPitch->Enabled =false;                                     //Steven 20170706 (wei) : 2x4中間的Pitch不同 for SCC
        edtRTCFileName->Enabled=false;                                          //kevin 20200804

        if(CosFunction.bOneCycleCanChangeArm==false)
        {
            gbShuttleMode->Enabled=false;
            rgShtModeNormal->Enabled=false;
            rgShtModeOneSide->Enabled=false;
        }
        else
        {
            if(ShuttleHasIC() || IndexHasIC() || InArmSuck.HasIC() || OutArmSuck.HasIC())
            {
                gbShuttleMode->Enabled=false;
                rgShtModeNormal->Enabled=false;
                rgShtModeOneSide->Enabled=false;
            }
            else if(LastSet.iTemperature==Tempture_Hot &&
                    TestIF_File.iShuttleMode==1)                                //JerryYang 20161212 加熱單ARM不能切為雙ARM,避免hang up
            {
                gbShuttleMode->Enabled=false;
                rgShtModeNormal->Enabled=false;
                rgShtModeOneSide->Enabled=false;
            }
            else
            {
                if(IniConfig.bShuttleModeAccseeLevel==true)                     //jou 2012-01-30 Yuedong Chen [Yuedong.Chen@amkor.com]
                {                                                               //請將Setup裡面的Shuttle mode在password control單獨弄一個level，類似之前修改的contact force
                    if(AccessLevel<LevelSet.AccessLevel[40])
                    {
                        gbShuttleMode->Enabled=false;
                        rgShtModeNormal->Enabled=false;
                        rgShtModeOneSide->Enabled=false;
                    }
                    else
                    {
                        gbShuttleMode->Enabled=true;
                        rgShtModeNormal->Enabled=true;
                        rgShtModeOneSide->Enabled=true;
                    }
                }
                else
                {
                    gbShuttleMode->Enabled=true;
                    rgShtModeNormal->Enabled=true;
                    rgShtModeOneSide->Enabled=true;
                }
            }
        }
    }
    else
    {
        grpIndexOption    ->Enabled=true;                                       //ChungHung 20150528 add
        grpUseXCenterPitch->Enabled=true;                                       //Steven 20170706 (wei) : 2x4中間的Pitch不同 for SCC
        edtRTCFileName    ->Enabled=true;                                       //機台內沒有IC //kevin 20200804
        rgInOutArmYPitch  ->Enabled=true;

        if((CUSTOMER_CODE==CC_AMKOR_China ||                                    //jou 2012-02-02 安靠技術員只能改shuttle mode其他的不能改
            CUSTOMER_CODE==CC_QUALCOMM) &&                                      //JerryYang 20170412 (Steven) add QUALCOMM
           AccessLevel<iDefSupervisorLevel)                                     //jou 2014-06-19 Security Have 5 Level 2->iDefSupervisorLevel
        {
            ScrollBar1          ->Visible=false;
            XPitch              ->Enabled=false;
            YPitch              ->Enabled=false;
            rgSelectSearchLast  ->Enabled=false;
            rgUseSuckMode       ->Enabled=false;
            chkOffCenterkit     ->Enabled=false;
        }
        else
        {
            ScrollBar1          ->Visible=fSecurity->Insufficient(21,false);
            XPitch              ->Enabled=fSecurity->Insufficient(21,false);
            YPitch              ->Enabled=fSecurity->Insufficient(21,false);
            rgSelectSearchLast  ->Enabled=fSecurity->Insufficient(21,false);
            rgUseSuckMode       ->Enabled=fSecurity->Insufficient(21,false);
            if(CosFunction.bCanUseBias)                                         //Steven 20190801 : SCC李國旗說要取消BIAS選項
                chkOffCenterkit ->Enabled=fSecurity->Insufficient(21,false);
            else
                chkOffCenterkit ->Enabled=false;
        }

        if(IniConfig.bShuttleModeAccseeLevel==true)                             //jou 2012-01-30 Yuedong Chen [Yuedong.Chen@amkor.com]
        {                                                                       //請將Setup裡面的Shuttle mode在password control單獨弄一個level，類似之前修改的contact force
            if(AccessLevel<LevelSet.AccessLevel[40])
            {
                gbShuttleMode->Enabled=false;
                rgShtModeNormal->Enabled=false;
                rgShtModeOneSide->Enabled=false;
            }
            else
            {
                gbShuttleMode->Enabled=true;
                rgShtModeNormal->Enabled=true;
                rgShtModeOneSide->Enabled=true;
            }
        }
        else
        {
            gbShuttleMode->Enabled=fSecurity->Insufficient(21, false);
            rgShtModeNormal->Enabled=fSecurity->Insufficient(21, false);
            rgShtModeOneSide->Enabled=fSecurity->Insufficient(21, false);
        }

        if(TestIF_File.iShuttleMode==1)                                         //Steven 20101013 單Arm使用多吸嘴會Hang Up
        {
            rgUseSuckMode->ItemIndex=0;
            rgUseSuckMode->Enabled=false;
        }
        else
        {
            if((CUSTOMER_CODE==CC_SCC ||                                        //jou 2014-06-19 Security Have 5 Level 2->iDefSupervisorLevel
               (CUSTOMER_CODE==CC_SCK && AccessLevel<=iDefSupervisorLevel)) &&
                IniConfig.bEnableRms)                                           //ChungHung 20130621 add SCK RMS
            {
                grpHandlingMode->Enabled=false;
                grpIndexOption->Enabled=false;
                rgSelectSearchLast->Enabled=false;
                rgUseSuckMode->Enabled=false;
            }
            else
            {
                grpHandlingMode->Enabled=fSecurity->Insufficient(21, false);
                grpIndexOption->Enabled=fSecurity->Insufficient(21, false);
            }
        }

        if((CUSTOMER_CODE==CC_SCC ||                                            //jou 2014-06-19 Security Have 5 Level 2->iDefSupervisorLevel
            CUSTOMER_CODE==CC_SCK) &&
            IniConfig.bEnableRms)                                               //ChungHung 20130621 add SCK RMS
        {
            grpSiteMap->Enabled=false;                                          //Steven 20111109 : grpSiteMap拉到外面來
        }
        else
        {
            if(IniConfig.bSiteMappingDisable==true)
            {
                if(iSetupSiteMapping==4)
                {
                    grpSiteMap->Enabled=false;
                }
                else
                {
                    grpSiteMap->Enabled=fSecurity->Insufficient(21, false);
                }
            }
            else
            {
                if((CUSTOMER_CODE==CC_AMKOR_China ||                            //jou 2014-06-19 Security Have 5 Level 2->iDefSupervisorLevel
                    CUSTOMER_CODE==CC_SIGURD_PeiXing ||                         //JerryYang 20160330 矽格北興也要鎖住site map
                    CUSTOMER_CODE==CC_QUALCOMM) &&                              //JerryYang 20170412 (Steven) add QUALCOMM
                    AccessLevel<iDefSupervisorLevel)
                {
                    grpSiteMap->Enabled=false;                                  //jou 2012-02-02 安靠技術員只能改shuttle mode其他的不能改
                }
                else if(IniConfig.bVTESTFunction==true &&
                        (MOT[MMTrayZ].fHasTray ||
                         MOT[MMTrayY].fHasTray ||
                         HasAnyICInMachine()))
                {
                    grpSiteMap->Enabled=false;
                    grpHandlingMode->Enabled=false;
                }
                else
                {
                    grpSiteMap->Enabled=fSecurity->Insufficient(94, false);     //Steven 20140428 : 21 --> 94
                }
            }
        }

        if(CUSTOMER_CODE==CC_SCC && IniConfig.bEnableFTP)                       //Ifor 20181023 add SCC 開啟FTP功能不可修改Setup相關資料僅可開關ARM
        {
            grpHandlingMode->Enabled=false;
            grpSiteMap->Enabled=false;
            grpIndexOption->Enabled=false;
            rgSelectSearchLast->Enabled=false;
            rgUseSuckMode->Enabled=false;
        }

        if(CosFunction.bLotStartLockCriticalPara && RunInfo.bLotStart)          //JerryYang 20220311 : ATP鎖定Critical parameter
        {
            if(bAuthCriticalPara[4])
            {
                grpHandlingMode->Enabled=false;
                grpSiteMap->Enabled=false;
            }
        }
    }

    if(IniConfig.bHaveRotateShuttle)
    {
        chkRotateShuttle->Visible=true;
    }
    else
    {
        chkRotateShuttle->Visible=false;
        chkRotateShuttle->Checked=false;
    }

    grpHeadCondition->Visible=(CUSTOMER_CODE==CC_SCC);                          //Steven 20220624 : JSCC楊建軍要求跟著工作檔跑

    grpUseXCenterPitch->Visible=(CosFunction.b2x4SupportCenterPitch==true &&
                                 (TestIF_File.iTestMode==_8Site2X4 ||           //Steven 20170706 (wei) : 2x4中間的Pitch不同 for SCC
                                  TestIF_File.iTestMode==_16Site4X4));          //Sam 20190226 : 16Site4X4

    gbPreciser->Visible=USE_PRECISER;                                           //Frank 20180410 (Steven) : InArm Preciser Station

    AnsiString S;
    OrgTestMode=TestIF.iTestMode;
    Image1->Picture->LoadFromFile(BmpPath+"2site.bmp");

    grpAutoSiteMap->Visible=(IniConfig.bVTESTFunction==true);
    fShow=true;                                                                 //Steven 20110907 : 一定要在ReadFile();前面
    ReadFile();
    bSavePressed=false;                                                         //Steven 20110907 : 一定要在ReadFile();後面
    DoIniDataToForm();
    ScrollBar1Change(this);                                                     //Steven 20120514 : 必須在DoIniDataToForm();後面
    DoIniDataToForm();                                                          //Steven 20120705 : 得再做一次,不然Pitch的數字會被改掉

    for(int i=0; i<MAX_SOCKET_ROW; i++)                                         //Steven 20120523 : 必須在ScrollBar1Change();後面
    {
        for(int j=0; j<MAX_SOCKET_COL; j++)
        {
            TestSiteCH[i][j]->ItemIndex =TestIF_File.iSiteMap[i][j];
            iRecordSiteMapOrder[i][j]   =TestIF_File.iSiteMap[i][j];            //kevin 20161003 Site 順序
        }
    }

    S.sprintf("SetUp  ''%s''  ", GetLastOpenFN());
    Caption=S;

    Left=75;                                                                    //Steven 20091103
    Top=10;                                                                     //Steven 20091103

    if(IniConfig.bSiteMappingFastSetDisable)                                    //jou 2010-11-23 start : ASE韓國不要快速設定Bin
    {
        btnLUpToRDownN->Visible=false;
        btnLUpToRDownZ->Visible=false;
        btnLDownToRUpZ->Visible=false;
        btnRUpToLDownZ->Visible=false;
        btnRDownToLUpZ->Visible=false;
        btnRUpToLDownN->Visible=false;
#ifdef SOFT_SIMULTE
        btnLUpToRDownN->Visible=true;
#endif
    }

    gbRTC->Visible=REAL_TIME_CCD;                                               //Steven 20110824 : Real time CCD - Set Up可以開關
    gbRTC->Enabled=true;
    gbOcr->Visible=INSTALL_OCR;
    gbOcr->Enabled=true;

    iTestModeOcr    =ScrollBar1->Position;                                      //Steven 20120716 : OCR
    lbShowMessage->Visible=false;
    if(CUSTOMER_CODE==CC_SIGURD_HUKOU)                                          //Sam 20240711 : RTC Auto Tuning
        cbEnableStepAsideMode->Enabled=false;

    rgInOutArmYPitch->Visible=(USE_IN_OUT_ARM_Y_PITCH==iXPitchManual635 ||
                               USE_IN_OUT_ARM_Y_PITCH==iXPitchManual360 ||      //JerryYang 20251218 : IN/OUT ARM支援不同模組
                               USE_OUT_ARM_Y_PITCH==iXPitchManual635 ||
                               USE_OUT_ARM_Y_PITCH==iXPitchManual360);

    if(ScrollBar1->Enabled==true && authSetup[0]==false)                        //Steven 20120405 : Set Up加入Security_new權限
    {
        ScrollBar1->Enabled=false;
    }
    if(YPitch->Enabled==true && authSetup[0]==false)
    {
        YPitch->Enabled=false;
    }
    if(XPitch->Enabled==true && authSetup[0]==false)
    {
        XPitch->Enabled=false;
    }
    if(rgSelectSearchLast->Enabled==true && authSetup[1]==false)
    {
        rgSelectSearchLast->Enabled=false;
    }
    if(rgUseSuckMode->Enabled==true && authSetup[2]==false)
    {
        rgUseSuckMode->Enabled=false;
    }
    if(grpIndexOption->Enabled==true && authSetup[3]==false)
    {
        grpIndexOption->Enabled=false;
    }
    if(gbShuttleMode->Enabled==true && authSetup[4]==false)
    {
        gbShuttleMode->Enabled=false;
    }
    if(grpSiteMap->Enabled==true && authSetup[5]==false)
    {
        grpSiteMap->Enabled=false;
    }
    if(gbRTC->Enabled==true && authSetup[6]==false)
    {
        gbRTC->Enabled=false;
    }

    if(IniConfig.bD58UseArm1PickPlaceArm2Test &&
       bUseTwoArm32Site==false)                                                 //Steven 20220819 : 加上保護 for D58
    {
        gbArm1PickArm2Test->Visible=true;                                       //kevin 20150127 讓setup 出現功能  Arm1 下壓 arm2 測試
        gbArm1PickArm2Test->Enabled=(AccessLevel>=LevelSet.AccessLevel[165]);
    }
    else
    {
        gbArm1PickArm2Test->Visible=false;                                      //kevin 20150127 讓setup 出現功能  Arm1 下壓 arm2 測試
        TestIF_File.bArm1PickPlaceArm2Test=false;
    }

    if(IniConfig.bIndexArm2SupplyLight==true ||                                 //jou 2012-10-19 Index Arm 2 供應光源 for CMOS
       TestIF_File.bForEgisTecTest==true ||                                     //Steven 20140922 : Arm2當作指紋測試
       (TestIF_File.bArm1PickPlaceArm2Test &&                                   //kevin 20150127 Arm1 下壓 arm2 測試
        IniConfig.bD58UseArm1PickPlaceArm2Test==true))                          //Ifor 20191122 : 避免D58功能關閉無法開ARM問題
    {
        gbShuttleMode->Visible=false;                                           //only use shuttle 1
    }
    else
    {
        gbShuttleMode->Visible=IniConfig.bD30EnableSiteModeSelect;
    }

    if(gbOcr->Enabled==true && authSetup[7]==false)                             //ChungHung 20130118 add OCR - 關閉要密碼
    {
        gbOcr->Enabled=false;
    }

    if(CUSTOMER_CODE==CC_KYEC_LEE)
        cbOcrFunction->Enabled=(AccessLevel>=iDefSupervisorLevel)?true:false;   //Alick 20170106 add OCR權限設定在SUPERVISOR以上

    //這兩個要保持在最下面---------------------------------
    bSiteMapHasChange=false;                                                    //Steven 20250102 : 紀錄test mode與site map是否有變更
    iTestMode=TestIF_File.iTestMode;
    for(int i=0; i<MAX_SOCKET_ROW; i++)
    {
        for(int j=0; j<MAX_SOCKET_COL; j++)
        {
            iTestSiteCh[i][j]=TestIF_File.iSiteMap[i][j];
        }
    }

    iASMTestMode=ScrollBar1->Position;                                          //Steven 20120927 : for auto site mapping
    for(int i=0; i<MAX_SOCKET_ROW; i++)
        for(int j=0; j<MAX_SOCKET_COL; j++)
            iASMSiteMap[i][j]=TestSiteCH[i][j]->ItemIndex;                      //Steven 20120927 : for auto site mapping

     if(IniConfig.bC08_SocketSensor)                                            //kevin 20130504 socket sensor enanle
     {
         grpSocketSensor->Visible=true;
         grpSocketSensor->Enabled=(AccessLevel>=LevelSet.AccessLevel[159]);
     }
     else
     {
         grpSocketSensor->Visible=false;
     }

    if(CUSTOMER_CODE==CC_KYEC_LEE || CUSTOMER_CODE==CC_KYEC_XILINX)             //20140320 wei   KYEC 低於權限顯示不能修改
    {
        if(AccessLevel<LevelSet.AccessLevel[21])
        {
            grpHandlingMode->Enabled=false;
            grpSiteMap->Enabled=false;
            rgSelectSearchLast->Enabled=false;
            rgUseSuckMode->Enabled=false;
            grpIndexOption->Enabled=false;
        }                                                                       //Frank 20161212 (Jou) Mark Code 機台內有IC就不能更改Setup裡的Site
//        else
//        {
//            grpHandlingMode->Enabled=true;
//            grpSiteMap->Enabled=true;
//            rgSelectSearchLast->Enabled=true;
//            rgUseSuckMode->Enabled=true;
//            grpIndexOption->Enabled=true;
//        }
    }

    if(CUSTOMER_CODE==CC_SCS)
    {
        if(TestIF_File.bNS7000kit==true ||
           TestIF_File.bNS7000CS==true ||
           TestIF_File.bNS8000CS==true)
        {
            ShowMyMessage("You are using Epson layout kit");
        }
        else
        {
            ShowMyMessage("You are using HonPrec layout kit");
        }
        chkNS7000CS->Caption="NS7000 layout kit";
    }

    if(CosFunction.bLockRTC && LastSet.iTester==ON_LINE)                        //JerryYang 20160220 add for Amkor-Philippine,避免人員開關RTC功能
    {
        cbEnableRealTimeCCD->Enabled=false;
    }
    else if(CosFunction.bLockRTC && LastSet.iTester==OFF_LINE)
    {
        cbEnableRealTimeCCD->Enabled=true;
    }

    paQualSite2X2Shift->Visible=((TestIF_File.iTestMode==QualSite2X2 ||
                                  TestIF_File.iTestMode==DualSite) &&           //kevin 20170415 (wei) add DualSite
                                 TestIF_File.bQualSite2X2Shift);                //wei 20160226 TSMC X Shift

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //kevin 20160316
    {
        cb16DirectHeater->Caption="OnebyOne Direct Heater";                     //kevin 20210923 add 20200417 改名稱
        chkOffCenterkit->Caption="NS7000 bias kit(偏心Kit)";
        chkNS7000CS->Caption="NS7000 change socket(真空位置不同)";
        if(TestIF_File.bNSKitPress)
        {
            ShowMyMessage("You are using Epson layout kit");
        }

        if(bUseNSKitKey)                                                        //kevin 20170814 (Steven) 低FORCE 需軟體鎖住
            cbNSKitPress->Visible=true;                                         //kevin 20170804

        tsCheckTorque->TabVisible=true;                                         //kevin 20210806 add 扭力比對功能
        tsTrayMap->TabVisible=true;
        cb16DirectHeater->Caption="12/16 Site One by One Direct heater layout";         //kevin 20220503 change name
        cb12Site10DirectHeater->Caption="12 Site 10 One by One Direct heater layout";   //kevin 20220503 change name
    }
    else
    {
        tsCheckTorque->TabVisible=false;
        tsTrayMap->TabVisible=false;
        pgcASE->Visible=false;
    }

    if(INSTALL_DOUBLE_EP==2)
    {
        cbIndSLK->Visible=true;
    }
    else
    {
        cbIndSLK->Visible=false;
    }

    if(CUSTOMER_CODE==CC_SIGURD_PeiXing)                                        //Alick 20160602 新增矽格Site map Password用ini檔
    {
        bNeedEnterPassword=true;                                                //Alick 20160603 修改Site map需要密碼
        AnsiString sSigPath="C:\\Windows\\SitMap.ini";                          //Alick 20160603 密碼ini檔存位置
        FILE *fp;
        if(FileExists(sSigPath)==false)
        {
            fp=fopen("C:\\Windows\\SitMap.ini", "w+");
            fprintf(fp, "%s\n", "9045");
            fclose(fp);
        }
        else
        {
            TStringList *sTmp = new TStringList;
            sTmp->LoadFromFile(sSigPath);
            sSigPassword=sTmp->Strings[0];
            sTmp->Clear();                                                      //Ifor 20170603 (wei) TStringList 刪除前先 Clean
            delete sTmp;
        }
    }
    if(IniConfig.bSPILFunction==true)                                           //jou 20170418 (Steven) : 矽品-世明要求修改config F18至Recipe設定
        cbF18->Visible=true;
    btAutoShuttlePitch->Visible=(AUTO_SENSOR_INSTALL==1);                       //wei 20160914 Auto Shuttle Sensor

    cb16DirectHeater->Enabled=fSecurity->Insufficient(155, false);              //JerryYang 20180416 (Steven) : MicroChip Philippine要求加入權限控管

    cbUseTesterDry->Visible=Sen[SnTesterDryAir].Enable;                         //Ifor 20200115 : add Tester Dry Air Control
    if(CosFunction.bLockRTCByFile)                                              //Sam 20240311 : 新增 RTC Lock by file 功能
    {
        fConfiguration->ReadLockByFile();
        cbEnableRealTimeCCD->Enabled=IniConfig.bRTC_Enable;
        if(IniConfig.bRTC_Enable==false)
            cbEnableRealTimeCCD->Checked==IniConfig.bRTC_Active;
    }

    if(CosFunction.bDisableOpenAllSiteWhenChangeShtMod)
    {
        SetShtMode(rgShtModeNormal->Checked,
                   rgShtModeOneSide->Checked,
                   rgUseSht1->Checked,
                   rgUseSht2->Checked);
    }

    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);                                  //Steven 20100629
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::ReadUseSuckModeFile()
{
    AnsiString szDir=GetRecipeFileName("HandlerCondition.Data");
    TestIF_File.iUseSuckMode=ReadIniData(szDir, "Configuration", "Use Suck Mode", 4);

    if(IndexHasIC() || ShuttleHasIC() || InArmSuck.HasIC() ||
       MOT[MMPlate1].HasIC() || MOT[MMPlate2].HasIC())                          //Steven 20160213 : 解決Input擺放順序問題
    {

    }
    else
    {
        ResetShuttleWhichKit();
    }
}
//------------------------------------------------------------------------------
void TfSetup::CheckSTMMode()                                                    //Steven 20221006 : 確認STM模式
{
    iCloseSiteModeFor2x8=e2x8Standard;                                          //JerryYang 20191122 STM 8 site Auto clean支援前後排一起丟
    iCloseSiteModeFor2x6=e2x6Standard;                                          //Steven 20240417 : 12site關中間4site
    iCloseSiteModeFor1x4=e1x4Standard;

    if(TestIF_File.iTestMode==_12Site2X6)
    {
        CheckSTMMode_2x6_8();
    }
    else if(TestIF_File.iTestMode==_16Site2X8 ||
            TestIF_File.iTestMode==_32Site4X8N)
    {
        CheckSTMMode_2x8_8();
    }
    else if(TestIF_File.iTestMode==TriSite1X3 &&
            iInArmType==e9045_1x3_2_14)
    {
        CheckCloseSiteMode_1x3_14();                                            //Steven 20251113 : for 1x3_14 run 3x7 HP
    }
    else if(TestIF_File.iTestMode==_6Site2X3N &&
            iInArmType==e9045_1x3_2_14)
    {
        CheckCloseSiteMode_2x3N_14();                                            //Steven 20251113 : for 1x3_14 run 3x7 HP
    }   
    else if(TestIF_File.iTestMode==QualSite1X4)
    {
        CheckCloseSiteMode_1x4_4();                                             //Steven 20241111 : for 1x4 close site
    }
}
//------------------------------------------------------------------------------
int TfSetup::GetTestMode(AnsiString sTestMode)                                  //Steven 20250321 : 整合TestMode轉換
{
    for(int i=0; i<TotalTestMode; i++)
    {
        if(sTestMode==TestSiteFileName[0][i])
        {
            return i;
        }
    }
    return 0;
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::ReadFile()
{
    AnsiString str, str1, str2;
    int iSearch;
    bool bEnableCCDBuffer=false, bEnableCCDBuffer2=false;                       //Sam 20240311 : 新增 RTC Lock by file 功能
    static bool bFirstReadFile=true;
    AnsiString szDir=GetRecipeFileName("HandlerCondition.Data");

    TestIF_File.sTestMode=ReadIniData(szDir, "Configuration", "Test Mode", AnsiString("")); //Steven 20120814 Start: 改用文字存模式,方便新增新的模式

    if(TestIF_File.sTestMode=="")                                               //向舊版的相容,不要動!!
    {
        iSearch=ReadIniData(szDir, "Configuration", "Handling Mode", 0);
        switch(iSearch)
        {
            case 0:  TestIF_File.sTestMode="2-Site"                         ; break;
            case 1:  TestIF_File.sTestMode="Single Site"                    ; break;
            case 2:  TestIF_File.sTestMode="In-Line 4-Site(1X4)"            ; break;
            case 3:  TestIF_File.sTestMode="Square 4-Site(2X2)"             ; break;
            case 4:  TestIF_File.sTestMode="Square 4-Site(2X2) Busy Shuttle"; break;
            case 5:  TestIF_File.sTestMode="8-Site"                         ; break;
            case 6:  TestIF_File.sTestMode="16-Site"                        ; break;
            case 7:  TestIF_File.sTestMode="12-Site"                        ; break;
            case 8:  TestIF_File.sTestMode="32-Site N Mode"                 ; break;
            case 9:  TestIF_File.sTestMode="32-Site M Mode"                 ; break;
            case 10: TestIF_File.sTestMode="2-Site Busy Shuttle"            ; break;
        }
    }

    TestIF_File.iTestMode=GetTestMode(TestIF_File.sTestMode);                   //Steven 20250321 : 整合TestMode轉換

    if(CosFunction.bEnable6Site==false &&                                       //Steven 20100709 : HT9045讀到16Site的設定檔時，強制設定成8Site
       TestIF_File.iTestMode==_6Site2X3)
    {
        TestIF_File.iTestMode=_8Site2X4;
    }
    else if(MachineTypeChoice==Type_HT9045)                                     //9045
    {
        if(TestIF_File.iTestMode==_16Site2X8 ||
           TestIF_File.iTestMode==_12Site2X6 ||
           TestIF_File.iTestMode==_16Site4X4)
            TestIF_File.iTestMode=_8Site2X4;
        else if(TestIF_File.iTestMode==QualSite2X2N)                            //Steven 20200722 : 2x2 NN mode
            TestIF_File.iTestMode=QualSite2X2;
        else if(TestIF_File.iTestMode==_6Site2X3N)                              //Steven 20220425 : 2x3 NN mode
             TestIF_File.iTestMode=_8Site2X4;
        else if(TestIF_File.iTestMode==_8Site2X4N)                              //Wei 20231211 : 2X4NN Mode
             TestIF_File.iTestMode=_8Site2X4;
    }
    else if(MachineTypeChoice==Type_HT9046 ||
            SubMachineType==Type_HT9046LA)                                      //Steven 20190307 : Add for HT-9046LA
    {
        if(TestIF_File.iTestMode==_32Site4X8M ||
           TestIF_File.iTestMode==_32Site4X8N ||
           TestIF_File.iTestMode==_16Site4X4)                                   //Sam 20190226 : 16Site4X4
            TestIF_File.iTestMode=_16Site2X8;
        else if(TestIF_File.iTestMode==QualSite2X2N)                            //Steven 20200722 : 2x2 NN mode
            TestIF_File.iTestMode=QualSite2X2;
        else if(TestIF_File.iTestMode==_6Site2X3N)                              //Steven 20220425 : 2x3 NN mode
             TestIF_File.iTestMode=_8Site2X4;
        else if(TestIF_File.iTestMode==_8Site2X4N)                              //Wei 20231211 : 2X4NN Mode
             TestIF_File.iTestMode=_8Site2X4;
    }
    else if(MachineTypeChoice==Type_HT9045_12Site)                              //ChungHung 20130507 add HT9045 updata for 12site 517
    {
        if(TestIF_File.iTestMode==_16Site2X8)
            TestIF_File.iTestMode=_8Site2X4;
        else if(TestIF_File.iTestMode==QualSite2X2N)                            //Steven 20200722 : 2x2 NN mode
            TestIF_File.iTestMode=QualSite2X2;
        else if(TestIF_File.iTestMode==_6Site2X3N)                              //Steven 20220425 : 2x3 NN mode
             TestIF_File.iTestMode=_8Site2X4;
        else if(TestIF_File.iTestMode==_8Site2X4N)                              //Wei 20231211 : 2X4NN Mode
             TestIF_File.iTestMode=_8Site2X4;
    }
    else
    {
        if(TestIF_File.iTestMode==QualSite2X2N &&
           CosFunction.bCanUse2x2NNMode==false)                                 //Steven 20200722 : 2x2 NN mode
            TestIF_File.iTestMode=QualSite2X2;
        if(TestIF_File.iTestMode==_6Site2X3N &&
           CosFunction.bCanUse2x3NNMode==false)                                 //Steven 20220425 : 2x3 NN mode
            TestIF_File.iTestMode=_8Site2X4;
        if(TestIF_File.iTestMode==_8Site2X4N &&
           CosFunction.bCanUse2x4NNMode==false)                                 //Wei 20231211 : 2X4NN Mode
            TestIF_File.iTestMode=_8Site2X4;
    }

    if(TestIF_File.iTestMode==_8Site2X4     ||
       TestIF_File.iTestMode==QualSite1X4   ||
       TestIF_File.iTestMode==DualSite      ||
       TestIF_File.iTestMode==QualSite2X2   ||
       TestIF_File.iTestMode== _8Site1X4    ||                                  //ChungHung 20150528 add for 海思 _8Site1x4
       TestIF_File.iTestMode==_6Site2X3     ||                                  //ChungHung 20150119 add for 2X3_6
       TestIF_File.iTestMode==_12Site2X6    ||                                  //ChungHung 20141031 add 12Site HotPlate Check //Steven 20120323 : 可以檢查HotPlate的模式
       TestIF_File.iTestMode==_10Site2X5    ||
       TestIF_File.iTestMode==_16Site4X4    ||                                  //Sam 20190226 : 16Site4X4
       TestIF_File.iTestMode==_16Site2X8)                                       //Isaac 20191007 : add Hotplate check
    {
        bCanUseHotPlateCheck=true;
    }
    else
    {
        bCanUseHotPlateCheck=false;
    }

    if(CUSTOMER_CODE==CC_ASE_SG && TestIF_File.iTestMode==SingleSite)
    {
        bCanUseHotPlateCheck=true;
    }

    if(USE_PICKER_COUNT==ep1Picker)
        bCanUseHotPlateCheck=true;

    if(USE_16_HEATER==eht32HeaterEJ1N   ||
       USE_16_HEATER==eht32HeaterKT4H   ||                                      //Steven 20150211 : Index使用KT4H版32組加熱器
       USE_16_HEATER==eht32HeaterDTME08 ||                                      //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
       (ATC_SYSTEM==eNewATCSystem &&                                            //Ifor 20170814 (Steven) Add HT9046AT Direct Heater開關
        iATC_Use_Heat_Count>=8))                                                //kevin 20180504 add
    {
        TestIF_File.bUse32Heater        =ReadIniData(szDir, "Configuration", "12_16SiteDirectHeaterLayout", 0);   //Steven 20140923 : Index使用EJ1N版32組加熱器
        TestIF_File.b12SiteUse10Heater  =ReadIniData(szDir, "Configuration", "12Site10DirectHeaterLayout", 0);    //Steven 20140923 : Index使用EJ1N版32組加熱器
    }
    else
    {
        TestIF_File.bUse32Heater        =false;
        TestIF_File.b12SiteUse10Heater  =false;
    }
    IniConfig.iSiteMapDirection  =ReadIniData(szDir, "Configuration", "SiteMapDirection", 0);   //Jimmychiu 20230807 : #R230804-ATK-H9-01 , V3.21.792.1 ,Add the Sitemap items in information.txt

    int iSite=0;
    int K=1;
    if(CUSTOMER_CODE==CC_ASE_KaohSiung_K3)                                      //kevin 20211130 K5 強制 SITE 順序  林建續
    {                                                                           //kevin 20211130 add K3 K5 Site 順序固定
        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                str.sprintf("Site %c%c", i+'A', j+'a');                         //kevin 20180312 change  j+'A'==> j+'a'
                iSite=ReadIniData(szDir, "Configuration", str, 0);
                if(iSite==0)
                {
                    TestIF_File.iSiteMap[i][j]=0;                               //關SITE
//                    if(TestIF_File.iSiteMap[i][j]<=0)                         //Jimmychiu 20230807 : Mark for not use
//                    {
//                        TestIF_File.iSiteMap[i][j]=0; //kevin 20150427
//                    }
                }
                else
                {
                    TestIF_File.iSiteMap[i][j]=K;
                    if(TestIF_File.iSiteMap[i][j]<=0)
                    {
                        TestIF_File.iSiteMap[i][j]=0;                           //kevin 20150427
                    }
                }
                tSiteMap->Strings[i*MAX_SOCKET_COL+j]=AnsiString(TestIF_File.iSiteMap[i][j]);   //Steven 20140510 : Secs Gem
                iAutoSiteMap[i][j]=0;                                           //kevin 20150114
                K++;
            }
        }
    }
    else
    {
        for(int i=0; i<MAX_SOCKET_ROW; i++)
        {
            for(int j=0; j<MAX_SOCKET_COL; j++)
            {
                str.sprintf("Site %c%c", i+'A', j+'a');                         //kevin 20180312 change  j+'A'==> j+'a'
                TestIF_File.iSiteMap[i][j]=ReadIniData(szDir, "Configuration", str, 0);
                if(TestIF_File.iSiteMap[i][j]<=0)
                {
                    TestIF_File.iSiteMap[i][j]=0;                               //kevin 20150427
                }
                tSiteMap->Strings[i*MAX_SOCKET_COL+j]=AnsiString(TestIF_File.iSiteMap[i][j]);   //Steven 20140510 : Secs Gem

                iAutoSiteMap[i][j]=0;                                           //kevin 20150114
            }
        }
    }

    if(CUSTOMER_CODE==CC_SCC)                                                   //Steven 20220624 : JSCC楊建軍要求跟著工作檔跑
    {
        IniConfig.bLifeTimeCount[0]=ReadIniData(szDir, "O_Count", "bLifeTimeCount 1", false);
        IniConfig.bLifeTimeCount[1]=ReadIniData(szDir, "O_Count", "bLifeTimeCount 2", false);
        IniConfig.bLifeTimeCount[2]=ReadIniData(szDir, "O_Count", "bLifeTimeCount 3", false);
    }

    if(CosFunction.bI21EnableASMByRecipe)                                       //Steven 20210518 : 吳如春希望Auto site map從工作檔開關
    {
        if(IniConfig.bVTESTFunction==true)
            IniConfig.bI21EnableASM=ReadIniData(szDir, "Auto Site Mapping", "Enable Auto Site Mapping", true);
        else
            IniConfig.bI21EnableASM=ReadIniData(szDir, "Auto Site Mapping", "Enable Auto Site Mapping", false);

        if(CosFunction.bUseOpenCloseSiteMapAtAnyTime==true)                     //Ifor 20190620 : add Auto Site Maping 隨時關閉開關顯示
        {
            fMain->cbDisableSiteMappingCheck->Visible=IniConfig.bI21EnableASM;
        }
        else
        {
            fMain->cbDisableSiteMappingCheck->Visible=false;
        }
    }

    TestIF_File.iShuttleMode=ReadIniData(szDir, "Configuration", "Shuttle Mode", 0);
    TestIF_File.iShuttle_Sel=ReadIniData(szDir, "Configuration", "Shuttle1 Cancel", 0);

    if(TestIF_File.iShuttleMode==0)                                             //kevin 20220329 開兩支ARM只使用arm2 生產
    {
        TestIF_File.iShuttle_Sel=0;
    }
    iSelectArm=0;                                                               //kevin 20220426 0: Arm1 Arm 2  1: Arm1  2: Arm2
    if(TestIF_File.iShuttleMode==1)                                             //kevin 20220329 開兩支ARM只使用arm2 生產
    {
        if(TestIF_File.iShuttle_Sel==0)
            iSelectArm=1;
        else
            iSelectArm=2;
    }

    if(IniConfig.bD58UseArm1PickPlaceArm2Test)
        TestIF_File.bArm1PickPlaceArm2Test=ReadIniData(szDir, "Configuration", "bArm1PickPlaceArm2Test", false);  //JerryYang 20160108 要先讀取bArm1PickPlaceArm2Test再修改iShuttleMode
    else
        TestIF_File.bArm1PickPlaceArm2Test=false;
    if(IniConfig.bIndexArm2SupplyLight==true ||                                 //jou 2012-10-19 Index Arm 2 供應光源 for CMOS
       TestIF_File.bForEgisTecTest==true     ||                                 //Steven 20140922 : Arm2當作指紋測試
       (TestIF_File.bArm1PickPlaceArm2Test && IniConfig.bD58UseArm1PickPlaceArm2Test==true))       //kevin 20150127 Arm1 下壓 arm2 測試//Ifor 20191122 : 避免D58功能關閉無法開ARM問題
    {
        TestIF_File.iShuttleMode=1;                                             //only use shuttle 1
        TestIF_File.iShuttle_Sel=0;
    }

    if(TestIF_File.bArm1PickPlaceArm2Test)
    {
        TestIF_File.bRTC20CheckFunction =ReadIniData (szDir, "Configuration", "bRTC20CheckFunction", false); //Ifor 20190712 : add RTC2.0 Check
        TestIF_File.bRTC20GiveWayCheck  =ReadIniData  (szDir, "Configuration", "bRTC20GiveWayCheck", false);  //Ifor 20190712 : add RTC2.0 Check
    }
    else
    {
        TestIF_File.bRTC20CheckFunction =false;
        TestIF_File.bRTC20GiveWayCheck  =false;
    }

    TestIF_File.bRTCICResidueCheck=ReadIniData  (szDir, "Configuration", "bRTCICResidueCheck", false);  //Ifor 20190712 : add RTC2.0 Check
    TestIF_File.bRTC20OverFlowCheck=ReadIniData (szDir, "Configuration", "bRTC20OverFlowCheck", false); //Ifor 20190712 : add RTC2.0 Check

    TestIF_File.bArm1UseHeat=ReadIniData        (szDir, "Configuration", "bArm1UseHeat", false);        //Ifor 20190815 : add Arm1是否加熱開關
    TestIF_File.bArm1PickPlaceArm2Test_RunAutoClean=ReadIniData(szDir, "Configuration", "bArm1PickPlaceArm2Test_RunAutoClean", false);//Jimmychiu 20230710 : Auto Clean 跑 Arm1 下壓 arm2 測試

    if(IsNNMode()==NN_2Row)
    {
        TestIF_File.iShuttleMode=0;
    }

    TestIF_File.dSiteXPitch =ReadIniData(szDir, "Configuration", "X Pitch", 0.0);
    TestIF_File.dSiteYPitch =ReadIniData(szDir, "Configuration", "Y Pitch", 0.0);

    CheckSTMMode();                                                             //Steven 20221006 : 確認STM模式

    if(CosFunction.bNonCenterModeCanUseShtOffset && IniConfig.bA50Enable1x4BiasYOffset && TestIF_File.iTestMode==QualSite1X4)      //Steven 20200715 : for Tinton
        TestIF_File.i1x4SiteYOffset=ReadIniData(szDir, "Configuration", "i1x4SiteYOffset", 0);

    if(IsNNMode()==NN_2Row)                                                     //Steven 20201013 : NN mode add Y-Pitch offset selection //KenHsieh 20231227 : add Y Center Pitch mode
    {
        TestIF_File.iYPitchOffsetMode=ReadIniData(szDir, "Configuration", "iYPitchOffsetMode", 0);
    }
    else
    {
        TestIF_File.iYPitchOffsetMode=0;
    }

    TestIF_File.dSiteYOffset=ReadIniData(szDir, "Configuration", "Y Offset", 0.0);  //Steven 20140512 : For HT-9047

    TestIF_File.dShiftXPitch=ReadIniData(szDir, "Configuration", "X Shift Pitch", 0.0); //wei 20160226 TSMC X Shift

    TestIF_File.dPreciserXPitch =ReadIniData(szDir, "Configuration", "Preciser X Pitch", 0.0);    //Frank 20180410 (Steven) : InArm Preciser Station
    TestIF_File.dPreciserYPitch =ReadIniData(szDir, "Configuration", "Preciser Y Pitch", 0.0);

    if(TestIF_File.iTestMode<DualSite2x1 ||                                     //Steven 20140917 : 加上保護
       TestIF_File.iTestMode==QualSite2X2N ||                                   //Steven 20241220 : Add
       TestIF_File.iTestMode==_6Site2X3N ||
       TestIF_File.iTestMode==_8Site2X4N)
    {
        TestIF_File.dSiteYPitch=0;
    }

    iSearch=ReadIniData(szDir,"Configuration", "Search Last Mode", 0);
    if(iSearch==0)
        TestIF_File.bSearchLastMode=false;
    else
        TestIF_File.bSearchLastMode=true;

    if(TestIF_File.iShuttleMode==1)
    {
        switch(TestIF_File.iTestMode)
        {
            case DualSite:                                                      //1x2
                TestIF_File.iUseSuckMode=2;
                WriteIniData(szDir, "Configuration", "Use Suck Mode", 2);
                break;
            case QualSite1X4:                                                   //1x4
            case _8Site1X4:                                                     //ChungHung 20150528 add for 海思 _8Site1x4
            case QualSite2X2:                                                   //2x2
                TestIF_File.iUseSuckMode=4;
                WriteIniData(szDir, "Configuration", "Use Suck Mode", 4);
                break;
            case _8Site2X4:
            case _16Site4X4:
                if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                  //Steven 20231207 : 2x4 16 picker
                   USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)
                {
                    TestIF_File.iUseSuckMode=16;
                    WriteIniData(szDir, "Configuration", "Use Suck Mode", 16);
                }
                break;
        }
    }
    else
    {
        TestIF_File.iUseSuckMode=ReadIniData(szDir, "Configuration", "Use Suck Mode", 4);
    }

    if(bFirstReadFile==true)                                                    //Ifor 20161116 取得 Use Suck Mode 初始值
    {
        bFirstReadFile=false;
        iUseSuckModeBackup=TestIF_File.iUseSuckMode;                            //Ifor 20161116 add Brackup Use Suck Mode
    }

    if(IniConfig.bEnableAutoCleanFunction)
    {
        fCleaning->LoadAutoCleanData();                                         //ChungHung 20131120 AutoClean use Hotplate1      //JimmyChiu 20220121 mark 重複執行
        fCleaning->SaveAutoCleanData();                                         //Jimmychiu 20260122 : fixed for auto clean hang up for setup close arm diff
    }

    if(TestIF_File.iTestMode==QualSite2X2)                                      //Steven 20190724 : 2x2偏心開關
    {
        if(CosFunction.bCanUse2x2Bias)
            TestIF_File.bNS7000kit=ReadIniData(szDir, "Configuration", "NS7000 bias kit",       false);
        else
            TestIF_File.bNS7000kit=false;
    }
    else
    {
        if(CosFunction.bCanUseBias)                                             //Steven 20190801 : SCC李國旗說要取消BIAS選項
            TestIF_File.bNS7000kit=ReadIniData(szDir, "Configuration", "NS7000 bias kit",         false);
        else
            TestIF_File.bNS7000kit=false;
    }

    TestIF_File.bNS8000CS=ReadIniData(szDir, "Configuration", "NS8000 Change Socket",    false);   //Steven 20120606 : 16溫控器 8Site使用Hontech頭
    if(MachineTypeChoice==Type_HT1032)
        TestIF_File.b6CableLayoutKit=ReadIniData(szDir, "Configuration", "6 Cable Layout Kit",  false);   //Steven 20230613 : ATC6.1 16Site SLK

    TestIF_File.b2CableLayoutKit=ReadIniData(szDir, "Configuration", "2 Cable Layout Kit",      false);   //Steven 20150724 : 16溫控器 2條線版本
    TestIF_File.b1CableLayoutKit=ReadIniData(szDir, "Configuration", "1 Cable Layout Kit",      false);   //jou 2015-10-15  : 16溫控器 1條線版本

    TestIF_File.bNS7000CS       =ReadIniData(szDir, "Configuration", "NS7000 Change Socket",    false);   //Steven 20100119
    TestIF_File.bSingleHeater   =ReadIniData(szDir, "Configuration", "Single site Use single heater", false);//JerryYang 20161013 新增Single site一支加熱棒模式
    if(CUSTOMER_CODE==CC_TSMC_TAINAN)
        TestIF_File.bQualSite2X2Shift=ReadIniData(szDir, "Configuration", "QualSite2X2 Shift",      false);   //wei 20160226 TSMC X Shift
    else
        TestIF_File.bQualSite2X2Shift=false;

    if(TestIF_File.iTestMode==SingleSite)
    {
        TestIF_File.bSingleUseOtherSuck =ReadIniData(szDir, "Configuration", "Enable Single Use Other Suck",    false);   //wei 20220905 Single Use Other Suck(IN C OUT E)
        TestIF_File.bSingleInArmUseOtherSuck =ReadIniData(szDir, "Configuration", "Enable Single In Use Other Suck",    false);   //Ifor 20240430 add: Single Site InArm Use Other Suck(IN C)
    }
    else
    {
        TestIF_File.bSingleUseOtherSuck =false;                                 //wei 20220905 Single Use Other Suck(IN C OUT E)
        TestIF_File.bSingleInArmUseOtherSuck =false;                            //Ifor 20240430 add: Single Site InArm Use Other Suck(IN C)
    }
    TestIF_File.bRotateShuttle  =ReadIniData(szDir, "Configuration", "Rotate Shuttle",          false);         //Steven 20100713 : 轉轉蝦頭
    if(INSTALL_OCR==eocrUninstal)                                               //Steven 20160122 : 確保是關閉的
        TestIF_File.bOcrFunction=false;
    else
        TestIF_File.bOcrFunction=ReadIniData(szDir, "Configuration", "OCR Function",            false);         //Steven 20120716 : OCR

    if(INSTALL_SOCKET_CLAMP==false)                                             //JerryYang 20160523
        TestIF_File.bUseSLKClamp=false;
    else
        TestIF_File.bUseSLKClamp=ReadIniData(szDir, "Configuration", "Socket Clamp Function", false);

    if(TestIF_File.bUseSLKClamp==false)                                         //JerryYang 20160602
        TestIF_File.iSeparabilityTest=0;
    else
        TestIF_File.iSeparabilityTest=ReadIniData(szDir, "Configuration", "Enable Separability Test",       0);

    TestIF_File.sOcrText        =ReadIniData(szDir, "Configuration", "OCR Text",  AnsiString(""));              //Steven 20120716 : OCR

    TestIF_File.bEnableUsePreciser        =ReadIniData(szDir, "Configuration", "Enable Preciser",  false);      //Frank 20180410 (Steven) : InArm Preciser Station
    TestIF_File.bEnableRTPreciser         =ReadIniData(szDir, "Configuration", "Enable RT Preciser",  false);   //Frank 20180410 (Steven) : InArm Preciser Station

    if(IniConfig.bVTESTFunction==true)                                          //jou 20200701 : VTEST for auto site mapping cable mount
    {
        TestIF_File.bAutoSiteMappingOpenSite=ReadIniData(szDir, "Configuration", "Auto site mapping open site", true);
        TestIF_File.bAutoSiteMappingOneCycle=ReadIniData(szDir, "Configuration", "Auto site mapping one cycle", true);
    }

    if(IniConfig.bC08_SocketSensor)
    {
        TestIF_File.bEnSocketSensor            =ReadIniData(szDir, "Configuration", "SocketSensor",                 false);     //kevin 20130504 使用SOCKET SENSOR
        #ifdef MTK_Version
        TestIF_File.bEnSocketSensor            =true;
        #endif
        TestIF_File.bSocketSensorCheckFloating =ReadIniData(szDir, "Configuration", "bSocketSensorCheckFloating",   false);     //Steven 20180313 (jou) : 使用Socket Sensor驗證置偏
        TestIF_File.iSocketCount               =ReadIniData(szDir, "Configuration", "SocketCountt",                 8);         //kevin 20130504 使用多少顆SENSOR 最多8個

        if(CUSTOMER_CODE == CC_ASE_KaohSiung)                                   //kevin 20210924 ASE -KH 不卡SOCKET SENSOR數量
        {

        }
        else if(TestIF_File.iTestMode>=_16Site4X4)                              //Steven 20190919 : SocketSensor加上數量保護
        {
            if(TestIF_File.iSocketCount<4)
                TestIF_File.iSocketCount=4;
        }
        else if(TestIF_File.iTestMode>DualSite2x1)                              //Steven 20200306 : 2x1最少只需要一顆, 其他兩排的要兩顆以上
        {
            if(TestIF_File.iSocketCount<2)
                TestIF_File.iSocketCount=2;
        }

        TestIF_File.bCheckSocketFloating=false;                                 //Steven 20200420 : Socket Sensor功能可以選
        for(int i=0; i<iSnSocketCnt; i++)
        {
            str.sprintf("iSensorCheckType%d", i+1);
            if(i<TestIF_File.iSocketCount)
            {
                if(TestIF_File.bSocketSensorCheckFloating)
                    TestIF_File.iSensorCheckType[i]=ReadIniData(szDir, "Configuration", str, 2);
                else
                    TestIF_File.iSensorCheckType[i]=ReadIniData(szDir, "Configuration", str, 1);
            }
            else
            {
                TestIF_File.iSensorCheckType[i]=ReadIniData(szDir, "Configuration", str, 0);
            }
            if(TestIF_File.iSensorCheckType[i]==2)
                TestIF_File.bCheckSocketFloating=true;
        }

        if(CUSTOMER_CODE==CC_JCET)                                              //JerryYang 20170426 (Steven) 江蘇長電吳如春要求不顯示
            TestIF_File.bSocketDisibleinitialcheck=false;
        else
            TestIF_File.bSocketDisibleinitialcheck =ReadIniData(szDir, "Configuration", "SocketDisibleinitialcheck",    false); //kevin 20160209 (wei) 使用SOCKET SENSOR
    }
    else
    {
        TestIF_File.bEnSocketSensor=false;
    }
    if(CUSTOMER_CODE==CC_GIGAS)                                                 //Isaac 20210601 : 全智要把指定參數存出來
    {
        fRPDefault->ShowMonitoredParameter();
    }

    if(IsNNMode()!=None_NN)
    {
        bUseTwoArm32Site=true;                                                  //kevin 20190322 add two arm tester 32SITE 16SITE(4X4)
    }
    else
    {
        bUseTwoArm32Site=false;
    }

    TestIF_File.bCheckArm2Vacuum          =ReadIniData(szDir, "Configuration", "bCheckArm2Vacuum",  true);  //Steven 20150129 : 需要確認Arm2有沒有粘料

    fOCR->SetOcrInspText(TestIF_File.sOcrText);                                 //Steven 20120716 : OCR
    fOCR->cbEnableOcr->Checked  =TestIF_File.bOcrFunction;                                                  //Steven 20120716 : OCR
        TestIF_File.bOctal_80Kit    =ReadIniData(szDir, "Configuration", "Octal Pitch 80",              false); //2013-03-05    Dell    for ATK pitch 80mm kit

    TestIF_File.bSquare_OctalKit    =ReadIniData(szDir, "Configuration", "Square Octal Kit",        false); //Steven 20141224 : 2x2Site使用8Site Kit
    if(CUSTOMER_CODE==CC_HANA_MICRON)
        TestIF_File.b2x2Use16SiteKit=ReadIniData(szDir, "Configuration", "2x2 Use 16 Site Kit",     false); //Steven 20191113 : 2x2Site使用16Site Kit
    else
        TestIF_File.b2x2Use16SiteKit=false;

    if(CosFunction.bEnableOctal_16Kit)                                          //Steven 20140312 : 8Site使用16Site Kit
        TestIF_File.bOctal_16Kit    =ReadIniData(szDir, "Configuration", "Octal 16 Kit",            false); //Steven 20140312 : 8Site使用16Site Kit
    else
        TestIF_File.bOctal_16Kit    =false;

    if(CosFunction.bEnableOctal_12Kit)                                          //ChungHung 20140508 add for SCK
        TestIF_File.bOctal_12Kit    =ReadIniData(szDir, "Configuration", "Octal 12Kit",             false); //ChungHung 20140508 add for SCK
    else
        TestIF_File.bOctal_12Kit    =0;
    if(CosFunction.bEnableDual_1x4Kit)                                          //Isaac 20200311 : 1x2Site使用1x4Site Kit
        TestIF_File.b1x2Use1x4SiteKit =ReadIniData(szDir, "Configuration", "1x2_Use_1x4_Kit",       false);
    else
        TestIF_File.b1x2Use1x4SiteKit =false;

    if(CosFunction.bEnable_1x3Kit)                                              //KevinC 20260109 : 1x2Site and 2x2 NN mode 使用1x3Site Kit
        TestIF_File.bUse1x3SiteKit =ReadIniData(szDir, "Configuration", "Use_1x3_Kit",       false);
    else
        TestIF_File.bUse1x3SiteKit =false;

    if(CosFunction.bEnable12SiteUse16SLK)                                       //Steven 20240807 : 12Site使用16Site Kit
        TestIF_File.b2x6Use2x8SitSLK  =ReadIniData(szDir, "Configuration", "b2x6Use2x8SitSLK",        false);
    else
        TestIF_File.b2x6Use2x8SitSLK  =false;

    TestIF_File.bHontechLayoutKit2x2=ReadIniData(szDir, "Configuration", "bHontechLayoutKit2x2",    false); //jou 2015-12-10 SCS 要求 Hontech Layout kit要選擇Hontech.
    TestIF_File.bQualSite2X2Shift   =ReadIniData(szDir, "Configuration", "QualSite2X2 Shift",       false); //kevin 20171117 add

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)
        TestIF_File.bNSKitPress=ReadIniData(szDir, "Configuration", "bNSKitPress",  false);                 //kevin 20170804 (Steven) add 另一LOAD CELL 壓力值
    else
        TestIF_File.bNSKitPress=false;

    if(INSTALL_DOUBLE_EP==2 && TestIF_File.iTestMode==DualSite)
        TestIF_File.bIndEPSLK=ReadIniData(szDir, "Configuration", "bIndEPSLK",  false);
    else
        TestIF_File.bIndEPSLK=false;

    TestIF_File.b16Direct12Shuttle  =ReadIniData(szDir, "Configuration", "b16Direct12Shuttle",      false); //kevin 20191104 add 16 Site Direct Heater use 12 Site shuttle
    TestIF_File.b16Direct8Shuttle   =ReadIniData(szDir, "Configuration", "b16Direct8Shuttle",       false); //Steven 20210322 : 16 Site Direct Heater use 8 Site shuttle

    if(CosFunction.bLockRTCByFile)                                              //Sam 20240311 : 新增 RTC Lock by file 功能
    {
        fConfiguration->ReadLockByFile();
    }
    if(CosFunction.bLockRTC && LastSet.iTester==ON_LINE)                        //Steven 20191202 : 強制開啟RTC
    {
        bEnableCCDBuffer=false;
    }
    else if(IniConfig.bRTCbySystem)                                             //ChungHung 20120716 RTC by System
    {
        if(CosFunction.bLockRTCByFile && IniConfig.bRTC_Enable==false)          //Sam 20240311 : 新增 RTC Lock by file 功能
        {
            bEnableCCDBuffer2=ReadIniData("D:\\HT9045\\config\\config.ini", "RTC", "Enable", false);
            bEnableCCDBuffer=!IniConfig.bRTC_Active;
            if(bEnableCCDBuffer==false && bEnableCCDBuffer2==true)
            {
                ShowMyMessage("Program need to restart to active RTC fuinction!", "The program will automatically be closed after exit setup form.");
                fMain->bNeedRestartProgram=true;
            }
            WriteIniData("D:\\HT9045\\config\\config.ini", "RTC", "Enable", (!bEnableCCDBuffer)  ?0:1);
        }
        else
        {
            bEnableCCDBuffer=ReadIniData("D:\\HT9045\\config\\config.ini", "RTC", "Enable", false);
        }
    }
    else
    {
        if(CosFunction.bLockRTCByFile && IniConfig.bRTC_Enable==false)          //Sam 20240311 : 新增 RTC Lock by file 功能
        {
            bEnableCCDBuffer2=ReadIniData(szDir, "Configuration", "Real Time CCD", false);
            bEnableCCDBuffer=!IniConfig.bRTC_Active;
            if(bEnableCCDBuffer==false && bEnableCCDBuffer2==true)
            {
                ShowMyMessage("Program need to restart to active RTC fuinction!", "The program will automatically be closed after exit setup form.");
                fMain->bNeedRestartProgram=true;
            }
            WriteIniData(szDir, "Configuration", "Real Time CCD",       (!bEnableCCDBuffer)  ?0:1);
        }
        else
        {
            bEnableCCDBuffer=ReadIniData(szDir, "Configuration", "Real Time CCD", false);   //Steven 20110824 : Real time CCD - Set Up可以開關
        }
    }
//    #endif
    TestIF_File.bUseSocketFloat =ReadIniData(szDir, "Configuration", "bUseSocketFloat",          false);  //Frank QQ
    TestIF_File.bUseSocketFloat =false;                                         //Ifor 20180301 :未驗證完成暫時不用強制關閉

    if(TestIF_File.bNS7000kit)                                                  //Steven 20241113 : 偏心的kit不使用StepAsideMode
    {
        TestIF_File.bUseRTCStepAsideMode=false;
    }
    else if(CUSTOMER_CODE==CC_SIGURD_HUKOU)                                     //Sam 20240709 : 矽格湖口家葳要求強制開啟
    {
        TestIF_File.bUseRTCStepAsideMode=true;
    }
    else
    {
        TestIF_File.bUseRTCStepAsideMode =ReadIniData(szDir, "Configuration", "bUseRTCStepAsideMode", false);  //Ifor 20230908 add:RTC 讓位功能 By Setup File
    }

    if(InitialOK==true && COM2->bCCDDummyRum==true && bEnableCCDBuffer==false)
    {
        fMain->bNeedRestartProgram=true;
    }
#ifdef SOFT_SIMULTE
    COM2->bCCDDummyRum=true;
#else
    COM2->bCCDDummyRum=bEnableCCDBuffer;
#endif

    if(MachineTypeChoice==Type_HT9046_LS && bEnableCCDBuffer==true)             //2013-01-15    Dell   提高Index 運動速度(目前by9046LS專用)
        bGail_HighSpeedMode=true;

    if(REAL_TIME_CCD)
    {
        if(!COM2->bCCDDummyRum)
        {
            if(fShow)                                                           //如果有打開SetUp
            {
                if(bSavePressed)                                                //而且有存檔
                {
                    if(iTestMode!=ScrollBar1->Position)                         //如果有切換模式
                    {
                        iTestMode=ScrollBar1->Position;                         //Steven 20120319 : 防止Site Mode改來改去
                        RecordProcess("Delete ROI by Save setup");              //Steven 20220815 : 紀錄重置ROI
                        fLotInfo->RTCChangeFile();                              //那就要初使化工作檔
                    }
                }
            }
            else                                                                //沒有打開SetUp,表示是外部控制
            {
                if(bFirstTime==false)
                    RecordProcess("Delete ROI by Read setup");                  //Steven 20220815 : 紀錄重置ROI
                fLotInfo->RTCChangeFile(!bFirstTime);                           //所以也要初始化工作檔
            }
        }
        else
        {
            fLotInfo->LoadRTCFullViewImg(false);                                //Steven 20110825 : Real Time CCD - 使用白色圖
        }
    }

    if(INSTALL_OCR!=eocrUninstal)                                               //Steven 20120716 : OCR
    {
        fTemperFrom->SetShowYield(fTemperFrom->esytOcrOn, TestIF_File.bOcrFunction);
        if(TestIF_File.bOcrFunction)
        {
            if(fShow)
            {
                if(bSavePressed)
                {
                    if(iTestModeOcr!=ScrollBar1->Position)                      //如果有切換模式
                    {
                        iTestModeOcr=ScrollBar1->Position;                      //Steven 20120319 : 防止Site Mode改來改去
                        fOCR->SetOcrFileName(fMain->cbSetupFileName->Text);
                    }
                }
            }
            else
            {
                fOCR->SetOcrFileName(fMain->cbSetupFileName->Text);             //所以也要初始化工作檔
            }

            fMain->SendMSG_CMD(MSG_CMD_EnableBarCode);                          //Steven 20160122 : 確保是關閉的    //Steven 20160223 : 換位置
        }
        else
        {
            fMain->SendMSG_CMD(MSG_CMD_DisableBarCode);                         //Steven 20160122 : 確保是關閉的    //Steven 20160223 : 換位置
        }
    }

    if(USE_IN_OUT_ARM_Y_PITCH==iXPitch60)                                       //jou 2012-05-15 : 選擇 Y Pitch 機構模式
    {
        TestIF_File.iARM_Y_PITCH=6000;
    }
    else if(USE_IN_Y_IS_AUTO_PITCH==true || USE_OUT_Y_IS_AUTO_PITCH==true)                                             //Steven 20131002 : XY變距  //JerryYang 20251218 : IN/OUT ARM支援不同模組
    {
        TestIF_File.iARM_Y_PITCH=6000;
    }
    else
    {
        if(rgInOutArmYPitch->ItemIndex==iXPitch60)
        {
            TestIF_File.iARM_Y_PITCH=ReadIniData(szDir, "Configuration", "In Out Arm Y Pitch", 6000);
        }
        else
        {
            if(USE_IN_OUT_ARM_Y_PITCH==iXPitchManual635)
                TestIF_File.iARM_Y_PITCH=ReadIniData(szDir, "Configuration", "In Out Arm Y Pitch", 6350);
            else if(USE_IN_OUT_ARM_Y_PITCH==iXPitchManual360)                   //Steven 20140819 : Y-Pitch 36mm
                TestIF_File.iARM_Y_PITCH=ReadIniData(szDir, "Configuration", "In Out Arm Y Pitch", 3600);
        }
    }
    fHotPlate->SetArmHotPlateYPitch();                                          //Steven 20180122 : For Y-Pitch at Hot Plate

    if(CosFunction.bInOutArmUseBackRowSuck==true)                               //jou 20161122 (Steven) In & Out arm use back row suck
    {
        TestIF_File.bInArmUseBackRowSuck=ReadIniData(szDir, "Configuration", "In Arm Use Back Row", false);
        TestIF_File.bOutArmUseBackRowSuck=ReadIniData(szDir, "Configuration", "Out Arm Use Back Row", false);
    }
    else
    {
        TestIF_File.bInArmUseBackRowSuck=false;
        TestIF_File.bOutArmUseBackRowSuck=false;
    }
    TestIF_File.bUseTesterDry=CheckAndReadIniData(szDir, "Configuration", "Use Tester Dry Air", false); //Ifor 20200306 : Add Tester Dry Air讀不到檔案需將預設值寫入檔案
    TestIF_File.sRtcFileName =ReadIniData(szDir, "Configuration", "RTCFileName", fMain->cbSetupFileName->Text);    //kevin 20200731 RTC FILENAME

    if(CosFunction.b2x4SupportCenterPitch==false ||                             //Steven 20170706 (wei) : 2x4中間的Pitch不同 for SCC
       TestIF_File.iTestMode!=_8Site2X4)
    {
        TestIF_File.bEnableUseXCenterPitch=false;
    }
    else
    {
        TestIF_File.bEnableUseXCenterPitch  =ReadIniData(szDir, "Configuration", "Enable X Center Pitch", false);
        TestIF_File.dSiteXCenterPitch       =ReadIniData(szDir, "Configuration", "X Center Pitch", 0.0);
    }

    fStartCondition->ReadWriteStartCondition(true);                             //Steven 20250822 : 調整位置

    TestIF_File.bEnableReadAndCheckTorque            =CheckAndReadIniData(szDir, "Configuration", "EnableReadAndCheckTorque",                 IniConfig.bD01_1EnableReadAndCheckTorque);
    TestIF_File.dReadTorque                          =CheckAndReadIniData(szDir, "Configuration", "dReadTorque",                              IniConfig.dD01ReadTorque);
    TestIF_File.dReadTorqueDelayTime                 =CheckAndReadIniData(szDir, "Configuration", "dReadTorqueDelayTime",                     IniConfig.dD01ReadTorqueDelayTime);

    bool Test=false;
    if(Test)
        SetMotorSpeed();

    if(INDEX_PRESS_TYPE==e240KG || INDEX_PRESS_TYPE==e260KG)                    //Steven 20111003
        fTemp_Set->ReadTempFile(true);

    if(ATC_SYSTEM!=eNewATCSystem)                                               //Ifor 20160514 Add ATC 不需要重新排列溫控器位置，會出現錯誤的位置
    {
        if(USE_16_HEATER>=eht16Heater)                                          //Steven 20150128 : 移動到最下面,要先讀取完參數才行
        {
            fTemperFrom->SetIndex16HeaterPos();                                 //Steven 20111208 : 重新排列溫控器位置
        }
    }

    InitShuttleThreadParameter();
    fMain->bEnableAutoclean();                                                  //kevin 20130429 autoclean

    ChangeHotPlateDataToOneShuttle();                                           //JerryYang 20171218 (Steven) fix 加熱模式雙arm切單arm hang up問題

    if(IndexHasIC() || ShuttleHasIC() || InArmSuck.HasIC() ||
       MOT[MMPlate1].HasIC() || MOT[MMPlate2].HasIC())                          //Steven 20160213 : 解決Input擺放順序問題
    {

    }
    else
    {
        ResetShuttleWhichKit();
    }

    if(CosFunction.bRotateUseHT7000HPKit && USE_ROTATE_KIT)                     //Sam 20210416 : 新增特殊模式 For Rotate Function HT7000 HP Kit
        TestIF_File.UseRotateForHT7000HPKit=ReadIniData(szDir, "Configuration", "UseRotateForHT7000HPKit", false);
    else
        TestIF_File.UseRotateForHT7000HPKit=false;

    TestIF_File.bF18InshuttleDetect =ReadIniData(szDir, "Configuration", "bF18InshuttleDetect", false); //jou 20170418 (Steven) : 矽品-世明要求修改config F18至Recipe設定
    fLotInfo->ShowSocketID();                                                   //JerryYang 20190708 包成函式
    ATKRecipeInfo->SaveFile();                                                  //Steven 20170901 (wei) : For ATK要新增工作檔比對用的檔案
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::DoIniDataToForm()
{
    ScrollBar1->Position = TestIF_File.iTestMode;

    for(int i=0; i<MAX_SOCKET_ROW; i++)
        for(int j=0; j<MAX_SOCKET_COL; j++)
            TestSiteCH[i][j]->ItemIndex=TestIF_File.iSiteMap[i][j];

    if(TestIF_File.iShuttleMode==0)
    {
        rgShtModeNormal->Checked=true;
        iOldShuttleMode=0;
        iWhichShuttle0000=0;                                                    //KEVIN 20170504關Arm2 load tray 不會吸IC  Hang up

        if(TestIF_File.iUseSuckMode==4)                                         //Jimmychiu 20240219 : add for 8site2x4N
        {
            rgUseSuckMode->ItemIndex=0;
        }
        else if(TestIF_File.iUseSuckMode==8)
        {
            rgUseSuckMode->ItemIndex=1;
        }
    }
    else
    {
        rgShtModeOneSide->Checked=true;
        iOldShuttleMode=1;
    }

    if(TestIF_File.iShuttle_Sel==0)
    {
        rgUseSht1->Checked=true;
        iOldSelShuttle=0;
        iWhichShuttle0000=0;                                                    //KEVIN 20170504關Arm2 (wei) load tray 不會吸IC  Hang up
    }
    else
    {
        rgUseSht2->Checked=true;
        iOldSelShuttle=1;
        if(TestIF_File.iShuttleMode==1)
            iWhichShuttle0000=1;                                                //KEVIN 20170504 (wei) 關Arm2 load tray 不會吸IC  Hang up
    }

    rgYPitchOffsetMode->ItemIndex     = TestIF_File.iYPitchOffsetMode;          //Steven 20201013 : NN mode add Y-Pitch offset selection

    if(IniConfig.bSPILFunction==true)                                           //Ifor 20160614 矽品要求Hotplate 僅可到小數後兩位 (矽品 IT 系統限制)
    {
        XPitch->Text   = FormatFloat("0.00", TestIF_File.dSiteXPitch);          //Ifor 20160614 Setup pitch "0.000" -> "0.00"
        YPitch->Text   = FormatFloat("0.00", TestIF_File.dSiteYPitch);          //Ifor 20160614 Setup pitch "0.000" -> "0.00"
        if(TestIF_File.iYPitchOffsetMode!=1)                                    //KenHsieh 20231227 : add Y Center Pitch mode   //Steven 20201013 : NN mode add Y-Pitch offset selection
        {
            edYOffset->Text= FormatFloat("0.00", TestIF_File.dSiteYOffset);     //Ifor 20160614 Setup pitch "0.000" -> "0.00"
        }
        else
        {
            edYOffset->Text=10.00;
        }
    }
    else
    {
        XPitch->Text   = FormatFloat("0.000", TestIF_File.dSiteXPitch);         //jou 2016-04-22 Setup pitch "0.00" -> "0.000"
        YPitch->Text   = FormatFloat("0.000", TestIF_File.dSiteYPitch);         //jou 2016-04-22 Setup pitch "0.00" -> "0.000"
        if(TestIF_File.iYPitchOffsetMode!=1)                                    //KenHsieh 20231227 : add Y Center Pitch mode   //Steven 20201013 : NN mode add Y-Pitch offset selection
        {
            edYOffset->Text= FormatFloat("0.000", TestIF_File.dSiteYOffset);    //Steven 20140512 : For HT-9047 //jou 2016-04-22 Setup pitch "0.00" -> "0.000"
        }
        else
        {
            edYOffset->Text=10.000;
        }
    }

    edPreciserXPitch->Text=FormatFloat("0.000", TestIF_File.dPreciserXPitch);   //Frank 20180410 (Steven) : InArm Preciser Station
    edPreciserYPitch->Text=FormatFloat("0.000", TestIF_File.dPreciserYPitch);
    rgYOffset->ItemIndex  =TestIF_File.i1x4SiteYOffset;

    XShiftPitch->Text= FormatFloat("0.00", TestIF_File.dShiftXPitch);           //wei 20160226 TSMC X Shift

    if(TestIF_File.bSearchLastMode==false)
        rgSelectSearchLast->ItemIndex=0;
    else
        rgSelectSearchLast->ItemIndex=1;

    switch(TestIF_File.iTestMode)
    {
        case DualSite:                                                          //Steven 2012.09.21 : 反向
            if(TestIF_File.iUseSuckMode==4)
            {
                rgUseSuckMode->ItemIndex=1;                                     // 4 pick unit
            }
            else
            {
                rgUseSuckMode->ItemIndex=0;                                     // 2 pick unit
            }
            break;
        case QualSite1X4:
        case QualSite2X2:
        case _8Site1X4:                                                         //ChungHung 20150528 add for 海思 _8Site1x4
            if(USE_PICKER_COUNT!=0)                                             //Steven 20161117 : for HT-9045S
            {
                if(TestIF_File.iUseSuckMode==8)
                    rgUseSuckMode->ItemIndex=1;                                 // 8 pick unit
                else
                    rgUseSuckMode->ItemIndex=0;                                 // 4 pick unit
            }
            else
            {
                rgUseSuckMode->ItemIndex=0;
            }
            break;
        case _8Site2X4:
        case _16Site4X4:
            if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                      //Steven 20231207 : 2x4 16 picker
               USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)
            {
                if(TestIF_File.iUseSuckMode==16)
                    rgUseSuckMode->ItemIndex=1;                                 // 16 pick unit
                else
                    rgUseSuckMode->ItemIndex=0;                                 // 4 pick unit
            }
            else
            {
                rgUseSuckMode->ItemIndex=0;
            }
            break;
    }

    chkOffCenterkit     ->Checked   = TestIF_File.bNS7000kit;                   //jou 981208 : NS7000 bias kit
    chkNS7000CS         ->Checked   = TestIF_File.bNS7000CS;                    //Steven 20100119
    cbQualSite2X2Shift->Checked     = TestIF_File.bQualSite2X2Shift;            //wei 20160226 TSMC X Shift
    chkRotateShuttle    ->Checked   = TestIF_File.bRotateShuttle;               //Steven 20100713 : 轉轉蝦頭
    cbEnableRealTimeCCD ->Checked   = !COM2->bCCDDummyRum;                      //Steven 20110824 : Real time CCD - Set Up可以開關
    bNeedPassword                   = cbEnableRealTimeCCD->Checked;             //Steven 20110907 : Real Time CCD - 關掉要密碼
    cb16change12DirectHeater->Checked=TestIF_File.b16Direct12Shuttle;           //kevin 20191104 add 16 Site Direct Heater use 12 Site shuttle
    cb16change8DirectHeater ->Checked=TestIF_File.b16Direct8Shuttle;            //Steven 20210322 : 16 Site Direct Heater use 8 Site shuttle

    cbEnableSocketFloat ->Checked   =TestIF_File.bUseSocketFloat;               //Frank QQ
    cbEnableSocketFloat ->Visible   =false;                                     //Ifor 20180301 :未驗證完成暫時不用強制關閉

    cbEnableStepAsideMode->Checked  =TestIF_File.bUseRTCStepAsideMode;          //Ifor 20230908 add:RTC 讓位功能 By Setup File

    if(IniConfig.bKoreaFunction==false)                                         //ChungHung 20121031 add
        cbNS8000H       ->Checked   =TestIF_File.bNS8000CS;                     //Steven 20120606 : 16溫控器 8Site使用Hontech頭
    cb2CableLayoutKit->Checked      =TestIF_File.b2CableLayoutKit;              //Steven 20150724 : 16溫控器 2條線版本
    cb1CableLayoutKit   ->Checked   =TestIF_File.b1CableLayoutKit;              //jou 2015-10-15  : 16溫控器 1條線版本
    cb6CableLayoutKit   ->Checked   = TestIF_File.b6CableLayoutKit;             //Steven 20230613 : ATC6.1 16Site SLK
    cbOcrFunction       ->Checked   =TestIF_File.bOcrFunction;                  //Steven 20120716 : OCR
    bOCRNeedPassword                =cbOcrFunction->Checked;                    //ChungHung 20130118 add OCR - 關閉要密碼
    chkOctal80          ->Checked   = TestIF_File.bOctal_80Kit;                 //2013-03-05    Dell    for ATK pitch 80mm kit

    if(CUSTOMER_CODE==CC_SCC)                                                   //Steven 20220624 : JSCC楊建軍要求跟著工作檔跑
    {
        cbO12_LifeTimeCount->Checked=IniConfig.bLifeTimeCount[0];
        cbO13_LifeTimeCount->Checked=IniConfig.bLifeTimeCount[1];
        cbO14_LifeTimeCount->Checked=IniConfig.bLifeTimeCount[2];
    }

    if(CUSTOMER_CODE==CC_JCET &&
       bFirstTimeEnableSocketSensor)                                            //Ifor 20171121 (Steven) : add JCET 如春要求第一次啟動時須強制開啟Socket Sensor 功能
    {
        AnsiString szDir=GetRecipeFileName("HandlerCondition.Data");
        bFirstTimeEnableSocketSensor=false;

        if(JCET_FOR_EVAN)                                                       //Steven 20210806 : 嚴海軍要求切換工作檔時, Socket Sensor與Site Map要默認啟用
        {
            IniConfig.bI21EnableASM=true;
            WriteIniData(szDir, "Auto Site Mapping", "Enable Auto Site Mapping", 1);
            if(CosFunction.bUseOpenCloseSiteMapAtAnyTime==true)                 //Ifor 20190620 : add Auto Site Maping 隨時關閉開關顯示
            {
                fMain->cbDisableSiteMappingCheck->Visible=IniConfig.bI21EnableASM;
            }
            else
            {
                fMain->cbDisableSiteMappingCheck->Visible=false;
            }
        }

        if(IniConfig.bC08_SocketSensor && IniConfig.bD65EnableCheckSocketsensorFunction)
        {
            TestIF_File.bEnSocketSensor=true;
            WriteIniData(szDir, "Configuration", "SocketSensor", 1);
        }
    }

    #ifdef MTK_Version
    cbSocketSensor      ->Checked   =true;
    cbSocketSensor      ->Enabled   =false;
    #else
    cbSocketSensor      ->Checked   = TestIF_File.bEnSocketSensor;              //kevin 20130504  使用SOCKET SENSOR
    cbSocketSensor      ->Enabled   = (AccessLevel>=LevelSet.AccessLevel[159]); //Steven 20191129 : Socket sensor加入權限控制
    #endif
    if(CosFunction.bLotStartLockCriticalPara && RunInfo.bLotStart)              //JerryYang 20220311 : ATP鎖定Critical parameter
    {
        if(bAuthCriticalPara[15])
        {
            cbSocketSensor->Enabled=false;
        }
    }

    cbdisibleinitialcheck->Checked  = TestIF_File.bSocketDisibleinitialcheck;   //kevin 20170209 (wei) 不使用SOCKET SENSOR initial check
    cbNSKitPress->Checked           = TestIF_File.bNSKitPress;                  //kevin 20170804 (Steven) add 另一LOAD CELL 壓力值
    CoSocketCombo       ->ItemIndex = TestIF_File.iSocketCount-1;               //kevin 20130504 使用多少顆SENSOR 最多8個
    cbIndSLK->Checked               = TestIF_File.bIndEPSLK;                    //kevin 20170804 (Steven) add 另一LOAD CELL 壓力值

    for(int i=0; i<iSnSocketCnt; i++)                                           //Steven 20200420 : Socket Sensor功能可以選
    {
        MyTempRGBox[i]->ItemIndex=TestIF_File.iSensorCheckType[i];
        if(i<TestIF_File.iSocketCount)
        {
            MyTempRGBox[i]->Visible=true;
            if(MyTempRGBox[i]->ItemIndex==0)
            {
                if(TestIF_File.bSocketSensorCheckFloating)
                    MyTempRGBox[i]->ItemIndex=2;
                else
                    MyTempRGBox[i]->ItemIndex=1;
            }
        }
        else
        {
            MyTempRGBox[i]->Visible=false;
            MyTempRGBox[i]->ItemIndex=0;
        }
    }

    if(CosFunction.bI21EnableASMByRecipe)                                       //Steven 20210518 : 吳如春希望Auto site map從工作檔開關
        cbI21->Checked=IniConfig.bI21EnableASM;

    cbOctal16Site       ->Checked   =TestIF_File.bOctal_16Kit;                  //Steven 20140312 : 8Site使用16Site Kit
    cbSquareOctalLayout ->Checked   = TestIF_File.bSquare_OctalKit;             //Steven 20141224 : 2x2Site使用8Site Kit
    chk2x2Use16siteSLK  ->Checked   =TestIF_File.b2x2Use16SiteKit;              //Steven 20191113 : 2x2Site使用16Site Kit
    cb1x2Use1x4siteSLK  ->Checked   = TestIF_File.b1x2Use1x4SiteKit;            //Isaac 20200311 : 1x2Site使用1x4Site Kit
    cbUse1x3siteSLK     ->Checked   = TestIF_File.bUse1x3SiteKit;               //KevinC 20260109 : 1x2Site and 2x2 NN mode 使用1x3Site Kit
    chk12SiteUse2x8SLK  ->Checked   = TestIF_File.b2x6Use2x8SitSLK;             //Steven 20240807 : 12Site使用16Site Kit
    cb16DirectHeater    ->Checked   = TestIF_File.bUse32Heater;                 //Steven 20140923 : Index使用EJ1N版32組加熱器
    cb12Site10DirectHeater->Checked = TestIF_File.b12SiteUse10Heater;           //Steven 20140923 : Index使用EJ1N版32組加熱器
    Arm1PickArm2Test    ->Checked   = TestIF_File.bArm1PickPlaceArm2Test;       //kevin 20150127 Arm1 下壓 arm2 測試
    cbCheckArm2Vacuum   ->Checked   = TestIF_File.bCheckArm2Vacuum;             //Steven 20150129 : 需要確認Arm2有沒有粘料
    cbUseSLKClamp       ->Checked   = TestIF_File.bUseSLKClamp;                 //JerryYang 20160523
    rgseparabilityTest  ->ItemIndex = TestIF_File.iSeparabilityTest;            //JerryYang 20160602
    if(TestIF_File.bUseSLKClamp)                                                //JerryYang 20160714 bUseSLKClamp
        rgseparabilityTest->Enabled=true;
    else
        rgseparabilityTest->Enabled=false;
    cbSingleSiteSingleHeater->Checked=TestIF_File.bSingleHeater;                //JerryYang 20161013
    cbOctal12Site->Checked= TestIF_File.bOctal_12Kit;                           //ChungHung 20140508 add for SCK

    cbHotechLayoutKit2x2->Checked=TestIF_File.bHontechLayoutKit2x2;             //jou 2015-12-10 SCS 要求 Hontech Layout kit要選擇Hontech.

    cbEnablePreciser            ->Checked   = TestIF_File.bEnableUsePreciser;   //Frank 20180410 (Steven) : InArm Preciser Station
    cbEnabledPreciserRT         ->Checked   = TestIF_File.bEnableRTPreciser;    //Frank 20180410 (Steven) : InArm Preciser Station

    if(IniConfig.bVTESTFunction==true)                                          //jou 20200701 : VTEST for auto site mapping cable mount
    {
        chkSiteMapOpenSite->Checked = TestIF_File.bAutoSiteMappingOpenSite;
        chkSiteMapOneCycle->Checked = TestIF_File.bAutoSiteMappingOneCycle;
    }

    if(USE_IN_OUT_ARM_Y_PITCH==iXPitchManual635 ||                              //jou 2012-05-15 : 選擇 Y Pitch 機構模式
       USE_IN_OUT_ARM_Y_PITCH==iXPitchManual360)                                //Steven 20140819 : Y-Pitch 36mm
    {
        if(TestIF_File.iARM_Y_PITCH==6000)
            rgInOutArmYPitch->ItemIndex=0;
        else
            rgInOutArmYPitch->ItemIndex=iXPitchManual635;
    }

    cbF18->Checked=TestIF_File.bF18InshuttleDetect;                             //jou 20170418 (Steven) : 矽品-世明要求修改config F18至Recipe設定
    cbRTC20CheckFunction->Checked=TestIF_File.bRTC20CheckFunction;              //Ifor 20190712 : add RTC2.0 Check
    cbRTC20GiveWayCheck->Checked=TestIF_File.bRTC20GiveWayCheck;                //Ifor 20190712 : add RTC2.0 Check

    cbRTCICResidueCheck->Checked=TestIF_File.bRTCICResidueCheck;                //Ifor 20190712 : add RTC2.0 Check
    cbRTC20OverFlowCheck->Checked=TestIF_File.bRTC20OverFlowCheck;              //Ifor 20190712 : add RTC2.0 Check

    cbArm1UseHeat->Checked=TestIF_File.bArm1UseHeat;                            //Ifor 20190815 : add Arm1是否加熱開關
    cbAutoCleanAlso->Checked=TestIF_File.bArm1PickPlaceArm2Test_RunAutoClean;   //Jimmychiu 20230710 : Auto Clean 跑 Arm1 下壓 arm2 測試
    cbArm1OnlyPlaceArm2TestAndSuck->Checked=TestIF_File.bArm1OnlyPlaceArm2TestAndSuck;//Jimmychiu 20250722 : Arm1 Only Place Arm2 Test And Suck

    if(CosFunction.b2x4SupportCenterPitch==false ||                             //Steven 20170706 (wei) : 2x4中間的Pitch不同 for SCC
       TestIF_File.iTestMode!=_8Site2X4)
    {
        chkUseXCenterPitch->Checked=false;
    }
    else
    {
        chkUseXCenterPitch->Checked=TestIF_File.bEnableUseXCenterPitch;
        edtXCenterPitch->Text      =TestIF_File.dSiteXCenterPitch;
    }

    cbInUseBackRow->Checked =TestIF_File.bInArmUseBackRowSuck;                  //jou 20161122 (Steven) In & Out arm use back row suck
    cbOutUseBackRow->Checked=TestIF_File.bOutArmUseBackRowSuck;                 //jou 20161122 (Steven) In & Out arm use back row suck
    edtRTCFileName->Text    =TestIF_File.sRtcFileName;                          //kevin 20200731 RTC FILENAME

    chkReadTorque->Checked  =TestIF_File.bEnableReadAndCheckTorque;             //kevin 20210804
    edOverRange->Text       =TestIF_File.dReadTorque;                           //kevin 20210804 扭力誤差值 change by setup
    edDelayTime->Text       =TestIF_File.dReadTorqueDelayTime;                  //kevin 20210804 等待時間去讀取扭力 change by setup

    cbUseRotateForHT7000HPKit->Checked=TestIF_File.UseRotateForHT7000HPKit;     //Sam 20210416 : 新增特殊模式 For Rotate Function HT7000 HP Kit

    bDoubleUnloadTray->Checked  = TestIF_File.bDoubleUnloadTray;                //kevin 20220506 add Unload Tray 2  倍的設定

    edAuto1Count->Text       =TestIF_File.iUnloaderTrayCount[0];                //kevin 20220506 add Unload Tray Total
    edAuto2Count->Text       =TestIF_File.iUnloaderTrayCount[1];                //kevin 20220506 add Unload Tray Total
    edAuto3Count->Text       =TestIF_File.iUnloaderTrayCount[2];                //kevin 20220506 add Unload Tray Total
    cbSingleUseOtherSuck ->Checked =TestIF_File.bSingleUseOtherSuck;            //wei 20220905 Single Use Other Suck(IN C OUT E)
    edtGetValueDelayTime->Text=TestIF_File.dSendGetValueDelayTime;              //KenHsieh 20220317 : 延遲取值時間
    cbSingleInArmUseOtherSuck ->Checked =TestIF_File.bSingleInArmUseOtherSuck;  //Ifor 20240430 add: Single Site InArm Use Other Suck(IN C)
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::FormClose(TObject *Sender, TCloseAction &Action)
{
    AnsiString str;
    if(CUSTOMER_CODE==CC_ASE_KaohSiung ||
       CUSTOMER_CODE==CC_ASE_KaohSiung_K12)                                     //Steven 20131101 : Add ASE-K12
    {
        if(CHSetError())
        {
            Action=caNone;
            return;
        }
    }

    if(AUTO_SENSOR_INSTALL && bSaveNeedHome)
    {
        ShowMyMessage("Auto Shuttle Sensor Need Reset!!");
        return;
    }
    ReadFile();
    fShow=false;                                                                //Steven 20110907 : 一定要在ReadFile下面

    if(CosFunction.bI21EnableASMByRecipe)                                       //Steven 20210518 : 吳如春希望Auto site map從工作檔開關
    {
        if(CosFunction.bAutoSiteMappingUseHotPlate==true    &&                  //Ifor 20210913 add:關閉Site Mapping
           IniConfig.bI21AutoSiteMappingUseHotplate==true   &&
           LastSet.iRunStartMode==rsmAutoSiteMap            )
        {
            if(InArmSuck.HasIC()==false && OutArmSuck.HasIC()==false &&
               ShuttleHasIC()==false    && IndexHasIC()==false        )
            {
                if(IniConfig.bI21EnableASM==false)
                {
                    for(int i=0; i<2; i++)
                    {
                        for(int j=0; j<HotPlateForm.XDivision; j++)
                        {
                            for(int k=0; k<HotPlateForm.YDivision; k++)
                            {
                                MOT[MMPlate1+i].Tray.SiteMapData[j][k]=0;       //Ifor 20210913 add: 清除Auto Site Mapping 補回Has Null IC資料
                            }
                        }
                    }
                    SetRunStartMode(rsmContinuStart);
                    bAutoSiteMapHotplateSave=false;
                    bSiteMappingCHKOK=true;
                }
            }
            else
            {
                IniConfig.bI21EnableASM=true;
            }
        }
    }

    bNeedPassword=false;
    bOCRNeedPassword=false;                                                     //ChungHung 20130118 add OCR - 關閉要密碼
    lbShowMessage->Visible=false;

    rbTemp->SetFocus();                                                         //ChungHung 20130718 add 解決修改shuttle mode後按下離開  無法更新問題

    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);    //Steven 20100629
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::XPitchKeyPress(TObject *Sender, char &Key)
{
    if(OnlyNumberAndDotInPut(Key)==false)
        Key=NULL;
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::XPitchMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Barcode_Reader(bcSetup)==0)                                              // 20140103 wei KYEC Barcode Reader
    {
        return;
    }

    if(IniConfig.bSPILFunction==true)                                           //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 0.0, 1000.0);
    else
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 3, true, 0.0, 1000.0);
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::rgUseSuckModeClick(TObject *Sender)
{
    int iPos=ScrollBar1->Position;

    switch(iPos)
    {
        case DualSite:
        case QualSite1X4:
        case _8Site1X4:                                                         //ChungHung 20150528 add for 海思 _8Site1x4
            if(rgUseSuckMode->ItemIndex==1)
            {
                if(IniConfig.bDisableSelectSearchLast==true)                    //jou 2012-01-10 取消Setup，Search Last Mode功能。
                {
                    rgSelectSearchLast->Visible=false;
                    rgSelectSearchLast->ItemIndex=0;
                }
                else
                {
                    rgSelectSearchLast->Visible=true;
                }
            }
            else
            {
                rgSelectSearchLast->Visible=false;
            }
            break;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::sbtExitClick(TObject *Sender)
{
    if(AUTO_SENSOR_INSTALL && bSaveNeedHome)
    {
        ShowMyMessage("Auto Shuttle Sensor Need Reset!!");
        return;
    }

    if(bGetLotIDFormTester==false)
        fOCR->sTesterLotId=edOcrText->Text;
    sbtExit->Down=false;
    bNeedEnterPassword=true;
    Close();
    fMain->GetCZSiteMap(false);                                                 //Ifor 20201030 add:送Site Mapping 資料給GPIB
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::sbUpdateClick(TObject *Sender)
{
    if(IniConfig.bA02DisableSaveParsWhenSwitchToOp==true &&                     //RogerYang 20260305 張寧說時間到都要登出，且不可被更改任何設定(A01_2)
        AccessLevel==0)
    {
        ShowMyMessage("[A01_2]目前已切換到Operator權限，\r\n請重新登入再做設定!");
        Close();
    }

    if(IniConfig.bF33_Check2DHardware &&                                        //JerryYang 20250220 : 2DID硬體順序檢查功能
        (BAR_CODE_INSTALL==ebctUseCCDMode ||
         BAR_CODE_INSTALL==ebctInShtIntel ||
         BAR_CODE_INSTALL==ebctEtherNetCCD) &&
        TestIF_File.bEnableBarCode)// &&
//        TestIF_File.bEnableBottom2D==false)                                   //JerryYang 20250220 : 2DID硬體順序檢查功能
    {
        if(HasICUnderMachine() || HasAnyICInMachine())
        {
            if(TestIF_File.iShuttleMode==1 &&
               rgShtModeNormal->Checked==true &&
               rgShtModeOneSide->Checked==false)
            {
                rgShtModeNormal->Checked=false;
                rgShtModeOneSide->Checked=true;
                ShowMyMessage("因啟用2D CHECK功能,請先Clean out後再開ARM!");
                return;
            }
            else if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==0 &&  //Arm 1
                rgShtModeNormal->Checked==false && rgShtModeOneSide->Checked==true &&
                rgUseSht1->Checked==false && rgUseSht2->Checked==true)
            {
                ShowMyMessage("因啟用2D CHECK功能,請先Clean out後再開ARM!");
                return;
            }
            else if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1 &&  //Arm 2
                rgShtModeNormal->Checked==false && rgShtModeOneSide->Checked==true &&
                rgUseSht1->Checked==true && rgUseSht2->Checked==false)
            {
                ShowMyMessage("因啟用2D CHECK功能,請先Clean out後再開ARM!");
                return;
            }
        }
    }

    if(CHSetError())                                                            //jou 2010-09-02 start : Site map的問題,如出現相同或沒填的site,將所有的map全部清除,讓工程師重填.
    {
        for(int i=0; i<MAX_SOCKET_ROW; i++)
        {
            for(int j=0; j<MAX_SOCKET_COL; j++)
            {
                TestSiteCH[i][j]->ItemIndex=0;
            }
        }
        return;
    }

    if(CheckShuttlePitch()==false)                                              //wei 20160914 Auto Shuttle Sensor
    {
        for(int i=0; i<MAX_SOCKET_ROW; i++)                                     //jou 2010-09-02 start : Site map的問題,如出現相同或沒填的site,將所有的map全部清除,讓工程師重填.
        {
            for(int j=0; j<MAX_SOCKET_COL; j++)
            {
                TestSiteCH[i][j]->ItemIndex=0;
            }
        }
        return;
    }

    if(CosFunction.bLockRTCByFile &&                                            //ChungHung 20130118 add OCR - 關閉要密碼
       IniConfig.bRTC_Enable==false)                                            //Sam 20240311 : 新增 RTC Lock by file 功能
    {

    }
    else
    {
        if((bNeedPassword==true && gbRTC->Enabled==true && cbEnableRealTimeCCD->Checked==false) ||   //Steven 20110907 : 關RTC要密碼
           (bOCRNeedPassword==true && gbOcr->Enabled==true && cbOcrFunction->Checked==false))
        {
            if(DoPassword()==false)
            {
                if(bNeedPassword && cbEnableRealTimeCCD->Checked==false)
                    cbEnableRealTimeCCD->Checked=true;
                if(bOCRNeedPassword && cbOcrFunction->Checked==false)
                    cbOcrFunction->Checked=true;
            }
            else
            {
                if(bNeedPassword)
                {
                    bNeedPassword=false;
                    RecordProcess("RealTimeCCD Disabled");
                }
                if(bOCRNeedPassword)
                {
                    bOCRNeedPassword=false;
                    RecordProcess("OCR Disabled");
                }
            }
        }
        else if(bNeedPassword==true && gbRTC->Enabled==true && cbEnableRealTimeCCD->Checked==false)    //Steven 20110907 : 關RTC要密碼
        {
            if(DoPassword()==false)
            {
                cbEnableRealTimeCCD->Checked=true;
            }
            else
            {
                bNeedPassword=false;
                RecordProcess("RealTimeCCD Disabled");
            }
        }
        else if(bNeedPassword==false && gbRTC->Enabled==true && cbEnableRealTimeCCD->Checked==true)
        {
            ShowMyMessage("Program need to restart to active RTC fuinction!", "The program will automatically be closed after exit setup form.");
            fMain->bNeedRestartProgram=true;
        }
    }

    if(gbRTC->Enabled==true && cbEnableRealTimeCCD->Checked==true)              //Steven 20110907 : RTC開了又關,也要輸密碼
    {
        bNeedPassword=true;
    }

    if(gbOcr->Enabled==true && cbOcrFunction->Checked==true)                    //ChungHung 20130118 add OCR - 開了又關,也要輸密碼
    {
        bOCRNeedPassword=true;
    }

    SaveSetupFile(GetRecipePath());                                             //kevin 20150105
    #ifdef ASE_KaohSiung
        fBuilder->bSaveAsJobFile(GetLastOpenFN(), "JOBFILE");
    #endif

    fMain->bEnableAutoclean();                                                  //kevin 20120518 autoclean
    fContact->DutCount();                                                       //jou 2014-09-06 修正開啟程式的時候EP異常

    if(IniConfig.bEnable_SECS_GEM==true)                                        //Steven 20140528 : Secs Gem
    {
        bHasSaveSet=true;                                                       //Ifor 20151204 新增判斷機台有無修改設定檔
        EventReport(SECS_EVENT.ArmOnOff);                                       //45     開關Arm
        EventReport(SECS_EVENT.SaveRecipe);
    }
    fMain->BackupSetupFile();                                                   //Ifor 20170620 (wei) add Auto BackUp Setup File & Last Data
    fContact->ReadFile();                                                       //JerryYang 20180518 : add
    SetWorkParameter();
    if(AUTO_SENSOR_INSTALL)                                                     //wei 20170213
        bSaveNeedHome=true;

    if(CUSTOMER_CODE==CC_KYEC_LEE)                                              //Ifor 修改模式需切換是否使用NS KIT
    {
        ADAM_WriteVoltage(DeviceForm.dPress);
    }
}
//------------------------------------------------------------------------------
//kevin 20140105 儲存 檔案另存 jobfile
//------------------------------------------------------------------------------
void __fastcall TfSetup::SaveSetupFile(AnsiString sPath)
{
    AnsiString str;
    int iSite=0;                                                                //kevin 20160818
    MyForceDirectories(sPath);
    AnsiString sTemp    =IncludeTrailingPathDelimiter(sPath)+"Temperature.Data";
    AnsiString sContact =IncludeTrailingPathDelimiter(sPath)+"Contact.Data";
    AnsiString szDir    =IncludeTrailingPathDelimiter(sPath)+"HandlerCondition.Data";

    WriteIniData(szDir, "Configuration", "12_16SiteDirectHeaterLayout", cb16DirectHeater->Checked);                 //Steven 20140923 : Index使用EJ1N版32組加熱器
    WriteIniData(szDir, "Configuration", "12Site10DirectHeaterLayout",  cb12Site10DirectHeater->Checked);           //Steven 20140923 : Index使用EJ1N版32組加熱器

    WriteIniData(szDir, "Configuration", "Test Mode", TestSiteFileName[0][ScrollBar1->Position]);                   //Steven 20120814 : 改用文字存模式,方便新增新的模式

    TestIF_File.iTestMode=GetTestMode(TestSiteFileName[0][ScrollBar1->Position]);                   //Steven 20250321 : 整合TestMode轉換
    bSiteMapHasChange=false;
    if(iTestMode!=TestIF_File.iTestMode)                                        //Steven 20250102 : 紀錄test mode與site map是否有變更
    {
        bSiteMapHasChange=true;
        DoInArm_9045_Type();                                                    //Steven 20250321 : 要先更新一次
        iTestMode=TestIF_File.iTestMode;
    }

    if(CosFunction.bI21EnableASMByRecipe)                                       //Steven 20210518 : 吳如春希望Auto site map從工作檔開關
    {
        WriteIniData(szDir, "Auto Site Mapping", "Enable Auto Site Mapping",    cbI21->Checked?1:0);
    }

    if(CUSTOMER_CODE==CC_SCC)                                                   //Steven 20220624 : JSCC楊建軍要求跟著工作檔跑
    {
        WriteIniData(szDir, "O_Count", "bLifeTimeCount 1",          (cbO12_LifeTimeCount->Checked)?1:0);
        WriteIniData(szDir, "O_Count", "bLifeTimeCount 2",          (cbO13_LifeTimeCount->Checked)?1:0);
        WriteIniData(szDir, "O_Count", "bLifeTimeCount 3",          (cbO14_LifeTimeCount->Checked)?1:0);
    }

    WriteIniData(szDir, "Configuration", "Enable Single Use Other Suck",   (cbSingleUseOtherSuck->Checked)   ?1:0);         //wei 20220905 Single Use Other Suck(IN C OUT E)
    WriteIniData(szDir, "Configuration", "Enable Single In Use Other Suck",(cbSingleInArmUseOtherSuck->Checked)   ?1:0);    //Ifor 20240430 add: Single Site InArm Use Other Suck(IN C)
    WriteIniData(szDir, "Configuration", "Shuttle1 Cancel",         (rgUseSht1->Checked)            ?0:1);
    WriteIniData(szDir, "Configuration", "Shuttle2 Cancel",         (rgUseSht2->Checked)            ?0:1);
    WriteIniData(szDir, "Configuration", "Shuttle Mode",            (rgShtModeNormal->Checked)      ?0:1);
    WriteIniData(szDir, "Configuration", "QualSite2X2 Shift",       (cbQualSite2X2Shift->Checked)   ?1:0);          //wei 20160226 TSMC X Shift
    WriteIniData(szDir, "Configuration", "NS7000 bias kit",         (chkOffCenterkit->Checked)      ?1:0);          //jou 981208 : NS7000 bias kit
    if(IniConfig.bKoreaFunction==false)                                         //ChungHung 20121031 add
        WriteIniData(szDir, "Configuration", "NS8000 Change Socket", (cbNS8000H->Checked)        ?1:0);

    if(MachineTypeChoice==Type_HT1032)
        WriteIniData(szDir, "Configuration", "6 Cable Layout Kit",  (cb6CableLayoutKit->Checked)    ?1:0);          //Steven 20230613 : ATC6.1 16Site SLK
    WriteIniData(szDir, "Configuration", "2 Cable Layout Kit",      (cb2CableLayoutKit->Checked)    ?1:0);          //Steven 20150724 : 16溫控器 2條線版本
    WriteIniData(szDir, "Configuration", "1 Cable Layout Kit",      (cb1CableLayoutKit->Checked)    ?1:0);          //jou 2015-10-15  : 16溫控器 1條線版本
    WriteIniData(szDir, "Configuration", "Rotate Shuttle",          (chkRotateShuttle->Checked)     ?1:0);
    if(USE_16_HEATER==eht4Heater)
        WriteIniData(szDir, "Configuration", "Single site Use single heater", (cbSingleSiteSingleHeater->Checked)     ?1:0); //JerryYang 20161013 新增Single site一支加熱棒模式
    else
        WriteIniData(szDir, "Configuration", "Single site Use single heater", false); //JerryYang 20161013 新增Single site一支加熱棒模式
    if(INSTALL_OCR==eocrUninstal)                                               //Steven 20160122
        WriteIniData(szDir, "Configuration", "OCR Function",        false);     //Steven 20120716 : OCR
    else
        WriteIniData(szDir, "Configuration", "OCR Function",        (cbOcrFunction->Checked)        ?1:0);          //Steven 20120716 : OCR

    if(INSTALL_SOCKET_CLAMP==false)                                             //JerryYang 20160523
        WriteIniData(szDir, "Configuration", "Socket Clamp Function",        false);
    else
        WriteIniData(szDir, "Configuration", "Socket Clamp Function",        (cbUseSLKClamp->Checked)        ?1:0);

    if(cbUseSLKClamp->Checked==false)
        WriteIniData(szDir, "Configuration", "Enable Separability Test",            0);
    else
        WriteIniData(szDir, "Configuration", "Enable Separability Test",            rgseparabilityTest->ItemIndex);

    WriteIniData(szDir, "Configuration", "SocketSensor",            (cbSocketSensor->Checked)        ?1:0);         //kevin 20130504 socket sensor
    WriteIniData(szDir, "Configuration", "SocketCountt",            CoSocketCombo->Text);                           //kevin 20130504 socket sensor 多少個
    WriteIniData(szDir, "Configuration", "bArm1PickPlaceArm2Test",  (Arm1PickArm2Test->Checked)?1:0);               //kevin 20150127 Arm1 下壓 arm2 測試
    WriteIniData(szDir, "Configuration", "bCheckArm2Vacuum",        (cbCheckArm2Vacuum->Checked)?1:0);              //Steven 20150129 : 需要確認Arm2有沒有粘料

    for(int i=0; i<iSnSocketCnt; i++)                                           //Steven 20200420 : Socket Sensor功能可以選
    {
        str.sprintf("iSensorCheckType%d", i+1);
        WriteIniData(szDir, "Configuration", str, MyTempRGBox[i]->ItemIndex);
    }

    WriteIniData(szDir, "Configuration", "SocketDisibleinitialcheck", (cbdisibleinitialcheck->Checked)        ?1:0); //kevin 20170209 (wei) disible socket sensor index check
    WriteIniData(szDir, "Configuration", "bUseTwoArmwork", bUseTwoArm32Site);   //kevin 20190322 add use two arm

    WriteIniData(szDir, "Configuration", "Enable Preciser",             (cbEnablePreciser->Checked)           ?1:0);      //Frank 20180410 (Steven) : InArm Preciser Station
    WriteIniData(szDir, "Configuration", "Enable RT Preciser",          (cbEnabledPreciserRT->Checked)        ?1:0);      //Frank 20180410 (Steven) : InArm Preciser Station

    if(IniConfig.bVTESTFunction==true)                                          //jou 20200701 : VTEST for auto site mapping cable mount
    {
        WriteIniData(szDir, "Configuration", "Auto site mapping open site",  (chkSiteMapOpenSite->Checked)           ?1:0);
        WriteIniData(szDir, "Configuration", "Auto site mapping one cycle",  (chkSiteMapOneCycle->Checked)           ?1:0);
    }

    if(IniConfig.bRTCbySystem)                                                  //ChungHung 20120716 RTC by System
        WriteIniData("D:\\HT9045\\config\\config.ini", "RTC", "Enable",         (cbEnableRealTimeCCD->Checked)  ?0:1);
    else
        WriteIniData(szDir, "Configuration", "Real Time CCD",       (cbEnableRealTimeCCD->Checked)  ?0:1);      //Steven 20110824 : Real time CCD - Set Up可以開關

    WriteIniData(szDir, "Configuration", "bUseSocketFloat",         (cbEnableSocketFloat->Checked)  ?1:0);      //Frank QQ
    WriteIniData(szDir, "Configuration", "bUseRTCStepAsideMode",    (cbEnableStepAsideMode->Checked)  ?1:0);    //Ifor 20230908 add:RTC 讓位功能 By Setup File

    if(CUSTOMER_CODE==CC_GIGAS)                                                 //Isaac 20210422 : 1x4,xpitch>40,not support inarm 8 picker
    {
        if(TestIF_File.iTestMode==QualSite1X4 &&
           atoi(XPitch->Text.c_str())>40 &&
           rgUseSuckMode->ItemIndex==1)
        {
            ShowMyMessage("1x4, X pitch>40mm, not support in arm use 8 pick unit!!");
            rgUseSuckMode->ItemIndex=0;
            rgUseSuckMode->Visible=false;
            XPitch->Text="";
            return;
        }
    }

    WriteIniData(szDir, "Configuration", "X Pitch", FormatFloat("0.00", XPitch->Text.ToDouble()));  //JerryYang 20200214 統一格式小數點第二位
    WriteIniData(szDir, "Configuration", "Y Pitch", FormatFloat("0.00", YPitch->Text.ToDouble()));
    if(CosFunction.bNonCenterModeCanUseShtOffset)
    {
        if(IniConfig.bA50Enable1x4BiasYOffset &&                                //Steven 20200715 : for Tinton
           TestIF_File.iTestMode==QualSite1X4 &&
           chkOffCenterkit->Checked)
        {
            WriteIniData(szDir, "Configuration", "i1x4SiteYOffset", rgYOffset->ItemIndex);
        }
        else
        {
            WriteIniData(szDir, "Configuration", "i1x4SiteYOffset", 0);
        }
    }

    WriteIniData(szDir, "Configuration", "iYPitchOffsetMode",       rgYPitchOffsetMode->ItemIndex);
    if(rgYPitchOffsetMode->ItemIndex!=1)                                        //KenHsieh 20231227 : add Y Center Pitch mode
        WriteIniData(szDir, "Configuration", "Y Offset",            edYOffset->Text                    );               //Steven 20140512 : For HT-9047
    WriteIniData(szDir, "Configuration", "Search Last Mode",         rgSelectSearchLast->ItemIndex      );
    WriteIniData(szDir, "Configuration", "Octal Pitch 80",          (chkOctal80->Checked)           ?1:0);              //2013-03-05    Dell    for ATK pitch 80mm kit
    WriteIniData(szDir, "Configuration", "Octal 16 Kit",            (cbOctal16Site->Checked)        ?1:0);              //Steven 20140312 : 8Site使用16Site Kit
        WriteIniData(szDir, "Configuration", "Square Octal Kit",    (cbSquareOctalLayout->Checked)  ?1:0);              //Steven 20141224 : 2x2Site使用8Site Kit
    WriteIniData(szDir, "Configuration", "2x2 Use 16 Site Kit",     (chk2x2Use16siteSLK->Checked)   ?1:0);              //Steven 20191113 : 2x2Site使用16Site Kit
    WriteIniData(szDir, "Configuration", "Octal 12Kit",             (cbOctal12Site->Checked)        ?1:0);              //ChungHung 20140508 add for SCK
    WriteIniData(szDir, "Configuration", "1x2_Use_1x4_Kit",         (cb1x2Use1x4siteSLK->Checked)  ?1:0);               //Isaac 20200311 : 1x2Site使用1x4Site Kit
    WriteIniData(szDir, "Configuration", "Use_1x3_Kit",             (cbUse1x3siteSLK->Checked)  ?1:0);                  //KevinC 20260109 : 1x2Site and 2x2 NN mode 使用1x3Site Kit
    WriteIniData(szDir, "Configuration", "b2x6Use2x8SitSLK",        (chk12SiteUse2x8SLK->Checked)  ?1:0);               //Steven 20240807 : 12Site使用16Site Kit

    WriteIniData(szDir, "Configuration", "bHontechLayoutKit2x2",    (cbHotechLayoutKit2x2->Checked)        ?1:0);       //jou 2015-12-10 SCS 要求 Hontech Layout kit要選擇Hontech.
    WriteIniData(szDir, "Configuration", "X Shift Pitch",            XShiftPitch->Text                   );             //wei 20160226 TSMC X Shift
    WriteIniData(szDir, "Configuration", "bNSKitPress",             (cbNSKitPress->Checked)         ?1:0);              //kevin 20170804 (Steven) add 另一LOAD CELL 壓力值

    WriteIniData(szDir, "Configuration", "bIndEPSLK",               (cbIndSLK->Checked)             ?1:0);              //kevin 20170804 (Steven) add 另一LOAD CELL 壓力值

    WriteIniData(szDir, "Configuration", "b16Direct12Shuttle",      (cb16change12DirectHeater->Checked) ?1:0);          //kevin 20191104 add 16 Site Direct Heater use 12 Site shuttle
    WriteIniData(szDir, "Configuration", "b16Direct8Shuttle",       (cb16change8DirectHeater->Checked) ?1:0);           //Steven 20210322 : 16 Site Direct Heater use 8 Site shuttle
    WriteIniData(szDir, "Configuration", "bRTC20CheckFunction",     (cbRTC20CheckFunction->Checked)?1:0);               //Ifor 20190712 : add RTC2.0 Check
    WriteIniData(szDir, "Configuration", "bRTC20GiveWayCheck",      (cbRTC20GiveWayCheck->Checked)?1:0);                //Ifor 20190712 : add RTC2.0 Check

    WriteIniData(szDir, "Configuration", "bRTCICResidueCheck",      (cbRTCICResidueCheck->Checked)?1:0);                //Ifor 20190712 : add RTC2.0 Check
    WriteIniData(szDir, "Configuration", "bRTC20OverFlowCheck",     (cbRTC20OverFlowCheck->Checked)?1:0);               //Ifor 20190712 : add RTC2.0 Check

    WriteIniData(szDir, "Configuration", "bArm1UseHeat",            (cbArm1UseHeat->Checked)?1:0);                      //Ifor 20190815 : add Arm1是否加熱開關
    WriteIniData(szDir, "Configuration", "bArm1PickPlaceArm2Test_RunAutoClean",            (cbAutoCleanAlso->Checked)?1:0);//Jimmychiu 20230710 : Auto Clean 跑 Arm1 下壓 arm2 測試
    WriteIniData(szDir, "Configuration", "bArm1OnlyPlaceArm2TestAndSuck", (cbArm1OnlyPlaceArm2TestAndSuck->Checked)?1:0);//Jimmychiu 20250722 : Arm1 Only Place Arm2 Test And Suck

    WriteIniData(szDir, "Configuration", "Preciser X Pitch",        edPreciserXPitch->Text    );                        //Frank 20180410 (Steven) : InArm Preciser Station
    WriteIniData(szDir, "Configuration", "Preciser Y Pitch",        edPreciserYPitch->Text    );                        //Frank 20180410 (Steven) : InArm Preciser Station
    WriteIniData(szDir, "Configuration", "Use Tester Dry Air",      (cbUseTesterDry->Checked)           ?1:0);          //Ifor 20200227 : Fix Purge air無存檔問題

    if(CosFunction.bRotateUseHT7000HPKit && USE_ROTATE_KIT)                     //Sam 20210416 : 新增特殊模式 For Rotate Function HT7000 HP Kit
        WriteIniData(szDir, "Configuration", "UseRotateForHT7000HPKit",(cbUseRotateForHT7000HPKit->Checked)?1:0);
    else
        WriteIniData(szDir, "Configuration", "UseRotateForHT7000HPKit",0);

    WriteIniData(szDir, "Configuration", "UseAutoTrayMap",                bUseTrayMap->Checked);            //kevin 20220401 add Auto 123 Tray Map
    WriteIniData(szDir, "Configuration", "UseLoadCCDTrayMap",             bUseLoadCCD->Checked);            //kevin 20220330 add Load 載入CCD 判斷有無IC 存在
    WriteIniData(szDir, "Configuration", "DoubleUnloadTray",              bDoubleUnloadTray->Checked );     //kevin 20220506 add Unload Tray 2  倍的設定

    WriteIniData(szDir, "Configuration", "Auto1 Tray Count",              edAuto1Count->Text);              //kevin 20220401
    WriteIniData(szDir, "Configuration", "Auto2 Tray Count",              edAuto2Count->Text);              //kevin 20220401
    WriteIniData(szDir, "Configuration", "Auto3 Tray Count",              edAuto3Count->Text);              //kevin 20220401
    WriteIniData(szDir, "Configuration", "SiteMapDirection",              IniConfig.iSiteMapDirection);     //Jimmychiu 20230807 : #R230804-ATK-H9-01 , V3.21.792.1 ,Add the Sitemap items in information.txt
    //kevin 20211130
    int K=1;
    if(CUSTOMER_CODE==CC_ASE_KaohSiung_K3)                                      //kevin 20211130 K5 強制 SITE 順序  林建續
    {
        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                str.sprintf("Site %c%c", i+'A', j+'a');                         //kevin 20180312 j+'A' ==> j+'a'
                if(TestSiteCH[i][j]->ItemIndex==0)
                {
                    iSite=0;
                }
                else
                {
                    iSite=K;
                    TestSiteCH[i][j]->ItemIndex=K;
                }

                if(iSite<0)                                                     //kevin 20160818 沒有使用 Site 0
                    iSite=0;
                WriteIniData(szDir, "Configuration", str, iSite);               //kevin 20160818 沒有使用 Site 0
                K++;
            }
        }
    }
    else
    {
        for(int i=0; i<MAX_SOCKET_ROW; i++)
        {
            for(int j=0; j<MAX_SOCKET_COL; j++)
            {
                str.sprintf("Site %c%c", i+'A', j+'a');                         //kevin 20180312 j+'A' ==> j+'a'
                if(IniConfig.bSPILFunction==true)                               //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction  //JerryYang 20161019 影響到run check比對,矽品要求恢復成原本的存檔內容
                {
                    WriteIniData(szDir, "Configuration", str, TestSiteCH[i][j]->ItemIndex);
                }
                else
                {
                    iSite = TestSiteCH[i][j]->ItemIndex;
                    if(iSite<0)                                                 //kevin 20160818 沒有使用 Site 0
                        iSite=0;
                    WriteIniData(szDir, "Configuration", str, iSite);           //kevin 20160818 沒有使用 Site 0
                }
            }
        }
    }

    if(rgShtModeNormal->Checked)                                                //Steven 20101013 : 關單Arm不可使用多吸嘴
    {
        switch(TestIF_File.iTestMode)
        {
            case DualSite:                                                      //1x2
                WriteIniData(szDir, "Configuration", "Use Suck Mode", (rgUseSuckMode->ItemIndex==0)?2:4);
                break;
            case QualSite1X4:                                                   //1x4
            case QualSite2X2:                                                   //2x2
            case _8Site1X4:                                                     //ChungHung 20150528 add for 海思 _8Site1x4
            case _8Site2X4N:                                                    //Jimmychiu 20240219 : add for 8site2x4N
                WriteIniData(szDir, "Configuration", "Use Suck Mode", (rgUseSuckMode->ItemIndex==0)?4:8);
                break;
            case _8Site2X4:
            case _16Site4X4:
                if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                  //Steven 20231207 : 2x4 16 picker
                   USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)
                {
                    WriteIniData(szDir, "Configuration", "Use Suck Mode", (rgUseSuckMode->ItemIndex==0)?8:16);
                }
                break;
        }
    }
    else
    {
        switch(TestIF_File.iTestMode)
        {
            case DualSite:                                                      //1x2
                WriteIniData(szDir, "Configuration", "Use Suck Mode", 2);
                break;
            case TriSite1X3:                                                    //1x3  //Frank 20160329 add for 1x3_4
            case QualSite1X4:                                                   //1x4
            case QualSite2X2:                                                   //2x2
            case _8Site1X4:                                                     //ChungHung 20150528 add for 海思 _8Site1x4
                WriteIniData(szDir, "Configuration", "Use Suck Mode", 4);
                break;
            case _8Site2X4:
            case _16Site4X4:
                if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                  //Steven 20231207 : 2x4 16 picker
                   USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)
                {
                    WriteIniData(szDir, "Configuration", "Use Suck Mode", 8);
                }
                break;
        }
    }

    if(ATC_SYSTEM>eATC30 &&                                                     //20141204 ChungHung add for ATC3.0
       ATC_SYSTEM!=eNonChamber)                                                 //2014-05-30    Dell    for ATC6.0
    {                                                                           //Steven 20100119 Start: Use NS 7000 Change Socket
        if((TestIF_File.iTestMode==QualSite2X2  ||
            TestIF_File.iTestMode==_8Site2X4    ||
            TestIF_File.iTestMode==QualSite1X4  ||                              //kevin 20150121
            TestIF_File.iTestMode==_8Site1X4    ||                              //ChungHung 20150528 add for 海思 _8Site1x4
            TestIF_File.iTestMode==DualSite2x1  ||
            TestIF_File.iTestMode==QualSite2X2N ||                              //Frank 20200520 2X2NN Mode
            TestIF_File.iTestMode==_16Site4X4) &&                               //Sam 20190226 : 16Site4X4 //Steven 20140312 : For HT9045WA
            chkNS7000CS->Checked)
        {
            WriteIniData(szDir, "Configuration", "NS7000 Change Socket", 1);
        }
        else if(CUSTOMER_CODE==CC_SIGURD_PeiXing &&
                TestIF_File.iTestMode==_12Site2X6 &&
                chkNS7000CS->Checked)                                           //Ifor 20170420 (wei) add for 海思 _12Site2X6
        {
            WriteIniData(szDir, "Configuration", "NS7000 Change Socket", 1);
        }
        else
        {
            WriteIniData(szDir, "Configuration", "NS7000 Change Socket", 0);
        }
    }
    else
    {
        if((TestIF_File.iTestMode==QualSite2X2  ||
            TestIF_File.iTestMode==_8Site2X4    ||
            TestIF_File.iTestMode==QualSite1X4  ||                              //kevin 20150121
            TestIF_File.iTestMode==_8Site1X4    ||                              //ChungHung 20150528 add for 海思 _8Site1x4
            TestIF_File.iTestMode==DualSite2x1  ||
            TestIF_File.iTestMode==_16Site4X4) &&                               //Sam 20190226 : 16Site4X4 //Steven 20140312 : For HT9045WA
            chkNS7000CS->Checked)
        {
            WriteIniData(szDir, "Configuration", "NS7000 Change Socket", 1);
        }
        else if(CUSTOMER_CODE==CC_SIGURD_PeiXing &&
                TestIF_File.iTestMode==_12Site2X6 &&
                chkNS7000CS->Checked)                                           //Ifor 20170420 (wei) add for 海思 _12Site2X6
        {
            WriteIniData(szDir, "Configuration", "NS7000 Change Socket", 1);
        }
        else
        {
            WriteIniData(szDir, "Configuration", "NS7000 Change Socket", 0);
        }
    }

    if(USE_IN_OUT_ARM_Y_PITCH==iXPitch60)                                       //jou 2012-05-15 : 選擇 Y Pitch 機構模式
    {
        WriteIniData(szDir, "Configuration", "In Out Arm Y Pitch", 6000);
    }
    else if(USE_IN_Y_IS_AUTO_PITCH==true || USE_OUT_Y_IS_AUTO_PITCH==true)                                             //Steven 20131002 : XY變距  //JerryYang 20251218 : IN/OUT ARM支援不同模組
    {
        WriteIniData(szDir, "Configuration", "In Out Arm Y Pitch", 6000);
    }
    else
    {
        if(rgInOutArmYPitch->ItemIndex==iXPitch60)
        {
            WriteIniData(szDir, "Configuration", "In Out Arm Y Pitch", 6000);
        }
        else
        {
            if(USE_IN_OUT_ARM_Y_PITCH==iXPitchManual635)
                WriteIniData(szDir, "Configuration", "In Out Arm Y Pitch", 6350);
            else if(USE_IN_OUT_ARM_Y_PITCH==iXPitchManual360)                   //Steven 20140819 : Y-Pitch 36mm
                WriteIniData(szDir, "Configuration", "In Out Arm Y Pitch", 3600);
        }
    }
    WriteIniData(szDir, "Configuration", "bF18InshuttleDetect",        cbF18->Checked);         //jou 20170418 (Steven) : 矽品-世明要求修改config F18至Recipe設定
    WriteIniData(szDir, "Configuration", "In Arm Use Back Row", cbInUseBackRow->Checked);       //jou 20161122 (Steven) In & Out arm use back row suck
    WriteIniData(szDir, "Configuration", "Out Arm Use Back Row", cbOutUseBackRow->Checked);     //jou 20161122 (Steven) In & Out arm use back row suck
    WriteIniData(szDir, "Configuration", "RTCFileName", edtRTCFileName->Text);                  //kevin 20200731 RTC FILENAME

    if(CosFunction.b2x4SupportCenterPitch==true &&                              //Steven 20170706 (wei) : 2x4中間的Pitch不同 for SCC
       TestIF_File.iTestMode==_8Site2X4)
    {
        WriteIniData(szDir, "Configuration", "Enable X Center Pitch", chkUseXCenterPitch->Checked);
        WriteIniData(szDir, "Configuration", "X Center Pitch", edtXCenterPitch->Text);
    }
    else
    {
        WriteIniData(szDir, "Configuration", "Enable X Center Pitch", false);
    }

    WriteIniData(szDir, "Configuration", "EnableReadAndCheckTorque",    chkReadTorque->Checked);    //kevin 20210804 扭力比對by set up file
    WriteIniData(szDir, "Configuration", "dReadTorque",                 edOverRange->Text);
    WriteIniData(szDir, "Configuration", "dReadTorqueDelayTime",        edDelayTime->Text);
    WriteIniData(szDir, "Configuration", "SendGetValueDelayTime",       edtGetValueDelayTime->Text);//KenHsieh 20220317 : 延遲取值時間

//    AddSpace(szDir);
    bSavePressed=true;                                                          //Steven 20110907 : 一定要在ReadFile();之前

    ReadFile();
    fCleaning->LoadAutoCleanData();
    ChangeSite();
    sbUpdate->Down=false;

    bool bHasChange=false;
    if(IniConfig.bUseAutoSiteMapping)                                           //jou 2011-03-24 start : Auto Site Mapping
    {
        if(IniConfig.bI21EnableASM)                                             //Steven 20110502
        {
            if(iASMTestMode!=TestIF_File.iTestMode)                             //Steven 20120927 : for auto site mapping
                bHasChange=true;

            for(int i=0; i<MAX_SOCKET_ROW; i++)                                 //jou 2014-04-18 Auto site mapping 修正記憶體破壞 MAX_SOCKET_ROW <-> MAX_SOCKET_COL
            {
                for(int j=0; j<MAX_SOCKET_COL; j++)
                {
                    #ifndef ASE_KaohSiung                                       //kevin 20150114 高雄不使用
                        if(iASMSiteMap[i][j]!=TestSiteCH[i][j]->ItemIndex)
                            bHasChange=true;
                    #else
                        if(iASMSiteMap[i][j]==0)                                //kevin 20150114原本是 關site
                            if(TestSiteCH[i][j]->ItemIndex!=0)                  //現在開site
                                bHasChange=true;                                //要做autosite map
                    #endif
                }
            }

            if(CosFunction.bI21EnableASMByRecipe && LastSet.iRunStartMode!=rsmAutoSiteMap)
                bHasChange=true;

            if(bHasChange && HasICUnderMachine()==false)                        //Steven 20210518 : 有IC的狀態下, 不可以開關
            {
                if(TestIF_File.iTestMode==SingleSite)                           //Steven 20130610 : Single Site不做Auto Site Mapping
                    fMain->SetMainRunStartMode(rsmContinuStart);
                else
                    fMain->SetMainRunStartMode(rsmAutoSiteMap);
            }
        }
        else
        {
            if(CosFunction.bI21EnableASMByRecipe && HasICUnderMachine()==false) //Steven 20210518 : 有IC的狀態下, 不可以開關
            {
                fMain->cbRunStartMode->Enabled=true;
            }
        }
    }

    if(HasICUnderMachine()==false)                                              //有開SITE 才恢復畫面關SITE -> 開SITE
    {
        if(CosFunction.bDisableOpenAllSiteWhenChangeShtMod)                     //Jimmychiu 20241203 : Disable opening all sites when changing shuttle mode.
        {
            bool bdiff=VertifyShtModeisDiff(rgShtModeNormal->Checked,
                                            rgShtModeOneSide->Checked,
                                            rgUseSht1->Checked,
                                            rgUseSht2->Checked
                                            );
            fMain->ShowTestHeadComp(bdiff);
        }
        else
        {
//            bSiteMapHasChange=false;                                            //Steven 20250102 : 紀錄test mode與site map是否有變更
//            if(iTestMode!=TestIF_File.iTestMode)
//            {
//                bSiteMapHasChange=true;
//                iTestMode=TestIF_File.iTestMode;
//            }

            for(int i=0; i<MAX_SOCKET_ROW; i++)
            {
                for(int j=0; j<MAX_SOCKET_COL; j++)
                {
                    if(iTestSiteCh[i][j]!=TestIF_File.iSiteMap[i][j])
                    {
                        bSiteMapHasChange=true;
                        iTestSiteCh[i][j]=TestIF_File.iSiteMap[i][j];
                    }
                }
            }

            fMain->ShowTestHeadComp(bSiteMapHasChange);                         //kevin 20150401 判斷開關SITE
        }
    }
    else
    {
        fMain->ShowTestHeadComp(false);
    }
    myLog.Save_SiteStatusLog();                                                 //JerryYang 20160622 開關site記錄log

    if(IniConfig.bShuttleMode50)
    {
        if(rgShtModeNormal->Checked==false)                                     //Use Signal shuttle
        {
            if(rgUseSht1->Checked)                                              //shuttle 1
            {
                if(IniConfig.bRemeberAutoHeight==false)                         //ChungHung 20120725 Amkor_K 要可以記住AutoHeight的值，除非重新K高度 但選單Arm時只移動-50
                {
                    if(IniConfig.bD58UseArm1PickPlaceArm2Test==true &&          //Ifor 20190718 Fix Arm1PickPlace Arm2Test Contact Error
                       TestIF_File.bArm1PickPlaceArm2Test==true     )
                    {

                    }
                    else
                    {
                        WriteIniData(sContact, "Test Arm2", "Contact", AnsiString("-50"));  //強制設成-50mm
                    }
                }
                fContact->ReadFile();
            }
            else                                                                //shuttle 2
            {

                if(IniConfig.bRemeberAutoHeight==false)                         //ChungHung 20120725 Amkor_K 要可以記住AutoHeight的值，除非重新K高度 但選單Arm時只移動-50
                    WriteIniData(sContact, "Test Arm1", "Contact", AnsiString("-50")); //JerryYang 20190429 fix contact height被存成"1"的問題
                fContact->ReadFile();
            }
        }
    }

    if(rgShtModeNormal->Checked==true)
    {
        WriteIniData(sTemp, "ATC", "ATC7CH1Enabled", true);
        WriteIniData(sTemp, "ATC", "ATC7CH2Enabled", true);
        WriteIniData(sTemp, "ATC", "ATC7CH3Enabled", true);
        WriteIniData(sTemp, "ATC", "ATC7CH4Enabled", true);
    }
    else
    {
        if(rgUseSht1->Checked==true)
        {
            WriteIniData(sTemp, "ATC", "ATC7CH1Enabled", bTestSiteUse[0][0][0]);
            WriteIniData(sTemp, "ATC", "ATC7CH2Enabled", bTestSiteUse[0][0][1]);
        }
        else
        {
            WriteIniData(sTemp, "ATC", "ATC7CH1Enabled", rgUseSht1->Checked);
            WriteIniData(sTemp, "ATC", "ATC7CH2Enabled", rgUseSht1->Checked);
        }

        if(rgUseSht2->Checked==true)
        {
            WriteIniData(sTemp, "ATC", "ATC7CH3Enabled", bTestSiteUse[1][0][0]);
            WriteIniData(sTemp, "ATC", "ATC7CH4Enabled", bTestSiteUse[1][0][1]);
        }
        else
        {
            WriteIniData(sTemp, "ATC", "ATC7CH3Enabled", rgUseSht2->Checked);
            WriteIniData(sTemp, "ATC", "ATC7CH4Enabled", rgUseSht2->Checked);
        }
    }
    Temperature.bATC7ChannelEnabled[0] = ReadIniData(sTemp, "ATC", "ATC7CH1Enabled", false);
    Temperature.bATC7ChannelEnabled[1] = ReadIniData(sTemp, "ATC", "ATC7CH2Enabled", false);
    Temperature.bATC7ChannelEnabled[2] = ReadIniData(sTemp, "ATC", "ATC7CH3Enabled", false);
    Temperature.bATC7ChannelEnabled[3] = ReadIniData(sTemp, "ATC", "ATC7CH4Enabled", false);
    AnsiString asData1="", asData2="";
    asData1.sprintf("%d,%d", Temperature.bATC7ChannelEnabled[0], Temperature.bATC7ChannelEnabled[1]);
    asData2.sprintf("%d,%d", Temperature.bATC7ChannelEnabled[2], Temperature.bATC7ChannelEnabled[3]);
    ATCInterfaceForm->SendCommToATC7(ATC_CH_ENABLED, asData1, asData2);
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::btnLUpToRDownNClick(TObject *Sender)
{
    if(Barcode_Reader(bcSetup)==0)                                              // 20140103 wei KYEC Barcode Reader
    {
        return;
    }

    if(IniConfig.bG09NeedPasswordWhenEditSiteMap==true)                         //Alick 20160603 Site Map 需要密碼
    {
        if(bNeedEnterPassword==true)
        {
            fPassword->edPassword->Text="";
            fQwertyKey->ShowQwertyKey(fPassword->edPassword, N_NO_SYMBOL|N_NO_SPACE|N_PASSWORD);
            if(sSigPassword==fPassword->edPassword->Text)
            {
                bNeedEnterPassword=false;
            }
        }
    }
    else
        bNeedEnterPassword=false;

    if(bNeedEnterPassword==true)
        return;                                                                 //Alick 20161004 add for 矽格SiteMap密碼功能修正

    int iSX, iSY, iCT=1, iDirection;

    TSpeedButton *Ptr=(TSpeedButton *)Sender;
    iDirection=Ptr->Tag;
    if(iDirection>=0 && iDirection<=3)
    {
        iSX=MAX_SOCKET_ROW;
        iSY=MAX_SOCKET_COL;
    }
    else
    {
        iSX=MAX_SOCKET_COL;
        iSY=MAX_SOCKET_ROW;
    }
    for(int i=0; i<iSX; i++)
    {
        for(int j=0; j<iSY; j++)
        {
            if(iDirection==0)                                                   //Left_Top To Right
            {
                if(TestSiteCH[i][j]->Visible && TestSiteCH[i][j]->Enabled)      //Steven 20120813 : 加入'&& TestSiteCH[i][j]->Enabled' for 12Site
                {
                    TestSiteCH[i][j]->ItemIndex=iCT;
                    iCT++;
                }
            }
            else if(iDirection==1)                                              //Left_Bottom To Right
            {
                if(TestSiteCH[3-i][j]->Visible && TestSiteCH[i][j]->Enabled)
                {
                    TestSiteCH[3-i][j]->ItemIndex=iCT;
                    iCT++;
                }
            }
            else if(iDirection==2)                                              //Right_Top To Left
            {
                if(TestSiteCH[i][7-j]->Visible && TestSiteCH[i][j]->Enabled)
                {
                    TestSiteCH[i][7-j]->ItemIndex=iCT;
                    iCT++;
                }
            }
            else if(iDirection==3)                                              //Right_Bottom To Left
            {
                if(TestSiteCH[3-i][7-j]->Visible && TestSiteCH[i][j]->Enabled)
                {
                    TestSiteCH[3-i][7-j]->ItemIndex=iCT;
                    iCT++;
                }
            }
            else if(iDirection==4)                                              //Left_Top To Bottom
            {
                if(TestSiteCH[j][i]->Visible && TestSiteCH[j][i]->Enabled)
                {
                    TestSiteCH[j][i]->ItemIndex=iCT;
                    iCT++;
                }
            }
            else                                                                //Right_Top To Bottom
            {
                if(TestSiteCH[j][7-i]->Visible && TestSiteCH[j][i]->Enabled)
                {
                    TestSiteCH[j][7-i]->ItemIndex=iCT;
                    iCT++;
                }
            }
        }
    }

    if(iDirection==0)                                                           //Left_Top To Right
    {                                                                           //Jimmychiu 20230807 : #R230804-ATK-H9-01 , V3.21.792.1 ,Add the Sitemap items in information.txt
        IniConfig.iSiteMapDirection=5;
    }
    else if(iDirection==1)                                                      //Left_Bottom To Right
    {
        IniConfig.iSiteMapDirection=10;
    }
    else if(iDirection==2)                                                      //Right_Top To Left
    {
        IniConfig.iSiteMapDirection=9;
    }
    else if(iDirection==3)                                                      //Right_Bottom To Left
    {
        IniConfig.iSiteMapDirection=6;
    }
    else if(iDirection==4)                                                      //Left_Top To Bottom
    {
        IniConfig.iSiteMapDirection=7;
    }
    else if(iDirection==5)                                                      //Right_Top To Bottom
    {
        IniConfig.iSiteMapDirection=11;
    }
    else                                                                        //none
    {
        IniConfig.iSiteMapDirection=0;
    }
    Ptr->Down=false;
}
//------------------------------------------------------------------------------
bool __fastcall TfSetup::DoPassword()                                           //Steven 20101124
{
    bool bFlag=true;
    bool bTechComExist=FileExists(pwPath);                                      //2012-01-03    Dell modify
    int iLevel=0;

    iLevel=LevelSet.AccessLevel[37];

    if(REAL_TIME_CCD)
    {
        if(fQwertyKey->bShow==false)
        {
            if(bTechComExist)
                fMain->cbUserSelectChange(NULL);
            else
                fMain->stOperatorClick(fMain);

            if(AccessLevel<iLevel)
            {
                bFlag=false;
                if(bTechComExist)
                {
                    fPassword->Label3->Visible=true;
                    fPassword->Label4->Visible=true;
                }
            }
            else
            {
                if(bTechComExist)
                {
                    fPassword->Label3->Visible=false;
                    fPassword->Label4->Visible=false;
                }
            }
        }
    }
    return bFlag;
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::cbEnableRealTimeCCDClick(TObject *Sender)
{
    if(cbEnableRealTimeCCD->Checked==false)                                     //ChungHung 20120710 add SCK Disable IndexCheck
    {
        if(IniConfig.bD55DisableIndexCheck==true &&
           IniConfig.bM0109RTCOffCheckYieldPiggyBack==false)                    //Steven 20150723 : Fixed for SCK
        {
            cbEnableRealTimeCCD->Checked=true;
            ShowMyMessage("Must cancel \"[D55]Disable index check\" first "," ");
            return;
        }
    }

    if(COM2->bCCDDummyRum==false)                                               //jou 2011-11-09 需要可以再任何時間點都可以關閉Real time ccd,怕突然有東西壞掉之類的
    {
        COM2->SendCommToVision(COM2->rtInspEnd, false);                         //已經開啟real time
    }
    else
    {
        if(fMain->CheckCanChangeRealDummy()==false)                             //已經關閉real time
        {
            cbEnableRealTimeCCD->Checked=false;
            lbShowMessage->Visible=true;
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::cbAaChange(TObject *Sender)
{
    if(IniConfig.bG09NeedPasswordWhenEditSiteMap==true)                         //Alick 20160603 Site Map 需要密碼
    {
        if(bNeedEnterPassword==true)
        {
            fPassword->edPassword->Text="";
            fQwertyKey->ShowQwertyKey(fPassword->edPassword, N_NO_SYMBOL|N_NO_SPACE|N_PASSWORD);
            if(sSigPassword==fPassword->edPassword->Text)
            {
                bNeedEnterPassword=false;
            }
            else
            {
                if(TestIF_File.iTestMode==SingleSite)                           //Alick 20161216 add modify for PeiXing 用鍵盤選擇SITE別沒輸入密碼卻可以變更的Bug
                    ScrollBar1->SetFocus();
                else
                    XPitch->SetFocus();
                return;
            }
        }
    }
    else
    {
        bNeedEnterPassword=false;
    }

    if(bNeedEnterPassword==false)
    {
        if(Barcode_Reader(bcSetup)==0)                                          // 20140103 wei KYEC Barcode Reader
        {
            TComboBox *Ptr;
            Ptr=(TComboBox *)Sender;
            Ptr->ItemIndex=SitCH;
            Ptr->Repaint();
            return;
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::cbAaDropDown(TObject *Sender)
{
    if(IniConfig.bG09NeedPasswordWhenEditSiteMap==true)                         //Alick 20160603 Site Map 需要密碼
    {
        if(bNeedEnterPassword==true)
        {
            fPassword->edPassword->Text="";
            fQwertyKey->ShowQwertyKey(fPassword->edPassword, N_NO_SYMBOL|N_NO_SPACE|N_PASSWORD);
            if(sSigPassword==fPassword->edPassword->Text)
            {
                bNeedEnterPassword=false;
            }
            else
            {
                if(TestIF_File.iTestMode==SingleSite)                           //Alick 20161216 add modify for PeiXing 用鍵盤選擇SITE別沒輸入密碼卻可以變更的Bug
                    ScrollBar1->SetFocus();
                else
                    XPitch->SetFocus();
                return;
            }
        }
    }
    else
    {
        bNeedEnterPassword=false;
    }

    if(bNeedEnterPassword==false)
    {
        TComboBox *Ptr;
        Ptr=(TComboBox *)Sender;
        SitCH=Ptr->ItemIndex;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::RadioButton1KeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
        TRadioButton *Ptr;
        Ptr=(TRadioButton *)Sender;
        SitCH=Ptr->Checked;
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::rgShtModeNormalMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Barcode_Reader(bcSetup)==0)                                              // 20140103 wei KYEC Barcode Reader
    {
        if(TestIF_File.iShuttleMode==0)
            rgShtModeNormal->Checked=true;
        else
            rgShtModeOneSide->Checked=true;
        return;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::Arm1PickArm2TestClick(TObject *Sender)
{
    if(Arm1PickArm2Test->Checked)                                               //kevin 20150127 Arm1 下壓 arm2 測試
    {
        gbShuttleMode->Visible=false;                                           //only use shuttle 1
    }
    else
    {
        gbShuttleMode->Visible=IniConfig.bD30EnableSiteModeSelect;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::cbOctal12SiteClick(TObject *Sender)
{
    if(IniConfig.bSPILFunction==true ||                                         //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
       CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //jou 2015-05-14 Octal Site supply 12 sites channel
    {
        if((TestIF_File.iTestMode==_8Site2X4 ||
            TestIF_File.iTestMode==_16Site4X4) &&
           CosFunction.bEnableOctal_12Kit==true)
        {
            ScrollBar1Change(this);
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::chkOffCenterkitClick(TObject *Sender)
{
    int iCheckPos=ScrollBar1->Position;                                         //避免ScrollBar1->Position改變時，會有問題。

    if(CosFunction.b2x4SupportCenterPitch==true &&                              //Steven 20170706 (wei) : 2x4中間的Pitch不同 for SCC
       iCheckPos==_8Site2X4)
    {
        if(chkUseXCenterPitch->Checked==true &&
           FileExists(BmpPath+"8siteCenterX.bmp"))
        {
            Image1->Picture->LoadFromFile(BmpPath+"8siteCenterX.bmp");
        }
        else
        {
            Image1->Picture->LoadFromFile(BmpPath+TestSiteFileName[1][iCheckPos]);
        }
    }
    else if(chkOffCenterkit->Checked==false)
    {
        Image1->Picture->LoadFromFile(BmpPath+TestSiteFileName[1][iCheckPos]);
        rgYOffset->ItemIndex=0;
        rgYOffset->Visible=false;
    }
    else
    {
        rgYOffset->ItemIndex=0;
        rgYOffset->Visible=false;
        switch(iCheckPos)
        {
            case SingleSite:                                                    //1x1
                Image1->Picture->LoadFromFile(BmpPath+"1siteOffCentre.bmp");
                break;
            case DualSite:                                                      //1x2
                Image1->Picture->LoadFromFile(BmpPath+"2siteOffCentre.bmp");
                break;
            case QualSite1X4:                                                   //1x4
                if(CosFunction.bNonCenterModeCanUseShtOffset &&
                   IniConfig.bA50Enable1x4BiasYOffset &&
                   chkOffCenterkit->Checked)                                    //Steven 20200715 : for Tinton
                {
                    rgYOffset->Visible=true;
                }
                Image1->Picture->LoadFromFile(BmpPath+"4siteOffCentre.bmp");
                break;
            default:
                Image1->Picture->LoadFromFile(BmpPath+TestSiteFileName[1][iCheckPos]);
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::cbQualSite2X2ShiftClick(TObject *Sender)               //wei 20160226 TSMC X Shift
{
    int iCheckPos=ScrollBar1->Position;                                         //避免ScrollBar1->Position改變時，會有問題。

    if(cbQualSite2X2Shift->Checked==false)
    {
        if(chkOffCenterkit->Checked)                                            //kevin 20170415 (wei) KaohSiung add 2x2 Shift
            Image1->Picture->LoadFromFile(BmpPath+"2siteOffCentre.bmp");
        else
            Image1->Picture->LoadFromFile(BmpPath+TestSiteFileName[1][iCheckPos]);
        paQualSite2X2Shift->Visible=false;
    }
    else
    {
        if(iCheckPos==QualSite2X2)
        {
            Image1->Picture->LoadFromFile(BmpPath+"4siteRowOffCentre.bmp");
            paQualSite2X2Shift->Visible=true;
            XShiftPitch->Text=40.0;
        }
        else if(iCheckPos==DualSite)                                            //kevin 20170415 (wei) KaohSiung add 2x2 Shift
        {
            if(chkOffCenterkit->Checked)
                Image1->Picture->LoadFromFile(BmpPath+"2siteOffCentre.bmp");
            else
                Image1->Picture->LoadFromFile(BmpPath+TestSiteFileName[1][iCheckPos]);
            paQualSite2X2Shift->Visible=true;
            XShiftPitch->Text=-2000;
        }
        else
        {
            Image1->Picture->LoadFromFile(BmpPath+TestSiteFileName[1][iCheckPos]);
            paQualSite2X2Shift->Visible=false;
        }
    }
}
//------------------------------------------------------------------------------
bool __fastcall TfSetup::DoPasswordCheck()                                      //JerryYang 20160425 修改Site map需要密碼
{
    bool bFlag=true;
    bool bTechComExist=FileExists(pwPath);
    int iLevel=2;

    if(fQwertyKey->bShow==false)
    {
        if(bTechComExist)
            fMain->cbUserSelectChange(NULL);
        else
            fMain->stOperatorClick(fMain);

        if(AccessLevel<iLevel)
        {
            bFlag=false;
            if(bTechComExist)
            {
                fPassword->Label3->Visible=true;
                fPassword->Label4->Visible=true;
            }
        }
        else
        {
            if(bTechComExist)
            {
                fPassword->Label3->Visible=false;
                fPassword->Label4->Visible=false;
            }
        }

        if(bTechComExist)
        {
            fMain->btLogin->Caption="Login";

            if(CosFunction.bSecurityHave5Level==true)                           //jou 2014-06-19 Security Have 5 Level
            {
                if(CUSTOMER_CODE==CC_KYEC_LEE)                                  //wei 20160505 增加PE權限
                    fMain->spbUserName->Caption="Operator";
                else
                    fMain->spbUserName->Caption="Open";
            }
            else
            {
                fMain->spbUserName->Caption="Operator";
            }
            fMain->cbUserSelect->ItemIndex=0;
            AccessLevel=0;
            fMain->ChangeLevelAttr();
        }
    }
    return bFlag;
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::edOcrTextMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_NO_SYMBOL|N_NO_SPACE);
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::cbUseSLKClampClick(TObject *Sender)                    //JerryYang 20160714 沒裝分離式SLK就不能選分離測試模式
{
    if(cbUseSLKClamp->Checked==true)                                            //JerryYang 20160714 bUseSLKClamp
        rgseparabilityTest->Enabled=true;
    else
        rgseparabilityTest->Enabled=false;
}
//------------------------------------------------------------------------------
bool TfSetup::CheckShuttlePitch()                                               //wei 20160914 Auto Shuttle Sensor
{
    bool bShuttleSensorCanMove=false;
    double iShuttlePitch=0.0;
    double iXpitch=atof(XPitch->Text.c_str());

    if(AUTO_SENSOR_INSTALL)
    {
        if(ScrollBar1->Position==DualSite     ||                                //1x2 //sensor 2 4
           ScrollBar1->Position==QualSite2X2  ||
           ScrollBar1->Position==QualSite2X2N)                                  //2x2
        {
            iShuttlePitch=iXpitch*100;
            if(iShuttlePitch*3>=12000 && iShuttlePitch*3<=24000)
            {
                bShuttleSensorCanMove=true;
            }
            else if(iShuttlePitch>=9000 && iShuttlePitch<=18000)
            {
                bShuttleSensorCanMove=true;
            }
        }
        else if(ScrollBar1->Position==TriSite1X3 ||                             //1x3  //Frank 20160329 add for 1x3_4    //sensor 1 3 5
                ScrollBar1->Position==_6Site2X3  ||                             //ChungHung 20140115 add for 2x3_6
                ScrollBar1->Position==_6Site2X3N )                              //Steven 20220425 : 2x3 NN mode
        {
            iShuttlePitch=iXpitch*2*100;
            if(iShuttlePitch*3>=12000 && iShuttlePitch*3<=24000)
            {
                bShuttleSensorCanMove=true;
            }
            else if(iShuttlePitch>=8759 && iShuttlePitch<=24000)                //20170213
            {
                bShuttleSensorCanMove=true;
            }
        }
        else if(ScrollBar1->Position==QualSite1X4 ||                            //1x4                       //sensor 1 2 4 5
                ScrollBar1->Position==_8Site1X4   ||                            //ChungHung 20150528 add for 海思 _8Site1x4
                ScrollBar1->Position==_8Site2X4   ||
                ScrollBar1->Position==_16Site4X4  ||                            //Sam 20190226 : 16Site4X4 //2x4
                ScrollBar1->Position==_8Site2X4N  )                             //Wei 20231211 : 2X4NN Mode
        {
            iShuttlePitch=iXpitch*3*100;
            if(iShuttlePitch>=12000 && iShuttlePitch<=24000)
            {
                bShuttleSensorCanMove=true;
            }
        }

        if(bShuttleSensorCanMove==false)
        {
            ShowMyMessage("Auto Shuttle Sensor 不支援此Pitch!!");
            return false;
        }
    }
    else
    {
        return true;
    }

    return bShuttleSensorCanMove;
}
//---------------------------------------------------------------------------
void TfSetup::SetShtMode(bool bNormal, bool bOneSide, bool bUsedSht1, bool bUsedSht2) //Jimmychiu 20241203 : Disable opening all sites when changing shuttle mode.
{
    shtMode.Clear();
    shtMode.bNormal=bNormal;
    shtMode.bOneSide=bOneSide;
    shtMode.bUsedSht1=bUsedSht1;
    shtMode.bUsedSht2=bUsedSht2;
}
//------------------------------------------------------------------------------
bool TfSetup::VertifyShtModeisDiff(bool bNormal, bool bOneSide, bool bUsedSht1, bool bUsedSht2) //Jimmychiu 20241203 : Disable opening all sites when changing shuttle mode.
{
    if(shtMode.bNormal!=bNormal ||
       shtMode.bOneSide!=bOneSide ||
       shtMode.bUsedSht1!=bUsedSht1 ||
       shtMode.bUsedSht2!=bUsedSht2 )
    {
        return true;
    }
    else
    {
        return false;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::btAutoShuttlePitchClick(TObject *Sender)
{
    if(AUTO_SENSOR_INSTALL)                                                     //wei 20160914 Auto Shuttle Sensor
        fMain->Timer9->Enabled=true;
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::XShiftPitchMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Barcode_Reader(bcSetup)==0)
    {
        return;
    }

    if(IniConfig.bSPILFunction==true)
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, -4000.0, 4000.0);
    else
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 3, true, -4000.0, 4000.0);
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::CoSocketComboChange(TObject *Sender)
{
    for(int i=0; i<iSnSocketCnt; i++)                                           //Steven 20200420 : Socket Sensor功能可以選
    {
        if(i<atoi(CoSocketCombo->Text.c_str()))
        {
            MyTempRGBox[i]->Visible=true;
            if(TestIF_File.bSocketSensorCheckFloating)
                MyTempRGBox[i]->ItemIndex=2;
            else
                MyTempRGBox[i]->ItemIndex=1;
        }
        else
        {
            MyTempRGBox[i]->Visible=false;
            MyTempRGBox[i]->ItemIndex=0;
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::rgSensor1Click(TObject *Sender)
{
    TRadioGroup *Temp=(TRadioGroup *)Sender;
    if(Temp->Visible==true && Temp->ItemIndex==0)
        Temp->ItemIndex=1;
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::rgYPitchOffsetModeClick(TObject *Sender)
{
    if(rgYPitchOffsetMode->ItemIndex==1)                                        //Steven 20201013 : NN mode add Y-Pitch offset selection
    {
        labYOffset->Caption="Y Offset (mm)";                                    //KenHsieh 20231227 : add Y Center Pitch mode
        labYOffset->Width=93;
        edYOffset->Enabled=false;
        edYOffset->Text=10;
    }
    else if(rgYPitchOffsetMode->ItemIndex==0)                                   //KenHsieh 20231227 : add Y Center Pitch mode
    {
        labYOffset->Caption="Y Offset (mm)";
        labYOffset->Width=93;
        edYOffset->Text= FormatFloat("0.000", TestIF_File.dSiteYOffset);        //Steven 20140512 : For HT-9047 //jou 2016-04-22 Setup pitch "0.00" -> "0.000"
        edYOffset->Enabled=true;
    }
    else
    {
        labYOffset->Caption="Y Center Pitch (mm)";                              //KenHsieh 20231227 : add Y Center Pitch mode
        labYOffset->Width=134;
        edYOffset->Text= FormatFloat("0.000", TestIF_File.dSiteYOffset);        //Steven 20140512 : For HT-9047 //jou 2016-04-22 Setup pitch "0.00" -> "0.000"
        edYOffset->Enabled=true;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::cbI21Click(TObject *Sender)
{                                                                               //Ifor 20210913 add:關閉Site Mapping
    if(CosFunction.bI21EnableASMByRecipe)                                       //Steven 20210518 : 吳如春希望Auto site map從工作檔開關
    {
        if(cbI21->Checked==false    &&
           CosFunction.bAutoSiteMappingUseHotPlate==true    &&
           IniConfig.bI21AutoSiteMappingUseHotplate==true   &&
           LastSet.iRunStartMode==rsmAutoSiteMap            )
        {
            if(InArmSuck.HasIC()==false && OutArmSuck.HasIC()==false &&
               ShuttleHasIC()==false    && IndexHasIC()==false)
            {
                NewRecordProcess("", "Has no device, manually close AutoSiteMapping OK!", "無device，手動關閉AutoSiteMapping OK!");
            }
            else
            {
                if(cbI21->Checked==false)
                {
                    cbI21->Checked=true;
                    ShowMyMessage("Device remain in machine, do onecycle first, and can close AutoSiteMapping!","仍有device，請先執行onecycle才能關閉AutoSiteMapping");
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::FormShortCut(TWMKey &Msg, bool &Handled)
{
   if(Msg.CharCode==VK_F5)                                                      //Sam 20220725 : 介面新增鍵盤 F5 將視窗恢復初始位置
    {
        Left=75;
        Top=10;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::edOverRangeClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_DOUBLE, 0, true, 100.0, 0.0);
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::edDelayTimeClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_DOUBLE, 0, true, 10.0, 0.05);
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::edAuto1CountClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 40, 2);
}
//------------------------------------------------------------------------------
void __fastcall TfSetup::edtGetValueDelayTimeClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_DOUBLE, 1, true, 0, 1);
}
//------------------------------------------------------------------------------
