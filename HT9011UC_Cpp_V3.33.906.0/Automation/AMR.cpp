// =============================================================================
//  Automation/AMR.cpp  --  TeraPower AMR tray-count bridge  (W5 translation)
//
//  Faithful translation of golden Automation/AMR.cpp (467 lines, BCB6,
//  Big5/cp950).  Golden: HT9011UC_Code_V3.33.906.0_20260618/Automation/AMR.cpp
//  Translation wave: W5 (Automation network unit)
//  Translator: AI(W5-AMR-Translate) 20260710
//
//  SCOPE: whole file, translated verbatim (confirmed zero fMain-> hits by
//  recon).  Every method is pure counting/threshold logic over already-
//  translated global state (LastSet / IniConfig / CosFunction / TestIF_File /
//  BinSelect[] / MOT[] -- all live in cprod.h/Config.h/CosFunction.h/
//  canary_support.h/Motor/mymotor.h, already part of ht9045_globals /
//  ht9045_motor / the ht9045_sm canary_support shim), plus ONE VCL-form touch
//  (fLotInfo->RefreshAMR(), 5 call sites) and TWO fSCKART reads
//  (iFTRTCount/CheckLoadingCount already in FormsFacade; iCurrent93KARTStep is
//  NEW -- see report) routed through the established FormsFacade pattern.
//
//  DEPENDENCY GAPS (flagged, NOT filled here -- FormsFacade.h/canary_support.h
//  are shared files outside this unit's write boundary; see translate report):
//    1. LAST_GENERAL_SET (canary_support.h) is missing 6 fields this file
//       reads/writes: iAMRTrayConut[9], iAMRTrayLoaderTotal, bAMRTrayFeedWait,
//       bAMRLoaderLast, bAMRRequestSupplyTray, iBinData32[4][260] (golden
//       LastSet.h:366/502-506).
//    2. FormsFacade TfLotInfo is missing `void RefreshAMR();` (golden
//       uLotInfo.h:1416).
//    3. FormsFacade TfSCKART is missing `int iCurrent93KARTStep;` (golden
//       Automation/SCK_ART.h:263) -- iFTRTCount/iInputCount/CheckLoadingCount
//       are already present from the W6.3 TRAY-ARM wave.
//  Until the integrate agent adds these, this TU fails to COMPILE (missing
//  struct members), not merely to link -- expected per the parallel-unit
//  protocol (this file's own syntax has been checked separately; see report).
//
//  VCL/Borland conversions: AnsiString (1-based) via vclcompat, verbatim
//  `.sprintf(...)` idiom (vclcompat AnsiString supports it); no __fastcall in
//  this .cpp (golden AMR.cpp itself never marks these methods __fastcall,
//  only the ctor declaration in the header does -- see AMR.h note).
//
//  FAITHFUL GOLDEN QUIRKS (preserved, NOT "fixed"):
//    * GetTrackBinData: header declares the parameter `iAuto`, the .cpp
//      definition names it `iCate` -- a genuine golden header/impl parameter-
//      name mismatch (legal C++; only the header name is visible to callers).
//    * CheckUnloaderCount: the `if(iTrayFeed==1)` guard has a commented-out
//      `|| iCleanOut==1` right next to it (golden dead comment) -- kept
//      verbatim as a comment, not resurrected.
//    * SupplyCover: the ART-branch alternate body is entirely commented out
//      in the golden ("Sam20250625 fix" note) -- kept verbatim as a comment.
//    * ClearTrackBinData iterates the SAME iCatDataT3Pos[] table by identity
//      match `iAutoPos!=iPos -> continue` for EVERY iAuto, mirroring
//      GetTrackBinData's scan (not an alias table) -- faithful, not optimized.
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8.
//  Final gate: ZERO U+FFFD.
// =============================================================================
#include "MachineDefine.h"

#include "AMR.h"

#include "cprod.h"                 // IniConfig(Config.h)/CosFunction(CosFunction.h)/TestIF_File/BinSelect/SYSTEM_BIN_SELECT/MachineType enums
#include "cmydef.h"                 // iAutoTrayCount[]/iTrayFeed/iTestRunMode/iTestBinCount/HAS_IC
#include "Motor/mymotor.h"          // MOT[] (TTrayMotor)
#include "FormsFacade.h"            // fLotInfo->RefreshAMR() / fSCKART (offline stand-ins)
#include "canary_support.h"         // LastSet (LAST_GENERAL_SET shim) / ShowMyMessage / __FUNC__

//---------------------------------------------------------------------------
class TTeraPowerAMR AMR;
TTeraPowerAMR::TTeraPowerAMR()
{
};
//---------------------------------------------------------------------------
void TTeraPowerAMR::Initial()
{
    if(CosFunction.bEnableHandlerResultServer==false)
        return;
    LastSet.iAMRTrayConut[0]=IniConfig.iA60LoaderQtyAtOneTime;
    LastSet.iAMRTrayConut[1]=0;
    LastSet.iAMRTrayConut[2]=0;
    LastSet.iAMRTrayConut[3]=0;
    LastSet.iAMRTrayConut[4]=0;
    LastSet.iAMRTrayConut[5]=0;
    LastSet.iAMRTrayLoaderTotal=0;
    LastSet.bAMRTrayFeedWait=false;
    LastSet.bAMRLoaderLast=false;
    LastSet.bAMRRequestSupplyTray=false;                                        //Sam 20250312 : Initial 也要清除
    fLotInfo->RefreshAMR();
}
//---------------------------------------------------------------------------
void TTeraPowerAMR::LoaderInCount()
{
    if(CosFunction.bEnableHandlerResultServer==false || IniConfig.bA60EnableAMR==false)
        return;

    LastSet.iAMRTrayConut[0]--;
    LastSet.iAMRTrayLoaderTotal++;

    fLotInfo->RefreshAMR();
}
//---------------------------------------------------------------------------
void TTeraPowerAMR::UnloaderOutCount(int iAuto)
{
    if(CosFunction.bEnableHandlerResultServer==false || IniConfig.bA60EnableAMR==false)
        return;

    if(iAuto>=0 && iAuto<=2)
    {
        int iTrackPos=iAuto+3;
        LastSet.iAMRTrayConut[iTrackPos]++;

        if(IsAutoReTestTray(iAuto) && LastSet.iAMRTrayConut[iTrackPos]==1)
        {
            LastSet.iUnloaderTrayCount_ART[iAuto]--;   //RT 流程搬回去 Loader 時，要留一盤當上蓋所以要把那台減掉。
            iAutoTrayCount[iAuto]--;
        }

        fLotInfo->RefreshAMR();
    }
    else
    {
        ShowMyMessage("UnloaderOutCount error","");
    }
}
//---------------------------------------------------------------------------
bool TTeraPowerAMR::WaitAMRSupplyTray()                                         //Sam 20240827 : 新增 AMR 功能
{
    if(CosFunction.bEnableHandlerResultServer==false || IniConfig.bA60EnableAMR==false)
        return false;

    if(LastSet.bAMRRequestSupplyTray)
        return true;
    return false;
}
//---------------------------------------------------------------------------
bool TTeraPowerAMR::CheckLoaderCount()
{
    if(CosFunction.bEnableHandlerResultServer==false ||
       IniConfig.bA60EnableAMR==false)
    {
        return false;
    }

    if(LastSet.bAMRLoaderLast)
    {
        return false;                                                           //AMR 最後一次補盤
    }
    else if(IniConfig.bA10_AutoReTest && TestIF_File.bSCKART_EnableART)         //Sam 20250423 : AMR + ART Retest 修改滿 Tray 也要收盤
    {
        if(fSCKART->iFTRTCount==0)                                              //FT 時看計數數量要呼叫補盤
        {
            if(LastSet.iAMRTrayConut[0]<=IniConfig.iA60NotifyQty[0])
                return true;
        }
        else                                                                    //RT Loader 沒有 IC 呼叫補盤，且需要RT的 IC 數量還不夠時 呼叫補盤
        {
            if(MOT[MMTrayY_Car].fHasTray==false &&
               MOT[MMTrayY].HasRealIC()==false &&
               fSCKART->CheckLoadingCount()==0)
            {
                return true;
            }
        }
        return false;                                                           //當有開啟 ART 功能時且再做第二次測試時，就不需要用通知 AMR 補 Loader
    }
    else if(LastSet.iAMRTrayConut[0]<=IniConfig.iA60NotifyQty[0])
    {
        return true;                                                            //Loader 數量低於設定值要呼叫 AMR 補盤
    }
    else
    {
        return false;
    }
}
//---------------------------------------------------------------------------
bool TTeraPowerAMR::CheckUnloaderCount(int iAuto)
{
    if(CosFunction.bEnableHandlerResultServer==false || IniConfig.bA60EnableAMR==false)
        return false;

    if(iTrayFeed==1)// || iCleanOut==1)    /                                    /若開始執行 Tray Feed && CleanOut 流程就都回覆 False 改由 "HTGR,802" 來收所有盤。
        return false;

    if(IniConfig.bA10_AutoReTest && TestIF_File.bSCKART_EnableART)
    {
        if(fSCKART->iCurrent93KARTStep==3 ||  fSCKART->iCurrent93KARTStep==9)
        {
            //ART 模式下 FT測試中 &　RT測試中才需要判斷需要收料
        }
        else
        {
            return false;
        }
    }

    if(iAuto>=0 && iAuto<=2)
    {
        int iTrackPos=iAuto+3;
        if(LastSet.iAMRTrayConut[iTrackPos]>=IniConfig.iA60NotifyQty[iTrackPos])
            return true;                                                        //Unloader 數量大於設定值要呼叫 AMR 搬走
        else
            return false;
    }
    else
    {
        ShowMyMessage("CheckUnloaderCount error","");
        return false;
    }
}
//---------------------------------------------------------------------------
void TTeraPowerAMR::LoaderCarryIn()                                             //Loader 補盤用累加
{
    LastSet.iAMRTrayConut[0]+=IniConfig.iA60LoaderQtyAtOneTime;
    fLotInfo->RefreshAMR();
}
//---------------------------------------------------------------------------
void TTeraPowerAMR::UnloaderCarryOut(int iAuto)                                 //Unloader 搬走直接歸0
{
    if(iAuto>=0 && iAuto<=2)
    {
        int iTrackPos=iAuto+3;
        LastSet.iAMRTrayConut[iTrackPos]=0;
        iAutoTrayCount[iAuto]=0;                                                //Sam 20250430 : AMR 搬走 ART RT Tray 要把資料清空
        fLotInfo->RefreshAMR();
    }
    else
    {
        ShowMyMessage("UnloaderCarryOut error","");
    }
}
//---------------------------------------------------------------------------
bool TTeraPowerAMR::CheckTrayFeed()
{
    if(CosFunction.bEnableHandlerResultServer==false || IniConfig.bA60EnableAMR==false)
        return false;

    if(LastSet.iAMRTrayConut[0]!=0 ||
       LastSet.iAMRTrayConut[1]!=0 ||
       LastSet.iAMRTrayConut[2]!=0 ||
       LastSet.iAMRTrayConut[3]!=0 ||
       LastSet.iAMRTrayConut[4]!=0 ||
       LastSet.iAMRTrayConut[5]!=0 ||
       LastSet.iAMRTrayLoaderTotal!=0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
//---------------------------------------------------------------------------
AnsiString TTeraPowerAMR::GetTrackBinData(int iCate)
{
    AnsiString sBinData="",sCate="", s1="";

    if(iCate==0)
        sCate="Auto1=";
    else if(iCate==1)
         sCate="Auto2=";
    else if(iCate==2)
         sCate="Auto3=";
    else if(iCate==3)
        sCate="Fix1=";
    else if(iCate==4)
        sCate="Fix2=";
    else if(iCate==5)
        sCate="Fix3=";
    else if(iCate==6)
        sCate="Fix4=";
    else if(iCate==7)
        sCate="Fix5=";
    else if(iCate==8)
        sCate="Fix6=";
    else
        ShowMyMessage("GetTrackBinData error","");

    if(iCate>=0 && iCate<=8)
    {
        int iPos=0,iErrPos=0;
        int iAutoPos=iCate+1;
        for(int i=0; i<iTestBinCount; i++)
        {
            iPos=BinSelect[iTestRunMode].iCatDataT3Pos[i];
            if(iAutoPos!=iPos)
                continue;
            //sBinData+="HBin"+IntToStr(i)+".";
            s1.sprintf("HBin%d=%d.",i , LastSet.iBinData32[2][i]);
            sBinData+=s1;
        }
        iErrPos=BinSelect[iTestRunMode].IfErrorT3;

        if(iErrPos+1==iAutoPos)
        {
            s1.sprintf("HBinE=%d.", LastSet.iBinData32[2][iTestBinCount]);
            sBinData+=s1;
        }
    }

    if(sBinData=="")
    {
        sBinData=sCate+"NA=NA";
    }
    else
    {
        sBinData=sCate+sBinData;
    }
    return sBinData;
}
//---------------------------------------------------------------------------
AnsiString TTeraPowerAMR::GetTrackBinDataCateR(int iAuto)                       //Sam 20250423 : AMR + ART Retest 修改滿 Tray 也要收盤
{
    AnsiString sBinData="",sAuto="", s1="";

    if(iAuto==0)
        sAuto="Auto1=";
    else if(iAuto==1)
        sAuto="Auto2=";
    else if(iAuto==2)
        sAuto="Auto3=";
    else
        ShowMyMessage("GetTrackBinData error","");

    if(iAuto>=0 && iAuto<=2)
    {
        int iPos=0,iErrPos=0;
        int iAutoPos=iAuto+1;
        for(int i=0; i<iTestBinCount; i++)
        {
            iPos=BinSelect[iTestRunMode].iCatDataT3Pos[i];
            if(iPos>=1)
            {
                if(iAutoPos==iPos ||
                   BinSelect[iTestRunMode].bCateR[iPos-1])
                {
                                //取得有設定在 CateR 和 Retest 的 Category Bin count
                }
                else
                {
                    continue;   //此 Bin 沒有設定在 CateR 和 Retest 的 Category 裡
                }
            }
            else
            {
                 continue;      //此 Bin 沒有設定 Category
            }

            s1.sprintf("HBin%d=%d.",i , LastSet.iBinData32[2][i]);
            sBinData+=s1;
        }
        iErrPos=BinSelect[iTestRunMode].IfErrorT3;

        if(iErrPos+1==iAutoPos)
        {
            s1.sprintf("HBinE=%d.", LastSet.iBinData32[2][iTestBinCount]);
            sBinData+=s1;
        }
    }

    if(sBinData=="")
    {
        sBinData=sAuto+"NA=NA";
    }
    else
    {
        sBinData=sAuto+sBinData+",ReTest bin";
    }
    return sBinData;
}
//---------------------------------------------------------------------------
AnsiString TTeraPowerAMR::GetNormalFailBin()                                    //Spencerlin 20260202 : 取得 Auto1~3 不是 AutoRetest 或 CateR 的 Fail Bin
{
    int iPos=0;
    AnsiString sNormalFailBin="", s1="";

    for(int iBin=0; iBin<iTestBinCount; iBin++)
    {
        iPos=BinSelect[iTestRunMode].iCatDataT3Pos[iBin];                       //每個 Bin 的位置
        if(iPos>=ePosAuto1 && iPos<=ePosFix12)                                  //RogerYang 20260320 : 加入Fix也要 //只要 Auto1~3
        {
            if(BinSelect[iTestRunMode].bAutoRetest[iPos-ePosAuto1]==false &&    //不是 AutoRetest 或 CateR 的 Fail Bin
               BinSelect[iTestRunMode].bCateR[iPos-ePosAuto1]==false &&
               BinSelect[iTestRunMode].iStackDefFailCate[iPos-ePosAuto1]==1)
            {
                if(sNormalFailBin=="")
                {
                    s1.sprintf("%d", iBin);
                    sNormalFailBin=sNormalFailBin+s1;
                }
                else
                {
                    s1.sprintf(",%d", iBin);
                    sNormalFailBin=sNormalFailBin+s1;
                }
            }
        }
    }
    return sNormalFailBin;
}
//---------------------------------------------------------------------------
void TTeraPowerAMR::ClearTrackBinData(int iAuto)
{
    if(iAuto>=0 && iAuto<=2)
    {
        int iPos=0,iErrPos=0;
        int iAutoPos=iAuto+1;
        for(int i=0; i<iTestBinCount; i++)
        {
            iPos=BinSelect[iTestRunMode].iCatDataT3Pos[i];
            if(iAutoPos!=iPos)
                continue;
            LastSet.iBinData32[2][i]=0;
        }
        iErrPos=BinSelect[iTestRunMode].IfErrorT3;

        if(iErrPos+1==iAutoPos)
        {
            LastSet.iBinData32[2][iTestBinCount]=0;
        }
    }
    else
    {
        ShowMyMessage("ClearTrackBinData error","");
    }
}
//---------------------------------------------------------------------------
void TTeraPowerAMR::SupplyCover(int iAuto)                                      //判斷此盤是否當蓋子
{
    if(CosFunction.bEnableHandlerResultServer==false || IniConfig.bA60EnableAMR==false)
        return;

    if(iTrayFeed==1)// || iCleanOut==1)    //若開始執行 Tray Feed && CleanOut 流程就都回覆 False 改由 "HTGR,802" 來收所有盤。
        return;

    if(iAuto>=0 && iAuto<=2)
    {
        int iTrackPos=iAuto+3;
        if(LastSet.iAMRTrayConut[iTrackPos]==0 && MOT[MMAuto1_Car+iAuto].fHasTray==false)
        {
            MOT[MMAuto1+iAuto].SetTray(HAS_IC, __FUNC__);
        }
        else if((LastSet.iAMRTrayConut[iTrackPos]+1>=IniConfig.iA60NotifyQty[iTrackPos] && MOT[MMAuto1_Car+iAuto].fHasTray==true) ||    //Sam 20250814 : 修正空盤補錯問題
                (LastSet.iAMRTrayConut[iTrackPos]  >=IniConfig.iA60NotifyQty[iTrackPos] && MOT[MMAuto1_Car+iAuto].fHasTray==false))
        {
            /*  //Sam20250625 : 修正 RT 未補盤問題
            if(IsAutoReTestTray(iAuto))
            {
                if(fSCKART->iNeedRT==2) //Auto Retest 軌道只有在最後一次測試會收料才需要判斷補盤。
                {
                    MOT[MMAuto1+iAuto].SetTray(HAS_IC);
                }
            }
            else
            */
            {
                MOT[MMAuto1+iAuto].SetTray(HAS_IC, __FUNC__);
            }
        }
    }
    else
    {
        ShowMyMessage("SupplyCover error","");
    }
}
//---------------------------------------------------------------------------
bool TTeraPowerAMR::IsAutoReTestTray(int iAuto)                                 //判斷是否為 ART Retest tray
{
    if(IniConfig.bA10_AutoReTest && TestIF_File.bSCKART_EnableART)
    {
        if(iAuto>=0 && iAuto<=2)
        {
            if(BinSelect[iTestRunMode].bAutoRetest[iAuto])
            {
                return true;
            }
        }
    }
    return false;
}
//---------------------------------------------------------------------------
void TTeraPowerAMR::ARTReset()
{
    if(CosFunction.bEnableHandlerResultServer==false ||
       IniConfig.bA60EnableAMR==false)
    {
        return;
    }

    LastSet.bAMRLoaderLast=false;

    for(int i=0; i<3; i++)
    {
        if(IsAutoReTestTray(i))
        {
            LastSet.iAMRTrayConut[i+3]=1;                                       //有開啟 ART reset 會將盤全部都回收去只留下一盤當上蓋。
        }
    }
    fLotInfo->RefreshAMR();
}
//---------------------------------------------------------------------------
bool TTeraPowerAMR::NeedAMRTransport()                                          //Sam 20250423 : AMR 搬運花費時間太久可能會報警需要 By Pass
{
    if(CosFunction.bEnableHandlerResultServer==false ||
       IniConfig.bA60EnableAMR==false)
    {
        return false;
    }

    if(AMR.CheckLoaderCount()    ||     //Loader 需要補 Tray
       AMR.CheckUnloaderCount(0) ||     //Auto1 需要收 Tray
       AMR.CheckUnloaderCount(1) ||     //Auto2 需要收 Tray
       AMR.CheckUnloaderCount(2) ||     //Auto3 需要收 Tray
       LastSet.bAMRTrayFeedWait)        //Tray Feed
    {
        return  true;
    }
    return false;
}
//---------------------------------------------------------------------------
