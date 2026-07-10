// =============================================================================
//  BarCode/BarCode_Helpers.cpp  --  smallest W5-BarCode unit: 2 small helper
//                                   classes + 1 TfBarCode_Shim method body
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/BarCode/BarCode.cpp
//    TfHIKROBOTcmd::GetStartCmd/GetAnalysisResult        :11568-11588
//    uCCDUnloaderClip ctor/dtor/methods                  :11591-11719
//    TfBarCode::IsSHT2DIDScanFinish                      :9097-9152
//  Translation wave: W5-BarCode (BarCode front, parallel translate batch)
//  Translator: AI(W5-BarCode-Helpers-Translate) 20260711
//
//  See BarCode_Helpers.h for scope notes (this is NOT BarCode.cpp/.h as a
//  whole -- see MIGRATION_ROADMAP.md DEFERRED table for the ~24,500-line
//  remainder).
//
//  Gated hardware dependency (EJ1N::uSocketClient / TClientSocket, out of
//  scope -- see header banner): every `socketClient->` call site below is
//  individually wrapped `#if 0 // TODO(W5-BarCode-Helpers-socket) golden
//  BarCode.cpp:<line> ...` with an ACTIVE conservative-default replacement
//  alongside it, exactly per the surrounding control-flow/state-machine logic
//  translating faithfully in full (only the socket I/O call itself is gated).
// =============================================================================

#include "BarCode_Helpers.h"
#include "aHotPlateSubstrate.h"   // FLCarryKit / BLCarryKit (TMyKitSuck)
#include "cmydef.h"               // HAS_IC / HAS_HOT_IC

//---------------------------------------------------------------------------
// (1) TfHIKROBOTcmd -- golden BarCode.cpp:11568-11588. Pure string builders,
// no gating needed.
//---------------------------------------------------------------------------
AnsiString TfHIKROBOTcmd::GetStartCmd()
{
    AnsiString sret="start";
    return sret;
}
//---------------------------------------------------------------------------
AnsiString TfHIKROBOTcmd::GetAnalysisResult(AnsiString sreceive)
{
    AnsiString result = "";
    int lfPos = sreceive.Pos("LF");
    if (lfPos > 0)
    {
        AnsiString temp = sreceive.SubString(lfPos + 2, sreceive.Length());
        int atPos = temp.Pos("@");
        if (atPos > 0)
        {
            result = temp.SubString(1, atPos - 1);
        }
    }
    return result;
}

//---------------------------------------------------------------------------
// (2) uCCDUnloaderClip -- golden BarCode.cpp:11591-11719.
//---------------------------------------------------------------------------
uCCDUnloaderClip::uCCDUnloaderClip()
{
    InitialDatas();
    //AI(W5-BarCode-Helpers-Translate) 20260711: golden BarCode.cpp:11594-11595
    // `socketClient=new uSocketClient(); socketClient->SetReceiveFunc(ReceiveData);`
    // gated -- EJ1N::uSocketClient (EJ1N/uSocketServerClient.h) is out of scope
    // for this machine's config (KNOWLEDGE.md) and has no vclcompat shim.
    // Conservative default: no live socket offline.
#if 0 // TODO(W5-BarCode-Helpers-socket) golden BarCode.cpp:11594-11595 -- needs
      // a vclcompat EJ1N::uSocketClient shim (not yet built).
    socketClient=new uSocketClient();
    socketClient->SetReceiveFunc(ReceiveData);
#else
    socketClient=NULL;
#endif
}
//---------------------------------------------------------------------------
uCCDUnloaderClip::~uCCDUnloaderClip()
{
    //AI(W5-BarCode-Helpers-Translate) 20260711: golden BarCode.cpp:11600
    // `delete socketClient;` gated -- socketClient is always NULL offline (see
    // ctor above), so there is nothing to delete; kept gated (not just deleted
    // unconditionally) so the golden line is still visible/traceable once the
    // EJ1N shim lands.
#if 0 // TODO(W5-BarCode-Helpers-socket) golden BarCode.cpp:11600 -- delete the
      // real socketClient once the EJ1N shim exists.
    delete socketClient;
#endif
}
//---------------------------------------------------------------------------
void uCCDUnloaderClip::ReceiveData(char* cGet, int iLen)
{
    // FAITHFUL -- pure buffer/string logic, no socket-> calls of its own. Dead
    // in practice offline (never invoked: the golden registration path
    // `socketClient->SetReceiveFunc(ReceiveData)` is gated above), but
    // translated in full per the "translate everything else in the class
    // faithfully" instruction.
    asReceiveMsg=AnsiString(cGet);
    if(asReceiveMsg.Length()>iLen)
        asReceiveMsg=asReceiveMsg.SubString(0,iLen);
    asReceiveMsg=TfHIKROBOTcmd().GetAnalysisResult(asReceiveMsg);
    if(RecMsgCCDUnloaderClip)
    {
        RecMsgCCDUnloaderClip("[Receive]"+asReceiveMsg);
    }
}
//---------------------------------------------------------------------------
void uCCDUnloaderClip::SendCommandSocket(AnsiString asCommand)
{
#if 0 // TODO(W5-BarCode-Helpers-socket) golden BarCode.cpp:11617-11619 -- needs
      // EJ1N::uSocketClient::IsConnected()/DoCommuncation() lazy-(re)connect.
    if(socketClient->IsConnected()==false)
    {
        DoCommuncation();
    }
#endif
    asReceiveMsg="";
#if 0 // TODO(W5-BarCode-Helpers-socket) golden BarCode.cpp:11622 -- needs
      // EJ1N::uSocketClient::SendCommand (actual socket write).
    bflag=socketClient->SendCommand(asCommand.c_str(),asCommand.WideCharBufSize());
#else
    bflag=false;    // conservative default: no live socket offline -> send "fails"
#endif
    if(RecMsgCCDUnloaderClip)
    {
        if(bflag)
            RecMsgCCDUnloaderClip("[Send]"+asCommand);
        else
        {
            //AI(W5-BarCode-Helpers-Translate) 20260711: golden BarCode.cpp:11629
            // reads `socketClient->GetSocketAddress()`/`GetSocketPort()` (gated,
            // out of scope). Substituted with `sIP`/`sPort` -- the SAME values
            // the class itself last forwarded into SetCommParameter (golden
            // BarCode.cpp:11547 `ccdUnloader[i].SetCommParameter(ccdUnloader[i]
            // .sIP,ccdUnloader[i].sPort)`), so this is not a behavior guess: it
            // reports the address/port this instance was configured with.
#if 0 // TODO(W5-BarCode-Helpers-socket) golden BarCode.cpp:11629 -- restore the
      // live socketClient->GetSocketAddress()/GetSocketPort() reads once the
      // EJ1N shim exists.
            RecMsgCCDUnloaderClip("[Send Error]"+asCommand+"[IP]"+socketClient->GetSocketAddress()+"[Port]"+socketClient->GetSocketPort());
#else
            RecMsgCCDUnloaderClip("[Send Error]"+asCommand+"[IP]"+sIP+"[Port]"+sPort);
#endif
        }
    }
    Sleep(5);
}
//---------------------------------------------------------------------------
void uCCDUnloaderClip::SendCommandPhoto()
{
    if(bSimulationCommand)
        return;
    SendCommandSocket(TfHIKROBOTcmd().GetStartCmd());
}
//---------------------------------------------------------------------------
void uCCDUnloaderClip::SendCommandStr(AnsiString sMsg)
{
    //AI(W5-BarCode-Helpers-Translate) 20260711: FAITHFUL missing-semicolon
    // quirk preserved -- golden BarCode.cpp:11642-11647:
    //     if(bSimulationCommand)
    //         return
    //     SendCommandSocket(sMsg);
    // has NO semicolon after `return`, so BCB6's (and standard C++'s) grammar
    // folds the two source lines into ONE statement: `return
    // SendCommandSocket(sMsg);` (legal: SendCommandSocket returns void, and
    // `return void-expr;` from a void function is well-formed). The `if`
    // therefore has exactly that one statement as its body, and the function
    // ends right after -- there is NO unconditional trailing call. Net effect
    // (inverted from what the source LAYOUT visually suggests): when
    // bSimulationCommand==true this DOES call SendCommandSocket(sMsg) (as the
    // return-expression); when bSimulationCommand==false the if's body is
    // skipped and the function does NOTHING -- SendCommandSocket is never
    // invoked. Do not "fix" this; it is the exact golden-compiled behavior,
    // reproduced here as the equivalent explicit if/return.
    if(bSimulationCommand)
    {
        SendCommandSocket(sMsg);
        return;
    }
}
//---------------------------------------------------------------------------
bool uCCDUnloaderClip::SetCommParameter(AnsiString /*asAddress*/, AnsiString /*asPort*/)
{
#if 0 // TODO(W5-BarCode-Helpers-socket) golden BarCode.cpp:11651 -- needs
      // EJ1N::uSocketClient::SetCommParameter (real TClientSocket address/port set).
      // Params named asAddress/asPort again once un-gated (see the golden call
      // below and BarCode_Helpers.h's declaration).
    return socketClient->SetCommParameter(asAddress,asPort);
#else
    return true;    // conservative default: accept the parameter offline (no live socket to configure)
#endif
}
//---------------------------------------------------------------------------
bool uCCDUnloaderClip::DoCommuncation()
{
    if(bSimulationCommand)
        return true;
#if 0 // TODO(W5-BarCode-Helpers-socket) golden BarCode.cpp:11658 -- needs
      // EJ1N::uSocketClient::DoOpenCommuncation (real TClientSocket open).
    return socketClient->DoOpenCommuncation();
#else
    return false;   // conservative default: no live socket offline -> "connect" fails
#endif
}
//---------------------------------------------------------------------------
void uCCDUnloaderClip::DoSocketClose()
{
    if(bSimulationCommand)
        return;
#if 0 // TODO(W5-BarCode-Helpers-socket) golden BarCode.cpp:11665 -- needs
      // EJ1N::uSocketClient::Close (real TClientSocket close).
    socketClient->Close();
#endif
}
//---------------------------------------------------------------------------
const int iMaxRetryConn=10;
AnsiString uCCDUnloaderClip::DoGetPhotoCmd(bool bReset, AnsiString &sErr)
{
    // FAITHFUL -- calls only DoCommuncation()/SendCommandPhoto() (already
    // gated internally above); no socket-> call site of its own.
    if(bReset)
    {
        iCommandTask=1;
        sErr="";
        iRetryConn=0;
        bGetResult=false;
        return "";
    }
    switch(iCommandTask)
    {
        case 1:
            //send command
            if(DoCommuncation())
            {
                SendCommandPhoto();
                iCommandTask=100;
            }
            else
            {
                sErr=AnsiString().sprintf("Socket connection failed.IP:%s Port:%s",sIP,sPort);
            }
            break;
        case 100:
            if(bSimulationCommand)
            {
                asReceiveMsg=AnsiString("SimulateCode");
            }

            if(asReceiveMsg=="")
            {
                if(iRetryConn>iMaxRetryConn)
                {
                    iRetryConn=0;
                    sErr="Socket data reception failed.";
                }
                else
                {
                    iRetryConn++;
                }
            }
            else
            {
                bGetResult=true;
                return asReceiveMsg;
            }
            break;
    }
    return "";
}

//---------------------------------------------------------------------------
// (3) BarCode_IsSHT2DIDScanFinish -- golden TfBarCode::IsSHT2DIDScanFinish,
// BarCode.cpp:9097-9152. Pure read over FLCarryKit/BLCarryKit; zero
// VCL/socket coupling, FULL real body (no gate needed).
//---------------------------------------------------------------------------
bool BarCode_IsSHT2DIDScanFinish(int SHT)                                       //JerryYang 20200916 : 判斷是否掃完2D
{
    bool bResult=true;

    if(SHT==0)
    {
        if(FLCarryKit.HasRealIC())
        {
            for(int i=0; i<FLCarryKit.iShtRow; i++)
            {
                for(int j=0; j<FLCarryKit.iShtCol; j++)
                {
                    if(FLCarryKit.Item[i][j]==HAS_IC ||
                       FLCarryKit.Item[i][j]==HAS_HOT_IC)
                    {
                        if(FLCarryKit.cDeviceInf[i][j]==""  ||
                           FLCarryKit.cDeviceInf[i][j]=="0")
                        {
                            bResult=false;
                        }
                    }
                }
            }
        }
        else
        {
            bResult=true;                                                     //JerryYang 20230620 : 沒有real ic就不用掃2D
        }
    }
    else if(SHT==1)
    {
        if(BLCarryKit.HasRealIC())
        {
            for(int i=0; i<BLCarryKit.iShtRow; i++)
            {
                for(int j=0; j<BLCarryKit.iShtCol; j++)
                {
                    if(BLCarryKit.Item[i][j]==HAS_IC ||
                       BLCarryKit.Item[i][j]==HAS_HOT_IC)
                    {
                        if(BLCarryKit.cDeviceInf[i][j]==""  ||
                           BLCarryKit.cDeviceInf[i][j]=="0")
                        {
                            bResult=false;
                        }
                    }
                }
            }
        }
        else
        {
            bResult=true;                                                     //JerryYang 20230620 : 沒有real ic就不用掃2D
        }
    }
    // FAITHFUL golden gap: no trailing `else` for SHT values other than 0/1 --
    // bResult keeps its initial `true` (golden BarCode.cpp:9099/9151).
    return bResult;
}
