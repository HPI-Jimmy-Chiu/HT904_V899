// =============================================================================
//  forms/fProductionInfo.cpp  --  definitions for the fProductionInfo facade
//
//  AI(W906-W7-L1-Wave0) 20260801: NEW FILE.  See forms/fProductionInfo.h for the
//  golden provenance, the two-requests merge, and the pre-existing out-arm
//  variant extern situation this definition resolves.
// =============================================================================
#include "forms/fProductionInfo.h"
// AI(W906-FW3-PI1) 20260827: includes added for the FW3-PI1 read-only batch.
// All are pre-existing ports registered in ht9045_globals (Config.cpp's
// IniConfig home is actually cprod.cpp; cmydef.cpp; LastSet.cpp) or, for
// forms/fMain.h, ht9045_forms itself (the SAME target as this file, so no
// link-boundary concern) -- verified against CMakeLists.txt before use.
// MachineType.h/cprod.h/aHotPlateSubstrate.h/cSocket.h/Motor/mymotor.h/
// canary_support.h are DELIBERATELY NOT included here even though the
// suggested batch's golden functions reference symbols from them
// (ShowMyMessage/RecordProcess/ArmData/TestSocket/MOT[]/...) -- those symbols
// live only in ht9045_sm/ht9045_motor, which this file's target
// (ht9045_forms) does not and, per CMakeLists.txt:2249-2295, cannot link
// (target cycle). See forms/fProductionInfo.h's LINK-BOUNDARY EXCLUSIONS note
// for the full list of functions this cut, and the wave report for the
// per-symbol trace.
#include "forms/fMain.h"    // GetSetUpName(): fMain->cbSetupFileName->Text (same target, ht9045_forms)
#include "Config.h"          // IniConfig (CheckContactForceExist/bIntegerTimeCheck/bTimerCheck) -- defined cprod.cpp:50, ht9045_globals
#include "cmydef.h"          // iTo3Unload (ClearUnloaderTrayQty) -- cmydef.cpp, ht9045_globals
#include "LastSet.h"         // LastSet (BinCT / iN14_9_* tray counters) -- LastSet.cpp, ht9045_globals
#include "MachineType.h"     // e3Auto1..e3Fix5 (GetBin0_8List) -- header-only constants, no link cost
#include <cmath>              // pow() (cDynamicMultiContinualPassBinBySocket::GetMultiplierNum)
// AI(W906-FW3-PI2) 20260827: 1 include added for the FW3-PI2 read-only batch.
// CosFunction.h -- CosFunction (bEnableRPLog) -- defined cprod.cpp:52, ht9045_globals
// (same file/target as IniConfig above). SystemStart/InitialOK (TimeCount/bEnableRPLog)
// are already visible via cmydef.h, included above. Same link-boundary discipline as
// FW3-PI1: verified against CMakeLists.txt before use -- see forms/fProductionInfo.h's
// FW3-PI2 block comment for the per-function trace of what this wave did NOT translate.
#include "CosFunction.h"

TfProductionInfo::TfProductionInfo() : sLoadMO_TestFlow("") {
    // AI(W906-FW3-PI1) 20260827: widget pointer for the FW3-PI1 batch, same
    // ctor-body `new` idiom as every other facade (e.g. forms/fLotInfo.cpp's
    // edtSysLotID).  _bOEEStartLotSuccess/h/n/s/z and the array fields all use
    // in-class default member initializers instead (forms/fBinSel.h
    // precedent), so only the pointer member needs a line here.
    edInsertOPID_HALT = new TfLotInfoEdit();
}
// Offline: the real body refreshes the production-info form's per-tray IC-count
// display (golden ProductionInfo.h:377); there is no headless equivalent, so this
// is a no-op sink -- the same idiom every other UI-refresh facade method uses.
void TfProductionInfo::CalTrayICCount(int /*iWitchTray*/) {}
TfProductionInfo *fProductionInfo = new TfProductionInfo();

// ===========================================================================
// AI(W906-FW3-PI1) 20260827: FW3-PI1 wave -- read-only batch, faithful
// translation of golden ProductionInfo/ProductionInfo.cpp.  Each function's
// golden line span is cited on its own opening comment; see forms/
// fProductionInfo.h for the matching declaration-site citations.
// ===========================================================================

// golden ProductionInfo.cpp:1592-1598
AnsiString TfProductionInfo::MyBoolToString(bool b)
{
    if(b==true)
        return "Use";
    else
        return "No Use";
}
//---------------------------------------------------------------------------
// golden ProductionInfo.cpp:5839-5843
AnsiString TfProductionInfo::GetNowTime()
{
    TDateTime _now=utimetool.GetNow();
    return utimetool.TDatetmeToAnsiLog(_now);
}
//---------------------------------------------------------------------------
// golden ProductionInfo.cpp:5119-5123
int TfProductionInfo::GetNowTimeSec()
{
    DecodeTime(Now(), h, n, s, z);
    return h*3600+n*60+s;
}
//---------------------------------------------------------------------------
// golden ProductionInfo.cpp:5845-5848
void TfProductionInfo::SetPISTime()
{
    sPI_STime=GetNowTime();
}
//---------------------------------------------------------------------------
// golden ProductionInfo.cpp:5850-5853
AnsiString TfProductionInfo::GetPISTime()
{
    return sPI_STime;
}
//---------------------------------------------------------------------------
// golden ProductionInfo.cpp:369-372
AnsiString TfProductionInfo::OEE_GetMO()
{
    return _sOEE_MO;
}
//---------------------------------------------------------------------------
// golden ProductionInfo.cpp:1175-1178
bool TfProductionInfo::IsOEEStartLotSuccess()
{
    return _bOEEStartLotSuccess;
}
//---------------------------------------------------------------------------
// golden ProductionInfo.cpp:5382-5385
AnsiString TfProductionInfo::GetSetUpName()
{
    return fMain->cbSetupFileName->Text;
}
//---------------------------------------------------------------------------
// golden ProductionInfo.cpp:5834-5837
AnsiString TfProductionInfo::GetScheduleName()
{
    return sLoadMO_MO+"_"+sLoadMO_TestFlow;
}
//---------------------------------------------------------------------------
// golden ProductionInfo.cpp:5917-5920
void TfProductionInfo::SetInsertOPIDStr(const AnsiString& s)
{
    edInsertOPID_HALT->Text=FilterAlphanumeric(s);
}
//---------------------------------------------------------------------------
// golden ProductionInfo.cpp:5922-5925
AnsiString TfProductionInfo::GetInsertOPIDStr()
{
    return FilterAlphanumeric(edInsertOPID_HALT->Text);
}
//---------------------------------------------------------------------------
// golden ProductionInfo.cpp:5927-5939
AnsiString TfProductionInfo::FilterAlphanumeric(const AnsiString& s)
{
    AnsiString result;
    for (int i = 1; i <= s.Length(); i++)
    {
        char c = s[i];
        if((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') )
        {
            result += c;
        }
    }
    return result;
}
//---------------------------------------------------------------------------
// golden ProductionInfo.cpp:4931-4934
void TfProductionInfo::CheckNewDayAndSubtract(int &iTimeSec)
{
    iTimeSec=iTimeSec%86400;
}
//---------------------------------------------------------------------------
// golden ProductionInfo.cpp:4936-4947
bool TfProductionInfo::CheckContactForceExist()
{
    if( sDevice_Pin_Force==""  ||
        sDevice_Pin_Force=="0" ||
        sDevice_Pin_Count==""  ||
        sDevice_Pin_Count=="0" ||
        IniConfig.bD06ContactOffsetDefaultValue==false
    )
        return false;
    else
        return true;
}
//---------------------------------------------------------------------------
// golden ProductionInfo.cpp:4506-4520
bool TfProductionInfo::bIntegerTimeCheck()
{
    DecodeTime(Now(), h, n, s, z);
    if(IniConfig.iN14_16_IPSCInterval==120)                                     //2Hour
    {
        if(h%2==0)                                                              //0 2 4 6 8 10 12 14 16 18 20 22 24 偶數整點。
            return true;
    }
    else
    {
        if(n==0)                                                                //整點
            return true;
    }
    return false;
}
//---------------------------------------------------------------------------
// golden ProductionInfo.cpp:4522-4538
bool TfProductionInfo::bTimerCheck(int iTimeMode)
{
    static int iOldHour=-1;
    static int iOldmin=-1;
    DecodeTime(Now(), h, n, s, z);
    if((iTimeMode==1 && n==0) ||                                                //整點
       (iTimeMode==2 && n==30))                                                 //半小時
    {
        if(iOldHour!=h || iOldmin!=n)
        {
            iOldHour=h;
            iOldmin=n;
            return true;
        }
    }
    return false;
}
//---------------------------------------------------------------------------
// golden ProductionInfo.cpp:5773-5783
bool TfProductionInfo::IsContinueFailAlarm()
{
    if(sLoadMO_TestFlow.SubString(1, 1)=="R")
    {
        return false;
    }
    else
    {
        return true;
    }
}
//---------------------------------------------------------------------------
// golden ProductionInfo.cpp:495-515
AnsiString TfProductionInfo::GetCSVLineData(int iDataNum, AnsiString sCSVLineStr)
{
    AnsiString sRes="";
    for(int i=0; i<iDataNum-1; i++ )
    {
        if(sCSVLineStr.Pos(",")>0)
        {
            sCSVLineStr=sCSVLineStr.SubString((sCSVLineStr.Pos(",")+1), (sCSVLineStr.Length()-sCSVLineStr.Pos(",")));
        }
        else
        {
            sCSVLineStr="";
        }
    }
    sRes=sCSVLineStr;
    if( sCSVLineStr.Pos(",")>0)
    {
        sRes=sCSVLineStr.SubString(1, sCSVLineStr.Pos(",")-1);
    }
    return sRes;
}
//---------------------------------------------------------------------------
// golden ProductionInfo.cpp:6065-6077
std::vector<int> TfProductionInfo::GetBin0_8List()
{
    int iBin0_8[8]= {
                     e3Auto1,  e3Auto2,  e3Auto3,   e3Fix1,
                     e3Fix2,   e3Fix3,   e3Fix4,    e3Fix5,
                    };
    std::vector<int> list;
    for(int iarea=0; iarea<8; iarea++)
    {
        list.push_back(iBin0_8[iarea]);
    }
    return list;
}
//---------------------------------------------------------------------------
// golden ProductionInfo.cpp:4393-4405
void TfProductionInfo::CalculateNowUnloaderTrayQty(bool bIsClear)
{
    std::vector<int> lsBin0_8=GetBin0_8List();
    for(int iBin=0; iBin<6; iBin++)                                                      //Auto1~3，Fix1~3
    {
        iNowUnloaderTrayQty[iBin]+=LastSet.BinCT[0][lsBin0_8[iBin]]-iLastUnloaderTrayQty[iBin];
        iLastUnloaderTrayQty[iBin]=LastSet.BinCT[0][lsBin0_8[iBin]];
        if(bIsClear)
        {
            iLastUnloaderTrayQty[iBin]=0;
        }
    }
}
//---------------------------------------------------------------------------
// golden ProductionInfo.cpp:4429-4440
int TfProductionInfo::CalculateUnloadTotalICQty()                               //放到 Unloader 的總數
{
    int iTotalLoadQty;
    CalculateNowUnloaderTrayQty();

    iTotalLoadQty=0;
    for(int i=0; i<8; i++)
    {
        iTotalLoadQty=iTotalLoadQty+iNowUnloaderTrayQty[i];                     //Bin1 ~ Bin8  == Auto1~Auto3 Fix1~Fix3
    }
    return iTotalLoadQty;
}
//---------------------------------------------------------------------------
// golden ProductionInfo.cpp:4460-4467
void TfProductionInfo::ClearUnloaderTrayQty()
{
    for(int i=0; i<8; i++)
    {
        iNowUnloaderTrayQty[i] =0;
        iLastUnloaderTrayQty[i]=LastSet.BinCT[0][iTo3Unload[i]];
    }
}
//---------------------------------------------------------------------------
// golden ProductionInfo.cpp:3329-3339
void TfProductionInfo::ClearTrayCnt()
{
    for(int i=0; i<256; i++)
    {
//        LastSet.iN14_9_TrayICCnt[i]         =0;                                 //Tested IC
        LastSet.iN14_9_FullTrayCnt[i]       =0;                                 //Full Tray Tray Qty
        LastSet.iN14_9_PartialTrayCnt[i]    =0;                                 //Not Full Tray Tray Qty
        LastSet.iN14_9_PartialTrayICCnt[i]  =0;                                 //Not Full Tray IC Qty
        LastSet.iN14_9_InHandlerICCnt[i]    =0;                                 //In Machine Tray IC Qty
    }
}
//---------------------------------------------------------------------------
// golden ProductionInfo.cpp:5955-5963
int cDynamicMultiContinualPassBinBySocket::GetMultiplierNum()
{
    int iBack=1;
    if(IniConfig.bN14_24_DynaMultiContinuPassSocket)
    {
        iBack=pow(2,iDynamicThresholdNum);
    }
    return iBack;
}
//---------------------------------------------------------------------------
// golden ProductionInfo.cpp:5965-5969
void cDynamicMultiContinualPassBinBySocket::AddThresholdNum()
{
    if(iDynamicThresholdNum<IniConfig.iN14_24_DyMultiPassPower)//max value =5
        iDynamicThresholdNum++;
}
//---------------------------------------------------------------------------

// ===========================================================================
// AI(W906-FW3-PI2) 20260827: FW3-PI2 wave -- second read-only batch.  Each
// function's golden line span is cited on its own opening comment; see
// forms/fProductionInfo.h's FW3-PI2 block for the matching declaration-site
// citations and the per-function exclusion trace for the 7 priority names
// that were NOT translated this wave.
// ===========================================================================

// golden ProductionInfo.cpp:364-367
void TfProductionInfo::OEE_SetMO(AnsiString sMO)
{
    _sOEE_MO=sMO;
}
//---------------------------------------------------------------------------
// golden ProductionInfo.cpp:374-377
void TfProductionInfo::OEE_SetHandlerID(AnsiString sHDID)
{
    _sOEE_HandlerID=sHDID;
}
//---------------------------------------------------------------------------
// golden ProductionInfo.cpp:920-940
void TfProductionInfo::TimeCount()
{
    if(IniConfig.bN14_1_EnableOEEFunction==false ||
       IniConfig.iN14_1_OEERecordCycleTime==0)
    {
        return;
    }
    static int iLastSecond=0;
    int iSecond=atoi(FormatDateTime("ss", Now()).c_str());   // golden: Now().FormatString("ss") -- vclcompat has no
                                                               // TDateTime::FormatString member, established substitution
                                                               // (ainarm9045.cpp:916 and others)
    iSecond=(iSecond+500)*1000;
    dtNowDateTime=Now();
    if(iSecond!=iLastSecond)
    {
        iLastSecond=iSecond;
        //Power On 開始時間
        if(SystemStart==true)
        {
            _iOEE_RunTime++;
        }
    }
}
//---------------------------------------------------------------------------
// golden ProductionInfo.cpp:1126-1134
void TfProductionInfo::SetNextRecordDateTime()
{
    int iRecordCycleTime=IniConfig.iN14_1_OEERecordCycleTime;
    iNextDateTime=GetNowTimeSec();
    if(IniConfig.iN14_1_OEERecordCycleTime!=0)
    {
        iNextDateTime=iNextDateTime-iNextDateTime%iRecordCycleTime+iRecordCycleTime;
    }
}
//---------------------------------------------------------------------------
// golden ProductionInfo.cpp:2818-2822
void TfProductionInfo::SetStartStatus()
{
    _sOEE_ActivityID="";
    _sOEE_Status="Auto Run";
}
//---------------------------------------------------------------------------
// golden ProductionInfo.cpp:3958-3974
bool TfProductionInfo::bEnableRPLog()
{
    if(CosFunction.bRecipeParameterDefault   ==false ||
       CosFunction.bRecipeParameterDefaultLog==false ||
       InitialOK==false)
    {
        return false;
    }

    if(IniConfig.bN14_20_DefaultRecipeChangeLog             ==false ||
       IniConfig.asN14_20_ChangeLogPath                     ==""    ||
       IniConfig.bN14_20_DefaultRecipeChangeLogCycleRecord  ==false)
    {
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
// golden ProductionInfo.cpp:4949-4954
void TfProductionInfo::SetOffsetContactForce(AnsiString asValue)
{
    if(asValue=="")
        asValue="0";
    dOffsetContactForce=atof(asValue.c_str());
}
//---------------------------------------------------------------------------
// GATE (PI2-G1) -- AI(W906-FW3-PI2-fix) 20260827: EnableInArmAutoCalSuckZ
// (golden ProductionInfo.cpp:5907-5910) 與 EnableOutArmAutoCalSuckZ
// (golden :5912-5915) 本波交付後被主迴圈移除，**理由是連結期缺符號**：
//
//   undefined reference to `bEnableInarmSuckZAuto'
//   undefined reference to `bEnableOutarmSuckZAuto'
//
// 兩個全域在 `cmydef.cpp:6102-6103` **確實有定義**，但那一段整個在
// `cmydef.cpp:6011` 開始的 `#if 0 // TODO(W6): function bodies depend on
// untranslated globals/state machines` 內——**文字存在，編譯器看不到**。
// `cmydef.h:5806-5807` 只有 extern 宣告，所以 `-fsyntax-only` 完全過關，
// `nm --undefined-only` 也只會列出它們而不會說「沒人提供」。
// 是全新 build dir 的連結期把它抓出來的（gate pi2 兩側 build 都因此失敗）。
//
// **要解它必須先解 cmydef.cpp:6011 那個 #if 0**，那是行為變更、要單獨一波。
// 這裡刻意不留退化版本（例如恆回 false）——**缺符號比錯答案好**，
// 一個恆假的 EnableAutoCalSuckZ 會讓未來的呼叫者靜默走錯分支。
//---------------------------------------------------------------------------
// golden ProductionInfo.cpp:5949-5953 -- golden's OWN body is fully commented
// out (dead/no-op in golden itself); translated verbatim as an empty body.
void TfProductionInfo::InitialStringGrid(TStringGrid * /*sg*/)
{
//    sg->Colcount();
//    int iCol=sg->Colcount;
}
//---------------------------------------------------------------------------
// golden ProductionInfo.cpp:5821-5832
void TfProductionInfo::GetFTP_Setting(AnsiString &asUserID, AnsiString &asPassword, AnsiString &asHost)
{
    #ifdef SOFT_SIMULTE
    asUserID="HONPREC";
    asPassword="27025312";
    asHost="127.0.0.1";
    #else
    asUserID=IniConfig.asN14_3_OEEFTPUserName;
    asPassword=IniConfig.asN14_3_OEEFTPPassword;
    asHost=IniConfig.asN14_3_OEEFTPHost;
    #endif
}
//---------------------------------------------------------------------------
// golden ProductionInfo.cpp:5254-5278 -- self-contained helper (no golden-global
// references); its only golden caller, GetBinTraySetting, is NOT translated this
// wave -- see forms/fProductionInfo.h's FW3-PI2 block comment.
bool TfProductionInfo::SetBinTraySetting(TStringList* tlBinTray,AnsiString asSource,AnsiString asBin)
{
    bool bGetE=false;
    if(asSource.Trim()=="")
        return bGetE;
    int iTemp=0;
    asSource=asSource.Trim();
    TStringList* tempTL=new TStringList();
    tempTL->CommaText=asSource;
    AnsiString asString="";
    for(int i=0; i<tempTL->Count; i++)
    {
        asString=tempTL->Strings[i];
        if(asString=="E")
        {bGetE=true;}
        else
        {
            iTemp=atoi(tempTL->Strings[i].c_str());
            iTemp=iTemp>15?0:iTemp;
            tlBinTray->Strings[iTemp]=asBin;
        }
    }
    delete tempTL;
    return bGetE;
}
//---------------------------------------------------------------------------
