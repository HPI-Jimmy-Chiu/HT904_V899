#include "MachineDefine.h"
#pragma hdrstop

#include "OmronEJ1N.h"
//#include "TextProcess.h"
#include "myQwertyKeyBoard.h"
#include "cmydef.h"
#include "common.h"
#include "ExternFunction.h"
#include "database.h"
//#include "PComm.h"

#include "ATC_Handler_Side.h"
#include "bthermo.h"
//------------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "SPComm"
#pragma link "ALed"
#pragma link "MyLed"
#pragma resource "*.dfm"
TfOmron *fOmron;
TRS232Thread  *My232Thread;
TOmronProcessThread *MyOmronThread;
int iDelaySecc=5;                                                               //JerryYang : 20200317 : 1 --> 5
//------------------------------------------------------------------------------
__fastcall TRS232Thread::TRS232Thread(bool CreateSuspended)
    : TThread(CreateSuspended)
{
}
//------------------------------------------------------------------------------
void __fastcall TRS232Thread::RS232ThreadProcess(void)
{
    fOmron->Main232();
}
//------------------------------------------------------------------------------
void __fastcall TRS232Thread::Execute()                                         //Rs-232執行緒
{
    do
    {
        Synchronize(RS232ThreadProcess);                                        //用這個會影響主程式的繪圖效能
        MySleepEx(5, true);                                                     //jou 2012-08-09 沒加Sleep
    } while (!Terminated);
}
//------------------------------------------------------------------------------
__fastcall TOmronProcessThread::TOmronProcessThread(bool CreateSuspended)
    : TThread(CreateSuspended)
{
}
//------------------------------------------------------------------------------
void __fastcall TOmronProcessThread::OmronThreadProcess(void)
{
    fOmron->Timer1Timer();
}
//------------------------------------------------------------------------------
void __fastcall TOmronProcessThread::Execute()                                  //Omron執行緒
{
    do
    {
        Synchronize(OmronThreadProcess);
        MySleepEx(50, true);                                                    //Steven 20200527 : 100 --> 50
    } while (!Terminated);
}
//------------------------------------------------------------------------------
__fastcall TfOmron::TfOmron(TComponent* Owner)
    : TForm(Owner)
{
    SendBufferASCII="";
    SendBufferTEXT="";
    bRecvData       =false;
    bReadPV         =false;
    bReadSV         =false;
    iSetSV          =0;
    bAT             =false;
    bATOneByOne     =false;
    bRunStop        =false;
    bSetThermoType  =false;
    bSendCommand    =false;
    bGetComponent   =false;
    bShow           =false;
    bGetComponentFinish=false;

    iCode       =-4;
    iCommPort   =12;
    bInitComm   =false;
    asTStrTemp="";
    asRStr="";
    asTStr="";
    asBCC="";
    asBCCConfirm="";
    iChanel=0;
    iShowLevel=0;

    iOmronTask=1;
    int i, j;
    AnsiString str;
    bReadError=false;
    for(i=0; i<TOTAL_OMRON_UNIT; i++)
    {
        bModulType[i]=0;
        for(j=0; j<4; j++)
        {
            str.sprintf("CH%d-%d", i+1, j+1);
            myPal[i][j]                     = new TMyOmronPanel(this);
            myPal[i][j]->GroupBox->Parent   = ScrollBox1;
            myPal[i][j]->GroupBox->Caption  = str;
            str.sprintf("edSV_CH%d%d", i+1, j+1);
            myPal[i][j]->edSV->Name         = str;
            myPal[i][j]->GroupBox->Visible  = false;
            myPal[i][j]->iMyTag             = i*4+j;
            myPal[i][j]->edSV->Text         = AnsiString(30);

            dTempValue[i][j]=0.0;
            dSetValue[i][j]=0.0;
            bATStart[i][j]=false;
            bRunStopStatus[i][j]=false;
            bInputError[i][j]=false;
            bEvent[i][j][0]=false;
            bEvent[i][j][1]=false;
            bEvent[i][j][2]=false;
        }
    }

    sResponseCode[0][rc0000]="0000"; sResponseCode[1][rc0000]="Response Code OK.";
    sResponseCode[0][rc1001]="1001"; sResponseCode[1][rc1001]="命令過長";
    sResponseCode[0][rc1002]="1002"; sResponseCode[1][rc1002]="命令過短";
    sResponseCode[0][rc1101]="1101"; sResponseCode[1][rc1101]="區域類型錯誤, 對應的變數類型並不存在";
    sResponseCode[0][rc1003]="1003"; sResponseCode[1][rc1003]="數量不符, 數據數量與元素數量不符";
    sResponseCode[0][rc1100]="1100"; sResponseCode[1][rc1100]="參數錯誤";
    sResponseCode[0][rc110B]="110B"; sResponseCode[1][rc110B]="回應過長, 元素數量超出最大值";
    sResponseCode[0][rc2201]="2201"; sResponseCode[1][rc2201]="操作錯誤, 因操作停止導致命令無法執行";
    sResponseCode[0][rc2202]="2202"; sResponseCode[1][rc2202]="操作錯誤, 操作中不可執行命令";
    sResponseCode[0][rc2203]="2203"; sResponseCode[1][rc2203]="操作錯誤, 正常模式下無法執行命令";
    sResponseCode[0][rc0402]="0402"; sResponseCode[1][rc0402]="操作錯誤, 型號或版本不符無法操作命令";
    sResponseCode[0][rc3003]="3003"; sResponseCode[1][rc3003]="唯讀錯誤, 嘗試寫入唯讀變數";
    sResponseCode[0][rc7011]="7011"; sResponseCode[1][rc7011]="操作錯誤, 因設備出錯導致命令不能執行";
    sResponseCode[0][rc7012]="7012"; sResponseCode[1][rc7012]="操作錯誤, 因配置錯誤導致無法執行命令";
    sResponseCode[0][rc7013]="7013"; sResponseCode[1][rc7013]="操作錯誤, 因無法接收操作命令而導致無法執行命令";
    sResponseCode[0][rc7014]="7014"; sResponseCode[1][rc7014]="操作錯誤, 備份中無法執行命令";
    sResponseCode[0][rc7015]="7015"; sResponseCode[1][rc7015]="操作錯誤, 復位與啟動時無法執行命令";
    sResponseCode[0][rc7016]="7016"; sResponseCode[1][rc7016]="操作錯誤, 錯誤保持狀態下無法執行命令";
    sResponseCode[0][rc7020]="7020"; sResponseCode[1][rc7020]="操作錯誤, 電流控制模式下無法執行命令";
    sResponseCode[0][rc7021]="7021"; sResponseCode[1][rc7021]="操作錯誤, 因還未建立控制開關條件導致無法執行命令";
    sResponseCode[0][rc7030]="7030"; sResponseCode[1][rc7030]="操作錯誤, 電流輸出模式下無法執行命令";
    sResponseCode[0][rc7041]="7041"; sResponseCode[1][rc7041]="操作錯誤, 手動操作模式下無法執行命令";
    sResponseCode[0][rc7042]="7042"; sResponseCode[1][rc7042]="操作錯誤, 調整模式下無法執行命令";

    sEndCode[0][ec00]="00"; sEndCode[1][ec00]="End Code OK.";
    sEndCode[0][ec0F]="0F"; sEndCode[1][ec0F]="FINS command error, 無法執行指定的FINS命令";
    sEndCode[0][ec10]="10"; sEndCode[1][ec10]="Parity error, 數據接收中的1位OR與通信校驗中設定不符合";
    sEndCode[0][ec11]="11"; sEndCode[1][ec11]="Framing erro, stop bit is 0.";
    sEndCode[0][ec12]="12"; sEndCode[1][ec12]="Overrun error, 數據暫存已滿時收新數據";
    sEndCode[0][ec13]="13"; sEndCode[1][ec13]="BCC error, BCC檢查碼不符";
    sEndCode[0][ec14]="14"; sEndCode[1][ec14]="Format error, 格式錯誤";
    sEndCode[0][ec16]="16"; sEndCode[1][ec16]="Sub-address error, 子地址不存在";
    sEndCode[0][ec18]="18"; sEndCode[1][ec18]="Frame length error, 接收到的數量超出指定字節數";

    bResetCom=true;                                                             //開啟COM Port
    SetPanelPos(false);                                                         //設定畫面

    MyOmronThread=new TOmronProcessThread(true);
    MyOmronThread->Priority=tpNormal;

    #ifdef SOFT_SIMULTE
    for(i=0; i<TOTAL_OMRON_UNIT; i++)
    {
        bModulType[i]=4;
    }
    #endif

    LedEJ1N[0]=ledEJ1N1;                                                        //Steven 20210305 : EJ1N加入狀態顯示
    LedEJ1N[1]=ledEJ1N2;
    LedEJ1N[2]=ledEJ1N3;
    LedEJ1N[3]=ledEJ1N4;
    LedEJ1N[4]=ledEJ1N5;
    LedEJ1N[5]=ledEJ1N6;
    LedEJ1N[6]=ledEJ1N7;
    LedEJ1N[7]=ledEJ1N8;
    for(i=0; i<TOTAL_OMRON_UNIT; i++)
    {
        LedEJ1N[i]->TrueColor=clLime;
        LedEJ1N[i]->FalseColor=clSilver;
        LedEJ1N[i]->Value=false;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfOmron::FormShow(TObject *Sender)
{
    cbMrcSrc->ItemIndex=0;
    cbMrcSrcChange(this);
    Left=10;                                                                    //Steven 20220407 : 避免跑到畫面外
    Top =10;
    bShow=true;
    PageControl1->ActivePageIndex=0;
    SetPanelPos(bShow);                                                         //設定畫面
    Timer2->Enabled=true;

    if(iShowLevel<2)                                                            //Steven 20141027 : > --> <
    {
        tsAuto->TabVisible=false;
        tsManual->TabVisible=false;
    }
    bSaveData=false;
    if(USE_16_HEATER==eht16HeaterDTME08  ||                                     //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
       USE_16_HEATER==eht32HeaterDTME08)                                        //JimmyChiu 20210923 : Index使用DTME08版32組加熱器
    {
        PageControl1->ActivePageIndex=2;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfOmron::FormClose(TObject *Sender, TCloseAction &Action)
{
    Timer2->Enabled=false;
    bShow=false;
}
//------------------------------------------------------------------------------
void __fastcall TfOmron::FormDestroy(TObject *Sender)
{
    try
    {
        if(MyOmronThread!=NULL)                                                 //ChungHung 20140617 in main form dormdestory already delete so if exec will show error
            MyOmronThread->Terminate();

        delete MyOmronThread;
        MyOmronThread=NULL;

        Timer2->Enabled=false;
        if(bInitComm)
        {
            if(My232Thread!=NULL)                                               //ChungHung 20140617 in main form dormdestory already delete so if exec will show error
                My232Thread->Terminate();
            RS232_CloseDevice();                                                //關閉Rs-232通訊埠
        }

        for(int i=0; i<TOTAL_OMRON_UNIT; i++)
        {
            for(int j=0; j<4; j++)
            {
                if(myPal[i][j]!=NULL)                                           //Steven 20161220 (jou) : 修正delete方式
                    delete myPal[i][j];
                myPal[i][j]=NULL;                                               //kevin 20160825
            }
        }
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfOmron::FormDestroy");
    }
    LogSoftwareOffTime("TfOmron, FormDestroy");                                 //Steven 20210526 : 紀錄軟體執行時間
}
//------------------------------------------------------------------------------
void TfOmron::SetPanelPos(bool bInitial)
{
    int iTop2=0, iLeft2=0;
    int iHeight=124, iWidth=210;
    int i, j;
    int iTop[4] ={5, 129,   5, 129};
    int iLeft[4]={2,   2, 212, 212};

    //排列方式:
    // 1-1, 1-3, 2-1, 2-3
    // 1-2, 1-4, 2-2, 2-4
    for(i=0; i<TOTAL_OMRON_UNIT; i++)
    {
        if(i==0)
        {
            iTop2=0;
            iLeft2=0;
        }
        else if(i%2==0)                                                         //Steven 20200316 : 修正Omron溫控顯示位置
        {
            iTop2+=iHeight*2;
            iLeft2=0;
        }
        else
        {
            iLeft2=iWidth*2;
        }

        for(j=0; j<4; j++)
        {
            if(j>=bModulType[i])
                myPal[i][j]->GroupBox->Visible=false;
            else
                myPal[i][j]->GroupBox->Visible=bInitial;

            myPal[i][j]->GroupBox->Left =iLeft[j]+iLeft2;
            myPal[i][j]->GroupBox->Top  =iTop[j]+iTop2;
        }
    }

    if(bInitial==false)
        SetButton(0);
    else
        SetButton(1);

}
//------------------------------------------------------------------------------
void TfOmron::SetButton(int Function)                                           //0: 只有init, 1:全開, 2:全關, 3:只有Send, 4:只有Reset
{
    if(Function==1)
    {
        btGetModuler    ->Enabled=true;
        btAT            ->Enabled=true;
        btAT1by1        ->Enabled=true;
        btRT            ->Enabled=true;
        btRT1by1        ->Enabled=true;
        btSetSV         ->Enabled=true;
        btRunStop       ->Enabled=true;
        btSetThermoType ->Enabled=true;
        btResetCOM      ->Enabled=true;
        btSend          ->Enabled=true;
    }
    else
    {
        btGetModuler    ->Enabled=(Function==0);
        btAT            ->Enabled=false;
        btAT1by1        ->Enabled=false;
        btRT            ->Enabled=false;
        btRT1by1        ->Enabled=false;
        btSetSV         ->Enabled=false;
        btRunStop       ->Enabled=false;
        btSetThermoType ->Enabled=false;
//        btResetCOM      ->Enabled=(Function==4);
        btSend          ->Enabled=(Function==3);
    }
}
//------------------------------------------------------------------------------
bool TfOmron::RS232_OpenDevice()                                                //開啟Rs-232通訊埠
{
    bool bFlag=true;

    CommOmron->CommName="\\\\.\\"+HSys.sTempOmronComPort;                       //JerryYang 20201223 Omron通訊由PComm改為使用TComm元件
    CommOmron->Parity=Even;
    CommOmron->BaudRate=38400;
    CommOmron->ByteSize=_7;
    CommOmron->ParityCheck=false;
    CommOmron->StopBits=_2;
    try
    {
        CommOmron->StartComm();
    }
    catch(...)
    {
        bFlag=false;
        MyDBIProcess("Exception", "TCOM2::OmronInit");
    }

    if(bFlag)
        return true;
    else
        return false;
}
//------------------------------------------------------------------------------
void TfOmron::RS232_CloseDevice()                                               //關閉Rs-232通訊埠
{
    CommOmron->StopComm();                                                      //JerryYang 20201223 Omron通訊由PComm改為使用TComm元件
}
//------------------------------------------------------------------------------
#define     STX 0x02
#define     ETX 0x03
void TfOmron::SendCommand(AnsiString Command, AnsiString Func)                  //Rs232送碼位置
{
    int BCC=0;

    if(Command=="")
        return;

    BCC=SetBCC(Command);
    SendBufferASCII.sprintf("%c%s%c%c\r\n", STX, Command, ETX, BCC);
    SendBufferTEXT.sprintf("[STX]%s[ETX](0x%02X)", Command, BCC);

    CommOmron->WriteCommData(SendBufferASCII.c_str(), SendBufferASCII.Length());
    WriteInfoToMemo("["+Func+"] SEND :"+SendBufferTEXT);
}
//------------------------------------------------------------------------------
void TfOmron::WriteInfoToMemo(AnsiString asInfo)                                //工作訊息寫入memo
{
    AnsiString FileName;
    TDateTime myDateTime=Now();
    if(meLogs->Lines->Count>2000)
    {
        if(cbGetRecord->Checked==true)                                          //Steven 20140116 : Omron 加入記錄到檔案的功能 //Steven 20220309 : 解mark
        {
            FileName.sprintf("%s\\%04d%02d\\", asEJ1NLogPath, SystemYear, SystemMonth);
            MyForceDirectories(FileName, "fOmron::WriteInfoToMemo");
            FileName.sprintf("%s\\%04d%02d\\%04d-%02d-%02d %02d_%02d_%02d_CommLog.csv", asEJ1NLogPath, SystemYear, SystemMonth, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
            meLogs->Lines->SaveToFile(FileName);
        }
        meLogs->Clear();
    }

    meLogs->Lines->Add(myDateTime.TimeString()+" "+asInfo);
}
//------------------------------------------------------------------------------
bool __fastcall TfOmron::CheckEndAndResponseCode(AnsiString Buffer)             //True: Error, False: Normal
{
    AnsiString EndCode=Buffer.SubString(10, 2);
    AnsiString ResponseCode;
    int iStart;
    bool bMatch=false;
    bool Ret=true;

    if(EndCode.Pos(sEndCode[0][0])==1)                                          //End Code無錯誤
    {
        WriteInfoToMemo("MESS: End code satus: "+sEndCode[1][0]);
        Ret=false;
    }
    else
    {
        for(int i=1; i<ecTotal; i++)
        {
            iStart=EndCode.Pos(sEndCode[0][i]);
            if(iStart!=0)
            {
                bMatch=true;
                WriteInfoToMemo("MESS: End code status: "+sEndCode[1][i]);
            }
        }

        if(bMatch==false)
        {
            WriteInfoToMemo("MESS: Unknown end code : "+EndCode);
        }
    }

    if(Buffer.Length()>20)
    {
        ResponseCode=Buffer.SubString(16, 4);

        if(ResponseCode.Pos(sResponseCode[0][0])==1)                            //Response Code無錯誤
        {
            WriteInfoToMemo("MESS: Response status: "+sResponseCode[1][0]);
            return false;
        }
        for(int i=1; i<rcTotalCount; i++)
        {
            iStart=ResponseCode.Pos(sResponseCode[0][i]);
            if(iStart!=0)
            {
                WriteInfoToMemo("MESS: Response status: "+sResponseCode[1][i]);
                return true;
            }
        }

        WriteInfoToMemo("MESS: Unknown response code : "+ResponseCode);
        return true;
    }
    else
    {
        return Ret;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfOmron::cbMrcSrcChange(TObject *Sender)
{
    pcMrcSrc->Visible=false;
    tsReadVariable->TabVisible=false;
    tbWriteVariable->TabVisible=false;
    tsCompositeRead->TabVisible=false;
    tsCompositeWrite->TabVisible=false;
    tsCompositeReadReg->TabVisible=false;
    tsEchoTest->TabVisible=false;
    tsOperationCommand->TabVisible=false;

    switch(atoi(cbMrcSrc->Text.SubString(1, 4).c_str()))
    {
        case 101:                                                               //讀取設定值
            pcMrcSrc->Visible=true;
            tsReadVariable->TabVisible=true;
            break;
        case 102:                                                               //寫入設定值
            pcMrcSrc->Visible=true;
            tbWriteVariable->TabVisible=true;
            break;
        case 104:                                                               //讀取多個非連續設定值
            pcMrcSrc->Visible=true;
            tsCompositeRead->TabVisible=true;
            break;
        case 113:                                                               //寫入多個非連續設定值
            pcMrcSrc->Visible=true;
            tsCompositeWrite->TabVisible=true;
            break;
        case 110:                                                               //複合登記讀取
            break;
        case 111:                                                               //複合讀取登記
            pcMrcSrc->Visible=true;
            tsCompositeReadReg->TabVisible=true;
            break;
        case 112:                                                               //複合讀取登記確認
            break;
        case 801:
            pcMrcSrc->Visible=true;
            tsEchoTest->TabVisible=true;
            break;
        case 3005:
            pcMrcSrc->Visible=true;
            tsOperationCommand->TabVisible=true;
            break;
    }
}
//------------------------------------------------------------------------------
void TfOmron::GetPVValue(int Chanel, AnsiString Command)
{
    AnsiString Str="";
    double temp;

    for(int i=0; i<bModulType[Chanel]; i++)
    {
        if(i<bModulType[Chanel])
        {
            Str=Command.SubString(8, 3);                                        //讀取PV值
            Command=Command.SubString(11, Command.Length()-10);
            temp=HexStrToInt(Str.c_str());
            if(bInputError[Chanel][i])
                dTempValue[Chanel][i]=999.0;
            else
                dTempValue[Chanel][i]=temp/10.0;
        }
        else                                                                    //沒有用到的地方要重置
        {
            dTempValue[Chanel][i]    =0.0;
        }
    }
}
//------------------------------------------------------------------------------
void TfOmron::Get4in1SVValue(int Chanel, AnsiString Command)
{
    AnsiString Str="";
    double temp;

    for(int i=0; i<bModulType[Chanel]; i++)
    {
        if(i<bModulType[Chanel])
        {
            Str=Command.SubString(3, 8);                                        //讀取裝置狀態
            Command=Command.SubString(11, Command.Length()-10);
            Str=CountHexToByte(Str);
            bATStart[Chanel][i]      =(Str.SubString(9,  1)=="1")?true:false;   //AT ON / OFF
            bRunStopStatus[Chanel][i]=(Str.SubString(8,  1)=="0")?true:false;   //RUN=0 / STOP=1
            bEvent[Chanel][i][0]     =(Str.SubString(18, 1)=="1")?true:false;   //警報輸出
            bEvent[Chanel][i][1]     =(Str.SubString(19, 1)=="1")?true:false;   //警報輸出
            bEvent[Chanel][i][2]     =(Str.SubString(20, 1)=="1")?true:false;   //警報輸出
            bInputError[Chanel][i]   =(Str.SubString(26, 1)=="1")?true:false;   //Input Error

            Str=Command.SubString(8, 3);                                        //讀取SV值
            Command=Command.SubString(11, Command.Length()-10);
            temp=HexStrToInt(Str.c_str());
            dSetValue[Chanel][i]=temp/10.0;
        }
        else                                                                    //沒有用到的地方要重置
        {
            bATStart[Chanel][i]      =false;
            bRunStopStatus[Chanel][i]=false;
            bEvent[Chanel][i][0]     =false;
            bEvent[Chanel][i][1]     =false;
            bEvent[Chanel][i][2]     =false;
            bInputError[Chanel][i]   =false;
            dSetValue[Chanel][i]     =0.0;
        }
    }
}
//------------------------------------------------------------------------------
enum eOmronTask
{
    eotWaitCommand=1,
    eotGetComponent,
    eotSend1Cmd,
    eotInitReadPV,
    eotReadPV,
    eotReadSV,
    eotSetSV,
    eotAT,
    eotATOneByOne,
    eotRT,
    eotRTOneByOne,
    eotSetThermoTypeInit,
    eotSetThermoTypeStop,
    eotSetThermoTypeSet,
    eotSetThermoTypeRun,
    eotResetCom,
    eotRunStop
};
//------------------------------------------------------------------------------
void TfOmron::Timer1Timer()
{
    #ifdef SOFT_SIMULTE
        return;
    #endif

    static bool bTimerIn=false;
    static int iCount=0;
    static int iRunStop=0;
    int &Task=iOmronTask;
    if(InitialOK==false)
        return;

    if(Temperature.bATCActiveCooling ||                                         //Steven 20210805 : 使用ATC的時候, 不要跑Omron EJ1N
       ATC_SYSTEM==eATC60 ||
       (ATC_SYSTEM==eNewATCSystem &&
        ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_60))                        //Steven 20201120 : 修正ATC6.0不需要跑Omron EJ1N
    {
        return;
    }

    if(TC401HeaterControl!=NoHeater &&                                          //Steven 20171227 (Wei) : Add for HT-9045L
       (USE_16_HEATER==eht16HeaterEJ1N ||                                       //Steven 20120220 : Omron EJ1N溫控器
        USE_16_HEATER==eht32HeaterEJ1N))                                        //Steven 20140923 : Index使用EJ1N版32組加熱器
    {

    }
    else
    {
        return;
    }

    if(bTimerIn)                                                                //Steven 20250424 : add protection for Omren EJ1N
        return;
    else
        bTimerIn=true;

    switch(iOmronTask)
    {
        case eotWaitCommand:
            Task=eotInitReadPV;
            if(bResetCom)                                                       //重置RS232, 一定要在最上面
            {
                iResetComTask=1;
                Task=eotResetCom;
            }
            else if(bGetComponent)                                              //讀取目前裝置內容,一定擺第二個位置
            {
                iGetComponentTask=1;
                Task=eotGetComponent;
            }
            else if(iSetSV!=0 &&                                                //設定目標溫度
                    bAT==false)                                                 //Steven 20141002 : 做AT時不能設定溫度
            {
                iSetSVTask=1;
                Task=eotSetSV;
            }
            else if(bRunStop)                                                   //加熱棒電流輸出
            {
                iRunStopTask=1;
                Task=eotRunStop;
            }
            else if(bSendCommand)                                               //手動送資料
            {
                iSend1CmdTask=1;
                Task=eotSend1Cmd;
            }
            else if(bSetThermoType)                                             //設定感溫線種類
            {
                iSetThermoTypeTask=true;
                Task=eotSetThermoTypeInit;
            }
            else if(bAT)                                                        //自動調整控溫曲線
            {
                iATTask=1;
                Task=eotAT;
            }
            else if(bATOneByOne)
            {
                iATOneByOneTask=1;
                Task=eotATOneByOne;
            }
            else if(bRT)
            {
                iRTTask=1;
                Task=eotRT;
            }
            else if(bRTOneByOne)
            {
                iRTOneByOneTask=1;
                Task=eotRTOneByOne;
            }
            break;
        case eotSend1Cmd:
            if(DoSend1Cmd())
            {
                Task=eotWaitCommand;
                bSendCommand=false;
            }
            else if(bResetCom)
            {
                iResetComTask=1;
                Task=eotResetCom;
            }
            break;
        case eotGetComponent:
            if(DoGetComponent())
            {
                SetButton(1);
                bGetComponent=false;
                Task=eotWaitCommand;
            }
            break;
        case eotSetThermoTypeInit:
            iRunStop=rgRunStop->ItemIndex;
            rgRunStop->ItemIndex=0;
            iRunStopTask=1;
            Task=eotSetThermoTypeStop;
        case eotSetThermoTypeStop:
            if(DoRunStop())
            {
                Task=eotSetThermoTypeSet;
            }
            break;
        case eotSetThermoTypeSet:
            if(DoSetThermoType())
            {
                Task=eotSetThermoTypeRun;
                rgRunStop->ItemIndex=iRunStop;
                iRunStopTask=1;
            }
            break;
        case eotSetThermoTypeRun:
            if(DoRunStop())
            {
                SetButton(1);
                bSetThermoType=false;
                Task=eotInitReadPV;
            }
            break;
        case eotSetSV:
            if(DoSetSV())
            {
                SetButton(1);
                if(iSetSV==2)                                                   //Steven 20250424 : add protection for Omren EJ1N
                {
                    iSetSV=1;
                }
                else
                {
                    iSetSV=0;
                }
                Task=eotInitReadPV;
            }
            break;
        case eotResetCom:
            if(DoResetCom())
            {
                SetButton(0);
                bResetCom=false;
                bGetComponent=true;
                Task=eotWaitCommand;
            }
            break;
        case eotRunStop:
            if(DoRunStop())
            {
                SetButton(1);
                bRunStop=false;
                Task=eotInitReadPV;
            }
            break;
        case eotAT:
            if(DoAT())
            {
                SetButton(1);
                bAT=false;
                Task=eotInitReadPV;
            }
            break;
        case eotATOneByOne:
            if(DoATOneByOne())
            {
                SetButton(1);
                bATOneByOne=false;
                Task=eotInitReadPV;
            }
            break;
        case eotRT:
            if(DoRT())
            {
                SetButton(1);
                bRT=false;
                Task=eotInitReadPV;
            }
            break;
        case eotRTOneByOne:
            if(DoRTOneByOne())
            {
                SetButton(1);
                bRTOneByOne=false;
                Task=eotInitReadPV;
            }
            break;
        case eotInitReadPV:
            bReadPV=true;
            bReadSV=false;
            bRecvData=false;
            iGetPVTask=1;
            Task=eotReadPV;
        case eotReadPV:
            if(DoGetPV())                                                       //取得目前感溫溫度
            {
                Task=eotReadSV;
                bReadPV=false;
                bReadSV=true;
                bRecvData=false;
                iGetPVTask=1;
                iGetSVTask=1;
                iCount=0;
            }
            else
            {
                iCount++;
                if(iCount>500)                                                  //JerryYang 20201123 100->500
                {
                    bReadError=true;
                    bRecvData=false;
                    WriteInfoToMemo("ERRO: Communication Error!." +AnsiString(__FUNC__));
                    Task=eotWaitCommand;
                    iCount=0;
                }
                break;
            }
        case eotReadSV:
            if(DoGetSV())                                                       //取得溫控器狀態
            {
                Task=eotWaitCommand;
                bReadPV=false;
                bReadSV=false;
                bRecvData=false;
                iGetSVTask=1;
                iCount=0;
            }
            else
            {
                iCount++;
                if(iCount>500)                                                  //JerryYang 20201123 100->500
                {
                    bReadError=true;
                    bRecvData=false;
                    WriteInfoToMemo("ERRO: Communication Error!." +AnsiString(__FUNC__));
                    Task=eotWaitCommand;
                    iCount=0;
                }
            }
            break;
    }

    bTimerIn=false;
}
//------------------------------------------------------------------------------
void __fastcall TfOmron::btSendClick(TObject *Sender)
{
    bSendCommand=!bSendCommand;
    if(bSendCommand)
        SetButton(3);
    else
        SetButton(1);
}
//------------------------------------------------------------------------------
TQPF_Timer Send1CmdDelay;
bool TfOmron::DoSend1Cmd()
{
    AnsiString str;
    static int Addr=1, CH=0;
    int &Task=iSend1CmdTask;
    AnsiString Command="";
    switch(Task)
    {
        case 1:
            str.sprintf("%02d", Addr);
            cbUnitAddress->Text=str;
            str.sprintf("0%d00", CH);
            Edit8->Text=str;
            Command.sprintf("%s%s%s",
                            cbUnitAddress->Text,                                //單元編號
                            Label3->Caption,                                    //子地址與SID, 固定不變
                            cbMrcSrc->Text.SubString(1, 4)                      //MRC+SRC
                            );

            if(tsReadVariable->TabVisible==true)
            {
                Command+=ComboBox5->Text+Edit7->Text;
                Command+=Edit8->Text;
                Command+=Label11->Caption;
                Command+=Edit9->Text;
            }
            else if(tbWriteVariable->TabVisible==true)
            {
                Command+=ComboBox3->Text+Edit6->Text;
                Command+=Edit10->Text;
                Command+=Label40->Caption;
                Command+=Edit11->Text;
            }
            else if(tsCompositeRead->TabVisible==true)
            {
                Command+=ComboBox6->Text+Edit15->Text+Edit4->Text+Label27->Caption;
                Command+=ComboBox7->Text+Edit16->Text+Edit12->Text+Label27->Caption;
                Command+=ComboBox8->Text+Edit17->Text+Edit13->Text+Label27->Caption;
                Command+=ComboBox9->Text+Edit18->Text+Edit14->Text+Label27->Caption;
            }
            else if(tsCompositeWrite->TabVisible==true)
            {
                Command+=ComboBox1->Text +Edit22->Text+Edit3->Text +Label15->Caption+Edit26->Text;
                Command+=ComboBox2->Text +Edit23->Text+Edit19->Text+Label16->Caption+Edit27->Text;
                Command+=ComboBox10->Text+Edit24->Text+Edit20->Text+Label29->Caption+Edit28->Text;
                Command+=ComboBox11->Text+Edit25->Text+Edit21->Text+Label37->Caption+Edit29->Text;
            }
            else if(tsCompositeReadReg->TabVisible==true)
            {
                Command+=Edit5->Text;
            }
            else if(tsEchoTest->TabVisible==true)
            {
                Command+=Edit1->Text;
            }
            else if(tsOperationCommand->TabVisible==true)
            {
                Command+=ComboBox4->Text.SubString(1, 2)+Edit2->Text;
            }
            Task=200;
            bRecvData=false;
            SendCommand(Command, __FUNC__);
            Send1CmdDelay.SetSecAndOn(iDelaySecc);
            break;
        case 200:
            if(bRecvData)
            {
                LedEJ1N[Addr]->Value=true;                                      //Steven 20210305 : EJ1N加入狀態顯示
                CH++;
                if(CH>=5)
                {
                    CH=1;
                    Addr++;
                    if(Addr>=5)
                        Addr=1;
                    return true;
                }
                Task=1;
                return !bSendCommand;
            }
            else if(Send1CmdDelay.Off())
            {
                LedEJ1N[Addr]->FalseColor=clRed;                                //Steven 20210305 : EJ1N加入狀態顯示
                LedEJ1N[Addr]->Value=false;
                WriteInfoToMemo("MESS: Chanel "+AnsiString(Addr)+" Recv data fail!.  "+AnsiString(__FUNC__));
                return true;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
void __fastcall TfOmron::btGetModulerClick(TObject *Sender)
{
    SetButton(2);
    bGetComponent=true;
}
//------------------------------------------------------------------------------
TQPF_Timer GetComponentDelay;
bool TfOmron::DoGetComponent()
{
    static int Addr=0;
    AnsiString Command="";
    int &Task=iGetComponentTask;

    switch(Task)
    {
        case 1:
            Addr=0;
            Task=100;
            bGetComponentFinish=false;
        case 100:
            bRecvData=false;
            LedEJ1N[Addr]->FalseColor=clSilver;                                 //Steven 20210305 : EJ1N加入狀態顯示
            LedEJ1N[Addr]->Value=false;
            Command.sprintf("%02X0000503", Addr+1);
            SendCommand(Command, __FUNC__);
            GetComponentDelay.SetSecAndOn(iDelaySecc);
            Task=200;
            break;
        case 200:
            if(bRecvData)
            {
                LedEJ1N[Addr]->Value=true;                                      //Steven 20210305 : EJ1N加入狀態顯示
                Addr++;
                if(Addr<EJ1N_Count)                                             //Steven 20200318 : TOTAL_OMRON_UNIT --> EJ1N_Count
                {
                    Task=100;
                }
                else
                {
                    Task=300;
                }
            }
            else if(GetComponentDelay.Off())
            {
                LedEJ1N[Addr]->FalseColor=clRed;                                //Steven 20210305 : EJ1N加入狀態顯示
                LedEJ1N[Addr]->Value=false;
                if(bResetCom==false)                                            //JerryYang 20200324 EJ1N Get Component Error新增記錄
                {
                    NewRecordProcess("", "Reset Index heater", "GetComponentErr");
                    bResetCom=true;
                    for(int i=0; i<TOTAL_OMRON_UNIT; i++)
                    {
                        for(int j=0; j<4; j++)
                        {
                            dTempValue[i][j]=999;
                        }
                    }
                }
                bModulType[Addr]=0;
                Addr++;
                if(Addr<EJ1N_Count)
                {
                    Task=100;
                }
                else
                {
                    Task=300;
                }
                WriteInfoToMemo("MESS: Chanel "+AnsiString(Addr)+" Recv data fail!." +AnsiString(__FUNC__));
            }
            break;
        case 300:
            SetPanelPos(true);
            bGetComponent=false;
            bGetComponentFinish=true;
            return true;
    }
    return false;
}
//------------------------------------------------------------------------------
TQPF_Timer GetPVDelay;
bool TfOmron::DoGetPV()
{
    static int Addr=0;
    static AnsiString Command="";
    static int iRetryCnt=0;
    int &Task=iGetPVTask;

    switch(Task)
    {
        case 1:
            Addr=0;
            iRetryCnt=0;
            Task=100;
        case 100:
            if(bModulType[Addr]==0)
            {
                Addr++;
                if(Addr<EJ1N_Count)
                {
                    Task=100;
                    break;
                }
                else
                {
                    return true;
                }
            }
            else
            {
                if(bModulType[Addr]==4)
                {
                    Command.sprintf("%02X0000104C0000000C0010000C0020000C0030000", Addr+1);
//                    Command.sprintf("%02X0000104C0000000C0000100C0000200C0010000C0010100C0010200C0020000C0020100C0020200C0030000C0030100C0030200", Addr+1);
//                    Command.sprintf("%02X0000104C0000000C0010000C0020000C0030000C0000100C0010100C0020100C0030100C0000200C0010200C0020200C0030200", Addr+1);
                    // %02X000 : 單元編號+SID
                    // 0104    : 讀取多個非連續參數
                    // C0      : 類型
                    // 0000    : 0000~0300->PV值
                    // 0001    : 0001~0301->狀態
                    // 0002    : 0002~0302->SP值
                    // 00      : 分隔符號
                }
                else if(bModulType[Addr]==2)
                {
                    Command.sprintf("%02X0000104C0000100C0000200C0010100C0010200", Addr+1);
                }
                bReceivePV[Addr]=false;                                         //JerryYang 20201123 Omron通訊修正為一問一答
                bRecvData=false;
                SendCommand(Command, __FUNC__);
                GetPVDelay.SetSecAndOn(10);                                     //kevin 20210518  4->10
                Task=400;
            }
//            break;
//        case 150:
//            if(GetPVDelay.Off())
//            {
//                GetPVDelay.SetSecAndOn(iDelaySecc);
//                Task=400;
//            }
//            break;
        case 400:
            if(bRecvData && bReceivePV[Addr])                                   //JerryYang 20201123 Omron通訊修正為一問一答
            {
                LedEJ1N[Addr]->Value=true;                                      //Steven 20210305 : EJ1N加入狀態顯示
                Addr++;
                iRetryCnt=0;
//                bRecvData=false;
                if(Addr<EJ1N_Count)
                {
                    Task=100;
                }
                else
                {
                    return true;
                }
            }
            else if(GetPVDelay.Off())
            {
                LedEJ1N[Addr]->FalseColor=clRed;                                //Steven 20210305 : EJ1N加入狀態顯示
                LedEJ1N[Addr]->Value=false;
                if(iRetryCnt>=2)
                {
                    iRetryCnt=0;
                    if(bResetCom==false)
                    {
                        bResetCom=true;
                        for(int i=0; i<TOTAL_OMRON_UNIT; i++)
                        {
                            for(int j=0; j<4; j++)
                            {
                                dTempValue[i][j]=999;
                            }
                        }
                        NewRecordProcess("", "DoGetPV() case 400: bResetCom", "");       //kevin 20210518 add log omoron error
                    }
                    WriteInfoToMemo("MESS: Chanel "+AnsiString(Addr)+" Recv data fail!." +AnsiString(__FUNC__));
                    return true;
                }
                else
                {
                    WriteInfoToMemo("MESS: Chanel "+AnsiString(Addr)+" Recv data fail!.RetryAgain" +AnsiString(__FUNC__));
                    iRetryCnt++;
                    Task=100;
                    break;
                }
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
TQPF_Timer GetSVDelay;
bool TfOmron::DoGetSV()
{
    static int Addr=0;
    static AnsiString Command="";
    static int iRetryCnt=0;
    int &Task=iGetSVTask;

    switch(Task)
    {
        case 1:
            Addr=0;
            iRetryCnt=0;
            Task=100;
        case 100:
            if(bModulType[Addr]==0)
            {
                Addr++;
                if(Addr<EJ1N_Count)
                {
                    Task=100;
                    break;
                }
                else
                {
                    return true;
                }
            }
            else
            {
                if(bModulType[Addr]==4)
                {
                    Command.sprintf("%02X0000104C0000100C0000200C0010100C0010200C0020100C0020200C0030100C0030200", Addr+1);
                    // %02X000 : 單元編號+SID
                    // 0104    : 讀取多個非連續參數
                    // C0      : 類型
                    // 0000    : 0000~0300->PV值
                    // 0001    : 0001~0301->狀態
                    // 0002    : 0002~0302->SP值
                    // 00      : 分隔符號
                }
                else if(bModulType[Addr]==2)
                {
                    Command.sprintf("%02X0000104C0000100C0000200C0010100C0010200", Addr+1);
//                    Command.sprintf("%02X0000104C0000000C0010000C0000100C0010100C0000200C0010200", Addr+1);
                }
                bRecvData=false;
                bReceiveSV[Addr]=false;                                         //JerryYang 20201123 Omron通訊修正為一問一答
                SendCommand(Command, __FUNC__);
                GetSVDelay.SetSecAndOn(4);
                Task=400;
            }
//            break;
//        case 150:
//            if(GetSVDelay.Off())
//            {
//                GetSVDelay.SetSecAndOn(iDelaySecc);
//                GetSVDelay.On();
//                Task=400;
//            }
//            break;
        case 400:
            if(bRecvData && bReceiveSV[Addr])                                   //JerryYang 20201123 Omron通訊修正為一問一答
            {
                LedEJ1N[Addr]->Value=true;                                      //Steven 20210305 : EJ1N加入狀態顯示
                iRetryCnt=0;
                Addr++;
//                bRecvData=false;
                if(Addr<EJ1N_Count)
                {
                    Task=100;
                }
                else
                {
                    return true;
                }
            }
            else if(GetSVDelay.Off())
            {
                LedEJ1N[Addr]->FalseColor=clRed;
                LedEJ1N[Addr]->Value=false;                                     //Steven 20210305 : EJ1N加入狀態顯示
                if(iRetryCnt>=2)
                {
                    iRetryCnt=0;
                    if(bResetCom==false)
                    {
                        bResetCom=true;
                        for(int i=0; i<TOTAL_OMRON_UNIT; i++)
                        {
                            for(int j=0; j<4; j++)
                            {
                                dTempValue[i][j]=999;
                            }
                        }
                    }
                    WriteInfoToMemo("MESS: Chanel "+AnsiString(Addr)+" Recv data fail!." +AnsiString(__FUNC__));
                    return true;
                }
                else
                {
                    WriteInfoToMemo("MESS: Chanel "+AnsiString(Addr)+" Recv data fail!.RetryAgain" +AnsiString(__FUNC__));
                    iRetryCnt++;
                    Task=100;
                    break;
                }
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
void __fastcall TfOmron::btSetThermoTypeClick(TObject *Sender)
{
    SetButton(2);
    bSetThermoType=true;
}
//------------------------------------------------------------------------------
TQPF_Timer SetThermoTypeDelay;
bool TfOmron::DoSetThermoType()
{
    static int Addr=0;
    AnsiString Command="", SubCmd="";
    int &Task=iSetThermoTypeTask;

    switch(Task)
    {
        case 1:
            Addr=0;
            Task=100;
        case 100:
            if(bModulType[Addr]==0)
            {
                Addr++;
                if(Addr<EJ1N_Count)
                {
                    Task=100;
                }
                else
                {
                    return true;
                }
            }
            else
            {
                Command.sprintf("%02X0000113", Addr+1);

                for(int i=0; i<bModulType[Addr]; i++)
                {
                    SubCmd.sprintf("E0%02d00000000000%d", i+1, (rgSensorType->ItemIndex==0)?1:6);  //Frank 20141006 : SensorType 5 --> 6
                    Command+=SubCmd;
                }
                bRecvData=false;
                SendCommand(Command, __FUNC__);
                SetThermoTypeDelay.SetSecAndOn(iDelaySecc);
                Task=200;
            }
            break;
        case 200:
            if(bRecvData)
            {
                Addr++;
                if(Addr<EJ1N_Count)
                {
                    Task=100;
                }
                else
                {
                    return true;
                }
            }
            else if(SetThermoTypeDelay.Off())
            {
                Addr++;
                if(Addr<EJ1N_Count)
                {
                    Task=100;
                }
                else
                {
                    return true;
                }
                WriteInfoToMemo("MESS: Chanel "+AnsiString(Addr)+" Recv data fail!." +AnsiString(__FUNC__));
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
void __fastcall TfOmron::btSetSVClick(TObject *Sender)
{
    SetButton(2);
    if(iSetSV==0)                                                               //Steven 20250424 : add protection for Omren EJ1N
        iSetSV=1;
    else
        iSetSV=2;
}
//------------------------------------------------------------------------------
TQPF_Timer SetSVDelay;
bool TfOmron::DoSetSV()
{
    static int Addr=0, iErrorCount;
    AnsiString Command="", SubCmd="", Str, Str2;
    int &Task=iSetSVTask;

    switch(Task)
    {
        case 1:
            Addr=0;
            iErrorCount=0;
            Task=100;
        case 100:
            if(bModulType[Addr]==0)
            {
                Addr++;
                if(Addr<EJ1N_Count)
                {
                    Task=100;
                }
                else
                {
                    return true;
                }
            }
            else
            {
                Command.sprintf("%02X0000113", Addr+1);

                for(int i=0; i<bModulType[Addr]; i++)
                {
                    Str=myPal[Addr][i]->edSV->Text;
                    Str2=atof(Str.c_str())*10;                                  //把數字乘以10
                    SubCmd.sprintf("D4%02d00000000%04X", i+1, atoi(Str2.c_str()));
                    Command+=SubCmd;
                }
                bRecvData=false;
                SendCommand(Command, __FUNC__);
                SetSVDelay.SetSecAndOn(iDelaySecc);
                Task=200;
            }
            break;
        case 200:
            if(bRecvData)
            {
                LedEJ1N[Addr]->Value=true;                                      //Steven 20210305 : EJ1N加入狀態顯示
                Addr++;
                iErrorCount=0;
                if(Addr<EJ1N_Count)
                {
                    Task=100;
                }
                else
                {
                    return true;
                }
            }
            else if(SetSVDelay.Off())
            {
                LedEJ1N[Addr]->FalseColor=clRed;
                LedEJ1N[Addr]->Value=false;                                     //Steven 20210305 : EJ1N加入狀態顯示
//                Addr++;
                iErrorCount++;
//                if(Addr<EJ1N_Count)                                           //Steven 20250314 : EJ1N 設定SV失敗要重置COM Port
                if(iErrorCount<5)
                {
                    Task=100;
                }
                else
                {
                    NewRecordProcess("Exception", "EJ1N DoSetSV Fail, reset COM.");
                    bResetCom=true;
                    return true;
                }
                WriteInfoToMemo("MESS: Chanel "+AnsiString(Addr)+" Recv data fail!." +AnsiString(__FUNC__));
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
void __fastcall TfOmron::btATClick(TObject *Sender)
{
    SetButton(2);
    bAT=true;
    bSetAT=true;
}
//------------------------------------------------------------------------------
TQPF_Timer ATDelay;
bool TfOmron::DoAT()
{
    static int Addr=0;
    AnsiString Command="";
    int &Task=iATTask;

    switch(Task)
    {
        case 1:
            Addr=0;
            Task=100;
        case 100:
            if(bModulType[Addr]==0)
            {
                Addr++;
                if(Addr<EJ1N_Count)
                {
                    Task=100;
                }
                else
                {
                    return true;
                }
            }
            else
            {
                if(bSetAT)
                {
                    if(cb40PercentAT->Checked==true)
                        Command.sprintf("%02X00030050EFF", Addr+1);
                    else
                        Command.sprintf("%02X00030050FFF", Addr+1);
                }
                else
                {
                    Command.sprintf("%02X000300510FF", Addr+1);                 //取消AT
                }

                bRecvData=false;
                SendCommand(Command, __FUNC__);
                ATDelay.SetSecAndOn(iDelaySecc);
                Task=200;
            }
            break;
        case 200:
            if(bRecvData)
            {
                LedEJ1N[Addr]->Value=true;                                      //Steven 20210305 : EJ1N加入狀態顯示
                Addr++;
                if(Addr<EJ1N_Count)
                {
                    Task=100;
                }
                else
                {
                    return true;
                }
            }
            else if(ATDelay.Off())
            {
                LedEJ1N[Addr]->FalseColor=clRed;
                LedEJ1N[Addr]->Value=false;                                     //Steven 20210305 : EJ1N加入狀態顯示
                Addr++;
                if(Addr<EJ1N_Count)
                {
                    Task=100;
                }
                else
                {
                    return true;
                }
                WriteInfoToMemo("MESS: Chanel "+AnsiString(Addr)+" Recv data fail!." +AnsiString(__FUNC__));
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
TQPF_Timer ATOneByOneDelay;
bool TfOmron::DoATOneByOne()
{
    static int Chanel=0;
    static int Addr=0;
    AnsiString Command="";
    int &Task=iATOneByOneTask;

    switch(Task)
    {
        case 1:
            Chanel=0;
            Addr=0;
            Task=100;
        case 100:
            if(bModulType[Chanel]==0)
            {
                Chanel++;
                Addr=0;
                if(Chanel<EJ1N_Count)
                {
                    Task=100;
                }
                else
                {
                    return true;
                }
            }
            else
            {
                if(myPal[Chanel][Addr]->cbEnableAT->Checked==true)
                {
                    if(cb40PercentAT->Checked==true)
                        Command.sprintf("%02X00030050E%02d", Chanel+1, Addr);
                    else
                        Command.sprintf("%02X00030050F%02d", Chanel+1, Addr);

                    bRecvData=false;
                    SendCommand(Command, __FUNC__);
                    ATDelay.SetSecAndOn(iDelaySecc);
                }
                else
                {
                    ATDelay.SetSecAndOn(iDelaySecc);
                    bRecvData=true;
                }
                Task=200;
            }
            break;
        case 200:
            if(bRecvData)
            {
                LedEJ1N[Addr]->Value=true;                                      //Steven 20210305 : EJ1N加入狀態顯示
                if(Addr<bModulType[Chanel]-1)
                {
                    Addr++;
                }
                else
                {
                    Chanel++;
                    Addr=0;
                }
                if(Chanel<EJ1N_Count)
                {
                    Task=100;
                }
                else
                {
                    return true;
                }
            }
            else if(ATOneByOneDelay.Off())
            {
                LedEJ1N[Addr]->FalseColor=clRed;
                LedEJ1N[Addr]->Value=false;                                     //Steven 20210305 : EJ1N加入狀態顯示
                if(Addr<bModulType[Chanel]-1)
                {
                    Addr++;
                }
                else
                {
                    Chanel++;
                    Addr=0;
                }
                if(Chanel<EJ1N_Count)
                {
                    Task=100;
                }
                else
                {
                    return true;
                }
                WriteInfoToMemo("MESS: Chanel "+AnsiString(Chanel)+" - Addr "+AnsiString(Addr)+" Recv data fail!." +AnsiString(__FUNC__));
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
void __fastcall TfOmron::btRunStopClick(TObject *Sender)
{
    bRunStop=true;
    SetButton(2);
}
//------------------------------------------------------------------------------
TQPF_Timer RunStopDelay;
bool TfOmron::DoRunStop()
{
    static int Addr=0;
    AnsiString Command="";
    int &Task=iRunStopTask;

    switch(Task)
    {
        case 1:
            Addr=0;
            Task=100;
        case 100:
            if(bModulType[Addr]==0)
            {
                Addr++;
                if(Addr<EJ1N_Count)
                {
                    Task=100;
                }
                else
                {
                    return true;
                }
            }
            else
            {
                if(rgRunStop->ItemIndex==0)
                    Command.sprintf("%02X00030050BFF", Addr+1);
                else
                    Command.sprintf("%02X00030050AFF", Addr+1);
                bRecvData=false;
                SendCommand(Command, __FUNC__);
                RunStopDelay.SetSecAndOn(iDelaySecc);
                Task=200;
            }
            break;
        case 200:
            if(bRecvData)
            {
                Addr++;
                if(Addr<EJ1N_Count)
                {
                    Task=100;
                }
                else
                {
                    return true;
                }
            }
            else if(RunStopDelay.Off())
            {
                Addr++;
                if(Addr<EJ1N_Count)
                {
                    Task=100;
                }
                else
                {
                    return true;
                }
                WriteInfoToMemo("MESS: Chanel "+AnsiString(Addr)+" Recv data fail!." +AnsiString(__FUNC__));
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
int iMRCSRCPos=12;
AnsiString sMRCSRC0104="0104";
AnsiString sMRCSRC0503="0503";
TQPF_Timer Com13Delay;
//------------------------------------------------------------------------------
void TfOmron::Main232()                                                         //Rs232接收位置
{
    static int iHasErr=0;
    static bool bHasETX=false;
    static bool bHasBCC=false;
    static int iCommunCTErr=0;                                                  //JerryYang 20190807 10秒沒收到資料, reset COM port
    int iCheckBCC=0;
    AnsiString Str;
    if(InitialOK==false)
        return;

    if(asBCC!="")
    {
        Com13Delay.SetSecAndOn(10);
        if(asTStr.Pos("[STX]")==1)
        {
            Str=asTStr.SubString(6, asTStr.Pos("[ETX]")-6);
            iCheckBCC=SetBCC(Str);
            asBCCConfirm=MyDeCodeASCII(iCheckBCC);
            WriteInfoToMemo("RECV: "+asTStr);
            if(asBCCConfirm==asBCC)
            {
                WriteInfoToMemo("MESS: BCC Check OK ");
                if(CheckEndAndResponseCode(asTStr))
                {
                    iHasErr=1;
                }
                iChanel=atoi(asTStr.SubString(6, 2).c_str())-1;
            }
            else
            {
                iHasErr=4;
            }
        }
        else
        {
            iHasErr=5;
        }

        if(iHasErr || iChanel<0 || iChanel>=EJ1N_Count)
        {
            Label26->Caption=AnsiString(atoi(Label26->Caption.c_str())+1);
            bReadPV=false;
            bGetComponent=false;

            if(iHasErr==1)
            {
                WriteInfoToMemo("ERRO: End Code or Response Code Error ");
            }
            else if(iHasErr==2)
            {
                WriteInfoToMemo("ERRO: RStr='Err' Error ");
            }
            else if(iHasErr==3)
            {
                WriteInfoToMemo("ERRO: TStr Length > 200 Error ");
            }
            else if(iHasErr==4)
            {
                WriteInfoToMemo("ERRO: BCC Check Error ");
            }
            else if(iHasErr==5)
            {
                WriteInfoToMemo("ERRO: TStr [STX] position Error ");
            }
            else if(iHasErr==6)
            {
                WriteInfoToMemo("ERRO: BCC Length Error ");
            }
            else if(iHasErr==7)
            {
                WriteInfoToMemo("ERRO: TStr not empty with new [STX] Error ");
            }
            else
            {
                WriteInfoToMemo("ERRO: Chanel= "+AnsiString(iChanel)+" Error ");
            }

            iCommunCTErr++;
            if(iCommunCTErr>10)
            {
                if(bResetCom==false)
                {
                    bResetCom=true;
                    for(int i=0; i<TOTAL_OMRON_UNIT; i++)
                    {
                        for(int j=0; j<4; j++)
                        {
                            dTempValue[i][j]=999;
                        }
                    }
                    NewRecordProcess("", "TfOmron->Main232() iHasErr: bResetCom", "");       //kevin 20210518 add log omoron error
                }
                iCommunCTErr=0;
            }

            asTStr="";
            asBCC="";
            bHasETX=false;
            bRecvData=false;                                                    //不能true
            bHasBCC=false;
            iHasErr=0;
            return;
        }

        if(asTStr.AnsiPos(sMRCSRC0503)==iMRCSRCPos)
        {
            if(asTStr.Pos("TC4A")!=0)
            {
                bModulType[iChanel]=4;
            }
            else if(asTStr.Pos("TC2A")!=0)
            {
                bModulType[iChanel]=2;
            }
        }
        else if(bReadPV==true || bReadSV==true)
        {
            if(asTStr.AnsiPos(sMRCSRC0104)==iMRCSRCPos)
            {
                if(asTStr.Length()>70)                                          //bReadSV
                {
                    bReceiveSV[iChanel]=true;                                   //JerryYang 20201123 Omron通訊修正為一問一答
                    Get4in1SVValue(iChanel, asTStr.SubString(20, asTStr.Length()-7));
                }
                else                                                            //bReadPV
                {
                    bReceivePV[iChanel]=true;                                   //JerryYang 20201123 Omron通訊修正為一問一答
                    GetPVValue(iChanel, asTStr.SubString(20, asTStr.Length()-7));
                }
            }
        }
        iCommunCTErr=0;
        asTStr="";
        asBCC="";
        bHasETX=false;
        bRecvData=true;
        bHasBCC=false;
    }
    else
    {
        if(Com13Delay.Off())                                                    //JerryYang 20190807 10秒沒收到資料, reset COM port
        {
            iCommunCTErr++;
            if(iCommunCTErr>10)
            {
                iCommunCTErr=0;
                Com13Delay.SetSecAndOn(10);
                if(bResetCom==false)
                {
                    bResetCom=true;
                    for(int i=0; i<TOTAL_OMRON_UNIT; i++)
                    {
                        for(int j=0; j<4; j++)
                        {
                            dTempValue[i][j]=999;
                        }
                    }
                    NewRecordProcess("", "TfOmron->Main232() Com13Delay: bResetCom", "");       //kevin 20210518 add log omoron error
                }
            }
            asTStr="";
            asBCC="";
        }
        if(bResetCom==true)
        {
            Com13Delay.SetSecAndOn(10);
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfOmron::btResetCOMClick(TObject *Sender)
{
    SetButton(2);
    bResetCom=true;
    NewRecordProcess("", "btResetCOM button: bResetCom", "");                   //kevin 20210518 add log omoron error
}
//------------------------------------------------------------------------------
TQPF_Timer ResetComDelay;
bool TfOmron::DoResetCom()
{
    static bool bFirstIn=true;
    int &Task=iResetComTask;
    #ifdef SOFT_SIMULTE
        return true;
    #else
    switch(Task)
    {
        case 1:
            bReadError=false;
            if(bFirstIn==false)
            {
                My232Thread->Suspend();                                         //關閉Rs-232執行緒
                RS232_CloseDevice();                                            //關閉Rs-232通訊埠
            }
            ResetComDelay.SetSecAndOn(1);
            Task=2;
        case 2:
            if(ResetComDelay.Off())
            {
                bInitComm=RS232_OpenDevice();
                if(bFirstIn==false)
                {
                    SetButton(4);
                }

                if(bInitComm)
                {
                    if(bFirstIn)
                    {
                        My232Thread=new TRS232Thread(true);
                        My232Thread->Priority=tpNormal;
                        bFirstIn=false;
                    }
                    My232Thread->Resume();                                      //啟動Rs-232執行緒
                    return true;
                }
                else
                {
                    WriteInfoToMemo("MESS: Reset COM Error, Code: "+AnsiString(bInitComm));
                }
                Task=3;
            }
            break;
    }
    return false;
    #endif
}
//------------------------------------------------------------------------------
void __fastcall TfOmron::Button1Click(TObject *Sender)
{
    if(Button1->Caption=="Stop Control")
    {
        MyOmronThread->Suspend();
        Button1->Caption="Run Control";
    }
    else
    {
        MyOmronThread->Resume();
        Button1->Caption="Stop Control";
    }
}
//------------------------------------------------------------------------------
void __fastcall TfOmron::Timer2Timer(TObject *Sender)
{
    AnsiString Str, Record, FileName;

    if(bShow==false)
        return;

    Record.sprintf("%04d-%02d-%02d, %02d_%02d_%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);

    for(int i=0; i<EJ1N_Count; i++)
    {
        for(int j=0; j<4; j++)
        {
            myPal[i][j]->SetBG(clGray, clGray);                                 //設定背景
            myPal[i][j]->SetDegree("℃", clYellow);                             //顯示量測單位
            if(bRunStopStatus[i][j])                                            //顯示目前運作模式
                myPal[i][j]->SetRunStop("RUN", clYellow);
            else
                myPal[i][j]->SetRunStop("STOP", clBlack);

            Str.sprintf("%5.1f", dTempValue[i][j]);
            myPal[i][j]->SetPV(Str, clYellow);                                  //顯示量測值
            Record=Record+", "+Str;                                             //Steven 20140116 : Omron 加入記錄到檔案的功能

            myPal[i][j]->SetAT("AT", (bATStart[i][j])?clYellow:clBlack);                                            //顯示AT是否啟動
            myPal[i][j]->SetInputErr("Input Error", (bInputError[i][j])?clRed:clBlack);                             //感溫線異常
            myPal[i][j]->SetEvent("Event", (bEvent[i][j][0] || bEvent[i][j][1] || bEvent[i][j][2])?clRed:clBlack);  //警報事件
            myPal[i][j]->SetSP(AnsiString(dSetValue[i][j]), TColor(0x004080FF));                                    //目前設定值
        }
    }

    if(cbGetRecord->Checked==true)                                              //Steven 20140116 : Omron 加入記錄到檔案的功能
    {
        if(Memo1->Lines->Count>400 || bSaveData)
        {
            FileName.sprintf("%s\\%04d%02d\\", asEJ1NLogPath, SystemYear, SystemMonth);
            MyForceDirectories(FileName, "TfOmron::Timer2Timer");
            FileName.sprintf("%s\\%04d%02d\\%04d-%02d-%02d %02d_%02d_%02d.csv", asEJ1NLogPath, SystemYear, SystemMonth, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
            Memo1->Lines->SaveToFile(FileName);
            Memo1->Lines->Clear();

            Str="Date, Time";
            for(int i=0; i<EJ1N_Count; i++)
            {
                for(int j=0; j<4; j++)
                {
                    Str+=", "+myPal[i][j]->GroupBox->Caption;
                }
            }
            Memo1->Lines->Add(Str);
            bSaveData=false;
        }
        Memo1->Lines->Add(Record);
    }
}
//------------------------------------------------------------------------------
void __fastcall TfOmron::btATOffClick(TObject *Sender)
{
    SetButton(2);
    bAT=true;
    bSetAT=false;
}
//------------------------------------------------------------------------------
void __fastcall TfOmron::btAT1by1Click(TObject *Sender)
{
    SetButton(2);
    bATOneByOne=true;
    bSetAT=true;
}
//------------------------------------------------------------------------------
void __fastcall TfOmron::btRTClick(TObject *Sender)
{
    SetButton(2);
    bRT=true;
    bSetRT=true;
}
//------------------------------------------------------------------------------
void __fastcall TfOmron::btRT1by1Click(TObject *Sender)
{
    SetButton(2);
    bRTOneByOne=true;
    bSetRT=true;
}
//------------------------------------------------------------------------------
void __fastcall TfOmron::btRTOffClick(TObject *Sender)
{
    SetButton(2);
    bRT=true;
    bSetRT=false;
}
//------------------------------------------------------------------------------
bool TfOmron::DoRT()
{
    return true;
}
//------------------------------------------------------------------------------
bool TfOmron::DoRTOneByOne()
{
    return true;
}
//------------------------------------------------------------------------------
void __fastcall TfOmron::btSaveDataClick(TObject *Sender)
{
    bSaveData=true;
}
//------------------------------------------------------------------------------
void TfOmron::ClearOmronLog()                                                   //JerryYang 20201123 清除非當月份的Omron通訊log
{
    WIN32_FIND_DATA filedata;                                                   // Structure for file data
    AnsiString SDataPath, sFileName, sDate;
    HANDLE filehandle;                                                          // Handle for searching

    if(DirectoryExists(asEJ1NLogPath))
    {
        SDataPath=asEJ1NLogPath+"\\"+"*.*";
        filehandle=FindFirstFile((SDataPath+"*").c_str(), &filedata);           //Steven 20101118 Start : 不要顯示資料夾以外的檔案
        if(filehandle!=INVALID_HANDLE_VALUE)
        {
            do
            {
                if((filedata.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)!=0 ||    /* 不處理隱藏檔及 . 跟 .. */
                    strcmp(filedata.cFileName, ".")==0 ||
                    strcmp(filedata.cFileName, "..")==0)
                {
                    continue;
                }
                else if(filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)   //如果該檔案為資料夾
                {
                    sFileName=AnsiString(filedata.cFileName);
                    sDate.sprintf("%04d%02d", SystemYear, SystemMonth);
                    if(sFileName!=sDate)
                    {
                        sFileName=asEJ1NLogPath+"\\"+sFileName;
                        DeleteDirectory(sFileName);
                    }
                }
            } while(FindNextFile(filehandle, &filedata));
            FindClose(filehandle);
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfOmron::CommOmronReceiveData(TObject *Sender,
      Pointer Buffer, WORD BufferLength)
{                                                                               //JerryYang 20201223 Omron通訊由PComm改為使用TComm元件
    if(InitialOK==false)                                                        //Steven 20120202 : 加入Thread保護
        return;

    if(BufferLength==0)
        return;
    byte *data;

    if(BufferLength>1024)                                                       //JimmyChiu 20210922 Avoid overflow
    {
        LogMsg.sprintf("Receive (%d): %s", BufferLength, Buffer);
        WriteInfoToMemo(LogMsg);
        return;
    }

    char temp[1024]={'\0'};
    data=(byte *)Buffer;
    memcpy(temp, Buffer, BufferLength);

    for(int i=0; i<BufferLength; i++)                                           //JerryYang 20210310 : 修正字串串接時少一碼的問題
    {
        if(i>0 && MyDeCodeASCII(data[i-1])=="ETX")
        {

        }
        else if(MyDeCodeASCII(data[i])=="ETX")
        {
            asTStr=asTStr+"[ETX]";
            if(i+1<BufferLength)
            {
                asBCC=MyDeCodeASCII(data[i+1]);
                asTStr=asTStr+"("+asBCC+")";
            }
        }
        else if(MyDeCodeASCII(data[i])=="STX")
        {
            asTStr=asTStr+"[STX]";
        }
        else
        {
            asTStr=asTStr+MyDeCodeASCII(data[i]);
        }
    }
}
//------------------------------------------------------------------------------
void TfOmron::Initial_DTME08()
{
    this->Caption                   ="DTME08 Thermo Controller";
    Timer2->Enabled                 =false;
    GroupBox2->Visible              =false;
    grpStatus->Visible              =false;
    btSetSV->Visible                =false;
    btResetCOM->Visible             =false;
    Button3->Visible                =false;
    tsAuto->TabVisible              =false;
    tsManual->TabVisible            =false;
    tsRecord->TabVisible            =false; 
    PageControl1->ActivePageIndex   =2;
    gbAT->Caption                   ="Controller";
    for(int i=0; i<gbAT->ControlCount; i++)
        gbAT->Controls[i]->Visible=false;

    SetBtnParameter(btAT, 0);
    SetBtnParameter(btAT1by1, 1);
    if(USE_16_HEATER==eht32HeaterDTME08)
    {
        SetBtnParameter(btATOff, 2);
        SetBtnParameter(btRT, 3);
    }
}
//------------------------------------------------------------------------------
void TfOmron::SetBtnParameter(TButton * _btn, int _index)
{
    _btn->Visible=true;
    _btn->Caption=IntToStr(_index);
    _btn->OnClick=btnDTME08Click;
}
//------------------------------------------------------------------------------
void __fastcall TfOmron::btnDTME08Click(TObject *Sender)
{

}
//------------------------------------------------------------------------------
void __fastcall TfOmron::Edit1Click(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 0, 323);
}
//------------------------------------------------------------------------------