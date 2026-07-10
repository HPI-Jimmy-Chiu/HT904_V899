//---------------------------------------------------------------------------
//  Automation/HANA_ART.cpp  --  HANA-ART SRQ protocol helper (uHANA_ART)
//
//  Translation wave: W5 Wave 1 (parallel translate batch)
//  Translator: AI(W5-HanaART-Translate) 20260710
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/Automation/HANA_ART.cpp (928 lines, cp950)
//
//  SCOPE: the whole file (golden's own header calls it "the cleanest file in
//  the recon" -- no VCL widget derefs live IN this class; every VCL/global
//  touch is via fMain / fSCKART / fLotInfo (FormsFacade pattern) or via
//  already-translated globals (Prod/RunInfo/TestIF_File/BinSelect/Temperature/
//  IniConfig/CosFunction/LastSet/TestSocket/s6TrayName/slHanaTrayMap/...)).
//  This TU fills the `W7C2_TfHanaSeam` TU-local stand-in currently living in
//  csystem.cpp (grep that name) -- see the cross-unit note at the bottom of
//  this header comment.
//
//  TRANSLATION RULES (mirrors atester.cpp/aTester_Front.cpp conventions)
//  --------------------------------------------------------------------
//    * Off Borland: __fastcall removed (none present in this TU); AnsiString/
//      TStringList via vclcompat; numeric semantics EXACT (no int/float
//      rewrites, including the golden `int iSite=atof(...)` narrowing casts
//      in DoSetUpInfo -- preserved verbatim, not "fixed" to atoi).
//    * SOFT_SIMULTE is NOT defined in this target build (matches every other
//      translated engine file) -- the one `#ifdef SOFT_SIMULTE` block
//      (SetHandlerWaitingData's Site="HANA" override) is reproduced VERBATIM;
//      the preprocessor selects the "not defined" path today.
//    * Big5 Chinese comments decoded via cp950 (python `open(...,
//      encoding='cp950')`), preserved as correct UTF-8. ZERO U+FFFD.
//    * Golden gap preserved faithfully: `handleError(int)` is declared in
//      HANA_ART.h but has NO body anywhere in golden HANA_ART.cpp and is never
//      called -- faithfully left undefined here too (see HANA_ART.h comment).
//
//  FACADE ADDITIONS NEEDED (do NOT add these myself -- report only, per the
//  FormsFacade convention; this file is WRITTEN AS IF they already exist,
//  matching the established "report the exact method, integrate agent adds
//  it" workflow used by every prior W6/W7 wave). Until the integrate step
//  adds them this TU will not compile stand-alone -- EXPECTED, see task notes.
//  Full signatures/behavior are repeated in the final translate report; short
//  form here for local readability:
//    FormsFacade.h  TfMain   : SendMSG_CMD(int) / SendMSG_CMD(int,AnsiString) /
//                               GetSamSungMap(bool=true) / GetSamSungSoakTime(bool=true) /
//                               ArmStatusStrings()
//    FormsFacade.h  TfSCKART : iCurrent93KARTStep / sLotID / sProcessCode /
//                               DoARTLotStart(AnsiString,AnsiString,int) /
//                               AccessFile(bool,int=-1) / palLotNumber->Caption /
//                               palTestCnt->Caption / palRTTryCnt->Caption /
//                               edlRTTryCnt->Text / pnlProcessCode->Caption
//    FormsFacade.h  TfLotInfo: cbProcess->Text / SetLotID(AnsiString,bool=false) /
//                               SetLotStart(AnsiString,bool=false)
//    canary_support.h LAST_GENERAL_SET: iASEContact (int) / iBinData32[4][260] (int)
//
//  GATED DEPENDENCIES (genuinely missing INFRASTRUCTURE, not simple facade
//  methods -- flagged as dependencies rather than invented, per task rules):
//    1. HHandler2Gpib (golden MessageDef.h `extern MV HHandler2Gpib;`, ~20
//       fields incl. HWND) + MSG_CMD_* constants -- the WM_COPYDATA bridge
//       struct to the external 32-bit GPIB process.  MessageDef.h/.cpp are not
//       translated at all (W5-comms scope, hundreds of MSG_CMD_* constants);
//       this is whole new shared infrastructure, not a one-line facade add.
//       The two SPECIFIC manifest constants this file needs (MSG_CMD_RUN_HANA_ART
//       =158, MSG_CMD_HANA_ART=159 -- golden MessageDef.cpp:179-180) are
//       unambiguous data (like aTester_Front.cpp's local ATC_TYPE_33/35
//       precedent) so they are defined locally below; HHandler2Gpib itself is
//       NOT -- each of its 5 touch sites is individually gated `#if 0 //
//       TODO(W5-comms)` with the surrounding SendMSG_CMD() call left ACTIVE.
//    2. Public/MyStringList.h (TMyStringList) -- only forward-declared
//       (cmydef.h:15; ROADMAP W3 DEFERRED).  `slHanaTrayMap[i]->FirstRow=` /
//       `->AddText()` / `->sLastPathName` / `->sLastNameNoPath` /
//       `->sLastFileName` cannot be compiled against an incomplete type.
//       AddNewTrayHead() and SendTrayMapToFTP() are gated wholesale for this
//       reason (in addition to #3 for the latter).
//    3. ProductionInfo/TfFTP.h (TfFTP FTP client) -- not translated (W5
//       KYECFTP-adjacent scope); plus `class FileInfo` (PathCombin) and
//       `MNetLog()` -- neither has a translated home.  SendTrayMapToFTP() is
//       gated wholesale, returns 0 (matches golden HS_ERR_NoError's numeric
//       value; HS_Function.h's named constant is not translated either).
//    4. common.h's `MyForceDirectories()` declaration is ITSELF inside that
//       file's `#if 0 // TODO(wave-file)` gate (not merely link-incomplete
//       like WriteIniData's real bodies are) -- the identifier is not visible
//       at all today.  UpdateFileNameList()'s single call to it is gated;
//       every other statement in that function is ACTIVE (WriteIniData's
//       AnsiString-overload body IS real/linkable -- verified against
//       common.cpp:1021).
//
//  CROSS-UNIT WIRING (for the integrate agent, NOT done by this unit):
//    * golden main.h:1696 `uHANA_ART* hanaART;` / main.cpp:1500
//      `hanaART=new uHANA_ART();` -- FormsFacade.h's CURRENT `TfMainHanaART`
//      (2-method offline stand-in) is a stopgap for the fact this class did
//      not exist yet.  Now that it does, the natural integrate step is to
//      retype `TfMain::hanaART` to `uHANA_ART*` (dropping `TfMainHanaART`) --
//      NOT done here since FormsFacade.h is a forbidden shared file for this
//      unit.  Symbol-name note: `class uHANA_ART` (this file) and
//      `class TfMainHanaART` (FormsFacade.h) are DIFFERENT types with
//      DIFFERENT names -- no ODR collision either way this lands.
//    * csystem.cpp's `struct W7C2_TfHanaSeam` + `W7C2_fHana_ext` + the
//      `W7C2_HANAART` macro (csystem.cpp:2202-2210) is a 5-method TU-local
//      stand-in (IsContactAvailable/IsPrimeTest/EndPrimeTest/EndReTest/
//      NeedToRT) for exactly this class, used only inside DoART_AfterCleanOut
//      (which is itself dead offline -- see that function's own header
//      comment, csystem.cpp:3654-3659).  Once `fMain->hanaART` is retyped per
//      the bullet above, `W7C2_HANAART` could be redefined to
//      `fMain->hanaART` and the seam struct deleted -- again, csystem.cpp is
//      forbidden to this unit, so this is a report-only observation.
//    * MSG_CMD_RUN_HANA_ART / MSG_CMD_HANA_ART are defined LOCALLY below
//      (values 158/159, golden MessageDef.cpp:179-180).  If another parallel
//      unit in this same W5 batch also translates MessageDef.h/.cpp, its
//      real extern definitions will COLLIDE (duplicate-definition, both
//      `const unsigned int` at namespace scope) with this file's local
//      copies at link time -- flagged explicitly per task instructions.  The
//      integrate step should delete this file's local pair once MessageDef.h
//      lands (both are marked below for easy removal).
//---------------------------------------------------------------------------
#include "HANA_ART.h"

#include "FormsFacade.h"            // fMain / fSCKART / fLotInfo
#include "MachineType.h"            // MAX_SOCKET_TOTAL / MAX_SOCKET_ROW/COL / SOFT_SIMULTE
#include "cprod.h"                  // Prod / RunInfo / TestIF_File / BinSelect / Temperature / IniConfig / CosFunction
#include "cmydef.h"                 // s6TrayName / slHanaTrayMap / iTestBinCount / iTestRunMode / SystemYear../eTrayCount
#include "canary_support.h"         // LastSet (LAST_GENERAL_SET minimal shim) / RecordProcess
#include "aHotPlateSubstrate.h"     // TestSocket (TMyKitSuck) / SetRunStartMode
#include "acatchtray.h"             // ClearAllTrayCount / AddFixTrayCount
#include "common.h"                 // WriteIniData(AnsiString,AnsiString,AnsiString,AnsiString) -- real body, common.cpp:1021

#include <cstdlib>                  // atof/atoi (golden DoSetUpInfo/ParseLOTONStr)
//---------------------------------------------------------------------------

// AI(W5-HanaART-Translate) 20260710: golden MessageDef.cpp:179-180 manifest
// constants (unambiguous data, like aTester_Front.cpp's local ATC_TYPE_33/35
// precedent) -- MessageDef.h/.cpp have no translated home yet (W5-comms,
// hundreds of other MSG_CMD_* constants + the MV/VM structs).  DELETE this
// local pair once MessageDef.h lands for real (see file-header cross-unit note).
#ifndef MSG_CMD_RUN_HANA_ART
static const unsigned int MSG_CMD_RUN_HANA_ART = 158;                          //JimmyChiu 20241023 HANA ART Function
#endif
#ifndef MSG_CMD_HANA_ART
static const unsigned int MSG_CMD_HANA_ART     = 159;                          //JimmyChiu 20241023 HANA ART Function
#endif

//---------------------------------------------------------------------------
uHANA_ART::uHANA_ART()
{
    Clear();
}
//---------------------------------------------------------------------------
uHANA_ART::~uHANA_ART()
{
    Clear();
}
//---------------------------------------------------------------------------
void uHANA_ART::Clear()
{
    timeData.Clear();
    HdWaitData.Clear();
    HdStartData.Clear();
    TestData.Clear();
    bSimuTester=false;
    #ifdef SOFT_SIMULTE
//    bSimuTester=true;
    #endif
}
//---------------------------------------------------------------------------
AnsiString uHANA_ART::GetHanaSRQStr(unsigned int srq)
{
    AnsiString sRet="";
    switch(srq)
    {
        case SETUP_INFORM_REQUEST_SRQ0x55:          sRet="SETUP_INFORM_REQUEST_SRQ";        break;
        case SETUP_INFORM_RECEIVE_OK_SRQ0x56:       sRet="SETUP_INFORM_RECEIVE_OK_SRQ";     break;
        case SETUP_INFORM_RECEIVE_FAIL_SRQ0x57:     sRet="SETUP_INFORM_RECEIVE_FAIL_SRQ";   break;
        case LOT_START_SRQ0x63:                     sRet="LOT_START_SRQ";                   break;
        case LOTON_READ_SUCCESS_SRQ0x51:            sRet="LOTON_READ_SUCCESS_SRQ";          break;
        case LOTON_READ_FAIL_SRQ0x52:               sRet="LOTON_READ_FAIL_SRQ";             break;
        case LOT_END_SRQ0x64:                       sRet="LOT_END_SRQ";                     break;
        case STANDBY_TESTMODE_SRQ0x53:              sRet="STANDBY_TESTMODE_SRQ";            break;
        case PRIME_START_SRQ0x50:                   sRet="PRIMETESTSTARTOK";                break;
        case PRIME_END_SRQ0x54:                     sRet="PRIME_END_SRQ";                   break;
        case NORMAL_START_SRQ0x41:                  sRet="NORMAL_START_SRQ";                break;
        case RETEST_START_SRQ0x65:                  sRet="RETEST_START_SRQ";                break;
        case RETEST_END_SRQ0x66:                    sRet="RETEST_END_SRQ";                  break;
        case DUMMYTEST_START_SRQ0x42:               sRet="DUMMYTEST_START_SRQ";             break;
        case ERROR_START_SRQ0x67:                   sRet="ERROR_START_SRQ";                 break;
        case ERROR_END_SRQ0x68:                     sRet="ERROR_END_SRQ";                   break;
        case ERROR_CLEAR_SRQ0x69:                   sRet="ERROR_CLEAR_SRQ";                 break;
        case AUTO_START_SRQ0x40:                    sRet="AUTO_START_SRQ";                  break;
        case INIT_START_SRQ0x62:                    sRet="INIT_START_SRQ";                  break;
        case LOADER_REQ_SRQ0x61:                    sRet="LOADER_REQ_SRQ";                  break;
        case LOT_INFORM_SRQ0x58:                    sRet="LOT_INFORM_SRQ";                  break;
        case FQATEST_START_SRQ0x71:                 sRet="FQATEST_START_SRQ";               break;
        case FQATEST_END_SRQ0x72:                   sRet="FQATEST_END_SRQ";                 break;
        case WPSOCKET_REQ_SRQ0x73:                  sRet="WPSOCKET_REQ_SRQ";                break;
        case FBIN_MANUAL_REQ_SRQ0x74:                sRet="FBIN_MANUAL_REQ_SRQ";             break;
        case REAL_PARA_SEQ_SRQ0x75:                  sRet="REAL_PARA_SEQ_SRQ";               break;
        case LOT_INFO_METHOD_SRQ0x76:                sRet="LOT_INFO_METHOD_SRQ";             break;
        case RF_ID_SRQ0x77:                          sRet="RF_ID_SRQ";                       break;
        case SBL_SRQ0x78:                            sRet="SBL_SRQ";                         break;
        case AUTODUMPING_SRQ0x79:                    sRet="AUTODUMPING_SRQ";                 break;
        case EQP_MODEL_SRQ0x43:                      sRet="EQP_MODEL_SRQ";                   break;
        case HD_MODE_SRQ0x59:                        sRet="HD_MODE_SRQ";                     break;
        case INLINEUPDATE_SRQ0x60:                   sRet="INLINEUPDATE_SRQ";                break;
        default:                                     sRet="UNKNOWN_SRQ";                     break;
    }
//    RecordProcess(AnsiString().sprintf("Tester ==> HANA ART: Msg=%s"), sRet);
    return sRet;
}
//---------------------------------------------------------------------------
void uHANA_ART::SetHandlerWaitingData(AnsiString Model,
                                      AnsiString Model_NO,
                                      AnsiString Site,
                                      int HD_Mode)
{
    HdWaitData.Model=Model;
    HdWaitData.Model_NO=Model_NO;
    #ifdef SOFT_SIMULTE
    Site="HANA";
    #endif
    HdWaitData.Site=Site;
    HdWaitData.HD_Mode=HD_Mode;
}
//---------------------------------------------------------------------------
void uHANA_ART::DoRunHanaART(bool bRun)
{
    // AI(W5-HanaART-Translate) 20260710: HHandler2Gpib (golden MessageDef.h
    // `extern MV HHandler2Gpib;`) is whole untranslated W5-comms infrastructure
    // (see file-header GATED DEPENDENCIES #1) -- parked, not invented.
#if 0 // TODO(W5-comms): HHandler2Gpib.iLotStatus write (MessageDef.h MV not yet translated)
    HHandler2Gpib.iLotStatus=(bRun)?1:0;
#endif // TODO(W5-comms)
    fMain->SendMSG_CMD(MSG_CMD_RUN_HANA_ART);
}
//---------------------------------------------------------------------------
void uHANA_ART::SendHanaSRQInt(int iCmd)
{
    // AI(W5-HanaART-Translate) 20260710: HHandler2Gpib parked -- see
    // DoRunHanaART note above (file-header GATED DEPENDENCIES #1).
#if 0 // TODO(W5-comms): HHandler2Gpib.iStatus[0]/iLotStatus write (MessageDef.h MV not yet translated)
    HHandler2Gpib.iStatus[0]=iCmd;
    HHandler2Gpib.iLotStatus=1;
#endif // TODO(W5-comms)
    fMain->SendMSG_CMD(MSG_CMD_HANA_ART, GetHanaSRQStr(iCmd));
}
//---------------------------------------------------------------------------
void uHANA_ART::SendHanaCommand(AnsiString sCmd)
{
    // AI(W5-HanaART-Translate) 20260710: HHandler2Gpib parked -- see
    // DoRunHanaART note above (file-header GATED DEPENDENCIES #1).
#if 0 // TODO(W5-comms): HHandler2Gpib.iStatus[0]/iLotStatus write (MessageDef.h MV not yet translated)
    HHandler2Gpib.iStatus[0]=0;
    HHandler2Gpib.iLotStatus=2;
#endif // TODO(W5-comms)
    fMain->SendMSG_CMD(MSG_CMD_HANA_ART, sCmd);
}
//---------------------------------------------------------------------------
//void uHANA_ART::ShowErrMessage(AnsiString sMsg)
//{
//    AnsiString sFlow=GetNow_FTRT_Mode();
//    AnsiString().sprintf("[HanaART][%s]%s", sFlow, sMsg);
//}
//---------------------------------------------------------------------------
AnsiString uHANA_ART::GetNow_FTRT_Mode()
{
    AnsiString sFlow="";
    if(HdStartData.iFT_RT_Mode==HdStartData.frtFT ||
       HdStartData.iFT_RT_Mode==HdStartData.frtFT_LotEnd)
    {
        sFlow="FT";
    }
    else if(HdStartData.iFT_RT_Mode==HdStartData.frtRT)
    {
        sFlow="RT";
    }
    else
    {
        sFlow="Halt";
    }
    return sFlow;
}
//---------------------------------------------------------------------------
AnsiString uHANA_ART::GetHD_ID()
{
    AnsiString sRet=AnsiString().sprintf("%s,%s,%s", HdWaitData.Model, HdWaitData.Model_NO, HdWaitData.Site);
    return sRet;//HT9046,HT9046-01,HANA
}
//---------------------------------------------------------------------------
AnsiString uHANA_ART::GetHD_MAP()
{
    // AI(W5-HanaART-Translate) 20260710: fMain->GetSamSungMap(bool) -- requested
    // FormsFacade addition (golden main.h:1531, body Command.cpp:10137).  Written
    // ACTIVE per the established facade-growth convention (see file-header note).
    return fMain->GetSamSungMap(false);
}
//---------------------------------------------------------------------------
AnsiString uHANA_ART::GetHD_CT()
{
    AnsiString sRet="";
    for(int i=0; i<10; i++)
    {
        int iT3=BinSelect[iTestRunMode].iCatDataT3Pos[i]-1;
        if(iT3>=0 && Prod.bIsPassBin[i])                                        //stack define fail
        {
            sRet+=IntToStr(i);
        }
    }

    if(sRet.Length()<2)
    {
        sRet="CT0"+sRet;
    }
    else
    {
        sRet="CT"+sRet;
    }
    return sRet;
}
//---------------------------------------------------------------------------
AnsiString uHANA_ART::GetHD_Tempset()
{
    AnsiString asWorkTemperBase;
    if(LastSet.iTemperature==Tempture_Hot)
    {
        asWorkTemperBase.sprintf("%0.0f", Temperature.fWorkTemperBase);
    }
    else if(LastSet.iTemperature==Tempture_AmbientHot)
    {
        asWorkTemperBase.sprintf("%0.0f", Temperature.fWorkTemperBase);
    }
    else if(Temperature.bUseAbitCHK)
    {
        asWorkTemperBase.sprintf("%0.0f", Temperature.fAbitTemp);
    }
    else
    {
        asWorkTemperBase.sprintf("25");
    }

    return asWorkTemperBase;
}
//---------------------------------------------------------------------------
AnsiString uHANA_ART::GetHD_Soak()
{
    // AI(W5-HanaART-Translate) 20260710: fMain->GetSamSungSoakTime(bool) --
    // requested FormsFacade addition (golden main.h:1532, body Command.cpp:10305).
    return fMain->GetSamSungSoakTime(false);                                   //Steven 20250414 : HANA ART Function
}
//---------------------------------------------------------------------------
AnsiString uHANA_ART::GetHD_TDATA()
{
    AnsiString sRet="";
    for(int i=0; i<TDATAMaxCount; i++)
    {
        sRet+=IntToStr(timeData.TDATA[i]);
        if(i<(TDATAMaxCount-1))
        {
            sRet+=",";
        }
    }
    return sRet;
}
//---------------------------------------------------------------------------
AnsiString uHANA_ART::GetHD_CDATA()
{
    AnsiString sRet="";
    // AI(W5-HanaART-Translate) 20260710: LastSet.iBinData32[4][260] -- requested
    // addition to canary_support.h's LAST_GENERAL_SET minimal shim (golden
    // LastSet.h:366).  Written ACTIVE per the facade/substrate-growth
    // convention (see file-header note); not compilable stand-alone until the
    // integrate step adds the field (expected, see task notes).
    for(int i=0; i<iTestBinCount; i++)                                          //Steven 20251024 : fixed for CData
    {
        if(i==0)
            sRet+=IntToStr(LastSet.iBinData32[0][i]+LastSet.iBinData32[0][iTestBinCount]);
        else
            sRet+=(AnsiString(",")+IntToStr(LastSet.iBinData32[0][i]));
    }
    return sRet;
}
//---------------------------------------------------------------------------
AnsiString uHANA_ART::GetHD_SDATA()
{
    AnsiString sRet="";
    for(int i=0; i<SDATAMaxCount; i++)
    {
        sRet+=FloatToStrF(timeData.SDATA[i], ffFixed, 15, 2);
        if(i<(SDATAMaxCount-1))
        {
            sRet+=",";
        }
    }
    return sRet;
}
//---------------------------------------------------------------------------
AnsiString uHANA_ART::GetHD_JDATA()
{
    AnsiString sRet="", Str;
    AnsiString sJamPos[JDATAMaxCount]={"IJ", "OJ", "FCJ", "BCJ", "SJ", "FJ", "TTJ", "APJ"};
    for(int i=0; i<JDATAMaxCount; i++)
    {
        Str.sprintf("%s:%d", sJamPos[i], timeData.JDATA[i]);
        sRet+=Str;
        if(i<(JDATAMaxCount-1))
        {
            sRet+=",";
        }
    }
    return sRet;
}
//---------------------------------------------------------------------------
AnsiString uHANA_ART::GetHD_ADATA()
{
    AnsiString sRet="ZZ";
    return sRet;
}
//---------------------------------------------------------------------------
void uHANA_ART::DoSETUP_INFORM()
{
    if(HdStartData.iFT_RT_Mode==HdStartData.frtHALT)
    {
        HdStartData.iFT_RT_Mode=HdStartData.frtFT;
        TestData.iStatus=TestData.etsSetFT;
        SendHanaSRQInt(SETUP_INFORM_REQUEST_SRQ0x55);
    }
}
//---------------------------------------------------------------------------
void uHANA_ART::DoRT_START()
{
    HdStartData.iFT_RT_Mode=HdStartData.frtRT;
    TestData.iStatus=TestData.etsSetRT;
    SendHanaSRQInt(RETEST_START_SRQ0x65);                                            //Steven 20250415 : HANA ART Function 搬完tray才能送
}
//---------------------------------------------------------------------------
void uHANA_ART::DoCmdWhenHDStart(AnsiString sTestCmd)
{
    AnsiString sLotNo="", sMode="";
    int LotSize=0;
    sTestCmd=sTestCmd.Trim();
//    RecordProcess(AnsiString().sprintf("HanaART Cmd=%s", sTestCmd));
    if(sTestCmd=="HDMODEOK")
    {
        HdWaitData.bHD_MODE_STATUS=true;
    }
    else if(sTestCmd=="HDMODENG")
    {
        HdWaitData.bHD_MODE_STATUS=false;
    }
    else if(sTestCmd.Pos("DEVON:")>0)                                           //DEVON:8,25,0,CT01
    {
        if(DoSetUpInfo(sTestCmd))                                               //set up OK
        {
            SendHanaSRQInt(SETUP_INFORM_RECEIVE_OK_SRQ0x56);
        }
        else
        {
            SendHanaSRQInt(SETUP_INFORM_RECEIVE_FAIL_SRQ0x57);
        }
    }
    else if(sTestCmd=="SETUPOK")
    {
        SendHanaSRQInt(LOT_START_SRQ0x63);
    }
    else if(sTestCmd=="SETUPSTOP")
    {
//        ShowErrMessage("Set Up Stop.");
    }
    else if(sTestCmd=="LOADER?")
    {
        SendHanaCommand("A");
    }
    else if(sTestCmd.Pos("LOTON:")>0)
    {
        HdStartData.iNeedToRT=0;
        HdStartData.bDUMMY_TEST_0x42_OK=false;
        if(ParseLOTONStr(sTestCmd, sLotNo, LotSize, sMode))
        {
            fSCKART->DoARTLotStart(sLotNo, sMode, LotSize);                     //Steven 20250416 : HANA ART Function
            fSCKART->iCurrent93KARTStep=3;
            SendHanaSRQInt(LOTON_READ_SUCCESS_SRQ0x51);
            LastSet.iASEContact=-1;                                             // ++ first then add log, so -1 to let first contact as 0
            ClearAllTrayCount();                                                //Steven 20251029 : outputtray 數量
            UpdateFileNameList(1);
        }
        else
        {
            SendHanaSRQInt(LOTON_READ_FAIL_SRQ0x52);
        }
    }
    else if(sTestCmd=="LOTEND:NOLOT")
    {
        HdStartData.bDUMMY_TEST_0x42_OK=false;
    }
    else if(sTestCmd.Pos("TESTOK")>0)
    {
        if(HdWaitData.HD_Mode==HdWaitData.modeSMILL)
        {
            TestData.iStatus=TestData.etsSetFT_SMILL;
            SendHanaSRQInt(STANDBY_TESTMODE_SRQ0x53);
        }
        else//modeGeneral
        {
            StartPrimeTest();
        }
    }
    else if(sTestCmd.Pos("TESTSTOP")>0)
    {
//        ShowErrMessage("TEST Stop.");
    }
    else if(sTestCmd.Pos("HDMODE?")>0)
    {
        SendHanaCommand((HdWaitData.HD_Mode==HdWaitData.modeGeneral)?"GENERAL":"SMILL");
    }
    else if(sTestCmd.Pos("STEPOK?")>0)
    {
        SendHanaCommand("STEPREADY");
    }
    else if(sTestCmd.Pos("CONTACTOR?")>0)
    {
        SendHanaCommand(fMain->ArmStatusStrings());
    }
    else if(sTestCmd=="ID?")
    {
        SendHanaCommand(GetHD_ID());
    }
    else if(sTestCmd=="MAP?")
    {
        SendHanaCommand(GetHD_MAP());
    }
    else if(sTestCmd=="CT?")
    {
        SendHanaCommand(GetHD_CT());
    }
    else if(sTestCmd=="TEMPSET?")
    {
        SendHanaCommand(GetHD_Tempset());
    }
    else if(sTestCmd=="CONTACTOR?")
    {
        SendHanaCommand(GetHD_Tempset());
    }
    else if(sTestCmd=="SOAK?")
    {
        SendHanaCommand(GetHD_Soak());
    }
    else if(sTestCmd=="PMODEOK")
    {
        fSCKART->iFTRTCount=0;                                                  //Steven 20250421 : Hana ART
        fSCKART->palTestCnt->Caption=fSCKART->iFTRTCount;
        HdStartData.bDUMMY_TEST_0x42_OK=false;
        SendHanaSRQInt(PRIME_START_SRQ0x50);
        AddFixTrayCount();
    }
    else if(sTestCmd=="LOTEND:HDFAIL")
    {
//        ShowErrMessage("Lot End HD Fail");
    }
    else if(sTestCmd=="PRIMETESTSTARTOK")
    {
        StartPrimeTest();
    }
    else if(sTestCmd=="PRIMETESTEND")
    {
        HdStartData.iNeedToRT=0;
        SendHanaSRQInt(STANDBY_TESTMODE_SRQ0x53);
        HdStartData.iFT_RT_Mode=HdStartData.frtFT_LotEnd;
    }
    else if(sTestCmd=="RMODEOK")
    {
        HdStartData.bDUMMY_TEST_0x42_OK=false;
        if(LastSet.iRunStartMode==rsmContinuStart_ART &&
           HdStartData.iFT_RT_Mode==HdStartData.frtFT)
        {
            HdStartData.iNeedToRT=0;
            fSCKART->iFTRTCount++;                                              //Steven 20250421 : Hana ART
            fSCKART->AccessFile(false, -1);
            DoRT_START();
            if(fSCKART->iFTRTCount>=TestIF_File.iSCKART_TryCnt)                 //Steven 20251020 : for Hana ART
                SetRunStartMode(rsmContinuRetest_ART);
            else
                SetRunStartMode(rsmContinuStart_ART);                           //為了變更主畫面FT/RT顯示
        }
        else
        {
            HdStartData.iNeedToRT=1;
        }

        fSCKART->palTestCnt->Caption=fSCKART->iFTRTCount;
        fLotInfo->cbProcess->Text=fSCKART->sProcessCode;
        if(RunInfo.bLotStart==false)
        {
            fLotInfo->SetLotID(fSCKART->sLotID, false);
            fLotInfo->SetLotStart(__FUNC__);
        }
        LastSet.iASEContact=-1;                                                 // ++ first then add log, so -1 to let first contact as 0
        ClearAllTrayCount();                                                    //Steven 20251029 : outputtray 數量
        UpdateFileNameList(1);
        AddFixTrayCount();

//        SendHanaSRQInt(RETEST_START_SRQ);                                     //搬完tray才能送
    }
    else if(sTestCmd=="RETESTSTARTOK")
    {
        StartReTest();
    }
    else if(sTestCmd=="DUMMY_TEST_0x42_OK")
    {
        HdStartData.bDUMMY_TEST_0x42_OK=true;

        if(IsPrimeTest())
            fSCKART->iCurrent93KARTStep=3;
        else
            fSCKART->iCurrent93KARTStep=9;
    }
    else if(sTestCmd=="RETESTSTARTSTOP")
    {
//        ShowErrMessage("Retest Stop");
    }
    else if(sTestCmd=="RETESTEND")
    {
        SendHanaSRQInt(STANDBY_TESTMODE_SRQ0x53);
    }
    else if(sTestCmd.Pos("LOTRT:")>0)
    {
        if(ParseLOTONStr(sTestCmd, sLotNo, LotSize, sMode))
        {
            fLotInfo->cbProcess->Text=sMode;
            fSCKART->sProcessCode=sMode;
            fSCKART->AccessFile(false, 1);
        }

        SendHanaCommand("LOTRTOK");
    }
    else if(sTestCmd=="LOTEND:COMP")
    {
        HdStartData.iNeedToRT=2;
        fSCKART->iCurrent93KARTStep=12;
        UpdateFileNameList(2);
//        SendHanaSRQInt(LOT_END_SRQ);
    }
    else if(sTestCmd=="TDATA?")
    {
        SendHanaCommand(GetHD_TDATA());
    }
    else if(sTestCmd=="CDATA?")
    {
        SendHanaCommand(GetHD_CDATA());
    }
    else if(sTestCmd=="SDATA?")
    {
        SendHanaCommand(GetHD_SDATA());
    }
    else if(sTestCmd=="JDATA?")
    {
        SendHanaCommand(GetHD_JDATA());
    }
    else if(sTestCmd=="ADATA?")
    {
        SendHanaCommand(GetHD_ADATA());
    }
    else if(sTestCmd=="DATACLEAR")
    {
        timeData.Clear();
        SendHanaCommand("CLEAROK");
    }
    else
    {
    }
}
//---------------------------------------------------------------------------
bool uHANA_ART::DoSetUpInfo(AnsiString sCmd)
{   //DEVON:16,85,0,CT01,2
    //DEVON:16,85,0,CT12,2
    bool bRet=true;
    int iSiteCt=0;
    TStringList *List=new TStringList();
    AnsiString Str=sCmd.SubString(7, sCmd.Length());
    AnsiString sErr1="", sErr2="";
    List->CommaText=Str;

    if(List->Count==4 ||                                                        //Steven 20250414 : HANA ART Function
       List->Count==5)
    {
        // AI(W5-HanaART-Translate) 20260710: vclcompat's List->Strings[i] yields a
        // StringsProxy (implicit-AnsiString-convert only, no .c_str()/.Trim() of
        // its own) -- explicit AnsiString(...) cast needed, matching the existing
        // database.cpp/cpublic.cpp precedent for this exact shim gap.
        int    iSite=atof(AnsiString(List->Strings[0]).c_str());
        double dTemp=atof(AnsiString(List->Strings[1]).c_str());
        double dSoak=atof(AnsiString(List->Strings[2]).c_str());
        AnsiString sBinConfig=AnsiString(List->Strings[3]).Trim();

        for(int i=0; i<TestSocket.iShtRow; i++)                                 //wei 20160526 temp for sitemap
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                if(TestIF_File.iSiteMap[i][j]>0)                                //Steven 20190604 : Fixed for記憶體破壞
                    iSiteCt++;
            }
        }

        if(iSiteCt!=iSite)
        {
            bRet=false;
            sErr1.sprintf("Site Count not match. (%s)", sCmd);
            sErr2.sprintf("Handler Site : %d", iSiteCt);
            RecordProcess(sErr1, sErr2);
        }

        if(LastSet.iTemperature==Tempture_Hot)
        {
            if(Temperature.fWorkTemperBase!=dTemp)
            {
                bRet=false;
                sErr1.sprintf("Temperature not match. (%s)", sCmd);
                sErr2.sprintf("Handler Temp : %f", Temperature.fWorkTemperBase);
                RecordProcess(sErr1, sErr2);
            }

            if(Temperature.fSoakTime!=dSoak)
            {
                bRet=false;
                sErr1.sprintf("Soak Time not match. (%s)", sCmd);
                sErr2.sprintf("Handler Soak : %f", Temperature.fSoakTime);
                RecordProcess(sErr1, sErr2);
            }
        }
        else
        {
            if(LastSet.iTemperature==Tempture_AmbientHot)
            {
                if(Temperature.fWorkTemperBase!=dTemp)
                {
                    bRet=false;
                    sErr1.sprintf("Temperature not match. (%s)", sCmd);
                    sErr2.sprintf("Handler Temp : %f", Temperature.fWorkTemperBase);
                    RecordProcess(sErr1, sErr2);
                }
            }
            else if(Temperature.bUseAbitCHK)
            {
                if(Temperature.fAbitTemp!=dTemp)
                {
                    bRet=false;
                    sErr1.sprintf("Temperature not match. (%s)", sCmd);
                    sErr2.sprintf("Handler Temp : %f", Temperature.fAbitTemp);
                    RecordProcess(sErr1, sErr2);
                }
            }
            else
            {
                if(dTemp!=25)
                {
                    bRet=false;
                    sErr1.sprintf("Temperature not match. (%s)", sCmd);
                    sErr2.sprintf("Handler Temp : 25.0");
                    RecordProcess(sErr1, sErr2);
                }
            }

            if(dSoak!=0)
            {
                bRet=false;
                sErr1.sprintf("Soak Time not match. (%s)", sCmd);
                sErr2.sprintf("Handler Soak : 0.0");
                RecordProcess(sErr1, sErr2);
            }
        }

        Str=GetHD_CT();
        if(sBinConfig!=Str)
        {
            bRet=false;
            sErr1.sprintf("Pass Bin not match. (%s)", sCmd);
            sErr2.sprintf("Handler Bin : %s", Str);
            RecordProcess(sErr1, sErr2);
        }

        if(List->Count==5)                                                      //Steven 20251219 : for Hana ART
        {
            int iRTCount=atof(AnsiString(List->Strings[4]).c_str());  // see StringsProxy cast note above
            if(iRTCount>0)
            {
                fSCKART->edlRTTryCnt->Text=iRTCount;
                fSCKART->palRTTryCnt->Caption=iRTCount;
                TestIF_File.iSCKART_TryCnt=iRTCount;
                fSCKART->AccessFile(false, -1);
            }
        }
    }
    else
    {
        bRet=false;
    }
    List->Clear();
    delete List;
    return bRet;
}
//---------------------------------------------------------------------------
void uHANA_ART::StartPrimeTest()
{
    HdStartData.bEnableContact_FT=true;
    HdStartData.bDUMMY_TEST_0x42_OK=false;
    int iTolCh=TestSocket.iShtRow*TestSocket.iShtCol;
    // AI(W5-HanaART-Translate) 20260710: HHandler2Gpib.Site[] parked -- see
    // file-header GATED DEPENDENCIES #1 (MessageDef.h MV not yet translated).
#if 0 // TODO(W5-comms): HHandler2Gpib.Site[MAX_SOCKET_TOTAL] fill (MessageDef.h MV not yet translated)
    for(int i=0; i<MAX_SOCKET_TOTAL; i++)                                       //Steven 20250415 : HANA ART Function
    {
        if(i<iTolCh)
            HHandler2Gpib.Site[i]=true;
        else
            HHandler2Gpib.Site[i]=false;
    }
#else
    (void)iTolCh;
#endif // TODO(W5-comms)
    SendHanaSRQInt(DUMMYTEST_START_SRQ0x42);
}
//---------------------------------------------------------------------------
bool uHANA_ART::IsPrimeTest()
{
    return HdStartData.bEnableContact_FT;
}
//---------------------------------------------------------------------------
void uHANA_ART::EndPrimeTest()                                                  //FT end
{
    HdStartData.bEnableContact_FT=false;
    HdStartData.bDUMMY_TEST_0x42_OK=false;
    HdStartData.iNeedToRT=0;
    if(TestData.iStatus==TestData.etsSetFT)
    {
        TestData.iStatus=TestData.etsPrimeFT;
    }
    SendHanaSRQInt(PRIME_END_SRQ0x54);

    //Send Hana Tray Map by [N22]
    SendTrayMapToFTP();

    fSCKART->iCurrent93KARTStep=4;
}
//---------------------------------------------------------------------------
void uHANA_ART::StartReTest()
{
    HdStartData.bEnableContact_RT=true;
    HdStartData.bDUMMY_TEST_0x42_OK=false;
    int iTolCh=TestSocket.iShtRow*TestSocket.iShtCol;
    // AI(W5-HanaART-Translate) 20260710: HHandler2Gpib.Site[] parked -- see
    // file-header GATED DEPENDENCIES #1 (MessageDef.h MV not yet translated).
#if 0 // TODO(W5-comms): HHandler2Gpib.Site[MAX_SOCKET_TOTAL] fill (MessageDef.h MV not yet translated)
    for(int i=0; i<MAX_SOCKET_TOTAL; i++)                                       //Steven 20250415 : HANA ART Function
    {
        if(i<iTolCh)
            HHandler2Gpib.Site[i]=true;
        else
            HHandler2Gpib.Site[i]=false;
    }
#else
    (void)iTolCh;
#endif // TODO(W5-comms)

    SendHanaSRQInt(DUMMYTEST_START_SRQ0x42);
}
//---------------------------------------------------------------------------
void uHANA_ART::EndReTest()                                                     //RT end
{
    HdStartData.bEnableContact_RT=false;
    HdStartData.bDUMMY_TEST_0x42_OK=false;
    HdStartData.iNeedToRT=0;
    if(TestData.iStatus==TestData.etsSetRT)
    {
        TestData.iStatus=TestData.etsPrimeRT;
    }
    SendHanaSRQInt(RETEST_END_SRQ0x66);

    //Send Hana Tray Map by [N22]
    SendTrayMapToFTP();
    fSCKART->iCurrent93KARTStep=10;
}
//---------------------------------------------------------------------------
int uHANA_ART::NeedToRT()                                                       //Steven 20250415 : HANA ART Function
{                                                                               //0:Waiting command, 1:Need to RT, 2:Lot End
    return HdStartData.iNeedToRT;
}
//---------------------------------------------------------------------------
void uHANA_ART::EndOfLot()                                                      //Steven 20250415 : HANA ART Function
{
    SendHanaSRQInt(LOT_END_SRQ0x64);
}
//---------------------------------------------------------------------------
void uHANA_ART::TestDataNextStep()
{
    TestData.iStatus++;
    if(TestData.iStatus>=TestData.etsTotalStatus)
    {
        TestData.iStatus=0;
    }
}
//---------------------------------------------------------------------------
bool uHANA_ART::IsHanaArtAvailable()
{
    if(CosFunction.bUseSCKART &&
       IniConfig.bA10_AutoReTest &&
       TestIF_File.bSCKART_EnableART &&
       IniConfig.bA10_6_HANA_ART_TestMode_Enable)
    {
        return true;
    }
    else
    {
        return false;
    }
}
//---------------------------------------------------------------------------
bool uHANA_ART::IsContactAvailable()
{
    if(HdWaitData.HD_Mode==HdWaitData.modeGeneral)
    {
        return HdStartData.bEnableContact_FT;
    }
    else
    {
        if(HdStartData.bEnableContact_FT ||
           HdStartData.bEnableContact_RT)
        {
            if(HdStartData.bDUMMY_TEST_0x42_OK)
            {
                return true;
            }
        }
    }
    return false;
}
//---------------------------------------------------------------------------
bool uHANA_ART::ParseLOTONStr(const AnsiString input, AnsiString& sLotNo, int& LotSize, AnsiString& sMode)
{
    int colonPos=input.Pos(":");
    if(colonPos==0)
    {
        return false; // No :
    }
    AnsiString remainder=input.SubString(colonPos+1, input.Length()-colonPos);
    int commaPos1=remainder.Pos(",");
    if(commaPos1==0)
    {
        return false; // No ,
    }
    AnsiString remainderAfterFirstComma=remainder.SubString(commaPos1+1, remainder.Length()-commaPos1);
    int commaPos2=remainderAfterFirstComma.Pos(",");
    if(commaPos2==0)
    {
        return false; // No ,
    }
    sLotNo=remainder.SubString(1, commaPos1-1);                                // "ENGLOT"

    AnsiString lotSizeStr=remainderAfterFirstComma.SubString(1, commaPos2-1);   // "3000"
    LotSize=atoi(lotSizeStr.c_str());

    sMode=remainderAfterFirstComma.SubString(commaPos2+1, remainderAfterFirstComma.Length()-commaPos2); // "XE"
    return true;
}
//---------------------------------------------------------------------------
void uHANA_ART::UpdateFileNameList(int iUpdateStatus)                           //0: pass tray, 1: fail tray, 2: waiting to upload, 3: already upload
{
    if(fMain->hanaART->IsHanaArtAvailable()==false)
        return;

    AnsiString Str;
    AnsiString sPathName;
    AnsiString sFileName_ini;
    AnsiString sFileName_Log;
    sPathName.sprintf("D:\\HT9045_Log\\Hana_TrayMap\\%04d\\%s", SystemYear, fSCKART->palLotNumber->Caption);
    // AI(W5-HanaART-Translate) 20260710: MyForceDirectories's OWN declaration is
    // inside common.h's `#if 0 // TODO(wave-file)` gate (not merely link-
    // incomplete) -- the identifier is not visible in this tree yet (file-header
    // GATED DEPENDENCIES #4).  Every other statement in this function is ACTIVE.
#if 0 // TODO(wave-file): MyForceDirectories (common.h identifier itself gated)
    MyForceDirectories(sPathName, "uHANA_ART::UpdateFileNameList");
#endif // TODO(wave-file)

    sFileName_ini.sprintf("%s\\%s.ini", sPathName, fSCKART->palLotNumber->Caption);

    for(int i=0; i<eTrayCount; i++)
    {
        if(Prod.iTrayType[i]!=tNotUse &&
           Prod.iTrayType[i]!=tTrayBox)
        {
            sFileName_Log.sprintf("%s\\%s_%s_%s_%s_%s.txt", sPathName,
                                                             IniConfig.sGPIBMachineID,
                                                             fSCKART->pnlProcessCode->Caption,
                                                             fSCKART->palLotNumber->Caption,
                                                             RunInfo.LotStartTime,
                                                             s6TrayName[i]);
            WriteIniData(sFileName_ini, RunInfo.LotStartTime, s6TrayName[i], sFileName_Log);

            if(Prod.iIsPassT6[i]==0)
                WriteIniData(sFileName_ini, RunInfo.LotStartTime, s6TrayName[i]+AnsiString("_NeedUpload"), iUpdateStatus);
            else
                WriteIniData(sFileName_ini, RunInfo.LotStartTime, s6TrayName[i]+AnsiString("_NeedUpload"), 0);
        }
    }
}
//---------------------------------------------------------------------------
void uHANA_ART::AddNewTrayHead(int iAuto)
{
    if(fMain->hanaART->IsHanaArtAvailable()==false)
        return;

    // AI(W5-HanaART-Translate) 20260710: gated wholesale -- slHanaTrayMap[iAuto]
    // is a `TMyStringList*` (cmydef.h:140) and TMyStringList is only forward-
    // declared in this tree (Public/MyStringList.h is whole-file W3 DEFERRED
    // per MIGRATION_ROADMAP.md) -- ->FirstRow / ->AddText() cannot be compiled
    // against an incomplete type (file-header GATED DEPENDENCIES #2).  Also
    // needs fMain->cbSetupFileName->Text (golden main.h:875 TComboBox*, not
    // requested from FormsFacade this wave since the whole body is gated
    // anyway) and LoadForm->XDivision/YDivision (golden cprod.h -- ALREADY
    // available as `extern TRAY_TYPE_PARA *LoadForm;`, just unused while gated).
#if 0 // TODO(W3+W5-comms): TMyStringList (slHanaTrayMap) body -- incomplete type in this tree
    AnsiString str;
    str.sprintf("TR_NEW_%dX%d_%s", LoadForm->XDivision, LoadForm->YDivision, fMain->cbSetupFileName->Text);
    slHanaTrayMap[iAuto]->FirstRow=str;

    if(LastSet.iUnloaderTrayCount_ART[iAuto]>1)
    {
        str.sprintf("TR_CHA_%dX%d_%s", LoadForm->XDivision, LoadForm->YDivision, fMain->cbSetupFileName->Text);
        slHanaTrayMap[iAuto]->AddText(str);
    }
#else
    (void)iAuto;
#endif // TODO(W3+W5-comms)
}
//---------------------------------------------------------------------------
int uHANA_ART::SendTrayMapToFTP()
{
    // AI(W5-HanaART-Translate) 20260710: gated wholesale -- needs TfFTP
    // (ProductionInfo/TfFTP.h, not translated), `class FileInfo` (PathCombin,
    // no translated home), `MNetLog()` (no translated home), and
    // slHanaTrayMap[i]->sLastPathName/sLastNameNoPath/sLastFileName (same
    // incomplete-type issue as AddNewTrayHead) (file-header GATED DEPENDENCIES
    // #2/#3).  Offline default: 0 matches golden's HS_ERR_NoError numeric
    // value (HS_Function.h's named constant is not translated either, so the
    // literal is used directly with this citation instead of inventing the
    // header).
#if 0 // TODO(W5-comms): TfFTP / FileInfo / slHanaTrayMap[].sLast* -- none translated yet
    int iReturn=HS_ERR_NoError;
    AnsiString asDirPath="";
    AnsiString sSourceName="";
    AnsiString sTargetName="";
    AnsiString asFtpUplaodPath=IniConfig.sN22_1_FTPUploadPath;
    AnsiString asUserID=IniConfig.sN22_1_FTPUserName;
    AnsiString asPassword=IniConfig.sN22_1_FTPPassword;
    AnsiString asHost=IniConfig.sN22_1_FTPHost;

    TfFTP fFTP(asUserID, asPassword, asHost);
    if(fFTP.Connect(asUserID, asPassword, asHost, 30000, NMOS_AUTO, 0, 21))
    {
        AnsiString asError="";
        fFTP.CheckLocalFilePath(asDirPath);
        MNetLog(AnsiString().sprintf("FTP, Change folder : %s, %s", asFtpUplaodPath, sSourceName));
        fFTP.ChangeDirectories(asFtpUplaodPath);

        LotEndTime.sprintf("%04d%02d%02d%02d%02d%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
        for(int i=0; i<eTrayCount; i++)
        {
            if(Prod.iIsFailT6[i]==true)
            {
                asDirPath  =slHanaTrayMap[i]->sLastPathName;
                sSourceName=slHanaTrayMap[i]->sLastNameNoPath;
                sTargetName.sprintf("%s_%s_%s_%s_%s.txt", IniConfig.sGPIBMachineID,
                                                          fSCKART->pnlProcessCode->Caption,
                                                          fSCKART->palLotNumber->Caption,
                                                          s6TrayName[i],
                                                          LotEndTime);

                if(FileExists(FileInfo().PathCombin(asDirPath, sSourceName)))
                {
                    if(fFTP.Upload(asDirPath, asFtpUplaodPath, sSourceName, sTargetName, asError))
                    {
                        iReturn=HS_ERR_NoError;
                    }
                    else
                    {
                        iReturn=HS_ERR_FTPUploadError;
                    }
                }
                else
                {
                    iReturn=HS_ERR_FTPNoFileForUploadError;
                }
            }
            slHanaTrayMap[i]->sLastPathName="";
            slHanaTrayMap[i]->sLastFileName="";
        }
    }

    fFTP.Close();

//    bNeedUpload_N10=true;
    return iReturn;
#else
    return 0; // offline: 0 == golden HS_ERR_NoError (HS_Function.h not translated)
#endif // TODO(W5-comms)
}
//---------------------------------------------------------------------------
void uHANA_ART::RecoveryARTStatus()                                             //Steven 20251020 : for Hana ART
{
    HdStartData.bDUMMY_TEST_0x42_OK=true;
    if(LastSet.iRunStartMode==rsmContinuStart_ART)
    {
        HdStartData.bEnableContact_FT=true;
    }
    else if(LastSet.iRunStartMode==rsmContinuRetest_ART)
    {
        HdStartData.bEnableContact_RT=true;
    }
}
//---------------------------------------------------------------------------
