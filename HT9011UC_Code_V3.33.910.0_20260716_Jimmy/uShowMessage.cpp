#include "MachineDefine.h"
#pragma hdrstop

#include "uShowMessage.h"

#include "cMydef.h"
#include "common.h"
#include "cprod.h"
#include "cTemperFrom.h"
#include "cSortCT.h"
#include "cTestCategory.h"
#include "cContactCT.h"
#include "cShowBinSelect.h"
#include "uLotInfo.h"
#include "main.h"
#include "FTPClient.h"
#include "cContact.h"
#include "cSetUp.h"
#include "uHGemEquipment.h"
#include "SCK_ART.h"

#include "BarcodeReader.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfShowMessage *fShowMessage;
extern void LogSoftwareOnTime(AnsiString Flag);
extern void LogSoftwareOffTime(AnsiString Flag);
//---------------------------------------------------------------------------
__fastcall TfShowMessage::TfShowMessage(TComponent* Owner) : TForm(Owner)
{
    pCanvas = new TCanvas;
    hDC=GetDC(sgdSpeedView->Handle);
#ifdef SOFT_SIMULTE
    lblIndexCycleTime->Caption="Test 1";
    lblTestTime->Caption="Test 2";
    fShowMessage->Top=0;
    fShowMessage->Left=0;
    fShowMessage->Width=1280;
    fShowMessage->Height=1024;
#else
    fShowMessage->Align=alClient;
#endif
}
//---------------------------------------------------------------------------
void __fastcall TfShowMessage::ShowSpeed(bool tag)                              //Steven 20090720
{
    bool bFlag=false;

    if(ArmSpeed[InArm].bAutoSpeed==false)
    {
        if(ArmSpeed[IndexArm].iBodySP==ArmSpeed[InArm].iBodySP &&
           ArmSpeed[IndexArm].iBodySP==ArmSpeed[OutArm].iBodySP &&
           ArmSpeed[IndexArm].iBodySP==ArmSpeed[TrayArm].iBodySP &&
           ArmSpeed[IndexArm].iBodySP==SHSpeed.iSH1Sp &&
           ArmSpeed[IndexArm].iBodySP==SHSpeed.iSH2Sp &&
           ArmSpeed[IndexArm].iACDCBodySP==ArmSpeed[InArm].iACDCBodySP &&
           ArmSpeed[IndexArm].iACDCBodySP==ArmSpeed[OutArm].iACDCBodySP &&
           ArmSpeed[IndexArm].iACDCBodySP==ArmSpeed[TrayArm].iACDCBodySP &&
           ArmSpeed[IndexArm].iACDCBodySP==SHSpeed.iSH1ACDCSp &&
           ArmSpeed[IndexArm].iACDCBodySP==SHSpeed.iSH2ACDCSp)                  //¦pªG¥þ³¡ªº³t«×©M¥[³t«×³£¬Ûµ¥
        {
            bFlag=true;
        }
        else
        {
            bFlag=false;
        }
    }
    else
    {
        if(ArmSpeed[IndexArm].iBodySP==AutoArmSpeed[InArm].iBodySP &&
           ArmSpeed[IndexArm].iBodySP==AutoArmSpeed[OutArm].iBodySP &&
           ArmSpeed[IndexArm].iBodySP==ArmSpeed[TrayArm].iBodySP &&
           ArmSpeed[IndexArm].iBodySP==SHSpeed.iSH1Sp &&
           ArmSpeed[IndexArm].iBodySP==SHSpeed.iSH2Sp &&
           ArmSpeed[IndexArm].iACDCBodySP==AutoArmSpeed[InArm].iACDCBodySP &&
           ArmSpeed[IndexArm].iACDCBodySP==AutoArmSpeed[OutArm].iACDCBodySP &&
           ArmSpeed[IndexArm].iACDCBodySP==ArmSpeed[TrayArm].iACDCBodySP &&
           ArmSpeed[IndexArm].iACDCBodySP==SHSpeed.iSH1ACDCSp &&
           ArmSpeed[IndexArm].iACDCBodySP==SHSpeed.iSH2ACDCSp)                  //¦pªG¥þ³¡ªº³t«×©M¥[³t«×³£¬Ûµ¥
        {
            bFlag=true;
        }
        else
        {
            bFlag=false;
        }
    }

    if(bFlag)
    {
        bShowTotal=true;
        sgdSpeedView->Height=42;
        sgdSpeedView->RowCount=2;
    }
    else
    {
        bShowTotal=false;
        sgdSpeedView->Height=150;
        sgdSpeedView->RowCount=7;
    }

    if(tag)
    {
//        Label1->Visible=true;
        sgdSpeedView->Visible=true;
        sgdSpeedView->ColWidths[0]=64;
        sgdSpeedView->ColWidths[1]=64;
        sgdSpeedView->ColWidths[2]=64;
        pCanvas->Handle=hDC;
    }
    else
    {
//        Label1->Visible=false;
        sgdSpeedView->Visible=false;
    }
    iTrayTop=sgdSpeedView->Height+sgdSpeedView->Top+5;
    iTrayLeft=10;

    sgdSpeedView->Repaint();
    ShowTrayDeviceDir();
}
//---------------------------------------------------------------------------
void __fastcall TfShowMessage::ShowMyMessage()
{
    if(InitialOK==false)
        return;

    ShowSpeed(IniConfig.bG05ShowSpeedMessage);

    if(IniConfig.bSPILFunction==true)
    {
        lblIndexCycleTime->Left     =fTemperFrom->Left+fTemperFrom->Width;      //JerryYang 20230822 : UI½Õ¾ã
        lblIndexCycleTime->Top      =fTemperFrom->Top;
        lblTestTime->Left           =fTemperFrom->Left+fTemperFrom->Width;
        lblTestTime->Top            =lblIndexCycleTime->Top+lblIndexCycleTime->Height;
        sgdSpeedView->Left          =fTemperFrom->Left+fTemperFrom->Width;
        sgdSpeedView->Top           =lblTestTime->Top+lblTestTime->Height;

        fTemperFrom->Memo1->Top     =lblTestTime->Top+lblTestTime->Height;      //JerryYang 20190926 ²¾¨ì¥~­±¨Ó
        fTemperFrom->Button1->Top   =lblTestTime->Top+lblTestTime->Height+55;
        fTemperFrom->Memo1->Left    =208;
        fTemperFrom->Button1->Left  =208;
    }
    else
    {
        lblIndexCycleTime->Top      =fTemperFrom->Top+fTemperFrom->Height+10;
        lblTestTime->Top            =lblIndexCycleTime->Top+lblIndexCycleTime->Height;
        sgdSpeedView->Top           =lblTestTime->Top+lblTestTime->Height;
        fTemperFrom->Memo1->Top     =lblTestTime->Top+lblTestTime->Height;      //JerryYang 20190926 ²¾¨ì¥~­±¨Ó
        fTemperFrom->Button1->Top   =lblTestTime->Top+lblTestTime->Height+55;
        fTemperFrom->Memo1->Left    =208;
        fTemperFrom->Button1->Left  =208;
    }

    iTrayTop=sgdSpeedView->Height+sgdSpeedView->Top+5;
    iTrayLeft=10;
    lblInitialMessage->Top=300;
    ProgressBar1->Top=400;
    btnBackToMain->Top=50;

    if(IniConfig.iUserLanguage==eulKorea)
    {
        lblInitialMessage->Font->Charset=HANGEUL_CHARSET;
        lblInitialMessage->Caption="¾È³çÇÏ¼¼¿ä"+lblInitialMessage->Caption;
    }
    else if(IniConfig.iUserLanguage==eulJapan)
    {
        lblInitialMessage->Font->Charset=SHIFTJIS_CHARSET;
        lblInitialMessage->Caption="¡“ú‚Í"+lblInitialMessage->Caption;
    }
    else
    {
        lblInitialMessage->Font->Charset=DEFAULT_CHARSET;
        lblInitialMessage->Caption="Hello, "+lblInitialMessage->Caption;
    }

    ShowTrayDeviceDir();                                                        //jou 2013-03-25 show Tray & Device Direction start
    FormClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TfShowMessage::ShowTrayDeviceDir()
{
    bool bShow=(IniConfig.bShowTrayAndDeviceDir && UserDefForm_File[0].bEnableIndicator);                               //Steven 20190211 : ¥i¥H¨ú®øÅã¥ÜIC¤è¦V
    //jou 2013-03-25 show Tray & Device Direction start
    imgTrayHere->Visible    =bShow;
    imgDeviceHere->Visible  =bShow;
    imgTray->Visible        =bShow;
    imgDevice->Visible      =bShow;

    if(bShow)
    {
        imgTray->Top=iTrayTop;
        if(UserDefForm[0].iTrayDirection==0)
        {
            imgTray->Left=imgTrayHere->Width;
            imgTrayHere->Top=iTrayTop;
            imgTrayHere->Left=iTrayLeft;
        }
        else if(UserDefForm[0].iTrayDirection==1)
        {
            imgTray->Left=10;
            imgTrayHere->Top=iTrayTop+imgTrayHere->Height-imgTrayHere->Height;
            imgTrayHere->Left=imgTray->Left+imgTray->Width-10;
        }
        else if(UserDefForm[0].iTrayDirection==2)
        {
            imgTray->Left=10;
            imgTrayHere->Top=iTrayTop-imgTrayHere->Height/2;
            imgTrayHere->Left=imgTray->Left+imgTray->Width-10;
        }
        else if(UserDefForm[0].iTrayDirection==3)
        {
            imgTray->Left=imgTrayHere->Width;
            imgTrayHere->Top=iTrayTop+imgTrayHere->Height;
            imgTrayHere->Left=iTrayLeft;
        }

        imgDevice->Top=iTrayTop+10;
        imgDevice->Left=imgTray->Width+imgTrayHere->Width;
        if(UserDefForm[0].iDeviceDirection==0)                                  //0 deg.
        {
            imgDeviceHere->Top=imgDevice->Top+imgDeviceHere->Height/2-10;
            imgDeviceHere->Left=imgDevice->Left;
            imgDevice->Left+=imgDeviceHere->Width;
        }
        else if(UserDefForm[0].iDeviceDirection==1)                             //90 deg.
        {
            imgDeviceHere->Top=imgDevice->Top-10;
            imgDeviceHere->Left=imgDevice->Left+imgDevice->Width;
        }
        else if(UserDefForm[0].iDeviceDirection==2)                             //180 deg.
        {
            imgDeviceHere->Top=imgDevice->Top+imgDevice->Height-imgDeviceHere->Height-12;
            imgDeviceHere->Left=imgDevice->Left+imgDevice->Width;
        }
        else if(UserDefForm[0].iDeviceDirection==3)                             //270 deg.
        {
            imgDeviceHere->Top=imgDevice->Top+imgDevice->Height-imgDeviceHere->Height+12;
            imgDeviceHere->Left=imgDevice->Left;
            imgDevice->Left+=imgDeviceHere->Width;
        }
    }
    //jou 2013-03-25 show Tray & Device Direction end
}
//---------------------------------------------------------------------------
void __fastcall TfShowMessage::FormShow(TObject *Sender)
{
    bShow=true;
    ShowMyMessage();
    fTemperFrom->Memo1->Parent=fShowMessage;                                    //JerryYang 20190926 ²¾¨ì¥~­±¨Ó
    fTemperFrom->Button1->Parent=fShowMessage;
}
//---------------------------------------------------------------------------
void __fastcall TfShowMessage::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    bShow=false;
}
//---------------------------------------------------------------------------
void __fastcall TfShowMessage::sgdSpeedViewDrawCell(TObject *Sender,
      int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
    if(InitialOK==false)
        return;

    AnsiString str;
    bool bCenter=true;
    bCenter=(ACol==0)?false:true;
    pCanvas->Brush->Color=(bCenter)?clWhite:clBtnFace;
    switch(ARow)
    {
        case 0:
            if(     ACol==0) str="\0";
            else if(ACol==1) str="XY Speed";
            else if(ACol==2) str="XY Accel";
            pCanvas->Brush->Color=clBtnFace;
            break;
        case 1:
            if(bShowTotal && ACol==0)   str="Motor Speed";                      //¦pªG³t«×³£¬Ûµ¥
            else if(ACol==0) str="Index Arm";
            else if(ACol==1) str.sprintf("%d\%", ArmSpeed[IndexArm].iBodySP);
            else if(CUSTOMER_CODE==CC_SCS)                                      //jou 2016-05-11 SCS ­n¨D­nÅã¥Ü Motor ACD ¼Æ­È
            {
                if(ACol==2) str.sprintf("%d\%", ArmSpeed[IndexArm].iACDCBodySP);                                        //Steven 20160319 : mark¤£Åã¥Ü,Á×§K«È¤áGGYY
            }
            break;
        case 2:
            if(ArmSpeed[InArm].bAutoSpeed==false)
            {
                if(     ACol==0) str="Input Arm";
                else if(ACol==1) str.sprintf("%d\%", ArmSpeed[InArm].iBodySP);
                else if(ACol==2) str.sprintf("%d\%", ArmSpeed[InArm].iACDCBodySP);
            }
            else
            {
                if(     ACol==0) str="Input Arm";
                else if(ACol==1) str.sprintf("%d\%", AutoArmSpeed[InArm].iBodySP);
                else if(ACol==2) str.sprintf("%d\%", AutoArmSpeed[InArm].iACDCBodySP);
            }
            break;
        case 3:
            if(ArmSpeed[InArm].bAutoSpeed==false)
            {
                if(     ACol==0) str="Output Arm";
                else if(ACol==1) str.sprintf("%d\%", ArmSpeed[OutArm].iBodySP);
                else if(ACol==2) str.sprintf("%d\%", ArmSpeed[OutArm].iACDCBodySP);
            }
            else
            {
                if(     ACol==0) str="Output Arm";
                else if(ACol==1) str.sprintf("%d\%", AutoArmSpeed[OutArm].iBodySP);
                else if(ACol==2) str.sprintf("%d\%", AutoArmSpeed[OutArm].iACDCBodySP);
            }
            break;
        case 4:
            if(     ACol==0) str="Tray Arm";
            else if(ACol==1) str.sprintf("%d\%", ArmSpeed[TrayArm].iBodySP);
            else if(ACol==2) str.sprintf("%d\%", ArmSpeed[TrayArm].iACDCBodySP);
            break;
        case 5:
            if(     ACol==0) str="Shuttle 1";
            else if(ACol==1) str.sprintf("%d\%", SHSpeed.iSH1Sp);
            else if(ACol==2) str.sprintf("%d\%", SHSpeed.iSH1ACDCSp);
            break;
        case 6:
            if(     ACol==0) str="Shuttle 2";
            else if(ACol==1) str.sprintf("%d\%", SHSpeed.iSH2Sp);
            else if(ACol==2) str.sprintf("%d\%", SHSpeed.iSH2ACDCSp);
            break;
    }

    if(bCenter) MyDrawText(pCanvas, Rect, str.c_str());
    else        MyDrawText(pCanvas, Rect, str.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TfShowMessage::UpdateForm(int iBar)
{
    ProgressBar1->Position=iBar;
    LogSoftwareOnTime("UpdateForm, "+AnsiString(iBar));                         //Steven 20210526 : ¬ö¿ý³nÅé°õ¦æ®É¶¡
    if(iBar==100)
    {
        lblInitialMessage->Visible=false;                                       //Steven 20120726 : °µ§¹´NÁôÂÃ
        ProgressBar1->Visible=false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfShowMessage::FormMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    SetFormLayer();
}
//---------------------------------------------------------------------------
void __fastcall TfShowMessage::FormClick(TObject *Sender)
{
    SetFormLayer();
}
//---------------------------------------------------------------------------
void __fastcall TfShowMessage::SetFormLayer()                                   //§â¸ÓÅã¥Ü¦b³Ì¤W¼hªºµe­±Åã¥Ü¥X¨Ó
{
    fTemperFrom->BringToFront();
    fSortCT->BringToFront();
    fTestCategory->BringToFront();
    fContactCT->BringToFront();
    fShowBinSelect->BringToFront();
    fLotInfo->BringToFront();

    if(CosFunction.bManualSteplAutoTeach)                                       //JimmyChiu 20211020 : Auto alignment mode
        fAutoTeach->BringToFront();

    fMain->BringToFront();

    if(fContact->fShow)
    {
        fContact->BringToFront();
    }

    if(fFTPClient!=NULL)
    {
        if(fFTPClient->bShow)
        {
            fFTPClient->BringToFront();
        }
    }

    if(FormBarcodeReader->bShow)                                                //Steven 20140508
        FormBarcodeReader->BringToFront();

    if(fSCKART->bShow)                                                          //Steven 20161025 (wei) : SCK ART function
        fSCKART->BringToFront();

   if(fOffSet->fShow)                                                           //Sam 20250815 : ­×¥¿ offset µe­±»\¦í¾É­P [A01] ¥\¯à¥¢®Ä°ÝÃD
        fOffSet->BringToFront();
    fMain->MainFormSizeToEpson(true);                                           //Ifor 20170803 (wei) add ¦bªÅ¥Õ³B«ö¤U·Æ¹«¥ª/¥kÁä±j¨î¤Á´«¦ÜMain­¶­±Á×§Kµe­±Åã¥Ü²§±`
}
//---------------------------------------------------------------------------
void __fastcall TfShowMessage::FormDestroy(TObject *Sender)
{
    try
    {
        ReleaseDC(0, hDC);
        delete pCanvas;                                                         //Steven 20141023 : for ¸Ñºc²§±`
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfShowMessage::FormDestroy");
    }
    LogSoftwareOffTime("TfShowMessage, FormDestroy");
}
//---------------------------------------------------------------------------
