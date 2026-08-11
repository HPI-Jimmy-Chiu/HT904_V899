// ---------------------------------------------------------------------------
// MIGRATION NOTE (W1 Public leaf utilities - HT9045 BCB6 -> Standard C++17)
// Provenance: HT9011UC_Code_V3.33.906.0_20260618/Public/WinSocketErrorCode.cpp
// Changes from BCB6 original:
//   - Removed #include "MachineDefine.h", #pragma hdrstop,
//     #pragma package(smart_init): Borland-specific, not portable.
//   - Removed #include "cMyDB.h": only needed for deferred function.
//   - Added #include <winsock2.h> for WSAE*/WSA_QOS_* constants.
//   - Added #include "vclcompat/vcl_compat.h" for AnsiString/IntToStr.
//   - Dropped __fastcall from function definition (BCB6-only calling convention).
//   - GetErrorMsg(TObject*,int): faithfully translated; Sender unused
//     (matching BCB6 lines 12-314). 7 string literals (lines ~60-231)
//     carried 8 occurrences of a raw Big5 byte pair (0xA1 0x58, the
//     Big5 em dash) copied verbatim from the golden BCB6 source; under
//     UTF-8 that pair is an invalid/orphan byte sequence (MSVC C4828),
//     not merely a cosmetic issue. //AI(W906-encoding) 20260728: transcoded
//     the whole file cp950->UTF-8 (byte-exact per-line verification: every
//     line's decoded text is identical before/after, only the on-disk byte
//     encoding changed) so each pair now round-trips as a single valid
//     UTF-8 EM DASH (U+2014), matching what the Big5-locale BCB6 build
//     actually rendered.
//   - LogClientSocketExceptionError DEFERRED: TClientSocket->Name/Address/
//     Port fields require VCL ScktComp.hpp; MyDBIProcess requires cMyDB.
//     (BCB6 source lines 316-327.)
// ---------------------------------------------------------------------------

#include "vclcompat/vcl_compat.h"   // AnsiString, TObject, IntToStr
#include "Public/WinSocketErrorCode.h"
#include <winsock2.h>               // WSAE*/WSA_QOS_* constants

// BCB6 source: WinSocketErrorCode.cpp:12-314
AnsiString GetErrorMsg(TObject * /*Sender*/, int ErrorCode)
{
    AnsiString asResultStr = "";
    switch (ErrorCode)
    {
        case 6   /*WSA_INVALID_HANDLE*/:
            asResultStr = "Socket Error (6) Specified event object handle is invalid. An application attempts to use an event object, but the specified handle is not valid. Note that this error is returned by the operating system, so the error number may change in future releases of Windows.";
            break;
        case 8   /*WSA_NOT_ENOUGH_MEMORY*/:
            asResultStr = "Socket Error (8) Insufficient memory available. An application used a Windows Sockets function that directly maps to a Windows function. The Windows function is indicating a lack of required memory resources. Note that this error is returned by the operating system, so the error number may change in future releases of Windows.";
            break;
        case 87  /*WSA_INVALID_PARAMETER*/:
            asResultStr = "Socket Error (87) One or more parameters are invalid. An application used a Windows Sockets function which directly maps to a Windows function. The Windows function is indicating a problem with one or more parameters. Note that this error is returned by the operating system, so the error number may change in future releases of Windows.";
            break;
        case 995 /*WSA_OPERATION_ABORTED*/:
            asResultStr = "Socket Error (995) Overlapped operation aborted. An overlapped operation was canceled due to the closure of the socket, or the execution of the SIO_FLUSH command in WSAIoctl. Note that this error is returned by the operating system, so the error number may change in future releases of Windows.";
            break;
        case 996 /*WSA_IO_INCOMPLETE*/:
            asResultStr = "Socket Error (996) Overlapped I/O event object not in signaled state. The application has tried to determine the status of an overlapped operation which is not yet completed. Applications that use WSAGetOverlappedResult (with the fWait flag set to FALSE) in a polling mode to determine when an overlapped operation has completed, get this error code until the operation is complete. Note that this error is returned by the operating system, so the error number may change in future releases of Windows.";
            break;
        case 997 /*WSA_IO_PENDING*/:
            asResultStr = "Socket Error (997) Overlapped operations will complete later. The application has initiated an overlapped operation that cannot be completed immediately. A completion indication will be given later when the operation has been completed. Note that this error is returned by the operating system, so the error number may change in future releases of Windows.";
            break;
        case WSAEINTR:
            asResultStr = "Socket Error (10004) Interrupted function call. A blocking operation was interrupted by a call to WSACancelBlockingCall.";
            break;
        case WSAEBADF:
            asResultStr = "Socket Error (10009) File handle is not valid. The file handle supplied is not valid.";
            break;
        case WSAEACCES:
            asResultStr = "Socket Error (10013) Permission denied. An attempt was made to access a socket in a way forbidden by its access permissions. An example is using a broadcast address for sendto without broadcast permission being set using setsockopt(SO_BROADCAST). Another possible reason for the WSAEACCES error is that when the bind function is called (on Windows NT 4.0 with SP4 and later), another application, service, or kernel mode driver is bound to the same address with exclusive access. Such exclusive access is a new feature of Windows NT 4.0 with SP4 and later, and is implemented by using the SO_EXCLUSIVEADDRUSE option.";
            break;
        case WSAEFAULT:
            asResultStr = "Socket Error (10014) Bad address. The system detected an invalid pointer address in attempting to use a pointer argument of a call. This error occurs if an application passes an invalid pointer value, or if the length of the buffer is too small. For instance, if the length of an argument, which is a sockaddr structure, is smaller than the sizeof(sockaddr).";
            break;
        case WSAEINVAL:
            asResultStr = "Socket Error (10022) Invalid argument. Some invalid argument was supplied (for example, specifying an invalid level to the setsockopt function). In some instances, it also refers to the current state of the socket—for instance, calling accept on a socket that is not listening.";
            break;
        case WSAEMFILE:
            asResultStr = "Socket Error (10024) Too many open files. Too many open sockets. Each implementation may have a maximum number of socket handles available, either globally, per process, or per thread.";
            break;
        case WSAEWOULDBLOCK:
            asResultStr = "Socket Error (10035) Resource temporarily unavailable. This error is returned from operations on nonblocking sockets that cannot be completed immediately, for example recv when no data is queued to be read from the socket. It is a nonfatal error, and the operation should be retried later. It is normal for WSAEWOULDBLOCK to be reported as the result from calling connect on a nonblocking SOCK_STREAM socket, since some time must elapse for the connection to be established.";
            break;
        case WSAEINPROGRESS:
            asResultStr = "Socket Error (10036) Operation now in progress. A blocking operation is currently executing. Windows Sockets only allows a single blocking operation—per- task or thread—to be outstanding, and if any other function call is made (whether or not it references that or any other socket) the function fails with the WSAEINPROGRESS error.";
            break;
        case WSAEALREADY:
            asResultStr = "Socket Error (10037) Operation already in progress. An operation was attempted on a nonblocking socket with an operation already in progress—that is, calling connect a second time on a nonblocking socket that is already connecting, or canceling an asynchronous request (WSAAsyncGetXbyY) that has already been canceled or completed.";
            break;
        case WSAENOTSOCK:
            asResultStr = "Socket Error (10038) Socket operation on nonsocket. An operation was attempted on something that is not a socket. Either the socket handle parameter did not reference a valid socket, or for select, a member of an fd_set was not valid.";
            break;
        case WSAEDESTADDRREQ:
            asResultStr = "Socket Error (10039) Destination address required. A required address was omitted from an operation on a socket. For example, this error is returned if sendto is called with the remote address of ADDR_ANY.";
            break;
        case WSAEMSGSIZE:
            asResultStr = "Socket Error (10040) Message too long. A message sent on a datagram socket was larger than the internal message buffer or some other network limit, or the buffer used to receive a datagram was smaller than the datagram itself.";
            break;
        case WSAEPROTOTYPE:
            asResultStr = "Socket Error (10041) Protocol wrong type for socket. A protocol was specified in the socket function call that does not support the semantics of the socket type requested. For example, the ARPA Internet UDP protocol cannot be specified with a socket type of SOCK_STREAM.";
            break;
        case WSAENOPROTOOPT:
            asResultStr = "Socket Error (10042) Bad protocol option. An unknown, invalid or unsupported option or level was specified in a getsockopt or setsockopt call.";
            break;
        case WSAEPROTONOSUPPORT:
            asResultStr = "Socket Error (10043) Protocol not supported. The requested protocol has not been configured into the system, or no implementation for it exists. For example, a socket call requests a SOCK_DGRAM socket, but specifies a stream protocol.";
            break;
        case WSAESOCKTNOSUPPORT:
            asResultStr = "Socket Error (10044) Socket type not supported. The support for the specified socket type does not exist in this address family. For example, the optional type SOCK_RAW might be selected in a socket call, and the implementation does not support SOCK_RAW sockets at all.";
            break;
        case WSAEOPNOTSUPP:
            asResultStr = "Socket Error (10045) Operation not supported. The attempted operation is not supported for the type of object referenced. Usually this occurs when a socket descriptor to a socket that cannot support this operation is trying to accept a connection on a datagram socket.";
            break;
        case WSAEPFNOSUPPORT:
            asResultStr = "Socket Error (10046) Protocol family not supported. The protocol family has not been configured into the system or no implementation for it exists. This message has a slightly different meaning from WSAEAFNOSUPPORT. However, it is interchangeable in most cases, and all Windows Sockets functions that return one of these messages also specify WSAEAFNOSUPPORT.";
            break;
        case WSAEAFNOSUPPORT:
            asResultStr = "Socket Error (10047) Address family not supported by protocol family. An address incompatible with the requested protocol was used. All sockets are created with an associated address family (that is, AF_INET for Internet Protocols) and a generic protocol type (that is, SOCK_STREAM). This error is returned if an incorrect protocol is explicitly requested in the socket call, or if an address of the wrong family is used for a socket, for example, in sendto.";
            break;
        case WSAEADDRINUSE:
            asResultStr = "Socket Error (10048) Address already in use. Typically, only one usage of each socket address (protocol/IP address/port) is permitted. This error occurs if an application attempts to bind a socket to an IP address/port that has already been used for an existing socket, or a socket that was not closed properly, or one that is still in the process of closing. For server applications that need to bind multiple sockets to the same port number, consider using setsockopt (SO_REUSEADDR). Client applications usually need not call bind at all—connect chooses an unused port automatically. When bind is called with a wildcard address (involving ADDR_ANY), a WSAEADDRINUSE error could be delayed until the specific address is committed. This could happen with a call to another function later, including connect, listen, WSAConnect, or WSAJoinLeaf.";
            break;
        case WSAEADDRNOTAVAIL:
            asResultStr = "Socket Error (10049) Cannot assign requested address. The requested address is not valid in its context. This normally results from an attempt to bind to an address that is not valid for the local computer. This can also result from connect, sendto, WSAConnect, WSAJoinLeaf, or WSASendTo when the remote address or port is not valid for a remote computer (for example, address or port 0).";
            break;
        case WSAENETDOWN:
            asResultStr = "Socket Error (10050) Network is down. A socket operation encountered a dead network. This could indicate a serious failure of the network system (that is, the protocol stack that the Windows Sockets DLL runs over), the network interface, or the local network itself.";
            break;
        case WSAENETUNREACH:
            asResultStr = "Socket Error (10051) Network is unreachable. A socket operation was attempted to an unreachable network. This usually means the local software knows no route to reach the remote host.";
            break;
        case WSAENETRESET:
            asResultStr = "Socket Error (10052) Network dropped connection on reset. The connection has been broken due to keep-alive activity detecting a failure while the operation was in progress. It can also be returned by setsockopt if an attempt is made to set SO_KEEPALIVE on a connection that has already failed.";
            break;
        case WSAECONNABORTED:
            asResultStr = "Socket Error (10053) Software caused connection abort. An established connection was aborted by the software in your host computer, possibly due to a data transmission time-out or protocol error.";
            break;
        case WSAECONNRESET:
            asResultStr = "Socket Error (10054) Connection reset by peer. An existing connection was forcibly closed by the remote host. This normally results if the peer application on the remote host is suddenly stopped, the host is rebooted, the host or remote network interface is disabled, or the remote host uses a hard close (see setsockopt for more information on the SO_LINGER option on the remote socket). This error may also result if a connection was broken due to keep-alive activity detecting a failure while one or more operations are in progress. Operations that were in progress fail with WSAENETRESET. Subsequent operations fail with WSAECONNRESET.";
            break;
        case WSAENOBUFS:
            asResultStr = "Socket Error (10055) No buffer space available. An operation on a socket could not be performed because the system lacked sufficient buffer space or because a queue was full.";
            break;
        case WSAEISCONN:
            asResultStr = "Socket Error (10056) Socket is already connected. A connect request was made on an already-connected socket. Some implementations also return this error if sendto is called on a connected SOCK_DGRAM socket (for SOCK_STREAM sockets, the to parameter in sendto is ignored) although other implementations treat this as a legal occurrence.";
            break;
        case WSAENOTCONN:
            asResultStr = "Socket Error (10057) Socket is not connected. A request to send or receive data was disallowed because the socket is not connected and (when sending on a datagram socket using sendto) no address was supplied. Any other type of operation might also return this error—for example, setsockopt setting SO_KEEPALIVE if the connection has been reset.";
            break;
        case WSAESHUTDOWN:
            asResultStr = "Socket Error (10058) Cannot send after socket shutdown. A request to send or receive data was disallowed because the socket had already been shut down in that direction with a previous shutdown call. By calling shutdown a partial close of a socket is requested, which is a signal that sending or receiving, or both have been discontinued.";
            break;
        case WSAETOOMANYREFS:
            asResultStr = "Socket Error (10059) Too many references. Too many references to some kernel object.";
            break;
        case WSAETIMEDOUT:
            asResultStr = "Socket Error (10060) Connection timed out. A connection attempt failed because the connected party did not properly respond after a period of time, or the established connection failed because the connected host has failed to respond.";
            break;
        case WSAECONNREFUSED:
            asResultStr = "Socket Error (10061) Connection refused. No connection could be made because the target computer actively refused it. This usually results from trying to connect to a service that is inactive on the foreign host—that is, one with no server application running.";
            break;
        case WSAELOOP:
            asResultStr = "Socket Error (10062) Cannot translate name. Cannot translate a name.";
            break;
        case WSAENAMETOOLONG:
            asResultStr = "Socket Error (10063) Name too long. A name component or a name was too long.";
            break;
        case WSAEHOSTDOWN:
            asResultStr = "Socket Error (10064) Host is down. A socket operation failed because the destination host is down. A socket operation encountered a dead host. Networking activity on the local host has not been initiated. These conditions are more likely to be indicated by the error WSAETIMEDOUT.";
            break;
        case WSAEHOSTUNREACH:
            asResultStr = "Socket Error (10065) No route to host. A socket operation was attempted to an unreachable host. See WSAENETUNREACH.";
            break;
        case WSAENOTEMPTY:
            asResultStr = "Socket Error (10066) Directory not empty. Cannot remove a directory that is not empty.";
            break;
        case WSAEPROCLIM:
            asResultStr = "Socket Error (10067) Too many processes. A Windows Sockets implementation may have a limit on the number of applications that can use it simultaneously. WSAStartup may fail with this error if the limit has been reached.";
            break;
        case WSAEUSERS:
            asResultStr = "Socket Error (10068) User quota exceeded. Ran out of user quota.";
            break;
        case WSAEDQUOT:
            asResultStr = "Socket Error (10069) Disk quota exceeded. Ran out of disk quota.";
            break;
        case WSAESTALE:
            asResultStr = "Socket Error (10070) Stale file handle reference. The file handle reference is no longer available.";
            break;
        case WSAEREMOTE:
            asResultStr = "Socket Error (10071) Item is remote. The item is not available locally.";
            break;
        case WSASYSNOTREADY:
            asResultStr = "Socket Error (10091) Network subsystem is unavailable. This error is returned by WSAStartup if the Windows Sockets implementation cannot function at this time because the underlying system it uses to provide network services is currently unavailable. Users should check: That the appropriate Windows Sockets DLL file is in the current path. That they are not trying to use more than one Windows Sockets implementation simultaneously. If there is more than one Winsock DLL on your system, be sure the first one in the path is appropriate for the network subsystem currently loaded. The Windows Sockets implementation documentation to be sure all necessary components are currently installed and configured correctly.";
            break;
        case WSAVERNOTSUPPORTED:
            asResultStr = "Socket Error (10092) Winsock.dll version out of range. The current Windows Sockets implementation does not support the Windows Sockets specification version requested by the application. Check that no old Windows Sockets DLL files are being accessed.";
            break;
        case WSANOTINITIALISED:
            asResultStr = "Socket Error (10093) Successful WSAStartup not yet performed. Either the application has not called WSAStartup or WSAStartup failed. The application may be accessing a socket that the current active task does not own (that is, trying to share a socket between tasks), or WSACleanup has been called too many times.";
            break;
        case WSAEDISCON:
            asResultStr = "Socket Error (10101) Graceful shutdown in progress. Returned by WSARecv and WSARecvFrom to indicate that the remote party has initiated a graceful shutdown sequence.";
            break;
        case WSAENOMORE:
            asResultStr = "Socket Error (10102) No more results. No more results can be returned by the WSALookupServiceNext function.";
            break;
        case WSAECANCELLED:
            asResultStr = "Socket Error (10103) Call has been canceled. A call to the WSALookupServiceEnd function was made while this call was still processing. The call has been canceled.";
            break;
        case WSAEINVALIDPROCTABLE:
            asResultStr = "Socket Error (10104) Procedure call table is invalid. The service provider procedure call table is invalid. A service provider returned a bogus procedure table to Ws2_32.dll. This is usually caused by one or more of the function pointers being NULL.";
            break;
        case WSAEINVALIDPROVIDER:
            asResultStr = "Socket Error (10105) Service provider is invalid. The requested service provider is invalid. This error is returned by the WSCGetProviderInfo and WSCGetProviderInfo32 functions if the protocol entry specified could not be found. This error is also returned if the service provider returned a version number other than 2.0.";
            break;
        case WSAEPROVIDERFAILEDINIT:
            asResultStr = "Socket Error (10106) Service provider failed to initialize. The requested service provider could not be loaded or initialized. This error is returned if either a service provider's DLL could not be loaded (LoadLibrary failed) or the provider's WSPStartup or NSPStartup function failed.";
            break;
        case WSASYSCALLFAILURE:
            asResultStr = "Socket Error (10107) System call failure. A system call that should never fail has failed. This is a generic error code, returned under various conditions. Returned when a system call that should never fail does fail. For example, if a call to WaitForMultipleEvents fails or one of the registry functions fails trying to manipulate the protocol/namespace catalogs. Returned when a provider does not return SUCCESS and does not provide an extended error code. Can indicate a service provider implementation error.";
            break;
        case WSASERVICE_NOT_FOUND:
            asResultStr = "Socket Error (10108) Service not found. No such service is known. The service cannot be found in the specified name space.";
            break;
        case WSATYPE_NOT_FOUND:
            asResultStr = "Socket Error (10109) Class type not found. The specified class was not found.";
            break;
        case WSA_E_NO_MORE:
            asResultStr = "Socket Error (10110) No more results. No more results can be returned by the WSALookupServiceNext function.";
            break;
        case WSA_E_CANCELLED:
            asResultStr = "Socket Error (10111) Call was canceled. A call to the WSALookupServiceEnd function was made while this call was still processing. The call has been canceled.";
            break;
        case WSAEREFUSED:
            asResultStr = "Socket Error (10112) Database query was refused. A database query failed because it was actively refused.";
            break;
        case WSAHOST_NOT_FOUND:
            asResultStr = "Socket Error (11001) Host not found. No such host is known. The name is not an official host name or alias, or it cannot be found in the database(s) being queried. This error may also be returned for protocol and service queries, and means that the specified name could not be found in the relevant database.";
            break;
        case WSATRY_AGAIN:
            asResultStr = "Socket Error (11002) Nonauthoritative host not found. This is usually a temporary error during host name resolution and means that the local server did not receive a response from an authoritative server. A retry at some time later may be successful.";
            break;
        case WSANO_RECOVERY:
            asResultStr = "Socket Error (11003) This is a nonrecoverable error. This indicates that some sort of nonrecoverable error occurred during a database lookup. This may be because the database files (for example, BSD-compatible HOSTS, SERVICES, or PROTOCOLS files) could not be found, or a DNS request was returned by the server with a severe error.";
            break;
        case WSANO_DATA:
            asResultStr = "Socket Error (11004) Valid name, no data record of requested type. The requested name is valid and was found in the database, but it does not have the correct associated data being resolved for. The usual example for this is a host name-to-address translation attempt (using gethostbyname or WSAAsyncGetHostByName) which uses the DNS (Domain Name Server). An MX record is returned but no A record—indicating the host itself exists, but is not directly reachable.";
            break;
        case WSA_QOS_RECEIVERS:
            asResultStr = "Socket Error (11005) QoS receivers. At least one QoS reserve has arrived.";
            break;
        case WSA_QOS_SENDERS:
            asResultStr = "Socket Error (11006) QoS senders. At least one QoS send path has arrived.";
            break;
        case WSA_QOS_NO_SENDERS:
            asResultStr = "Socket Error (11007) No QoS senders. There are no QoS senders.";
            break;
        case WSA_QOS_NO_RECEIVERS:
            asResultStr = "Socket Error (11008) QoS no receivers. There are no QoS receivers.";
            break;
        case WSA_QOS_REQUEST_CONFIRMED:
            asResultStr = "Socket Error (11009) QoS request confirmed. The QoS reserve request has been confirmed.";
            break;
        case WSA_QOS_ADMISSION_FAILURE:
            asResultStr = "Socket Error (11010) QoS admission error. A QoS error occurred due to lack of resources.";
            break;
        case WSA_QOS_POLICY_FAILURE:
            asResultStr = "Socket Error (11011) QoS policy failure. The QoS request was rejected because the policy system couldn't allocate the requested resource within the existing policy.";
            break;
        case WSA_QOS_BAD_STYLE:
            asResultStr = "Socket Error (11012) QoS bad style. An unknown or conflicting QoS style was encountered.";
            break;
        case WSA_QOS_BAD_OBJECT:
            asResultStr = "Socket Error (11013) QoS bad object. A problem was encountered with some part of the filterspec or the provider-specific buffer in general.";
            break;
        case WSA_QOS_TRAFFIC_CTRL_ERROR:
            asResultStr = "Socket Error (11014) QoS traffic control error. An error with the underlying traffic control (TC) API as the generic QoS request was converted for local enforcement by the TC API. This could be due to an out of memory error or to an internal QoS provider error.";
            break;
        case WSA_QOS_GENERIC_ERROR:
            asResultStr = "Socket Error (11015) QoS generic error. A general QoS error.";
            break;
        case WSA_QOS_ESERVICETYPE:
            asResultStr = "Socket Error (11016) QoS service type error. An invalid or unrecognized service type was found in the QoS flowspec.";
            break;
        case WSA_QOS_EFLOWSPEC:
            asResultStr = "Socket Error (11017) QoS flowspec error. An invalid or inconsistent flowspec was found in the QOS structure.";
            break;
        case WSA_QOS_EPROVSPECBUF:
            asResultStr = "Socket Error (11018) Invalid QoS provider buffer. An invalid QoS provider-specific buffer.";
            break;
        case WSA_QOS_EFILTERSTYLE:
            asResultStr = "Socket Error (11019) Invalid QoS filter style. An invalid QoS filter style was used.";
            break;
        case WSA_QOS_EFILTERTYPE:
            asResultStr = "Socket Error (11020) Invalid QoS filter type. An invalid QoS filter type was used.";
            break;
        case WSA_QOS_EFILTERCOUNT:
            asResultStr = "Socket Error (11021) Incorrect QoS filter count. An incorrect number of QoS FILTERSPECs were specified in the FLOWDESCRIPTOR.";
            break;
        case WSA_QOS_EOBJLENGTH:
            asResultStr = "Socket Error (11022) Invalid QoS object length. An object with an invalid ObjectLength field was specified in the QoS provider-specific buffer.";
            break;
        case WSA_QOS_EFLOWCOUNT:
            asResultStr = "Socket Error (11023) Incorrect QoS flow count. An incorrect number of flow descriptors was specified in the QoS structure.";
            break;
        case WSA_QOS_EUNKOWNPSOBJ:
            asResultStr = "Socket Error (11024) Unrecognized QoS object. An unrecognized object was found in the QoS provider-specific buffer.";
            break;
        case WSA_QOS_EPOLICYOBJ:
            asResultStr = "Socket Error (11025) Invalid QoS policy object. An invalid policy object was found in the QoS provider-specific buffer.";
            break;
        case WSA_QOS_EFLOWDESC:
            asResultStr = "Socket Error (11026) Invalid QoS flow descriptor. An invalid QoS flow descriptor was found in the flow descriptor list.";
            break;
        case WSA_QOS_EPSFLOWSPEC:
            asResultStr = "Socket Error (11027) Invalid QoS provider-specific flowspec. An invalid or inconsistent flowspec was found in the QoS provider-specific buffer.";
            break;
        case WSA_QOS_EPSFILTERSPEC:
            asResultStr = "Socket Error (11028) Invalid QoS provider-specific filterspec. An invalid FILTERSPEC was found in the QoS provider-specific buffer.";
            break;
        case WSA_QOS_ESDMODEOBJ:
            asResultStr = "Socket Error (11029) Invalid QoS shape discard mode object. An invalid shape discard mode object was found in the QoS provider-specific buffer.";
            break;
        case WSA_QOS_ESHAPERATEOBJ:
            asResultStr = "Socket Error (11030) Invalid QoS shaping rate object. An invalid shaping rate object was found in the QoS provider-specific buffer.";
            break;
        case WSA_QOS_RESERVED_PETYPE:
            asResultStr = "Socket Error (11031) Reserved policy QoS element type. A reserved policy element was found in the QoS provider-specific buffer.";
            break;
        default:
            // BCB6 source: WinSocketErrorCode.cpp:305
            asResultStr = "Socket Error (" + IntToStr(ErrorCode) + ") Unknown error code.";
            break;
    }
    return asResultStr;
}
// ---------------------------------------------------------------------------
// LogClientSocketExceptionError -- DEFERRED
// Requires VCL TClientSocket (->Name/Address/Port) from ScktComp.hpp and
// MyDBIProcess from cMyDB.h.  Neither is available in the W1 pure-logic
// translation wave.  Original: WinSocketErrorCode.cpp:316-327.
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// WAVE SCOPE EXTENSION -- PT-Wk (APPEND-ONLY; the head banner above is left
// byte-for-byte untouched, per this wave's append-only rule)
// Translator: AI(PT-Wk-winsock) 20260811
// Golden source: HT9011UC_Code_V3.33.906.0_20260618/Public/WinSocketErrorCode.cpp
//
// ROLE
// ----
// Leaf utility TU of the ht9045_public library (CMakeLists.txt:439).  It turns a
// Winsock error code into an operator-readable AnsiString (GetErrorMsg, above)
// and -- with this extension -- formats a socket EXCEPTION into an EventLog
// line.  Nothing pumps it on a timer; every entry point is called from a VCL
// socket OnError / catch(...) handler in a consumer TU.
//
// WAVE SCOPE
// ----------
//   * LogClientSocketExceptionError(TObject*, AnsiString)   golden :316
//     ACTIVE formatting / GATED sink -- see GATE G-PTk4 and G-PTk5.
//
// GATE REGISTER
// -------------
// G-PTk4 -- the `MyDBIProcess("Exception", Str);` sink.
//   (a) GOLDEN LINE: Public/WinSocketErrorCode.cpp:326.
//   (b) WHY THE OFFLINE DEFAULT IS FAITHFUL -- and, more importantly, why this
//       SHOULD stay gated even though the old premise has expired.  The header
//       banner above (written in W1) says MyDBIProcess "requires cMyDB" and is
//       unavailable.  THAT PREMISE IS DEAD: MyDBIProcess IS now real in this
//       tree -- declared aHotPlateSubstrate.h:933, defined aHotPlateSubstrate.cpp
//       :1099 (a counting canary: W906_MyDBIProcess_Count / _LastS1 / _LastS2).
//       The gate stays anyway, for a DIFFERENT and structural reason:
//         - aHotPlateSubstrate.cpp lives in ht9045_sm (CMakeLists.txt:1466).
//         - THIS file lives in ht9045_public (CMakeLists.txt:439), whose only
//           link edge is `target_link_libraries(ht9045_public PUBLIC vclcompat)`
//           (CMakeLists.txt:464).  ht9045_public is a LEAF.
//         - tests/CMakeLists.txt:119 is `target_link_libraries(
//           test_WinSocketErrorCode PRIVATE ht9045_public)` -- ht9045_public
//           ALONE.  That test calls GetErrorMsg, which extracts THIS object
//           from libht9045_public.a.  An active MyDBIProcess call here would
//           make that link fail with `undefined reference to
//           MyDBIProcess(AnsiString, AnsiString)`, and the same is true for
//           test_HTMD5 / test_cJSON / test_ExternFunction / test_cBootLog /
//           test_keypro_tcomm if the object is ever pulled in for them.
//       So the honest answer to "why SHOULD this be gated" is: not because the
//       symbol is missing, but because wiring a leaf utility library to the
//       state-machine library inverts the dependency graph and breaks five
//       standalone unit-test targets.  The main loop owns that call (see the
//       report's "for the main loop" section); this file must not make it.
//   (c) HOW REAL-MACHINE BEHAVIOUR DIFFERS: on the machine this writes one row
//       into the "Exception" EventLog table per socket exception.  Offline the
//       row is not written.  It is pure best-effort DIAGNOSTICS -- golden's
//       callers (Interface/TesterTCP.cpp, Automation/automation.cpp,
//       EJ1N/uSocketServerClient.cpp, SECSGEM, BarCode) all call it from inside
//       a handler that has ALREADY done the behaviourally load-bearing thing
//       (Close()/Active=false/bConnected=false), so no control flow depends on
//       it.  `Str` is still built in full so un-gating is a one-line edit.
//
// G-PTk5 -- the `Ptr->Name` field read.
//   (a) GOLDEN LINE: Public/WinSocketErrorCode.cpp:322, the first `%s`.
//   (b) WHY THE OFFLINE DEFAULT IS FAITHFUL: in BCB6 `Name` is inherited from
//       VCL TComponent (the design-time component name, e.g. "ClientSocket1").
//       This port`s vclcompat TComponent (vclcompat/Comm.h:84) models only
//       Owner_ -- it has no Name -- and vclcompat TClientSocket
//       (vclcompat/ClientSocket.h:325) adds Address/Port/Tag but not Name.
//       There is no Name to read, and inventing one would fabricate diagnostic
//       text.  Substituting "" keeps the field PRESENT and empty, which is
//       exactly the shape golden itself emits on its own NULL path (:324,
//       `"Name:, IP:, Port:, ..."`).  This does not touch the IP/Port/Msg
//       fields, which are real.
//   (c) HOW REAL-MACHINE BEHAVIOUR DIFFERS: the logged line reads `Name:,`
//       instead of `Name:ClientSocket_TCPIP,`.  Diagnostics only -- and with
//       G-PTk4 in force the line is not written at all today.
//   UN-GATE CONDITION: add `AnsiString Name;` to vclcompat TComponent (VCL
//       parity) and populate it where the sockets are constructed.
//
// TRAP 5 (two headers, same class name) for the TClientSocket used below: there
// is exactly ONE `class TClientSocket` in this tree -- vclcompat/ClientSocket.h
// :325, in namespace Scktcomp, hoisted to global scope by that header`s own
// `using namespace Scktcomp` (:403).  No second declaration exists anywhere in
// the port.  The cast below reads only Address (AnsiString) and Port (int),
// both plain public data members of that one class, and calls NO member
// function -- so this TU adds no link-time dependency on ClientSocket.cpp
// either; it needs the layout, nothing more.
//
// NOTE ON INCLUDE PLACEMENT: `#include "vclcompat/ClientSocket.h"` sits HERE,
// mid-file, rather than in the include block at the top.  That is deliberate --
// this wave is strictly append-only on existing mirrors, and editing the header
// block would be an insertion.  It is placed after the file`s existing
// <winsock2.h> (:29) so the winsock2-before-windows.h ordering rule still holds.
// The main loop may hoist it to the top when it next touches this file.
// ---------------------------------------------------------------------------
#include "vclcompat/ClientSocket.h"   // TClientSocket (->Address/->Port); golden reached this via ScktComp.hpp

// ---------------------------------------------------------------------------
// LogClientSocketExceptionError -- golden Public/WinSocketErrorCode.cpp:316-327.
// ---------------------------------------------------------------------------
void LogClientSocketExceptionError(TObject *Sender, AnsiString Msg)             //Steven 20231113 : 紀錄網路連線例外
{
    TClientSocket *Ptr;
    Ptr=(TClientSocket *) Sender;
    AnsiString Str;
    if(Ptr!=NULL)
        // GATE G-PTk5: golden passes `Ptr->Name` as the first %s; vclcompat`s
        // TComponent/TClientSocket carry no Name member.  "" keeps the field
        // present and empty (same shape as golden`s own NULL path below).
        Str.sprintf("Name:%s, IP:%s, Port:%d, Exception Error:%s", "", Ptr->Address, Ptr->Port, Msg);
    else
        Str.sprintf("Name:, IP:, Port:, Exception Error:%s", Msg);

#if 0 // GATE G-PTk4 -- golden :326.  MyDBIProcess lives in ht9045_sm; this TU is
      // in the leaf library ht9045_public, and tests/CMakeLists.txt:119 links
      // test_WinSocketErrorCode against ht9045_public ALONE.  See register above.
    MyDBIProcess("Exception", Str);
#endif
    (void)Str;   // consumed by the gated sink above; kept built so un-gating is one line
}
// ---------------------------------------------------------------------------
