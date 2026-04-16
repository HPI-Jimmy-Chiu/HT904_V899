#include "MachineDefine.h"
#pragma hdrstop

#include "cTrayForm.h"
#include "cProd.h"
#include "common.h"
#include "cConfiguration.h"
#include "mymotor.h"
#include "uShowMessage.h"
#include "cSecurity.h"
#include "BarcodeReader.h"
#include "cMyDB.h"
#include "cBuilder.h"
#include "csystem.h"
#include "myQwertyKeyBoard.h"
#include "main.h"
#include "HTEditList.h"
#include "uHGemHT9045.h"
#include "cAuthority.h"
#include "mymessbox.h"
//------------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "HTray"
#pragma resource "*.dfm"
TfTrayForm *fTrayForm;

//------------------------------------------------------------------------------
__fastcall TfTrayForm::TfTrayForm(TComponent* Owner)
    : TForm(Owner)
{
    bool bShow=true, bNoShow=false;
    bool bEnable=true;//, bDisable=false;
    bool bReadFromFile=true;//, bFixedValue=false;

    TEdit *TempEdit[3][17]= //2014-03-04    Dell    10->14 (for SPIL WLP Add Tray Block)     //Frank 20160920 add
    {
        {TrayName1, XST1, YST1, XPitch1, YPitch1, Tp1Thick, XCT1, YCT1, Tp1TickUp, edMemo1, XBStart1, YBStart1, XBP1, YBP1, XBItem1, YBItem1, YBTypeSize1},
        {TrayName2, XST2, YST2, XPitch2, YPitch2, Tp2Thick, XCT2, YCT2, Tp2TickUp, edMemo2, XBStart2, YBStart2, XBP2, YBP2, XBItem2, YBItem2, YBTypeSize2},
        {TrayName3, XST3, YST3, XPitch3, YPitch3, Tp3Thick, XCT3, YCT3, Tp3TickUp, edMemo3, XBStart3, YBStart3, XBP3, YBP3, XBItem3, YBItem3, YBTypeSize3}
    };

    XCT1->Tag=0;                                                                //Steven 20251003 : 避免出錯
    XCT2->Tag=1;
    XCT3->Tag=2;
    YCT1->Tag=0;
    YCT2->Tag=1;
    YCT3->Tag=2;
    XPitch1->Tag=0;
    XPitch2->Tag=1;
    XPitch3->Tag=2;
    YPitch1->Tag=0;
    YPitch2->Tag=1;
    YPitch3->Tag=2;

    for(int i=0; i<3; i++)
        for(int j=0; j<17; j++)
            TrayEdit[i][j]=TempEdit[i][j];

    elTrayForm->Add(edBoatYDivision,         &UserDefForm_File[0].iCassetteZItem,    ECPosInt,     "Type0",    "Cassette Z Item",        bShow, bEnable, bReadFromFile, 14,      false, 1, 50); //RogerYang 20260203 : Add for HT9046CR
    elTrayForm->Add(edBoatYStart,  &UserDefForm_File[0].dCassetteZStart,   ECDouble,     "Type0",    "Cassette Z Start",       bShow, bEnable, bReadFromFile, 500,      false, 1, 3000);
    elTrayForm->Add(edBoatYPitch,       &UserDefForm_File[0].dCassetteZPitch,   ECDouble,     "Type0",    "Cassette Z Pitch",       bShow, bEnable, bReadFromFile, 500,      false, 1, 3000);

//    elTrayForm->Add(TrayName1,  &UserDefForm_File[0].Alias,       ECText,       "Type0",    "Name",                 bShow, bEnable, bReadFromFile, AnsiString("NONE"),      true);
    elTrayForm->Add(XCT1,       &UserDefForm_File[0].XDivision,   ECPosInt,     "Type0",    "X Division",           bShow, bEnable, bReadFromFile, 14,      false, 1, 1000);
    elTrayForm->Add(YCT1,       &UserDefForm_File[0].YDivision,   ECPosInt,     "Type0",    "Y Division",           bShow, bEnable, bReadFromFile, 35,      false, 1, 1000);
    elTrayForm->Add(XST1,       &UserDefForm_File[0].XStart,      ECDouble,     "Type0",    "X Start",              bShow, bEnable, bReadFromFile, 0.1,     true, 0.001, 1000.00);
    elTrayForm->Add(YST1,       &UserDefForm_File[0].YStart,      ECDouble,     "Type0",    "Y Start",              bShow, bEnable, bReadFromFile, 0.1,     true, 0.001, 1000.00);
    elTrayForm->Add(XPitch1,    &UserDefForm_File[0].XPitch,      ECDouble,     "Type0",    "X Pitch",              bShow, bEnable, bReadFromFile, 0.1,     true, 0.000, 1000.00);
    elTrayForm->Add(YPitch1,    &UserDefForm_File[0].YPitch,      ECDouble,     "Type0",    "Y Pitch",              bShow, bEnable, bReadFromFile, 0.1,     true, 0.001, 1000.00);
    elTrayForm->Add(Tp1Thick,   &UserDefForm_File[0].ZDepth,      ECDouble,     "Type0",    "Think",                bShow, bEnable, bReadFromFile, 0.1,     true, 0.001, 1000.00);
    elTrayForm->Add(Tp1TickUp,   &UserDefForm_File[0].iPickUp,    ECDouble,     "Type0",    "Pick Up",              bShow, bEnable, bReadFromFile, 0.1,     true, 0.001, 1000.00);
//    elTrayForm->Add(edMemo1,    &UserDefForm_File[0].Alias,       ECText,       "Type0",    "Memo",                 bShow, bEnable, bReadFromFile, AnsiString("NONE"),      true);
    if(IniConfig.bUseTrayBlockMode)
    {
        elTrayForm->Add(XBStart1,   &UserDefForm_File[0].BlockXStart, ECDouble,     "Type0",    "Block XStart",         bShow, bEnable, bReadFromFile, 0.1,     true, 0.001, 1000.00);
        elTrayForm->Add(YBStart1,   &UserDefForm_File[0].BlockYStart, ECDouble,     "Type0",    "Block YStart",         bShow, bEnable, bReadFromFile, 0.1,     true, 0.001, 1000.00);
        elTrayForm->Add(XBP1,       &UserDefForm_File[0].BlockPitchX, ECDouble,     "Type0",    "Block PitchX",         bShow, bEnable, bReadFromFile, 0.1,     true, 0.001, 1000.00);
        elTrayForm->Add(YBP1,       &UserDefForm_File[0].BlockPitchY, ECDouble,     "Type0",    "Block PitchY",         bShow, bEnable, bReadFromFile, 0.1,     true, 0.001, 1000.00);
        elTrayForm->Add(XBItem1,    &UserDefForm_File[0].BlockXItem,  ECPosInt,     "Type0",    "Block XItem",          bShow, bEnable, bReadFromFile, 2,      false, 1, 1000);
        elTrayForm->Add(YBItem1,    &UserDefForm_File[0].BlockYItem,  ECPosInt,     "Type0",    "Block YItem",          bShow, bEnable, bReadFromFile, 5,      false, 1, 1000);
        elTrayForm->Add(YBTypeSize1,&UserDefForm_File[0].BlockTraySize, ECText,     "Type0",    "Block Tray Type Size", bShow, bEnable, bReadFromFile, 0.1,     true, 0.001, 1000.00);
    }

//    elTrayForm->Add(TrayName2,  &UserDefForm_File[1].Alias,       ECText,       "Type1",    "Name",                 bShow, bEnable, bReadFromFile, AnsiString("NONE"),      true);
    elTrayForm->Add(XST2,       &UserDefForm_File[1].XStart,      ECDouble,     "Type1",    "X Start",              bShow, bEnable, bReadFromFile, 0.1,     true, 0.001, 1000.00);
    elTrayForm->Add(YST2,       &UserDefForm_File[1].YStart,      ECDouble,     "Type1",    "Y Start",              bShow, bEnable, bReadFromFile, 0.1,     true, 0.001, 1000.00);
    elTrayForm->Add(XPitch2,    &UserDefForm_File[1].XPitch,      ECDouble,     "Type1",    "X Pitch",              bShow, bEnable, bReadFromFile, 0.1,     true, 0.000, 1000.00);
    elTrayForm->Add(YPitch2,    &UserDefForm_File[1].YPitch,      ECDouble,     "Type1",    "Y Pitch",              bShow, bEnable, bReadFromFile, 0.1,     true, 0.001, 1000.00);
    elTrayForm->Add(Tp2Thick,   &UserDefForm_File[1].ZDepth,      ECDouble,     "Type1",    "Think",                bShow, bEnable, bReadFromFile, 0.1,     true, 0.001, 1000.00);
    elTrayForm->Add(XCT2,       &UserDefForm_File[1].XDivision,   ECPosInt,     "Type1",    "X Division",           bShow, bEnable, bReadFromFile, 14,     false, 1, 1000);
    elTrayForm->Add(YCT2,       &UserDefForm_File[1].YDivision,   ECPosInt,     "Type1",    "Y Division",           bShow, bEnable, bReadFromFile, 35,     false, 1, 1000);
    elTrayForm->Add(Tp2TickUp,   &UserDefForm_File[1].iPickUp,    ECDouble,     "Type1",    "Pick Up",              bShow, bEnable, bReadFromFile, 0.1,     true, 0.001, 1000.00);
//    elTrayForm->Add(edMemo2,    &UserDefForm_File[1].Alias,       ECText,       "Type1",    "Memo",                 bShow, bEnable, bReadFromFile, AnsiString("NONE"),      true);
    if(IniConfig.bUseTrayBlockMode)
    {
        elTrayForm->Add(XBStart2,   &UserDefForm_File[1].BlockXStart, ECDouble,     "Type1",    "Block XStart",         bShow, bEnable, bReadFromFile, 0.1,     true, 0.001, 1000.00);
        elTrayForm->Add(YBStart2,   &UserDefForm_File[1].BlockYStart, ECDouble,     "Type1",    "Block YStart",         bShow, bEnable, bReadFromFile, 0.1,     true, 0.001, 1000.00);
        elTrayForm->Add(XBP2,       &UserDefForm_File[1].BlockPitchX, ECDouble,     "Type1",    "Block PitchX",         bShow, bEnable, bReadFromFile, 0.1,     true, 0.001, 1000.00);
        elTrayForm->Add(YBP2,       &UserDefForm_File[1].BlockPitchY, ECDouble,     "Type1",    "Block PitchY",         bShow, bEnable, bReadFromFile, 0.1,     true, 0.001, 1000.00);
        elTrayForm->Add(XBItem2,    &UserDefForm_File[1].BlockXItem,  ECPosInt,     "Type1",    "Block XItem",          bShow, bEnable, bReadFromFile, 2,      false, 1, 1000);
        elTrayForm->Add(YBItem2,    &UserDefForm_File[1].BlockYItem,  ECPosInt,     "Type1",    "Block YItem",          bShow, bEnable, bReadFromFile, 5,      false, 1, 1000);
        elTrayForm->Add(YBTypeSize2,&UserDefForm_File[1].BlockTraySize, ECText,     "Type1",    "Block Tray Type Size", bShow, bEnable, bReadFromFile, 0.1,     true, 0.001, 1000.00);
    }

//    elTrayForm->Add(TrayName1,  &UserDefForm_File[2].Alias,       ECText,       "Type2",    "Name",                 bShow, bEnable, bReadFromFile, AnsiString("NONE"),      true);
    elTrayForm->Add(XST3,       &UserDefForm_File[2].XStart,      ECDouble,     "Type2",    "X Start",              bShow, bEnable, bReadFromFile, 0.1,     true, 0.001, 1000.00);
    elTrayForm->Add(YST3,       &UserDefForm_File[2].YStart,      ECDouble,     "Type2",    "Y Start",              bShow, bEnable, bReadFromFile, 0.1,     true, 0.001, 1000.00);
    elTrayForm->Add(XPitch3,    &UserDefForm_File[2].XPitch,      ECDouble,     "Type2",    "X Pitch",              bShow, bEnable, bReadFromFile, 0.1,     true, 0.000, 1000.00);
    elTrayForm->Add(YPitch3,    &UserDefForm_File[2].YPitch,      ECDouble,     "Type2",    "Y Pitch",              bShow, bEnable, bReadFromFile, 0.1,     true, 0.001, 1000.00);
    elTrayForm->Add(Tp3Thick,   &UserDefForm_File[2].ZDepth,      ECDouble,     "Type2",    "Think",                bShow, bEnable, bReadFromFile, 0.1,     true, 0.001, 1000.00);
    elTrayForm->Add(XCT3,       &UserDefForm_File[2].XDivision,   ECPosInt,     "Type2",    "X Division",           bShow, bEnable, bReadFromFile, 14,     false, 1, 1000);
    elTrayForm->Add(YCT3,       &UserDefForm_File[2].YDivision,   ECPosInt,     "Type2",    "Y Division",           bShow, bEnable, bReadFromFile, 35,     false, 1, 1000);
    elTrayForm->Add(Tp3TickUp,   &UserDefForm_File[2].iPickUp,    ECDouble,     "Type2",    "Pick Up",              bShow, bEnable, bReadFromFile, 0.1,     true, 0.001, 1000.00);
//    elTrayForm->Add(edMemo3,    &UserDefForm_File[2].Alias,       ECText,       "Type2",    "Memo",                 bShow, bEnable, bReadFromFile, AnsiString("NONE"),      true);
    if(IniConfig.bUseTrayBlockMode)
    {
        elTrayForm->Add(XBStart3,   &UserDefForm_File[2].BlockXStart, ECDouble,     "Type2",    "Block XStart",         bShow, bEnable, bReadFromFile, 0.1,     true, 0.001, 1000.00);
        elTrayForm->Add(YBStart3,   &UserDefForm_File[2].BlockYStart, ECDouble,     "Type2",    "Block YStart",         bShow, bEnable, bReadFromFile, 0.1,     true, 0.001, 1000.00);
        elTrayForm->Add(XBP3,       &UserDefForm_File[2].BlockPitchX, ECDouble,     "Type2",    "Block PitchX",         bShow, bEnable, bReadFromFile, 0.1,     true, 0.001, 1000.00);
        elTrayForm->Add(YBP3,       &UserDefForm_File[2].BlockPitchY, ECDouble,     "Type2",    "Block PitchY",         bShow, bEnable, bReadFromFile, 0.1,     true, 0.001, 1000.00);
        elTrayForm->Add(XBItem3,    &UserDefForm_File[2].BlockXItem,  ECPosInt,     "Type2",    "Block XItem",          bShow, bEnable, bReadFromFile, 2,      false, 1, 1000);
        elTrayForm->Add(YBItem3,    &UserDefForm_File[2].BlockYItem,  ECPosInt,     "Type2",    "Block YItem",          bShow, bEnable, bReadFromFile, 5,      false, 1, 1000);
        elTrayForm->Add(YBTypeSize3,&UserDefForm_File[2].BlockTraySize, ECText,     "Type2",    "Block Tray Type Size", bShow, bEnable, bReadFromFile, 0.1,     true, 0.001, 1000.00);
    }
    elTrayForm->SetBarcodeType(bcTrayForm);
    asErrorMsg="";
    if(USE_COLORSENSOR_MUN==eCSMUN_Install_Loader)                              //Jimmychiu 20230630 : add color sensor MU-N in Loader
    {
        InitialColorSensorGUI();                                                //Jimmychiu 20230630 : add color sensor MU-N in Loader
        elTrayForm->Add(GetColorSensor("ColorSensor_FT")->edEnable,        &TestIF_File.sLoaderColorSenFTEnable,     ECText,     "Color Sensor", "sLoaderColorSenFTEnable" ,  bNoShow, bEnable, bReadFromFile, "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0");
        elTrayForm->Add(GetColorSensor("ColorSensor_RT")->edEnable,        &TestIF_File.sLoaderColorSenRTEnable,     ECText,     "Color Sensor", "sLoaderColorSenRTEnable" ,  bNoShow, bEnable, bReadFromFile, "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0");
        tsColorSensor->TabVisible=true;
        LoadColorSensorMapping();
    }
    else
    {
        tsColorSensor->TabVisible=false;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfTrayForm::FormShow(TObject *Sender)
{
    AnsiString S;
    LoadImage();
    ReadFile();
    DoIniDataToForm();
    S.sprintf("Tray Form  ''%s''  ", GetLastOpenFN());
    Caption=S;
    PageControl1->ActivePage=TabSheet1;
    UpDateType();

    cbTrayType1->ItemIndex=0;
    cbTrayType2->ItemIndex=0;
    cbTrayType3->ItemIndex=0;
    cbTrayType1->Items->Clear();
    cbTrayType2->Items->Clear();
    cbTrayType3->Items->Clear();
    cbTrayType1->Text="Select from Database...";
    cbTrayType2->Text="Select from Database...";
    cbTrayType3->Text="Select from Database...";

    if(bHasTrayCSV)                                                             //Steven 20210629 : Tray Form改成CSV
    {
        fConfiguration->sbtReloadTray->Click();
        for(int i=0; i<fConfiguration->strngrdTray->RowCount; i++)
        {
            if(fConfiguration->strngrdTray->Cells[0][i]!="" &&
               fConfiguration->strngrdTray->Cells[0][i]!=" ")
            {
                cbTrayType1->Items->Add(fConfiguration->strngrdTray->Cells[0][i]);
                cbTrayType2->Items->Add(fConfiguration->strngrdTray->Cells[0][i]);
                cbTrayType3->Items->Add(fConfiguration->strngrdTray->Cells[0][i]);
            }
        }
    }

    tsBinBox->TabVisible=(IniConfig.bBinBox || IniConfig.bShowTrayAndDeviceDir);
    //jou 2012-12-11 support Bin Box start
    edtBinBoxNow->Text   = LastSet.iBinBoxCount;
    pnlBixBox->Visible   = IniConfig.bBinBox;
    //jou 2012-12-11 support Bin Box end

    //jou 2013-03-25 show Tray & Device Direction start
    rgDeviceDir->Visible        = IniConfig.bShowTrayAndDeviceDir;
    rgTrayDir->Visible          = IniConfig.bShowTrayAndDeviceDir;
    chkEnableIndicator->Visible = IniConfig.bShowTrayAndDeviceDir;              //Steven 20190211 : 可以取消顯示IC方向

    cbUseThickTray->Visible=CosFunction.bThickTrayUseDiffHeight;                //JerryYang 20230620 : add厚tray選項

    if(IniConfig.bShowTrayAndDeviceDir)
    {
        tsBinBox->Caption="Tray && Device Direct";
    }
    //jou 2013-03-25 show Tray & Device Direction end

    if(IniConfig.bUseTrayBlockMode && IniConfig.bP06_LoaderUseCarrierTray==true)
    {
        gb_TrayBlock1->Visible=true;
        gb_TrayBlock2->Visible=true;
        gb_TrayBlock3->Visible=true;
    }

    //jou 981207 權限控制
    if(CUSTOMER_CODE==CC_SCC || CUSTOMER_CODE==CC_SCK)                          //Steven 20101221   //ChungHung 20130621 add SCK RMS
    {
        if(IniConfig.bEnableRms && AccessLevel<=iDefEngineerLevel)              //jou 2014-06-19 Security Have 5 Level 1->iDefEngineerLevel
        {
            TabSheet1->Enabled=false;
            TabSheet2->Enabled=false;
            TabSheet3->Enabled=false;
            GroupBox1->Enabled=false;
        }
        else if(HasICUnderMachine()) //Steven 20110826
        {
            TabSheet1->Enabled=false;
            TabSheet2->Enabled=false;
            TabSheet3->Enabled=false;
            GroupBox1->Enabled=false;
        }
        else
        {
            TabSheet1->Enabled=fSecurity->Insufficient(14, false);
            TabSheet2->Enabled=fSecurity->Insufficient(14, false);
            TabSheet3->Enabled=fSecurity->Insufficient(14, false);
            GroupBox1->Enabled=fSecurity->Insufficient(14, false);
        }
    }
    else
    {
        if(HasICUnderMachine())                                                 //Steven 20110826
        {
            TabSheet1->Enabled=false;
            TabSheet2->Enabled=false;
            TabSheet3->Enabled=false;
            GroupBox1->Enabled=false;
        }
        else
        {
            TabSheet1->Enabled=fSecurity->Insufficient(14, false);
            TabSheet2->Enabled=fSecurity->Insufficient(14, false);
            TabSheet3->Enabled=fSecurity->Insufficient(14, false);
            GroupBox1->Enabled=fSecurity->Insufficient(14, false);
            if(CosFunction.bLotStartLockCriticalPara && RunInfo.bLotStart)      //JerryYang 20220311 : ATP鎖定Critical parameter
            {
                if(bAuthCriticalPara[18])
                {
                    TabSheet1->Enabled=false;
                    TabSheet2->Enabled=false;
                    TabSheet3->Enabled=false;
                    GroupBox1->Enabled=false;
                }
            }
        }
    }
    if(IniConfig.bC03UseCatchTray)                                              //kevin 20220302 add tray X width
    {
        Lab1XPickup->Caption="Catch Tray Range:60-73";
        Lab2XPickup->Caption="Catch Tray Range:60-73";
        Lab3XPickup->Caption="Catch Tray Range:60-73";
    }
    else
    {
        Lab1XPickup->Caption="Tray Range:30-110";
        Lab2XPickup->Caption="Tray Range:30-110";
        Lab3XPickup->Caption="Tray Range:30-110";
    }

    pnlUnloadTrayCount->Visible=(CUSTOMER_CODE==CC_VTEST_Shanghai);

    if(USE_COLORSENSOR_MUN==eCSMUN_Install_Loader)                              //Jimmychiu 20230630 : add color sensor MU-N in Loader
    {
        tsColorSensor->TabVisible=true;
        LoadColorSensorMapping();
    }
    else
    {
        tsColorSensor->TabVisible=false;
    }
    Top=10;
    Left=200;
    fShow=true;

    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);                                  //Steven 20100629
}
//------------------------------------------------------------------------------
void __fastcall TfTrayForm::LoadImage()
{
    Image1->Picture->LoadFromFile(BmpPath+"TrayForm2.bmp");
    Image2->Picture->LoadFromFile(BmpPath+"TrayForm2.bmp");
    Image3->Picture->LoadFromFile(BmpPath+"TrayForm2.bmp");
    Image4->Picture->LoadFromFile(BmpPath+"TrayForm3.bmp");
    Image5->Picture->LoadFromFile(BmpPath+"TrayForm3.bmp");
    Image6->Picture->LoadFromFile(BmpPath+"TrayForm3.bmp");

    if(IniConfig.bShowTrayAndDeviceDir)                                         //jou 2013-03-25 show Tray & Device Direction
    {
        if(FileExists(BmpPath+"Device0.bmp"))
            imgDevice0->Picture->LoadFromFile(BmpPath+"Device0.bmp");
        if(FileExists(BmpPath+"Device90.bmp"))
            imgDevice90->Picture->LoadFromFile(BmpPath+"Device90.bmp");
        if(FileExists(BmpPath+"Device180.bmp"))
            imgDevice180->Picture->LoadFromFile(BmpPath+"Device180.bmp");
        if(FileExists(BmpPath+"Device270.bmp"))
            imgDevice270->Picture->LoadFromFile(BmpPath+"Device270.bmp");
        if(FileExists(BmpPath+"Tray0.bmp"))
            imgTray0->Picture->LoadFromFile(BmpPath+"Tray0.bmp");
        if(FileExists(BmpPath+"Tray180.bmp"))
            imgTray180->Picture->LoadFromFile(BmpPath+"Tray180.bmp");
        if(FileExists(BmpPath+"Tray0_Flip.bmp"))
            imgTray0_Flip->Picture->LoadFromFile(BmpPath+"Tray0_Flip.bmp");
        if(FileExists(BmpPath+"Tray180_Flip.bmp"))
            imgTray180_Flip->Picture->LoadFromFile(BmpPath+"Tray180_Flip.bmp");
    }
}
//------------------------------------------------------------------------------
void __fastcall TfTrayForm::DoIniDataToForm()                                   //kevin 20181109
{
    AnsiString S="", asString="";
    S=GetLastOpenFN();
    AnsiString szDir="";
    szDir.sprintf("%s%s", DataPath, S);

    if(elTrayForm!=NULL)
    {
        elTrayForm->InitialDataToEdit();
    }

    TMyTray1->XItem=UserDefForm_File[0].XDivision;
    TMyTray1->YItem=UserDefForm_File[0].YDivision;
    TMyTray2->XItem=UserDefForm_File[1].XDivision;
    TMyTray2->YItem=UserDefForm_File[1].YDivision;
    TMyTray3->XItem=UserDefForm_File[2].XDivision;
    TMyTray3->YItem=UserDefForm_File[2].YDivision;

    szDir+="\\Tray.Data";

    for(int i=0; i<3; i++)
    {
        TrayEdit[i][0]->Text     =UserDefForm_File[i].Alias;
        TrayEdit[i][9]->Text     =UserDefForm_File[i].cMemo;
    }

    if(IniConfig.bBinBox)                                                       //jou 2012-12-11 support Bin Box
    {
        edtBinBoxName->Text =UserDefForm_File[3].Alias;
        edtBinBoxAlarm->Text=UserDefForm_File[3].iBinBoxAlarm;
    }

    if(IniConfig.bShowTrayAndDeviceDir)                                         //jou 2013-03-25 show Tray & Device Direction
    {
        rgDeviceDir->ItemIndex      =UserDefForm_File[0].iDeviceDirection;
        rgTrayDir->ItemIndex        =UserDefForm_File[0].iTrayDirection;
        chkEnableIndicator->Checked =UserDefForm_File[0].bEnableIndicator;      //Steven 20190211 : 可以取消顯示IC方向
    }

    if(CosFunction.bThickTrayUseDiffHeight)                                     //JerryYang 20230620 : add厚tray選項
    {
        cbUseThickTray->Checked=UserDefForm_File[0].bUseThickTray;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfTrayForm::ReadFile()
{
    AnsiString S="", asString="";
    S=GetLastOpenFN();
    AnsiString szDir="";
    szDir.sprintf("%s%s\\", DataPath, S);                                       //Steven 20210716 : Add \\
    if(elTrayForm!=NULL)
    {
        elTrayForm->ReadEditTextFromFile(szDir, "Tray.Data");
        elTrayForm->InitialDataToEdit();
    }
    szDir+="Tray.Data";
    for(int i=0; i<3; i++)
    {
        S="Type"+AnsiString(i);
        asString=ReadIniData(szDir, S, "Name", AnsiString("NONE"));
        strncpy(UserDefForm_File[i].Alias,   asString.c_str(), sizeof(UserDefForm_File[i].Alias));

        asString=ReadIniData(szDir, S, "Memo", AnsiString("NONE"));
        strncpy(UserDefForm_File[i].cMemo,   asString.c_str(), sizeof(UserDefForm_File[i].cMemo));

        if(IniConfig.bSPILFunction)                                             //JerryYang 20250731 : 矽品凱富要求不要強制改0
        {

        }
        else
        {
            if(UserDefForm_File[i].XDivision==1)                                //Steven 20240808 : Tray只有一行的, X Pitch為0
                UserDefForm_File[i].XPitch=0;
        }
    }

    //jou 2012-12-11 support Bin Box start
    if(IniConfig.bBinBox)
    {
        edtBinBoxName->Text=ReadIniData(szDir, "Bin Box", "Name", AnsiString("Bin Box"));
        strncpy(UserDefForm_File[3].Alias, edtBinBoxName->Text.c_str(), sizeof(UserDefForm_File[3].Alias));
        edtBinBoxAlarm->Text=ReadIniData(szDir, "Bin Box", "Alarm Count", 1000);

        UserDefForm_File[3].XStart      =0;
        UserDefForm_File[3].YStart      =0;

        if(UserDefForm_File[0].XPitch<12.80)
            UserDefForm_File[3].XPitch  =UserDefForm_File[0].XPitch*2.0;
        else
            UserDefForm_File[3].XPitch  =UserDefForm_File[0].XPitch;

        UserDefForm_File[3].YPitch      =UserDefForm_File[0].YPitch;
        UserDefForm_File[3].ZDepth      =0;
        UserDefForm_File[3].XDivision   =4;
        UserDefForm_File[3].YDivision   =2;
        UserDefForm_File[3].iPickUp     =0;
        strncpy(UserDefForm_File[3].cMemo, "", sizeof(UserDefForm_File[3].cMemo));
        UserDefForm_File[3].iBinBoxAlarm=atoi(edtBinBoxAlarm->Text.c_str());
    }
    //jou 2012-12-11 support Bin Box end

    //jou 2013-03-25 show Tray & Device Direction start
    if(IniConfig.bShowTrayAndDeviceDir)
    {
        rgDeviceDir->ItemIndex      =ReadIniData(szDir, "ShowTrayAndDeviceDir", "Device Direction", 0);
        rgTrayDir->ItemIndex        =ReadIniData(szDir, "ShowTrayAndDeviceDir", "Tray Direction", 0);
        chkEnableIndicator->Checked =ReadIniData(szDir, "ShowTrayAndDeviceDir", "Enable Indicator", true); //Steven 20190211 : 可以取消顯示IC方向

        for(int i=0; i<4; i++)
        {
            UserDefForm_File[i].iDeviceDirection=rgDeviceDir->ItemIndex;
            UserDefForm_File[i].iTrayDirection  =rgTrayDir->ItemIndex;
            UserDefForm_File[i].bEnableIndicator=chkEnableIndicator->Checked;   //Steven 20190211 : 可以取消顯示IC方向
        }
    }
    //jou 2013-03-25 show Tray & Device Direction end

    if(CosFunction.bThickTrayUseDiffHeight)                                     //JerryYang 20230620 : add厚tray選項
    {
        UserDefForm_File[0].bUseThickTray=ReadIniData(szDir, "Flag", "UseThickTray", false);
    }

    if(CUSTOMER_CODE==CC_VTEST_Shanghai)
    {
        TrayForm.iUnloadTrayCount =CheckAndReadIniData(szDir, "Other", "Unload Tray Count", 10);
        edUnloadTrayCount->Text=TrayForm.iUnloadTrayCount;
    }

    if(IniConfig.bShowTrayAndDeviceDir)                                         //jou 2013-03-25 show Tray & Device Direction
    {
        rgDeviceDirClick(this);
        rgTrayDirClick(this);
    }
    LoadColorSensorEnable();                                                    //Jimmychiu 20230630 : add color sensor MU-N in Loader
}
//------------------------------------------------------------------------------
void __fastcall TfTrayForm::PageControl1Change(TObject *Sender)
{
    UpDateType();
}
//------------------------------------------------------------------------------
void __fastcall TfTrayForm::UpDateType()
{
    cbCopyFrom->Clear();
    AnsiString S[3]={"Type1", "Type2", "Type3"};
    for(int i=0; i<3; i++)
        if(i!=PageControl1->ActivePage->Tag)
            cbCopyFrom->Items->Add(S[i]);
}
//------------------------------------------------------------------------------
void __fastcall TfTrayForm::XST1KeyPress(TObject *Sender, char &Key)
{
    if(OnlyNumberAndDotInPut(Key)==false)
        Key=NULL;
}
//------------------------------------------------------------------------------
void __fastcall TfTrayForm::XCT1KeyPress(TObject *Sender, char &Key)
{
    if(OnlyNumberInPut(Key)==false)
        Key=NULL;
}
//------------------------------------------------------------------------------
void __fastcall TfTrayForm::YST1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Barcode_Reader(bcTrayForm)==0)                                           // 20140103 wei KYEC Barcode Reader
    {
        return;
    }

    if(CUSTOMER_CODE==CC_ASE_SG)                                                //Ifor 20250926 add: ASE SG 要求最小值為0
    {
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 3, true, 0.000, 1000.00);
    }
    else
    {
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 3, true, 0.001, 1000.00);
    }
}
//------------------------------------------------------------------------------
void __fastcall TfTrayForm::XCT1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Barcode_Reader(bcTrayForm)==0)                                           // 20140103 wei KYEC Barcode Reader
    {
        return;
    }

    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 1, 1000);
    TEdit *TempEdit=(TEdit *)Sender;
    if(     TempEdit->Tag==0)
    {
        if(TempEdit->Text=="1")
            XPitch1->Text="0";
        TMyTray1->XItem=atoi(TempEdit->Text.c_str());
    }
    else if(TempEdit->Tag==1)
    {
        if(TempEdit->Text=="1")
            XPitch2->Text="0";
        TMyTray2->XItem=atoi(TempEdit->Text.c_str());
    }
    else if(TempEdit->Tag==2)
    {
        if(TempEdit->Text=="1")
            XPitch3->Text="0";
        TMyTray3->XItem=atoi(TempEdit->Text.c_str());
    }
}
//------------------------------------------------------------------------------
void __fastcall TfTrayForm::YCT1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Barcode_Reader(bcTrayForm)==0)                                           // 20140103 wei KYEC Barcode Reader
    {
        return;
    }

    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 1, 1000);
    TEdit *TempEdit=(TEdit *)Sender;
    if(     TempEdit->Tag==0)
    {
        if(TempEdit->Text=="1")
            YPitch1->Text="0";
        TMyTray1->YItem=atoi(TempEdit->Text.c_str());
    }
    else if(TempEdit->Tag==1)
    {
        if(TempEdit->Text=="1")
            YPitch2->Text="0";
        TMyTray2->YItem=atoi(TempEdit->Text.c_str());
    }
    else if(TempEdit->Tag==2)
    {
        if(TempEdit->Text=="1")
            YPitch3->Text="0";
        TMyTray3->YItem=atoi(TempEdit->Text.c_str());
    }
}
//------------------------------------------------------------------------------
 void __fastcall TfTrayForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    ReadFile();
    DoIniDataToForm();                                                          //JerryYang 20250411 : 離開頁面要刷新一次, 避免誤存檔
    fShow=false;

    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);    //Steven 20100629
}
//------------------------------------------------------------------------------
void __fastcall TfTrayForm::cbTrayType1Change(TObject *Sender)
{
    if(Barcode_Reader(bcTrayForm)==0)                                           // 20140103 wei KYEC Barcode Reader
    {
        return;
    }
    TComboBox *ComboBox=(TComboBox *)Sender;
    ShowTypePage(ComboBox->Tag, ComboBox->ItemIndex);
}
//------------------------------------------------------------------------------
void __fastcall TfTrayForm::ShowTypePage(int iTag, int index)
{
    TTMyTray *TMyTray[3]={TMyTray1, TMyTray2, TMyTray3};
    if(bHasTrayCSV)                                                             //Steven 20210629 : Tray Form改成CSV
    {
        if(index<1)
            return;

        fConfiguration->sbtReloadTray->Click();
        TrayEdit[iTag][0]->Text =fConfiguration->strngrdTray->Cells[0][index].Trim();   //"Package Type"
        TrayEdit[iTag][1]->Text =fConfiguration->strngrdTray->Cells[1][index].Trim();   //"X Start Pos"
        TrayEdit[iTag][2]->Text =fConfiguration->strngrdTray->Cells[2][index].Trim();   //"Y Start Pos"
        TrayEdit[iTag][3]->Text =fConfiguration->strngrdTray->Cells[3][index].Trim();   //"X Pitch"
        TrayEdit[iTag][4]->Text =fConfiguration->strngrdTray->Cells[4][index].Trim();   //"Y Pitch"
        TrayEdit[iTag][5]->Text =fConfiguration->strngrdTray->Cells[9][index].Trim();   //"Z Tray Tickness"
        TrayEdit[iTag][6]->Text =fConfiguration->strngrdTray->Cells[5][index].Trim();   //"Columns (X)"
        TrayEdit[iTag][7]->Text =fConfiguration->strngrdTray->Cells[6][index].Trim();   //"Rows (Y)"
        TrayEdit[iTag][8]->Text =atof(fConfiguration->strngrdTray->Cells[7][index].Trim().c_str())/2.0;   //"X Width"       //Steven 20210826 : 修正讀取Tray X width需要除以二
        TrayEdit[iTag][9]->Text =fConfiguration->strngrdTray->Cells[11][index].Trim();  //"Memo"
        TMyTray[iTag]->XItem    =atoi(fConfiguration->strngrdTray->Cells[5][index].c_str());   //"Columns (X)
        TMyTray[iTag]->YItem    =atoi(fConfiguration->strngrdTray->Cells[6][index].c_str());   //"Rows (Y)")
    }
}
//------------------------------------------------------------------------------
void __fastcall TfTrayForm::sbtExitClick(TObject *Sender)
{
    sbtExit->Down=false;
    Close();
}
//------------------------------------------------------------------------------
void __fastcall TfTrayForm::spbSaveClick(TObject *Sender)
{
    if(IniConfig.bA02DisableSaveParsWhenSwitchToOp==true &&                     //RogerYang 20260305 張寧說時間到都要登出，且不可被更改任何設定(A01_2)
        AccessLevel==0)
    {
        ShowMyMessage("[A01_2]目前已切換到Operator權限，\r\n請重新登入再做設定!");
        Close();
    }

    AnsiString S=GetLastOpenFN();
    AnsiString szDir="";
    szDir.sprintf("%s%s\\", DataPath, S);                                       //Steven 20210716 : Add \\

//kevin 20150105  Start 另存 jobfile
    SaveSetupFile(szDir, S);//kevin 20150105
    #ifdef ASE_KaohSiung
        fBuilder->bSaveAsJobFile(S,"JOBFILE");
    #endif
//kevin 20150105 end
    AddSpace(szDir);
    ReadFile();
    spbSave->Down=false;
    if(IniConfig.bEnable_SECS_GEM==true)
    {
        bHasSaveSet=true;                                                       //Ifor 20151204 新增判斷機台有無修改設定檔
        EventReport(SECS_EVENT.SaveRecipe);
    }
    fMain->BackupSetupFile();                                                   //Ifor 20170620 (wei) add Auto BackUp Setup File & Last Data
}
//------------------------------------------------------------
//kevin 20140105 儲存 檔案另存 jobfile
//------------------------------------------------------------
void __fastcall TfTrayForm::SaveSetupFile(AnsiString szDir, AnsiString S)
{
    MyForceDirectories(szDir);
    SaveColorSensorEnable();                                                    //Jimmychiu 20230630 : add color sensor MU-N in Loader
    if(elTrayForm!=NULL)
    {
        elTrayForm->SaveEditTextToFile(szDir, "Tray.Data");
    }

    szDir+="Tray.Data";
    for(int i=0; i<3; i++)
    {
        S="Type"+AnsiString (i);
        WriteIniData(szDir, S, "Name",      TrayEdit[i][0]->Text);
        WriteIniData(szDir, S, "Memo",      TrayEdit[i][9]->Text);
    }

    //jou 2012-12-11 support Bin Box start
    if(IniConfig.bBinBox)
    {
        WriteIniData(szDir, "Bin Box", "Name",          edtBinBoxName->Text);
        WriteIniData(szDir, "Bin Box", "Alarm Count",   edtBinBoxAlarm->Text);
    }
    //jou 2012-12-11 support Bin Box end

    //jou 2013-03-25 show Tray & Device Direction start
    if(IniConfig.bShowTrayAndDeviceDir)
    {
        WriteIniData(szDir, "ShowTrayAndDeviceDir", "Device Direction",  rgDeviceDir->ItemIndex);
        WriteIniData(szDir, "ShowTrayAndDeviceDir", "Tray Direction",    rgTrayDir->ItemIndex);
        WriteIniData(szDir, "ShowTrayAndDeviceDir", "Enable Indicator",  chkEnableIndicator->Checked); //Steven 20190211 : 可以取消顯示IC方向
    }
    //jou 2013-03-25 show Tray & Device Direction end

    if(CUSTOMER_CODE==CC_VTEST_Shanghai)
    {
        WriteIniData(szDir, "Other", "Unload Tray Count",  edUnloadTrayCount->Text);
    }

    if(CosFunction.bThickTrayUseDiffHeight)                                     //JerryYang 20230620 : add厚tray選項
    {
        WriteIniData(szDir, "Flag", "UseThickTray",  cbUseThickTray->Checked);

        if(HasICUnderMachine()==false && cbUseThickTray->Checked==false && atof(Tp1Thick->Text.c_str())>9)
        {
            ShowMyMessage("tray厚度大於9mm, 請確認是否需要勾選Thick tray選項換算in/out arm高度");
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfTrayForm::spbCopyClick(TObject *Sender)
{
    if(Barcode_Reader(bcTrayForm)==0)                                           // 20140103 wei KYEC Barcode Reader
    {
        return;
    }
    spbCopy->Down=false;
    int i, CopyIndex=0, NowIndex;
    if(cbCopyFrom->Text=="") return;
    AnsiString S[3]={"Type1", "Type2", "Type3"};
    NowIndex=PageControl1->ActivePageIndex;
    for(i=0; i<3; i++)
    {
        if(S[i]==cbCopyFrom->Text)
        {
            CopyIndex=i;
            break;
        }
    }
    for(i=0; i<9; i++)
        TrayEdit[NowIndex][i]->Text=TrayEdit[CopyIndex][i]->Text;
}
//------------------------------------------------------------------------------
void __fastcall TfTrayForm::edtBinBoxAlarmMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 5000, 100);
}
//------------------------------------------------------------------------------
void __fastcall TfTrayForm::edtBinBoxAlarmKeyPress(TObject *Sender,
      char &Key)
{
    if(OnlyNumberInPut(Key)==false)
        Key=NULL;
}
//------------------------------------------------------------------------------
void __fastcall TfTrayForm::btnBinBoxResetClick(TObject *Sender)
{
    edtBinBoxNow->Text=0;
    LastSet.iBinBoxCount=0;
    MOT[MManualTray3].ClearTray(__FUNC__);
}
//------------------------------------------------------------------------------
void __fastcall TfTrayForm::rgDeviceDirClick(TObject *Sender)
{
    TImage *Image[4]={imgDevice0, imgDevice90, imgDevice180, imgDevice270};
    AnsiString asString[4] ={"\\Device0.bmp", "\\Device90.bmp", "\\Device180.bmp", "\\Device270.bmp"};
    AnsiString asString1[4]={"\\Here.bmp", "\\Here1.bmp", "\\Here1.bmp", "\\Here.bmp"};

    for(int i=0; i<4; i++)
    {
        if(i==rgDeviceDir->ItemIndex)
        {
            Image[i]->Visible=true;
            if(FileExists(BmpPath+asString[i]))
                fShowMessage->imgDevice->Picture->LoadFromFile(BmpPath+asString[i]);

            if(FileExists(BmpPath+asString1[i]))
                fShowMessage->imgDeviceHere->Picture->LoadFromFile(BmpPath+asString1[i]);
        }
        else
        {
            Image[i]->Visible=false;
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfTrayForm::rgTrayDirClick(TObject *Sender)
{
    TImage *Image[4]={imgTray0, imgTray180, imgTray0_Flip, imgTray180_Flip};
    AnsiString asString[4] ={"\\Tray0.bmp", "\\Tray180.bmp", "\\Tray0_Flip.bmp", "\\Tray180_Flip.bmp"};
    AnsiString asString1[4]={"\\Here.bmp", "\\Here1.bmp", "\\Here1.bmp", "\\Here.bmp"};

    for(int i=0; i<4; i++)
    {
        if(i==rgTrayDir->ItemIndex)
        {
            Image[i]->Visible=true;
            if(FileExists(BmpPath+asString[i]))
                fShowMessage->imgTray->Picture->LoadFromFile(BmpPath+asString[i]);

            if(FileExists(BmpPath+asString1[i]))
                fShowMessage->imgTrayHere->Picture->LoadFromFile(BmpPath+asString1[i]);
        }
        else
        {
            Image[i]->Visible=false;
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfTrayForm::TrayName1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_NO_SYMBOL);
}
//------------------------------------------------------------------------------
void __fastcall TfTrayForm::Tp1TickUpMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Barcode_Reader(bcTrayForm)==0)                                           // 20140103 wei KYEC Barcode Reader
    {
        return;
    }

    if(IniConfig.bC03UseCatchTray)                                              //kevin 20220302 夾TRAY 模組 限定 TRAY 寬度避免 被 IN OUT ARM 撞機
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 3, true, 60, 73.00);    //kevin 20220302 set up Tray X Width range 40-100
    else
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 3, true, 30, 110.00);    //kevin 20220302 set up Tray X Width range 40-100
}
//------------------------------------------------------------------------------
void __fastcall TfTrayForm::FormShortCut(TWMKey &Msg, bool &Handled)
{
    if(Msg.CharCode==VK_F5)                                                     //Sam 20220725 : 介面新增鍵盤 F5 將視窗恢復初始位置
    {
        Top=10;
        Left=200;
    }
}
//------------------------------------------------------------------------------
void TfTrayForm::InitialColorSensorGUI()                                        //Jimmychiu 20230630 : add color sensor MU-N in Loader
{
    AddColorSensor("ColorSensor_FT", &TestIF_File.sLoaderColorSenFTEnable, lbFT_LCS->Top,lbFT_LCS->Left);
    AddColorSensor("ColorSensor_RT", &TestIF_File.sLoaderColorSenRTEnable, lbRT_LCS->Top,lbRT_LCS->Left);
}
//------------------------------------------------------------------------------
void TfTrayForm::LoadColorSensorMapping()                                       //Jimmychiu 20230630 : add color sensor MU-N in Loader
{
    AnsiString FileName="D:\\HT9045\\system\\ColorSensorType.ini";
    AnsiString arrName=CheckAndReadIniData(FileName, AnsiString("ColorDefine"), AnsiString("ColorName"),    AnsiString("None,Brown,Orange,Red,Yellow,White,Green,Blue,Black"));
    for(int i=0; i<GetColorSensorSize(); i++)
    {
        GetColorSensor(i)->SetCB_Name(arrName);
    }
}
//------------------------------------------------------------------------------
void TfTrayForm::LoadColorSensorEnable()                                        //Jimmychiu 20230630 : add color sensor MU-N in Loader
{
    for(int i=0; i<GetColorSensorSize(); i++)
    {
        GetColorSensor(i)->LoadColorSensorEnable();
    }
}
//------------------------------------------------------------------------------
void TfTrayForm::SaveColorSensorEnable()                                        //Jimmychiu 20230630 : add color sensor MU-N in Loader
{
    for(int i=0; i<GetColorSensorSize(); i++)
    {
        GetColorSensor(i)->SaveColorSensorEnable();
    }
}
//------------------------------------------------------------------------------
void TfTrayForm::AddColorSensor(AnsiString asName, AnsiString *_asArrBooleanData, int iTop, int iLeft)
{
    uColorSensorInfo *uci=new uColorSensorInfo(asName, _asArrBooleanData, gbUseLoaderColorSensor, iTop, iLeft);
    mapCSInfo[GetColorSensorSize()]=uci;
}
//------------------------------------------------------------------------------
void TfTrayForm::ClearAllColorSensor()
{
    for(int i=0; i<GetColorSensorSize(); i++)
    {
        delete GetColorSensor(i);
    }
}
//------------------------------------------------------------------------------
bool TfTrayForm::IsEnableColorSensor()
{
    return (USE_COLORSENSOR_MUN==eCSMUN_Install_Loader && IniConfig.bC17UseLoaderColorSensor==true);
}
//------------------------------------------------------------------------------
uColorSensorInfo* TfTrayForm::GetColorSensor(int iIndex)
{
    iIndex=(iIndex>=GetColorSensorSize() || iIndex<0)?0:iIndex;
    return mapCSInfo[iIndex];
}
//------------------------------------------------------------------------------
uColorSensorInfo* TfTrayForm::GetColorSensor(AnsiString asName)
{
    for(int i=0; i<GetColorSensorSize(); i++)
    {
        if(GetColorSensor(i)->asName.Pos(asName)>0)
            return GetColorSensor(i);
    }
    return NULL;
}
//------------------------------------------------------------------------------
uColorSensorInfo::uColorSensorInfo(AnsiString _asName,AnsiString *_asArrBooleanData, TGroupBox *gbUseLoaderColorSensor, int iTop, int iLeft)
{
    asName=_asName;
    asArrBooleanData=_asArrBooleanData;
    for(int i=0; i<MAX_COLORSENSOR_NUM; i++)
    {
        cbLoaderColorSensorMUN[i]                    = new TCheckBox(this);
        cbLoaderColorSensorMUN[i]->Parent            = gbUseLoaderColorSensor;
        cbLoaderColorSensorMUN[i]->Caption           = AnsiString().sprintf("%d", i+1);;
        cbLoaderColorSensorMUN[i]->Visible           = true;
        cbLoaderColorSensorMUN[i]->Top               = iTop;
        cbLoaderColorSensorMUN[i]->Left              = iLeft+(70*(i+1));
    }
    edEnable=new TEdit(this);
}
//------------------------------------------------------------------------------
uColorSensorInfo::~uColorSensorInfo()
{
    for(int i=0; i<MAX_COLORSENSOR_NUM; i++)
    {
        delete cbLoaderColorSensorMUN[i];
    }
    delete edEnable;
}
//------------------------------------------------------------------------------
void uColorSensorInfo::SetCB_Name(AnsiString asCommaText)
{
    TStringList* slArrName=new TStringList();
    slArrName->CommaText=asCommaText;
    for(int i=0; i<MAX_COLORSENSOR_NUM; i++)
    {
        if(i<slArrName->Count)
        {
            cbLoaderColorSensorMUN[i]->Caption=slArrName->Strings[i];
            cbLoaderColorSensorMUN[i]->Visible=true;
            if(i==0)//first set is No Setting Color
            {
                cbLoaderColorSensorMUN[i]->Visible=false;
            }
        }
        else
        {
            cbLoaderColorSensorMUN[i]->Visible=false;
        }
    }
    delete slArrName;
}
//------------------------------------------------------------------------------
void uColorSensorInfo::LoadColorSensorEnable()
{
    TStringList* slArrText=new TStringList();
    slArrText->CommaText=*asArrBooleanData;
    bool bvalue=false;
    for(int i=0; i<MAX_COLORSENSOR_NUM; i++)
    {
        if(i<slArrText->Count)
        {
            bvalue=(slArrText->Strings[i]=="1")?true:false;
            cbLoaderColorSensorMUN[i]->Checked=bvalue;
            if(i==0)//first set is No Setting Color
            {
                cbLoaderColorSensorMUN[i]->Checked=false;
            }
        }
        else
        {
            cbLoaderColorSensorMUN[i]->Checked=false;
        }
    }
    delete slArrText;
}
//------------------------------------------------------------------------------
void uColorSensorInfo::SaveColorSensorEnable()
{
    TStringList* slArrText=new TStringList();
    bool bvalue=false;
    for(int i=0; i<MAX_COLORSENSOR_NUM; i++)
    {
        bvalue=cbLoaderColorSensorMUN[i]->Checked;
        slArrText->Add((bvalue)?"1":"0");
    }
    edEnable->Text=slArrText->CommaText;
    delete slArrText;
}
//------------------------------------------------------------------------------
bool uColorSensorInfo::IsColorEable(int iInput,AnsiString &sErrorMsg)
{
    if(iInput>=0 && iInput<MAX_COLORSENSOR_NUM)
    {
        if(cbLoaderColorSensorMUN[iInput]->Checked==true)
        {
            return true;
        }
        else
        {
            sErrorMsg=AnsiString().sprintf("Color %s is not Allowed", GetColorName(iInput));
            return false;
        }
    }
    else                                                                        //非範圍內
    {
        sErrorMsg="UnKnown Color";
        return false;
    }
}
//------------------------------------------------------------------------------
void uColorSensorInfo::SetColorEnable(AnsiString sColor, bool bEnable)
{
    for(int i=0; i<MAX_COLORSENSOR_NUM; i++)
    {
        asTemp=GetColorName(i);
        if(asTemp.Pos(sColor)>0)
        {
            cbLoaderColorSensorMUN[i]->Checked=bEnable;
        }
        else
        {
            continue;
        }
    }
}
//------------------------------------------------------------------------------
void uColorSensorInfo::SetAllEnable(bool bEnable)
{
    for(int i=0; i<MAX_COLORSENSOR_NUM; i++)
    {
        cbLoaderColorSensorMUN[i]->Checked=bEnable;
    }
}
//------------------------------------------------------------------------------
AnsiString uColorSensorInfo::GetColorName(int iIndex)
{
    if(iIndex>=0 && iIndex<MAX_COLORSENSOR_NUM)
    {
        return cbLoaderColorSensorMUN[iIndex]->Caption;
    }
    else                                                                        //非範圍內
    {
        return "UnKnown Color";
    }
}
//------------------------------------------------------------------------------
void __fastcall TfTrayForm::XPitch1Click(TObject *Sender)
{
    if(Barcode_Reader(bcTrayForm)==0)                                           // 20140103 wei KYEC Barcode Reader
    {
        return;
    }

    TEdit *TempEdit=(TEdit *)Sender;
    if(     TempEdit->Tag==0)
    {
        if(XCT1->Text=="1")
        {
            if(CUSTOMER_CODE==CC_SCC)                                           //Steven 20251105 : JSCC張林濤要求可以改成0.01
                fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 3, true, 0.000, 1000.00);
            else
                XPitch1->Text="0";
        }
        else
        {
            fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 3, true, 0.001, 1000.00);
        }
    }
    else if(TempEdit->Tag==1)
    {
        if(XCT1->Text=="1")
        {
            if(CUSTOMER_CODE==CC_SCC)
                fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 3, true, 0.000, 1000.00);
            else
                XPitch2->Text="0";
        }
        else
        {
            fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 3, true, 0.001, 1000.00);
        }
    }
    else if(TempEdit->Tag==2)
    {
        if(XCT1->Text=="1")
        {
            if(CUSTOMER_CODE==CC_SCC)
                fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 3, true, 0.000, 1000.00);
            else
                XPitch3->Text="0";
        }
        else
        {
            fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 3, true, 0.001, 1000.00);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTrayForm::edUnloadTrayCountMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 1, true, 1, 20);
}
//---------------------------------------------------------------------------
void __fastcall TfTrayForm::YPitch1Click(TObject *Sender)
{
    if(Barcode_Reader(bcTrayForm)==0)                                           // 20140103 wei KYEC Barcode Reader
    {
        return;
    }

    TEdit *TempEdit=(TEdit *)Sender;
    if(     TempEdit->Tag==0)
    {
        if(YCT1->Text=="1")
        {
            if(CUSTOMER_CODE==CC_SCC)
                fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 3, true, 0.000, 1000.00);
            else
                YPitch1->Text="0";
        }
        else
        {
            fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 3, true, 0.001, 1000.00);
        }
    }
    else if(TempEdit->Tag==1)
    {
        if(YCT1->Text=="1")
        {
            if(CUSTOMER_CODE==CC_SCC)
                fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 3, true, 0.000, 1000.00);
            else
                YPitch2->Text="0";
        }
        else
        {
            fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 3, true, 0.001, 1000.00);
        }
    }
    else if(TempEdit->Tag==2)
    {
        if(YCT1->Text=="1")
        {
            if(CUSTOMER_CODE==CC_SCC)
                fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 3, true, 0.000, 1000.00);
            else
                YPitch3->Text="0";
        }
        else
        {
            fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 3, true, 0.001, 1000.00);
        }
    }
}
//---------------------------------------------------------------------------
