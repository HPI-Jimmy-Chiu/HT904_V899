#include "MachineDefine.h"
#pragma hdrstop

#include "uteach.h"

#include "uMotorTest.h"
#include "mymotor.h"
#include "mymessbox.h"
#include "MyLaneIo.h"
#include "common.h"
#include "mysensor.h"
#include "cprod.h"
#include "uhome.h"
#include "cpublic.h"
#include "myswitch.h"
//#include "input.h"
#include "mycylin.h"
#include "iosetview.h"
#include "csystem.h"
#include "uTechShow.h"
#include "ainarm9045.h"
#include "cinitial.h"
#include "note.h"
#include "InOutArmZteach.h"
#include "cmydef.h"
#include "BarCode.h"
#include "myQwertyKeyBoard.h"
#include "LtcSensor.h"
#include "main.h"                                                               //Isaac 20200903 :TTL RS232通訊
#include "MessageDef.h"                                                         //Isaac 20200903 :TTL RS232通訊
#include "HTEditList.h"
#include "acatchtray.h"
//------------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ALed"
#pragma link "BtnPanelLane"
#pragma resource "*.dfm"
TfTeach *fTeach;

int SelMotSpeed=0, iAuto_TeachPitch=1, Tech_Part=0, TECH_MAX_ITEM=0, TechTwoItem=0;
int TECH_DOUBLE_ITEM=0;
bool Teachshow=false;

extern void InitInArmCCDAlignmentTeachUnitProcess();
extern void InitInArmXYAutoTeachProcess();
extern void InitDoAutoRunLoaderTray();
extern void InitOutArmXYZHome();
extern void InitOutArmCCDAlignmentTeachUnitProcess();
extern void InitOutArmXYAutoTeachProcess();
extern void InitDoAutoRunUnloaderTray();
extern void InitInArmXYZHome();
extern void InitInArmPickerXYAndPitchAutoAlignment();
extern void InitOutArmPickerXYAndPitchAutoAlignment();
extern void InitAutoAlignmentTask();                                            //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
bool PitchX_Home=false;                                                         //Ztex 2023.12.15 Add Pitch X Home Twice
int  PitchX_Home_Index=99;                                                      //Ztex 2023.12.15 Add Pitch X Home Twice
//------------------------------------------------------------------------------
TECH_PARA::TECH_PARA(int *Par1, int MotorNo, TEdit *EdtSet, AnsiString _Key, TSpeedButton *SPBFun, TSpeedButton *SPBGo, bool Visible)
{
    Parameter       =Par1;
    MotorSelect     =MotorNo;
    SetEdit         =EdtSet;
    funButton       =SPBFun;
    btGo            =SPBGo;
    Key             =_Key;

    SetEdit->Visible=Visible;
    if(funButton!=NULL)
        funButton->Visible=Visible;
    if(btGo!=NULL)
        btGo->Visible=Visible;
}
//------------------------------------------------------------------------------
void TECH_PARA::ReadFromFile()                                                  //Steven 20240501 : Teach改存成ini
{
    if(MOT[MotorSelect].Alias=="MInShutte1")                                    //Steven 20250926 : Fixed for Shuttle Teach
    {
        if(CheckSectionExist(asTeachPath, "MInShutte1"))
            (int)(*Parameter)=CheckAndReadIniData(asTeachPath, "MInShutte1", Key, (int)(*Parameter));
        else
            (int)(*Parameter)=CheckAndReadIniData(asTeachPath, "MInShuttle1", Key, (int)(*Parameter));
    }
    else if(MOT[MotorSelect].Alias=="MInShuttle1")
    {
        if(CheckSectionExist(asTeachPath, "MInShuttle1"))
            (int)(*Parameter)=CheckAndReadIniData(asTeachPath, "MInShuttle1", Key, (int)(*Parameter));
        else
            (int)(*Parameter)=CheckAndReadIniData(asTeachPath, "MInShutte1", Key, (int)(*Parameter));
    }
    else if(MOT[MotorSelect].Alias=="MInShutte2")
    {
        if(CheckSectionExist(asTeachPath, "MInShutte2"))
            (int)(*Parameter)=CheckAndReadIniData(asTeachPath, "MInShutte2", Key, (int)(*Parameter));
        else
            (int)(*Parameter)=CheckAndReadIniData(asTeachPath, "MInShuttle2", Key, (int)(*Parameter));
    }
    else if(MOT[MotorSelect].Alias=="MInShuttle2")
    {
        if(CheckSectionExist(asTeachPath, "MInShuttle2"))
            (int)(*Parameter)=CheckAndReadIniData(asTeachPath, "MInShuttle2", Key, (int)(*Parameter));
        else
            (int)(*Parameter)=CheckAndReadIniData(asTeachPath, "MInShutte2", Key, (int)(*Parameter));
    }
    else if(MOT[MotorSelect].Alias!="")
    {
        (int)(*Parameter)=CheckAndReadIniData(asTeachPath, MOT[MotorSelect].Alias, Key, (int)(*Parameter));
    }
    else
    {
        (int)(*Parameter)=0;
    }
    SetEdit->Text=(int)(*Parameter);
}
//------------------------------------------------------------------------------
void TECH_PARA::SaveToFile(bool bSaveByTeach)                                   //Steven 20240501 : Teach改存成ini
{
    if(bSaveByTeach==true)
        SetEdit->Text=(int)(*Parameter);
    else
        (int)(*Parameter)=atoi(SetEdit->Text.c_str());

    if(MOT[MotorSelect].Alias=="MInShutte1")                                    //Steven 20250926 : Fixed for Shuttle Teach
    {
        if(CheckSectionExist(asTeachPath, "MInShutte1"))
            WriteIniData(asTeachPath, "MInShutte1", Key, SetEdit->Text);
        else
            WriteIniData(asTeachPath, "MInShuttle1", Key, SetEdit->Text);
    }
    else if(MOT[MotorSelect].Alias=="MInShuttle1")
    {
        if(CheckSectionExist(asTeachPath, "MInShuttle1"))
            WriteIniData(asTeachPath, "MInShuttle1", Key, SetEdit->Text);
        else
            WriteIniData(asTeachPath, "MInShutte1", Key, SetEdit->Text);
    }
    else if(MOT[MotorSelect].Alias=="MInShutte2")
    {
        if(CheckSectionExist(asTeachPath, "MInShutte2"))
            WriteIniData(asTeachPath, "MInShutte2", Key, SetEdit->Text);
        else
            WriteIniData(asTeachPath, "MInShuttle2", Key, SetEdit->Text);
    }
    else if(MOT[MotorSelect].Alias=="MInShuttle2")
    {
        if(CheckSectionExist(asTeachPath, "MInShuttle2"))
            WriteIniData(asTeachPath, "MInShuttle2", Key, SetEdit->Text);
        else
            WriteIniData(asTeachPath, "MInShutte2", Key, SetEdit->Text);
    }
    else if(MOT[MotorSelect].Alias!="")
    {
        WriteIniData(asTeachPath, MOT[MotorSelect].Alias, Key, SetEdit->Text);
    }
}
//------------------------------------------------------------------------------
TECH_TWOPARA::TECH_TWOPARA(int *Par1, int *Par2, int Mot1, int Mot2, TEdit *Edt1, TEdit *Edt2, AnsiString _Key1, AnsiString _Key2, TSpeedButton *SPB1, TSpeedButton *SPB2, bool Visible)
{
    Parameter   [0]=Par1;
    Parameter   [1]=Par2;
    MotorSelect [0]=Mot1;
    MotorSelect [1]=Mot2;
    SetEdit     [0]=Edt1;
    SetEdit     [1]=Edt2;
    Key         [0]=_Key1;
    Key         [1]=_Key2;
    funButton      =SPB1;
    btGo           =SPB2;

    SetEdit     [0]->Visible=Visible;
    SetEdit     [1]->Visible=Visible;
    if(funButton!=NULL)
        funButton->Visible=Visible;
    if(btGo!=NULL)
        btGo->Visible=Visible;
}
//------------------------------------------------------------------------------
void TECH_TWOPARA::ReadFromFile()                                               //Steven 20240501 : Teach改存成ini
{
    if(MOT[MotorSelect[0]].Alias!="")
        (int)(*Parameter[0])=CheckAndReadIniData(asTeachPath, MOT[MotorSelect[0]].Alias, Key[0], (int)(*Parameter[0]));

    if(MOT[MotorSelect[1]].Alias!="")
        (int)(*Parameter[1])=CheckAndReadIniData(asTeachPath, MOT[MotorSelect[1]].Alias, Key[1], (int)(*Parameter[1]));
    SetEdit[0]->Text=(int)(*Parameter[0]);
    SetEdit[1]->Text=(int)(*Parameter[1]);
}
//------------------------------------------------------------------------------
void TECH_TWOPARA::SaveToFile(bool bSaveByTeach)                                //Steven 20240501 : Teach改存成ini
{
    if(bSaveByTeach==true)
    {
        SetEdit[0]->Text=(int)(*Parameter[0]);
        SetEdit[1]->Text=(int)(*Parameter[1]);
    }
    else
    {
        (int)(*Parameter[0])=atoi(SetEdit[0]->Text.c_str());
        (int)(*Parameter[1])=atoi(SetEdit[1]->Text.c_str());
    }

    if(MOT[MotorSelect[0]].Alias!="")
        WriteIniData(asTeachPath, MOT[MotorSelect[0]].Alias, Key[0], SetEdit[0]->Text);
    if(MOT[MotorSelect[1]].Alias!="")
        WriteIniData(asTeachPath, MOT[MotorSelect[1]].Alias, Key[1], SetEdit[1]->Text);
}
//------------------------------------------------------------------------------
TECH_MotorAxle::TECH_MotorAxle(int MotorNo, TSpeedButton *SPB, bool Visible)
{
    MotorSelect             =MotorNo;
    SelButton               =SPB;
    SelButton->GroupIndex   =1;
    SelButton->Tag          =MotorNo;
    SelButton->AllowAllUp   =true;
    SelButton->Visible      =Visible;
}
//------------------------------------------------------------------------------
void TECH_SUCKPARA::ReadFromFile()                                              //Steven 20240523 : Teach SUCKPARA改存成ini
{
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<8; j++)
        {
            if(MOT[MotorSelect[i][j]].Alias!="")
            {
                (int)(*Parameter[i][j])=CheckAndReadIniData(asTeachPath, Group, Key[i][j], (int)(*Parameter[i][j]));
                SetEdit[i][j]->Text=(int)(*Parameter[i][j]);
            }
        }
    }
}
//------------------------------------------------------------------------------
void TECH_SUCKPARA::SaveToFile(bool bSaveByTeach)                               //Steven 20240523 : Teach SUCKPARA改存成ini
{
    for(int i=0; i<2; i++)
    {
        for(int j=0; j<8; j++)
        {
            if(MOT[MotorSelect[i][j]].Alias!="")
            {
                if(bSaveByTeach==true)
                    SetEdit[i][j]->Text=(int)(*Parameter[i][j]);
                else
                    (int)(*Parameter[i][j])=CheckAndReadIniData(asTeachPath, Group, Key[i][j], (int)(*Parameter[i][j]));

                WriteIniData(asTeachPath, Group, Key[i][j], SetEdit[i][j]->Text);
            }
        }
    }
}
//------------------------------------------------------------------------------
_fastcall TfTeach::TfTeach(TComponent* Owner)
    : TForm(Owner)
{
    bUseIniFile=false;                                                          //Steven 20240501 : Teach改存成ini
    AnsiString asString;
    ActiveMotorIndex=-1;
    TwoActiveMotorIndex=-1;
    fShow=false;
    fTechAuto=false;
    iTechIndex=1;
    iTechTask=1;
    bAutoGetHomeSensor2Position=false;
    iGetHomeSensor2PositionTask=1;
    EdtTemp->Left=-200;

    gbOutSh2->Visible=false;
    gbOutSh1->Visible=false;
    SetButton060->OnClick=SetButton140Click;
    SetButton061->OnClick=SetButton140Click;
    SetButton062->OnClick=SetButton140Click;
    SetButton063->OnClick=SetButton140Click;

    for(int i=0; i<2; i++)
    {
        for(int j=0; j<8; j++)
        {
            bInArmZHome[i][j]=true;
            bOutArmZHome[i][j]=true;
        }
    }

    TEdit *teInArm[2][8]={{setEditZ1A, setEditZ1C, setEditZ1E, setEditZ1G, setEditZ1I, setEditZ1K, setEditZ1M, setEditZ1O},
                          {setEditZ1B, setEditZ1D, setEditZ1F, setEditZ1H, setEditZ1J, setEditZ1L, setEditZ1N, setEditZ1P}};

    TEdit *teOutArm[2][8]={{setEditZ2A, setEditZ2C, setEditZ2E, setEditZ2G,setEditZ2I, setEditZ2K, setEditZ2M, setEditZ2O},
                           {setEditZ2B, setEditZ2D, setEditZ2F, setEditZ2H,setEditZ2J, setEditZ2L, setEditZ2N, setEditZ2P}};

    for(int i=0; i<2; i++)
    {
        for(int j=0; j<8; j++)
        {
            InZEditPtr[i][j]        =teInArm[i][j];

            OutZEditPtr[i][j]       =teOutArm[i][j];
        }
    }

    asString=GrapicPath+"Index.BMP";                                            //jou 2012-08-22 將大型圖片用LoadFormFile節省compiler時間 & 縮小執行檔大小
    if(FileExists(asString))
        Image8->Picture->LoadFromFile(asString);
    if(USE_PICKER_COUNT==ep16Picker)                                            //Ztex 2023.12.06 Add HT-1032
    {
        asString=GrapicPath+"44pitch.BMP";
        if(FileExists(asString))
            Image2->Picture->LoadFromFile(asString);
        asString=GrapicPath+"154pitch.BMP";
        if(FileExists(asString))
            Image4->Picture->LoadFromFile(asString);
    }
    else
    {
        asString=GrapicPath+"40pitch.BMP";
        if(FileExists(asString))
            Image2->Picture->LoadFromFile(asString);
        asString=GrapicPath+"120pitch.BMP";
        if(FileExists(asString))
            Image4->Picture->LoadFromFile(asString);
    }
    asString=GrapicPath+"pitchY25.BMP";
    if(FileExists(asString))
        Image3->Picture->LoadFromFile(asString);
    asString=GrapicPath+"pitchY60.BMP";
    if(FileExists(asString))
        Image6->Picture->LoadFromFile(asString);
    asString=GrapicPath+"Shuttle.BMP";
    if(FileExists(asString))
        Image1->Picture->LoadFromFile(asString);
    asString=GrapicPath+"Kit.BMP";
    if(FileExists(asString))
        Image5->Picture->LoadFromFile(asString);
    asString=GrapicPath+"Tray.BMP";
    if(FileExists(asString))
        Image7->Picture->LoadFromFile(asString);

    for(int i=0; i<2; i++)
    {
        for(int j=0; j<8; j++)
        {
            asString.sprintf("Picker%s", IndexSuckName[i][j]);
            TechSuckPara[0].Key[i][j]           =asString;
            TechSuckPara[0].SetEdit[i][j]       =teInArm[i][j];
            TechSuckPara[0].MotorSelect[i][j]   =InArmZIndex[i][j];

            TechSuckPara[1].Key[i][j]           =asString;
            TechSuckPara[1].SetEdit[i][j]       =teOutArm[i][j];
            TechSuckPara[1].MotorSelect[i][j]   =OutArmZIndex[i][j];

            if(j<4)                                                             //Steven 20240703 : i --> j
            {
                TechSuckPara[0].Parameter[i][j] =&Tech.iInArmZHeightSub[i][j];
                TechSuckPara[1].Parameter[i][j] =&Tech.iOutArmZHeightSub[i][j];
            }
            else
            {
                TechSuckPara[0].Parameter[i][j] =&Tech.iInArmZHeightSub_16[i][j-4];
                TechSuckPara[1].Parameter[i][j] =&Tech.iOutArmZHeightSub_16[i][j-4];
            }
        }
    }
    TechSuckPara[0].iTag=0;
    TechSuckPara[0].Group="InArmZSub";
    TechSuckPara[1].iTag=1;
    TechSuckPara[1].Group="OutArmZSub";

    TechPara.push_back(new TECH_PARA(&Tech.iInArmSafeZ1                     ,   MInArmZA        , setEditInZSafeHeight      , "setEditInZSafeHeight"        , SetButton030          , GoButton030));
    TechPara.push_back(new TECH_PARA(&Tech.iTestZ1ShutlePick                ,   MTestZ1         , setEditIndex1ToSht1Z      , "setEditIndex1ToSht1Z"        , SetButton066          , GoButton066));
    TechPara.push_back(new TECH_PARA(&Tech.iTestZ2ShutlePick                ,   MTestZ2         , setEditIndex2ToSht2Z      , "setEditIndex2ToSht2Z"        , SetButton067          , GoButton067));
    TechPara.push_back(new TECH_PARA(&Tech.iTestZDown                       ,   MTestZ1         , setEditWaitTestZDown      , "setEditWaitTestZDown"        , SetButton068          , GoButton068));    //index to socket hight
    TechPara.push_back(new TECH_PARA(&Tech.iTestZ1ShutleWait                ,   MTestZ1         , setEditTestZSafePos       , "setEditTestZSafePos"         , SetButton069          , GoButton069));
    TechPara.push_back(new TECH_PARA(&Tech.iTestZ2ShutleWait                ,   MTestZ2         , setEditTestZSafePos       , "setEditTestZSafePos"         , SetButton069          , GoButton069));    //Steven 20091113
    TechPara.push_back(new TECH_PARA(&Teach.iLoadCellY1                     ,   MTestY1         , setEdLoadCellY1           , "setEdLoadCellY1"             , SetButton068          , GoButton068));    //kevin 20190907 add loadcell pos
    TechPara.push_back(new TECH_PARA(&Teach.iLoadCellY2                     ,   MTestY2         , setEdLoadCellY2           , "setEdLoadCellY2"             , SetButton068          , GoButton068));    //kevin 20190907 add loadcell pos
    TechPara.push_back(new TECH_PARA(&Teach.iLoadCellZ1Down                 ,   MTestZ1         , setEdLoadCellZ1           , "setEdLoadCellZ1"             , SetButton068          , GoButton068));    //kevin 20190907 add loadcell pos
    TechPara.push_back(new TECH_PARA(&Teach.iLoadCellZ2Down                 ,   MTestZ2         , setEdLoadCellZ2           , "setEdLoadCellZ2"             , SetButton068          , GoButton068));    //kevin 20190907 add loadcell pos

    TechPara.push_back(new TECH_PARA(&Tech.iOutArmSafeZ1                    ,   MOutArmZA       , setEditOutZSafeHeight     , "setEditOutZSafeHeight"       , SetButton106          , GoButton106));
    TechPara.push_back(new TECH_PARA(&Tech.iTrayXLoader                     ,   MTrayX          , setEditTrayLoaderX        , "setEditTrayLoaderX"          , SetButton140          , GoButton140));
    TechPara.push_back(new TECH_PARA(&Tech.iTrayXEmpty                      ,   MTrayX          , setEditTrayEmptyX         , "setEditTrayEmptyX"           , SetButton141          , GoButton141));
    TechPara.push_back(new TECH_PARA(&Tech.iTrayXColor                      ,   MTrayX          , setEditTrayColorX         , "setEditTrayColorX"           , SetButton142          , GoButton142));
    TechPara.push_back(new TECH_PARA(&Tech.iTrayXAuto1                      ,   MTrayX          , setEditTrayAuto1X         , "setEditTrayAuto1X"           , SetButton143          , GoButton143));
    TechPara.push_back(new TECH_PARA(&Tech.iTrayXAuto2                      ,   MTrayX          , setEditTrayAuto2X         , "setEditTrayAuto2X"           , SetButton144          , GoButton144));
    TechPara.push_back(new TECH_PARA(&Tech.iTrayXAuto3                      ,   MTrayX          , setEditTrayAuto3X         , "setEditTrayAuto3X"           , SetButton145          , GoButton145));
    TechPara.push_back(new TECH_PARA(&Tech.iTrayXAuto4                      ,   MTrayX          , setEdtAuto4               , "setEdtAuto4"                 , btnAuto4              , GoBtnAuto4));
    TechPara.push_back(new TECH_PARA(&Tech.iTrayXAuto5                      ,   MTrayX          , setEdtAuto5               , "setEdtAuto5"                 , btnAuto5              , GoBtnAuto5));
    TechPara.push_back(new TECH_PARA(&Tech.iTrayXAuto6                      ,   MTrayX          , setEdtAuto6               , "setEdtAuto6"                 , btnAuto6              , GoBtnAuto6));

    if(USE_PICKER_COUNT==ep16Picker)
    {
        TechPara.push_back(new TECH_PARA(&Tech.iInArmX40Pitch               ,   MInArmPitch     , setEditInXPitch40         , "setEditInXPitch40"           , SetButtonInX140       , GoButton003));
        TechPara.push_back(new TECH_PARA(&Tech.iInArmX120Pitch              ,   MInArmPitch     , setEditInXPitch120        , "setEditInXPitch120"          , SetButtonInX1120      , GoButton005));
        TechPara.push_back(new TECH_PARA(&Tech.iInArmX40Pitch2              ,   MInArmPitchX2   , setEditInX240             , "setEditInX240"               , SetButtonInX240       , GoButtonInX240));
        TechPara.push_back(new TECH_PARA(&Tech.iInArmX120Pitch2             ,   MInArmPitchX2   , setEditInX2120            , "setEditInX2120"              , SetButtonInX2120      , GoButtonInX2120));
        TechPara.push_back(new TECH_PARA(&Tech.iInArmY15Pitch               ,   MInArmPitchY    , setEditInY15              , "setEditInY15"                , SetButtonInY15        , GoButtonInY15));
        TechPara.push_back(new TECH_PARA(&Tech.iInArmY60Pitch               ,   MInArmPitchY    , setEditInY60              , "setEditInY60"                , SetButtonInY60        , GoButtonInY60));
        TechPara.push_back(new TECH_PARA(&Tech.iOutArmX40Pitch              ,   MOutArmPitch    , setEditOutXPitch40        , "setEditOutXPitch40"          , SetButtonOutX140      , GoButton004));
        TechPara.push_back(new TECH_PARA(&Tech.iOutArmX120Pitch             ,   MOutArmPitch    , setEditOutXPitch120       , "setEditOutXPitch120"         , SetButtonOutX1120     , GoButton006));
        TechPara.push_back(new TECH_PARA(&Tech.iOutArmX40Pitch2             ,   MOutArmPitchX2  , setEditOutX240            , "setEditOutX240"              , SetButtonOutX240      , GoButtonOutX240));
        TechPara.push_back(new TECH_PARA(&Tech.iOutArmX120Pitch2            ,   MOutArmPitchX2  , setEditOutX2120           , "setEditOutX2120"             , SetButtonOutX2120     , GoButtonOutX2120));
        TechPara.push_back(new TECH_PARA(&Tech.iOutArmY15Pitch              ,   MOutArmPitchY   , setEditOutY15             , "setEditOutY15"               , SetButtonOutY15       , GoButtonOutY15));
        TechPara.push_back(new TECH_PARA(&Tech.iOutArmY60Pitch              ,   MOutArmPitchY   , setEditOutY60             , "setEditOutY60"               , SetButtonOutY60       , GoButtonOutY60));
    }
    else
    {
        TechPara.push_back(new TECH_PARA(&Tech.iInArmX40Pitch               ,   MInArmPitch     , setEditInXPitch40         , "setEditInXPitch40"           , SetButtonInX140       , GoButton003));
        TechPara.push_back(new TECH_PARA(&Tech.iInArmX120Pitch              ,   MInArmPitch     , setEditInXPitch120        , "setEditInXPitch120"          , SetButtonInX1120      , GoButton005));
        TechPara.push_back(new TECH_PARA(&Tech.iInArmX40Pitch2              ,   MInArmPitchX2   , setEditInX240             , "setEditInX240"               , SetButtonInX240       , GoButtonInX240));
        TechPara.push_back(new TECH_PARA(&Tech.iInArmX120Pitch2             ,   MInArmPitchX2   , setEditInX2120            , "setEditInX2120"              , SetButtonInX2120      , GoButtonInX2120));
        TechPara.push_back(new TECH_PARA(&Tech.iInArmX40Pitch3              ,   MInArmPitchX3   , setEditInX340             , "setEditInX340"               , SetButtonInX340       , GoButtonInX340));
        TechPara.push_back(new TECH_PARA(&Tech.iInArmX120Pitch3             ,   MInArmPitchX3   , setEditInX3120            , "setEditInX3120"              , SetButtonInX3120      , GoButtonInX3120));
        TechPara.push_back(new TECH_PARA(&Tech.iInArmX40Pitch4              ,   MInArmPitchX4   , setEditInX440             , "setEditInX440"               , SetButtonInX440       , GoButtonInX440));
        TechPara.push_back(new TECH_PARA(&Tech.iInArmX120Pitch4             ,   MInArmPitchX4   , setEditInX4120            , "setEditInX4120"              , SetButtonInX4120      , GoButtonInX4120));

        TechPara.push_back(new TECH_PARA(&Tech.iInArmY15Pitch               ,   MInArmPitchY    , setEditInY15              , "setEditInY15"                , SetButtonInY15        , GoButtonInY15));
        TechPara.push_back(new TECH_PARA(&Tech.iInArmY60Pitch               ,   MInArmPitchY    , setEditInY60              , "setEditInY60"                , SetButtonInY60        , GoButtonInY60));

        TechPara.push_back(new TECH_PARA(&Tech.iOutArmX40Pitch              ,   MOutArmPitch    , setEditOutXPitch40        , "setEditOutXPitch40"          , SetButtonOutX140      , GoButton004));
        TechPara.push_back(new TECH_PARA(&Tech.iOutArmX120Pitch             ,   MOutArmPitch    , setEditOutXPitch120       , "setEditOutXPitch120"         , SetButtonOutX1120     , GoButton006));
        TechPara.push_back(new TECH_PARA(&Tech.iOutArmX40Pitch2             ,   MOutArmPitchX2  , setEditOutX240            , "setEditOutX240"              , SetButtonOutX240      , GoButtonOutX240));
        TechPara.push_back(new TECH_PARA(&Tech.iOutArmX120Pitch2            ,   MOutArmPitchX2  , setEditOutX2120           , "setEditOutX2120"             , SetButtonOutX2120     , GoButtonOutX2120));
        TechPara.push_back(new TECH_PARA(&Tech.iOutArmX40Pitch3             ,   MOutArmPitchX3  , setEditOutX340            , "setEditOutX340"              , SetButtonOutX340      , GoButtonOutX340));
        TechPara.push_back(new TECH_PARA(&Tech.iOutArmX120Pitch3            ,   MOutArmPitchX3  , setEditOutX3120           , "setEditOutX3120"             , SetButtonOutX3120     , GoButtonOutX3120));
        TechPara.push_back(new TECH_PARA(&Tech.iOutArmX40Pitch4             ,   MOutArmPitchX4  , setEditOutX440            , "setEditOutX440"              , SetButtonOutX440      , GoButtonOutX440));
        TechPara.push_back(new TECH_PARA(&Tech.iOutArmX120Pitch4            ,   MOutArmPitchX4  , setEditOutX4120           , "setEditOutX4120"             , SetButtonOutX4120     , GoButtonOutX4120));

        TechPara.push_back(new TECH_PARA(&Tech.iOutArmY15Pitch              ,   MOutArmPitchY   , setEditOutY15             , "setEditOutY15"               , SetButtonOutY15       , GoButtonOutY15));
        TechPara.push_back(new TECH_PARA(&Tech.iOutArmY60Pitch              ,   MOutArmPitchY   , setEditOutY60             , "setEditOutY60"               , SetButtonOutY60       , GoButtonOutY60));
    }
    TechPara.push_back(new TECH_PARA(&Tech.iTrayXClean                      ,   MTrayX          , setEditTrayCleanX         , "setEditTrayCleanX"           , SetButton146          , GoButton146));       //wei 20150826 拍拍Tray X軸 Teach
    TechPara.push_back(new TECH_PARA(&Tech.iTrayXOCR                        ,   MTrayX          , setEditTrayOCRX           , "setEditTrayOCRX"             , SetButton147          , GoButton147));       //wei 20151001 OCRTray X軸 Teach

    TechPara.push_back(new TECH_PARA(&Tech.OutSH1ZDetectPos                 ,   MInShuttle1     , setEditOutSht1KitPos      , "setEditOutSht1KitPos"        , SetButton200          , GoButton200));
    TechPara.push_back(new TECH_PARA(&Tech.OutSH2ZDetectPos                 ,   MInShuttle2     , setEditOutSht2KitPos      , "setEditOutSht2KitPos"        , SetButton201          , GoButton201));

    TechPara.push_back(new TECH_PARA(&Tech.OutSH1ZOneRowDetectPos           ,   MInShuttle1     , setEditOutSht1OneRowKit   , "setEditOutSht1OneRowKit"     , SetButton206          , GoButton206));
    TechPara.push_back(new TECH_PARA(&Tech.OutSH2ZOneRowDetectPos           ,   MInShuttle2     , setEditOutSht2OneRowKit   , "setEditOutSht2OneRowKit"     , SetButton207          , GoButton207));

    TechPara.push_back(new TECH_PARA(&Tech.iInShuttle1Left                  ,   MInShuttle1     , setEditInSht1Left         , "setEditInSht1Left"           , SetButton060          , GoButton060));
    TechPara.push_back(new TECH_PARA(&Tech.iInShuttle1Right                 ,   MInShuttle1     , setEditInSht1Right        , "setEditInSht1Right"          , SetButton061          , GoButton061));
    TechPara.push_back(new TECH_PARA(&Tech.iInShuttle2Left                  ,   MInShuttle2     , setEditInSht2Left         , "setEditInSht2Left"           , SetButton062          , GoButton062));
    TechPara.push_back(new TECH_PARA(&Tech.iInShuttle2Right                 ,   MInShuttle2     , setEditInSht2Right        , "setEditInSht2Right"          , SetButton063          , GoButton063));

    TechPara.push_back(new TECH_PARA(&Tech.iInSH1Sen7DetectPos              ,   MInShuttle1     , edtEditInSht1OctSiteKit   , "edtEditInSht1OctSiteKit"     , btnSetButton204       , btnGoButton204));
    TechPara.push_back(new TECH_PARA(&Tech.iInSH2Sen7DetectPos              ,   MInShuttle2     , edtEditInSht2OctSiteKit   , "edtEditInSht2OctSiteKit"     , btnSetButton205       , btnGoButton205));
    TechPara.push_back(new TECH_PARA(&Tech.iInSH1BarCodePos                 ,   MInShuttle1     , edtSetEditIS1BarCode      , "edtSetEditIS1BarCode"        , btnSetInSht1BarCode   , btnGoInSht1BarCode));
    TechPara.push_back(new TECH_PARA(&Tech.iInSH2BarCodePos                 ,   MInShuttle2     , edtSetEditIS2BarCode      , "edtSetEditIS2BarCode"        , btnSetInSht2BarCode   , btnGoInSht2BarCode));
    TechPara.push_back(new TECH_PARA(&Tech.iOutSH1BarCodePos                ,   MInShuttle1     , edtSetEditOS1BarCode      , "edtSetEditOS1BarCode"        , btnSetOutSht1BarCode  , btnGoOutSht1BarCode));
    TechPara.push_back(new TECH_PARA(&Tech.iOutSH2BarCodePos                ,   MInShuttle2     , edtSetEditOS2BarCode      , "edtSetEditOS2BarCode"        , btnSetOutSht2BarCode  , btnGoOutSht2BarCode));

//    TechPara.push_back(new TECH_PARA(&Tech.iShuttle1BusyHalfOffset          ,   MInShuttle1     , setEditSht1BusyHalf       , "setEditSht1BusyHalf"         , SetButton202          , GoButton202));
//    TechPara.push_back(new TECH_PARA(&Tech.iShuttle2BusyHalfOffset          ,   MInShuttle2     , setEditSht2BusyHalf       , "setEditSht2BusyHalf"         , SetButton203          , GoButton203));
    TechPara.push_back(new TECH_PARA(&Tech.iInArmLoadStagePickZ2            ,   MInArmZE        , SetEditPickLoader         , "SetEditPickLoader"           , SetButtonPick1        , GoButtonPick1));
    TechPara.push_back(new TECH_PARA(&Tech.iInArmPlatePickZ2                ,   MInArmZE        , SetEditHP                 , "SetEditHP"                   , SetButtonPick2        , GoButtonPick2));
    TechPara.push_back(new TECH_PARA(&Tech.iOutArmShuttlePickZ2             ,   MOutArmZE       , SetEditPickOutSht         , "SetEditPickOutSht"           , SetButtonPick4        , GoButtonPick4));
    TechPara.push_back(new TECH_PARA(&Tech.iOutArmPlaceFixZ1                ,   MOutArmZE       , SetEditPlaceFix           , "SetEditPlaceFix"             , SetButtonPlace1       , GoButtonPlace1));
    TechPara.push_back(new TECH_PARA(&Tech.iOutArmPlaceZ2                   ,   MOutArmZE       , SetEditPlaceAuto          , "SetEditPlaceAuto"            , SetButtonPlace2       , GoButtonPlace2));
    TechPara.push_back(new TECH_PARA(&Tech.iInArmShuttlePlaceZ              ,   MInArmZE        , SetEditPlaceInShuttle     , "SetEditPlaceInShuttle"       , SetButtonPlace3       , GoButtonPlace3));

    TechPara.push_back(new TECH_PARA(&Tech.Sht1XGabage                      ,   MInArmX         , setEditSht1XGabage        , "setEditSht1XGabage"          , btnSetSht1XGabage     , btnGoSht1XGabage));   //kevin 20220819 add place shuttle ic error  要吸起IC放置一個地方集中
    TechPara.push_back(new TECH_PARA(&Tech.Sht1YGabage                      ,   MInArmY         , setEditSht1YGabage        , "setEditSht1YGabage"          , btnSetSht1YGabage     , btnGoSht1YGabage));   //kevin 20220819 add place shuttle ic error  要吸起IC放置一個地方集中
    TechPara.push_back(new TECH_PARA(&Tech.Sht2XGabage                      ,   MInArmX         , setEditSht2XGabage        , "setEditSht2XGabage"          , btnSetSht2XGabage     , btnGoSht2XGabage));   //kevin 20220819 add place ic error  要吸起IC放置一個地方集中
    TechPara.push_back(new TECH_PARA(&Tech.Sht2YGabage                      ,   MInArmY         , setEditSht2YGabage        , "setEditSht2YGabage"          , btnSetSht2YGabage     , btnGoSht2YGabage));   //kevin 20220819 add place ic error  要吸起IC放置一個地方集中
    TechPara.push_back(new TECH_PARA(&Tech.LoadXGabage                      ,   MInArmX         , setEditLoadXGabage        , "setEditLoadXGabage"          , btnSetLoadXGabage     , btnGoLoadXGabage));   //kevin 20220819 add place ic error  要吸起IC放置一個地方集中
    TechPara.push_back(new TECH_PARA(&Tech.LoadYGabage                      ,   MInArmY         , setEditLoadYGabage        , "setEditLoadYGabage"          , btnSetLoadYGabage     , btnGoLoadYGabage));   //kevin 20220819 add place ic error  要吸起IC放置一個地方集中
    TechPara.push_back(new TECH_PARA(&Tech.iGabageX                         ,   MInArmX         , setEdGabageX              , "setEdGabageX"                , btnSetXGabage         , btnGoXGabage));       //kevin 20220819 add 放置一個地方集中
    TechPara.push_back(new TECH_PARA(&Tech.iGabageY                         ,   MInArmY         , setEdGabageY              , "setEdGabageY"                , btnSetYGabage         , btnGoYGabage));       //kevin 20220819 add 放置一個地方集中

    TechPara.push_back(new TECH_PARA(&Teach.iAutoCleanPick                  ,   MInArmZE        , SetEditAutoClean          , "SetEditAutoClean"            , SetBtnAutoCleanPick2  , GoBtnAutoCleanPick2));//kevin 20190305 add AutoCleacl pos

    TechPara.push_back(new TECH_PARA(&Tech.iInArmPreciserPlaceZ             ,   MInArmZE        , SetEditPlacePreciserZ     , "SetEditPlacePreciserZ"       , SetBtnPlacePreciser   , GoBtnPlacePreciser)); //Frank 20180410 (Steven) : InArm Preciser Station
    TechPara.push_back(new TECH_PARA(&Tech.iPreciserOpenPitch               ,   MPreciser       , setEditPreciserPitchOpen  , "setEditPreciserPitchOpen"    , SetBtnPreciserOpen    , GoBtnPreciserOpen));
    TechPara.push_back(new TECH_PARA(&Tech.iPreciserClosePitch              ,   MPreciser       , setEditPreciserPitchClose , "setEditPreciserPitchClose"   , SetBtnPreciserClose   , GoBtnPreciserClose));

    TechPara.push_back(new TECH_PARA(&Tech.iTrayLoaderZ[0]                  ,   MLoaderZ        , setLoaderZ                , "setLoaderZ"                  , setBtnLoaderZ         , GoBtnLoaderZ));       //Steven 20120822 : 加入Tray Z軸馬達
    TechPara.push_back(new TECH_PARA(&Tech.iTrayLoaderZ[1]                  ,   MEmptyZ         , setEmptyZ                 , "setEmptyZ"                   , setBtnEmptyZ          , GoBtnEmptyZ));
    TechPara.push_back(new TECH_PARA(&Tech.iTrayLoaderZ[2]                  ,   MColorZ         , setColorZ                 , "setColorZ"                   , setBtnColorZ          , GoBtnColorZ));
    TechPara.push_back(new TECH_PARA(&Tech.iTrayLoaderZ[3]                  ,   MAuto1Z         , setAuto1Z                 , "setAuto1Z"                   , setBtnAuto1Z          , GoBtnAuto1Z));
    TechPara.push_back(new TECH_PARA(&Tech.iTrayLoaderZ[4]                  ,   MAuto2Z         , setAuto2Z                 , "setAuto2Z"                   , setBtnAuto2Z          , GoBtnAuto2Z));
    TechPara.push_back(new TECH_PARA(&Tech.iTrayLoaderZ[5]                  ,   MAuto3Z         , setAuto3Z                 , "setAuto3Z"                   , setBtnAuto3Z          , GoBtnAuto3Z));
    TechPara.push_back(new TECH_PARA(&Tech.iTrayAuto4Z[0]                   ,   MAuto4Z         , setAuto4Z                 , "setAuto4Z"                   , setBtnAuto4Z          , GoBtnAuto4Z));        //Steven 20230907 : For HT-9011UC
    TechPara.push_back(new TECH_PARA(&Tech.iTrayAuto4Z[1]                   ,   MAuto5Z         , setAuto5Z                 , "setAuto5Z"                   , setBtnAuto5Z          , GoBtnAuto5Z));
    TechPara.push_back(new TECH_PARA(&Tech.iTrayAuto4Z[2]                   ,   MAuto6Z         , setAuto6Z                 , "setAuto6Z"                   , setBtnAuto6Z          , GoBtnAuto6Z));

    if(TRAY_ARM_MODE==eUnderCoveyor)                                            //Ztex 2023.12.06 Add HT-1032
    {
        TechPara.push_back(new TECH_PARA(&Tech.iTrayArmZPnP[0]              ,   MTrayZ          , setLoaderZUp              , "setLoaderZUp"                , SetBtnLoaderZUp       , GoBtnLoaderZUp));
        TechPara.push_back(new TECH_PARA(&Tech.iTrayArmZPnP[1]              ,   MTrayZ          , setEmptyZUp               , "setEmptyZUp"                 , SetBtnEmptyZUp        , GoBtnEmptyZUp));
        TechPara.push_back(new TECH_PARA(&Tech.iTrayArmZPnP[2]              ,   MTrayZ          , setColorZUp               , "setColorZUp"                 , SetBtnColorZUp        , GoBtnColorZUp));
        TechPara.push_back(new TECH_PARA(&Tech.iTrayArmZPnP[3]              ,   MTrayZ          , setAuto1ZUp               , "setAuto1ZUp"                 , SetBtnAuto1ZUp        , GoBtnAuto1ZUp));
        TechPara.push_back(new TECH_PARA(&Tech.iTrayArmZPnP[4]              ,   MTrayZ          , setAuto2ZUp               , "setAuto2ZUp"                 , SetBtnAuto2ZUp        , GoBtnAuto2ZUp));
        TechPara.push_back(new TECH_PARA(&Tech.iTrayArmZPnP[5]              ,   MTrayZ          , setAuto3ZUp               , "setAuto3ZUp"                 , SetBtnAuto3ZUp        , GoBtnAuto3ZUp));
        if(AUTO_EMPTY_COLOR>=3)
        {
            TechPara.push_back(new TECH_PARA(&Tech.iTrayArmZPnP[6]          ,   MTrayZ          , setAuto4ZUp               , "setAuto4ZUp"                 , SetBtnAuto4ZUp        , GoBtnAuto4ZUp));
            TechPara.push_back(new TECH_PARA(&Tech.iTrayArmZPnP[7]          ,   MTrayZ          , setAuto5ZUp               , "setAuto5ZUp"                 , SetBtnAuto5ZUp        , GoBtnAuto5ZUp));
        }

        if(AUTO_EMPTY_COLOR>=4)
        {
            TechPara.push_back(new TECH_PARA(&Tech.iTrayArmZPnP[8]          ,   MTrayZ          , setAuto6ZUp               , "setAuto6ZUp"                 , SetBtnAuto6ZUp        , GoBtnAuto6ZUp));
        }
    }

    TechPara.push_back(new TECH_PARA(&Tech.M_In_iRotateA                    ,   MInRotateKit    , setEditRotateA            , "setEditRotateA"              , SetButtonRotateA      , GoButtonRotateA));    //2013-04-12    Dell :旋轉站;馬達版
    TechPara.push_back(new TECH_PARA(&Tech.M_Out_iRotateA                   ,   MOutRotateKit   , setEditRotateOutA         , "setEditRotateOutA"           , SetButtonRotateOutA   , GoButtonRotateOutA));
    TechPara.push_back(new TECH_PARA(&Tech.M_In_iRotatePick                 ,   MInArmZE        , SetEditPickInRotate       , "SetEditPickInRotate"         , SetButtonPick3        , GoButtonPick3));
    TechPara.push_back(new TECH_PARA(&Tech.M_In_iRotatePlace                ,   MInArmZE        , SetEditPlaceInRotate      , "SetEditPlaceInRotate"        , SetButtonPlace4       , GoButtonPlace4));
    TechPara.push_back(new TECH_PARA(&Tech.M_Out_iRotatePick                ,   MOutArmZE       , SetEditPickOutRotate      , "SetEditPickOutRotate"        , SetButtonPick5        , GoButtonPick5));
    TechPara.push_back(new TECH_PARA(&Tech.M_Out_iRotatePlace               ,   MOutArmZE       , SetEditPlaceOutRotate     , "SetEditPlaceOutRotate"       , SetButtonPlace5       , GoButtonPlace5));

    TechPara.push_back(new TECH_PARA(&Tech.iInRotate[0][0]                  ,   MInRotateKit    , setEditInRA               , "setEditInRA"                 , SetButtonInRA         , GoButtonInRA));       //Steven 20170329 (Wei) : Add individual rotate motor
    TechPara.push_back(new TECH_PARA(&Tech.iInRotate[1][0]                  ,   MInRotateB      , setEditInRB               , "setEditInRB"                 , SetButtonInRB         , GoButtonInRB));
    TechPara.push_back(new TECH_PARA(&Tech.iInRotate[0][1]                  ,   MInRotateC      , setEditInRC               , "setEditInRC"                 , SetButtonInRC         , GoButtonInRC));
    TechPara.push_back(new TECH_PARA(&Tech.iInRotate[1][1]                  ,   MInRotateD      , setEditInRD               , "setEditInRD"                 , SetButtonInRD         , GoButtonInRD));
    TechPara.push_back(new TECH_PARA(&Tech.iInRotate[0][2]                  ,   MInRotateE      , setEditInRE               , "setEditInRE"                 , SetButtonInRE         , GoButtonInRE));
    TechPara.push_back(new TECH_PARA(&Tech.iInRotate[1][2]                  ,   MInRotateF      , setEditInRF               , "setEditInRF"                 , SetButtonInRF         , GoButtonInRF));
    TechPara.push_back(new TECH_PARA(&Tech.iInRotate[0][3]                  ,   MInRotateG      , setEditInRG               , "setEditInRG"                 , SetButtonInRG         , GoButtonInRG));
    TechPara.push_back(new TECH_PARA(&Tech.iInRotate[1][3]                  ,   MInRotateH      , setEditInRH               , "setEditInRH"                 , SetButtonInRH         , GoButtonInRH));
    TechPara.push_back(new TECH_PARA(&Tech.iOutRotate[0][0]                 ,   MOutRotateKit   , setEditOutRA              , "setEditOutRA"                , SetButtonOutRA        , GoButtonOutRA));
    TechPara.push_back(new TECH_PARA(&Tech.iOutRotate[1][0]                 ,   MOutRotateB     , setEditOutRB              , "setEditOutRB"                , SetButtonOutRB        , GoButtonOutRB));
    TechPara.push_back(new TECH_PARA(&Tech.iOutRotate[0][1]                 ,   MOutRotateC     , setEditOutRC              , "setEditOutRC"                , SetButtonOutRC        , GoButtonOutRC));
    TechPara.push_back(new TECH_PARA(&Tech.iOutRotate[1][1]                 ,   MOutRotateD     , setEditOutRD              , "setEditOutRD"                , SetButtonOutRD        , GoButtonOutRD));
    TechPara.push_back(new TECH_PARA(&Tech.iOutRotate[0][2]                 ,   MOutRotateE     , setEditOutRE              , "setEditOutRE"                , SetButtonOutRE        , GoButtonOutRE));
    TechPara.push_back(new TECH_PARA(&Tech.iOutRotate[1][2]                 ,   MOutRotateF     , setEditOutRF              , "setEditOutRF"                , SetButtonOutRF        , GoButtonOutRF));
    TechPara.push_back(new TECH_PARA(&Tech.iOutRotate[0][3]                 ,   MOutRotateG     , setEditOutRG              , "setEditOutRG"                , SetButtonOutRG        , GoButtonOutRG));
    TechPara.push_back(new TECH_PARA(&Tech.iOutRotate[1][3]                 ,   MOutRotateH     , setEditOutRH              , "setEditOutRH"                , SetButtonOutRH        , GoButtonOutRH));

    TechPara.push_back(new TECH_PARA(&Tech.iSH1Laser                        ,   MInShuttle1     , edtSetSht1Laser           , "edtSetSht1Laser"             , btnSetSht1Laser       , btnGoSht1Laser));     //Steven 20140228 : 雷射測距功能
    TechPara.push_back(new TECH_PARA(&Tech.iSH2Laser                        ,   MInShuttle2     , edtSetSht2Laser           , "edtSetSht2Laser"             , btnSetSht2Laser       , btnGoSht2Laser));

    TechPara.push_back(new TECH_PARA(&Tech.iOutArmPlaceFix2Z1               ,   MOutArmZE       , SetEditPlaceFix2          , "SetEditPlaceFix2"            , SetButtonPlace6       , GoButtonPlace6));     //ChungHung 20140722 add for HT9046LA

    TechPara.push_back(new TECH_PARA(&Tech.M_iTopView_Pick                  ,   MOutArmZE       , setEditTopView_Pick       , "setEditTopView_Pick"         , SetBtnTopView_Pick    , GoBtnTopView_Pick, false));    //wei 20160617 Vitrox   //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
    TechPara.push_back(new TECH_PARA(&Tech.M_iTopView_Place                 ,   MOutArmZE       , setEditTopView_Place      , "setEditTopView_Place"        , SetBtnTopView_Place   , GoBtnTopView_Place, false));   //wei 20160617 Vitrox
    TechPara.push_back(new TECH_PARA(&Tech.M_iPADView_Z                     ,   MOutArmZE       , setEditPADViewZ           , "setEditPADViewZ"             , SetBtnPADView_Z       , GoBtnPADView_Z));              //wei 20160617 Vitrox
    TechPara.push_back(new TECH_PARA(&Tech.M_iBGAView_Z                     ,   MOutArmZE       , setEditBGAViewZ           , "setEditBGAViewZ"             , SetBtnBGAView_Z       , GoBtnBGAView_Z));              //wei 20160617 Vitrox
    TechPara.push_back(new TECH_PARA(&Tech.M_iTopViewKit_Zup                ,   MAOIKit         , setEditTopViewKitZup      , "setEditTopViewKitZup"        , SetBtnTopViewKit_Zup  , GoBtnTopViewKit_Zup, false));  //wei 20160617 Vitrox
    TechPara.push_back(new TECH_PARA(&Tech.M_iTopView_KitZ                  ,   MAOIKit         , setEditTopViewKitZ        , "setEditTopViewKitZ"          , SetBtnTopViewKit_Z    , GoBtnTopViewKit_Z, false));    //wei 20160617 Vitrox

    TechPara.push_back(new TECH_PARA(&Tech.iShuttle1120Pitch                ,   MShuttle1Pitch  , setEditSht1Pitch120       , "setEditSht1Pitch120"         , SetButton153          , GoButton153));       //wei 20160914 Auto Shuttle Sensor
    TechPara.push_back(new TECH_PARA(&Tech.iShuttle1180Pitch                ,   MShuttle1Pitch  , setEditSht1Pitch180       , "setEditSht1Pitch180"         , SetButton154          , GoButton154));       //wei 20160914 Auto Shuttle Sensor
    TechPara.push_back(new TECH_PARA(&Tech.iShuttle2120Pitch                ,   MShuttle2Pitch  , setEditSht2Pitch120       , "setEditSht2Pitch120"         , SetButton155          , GoButton155));       //wei 20160914 Auto Shuttle Sensor
    TechPara.push_back(new TECH_PARA(&Tech.iShuttle2180Pitch                ,   MShuttle2Pitch  , setEditSht2Pitch180       , "setEditSht2Pitch180"         , SetButton156          , GoButton156));       //wei 20160914 Auto Shuttle Sensor

    TechPara.push_back(new TECH_PARA(&Tech.iTrayMapping                     ,   MTrayX          , setEditTrayMapX           , "setEditTrayMapX"             , SetButton160          , GoButton160));       //wei 20161219 Tray Mapping
    TechPara.push_back(new TECH_PARA(&Tech.iTrayID                          ,   MTrayX          , setEditTrayIDX            , "setEditTrayIDX"              , SetButton161          , GoButton161));       //wei 20161219 Tray Mapping

    TechPara.push_back(new TECH_PARA(&Tech.iLoadHingeR[0]                   ,   MLoadHingeR     , setEditHingeRotateLoader  , "setEditHingeRotateLoader"    , SetButton162          , GoButton162));       //wei 20170405
    TechPara.push_back(new TECH_PARA(&Tech.iLoadHingeR[1]                   ,   MLoadHingeR     , setEditHingeRotateEmpty   , "setEditHingeRotateEmpty"     , SetButton163          , GoButton163));       //wei 20170405

    TechPara.push_back(new TECH_PARA(&Tech.iCassetteArmX[0]                 ,   MCasArmX        , setEditBuffer1X           , "setEditBuffer1X"             , SetBtnBuffer1X        , GoBtnBuffer1X));     //wei 20180702 MR
    TechPara.push_back(new TECH_PARA(&Tech.iCassetteArmX[1]                 ,   MCasArmX        , setEditBuffer2X           , "setEditBuffer2X"             , SetBtnBuffer2X        , GoBtnBuffer2X));     //wei 20180702 MR
    TechPara.push_back(new TECH_PARA(&Tech.iCassetteArmX[2]                 ,   MCasArmX        , setEditBuffer3X           , "setEditBuffer3X"             , SetBtnBuffer3X        , GoBtnBuffer3X));     //wei 20180702 MR
    TechPara.push_back(new TECH_PARA(&Tech.iCassetteArmX[3]                 ,   MCasArmX        , setEditBuffer4X           , "setEditBuffer4X"             , SetBtnBuffer4X        , GoBtnBuffer4X));     //wei 20180702 MR
    TechPara.push_back(new TECH_PARA(&Tech.iCassetteArmX[4]                 ,   MCasArmX        , setEditBuffer5X           , "setEditBuffer5X"             , SetBtnBuffer5X        , GoBtnBuffer5X));     //wei 20180702 MR
    TechPara.push_back(new TECH_PARA(&Tech.iCassetteArmX[5]                 ,   MCasArmX        , setEditBuffer6X           , "setEditBuffer6X"             , SetBtnBuffer6X        , GoBtnBuffer6X));     //wei 20180702 MR
    TechPara.push_back(new TECH_PARA(&Tech.iCassetteArmX[6]                 ,   MCasArmX        , setEditBuffer7X           , "setEditBuffer7X"             , SetBtnBuffer7X        , GoBtnBuffer7X));     //wei 20180702 MR
    TechPara.push_back(new TECH_PARA(&Tech.iCassetteArmX[7]                 ,   MCasArmX        , setEditBuffer8X           , "setEditBuffer8X"             , SetBtnBuffer8X        , GoBtnBuffer8X));     //wei 20180702 MR
    TechPara.push_back(new TECH_PARA(&Tech.iCassetteArmX[8]                 ,   MCasArmX        , setEditBuffer9X           , "setEditBuffer9X"             , SetBtnBuffer9X        , GoBtnBuffer9X));     //wei 20180702 MR
    TechPara.push_back(new TECH_PARA(&Tech.iCassetteArmX[9]                 ,   MCasArmX        , setEditBuffer10X          , "setEditBuffer10X"            , SetBtnBuffer10X       , GoBtnBuffer10X));    //wei 20180702 MR

    TechPara.push_back(new TECH_PARA(&Tech.iCassetteArmZ[0]                 ,   MCasArmZ        , setEditBuffer1Z           , "setEditBuffer1Z"             , SetBtnBuffer1Z        , GoBtnBuffer1Z));     //wei 20180702 MR
    TechPara.push_back(new TECH_PARA(&Tech.iCassetteArmZ[1]                 ,   MCasArmZ        , setEditBuffer2Z           , "setEditBuffer2Z"             , SetBtnBuffer2Z        , GoBtnBuffer2Z));     //wei 20180702 MR
    TechPara.push_back(new TECH_PARA(&Tech.iCassetteArmZ[2]                 ,   MCasArmZ        , setEditBuffer3Z           , "setEditBuffer3Z"             , SetBtnBuffer3Z        , GoBtnBuffer3Z));     //wei 20180702 MR
    TechPara.push_back(new TECH_PARA(&Tech.iCassetteArmZ[3]                 ,   MCasArmZ        , setEditBuffer4Z           , "setEditBuffer4Z"             , SetBtnBuffer4Z        , GoBtnBuffer4Z));     //wei 20180702 MR
    TechPara.push_back(new TECH_PARA(&Tech.iCassetteArmZ[4]                 ,   MCasArmZ        , setEditBuffer5Z           , "setEditBuffer5Z"             , SetBtnBuffer5Z        , GoBtnBuffer5Z));     //wei 20180702 MR
    TechPara.push_back(new TECH_PARA(&Tech.iCassetteArmZ[5]                 ,   MCasArmZ        , setEditBuffer6Z           , "setEditBuffer6Z"             , SetBtnBuffer6Z        , GoBtnBuffer6Z));     //wei 20180702 MR
    TechPara.push_back(new TECH_PARA(&Tech.iCassetteArmZ[6]                 ,   MCasArmZ        , setEditBuffer7Z           , "setEditBuffer7Z"             , SetBtnBuffer7Z        , GoBtnBuffer7Z));     //wei 20180702 MR
    TechPara.push_back(new TECH_PARA(&Tech.iCassetteArmZ[7]                 ,   MCasArmZ        , setEditBuffer8Z           , "setEditBuffer8Z"             , SetBtnBuffer8Z        , GoBtnBuffer8Z));     //wei 20180702 MR
    TechPara.push_back(new TECH_PARA(&Tech.iCassetteArmZ[8]                 ,   MCasArmZ        , setEditBuffer9Z           , "setEditBuffer9Z"             , SetBtnBuffer9Z        , GoBtnBuffer9Z));     //wei 20180702 MR
    TechPara.push_back(new TECH_PARA(&Tech.iCassetteArmZ[9]                 ,   MCasArmZ        , setEditBuffer10Z          , "setEditBuffer10Z"            , SetBtnBuffer10Z       , GoBtnBuffer10Z));    //wei 20180702 MR

    TechPara.push_back(new TECH_PARA(&Tech.iLoadPortZ[0]                    ,   MCaselevatorZ   , setEditLoadPortZ          , "setEditLoadPortZ"            , SetBtnLoadPortZ       , GoBtnLoadPortZ));   //wei 20180702 MR
    TechPara.push_back(new TECH_PARA(&Tech.iLoadPortZ[1]                    ,   MCaselevatorZ   , setEditLoadSafeZ          , "setEditLoadSafeZ"            , SetBtnLoadSafeZ       , GoBtnLoadSafeZ));
    TechPara.push_back(new TECH_PARA(&Tech.iLoadPortZ[2]                    ,   MCaselevatorZ   , setEditLoadTemporaryZ     , "setEditLoadTemporaryZ"       , SetBtnLoadTemporaryZ  , GoBtnLoadTemporaryZ));

    TechPara.push_back(new TECH_PARA(&Tech.iStackedTrayX[0]                 ,   MStackedTrayX   , setEditStackedLoaderX     , "setEditStackedLoaderX"       , SetBtnStackedLoaderX  , GoBtnStackedLoaderX));
    TechPara.push_back(new TECH_PARA(&Tech.iStackedTrayX[1]                 ,   MStackedTrayX   , setEditStackedEmptyX      , "setEditStackedEmptyX"        , SetBtnStackedEmptyX   , GoBtnStackedEmptyX));
    TechPara.push_back(new TECH_PARA(&Tech.iStackedTrayX[2]                 ,   MStackedTrayX   , setEditStackedConversionX , "setEditStackedConversionX"   , SetBtnStackedConversionX, GoBtnStackedConversionX));
    TechPara.push_back(new TECH_PARA(&Tech.iStackedTrayX[3]                 ,   MStackedTrayX   , setEditStackedAuto1X      , "setEditStackedAuto1X"        , SetBtnStackedAuto1X   , GoBtnStackedAuto1X));
    TechPara.push_back(new TECH_PARA(&Tech.iStackedTrayX[4]                 ,   MStackedTrayX   , setEditStackedAuto2X      , "setEditStackedAuto2X"        , SetBtnStackedAuto2X   , GoBtnStackedAuto2X));
    TechPara.push_back(new TECH_PARA(&Tech.iStackedTrayX[5]                 ,   MStackedTrayX   , setEditStackedAuto3X      , "setEditStackedAuto3X"        , SetBtnStackedAuto3X   , GoBtnStackedAuto3X));

    TechPara.push_back(new TECH_PARA(&Tech.iStackedTrayZ[0]                 ,   MStackedTrayZ   , setEditStackedLoaderZ     , "setEditStackedLoaderZ"       , SetBtnStackedLoaderZ  , GoBtnStackedLoaderZ));
    TechPara.push_back(new TECH_PARA(&Tech.iStackedTrayZ[1]                 ,   MStackedTrayZ   , setEditStackedEmptyZ      , "setEditStackedEmptyZ"        , SetBtnStackedEmptyZ   , GoBtnStackedEmptyZ));
    TechPara.push_back(new TECH_PARA(&Tech.iStackedTrayZ[2]                 ,   MStackedTrayZ   , setEditStackedConversionZ , "setEditStackedConversionZ"   , SetBtnStackedConversionZ, GoBtnStackedConversionZ));
    TechPara.push_back(new TECH_PARA(&Tech.iStackedTrayZ[3]                 ,   MStackedTrayZ   , setEditStackedAuto1Z      , "setEditStackedAuto1Z"        , SetBtnStackedAuto1Z   , GoBtnStackedAuto1Z));
    TechPara.push_back(new TECH_PARA(&Tech.iStackedTrayZ[4]                 ,   MStackedTrayZ   , setEditStackedAuto2Z      , "setEditStackedAuto2Z"        , SetBtnStackedAuto2Z   , GoBtnStackedAuto2Z));
    TechPara.push_back(new TECH_PARA(&Tech.iStackedTrayZ[5]                 ,   MStackedTrayZ   , setEditStackedAuto3Z      , "setEditStackedAuto3Z"        , SetBtnStackedAuto3Z   , GoBtnStackedAuto3Z));

    TechPara.push_back(new TECH_PARA(&Tech.iTrayBracketZ[0]                 ,   MTrayBracketZ   , setEditTrayBracketSaftZ   , "setEditTrayBracketSaftZ"     , SetBtnTrayBracketSaftZ, GoBtnTrayBracketSaftZ));
    TechPara.push_back(new TECH_PARA(&Tech.iTrayBracketZ[1]                 ,   MTrayBracketZ   , setEditTrayBracketConversionZ, "setEditTrayBracketConversionZ", SetBtnTrayBracketConversionZ, GoBtnTrayBracketConversionZ));

/*
    TechPara.push_back(new TECH_PARA(&Tech.iLoadRobotZ[0]                   ,   MLoadRobotZ     , setEditLoadPortBufferZ    , "setEditLoadPortBufferZ"      , SetBtnLoadPortBufferZ , GoBtnLoadPortBufferZ));   //Sam 20190112 LM
    TechPara.push_back(new TECH_PARA(&Tech.iLoadRobotZ[1]                   ,   MLoadRobotZ     , setEditLoadPort1Z         , "setEditLoadPort1Z"           , SetBtnLoadPort1Z      , GoBtnLoadPort1Z));
    TechPara.push_back(new TECH_PARA(&Tech.iLoadRobotZ[2]                   ,   MLoadRobotZ     , setEditLoadPort2Z         , "setEditLoadPort2Z"           , SetBtnLoadPort2Z      , GoBtnLoadPort2Z));
    TechPara.push_back(new TECH_PARA(&Tech.iLoadRobotZ[3]                   ,   MLoadRobotZ     , setEditLoadPort3Z         , "setEditLoadPort3Z"           , SetBtnLoadPort3Z      , GoBtnLoadPort3Z));
    TechPara.push_back(new TECH_PARA(&Tech.iLoadRobotZ[4]                   ,   MLoadRobotZ     , setEditLoadPort4Z         , "setEditLoadPort4Z"           , SetBtnLoadPort4Z      , GoBtnLoadPort4Z));
*/
    TechPara.push_back(new TECH_PARA(&Tech.iLDCassetteFront                 ,   MLoaderY        , SetEditLDFront             , "SetEditLDFront"              , SetLoaderCassetteFront     , GoLoaderCassetteFront));              //Frank 20251217 add
    TechPara.push_back(new TECH_PARA(&Tech.iLDCassetteFrontBack             ,   MLoaderY        , SetEditLDFrontBack         , "SetEditLDFrontBack"          , SetLoaderCassetteFrontBack , GoLoaderCassetteFrontBack));
    TechPara.push_back(new TECH_PARA(&Tech.iLDCassetteRear                  ,   MLoaderY_CCW    , SetEditLDRear              , "SetEditLDRear"               , SetLoaderCassetteRear      , GoLoaderCassetteRear));
    TechPara.push_back(new TECH_PARA(&Tech.iLDCassetteRearBack              ,   MLoaderY_CCW    , SetEditLDRearBack          , "SetEditLDRearBack"           , SetLoaderCassetteRearBack  , GoLoaderCassetteRearBack));
    TechPara.push_back(new TECH_PARA(&Tech.iLDCassetteZStart                ,   MTrayZ          , SetEditLDCassetteZStart    , "SetEditLDCassetteZStart"     , SetLoaderCassetteZStart    , GoLoaderCassetteZStart));

    TechPara.push_back(new TECH_PARA(&Tech.iAutoCassetteFront[0]            ,   MAuto1Y         , SetEditAuto1Front          , "SetEditAuto1Front"           , SetAuto1CassetteFront      , GoAuto1CassetteFront));
    TechPara.push_back(new TECH_PARA(&Tech.iAutoCassetteFrontBack[0]        ,   MAuto1Y         , SetEditAuto1FrontBack      , "SetEditAuto1FrontBack"       , SetAuto1CassetteFrontBack  , GoAuto1CassetteFrontBack));
    TechPara.push_back(new TECH_PARA(&Tech.iAutoCassetteRear[0]             ,   MAuto1Y_CCW     , SetEditAuto1Rear           , "SetEditAuto1Rear"            , SetAuto1CassetteRear       , GoAuto1CassetteRear));
    TechPara.push_back(new TECH_PARA(&Tech.iAutoCassetteRearBack[0]         ,   MAuto1Y_CCW     , SetEditAuto1RearBack       , "SetEditAuto1RearBack"        , SetAuto1CassetteRearBack   , GoAuto1CassetteRearBack));
    TechPara.push_back(new TECH_PARA(&Tech.iAutoCassetteZStart[0]           ,   MAuto1Z         , SetEditAuto1CassetteZStart , "SetEditAuto1CassetteZStart"  , SetAuto1CassetteZStart     , GoAuto1CassetteZStart));

    TechPara.push_back(new TECH_PARA(&Tech.iAutoCassetteFront[1]            ,   MAuto2Y         , SetEditAuto2Front          , "SetEditAuto2Front"           , SetAuto2CassetteFront      , GoAuto2CassetteFront));
    TechPara.push_back(new TECH_PARA(&Tech.iAutoCassetteFrontBack[1]        ,   MAuto2Y         , SetEditAuto2FrontBack      , "SetEditAuto2FrontBack"       , SetAuto2CassetteFrontBack  , GoAuto2CassetteFrontBack));
    TechPara.push_back(new TECH_PARA(&Tech.iAutoCassetteRear[1]             ,   MAuto2Y_CCW     , SetEditAuto2Rear           , "SetEditAuto2Rear"            , SetAuto2CassetteRear       , GoAuto2CassetteRear));
    TechPara.push_back(new TECH_PARA(&Tech.iAutoCassetteRearBack[1]         ,   MAuto2Y_CCW     , SetEditAuto2RearBack       , "SetEditAuto2RearBack"        , SetAuto2CassetteRearBack   , GoAuto2CassetteRearBack));
    TechPara.push_back(new TECH_PARA(&Tech.iAutoCassetteZStart[1]           ,   MAuto2Z         , SetEditAuto2CassetteZStart , "SetEditAuto2CassetteZStart"  , SetAuto2CassetteZStart     , GoAuto2CassetteZStart));

    TechPara.push_back(new TECH_PARA(&Tech.iUnloadRobotZ[0]                 ,   MUnloadRobotZ   , setEditUnloadPortBufferZ  , "setEditUnloadPortBufferZ"    , SetBtnUnloadPortBufferZ, GoBtnUnloadPortBufferZ));
    TechPara.push_back(new TECH_PARA(&Tech.iUnloadRobotZ[1]                 ,   MUnloadRobotZ   , setEditUnloadPort1Z       , "setEditUnloadPort1Z"         , SetBtnUnloadPort1Z    , GoBtnUnloadPort1Z));
    TechPara.push_back(new TECH_PARA(&Tech.iUnloadRobotZ[2]                 ,   MUnloadRobotZ   , setEditUnloadPort2Z       , "setEditUnloadPort2Z"         , SetBtnUnloadPort2Z    , GoBtnUnloadPort2Z));
    TechPara.push_back(new TECH_PARA(&Tech.iUnloadRobotZ[3]                 ,   MUnloadRobotZ   , setEditUnloadPort3Z       , "setEditUnloadPort3Z"         , SetBtnUnloadPort3Z    , GoBtnUnloadPort3Z));
    TechPara.push_back(new TECH_PARA(&Tech.iUnloadRobotZ[4]                 ,   MUnloadRobotZ   , setEditUnloadPort4Z       , "setEditUnloadPort4Z"         , SetBtnUnloadPort4Z    , GoBtnUnloadPort4Z));

    TechPara.push_back(new TECH_PARA(&Tech.iOutArmBinBoxZ                   ,   MOutArmZE       , edtBinBoxZ                , "edtBinBoxZ"                  , btnSetBinBoxZ         , btnGoBinBoxZ));
    TechPara.push_back(new TECH_PARA(&Tech.iFix3PosL                        ,   MFix3Full       , setFix3L                  , "setFix3L"                    , setBtnFix3L           , GoBtnFix3L));             //JimmyChiu 20220927 : Stepper Motor Control in Fix3
    TechPara.push_back(new TECH_PARA(&Tech.iFix3PosR                        ,   MFix3Full       , setFix3R                  , "setFix3R"                    , setBtnFix3R           , GoBtnFix3R));

    TechPara.push_back(new TECH_PARA(&Tech.M_ScannerAOI_Z                   ,   MOutArmZE       , setEditScannerAOIZ        , "setEditScannerAOIZ"          , SetBtnScannerAOI_Z    , GoBtnScannerAOI_Z));      //Ifor 20191211 : add Scanner AOI
    TechPara.push_back(new TECH_PARA(&Tech.iMagazineTray1Pos                ,   MMagazine       , edtEditMagZTray1          , "edtEditMagZTray1"            , setBtnMagZTray1       , GoBtnMagZTray1));         //JerryYang 20220909 : add magazine
    TechPara.push_back(new TECH_PARA(&Tech.iCatchMazTray_Front              ,   MCatchMgzTray   , setEditCatchMagFront      , "setEditCatchMagFront"        , sbCatchMagFront       , sbGoCatchMagFront));      //JerryYang 20220909 : add magazine
    TechPara.push_back(new TECH_PARA(&Tech.iCatchMazTray_Rear               ,   MCatchMgzTray   , setEditCatchMagRear       , "setEditCatchMagRear"         , sbCatchMagRear        , sbGoCatchMagRear));       //JerryYang 20220909 : add magazine

    TechPara.push_back(new TECH_PARA(&Tech.iInArmNGBinBoxPlaceZ             ,   MInArmZE        , SetEditPlaceNGBinBoxZ     , "SetEditPlaceNGBinBoxZ"       , SetButtonPlaceNGBinBox, GoButtonPlaceNGBinBox));

    TechPara.push_back(new TECH_PARA(&Tech.iInArmZBasePickerAlignmentPos[0][0], MInArmZA        , setEditAlignInZAa         , "setEditAlignInZAa"           , sbAlignInZAa          , sbGoAlignInZAa));
    TechPara.push_back(new TECH_PARA(&Tech.iInArmZBasePickerAlignmentPos[0][1], MInArmZC        , setEditAlignInZAb         , "setEditAlignInZAb"           , sbAlignInZAb          , sbGoAlignInZAb));
    TechPara.push_back(new TECH_PARA(&Tech.iInArmZBasePickerAlignmentPos[0][2], MInArmZE        , setEditAlignInZAc         , "setEditAlignInZAc"           , sbAlignInZAc          , sbGoAlignInZAc));
    TechPara.push_back(new TECH_PARA(&Tech.iInArmZBasePickerAlignmentPos[0][3], MInArmZG        , setEditAlignInZAd         , "setEditAlignInZAd"           , sbAlignInZAd          , sbGoAlignInZAd));
    TechPara.push_back(new TECH_PARA(&Tech.iInArmZBasePickerAlignmentPos[1][0], MInArmZB        , setEditAlignInZBa         , "setEditAlignInZBa"           , sbAlignInZBa          , sbGoAlignInZBa));
    TechPara.push_back(new TECH_PARA(&Tech.iInArmZBasePickerAlignmentPos[1][1], MInArmZD        , setEditAlignInZBb         , "setEditAlignInZBb"           , sbAlignInZBb          , sbGoAlignInZBb));
    TechPara.push_back(new TECH_PARA(&Tech.iInArmZBasePickerAlignmentPos[1][2], MInArmZF        , setEditAlignInZBc         , "setEditAlignInZBc"           , sbAlignInZBc          , sbGoAlignInZBc));
    TechPara.push_back(new TECH_PARA(&Tech.iInArmZBasePickerAlignmentPos[1][3], MInArmZH        , setEditAlignInZBd         , "setEditAlignInZBd"           , sbAlignInZBd          , sbGoAlignInZBd));

    TechPara.push_back(new TECH_PARA(&Tech.iOutArmZBasePickerAlignmentPos[0][0], MOutArmZA      , setEditAlignOutZAa        , "setEditAlignOutZAa"          , sbAlignOutZAa         , sbGoAlignOutZAa));
    TechPara.push_back(new TECH_PARA(&Tech.iOutArmZBasePickerAlignmentPos[0][1], MOutArmZC      , setEditAlignOutZAb        , "setEditAlignOutZAb"          , sbAlignOutZAb         , sbGoAlignOutZAb));
    TechPara.push_back(new TECH_PARA(&Tech.iOutArmZBasePickerAlignmentPos[0][2], MOutArmZE      , setEditAlignOutZAc        , "setEditAlignOutZAc"          , sbAlignOutZAc         , sbGoAlignOutZAc));
    TechPara.push_back(new TECH_PARA(&Tech.iOutArmZBasePickerAlignmentPos[0][3], MOutArmZG      , setEditAlignOutZAd        , "setEditAlignOutZAd"          , sbAlignOutZAd         , sbGoAlignOutZAd));
    TechPara.push_back(new TECH_PARA(&Tech.iOutArmZBasePickerAlignmentPos[1][0], MOutArmZB      , setEditAlignOutZBa        , "setEditAlignOutZBa"          , sbAlignOutZBa         , sbGoAlignOutZBa));
    TechPara.push_back(new TECH_PARA(&Tech.iOutArmZBasePickerAlignmentPos[1][1], MOutArmZD      , setEditAlignOutZBb        , "setEditAlignOutZBb"          , sbAlignOutZBb         , sbGoAlignOutZBb));
    TechPara.push_back(new TECH_PARA(&Tech.iOutArmZBasePickerAlignmentPos[1][2], MOutArmZF      , setEditAlignOutZBc        , "setEditAlignOutZBc"          , sbAlignOutZBc         , sbGoAlignOutZBc));
    TechPara.push_back(new TECH_PARA(&Tech.iOutArmZBasePickerAlignmentPos[1][3], MOutArmZH      , setEditAlignOutZBd        , "setEditAlignOutZBd"          , sbAlignOutZBd         , sbGoAlignOutZBd));

    if(USE_PICKER_COUNT==ep16Picker)                                            //Ztex 2023.12.06 Add HT-1032
    {
        TechPara.push_back(new TECH_PARA(&Tech.iInArmZBasePickerAlignmentPos[0][4],  MInArmZA   , setEditAlignInZAe         , "setEditAlignInZAe"           , sbAlignInZAe          , sbGoAlignInZAe));
        TechPara.push_back(new TECH_PARA(&Tech.iInArmZBasePickerAlignmentPos[0][5],  MInArmZC   , setEditAlignInZAf         , "setEditAlignInZAf"           , sbAlignInZAf          , sbGoAlignInZAf));
        TechPara.push_back(new TECH_PARA(&Tech.iInArmZBasePickerAlignmentPos[0][6],  MInArmZE   , setEditAlignInZAg         , "setEditAlignInZAg"           , sbAlignInZAg          , sbGoAlignInZAg));
        TechPara.push_back(new TECH_PARA(&Tech.iInArmZBasePickerAlignmentPos[0][7],  MInArmZG   , setEditAlignInZAh         , "setEditAlignInZAh"           , sbAlignInZAh          , sbGoAlignInZAh));
        TechPara.push_back(new TECH_PARA(&Tech.iInArmZBasePickerAlignmentPos[1][4],  MInArmZB   , setEditAlignInZBe         , "setEditAlignInZBe"           , sbAlignInZBe          , sbGoAlignInZBe));
        TechPara.push_back(new TECH_PARA(&Tech.iInArmZBasePickerAlignmentPos[1][5],  MInArmZD   , setEditAlignInZBf         , "setEditAlignInZBf"           , sbAlignInZBf          , sbGoAlignInZBf));
        TechPara.push_back(new TECH_PARA(&Tech.iInArmZBasePickerAlignmentPos[1][6],  MInArmZF   , setEditAlignInZBg         , "setEditAlignInZBg"           , sbAlignInZBg          , sbGoAlignInZBg));
        TechPara.push_back(new TECH_PARA(&Tech.iInArmZBasePickerAlignmentPos[1][7],  MInArmZH   , setEditAlignInZBh         , "setEditAlignInZBh"           , sbAlignInZBh          , sbGoAlignInZBh));

        TechPara.push_back(new TECH_PARA(&Tech.iOutArmZBasePickerAlignmentPos[0][4], MOutArmZA  , setEditAlignOutZAe        , "setEditAlignOutZAe"          , sbAlignOutZAe         , sbGoAlignOutZAe));
        TechPara.push_back(new TECH_PARA(&Tech.iOutArmZBasePickerAlignmentPos[0][5], MOutArmZC  , setEditAlignOutZAf        , "setEditAlignOutZAf"          , sbAlignOutZAf         , sbGoAlignOutZAf));
        TechPara.push_back(new TECH_PARA(&Tech.iOutArmZBasePickerAlignmentPos[0][6], MOutArmZE  , setEditAlignOutZAg        , "setEditAlignOutZAg"          , sbAlignOutZAg         , sbGoAlignOutZAg));
        TechPara.push_back(new TECH_PARA(&Tech.iOutArmZBasePickerAlignmentPos[0][7], MOutArmZG  , setEditAlignOutZAh        , "setEditAlignOutZAh"          , sbAlignOutZAh         , sbGoAlignOutZAh));
        TechPara.push_back(new TECH_PARA(&Tech.iOutArmZBasePickerAlignmentPos[1][4], MOutArmZB  , setEditAlignOutZBe        , "setEditAlignOutZBe"          , sbAlignOutZBe         , sbGoAlignOutZBe));
        TechPara.push_back(new TECH_PARA(&Tech.iOutArmZBasePickerAlignmentPos[1][5], MOutArmZD  , setEditAlignOutZBf        , "setEditAlignOutZBf"          , sbAlignOutZBf         , sbGoAlignOutZBf));
        TechPara.push_back(new TECH_PARA(&Tech.iOutArmZBasePickerAlignmentPos[1][6], MOutArmZF  , setEditAlignOutZBg        , "setEditAlignOutZBg"          , sbAlignOutZBg         , sbGoAlignOutZBg));
        TechPara.push_back(new TECH_PARA(&Tech.iOutArmZBasePickerAlignmentPos[1][7], MOutArmZH  , setEditAlignOutZBh        , "setEditAlignOutZBh"          , sbAlignOutZBh         , sbGoAlignOutZBh));
    }
    TechPara.push_back(new TECH_PARA(&Tech.iMagazineStandbyPos,  MMagazine,     edtEditMagZStandby, "edtEditMagZStandby",  setBtnMagZTrayStandby,  GoBtnMagZTrayStandby));       //Ifor 20240102 add: Magazine Tray Standby Pos

    TechMotorAxle.push_back(new TECH_MotorAxle(MInArmX              , MotorInArmX));
    TechMotorAxle.push_back(new TECH_MotorAxle(MInArmY              , MotorInArmY));
    TechMotorAxle.push_back(new TECH_MotorAxle(MInArmPitch          , MotorInArmPitchX));
    TechMotorAxle.push_back(new TECH_MotorAxle(MInArmPitch          , btnInXPitch1));
    TechMotorAxle.push_back(new TECH_MotorAxle(MInArmZA             , MotorInArmZA));
    if(InOutArmPickerUseMotor!=eptUseMotCyn)
    {
        TechMotorAxle.push_back(new TECH_MotorAxle(MInArmZB         , MotorInArmZB));
        TechMotorAxle.push_back(new TECH_MotorAxle(MInArmZC         , MotorInArmZC));
        TechMotorAxle.push_back(new TECH_MotorAxle(MInArmZD         , MotorInArmZD));
        TechMotorAxle.push_back(new TECH_MotorAxle(MInArmZE         , MotorInArmZE));
        TechMotorAxle.push_back(new TECH_MotorAxle(MInArmZF         , MotorInArmZF));
        TechMotorAxle.push_back(new TECH_MotorAxle(MInArmZG         , MotorInArmZG));
        TechMotorAxle.push_back(new TECH_MotorAxle(MInArmZH         , MotorInArmZH));
    }

    if(USE_PICKER_COUNT==ep16Picker &&                                          //Ztex 2023.12.06 Add HT-1032
       InOutArmPickerUseMotor!=eptUseMotCyn)
    {
        TechMotorAxle.push_back(new TECH_MotorAxle(MInArmZAe        , MotorInArmZAe));
        TechMotorAxle.push_back(new TECH_MotorAxle(MInArmZBe        , MotorInArmZAf));
        TechMotorAxle.push_back(new TECH_MotorAxle(MInArmZAf        , MotorInArmZAg));
        TechMotorAxle.push_back(new TECH_MotorAxle(MInArmZBf        , MotorInArmZAh));
        TechMotorAxle.push_back(new TECH_MotorAxle(MInArmZAg        , MotorInArmZBe));
        TechMotorAxle.push_back(new TECH_MotorAxle(MInArmZBg        , MotorInArmZBf));
        TechMotorAxle.push_back(new TECH_MotorAxle(MInArmZAh        , MotorInArmZBg));
        TechMotorAxle.push_back(new TECH_MotorAxle(MInArmZBh        , MotorInArmZBh));
    }

    TechMotorAxle.push_back(new TECH_MotorAxle(MInShuttle1          , MotorInSh1));
    TechMotorAxle.push_back(new TECH_MotorAxle(MInShuttle2          , MotorInSh2));
    TechMotorAxle.push_back(new TECH_MotorAxle(MTestY1              , MotorIndexArm1Y));
    TechMotorAxle.push_back(new TECH_MotorAxle(MTestZ1              , MotorIndexArm1Z));
    TechMotorAxle.push_back(new TECH_MotorAxle(MTestZ2              , MotorIndexArm2Z));
    TechMotorAxle.push_back(new TECH_MotorAxle(MTestY2              , MotorIndexArm2Y));
/*  #ifdef Carry4
    TechMotorAxle.push_back(new TECH_MotorAxle(MOutShuttle1         , MotorOutSh1));
    TechMotorAxle.push_back(new TECH_MotorAxle(MOutShuttle2         , MotorOutSh2));
  #else*/
    TechMotorAxle.push_back(new TECH_MotorAxle(MOutShuttle1         , MotorOutSh1           , false));
    TechMotorAxle.push_back(new TECH_MotorAxle(MOutShuttle2         , MotorOutSh2           , false));
//  #endif
    TechMotorAxle.push_back(new TECH_MotorAxle(MOutArmX             , MotorOutArmX));
    TechMotorAxle.push_back(new TECH_MotorAxle(MOutArmY             , MotorOutArmY));
    TechMotorAxle.push_back(new TECH_MotorAxle(MOutArmPitch         , MotorOutArmPitchX));
    TechMotorAxle.push_back(new TECH_MotorAxle(MOutArmPitch         , btnOutXPitch1));
    TechMotorAxle.push_back(new TECH_MotorAxle(MOutArmZA            , MotorOutArmZA));

    if(USE_PICKER_COUNT==ep16Picker)                                            //Steven 20240502 : For HT-1032
    {
        MotorInArmZA->Caption="Z Aa";   MotorOutArmZA->Caption="Z Aa";
        MotorInArmZC->Caption="Z Ab";   MotorOutArmZC->Caption="Z Ab";
        MotorInArmZE->Caption="Z Ac";   MotorOutArmZE->Caption="Z Ac";
        MotorInArmZG->Caption="Z Ad";   MotorOutArmZG->Caption="Z Ad";
        MotorInArmZB->Caption="Z Ba";   MotorOutArmZB->Caption="Z Ba";
        MotorInArmZD->Caption="Z Bb";   MotorOutArmZD->Caption="Z Bb";
        MotorInArmZF->Caption="Z Bc";   MotorOutArmZF->Caption="Z Bc";
        MotorInArmZH->Caption="Z Bd";   MotorOutArmZH->Caption="Z Bd";
    }

    if(InOutArmPickerUseMotor!=eptUseMotCyn)
    {
        TechMotorAxle.push_back(new TECH_MotorAxle(MOutArmZB        , MotorOutArmZB));
        TechMotorAxle.push_back(new TECH_MotorAxle(MOutArmZC        , MotorOutArmZC));
        TechMotorAxle.push_back(new TECH_MotorAxle(MOutArmZD        , MotorOutArmZD));
        TechMotorAxle.push_back(new TECH_MotorAxle(MOutArmZE        , MotorOutArmZE));
        TechMotorAxle.push_back(new TECH_MotorAxle(MOutArmZF        , MotorOutArmZF));
        TechMotorAxle.push_back(new TECH_MotorAxle(MOutArmZG        , MotorOutArmZG));
        TechMotorAxle.push_back(new TECH_MotorAxle(MOutArmZH        , MotorOutArmZH));
    }
    if(USE_PICKER_COUNT==ep16Picker &&                                          //Ztex 2023.12.06 Add HT-1032
       InOutArmPickerUseMotor!=eptUseMotCyn)
    {
        TechMotorAxle.push_back(new TECH_MotorAxle(MOutArmZAe       , MotorOutArmZAe));
        TechMotorAxle.push_back(new TECH_MotorAxle(MOutArmZBe       , MotorOutArmZAf));
        TechMotorAxle.push_back(new TECH_MotorAxle(MOutArmZAf       , MotorOutArmZAg));
        TechMotorAxle.push_back(new TECH_MotorAxle(MOutArmZBf       , MotorOutArmZAh));
        TechMotorAxle.push_back(new TECH_MotorAxle(MOutArmZAg       , MotorOutArmZBe));
        TechMotorAxle.push_back(new TECH_MotorAxle(MOutArmZBg       , MotorOutArmZBf));
        TechMotorAxle.push_back(new TECH_MotorAxle(MOutArmZAh       , MotorOutArmZBg));
        TechMotorAxle.push_back(new TECH_MotorAxle(MOutArmZBh       , MotorOutArmZBh));
    }

    TechMotorAxle.push_back(new TECH_MotorAxle(MTrayX               , MotorTrayX));
    TechMotorAxle.push_back(new TECH_MotorAxle(MInArmPitchY         , MotorInArmPitchY      , USE_IN_Y_IS_AUTO_PITCH));                                       //Ztex 2023.12.06 Add HT-1032 //Ztex 2024.02.24 Add HT-1132  //JerryYang 20251218 : IN/OUT ARM支援不同模組
    TechMotorAxle.push_back(new TECH_MotorAxle(MInArmPitchY         , btnInYPitch           , USE_IN_Y_IS_AUTO_PITCH));                                       //Ztex 2023.12.06 Add HT-1032 //Ztex 2024.02.24 Add HT-1132
    TechMotorAxle.push_back(new TECH_MotorAxle(MInArmPitchX2        , MotorInArmPitchX2     , USE_IN_Y_IS_AUTO_PITCH));                                       //Ztex 2023.12.06 Add HT-1032 //Ztex 2024.02.24 Add HT-1132
    TechMotorAxle.push_back(new TECH_MotorAxle(MInArmPitchX2        , btnInXPitch2          , USE_IN_Y_IS_AUTO_PITCH));                                       //Ztex 2023.12.06 Add HT-1032 //Ztex 2024.02.24 Add HT-1132
    TechMotorAxle.push_back(new TECH_MotorAxle(MOutArmPitchY        , MotorOutArmPitchY     , USE_OUT_Y_IS_AUTO_PITCH));                                       //Ztex 2023.12.06 Add HT-1032 //Ztex 2024.02.24 Add HT-1132
    TechMotorAxle.push_back(new TECH_MotorAxle(MOutArmPitchY        , btnOutYPitch          , USE_OUT_Y_IS_AUTO_PITCH));                                       //Ztex 2023.12.06 Add HT-1032 //Ztex 2024.02.24 Add HT-1132
    TechMotorAxle.push_back(new TECH_MotorAxle(MOutArmPitchX2       , MotorOutArmPitchX2    , USE_OUT_Y_IS_AUTO_PITCH));   //Steven 20120706 : 加入OCR與Y變距  //Ztex 2023.12.06 Add HT-1032 //Ztex 2024.02.24 Add HT-1132
    TechMotorAxle.push_back(new TECH_MotorAxle(MOutArmPitchX2       , btnOutXPitch2         , USE_OUT_Y_IS_AUTO_PITCH));   //Steven 20120706 : 加入OCR與Y變距  //Ztex 2023.12.06 Add HT-1032 //Ztex 2024.02.24 Add HT-1132
    TechMotorAxle.push_back(new TECH_MotorAxle(MLoaderZ             , MotorLoaderZ          , LOAD_Z_USE_MOTOR[0]));
    TechMotorAxle.push_back(new TECH_MotorAxle(MEmptyZ              , MotorEmptyZ           , LOAD_Z_USE_MOTOR[1]));    //Steven 20120901 : 加入Tray Z
    TechMotorAxle.push_back(new TECH_MotorAxle(MColorZ              , MotorColorZ           , LOAD_Z_USE_MOTOR[2]));
    TechMotorAxle.push_back(new TECH_MotorAxle(MAuto1Z              , MotorAuto1Z           , LOAD_Z_USE_MOTOR[3]));
    TechMotorAxle.push_back(new TECH_MotorAxle(MAuto2Z              , MotorAuto2Z           , LOAD_Z_USE_MOTOR[4]));
    TechMotorAxle.push_back(new TECH_MotorAxle(MAuto3Z              , MotorAuto3Z           , LOAD_Z_USE_MOTOR[5]));
    TechMotorAxle.push_back(new TECH_MotorAxle(MAuto4Z              , MotorAuto4Z           , LOAD_Z_USE_MOTOR[6]));    //Steven 20230907 : For HT-9011UC
    TechMotorAxle.push_back(new TECH_MotorAxle(MAuto5Z              , MotorAuto5Z           , LOAD_Z_USE_MOTOR[7]));
    TechMotorAxle.push_back(new TECH_MotorAxle(MAuto6Z              , MotorAuto6Z           , LOAD_Z_USE_MOTOR[8]));
    TechMotorAxle.push_back(new TECH_MotorAxle(MInRotateKit         , MotorInRotateKit      , (USE_ROTATE_KIT && iRotate_Type!=eCynRotate)));   //2013-04-12    Dell :旋轉站;馬達版
    TechMotorAxle.push_back(new TECH_MotorAxle(MOutRotateKit        , MotorOutRotateKit     , (USE_ROTATE_KIT && iRotate_Type!=eCynRotate)));

    if(USE_PICKER_COUNT==ep16Picker)                                            //Ztex 2023.12.06 Add HT-1032
    {
        TechMotorAxle.push_back(new TECH_MotorAxle(MInArmPitchX3    , MotorInArmPitchX3));
        TechMotorAxle.push_back(new TECH_MotorAxle(MInArmPitchX4    , MotorInArmPitchX4));
        TechMotorAxle.push_back(new TECH_MotorAxle(MOutArmPitchX3   , MotorOutArmPitchX3));
        TechMotorAxle.push_back(new TECH_MotorAxle(MOutArmPitchX4   , MotorOutArmPitchX4));

        TechMotorAxle.push_back(new TECH_MotorAxle(MInArmPitchX3    , btnInXPitch3));
        TechMotorAxle.push_back(new TECH_MotorAxle(MInArmPitchX4    , btnInXPitch4));
        TechMotorAxle.push_back(new TECH_MotorAxle(MOutArmPitchX3   , btnOutXPitch3));
        TechMotorAxle.push_back(new TECH_MotorAxle(MOutArmPitchX4   , btnOutXPitch4));
    }

    TechMotorAxle.push_back(new TECH_MotorAxle(MInRotateKit         , MotorInRA             , (USE_ROTATE_KIT && (iRotate_Type==e8MotRotate || iRotate_Type==e4MotRotate || iRotate_Type==e2MotRotate2Dut))));  //Steven 20170329 (Wei) : Add individual rotate motor
    TechMotorAxle.push_back(new TECH_MotorAxle(MInRotateB           , MotorInRB             , (USE_ROTATE_KIT && (iRotate_Type==e8MotRotate || iRotate_Type==e4MotRotate))));
    TechMotorAxle.push_back(new TECH_MotorAxle(MInRotateC           , MotorInRC             , (USE_ROTATE_KIT &&  iRotate_Type==e8MotRotate)));
    TechMotorAxle.push_back(new TECH_MotorAxle(MInRotateD           , MotorInRD             , (USE_ROTATE_KIT &&  iRotate_Type==e8MotRotate)));
    TechMotorAxle.push_back(new TECH_MotorAxle(MInRotateE           , MotorInRE             , (USE_ROTATE_KIT && (iRotate_Type==e8MotRotate || iRotate_Type==e4MotRotate || iRotate_Type==e2MotRotate2Dut))));
    TechMotorAxle.push_back(new TECH_MotorAxle(MInRotateF           , MotorInRF             , (USE_ROTATE_KIT && (iRotate_Type==e8MotRotate || iRotate_Type==e4MotRotate))));
    TechMotorAxle.push_back(new TECH_MotorAxle(MInRotateG           , MotorInRG             , (USE_ROTATE_KIT &&  iRotate_Type==e8MotRotate)));
    TechMotorAxle.push_back(new TECH_MotorAxle(MInRotateH           , MotorInRH             , (USE_ROTATE_KIT &&  iRotate_Type==e8MotRotate)));
    TechMotorAxle.push_back(new TECH_MotorAxle(MOutRotateKit        , MotorOutRA            , (USE_ROTATE_KIT &&  iRotate_Type==e8MotRotate)));
    TechMotorAxle.push_back(new TECH_MotorAxle(MOutRotateB          , MotorOutRB            , (USE_ROTATE_KIT &&  iRotate_Type==e8MotRotate)));
    TechMotorAxle.push_back(new TECH_MotorAxle(MOutRotateC          , MotorOutRC            , (USE_ROTATE_KIT && (iRotate_Type==e8MotRotate || iRotate_Type==e4MotRotate || iRotate_Type==e2MotRotate2Dut))));
    TechMotorAxle.push_back(new TECH_MotorAxle(MOutRotateD          , MotorOutRD            , (USE_ROTATE_KIT && (iRotate_Type==e8MotRotate || iRotate_Type==e4MotRotate))));
    TechMotorAxle.push_back(new TECH_MotorAxle(MOutRotateE          , MotorOutRE            , (USE_ROTATE_KIT &&  iRotate_Type==e8MotRotate)));
    TechMotorAxle.push_back(new TECH_MotorAxle(MOutRotateF          , MotorOutRF            , (USE_ROTATE_KIT &&  iRotate_Type==e8MotRotate)));
    TechMotorAxle.push_back(new TECH_MotorAxle(MOutRotateG          , MotorOutRG            , (USE_ROTATE_KIT && (iRotate_Type==e8MotRotate || iRotate_Type==e4MotRotate || iRotate_Type==e2MotRotate2Dut))));
    TechMotorAxle.push_back(new TECH_MotorAxle(MOutRotateH          , MotorOutRH            , (USE_ROTATE_KIT && (iRotate_Type==e8MotRotate || iRotate_Type==e4MotRotate))));

    TechMotorAxle.push_back(new TECH_MotorAxle(MTrayZ               , MotorTrayZ            , TRAY_ARM_MODE==eUnderCoveyor));
    TechMotorAxle.push_back(new TECH_MotorAxle(MFix3Full            , MotorFix3             , FIX3_FULL_PLACE==Fix3K_UseStepperMotor));     //JimmyChiu 20220927 : Stepper Motor Control in Fix3

    TechMotorAxle.push_back(new TECH_MotorAxle(MInSh1LtcSenZ1       , MotorInSh1LtcZ1       , In_Shuttle_Auto_Latch));  //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料
    TechMotorAxle.push_back(new TECH_MotorAxle(MInSh1LtcSenZ2       , MotorInSh1LtcZ2       , In_Shuttle_Auto_Latch));
    TechMotorAxle.push_back(new TECH_MotorAxle(MInSh2LtcSenZ1       , MotorInSh2LtcZ1       , In_Shuttle_Auto_Latch));
    TechMotorAxle.push_back(new TECH_MotorAxle(MInSh2LtcSenZ2       , MotorInSh2LtcZ2       , In_Shuttle_Auto_Latch));

    TechMotorAxle.push_back(new TECH_MotorAxle(MTrayZ               , MotorCassLDZ          , USE_LdUldCassetteMode));  //Ifor 20251216 add:Boat Carrier
    TechMotorAxle.push_back(new TECH_MotorAxle(MAuto1Z              , MotorCassAuto1Z       , USE_LdUldCassetteMode));
    TechMotorAxle.push_back(new TECH_MotorAxle(MAuto2Z              , MotorCassAuto2Z       , USE_LdUldCassetteMode));
    TechMotorAxle.push_back(new TECH_MotorAxle(MLoaderY             , btnLoaderY            , INSTALL_OCR_YMot==eocrYMotInstal ||USE_LdUldCassetteMode));
    TechMotorAxle.push_back(new TECH_MotorAxle(MLoaderY_CCW         , MotorLoaderYCCW       , USE_LdUldCassetteMode));
    TechMotorAxle.push_back(new TECH_MotorAxle(MAuto1Y              , MotorAuto1YCW         , USE_LdUldCassetteMode));
    TechMotorAxle.push_back(new TECH_MotorAxle(MAuto1Y_CCW          , MotorAuto1YCCW        , USE_LdUldCassetteMode));
    TechMotorAxle.push_back(new TECH_MotorAxle(MAuto2Y              , MotorAuto2YCW         , USE_LdUldCassetteMode));
    TechMotorAxle.push_back(new TECH_MotorAxle(MAuto2Y_CCW          , MotorAuto2YCCW        , USE_LdUldCassetteMode));
    TechMotorAxle.push_back(new TECH_MotorAxle(MLdCarRotArm         , btnLoaderRotZ         , USE_LD_Rot_Arm));

    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iInArmPreciserX            ,   &Tech.iInArmPreciserY           ,   MInArmX         , MInArmY       , setEditPreciserX      , setEditPreciserY      , "setEditPreciserX"        , "setEditPreciserY"        , SetButtonPreciser , GoButtonPreciser));
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iInArmLoadStageX           ,   &Tech.iInArmLoadStageY          ,   MInArmX         , MInArmY       , setEditLoaderX        , setEditLoaderY        , "setEditLoaderX"          , "setEditLoaderY"          , SetButton020      , GoButton020));
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iInArmPlate1X              ,   &Tech.iInArmPlate1Y             ,   MInArmX         , MInArmY       , setEditHP1X           , setEditHP1Y           , "setEditHP1X"             , "setEditHP1Y"             , SetButton024      , GoButton024));
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iInArmPlate2X              ,   &Tech.iInArmPlate2Y             ,   MInArmX         , MInArmY       , setEditHP2X           , setEditHP2Y           , "setEditHP2X"             , "setEditHP2Y"             , SetButton026      , GoButton026));
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iInArmShuttle1X            ,   &Tech.iInArmShuttle1Y           ,   MInArmX         , MInArmY       , setEditInSht1X        , setEditInSht1Y        , "setEditInSht1X"          , "setEditInSht1Y"          , SetButton040      , GoButton040));
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iInArmShuttle2X            ,   &Tech.iInArmShuttle2Y           ,   MInArmX         , MInArmY       , setEditInSht2X        , setEditInSht2Y        , "setEditInSht2X"          , "setEditInSht2Y"          , SetButton042      , GoButton042));
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iOutArmShuttle1X           ,   &Tech.iOutArmShuttle1Y          ,   MOutArmX        , MOutArmY      , setEditOutSht1X       , setEditOutSht1Y       , "setEditOutSht1X"         , "setEditOutSht1Y"         , SetButton080      , GoButton080));
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iOutArmShuttle2X           ,   &Tech.iOutArmShuttle2Y          ,   MOutArmX        , MOutArmY      , setEditOutSht2X       , setEditOutSht2Y       , "setEditOutSht2X"         , "setEditOutSht2Y"         , SetButton082      , GoButton082));
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iOutArmAuto1X              ,   &Tech.iOutArmAuto1Y             ,   MOutArmX        , MOutArmY      , setEditAuto1X         , setEditAuto1Y         , "setEditAuto1X"           , "setEditAuto1Y"           , SetButton100      , GoButton100));
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iOutArmAuto2X              ,   &Tech.iOutArmAuto2Y             ,   MOutArmX        , MOutArmY      , setEditAuto2X         , setEditAuto2Y         , "setEditAuto2X"           , "setEditAuto2Y"           , SetButton102      , GoButton102));
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iOutArmAuto3X              ,   &Tech.iOutArmAuto3Y             ,   MOutArmX        , MOutArmY      , setEditAuto3X         , setEditAuto3Y         , "setEditAuto3X"           , "setEditAuto3Y"           , SetButton104      , GoButton104));
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iOutArmAuto4X              ,   &Tech.iOutArmAuto4Y             ,   MOutArmX        , MOutArmY      , setEditAuto4X         , setEditAuto4Y         , "setEditAuto4X"           , "setEditAuto4Y"           , btnSetBtnAuto4    , btnGoBtnAuto4));  //Steven 20230907 : For HT-9011UC
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iOutArmAuto5X              ,   &Tech.iOutArmAuto5Y             ,   MOutArmX        , MOutArmY      , setEditAuto5X         , setEditAuto5Y         , "setEditAuto5X"           , "setEditAuto5Y"           , btnSetBtnAuto5    , btnGoBtnAuto5));
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iOutArmAuto6X              ,   &Tech.iOutArmAuto6Y             ,   MOutArmX        , MOutArmY      , setEditAuto6X         , setEditAuto6Y         , "setEditAuto6X"           , "setEditAuto6Y"           , btnSetBtnAuto6    , btnGoBtnAuto6));
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iOutArmFix1X               ,   &Tech.iOutArmFix1Y              ,   MOutArmX        , MOutArmY      , setEditFix1X          , setEditFix1Y          , "setEditFix1X"            , "setEditFix1Y"            , SetButton120      , GoButton120));
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iOutArmFix2X               ,   &Tech.iOutArmFix2Y              ,   MOutArmX        , MOutArmY      , setEditFix2X          , setEditFix2Y          , "setEditFix2X"            , "setEditFix2Y"            , SetButton122      , GoButton122));
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iOutArmFix3X               ,   &Tech.iOutArmFix3Y              ,   MOutArmX        , MOutArmY      , setEditFix3X          , setEditFix3Y          , "setEditFix3X"            , "setEditFix3Y"            , SetButton124      , GoButton124));
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iOutArmFix4X               ,   &Tech.iOutArmFix4Y              ,   MOutArmX        , MOutArmY      , setEditFix4X          , setEditFix4Y          , "setEditFix4X"            , "setEditFix4Y"            , btnSetBtnFix4     , btnGoBtnFix4));   //Steven 20230907 : For HT-9011UC
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iOutArmFix5X               ,   &Tech.iOutArmFix5Y              ,   MOutArmX        , MOutArmY      , setEditFix5X          , setEditFix5Y          , "setEditFix5X"            , "setEditFix5Y"            , btnSetBtnFix5     , btnGoBtnFix5));
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iOutArmFix6X               ,   &Tech.iOutArmFix6Y              ,   MOutArmX        , MOutArmY      , setEditFix6X          , setEditFix6Y          , "setEditFix6X"            , "setEditFix6Y"            , btnSetBtnFix6     , btnGoBtnFix6));

    if(USE_INDEX_ARM_AXES==IndexArm_3_Axis)                                     //JimmyChiu 20220708 : add Index Arm Axis
    {
        TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iHT9040TestY1_Middle   ,   &Tech.iHT9040TestY1_Middle      ,   MTestY1         , MTestY1       , setEditIndex1ToSocketY, setEditIndex1ToSocketY, "setEditIndex1ToSocketY"  , "setEditIndex1ToSocketY"  , SetButton064      , GoButton064));
        TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iHT9040TestY1_Front    ,   &Tech.iHT9040TestY1_Front       ,   MTestY1         , MTestY1       , setEditIndex1ToSht1Y  , setEditIndex1ToSht1Y  , "setEditIndex1ToSht1Y"    , "setEditIndex1ToSht1Y"    , SetButton065      , GoButton065));
    }
    else
    {
        TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iHT9040TestY1_Middle   ,   &Tech.iHT9040TestY2_Rear        ,   MTestY1         , MTestY2       , setEditIndex1ToSocketY, setEditIndex2ToSht2Y  , "setEditIndex1ToSocketY"  , "setEditIndex2ToSht2Y"    , SetButton064      , GoButton064));
        TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iHT9040TestY2_Middle   ,   &Tech.iHT9040TestY1_Front       ,   MTestY2         , MTestY1       , setEditIndex2ToSocketY, setEditIndex1ToSht1Y  , "setEditIndex2ToSocketY"  , "setEditIndex1ToSht1Y"    , SetButton065      , GoButton065));
    }

    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iInArmPickX                ,   &Tech.iInArmPickY               ,   MInArmX         , MInArmY       , setInPickX            , setInPickY            , "setInPickX"              , "setInPickY"              , SetBtnInPick      , GoBtnInPick   ,   false));
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iOutArmPickX               ,   &Tech.iOutArmPickY              ,   MOutArmX        , MOutArmY      , setOutPickX           , setOutPickY           , "setOutPickX"             , "setOutPickY"             , SetBtnOutPick     , GoBtnOutPick  ,   false));
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iInArmAutoCleanX           ,   &Tech.iInArmAutoCleanY          ,   MInArmX         , MInArmY       , setEditAutoCleanX     , setEditAutoCleanY     , "setEditAutoCleanX"       , "setEditAutoCleanY"       , SetButton014      , GoButton014));            //Steven 20120706 : 加入OCR與Y變距
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.M_In_iRotateX              ,   &Tech.M_In_iRotateY             ,   MInArmX         , MInArmY       , setEditRotateX        , setEditRotateY        , "setEditRotateX"          , "setEditRotateY"          , SetButtonRotate   , GoButtonRotate));         //2013-04-12    Dell :旋轉站;馬達版
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.M_Out_iRotateX             ,   &Tech.M_Out_iRotateY            ,   MOutArmX        , MOutArmY      , setEditRotateOutX     , setEditRotateOutY     , "setEditRotateOutX"       , "setEditRotateOutY"       , SetButtonRotateOut, GoButtonRotateOut));
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iHP1LaserX                 ,   &Tech.iHP1LaserY                ,   MInArmX         , MInArmY       , setEdtHP1LaserX       , setEdtHP1LaserY       , "setEdtHP1LaserX"         , "setEdtHP1LaserY"         , SetBtnHP1Laser    , GoBtnHP1Laser));          //Steven 20140228 : 雷射測距功能
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iHP2LaserX                 ,   &Tech.iHP2LaserY                ,   MInArmX         , MInArmY       , setEdtHP2LaserX       , setEdtHP2LaserY       , "setEdtHP2LaserX"         , "setEdtHP2LaserY"         , SetBtnHP2Laser    , GoBtnHP2Laser));
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.M_iTopView_X               ,   &Tech.M_iTopView_Y              ,   MOutArmX        , MOutArmY      , setEditTopViewX       , setEditTopViewY       , "setEditTopViewX"         , "setEditTopViewY"         , SetBtnTopView     , GoBtnTopView  ,   false));
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.M_iPADView_X               ,   &Tech.M_iPADView_Y              ,   MOutArmX        , MOutArmY      , setEditPADViewX       , setEditPADViewY       , "setEditPADViewX"         , "setEditPADViewY"         , SetBtnPADView     , GoBtnPADView));           //wei 20160617 Vitrox
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.M_iBGAView_X               ,   &Tech.M_iBGAView_Y              ,   MOutArmX        , MOutArmY      , setEditBGAViewX       , setEditBGAViewY       , "setEditBGAViewX"         , "setEditBGAViewY"         , SetBtnBGAView     , GoBtnBGAView));           //wei 20160617 Vitrox
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.M_iTopViewSafePos_X        ,   &Tech.M_iTopViewSafePos_Y       ,   MOutArmX        , MOutArmY      , setEditSafePosX       , setEditSafePosY       , "setEditSafePosX"         , "setEditSafePosY"         , SetBtnSafePos     , GoBtnSafePos));
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iInArmDecay_X              ,   &Tech.iInArmDecay_Y             ,   MInArmX         , MInArmY       , setEdtINDecayX        , setEdtINDecayY        , "setEdtINDecayX"          , "setEdtINDecayY"          , SetDecayInButton  , GoDecayInButton));        //Ifor 20151209 :新增 Auto Decay In Arm Teach 點位
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iOutArmDecay_X             ,   &Tech.iOutArmDecay_Y            ,   MOutArmX        , MOutArmY      , setEdtOUTDecayX       , setEdtOUTDecayY       , "setEdtOUTDecayX"         , "setEdtOUTDecayY"         , SetDecayOutButton , GoDecayOutButton));       //Ifor 20151209 :新增 Auto Decay Out Arm Teach 點位
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iOutArmBinBoxX             ,   &Tech.iOutArmBinBoxY            ,   MOutArmX        , MOutArmY      , edtBinBoxX            , edtBinBoxY            , "edtBinBoxX"              , "edtBinBoxY"              , btnSetBtnBinBox   , btnGoBtnBinBox));         //kevin 20160822 bulk box
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iBottom2DIDInX             ,   &Tech.iBottom2DIDInY            ,   MInArmX         , MInArmY       , edtBt2DX              , edtBt2DY              , "edtBt2DX"                , "edtBt2DY"                , btnBottom2DSet        , btnBottom2DGo));      //Steven 20190308 : Bottom 2D
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iInArmNGBinBoxX            ,   &Tech.iInArmNGBinBoxY           ,   MInArmX         , MInArmY       , setEditNGBinBoxX      , setEditNGBinBoxY      , "setEditNGBinBoxX"        , "setEditNGBinBoxY"        , SetButtonNGBinBox     , GoButtonNGBinBox));
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.M_ScannerAOI_X             ,   &Tech.M_ScannerAOI_Y            ,   MOutArmX        , MOutArmY      , setEditScannerAOIX    , setEditScannerAOIY    , "setEditScannerAOIX"      , "setEditScannerAOIY"      , SetBtnScannerAOI      ,   GoBtnScannerAOI));  //Ifor 20191211 : add Scanner AOI
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iInArmXCCDAlignmentPos     ,   &Tech.iInArmYCCDAlignmentPos    ,   MInArmX         , MInArmY       , edtInArmXCCDPos       , edtInArmYCCDPos       , "edtInArmXCCDPos"         , "edtInArmYCCDPos"         , sbInArmCCDPos         , sbGoInArmCCDPos));    //KenHsieh 20210813 : add CCD AUTO ALIGNMENT
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iInArmXBasePickerAlignmentPos, &Tech.iInArmYBasePickerAlignmentPos,MInArmX         , MInArmY       , edtInArmXBasePickerPos, edtInArmYBasePickerPos, "edtInArmXBasePickerPos"  , "edtInArmYBasePickerPos"  , sbInArmBasePickerPos  , sbGoInArmBasePickerPos));
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iOutArmXCCDAlignmentPos    ,   &Tech.iOutArmYCCDAlignmentPos   ,   MOutArmX        , MOutArmY      , edtOutArmXCCDPos      , edtOutArmYCCDPos      , "edtOutArmXCCDPos"        , "edtOutArmYCCDPos"        , sbOutArmCCDPos        , sbGoOutArmCCDPos));
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iOutArmXBasePickerAlignmentPos,&Tech.iOutArmYBasePickerAlignmentPos,MOutArmX       , MOutArmY      , edtOutArmXBasePickerPos,edtOutArmYBasePickerPos,"edtOutArmXBasePickerPos" ,"edtOutArmYBasePickerPos"  , sbOutArmBasePickerPos , sbGoOutArmBasePickerPos));
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iInPlacementX              ,   &Tech.iInPlacementY             ,   MInPlacementX   , MInPlacementY , setEditInarmPlacementX, setEditInarmPlacementY, "setEditInarmPlacementX"  , "setEditInarmPlacementY"  , SetInarmPlacementXY   ,   GoInarmPlacementXY));   //JimmyChiu 20220908 add Pickup Error Placement
    TechTwoPara.push_back(new TECH_TWOPARA(&Tech.iInPlacementOffsetByBasicX ,   &Tech.iInPlacementOffsetByBasicY,   MInArmX         , MInArmY       , setEditInarmPlacementXOffsetByBasicSuck       , setEditInarmPlacementYOffsetByBasicSuck, "setEditInarmPlacementXOffsetByBasicSuck", "setEditInarmPlacementYOffsetByBasicSuck", SetComputeInSh2, btnStop));

    if(INSTALL_OCR_YMot==eocrYMotInstal)                                        //Frank 20250214 add
    {
        TechPara.push_back(new TECH_PARA(&Tech.iMLoaderYCarPos      ,MLoaderY  , setYCarPos     , "setYCarPos"              , sbLoaderYCarPos               , GoBtnYCarPos));
        TechPara.push_back(new TECH_PARA(&Tech.iMLoaderYOCRPos      ,MLoaderY  , setYOCRPos     , "setYOCRPos"              , sbLoaderYOCRPos               , GoBtnYOCRPos));
        TechPara.push_back(new TECH_PARA(&Tech.iMLoaderYSurePos     ,MLoaderY  , setYSurePos    , "setYSurePos"             , sbLoaderYSurePos              , GoBtnYSurePos));
    }

    TechPara.push_back(new TECH_PARA(&Tech.iInSH1SenICDetectPos     ,MInShuttle1       ,edtSetSH1_16SiteKit  ,"edtSetSH1_16SiteKit"   ,btnSetSH1_16SiteKit       ,btnGoSH1_16SiteKit));   //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料
    TechPara.push_back(new TECH_PARA(&Tech.iInSH2SenICDetectPos     ,MInShuttle2       ,edtSetSH2_16SiteKit  ,"edtSetSH2_16SiteKit"   ,btnSetSH2_16SiteKit       ,btnGoSH2_16SiteKit));   //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料

    TechPara.push_back(new TECH_PARA(&Tech.iInSH1SenICDetectZ1      ,MInSh1LtcSenZ1    ,setEditInSh1LtcSenZ1 ,"setEditInSh1LtcSenZ1"  ,SetButtonInSh1LtcSenZ1    ,GoButtonInSh1LtcSenZ1)); //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料
    TechPara.push_back(new TECH_PARA(&Tech.iInSH1SenICDetectZ2      ,MInSh1LtcSenZ2    ,setEditInSh1LtcSenZ2 ,"setEditInSh1LtcSenZ2"  ,SetButtonInSh1LtcSenZ2    ,GoButtonInSh1LtcSenZ2));
    TechPara.push_back(new TECH_PARA(&Tech.iInSH2SenICDetectZ1      ,MInSh2LtcSenZ1    ,setEditInSh2LtcSenZ1 ,"setEditInSh2LtcSenZ1"  ,SetButtonInSh2LtcSenZ1    ,GoButtonInSh2LtcSenZ1));
    TechPara.push_back(new TECH_PARA(&Tech.iInSH2SenICDetectZ2      ,MInSh2LtcSenZ2    ,setEditInSh2LtcSenZ2 ,"setEditInSh2LtcSenZ2"  ,SetButtonInSh2LtcSenZ2    ,GoButtonInSh2LtcSenZ2));

    if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)                                //Steven 20141029 : XY-Pitch
    {
        MotorInArmZE->Flat      =false;
        MotorInArmZF->Flat      =true;
        if(USE_OUT_Y_IS_AUTO_PITCH==iXYPitchVariable)                           //RogerYang 20251222 : IN/OUT ARM支援不同模組
        {
            MotorOutArmZE->Flat     =false;
            MotorOutArmZD->Flat     =true;
        }
    }
    else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)                            //Ztex 2023.12.06 Add HT-1032
    {
        MotorInArmZE->Flat      =false;
        MotorInArmZH->Flat      =true;
        MotorOutArmZE->Flat     =false;
        MotorOutArmZBe->Flat    =true;
    }
    else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                       //Ztex 2024.02.24 Add HT-1132
    {
        MotorInArmZE->Flat      =false;
        MotorInArmZD->Flat      =true;
        MotorOutArmZE->Flat     =false;
        MotorOutArmZF->Flat     =true;
    }

    TECH_MAX_ITEM=TechPara.size();
    TechTwoItem=TechTwoPara.size();
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnJogPMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fTechAuto=false;
    AnsiString S1="", S2="", S3="";                                             //Ztex 2024.08.18 Add Jog Ckeck SoftLimit

    if(CheckCanMove()==false || ActiveMotorIndex==-1 || IsCanQuickJogMove()==false)
        return;

    int NowPos=atoi(edtNowPosition->Text.c_str());
    if(abs(NowPos)>999999)                                                      //Steven 20100831 : 溢位保護
    {
        btnJogP->Down=false;
        ShowMyMessage("Position over limitation!", "移動位置超過限制!!");
        return;
    }

    if(CheckSafeDoorIsClosed()==false)
    {
        btnJogP->Down=false;
        return;
    }

    if(NowPos>=MOT[ActiveMotorIndex].Motor->PSoftLimitP)                        //Ztex 2024.08.18 Add Jog Ckeck SoftLimit
    {
        S1=AnsiString("The target position of ")+MOT[ActiveMotorIndex].Alias+AnsiString(" over positive soft limit !");
        S2=MOT[ActiveMotorIndex].Alias+AnsiString("的目標位置超過正向軟體極限!");
        S3.sprintf("%d > %d", NowPos, MOT[ActiveMotorIndex].Motor->PSoftLimitP);
        ShowMyMessage(S1, S2, S3);
        return;
    }

    SelMotSpeed=atoi(edtSpeed->Text.c_str());
    if(INDEX_MOTION_CARD==0 && (ActiveMotorIndex==MTestY1 || ActiveMotorIndex==MTestZ1 || ActiveMotorIndex==MTestZ2 || ActiveMotorIndex==MTestY2))  //Steven 20210623 : Index使用Galil
        MOT[ActiveMotorIndex].Gali_JogP(SelMotSpeed*100);
    else
        MOT[ActiveMotorIndex].JogP(SelMotSpeed);
}
//---------------------------------------------------------------------------
bool __fastcall TfTeach::IsCanQuickJogMove()
{
    #ifdef SOFT_SIMULTE
        return true;
    #else
    {
        if(ActiveMotorIndex==-1)
            return false;

        if(ActiveMotorIndex==MInArmX || ActiveMotorIndex==MInArmY)
        {
            for(int i=0; i<MAX_ARM_Row; i++)                                    //Steven 20240601 : 修正for Teach
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    MOT[InArmZIndex[i][j]].ScanMotorStatus();
                    if(MOT[InArmZIndex[i][j]].Led[iHomeLed]==false)
                    {
                        ShowMyMessage("Please let InArm Z at home position first!!", "請先讓InArm的Z軸在home的位置上!!", "Teach");
                        return false;
                    }
                }
            }
        }
        if(ActiveMotorIndex==MOutArmX || ActiveMotorIndex==MOutArmY)
        {
            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    MOT[OutArmZIndex[i][j]].ScanMotorStatus();
                    if(MOT[OutArmZIndex[i][j]].Led[iHomeLed]==false)
                    {
                        ShowMyMessage("Please let OutArm Z at home position first!!", "請先讓OutArm的Z軸在home的位置上!!", "Teach");
                        return false;
                    }
                }
            }
        }
        if(ActiveMotorIndex==MInShuttle1 || ActiveMotorIndex==MInShuttle2)
        {
            MOT[MTestZ1].Gali_ScanMotStatus();
            if(MOT[MTestZ1].Led[iHomeLed]==false)
            {
                ShowMyMessage("Please let MTestZ1 at home position first!!", "請先讓MTestZ1在home的位置上!!", "Teach");
                return false;
            }
            MOT[MTestZ2].Gali_ScanMotStatus();
            if(MOT[MTestZ2].Led[iHomeLed]==false)
            {
                ShowMyMessage("Please let MTestZ2 at home position first!!", "請先讓MTestZ2在home的位置上!!", "Teach");
                return false;
            }

            for(int i=0; i<MAX_ARM_Row; i++)
            {
                for(int j=0; j<MAX_ARM_Col; j++)
                {
                    MOT[InArmZIndex[i][j]].ScanMotorStatus();
                    if(MOT[InArmZIndex[i][j]].Led[iHomeLed]==false)                        //Steven 20091004 Start: Shuttle move need check In/Out Arm Z position
                    {
                        ShowMyMessage("Please let InArm Z at home position first!!", "請先讓InArm的Z軸在home的位置上!!", "Teach");
                        return false;
                    }
                    MOT[OutArmZIndex[i][j]].ScanMotorStatus();
                    if(MOT[OutArmZIndex[i][j]].Led[iHomeLed]==false)
                    {
                        ShowMyMessage("Please let OutArm Z at home position first!!", "請先讓OutArm的Z軸在home的位置上!!", "Teach");
                        return false;
                    }
                }
            }
        }

        if(INDEX_MOTION_CARD==0 && (ActiveMotorIndex==MTestY1 ||
                                    ActiveMotorIndex==MTestY2))                 //Steven 20210623 : Index使用Galil
        {
            MOT[MTestZ1].Gali_ScanMotStatus();
            if(MOT[MTestZ1].Led[iHomeLed]==false)                               //Steven 20091004 Start : Move Test Arm need check Z first
            {
                ShowMyMessage("Please let MTestZ1 at home position first!!", "請先讓MTestZ1在home的位置上!!", "Teach");
                return false;
            }
            MOT[MTestZ2].Gali_ScanMotStatus();
            if(MOT[MTestZ2].Led[iHomeLed]==false)
            {
                ShowMyMessage("Please let MTestZ2 at home position first!!", "請先讓MTestZ2在home的位置上!!", "Teach");
                return false;
            }
        }

        if(ActiveMotorIndex==MTrayX)
        {
            if(TRAY_ARM_MODE==eUnderCoveyor)
            {
                if(MOT[MTrayZ].Led[iHomeLed]==false)
                {
                    ShowMyMessage("Please let TrayArm Z at home position first!!", "請先讓TrayArm的Z軸在home的位置上!!", "Teach");
                    return false;
                }
                if((Cylinder[C_TrayXFloodgate1].Enable==true && Cylinder[C_TrayXFloodgate1].OffSensor()==true ) ||
                   (Cylinder[C_TrayXFloodgate2].Enable==true && Cylinder[C_TrayXFloodgate2].OffSensor()==true ) ||
                   (Cylinder[C_TrayXFloodgate3].Enable==true && Cylinder[C_TrayXFloodgate3].OffSensor()==true ) ||
                   (Cylinder[C_TrayXFloodgate4].Enable==true && Cylinder[C_TrayXFloodgate4].OffSensor()==true))
                {
                    ShowMyMessage("Tray X Floodgate Need Open", "Tray X 閘門需打開!!");
                    return false;
                }
            }
            else
            {
                if(Cylinder[C_TrayX_UpDown].Status)
                {
                    ShowMyMessage("Tray X can't move when cylinder is at down position!!", "氣缸在下始點不能進行寸動!!", "Teach");
                    return false;
                }

                MOT[MInArmY].ScanMotorStatus();
                if(MOT[MInArmY].Led[iHomeLed]==false)                           //Steven 20091006 Start: Before move Tray Arm, In/Out Arm Y must home
                {
                    ShowMyMessage("Please let InArm Y at home position first!!", "請先讓InArm的Y軸在home的位置上!!", "Teach");
                    return false;
                }
                MOT[MOutArmY].ScanMotorStatus();
                if(MOT[MOutArmY].Led[iHomeLed]==false)
                {
                    ShowMyMessage("Please let OutArm Y at home position first!!", "請先讓OutArm的Y軸在home的位置上!!", "Teach");
                    return false;
                }
            }
        }

        if(ActiveMotorIndex==MMagazine)                                         //JerryYang 20220909 : add magazine
        {
            if(MOT[MCatchMgzTray].Led[iHomeLed]==false)
            {
                ShowMyMessage("Please let Magagzine Y In Axis at home position first!!", "請先讓Magazine Y In軸在home的位置上!!", "Teach");
                return false;
            }

            if(Sen[SnMagazineTrackDetect].IsOn()==true ||
               Sen[SnMagazineTrackDetect2].IsOn()==true)                        //Sam 20221116 : Magazine TrayArm 自動補 Tray
            {
                ShowMyMessage("Magazine Track sensor is On, please remove the tray!!", "Magezine軌道sensor偵測到tray盤，請先取出tray盤!!", "Teach");
                return false;
            }
        }

        if(CheckShuttleCanMove(ActiveMotorIndex)==false)                        //Ztex 2023.06.02 Add Check_SHUTTLE_FLOODGATE_Staste
        {
            return false;
        }

        if(ActiveMotorIndex==MInArmPitchY || ActiveMotorIndex==MOutArmPitchY)   //Ztex 2024.09.05 Add 16 Picker Check Has In Safe Pos
        {
            if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||
               USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)
            {
                for(int i=0; i<InArmSuck.iMaxRow; i++)
                {
                    for(int j=0; j<InArmSuck.iMaxCol; j++)
                    {
                        MOT[InArmZIndex[i][j]].ScanMotorStatus();
                        if(MOT[InArmZIndex[i][j]].Led[iHomeLed]==false)
                        {
                            ShowMyMessage("Please let InArm Z at home position first!!", "請先讓InArm的Z軸在home的位置上!!", "Teach");
                            return false;
                        }
                        MOT[OutArmZIndex[i][j]].ScanMotorStatus();
                        if(MOT[OutArmZIndex[i][j]].Led[iHomeLed]==false)
                        {
                            ShowMyMessage("Please let InArm Z at home position first!!", "請先讓InArm的Z軸在home的位置上!!", "Teach");
                            return false;
                        }
                    }
                }
            }
        }

        return true;
    }
    #endif
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnJogNMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(CheckCanMove()==false || ActiveMotorIndex==-1 || IsCanQuickJogMove()==false)
        return;
    fTechAuto=false;
    AnsiString S1="", S2="", S3="";                                             //Ztex 2024.08.18 Add Jog Ckeck SoftLimit

    int NowPos=atoi(edtNowPosition->Text.c_str());
    if(abs(NowPos)>999999)                                                      //Steven 20100831 : 溢位保護
    {
        btnJogN->Down=false;
        ShowMyMessage("Position over limitation!", "移動位置超過限制!!");
        return;
    }

    if(CheckSafeDoorIsClosed()==false)
    {
        btnJogN->Down=false;
        return;
    }

    if(NowPos<=MOT[ActiveMotorIndex].Motor->PSoftLimitN)                        //Ztex 2024.08.18 Add Jog Ckeck SoftLimit
    {
        S1=AnsiString("The target position of ")+MOT[ActiveMotorIndex].Alias+AnsiString(" below negative soft limit !");
        S2=MOT[ActiveMotorIndex].Alias+AnsiString("的目標位置低於負向軟體極限!");
        S3.sprintf("%d <= %d", NowPos, MOT[ActiveMotorIndex].Motor->PSoftLimitN);
        ShowMyMessage(S1, S2, S3);
        return;
    }

    SelMotSpeed=atoi(edtSpeed->Text.c_str());
    if(INDEX_MOTION_CARD==0 && (ActiveMotorIndex==MTestY1 || ActiveMotorIndex==MTestZ1 ||
                                ActiveMotorIndex==MTestZ2 || ActiveMotorIndex==MTestY2))  //Steven 20210623 : Index使用Galil
        MOT[ActiveMotorIndex].Gali_JogN(SelMotSpeed*100);                       //調整Galil的速度
    else
        MOT[ActiveMotorIndex].JogN(SelMotSpeed);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnJogPMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fTechAuto=false;
    if(ActiveMotorIndex==-1)
        return;
    if(INDEX_MOTION_CARD==0 && (ActiveMotorIndex==MTestY1 || ActiveMotorIndex==MTestZ1 ||
                                ActiveMotorIndex==MTestZ2 || ActiveMotorIndex==MTestY2))  //Steven 20210623 : Index使用Galil
        MOT[ActiveMotorIndex].Gali_Command("ST", __FUNC__);
    else
        MOT[ActiveMotorIndex].PCIL132_StopMotor();
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::UpdateMotorTeachMonitor(int Index)
{
    if(Index==-1)
        return;

    unsigned int Speed;

    ScrollBar1->Min=1;
    ScrollBar1->Position=1;
    edtSpeed->Text="1";
    Speed=ScrollBar1->Position;
    MOT[Index].SetSpeed(Speed);
    if(Speed>100)
        Speed=100;
    if(Speed<1)
        Speed=1;
    Panel2->Caption=MOT[Index].NumberAlias;
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::UpdateMotorTeachTwoMonitor(int OldIndex, int Index)
{
    unsigned int Speed;
    if(ActiveMotorIndex==-1 || OldIndex==-1 || Index==-1)                       //Steven 20230731 : Add protection
        return;

    ScrollBar1->Min=1;
    ScrollBar1->Position=1;
    edtSpeed->Text="1";
    Speed=ScrollBar1->Position;
    MOT[Index].SetSpeed(Speed);
    if(Speed>100)
        Speed=100;
    if(Speed<1)
        Speed=1;
    Panel2->Caption=MOT[OldIndex].NumberAlias+" "+MOT[Index].NumberAlias;
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::ScrollBar1Change(TObject *Sender)
{
    int Speed;
    if(ActiveMotorIndex==-1)
        return;
    Speed=ScrollBar1->Position;
    MOT[ActiveMotorIndex].SetSpeed(Speed/3);
    edtSpeed->Text=Speed;
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::ScanNowMotorStatus()
{
    static TALed *pLed[10]={ALed1, ALed2, ALed3, ALed4, ALed5, ALed6, ALed7, ALed8, ALed9, ALed10};
    if(ActiveMotorIndex==-1)
    {
        for(int i=0; i<10; i++)
            pLed[i]->Value=false;
        return;
    }
    if(INDEX_MOTION_CARD==0 && (ActiveMotorIndex==MTestY1 || ActiveMotorIndex==MTestZ1 ||
                                ActiveMotorIndex==MTestZ2 || ActiveMotorIndex==MTestY2))  //Steven 20210623 : Index使用Galil
    {
        MOT[ActiveMotorIndex].Gali_ScanMotStatus();
        edtNowPosition->Text=MOT[ActiveMotorIndex].Gali_ReadPos();
        pnlEncoderPos->Caption=MOT[ActiveMotorIndex].Gali_ReadEncoderPos();
    }
    else
    {
        MOT[ActiveMotorIndex].ScanMotorStatus();
        edtNowPosition->Text=MOT[ActiveMotorIndex].ReadPos();
        if(MOT[ActiveMotorIndex].Motor->MotorType==1 || MOT[ActiveMotorIndex].Motor->MotorType==3)
            pnlEncoderPos->Caption= MOT[ActiveMotorIndex].ReadEncoderPos();
        else
            pnlEncoderPos->Caption= MOT[ActiveMotorIndex].ReadPos();
    }

    for(int i=0; i<10; i++)
        pLed[i]->Value=MOT[ActiveMotorIndex].Led[i];

/*
    if(ActiveMotorIndex==MInArmPitch || ActiveMotorIndex==MOutArmPitch ||
       ActiveMotorIndex==MInArmPitchY || ActiveMotorIndex==MOutArmPitchY)
        ALed10->Value=MOT[ActiveMotorIndex].Home2Led;
*/
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::Timer1Timer(TObject *Sender)
{
    static bool bTimerOn=false;
    if(fShow==false)
        return;

    if(ActiveMotorIndex==-1 ||
       (btnHome->Down && (MyMessageBox->fShow || fNote->fShow)))                //Steven 20230731 : Add protection
    {
        btnHome->Down=false;
    }

    ScanNowMotorStatus();
    fiosetview->ScanLed(fTeach);

    if(DoZHome()==false)
        return;

    if(bTimerOn==true)
        return;

    bTimerOn=true;

    if(btnHome->Down)
    {
        if(INDEX_MOTION_CARD==0 &&
           (ActiveMotorIndex==MTestY1 ||
            ActiveMotorIndex==MTestZ1 ||
            ActiveMotorIndex==MTestZ2 ||
            ActiveMotorIndex==MTestY2))                                         //Steven 20210623 : Index使用Galil
        {
            if(fMotorTest->DoGaliHome(ActiveMotorIndex))
            {
                btnHome->Down=false;
                edtMoveTo->Text=-fMotorTest->DoGaliHome(ActiveMotorIndex);
                MOT[ActiveMotorIndex].HomeFlag=1;
                edtSetToOffset->Text=MOT[ActiveMotorIndex].Motor->LastHomePos;
            }
        }
        else
        {
            if(ProcessSingleMotorHome(ActiveMotorIndex))
            {
                edtSetToOffset->Text=MOT[ActiveMotorIndex].Motor->LastHomePos;
                btnHome->Down=false;
                MOT[ActiveMotorIndex].SetSpeed(1);
            }
        }
    }

    DoPitch_Home();                                                             //Ztex 2023.12.15 Add Pitch X Home Twice
    bTimerOn=false;
}
//------------------------------------------------------------------------------
void __fastcall TfTeach::FormShow(TObject *Sender)
{
    EdtTemp->SetFocus();
    fShow=true;
    Left=5;
    Top =5;
    fTeach->ReadFile();                                                         //kevin 20190305 add teach.ini
//    fTeach->DoIniDataToForm();                                                //KenHsieh 20210813 : add CCD AUTO ALIGNMENT
    InitAutoAlignmentTask();                                                    //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT

    tsAOI->TabVisible=(USE_AOI_Inspection ||                                    //Steven 20140909 : AOI沒用到不要出現
                       USE_Scanner_AOI_Inspection==true ||
                       USE_Fix_AI_CCD ||
                       USE_Top_Scanner_AOI_Inspection==true);                   //Ifor 20200902 add: TFAMD Top AOI
    SetButton070->Visible=false;
    GoButton070->Visible =false;
    SetButton071->Visible=false;
    GoButton071->Visible =false;
    SetButton064->Visible=true;
    GoButton064->Visible =true;
    SetButton065->Visible=true;
    GoButton065->Visible =true;
    pnlStop->Color=(TColor)0x00DFD9CC;                                          //Steven 20230815 : Add for MotorTest

    pnlOcr_Go->Visible          =(INSTALL_OCR!=eocrUninstal);
    pnlOcr_XY->Visible          =(INSTALL_OCR!=eocrUninstal);

    pnlBottom2D_Go->Visible     =(BOTTOM_2DID!=0);
    pnlBottom2D_XY->Visible     =(BOTTOM_2DID!=0);

    pnlPrecisor_Go->Visible     =(USE_PRECISER==1);
    pnlPreciser_XY->Visible     =(USE_PRECISER==1);
    pnlPrecisor_Place->Visible  =(USE_PRECISER==1);
    grpPrecisor_Open->Visible   =(USE_PRECISER==1);

    pnlNGBin_Go->Visible        =(CUSTOMER_CODE==CC_ASE_KaohSiung_K3 && TRAY_MAPPING_GRAB==2);
    pnlNGBin_XY->Visible        =(CUSTOMER_CODE==CC_ASE_KaohSiung_K3 && TRAY_MAPPING_GRAB==2);  //KenHsieh 20220923 : add Tray Map Throw IC Function
    pnlNGBin_Place->Visible     =(CUSTOMER_CODE==CC_ASE_KaohSiung_K3 && TRAY_MAPPING_GRAB==2);

    pnlAutoClean_Go->Visible    =(IniConfig.bEnableAutoCleanFunction);
    pnlAutoClean_XY->Visible    =(IniConfig.bEnableAutoCleanFunction);
    pnlAutoClean_Pick->Visible  =(IniConfig.bEnableAutoCleanFunction);

    pgcShuttle->ActivePage      =tsShuttlePos;
    pnlInArmZ ->Visible         =(USE_PICKER_COUNT==ep16Picker);
    pnlOutArmZ->Visible         =(USE_PICKER_COUNT==ep16Picker);

    pnlInPickerCCD1->Visible    =(USE_PICKER_COUNT==ep16Picker);
    pnlInPickerCCD2->Visible    =(USE_PICKER_COUNT==ep16Picker);
    pnlInPickerCCD3->Visible    =(USE_PICKER_COUNT==ep16Picker);
    pnlOutPickerCCD1->Visible   =(USE_PICKER_COUNT==ep16Picker);
    pnlOutPickerCCD2->Visible   =(USE_PICKER_COUNT==ep16Picker);
    pnlOutPickerCCD3->Visible   =(USE_PICKER_COUNT==ep16Picker);

    pnlInDecay_Go->Visible      =(CosFunction.bESDAutoDecayTeachFunction==true);
    pnlInDecay_XY->Visible      =(CosFunction.bESDAutoDecayTeachFunction==true);
    pnlOutDecay_Go->Visible     =(CosFunction.bESDAutoDecayTeachFunction==true);
    pnlOutDecay_XY->Visible     =(CosFunction.bESDAutoDecayTeachFunction==true);

    TECH_MAX_ITEM=TechPara.size();
    for(int i=0; i<TECH_MAX_ITEM; i++)
    {
        if(TechPara[i]->funButton!=NULL)
        {
            TechPara[i]->funButton->GroupIndex=1+i;
            TechPara[i]->funButton->AllowAllUp=true;
            TechPara[i]->funButton->Tag=i;
        }

        if(TechPara[i]->btGo!=NULL)
        {
            TechPara[i]->btGo->GroupIndex=1+i;
            TechPara[i]->btGo->AllowAllUp=true;
            TechPara[i]->btGo->Tag=i;
        }
    }

    TechTwoItem=TechTwoPara.size();
    for(int i=0; i<TechTwoItem; i++)
    {
        if(TechTwoPara[i]->funButton!=NULL)
        {
            TechTwoPara[i]->funButton->GroupIndex=1+i;
            TechTwoPara[i]->funButton->AllowAllUp=true;
            TechTwoPara[i]->funButton->Tag=i;
        }

        if(TechTwoPara[i]->btGo!=NULL)
        {
            TechTwoPara[i]->btGo->GroupIndex=1+i;
            TechTwoPara[i]->btGo->AllowAllUp=true;
            TechTwoPara[i]->btGo->Tag=i;
        }
        for(int j=0; j<2; j++)
        {
            TechTwoPara[i]->SetEdit[j]->Text=(int)(*TechTwoPara[i]->Parameter[j]);
        }
    }

    TechMotorItem=TechMotorAxle.size();
    for(int i=0; i<TechMotorItem; i++)
    {
        TechMotorAxle[i]->SelButton->Tag=i;
    }

    TechSuckPara[0].funButton=btnSetInArmZ;
    TechSuckPara[0].funButton->GroupIndex=1;
    TechSuckPara[0].funButton->AllowAllUp=true;
    TechSuckPara[0].funButton->Tag=0;
    TechSuckPara[0].btGo=btnGoInArmZ;
    TechSuckPara[0].btGo->GroupIndex=1;
    TechSuckPara[0].btGo->AllowAllUp=true;
    TechSuckPara[0].btGo->Tag=0;

    TechSuckPara[1].funButton=btnSetOutArmZ;
    TechSuckPara[1].funButton->GroupIndex=1;
    TechSuckPara[1].funButton->AllowAllUp=true;
    TechSuckPara[1].funButton->Tag=0;
    TechSuckPara[1].btGo=btnGoOutArmZ;
    TechSuckPara[1].btGo->GroupIndex=1;
    TechSuckPara[1].btGo->AllowAllUp=true;
    TechSuckPara[1].btGo->Tag=1;

    fTechAuto=false;
    iTechIndex=1;
    iTechTask=1;
    for(int i=0; i<TOTAL_MOTOR; i++)
    {
        MOT[i].fCanMove=true;
        MOT[i].fCanMoveL=true;
        MOT[i].fCanMoveR=true;
        MOT[i].fCanMoveM=true;
    }

    for(int i=0; i<TECH_MAX_ITEM; i++)
    {
        if(TechPara[i]->SetEdit->Text!=NULL)
            TechPara[i]->SetEdit->Text=(int)(*TechPara[i]->Parameter);
    }

    for(int i=0; i<2; i++)
    {
        for(int j=0; j<8; j++)
        {
            if(TechSuckPara[0].SetEdit[i][j]!=NULL)
                TechSuckPara[0].SetEdit[i][j]->Text=(int)(*TechSuckPara[0].Parameter[i][j]);

            if(TechSuckPara[1].SetEdit[i][j]!=NULL)
                TechSuckPara[1].SetEdit[i][j]->Text=(int)(*TechSuckPara[1].Parameter[i][j]);
        }
    }

    TechSuckPara[0].SetEdit[iInArmYBase][iInArmXBase]->Enabled=false;
    TechSuckPara[1].SetEdit[iOutArmYBase][iOutArmXBase]->Enabled=false;

    bOutDataChange=false;
    for(int i=0; i<3; i++)
    {
        ActiveFlag[i]=false;
    }

    fiosetview->SetCompomentIO(fTeach);
    PageControl2->ActivePageIndex=8;
    PageControl2->ActivePage=tsAxleCtrl;

//    bInArmAllUp=false;
//    bOutArmAllUp=false;

    SetMotorSpeed();
    fAllMotorHome=false;

    pcHandPitch->ActivePageIndex=0;

    //jou 2012-12-11 support Bin Box start
    pnlBinBox_Go->Visible       =(IniConfig.bBinBox || CosFunction.bHWBinBox)?true:false;
    pnlBinBox_XY->Visible       =(IniConfig.bBinBox || CosFunction.bHWBinBox)?true:false;
    pnlBinBox_Place->Visible    =(IniConfig.bBinBox || CosFunction.bHWBinBox)?true:false;
    //jou 2012-12-11 support Bin Box end

    //Steven 20121009 Start: Bar Code
    palBarCodeInShuttle->Visible    =!(BAR_CODE_INSTALL==ebctUninstall);
    palBarCodeOutShuttle->Visible   =!(BAR_CODE_INSTALL==ebctUninstall);
    palLaserInShuttle->Visible      =!(USE_LASER_DISTANCE==0);
    pnlLaserHP1_Go->Visible         =!(USE_LASER_DISTANCE==0);
    pnlLaserHP1_XY->Visible         =!(USE_LASER_DISTANCE==0);
    pnlLaserHP2_Go->Visible         =!(USE_LASER_DISTANCE==0);
    pnlLaserHP2_XY->Visible         =!(USE_LASER_DISTANCE==0);
    //Steven 20121009 End: Bar Code

    //Steven 20130126 Start: Fix3滿盤功能
    setEditInSht1Left->Color        =(FIX3_FULL_PLACE==Fix3K_ShortShuttle)?clYellow:clWhite;
    setEditInSht1Right->Color       =(FIX3_FULL_PLACE==Fix3K_ShortShuttle)?clYellow:clWhite;
    setEditInSht2Left->Color        =(FIX3_FULL_PLACE==Fix3K_ShortShuttle)?clYellow:clWhite;
    setEditInSht2Right->Color       =(FIX3_FULL_PLACE==Fix3K_ShortShuttle)?clYellow:clWhite;
    setEditOutSht1KitPos->Color     =(FIX3_FULL_PLACE==Fix3K_ShortShuttle)?clYellow:clWhite;
    setEditOutSht2KitPos->Color     =(FIX3_FULL_PLACE==Fix3K_ShortShuttle)?clYellow:clWhite;
    edtEditInSht1OctSiteKit->Color  =(FIX3_FULL_PLACE==Fix3K_ShortShuttle)?clYellow:clWhite;
    edtEditInSht2OctSiteKit->Color  =(FIX3_FULL_PLACE==Fix3K_ShortShuttle)?clYellow:clWhite;
    setEditOutSht1OneRowKit->Color  =(FIX3_FULL_PLACE==Fix3K_ShortShuttle)?clYellow:clWhite;
    setEditOutSht2OneRowKit->Color  =(FIX3_FULL_PLACE==Fix3K_ShortShuttle)?clYellow:clWhite;
    edtSetEditOS1BarCode->Color     =(FIX3_FULL_PLACE==Fix3K_ShortShuttle)?clYellow:clWhite;
    edtSetEditOS2BarCode->Color     =(FIX3_FULL_PLACE==Fix3K_ShortShuttle)?clYellow:clWhite;
    edtSetEditIS1BarCode->Color     =(FIX3_FULL_PLACE==Fix3K_ShortShuttle)?clYellow:clWhite;
    edtSetEditIS2BarCode->Color     =(FIX3_FULL_PLACE==Fix3K_ShortShuttle)?clYellow:clWhite;
    setEditInSht1X->Color           =(FIX3_FULL_PLACE==Fix3K_ShortShuttle)?clYellow:clWhite;
    setEditInSht2X->Color           =(FIX3_FULL_PLACE==Fix3K_ShortShuttle)?clYellow:clWhite;
    setEditInSht1X->Color           =(FIX3_FULL_PLACE==Fix3K_ShortShuttle)?clYellow:clWhite;
    setEditInSht2X->Color           =(FIX3_FULL_PLACE==Fix3K_ShortShuttle)?clYellow:clWhite;
    setEditFix1X->Color             =(FIX3_FULL_PLACE==Fix3K_ShortShuttle)?clYellow:clWhite;
    setEditFix2X->Color             =(FIX3_FULL_PLACE==Fix3K_ShortShuttle)?clYellow:clWhite;
    setEditFix3X->Color             =(FIX3_FULL_PLACE==Fix3K_ShortShuttle)?clYellow:clWhite;
    //Steven 20130126 End: Fix3滿盤功能

    //2013-04-12    Dell :旋轉站;馬達版
    grpRotate_Axis->Visible     =((USE_ROTATE_KIT==1) && (iRotate_Type==e1MotRotate || iRotate_Type==e1MotRotate1Dut || iRotate_Type==eInOutArm1Motor));      //add One sucker with rotate  //Frank 20180512 (Steven) : modify 1 Dut 1 Motor
    grpRotate_Kit->Visible      =((USE_ROTATE_KIT==1) && (iRotate_Type==e1MotRotate || iRotate_Type==e1MotRotate1Dut || iRotate_Type==eInOutArm1Motor));      //add One sucker with rotate  //Frank 20180512 (Steven) : modify 1 Dut 1 Motor

    pnlInRot_Go->Visible        =(((USE_ROTATE_KIT==1) && (iRotate_Type!=eCynRotate)) || (USE_DIE_CLEAN==1));
    pnlInRot_XY->Visible        =(((USE_ROTATE_KIT==1) && (iRotate_Type!=eCynRotate)) || (USE_DIE_CLEAN==1));
    grpInRot_Axis->Visible      =((USE_ROTATE_KIT==1)  && (iRotate_Type==e8MotRotate || iRotate_Type==e4MotRotate || iRotate_Type==e2MotRotate2Dut));
    pnlInRot_Pick->Visible      =(USE_ROTATE_KIT==1 || USE_DIE_CLEAN==1);
    pnlInRot_Place->Visible     =(USE_ROTATE_KIT==1 || USE_DIE_CLEAN==1);

    pnlOutRot_Go->Visible       =(((USE_ROTATE_KIT==1) && (iRotate_Type!=eCynRotate)) || (USE_DIE_CLEAN==1));
    pnlOutRot_XY->Visible       =(((USE_ROTATE_KIT==1) && (iRotate_Type!=eCynRotate)) || (USE_DIE_CLEAN==1));
    grpOutRot_Axis->Visible     =((USE_ROTATE_KIT==1)  && (iRotate_Type==e8MotRotate || iRotate_Type==e4MotRotate || iRotate_Type==e2MotRotate2Dut));
    pnlOutRot_Pick->Visible     =(USE_ROTATE_KIT==1 || USE_DIE_CLEAN==1);
    pnlOutRot_Place->Visible    =(USE_ROTATE_KIT==1 || USE_DIE_CLEAN==1);

    //Steven 20170329 (Wei) : Add individual rotate motor
    //==>
    tsRotate->TabVisible        =((USE_ROTATE_KIT==1) && (iRotate_Type==e8MotRotate || iRotate_Type==e4MotRotate || iRotate_Type==e2MotRotate2Dut));
    MotorInRC->Visible          =((USE_ROTATE_KIT==1) && (iRotate_Type==e8MotRotate));
    MotorInRD->Visible          =((USE_ROTATE_KIT==1) && (iRotate_Type==e8MotRotate));
    MotorInRG->Visible          =((USE_ROTATE_KIT==1) && (iRotate_Type==e8MotRotate));
    MotorInRH->Visible          =((USE_ROTATE_KIT==1) && (iRotate_Type==e8MotRotate));
    MotorOutRA->Visible         =((USE_ROTATE_KIT==1) && (iRotate_Type==e8MotRotate));
    MotorOutRB->Visible         =((USE_ROTATE_KIT==1) && (iRotate_Type==e8MotRotate));
    MotorOutRE->Visible         =((USE_ROTATE_KIT==1) && (iRotate_Type==e8MotRotate));
    MotorOutRF->Visible         =((USE_ROTATE_KIT==1) && (iRotate_Type==e8MotRotate));

    if(iRotate_Type==e4MotRotate || iRotate_Type==e2MotRotate2Dut)
    {
        MotorOutRE->Flat=false;
        MotorOutRG->Flat=true;
        lblOutRotateFixE->Visible=false;
    }
    else
    {
        MotorOutRE->Flat=true;
        MotorOutRG->Flat=false;
        lblOutRotateFixG->Visible=false;
    }
    grbInRB->Visible            =((USE_ROTATE_KIT==1) && (iRotate_Type!=e2MotRotate2Dut));
    grbInRC->Visible            =((USE_ROTATE_KIT==1) && (iRotate_Type==e8MotRotate));
    grbInRD->Visible            =((USE_ROTATE_KIT==1) && (iRotate_Type==e8MotRotate));
    grbInRF->Visible            =((USE_ROTATE_KIT==1) && (iRotate_Type!=e2MotRotate2Dut));
    grbInRG->Visible            =((USE_ROTATE_KIT==1) && (iRotate_Type==e8MotRotate));
    grbInRH->Visible            =((USE_ROTATE_KIT==1) && (iRotate_Type==e8MotRotate));
    grbOutRA->Visible           =((USE_ROTATE_KIT==1) && (iRotate_Type!=e2MotRotate2Dut));
    grbOutRB->Visible           =((USE_ROTATE_KIT==1) && (iRotate_Type!=e2MotRotate2Dut));
    grbOutRC->Visible           =((USE_ROTATE_KIT==1) && (iRotate_Type==e8MotRotate || iRotate_Type==e2MotRotate2Dut));
    grbOutRD->Visible           =((USE_ROTATE_KIT==1) && (iRotate_Type==e8MotRotate));
    grbOutRE->Visible           =((USE_ROTATE_KIT==1) && (iRotate_Type!=e2MotRotate2Dut));
    grbOutRF->Visible           =((USE_ROTATE_KIT==1) && (iRotate_Type!=e2MotRotate2Dut));
    grbOutRG->Visible           =((USE_ROTATE_KIT==1) && (iRotate_Type==e8MotRotate || iRotate_Type==e2MotRotate2Dut));
    grbOutRH->Visible           =((USE_ROTATE_KIT==1) && (iRotate_Type==e8MotRotate));
    //<==
    //Steven 20170329 (Wei) : Add individual rotate motor

    lblInRot_XY->Caption=(USE_DIE_CLEAN==1)?"CLEAN":"Rotate";                   //2013-04-12    Dell :旋轉站;馬達版

    //Steven 20131002 : XY變距
    gbInX240mm->Visible         =(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable || USE_PICKER_COUNT==ep16Picker || USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc);//Ztex 2024.02.24 Add HT-1132
    gbInX340mm->Visible         =(USE_PICKER_COUNT==ep16Picker);
    gbInX440mm->Visible         =(USE_PICKER_COUNT==ep16Picker);
    gbOutX240mm->Visible        =(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable || USE_PICKER_COUNT==ep16Picker || USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc);//Ztex 2024.02.24 Add HT-1132
    gbOutX340mm->Visible        =(USE_PICKER_COUNT==ep16Picker);
    gbOutX440mm->Visible        =(USE_PICKER_COUNT==ep16Picker);
    gbInX2120->Visible          =(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable || USE_PICKER_COUNT==ep16Picker || USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc);//Ztex 2024.02.24 Add HT-1132
    gbInX3120->Visible          =(USE_PICKER_COUNT==ep16Picker);
    gbInX4120->Visible          =(USE_PICKER_COUNT==ep16Picker);
    gbOutX2120->Visible         =(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable || USE_PICKER_COUNT==ep16Picker || USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc);//Ztex 2024.02.24 Add HT-1132
    gbOutX3120->Visible         =(USE_PICKER_COUNT==ep16Picker);
    gbOutX4120->Visible         =(USE_PICKER_COUNT==ep16Picker);
    palXYPitch->Visible         =(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable || USE_PICKER_COUNT==ep16Picker || USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc);//Ztex 2024.02.24 Add HT-1132
    pnlYPitch->Visible          =(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable || USE_PICKER_COUNT==ep16Picker || USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc);//Ztex 2024.02.24 Add HT-1132
    if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable || USE_PICKER_COUNT==ep16Picker || USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)  //RogerYang 20251222 : IN/OUT ARM支援不同模組
    {
        bool bUse=(USE_OUT_ARM_Y_PITCH==iXYPitchVariable);                      //RogerYang 20260205 : Fix
        btnOutXPitch2->Visible      =bUse;
        setEditOutX240->Visible     =bUse;
        setEditOutX2120->Visible    =bUse;
        btnOutYPitch->Visible       =bUse;
        setEditOutY15->Visible      =bUse;
        setEditOutY60->Visible      =bUse;
        gbOutX240mm->Visible        =bUse;
        gbOutX2120->Visible         =bUse;
        gbOutY15->Visible           =bUse;
        gbOutY60->Visible           =bUse;
    }
    palXYPitch1->Visible        =(USE_PICKER_COUNT==ep16Picker);
    tsYPitch15->TabVisible      =(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable || USE_PICKER_COUNT==ep16Picker || USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc);//Ztex 2024.02.24 Add HT-1132
    tsYPitch60->TabVisible      =(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable || USE_PICKER_COUNT==ep16Picker || USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc);//Ztex 2024.02.24 Add HT-1132
    MotorInArmPitchY->Visible   =(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable || USE_PICKER_COUNT==ep16Picker || USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc);//Ztex 2024.02.24 Add HT-1132
    MotorInArmPitchX2->Visible  =(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable || USE_PICKER_COUNT==ep16Picker || USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc);//Ztex 2024.02.24 Add HT-1132
    MotorOutArmPitchY->Visible  =(USE_OUT_Y_IS_AUTO_PITCH==iXYPitchVariable || USE_PICKER_COUNT==ep16Picker || USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc);//Ztex 2024.02.24 Add HT-1132   //RogerYang 20251222 : IN/OUT ARM支援不同模組
    MotorOutArmPitchX2->Visible =(USE_OUT_Y_IS_AUTO_PITCH==iXYPitchVariable || USE_PICKER_COUNT==ep16Picker || USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc);//Ztex 2024.02.24 Add HT-1132   //RogerYang 20251222 : IN/OUT ARM支援不同模組

    pnlInArm16Z->Visible        =(USE_PICKER_COUNT==ep16Picker);
    pnlOutArm16Z->Visible       =(USE_PICKER_COUNT==ep16Picker);
    MotorInArmPitchX3->Visible  =(USE_PICKER_COUNT==ep16Picker);
    MotorInArmPitchX4->Visible  =(USE_PICKER_COUNT==ep16Picker);
    MotorOutArmPitchX3->Visible =(USE_PICKER_COUNT==ep16Picker);
    MotorOutArmPitchX4->Visible =(USE_PICKER_COUNT==ep16Picker);

    edShtCheckRange->Text=CHECK_RANGE;                                          //Steven 20160108 : 改去Teaching調整
    tsShuttleSensor->TabVisible =(AUTO_SENSOR_INSTALL==1);                      //wei 20160914 Auto Shuttle Sensor

    gbTrayMapping->Visible      =(USE_TRAY_MAPPING==etmInstall);                //wei 20161219 Tray Mapping
    tsHinge->TabVisible         =(USE_LOADER_HINGE==1);                         //wei 20170405
    tsMR   ->TabVisible         =(USE_MR_SYSTEM);
    tsPickUpErrorPlacement->TabVisible=(fMain->cInplace->InArmPlacementEnable());   //JimmyChiu 20220908 add Pickup Error Placement
    gbFix3 ->Visible            =(FIX3_FULL_PLACE==Fix3K_UseStepperMotor);          //JimmyChiu 20220927 : Stepper Motor Control in Fix3
    gbAxesCtrlFix3 ->Visible    =(FIX3_FULL_PLACE==Fix3K_UseStepperMotor);          //JimmyChiu 20220927 : Stepper Motor Control in Fix3
    SetButtonPlace6->Caption    =(FIX3_FULL_PLACE==Fix3K_UseStepperMotor)?"Fix3":"Fix2";          //JimmyChiu 20220927 : Stepper Motor Control in Fix3
    if(USE_Scanner_AOI_Inspection==true ||
       USE_Top_Scanner_AOI_Inspection==true)                                    //Ifor 20200902 add: TFAMD Top AOI
    {
        pnlPadView_Go->Visible          = false;
        pnlPadView_Place->Visible       = false;

        pnlBGAView_Go->Visible          = false;
        pnlBGAView_Place->Visible       = false;

        pnlTopViewKit_Pick->Visible     = false;
        pnlTopViewKit_Place->Visible    = false;

        pnlTopView_Go->Visible          = (USE_Top_Scanner_AOI_Inspection==true);
        pnlTopView_Pick->Visible        = (USE_Top_Scanner_AOI_Inspection==true);
        pnlTopView_Place->Visible       = (USE_Top_Scanner_AOI_Inspection==true);

        pnlAOISafePos_Go->Visible       = (USE_Top_Scanner_AOI_Inspection==true);
    }

    pnlScannerAOI_Place->Visible = false;

    //Steven 20190813 : 入Tray改用步進馬達
    //==>
    gbTrayZ->Visible=(LOAD_Z_USE_MOTOR[0] || LOAD_Z_USE_MOTOR[1] || LOAD_Z_USE_MOTOR[2] ||
                      LOAD_Z_USE_MOTOR[3] || LOAD_Z_USE_MOTOR[4] || LOAD_Z_USE_MOTOR[5] ||
                      LOAD_Z_USE_MOTOR[6] || LOAD_Z_USE_MOTOR[7] || LOAD_Z_USE_MOTOR[8]);
    grpTrayZAxis->Visible=gbTrayZ->Visible;                                     //Steven 20200723
    gbTrayArmZ->Visible=(TRAY_ARM_MODE==eUnderCoveyor);
    pnlAuto4ZUp->Visible=(AUTO_EMPTY_COLOR>=3);
    pnlAuto5ZUp->Visible=(AUTO_EMPTY_COLOR>=3);
    pnlAuto6ZUp->Visible=(AUTO_EMPTY_COLOR>=4);

    pnl6Auto_Go->Visible=(AUTO_EMPTY_COLOR>=3);
    pnlAuto6_Go->Visible=(AUTO_EMPTY_COLOR>=4);
    pnlAuto6_XY->Visible=(AUTO_EMPTY_COLOR>=4);
    pnlAuto4_XY->Visible=(AUTO_EMPTY_COLOR>=3);
    pnlAuto5_XY->Visible=(AUTO_EMPTY_COLOR>=3);
    pnlAuto6_XY->Visible=(AUTO_EMPTY_COLOR>=4);
    pnlFix4_XY->Visible =(AUTO_EMPTY_COLOR>=3);
    pnlFix5_XY->Visible =(AUTO_EMPTY_COLOR>=3);
    pnlFix6_XY->Visible =(AUTO_EMPTY_COLOR>=3);

    pnlLoaderZ->Visible=LOAD_Z_USE_MOTOR[0];
    pnlEmptyZ->Visible =LOAD_Z_USE_MOTOR[1];
    pnlColorZ->Visible =LOAD_Z_USE_MOTOR[2];
    pnlAuto1Z->Visible =LOAD_Z_USE_MOTOR[3];
    pnlAuto2Z->Visible =LOAD_Z_USE_MOTOR[4];
    pnlAuto3Z->Visible =LOAD_Z_USE_MOTOR[5];
    pnlAuto4Z->Visible =LOAD_Z_USE_MOTOR[6];                                    //Steven 20230907 : For HT-9011UC
    pnlAuto5Z->Visible =LOAD_Z_USE_MOTOR[7];
    pnlAuto6Z->Visible =LOAD_Z_USE_MOTOR[8];
    grpAuto456->Visible=(LOAD_Z_USE_MOTOR[6] ||
                         LOAD_Z_USE_MOTOR[7] ||
                         LOAD_Z_USE_MOTOR[8]);
    btnAuto4  ->Visible =(AUTO_EMPTY_COLOR>=3);
    GoBtnAuto4->Visible =(AUTO_EMPTY_COLOR>=3);
    setEdtAuto4->Visible=(AUTO_EMPTY_COLOR>=3);
    btnAuto5  ->Visible =(AUTO_EMPTY_COLOR>=3);
    GoBtnAuto5->Visible =(AUTO_EMPTY_COLOR>=3);
    setEdtAuto5->Visible=(AUTO_EMPTY_COLOR>=3);
    btnAuto6  ->Visible =(AUTO_EMPTY_COLOR>=4);
    GoBtnAuto6->Visible =(AUTO_EMPTY_COLOR>=4);
    setEdtAuto6->Visible=(AUTO_EMPTY_COLOR>=4);
    //<==
    //Steven 20190813 : 入Tray改用步進馬達

    SetEditAutoClean->Text =Teach.iAutoCleanPick;                               //kevin 20190305 add

    //Isaac 20201110 : Index Y find motor phase
    //=>
    if(IniConfig.bD63CheckIndexZHomeToZPhaseDistanceRange==true &&
       (bY1ModifyDistanceRef==true || bY2ModifyDistanceRef==true))
    {
        PageControl2->ActivePage=TabSheet10;                                    //強制切換到index teaching分頁
        lblFindPhaseNotes->Caption="Index Y馬達尋相和原始值差異過大，請重新確認Index Y Teaching 點位";
        lblFindPhaseNotes->Width=697;
    }
    else
    {
        lblFindPhaseNotes->Caption=" ";
    }
    //<=
    //Isaac 20201110 : Index Y find motor phase

    //JimmyChiu 20220708 : add Index Arm Axis
    //<==
    lblArm1->Caption                    =(USE_INDEX_ARM_AXES==IndexArm_3_Axis)?"Arm Y":"Arm 1";
    setEditIndex2ToSocketY->Visible     =(USE_INDEX_ARM_AXES==IndexArm_3_Axis)?false:true;
    setEditIndex2ToSht2Y->Visible       =(USE_INDEX_ARM_AXES==IndexArm_3_Axis)?false:true;
    lblArm2->Visible                    =(USE_INDEX_ARM_AXES==IndexArm_3_Axis)?false:true;
    //Min X
    SetButtonInX140->Caption            =GetMinXPitchCaptionName();
    SetButtonInX240->Caption            =GetMinXPitchCaptionName();
    SetButtonInX340->Caption            =GetMinXPitchCaptionName();
    SetButtonInX440->Caption            =GetMinXPitchCaptionName();
    SetButtonOutX140->Caption           =GetMinXPitchCaptionName();
    SetButtonOutX240->Caption           =GetMinXPitchCaptionName();
    SetButtonOutX340->Caption           =GetMinXPitchCaptionName();
    SetButtonOutX440->Caption           =GetMinXPitchCaptionName();
    gbInX140mm->Caption                 =GetMinXPitchCaptionName("In XP 1");
    gbInX240mm->Caption                 =GetMinXPitchCaptionName("In XP 2");
    gbInX340mm->Caption                 =GetMinXPitchCaptionName("In XP 3");    //RogerYang 20260205 : fix
    gbInX440mm->Caption                 =GetMinXPitchCaptionName("In XP 4");    //RogerYang 20260205 : fix
    gbOutX140mm->Caption                =GetMinXPitchCaptionName("Out XP 1");
    gbOutX240mm->Caption                =GetMinXPitchCaptionName("Out XP 2");
    gbOutX340mm->Caption                =GetMinXPitchCaptionName("Out XP 3");
    gbOutX440mm->Caption                =GetMinXPitchCaptionName("Out XP 4");
    tsXPitch40->Caption                 =GetMinXPitchCaptionName("X Pitch");
    //Max X
    SetButtonInX1120->Caption          =GetMaxXPitchCaptionName();
    SetButtonInX2120->Caption          =GetMaxXPitchCaptionName();
    SetButtonInX3120->Caption          =GetMaxXPitchCaptionName();
    SetButtonInX4120->Caption          =GetMaxXPitchCaptionName();
    SetButtonOutX1120->Caption         =GetMaxXPitchCaptionName();
    SetButtonOutX2120->Caption         =GetMaxXPitchCaptionName();
    SetButtonOutX3120->Caption         =GetMaxXPitchCaptionName();
    SetButtonOutX4120->Caption         =GetMaxXPitchCaptionName();
    gbInX1120->Caption                 =GetMaxXPitchCaptionName("In XP 1");
    gbInX2120->Caption                 =GetMaxXPitchCaptionName("In XP 2");
    gbInX3120->Caption                 =GetMaxXPitchCaptionName("In XP 3");
    gbInX4120->Caption                 =GetMaxXPitchCaptionName("In XP 4");
    gbOutX1120->Caption                =GetMaxXPitchCaptionName("Out XP 1");
    gbOutX2120->Caption                =GetMaxXPitchCaptionName("Out XP 2");
    gbOutX3120->Caption                =GetMaxXPitchCaptionName("Out XP 3");
    gbOutX4120->Caption                =GetMaxXPitchCaptionName("Out XP 4");
    tsXPitch120->Caption               =GetMaxXPitchCaptionName("X Pitch");
    //Min Y
    SetButtonInY15->Caption             =GetMinYPitchCaptionName();
    SetButtonOutY15->Caption            =GetMinYPitchCaptionName();
    gbInY15->Caption                    =GetMinYPitchCaptionName("In YP");
    gbOutY15->Caption                   =GetMinYPitchCaptionName("Out YP");
    tsYPitch15->Caption                 =GetMinYPitchCaptionName("Y Pitch");
    //Max X
    SetButtonInY60->Caption             =GetMaxYPitchCaptionName();
    SetButtonOutY60->Caption            =GetMaxYPitchCaptionName();
    gbInY60->Caption                    =GetMaxYPitchCaptionName("In YP");
    gbOutY60->Caption                   =GetMaxYPitchCaptionName("Out YP");
    tsYPitch60->Caption                 =GetMaxYPitchCaptionName("Y Pitch");
    //<==
    //JimmyChiu 20220708 : add Index Arm Axis

    tsMagazine->TabVisible              =(AUTO3_IS_MAGAZINE==1);                //JerryYang 20220909 : add magazine

    pgc_UsePitchX->Visible              =(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be);//Ztex 2023.12.27 For HT-1032 AT
    TriTemp_Teach();                                                            //Ztex 2024.08.18 Add TriTemp_Teach Function

    if(In_Shuttle_Auto_Latch==eInSHAutoLtc)                                     //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料
    {
        grpInShLtcZ->Visible=true;
        grpInShLtcZPos->Visible=true;
        grpInShLtcPos->Visible=true;
    }

    if(INSTALL_OCR_YMot==eocrYMotInstal)                                        //RogerYang 20260223 : 整合Boat Carrier & Load Y
    {
        grpLoadY->Visible=true;
        MotorLoaderYCCW->Visible=false;
        MotorAuto1YCW->Visible=false;
        MotorAuto1YCCW->Visible=false;
        MotorAuto2YCW->Visible=false;
        MotorAuto2YCCW->Visible=false;
    }
    else if(USE_LdUldCassetteMode)                                              //Ifor 20251216 add:Boat Carrier
    {
        grpCassetteZ->Visible=true;
        grpLoadY->Caption="Cassette Y";
        grpLoadY->Visible=true;
        btnLoaderRotZ->Visible=false;
    }
    else
    {
        grpLoadY->Visible=false;
    }
    //這一行請保持在最下面!!-----------------
    myLog.Do_Log(Sender, "fTeach", asLogPath);                                  //kevin 20181025   //Steven 20100629
}
//---------------------------------------------------------------------------
AnsiString TfTeach::GetMinXPitchCaptionName(AnsiString Str)
{
    if(Str=="")
        Str.sprintf("%0.0fmm", GetMinXPitchMM());
    else
        Str.sprintf("%s %0.0fmm", Str, GetMinXPitchMM());
    return Str;
}
//---------------------------------------------------------------------------
AnsiString TfTeach::GetMaxXPitchCaptionName(AnsiString Str)
{
    if(Str=="")
        Str.sprintf("%0.0fmm", GetMaxXPitchMM());
    else
        Str.sprintf("%s %0.0fmm", Str, GetMaxXPitchMM());
    return Str;
}
//---------------------------------------------------------------------------
AnsiString TfTeach::GetMinYPitchCaptionName(AnsiString Str)
{
    if(Str=="")
        Str.sprintf("%0.0fmm", GetMinYPitchMM());
    else
        Str.sprintf("%s %0.0fmm", Str, GetMinYPitchMM());
    return Str;
}
//---------------------------------------------------------------------------
AnsiString TfTeach::GetMaxYPitchCaptionName(AnsiString Str)
{
    if(Str=="")
        Str.sprintf("%0.0fmm", GetMaxYPitchMM());
    else
        Str.sprintf("%s %0.0fmm", Str, GetMaxYPitchMM());
    return Str;
}
//---------------------------------------------------------------------------
double TfTeach::GetMinXPitchMM()
{
    if(USE_IN_OUT_ARM_X_PITCH==iXPitch16Pick)                                   //Ztex 2024.02.29 Add HT-1032AT
        return (double)(iXpitchMinX7*0.01);
    else
        return (double)(iXpitchMinX3*0.01);
}
//---------------------------------------------------------------------------
double TfTeach::GetMaxXPitchMM()
{
    if(USE_IN_OUT_ARM_X_PITCH==iXPitch16Pick)                                   //Ztex 2024.02.29 Add HT-1032AT
        return (double)(iXpitchMaxX7*0.01);
    else
        return (double)(iXpitchMaxX3*0.01);
}
//---------------------------------------------------------------------------
double TfTeach::GetMinYPitchMM()
{
    return (double)(IN_OUT_ARM_Y_PITCH_MIN*0.01);
}
//---------------------------------------------------------------------------
double TfTeach::GetMaxYPitchMM()
{
    return 60.0;//(double)(IN_OUT_ARM_Y_PITCH_MAX*0.01);                        //Jimmychiu 20221208 依照已出機機台都是儲存Pitch=60
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::FormClose(TObject *Sender, TCloseAction &Action)
{
    fShow=false;
//    Tech.iShuttle2BusyHalfOffset=atoi(setEditSht1BusyHalf->Text.c_str());
//    Tech.iShuttle1BusyHalfOffset=atoi(setEditSht2BusyHalf->Text.c_str());
//    if(Tech.iShuttle2BusyHalfOffset>300)
//        Tech.iShuttle2BusyHalfOffset=300;
//    if(Tech.iShuttle2BusyHalfOffset<-300)
//        Tech.iShuttle2BusyHalfOffset=-300;
//    if(Tech.iShuttle1BusyHalfOffset>300)
//        Tech.iShuttle1BusyHalfOffset=300;
//    if(Tech.iShuttle1BusyHalfOffset<-300)
//        Tech.iShuttle1BusyHalfOffset=-300;

    bInArmXPitch_40mm=false;                                                    //KenHsieh 20211023 : 新增X Pitch 40mm teach
    bOutArmXPitch_40mm=false;                                                   //KenHsieh 20211023 : 新增X Pitch 40mm teach
    btnStop->Click();
    Set_Pitch_SetGroup();                                                       //Ztex 2024.04.02 Set Pitch SetGroup
    //這一行請保持在最下面!!-----------------
    myLog.Do_Log(Sender, "fTeach", asLogPath);                                  //kevin 20181025   //Steven 20100629
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnSetToClick(TObject *Sender)
{
    btnSetTo->Down=false;
    EditPtr->Text=edtNowPosition->Text;
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnMovePClick(TObject *Sender)
{
    btnMoveP->Down=false;
    int P1,P2;
    fTechAuto=false;
    if(CheckCanMove()==false || ActiveMotorIndex==-1 || IsCanQuickJogMove()==false)
        return;
    P1=atoi(edtNowPosition->Text.c_str());
    P2=atoi(ComboBox1->Text.c_str());

    if(abs(P1)>999999)                                                          //Steven 20100831 : 溢位保護
    {
        ShowMyMessage("Position over limitation!", "移動位置超過限制!!");
        return;
    }

    if(INDEX_MOTION_CARD==0 && (ActiveMotorIndex==MTestY1 || ActiveMotorIndex==MTestZ1 ||
                                ActiveMotorIndex==MTestZ2 || ActiveMotorIndex==MTestY2))  //Steven 20210623 : Index使用Galil
    {
        MOT[ActiveMotorIndex].MovFlag=false;
        MOT[ActiveMotorIndex].Gali_MovePR(P2, MOT[ActiveMotorIndex].Motor->PJogHighSpeed*ScrollBar1->Position/100);
    }
    else
    {
        MOT[ActiveMotorIndex].fCMD=false;
        MOT[ActiveMotorIndex].MotorMove(P1+P2);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnHomeClick(TObject *Sender)                          //home
{
    fTechAuto=false;

    if(Sen[SnMotorPower].IsOff()==true)                                         //jou 2010-08-05 start : 未power on就home會造成程式例外
    {
        ShowMyMessage("Motor power is OFF!!", "電源被關閉!!", "Teach");
        btnHome->Down=false;
        return;
    }

    if(CheckCanMove()==false || IsCanQuickJogMove()==false)                     //Steven 20091004 : Before home X or Y, need check all Z is home
    {
        btnHome->Down=false;
        return;
    }
    if(ActiveMotorIndex==-1 ||
       ActiveMotorIndex==MInArmXScale  || ActiveMotorIndex==MInArmYScale ||     //Steven 20160426 : 磁性尺
       ActiveMotorIndex==MOutArmXScale || ActiveMotorIndex==MOutArmYScale)
    {
        btnHome->Down=false;
        return;
    }

    if(btnHome->Down==true)
    {
        if(IsIndexMotorOutOfPower())
        {
            ShowMyMessage("EMG pressed or out of power!!", "緊停被按下或電源被關閉!!", "Teach");
            btnHome->Down=false;                                                //Steven 20180507 (Jou) : 避免卡住跳不出去
        }
        if(INDEX_MOTION_CARD==0 && (ActiveMotorIndex==MTestY1 || ActiveMotorIndex==MTestZ1 ||
                                    ActiveMotorIndex==MTestZ2 || ActiveMotorIndex==MTestY2))  //Steven 20210623 : Index使用Galil
        {
            MOT[ActiveMotorIndex].ServoOnOff(true);
            MOT[ActiveMotorIndex].iGali_SingalHomeTask=1;
            MOT[MTestY1].MovFlag=false;
            MOT[MTestZ1].MovFlag=false;
            MOT[MTestZ2].MovFlag=false;
            MOT[MTestY1].bScanFlag=false;
            MOT[MTestZ1].bScanFlag=false;
            MOT[MTestZ2].bScanFlag=false;
            MOT[MTestY1].GaliSofDelayCount=0;
            MOT[MTestZ1].GaliSofDelayCount=0;
            MOT[MTestZ2].GaliSofDelayCount=0;
            MOT[MTestY2].MovFlag=false;
            MOT[MTestY2].bScanFlag=false;
            MOT[MTestY2].GaliSofDelayCount=0;
            fHome->InitGali_HomeTask();
        }
        InitProcessSingleMotorTask(ActiveMotorIndex);
        fHome->iHomeStep=1;
    }
    else
    {
        if(INDEX_MOTION_CARD==0 && (ActiveMotorIndex==MTestY1 || ActiveMotorIndex==MTestZ1 ||
                                    ActiveMotorIndex==MTestZ2 || ActiveMotorIndex==MTestY2))  //Steven 20210623 : Index使用Galil
            MOT[ActiveMotorIndex].Gali_Command("ST", __FUNC__);
        else
            StopAllMotor();

        MOT[ActiveMotorIndex].SetSpeed(1);                                      //jou 2012-05-21 Home沒做完就暫停，再按Jog就飛出去撞機
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnSetToOffsetClick(TObject *Sender)
{
    EditPtr->Text=edtSetToOffset->Text;
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnMoveNClick(TObject *Sender)
{
    btnMoveN->Down=false;
    int P1, P2;
    fTechAuto=false;
    if(CheckCanMove()==false || ActiveMotorIndex==-1 || IsCanQuickJogMove()==false)
        return;
    P1=atoi(edtNowPosition->Text.c_str());
    P2=atoi(ComboBox1->Text.c_str());

    if(abs(P1)>999999)                                                          //Steven 20100831 : 溢位保護
    {
        ShowMyMessage("Position over limitation!", "移動位置超過限制!!");
        return;
    }

    if(INDEX_MOTION_CARD==0 && (ActiveMotorIndex==MTestY1 || ActiveMotorIndex==MTestZ1 ||
                                ActiveMotorIndex==MTestZ2 || ActiveMotorIndex==MTestY2))  //Steven 20210623 : Index使用Galil
    {
        MOT[ActiveMotorIndex].MovFlag=false;
        MOT[ActiveMotorIndex].Gali_MovePR(-P2, MOT[ActiveMotorIndex].Motor->PJogHighSpeed*ScrollBar1->Position/100);
    }
    else
    {
        MOT[ActiveMotorIndex].fCMD=false;
        MOT[ActiveMotorIndex].MotorMove(P1-P2);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::UpdateTempTech()
{
    bOutDataChange=false;

    for(int i=0; i<TECH_MAX_ITEM; i++)
        *TechPara[i]->Parameter=atoi(TechPara[i]->SetEdit->Text.c_str());

    for(int i=0; i<TechTwoItem; i++)
        for(int j=0; j<2; j++)
            *TechTwoPara[i]->Parameter[j]=atoi(TechTwoPara[i]->SetEdit[j]->Text.c_str());

    for(int i=0; i<2; i++)
    {
        for(int j=0; j<8; j++)
        {
            *TechSuckPara[0].Parameter[i][j]=atoi(TechSuckPara[0].SetEdit[i][j]->Text.c_str());
            *TechSuckPara[1].Parameter[i][j]=atoi(TechSuckPara[1].SetEdit[i][j]->Text.c_str());
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnSaveClick(TObject *Sender)
{
    int ret;
    AnsiString strNote="";

    if(fAllMotorHome==false)
    {
        ret=MessageDlg("Sure to Save? (確定要存檔?)",mtConfirmation, TMsgDlgButtons()<<mbYes<<mbNo, 0);
        if(ret==mrNo)
        return;
    }
    UpdateTempTech();

    if((CUSTOMER_CODE==CC_ASE_KaohSiung ||
        CUSTOMER_CODE==CC_ASE_KaohSiung_K12) &&
        Tech.iTestZDown<-1000)                                                  //kevin 20190930 add 保護
        Tech.iTestZDown=-1000;

    WriteIniDataGeneral("Shuttle", "CHECK_RANGE", atoi(edShtCheckRange->Text.c_str()));   //Steven 20160108 : 改去Teaching調整
    SaveFile(false);                                                            //Steven 20240501 : Teach改存成ini
    ReadFile();

    InitShuttleThreadParameter();                                               //Steven 20120921 : Teach做完要重新Init一次
    fAllMotorHome=false;

    //Isaac 20201110 : Index Y find motor phase
    //=>
    if(IniConfig.bD63CheckIndexZHomeToZPhaseDistanceRange==true)
    {
        lblFindPhaseNotes->Caption=" ";

        if(bY1ModifyDistanceRef==true && bY2ModifyDistanceRef==true)
        {
            if((iIndexY1FindPhaseFrontPos==0 && iIndexY1FindPhaseMiddlePos==0) ||
               (iIndexY2FindPhaseMiddlePos==0 && iIndexY2FindPhaseRearPos==0))
            {
                ShowMyMessage("Recorded position error!! Press Home button to find IndexY motor phase!", "馬達尋相位置紀錄錯誤.請按全機回home，重新馬達尋相", "btnSetIndexYPhasePositionClick");
                lblFindPhaseNotes->Caption="請離開Teaching頁面，重新整體回home";
            }
            else if(iIndexY1FindPhaseFrontPos !=Tech.iHT9040TestY1_Front &&
                    iIndexY1FindPhaseMiddlePos!=Tech.iHT9040TestY1_Middle &&
                    iIndexY2FindPhaseMiddlePos!=Tech.iHT9040TestY2_Middle &&
                    iIndexY2FindPhaseRearPos  !=Tech.iHT9040TestY2_Rear)
            {
                if(SetIndexYPhasePosition())                                    //Isaac 20201110 : Index Y find motor phase，存入IndexY尋相找的位置
                    lblFindPhaseNotes->Caption="Index Y1 and Y2 Teaching Position are Changed! Save OK.";
                else
                    lblFindPhaseNotes->Caption="Index Y1 and Y2 Teaching Position are Changed! Please save value.";
            }
            else
            {
                ShowMyMessage("YPhase has SKIP. Please check index Y1 and Y2 teaching Position!", "Y1 and Y2 Phase有SKIP.請重新確認Idex Y軸教導點位", "btnSetIndexYPhasePositionClick");
                strNote.sprintf("Old Pos Y1: SHTY:%d,SocketY:%d ; Y2:SocketY:%d,SHTY:%d", iIndexY1FindPhaseFrontPos, iIndexY1FindPhaseMiddlePos, iIndexY2FindPhaseMiddlePos, iIndexY2FindPhaseRearPos);
                lblFindPhaseNotes->Caption="Index Y1 and Y2 Teaching Positions no chaged!"+strNote;
            }
        }
        else if(bY1ModifyDistanceRef==true)
        {
            if(iIndexY1FindPhaseFrontPos==0 && iIndexY1FindPhaseMiddlePos==0)
            {
                ShowMyMessage("Recorded position error!! Press Home button to find IndexY1 motor phase!", "馬達尋相位置紀錄錯誤.請按全機回home，重新馬達尋相", "btnSetIndexYPhasePositionClick");
                lblFindPhaseNotes->Caption="請離開Teaching頁面，重新整體回home";
            }
            else if(iIndexY1FindPhaseFrontPos!=Tech.iHT9040TestY1_Front &&
                    iIndexY1FindPhaseMiddlePos!=Tech.iHT9040TestY1_Middle)
            {
                if(SetIndexYPhasePosition())                                    //Isaac 20201110 : Index Y find motor phase，存入IndexY尋相找的位置
                    lblFindPhaseNotes->Caption="Index Y1 Teaching Position are Changed! Save OK.";
                else
                    lblFindPhaseNotes->Caption="Index Y1 Teaching Position are Changed! Please save value.";
            }
            else
            {
                ShowMyMessage("YPhase has SKIP. Please check index Y1 teaching Position!", "Y1 Phase有SKIP.請重新確認Idex Y1教導點位", "btnSetIndexYPhasePositionClick");
                strNote.sprintf("Old Pos Y1: SHTY:%d,SocketY:%d", iIndexY1FindPhaseFrontPos, iIndexY1FindPhaseMiddlePos);
                lblFindPhaseNotes->Caption="Index Y1 Teaching Position no chaged!"+strNote;
            }
        }
        else if(bY2ModifyDistanceRef==true)
        {
            if(iIndexY2FindPhaseMiddlePos==0 && iIndexY2FindPhaseRearPos==0)
            {
                ShowMyMessage("Recorded position error!! Press Home button to find IndexY2 motor phase!", "馬達尋相位置紀錄錯誤.請按全機回home，重新馬達尋相", "btnSetIndexYPhasePositionClick");
                lblFindPhaseNotes->Caption="請離開Teaching頁面，重新整體回home";
            }
            else if(iIndexY2FindPhaseMiddlePos!=Tech.iHT9040TestY2_Middle &&
                    iIndexY2FindPhaseRearPos!=Tech.iHT9040TestY2_Rear)
            {
                if(SetIndexYPhasePosition())                                    //Isaac 20201110 : Index Y find motor phase，存入IndexY尋相找的位置
                    lblFindPhaseNotes->Caption="Index Y2 Teaching Position are Changed! Save OK.";
                else
                    lblFindPhaseNotes->Caption="Index Y2 Teaching Position are Changed! Please save value.";
            }
            else
            {
                ShowMyMessage("YPhase has SKIP. Please check index Y2 teaching Position!", "Y2 Phase有SKIP.請重新確認Idex Y2教導點位", "btnSetIndexYPhasePositionClick");
                strNote.sprintf("Old Pos Y2:SocketY:%d,SHTY:%d", iIndexY2FindPhaseMiddlePos, iIndexY2FindPhaseRearPos);
                lblFindPhaseNotes->Caption="Index Y2 Teaching Positions no chaged!"+strNote;
            }
        }
    }
    //=>
    //Isaac 20201110 : Index Y find motor phase
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnMoveToClick(TObject *Sender)
{
    btnMoveTo->Down=false;
    if(CheckCanMove()==false || ActiveMotorIndex==-1 || IsCanQuickJogMove()==false)
        return;
    if(INDEX_MOTION_CARD==0 && (ActiveMotorIndex==MTestY1 || ActiveMotorIndex==MTestZ1 ||
                                ActiveMotorIndex==MTestZ2 || ActiveMotorIndex==MTestY2))  //Steven 20210623 : Index使用Galil
    {
        MOT[ActiveMotorIndex].MovFlag=false;
        MOT[ActiveMotorIndex].Gali_MotMove(atoi(edtMoveTo->Text.c_str()), MOT[ActiveMotorIndex].Motor->PHomeHighSpeed);     //2012-01-15    Dell Debug readspeed讀不到初值,改帶joghighspeed
    }
    else
    {
        MOT[ActiveMotorIndex].fCMD=false;
        MOT[ActiveMotorIndex].MotorMove(atoi(edtMoveTo->Text.c_str()));
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::BtnPanel1Click(TObject *Sender)
{
    TBtnPanelLane *Ptr;
    Ptr=(TBtnPanelLane *) Sender;
    bOutDataChange=true;
    Ptr->Down=!Ptr->Down;
    if((Ptr->Down && Ptr->OutType==1) || (Ptr->Down==false && Ptr->OutType==0))
        MyLaneIO.IOBitOn(Ptr->OutIP, Ptr->OutIP, Ptr->OutPort, Ptr->OutBit, Ptr->ISABase, Ptr->Alias);  //Sam 20230724 : 新增 PCI1203 IO 模組
    else
        MyLaneIO.IOBitOff(Ptr->OutIP, Ptr->OutIP, Ptr->OutPort, Ptr->OutBit, Ptr->ISABase, Ptr->Alias); //Sam 20230724 : 新增 PCI1203 IO 模組
     bOutDataChange=true;
     if(Ptr->Down)
     {
          Ptr->Color=Ptr->TrueColor;
          Ptr->Font->Color=Ptr->TrueFontColor;
          Ptr->BevelInner=bvLowered;
          Ptr->BevelOuter=bvLowered;
     }
     else
     {
          Ptr->Color=Ptr->FalseColor;
          Ptr->Font->Color=Ptr->FalseFontColor;
          Ptr->BevelInner=bvRaised;
          Ptr->BevelOuter=bvRaised;
     }
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnMotorTestClick(TObject *Sender)
{
    fMotorTest->ShowModal();
    fAllMotorHome=false;
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::edtSpeedChange(TObject *Sender)
{
    int Speed;
    if(ActiveMotorIndex==-1)
        return;
    Speed=atoi(edtSpeed->Text.c_str());
    if(Speed>ScrollBar1->Max)
        Speed=ScrollBar1->Max;
    if(Speed<ScrollBar1->Min)
        Speed=ScrollBar1->Min;
    ScrollBar1->Position=Speed;
    MOT[ActiveMotorIndex].SetSpeed(Speed);
    edtSpeed->Text=Speed;
}
//---------------------------------------------------------------------------
bool  __fastcall TfTeach::CheckCanMove()
{
    if(IsEMGPressed())
    {
        ShowMyMessage("EMG Stop", "");
        return false;
    }
    else
    {
        return true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::DoAutoTech()
{
    /*int &Task=iTechTask,i,GetGaliHomePos;
    if(fTechAuto==false)
        return ;
    static int    ct=0,IndexMotor,iNeedHomeCount,mtIndex[10];
    static TEdit  *editPtr[10];
    AnsiString SCheckHomeAxis[4]={"InArmZ3","InArmZ4","OutArmZ3","OutArmZ4"};
    int iArmSuck=2;
    iArmSuck=MAX_ARM_SUCK;
    switch(Task)
    {
        case 1:
            Panel4->Caption="Auto calibration";
            fHome->iHomeStep=1;
            ct=0;
            if(iTechIndex==0) // Inarm Pick from loader
            {
                iNeedHomeCount=6;
                mtIndex[0]=MInArmZ1;    editPtr[0]=setEdit022;
                mtIndex[1]=MInArmZB;    editPtr[1]=setEdit023;
                mtIndex[2]=MInArmZ3;    editPtr[2]=EditEmpty;
                mtIndex[3]=MInArmZ4;    editPtr[3]=EditEmpty;
                mtIndex[4]=MInArmX;     editPtr[4]=setEditLoaderX;
                mtIndex[5]=MInArmY;     editPtr[5]=setEditLoaderY;
            }
            else if(iTechIndex==1) // inarm pick and place plate
            {
                iNeedHomeCount=6;
                mtIndex[0]=MInArmZ1;    editPtr[0]=EditEmpty;
                mtIndex[1]=MInArmZB;    editPtr[1]=EditEmpty;
                mtIndex[2]=MInArmZ3;    editPtr[2]=EditEmpty;
                mtIndex[3]=MInArmZ4;    editPtr[3]=EditEmpty;
                mtIndex[4]=MInArmX;     editPtr[4]=setEditHP1X;
                mtIndex[5]=MInArmY;     editPtr[5]=setEditHP1Y;
            }
            else if(iTechIndex==2)
            {
                iNeedHomeCount=6;
                mtIndex[0]=MInArmZ1;    editPtr[0]=setEdit028;
                mtIndex[1]=MInArmZB;    editPtr[1]=setEdit029;
                mtIndex[2]=MInArmZ3;    editPtr[2]=EditEmpty;
                mtIndex[3]=MInArmZ4;    editPtr[3]=EditEmpty;
                mtIndex[4]=MInArmX;     editPtr[4]=setEditHP2X;
                mtIndex[5]=MInArmY;     editPtr[5]=setEditHP2Y;
            }
            else if(iTechIndex==3) // inarm shuttle 1
            {
                iNeedHomeCount=6;
                mtIndex[0]=MInArmZ1;    editPtr[0]=EditEmpty;
                mtIndex[1]=MInArmZB;    editPtr[1]=EditEmpty;
                mtIndex[2]=MInArmZ3;    editPtr[2]=EditEmpty;
                mtIndex[3]=MInArmZ4;    editPtr[3]=EditEmpty;
                mtIndex[4]=MInArmX;     editPtr[4]=setEditInSht1X;
                mtIndex[5]=MInArmY;     editPtr[5]=setEditInSht1Y;
            }
            else if(iTechIndex==4) // inarm shuttle 2
            {
                iNeedHomeCount=6;
                mtIndex[0]=MInArmZ1;    editPtr[0]=EditEmpty;
                mtIndex[1]=MInArmZB;    editPtr[1]=EditEmpty;
                mtIndex[2]=MInArmZ3;    editPtr[2]=EditEmpty;
                mtIndex[3]=MInArmZ4;    editPtr[3]=EditEmpty;
                mtIndex[4]=MInArmX;     editPtr[4]=setEditInSht2X;
                mtIndex[5]=MInArmY;     editPtr[5]=setEditInSht2Y;
            }
            else if(iTechIndex==5) // outarm shuttle 1
            {
                iNeedHomeCount=6;
                mtIndex[0]=MOutArmZA;   editPtr[0]=EditEmpty;
                mtIndex[1]=MOutArmZB;   editPtr[1]=EditEmpty;
                mtIndex[2]=MOutArmZ3;   editPtr[2]=EditEmpty;
                mtIndex[3]=MOutArmZ4;   editPtr[3]=EditEmpty;
                mtIndex[4]=MOutArmX;    editPtr[4]=setEditInSht1X;
                mtIndex[5]=MOutArmY;    editPtr[5]=setEditInSht1Y;
            }
            else if(iTechIndex==6)      // outarm shuttle 2
            {
                iNeedHomeCount=6;
                mtIndex[0]=MOutArmZA;   editPtr[0]=setEdit084;
                mtIndex[1]=MOutArmZB;   editPtr[1]=setEdit085;
                mtIndex[2]=MOutArmZ3;   editPtr[2]=EditEmpty;
                mtIndex[3]=MOutArmZ4;   editPtr[3]=EditEmpty;
                mtIndex[4]=MOutArmX;    editPtr[4]=setEditInSht2X;
                mtIndex[5]=MOutArmY;    editPtr[5]=setEditInSht2Y;
            }
            else if(iTechIndex==7)      // outarm AUTO 1
            {
                iNeedHomeCount=6;
                mtIndex[0]=MOutArmZA;   editPtr[0]=EditEmpty;
                mtIndex[1]=MOutArmZB;   editPtr[1]=EditEmpty;
                mtIndex[2]=MOutArmZ3;   editPtr[2]=EditEmpty;
                mtIndex[3]=MOutArmZ4;   editPtr[3]=EditEmpty;
                mtIndex[4]=MOutArmX;    editPtr[4]=setEditAuto1X;
                mtIndex[5]=MOutArmY;    editPtr[5]=setEditAuto1Y;
            }
            else if(iTechIndex==8)      // outarm AUTO 2
            {
                iNeedHomeCount=6;
                mtIndex[0]=MOutArmZA;   editPtr[0]=EditEmpty;
                mtIndex[1]=MOutArmZB;   editPtr[1]=EditEmpty;
                mtIndex[2]=MOutArmZ3;   editPtr[2]=EditEmpty;
                mtIndex[3]=MOutArmZ4;   editPtr[3]=EditEmpty;
                mtIndex[4]=MOutArmX;    editPtr[4]=setEditAuto2X;
                mtIndex[5]=MOutArmY;    editPtr[5]=setEditAuto2Y;
            }
            else if(iTechIndex==9)      // outarm AUTO 3
            {
                iNeedHomeCount=6;
                mtIndex[0]=MOutArmZA;   editPtr[0]=setEdit108;
                mtIndex[1]=MOutArmZB;   editPtr[1]=setEdit109;
                mtIndex[2]=MOutArmZ3;   editPtr[2]=EditEmpty;
                mtIndex[3]=MOutArmZ4;   editPtr[3]=EditEmpty;
                mtIndex[4]=MOutArmX;    editPtr[4]=setEditAuto3X;
                mtIndex[5]=MOutArmY;    editPtr[5]=setEditAuto3Y;
            }
            else if(iTechIndex==10)    // outarm FIX 1
            {
                iNeedHomeCount=6;
                mtIndex[0]=MOutArmZA;   editPtr[0]=EditEmpty;
                mtIndex[1]=MOutArmZB;   editPtr[1]=EditEmpty;
                mtIndex[2]=MOutArmZ3;   editPtr[2]=EditEmpty;
                mtIndex[3]=MOutArmZ4;   editPtr[3]=EditEmpty;
                mtIndex[4]=MOutArmX;    editPtr[4]=setEditFix1X;
                mtIndex[5]=MOutArmY;    editPtr[5]=setEditFix1Y;
            }
            else if(iTechIndex==11)     // outarm FIX 2
            {
                iNeedHomeCount=6;
                mtIndex[0]=MOutArmZA;   editPtr[0]=EditEmpty;
                mtIndex[1]=MOutArmZB;   editPtr[1]=EditEmpty;
                mtIndex[2]=MOutArmZ3;   editPtr[2]=EditEmpty;
                mtIndex[3]=MOutArmZ4;   editPtr[3]=EditEmpty;
                mtIndex[4]=MOutArmX;    editPtr[4]=setEditFix2X;
                mtIndex[5]=MOutArmY;    editPtr[5]=setEditFix2Y;
            }
            else if(iTechIndex==12)     // outarm FIX 3
            {
                iNeedHomeCount=6;
                mtIndex[0]=MOutArmZA;   editPtr[0]=EditEmpty;
                mtIndex[1]=MOutArmZB;   editPtr[1]=EditEmpty;
                mtIndex[2]=MOutArmZ3;   editPtr[2]=EditEmpty;
                mtIndex[3]=MOutArmZ4;   editPtr[3]=EditEmpty;
                mtIndex[4]=MOutArmX;    editPtr[4]=setEditFix3X;
                mtIndex[5]=MOutArmY;    editPtr[5]=setEditFix3Y;
            }
            else if(iTechIndex>=13 && iTechIndex<=18) //Tray Arm
            {
                iNeedHomeCount=1;
                if(iTechIndex==13)  editPtr[0]=setEditTrayLoaderX;
                if(iTechIndex==14)  editPtr[0]=setEditTrayEmptyX;
                if(iTechIndex==15)  editPtr[0]=setEditTrayColorX;
                if(iTechIndex==16)  editPtr[0]=setEditTrayAuto1X;
                if(iTechIndex==17)  editPtr[0]=setEditTrayAuto2X;
                if(iTechIndex==18)  editPtr[0]=setEditTrayAuto3X;
                mtIndex[0]=MTrayX;
                for(i=0; i<iNeedHomeCount; i++)
                    editPtr[i]->Color=clRed;
                Task=50;
                break;
            }
            else if(iTechIndex==19 )
            {
                iNeedHomeCount=iArmSuck;
                mtIndex[0]=MInArmZ1;    editPtr[0]=Edit6;
                mtIndex[1]=MInArmZB;    editPtr[1]=Edit7;
                mtIndex[2]=MInArmZ3;    editPtr[2]=Edit9;
                mtIndex[3]=MInArmZ4;    editPtr[3]=Edit10;
                for(i=0; i<iNeedHomeCount; i++)
                    editPtr[i]->Color=clRed;
            }
            else if(iTechIndex==20)
            {
                iNeedHomeCount=iArmSuck;
                mtIndex[0]=MOutArmZA;   editPtr[0]=Edit6;
                mtIndex[1]=MOutArmZB;   editPtr[1]=Edit7;
                mtIndex[2]=MOutArmZ3;   editPtr[2]=Edit9;
                mtIndex[3]=MOutArmZ4;   editPtr[3]=Edit10;
                for(i=0; i<iNeedHomeCount; i++)
                    editPtr[i]->Color=clRed;
            }
            else if(iTechIndex==21)
            {
                iNeedHomeCount=3;
                mtIndex[0]=MTestZ1;     editPtr[0]=setEditInSht2Right;
                mtIndex[1]=MTestY1;     editPtr[1]=setEditInSht1Right;
                mtIndex[2]=MShuttle1;   editPtr[2]=setEditInSht1Left;
                for(i=0; i<iNeedHomeCount; i++)
                    editPtr[i]->Color=clRed;
            }
            else if(iTechIndex==22)
            {
                iNeedHomeCount=3;
                mtIndex[0]=MTestZ1;     editPtr[0]=setEditInSht2Right;
                mtIndex[1]=MTestY1;     editPtr[1]=setEditInSht1Right;
                mtIndex[2]=MShuttle1;   editPtr[2]=setEditOutSht1Left;
                for(i=0; i<iNeedHomeCount; i++)
                    editPtr[i]->Color=clRed;
            }
            else if(iTechIndex==23)
            {
                iNeedHomeCount=3;
                mtIndex[0]=MTestZ2;     editPtr[0]=setEditOutSht2Right;
                mtIndex[1]=MTestY1;     editPtr[1]=setEditOutSht1Right;
                mtIndex[2]=MInShuttle2; editPtr[2]=setEditInSht2Left;
                for(i=0; i<iNeedHomeCount; i++)
                    editPtr[i]->Color=clRed;
            }
            else if(iTechIndex==24)
            {
                iNeedHomeCount=3;
                mtIndex[0]=MTestZ2;     editPtr[0]=setEditOutSht2Right;
                mtIndex[1]=MTestY1;     editPtr[1]=setEditOutSht1Right;
                mtIndex[2]=MInShuttle2; editPtr[2]=setEditOutSht2Left;
                for(i=0; i<iNeedHomeCount; i++)
                    editPtr[i]->Color=clRed;
            }
            else if(iTechIndex==25 ) // out arm inspect
            {
                iNeedHomeCount=6;
                mtIndex[0]=MOutArmZA;   editPtr[0]=Edit6;
                mtIndex[1]=MOutArmZB;   editPtr[1]=setEdit152;
                mtIndex[2]=MOutArmZ3;   editPtr[2]=Edit6;
                mtIndex[3]=MOutArmZ4;   editPtr[3]=Edit6;
                mtIndex[4]=MOutArmX;    editPtr[4]=setEdit150;
                mtIndex[5]=MOutArmY;    editPtr[5]=setEdit151;
                for(i=0; i<iNeedHomeCount; i++)
                    editPtr[i]->Color=clRed;
            }
            else if(iTechIndex==26 )   // in arm 50 mm pitch
            {
                iNeedHomeCount=5;
                mtIndex[0]=MInArmZ1;    editPtr[0]=Edit6;
                mtIndex[1]=MInArmZB;    editPtr[1]=Edit6;
                mtIndex[2]=MInArmZ3;    editPtr[2]=Edit6;
                mtIndex[3]=MInArmZ4;    editPtr[3]=Edit6;
                mtIndex[4]=MInArmPitch; editPtr[4]=setEditInXPitch40;
                for(i=0; i<iNeedHomeCount; i++)
                    editPtr[i]->Color=clRed;
            }
            else if(iTechIndex==27 )   // in arm 80 mm pitch
            {
                iNeedHomeCount=5;
                mtIndex[0]=MInArmZ1;    editPtr[0]=Edit6;
                mtIndex[1]=MInArmZB;    editPtr[1]=Edit6;
                mtIndex[2]=MInArmZ3;    editPtr[2]=Edit6;
                mtIndex[3]=MInArmZ4;    editPtr[3]=Edit6;
                mtIndex[4]=MInArmPitch; editPtr[4]=setEditInXPitch120;
                for(i=0; i<iNeedHomeCount; i++)
                    editPtr[i]->Color=clRed;
            }
            else if(iTechIndex==28 ) // outarm 50 mm pitch
            {
                iNeedHomeCount=5;
                mtIndex[0]=MOutArmZA;       editPtr[0]=Edit6;
                mtIndex[1]=MOutArmZB;       editPtr[1]=Edit6;
                mtIndex[2]=MOutArmZ3;       editPtr[2]=Edit6;
                mtIndex[3]=MOutArmZ4;       editPtr[3]=Edit6;
                mtIndex[4]=MOutArmPitch;    editPtr[4]=setEditOutXPitch40;
                for(i=0; i<iNeedHomeCount; i++)
                    editPtr[i]->Color=clRed;
            }
            else if(iTechIndex==29 ) // outarm 80 mm pitch
            {
                iNeedHomeCount=5;
                mtIndex[0]=MOutArmZA;       editPtr[0]=Edit6;
                mtIndex[1]=MOutArmZB;       editPtr[1]=Edit6;
                mtIndex[2]=MOutArmZ3;       editPtr[2]=Edit6;
                mtIndex[3]=MOutArmZ4;       editPtr[3]=Edit6;
                mtIndex[4]=MOutArmPitch;    editPtr[4]=setEditOutXPitch120;
                for(i=0; i<iNeedHomeCount; i++)
                    editPtr[i]->Color=clRed;
            }
            else if(iTechIndex==30)
            {
                iNeedHomeCount=6;
                mtIndex[0]=MInArmZ1;    editPtr[0]=Edit6;
                mtIndex[1]=MInArmZB;    editPtr[1]=Edit7;
                mtIndex[2]=MInArmZ3;    editPtr[2]=Edit6;
                mtIndex[3]=MInArmZ4;    editPtr[3]=Edit6;
                mtIndex[4]=MInArmX;     editPtr[4]=Edit15;
                mtIndex[5]=MInArmY;     editPtr[5]=Edit16;
            }
            else if(iTechIndex==31)
            {
                iNeedHomeCount=5;
                mtIndex[0]=MInArmZ1;    editPtr[0]=Edit6;
                mtIndex[1]=MInArmZB;    editPtr[1]=Edit6;
                mtIndex[2]=MInArmZ3;    editPtr[2]=Edit6;
                mtIndex[3]=MInArmZ4;    editPtr[3]=Edit6;
                mtIndex[4]=MInArmPitch2; editPtr[4]=setEditAutoCleanX;
                for(i=0; i<iNeedHomeCount; i++)
                    editPtr[i]->Color=clRed;
            }
            else if(iTechIndex==32 )
            {
                iNeedHomeCount=5;
                mtIndex[0]=MInArmZ1;    editPtr[0]=Edit6;
                mtIndex[1]=MInArmZB;    editPtr[1]=Edit6;
                mtIndex[2]=MInArmZ3;    editPtr[2]=Edit6;
                mtIndex[3]=MInArmZ4;    editPtr[3]=Edit6;
                mtIndex[4]=MInArmPitch2; editPtr[4]=setEditAutoCleanY;
                for(i=0; i<iNeedHomeCount; i++)
                    editPtr[i]->Color=clRed;
            }
            else if(iTechIndex==33 )
            {
                iNeedHomeCount=5;
                mtIndex[0]=MOutArmZA;    editPtr[0]=Edit6;
                mtIndex[1]=MOutArmZB;    editPtr[1]=Edit6;
                mtIndex[2]=MOutArmZ3;    editPtr[2]=Edit6;
                mtIndex[3]=MOutArmZ4;    editPtr[3]=Edit6;
                mtIndex[4]=MOutArmPitch2; editPtr[4]=setEdit088;
                for(i=0; i<iNeedHomeCount; i++)
                    editPtr[i]->Color=clRed;
            }
            else if(iTechIndex==34 )
            {
                iNeedHomeCount=5;
                mtIndex[0]=MOutArmZA;    editPtr[0]=Edit6;
                mtIndex[1]=MOutArmZB;    editPtr[1]=Edit6;
                mtIndex[2]=MOutArmZ3;    editPtr[2]=Edit6;
                mtIndex[3]=MOutArmZ4;    editPtr[3]=Edit6;
                mtIndex[4]=MOutArmPitch2; editPtr[4]=setEdit089;
                for(i=0; i<iNeedHomeCount; i++)
                    editPtr[i]->Color=clRed;
            }
            for(i=0; i<iNeedHomeCount; i++)
                editPtr[i]->Color=clRed;
            Task=100;
            break;
        case 50:
            if(Cylinder[C_TrayX_UpDown].Pop())
                Task=100;
            break;
        case 100:
            if(iArmSuck==4)
            {
                MOT[MInArmZ3].ScanMotorStatus();
                MOT[MInArmZ4].ScanMotorStatus();
                MOT[MOutArmZ3].ScanMotorStatus();
                MOT[MOutArmZ4].ScanMotorStatus();
            }
            IndexMotor=mtIndex[ct];
            if(INDEX_MOTION_CARD==0 && (ActiveMotorIndex==MTestY1 || ActiveMotorIndex==MTestZ1 || ActiveMotorIndex==MTestZ2 || ActiveMotorIndex==MTestY2))  //Steven 20210623 : Index使用Galil
                fHome->InitDoTestZHome();
            else
                InitProcessSingleMotorTask(IndexMotor);
            Panel4->Caption=MOT[ IndexMotor ].NumberAlias+" Homing.........";
            Task=200;
            break;
        case 200:
            if(INDEX_MOTION_CARD==0 && (ActiveMotorIndex==MTestY1 || ActiveMotorIndex==MTestZ1 || ActiveMotorIndex==MTestZ2 || ActiveMotorIndex==MTestY2))  //Steven 20210623 : Index使用Galil
            {
                GetGaliHomePos=fHome->GetHomePitch(IndexMotor);
                if(GetGaliHomePos!=0)
                {
                    editPtr[ct]->Text=GetGaliHomePos;
                    editPtr[ct]->Color=clAqua;
                    Task=300;
                }
            }
            else
            {
                if(ProcessSingleMotorHome(IndexMotor))
                {
                    editPtr[ct]->Text=-(int)(MOT[IndexMotor].Motor->LastHomePos*MOT[IndexMotor].Motor->GearRatio);
                    if(iTechIndex==26 || iTechIndex==27)
                    {
                        if(IndexMotor==MInArmX)
                            Tech.TechInArmPitchX=atoi(editPtr[ct]->Text.c_str());
                        if(IndexMotor==MInArmY)
                            Tech.TechInArmPitchY=atoi(editPtr[ct]->Text.c_str());
                    }
                    if(iTechIndex==28 || iTechIndex==29)
                    {
                        if(IndexMotor==MOutArmX)
                            Tech.TechOutArmPitchX=atoi(editPtr[ct]->Text.c_str());
                        if(IndexMotor==MOutArmY)
                            Tech.TechOutArmPitchY=atoi(editPtr[ct]->Text.c_str());
                    }
                    MOT[IndexMotor].HomeFlag=1;
                    editPtr[ct]->Color=clAqua;
                    Task=300;
                }
            }
            break;
        case 300:
            ct++;
            if(ct>=iNeedHomeCount)
            {
                Panel4->Caption="Finish";
                for(i=0; i<iNeedHomeCount; i++)
                    editPtr[i]->Color=clWindow;
                if(iTechIndex==19 || iTechIndex==20 || iTechIndex==30)
                {
                    Task=400;
                    break;
                }
                fTechAuto=false;
                Task=1;
                return ;
            }
            else
            {
                Task=100;
            }
            break;
        case 400:
            if(iTechIndex==19)
            {
                EditSh1Speed->Text  =atoi( editPtr[0]->Text.c_str())-atoi(editPtr[1]->Text.c_str());
                if(iArmSuck==4)
                {
                    Edit11->Text =atoi( editPtr[2]->Text.c_str())-atoi(editPtr[1]->Text.c_str());
                    Edit12->Text =atoi( editPtr[3]->Text.c_str())-atoi(editPtr[1]->Text.c_str());
                }
                else
                {
                    Edit11->Text =0;
                    Edit12->Text =0;
                }
            }
            else if(iTechIndex==20)
            {
                EditSh2Speed->Text  =atoi( editPtr[0]->Text.c_str())-atoi(editPtr[1]->Text.c_str());
                if(iArmSuck==4)
                {
                    Edit13->Text =atoi( editPtr[2]->Text.c_str())-atoi(editPtr[1]->Text.c_str());
                    Edit14->Text =atoi( editPtr[3]->Text.c_str())-atoi(editPtr[1]->Text.c_str());
                }
                else
                {
                    Edit13->Text =0;
                    Edit14->Text =0;
                }
            }
            else if(iTechIndex==30)
            {
                Edit15->Text=atoi(Edit15->Text.c_str())-atoi(setEditLoaderX->Text.c_str());
                Edit16->Text=atoi(Edit16->Text.c_str())-atoi(setEditLoaderY->Text.c_str());
            }
            fTechAuto=false;
            Task=1;
            return ;
    }  */
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnArm1YServoClick(TObject *Sender)
{
    static bool bflag=false;
    if(bflag==false)
    {
        MOT[MTestY1].Gali_Command("MOX", __FUNC__);
        if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)                                 //JimmyChiu 20220708 : add Index Arm Axis
            MOT[MTestY2].Gali_Command("MOW", __FUNC__);
        btnArm1YServo->Caption="Server OFF";
        btnArm2YServo->Caption="Server OFF";
    }
    else
    {
        MOT[MTestY1].Gali_Command("SHX", __FUNC__);
        if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)                                 //JimmyChiu 20220708 : add Index Arm Axis
            MOT[MTestY2].Gali_Command("SHW", __FUNC__);
        btnArm1YServo->Caption="Server ON";
        btnArm2YServo->Caption="Server ON";
    }
    bflag=!bflag;
    fAllMotorHome=false;
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnIOClick(TObject *Sender)
{
    fiosetview->ShowModal();
    btnIO->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnStopClick(TObject *Sender)
{
    StopAllMotor();
    Tech_Part=0;
    MOT[MTestY1].Gali_Command("ST", __FUNC__);                                  //Steven 20230721 : 修正Teaching畫面的STOP按鈕
    AllBtnUp();                                                                 //Steven 20230815 : Add for MotorTest
}
//---------------------------------------------------------------------------
int TfTeach::GetPitchOffSet(int WhichPitch, int Pos)                            //ChungHung 20140116 沒有用到
{
    double m, r;
    int InArm50, InArm80, OutArm50, OutArm80;
    InArm50 =atoi(setEditInXPitch40->Text.c_str());
    InArm80 =atoi(setEditInXPitch120->Text.c_str());
    OutArm50=atoi(setEditOutXPitch40->Text.c_str());
    OutArm80=atoi(setEditOutXPitch120->Text.c_str());
    if(WhichPitch==MInArmPitch)
    {
        if(InArm50==0 || InArm80==0 || InArm50 == InArm80)
        {
            return (Pos-iInArmPitch);
        }
        else
        {
            m=(InArm80-InArm50)/3000.0;
            r=(double)InArm50+m*((double)Pos-5000.0);
        }
    }
    else
    {
        if(OutArm50==0 || OutArm80==0 || OutArm50==OutArm80)
        {
            return (Pos-iOutArmPitch);
        }
        else
        {
            m=(OutArm80-OutArm50)/3000.0;
            r=(double)OutArm50+m*((double)Pos-5000.0);
        }
    }
    return (int)r;
}
//---------------------------------------------------------------------------
void TfTeach::DoAutoTechPitch()                                                 //ChungHung 20140116 沒用到
{
    int MotData[10], Pos[2], PitchData;
    if(Tech_Part==0)
        return;
    bool MoveFlag[4]={false, false, false, false};
    int &Task=iAuto_TeachPitch;
    if(Tech_Part==1)
    {
        MotData[0]=MInArmX;         MotData[1]=MInArmY;
        MotData[2]=MInArmPitch;     MotData[3]=MInArmZA;
        MotData[4]=MInArmZB;        MotData[5]=MInArmZC;
        MotData[6]=MInArmZD;
        Pos[0]=Tech.TechInArmPitchX;
        Pos[1]=Tech.TechInArmPitchY;
        PitchData=5000;
    }
    else if(Tech_Part==2)
    {
        MotData[0]=MOutArmX;        MotData[1]=MOutArmY;
        MotData[2]=MOutArmPitch;    MotData[3]=MOutArmZA;
        MotData[4]=MOutArmZB;       MotData[5]=MOutArmZC;
        MotData[6]=MOutArmZD;
        Pos[0]=Tech.TechOutArmPitchX;
        Pos[1]=Tech.TechOutArmPitchY;
        PitchData=5000;
    }
    switch(Task)
    {
        case 1:
            for(int i=0; i<2; i++)
                MoveFlag[i]=MOT[MotData[i]].MotorMove(Pos[i]);
            MoveFlag[2]=MOT[MotData[2]].MotorMove(GetPitchOffSet(MotData[2], PitchData));
            for(int i=0; i<3; i++)
            {
                if(MoveFlag[i]==false)
                    return;
            }
            Task=100;
            break;
        case 100:
            for(int i=3; i<7; i++)
                MoveFlag[i-3]=MOT[MotData[i]].MotorMove(-2000);
            for(int i=0; i<4; i++)
            {
                if(MoveFlag[i]==false)
                    return;
                Tech_Part=0;
            }
            break;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfTeach::pnlEncoderPosDblClick(TObject *Sender)
{
    EditPtr->Text=pnlEncoderPos->Caption;
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::spOutArmPitchClick(TObject *Sender)                    //ChungHung 20140116 沒用到
{
    ActiveMotorIndex=MOutArmPitch;
    MOT[ActiveMotorIndex].iGali_SingalHomeTask=1;
    for(int i=0; i<TECH_MAX_ITEM; i++)
    {
        if(TechPara[i]->funButton!=NULL)
            TechPara[i]->funButton->Down=false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::Button39Click(TObject *Sender)                         //ChungHung 20140116 沒用到
{
    iPitchMotor=MOutArmPitch;
    iGetHomeSensor2PositionTask=1;
    bAutoGetHomeSensor2Position=true;
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::Button40Click(TObject *Sender)                         //ChungHung 20140116 沒用到
{
    iPitchMotor=MInArmPitch;
    iGetHomeSensor2PositionTask=1;
    bAutoGetHomeSensor2Position=true;
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::BtnPanelLane1Click(TObject *Sender)
{
    TBtnPanelLane *Ptr;
    Ptr=(TBtnPanelLane *) Sender;
    bOutDataChange=true;
    Ptr->Down=!Ptr->Down;
    if((Ptr->Down && Ptr->OutType==1) ||
       (Ptr->Down==false  && Ptr->OutType==0))
        MyLaneIO.IOBitOn(Ptr->OutRing, Ptr->OutIP, Ptr->OutPort, Ptr->OutBit, Ptr->ISABase, Ptr->Alias);    //Sam 20230724 : 新增 PCI1203 IO 模組
    else
        MyLaneIO.IOBitOff(Ptr->OutRing, Ptr->OutIP, Ptr->OutPort, Ptr->OutBit, Ptr->ISABase, Ptr->Alias);   //Sam 20230724 : 新增 PCI1203 IO 模組
    if(Ptr->Down)
    {
        Ptr->Color=Ptr->TrueColor;
        Ptr->Font->Color=Ptr->TrueFontColor;
        Ptr->BevelInner=bvLowered;
        Ptr->BevelOuter=bvLowered;
    }
    else
    {
        Ptr->Color=Ptr->FalseColor;
        Ptr->Font->Color=Ptr->FalseFontColor;
        Ptr->BevelInner=bvRaised;
        Ptr->BevelOuter=bvRaised;
    }
    if(SW[SwFMotorBreaker].Port==Ptr->OutPort &&
       SW[SwFMotorBreaker].Bit==Ptr->OutBit)
    {
        fAllMotorHome=false;
        MOT[MTestZ1].HomeFlag=0;
    }
    if(SW[SwBMotorBreaker].Port==Ptr->OutPort &&
       SW[SwBMotorBreaker].Bit==Ptr->OutBit)
    {
        fAllMotorHome=false;
        MOT[MTestZ2].HomeFlag=0;
    }
    if(SW[SwMotorRelay].Port==Ptr->OutPort &&
       SW[SwMotorRelay].Bit==Ptr->OutBit)
    {
        fAllMotorHome=false;
        for(int i=0; i<TOTAL_MOTOR; i++)
            MOT[i].HomeFlag=0;
    }
    if(SW[SwServerON].Port==Ptr->OutPort &&
       SW[SwServerON].Bit==Ptr->OutBit)
    {
        fAllMotorHome=false;
        for(int i=0; i<TOTAL_MOTOR; i++)
            MOT[i].HomeFlag=0;
    }
}
//---------------------------------------------------------------------------
void TfTeach::InitialTeachEditList()
{
    bool bShow=true;
    bool bEnable=true;//, bDisable=false;
//    bool bNoShow=(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker)?true:false;
    bool bReadFromFile=true;
//    bool bFixedValue=(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||
//                      USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)?true:false;      //Ztex 2023.12.06 Add HT-1032

    bool bFixedValue=false;

    elTeach->Add(SetEditAutoClean,              &Teach.iAutoCleanPick           , ECInteger, "InArm",        "AutoCleanPick",           bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(setEdLoadCellY1,               &Teach.iLoadCellY1              , ECInteger, "Index",        "iLoadCellY1",             bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(setEdLoadCellY2,               &Teach.iLoadCellY2              , ECInteger, "Index",        "iLoadCellY2",             bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(setEdLoadCellZ1,               &Teach.iLoadCellZ1Down          , ECInteger, "Index",        "iLoadCellZ1Down",         bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(setEdLoadCellZ2,               &Teach.iLoadCellZ2Down          , ECInteger, "Index",        "iLoadCellZ2Down",         bShow, bEnable, bReadFromFile,   0, false);

    //舊資料
    elTeach->Add(EdtTemp,     &Tech._iInArmCCD_Pitch_PosX[0]   , ECInteger, "ArmAlignment", "InArmAlignmentPitch1",    bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iInArmCCD_Pitch_PosX[1]   , ECInteger, "ArmAlignment", "InArmAlignmentPitch2",    bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iInArmCCD_Pitch_PosX[2]   , ECInteger, "ArmAlignment", "InArmAlignmentPitch3",    bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iInArmCCD_Pitch_PosX[3]   , ECInteger, "ArmAlignment", "InArmAlignmentPitch4",    bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iInArmCCD_Pitch_PosX[4]   , ECInteger, "ArmAlignment", "InArmAlignmentPitch5",    bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iInArmCCD_Pitch_PosX[5]   , ECInteger, "ArmAlignment", "InArmAlignmentPitch6",    bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iInArmCCD_Picker_PosZ[0]  , ECInteger, "ArmAlignment", "InArmAlignmentPick1_Z",   bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iInArmCCD_Picker_PosZ[1]  , ECInteger, "ArmAlignment", "InArmAlignmentPick2_Z",   bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iInArmCCD_Picker_PosZ[2]  , ECInteger, "ArmAlignment", "InArmAlignmentPick3_Z",   bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iInArmCCD_Picker_PosZ[3]  , ECInteger, "ArmAlignment", "InArmAlignmentPick4_Z",   bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iInArmCCD_Picker_PosZ[4]  , ECInteger, "ArmAlignment", "InArmAlignmentPick5_Z",   bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iInArmCCD_Picker_PosZ[5]  , ECInteger, "ArmAlignment", "InArmAlignmentPick6_Z",   bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iInArmCCD_Picker_PosZ[6]  , ECInteger, "ArmAlignment", "InArmAlignmentPick7_Z",   bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iInArmCCD_Picker_PosZ[7]  , ECInteger, "ArmAlignment", "InArmAlignmentPick8_Z",   bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iInArmCCD_Picker_PosX[0]  , ECInteger, "ArmAlignment", "InArmAlignmentPick1_X",   bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iInArmCCD_Picker_PosX[1]  , ECInteger, "ArmAlignment", "InArmAlignmentPick2_X",   bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iInArmCCD_Picker_PosX[2]  , ECInteger, "ArmAlignment", "InArmAlignmentPick3_X",   bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iInArmCCD_Picker_PosX[3]  , ECInteger, "ArmAlignment", "InArmAlignmentPick4_X",   bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iInArmCCD_Picker_PosX[4]  , ECInteger, "ArmAlignment", "InArmAlignmentPick5_X",   bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iInArmCCD_Picker_PosX[5]  , ECInteger, "ArmAlignment", "InArmAlignmentPick6_X",   bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iInArmCCD_Picker_PosX[6]  , ECInteger, "ArmAlignment", "InArmAlignmentPick7_X",   bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iInArmCCD_Picker_PosX[7]  , ECInteger, "ArmAlignment", "InArmAlignmentPick8_X",   bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iInArmCCD_Picker_PosY[0]  , ECInteger, "ArmAlignment", "InArmAlignmentPick1_Y",   bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iInArmCCD_Picker_PosY[1]  , ECInteger, "ArmAlignment", "InArmAlignmentPick2_Y",   bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iInArmCCD_Picker_PosY[2]  , ECInteger, "ArmAlignment", "InArmAlignmentPick3_Y",   bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iInArmCCD_Picker_PosY[3]  , ECInteger, "ArmAlignment", "InArmAlignmentPick4_Y",   bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iInArmCCD_Picker_PosY[4]  , ECInteger, "ArmAlignment", "InArmAlignmentPick5_Y",   bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iInArmCCD_Picker_PosY[5]  , ECInteger, "ArmAlignment", "InArmAlignmentPick6_Y",   bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iInArmCCD_Picker_PosY[6]  , ECInteger, "ArmAlignment", "InArmAlignmentPick7_Y",   bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iInArmCCD_Picker_PosY[7]  , ECInteger, "ArmAlignment", "InArmAlignmentPick8_Y",   bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iOutArmCCD_Pitch_PosX[0]  , ECInteger, "ArmAlignment", "OutArmAlignmentPitch1",   bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iOutArmCCD_Pitch_PosX[1]  , ECInteger, "ArmAlignment", "OutArmAlignmentPitch2",   bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iOutArmCCD_Pitch_PosX[2]  , ECInteger, "ArmAlignment", "OutArmAlignmentPitch3",   bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iOutArmCCD_Pitch_PosX[3]  , ECInteger, "ArmAlignment", "OutArmAlignmentPitch4",   bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iOutArmCCD_Pitch_PosX[4]  , ECInteger, "ArmAlignment", "OutArmAlignmentPitch5",   bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iOutArmCCD_Pitch_PosX[5]  , ECInteger, "ArmAlignment", "OutArmAlignmentPitch6",   bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iOutArmCCD_Picker_PosZ[0] , ECInteger, "ArmAlignment", "OutArmAlignmentPick1_Z",  bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iOutArmCCD_Picker_PosZ[1] , ECInteger, "ArmAlignment", "OutArmAlignmentPick2_Z",  bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iOutArmCCD_Picker_PosZ[2] , ECInteger, "ArmAlignment", "OutArmAlignmentPick3_Z",  bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iOutArmCCD_Picker_PosZ[3] , ECInteger, "ArmAlignment", "OutArmAlignmentPick4_Z",  bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iOutArmCCD_Picker_PosZ[4] , ECInteger, "ArmAlignment", "OutArmAlignmentPick5_Z",  bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iOutArmCCD_Picker_PosZ[5] , ECInteger, "ArmAlignment", "OutArmAlignmentPick6_Z",  bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iOutArmCCD_Picker_PosZ[6] , ECInteger, "ArmAlignment", "OutArmAlignmentPick7_Z",  bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iOutArmCCD_Picker_PosZ[7] , ECInteger, "ArmAlignment", "OutArmAlignmentPick8_Z",  bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iOutArmCCD_Picker_PosX[0] , ECInteger, "ArmAlignment", "OutArmAlignmentPick1_X",  bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iOutArmCCD_Picker_PosX[1] , ECInteger, "ArmAlignment", "OutArmAlignmentPick2_X",  bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iOutArmCCD_Picker_PosX[2] , ECInteger, "ArmAlignment", "OutArmAlignmentPick3_X",  bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iOutArmCCD_Picker_PosX[3] , ECInteger, "ArmAlignment", "OutArmAlignmentPick4_X",  bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iOutArmCCD_Picker_PosX[4] , ECInteger, "ArmAlignment", "OutArmAlignmentPick5_X",  bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iOutArmCCD_Picker_PosX[5] , ECInteger, "ArmAlignment", "OutArmAlignmentPick6_X",  bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iOutArmCCD_Picker_PosX[6] , ECInteger, "ArmAlignment", "OutArmAlignmentPick7_X",  bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iOutArmCCD_Picker_PosX[7] , ECInteger, "ArmAlignment", "OutArmAlignmentPick8_X",  bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iOutArmCCD_Picker_PosY[0] , ECInteger, "ArmAlignment", "OutArmAlignmentPick1_Y",  bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iOutArmCCD_Picker_PosY[1] , ECInteger, "ArmAlignment", "OutArmAlignmentPick2_Y",  bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iOutArmCCD_Picker_PosY[2] , ECInteger, "ArmAlignment", "OutArmAlignmentPick3_Y",  bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iOutArmCCD_Picker_PosY[3] , ECInteger, "ArmAlignment", "OutArmAlignmentPick4_Y",  bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iOutArmCCD_Picker_PosY[4] , ECInteger, "ArmAlignment", "OutArmAlignmentPick5_Y",  bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iOutArmCCD_Picker_PosY[5] , ECInteger, "ArmAlignment", "OutArmAlignmentPick6_Y",  bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iOutArmCCD_Picker_PosY[6] , ECInteger, "ArmAlignment", "OutArmAlignmentPick7_Y",  bShow, bEnable, bReadFromFile,   0, false);
    elTeach->Add(EdtTemp,     &Tech._iOutArmCCD_Picker_PosY[7] , ECInteger, "ArmAlignment", "OutArmAlignmentPick8_Y",  bShow, bEnable, bReadFromFile,   0, false);

    elTeach->Add(edtInArmCCDXResolution,        &Tech.dInArmCCDXResolution      , ECDouble , "ArmAlignment", "InArmCCDXResolution",     bShow, bEnable, bReadFromFile, 0.0, false, 10.0, -10.0);
    elTeach->Add(edtInArmCCDYResolution,        &Tech.dInArmCCDYResolution      , ECDouble , "ArmAlignment", "InArmCCDYResolution",     bShow, bEnable, bReadFromFile, 0.0, false, 10.0, -10.0);
    elTeach->Add(edtInArmCCDXRadian,            &Tech.dInArmCCDXRadian          , ECDouble , "ArmAlignment", "InArmCCDXRadian",         bShow, bEnable, bReadFromFile, 0.0, false, 10.0, -10.0);
    elTeach->Add(edtInArmCCDYRadian,            &Tech.dInArmCCDYRadian          , ECDouble , "ArmAlignment", "InArmCCDYRadian",         bShow, bEnable, bReadFromFile, 0.0, false, 10.0, -10.0);
    elTeach->Add(edtOutArmCCDXResolution,       &Tech.dOutArmCCDXResolution     , ECDouble , "ArmAlignment", "OutArmCCDXResolution",    bShow, bEnable, bReadFromFile, 0.0, false, 10.0, -10.0);
    elTeach->Add(edtOutArmCCDYResolution,       &Tech.dOutArmCCDYResolution     , ECDouble , "ArmAlignment", "OutArmCCDYResolution",    bShow, bEnable, bReadFromFile, 0.0, false, 10.0, -10.0);
    elTeach->Add(edtOutArmCCDXRadian,           &Tech.dOutArmCCDXRadian         , ECDouble , "ArmAlignment", "OutArmCCDXRadian",        bShow, bEnable, bReadFromFile, 0.0, false, 10.0, -10.0);
    elTeach->Add(edtOutArmCCDYRadian,           &Tech.dOutArmCCDYRadian         , ECDouble , "ArmAlignment", "OutArmCCDYRadian",        bShow, bEnable, bReadFromFile, 0.0, false, 10.0, -10.0);

    elTeach->Add(edtInAlignPitchXAa,            &Tech.iInArmCCD_Pitch_PosX[0][0], ECInteger, "ArmAlignment", "InArmPitchXAlignmentAa",    bShow, bEnable, bReadFromFile,   0, false, 4000, 0);
    elTeach->Add(edtInAlignPitchXAb,            &Tech.iInArmCCD_Pitch_PosX[0][1], ECInteger, "ArmAlignment", "InArmPitchXAlignmentAb",    bShow, bEnable, bReadFromFile,   0, false, 4000, 0);
    elTeach->Add(edtInAlignPitchXAc,            &Tech.iInArmCCD_Pitch_PosX[0][2], ECInteger, "ArmAlignment", "InArmPitchXAlignmentAc",    bShow, bEnable, bReadFromFile,   0, false, 4000, 0);
    elTeach->Add(edtInAlignPitchXAd,            &Tech.iInArmCCD_Pitch_PosX[0][3], ECInteger, "ArmAlignment", "InArmPitchXAlignmentAd",    bShow, bEnable, bReadFromFile,   0, false, 4000, 0);
    elTeach->Add(edtInAlignPitchXAe,            &Tech.iInArmCCD_Pitch_PosX[0][4], ECInteger, "ArmAlignment", "InArmPitchXAlignmentAe",    bShow, bEnable, bFixedValue,   0, false, 4000, 0);
    elTeach->Add(edtInAlignPitchXAf,            &Tech.iInArmCCD_Pitch_PosX[0][5], ECInteger, "ArmAlignment", "InArmPitchXAlignmentAf",    bShow, bEnable, bFixedValue,   0, false, 4000, 0);
    elTeach->Add(edtInAlignPitchXAg,            &Tech.iInArmCCD_Pitch_PosX[0][6], ECInteger, "ArmAlignment", "InArmPitchXAlignmentAg",    bShow, bEnable, bFixedValue,   0, false, 4000, 0);
    elTeach->Add(EdtTemp,                       &Tech.iInArmCCD_Pitch_PosX[0][7], ECInteger, "ArmAlignment", "InArmPitchXAlignmentAh",    bShow, bEnable, bFixedValue,   0, false, 4000, 0);
    elTeach->Add(edtInAlignPitchXBa,            &Tech.iInArmCCD_Pitch_PosX[1][0], ECInteger, "ArmAlignment", "InArmPitchXAlignmentBa",    bShow, bEnable, bReadFromFile,   0, false, 4000, 0);
    elTeach->Add(edtInAlignPitchXBb,            &Tech.iInArmCCD_Pitch_PosX[1][1], ECInteger, "ArmAlignment", "InArmPitchXAlignmentBb",    bShow, bEnable, bReadFromFile,   0, false, 4000, 0);
    elTeach->Add(edtInAlignPitchXBc,            &Tech.iInArmCCD_Pitch_PosX[1][2], ECInteger, "ArmAlignment", "InArmPitchXAlignmentBc",    bShow, bEnable, bReadFromFile,   0, false, 4000, 0);
    elTeach->Add(edtInAlignPitchXBd,            &Tech.iInArmCCD_Pitch_PosX[1][3], ECInteger, "ArmAlignment", "InArmPitchXAlignmentBd",    bShow, bEnable, bReadFromFile,   0, false, 4000, 0);
    elTeach->Add(edtInAlignPitchXBe,            &Tech.iInArmCCD_Pitch_PosX[1][4], ECInteger, "ArmAlignment", "InArmPitchXAlignmentBe",    bShow, bEnable, bFixedValue,   0, false, 4000, 0);
    elTeach->Add(edtInAlignPitchXBf,            &Tech.iInArmCCD_Pitch_PosX[1][5], ECInteger, "ArmAlignment", "InArmPitchXAlignmentBf",    bShow, bEnable, bFixedValue,   0, false, 4000, 0);
    elTeach->Add(edtInAlignPitchXBg,            &Tech.iInArmCCD_Pitch_PosX[1][6], ECInteger, "ArmAlignment", "InArmPitchXAlignmentBg",    bShow, bEnable, bFixedValue,   0, false, 4000, 0);
    elTeach->Add(EdtTemp,                       &Tech.iInArmCCD_Pitch_PosX[1][7], ECInteger, "ArmAlignment", "InArmPitchXAlignmentBh",    bShow, bEnable, bFixedValue,   0, false, 4000, 0);

    elTeach->Add(edtInArmAlignPitchY,           &Tech.iInArmCCD_Pitch_PosY      , ECInteger, "ArmAlignment", "InArmAlignmentPitch_Y",    bShow, bEnable, bReadFromFile,   0, false, 7500, 1500);

    elTeach->Add(edtInArmZAlignAa,             &Tech.iInArmCCD_Picker_PosZ[0][0], ECInteger, "ArmAlignment", "InArmZAlignmentAa",   bShow, bEnable, bReadFromFile,   0, false, -500, -3000);
    elTeach->Add(edtInArmZAlignAb,             &Tech.iInArmCCD_Picker_PosZ[0][1], ECInteger, "ArmAlignment", "InArmZAlignmentAb",   bShow, bEnable, bReadFromFile,   0, false, -500, -3000);
    elTeach->Add(edtInArmZAlignAc,             &Tech.iInArmCCD_Picker_PosZ[0][2], ECInteger, "ArmAlignment", "InArmZAlignmentAc",   bShow, bEnable, bReadFromFile,   0, false, -500, -3000);
    elTeach->Add(edtInArmZAlignAd,             &Tech.iInArmCCD_Picker_PosZ[0][3], ECInteger, "ArmAlignment", "InArmZAlignmentAd",   bShow, bEnable, bReadFromFile,   0, false, -500, -3000);
    elTeach->Add(edtInArmZAlignAe,             &Tech.iInArmCCD_Picker_PosZ[0][4], ECInteger, "ArmAlignment", "InArmZAlignmentAe",   bShow, bEnable, bFixedValue,   0, false, -500, -3000);
    elTeach->Add(edtInArmZAlignAf,             &Tech.iInArmCCD_Picker_PosZ[0][5], ECInteger, "ArmAlignment", "InArmZAlignmentAf",   bShow, bEnable, bFixedValue,   0, false, -500, -3000);
    elTeach->Add(edtInArmZAlignAg,             &Tech.iInArmCCD_Picker_PosZ[0][6], ECInteger, "ArmAlignment", "InArmZAlignmentAg",   bShow, bEnable, bFixedValue,   0, false, -500, -3000);
    elTeach->Add(edtInArmZAlignAh,             &Tech.iInArmCCD_Picker_PosZ[0][7], ECInteger, "ArmAlignment", "InArmZAlignmentAh",   bShow, bEnable, bFixedValue,   0, false, -500, -3000);
    elTeach->Add(edtInArmZAlignBa,             &Tech.iInArmCCD_Picker_PosZ[1][0], ECInteger, "ArmAlignment", "InArmZAlignmentBa",   bShow, bEnable, bReadFromFile,   0, false, -500, -3000);
    elTeach->Add(edtInArmZAlignBb,             &Tech.iInArmCCD_Picker_PosZ[1][1], ECInteger, "ArmAlignment", "InArmZAlignmentBb",   bShow, bEnable, bReadFromFile,   0, false, -500, -3000);
    elTeach->Add(edtInArmZAlignBc,             &Tech.iInArmCCD_Picker_PosZ[1][2], ECInteger, "ArmAlignment", "InArmZAlignmentBc",   bShow, bEnable, bReadFromFile,   0, false, -500, -3000);
    elTeach->Add(edtInArmZAlignBd,             &Tech.iInArmCCD_Picker_PosZ[1][3], ECInteger, "ArmAlignment", "InArmZAlignmentBd",   bShow, bEnable, bReadFromFile,   0, false, -500, -3000);
    elTeach->Add(edtInArmZAlignBe,             &Tech.iInArmCCD_Picker_PosZ[1][4], ECInteger, "ArmAlignment", "InArmZAlignmentBe",   bShow, bEnable, bFixedValue,   0, false, -500, -3000);
    elTeach->Add(edtInArmZAlignBf,             &Tech.iInArmCCD_Picker_PosZ[1][5], ECInteger, "ArmAlignment", "InArmZAlignmentBf",   bShow, bEnable, bFixedValue,   0, false, -500, -3000);
    elTeach->Add(edtInArmZAlignBg,             &Tech.iInArmCCD_Picker_PosZ[1][6], ECInteger, "ArmAlignment", "InArmZAlignmentBg",   bShow, bEnable, bFixedValue,   0, false, -500, -3000);
    elTeach->Add(edtInArmZAlignBh,             &Tech.iInArmCCD_Picker_PosZ[1][7], ECInteger, "ArmAlignment", "InArmZAlignmentBh",   bShow, bEnable, bFixedValue,   0, false, -500, -3000);

    elTeach->Add(edtInArmXAlignAa,             &Tech.iInArmCCD_Picker_PosX[0][0], ECInteger, "ArmAlignment", "InArmXAlignmentAa",   bShow, bEnable, bReadFromFile,   0, false, 99999, -99999);
    elTeach->Add(edtInArmXAlignAb,             &Tech.iInArmCCD_Picker_PosX[0][1], ECInteger, "ArmAlignment", "InArmXAlignmentAb",   bShow, bEnable, bReadFromFile,   0, false, 99999, -99999);
    elTeach->Add(edtInArmXAlignAc,             &Tech.iInArmCCD_Picker_PosX[0][2], ECInteger, "ArmAlignment", "InArmXAlignmentAc",   bShow, bEnable, bReadFromFile,   0, false, 99999, -99999);
    elTeach->Add(edtInArmXAlignAd,             &Tech.iInArmCCD_Picker_PosX[0][3], ECInteger, "ArmAlignment", "InArmXAlignmentAd",   bShow, bEnable, bReadFromFile,   0, false, 99999, -99999);
    elTeach->Add(edtInArmXAlignAe,             &Tech.iInArmCCD_Picker_PosX[0][4], ECInteger, "ArmAlignment", "InArmXAlignmentAe",   bShow, bEnable, bFixedValue,     0, false, 99999, -99999);
    elTeach->Add(edtInArmXAlignAf,             &Tech.iInArmCCD_Picker_PosX[0][5], ECInteger, "ArmAlignment", "InArmXAlignmentAf",   bShow, bEnable, bFixedValue,     0, false, 99999, -99999);
    elTeach->Add(edtInArmXAlignAg,             &Tech.iInArmCCD_Picker_PosX[0][6], ECInteger, "ArmAlignment", "InArmXAlignmentAg",   bShow, bEnable, bFixedValue,     0, false, 99999, -99999);
    elTeach->Add(edtInArmXAlignAh,             &Tech.iInArmCCD_Picker_PosX[0][7], ECInteger, "ArmAlignment", "InArmXAlignmentAh",   bShow, bEnable, bFixedValue,     0, false, 99999, -99999);
    elTeach->Add(edtInArmXAlignBa,             &Tech.iInArmCCD_Picker_PosX[1][0], ECInteger, "ArmAlignment", "InArmXAlignmentBa",   bShow, bEnable, bReadFromFile,   0, false, 99999, -99999);
    elTeach->Add(edtInArmXAlignBb,             &Tech.iInArmCCD_Picker_PosX[1][1], ECInteger, "ArmAlignment", "InArmXAlignmentBb",   bShow, bEnable, bReadFromFile,   0, false, 99999, -99999);
    elTeach->Add(edtInArmXAlignBc,             &Tech.iInArmCCD_Picker_PosX[1][2], ECInteger, "ArmAlignment", "InArmXAlignmentBc",   bShow, bEnable, bReadFromFile,   0, false, 99999, -99999);
    elTeach->Add(edtInArmXAlignBd,             &Tech.iInArmCCD_Picker_PosX[1][3], ECInteger, "ArmAlignment", "InArmXAlignmentBd",   bShow, bEnable, bReadFromFile,   0, false, 99999, -99999);
    elTeach->Add(edtInArmXAlignBe,             &Tech.iInArmCCD_Picker_PosX[1][4], ECInteger, "ArmAlignment", "InArmXAlignmentBe",   bShow, bEnable, bFixedValue,     0, false, 99999, -99999);
    elTeach->Add(edtInArmXAlignBf,             &Tech.iInArmCCD_Picker_PosX[1][5], ECInteger, "ArmAlignment", "InArmXAlignmentBf",   bShow, bEnable, bFixedValue,     0, false, 99999, -99999);
    elTeach->Add(edtInArmXAlignBg,             &Tech.iInArmCCD_Picker_PosX[1][6], ECInteger, "ArmAlignment", "InArmXAlignmentBg",   bShow, bEnable, bFixedValue,     0, false, 99999, -99999);
    elTeach->Add(edtInArmXAlignBh,             &Tech.iInArmCCD_Picker_PosX[1][7], ECInteger, "ArmAlignment", "InArmXAlignmentBh",   bShow, bEnable, bFixedValue,     0, false, 99999, -99999);

    elTeach->Add(edtInArmYAlignAa,             &Tech.iInArmCCD_Picker_PosY[0][0], ECInteger, "ArmAlignment", "InArmYAlignmentAa",   bShow, bEnable, bReadFromFile,   0, false, 99999, -99999);
    elTeach->Add(edtInArmYAlignAb,             &Tech.iInArmCCD_Picker_PosY[0][1], ECInteger, "ArmAlignment", "InArmYAlignmentAb",   bShow, bEnable, bReadFromFile,   0, false, 99999, -99999);
    elTeach->Add(edtInArmYAlignAc,             &Tech.iInArmCCD_Picker_PosY[0][2], ECInteger, "ArmAlignment", "InArmYAlignmentAc",   bShow, bEnable, bReadFromFile,   0, false, 99999, -99999);
    elTeach->Add(edtInArmYAlignAd,             &Tech.iInArmCCD_Picker_PosY[0][3], ECInteger, "ArmAlignment", "InArmYAlignmentAd",   bShow, bEnable, bReadFromFile,   0, false, 99999, -99999);
    elTeach->Add(edtInArmYAlignAe,             &Tech.iInArmCCD_Picker_PosY[0][4], ECInteger, "ArmAlignment", "InArmYAlignmentAe",   bShow, bEnable, bFixedValue,     0, false, 99999, -99999);
    elTeach->Add(edtInArmYAlignAf,             &Tech.iInArmCCD_Picker_PosY[0][5], ECInteger, "ArmAlignment", "InArmYAlignmentAf",   bShow, bEnable, bFixedValue,     0, false, 99999, -99999);
    elTeach->Add(edtInArmYAlignAg,             &Tech.iInArmCCD_Picker_PosY[0][6], ECInteger, "ArmAlignment", "InArmYAlignmentAg",   bShow, bEnable, bFixedValue,     0, false, 99999, -99999);
    elTeach->Add(edtInArmYAlignAh,             &Tech.iInArmCCD_Picker_PosY[0][7], ECInteger, "ArmAlignment", "InArmYAlignmentAh",   bShow, bEnable, bFixedValue,     0, false, 99999, -99999);
    elTeach->Add(edtInArmYAlignBa,             &Tech.iInArmCCD_Picker_PosY[1][0], ECInteger, "ArmAlignment", "InArmYAlignmentBa",   bShow, bEnable, bReadFromFile,   0, false, 99999, -99999);
    elTeach->Add(edtInArmYAlignBb,             &Tech.iInArmCCD_Picker_PosY[1][1], ECInteger, "ArmAlignment", "InArmYAlignmentBb",   bShow, bEnable, bReadFromFile,   0, false, 99999, -99999);
    elTeach->Add(edtInArmYAlignBc,             &Tech.iInArmCCD_Picker_PosY[1][2], ECInteger, "ArmAlignment", "InArmYAlignmentBc",   bShow, bEnable, bReadFromFile,   0, false, 99999, -99999);
    elTeach->Add(edtInArmYAlignBd,             &Tech.iInArmCCD_Picker_PosY[1][3], ECInteger, "ArmAlignment", "InArmYAlignmentBd",   bShow, bEnable, bReadFromFile,   0, false, 99999, -99999);
    elTeach->Add(edtInArmYAlignBe,             &Tech.iInArmCCD_Picker_PosY[1][4], ECInteger, "ArmAlignment", "InArmYAlignmentBe",   bShow, bEnable, bFixedValue,     0, false, 99999, -99999);
    elTeach->Add(edtInArmYAlignBf,             &Tech.iInArmCCD_Picker_PosY[1][5], ECInteger, "ArmAlignment", "InArmYAlignmentBf",   bShow, bEnable, bFixedValue,     0, false, 99999, -99999);
    elTeach->Add(edtInArmYAlignBg,             &Tech.iInArmCCD_Picker_PosY[1][6], ECInteger, "ArmAlignment", "InArmYAlignmentBg",   bShow, bEnable, bFixedValue,     0, false, 99999, -99999);
    elTeach->Add(edtInArmYAlignBh,             &Tech.iInArmCCD_Picker_PosY[1][7], ECInteger, "ArmAlignment", "InArmYAlignmentBh",   bShow, bEnable, bFixedValue,     0, false, 99999, -99999);

    elTeach->Add(edtOutAlignPitchXAa,          &Tech.iOutArmCCD_Pitch_PosX[0][0], ECInteger, "ArmAlignment", "OutArmPitchXAlignmentAa",   bShow, bEnable, bReadFromFile,   0, false, 4000, 0);
    elTeach->Add(edtOutAlignPitchXAb,          &Tech.iOutArmCCD_Pitch_PosX[0][1], ECInteger, "ArmAlignment", "OutArmPitchXAlignmentAb",   bShow, bEnable, bReadFromFile,   0, false, 4000, 0);
    elTeach->Add(edtOutAlignPitchXAc,          &Tech.iOutArmCCD_Pitch_PosX[0][2], ECInteger, "ArmAlignment", "OutArmPitchXAlignmentAc",   bShow, bEnable, bReadFromFile,   0, false, 4000, 0);
    elTeach->Add(edtOutAlignPitchXAd,          &Tech.iOutArmCCD_Pitch_PosX[0][3], ECInteger, "ArmAlignment", "OutArmPitchXAlignmentAd",   bShow, bEnable, bReadFromFile,   0, false, 4000, 0);
    elTeach->Add(edtOutAlignPitchXAe,          &Tech.iOutArmCCD_Pitch_PosX[0][4], ECInteger, "ArmAlignment", "OutArmPitchXAlignmentAe",   bShow, bEnable, bFixedValue,   0, false, 4000, 0);
    elTeach->Add(edtOutAlignPitchXAf,          &Tech.iOutArmCCD_Pitch_PosX[0][5], ECInteger, "ArmAlignment", "OutArmPitchXAlignmentAf",   bShow, bEnable, bFixedValue,   0, false, 4000, 0);
    elTeach->Add(edtOutAlignPitchXAg,          &Tech.iOutArmCCD_Pitch_PosX[0][4], ECInteger, "ArmAlignment", "OutArmPitchXAlignmentAg",   bShow, bEnable, bFixedValue,   0, false, 4000, 0);
    elTeach->Add(EdtTemp,                      &Tech.iOutArmCCD_Pitch_PosX[0][5], ECInteger, "ArmAlignment", "OutArmPitchXAlignmentAh",   bShow, bEnable, bFixedValue,   0, false);
    elTeach->Add(edtOutAlignPitchXBa,          &Tech.iOutArmCCD_Pitch_PosX[1][0], ECInteger, "ArmAlignment", "OutArmPitchXAlignmentBa",   bShow, bEnable, bReadFromFile,   0, false, 4000, 0);
    elTeach->Add(edtOutAlignPitchXBb,          &Tech.iOutArmCCD_Pitch_PosX[1][1], ECInteger, "ArmAlignment", "OutArmPitchXAlignmentBb",   bShow, bEnable, bReadFromFile,   0, false, 4000, 0);
    elTeach->Add(edtOutAlignPitchXBc,          &Tech.iOutArmCCD_Pitch_PosX[1][2], ECInteger, "ArmAlignment", "OutArmPitchXAlignmentBc",   bShow, bEnable, bReadFromFile,   0, false, 4000, 0);
    elTeach->Add(edtOutAlignPitchXBd,          &Tech.iOutArmCCD_Pitch_PosX[1][3], ECInteger, "ArmAlignment", "OutArmPitchXAlignmentBd",   bShow, bEnable, bReadFromFile,   0, false, 4000, 0);
    elTeach->Add(edtOutAlignPitchXBe,          &Tech.iOutArmCCD_Pitch_PosX[1][4], ECInteger, "ArmAlignment", "OutArmPitchXAlignmentBe",   bShow, bEnable, bFixedValue,   0, false, 4000, 0);
    elTeach->Add(edtOutAlignPitchXBf,          &Tech.iOutArmCCD_Pitch_PosX[1][5], ECInteger, "ArmAlignment", "OutArmPitchXAlignmentBf",   bShow, bEnable, bFixedValue,   0, false, 4000, 0);
    elTeach->Add(edtOutAlignPitchXBg,          &Tech.iOutArmCCD_Pitch_PosX[1][4], ECInteger, "ArmAlignment", "OutArmPitchXAlignmentBg",   bShow, bEnable, bFixedValue,   0, false, 4000, 0);
    elTeach->Add(EdtTemp,                      &Tech.iOutArmCCD_Pitch_PosX[1][5], ECInteger, "ArmAlignment", "OutArmPitchXAlignmentBh",   bShow, bEnable, bFixedValue,   0, false);

    elTeach->Add(edtOutArmAlignPitchY,         &Tech.iOutArmCCD_Pitch_PosY      , ECInteger, "ArmAlignment", "OutArmAlignmentPitch_Y",   bShow, bEnable, bReadFromFile,   0, false, 7500, 1500);

    elTeach->Add(edtOutArmZAlignAa,           &Tech.iOutArmCCD_Picker_PosZ[0][0], ECInteger, "ArmAlignment", "OutArmZAlignmentAa",  bShow, bEnable, bReadFromFile,   0, false, -500, -3000);
    elTeach->Add(edtOutArmZAlignAb,           &Tech.iOutArmCCD_Picker_PosZ[0][1], ECInteger, "ArmAlignment", "OutArmZAlignmentAb",  bShow, bEnable, bReadFromFile,   0, false, -500, -3000);
    elTeach->Add(edtOutArmZAlignAc,           &Tech.iOutArmCCD_Picker_PosZ[0][2], ECInteger, "ArmAlignment", "OutArmZAlignmentAc",  bShow, bEnable, bReadFromFile,   0, false, -500, -3000);
    elTeach->Add(edtOutArmZAlignAd,           &Tech.iOutArmCCD_Picker_PosZ[0][3], ECInteger, "ArmAlignment", "OutArmZAlignmentAd",  bShow, bEnable, bReadFromFile,   0, false, -500, -3000);
    elTeach->Add(edtOutArmZAlignAe,           &Tech.iOutArmCCD_Picker_PosZ[0][4], ECInteger, "ArmAlignment", "OutArmZAlignmentAe",  bShow, bEnable, bFixedValue,   0, false, -500, -3000);
    elTeach->Add(edtOutArmZAlignAf,           &Tech.iOutArmCCD_Picker_PosZ[0][5], ECInteger, "ArmAlignment", "OutArmZAlignmentAf",  bShow, bEnable, bFixedValue,   0, false, -500, -3000);
    elTeach->Add(edtOutArmZAlignAg,           &Tech.iOutArmCCD_Picker_PosZ[0][6], ECInteger, "ArmAlignment", "OutArmZAlignmentAg",  bShow, bEnable, bFixedValue,   0, false, -500, -3000);
    elTeach->Add(edtOutArmZAlignAh,           &Tech.iOutArmCCD_Picker_PosZ[0][7], ECInteger, "ArmAlignment", "OutArmZAlignmentAh",  bShow, bEnable, bFixedValue,   0, false, -500, -3000);
    elTeach->Add(edtOutArmZAlignBa,           &Tech.iOutArmCCD_Picker_PosZ[1][0], ECInteger, "ArmAlignment", "OutArmZAlignmentBa",  bShow, bEnable, bReadFromFile,   0, false, -500, -3000);
    elTeach->Add(edtOutArmZAlignBb,           &Tech.iOutArmCCD_Picker_PosZ[1][1], ECInteger, "ArmAlignment", "OutArmZAlignmentBb",  bShow, bEnable, bReadFromFile,   0, false, -500, -3000);
    elTeach->Add(edtOutArmZAlignBc,           &Tech.iOutArmCCD_Picker_PosZ[1][2], ECInteger, "ArmAlignment", "OutArmZAlignmentBc",  bShow, bEnable, bReadFromFile,   0, false, -500, -3000);
    elTeach->Add(edtOutArmZAlignBd,           &Tech.iOutArmCCD_Picker_PosZ[1][3], ECInteger, "ArmAlignment", "OutArmZAlignmentBd",  bShow, bEnable, bReadFromFile,   0, false, -500, -3000);
    elTeach->Add(edtOutArmZAlignBe,           &Tech.iOutArmCCD_Picker_PosZ[1][4], ECInteger, "ArmAlignment", "OutArmZAlignmentBe",  bShow, bEnable, bFixedValue,   0, false, -500, -3000);
    elTeach->Add(edtOutArmZAlignBf,           &Tech.iOutArmCCD_Picker_PosZ[1][5], ECInteger, "ArmAlignment", "OutArmZAlignmentBf",  bShow, bEnable, bFixedValue,   0, false, -500, -3000);
    elTeach->Add(edtOutArmZAlignBg,           &Tech.iOutArmCCD_Picker_PosZ[1][6], ECInteger, "ArmAlignment", "OutArmZAlignmentBg",  bShow, bEnable, bFixedValue,   0, false, -500, -3000);
    elTeach->Add(edtOutArmZAlignBh,           &Tech.iOutArmCCD_Picker_PosZ[1][7], ECInteger, "ArmAlignment", "OutArmZAlignmentBh",  bShow, bEnable, bFixedValue,   0, false, -500, -3000);

    elTeach->Add(edtOutArmXAlignAa,           &Tech.iOutArmCCD_Picker_PosX[0][0], ECInteger, "ArmAlignment", "OutArmXAlignmentAa",  bShow, bEnable, bReadFromFile,   0, false, 99999, -99999);
    elTeach->Add(edtOutArmXAlignAb,           &Tech.iOutArmCCD_Picker_PosX[0][1], ECInteger, "ArmAlignment", "OutArmXAlignmentAb",  bShow, bEnable, bReadFromFile,   0, false, 99999, -99999);
    elTeach->Add(edtOutArmXAlignAc,           &Tech.iOutArmCCD_Picker_PosX[0][2], ECInteger, "ArmAlignment", "OutArmXAlignmentAc",  bShow, bEnable, bReadFromFile,   0, false, 99999, -99999);
    elTeach->Add(edtOutArmXAlignAd,           &Tech.iOutArmCCD_Picker_PosX[0][3], ECInteger, "ArmAlignment", "OutArmXAlignmentAd",  bShow, bEnable, bReadFromFile,   0, false, 99999, -99999);
    elTeach->Add(edtOutArmXAlignAe,           &Tech.iOutArmCCD_Picker_PosX[0][4], ECInteger, "ArmAlignment", "OutArmXAlignmentAe",  bShow, bEnable, bFixedValue,     0, false, 99999, -99999);
    elTeach->Add(edtOutArmXAlignAf,           &Tech.iOutArmCCD_Picker_PosX[0][5], ECInteger, "ArmAlignment", "OutArmXAlignmentAf",  bShow, bEnable, bFixedValue,     0, false, 99999, -99999);
    elTeach->Add(edtOutArmXAlignAg,           &Tech.iOutArmCCD_Picker_PosX[0][6], ECInteger, "ArmAlignment", "OutArmXAlignmentAg",  bShow, bEnable, bFixedValue,     0, false, 99999, -99999);
    elTeach->Add(edtOutArmXAlignAh,           &Tech.iOutArmCCD_Picker_PosX[0][7], ECInteger, "ArmAlignment", "OutArmXAlignmentAh",  bShow, bEnable, bFixedValue,     0, false, 99999, -99999);
    elTeach->Add(edtOutArmXAlignBa,           &Tech.iOutArmCCD_Picker_PosX[1][0], ECInteger, "ArmAlignment", "OutArmXAlignmentBa",  bShow, bEnable, bReadFromFile,   0, false, 99999, -99999);
    elTeach->Add(edtOutArmXAlignBb,           &Tech.iOutArmCCD_Picker_PosX[1][1], ECInteger, "ArmAlignment", "OutArmXAlignmentBb",  bShow, bEnable, bReadFromFile,   0, false, 99999, -99999);
    elTeach->Add(edtOutArmXAlignBc,           &Tech.iOutArmCCD_Picker_PosX[1][2], ECInteger, "ArmAlignment", "OutArmXAlignmentBc",  bShow, bEnable, bReadFromFile,   0, false, 99999, -99999);
    elTeach->Add(edtOutArmXAlignBd,           &Tech.iOutArmCCD_Picker_PosX[1][3], ECInteger, "ArmAlignment", "OutArmXAlignmentBd",  bShow, bEnable, bReadFromFile,   0, false, 99999, -99999);
    elTeach->Add(edtOutArmXAlignBe,           &Tech.iOutArmCCD_Picker_PosX[1][4], ECInteger, "ArmAlignment", "OutArmXAlignmentBe",  bShow, bEnable, bFixedValue,     0, false, 99999, -99999);
    elTeach->Add(edtOutArmXAlignBf,           &Tech.iOutArmCCD_Picker_PosX[1][5], ECInteger, "ArmAlignment", "OutArmXAlignmentBf",  bShow, bEnable, bFixedValue,     0, false, 99999, -99999);
    elTeach->Add(edtOutArmXAlignBg,           &Tech.iOutArmCCD_Picker_PosX[1][6], ECInteger, "ArmAlignment", "OutArmXAlignmentBg",  bShow, bEnable, bFixedValue,     0, false, 99999, -99999);
    elTeach->Add(edtOutArmXAlignBh,           &Tech.iOutArmCCD_Picker_PosX[1][7], ECInteger, "ArmAlignment", "OutArmXAlignmentBh",  bShow, bEnable, bFixedValue,     0, false, 99999, -99999);

    elTeach->Add(edtOutArmYAlignAa,           &Tech.iOutArmCCD_Picker_PosY[0][0], ECInteger, "ArmAlignment", "OutArmYAlignmentAa",  bShow, bEnable, bReadFromFile,   0, false, 99999, -99999);
    elTeach->Add(edtOutArmYAlignAb,           &Tech.iOutArmCCD_Picker_PosY[0][1], ECInteger, "ArmAlignment", "OutArmYAlignmentAb",  bShow, bEnable, bReadFromFile,   0, false, 99999, -99999);
    elTeach->Add(edtOutArmYAlignAc,           &Tech.iOutArmCCD_Picker_PosY[0][2], ECInteger, "ArmAlignment", "OutArmYAlignmentAc",  bShow, bEnable, bReadFromFile,   0, false, 99999, -99999);
    elTeach->Add(edtOutArmYAlignAd,           &Tech.iOutArmCCD_Picker_PosY[0][3], ECInteger, "ArmAlignment", "OutArmYAlignmentAd",  bShow, bEnable, bReadFromFile,   0, false, 99999, -99999);
    elTeach->Add(edtOutArmYAlignAe,           &Tech.iOutArmCCD_Picker_PosY[0][4], ECInteger, "ArmAlignment", "OutArmYAlignmentAe",  bShow, bEnable, bFixedValue,     0, false, 99999, -99999);
    elTeach->Add(edtOutArmYAlignAf,           &Tech.iOutArmCCD_Picker_PosY[0][5], ECInteger, "ArmAlignment", "OutArmYAlignmentAf",  bShow, bEnable, bFixedValue,     0, false, 99999, -99999);
    elTeach->Add(edtOutArmYAlignAg,           &Tech.iOutArmCCD_Picker_PosY[0][6], ECInteger, "ArmAlignment", "OutArmYAlignmentAg",  bShow, bEnable, bFixedValue,     0, false, 99999, -99999);
    elTeach->Add(edtOutArmYAlignAh,           &Tech.iOutArmCCD_Picker_PosY[0][7], ECInteger, "ArmAlignment", "OutArmYAlignmentAh",  bShow, bEnable, bFixedValue,     0, false, 99999, -99999);
    elTeach->Add(edtOutArmYAlignBa,           &Tech.iOutArmCCD_Picker_PosY[1][0], ECInteger, "ArmAlignment", "OutArmYAlignmentBa",  bShow, bEnable, bReadFromFile,   0, false, 99999, -99999);
    elTeach->Add(edtOutArmYAlignBb,           &Tech.iOutArmCCD_Picker_PosY[1][1], ECInteger, "ArmAlignment", "OutArmYAlignmentBb",  bShow, bEnable, bReadFromFile,   0, false, 99999, -99999);
    elTeach->Add(edtOutArmYAlignBc,           &Tech.iOutArmCCD_Picker_PosY[1][2], ECInteger, "ArmAlignment", "OutArmYAlignmentBc",  bShow, bEnable, bReadFromFile,   0, false, 99999, -99999);
    elTeach->Add(edtOutArmYAlignBd,           &Tech.iOutArmCCD_Picker_PosY[1][3], ECInteger, "ArmAlignment", "OutArmYAlignmentBd",  bShow, bEnable, bReadFromFile,   0, false, 99999, -99999);
    elTeach->Add(edtOutArmYAlignBe,           &Tech.iOutArmCCD_Picker_PosY[1][4], ECInteger, "ArmAlignment", "OutArmYAlignmentBe",  bShow, bEnable, bFixedValue,     0, false, 99999, -99999);
    elTeach->Add(edtOutArmYAlignBf,           &Tech.iOutArmCCD_Picker_PosY[1][5], ECInteger, "ArmAlignment", "OutArmYAlignmentBf",  bShow, bEnable, bFixedValue,     0, false, 99999, -99999);
    elTeach->Add(edtOutArmYAlignBg,           &Tech.iOutArmCCD_Picker_PosY[1][6], ECInteger, "ArmAlignment", "OutArmYAlignmentBg",  bShow, bEnable, bFixedValue,     0, false, 99999, -99999);
    elTeach->Add(edtOutArmYAlignBh,           &Tech.iOutArmCCD_Picker_PosY[1][7], ECInteger, "ArmAlignment", "OutArmYAlignmentBh",  bShow, bEnable, bFixedValue,     0, false, 99999, -99999);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::SetButton140Click(TObject *Sender)
{
    int Tag;
    TSpeedButton *Ptr;
    ActiveFlag[0]=true;
    ActiveFlag[1]=false;
    ActiveFlag[2]=false;

    Ptr=(TSpeedButton *)Sender;
    Ptr->Down=false;
    Tag=Ptr->Tag;
    ActiveMotorIndex=TechPara[Tag]->MotorSelect;
    if(USE_PICKER_COUNT==ep1Picker && (ActiveMotorIndex==7 || ActiveMotorIndex==26))    //Ifor 20260109 fix 單一吸嘴模組顯示異常問題
    {
        ActiveMotorIndex=ActiveMotorIndex-4;
    }
    EditPtr=TechPara[Tag]->SetEdit;

    if(ActiveMotorIndex==-1)
        return;

    UpdateMotorTeachMonitor(ActiveMotorIndex);
    MOT[ActiveMotorIndex].iGali_SingalHomeTask=1;

    if(ActiveMotorIndex!=MTestZ1 && ActiveMotorIndex!=MTestZ2)
    {
        if(MOT[ActiveMotorIndex].HomeFlag==0)
        {
            ShowMyMessage("motor need home!!", "馬達需要歸零", "Teach");
            return;
        }
        else
        {
            if(ActiveMotorIndex!=MTrayBracketZ && ActiveMotorIndex!=MMagazine)  //JerryYang 20220909 : add magazine
                MOT[ActiveMotorIndex].ServoOnOff(false);
            fTeachShow->ShowModal();
            MOT[ActiveMotorIndex].ServoOnOff(true);
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfTeach::GoButton140Click(TObject *Sender)                      //Tray Arm Teaching
{
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;
    int Tag, iNewPos;
    Tag=Ptr->Tag;

    ActiveMotorIndex=TechPara[Tag]->MotorSelect;
    if(USE_PICKER_COUNT==ep1Picker && (ActiveMotorIndex==7 || ActiveMotorIndex==26))    //Ifor 20260109 fix 單一吸嘴模組顯示異常問題
    {
        ActiveMotorIndex=ActiveMotorIndex-4;
    }
    if(CheckCanMove()==false || ActiveMotorIndex==-1 || IsCanQuickJogMove()==false)
        return;
    Ptr->Down=false;

    #ifndef SOFT_SIMULTE
    if(MOT[ActiveMotorIndex].Motor->Enable==false)
        return;
    #endif
    if(MOT[ActiveMotorIndex].HomeFlag==0)
    {
        ShowMyMessage("motor need home", "馬達需要歸零", "Teach");
        return;
    }

    EditPtr=TechPara[Tag]->SetEdit;
    MOT[ActiveMotorIndex].SetSpeed(ScrollBar1->Position);
    UpdateMotorTeachMonitor(ActiveMotorIndex);
    iNewPos=atoi(EditPtr->Text.c_str());
    if(iNewPos>MOT[ActiveMotorIndex].Motor->PSoftLimitP)
    {
        ShowMyMessage("Over max soft limit, abort process!", "超過軟體正極限");
        return;
    }
    if(iNewPos<MOT[ActiveMotorIndex].Motor->PSoftLimitN)
    {
        ShowMyMessage("Bellow min soft limit, abort process!", "小於軟體負極限");
        return;
    }
    if(INDEX_MOTION_CARD==0 && (ActiveMotorIndex==MTestY1 || ActiveMotorIndex==MTestZ1 ||
                                ActiveMotorIndex==MTestZ2 || ActiveMotorIndex==MTestY2))  //Steven 20210623 : Index使用Galil
    {
        MOT[ActiveMotorIndex].MovFlag=false;
        MOT[ActiveMotorIndex].Gali_MotMove(iNewPos, MOT[ActiveMotorIndex].Motor->ReadSpeed()*100);
    }
    else
    {
        MOT[ActiveMotorIndex].InitMOTParameter();
        MOT[ActiveMotorIndex].SetSpeed(1);
        int iBacklashSet=0;
        if(ActiveMotorIndex==MInRotateKit || ActiveMotorIndex==MOutRotateKit)   //RogerYang 20260113 : Rotator新增背隙補償
        {
            int iBacklash=0;
            bool bIsInRot=(ActiveMotorIndex==MInRotateKit)?true:false;
            if(bIsInRot==true)
            {
                iBacklashSet=atoi(edtEditRotateInBacklash->Text.c_str());
            }
            else
            {
                iBacklashSet=atoi(edtEditRotateOutBacklash->Text.c_str());
            }
            iBacklash=MOT[ActiveMotorIndex].GetRotatorBacklash(iNewPos, bIsInRot, iBacklashSet);
            iNewPos+=iBacklash;
        }
        MOT[ActiveMotorIndex].MotorMove(iNewPos);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::GetTechPos()
{
    if(ActiveMotorIndex==-1)
        return;

    if(ActiveFlag[0])
    {
        if(ActiveMotorIndex==MTestZ1 || ActiveMotorIndex==MTestZ2)
        {
            EditPtr->Text=MOT[ActiveMotorIndex].Gali_ReadEncoderPos();
        }
        else
        {
            if(MOTION_CARD_TYPE==MotionCard_Contec && MOT[ActiveMotorIndex].Motor->MotorType==0)
            {
                EditPtr->Text=MOT[ActiveMotorIndex].ReadPos();
            }
            else
            {
                EditPtr->Text=MOT[ActiveMotorIndex].ReadEncoderPos();
            }
        }
    }
    if(ActiveFlag[1])
    {
        if(INDEX_MOTION_CARD==0 && (TwoActiveMotorIndex==MTestY1 || TwoActiveMotorIndex==MTestZ1 ||
                                    TwoActiveMotorIndex==MTestZ2 || TwoActiveMotorIndex==MTestY2))  //Steven 20210623 : Index使用Galil
        {
            if(OldIndex>0)
                EditPtr->Text=MOT[OldIndex].Gali_ReadEncoderPos();
            EditPtr1->Text=MOT[TwoActiveMotorIndex].Gali_ReadEncoderPos();
        }
        else
        {
            if(iTechSelMot[0]>=0)
                EditPtr->Text=MOT[iTechSelMot[0]].ReadEncoderPos();
            if(iTechSelMot[1]>=0)
                EditPtr1->Text=MOT[iTechSelMot[1]].ReadEncoderPos();
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::SetButton020Click(TObject *Sender)
{
    int Tag;
    TSpeedButton *Ptr;

    Ptr=(TSpeedButton *)Sender;
    Tag=Ptr->Tag;
    ActiveFlag[0]=false;
    ActiveFlag[1]=true;
    ActiveFlag[2]=false;

    EditPtr=TechTwoPara[Tag]->SetEdit[0];
    EditPtr1=TechTwoPara[Tag]->SetEdit[1];

    Ptr->Down=false;                                                            //Steven 20120823 : 換位置
    for(int i=0; i<2; i++)
    {
        TwoActiveMotorIndex=TechTwoPara[Tag]->MotorSelect[i];
        iTechSelMot[i]=TwoActiveMotorIndex;
        if(TwoActiveMotorIndex<0)
            return;
        UpdateMotorTeachMonitor(TwoActiveMotorIndex);
        MOT[TwoActiveMotorIndex].iGali_SingalHomeTask=1;

        if(MOT[TwoActiveMotorIndex].HomeFlag==0)
        {
            ShowMyMessage("motor need home", "馬達需要歸零", "Teach");
            return;
        }

        MOT[TwoActiveMotorIndex].ServoOnOff(false);
    }
    fTeachShow->ShowModal();

    for(int i=0; i<2; i++)
    {
        TwoActiveMotorIndex=TechTwoPara[Tag]->MotorSelect[i];
        if(TwoActiveMotorIndex>=0)
            MOT[TwoActiveMotorIndex].ServoOnOff(true);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::GoButton020Click(TObject *Sender)
{
    int Tag, iNewPos;

    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;

    Tag=Ptr->Tag;
    Ptr->Down=false;

    for(int i=0; i<2; i++)                                                      //Steven 20091004 Start: Before move X or Y, need check Z at home
    {
        ActiveMotorIndex=TechTwoPara[Tag]->MotorSelect[i];
        if(CheckCanMove()==false || ActiveMotorIndex==-1 || IsCanQuickJogMove()==false)
            return;
    }

    for(int i=0; i<2; i++)
    {
        if(USE_INDEX_ARM_AXES==IndexArm_3_Axis)                                 //JimmyChiu 20220708 : add Index Arm Axis
        {
            if((ActiveMotorIndex==MTestZ1 || ActiveMotorIndex==MTestZ2) && i>0)                           //jimmychiu 20220805 add
                continue;
            if((ActiveMotorIndex==MTestY1 || ActiveMotorIndex==MTestY2) && i>0)                           //jimmychiu 20220805 add
                continue;
        }
        OldIndex=TechTwoPara[Tag]->MotorSelect[0];
        TwoActiveMotorIndex=TechTwoPara[Tag]->MotorSelect[i];

#ifndef SOFT_SIMULTE
        if(MOT[TwoActiveMotorIndex].Motor->Enable==false)
            return;
#endif

        EditPtr=TechTwoPara[Tag]->SetEdit[i];
        ScrollBar1->Position=20;                                                //Steven 20210723 : 應驗證課要求,Go按鈕速度提升到20%
        MOT[TwoActiveMotorIndex].SetSpeed(ScrollBar1->Position);
        UpdateMotorTeachTwoMonitor(OldIndex, TwoActiveMotorIndex);
        iNewPos=atoi(EditPtr->Text.c_str());
        if(iNewPos>MOT[TwoActiveMotorIndex].Motor->PSoftLimitP)
        {
            ShowMyMessage("Over max soft limit, abort process!", "超過軟體正極限");
            return;
        }
        if(iNewPos<MOT[TwoActiveMotorIndex].Motor->PSoftLimitN)
        {
            ShowMyMessage("Bellow min soft limit, abort process!", "小於軟體負極限");
            return;
        }

        if(INDEX_MOTION_CARD==0 && (TwoActiveMotorIndex==MTestY1 || TwoActiveMotorIndex==MTestZ1 ||
                                    TwoActiveMotorIndex==MTestZ2 || TwoActiveMotorIndex==MTestY2))  //Steven 20210623 : Index使用Galil
        {
            MOT[TwoActiveMotorIndex].MovFlag=false;
            MOT[TwoActiveMotorIndex].Gali_MotMove(iNewPos, MOT[TwoActiveMotorIndex].Motor->PJogLowSpeed*100);
        }
        else
        {
            MOT[TwoActiveMotorIndex].InitMOTParameter();
            MOT[TwoActiveMotorIndex].SetSpeed(20);                              //Steven 20210723 : 應驗證課要求,Go按鈕速度提升到20%
            MOT[TwoActiveMotorIndex].MotorMove(iNewPos);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::PageControl2Change(TObject *Sender)
{
//    TabSheet20->Visible=false;
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::MotorTrayXClick(TObject *Sender)
{
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;
    int Tag=Ptr->Tag;

    ActiveMotorIndex=TechMotorAxle[Tag]->MotorSelect;

    for(int i=0; i<TechMotorItem; i++)                                          //Steven 20120901 : 一次只有一個被按下去
    {
        if(TechMotorAxle[i]->MotorSelect!=ActiveMotorIndex)
            TechMotorAxle[i]->SelButton->Down=false;
    }

    UpdateMotorTeachMonitor(ActiveMotorIndex);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::SetButton064Click(TObject *Sender)
{
    TSpeedButton *Ptr;

    ActiveFlag[0]=false;
    ActiveFlag[1]=true;
    ActiveFlag[2]=false;

    Ptr=(TSpeedButton *)Sender;
    int Tag=Ptr->Tag;
    Ptr->Down=false;

    OldIndex=TechTwoPara[Tag]->MotorSelect[0];
    EditPtr =TechTwoPara[Tag]->SetEdit[0];
    EditPtr1=TechTwoPara[Tag]->SetEdit[1];

    MOT[TwoActiveMotorIndex].iGali_SingalHomeTask=1;
    for(int i=0; i<2; i++)
    {
        TwoActiveMotorIndex=TechTwoPara[Tag]->MotorSelect[i];
        iTechSelMot[i]=TwoActiveMotorIndex;

        if(TwoActiveMotorIndex<0)
        {
            return;
        }

        if(MOT[TwoActiveMotorIndex].HomeFlag==0)
        {
            ShowMyMessage("motor need home", "馬達需要歸零", "Teach");
            return;
        }

        if(TwoActiveMotorIndex!=MTestY1 && TwoActiveMotorIndex!=MTestY2)
            MOT[TwoActiveMotorIndex].ServoOnOff(false);
    }
    UpdateMotorTeachTwoMonitor(OldIndex, TwoActiveMotorIndex);
    fTeachShow->ShowModal();

    for(int i=0; i<2; i++)
    {
        TwoActiveMotorIndex=TechTwoPara[Tag]->MotorSelect[i];
        if(TwoActiveMotorIndex!=MTestY1 && TwoActiveMotorIndex!=MTestY2)
            MOT[TwoActiveMotorIndex].ServoOnOff(true);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnRecordY1Click(TObject *Sender)
{
    GetTechPos();
}
//---------------------------------------------------------------------------
AnsiString __fastcall TfTeach::CheckEditNumber(int iMax, int iMin, AnsiString asCurrent)
{
    int iCurrent=atoi(asCurrent.c_str());

    if(iMin>iMax)                                                               //Steven 20140505 : 要確保MAX > MIN
    {
        int Temp=iMax;
        iMax=iMin;
        iMin=Temp;
    }

    if(iCurrent>iMax)
    {
        return AnsiString(iMax);
    }
    else if(iCurrent<iMin)
    {
        return AnsiString(iMin);
    }
    else
    {
        return asCurrent;
    }
}

//---------------------------------------------------------------------------
void __fastcall TfTeach::setEditLimitClick(TObject *Sender)
{
    TEdit *Ptr;
    Ptr=(TEdit *)Sender;
    if(Ptr==setEditWaitTestZDown)                                               //Steven 20100708 Wait_TestZ_Down 必須小於零
    {
        int iMinDis=(USE_INDEX_ARM_AXES==IndexArm_3_Axis)?-2000:-1500;          //JimmyChiu 20220708 : add Index Arm Axis
        fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 0, iMinDis);
        Ptr->Text=CheckEditNumber(0, iMinDis, Ptr->Text);
        if(atoi(Ptr->Text.c_str())>atoi(setEditTestZSafePos->Text.c_str()))
        {
            Ptr->Text=setEditTestZSafePos->Text;
        }
    }
    else if(Ptr==setEditTestZSafePos)                                           //Steven 20100708 TestZSafePoision 必須小於2500
    {
        if(CUSTOMER_CODE==CC_KYEC_LEE)
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 300, -1500);
        }
        else
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 1500, -1500);
        }
        if(atoi(Ptr->Text.c_str())<atoi(setEditWaitTestZDown->Text.c_str()))
        {
            Ptr->Text=setEditWaitTestZDown->Text;
        }
    }
    else if(Ptr==setEditLoaderX || Ptr==setEditOcrX)                            //Steven 2010112 Loader X 限制
    {
        if(MachineTypeChoice==Type_HT9046_LS || MachineTypeChoice==Type_HT1032) //Steven 20140528 : 修正9046LS的Teach限制範圍
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 29000, 19000);
        }
        else
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 23000, 19000);
        }
    }
    else if(Ptr==setEditLoaderY || Ptr==setEditOcrY)                            //Steven 2010112 Loader Y 限制
    {
        if(MachineTypeChoice==Type_HT9046_LS ||                                 //Steven 20140528 : 修正9046LS的Teach限制範圍
           MachineTypeChoice==Type_HT1032)
        {
            if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||
               USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                    //Ztex 2024.02.24 Add HT-1132
            {
                fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -60000, -68000);
            }
            else
            {
                fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -70000,  -74000);
            }
        }
        else
        {
            if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||
               USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                    //Ztex 2024.02.24 Add HT-1132
            {
                fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -45000, -100000);
            }
            else
            {
                fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -53000, -100000);
            }
        }
    }
    else if(Ptr==setEditHP1X || Ptr==setEdtHP1LaserX ||                         //Steven 2010112 HP1 X 限制
            Ptr==setEditHP2X || Ptr==setEdtHP2LaserX)                           //Steven 2010112 HP2 X 限制
    {
        if(HOT_PLATE_POSITION==0)                                               //Steven 20140222 : Hot Plate Pin的位置
        {
            if(MachineTypeChoice==Type_HT9046_LS ||                             //Steven 20140528 : 修正9046LS的Teach限制範圍
               MachineTypeChoice==Type_HT1032)
            {
                fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 5000, -1000);
            }
            else
            {
                fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 5000, -1000);
            }
        }
        else
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 12000, 4000);//Ztex 8000->12000
        }
    }
    else if(Ptr==setEditHP1Y ||                                                 //Steven 2010112 HP1 Y 限制
            Ptr==setEdtHP1LaserY)
    {
        if(HOT_PLATE_POSITION==0)                                               //Steven 20140222 : Hot Plate Pin的位置
        {
            if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||                      //Steven 20160628 : Add for XY-Pitch
               USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                    //Ztex 2024.02.24 Add HT-1132
            {
                fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -40000, -50000);
            }
            else if(MachineTypeChoice==Type_HT9046_LS ||                        //Steven 20140528 : 修正9046LS的Teach限制範圍
                    MachineTypeChoice==Type_HT1032)
            {
                fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -46000, -53000);  //JerryYang 20241220 : 修改最大值-46000
            }
            else
            {
                fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -46000, -53000);  //JerryYang 20241220 : 修改最大值-46000
            }
        }
        else
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -80000, -91000);
        }
    }
    else if(Ptr==setEditInSht1X ||                                              //Steven 2010112 IN SH1 X 限制
            Ptr==setEditInSht2X)
    {
        if(MachineTypeChoice==Type_HT9046_LS ||                                 //Steven 20140528 : 修正9046LS的Teach限制範圍
           MachineTypeChoice==Type_HT1032)
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 39000, 26000);//Ztex33000 ->27000
        }
        else if(USE_HOTPLATE_TYPE==1)                                           //Hotplate 2.0x
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 45000, 39000);
        }
        else
        {
            if(FIX3_FULL_PLACE==Fix3K_ShortShuttle)
            {
                fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 37000, 33000);
            }
            else
            {
                fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 35000, 29000);
            }
        }
    }
    else if(Ptr==setEditInSht1Y)                                                //Steven 2010112 IN SH1 Y 限制
    {
        if(MachineTypeChoice==Type_HT9046_LS ||                                 //Steven 20140528 : 修正9046LS的Teach限制範圍
           MachineTypeChoice==Type_HT1032)
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -41000, -55000);
        }
        else
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -35000, -41000);
        }
    }
    else if(Ptr==setEditInSht2Y)                                                //Steven 2010112 IN SH2 Y 限制
    {
        if(MachineTypeChoice==Type_HT9046_LS ||                                 //Steven 20140528 : 修正9046LS的Teach限制範圍
           MachineTypeChoice==Type_HT1032)
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -2000, -9000);
        }
        else
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -2000, -9000);
        }
    }
    else if(Ptr==setEditAutoCleanX || Ptr==setInPickX)                          //jou 2012-05-21 Auto Clean X 限制
    {
        if(USE_HOTPLATE_TYPE==1)                                                //Hotplate 2.0x
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 24371, 20371);
        }
        else
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 0, 30000);
        }
    }
    else if(Ptr==setEditAutoCleanY || Ptr==setInPickY)                          //jou 2012-05-21 Auto Clean Y 限制
    {
        if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable ||
           USE_IN_OUT_ARM_Y_PITCH==iXYPitchIn_Bb_Out_Bc)                        //Ztex 2024.02.24 Add HT-1132
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -5000, -92000);
        }
        else
        {
            if(CUSTOMER_CODE==CC_ASE_KaohSiung)
            {
                fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -5000, -95000);
            }
            else
            {
                fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -5000, -92000);
            }
        }
    }
    else if(Ptr==setEdLoadCellY1)                                               //kevin 20190305 add load ARM1 Y POS
    {
        fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 50000, 0);
    }
    else if(Ptr==setEdLoadCellY2)                                               //kevin 20190305 add load ARM2 Y POS
    {
        fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -50000, 0);
    }
    else if(Ptr==setEdLoadCellZ1 ||
            Ptr==setEdLoadCellZ2)                                               //kevin 20190305 add load ARM 1 2 Z POS
    {
        fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -130000, 0);
    }
    else if(Ptr==setEditOutSht1X ||                                             //Steven 2010112 OUT SH1 X 限制
            Ptr==setEditOutSht2X)                                               //Steven 2010112 OUT SH2 X 限制
    {
        if(MachineTypeChoice==Type_HT9046_LS ||                                 //Steven 20140528 : 修正9046LS的Teach限制範圍
           MachineTypeChoice==Type_HT1032)
        {
            if(AUTO_EMPTY_COLOR>=3)
            {
                fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -85000, -90000);
            }
            else
            {
                fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -45000, -52000);
            }
        }
        else
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -46000, -52000);
        }
    }
    else if(Ptr==setEditOutSht1Y)                                               //Steven 2010112 OUT SH1 Y 限制
    {
        if(MachineTypeChoice==Type_HT9046_LS ||                                 //Steven 20140528 : 修正9046LS的Teach限制範圍
           MachineTypeChoice==Type_HT1032)
        {
            if(AUTO_EMPTY_COLOR>=3)                                             //RogerYang 20251222 : add for 9011UC
            {
                fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -46000, -53000);
            }
            else
            {
                fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -48000, -55000);
            }
        }
        else
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -35000, -41000);
        }
    }
    else if(Ptr==setEditOutSht2Y)                                               //Steven 2010112 OUT SH2 Y 限制
    {
        if(MachineTypeChoice==Type_HT9046_LS ||                                 //Steven 20140528 : 修正9046LS的Teach限制範圍
           MachineTypeChoice==Type_HT1032)
        {
            if(AUTO_EMPTY_COLOR>=3)                                             //RogerYang 20251222 : add for 9011UC
            {
                fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -800, -7800);
            }
            else
            {
                fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -2000, -9000);
            }
        }
        else
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -3000, -9000);
        }
    }
    else if(Ptr==setEditFix1Y ||                                                //Steven 2010112 FIX1 Y 限制
            Ptr==setEditFix2Y ||                                                //Steven 2010112 FIX2 Y 限制
            Ptr==setEditFix3Y ||                                                //Steven 2010112 FIX3 Y 限制
            Ptr==setEditFix4Y ||                                                //RogerYang 20251222 : add for 9011UC
            Ptr==setEditFix5Y ||
            Ptr==setEditFix6Y)
    {
        if(MachineTypeChoice==Type_HT9046_LS ||                                 //Steven 20140528 : 修正9046LS的Teach限制範圍
           MachineTypeChoice==Type_HT1032)
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -7000, -20000);
        }
        else
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -7000, -13000);
        }
    }
    else if(Ptr==setEditFix1X)                                                  //Steven 2010112 FIX1 X 限制
    {
        if(MachineTypeChoice==Type_HT9046_LS ||                                 //Steven 20140528 : 修正9046LS的Teach限制範圍
           MachineTypeChoice==Type_HT1032)
        {
            if(AUTO_EMPTY_COLOR>=3)                                             //RogerYang 20251222 : add for 9011UC
            {
                fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -80000, -88000);
            }
            else
            {
                fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -40000, -46000);
            }
        }
        else if(USE_PICKER_COUNT==0)                                            //Ifor 20170309 (wei) add HT9045S Fix盤X軸 Teach 限制範圍
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -33000, -46000);
        }
        else
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -33000, -39000);
        }
    }
    else if(Ptr==setEditFix2X)                                                  //Steven 2010112 FIX2 X 限制
    {
        if(MachineTypeChoice==Type_HT9046_LS ||                                 //Steven 20140528 : 修正9046LS的Teach限制範圍
           MachineTypeChoice==Type_HT1032)
        {
            if(AUTO_EMPTY_COLOR>=3)                                             //RogerYang 20251222 : add for 9011UC
            {
                fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -64000, -72000);
            }
            else
            {
                fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -25000, -32000);
            }
        }
        else if(USE_PICKER_COUNT==0)                                            //Ifor 20170309 (wei) add HT9045S Fix盤X軸 Teach 限制範圍
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -18000, -32000);
        }
        else
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -18000, -24000);
        }
    }
    else if(Ptr==setEditFix3X)                                                  //Steven 2010112 FIX3 X 限制
    {
        if(MachineTypeChoice==Type_HT9046_LS ||                                 //Steven 20140528 : 修正9046LS的Teach限制範圍
           MachineTypeChoice==Type_HT1032)
        {
            if(AUTO_EMPTY_COLOR>=3)                                             //RogerYang 20251222 : add for 9011UC
            {
                fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -50000, -58000);
            }
            else
            {
                fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -11000, -20000);
            }
        }
        else if(USE_PICKER_COUNT==0)                                            //Ifor 20170309 (wei) add HT9045S Fix盤X軸 Teach 限制範圍
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -4000, -20000);
        }
        else
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -4000, -10000);
        }
    }
    else if(Ptr==setEditFix4X)                                                  //RogerYang 20251222 : add for 9011UC
    {
        if(AUTO_EMPTY_COLOR>=3)
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -36000, -44000);
        }
    }
    else if(Ptr==setEditFix5X)                                                  //RogerYang 20251222 : add for 9011UC
    {
        if(AUTO_EMPTY_COLOR>=3)
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -22000, -30000);
        }
    }
    else if(Ptr==setEditFix6X)                                                  //RogerYang 20251222 : add for 9011UC
    {
        if(AUTO_EMPTY_COLOR>=3)
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -7000, -15000);
        }
    }
    else if(Ptr==setEditAuto1Y ||                                               //Steven 2010112 Auto1 Y 限制
            Ptr==setEditAuto2Y ||                                               //Steven 2010112 Auto2 Y 限制
            Ptr==setEditAuto3Y ||                                               //Steven 2010112 Auto3 Y 限制
            Ptr==setOutPickY)
    {
        if(MachineTypeChoice==Type_HT9046_LS || MachineTypeChoice==Type_HT1032)   //Steven 20140528 : 修正9046LS的Teach限制範圍
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -60000, -74000);
        }
        else
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -53000, -59000);
        }
    }
    else if(Ptr==setEditAuto1X)                                                 //Steven 2010112 Auto1 X 限制
    {
        if(MachineTypeChoice==Type_HT9046_LS ||                                 //Steven 20140528 : 修正9046LS的Teach限制範圍
           MachineTypeChoice==Type_HT1032)
        {
            if(AUTO_EMPTY_COLOR>=3)                                             //RogerYang 20251222 : add for 9011UC
            {
                fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -87000, -95000);
            }
            else
            {
                fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -46000, -56000);
            }
        }
        else
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -50000, -56000);
        }
    }
    else if(Ptr==setEditAuto2X)                                                 //Steven 2010112 Auto2 X 限制
    {
        if(MachineTypeChoice==Type_HT9046_LS ||                                 //Steven 20140528 : 修正9046LS的Teach限制範圍
           MachineTypeChoice==Type_HT1032)
        {
            if(AUTO_EMPTY_COLOR>=3)                                             //RogerYang 20251222 : add for 9011UC
            {
                fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -70000, -78000);
            }
            else
            {
                fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -30000, -37000);
            }
        }
        else
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -32000, -38000);
        }
    }
    else if(Ptr==setEditAuto3X ||                                               //Steven 2010112 Auto3 X 限制
            Ptr==setOutPickX)
    {
        if(MachineTypeChoice==Type_HT9046_LS ||                                 //Steven 20140528 : 修正9046LS的Teach限制範圍
           MachineTypeChoice==Type_HT1032)
        {
            if(AUTO_EMPTY_COLOR>=3)                                             //RogerYang 20251222 : add for 9011UC
            {
                fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -52000, -60000);
            }
            else
            {
                fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -12000, -19000);
            }
        }
        else
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -13000, -19000);
        }
    }
    else if(Ptr==setEditAuto4X)                                                  //RogerYang 20251222 : add for 9011UC
    {
        if(AUTO_EMPTY_COLOR>=3)
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -36000, -40000);
        }
    }
    else if(Ptr==setEditAuto5X)                                                  //RogerYang 20251222 : add for 9011UC
    {
        if(AUTO_EMPTY_COLOR>=3)
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -14000, -22000);
        }
    }
    else if(Ptr==edtBinBoxX)                                                    //Steven 2010112 Bin Box X 限制
    {
        fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 0, -55480);
    }
    else if(Ptr==edtBinBoxY)                                                    //Steven 2010112 Bin Box Y 限制
    {
        fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 0, -40000);
    }
    else if(Ptr==setEditTopViewX)                                               //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
    {
        if(USE_Top_Scanner_AOI_Inspection==true)
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -16000, -50000);
        }
        else
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -13000, -16000);
        }
    }
    else if(Ptr==setEditTopViewY)                                               //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
    {
        if(USE_Top_Scanner_AOI_Inspection==true)
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -6000, -40000);
        }
        else
        {
            fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -30000, -35000);
        }
    }
    else if(Ptr==setEditPADViewX)                                               //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
    {
        fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -13000, -16000);
    }
    else if(Ptr==setEditPADViewY)                                               //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
    {
        fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -23000, -27000);
    }
    else if(Ptr==setEditBGAViewX ||                                             //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
            Ptr==setEditScannerAOIX)
    {
        fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -13000, -16000);
    }
    else if(Ptr==setEditBGAViewY ||                                             //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
            Ptr==setEditScannerAOIY)
    {
        fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -17000, -20000);
    }
    else if(Ptr==setEditSafePosX)                                               //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
    {
        fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -13000, -19000);
    }
    else if(Ptr==setEditSafePosY)                                               //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
    {
        fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -10000, -15000);
    }
    else
    {
        fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -200000, 200000);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnZ1ServoClick(TObject *Sender)
{
    static bool bflag=false;
    if(bflag==false)
    {
        MOT[MTestZ1].Gali_Command("MOY", __FUNC__);
        btnZ1Servo->Caption="Server OFF";
    }
    else
    {
        MOT[MTestZ1].Gali_Command("SHY", __FUNC__);
        btnZ1Servo->Caption="Server ON";
    }
    bflag=!bflag;
    fAllMotorHome=false;
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnZ2ServoClick(TObject *Sender)
{
    static bool bflag=false;
    if(bflag==false)
    {
        MOT[MTestZ2].Gali_Command("MOZ", __FUNC__);
        btnZ2Servo->Caption="Server OFF";
    }
    else
    {
        MOT[MTestZ2].Gali_Command("SHZ", __FUNC__);
        btnZ2Servo->Caption="Server ON";
    }
    bflag=!bflag;
    fAllMotorHome=false;
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnServoClick(TObject *Sender)
{
    btnServo->Down=false;
    if(ActiveMotorIndex==-1)
        return;
    MOT[ActiveMotorIndex].ServoOnOff(!MOT[ActiveMotorIndex].Led[iServoOn]);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::Label2Click(TObject *Sender)
{
    PageControl2->ActivePage=tsAxleCtrl;
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnSetAllInArmZClick(TObject *Sender)
{
//    if(bInArmAllUp==false)                                                    //Jimmychiu 20260223 : Mark for Set all in arm Z not work
//    {
//        ShowMyMessage("Must press 'In Z All Up' first", "");
//        return;
//    }
//    bInArmAllUp=false;

    int iStandZ;
    int iZ[2][8];
    ZeroMemory(iZ, sizeof(iZ));
    if(USE_PICKER_COUNT==0)                                                     //Ifor 20170309 (wei) add HT9045S 未使用的吸嘴不讀取目前位置
    {
        for(int i=0; i<InArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<InArmSuck.iMaxCol; j++)
            {
                if(i==0 || i==2)
                    InZEditPtr[i][j]->Text=MOT[InArmZIndex[i][i]].ReadPos();
                else
                    InZEditPtr[i][j]->Text=0;
            }
        }
    }
    else
    {
        for(int i=0; i<InArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<InArmSuck.iMaxCol; j++)
            {
                InZEditPtr[i][j]->Text=MOT[InArmZIndex[i][j]].ReadPos();
            }
        }
    }
    iStandZ=atoi(InZEditPtr[iInArmYBase][iInArmXBase]->Text.c_str());

    if(USE_PICKER_COUNT==0)                                                     //Ifor 20170328 (wei) add HT9045S 未使用的吸嘴不讀取目前位置
    {
        for(int i=0; i<InArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<InArmSuck.iMaxCol; j++)
            {
                if(i==iInArmYBase && j==iInArmXBase)
                    continue;
                iZ[i][j]=atoi(InZEditPtr[i][j]->Text.c_str())-iStandZ;
                if(i==0 || i==2)
                    InZEditPtr[i][j]->Text=iZ[i][j];
                else
                    InZEditPtr[i][j]->Text=0;
            }
        }
    }
    else
    {
        for(int i=0; i<InArmSuck.iMaxRow; i++)                                  //Ztex 2023.12.27 For HT-1032 AT
        {
            for(int j=0; j<InArmSuck.iMaxCol; j++)
            {
                iZ[i][j]=atoi(InZEditPtr[i][j]->Text.c_str())-iStandZ;
                InZEditPtr[i][j]->Text=iZ[i][j];
            }
        }
        InZEditPtr[iInArmYBase][iInArmXBase]->Text=0;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnSetAllOutArmZClick(TObject *Sender)
{
//    if(bOutArmAllUp==false)                                                   //Jimmychiu 20260223 : Mark for Set all in arm Z not work
//    {
//        ShowMyMessage("Must press 'Out Z All Up' first", "");
//        return;
//    }
//    bOutArmAllUp=false;

    int iStandZ;
    int iZ[2][8];

    ZeroMemory(iZ, sizeof(iZ));

    if(USE_PICKER_COUNT==0)                                                     //Ifor 20170309 (wei) add HT9045S 未使用的吸嘴不讀取目前位置
    {
        for(int i=0; i<OutArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<OutArmSuck.iMaxCol; j++)
            {
                if(i==0 || i==2)
                    OutZEditPtr[i][j]->Text=MOT[OutArmZIndex[i][i]].ReadPos();
                else
                    OutZEditPtr[i][j]->Text=0;
            }
        }
    }
    else
    {
        for(int i=0; i<OutArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<OutArmSuck.iMaxCol; j++)
            {
                OutZEditPtr[i][j]->Text=MOT[OutArmZIndex[i][j]].ReadPos();
            }
        }
    }

    iStandZ=atoi(OutZEditPtr[iOutArmYBase][iOutArmXBase]->Text.c_str());        //JerryYang 20241021 : modify

    if(USE_PICKER_COUNT==0)                                                     //Ifor 20170328 (wei) add HT9045S 未使用的吸嘴不讀取目前位置
    {
        for(int i=0; i<OutArmSuck.iMaxRow; i++)
        {
            for(int j=0; j<OutArmSuck.iMaxCol; j++)
            {
                if(i==iOutArmYBase && j==iOutArmXBase)
                    continue;
                iZ[i][j]=atoi(OutZEditPtr[i][j]->Text.c_str())-iStandZ;
                if(i==0 || i==2)
                    OutZEditPtr[i][j]->Text=iZ[i][j];
                else
                    OutZEditPtr[i][j]->Text=0;
            }
        }
    }
    else
    {
        for(int i=0; i<OutArmSuck.iMaxRow; i++)                                 //Ztex 2023.12.27 For HT-1032 AT
        {
            for(int j=0; j<OutArmSuck.iMaxCol; j++)
            {
                iZ[i][j]=atoi(OutZEditPtr[i][j]->Text.c_str())-iStandZ;
                OutZEditPtr[i][j]->Text=iZ[i][j];
            }
        }
        OutZEditPtr[iOutArmYBase][iOutArmXBase]->Text=0;                        //JerryYang 20241021 : modify
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnInZAllUpClick(TObject *Sender)
{
    for(int i=0; i<InArmSuck.iMotRow; i++)
    {
        for(int j=0; j<InArmSuck.iMotCol; j++)
        {
            bInArmZHome[i][j]=false;
            InitProcessSingleMotorTask(InArmZIndex[i][j]);
        }
    }

//    bInArmAllUp=true;
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnOutZAllUpClick(TObject *Sender)
{
    for(int i=0; i<OutArmSuck.iMotRow; i++)
    {
        for(int j=0; j<OutArmSuck.iMotCol; j++)
        {
            bOutArmZHome[i][j]=false;
            InitProcessSingleMotorTask(OutArmZIndex[i][j]);
        }
    }

//    bOutArmAllUp=true;
}
//---------------------------------------------------------------------------
bool TfTeach::DoZHome()
{
    bool bFlag[2]={false, false};
    for(int i=0; i<InArmSuck.iMotRow; i++)
    {
        for(int j=0; j<InArmSuck.iMotCol; j++)
        {
            if(bInArmZHome[i][j]==false)
            {
                bInArmZHome[i][j]=ProcessSingleMotorHome(InArmZIndex[i][j]);
                bFlag[0]=true;
            }
            if(bOutArmZHome[i][j]==false)
            {
                bOutArmZHome[i][j]=ProcessSingleMotorHome(OutArmZIndex[i][j]);
                bFlag[1]=true;
            }
        }
    }

    if(bFlag[0])
        btnInZAllUp->Caption="Homeing...";
    else
        btnInZAllUp->Caption="In Z All Up";

    if(bFlag[1])
        btnOutZAllUp->Caption="Homeing...";
    else
        btnOutZAllUp->Caption="Out Z All Up";

    if(bFlag[0]==false && bFlag[1]==false)
        return true;
    else
        return false;
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnOutZAllDownClick(TObject *Sender)
{
    int Pos[2][8];

    for(int i=0; i<OutArmSuck.iMotRow; i++)
    {
        for(int j=0; j<OutArmSuck.iMotCol; j++)
        {
            MOT[OutArmZIndex[i][j]].SetSpeed(10);
            Pos[i][j]=atoi(SetEditPickOutSht->Text.c_str())+atoi(OutZEditPtr[i][j]->Text.c_str());
        }
    }

    for(int i=0; i<OutArmSuck.iMotRow; i++)
    {
        for(int j=0; j<OutArmSuck.iMotCol; j++)
        {
            MOT[OutArmZIndex[i][j]].MotorMove(Pos[i][j]);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::FormDestroy(TObject *Sender)
{
    try
    {
        for(vector<TECH_PARA *>::iterator iter=TechPara.begin(); iter!=TechPara.end(); ++iter)
        {
            delete *iter;
        }
        vec_clr(TechPara);

        for(vector<TECH_MotorAxle *>::iterator iter=TechMotorAxle.begin(); iter!=TechMotorAxle.end(); ++iter)
        {
            delete *iter;
        }
        vec_clr(TechMotorAxle);

        for(vector<TECH_TWOPARA *>::iterator iter=TechTwoPara.begin(); iter!=TechTwoPara.end(); ++iter)
        {
            delete *iter;
        }
        vec_clr(TechTwoPara);
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfTeach::FormDestroy");
    }
    LogSoftwareOffTime("TfTeach, FormDestroy");                                 //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnAutoTeachZClick(TObject *Sender)
{
    bLoadInarmAutoHigh=true;                                                    //kevin 20171107 (wei) Auto teach open vacumm
    Zteach->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::sbBarCodeClick(TObject *Sender)
{
    sbBarCode->Down=false;
    fBarCode->Show();
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::SpeedButtonInRotatePos90Click(TObject *Sender)         //JerryYang 20160302 Rotate正轉90度
{
    SpeedButtonInRotatePos90->Down=false;
    int P1, P2;
    fTechAuto=false;
    if(CheckCanMove()==false || ActiveMotorIndex==-1 || IsCanQuickJogMove()==false)
        return;
    P1=atoi(edtNowPosition->Text.c_str());
    if(iRotate_Type==e4MotRotate ||
       iRotate_Type==e8MotRotate ||                                             //Steven 20191112 : 六軸步進版就是3200轉一圈
       iRotate_Type==e2MotRotate2Dut)
        P2=800;
    else if(iRotate_Type==eInOutArm1Motor)
        P2=1250;                                                                //Ifor 20251204 add: Rotate Pulse下1250等於轉90度
    else
        P2=2000;                                                                //JerryYang 20160302 Rotate Pulse下2000等於轉90度

    if(abs(P1)>999999)                                                          //Steven 20100831 : 溢位保護
    {
        ShowMyMessage("Position over limitation!", "移動位置超過限制!!");
        return;
    }

    if(ActiveMotorIndex==MInRotateKit || ActiveMotorIndex==MInRotateB  ||       //Steven 20170329 (Wei) : Add individual rotate motor
       ActiveMotorIndex==MInRotateC   || ActiveMotorIndex==MInRotateD  ||
       ActiveMotorIndex==MInRotateE   || ActiveMotorIndex==MInRotateF  ||
       ActiveMotorIndex==MInRotateG   || ActiveMotorIndex==MInRotateH  ||
       ActiveMotorIndex==MOutRotateKit || ActiveMotorIndex==MOutRotateB ||
       ActiveMotorIndex==MOutRotateC   || ActiveMotorIndex==MOutRotateD ||
       ActiveMotorIndex==MOutRotateE   || ActiveMotorIndex==MOutRotateF ||
       ActiveMotorIndex==MOutRotateG   || ActiveMotorIndex==MOutRotateH)
    {
        MOT[ActiveMotorIndex].fCMD=false;
        int iBacklash=0;
        if(ActiveMotorIndex==MInRotateKit || ActiveMotorIndex==MOutRotateKit)   //RogerYang 20260113 : Rotator新增背隙補償
        {
            int iBacklashSet=0;
            bool bIsInRot=(ActiveMotorIndex==MInRotateKit)?true:false;
            if(bIsInRot==true)
            {
                iBacklashSet=atoi(edtEditRotateInBacklash->Text.c_str());
            }
            else
            {
                iBacklashSet=atoi(edtEditRotateOutBacklash->Text.c_str());
            }
            iBacklash=MOT[ActiveMotorIndex].GetRotatorBacklash(P1+P2, bIsInRot, iBacklashSet);
        }
        MOT[ActiveMotorIndex].MotorMove(P1+P2+iBacklash);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::SpeedButtonInRotateNeg90Click(TObject *Sender)         //JerryYang 20160302 Rotate反轉90度
{
    SpeedButtonInRotateNeg90->Down=false;
    int P1, P2;
    fTechAuto=false;
    if(CheckCanMove()==false || ActiveMotorIndex==-1 || IsCanQuickJogMove()==false)
        return;
    P1=atoi(edtNowPosition->Text.c_str());
    if(iRotate_Type==e4MotRotate ||
       iRotate_Type==e8MotRotate ||                                             //Steven 20191112 : 六軸步進版就是3200轉一圈
       iRotate_Type==e2MotRotate2Dut)
        P2=800;
    else if(iRotate_Type==eInOutArm1Motor)
        P2=1250;
    else
        P2=2000;                                                                //JerryYang 20160302 Rotate Pulse下2000等於轉90度

    if(abs(P1)>999999)                                                          //Steven 20100831 : 溢位保護
    {
        ShowMyMessage("Position over limitation!", "移動位置超過限制!!");
        return;
    }

    if(ActiveMotorIndex==MInRotateKit || ActiveMotorIndex==MInRotateB  ||       //Steven 20170329 (Wei) : Add individual rotate motor
       ActiveMotorIndex==MInRotateC   || ActiveMotorIndex==MInRotateD  ||
       ActiveMotorIndex==MInRotateE   || ActiveMotorIndex==MInRotateF  ||
       ActiveMotorIndex==MInRotateG   || ActiveMotorIndex==MInRotateH  ||
       ActiveMotorIndex==MOutRotateKit || ActiveMotorIndex==MOutRotateB ||
       ActiveMotorIndex==MOutRotateC   || ActiveMotorIndex==MOutRotateD ||
       ActiveMotorIndex==MOutRotateE   || ActiveMotorIndex==MOutRotateF ||
       ActiveMotorIndex==MOutRotateG   || ActiveMotorIndex==MOutRotateH)
    {
        MOT[ActiveMotorIndex].fCMD=false;
        int iBacklash=0;
        if(ActiveMotorIndex==MInRotateKit || ActiveMotorIndex==MOutRotateKit)   //RogerYang 20260113 : Rotator新增背隙補償
        {
            int iBacklashSet=0;
            bool bIsInRot=(ActiveMotorIndex==MInRotateKit)?true:false;
            if(bIsInRot==true)
            {
                iBacklashSet=atoi(edtEditRotateInBacklash->Text.c_str());
            }
            else
            {
                iBacklashSet=atoi(edtEditRotateOutBacklash->Text.c_str());
            }
            iBacklash=MOT[ActiveMotorIndex].GetRotatorBacklash(P1-P2, bIsInRot, iBacklashSet);
        }
        MOT[ActiveMotorIndex].MotorMove(P1-P2+iBacklash);
    }
}
//---------------------------------------------------------------------------
static int iShuttleSelect=0;
void __fastcall TfTeach::btClearMemoClick(TObject *Sender)
{
    Memo1->Lines->Clear();
    Memo2->Lines->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::EditSh1SpeedClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, (CosFunction.bLimitMaxSpeed)?CosFunction.iLimitMaxSpeed:100, 1);  //Steven 20150420
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnSht1GoLatchClick(TObject *Sender)
{
    if(MOT[MInShuttle1].HomeFlag==0)
    {
        ShowMyMessage("motor need home", "馬達需要歸零", "Teach");
        return;
    }
    else
    {
        iShuttleSelect=1;
        MOT[MInShuttle1].InitMOTParameter();
        MOT[MInShuttle1].SetSpeed(atoi(EditSh1Speed->Text.c_str()));
        MOT[MInShuttle1].MotorMove(Tech.iInShuttle1Right);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnSht2GoLatchClick(TObject *Sender)
{
    if(MOT[MInShuttle2].HomeFlag==0)
    {
        ShowMyMessage("motor need home", "馬達需要歸零", "Teach");
        return;
    }
    else
    {
        iShuttleSelect=2;
        MOT[MInShuttle2].InitMOTParameter();
        MOT[MInShuttle2].SetSpeed(atoi(EditSh2Speed->Text.c_str()));
        MOT[MInShuttle2].MotorMove(Tech.iInShuttle2Right);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnGetLatchClick(TObject *Sender)
{
    if(iShuttleSelect==1)
    {
        for(int i=0; i<fLtcSensor->LatchDataCnt3; i++)
        {
            Memo1->Lines->Add(AnsiString(abs(fLtcSensor->LatchDataTable3[i])));
        }
        for(int i=0; i<fLtcSensor->LatchDataCnt2; i++)
        {
            Memo2->Lines->Add(AnsiString(abs(fLtcSensor->LatchDataTable2[i])));
        }
    }
    else if(iShuttleSelect==2)
    {
        for(int i=0; i<fLtcSensor->LatchDataCnt0; i++)
        {
            Memo1->Lines->Add(AnsiString(abs(fLtcSensor->LatchDataTable0[i])));
        }
        for(int i=0; i<fLtcSensor->LatchDataCnt1; i++)
        {
            Memo2->Lines->Add(AnsiString(abs(fLtcSensor->LatchDataTable1[i])));
        }
    }
}
//---------------------------------------------------------------------------
//kevin 20190305 add read teach.ini
//---------------------------------------------------------------------------
void TfTeach::ReadFile()
{
    AnsiString sUpdateKey="Update2";
    int iFileHandle;
    AnsiString szDir="";

    szDir.sprintf("%s", asTeachPath);                                           //Steven 20100927 Start : Offset改資料夾
    if(FileExists(szDir)==false)
    {
        iFileHandle=FileCreate(szDir);
        FileClose(iFileHandle);
    }

    if(CheckKeyExist(asTeachPath, "Teach INI", sUpdateKey))                     //Steven 20240501 : Teach改存成ini
    {
        bUseIniFile=CheckAndReadIniData(asTeachPath, "Teach INI", sUpdateKey, false);
    }
    else
    {
        bUseIniFile=false;
    }

    if(bUseIniFile==false)
    {
        ReadData("d:\\HT9045\\system\\tech.dat", (char *)&Tech.iZLoad, sizeof(TECH));
        Tech.iInArmPickX =(Tech.iInArmPickX ==0)?Tech.iInArmLoadStageX      :Tech.iInArmPickX;
        Tech.iInArmPickY =(Tech.iInArmPickY ==0)?Tech.iInArmLoadStageY-2000 :Tech.iInArmPickY;
        Tech.iOutArmPickX=(Tech.iOutArmPickX==0)?Tech.iOutArmAuto1X         :Tech.iOutArmPickX;
        Tech.iOutArmPickY=(Tech.iOutArmPickY==0)?Tech.iOutArmAuto1Y-2000    :Tech.iOutArmPickY;
        if(CosFunction.bOutShuttleSensorCanNotDisable)                          //Steven 20151202 : Out Shuttle Sensor不能關閉檢查
        {
            if(Tech.OutSH1ZDetectPos==0)
                Tech.OutSH1ZDetectPos=20599;
            if(Tech.OutSH2ZDetectPos==0)
                Tech.OutSH2ZDetectPos=20469;
            if(Tech.OutSH1ZOneRowDetectPos==0)
                Tech.OutSH1ZOneRowDetectPos=20565;
            if(Tech.OutSH2ZOneRowDetectPos==0)
                Tech.OutSH2ZOneRowDetectPos=20453;
        }

        for(int i=0; i<TECH_MAX_ITEM; i++)
            TechPara[i]->SaveToFile(true);

        for(int i=0; i<TechTwoItem; i++)
            TechTwoPara[i]->SaveToFile(true);

        TechSuckPara[0].SaveToFile(true);                                       //Steven 20240523 : Teach SUCKPARA改存成ini
        TechSuckPara[1].SaveToFile(true);

        bUseIniFile=true;
        WriteIniData(asTeachPath, "Teach INI", sUpdateKey, bUseIniFile);
    }
    else
    {
        if(CheckSectionExist(asTeachPath, "MInShuttle1")==false ||              //Steven 20250507 : 修改group name
           CheckSectionExist(asTeachPath, "MInShuttle2")==false)
        {
            ReadData("d:\\HT9045\\system\\tech.dat", (char *)&Tech.iZLoad, sizeof(TECH));
            for(int i=0; i<TECH_MAX_ITEM; i++)
            {
                if(TechPara[i]->MotorSelect==MInShuttle1 ||
                   TechPara[i]->MotorSelect==MInShuttle2)
                {
                    TechPara[i]->SaveToFile(true);
                }
            }
        }

        for(int i=0; i<TECH_MAX_ITEM; i++)
            TechPara[i]->ReadFromFile();

        for(int i=0; i<TechTwoItem; i++)
            TechTwoPara[i]->ReadFromFile();

        TechSuckPara[0].ReadFromFile();                                         //Steven 20240523 : Teach SUCKPARA改存成ini
        TechSuckPara[1].ReadFromFile();

        Tech.bAOAMatrix=CheckAndReadIniData(asTeachPath, "Teach INI", "bAOAMatrix", false);

        Tech.M_In_iRotateA_Backlash=CheckAndReadIniData(asTeachPath, "MInRotate", "edtEditRotateInBacklash", 0);        //RogerYang 20260113 : Rotator新增背隙補償
        Tech.M_Out_iRotateA_Backlash=CheckAndReadIniData(asTeachPath, "MOutRotate", "edtEditRotateOutBacklash", 0);
        edtEditRotateInBacklash->Text=Tech.M_In_iRotateA_Backlash;                                                      //RogerYang 20260113 : Rotator新增背隙補償
        edtEditRotateOutBacklash->Text=Tech.M_Out_iRotateA_Backlash;
    }

    if(MACHINE_HAS_AUTO_ALIGNMENT_CCD)
    {
        if(Tech.bAOAMatrix==false)
        {
            for(int i=0; i<8; i++)
            {
                Tech.iInArmZBasePickerAlignmentPos[i%2][i/2]           =Tech._iInArmZBasePickerAlignmentPos[i];
                Tech.iOutArmZBasePickerAlignmentPos[i%2][i/2]          =Tech._iOutArmZBasePickerAlignmentPos[i];
                Tech.iInArmCCD_Picker_PosX[i/4][i%4]                   =Tech._iInArmCCD_Picker_PosX[i];
                Tech.iInArmCCD_Picker_PosY[i/4][i%4]                   =Tech._iInArmCCD_Picker_PosY[i];
                Tech.iInArmCCD_Picker_PosZ[i/4][i%4]                   =Tech._iInArmCCD_Picker_PosZ[i];
                Tech.iOutArmCCD_Picker_PosX[i/4][i%4]                  =Tech._iOutArmCCD_Picker_PosX[i];
                Tech.iOutArmCCD_Picker_PosY[i/4][i%4]                  =Tech._iOutArmCCD_Picker_PosY[i];
                Tech.iOutArmCCD_Picker_PosZ[i/4][i%4]                  =Tech._iOutArmCCD_Picker_PosZ[i];
            }

            Tech.iInArmCCD_Pitch_PosX[0][0]=Tech._iInArmCCD_Pitch_PosX[0];
            Tech.iInArmCCD_Pitch_PosX[0][1]=Tech._iInArmCCD_Pitch_PosX[1];
            Tech.iInArmCCD_Pitch_PosX[0][2]=Tech._iInArmCCD_Pitch_PosX[2];
            Tech.iInArmCCD_Pitch_PosX[1][0]=Tech._iInArmCCD_Pitch_PosX[3];
            Tech.iInArmCCD_Pitch_PosX[1][1]=Tech._iInArmCCD_Pitch_PosX[4];
            Tech.iInArmCCD_Pitch_PosX[1][2]=Tech._iInArmCCD_Pitch_PosX[5];
            Tech.iOutArmCCD_Pitch_PosX[0][0]=Tech._iOutArmCCD_Pitch_PosX[0];
            Tech.iOutArmCCD_Pitch_PosX[0][1]=Tech._iOutArmCCD_Pitch_PosX[1];
            Tech.iOutArmCCD_Pitch_PosX[0][2]=Tech._iOutArmCCD_Pitch_PosX[2];
            Tech.iOutArmCCD_Pitch_PosX[1][0]=Tech._iOutArmCCD_Pitch_PosX[3];
            Tech.iOutArmCCD_Pitch_PosX[1][1]=Tech._iOutArmCCD_Pitch_PosX[4];
            Tech.iOutArmCCD_Pitch_PosX[1][2]=Tech._iOutArmCCD_Pitch_PosX[5];
            Tech.bAOAMatrix=true;
            WriteIniData(asTeachPath, "Teach INI", "bAOAMatrix", Tech.bAOAMatrix);
        }
    }
    else
    {
        Tech.bAOAMatrix=true;
    }

    AnsiString FileName=ExtractFileName(asTeachPath);
    AnsiString FilePath=ExtractFilePath(asTeachPath);
    elTeach->ReadEditTextFromFile(FilePath, FileName);                          //JerryYang 20241119 : fix AOA

    if(Teach.iAutoCleanPick==0)                                                 //RogerYang 20250627 移到Read ini之後
    {
        Teach.iAutoCleanPick=Tech.iInArmPlatePickZ2;                            //資料不存在帶入原先
        WriteIniData(szDir, "InArm", "AutoCleanPick", Tech.iInArmPlatePickZ2);  //AutoClean pick Kit
    }

}
//------------------------------------------------------------------------------
//kevin 20190305 add SAVE teach.ini
//------------------------------------------------------------------------------
void TfTeach::SaveFile(bool bSaveByTeach)                                       //Steven 20240501 : Teach改存成ini
{
    WriteData("d:\\HT9045\\system\\tech.dat", (char *)&Tech.iZLoad, sizeof(TECH));
    for(int i=0; i<TECH_MAX_ITEM; i++)
        TechPara[i]->SaveToFile(bSaveByTeach);

    for(int i=0; i<TechTwoItem; i++)
        TechTwoPara[i]->SaveToFile(bSaveByTeach);

    TechSuckPara[0].SaveToFile(bSaveByTeach);                                   //Steven 20240523 : Teach SUCKPARA改存成ini
    TechSuckPara[1].SaveToFile(bSaveByTeach);

    AnsiString FileName=ExtractFileName(asTeachPath);
    AnsiString FilePath=ExtractFilePath(asTeachPath);

    elTeach->SaveEditTextToFile(FilePath, FileName);                            //JerryYang 20241119 : fix AOA

    Tech.M_In_iRotateA_Backlash=atoi(edtEditRotateInBacklash->Text.c_str());    //RogerYang 20260113 : Rotator新增背隙補償
    Tech.M_Out_iRotateA_Backlash=atoi(edtEditRotateOutBacklash->Text.c_str());
    WriteIniData(asTeachPath, "MInRotate", "edtEditRotateInBacklash", Tech.M_In_iRotateA_Backlash);  //RogerYang 20260113 : Rotator新增背隙補償
    WriteIniData(asTeachPath, "MOutRotate", "edtEditRotateOutBacklash", Tech.M_Out_iRotateA_Backlash);
}
//------------------------------------------------------------------------------
void __fastcall TfTeach::spTTLResetClick(TObject *Sender)                       //Isaac 20200903 :TTL RS232通訊
{
    fMain->GetTTLState();
}
//------------------------------------------------------------------------------
void __fastcall TfTeach::cbEnableTTLButtonUseClick(TObject *Sender)             //Isaac 20200903 :TTL RS232通訊
{
    if(cbEnableTTLButtonUse->Checked==true)
        fMain->Send_Command_TTL("@WENBU10000000");
    else
        fMain->Send_Command_TTL("@WENBU00000000");
}
//------------------------------------------------------------------------------
bool TfTeach::SetIndexYPhasePosition()                                          //Isaac 20201110 : Index Y find motor phase，存入IndexY尋相找的位置
{
    AnsiString str="";
    int ret=-1;
    AnsiString sTime=Now().FormatString("hh:nn:ss"), sData="";

    ret=ShowMyMessageBox_YES_NO("Sure To Save Index_Y1_Home_Position/Index_Y2_Home_Position?", "確定要儲存Home_Position的值嗎？");//Yes:1,No:2
    if(ret==2)
    {
        return false;
    }
    else
    {
        if(bY1ModifyDistanceRef==true && bY2ModifyDistanceRef==true)
        {
            WriteIniData(asGeneralPath, "IndexDriver", "Index_Y1_Home_Position", iY1ModifyDistanceRef);
            WriteIniData(asGeneralPath, "IndexDriver", "Index_Y2_Home_Position", iY2ModifyDistanceRef);

            bY1ModifyDistanceRef=false;
            bY2ModifyDistanceRef=false;
            iIndexY1FindPhaseFrontPos=0;
            iIndexY1FindPhaseMiddlePos=0;
            iIndexY2FindPhaseMiddlePos=0;
            iIndexY2FindPhaseRearPos=0;

            str.sprintf("Save Index_Y1_Home_Position as %d and Index_Y2_Home_Position as %d", iY1ModifyDistanceRef, iY2ModifyDistanceRef);
            ShowMyMessage(str);
            lblFindPhaseNotes->Caption="Set Index Y Phase Position OK.(Y1ModifyDistanceRef and Y2ModifyDistanceRef)";
        }
        else if(bY1ModifyDistanceRef==true)
        {
            WriteIniData(asGeneralPath, "IndexDriver", "Index_Y1_Home_Position", iY1ModifyDistanceRef);

            bY1ModifyDistanceRef=false;
            iIndexY1FindPhaseFrontPos=0;
            iIndexY1FindPhaseMiddlePos=0;

            str.sprintf("Save Index_Y1_Home_Position as %d", iY1ModifyDistanceRef);
            ShowMyMessage(str);
            lblFindPhaseNotes->Caption="Set Index Y Phase Position OK.(Y1ModifyDistanceRef)";
        }
        else if(bY2ModifyDistanceRef==true)
        {
            WriteIniData(asGeneralPath, "IndexDriver", "Index_Y2_Home_Position", iY2ModifyDistanceRef);

            bY2ModifyDistanceRef=false;
            iIndexY2FindPhaseMiddlePos=0;
            iIndexY2FindPhaseRearPos=0;

            str.sprintf("Save Index_Y2_Home_Position as %d", iY2ModifyDistanceRef);
            ShowMyMessage(str);
            lblFindPhaseNotes->Caption="Set Index Y Phase Position OK.(Y2ModifyDistanceRef)";
        }
        else
        {
            ShowMyMessage("Exception!!");
            lblFindPhaseNotes->Caption="Exception!!";
        }
    }
    return true;
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::pnlExitClick(TObject *Sender)
{
    int ret=MessageDlg("Sure to exit? (確定要離開?)", mtConfirmation, TMsgDlgButtons()<<mbYes<<mbNo, 0);
    if(ret==mrNo)
        return;
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::SpeedButton1Click(TObject *Sender)
{
    int Tag;
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;
    Tag=Ptr->Tag;

    ActiveMotorIndex=TechMotorAxle[Tag]->MotorSelect;

    for(int i=0; i<TechMotorItem; i++)                                          //Steven 20120901 : 一次只有一個被按下去
    {
        if(TechMotorAxle[i]->MotorSelect!=ActiveMotorIndex)
            TechMotorAxle[i]->SelButton->Down=false;
    }

    UpdateMotorTeachMonitor(ActiveMotorIndex);
}
//---------------------------------------------------------------------------
//KenHsieh 20210813 : add CCD AUTO ALIGNMENT
//==>
void __fastcall TfTeach::sbAutoCalibrationInArmCCDResolutionClick(
      TObject *Sender)
{
    int iIndex[10]={MInArmZA, MInArmZB, MInArmZC, MInArmZD, MInArmZE, MInArmZF, MInArmZG, MInArmZH, MInArmX, MInArmY};

    if(fAutoAlignment->ClientSocket1->Active==true)                             //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
    {
        for(int i=0; i<10; i++)
        {
            ActiveMotorIndex=iIndex[i];
            ScrollBar1->Position=10;
            UpdateMotorTeachMonitor(ActiveMotorIndex);
        }

        Teach_AutoAlignmentUnit=1;                                              //JerryYang 20241119 : fix AOA
        InitAutoAlignmentTask();                                                //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
        InitInArmXYZHome();
        InitInArmCCDAlignmentTeachUnitProcess();
    }
    else
    {
        ShowMyMessage("Auto Alignment CCD1 Not Connected");
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::sbAutoCalibrationOutArmCCDResolutionClick(
      TObject *Sender)
{
    int iIndex[10]={MOutArmZA, MOutArmZB, MOutArmZC, MOutArmZD, MOutArmZE, MOutArmZF, MOutArmZG, MOutArmZH, MOutArmX, MOutArmY};

    if(fAutoAlignment->ClientSocket2->Active==true)                             //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
    {
        for(int i=0; i<10; i++)
        {
            ActiveMotorIndex=iIndex[i];
            ScrollBar1->Position=10;
            UpdateMotorTeachMonitor(ActiveMotorIndex);
        }

        Teach_AutoAlignmentUnit=2;                                              //JerryYang 20241119 : fix AOA
        InitAutoAlignmentTask();                                                //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
        InitOutArmXYZHome();
        InitOutArmCCDAlignmentTeachUnitProcess();
    }
    else
    {
        ShowMyMessage("Auto Alignment CCD2 Not Connected");
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::sbAutoCalibrationInArmCCDPickerDisClick(
      TObject *Sender)
{
    int iIndex[10]={MInArmZA, MInArmZB, MInArmZC, MInArmZD, MInArmZE, MInArmZF, MInArmZG, MInArmZH, MInArmX, MInArmY};

    for(int i=0; i<10; i++)
    {
        ActiveMotorIndex=iIndex[i];
        ScrollBar1->Position=10;
        UpdateMotorTeachMonitor(ActiveMotorIndex);
    }

    bInArmXPitch_40mm=false;                                                    //KenHsieh 20211023 : 新增X Pitch 40mm teach
    ActiveMotorIndex=MInArmX;
    Teach_AutoAlignmentUnit=3;                                                  //JerryYang 20241119 : fix AOA
    InitAutoAlignmentTask();                                                    //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
    InitInArmXYZHome();
    InitInArmPickerXYAndPitchAutoAlignment();
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::sbAutoCalibrationOutArmCCDPickerDisClick(
      TObject *Sender)
{
    int iIndex[10]={MOutArmZA, MOutArmZB, MOutArmZC, MOutArmZD, MOutArmZE, MOutArmZF, MOutArmZG, MOutArmZH, MOutArmX, MOutArmY};

    for(int i=0; i<10; i++)
    {
        ActiveMotorIndex=iIndex[i];
        ScrollBar1->Position=10;
        UpdateMotorTeachMonitor(ActiveMotorIndex);
    }

    bOutArmXPitch_40mm=false;                                                   //KenHsieh 20211023 : 新增X Pitch 40mm teach
    ActiveMotorIndex=MOutArmX;
    Teach_AutoAlignmentUnit=4;                                                  //JerryYang 20241119 : fix AOA
    InitAutoAlignmentTask();                                                    //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
    InitOutArmXYZHome();
    InitOutArmPickerXYAndPitchAutoAlignment();
}
//---------------------------------------------------------------------------
//<==
//KenHsieh 20210813 : add CCD AUTO ALIGNMENT
//---------------------------------------------------------------------------
void __fastcall TfTeach::SetComputeInSh2Click(TObject *Sender)
{
    int iNowInarmX=MOT[MInArmX].ReadPos();                                      //JimmyChiu 20220908 add Pickup Error Placement
    int iNowInarmY=MOT[MInArmY].ReadPos();
    int iSh2X=(setEditInSht2X->Text=="")?0:atoi(setEditInSht2X->Text.c_str());
    int iSh2Y=(setEditInSht2Y->Text=="")?0:atoi(setEditInSht2Y->Text.c_str());
    int iOffsetX=iNowInarmX-iSh2X;
    int iOffsetY=iNowInarmY-iSh2Y;
    setEditInarmPlacementXOffsetByBasicSuck->Text=IntToStr(iOffsetX);
    setEditInarmPlacementYOffsetByBasicSuck->Text=IntToStr(iOffsetY);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::edtMoveToClick(TObject *Sender)
{
    if(ActiveMotorIndex==-1)
        return;

    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, MOT[ActiveMotorIndex].Motor->PSoftLimitP, MOT[ActiveMotorIndex].Motor->PSoftLimitN);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::setEditZ1AClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 100, -3000);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::setEditInXPitch40Click(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, MOT[MInArmPitch].Motor->PSoftLimitP, MOT[MInArmPitch].Motor->PSoftLimitN);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::setEditInY15Click(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, MOT[MInArmPitchY].Motor->PSoftLimitP, MOT[MInArmPitchY].Motor->PSoftLimitN);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::setEditOutXPitch40Click(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, MOT[MOutArmPitch].Motor->PSoftLimitP, MOT[MOutArmPitch].Motor->PSoftLimitN);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::setEditOutY15Click(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, MOT[MOutArmPitchY].Motor->PSoftLimitP, MOT[MOutArmPitchY].Motor->PSoftLimitN);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::setEditIndex1ToSocketYClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, MOT[MTestY1].Motor->PSoftLimitP, MOT[MTestY1].Motor->PSoftLimitN);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::setEditIndex2ToSocketYClick(TObject *Sender)
{
    if(USE_INDEX_ARM_AXES==IndexArm_4_Axis)                                     //JimmyChiu 20220708 : add Index Arm Axis
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, MOT[MTestY2].Motor->PSoftLimitP, MOT[MTestY2].Motor->PSoftLimitN);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::setEditIndex1ToSht1ZClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 0, fIndexDownPos*100);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::setEditInSht1LeftClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, MOT[MInShuttle1].Motor->PSoftLimitP, MOT[MInShuttle1].Motor->PSoftLimitN);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::setEditInSht2LeftClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, MOT[MInShuttle2].Motor->PSoftLimitP, MOT[MInShuttle2].Motor->PSoftLimitN);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::setEditInXClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, MOT[MInArmX].Motor->PSoftLimitP, MOT[MInArmX].Motor->PSoftLimitN);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::setEditInYClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, MOT[MInArmY].Motor->PSoftLimitP, MOT[MInArmY].Motor->PSoftLimitN);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::setEditRotateAClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, MOT[MInRotateKit].Motor->PSoftLimitP, MOT[MInRotateKit].Motor->PSoftLimitN);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::edtEditRotateInBacklashClick(TObject *Sender)           //RogerYang 20260113 : Rotator新增背隙補償
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 500, 0);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::edShtCheckRangeClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 500, 2500);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::SetEditPickLoaderClick(TObject *Sender)
{
    if(USE_PICKER_COUNT==ep1Picker)
        fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -500, -7000);
    else
        fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, -500, -3000);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::setEditInZSafeHeightClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 0, 200);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::setEditOutXClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, MOT[MOutArmX].Motor->PSoftLimitP, MOT[MOutArmX].Motor->PSoftLimitN);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::setEditOutYClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, MOT[MOutArmY].Motor->PSoftLimitP, MOT[MOutArmY].Motor->PSoftLimitN);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::setFix3LClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, MOT[MFix3Full].Motor->PSoftLimitP, MOT[MFix3Full].Motor->PSoftLimitN);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::setEditTrayXClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, MOT[MTrayX].Motor->PSoftLimitP, MOT[MTrayX].Motor->PSoftLimitN);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::setLoaderZClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, MOT[MLoaderZ].Motor->PSoftLimitP, MOT[MLoaderZ].Motor->PSoftLimitN);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::setEditHingeRotateLoaderClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, MOT[MLoadHingeR].Motor->PSoftLimitP, MOT[MLoadHingeR].Motor->PSoftLimitN);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::setEditBuffer1XClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, MOT[MCasArmX].Motor->PSoftLimitP, MOT[MCasArmX].Motor->PSoftLimitN);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::setEditBuffer1ZClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, MOT[MCasArmZ].Motor->PSoftLimitP, MOT[MCasArmZ].Motor->PSoftLimitN);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::setEditLoadPortZClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, MOT[MCaselevatorZ].Motor->PSoftLimitP, MOT[MCaselevatorZ].Motor->PSoftLimitN);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::setEditTrayBracketSaftZClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, MOT[MTrayBracketZ].Motor->PSoftLimitP, MOT[MTrayBracketZ].Motor->PSoftLimitN);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::setEditUnloadPortBufferZClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, MOT[MUnloadRobotZ].Motor->PSoftLimitP, MOT[MUnloadRobotZ].Motor->PSoftLimitN);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::setEditStackedLoaderXClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, MOT[MStackedTrayX].Motor->PSoftLimitP, MOT[MStackedTrayX].Motor->PSoftLimitN);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::setEditStackedLoaderZClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, MOT[MStackedTrayZ].Motor->PSoftLimitP, MOT[MStackedTrayZ].Motor->PSoftLimitN);
}
//---------------------------------------------------------------------------
void TfTeach::LockAllButton(bool bLock)                                         //Steven 20230815 : Add for MotorTest
{
    PageControl2->Enabled   =!bLock;
    btnMoveN->Enabled       =!bLock;
    btnMoveP->Enabled       =!bLock;
    btnHome->Enabled        =!bLock;
    btnMoveTo->Enabled      =!bLock;
    pnlStop->Color=(bLock)?clYellow:(TColor)0x00DFD9CC;
}
//---------------------------------------------------------------------------
void TfTeach::AllBtnUp()                                                        //Steven 20230815 : Add for MotorTest
{
    btnJogN->Down    =false;
    btnJogP->Down    =false;
    btnHome->Down    =false;
    btnMoveP->Down   =false;
    btnMoveN->Down   =false;
    btnMoveTo->Down  =false;
    if(PtrGoBtn020!=NULL)
        PtrGoBtn020->Down=false;
    if(PtrGoBtn140!=NULL)
        PtrGoBtn140->Down=false;
}
//---------------------------------------------------------------------------
TQPF_Timer MoveDelay;
bool VerifyMotorAction()                                                        //Steven 20230728 : 確認馬達有沒有在動
{
    bool bDoorClosed=false, bDelayOk=false;
    bool bHasMoveing=false;
    static bool bNeedStopMot=false;
    AnsiString asString;

    if(fTeach->fShow==true || fMotorTest->fShow==true)
    {
        for(int i=0; i<TOTAL_MOTOR; i++)
        {
            if(bDoorClosed==false)
            {
                if(fTeach->btnHome->Down ||
                   fMotorTest->btnHome->Down)                                   //JerryYang 20230815 : add
                {
                    bHasMoveing=true;
                    asString="*Lock by Homeing";
                }
                else if(INDEX_MOTION_CARD==0 &&                                 //Steven 20210623 : Index使用Galil
                        (MOT[i].Mot_Name==MTestY1 || MOT[i].Mot_Name==MTestZ1 ||
                         MOT[i].Mot_Name==MTestZ2 || MOT[i].Mot_Name==MTestY2))
                {
                    if(MOT[i].Motor!=NULL           &&
                       MOT[i].Motor->Enable==true)
                    {
                        MOT[i].ScanMotorStatus();
                        if(MOT[i].Led[iInposLed])
                        {
                            bHasMoveing=true;
                            asString.sprintf("*Lock by %s moveing", MOT[i].NumberAlias);
                        }
                    }
                }
                else
                {
                    if(MOT[i].Motor!=NULL           &&
                       MOT[i].Motor->Enable==true   &&
                       MOT[i].Motor->MotionDone()==false)
                    {
                        bHasMoveing=true;
                        asString.sprintf("*Lock by %s moveing", MOT[i].NumberAlias);
                    }
                }
            }
        }

        if(bHasMoveing)
        {
            bNeedStopMot=true;                                                  //For switch case type motor move such as Home()
            fTeach->LockAllButton(true);
            fMotorTest->LockAllButton(true);
            MoveDelay.SetSecAndOn(2);                                           //For non-return type move, after arrive and wait 2 sec, then reset flag.
            fTeach->labLock->Visible=true;
            fTeach->labLock->Caption=asString;
        }
        else
        {
            fTeach->labLock->Visible=false;
        }

        if(bNeedStopMot==true)
        {
            bDoorClosed=CheckSafeDoorIsClosed();
            bDelayOk   =MoveDelay.Off();

            if(bDoorClosed==false ||                                            //Safe door is opened
               bDelayOk==true)                                                  //Delay OK
            {
                StopAllMotor();
                bNeedStopMot=false;
                fTeach->LockAllButton(false);
                fMotorTest->LockAllButton(false);
                fTeach->AllBtnUp();                                             //Steven 20230815 : Add for MotorTest
                fMotorTest->AllBtnUp();                                         //Steven 20230815 : Add for MotorTest
                fMotorTest->bSingleHome=false;
            }
        }
    }
    return bDoorClosed;
}
//---------------------------------------------------------------------------
bool TfTeach::CheckShuttleCanMove(int iMotorIndex)                              //Ztex 2023.06.02 Add Check_SHUTTLE_FLOODGATE_Staste
{
    int iActiveMotorIndex = iMotorIndex;
    bool bReturnStatus = false;
    switch(iActiveMotorIndex)
    {
        case 11:  //MInShuttle1
        {
            if(SHUTTLE_FLOODGATE==1)
            {
               if(Cylinder[C_Shuttle1Floodgate].OffSensor()==false  &&
                  Cylinder[C_OutShuttle1Floodgate].OffSensor()==false)          //Ifor 20240620 add:Out Shuttle Floodgate
               {
                    ShowErrorMessage("WAR16435", K_RETRY, MMSystem);
                    bReturnStatus = false;
               }
               else
               {
                  bReturnStatus = true;
               }
            }
            else
            {
                bReturnStatus = true;
            }
        }
        break;
        //========================================
        case 12:  //MInShuttle2
        {
            if(SHUTTLE_FLOODGATE==1)
            {
                 if(Cylinder[C_Shuttle2Floodgate].OffSensor()==false)
                 {
                        ShowErrorMessage("WAR16436", K_RETRY, MMSystem);
                        bReturnStatus = false;
                 }
                 else
                 {
                    bReturnStatus = true;
                 }
            }
            else
            {
                bReturnStatus = true;
            }
        }
        break;
        //========================================
        default:
        {
            bReturnStatus = true;
        }
        break;
    }
    return bReturnStatus;
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnInClosePitchClick(TObject *Sender)
{
    long lPosition[4]={MOT[MInArmPitch  ].Motor->ReadPos(),
                       MOT[MInArmPitchX2].Motor->ReadPos(),
                       MOT[MInArmPitchX3].Motor->ReadPos(),
                       MOT[MInArmPitchX4].Motor->ReadPos()};

    long lTarget[4]={atoi(setEditInXPitch40->Text.c_str()),
                     atoi(setEditInX240->Text.c_str()),
                     atoi(setEditInX340->Text.c_str()),
                     atoi(setEditInX440->Text.c_str())};

    BYTE bDevNo[4]={MOT[MInArmPitch  ].Motor->iPortID,
                    MOT[MInArmPitchX2].Motor->iPortID,
                    MOT[MInArmPitchX3].Motor->iPortID,
                    MOT[MInArmPitchX4].Motor->iPortID};

    int iDevPos[4];
    iDevPos[0]=lTarget[0]-lPosition[0];
    iDevPos[1]=lTarget[1]-lPosition[1];
    iDevPos[2]=lTarget[2]-lPosition[2];
    iDevPos[3]=lTarget[3]-lPosition[3];
    MOT[MInArmPitch  ].GetRealPos(&iDevPos[0]);
    MOT[MInArmPitchX2].GetRealPos(&iDevPos[1]);
    MOT[MInArmPitchX3].GetRealPos(&iDevPos[2]);
    MOT[MInArmPitchX4].GetRealPos(&iDevPos[3]);
    long lDevPos[4]={iDevPos[0], iDevPos[1], iDevPos[2], iDevPos[3]};

    MOT[MInArmPitch].Motor->SetGroup(1, 4, bDevNo);
    MOT[MInArmPitch].Motor->LineNMove(bDevNo, lDevPos, 4);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnInOpenPitchClick(TObject *Sender)
{
    long lPosition[4]={MOT[MInArmPitch  ].Motor->ReadPos(),
                       MOT[MInArmPitchX2].Motor->ReadPos(),
                       MOT[MInArmPitchX3].Motor->ReadPos(),
                       MOT[MInArmPitchX4].Motor->ReadPos()};

    long lTarget[4]={atoi(setEditInXPitch120->Text.c_str()),
                     atoi(setEditInX2120->Text.c_str()),
                     atoi(setEditInX3120->Text.c_str()),
                     atoi(setEditInX4120->Text.c_str())};

    BYTE bDevNo[4]={MOT[MInArmPitch  ].Motor->iPortID,
                    MOT[MInArmPitchX2].Motor->iPortID,
                    MOT[MInArmPitchX3].Motor->iPortID,
                    MOT[MInArmPitchX4].Motor->iPortID};

    int iDevPos[4];
    iDevPos[0]=lTarget[0]-lPosition[0];
    iDevPos[1]=lTarget[1]-lPosition[1];
    iDevPos[2]=lTarget[2]-lPosition[2];
    iDevPos[3]=lTarget[3]-lPosition[3];
    MOT[MInArmPitch  ].GetRealPos(&iDevPos[0]);
    MOT[MInArmPitchX2].GetRealPos(&iDevPos[1]);
    MOT[MInArmPitchX3].GetRealPos(&iDevPos[2]);
    MOT[MInArmPitchX4].GetRealPos(&iDevPos[3]);
    long lDevPos[4]={iDevPos[0], iDevPos[1], iDevPos[2], iDevPos[3]};

    MOT[MInArmPitch].Motor->SetGroup(1, 4, bDevNo);
    MOT[MInArmPitch].Motor->LineNMove(bDevNo, lDevPos, 4);
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnPitchTestClick(TObject *Sender)
{
    if(pgc_UsePitchX->Visible==false)
        pgc_UsePitchX->Visible=true;
    else
        pgc_UsePitchX->Visible=false;
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btn_InPitchX_13Click(TObject *Sender)
{
    int Tag;
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;
    Tag=Ptr->Tag;
    long lPosition[4]={MOT[MInArmPitch  ].Motor->ReadPos(),
                       MOT[MInArmPitchX2].Motor->ReadPos(),
                       MOT[MInArmPitchX3].Motor->ReadPos(),
                       MOT[MInArmPitchX4].Motor->ReadPos()};

    long lTarget[4]={atoi(edt_InPitchX_13->Text.c_str()),
                     atoi(edt_InPitchX_24->Text.c_str()),
                     atoi(edt_InPitchX_13->Text.c_str()),
                     atoi(edt_InPitchX_24->Text.c_str())};

    BYTE bDevNo_13[2]={MOT[MInArmPitch  ].Motor->iPortID,
                       MOT[MInArmPitchX3].Motor->iPortID
                      };
    BYTE bDevNo_24[2]={MOT[MInArmPitchX2 ].Motor->iPortID,
                       MOT[MInArmPitchX4 ].Motor->iPortID
                      };

    int iDevPos[4];
    iDevPos[0]=lTarget[0]-lPosition[0];
    iDevPos[1]=lTarget[1]-lPosition[1];
    iDevPos[2]=lTarget[2]-lPosition[2];
    iDevPos[3]=lTarget[3]-lPosition[3];
    MOT[MInArmPitch  ].GetRealPos(&iDevPos[0]);
    MOT[MInArmPitchX2].GetRealPos(&iDevPos[1]);
    MOT[MInArmPitchX3].GetRealPos(&iDevPos[2]);
    MOT[MInArmPitchX4].GetRealPos(&iDevPos[3]);
    long lDevPos_13[2]={iDevPos[0],iDevPos[2]};
    long lDevPos_24[2]={iDevPos[1],iDevPos[3]};

    if(Tag==0)
    {
        MOT[MInArmPitch].Motor->SetGroup(1, 2, bDevNo_13);
        MOT[MInArmPitch].Motor->LineNMove(bDevNo_13, lDevPos_13, 2);
    }
    else
    {
        MOT[MInArmPitch].Motor->SetGroup(1, 2, bDevNo_24);
        MOT[MInArmPitch].Motor->LineNMove(bDevNo_24, lDevPos_24, 2);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btn_InPX_13HomeClick(TObject *Sender)
{
    int Tag;
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;
    Tag=Ptr->Tag;
    TButton *btn[4]={btn_InPX_13Home,btn_InPX_24Home,btn_OutPX_13Home,btn_OutPX_24Home};

    if(Tag==0)
    {
        PitchX_Home_Index=0;
        InitProcessSingleMotorTask(MInArmPitch);
        InitProcessSingleMotorTask(MInArmPitchX3);
        fHome->iHomeStep=1;
    }
    else if(Tag==1)
    {
        PitchX_Home_Index=1;
        InitProcessSingleMotorTask(MInArmPitchX2);
        InitProcessSingleMotorTask(MInArmPitchX4);
        fHome->iHomeStep=1;
    }
    else if(Tag==2)
    {
        PitchX_Home_Index=2;
        InitProcessSingleMotorTask(MOutArmPitch);
        InitProcessSingleMotorTask(MOutArmPitchX3);
        fHome->iHomeStep=1;
    }
    else if(Tag==3)
    {
        PitchX_Home_Index=3;
        InitProcessSingleMotorTask(MOutArmPitchX2);
        InitProcessSingleMotorTask(MOutArmPitchX4);
        fHome->iHomeStep=1;
    }
    else
    {
        PitchX_Home=false;
        PitchX_Home_Index=99;
        return;
    }
    PitchX_Home=true;
    btn[Tag]->Enabled=false;
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btn_OutPitch_13Click(TObject *Sender)
{
    int Tag;
    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;
    Tag=Ptr->Tag;
    long lPosition[4]={MOT[MOutArmPitch  ].Motor->ReadPos(),
                       MOT[MOutArmPitchX2].Motor->ReadPos(),
                       MOT[MOutArmPitchX3].Motor->ReadPos(),
                       MOT[MOutArmPitchX4].Motor->ReadPos()};

    long lTarget[4]={atoi(edt_OutPitch_13->Text.c_str()),
                     atoi(edt_OutPitch_24->Text.c_str()),
                     atoi(edt_OutPitch_13->Text.c_str()),
                     atoi(edt_OutPitch_24->Text.c_str())};

    BYTE bDevNo_13[2]={MOT[MOutArmPitch  ].Motor->iPortID,
                       MOT[MOutArmPitchX3].Motor->iPortID
                      };
    BYTE bDevNo_24[2]={MOT[MOutArmPitchX2 ].Motor->iPortID,
                       MOT[MOutArmPitchX4 ].Motor->iPortID
                      };

    int iDevPos[4];
    iDevPos[0]=lTarget[0]-lPosition[0];
    iDevPos[1]=lTarget[1]-lPosition[1];
    iDevPos[2]=lTarget[2]-lPosition[2];
    iDevPos[3]=lTarget[3]-lPosition[3];
    MOT[MOutArmPitch  ].GetRealPos(&iDevPos[0]);
    MOT[MOutArmPitchX2].GetRealPos(&iDevPos[1]);
    MOT[MOutArmPitchX3].GetRealPos(&iDevPos[2]);
    MOT[MOutArmPitchX4].GetRealPos(&iDevPos[3]);
    long lDevPos_13[2]={iDevPos[0],iDevPos[2]};
    long lDevPos_24[2]={iDevPos[1],iDevPos[3]};

    if(Tag==0)
    {
        MOT[MOutArmPitch].Motor->SetGroup(1, 2, bDevNo_13);
        MOT[MOutArmPitch].Motor->LineNMove(bDevNo_13, lDevPos_13, 2);
    }
    else
    {
        MOT[MOutArmPitch].Motor->SetGroup(1, 2, bDevNo_24);
        MOT[MOutArmPitch].Motor->LineNMove(bDevNo_24, lDevPos_24, 2);
    }
}
//---------------------------------------------------------------------------
void TfTeach::DoPitch_Home()
{
    static bool bFlag[2]={false,false};
    TButton *btn[4]={btn_InPX_13Home, btn_InPX_24Home, btn_OutPX_13Home, btn_OutPX_24Home};
    if(PitchX_Home==true && PitchX_Home_Index!=99)
    {
        if(PitchX_Home_Index==0)
        {
            if(ProcessSingleMotorHome(MInArmPitch))
            {
                bFlag[0]=true;
                edt_InPX1_Home_Pos->Text=MOT[MInArmPitch].Motor->LastHomePos;
                MOT[MInArmPitch].SetSpeed(1);
            }
            if(ProcessSingleMotorHome(MInArmPitchX3))
            {
                bFlag[1]=true;
                edt_InPX3_Home_Pos->Text=MOT[MInArmPitchX3].Motor->LastHomePos;
                MOT[MInArmPitchX3].SetSpeed(1);
            }
        }
        else if(PitchX_Home_Index==1)
        {
            if(ProcessSingleMotorHome(MInArmPitchX2))
            {
                bFlag[0]=true;
                edt_InPX2_Home_Pos->Text=MOT[MInArmPitchX2].Motor->LastHomePos;
                MOT[MInArmPitchX2].SetSpeed(1);
            }
            if(ProcessSingleMotorHome(MInArmPitchX4))
            {
                bFlag[1]=true;
                edt_InPX4_Home_Pos->Text=MOT[MInArmPitchX4].Motor->LastHomePos;
                MOT[MInArmPitchX4].SetSpeed(1);
            }
        }
        else if(PitchX_Home_Index==2)
        {
            if(ProcessSingleMotorHome(MOutArmPitch))
            {
                bFlag[0]=true;
                edt_OutPX1_Home_Pos->Text=MOT[MOutArmPitch].Motor->LastHomePos;
                MOT[MOutArmPitch].SetSpeed(1);
            }
            if(ProcessSingleMotorHome(MOutArmPitchX3))
            {
                bFlag[1]=true;
                edt_OutPX3_Home_Pos->Text=MOT[MOutArmPitchX3].Motor->LastHomePos;
                MOT[MOutArmPitchX3].SetSpeed(1);
            }
        }
        else if(PitchX_Home_Index==3)
        {
            if(ProcessSingleMotorHome(MOutArmPitchX2))
            {
                bFlag[0]=true;
                edt_OutPX2_Home_Pos->Text=MOT[MOutArmPitchX2].Motor->LastHomePos;
                MOT[MOutArmPitchX2].SetSpeed(1);
            }
            if(ProcessSingleMotorHome(MOutArmPitchX4))
            {
                bFlag[1]=true;
                edt_OutPX4_Home_Pos->Text=MOT[MOutArmPitchX4].Motor->LastHomePos;
                MOT[MOutArmPitchX4].SetSpeed(1);
            }
        }

        if(bFlag[0]&&bFlag[1])
        {
            PitchX_Home=false;
            btn[PitchX_Home_Index]->Enabled=true;
            PitchX_Home_Index=99;
            bFlag[0]=false;
            bFlag[1]=false;
        }
    }
    else
    {
        return;
    }
}
//---------------------------------------------------------------------------
void TfTeach:: Set_Pitch_SetGroup()                                             //Ztex 2024.04.02 Set Pitch SetGroup
{
    if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||
       USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)
    {
        BYTE bDevNo_In[4]= {MOT[MInArmPitch  ].Motor->iPortID,
                            MOT[MInArmPitchX2].Motor->iPortID,
                            MOT[MInArmPitchX3].Motor->iPortID,
                            MOT[MInArmPitchX4].Motor->iPortID};
        BYTE bDevNo_Put[4]={MOT[MOutArmPitch  ].Motor->iPortID,
                            MOT[MOutArmPitchX2].Motor->iPortID,
                            MOT[MOutArmPitchX3].Motor->iPortID,
                            MOT[MOutArmPitchX4].Motor->iPortID};

        MOT[MInArmPitch].Motor->SetGroup(1, 4, bDevNo_In);
        MOT[MOutArmPitch].Motor->SetGroup(2, 4, bDevNo_Put);
    }
}
//Ztex 2023.12.15 Add Pitch X Home Twice <==
//---------------------------------------------------------------------------
int iDoPitchLoop_Task=1;
int iLoop_Pitch_Count=0;
TQPF_Timer TeachLoopWait;
void __fastcall TfTeach::btn_PitchX1_3_LoopClick(TObject *Sender)
{
    static bool bUse=false;
    if(bUse==false)
    {
        iDoPitchLoop_Task=1;
        tmr_PitchLoop->Enabled=true;
        bUse=true;
        iLoop_Pitch_Count=0;
    }
    else
    {
        tmr_PitchLoop->Enabled=false;
        bUse=false;
    }
}
//---------------------------------------------------------------------------
void TfTeach::DoPitchLoopMove()                                                 //Ztex 2024.06.18 Add Pitch Loop
{
    int &Task=iDoPitchLoop_Task;

    BYTE bDevNo[4]={MOT[MInArmPitch  ].Motor->iPortID,
                    MOT[MInArmPitchX2].Motor->iPortID,
                    MOT[MInArmPitchX3].Motor->iPortID,
                    MOT[MInArmPitchX4].Motor->iPortID};

    int iDevPos[4];
    long lDevPos[4];
    int Wait=atoi(edt_LoopPitch_DelayTime->Text.c_str());
    long lPosition[4];

    long lTarget_1[4]={atoi(edt_Loop_1_XPo1->Text.c_str()),
                        atoi(edt_Loop_1_XPo1->Text.c_str()),
                        atoi(edt_Loop_3_XPo1->Text.c_str()),
                        atoi(edt_Loop_3_XPo1->Text.c_str())};

    long lTarget_2[4]={atoi(edt_Loop_1_XPo2->Text.c_str()),
                        atoi(edt_Loop_1_XPo2->Text.c_str()),
                        atoi(edt_Loop_3_XPo2->Text.c_str()),
                        atoi(edt_Loop_3_XPo2->Text.c_str())};

    switch(Task)
    {
        case 1:
            lPosition[0]=MOT[MInArmPitch  ].Motor->ReadPos();
            lPosition[1]=MOT[MInArmPitchX2  ].Motor->ReadPos();
            lPosition[2]=MOT[MInArmPitchX3  ].Motor->ReadPos();
            lPosition[3]=MOT[MInArmPitchX4  ].Motor->ReadPos();

            iDevPos[0]=lTarget_1[0]-lPosition[0];
            iDevPos[1]=lTarget_1[1]-lPosition[1];
            iDevPos[2]=lTarget_1[2]-lPosition[2];
            iDevPos[3]=lTarget_1[3]-lPosition[3];
            MOT[MInArmPitch  ].GetRealPos(&iDevPos[0]);
            MOT[MInArmPitchX2].GetRealPos(&iDevPos[1]);
            MOT[MInArmPitchX3].GetRealPos(&iDevPos[2]);
            MOT[MInArmPitchX4].GetRealPos(&iDevPos[3]);

            lDevPos[0]=iDevPos[0];
            lDevPos[1]=iDevPos[1];
            lDevPos[2]=iDevPos[2];
            lDevPos[3]=iDevPos[3];

            MOT[MInArmPitch].Motor->LineNMove(bDevNo, lDevPos, 4);

            Task=100;
            break;
        case 100:
            if(MOT[MInArmPitch].Motor->MotionDone()==true)
            {
                TeachLoopWait.Set0_1SecAndOn(Wait);
                Task=200;
            }
            break;
        case 200:
            if(TeachLoopWait.Off())
                Task=1000;
            break;
        case 1000:
            lPosition[0]=MOT[MInArmPitch  ].Motor->ReadPos();
            lPosition[1]=MOT[MInArmPitchX2  ].Motor->ReadPos();
            lPosition[2]=MOT[MInArmPitchX3  ].Motor->ReadPos();
            lPosition[3]=MOT[MInArmPitchX4  ].Motor->ReadPos();

            iDevPos[0]=lTarget_2[0]-lPosition[0];
            iDevPos[1]=lTarget_2[1]-lPosition[1];
            iDevPos[2]=lTarget_2[2]-lPosition[2];
            iDevPos[3]=lTarget_2[3]-lPosition[3];
            MOT[MInArmPitch  ].GetRealPos(&iDevPos[0]);
            MOT[MInArmPitchX2].GetRealPos(&iDevPos[1]);
            MOT[MInArmPitchX3].GetRealPos(&iDevPos[2]);
            MOT[MInArmPitchX4].GetRealPos(&iDevPos[3]);

            lDevPos[0]=iDevPos[0];
            lDevPos[1]=iDevPos[1];
            lDevPos[2]=iDevPos[2];
            lDevPos[3]=iDevPos[3];

            MOT[MInArmPitch].Motor->LineNMove(bDevNo, lDevPos, 4);
            iLoop_Pitch_Count++;
            lbl_LoopCount->Caption=AnsiString(iLoop_Pitch_Count);
            Task=1100;
            break;
        case 1100:
            if(MOT[MInArmPitch].Motor->MotionDone()==true)
            {
                Task=1;
            }
            break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::tmr_PitchLoopTimer(TObject *Sender)                    //Ztex 2024.06.18 Add Pitch Loop
{
    DoPitchLoopMove();
}
//---------------------------------------------------------------------------
void TfTeach::TriTemp_Teach()                                                   //Ztex 2024.08.18 Add TriTemp_Teach Function
{
    if(Tri_Temp_Machine==0)
        return;

    pgc_UsePitchX->Visible=true;

    for(int i=0; i<TOTAL_MOTOR; i++)
    {
        if(MOT[i].Motor->Enable==true)
            SetMotorScaleSpeed(i,10);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnSetAllInArmZ_MoveClick(TObject *Sender)
{
    static bool b = false;
    int iMot;
    for(int i=0; i<InArmSuck.iMotRow; i++)
    {
        for(int j=0; j<InArmSuck.iMotCol; j++)
        {
            iMot=InArmSuck.Suck[i][j].iMotNo;
            if(i<InArmSuck.iPickRow && j<InArmSuck.iPickCol)
            {
              if(b==false)
                  MOT[iMot].MotorMove(Prod.ZInArm_Tray_Pick[i][j]);
              else
                  MOT[iMot].MotorMove(0);
            }
        }
    }

    if(b==false)
      b=true;
    else
      b=false;
}
//---------------------------------------------------------------------------
void __fastcall TfTeach::btnSetAllOutArmZ_MoveClick(TObject *Sender)
{
    static bool b = false;
    int iMot;
    for(int i=0; i<OutArmSuck.iMotRow; i++)
    {
        for(int j=0; j<OutArmSuck.iMotCol; j++)
        {
            iMot=OutArmSuck.Suck[i][j].iMotNo;
            if(i<OutArmSuck.iPickRow && j<OutArmSuck.iPickCol)
            {
              if(b==false)
                  MOT[iMot].MotorMove(Prod.ZOutArm_Auto_Place[0][i][j]);
              else
                  MOT[iMot].MotorMove(0);
            }
        }
    }

    if(b==false)
      b=true;
    else
      b=false;
}
//---------------------------------------------------------------------------
