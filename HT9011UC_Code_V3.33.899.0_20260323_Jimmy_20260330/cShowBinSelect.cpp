#include "MachineDefine.h"
#pragma hdrstop

#include "cShowBinSelect.h"
#include "myQwertyKeyBoard.h"
#include "cprod.h"
#include "main.h"
#include "myswitch.h"
#include "cContact.h"
#include "mymessbox.h"
#include "iosetview.h"
#include "database.h"
#include "cpublic.h"
#include "csystem.h"
#include "cSortCT.h"
#include "cCounterClear.h"
#include "cBinSel.h"
#include "note.h"
#include "AutoClean.h"
#include "cMyDB.h"
#include "MyMotor.h"
#include "atester.h"
#include "cSocket.h"
#include "uCleaning.h"
#include "cSecurity.h"
#include "cmydef.h"
#include "MyKitSuck.h"
#include "common.h"
#include "uYieldMonitoring.h"
#include "atester_ProcessCount.h"
#include "uLotInfo.h"
#include "FixAICCD.h"
#include "database.h"
#include "AGV.h"
#include "UsecegemMainFrom.h"
#include "uHGemHT9045.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "HTray"
#pragma resource "*.dfm"
TfShowBinSelect *fShowBinSelect;
//---------------------------------------------------------------------------
__fastcall TfShowBinSelect::TfShowBinSelect(TComponent* Owner)
    : TForm(Owner)
{
    TLabel *tempMyBinSel[]=
    {
        laAuto1, laAuto2, laAuto3, laAuto4, laAuto5, laAuto6,
        laFix1,  laFix2,  laFix3,  laFix4,  laFix5,  laFix6,
        laFix7,  laFix8,  laFix9,  laFix10, laFix11, laFix12, laBinBox,
        laMag1,  laMag2,  laMag3,  laMag4,  laMag5,  laMag6,  laMag7,          //JerryYang 20220909 : add magazine
        laMag8,  laMag9,  laMag10, laMag11, laMag12, laMag13, laMag14
    };

    TLabel *tempMyBinSelLab[]=                                                  //JerryYang 20220909 : add magazine
    {
        labAuto1, labAuto2, labAuto3, labAuto4, labAuto5, labAuto6,
        labFix1,  labFix2,  labFix3,  labFix4,  labFix5,  labFix6,
        labFix7,  labFix8,  labFix9,  labFix10, labFix11, labFix12, labBinBox,
        labMag1,  labMag2,  labMag3,  labMag4,  labMag5,  labMag6,  labMag7,
        labMag8,  labMag9,  labMag10, labMag11, labMag12, labMag13, labMag14
    };

    TLabel *tempMyBinSelARTFT[]=                                                //JerryYang 20220331 : �n���n�Dshow ART BIN
    {
        laArtFtAuto1, laArtFtAuto2, laArtFtAuto3, laArtFtAuto4, laArtFtAuto5, laArtFtAuto6,
        laArtFtFix1,  laArtFtFix2,  laArtFtFix3,  laArtFtFix4,  laArtFtFix5,  laArtFtFix6,
        laArtFtFix7,  laArtFtFix8,  laArtFtFix9,  laArtFtFix10, laArtFtFix11, laArtFtFix12, laArtFtBinBox,
        laArtFtMag1,  laArtFtMag2,  laArtFtMag3,  laArtFtMag4,  laArtFtMag5,  laArtFtMag6,  laArtFtMag7,
        laArtFtMag8,  laArtFtMag9,  laArtFtMag10, laArtFtMag11, laArtFtMag12, laArtFtMag13, laArtFtMag14
    };

    TLabel *tempMyBinSelARTFTLab[]=
    {
        labArtFtAuto1, labArtFtAuto2, labArtFtAuto3, labArtFtAuto4, labArtFtAuto5, labArtFtAuto6,
        labArtFtFix1,  labArtFtFix2,  labArtFtFix3,  labArtFtFix4,  labArtFtFix5,  labArtFtFix6,
        labArtFtFix7,  labArtFtFix8,  labArtFtFix9,  labArtFtFix10, labArtFtFix11, labArtFtFix12, labArtFtBinBox,
        labArtFtMag1,  labArtFtMag2,  labArtFtMag3,  labArtFtMag4,  labArtFtMag5,  labArtFtMag6,  labArtFtMag7,
        labArtFtMag8,  labArtFtMag9,  labArtFtMag10, labArtFtMag11, labArtFtMag12, labArtFtMag13, labArtFtMag14
    };

    TLabel *tempMyBinSelARTRT[]=
    {
        laArtRtAuto1, laArtRtAuto2, laArtRtAuto3, laArtRtAuto4, laArtRtAuto5, laArtRtAuto6,
        laArtRtFix1,  laArtRtFix2,  laArtRtFix3,  laArtRtFix4,  laArtRtFix5,  laArtRtFix6,
        laArtRtFix7,  laArtRtFix8,  laArtRtFix9,  laArtRtFix10, laArtRtFix11, laArtRtFix12, laArtRtBinBox,
        laArtRtMag1,  laArtRtMag2,  laArtRtMag3,  laArtRtMag4,  laArtRtMag5,  laArtRtMag6,  laArtRtMag7,
        laArtRtMag8,  laArtRtMag9,  laArtRtMag10, laArtRtMag11, laArtRtMag12, laArtRtMag13, laArtRtMag14
    };

    TLabel *tempMyBinSelARTRTLab[]=
    {
        labArtRtAuto1, labArtRtAuto2, labArtRtAuto3, labArtRtAuto4, labArtRtAuto5, labArtRtAuto6,
        labArtRtFix1,  labArtRtFix2,  labArtRtFix3,  labArtRtFix4,  labArtRtFix5,  labArtRtFix6,
        labArtRtFix7,  labArtRtFix8,  labArtRtFix9,  labArtRtFix10, labArtRtFix11, labArtRtFix12, labArtRtBinBox,
        labArtRtMag1,  labArtRtMag2,  labArtRtMag3,  labArtRtMag4,  labArtRtMag5,  labArtRtMag6,  labArtRtMag7,
        labArtRtMag8,  labArtRtMag9,  labArtRtMag10, labArtRtMag11, labArtRtMag12, labArtRtMag13, labArtRtMag14
    };

    TLabeledEdit *tempAi[]=
    {
         edAiAuto1,  edAiAuto2,  edAiAuto3,  edAiAuto4,  edAiAuto5,  edAiAuto6,
         edlAiFix1,  edlAiFix2,  edlAiFix3,  edlAiFix4,  edlAiFix5,  edlAiFix6,
         edlAiFix7,  edlAiFix8,  edlAiFix9,  edlAiFix10, edlAiFix11, edlAiFix12, edlAiBinBox,
         edlAiMag1,  edlAiMag2,  edlAiMag3,  edlAiMag4,  edlAiMag5,  edlAiMag6,  edlAiMag7,
         edlAiMag8,  edlAiMag9,  edlAiMag10, edlAiMag11, edlAiMag12, edlAiMag13, edlAiMag14
    };

    TGroupBox *tempGrpBinDisp[]=                                                //JerryYang 20220909 : add magazine
                                {gbAuto1, gbAuto2, gbAuto3, gbAuto4, gbAuto5, gbAuto6,
                                 gbFix1,  gbFix2,  gbFix3,  gbFix4,  gbFix5,  gbFix6,
                                 gbFix7,  gbFix8,  gbFix9,  gbFix10, gbFix11, gbFix12, gbBinBox,
                                 gbMag1,  gbMag2,  gbMag3,  gbMag4,  gbMag5,  gbMag6,  gbMag7,
                                 gbMag8,  gbMag9,  gbMag10, gbMag11, gbMag12, gbMag13, gbMag14};

    ColorRed    =1;                                                             //1:red, 2:green, 3:orange
    ColorGreen  =2;
    ColorOrange =3;

    for(int i=0; i<eTrayCount; i++)
    {
        MyBinSel[i]         =tempMyBinSel[i];
        MyBinSelLab[i]      =tempMyBinSelLab[i];                                //JerryYang 20220909 : add magazine

        MyBinSelARTFT[i]    =tempMyBinSelARTFT[i];                              //JerryYang 20220331 : �n���n�Dshow ART BIN
        MyBinSelARTFTLab[i] =tempMyBinSelARTFTLab[i];
        MyBinSelARTRT[i]    =tempMyBinSelARTRT[i];
        MyBinSelARTRTLab[i] =tempMyBinSelARTRTLab[i];
        grpBinDisp[i]       =tempGrpBinDisp[i];
        EditAi[i]           =tempAi[i];
        tempAi[i]->Align    =alTop;
    }

    bShow=false;
    tsUPH=new TStringList();

    for(int i=0; i<20; i++)                                                     //Steven 20140621 : For SECS GEM
    {
        tsUPH->Add("");
    }

    for(int i=0; i<eTrayCount; i++)
    {
        sBinCode_ATK[i] ="";
    }

    ShowInitialString();

    btReturn->Align=alBottom;
}
//---------------------------------------------------------------------------
void __fastcall TfShowBinSelect::FormDestroy(TObject *Sender)
{
    try
    {
        TimerAutoCleanCount->Enabled=false;                                     //Steven 20160727
        tsUPH->Clear();                                                         //Ifor 20170603 (wei) TStringList �R���e�� Clean
        delete tsUPH;                                                           //Steven 20160108 : release memory
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfShowBinSelect::FormDestroy");
    }
    LogSoftwareOffTime("TfShowBinSelect, FormDestroy");                         //Steven 20210526 : �����n�����ɶ�
}
//---------------------------------------------------------------------------
AnsiString DelDot(AnsiString asBuffer)
{
    AnsiString asReturn;
    int iBegin=0;
    for(int i=0; i<iTestBinCount; i++)                                          //kevin 20140317 256 bin
    {
        if(i==1)
        {
            iBegin=asBuffer.Pos(AnsiString(i)+" ");
            if(iBegin!=1)
                continue;
        }
        else
            iBegin=asBuffer.Pos(" "+AnsiString(i)+" ");
        if(iBegin>0)
        {
            break;
        }
    }

    if(iBegin==0)
    {
        iBegin=asBuffer.Pos(AnsiString("E"));
    }

    asReturn=asBuffer.Delete(1, iBegin-1);
    return asReturn;
}
//---------------------------------------------------------------------------
TQPF_Timer AlarmDelay;
void __fastcall TfShowBinSelect::ChangeBinDispStatus()                          //Steven 20110411 : ��ܥثe��ܾ������A
{
    TPanel *UnLoadPanel[]={pnlLoader, pnlEmpty, pnlColor,
                           pnlAuto1,  pnlAuto2, pnlAuto3,
                           pnlFix1,   pnlFix2,  pnlFix3,  pnlFix4,  pnlFix5,  pnlFix6,  pnlBinBox,
                           pnlMag1,   pnlMag2,  pnlMag3,  pnlMag4,  pnlMag5,  pnlMag6,  pnlMag7,
                           pnlMag8,   pnlMag9,  pnlMag10, pnlMag11, pnlMag12, pnlMag13, pnlMag14,
                           pnlAuto4,  pnlAuto5, pnlAuto6,
                           pnlFix7,   pnlFix8,  pnlFix9,  pnlFix10, pnlFix11, pnlFix12};  //JerryYang 20220909 : add magazine

    TColor ColorMap[]={clGray, clRed, clGreen, (TColor)0x000080FF, clBlack};
    int iColor, iBin;
    AnsiString sTempEng="", sTempChi="";
    static bool bChangeColor=false;
    static int iAlarmFlag=0;
    bool bHasError=false, bErrFlag[eBinDispTotal];                              //JerryYang 20220909 : 12->eBinDispTotal

    if(InitialOK==false)
    {
        return;
    }

    for(int i=0; i<eBinDispTotal; i++)                                          //JerryYang 20220909 : 12->eBinDispTotal
    {
        if(HSys.BinDisCtrl->UnitHasInstall(i))
        {
            bErrFlag[i]=HSys.BinDisCtrl->GerErrNow(i);
        }
        else
        {
            bErrFlag[i]=false;
        }
    }
    sTempChi="";                                                                //Jimmychiu 20231031 : add Bin display error record
    for(int i=0; i<eBinDispTotal; i++)                                          //JerryYang 20220909 : 12->eBinDispTotal
    {
        if(AUTO3_IS_MAGAZINE==0 && i>=eBinDispMag1 && i<=eBinDispMag14)         //JerryYang 20230515 : �S��Magazine�N���n���?
        {
        }
        else if(AUTO3_IS_MAGAZINE==1 && i==eBinDispAuto3)
        {

        }
        else if(i==eBinDispBulkBox ||                                           //Jimmychiu 20231030 : bin Box not display
                i>=eBinDispAuto4)
        {
        }
        else if(bErrFlag[i] ||
           (i>=3 && HSys.BinDisCtrl->UnitHasInstall(i)==false))                 //Steven 20211221 : Loader, empty, color�i��S��
        {
            bHasError=true;
            sTempChi=sTempChi+" "+UnLoadPanel[i]->Name;                         //Jimmychiu 20231031 : add Bin display error record
            if(iAlarmFlag==0)
            {
                iAlarmFlag=1;                                                   //Steven 20211130 : JSCC�n�DBin��ܾ����`�nalarm
            }
        }
    }

    if(bHasError)
    {
        bChangeColor=!bChangeColor;
        PageControl1->ActivePageIndex==3;                                       //tsUnloadMap
    }
    else
    {
        iAlarmFlag=0;                                                           //Steven 20211130 : JSCC�n�DBin��ܾ����`�nalarm
    }

    if(IniConfig.bG16BinDispNeedAlarm)                                          //Steven 20211220 : �ץ�G16, Bin��ܾ����`�nalarm
    {
        ;
    }
    else if(PageControl1->ActivePage!=tsUnloadMap)
    {
        if(CUSTOMER_CODE==CC_KYEC_LEE || CUSTOMER_CODE==CC_AMD_M)               //Ifor 20221026 add: KYEC �n�D�@������
        {

        }
        else
        {
            return;
        }
    }

    for(int i=0; i<eBinDispTotal; i++)                                          //JerryYang 20220909 : 12->eBinDispTotal
    {
        if(HSys.BinDisCtrl->UnitHasInstall(i))
        {
            iColor=HSys.BinDisCtrl->GetColorNow(i);
            iBin=HSys.BinDisCtrl->GetBinNow(i);

            if(bErrFlag[i]==false)
            {
                UnLoadPanel[i]->Color=ColorMap[iColor];
                if(iBin==111)
                {
                    UnLoadPanel[i]->Caption="L";
                }
                else if(iBin==104)
                {
                    UnLoadPanel[i]->Caption="E";
                }
                else if(iBin==102)
                {
                    UnLoadPanel[i]->Caption="C";
                }
                else if(iBin==123 || iBin==-1)
                {
                    UnLoadPanel[i]->Caption="X";
                }
                else
                {
                    UnLoadPanel[i]->Caption=AnsiString(iBin);
                }
            }
            else                                                                //�������ܷ|�ܰ{�{!!
            {
                if(bChangeColor)
                    UnLoadPanel[i]->Color=ColorMap[4];
                else
                    UnLoadPanel[i]->Color=ColorMap[1];
            }
        }
        else
        {
            UnLoadPanel[i]->Color=clGray;
            UnLoadPanel[i]->Caption="X";
        }
    }

    if((CUSTOMER_CODE==CC_KYEC_LEE || CUSTOMER_CODE==CC_AMD_M) && bBinDispAlarm==false)//Ifor 20220714 add:Bin Disp ���`��ĵ �C��Onecycle �ˬd�@��
    {
        if(bHasError && fHome->fShow==false)
        {
            bBinDispAlarm=true;
            #ifndef SOFT_SIMULTE
//            HSys.BinDisCtrl->CommBin->StopComm();
            HSys.BinDisCtrl->bFirstInit=true;
            HSys.BinDisCtrl->ProcessStopStart(true);
            ShowMyMessage("Please check bin display. It have communication error!", "�нT�{Bin��ܾ������A�C");
            #endif
        }
    }
    else if(IniConfig.bG16BinDispNeedAlarm)                                          //Steven 20211130 : JSCC�n�DBin��ܾ����`�nalarm
    {
        if(iAlarmFlag==1)
        {
            AlarmDelay.SetSecAndOn(60);
            iAlarmFlag=2;
        }

        if(iAlarmFlag==2 && AlarmDelay.Off())
        {
            #ifndef SOFT_SIMULTE
            sTempEng=AnsiString().sprintf("%s Error part:%s", "Please check bin display. It have communication error!", sTempChi);
            sTempChi=AnsiString().sprintf("%s ���`��m:%s", "�нT�{Bin��ܾ������A!", sTempChi);
            HSys.BinDisCtrl->CommBin->StopComm();
            HSys.BinDisCtrl->bFirstInit=true;
            HSys.BinDisCtrl->ProcessStopStart(true);
            ShowMyMessage(sTempEng, sTempChi);
            AlarmDelay.SetSecAndOn(60);
            iAlarmFlag=0;
            #endif
        }
    }

    if(bHasError)
    {
        sbRunStatus->Panels->Items[0]->Text="Bin display got error!!";
        sbRunStatus->Color=clRed;
    }
    else
    {
        sbRunStatus->Panels->Items[0]->Text=HSys.BinDisCtrl->GetRunStatus();
        sbRunStatus->Color=clBtnFace;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfShowBinSelect::ShowBinSel()
{
    int Data, iLengh=0, i2=0;
    bool iRecord[100]={false};
    AnsiString S[eTrayCount], asBuf;

    TPanel *UnLoadPanel[]={pnlAuto1, pnlAuto2, pnlAuto3, pnlAuto4, pnlAuto5, pnlAuto6,
                           pnlFix1,  pnlFix2,  pnlFix3,  pnlFix4,  pnlFix5,  pnlFix6,
                           pnlFix7,  pnlFix8,  pnlFix9,  pnlFix10, pnlFix11, pnlFix12, pnlBinBox,
                           pnlMag1,  pnlMag2,  pnlMag3,  pnlMag4,  pnlMag5,  pnlMag6,  pnlMag7,
                           pnlMag8,  pnlMag9,  pnlMag10, pnlMag11, pnlMag12, pnlMag13, pnlMag14};  //JerryYang 20220909 : add magazine

    TLabel *UnLoadLabel[]={lblAuto1, lblAuto2, lblAuto3, lblAuto4, lblAuto5, lblAuto6,
                           lblFix1,  lblFix2,  lblFix3,  lblFix4,  lblFix5,  lblFix6,
                           lblFix7,  lblFix8,  lblFix9,  lblFix10, lblFix11, lblFix12, lblBinBox,
                           lblMag1,  lblMag2,  lblMag3,  lblMag4,  lblMag5,  lblMag6,  lblMag7,
                           lblMag8,  lblMag9,  lblMag10, lblMag11, lblMag12, lblMag13, lblMag14};   //JerryYang 20220909 : add magazine

    if(IniConfig.bVTESTFunction==true)                                          //RogerYang 20250224 �����ݨD �s�W�T�C�H��,�Ӯ�,�ƶq,site
    {
        UPH_StringGrid->ColCount=7;
    }

    for(int i=0; i<eTrayCount; i++)                                             //JerryYang 20220909 : BinTrayTotal->eTrayCount
    {
        S[i]="";                                                                //JerryYang 20230512 : �w�]���ŭ�
        BinAssign[i]="";
        bUnloadHasBin[i]=false;                                                 //kevin 20180705 ��BIN TRAY set true
        iTrayLastBin[i]=0;                                                      //kevin 20180705 �C�@��TRAY�̫�@��BIN
    }

    for(int j=0; j<eTrayCount; j++)                                             //JerryYang 20220909 : iBinSelCT->eTrayCount
        iBinTray[j]=false;                                                      //kevin 20170328 (Steven) add �P�_TRAY �O�_�ϥ� BIN

    for(int i=0; i<iTestBinCount; i++)
    {
        Data=Prod.iT6CatData[i];
        if(Data<0)                                                              //kevin 20140317 256 bin 0 start
            continue;

        for(int j=0; j<eTrayCount; j++)
        {
            if(grpBinDisp[j]->Visible==false)
                continue;

            if(j==Data)
            {
                if(fBinSel->chkShow0Xbin->Checked)                              //jou 20220719 : show 0X bin
                {
                    asBuf.sprintf("%02d ", i);
                    S[j]+=asBuf;
                }
                else
                {
                    if(IniConfig.bSPILFunction==true &&
                       CosFunction.bSortingBy2DList==true &&
                       LastSet.iTester==_2D_SORT &&
                       TestIF_File.bSortingBy2DIDList==true)                    //JerryYang 20230322 : SPIL���sIT�n�D
                    {
                        if(S[j]=="")
                        {
                            S[j]=AnsiString(i);                                 //kevin 20140317 bin 0 start
                        }
                        else
                        {
                            S[j]+=" "+AnsiString(i);                            //kevin 20140317 bin 0 start
                        }
                    }
                    else
                    {
                        S[j]+=AnsiString(i)+" ";                                //kevin 20140317 bin 0 start
                    }
                }

                sBinData[j]     =AnsiString(i)+" ";                             //kevin 20180202 record bin
                iBinTray[j]     =true;                                          //kevin 20170328 (Steven) add �P�_TRAY �O�_�ϥ� BIN
                iLengh          =S[j].Length();
                BinAssign[j]   +=AnsiString(i)+",";                             //kevin 20180705 add Auto 123 fix 123 bin data
                bUnloadHasBin[j]=true;                                          //kevin 20180705 ��BIN TRAY set true
                iTrayLastBin[j] =i;                                             //kevin 20180705 �C�@��TRAY�̫�@��BIN
                i2              =iLengh/460;
                if(i2!=0)
                {
                    if(iRecord[i2]==false)
                    {
                        iRecord[i2]  =true;
                        S[j]        +="\n\r";
                        sBinData[j]  ="\n\r";                                   //kevin 20180202 record bin
                        BinAssign[j]+="\n\r";                                   //kevin 20180705 add Auto 123 fix 123 bin data
                    }
                }
            }
            else
            {
                if(CosFunction.bSortingBy2DList==true &&
                   LastSet.iTester==_2D_SORT &&
                   TestIF_File.bSortingBy2DIDList==true)                        //Frank 20221122 : 2DID sorting for ATK
                {

                }
                else if(IniConfig.bSPILFunction==true)                          //JerryYang 20250320 : SPIL��γr�����j
                {
                }
                else
                {
                    if(iTestBinCount<=16)                                       //jou 2014-04-30 15 Bin �^�_ ... ����n�\Ū
                    {
                        S[j]        +=". ";
                        BinAssign[j]+=",";                                      //kevin 20180705 add Auto 123 fix 123 bin data
                    }
                }
            }
        }
    }

    Data=Prod.iIfErrorT6;
    if(TestIF_File.iTestType==RS232_MODE &&
       TestIF_File.iRs232Mode==eRs23232Bin)                                     //Steven 20121112 : RS232�䴩32Bin
    {
        if(Data>0)
        {
            S               [Data] =" E"+S[Data];
            sBinData        [Data] =" E"+S[Data];                               //kevin 20180202 record bin
            iBinTray        [Data] =true;                                       //kevin 20170328 (Steven) add �P�_Fix TRAY �O�_�ϥ� BIN
            BinAssign       [Data]+="E";                                        //kevin 20180705 add Auto 123 fix 123 bin data
            bUnloadHasBin   [Data] =true;                                       //kevin 20180705 ��BIN TRAY set true
            iTrayLastBin    [Data] =999;                                        //kevin 20180705 �C�@��TRAY�̫�@��BIN  Error bin
        }
    }
    else
    {
        if(iHWFix_BinBox==1 || Data==-1)                                        //kevin 20160819 //Steven 20230929 : && --> ||
        {
            S               [eBulkBox]+="E ";
            sBinData        [eBulkBox]+="E ";                                   //kevin 20180202 record bin
            iBinTray        [eBulkBox] =true;                                   //kevin 20170328 (Steven) add �P�_Fix TRAY �O�_�ϥ� BIN
            BinAssign       [eBulkBox]+="E";                                    //kevin 20180705 add Auto 123 fix 123 bin data
            bUnloadHasBin   [eBulkBox] =true;                                   //kevin 20180705 ��BIN TRAY set true
            iTrayLastBin    [eBulkBox] =999;                                    //kevin 20180705 �C�@��TRAY�̫�@��BIN  Error bin
        }
        else if(Data>0)
        {
            if(IniConfig.bSPILFunction==true &&
               CosFunction.bSortingBy2DList==true &&
               LastSet.iTester==_2D_SORT &&
               TestIF_File.bSortingBy2DIDList==true)                            //JerryYang 20230322 : SPIL���sIT�n�D
            {
                S           [Data]+="ERR";
            }
            else
            {
                S           [Data]+="E ";
            }
            sBinData        [Data]+="E ";                                       //kevin 20180202 record bin
            iBinTray        [Data] =true;                                       //kevin 20170328 (Steven) add �P�_Fix TRAY �O�_�ϥ� BIN
            BinAssign       [Data]+="E";                                        //kevin 20180705 add Auto 123 fix 123 bin data
            bUnloadHasBin   [Data] =true;                                       //kevin 20180705 ��BIN TRAY set true
            iTrayLastBin    [Data] =999;                                        //kevin 20180705 �C�@��TRAY�̫�@��BIN  Error bin
            bUnloadHasBin   [Data] =false;                                      //kevin 20220906 ��BIN TRAY set true
        }
    }

//    if(CosFunction.bSortingBy2DList==true &&                                  //JerryYang 20250505 : Mark��
//       LastSet.iTester==_2D_SORT &&
//       TestIF_File.bSortingBy2DIDList==true)
//    {
//
//    }
//    else
//    {
        if(iTestBinCount<=16)
        {
            for(int i=0; i<eTrayCount; i++)                                     //JerryYang 20220909 : BinTrayTotal->eTrayCount                       //kevin 20140317 bin
            {
                S[i]+=". . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . ";
                if(IniConfig.bSPILFunction==true)                               //JerryYang 20250320 : SPIL��γr�����j
                {
                    AnsiString strTemp=BinAssign[i];
                    int len=strTemp.Length();

                    if (len>0 && strTemp[len]==',')
                    {
                        BinAssign[i]=BinAssign[i].SubString(1, len-1);
                    }
                }
                else
                {
                    BinAssign[i]+=",";                                          //kevin 20180705 add Auto 123 fix 123 bin data
                }
            }
        }
//    }

    for(int i=eFix2; i<=iFixMax; i++)
    {
        if(Prod.bLinkTo6Tray[i]==true)
        {
            if(S[i-1].AnsiPos("LINK")==0)
                S[i]=AnsiString("LINK ")+S[i-1];
            else
                S[i]=S[i-1];
            BinAssign[i]=BinAssign[i-1];
        }
    }

    //Ifor 20231122 add Magazine Link
    //==>
    if(AUTO3_IS_MAGAZINE==1)
    {
        for(int i=1; i<14; i++)
        {
            if(BinSelect[iTestRunMode].bMagazineLink[i]==true)
            {
                for(int j=0; j<14; j++)
                {
                    if(BinSelect[iTestRunMode].bMagazineLink[i-j]==false)
                    {
                        S[eMag1+i]=AnsiString("LINK ")+S[eMag1+(i-j)];
//                        S[10+i]=S[10+(i-j)];
                        BinAssign[eMag1+i]=BinAssign[eMag1+(i-j)];
                        break;
                    }
                }
            }
        }
    }
    //<==
    //Ifor 20231122 add Magazine Link

    for(int i=0; i<eTrayCount; i++)                                             //JerryYang 20220909 : 10->eTrayCount  //kevin 20160819  //kevin 20110901�ϥ�FIX��2TRAY
    {
        MyBinSel[i]->Caption=S[i];
        if(S[i].Pos("LINK ")>0)
        {
            UnLoadLabel[i]->Caption=DelDot(MyBinSel[i]->Caption.SubString(6,MyBinSel[i]->Caption.Length()));
        }
        else
        {
            UnLoadLabel[i]->Caption=DelDot(MyBinSel[i]->Caption);
        }

        int j=MyBinSel[i]->Caption.Pos(" . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . ");

        if(iHWFix_BinBox==1 && i==eBulkBox)                                     //kevin 20160825
            continue;

        if(Prod.iTrayType[i]==tNotUse)
            continue;

        if(iTestBinCount<=16 && j==1)
        {
            UnLoadPanel[i]->Color                           =clGray;
            UnLoadLabel[i]->Font->Color                     =clGray;
            fSortCT->myCountPanel[i].pnlYield->Font->Color  =clGray;
            MyBinSel[i]->Font->Color                        =clGray;
            fSortCT->myCountPanel[i].pnlCount->Font->Color  =clGray;            //kevin 20150615
        }
        else
        {
            UnLoadPanel[i]->Color                           =tcBinColor[Prod.iIsFailT6[i]];
            UnLoadLabel[i]->Font->Color                     =tcBinColor[Prod.iIsFailT6[i]];
            fSortCT->myCountPanel[i].pnlYield->Font->Color  =tcBinColor[Prod.iIsFailT6[i]];
            MyBinSel[i]->Font->Color                        =tcBinColor[Prod.iIsFailT6[i]];
            fSortCT->myCountPanel[i].pnlCount->Font->Color  =tcBinColor[Prod.iIsFailT6[i]]; //kevin 20150615
        }

        MyBinSel[i]->Caption=S[i];                                              //JerryYang 20250320 : SPIL��γr�����j
        if(IniConfig.bSPILFunction==true)
        {
            sSVBinAssign[i]=BinAssign[i];
        }
        else if(fAGV->IsATK_AMR())
        {
            sSVBinAssign[i]=BinAssign[i];
            if(sBinCode_ATK[i]!=BinAssign[i])
            {
                sBinCode_ATK[i]=BinAssign[i];
                EventReport(SECS_EVENT.OutputPort1BinCode+i);
            }
        }
        else
        {
            sSVBinAssign[i]=MyBinSel[i]->Caption;
        }
    }

    if(AUTO3_IS_MAGAZINE!=1)
    {
        for(int i=eMag1; i<eMag1+14; i++)
        {
            UnLoadLabel[i]->Caption=". . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . ";
            UnLoadPanel[i]->Caption="X";
            if(NUMBER_PANEL_TYPE==3)    //�T����ܾ�?
            {
                UnLoadPanel[i]->Color=(TColor)0x000080FF;
                UnLoadLabel[i]->Font->Color=(TColor)0x000080FF;
            }
            else
            {
                UnLoadPanel[i]->Color=clGray;
                UnLoadLabel[i]->Font->Color=clGray;
            }
        }
    }

    if(TrayForm.iFixTrayMode==false)                                            //�n��ѤU���T�ӧ令���?
    {
        for(int i=iFixRight+1; i<=iFixRightHalf; i++)
        {
            UnLoadLabel[i]->Caption=". . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . ";
            UnLoadPanel[i]->Caption="X";
            if(NUMBER_PANEL_TYPE==3 ||                                          //�T����ܾ�?
               NUMBER_PANEL_TYPE==4)                                            //Sam 20240604 : �s�W BinDisplay TFT
            {
                UnLoadPanel[i]->Color=(TColor)0x000080FF;
                UnLoadLabel[i]->Font->Color=(TColor)0x000080FF;
            }
            else
            {
                UnLoadPanel[i]->Color=clGray;
                UnLoadLabel[i]->Font->Color=clGray;
            }
        }
    }

    if(AUTO_EMPTY_COLOR!=0)  //���y
    {
        pnlEmpty->Color=(TColor)0x000080FF;
        pnlColor->Color=(TColor)0x000080FF;
    }
    else
    {
        pnlEmpty->Color=clGray;
        pnlColor->Color=clGray;
    }

    if(IniConfig.bAutoTrayLink==true)   //jou 2012-06-14 Auto Tray Link
    {
        for(int i=eAuto2; i<=iAutoRight; i++)
        {
            if(Prod.bLinkTo6Tray[i]==true)
            {
                if(MyBinSel[i-1]->Caption.AnsiPos("LINK")==0)
                    MyBinSel[i]->Caption=AnsiString("LINK ")+MyBinSel[i-1]->Caption;
                else
                    MyBinSel[i]->Caption=MyBinSel[i-1]->Caption;
            }
        }
    }

    ShowBinSel_ARTNor();                                                        //JerryYang 20220331 : �n���n�Dshow ART BIN
    ShowBinSel_ARTRT();

    PageControl1Change(this);
}
//---------------------------------------------------------------------------
void __fastcall TfShowBinSelect::FormShow(TObject *Sender)
{
    if(NUMBER_PANEL_TYPE==3 ||
       NUMBER_PANEL_TYPE==4)                                                    //Sam 20240604 : �s�W BinDisplay TFT
        bUpdateBinDigital=true;
    else
        fShowBinSelect->tsUnloadMap->TabVisible=false;                          //Steven 20110411

    if(CUSTOMER_CODE==CC_KYEC_LEE)                                              //wei 20150904
        ed_AutoCleanCount->Visible=false;
    else
        ed_AutoCleanCount->Visible=true;

    if(CUSTOMER_CODE==CC_KYEC_LEE)                                              //wei 20151111 �����clean
        btnClearCount->Visible=false;
    else
        btnClearCount->Visible=true;

    if(CUSTOMER_CODE==CC_Greatek)                                               //JimmyChiu 20220124 : �W�׭n�D������ܦW��
    {
        fShowBinSelect->palUnloader->Width=110;
        labAuto1->Caption="Auto1 (CAT A)";
        labAuto2->Caption="Auto2 (CAT B)";
        labAuto3->Caption="Auto3 (CAT C)";
        labFix1->Caption ="Fix1 (CAT D)";
        labFix2->Caption ="Fix2 (CAT E)";
        labFix3->Caption ="Fix3 (JAM)";
    }

    tsFxiAI->TabVisible=USE_Fix_AI_CCD;

    ShowBinSel();
    bShow=true;
    PageControl1->ActivePage=tsTestBin;
    PageControl1Change(this);
    Tab_UPH->TabVisible         =IniConfig.bShowUPH;                            //Steven 20100827
    btnAutoClean->Visible       =IniConfig.bEnableAutoCleanFunction;
    tsASE->TabVisible           =IniConfig.bASE_Report;                         //kevin 20141106 �������� �qSchedule NUM
    PageControl1_ART->TabVisible=(USE_AUTO_RETEST==eartInstall &&               //kevin 201506016
                                  IniConfig.bA10_AutoReTest);                   //wei 20150331 ���}�\��N���?

    if(CosFunction.bUseARTSortCount==true)                                      //Ifor 20170321 add MRT Mode
    {
        PageControl1_ART->TabVisible=true;
        PageControl1_ART->Caption="Category Info AUTO";
    }
    if(CUSTOMER_CODE==CC_KYEC_LEE)                                              //Ifor 20180313 :add �ʤ��촼�n�D Clean Reset ���� �ݨD Auto Clean �]�w�����M��
    {
        btnCleanReset->Visible=false;
    }
    else
    {
        btnCleanReset->Visible=IniConfig.bEnableAutoCleanFunction;
    }
    if(CUSTOMER_CODE==CC_UNISEM_M)
    {
        labJamrate->Visible =true;
        Jamrate->Visible    =true;
    }
    Panel1->Visible=CosFunction.IntervalYieldCount;                             //wei 20180606 Interval Low Yield By Site

    pnlSpeciallYield->Visible  =CosFunction.bSpecailLowYeild;                   //Sam 20210505 : PTI �n�D����q Low Yeild

    if(IniConfig.bG15LoadInputCount)                                            //kevin 20211106 ��J���ƹF���N Clean out    //JimmyChiu 20220124 fix name
    {
        PLoadInput->Visible=true;
        PLoadInput->Top =120;
        PLoadInput->Left=14;
    }
    else
    {
        PLoadInput->Visible=false;
    }

    tsCategoryInfoContCT->TabVisible=CosFunction.bCategoryInfoByContactCT;       //Sam 20240131 : �s�W�@�� CategoryInfo ������ƨϥ�? ContactCT
    gbTriggerAlm->Visible=IniConfig.bSPILFunction;                              //RogerYang 20260105 Jimmy Add.
    SetAutoVisible();
    if(CUSTOMER_CODE==CC_GIGAS)
    {
        if(palAutoDeviceEjection) palAutoDeviceEjection->Visible=true;
    }
    else
    {
        if(palAutoDeviceEjection) palAutoDeviceEjection->Visible=false;
    }

    //AI(ht9045-v899) 20260415: dynamic stack layout for tsIndex tab to prevent overlapping panels
    int iNextTop = gbAutoCleanCount->Top + gbAutoCleanCount->Height + 4;
    if(Panel1->Visible)
    {
        Panel1->Top = iNextTop;
        iNextTop += Panel1->Height + 4;
    }
    if(pnlSpeciallYield->Visible)
    {
        pnlSpeciallYield->Top = iNextTop;
        iNextTop += pnlSpeciallYield->Height + 4;
    }
    if(gbTriggerAlm->Visible)
    {
        gbTriggerAlm->Top = iNextTop;
        iNextTop += gbTriggerAlm->Height + 4;
    }
    if(palAutoDeviceEjection && palAutoDeviceEjection->Visible)
    {
        palAutoDeviceEjection->Top = iNextTop;
        iNextTop += palAutoDeviceEjection->Height + 4;
    }
    if(gbCopyRecipe && gbCopyRecipe->Visible)
    {
        gbCopyRecipe->Top = iNextTop;
        iNextTop += gbCopyRecipe->Height + 4;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfShowBinSelect::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    bShow=false;
}
//---------------------------------------------------------------------------
void __fastcall TfShowBinSelect::InitShowBinDigital()
{
    iShowBinDigitalTask=1;
    bUpdateBinDigital=true;
}
//---------------------------------------------------------------------------
void __fastcall TfShowBinSelect::ShowBinDigital()
{
    if(NUMBER_PANEL_TYPE==3 ||
       NUMBER_PANEL_TYPE==4)                                                    //Sam 20240604 : �s�W BinDisplay TFT
        return;

    if(NUMBER_PANEL_TYPE!=0)                                                    //Steven 20091001
    {
        int i, j;
        int iArray[12];
        //Steven 20090917 Start
        for(i=0; i<12; i++)
            iArray[i]=-1;
        if(NUMBER_PANEL_TYPE==2)        //2 Digital
        {
            iArray[0]=111;     //L
            iArray[1]=104;     //E
            iArray[2]=102;     //C
        }
        else if(NUMBER_PANEL_TYPE==1)   //1 Digital
        {
            iArray[0]=21;      //L
            iArray[1]=14;      //E
            iArray[2]=12;      //C
        }
        //Steven 20090917 End
        for(i=0; i<12; i++)
            iShowAutoBin[i]=iArray[i];
        for(i=3; i<12; i++)
        {
            for(j=0; j<iTestBinCount; j++)
            {
                if(iShowAutoBin[i]==-1)
                {
                    if(Prod.iT6PosCate[j]==(i-2))
                       // iShowAutoBin[i]=j+1;
                        iShowAutoBin[i]=j;//kevin 20140324 bin 0
                }
            }
        }

        if(IniConfig.bAutoTrayLink==true)                                       //jou 2012-06-14 Auto Tray Link
        {
            if(Prod.bLinkTo6Tray[1]==true)
            {
                iShowAutoBin[4]=iShowAutoBin[3];
            }

            if(Prod.bLinkTo6Tray[1]==true && Prod.bLinkTo6Tray[2]==true)
            {
                iShowAutoBin[5]=iShowAutoBin[3];
            }
            else if(Prod.bLinkTo6Tray[1]==false && Prod.bLinkTo6Tray[2]==true)
            {
                iShowAutoBin[5]=iShowAutoBin[4];
            }
        }

        //Steven 20181113 : �ץ�Fix Link��ܰ��D
        //==>
        if(Prod.bLinkTo6Tray[eFix2]==true)
        {
            iShowAutoBin[7]=iShowAutoBin[6];
        }

        if(Prod.bLinkTo6Tray[eFix2]==true && Prod.bLinkTo6Tray[eFix3]==true)
        {
            iShowAutoBin[8]=iShowAutoBin[6];
        }
        else if(Prod.bLinkTo6Tray[eFix2]==false && Prod.bLinkTo6Tray[eFix3]==true)
        {
            iShowAutoBin[8]=iShowAutoBin[7];
        }
        //<==
        //Steven 20181113 : �ץ�Fix Link��ܰ��D

        //Ifor 20231122 add Magazine Link
        //==>
        if(AUTO3_IS_MAGAZINE==1)
        {
            for(i=0; i<14; i++)
            {
                if(BinSelect[iTestRunMode].bMagazineLink[i]==true)
                {
                    for(j=0; j<14; j++)
                    {
                        if(BinSelect[iTestRunMode].bMagazineLink[i-j]==false)
                        {
                            MyBinSel[eMag1+i]->Caption=MyBinSel[eMag1+(i-j)]->Caption;
                            break;
                        }
                    }
                }
                else
                {
                     MyBinSel[eMag1+i]->Caption=MyBinSel[eMag1+i]->Caption;
                }
            }
        }
        //<==
        //Ifor 20231122 add Magazine Link

        //jou 2010-01-12 start : E bin show "E"   (E=R)
        if(CUSTOMER_CODE==CC_ASE_KaohSiung || CUSTOMER_CODE==CC_ASE_KaohSiung_K12)     //Steven 20131101 : Add ASE-K12
        {
            iShowAutoBin[Prod.iIfErrorT6+3]=104; //E
        }
        else
        {
            //Steven 20100224 �L�k���?"R"
            //iShowAutoBin[Prod.iIfErrorT6+3]=117; //R
        }
        //jou 2010-01-12 end
    }
}
//---------------------------------------------------------------------------
TQPF_Timer BinTimer;
void __fastcall TfShowBinSelect::DoShowBinDigital()
{
    if(bUpdateBinDigital==true &&
       (NUMBER_PANEL_TYPE==3 ||                                                 //Steven : �m����ܾ�?
        NUMBER_PANEL_TYPE==4))                                                  //Sam 20240604 : �s�W BinDisplay TFT
    {
        int Data;//, iBinSelCT=0;
        int iBinSet[MAX_BIN_UNIT][TEST_MAX_BIN];                                //iBinSet[0~2][20] �OLoader, Empty, Color�A�ҥH���]�w     //Steven 20140402 : Fixed �O����}�a
        int iBinCount[TEST_MAX_BIN]={-1};                                       //�C�Ӧ�m�Q���t��X��Bin                                 //Steven 20140402 : Fixed �O����}�a
        int iBinColor[TEST_MAX_BIN]={-1};                                       //1:red, 2:green, 3:orange                                //Steven 20140402 : Fixed �O����}�a

//        if(AUTO3_IS_MAGAZINE==1)                                                //JerryYang 20220909 : add magazine            //QQQ
//        {
//            iBinSelCT=eTrayCount;
//        }
//        else
//        {
//            if(TrayForm.iFixTrayMode)
//            {
//                iBinSelCT=9;                                                    //use up down
//            }
//            else
//            {
//                iBinSelCT=6;
//            }
//        }

        //��l��
        for(int i=0; i<MAX_BIN_UNIT; i++)                                       //Steven 20140402 : Fixed �O����}�a
        {
            iBinColor[i]=ColorOrange;
            iBinCount[i]=0;
            for(int j=0; j<TEST_MAX_BIN; j++)                                   //Steven 20140402 : Fixed �O����}�a
                iBinSet[i][j]=-1;
        }

        AddBinDisp[eBinDispLoader    ]=eBinDispLoader;
        AddBinDisp[eBinDispEmpty     ]=eBinDispEmpty;
        AddBinDisp[eBinDispColor     ]=eBinDispColor;
        AddBinDisp[eBinDispAuto1     ]=eBinDispAuto1;
        AddBinDisp[eBinDispAuto2     ]=eBinDispAuto2;
        AddBinDisp[eBinDispAuto3     ]=eBinDispAuto3;
        AddBinDisp[eBinDispFix1      ]=eBinDispFix1 ;
        AddBinDisp[eBinDispFix2      ]=eBinDispFix2 ;
        AddBinDisp[eBinDispFix3      ]=eBinDispFix3 ;
        AddBinDisp[eBinDispFix4      ]=eBinDispFix4 ;
        AddBinDisp[eBinDispFix5      ]=eBinDispFix5 ;
        AddBinDisp[eBinDispFix6      ]=eBinDispFix6 ;
        AddBinDisp[eBinDispBulkBox   ]=eBinDispBulkBox;
        AddBinDisp[eBinDispMag1      ]=eBinDispMag1;
        AddBinDisp[eBinDispMag2      ]=eBinDispMag2;
        AddBinDisp[eBinDispMag3      ]=eBinDispMag3;
        AddBinDisp[eBinDispMag4      ]=eBinDispMag4;
        AddBinDisp[eBinDispMag5      ]=eBinDispMag5;
        AddBinDisp[eBinDispMag6      ]=eBinDispMag6;
        AddBinDisp[eBinDispMag7      ]=eBinDispMag7;
        AddBinDisp[eBinDispMag8      ]=eBinDispMag8;
        AddBinDisp[eBinDispMag9      ]=eBinDispMag9;
        AddBinDisp[eBinDispMag10     ]=eBinDispMag10;
        AddBinDisp[eBinDispMag11     ]=eBinDispMag11;
        AddBinDisp[eBinDispMag12     ]=eBinDispMag12;
        AddBinDisp[eBinDispMag13     ]=eBinDispMag13;
        AddBinDisp[eBinDispMag14     ]=eBinDispMag14;
        AddBinDisp[eBinDispAuto4     ]=eBinDispAuto4;
        AddBinDisp[eBinDispAuto5     ]=eBinDispAuto5;
        AddBinDisp[eBinDispAuto6     ]=eBinDispAuto6;
        AddBinDisp[eBinDispFix7      ]=eBinDispFix7;
        AddBinDisp[eBinDispFix8      ]=eBinDispFix8;
        AddBinDisp[eBinDispFix9      ]=eBinDispFix9;
        AddBinDisp[eBinDispFix10     ]=eBinDispFix10;
        AddBinDisp[eBinDispFix11     ]=eBinDispFix11;
        AddBinDisp[eBinDispFix12     ]=eBinDispFix12;

        if(AUTO_EMPTY_COLOR>=3)                                                 //Fix 1-6 Addr�q0�}�l, �ϥβĤG��COM PORT
        {
            AddBinDisp[eBinDispFix1  ]=0;
            AddBinDisp[eBinDispFix2  ]=1;
            AddBinDisp[eBinDispFix3  ]=2;
            AddBinDisp[eBinDispFix4  ]=3;
            AddBinDisp[eBinDispFix5  ]=4;
            AddBinDisp[eBinDispFix6  ]=5;
            AddBinDisp[eBinDispFix7  ]=6;
            AddBinDisp[eBinDispFix8  ]=7;
            AddBinDisp[eBinDispFix9  ]=8;
            AddBinDisp[eBinDispFix10 ]=9;
            AddBinDisp[eBinDispFix11 ]=10;
            AddBinDisp[eBinDispFix12 ]=11;

            AddBinDisp[eBinDispAuto4 ]=3+eAuto4;                                // L=0 E=1 C=2 A1=3 A2=4 A3=5 A4=6 A5=7 A6=8
            AddBinDisp[eBinDispAuto5 ]=3+eAuto5;
            AddBinDisp[eBinDispAuto6 ]=3+eAuto6;
        }

        if(TestIF_File.iMagDisplayOrder==1)
        {
            AddBinDisp[eBinDispMag1      ]=1;                                   //BIN��ܾ�����?15��, addr 0���ϥ�,�qaddr1�}�l
            AddBinDisp[eBinDispMag2      ]=2;
            AddBinDisp[eBinDispMag3      ]=3;
            AddBinDisp[eBinDispMag4      ]=4;
            AddBinDisp[eBinDispMag5      ]=5;
            AddBinDisp[eBinDispMag6      ]=6;
            AddBinDisp[eBinDispMag7      ]=7;
            AddBinDisp[eBinDispMag8      ]=8;
            AddBinDisp[eBinDispMag9      ]=9;
            AddBinDisp[eBinDispMag10     ]=10;
            AddBinDisp[eBinDispMag11     ]=11;
            AddBinDisp[eBinDispMag12     ]=12;
            AddBinDisp[eBinDispMag13     ]=13;
            AddBinDisp[eBinDispMag14     ]=14;
        }
        else
        {
            AddBinDisp[eBinDispMag1      ]=14;                                  //JerryYang 20250919 : Magazine 1�b�̤U��, addr�O14
            AddBinDisp[eBinDispMag2      ]=13;
            AddBinDisp[eBinDispMag3      ]=12;
            AddBinDisp[eBinDispMag4      ]=11;
            AddBinDisp[eBinDispMag5      ]=10;
            AddBinDisp[eBinDispMag6      ]=9;
            AddBinDisp[eBinDispMag7      ]=8;
            AddBinDisp[eBinDispMag8      ]=7;
            AddBinDisp[eBinDispMag9      ]=6;
            AddBinDisp[eBinDispMag10     ]=5;
            AddBinDisp[eBinDispMag11     ]=4;
            AddBinDisp[eBinDispMag12     ]=3;
            AddBinDisp[eBinDispMag13     ]=2;
            AddBinDisp[eBinDispMag14     ]=1;
        }

        iBinSet[0][0]=111;  //Loader
        iBinSet[1][0]=104;  //Empty Tray
        iBinSet[2][0]=102;  //Color Tray
        iBinSet[0][1]=-1;   //Loader
        iBinSet[1][1]=-1;   //Empty Tray
        iBinSet[2][1]=-1;   //Color Tray
        iBinColor[0]=ColorOrange;     //���?
        iBinColor[1]=ColorOrange;
        iBinColor[2]=ColorOrange;

        //�]�wBin����m
        for(int i=0; i<iTestBinCount; i++)                                      //Steven 20121112 : RS232�䴩32Bin 15 --> iTestBinCount
        {
            Data=iTo3PosUnload[Prod.iT6PosCate[i]];                             //JerryYang 20230926
            if(Data<=0 || Data>e3TrayCount) continue;                           //JerryYang 20220909 : �ץ��O�@    //Steven 20121112 : RS232�䴩32Bin 15 --> iTestBinCount
            for(int j=1; j<=e3TrayCount; j++)                                   //RogerYang 20250825 : "<" -> "<=" �ץ��̫�@����ܾ��S�g�J //Steven 20140404 Start : Fixed 256Bin
            {
                if(j==Data)                                                     //Steven 20140402 : Data-1 --> Data
                {
                    iBinSet[Data+2][iBinCount[Data+2]]=i;                       //Steven 20140402 : i+1 --> i
                    iBinCount[Data+2]++;
                }
            }
        }

        if(IniConfig.bAutoTrayLink==true)                                       //jou 2012-06-14 Auto Tray Link
        {
            if(Prod.bLinkTo6Tray[1]==true)
            {
                iBinCount[4]=iBinCount[3];
                for(int i=0; i<MAX_BIN_UNIT; i++)                               //Steven 20140402 : Fixed �O����}�a
                    iBinSet[4][i]=iBinSet[3][i];
            }

            if(Prod.bLinkTo6Tray[1]==true && Prod.bLinkTo6Tray[2]==true)
            {
                iBinCount[5]=iBinCount[3];
                for(int i=0; i<MAX_BIN_UNIT; i++)                               //Steven 20140402 : Fixed �O����}�a
                    iBinSet[5][i]=iBinSet[3][i];
            }
            else if(Prod.bLinkTo6Tray[1]==false && Prod.bLinkTo6Tray[2]==true)
            {
                iBinCount[5]=iBinCount[4];
                for(int i=0; i<MAX_BIN_UNIT; i++)                               //Steven 20140402 : Fixed �O����}�a
                    iBinSet[5][i]=iBinSet[4][i];
            }
        }

        //Steven 20181113 : �ץ�Fix Link��ܰ��D
        //==>
        if(Prod.bLinkTo6Tray[eFix2]==true)
        {
            iBinCount[7]=iBinCount[6];
            for(int i=0; i<MAX_BIN_UNIT; i++)                                   //Steven 20140402 : Fixed �O����}�a
                iBinSet[7][i]=iBinSet[6][i];
        }

        if(Prod.bLinkTo6Tray[eFix2]==true && Prod.bLinkTo6Tray[eFix3]==true)
        {
            iBinCount[8]=iBinCount[6];
            for(int i=0; i<MAX_BIN_UNIT; i++)                                   //Steven 20140402 : Fixed �O����}�a
                iBinSet[8][i]=iBinSet[6][i];
        }
        else if(Prod.bLinkTo6Tray[eFix2]==false && Prod.bLinkTo6Tray[eFix3]==true)
        {
            iBinCount[8]=iBinCount[7];
            for(int i=0; i<MAX_BIN_UNIT; i++)                                   //Steven 20140402 : Fixed �O����}�a
                iBinSet[8][i]=iBinSet[7][i];
        }
        //<==
        //Steven 20181113 : �ץ�Fix Link��ܰ��D
        //Ifor 20231122 add Magazine Link
        //==>
        if(AUTO3_IS_MAGAZINE==1)
        {
            for(int i=0; i<14; i++)
            {
                if(BinSelect[iTestRunMode].bMagazineLink[i]==true)
                {
                    for(int j=0; j<14; j++)
                    {
                        if(BinSelect[iTestRunMode].bMagazineLink[i-j]==false)
                        {
                            iBinCount[13+i]=iBinCount[13+i-j];
                            for(int k=0; k<MAX_BIN_UNIT; k++)
                                iBinSet[13+i][k]=iBinSet[13+i-j][k];
                            break;
                        }
                    }
                }
            }
        }
        //<==
        //Ifor 20231122 add Magazine Link
        if(TestIF_File.bEnableQASampling && TestIF_File.iQASamplingT3Pos!=0)     //Steven 20190326 : QA Sampling
        {
            iBinSet[TestIF_File.iQASamplingT3Pos-1][0]=116;
        }

        //�]�wError Bin����m
        Data=iTo3Unload[Prod.iIfErrorT6];
        if(Data>=0 && Data<e3TrayCount)                                         //JerryYang 20220909 : 9->iBinSelCT
        {
            if(Data>=eBinDispMag1 && Data<=eBinDispMag14)                       //JerryYang 20220909 : add magazine
            {
                if(AUTO3_IS_MAGAZINE==1)
                {
                   int iSort=Data-9;
                   int iLinkCount=0;    //Ifor 20240909 Fix:�O���鷸����D
                    for(int i=iSort; i<14; i++)
                    {
                        if(BinSelect[iTestRunMode].bMagazineLink[i]==true)
                        {
                            iLinkCount++;
                            iBinSet[Data+iLinkCount+3][iBinCount[Data+iLinkCount+3]]=999;    //Magazine bin���䴩��T���?, 999���ӷ�ERROR BIN
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                iBinSet[Data+3][iBinCount[Data+3]]=999;                         //Magazine bin���䴩��T���?, 999���ӷ�ERROR BIN
            }
            else
            {
                iBinSet[Data+3][iBinCount[Data+3]]=104;
            }
            iBinCount[Data+3]++;
        }

        //�]�w�C��
        for(int j=0; j<eTrayCount; j++)
        {
            if(Prod.iIsFailT6[j]>0)                                             //Steven 20240105 : Prod.bIsPass --> Prod.iIsFailT6   //Steven 20240709 : ==1 --> >0
                iBinColor[iTo3Unload[j]+3]=ColorRed;
            else
                iBinColor[iTo3Unload[j]+3]=ColorGreen;
        }

        //Steven 20181113 : �ץ�Fix Link��ܰ��D
        //==>
        if(Prod.bLinkTo6Tray[eFix2]==true)
        {
            iBinColor[7]=iBinColor[6];
        }

        if(Prod.bLinkTo6Tray[eFix2]==true && Prod.bLinkTo6Tray[eFix3]==true)
        {
            iBinColor[8]=iBinColor[6];
        }
        else if(Prod.bLinkTo6Tray[eFix2]==false && Prod.bLinkTo6Tray[eFix3]==true)
        {
            iBinColor[8]=iBinColor[7];
        }
        //<==
        //Steven 20181113 : �ץ�Fix Link��ܰ��D

        for(int i=3; i<MAX_BIN_UNIT; i++)                                       //Steven 20140402 : Fixed �O����}�a
            if(iBinSet[i][0]==-1)
                iBinColor[i]=ColorOrange;                                       //�S�Ψ쪺�N��ܾ��

        //jou 2014-03-28 SPIL Handler  On-line & Offline Switch Flow
        if(IniConfig.bSPILFunction==true && LastSet.iTester==OFF_LINE)          //JerryYang 20170328 (Jou) ���~�Ȥ�X�Τ@��SPILFunction
        {
            for(int i=0; i<MAX_BIN_UNIT; i++)
            {
                iBinColor[i]=ColorRed;
                iBinCount[i]=2;
                for(int j=0; j<MAX_BIN_UNIT; j++)
                {
                    if(j==0)
                        iBinSet[i][j]=123;
                    else if(j==1)
                        iBinSet[i][j]=0;
                    else
                        iBinSet[i][j]=-1;
                }
            }
        }

        //�}�l�]�w����ܾ��W
        for(int i=0; i<MAX_BIN_UNIT; i++)                                       //Steven 20140402 : Fixed �O����}�a
            HSys.BinDisCtrl->WriteTargetBin(i, iBinSet[i], iBinColor[i]);

        bUpdateBinDigital=false;

        HSys.BinDisCtrl->ProcessStopStart(true);
        return;
    }

    if(NUMBER_PANEL_TYPE==0)
        return;                                                                 //Steven 20100512 : �S���w��
    if(bUpdateBinDigital==false)
        return;

    int &Task=iShowBinDigitalTask;
    bool bFlag;
    static int iCount=0;
    switch(Task)
    {
        case 1:
            if(fiosetview->fShow)
            {
                SW[iNumPanelDown].On();
                Task=400;
            }
            else
            {
                for(int i=0; i<12; i++)
                {
                    SW[SwLoaderBin+i].On();
                }
                iCount=0;
                Task=100;
            }
            ShowBinDigital();
            BinTimer.SetSecAndOn(2);
            break;
        case 100:
            if(BinTimer.Off())
            {
                for(int i=0; i<12; i++)
                {
                    SW[SwLoaderBin+i].Off();
                }
                Task=200;
                BinTimer.SetSecAndOn(0.1);
            }
            break;
        case 200:
            if(BinTimer.Off())
            {
                iCount++;
                bFlag=false;
                for(int i=0; i<12; i++)
                {
                    if(iCount<=iShowAutoBin[i])
                    {
                        SW[SwLoaderBin+i].On();
                        bFlag=true;
                    }
                }
                BinTimer.SetSecAndOn(0.1);
                Task=100;
                if(bFlag==false)
                {
                    Task=300;
                }
            }
            break;
        case 300:
            bUpdateBinDigital==false;
            break;
        case 400:                                                               //Steven 20090919 Start
            if(BinTimer.Off())
            {
                if(fiosetview->fShow && bNumPanelDown)
                {
                    SW[iNumPanelDown].Off();
                    BinTimer.SetSecAndOn(0.1);
                    Task=500;
                }
                else
                {
                    bUpdateBinDigital=true;
                    Task=1;
                }
            }
            break;
        case 500:
            if(BinTimer.Off())
            {
                if(fiosetview->fShow && bNumPanelDown)
                {
                    SW[iNumPanelDown].On();
                    BinTimer.SetSecAndOn(0.1);
                    Task=400;
                }
                else
                {
                    bUpdateBinDigital=true;
                    Task=1;
                }
            }
            break;
    }
}
//---------------------------------------------------------------------------
void TfShowBinSelect::SetLabelVisible(int iTag, bool bVisible)
{
    MyBinSel[iTag]        ->Visible=bVisible;
    MyBinSelLab[iTag]     ->Visible=bVisible;
    MyBinSelARTFT[iTag]   ->Visible=bVisible;
    MyBinSelARTFTLab[iTag]->Visible=bVisible;
    MyBinSelARTRT[iTag]   ->Visible=bVisible;
    MyBinSelARTRTLab[iTag]->Visible=bVisible;
    grpBinDisp[iTag]      ->Visible=bVisible;
}
//---------------------------------------------------------------------------
void TfShowBinSelect::SetAutoVisible()
{
    gbBinBox->Visible  =(iHWFix_BinBox==1);
    pnlMag123->Visible =(AUTO3_IS_MAGAZINE>0);
    pnlFix789->Visible =(AUTO_EMPTY_COLOR>=3);
    pnlAuto456->Visible=(AUTO_EMPTY_COLOR>=3);
    gbAuto6->Visible   =(AUTO_EMPTY_COLOR>=4);

    for(int i=eAuto1; i<eTrayCount; i++)
    {
        if(Prod.iTrayType[i]==tNotUse)
        {
            SetLabelVisible(i, false);
        }
        else
        {
            SetLabelVisible(i, true);
        }
    }

    if(AUTO3_IS_MAGAZINE>0)
        SetLabelVisible(iMagAtAuto, false);

    if(AUTO_EMPTY_COLOR>=3)                                                     //Steven 20230907 : For HT-9011UC
    {
        if(CosFunction.bUseTrayUpDownSet)                                       //wei 20160224 TSMC FIX UPDOWN
        {
            pnlFix789->Visible=true;
        }
        else
        {
            pnlFix789->Visible=false;
        }
    }

    if(iHWFix_BinBox==1)                                                        //kevin 20160819 Error bin box
    {
        MyBinSel[eBulkBox]->Caption     ="E";
        MyBinSelARTFT[eBulkBox]->Caption="E";
        MyBinSelARTRT[eBulkBox]->Caption="E";
    }
}
//---------------------------------------------------------------------------
void __fastcall TfShowBinSelect::PageControl1Change(TObject *Sender)
{
    int iTop=0;
    SetAutoVisible();
    if(PageControl1->ActivePageIndex==0)                                        //tsTestBin
    {
        if(fBinSel->chkShow0Xbin->Checked)                                      //jou 20220719 : show 0X bin
            fShowBinSelect->Width=400;                                          //kevin 20140317  280;
        else
            fShowBinSelect->Width=331;                                          //kevin 20140317  280;

        ScrollBox1->AutoScroll=true;
        if(TestIF_File.iTestType==RS232_MODE &&
           TestIF_File.iRs232Mode==eRs23232Bin)                                 //Steven 20121112 : RS232�䴩32Bin
        {
            pnlShowBin->Width=22*iTestBinCount;
        }
        else
        {
            if(iTestBinCount<=16)                                               //kevin 20140305
            {
                if(fBinSel->chkShow0Xbin->Checked)                              //jou 20220719 : show 0X bin
                    pnlShowBin->Width=321;
                else
                    pnlShowBin->Width=252;
                ScrollBox1->AutoScroll=false;
            }
            else
            {
                pnlShowBin->Width=22*293;                                       //kevin 20140305
            }
        }

        iTop=0;
        for(int i=0; i<eTrayCount; i++)
        {
            if(MyBinSel[i]->Visible==true)
            {
                iTop=iTop+MyBinSel[i]->Height;
            }
        }
        pnlShowBin->Height=iTop;
        fShowBinSelect->Height=iTop+btReturn->Height+100;
    }
    else if(PageControl1->ActivePage==tsIndex)                                  //Sam 20210505 : PTI �n�D����q Low Yeild
    {
        fShowBinSelect->Width=279;
        //AI(ht9045-v899) 20260415: dynamic height for tsIndex based on visible panel stack
        int iBottomIndex = gbAutoCleanCount->Top + gbAutoCleanCount->Height + 4;
        if(Panel1->Visible)
            iBottomIndex = Panel1->Top + Panel1->Height + 4;
        if(pnlSpeciallYield->Visible)
            iBottomIndex = pnlSpeciallYield->Top + pnlSpeciallYield->Height + 4;
        if(gbTriggerAlm->Visible)
            iBottomIndex = gbTriggerAlm->Top + gbTriggerAlm->Height + 4;
        if(palAutoDeviceEjection && palAutoDeviceEjection->Visible)
            iBottomIndex = palAutoDeviceEjection->Top + palAutoDeviceEjection->Height + 4;
        if(gbCopyRecipe && gbCopyRecipe->Visible)
            iBottomIndex = gbCopyRecipe->Top + gbCopyRecipe->Height + 4;
        if(iBottomIndex < 350) iBottomIndex = 350;
        fShowBinSelect->Height = iBottomIndex + btReturn->Height + 60;
    }
    else if(PageControl1->ActivePage==tsARTNormalBin)                           //tsTestBin  //JerryYang 20220331 : �n���n�Dshow ART BIN
    {
        fShowBinSelect->Width=331;
        ScrollBox2->AutoScroll=true;
        if(TestIF_File.iTestType==RS232_MODE &&
           TestIF_File.iRs232Mode==eRs23232Bin)                                 //Steven 20121112 : RS232�䴩32Bin
        {
            pnlShowBin_ARTFT->Width=22*iTestBinCount;
        }
        else
        {
            if(iTestBinCount<=16)                                               //kevin 20140305
            {
                pnlShowBin_ARTFT->Width=252;
                ScrollBox2->AutoScroll=false;
            }
            else
            {
                pnlShowBin_ARTFT->Width=22*293;                                 //kevin 20140305
            }
        }

        iTop=0;
        for(int i=0; i<eTrayCount; i++)
        {
            if(MyBinSelARTFT[i]->Visible==true)
            {
                iTop=iTop+MyBinSelARTFT[i]->Height;
            }
        }
        pnlShowBin_ARTFT->Height=iTop;
        fShowBinSelect->Height=iTop+100;
    }
    else if(PageControl1->ActivePage==tsARTRTBin)                               //tsTestBin
    {
        fShowBinSelect->Width=331;                                              //kevin 20140317  280;
        ScrollBox3->AutoScroll=true;
        if(TestIF_File.iTestType==RS232_MODE &&
           TestIF_File.iRs232Mode==eRs23232Bin)                                 //Steven 20121112 : RS232�䴩32Bin
        {
            laArtRtAuto6->Width=22*iTestBinCount;
        }
        else
        {
            if(iTestBinCount<=16)
            {
                laArtRtAuto6->Width=252;
                ScrollBox3->AutoScroll=false;
            }
            else
            {
                laArtRtAuto6->Width=22*293;
            }
        }

        iTop=0;
        for(int i=0; i<eTrayCount; i++)
        {
            if(MyBinSelARTRT[i]->Visible==true)
            {
                iTop=iTop+MyBinSelARTRT[i]->Height;
            }
        }
        pnlShowBin_ARTRT->Height=iTop;
        fShowBinSelect->Height=iTop+100;
    }
    else if(PageControl1->ActivePage==tsUnloadMap)
    {
        if(iHWFix_BinBox==1)
            fShowBinSelect->Width=364;
        else
            fShowBinSelect->Width=280;

        iTop=120+pnlLoad->Height+pnlFix123->Height+pnlAuto123->Height;

        if(pnlAuto456->Visible)
            iTop=iTop+pnlAuto456->Height+2;

        if(pnlFix789->Visible)
            iTop=iTop+pnlFix789->Height+2;

        if(pnlMag123->Visible)
            iTop=iTop+pnlMag123->Height;

        fShowBinSelect->Height=iTop;
    }
    else if(PageControl1->ActivePage==Tab_UPH)                                  //RogerYang 20250224 �����ݨD �s�W�T�C�H��,�Ӯ�,�ƶq,site
    {
        if(IniConfig.bVTESTFunction==true)
        {
            fShowBinSelect->Width=455;                                          //�s�W�T�����?
        }
        else
        {
            fShowBinSelect->Width=279;
        }
        fShowBinSelect->Height=380;
    }
    else
    {
        fShowBinSelect->Width=279;
        fShowBinSelect->Width=357;                                              //JerryYang 20230322 : UI�ק�
        fShowBinSelect->Height=380;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfShowBinSelect::ShowInitialString()
{
    if(CosFunction.bSortingBy2DList==true &&
       LastSet.iTester==_2D_SORT &&
       TestIF_File.bSortingBy2DIDList==true)                                    //JerryYang 20230322 : 2DID expect count
    {
        StrGrdCategory->ColCount=4;
        StrGrdCategory->Cells[1][0]="expect count";
        StrGrdCategory->Cells[2][0]="Count";
        StrGrdCategory->Cells[3][0]="Percent";
    }
    else
    {
        StrGrdCategory->Cells[1][0]="Count";
        StrGrdCategory->Cells[2][0]="Percent";
    }
    StrGrdCategory->RowCount=iTestBinCount+2;

    if(LastSet.iTester==_2D_SORT)                                               //Frank 20221122 : 2DID sorting for ATK
    {
        for(int i=0; i<iTestBinCount; i++)                                      //JerryYang 20220425 Nvidia Ryan�n�D�אּ���Bin�r��
            StrGrdCategory->Cells[0][1+i]="BIN "+AnsiString(i);
    }
    else
    {
        for(int i=0; i<iTestBinCount; i++)
            StrGrdCategory->Cells[0][1+i]="Category "+AnsiString(i);
    }

    StrGrdCategory->Cells[0][iTestBinCount+1]="Error Bin";

    if(CosFunction.bCategoryInfoByContactCT)                                    //Sam 20240131 : �s�W�@�� CategoryInfo ������ƨϥ�? ContactCT
    {
        StrGrdCategoryContCT->Cells[1][0]="Count";
        StrGrdCategoryContCT->Cells[2][0]="Percent";
        StrGrdCategoryContCT->RowCount=iTestBinCount+2;
        for(int i=0; i<iTestBinCount; i++)
            StrGrdCategoryContCT->Cells[0][1+i]="Category "+AnsiString(i);
        StrGrdCategoryContCT->Cells[0][iTestBinCount+1]="Error Bin";
    }

    if(USE_AUTO_RETEST==eartInstall || CosFunction.bUseARTSortCount)            //wei 20150923 add ART�p�� //Ifor 20170316 (wei) add MRT Mode
    {
        StrGrdCategoryART->Cells[1][0]="Count";
        StrGrdCategoryART->Cells[2][0]="Percent";
        StrGrdCategoryART->RowCount=iTestBinCount+2;
        for(int i=0; i<iTestBinCount; i++)
            StrGrdCategoryART->Cells[0][1+i]="Category "+AnsiString(i);

        StrGrdCategoryART->Cells[0][iTestBinCount+1]="Error Bin";

        StrARTSkipICCount->Cells[1][0]="Count";                                 //Frank 20160819
        StrARTSkipICCount->RowCount=22;
        StrARTSkipICCount->ColCount=2;

        for(int i=0; i<20; i++)
        {
            if(i==0)
                StrARTSkipICCount->Cells[0][1+i]="FT "+AnsiString(1);
            else
                StrARTSkipICCount->Cells[0][1+i]="RT "+AnsiString(i);
        }
        StrARTSkipICCount->Cells[0][21]="Total" ;
    }
    ShowCategoryBin();
}
//---------------------------------------------------------------------------
void __fastcall TfShowBinSelect::ShowCategoryBin()
{
    int i=0, sum=0, Sum_ART=0, iTemp=0, Sum_ConCT=0;                            //Sam 20240131 : �s�W�@�� CategoryInfo ������ƨϥ�? ContactCT
    double f=0.0, fSPBin=0.0;                                                   //Isaac 20171113 (Steven) : add ATK Special Bin Yield alarm
    AnsiString S1, S2;
    static bool bFirstTime=true;

    if(bFirstTime==true)
    {
        bFirstTime=false;
        for(i=0; i<TEST_MAX_BIN; i++)
        {
            iLoadPersentCT[i]=0;
            iLoadCountCT[i]=0;
            iYeildCT[i]=0;                                                      //wei 20151111
        }
    }

    if(fContact->fShow==false)
    {
        if(CosFunction.bLowYieldUseContactCounts)                               //Sam 20230620 : �u�� Smart Auto Clean
        {
            for(i=0; i<iTestBinCount; i++)
                sum+=LastSet.iBinData32[0][i];
        }
        else
        {
            for(i=0; i<eTrayCount; i++)
            {
                sum    +=LastSet.BinCT    [0][iTo3Unload[i]];
                Sum_ART+=LastSet.BinCT_ART[0][iTo3Unload[i]];                   //wei 20150923 add ART�p��
            }
        }

        if(CosFunction.bCategoryInfoByContactCT)                                //Sam 20240131 : �s�W�@�� CategoryInfo ������ƨϥ�? ContactCT
            Sum_ConCT=ArmData[0]->GetTotalCT()+ArmData[1]->GetTotalCT();

        if(LastSet.iTester==_2D_SORT)                                           //Frank 20221122 : 2DID sorting for ATK
        {
            for(i=0; i<iTestBinCount+1; i++)                                    //Steven 20121112 : RS232�䴩32Bin 15 --> iTestBinCount
            {
                fShowBinSelect->StrGrdCategory->Cells[0][1+i]="";
                fShowBinSelect->StrGrdCategory->Cells[1][1+i]="";
                fShowBinSelect->StrGrdCategory->Cells[2][1+i]="";
                fShowBinSelect->StrGrdCategory->Cells[3][1+i]="";
                if(iByBinCnt[i]>0 || i==iTestBinCount)
                {
                    if(i==iTestBinCount)
                    {
                        fShowBinSelect->StrGrdCategory->Cells[0][1+iTemp]="Error Bin";
                    }
                    else
                    {
                        fShowBinSelect->StrGrdCategory->Cells[0][1+iTemp]="BIN "+AnsiString(i);
                    }
                    fShowBinSelect->StrGrdCategory->Cells[2][1+iTemp]=LastSet.iBinData32[0][i];
                    fShowBinSelect->StrGrdCategoryART->Cells[1][1+iTemp]=LastSet.iBinData32_ART[0][i];      //wei 20150923 add ART�p��
                    fShowBinSelect->StrGrdCategory->Cells[1][1+iTemp]=iByBinCnt[i];
                    if(sum>0)
                    {
                        f=ChangeToFloat((double)LastSet.iBinData32[0][i], (double)sum);                     //Steven 20250820 : �w�ﰣ�H0�[�W�O�@
                        fShowBinSelect->StrGrdCategory->Cells[3][1+iTemp]=ChangeToPercentage((double)LastSet.iBinData32[0][i], (double)sum);
                    }
                    else
                    {
                        fShowBinSelect->StrGrdCategory->Cells[3][1+iTemp]="0.00%";
                    }

                    if((USE_AUTO_RETEST==eartInstall && IniConfig.bA10_AutoReTest) ||
                        CosFunction.bUseARTSortCount)                           //wei 20150923 add ART�p��  //Ifor 20170316 (wei) add MRT Mode
                    {
                        if(Sum_ART>0)
                        {
//                            f1=ChangeToFloat((double)LastSet.iBinData32_ART[0][i], (double)Sum_ART);
                            fShowBinSelect->StrGrdCategoryART->Cells[2][1+iTemp]=ChangeToPercentage((double)LastSet.iBinData32_ART[0][i], (double)Sum_ART);
                        }
                        else
                        {
                            fShowBinSelect->StrGrdCategoryART->Cells[2][1+iTemp]="0.00%";
                        }
                    }
                    iTemp++;
                }
            }
        }
        else
        {
            for(i=0; i<iTestBinCount+1; i++)                                    //Steven 20121112 : RS232�䴩32Bin 15 --> iTestBinCount
            {
                StrGrdCategory->Cells[1][1+i]=LastSet.iBinData32[0][i];
                StrGrdCategoryART->Cells[1][1+i]=LastSet.iBinData32_ART[0][i];  //wei 20150923 add ART�p��
                StrGrdCategoryContCT->Cells[1][1+i]=ArmData[0]->GetSelBin(i)+ArmData[1]->GetSelBin(i);   //Sam 20240131 : �s�W�@�� CategoryInfo ������ƨϥ�? ContactCT
                if(sum>0)
                {
                    f=ChangeToFloat((double)LastSet.iBinData32[0][i], (double)sum);
                    StrGrdCategory->Cells[2][1+i]=ChangeToPercentage((double)LastSet.iBinData32[0][i], (double)sum);;
                }
                else
                {
                    StrGrdCategory->Cells[2][1+i]="0.00%";
                }

                if(CosFunction.bCategoryInfoByContactCT)
                {
                    if(Sum_ConCT>0)
                    {
//                        f1=ChangeToFloat((double)(ArmData[0]->GetSelBin(i)+ArmData[1]->GetSelBin(i)), (double)Sum_ConCT);
                        StrGrdCategoryContCT->Cells[2][1+i]=ChangeToPercentage((double)(ArmData[0]->GetSelBin(i)+ArmData[1]->GetSelBin(i)), (double)Sum_ConCT);
                    }
                    else
                    {
                        StrGrdCategoryContCT->Cells[2][1+i]="0.00%";
                    }
                }

                if((USE_AUTO_RETEST==eartInstall && IniConfig.bA10_AutoReTest) ||
                    CosFunction.bUseARTSortCount)                               //wei 20150923 add ART�p��  //Ifor 20170316 (wei) add MRT Mode
                {
                    if(Sum_ART>0)
                    {
//                        f1=ChangeToFloat((double)LastSet.iBinData32_ART[0][i], (double)Sum_ART);
                        StrGrdCategoryART->Cells[2][1+i]=ChangeToPercentage((double)LastSet.iBinData32_ART[0][i], (double)Sum_ART);
                    }
                    else
                    {
                        StrGrdCategoryART->Cells[2][1+i]="0.00%";
                    }
                }
            }

            if(LastSet.iTester!=_2D_SORT)                                       //JerryYang 20230322 : 2DID sort���ݰ�
            {
                for(i=0; i<iTestBinCount; i++)                                  //Steven 20121112 : RS232�䴩32Bin 15 --> iTestBinCount
                {
                    if(RunInfo.iUnloadCount<iLoadPersentCT[i])                  //Steven 20140905 : LastSet.SendCT[0] --> RunInfo.iUnloadCount
                        iLoadPersentCT[i]=RunInfo.iUnloadCount;

                    if(RunInfo.iUnloadCount<iLoadCountCT[i])
                        iLoadCountCT[i]=RunInfo.iUnloadCount;

                    if(RunInfo.iUnloadCount<iYeildCT[i])                        //wei 20151111
                        iYeildCT[i]=RunInfo.iUnloadCount;

                    StrGrdCategory->Cells[1][1+i]=LastSet.iBinData32[0][i];
                    StrGrdCategoryART->Cells[1][1+i]=LastSet.iBinData32_ART[0][i];       //wei 20150923 add ART�p��
                    StrGrdCategoryContCT->Cells[1][1+i]=ArmData[0]->GetSelBin(i)+ArmData[1]->GetSelBin(i);   //Sam 20240131 : �s�W�@�� CategoryInfo ������ƨϥ�? ContactCT

                    if(sum>0)
                    {
                        f=ChangeToFloat((double)LastSet.iBinData32[0][i], (double)sum);
                        fSPBin=ChangeToFloat((double)LastSet.iBinData32[0][Prod.iSCKART_SPBinSelect], (double)sum);       //Isaac 20171113 (Steven) : add ATK Special Bin Yield alarm

                        StrGrdCategory->Cells[2][1+i]=ChangeToPercentage((double)LastSet.iBinData32[0][i], (double)sum);

                        if(CosFunction.bCategoryInfoByContactCT)                //Sam 20240131 : �s�W�@�� CategoryInfo ������ƨϥ�? ContactCT
                        {
                            if(Sum_ConCT>0)
                            {
                                f=ChangeToFloat((double)(ArmData[0]->GetSelBin(i)+ArmData[1]->GetSelBin(i)), (double)Sum_ConCT);
                                StrGrdCategoryContCT->Cells[2][1+i]=ChangeToPercentage((double)(ArmData[0]->GetSelBin(i)+ArmData[1]->GetSelBin(i)), (double)Sum_ConCT);
                            }
                            else
                            {
                                f=0.0;
                            }
                        }

                        if((USE_AUTO_RETEST==eartInstall && IniConfig.bA10_AutoReTest) ||
                           CosFunction.bUseARTSortCount)                        //wei 20150923 add ART�p��  //Ifor 20170316 (wei) add MRT Mode
                        {
//                            f1=ChangeToFloat((double)LastSet.iBinData32_ART[0][i], (double)Sum_ART);
                            StrGrdCategoryART->Cells[2][1+i]=ChangeToPercentage((double)LastSet.iBinData32_ART[0][i], (double)Sum_ART);
                        }

                        if(TestIF_File.bSCKART_EnableSPBinAlarm==true &&
                           TestIF_File.bSCKART_EnableART==true)                 //Isaac 20171113 (Steven) : add ATK Special Bin Yield alarm
                        {
                            if(Prod.dSCKART_SPBinAlarmYield!=0.0 && fSPBin>=Prod.dSCKART_SPBinAlarmYield)
                            {
                                bSPBinYieldAlarm=true;
                            }
                            else
                            {
                                bSPBinYieldAlarm=false;
                            }
                        }
                        else
                        {
                            bSPBinYieldAlarm=false;
                        }

                        if(SystemStart && iHome==0)
                        {
                            if(Prod.bFailure[i]==true)
                            {
                                if(IniConfig.bI30ContFailBin)                   //kevin 20160407  add Fail bin �W�L�ƶq�oĵ�i�T��
                                {
                                    if(Prod.dFailureLimit[i]!=0.0 && f>=Prod.dFailureLimit[i])
                                    {
                                        fYieldMonitoring->ClearYieldCount();

                                        if(CUSTOMER_CODE!=CC_KYEC_LEE)          //wei 20151111
                                           iLoadPersentCT[i]=RunInfo.iUnloadCount;

                                        if(IniConfig.bEnableAutoCleanFunction &&//JerryYang 20161121 �ץ�Yield alarm�n������one cycle���\��v�Tauto clean
                                          TestIF.iAutoClean_Function==true &&
                                          (TestIF.iAutoClean_Mode & M_SOCKET_ALARM))
                                        {
                                            InitialAutoCleanAllTask();          //Sam 20230504 : ��z InitialAutoCleanTask
                                        }
                                        S1.sprintf("Category %d count over limit %2.1f%% ", i, Prod.dFailureLimit[i]);
                                        DoLowYieldAlarm("WAR07357", S1);
                                        break;
                                    }
                                }
                                else
                                {
                                    if(CosFunction.bYieldControlBinSelectUseContactCount==true) //KaiChen 20181115 : BinSelect�̭� Yield����ϥ�? Contact Count
                                    {
                                        if(iLowYieldBinSelectContactCount>Prod.iPersentIgnore[i] &&
                                           Prod.dFailureLimit[i]!=0       &&
                                           f>=Prod.dFailureLimit[i])
                                        {
                                            fShowBinSelect->iLowYieldBinSelectContactCount=0;   //KaiChen 20181115 : BinSelect�̭� Yield����ϥ�? Contact Count
                                            fYieldMonitoring->ClearYieldCount();

                                            if(CUSTOMER_CODE!=CC_KYEC_LEE)      //wei 20151111
                                                iLoadPersentCT[i]=RunInfo.iUnloadCount;
                                            if(IniConfig.bEnableAutoCleanFunction && TestIF.iAutoClean_Function==true && (TestIF.iAutoClean_Mode & M_SOCKET_ALARM))
                                            {
                                                InitialAutoCleanAllTask();      //Sam 20230504 : ��z InitialAutoCleanTask
                                            }
                                            S1.sprintf("Category %d count over limit %2.1f%% ", i, Prod.dFailureLimit[i]);
                                            DoLowYieldAlarm("WAR07357", S1);    //Steven 20180627 (wei) : ��XLow Yield Alarm
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        bool bNeedCheck=false;
                                        if(CosFunction.bLowYieldUseContactCounts && //Sam 20230620 : �u�� Smart Auto Clean
                                           CUSTOMER_CODE!=CC_PTI)                   //Sam 20250102 : �ץ��O�� AI Clean
                                        {
                                            if(Sum_ConCT>Prod.iPersentIgnore[i])    //Sam 20250102 : �ץ��O�� AI Clean
                                                bNeedCheck=true;
                                        }
                                        else
                                        {
                                            if(RunInfo.iUnloadCount-iLoadPersentCT[i]>Prod.iPersentIgnore[i])    //Steven 20140905 : LastSet.SendCT[0] --> RunInfo.iUnloadCount
                                                bNeedCheck=true;
                                        }

                                       if(bNeedCheck &&
                                          Prod.dFailureLimit[i]!=0       &&
                                          f>=Prod.dFailureLimit[i])
                                       {
                                           fYieldMonitoring->ClearYieldCount();

                                           if(CUSTOMER_CODE!=CC_KYEC_LEE)       //wei 20151111
                                               iLoadPersentCT[i]=RunInfo.iUnloadCount;
                                           if(IniConfig.bEnableAutoCleanFunction && TestIF.iAutoClean_Function==true && (TestIF.iAutoClean_Mode & M_SOCKET_ALARM))
                                           {
                                                InitialAutoCleanAllTask();      //Sam 20230504 : ��z InitialAutoCleanTask
                                           }
                                           S1.sprintf("Category %d count over limit %2.1f%% ", i, Prod.dFailureLimit[i]);
                                           DoLowYieldAlarm("WAR07357", S1);     //Steven 20180627 (wei) : ��XLow Yield Alarm
                                           if(CosFunction.bSmartAutoClean)
                                           {
                                                                                //Smart Auto Clean �]���~�|�M���?
                                           }
                                           else
                                           {
                                               fCounterClear->ClearCount(ctBinCount);
                                           }
                                           break;
                                       }
                                    }
                                }
                            }
                            else if(Prod.bFailCountEnable[i]==true)
                            {
                                if(IniConfig.bI30ContFailBin)                   //kevin 20160407  add Fail bin �W�L�ƶq�oĵ�i�T��
                                {
                                    if(Prod.iFailCountLimit[i]!=0 && LastSet.iBinData32[0][i]>=Prod.iFailCountLimit[i])
                                    {
                                        fYieldMonitoring->ClearYieldCount();
                                        if(CUSTOMER_CODE!=CC_KYEC_LEE)          //wei 20151111
                                           iLoadCountCT[i]=RunInfo.iUnloadCount;
                                        fCounterClear->ClearCount(ctBinCount);
                                        if(IniConfig.bEnableAutoCleanFunction && TestIF.iAutoClean_Function==true && (TestIF.iAutoClean_Mode & M_SOCKET_ALARM))
                                        {
                                            InitialAutoCleanAllTask();          //Sam 20230504 : ��z InitialAutoCleanTask
                                        }
                                        S1.sprintf("BIN %d over limit count %d ", i, Prod.iFailCountLimit[i]);
                                        DoLowYieldAlarm("WAR07358", S1);        //Steven 20180627 (wei) : ��XLow Yield Alarm
                                        break;
                                    }
                                }
                                else
                                {
                                    bool bNeedCheck=false;
                                    if(CosFunction.bLowYieldUseContactCounts && //Sam 20230620 : �u�� Smart Auto Clean
                                       CUSTOMER_CODE!=CC_PTI)                   //Sam 20250102 : �ץ��O�� AI Clean
                                    {
                                        if(Sum_ConCT>Prod.iFailCountIgnore[i])  //Sam 20250102 : �ץ��O�� AI Clean
                                            bNeedCheck=true;
                                    }
                                    else
                                    {
                                        if(RunInfo.iUnloadCount-iLoadCountCT[i]>Prod.iFailCountIgnore[i])  //Steven 20140905 : LastSet.SendCT[0] --> RunInfo.iUnloadCount
                                            bNeedCheck=true;
                                    }

                                    if(bNeedCheck &&
                                      Prod.iFailCountLimit[i]!=0       &&
                                      LastSet.iBinData32[0][i]>=Prod.iFailCountLimit[i])
                                    {
                                        fYieldMonitoring->ClearYieldCount();
                                        if(CUSTOMER_CODE!=CC_KYEC_LEE)          //wei 20151111
                                            iLoadCountCT[i]=RunInfo.iUnloadCount;
                                        //fCounterClear->ClearCount(ctBinCount);
                                        S1.sprintf("BIN %d over limit count %d ", i, Prod.iFailCountLimit[i]);
                                        DoLowYieldAlarm("WAR07358", S1);        //Steven 20180627 (wei) : ��XLow Yield Alarm
                                        if(CosFunction.bSmartAutoClean)
                                        {
                                                                                //Smart Auto Clean �]���~�|�M���?
                                        }
                                        else
                                        {
                                            fCounterClear->ClearCount(ctBinCount);
                                        }
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfShowBinSelect::UPH_StringGridDblClick(TObject *Sender)        //20090720 Steven: Delete 1 UPH Record
{
    int iRow=0, iTotalUPH=0, iCount=0;
    if(!SystemStart && AccessLevel>=iDefHonPrecLevel &&
       PageControl1->ActivePageIndex==2)                                        //jou 2014-06-19 Security Have 5 Level 3->iDefHonPrecLevel
    {
        iRow=UPH_StringGrid->Selection.Top;
        if(iRow>0 && iRow<11)                                                   //Row����1~10��
        {
            if(Application->MessageBoxA("Do you want to delete this record?", "Confirm", MB_OKCANCEL)==IDOK)
            {
                for(int i=iRow; i<=10; i++)                                     //�Nrow[i+1]���ȩ��row[i]
                {
                    UPH_StringGrid->Cells[0][i]=UPH_StringGrid->Cells[0][i+1];
                    UPH_StringGrid->Cells[1][i]=UPH_StringGrid->Cells[1][i+1];
                    UPH_StringGrid->Cells[2][i]=UPH_StringGrid->Cells[2][i+1];
                    UPH_StringGrid->Cells[3][i]=UPH_StringGrid->Cells[3][i+1];
                    if(IniConfig.bVTESTFunction==true)                          //RogerYang 20250224 �����ݨD �s�W�T�C�H��,�Ӯ�,�ƶq,site
                    {
                        UPH_StringGrid->Cells[4][i]=UPH_StringGrid->Cells[4][i+1];
                        UPH_StringGrid->Cells[5][i]=UPH_StringGrid->Cells[5][i+1];
                        UPH_StringGrid->Cells[6][i]=UPH_StringGrid->Cells[6][i+1];
                    }
                }

                for(int i=1; i<=10; i++)                                        //���s�p��UPH��������
                {
                    try
                    {
                        if(UPH_StringGrid->Cells[3][i]!="")
                        {
                            iTotalUPH+=atoi(AnsiString(UPH_StringGrid->Cells[3][i]).c_str());
                            iCount++;
                        }
                    }
                    catch(...)
                    {
                        MyDBIProcess("Exception", "UPH_StringGridDblClick");
                    }
                }
                RunInfo.iAvgUPH=(iCount==0)?0:iTotalUPH/iCount;
                UPH_StringGrid->Cells[3][12]=RunInfo.iAvgUPH;
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfShowBinSelect::btnAutoCleanClick(TObject *Sender)
{
    if(bRunAutoClean)
        return;

    if(IniConfig.bEnableAutoCleanFunction && TestIF.iAutoClean_Function==true)
    {
        if(TestIF.iAutoClean_Mode & M_MANUAL)                                   //pig 2012.09.17 AutoClean�� start
        {
            iAutoClean_IndexContactCount=0;
            fMain->AutoCleanContactCountLabel->Caption=iAutoClean_IndexContactCount;
            InitialAutoCleanTask();
            InitialShuttleAutoCleanTask();
            InitialIndexAutoCleanTask();

            if(HasICUnderMachine())
            {
                InitialAutoCleanAllTask();                                      //Sam 20230504 : ��z InitialAutoCleanTask
            }
            else
            {
                if(fAllMotorHome==false)
                {
                    ShowMyMessage("Auto Clean Need Home","Auto Clean �ݭn�k�s");
                    return;
                }

                if(MOT[MTrayX].ReadPos()<Prod.iXTrayEmpty)
                {
                    ShowMyMessage("Tray arm not Safe pos","");
                    return;
                }

                bRunAutoClean=true;
                hAutoCleanHangUp.SetSecAndOn(Prod.iHangupMaxTime);              //Steven 20220702 : �w��Auto Clean��Hang Up����
            }

            RecordProcess("AUTO CLEAN pressed");                                // 2012.10.09 , Joye , Auto Clean (Chen)
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfShowBinSelect::TimerAutoCleanCountTimer(TObject *Sender)
{
    if(InitialOK==false)
        return;

    if(IniConfig.bG10ShowImmediateUPH && SystemStart)
        CaculateUPH();                                                          //Steven 20160727 : Show immediate UPH

    if(IniConfig.bEnableAutoCleanFunction==true)                                //Alick 20160624 �ק令�P�_INI�O�_�ҥ�AUTO CLEAN�\��
    {
        btnCleanReset->Enabled     = true;                                      //Steven 20140514
        ed_AutoCleanCount->Enabled = false;

        if(CUSTOMER_CODE==CC_KYEC_LEE)                                          //Ifor 20160902 add �ʤ��n�DAutoclean life time
        {
            AutocleanlifeTime->Caption="Clean Device LifeTime: "+ed_AutoCleanCount->Text+"/"+ IntToStr(TestIF_File.iAutoClean_AlarmCount);
        }
        else
        {
            AutocleanlifeTime->Visible=false;
        }
    }

    BulkBox->Visible=IniConfig.bI33ErrorBinBox;                                 //kevin 20160824 add error bin box
    if(CUSTOMER_CODE==CC_ASE_KaohSiung)
    {
        labScheduleNAME->Caption=ASET_ScheduleNAME;
        labInQty->Caption=ASET_INTQTY;
    }
    if(CosFunction.bHWBinBox && iHWFix_BinBox==1)                               //kevin 20160822 error bin box �M��
    {
        rg_FixBinBox->ItemIndex=iHWFix_BinBox;                                  //kevin 20160819 error bin �n���? Bin Box "\\Binasgn.Data";
        ed_FixBinBoxAlarmCount->Text=iErrorBinBoxAlarm;                         //kevin 20160819
        LabErrorBinNowCount->Caption=LastSet.iBinBoxCount;
    }
    PageControl1_ART->TabVisible  =((USE_AUTO_RETEST==eartInstall && IniConfig.bA10_AutoReTest) || CosFunction.bUseARTSortCount);   //kevin 201506016  //wei 20150331 ���}�\��N���?
    Tab_ARTSkipICCount->TabVisible=((USE_AUTO_RETEST==eartInstall && IniConfig.bA10_AutoReTest && bForKyecBu3RunART==true) || CosFunction.bUseARTSortCount);  //Frank 20160819    //wei 20161118 bRunART-->bForKyecBu3RunART

//    if(CUSTOMER_CODE==CC_JCET)
//    {                                                                           //jou 20180130 (Steven) : JCET �p�K�n�D���x�b�Ͳ���SLT lot no����ק�?
//        if(HasICUnderMachine())
//            edSLT04->Enabled=false;
//        else
//            edSLT04->Enabled=true;
//    }
}
//---------------------------------------------------------------------------
void __fastcall TfShowBinSelect::ed_AutoCleanCountClick(TObject *Sender)
{
    if(fSecurity->Insufficient(43))
    {
        fCleaning->btnResetCleanCountClick(Owner);
        ed_AutoCleanCount->Text=0;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfShowBinSelect::labAuto1Click(TObject *Sender)
{
    fShowBinSelect->Left=10;
    fShowBinSelect->Top =130;
    SetAutoVisible();

    if(PageControl1->ActivePageIndex==0)                                        //tsTestBin
    {
        fShowBinSelect->Width=1024;
    }
    else
    {
        if(fBinSel->chkShow0Xbin->Checked)                                      //jou 20220719 : show 0X bin
            fShowBinSelect->Width=400;                                          //kevin 20140317  280;
        else
            fShowBinSelect->Width=331;                                          //kevin 20140317  280;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfShowBinSelect::btReturnClick(TObject *Sender)
{
    fShowBinSelect->Left=756;
    fShowBinSelect->Top =412;
    fShowBinSelect->Left=756;
    fShowBinSelect->Top =420;
    PageControl1Change(this);
}
//---------------------------------------------------------------------------
void __fastcall TfShowBinSelect::btnCleanResetClick(TObject *Sender)
{
    if(fSecurity->Insufficient(97))
    {
        fCleaning->btnResetCleanCountClick(Owner);
        ed_AutoCleanCount->Text=0;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfShowBinSelect::btnClearCountClick(TObject *Sender)
{
    if(CUSTOMER_CODE!=CC_Greatek)                                               //Sam 201700915 (Steven) : �W�ײM�������v��
    {
        if(fSecurity->Insufficient(108)==false)                                 //Steven 20240105 : add level for clear count
            return;
    }

    int ret=ShowMyMessageBox_YES_NO("Sure To Clear Counter?", "�T�w�O�_�n���s�p�ơH");
    if(ret==2)
    {
        return;
    }

    fCounterClear->ClearCount(ctIndexCount);
}
//---------------------------------------------------------------------------
void TfShowBinSelect::CaculateUPH()                                             //Steven 20160727 : Show immediate UPH
{
    static int iOldSum=-1, PassSum;
    static int UPH=0;
    static DWORD NowTickCount=-1, LastTimeTick=1, PassSec;
    int sum=0;

    for(int i=0; i<eTrayCount; i++)
    {
        if(Prod.iTrayType[i]!=tNotUse)
            sum+=LastSet.BinCT[0][iTo3Unload[i]];
    }

    if((OutArmSuck.HasIC() || ShuttleHasIC() || IndexHasIC()) && sum>iOldSum)
    {
        NowTickCount=MyTickCount();
        if(NowTickCount<LastTimeTick)
        {
            iOldSum=sum;
            LastTimeTick=MyTickCount();
        }
        else
        {
            PassSec=(NowTickCount-LastTimeTick)/1000;
            PassSum=sum-iOldSum;
            if(PassSec>0)
            {
                UPH=PassSum*3600/PassSec;
                fMain->StatusBar1->Panels->Items[7]->Text="Curr UPH: "+AnsiString(UPH);
            }
        }
    }
    else
    {
        iOldSum=sum;
        LastTimeTick=MyTickCount();
        UPH=0;
    }
    iRecordEventLogUPH=UPH;
}
//---------------------------------------------------------------------------
void __fastcall TfShowBinSelect::edSLT01Change(TObject *Sender)
{
    strcpy(LastSet.szJamClearData[0], fMain->StatusBar1->Panels->Items[6]->Text.c_str());  //Alick 20160920 add for JCET
}
//---------------------------------------------------------------------------
void __fastcall TfShowBinSelect::btnSetInpputCntClick(TObject *Sender)
{
    if(IniConfig.bG15LoadInputCount)                                            //kevin 20211106 ��J���ƹF���N Clean out    //JimmyChiu 20220124 fix name
    {
        try
        {
            StipulateInputCount=StrToInt(EdLoadCount->Text);                    //kevin 20211106
        }
        catch(...)
        {
            StipulateInputCount=0;
        }
    }
}
//---------------------------------------------------------------------------
void  TfShowBinSelect::RefreshAiCnt()
{
    for(int i=0; i<eTrayCount; i++)
    {
        if(Prod.iTrayType[i]!=tNotUse)
            EditAi[i]->Text=IntToStr(fFixAICCD->UnloadAICntNG[i]);
        else
            EditAi[i]->Text="";
    }
}
//---------------------------------------------------------------------------
void __fastcall TfShowBinSelect::ShowBinSel_ARTNor()                            //JerryYang 20220331 : �n���n�Dshow ART BIN
{
    if(IniConfig.bSPILFunction && bCanRunSCKART)
    {
        tsARTNormalBin->TabVisible=true;
        int Data, BinTrayTotal=9;
        AnsiString S[eTrayCount], Str;
        if(iHWFix_BinBox==1)                                                    //kevin 20160819
        {
            BinTrayTotal=10;
        }

        if(iTestRunMode==FT)
        {
            palARTNor->Color=clGreen;
        }
        else
        {
            palARTNor->Color=clBtnFace;
        }

        for(int i=eAuto1; i<=iAutoRight; i++)
        {
            if(Prod.bTo6AutoRetest[FT][i]==true)
                Str.sprintf("%s (R)", s6TrayName[i]);
            else
                Str.sprintf("%s", s6TrayName[i]);
            MyBinSelARTFT[i]->Caption=Str;
        }

        for(int i=0; i<BinTrayTotal; i++)
        {
            S[i]=" ";
            BinAssign[i]="";
            bUnloadHasBin[i]=false;                                             //kevin 20180705 ��BIN TRAY set true
            iTrayLastBin[i]=0;                                                  //kevin 20180705 �C�@��TRAY�̫�@��BIN
        }

        int iLengh=0, i2=0;
        bool iRecord[100]={false};
        for(int i=0; i<eTrayCount; i++)
            iBinTray[i]=false;                                                  //kevin 20170328 (Steven) add �P�_TRAY �O�_�ϥ� BIN

        for(int i=0; i<iTestBinCount; i++)
        {
            Data=Prod.iTo6CatData[FT][i];
            if(Data<0)                                                         //JerryYang 20251020 : fix ART��ܿ��~  //kevin 20140317 256 bin 0 start
                continue;

            for(int j=0; j<eTrayCount; j++)                                     //JerryYang 20251020 : fix ART��ܿ��~
            {
                if(j==Data)
                {
                    S[j]+=AnsiString(i)+" ";                                    //kevin 20140317 bin 0 start
                    sBinData[j]=AnsiString(i)+" ";                              //kevin 20180202 record bin
                    iBinTray[j]=true;                                           //kevin 20170328 (Steven) add �P�_TRAY �O�_�ϥ� BIN
                    iLengh=S[j].Length();
                    i2=iLengh/460;

                    BinAssign[j]+=AnsiString(i)+",";                            //kevin 20180705 add Auto 123 fix 123 bin data
                    bUnloadHasBin[j]=true;                                      //kevin 20180705 ��BIN TRAY set true
                    iTrayLastBin[j]=i;                                          //kevin 20180705 �C�@��TRAY�̫�@��BIN

                    if(i2!=0)
                    {
                        if(iRecord[i2]==false)
                        {
                            iRecord[i2]=true;
                            S[j]+="\n\r";
                            sBinData[j]="\n\r";                                 //kevin 20180202 record bin
                            BinAssign[j]+="\n\r";                               //kevin 20180705 add Auto 123 fix 123 bin data
                        }
                    }
                }
                else
                {
                    if(iTestBinCount<=16)                                       //jou 2014-04-30 15 Bin �^�_ ... ����n�\Ū
                    {
                        S[j]+=". ";
                        BinAssign[j]+=",";                                      //kevin 20180705 add Auto 123 fix 123 bin data
                    }
                }
            }
        }

        Data=Prod.iTo6IfError[FT];

        if(TestIF_File.iTestType==RS232_MODE && TestIF_File.iRs232Mode==eRs23232Bin)    //Steven 20121112 : RS232�䴩32Bin
        {
            if(Data>=0)
            {
                S[Data]=" E"+S[Data];
                sBinData[Data]=" E"+S[Data];                                    //kevin 20180202 record bin
                iBinTray[9]=true;                                               //kevin 20170328 (Steven) add �P�_Fix TRAY �O�_�ϥ� BIN
                BinAssign[Data]+="E";                                           //kevin 20180705 add Auto 123 fix 123 bin data
                bUnloadHasBin[Data]=true;                                       //kevin 20180705 ��BIN TRAY set true
                iTrayLastBin[Data]=999;                                         //kevin 20180705 �C�@��TRAY�̫�@��BIN  Error bin
            }
        }
        else
        {
            if(iHWFix_BinBox==1 && Data==-1)                                    //kevin 20160819
            {
                S[9]+="E ";
                sBinData[9]+="E ";                                              //kevin 20180202 record bin
                iBinTray[9]=true;                                               //kevin 20170328 (Steven) add �P�_Fix TRAY �O�_�ϥ� BIN
                BinAssign[9]+="E";                                              //kevin 20180705 add Auto 123 fix 123 bin data
                bUnloadHasBin[9]=true;                                          //kevin 20180705 ��BIN TRAY set true
                iTrayLastBin[9]=999;                                            //kevin 20180705 �C�@��TRAY�̫�@��BIN  Error bin
            }
            else if(Data>=0)
            {
                S[Data]+="E ";
                sBinData[Data]+="E ";                                           //kevin 20180202 record bin
                iBinTray[9]=true;                                               //kevin 20170328 (Steven) add �P�_Fix TRAY �O�_�ϥ� BIN
                BinAssign[Data]+="E";                                           //kevin 20180705 add Auto 123 fix 123 bin data
                bUnloadHasBin[Data]=true;                                       //kevin 20180705 ��BIN TRAY set true
                iTrayLastBin[Data]=999;                                         //kevin 20180705 �C�@��TRAY�̫�@��BIN  Error bin
            }
        }

        //jou 2014-04-30 15 Bin �^�_ ... ����n�\Ū
        if(iTestBinCount<=16)
        {
            for(int i=0; i<BinTrayTotal; i++)                                   //kevin 20140317 bin
            {
                S[i]+=". . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . ";
                BinAssign[i]+=",";                                              //kevin 20180705 add Auto 123 fix 123 bin data
            }
        }

        //Steven 20181113 : �ץ�Fix Link��ܰ��D
        //==>
        if(Prod.bTo6AutoLink[FT][eFix2]==true)
        {
            S[4]=AnsiString("LINK ")+S[3];
            BinAssign[4]=BinAssign[3];
        }

        if(Prod.bTo6AutoLink[FT][eFix2]==true &&
           Prod.bTo6AutoLink[FT][eFix3]==true)
        {
            S[5]=AnsiString("LINK ")+S[3];
            BinAssign[5]=BinAssign[3];
        }
        else if(Prod.bTo6AutoLink[FT][eFix2]==false &&
                Prod.bTo6AutoLink[FT][eFix3]==true)
        {
            S[5]=AnsiString("LINK ")+S[4];
            BinAssign[5]=BinAssign[4];
        }
        //<==
        //Steven 20181113 : �ץ�Fix Link��ܰ��D
        //Ifor 20231122 add Magazine Link
        //==>
        if(AUTO3_IS_MAGAZINE==1)
        {
            for(int i=1; i<14; i++)
            {
                if(BinSelect[FT].bMagazineLink[i]==true)
                {
                    for(int j=0; j<14; j++)
                    {
                        if(BinSelect[FT].bMagazineLink[i-j]==false)
                        {
                            S[eMag1+i]=AnsiString("LINK ")+S[eMag1+(i-j)];
                            BinAssign[eMag1+i]=BinAssign[eMag1+(i-j)];
                            break;
                        }
                    }
                }
            }
        }
        //<==
        //Ifor 20231122 add Magazine Link
        for(int i=0; i<eTrayCount; i++)                                         //JerryYang 20251020 : fix ART��ܿ��~
        {
            MyBinSelARTFT[i]->Caption=S[i];
            int j=MyBinSelARTFT[i]->Caption.Pos(" . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . ");    //kevin 20140317 bin
            if(iHWFix_BinBox==1 && i==eBulkBox)                                 //kevin 20160825
                continue;
            if(iTestBinCount<=16 && j==1)
            {
                MyBinSelARTFT[i]->Font->Color                    =clGray;
            }
            else
            {
                MyBinSelARTFT[i]->Font->Color                    =tcBinColor[Prod.iTo6StackDefFailCate[FT][i]];
            }
        }

        if(IniConfig.bAutoTrayLink==true)                                       //jou 2012-06-14 Auto Tray Link
        {
            if(Prod.bTo6AutoLink[FT][eFix2]==true)
            {
                MyBinSelARTFT[1]->Caption=MyBinSelARTFT[0]->Caption;
            }

            if(Prod.bTo6AutoLink[FT][eFix2]==true &&
               Prod.bTo6AutoLink[FT][eFix3]==true)
            {
                MyBinSelARTFT[2]->Caption=MyBinSelARTFT[0]->Caption;
            }
            else if(Prod.bTo6AutoLink[FT][eFix2]==false &&
                    Prod.bTo6AutoLink[FT][eFix3]==true)
            {
                MyBinSelARTFT[2]->Caption=MyBinSelARTFT[1]->Caption;
            }
        }
    }
    else
    {
        tsARTNormalBin->TabVisible=false;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfShowBinSelect::ShowBinSel_ARTRT()                             //JerryYang 20220331 : �n���n�Dshow ART BIN
{
    if(IniConfig.bSPILFunction && bCanRunSCKART)
    {
        tsARTRTBin->TabVisible=true;
        int Data, iBinSelCT=0, BinTrayTotal=9;
        AnsiString S[eTrayCount];

        if(iHWFix_BinBox==1)                                                    //kevin 20160819
        {
            BinTrayTotal=10;
        }

        for(int i=0; i<BinTrayTotal; i++)
        {
            S[i]=" ";
            BinAssign[i]="";
            bUnloadHasBin[i]=false;                             //kevin 20180705 ��BIN TRAY set true
            iTrayLastBin[i]=0;                                  //kevin 20180705 �C�@��TRAY�̫�@��BIN
        }

        if(iTestRunMode==RT)
        {
            palARTRT->Color=clGreen;
        }
        else
        {
            palARTRT->Color=clBtnFace;
        }

        if(TrayForm.iFixTrayMode)
        {
            iBinSelCT=9;                                                        //use up down
        }
        else
        {
            iBinSelCT=6;
        }
        int iLengh=0,i2=0;
        bool iRecord[100]={false};
        for(int j=0; j<iBinSelCT; j++)
            iBinTray[j]=false;                                                  //kevin 20170328 (Steven) add �P�_TRAY �O�_�ϥ� BIN

        for(int i=0; i<iTestBinCount; i++)
        {
            Data=Prod.iTo6CatData[RT][i];
            if(Data<0)                                                          //JerryYang 20251020 : fix ART��ܿ��~  //kevin 20140317 256 bin 0 start
              continue;

            for(int j=0; j<eTrayCount; j++)                                     //JerryYang 20251020 : fix ART��ܿ��~
            {
                if(j==Data)
                {
                    S[j]+=AnsiString(i)+" ";                                    //kevin 20140317 bin 0 start
                    sBinData[j]=AnsiString(i)+" ";                              //kevin 20180202 record bin
                    iBinTray[j]=true;                                           //kevin 20170328 (Steven) add �P�_TRAY �O�_�ϥ� BIN
                    iLengh=S[j].Length();
                    i2=iLengh/460;

                    BinAssign[j]+=AnsiString(i)+",";                            //kevin 20180705 add Auto 123 fix 123 bin data
                    bUnloadHasBin[j]=true;                                      //kevin 20180705 ��BIN TRAY set true
                    iTrayLastBin[j]=i;                                          //kevin 20180705 �C�@��TRAY�̫�@��BIN

                    if(i2 !=0)
                    {
                        if(iRecord[i2]==false)
                        {
                            iRecord[i2]=true;
                            S[j]+="\n\r";
                            sBinData[j]="\n\r";                                 //kevin 20180202 record bin
                            BinAssign[j]+="\n\r";                               //kevin 20180705 add Auto 123 fix 123 bin data
                        }
                    }
                }
                else
                {
                    //jou 2014-04-30 15 Bin �^�_ ... ����n�\Ū
                    if(iTestBinCount<=16)
                    {
                        S[j]+=". ";
                        BinAssign[j]+=",";                                      //kevin 20180705 add Auto 123 fix 123 bin data
                    }
                }
            }
        }

        Data=Prod.iTo6IfError[RT];

        if(TestIF_File.iTestType==RS232_MODE &&
           TestIF_File.iRs232Mode==eRs23232Bin)                                 //Steven 20121112 : RS232�䴩32Bin
        {
            if(Data>=0)
            {
                S[Data]=" E"+S[Data];
                sBinData[Data]=" E"+S[Data];                    //kevin 20180202 record bin
                iBinTray[9]=true;                               //kevin 20170328 (Steven) add �P�_Fix TRAY �O�_�ϥ� BIN
                BinAssign[Data]+="E";                           //kevin 20180705 add Auto 123 fix 123 bin data
                bUnloadHasBin[Data]=true;                       //kevin 20180705 ��BIN TRAY set true
                iTrayLastBin[Data]=999;                         //kevin 20180705 �C�@��TRAY�̫�@��BIN  Error bin
            }
        }
        else
        {
            if(iHWFix_BinBox==1 && Data==-1)                    //kevin 20160819
            {
                S[9]+="E ";
                sBinData[9]+="E ";                              //kevin 20180202 record bin
                iBinTray[9]=true;                               //kevin 20170328 (Steven) add �P�_Fix TRAY �O�_�ϥ� BIN
                BinAssign[9]+="E";                              //kevin 20180705 add Auto 123 fix 123 bin data
                bUnloadHasBin[9]=true;                          //kevin 20180705 ��BIN TRAY set true
                iTrayLastBin[9]=999;                            //kevin 20180705 �C�@��TRAY�̫�@��BIN  Error bin
            }
            else if(Data>=0)
            {
                S[Data]+="E ";
                sBinData[Data]+="E ";                           //kevin 20180202 record bin
                iBinTray[9]=true;                               //kevin 20170328 (Steven) add �P�_Fix TRAY �O�_�ϥ� BIN
                BinAssign[Data]+="E";                           //kevin 20180705 add Auto 123 fix 123 bin data
                bUnloadHasBin[Data]=true;                       //kevin 20180705 ��BIN TRAY set true
                iTrayLastBin[Data]=999;                         //kevin 20180705 �C�@��TRAY�̫�@��BIN  Error bin
            }
        }

        //jou 2014-04-30 15 Bin �^�_ ... ����n�\Ū
        if(iTestBinCount<=16)
        {
            for(int i=0; i<BinTrayTotal; i++)                                   //kevin 20140317 bin
            {
                S[i]+=". . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . ";
                BinAssign[i]+=",";                                              //kevin 20180705 add Auto 123 fix 123 bin data
            }
        }

        //Steven 20181113 : �ץ�Fix Link��ܰ��D
        //==>
        if(Prod.bTo6AutoLink[RT][eFix2]==true)
        {
            S[4]=AnsiString("LINK ")+S[3];
            BinAssign[4]=BinAssign[3];
        }

        if(Prod.bTo6AutoLink[RT][eFix2]==true &&
           Prod.bTo6AutoLink[RT][eFix3]==true)
        {
            S[5]=AnsiString("LINK ")+S[3];
            BinAssign[5]=BinAssign[3];
        }
        else if(Prod.bTo6AutoLink[RT][eFix2]==false &&
                Prod.bTo6AutoLink[RT][eFix3]==true)
        {
            S[5]=AnsiString("LINK ")+S[4];
            BinAssign[5]=BinAssign[4];
        }
        //<==
        //Steven 20181113 : �ץ�Fix Link��ܰ��D
        //Ifor 20231122 add Magazine Link
        //==>
        if(AUTO3_IS_MAGAZINE==1)
        {
            for(int i=1; i<14; i++)
            {
                if(BinSelect[RT].bMagazineLink[i]==true)
                {
                    for(int j=0; j<14; j++)
                    {
                        if(BinSelect[RT].bMagazineLink[i-j]==false)
                        {
                            S[eMag1+i]=AnsiString("LINK ")+S[eMag1+(i-j)];
                            BinAssign[eMag1+i]=BinAssign[eMag1+(i-j)];
                            break;
                        }
                    }
                }
            }
        }
        //<==
        //Ifor 20231122 add Magazine Link

        for(int i=0; i<eTrayCount; i++)                                         //JerryYang 20251020 : fix ART��ܿ��~
        {
            MyBinSelARTRT[i]->Caption=S[i];
            int j=MyBinSelARTRT[i]->Caption.Pos(" . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . ");    //kevin 20140317 bin
            if(iHWFix_BinBox==1 && i==eBulkBox)                                 //kevin 20160825
                continue;
            if(iTestBinCount<=16 && j==1)
            {
                MyBinSelARTRT[i]->Font->Color                    =clGray;
            }
            else
            {
                MyBinSelARTRT[i]->Font->Color                    =tcBinColor[Prod.iTo6StackDefFailCate[RT][i]];
            }
        }

        if(IniConfig.bAutoTrayLink==true)                                       //jou 2012-06-14 Auto Tray Link
        {
            if(Prod.bTo6AutoLink[RT][eFix2]==true)
            {
                MyBinSelARTRT[1]->Caption=MyBinSelARTRT[0]->Caption;
            }

            if(Prod.bTo6AutoLink[RT][eFix2]==true &&
               Prod.bTo6AutoLink[RT][eFix3]==true)
            {
                MyBinSelARTRT[2]->Caption=MyBinSelARTRT[0]->Caption;
            }
            else if(Prod.bTo6AutoLink[RT][eFix2]==false &&
                    Prod.bTo6AutoLink[RT][eFix3]==true)
            {
                MyBinSelARTRT[2]->Caption=MyBinSelARTRT[1]->Caption;
            }
        }
    }
    else
    {
        tsARTRTBin->TabVisible=false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfShowBinSelect::EdLoadCountClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 100000, 0);
}
//---------------------------------------------------------------------------
void __fastcall TfShowBinSelect::btnICMisPlacementClick(TObject *Sender)
{
    int ret=0;
    AnsiString S="";

    TSpeedButton *Ptr;
    Ptr=(TSpeedButton *)Sender;

    if(SystemStart)
        return;

    if(IniConfig.bSPILFunction==false)
        return;

    if(edtTriggerAlm->Text=="")                                                 //JerryYang 20251020 : modify
        return;

    ret=ShowMyMessageBox_YES_NO("Are you sure to trigger IC Misplacement alarm?", "�T�w�nĲ�oIC�m�����`?");

    if(ret==1)
    {
        S=AnsiString("======== ")+edtTriggerAlm->Text+AnsiString(" Trigger alarm ========");
        RecordProcess(S.c_str());

        if(Ptr->Tag==0)
        {
            ShowErrorMessage("JAM1110", 0, iMMAuto[0], false, "Manually Trigger"); //wei 20160511 ���~Auto�����S��Tray Auto������Tray �u��Skip
        }
        else if(Ptr->Tag==1)
        {
            ShowErrorMessage("JAM1210", 0, iMMAuto[1], false, "Manually Trigger"); //wei 20160511 ���~Auto�����S��Tray Auto������Tray �u��Skip
        }
        else if(Ptr->Tag==2)
        {
            ShowErrorMessage("JAM1310", 0, iMMAuto[2], false, "Manually Trigger"); //wei 20160511 ���~Auto�����S��Tray Auto������Tray �u��Skip
        }
    }
    edtTriggerAlm->Text="";
}
//---------------------------------------------------------------------------
void __fastcall TfShowBinSelect::sbCopyRecipeClick(TObject *Sender)
{
    sbCopyRecipe->Enabled=false;
    fMain->RunBatchCopyRecipe(AnsiString().sprintf("%s%s", DataPath, fMain->cbSetupFileName->Text));
    sbCopyRecipe->Enabled=true;        
}
//---------------------------------------------------------------------------

