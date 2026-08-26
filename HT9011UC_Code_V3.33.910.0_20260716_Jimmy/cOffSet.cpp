#include "MachineDefine.h"
#pragma hdrstop

#include "cOffSet.h"
#include "common.h"
#include "cprod.h"
//#include "INPUT.h"
//#include "ArmOffsetData.h"
#include "bthermo.h"
#include "note.h"
#include "cAuthority.h"
#include "cMyDef.h"
#include "main.h"
#include "myQwertyKeyBoard.h"
#include "cSecurity.h"
#include "cBuilder.h"
#include "BarcodeReader.h"                                                      // 2013.11.29 , Joye , KYEC Barcode Reader  20140103 wei
#include "BarCode.h"
#include "csystem.h"
#include "mymessbox.h"
#include "ProductionInfo.h"                                                     //Sam 20170809 (Steven) 移植超豐 OEE 功能 form HT-7045
#include "cinitial.h"
#include "fAOI.h"

#include "ainarm2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma resource "*.dfm"
TfOffSet *fOffSet;
AnsiString CapStr[OfsTotal]=
{
    "Loader",                           //0
    "Hot Plate1",                       //1
    "Hot Plate2",                       //2
    "Input Shuttle1",                   //3
    "Input Shuttle2",                   //4
    "Output Shuttle1",                  //5
    "Output Shuttle2",                  //6
    "Auto1",                            //7
    "Auto2",                            //8
    "Auto3",                            //9
    "Auto4",                            //10
    "Auto5",                            //11
    "Auto6",                            //12
    "Fix1",                             //13
    "Fix2",                             //14
    "Fix3",                             //15
    "Fix4",                             //16
    "Fix5",                             //17
    "Fix6",                             //18
    "Auto Clean",                       //19
    "OCR",                              //20
    "Input Rotate",                     //21
    "Output Rotate",                    //22
    "Top View",                         //23 //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
    "PAD View",                         //24 //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
    "BGA View",                         //25 //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
    "Loader Row B",                     //26 //Steven 20140811 : For 32 Site Loader and Shuttle Offset
    "In Shuttle 1 Left B",              //27 //Steven 20140811 : For 32 Site Loader and Shuttle Offset
    "In Shuttle 1 Right A",             //28 //Steven 20140811 : For 32 Site Loader and Shuttle Offset
    "In Shuttle 1 Right B",             //29 //Steven 20140811 : For 32 Site Loader and Shuttle Offset
    "In Shuttle 2 Left B",              //30 //Steven 20140811 : For 32 Site Loader and Shuttle Offset
    "In Shuttle 2 Right A",             //31 //Steven 20140811 : For 32 Site Loader and Shuttle Offset
    "In Shuttle 2 Right B",             //32 //Steven 20140811 : For 32 Site Loader and Shuttle Offset
    "Input Shuttle1 Auto Clean",        //33 //20140923 wei : For Shuttle Auto Clean
    "In Shuttle 1 Left B Auto Clean",   //34 //20140923 wei : For Shuttle Auto Clean
    "In Shuttle 1 Right A Auto Clean",  //35 //20140923 wei : For Shuttle Auto Clean
    "In Shuttle 1 Right B Auto Clean",  //36 //20140923 wei : For Shuttle Auto Clean
    "Input Shuttle2 Auto Clean",        //37 //20140923 wei : For Shuttle Auto Clean
    "In Shuttle 2 Left B Auto Clean",   //38 //20140923 wei : For Shuttle Auto Clean
    "In Shuttle 2 Right A Auto Clean",  //39 //20140923 wei : For Shuttle Auto Clean
    "In Shuttle 2 Right B Auto Clean",  //40 //20140923 wei : For Shuttle Auto Clean
    "Auto Shuttle 1",                   //41 //wei 20160914 Auto Shuttle Sensor
    "Auto Shuttle 2",                   //42 //wei 20160914 Auto Shuttle Sensor
    "Preciser",                         //43 //Frank 20180410 (Steven) : InArm Preciser Station
    "Out Shuttle 1 Left B",             //44 //Steven 20190802 : For 32 Site Out Shuttle Offset
    "Out Shuttle 1 Right A",            //45 //Steven 20190802 : For 32 Site Out Shuttle Offset
    "Out Shuttle 1 Right B",            //46 //Steven 20190802 : For 32 Site Out Shuttle Offset
    "Out Shuttle 2 Left B",             //47 //Steven 20190802 : For 32 Site Out Shuttle Offset
    "Out Shuttle 2 Right A",            //48 //Steven 20190802 : For 32 Site Out Shuttle Offset
    "Out Shuttle 2 Right B",            //49 //Steven 20190802 : For 32 Site Out Shuttle Offset
    "Scan AOI",                         //50 //Ifor 20191229 : add
    "InArm Placement",                  //51 //JimmyChiu 20220908 add Pickup Error Placement
    "Bottom 2D",                        //52
};

AnsiString CapStrInput[InOfsTotal]=
{
    "Loader",                           //0
    "Hot Plate1",                       //1
    "Hot Plate2",                       //2
    "Input Shuttle1",                   //3
    "Input Shuttle2",                   //4
    "Auto Clean",                       //5
    "OCR",                              //6
    "Input Rotate",                     //7
    "Loader Row B",                     //8 //Steven 20140811 : For 32 Site Loader and Shuttle Offset
    "In Shuttle 1 Left B",              //9 //Steven 20140811 : For 32 Site Loader and Shuttle Offset
    "In Shuttle 1 Right A",             //10 //Steven 20140811 : For 32 Site Loader and Shuttle Offset
    "In Shuttle 1 Right B",             //11 //Steven 20140811 : For 32 Site Loader and Shuttle Offset
    "In Shuttle 2 Left B",              //12 //Steven 20140811 : For 32 Site Loader and Shuttle Offset
    "In Shuttle 2 Right A",             //13 //Steven 20140811 : For 32 Site Loader and Shuttle Offset
    "In Shuttle 2 Right B",             //14 //Steven 20140811 : For 32 Site Loader and Shuttle Offset
    "Input Shuttle1 Auto Clean",        //15 //20140923 wei : For Shuttle Auto Clean
    "In Shuttle 1 Left B Auto Clean",   //16 //20140923 wei : For Shuttle Auto Clean
    "In Shuttle 1 Right A Auto Clean",  //17 //20140923 wei : For Shuttle Auto Clean
    "In Shuttle 1 Right B Auto Clean",  //18 //20140923 wei : For Shuttle Auto Clean
    "Input Shuttle2 Auto Clean",        //19 //20140923 wei : For Shuttle Auto Clean
    "In Shuttle 2 Left B Auto Clean",   //20 //20140923 wei : For Shuttle Auto Clean
    "In Shuttle 2 Right A Auto Clean",  //21 //20140923 wei : For Shuttle Auto Clean
    "In Shuttle 2 Right B Auto Clean",  //22 //20140923 wei : For Shuttle Auto Clean
    "Auto Shuttle 1",                   //23 //wei 20160914 Auto Shuttle Sensor
    "Auto Shuttle 2",                   //24 //wei 20160914 Auto Shuttle Sensor
    "Preciser",                         //25 //Frank 20180410 (Steven) : InArm Preciser Station
    "InArm Placement",                  //26 //JimmyChiu 20220908 add Pickup Error Placement
    "Bottom 2D",                        //27
};

AnsiString CapStrOutput[OutOfsTotal]=
{
    "Output Shuttle1",                  //0
    "Output Shuttle2",                  //1
    "Auto1",                            //2
    "Auto2",                            //3
    "Auto3",                            //4
    "Auto4",                            //5
    "Auto5",                            //6
    "Auto6",                            //7
    "Fix1",                             //8
    "Fix2",                             //9
    "Fix3",                             //10
    "Fix4",                             //11
    "Fix5",                             //12
    "Fix6",                             //13
    "Output Rotate",                    //14
    "Top View",                         //15 //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
    "PAD View",                         //16 //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
    "BGA View",                         //17 //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
    "Out Shuttle 1 Left B",             //18 //Steven 20190802 : For 32 Site Out Shuttle Offset
    "Out Shuttle 1 Right A",            //19 //Steven 20190802 : For 32 Site Out Shuttle Offset
    "Out Shuttle 1 Right B",            //20 //Steven 20190802 : For 32 Site Out Shuttle Offset
    "Out Shuttle 2 Left B",             //21 //Steven 20190802 : For 32 Site Out Shuttle Offset
    "Out Shuttle 2 Right A",            //22 //Steven 20190802 : For 32 Site Out Shuttle Offset
    "Out Shuttle 2 Right B",            //23 //Steven 20190802 : For 32 Site Out Shuttle Offset
    "Scan AOI",                         //24 //Ifor 20191229 : add
    "SortShuttleRight"                  //25                                    //RogerYang 20250417 for HT9046AU add
};

AnsiString CapStrSortput[SortOfsTotal]=                                         //RogerYang 20250417 for HT9046AU add
{
    "SortShuttleLeft",                  //0
    "Auto4",                            //1
    "Auto5",                            //2
    "Auto6"                             //3
};

AnsiString SpecialOffSetName[trayOfsTotal]=
{
    "TrayArm && Loader Track",
    "TrayArm && Empty Track",
    "TrayArm && Color Track",
    "TrayArm && Auto1 Track",
    "TrayArm && Auto2 Track",
    "TrayArm && Auto3 Track",
    "TrayArm && Auto4 Track",
    "TrayArm && Auto5 Track",
    "TrayArm && Auto6 Track",
    "TestArm1 && Shuttle1",
    "TestArm2 && Shuttle2"
};
const int iSaveStander=1;
const int iSaveSpecial=2;
const int iSaveAll=3;
//JimmyChiu 20211020 : Auto alignment mode
//==>
//AnsiString asInArm[5]={"Loader", "Hot Plate1", "Hot Plate2", "Shuttle1", "Shuttle2"};
//AnsiString asOutArm[8]={"Out Shuttle1", "Out Shuttle2",
//                        "Auto1", "Auto2", "Auto3", "Auto4", "Auto5", "Auto6",
//                        "Fix1",  "Fix2",  "Fix3",  "Fix4",  "Fix5",  "Fix6"};
//<==
//JimmyChiu 20211020 : Auto alignment mode
//enum{tRotate_In  =15,
//     tRotate_Out =16};
//---------------------------------------------------------------------------
__fastcall TfOffSet::TfOffSet(TComponent* Owner)
    : TForm(Owner)
{
    PageControl1->ActivePageIndex   =1;                                         //Steven 20120706 : 調整畫面
    PageControl1->ActivePage        =tsIndexOffset;
//    tsInOutArmOffset->TabVisible    =false;                                     //Steven 20120706 : 調整畫面
    tsIndexOffset->TabVisible       =false;                                     //Steven 20120706 : 調整畫面
    tsArmOffset->TabVisible         =false;
//    tsPickerList->TabVisible        =false;
    PageControl1->ActivePageIndex   =0;
    PageControl1->ActivePage        =tsInOutArmOffset;

    iOffsetMap[OfsLoader                ]=InOfsLoader;
    iOffsetMap[OfsHP1                   ]=InOfsHP1;
    iOffsetMap[OfsHP2                   ]=InOfsHP2;
    iOffsetMap[OfsInSh1                 ]=InOfsInSh1;
    iOffsetMap[OfsInSh2                 ]=InOfsInSh2;
    iOffsetMap[OfsOutSh1                ]=OutOfsOutSh1;
    iOffsetMap[OfsOutSh2                ]=OutOfsOutSh2;
    iOffsetMap[OfsAuto1                 ]=OutOfsAuto1;
    iOffsetMap[OfsAuto2                 ]=OutOfsAuto2;
    iOffsetMap[OfsAuto3                 ]=OutOfsAuto3;
    iOffsetMap[OfsAuto4                 ]=OutOfsAuto4;
    iOffsetMap[OfsAuto5                 ]=OutOfsAuto5;
    iOffsetMap[OfsAuto6                 ]=OutOfsAuto6;
    iOffsetMap[OfsFix1                  ]=OutOfsFix1;
    iOffsetMap[OfsFix2                  ]=OutOfsFix2;
    iOffsetMap[OfsFix3                  ]=OutOfsFix3;
    iOffsetMap[OfsFix4                  ]=OutOfsFix4;
    iOffsetMap[OfsFix5                  ]=OutOfsFix5;
    iOffsetMap[OfsFix6                  ]=OutOfsFix6;
    iOffsetMap[OfsAutoClean             ]=InOfsAutoClean;
    iOffsetMap[OfsOCR                   ]=InOfsOCR;
    iOffsetMap[OfsRotate_In             ]=InOfsRotate_In;
    iOffsetMap[OfsRotate_Out            ]=OutOfsRotate_Out;
    iOffsetMap[OfsTopView               ]=OutOfsTopView_Out;
    iOffsetMap[OfsPADView               ]=OutOfsPADView_Out;
    iOffsetMap[OfsBGAView               ]=OutOfsBGA_Out;
    iOffsetMap[OfsLoaderB               ]=InOfsLoaderB;
    iOffsetMap[OfsInSh1LB               ]=InOfsInSh1LB;
    iOffsetMap[OfsInSh1RA               ]=InOfsInSh1RA;
    iOffsetMap[OfsInSh1RB               ]=InOfsInSh1RB;
    iOffsetMap[OfsInSh2LB               ]=InOfsInSh2LB;
    iOffsetMap[OfsInSh2RA               ]=InOfsInSh2RA;
    iOffsetMap[OfsInSh2RB               ]=InOfsInSh2RB;
    iOffsetMap[OfsInSh1_AutoClean       ]=InOfsInSh1_AutoClean;
    iOffsetMap[OfsInSh1LB_AutoClean     ]=InOfsInSh1LB_AutoClean;
    iOffsetMap[OfsInSh1RA_AutoClean     ]=InOfsInSh1RA_AutoClean;
    iOffsetMap[OfsInSh1RB_AutoClean     ]=InOfsInSh1RB_AutoClean;
    iOffsetMap[OfsInSh2_AutoClean       ]=InOfsInSh2_AutoClean;
    iOffsetMap[OfsInSh2LB_AutoClean     ]=InOfsInSh2LB_AutoClean;
    iOffsetMap[OfsInSh2RA_AutoClean     ]=InOfsInSh2RA_AutoClean;
    iOffsetMap[OfsInSh2RB_AutoClean     ]=InOfsInSh2RB_AutoClean;
    iOffsetMap[OfsAutoSh1               ]=InOfsAutoSh1;
    iOffsetMap[OfsAutoSh2               ]=InOfsAutoSh2;
    iOffsetMap[OfsPreciser              ]=InOfsPreciser;
    iOffsetMap[OfsOutSh1LB              ]=OutOfsOutSh1LB;
    iOffsetMap[OfsOutSh1RA              ]=OutOfsOutSh1RA;
    iOffsetMap[OfsOutSh1RB              ]=OutOfsOutSh1RB;
    iOffsetMap[OfsOutSh2LB              ]=OutOfsOutSh2LB;
    iOffsetMap[OfsOutSh2RA              ]=OutOfsOutSh2RA;
    iOffsetMap[OfsOutSh2RB              ]=OutOfsOutSh2RB;
    iOffsetMap[OfsScanAOI               ]=OutOfsScannerAOI;
    iOffsetMap[OfsInPlacement           ]=InOfsPlacement;
    iOffsetMap[OfsBottom2DID            ]=InOfsBottom2DID;

    TSpeedButton *tempSpPtr[]=
    {
        sbLoader,  sbHp1,  sbHp2, sbInSh1, sbInSh2,
        sbOutSh1,  sbOutSh2,
        sbAuto1, sbAuto2, sbAuto3, sbAuto4, sbAuto5, sbAuto6,                   //Steven 20230907 : For HT-9011UC
        sbFix1,  sbFix2,  sbFix3,  sbFix4,  sbFix5,  sbFix6,                    //Steven 20230907 : For HT-9011UC
        sbAutoClean, sbOCR,
        sbInRotate, sbOutRotate,
        sbTopView, sbPADView, sbBGAView,                                        //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
        sbLoaderB,                                                              //Steven 20140811 : For 32 Site Loader and Shuttle Offset
        sbInSh1LB, sbInSh1RA, sbInSh1RB,                                        //Steven 20140811 : For 32 Site Loader and Shuttle Offset
        sbInSh2LB, sbInSh2RA, sbInSh2RB,                                        //Steven 20140811 : For 32 Site Loader and Shuttle Offset
        sbInSh1_AutoClean, sbInSh1LB_AutoClean, sbInSh1RA_AutoClean, sbInSh1RB_AutoClean,         //20140923 wei : For Shuttle Auto Clean
        sbInSh2_AutoClean, sbInSh2LB_AutoClean, sbInSh2RA_AutoClean, sbInSh2RB_AutoClean,         //20140923 wei : For Shuttle Auto Clean
        sbAutoSh1,sbAutoSh2,                                                                      //wei 20160914 Auto Shuttle Sensor
        sbPreciser,                                                             //Frank 20180410 (Steven) : InArm Preciser Station
        sbOutSh1LB, sbOutSh1RA, sbOutSh1RB,                                     //Steven 20190802 : For 32 Site Out Shuttle Offset
        sbOutSh2LB, sbOutSh2RA, sbOutSh2RB,
        sbScanAOI,                                                              //Ifor 20191229 : add Scan AOI
        sbInPlacement,                                                          //JimmyChiu 20220908 add Pickup Error Placement
        btnBottom2D,
    };
    TSpeedButton *SpecOffSetPtr[]=
    {
        btnTrayOfsLoader, btnTrayOfsEmpty, btnTrayOfsColor,
        btnTrayOfsAuto1,  btnTrayOfsAuto2, btnTrayOfsAuto3,
        btnTrayOfsAuto4,  btnTrayOfsAuto5, btnTrayOfsAuto6,
        IndexOffSetBT1,   IndexOffSetBT2
    };
    TEdit *tempPickEdit[2][8]=
    {
        {EditPickA, EditPickC, EditPickE, EditPickG, EditPickI, EditPickK, EditPickM, EditPickO},
        {EditPickB, EditPickD, EditPickF, EditPickH, EditPickJ, EditPickL, EditPickN, EditPickP}
    };
    TEdit *tempRelsEdit[2][8]=
    {
        {EdtRelsA, EdtRelsC, EdtRelsE, EdtRelsG, EdtRelsI, EdtRelsK, EdtRelsM, EdtRelsO},
        {EdtRelsB, EdtRelsD, EdtRelsF, EdtRelsH, EdtRelsJ, EdtRelsL, EdtRelsN, EdtRelsP}
    };
    TLabel *tempPickLab[2][8]=
    {
        {LabPickA, LabPickC, LabPickE, LabPickG, LabPickI, LabPickK, LabPickM, LabPickO},
        {LabPickB, LabPickD, LabPickF, LabPickH, LabPickJ, LabPickL, LabPickN, LabPickP}
    };
    TLabel *tempRelsLab[2][8]=
    {
        {LabRelsA, LabRelsC, LabRelsE, LabRelsG, LabRelsI, LabRelsK, LabRelsM, LabRelsO},
        {LabRelsB, LabRelsD, LabRelsF, LabRelsH, LabRelsJ, LabRelsL, LabRelsN, LabRelsP}
    };

    TEdit *SingleOffsetTEditX[2][8]=                                            //Eastsun 20251224 : 吸嘴單獨offset功能UI陣列
    {                                                                           //Eastsun 20251224 : X編輯元件edit
        {EdtOffsetAX, EdtOffsetCX, EdtOffsetEX, EdtOffsetGX, EdtOffsetIX, EdtOffsetKX, EdtOffsetMX, EdtOffsetOX},
        {EdtOffsetBX, EdtOffsetDX, EdtOffsetFX, EdtOffsetHX, EdtOffsetJX, EdtOffsetLX, EdtOffsetNX, EdtOffsetPX}
    };
    TEdit *SingleOffsetTEditY[2][8]=                                            //Eastsun 20251224 : 吸嘴單獨offset功能UI陣列
    {                                                                           //Eastsun 20251224 : Y編輯元件edit
        {EdtOffsetAY, EdtOffsetCY, EdtOffsetEY, EdtOffsetGY, EdtOffsetIY, EdtOffsetKY, EdtOffsetMY, EdtOffsetOY},
        {EdtOffsetBY, EdtOffsetDY, EdtOffsetFY, EdtOffsetHY, EdtOffsetJY, EdtOffsetLY, EdtOffsetNY, EdtOffsetPY}
    };
    TLabel *SingleOffsetLabX[2][8]=                                                 //Eastsun 20251224 : 吸嘴單獨offset功能UI陣列
    {                                                                           //Eastsun 20251224 : X顯示元件label
        {LabOffsetAX, LabOffsetCX, LabOffsetEX, LabOffsetGX, LabOffsetIX, LabOffsetKX, LabOffsetMX, LabOffsetOX},
        {LabOffsetBX, LabOffsetDX, LabOffsetFX, LabOffsetHX, LabOffsetJX, LabOffsetLX, LabOffsetNX, LabOffsetPX}
    };
    TLabel *SingleOffsetLabY[2][8]=                                                 //Eastsun 20251224 : 吸嘴單獨offset功能UI陣列
    {                                                                           //Eastsun 20251224 : Y顯示元件label
        {LabOffsetAY, LabOffsetCY, LabOffsetEY, LabOffsetGY, LabOffsetIY, LabOffsetKY, LabOffsetMY, LabOffsetOY},
        {LabOffsetBY, LabOffsetDY, LabOffsetFY, LabOffsetHY, LabOffsetJY, LabOffsetLY, LabOffsetNY, LabOffsetPY}
    };

    for(int i=0; i<OfsTotal; i++)
    {
        OffSetSelBot[i]=tempSpPtr[i];
        OffSetSelBot[i]->Tag=i;
        OffSetSelBot[i]->OnClick=SpBotSelClick;
    }

    for(int i=0; i<2; i++)
    {
        for(int j=0; j<8; j++)
        {
            MyPickEdit[i][j]=tempPickEdit[i][j];
            MyPickEdit[i][j]->Visible=false;
            MyPickEdit[i][j]->Text=0.0;

            MyRelsEdit[i][j]=tempRelsEdit[i][j];
            MyRelsEdit[i][j]->Visible=false;
            MyRelsEdit[i][j]->Text=0.0;

            MyPickLab[i][j]=tempPickLab[i][j];
            MyPickLab[i][j]->Visible=false;

            MyRelsLab[i][j]=tempRelsLab[i][j];
            MyRelsLab[i][j]->Visible=false;

            MySingleOffsetTEditX[i][j]=SingleOffsetTEditX[i][j];
            MySingleOffsetTEditX[i][j]->Visible=false;
            MySingleOffsetTEditX[i][j]->Text=0.0;

            MySingleOffsetTEditY[i][j]=SingleOffsetTEditY[i][j];
            MySingleOffsetTEditY[i][j]->Visible=false;
            MySingleOffsetTEditY[i][j]->Text=0.0;

            MySingleOffsetLabX[i][j]=SingleOffsetLabX[i][j];
            MySingleOffsetLabX[i][j]->Visible=false;

            MySingleOffsetLabY[i][j]=SingleOffsetLabY[i][j];
            MySingleOffsetLabY[i][j]->Visible=false;
        }
    }

    for(int i=0; i<trayOfsTotal; i++)
    {
        OfffSetSpecSelBot[i]=SpecOffSetPtr[i];
        OfffSetSpecSelBot[i]->Tag=i;
        OfffSetSpecSelBot[i]->OnClick=IndexOffSetBT2Click;
    }

    iNowOffsetSel       =-1;
    iSpecialOffSetSel   =-1;
    LastFileName        ="";
    edArmX->Text        =0.0;
    edArmY->Text        =0.0;
    edPickUp->Text      =0.0;
    edRelease->Text     =0.0;
    edPitchX1->Text     =0.0;

    edPitchY->Text      =0.0;                                                   //ChungHung 20131231 alter AutoYPitch
    edPitchX2->Text     =0.0;                                                   //ChungHung 20131231 alter AutoYPitch
    bEnterSpecialOffset =false;

    if(CUSTOMER_CODE==CC_ASE_KaohSiung ||                                       //jou 2010-08-03 offset tray arm 調整 offset,loader處隱藏,只保留Tray set.
       CUSTOMER_CODE==CC_ASE_KaohSiung_K12)                                     //Steven 20131101 : Add ASE-K12
        btnTrayOfsLoader->Visible=false;

    Image2->Picture->Bitmap->TransparentColor = Image2->Canvas->Pixels[0][0];

    sgOffsetList->ColCount=oiArmTotal;
    sgOffsetList->RowCount=OfsTotal+1;
    sgOffsetList->DefaultColWidth=80;
    sgOffsetList->ColWidths[0]=200;
    for(int i=0; i<OfsTotal; i++)
    {
        sgOffsetList->Cells[0][i+1]=AnsiString(" ")+CapStr[i];
    }
    sgOffsetList->Cells[oiArmX     ][0]=" X";
    sgOffsetList->Cells[oiArmY     ][0]=" Y";
    sgOffsetList->Cells[oiArmPitch1][0]=" X Pitch 1";
    sgOffsetList->Cells[oiArmPitch2][0]=" X Pitch 2";
    sgOffsetList->Cells[oiArmPitch3][0]=" X Pitch 3";
    sgOffsetList->Cells[oiArmPitch4][0]=" X Pitch 4";
    sgOffsetList->Cells[oiArmPitchY][0]=" Y Pitch";
    sgOffsetList->Cells[oiArmPick  ][0]=" Pick";
    sgOffsetList->Cells[oiArmPlace ][0]=" Place";

    sgOffsetList->Cells[oiArmPickA_X ][0]=" A_X";                               //Eastsun 20251218 : 每支吸嘴獨立偏移XY
    sgOffsetList->Cells[oiArmPickA_Y ][0]=" A_Y";
    sgOffsetList->Cells[oiArmPickB_X ][0]=" B_X";
    sgOffsetList->Cells[oiArmPickB_Y ][0]=" B_Y";
    sgOffsetList->Cells[oiArmPickC_X ][0]=" C_X";
    sgOffsetList->Cells[oiArmPickC_Y ][0]=" C_Y";
    sgOffsetList->Cells[oiArmPickD_X ][0]=" D_X";
    sgOffsetList->Cells[oiArmPickD_Y ][0]=" D_Y";
    sgOffsetList->Cells[oiArmPickE_X ][0]=" E_X";
    sgOffsetList->Cells[oiArmPickE_Y ][0]=" E_Y";
    sgOffsetList->Cells[oiArmPickF_X ][0]=" F_X";
    sgOffsetList->Cells[oiArmPickF_Y ][0]=" F_Y";
    sgOffsetList->Cells[oiArmPickG_X ][0]=" G_X";
    sgOffsetList->Cells[oiArmPickG_Y ][0]=" G_Y";
    sgOffsetList->Cells[oiArmPickH_X ][0]=" H_X";
    sgOffsetList->Cells[oiArmPickH_Y ][0]=" H_Y";

    btnSortAuto1->Tag=eAuto1;
    btnSortAuto2->Tag=eAuto2;
    btnSortAuto3->Tag=eAuto3;
    btnSortFix1->Tag =eFix1;
    btnSortFix2->Tag =eFix2;
    btnSortFix3->Tag =eFix3;

    if(USE_Scanner_AOI_Inspection==(int)eBtnAOI_TopBottomInstall)                    //Jimmychiu 20240322 : Top & Bottom Inspect
    {
        sbScanAOI->Visible=true;
        CapStr[OfsScanAOI]="Top Bottom AOI";
        CapStrOutput[OutOfsScannerAOI]="Top Bottom AOI";
    }

    LoadImage();
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::LoadImage(bool bInArm)
{
    AnsiString asString;                                                        //jou 2012-08-22 將大型圖片用LoadFormFile節省compiler時間 & 縮小執行檔大小

    if(USE_OUT_SORT_ARM!=eartUninstall)                                         //Steven 20240822 : For HT-9046AU
        asString=BmpPath+"InOutArmOffset_AU.bmp";
    else if(AUTO_EMPTY_COLOR>=3)                                                //Steven 20230907 : For HT-9011UC
        asString=BmpPath+"InOutArmOffset_6.bmp";
    else
        asString=BmpPath+"InOutArmOffset.BMP";

    if(FileExists(asString))
    {
        Image1->Picture->LoadFromFile(asString);
        Image3->Picture->LoadFromFile(asString);
    }

    SetXYPitchVCLVisible(USE_IN_Y_IS_AUTO_PITCH==true);                               //kevin 20151105  : XY變距  //JerryYang 20251218 : IN/OUT ARM支援不同模組
    if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)                                //Steven 20141029 : XY-Pitch
    {
        if(bInArm)
            asString=BmpPath+"SuckBaseF.bmp";
        else
            asString=BmpPath+"SuckBaseD.bmp";
    }
    else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker)
    {
        asString=BmpPath+"SuckBase1032_Bd.bmp";
    }
    else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                            //Ztex 2023.12.06 Add HT-1032
    {
        if(bInArm)
            asString=BmpPath+"SuckBase1032_Bd.bmp";
        else
            asString=BmpPath+"SuckBase1032_Be.bmp";
    }
    else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                       //Ztex 2024.02.24 Add HT-1132
    {
        if(bInArm)
            asString=BmpPath+"SuckBaseD.bmp";
        else
            asString=BmpPath+"SuckBaseF.bmp";
    }
    else
    {
        asString=BmpPath+"SuckUnit1.BMP";
    }

    if(FileExists(asString))
        Image2->Picture->LoadFromFile(asString);

    SetXYPitchVCLVisible(USE_IN_Y_IS_AUTO_PITCH==true);                               //kevin 20151105 add  Steven 20140510 : XY變距
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::FormShow(TObject *Sender)
{
    fShow=true;
    AnsiString S;
    GetLimitAuth();
    LastFileName=GetLastOpenFN();
    ReadFile();
    S.sprintf("Position Offset  ''%s''  ",LastFileName);
    Caption=S;

    bEnterOffset=true;
    bUseUpdate=false;
    Left=10;
    Top=10;

    if(iNowOffsetSel!=-1)
        DoIniDataToForm(iNowOffsetSel, iSaveStander);
    if(iSpecialOffSetSel!=-1)
        DoIniDataToForm(iSpecialOffSetSel, iSaveSpecial);

    S.sprintf("%d~%dmm", InputLimit.iOffsetXYHigh, InputLimit.iOffsetXYLow);    //Steven 20140909 : 修正Offset限制的顯示
    lblArmXRange    ->Caption=S;
    lblArmYRange    ->Caption=S;
    lblPitchX1Range ->Caption=S;
    lblPitchX2Range ->Caption=S;
    lblPitchYRange  ->Caption=S;

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //kevin 20211111 add 1 ~ -1 abel for K7
    {
        S.sprintf("%d~%dmm", 1, -1);
        lblReleaseRange->Caption=S;
        S.sprintf("%d~%dmm", InputLimit.iOffsetZHigh, InputLimit.iOffsetZLow);  //Steven 20140909 : 修正Offset限制的顯示
        lblPickUpRange->Caption=S;
    }
    else
    {
        S.sprintf("%d~%dmm", InputLimit.iOffsetZHigh, InputLimit.iOffsetZLow);  //Steven 20140909 : 修正Offset限制的顯示
        lblReleaseRange->Caption=S;
        lblPickUpRange->Caption=S;
    }

    S.sprintf("Shuttle Right Offset (%2.f<->%2.f)", InputLimit.dShuttleHigh, InputLimit.dShuttleLow);       //ChungHung 20150115 add for ATK +/-2 mm
    lblShtRight->Caption=S;
    S.sprintf("Shuttle Left Offset (%2.f<->%2.f)", InputLimit.dShuttleHigh, InputLimit.dShuttleLow);
    lblShtLeft->Caption=S;

    sbAutoClean->Visible=(IniConfig.bEnableAutoCleanFunction && IniConfig.bE43AutoCleanUseHotplate==false); //Steven 20230417 : 使用E43加熱盤不顯示AutoClean
    sbInRotate->Visible =((USE_ROTATE_KIT || USE_DIE_CLEAN==1)&& iRotate_Type!=eInOutArm1Motor);                  //jou 2014-05-29 修正offset畫面顯示錯誤
    sbOutRotate->Visible=(USE_ROTATE_KIT && iRotate_Type!=eInOutArm1Motor);                                        //jou 2014-05-29 修正offset畫面顯示錯誤
    sbOCR->Visible      =TestIF.bOcrFunction;

    chkInArmScakeEnable->Checked=TestIF_File.bInArmUseDifferentScaleBySetupFile;

    edLodX->Text=TestIF_File.fLoaderTrayXScaleBySetupFile;
    edLodY->Text=TestIF_File.fLoaderTrayYScaleBySetupFile;
    edt_HP1X->Text=TestIF_File.fHotPlateXScaleBySetupFile[0];
    edt_HP1Y->Text=TestIF_File.fHotPlateYScaleBySetupFile[0];
    edt_HP2X->Text=TestIF_File.fHotPlateXScaleBySetupFile[1];
    edt_HP2Y->Text=TestIF_File.fHotPlateYScaleBySetupFile[1];

    ed_IS1X->Text=TestIF_File.fInShuttleXScaleBySetupFile[0];
    ed_IS1Y->Text=TestIF_File.fInShuttleYScaleBySetupFile[0];
    ed_IS2X->Text=TestIF_File.fInShuttleXScaleBySetupFile[1];
    ed_IS2Y->Text=TestIF_File.fInShuttleYScaleBySetupFile[1];

    chkOutArmScakeEnable->Checked=TestIF_File.bOutArmUseDifferentScaleBySetupFile;

    ed_OS1X->Text=TestIF_File.fOutShuttleXScaleBySetupFile[0];
    ed_OS1Y->Text=TestIF_File.fOutShuttleYScaleBySetupFile[0];
    ed_OS2X->Text=TestIF_File.fOutShuttleXScaleBySetupFile[1];
    ed_OS2Y->Text=TestIF_File.fOutShuttleYScaleBySetupFile[1];

    lblShowMessage->Caption         ="";
    labWarningForStop->Visible      =false;                                     //jou 2011-02-18
    btnBackClick(this);

    pan_AutoOffsetMove->Visible     =IniConfig.bUseAutoOffsetFunction;          //jou 2013-08-29 Use Auto Offset Funtion
    palPreciser->Visible            =(TestIF_File.bEnableUsePreciser ||         //JerryYang 20180820 add
                                      TestIF_File.bEnableRTPreciser);
    if(AccessLevel<iDefHonPrecLevel)
    {
        tsScale->Enabled=false;
    }
    else
    {
        tsScale->Enabled=true;
    }

    pnlPicker->Enabled      =fSecurity->Insufficient(2, false) && authMainForm[2];  //jou 981207 權限控制
    btnOffsetList->Enabled  =fSecurity->Insufficient(2, false) && authMainForm[2];
//    btnPickerList->Enabled  =fSecurity->Insufficient(2, false) && authMainForm[2];
    if(fBarCode->bShow && CUSTOMER_CODE==CC_KYEC_XILINX)                            //Frank 20171030 (Steven) add BarCode調整Offset
        pnlIndexOfs->Enabled=true;
    else
        pnlIndexOfs->Enabled=fSecurity->Insufficient(2,false) && authMainForm[2];   //jou 2014-06-12

    grpOutArm456->Visible=(AUTO_EMPTY_COLOR>=3);
    palAuto6    ->Visible=(AUTO_EMPTY_COLOR>=4);
    btnSortAuto6->Visible=(AUTO_EMPTY_COLOR>=4);

    //Steven 20160630 : add USE_IN_OUT_ARM_Y_PITCH!=iXYPitchVariable
    sbLoaderB->Visible=TestIF_File.bEnableAutoAlignment==false && (IniConfig.bE46_LoaderUse2Offset && (USE_IN_Y_IS_AUTO_PITCH==false || TestIF.dSiteYPitch!=TestIF_File.iARM_Y_PITCH) && (TestIF.iTestMode==_32Site4X8N || TestIF_File.iTestMode>=QualSite2X2));  //JerryYang 20251218 : IN/OUT ARM支援不同模組
    sbInSh1LB->Visible=TestIF_File.bEnableAutoAlignment==false && (USE_IN_Y_IS_AUTO_PITCH==false && (TestIF.dSiteYPitch!=TestIF_File.iARM_Y_PITCH && (TestIF.iTestMode==_32Site4X8N || (IniConfig.bE47_ShuttleUse4Offset && TestIF_File.iTestMode>=QualSite2X2))));
    sbInSh1RA->Visible=TestIF_File.bEnableAutoAlignment==false && (TestIF.iTestMode==_32Site4X8N || (IniConfig.bE47_ShuttleUse4Offset && TestIF_File.iTestMode>=_10Site2X5));
    sbInSh1RB->Visible=TestIF_File.bEnableAutoAlignment==false && (USE_IN_Y_IS_AUTO_PITCH==false && (TestIF.dSiteYPitch!=TestIF_File.iARM_Y_PITCH && (TestIF.iTestMode==_32Site4X8N || (IniConfig.bE47_ShuttleUse4Offset && TestIF_File.iTestMode>=_10Site2X5))));
    sbInSh2LB->Visible=TestIF_File.bEnableAutoAlignment==false && (USE_IN_Y_IS_AUTO_PITCH==false && (TestIF.dSiteYPitch!=TestIF_File.iARM_Y_PITCH && (TestIF.iTestMode==_32Site4X8N || (IniConfig.bE47_ShuttleUse4Offset && TestIF_File.iTestMode>=QualSite2X2))));
    sbInSh2RA->Visible=TestIF_File.bEnableAutoAlignment==false && (TestIF.iTestMode==_32Site4X8N || (IniConfig.bE47_ShuttleUse4Offset && TestIF_File.iTestMode>=_10Site2X5));  //Steven 20140811 : For 32 Site Loader and Shuttle Offset
    sbInSh2RB->Visible=TestIF_File.bEnableAutoAlignment==false && (USE_IN_Y_IS_AUTO_PITCH==false && (TestIF.dSiteYPitch!=TestIF_File.iARM_Y_PITCH && (TestIF.iTestMode==_32Site4X8N || (IniConfig.bE47_ShuttleUse4Offset && TestIF_File.iTestMode>=_10Site2X5))));

    //Steven 20190802 : For 32 Site Out Shuttle Offset
    sbOutSh1LB->Visible=TestIF_File.bEnableAutoAlignment==false &&(USE_OUT_Y_IS_AUTO_PITCH==false && (TestIF.dSiteYPitch!=TestIF_File.iARM_Y_PITCH && (TestIF.iTestMode==_32Site4X8N || (IniConfig.bE47_ShuttleUse4Offset && TestIF_File.iTestMode>=QualSite2X2))));
    sbOutSh1RA->Visible=TestIF_File.bEnableAutoAlignment==false &&(TestIF.iTestMode==_32Site4X8N || (IniConfig.bE47_ShuttleUse4Offset && TestIF_File.iTestMode>=_12Site2X6));
    sbOutSh1RB->Visible=TestIF_File.bEnableAutoAlignment==false &&(USE_OUT_Y_IS_AUTO_PITCH==false && (TestIF.dSiteYPitch!=TestIF_File.iARM_Y_PITCH && (TestIF.iTestMode==_32Site4X8N || (IniConfig.bE47_ShuttleUse4Offset && TestIF_File.iTestMode>=_10Site2X5))));
    sbOutSh2LB->Visible=TestIF_File.bEnableAutoAlignment==false &&(USE_OUT_Y_IS_AUTO_PITCH==false && (TestIF.dSiteYPitch!=TestIF_File.iARM_Y_PITCH && (TestIF.iTestMode==_32Site4X8N || (IniConfig.bE47_ShuttleUse4Offset && TestIF_File.iTestMode>=QualSite2X2))));
    sbOutSh2RA->Visible=TestIF_File.bEnableAutoAlignment==false &&(TestIF.iTestMode==_32Site4X8N || (IniConfig.bE47_ShuttleUse4Offset && TestIF_File.iTestMode>=_12Site2X6));
    sbOutSh2RB->Visible=TestIF_File.bEnableAutoAlignment==false &&(USE_OUT_Y_IS_AUTO_PITCH==false && (TestIF.dSiteYPitch!=TestIF_File.iARM_Y_PITCH && (TestIF.iTestMode==_32Site4X8N || (IniConfig.bE47_ShuttleUse4Offset && TestIF_File.iTestMode>=_10Site2X5))));

    sbInSh1LB_AutoClean->Visible=TestIF_File.bEnableAutoAlignment==false && (USE_IN_Y_IS_AUTO_PITCH==false && (TestIF.iTestMode==_32Site4X8N || (IniConfig.bE48_ShuttleUse4Offset_Autoclean && TestIF_File.iTestMode>=QualSite2X2)));     //20140923 wei : For Shuttle Auto Clean
    sbInSh1RA_AutoClean->Visible=TestIF_File.bEnableAutoAlignment==false && (TestIF.iTestMode==_32Site4X8N || (IniConfig.bE48_ShuttleUse4Offset_Autoclean && TestIF_File.iTestMode>=_10Site2X5));                                                    //20140923 wei : For Shuttle Auto Clean
    sbInSh1RB_AutoClean->Visible=TestIF_File.bEnableAutoAlignment==false && (USE_IN_Y_IS_AUTO_PITCH==false && (TestIF.iTestMode==_32Site4X8N || (IniConfig.bE48_ShuttleUse4Offset_Autoclean && TestIF_File.iTestMode>=_10Site2X5)));      //20140923 wei : For Shuttle Auto Clean
    sbInSh2LB_AutoClean->Visible=TestIF_File.bEnableAutoAlignment==false && (USE_IN_Y_IS_AUTO_PITCH==false && (TestIF.iTestMode==_32Site4X8N || (IniConfig.bE48_ShuttleUse4Offset_Autoclean && TestIF_File.iTestMode>=QualSite2X2)));     //20140923 wei : For Shuttle Auto Clean
    sbInSh2RA_AutoClean->Visible=TestIF_File.bEnableAutoAlignment==false && (TestIF.iTestMode==_32Site4X8N || (IniConfig.bE48_ShuttleUse4Offset_Autoclean && TestIF_File.iTestMode>=_10Site2X5));                                                    //20140923 wei : For Shuttle Auto Clean
    sbInSh2RB_AutoClean->Visible=TestIF_File.bEnableAutoAlignment==false && (USE_IN_Y_IS_AUTO_PITCH==false && (TestIF.iTestMode==_32Site4X8N || (IniConfig.bE48_ShuttleUse4Offset_Autoclean && TestIF_File.iTestMode>=_10Site2X5)));      //20140923 wei : For Shuttle Auto Clean

    sbAutoSh1->Visible    =(AUTO_SENSOR_INSTALL==1);                            //wei 20160914 Auto Shuttle Sensor
    sbAutoSh2->Visible    =(AUTO_SENSOR_INSTALL==1);                            //wei 20160914 Auto Shuttle Sensor

    Pnl_AOI->Visible      =(USE_AOI_Inspection==1 || USE_Fix_AI_CCD==1 || (USE_Top_Scanner_AOI_Inspection==1 && ScannerAOIIF.iEnableTopScannerMode!=0));   //RogerYang 20180901 add 矽格湖口Demo AI CCD Function     //Steven 20140909 : AOI沒用到不要出現//Ifor 20200902 add: TFAMD Top AOI
    sbBGAView->Visible    =Pnl_AOI->Visible;
    Pnl_ScanAOI->Visible  =(USE_Scanner_AOI_Inspection==(int)eBtnAOI_BottomInstall && ScannerAOIIF.iEnableScannerMode!=0) || (FrmAOI->RunTopBottomInspect()==true); //Ifor 20191229   //Jimmychiu 20240322 : Top & Bottom Inspect

    sbScanAOI->Visible    =Pnl_ScanAOI->Visible;
    sbPreciser->Visible   =(USE_PRECISER);                                      //Frank 20180410 (Steven) : InArm Preciser Station
    grpAutoClean->Visible =(TestIF_File.iAutoClean_Function && IniConfig.bE48_ShuttleUse4Offset_Autoclean) ;        //20140923 wei : For Shuttle Auto Clean
    if(grpAutoClean->Visible==false)
    {
        sbInSh1_AutoClean->Visible=false;
        sbInSh2_AutoClean->Visible=false;
        sbInSh1LB_AutoClean->Visible=false;
        sbInSh1RA_AutoClean->Visible=false;
        sbInSh1RB_AutoClean->Visible=false;
        sbInSh2LB_AutoClean->Visible=false;
        sbInSh2RA_AutoClean->Visible=false;
        sbInSh2RB_AutoClean->Visible=false;
    }
    else
    {
        sbInSh1_AutoClean->Visible=true;
        sbInSh2_AutoClean->Visible=true;
    }

    sbInPlacement->Visible=(fMain->cInplace->InArmPlacementEnable());           //JimmyChiu 20220908 add Pickup Error Placement
    sbInPlacement->Visible=(BOTTOM_2DID>0);                                     //jerryYang 20230407 add 顯示

    if(USE_AUTO_RETEST==eartInstall &&
       (IniConfig.bA10_AutoReTest || bAutoReTest_ART) &&                        //kevin 20170831 (Steven) add ART tray arm offset
       (LastSet.iRunStartMode==rsmInitial_ART       ||
        LastSet.iRunStartMode==rsmContinuStart_ART  ||
        LastSet.iRunStartMode==rsmContinuRetest_ART ||
        LastSet.iRunStartMode==rsmAutoRetest))
    {
        btnTrayOfsLoader->Visible       =true;
//        pnlTrayXART->Visible            =true;
    }
    else
    {
//        pnlTrayXART->Visible            =false;
        if(CUSTOMER_CODE==CC_ASE_KaohSiung ||
           CUSTOMER_CODE==CC_ASE_KaohSiung_K12)                                 //Steven 20131101 : Add ASE-K12
        {
            btnTrayOfsLoader->Visible   =false;
        }
    }

//    fOffSet->Width          =(AUTO_EMPTY_COLOR>=3)?1300:1100;                   //Steven 20230907 : For HT-9011UC
    pnlMain->Width          =(AUTO_EMPTY_COLOR>=3)?751:571;
    pnlPicker->Width        =(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||
                              USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)?976:487; //Ztex 2023.12.06 Add HT-1032
    sbAuto4->Visible        =(AUTO_EMPTY_COLOR>=3);
    sbAuto5->Visible        =(AUTO_EMPTY_COLOR>=3);
    sbAuto6->Visible        =(AUTO_EMPTY_COLOR>=4);
    sbFix4->Visible         =(AUTO_EMPTY_COLOR>=3);
    sbFix5->Visible         =(AUTO_EMPTY_COLOR>=3);
    sbFix6->Visible         =(AUTO_EMPTY_COLOR>=3);
    btnTrayOfsAuto4->Visible=(AUTO_EMPTY_COLOR>=3);
    btnTrayOfsAuto5->Visible=(AUTO_EMPTY_COLOR>=3);
    btnTrayOfsAuto6->Visible=(AUTO_EMPTY_COLOR>=4);
    fOffSet->Width          =pnlMain->Width+pnlPicker->Width+5;

    if(USE_OUT_SORT_ARM!=eartUninstall)                                         //Steven 20240822 : For HT-9046AU
    {
        sbAuto1->Left=548;
        sbAuto2->Left=608;
        sbAuto3->Left=668;
        sbAuto4->Left=308;                                                      //RogerYang 20250505 Fix Position
        sbAuto5->Left=368;
        sbAuto6->Left=428;
        sbLoader->Left=188;
    }

    LoadImage();                                                                //Steven 20141029 : XY-Pitch
    grpSetupTeach->Visible=false;                                               //JerryYang 20180921 Setup Teach功能
    tsSetupTeach->TabVisible=false;
    tsZCalibration->TabVisible=false;
    tsScale->TabVisible=false;
    ts_Offset_HotTempShiftOffset->TabVisible=false;
    sbZcalibration->Visible=false;

    if(CUSTOMER_CODE==CC_ASE_CL)
    {
        tsZCalibration->TabVisible=true;
        tsScale->TabVisible=true;
        ts_Offset_HotTempShiftOffset->TabVisible=true;
        sbZcalibration->Visible=true;
    }

    if(IniConfig.bA30SetupTeachFunction)
    {
        if(LastSet.iTester==OFF_LINE)
        {
            TimerSetupTeach->Enabled=true;
            grpSetupTeach->Visible  =true;
            tsSetupTeach->TabVisible=true;
//            fOffSet->Width          =(AUTO_EMPTY_COLOR>=3)?1450:1262;
        }
    }

    if(CosFunction.bOEEFunction && fProductionInfo!=NULL)
    {
        pnlOEE_Contact->Visible     =CosFunction.bOffsetContactForce && IniConfig.bD06ContactOffsetDefaultValue && fProductionInfo->CheckContactForceExist();      //JimmyChiu 20220114 : Index 總壓力 Offset，加總後數值不顯示於Contact Form
        edOffsetContactForce->Text  =FloatToStr(fProductionInfo->GetOffsetContactForce());
        ShowFinalAirForce();                                                    //JimmyChiu 20220114 : Index 總壓力 Offset，加總後數值不顯示於Contact Form
    }
    else
    {
        pnlOEE_Contact->Visible     =false;
    }

    iIndexChange            =0;                                                 //kevin 20211211 index socket value change
    if(USE_Top_Scanner_AOI_Inspection==1)                                       //Ifor 20200902 add: TFAMD Top AOI
    {
        sbBGAView->Visible  =false;
        sbPADView->Visible  =false;
        sbTopView->Visible  =true;
        ck_AOI->Caption     ="Top AOI";
        sbTopView->Top      =32;
    }
    else
    {
        sbBGAView->Visible  =false;
        sbPADView->Visible  =false;
        sbTopView->Visible  =false;
    }

    if(CUSTOMER_CODE==CC_ASE_CL)
    {
        if(InArmSuck.HasIC()==true || OutArmSuck.HasIC()==true ||
           ShuttleHasIC()==true || IndexHasIC()==true || LastSet.iRealDummy!=REALLY)
        {
            sbInArmZCalibration->Visible=false;
            sbOutArmZCalibration->Visible=false;
        }
        else
        {
            sbInArmZCalibration->Visible=true;
            sbOutArmZCalibration->Visible=true;
        }
        sbInArmZCalibration->Enabled=true;
        sbOutArmZCalibration->Enabled=true;
    }
    else
    {
        sbInArmZCalibration->Visible=false;
        sbOutArmZCalibration->Visible=false;
    }

    bDoInZTeach=false;
    bDoOutZTeach=false;

    //Eastsun 20260710 Merge - B03 add: KYEC LEE offset enter barcode
    if(CUSTOMER_CODE==CC_KYEC_LEE && USE_BU5_Function==true)                    //Frank 20260710 ADD
        bOffsetEnterBarcode=true;

    Timer1->Enabled=true;                                                       //Steven 20220120 : 機台在動的時候,不可以調整Shuttle Offset,避免Hang up

    if(CUSTOMER_CODE==CC_ASE_CL)
    {
        sg_Offset_HotTempShiftOffset->ColWidths[0]  = 100;
        sg_Offset_HotTempShiftOffset->Cells[0][0]   = "Position";
        sg_Offset_HotTempShiftOffset->Cells[0][1]   = "LoaderX";
        sg_Offset_HotTempShiftOffset->Cells[0][2]   = "LoaderY";
        sg_Offset_HotTempShiftOffset->Cells[0][3]   = "HotPlate1X";
        sg_Offset_HotTempShiftOffset->Cells[0][4]   = "HotPlate1Y";
        sg_Offset_HotTempShiftOffset->Cells[0][5]   = "HotPlate2X";
        sg_Offset_HotTempShiftOffset->Cells[0][6]   = "HotPlate2Y";
        sg_Offset_HotTempShiftOffset->Cells[0][7]   = "InShuttle1X";
        sg_Offset_HotTempShiftOffset->Cells[0][8]   = "InShuttle1Y";
        sg_Offset_HotTempShiftOffset->Cells[0][9]   = "InShuttle2X";
        sg_Offset_HotTempShiftOffset->Cells[0][10]  = "InShuttle2Y";
        sg_Offset_HotTempShiftOffset->Cells[0][11]  = "OutShuttle1X";
        sg_Offset_HotTempShiftOffset->Cells[0][12]  = "OutShuttle1Y";
        sg_Offset_HotTempShiftOffset->Cells[0][13]  = "OutShuttle2X";
        sg_Offset_HotTempShiftOffset->Cells[0][14]  = "OutShuttle2Y";
        sg_Offset_HotTempShiftOffset->Cells[0][15]  = "Index1ToShuttleRight";
        sg_Offset_HotTempShiftOffset->Cells[0][16]  = "Index1ToShuttleLeft";
        sg_Offset_HotTempShiftOffset->Cells[0][17]  = "Index2ToShuttleRight";
        sg_Offset_HotTempShiftOffset->Cells[0][18]  = "Index2ToShuttleLeft";
        sg_Offset_HotTempShiftOffset->Cells[0][20]  = "LoaderXScale";
        sg_Offset_HotTempShiftOffset->Cells[0][21]  = "LoaderYScale";
        sg_Offset_HotTempShiftOffset->Cells[0][22]  = "HotPlate1XScale";
        sg_Offset_HotTempShiftOffset->Cells[0][23]  = "HotPlate1YScale";
        sg_Offset_HotTempShiftOffset->Cells[0][24]  = "HotPlate2XScale";
        sg_Offset_HotTempShiftOffset->Cells[0][25]  = "HotPlate2YScale";
        sg_Offset_HotTempShiftOffset->Cells[0][27]  = "Auto1XOffset";
        sg_Offset_HotTempShiftOffset->Cells[0][28]  = "Auto1YOffset";
        sg_Offset_HotTempShiftOffset->Cells[0][29]  = "Auto2XOffset";
        sg_Offset_HotTempShiftOffset->Cells[0][30]  = "Auto2YOffset";
        sg_Offset_HotTempShiftOffset->Cells[0][31]  = "Auto3XOffset";
        sg_Offset_HotTempShiftOffset->Cells[0][32]  = "Auto3YOffset";
        sg_Offset_HotTempShiftOffset->Cells[0][33]  = "Fix1XOffset";
        sg_Offset_HotTempShiftOffset->Cells[0][34]  = "Fix1YOffset";
        sg_Offset_HotTempShiftOffset->Cells[0][35]  = "Fix2XOffset";
        sg_Offset_HotTempShiftOffset->Cells[0][36]  = "Fix2YOffset";
        sg_Offset_HotTempShiftOffset->Cells[0][37]  = "Fix3XOffset";
        sg_Offset_HotTempShiftOffset->Cells[0][38]  = "Fix3YOffset";
        sg_Offset_HotTempShiftOffset->Cells[1][0]   = "Low Temp";
        sg_Offset_HotTempShiftOffset->Cells[2][0]   = "Mid Temp";
        sg_Offset_HotTempShiftOffset->Cells[3][0]   = "High Temp";
        sg_Offset_HotTempShiftOffset->Cells[4][0]   = "LM Ratio";
        sg_Offset_HotTempShiftOffset->Cells[5][0]   = "MH Ratio";
        sg_Offset_HotTempShiftOffset->Cells[6][0]   = "Current Temp";

        fOffSet->ReadHotTempShiftOffsetData();
    }

    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);                                  //Steven 20100629
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::FormClose(TObject *Sender, TCloseAction &Action)
{
    fShow=false;
    cb_AutoOffsetPositionCheck->Checked=false;
    bOffsetEnterBarcode=false;
    ReadFile();

    Timer1->Enabled=false;                                                      //Steven 20220120 : 機台在動的時候,不可以調整Shuttle Offset,避免Hang up
    TimerSetupTeach->Enabled=false;                                             //JerryYang 20180921 Setup Teach功能

    if(bDoInZTeach==true || bDoOutZTeach==true)                                 //JerryYang 20250120 : modify
    {
        bDoInZTeach=false;
        bDoOutZTeach=false;

        fAllMotorHome=false;
        iHome=1;

        NewRecordProcess("", "ZCalibration not finish, do homing!", "OffSet->FormClose");
    }

    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);                                  //Steven 20100629
}
//---------------------------------------------------------------------------
void TfOffSet::SetXYPitchVCLVisible(bool bVisible)                              //Steven 20140510 : XY變距
{
    if(bVisible)                                                                //Steven 20151103 : Fixed for 32Site YPitch
    {
        edPitchX1->Visible  =bVisible;
    }
    lblPitchX2->Visible     =bVisible;
    lblPitchY->Visible      =bVisible;
    edPitchX2->Visible      =bVisible;
    edPitchY->Visible       =bVisible;
    lblPitchX2Range->Visible=bVisible;
    lblPitchYRange->Visible =bVisible;
    if(USE_PICKER_COUNT==ep16Picker)
    {
        lblPitchX3->Visible     =bVisible;
        lblPitchX4->Visible     =bVisible;
        edPitchX3->Visible      =bVisible;
        edPitchX4->Visible      =bVisible;
    }
    else
    {
        lblPitchX3->Visible     =false;
        lblPitchX4->Visible     =false;
        edPitchX3->Visible      =false;
        edPitchX4->Visible      =false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::ShowOneByOneOffSet()
{
    if(iNowOffsetSel<OfsLoader || iNowOffsetSel>=OfsTotal)
        return;

    if(LastSet.iLanguageCountry==0)
    {
        labWarningForZ->Caption="*. In arm Z axis offset are same as Loader";
        lblOutArm->Caption="*. Out arm Z axis offset are same as Auto1";
    }
    else
    {
        labWarningForZ->Caption="*. In Arm Z軸補償在Loader";
        lblOutArm->Caption="*. Out Arm Z軸補償在Auto 1";
    }

    for(int i=0; i<InArmSuck.iMotRow; i++)
    {
        for(int j=0; j<InArmSuck.iMotCol; j++)
        {
            if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||                      //Steven 20141029 : XY-Pitch
               USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                      //QQ
               USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be  ||                      //Ztex 2023.12.06 Add HT-1032
               USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                    //Ztex 2024.02.24 Add HT-1132
            {
                if((iNowOffsetSel>=OfsOutSh1 &&
                    iNowOffsetSel<=OfsFix6) ||                                  //Steven 20230907 : For HT-9011UC Fix3 --> Fix6
                    iNowOffsetSel==OfsRotate_Out    ||
                    iNowOffsetSel==OfsTopView       ||
                    iNowOffsetSel==OfsPADView       ||                          //wei 20160617 Vitrox
                    iNowOffsetSel==OfsBGAView       ||                          //wei 20160617 Vitrox
                    iNowOffsetSel==OfsScanAOI       )                           //Ifor 20191229 : add
                {
                    if(i==iOutArmYBase && j==iOutArmXBase)
                    {
                        MyPickLab[i][j]    ->Visible=false;
                        MyPickEdit[i][j]   ->Visible=false;
                        MyPickEdit[i][j]   ->Text   =0;
                        MyRelsLab[i][j]    ->Visible=false;
                        MyRelsEdit[i][j]   ->Visible=false;
                        MyRelsEdit[i][j]   ->Text   =0;
                        continue;
                    }
                }
                else
                {
                    if(i==iInArmYBase && j==iInArmXBase)
                    {
                        MyPickLab[i][j]    ->Visible=false;
                        MyPickEdit[i][j]   ->Visible=false;
                        MyPickEdit[i][j]   ->Text   =0;
                        MyRelsLab[i][j]    ->Visible=false;
                        MyRelsEdit[i][j]   ->Visible=false;
                        MyRelsEdit[i][j]   ->Text   =0;
                        continue;
                    }
                }
            }
            else
            {
                if(i==iInArmYBase && j==iInArmXBase)
                {
                    MyPickLab[i][j]    ->Visible=false;
                    MyPickEdit[i][j]   ->Visible=false;
                    MyPickEdit[i][j]   ->Text   =0;
                    MyRelsLab[i][j]    ->Visible=false;
                    MyRelsEdit[i][j]   ->Visible=false;
                    MyRelsEdit[i][j]   ->Text   =0;
                    continue;
                }
            }

            if(IniConfig.bE33InOutArmZOffsetSameOne==true)
            {
                if((iNowOffsetSel>=OfsLoader && iNowOffsetSel<=OfsInSh2) ||     //jou 2014-03-11 add AutoClean & In Rotate
                    iNowOffsetSel==OfsAutoClean ||
                    iNowOffsetSel==OfsOCR ||
                    iNowOffsetSel==OfsRotate_In ||
                   (iNowOffsetSel>=OfsLoaderB && iNowOffsetSel<=OfsInSh2RB) ||
                    iNowOffsetSel==OfsAutoSh1 ||
                    iNowOffsetSel==OfsAutoSh2 ||                                //Steven 20140811 : For 32 Site Loader and Shuttle Offset    //wei 20160914 Auto Shuttle Sensor
                    iNowOffsetSel==OfsPreciser ||                               //Frank 20180512 : modify 1 Dut 1 Motor
                    iNowOffsetSel==OfsBottom2DID)                               //JerryYang 20260223 : fix bottom 2DID offset
                {
                    labWarningForZ  ->Visible=true;
                    lblOutArm       ->Visible=false;
                }
                else
                {
                    labWarningForZ  ->Visible=false;
                    lblOutArm       ->Visible=true;
                }

                if(iNowOffsetSel>=OfsTopView && iNowOffsetSel<=OfsBGAView)      //wei 20160617 Vitrox
                {
                    MyRelsLab[i][j] ->Visible=true;
                    MyRelsEdit[i][j]->Visible=true;
                    MyPickLab[i][j] ->Visible=false;
                    MyPickEdit[i][j]->Visible=false;
                    labWarningForZ  ->Visible=false;
                    lblOutArm       ->Visible=false;
                }
                else if(iNowOffsetSel==OfsLoader ||                             //kevin 20130604 add rotate
                        iNowOffsetSel==OfsAuto1)                                //ChungHung 20140506 alter 參數放錯
                {
                    MyPickLab[i][j] ->Visible=true;
                    MyPickEdit[i][j]->Visible=true;
                    MyRelsLab[i][j] ->Visible=true;
                    MyRelsEdit[i][j]->Visible=true;
                }
                else if(iNowOffsetSel==OfsScanAOI)                              //Ifor 20191229 : add
                {
                    MyRelsLab[i][j] ->Visible=true;
                    MyRelsEdit[i][j]->Visible=true;
                    if(USE_Scanner_AOI_Inspection==(int)eBtnAOI_TopBottomInstall)            //Jimmychiu 20240322 : Top & Bottom Inspect
                    {
                        MyPickLab[i][j]->Visible=true;
                        MyPickEdit[i][j]->Visible=true;
                    }
                    else
                    {
                        MyPickLab[i][j] ->Visible=false;
                        MyPickEdit[i][j]->Visible=false;
                    }
                    labWarningForZ  ->Visible=false;
                    lblOutArm       ->Visible=false;
                }
                else
                {
                    MyPickLab[i][j] ->Visible=false;
                    MyPickEdit[i][j]->Visible=false;
                    MyRelsLab[i][j] ->Visible=false;
                    MyRelsEdit[i][j]->Visible=false;
                }
            }
            else
            {
                labWarningForZ      ->Visible=false;
                lblOutArm           ->Visible=false;
                //------------------------------------------
                //pick Group
                //------------------------------------------
                if(iNowOffsetSel==OfsAutoClean  ||
                   iNowOffsetSel==OfsInSh1      ||
                   iNowOffsetSel==OfsInSh2      ||
                   (iNowOffsetSel>=OfsAuto1 &&                                  //InSH1 InSH2 Auto1~Fix3
                    iNowOffsetSel<=OfsFix6) ||                                  //Steven 20230907 : For HT-9011UC Fix3 --> Fix6
                   (iNowOffsetSel>=OfsInSh1LB && iNowOffsetSel<=OfsInSh2RB) ||  //Steven 20140811 : For 32 Site Loader and Shuttle Offset
                   iNowOffsetSel==OfsAutoSh1 || iNowOffsetSel==OfsAutoSh2 ||    //wei 20160914 Auto Shuttle Sensor
                   iNowOffsetSel==OfsPreciser)
                {
                    if(iNowOffsetSel==OfsFix3 &&
                       CosFunction.bUseFix3FullTray &&                          //Ifor 20161222 (jou) add 若有開啟Fix3 Full Tray 客戶功能 顯示Fix3 Pickup Offset
                       FIX3_FULL_PLACE==Fix3K_Uninstall)                        //Steven 20250911 : for 硬體 fix3 full 可以關閉
                    {
                        MyPickLab[i][j] ->Visible=true;
                        MyPickEdit[i][j]->Visible=true;
                    }
                    else
                    {
                        MyPickLab[i][j] ->Visible=false;
                        MyPickEdit[i][j]->Visible=false;
                        MyPickEdit[i][j]->Text=0;
                    }
                }
                else if(iNowOffsetSel>=OfsTopView && iNowOffsetSel<=OfsBGAView) //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
                {
                    MyPickLab[i][j] ->Visible=false;
                    MyPickEdit[i][j]->Visible=false;
                    MyPickEdit[i][j]->Text=0;
                }
                else if(iNowOffsetSel==OfsScanAOI)                              //Ifor 20191229 : add
                {
                    MyPickLab[i][j] ->Visible=false;
                    MyPickEdit[i][j]->Visible=false;
                    MyPickEdit[i][j]->Text=0;
                }
                else
                {
                    MyPickLab[i][j]->Visible=true;
                    MyPickEdit[i][j]->Visible=true;
                }
                //------------------------------------------
                //Release Group
                //------------------------------------------
                if(iNowOffsetSel==OfsAutoClean ||
                   iNowOffsetSel==OfsLoader    ||
                   iNowOffsetSel==OfsOutSh1    ||
                   iNowOffsetSel==OfsOutSh2    ||                               //Loader and OutSH1 OutSH2
                   iNowOffsetSel==OfsLoaderB   ||                               //Steven 20140811 : For 32 Site Loader and Shuttle Offset
                   iNowOffsetSel==OfsAutoSh1   ||
                   iNowOffsetSel==OfsAutoSh2     )                              //wei 20160914 Auto Shuttle Sensor
                {
                    MyRelsLab[i][j] ->Visible=false;
                    MyRelsEdit[i][j]->Visible=false;
                    MyRelsEdit[i][j]->Text=0;
                }
                else
                {
                    MyRelsEdit[i][j]->Visible=true;
                    MyRelsLab[i][j] ->Visible=true;
                }
            }

            bool bShowSingleOffsetSet = false;
            if(ArmSpeed[OutArm].bVariModeFIX==true ||                           //Eastsun 20251224 : 吸嘴獨立offset功能
               ArmSpeed[InArm].bVariModeFIX ==true)                             //Eastsun 20251224 : 只有在IN OUT ARM
            {                                                                   //Eastsun 20251224 : 開啟Fix才有效
                if((iNowOffsetSel>=OfsAuto1     &&                              //Eastsun 20251224 : 選擇在loader/unloader區才可顯示
                   iNowOffsetSel<=OfsFix6)      ||
                   iNowOffsetSel==OfsLoader     )
                {
                    MySingleOffsetTEditX[i][j]->Visible=true;
                    MySingleOffsetTEditY[i][j]->Visible=true;
                    MySingleOffsetLabX[i][j]->Visible=true;
                    MySingleOffsetLabY[i][j]->Visible=true;
                    bShowSingleOffsetSet = true;
                }
            }

            if(!bShowSingleOffsetSet)                                           //Eastsun 20251224 : 其他區域沒有寫offset所以不寫
            {
                MySingleOffsetTEditX[i][j]->Visible=false;
                MySingleOffsetTEditY[i][j]->Visible=false;
                MySingleOffsetLabX[i][j]->Visible=false;
                MySingleOffsetLabY[i][j]->Visible=false;
            }
        }
    }

    if(IniConfig.bE34InOutArmPitchZOffsetSameOne==false)
    {
        switch(iNowOffsetSel)                                                   //Sam 20230508 : 整理 Offset 顯示
        {
            case OfsAutoClean:
            case OfsHP1:
            case OfsHP2:
            case OfsPreciser:                                                   //Sam 20230508 : 修正 Preciser 顯示
                edPickUp ->Visible=true;
                edRelease->Visible=true;
                edPitchX1->Visible=true;
                if(CUSTOMER_CODE!=CC_ASE_KaohSiung)                             //kevin 20190212
                    SetXYPitchVCLVisible(USE_IN_Y_IS_AUTO_PITCH==true);         //JerryYang 20251218 : IN/OUT ARM支援不同模組
                break;
            case OfsInSh1:
            case OfsInSh2:
                if(IniConfig.bP27AutoSortingBinTrayByOutArmwhenCleanOut==false) //RogerYang 20251225 : IN/OUT ARM支援不同模組
                {
                    edPickUp->Visible=false;
                }
                else
                {
                    edPickUp->Visible=true;
                }
                edRelease->Visible=true;
                edPitchX1->Visible=true;
                SetXYPitchVCLVisible(USE_IN_Y_IS_AUTO_PITCH==true);
                break;
            case OfsAuto1:
            case OfsAuto2:
            case OfsAuto3:
            case OfsAuto4:                                                      //Steven 20230907 : For HT-9011UC
            case OfsAuto5:
            case OfsAuto6:
            case OfsFix1:
            case OfsFix2:
            case OfsFix3:
            case OfsFix4:                                                       //Steven 20230907 : For HT-9011UC
            case OfsFix5:
            case OfsFix6:
                if(IniConfig.bP27AutoSortingBinTrayByOutArmwhenCleanOut==false &&
                   CosFunction.bUseFix3FullTray==false &&                       //JerryYang 20150910 整盤功能需要Tray Pickup IC Offset   //Ifor 20161222 (jou) add Fix3 Full Tray Offset
                   AUTO3_IS_MAGAZINE==0)                                        //JerryYang 20220909 : add magazine
                {
                    edPickUp->Visible=false;
                }
                else                                                            //RogerYang 20170731 (wei) 修正功能重複開啟後 PickUp 設定就不見問題
                {
                    edPickUp->Visible=true;
                }
                edRelease->Visible=true;
                edPitchX1->Visible=true;
                SetXYPitchVCLVisible(USE_OUT_Y_IS_AUTO_PITCH==true);            //JerryYang 20251218 : IN/OUT ARM支援不同模組
                break;
            case OfsLoader:
                if(fContact->IsRun2DCheck()==true)          //JerryYang 20250220 : 2DID硬體順序檢查功能
                    edRelease->Visible=true;
                else
                    edRelease->Visible=false;
                edPickUp ->Visible=true;
                edPitchX1->Visible=true;
                SetXYPitchVCLVisible(USE_IN_Y_IS_AUTO_PITCH==true);             //JerryYang 20251218 : IN/OUT ARM支援不同模組
                break;
            case OfsOutSh1:
            case OfsOutSh2:
                edRelease->Visible=false;
                edPickUp ->Visible=true;
                edPitchX1->Visible=true;
                SetXYPitchVCLVisible(USE_OUT_Y_IS_AUTO_PITCH==true);            //JerryYang 20251218 : IN/OUT ARM支援不同模組
                break;
            case OfsOCR:
                edRelease->Visible=true;
                edPickUp ->Visible=false;
                edPitchX1->Visible=false;
                SetXYPitchVCLVisible(USE_IN_Y_IS_AUTO_PITCH==true);             //JerryYang 20251218 : IN/OUT ARM支援不同模組
                break;
            case OfsInSh1LB:
            case OfsInSh1RA:
            case OfsInSh1RB:
            case OfsInSh2LB:
            case OfsInSh2RA:
            case OfsInSh2RB:
            case OfsOutSh1LB:
            case OfsOutSh1RA:
            case OfsOutSh1RB:
            case OfsOutSh2LB:
            case OfsOutSh2RA:
            case OfsOutSh2RB:
                edRelease->Visible=false;
                edPickUp ->Visible=false;
                edPitchX1->Visible=true;
                SetXYPitchVCLVisible(USE_OUT_Y_IS_AUTO_PITCH==true);
                break;
            case OfsLoaderB:
            case OfsAutoSh1:
            case OfsAutoSh2:
            case OfsInSh1LB_AutoClean:
            case OfsInSh1RA_AutoClean:
            case OfsInSh1RB_AutoClean:
            case OfsInSh2LB_AutoClean:
            case OfsInSh2RA_AutoClean:
            case OfsInSh2RB_AutoClean:
                edRelease->Visible=false;
                edPickUp ->Visible=false;
                edPitchX1->Visible=false;
                SetXYPitchVCLVisible(USE_IN_Y_IS_AUTO_PITCH==true);
                break;
            case OfsRotate_In:
            case OfsRotate_Out:
            case OfsInSh1_AutoClean:
            case OfsInSh2_AutoClean:
                edRelease->Visible=true;
                edPickUp ->Visible=true;
                edPitchX1->Visible=true;
                SetXYPitchVCLVisible(USE_IN_Y_IS_AUTO_PITCH==true);
                break;
            case OfsScanAOI:
                edRelease->Visible=true;
                if(USE_Scanner_AOI_Inspection==(int)eBtnAOI_TopBottomInstall)            //Jimmychiu 20240322 : Top & Bottom Inspect
                {
                    edPickUp->Visible=true;
                }
                else
                {
                    edPickUp ->Visible=false;
                }

                edPitchX1->Visible=true;
                SetXYPitchVCLVisible(false);
                break;
            case OfsBottom2DID:                                                 //JerryYang 20260223 : fix bottom 2DID offset
                edRelease->Visible=true;
                edPickUp ->Visible=false;
                SetXYPitchVCLVisible(USE_IN_Y_IS_AUTO_PITCH==true); //Steven 20140510 : XY變距
                break;
        }
    }
    else
    {
        switch(iNowOffsetSel)                                                   //jou 2010-12-20 Pitch & Z 縮減為一個
        {
            case OfsLoader:                                                     //Loader
                edPickUp ->Visible=true;
                if(fContact->IsRun2DCheck()==true)                              //JerryYang 20250220 : 2DID硬體順序檢查功能
                    edRelease->Visible=true;
                else
                    edRelease->Visible=false;
                edPitchX1->Visible=true;
                SetXYPitchVCLVisible(USE_IN_Y_IS_AUTO_PITCH==true); //Steven 20140510 : XY變距    //JerryYang 20251218 : IN/OUT ARM支援不同模組
                break;
            case OfsOutSh1:                                                     //Out Shuttle 1
                edPickUp ->Visible=true;
                edRelease->Visible=false;
                edPitchX1->Visible=true;
                SetXYPitchVCLVisible(USE_OUT_Y_IS_AUTO_PITCH==true); //Steven 20140510 : XY變距   //JerryYang 20251218 : IN/OUT ARM支援不同模組
                break;
            case OfsHP1:                                                        //Hotplate 1
            case OfsPreciser:                                                   //Sam 20230508 : 修正 Preciser 顯示
                edPickUp ->Visible=true;
                edRelease->Visible=true;
                edPitchX1->Visible=false;
                SetXYPitchVCLVisible(false);                                    //Steven 20140510 : XY變距
                break;
            case OfsHP2:                                                        //Hotplate 2
                edPickUp ->Visible=true;
                edRelease->Visible=false;
                edPitchX1->Visible=false;
                SetXYPitchVCLVisible(false);                                    //Steven 20140510 : XY變距
                break;
            case OfsOutSh2:                                                     //Out Shuttle 2
            case OfsLoaderB:                                                    //20 //Steven 20140811 : For 32 Site Loader and Shuttle Offset
                edPickUp ->Visible=true;
                edRelease->Visible=false;
                edPitchX1->Visible=false;
                SetXYPitchVCLVisible(false);                                    //Steven 20140510 : XY變距
                break;
            case OfsInSh2:                                                      //In Shuttle 2
            case OfsAuto2:                                                      //Auto 2
            case OfsAuto3:                                                      //Auto 3
            case OfsAuto4:                                                      //Auto 4    //Steven 20230907 : For HT-9011UC
            case OfsAuto5:                                                      //Auto 5
            case OfsAuto6:                                                      //Auto 6
            case OfsFix2:                                                       //Fix 2
                if(FIX3_FULL_PLACE==Fix3K_UseCylinder46LA)                      //ChungHung 20140722 add for HT9046LA
                {
                    edPickUp ->Visible=false;
                    edRelease->Visible=true;
                    edPitchX1->Visible=false;
                    SetXYPitchVCLVisible(false);                                //Steven 20140510 : XY變距
                    break;
                }
            case OfsFix3:                                                       //Fix 3
            case OfsFix4:                                                       //Fix 4     //Steven 20230907 : For HT-9011UC
            case OfsFix5:                                                       //Fix 5
            case OfsFix6:                                                       //Fix 6
            case OfsInSh1LB:                                                    //21 //Steven 20140811 : For 32 Site Loader and Shuttle Offset
            case OfsInSh1RA:                                                    //22 //Steven 20140811 : For 32 Site Loader and Shuttle Offset
            case OfsInSh1RB:                                                    //23 //Steven 20140811 : For 32 Site Loader and Shuttle Offset
            case OfsInSh2LB:                                                    //24 //Steven 20140811 : For 32 Site Loader and Shuttle Offset
            case OfsInSh2RA:                                                    //25 //Steven 20140811 : For 32 Site Loader and Shuttle Offset
            case OfsInSh2RB:                                                    //26 //Steven 20140811 : For 32 Site Loader and Shuttle Offset
                edPickUp ->Visible=false;
                edRelease->Visible=false;
                edPitchX1->Visible=false;
                SetXYPitchVCLVisible(false);                                    //Steven 20140510 : XY變距
                break;
            case OfsInSh1:                                                      //In Shuttle 1
            case OfsAuto1:                                                      //Auto 1
                edPickUp ->Visible=false;
                edRelease->Visible=true;
                edPitchX1->Visible=true;
                SetXYPitchVCLVisible(USE_OUT_Y_IS_AUTO_PITCH==true);                   //Steven 20140510 : XY變距  //JerryYang 20251218 : IN/OUT ARM支援不同模組
                break;
            case OfsFix1:                                                       //Fix 1
                edPickUp ->Visible=false;
                edRelease->Visible=true;
                edPitchX1->Visible=false;
                SetXYPitchVCLVisible(false);                                    //Steven 20140510 : XY變距
                break;
            case OfsTopView:                                                    //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
                edPickUp ->Visible=true;
                edPitchX1->Visible=false;
                SetXYPitchVCLVisible(false);                                    //Steven 20140510 : XY變距
                break;
            case OfsPADView:                                                    //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
            case OfsBGAView:                                                    //wei 20160617 Vitrox
                edPitchX1->Visible=false;
                edPickUp ->Visible=false;
                SetXYPitchVCLVisible(false);                                    //Steven 20140510 : XY變距
                break;
            case OfsScanAOI:                                                    //Ifor 20191229 : add
                edPitchX1->Visible=true;
                edRelease->Visible=true;
                if(USE_Scanner_AOI_Inspection==(int)eBtnAOI_TopBottomInstall)            //Jimmychiu 20240322 : Top & Bottom Inspect
                {
                    edPickUp->Visible=true;
                }
                else
                {
                    edPickUp ->Visible=false;
                }
                break;
            case OfsBottom2DID:                                                 //JerryYang 20260223 : fix bottom 2DID offset
                edRelease->Visible=true;
                edPickUp ->Visible=false;
                SetXYPitchVCLVisible(USE_IN_Y_IS_AUTO_PITCH==true);
                SetXYPitchVCLVisible(false);                                    //Steven 20140510 : XY變距
                break;
        }
    }

    if(USE_PICKER_COUNT==0)                                                     //Steven 20161117 : for HT-9045S
    {
        EdtRelsC ->Visible=false;
        EdtRelsD ->Visible=false;
        EdtRelsG ->Visible=false;
        EdtRelsH ->Visible=false;
        EditPickC->Visible=false;
        EditPickD->Visible=false;
        EditPickG->Visible=false;
        EditPickH->Visible=false;
    }

    if(iNowOffsetSel==OfsPreciser)                                              //Frank 20180410 (Steven) : InArm Preciser Station
    {
        palPreciser->Visible=true;
    }
    else
    {
        palPreciser->Visible=false;
    }

    lblRelease      ->Visible=edRelease->Visible;
    lblReleaseRange ->Visible=edRelease->Visible;                               //kevin 20140320 秀極限範圍
    lblPickUp       ->Visible=edPickUp ->Visible;
    lblPickUpRange  ->Visible=edPickUp ->Visible;                               //kevin 20140320 秀極限範圍
    lblPitchX1      ->Visible=edPitchX1->Visible;
    lblPitchX1Range ->Visible=edPitchX1->Visible;                               //kevin 20140320 秀極限範圍
    if(CUSTOMER_CODE==CC_ASE_CL && AccessLevel>=iDefEngineerLevel)              //JerryYang 20250120 : modify
    {
        sbZcalibration->Visible=true;
    }
    else
    {
        sbZcalibration->Visible=false;
    }
}
//---------------------------------------------------------------------------
bool __fastcall TfOffSet::SaveFile(int iSelPartData, int SpecialMode, bool bReset)
{
    if(iSelPartData<0 || iSelPartData>OfsTotal)
        return false;
    AnsiString szDir="", szFilePath="";

    if(CosFunction.bSaveOffsetByMachine &&
       IniConfig.bA57_3SaveOffsetByMachine)                                     //JimmyChiu 20220618 : save by machine
    {
        szDir=sSaveByMachine;
    }
    else
    {
        szDir=GetOffsetPath();
    }
    szFilePath=szDir;

    bool bResult=SaveSetupFile(szDir, szFilePath, iSelPartData, SpecialMode, bReset);//kevin 20150105
    #ifdef ASE_KaohSiung
        fBuilder->bSaveAsJobFile(LastFileName, "JOBFILE");                      //kevin 20150105  Start 另存 jobfile
    #endif
    return bResult;
}
//------------------------------------------------------------
AnsiString TfOffSet::GetOffsetPath(AnsiString FileFolder)
{
    AnsiString szDir="", Str;
    int iPos;

    if(CUSTOMER_CODE==CC_ASE_CL ||
       IniConfig.bE45_AllSetupFileUseOneFile)                                   //Steven 20140827 : 所有工作檔共用同一個Offset檔案
    {
        szDir=FileInfo().PathCombin(DefaultPath, "DefineOffset");
    }
    else
    {
        if(FileFolder!="")
            LastFileName=FileFolder;
        else
            LastFileName=GetLastOpenFN();

        if(IniConfig.bE59GroupOffsetFile)                                       //Steven 20190327 : Offset file使用中括號做群組
        {
            iPos=LastFileName.AnsiPos("]");
            if(iPos!=0)
            {
                Str=LastFileName.SubString(0, iPos);
                szDir=FileInfo().PathCombin(Str, OffsetPath);
            }
            else
            {
                szDir=FileInfo().PathCombin(OffsetPath, LastFileName);          //Steven 20100927 Start : Offset改資料夾
            }
        }
        else
        {
            szDir=FileInfo().PathCombin(OffsetPath, LastFileName);              //Steven 20100927 Start : Offset改資料夾
        }
    }
    return szDir;
}
//------------------------------------------------------------
//kevin 20140105 儲存 檔案另存 jobfile
//------------------------------------------------------------
bool __fastcall TfOffSet::SaveSetupFile(AnsiString szDir, AnsiString szFilePath, int iSelPartData, int SpecialMode, bool bReset)
{
    bool FilePathErr=false;
    bool bOverLimitation=false;
    bool bUnderLimitation=false;
    double dPickup, dRelease;
    int  iNum;
    AnsiString str;

    int iZLimitHigh;
    int iZLimitLow;
    if(iSelPartData==OfsAuto1 ||                                                //Steven 20210317 : 通富微要求unloader獨立設置
       iSelPartData==OfsAuto2 ||
       iSelPartData==OfsAuto3 ||
       iSelPartData==OfsAuto4 ||                                                //Steven 20230907 : For HT-9011UC
       iSelPartData==OfsAuto5 ||
       iSelPartData==OfsAuto6 ||
       iSelPartData==OfsFix1  ||
       iSelPartData==OfsFix2  ||
       iSelPartData==OfsFix3  ||
       iSelPartData==OfsFix4  ||                                                //Steven 20230907 : For HT-9011UC
       iSelPartData==OfsFix5  ||
       iSelPartData==OfsFix6)
    {
        iZLimitHigh=InputLimit.iOffsetUnloaderZHigh;
        iZLimitLow =InputLimit.iOffsetUnloaderZLow;
    }
    else
    {
        iZLimitHigh=InputLimit.iOffsetZHigh;
        iZLimitLow =InputLimit.iOffsetZLow;
    }

    MyForceDirectories(szDir);

    if(Tri_Temp_Machine==1)                                                     //Ztex 2024.03.13 Add Low Temp Offset
        szDir+=Tri_Position_Offset();
    else
        szDir+="\\Position Offset.Data";

    if(!FileExists(szDir))
        FilePathErr=true;

    if(bReset || FilePathErr)                                                   //save mode at first step
    {
        for(int k=0; k<OfsTotal; k++)                                           //only make ini mode
        {
            if(AUTO_EMPTY_COLOR<3)                                              //Steven 20230907 : For HT-9011UC
            {
                if(k==OfsAuto4 || k==OfsAuto5 || k==OfsAuto6 ||
                   k==OfsFix4  || k==OfsFix5  || k==OfsFix6)
                {
                    continue;                                                   //沒用到的不存檔
                }
            }

            WriteIniData(szDir, CapStr[k], "One By One", 1);
            WriteIniData(szDir, CapStr[k], "VariableY",  0.0);                  //ChungHung 20131231 alter AutoYPitch
            WriteIniData(szDir, CapStr[k], "Variable",   0.0);
            WriteIniData(szDir, CapStr[k], "Variable2",  0.0);                  //Steven 20131002 : XY變距
            WriteIniData(szDir, CapStr[k], "Hand X",     0.0);
            WriteIniData(szDir, CapStr[k], "Hand Y",     0.0);
            WriteIniData(szDir, CapStr[k], "PickUp",     0.0);
            WriteIniData(szDir, CapStr[k], "Place",      0.0);
            for(int i=0; i<InArmSuck.iMotCol; i++)                              //8=In/out arm suction
            {
                for(int j=0; j<InArmSuck.iMotRow; j++)
                {
                    WriteIniData(szDir, CapStr[k], str.sprintf("Hand %cX",  'A'+(i*2)+j), 0.0);
                    WriteIniData(szDir, CapStr[k], str.sprintf("Hand %cY",  'A'+(i*2)+j), 0.0);
                    WriteIniData(szDir, CapStr[k], str.sprintf("PickUp %c", 'A'+(i*2)+j), 0.0);
                    WriteIniData(szDir, CapStr[k], str.sprintf("Place %c",  'A'+(i*2)+j), 0.0);
                }
            }
        }

        if(bReset)
            return false;

        for(int i=0; i<2; i++)
        {
            str="";
            str.sprintf("Test Arm%d", i+1);
            WriteIniData(szDir, str, "Pick Up",       0.0);
            WriteIniData(szDir, str, "Place",         0.0);
            WriteIniData(szDir, str, "Contact",       0.0);
            WriteIniData(szDir, str, "Shuttle Right", 0.0);
            WriteIniData(szDir, str, "Busy SH Halt",  0.0);
            WriteIniData(szDir, str, "ART-Place",     0.0);                     //kevin 20170831 (Steven) ART offset
        }

        for(int i=0; i<=tOfsAuto6; i++)
        {
            WriteIniData(szDir, SpecialOffSetName[i], "Place", 0.0);
            WriteIniData(szDir, SpecialOffSetName[i], "ART-Place", 0.0);        //kevin 20170831 (Steven) ART offset
            WriteIniData(szDir, SpecialOffSetName[i], "Loader Z", 0.0);         //Steven 20190813 : 入Tray改用步進馬達
        }
        FilePathErr=false;
    }

    if(SpecialMode & iSaveSpecial)
    {
        if(iSelPartData<=tOfsAuto6)
        {
            if(AUTO_EMPTY_COLOR<3 &&                                            //Steven 20230907 : For HT-9011UC
               (SpecialMode==tOfsAuto4 ||
                SpecialMode==tOfsAuto5 ||
                SpecialMode==tOfsAuto6))
            {
            }
            else
            {
                WriteIniData(szDir, SpecialOffSetName[iSelPartData], "Place",       edtTrayArmX->Text);
                WriteIniData(szDir, SpecialOffSetName[iSelPartData], "ART-Place",   edtARTPlace->Text);     //kevin 20170831 (Steven) ART offset
                WriteIniData(szDir, SpecialOffSetName[iSelPartData], "Loader Z",    edlLoadZ->Text);        //Steven 20190813 : 入Tray改用步進馬達
            }
        }
        else
        {
            if(iSelPartData==tOfsIndex1)
            {
                iNum=1;
            }
            else if(iSelPartData==tOfsIndex2)
            {
                iNum=2;
            }
            else
            {
                ShowErrorMessage("WAR1682", 0, MMSystem, 0, szDir);
                return true;
            }

            str="";
            str.sprintf("Test Arm%d", iNum);                                    //Steven 20140825 : iNum --> iNum+1
            WriteIniData(szDir, str, "Pick Up",       IndexArmOffSet1->Text);
            WriteIniData(szDir, str, "Place",         IndexArmOffSet2->Text);
            if(CosFunction.bContactHaveOffset==false)                           //ChungHung 20140327 add by Customer 強制Contact Offset為0
            {
                WriteIniData(szDir, str, "Contact",   0);
            }
            else
            {
                WriteIniData(szDir, str, "Contact",   IndexArmOffSet3->Text);
            }

            szDir=szFilePath;                                                   //kevin 20150105
            if(Tri_Temp_Machine==1)                                             //Ztex 2024.03.13 Add Low Temp Offset
                szDir+=Tri_Position_Offset();
            else if(LastSet.iTemperature==Tempture_Hot)
                szDir+="\\Position Offset Hot.Data";
            else
                szDir+="\\Position Offset.Data";

            WriteIniData(szDir, str, "Busy SH Halt",  IndexArmOffSet4->Text);
            WriteIniData(szDir, str, "Shuttle Right", IndexArmOffSet5->Text);
            WriteIniData(szDir, str, "Shuttle Left",  IndexArmOffSet6->Text);
            WriteIniData(szDir, str, "Shuttle for 2D",  edShtFor2D->Text);      //Steven 20151218 : Offset for 2d reader
        }
    }

    bOverLimitation=false;
    bUnderLimitation=false;
    if(SpecialMode & iSaveStander)
    {
        if(AUTO_EMPTY_COLOR<3 &&                                                //Steven 20230907 : For HT-9011UC
           (iSelPartData==OfsAuto4 || iSelPartData==OfsAuto5 || iSelPartData==OfsAuto6 ||
            iSelPartData==OfsFix4  || iSelPartData==OfsFix5  || iSelPartData==OfsFix6))
        {
            //沒用到的不存檔
        }
        else
        {
            szDir=szFilePath;
            if(Tri_Temp_Machine==1)                                             //Ztex 2024.03.13 Add Low Temp Offset
                szDir+=Tri_Position_Offset();
            else if(LastSet.iTemperature==Tempture_Hot && ((iSelPartData==OfsInSh1    || iSelPartData==OfsInSh2 ||
                                                       iSelPartData==OfsOutSh1   || iSelPartData==OfsOutSh2) ||
                                                      (iSelPartData>=OfsInSh1LB  && iSelPartData<=OfsInSh2RB) ||    //Steven 20140811 : For 32 Site Loader and Shuttle Offset
                                                      (iSelPartData>=OfsOutSh1LB && iSelPartData<=OfsOutSh2RB)))    //Steven 20190802 : For 32 Site Out Shuttle Offset
                szDir+="\\Position Offset Hot.Data";
            else
                szDir+="\\Position Offset.Data";

            WriteIniData(szDir, CapStr[iSelPartData], "Hand X",   edArmX->Text);
            WriteIniData(szDir, CapStr[iSelPartData], "Hand Y",   edArmY->Text);

            szDir=szFilePath;

            if(Tri_Temp_Machine==1)                                             //Ztex 2024.03.13 Add Low Temp Offset
                szDir+=Tri_Position_Offset();
            else
                szDir+="\\Position Offset.Data";

            //Steven 20210324 : 超出上下限要提示
            //==>
            dPickup =atof(edPickUp->Text.c_str());
            dRelease=atof(edRelease->Text.c_str());

            if(edPickUp->Visible==true)
            {
                if(dPickup<double(InputLimit.iOffsetZLow))
                {
                    bUnderLimitation=true;
                    edPickUp->Color=clYellow;
                }
                else if(dPickup>double(InputLimit.iOffsetZHigh))
                {
                    bOverLimitation=true;
                    edPickUp->Color=clRed;
                }
                else
                {
                    edPickUp->Color=clWhite;
                }
            }
            else
            {
                edPickUp->Color=clWhite;
            }

            if(edRelease->Visible==true)
            {
                if(dRelease<double(iZLimitLow))
                {
                    bUnderLimitation=true;
                    edRelease->Color=clYellow;
                }
                else if(dRelease>double(iZLimitHigh))
                {
                    bOverLimitation=true;
                    edRelease->Color=clRed;
                }
                else
                {
                    edRelease->Color=clWhite;
                }
            }
            else
            {
                edRelease->Color=clWhite;
            }
            //<==
            //Steven 20210324 : 超出上下限要提示

            edPickUp->Text =CheckRange(dPickup, double(InputLimit.iOffsetZHigh), double(InputLimit.iOffsetZLow));  //JerryYang 20190328 Offset上下限保護
            edRelease->Text=CheckRange(dRelease, double(iZLimitHigh), double(iZLimitLow));
            WriteIniData(szDir, CapStr[iSelPartData], "One By One", 1);
            WriteIniData(szDir, CapStr[iSelPartData], "VariableY",edPitchY->Text);  //ChungHung 20131231 alter AutoYPitch
            WriteIniData(szDir, CapStr[iSelPartData], "Variable4",edPitchX4->Text); //Steven 20131002 : XY變距
            WriteIniData(szDir, CapStr[iSelPartData], "Variable3",edPitchX3->Text); //Steven 20131002 : XY變距
            WriteIniData(szDir, CapStr[iSelPartData], "Variable2",edPitchX2->Text); //Steven 20131002 : XY變距
            WriteIniData(szDir, CapStr[iSelPartData], "Variable", edPitchX1->Text);
            WriteIniData(szDir, CapStr[iSelPartData], "PickUp",   edPickUp->Text);
            WriteIniData(szDir, CapStr[iSelPartData], "Place",    edRelease->Text);

            if(iSelPartData==OfsPreciser)                                                       //wei 20240507 避免存檔錯誤
            {
                WriteIniData(szDir, "Preciser", "Preciser Open",      edPreciserOpen->Text);    //JerryYang 20180820 add
                WriteIniData(szDir, "Preciser", "Preciser Close",     edPreciserClose->Text);
            }

            for(int i=0; i<InArmSuck.iMotCol; i++)                              //8=In/out arm suction
            {
                for(int j=0; j<InArmSuck.iMotRow; j++)
                {
                    if(IniConfig.bE33InOutArmZOffsetSameOne==false ||
                       ((IniConfig.bE33InOutArmZOffsetSameOne==true) &&
                        (iSelPartData==OfsLoader  || iSelPartData==OfsAuto1   ||
                         iSelPartData==OfsPADView || iSelPartData==OfsBGAView ||
                         iSelPartData==OfsScanAOI || iSelPartData==OfsBottom2DID)))  //KaiChen 20200414 ：新增Bottom2DID OffSet  //Steven 20260316 : Fix condition
                    {
                        //Steven 20210324 : 超出上下限要提示
                        //==>
                        dPickup =atof(MyPickEdit[j][i]->Text.c_str());
                        dRelease=atof(MyRelsEdit[j][i]->Text.c_str());
                        if(MyPickEdit[j][i]->Visible==true)
                        {
                            if(dPickup<double(InputLimit.iOffsetZLow))
                            {
                                bUnderLimitation=true;
                                MyPickEdit[j][i]->Color=clYellow;
                            }
                            else if(dPickup>double(InputLimit.iOffsetZHigh))
                            {
                                bOverLimitation=true;
                                MyPickEdit[j][i]->Color=clRed;
                            }
                            else
                            {
                                MyPickEdit[j][i]->Color=clWhite;
                            }
                        }
                        else
                        {
                            MyPickEdit[j][i]->Color=clWhite;
                        }

                        if(MyPickEdit[j][i]->Visible==true)
                        {
                            if(dRelease<double(iZLimitLow))
                            {
                                bUnderLimitation=true;
                                MyRelsEdit[j][i]->Color=clYellow;
                            }
                            else if(dRelease>double(iZLimitHigh))
                            {
                                bOverLimitation=true;
                                MyRelsEdit[j][i]->Color=clRed;
                            }
                            else
                            {
                                MyRelsEdit[j][i]->Color=clWhite;
                            }
                        }
                        else
                        {
                            MyRelsEdit[j][i]->Color=clWhite;
                        }
                        //<==
                        //Steven 20210324 : 超出上下限要提示

                        MyPickEdit[j][i]->Text=CheckRange(dPickup, double(InputLimit.iOffsetZHigh), double(InputLimit.iOffsetZLow));   //JerryYang 20190328 Offset上下限保護
                        MyRelsEdit[j][i]->Text=CheckRange(dRelease, double(iZLimitHigh), double(iZLimitLow));

                        //WriteIniData(szDir, CapStr[iSelPartData], str.sprintf("Hand %cX",  'A'+(i*2)+j), MyXOffsetEdit[j][i]->Text);
                        //WriteIniData(szDir, CapStr[iSelPartData], str.sprintf("Hand %cY",  'A'+(i*2)+j), MyYOffsetEdit[j][i]->Text);
                        //==>
                        WriteIniData(szDir, CapStr[iSelPartData], str.sprintf("Hand %cX",  'A'+(i*2)+j), MySingleOffsetTEditX[j][i]->Text);//Eastsun 20251230 : 獨立offset功能 修復
                        WriteIniData(szDir, CapStr[iSelPartData], str.sprintf("Hand %cY",  'A'+(i*2)+j), MySingleOffsetTEditY[j][i]->Text);

                        WriteIniData(szDir, CapStr[iSelPartData], str.sprintf("PickUp %c", 'A'+(i*2)+j), MyPickEdit[j][i]->Text);
                        WriteIniData(szDir, CapStr[iSelPartData], str.sprintf("Place %c",  'A'+(i*2)+j), MyRelsEdit[j][i]->Text);
                    }
                }
            }
        }
    }

    AddSpace(szDir);
    if(CUSTOMER_CODE==CC_TFME_CHINA)                                            //Steven 20210324 : 超出上下限要提示
    {
        if(bOverLimitation || bUnderLimitation)
        {
            ShowMyMessage("There are some offset is not inside the limitation!\r And already changed automatically.", "部分補償值超出限制, 已被重置數值!");
            return true;
        }
    }
    return false;
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::ClearIndexOffset()
{
    AnsiString str;
    AnsiString szDir="";
    if(CosFunction.bSaveOffsetByMachine && IniConfig.bA57_3SaveOffsetByMachine) //JimmyChiu 20220618 : save by machine
    {
        szDir=sSaveByMachine;
    }
    else
    {
        szDir=fOffSet->GetOffsetPath();                                         //Steven 20190109 : 整合Offset路徑
    }
    MyForceDirectories(szDir);

    szDir+="\\Position OffSet.Data";

    for(int iNum=0; iNum<2; iNum++)
    {
        str="";
        str.sprintf("Test Arm%d", iNum+1);                                      //Steven 20140825 : iNum --> iNum+1
        WriteIniData(szDir, str, "Pick Up",       0);
        WriteIniData(szDir, str, "Place",         0);
        WriteIniData(szDir, str, "Contact",       0);

        Offset.iIndexArmContact[iNum]=0;
        Offset.iIndexArmPickUp [iNum]=0;
        Offset.iIndexArmPlace  [iNum]=0;

        Offset_File.iIndexArmContact[iNum]=0;
        Offset_File.iIndexArmPickUp [iNum]=0;
        Offset_File.iIndexArmPlace  [iNum]=0;
    }

    IndexArmOffSet1->Text="0";
    IndexArmOffSet2->Text="0";
    IndexArmOffSet3->Text="0";
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::ReadInvisibleFile(AnsiString szDir2)
{
    AnsiString str="";
    InvisibleOffset.LoaderXOffset=CheckRange(ReadWriteIni(szDir2,       "Loader", "LoaderXOffset", InvisibleOffset.LoaderXOffset , 0, true), -300, 300);
    InvisibleOffset.LoaderYOffset=CheckRange(ReadWriteIni(szDir2,       "Loader", "LoaderYOffset", InvisibleOffset.LoaderYOffset , 0, true), -300, 300);
    InvisibleOffset.LoaderZOffset=CheckRange(ReadWriteIni(szDir2,       "Loader", "LoaderZOffset", InvisibleOffset.LoaderZOffset , 0, true), -300, 300);
    InvisibleOffset.LoaderXPOffset=CheckRange(ReadWriteIni(szDir2,      "Loader", "LoaderXPitchOffset", InvisibleOffset.LoaderXPOffset , 0, true), -300, 300);
    InvisibleOffset.LoaderXP2Offset=CheckRange(ReadWriteIni(szDir2,     "Loader", "LoaderXPitch2Offset", InvisibleOffset.LoaderXP2Offset , 0, true), -300, 300);
    InvisibleOffset.LoaderYPOffset=CheckRange(ReadWriteIni(szDir2,      "Loader", "LoaderYPitchOffset", InvisibleOffset.LoaderYPOffset , 0, true), -300, 300);

    InvisibleOffset.HotPlate1XOffset=CheckRange(ReadWriteIni(szDir2,    "HotPlate", "HotPlate1XOffset",InvisibleOffset.HotPlate1XOffset ,0, true), -300, 300);
    InvisibleOffset.HotPlate1YOffset=CheckRange(ReadWriteIni(szDir2,    "HotPlate", "HotPlate1YOffset",InvisibleOffset.HotPlate1YOffset ,0, true), -300, 300);
    InvisibleOffset.HotPlate1ZOffset=CheckRange(ReadWriteIni(szDir2,    "HotPlate", "HotPlate1ZOffset",InvisibleOffset.HotPlate1ZOffset ,0, true), -300, 300);
    InvisibleOffset.HotPlate1XPOffset=CheckRange(ReadWriteIni(szDir2,   "HotPlate", "HotPlate1XPitchOffset", InvisibleOffset.HotPlate1XPOffset , 0, true), -300, 300);
    InvisibleOffset.HotPlate1XP2Offset=CheckRange(ReadWriteIni(szDir2,  "HotPlate", "HotPlate1XPitch2Offset", InvisibleOffset.HotPlate1XP2Offset , 0, true), -300, 300);
    InvisibleOffset.HotPlate1YPOffset=CheckRange(ReadWriteIni(szDir2,   "HotPlate", "HotPlate1YPitchOffset", InvisibleOffset.HotPlate1YPOffset , 0, true), -300, 300);

    InvisibleOffset.HotPlate2XOffset=CheckRange(ReadWriteIni(szDir2,    "HotPlate", "HotPlate2XOffset",InvisibleOffset.HotPlate2XOffset ,0, true), -300, 300);
    InvisibleOffset.HotPlate2YOffset=CheckRange(ReadWriteIni(szDir2,    "HotPlate", "HotPlate2YOffset",InvisibleOffset.HotPlate2YOffset ,0, true), -300, 300);
    InvisibleOffset.HotPlate2ZOffset=CheckRange(ReadWriteIni(szDir2,    "HotPlate", "HotPlate2ZOffset",InvisibleOffset.HotPlate2ZOffset ,0, true), -300, 300);
    InvisibleOffset.HotPlate2XPOffset=CheckRange(ReadWriteIni(szDir2,   "HotPlate", "HotPlate2XPitchOffset", InvisibleOffset.HotPlate2XPOffset , 0, true), -300, 300);
    InvisibleOffset.HotPlate2XP2Offset=CheckRange(ReadWriteIni(szDir2,  "HotPlate", "HotPlate2XPitch2Offset", InvisibleOffset.HotPlate2XP2Offset , 0, true), -300, 300);
    InvisibleOffset.HotPlate2YPOffset=CheckRange(ReadWriteIni(szDir2,   "HotPlate", "HotPlate2YPitchOffset", InvisibleOffset.HotPlate2YPOffset , 0, true), -300, 300);

    InvisibleOffset.InSht1XOffset=CheckRange(ReadWriteIni(szDir2,    "InShuttle", "Shuttle1XOffset", InvisibleOffset.InSht1XOffset,0, true), -300, 300);
    InvisibleOffset.InSht1YOffset=CheckRange(ReadWriteIni(szDir2,    "InShuttle", "Shuttle1YOffset", InvisibleOffset.InSht1YOffset,0, true), -300, 300);
    InvisibleOffset.InSht1ZOffset=CheckRange(ReadWriteIni(szDir2,    "InShuttle", "Shuttle1ZOffset",InvisibleOffset.InSht1ZOffset,0, true), -300, 300);
    InvisibleOffset.InSht1XPOffset=CheckRange(ReadWriteIni(szDir2,   "InShuttle", "Shuttle1XPitchOffset", InvisibleOffset.InSht1XPOffset , 0, true), -300, 300);
    InvisibleOffset.InSht1XP2Offset=CheckRange(ReadWriteIni(szDir2,  "InShuttle", "Shuttle1XPitch2Offset", InvisibleOffset.InSht1XP2Offset , 0, true), -300, 300);
    InvisibleOffset.InSht1YPOffset=CheckRange(ReadWriteIni(szDir2,   "InShuttle", "Shuttle1YPitchOffset", InvisibleOffset.InSht1YPOffset , 0, true), -300, 300);

    InvisibleOffset.InSht2XOffset=CheckRange(ReadWriteIni(szDir2,    "InShuttle", "Shuttle2XOffset", InvisibleOffset.InSht2XOffset,0, true), -300, 300);
    InvisibleOffset.InSht2YOffset=CheckRange(ReadWriteIni(szDir2,    "InShuttle", "Shuttle2YOffset", InvisibleOffset.InSht2YOffset,0, true), -300, 300);
    InvisibleOffset.InSht2ZOffset=CheckRange(ReadWriteIni(szDir2,    "InShuttle", "Shuttle2ZOffset",InvisibleOffset.InSht2ZOffset,0, true), -300, 300);
    InvisibleOffset.InSht2XPOffset=CheckRange(ReadWriteIni(szDir2,   "InShuttle", "Shuttle2XPitchOffset", InvisibleOffset.InSht2XPOffset , 0, true), -300, 300);
    InvisibleOffset.InSht2XP2Offset=CheckRange(ReadWriteIni(szDir2,  "InShuttle", "Shuttle2XPitch2Offset", InvisibleOffset.InSht2XP2Offset , 0, true), -300, 300);
    InvisibleOffset.InSht2YPOffset=CheckRange(ReadWriteIni(szDir2,   "InShuttle", "Shuttle2YPitchOffset", InvisibleOffset.InSht2YPOffset , 0, true), -300, 300);

    InvisibleOffset.OutSht1XOffset=CheckRange(ReadWriteIni(szDir2,   "OutShuttle", "Shuttle1XOffset", InvisibleOffset.OutSht1XOffset,0, true), -300, 300);
    InvisibleOffset.OutSht1YOffset=CheckRange(ReadWriteIni(szDir2,   "OutShuttle", "Shuttle1YOffset", InvisibleOffset.OutSht1YOffset,0, true), -300, 300);
    InvisibleOffset.OutSht1ZOffset=CheckRange(ReadWriteIni(szDir2,   "OutShuttle", "Shuttle1ZOffset",InvisibleOffset.OutSht1ZOffset,0, true), -300, 300);
    InvisibleOffset.OutSht1XPOffset=CheckRange(ReadWriteIni(szDir2,  "OutShuttle", "Shuttle1XPitchOffset",InvisibleOffset.OutSht1XPOffset,0, true), -300, 300);
    InvisibleOffset.OutSht1XP2Offset=CheckRange(ReadWriteIni(szDir2, "OutShuttle", "Shuttle1XPitch2Offset",InvisibleOffset.OutSht1XP2Offset,0, true), -300, 300);
    InvisibleOffset.OutSht1YPOffset=CheckRange(ReadWriteIni(szDir2,  "OutShuttle", "Shuttle1YPitchOffset",InvisibleOffset.OutSht1YPOffset,0, true), -300, 300);

    InvisibleOffset.OutSht2XOffset=CheckRange(ReadWriteIni(szDir2,   "OutShuttle", "Shuttle2XOffset", InvisibleOffset.OutSht2XOffset, 0 , true), -300, 300);
    InvisibleOffset.OutSht2YOffset=CheckRange(ReadWriteIni(szDir2,   "OutShuttle", "Shuttle2YOffset", InvisibleOffset.OutSht2YOffset, 0 , true), -300, 300);
    InvisibleOffset.OutSht2ZOffset=CheckRange(ReadWriteIni(szDir2,   "OutShuttle", "Shuttle22ZOffset",InvisibleOffset.OutSht2ZOffset ,0, true), -300, 300);
    InvisibleOffset.OutSht2XPOffset=CheckRange(ReadWriteIni(szDir2,  "OutShuttle", "Shuttle2XPitchOffset",InvisibleOffset.OutSht2XPOffset,0, true), -300, 300);
    InvisibleOffset.OutSht2XP2Offset=CheckRange(ReadWriteIni(szDir2, "OutShuttle", "Shuttle2XPitch2Offset",InvisibleOffset.OutSht2XP2Offset,0, true), -300, 300);
    InvisibleOffset.OutSht2YPOffset=CheckRange(ReadWriteIni(szDir2,  "OutShuttle", "Shuttle2YPitchOffset",InvisibleOffset.OutSht2YPOffset,0, true), -300, 300);

    InvisibleOffset.Auto1XOffset=CheckRange(ReadWriteIni(szDir2,   "Unloader", "Auto1XOffset", InvisibleOffset.Auto1XOffset,0, true), -300, 300);
    InvisibleOffset.Auto1YOffset=CheckRange(ReadWriteIni(szDir2,   "Unloader", "Auto1YOffset", InvisibleOffset.Auto1YOffset,0, true), -300, 300);
    InvisibleOffset.Auto1ZOffset=CheckRange(ReadWriteIni(szDir2,   "Unloader", "Auto1ZOffset", InvisibleOffset.Auto1ZOffset,0, true), -300, 300);
    InvisibleOffset.Auto1XPOffset=CheckRange(ReadWriteIni(szDir2,  "Unloader", "Auto1XPitchOffset", InvisibleOffset.Auto1XPOffset,0, true), -300, 300);
    InvisibleOffset.Auto1XP2Offset=CheckRange(ReadWriteIni(szDir2, "Unloader", "Auto1XPitch2Offset", InvisibleOffset.Auto1XP2Offset,0, true), -300, 300);
    InvisibleOffset.Auto1YPOffset=CheckRange(ReadWriteIni(szDir2,  "Unloader", "Auto1YPitchOffset", InvisibleOffset.Auto1YPOffset,0, true), -300, 300);

    InvisibleOffset.Auto2XOffset=CheckRange(ReadWriteIni(szDir2,   "Unloader", "Auto2XOffset", InvisibleOffset.Auto2XOffset,0, true), -300, 300);
    InvisibleOffset.Auto2YOffset=CheckRange(ReadWriteIni(szDir2,   "Unloader", "Auto2YOffset", InvisibleOffset.Auto2YOffset,0, true), -300, 300);
    InvisibleOffset.Auto2ZOffset=CheckRange(ReadWriteIni(szDir2,   "Unloader", "Auto2ZOffset", InvisibleOffset.Auto2ZOffset,0, true), -300, 300);
    InvisibleOffset.Auto2XPOffset=CheckRange(ReadWriteIni(szDir2,  "Unloader", "Auto2XPitchOffset", InvisibleOffset.Auto2XPOffset,0, true), -300, 300);
    InvisibleOffset.Auto2XP2Offset=CheckRange(ReadWriteIni(szDir2, "Unloader", "Auto2XPitch2Offset", InvisibleOffset.Auto2XP2Offset,0, true), -300, 300);
    InvisibleOffset.Auto2YPOffset=CheckRange(ReadWriteIni(szDir2,  "Unloader", "Auto2YPitchOffset", InvisibleOffset.Auto2YPOffset,0, true), -300, 300);

    InvisibleOffset.Auto3XOffset=CheckRange(ReadWriteIni(szDir2,   "Unloader", "Auto3XOffset", InvisibleOffset.Auto3XOffset,0, true), -300, 300);
    InvisibleOffset.Auto3YOffset=CheckRange(ReadWriteIni(szDir2,   "Unloader", "Auto3YOffset", InvisibleOffset.Auto3YOffset,0, true), -300, 300);
    InvisibleOffset.Auto3ZOffset=CheckRange(ReadWriteIni(szDir2,   "Unloader", "Auto3ZOffset", InvisibleOffset.Auto3ZOffset,0, true), -300, 300);
    InvisibleOffset.Auto3XPOffset=CheckRange(ReadWriteIni(szDir2,  "Unloader", "Auto3XPitchOffset", InvisibleOffset.Auto3XPOffset,0, true), -300, 300);
    InvisibleOffset.Auto3XP2Offset=CheckRange(ReadWriteIni(szDir2, "Unloader", "Auto3XPitch2Offset", InvisibleOffset.Auto3XP2Offset,0, true), -300, 300);
    InvisibleOffset.Auto3YPOffset=CheckRange(ReadWriteIni(szDir2,  "Unloader", "Auto3YPitchOffset", InvisibleOffset.Auto3YPOffset,0, true), -300, 300);

    InvisibleOffset.Fix1XOffset=CheckRange(ReadWriteIni(szDir2,    "Unloader", "Fix1XOffset",InvisibleOffset.Fix1XOffset ,0, true), -300, 300);
    InvisibleOffset.Fix1YOffset=CheckRange(ReadWriteIni(szDir2,    "Unloader", "Fix1YOffset",InvisibleOffset.Fix1YOffset ,0, true), -300, 300);
    InvisibleOffset.Fix1ZOffset=CheckRange(ReadWriteIni(szDir2,    "Unloader", "Fix1ZOffset",InvisibleOffset.Fix1ZOffset ,0, true), -300, 300);
    InvisibleOffset.Fix1XPOffset=CheckRange(ReadWriteIni(szDir2,   "Unloader", "Fix1XPitchOffset",InvisibleOffset.Fix1XPOffset ,0, true), -300, 300);
    InvisibleOffset.Fix1XP2Offset=CheckRange(ReadWriteIni(szDir2,  "Unloader", "Fix1XPitch2Offset",InvisibleOffset.Fix1XP2Offset ,0, true), -300, 300);
    InvisibleOffset.Fix1YPOffset=CheckRange(ReadWriteIni(szDir2,   "Unloader", "Fix1YPitchOffset",InvisibleOffset.Fix1YPOffset ,0, true), -300, 300);

    InvisibleOffset.Fix2XOffset=CheckRange(ReadWriteIni(szDir2,    "Unloader", "Fix2XOffset",InvisibleOffset.Fix2XOffset ,0, true), -300, 300);
    InvisibleOffset.Fix2YOffset=CheckRange(ReadWriteIni(szDir2,    "Unloader", "Fix2YOffset",InvisibleOffset.Fix2YOffset ,0, true), -300, 300);
    InvisibleOffset.Fix2ZOffset=CheckRange(ReadWriteIni(szDir2,    "Unloader", "Fix2ZOffset",InvisibleOffset.Fix2ZOffset ,0, true), -300, 300);
    InvisibleOffset.Fix2XPOffset=CheckRange(ReadWriteIni(szDir2,   "Unloader", "Fix2XPitchOffset",InvisibleOffset.Fix2XPOffset ,0, true), -300, 300);
    InvisibleOffset.Fix2XP2Offset=CheckRange(ReadWriteIni(szDir2,  "Unloader", "Fix2XPitch2Offset",InvisibleOffset.Fix2XP2Offset ,0, true), -300, 300);
    InvisibleOffset.Fix2YPOffset=CheckRange(ReadWriteIni(szDir2,   "Unloader", "Fix2YPitchOffset",InvisibleOffset.Fix2YPOffset ,0, true), -300, 300);

    InvisibleOffset.Fix3XOffset=CheckRange(ReadWriteIni(szDir2,    "Unloader", "Fix3XOffset",InvisibleOffset.Fix3XOffset ,0, true), -300, 300);
    InvisibleOffset.Fix3YOffset=CheckRange(ReadWriteIni(szDir2,    "Unloader", "Fix3YOffset",InvisibleOffset.Fix3YOffset ,0, true), -300, 300);
    InvisibleOffset.Fix3ZOffset=CheckRange(ReadWriteIni(szDir2,    "Unloader", "Fix3ZOffset",InvisibleOffset.Fix3ZOffset ,0, true), -300, 300);
    InvisibleOffset.Fix3XPOffset=CheckRange(ReadWriteIni(szDir2,   "Unloader", "Fix3XPitchOffset",InvisibleOffset.Fix3XPOffset ,0, true), -300, 300);
    InvisibleOffset.Fix3XP2Offset=CheckRange(ReadWriteIni(szDir2,  "Unloader", "Fix3XPitch2Offset",InvisibleOffset.Fix3XP2Offset ,0, true), -300, 300);
    InvisibleOffset.Fix3YPOffset=CheckRange(ReadWriteIni(szDir2,   "Unloader", "Fix3YPitchOffset",InvisibleOffset.Fix3YPOffset ,0, true), -300, 300);

    for(int i=0; i<MAX_ARM_Col; i++)//8=In/out arm suction
    {
        for(int j=0; j<MAX_ARM_Row; j++)//8=In/out arm suction
        {
            if(i==iInArmXBase && j==iInArmYBase)
            {
                InvisibleOffset.InarmPickOffSet[j][i]=CheckRange(ReadWriteIni(szDir2,     "Loader", str.sprintf("PickUp %c", 'A'+(i*2)+j), InvisibleOffset.InarmPickOffSet[j][i] ,0, true), 0, 0);
                InvisibleOffset.InarmReleaseOffset[j][i]=CheckRange(ReadWriteIni(szDir2,  "Loader", str.sprintf("Place %c", 'A'+(i*2)+j), InvisibleOffset.InarmReleaseOffset[j][i] ,0, true), 0, 0);

                InvisibleOffset.OutarmPickOffSet[j][i]=CheckRange(ReadWriteIni(szDir2,     "Unloader", str.sprintf("PickUp %c", 'A'+(i*2)+j), InvisibleOffset.OutarmPickOffSet[j][i] ,0, true), 0, 0);
                InvisibleOffset.OutarmReleaseOffset[j][i]=CheckRange(ReadWriteIni(szDir2,  "Unloader", str.sprintf("Place %c", 'A'+(i*2)+j), InvisibleOffset.OutarmReleaseOffset[j][i] ,0, true), 0, 0);
            }
            else
            {
                InvisibleOffset.InarmPickOffSet[j][i]=CheckRange(ReadWriteIni(szDir2,     "Loader", str.sprintf("PickUp %c", 'A'+(i*2)+j),InvisibleOffset.InarmPickOffSet[j][i] ,0, true), -300, 300);
                InvisibleOffset.InarmReleaseOffset[j][i]=CheckRange(ReadWriteIni(szDir2,  "Loader", str.sprintf("Place %c", 'A'+(i*2)+j),InvisibleOffset.InarmReleaseOffset[j][i] ,0, true), -300, 300);

                InvisibleOffset.OutarmPickOffSet[j][i]=CheckRange(ReadWriteIni(szDir2,     "Unloader", str.sprintf("PickUp %c", 'A'+(i*2)+j),InvisibleOffset.OutarmPickOffSet[j][i] ,0, true), -300, 300);
                InvisibleOffset.OutarmReleaseOffset[j][i]=CheckRange(ReadWriteIni(szDir2,  "Unloader", str.sprintf("Place %c", 'A'+(i*2)+j),InvisibleOffset.OutarmReleaseOffset[j][i] ,0, true), -300, 300);
            }
        }
    }
    ReadWriteIni(szDir2,   "LoaderScale", "LoaderX", TestIF_File.fLoaderTrayXScaleBySetupFile ,1.0, true);
    ReadWriteIni(szDir2,   "LoaderScale", "LoaderY", TestIF_File.fLoaderTrayYScaleBySetupFile ,1.0, true);
    ReadWriteIni(szDir2,   "LoaderScale", "HotPlate1X", TestIF_File.fHotPlateXScaleBySetupFile[0]  ,1.0, true);
    ReadWriteIni(szDir2,   "LoaderScale", "HotPlate1Y", TestIF_File.fHotPlateYScaleBySetupFile[0]  ,1.0, true);
    ReadWriteIni(szDir2,   "LoaderScale", "HotPlate2X", TestIF_File.fHotPlateXScaleBySetupFile[1]  ,1.0, true);
    ReadWriteIni(szDir2,   "LoaderScale", "HotPlate2Y", TestIF_File.fHotPlateYScaleBySetupFile[1]  ,1.0, true);

    ReadWriteIni(szDir2,   "LoaderScale", "Shuttle1X", TestIF_File.fInShuttleXScaleBySetupFile[0]  ,1.0, true);
    ReadWriteIni(szDir2,   "LoaderScale", "Shuttle1Y", TestIF_File.fInShuttleYScaleBySetupFile[0]  ,1.0, true);
    ReadWriteIni(szDir2,   "LoaderScale", "Shuttle2X", TestIF_File.fInShuttleXScaleBySetupFile[1]  ,1.0, true);
    ReadWriteIni(szDir2,   "LoaderScale", "Shuttle2Y", TestIF_File.fInShuttleYScaleBySetupFile[1]  ,1.0, true);

    ReadWriteIni(szDir2,   "UnLoaderScale", "Shuttle1X", TestIF_File.fOutShuttleXScaleBySetupFile[0]  ,1.0, true);
    ReadWriteIni(szDir2,   "UnLoaderScale", "Shuttle1Y", TestIF_File.fOutShuttleYScaleBySetupFile[0]  ,1.0, true);
    ReadWriteIni(szDir2,   "UnLoaderScale", "Shuttle2X", TestIF_File.fOutShuttleXScaleBySetupFile[1]  ,1.0, true);
    ReadWriteIni(szDir2,   "UnLoaderScale", "Shuttle2Y", TestIF_File.fOutShuttleYScaleBySetupFile[1]  ,1.0, true);

    ReadWriteIni(szDir2,   "Index1","ShuttleROffset",0, 0, true);
    ReadWriteIni(szDir2,   "Index1","ShuttleLOffset",0, 0, true);
    ReadWriteIni(szDir2,   "Index2","ShuttleROffset",0, 0, true);
    ReadWriteIni(szDir2,   "Index2","ShuttleLOffset",0, 0, true);
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::ReadFile()
{
    AnsiString str="", S;
    AnsiString szDir="", szDir2="";

    if(CUSTOMER_CODE==CC_TSMC_TAINAN && LastFileName.AnsiPos("_NET")>0)         //wei 20161101 FTP下載讀取錯誤問題
    {
        LastFileName.Delete(LastFileName.Length()-3, LastFileName.Length());
    }

    if(CosFunction.bSaveOffsetByMachine && IniConfig.bA57_3SaveOffsetByMachine) //JimmyChiu 20220618 : save by machine
    {
        szDir=sSaveByMachine;
    }
    else
    {
        szDir=fOffSet->GetOffsetPath();                                         //Steven 20190109 : 整合Offset路徑
    }
    MyForceDirectories(szDir);

    if(Tri_Temp_Machine==1)                                                     //Ztex 2024.03.13 Add Low Temp Offset
        szDir+=Tri_Position_Offset();
    else
        szDir+="\\Position Offset.Data";

    for(int i=0; i<InOfsTotal; i++)                                             //Steven 20140425 : 重整Offset
    {
        InArmOffSet_File[i]->SetOneByOne (ReadIniData(szDir, CapStrInput[i], "One By One",0));
        InArmOffSet_File[i]->SetVariableY(ReadIniData(szDir, CapStrInput[i], "VariableY", 0.0));//ChungHung 20131231 alter AutoYPitch
        InArmOffSet_File[i]->SetVariable (ReadIniData(szDir, CapStrInput[i], "Variable",  0.0));
        InArmOffSet_File[i]->SetVariable2(ReadIniData(szDir, CapStrInput[i], "Variable2", 0.0));//Steven 20131002 : XY變距
        InArmOffSet_File[i]->SetVariable3(ReadIniData(szDir, CapStrInput[i], "Variable3", 0.0));
        InArmOffSet_File[i]->SetVariable4(ReadIniData(szDir, CapStrInput[i], "Variable4", 0.0));
        InArmOffSet_File[i]->SetX        (ReadIniData(szDir, CapStrInput[i], "Hand X",    0.0));
        InArmOffSet_File[i]->SetY        (ReadIniData(szDir, CapStrInput[i], "Hand Y",    0.0));
        InArmOffSet_File[i]->SetPickUp   (ReadIniData(szDir, CapStrInput[i], "PickUp",    0.0));
        InArmOffSet_File[i]->SetPlace    (ReadIniData(szDir, CapStrInput[i], "Place",     0.0));

        if(IniConfig.bE33InOutArmZOffsetSameOne==true)                          //jou 2014-10-07 加速 offset 讀取時間
        {
            if(i==InOfsLoader)
            {
                for(int j=0; j<InArmSuck.iMotCol; j++)
                {
                    for(int k=0; k<InArmSuck.iMotRow; k++)
                    {
                        InArmOffSet_File[i]->SingleOffSet->dPosOffSetX[k][j]   =ReadIniData(szDir, CapStrInput[i], str.sprintf("Hand %cX",  'A'+(j*2)+k), 0.0);
                        InArmOffSet_File[i]->SingleOffSet->dPosOffSetY[k][j]   =ReadIniData(szDir, CapStrInput[i], str.sprintf("Hand %cY",  'A'+(j*2)+k), 0.0);
                        InArmOffSet_File[i]->SingleOffSet->dPickUpOffSet[k][j] =ReadIniData(szDir, CapStrInput[i], str.sprintf("PickUp %c", 'A'+(j*2)+k), 0.0);
                        InArmOffSet_File[i]->SingleOffSet->dPlaceOffSet[k][j]  =ReadIniData(szDir, CapStrInput[i], str.sprintf("Place %c",  'A'+(j*2)+k), 0.0);
                    }
                }
            }
            else if(i==InOfsBottom2DID)                                         //KaiChen 20200414 ：新增Bottom2DID OffSet
            {
                for(int j=0; j<InArmSuck.iMotCol; j++)
                {
                    for(int k=0; k<InArmSuck.iMotRow; k++)
                    {
                        InArmOffSet_File[i]->SingleOffSet->dPosOffSetX[k][j]   =ReadIniData(szDir, CapStrInput[i], str.sprintf("Hand %cX",  'A'+(j*2)+k), 0.0);
                        InArmOffSet_File[i]->SingleOffSet->dPosOffSetY[k][j]   =ReadIniData(szDir, CapStrInput[i], str.sprintf("Hand %cY",  'A'+(j*2)+k), 0.0);
                        InArmOffSet_File[i]->SingleOffSet->dPickUpOffSet[k][j] =ReadIniData(szDir, CapStrInput[i], str.sprintf("PickUp %c", 'A'+(j*2)+k), 0.0);
                        InArmOffSet_File[i]->SingleOffSet->dPlaceOffSet[k][j]  =ReadIniData(szDir, CapStrInput[i], str.sprintf("Place %c",  'A'+(j*2)+k), 0.0);
                    }
                }
            }
            else
            {
                for(int j=0; j<InArmSuck.iMotCol; j++)
                {
                    for(int k=0; k<InArmSuck.iMotRow; k++)
                    {
                        InArmOffSet_File[i]->SingleOffSet->dPosOffSetX[k][j]   =0.0;
                        InArmOffSet_File[i]->SingleOffSet->dPosOffSetY[k][j]   =0.0;
                        InArmOffSet_File[i]->SingleOffSet->dPickUpOffSet[k][j] =0.0;
                        InArmOffSet_File[i]->SingleOffSet->dPlaceOffSet[k][j]  =0.0;
                    }
                }
            }
        }
        else
        {
            for(int j=0; j<InArmSuck.iMotCol; j++)
            {
                for(int k=0; k<InArmSuck.iMotRow; k++)
                {
                    InArmOffSet_File[i]->SingleOffSet->dPosOffSetX[k][j]   =ReadIniData(szDir, CapStrInput[i], str.sprintf("Hand %cX",  'A'+(j*2)+k), 0.0);
                    InArmOffSet_File[i]->SingleOffSet->dPosOffSetY[k][j]   =ReadIniData(szDir, CapStrInput[i], str.sprintf("Hand %cY",  'A'+(j*2)+k), 0.0);
                    InArmOffSet_File[i]->SingleOffSet->dPickUpOffSet[k][j] =ReadIniData(szDir, CapStrInput[i], str.sprintf("PickUp %c", 'A'+(j*2)+k), 0.0);
                    InArmOffSet_File[i]->SingleOffSet->dPlaceOffSet[k][j]  =ReadIniData(szDir, CapStrInput[i], str.sprintf("Place %c",  'A'+(j*2)+k), 0.0);
                }
            }
        }
        Offset_File.dInArmPickUp[i]=ReadIniData(szDir, CapStrInput[i], "PickUp",    0.0);           //KenHsieh 20220914 : 新增SECS用
    }

    for(int i=0; i<OutOfsTotal; i++)                                            //Steven 20140425 : 重整Offset
    {
        if(AUTO_EMPTY_COLOR<3)                                                  //Steven 20230907 : For HT-9011UC
        {
            if(i==OutOfsAuto4 || i==OutOfsAuto5 || i==OutOfsAuto6 ||
               i==OutOfsFix4  || i==OutOfsFix5  || i==OutOfsFix6)
            {
                continue;
            }
        }

        OutArmOffSet_File[i]->SetOneByOne (ReadIniData(szDir, CapStrOutput[i], "One By One", 0));
        OutArmOffSet_File[i]->SetVariableY(ReadIniData(szDir, CapStrOutput[i], "VariableY",  0.0)); //ChungHung 20131231 alter AutoYPitch
        OutArmOffSet_File[i]->SetVariable (ReadIniData(szDir, CapStrOutput[i], "Variable",   0.0));
        OutArmOffSet_File[i]->SetVariable2(ReadIniData(szDir, CapStrOutput[i], "Variable2",  0.0)); //Steven 20131002 : XY變距
        OutArmOffSet_File[i]->SetVariable3(ReadIniData(szDir, CapStrOutput[i], "Variable3",  0.0));
        OutArmOffSet_File[i]->SetVariable4(ReadIniData(szDir, CapStrOutput[i], "Variable4",  0.0));
        OutArmOffSet_File[i]->SetX        (ReadIniData(szDir, CapStrOutput[i], "Hand X",     0.0));
        OutArmOffSet_File[i]->SetY        (ReadIniData(szDir, CapStrOutput[i], "Hand Y",     0.0));
        OutArmOffSet_File[i]->SetPickUp   (ReadIniData(szDir, CapStrOutput[i], "PickUp",     0.0));
        OutArmOffSet_File[i]->SetPlace    (ReadIniData(szDir, CapStrOutput[i], "Place",      0.0));

        if(IniConfig.bE33InOutArmZOffsetSameOne==true)                          //jou 2014-10-07 加速 offset 讀取時間
        {
            if(i==OutOfsAuto1 || i==OutOfsPADView_Out ||
               i==OutOfsBGA_Out || i==OutOfsScannerAOI)                         //wei 20160617 Vitrox
            {
                for(int j=0; j<OutArmSuck.iMotCol; j++)
                {
                    for(int k=0; k<OutArmSuck.iMotRow; k++)
                    {
                        OutArmOffSet_File[i]->SingleOffSet->dPosOffSetX[k][j]  =ReadIniData(szDir, CapStrOutput[i], str.sprintf("Hand %cX",  'A'+(j*2)+k), 0.0);
                        OutArmOffSet_File[i]->SingleOffSet->dPosOffSetY[k][j]  =ReadIniData(szDir, CapStrOutput[i], str.sprintf("Hand %cY",  'A'+(j*2)+k), 0.0);
                        OutArmOffSet_File[i]->SingleOffSet->dPickUpOffSet[k][j]=ReadIniData(szDir, CapStrOutput[i], str.sprintf("PickUp %c", 'A'+(j*2)+k), 0.0);
                        OutArmOffSet_File[i]->SingleOffSet->dPlaceOffSet[k][j] =ReadIniData(szDir, CapStrOutput[i], str.sprintf("Place %c",  'A'+(j*2)+k), 0.0);
                    }
                }
            }
            else if(i==OutOfsAuto2 || i==OutOfsAuto3 ||                        //RogerYang 20260126 : 修正Auto1的Pick/Place offset被清成0 //eastsun 20251230 : 獨立偏移功能unloader區也要偏移
                    i==OutOfsAuto4 || i==OutOfsAuto5 || i==OutOfsAuto6 ||    //Steven 20260316 : Fix missing i== and add Auto4~6, Fix4~6
                    i==OutOfsFix1  || i==OutOfsFix2  || i==OutOfsFix3 ||
                    i==OutOfsFix4  || i==OutOfsFix5  || i==OutOfsFix6)
            {
                for(int j=0; j<OutArmSuck.iMotCol; j++)
                {
                    for(int k=0; k<OutArmSuck.iMotRow; k++)
                    {
                        OutArmOffSet_File[i]->SingleOffSet->dPosOffSetX[k][j]  =ReadIniData(szDir, CapStrOutput[i], str.sprintf("Hand %cX",  'A'+(j*2)+k), 0.0);
                        OutArmOffSet_File[i]->SingleOffSet->dPosOffSetY[k][j]  =ReadIniData(szDir, CapStrOutput[i], str.sprintf("Hand %cY",  'A'+(j*2)+k), 0.0);
                        OutArmOffSet_File[i]->SingleOffSet->dPickUpOffSet[k][j]=0.0;
                        OutArmOffSet_File[i]->SingleOffSet->dPlaceOffSet[k][j] =0.0;
                    }
                }
            }
            else
            {
                for(int j=0; j<OutArmSuck.iMotCol; j++)
                {
                    for(int k=0; k<OutArmSuck.iMotRow; k++)
                    {
                        OutArmOffSet_File[i]->SingleOffSet->dPosOffSetX[k][j]  =0.0;
                        OutArmOffSet_File[i]->SingleOffSet->dPosOffSetY[k][j]  =0.0;
                        OutArmOffSet_File[i]->SingleOffSet->dPickUpOffSet[k][j]=0.0;
                        OutArmOffSet_File[i]->SingleOffSet->dPlaceOffSet[k][j] =0.0;
                    }
                }
            }
        }
        else
        {
            for(int j=0; j<OutArmSuck.iMotCol; j++)
            {
                for(int k=0; k<OutArmSuck.iMotRow; k++)
                {
                    OutArmOffSet_File[i]->SingleOffSet->dPosOffSetX[k][j]  =ReadIniData(szDir, CapStrOutput[i], str.sprintf("Hand %cX",  'A'+(j*2)+k), 0.0);
                    OutArmOffSet_File[i]->SingleOffSet->dPosOffSetY[k][j]  =ReadIniData(szDir, CapStrOutput[i], str.sprintf("Hand %cY",  'A'+(j*2)+k), 0.0);
                    OutArmOffSet_File[i]->SingleOffSet->dPickUpOffSet[k][j]=ReadIniData(szDir, CapStrOutput[i], str.sprintf("PickUp %c", 'A'+(j*2)+k), 0.0);
                    OutArmOffSet_File[i]->SingleOffSet->dPlaceOffSet[k][j] =ReadIniData(szDir, CapStrOutput[i], str.sprintf("Place %c",  'A'+(j*2)+k), 0.0);
                }
            }
        }
        Offset_File.dOutArmPickUp[i]=ReadIniData(szDir, CapStrOutput[i], "PickUp",     0.0);        //KenHsieh 20220914 : 新增SECS用
    }

    //Sort Arm
    if(USE_OUT_SORT_ARM!=eartUninstall)                                         //RogerYang 20250417 for HT9046AU add
    {
        for(int i=0; i<SortOfsTotal; i++)
        {
            SortArmOffSet_File[i]->SetOneByOne (ReadIniData(szDir, CapStrSortput[i], "One By One", 0));
            SortArmOffSet_File[i]->SetVariableY(ReadIniData(szDir, CapStrSortput[i], "VariableY",  0.0)); //alter AutoYPitch
            SortArmOffSet_File[i]->SetVariable (ReadIniData(szDir, CapStrSortput[i], "Variable",   0.0));
            SortArmOffSet_File[i]->SetVariable2(ReadIniData(szDir, CapStrSortput[i], "Variable2",  0.0)); //XY變距
            SortArmOffSet_File[i]->SetVariable3(ReadIniData(szDir, CapStrSortput[i], "Variable3",  0.0));
            SortArmOffSet_File[i]->SetVariable4(ReadIniData(szDir, CapStrSortput[i], "Variable4",  0.0));
            SortArmOffSet_File[i]->SetX        (ReadIniData(szDir, CapStrSortput[i], "Hand X",     0.0));
            SortArmOffSet_File[i]->SetY        (ReadIniData(szDir, CapStrSortput[i], "Hand Y",     0.0));
            SortArmOffSet_File[i]->SetPickUp   (ReadIniData(szDir, CapStrSortput[i], "PickUp",     0.0));
            SortArmOffSet_File[i]->SetPlace    (ReadIniData(szDir, CapStrSortput[i], "Place",      0.0));

            if(IniConfig.bE33InOutArmZOffsetSameOne==true)                      //加速 offset 讀取時間
            {
                for(int j=0; j<OutArm2Suck.iMotCol; j++)
                {
                    SortArmOffSet_File[i]->SingleOffSet->dPosOffSetX[0][j]  =0.0;
                    SortArmOffSet_File[i]->SingleOffSet->dPosOffSetY[0][j]  =0.0;
                    SortArmOffSet_File[i]->SingleOffSet->dPickUpOffSet[0][j]=0.0;
                    SortArmOffSet_File[i]->SingleOffSet->dPlaceOffSet[0][j] =0.0;
                }
            }
            else
            {
                for(int j=0; j<OutArm2Suck.iMotCol; j++)                        //吸嘴只有Aa、Ab，定義A、C(左+右)
                {
                    SortArmOffSet_File[i]->SingleOffSet->dPosOffSetX[0][j]  =ReadIniData(szDir, CapStrSortput[i], str.sprintf("Hand %cX",  'A'+(j*2)), 0.0);
                    SortArmOffSet_File[i]->SingleOffSet->dPosOffSetY[0][j]  =ReadIniData(szDir, CapStrSortput[i], str.sprintf("Hand %cY",  'A'+(j*2)), 0.0);
                    SortArmOffSet_File[i]->SingleOffSet->dPickUpOffSet[0][j]=ReadIniData(szDir, CapStrSortput[i], str.sprintf("PickUp %c", 'A'+(j*2)), 0.0);
                    SortArmOffSet_File[i]->SingleOffSet->dPlaceOffSet[0][j] =ReadIniData(szDir, CapStrSortput[i], str.sprintf("Place %c",  'A'+(j*2)), 0.0);
                }
            }
            Offset_File.dSortArmPickUp[i]=ReadIniData(szDir, CapStrSortput[i], "PickUp",     0.0);        //新增SECS用
        }
    }

    //Test Arm
    for(int i=0; i<2; i++)
    {
        str.sprintf("Test Arm%d", i+1);                                         //Steven 20140825 : i --> i+1
        Offset_File.iIndexArmPickUp[i] =ReadIniData(szDir, str, "Pick Up",      0.0);
        Offset_File.iIndexArmPlace[i]  =ReadIniData(szDir, str, "Place",        0.0);
        Offset_File.iIndexArmContact[i]=ReadIniData(szDir, str, "Contact",      0.0);
    }

    //Tray Arm
    for(int i=0; i<=tOfsAuto6; i++)
    {
        if(AUTO_EMPTY_COLOR<3 &&                                            //Steven 20230907 : For HT-9011UC
           (i==tOfsAuto4 || i==tOfsAuto5 || i==tOfsAuto6))
        {
            continue;
        }

        Offset_File.iTrayArmX[i]     =ReadIniData(szDir, SpecialOffSetName[i], "Place", 0.0);
        Offset_File.iTrayArmX_ART[i] =ReadIniData(szDir, SpecialOffSetName[i], "ART-Place", 0.0); //kevin 20170831 (Steven) ART offset
        Offset_File.dTrayZseparate[i]=ReadIniData(szDir, SpecialOffSetName[i], "Loader Z", 0.0);  //Steven 20190813 : 入Tray改用步進馬達
    }

    if(CosFunction.bSaveOffsetByMachine && IniConfig.bA57_3SaveOffsetByMachine) //JimmyChiu 20220618 : save by machine
    {
        szDir=sSaveByMachine;
    }
    else
    {
        szDir=fOffSet->GetOffsetPath();                                         //Steven 20190109 : 整合Offset路徑
    }

    if(Tri_Temp_Machine==1)                                                     //Ztex 2024.03.13 Add Low Temp Offset
        szDir+=Tri_Position_Offset();
    else if(LastSet.iTemperature==Tempture_Hot)
        szDir+="\\Position Offset Hot.Data";
    else
        szDir+="\\Position Offset.Data";

    for(int i=0; i<2; i++)
    {
        InArmOffSet_File[InOfsInSh1+i]->SetX    (ReadIniData(szDir, CapStrInput[InOfsInSh1+i], "Hand X",    0.0));
        InArmOffSet_File[InOfsInSh1+i]->SetY    (ReadIniData(szDir, CapStrInput[InOfsInSh1+i], "Hand Y",    0.0));

        OutArmOffSet_File[OutOfsOutSh1+i]->SetX (ReadIniData(szDir, CapStrOutput[OutOfsOutSh1+i], "Hand X",     0.0));  //Steven 20140510
        OutArmOffSet_File[OutOfsOutSh1+i]->SetY (ReadIniData(szDir, CapStrOutput[OutOfsOutSh1+i], "Hand Y",     0.0));
    }

    for(int i=InOfsInSh1LB; i<=InOfsInSh2RB; i++)                               //Steven 20141006 : Fixed Offset
    {
        InArmOffSet_File[i]->SetX    (ReadIniData(szDir, CapStrInput[i], "Hand X",    0.0));
        InArmOffSet_File[i]->SetY    (ReadIniData(szDir, CapStrInput[i], "Hand Y",    0.0));
    }

    for(int i=OutOfsOutSh1LB; i<=OutOfsOutSh2RB; i++)                           //Steven 20190802 : For 32 Site Out Shuttle Offset
    {
        OutArmOffSet_File[i]->SetX    (ReadIniData(szDir, CapStrOutput[i], "Hand X",    0.0));
        OutArmOffSet_File[i]->SetY    (ReadIniData(szDir, CapStrOutput[i], "Hand Y",    0.0));
    }

    //Test Arm
    for(int i=0; i<2; i++)
    {
        str.sprintf("Test Arm%d", i+1);                                         //Steven 20140825 : i --> i+1
        Offset_File.iSHHalft[i]         =ReadIniData(szDir, str, "Busy SH Halt",  0.0);
        Offset_File.iSHRightPod[i]      =ReadIniData(szDir, str, "Shuttle Right", 0.0);
        Offset_File.iSHLeftPod[i]       =ReadIniData(szDir, str, "Shuttle Left" , 0.0);
        Offset_File.iSHLeft2D[i]        =ReadIniData(szDir, str, "Shuttle for 2D",0.0);      //Steven 20151218 : Offset for 2d reader
    }

    Offset_File.iPreciserOpen=ReadIniData(szDir, "Preciser", "Preciser Open",  0.0);    //Frank 20180410 (Steven) : InArm Preciser Station
    Offset_File.iPreciserClose=ReadIniData(szDir, "Preciser", "Preciser Close",  0.0);

    if(CosFunction.bUseInvisibleOffset)                                         //JerryYang 20250120 : add
    {
        szDir2="D:\\HT9045\\IniData\\DefineOffset\\STD_125.Data";               //強制產生125檔案
        if(FileExists(szDir2)==false)
        {
            ReadInvisibleFile(szDir2);
        }

        szDir2="D:\\HT9045\\IniData\\DefineOffset\\STD_25.Data";                //強制產生25檔案
        if(FileExists(szDir2)==false)
        {
            ReadInvisibleFile(szDir2);
        }

        szDir2.sprintf("D:\\HT9045\\data\\");
        if(!(DirectoryExists(szDir2)))
            ForceDirectories(szDir2);

        szDir2.sprintf("D:\\HT9045\\data\\%s.ini",fMain->cbSetupFileName->Text);
        ReadInvisibleFile(szDir2);
    }

    LoadLoaderScaleValues();
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::DoIniDataToForm(int iNowOffsetSel, int SpecialMode)
{
    int iNum;

    if(iNowOffsetSel<0)                                                         //jou 2011-11-22 防止讀取到unknow記憶體,iNowOffsetSel=-1
    {
        return;
    }

    ShowOffSetList();
    if(SpecialMode==iSaveStander)
    {
        if(iNowOffsetSel==OfsRotate_In          || iNowOffsetSel==OfsHP1                || iNowOffsetSel==OfsHP2                ||
           iNowOffsetSel==OfsAutoClean          || iNowOffsetSel==OfsOCR                || iNowOffsetSel==OfsInPlacement        ||
           iNowOffsetSel==OfsLoader             || iNowOffsetSel==OfsLoaderB            ||
           iNowOffsetSel==OfsAutoSh1            || iNowOffsetSel==OfsAutoSh2            || iNowOffsetSel==OfsPreciser           || iNowOffsetSel==OfsBottom2DID         ||
           iNowOffsetSel==OfsInSh1              || iNowOffsetSel==OfsInSh1LB            || iNowOffsetSel==OfsInSh1RA            || iNowOffsetSel==OfsInSh1RB            ||
           iNowOffsetSel==OfsInSh2              || iNowOffsetSel==OfsInSh2LB            || iNowOffsetSel==OfsInSh2RA            || iNowOffsetSel==OfsInSh2RB            ||
           iNowOffsetSel==OfsInSh1_AutoClean    || iNowOffsetSel==OfsInSh1LB_AutoClean  || iNowOffsetSel==OfsInSh1RA_AutoClean  || iNowOffsetSel==OfsInSh1RB_AutoClean  ||
           iNowOffsetSel==OfsInSh2_AutoClean    || iNowOffsetSel==OfsInSh2LB_AutoClean  || iNowOffsetSel==OfsInSh2RA_AutoClean  || iNowOffsetSel==OfsInSh2RB_AutoClean)
        {
            if(iNowOffsetSel==OfsOCR)                                           //OCR
                CheckBox1->Checked      =true;

            edArmX->Text                =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[iNowOffsetSel]]->GetX());
            edArmY->Text                =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[iNowOffsetSel]]->GetY());
            edPickUp->Text              =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[iNowOffsetSel]]->GetPickUp());
            edRelease->Text             =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[iNowOffsetSel]]->GetPlace());
            edPitchX1->Text             =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[iNowOffsetSel]]->GetVariable());

            if(USE_IN_Y_IS_AUTO_PITCH==true ||              //JerryYang 20251218 : IN/OUT ARM支援不同模組
               USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker)
            {
                edPitchX2->Text         =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[iNowOffsetSel]]->GetVariable2());
            }

            if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||
               USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                         //Ztex 2023.12.25 for HT-1032 AT
            {
                edPitchX3->Text         =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[iNowOffsetSel]]->GetVariable3());
                edPitchX4->Text         =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[iNowOffsetSel]]->GetVariable4());
            }

            if(USE_IN_Y_IS_AUTO_PITCH==true)                //JerryYang 20251218 : IN/OUT ARM支援不同模組
            {
                edPitchY->Text          =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[iNowOffsetSel]]->GetVariableY());
            }

            for(int i=0; i<InArmSuck.iMotRow; i++)
            {
                for(int j=0; j<InArmSuck.iMotCol; j++)
                {
                    //MyXOffsetEdit[i][j]->Text  =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[iNowOffsetSel]]->SingleOffSet->dPosOffSetX[i][j]);
                    //MyYOffsetEdit[i][j]->Text  =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[iNowOffsetSel]]->SingleOffSet->dPosOffSetY[i][j]);
                    //==>
                    MySingleOffsetTEditX[i][j]->Text  =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[iNowOffsetSel]]->SingleOffSet->dPosOffSetX[i][j]);  //Eastsun 20251230 : 修復獨立offset功能
                    MySingleOffsetTEditY[i][j]->Text  =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[iNowOffsetSel]]->SingleOffSet->dPosOffSetY[i][j]);

                    MyPickEdit[i][j]->Text     =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[iNowOffsetSel]]->SingleOffSet->dPickUpOffSet[i][j]);
                    MyRelsEdit[i][j]->Text     =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[iNowOffsetSel]]->SingleOffSet->dPlaceOffSet[i][j]);
                }
            }

            if(iNowOffsetSel==OfsPreciser)                                                  //wei 20240507 避免存檔顯示錯誤
            {
                edPreciserOpen->Text    =FormatFloat("0.00", Offset_File.iPreciserOpen);    //JerryYang 20180820 add
                edPreciserClose->Text   =FormatFloat("0.00", Offset_File.iPreciserClose);
            }
        }
        else if(iNowOffsetSel==OfsOutSh1        || iNowOffsetSel==OfsOutSh1LB || iNowOffsetSel==OfsOutSh1RA || iNowOffsetSel==OfsOutSh1RB   ||
                iNowOffsetSel==OfsOutSh2        || iNowOffsetSel==OfsOutSh2LB || iNowOffsetSel==OfsOutSh2RA || iNowOffsetSel==OfsOutSh2RB   ||
                iNowOffsetSel==OfsAuto1         || iNowOffsetSel==OfsAuto2    || iNowOffsetSel==OfsAuto3    || iNowOffsetSel==OfsAuto4      || iNowOffsetSel==OfsAuto5  || iNowOffsetSel==OfsAuto6 ||
                iNowOffsetSel==OfsFix1          || iNowOffsetSel==OfsFix2     || iNowOffsetSel==OfsFix3     || iNowOffsetSel==OfsFix4       || iNowOffsetSel==OfsFix5   || iNowOffsetSel==OfsFix6  ||
                iNowOffsetSel==OfsRotate_Out    || iNowOffsetSel==OfsTopView  || iNowOffsetSel==OfsPADView  || iNowOffsetSel==OfsBGAView    || iNowOffsetSel==OfsScanAOI)
        {
            edArmX->Text                =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[iNowOffsetSel]]->GetX());
            edArmY->Text                =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[iNowOffsetSel]]->GetY());
            edPickUp->Text              =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[iNowOffsetSel]]->GetPickUp());
            edRelease->Text             =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[iNowOffsetSel]]->GetPlace());
            edPitchX1->Text             =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[iNowOffsetSel]]->GetVariable());

            if(USE_OUT_Y_IS_AUTO_PITCH==true ||     //JerryYang 20251218 : IN/OUT ARM支援不同模組
               USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker)
            {
                edPitchX2->Text         =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[iNowOffsetSel]]->GetVariable2());
            }

            if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||
               USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                         //Ztex 2023.12.25 For HT-1032 AT
            {
                edPitchX3->Text         =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[iNowOffsetSel]]->GetVariable3());
                edPitchX4->Text         =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[iNowOffsetSel]]->GetVariable4());
            }

            if(USE_OUT_Y_IS_AUTO_PITCH==true)           //JerryYang 20251218 : IN/OUT ARM支援不同模組
            {
                edPitchY->Text          =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[iNowOffsetSel]]->GetVariableY());   //ChungHung 20131231 alter AutoYPitch
            }

            for(int i=0; i<OutArmSuck.iMotRow; i++)
            {
                for(int j=0; j<OutArmSuck.iMotCol; j++)
                {
                    //MyXOffsetEdit[i][j]->Text  =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[iNowOffsetSel]]->SingleOffSet->dPosOffSetX[i][j]);
                    //MyYOffsetEdit[i][j]->Text  =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[iNowOffsetSel]]->SingleOffSet->dPosOffSetY[i][j]);
                    //==>
                    MySingleOffsetTEditX[i][j]->Text  =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[iNowOffsetSel]]->SingleOffSet->dPosOffSetX[i][j]);  //Eastsun 20251230 : 修復獨立offset功能
                    MySingleOffsetTEditY[i][j]->Text  =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[iNowOffsetSel]]->SingleOffSet->dPosOffSetY[i][j]);

                    MyPickEdit[i][j]->Text     =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[iNowOffsetSel]]->SingleOffSet->dPickUpOffSet[i][j]);
                    MyRelsEdit[i][j]->Text     =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[iNowOffsetSel]]->SingleOffSet->dPlaceOffSet[i][j]);
                }
            }
        }
    }
    else  //Tray Arm Offset
    {
        if(iNowOffsetSel<=tOfsAuto6)
        {
            edtTrayArmX->Text=FormatFloat("0.00", Offset_File.iTrayArmX[iNowOffsetSel]);
            Panel2->Visible=true;
            Panel3->Visible=false;
            edtARTPlace->Text=FormatFloat("0.00", Offset_File.iTrayArmX_ART[iNowOffsetSel]); //kevin 20170831 (Steven) ART offset
            if(USE_AUTO_RETEST==eartInstall &&
               (IniConfig.bA10_AutoReTest || bAutoReTest_ART) &&                //kevin 20170831 (Steven) add ART tray arm offset
               (LastSet.iRunStartMode==rsmInitial_ART       ||
                LastSet.iRunStartMode==rsmContinuStart_ART  ||
                LastSet.iRunStartMode==rsmContinuRetest_ART ||
                LastSet.iRunStartMode==rsmAutoRetest))
            {
                pnlTrayXART->Visible=true;
            }
            else
            {
                pnlTrayXART->Visible=false;                                     //Ztex 2024.03.19 Add HT-1032AT
            }
            edlLoadZ->Text=FormatFloat("0.00", Offset_File.dTrayZseparate[iNowOffsetSel]);   //Steven 20190813 : 入Tray改用步進馬達
            edlLoadZ->Visible=(LOAD_Z_USE_MOTOR[iNowOffsetSel] || TRAY_ARM_MODE==eUnderCoveyor);//Ztex 2024.03.19 Add HT-1032AT
            lblLoadZ->Visible=(LOAD_Z_USE_MOTOR[iNowOffsetSel] || TRAY_ARM_MODE==eUnderCoveyor);//Ztex 2024.03.19 Add HT-1032AT
        }
        else
        {
            pnlTrayXART->Visible=false;
            iNum=iNowOffsetSel-tOfsIndex1;
            if(iNum>1)
                ShowErrorMessage("WAR1681", 0, MMSystem, 0, "ReadOffset");

            IndexArmOffSet1->Text=FormatFloat("0.00", Offset_File.iIndexArmPickUp[iNum]);
            IndexArmOffSet2->Text=FormatFloat("0.00", Offset_File.iIndexArmPlace[iNum]);
            IndexArmOffSet3->Text=FormatFloat("0.00", Offset_File.iIndexArmContact[iNum]);
            IndexArmOffSet4->Text=FormatFloat("0.00", Offset_File.iSHHalft[iNum]);
            IndexArmOffSet5->Text=FormatFloat("0.00", Offset_File.iSHRightPod[iNum]);
            IndexArmOffSet6->Text=FormatFloat("0.00", Offset_File.iSHLeftPod[iNum]);
            edShtFor2D->Text     =FormatFloat("0.00", Offset_File.iSHLeft2D[iNum]);  //Steven 20151218 : Offset for 2d reader
            Panel2->Visible=false;
            Panel3->Visible=true;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::SpBotSelClick(TObject *Sender)
{
    AnsiString S;
    if(bOffsetEnterBarcode==false)
    {
        if(Barcode_Reader(bcOffset)==0)                                         // 20140103 wei KYEC Barcode Reader
        {
            return;
        }
    }
    TSpeedButton *Ptr=(TSpeedButton *)Sender;
    bool bResult=SaveFile(iNowOffsetSel, iSaveStander, false);
    if(bResult)
    {
        Ptr->Down=false;
        if(iNowOffsetSel!=-1)
            OffSetSelBot[iNowOffsetSel]->Down=true;
        iNowOffsetSel=iNowOffsetSel;
    }
    else
    {
        iNowOffsetSel=Ptr->Tag;
    }

    if(Ptr->Tag==OfsRotate_In && USE_DIE_CLEAN!=0)
        palOffsetParts->Caption="Die Clean";
    else
        palOffsetParts->Caption=CapStr[iNowOffsetSel];

    if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||                              //Steven 20141029 : XY-Pitch
       USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                            //Ztex 2024.02.24 Add HT-1132
    {
        if((iNowOffsetSel>=OfsOutSh1 && iNowOffsetSel<=OfsFix3) ||
            iNowOffsetSel==OfsRotate_Out    ||
            iNowOffsetSel==OfsTopView       ||
            iNowOffsetSel==OfsPADView       ||                                  //wei 20160617 Vitrox
            iNowOffsetSel==OfsBGAView       ||                                  //wei 20160617 Vitrox
            iNowOffsetSel==OfsScanAOI       )
        {
            LoadImage(false);                                                   //Steven 20141029 : XY-Pitch
        }
        else
        {
            LoadImage(true);
        }
    }
    else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||
            USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                            //Ztex 2023.12.06 Add HT-1032
    {
        if(iNowOffsetSel==OfsAuto1  ||
           iNowOffsetSel==OfsAuto2  ||
           iNowOffsetSel==OfsAuto3  ||
           iNowOffsetSel==OfsAuto4  ||
           iNowOffsetSel==OfsAuto5  ||
           iNowOffsetSel==OfsAuto6  ||
           iNowOffsetSel==OfsFix1   ||
           iNowOffsetSel==OfsFix2   ||
           iNowOffsetSel==OfsFix3   ||
           iNowOffsetSel==OfsFix4   ||
           iNowOffsetSel==OfsFix5   ||
           iNowOffsetSel==OfsFix6   ||
           iNowOffsetSel==OfsOutSh1 ||
           iNowOffsetSel==OfsOutSh2 ||
           iNowOffsetSel==OfsOutSh1RA ||
           iNowOffsetSel==OfsOutSh2RA)
            LoadImage(false);
        else
            LoadImage(true);
    }

    int iZLimitHigh;                                                            //Steven 20210317 : 通富微要求unloader獨立設置
    int iZLimitLow;
    if(iNowOffsetSel==OfsAuto1 ||
       iNowOffsetSel==OfsAuto2 ||
       iNowOffsetSel==OfsAuto3 ||
       iNowOffsetSel==OfsAuto4 ||                                               //Steven 20230907 : For HT-9011UC
       iNowOffsetSel==OfsAuto5 ||
       iNowOffsetSel==OfsAuto6 ||
       iNowOffsetSel==OfsFix1  ||
       iNowOffsetSel==OfsFix2  ||
       iNowOffsetSel==OfsFix3  ||
       iNowOffsetSel==OfsFix4  ||                                               //Steven 20230907 : For HT-9011UC
       iNowOffsetSel==OfsFix5  ||
       iNowOffsetSel==OfsFix6)
    {
        iZLimitHigh=InputLimit.iOffsetUnloaderZHigh;
        iZLimitLow =InputLimit.iOffsetUnloaderZLow;
    }
    else
    {
        iZLimitHigh=InputLimit.iOffsetZHigh;
        iZLimitLow =InputLimit.iOffsetZLow;
    }

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //kevin 20211111 add 1 ~ -1
    {
        if(iNowOffsetSel==OfsInSh1 || iNowOffsetSel==OfsInSh2)
        {
            S.sprintf("%d~%dmm", 1, -1);
        }
        else
        {
            S.sprintf("%d~%dmm", iZLimitHigh, iZLimitLow);
        }
    }
    else
    {
        S.sprintf("%d~%dmm", iZLimitHigh, iZLimitLow);
    }
    lblReleaseRange->Caption=S;
    lblPickUpRange->Caption=S;

    ShowOneByOneOffSet();
    ReadFile();
    DoIniDataToForm(iNowOffsetSel, iSaveStander);
    Ptr->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::IndexOffSetBT2Click(TObject *Sender)
{
    TSpeedButton *Ptr=(TSpeedButton *)Sender;
    pnlIndexOffset->Caption=SpecialOffSetName[Ptr->Tag];
    SaveFile(iSpecialOffSetSel, iSaveSpecial, false);
    iSpecialOffSetSel=Ptr->Tag;
    ReadFile();
    DoIniDataToForm(iSpecialOffSetSel, iSaveSpecial);
    Ptr->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::edArmXMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(IniConfig.bSPILFunction)                                                 //JerryYang 20220923 : 矽品蘇州要求offset limit要By區域設定
    {
        if(iNowOffsetSel==OfsOCR)   //JerryYang 20240111 : add
        {
            fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 100.0, -100.0);
        }
        else if(palOffsetParts->Caption==CapStr[OfsLoader])
        {
            fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, (double)InputLimit.dLoaderOffsetXYHigh, (double)InputLimit.dLoaderOffsetXYLow);
        }
        else if(palOffsetParts->Caption==CapStr[OfsHP1] || palOffsetParts->Caption==CapStr[OfsHP2])
        {
            fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, (double)InputLimit.dHPOffsetXYHigh, (double)InputLimit.dHPOffsetXYLow);
        }
        else if(palOffsetParts->Caption==CapStr[OfsInSh1]   || palOffsetParts->Caption==CapStr[OfsInSh2]   ||
                palOffsetParts->Caption==CapStr[OfsInSh1LB] || palOffsetParts->Caption==CapStr[OfsInSh1RA] ||
                palOffsetParts->Caption==CapStr[OfsInSh1RB] || palOffsetParts->Caption==CapStr[OfsInSh2LB] ||
                palOffsetParts->Caption==CapStr[OfsInSh2RA] || palOffsetParts->Caption==CapStr[OfsInSh2RB])
        {
            fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, (double)InputLimit.dInShtOffsetXYHigh, (double)InputLimit.dInShtOffsetXYLow);
        }
        else if(palOffsetParts->Caption==CapStr[OfsOutSh1]   || palOffsetParts->Caption==CapStr[OfsOutSh1]   ||
                palOffsetParts->Caption==CapStr[OfsOutSh1LB] || palOffsetParts->Caption==CapStr[OfsOutSh1RB] ||
                palOffsetParts->Caption==CapStr[OfsOutSh1RB] || palOffsetParts->Caption==CapStr[OfsOutSh2LB] ||
                palOffsetParts->Caption==CapStr[OfsOutSh2RA] || palOffsetParts->Caption==CapStr[OfsOutSh2RB])
        {
            fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, (double)InputLimit.dOutShtOffsetXYHigh, (double)InputLimit.dOutShtOffsetXYLow);
        }
        else if(palOffsetParts->Caption==CapStr[OfsAuto1] || palOffsetParts->Caption==CapStr[OfsAuto2] || palOffsetParts->Caption==CapStr[OfsAuto3] ||
                palOffsetParts->Caption==CapStr[OfsAuto4] || palOffsetParts->Caption==CapStr[OfsAuto5] || palOffsetParts->Caption==CapStr[OfsAuto6] ||
                palOffsetParts->Caption==CapStr[OfsFix1]  || palOffsetParts->Caption==CapStr[OfsFix2]  || palOffsetParts->Caption==CapStr[OfsFix3]  ||
                palOffsetParts->Caption==CapStr[OfsFix4]  || palOffsetParts->Caption==CapStr[OfsFix5]  || palOffsetParts->Caption==CapStr[OfsFix6] )
        {
            fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, (double)InputLimit.dUnloadOffsetXYHigh, (double)InputLimit.dUnloadOffsetXYLow);
        }
        else
        {
            fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, (double)InputLimit.iOffsetXYHigh, (double)InputLimit.iOffsetXYLow);
        }
    }
    else if(iNowOffsetSel==OfsOCR)
    {
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 100.0, -100.0);
    }
    else
    {
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, (double)InputLimit.iOffsetXYHigh, (double)InputLimit.iOffsetXYLow);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::edReleaseMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int iPick1, iRelease1, i;
    double fBuf;
    TEdit *Buffer;
    Buffer=(TEdit *)Sender;
    //Steven 20210317 : 通富微要求unloader獨立設置
    //==>
    int iZLimitHigh;
    int iZLimitLow;
    if(iNowOffsetSel==OfsAuto1 ||
       iNowOffsetSel==OfsAuto2 ||
       iNowOffsetSel==OfsAuto3 ||
       iNowOffsetSel==OfsAuto4 ||                                               //Steven 20230907 : For HT-9011UC
       iNowOffsetSel==OfsAuto5 ||
       iNowOffsetSel==OfsAuto6 ||
       iNowOffsetSel==OfsFix1  ||
       iNowOffsetSel==OfsFix2  ||
       iNowOffsetSel==OfsFix3  ||
       iNowOffsetSel==OfsFix4  ||                                               //Steven 20230907 : For HT-9011UC
       iNowOffsetSel==OfsFix5  ||
       iNowOffsetSel==OfsFix6)
    {
        iZLimitHigh=InputLimit.iOffsetUnloaderZHigh;
        iZLimitLow =InputLimit.iOffsetUnloaderZLow;
    }
    else
    {
        iZLimitHigh=InputLimit.iOffsetZHigh;
        iZLimitLow =InputLimit.iOffsetZLow;
    }
    //<==
    //Steven 20210317 : 通富微要求unloader獨立設置

    //Steven 20090805 : Z Offset
    if(iNowOffsetSel==OfsRotate_In || iNowOffsetSel==OfsRotate_Out)             //In Rotate & Out Rotate
    {
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 5.0, -5.0);       //Steven 20141120 : Modify
    }
    else
    {
        if(IniConfig.bChangeKitNoHardStop && PageControl1->ActivePageIndex==1 &&//jou 2015-12-08 Xilinx 驗證用
           (pnlIndexOffset->Caption==SpecialOffSetName[tOfsIndex1] ||
            pnlIndexOffset->Caption==SpecialOffSetName[tOfsIndex2]))            //Frank 20171030 (Steven) add Floating Shuttle調整Offset
        {
            fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, (double)InputLimit.dShuttleHigh, (double)InputLimit.dShuttleLow);
        }
        else
        {
            if((pnlIndexOffset->Caption==SpecialOffSetName[tOfsIndex1] ||
                pnlIndexOffset->Caption==SpecialOffSetName[tOfsIndex2]) &&
               CUSTOMER_CODE==CC_KYEC_LEE &&                                    //Ifor 20190919 : add KYEC 要求Index Pickup Offset -0.5~0.5 mm
               Buffer->Tag==99)
            {
                fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, (double)10, (double)-0.5);       //Steven 20141120 : Modify
            }
            else
            {
                if(IniConfig.bSPILFunction)                                     //JerryYang 20220923 : 矽品蘇州要求offset limit要By區域設定
                {
                    if(palOffsetParts->Caption==CapStr[OfsLoader])
                    {
                        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, (double)InputLimit.dLoaderOffsetZHigh, (double)InputLimit.dLoaderOffsetZLow);
                    }
                    else if(palOffsetParts->Caption==CapStr[OfsHP1] || palOffsetParts->Caption==CapStr[OfsHP2])
                    {
                        if(Buffer->Name=="edPickUp")
                        {
                            fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, (double)InputLimit.dHPOffsetZHigh, (double)InputLimit.dHPOffsetZLow);
                        }
                        else
                        {
                            fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, (double)InputLimit.dHPOffsetZRelHigh, (double)InputLimit.dHPOffsetZRelLow);
                        }
                    }
                    else if(palOffsetParts->Caption==CapStr[OfsInSh1]    || palOffsetParts->Caption==CapStr[OfsInSh2]    ||
                            palOffsetParts->Caption==CapStr[OfsOutSh1LB] || palOffsetParts->Caption==CapStr[OfsOutSh1RB] ||
                            palOffsetParts->Caption==CapStr[OfsOutSh1RB] || palOffsetParts->Caption==CapStr[OfsOutSh2LB] ||
                            palOffsetParts->Caption==CapStr[OfsOutSh2RA] || palOffsetParts->Caption==CapStr[OfsOutSh2RB])
                    {
                        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, (double)InputLimit.dInShtOffsetZHigh, (double)InputLimit.dInShtOffsetZLow);
                    }
                    else if(palOffsetParts->Caption==CapStr[OfsOutSh1]   || palOffsetParts->Caption==CapStr[OfsOutSh2]   ||
                            palOffsetParts->Caption==CapStr[OfsOutSh1LB] || palOffsetParts->Caption==CapStr[OfsOutSh1RB] ||
                            palOffsetParts->Caption==CapStr[OfsOutSh1RB] || palOffsetParts->Caption==CapStr[OfsOutSh2LB] ||
                            palOffsetParts->Caption==CapStr[OfsOutSh2RA] || palOffsetParts->Caption==CapStr[OfsOutSh2RB])
                    {
                        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, (double)InputLimit.dOutShtOffsetZHigh, (double)InputLimit.dOutShtOffsetZLow);
                    }
                    else if(palOffsetParts->Caption==CapStr[OfsAuto1] || palOffsetParts->Caption==CapStr[OfsAuto2] || palOffsetParts->Caption==CapStr[OfsAuto3] ||
                            palOffsetParts->Caption==CapStr[OfsAuto4] || palOffsetParts->Caption==CapStr[OfsAuto5] || palOffsetParts->Caption==CapStr[OfsAuto6] ||
                            palOffsetParts->Caption==CapStr[OfsFix1]  || palOffsetParts->Caption==CapStr[OfsFix2]  || palOffsetParts->Caption==CapStr[OfsFix3]  ||
                            palOffsetParts->Caption==CapStr[OfsFix4]  || palOffsetParts->Caption==CapStr[OfsFix5]  || palOffsetParts->Caption==CapStr[OfsFix6] )
                    {
                        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, (double)InputLimit.dUnloadOffsetZHigh, (double)InputLimit.dUnloadOffsetZLow);
                    }
                    else
                    {
                        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, (double)InputLimit.iOffsetZHigh, (double)InputLimit.iOffsetZLow);
                    }
                }
                else
                {
                    if(Buffer==edRelease || Buffer==EdtRelsA || Buffer==EdtRelsB || Buffer==EdtRelsC ||
                       Buffer==EdtRelsD  || Buffer==EdtRelsE || Buffer==EdtRelsF || Buffer==EdtRelsG || Buffer==EdtRelsH)
                        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, (double)iZLimitHigh, (double)iZLimitLow);
                    else
                        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, (double)InputLimit.iOffsetZHigh, (double)InputLimit.iOffsetZLow);       //Steven 20141120 : Modify
                }
            }
        }
    }

    if(pnlIndexOffset->Caption==SpecialOffSetName[tOfsIndex1] ||
       pnlIndexOffset->Caption==SpecialOffSetName[tOfsIndex2])
    {
        if(pnlIndexOffset->Caption==SpecialOffSetName[tOfsIndex1])
            i=0;
        else
            i=1;

        iPick1      =DeviceForm.IndexArmPick[i]+atof(IndexArmOffSet1->Text.c_str())*100;
        iRelease1   =DeviceForm.IndexPlace[i]+atof(IndexArmOffSet2->Text.c_str())*100;
        if(iRelease1<iPick1)
        {
            fBuf=iPick1-DeviceForm.IndexPlace[i];
            IndexArmOffSet2->Text=fBuf/100.0;
            lblShowMessage->Caption="Relase高度低於Pick高度 ; Relase high below the Pick high ";
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::CheckBox1Click(TObject *Sender)
{
    ShowOneByOneOffSet();
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::edArmXKeyPress(TObject *Sender, char &Key)
{
    if(OnlyNumberAndDotInPut(Key)==false)   Key=NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::sbtExitClick(TObject *Sender)
{
    sbtExit->Down=false;
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::spbSaveClick(TObject *Sender)
{
    AnsiString S="";
    spbSave->Down=false;
    fMain->iOperatorModeCount=0;                                                //Ifor 20151027 時間到登出
    if(PageControl1->ActivePageIndex==0)
    {
        SaveFile(iNowOffsetSel, iSaveStander, false);
        bEnterSpecialOffset=false;
    }
    else
    {
        SaveFile(iSpecialOffSetSel, iSaveSpecial, false);
        bEnterSpecialOffset=true;
    }

    ReadFile();
    bEnterOffset=true;

    if(CUSTOMER_CODE==CC_ASE_CL)
    {
        if(LastSet.iTemperature==Tempture_Ambient)
        {
            SetTempShiftOffsetPeriod(25,fMain->cbSetupFileName->Text);
        }
        else
        {
            SetTempShiftOffsetPeriod(Temperature.fWorkTemperBase, fMain->cbSetupFileName->Text);
        }

        S.sprintf("D:\\HT9045\\data\\%s.ini", fMain->cbSetupFileName->Text);
//
    WriteIniData(S,"LoaderScale","ScanleEnable",fOffSet->chkInArmScakeEnable->Checked);
//    WriteIniData(S,"LoaderScale","LoaderX",fOffSet->edLodX->Text);
//    WriteIniData(S,"LoaderScale","LoaderY",fOffSet->edLodY->Text);
//    WriteIniData(S,"LoaderScale","HotPlate1X",fOffSet->edt_HP1X->Text);
//    WriteIniData(S,"LoaderScale","HotPlate1Y",fOffSet->edt_HP1Y->Text);
//    WriteIniData(S,"LoaderScale","HotPlate2X",fOffSet->edt_HP2X->Text);
//    WriteIniData(S,"LoaderScale","HotPlate2Y",fOffSet->edt_HP2Y->Text);
        WriteIniData(S,"LoaderScale","Shuttle1X",fOffSet->ed_IS1X->Text);
        WriteIniData(S,"LoaderScale","Shuttle1Y",fOffSet->ed_IS1Y->Text);
        WriteIniData(S,"LoaderScale","Shuttle2X",fOffSet->ed_IS2X->Text);
        WriteIniData(S,"LoaderScale","Shuttle2Y",fOffSet->ed_IS2Y->Text);
//
        WriteIniData(S,"UnLoaderScale","ScanleEnable",fOffSet->chkOutArmScakeEnable->Checked);
        WriteIniData(S,"UnLoaderScale","Shuttle1X",fOffSet->ed_OS1X->Text);
        WriteIniData(S,"UnLoaderScale","Shuttle1Y",fOffSet->ed_OS1Y->Text);
        WriteIniData(S,"UnLoaderScale","Shuttle2X",fOffSet->ed_OS2X->Text);
        WriteIniData(S,"UnLoaderScale","Shuttle2Y",fOffSet->ed_OS2Y->Text);

        chkInArmScakeEnable->Checked=TestIF_File.bInArmUseDifferentScaleBySetupFile;

        edLodX->Text=TestIF_File.fLoaderTrayXScaleBySetupFile;
        edLodY->Text=TestIF_File.fLoaderTrayYScaleBySetupFile;
        edt_HP1X->Text=TestIF_File.fHotPlateXScaleBySetupFile[0];
        edt_HP1Y->Text=TestIF_File.fHotPlateYScaleBySetupFile[0];
        edt_HP2X->Text=TestIF_File.fHotPlateXScaleBySetupFile[1];
        edt_HP2Y->Text=TestIF_File.fHotPlateYScaleBySetupFile[1];

        ed_IS1X->Text=TestIF_File.fInShuttleXScaleBySetupFile[0];
        ed_IS1Y->Text=TestIF_File.fInShuttleYScaleBySetupFile[0];
        ed_IS2X->Text=TestIF_File.fInShuttleXScaleBySetupFile[1];
        ed_IS2Y->Text=TestIF_File.fInShuttleYScaleBySetupFile[1];

        chkOutArmScakeEnable->Checked=TestIF_File.bOutArmUseDifferentScaleBySetupFile;

        ed_OS1X->Text=TestIF_File.fOutShuttleXScaleBySetupFile[0];
        ed_OS1Y->Text=TestIF_File.fOutShuttleYScaleBySetupFile[0];
        ed_OS2X->Text=TestIF_File.fOutShuttleXScaleBySetupFile[1];
        ed_OS2Y->Text=TestIF_File.fOutShuttleYScaleBySetupFile[1];
    }
    bUseUpdate=true;

    fMain->Pause("Save Offset");                                                //jou 2011-02-18
    labWarningForStop->Visible=true;                                            //jou 2011-02-18
    SetWorkParameter();                                                         //Steven 20221122 : 存檔後要重新load參數
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::IndexArmOffSet3MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, InputLimit.dContactHigh, InputLimit.dContactLow); //Steven 20140123 : Contact Height的Offset限制
    iIndexChange=3;                                                             //kevin 20211211 index socket value change
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::sb_AutoOffsetUpClick(TObject *Sender)                 //jou 2013-08-29 Use Auto Offset Funtion start
{
    if(SystemStart)
        return;

    edArmY->Text=FormatFloat("0.00", atof(edArmY->Text.c_str())+0.1);
    spbSaveClick(this);
    if(cb_AutoOffsetPositionCheck->Checked==true)
    {
        fMain->Start("sb_AutoOffsetUpClick");
    }
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::sb_AutoOffsetDownClick(TObject *Sender)
{
    if(SystemStart)
        return;

    edArmY->Text=FormatFloat("0.00", atof(edArmY->Text.c_str())-0.1);
    spbSaveClick(this);
    if(cb_AutoOffsetPositionCheck->Checked==true)
    {
        fMain->Start("sb_AutoOffsetDownClick");
    }
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::sb_AutoOffsetRightClick(TObject *Sender)
{
    if(SystemStart)
        return;

    edArmX->Text=FormatFloat("0.00", atof(edArmX->Text.c_str())+0.1);
    spbSaveClick(this);
    if(cb_AutoOffsetPositionCheck->Checked==true)
    {
        fMain->Start("sb_AutoOffsetRightClick");
    }
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::sb_AutoOffsetLeftClick(TObject *Sender)
{
    if(SystemStart)
        return;

    edArmX->Text=FormatFloat("0.00", atof(edArmX->Text.c_str())-0.1);
    spbSaveClick(this);
    if(cb_AutoOffsetPositionCheck->Checked==true)
    {
        fMain->Start("sb_AutoOffsetLeftClick");
    }
}
//---------------------------------------------------------------------------
bool __fastcall TfOffSet::UseAutoOffsetFunction(AnsiString asWhich)
{
    if(fAutoTeach->IsArmNeedPause(asWhich))                                     //JimmyChiu 20211020 : Auto alignment mode
        return true;
    if(IniConfig.bUseAutoOffsetFunction && fOffSet->fShow)
    {
        if(cb_AutoOffsetPositionCheck->Checked &&
           asWhich==palOffsetParts->Caption)
        {
            return true;
        }
    }
    return false;
}
//---------------------------------------------------------------------------
bool __fastcall TfOffSet::UseInArmSetupTeach(int iArea)                         //JerryYang 20180921 Setup Teach功能
{
    if(CosFunction.bManualSteplAutoTeach &&
       IniConfig.bA56EnableAutoTeachFunciton)                                   //JimmyChiu 20211020 : Auto alignment mode
        return false;

    if(IniConfig.bA30SetupTeachFunction==false)
        return false;
    if(iArea>InOfsInSh2)
        return false;
    if(LastSet.bNeedSetupTeach)
    {
        if(bInArmStop[iArea]==false)
        {
            bInArmStop[iArea]=true;
            return true;
        }
    }
    return false;
}
//---------------------------------------------------------------------------
bool __fastcall TfOffSet::UseOutArmSetupTeach(int iArea)                        //JerryYang 20180921 Setup Teach功能
{
    if(IniConfig.bA30SetupTeachFunction==false)
        return false;

    if(AUTO_EMPTY_COLOR==3)
    {
        if(iArea==OutOfsAuto6 || iArea>OutOfsFix6)
            return false;
    }
    else if(AUTO_EMPTY_COLOR>=4)
    {
        if(iArea>OutOfsFix6)
            return false;
    }
    else if(iArea>OutOfsFix3)
    {
        return false;
    }

    if(LastSet.bNeedSetupTeach)
    {
        if(bOutArmStop[iArea]==false)
        {
            bOutArmStop[iArea]=true;
            return true;
        }
    }
    return false;
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::ck_AOIClick(TObject *Sender)                          //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
{
    if(USE_Top_Scanner_AOI_Inspection==1)                                       //Ifor 20200902 add: TFAMD Top AOI
    {
        if(ck_AOI->Checked)
        {
            Pnl_AOI->Height=81;
        }
        else
        {
            Pnl_AOI->Height=33;
        }
    }

    if(USE_AOI_Inspection==1 || USE_Fix_AI_CCD==1)                              //RogerYang 20180901 add 矽格湖口Demo AI CCD Function
    {
        if(ck_AOI->Checked)
        {
            if(USE_Fix_AI_CCD==1)                                               //RogerYang 20180901 add 矽格湖口Demo AI CCD Function
                Pnl_AOI->Height=80;
            else
                Pnl_AOI->Height=177;
        }
        else
        {
        }
    }

    if(USE_Scanner_AOI_Inspection==(int)eBtnAOI_TopBottomInstall)                    //Jimmychiu 20240322 : Top & Bottom Inspect
    {
        if(ck_AOI->Checked)
        {
            Pnl_AOI->Height = 81;
        }
        else
        {
            Pnl_AOI->Height = 33;
            Pnl_AOI->Height=33;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::IndexArmOffSet5MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, (double)InputLimit.dShuttleHigh, (double)InputLimit.dShuttleLow);  //ChungHung 20150115 add for ATK +/-2 mm
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::edShtFor2DMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    double dMaxDimension;                                                       //Frank 20171030 (Steven) add Floating Shuttle調整Offset
    double dTrayXPitch=0.0;
    double dTrayYPitch=0.0;

    dTrayXPitch=UserDefForm_File[TrayForm.Loader.iTrayType].XPitch;
    dTrayYPitch=UserDefForm_File[TrayForm.Loader.iTrayType].YPitch;

    if(UserDefForm_File[TrayForm.Loader.iTrayType].XDivision==1)                //Ifor 20251017 add:2D Offset改用IC大小判斷
    {
        dMaxDimension=120.0;
    }
    else
    {
        if(dTrayXPitch>dTrayYPitch)
        {
            dMaxDimension=dTrayXPitch;
        }
        else if(dTrayYPitch>dTrayXPitch)
        {
            dMaxDimension=dTrayYPitch;
        }
        else
        {
            dMaxDimension=dTrayXPitch;
        }
    }

    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, dMaxDimension, -(dMaxDimension));
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::edPreciserOpenMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 0.0, true, 0.5, 0.0);  //Frank 20180410 (Steven) : InArm Preciser Station
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::FormDestroy(TObject *Sender)
{
    TimerSetupTeach->Enabled=false;
    LogSoftwareOffTime("TfOffSet, FormDestroy");                                //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::TimerSetupTeachTimer(TObject *Sender)                 //JerryYang 20180921 Setup Teach功能
{
    if(IniConfig.bA30SetupTeachFunction==false)                                 //JimmyChiu 20211020 : Auto alignment mode
        return;

//    TPanel *palInArm[]={palLoader,          //0       //JerryYang 20230523 : 沒在用, Mark掉
//                        palHP1,             //1
//                        palHP2,             //2
//                        palSht1,            //3
//                        palSht2             //4
//                       };
//    TPanel *palOutArm[]={palOutSht1,        //0
//                         palOutSht2,        //1
//                         palAuto1,          //2
//                         palAuto2,          //3
//                         palAuto3,          //4
//                         palFix1,           //5
//                         palFix2,           //6
//                         palFix3            //7
//                        };
//
//    if(LastSet.iTemperature==Tempture_Hot)  //加熱模式
//    {
//        if(HotPlateForm.iPlateSelect==1)
//        {
//            bInArmSetupTeach[InOfsHP1]=false;
//            bInArmSetupTeach[InOfsHP2]=true;
//        }
//        else if(HotPlateForm.iPlateSelect==2)
//        {
//            bInArmSetupTeach[InOfsHP1]=true;
//            bInArmSetupTeach[InOfsHP2]=false;
//        }
//        else
//        {
//            bInArmSetupTeach[InOfsHP1]=false;
//            bInArmSetupTeach[InOfsHP2]=false;
//        }
//    }
//    else
//    {
//        bInArmSetupTeach[InOfsHP1]=true;   //常溫不檢查
//        bInArmSetupTeach[InOfsHP2]=true;
//    }
//
//    if(TestIF_File.iShuttleMode==0)
//    {
//        bInArmSetupTeach[InOfsInSh1]=false;
//        bOutArmSetupTeach[OutOfsOutSh1]=false;
//
//        bInArmSetupTeach[InOfsInSh2]=false;
//        bOutArmSetupTeach[OutOfsOutSh2]=false;
//    }
//    else if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==0) //Shuttle 1
//    {
//        bInArmSetupTeach[InOfsInSh1]=false;
//        bOutArmSetupTeach[OutOfsOutSh1]=false;
//
//        bInArmSetupTeach[InOfsInSh2]=true;
//        bOutArmSetupTeach[OutOfsOutSh2]=true;
//    }
//    else if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1) //Shuttle 2
//    {
//        bInArmSetupTeach[InOfsInSh1]=true;
//        bOutArmSetupTeach[OutOfsOutSh1]=true;
//
//        bInArmSetupTeach[InOfsInSh2]=false;
//        bOutArmSetupTeach[OutOfsOutSh2]=false;
//    }
//
//    for(i=0; i<5; i++)
//    {
//        if(iInArmPickPlaceCnt[i]>=5 || bInArmSetupTeach[i]==true)  //JerryYang 20191007 10->5
//        {
//            palInArm[i]->Caption=asInArm[i]+"_Finish";
//            palInArm[i]->Color=clGreen;
//        }
//        else
//        {
//            palInArm[i]->Caption=asInArm[i];
//            palInArm[i]->Color=clGray;
//        }
//    }
//
//    for(i=0; i<8; i++)
//    {
//        if(iOutArmPickPlaceCnt[i]>=5 || bOutArmSetupTeach[i]==true)  //JerryYang 20191007 10->5
//        {
//            palOutArm[i]->Caption=asOutArm[i]+"_Finish";
//            palOutArm[i]->Color=clGreen;
//        }
//        else
//        {
//            palOutArm[i]->Caption=asOutArm[i];
//            palOutArm[i]->Color=clGray;
//        }
//    }

//    if(LastSet.bNeedSetupTeach==true) //JerryYang 20230523 : 沒在用, Mark掉
//    {
//        if(CheckSetupFinish()==true)
//        {
//            LastSet.bNeedSetupTeach=false;
//        }
//    }
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::btnSortAuto1Click(TObject *Sender)                         //JerryYang 20180921 Setup Teach功能
{
    TButton *Ptr;
    Ptr=(TButton *)Sender;
    if(IniConfig.bA30SetupTeachFunction &&
       LastSet.iTester==OFF_LINE &&
       LastSet.bNeedSetupTeach)
    {
        iSortUnloadT6=Ptr->Tag;
    }
}
//---------------------------------------------------------------------------
bool bhasKeyDown=false;
void __fastcall TfOffSet::IndexArmOffSet1KeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
    if(CUSTOMER_CODE==CC_KYEC_LEE)                                              //Ifor 20190930 add : KYEC Index Offset 讀檔不卡Range，有修改才卡Range
    {
        bhasKeyDown=true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::IndexArmOffSet3KeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
    if(CUSTOMER_CODE==CC_KYEC_LEE)                                              //Ifor 20190930 add : KYEC Index Offset 讀檔不卡Range，有修改才卡Range
    {
        bhasKeyDown=true;
    }
    iIndexChange=1;                                                             //kevin 20211211 index socket value change
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::IndexArmOffSet1KeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    if(CUSTOMER_CODE==CC_KYEC_LEE && bhasKeyDown==true)                         //Ifor 20190930 add : KYEC Index Offset 讀檔不卡Range，有修改才卡Range
    {
        IndexArmOffSet1->Text="0";
        bhasKeyDown=false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::IndexArmOffSet3KeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    if(CUSTOMER_CODE==CC_KYEC_LEE && bhasKeyDown==true)                         //Ifor 20190930 add : KYEC Index Offset 讀檔不卡Range，有修改才卡Range
    {
        IndexArmOffSet3->Text="0";
        bhasKeyDown=false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::IndexArmOffSet3Change(TObject *Sender)
{
    iIndexChange=2;                                                             //kevin 20211211 index socket value change
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::ck_ScanAOIClick(TObject *Sender)
{
    if((USE_Scanner_AOI_Inspection==(int)eBtnAOI_BottomInstall) ||
        (USE_Scanner_AOI_Inspection==(int)eBtnAOI_TopBottomInstall))                   //Jimmychiu 20240322 : Top & Bottom Inspect
    {
        if(ck_ScanAOI->Checked)
        {
            Pnl_ScanAOI->Height=81;
        }
        else
        {
            Pnl_ScanAOI->Height=33;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::Timer1Timer(TObject *Sender)
{
    static bool bRun=false;
    if(bRun)
        return;

    bRun=true;

    if(SystemStart)                                                             //Steven 20220120 : 機台在動的時候,不可以調整Shuttle Offset,避免Hang up
    {
        IndexArmOffSet5->Enabled=false;
        IndexArmOffSet6->Enabled=false;
    }
    else
    {
        if(iSpecialOffSetSel==tOfsIndex1)
        {
            if(InSHT1InRT())
            {
                IndexArmOffSet5->Enabled=false;
                IndexArmOffSet6->Enabled=true;
            }
            else if(InSHT1InLF())
            {
                IndexArmOffSet5->Enabled=true;
                IndexArmOffSet6->Enabled=false;
            }
            else
            {
                IndexArmOffSet5->Enabled=true;
                IndexArmOffSet6->Enabled=true;
            }
        }
        else if(iSpecialOffSetSel==tOfsIndex2)
        {
            if(InSHT2InRT())
            {
                IndexArmOffSet5->Enabled=false;
                IndexArmOffSet6->Enabled=true;
            }
            else if(InSHT2InLF())
            {
                IndexArmOffSet5->Enabled=true;
                IndexArmOffSet6->Enabled=false;
            }
            else
            {
                IndexArmOffSet5->Enabled=true;
                IndexArmOffSet6->Enabled=true;
            }
        }
        else
        {
            IndexArmOffSet5->Enabled=false;
            IndexArmOffSet6->Enabled=false;
        }
    }

    bRun=false;
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::edOffsetContactForceMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 3, true, 0.0, 10.0);   //JimmyChiu 20220114 : Index 總壓力 Offset，加總後數值不顯示於Contact Form
}
//---------------------------------------------------------------------------
void TfOffSet::ShowFinalAirForce()                                              //JimmyChiu 20220114 : Index 總壓力 Offset，加總後數值不顯示於Contact Form
{
    lbFinalAirForce->Caption=FloatToStr(DeviceForm.fAireForce);
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::edOffsetContactForceChange(TObject *Sender)
{
    if(fProductionInfo!=NULL &&
       fProductionInfo->CheckContactForceExist())                               //JimmyChiu 20220117 確認Server是否設定Contact Force，如無設定依照原設定
    {
        fProductionInfo->SetOffsetContactForce(edOffsetContactForce->Text);
        dTemp=DeviceForm.dPress+fProductionInfo->GetOffsetContactForce()*fContact->dDutCount;
        lbFinalAirForce->Caption=FloatToStr(dTemp);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::btnBackClick(TObject *Sender)
{
    btnBack->Down=false;
    PageControl1->ActivePage=tsInOutArmOffset;
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::btnOffsetListClick(TObject *Sender)
{
    btnOffsetList->Down=false;
    PageControl1->ActivePage=tsArmOffset;
    ShowOffSetList();
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::btnToIndexOffsetClick(TObject *Sender)
{
    btnToIndexOffset->Down=false;
    PageControl1->ActivePage=tsIndexOffset;
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::btnToArmOffsetClick(TObject *Sender)
{
    btnToArmOffset->Down=false;
    PageControl1->ActivePage=tsInOutArmOffset;
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::ShowOffSetList()
{
    int iCol;

    if(USE_IN_Y_IS_AUTO_PITCH==true)                                    //JerryYang 20251218 : IN/OUT ARM支援不同模組
    {
        sgOffsetList->ColWidths[oiArmPitch2]=80;
    }
    else
    {
        sgOffsetList->ColWidths[oiArmPitch2]=-1;
    }

    if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                              //QQ
       USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                                 //Ztex 2023.12.06 Add HT-1032
    {
        sgOffsetList->ColWidths[oiArmPitch3]=80;
        sgOffsetList->ColWidths[oiArmPitch4]=80;
    }
    else
    {
        sgOffsetList->ColWidths[oiArmPitch3]=-1;
        sgOffsetList->ColWidths[oiArmPitch4]=-1;
    }

    if(USE_IN_Y_IS_AUTO_PITCH==true)                                    //JerryYang 20251218 : IN/OUT ARM支援不同模組
    {
        sgOffsetList->ColWidths[oiArmPitchY]=80;
    }
    else
    {
        sgOffsetList->ColWidths[oiArmPitchY]=-1;
    }

    for(int i=0; i<OfsTotal; i++)
    {
        iCol=i+1;
        if(OffSetSelBot[i]->Visible==true)
            sgOffsetList->RowHeights[i+1]=30;
        else
            sgOffsetList->RowHeights[i+1]=-1;

        if(i==OfsRotate_In          || i==OfsHP1                || i==OfsHP2                ||
           i==OfsAutoClean          || i==OfsOCR                || i==OfsInPlacement        ||
           i==OfsLoader             || i==OfsLoaderB            ||
           i==OfsAutoSh1            || i==OfsAutoSh2            || i==OfsPreciser           || i==OfsBottom2DID         ||
           i==OfsInSh1              || i==OfsInSh1LB            || i==OfsInSh1RA            || i==OfsInSh1RB            ||
           i==OfsInSh2              || i==OfsInSh2LB            || i==OfsInSh2RA            || i==OfsInSh2RB            ||
           i==OfsInSh1_AutoClean    || i==OfsInSh1LB_AutoClean  || i==OfsInSh1RA_AutoClean  || i==OfsInSh1RB_AutoClean  ||
           i==OfsInSh2_AutoClean    || i==OfsInSh2LB_AutoClean  || i==OfsInSh2RA_AutoClean  || i==OfsInSh2RB_AutoClean)
        {
            sgOffsetList->Cells[oiArmX      ][iCol]    =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[i]]->GetX());
            sgOffsetList->Cells[oiArmY      ][iCol]    =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[i]]->GetY());
            sgOffsetList->Cells[oiArmPick   ][iCol]    =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[i]]->GetPickUp());
            sgOffsetList->Cells[oiArmPlace  ][iCol]    =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[i]]->GetPlace());
            sgOffsetList->Cells[oiArmPitch1 ][iCol]    =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[i]]->GetVariable());
            sgOffsetList->Cells[oiArmPickA_X][iCol]    =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[i]]->SingleOffSet->dPosOffSetX[0][0]);     //Eastsun 20251219 : in/out Arm Fix模式 獨立吸嘴偏移X功能  [A C E G] [B D F H]
            sgOffsetList->Cells[oiArmPickA_Y][iCol]    =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[i]]->SingleOffSet->dPosOffSetY[0][0]);
            sgOffsetList->Cells[oiArmPickB_X][iCol]    =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[i]]->SingleOffSet->dPosOffSetX[1][0]);
            sgOffsetList->Cells[oiArmPickB_Y][iCol]    =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[i]]->SingleOffSet->dPosOffSetY[1][0]);
            sgOffsetList->Cells[oiArmPickC_X][iCol]    =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[i]]->SingleOffSet->dPosOffSetX[0][1]);
            sgOffsetList->Cells[oiArmPickC_Y][iCol]    =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[i]]->SingleOffSet->dPosOffSetY[0][1]);
            sgOffsetList->Cells[oiArmPickD_X][iCol]    =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[i]]->SingleOffSet->dPosOffSetX[1][1]);
            sgOffsetList->Cells[oiArmPickD_Y][iCol]    =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[i]]->SingleOffSet->dPosOffSetY[1][1]);
            sgOffsetList->Cells[oiArmPickE_X][iCol]    =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[i]]->SingleOffSet->dPosOffSetX[0][2]);
            sgOffsetList->Cells[oiArmPickE_Y][iCol]    =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[i]]->SingleOffSet->dPosOffSetY[0][2]);
            sgOffsetList->Cells[oiArmPickF_X][iCol]    =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[i]]->SingleOffSet->dPosOffSetX[1][2]);
            sgOffsetList->Cells[oiArmPickF_Y][iCol]    =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[i]]->SingleOffSet->dPosOffSetY[1][2]);
            sgOffsetList->Cells[oiArmPickG_X][iCol]    =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[i]]->SingleOffSet->dPosOffSetX[0][3]);
            sgOffsetList->Cells[oiArmPickG_Y][iCol]    =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[i]]->SingleOffSet->dPosOffSetY[0][3]);
            sgOffsetList->Cells[oiArmPickH_X][iCol]    =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[i]]->SingleOffSet->dPosOffSetX[1][3]);
            sgOffsetList->Cells[oiArmPickH_Y][iCol]    =FormatFloat("0.00", InArmOffSet_File[iOffsetMap[i]]->SingleOffSet->dPosOffSetY[1][3]);

            if(USE_IN_Y_IS_AUTO_PITCH==true)                                    //JerryYang 20251218 : IN/OUT ARM支援不同模組
            {
                sgOffsetList->Cells[oiArmPitch2][iCol]=FormatFloat("0.00", InArmOffSet_File[iOffsetMap[i]]->GetVariable2());
            }

            if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                      //QQ
               USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                         //Ztex 2023.12.06 Add HT-1032
            {
                sgOffsetList->Cells[oiArmPitch3][iCol]=FormatFloat("0.00", InArmOffSet_File[iOffsetMap[i]]->GetVariable3());
                sgOffsetList->Cells[oiArmPitch4][iCol]=FormatFloat("0.00", InArmOffSet_File[iOffsetMap[i]]->GetVariable4());
            }

            if(USE_IN_Y_IS_AUTO_PITCH==true)                                    //JerryYang 20251218 : IN/OUT ARM支援不同模組
            {
                sgOffsetList->Cells[oiArmPitchY][iCol]=FormatFloat("0.00", InArmOffSet_File[iOffsetMap[i]]->GetVariableY());
            }
        }
        else if(i==OfsOutSh1        || i==OfsOutSh1LB || i==OfsOutSh1RA || i==OfsOutSh1RB   ||
                i==OfsOutSh2        || i==OfsOutSh2LB || i==OfsOutSh2RA || i==OfsOutSh2RB   ||
                i==OfsAuto1         || i==OfsAuto2    || i==OfsAuto3    || i==OfsAuto4      || i==OfsAuto5  || i==OfsAuto6 ||
                i==OfsFix1          || i==OfsFix2     || i==OfsFix3     || i==OfsFix4       || i==OfsFix5   || i==OfsFix6  ||
                i==OfsRotate_Out    || i==OfsTopView  || i==OfsPADView  || i==OfsBGAView    || i==OfsScanAOI)
        {
            sgOffsetList->Cells[oiArmX      ][iCol]    =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[i]]->GetX());
            sgOffsetList->Cells[oiArmY      ][iCol]    =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[i]]->GetY());
            sgOffsetList->Cells[oiArmPick   ][iCol]    =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[i]]->GetPickUp());
            sgOffsetList->Cells[oiArmPlace  ][iCol]    =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[i]]->GetPlace());
            sgOffsetList->Cells[oiArmPitch1 ][iCol]    =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[i]]->GetVariable());
            sgOffsetList->Cells[oiArmPickA_X][iCol]    =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[i]]->SingleOffSet->dPosOffSetX[0][0]);     //Eastsun 20251219 : in/out Arm Fix模式 獨立吸嘴偏移X功能  [A C E G] [B D F H]
            sgOffsetList->Cells[oiArmPickA_Y][iCol]    =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[i]]->SingleOffSet->dPosOffSetY[0][0]);
            sgOffsetList->Cells[oiArmPickB_X][iCol]    =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[i]]->SingleOffSet->dPosOffSetX[1][0]);
            sgOffsetList->Cells[oiArmPickB_Y][iCol]    =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[i]]->SingleOffSet->dPosOffSetY[1][0]);
            sgOffsetList->Cells[oiArmPickC_X][iCol]    =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[i]]->SingleOffSet->dPosOffSetX[0][1]);
            sgOffsetList->Cells[oiArmPickC_Y][iCol]    =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[i]]->SingleOffSet->dPosOffSetY[0][1]);
            sgOffsetList->Cells[oiArmPickD_X][iCol]    =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[i]]->SingleOffSet->dPosOffSetX[1][1]);
            sgOffsetList->Cells[oiArmPickD_Y][iCol]    =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[i]]->SingleOffSet->dPosOffSetY[1][1]);
            sgOffsetList->Cells[oiArmPickE_X][iCol]    =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[i]]->SingleOffSet->dPosOffSetX[0][2]);
            sgOffsetList->Cells[oiArmPickE_Y][iCol]    =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[i]]->SingleOffSet->dPosOffSetY[0][2]);
            sgOffsetList->Cells[oiArmPickF_X][iCol]    =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[i]]->SingleOffSet->dPosOffSetX[1][2]);
            sgOffsetList->Cells[oiArmPickF_Y][iCol]    =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[i]]->SingleOffSet->dPosOffSetY[1][2]);
            sgOffsetList->Cells[oiArmPickG_X][iCol]    =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[i]]->SingleOffSet->dPosOffSetX[0][3]);
            sgOffsetList->Cells[oiArmPickG_Y][iCol]    =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[i]]->SingleOffSet->dPosOffSetY[0][3]);
            sgOffsetList->Cells[oiArmPickH_X][iCol]    =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[i]]->SingleOffSet->dPosOffSetX[1][3]);
            sgOffsetList->Cells[oiArmPickH_Y][iCol]    =FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[i]]->SingleOffSet->dPosOffSetY[1][3]);

            if(USE_OUT_Y_IS_AUTO_PITCH==true)                                   //JerryYang 20251218 : IN/OUT ARM支援不同模組
            {
                sgOffsetList->Cells[oiArmPitch2][iCol]=FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[i]]->GetVariable2());
            }

            if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||                      //QQ
               USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                         //Ztex 2023.12.06 Add HT-1032
            {
                sgOffsetList->Cells[oiArmPitch3][iCol]=FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[i]]->GetVariable3());
                sgOffsetList->Cells[oiArmPitch4][iCol]=FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[i]]->GetVariable4());
            }

            if(USE_OUT_Y_IS_AUTO_PITCH==true)                                   //JerryYang 20251218 : IN/OUT ARM支援不同模組
            {
                sgOffsetList->Cells[oiArmPitchY][iCol]=FormatFloat("0.00", OutArmOffSet_File[iOffsetMap[i]]->GetVariableY());
            }
        }
    }
}
//---------------------------------------------------------------------------
AnsiString TfOffSet::Tri_Position_Offset()                                      //Ztex 2024.03.29 Tri Temp Position Offset
{
    AnsiString sResult="";

    if(Temperature.fWorkTemperBase<=25 && LastSet.iTemperature==Tempture_Hot)
    {
        sResult="\\Position Offset cool.Data";
    }
    else if(Temperature.fWorkTemperBase>=25 && LastSet.iTemperature==Tempture_Hot)
    {
        sResult="\\Position Offset Hot.Data";
    }
    else
    {
        sResult="\\Position Offset.Data";
    }
    return sResult;
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::sbZcalibrationClick(TObject *Sender)
{
    sbZcalibration->Down=false;
    PageControl1->ActivePage=tsZCalibration;
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::sbInArmZCalibrationMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int ret;
    if(fAllMotorHome==false)
    {
        ShowMyMessage("Z Calibration Need Home","Z Calibration 需要歸零");
        return;
    }

    if(InArmSuck.HasIC()==true || OutArmSuck.HasIC()==true ||
       ShuttleHasIC()==true || IndexHasIC()==true)
    {
        ShowMyMessage("Please complete ONE CYCLE first","請先完成ONE CYCLE");
        return;
    }

    if(LastSet.iRealDummy!=REALLY)
    {
        ShowMyMessage("Please change mode to REAL","請先切換為生產模式");
        return;
    }

    ret=ShowMyMessageBox_YES_NO("Please make sure there are 8 picker on input arm?", "請確認input arm是否已安裝8支吸嘴?");
    if(ret==2)
    {
        return;
    }
    else
    {
        AutoTeachLoadTrayZ(true, InArm, iInArmZTeachTask);
        bDoInZTeach=true;
        sbInArmZCalibration->Down=true;
        sbOutArmZCalibration->Enabled=false;
        fMain->Start("sbInArmZCalibrationMouseDown");
    }
}
//---------------------------------------------------------------------------
void __fastcall TfOffSet::sbOutArmZCalibrationMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int ret;
    if(fAllMotorHome==false)
    {
        ShowMyMessage("Z Calibration Need Home","Z Calibration 需要歸零");
        return;
    }

    if(InArmSuck.HasIC()==true && OutArmSuck.HasIC()==true &&
       ShuttleHasIC()==true && IndexHasIC()==true)
    {
        ShowMyMessage("Please complete ONE CYCLE first","請先完成ONE CYCLE");
        return;
    }

    ret=ShowMyMessageBox_YES_NO("Please make sure there are 8 picker on output arm?", "請確認output arm是否已安裝8支吸嘴?");
    if(ret==2)
    {
        return;
    }
    else
    {
        AutoTeachLoadTrayZ(true, OutArm, iOutArmZTeachTask);
        bDoOutZTeach=true;
        sbOutArmZCalibration->Down=true;
        sbInArmZCalibration->Enabled=false;
        fMain->Start("sbInArmZCalibrationMouseDown");
    }
}
//---------------------------------------------------------------------------
void SetTempShiftOffsetPeriod(int CurrentTemp,AnsiString aCurrentSetupFile)
{
    AnsiString aSetupFile[3],aLowSetupFile,aMidSetupFile,aHighSetupFile,S,S1;
    int iOffsetPos=0,iCurrentOffset=0;
    double fOffsetPos=0.0;
    int iTempOffset[30]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//,iPoint=3;
    bool bUse3Point=true;

    S.sprintf("D:\\HT9045\\data\\%s.ini",aCurrentSetupFile);

    aLowSetupFile.sprintf("D:\\HT9045\\IniData\\DefineOffset\\STD_25.data");
    aMidSetupFile.sprintf("D:\\HT9045\\IniData\\DefineOffset\\STD_75.data");
    aHighSetupFile.sprintf("D:\\HT9045\\IniData\\DefineOffset\\STD_125.data");

    aSetupFile[0]   = aLowSetupFile;
    aSetupFile[1]   = aMidSetupFile;
    aSetupFile[2]   = aHighSetupFile;

    if( CheckFileExist(aMidSetupFile.c_str())==false )
    {
        bUse3Point = false;
//        iPoint = 2;
//        aSetupFile[1] = aHighSetupFile;
    }

    iTempOffset[2]=Temperature.fTempOffSet[UserOffSet][tcHotPlate1];
    iTempOffset[3]=Temperature.fTempOffSet[UserOffSet][tcHotPlate1];
    iTempOffset[4]=Temperature.fTempOffSet[UserOffSet][tcHotPlate2];
    iTempOffset[5]=Temperature.fTempOffSet[UserOffSet][tcHotPlate2];
    iTempOffset[6]=Temperature.fTempOffSet[UserOffSet][tcShuttle1];
    iTempOffset[7]=Temperature.fTempOffSet[UserOffSet][tcShuttle1];
    iTempOffset[8]=Temperature.fTempOffSet[UserOffSet][tcShuttle2];
    iTempOffset[9]=Temperature.fTempOffSet[UserOffSet][tcShuttle2];

    for(int i=0;i<3;i++)
    {
        iOffsetPos = ReadIniData(aSetupFile[i],"Loader","LoaderXOffset",0);
        InvisibleOffset.iTempShiftOffset[i][0] = iOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][1]=iOffsetPos;
        iOffsetPos = ReadIniData(aSetupFile[i],"Loader","LoaderYOffset",0);
        InvisibleOffset.iTempShiftOffset[i][1] = iOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][2]=iOffsetPos;
        iOffsetPos = ReadIniData(aSetupFile[i],"HotPlate","HotPlate1XOffset",0);
        InvisibleOffset.iTempShiftOffset[i][2] = iOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][3]=iOffsetPos;
        iOffsetPos = ReadIniData(aSetupFile[i],"HotPlate","HotPlate1YOffset",0);
        InvisibleOffset.iTempShiftOffset[i][3] = iOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][4]=iOffsetPos;
        iOffsetPos = ReadIniData(aSetupFile[i],"HotPlate","HotPlate2XOffset",0);
        InvisibleOffset.iTempShiftOffset[i][4] = iOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][5]=iOffsetPos;
        iOffsetPos = ReadIniData(aSetupFile[i],"HotPlate","HotPlate2YOffset",0);
        InvisibleOffset.iTempShiftOffset[i][5] = iOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][6]=iOffsetPos;
        iOffsetPos = ReadIniData(aSetupFile[i],"InShuttle","Shuttle1XOffset",0);
        InvisibleOffset.iTempShiftOffset[i][6] = iOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][7]=iOffsetPos;
        iOffsetPos = ReadIniData(aSetupFile[i],"InShuttle","Shuttle1YOffset",0);
        InvisibleOffset.iTempShiftOffset[i][7] = iOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][8]=iOffsetPos;
        iOffsetPos = ReadIniData(aSetupFile[i],"InShuttle","Shuttle2XOffset",0);
        InvisibleOffset.iTempShiftOffset[i][8] = iOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][9]=iOffsetPos;
        iOffsetPos = ReadIniData(aSetupFile[i],"InShuttle","Shuttle2YOffset",0);
        InvisibleOffset.iTempShiftOffset[i][9] = iOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][10]=iOffsetPos;
        iOffsetPos = ReadIniData(aSetupFile[i],"OutShuttle","Shuttle1XOffset",0);
        InvisibleOffset.iTempShiftOffset[i][10] = iOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][11]=iOffsetPos;
        iOffsetPos = ReadIniData(aSetupFile[i],"OutShuttle","Shuttle1YOffset",0);
        InvisibleOffset.iTempShiftOffset[i][11] = iOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][12]=iOffsetPos;
        iOffsetPos = ReadIniData(aSetupFile[i],"OutShuttle","Shuttle2XOffset",0);
        InvisibleOffset.iTempShiftOffset[i][12] = iOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][13]=iOffsetPos;
        iOffsetPos = ReadIniData(aSetupFile[i],"OutShuttle","Shuttle2YOffset",0);
        InvisibleOffset.iTempShiftOffset[i][13] = iOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][14]=iOffsetPos;

        iOffsetPos = ReadIniData(aSetupFile[i],"Index1","ShuttleROffset",0);
        InvisibleOffset.iTempShiftOffset[i][14] = iOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][15]=iOffsetPos;
        iOffsetPos = ReadIniData(aSetupFile[i],"Index1","ShuttleLOffset",0);
        InvisibleOffset.iTempShiftOffset[i][15] = iOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][16]=iOffsetPos;
        iOffsetPos = ReadIniData(aSetupFile[i],"Index2","ShuttleROffset",0);
        InvisibleOffset.iTempShiftOffset[i][16] = iOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][17]=iOffsetPos;
        iOffsetPos = ReadIniData(aSetupFile[i],"Index2","ShuttleLOffset",0);
        InvisibleOffset.iTempShiftOffset[i][17] = iOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][18]=iOffsetPos;

        fOffsetPos = ReadIniData(aSetupFile[i],"LoaderScale","LoaderX",1.0);
        InvisibleOffset.fTempShiftOffset[i][0] = fOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][20]=fOffsetPos;
        fOffsetPos = ReadIniData(aSetupFile[i],"LoaderScale","LoaderY",1.0);
        InvisibleOffset.fTempShiftOffset[i][1] = fOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][21]=fOffsetPos;
        fOffsetPos = ReadIniData(aSetupFile[i],"LoaderScale","HotPlate1X",1.0);
        InvisibleOffset.fTempShiftOffset[i][2] = fOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][22]=fOffsetPos;
        fOffsetPos = ReadIniData(aSetupFile[i],"LoaderScale","HotPlate1Y",1.0);
        InvisibleOffset.fTempShiftOffset[i][3] = fOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][23]=fOffsetPos;
        fOffsetPos = ReadIniData(aSetupFile[i],"LoaderScale","HotPlate2X",1.0);
        InvisibleOffset.fTempShiftOffset[i][4] = fOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][24]=fOffsetPos;
        fOffsetPos = ReadIniData(aSetupFile[i],"LoaderScale","HotPlate2Y",1.0);
        InvisibleOffset.fTempShiftOffset[i][5] = fOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][25]=fOffsetPos;

        iOffsetPos = ReadIniData(aSetupFile[i],"UnloaderOffset","Auto1XOffset",0);
        InvisibleOffset.iTempShiftOffset[i][18] = iOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][27]=iOffsetPos;
        iOffsetPos = ReadIniData(aSetupFile[i],"UnloaderOffset","Auto1YOffset",0);
        InvisibleOffset.iTempShiftOffset[i][19] = iOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][28]=iOffsetPos;
        iOffsetPos = ReadIniData(aSetupFile[i],"UnloaderOffset","Auto2XOffset",0);
        InvisibleOffset.iTempShiftOffset[i][20] = iOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][29]=iOffsetPos;
        iOffsetPos = ReadIniData(aSetupFile[i],"UnloaderOffset","Auto2YOffset",0);
        InvisibleOffset.iTempShiftOffset[i][21] = iOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][30]=iOffsetPos;
        iOffsetPos = ReadIniData(aSetupFile[i],"UnloaderOffset","Auto3XOffset",0);
        InvisibleOffset.iTempShiftOffset[i][22] = iOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][31]=iOffsetPos;
        iOffsetPos = ReadIniData(aSetupFile[i],"UnloaderOffset","Auto3YOffset",0);
        InvisibleOffset.iTempShiftOffset[i][23] = iOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][32]=iOffsetPos;
        iOffsetPos = ReadIniData(aSetupFile[i],"UnloaderOffset","Fix1XOffset",0);
        InvisibleOffset.iTempShiftOffset[i][24] = iOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][33]=iOffsetPos;
        iOffsetPos = ReadIniData(aSetupFile[i],"UnloaderOffset","Fix1YOffset",0);
        InvisibleOffset.iTempShiftOffset[i][25] = iOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][34]=iOffsetPos;
        iOffsetPos = ReadIniData(aSetupFile[i],"UnloaderOffset","Fix2XOffset",0);
        InvisibleOffset.iTempShiftOffset[i][26] = iOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][35]=iOffsetPos;
        iOffsetPos = ReadIniData(aSetupFile[i],"UnloaderOffset","Fix2YOffset",0);
        InvisibleOffset.iTempShiftOffset[i][27] = iOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][36]=iOffsetPos;
        iOffsetPos = ReadIniData(aSetupFile[i],"UnloaderOffset","Fix3XOffset",0);
        InvisibleOffset.iTempShiftOffset[i][28] = iOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][37]=iOffsetPos;
        iOffsetPos = ReadIniData(aSetupFile[i],"UnloaderOffset","Fix3YOffset",0);
        InvisibleOffset.iTempShiftOffset[i][29] = iOffsetPos;
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[1+i][38]=iOffsetPos;
    }

    for(int i=0;i<30;i++)
    {
        if( i < 18 )
        {
            if( bUse3Point )
            {
                InvisibleOffset.fTempShiftOffsetRatio[0][i]  = (InvisibleOffset.iTempShiftOffset[1][i] - InvisibleOffset.iTempShiftOffset[0][i])/(75.0-25.0);
                fOffSet->sg_Offset_HotTempShiftOffset->Cells[4][i+1]=InvisibleOffset.fTempShiftOffsetRatio[0][i];
                InvisibleOffset.fTempShiftOffsetRatio[1][i]  = (InvisibleOffset.iTempShiftOffset[2][i] - InvisibleOffset.iTempShiftOffset[1][i])/(125.0-75.0);
                fOffSet->sg_Offset_HotTempShiftOffset->Cells[5][i+1]=InvisibleOffset.fTempShiftOffsetRatio[1][i];
            }
            else
            {
                InvisibleOffset.fTempShiftOffsetRatio[0][i]  = (InvisibleOffset.iTempShiftOffset[2][i] - InvisibleOffset.iTempShiftOffset[0][i])/(125.0-25.0);
                fOffSet->sg_Offset_HotTempShiftOffset->Cells[4][i+1]=InvisibleOffset.fTempShiftOffsetRatio[0][i];
            }
        }
        else
        {
            if( bUse3Point )
            {
                InvisibleOffset.fTempShiftOffsetRatio[0][i]  = (InvisibleOffset.iTempShiftOffset[1][i] - InvisibleOffset.iTempShiftOffset[0][i])/(75.0-25.0);
                fOffSet->sg_Offset_HotTempShiftOffset->Cells[4][i+1+8]=InvisibleOffset.fTempShiftOffsetRatio[0][i];
                InvisibleOffset.fTempShiftOffsetRatio[1][i]  = (InvisibleOffset.iTempShiftOffset[2][i] - InvisibleOffset.iTempShiftOffset[1][i])/(125.0-75.0);
                fOffSet->sg_Offset_HotTempShiftOffset->Cells[5][i+1+8]=InvisibleOffset.fTempShiftOffsetRatio[1][i];
            }
            else
            {
                InvisibleOffset.fTempShiftOffsetRatio[0][i]  = (InvisibleOffset.iTempShiftOffset[2][i] - InvisibleOffset.iTempShiftOffset[0][i])/(125.0-25.0);
                fOffSet->sg_Offset_HotTempShiftOffset->Cells[4][i+1+8]=InvisibleOffset.fTempShiftOffsetRatio[0][i];
            }
        }
    }
    for(int i=0;i<6;i++)
    {
        if( bUse3Point )
        {
            InvisibleOffset.fTempShiftScaleRatio[0][i]  = (InvisibleOffset.fTempShiftOffset[1][i] - InvisibleOffset.fTempShiftOffset[0][i])/(75.0-25.0);
            S1.sprintf( "%2.4f" , InvisibleOffset.fTempShiftScaleRatio[0][i] );
            fOffSet->sg_Offset_HotTempShiftOffset->Cells[4][i+1+19]=S1;
            InvisibleOffset.fTempShiftScaleRatio[1][i]  = (InvisibleOffset.fTempShiftOffset[2][i] - InvisibleOffset.fTempShiftOffset[1][i])/(125.0-75.0);
            S1.sprintf( "%2.4f" , InvisibleOffset.fTempShiftScaleRatio[1][i] );
            fOffSet->sg_Offset_HotTempShiftOffset->Cells[5][i+1+19]=S1;
        }
        else
        {
            InvisibleOffset.fTempShiftScaleRatio[0][i]  = (InvisibleOffset.fTempShiftOffset[2][i] - InvisibleOffset.fTempShiftOffset[0][i])/(125.0-25.0);
            S1.sprintf( "%2.4f" , InvisibleOffset.fTempShiftScaleRatio[0][i] );
            fOffSet->sg_Offset_HotTempShiftOffset->Cells[4][i+1+19]=S1;
        }
    }

    for(int i=0;i<30;i++)
    {
        if( bUse3Point )
        {
            if( (CurrentTemp+iTempOffset[i]) == 25 )
            {
                iCurrentOffset = InvisibleOffset.iTempShiftOffset[0][i];
            }
            else if( (CurrentTemp+iTempOffset[i]) == 75 )
            {
                iCurrentOffset = InvisibleOffset.iTempShiftOffset[1][i];
            }
            else if( (CurrentTemp+iTempOffset[i]) == 125 )
            {
                iCurrentOffset = InvisibleOffset.iTempShiftOffset[2][i];
            }
            else if( (CurrentTemp+iTempOffset[i])>25 && (CurrentTemp+iTempOffset[i])<75 )
            {
                iCurrentOffset = (InvisibleOffset.fTempShiftOffsetRatio[0][i]*((CurrentTemp+iTempOffset[i])-25))+InvisibleOffset.iTempShiftOffset[0][i];
            }
            else if( (CurrentTemp+iTempOffset[i])>75 )
            {
                iCurrentOffset = int(InvisibleOffset.fTempShiftOffsetRatio[1][i]*((CurrentTemp+iTempOffset[i])-75))+InvisibleOffset.iTempShiftOffset[1][i];
            }
        }
        else
        {
            if( (CurrentTemp+iTempOffset[i]) == 25 )
            {
                iCurrentOffset = InvisibleOffset.iTempShiftOffset[0][i];
            }
            else if( (CurrentTemp+iTempOffset[i]) == 125 )
            {
                iCurrentOffset = InvisibleOffset.iTempShiftOffset[2][i];
            }
            else
                iCurrentOffset = (InvisibleOffset.fTempShiftOffsetRatio[0][i]*((CurrentTemp+iTempOffset[i])-25))+InvisibleOffset.iTempShiftOffset[0][i];
        }

        if( i< 18 )
            fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][i+1] = iCurrentOffset;
        else
            fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][i+1+8] = iCurrentOffset;
    }

    for(int i=0;i<6;i++)
    {
        if( bUse3Point )
        {
            if( CurrentTemp>=25 && CurrentTemp<=75 )
            {
                fOffsetPos = (InvisibleOffset.fTempShiftScaleRatio[0][i]*((CurrentTemp+iTempOffset[i])-25))+InvisibleOffset.fTempShiftOffset[0][i];
            }
            else if( CurrentTemp>75 )
            {
                fOffsetPos = (InvisibleOffset.fTempShiftScaleRatio[1][i]*((CurrentTemp+iTempOffset[i])-75))+InvisibleOffset.fTempShiftOffset[1][i];
            }
        }
        else
        {
            fOffsetPos = (InvisibleOffset.fTempShiftScaleRatio[0][i]*((CurrentTemp+iTempOffset[i])-25))+InvisibleOffset.fTempShiftOffset[0][i];
        }
        S1.sprintf( "%2.4f" , fOffsetPos );
        fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][i+1+19] = S1;
    }
//    bCheckTempShiftOffsetCompare = true;
    WriteIniData(S,"Loader","LoaderXOffset",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][1]);
    WriteIniData(S,"Loader","LoaderYOffset",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][2]);
    WriteIniData(S,"HotPlate","HotPlate1XOffset",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][3]);
    WriteIniData(S,"HotPlate","HotPlate1YOffset",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][4]);
    WriteIniData(S,"HotPlate","HotPlate2XOffset",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][5]);
    WriteIniData(S,"HotPlate","HotPlate2YOffset",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][6]);
    WriteIniData(S,"InShuttle","Shuttle1XOffset",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][7]);
    WriteIniData(S,"InShuttle","Shuttle1YOffset",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][8]);
    WriteIniData(S,"InShuttle","Shuttle2XOffset",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][9]);
    WriteIniData(S,"InShuttle","Shuttle2YOffset",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][10]);
    WriteIniData(S,"OutShuttle","Shuttle1XOffset",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][11]);
    WriteIniData(S,"OutShuttle","Shuttle1YOffset",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][12]);
    WriteIniData(S,"OutShuttle","Shuttle2XOffset",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][13]);
    WriteIniData(S,"OutShuttle","Shuttle2YOffset",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][14]);
    WriteIniData(S,"Index1","ShuttleROffset",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][15]);
    WriteIniData(S,"Index1","ShuttleLOffset",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][16]);
    WriteIniData(S,"Index2","ShuttleROffset",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][17]);
    WriteIniData(S,"Index2","ShuttleLOffset",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][18]);

    WriteIniData(S,"LoaderScale","LoaderX",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][20]);
    WriteIniData(S,"LoaderScale","LoaderY",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][21]);
    WriteIniData(S,"LoaderScale","HotPlate1X",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][22]);
    WriteIniData(S,"LoaderScale","HotPlate1Y",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][23]);
    WriteIniData(S,"LoaderScale","HotPlate2X",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][24]);
    WriteIniData(S,"LoaderScale","HotPlate2Y",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][25]);

    WriteIniData(S,"UnloaderOffset","Auto1XOffset",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][27]);
    WriteIniData(S,"UnloaderOffset","Auto1YOffset",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][28]);
    WriteIniData(S,"UnloaderOffset","Auto2XOffset",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][29]);
    WriteIniData(S,"UnloaderOffset","Auto2YOffset",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][30]);
    WriteIniData(S,"UnloaderOffset","Auto3XOffset",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][31]);
    WriteIniData(S,"UnloaderOffset","Auto3YOffset",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][32]);
    WriteIniData(S,"UnloaderOffset","Fix1XOffset",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][33]);
    WriteIniData(S,"UnloaderOffset","Fix1YOffset",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][34]);
    WriteIniData(S,"UnloaderOffset","Fix2XOffset",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][35]);
    WriteIniData(S,"UnloaderOffset","Fix2YOffset",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][36]);
    WriteIniData(S,"UnloaderOffset","Fix3XOffset",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][37]);
    WriteIniData(S,"UnloaderOffset","Fix3YOffset",fOffSet->sg_Offset_HotTempShiftOffset->Cells[6][38]);

    fOffSet->WriteHotTempShiftOffsetData();
}
//==============================================================================
void __fastcall TfOffSet::LoadLoaderScaleValues()
{
    AnsiString S,aPath;
    S=AnsiString(fMain->cbSetupFileName->Text);

    aPath.sprintf("D:\\HT9045\\Data\\%s.ini",S);

    TestIF_File.bInArmUseDifferentScaleBySetupFile   =ReadIniData(aPath,"LoaderScale","ScanleEnable",false);
    TestIF_File.fLoaderTrayXScaleBySetupFile     =CheckRange(ReadIniData(aPath,"LoaderScale","LoaderX",1.0), 0.95,1.05);
    TestIF_File.fLoaderTrayYScaleBySetupFile     =CheckRange(ReadIniData(aPath,"LoaderScale","LoaderY",1.0), 0.95,1.05);
    TestIF_File.fHotPlateXScaleBySetupFile[0]    =CheckRange(ReadIniData(aPath,"LoaderScale","HotPlate1X",1.0), 0.95,1.05);
    TestIF_File.fHotPlateYScaleBySetupFile[0]    =CheckRange(ReadIniData(aPath,"LoaderScale","HotPlate1Y",1.0), 0.95,1.05);
    TestIF_File.fHotPlateXScaleBySetupFile[1]    =CheckRange(ReadIniData(aPath,"LoaderScale","HotPlate2X",1.0), 0.95,1.05);
    TestIF_File.fHotPlateYScaleBySetupFile[1]    =CheckRange(ReadIniData(aPath,"LoaderScale","HotPlate2Y",1.0), 0.95,1.05);
    TestIF_File.fInShuttleXScaleBySetupFile[0]   =CheckRange(ReadIniData(aPath,"LoaderScale","Shuttle1X",1.0), 0.95,1.05);
    TestIF_File.fInShuttleYScaleBySetupFile[0]   =CheckRange(ReadIniData(aPath,"LoaderScale","Shuttle1Y",1.0), 0.95,1.05);
    TestIF_File.fInShuttleXScaleBySetupFile[1]   =CheckRange(ReadIniData(aPath,"LoaderScale","Shuttle2X",1.0), 0.95,1.05);
    TestIF_File.fInShuttleYScaleBySetupFile[1]   =CheckRange(ReadIniData(aPath,"LoaderScale","Shuttle2Y",1.0), 0.95,1.05);

    TestIF_File.bOutArmUseDifferentScaleBySetupFile  = ReadIniData(aPath,"UnLoaderScale","ScanleEnable",false);
    TestIF_File.fOutShuttleXScaleBySetupFile[0]      = CheckRange(ReadIniData(aPath,"UnLoaderScale","Shuttle1X",1.0), 0.95,1.05);
    TestIF_File.fOutShuttleYScaleBySetupFile[0]      = CheckRange(ReadIniData(aPath,"UnLoaderScale","Shuttle1Y",1.0), 0.95,1.05);
    TestIF_File.fOutShuttleXScaleBySetupFile[1]      = CheckRange(ReadIniData(aPath,"UnLoaderScale","Shuttle1X",1.0), 0.95,1.05);
    TestIF_File.fOutShuttleYScaleBySetupFile[1]      = CheckRange(ReadIniData(aPath,"UnLoaderScale","Shuttle1Y",1.0), 0.95,1.05);
}
//---------------------------------------------------------------------------
int __fastcall TfOffSet::LoadASECLOffsetIndexValues(int iType,int iPos)
{
    AnsiString aPath, S;
    S=AnsiString(fMain->cbSetupFileName->Text);
    aPath.sprintf("D:\\HT9045\\Data\\%s.ini",S);
    int iOffsetPos=0;

    if(CUSTOMER_CODE!=CC_ASE_CL)
        return 0;
//    double iGearRatio=0.5;
//    if(IndexGearRatio==1)
//        iGearRatio=1.0;

    switch(iType)
    {
        case 14:
            if(iPos==0)
            {
                iOffsetPos = ReadIniData(aPath,"Index1","ShuttleLOffset",0);
            }
            else
            {
                iOffsetPos = ReadIniData(aPath,"Index1","ShuttleROffset",0);
            }
            break;

        case 15:
            if( iPos==0 )
            {
                iOffsetPos = ReadIniData(aPath,"Index2","ShuttleLOffset",0);
            }
            else
            {
                iOffsetPos = ReadIniData(aPath,"Index2","ShuttleROffset",0);
            }
            break;
    }
    return iOffsetPos;
}
//pig 20221026 ModifyAutoOffset
//==============================================================================
void __fastcall TfOffSet::edLodXClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 6, true, 0.95, 1.05);
}
//==============================================================================
void __fastcall TfOffSet::WriteHotTempShiftOffsetData()
{
    TStringList *memoPtr;
    AnsiString Filename;

    memoPtr=new TStringList;
    CopyStringGridAsTabFormat(fOffSet->sg_Offset_HotTempShiftOffset,memoPtr);

    Filename="D:\\HT9045\\IniData\\DefineOffset\\HotTempShiftOffsetData.txt";
    memoPtr->SaveToFile(Filename);

    delete memoPtr;
}
//------------------------------------------------------------------------------
void TfOffSet::CopyStringGridAsTabFormat(TStringGrid *strGrd,TStringList *memoPtr)
{
    int x,y,endx;
    AnsiString S;

    memoPtr->Clear();
    for(y=0;y<strGrd->RowCount;y++)
    {
        S="";
        endx=0;
        for(x=(strGrd->ColCount-1);x>=0;x--)
        {
            if( strGrd->Cells[x][y]!="")
            {
                endx=x;
                break;
            }
        }
        for(x=0;x<=endx;x++)
        {
            S+=strGrd->Cells[x][y];
            S+="\t";
        }
        memoPtr->Add(S);
    }
}
//------------------------------------------------------------------------------
bool __fastcall TfOffSet::ReadHotTempShiftOffsetData()
{
    TStringList *memoPtr;
    AnsiString Filename;

    memoPtr=new TStringList;
    Filename="D:\\HT9045\\IniData\\DefineOffset\\HotTempShiftOffsetData.txt";
    if( CheckFileExist( Filename.c_str() )==false )
        return false;
//    bCheckTempShiftOffsetCompare = true;
    memoPtr->LoadFromFile(Filename);
    while(1)
    {
        if( memoPtr->Count==0 )                                                 //pig 20220125
            break;
        if( memoPtr->Strings[memoPtr->Count-1]=="" )
            memoPtr->Delete(memoPtr->Count-1);
        else
            break;
    }

    PasteStringGridAsTabFormat(fOffSet->sg_Offset_HotTempShiftOffset,memoPtr);
    delete memoPtr;

    return true;
}
//------------------------------------------------------------------------------
bool SplitStrByTabOnly(char *str,char *dest,int Max)
{
    char Buffer[10240];
    int ct1=0,ct2=0;

    if( str[ct1]=='\t' )
    {
        strcpy(dest,"");
        for(;;)
        {
            Buffer[ct1]=str[ct1+1];
            if( Buffer[ct1]==0 )
            {
                strcpy(str,Buffer);
                return true;
            }
            ct1++;
        }
    }
    else
    {
        while(1)         // find first character
        {
            if( str[ct1]=='\x0' || str[ct1]=='\r' ||  str[ct1]=='\n'  )
                return false;
            if( ( str[ct1]!='\t' && str[ct1]!='\r') )
                break;
            ct1++;
        }
        while(1)
        {
            dest[ct2]=str[ct1];
            ct2++;
            ct1++;
            dest[ct2]='\x0';
            if( (ct2+1 ) >=Max ) break;

            if(   str[ct1]!='\t' && str[ct1]!='\0' && str[ct1]!='\r' )
            {
            }
            else
            {
                break;
            }
        }
        ct2=0;
        while(1)
        {
            Buffer[ct2]=str[ct1];
            if( str[ct1]=='\x0')
                break;
            ct1++;
            ct2++;
            Buffer[ct2]='\x0';
            if(ct2>=10240)
                break;
        }
        strcpy(str,Buffer);
        ct1=0;
        if( str[0]=='\t' )
        {
            for(;;)
            {
                Buffer[ct1]=str[ct1+1];
                if( Buffer[ct1]==0 )
                {
                    strcpy(str,Buffer);
                    return true;
                }
                ct1++;
            }
        }
    }
    return true;
}
//------------------------------------------------------------------------------
void TfOffSet::PasteStringGridAsTabFormat(TStringGrid *strGrd,TStringList *memoPtr)
{
    int x,y,StartX,StartY;
    char str[8192],str2[256];
    bool flag;
    AnsiString S;

    for(y=0;y<strGrd->RowCount;y++)
        for(x=0;x<strGrd->ColCount;x++)
            strGrd->Cells[x][y]="";

    for(y=0;y<memoPtr->Count;y++)
    {
        if(y>=strGrd->RowCount)
            break;
        strcpy(str,memoPtr->Strings[y].c_str());
        x=0;
        do
        {
            flag=SplitStrByTabOnly(str,str2,256);
            if( flag==false)
                break;
            StartX=x;
            StartY=y;

            strGrd->Cells[StartX][StartY]=str2;
            x++;
            if(x>=strGrd->ColCount)
                break;
        }while(1);
    }
}
//==============================================================================
