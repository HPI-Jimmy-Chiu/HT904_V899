// =============================================================================
//  Monitor/MonitorTCPIP.cpp  --  generic TCP-connection monitor/adapter wrapper
//
//  Faithful translation of golden Monitor/MonitorTCPIP.cpp (176 lines, BCB6,
//  Big5/cp950 -- this file has no Chinese text of its own).
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2 (group "socket").
//
//  ROLE: see MonitorTCPIP.h's banner for the class's overall role. This file
//  implements every method plus the free-function thread body MV_TCPIP_WATCH.
//
//  WAVE SCOPE -- ACTIVE vs satisfied-by-shim:
//   ACTIVE (faithful, verbatim -- all 12 golden functions with a golden body):
//     MV_TCPIP_WATCH(void*)                golden :12-27
//     MonitorTCPIP::MonitorTCPIP(TCPIP*)   golden :30-40
//     MonitorTCPIP::~MonitorTCPIP()        golden :43-54
//     InitialSocket(AnsiString,int)        golden :57-60
//     InitialMemo(TMemo*)                  golden :63-66
//     Connect()                            golden :69-72
//     Disconnect()                         golden :75-78
//     IsConnect()                          golden :81-84
//     GetReceiveCommand(int)               golden :87-90
//     SendCommand(DATA)                    golden :93-98
//     SendCommand(AnsiString)              golden :101-106
//     AdapterData(int&,DATA&)              golden :109-154
//     ShowData(AnsiString,DATA)            golden :157-176
//                                                                    (13 fns)
//   NOT DEFINED (matches golden -- see MonitorTCPIP.h's own note):
//     GetDataString()  golden :40 (declared in the .h, never defined anywhere
//       in golden's own .cpp, never called tree-wide). Not counted in
//       golden_fns/ported_fns below since there is no golden BODY to port.
//   golden_fns = 13 (every golden function that HAS a golden body); ported_fns
//   = 13. GetDataString() is a 14th DECLARATION with no body in either tree.
//   SATISFIED-BY-SUBSTRATE (already real in this tree, nothing shimmed here):
//     HThreadCtrl::Initial/SetSleepDelayCount/CloseThread (HThreadCtrl/
//     HThreadCtrl.cpp -- AI(W906-PT-W2) sibling unit, same wave), MySleep
//     (common.cpp:1919), MyDBIProcess (aHotPlateSubstrate.h:924), TMemo
//     (vclcompat/Controls.h:310, ->Lines a real TStringList*).
//
//  GATE REGISTER -- EMPTY.  Every call this file makes on its own
//  `TCPIP* _socket` member is ACTIVE and verbatim from golden.
//
//  AI(W906-PT-W2-integrate) 20260807: this banner previously carried ONE gate
//  FAMILY covering all 9 `_socket->...` call sites plus `delete _socket`,
//  justified like this:
//
//      "golden ATC/TCPData.h's class TCPIP ... confirmed by a whole-tree grep
//       (excluding build*/), run 20260807, to have NO port anywhere -- neither
//       the class nor a single one of its methods exists in any ported .h/.cpp"
//
//  THAT CLAIM WAS FALSE BY THE TIME THIS FILE LANDED, and the gate is retired.
//  ATC/TCPData.{h,cpp} were translated in the SAME wave (PT-W2, group "atc")
//  and written to disk BEFORE this file was.  ATC/TCPData.h:74-99 is the real
//  abstract `class TCPIP` with exactly the members golden calls here --
//  Initial/Connect/DisConnect/Send(DATA)/Send(string) pure virtual, plus
//  concrete IsConnect() (:91) and PopBuffer() (:97).  The grep was run at the
//  start of this unit's work and was simply stale by the end of it.
//
//  What the stale gate would have shipped: IsConnect() hardcoded to false,
//  both SendCommand() overloads hardcoded to -1, AdapterData() hardcoded to
//  0 bytes, and InitialSocket/Connect/Disconnect as permanent no-ops -- i.e. a
//  monitor link that reports "never connected, every send failed, nothing ever
//  received" on a machine where it is working, with the file's own banner
//  certifying that as faithful.  The gates are gone; all 9 sites now call the
//  real object, each tagged with its golden line.
//
//  PROCESS NOTE FOR FUTURE WAVES: an absence-claim ("X exists nowhere in the
//  tree") is only valid as of the instant it was measured.  When sibling groups
//  of the same wave are landing files concurrently, re-run the grep at the END
//  of the unit's work, not the start.  Three separate PT-W2 units made this
//  identical mistake (this one, EJ1N/MyOmronPanel.h, Public/HTEditList.cpp).
//
//  NOT CURRENTLY WIRED INTO THE BUILD, STATED PLAINLY: a whole-tree grep for
//  `MonitorTCPIP` (excluding build*/), run 20260807, found NO reference to
//  this class anywhere outside its own two files -- golden's sole consumer,
//  Monitor/MonitorInterface.cpp, has no port yet. This translation is
//  therefore currently a leaf unit with no live caller; it is added to
//  ht9045_sm's CMakeLists.txt sources by the integrate step per hard rule 7,
//  same as every other unit in this wave.
//
//  VCL/Borland conversions:
//    * No `_fastcall`/`__property`/`__published` anywhere in golden's own 176
//      lines -- nothing to strip.
//    * `#pragma hdrstop` (golden :2) and `#pragma package(smart_init)`
//      (golden :10) dropped, matching every other unit in this tree.
//    * golden's own commented-out `//#include "IncludeAllHeader.H"` (golden
//      :6) kept as a comment below, matching golden.
//
//  Big5: this file has no Chinese comments to preserve. Final gate: ZERO
//  U+FFFD (vacuously true).
// =============================================================================
// BCB6 ORIGINAL include block (mirrored as a comment for provenance):
//   #include "MachineDefine.h" ; #pragma hdrstop
//   #include "MonitorTCPIP.h"
//   //#include "IncludeAllHeader.H"
//   #include "common.h"
//   ; #pragma package(smart_init)
// =============================================================================
#include "MachineDefine.h"          // de-VCL'd include hub (vclcompat umbrella + portable STL + using namespace std)
#include "Monitor/MonitorTCPIP.h"   // this unit's own contract
// golden :6  //#include "IncludeAllHeader.H"  -- already commented out in golden
#include "common.h"                 // MySleep(DWORD) -- real body common.cpp:1919
#include "aHotPlateSubstrate.h"     // MyDBIProcess

//---------------------------------------------------------------------------
// Thread function ----------------------------------------------------------
void MV_TCPIP_WATCH( void* pParameter )
{
    MonitorTCPIP* pMV = (MonitorTCPIP*) pParameter;

    int iType;
    DATA vData;
    if( pMV->AdapterData( iType , vData ) != 0 )
    {
    }
    else
    {
        MySleep( 500 );
    }

     MySleep( 10 );
}

//---------------------------------------------------------------------------
MonitorTCPIP::MonitorTCPIP( TCPIP* pSocket )
{
    _socket     = pSocket;
    _memo       = NULL;

    for(int i=0; i<10; i++)
        _bCommandReady[i] = 0;

    _socket_thread.Initial( MV_TCPIP_WATCH , this );
    _socket_thread.SetSleepDelayCount(0);
}

//---------------------------------------------------------------------------
MonitorTCPIP::~MonitorTCPIP()
{
    try
    {
        delete _socket;                                                     // golden :47
        _socket_thread.CloseThread(1000);
    }
    catch(...)
    {
        MyDBIProcess("Exception", "~MonitorTCPIP");
    }
}

//---------------------------------------------------------------------------
void MonitorTCPIP::InitialSocket( AnsiString sAddress , int iPort )
{
    _socket->Initial( sAddress.c_str() , iPort );                           // golden :59
}

//---------------------------------------------------------------------------
void MonitorTCPIP::InitialMemo( TMemo* pMemo )
{
    _memo       = pMemo;
}

//---------------------------------------------------------------------------
void MonitorTCPIP::Connect()
{
    _socket->Connect();                                                     // golden :71
}

//---------------------------------------------------------------------------
void MonitorTCPIP::Disconnect()
{
    _socket->DisConnect();                                                  // golden :77
}

//---------------------------------------------------------------------------
bool MonitorTCPIP::IsConnect()
{
    return _socket->IsConnect();                                            // golden :83
}

//---------------------------------------------------------------------------
int MonitorTCPIP::GetReceiveCommand(int index)
{
    return _bCommandReady[index];
}

//---------------------------------------------------------------------------
int MonitorTCPIP::SendCommand( DATA SendData )
{
    //MySleep(10);    // Send Dealy

    return _socket->Send( SendData );                                       // golden :97
}

//---------------------------------------------------------------------------
int MonitorTCPIP::SendCommand( AnsiString SendData )
{
    //MySleep(10);    // Send Dealy

    return _socket->Send( SendData.c_str() );                               // golden :105
}

//---------------------------------------------------------------------------
int MonitorTCPIP::AdapterData( int& iType , DATA& vData )
{
    int iSize;

    iSize = _socket->PopBuffer( iType , vData );                            // golden :113

    if( iSize == 0 )
        return 0;

    if( iType == TD_READ )
    {
        ShowData( "[M->H] " , vData );
    }
    else if( iType == TD_SEND  )
    {
        ShowData( "[H->M] " , vData );
    }
    else if( iType == TD_STATE )
    {
        if( vData[0] == 0x00 )
        {
            ShowData( "[STATE] Connect OK! " , vData );
        }
        else if( vData[0] == 0xFF )
        {
            ShowData( "[STATE] Disconnect! " , vData );
        }
        else
        {
            ShowData( "[STATE] Unknow " , vData );
        }
    }
    else if( iType == TD_ERROR )
    {
        //int iCode = vData[0]*0xFF + vData[1];
        //AnsiString sErr = "[ERROR: " + AnsiString( iCode ) + " ]";

        //ShowData( sErr , vData );
    }
    else
    {
        //ShowData( "[UNKNOW]" , vData );
    }

    return iSize;
}

//---------------------------------------------------------------------------
void MonitorTCPIP::ShowData( AnsiString sType , DATA vData )
{
    AnsiString sLog     = sType + "";

    unsigned int iSize = vData.size();

    for( unsigned int i = 0 ; i < iSize ; i++ )
    {
        sLog    += (char)vData[i];
    }

    if( _memo != NULL )
    {
        if( _memo->Lines->Count >= 300 )
        {
            _memo->Clear();
        }
        _memo->Lines->Add( sLog );   // AI(W906-PT-W2) 20260807: golden ->Lines->Append(sLog) -- real VCL TStrings::Append is a thin forwarder to Add(); vclcompat::TStringList has no Append (confirmed by grep; same pre-existing gap already uncaught in PMAlarm/PMAlarmSystem.cpp:220/224/228), so this calls ->Add(sLog) directly -- byte-for-bit identical behaviour to real VCL, not a logic change.
    }
}
