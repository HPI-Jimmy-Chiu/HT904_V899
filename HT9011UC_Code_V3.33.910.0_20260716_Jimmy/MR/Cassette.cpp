//---------------------------------------------------------------------------
#include "MachineDefine.h"

#include <vcl.h>
#pragma hdrstop

#include "Cassette.h"

#include "MachineType.h"
#include "note.h"
#include "main.h"
#include "mymessbox.h"
#include "asendic.h"
#include "mymotor.h"
#include "mycylin.h"
#include "mysensor.h"
#include "myswitch.h"
#include "MyKitSuck.h"
#include "cprod.h"
#include "cpublic.h"
#include "uHGemHT9045.h"
#include "cmydef.h"
#include "csystem.h"
#include "acatchcassette.h"
#include "uTrayEditForm.h"
#include "uLotInfo.h"
#include "common.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "HTray"
#pragma link "ALed"
#pragma resource "*.dfm"
TfCassette *fCassette;
//---------------------------------------------------------------------------
__fastcall TfCassette::TfCassette(TComponent* Owner)
    : TForm(Owner)
{
    TTMyTray *mt[]=
    {
        fCassette->mtCassette01, fCassette->mtCassette02,       fCassette->mtCassette03,
        fCassette->mtCassette04, fCassette->mtCassette05,       fCassette->mtCassette06,
        fCassette->mtCassette07, fCassette->mtCassette08,       fCassette->mtCassette09,
        fCassette->mtCassette10, fCassette->mtLoadPort,         fCassette->mtTrayLoader,
        fCassette->mtTrayEmpty,  fCassette->mtTrayConversion,   fCassette->mtTrayAuto1,
        fCassette->mtTrayAuto2,  fCassette->mtTrayAuto3,        fCassette->TMyTray1,
        fCassette->TMyTray2,     fCassette->TMyTray3,           fCassette->TMyTray4,
        fCassette->mtLoadPort1,  fCassette->mtLoadPort2,        fCassette->mtLoadPort3,     fCassette->mtLoadPort4,    //Sam 20190112 LM
        fCassette->mtUnloadPort1,fCassette->mtUnloadPort2,      fCassette->mtUnloadPort3,   fCassette->mtUnloadPort4,   //Sam 20190112 LM
        fCassette->mtMLoadRobotZ,fCassette->mtMUnloadRobotZ                                                             //Sam 20190112 LM
    };
    for(int i=0; i<sizeof(mt)/4; i++)
    {
        mt[i]->SetColorMap(NULL_IC,(TColor)0x00D7D7D7);
        mt[i]->SetColorMap(1 ,clGreen);
        mt[i]->SetColorMap(HAS_IC ,clGreen);
        mt[i]->SetColorMap(3 ,(TColor)0x00FF8080);
        mt[i]->SetColorMap(4 ,(TColor)0x004080FF);
        mt[i]->SetColorMap(5 ,clWhite);
        mt[i]->SetColorMap(6 ,clAqua);
        mt[i]->SetColorMap(HAS_CLEAN_IC      ,clInactiveCaption);
        mt[i]->SetColorMap(HAS_NULL_CLEAN_IC ,clAqua);
        mt[i]->SetColorMap(CLEAN_FINISH_IC   ,clBlue);
        mt[i]->SetColorMap(10 ,clInactiveCaption);

        mt[i]->SetColorMap(HAS_OCR_OK,clLime);
        mt[i]->SetColorMap(HAS_OCR_NG,clRed);
        mt[i]->SetColorMap(HAS_OCR_Err,clNavy);

        mt[i]->SetColorMap(HAS_CASSETTE_EMPTY,clFuchsia);       //wei 20180702 MR
        mt[i]->SetColorMap(HAS_CASSETTE_TRAY,clAqua);           //wei 20180702 MR
        mt[i]->SetColorMap(HAS_CASSETTE_DEVICE,clBlue);         //wei 20180702 MR
//        mt[i]->SetColorMap(HAS_CASSETTE_FULLDEVICE,clGreen);    //wei 20180702 MR
        mt[i]->SetColorMap(HAS_CASSETTE_FULLTRAY,clNavy);       //wei 20180702 MR
        mt[i]->SetColorMap(HAS_CASSETTE_PASS,clGreen);          //wei 20180702 MR
        mt[i]->SetColorMap(HAS_CASSETTE_FAIL,clRed);            //wei 20180702 MR
    }
}
//---------------------------------------------------------------------------
void __fastcall TfCassette::FormShow(TObject *Sender)
{
    bShow=true;
    fCassette->Height=870;
    fCassette->Width=1100;
    AnsiString str="";
    sgMRBufferList->Cells[ 0][ 0]="Data";
    for(int i=0; i<10; i++)
    {
        str.sprintf("Buffer%d", i+1);
        sgMRBufferList->Cells[i+1][ 0]=str.c_str();
    }
    sgMRBufferList->Cells[ 0][ 1]="ID";
    sgMRBufferList->Cells[ 0][ 2]="Cassette Data";
    sgMRBufferList->Cells[ 0][ 3]="Lot ID";

    sgMRTrayList->Cells[ 0][ 0]="Data";
    sgMRTrayList->Cells[ 0][ 1]="Count";
    sgMRTrayList->Cells[ 1][ 0]="Loader";
    sgMRTrayList->Cells[ 2][ 0]="Empty";
    sgMRTrayList->Cells[ 3][ 0]="Conversion";
    sgMRTrayList->Cells[ 4][ 0]="Auto1";
    sgMRTrayList->Cells[ 5][ 0]="Auto2";
    sgMRTrayList->Cells[ 6][ 0]="Auto3";

    for(int i=0; i<20; i++)
    {
        str.sprintf("Tray %d", i+1);
        sgTrayIDList->Cells[ 0][i+1]=str.c_str();
    }
    sgTrayIDList->Cells[ 1][ 0]="Auto 1";
    sgTrayIDList->Cells[ 2][ 0]="Auto 2";
    sgTrayIDList->Cells[ 3][ 0]="Auto 3";

    Panel9->Height=25;
    Panel9->Width=25;
    Panel10->Height=25;
    Panel10->Width=25;
    Panel11->Height=25;
    Panel11->Width=25;
    Panel12->Height=25;
    Panel12->Width=25;
    Panel13->Height=25;
    Panel13->Width=25;

    //Sam 20190112 LM 使用 LM/MR 模擬物件整合
    //==>
    if(USE_MR_SYSTEM==2)
    {
        mtLoadPort->Visible=false;
        Label10->Visible=false;
        Label9->Visible=false;
        mtCassette01->Visible=false;
        mtCassette10->Visible=false;
        Panel7->Visible=false;
    }
    else
    {
        palMLoadRobotZBase->Visible=false;
        palMLoadRobotZ->Visible=false;
        mtLoadPort1->Visible=false;
        mtLoadPort2->Visible=false;
        mtLoadPort3->Visible=false;
        mtLoadPort4->Visible=false;

        palMUnloadRobotZBase->Visible=false;
        palMUnloadRobotZ->Visible=false;
        mtUnloadPort1->Visible=false;
        mtUnloadPort2->Visible=false;
        mtUnloadPort3->Visible=false;
        mtUnloadPort4->Visible=false;
    }
    //<==
    //Sam 20190112 LM  使用 LM/MR 模擬物件整合

    fLotInfo->btZoom->Click();

    #ifdef SOFT_SIMULTE
    CheckBox10->Visible=true;
    #endif
}
//---------------------------------------------------------------------------
void __fastcall TfCassette::FormClose(TObject *Sender, TCloseAction &Action)
{
    bShow=false;
}
//---------------------------------------------------------------------------
void __fastcall TfCassette::CheckBox10Click(TObject *Sender)
{
    MOT[MMLoadPort].SetTray(HAS_CASSETTE_DEVICE);
//    MOT[MMCABuffer2].SetTray(HAS_CASSETTE_EMPTY);
//    MOT[MMCABuffer3].SetTray(HAS_CASSETTE_EMPTY);
//    MOT[MMCABuffer4].SetTray(HAS_CASSETTE_TRAY);
}
//---------------------------------------------------------------------------
void __fastcall TfCassette::mtCassette02MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(SystemStart)
        return;

    TTMyTray *BufferBtn;
    BufferBtn = (TTMyTray *)Sender;

    if(Button==mbRight && AccessLevel==iDefHonPrecLevel)
    {
        EditTray(MMCABuffer1+BufferBtn->Tag, 3);
        if(MOT[MMCABuffer1+BufferBtn->Tag].Tray.Data[0][0]!=0)
        {
            MOT[MMCABuffer1+BufferBtn->Tag].fHasTray=true;
        }
        else
        {
            MOT[MMCABuffer1+BufferBtn->Tag].fHasTray=false;
            MOT[MMCABuffer1+BufferBtn->Tag].Tray.cCassetteID="";
            MOT[MMCABuffer1+BufferBtn->Tag].Tray.cCassetteLot="";
            ShowCassetteTrayData();
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfCassette::cbS10F5NoShowClick(TObject *Sender)
{
    if(cbS10F5NoShow->Checked==true)
        bS10F5show=true;
    else
        bS10F5show=false;
}
//---------------------------------------------------------------------------
void __fastcall TfCassette::cbQAModeClick(TObject *Sender)
{
    if(cbQAMode->Checked==true)
        bRunQAMode=true;
    else
        bRunQAMode=false;
}
//---------------------------------------------------------------------------
void TfCassette::MRSystemDataSave()
{
    AnsiString str="";
    AnsiString asIniFileName="D:\\HT9045\\system\\MRSystemData.txt";
    DeleteFile(asIniFileName);
    listMRSystemData=new TStringList();

    for(int i=0; i<10; i++)
    {
//        str.sprintf("%s,%s,%s,",MOT[MMCABuffer1+i].Tray.cCassetteID,sgMRBufferList->Cells[i+2][ 2],MOT[MMCABuffer1+i].Tray.cCassetteLot);
//        listMRSystemData->Add(str.c_str());
        listMRSystemData->Add(MOT[MMCABuffer1+i].Tray.cCassetteID);
        listMRSystemData->Add(sgMRBufferList->Cells[i+1][ 2]);
        listMRSystemData->Add(MOT[MMCABuffer1+i].Tray.cCassetteLot);
    }

    for(int i=0; i<6; i++)
    {
        listMRSystemData->Add(iMRTrayCount[i]);
    }

    listMRSystemData->SaveToFile(asIniFileName);
    listMRSystemData->Clear();
    delete listMRSystemData;
    listMRSystemData=NULL;
    ShowCassetteTrayData();
}
//---------------------------------------------------------------------------
void TfCassette::MRSystemDataRead()
{
    AnsiString str="";
    AnsiString asIniFileName="D:\\HT9045\\system\\MRSystemData.txt";

    listMRSystemData=new TStringList();
    listMRSystemData->Clear();
    listMRSystemData->LoadFromFile(asIniFileName);

    for(int i=0; i<10 ; i++)
    {
        MOT[MMCABuffer1+i].Tray.cCassetteID =listMRSystemData->Strings[i*3];
        sgMRBufferList->Cells[i+1][ 2]      =listMRSystemData->Strings[i*3+1];
        MOT[MMCABuffer1+i].Tray.cCassetteLot=listMRSystemData->Strings[i*3+2];
    }

    for(int i=0; i<6; i++)
    {
        iMRTrayCount[i]=atoi(listMRSystemData->Strings[30+i].c_str());
    }

    MRSystemDataReadData();
    listMRSystemData->Clear();
    delete listMRSystemData;
    listMRSystemData=NULL;
}
//---------------------------------------------------------------------------
void TfCassette::MRSystemDataReadData()
{
    for(int i=0; i<10; i++)
    {
        if(sgMRBufferList->Cells[i+1][ 2]=="EMPTY")
        {
            MOT[MMCABuffer1+i].SetTraySingleData(0, 0, HAS_CASSETTE_EMPTY);
            fLotInfo->sgMRBufferLotList->Cells[2][i+1]="EMPTY";
        }
        else if(sgMRBufferList->Cells[i+1][ 2]=="TRAY")
        {
            MOT[MMCABuffer1+i].SetTraySingleData(0, 0, HAS_CASSETTE_TRAY);
            fLotInfo->sgMRBufferLotList->Cells[2][i+1]="TRAY";
        }
        else if(sgMRBufferList->Cells[i+1][ 2]=="DEVICE")
        {
            MOT[MMCABuffer1+i].SetTraySingleData(0, 0, HAS_CASSETTE_DEVICE);
            fLotInfo->sgMRBufferLotList->Cells[2][i+1]="DEVICE";
        }
        else if(sgMRBufferList->Cells[i+1][ 2]=="FULL TRAY")
        {
            MOT[MMCABuffer1+i].SetTraySingleData(0, 0, HAS_CASSETTE_FULLTRAY);
            fLotInfo->sgMRBufferLotList->Cells[2][i+1]="FULL TRAY";
        }
        else if(sgMRBufferList->Cells[i+1][ 2]=="FULL PASS BIN")
        {
            MOT[MMCABuffer1+i].SetTraySingleData(0, 0, HAS_CASSETTE_PASS);
            fLotInfo->sgMRBufferLotList->Cells[2][i+1]="FULL PASS BIN";
        }
        else if(sgMRBufferList->Cells[i+1][ 2]=="FULL FAIL BIN")
        {
            MOT[MMCABuffer1+i].SetTraySingleData(0, 0, HAS_CASSETTE_FAIL);
            fLotInfo->sgMRBufferLotList->Cells[2][i+1]="FULL FAIL BIN";
        }
        else
        {
            MOT[MMCABuffer1+i].SetTraySingleData(0, 0, NULL_IC);
            fLotInfo->sgMRBufferLotList->Cells[2][i+1]="";
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfCassette::Button1Click(TObject *Sender)
{
    int iBuffer=0;

    if(iDownNumber==0)
        iBuffer=atoi(fCassette->Edit2->Text.c_str())-1;
    else
        iBuffer=iDownNumber;

    if(Sen[SnLoadPortPresent].IsOn()==false &&
       Sen[SnLoadPortPlacement1].IsOn()==false && Sen[SnLoadPortPlacement2].IsOn()==false &&
       Sen[SnBuffer1HaveCassette].IsOn()==false && MOT[MMCABuffer1].HasIC()==false &&
       LastSet.iRealDummy!=DUMMY && Sen[SnBuffer1HaveCassette+iBuffer].IsOn())
    {
        InitialManualCatchCassetteTask();
        InitialLoadPortCassetteTask();
        ZeroMemory(bManualCatchCassetteDown, sizeof(bManualCatchCassetteDown));
        bManualCatchCassetteDown[0]=true;
    }
    else
    {
        SW[SwMRError].OnOff(true);
        if(Sen[SnLoadPortPresent].IsOn() || Sen[SnLoadPortPlacement1].IsOn() ||
           Sen[SnLoadPortPlacement2].IsOn())
        {
            ShowMyMessage("Check Load port,Please move cassette!!");
        }

        if(Sen[SnBuffer1HaveCassette].IsOn())
        {
            ShowMyMessage("Check buffer1,Please move cassette!!");
        }

        if(Sen[SnBuffer1HaveCassette+iBuffer].IsOn()==false)
        {
            ShowMyMessage("Check buffer no cassette!!");
        }
        bManualCatchCassetteDown[0]=false;
        SW[SwMRError].OnOff(false);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfCassette::Button2Click(TObject *Sender)
{
    if(WhichBufferCanPlace()!=0 && Sen[SnBuffer1HaveCassette].IsOn()==false &&
       MOT[MMCABuffer1].HasIC()==false && Sen[SnLoadPortPresent].IsOn() &&
       Sen[SnLoadPortPlacement1].IsOn() && Sen[SnLoadPortPlacement2].IsOn())
    {
        InitialLoadPortCassetteTask();
        InitialCatchCassetteTask();
        ZeroMemory(bManualCatchCassetteUp, sizeof(bManualCatchCassetteUp));
        bManualCatchCassetteUp[0]=true;
    }
    else
    {
        SW[SwMRError].OnOff(true);
        if(WhichBufferCanPlace()==0)
        {
            ShowMyMessage("No have buffer,can place cassette!!");
        }

        if(Sen[SnBuffer1HaveCassette].IsOn())
        {
            ShowMyMessage("Check buffer1,Please move cassette!!");
        }

        if(Sen[SnLoadPortPresent].IsOn()==false ||
           Sen[SnLoadPortPlacement1].IsOn()==false || Sen[SnLoadPortPlacement2].IsOn()==false)
        {
            ShowMyMessage("Please place cassette to Load port!!");
        }
        bManualCatchCassetteUp[0]=false;
        SW[SwMRError].OnOff(false);
    }
}
//---------------------------------------------------------------------------

void __fastcall TfCassette::Button3Click(TObject *Sender)
{
    bManualCatchCassetteDown[0]=false;
    bManualCatchCassetteUp[0]=false;
}
//---------------------------------------------------------------------------
void __fastcall TfCassette::Panel14MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Button==mbRight)
    {
        Button1->Visible=true;
        Button2->Visible=true;
        Button3->Visible=true;
        Edit2->Visible=true;
        Button4->Visible=true;
    }
    else
    {
        Button1->Visible=false;
        Button2->Visible=false;
        Button3->Visible=false;
        Edit2->Visible=false;
        Button4->Visible=false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfCassette::mtTrayEmptyMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(SystemStart)
        return;

    TTMyTray *BufferBtn;
    BufferBtn = (TTMyTray *)Sender;

    if(Button==mbRight && AccessLevel==iDefHonPrecLevel)
    {
        EditTray(MMTrayLoader+BufferBtn->Tag, 3);
        if(MOT[MMTrayLoader+BufferBtn->Tag].Tray.Data[0][0]!=0)
            MOT[MMTrayLoader+BufferBtn->Tag].fHasTray=true;
        else
            MOT[MMTrayLoader+BufferBtn->Tag].fHasTray=false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfCassette::Button4Click(TObject *Sender)
{
    for(int i=0; i<10; i++)
    {
        if(Sen[SnBuffer1HaveCassette+i].IsOn())
        {
            MOT[MMCABuffer1+i].SetTray(HAS_CASSETTE_EMPTY);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfCassette::cbAutoModeClick(TObject *Sender)
{
    if(cbAutoMode->Checked==true)
        bMRAutoMode=true;
    else
        bMRAutoMode=false;
}
//---------------------------------------------------------------------------
void __fastcall TfCassette::Button6Click(TObject *Sender)
{
    AnsiString str="";
    str=CheckNewLotID2();
    fCassette->Memo1->Lines->Add(str.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TfCassette::GroupBox1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Button==mbRight)
    {
        Button1->Visible=true;
        Button2->Visible=true;
        Button3->Visible=true;
        Edit2->Visible=true;
        Button4->Visible=true;
    }
    else
    {
        Button1->Visible=false;
        Button2->Visible=false;
        Button3->Visible=false;
        Edit2->Visible=false;
        Button4->Visible=false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfCassette::cbPort1Click(TObject *Sender)
{
    int ret;
    bool bCheckLotOK=false;
    TCheckBox *cbPort;
    cbPort=(TCheckBox *)Sender;
    bCassetteMessage=true;
    bLoadPortHasCassette=true;
    ret=ShowMyMessageBox_YES_NO("確認Cassette資料?", "1:Device    2:Empty Tray    3:Empty Cassette");
    if(ret==1)
    {
        if(MyMessageBox->rgCassetteStatus->ItemIndex==0)
        {
            MOT[MMLoadPort1+cbPort->Tag].SetTray(HAS_CASSETTE_DEVICE);
            MOT[MMLoadPort1+cbPort->Tag].Tray.cCassetteLot=MyMessageBox->edCassetteLotID->Text;
            asSameLotID=MyMessageBox->edCassetteLotID->Text;

            for(int i=0; i<4; i++)
            {
                if(asSameLotID==asMRLotData[0][i])
                {
                    bCheckLotOK=true;
                }
            }

            if(bCheckLotOK==false)
            {
//                iLotCount=0;
//                iLotCount=ShowMyInput("請輸入 Lot  數量" , "請輸入數字 : ");
            }
        }
        else if(MyMessageBox->rgCassetteStatus->ItemIndex==1)
        {
            MOT[MMLoadPort1+cbPort->Tag].SetTray(HAS_CASSETTE_TRAY);
        }
        else if(MyMessageBox->rgCassetteStatus->ItemIndex==2)
        {
            MOT[MMLoadPort1+cbPort->Tag].SetTray(HAS_CASSETTE_EMPTY);
        }

        bLoadPortHasCassette=false;
        bCassetteMessage=false;
    }
    else if(ret==2)
    {
        bLoadPortHasCassette=false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfCassette::cbTrayModeClick(TObject *Sender)
{
    iMRStartMode=1;
    cbCassetteMode->Checked=false;
    cbCleanMode->Checked=false;
}
//---------------------------------------------------------------------------
void __fastcall TfCassette::cbCassetteModeClick(TObject *Sender)
{
    iMRStartMode=2;                                 //wei 20190227
    MOT[MMCABuffer5].SetTray(HAS_CASSETTE_DEVICE);
    MOT[MMCABuffer5].Tray.cCassetteLot="Test1";
    ShowCassetteData(0, "Test1");
    MOT[MMCABuffer7].SetTray(HAS_CASSETTE_TRAY);
    bLoadNeedTray=true;
    bEmptyNeedTray=true;
    ShowCassetteTrayData();
    cbTrayMode->Checked=false;
    cbCleanMode->Checked=false;
}
//---------------------------------------------------------------------------
void __fastcall TfCassette::cbCleanModeClick(TObject *Sender)
{
    iMRStartMode=0;                                 //wei 20190227
    MOT[MMCABuffer5].ClearTray();
    ShowCassetteData(0, "");
    MOT[MMCABuffer7].ClearTray();
    MOT[MMTrayEmpty].ClearTray();

    bLoadNeedTray=false;
    bEmptyNeedTray=false;
    ShowCassetteTrayData();
    cbTrayMode->Checked=false;
    cbCassetteMode->Checked=false;
}
//---------------------------------------------------------------------------

