// ===========================================================================
//  KYECFTP/FTPClient_EventHandlers.cpp
//  Implementation of the NMFTP1 event-handler bodies. See the .h for the
//  full provenance / discovered-quirk / design-decision notes -- this file
//  only implements what is documented there.
//
//  AI(W5-Final-FTPClientEventHandlers) 20260711: new file.
// ===========================================================================
#include "KYECFTP/FTPClient_EventHandlers.h"

#include "cmydef.h"            // CUSTOMER_CODE (extern int, cmydef.h:3181)
                                // + transitively MachineType.h (CC_JSCC_OS #define :152)
#include "canary_support.h"     // ShowMyMessage (golden mymessbox.h:58) -- real Sim body,
                                // already linked by every other translated leaf that needs it.
                                // NOTE: canary_support.h is a hard-boundary shared header this
                                // unit only INCLUDES (read-only use), never edits.

// ---------------------------------------------------------------------------
//  MyDBIProcess -- golden cMyDB.h:20 (`void __fastcall MyDBIProcess(AnsiString
//  asTable, AnsiString S1, AnsiString S2="")`, 3-arg with a defaulted S2).
//  Every call in THIS unit's scope (NMFTP1Success/NMFTP1Failure) supplies
//  exactly 2 explicit args, relying on golden's S2 default -- so it links
//  directly against the already-real 2-arg overload `aHotPlateSubstrate.cpp:
//  698` (`void MyDBIProcess(AnsiString, AnsiString) {}`), declared here via a
//  local `extern` redeclaration -- the SAME established pattern
//  `CCLink/MyCCLink.cpp:36` already uses (deliberately NOT #including the
//  796-line aHotPlateSubstrate.h just for this one extern).
// ---------------------------------------------------------------------------
extern void MyDBIProcess(AnsiString S1, AnsiString S2);

// ===========================================================================
//  Golden file-scope globals (FTPClient.cpp:42/44) + demoted singleton
//  members (see .h STATE DESIGN note). Defined here (this is the one TU that
//  owns them until a Phase-2 unit translates the rest of FTPClient.cpp).
// ===========================================================================
vclcompat::TStringList* tmpList = NULL;   // golden :42 (lazily `new`'d, matches golden's own NULL-check-then-new idiom)
bool                    bTempList = false; // golden :44
bool                    bListOk   = false; // golden TfFTPClient::bListOk initial state (never explicitly initialized
                                            // in golden's ctor either -- BCB6 zero-initializes form fields; false is
                                            // the faithful equivalent)
bool                    bError    = false; // ditto, TfFTPClient::bError

void FTPClientEvt_ResetStateForTest()
{
    if (tmpList) { tmpList->Clear(); }
    bTempList = false;
    bListOk   = false;
    bError    = false;
    ResetFTPClientEvtLogSinkToSimDefault();
}

// ===========================================================================
//  Widget-touch seam (see .h WIDGET-TOUCH SEAM note).
// ===========================================================================
namespace {

std::vector<AnsiString> g_SimMemoLines;
std::vector<AnsiString> g_SimServerFileItems;
std::vector<AnsiString> g_SimListBox1Items;

FTPClientEvt_LogSink g_LogSink; // installed by ResetFTPClientEvtLogSinkToSimDefault() below

void InstallSimLogSink()
{
    g_LogSink.AddMemoLine       = [](const AnsiString& s) { g_SimMemoLines.push_back(s); };
    g_LogSink.AddServerFileItem = [](const AnsiString& s) { g_SimServerFileItems.push_back(s); };
    g_LogSink.AddListBox1Item   = [](const AnsiString& s) { g_SimListBox1Items.push_back(s); };
}

// Golden :1813, TfLotInfo (uLotInfo.h). Not on FormsFacade.h's TfLotInfo yet
// (grepped -- confirmed absent). See .h "GATED DEPENDENCY" note for the
// conservative-default rationale.
AnsiString Gated_GetLotInfo_sJSCCOSFileName()
{
    // TODO(FormsFacade-wiring): golden FTPClient.cpp:1813 needs
    // fLotInfo->sJSCCOSFileName (AnsiString member, TfLotInfo/uLotInfo.h).
    // Add `AnsiString sJSCCOSFileName;` to FormsFacade.h's TfLotInfo and
    // replace this stand-in with a direct `fLotInfo->sJSCCOSFileName` read.
    return AnsiString("");
}

// Struct-static init-order-safe "run once" installer -- guarantees the Sim
// sink is installed before any translation-unit-init-order-dependent first
// use (same reasoning MiniFtpEngine.cpp used for its own HType seeding).
struct SimSinkInstaller { SimSinkInstaller() { InstallSimLogSink(); } } g_SimSinkInstaller;

} // anonymous namespace

void SetFTPClientEvtLogSink(const FTPClientEvt_LogSink& Sink)
{
    g_LogSink = Sink;
}

void ResetFTPClientEvtLogSinkToSimDefault()
{
    InstallSimLogSink();
    g_SimMemoLines.clear();
    g_SimServerFileItems.clear();
    g_SimListBox1Items.clear();
}

const std::vector<AnsiString>& FTPClientEvt_SimMemoLines()       { return g_SimMemoLines; }
const std::vector<AnsiString>& FTPClientEvt_SimServerFileItems() { return g_SimServerFileItems; }
const std::vector<AnsiString>& FTPClientEvt_SimListBox1Items()   { return g_SimListBox1Items; }

// ===========================================================================
//  NMFTP1ListItem -- golden FTPClient.cpp:1799-1829.
// ===========================================================================
void FTPClientEvt_NMFTP1ListItem(AnsiString Listing)
{
    bool bAdd = true;
    // list the files inside a directory -----
    if (bTempList == false)
    {
        if (tmpList == NULL)
            tmpList = new vclcompat::TStringList;

        // DISCOVERED GOLDEN QUIRK #4 (preserved verbatim, NOT fixed): by De
        // Morgan, this condition EXCLUDES an entry only when it contains
        // BOTH ".Offset" AND the literal substring "ATC_Recipe.zip" at once
        // (e.g. "ATC_Recipe.zip.Offset.zip") -- an entry containing just
        // ".Offset" alone (e.g. "Foo.Offset.zip") is NOT excluded, since
        // `AnsiPos("ATC_Recipe.zip")==0` (not-found) makes the OR true. The
        // "JerryYang 20190906" comment's evident INTENT ("normally hide
        // .Offset files, except let ATC_Recipe.zip through") would need
        // `AnsiPos("ATC_Recipe.zip") != 0` here, not `==0` -- this reads as a
        // genuine golden logic slip, transcribed EXACTLY as written (not
        // "fixed"). See test [G] for a worked example of both branches.
        if (Listing.AnsiPos(".zip") > 0 &&
            (Listing.AnsiPos(".Offset") == 0 ||
             Listing.AnsiPos("ATC_Recipe.zip") == 0))              // JerryYang 20190906: ATC work-file goes via Handler upload/download
        {
            if (CUSTOMER_CODE == CC_JSCC_OS &&                     // RogerYang 20260127: JSCC_OS only shows the device's matching work-file
                Listing.AnsiPos(Gated_GetLotInfo_sJSCCOSFileName()) == 0)
            {
                bAdd = false;
            }

            if (bAdd == true)
            {
                if (g_LogSink.AddServerFileItem)
                    g_LogSink.AddServerFileItem(Listing.SubString(1, Listing.Length() - 4));
                tmpList->Add(Listing.SubString(1, Listing.Length() - 4));  // Landam
            }
        }
    }
    else
    {
        if (g_LogSink.AddListBox1Item)
            g_LogSink.AddListBox1Item(Listing);
    }
}

// ===========================================================================
//  NMFTP1Success -- golden FTPClient.cpp:1960-1986.
// ===========================================================================
void FTPClientEvt_NMFTP1Success(TCmdType Trans_Type)
{
    AnsiString Str = "";
    switch (Trans_Type)
    {
        case cmdChangeDir   : Str = "Success -- ChangeDir successful"     ; break;
        case cmdMakeDir     : Str = "Success -- MakeDir successful"       ; break;
        case cmdRemoveDir   : Str = "Success -- RemoveDir successful"     ; break;
        case cmdDelete      : Str = "Success -- Delete successful"        ; break;
        case cmdRename      : Str = "Success -- Rename successful"        ; break;
        case cmdReInit      : Str = "Success -- Reinitialize successful"  ; break;
        case cmdCurrentDir  : Str = "Success -- CurrentDir successful"    ; break;
        case cmdList        : Str = "Success -- List successful"         ; bListOk = true; break;
        case cmdNList       : Str = "Success -- NList successful"        ; bListOk = true; break;
        case cmdUpRestore   : Str = "Success -- UpRestore successful"     ; break;
        case cmdDownRestore : Str = "Success -- DownRestore successful"   ; break;
        case cmdDownload    : Str = "Success -- Download successful"      ; break;
        case cmdUpload      : Str = "Success -- Upload successful"        ; break;
        case cmdAppend      : Str = "Success -- Append successful"        ; break;
        case cmdAllocate    : Str = "Success -- Allocate successful"      ; break;
        case cmdDoCommand   : Str = "Success -- DoCommand successful"     ; break;
    }
    if (g_LogSink.AddMemoLine)
        g_LogSink.AddMemoLine(Str);
    MyDBIProcess("Exception", Str);
    bError = false;
}

// ===========================================================================
//  NMFTP1AuthenticationFailed -- golden FTPClient.cpp:1987-1992.
// ===========================================================================
void FTPClientEvt_NMFTP1AuthenticationFailed(bool & /*Handled*/)
{
    // Handled deliberately left untouched -- see QUIRK #3 in the .h.
    if (g_LogSink.AddMemoLine)
        g_LogSink.AddMemoLine("Authentication Failed");
    bError = true;
}

// ===========================================================================
//  NMFTP1TransactionStop -- golden FTPClient.cpp:1993-1997.
// ===========================================================================
void FTPClientEvt_NMFTP1TransactionStop(TObject * /*Sender*/)
{
    if (g_LogSink.AddMemoLine)
        g_LogSink.AddMemoLine("Data transfer end");
}

// ===========================================================================
//  NMFTP1TransactionStart -- golden FTPClient.cpp:1998-2002.
// ===========================================================================
void FTPClientEvt_NMFTP1TransactionStart(TObject * /*Sender*/)
{
    if (g_LogSink.AddMemoLine)
        g_LogSink.AddMemoLine("Data transfer start");
}

// ===========================================================================
//  NMFTP1ConnectionFailed -- golden FTPClient.cpp:2003-2008.
// ===========================================================================
void FTPClientEvt_NMFTP1ConnectionFailed(TObject * /*Sender*/)
{
    if (g_LogSink.AddMemoLine)
        g_LogSink.AddMemoLine("Failure -- Connection Failed");
    bError = true;
}

// ===========================================================================
//  NMFTP1Failure -- golden FTPClient.cpp:2009-2035.
// ===========================================================================
void FTPClientEvt_NMFTP1Failure(bool & /*Handled*/, TCmdType Trans_Type)
{
    // Handled deliberately left untouched -- see QUIRK #3 in the .h.
    AnsiString Str = "";
    switch (Trans_Type)
    {
        case cmdChangeDir   : Str = "Failure -- ChangeDir failed"      ; break;
        case cmdMakeDir     : Str = "Failure -- MakeDir failed"        ; break;
        case cmdDelete      : Str = "Failure -- Delete failed"         ; break;
        case cmdRemoveDir   : Str = "Failure -- RemoveDir failed"      ; break;
        case cmdList        : Str = "Failure -- List failed"           ; break;
        case cmdRename      : Str = "Failure -- Rename failed"         ; break;
        case cmdUpRestore   : Str = "Failure -- UploadRestore failed"  ; break;
        case cmdDownRestore : Str = "Failure -- DownloadRestore failed"; break;
        case cmdDownload    : Str = "Failure -- Download failed"       ; break;
        case cmdUpload      : Str = "Failure -- Upload failed"         ; break;
        case cmdAppend      : Str = "Failure -- UploadAppend failed"   ; break;
        case cmdReInit      : Str = "Failure -- Reinitialize failed"   ; break;
        case cmdAllocate    : Str = "Failure -- Allocate failed"       ; break;
        case cmdNList       : Str = "Failure -- NList failed"          ; break;
        case cmdDoCommand   : Str = "Failure -- DoCommand failed"      ; break;
        case cmdCurrentDir  : Str = "Failure -- CurrentDir failed"     ; break;
    }
    if (g_LogSink.AddMemoLine)
        g_LogSink.AddMemoLine(Str);
    MyDBIProcess("Exception", Str);
    bError = true;
}

// ===========================================================================
//  NMFTP1UnSupportedFunction -- golden FTPClient.cpp:2036-2061.
//  DEAD in golden (never wired to any TNMFTP instance -- see QUIRK #2 in the
//  .h). Translated faithfully anyway per this unit's task scope; NOT wired
//  by FTPClientEvt_WireNMFTP1Handlers() below, matching golden exactly.
// ===========================================================================
void FTPClientEvt_NMFTP1UnSupportedFunction(TCmdType Trans_Type)
{
    AnsiString Str = "";
    switch (Trans_Type)
    {
        case cmdChangeDir   : Str = "UnSupported -- ChangeDir not supported by this server"       ; break;
        case cmdMakeDir     : Str = "UnSupported -- MakeDir not supported by this server"          ; break;
        case cmdDelete      : Str = "UnSupported -- Delete not supported by this server"           ; break;
        case cmdRemoveDir   : Str = "UnSupported -- RemoveDir not supported by this server"        ; break;
        case cmdList        : Str = "UnSupported -- List not supported by this server"             ; break;
        case cmdRename      : Str = "UnSupported -- Rename not supported by this server"           ; break;
        case cmdUpRestore   : Str = "UnSupported -- UploadRestore not supported by this server"    ; break;
        case cmdDownRestore : Str = "UnSupported -- DownloadRestore not supported by this server"  ; break;
        case cmdDownload    : Str = "UnSupported -- Download not supported by this server"         ; break;
        case cmdUpload      : Str = "UnSupported -- Upload not supported by this server"           ; break;
        case cmdAppend      : Str = "UnSupported -- UploadAppend not supported by this server"     ; break;
        case cmdReInit      : Str = "UnSupported -- Reinitialize not supported by this server"     ; break;
        case cmdAllocate    : Str = "UnSupported -- Allocate not supported by this server"         ; break;
        case cmdNList       : Str = "UnSupported -- NList not supported by this server"            ; break;
        case cmdDoCommand   : Str = "UnSupported -- DoCommand not supported by this server"        ; break;
        case cmdCurrentDir  : Str = "UnSupported -- CurrentDir not supported by this server"       ; break;
    }
    if (g_LogSink.AddMemoLine)
        g_LogSink.AddMemoLine(Str);
    MyDBIProcess("Exception", Str);
}

// ===========================================================================
//  NMFTP1Error -- golden FTPClient.cpp:2086-2091.
//  //Eastsun 20260518: added error-location code
// ===========================================================================
void FTPClientEvt_NMFTP1Error(TComponent * /*Sender*/, vclcompat::Word /*Errno*/,
                               AnsiString Errmsg)
{
    ShowMyMessage("TfFTPClient:NMFTP1Error:" + Errmsg, "");
}

// ===========================================================================
//  NMFTP1Status -- golden FTPClient.cpp:2092-2097.
//  //Eastsun 20260518: added error-location code
// ===========================================================================
void FTPClientEvt_NMFTP1Status(TComponent * /*Sender*/, AnsiString Status)
{
    ShowMyMessage("TfFTPClient:NMFTP1Status:" + Status, "");
}

// ===========================================================================
//  FTPClientEvt_WireNMFTP1Handlers -- see .h banner. Reproduces golden's own
//  9-line wiring block verbatim (golden :137-145 et al.) -- deliberately
//  excludes OnUnSupportedFunction (QUIRK #2) and OnConnect/OnDisconnect
//  (NMFTP1 has no such handlers in golden at all -- only the sibling NMFTP2
//  family does, golden :4225/:4232, out of this unit's scope).
// ===========================================================================
void FTPClientEvt_WireNMFTP1Handlers(Nmftp::TNMFTP& Ftp)
{
    Ftp.OnListItem             = FTPClientEvt_NMFTP1ListItem;
    Ftp.OnSuccess              = FTPClientEvt_NMFTP1Success;
    Ftp.OnFailure              = FTPClientEvt_NMFTP1Failure;
    Ftp.OnError                = FTPClientEvt_NMFTP1Error;
    Ftp.OnStatus               = FTPClientEvt_NMFTP1Status;
    Ftp.OnConnectionFailed     = FTPClientEvt_NMFTP1ConnectionFailed;
    Ftp.OnTransactionStart     = FTPClientEvt_NMFTP1TransactionStart;
    Ftp.OnTransactionStop      = FTPClientEvt_NMFTP1TransactionStop;
    Ftp.OnAuthenticationFailed = FTPClientEvt_NMFTP1AuthenticationFailed;
    // OnUnSupportedFunction deliberately NOT wired -- see QUIRK #2.
}
