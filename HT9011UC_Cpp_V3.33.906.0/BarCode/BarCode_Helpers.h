// =============================================================================
//  BarCode/BarCode_Helpers.h  --  smallest W5-BarCode unit: 2 small helper
//                                 classes + 1 TfBarCode_Shim method body
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/BarCode/BarCode.h / .cpp
//  Translation wave: W5-BarCode (BarCode front, parallel translate batch)
//  Translator: AI(W5-BarCode-Helpers-Translate) 20260711
//
//  IMPORTANT -- this is NOT a translation of BarCode.h/.cpp as a whole. That
//  pair (+ BarCode_Sh1.cpp / BarCode_Sh2.cpp / BarcodeXML.cpp) is the golden
//  TfBarCode VCL FORM (~24,500 lines total, the single largest golden front --
//  see MIGRATION_ROADMAP.md DEFERRED table) and remains deferred in its
//  entirety except for the pieces this file owns. Named distinctly
//  (BarCode_Helpers, NOT BarCode/BarCode_Sh1/BarCode_Sh2) so nobody mistakes
//  this for a full-file translation and so this unit's file does not collide
//  with the OTHER parallel W5-BarCode translate units reading from the SAME
//  golden BarCode.cpp/BarCode_Sh1.cpp/BarCode_Sh2.cpp files this wave --
//  mirrors the established CCLink/MyCCLinkSensor_predicates.h and
//  Automation/AGV_predicates.h precedent (same project convention).
//
//  Owns exactly 3 things:
//   (1) TfHIKROBOTcmd        -- golden BarCode.h:31-39 / BarCode.cpp:11568-11588.
//                               Stateless string-builder helper (HIK-Robot
//                               vision-camera command protocol), 2 methods.
//   (2) uCCDUnloaderClip     -- golden BarCode.h:41-81 / BarCode.cpp:11591-11719.
//                               Per-Auto/Fix-bin CCD "unloader clip" photo-
//                               trigger socket helper (10 methods). It owns a
//                               `uSocketClient*` (EJ1N/uSocketServerClient.h) --
//                               that module is OUT OF SCOPE for this machine's
//                               config (KNOWLEDGE.md: "本機關閉(出範圍)... Moxa
//                               (EJ1N 2020 改 TComm)") and has NO vclcompat
//                               shim. Every socketClient-> call site is
//                               individually gated `#if 0` with a conservative
//                               offline default (see BarCode_Helpers.cpp for
//                               the exact gate list); every other
//                               constructor/state/non-socket line is FAITHFUL.
//   (3) BarCode_IsSHT2DIDScanFinish(int SHT)
//                             -- golden TfBarCode::IsSHT2DIDScanFinish, one of
//                               the 17 TfBarCode_Shim methods (aHotPlateSubstrate.h:
//                               652) the W6.5 SHUTTLE/CARRY wave stubbed to an
//                               always-`true` offline default. Golden body:
//                               BarCode.cpp:9097-9152. Free function named
//                               distinctly (BarCode_ prefix) so it cannot be
//                               confused with TfBarCode_Shim::IsSHT2DIDScanFinish
//                               itself -- mirrors the AGV_/UseCanBusOrEtherCAT
//                               free-function-vs-shim-method naming precedent.
//                               Pure read over already-translated FLCarryKit/
//                               BLCarryKit (TMyKitSuck, aHotPlateSubstrate.h) +
//                               HAS_IC/HAS_HOT_IC (cmydef.h); zero VCL/socket
//                               coupling, so this one is a full real body (not
//                               a stub-with-gates like uCCDUnloaderClip).
//
//  Dependencies -- ALL already translated / part of ht9045_globals or
//  ht9045_sm's aHotPlateSubstrate, zero NEW shim needed for (3):
//    FLCarryKit / BLCarryKit   -- aHotPlateSubstrate.h:351-352 (extern TMyKitSuck)
//    TMyKitSuck::HasRealIC()/iShtRow/iShtCol/Item[][]/cDeviceInf[][]
//                              -- aHotPlateSubstrate.h:172-244
//    HAS_IC / HAS_HOT_IC       -- cmydef.h:158/160 (extern const int)
// =============================================================================
#ifndef BarCode_HelpersH
#define BarCode_HelpersH

#include "vclcompat/vcl_compat.h"
#include <functional>

// ---------------------------------------------------------------------------
//  golden BarCode.h:28 `typedef void (__closure *TPointVoidShowMsg)(AnsiString);`
//  -- BCB6 bound-method closure has no standard-C++ equivalent syntax; per
//  vclcompat/vcl_compat.h's documented convention ("Where a __closure typedef
//  is needed, translated code should use a std::function-style typedef
//  instead"), translated here as std::function<void(AnsiString)>. Default-
//  constructed std::function is "empty" (operator bool()==false), matching
//  the golden `if(RecMsgCCDUnloaderClip)` null-closure guard semantics.
// ---------------------------------------------------------------------------
typedef std::function<void(AnsiString)> TPointVoidShowMsg;

// =============================================================================
//  (1) TfHIKROBOTcmd -- golden BarCode.h:31-39 (Jimmychiu 20250818 comment:
//      "海康智能讀碼" = "HIKROBOT smart code-reading"). Stateless: default
//      ctor/dtor, 2 pure string-builder methods, no members. FAITHFUL, whole
//      class, verbatim.
// =============================================================================
class TfHIKROBOTcmd
{
private:    // User declarations
public:     // User declarations
    TfHIKROBOTcmd(){}
    ~TfHIKROBOTcmd(){}
    AnsiString GetStartCmd();
    AnsiString GetAnalysisResult(AnsiString sreceive);
};

// =============================================================================
//  (2) uCCDUnloaderClip -- golden BarCode.h:41-81. Per-slot (Auto1/2/3, Fix1/2/3
//      -- see golden eCCD_ReadUnloaderClip, NOT translated here: it indexes an
//      array member of the still-golden TfBarCode form) CCD-unloader-clip photo
//      trigger over a raw TCP socket (HIK-Robot vision camera protocol via
//      TfHIKROBOTcmd above).
//
//      `socketClient` is golden `uSocketClient*` (EJ1N/uSocketServerClient.h,
//      itself a thin wrapper around VCL `TClientSocket`). That header is OUT OF
//      SCOPE for this front (see file banner) -- kept here as an opaque
//      `void*`, always NULL offline, so this class needs no forward declaration
//      of (and never dereferences) the untranslated `uSocketClient` type. Every
//      site that would need it is gated in the .cpp (see there for the golden
//      file:line list) with a conservative "no live socket" default; every
//      other member/method is FAITHFUL.
// =============================================================================
class uCCDUnloaderClip
{
private:
    void       *socketClient;    // golden `uSocketClient*` (EJ1N/uSocketServerClient.h) -- out of scope; opaque, always NULL offline (see .cpp gates)
    bool        bflag;
    void        SendCommandSocket(AnsiString asCommand);
    void        ReceiveData(char* cGet, int iLen);
    bool        bSimulationCommand;
    int         iCommandTask;
public: // User declarations
    uCCDUnloaderClip();
    ~uCCDUnloaderClip();
    void InitialDatas()
    {
        asReceiveMsg="";
        bflag=false;
        #ifdef SOFT_SIMULTE
        bSimulationCommand=true;
        #else
        bSimulationCommand=false;
        #endif
        sIP="";
        sPort="";
        iCommandTask=1;
        iRetryConn=0;
        bGetResult=false;
    }
    int iRetryConn;
    bool bGetResult;
    //socket
    AnsiString asReceiveMsg;
    void SendCommandPhoto();
    bool SetCommParameter(AnsiString asAddress, AnsiString asPort);
    bool DoCommuncation();
    void DoSocketClose();
    void SendCommandStr(AnsiString sMsg);
    AnsiString sIP;
    AnsiString sPort;
    TPointVoidShowMsg RecMsgCCDUnloaderClip;
    AnsiString DoGetPhotoCmd(bool bReset, AnsiString &sErr);
};

// =============================================================================
//  (3) BarCode_IsSHT2DIDScanFinish -- golden TfBarCode::IsSHT2DIDScanFinish
//      (BarCode.cpp:9097-9152, JerryYang 20200916 : 判斷是否掃完2D = "determine
//      whether the 2D [barcode] scan has finished"). SHT==0 checks the front
//      in-shuttle kit (FLCarryKit); SHT==1 (golden `else if`, so any OTHER
//      value falls through and returns the initial bResult=true) checks the
//      rear in-shuttle kit (BLCarryKit). Real IC present at a site with no
//      decoded 2DID/barcode string yet (cDeviceInf=="" or =="0") -> not
//      finished. No real IC anywhere in the kit -> trivially finished (golden
//      JerryYang 20230620 comment: 沒有real ic就不用掃2D = "no real IC means no
//      need to scan 2D").
// =============================================================================
bool BarCode_IsSHT2DIDScanFinish(int SHT);

#endif // BarCode_HelpersH
