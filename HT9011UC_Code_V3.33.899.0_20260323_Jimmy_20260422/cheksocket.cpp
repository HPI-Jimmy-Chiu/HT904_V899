//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "cheksocket.h"

#include "cpublic.h"
#include "cmydef.h"
#include "cprod.h"
#include "main.h"
//#include "uCCDView.h"
#include "myswitch.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "EPICOLOACLLib_OCX"
#pragma link "eVision_OCX"
#pragma resource "*.dfm"
TfCCDPin1 *fCCDPin1;
//---------------------------------------------------------------------------
__fastcall TfCCDPin1::TfCCDPin1(TComponent* Owner)
    : TForm(Owner)
{
    fShow=false;
    // BW
    CCD->ColorFormat=1;
    // NTSC
    CCD->Standard=2;        // NTSC=1     PAL=2

    LiveImage->SetSize (CCD->ImageSizeX, CCD->ImageSizeY);
    SourceImage->SetSize (CCD->ImageSizeX, CCD->ImageSizeY);
    CCD->AddSurface (CCD->CreateVisionSurface(LiveImage->DefaultDispatch));

    ROI1->ParentImage=EBW8Image1->DefaultDispatch;
    ROI1->DrawColor=clRed;
    ROI2->ParentImage=EBW8Image2->DefaultDispatch;
    ROI2->DrawColor=clRed;
    bLive=false;
    bGrabOne=false;

}
//---------------------------------------------------------------------------
void __fastcall TfCCDPin1::ReDrawAll()
{
    LiveImage->Refresh();
    if(bLive || fShow==false)
        return;
}

//----------------------------------------------------------------------------
void __fastcall TfCCDPin1::SaveImage()
{
    char str[256];
    if(bHasSourceImage)
    {
        sprintf(str,"d:\\newtesthander\\data\\%s.bmp",fMain->ComboBox3->Text.c_str());
        SourceImage->SaveFile(WideString(str),eGrayLevelBmp);
        TestIF.iCCDVarlance=atoi(Edit3->Text.c_str());
    }
}
//----------------------------------------------------------------------------
void __fastcall TfCCDPin1::Load()
{
    char str[256];
    sprintf(str,"d:\\newtesthander\\data\\%s.bmp",fMain->ComboBox3->Text.c_str());
    bHasSourceImage=false;
    bChangeSourceImage=false;
    if(CheckFileExist(str))
    {
        bHasSourceImage=true;
        SourceImage->LoadFile(WideString(str));
        SourceImage->Refresh();
        Edit3->Text=TestIF.iCCDVarlance;
    }
}

void __fastcall TfCCDPin1::FormShow(TObject *Sender)
{
    char str[256];
    fShow=true;
    SW[SwCCDLight].On();
    Load();

}
//---------------------------------------------------------------------------

void __fastcall TfCCDPin1::FormClose(TObject *Sender, TCloseAction &Action)
{
    fShow=false;
    CCD->Stop();
    SW[SwCCDLight].Off();

}
//---------------------------------------------------------------------------

void __fastcall TfCCDPin1::CCDSurfaceAvailable(TObject *Sender,
      long SurfaceHandle, long Data, long Flags, long TimeCode,
      long ExtendedStatus)
{
    ReDrawAll();
    bGrabOne=true;
    //if(fShow==false && fCCDView->fShow)
    //{
    //    EasyMain->Op12(eImgCopy,LiveImage->DefaultDispatch,fCCDView->EBW8Image1->DefaultDispatch);
        LiveImage->Refresh();
    //}

}
//---------------------------------------------------------------------------

void __fastcall TfCCDPin1::SpeedButton1Click(TObject *Sender)
{
    bLive=true;
    CCD->Live();

}
//---------------------------------------------------------------------------

void __fastcall TfCCDPin1::SpeedButton2Click(TObject *Sender)
{
    bLive=false;
    CCD->Grab();

}
//---------------------------------------------------------------------------

void __fastcall TfCCDPin1::SpeedButton3Click(TObject *Sender)
{
    bLive=false;
    CCD->Stop();
}
//---------------------------------------------------------------------------

void __fastcall TfCCDPin1::Button1Click(TObject *Sender)
{
    EasyMain->Op12(eImgCopy,LiveImage->DefaultDispatch,SourceImage->DefaultDispatch);
    bHasSourceImage=true;
    bChangeSourceImage=true;
    SourceImage->Refresh();

}
//---------------------------------------------------------------------------

void __fastcall TfCCDPin1::Button2Click(TObject *Sender)
{
    Caculate();

}
//---------------------------------------------------------------------------
#define XR   32
#define YR   24
bool __fastcall TfCCDPin1::Caculate()
{
/*
    char str[256];
    char str2[256];
    int x,y;
    EasyMain->Op123( eImgSubtract,LiveImage->DefaultDispatch,SourceImage->DefaultDispatch,EBW8Image1->DefaultDispatch);
    EasyMain->Op123( eImgSubtract,SourceImage->DefaultDispatch,LiveImage->DefaultDispatch,EBW8Image2->DefaultDispatch);
    LiveImage->DrawColor=clRed;
    EBW8Image1->DrawColor=clRed;
    EBW8Image2->DrawColor=clRed;
    EBW8Image1->Refresh();
    EBW8Image2->Refresh();
    float lngAverage,lngVariance,Max;
    Memo1->Lines->Clear();
    Memo1->Lines->Add("========================= PAGE1 =========================");
    Max=atof(Edit3->Text.c_str());
    LiveImage->Refresh();
    EBW8Image1->Refresh();
    EBW8Image2->Refresh();
    if(fCCDView->fShow)
    {
        EasyMain->Op12(eImgCopy,LiveImage->DefaultDispatch,fCCDView->EBW8Image1->DefaultDispatch);
        fCCDView->EBW8Image1->Refresh();
    }

    bool bPass=true;
    for(x=4;x<16;x++)
    {
        for(y=5;y<19;y++)
        {
            ROI1->SetPlacement(x*XR,y*YR,XR,YR);
            EasyMain->PixelVarianceBW8(ROI1->DefaultDispatch,&lngVariance,&lngAverage);
            sprintf(str,"X=%d Y=%d Var=%7.1f ",x*XR,y*YR,lngVariance);
            Label3->Caption=str;
            Label3->Refresh();
            if( lngVariance>Max )
            {
                strcpy(str2," XXXX ");
                strcat(str2,str);
                Memo1->Lines->Add(str2);
                if(fShow)
                {
                    LiveImage->DrawBox(x*XR,y*YR,x*XR+XR,y*YR+YR);
                    EBW8Image1->DrawBox(x*XR,y*YR,x*XR+XR,y*YR+YR);
                }
                if(fCCDView->fShow)
                {
                    fCCDView->EBW8Image1->DrawBox(x*XR,y*YR,x*XR+XR,y*YR+YR);
                }
                bPass=false;
            }
        }
    }
    Memo1->Lines->Add("========================= PAGE2 =========================");
    for(x=4;x<16;x++)
    {
        for(y=5;y<19;y++)
        {
            ROI2->SetPlacement(x*XR,y*YR,XR,YR);
            EasyMain->PixelVarianceBW8(ROI2->DefaultDispatch,&lngVariance,&lngAverage);
            sprintf(str,"X=%d Y=%d Var=%7.1f ",x*XR,y*YR,lngVariance);
            Label3->Caption=str;
            Label3->Refresh();
            if( lngVariance>Max )
            {
                strcpy(str2," XXXX ");
                strcat(str2,str);
                Memo1->Lines->Add(str2);
                if(fShow)
                {
                    LiveImage->DrawBox(x*XR,y*YR,x*XR+XR,y*YR+YR);
                    EBW8Image2->DrawBox(x*XR,y*YR,x*XR+XR,y*YR+YR);
                }
                if(fCCDView->fShow)
                {
                    fCCDView->EBW8Image1->DrawBox(x*XR,y*YR,x*XR+XR,y*YR+YR);
                }

                bPass=false;
            }
        }
    }
    return bPass;
    */
    return true;
}
//---------------------------------------------------------------------------
