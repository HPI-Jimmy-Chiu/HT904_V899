#include "MachineDefine.h"
#pragma hdrstop

#ifdef SECS_GEM
    #include "uHGemEquipment.h"
#endif

#include "SECSGEM.h"

#include "MachineType.h"
#include "main.h"
#include "cSortCT.h"
#include "cObserver.h"
#include "cprod.h"
#include "mysensor.h"
#include "uShowMessage.h"
#include "cTemperFrom.h"
#include "csystem.h"
#include "rs232.h"
#include "uLotInfo.h"
#include "common.h"
#include "atester.h"
#include "mymessbox.h"
#include "INPUT.h"
#include "cMyDB.h"
#include "cTowerLight.h"
#include "cmydef.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfSecsGem *fSecsGem;

struct ETypeStruct
{
    enum
    {
        Start       =1,
        Pause       =2,
        Home        =3,
        OneCycle    =4,
        CleanOut    =5,
        LotStart    =6,

        TotalEvent                                                              //一定要在最下面
    };
};
struct ETypeStruct CEIDType;
//---------------------------------------------------------------------------
__fastcall TfSecsGem::TfSecsGem(TComponent* Owner)
    : TForm(Owner)
{
    int i, j;

    bStartLoadCount=false;
    bAutoRunMode=false;
    bTesterDucking=false;

    iMaxRow=2;
    iMaxCol=8;

    TLabel *lblSiteLabelTemp[MAX_Index_Row][MAX_Index_Col]={
        {lblSiteLabelAa,lblSiteLabelAb,lblSiteLabelAc,lblSiteLabelAd,lblSiteLabelAe,lblSiteLabelAf,lblSiteLabelAg,lblSiteLabelAh},
        {lblSiteLabelBa,lblSiteLabelBb,lblSiteLabelBc,lblSiteLabelBd,lblSiteLabelBe,lblSiteLabelBf,lblSiteLabelBg,lblSiteLabelBh}};
    TEdit *edSocketIDTemp[MAX_Index_Row][MAX_Index_Col]={
        {edtSocketIDAa,edtSocketIDAb,edtSocketIDAc,edtSocketIDAd,edtSocketIDAe,edtSocketIDAf,edtSocketIDAg,edtSocketIDAh},
        {edtSocketIDBa,edtSocketIDBb,edtSocketIDBc,edtSocketIDBd,edtSocketIDBe,edtSocketIDBf,edtSocketIDBg,edtSocketIDBh}};
    TEdit *edSocketUseCountTemp[MAX_Index_Row][MAX_Index_Col]={
        {edtSocketCountAa,edtSocketCountAb,edtSocketCountAc,edtSocketCountAd,edtSocketCountAe,edtSocketCountAf,edtSocketCountAg,edtSocketCountAh},
        {edtSocketCountBa,edtSocketCountBb,edtSocketCountBc,edtSocketCountBd,edtSocketCountBe,edtSocketCountBf,edtSocketCountBg,edtSocketCountBh}};
    TSpeedButton *InputBtnTemp[MAX_Index_Row][MAX_Index_Col]={
        {btnInputSocketIDAa,btnInputSocketIDAb,btnInputSocketIDAc,btnInputSocketIDAd,btnInputSocketIDAe,btnInputSocketIDAf,btnInputSocketIDAg,btnInputSocketIDAh},
        {btnInputSocketIDBa,btnInputSocketIDBb,btnInputSocketIDBc,btnInputSocketIDBd,btnInputSocketIDBe,btnInputSocketIDBf,btnInputSocketIDBg,btnInputSocketIDBh}};
    TSpeedButton *GetCountBtnTemp[MAX_Index_Row][MAX_Index_Col]={
        {btnGetSocketUseCountAa,btnGetSocketUseCountAb,btnGetSocketUseCountAc,btnGetSocketUseCountAd,btnGetSocketUseCountAe,btnGetSocketUseCountAf,btnGetSocketUseCountAg,btnGetSocketUseCountAh},
        {btnGetSocketUseCountBa,btnGetSocketUseCountBb,btnGetSocketUseCountBc,btnGetSocketUseCountBd,btnGetSocketUseCountBe,btnGetSocketUseCountBf,btnGetSocketUseCountBg,btnGetSocketUseCountBh}};

    for(i=0; i<iMaxRow; i++)
    {
        for(j=0; j<iMaxCol; j++)
        {
            lblSiteLabel[i][j]     = lblSiteLabelTemp[i][j];
            edSocketID[i][j]       = edSocketIDTemp[i][j];
            edSocketID[i][j]->Tag    = j+i*iMaxCol;
            edSocketID[i][j]->OnChange = edtSocketIDAaChange;
            edSocketUseCount[i][j] = edSocketUseCountTemp[i][j];
            InputBtn[i][j]         = InputBtnTemp[i][j];
            InputBtn[i][j]->OnClick = btnInputSocketIDAaClick;
            InputBtn[i][j]->Tag    = j+i*iMaxCol;
            GetCountBtn[i][j]      = GetCountBtnTemp[i][j];
            GetCountBtn[i][j]->OnClick = btnGetSocketUseCountAaClick;
            GetCountBtn[i][j]->Tag = j+i*iMaxCol;
            GetCountBtn[i][j]->Visible=false;
        }
    }
    bPauseByHost=false;
}
//---------------------------------------------------------------------------
void __fastcall TfSecsGem::Panel1Click(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
#ifdef SECS_GEM
//==============================================================================
// GEM function Start
//==============================================================================
int  GemCallBack()
{
    return fSecsGem->MySFCode();
}
//---------------------------------------------------------------------------
int ListTest[3]={150,151,156};
int SubListTest[3]={100,160};
double floatdata=12345.678;
bool SECS_GEM_PPSIGNALTOWER_CONTROL_flag=false;
int  iSECS_GEM_PPSIGNALTOWER_CONTROL_RED;
int  iSECS_GEM_PPSIGNALTOWER_CONTROL_GREEN;
int  iSECS_GEM_PPSIGNALTOWER_CONTROL_YELLOW;
//---------------------------------------------------------------------------
bool TfSecsGem::IsSupportRemoteCommand(AnsiString S)
{
    S=S.UpperCase();
    if(S=="MAINTANCE"  ||
        S=="SETUP"      ||
        S=="OPERATOR" )
        return true;
    return false;
}
//---------------------------------------------------------------------------

bool bWaitHostTransferReceipe=false;

unsigned char __fastcall TfSecsGem::SECS_PublicRemoteCommand(AnsiString Command)
{
    int iParameterCount,SVlen,ret,i,ret1,ret2,ret3,ret4,iV,Index;
    TStringList *Parameter;
    TStringList *Value;
    char str[1024];
    unsigned char Type;
    AnsiString P,V;

    if( HGem->GetDataItemLenAndTypeAndDelete(iParameterCount,HType.LIST_TYPE)!=1)
        return 1;                                                               // format error
    Parameter=new TStringList;
    Value=new TStringList;
    Parameter->Clear();
    Value->Clear();
    for(i=0; i<iParameterCount; i++)
    {
        if( HGem->DataItemIn( 2,HType.LIST_TYPE,NULL) !=1)
        {
            delete Parameter;
            delete Value;
            return 1;
        }
        ret1=HGem->GetDataItemLenAndType  ( SVlen,Type       );
        ret2=HGem->DataItemIn             ( SVlen,Type    ,P );
        ret3=HGem->GetDataItemLenAndType  ( SVlen,Type       );
        ret4=HGem->DataItemIn             ( SVlen,Type    ,V );
        if( ret1!=1 || ret2!=1 || ret3!=1 || ret4!=1)
        {
            delete Parameter;
            delete Value;
            return 1;
        }
        Parameter->Add(P);
        Value->Add(V);
    }
    Command=Command.LowerCase();
    if(Command=="maintance")
    {
        for(i=0; i<iParameterCount; i++)
        {
            P=Parameter->Strings[i];
            V=Value    ->Strings[i];
            P=P.LowerCase();
            iV=atoi(V.c_str());
            Index=-1;
            if( P=="running_music_select"   ) Index=0;
            if( P=="error_jam_music_select" ) Index=1;
            if( P=="pause_music_select"     ) Index=2;
            if( P=="message_music_select"   ) Index=3;
            if( P=="heating_music_select"   ) Index=4;
            if( P=="homing_music_select"    ) Index=5;
            if(Index!=-1)
            {
                if(iV<0) iV=0;
                if(iV>4) iV=4;

                TComboBox *Ptr[6]=
                {
                    fTowerLight->ComboBox6,fTowerLight->ComboBox7,fTowerLight->ComboBox8,
                    fTowerLight->ComboBox9,fTowerLight->ComboBox10,fTowerLight->ComboBox1
                };
                Ptr[Index]->ItemIndex=iV;
            }
        }
    }
    else if(Command=="setup")
    {
        for(i=0; i<iParameterCount; i++)
        {
            P=Parameter->Strings[i];
            V=Value    ->Strings[i];
            iV=atoi(V.c_str());
            Index=-1;
            if( P=="Hot_Ambient(0:A,1:H)"   )
            {
                if(V=="0")
                {
                    LastSet.iTemperature=1;
                    fMain->Panel42Click(fMain);
                }
                else
                {
                    LastSet.iTemperature=0;
                    fMain->Panel42Click(fMain);
                }
            }
            else if( P=="Tempture"   )
            {
                fMain->edWorkTemperBase->Text=V;
            }
            else if( P=="SoakTime"   )
            {
                fMain->edSoakTime->Text=V;
            }
            else if( P=="Customer"   )
            {
                fMain->Edit2->Text=V;
            }
        }
    }
    else if(Command=="operator")
    {
        for(i=0; i<iParameterCount; i++)
        {
            P=Parameter->Strings[i];
            V=Value    ->Strings[i];
            P=P.LowerCase();
            V=V.LowerCase();
            iV=atoi(V.c_str());
            Index=-1;
            if(      P=="start" )
            {
                MyMessageBox->Close();
                fMain->BtnStartClick(fMain);
            }
            else if( P=="pause" )
            {
                fMain->BtnPauseClick(fMain);
                MyMessageBox->Close();
                ShowMyMessage("Pause by host");
            }
            else if( P=="resume" )
            {
                MyMessageBox->Close();
                bPauseByHost=false;
            }
            else if( P=="clear"  )
                fSortCT->btnClearCountClick(fSortCT);
            else if( P=="alarm"  )
            {
//                MyMessageBox->Close();
//                ShowMyMessage(V);
                fMain->lbCheckSafeDoorDisable->Caption=V;
            }
            else if( P=="stop"  )
            {
                MyMessageBox->Close();
                ShowMyMessage(V);
                bPauseByHost=true;
            }
            else if( P=="language"  )
            {
                if(V=="english")
                    LastSet.iLanguageCountry = 1;
                else if( V=="chinese")
                    LastSet.iLanguageCountry = 0;
                else
                    LastSet.iLanguageCountry = 1;
                fMain->sbLaguageClick(fMain);
            }
        }
    }

    delete Parameter;
    delete Value;
    return 0;
}
//2013/06/30 Lee start
//---------------------------------------------------------------------------
int  TfSecsGem::MySFCode()
{
    unsigned char Command,HCACK,Type;
    char CommandStr[256];
    char str[256];
    int ret=0,SVlen,i,len;
    AnsiString S;

    if(HGem->Remote.MessageID_S==2 && HGem->Remote.MessageID_F==41)             //  S10,F3 Terminal Display, Single (VTN)
    {
        if( HGem->DataItemIn( 2,HType.LIST_TYPE,NULL)==1)                       // 需要補充多重 Command
        {
            HGem->GetDataItemLenAndType(len,Type);
            if(Type==HType.ASCII_TYPE)
            {
                ret=HGem->DataItemIn( 256   ,HType.ASCII_TYPE,CommandStr);
                if(ret==-1)
                    S="";
            }
            else
            {
                S="";
            }
            S=CommandStr;
            S=S.UpperCase();
            if(IsSupportRemoteCommand(S))
            {
                HCACK=SECS_PublicRemoteCommand(S);
            }
            else if(S=="ONLINE_LOCAL")
            {
                GemBtnOnlineLocal->OnClick(this);
                HCACK=0;
            }
            else if(S=="ONLINE_REMOTE")
            {
                GemBtnOnlineRemote->OnClick(this);
                HCACK=0;
            }
            HGem->InitLocalHead(2,42,0);                                        //S2,F42    Host Command Acknowledge (HCA)
            HGem->DataItemOut(2,HType.LIST_TYPE,NULL);

            HGem->DataItemOut(1,HType.BINARY_TYPE,&HCACK);
            HGem->DataItemOut(1,HType.LIST_TYPE,NULL);
            HGem->DataItemOut(2,HType.LIST_TYPE,NULL);
            strcpy(str,"");
            HGem->DataItemOut(strlen(str),HType.ASCII_TYPE,str);
            HGem->DataItemOut(strlen(str),HType.ASCII_TYPE,str);
            HGem->SendLocalData();

            //HGem->LocalAcknowledge(HGem->Remote.MessageID_S,HGem->Remote.MessageID_F+1,0);
//                0 = Acknowledge, command hasbeen performed
//                1 = Command does not exist "
//                2 = Cannot perform now
//                3 = At least one parameter isinvalid "
//                4 = Acknowledge, command willbe performed with completionsignaled later by an event
//                5 = Rejected, Already inDesired Condition "
//                6 = No such object exists
//                7-63 Reserved "

            return 1;
        }
    }
    else if(HGem->Remote.MessageID_S==100 && HGem->Remote.MessageID_F==5 )
    {
        if( HGem->DataItemIn( 2,HType.LIST_TYPE,NULL)==1)
        {
            HGem->DataItemIn( 1     ,HType.UINT_1_TYPE,&Command);
            HGem->DataItemIn( 256   ,HType.ASCII_TYPE,CommandStr);
            if(Command==0)
            {
                fMain->BtnPauseClick(this);
            }
            else if(Command==1)
            {
//                SpeedButton3Click(this);
                fSortCT->btnClearCountClick(fSortCT);
            }
            else if(Command==2)
            {
                fMain->BtnStartClick(this);
            }
        }
        HGem->LocalAcknowledge(HGem->Remote.MessageID_S,HGem->Remote.MessageID_F+1,0);
        return 1;
    }
    else if(HGem->Remote.MessageID_S==7 && HGem->Remote.MessageID_F==1)         //S7,F1 Process Program Load Inquire (PPI)
    {
        HGem->DataItemIn( 256   ,HType.ASCII_TYPE,CommandStr);
        if(fMain->ComboBox3->Enabled)
        {
            S=CommandStr;
            ret=S.LastDelimiter(".");
            S=S.SubString(1,ret-1);
            ret=fMain->ComboBox3->Items->IndexOf( S );
            fMain->ComboBox3Change(this);
            if( ret!=-1)
            {
                fMain->ComboBox3->ItemIndex=ret;
                HGem->LocalAcknowledge(HGem->Remote.MessageID_S,HGem->Remote.MessageID_F+1,0);
                return 1;
            }
        }

        HGem->LocalAcknowledge(HGem->Remote.MessageID_S,HGem->Remote.MessageID_F+1,1);
        return 1;
    }
    else if(HGem->Remote.MessageID_S==7 && HGem->Remote.MessageID_F==5 )        //S7,F5 Process Program Request (PPR)
    {
        fMain->ComboBox3Change(this);
    }
    else if(HGem->Remote.MessageID_S==7 && HGem->Remote.MessageID_F==17 )       //S7,F17 Delete Process Program Send (DPS)
    {
        fMain->ComboBox3Change(this);
    }
    else if(HGem->Remote.MessageID_S==100 && HGem->Remote.MessageID_F==5 )
    {
        if( HGem->DataItemIn( 2,HType.LIST_TYPE,NULL)==1)
        {
            HGem->DataItemIn( 1     ,HType.UINT_1_TYPE,&Command);
            HGem->DataItemIn( 256   ,HType.ASCII_TYPE,CommandStr);
        }
        HGem->LocalAcknowledge(HGem->Remote.MessageID_S,HGem->Remote.MessageID_F+1,0);
        return 1;
    }
    else if(HGem->Remote.MessageID_S==102 && HGem->Remote.MessageID_F==22 )     //S102,F22  回傳 Socket 目前使用次數
    {
        ProcessHostRetrunSocketNumber();
        return 1;
    }
    else if(HGem->Remote.MessageID_S==102 && HGem->Remote.MessageID_F==26 )     //S102,F26  建立新Socket reply
    {
        ProcessHostRetrunSocketNumber();
        return 1;
    }
    else if(HGem->Remote.MessageID_S==102 && HGem->Remote.MessageID_F==28 )     //S102,F28  回應 將 Site 用到的 socket 卸下 ( Host Database 不清資料 )命令
    {
        ProcessHostRetrunRemoveSocketID();
        return 1;
    }

    return 0;
}
//---------------------------------------------------------------------------

void __fastcall TfSecsGem::ProcessHostRetrunRemoveSocketID()
{
#ifdef SECS_GEM
    int  ACK;

    if( HGem->DataItemIn( 1,HType.LIST_TYPE,NULL)!=1)
        return;
    if( HGem->DataItemIn( 1,HType.INT_4_TYPE,&ACK)!=1)
        return;

    bRemoveSocketID=true;
#endif
}
//---------------------------------------------------------------------------

void TfSecsGem::GemInitialDefault()
{
    AnsiString S;
    GemSBSetup->OnClick=HGem->GemSBSetupClick;
    HGem->CurrentDirectory=AnsiString(CurrentDir)+"\\SECS";                     // 2013/04/02 lee;
    S=AnsiString(CurrentDir)+"\\SECS";
    mkdir(S.c_str());

    HGem->SetDisplayPtr(GemCommMemo);
    HGem->MoveCheckCallBack=GemCallBack;
//    HGem->SetTerminalWindows(fMain->MemoEapLog);                                //可以有 2 個 Terminal
    HGem->SetTerminalWindows2(GemTerminalWindow);
    HGem->SECSConnectionState=SECSConnectionState;
    HGem->GEMCommunicatingState=GEMCommunicatingState;
    HGem->GemPanelControlState=GemPanelControlState;
    HGem->GemBtnSendTerminalMessage=GemBtnSendTerminalMessage;
    GemBtnSendTerminalMessage->OnClick=HGem->GemBtnSendTerminalMessageClick;
    HGem->GemTerminalSendEdit=GemTerminalSendEdit;
    GemTerminalSendEdit->OnKeyDown=HGem->GemTerminalSendEditKeyDown;
    HGem->SetEstablishCommunicationsTryCount(5);
    HGem->GemRemoteReceipeList=GemRemoteReceipeList;
    HGem->GemListRemoteFileName=GemListRemoteFileName;
    GemListRemoteFileName->OnClick=HGem->GemListRemoteFileNameClick;

    HGem->GemDownLoadRemoteFileName=GemDownLoadRemoteFileName;
    GemDownLoadRemoteFileName->OnClick=HGem->GemDownLoadRemoteFileNameClick;

    HGem->GemDeleteRemoteFileName=GemDeleteRemoteFileName;
    GemDeleteRemoteFileName->OnClick=HGem->GemDeleteRemoteFileNameClick;

    HGem->GemRefreshLocalFile=GemRefreshLocalFile;
    GemRefreshLocalFile->OnClick=HGem->GemRefreshLocalFileClick;

    HGem->GemUpLoadLocalFileName=GemUpLoadLocalFileName;
    GemUpLoadLocalFileName->OnClick=HGem->GemUpLoadLocalFileNameClick;

    HGem->GemSelectAllRemoteFile=GemSelectAllRemoteFile;
    GemSelectAllRemoteFile->OnClick=HGem->GemSelectAllRemoteFileClick;

    HGem->GemDisSelectAllRemoteFile=GemDisSelectAllRemoteFile;
    GemDisSelectAllRemoteFile->OnClick=HGem->GemDisSelectAllRemoteFileClick;

    HGem->GemSelectAllLocalFile=GemSelectAllLocalFile;
    GemSelectAllLocalFile->OnClick=HGem->GemSelectAllLocalFileClick;

    HGem->GemDisSelectAllLocalFile=GemDisSelectAllLocalFile;
    GemDisSelectAllLocalFile->OnClick=HGem->GemDisSelectAllLocalFileClick;

    HGem->GemLocalFileLixtBox=GemLocalFileLixtBox;

    HGem->BtnEnableComm=BtnEnableComm;
    BtnEnableComm->OnClick=HGem->BtnEnableCommClick;

    HGem->BtnDisableComm=BtnDisableComm;
    BtnDisableComm->OnClick=HGem->BtnDisableCommClick;

    HGem->GemBtnOnlineRequest=GemBtnOnlineRequest;
    GemBtnOnlineRequest->OnClick=HGem->GemBtnOnlineRequestClick;

    HGem->GemBtnOfflineRequest=GemBtnOfflineRequest;
    GemBtnOfflineRequest->OnClick=HGem->GemBtnOfflineRequestClick;

    HGem->GemBtnOnlineRemote=GemBtnOnlineRemote;
    GemBtnOnlineRemote->OnClick=HGem->GemBtnOnlineRemoteClick;

    HGem->GemBtnOnlineLocal=GemBtnOnlineLocal;
    GemBtnOnlineLocal->OnClick=HGem->GemBtnOnlineLocalClick;

    HGem->EnableOrDisablePtr=GemEnableOrDisable;
}
//---------------------------------------------------------------------------
void TfSecsGem::GemAlarmCodeList()
{
    AnsiString asQuery;
    int i;

    strngrdAlarmList1->ColWidths[0]=30;
    strngrdAlarmList1->ColWidths[1]=100;                                        //ID Alarm List
    strngrdAlarmList1->ColWidths[2]=80;                                         //Alarm Code
    strngrdAlarmList1->ColWidths[3]=80;                                         //Unit No
    strngrdAlarmList1->ColWidths[4]=80;                                         //ID Axle
    strngrdAlarmList1->ColWidths[5]=40;                                         //Type
    strngrdAlarmList1->ColWidths[6]=500;                                        //Message
    asQuery="SELECT * FROM AlarmList";
    MyDBVProcess(asQuery, strngrdAlarmList1);

    HGem->ReadAlamData();
    for(i=1; i<strngrdAlarmList1->RowCount; i++)
    {
        HGem->SetAlamData(strngrdAlarmList1->Cells[1][i],strngrdAlarmList1->Cells[6][i],atoi(strngrdAlarmList1->Cells[5][i].c_str()));
    }
    HGem->WriteAlamData();
//    tsAlarmList->TabVisible=false;

    HGem->ReadEventReportData();
//DATAID    CEID    Report ID   Remark                                      Report ID   SVID
//1         1       1           按下 Start                                    1           100,101,102,103,104,000
//1         2       1           按下 Pause                                    2           505
//1         3       1           按下 One Cycle                                3           506
//1         4       1           按下 Clean Out                                4           504
//1         5       1           按下 Clear Count                          5
//1         6       1           按下 Lot Start                                6

    unsigned CEDIDContent1[] ={10};
    unsigned CEDIDContent2[] ={11};
    unsigned CEDIDContent3[] ={12};
    unsigned CEDIDContent4[] ={13};
    unsigned CEDIDContent5[] ={14};
    unsigned CEDIDContent6[] ={15};

    int EquDefault=1;
    HGem->SetCEIDContent(1   ,sizeof(CEDIDContent1)/sizeof(unsigned) ,CEDIDContent1  ,EquDefault);                      // Ceid=1 ,有 1個 ReportID ,分別是 1
    HGem->SetCEIDContent(2   ,sizeof(CEDIDContent2)/sizeof(unsigned) ,CEDIDContent2  ,EquDefault);                      // Ceid=2 ,有 1個 ReportID ,分別是 1
    HGem->SetCEIDContent(3   ,sizeof(CEDIDContent3)/sizeof(unsigned) ,CEDIDContent3  ,EquDefault);                      // Ceid=3 ,有 1個 ReportID ,分別是 1
    HGem->SetCEIDContent(4   ,sizeof(CEDIDContent4)/sizeof(unsigned) ,CEDIDContent4  ,EquDefault);                      // Ceid=4 ,有 1個 ReportID ,分別是 1
    HGem->SetCEIDContent(5   ,sizeof(CEDIDContent5)/sizeof(unsigned) ,CEDIDContent5  ,EquDefault);                      // Ceid=5 ,有 1個 ReportID ,分別是 1
    HGem->SetCEIDContent(6   ,sizeof(CEDIDContent6)/sizeof(unsigned) ,CEDIDContent6  ,EquDefault);                      // Ceid=5 ,有 1個 ReportID ,分別是 1
    //ict=HGem->GetCEIDContent(1,CEDIDContent3,1);
    // 現在共有 CEID=1..11 Report=1..4
    unsigned ReportIDContent1[]={151,152};
    unsigned ReportIDContent2[]={151};
    unsigned ReportIDContent3[]={151};
    unsigned ReportIDContent4[]={151};
    unsigned ReportIDContent5[]={151};
    unsigned ReportIDContent6[]={151};

    HGem->SetReportIDContent(10   ,sizeof(ReportIDContent1)/sizeof(unsigned) ,ReportIDContent1,EquDefault);             // Report=1 ,有 5 個 SVID
    HGem->SetReportIDContent(11   ,sizeof(ReportIDContent2)/sizeof(unsigned) ,ReportIDContent2,EquDefault);             // Report=1 ,有 5 個 SVID
    HGem->SetReportIDContent(12   ,sizeof(ReportIDContent3)/sizeof(unsigned) ,ReportIDContent3,EquDefault);             // Report=1 ,有 5 個 SVID
    HGem->SetReportIDContent(13   ,sizeof(ReportIDContent4)/sizeof(unsigned) ,ReportIDContent4,EquDefault);             // Report=1 ,有 5 個 SVID
    HGem->SetReportIDContent(14   ,sizeof(ReportIDContent5)/sizeof(unsigned) ,ReportIDContent5,EquDefault);             // Report=1 ,有 5 個 SVID
    HGem->SetReportIDContent(15   ,sizeof(ReportIDContent6)/sizeof(unsigned) ,ReportIDContent6,EquDefault);             // Report=1 ,有 5 個 SVID

    unsigned ReportIDContent[1024];
    int ict;
//    ict=HGem->GetReportIDContent(1,ReportIDContent1,EquDefault);  // 應該回傳 16

    HGem->SaveEventReportData();
}
//---------------------------------------------------------------------------

void TfSecsGem::SECS_SETData()
{
    //============================================================================================================================================
    // 注意 : 當定義 ECID 也會同時定義 SVID 1..99 為系統內定 ID 不得使用
    // 當物件是 TComboBox 時 SV 要指向的是 Text ,EC 要指向的是 ItemIndex
    //============================================================================================================================================

    //============================================================================================================================================
    // MAINFORM
    //============================================================================================================================================

    //-----------  SV ----------------------
    HGem->SetSVDataPointer(100,HType.ASCII_TYPE  ,"MachineType"         ,"", GEM_MachineName,"");
    HGem->SetSVDataPointer(101,HType.ASCII_TYPE  ,"SerialNo"            ,"", fObserver->APHeadLabel28,"");
    HGem->SetSVDataPointer(102,HType.ASCII_TYPE  ,"MachineID"           ,"", fObserver->APHeadLabel5,"");
    HGem->SetSVDataPointer(103,HType.ASCII_TYPE  ,"SoftwareVersion"     ,"", fMain->StatusBar1->Panels->Items[4],"");
    HGem->SetSVDataPointer(104,HType.ASCII_TYPE  ,"SoftwareRunningTime" ,"", fObserver->labPowerOnTime,"");
    HGem->SetSVDataPointer(105,HType.ASCII_TYPE  ,"RunningTime"         ,"", fObserver->labRunningTime,"");
    HGem->SetSVDataPointer(106,HType.ASCII_TYPE  ,"ProductTime"         ,"", fObserver->labProductTime,"");
    HGem->SetSVDataPointer(111,HType.ASCII_TYPE  ,"MachineRunStatus"    ,"", fMain->palMainStatus,"");
    HGem->SetSVDataPointer(112,HType.ASCII_TYPE  ,"SetupFileName"       ,"", fMain->ComboBox3,"");
    HGem->SetSVDataPointer(113,HType.ASCII_TYPE  ,"User Level"          ,"", fMain->cbUserSelect,"");
    HGem->SetSVDataPointer(114,HType.INT_4_TYPE  ,"RunMode"             ,"", &LastSet.iRealDummy,"");
    HGem->SetSVDataPointer(115,HType.INT_4_TYPE  ,"StartMode"           ,"", &LastSet.iRunStartMode,"0:FT  ,2:RT");     //2013/07/29 lee
    HGem->SetSVDataPointer(116,HType.INT_4_TYPE  ,"PressureAir"         ,"", &Sen[SnAirIsEnough].State,"");
    HGem->SetSVDataPointer(117,HType.INT_4_TYPE  ,"NegativePressureAir" ,"", &Sen[SnNegativePressureAir].State,"");
    HGem->SetSVDataPointer(118,HType.INT_4_TYPE  ,"MotorPower"          ,"", &Sen[SnMotorPower].State,"");
    HGem->SetSVDataPointer(119,HType.INT_4_TYPE  ,"SafeLock"            ,"", &Sen[SnRKCoverOpen].State,"");
    HGem->SetSVDataPointer(120,HType.BOOLEAN_TYPE,"EMGStatus"           ,"", &GEM_EMGPressed,"");
    HGem->SetSVDataPointer(121,HType.INT_4_TYPE  ,"EPOutPut"            ,"", &Sen[SnEPAlarm].State,"");
    HGem->SetSVDataPointer(131,HType.ASCII_TYPE  ,"Tester OnLine Mode"  ,"", fMain->Label1,"");
    HGem->SetSVDataPointer(132,HType.ASCII_TYPE  ,"Tester Interface"    ,"", fMain->Panel4,"");
    HGem->SetSVDataPointer(133,HType.ASCII_TYPE  ,"TesterTime"          ,"", fShowMessage->Label3,"");
    HGem->SetSVDataPointer(134,HType.ASCII_TYPE  ,"IndexTime"           ,"", fMain->StatusBar1->Panels->Items[0],"");
    HGem->SetSVDataPointer(135,HType.ASCII_TYPE  ,"CycleTime"           ,"", fShowMessage->Label2,"");
    HGem->SetSVDataPointer(136,HType.INT_4_TYPE  ,"UPH"                 ,"", fMain->StatusBar1->Panels->Items[3],"");
    HGem->SetSVDataPointer(137,HType.INT_4_TYPE  ,"Dut Layout"          ,"", &TestIF.iTestMode,"");
    HGem->SetSVDataPointer(151,HType.INT_4_TYPE  ,"LoaderCount"         ,"", &LastSet.SendCT[0],"");
    HGem->SetSVDataPointer(152,HType.INT_4_TYPE  ,"UnLoadingCount"      ,"", fSortCT->Panel4,"");
    HGem->SetSVDataPointer(159,HType.INT_4_TYPE  ,"Output count of Auto 1","", &LastSet.BinCT[0][0],"");
    HGem->SetSVDataPointer(160,HType.INT_4_TYPE  ,"Output count of Auto 2","", &LastSet.BinCT[0][1],"");
    HGem->SetSVDataPointer(161,HType.INT_4_TYPE  ,"Output count of Auto 3","", &LastSet.BinCT[0][2],"");
    HGem->SetSVDataPointer(169,HType.INT_4_TYPE  ,"Output count of Fix  1","", &LastSet.BinCT[0][3],"");
    HGem->SetSVDataPointer(170,HType.INT_4_TYPE  ,"Output count of Fix  2","", &LastSet.BinCT[0][4],"");
    HGem->SetSVDataPointer(171,HType.INT_4_TYPE  ,"Output count of Fix  3","", &LastSet.BinCT[0][5],"");
    HGem->SetSVDataPointer(172,HType.INT_4_TYPE  ,"Output count of Fix  4","", &LastSet.BinCT[0][6],"");
    HGem->SetSVDataPointer(173,HType.INT_4_TYPE  ,"Output count of Fix  5","", &LastSet.BinCT[0][7],"");
    HGem->SetSVDataPointer(174,HType.INT_4_TYPE  ,"Output count of Fix  6","", &LastSet.BinCT[0][8],"");
    HGem->SetSVDataPointer(191,HType.ASCII_TYPE  ,"TemperatureMode"     ,"", fMain->lblTemperatureMode,"");
    HGem->SetSVDataPointer(192,HType.FT_8_TYPE   ,"TemperatureSet"      ,"", fMain->edWorkTemperBase,"");
    HGem->SetSVDataPointer(193,HType.INT_4_TYPE  ,"SoakTime"            ,"", fMain->edSoakTime,"");
    HGem->SetSVDataPointer(201,HType.ASCII_TYPE  ,"hlNamePlate1"        ,"", fTemperFrom->hlTempPlate1,"");
    HGem->SetSVDataPointer(202,HType.ASCII_TYPE  ,"hlNamePlate2"        ,"", fTemperFrom->hlTempPlate2,"");
    HGem->SetSVDataPointer(203,HType.ASCII_TYPE  ,"hlNameShuttle1"      ,"", fTemperFrom->hlTempShuttle1,"");
    HGem->SetSVDataPointer(204,HType.ASCII_TYPE  ,"hlNameShuttle2"      ,"", fTemperFrom->hlTempShuttle2,"");
    HGem->SetSVDataPointer(205,HType.ASCII_TYPE  ,"hlNameHead12"        ,"", fTemperFrom->hlTempHead12,"");
    HGem->SetSVDataPointer(206,HType.ASCII_TYPE  ,"hlNameHead34"        ,"", fTemperFrom->hlTempHead34,"");
    HGem->SetSVDataPointer(207,HType.ASCII_TYPE  ,"hlNameHead56"        ,"", fTemperFrom->hlTempHead56,"");
    HGem->SetSVDataPointer(208,HType.ASCII_TYPE  ,"hlNameHead78"        ,"", fTemperFrom->hlTempHead78,"");
    HGem->SetSVDataPointer(209,HType.ASCII_TYPE  ,"hlNameDut"           ,"", fTemperFrom->hlTempDut,"");
    HGem->SetSVDataPointer(210,HType.ASCII_TYPE  ,"hlNameChamber"       ,"", fTemperFrom->hlTempChamber,"");
    HGem->SetSVDataPointer(211,HType.ASCII_TYPE  ,"hlNameCCD"           ,"", fTemperFrom->hlTempCCD,"");
    HGem->SetSVDataPointer(212,HType.ASCII_TYPE  ,"HlNamePlate1_2"      ,"", fTemperFrom->hlTempPlate1_2,"");
    HGem->SetSVDataPointer(213,HType.ASCII_TYPE  ,"HlNamePlate2_2"      ,"", fTemperFrom->hlTempPlate2_2,"");
    HGem->SetSVDataPointer(214,HType.ASCII_TYPE  ,"HlNameShuttle1_2"    ,"", fTemperFrom->hlTempShuttle1_2,"");
    HGem->SetSVDataPointer(215,HType.ASCII_TYPE  ,"HlNameShuttle2_2"    ,"", fTemperFrom->hlTempShuttle2_2,"");
    HGem->SetSVDataPointer(216,HType.ASCII_TYPE  ,"hlNameAa1"           ,"", fTemperFrom->hlTempAa1,"");
    HGem->SetSVDataPointer(217,HType.ASCII_TYPE  ,"hlNameBa1"           ,"", fTemperFrom->hlTempBa1,"");
    HGem->SetSVDataPointer(218,HType.ASCII_TYPE  ,"hlNameAb1"           ,"", fTemperFrom->hlTempAb1,"");
    HGem->SetSVDataPointer(219,HType.ASCII_TYPE  ,"hlNameBb1"           ,"", fTemperFrom->hlTempBb1,"");
    HGem->SetSVDataPointer(220,HType.ASCII_TYPE  ,"hlNameAc1"           ,"", fTemperFrom->hlTempAc1,"");
    HGem->SetSVDataPointer(221,HType.ASCII_TYPE  ,"hlNameBc1"           ,"", fTemperFrom->hlTempBc1,"");
    HGem->SetSVDataPointer(222,HType.ASCII_TYPE  ,"hlNameAd1"           ,"", fTemperFrom->hlTempAd1,"");
    HGem->SetSVDataPointer(223,HType.ASCII_TYPE  ,"hlNameBd1"           ,"", fTemperFrom->hlTempBd1,"");
    HGem->SetSVDataPointer(224,HType.ASCII_TYPE  ,"hlNameAa2"           ,"", fTemperFrom->hlTempAa2,"");
    HGem->SetSVDataPointer(225,HType.ASCII_TYPE  ,"hlNameBa2"           ,"", fTemperFrom->hlTempBa2,"");
    HGem->SetSVDataPointer(226,HType.ASCII_TYPE  ,"hlNameAb2"           ,"", fTemperFrom->hlTempAb2,"");
    HGem->SetSVDataPointer(227,HType.ASCII_TYPE  ,"hlNameBb2"           ,"", fTemperFrom->hlTempBb2,"");
    HGem->SetSVDataPointer(228,HType.ASCII_TYPE  ,"hlNameAc2"           ,"", fTemperFrom->hlTempAc2,"");
    HGem->SetSVDataPointer(229,HType.ASCII_TYPE  ,"hlNameBc2"           ,"", fTemperFrom->hlTempBc2,"");
    HGem->SetSVDataPointer(230,HType.ASCII_TYPE  ,"hlNameAd2"           ,"", fTemperFrom->hlTempAd2,"");
    HGem->SetSVDataPointer(231,HType.ASCII_TYPE  ,"hlNameBd2"           ,"", fTemperFrom->hlTempBd2,"");
    HGem->SetSVDataPointer(232,HType.ASCII_TYPE  ,"HlNameChamber_2"     ,"", fTemperFrom->hlTempChamber_2,"");
    HGem->SetSVDataPointer(233,HType.ASCII_TYPE  ,"HlNameDut_2"         ,"", fTemperFrom->hlTempDut_2,"");
    HGem->SetSVDataPointer(234,HType.ASCII_TYPE  ,"HlNameCCD_2"         ,"", fTemperFrom->hlTempCCD_2,"");
    HGem->SetSVDataPointer(251,HType.INT_4_TYPE  ,"Ionizer01"           ,"", &Sen[SnIonFanAlarm].State,"");
    HGem->SetSVDataPointer(252,HType.INT_4_TYPE  ,"Ionizer02"           ,"", &Sen[SnIonFanLevelAlarm].State,"");
    HGem->SetSVDataPointer(253,HType.INT_4_TYPE  ,"Ionizer03"           ,"", &Sen[SnIonBarrierAlarm].State,"");
    HGem->SetSVDataPointer(254,HType.INT_4_TYPE  ,"Ionizer04"           ,"", &Sen[SnIonBarrierLevelAlarm].State,"");
    HGem->SetSVDataPointer(255,HType.INT_4_TYPE  ,"Ionizer05"           ,"", &Sen[SnIonBarrierConditionAlarm].State,"");
    HGem->SetSVDataPointer(256,HType.INT_4_TYPE  ,"Ionizer06"           ,"", &Sen[SnIonFan6Alarm].State,"");
    HGem->SetSVDataPointer(257,HType.INT_4_TYPE  ,"Ionizer07"           ,"", &Sen[SnIonFan7Alarm].State,"");
    HGem->SetSVDataPointer(258,HType.INT_4_TYPE  ,"Ionizer08"           ,"", &Sen[SnIonFan8Alarm].State,"");
    HGem->SetSVDataPointer(259,HType.INT_4_TYPE  ,"Ionizer09"           ,"", &Sen[SnIonFan9Alarm].State,"");
    HGem->SetSVDataPointer(260,HType.INT_4_TYPE  ,"Ionizer10"           ,"", &Sen[SnIonFan10Alarm].State,"");
    HGem->SetSVDataPointer(261,HType.INT_4_TYPE  ,"Ionizer11"           ,"", &Sen[SnIonFan11Alarm].State,"");
    HGem->SetSVDataPointer(291,HType.INT_4_TYPE  ,"SafeDoor01"          ,"", &Sen[SnSafeDoor1].State,"");
    HGem->SetSVDataPointer(292,HType.INT_4_TYPE  ,"SafeDoor02"          ,"", &Sen[SnSafeDoor2].State,"");
    HGem->SetSVDataPointer(293,HType.INT_4_TYPE  ,"SafeDoor03"          ,"", &Sen[SnSafeDoor3].State,"");
    HGem->SetSVDataPointer(294,HType.INT_4_TYPE  ,"SafeDoor04"          ,"", &Sen[SnSafeDoor4].State,"");
    HGem->SetSVDataPointer(295,HType.INT_4_TYPE  ,"SafeDoor05"          ,"", &Sen[SnSafeDoor5].State,"");
    HGem->SetSVDataPointer(296,HType.INT_4_TYPE  ,"SafeDoor06"          ,"", &Sen[SnSafeDoor6].State,"");
    HGem->SetSVDataPointer(297,HType.INT_4_TYPE  ,"SafeDoor07"          ,"", &Sen[SnSafeDoor7].State,"");
    HGem->SetSVDataPointer(298,HType.INT_4_TYPE  ,"SafeDoor08"          ,"", &Sen[SnSafeDoor8].State,"");
    HGem->SetSVDataPointer(299,HType.INT_4_TYPE  ,"SafeDoor09"          ,"", &Sen[SnSafeDoor9].State,"");
    HGem->SetSVDataPointer(300,HType.INT_4_TYPE  ,"SafeDoor10"          ,"", &Sen[SnSafeDoor10].State,"");
    HGem->SetSVDataPointer(305,HType.BOOLEAN_TYPE,"ChamberDoor01"       ,"", &bHeaterDoorIsOpen[0],"");
    HGem->SetSVDataPointer(306,HType.BOOLEAN_TYPE,"ChamberDoor02"       ,"", &bHeaterDoorIsOpen[1],"");
    HGem->SetSVDataPointer(311,HType.BOOLEAN_TYPE,"RTCInstall"          ,"", &REAL_TIME_CCD,"");
    HGem->SetSVDataPointer(312,HType.BOOLEAN_TYPE,"RTCEnable"           ,"", &COM2->bCCDDummyRum,"");
    HGem->SetSVDataPointer(313,HType.BOOLEAN_TYPE,"RTCAlarm"            ,"", &Sen[SnRealTimeCCDStop].State,"");
    HGem->SetSVDataPointer(314,HType.BOOLEAN_TYPE,"RTCCooling"          ,"", fLotInfo->ALed3,"");

    HGem->SetSVDataPointer(315,HType.ASCII_TYPE,"Lot No."          ,"", fMain->Edit1,"");                               //2013/07/29 lee

    HGem->SetSVDataPointer(1000,HType.ASCII_TYPE,"Site Aa Socket ID"  ,"",edtSocketIDAa,"");
    HGem->SetSVDataPointer(1001,HType.ASCII_TYPE,"Site Ab Socket ID"  ,"",edtSocketIDAb,"");
    HGem->SetSVDataPointer(1002,HType.ASCII_TYPE,"Site Ac Socket ID"  ,"",edtSocketIDAc,"");
    HGem->SetSVDataPointer(1003,HType.ASCII_TYPE,"Site Ad Socket ID"  ,"",edtSocketIDAd,"");
    HGem->SetSVDataPointer(1004,HType.ASCII_TYPE,"Site Ae Socket ID"  ,"",edtSocketIDAe,"");
    HGem->SetSVDataPointer(1005,HType.ASCII_TYPE,"Site Af Socket ID"  ,"",edtSocketIDAf,"");
    HGem->SetSVDataPointer(1006,HType.ASCII_TYPE,"Site Ag Socket ID"  ,"",edtSocketIDAg,"");
    HGem->SetSVDataPointer(1007,HType.ASCII_TYPE,"Site Ah Socket ID"  ,"",edtSocketIDAh,"");
    HGem->SetSVDataPointer(1008,HType.ASCII_TYPE,"Site Ba Socket ID"  ,"",edtSocketIDBa,"");
    HGem->SetSVDataPointer(1009,HType.ASCII_TYPE,"Site Bb Socket ID"  ,"",edtSocketIDBb,"");
    HGem->SetSVDataPointer(1010,HType.ASCII_TYPE,"Site Bc Socket ID"  ,"",edtSocketIDBc,"");
    HGem->SetSVDataPointer(1011,HType.ASCII_TYPE,"Site Bd Socket ID"  ,"",edtSocketIDBd,"");
    HGem->SetSVDataPointer(1012,HType.ASCII_TYPE,"Site Be Socket ID"  ,"",edtSocketIDBe,"");
    HGem->SetSVDataPointer(1013,HType.ASCII_TYPE,"Site Bf Socket ID"  ,"",edtSocketIDBf,"");
    HGem->SetSVDataPointer(1014,HType.ASCII_TYPE,"Site Bg Socket ID"  ,"",edtSocketIDBg,"");
    HGem->SetSVDataPointer(1015,HType.ASCII_TYPE,"Site Bh Socket ID"  ,"",edtSocketIDBh,"");

    HGem->SetSVDataPointer(1100,HType.ASCII_TYPE,"Site Aa Socket Use Count"  ,"",edtSocketCountAa,"");
    HGem->SetSVDataPointer(1101,HType.ASCII_TYPE,"Site Ab Socket Use Count"  ,"",edtSocketCountAb,"");
    HGem->SetSVDataPointer(1102,HType.ASCII_TYPE,"Site Ac Socket Use Count"  ,"",edtSocketCountAc,"");
    HGem->SetSVDataPointer(1103,HType.ASCII_TYPE,"Site Ad Socket Use Count"  ,"",edtSocketCountAd,"");
    HGem->SetSVDataPointer(1104,HType.ASCII_TYPE,"Site Ae Socket Use Count"  ,"",edtSocketCountAe,"");
    HGem->SetSVDataPointer(1105,HType.ASCII_TYPE,"Site Af Socket Use Count"  ,"",edtSocketCountAf,"");
    HGem->SetSVDataPointer(1106,HType.ASCII_TYPE,"Site Ag Socket Use Count"  ,"",edtSocketCountAg,"");
    HGem->SetSVDataPointer(1107,HType.ASCII_TYPE,"Site Ah Socket Use Count"  ,"",edtSocketCountAh,"");
    HGem->SetSVDataPointer(1108,HType.ASCII_TYPE,"Site Ba Socket Use Count"  ,"",edtSocketCountBa,"");
    HGem->SetSVDataPointer(1109,HType.ASCII_TYPE,"Site Bb Socket Use Count"  ,"",edtSocketCountBb,"");
    HGem->SetSVDataPointer(1110,HType.ASCII_TYPE,"Site Bc Socket Use Count"  ,"",edtSocketCountBc,"");
    HGem->SetSVDataPointer(1111,HType.ASCII_TYPE,"Site Bd Socket Use Count"  ,"",edtSocketCountBd,"");
    HGem->SetSVDataPointer(1112,HType.ASCII_TYPE,"Site Be Socket Use Count"  ,"",edtSocketCountBe,"");
    HGem->SetSVDataPointer(1113,HType.ASCII_TYPE,"Site Bf Socket Use Count"  ,"",edtSocketCountBf,"");
    HGem->SetSVDataPointer(1114,HType.ASCII_TYPE,"Site Bg Socket Use Count"  ,"",edtSocketCountBg,"");
    HGem->SetSVDataPointer(1115,HType.ASCII_TYPE,"Site Bh Socket Use Count"  ,"",edtSocketCountBh,"");

    //-----------  EC ----------------------
    HGem->SetECDataPointer(520,HType.BOOLEAN_TYPE,"PPSIGNALTOWER_flag","",&SECS_GEM_PPSIGNALTOWER_CONTROL_flag,true,false,false,"");

    HGem->SetECDataPointer(521,HType.INT_4_TYPE,"PPSIGNALTOWER_RED"   ,"",&iSECS_GEM_PPSIGNALTOWER_CONTROL_RED,2,0,0,"");
    HGem->SetECDataPointer(522,HType.INT_4_TYPE,"PPSIGNALTOWER_GREEN" ,"",&iSECS_GEM_PPSIGNALTOWER_CONTROL_GREEN,2,0,0,"");
    HGem->SetECDataPointer(523,HType.INT_4_TYPE,"PPSIGNALTOWER_YELLOW","",&iSECS_GEM_PPSIGNALTOWER_CONTROL_YELLOW,2,0,0,"");

    HGem->SetECDataPointer(530,HType.INT_4_TYPE,"SIGNALTOWER_Green_Running" ,"",&LastSet.MessageLight[0][0],2,0,0,"");
    HGem->SetECDataPointer(531,HType.INT_4_TYPE,"SIGNALTOWER_Yellow_Running","",&LastSet.MessageLight[0][1],2,0,0,"");
    HGem->SetECDataPointer(532,HType.INT_4_TYPE,"SIGNALTOWER_Red_Running"   ,"",&LastSet.MessageLight[0][2],2,0,0,"");

    HGem->SetECDataPointer(533,HType.INT_4_TYPE,"SIGNALTOWER_Green_ErrorJam" ,"",&LastSet.MessageLight[1][0],2,0,0,"");
    HGem->SetECDataPointer(534,HType.INT_4_TYPE,"SIGNALTOWER_Yellow_ErrorJam","",&LastSet.MessageLight[1][1],2,0,0,"");
    HGem->SetECDataPointer(535,HType.INT_4_TYPE,"SIGNALTOWER_Red_ErrorJam"   ,"",&LastSet.MessageLight[1][2],2,0,0,"");

    HGem->SetECDataPointer(536,HType.INT_4_TYPE,"SIGNALTOWER_Green_Pause" ,"",&LastSet.MessageLight[2][0],2,0,0,"");
    HGem->SetECDataPointer(537,HType.INT_4_TYPE,"SIGNALTOWER_Yellow_Pause","",&LastSet.MessageLight[2][1],2,0,0,"");
    HGem->SetECDataPointer(538,HType.INT_4_TYPE,"SIGNALTOWER_Red_Pause"   ,"",&LastSet.MessageLight[2][2],2,0,0,"");

    HGem->SetECDataPointer(539,HType.INT_4_TYPE,"SIGNALTOWER_Green_Message" ,"",&LastSet.MessageLight[3][0],2,0,0,"");
    HGem->SetECDataPointer(540,HType.INT_4_TYPE,"SIGNALTOWER_Yellow_Message","",&LastSet.MessageLight[3][1],2,0,0,"");
    HGem->SetECDataPointer(541,HType.INT_4_TYPE,"SIGNALTOWER_Red_Message"   ,"",&LastSet.MessageLight[3][2],2,0,0,"");

    HGem->SetECDataPointer(542,HType.INT_4_TYPE,"SIGNALTOWER_Green_Heating" ,"",&LastSet.MessageLight[4][0],2,0,0,"");
    HGem->SetECDataPointer(543,HType.INT_4_TYPE,"SIGNALTOWER_Yellow_Heating","",&LastSet.MessageLight[4][1],2,0,0,"");
    HGem->SetECDataPointer(544,HType.INT_4_TYPE,"SIGNALTOWER_Red_Heating"   ,"",&LastSet.MessageLight[4][2],2,0,0,"");

    HGem->SetECDataPointer(545,HType.INT_4_TYPE,"SIGNALTOWER_Green_Homing" ,"",&LastSet.MessageLight[5][0],2,0,0,"");
    HGem->SetECDataPointer(546,HType.INT_4_TYPE,"SIGNALTOWER_Yellow_Homing","",&LastSet.MessageLight[5][1],2,0,0,"");
    HGem->SetECDataPointer(547,HType.INT_4_TYPE,"SIGNALTOWER_Red_Homing"   ,"",&LastSet.MessageLight[5][2],2,0,0,"");

    HGem->SetECDataPointer(601,HType.INT_4_TYPE,"TesterDutLayout"   ,"",&TestIF.iTestMode,TotalTestMode,0,0,"");
    HGem->SetECDataPointer(602,HType.FT_8_TYPE,"TesterDutXPitch"   ,"",&TestIF.dSiteXPitch,8000.0,2000.0,4000.0,"");
    HGem->SetECDataPointer(603,HType.FT_8_TYPE,"TesterDutYPitch"   ,"",&TestIF.dSiteYPitch,8000.0,2000.0,6000.0,"");
    HGem->SetECDataPointer(604,HType.ASCII_TYPE,"SiteMapping"   ,"",&TestIF.asGEM_SiteMapping,"","","","");
    HGem->SetECDataPointer(605,HType.ASCII_TYPE,"DutOnOff"   ,"",&TestIF.asGEM_DutOnOff,"","","","");
    HGem->SetECDataPointer(606,HType.BOOLEAN_TYPE,"PickerMode"   ,"",&TestIF.bSearchLastMode,false,true,false,"");
    HGem->SetECDataPointer(607,HType.INT_4_TYPE,"InArmUseSuckerMode","",&TestIF.iUseSuckMode,8,2,2,"");
    HGem->SetECDataPointer(608,HType.BOOLEAN_TYPE,"NSBiasKit","",&TestIF.bSearchLastMode,false,true,true,"");
    HGem->SetECDataPointer(609,HType.BOOLEAN_TYPE,"ShuttleMode","",&TestIF.iShuttleMode,false,true,false,"");
    HGem->SetECDataPointer(610,HType.BOOLEAN_TYPE,"ShuttleSelect","",&TestIF.iShuttle_Sel,false,true,true,"");

    HGem->SetECDataPointer(621,HType.INT_4_TYPE,"TrayForm1 X Division","",&UserDefForm[0].XDivision,30,2,14,"");
    HGem->SetECDataPointer(622,HType.INT_4_TYPE,"TrayForm1 Y Division","",&UserDefForm[0].YDivision,70,2,35,"");
    HGem->SetECDataPointer(623,HType.FT_8_TYPE,"TrayForm1 X Start","mm",&UserDefForm[0].XStart,50.0,1.0,8.0,"");
    HGem->SetECDataPointer(624,HType.FT_8_TYPE,"TrayForm1 Y Start","mm",&UserDefForm[0].YStart,50.0,1.0,8.0,"");
    HGem->SetECDataPointer(625,HType.FT_8_TYPE,"TrayForm1 X Pitch","mm",&UserDefForm[0].XPitch,50.0,1.0,9.2,"");
    HGem->SetECDataPointer(626,HType.FT_8_TYPE,"TrayForm1 Y Pitch","mm",&UserDefForm[0].YPitch,50.0,1.0,8.8,"");
    HGem->SetECDataPointer(627,HType.FT_8_TYPE,"TrayForm1 Pick up","mm",&UserDefForm[0].iPickUp,130.0,1.0,68.0,"");

    HGem->SetECDataPointer(631,HType.INT_4_TYPE,"TrayForm2 X Division","",&UserDefForm[1].XDivision,30,2,14,"");
    HGem->SetECDataPointer(632,HType.INT_4_TYPE,"TrayForm2 Y Division","",&UserDefForm[1].YDivision,70,2,35,"");
    HGem->SetECDataPointer(633,HType.FT_8_TYPE,"TrayForm2 X Start","mm",&UserDefForm[1].XStart,50.0,1.0,8.0,"");
    HGem->SetECDataPointer(634,HType.FT_8_TYPE,"TrayForm2 Y Start","mm",&UserDefForm[1].YStart,50.0,1.0,8.0,"");
    HGem->SetECDataPointer(635,HType.FT_8_TYPE,"TrayForm2 X Pitch","mm",&UserDefForm[1].XPitch,50.0,1.0,9.2,"");
    HGem->SetECDataPointer(636,HType.FT_8_TYPE,"TrayForm2 Y Pitch","mm",&UserDefForm[1].YPitch,50.0,1.0,8.8,"");
    HGem->SetECDataPointer(637,HType.FT_8_TYPE,"TrayForm2 Pick up","mm",&UserDefForm[1].iPickUp,130.0,1.0,68.0,"");

    HGem->SetECDataPointer(641,HType.INT_4_TYPE,"TrayForm3 X Division","",&UserDefForm[2].XDivision,30,2,14,"");
    HGem->SetECDataPointer(642,HType.INT_4_TYPE,"TrayForm3 Y Division","",&UserDefForm[2].YDivision,70,2,35,"");
    HGem->SetECDataPointer(643,HType.FT_8_TYPE,"TrayForm3 X Start","mm",&UserDefForm[2].XStart,50.0,1.0,8.0,"");
    HGem->SetECDataPointer(644,HType.FT_8_TYPE,"TrayForm3 Y Start","mm",&UserDefForm[2].YStart,50.0,1.0,8.0,"");
    HGem->SetECDataPointer(645,HType.FT_8_TYPE,"TrayForm3 X Pitch","mm",&UserDefForm[2].XPitch,50.0,1.0,9.2,"");
    HGem->SetECDataPointer(646,HType.FT_8_TYPE,"TrayForm3 Y Pitch","mm",&UserDefForm[2].YPitch,50.0,1.0,8.8,"");
    HGem->SetECDataPointer(647,HType.FT_8_TYPE,"TrayForm3 Pick up","mm",&UserDefForm[2].iPickUp,130.0,1.0,68.0,"");

    HGem->SetECDataPointer(651,HType.BOOLEAN_TYPE,"LoadType","",&TrayForm.LodareType,false,true,false,"");
    HGem->SetECDataPointer(652,HType.BOOLEAN_TYPE,"FixTrayMode","",&TrayForm.iFixTrayMode,false,true,false,"");
    HGem->SetECDataPointer(653,HType.ASCII_TYPE,"LoaderTo_FT","",&TrayForm.asGEM_LoaderTo_FT,"","","","");
    HGem->SetECDataPointer(654,HType.ASCII_TYPE,"LoaderTo_RT","",&TrayForm.asGEM_LoaderTo_RT,"","","","");
    HGem->SetECDataPointer(655,HType.ASCII_TYPE,"TrayType","",&TrayForm.asGEM_TrayType,"","","","");
    HGem->SetECDataPointer(656,HType.INT_4_TYPE,"LoaderTrayMode","",&TrayForm.iManualRemoveLoader,0,2,0,"");
    HGem->SetECDataPointer(657,HType.BOOLEAN_TYPE,"UnoadingTrayMode","",&TrayForm.bAutoFeed,false,true,true,"");

    HGem->SetECDataPointer(671,HType.INT_4_TYPE,"PlateSelection","",&HotPlateForm.iPlateSelect,3,1,3,"");
    HGem->SetECDataPointer(672,HType.INT_4_TYPE,"PlateForm X Division","",&HotPlateForm.XDivision,50,2,6,"");
    HGem->SetECDataPointer(673,HType.INT_4_TYPE,"PlateForm Y Division","",&HotPlateForm.YDivision,50,2,11,"");
    HGem->SetECDataPointer(674,HType.FT_8_TYPE,"PlateForm X Start","mm",&HotPlateForm.XStart,50.0,1.0,13.33,"");
    HGem->SetECDataPointer(675,HType.FT_8_TYPE,"PlateForm Y Start","mm",&HotPlateForm.YStart,50.0,1.0,20.0,"");
    HGem->SetECDataPointer(676,HType.FT_8_TYPE,"PlateForm X Pitch","mm",&HotPlateForm.XPitch,50.0,1.0,26.66,"");
    HGem->SetECDataPointer(677,HType.FT_8_TYPE,"PlateForm Y Pitch","mm",&HotPlateForm.YPitch,50.0,1.0,30.0,"");

    HGem->SetECDataPointer(681,HType.INT_4_TYPE,"JamSoakTime","sec",&Temperature.fSoakTime,500,0,90,"");
    HGem->SetECDataPointer(682,HType.BOOLEAN_TYPE,"AmbientCheck","",&Temperature.bUseAbitCHK,false,true,true,"");
    HGem->SetECDataPointer(683,HType.FT_8_TYPE,"AmbientTemp","℃",&Temperature.fAbitTemp,60.0,15.0,3.0,"");
    HGem->SetECDataPointer(684,HType.INT_4_TYPE,"TempRange","℃",&LastSet.iTemptureRange,10,2,3,"");
    HGem->SetECDataPointer(685,HType.INT_4_TYPE,"ChamberRange","℃",&LastSet.iChamberTemptureRange,30,2,3,"");

    HGem->SetECDataPointer(701,HType.INT_4_TYPE,"ContactMode","",&DeviceForm.ContactMode,2,0,0,"");
    HGem->SetECDataPointer(702,HType.INT_4_TYPE,"VacummMode","",&DeviceForm.VacuumMode,2,0,0,"");
    HGem->SetECDataPointer(703,HType.FT_8_TYPE ,"DropWaitTime","sec",&DeviceForm.DropWait,10.0,0.1,0.1,"");
    HGem->SetECDataPointer(704,HType.INT_4_TYPE,"DropSpeed","%%",&DeviceForm.DropSpeed,100,1,30,"");
    HGem->SetECDataPointer(705,HType.INT_4_TYPE,"HeadType","",&DeviceForm.iHeadDeviceCT,6,2,2,"");
    HGem->SetECDataPointer(706,HType.INT_4_TYPE,"KitDiameter","10mm",&DeviceForm.dKitDiameter,6,3,3,"");
    HGem->SetECDataPointer(707,HType.FT_8_TYPE,"ContactForce","kg",&DeviceForm.fAireForce,240.0,16.0,20.0,"");
    HGem->SetECDataPointer(708,HType.FT_8_TYPE,"ReleaseHeightArm1","mm",&DeviceForm.IndexPlace[0],10.0,-50.0,0.0,"");
    HGem->SetECDataPointer(709,HType.FT_8_TYPE,"ReleaseHeightArm2","mm",&DeviceForm.IndexPlace[1],10.0,-50.0,0.0,"");
    HGem->SetECDataPointer(710,HType.FT_8_TYPE,"PickUpHeightArm1","mm",&DeviceForm.IndexArmPick[0],10.0,-50.0,-2.0,"");
    HGem->SetECDataPointer(711,HType.FT_8_TYPE,"PickUpHeightArm2","mm",&DeviceForm.IndexArmPick[1],10.0,-50.0,-2.0,"");
    HGem->SetECDataPointer(712,HType.FT_8_TYPE,"DropOffSetArm1","mm",&DeviceForm.IndexDrop[0],10.0,0.0,2.0,"");
    HGem->SetECDataPointer(713,HType.FT_8_TYPE,"DropOffSetArm2","mm",&DeviceForm.IndexDrop[1],10.0,0.0,2.0,"");
    HGem->SetECDataPointer(714,HType.FT_8_TYPE,"ContactHeightArm1","mm",&DeviceForm.IndexContact[0],-50.-135,0.0,-110.0,"");
    HGem->SetECDataPointer(715,HType.FT_8_TYPE,"ContactHeightArm2","mm",&DeviceForm.IndexContact[1],-50.-135,0.0,-110.0,"");

    HGem->SetECDataPointer(721,HType.INT_4_TYPE,"ConnectionMethod","",&TestIF.iTestType,2,0,1,"");
    HGem->SetECDataPointer(723,HType.INT_4_TYPE,"GPIBAddress","",&TestIF.iGpibAddress,32,1,1,"");
    HGem->SetECDataPointer(724,HType.FT_8_TYPE,"MaxTestTime","sec",&TestIF.iMaxTime,1000.0,60.0,300.0,"");
    HGem->SetECDataPointer(725,HType.FT_8_TYPE,"StartDelayTime","sec",&TestIF.iStartDelayTime,10.0,0.01,0.1,"");
}
//---------------------------------------------------------------------------
void TfSecsGem::GemInitial()
{
    int ret;
    AnsiString asString;
//    LoadSystem();

    GemInitialDefault();
    HGem->SetTimeFormat(1);                                                     // 0:12-bytes, 1:16-bytes, 2:14-bytes, 3:ISO8601 format
    HGem->OpenCommuncation("127.0.0.1",5100);                                   // 本機
    //HGem->OpenCommuncation("192.168.120.103",5100);  // Joye
    //HGem->OpenCommuncation("10.85.5.150",5000);     // scc
    //HGem->OpenCommuncation(5100);     // scc
//    HGem->OpenCommuncation("192.168.14.1",5100);        // 矽格

    ret=HGem->SetReceipeDirectoryAndGlobalName("D:\\HT9045\\IniData\\Data\\","*.*",2);                                  //  0 : 只傳副檔名為 bld
                                                                                       //  1 : 主檔名相同地所有檔案都要傳
                                                                                       //  2 : 路徑下的目錄名,及下面的檔案

    asString=CheckAndReadIniDataGeneral("Version", "Serial No", "29818");
    strcpy(GEM_MachineName,asString.c_str());
    HGem->SetMachineTypeAndSoftwarseVer(GEM_MachineName,fMain->StatusBar1->Panels->Items[4]->Text);

    SECS_SETData();
    GemAlarmCodeList();

    HGem->Timer1->Enabled=true;
}
#endif
//---------------------------------------------------------------------------
void __fastcall TfSecsGem::btnStartClick(TObject *Sender)
{
#ifdef SECS_GEM
    int i, j;

    if(CUSTOMER_CODE==CC_SIGURD_HUKOU)
    {
        if(fSecsGem->CheckSocketState()==false)
        {
            if(fAllMotorHome==false)
            {
                bHomeByStart=false;
                fMain->Home();
            }
            return;
        }

        if( HGem->IsConnect())
        {
            for(i=0; i<iMaxRow; i++)
            {
                for(j=0; j<iMaxCol; j++)
                {
                    if( edSocketID[i][j]->Color==clYellow)
                    {
                        return;
                    }
                }
            }
        }

        plStatus->Caption="Running";

        for(i=0; i<iMaxRow; i++)
        {
            for(j=0; j<iMaxCol; j++)
            {
                InputBtn[i][j]->Visible=false;
                GetCountBtn[i][j]->Visible=false;
            }
        }
    }

    HGem->EventReport(1,CEIDType.Start);
#endif
}
//---------------------------------------------------------------------------

void __fastcall TfSecsGem::btnPauseClick(TObject *Sender)
{
#ifdef SECS_GEM
    plStatus->Caption="Pause";

    if(CUSTOMER_CODE==CC_SIGURD_HUKOU)
    {
        int i, j,iRow=0,iCol=0;

        GetRowCol(iRow,iCol);

        for(i=0; i<iMaxRow; i++)
        {
            for(j=0; j<iMaxCol; j++)
            {
                if(i<iRow && j <iCol && LastSet.bUseTestSocket[0][i][j]==true)
                    InputBtn[i][j]->Visible=true;
            }
        }
        WrireSocketUseCount();
    }

    HGem->EventReport(1,CEIDType.Pause);
#endif
}
//---------------------------------------------------------------------------

void __fastcall TfSecsGem::btnHaltClick(TObject *Sender)
{
    plStatus->Caption="Halt";

    if(CUSTOMER_CODE==CC_SIGURD_HUKOU)
    {
        int i, j,iRow=0,iCol=0;

        GetRowCol(iRow,iCol);

        for(i=0; i<iMaxRow; i++)
        {
            for(j=0; j<iMaxCol; j++)
            {
                if(i<iRow && j <iCol && LastSet.bUseTestSocket[0][i][j]==true)
                    InputBtn[i][j]->Visible=true;
            }
        }
    }
}
//---------------------------------------------------------------------------

void TfSecsGem::RemoveSocketID(int iRow,int iCol)
{
#ifdef SECS_GEM
    int iCT=0,i, j;
    char str[256];

    if(iRow==-1 && iCol==-1)
    {
        for(i=0; i<MAX_Index_Row; i++)
        {
            for(j=0; j<MAX_Index_Col; j++)
            {
                if( edSocketID[i][j]->Text!="")
                {
                    iCT++;
                }
            }
        }
    }
    else
    {
        iCT=1;
    }

    HGem->InitLocalHead(102,27,1);                                              //S102,F27  將 Site 用到的 socket 卸下 ( Host Database 不清資料 )
    HGem->DataItemOut(iCT,HType.LIST_TYPE,NULL);
    if(iRow==-1 && iCol==-1)
    {
        for(i=0; i<MAX_Index_Row; i++)
        {
            for(j=0; j<MAX_Index_Col; j++)
            {
                if( edSocketID[i][j]->Text!="")
                {
                    HGem->DataItemOut(1,HType.LIST_TYPE,NULL);
                    strcpy(str,edSocketID[i][j]->Text.c_str());
                    HGem->DataItemOut(strlen(str),HType.ASCII_TYPE,str);
                }
            }
        }
    }
    else
    {
        if( edSocketID[iRow][iCol]->Text!="")
        {
            HGem->DataItemOut(1,HType.LIST_TYPE,NULL);
            strcpy(str,edSocketID[iRow][iCol]->Text.c_str());
            HGem->DataItemOut(strlen(str),HType.ASCII_TYPE,str);
        }
    }
    HGem->SendLocalData();
    bRemoveSocketID=false;
#endif
    bRemoveSocketID=true;
}
//---------------------------------------------------------------------------

void TfSecsGem::AllUpdateAndRemoveSocketID()
{
    SaveSystem();
    WrireSocketUseCount();
    RemoveSocketID(-1,-1);

    for(int i=0; i<MAX_Index_Row; i++)
    {
        for(int j=0; j<MAX_Index_Col; j++)
        {
            edSocketID[i][j]->Text="";
            edSocketUseCount[i][j]->Text="";
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TfSecsGem::btnInputSocketIDAaClick(TObject *Sender)
{
    if(SECSConnectionState->Caption=="Disconnection")
    {
        ShowMyMessage("Server Disconnection","伺服器斷線");
        return;
    }

#ifdef SECS_GEM
    int Tag,i, j,iRow=0,iCol=0;

    TSpeedButton *P;
    P=(TSpeedButton *)Sender;
    Tag=P->Tag;

    iRow=ChangeToFloatNonPcnt((double)(Tag), (double)(iMaxCol));
    iCol=Tag%iMaxCol;

    RemoveSocketID(iRow,iCol);

    for(i=0; i<iMaxRow; i++)
    {
        for(j=0; j<iMaxCol; j++)
        {
            if(i==iRow && j==iCol)
            {
                edSocketID[i][j]->Enabled=true;
                edSocketID[i][j]->Text="";
                edSocketID[i][j]->SetFocus();
                edSocketUseCount[i][j]->Text="";
                InputBtn[i][j]->Visible=true;
                edSocketID[i][j]->Color=clYellow;
            }
            else
            {
                edSocketID[i][j]->Enabled=false;
                InputBtn[i][j]->Visible=false;
                edSocketID[i][j]->Color=clWhite;
            }
            GetCountBtn[i][j]->Visible=false;
        }
    }
#endif
}
//---------------------------------------------------------------------------

void __fastcall TfSecsGem::GetSocketUseCount(int iRow,int iCol)
{
#ifdef SECS_GEM
    int iCT=0,i, j;
    int Index=0;
    char str[64];

    if(iRow==-1 && iCol==-1)
    {
        for(i=0; i<MAX_Index_Row; i++)
        {
            for(j=0; j<MAX_Index_Col; j++)
            {
                if( edSocketID[i][j]->Text!="")
                {
                    iCT++;
                }
            }
        }
    }
    else
    {
        iCT=1;
    }

    HGem->InitLocalHead(102,21,1);                                              //S102,F21  傳回 Socket 目前使用次數
    HGem->DataItemOut(iCT,HType.LIST_TYPE,NULL);

    if(iRow==-1 && iCol==-1)
    {
        for(i=0; i<MAX_Index_Row; i++)
        {
            for(j=0; j<MAX_Index_Col; j++)
            {
                if( edSocketID[i][j]->Text!="")
                {
                    HGem->DataItemOut(3,HType.LIST_TYPE,NULL);
                    strcpy(str,edSocketID[i][j]->Text.c_str());
                    HGem->DataItemOut(strlen(str),HType.ASCII_TYPE,str);
                    strcpy(str,lblSiteLabel[i][j]->Caption.c_str());
                    HGem->DataItemOut(strlen(str),HType.ASCII_TYPE,str);
                    HGem->DataItemOut(1,HType.INT_4_TYPE,&Index);
                }
            }
        }
    }
    else
    {
        HGem->DataItemOut(3,HType.LIST_TYPE,NULL);
        strcpy(str,edSocketID[iRow][iCol]->Text.c_str());
        HGem->DataItemOut(strlen(str),HType.ASCII_TYPE,str);
        strcpy(str,lblSiteLabel[iRow][iCol]->Caption.c_str());
        HGem->DataItemOut(strlen(str),HType.ASCII_TYPE,str);
        HGem->DataItemOut(1,HType.INT_4_TYPE,&Index);
    }
    HGem->SendLocalData();
#endif
}
//---------------------------------------------------------------------------

void __fastcall TfSecsGem::btnGetSocketUseCountAaClick(TObject *Sender)
{
    if(SECSConnectionState->Caption=="Disconnection")
    {
        ShowMyMessage("Server Disconnection","伺服器斷線");
        return;
    }

#ifdef SECS_GEM
    int Tag,iRow=0,iCol=0;

    TSpeedButton *P;
    P=(TSpeedButton *)Sender;
    Tag=P->Tag;

    iRow=ChangeToFloatNonPcnt((double)(Tag), (double)(iMaxCol));
    iCol=Tag%iMaxCol;

    GetSocketUseCount(iRow,iCol);
#endif
}
//---------------------------------------------------------------------------
void TfSecsGem::WrireSocketUseCount()
{
#ifdef SECS_GEM
    char str[256];

    int i, j,iUseCT;
    int ct=0;
    for(i=0; i<iMaxRow; i++)
    {
        for(j=0; j<iMaxCol; j++)
        {
            if( edSocketID[i][j]->Text!="" && edSocketUseCount[i][j]->Text!="")
                ct++;
        }
    }

    HGem->InitLocalHead(102,23,1);                                              //S102,F23  設定 Socket 目前使用次數
    HGem->DataItemOut(ct,HType.LIST_TYPE,NULL);

    for(i=0; i<iMaxRow; i++)
    {
        for(j=0; j<iMaxCol; j++)
        {
            if( edSocketID[i][j]->Text!="" && edSocketUseCount[i][j]->Text!="")
            {
                HGem->DataItemOut(3,HType.LIST_TYPE,NULL);
                strcpy(str,edSocketID[i][j]->Text.c_str());
                HGem->DataItemOut(strlen(str),HType.ASCII_TYPE,str);
                iUseCT=atoi(edSocketUseCount[i][j]->Text.c_str());
                HGem->DataItemOut(1,HType.INT_4_TYPE,&iUseCT);
                strcpy(str,lblSiteLabel[i][j]->Caption.c_str());
                HGem->DataItemOut(strlen(str),HType.ASCII_TYPE,str);
            }
        }
    }
    HGem->SendLocalData();

#endif
}
//---------------------------------------------------------------------------
void __fastcall TfSecsGem::edtSocketIDAaChange(TObject *Sender)
{
    if(bShow==false || bAutoRunMode==true || bStartLoadCount==true)
        return;

    int Tag,iRow=0,iCol=0;
    TEdit *P;
    P=(TEdit *)Sender;

    iRow=ChangeToFloatNonPcnt((double)(P->Tag), (double)(iMaxCol));
    iCol=P->Tag%iMaxCol;

    if( P->Text!="")
        GetCountBtn[iRow][iCol]->Visible=true;
    else
        GetCountBtn[iRow][iCol]->Visible=false;
}
//---------------------------------------------------------------------------

void __fastcall TfSecsGem::FormShow(TObject *Sender)
{
    bool bFlag;
    int i, j,iRow=0,iCol=0;
    TabSheet1->TabVisible=false;

    GetRowCol(iRow,iCol);

    for(i=0; i<MAX_Index_Row; i++)
    {
        for(j=0; j<MAX_Index_Col; j++)
        {
            if(i<iRow && j <iCol && LastSet.bUseTestSocket[0][i][j]==true)
                bFlag=true;
            else
                bFlag=false;

            lblSiteLabel[i][j]->Visible=bFlag;
            edSocketID[i][j]->Visible=bFlag;
            edSocketUseCount[i][j]->Visible=bFlag;
            InputBtn[i][j]->Visible=bFlag;

            GetCountBtn[i][j]->Visible=false;

            if(edtOverContact->Text!="NA" && atoi(edSocketUseCount[i][j]->Text.c_str()) >=  atoi(edtOverContact->Text.c_str()))
            {
                lblSiteLabel[i][j]->Font->Color=clRed;
            }
            else
            {
                lblSiteLabel[i][j]->Font->Color=clBlack;
            }
        }
    }

    if(AccessLevel>=2)
        btnLoad->Visible=true;
    else
        btnLoad->Visible=false;

//    TabSheet2->Enabled=false;

    bShow=true;
}
//---------------------------------------------------------------------------

void __fastcall TfSecsGem::ProcessHostRetrunSocketNumber()
{
#ifdef SECS_GEM
    int  i,iLen,ACK[16],UseCount[16],iTypeIndex[16],iLifeTimeMax=0,iLifeTime[16];
    char Message[16][512];
    char cSiteIndex[16][64];

    if(HGem->GetDataItemLenAndTypeAndDelete(iLen,HType.LIST_TYPE)!=1)
        return;

    for(i=0; i<iLen; i++)
    {
        if( HGem->DataItemIn( 6,HType.LIST_TYPE,NULL)!=1)
            return;
        if( HGem->DataItemIn( 1,HType.INT_4_TYPE,&ACK[i])!=1)
            return;
        if( HGem->DataItemIn( 2,HType.ASCII_TYPE,cSiteIndex[i])!=1)
            return;

        if( HGem->DataItemIn( 1,HType.INT_4_TYPE,&UseCount[i])!=1)
            return;
        if( HGem->DataItemIn( 256,HType.ASCII_TYPE,Message[i])!=1)
            return;
        if( HGem->DataItemIn( 1,HType.INT_4_TYPE,&iTypeIndex[i])!=1)
            return;

        if( HGem->DataItemIn( 1,HType.INT_4_TYPE,&iLifeTime[i])!=1)
            return;

        if(iLifeTimeMax<iLifeTime[i])
            iLifeTimeMax=iLifeTime[i];
    }

    for(i=0; i<iLen; i++)
    {
        if(iTypeIndex[i]==0)
        {
            HostRetrunSocketNumber(ACK[i],cSiteIndex[i],UseCount[i],Message[i],iTypeIndex[i]);
        }
        else if(iTypeIndex[i]==1)
        {
            HostRetrunLoadBoardNumber(ACK[i],cSiteIndex[i],UseCount[i],Message[i],iTypeIndex[i]);
        }
    }

    if(iLifeTimeMax==0)
        edtOverContact->Text="NA";
    else
        edtOverContact->Text=iLifeTimeMax;

    WrireSocketUseCount();
#endif
}
//---------------------------------------------------------------------------

void __fastcall TfSecsGem::HostRetrunSocketNumber(int ACK,char *cSiteIndex,int UseCount,char *Message,int iTypeIndex)
{
#ifdef SECS_GEM
    int i, j,iRow=0,iCol=0;
    char str[256];

    for(i=0; i<MAX_Index_Row; i++)
    {
        for(j=0; j<MAX_Index_Col; j++)
        {
            if(lblSiteLabel[i][j]->Caption==cSiteIndex)
            {
                iRow=i;
                iCol=j;
            }
        }
    }

    if(ACK==0)
    {
        edSocketUseCount[iRow][iCol]->Text=UseCount;
        edSocketID[iRow][iCol]->Enabled=false;
        edSocketID[iRow][iCol]->Color=clWhite;
        GetCountBtn[iRow][iCol]->Visible=false;
        for(i=0; i<MAX_Index_Row; i++)
        {
            for(j=0; j<MAX_Index_Col; j++)
            {
                if(edSocketID[i][j]->Visible==true)
                    InputBtn[i][j]->Visible=true;
            }
        }
    }
    else if(ACK==1)                                                             //jou 2012-10-02 統一由sever建立socket number
    {
//        if(MessageDlg("Socket ID not exist , Create new socket ID ?", mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
//        {
//
//            strcpy(str,edSocketID[iRow][iCol]->Text.c_str());
//            if(strlen(str)==0)
//                return;
//            HGem->InitLocalHead(102,25,1);    //S102,F25  建立新Socket
//            HGem->DataItemOut(4,HType.LIST_TYPE,NULL);
//            HGem->DataItemOut(strlen(str),HType.ASCII_TYPE,str);
//            strcpy(str,lblSiteLabel[iRow][iCol]->Caption.c_str());
//            HGem->DataItemOut(strlen(str),HType.ASCII_TYPE,str);
//            UseCount=0;
//            HGem->DataItemOut(1,HType.INT_4_TYPE,&UseCount);
//            HGem->DataItemOut(1,HType.INT_4_TYPE,&iTypeIndex);
//
//            HGem->SendLocalData();
//        }
//        else
        if(MessageDlg("Socket ID not exist", mtError    , TMsgDlgButtons() << mbCancel, 0) == mrCancel )
        {
            edSocketID[iRow][iCol]->Text="";
            edSocketUseCount[iRow][iCol]->Text="";
            edSocketID[iRow][iCol]->Enabled=false;
            edSocketID[iRow][iCol]->Color=clWhite;
            GetCountBtn[iRow][iCol]->Visible=false;
            for(i=0; i<MAX_Index_Row; i++)
            {
                for(j=0; j<MAX_Index_Col; j++)
                {
                    if(edSocketID[i][j]->Visible==true)
                        InputBtn[i][j]->Visible=true;
                }
            }
        }
    }
    else if(ACK==2)
    {
        MessageDlg(Message, mtError , TMsgDlgButtons() << mbCancel  , 0);
        edSocketID[iRow][iCol]->Text="";
        edSocketUseCount[iRow][iCol]->Text="";
        edSocketID[iRow][iCol]->Enabled=false;
        edSocketID[iRow][iCol]->Color=clWhite;
        GetCountBtn[iRow][iCol]->Visible=false;
        for(i=0; i<MAX_Index_Row; i++)
        {
            for(j=0; j<MAX_Index_Col; j++)
            {
                if(edSocketID[i][j]->Visible==true)
                    InputBtn[i][j]->Visible=true;
            }
        }
    }
#endif
}
//---------------------------------------------------------------------------
void __fastcall TfSecsGem::HostRetrunLoadBoardNumber(int ACK,char *cSiteIndex,int UseCount,char *Message,int iTypeIndex)
{
#ifdef SECS_GEM
    int i, j,iRow=0,iCol=0;
    char str[256];

    for(i=0; i<MAX_Index_Row; i++)
    {
        for(j=0; j<MAX_Index_Col; j++)
        {
            if(lblSiteLabel[i][j]->Caption==cSiteIndex)
            {
                iRow=i;
                iCol=j;
            }
        }
    }

    if(ACK==0)
    {
        edSocketUseCount[iRow][iCol]->Text=UseCount;
        edSocketID[iRow][iCol]->Enabled=false;
        edSocketID[iRow][iCol]->Color=clWhite;
        GetCountBtn[iRow][iCol]->Visible=false;
        for(i=0; i<MAX_Index_Row; i++)
            for(j=0; j<MAX_Index_Col; j++)
                InputBtn[i][j]->Visible=true;
    }
    else if(ACK==1)
    {
        if(MessageDlg("LoadBoard ID not exist , Create new socket ID ?", mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
        {
            strcpy(str,edSocketID[iRow][iCol]->Text.c_str());
            if(strlen(str)==0)
                return;
            HGem->InitLocalHead(102,25,1);                                      //S102,F25  建立新Socket
            HGem->DataItemOut(4,HType.LIST_TYPE,NULL);
            HGem->DataItemOut(strlen(str),HType.ASCII_TYPE,str);
            strcpy(str,lblSiteLabel[iRow][iCol]->Caption.c_str());
            HGem->DataItemOut(strlen(str),HType.ASCII_TYPE,str);
            UseCount=0;
            HGem->DataItemOut(1,HType.INT_4_TYPE,&UseCount);
            HGem->DataItemOut(1,HType.INT_4_TYPE,&iTypeIndex);

            HGem->SendLocalData();
        }
        else
        {
            edSocketID[iRow][iCol]->Text="";
            edSocketUseCount[iRow][iCol]->Text="";
            edSocketID[iRow][iCol]->Enabled=false;
            edSocketID[iRow][iCol]->Color=clWhite;
            GetCountBtn[iRow][iCol]->Visible=false;
            for(i=0; i<MAX_Index_Row; i++)
                for(j=0; j<MAX_Index_Col; j++)
                    InputBtn[i][j]->Visible=true;
        }
    }
    else if(ACK==2)
    {
        MessageDlg(Message, mtError , TMsgDlgButtons() << mbCancel  , 0);
        edSocketID[iRow][iCol]->Text="";
        edSocketUseCount[iRow][iCol]->Text="";
        edSocketID[iRow][iCol]->Enabled=false;
        edSocketID[iRow][iCol]->Color=clWhite;
        GetCountBtn[iRow][iCol]->Visible=false;
        for(i=0; i<MAX_Index_Row; i++)
            for(j=0; j<MAX_Index_Col; j++)
                InputBtn[i][j]->Visible=true;
    }
#endif
}
//---------------------------------------------------------------------------

void __fastcall TfSecsGem::LoadSystem()
{
    char str[256];
    int i, j;

    edtOverContact->Text=CheckAndReadIniData(SecsGemPath,"Socket Manage","Socket Over Contact",100000);

    for(i=0; i<MAX_Index_Row; i++)
    {
        for(j=0; j<MAX_Index_Col; j++)
        {
            if(edSocketID[i][j]->Text=="")
            {
                sprintf(str,"Site %s Socket ID",lblSiteLabel[i][j]->Caption);
                edSocketID[i][j]->Text=CheckAndReadIniData(SecsGemPath,"Socket Manage",str,"");
            }

            if(edSocketUseCount[i][j]->Text=="")
            {
                sprintf(str,"Site %s Socket Use Count",lblSiteLabel[i][j]->Caption);
                edSocketUseCount[i][j]->Text=CheckAndReadIniData(SecsGemPath,"Socket Manage",str,"");
            }
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TfSecsGem::SaveSystem()
{
    char str[256];
    int i, j;

    WriteIniData(SecsGemPath,"Socket Manage","Socket Over Contact",atoi(edtOverContact->Text.c_str()));

    for(i=0; i<MAX_Index_Row; i++)
    {
        for(j=0; j<MAX_Index_Col; j++)
        {
            if( edSocketID[i][j]->Color==clWhite)
            {
                sprintf(str,"Site %s Socket ID",lblSiteLabel[i][j]->Caption);
                WriteIniData(SecsGemPath,"Socket Manage",str,edSocketID[i][j]->Text.c_str());
            }
            sprintf(str,"Site %s Socket Use Count",lblSiteLabel[i][j]->Caption);
            WriteIniData(SecsGemPath,"Socket Manage",str,edSocketUseCount[i][j]->Text.c_str());
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfSecsGem::FormClose(TObject *Sender, TCloseAction &Action)
{
    SaveSystem();
    Timer1->Enabled=false;
    bStartLoadCount=false;
    bAutoRunMode=false;
    bShow=false;
}
//---------------------------------------------------------------------------

void __fastcall TfSecsGem::btnLoadClick(TObject *Sender)
{
    int i, j;

    if(SECSConnectionState->Caption=="Disconnection")
    {
        ShowMyMessage("Server Disconnection","伺服器斷線");
        return;
    }

    LoadSystem();

    bStartLoadCount=true;
    iStartLoadCountTask=1;
    Timer1->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TfSecsGem::AddSocketCount()
{
    int iOverContact=0;
    bool bAlarm=false;
    AnsiString asString;

    int i, j,iRow=0,iCol=0;

    GetRowCol(iRow,iCol);

    if(edtOverContact->Text=="NA")
        iOverContact=0;
    else
        iOverContact=atoi(edtOverContact->Text.c_str());

    for(i=0; i<iMaxRow; i++)
    {
        for(j=0; j<iMaxCol; j++)
        {
            if(i<iRow && j <iCol && edSocketID[i][j]->Text!="" && edSocketID[i][j]->Visible==true
               && LastSet.bUseTestSocket[0][i][j]==true)
            edSocketUseCount[i][j]->Text=atoi(edSocketUseCount[i][j]->Text.c_str())+1;

            if((iOverContact>0) && (iOneCycle==0) && (atoi(edSocketUseCount[i][j]->Text.c_str())>iOverContact))
            {
                bAlarm=true;
                asString+=lblSiteLabel[i][j]->Caption+",";
            }
        }
    }

    if(bAlarm)
    {
        ShowMyMessage("Socket "+asString+" Site Over Contact : "+edtOverContact->Text,"Socket 壓測超過設定值");
        InitOneCycle();
        tsSocket1->Enabled=false;
        fMain->BtnOneCycle->Down=true;
    }
}
//---------------------------------------------------------------------------

HTimer hLoadTimeOut;
void TfSecsGem::StartLoadCount()
{
//    int iRow=0,iCol=0;
    int &Task=iStartLoadCountTask;

    switch(Task)
    {
        case 1:
            Task=100;
        case 100:
            RemoveSocketID(-1,-1);
            hLoadTimeOut.SetSec(2);
            hLoadTimeOut.On();
            Task=200;
            break;
        case 200:
            if(hLoadTimeOut.Off())
            {
                Task=100;
                break;
            }

            if(bRemoveSocketID==true)
            {
                Task=300;
            }
            else
            {
                break;
            }
        case 300:
            GetSocketUseCount(-1,-1);
            hLoadTimeOut.SetSec(2);
            hLoadTimeOut.On();
            Task=400;
            break;
        case 400:
            if(hLoadTimeOut.Off())
            {
                Task=500;
            }
            break;
        case 500:
            bStartLoadCount=false;
            iStartLoadCountTask=1;
            Timer1->Enabled=false;
            break;
    }
}
//---------------------------------------------------------------------------

HTimer hAutoTimeOut;
void TfSecsGem::AutoRunMode()
{
    static int i=0,j=0;
    int iRow=0,iCol=0;
    int &Task=iAutoRunModeTask;

    switch(Task)
    {
        case 1:
            i=0;
            j=0;
            Task=100;
        case 100:
            if(edSocketID[i][j]->Visible==true && edSocketID[i][j]->Text=="")
            {
                edSocketID[i][j]->Enabled=true;
                edSocketID[i][j]->Text="";
                edSocketID[i][j]->SetFocus();
                edSocketUseCount[i][j]->Text="";
                InputBtn[i][j]->Visible=false;
                edSocketID[i][j]->Color=clYellow;
                Task=150;
            }
            else
            {
                Task=500;
            }
            break;
        case 150:
            if(edSocketID[i][j]->Visible==true && edSocketID[i][j]->Text!="")
            {
                hAutoTimeOut.SetSec(0.5);
                hAutoTimeOut.On();
                Task=200;
            }
            break;
        case 200:
            if(hAutoTimeOut.Off())
            {
                edSocketID[i][j]->Enabled=false;
                edSocketID[i][j]->Color=clWhite;
                Task=500;
            }
            break;
        case 500:
            GetRowCol(iRow,iCol);
            j++;
            if(j>=iCol)
            {
                i++;
                j=0;
                if(i>=iRow)
                {
                    i=0;
                    j=0;
                    bAutoRunMode=false;
                    iAutoRunModeTask=1;
                    bStartLoadCount=true;
                    iStartLoadCountTask=1;
                }
            }
            Task=100;
            break;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfSecsGem::Timer1Timer(TObject *Sender)
{
    if(bStartLoadCount==true)
    {
        StartLoadCount();
    }
    else if(bAutoRunMode==true)
    {
        AutoRunMode();
    }
    DoToolingMsg();
}
//---------------------------------------------------------------------------
void __fastcall TfSecsGem::DoToolingMsg()
{
    /*
    static bool bShowBoard[8] = { false,false,false,false,false,false,false,false };
    static bool bShowSocket[16] = { false,false,false,false,false,false,false,false,
                                    false,false,false,false,false,false,false,false };

    TCheckBox *Open_Site[MAX_SITE] = { fMain->ChkB_Tester1, fMain->ChkB_Tester2,
                                  fMain->ChkB_Tester3, fMain->ChkB_Tester4,
                                  fMain->ChkB_Tester5, fMain->ChkB_Tester6,
                                  fMain->ChkB_Tester7, fMain->ChkB_Tester8 };

    float fPercent = fMaintenance->Edit_OverAlarrm->Text.ToIntDef(90) / 100.0;

    int i, j;
    for(i=0; i<8; i++)
    {
        if( HSys.LastScreenSet.bToolingAlarm==true)
        {
            if(Open_Site[i]->Checked==true)
            {
                if( edLoadBoardUseCount[i]->Text.ToIntDef(0) > edLoadBoardUseLimit[i]->Text.ToIntDef(3000))
                {
                    if( HSys.Sys.SystemStart==true)
                    {
                        if( HSys.LastScreenSet.bToolAlarmPuase==true)
                        {
                            HSys.Sys.SystemStart = false;
                        }
                        ShowToolingMessage(ToolMessage_BoardOverLimit, K_RETRY, Tool_On_Site0 + i, edLoadBoardID[i]->Text );
                        Open_Site[i]->Checked = false;
                    }
                }
                else if( edLoadBoardUseCount[i]->Text.ToIntDef(0) > edLoadBoardUseLimit[i]->Text.ToIntDef(3000)*fPercent )
                {
                    if(bShowBoard[i]==false)
                    {
                        if( HSys.LastScreenSet.bToolAlarmPuase==true)
                        {
                            HSys.Sys.SystemStart = false;
                        }
                        ShowToolingMessage(ToolMessage_BoardOverAlarm, K_RETRY, Tool_On_Site0 + i, edLoadBoardID[i]->Text );
                        bShowBoard[i] = true;
                    }
                }
                else
                {
                    bShowBoard[i] = false;
                }
            }
        }
        else
        {
            bShowBoard[i] = false;
        }
    }

    for(i=0; i<16; i++)
    {
        if( HSys.LastScreenSet.bToolingAlarm==true)
        {
            if(Open_Site[i%2]->Checked==true)
            {
                if( edSocketUseCount[i]->Text.ToIntDef(0) > edSocketUseLimit[i]->Text.ToIntDef(3000)    &&
                    fMain->pPanel_Socket[ i ]->Color==clTeal                                            )
                {
                    if(i % 2 == 1)
                    {
                        j = i - 1;
                    }
                    else
                    {
                        j = i + 1;
                    }

                    if( HSys.Sys.SystemStart==true)
                    {
                        if( HSys.LastScreenSet.bToolAlarmPuase==true)
                        {
                            HSys.Sys.SystemStart = false;
                        }
                        ShowToolingMessage(ToolMessage_SocketOverLimit, K_RETRY, Vaccum_On_Socket1 + j, edSocketID[i]->Text );
                        fMain->pPanel_Socket[ i ]->Color = clRed;
                    }
                }
                else if( edSocketUseCount[i]->Text.ToIntDef(0) > edSocketUseLimit[i]->Text.ToIntDef(3000)*fPercent )
                {
                    if(bShowSocket[i]==false)
                    {
                        if(i % 2 == 1)
                        {
                            j = i - 1;
                        }
                        else
                        {
                            j = i + 1;
                        }

                        if( HSys.LastScreenSet.bToolAlarmPuase==true)
                        {
                            HSys.Sys.SystemStart = false;
                        }
                        ShowToolingMessage(ToolMessage_SocketOverAlarm, K_RETRY, Vaccum_On_Socket1 + j, edSocketID[i]->Text );                                  //13.06.21.01   klutter
                        bShowSocket[i] = true;
                    }
                }
                else
                {
                    bShowSocket[i] = false;
                }
            }
        }
        else
        {
            bShowSocket[i] = false;
        }
    }
    */
}
//---------------------------------------------------------------------------

bool __fastcall TfSecsGem::CheckSocketState()
{
    int i, j,iRow=0,iCol=0;

    if(SECSConnectionState->Caption=="Disconnection")
    {
        ShowMyMessage("Server Disconnection","伺服器斷線");
        return false;
    }

    GetRowCol(iRow,iCol);

    for(i=0; i<iMaxRow; i++)
    {
        for(j=0; j<iMaxCol; j++)
        {
            if(i<iRow && j <iCol && LastSet.bUseTestSocket[0][i][j]==true)
            {
                if(edSocketID[i][j]->Text=="")
                {
                    ShowMyMessage("Socket ID error","Socket ID 未輸入");
                    fSecsGem->Show();
                    return false;
                }

                if(edSocketUseCount[i][j]->Text=="")
                {
                    ShowMyMessage("Socket Count error","Socket Count 未讀取");
                    fSecsGem->Show();
                    return false;
                }
            }
        }
    }

    return true;
}
//---------------------------------------------------------------------------

void __fastcall TfSecsGem::btnAutoRunClick(TObject *Sender)
{
    if(SECSConnectionState->Caption=="Disconnection")
    {
        ShowMyMessage("Server Disconnection","伺服器斷線");
        return;
    }

    bAutoRunMode=true;
    iAutoRunModeTask=1;
    Timer1->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TfSecsGem::edtOverContactMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TEdit *ptr=(TEdit *)Sender;
    MyInputBox(ptr);
    ptr->Text=AnsiString(CheckRandge(atoi(ptr->Text.c_str()), 1000000, 100));   //Steven 20111024 : 最小值改成1
}
//---------------------------------------------------------------------------

void __fastcall TfSecsGem::ReportAlarmWithMessage(AnsiString Code,AnsiString Message)
{
#ifdef SECS_GEM
    HGem->ReportAlarmWithMessage(Code,Message, fMain->StatusBar1->Panels->Items[8]->Text,"",false);
#endif
}
//---------------------------------------------------------------------------
void AddEditValue(TEdit *P,unsigned Value)
{
    unsigned old=atoi(P->Text.c_str());
    old+=Value;
    P->Text=old;
}

//---------------------------------------------------------------------------
void __fastcall TfSecsGem::SpeedButton2Click(TObject *Sender)
{
    AddEditValue(edSocketCount0,1000);
    AddEditValue(edSocketCount1,1001);
    AddEditValue(edSocketCount2,1002);
    AddEditValue(edSocketCount3,1003);
    AddEditValue(edSocketCount4,1004);
    AddEditValue(edSocketCount5,1005);
    AddEditValue(edSocketCount6,1006);
    AddEditValue(edSocketCount7,1007);
    AddEditValue(edSocketCount8,1008);
    AddEditValue(edSocketCount9,1009);
    AddEditValue(edSocketCount10,1010);
    AddEditValue(edSocketCount11,1011);
    AddEditValue(edSocketCount12,1012);
    AddEditValue(edSocketCount13,1013);
    AddEditValue(edSocketCount14,1014);
    AddEditValue(edSocketCount15,1015);
    AddEditValue(edLoadBoardCount0,1016);
    AddEditValue(edLoadBoardCount1,1017);
    AddEditValue(edLoadBoardCount2,1018);
    AddEditValue(edLoadBoardCount3,1019);
    AddEditValue(edLoadBoardCount4,1020);
    AddEditValue(edLoadBoardCount5,1021);
    AddEditValue(edLoadBoardCount6,1022);
    AddEditValue(edLoadBoardCount7,1023);
    AddEditValue(edKitCount0,1024);
}
//---------------------------------------------------------------------------

void __fastcall TfSecsGem::SpeedButton3Click(TObject *Sender)
{
#ifdef SECS_GEM
    TSpeedButton *P;
    P=(TSpeedButton *)Sender;
    HGem->ReportAlarm(P->Caption);
#endif
}
//---------------------------------------------------------------------------

void __fastcall TfSecsGem::Button2Click(TObject *Sender)
{
#ifdef SECS_GEM
    TButton *P;
    P=(TButton *)Sender;
    HGem->EventReport(1,atoi(P->Caption.c_str()));
#endif
}
//---------------------------------------------------------------------------

void __fastcall TfSecsGem::Button4Click(TObject *Sender)
{
#ifdef SECS_GEM
    HGem->GetAllSVInformation(Memo1);
#endif
}
//---------------------------------------------------------------------------

void __fastcall TfSecsGem::btnHomeClick(TObject *Sender)
{
#ifdef SECS_GEM
    HGem->EventReport(1,CEIDType.Home);
#endif
}
//---------------------------------------------------------------------------

void __fastcall TfSecsGem::btnOneCycleClick(TObject *Sender)
{
#ifdef SECS_GEM
    HGem->EventReport(1,CEIDType.OneCycle);
#endif
}
//---------------------------------------------------------------------------

void __fastcall TfSecsGem::btnCleanOutClick(TObject *Sender)
{
// 2013/07/29 lee
/*
#ifdef SECS_GEM
    HGem->EventReport(1,CEIDType.CleanOut);
#endif
*/
}
//---------------------------------------------------------------------------
void __fastcall TfSecsGem::SecsGemCleanOut()
{
// 2013/07/29 lee
#ifdef SECS_GEM
    HGem->EventReport(1,CEIDType.CleanOut);
#endif
}
//---------------------------------------------------------------------------
void __fastcall TfSecsGem::SecsGemLotStart()
{
// 2013/07/29 lee
#ifdef SECS_GEM
    HGem->EventReport(1,CEIDType.LotStart);
#endif
}
