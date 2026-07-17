// =============================================================================
//  Automation/automation.cpp  --  TfAutomation implementation
//
//  Translation wave: W906-Automation
//  Translator: AI(W906-Automation) 20260716
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/Automation/automation.cpp
//                 (2501 lines, cp950) -- see automation.h for the full scope
//                 note (in-scope method list, deferred bucket, omitted-widget
//                 list, and the fAutomation/fAutomationEngine naming note).
//
//  Big5: decoded via cp950 (python `open(path, encoding='cp950')`) before
//  translation; reproduced here as correct UTF-8. Final gate: ZERO U+FFFD
//  bytes (verified before hand-off).
// =============================================================================
#include "automation.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "MachineDefine.h"
#include "cmydef.h"                     // CUSTOMER_CODE, CC_*, CosFunction, InitialOK, SoftStart, LogSoftwareOffTime
#include "Config.h"                     // IniConfig (sN08OlpIP/sN08OlpPort/bN08_1SaveOLPLog)
#include "common.h"                     // asOLPLogPath (real, common.cpp:116)
#include "canary_support.h"             // RecordProcess
#include "FormsFacade.h"                // fMain->palMainStatus/cbSetupFileName/edWorkTemperBase (+Home, this front's FormsFacade gap)
#include "auto9045.h"                   // GetHandlerType/GetDataPath/GetMainStatus/CheckInitialOK/GetTestResult/AlarmMessage/ShowOLPState (all REAL)
#include "Public/WinSocketErrorCode.h"  // GetErrorMsg
#include "Public/cJSON.h"               // cJSON_*

// =============================================================================
//  TU-LOCAL STOPGAPS (each cites its golden home + why a shared real
//  definition is not yet available; same discipline as auto9045.cpp's own
//  "GATED DEPENDENCIES" block)
// =============================================================================

// ---- bLockByServer: golden ckernel.cpp:52 (NOT YET translated) -------------
// AI(W906-Automation) 20260716: golden `extern bool bLockByServer;`
// (automation.cpp:949, immediately before the deferred ProcessBuffer ladder)
// needs SOME translation unit to define the symbol so a future ProcessBuffer
// translation links. Real golden home is ckernel.cpp:52 verbatim:
// `bool bLockByServer=false; // 2009.11.13 , Joye` -- ckernel.cpp itself is
// not translated this wave. Parking the definition here (this front's own
// file) rather than faking a ckernel.cpp stand-in. REMOVE THIS when
// ckernel.cpp is translated for real (it will define the same symbol ->
// duplicate-definition link error otherwise) -- flagged loudly so that
// future translator doesn't miss it.
bool bLockByServer = false;

// ---- NewRecordProcess: golden home cMyDB.h (via acatchtray_shims.h, REAL,
// already translated: `void NewRecordProcess(AnsiString, AnsiString, AnsiString)
// {}` in acatchtray_shims.cpp). Redeclare the extern locally rather than
// #include-ing acatchtray_shims.h wholesale (same precedent already used by
// several TUs for MyDBIProcess -- see the next stopgap below).
extern void NewRecordProcess(AnsiString S1, AnsiString S2 = "", AnsiString S3 = "");

// ---- MyDBIProcess: golden home aHotPlateSubstrate.cpp:703 (REAL, already
// translated: `void MyDBIProcess(AnsiString, AnsiString) {}`). Redeclare the
// extern locally, same precedent already used by CCLink/MyCCLink.cpp /
// KYECFTP/FTPClient_*.cpp / SECSGEM/uHGemClass.cpp (NOT #include-ing
// aHotPlateSubstrate.h wholesale -- that header also declares an unrelated
// global `class TList` that collides with vclcompat::TList if both are
// visible unqualified in one TU, see KNOWLEDGE.md; this file avoids the
// question entirely by not including that header).
extern void MyDBIProcess(AnsiString S1, AnsiString S2);

// ---- LogClientSocketExceptionError: DEFERRED tree-wide ---------------------
// Golden home Public/WinSocketErrorCode.h/.cpp explicitly documents this as
// DEFERRED (couples to VCL TClientSocket->Name/Address/Port + MyDBIProcess).
// Same precedent as BarCode/BarCode_8CCD_Glue.cpp's
// Glue8CCD_LogClientSocketExceptionError: a distinctly-named TU-local
// wrapper routed to RecordProcess (a real log line), not a silent no-op.
static void W906Auto_LogClientSocketExceptionError(TObject * /*Sender*/, const AnsiString &where)
{
    RecordProcess("ClientSocketException: " + where, "Automation");
}

// ---- MyForceDirectories: golden common.h:262 / common.cpp:1667-1717 ------
// Declared in common.h but common.cpp's own body is gated (`#if 0 //
// TODO(wave-N)`, out of THIS unit's scope). TU-local faithful copy of the
// CORE action (create the directory if missing), same precedent + same
// documented omissions as Automation/auto9045.cpp's own W5FA_MyForceDirectories
// gate #1 (auto9045.cpp:101-123): the golden auto-detect-trailing-filename
// branch and the try/catch around ForceDirectories are both omitted (every
// call site in THIS file passes a bare directory path, never a path with a
// trailing filename component, so the omission is behaviorally inert here).
static int W906Auto_MyForceDirectories(AnsiString Directory, AnsiString Function = "")
{
    if (Directory == "")
    {
        RecordProcess("Directory value is NULL!", Function);
        return -1;
    }
    if (DirectoryExists(Directory) == false)
        ForceDirectories(Directory);
    return 1;
}
#define MyForceDirectories W906Auto_MyForceDirectories

//---------------------------------------------------------------------------
// AI(W906-Automation) 20260716: see automation.h's "GLOBAL-NAME COLLISION
// AVOIDED" note -- golden's bare `fAutomation` is already claimed by
// atester_shims.h's TfAutomationShim. This is the REAL engine singleton;
// DEFINED (self-instantiated) at the bottom of this file, after
// TfAutomation's own methods -- see that definition's own comment.

//Steven 20100830 : For OLP
static bool bStandard = true;   //Steven 20110208
bool bRestConnect = false;      //Ifor 20170517 (Steven) add OLP Server 若連線數 > 1 重置 Server
AnsiString TCPstr = "";
AnsiString sDLFileName = "";    // only referenced by the deferred ProcessBuffer1 -- kept for shape fidelity
AnsiString ReceiveString = "";
AnsiString TCPstr2 = "";
AnsiString ReceiveString2 = "";
AnsiString STX;
AnsiString SOH;
AnsiString ETX;
AnsiString stx = "[STX]";
AnsiString soh = "[SOH]";
AnsiString etx = "[ETX]";
AnsiString HandlerType;
AnsiString aDataPath;
bool WRITE = false;
bool READ = true;
//Steven 20100830 End

//ChungHung 20101119 Start
static const int iMaxEvent = 10;
struct sResult
{
    bool def;
    AnsiString CommandString;
    AnsiString NumString;
    sResult()
    {
        def = false;
        CommandString = "";
        NumString = "";
    }
};
static sResult MyEventResult[iMaxEvent];
//---------------------------------------------------------------------------
TfAutomation::TfAutomation(TComponent * /*Owner*/)
{
    // ---- widget stand-in construction + golden .dfm design-time defaults --
    // (no VCL streaming in this shim -- defaults set explicitly here; see
    // automation.h's own file-head "WIDGET STAND-INS" / "EVENT WIRING" notes)
    Memo     = new TfAutomationMemo();
    MemoChar = new TfAutomationMemo();
    MemoCode = new TfAutomationMemo();
    OLPServer = new TServerSocket(NULL);
    OLPServer->Port = 5001;                          // golden automation.dfm: OLPServer.Port=5001
    tmrOLP = new TfAutomationTimer();
    OLPClient = new TClientSocket(NULL);
    OLPClient->Address = "10.33.10.70";              // golden automation.dfm: OLPClient.Address
    OLPClient->Port = 5002;                          // golden automation.dfm: OLPClient.Port
    btUDPSend = new TfAutomationVisibleFlag();
    gbAutomation = new TfAutomationVisibleFlag();
    OnLine = new TfAutomationEnableFlag();
    OnLine2 = new TfAutomationEnableFlag();
    cbbOLPCommand = new TfAutomationTextField();
    cbbOLPCommand->Text = "0001";                    // golden automation.dfm: cbbOLPCommand.Text
    chkViewComm = new TfAutomationCheckFlag();
    btnConnect = new TfAutomationEnableFlag();
    btUDPMap = new TfAutomationVisibleFlag();
    btUDPTemp = new TfAutomationVisibleFlag();
    edinputIP = new TfAutomationTextField();
    edinputIP->Text = "192.168.120.143";             // golden automation.dfm: edinputIP.Text
    edinputport = new TfAutomationTextField();
    edinputport->Text = "6670";                      // golden automation.dfm: edinputport.Text

    // ---- event wiring (golden automation.dfm) ------------------------------
    OLPServer->OnClientConnect = [this](TObject *Sender, TCustomWinSocket *Socket)
        { OLPServerClientConnect(Sender, Socket); };
    OLPServer->OnClientDisconnect = [this](TObject *Sender, TCustomWinSocket *Socket)
        { OLPServerClientDisconnect(Sender, Socket); };
    OLPServer->OnClientRead = [this](TObject *Sender, TCustomWinSocket *Socket)
        { OLPServerClientRead(Sender, Socket); };
    OLPServer->OnClientError = [this](TObject *Sender, TCustomWinSocket *Socket,
                                       TErrorEvent ErrorEvent, int &ErrorCode)
        { OLPServerClientError(Sender, Socket, ErrorEvent, ErrorCode); };
    // ADAPTATION (vclcompat API gap, not a golden quirk): golden automation.dfm
    // wires `OnConnecting = OLPClientConnecting` (real VCL TClientSocket fires
    // OnConnecting when a connect ATTEMPT starts, distinct from OnConnect
    // firing on SUCCESS). This front's write scope excludes
    // vclcompat/ClientSocket.h, whose TClientSocket does not expose an
    // OnConnecting event slot (only OnConnect/OnDisconnect/OnError/OnRead --
    // see that header's own SCOPED API SURFACE note, reverse-engineered only
    // from MyPLC/ModbusTCPClient.cpp's call sites, which never needed
    // OnConnecting). OLPClientConnecting itself IS still translated below
    // (real body, callable directly / by a future test) -- it is simply not
    // auto-fired by this shim today. Flagged for a future wave to add a real
    // TClientSocket::OnConnecting to vclcompat/ClientSocket.h if this becomes
    // load-bearing.
    OLPClient->OnConnect = [this](TObject *Sender, TCustomWinSocket *Socket)
        { OLPClientConnect(Sender, Socket); };
    OLPClient->OnDisconnect = [this](TObject *Sender, TCustomWinSocket *Socket)
        { OLPClientDisconnect(Sender, Socket); };
    OLPClient->OnRead = [this](TObject *Sender, TCustomWinSocket *Socket)
        { OLPClientRead(Sender, Socket); };
    OLPClient->OnError = [this](TObject *Sender, TCustomWinSocket *Socket,
                                 TErrorEvent ErrorEvent, int &ErrorCode)
        { OLPClientError(Sender, Socket, ErrorEvent, ErrorCode); };

    try
    {
        STX.sprintf("%c", 0x02);
        SOH.sprintf("%c", 0x01);
        ETX.sprintf("%c", 0x03);
        OLPClientConnected = false;
        if (CUSTOMER_CODE == CC_ASE_CL)
        {
            bStandard = true;   //Steven 20110208
            OLPServer->Open();
            gbAutomation->Visible = true;
        }
        else if (CUSTOMER_CODE == CC_HTML_Monitor ||
                 CUSTOMER_CODE == CC_Greatek ||   //Jimmychiu 20260318 : 超豐加入透過Socket遠端控制
                 CUSTOMER_CODE == CC_MTI ||
                 CUSTOMER_CODE == CC_PTI)         //Sam 20190429 : Add CC_PTI_NEWWORK
        {
            OLPServer->Port = 6671;               //Sam 20210812 : PTI 強制寫死6671
            #ifdef SOFT_SIMULTE
            OLPServer->Port = 5001;
            #endif
            bStandard = false;
            OLPServer->Open();
            gbAutomation->Visible = true;
        }
        else
        {
            bStandard = true;
            OLPServer->Close();
            gbAutomation->Visible = false;
        }

        btUDPSend->Visible = (CosFunction.bAmkorFunction || CUSTOMER_CODE == CC_QUALCOMM); //Steven 20120330 : ATK Site Map Monitorning //JerryYang 20170412 (Steven) add QUALCOMM
        btUDPMap->Visible  = (CosFunction.bAmkorFunction || CUSTOMER_CODE == CC_QUALCOMM);
        btUDPTemp->Visible = (CosFunction.bAmkorFunction || CUSTOMER_CODE == CC_QUALCOMM);

        if (CUSTOMER_CODE == CC_HTML_Monitor ||
            CUSTOMER_CODE == CC_MTI ||
            CUSTOMER_CODE == CC_PTI)   //Sam 20190429 : Add CC_PTI_NEWWORK
        {
            OLPClient->Address = edinputIP->Text;
            OLPClient->Port = atoi(edinputport->Text.c_str());
            OLPClient->Open();
        }
        else
        {
            OLPClient->Close();
        }

        if (CosFunction.bOLPFunction)   //Steven 20141229 : OLP功能
            tmrOLP->Enabled = true;
        else
            tmrOLP->Enabled = false;

        //Steven 20100830 Start: For OLP
        CommandBuffer = new vclcompat::TList();   // 2009.11.13 , Joye
        CommandBuffer->Clear();
        CommandClientReplyBuffer = new TStringList();   //Sam 20200813 : 增加 Log debug
        CommandClientReplyBuffer->Clear();

        GetTimeInfo();
        HandlerType = GetHandlerType();
        aDataPath = GetDataPath();

        bReceive = false;
        bReceive2 = false;
        iSocketHandle = 0;   //Sam 20200727 : 修正連線問題

        bReceiveHasData = false;   // 2011.09.05 , Joye , OLP
        bOneCycle = false;
        bCleanOut = false;

        TestMode = 0;
        // golden also zeros 4 ATK char[256] buffers here (sATKSendMessage/
        // sATKSendTemp/sATKSendMap/sATKSendTempAndMap) -- all 4 are part of
        // the omitted NMUDP1/Amkor block (see automation.h's file-head "NOT
        // DECLARED" note), so there is nothing left to zero here.
    }
    catch (...)
    {
        MyDBIProcess("Exception", "TfAutomation::TfAutomation");
    }
    iClientReConnect = 0;   //Sam 20240226 : Client 重連次數
    brecordmsgLock = false;
}
//---------------------------------------------------------------------------
bool TfAutomation::GetEventNum(int &num, AnsiString cmdstr, AnsiString numstr)
{
    for (int i = 0; i < iMaxEvent; i++)
    {
        if (MyEventResult[i].CommandString == "")
        {
            num = i;
            SetEventResult(i, false, cmdstr, numstr);
            return true;
        }
    }
    num = -1;
    return false;
}
//---------------------------------------------------------------------------
bool TfAutomation::SetEventResult(int num, bool def, AnsiString cmdstr, AnsiString numstr)
{
    if (num >= 0 && num < iMaxEvent)
    {
        MyEventResult[num].def = def;
        MyEventResult[num].CommandString = cmdstr;
        MyEventResult[num].NumString = numstr;

        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
bool TfAutomation::GetEventResult(int num)
{
    bool result = false;
    if (num >= 0 && num < iMaxEvent)
    {
        result = MyEventResult[num].def;
        //str=MyEventResult[num].GetString;
        return result;
    }
    return result;
}
//---------------------------------------------------------------------------
void TfAutomation::CompareEvent(AnsiString str)
{
    for (int i = 0; i < iMaxEvent; i++)
    {
        if (MyEventResult[i].CommandString != "")
        {
            if (str.Pos(MyEventResult[i].CommandString) != 0 &&
                str.Pos(MyEventResult[i].NumString) != 0 || MyEventResult[i].NumString == "")
            {
                SetEventResult(i, true, "", "");
                return;
            }
        }
    }
}
//---------------------------------------------------------------------------
void TfAutomation::ClearEvent(int num)
{
    SetEventResult(num, false, "", "");
    return;
}
//ChungHung 20101119 End
//---------------------------------------------------------------------------
AnsiString SplitDataBySoh(AnsiString &Buffer)
{
    int pos;
    AnsiString Data;
    pos = Buffer.Pos(SOH);
    if (pos == 0)
        pos = Buffer.Pos(ETX);

    Data = Buffer.SubString(1, pos - 1);
    Buffer.Delete(1, pos);
    return Data;
}
//---------------------------------------------------------------------------
AnsiString GetCRC(AnsiString &Buffer)
{
    AnsiString Data;
    Data = Buffer.SubString(1, 5);
    Buffer.Delete(1, 1);
    // preserved golden quirk: only removes 1 char here, not the 5 chars just
    // extracted above -- golden automation.cpp:225-231 verbatim. GetCRC has
    // no in-scope caller this wave (translated for shape fidelity as one of
    // this front's assigned "6 zero-dependency free functions"); not "fixed".
    return Data;
}
//---------------------------------------------------------------------------
AnsiString AddBlock(AnsiString S)
{
    AnsiString S1;
    S1 = "[" + S + "]";
    return S1;
}
//---------------------------------------------------------------------------
bool IsStartWord(AnsiString &Buffer, AnsiString w)
{
    if (Buffer.Pos(w) == 1)
    {
        Buffer.Delete(1, 1);
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
AnsiString crc16(char *ip_str, int n)
{
    unsigned crctmp = 65535, ch;
    char str[256];
    AnsiString S;
    for (int i = 0; i < n; i++)
    {
        ch = ip_str[i];
        crctmp = crctmp ^ ch;
        for (int j = 0; j < 8; j++)
        {
            if ((crctmp & 1) == 1)
            {
                crctmp = crctmp / 2;
                crctmp = crctmp ^ 40961;
            }
            else
            {
                crctmp = crctmp / 2;
            }
        }
    }
    sprintf(str, "%5d", crctmp);   // preserved golden quirk: %d against an
                                   // `unsigned` arg (harmless in practice --
                                   // crctmp's value range fits in int).
    S = str;
    return S;
}
//---------------------------------------------------------------------------
AnsiString TfAutomation::GetTimeInfo()
{
    static TDateTime dtPresent;
    AnsiString TimeString;
    dtPresent = Now();
    DecodeDate(dtPresent, aSystemYear, aSystemMonth, aSystemDate);
    DecodeTime(dtPresent, aSystemHour, aSystemMin, aSystemSec, aSystemMSec);
    TimeString.sprintf("%04d%02d%02d%02d%02d%02d", aSystemYear, aSystemMonth, aSystemDate, aSystemHour, aSystemMin, aSystemSec);
    return TimeString;
}
//---------------------------------------------------------------------------
void TfAutomation::OLPServerClientConnect(TObject * /*Sender*/, TCustomWinSocket * /*Socket*/)
{
    // preserved golden quirk: neither the Sender nor the event's own Socket
    // parameter is used here -- the body reads via the class member
    // `OLPServer->Socket` instead (golden automation.cpp:288-321 verbatim).
    //Ifor 20170517 (Steven) add OLP Server 若連線數 > 1 重置 Server
    //==>
    int iConnectCount = (int)(OLPServer->Socket->ActiveConnections);

    if (CUSTOMER_CODE == CC_PTI)   //Sam 20200826 : PTI 允許多重連線
    {
        OnLine->Enabled = true;
        ShowOLPState(1);   // 2009.12.02 , Joye
        if (iConnectCount > 1)
        {
            NewRecordProcess("", "OLP server is about to reset due to too many connections", "iConnectCount=" + IntToStr(iConnectCount));
        }
    }
    else
    {
        if (iConnectCount == 1)
        {
            OnLine->Enabled = true;
            ShowOLPState(1);   // 2009.12.02 , Joye
        }
        else
        {
            NewRecordProcess("", "OLP server is about to reset due to too many connections", "iConnectCount=" + IntToStr(iConnectCount));
            bRestConnect = true;
            ShowOLPState(0);
        }
    }

    //<==
    //Ifor 20170517 (Steven) add OLP Server 若連線數 > 1 重置 Server
}
//---------------------------------------------------------------------------
void TfAutomation::OLPServerClientDisconnect(TObject * /*Sender*/, TCustomWinSocket * /*Socket*/)
{
    OnLine->Enabled = false;
    ShowOLPState(0);   // 2009.12.02 , Joye
}
//---------------------------------------------------------------------------
void TfAutomation::OLPServerClientError(TObject *Sender, TCustomWinSocket * /*Socket*/,
                                         TErrorEvent /*ErrorEvent*/, int &ErrorCode)
{
    ShowMSG(GetErrorMsg(Sender, ErrorCode));
    if (ErrorCode == 10053)   // 網路線拔除
    {
        ShowOLPState(2);   // 2009.12.02 , Joye
    }
    else if (ErrorCode == 10061)   //連線失敗
    {
        ShowOLPState(0);   // 2009.12.02 , Joye
    }
    else
    {
        ShowOLPState(0);
    }
    ErrorCode = 0;
    OnLine->Enabled = false;
    try
    {
        OLPServer->Close();
    }
    catch (...)
    {
        W906Auto_LogClientSocketExceptionError(Sender, "OLPServer Error");
    }
    OLPServer->Open();
}
//---------------------------------------------------------------------------
void TfAutomation::OLPServerClientRead(TObject * /*Sender*/, TCustomWinSocket *Socket)
{
    int StartPos = 0, EndPos = 0;
    if (ReceiveString.Length() > 1024)   //jimmychiu 20251009 : avoid msg too long to dead
    {
        ShowMSG(AnsiString().sprintf("OLPServerClientRead too long to delete, %s", ReceiveString));
        ReceiveString = "";
    }
    //接收字串
    ReceiveString += Socket->ReceiveText();   //Sam 20200727 : 修正連線問題
    iSocketHandle = Socket->SocketHandle;
    //尋找STX
    StartPos = ReceiveString.Pos(STX);
    //尋找ETX
    EndPos = ReceiveString.Pos(ETX);

    if (bReceive == false && StartPos > 0 && EndPos > 0 && StartPos < EndPos)
    {
        // preserved golden quirk: SubString(StartPos,EndPos) uses EndPos as a
        // CHARACTER COUNT (vclcompat AnsiString::SubString(start,len), real
        // BCB6 semantics), not an "end position" -- only correct when
        // StartPos==1 (the common case: STX at the very start of a freshly-
        // accumulated buffer). If leftover data ever preceded STX
        // (StartPos>1), this would over-read past the ETX. Golden
        // automation.cpp:378 verbatim; not "fixed" here.
        TCPstr = ReceiveString.SubString(StartPos, EndPos);
        ReceiveString.Delete(1, EndPos);
        bReceive = true;
    }
    else if (ReceiveString.Length() > 5000)
    {
        ReceiveString = "";
    }

    if (bReceive)
    {
        ProcessBuffer(TCPstr, iSocketHandle);   // GATED -- see automation.h file-head note //Sam 20200813 : 增加 Log debug //被動回傳
        bReceive = false;

        // 2011.09.05 , Joye , OLP ------------------------
        if (ReceiveString.Length() > 0)
        {
            bReceiveHasData = true;
        }
        else
            bReceiveHasData = false;
        // ------------------------------------------------
    }
}
//---------------------------------------------------------------------------
void TfAutomation::CommandProcess(AnsiString CMD, AnsiString V_TOTAL, AnsiString *Data,
                                   bool Standard, int iHandle)   //Sam 20200813 : 增加 Log debug
{
    AnsiString HEAD, head, SendString, S2, cmd, v_total;

    cmd = AddBlock(CMD);
    v_total = AddBlock(V_TOTAL);

    HEAD = STX + CMD + SOH + V_TOTAL;
    head = stx + cmd + soh + v_total;

    for (int i = 0; i < atoi(V_TOTAL.c_str()); i++)
    {
        HEAD = HEAD + SOH + Data[i];
        head = head + soh + AddBlock(Data[i]);
    }
    HEAD += ETX;
    head += etx;

    SendString = HEAD;
    S2 = head;
    if (Standard == true)
    {
        bReceive = false;
        SendClient(SendString, S2, iHandle);   //Sam 20200813 : 增加 Log debug
    }
    else
    {
        bReceive = false;
        SendServer(SendString, S2);
    }
}
//---------------------------------------------------------------------------
void TfAutomation::tmrOLPTimer(TObject * /*Sender*/)   //主動回傳
{
    static bool bRun = false;
    if (bRun)
        return;
    bRun = true;
    //
    static int TimeCount = 0;
    static bool bfirst = true;   // golden: static int bfirst=true (used as a bool flag) -- translated to bool, same truthiness
    AnsiString S = "";
    int iConnectCount = 0;
    static bool bRecordState = false;
    if (CosFunction.bOLPFunction == false || InitialOK == false)
    {
        bRun = false;
        return;
    }

    if (bfirst)
    {
        // golden: `fAutomation->edinputIP->Text=...;` -- `fAutomation` is
        // this same singleton (only one TfAutomation instance ever exists in
        // this shim), so direct member access is behaviorally identical; see
        // automation.h's own "EVENT WIRING" note.
        edinputIP->Text = IniConfig.sN08OlpIP;
        if (IniConfig.sN08OlpPort == "")
            IniConfig.sN08OlpPort = "6670";
        edinputport->Text = IniConfig.sN08OlpPort;
        bfirst = false;
    }

    if (SendInvalid == false)
    {
        SendInvalid = true;
        TimeCount = 0;
    }

    if (SendInvalid == true && TimeCount >= 10000 && OnLine2->Enabled == true)
    {
        SendOLPClient(" " + ETX);
        SendInvalid = false;
        TimeCount = 0;
    }
    else if (OnLine2->Enabled == false)
    {
        if (CUSTOMER_CODE == CC_MTI || CUSTOMER_CODE == CC_PTI)   //Sam 20190429 : Add CC_PTI_NEWWORK
        {
            if (iClientReConnect < 10)   //Sam 20240226 : Client 重連次數
            {
                iClientReConnect++;
                OLPClient->Address = edinputIP->Text;
                OLPClient->Port = atoi(edinputport->Text.c_str());
                OLPClient->Open();
            }
        }
    }
    else
    {
        TimeCount++;
    }
    //Ifor 20170517 (Steven) add OLP Server 若連線數 > 1 重置 Server
    //==>
    try
    {
        if (bRestConnect == true && bReceive == false)   //Sam 20200727 : 修正連線問題
        {
            OLPServer->Close();
            OLPServer->Open();
            OnLine->Enabled = false;
            bRestConnect = false;
            bRecordState = false;
            NewRecordProcess("", "OLP server reset !!!", "");   //Sam 20200826 : Add Log
            bRun = false;
            return;
        }
        else if (bRestConnect == true && bReceive == true && bRecordState == false)   //Sam 20200813 : 增加 Log debug
        {
            bRecordState = true;
            NewRecordProcess("", "OLP server will reset connect,But client is receiving data!!!", "");
        }
    }
    catch (...)
    {
        MyDBIProcess("Exception", "TfAutomation::tmrOLPTimer");
    }
    //<==
    //Ifor 20170517 (Steven) add OLP Server 若連線數 > 1 重置 Server

    if (CheckInitialOK() == false)
    {
        bRun = false;
        return;
    }
    AnsiString Data[32], TimeString = "";   //Sam 20191126 : 20>32
    AnsiString MainStatus = GetMainStatus();
    MyProcessBuffer *ProcessPtr;
    static int Task = 1;
    static MyProcessBuffer Process;
    static AnsiString SysStatus = "";
    int DutCount;

    if (SysStatus == "")
        SysStatus = MainStatus;

    if (MainStatus != SysStatus)
    {
        SendReportRequest("0001", bStandard);   // GATED -- see automation.h file-head note
        SysStatus = MainStatus;
    }

    switch (Task)
    {
        case 1:
            if (CommandBuffer->Count != 0)
            {
                if ((bStandard == true && OnLine->Enabled == true) || (bStandard == false && OnLine2->Enabled == true))
                {
                    ProcessPtr = (MyProcessBuffer *)CommandBuffer->Items[0];
                    Process.Command = ProcessPtr->Command;
                    Process.SubCommand = ProcessPtr->SubCommand;
                    Process.AlarmID = ProcessPtr->AlarmID;
                    Process.AlarmMessage = ProcessPtr->AlarmMessage;
                    Process.TimeString = ProcessPtr->TimeString;
                    Process.ErrType = ProcessPtr->ErrType;
                    CommandBuffer->Delete(0);
                    Task = 100;
                }
                else
                {
                    WriteTCPDataToTextFile();   //2008/08/05 lee
                }
            }
            break;
        case 100:
            if (Process.Command == "ALARM_REQUEST")
            {
                Data[0] = GetTimeInfo();
                Data[1] = Process.AlarmID;
                Data[2] = "1";   //Steven 20110802 : Count固定為1
                Data[3] = Process.AlarmMessage;
                CommandProcess(Process.Command, 4, Data, bStandard);
            }
            else if (Process.Command == "MESSAGE_REQUEST")
            {
                Data[0] = GetTimeInfo();
                Data[1] = Process.AlarmMessage;
                CommandProcess(Process.Command, 2, Data, bStandard);
            }
            else if (Process.Command == "PRODUCTION_REQUEST")
            {
                Data[0] = GetTimeInfo();
                Data[1] = Process.AlarmMessage;
                CommandProcess(Process.Command, 2, Data, bStandard);
            }
            else if (Process.Command == "PRODUCTION_REPLY")
            {
                Data[0] = Process.AlarmMessage;   //Brian,20181130
                Data[1] = "0";                    //Brian,20181130
                CommandProcess(Process.Command, 2, Data, bStandard);
            }
            else if (Process.Command == "TEST_RESULT_REQUEST")
            {
                DutCount = GetTestResult(Data);
                CommandProcess("TEST_RESULT_REQUEST", DutCount, Data, bStandard);
            }
            Task = 1;
            break;
    }

    // 2011.09.05 , Joye , OLP ------------------------>>
    if (bReceiveHasData == true)
    {
        //尋找STX
        int StartPos = ReceiveString.Pos(STX);
        //尋找ETX
        int EndPos = ReceiveString.Pos(ETX);

        if (StartPos != 0 && EndPos != 0 && StartPos < EndPos)
        {
            // same preserved SubString(start,len)-as-EndPos quirk as
            // OLPServerClientRead above -- see that method's own comment.
            AnsiString sSubTCPstr = ReceiveString.SubString(StartPos, EndPos);
            ReceiveString.Delete(1, EndPos);

            ProcessBuffer(sSubTCPstr, iSocketHandle);   // GATED -- see automation.h file-head note //Sam 20200813 : 增加 Log debug //被動回傳

            if (ReceiveString.Length() > 0)
            {
                bReceiveHasData = true;
            }
            else
            {
                bReceiveHasData = false;
            }
        }
    }
    // 2011.09.05 , Joye , OLP ------------------------<<

    if (CommandClientReplyBuffer->Count >= 1)   //Sam 20200813 : send client data error add retry send
    {
        iConnectCount = (int)(OLPServer->Socket->ActiveConnections);
        if (OnLine->Enabled == true && iConnectCount >= 1)
        {
            if (iConnectCount > 1)
            {
                NewRecordProcess("", "OLP connected warnning!!!", "iConnectCount=" + IntToStr(iConnectCount));
            }
            NewRecordProcess("", "OLP retry reply !!!", "Send socket handle=" + IntToStr(OLPServer->Socket->Connections[0]->Handle));   //Sam 20200826 : Add Log
            S = CommandClientReplyBuffer->Strings[0];
            OLPServer->Socket->Connections[0]->SendBuf(const_cast<char *>(S.c_str()), S.Length());
            ShowCharHex(S);
            CommandClientReplyBuffer->Delete(0);
        }
    }
    bRun = false;
}
//---------------------------------------------------------------------------
void TfAutomation::btnConnectClick(TObject * /*Sender*/)
{
    // preserved golden quirk: `Text!=0` compares Text to the STRING "0" (via
    // AnsiString's implicit int ctor), NOT "is text non-empty" -- golden
    // automation.cpp:648-651 verbatim.
    if (edinputIP->Text != 0)
        OLPClient->Address = edinputIP->Text;
    if (edinputport->Text != 0)
        OLPClient->Port = edinputport->Text.ToInt();

    try
    {
        if (CUSTOMER_CODE == CC_MTI || CUSTOMER_CODE == CC_PTI)   //Sam 20240226 : Client 重連次數
        {
            iClientReConnect = 0;
        }
        btnConnect->Enabled = false;
        OLPServer->Open();
    }
    catch (...)
    {
        MyDBIProcess("Exception", "TfAutomation::btnConnectClick");
    }
}
//---------------------------------------------------------------------------
void TfAutomation::btnDisconnectClick(TObject * /*Sender*/)
{
    if (CUSTOMER_CODE == CC_MTI || CUSTOMER_CODE == CC_PTI)
    {
        iClientReConnect = 0;
    }
    OLPServer->Close();
    OnLine->Enabled = false;

    ShowOLPState(0);   // 2009.12.02 , Joye
    btnConnect->Enabled = true;
}
//---------------------------------------------------------------------------
void TfAutomation::btEventReportClick(TObject * /*Sender*/)
{
    // preserved golden quirk: S is computed (and would decide "RUN" vs the
    // current status text) but then never actually used below -- golden
    // automation.cpp:681-701 verbatim (vestigial from an earlier version).
    AnsiString S;
    S = GetMainStatus().UpperCase();
    if (S == "PAUSE" || S == "HALT")
    {
    }
    else
    {
        S = "RUN";
    }

    if (CUSTOMER_CODE == CC_MTI || CUSTOMER_CODE == CC_PTI)   //Sam 20190429 : Add CC_PTI_NEWWORK
    {
        SendReportRequest(cbbOLPCommand->Text, 0);   // GATED -- see automation.h file-head note
    }
    else
    {
        SendReportRequest(cbbOLPCommand->Text);   // GATED -- see automation.h file-head note
    }
}
//---------------------------------------------------------------------------
void TfAutomation::btAlarmReportClick(TObject * /*Sender*/)
{
    DoCommandBuffer("ALARM_REQUEST", "", "InArm:Device pick-up error on Hot Plate", 1, "JAM0109");
}
//---------------------------------------------------------------------------
void TfAutomation::btTestResultClick(TObject * /*Sender*/)
{
    DoCommandBuffer("TEST_RESULT_REQUEST");
}
//---------------------------------------------------------------------------
void TfAutomation::MemoDblClick(TObject * /*Sender*/)
{
    SaveRecord();
    Memo->Clear();
    MemoChar->Clear();
    MemoCode->Clear();
}
//---------------------------------------------------------------------------
void TfAutomation::ShowCharHex(AnsiString S)
{
    if (chkViewComm->Checked == false)
        return;

    AnsiString S1 = "", S2 = "";
    char s[12];
    char Target[10240];
    int len, i;
    strncpy(Target, S.c_str(), sizeof(Target));
    len = strlen(Target);
    for (i = 0; i < len; i++)
    {
        if (Target[i] == 0x01 || Target[i] == 0x02 || Target[i] == 0x03)
        {
            if (Target[i] == 0x02)
                s[0] = '<';
            if (Target[i] == 0x01)
                s[0] = '=';
            if (Target[i] == 0x03)
                s[0] = '>';

            s[1] = ' ';
            s[2] = 0;
        }
        else
        {
            s[0] = Target[i];
            s[1] = ' ';
            s[2] = 0;
        }
        S1 += s;

        // preserved golden quirk: `Target[i]` is a (signed, on this
        // toolchain) `char`; for byte values >=0x80 this sign-extends
        // through the varargs promotion to int, so `%02X` (a MINIMUM width,
        // not a max) prints an 8-hex-digit "FFFFFFxx" instead of 2 digits --
        // golden automation.cpp:754 verbatim, not "fixed".
        sprintf(s, "%02X ", Target[i]);
        S2 += s;
    }
    MemoChar->Lines->Add(S1);
    MemoChar->Lines->Add("");
    MemoCode->Lines->Add(S2);
    MemoCode->Lines->Add("");

    if (MemoChar->Lines->Count > 1000)
    {
        MemoChar->Clear();
    }

    if (MemoCode->Lines->Count > 1000)
    {
        MemoCode->Clear();
    }
}
//---------------------------------------------------------------------------
void TfAutomation::ShowRecord(bool bRead, AnsiString S, int iHandle)   //Sam 20200813 : 增加 Log debug
{
    if (IniConfig.bN08_1SaveOLPLog == false && chkViewComm->Checked == false)
        return;

    AnsiString S2;
    char str[256];
    if (bRead)
        sprintf(str, "%s [Receive] [%06d]:", GetTimeInfo().c_str(), iHandle);   //Sam 20200813 : 增加 Log debug
    else
        sprintf(str, "%s [Send   ] [%06d]:", GetTimeInfo().c_str(), iHandle);   //Sam 20200813 : 增加 Log debug
    S2 = AnsiString(str) + S;
    ShowMSG(S2);
}
//---------------------------------------------------------------------------
void TfAutomation::SaveRecord()   //Steven 20141229 : OLP的Log要存檔
{
    AnsiString FileName;

    if (CUSTOMER_CODE == CC_PTI)   //Sam 20200812 : PTI 暫時強制打開
    {
        IniConfig.bN08_1SaveOLPLog = true;
    }

    if (IniConfig.bN08_1SaveOLPLog)
    {
        FileName.sprintf("%s\\%04d_%02d", asOLPLogPath, SystemYear, SystemMonth);
        MyForceDirectories(FileName);
        FileName.sprintf("%s\\%04d_%02d\\%04d-%02d-%02d %02d-%02d-%02d.log", asOLPLogPath, SystemYear, SystemMonth, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
        Memo->Lines->SaveToFile(FileName);
    }
}
//---------------------------------------------------------------------------
void TfAutomation::SendClient(AnsiString S, AnsiString S2, int iHandle)
{
    int i;
    int iConnectCount = (int)(OLPServer->Socket->ActiveConnections);
    int iConnect = -1;

    if (OnLine->Enabled == true && iConnectCount >= 1)   //Sam 20200727 : 如果連接數大於1以上，以 SocketHandle 來確保回傳
    {
        iConnect = -1;
        for (i = 0; i < iConnectCount; i++)   //找尋對應 iConnect Client Handle
        {
            if (iHandle != iSocketHandle)
            {
                //iHandle != iSocketHandle 時做紀錄 Debug 用
                NewRecordProcess("", "OLP iHandle not match iSocketHandle", "iHandle=" + IntToStr(iHandle) + ",iSocketHandle=" + IntToStr(iSocketHandle));
            }

            //基本上 iHandle & iSocketHandle 是一樣的東西 Debug 中先這樣弄
            if (OLPServer->Socket->Connections[i]->SocketHandle == iHandle)   //從資料流丟過來的 Handle
            {
                iConnect = i;
                break;
            }
            else if (OLPServer->Socket->Connections[i]->SocketHandle == iSocketHandle)   //接收到 Client 時紀錄 Handle
            {
                iConnect = i;
                break;
            }
        }

        if (iConnect == -1)
        {
            //如果真的找不到對應 Handle 就丟 iConnect=0
            NewRecordProcess("", "OLP Not Find SocketHandle", "iConnectCount=" + IntToStr(iConnectCount));
            iConnect = 0;
        }

        // preserved golden quirk: since the block above forces iConnect=0
        // whenever it was -1, `iConnect` can NEVER be -1 by this point --
        // the `else` branch below (the AlarmMessage call) is dead/
        // unreachable code. Golden automation.cpp:836-853 verbatim; not
        // "fixed" here.
        if (iConnect != -1)
        {
            OLPServer->Socket->Connections[iConnect]->SendBuf(const_cast<char *>(S.c_str()), S.Length());
            ShowCharHex(S);
            ShowRecord(WRITE, S2, OLPServer->Socket->Connections[iConnect]->SocketHandle);
        }
        else
        {
            if (bStandard == false)
                AlarmMessage("Not Find SocketHandle.", "找不到 SocketHandle ...");
        }
    }
    else
    {
        ShowRecord(WRITE, S2, iHandle);
        CommandClientReplyBuffer->Add(S);
        NewRecordProcess("", "OLP Wait client connect", "iHandle=" + IntToStr(iHandle) + ", iSocketHandle=" + IntToStr(iSocketHandle) + ", iConnectCount=" + IntToStr(iConnectCount));   //Sam 20200826 : Add Log
        if (bStandard == false)
            AlarmMessage("Wait client connect.", "等待連線...");
    }
}
//---------------------------------------------------------------------------
void TfAutomation::SendServer(AnsiString S, AnsiString S2)
{
    int iHandle = 0;
    if (OnLine2->Enabled == true)   //是否連線中
    {
        iHandle = (int)OLPClient->Socket->Handle;
        OLPClient->Socket->SendBuf(const_cast<char *>(S.c_str()), S.Length());
        ShowCharHex(S);
        ShowRecord(WRITE, S2, iHandle);   //Sam 20200813 : 增加 Log debug
    }
    else
    {
        if (IniConfig.bN08_1SaveOLPLog == false && chkViewComm->Checked == false)   //Sam 20240226 : Client 重連次數
            return;

        char str[256];
        sprintf(str, "%s [Send Error] [%06d]:", GetTimeInfo().c_str(), 0);   //Sam 20200813 : 增加 Log debug
        S2 = AnsiString(str) + S2;
        ShowMSG(S2);
    }
}
//---------------------------------------------------------------------------
void TfAutomation::WriteTCPDataToTextFile()   //把離線的相關資料寫到Log檔
{
    AnsiString str = "", datestr = "";
    FILE *file;
    MyProcessBuffer *ProcessPtr;
    MyProcessBuffer *Process = new MyProcessBuffer();
    GetTimeInfo();

    // preserved golden quirk: this line dereferences CommandBuffer->Items[0]
    // UNCONDITIONALLY, before the `if(CommandBuffer->Count>0)` guard below --
    // vclcompat::TList's Items[] is an UNCHECKED std::vector subscript (see
    // TList.h's own "mirrors BCB6" note), i.e. undefined behavior if this
    // were ever reached with an empty list. Golden automation.cpp:895-896
    // verbatim; in practice tmrOLPTimer (this function's only in-scope
    // caller) only calls it from inside `if(CommandBuffer->Count!=0)`, so
    // Count is never 0 here today -- preserved, not "fixed".
    ProcessPtr = (MyProcessBuffer *)CommandBuffer->Items[0];
    if (CommandBuffer->Count > 0)
    {
        ProcessPtr = (MyProcessBuffer *)CommandBuffer->Items[0];
        Process->Command = ProcessPtr->Command;
        if (Process->Command == "")
            Process->Command = "NULL";

        Process->SubCommand = ProcessPtr->SubCommand;
        if (Process->SubCommand == "")
            Process->SubCommand = "NULL";

        Process->AlarmID = ProcessPtr->AlarmID;
        if (Process->AlarmID == "")
            Process->AlarmID = "NULL";

        Process->AlarmMessage = ProcessPtr->AlarmMessage;
        if (Process->AlarmMessage == "")
            Process->AlarmMessage = "NULL";

        Process->TimeString = ProcessPtr->TimeString;
        if (Process->TimeString == "")
            Process->TimeString = "NULL";

        Process->ErrType = ProcessPtr->ErrType;
        str.sprintf("%s#%s#%s#%s#%s#%d\n", Process->Command.c_str(), Process->SubCommand.c_str(), Process->AlarmID.c_str(), Process->AlarmMessage.c_str(), Process->TimeString.c_str(), Process->ErrType);

        datestr.sprintf("d:\\%s_Log\\TCP_Data\\", HandlerType);
        MyForceDirectories(datestr);
        GetTimeInfo();   //Steven 20101105
        datestr.sprintf("d:\\%s_Log\\TCP_Data\\%04d%02d%02d.tcp", HandlerType, aSystemYear, aSystemMonth, aSystemDate);
        file = fopen(datestr.c_str(), "a+");
        if (file != NULL)
        {
            fputs(str.c_str(), file);
            fclose(file);
        }
        CommandBuffer->Clear();
    }
    delete Process;
}
//---------------------------------------------------------------------------
void TfAutomation::DoCommandBuffer(AnsiString Command, AnsiString SubCommand, AnsiString AlarmMessage, int ErrType, AnsiString AlarmID)
{
    // preserved golden quirk: `P1` is overwritten on every call with no
    // delete of the previous value, and the ProcessBuffer* items this
    // pushes into CommandBuffer are never delete'd anywhere in the in-scope
    // method set either (tmrOLPTimer's case 1->100 consumption path copies
    // *ProcessPtr by value into the static `Process` and never frees the
    // pointer) -- a genuine, faithfully-preserved golden memory leak. Golden
    // automation.cpp:937-947 verbatim; not "fixed" here.
    P1 = new MyProcessBuffer();   //Steven 20120102 : 改為全域變數
    P1->Command = Command;
    P1->SubCommand = SubCommand;
    P1->AlarmMessage = AlarmMessage;
    P1->ErrType = ErrType;
    P1->AlarmID = AlarmID;
    P1->TimeString = GetTimeInfo();
    CommandBuffer->Add(P1);
}
//---------------------------------------------------------------------------
void TfAutomation::ProcessBuffer1()
{
    // GATED (deferred future wave): golden body automation.cpp:1980-2045 --
    // the PP_DL_REQUEST binary file-receive sibling of the ProcessBuffer OLP
    // command-dispatch ladder (writes a received .zip via TCPstr byte-pair
    // decoding). See automation.h's own file-head note. Safe no-op stub:
    // this front's in-scope method set never calls ProcessBuffer1 (it has no
    // in-scope caller at all -- declared here purely for class-shape
    // fidelity with golden's header).
}
//---------------------------------------------------------------------------
void TfAutomation::ProcessBuffer(AnsiString /*Buffer*/, int /*iHandle*/)
{
    // GATED (deferred future wave): golden body automation.cpp:951-1979 --
    // the ~1030-line OLP command-dispatch ladder (~130 branch string
    // literals: PP_LOAD_INQUIRE, HTMLJSON_INQUIRE, ACT_TEMP_INQUIRE,
    // TRAY_INFO_INQUIRE, ... dozens more). See automation.h's own file-head
    // note. Safe no-op: does not parse/dispatch/reply. Both in-scope callers
    // (OLPServerClientRead, tmrOLPTimer) tolerate a no-op here -- neither's
    // OWN translated logic depends on this function actually doing anything
    // (it only affects whether an OLP reply is ever sent back to the host).
}
//---------------------------------------------------------------------------
void TfAutomation::SendReportRequest(AnsiString /*ReportID*/, bool /*Standard*/)
{
    // GATED (deferred future wave): golden body automation.cpp:2046-2131 --
    // sends a host EVENT/REPORT (ReportID "0001".."0007" branches into
    // GetMainStatus/GetProductivity/GetLoadCount/GetSortingCount/
    // GetSocketCount/GetHeadCount). See automation.h's own file-head note --
    // this function's dependency surface already looks fully ready
    // (SendClient/SendServer are both real here; its auto9045.h callees are
    // already translated), flagged as a low-risk pickup for the next wave,
    // but out of THIS front's assigned scope (bundled with ProcessBuffer/
    // ProcessBuffer1 per the task brief's explicit deferral).
}
//---------------------------------------------------------------------------
void TfAutomation::OLPClientRead(TObject * /*Sender*/, TCustomWinSocket * /*Socket*/)
{
    // preserved golden quirk: unlike OLPServerClientRead (which reads via its
    // OWN `Socket` event parameter -- needed there to disambiguate multiple
    // possible connections), this method reads via the class member
    // `OLPClient->Socket` instead of its own `Socket` parameter -- harmless
    // here since OLPClient only ever has ONE connection, but an asymmetric
    // translation choice already present in golden (automation.cpp:2132-
    // 2171 verbatim); preserved, not "fixed" to use the parameter.
    if (ReceiveString2.Length() > 1000)
    {
        ShowMSG(AnsiString().sprintf("OLPServerClientRead too long to delete, %s", ReceiveString2));
        ReceiveString2 = "";
    }
    //讀取訊息
    int StartPos = 0, EndPos = 0;
    //接收字串
    ReceiveString2 += OLPClient->Socket->ReceiveText();

    while (true)
    {
        //尋找STX
        StartPos = ReceiveString2.Pos(STX);
        //尋找ETX
        EndPos = ReceiveString2.Pos(ETX);

        if (bReceive2 == false && StartPos > 0 && EndPos > 0 && StartPos < EndPos)
        {
            TCPstr2 = ReceiveString2.SubString(StartPos, EndPos);
            ReceiveString2.Delete(1, EndPos);
            bReceive2 = true;
        }

        if (bReceive2)
        {
            CompareEvent(TCPstr2);
            bReceive2 = false;
        }
        else
        {
            break;
        }
    }

    SendInvalid = false;
}
//---------------------------------------------------------------------------
void TfAutomation::OLPClientConnect(TObject * /*Sender*/, TCustomWinSocket * /*Socket*/)
{
    OnLine2->Enabled = true;
    iClientReConnect = 0;   //Sam 20240226 : Client 重連次數
    if (CUSTOMER_CODE == CC_MTI || CUSTOMER_CODE == CC_PTI)   //Sam 20190429 : Add CC_PTI_NEWWORK
    {
        ShowOLPState(1);
    }
    OLPClientConnected = true;
}
//---------------------------------------------------------------------------
void TfAutomation::OLPClientDisconnect(TObject * /*Sender*/, TCustomWinSocket * /*Socket*/)
{
    OnLine2->Enabled = false;
    iClientReConnect = 0;   //Sam 20240226 : Client 重連次數
    OLPClientConnected = false;
}
//---------------------------------------------------------------------------
void TfAutomation::OLPClientError(TObject *Sender, TCustomWinSocket *Socket, TErrorEvent /*ErrorEvent*/, int &ErrorCode)
{
    ShowMSG("OLPClient Connect fail : " + GetErrorMsg(Sender, ErrorCode));
    if (ErrorCode == 10053)   // 網路線拔除
    {
        if (CUSTOMER_CODE == CC_MTI || CUSTOMER_CODE == CC_PTI)   //Sam 20190429 : Add CC_PTI_NEWWORK
        {
            ShowOLPState(2);
        }
    }
    else if (ErrorCode == 10061)   //連線失敗
    {
    }
    else
    {
    }

    ErrorCode = 0;
    OnLine2->Enabled = false;
    try
    {
        // ADAPTATION (vclcompat API gap, not a golden quirk): golden calls
        // `Socket->Close();` on the per-connection socket. This front's
        // write scope excludes vclcompat/ClientSocket.h, whose
        // TCustomWinSocket does not implement a Close() method -- only
        // Disconnect(int Port) (itself modeled on the SAME golden idiom used
        // by MyPLC/ModbusTCPClient.cpp's SocketError handler, see that
        // header's own SCOPED API SURFACE note). Substituting the nearest
        // available equivalent; flagged for a future wave to add a real
        // TCustomWinSocket::Close() to vclcompat/ClientSocket.h if this
        // becomes load-bearing.
        Socket->Disconnect(Socket->RemotePort);
    }
    catch (...)
    {
        W906Auto_LogClientSocketExceptionError(Sender, "OLPClient Error");
    }
}
//---------------------------------------------------------------------------
void TfAutomation::OLPClientConnecting(TObject * /*Sender*/, TCustomWinSocket * /*Socket*/)
{
    OnLine2->Enabled = true;
}
//---------------------------------------------------------------------------
void TfAutomation::FormDestroy(TObject * /*Sender*/)
{
    try
    {
        SaveRecord();   //Steven 20141229 : OLP的Log要存檔
        OLPClient->Close();
        OLPServer->Close();
        tmrOLP->Enabled = false;
        if (CommandBuffer->Count == 0)
            delete CommandBuffer;
        // preserved golden quirk: if CommandBuffer is NOT empty at teardown,
        // it is deliberately NOT deleted here (golden automation.cpp:2237-
        // 2238 verbatim) -- a genuine leak, not "fixed".

        CommandClientReplyBuffer->Clear();
        delete CommandClientReplyBuffer;

        //    delete Pbuffer;   //Steven 20120102 : 改為全域變數
    }
    catch (...)
    {
        MyDBIProcess("Exception", "TfAutomation::FormDestroy");
    }
    LogSoftwareOffTime("TfAutomation, FormDestroy");   //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
bool charcmp(char *buf1, char *buf2, int size)   //ChungHung 20120413 add
{
    for (int i = 0; i < size; i++)
    {
        if (buf1[i] != buf2[i])
            return false;
    }

    return true;
}
//---------------------------------------------------------------------------
int TfAutomation::GetEventStrResult(int num)   //Brian,20181025
{
    AnsiString str1, str2;
    if (num >= 0 && num < iMaxEvent)
    {
        str1 = MyEventResult[num].CommandString;
        str2 = MyEventResult[num].NumString;
        if (str1 == "PRODUCTION_REPLY" && str2 == "0003")
        {
            return 1;
        }

        if (str1 == "PRODUCTION_REQUEST" && str2 == "0007")
        {
            return 2;
        }

        if (str1 == "TEST_RESULT_REPLY" && str2 == "")
        {
            return 3;
        }
    }
    return 4;
}
//---------------------------------------------------------------------------
void TfAutomation::SckSendAlarmCode(AnsiString Text)   //ChungHung 20150518 add for SCK send Alarm Code
{
    SendOLPClient(Text);
    AnsiString S2;
    S2.sprintf("%s : %s", GetTimeInfo(), Text);
    ShowMSG(S2);
}
//---------------------------------------------------------------------------
void TfAutomation::SendOLPClient(AnsiString Text)
{
    if (OLPClientConnected == true)
    {
        OLPClient->Socket->SendText(Text);
    }
}
//---------------------------------------------------------------------------
void TfAutomation::ShowMSG(AnsiString asStr)
{
    if (brecordmsgLock == false)
    {
        brecordmsgLock = true;
        if (Memo->Lines->Count > 500)
        {
            SaveRecord();
            Memo->Lines->Clear();
        }
        Memo->Lines->Add(asStr);
        brecordmsgLock = false;
    }
    Memo->Lines->Add("");
}
//---------------------------------------------------------------------------
AnsiString TfAutomation::GetHTMLJSONDatas()
{
    AnsiString machineStatus = GetMachineStatus();
    AnsiString workOrder = GetWorkOrder();
    AnsiString sMainTemp = GetMainTemp();
    double temperatures[10];
    GetTemperatures(temperatures);

    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "machineStatus", machineStatus.c_str());
    cJSON_AddStringToObject(root, "workOrder", workOrder.c_str());
    cJSON *temps = cJSON_CreateObject();
    for (int i = 0; i < 10; i++)
    {
        // ADAPTATION: golden calls the real-VCL static method
        // `AnsiString::Format(...)` (with ARRAYOFCONST-wrapped args); this
        // vclcompat AnsiString has no such static member, only the free
        // function `Format(fmt, args...)` (vclcompat/SysUtils.h) -- same
        // effective printf-style formatting, ARRAYOFCONST((x)) reduces to
        // the bare arg per that header's own macro note.
        AnsiString sensor = Format("A%d", i + 1);
        AnsiString tempStr = Format("%.1f", temperatures[i]);
        cJSON_AddStringToObject(temps, sensor.c_str(), tempStr.c_str());
    }
    cJSON_AddItemToObject(root, "temperatures", temps);
    cJSON_AddStringToObject(root, "MainTemp", sMainTemp.c_str());

    char *jsonStr = cJSON_PrintUnformatted(root);
    AnsiString result = jsonStr;
    cJSON_free(jsonStr);
    cJSON_Delete(root);
    return result;
}
//---------------------------------------------------------------------------
AnsiString TfAutomation::GetMachineStatus()
{
    return fMain->palMainStatus->Caption;
}
//---------------------------------------------------------------------------
AnsiString TfAutomation::GetWorkOrder()
{
    return fMain->cbSetupFileName->Text;
}
//---------------------------------------------------------------------------
void TfAutomation::GetTemperatures(double temps[10])
{
    for (int i = 0; i < 10; i++)
    {
        temps[i] = i;
    }
}
//---------------------------------------------------------------------------
AnsiString TfAutomation::GetMainTemp()
{
    return fMain->edWorkTemperBase->Text;
}
//---------------------------------------------------------------------------

// AI(W906-Automation) 20260716: self-instantiate the singleton (no VCL
// .dpr auto-create-form list in this shim) -- same idiom already established
// by FormsFacade.cpp's fMain/fSortCT/fLotInfo/fOffSet/fSCKART.
TfAutomation *fAutomationEngine = new TfAutomation(NULL);
