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
//  Follow-up wave: W906-AutoPB (20260720) -- ProcessBuffer/ProcessBuffer1/
//  SendReportRequest, the 3 methods this wave's own scope note originally
//  deferred as safe no-op stubs, are now REAL (fully translated, ~1181 golden
//  lines) -- see automation.h's own file-head note (now "TRANSLATED" rather
//  than "EXPLICITLY DEFERRED") and each function's own AI(W906-AutoPB)
//  comment for the per-branch decision log.
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

// ---- MySleep: golden common.h:261 (REAL definition: acarry_shims.cpp:153,
// offline no-op). Redeclare the extern locally -- same reuse-not-duplicate
// precedent as auto9045.cpp's own Gate #2 (auto9045.cpp:130) / CCLink/
// MyCCLink.cpp / KYECFTP/FTPClient_Transfer.cpp / Interface/TesterTCP.cpp.
// AI(W906-AutoPB) 20260720: added for ProcessBuffer's REPORT_INQUIRE branch
// (golden :995 `MySleep(100);`).
extern void MySleep(DWORD dwMilliseconds);

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
        ProcessBuffer(TCPstr, iSocketHandle);   // REAL as of W906-AutoPB -- full OLP dispatch ladder //Sam 20200813 : 增加 Log debug //被動回傳
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
        SendReportRequest("0001", bStandard);   // REAL as of W906-AutoPB -- full OLP dispatch ladder
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

            ProcessBuffer(sSubTCPstr, iSocketHandle);   // REAL as of W906-AutoPB -- full OLP dispatch ladder //Sam 20200813 : 增加 Log debug //被動回傳

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
    // AI(W906-AutoPB-D5) 20260720: golden's `Text!=0` (automation.cpp:648-651)
    // compares against the STRING "0" -- BCB6 resolves the bare 0 through
    // AnsiString's int ctor. Under vclcompat, a bare `!=0` instead binds the
    // const char* overload (0 == NULL pointer == empty string), silently
    // changing the comparison to `!=""` -- the W906-AutoPB design's compile
    // experiment (design doc S2-B) proved this divergence. Spelling
    // `AnsiString(0)` forces the int ctor and restores golden's compare-with-
    // "0" semantics, same as the P18/D4 sites translated in the ProcessBuffer
    // wave. (The previous comment here claimed `!=0` already compared "0"
    // under vclcompat -- that was factually wrong and is corrected herewith.)
    if (edinputIP->Text != AnsiString(0))
        OLPClient->Address = edinputIP->Text;
    if (edinputport->Text != AnsiString(0))
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
        SendReportRequest(cbbOLPCommand->Text, 0);   // REAL as of W906-AutoPB -- full OLP dispatch ladder
    }
    else
    {
        SendReportRequest(cbbOLPCommand->Text);   // REAL as of W906-AutoPB -- full OLP dispatch ladder
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
// AI(W906-AutoPB) 20260720: real translation, golden automation.cpp:1980-2044
// (PP_DL_REQUEST binary file-receive sibling of ProcessBuffer's command-
// dispatch ladder). Preserved golden quirk (P12, design §5.1): reads the
// file-scope global `TCPstr`, NOT a parameter -- when reached via the
// tmrOLPTimer leftover-data pump (rather than freshly off the wire), TCPstr
// may be a stale prior frame at the moment this executes; not "fixed" here.
void TfAutomation::ProcessBuffer1()
{
    AnsiString Command, V_Total, Temp, ShowString, filename, filelength;
    AnsiString Data[4];
    int pos, ret = 0;
    FILE *fs;

    if (IsStartWord(TCPstr, STX))
    {
        ShowCharHex(TCPstr);
        ShowString = stx;
        pos = TCPstr.Pos('\x01');            //取得分隔符號位置
        Command = TCPstr.SubString(1, pos - 1);   //取得Command
        TCPstr.Delete(1, pos);                //刪除以擷取字串
        ShowString = ShowString + AddBlock(Command) + soh;
    }
    else
    {
        Command = "";
        return;
    }

    if (Command == "PP_DL_REQUEST")   //接收到Host傳送過來的壓縮檔
    {
        pos = TCPstr.Pos('\x01');
        V_Total = TCPstr.SubString(1, pos - 1);
        TCPstr.Delete(1, pos);   //取得資料筆數 並刪除已讀取字串
        pos = TCPstr.AnsiPos('\x01');
        sDLFileName = TCPstr.SubString(1, pos - 1);   //取得檔案名
        // preserved golden quirk (P12/design §2 C-2): if sDLFileName has no
        // ".zip" substring, Pos() returns 0 and Delete(0,4) is a documented
        // BCB6/vclcompat 1-based no-op (index<1 -> no removal) -- not "fixed".
        sDLFileName.Delete(sDLFileName.Pos(".zip"), 4);   //刪除附檔名
        TCPstr.Delete(1, pos);   //刪除已讀取資料
        pos = TCPstr.Pos('\x01');
        filelength = TCPstr.SubString(1, pos - 1);   //取得檔案大小
        TCPstr.Delete(1, pos);
        ShowString = ShowString + AddBlock(V_Total) + soh + AddBlock(sDLFileName + ".zip") + soh + AddBlock(filelength);

        int s;
        AnsiString ddd;
        filename = aDataPath + sDLFileName + ".zip";   //filename : 存放路徑
        fs = fopen(filename.c_str(), "wb");   //開啟檔案
        if (fs != NULL)   // 2009.07.30 , Joye
        {
            // preserved golden quirk (P11, design §5.1 -- "out-of-the-box
            // broken" hex-pair decoder): `ddd="0x"+c1+c2` builds a string like
            // "0x41", but atoi() does NOT parse a "0x" prefix (atoi stops at
            // the first non-digit -- here, literally the 'x' right after
            // the leading '0') -- so `s` is ALWAYS 0 regardless of the two
            // hex-digit characters that follow. Every byte this loop ever
            // fprintf's is therefore NUL (0x00): the .zip this writes out is
            // golden-broken from the factory (all-zero bytes), not a
            // translation defect. KYECFTP is the real-world file-transfer
            // path customers actually use; this PP_DL path is legacy/
            // unused. Not "fixed" here -- see design §5.1 P11.
            for (int i = 0; i < (TCPstr.Length() - 2); i += 2)   //寫入檔案
            {
                ddd = "0x" + AnsiString(TCPstr.c_str()[i]) + AnsiString(TCPstr.c_str()[i + 1]);
                s = atoi(ddd.c_str());
                fprintf(fs, "%1c", s);
            }
            fclose(fs);   //關檔
        }

        ShowString = ShowString + soh + AddBlock(TCPstr) + etx;
        ShowRecord(READ, ShowString, 0);

        ret = DoDLRequest(sDLFileName);
        if (ret == 0)
            Data[0] = "0";
        else
            Data[0] = "1";
        // preserved golden quirk (P13, design §5.1): 4th arg 0 -> Standard=
        // false -> routes through SendServer (the OLPClient path), the
        // OPPOSITE of every ProcessBuffer branch's bClient=true/SendClient --
        // not "fixed" here.
        CommandProcess("PP_DL_REPLY", 1, Data, 0);
        TCPstr = "";
        sDLFileName = "";
    }
    TCPstr = "";
}
//---------------------------------------------------------------------------
// AI(W906-AutoPB) 20260720: golden :949 `extern bool bLockByServer;` omitted
// here -- this TU already has a real DEFINITION of the same symbol at this
// file's own line ~48 (see that definition's own comment); redeclaring an
// extern in the same TU as its own definition is legal C++ but adds nothing,
// omitted for minimal diff. Kept for the record: golden line was
// `extern bool bLockByServer;` immediately above ProcessBuffer.
extern bool SoftStart;   // golden :950 -- dead declaration (design §5.1 P14):
// the ONLY reference to this symbol anywhere in ProcessBuffer is the
// commented-out `//SoftStart=true;` at golden :1572 (preserved below as a
// comment, never executed). Real symbol home: cmydef.h:223/cmydef.cpp:288
// (already visible via this file's own `#include "cmydef.h"`) -- this extern
// is a harmless duplicate redeclaration, kept verbatim for golden shape
// fidelity, not because anything here actually needs it.
//---------------------------------------------------------------------------
// AI(W906-AutoPB) 20260720: real translation, golden automation.cpp:951-1978
// (the OLP command-dispatch ladder: ~60 *_INQUIRE branches + ~70 *_REQUEST
// branches). See design doc DESIGN_automation_ProcessBuffer.md (W906-AutoPB)
// for the full per-branch decision log; golden bugs/quirks are preserved
// verbatim and flagged inline (P1-P18); the 3 PORT-ONLY UB guards (D1-D3) are
// each flagged where they diverge from golden's literal (but undefined)
// behavior.
void TfAutomation::ProcessBuffer(AnsiString Buffer, int iHandle)   //Sam 20200813 : 增加 Log debug
{
    int iType;
    bool bClient = true;

    // AI(W906-AutoPB) 20260720: golden `if(Buffer==NULL) return;` (:956) --
    // rewritten as `AnsiString(0)` per this wave's NULL/0 BCB6-fidelity rule
    // (design §2 definition B / §5.2 D4): vclcompat AnsiString's free-function
    // `operator==(const AnsiString&, const char*)` binds a literal NULL/0 to
    // the char* overload FIRST (standard conversion beats user conversion),
    // comparing against "" -- but real BCB6 AnsiString has only the member
    // `operator==(const AnsiString&)`, so NULL/0 there converts via
    // AnsiString(int) and compares against the STRING "0". Writing
    // AnsiString(0) explicitly reproduces the BCB6-correct comparison
    // (verified by an independent compile experiment, design §1.2-4).
    if (Buffer == AnsiString(0))   //如果Buffer是Null的話，就要跳開，不然會出現異常。
        return;

    AnsiString Command, V_Total, ShowString;
    AnsiString Data[40];        //Sam 20200415 : fix 32 Site
    int v_total;

    ShowCharHex(Buffer);

    if (IsStartWord(Buffer, STX))
    {
        ShowString = stx;
        Command = SplitDataBySoh(Buffer);
        ShowString = ShowString + AddBlock(Command);
    }
    else
    {
        Command = "";
        return;
    }

    V_Total = SplitDataBySoh(Buffer);
    ShowString = ShowString + soh + AddBlock(V_Total);
    v_total = atoi(V_Total.c_str());

    // AI(W906-AutoPB) 20260720: PORT-ONLY bounds guard (D1, design §2 C-4a) --
    // golden loops to the wire-supplied v_total unchecked against
    // `AnsiString Data[40]` (stack smash for v_total>40, UB). Guarded because
    // vclcompat AnsiString makes the same overrun a hard crash (non-trivial,
    // std::string-backed) where BCB6's stack layout might have "happened" to
    // survive it. Observable delta exists ONLY inside golden-UB input space
    // (hostile/corrupt frames); every legal OLP frame fits in 40 fields.
    for (int i = 0; i < v_total && i < 40; i++)
    {
        Data[i] = SplitDataBySoh(Buffer);
        ShowString = ShowString + soh + AddBlock(Data[i]);
    }
    ShowString = ShowString + etx;
    ShowRecord(READ, ShowString, iHandle);   //Sam 20200813 : 增加 Log debug

    if (Command.Pos("INQUIRE") != 0)   // preserved golden quirk (P1): SUBSTRING
                                        // match -- any Command containing
                                        // "INQUIRE" anywhere enters this ladder,
                                        // not just a trailing "_INQUIRE" suffix.
    {
        if (Command == "REPORT_INQUIRE")
        {
            Data[0] = 0;
            CommandProcess("REPORT_GRANT", 1, Data, bClient, iHandle);   //Sam 20200813 : 增加 Log debug
            MySleep(100);           //Steven 20110902 : 動作太快可能只會做其中一個,所以要Delay一下
            SendReportRequest(Data[2]);   // preserved golden quirk (P15): if
                                           // v_total<3 the wire never supplied
                                           // a 3rd field, so Data[2] is "" ->
                                           // SendReportRequest's unknown-ID path.
        }
        else
        {
            TCPstr = "";   // preserved golden quirk (P16): clears the file-scope
                            // global TCPstr on entry to the generic-INQUIRE path,
                            // independent of ProcessBuffer1's own use of TCPstr.
            int iInquire = 0;
            AnsiString CMD = Command;
            CMD.Delete(CMD.Length() - 6, 7);
            CMD += "GRANT";

            if (Command == "PP_LOAD_INQUIRE")
            {
                if (CheckCanChangeRealDummy())
                {
                    Data[0] = "0";
                }
                else
                {
                    Data[0] = "1";
                }
                iInquire = 1;
            }
            else if (Command == "HTMLJSON_INQUIRE")
            {
                Data[0] = GetHTMLJSONDatas();
                iInquire = 2;
            }
            else if (Command == "VERSION_INQUIRE")
            {
                Data[0] = GetSoftwareVersion();
                iInquire = 2;
            }
            else if (Command == "ACT_TEMP_INQUIRE")
            {
                for (int i = 0; i < 10; i++)
                    Data[i] = GetActTemp(i);
                Data[10] = "0";
                iInquire = 11;
            }
            else if (Command == "TRAY_INFO_INQUIRE")
            {
                iInquire = GetTrayForm(Data) + 1;
            }
            else if (Command == "HANDMODE_INQUIRE")
            {
                iInquire = GetHandMode(Data) + 1;
            }
            else if (Command == "PLATE_INFO_INQUIRE")
            {
                iInquire = GetPlateForm(Data) + 1;
            }
            else if (Command == "SETUP_FILE_NAME_INQUIRE")
            {
                Data[0] = GetSetUpFileName();
                iInquire = 2;
            }
            else if (Command == "JAM_COUNT_INQUIRE")
            {
                Data[0] = GetJamCount();
                iInquire = 2;
            }
            else if (Command == "CATEGORY_INQUIRE")
            {
                iInquire = GetCategory(Data) + 1;
            }
            else if (Command == "BINDEFINE_INQUIRE")       //Sam 20230803 : 新增OLP指令
            {
                iInquire = GetBindefine(Data) + 1;
            }
            else if (Command == "FIXTRAYDEFINE_INQUIRE")   //Sam 20230921 : 新增 FixTray 指令
            {
                iInquire = GetFixTrayDefine(Data) + 1;
            }
            else if (Command == "MAPPING_INQUIRE")
            {
                iInquire = GetMapping(Data) + 1;
            }
            else if (Command == "DUT_INQUIRE")
            {
                iInquire = GetDutOnOff(Data) + 1;
            }
            else if (Command == "SOAK_TIME_INQUIRE")
            {
                Data[0] = GetSoakTime();
                iInquire = 2;
            }
            else if (Command == "TEMPERATURE_INQUIRE")
            {
                Data[0] = GetTemperature();
                iInquire = 2;
            }
            else if (Command == "TEMPMODE_INQUIRE")
            {
                Data[0] = GetTempMode();
                iInquire = 2;
            }
            else if (Command == "CONNECTION_INQUIRE")
            {
                Data[0] = GetTesterConnect();
                iInquire = 2;
            }
            else if (Command == "TESTMODE_INQUIRE")
            {
                Data[0] = GetTesterMode();
                iInquire = 2;
            }
            else if (Command == "AlarmMode_INQUIRE")
            {
            }
            else if (Command == "AllSiteFail_INQUIRE")
            {
            }
            else if (Command == "ByHeadFail_INQUIRE")
            {
            }
            else if (Command == "ByBinAll_INQUIRE")
            {
            }
            else if (Command == "SetSiteYield_INQUIRE")
            {
            }
            else if (Command == "BinOverLimitSelec_INQUIRE")
            {
            }
            else if (Command == "BinOverLimitSet_INQUIRE")
            {
            }
            else if (Command == "BinOverCountSet_INQUIRE")
            {
            }
            else if (Command == "SetAutoHeight_INQUIRE")
            {
                Data[0] = GetAutoHeight(0);
                Data[1] = GetAutoHeight(1);
                iInquire = 3;
            }
            else if (Command == "SetContactOffset_INQUIRE")
            {
                Data[0] = GetContactOffset(0);
                Data[1] = GetContactOffset(1);
                iInquire = 3;
            }
            else if (Command == "SetContactTestMode_INQUIRE")
            {
                Data[0] = GetContactTestMode();
                iInquire = 2;
            }
            else if (Command == "SetSecondSpeed_INQUIRE")
            {
                Data[0] = GetSecondSpeed();
                iInquire = 2;
            }
            else if (Command == "SetContactWaitHeight_INQUIRE")
            {
                Data[0] = GetContactWaitHeight();
                iInquire = 2;
            }
            else if (Command == "SetDropHeight_INQUIRE")
            {
                // preserved golden quirk (P2, design §5.1): the SECOND
                // GetDropHeight(1) result is written to Data[0] again (a
                // literal typo in golden -- should be Data[1]) -- this
                // OVERWRITES GetDropHeight(0) rather than filling a second
                // field. Combined with the trailing Data[iInquire-1]="0"
                // append below, the reply's 2nd field ends up "0" (the
                // append) with the FIRST field carrying GetDropHeight(1),
                // and GetDropHeight(0)'s result is lost entirely. Not "fixed".
                Data[0] = GetDropHeight(0);
                Data[0] = GetDropHeight(1);
                iInquire = 3;
            }
            else if (Command == "SetReleaseWait_INQUIRE")
            {
                Data[0] = GetReleaseWait();
                iInquire = 2;
            }
            else if (Command == "SetShuttlePickOffset_INQUIRE")
            {
                // preserved golden quirk (P2): same double-write-to-Data[0]
                // pattern as SetDropHeight_INQUIRE above -- not "fixed".
                Data[0] = GetShuttlePickOffset(0);
                Data[0] = GetShuttlePickOffset(1);
                iInquire = 3;
            }
            else if (Command == "ShuttlePickHeight_INQUIRE")
            {
                // preserved golden quirk (P2): same pattern -- not "fixed".
                Data[0] = GetShuttlePickHeight(0);
                Data[0] = GetShuttlePickHeight(1);
                iInquire = 3;
            }
            else if (Command == "ShuttleReleaseHeight_INQUIRE")
            {
                // preserved golden quirk (P2): same pattern -- not "fixed".
                Data[0] = GetShuttleReleaseHeight(0);
                Data[0] = GetShuttleReleaseHeight(1);
                iInquire = 3;
            }
            else if (Command == "ArmTotalForce_INQUIRE ")   // preserved golden
                // quirk (P3, design §5.1): literal TRAILING SPACE in this string
                // literal -- no real incoming "ArmTotalForce_INQUIRE" command
                // (without the trailing space) can ever match this branch; it
                // always falls through to the unknown-INQUIRE path below. Kept
                // exactly as golden wrote it (including the space), not "fixed".
            {
                // preserved golden quirk (P2, dead code due to P3 above): same
                // double-write-to-Data[0] pattern -- not "fixed".
                Data[0] = GetArmTotalForce(0);
                Data[0] = GetArmTotalForce(1);
                iInquire = 3;
            }
            else if (Command == "ClinderForce_INQUIRE")
            {
                // preserved golden quirk (P2): same pattern -- not "fixed".
                Data[0] = GetClinderForce(0);
                Data[0] = GetClinderForce(1);
                iInquire = 3;
            }
            else if (Command == "ForcePerDevice_INQUIRE")
            {
                // preserved golden quirk (P2): same pattern -- not "fixed".
                Data[0] = GetForcePerDevice(0);
                Data[0] = GetForcePerDevice(1);
                iInquire = 3;
            }
            else if (Command == "SetNoPerPin_INQUIRE")
            {
                Data[0] = GetSetNoPerPin();
                iInquire = 2;
            }
            else if (Command == "SetForcePerPin_INQUIRE")
            {
                Data[0] = GetForcePerPin();
                iInquire = 2;
            }
            else if (Command == "SetContactForcen_INQUIRE")
            {
                Data[0] = GetContactForcen();
                iInquire = 2;
            }
            else if (Command == "GetStartPos_INQUIRE")
            {
                // preserved golden quirk (P2): same pattern -- not "fixed".
                Data[0] = GetStartPos(0);
                Data[0] = GetStartPos(1);
                iInquire = 3;
            }
            else if (Command == "GetDivision_INQUIRE")
            {
                // preserved golden quirk (P2): same pattern -- not "fixed".
                Data[0] = GetDivision(0);
                Data[0] = GetDivision(1);
                iInquire = 3;
            }
            else if (Command == "GetDimemsion_INQUIRE")
            {
                iInquire = GetDimemsion(Data) + 1;
            }
            else if (Command == "GetZThickness_INQUIRE")
            {
                Data[0] = GetZThickness();
                iInquire = 2;
            }
            else if (Command == "SetTrayType_INQUIRE")
            {
                Data[0] = GetTrayType();
                iInquire = 2;
            }
            else if (Command == "GetPitch_INQUIRE")
            {
                // preserved golden quirk (P4, design §5.1): fills Data[0..1]
                // but iInquire=2 -- the trailing Data[iInquire-1]="0" append
                // below (Data[1]="0") OVERWRITES GetPitch(1)'s result. Not "fixed".
                Data[0] = GetPitch(0);
                Data[1] = GetPitch(1);
                iInquire = 2;
            }
            // preserved golden quirk (P6, design §5.1): this exact commented-
            // out block (superseded by the real GetIP_INQUIRE/GetPort_INQUIRE/
            // GetCusCode_INQUIRE branches further below, near golden :1468) is
            // kept as a comment for shape fidelity -- not resurrected.
            //            else if(Command=="GetIP_INQUIRE")
            //            {
            //                Data[0]=GetIP(Data);
            //                iInquire=2;
            //            }
            //            else if(Command=="GetPort_INQUIRE")
            //            {
            //                Data[0]=GetPort(Data);
            //                iInquire=2;
            //            }
            //            else if(Command=="GetCusCode_INQUIRE")
            //            {
            //                Data[0]=GetCusCode(Data);
            //                iInquire=2;
            //            }
            //Sam 20190802 : Add New OLP For HT9045
            //==>
            else if (Command == "LowYield_INQUIRE")
            {
                iInquire = GetLowYield(Data) + 1;
            }
            else if (Command == "ByArmPerSiteDiffYield_INQUIRE")
            {
                iInquire = GetByArmPerSiteDiffYield(Data) + 1;
            }
            else if (Command == "ConsecutiveFailureAlarmByHead_INQUIRE")
            {
                iInquire = GetConsecutiveFailureAlarmByHead(Data) + 1;
            }
            else if (Command == "ConsecutiveFailureAlarmBySocket_INQUIRE")
            {
                iInquire = GetConsecutiveFailureAlarmBySocket(Data) + 1;
            }
            else if (Command == "AllSiteFailFor9045_INQUIRE")
            {
                iInquire = GetAllSiteFailFor9045(Data) + 1;
            }
            else if (Command == "TrayFormTypeInfo_INQUIRE")
            {
                iType = atoi(Data[0].c_str());
                iInquire = GetTrayFormTypeInfo(Data, iType) + 1;
            }
            else if (Command == "TrayFormTypeThickness_INQUIRE")
            {
                iType = atoi(Data[0].c_str());
                iInquire = GetTrayFormTypeThickness(Data, iType) + 1;
            }
            else if (Command == "TrayFormTypePickUp_INQUIRE")
            {
                iType = atoi(Data[0].c_str());
                iInquire = GetTrayFormTypePickUp(Data, iType) + 1;
            }
            else if (Command == "TrayFormTypeName_INQUIRE")
            {
                iType = atoi(Data[0].c_str());
                iInquire = GetTrayFormTypeName(Data, iType) + 1;
            }
            else if (Command == "ContactModeFor9045_INQUIRE")
            {
                iInquire = GetContactModeFor9045(Data) + 1;
            }
            else if (Command == "ContactVacuumMode_INQUIRE")
            {
                iInquire = GetContactVacuumMode(Data) + 1;
            }
            else if (Command == "ContactDropWait_INQUIRE")
            {
                iInquire = GetContactDropWait(Data) + 1;
            }
            else if (Command == "SlowContactSpeed_INQUIRE")
            {
                iInquire = GetSlowContactSpeed(Data) + 1;
            }
            else if (Command == "ShuttleWaitOutSideCamber_INQUIRE")
            {
                iInquire = GetShuttleWaitOutSideCamber(Data) + 1;
            }
            else if (Command == "PickShuttleDeviceAfterTested_INQUIRE")
            {
                iInquire = GetPickShuttleDeviceAfterTested(Data) + 1;
            }
            else if (Command == "PickShuttleDeviceThenWaitOnShuttle_INQUIRE")
            {
                iInquire = GetPickShuttleDeviceThenWaitOnShuttle(Data) + 1;
            }
            else if (Command == "PickShuttleDeviceTogetherFor32SiteN_INQUIRE")
            {
                iInquire = GetPickShuttleDeviceTogetherFor32SiteN(Data) + 1;
            }
            else if (Command == "IndexArm1Height_INQUIRE")
            {
                iInquire = GetIndexArm1Height(Data) + 1;
            }
            else if (Command == "IndexArm2Height_INQUIRE")
            {
                iInquire = GetIndexArm2Height(Data) + 1;
            }
            else if (Command == "TestICCheckMode_INQUIRE")
            {
                iInquire = GetTestICCheckMode(Data) + 1;
            }
            else if (Command == "AboveSocket_INQUIRE")
            {
                iInquire = GetAboveSocket(Data) + 1;
            }
            else if (Command == "ContactForceInfo_INQUIRE")
            {
                iInquire = GetContactForceInfo(Data) + 1;
            }
            else if (Command == "HotPlateFormName_INQUIRE")
            {
                iInquire = GetHotPlateFormName(Data) + 1;
            }
            else if (Command == "HotPlate1_INQUIRE")
            {
                iInquire = GetHotPlate1(Data) + 1;
            }
            else if (Command == "HotPlate2_INQUIRE")
            {
                iInquire = GetHotPlate2(Data) + 1;
            }
            else if (Command == "InterfaceType_INQUIRE")
            {
                iInquire = GetInterfaceType(Data) + 1;
            }
            else if (Command == "TesterInitialMaximumTest_INQUIRE")
            {
                iInquire = GetTesterInitialMaximumTest(Data) + 1;
            }
            else if (Command == "TesterMaximumTest_INQUIRE")
            {
                iInquire = GetTesterMaximumTest(Data) + 1;
            }
            else if (Command == "TesterDummyTest_INQUIRE")
            {
                iInquire = GetTesterDummyTest(Data) + 1;
            }
            else if (Command == "TesterStartDelay_INQUIRE")
            {
                iInquire = GetTesterStartDelay(Data) + 1;
            }
            else if (Command == "HotSoakTime_INQUIRE")
            {
                iInquire = GetHotSoakTime(Data) + 1;
            }
            else if (Command == "HotJamSoakTime_INQUIRE")
            {
                iInquire = GetHotJamSoakTime(Data) + 1;
            }
            else if (Command == "HotInitialWaitTime_INQUIRE")
            {
                iInquire = GetHotInitialWaitTime(Data) + 1;
            }
            else if (Command == "HotInitialStart1Time_INQUIRE")
            {
                iInquire = GetHotInitialStart1Time(Data) + 1;
            }
            else if (Command == "HotShuttleSoakTime_INQUIRE")
            {
                iInquire = GetHotShuttleSoakTime(Data) + 1;
            }
            else if (Command == "ChamberCoolingTemp_INQUIRE")
            {
                iInquire = GetChamberCoolingTemp(Data) + 1;
            }
            else if (Command == "HotIndexSoakTime_INQUIRE")
            {
                iInquire = GetHotIndexSoakTime(Data) + 1;
            }
            else if (Command == "HotOpenShortTime_INQUIRE")
            {
                iInquire = GetHotOpenShortTime(Data) + 1;
            }
            else if (Command == "HotZ1Down_INQUIRE")
            {
                iInquire = GetHotZ1Down(Data) + 1;
            }
            else if (Command == "HotShuttleSoakMode_INQUIRE")
            {
                iInquire = GetHotShuttleSoakMode(Data) + 1;
            }
            else if (Command == "MachineTempMode_INQUIRE")
            {
                iInquire = GetMachineTempMode(Data) + 1;
            }
            else if (Command == "AmbientCheck_INQUIRE")
            {
                iInquire = GetAmbientCheck(Data) + 1;
            }
            else if (Command == "AmbientCheckTemp_INQUIRE")
            {
                iInquire = GetAmbientCheckTemp(Data) + 1;
            }
            else if (Command == "TempeartureOffset_INQUIRE")
            {
                iInquire = GetTemperatureOffset(Data) + 1;
            }
            else if (Command == "ContactCountForOffsetPeriod_INQUIRE")
            {
                iInquire = GetContactCountForOffsetPeriod(Data) + 1;
            }
            else if (Command == "ContactCountForCoolDown_INQUIRE")
            {
                iInquire = GetContactCountForCoolDown(Data) + 1;
            }
            else if (Command == "InterfaceDIOInfo_INQUIRE")
            {
                iInquire = GetInterfaceDIOInfo(Data) + 1;
            }
            else if (Command == "InterfaceGPIBInfo_INQUIRE")
            {
                iInquire = GetInterfaceGPIBInfo(Data) + 1;
            }
            else if (Command == "InterfaceRS232Info_INQUIRE")
            {
                iInquire = GetInterfaceRS232Info(Data) + 1;
            }
            else if (Command == "LotInfo_INQUIRE")
            {
                iInquire = GetLotInfo(Data) + 1;
            }
            else if (Command == "GetIP_INQUIRE")
            {
                iInquire = GetIP(Data) + 1;
            }
            else if (Command == "GetPort_INQUIRE")
            {
                iInquire = GetPort(Data) + 1;
            }
            else if (Command == "GetCusCode_INQUIRE")
            {
                iInquire = GetCusCode(Data) + 1;
            }
            else if (Command == "StartMode_INQUIRE")   //Sam 20221212 : 新增 StartMode 指令
            {
                iInquire = GetStartMode(Data) + 1;
            }
            else if (Command == "LOTTOTAL_INQUIRE")    //Sam 20230803 : 新增OLP指令
            {
                iInquire = GetLotTotal(Data) + 1;
            }

            // AI(W906-AutoPB) 20260720: PORT-ONLY guard (D2, design §2 C-4b) --
            // golden `if(iInquire!=1) Data[iInquire-1]="0";` writes Data[-1]
            // (UB) whenever iInquire is still 0: the 8 empty *_INQUIRE
            // branches above (AlarmMode_INQUIRE etc.), the unreachable
            // ArmTotalForce_INQUIRE-with-trailing-space branch (P3), and any
            // genuinely unknown Command all leave iInquire at its initial 0.
            // BCB6 happened to hit an adjacent stack AnsiString there; under
            // vclcompat's non-trivial AnsiString this is a near-certain
            // crash. Guarded to the most conservative concrete outcome for
            // this input space: skip the append entirely, so CommandProcess
            // below is called with iInquire==0 (V_TOTAL="0", zero data
            // fields) -- a legal, if empty, GRANT frame. Delta exists ONLY
            // inside this golden-UB input space (design §5.2 D2).
            if (iInquire != 1 && iInquire >= 1)
                Data[iInquire - 1] = "0";
            CommandProcess(CMD, iInquire, Data, bClient, iHandle);
        }
    }
    else if (Command == "ON_LINE_REQUEST")  // 2008/05/23
    {
        DoOnLineReply(Data);
        if (CUSTOMER_CODE == CC_MTI || CUSTOMER_CODE == CC_PTI)  //Sam 20190429 : Add CC_PTI_NEWWORK
        {
            CommandProcess("ON_LINE_REPLY", 5, Data, bClient, iHandle);//for sback
        }
        else
        {
            CommandProcess("ON_LINE_REPLY", 4, Data, bClient, iHandle);
        }
    }
    else if (Command == "INITIATE_REQUEST")  //2008/05/23
    {
        SYSTEMTIME SysTime;
        SysTime.wYear   = atoi(Data[0].SubString(1,  4).c_str());
        SysTime.wMonth  = atoi(Data[0].SubString(5,  2).c_str());
        SysTime.wDay    = atoi(Data[0].SubString(7,  2).c_str());
        SysTime.wHour   = atoi(Data[0].SubString(9,  2).c_str());
        SysTime.wMinute = atoi(Data[0].SubString(11, 2).c_str());
        SysTime.wSecond = atoi(Data[0].SubString(13, 2).c_str());
        SysTime.wMilliseconds = 0;
        SetLocalTime(&SysTime);

        Data[0] = 0;

        if (CUSTOMER_CODE == CC_MTI || CUSTOMER_CODE == CC_PTI)                      //2016.03.24 , Brian   Add CC_PTI_NEWWORK
        {
           fMain->Home("TfAutomation::ProcessBuffer");
        }
        CommandProcess("INITIATE_REPLY", 1, Data, bClient, iHandle);
        //Sam 20190429 : Add CC_PTI_NEWWORK
        if (CUSTOMER_CODE == CC_MTI || CUSTOMER_CODE == CC_PTI)
        {
            OLPServer->Close();
            OnLine->Enabled = false;

            ShowOLPState(0);
            btnConnect->Enabled = true;

            // AI(W906-AutoPB) 20260720: NULL/0 BCB6-fidelity rule (design §2
            // definition B / §5.2 D4/P18) applied to these two NEW comparisons
            // -- same rationale as the Buffer==AnsiString(0) guard at entry.
            if (edinputIP->Text != AnsiString(0))
                OLPClient->Address = edinputIP->Text;
            if (edinputport->Text != AnsiString(0))
                OLPClient->Port = edinputport->Text.ToInt();
            try
            {
                btnConnect->Enabled = false;
                OLPServer->Open();
                OLPServer->Active = true;
                ShowOLPState(1);//Hsiong 2015.04.16 Add CC_MTI_NEWWORK
            }
            catch (...)
            {
                MyDBIProcess("Exception", "TfAutomation::ProcessBuffer");
            }
        }
    }
    else if (Command == "PAUSE_REQUEST")
    {
        bLockByServer = true;
        SoftStop = true;

        Data[0] = 0;
        CommandProcess("PAUSE_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "RESUME_REQUEST")
    {
        bLockByServer = false;
        // preserved golden quirk (P8, design §5.1): RESUME does NOT clear
        // SoftStop (only PAUSE_REQUEST's bLockByServer is undone) -- not "fixed".

        Data[0] = 0;
        CommandProcess("RESUME_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "START_REQUEST")
    {
        if (fMain->palMainStatus->Caption == "HALT" && SystemStart == false)         //Sam 20240125 : 只能在閒置時啟動
        {
            MyDBIProcess("Message", "Automation Command Start succeed!!");
            fMain->Start("TfAutomation::ProcessBuffer");
            //SoftStart=true;
        }
        else
        {
            MyDBIProcess("Message", "Automation Command Start fail!!");
        }
        Data[0] = 0;
        CommandProcess("START_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "PAUSE_REQUEST")
    // preserved golden quirk (P7, design §5.1): a SECOND, unreachable
    // `else if(Command=="PAUSE_REQUEST")` -- the first PAUSE_REQUEST branch
    // (above, right after ON_LINE_REQUEST) always matches first, so this
    // entire block is dead code. It ALSO replies with "START_REPLY" (not
    // "PAUSE_REPLY"), which would be wrong even if it were ever reached.
    // Not "fixed" -- kept verbatim for golden shape fidelity.
    {
        if (SystemStart == true)                                                   //Sam 20240125 : 只能在閒置時啟動
        {
            MyDBIProcess("Message", "Automation Command PAUSE succeed!!");
            fMain->Pause("TfAutomation::ProcessBuffer");
        }
        else
        {
            MyDBIProcess("Message", "Automation Command PAUSE fail!!");
        }
        Data[0] = 0;
        CommandProcess("START_REPLY", 1, Data, bClient, iHandle);
    }
  #ifdef DEBUG_DUTONOFF
    else if (Command == "CLEANOUT_REQUEST")
    {
        iCleanOut = 1;
        bCleanOut = true;
        Data[0] = 0;
//        CommandProcess("CLEANOUT_REPLY",1,Data);
    }
    else if (Command == "HOMEANDSTART_REQUEST")
    {
        DoHomeAndStart();

        Data[0] = 0;
        CommandProcess("CLEANOUT_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "ONECYCLE_REQUEST")
    {
        DoOneCycle();
        Data[0] = 0;
        bOneCycle = true;

//        CommandProcess("ONECYCLE_REPLY",1,Data);
    }
    #endif
    else if (Command == "CLEAR_REPORT_REQUEST")
    {
        int iSwitchCase = 0;
        for (int i = 0; i < v_total; i++)
        {
            // preserved golden quirk (P9, design §5.1): reads Data[0] on EVERY
            // iteration of this loop, never Data[i] -- so only the first
            // supplied report-code field is ever consulted, v_total-1 times
            // over (with no effect beyond the first pass, since
            // DoClearReportRequest is presumably idempotent per code). Not "fixed".
            iSwitchCase = atoi(Data[0].c_str());
            DoClearReportRequest(iSwitchCase);
        }

        Data[0] = 0;
        CommandProcess("CLEAR_REPORT_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "PP_UL_REQUEST")                                           //把工作檔送給HOST
    {
        DoULRequest(Data);
        CommandProcess("PP_UL_REPLY", 4, Data, bClient, iHandle);
    }
    else if (Command == "PP_DL_REQUEST")                                           //收到檔案並解壓縮
    {
        ProcessBuffer1();
    }
    else if (Command == "CATEGORY_REQUEST")
    {
        NewRecordProcess("", "CATEGORY_REQUEST", ShowString);                     //Sam 20230803 : 客戶設定 Bin 要特別紀錄
        Data[0] = SetCategory(Data);
        if (atoi(Data[0].c_str()) >= 3)                                            //Sam 20230921 : Bin 設定錯誤不能啟動
            LastSet.OLPSetBinErr[0] = atoi(Data[0].c_str());
        else
            LastSet.OLPSetBinErr[0] = 0;

        CommandProcess("CATEGORY_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "BINDEFINE_REQUEST")                                       //Sam 20230803 : 新增OLP指令
    {
        NewRecordProcess("", "BINDEFINE_REQUEST", ShowString);                    //Sam 20230803 : 客戶設定 Bin 要特別紀錄
        Data[0] = SetBinDefine(Data);
        if (atoi(Data[0].c_str()) >= 3)                                            //Sam 20230921 : Bin 設定錯誤不能啟動
            LastSet.OLPSetBinErr[1] = atoi(Data[0].c_str());
        else
            LastSet.OLPSetBinErr[1] = 0;

        CommandProcess("BINDEFINE_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "FIXTRAYDEFINE_REQUEST")                                   //Sam 20230921 : 新增 FixTray 指令
    {
        NewRecordProcess("", "FIXTRAYDEFINE_REQUEST", ShowString);
        Data[0] = SetFixTrayDefine(Data);
        if (atoi(Data[0].c_str()) >= 3)    //Sam 20230921 : Bin 設定錯誤不能啟動
            LastSet.OLPSetBinErr[2] = atoi(Data[0].c_str());
        else
            LastSet.OLPSetBinErr[2] = 0;

        CommandProcess("FIXTRAYDEFINE_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "MAPPING_REQUEST")
    {
        if (CheckNeedCleanOut() == false)
        {
            Data[0] = AnsiString(SetMapping(Data));
        }
        else
        {
            Data[0] = "1";
        }
        CommandProcess("MAPPING_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "DUT_REQUEST")
    {
        if (CheckNeedCleanOut() == false)
        {
            Data[0] = AnsiString(SetDutOnOff(Data));
        }
        else
        {
            Data[0] = "1";
        }
        CommandProcess("DUT_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "SOAK_TIME_REQUEST")
    {
        if (CheckNeedCleanOut() == false)
        {
            SetSoakTime(Data);
            Data[0] = "0";
        }
        else
        {
            Data[0] = "1";
        }
        CommandProcess("SOAK_TIME_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "TEMPERATURE_REQUEST")
    {
        if (CheckNeedCleanOut() == false)
        {
            SetTemperature(Data);
            Data[0] = "0";
        }
        else
        {
            Data[0] = "1";
        }
        CommandProcess("TEMPERATURE_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "TEMPMODE_REQUEST")
    {
        if (CheckNeedCleanOut() == false)
        {
            SetTempMode(Data);
            Data[0] = "0";
        }
        else
        {
            Data[0] = "1";
        }
        CommandProcess("TEMPMODE_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "CONNECTION_REQUEST")
    {
        if (CheckNeedCleanOut() == false)
        {
            Data[0] = SetTesterConnect(Data);
        }
        else
        {
            Data[0] = "1";
        }
        CommandProcess("CONNECTION_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "TESTMODE_REQUEST")
    {
        if (CheckNeedCleanOut() == false)
        {
            Data[0] = SetTesterMode(Data);
        }
        else
        {
            Data[0] = "1";
        }
        CommandProcess("TESTMODE_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "SETUP_FILE_NAME_REQUEST")
    {
        if (CheckNeedCleanOut() == false)
        {
            Data[0] = SetSetUpFileName(Data);
        }
        else
        {
            Data[0] = "1";
        }
        CommandProcess("SETUP_FILE_NAME_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "AlarmMode_REQUEST")
    {
    }
    else if (Command == "AllSiteFail_REQUEST")
    {
    }
    else if (Command == "ByHeadFail_REQUEST")
    {
    }
    else if (Command == "ByBinAll_REQUEST")
    {
    }
    else if (Command == "SetSiteYield_REQUEST")
    {
    }
    else if (Command == "BinOverLimitSelec_REQUEST")
    {
    }
    else if (Command == "BinOverLimitSet_REQUEST")
    {
    }
    else if (Command == "BinOverCountSet_REQUEST")
    {
    }
    else if (Command == "SetContactTestMode_REQUEST")
    {
    }
    else if (Command == "SetDropHeight_REQUEST")
    {
    }
    else if (Command == "SetReleaseWait_REQUEST")
    {
    }
    else if (Command == "LowYield_REQUEST")
    {
        Data[0] = SetLowYield(Data);
        CommandProcess("LowYield_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "ByArmPerSiteDiffYield_REQUEST")
    {
        Data[0] = SetByArmPerSiteDiffYield(Data);
        CommandProcess("ByArmPerSiteDiffYield_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "ConsecutiveFailureAlarmByHead_REQUEST")
    {
        Data[0] = SetConsecutiveFailureAlarmByHead(Data);
        CommandProcess("ConsecutiveFailureAlarmByHead_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "ConsecutiveFailureAlarmBySocket_REQUEST")
    {
        Data[0] = SetConsecutiveFailureAlarmBySocket(Data);
        CommandProcess("ConsecutiveFailureAlarmBySocket_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "AllSiteFailFor9045_REQUEST")
    {
        Data[0] = SetAllSiteFailFor9045(Data);
        CommandProcess("AllSiteFailFor9045_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "ContactModeFor9045_REQUEST")
    {
        Data[0] = SetContactModeFor9045(Data);
        CommandProcess("ContactModeFor9045_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "ContactVacuumMode_REQUEST")
    {
        Data[0] = SetContactVacuumMode(Data);
        CommandProcess("ContactVacuumMode_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "ContactDropWait_REQUEST")
    {
        Data[0] = SetContactDropWait(Data);
        CommandProcess("ContactDropWait_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "SlowContactSpeed_REQUEST")
    {
        Data[0] = SetSlowContactSpeed(Data);
        CommandProcess("SlowContactSpeed_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "ShuttleWaitOutSideCamber_REQUEST")
    {
        Data[0] = SetShuttleWaitOutSideCamber(Data);
        CommandProcess("ShuttleWaitOutSideCamber_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "PickShuttleDeviceAfterTested_REQUEST")
    {
        Data[0] = SetPickShuttleDeviceAfterTested(Data);
        CommandProcess("PickShuttleDeviceAfterTested_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "PickShuttleDeviceThenWaitOnShuttle_REQUEST")
    {
        Data[0] = SetPickShuttleDeviceThenWaitOnShuttle(Data);
        CommandProcess("PickShuttleDeviceThenWaitOnShuttle_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "PickShuttleDeviceTogetherFor32SiteN_REQUEST")
    {
        Data[0] = SetPickShuttleDeviceTogetherFor32SiteN(Data);
        CommandProcess("PickShuttleDeviceTogetherFor32SiteN_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "IndexArm1Height_REQUEST")
    {
        Data[0] = SetIndexArm1Height(Data);
        CommandProcess("IndexArm1Height_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "IndexArm2Height_REQUEST")
    {
        Data[0] = SetIndexArm2Height(Data);
        CommandProcess("IndexArm2Height_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "TestICCheckMode_REQUEST")
    {
        Data[0] = SetTestICCheckMode(Data);
        CommandProcess("TestICCheckMode_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "AboveSocket_REQUEST")
    {
        Data[0] = SetAboveSocket(Data);
        CommandProcess("AboveSocket_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "HotPlate1_REQUEST")
    {
        Data[0] = SetHotPlate1(Data);
        CommandProcess("HotPlate1_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "HotPlate2_REQUEST")
    {
        Data[0] = SetHotPlate2(Data);
        CommandProcess("HotPlate2_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "TesterInitialMaximumTest_REQUEST")
    {
        Data[0] = SetTesterInitialMaximumTest(Data);
        CommandProcess("TesterInitialMaximumTest_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "TesterMaximumTest_REQUEST")
    {
        Data[0] = SetTesterMaximumTest(Data);
        CommandProcess("TesterMaximumTest_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "TesterDummyTest_REQUEST")
    {
        Data[0] = SetTesterDummyTest(Data);
        CommandProcess("TesterDummyTest_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "TesterStartDelay_REQUEST")
    {
        Data[0] = SetTesterStartDelay(Data);
        CommandProcess("TesterStartDelay_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "HotZ1Down_REQUEST")
    {
        Data[0] = SetHotZ1Down(Data);
        CommandProcess("HotZ1Down_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "HotShuttleSoakMode_REQUEST")
    {
        Data[0] = SetHotShuttleSoakMode(Data);
        CommandProcess("HotShuttleSoakMode_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "AmbientCheck_REQUEST")
    {
        Data[0] = SetAmbientCheck(Data);
        CommandProcess("AmbientCheck_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "AmbientCheckTemp_REQUEST")
    {
        Data[0] = SetAmbientCheckTemp(Data);
        CommandProcess("AmbientCheckTemp_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "TempeartureOffset_REQUEST")
    {
        Data[0] = SetTemperatureOffset(Data);
        CommandProcess("TempeartureOffset_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "ContactCountForOffsetPeriod_REQUEST")
    {
        Data[0] = SetContactCountForOffsetPeriod(Data);
        CommandProcess("ContactCountForOffsetPeriod_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "ContactCountForCoolDown_REQUEST")
    {
        Data[0] = SetContactCountForCoolDown(Data);
        CommandProcess("ContactCountForCoolDown_REPLY", 1, Data, bClient, iHandle);
    }
    else if (Command == "LotInfo_REQUEST")
    {
        if (CUSTOMER_CODE == CC_Greatek && CosFunction.bOEEFunction)               //AI(JimmyChiu) 20260515: F899-008 §6.2 超豐在 OEE 開啟才走內嵌 OEE Start Lot
        {
            AnsiString asErrorMsg = "";
            Data[0] = SetLotInfoGreatekOEE(Data, asErrorMsg);
            Data[1] = asErrorMsg;
            if (Data[0] == "0")
                CommandProcess("LotInfo_REPLY", 1, Data, bClient, iHandle);
            else
                CommandProcess("LotInfo_REPLY", 2, Data, bClient, iHandle);
        }
        else
        {
            Data[0] = SetLotInfo(Data);
            CommandProcess("LotInfo_REPLY", 1, Data, bClient, iHandle);
        }
    }
    else if (Command == "StartMode_REQUEST")   //Sam 20221212 : 新增 StartMode 指令
    {
        Data[0] = SetStartMode(Data);
        CommandProcess("StartMode_REPLY", 1, Data, bClient, iHandle);
    }
}
//---------------------------------------------------------------------------
// AI(W906-AutoPB) 20260720: real translation, golden automation.cpp:2046-2130
// (host EVENT/REPORT sender -- ReportID "0001".."0007").
void TfAutomation::SendReportRequest(AnsiString ReportID, bool Standard)
{
    // AI(W906-AutoPB) 20260720: PORT-ONLY init guard (D3, design §2 C-4c) --
    // golden `int iTotal, i;` leaves iTotal UNINITIALIZED (:2048); for any
    // ReportID outside "0001".."0007" none of the branches below ever assigns
    // it, so golden reads garbage there (UB: unbounded loop count at :2112).
    // vclcompat AnsiString under that garbage loop count is a near-certain
    // crash (out-of-bounds Data[i] read/heap corruption), not merely "wrong
    // reply" the way BCB6's incidental stack layout might have gotten away
    // with -- so this is guarded to the most conservative concrete value: an
    // empty (SV_TOTAL="0", zero data fields) reply for any unknown ReportID.
    // Delta exists ONLY inside golden-UB input space (see design §5.2 D3).
    int iTotal = 0, i;
    AnsiString CMD = "REPORT_REQUEST", SV_TOTAL;
    AnsiString V_TOTAL = 2;
    AnsiString HEAD, head, SendString, S2, cmd, v_total, sv_total;   //,ReportID;
    AnsiString Data[100];
    AnsiString R_TIME, r_time, S;

    R_TIME = GetTimeInfo();
    r_time = AddBlock(R_TIME);

    cmd = AddBlock(CMD);
    v_total = AddBlock(V_TOTAL);

    HEAD = STX + CMD + SOH + V_TOTAL + SOH + R_TIME + SOH + ReportID + SOH;
    head = stx + cmd + soh + v_total + soh + r_time + soh + AddBlock(ReportID) + soh;

    if (ReportID == "0001")
    {
        iTotal = 1;
        S = GetMainStatus();
        Data[0] = S;
    }
    else if (ReportID == "0002")   //MTBF與MUBF
    {
        if (CUSTOMER_CODE == CC_MTI || CUSTOMER_CODE == CC_PTI)
        {
            iTotal = 7;
        }
        else
        {
            iTotal = 4;
        }
        GetProductivity(Data);
    }
    else if (ReportID == "0003")   //載入的IC數
    {
        iTotal = 1;
        Data[0] = GetLoadCount();
    }
    else if (ReportID == "0004")   //每個Tray分幾顆IC
    {
        iTotal = 9;
        GetSortingCount(Data);
    }
    else if (ReportID == "0005")
    {
        iTotal = GetSocketCount(Data);
    }
    else if (ReportID == "0006")
    {
        iTotal = GetHeadCount(Data);
    }
    else if (ReportID == "0007")
    {
        iTotal = 2;
        Data[0] = "100";
        Data[1] = "101";
    }

    SV_TOTAL = AnsiString(iTotal);
    sv_total = AddBlock(SV_TOTAL);
    HEAD = HEAD + SV_TOTAL;
    head = head + sv_total;

    for (i = 0; i < iTotal; i++)
    {
        HEAD = HEAD + SOH + Data[i];
        head = head + soh + AddBlock(Data[i]);
    }

    SendString = HEAD + ETX;
    S2 = head + etx;

    if (CUSTOMER_CODE == CC_MTI || CUSTOMER_CODE == CC_PTI)   //Sam 20190429 : Add CC_PTI_NEWWORK
    {
        ShowOLPState(1);
    }

    if (Standard)
        SendClient(SendString, S2, iSocketHandle);                                        //2007_0522
    else
        SendServer(SendString, S2);
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
