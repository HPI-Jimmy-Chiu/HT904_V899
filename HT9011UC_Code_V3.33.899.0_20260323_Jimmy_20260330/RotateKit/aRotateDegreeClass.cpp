#include "MachineDefine.h"
#pragma hdrstop

#include "aRotateDegreeClass.h"

#include "fRotate.h"
#include "cmydef.h"
#include "common.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmRot *frmRot;
TfrmRot *FrmRotDegree[2][4][8];
//---------------------------------------------------------------------------
__fastcall TfrmRot::TfrmRot(TComponent* Owner)
    : TForm(Owner)
{
    lblSiteMap->Caption="";
}
//---------------------------------------------------------------------------
void __fastcall TfrmRot::FormDestroy(TObject *Sender)
{
    try
    {
        grpRotDegree->Parent=pnlRotDegree;
        rgRotateDegree->Parent=pnlRotDegree;
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfrmRot::FormDestroy");
    }
    LogSoftwareOffTime("TfrmRot, FormDestroy"); //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
void TfrmRot::SetTag(bool bIn, int iY, int iX)
{
    bInput=bIn;
    iXPos=iX;
    iYPos=iY;
}
//---------------------------------------------------------------------------
void TfrmRot::SetCaption(AnsiString caption)
{
    grpRotDegree->Caption=caption;
    rgRotateDegree->Caption=caption;
//    rgRotateDegree->Name=AnsiString("RotateDegree")+caption;
}
//---------------------------------------------------------------------------
void TfrmRot::SetDegree(int iDegree)
{
    if(iDegree<0)
    {
        iDegree=(4+iDegree)%4;
        rgRotateDegree->ItemIndex=iDegree;
    }
    else
    {
        rgRotateDegree->ItemIndex=iDegree;
    }
    ReloadPicture();
}
//---------------------------------------------------------------------------
void TfrmRot::ReloadPicture()
{
    int iAng=(rgRotateDegree->ItemIndex+(FrmRotate->iFromTrayAngle/90))%4;

    if(iAng==0)
    {
        imgRotDegree->Picture->LoadFromFile(BmpPath+"Device0_1.bmp");
    }
    else if(iAng==1)
    {
        imgRotDegree->Picture->LoadFromFile(BmpPath+"Device90_1.bmp");
    }
    else if(iAng==2)
    {
        imgRotDegree->Picture->LoadFromFile(BmpPath+"Device180_1.bmp");
    }
    else
    {
        imgRotDegree->Picture->LoadFromFile(BmpPath+"Device270_1.bmp");
    }

    if(iSiteMap==0)
        lblSiteMap->Caption="";
    else
        lblSiteMap->Caption=iSiteMap;
    lblSiteMap->Width=imgRotDegree->Width;
}
//---------------------------------------------------------------------------
void __fastcall TfrmRot::imgRotDegreeClick(TObject *Sender)
{
    int index=rgRotateDegree->ItemIndex;
    int iIn=(bInput)?0:1;

    index++;
    if(index>=rgRotateDegree->Items->Count)
    {
        index=0;
    }

    rgRotateDegree->ItemIndex=index;
    FrmRotate->iRotateDutDate[iIn][iYPos][iXPos]=90*index;

    if(FrmRotate->rgSelect->ItemIndex==0)
    {
        for(int i=0; i<MAX_SOCKET_ROW; i++)
        {
            for(int j=0; j<MAX_SOCKET_COL; j++)
            {
                FrmRotDegree[iIn][i][j]->rgRotateDegree->ItemIndex=rgRotateDegree->ItemIndex;
                FrmRotDegree[iIn][i][j]->ReloadPicture();
            }
        }
    }
    ReloadPicture();
}
//---------------------------------------------------------------------------
