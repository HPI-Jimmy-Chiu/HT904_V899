#include "MachineDefine.h"
#pragma hdrstop

#include "UsecegemMainFrom.h"

#include "uHGemClass.h"
#include "uHGemHT9045.h"
#include "cmydef.h"                                                             //wei 20150511
#include "uHGemEquipment.h"
#include "main.h"
#include "common.h"

#include "database.h"
#include "mymessbox.h"
#include "note.h"
//#include "csystem.h"
//#include "cprod.h"
#include "cTowerLight.h"
//#include "cConfiguration.h"
//#include "cShowBinSelect.h"
#include "cpublic.h"
#include "BarcodeReader.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFSECS *FSECS;
extern  bool InitialOK;
extern TMyStringList *tsSoftwareExeTime;                                        //Steven 20210526 : 紀錄軟體執行時間
extern TQPF_Timer  SoftwareExeTimer;
extern int iSoftwareExeTag;
extern void LogSoftwareOffTime(AnsiString Flag);
//---------------------------------------------------------------------------
__fastcall TFSECS::TFSECS(TComponent* Owner)
    : TForm(Owner)
{
    bLotStartByHost=false;
    bGemInitial=false;                                                          //Steven 20220511 : 確認有建構

//    CurrentDir = GetCurrentDir();
//    AnsiString S = CurrentDir;
//    while(S.Pos("\\")!=0)
//    {
//        S=S.Delete(1, S.Pos("\\"));
//    }
//    S=CurrentDir.SubString(1, CurrentDir.Pos("\\"+S)-1);
//    CurrentDir = S;

    // 2013/05/8 lee start
    TEdit *edSocketUseCountTemp[]=
        {
        edSocketCount0,edSocketCount1,edSocketCount2,
        edSocketCount3,edSocketCount4,edSocketCount5,
        edSocketCount6,edSocketCount7,edSocketCount8,
        edSocketCount9,edSocketCount10,edSocketCount11,
        edSocketCount12,edSocketCount13,edSocketCount14,
        edSocketCount15
        };

    TEdit *edLoadBoardUseCountTemp[8]=
        {
        edLoadBoardCount0,edLoadBoardCount1,edLoadBoardCount2,
        edLoadBoardCount3,edLoadBoardCount4,edLoadBoardCount5,
        edLoadBoardCount6,edLoadBoardCount7
        };

    TSpeedButton *GetCountBtn[16]=
        {
        spGetSocketUseCount0,spGetSocketUseCount1,spGetSocketUseCount2,
        spGetSocketUseCount3,spGetSocketUseCount4,spGetSocketUseCount5,
        spGetSocketUseCount6,spGetSocketUseCount7,spGetSocketUseCount8,
        spGetSocketUseCount9,spGetSocketUseCount10,spGetSocketUseCount11,
        spGetSocketUseCount12,spGetSocketUseCount13,spGetSocketUseCount14,
        spGetSocketUseCount15
        };

    TEdit *edSocketIDTemp[16]=
        {
        edSocketID0,edSocketID1,edSocketID2,
        edSocketID3,edSocketID4,edSocketID5,
        edSocketID6,edSocketID7,edSocketID8,
        edSocketID9,edSocketID10,edSocketID11,
        edSocketID12,edSocketID13,edSocketID14,
        edSocketID15
        };

    TEdit *edLoadBoardIDTemp[8]=
        {
            edLoadBoardID0,edLoadBoardID1,edLoadBoardID2,
            edLoadBoardID3,edLoadBoardID4,edLoadBoardID5,edLoadBoardID6,edLoadBoardID7
        };

    TSpeedButton *spInputSocketIDTemp[16]=
        {
        spInputSocketID0,spInputSocketID1,spInputSocketID2,
        spInputSocketID3,spInputSocketID4,spInputSocketID5,
        spInputSocketID6,spInputSocketID7,spInputSocketID8,
        spInputSocketID9,spInputSocketID10,spInputSocketID11,
        spInputSocketID12,spInputSocketID13,spInputSocketID14,
        spInputSocketID15
        };

    TSpeedButton *spGetLoadBoardUseCountTemp[8]=
        {
        spGetLoadBoardUseCount0,spGetLoadBoardUseCount1,spGetLoadBoardUseCount2,
        spGetLoadBoardUseCount3,spGetLoadBoardUseCount4,spGetLoadBoardUseCount5,
        spGetLoadBoardUseCount6,spGetLoadBoardUseCount7
        };
    TSpeedButton *spInputLoadBoardIDTemp[8]=
        {
        spInputLoadBoardID0,spInputLoadBoardID1,spInputLoadBoardID2,
        spInputLoadBoardID3,spInputLoadBoardID4,spInputLoadBoardID5,
        spInputLoadBoardID6,spInputLoadBoardID7
        };

    TEdit *edSocketUseLimitTemp[16]=
        {
        edSocketUseLimit0,edSocketUseLimit1,edSocketUseLimit2,edSocketUseLimit3,
        edSocketUseLimit4,edSocketUseLimit5,edSocketUseLimit6,edSocketUseLimit7,
        edSocketUseLimit8,edSocketUseLimit9,edSocketUseLimit10,edSocketUseLimit11,
        edSocketUseLimit12,edSocketUseLimit13,edSocketUseLimit14,edSocketUseLimit15
        };

    TEdit *edLoadBoardUseLimitTemp[8]=
        {
        edLoadBoardLimitCount0,edLoadBoardLimitCount1,edLoadBoardLimitCount2,edLoadBoardLimitCount3,
        edLoadBoardLimitCount4,edLoadBoardLimitCount5,edLoadBoardLimitCount6,edLoadBoardLimitCount7
        };

    for(int i=0; i<16; i++)
    {
        edSocketUseCount[i]     =edSocketUseCountTemp[i];
        spGetSocketUseCount[i]  =GetCountBtn[i];
        edSocketID[i]           =edSocketIDTemp[i];
        spInputSocketID[i]      =spInputSocketIDTemp[i];
        edSocketUseLimit[i]     =edSocketUseLimitTemp[i];
    }
    for(int i=0; i<8; i++)
    {
        edLoadBoardUseCount[i]      =edLoadBoardUseCountTemp[i];
        edLoadBoardID[i]            =edLoadBoardIDTemp[i];
        spGetLoadBoardUseCount[i]   =spGetLoadBoardUseCountTemp[i];
        spInputLoadBoardID[i]       =spInputLoadBoardIDTemp[i];
        edLoadBoardUseLimit[i]      =edLoadBoardUseLimitTemp[i];
    }
}
//------------------------------------------------------------------------------
void __fastcall TFSECS::FormCreate(TObject *Sender)
{
    TimerTooling->Enabled=true;
    TimerSecsAlarm->Enabled=true;
}
//---------------------------------------------------------------------------
//2013/06/30 Lee start

// 以下到 lee end 之前 之前只有  void __fastcall TfMain::SECS_SETData(THGem *Ptr)
// 可以全部取代
//------------------------------------------------------------------------------
AnsiString GetDownListContentString(TObject * Ptr)
{
    AnsiString S="";
    int ct, i;
    TComboBox   *cbPtr  = dynamic_cast <TComboBox *>(Ptr);
    TRadioGroup *rgPtr  = dynamic_cast <TRadioGroup *>(Ptr);
    if(cbPtr!=NULL)
    {
        ct=cbPtr->Items->Count;
        for(i=0; i<ct; i++)
        {
            S+=i;
            S+=":";
            S+=cbPtr->Items->Strings[i];
            if(i!=(ct-1))
                S+=" , ";
        }
    }
    else if(rgPtr!=NULL)
    {
        ct=rgPtr->Items->Count;
        for(i=0; i<ct; i++)
        {
            S+=i;
            S+=":";
            S+=rgPtr->Items->Strings[i];
            if(i!=(ct-1))
                S+=" , ";
        }
    }
    return S;
}
//------------------------------------------------------------------------------
void EventReport(unsigned Ceid)
{
    HGem->EventReport(1, Ceid);
}
//------------------------------------------------------------------------------
void __fastcall TFSECS::SECS_SETData()
{
    //============================================================================================================================================
    // 注意 : 當定義 ECID 也會同時定義 SVID 1..99 為系統內定 ID 不得使用
    // 當物件是 TComboBox 時 SV 要指向的是 Text ,EC 要指向的是 ItemIndex
    //============================================================================================================================================
    HSys.MyGem->AddSV();
    HSys.MyGem->AddEC();
    HSys.MyGem->AddAlarmList();
    HSys.MyGem->AddCEID();
    HSys.MyGem->AddReprot();
    HGem->SaveEventReportData();
}
//------------------------------------------------------------------------------
void TFSECS::AddECInfoFromData(int ECID, TObject *P)
{
    AnsiString Str;
    AnsiString FileName;

    FileName.sprintf("D:\\%s\\system\\%s_EC.csv", HSys.MyGem->HandlerPath, HSys.MyGem->HandlerPath);

    if(FileExists(FileName))
    {
        TStrings *sECList=new TStringList();
        TStrings *sListX=new TStringList();

        sECList->LoadFromFile(FileName);
        for(int i=1; i<sECList->Count; i++)
        {
            sListX->Clear();
            sListX->CommaText=sECList->Strings[i];
            if(sListX->Count>2)
            {
                if(ECID==atoi(sListX->Strings[0].c_str()))
                {
                    AnsiString ItemName     ="";
                    AnsiString MaxValue     ="";
                    AnsiString MinValue     ="";
                    AnsiString Default      ="";
                    AnsiString Description  ="";
                    AnsiString Type         ="";

                    ItemName    =sListX->Strings[1];
                    Type        =sListX->Strings[2];
                    if(sListX->Count>3)
                        MaxValue    =sListX->Strings[3];
                    if(sListX->Count>4)
                        MinValue    =sListX->Strings[4];
                    if(sListX->Count>7)
                        Default     =sListX->Strings[7];
                    if(sListX->Count>8)
                        Description =sListX->Strings[8];

                    if(Type=="FLOAT_8")
                    {
                        HGem->SetECDataPointer(ECID, HType.FT_8_TYPE,   ItemName, "", P, MinValue, MaxValue, Default, Description);
                    }
                    else if(Type=="FLOAT_4")
                    {
                        HGem->SetECDataPointer(ECID, HType.FT_4_TYPE,   ItemName, "", P, MinValue, MaxValue, Default, Description);
                    }
                    else if(Type=="INT_1")
                    {
                        HGem->SetECDataPointer(ECID, HType.INT_1_TYPE,   ItemName, "", P, MinValue, MaxValue, Default, Description);
                    }
                    else if(Type=="INT_2")
                    {
                        HGem->SetECDataPointer(ECID, HType.INT_2_TYPE,   ItemName, "", P, MinValue, MaxValue, Default, Description);
                    }
                    else if(Type=="INT_4")
                    {
                        HGem->SetECDataPointer(ECID, HType.INT_4_TYPE,   ItemName, "", P, MinValue, MaxValue, Default, Description);
                    }
                    else if(Type=="INT_8")
                    {
                        HGem->SetECDataPointer(ECID, HType.INT_8_TYPE,   ItemName, "", P, MinValue, MaxValue, Default, Description);
                    }
                    else if(Type=="ASCII")
                    {
                        HGem->SetECDataPointer(ECID, HType.ASCII_TYPE,   ItemName, "", P, MinValue, MaxValue, Default, Description);
                    }
                    else if(Type=="BOOL")
                    {
                        HGem->SetECDataPointer(ECID, HType.BOOLEAN_TYPE,   ItemName, "", P, MinValue, MaxValue, Default, Description);
                    }
                }
            }
        }
        sECList->Clear();                                                       //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        sListX->Clear();                                                        //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        delete sECList;
        delete sListX;
    }
    else
    {
        Str.sprintf("The file %s is gone!", FileName);
        HSys.MyGem->SecsAlarmMessage->Add(Str);                                 //Steven 20150519 : 修正SECS GEM使用ShowMyMessage會出現記憶體破壞
    }
}
//------------------------------------------------------------------------------
bool TFSECS::IsSupportRemoteCommand(AnsiString S)
{
    S=S.UpperCase();
    if(S=="MAINTANCE"  ||
       S=="SETUP"      ||
       S=="OPERATOR" )
        return true;
    return false;
}
//------------------------------------------------------------------------------
bool bWaitHostTransferReceipe=false;
bool bPauseByHost=false;
unsigned char __fastcall TFSECS::SECS_PublicRemoteCommand(AnsiString Command)
{
    int iParameterCount, SVlen, i, ret1, ret2, ret3, ret4, iV, Index;
    TStringList *Parameter;
    TStringList *Value;
//    char str[1024];
    unsigned char Type;
    AnsiString P,V;

    if(HGem->GetDataItemLenAndTypeAndDelete(iParameterCount, HType.LIST_TYPE)!=1)
        return 1;                                                               // format error
    Parameter=new TStringList;
    Value=new TStringList;
    Parameter->Clear();
    Value->Clear();
    for(i=0; i<iParameterCount; i++)
    {
        if(HGem->DataItemIn(2, HType.LIST_TYPE, NULL)!=1)
        {
            Parameter->Clear();                                                 //Ifor 20170603 (wei) TStringList 刪除前先 Clean
            Value->Clear();                                                     //Ifor 20170603 (wei) TStringList 刪除前先 Clean
            delete Parameter;
            delete Value;
            return 1;
        }
        ret1=HGem->GetDataItemLenAndType  ( SVlen,Type       );
        ret2=HGem->DataItemIn             ( SVlen,Type    ,P );
        ret3=HGem->GetDataItemLenAndType  ( SVlen,Type       );
        ret4=HGem->DataItemIn             ( SVlen,Type    ,V );
        if(ret1!=1 || ret2!=1 || ret3!=1 || ret4!=1)
        {
            Parameter->Clear();                                                 //Ifor 20170603 (wei) TStringList 刪除前先 Clean
            Value->Clear();                                                     //Ifor 20170603 (wei) TStringList 刪除前先 Clean
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

                TComboBox *Ptr[7]=                                              //20140124 wei
                {
                    fTowerLight->cbRunning, fTowerLight->cbJam, fTowerLight->cbPause, fTowerLight->cbMessage,
                    fTowerLight->cbHeating, fTowerLight->cbHome, fTowerLight->cbOffLine
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
            P=P.LowerCase();
            iV=atoi(V.c_str());
            Index=-1;
            if( P=="product_grape_a"   ) Index=0;
            if( P=="product_grape_b"   ) Index=1;
            if( P=="product_grape_c"   ) Index=2;
            if( P=="product_grape_d"   ) Index=3;
            if( P=="product_grape_e"   ) Index=4;
            if(Index!=-1)
            {
                if(iV<0) iV=0;                                                  //應該要更嚴謹 ALARM
                if(iV>1) iV=1;
                bWaitHostTransferReceipe=true;
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
        }
    }
    Parameter->Clear();                                                         //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    Value->Clear();                                                             //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete Parameter;
    delete Value;
    return 0;
}
//2013/06/30 Lee start
//---------------------------------------------------------------------------
AnsiString sBackSocketID[16];
AnsiString sBackSocketUseCount[16];
AnsiString sBackSocketUseLimit[16];

AnsiString sLoadBoardID[8];
AnsiString sLoadBoardUseCount[8];
AnsiString sLoadBoardUseLimit[8];

AnsiString sBackKitID0;
AnsiString sBackKitCount0;
AnsiString sBackKitLimitCount0;

int iYieldLimit=-1, iYieldLimitHigh=-1, iYieldLimitLow=-1;
AnsiString sCustomer;
AnsiString sSchedule;  // OK
AnsiString sLot;
AnsiString sDevice_type;   // OK
AnsiString sStation;
AnsiString sQuantity;
AnsiString sPkg_type;
AnsiString sHnd_setup_file;
AnsiString sTP_name;
AnsiString s2D_filename;  // OK
AnsiString s2D_Binfilename;
char sSocketIndexName[16][64];
char sSocketID[16][64];
int  iSocketUseCount[16];
int  iSocketMaxUseCountLimit[16];

char sTestFixtureIndexName[8][64];
char sTestFixtureID[8][64];
int  iTestFixtureUseCount[8];
int  iTestFixtureMaxUseCountLimit[8];

char sKitID[64];
int  iKitUseCount;
int  iKitMaxUseCountLimit;
//---------------------------------------------------------------------------
bool SECS_GEM_PPMUSIC_CONTROL_flag=false;
int  iSECS_GEM_PPMUSIC_CONTROL_CLASS=0;

bool SECS_GEM_PPSIGNALTOWER_CONTROL_flag=false;
int iSECS_GEM_PPSIGNALTOWER_CONTROL_RED=0;
int iSECS_GEM_PPSIGNALTOWER_CONTROL_GREEN=0;
int iSECS_GEM_PPSIGNALTOWER_CONTROL_YELLOW=0;
unsigned char CPACK=0 ;

// 2013/05/8 lee start
//------------------------------------------------------------------------------
void TFSECS::SetToolingData(bool flag)
{
    for(int i=0; i<16; i++)
    {
        if(flag==true)
        {
            edSocketID[i]->Text=sSocketID[i];
            edSocketUseCount[i]->Text=iSocketUseCount[i];
            edSocketUseLimit[i]->Text=iSocketMaxUseCountLimit[i];
        }
        else
        {
            if(sBackSocketID[i]!=sSocketID[i])
            {
                edSocketID[i]->Text=sSocketID[i];
                edSocketUseCount[i]->Text=iSocketUseCount[i];
            }
            edSocketUseLimit[i]->Text=iSocketMaxUseCountLimit[i];
        }
    }

    for(int i=0; i<8; i++)
    {
        if(flag==true)
        {
            edLoadBoardID[i]->Text=sTestFixtureID[i];
            edLoadBoardUseCount[i]->Text=iTestFixtureUseCount[i];
            edLoadBoardUseLimit[i]->Text=iTestFixtureMaxUseCountLimit[i];
        }
        else
        {
            if( sLoadBoardID[i]!=sTestFixtureID[i])
            {
                edLoadBoardID[i]->Text=sTestFixtureID[i];
                edLoadBoardUseCount[i]->Text=iTestFixtureUseCount[i];
            }
            edLoadBoardUseLimit[i]->Text=iTestFixtureMaxUseCountLimit[i];
        }
    }
    if(flag==true)
    {
        edKitID0->Text=sKitID;
        edKitCount0->Text=iKitUseCount;
    }
    else
    {
        if(sBackKitID0!=sKitID)
        {
            edKitID0->Text=sKitID;
            edKitCount0->Text=iKitUseCount;
        }
    }
    edKitLimitCount0->Text=iKitMaxUseCountLimit;
}
//------------------------------------------------------------------------------
int TFSECS::MySFCode()
{
    unsigned char Command, HCACK=0;//, CPACK;
    char CommandStr[1024];
//    char str[256],str1[256],str2[256],str3[256];
    AnsiString PPID;
//    const int i20K=20*1024;
//    unsigned char BinData[i20K];
//    unsigned inValidCount;

    AnsiString S, S1;
    if(HGem->Remote.MessageID_S==2 && HGem->Remote.MessageID_F==41)             //  S2,F41 Remote Command with Parameters
    {
        HCACK=HSys.MyGem->S2F42_Host_Command_Acknowledge();
        return 1;
    }
  /*  else if(HGem->Remote.MessageID_S==7 && HGem->Remote.MessageID_F==1)
    {
        HCACK=HSys.MyGem->S7F2_ProcessProgramLoadGrant();
        return 1;
    }      */                                                                   //Steven 20150519 : 修正SECS GEM使用ShowMyMessage會出現記憶體破壞
    else if(HGem->Remote.MessageID_S==7 && HGem->Remote.MessageID_F==23)
    {
        HCACK=HSys.MyGem->S7F24_FormattedProcessProgramSendAcknowledge();
        return 1;
    }
    else if(HGem->Remote.MessageID_S==7 && HGem->Remote.MessageID_F==25)
    {
        HCACK=HSys.MyGem->S7F26_FormattedProcessProgramData();
        return 1;
    }
  /*  else if(HGem->Remote.MessageID_S==10 && HGem->Remote.MessageID_F==3)
    {
        // 這裡是 user 自行定義的 Terminal Message 地方
        // CommandStr 是要顯示的訊息
        // 若 SECS 自訂地 message 不符合需求可以在此
        // 自訂顯示方式
        HSys.MyGem->S10F4_TerminalDisplaySingleAcknowledge();                   //wei 20150511 SECS_S10F4顯示  // [S10F4] Terminal Display Single Acknowledge
        return 1;
    }    */  //Steven 20150519 : 修正SECS GEM使用ShowMyMessage會出現記憶體破壞
    else if(HGem->Remote.MessageID_S==100 && HGem->Remote.MessageID_F==5)
    {
        if(HGem->DataItemIn(2, HType.LIST_TYPE,NULL)==1)
        {
            HGem->DataItemIn(1, HType.UINT_1_TYPE,&Command);
            HGem->DataItemIn(256, HType.ASCII_TYPE,CommandStr);
        }
        HGem->LocalAcknowledge(HGem->Remote.MessageID_S,HGem->Remote.MessageID_F+1, 0);
        return 1;
    }
    else if(HGem->Remote.MessageID_S==102 && HGem->Remote.MessageID_F==2)
    {
        HGem->DataItemIn(256, HType.ASCII_TYPE,CommandStr);
        return 1;
    }
    else if(HGem->Remote.MessageID_S==102 && HGem->Remote.MessageID_F==4)
    {
        return 1;
    }
    else if(HGem->Remote.MessageID_S==102 && HGem->Remote.MessageID_F==6)
    {
        return 1;
    }
    else if(HGem->Remote.MessageID_S==102 && HGem->Remote.MessageID_F==8)
    {
        return 1;
    }
    else if(HGem->Remote.MessageID_S==102 && HGem->Remote.MessageID_F==10)
    {
        return 1;
    }
    else if(HGem->Remote.MessageID_S==102 && HGem->Remote.MessageID_F==22)
    {
        return 1;
    }
    else if(HGem->Remote.MessageID_S==102 && HGem->Remote.MessageID_F==26)
    {
        return 1;
    }
    else if(HGem->Remote.MessageID_S==102 && HGem->Remote.MessageID_F==10)
    {
        return 1;
    }
    return HCACK;
}
//------------------------------------------------------------------------------
int GemCallBack()
{
    return FSECS->MySFCode();
}
//------------------------------------------------------------------------------
int ListTest[3]={150,151,156};
int SubListTest[3]={100,160};
double floatdata=12345.678;
//------------------------------------------------------------------------------
void TFSECS::GemInitialDefault()
{
    GemSBSetup->OnClick=HGem->GemSBSetupClick;
    HGem->SetCurrentDirectory("D:\\HT9045\\SECS\\SECS");
    HGem->SetDisplayPtr(GemCommMemo);
    HGem->MoveCheckCallBack=GemCallBack;
    HGem->SetTerminalWindows(GemTerminalWindow);                                //可以有 2 個 Terminal
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
//------------------------------------------------------------------------------
char szTestFixtureID[8][36];                                                    //2013_03_30 lee
char szSocketID[16][36];                                                        //2013_03_30 lee
//------------------------------------------------------------------------------
void TFSECS::GemInitial(AnsiString HandlerType, AnsiString SoftwareVersion)
{
    bGemInitial=true;                                                           //Steven 20220511 : 確認有建構
    GemInitialDefault();
    HGem->SetTimeFormat(1);                                                     // 0:12-bytes, 1:16-bytes, 2:14-bytes, 3:ISO8601 format
    HGem->SetDefaultAddressAndPort("127.0.0.1", "5098", "0");                   // 本機
    HGem->SetReceipeDirectoryAndGlobalName(HSys.MyGem->DataPath, "*.ini", 2);   //  0 : 只傳副檔名為 bld  //Steven 20140902 : 0 --> 2
    HGem->Alias="SECS";
    HGem->SetMachineTypeAndSoftwarseVer(HandlerType, SoftwareVersion);
    if(bReadSecsGemData==false)                                                 //wei 20160308 是否讀取SECSGEM資料
        SECS_SETData();
    HGem->Timer1->Enabled=true;
    AnsiString S;
    S=IntToHex(12, 2);
}
//------------------------------------------------------------------------------
void AddEditValue(TCustomEdit *P, unsigned Value)
{
    unsigned old=atoi(P->Text.c_str());
    old+=Value;
    P->Text=old;
}
//------------------------------------------------------------------------------
void __fastcall TFSECS::SpeedButton2Click(TObject *Sender)
{
    AddEditValue(edSocketCount0, 1000);
    AddEditValue(edSocketCount1, 1001);
    AddEditValue(edSocketCount2, 1002);
    AddEditValue(edSocketCount3, 1003);
    AddEditValue(edSocketCount4, 1004);
    AddEditValue(edSocketCount5, 1005);
    AddEditValue(edSocketCount6, 1006);
    AddEditValue(edSocketCount7, 1007);
}
//---------------------------------------------------------------------------
void __fastcall TFSECS::TimerToolingTimer(TObject *Sender)
{
    if(InitialOK==false)
    {
        return;
    }
    DoToolingMsg();
}
//---------------------------------------------------------------------------
void __fastcall TFSECS::DoToolingMsg()
{

}
//------------------------------------------------------------------------------
void __fastcall TFSECS::Button2Click(TObject *Sender)
{
    TButton *P;
    P=(TButton *)Sender;
    HGem->EventReport(1, atoi(P->Caption.c_str()));
}
//---------------------------------------------------------------------------
void __fastcall TFSECS::Button4Click(TObject *Sender)
{
    HGem->GetAllSVInformation(Memo1);
}
//---------------------------------------------------------------------------
void __fastcall TFSECS::FormShow(TObject *Sender)
{
    PageControl1->ActivePage=TabSheet1;
    InitialECStringGrid();

    HGem->GetALLECInformation(sgECIDList);
    sgECIDList->FixedRows=1;
    ReadDefineCCode();

    TabSheet2->TabVisible=false;
    TabSheet3->TabVisible=false;
    TabSheet4->TabVisible=false;
    TabSheet5->TabVisible=false;
    TabSheet10->TabVisible=false;
    TabSheet11->TabVisible=false;
    tsCCode->TabVisible=false;
    l2DBarCodeFilename->Visible=false;
    ed2DBarCodeFilename->Visible=false;

    if(CUSTOMER_CODE==CC_KYEC_LEE)    //wei 20150817 SECSGEM 斷線輸入密碼
        GroupBox1->Visible=true;
    else
        GroupBox1->Visible=false;
}
//---------------------------------------------------------------------------
void TFSECS::InitialECStringGrid()
{
    for(int i=0; i<sgECIDList->RowCount; i++)
    {
        for(int j=0; j<sgECIDList->ColCount; j++)
        {
            sgECIDList->Cells[j][i]="";
        }
    }

    sgECIDList->RowCount=1;
    sgECIDList->Cells[0][0]="EC ID";
    sgECIDList->Cells[1][0]="EC NAME";
    sgECIDList->Cells[2][0]="LENGTH";
    sgECIDList->Cells[3][0]="TYPE";
    sgECIDList->Cells[4][0]="MIN";
    sgECIDList->Cells[5][0]="MAX";
    sgECIDList->Cells[6][0]="UNIT";
    sgECIDList->Cells[7][0]="DEFAULT";
    sgECIDList->Cells[8][0]="REMARK";
    sgECIDList->ColWidths[1]=300;
    sgECIDList->ColWidths[8]=500;

    for(int i=0; i<sgCCodeList->RowCount; i++)
    {
        for(int j=0; j<sgCCodeList->ColCount; j++)
        {
            sgCCodeList->Cells[j][i]="";
        }
    }
    sgCCodeList->Cells[0][0]="CCode";
    sgCCodeList->Cells[1][0]="Items";
    sgCCodeList->RowCount=1;
    sgCCodeList->FixedCols=1;
    iCurrentCCodeRow=-1;
}
//------------------------------------------------------------------------------
void __fastcall TFSECS::spbNewCCodeClick(TObject *Sender)
{
    int iRow;

    if(edCCodeAppend->Text=="")
    {
        return;
    }

    for(int i=1; i<sgCCodeList->RowCount; i++)
    {
        if(sgCCodeList->Cells[0][i]==edCCodeAppend->Text)
        {
            ShowMessage("Duplicate CCode Define!!!");
            return;
        }
    }
    iRow=sgCCodeList->RowCount;
    sgCCodeList->RowCount=sgCCodeList->RowCount+1;
    sgCCodeList->Cells[0][iRow]=edCCodeAppend->Text;

    if(sgCCodeList->RowCount>1)
    {
        sgCCodeList->FixedRows=1;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFSECS::sgCCodeListMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int ARow, ACol;
    TStringGrid *P;
    AnsiString CCode;

    P=(TStringGrid *)Sender;

    P->MouseToCell(X, Y, ACol, ARow);
    iCurrentCCodeRow=ARow;
    if(ARow<=0)
    {
        return;
    }
    if(ACol<0)
    {
        iCurrentCCodeRow=-1;
        return;
    }

    CCode=P->Cells[0][ARow];
    if(CCode!="")
        edAciveCCode->Text=CCode;
}
//---------------------------------------------------------------------------
void __fastcall TFSECS::sgECIDListMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    bool bAdd;
    int ARow, ACol, iCol, iRow;
    TStringGrid *P;
    AnsiString CEID;

    P=(TStringGrid *)Sender;

    P->MouseToCell(X, Y, ACol, ARow);
    if(ARow<=0)
        return;
    if(ACol<0)
        return;

    CEID=P->Cells[0][ARow];
    if(edAciveCCode->Text=="")
    {
        ShowMessage("Must slect or define CEID ID first !!!");
        return;
    }
    iRow=iCurrentCCodeRow;

    if(iRow==-1)
    {
        ShowMessage("Error CCode , Must slect or define CCode first !!!");
        return;
    }
    for(iCol=1; iCol<sgCCodeList->ColCount; iCol++)
    {
        if(sgCCodeList->Cells[iCol][iRow]==CEID)
        {
            ShowMessage("Repeat define CEID at same CCode , ReSelect again !!!");
            return;
        }
    }

    bAdd=false;
    for(iCol=1; iCol<sgCCodeList->ColCount; iCol++)
    {
        if(sgCCodeList->Cells[iCol][iRow]=="")
        {
            sgCCodeList->Cells[iCol][iRow]=CEID;
            bAdd=true;
            break;
        }
    }

    if(bAdd==false)
    {
        iCol=sgCCodeList->ColCount;
        sgCCodeList->ColCount=sgCCodeList->ColCount+1;
        sgCCodeList->Cells[iCol][iRow]=CEID;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFSECS::apbUpdateCCodeClick(TObject *Sender)
{
    TStringList *memoPtr;
    AnsiString Filename;
    memoPtr=new TStringList;

    HGem->CopyStringGridAsTabFormat(sgCCodeList, memoPtr);
    Filename=IncludeTrailingPathDelimiter(HGem->GemSystemPath)+sgCCodeList->Name+".def";
    memoPtr->SaveToFile(Filename);
    memoPtr->Clear();                                                           //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete memoPtr;
}
//---------------------------------------------------------------------------
void __fastcall TFSECS::ReadDefineCCode()
{
    TStringList *memoPtr;
    AnsiString Filename;
    memoPtr=new TStringList;

    Filename=IncludeTrailingPathDelimiter(HGem->GemSystemPath)+sgCCodeList->Name+".def";

    if(FileExists(Filename)==false)
    {
        memoPtr->Clear();                                                       //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        delete memoPtr;
        return;
    }

    memoPtr->LoadFromFile(Filename);
    sgCCodeList->RowCount=memoPtr->Count;
    HGem->PasteStringGridAsTabFormat(sgCCodeList, memoPtr);
    if(sgCCodeList->RowCount>1)
        sgCCodeList->FixedRows=1;
    memoPtr->Clear();                                                           //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete memoPtr;
}
//---------------------------------------------------------------------------
void __fastcall TFSECS::FormDestroy(TObject *Sender)
{
    AnsiString str;
    if(bGemInitial==true)                                                       //Steven 20220511 : 確認有建構
    {
        try
        {
            TimerTooling->Enabled=false;
            TimerSecsAlarm->Enabled=false;

//            HGem->SetDisplayPtr(NULL);
//            HGem->MoveCheckCallBack=NULL;
//            HGem->SetTerminalWindows(NULL);                                   //可以有 2 個 Terminal
//            HGem->SetTerminalWindows2(NULL);
//            HGem->SECSConnectionState           =NULL;
//            HGem->GEMCommunicatingState         =NULL;
//            HGem->GemPanelControlState          =NULL;
//            HGem->GemBtnSendTerminalMessage     =NULL;
//            GemBtnSendTerminalMessage->OnClick  =NULL;
//            HGem->GemTerminalSendEdit           =NULL;
//            GemTerminalSendEdit->OnKeyDown      =NULL;
//            HGem->GemRemoteReceipeList          =NULL;
//            HGem->GemListRemoteFileName         =NULL;
//            GemListRemoteFileName->OnClick      =NULL;
//            HGem->GemDownLoadRemoteFileName     =NULL;
//            GemDownLoadRemoteFileName->OnClick  =NULL;
//            HGem->GemDeleteRemoteFileName       =NULL;
//            GemDeleteRemoteFileName->OnClick    =NULL;
//            HGem->GemRefreshLocalFile           =NULL;
//            GemRefreshLocalFile->OnClick        =NULL;
//            HGem->GemUpLoadLocalFileName        =NULL;
//            GemUpLoadLocalFileName->OnClick     =NULL;
//            HGem->GemSelectAllRemoteFile        =NULL;
//            GemSelectAllRemoteFile->OnClick     =NULL;
//            HGem->GemDisSelectAllRemoteFile     =NULL;
//            GemDisSelectAllRemoteFile->OnClick  =NULL;
//            HGem->GemSelectAllLocalFile         =NULL;
//            GemSelectAllLocalFile->OnClick      =NULL;
//            HGem->GemDisSelectAllLocalFile      =NULL;
//            GemDisSelectAllLocalFile->OnClick   =NULL;
//            HGem->GemLocalFileLixtBox           =NULL;
//            HGem->BtnEnableComm                 =NULL;
//            BtnEnableComm->OnClick              =NULL;
//            HGem->BtnDisableComm                =NULL;
//            BtnDisableComm->OnClick             =NULL;
//            HGem->GemBtnOnlineRequest           =NULL;
//            GemBtnOnlineRequest->OnClick        =NULL;
//            HGem->GemBtnOfflineRequest          =NULL;
//            GemBtnOfflineRequest->OnClick       =NULL;
//            HGem->GemBtnOnlineRemote            =NULL;
//            GemBtnOnlineRemote->OnClick         =NULL;
//            HGem->GemBtnOnlineLocal             =NULL;
//            GemBtnOnlineLocal->OnClick          =NULL;
//            HGem->EnableOrDisablePtr            =NULL;
        //    Close();
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TFSECS::FormDestroy");
        }
    }
    LogSoftwareOffTime("TFSECS, FormDestroy");
}
//---------------------------------------------------------------------------
void __fastcall TFSECS::TimerSecsAlarmTimer(TObject *Sender)                    //Steven 20150519
{
    static bool bRun=false;

    AnsiString str;
    if(InitialOK==false)
    {
        return;
    }

    if(bRun==true)
    {
        return;
    }
    bRun=true;

    if(MyMessageBox->fShow==true)
    {
        if(CUSTOMER_CODE==CC_MAXIM_THAILAND)                                    //Ifor 20251018 有新資料需更新
        {

        }
        else if(CUSTOMER_CODE==CC_KYEC_LEE)                                     //Ifor 20170616 (wei) KYEC add  PreAlarm時不Return
        {
            if(iUnLoaderCount==0)
            {
                bRun=false;
                return;
            }
        }
        else
        {
            bRun=false;
            return;
        }
    }

    if(CUSTOMER_CODE==CC_KYEC_LEE)                                              //Ifor 20170425 add KYEC 要求 SECS GEM Message 要在最上層
    {
        if(HSys.MyGem->SecsAlarmMessage->Count==0)
        {
            bRun=false;
            return;
        }
    }
//    else                                                                      //JerryYang 20170907 (wei) Mark掉,為了讓note及message能同時show
//    {
//        if(fNote->fShow==true)                                                //JerryYang 20161213 先處理完note才show，避免Note及message同時showmodal可能會畫面卡死
//        {
//            return;
//        }
//    }

    if(CUSTOMER_CODE==CC_KYEC_LEE && FormBarcodeReader->bShow)                  //Ifor 20170616 (wei) KYEC add 避免BarCode From開啟時 Show SECSGEM Alrm 卡死
    {
        FormBarcodeReader->Close();
        bRun=false;
        return;
    }

    if(HSys.MyGem->SecsAlarmMessage->Count!=0)
    {
        if(CUSTOMER_CODE==CC_MAXIM_THAILAND)                                    //Ifor 20251018 add:收到新的SECS Message 需關閉後重新顯示訊息
        {
            if(MyMessageBox->Visible==true)
            {
                MyMessageBox->Close();
                bRun=false;
                return;
            }
        }

        if(bAlarmAfterPreAlarm==false)                                          //Ifor 20170906 (wei) add 避免 PreAlarm -> Alarm -> SECS GEM Alarm 同時發生造成當機問題
        {
            bSECSGEMAlarm=true;                                                 //wei 20150817 S10F3 Alarm Reset畫面
            str=HSys.MyGem->SecsAlarmMessage->Strings[0];                       //Ifor 20251018 add:取出資料並刪除避免Secs Alarm Message Count 異常
            HSys.MyGem->SecsAlarmMessage->Delete(0);
            ShowMyMessage(str);//HSys.MyGem->SecsAlarmMessage->Strings[0]);     //Steven 20251204 : fixed for SECS GEM
        }
    }
    bRun=false;
}
//---------------------------------------------------------------------------
void __fastcall TFSECS::spEnterClick(TObject *Sender)
{
    if(edSecsGemPassWord->Text==asSECSGEMPassWord)                              //wei 20150817 SECSGEM 斷線輸入密碼
    {
        bSECSGEMbyPass=true;
        edSecsGemPassWord->Text="";
    }
}
//---------------------------------------------------------------------------
void __fastcall TFSECS::btnS7F6Click(TObject *Sender)
{
    HSys.MyGem->S7F6_ProcessProgramData(fMain->cbSetupFileName->Text);
}
//---------------------------------------------------------------------------