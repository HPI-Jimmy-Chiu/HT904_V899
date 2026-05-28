#include "MachineDefine.h"
#pragma hdrstop

#include "cHotPlate.h"
#include "cUnitConvert.h"
#include "common.h"
#include "cprod.h"
#include "cConfiguration.h"
#include "mymessbox.h"
#include "cmydef.h"
#include "cSecurity.h"
#include "BarcodeReader.h"
#include "cMyDB.h"
#include "ainarm2.h"
#include "cBuilder.h"
#include "csystem.h"
#include "myQwertyKeyBoard.h"
#include "main.h"
#include "uHGemHT9045.h"
#include "MyKitSuck.h"
#include "cAuthority.h"
#include "ainarm_SearchPlacePlate.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfHotPlate *fHotPlate;
//---------------------------------------------------------------------------
__fastcall TfHotPlate::TfHotPlate(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
extern bool HasICUnderHotPlate();

void __fastcall TfHotPlate::FormShow(TObject *Sender)
{
    AnsiString S;
    LoadImage();
    ReadFile();
    DoIniDataToForm();
    S.sprintf("Hot Plate  ''%s''  ",GetLastOpenFN());
    Caption=S;
    cbSelectHPFromDB->ItemIndex=0;
    cbSelectHPFromDB->Items->Clear();
    cbSelectHPFromDB->Text="Select from Database...";

    if(bHasPlateCSV)                                                            //Steven 20210629 : Plate Form改成CSV
    {
        fConfiguration->sbtReloadHP->Click();
        for(int i=0; i<fConfiguration->strngrdHP->RowCount; i++)
        {
            if(fConfiguration->strngrdHP->Cells[0][i]!="" &&
               fConfiguration->strngrdHP->Cells[0][i]!=" ")
            {
                cbSelectHPFromDB->Items->Add(fConfiguration->strngrdHP->Cells[0][i]);
            }
        }
    }
    fShow=true;

    if(LastSet.iLanguageCountry==1)
        Label8->Caption="Note : 需要Clean Out後,才能改變Hotplate資料";
    else
        Label8->Caption="Note : Need Clean Out Can Change data";

    Top=10;
    Left=200;

    if(IniConfig.bHotPlateMove1CM)                                              //jou 2010-12-15
    {
        chkUseWideHotplate->Visible=true;
    }
    else
    {
        chkUseWideHotplate->Visible=false;
    }

   if(USE_ROTATE_KIT==1 && iRotate_Type==eCynRotate)                            //kevin 20130722  氣缸版
    {
        if(iRotate_In_Index==eripHotPlate1)
        {
            cbEnableHP1->Enabled=false;
            cbEnableHP1->Checked=false;
        }
        else if(iRotate_In_Index==eripHotPlate2)
        {
            cbEnableHP2->Enabled=false;
            cbEnableHP2->Checked=false;
        }
    }

    if(HasICUnderHotPlate())                                                    //Steven 20110826 : 不使用HasIcUnderMachine
    {
        GroupBox1->Enabled=false;
        GroupBox2->Enabled=false;
        Panel1->Enabled=false;
    }
    else                                                                        //jou 981207 權限控制
    {
        if(CUSTOMER_CODE==CC_SCC &&
           IniConfig.bEnableRms &&
           AccessLevel<=iDefEngineerLevel)                                      //jou 2014-06-19 Security Have 5 Level 1->iDefEngineerLevel
        {
            GroupBox1->Enabled=false;
            GroupBox2->Enabled=false;
            Panel1->Enabled=false;
        }
        else if(CosFunction.bLotStartLockCriticalPara && RunInfo.bLotStart)     //JerryYang 20220311 : ATP鎖定Critical parameter
        {
            if(bAuthCriticalPara[19])
            {
                GroupBox1->Enabled=false;
                GroupBox2->Enabled=false;
                Panel1->Enabled=false;
            }
        }
        else
        {
            GroupBox1->Enabled=fSecurity->Insufficient(15, false);
            GroupBox2->Enabled=fSecurity->Insufficient(15, false);
            Panel1->Enabled=fSecurity->Insufficient(15, false);
        }
    }

    if(CUSTOMER_CODE==CC_KYEC_LEE || CUSTOMER_CODE==CC_KYEC_XILINX)             //20140320 wei   KYEC 低於權限顯示不能修改
    {
        if(AccessLevel<LevelSet.AccessLevel[15])
        {
            GroupBox2->Enabled=false;
            GroupBox1->Enabled=false;
            Panel1->Enabled=false;
        }
        else
        {
            GroupBox2->Enabled=true;
            GroupBox1->Enabled=true;
            Panel1->Enabled=true;
        }
    }

    chkTrayHotplateCheck->Visible=(IniConfig.bVTESTFunction==true);             //jou 20240126 : Tray & hotplate by recipe MES控制檢查

    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);                                  //Steven 20100629
}
//---------------------------------------------------------------------------
void __fastcall TfHotPlate::LoadImage()
{
    Image1->Picture->LoadFromFile(BmpPath+"\\HoatPlate2.bmp");
    Image2->Picture->LoadFromFile(BmpPath+"\\HoatPlate2.bmp");
    Image3->Picture->LoadFromFile(BmpPath+"\\HoatPlate1.bmp");
}
//---------------------------------------------------------------------------
void __fastcall TfHotPlate::ReadFile()
{
    AnsiString S="", asString="";
    S=GetLastOpenFN();
    AnsiString szDir="";
    szDir.sprintf("%s%s", DataPath, S);
    szDir+="\\HotPlate.Data";

    asString=ReadIniData(szDir, "Hotplate Form", "Name", AnsiString("ReadDataError"));
    strncpy(HotPlateForm_File.Alias,  asString.c_str(), sizeof(HotPlateForm_File.Alias));
    HotPlateForm_File.XStart         =ReadIniData(szDir, "Hotplate Form", "X Start",    0.1);
    HotPlateForm_File.YStart         =ReadIniData(szDir, "Hotplate Form", "Y Start",    0.1);
    HotPlateForm_File.XPitch         =ReadIniData(szDir, "Hotplate Form", "X Pitch",    0.1);
    HotPlateForm_File.YPitch         =ReadIniData(szDir, "Hotplate Form", "Y Pitch",    0.1);
    HotPlateForm_File.XDivision      =ReadIniData(szDir, "Hotplate Form", "X Division", 1);     //Steven 20140516 : 0.1 --> 1
    HotPlateForm_File.YDivision      =ReadIniData(szDir, "Hotplate Form", "Y Division", 1);     //Steven 20140516 : 0.1 --> 1
    HotPlateForm_File.iPlateSelect   =ReadIniData(szDir, "Hotplate Form", "Using Flag", 2);     //Steven 20160729 : 0 --> 2 預設開啟加熱盤2

    if(HotPlateForm_File.XDivision<=0)                                          //Steven 20140516 : 0.1 --> 1
        HotPlateForm_File.XDivision=6;
    else if(HotPlateForm_File.XDivision==1)
        HotPlateForm_File.XPitch=0;

    if(HotPlateForm_File.YDivision<=0)                                          //Steven 20140516 : 0.1 --> 1
        HotPlateForm_File.YDivision=11;

    if(HotPlateForm_File.XPitch==26.67)                                         //Steven 20241212 : 26.67 x 3 = 8001
    {
        HotPlateForm_File.XPitch=26.66;
    }

    if(TestIF_File.iTestMode>=_6Site2X3 &&                                      //JerryYang 20230818 : fix 沒有轉換成Y=10
       TestIF_File.iTestMode!=_6Site2X3N &&
       TestIF_File.iTestMode!=_8Site2X4N &&                                     //Wei 20231211 : 2X4NN Mode
       HotPlateForm_File.XDivision==4 && HotPlateForm_File.YDivision%2==1)      //Steven 20230201 : 2x3以上不支援4x11
    {
        HotPlateForm_File.YDivision=10;
    }

    if(IniConfig.bHotPlateMove1CM)                                              //jou 2010-12-15
    {
        HotPlateForm_File.bUseWideHotplate   =ReadIniData(szDir, "Hotplate Form", "Use Wide Hotplate",   false);
    }
    else
    {
        HotPlateForm_File.bUseWideHotplate   =true;
    }

     if(IniConfig.bVTESTFunction==true)
        HotPlateForm_File.bTrayHotplateCheck      =CheckAndReadIniData(szDir, "System", "bTrayHotplateCheck",    true);  //jou 20240126 : Tray & hotplate by recipe MES控制檢查

    if(USE_ROTATE_KIT==1&& iRotate_Type==eCynRotate)                            //kevin 20130722  氣缸版
    {
        if(iRotate_In_Index==eripHotPlate1)
        {
            HotPlateForm_File.iPlateSelect=2;
        }
        else if(iRotate_In_Index==eripHotPlate2)
        {
            HotPlateForm_File.iPlateSelect=1;
        }
    }

    if(HotPlateForm_File.iPlateSelect<=0)                                       //Steven 20160729 : 最少要開一片加熱盤
        HotPlateForm_File.iPlateSelect=2;

    if(CosFunction.bAutoCleanUseHPSetByRecipe)                                  //Steven 20210825 : Auto Clean使用加熱盤要改成在工作檔設定
    {
        if(TestIF_File.iAutoClean_Function && IniConfig.bE43AutoCleanUseHotplate)
        {
            HotPlateForm_File.iPlateSelect=2;
        }
    }

    if(USE_PRECISER==1 && iPreciserInstallArea==2)                              //Ifor 20191008 : add Preciser Install Area
    {
        HotPlateForm_File.iPlateSelect=2;
    }

//    IniConfig.bHandlerCanUse8PickAtHotMode=(HOT_PLATE_LIMITATION==1);         //Steven 20151117 : 2x2 8Picker at Hot mode  //JerryYang 20161007 Mark

    if(i8PickerHPMode==iHPWideHP)                                               //JerryYang 20161007 簡化Hotplate判斷式
    {                                                                           //jou 2015-03-26 修正Hotplate Auto Download 檔案下載時也需加上保護避免Hand up
        if(HotPlateForm_File.XDivision==12)
        {
            HotPlateForm_File.XDivision=12;
        }
        else
        {
            if(HotPlateForm_File.XDivision>8)
                HotPlateForm_File.XDivision=8;
        }
    }
    else
    {
        if(HotPlateForm_File.XDivision==12 && (TestIF_File.iTestMode==QualSite2X2   ||      //Steven 20180409 (Jou) : Add 2x2 support X=12
                                               TestIF_File.iTestMode==_8Site2X4     ||
                                               TestIF_File.iTestMode==_16Site2X8    ||
                                               TestIF_File.iTestMode==_12Site2X6    ||
                                               TestIF_File.iTestMode==_16Site4X4    ||      //Sam 20190226 : 16Site4X4
                                               TestIF_File.iTestMode==_32Site4X8N))
        {
            HotPlateForm_File.XDivision=12;
        }
        else if(HotPlateForm_File.XDivision==16 && (TestIF_File.iTestMode==_8Site2X4  ||
                                                    TestIF_File.iTestMode==_16Site2X8 ||
                                                    TestIF_File.iTestMode==_12Site2X6 ||
                                                    TestIF_File.iTestMode==_16Site4X4 ||    //Sam 20190226 : 16Site4X4
                                                    TestIF_File.iTestMode==_32Site4X8N))    //Steven 20150826 : 16x24 Hot Plate for 32Site
        {
            HotPlateForm_File.XDivision=16;
        }
        else
        {
            if(HotPlateForm_File.XDivision>8)
                HotPlateForm_File.XDivision=8;
        }
    }

    if(USE_PICKER_COUNT==ep1Picker)                                             //JerryYang 20250902 : 單吸嘴模組可以支援X數量3
    {

    }
    else if(!(TestIF_File.iTestMode==DualSite ||
         TestIF_File.iTestMode==QualSite2X2N))                                  //Frank 20200520 2X2NN Mode
    {
        if(TestIF_File.iTestMode==SingleSite    ||
           TestIF_File.iTestMode==TriSite1X3    ||                              //Steven 20241126 : Add 1x3 for 3x7 HP
           TestIF_File.iTestMode==DualSite2x1   ||                              //Steven 20211228 : add for 2x1 mode
           TestIF_File.iTestMode==_6Site2X3N    ||                              //Steven 20220425 : 2X3NN Mode
           TestIF_File.iTestMode==_8Site2X4N)                                   //Wei 20231211 : 2X4NN Mode
        {
            ;
        }
        else
        {
            if(HotPlateForm_File.XDivision==3)
                HotPlateForm_File.XDivision=2;
        }
    }
    i8PickerHPMode=CheckHotPlate8PickMode();                                    //JerryYang 20161010 判斷HotPlate Offset 10mm
    SetArmHotPlateYPitch();                                                     //Steven 20180122 : For Y-Pitch at Hot Plate
}
//---------------------------------------------------------------------------
void __fastcall TfHotPlate::DoIniDataToForm()
{
    HotPlateName->Text      =HotPlateForm_File.Alias;

    XST1->Text              =FormatFloat("0.000", HotPlateForm_File.XStart);    //Steven 20160513 : 0.00 --> 0.0000 避免自動四捨五入
    YST1->Text              =FormatFloat("0.000", HotPlateForm_File.YStart);
    XPitch1->Text           =FormatFloat("0.000", HotPlateForm_File.XPitch);
    YPitch1->Text           =FormatFloat("0.000", HotPlateForm_File.YPitch);

    XCT1->Text              =HotPlateForm_File.XDivision;
    YCT1->Text              =HotPlateForm_File.YDivision;

    if(CosFunction.bAutoCleanUseHPSetByRecipe)                                  //Steven 20210825 : Auto Clean使用加熱盤要改成在工作檔設定
    {                                                                           //ChungHung 20131120 AutoClean use Hotplate1
        if(TestIF_File.iAutoClean_Function &&
           IniConfig.bE43AutoCleanUseHotplate)
        {
            cbEnableHP1->Visible=false;
        }
        else
        {
            cbEnableHP1->Visible=true;
        }
    }
    else
    {
        if(IniConfig.bE43AutoCleanUseHotplate)
        {
            cbEnableHP1->Visible=false;
        }
        else
        {
            cbEnableHP1->Visible=true;
        }
    }

    if(USE_PRECISER==1 && iPreciserInstallArea==2)                              //Ifor 20191008 : add Preciser Install Area
    {
        cbEnableHP1->Visible=false;
    }

    cbEnableHP2->Checked=HotPlateForm_File.iPlateSelect&0x02;
    cbEnableHP1->Checked=HotPlateForm_File.iPlateSelect&0x01;

    if(IniConfig.bHotPlateMove1CM)                                              //jou 2010-12-15
    {
        chkUseWideHotplate->Checked=HotPlateForm_File.bUseWideHotplate;
    }
    else
    {
        chkUseWideHotplate->Checked=true;
    }

    if(IniConfig.bVTESTFunction==true)
        chkTrayHotplateCheck->Checked=HotPlateForm_File.bTrayHotplateCheck;     //jou 20240126 : Tray & hotplate by recipe MES控制檢查
}
//---------------------------------------------------------------------------
void __fastcall TfHotPlate::XST1KeyPress(TObject *Sender, char &Key)
{
    if(OnlyNumberAndDotInPut(Key)==false)
        Key=NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfHotPlate::YCT1KeyPress(TObject *Sender, char &Key)
{
    if(OnlyNumberInPut(Key)==false)
        Key=NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfHotPlate::XST1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Barcode_Reader(bcPlateForm)==0)                                          // 20140103 wei KYEC Barcode Reader
    {
        return;
    }
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 3, true, 0.001, 1000.00);
}
//---------------------------------------------------------------------------
void __fastcall TfHotPlate::XCT1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Barcode_Reader(bcPlateForm)==0)                                          // 20140103 wei KYEC Barcode Reader
    {
        return;
    }
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 1, 1000);
    if(XCT1->Text=="1")
        XPitch1->Text="0";
}
//---------------------------------------------------------------------------
void __fastcall TfHotPlate::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    ReadFile();
    DoIniDataToForm();                                                          //JerryYang 20250411 : 離開頁面要刷新一次, 避免誤存檔
    fShow=false;

    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);                                  //Steven 20100629
}
//---------------------------------------------------------------------------
void __fastcall TfHotPlate::cbSelectHPFromDBChange(TObject *Sender)
{
    if(fShow==false)                                                            //Steven 20140401 : 避免讀取HotPlate資料庫
        return;

    if(Barcode_Reader(bcPlateForm)==0)                                          // 20140103 wei KYEC Barcode Reader
    {
        return;
    }
    TComboBox *ComboBox=(TComboBox *)Sender;
    int index=ComboBox->ItemIndex;

    if(bHasPlateCSV)                                                            //Steven 20210629 : Plate Form改成CSV
    {
        if(index<1)
            return;

        fConfiguration->sbtReloadHP->Click();
        HotPlateName->Text  =fConfiguration->strngrdHP->Cells[0][index].Trim(); //"Package Type"
        XST1->Text          =fConfiguration->strngrdHP->Cells[1][index].Trim(); //"X Start Pos"
        YST1->Text          =fConfiguration->strngrdHP->Cells[2][index].Trim(); //"Y Start Pos"
        XPitch1->Text       =fConfiguration->strngrdHP->Cells[3][index].Trim(); //"X Pitch"
        YPitch1->Text       =fConfiguration->strngrdHP->Cells[4][index].Trim(); //"Y Pitch"
        XCT1->Text          =fConfiguration->strngrdHP->Cells[5][index].Trim(); //"Columns (X)"
        YCT1->Text          =fConfiguration->strngrdHP->Cells[6][index].Trim(); //"Rows (Y)"
    }
}
//---------------------------------------------------------------------------
void __fastcall TfHotPlate::spbSaveClick(TObject *Sender)
{
    if(IniConfig.bA02DisableSaveParsWhenSwitchToOp==true &&                     //RogerYang 20260305 張寧說時間到都要登出，且不可被更改任何設定(A01_2)
        AccessLevel==0)
    {
        ShowMyMessage("[A01_2]目前已切換到Operator權限，\r\n請重新登入再做設定!");
        Close();
    }

    if(cbEnableHP1->Checked==false && cbEnableHP2->Checked==false)
    {
        ShowMyMessage("Need select at least one hotplate.", "請至少選擇一個加熱盤。");
        cbEnableHP2->Checked=true;
        return;
    }
    AnsiString S="";
    S=GetLastOpenFN();
    AnsiString szDir="";
    szDir.sprintf("%s%s", DataPath, S);

    SaveSetupFile(szDir, S);                                                    //kevin 20150105
    #ifdef ASE_KaohSiung
        fBuilder->bSaveAsJobFile(S, "JOBFILE");                                 //kevin 20150105  Start 另存 jobfile
    #endif
    AddSpace(szDir);
    ReadFile();
    spbSave->Down=false;
    if(IniConfig.bEnable_SECS_GEM==true)
    {
        bHasSaveSet=true;                                                       //Ifor 20151204 新增判斷機台有無修改設定檔
        EventReport(SECS_EVENT.SaveRecipe);
    }
    fMain->BackupSetupFile();                                                   //Ifor 20170508 (wei) add Auto BackUp Setup File & Last Data
    fMain->ChangeATCSiteUse();                                                  //Ztex 2024.09.06 Add Hot Plate ChangeATCSiteUse
}
//---------------------------------------------------------------------------
//kevin 20140105 儲存 檔案另存 jobfile
//---------------------------------------------------------------------------
extern bool HotPlateYPitchCanPutAll();
void __fastcall TfHotPlate::SaveSetupFile(AnsiString szDir, AnsiString S)
{
    int flag=0;

    int iXItem=atoi(XCT1->Text.c_str());
    int iXPitch1=atoi(XPitch1->Text.c_str());
    int iYPitch1=atoi(YPitch1->Text.c_str());                                   //JerryYang 20230822 : add

    if(USE_PICKER_COUNT==ep1Picker)                                             //JerryYang 20250902 : 單吸嘴模組可以支援X數量3
    {

    }
    else
    {
        if(i8PickerHPMode==iHPWideHP)                                               //JerryYang 20161007 簡化Hotplate判斷式  //Steven 20151117 : 2x2 8Picker at Hot mode
        {                                                                           //ChungHung Add 20101025 CC_ASE_CL HotPlate Offset 10mm
            if(iXItem==12)                                                          //ChungHung 20140421 add 開放 XItem 12
            {
                XCT1->Text=12;
            }
            else
            {
                if(iXItem>8)
                    XCT1->Text=8;
            }
        }
        else
        {
            if(iXItem==12 && (TestIF.iTestMode==QualSite2X2 ||                      //Steven 20180409 (Jou) : Add 2x2 support X=12
                              TestIF.iTestMode==_8Site2X4   ||
                              TestIF.iTestMode==_12Site2X6  ||
                              TestIF.iTestMode==_16Site2X8  ||
                              TestIF.iTestMode==_10Site2X5  ||
                              TestIF.iTestMode==_16Site4X4  ||                      //Sam 20190226 : 16Site4X4
                              TestIF.iTestMode==_32Site4X8N))
            {
                XCT1->Text=12;
            }
            else if(iXItem==16 && (TestIF.iTestMode==_8Site2X4  ||
                                   TestIF.iTestMode==_12Site2X6 ||
                                   TestIF.iTestMode==_16Site2X8 ||
                                   TestIF.iTestMode==_16Site4X4 ||                  //Sam 20190226 : 16Site4X4
                                   TestIF.iTestMode==_32Site4X8N))                  //Steven 20150826 : 16x24 Hot Plate for 32Site
            {
                XCT1->Text=16;
            }
            else
            {
                if(iXItem>8)
                    XCT1->Text=8;
            }
        }

        if(!(TestIF.iTestMode==DualSite ||                                          //20111015 Dell For Korea
             TestIF.iTestMode==QualSite2X2N))                                       //Frank 20200520 2X2NN Mode
        {
            if(TestIF_File.iTestMode==SingleSite    ||
               TestIF_File.iTestMode==TriSite1X3    ||                              //Steven 20241126 : Add 1x3 for 3x7 HP
               TestIF_File.iTestMode==DualSite2x1   ||                              //Steven 20211228 : add for 2x1 mode
               TestIF_File.iTestMode==_6Site2X3N    ||                              //Steven 20220425 : 2X3NN Mode
               TestIF_File.iTestMode==_8Site2X4N)                                   //Wei 20231211 : 2X4NN Mode
            {
                ;
            }
            else if(TestIF.iTestMode==_8Site2X4 ||                                  //Ifor 20151006 Add
                    TestIF.iTestMode==_16Site4X4)                                   //Sam 20190226 : 16Site4X4
            {
                if(ArmCanSuck4IC(0)==false)
                {
                    if(iXItem==3)
                    {
                        XCT1->Text=2;
                        iXPitch1=iXPitch1*2;
                        XPitch1->Text=iXPitch1;                                     
                    }
                }
                else
                {
                    if(iXItem==6 && iYPitch1==20)                                   //JerryYang 20230822 : add
                    {
                        XCT1->Text=4;
                        ShowMyMessage("Not support X-division: 6 (y-pitch: 20)");
                    }
                }
            }
            else
            {
                if(iXItem==3)
                    XCT1->Text=2;
            }
        }
    }

    MyForceDirectories(szDir);
    if(cbEnableHP2->Checked)
        flag+=2;
    if(cbEnableHP1->Checked)
        flag+=1;

//    LastSet.iRunStartMode=rsmInitialStart;                                    //jou 2012-01-17 make code,Save會讓Auto Tray疊料 //Eloit 2010_0819
//    fMain->cbRunStartMode->ItemIndex=LastSet.iRunStartMode;

    int iYStart=atof(YST1->Text.c_str())*100;
    if(USE_IN_Y_IS_AUTO_PITCH==false && iYStart<=1100)                                //Steven 20141203 : Hand Pitch 63.5遇到Y-Start=10且不能一次放時B排行程不足  //JerryYang 20251218 : IN/OUT ARM支援不同模組
    {
        if(TestIF.iTestMode>=QualSite2X2 && HotPlateYPitchCanPutAll()==false)
        {
            YCT1->Text=AnsiString(atoi(YCT1->Text.c_str())-1);
            YST1->Text=AnsiString(atof(YST1->Text.c_str())+atof(YPitch1->Text.c_str()));
        }
    }

    szDir+="\\HotPlate.Data";

//    if(atof(XPitch1->Text.c_str())==26.67)
//    {
//        XPitch1->Text="26.66";
//    }

    WriteIniData(szDir, "Hotplate Form", "Name",       HotPlateName->Text);
    WriteIniData(szDir, "Hotplate Form", "X Start",    FormatFloat("0.000", XST1->Text.ToDouble()));    //Steven 20220801 : 存檔時候要指定小數點位數
    WriteIniData(szDir, "Hotplate Form", "Y Start",    FormatFloat("0.000", YST1->Text.ToDouble()));
    WriteIniData(szDir, "Hotplate Form", "X Pitch",    FormatFloat("0.000", XPitch1->Text.ToDouble()));
    WriteIniData(szDir, "Hotplate Form", "Y Pitch",    FormatFloat("0.000", YPitch1->Text.ToDouble()));
    WriteIniData(szDir, "Hotplate Form", "X Division", XCT1->Text);
    WriteIniData(szDir, "Hotplate Form", "Y Division", YCT1->Text);
    WriteIniData(szDir, "Hotplate Form", "Using Flag", flag);

    if(IniConfig.bHotPlateMove1CM)                                              //jou 2010-12-15
    {
        if(atoi(XCT1->Text.c_str())==8 &&                                       //jou 2012-08-22 開啟右移1CM & 2x2 & 8支吸嘴 & Hotplate X=8 & X Pitch > 26.6 Hang up，須強制將Use Wide Hotplate=true
           TestIF.iUseSuckMode==8 &&
           TestIF.iTestMode==QualSite2X2 &&
           atof(XPitch1->Text.c_str())>=26.67)
        {
            chkUseWideHotplate->Checked=true;
        }

        WriteIniData(szDir, "Hotplate Form", "Use Wide Hotplate", chkUseWideHotplate->Checked);
    }
    else
    {
        WriteIniData(szDir, "Hotplate Form", "Use Wide Hotplate", true);
    }

    if(IniConfig.bVTESTFunction==true)
        WriteIniData(szDir, "System", "bTrayHotplateCheck", chkTrayHotplateCheck->Checked);    //jou 20240126 : Tray & hotplate by recipe MES控制檢查
}
//---------------------------------------------------------------------------
void __fastcall TfHotPlate::sbtExitClick(TObject *Sender)
{
    sbtExit->Down=false;
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfHotPlate::HotPlateNameMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_NO_SYMBOL);
}
//---------------------------------------------------------------------------
int __fastcall TfHotPlate::CheckHotPlate8PickMode()                             //JerryYang 20161007 簡化Hot plate offset判斷式
{
    double dHotPlateWidth=(2*HotPlateForm_File.XStart+HotPlateForm_File.XPitch*(HotPlateForm_File.XDivision-1));
    if(IniConfig.bHotPlateMove1CM &&
       HotPlateForm.bUseWideHotplate==false)                                    //ChungHung Add 20101025 CC_ASE_CL HotPlate Offset 10mm
    {
        return iHPWideHP;
    }
    else if(HOT_PLATE_POSITION==1 && dHotPlateWidth<=170)
    {
        if(TestIF.iTestMode==QualSite2X2 &&
           TestIF.iUseSuckMode==8 &&
           HotPlateForm_File.XDivision==6)                                      //JerryYang 20161222 (jou) 暫不支援6x11 2x2_8吸嘴 //Steven 20151117 : 2x2 8Picker at Hot mode  暫時不支援6x11 8吸嘴
        {
            return iHPNoUse8suck;
        }
        else
        {
            return iHPWideHP;
        }
    }
    else if(HotPlateForm_File.XDivision==6 &&
            HotPlateForm_File.XPitch>27.0)                                      //Steven 20180516 : 6x11加大 X-Pitch 35mm
    {
        return iHPWideHP;
    }
    else if(TestIF.iTestMode==DualSite &&
            HOT_PLATE_POSITION==1)                                              //JerryYang 20170603 (wei) hot plate pin在右要能跑Dual site 4吸嘴
    {
        return iHPWideHP;
    }
    else if(TestIF.iTestMode==QualSite2X2 &&
            TestIF.iUseSuckMode==8 &&
            HotPlateForm_File.XDivision==12)                                    //Steven 20180409 (Jou) : Add 2x2 support X=12
    {
        return iHPWideHP;
    }
    else
    {
        return iHPNoUse8suck;
    }
}
//---------------------------------------------------------------------------
void TfHotPlate::SetArmHotPlateYPitch()                                         //Steven 20180122 : For Y-Pitch at Hot Plate
{
    if(USE_IN_Y_IS_AUTO_PITCH==true)                                                  //Steven 20131002 : XY變距  //JerryYang 20251218 : IN/OUT ARM支援不同模組
    {
        if(HotPlateForm_File.XDivision<8)
        {
            if(HotPlateForm_File.YDivision==16)
            {
                TestIF_File.iARM_HP_Y_PITCH=iUnitMultiply100(HotPlateForm_File.YPitch)*4;
                if(TestIF_File.iARM_HP_Y_PITCH>IN_OUT_ARM_Y_PITCH_MAX)
                    TestIF_File.iARM_HP_Y_PITCH=iUnitMultiply100(HotPlateForm_File.YPitch)*2;
            }
            else
            {
                TestIF_File.iARM_HP_Y_PITCH=TestIF_File.iARM_Y_PITCH;
            }
        }
        else
        {
            if(HotPlateForm_File.YDivision==12)
            {
                TestIF_File.iARM_HP_Y_PITCH=iUnitMultiply100(HotPlateForm_File.YPitch)*3;
                if(TestIF_File.iARM_HP_Y_PITCH>IN_OUT_ARM_Y_PITCH_MAX)
                    TestIF_File.iARM_HP_Y_PITCH=iUnitMultiply100(HotPlateForm_File.YPitch)*2;
            }
            else if(HotPlateForm_File.YDivision==16)
            {
                TestIF_File.iARM_HP_Y_PITCH=iUnitMultiply100(HotPlateForm_File.YPitch)*4;
                if(TestIF_File.iARM_HP_Y_PITCH>IN_OUT_ARM_Y_PITCH_MAX)
                    TestIF_File.iARM_HP_Y_PITCH=iUnitMultiply100(HotPlateForm_File.YPitch)*2;
            }
            else if(HotPlateForm_File.YDivision==18)
            {
                TestIF_File.iARM_HP_Y_PITCH=iUnitMultiply100(HotPlateForm_File.YPitch)*3;
                if(TestIF_File.iARM_HP_Y_PITCH>IN_OUT_ARM_Y_PITCH_MAX)
                {
                    if(HotPlateForm_File.YPitch>20.0)
                        TestIF_File.iARM_HP_Y_PITCH=iUnitMultiply100(HotPlateForm_File.YPitch);
                    else
                        TestIF_File.iARM_HP_Y_PITCH=TestIF_File.iARM_Y_PITCH;
                }
            }
            else if(HotPlateForm_File.YDivision==24)
            {
                TestIF_File.iARM_HP_Y_PITCH=iUnitMultiply100(HotPlateForm_File.YPitch)*4;
                if(TestIF_File.iARM_HP_Y_PITCH>IN_OUT_ARM_Y_PITCH_MAX)
                    TestIF_File.iARM_HP_Y_PITCH=iUnitMultiply100(HotPlateForm_File.YPitch)*3;
            }
            else
            {
                TestIF_File.iARM_HP_Y_PITCH=TestIF_File.iARM_Y_PITCH;
            }
        }
    }
    else
    {
        TestIF_File.iARM_HP_Y_PITCH=TestIF_File.iARM_Y_PITCH;
    }

    GetHotPlateYHalfPos();
}
//---------------------------------------------------------------------------
void TfHotPlate::ShowTypePage(int index)                                        //JimmyChiu 20220901 add ShowTypePage func in cHotPlate.cpp
{
    if(bHasPlateCSV)                                                            //Steven 20210629 : Plate Form改成CSV           void ShowTypePage(int index);
    {
        if(index<1)
            return;
        fConfiguration->sbtReloadHP->Click();
        HotPlateName->Text  =fConfiguration->strngrdHP->Cells[0][index].Trim(); //"Package Type"
        XST1->Text          =fConfiguration->strngrdHP->Cells[1][index].Trim(); //"X Start Pos"
        YST1->Text          =fConfiguration->strngrdHP->Cells[2][index].Trim(); //"Y Start Pos"
        XPitch1->Text       =fConfiguration->strngrdHP->Cells[3][index].Trim(); //"X Pitch"
        YPitch1->Text       =fConfiguration->strngrdHP->Cells[4][index].Trim(); //"Y Pitch"
        XCT1->Text          =fConfiguration->strngrdHP->Cells[5][index].Trim(); //"Columns (X)"
        YCT1->Text          =fConfiguration->strngrdHP->Cells[6][index].Trim(); //"Rows (Y)"
    }
}
//---------------------------------------------------------------------------
void __fastcall TfHotPlate::FormShortCut(TWMKey &Msg, bool &Handled)
{
    if(Msg.CharCode==VK_F5)                                                     //Sam 20220725 : 介面新增鍵盤 F5 將視窗恢復初始位置
    {
        Top=10;
        Left=200;
    }
}
//---------------------------------------------------------------------------
