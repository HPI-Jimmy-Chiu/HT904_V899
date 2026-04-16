//---------------------------------------------------------------------------
#include "MachineDefine.h"
#pragma hdrstop

#include "fRENESAS_ServerFrm.h"

#include "INPUT.h"
#include "cmydef.h"
#include "cprod.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ufrmProdInfo"
#pragma resource "*.dfm"
TfServerFrm *fServerFrm;
//---------------------------------------------------------------------------
__fastcall TfServerFrm::TfServerFrm(TComponent* Owner)
    : TForm(Owner)
{

}
//---------------------------------------------------------------------------
void TfServerFrm::AddRecvLog(AnsiString asRecvData)
{
    //mmoHistory->Lines->Add(asRecvData);
}
//---------------------------------------------------------------------------
//void __fastcall TfServerFrm::OnProcTimer(TObject *Sender)
//{
//
//}
//---------------------------------------------------------------------------
void __fastcall TfServerFrm::FormCreate(TObject *Sender)
{
    iServerListenStatus=0;
    asHandlerLastSeqStatus=RENESAS_CMD.LotEndReplyCMD;                          //RogerYang 20251009 : init "41"
}
//---------------------------------------------------------------------------
void __fastcall TfServerFrm::ProcTimerTimer(TObject *Sender)
{
//    if(iServerListenStatus==1)
//        pnlListenStatus->Color=clGreen;
//    else
//        pnlListenStatus->Color=clRed;

    //Kirin 20191220 FTCT
    //==>
    if(TestIF_File.bRENESAS_EnableFTCT==true)
    {
        pnlOnLine->Color=clGreen;
        pnlOnLine->Caption ="OnLine";
    }
    else
    {
        pnlOnLine->Color=clRed;
        pnlOnLine->Caption ="OffLine";
    }
    //<==
    //Kirin 20191220 FTCT

    if(TestMode=="00") //Kirin 20191220 Test
    {
        pnlTestMode->Color=clGreen;
        pnlTestMode->Caption ="Test Start";
    }
    else if(TestMode=="10")
    {
        pnlTestMode->Color=clGreen;
        pnlTestMode->Caption ="Re-test Start";
    }
    else if(TestMode=="20")
    {
        pnlTestMode->Color=clGreen;
        pnlTestMode->Caption ="Re-re-test Start";
    }
    else
    {
        pnlTestMode->Color=clGray;
        pnlTestMode->Caption ="no select";
    }
    //Kirin 20200112 hdParamter Show
    //==>
    lbledtHdPass->Text=iHdPass;
    lbledtHdFail->Text=iHdFail;
    lbledtHdRejectATPass->Text=iHdRejectATPass;
    lbledtHdRejectATFail->Text=iHdRejectATFail;
    lbledtHdRejectBT->Text=iHdRejectBT;
    lbledtTesterMatch->Text=iTesterMatch;
    lbledtTesterUnmatch->Text=iTesterUnmatch;
    lbledtTesterIgnore->Text=iTestIgnore;
    lbledtHdPickUp->Text=iHdPickUp;
    //<==
    //Kirin 20200112 hdParamter Show
    pnlFTCTLastSeqStatus->Caption=asFTCTLastSeqStatus;    //Kirin 20200207 增加最後送出狀態來看
    pnlHandlerSeq->Caption       =asHandlerLastSeqStatus; //Kirin 20200207 增加最後送出狀態來看
    pnlNowCounter->Caption       =iNowCounter; //Kirin 20220224
    pnlSetCounter->Caption       =iSetCounter; //Kirin 20220224
    //Kirin 20221115 add two passbin
    //==>
    lbledtHdPass1->Text=iHdPass1;
    lbledtHdPass2->Text=iHdPass2;
    lbledtHdRejectATPass1->Text=iHdRejectATPass1;
    lbledtHdRejectATPass2->Text=iHdRejectATPass2;
    //<==
    //Kirin 20221115 add two passbin
    //Kirin 20231211 FTCT Code 自定義
    //==>
    AnsiString asFTCTDesc1;
    AnsiString asFTCTDesc2;
    //if(iGetTwoPassBinMode()==1)
    {
       // if(bCustomizeFTCTCode)
        {
            /*
            if(Trim(AnsiString(TestIF.cFTCTResult[0]))!="")
            {
                asFTCTDesc1.sprintf("%s",TestIF.cFTCTResult[0]);
                asFTCTDesc1=asFTCTDesc1.Trim();
                if(asFTCTDesc1.Length()==0)
                    asFTCTDesc1="1";
            }
            else
            {
                asFTCTDesc1="1";
            }
            if(Trim(AnsiString(TestIF.cFTCTResult[1]))!="")
            {
                asFTCTDesc2.sprintf("%s",TestIF.cFTCTResult[1]);
                asFTCTDesc2=asFTCTDesc2.Trim();
                if(asFTCTDesc2.Length()==0)
                    asFTCTDesc2="2";
            }
            else
            {
                asFTCTDesc2="2";
            }
            */
        }
       // else
        {
            asFTCTDesc1="1";
            asFTCTDesc2="2";
        }
    }
    //<==
    //Kirin 20231211 FTCT Code 自定義
    //Kirin 20231211
    //==>
    AnsiString asTempDesc1;
    AnsiString asTempDesc2;
    asTempDesc1.sprintf("HdPass%s",asFTCTDesc1.c_str());
    asTempDesc2.sprintf("HdPass%s",asFTCTDesc2.c_str());
    lbledtHdPass1->EditLabel->Caption=asTempDesc1;
    lbledtHdPass2->EditLabel->Caption=asTempDesc2;
    asTempDesc1.sprintf("HdATPass%s",asFTCTDesc1.c_str());
    asTempDesc2.sprintf("HdATPass%s",asFTCTDesc2.c_str());
    lbledtHdRejectATPass1->EditLabel->Caption=asTempDesc1;
    lbledtHdRejectATPass2->EditLabel->Caption=asTempDesc2;
    //<==
    //Kirin 20231211
}
//---------------------------------------------------------------------------
// //Kirin 20220127 人員可以輸入Loader輸入數量
//---------------------------------------------------------------------------
void __fastcall TfServerFrm::lbledtInputerQtyClick(TObject *Sender)
{
    //Kirin 20220127 人員可以輸入Loader輸入數量
    //==>
//    TEdit *Ptr;
//    Ptr=(TEdit*)Sender;
//    int iBackupNum=atoi(Ptr->Text.c_str());
//
//    MyInputBox( (TEdit*)Sender);
    //<==
    //Kirin 20220127 人員可以輸入Loader輸入數量
}
//---------------------------------------------------------------------------
void __fastcall TfServerFrm::frameProdInfo1ShowTimerTimer(TObject *Sender)
{
    frameProdInfo1->ShowTimerTimer(Sender);
}
//---------------------------------------------------------------------------

