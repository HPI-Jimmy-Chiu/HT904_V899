// =============================================================================
//  forms/fAirCon.cpp  --  definitions for the fAirCon facade
//
//  AI(W906-FW3-TIF1) 20260828: new file, FW wave FW3-TIF1.
//  GOLDEN SOURCE: HT9011UC_Code_V3.33.906.0_20260618/cAirCon.cpp (197 lines)
//  + cAirCon.h (69 lines), read with `io.open(p, encoding='cp950')`.
//  SPAN: 16 golden `TfAirCon::` member bodies, 164 span lines
//  (tools/census/wave_preflight.py, 20260828).
//  THIS WAVE: 5 of 16 members ACTIVE (31.3%) / 36 of 164 golden span lines
//  (22.0%); 11 members GATED (bodies translated below inside `#if 0`).
//
//  See forms/fAirCon.h for the full GATE REGISTER (A-1)..(A-11), the ACTIVE
//  reachability evidence, DEVIATIONS D-1..D-7, the two preserved GOLDEN BUGS
//  and the absence-claim provenance. The short version of the register:
//
//    (A-1)  SendCommand         wire write  (CommAirCon->WriteCommData)
//    (A-2)  InitialAirConCom    opens COM   (CommAirCon->StartComm)
//    (A-3)  StartOutput         transitive on (A-1)
//    (A-4)  SetCurrent          transitive on (A-1)
//    (A-5)  SetCLS              transitive on (A-1)
//    (A-6)  GetStatus           transitive on (A-1)
//    (A-7)  GetCurrent          transitive on (A-1)
//    (A-8)  PowerUp             SW[SwAirConditioner].On()  -- hardware AND
//                               link boundary (myswitch.cpp = ht9045_io,
//                               CMakeLists.txt:1240; ht9045_forms links only
//                               vclcompat/ht9045_globals/ht9045_core, :935-942)
//    (A-9)  PowerDown           same as (A-8), .Off()
//    (A-10) sbUpdateClick       transitive: every switch arm is (A-3)..(A-7)
//    (A-11) FormDestroy         transitive (A-9) + StopComm + LogSoftwareOffTime
//                               (acarry_shims.cpp = ht9045_sm, :2054; NOT one
//                               of the four verified forms->sm exceptions)
// =============================================================================
#include "forms/fAirCon.h"
#include "forms/fQwertyKey.h"   // fQwertyKey->ShowQwertyKey (real, forms/fQwertyKey.cpp, ht9045_forms)
#include "cmydef.h"             // N_DOUBLE (cmydef.cpp, ht9045_globals)

#include <cstdio>               // sprintf  (golden :92, ACTIVE CommAirConReceiveData)

// AI(W906-FW3-TIF1) 20260828: TfAirCon/fAirCon were FREE tree-wide -- same
// idiom as forms/fPrecaution.cpp:14 / forms/fHotPlate.cpp:24. golden's ctor
// body (:19-20) is EMPTY past `: TForm(Owner)`, so this static-init `new`
// touches nothing beyond its own already-`new`'d widget stand-ins -- no SIOF
// risk (docs/KNOWLEDGE.md "static-init ctor 不可碰 NULL 全域").
TfAirCon *fAirCon = new TfAirCon();

TfAirCon::TfAirCon()
{
    // golden :17-20 -- empty ctor body past `: TForm(Owner)`. Widget fields
    // are NSDMI-`new`'d in the header.
}

// ---------------------------------------------------------------------------
void TfAirCon::AddMessage(AnsiString Str)   // golden :22-28  ACTIVE
{
    if(Memo1->Lines->Count>1024)
        Memo1->Clear();

    Memo1->Lines->Add(Str);
}

// ---------------------------------------------------------------------------
void TfAirCon::sbtExitClick(TObject *Sender)   // golden :30-33  ACTIVE
{
    (void)Sender;
    Close();   // DEVIATION D-2 -- port-only no-op
}

// ---------------------------------------------------------------------------
// golden :79-94  ACTIVE.  Two GOLDEN BUGS preserved verbatim, see forms/
// fAirCon.h (B-1) void* passed to a %s conversion, (B-2) the formatted cStr
// is built then discarded and the raw cPtr logged instead.
void TfAirCon::CommAirConReceiveData(TObject *Sender, void *Buffer, Word BufferLength)
{
    (void)Sender;
    char *cPtr=(char *)Buffer;
    char cStr[512];

    if(BufferLength>512)                                                        //JimmyChiu 20210922 Avoid overflow
    {
        LogMsg.sprintf("Receive (%d): %s", BufferLength, Buffer);               // GOLDEN BUG (B-1), verbatim
        AddMessage(LogMsg);
        return;
    }

    sprintf(cStr, "Read  : %s", cPtr);
    AddMessage(cPtr);                                                           // GOLDEN BUG (B-2), verbatim: cStr built then discarded
    (void)cStr;                                                                 // port-only: silences -Wunused-but-set-variable that (B-2) would otherwise raise
}

// ---------------------------------------------------------------------------
void TfAirCon::edOutputCurrentMouseDown(TObject *Sender, TMouseButton Button,
                                        TShiftState Shift, int X, int Y)   // golden :191-195  ACTIVE
{
    (void)Button; (void)Shift; (void)X; (void)Y;
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 1, true, 5.8, 0.0);
}

// ===========================================================================
//  GATE REGISTER -- translated golden bodies, deliberately NOT COMPILED.
//
//  Each block below is golden's own text (cp950 -> UTF-8, comments verbatim)
//  with only the port-mechanical edits noted per block. They are declared in
//  forms/fAirCon.h, so calling one is a LINK ERROR by design; the text is
//  kept so a future wave that is allowed a write path / an ht9045_io edge
//  does not have to re-translate from scratch.
//
//  ⚠ THESE BLOCKS HAVE NEVER BEEN COMPILED. Anything inside `#if 0` is not
//  even parsed beyond preprocessor tokens, so treat them as a faithful
//  TRANSCRIPT, not as verified code -- in particular `CommAirCon` (golden
//  `TComm*`) is NOT a member of this facade (see forms/fAirCon.h FIELD LIST),
//  so un-gating (A-1)/(A-2)/(A-11) requires adding it first.
// ===========================================================================

#if 0 // GATE (A-10) sbUpdateClick -- golden :35-65. TRANSITIVE: every switch
      // arm dispatches into a gated wire-writing method (A-3)..(A-7).
void TfAirCon::sbUpdateClick(TObject *Sender)
{
    (void)Sender;
    if(IniConfig.bA02DisableSaveParsWhenSwitchToOp==true &&                     //RogerYang 20260305 張寧說時間到都要登出，且不可被更改任何設定(A01_2)
        AccessLevel==0)
    {
        ShowMyMessage("[A01_2]目前已切換到Operator權限，\r\n請重新登入再做設定!");
        Close();
    }

    switch(rgAirConCommand->ItemIndex)
    {
        case eCLS:                                                              //致能輸出電壓
            SetCLS();
            break;
        case ePwrOn:                                                            //開啟輸出電壓
            StartOutput(true);
            break;
        case ePwrOff:                                                           //關閉輸出電壓
            StartOutput(false);
            break;
        case ePwrSts:                                                           //回傳power supply狀態
            GetStatus();
            break;
        case eSetCur:                                                           //設定電壓電流值
            SetCurrent(atof(edOutputCurrent->Text.c_str()));
            break;
        case eGetCur:                                                           //讀取電流
            GetCurrent();
            break;
    }
}
#endif // GATE (A-10)

#if 0 // GATE (A-1) SendCommand -- golden :67-77. SENDS ON THE WIRE
      // (CommAirCon->WriteCommData). vclcompat::TComm is REAL and reachable
      // from ht9045_forms -- this gate is the wave's SAFETY POLICY, not a
      // missing symbol.
void TfAirCon::SendCommand(AnsiString Str)
{
    AnsiString s;
    char cStr[256];

    s.sprintf("Write : %s", Str);
    sprintf(cStr,"%s%c", Str, char(0x0A));

    AddMessage(s);
    CommAirCon->WriteCommData(cStr, strlen(cStr));
}
#endif // GATE (A-1)

#if 0 // GATE (A-2) InitialAirConCom -- golden :96-120. OPENS A REAL COM PORT
      // (CommAirCon->StartComm). golden's `#ifdef HT9045` two-arm
      // ShowMyMessage is kept verbatim; both arms bind to canary_support.h's
      // `ShowMyMessage(AnsiString S1, AnsiString S2="", ...)` default.
void TfAirCon::InitialAirConCom(AnsiString ComPort, Spcomm::TParity Parity, unsigned Rate,
                                Spcomm::TByteSize ByteSize, bool ParityCheck, Spcomm::TStopBits StopBits)
{
    CommAirCon->CommName       = "\\\\.\\"+ComPort;
    CommAirCon->Parity         = Parity;
    CommAirCon->BaudRate       = Rate;
    CommAirCon->ByteSize       = ByteSize;
    CommAirCon->ParityCheck    = ParityCheck;
    CommAirCon->StopBits       = StopBits;
    if(GetCOMPortStatus(ComPort))
    {
        try
        {
            CommAirCon->StartComm();                                            //僅能啟動一次
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TfAirCon::InitialAirConCom");
            #ifdef HT9045
                ShowMyMessage("Air Con : "+ComPort+" port error", "");
            #else
                ShowMyMessage("Air Con : "+ComPort+" port error");
            #endif
        }
    }
}
#endif // GATE (A-2)

#if 0 // GATE (A-11) FormDestroy -- golden :122-137. (a) transitive on (A-9)
      // PowerDown, (b) CommAirCon->StopComm closes a real port, (c)
      // LogSoftwareOffTime's only body is acarry_shims.cpp:255 = ht9045_sm.
void TfAirCon::FormDestroy(TObject *Sender)
{
    (void)Sender;
    try
    {
        if(USE_AIR_CONDITIONER==2)
        {
            PowerDown();
            CommAirCon->StopComm();
        }
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfAirCon::FormDestroy");
    }
    LogSoftwareOffTime("TfAirCon, FormDestroy");                                //Steven 20210526 : 紀錄軟體執行時間
}
#endif // GATE (A-11)

#if 0 // GATE (A-3) StartOutput -- golden :139-145. TRANSITIVE on (A-1).
void TfAirCon::StartOutput(bool bEnable)                                        //開啟輸出電壓
{
    if(bEnable)
        SendCommand(":OUTPut:STATe 1");
    else
        SendCommand(":OUTPut:STATe 0");
}
#endif // GATE (A-3)

#if 0 // GATE (A-4) SetCurrent -- golden :147-153. TRANSITIVE on (A-1).
      // CheckRange is the MachineType.h:1525 template and is NOT what gates
      // this; note golden's own argument order is (Value, Maximum, Minimum).
void TfAirCon::SetCurrent(double dCurrent)                                      //設定電流
{
    AnsiString Str;
    dCurrent=CheckRange(dCurrent, 5.8, 0.0);                                    //最大輸出為5.8A
    Str.sprintf(":CHAN1:VOLT 00.00;CURR %5.2f", dCurrent);
    SendCommand(Str);
}
#endif // GATE (A-4)

#if 0 // GATE (A-5) SetCLS -- golden :155-158. TRANSITIVE on (A-1).
void TfAirCon::SetCLS()                                                         //致能輸出電壓
{
    SendCommand("*CLS");
}
#endif // GATE (A-5)

#if 0 // GATE (A-6) GetStatus -- golden :160-163. TRANSITIVE on (A-1).
void TfAirCon::GetStatus()                                                      //讀取Power狀態
{
    SendCommand(":OUTPut:STATe?");
}
#endif // GATE (A-6)

#if 0 // GATE (A-7) GetCurrent -- golden :165-168. TRANSITIVE on (A-1).
void TfAirCon::GetCurrent()                                                     //讀取電流
{
    SendCommand("CHAN1:CURR ?");
}
#endif // GATE (A-7)

#if 0 // GATE (A-8) PowerUp -- golden :170-179. HARDWARE (SW[SwAirConditioner]
      // .On()) AND LINK BOUNDARY (myswitch.cpp = ht9045_io). Pre-committed at
      // csystem.cpp:26877 / :26966 / :26982 / :27004 / :27020.
void TfAirCon::PowerUp(double dCurrent)
{
    if(USE_AIR_CONDITIONER==2)                                                  //Steven 20131011 : 冷氣機
    {
        SetCurrent(dCurrent);
        StartOutput(true);
        SetCLS();
    }
    SW[SwAirConditioner].On();
}
#endif // GATE (A-8)

#if 0 // GATE (A-9) PowerDown -- golden :181-189. Same as (A-8), .Off().
      // Pre-committed at csystem.cpp:26898.
void TfAirCon::PowerDown()
{
    if(USE_AIR_CONDITIONER==2)                                                  //Steven 20131011 : 冷氣機
    {
        SetCurrent(0.0);
        StartOutput(false);
    }
    SW[SwAirConditioner].Off();
}
#endif // GATE (A-9)
