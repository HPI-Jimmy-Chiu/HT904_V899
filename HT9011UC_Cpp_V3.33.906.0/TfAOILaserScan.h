// =============================================================================
//  TfAOILaserScan.h  --  generic AOI/laser-scanner client family (Loader tray
//                        residual-IC check + tray cell-map read-back)
//
//  Faithful translation of golden TfAOILaserScan.h (192 lines, BCB6,
//  Big5/cp950).
//  Translator: AI(W906-PT-W3) 20260807
//  Translation wave: W906-PT-W3 group "aoilaser".
//
//  ROLE: TAOISocket is a tiny standalone request/response socket cursor (its
//  own copy of the send/wait-for-reply state machine, duplicated onto
//  TfAOI's own socketClient below -- golden keeps both, this port keeps both
//  too).  TfAOI is the abstract "one AOI/laser-scan device" base: it owns the
//  live uSocketClient, the send/receive plumbing, the ASCII comma-protocol
//  helpers (GetAOI_Command's LF/E9/E1/GV/SL/CF/E2/GS opcode table), the
//  Loader-Y-motor move-out/move-in state machines that carry a tray under the
//  scanner, and an auto-tuning composite state machine.  Every actual AOI
//  command (LoadFile/ClearBuffer/Function/GetValue/CreateFile/GoldenImage/
//  GetStatus) is a pure-virtual-with-default (`{return true;}`) hook.
//  TfTrayDeviceCheckFromLoader overrides those hooks for a residual-IC GO/NOGO
//  check (function code "5"); TfTrayMapFromLoader overrides them for a full
//  per-cell tray map read-back (function code "4") into a TTMyTray grid
//  widget.  Both are instantiated by (not-yet-translated) cTrayMapping.cpp
//  ::InitialAOIGroup (golden :5398-5412) and registered into its
//  `std::map<int,TfAOI*> mapAOI`.
//
//  WAVE SCOPE -- ACTIVE (faithful, verbatim -- every golden function in this
//  header; all bodies below are inline exactly as golden wrote them):
//    TAOISocket                    golden :14-38  (class, all inline)
//    TfAOI                         golden :40-144 (class; 6 members have
//                                   .cpp bodies -- see TfAOILaserScan.cpp's
//                                   own function census)
//    TfTrayDeviceCheckFromLoader   golden :147-164 (class; all 10 members
//                                   have .cpp bodies)
//    TfTrayMapFromLoader           golden :167-190 (class; all 14 members
//                                   have .cpp bodies)
//  SATISFIED-BY-SUBSTRATE: uSocketClient (EJ1N/uSocketServerClient.h, real
//  compiled body EJ1N/uSocketServerClient.cpp), TStringList/AnsiString
//  (vclcompat), TQPF_Timer (myTimer.h/.cpp).
//
//  GATE REGISTER -- see TfAOILaserScan.cpp for the full writeup (this header
//  only needs the ONE forward declaration below; every call-site gate lives
//  in the .cpp because none of THIS file's inline bodies dereference a
//  TTMyTray).
//   (1) `class TTMyTray;` -- golden :9 `#include "HTray.h"` (golden
//       elec/myvcl/HTray.h, a VCL `TCustomControl`-derived custom grid
//       widget).  TTMyTray is one of the THREE custom VCL control families
//       this whole port tree defers to wave W7-C4 (vclcompat::TrayCore) --
//       see forms/FormWidgets.h's own TfMainGrid/TfMainAutoCleanGrid stand-
//       ins for golden's OTHER TTMyTray* members (htShullte0/1, tmyAutoClean,
//       mtPlate2), and Motor/mymotor.h:74's identical forward-decl-only
//       treatment of TTrayMotor::pHTray.  Grepped the WHOLE port tree this
//       wave (excluding build*/): zero compiled definitions of TTMyTray
//       exist anywhere.  A pointer to an incomplete type needs no complete
//       definition to declare (every TTMyTray* member/parameter/return type
//       in THIS header compiles against the forward declaration alone); the
//       .cpp gates the handful of call sites that actually dereference one.
//       Landing a real TTMyTray definition from inside this narrow,
//       single-golden-unit wave would exceed the wave's assigned scope (rule
//       7: new mirrored files ONLY, no touching forms/FormWidgets.h or
//       vclcompat/) and risk an ODR collision with whatever shape W7-C4
//       eventually gives vclcompat::TrayCore.
//
//  VCL/Borland conversions:
//   * `#include <StdCtrls.hpp>` (golden :6) -- this header declares no VCL
//     TObject/TComponent-derived class (TAOISocket/TfAOI/
//     TfTrayDeviceCheckFromLoader/TfTrayMapFromLoader are all plain C++
//     classes in golden too, never `: public TObject`), so nothing here
//     actually needs it; dropped in favour of "vclcompat/vcl_compat.h" for
//     AnsiString/TStringList.
//   * `#include "uSocketServerClient.h"` (golden :7) -> "EJ1N/uSocketServerClient.h",
//     this tree's established path for that unit (see EJ1N/uDTME08Control.h:75
//     for the identical mapping).
//   * `#include "HTray.h"` (golden :9) -> forward declaration only; see GATE (1).
//   * `typedef void (__closure *TPointVoidShowMsg)(AnsiString);` (golden :11)
//     -- BCB6 bound-method closure has no standard-C++ spelling.  Per
//     vclcompat/vcl_compat.h's documented convention, translated to
//     `std::function<void(AnsiString)>` -- the SAME choice this tree's
//     BarCode/BarCode_Helpers.h and EJ1N/uDTME08Control.h already made for
//     their own identically-named typedef (duplicate identical typedefs
//     across headers are legal C++, not an ODR conflict).  A default-
//     constructed std::function is "empty" (`operator bool()==false`),
//     matching golden's `if(RecordMsg)` null-closure guard at
//     TfAOILaserScan.cpp golden :35 exactly.  NOTE: TfAOI::RecordMsgLaser /
//     RecordErrorMsgLaser are, like golden, never assigned inside TfAOI's own
//     ctor (golden :92-110) -- they are wired up externally by (not yet
//     translated) TfTrayMapping::AddAOIMemeber (golden cTrayMapping.cpp
//     :5419-5424).  Several TfAOI/derived-class bodies call them
//     UNCONDITIONALLY, with no `if(RecordMsgLaser)` guard, exactly as golden
//     does -- calling an empty std::function there throws
//     std::bad_function_call, the direct analogue of golden's own null-
//     closure-call undefined behaviour if AddAOIMemeber were skipped.  Not
//     "fixed" here; preserved verbatim, both class of failure and cause.
//   * `__fastcall` / `__property` / `__published`: none in this header --
//     golden has none either (confirmed: no VCL base class).
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8.
//  Final gate: ZERO U+FFFD.
// =============================================================================
// BCB6 ORIGINAL include block (mirrored as a comment for provenance):
//   #include <StdCtrls.hpp>
//   #include "uSocketServerClient.h"   //JimmyChiu 20220219 : 殘料檢查 From Loader
//   #include "myTimer.h"
//   #include "HTray.h"                 //JimmyChiu 20220219 : 殘料檢查 From Loader
// =============================================================================
#ifndef TfAOILaserScanH
#define TfAOILaserScanH
//---------------------------------------------------------------------------
#include "vclcompat/vcl_compat.h"       // AnsiString, TStringList (was <StdCtrls.hpp>)
#include "EJ1N/uSocketServerClient.h"   // uSocketClient (was bare "uSocketServerClient.h")
#include "myTimer.h"                    // TQPF_Timer
#include <functional>                   // TPointVoidShowMsg -- see banner ADAPTATION note

// GATE (1) -- golden `#include "HTray.h"` (golden :9).  TTMyTray is a VCL
// custom-control widget with NO compiled definition anywhere in this port
// tree (see banner).  Forward declaration is sufficient for every TTMyTray*
// use in THIS header; the .cpp gates the few call sites that dereference one.
class TTMyTray;
//---------------------------------------------------------------------------
//AI(W906-PT-W3) 20260807: golden `typedef void (__closure *TPointVoidShowMsg)
//  (AnsiString);` (golden :11) -- see banner ADAPTATION note.
typedef std::function<void(AnsiString)> TPointVoidShowMsg;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TAOISocket                                                                //Jimmychiu 20240322 : Add for aoi connect class
{
private:    // User declarations
    uSocketClient *socketClient;
    TPointVoidShowMsg RecordMsg;
    int iCommandTask;
    bool bflag;
public:     // User declarations
    TAOISocket();
    ~TAOISocket();
    AnsiString DoSendCommand(bool bIsFirst,AnsiString asCommand);
    void ReceiveData(char* cGet,int iLen);
    void SendCommandSocket(AnsiString asCommand);
    bool DoCommuncation();
    bool IsCommActive();
    void SetRecordMsg(TPointVoidShowMsg t)
    {
        RecordMsg=t;
    }
    AnsiString asReceiveMsg;
    AnsiString GetSocketAddress(){return socketClient->GetSocketAddress();}
    AnsiString GetSocketPort(){return socketClient->GetSocketPort();}
    bool SetCommParameter(AnsiString asAddress,AnsiString asPort){return socketClient->SetCommParameter(asAddress,asPort);}
    void Close(){socketClient->Close();}
};
//---------------------------------------------------------------------------
class TfAOI                                                                     //JimmyChiu 20220312 add AOI base class
{
private:    // User declarations
    bool bflag;
    int iCommandDelayTime;
    AnsiString asTempStr;
    int iMovetrack;
public:     // User declarations
    TfAOI(AnsiString asname);
    TfAOI();
    ~TfAOI();
    enum eAOI_Command
    {
        eAOI_GetVersion=0,
        eAOI_ClearBuffer,
        eAOI_GetFileName,
        eAOI_LoadFile,
        eAOI_Reset,
        eAOI_GrabIC,
        eAOI_GetValueIC,
        eAOI_SetLot,
        eAOI_CreateFile,
        eAOI_SetGoldenImage,
        eAOI_GetStatus,
        eAOITotal
    };
    enum eMoveTrack
    {
        eMT_Loader=0,
        eMT_Empty,
        eMT_Color,
        eMT_Auto1,
        eMT_Auto2,
        eMT_Auto3,
        eMTTotal
    };
    TAOISocket  *socketAOI;
    TStringList *tsAnalysis;
    int iRow,iCol;
    int CommandDelay(){return iActiveDelayTime;}
    int iLD_TrayArrivalDely(){return 4;}
    int LoaderCarEdgePushDelay(){return 1;}
    //common cmd
    AnsiString GetLoadFileName();
    AnsiString GetLotMO();
    AnsiString GetErrorMsg(AnsiString asFunName,AnsiString asErrorType,AnsiString asContent);
    AnsiString GetAOIReturnError(){return "AOI return Error";}
    AnsiString GetReceiveWrongCommand(){return "Receive wrong command";}
    AnsiString GetTrayData();
    TPointVoidShowMsg RecordMsgLaser;
    TPointVoidShowMsg RecordErrorMsgLaser;
    //<==socket Ctrl
    AnsiString asSendMsg;
    AnsiString asReceiveMsg;
    uSocketClient *socketClient;
    AnsiString asName;
    TQPF_Timer AOIDelay;
    TQPF_Timer htDetectICFloatingDelay;
    TQPF_Timer ttLoaderCarEdgePushDelay;
    TQPF_Timer TrayArrivalDelay;
    AnsiString DoSendCommand(bool bIsFirst,AnsiString asCommand);
    void ReceiveData(char* cGet,int iLen);
    void SendCommandSocket(AnsiString asCommand);
    //<==socket Ctrl
    int iTDCTask;
    int iCommandTask;
    bool bSimulationAOICommand;
    AnsiString GetAOI_Command(eAOI_Command eaoi);
    bool DoCommuncation();
    bool IsCommActive();
    AnsiString GetXC_YC();
    virtual TTMyTray* GetTrayIC(){return NULL;}
    virtual bool DoSettingBeforeMotorMove(bool bIsFirst,AnsiString &asError){return true;}
    virtual bool DoGetValueAfterMotorArrival(bool bIsFirst,AnsiString &asError){return true;}
    //auto tuning
    bool DoAutoTuning(bool bIsFirst,AnsiString &asError);
    //single command s
    virtual bool DoAOI_LoadFile(bool bIsFirst){return true;}
    virtual bool DoAOI_Function(bool bIsFirst, int iIndex=0){return true;}        //RogerYang 20250830 : 殘料檢變更格式
    virtual bool DoGetValue(bool bIsFirst){return true;}
    virtual bool DoAOI_ClearBuffer(bool bIsFirst){return true;}
    bool DoSetLot(bool bIsFirst);
    virtual bool DoCreateFile(bool bIsFirst){return true;}
    virtual bool DoGoldenImage(bool bIsFirst, int index=0){return true;}          //RogerYang 20250830 : 殘料檢變更格式
    virtual bool DoGetStatus(bool bIsFirst){return true;}
    //single command e
    virtual bool IsRemainIC(){return true;}
    virtual bool HasNullIC(TTMyTray* mTray){return false;}
    bool DoMoveOut(bool bIsFirst,AnsiString &asError);
    bool DoMoveIn(bool bIsFirst,AnsiString &asError);
    void SetMoveTrack(int iTrack);
    int  GetMoveTrack();
    bool GetInSensor();
    bool GetOutSensor();
    AnsiString GetName(){return asName;}
    bool bNeedCheckInitialStart;
    bool bRealRunCheckInitialStart;
    bool bEnableRetryCount;
    int  iRemainICLaserRetryCount;
    bool bRemainICLaserAutoSkip;
    int iActiveDelayTime;
    int SetAndBackUpLoaderYSpeed(int iLoaderYCWSpeedPerc=80);                   //RogerYang 20250906 : Add for MLoaderY
    void RestoreLoaderYSpeedRaw(int iRawSpeed);                                 //AI(ht9045-v899) 20260505: 還原 raw speed (與 SetAndBackUpLoaderYSpeed 配對)
    virtual void CloneTrayData(TTMyTray* sourceT,TTMyTray* targetT){}
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TfTrayDeviceCheckFromLoader : public  TfAOI             //JimmyChiu 20220312 add class
{
private:    // User declarations
    bool bIsRemainIC;
    AnsiString GetFunctionCode(){return "5";}
public:     // User declarations
    TfTrayDeviceCheckFromLoader(AnsiString asname);
    ~TfTrayDeviceCheckFromLoader();
    bool IsRemainIC(){return bIsRemainIC;}
    bool DoSettingBeforeMotorMove(bool bIsFirst,AnsiString &asError);
    bool DoGetValueAfterMotorArrival(bool bIsFirst,AnsiString &asError);
    bool DoAOI_LoadFile(bool bIsFirst);
    bool DoAOI_Function(bool bIsFirst, int iIndex=0);                           //RogerYang 20250830 : 殘料檢變更格式
    bool DoGetValue(bool bIsFirst);
    bool DoAOI_ClearBuffer(bool bIsFirst);
    bool DoGoldenImage(bool bIsFirst, int iIndex);                              //RogerYang 20250830 : 殘料檢變更格式
    bool DoGetStatus(bool bIsFirst);
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TfTrayMapFromLoader : public  TfAOI             //JimmyChiu 20220312 add class
{
private:    // User declarations
    TTMyTray* trayMapIC;
    int itemp;
    void AnalysisRowCol(AnsiString asInput,int &icow,int &irow);
    AnsiString GetFunctionCode(){return "4";}
public:     // User declarations
    TfTrayMapFromLoader(AnsiString asname);
    ~TfTrayMapFromLoader();
    void AnalysisTrayData(TTMyTray* mTray,AnsiString asRawData);
    TTMyTray* GetTrayIC(){return trayMapIC;}
    bool HasNullIC(TTMyTray* mTray);
    void SetTrayIC(TTMyTray* _value){trayMapIC=_value;}
    bool DoSettingBeforeMotorMove(bool bIsFirst,AnsiString &asError);
    bool DoGetValueAfterMotorArrival(bool bIsFirst,AnsiString &asError);
    bool DoAOI_LoadFile(bool bIsFirst);
    bool DoAOI_Function(bool bIsFirst, int iIndex=0);                           //RogerYang 20250830 : 殘料檢變更格式
    bool DoGetValue(bool bIsFirst);
    bool DoAOI_ClearBuffer(bool bIsFirst);
    bool DoCreateFile(bool bIsFirst);
    bool DoGoldenImage(bool bIsFirst, int iIndex);                              //RogerYang 20250830 : 殘料檢變更格式
    bool DoGetStatus(bool bIsFirst);
};
//---------------------------------------------------------------------------
#endif
