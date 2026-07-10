// =============================================================================
//  Automation/SCK_ART.cpp  --  SCK/ATK "Auto Retest" (ART) lot-tracking, 8-FUNCTION EXTRACT
//
//  Translation wave: W5-SckArt-Translate
//  Translator: AI(W5-SckArt-Translate) 20260710
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/Automation/SCK_ART.cpp (4392 lines, cp950)
//
//  SCOPE (narrow, per hand-off -- do NOT expand): exactly the 8 functions declared in this file's
//  header (SCK_ART.h) -- see that file's header comment for the full golden-function-name mapping,
//  the "why free functions not a TfSCKART class" rationale (ODR: FormsFacade.h already owns a
//  different, offline-stub `class TfSCKART`/`fSCKART`, a hard-boundary file this wave may not touch),
//  and the complete gated-dependency list (HANA NeedToRT / AccessFile / 4 missing LastSet fields /
//  ArmData-GetPCA substrate). Every gate is local to this TU (macros just below), matches the
//  project's established "#if 0 // TODO(W7)" / W64B_-local-macro convention (see aTester_Front.cpp's
//  file-header gate list for the precedent), and is re-cited at each call site.
//
//  Big5 Chinese comments decoded via cp950 (python open(..., encoding='cp950')), verified rendered
//  correctly, and reproduced here as real UTF-8 (not an English gloss) wherever the golden line
//  carries one. ZERO U+FFFD (checked before hand-off).
// =============================================================================

#include "SCK_ART.h"

#include "MachineDefine.h"
#include "MachineType.h"      // eAutoRetest (eartInstall), eSpeedPart (InArm/OutArm), MAX_SOCKET_ROW/COL, CC_* customer codes
#include "cprod.h"             // Prod (iIsFailT6), TestIF_File (SYSTEM_TEST_IF), ArmSpeed[] (ARM_CONDITION), IniConfig, CosFunction (via cprod.h's own includes)
#include "cmydef.h"            // CUSTOMER_CODE, USE_AUTO_RETEST, K_RETRY/K_TRAY_FEED, MMSystem
#include "canary_support.h"    // LastSet (LAST_GENERAL_SET shim), ShowErrorMessage
#include "aHotPlateSubstrate.h"// TestSocket (TMyKitSuck: iShtRow/iShtCol)
#include "FormsFacade.h"       // fMain (ShowTestHeadComp, hanaART->IsHanaArtAvailable)

// ---------------------------------------------------------------------------
// SckArtState ctor -- golden TfSCKART::TfSCKART(TComponent*) (SCK_ART.cpp:36-56), the subset that
// initializes the fields this struct mirrors. iCurrentStatus has no explicit golden ctor line (the
// TForm-inherited int member zero-inits under BCB6's default component construction); 0 matches
// iLOTSTATUS_NONE, the same "no status yet" value SetLotStatus's own `default:` case maps to.
// ---------------------------------------------------------------------------
SckArtState::SckArtState()
    : sLOTSTATUS("NONE"),              // golden :51
      iCurrentStatus(0),               // golden ctor does not set this explicitly; 0==iLOTSTATUS_NONE
      iTesterType(0),                  // golden :42 (0: Flex, 1: 93K)
      iInputCount(0),                  // golden TForm int member; zero-init (no explicit ctor line)
      iFTRTCount(0),                   // golden TForm int member; zero-init (no explicit ctor line)
      iManualRejectCnt(0),              // golden TForm int member; zero-init (no explicit ctor line)
      iNeedRT(0),                      // golden TForm int member; zero-init (no explicit ctor line)
      dCurrYield(0.0),                 // golden TForm double member; zero-init (no explicit ctor line)
      iCurrent93KARTStep(0),           // golden TForm int member; zero-init (no explicit ctor line)
      bLdCntExdInputCnt(false),        // golden TForm bool member; zero-init (no explicit ctor line)
      bBackUpInArmMode(false),         // golden TForm bool member; zero-init (no explicit ctor line)
      bBackUpOutArmMode(false)         // golden TForm bool member; zero-init (no explicit ctor line)
{
}

// =============================================================================
//  GATED DEPENDENCIES -- TU-local stand-ins for substrate not yet in the translated tree.
//  See SCK_ART.h's file-head comment for the full rationale; each macro re-cites its golden home.
// =============================================================================

// ---- Gate #1: TfMainHanaART::NeedToRT() (golden main.h) -------------------------------------
//   FormsFacade.h's TfMainHanaART has IsHanaArtAvailable()/AddNewTrayHead() but not NeedToRT().
//   SAME gap already flagged + solved (default 0 = "waiting command") in csystem.cpp's
//   W7C2_TfHanaSeam::NeedToRT() -- reuse the identical default here for consistency. Since
//   IsHanaArtAvailable() is hardwired false offline (FormsFacade.cpp), the branch that calls this
//   macro is unreachable at runtime today; gating changes NO offline behaviour.
#define W5SCKART_HANA_NEEDTORT()   (0)   // golden main.h (TfMainHanaART::NeedToRT, 0/1/2) -- offline stand-in

// ---- Gate #2: TfSCKART::AccessFile(bool,int=-1) (golden SCK_ART.h:271) ----------------------
//   Not in this wave's 8-function scope (persists ART lot state to the setup file). File-I/O side
//   effect only -- does not affect CheckNeedRT's control flow / return value. No-op stand-in.
#define W5SCKART_ACCESSFILE(bRead, iAccess)   do { } while(0)   // golden TfSCKART::AccessFile(bRead,iAccess)

// ---- Gate #3: 4 LastSet fields absent from canary_support.h's LAST_GENERAL_SET shim ---------
//   golden LastSet.h:410 / :22-24 / :507 / :392. TU-local zero-initialized stand-ins (matches the
//   shim's own all-fields-default-0/false posture). CROSS-UNIT NOTE: csystem.cpp's W7C2_TfSCKARTSeam
//   already carries its OWN separate copies of iSCKART_RTUnitCount and lSCKARTBinCT[256] (same
//   golden fields) as TU-local statics -- when folding these into the real LastSet, retire BOTH
//   copies so exactly one survives.
static int  W5SckArt_LS_iSCKART_RTUnitCount = 0;             // golden LastSet.h:410
static int  W5SckArt_LS_iHdRejectATFail     = 0;             // golden LastSet.h:23
static int  W5SckArt_LS_iHdRejectATPass     = 0;             // golden LastSet.h:22
static int  W5SckArt_LS_iHdRejectBT         = 0;             // golden LastSet.h:24
static long W5SckArt_LS_lSCKARTBinCT[256]   = {0};           // golden LastSet.h:507
static bool W5SckArt_LS_bUseTestSocketEE[2][MAX_SOCKET_ROW][MAX_SOCKET_COL] = {{{false}}}; // golden LastSet.h:392
#define W5SCKART_LS_RTUNITCOUNT        W5SckArt_LS_iSCKART_RTUnitCount
#define W5SCKART_LS_HDREJECT_ATFAIL    W5SckArt_LS_iHdRejectATFail
#define W5SCKART_LS_HDREJECT_ATPASS    W5SckArt_LS_iHdRejectATPass
#define W5SCKART_LS_HDREJECT_BT        W5SckArt_LS_iHdRejectBT
#define W5SCKART_LS_BINCT(i)           W5SckArt_LS_lSCKARTBinCT[i]
#define W5SCKART_LS_USETESTSOCKETEE(k,i,j)  W5SckArt_LS_bUseTestSocketEE[k][i][j]

// ---- Gate #4: ArmData[]->ArmSKET[][]->GetPCA() (golden cSocket.h:41/91/180) ------------------
//   Per-arm/per-socket pass-count-average tracker (TArm/TMySocket, cSocket.h/.cpp) is not translated
//   at all yet (related family to the atester.cpp GetTesterResult/ProcessTestResult DEFERRED entry).
//   Stand-in returns 0.0. Given TestIF_File.dSCKART_AutoSocketOffYield is a non-negative configured
//   threshold, dTargetYield(=dMaxYield-threshold) stays <=0 with every site's GetPCA()==0.0, so
//   DoAutoSocketOff's site-disable body is naturally inert offline (branch reachable, harmless) --
//   see the call site below for the same reasoning inline.
static double W5SckArt_ArmData_GetPCA(int /*iArm*/, int /*iRow*/, int /*iCol*/) { return 0.0; }
#define W5SCKART_ARMDATA_GETPCA(iArm,iRow,iCol)  W5SckArt_ArmData_GetPCA(iArm,iRow,iCol)

// =============================================================================
//  1. SetLotStatus -- golden TfSCKART::SetLotStatus(int) (SCK_ART.cpp:639-667)
// =============================================================================
void SckArt_SetLotStatus(SckArtState &st, int iStatus, int *outComboItemIndex)
{
    switch(iStatus)
    {
        case 1:                                                                 //iLOTSTATUS_W
            st.sLOTSTATUS="LOTSTATUS_W";
            break;
        case 2:                                                                 //iLOTSTATUS_T
            st.sLOTSTATUS="LOTSTATUS_T";
            break;
        case 3:                                                                 //iLOTSTATUS_L
            st.sLOTSTATUS="LOTSTATUS_L";
            break;
        case 4:                                                                 //iLOTSTATUS_R
            st.sLOTSTATUS="LOTSTATUS_R";
            break;
        case 5:                                                                 //iLOTSTATUS_F
            st.sLOTSTATUS="LOTSTATUS_F";
            break;
        case 6:                                                                 //iLOTSTATUS_A
            st.sLOTSTATUS="LOTSTATUS_A";
            break;
        default:
            st.sLOTSTATUS="NONE";
    }
    // AI(W5-SckArt-Translate) 20260710: extract-calc-core -- golden `cbLotStatus->ItemIndex=iStatus;`
    // (VCL TComboBox selection write, SCK_ART.cpp:664) turned into this explicit out-param; the
    // caller (the real FormsFacade TfSCKART wrapper, once it exists) applies it to the real widget.
    if(outComboItemIndex!=0)
        *outComboItemIndex=iStatus;
    st.iCurrentStatus=iStatus;
//    SafeFile();                                                               // golden :666 (commented out in golden itself -- verbatim)
}

// =============================================================================
//  2. GetLotStatus -- golden TfSCKART::GetLotStatus() (SCK_ART.cpp:669-672)
// =============================================================================
AnsiString SckArt_GetLotStatus(const SckArtState &st)
{
    return st.sLOTSTATUS;
}

// =============================================================================
//  3. CheckLoadingCount -- golden TfSCKART::CheckLoadingCount() (SCK_ART.cpp:930-993)
//     0:not enough, 1:same, 2:over count
// =============================================================================
int SckArt_CheckLoadingCount(SckArtState &st)
{
    if((CosFunction.bUseSCKART &&
        USE_AUTO_RETEST==eartInstall &&
        IniConfig.bA10_AutoReTest &&
        TestIF_File.bSCKART_EnableART) ||
       IniConfig.bA65_BundleIDList)                                             //Steven 20161201 : For SCK 93K ART
    {
        if(CUSTOMER_CODE==CC_PTI && IniConfig.bB03_TesterReport)                //Sam 20240809 : PTI ART 模式
        {
            if(st.iFTRTCount==0 || TestIF_File.iSCKART_TryCnt<st.iFTRTCount)
                return 0;
        }
        #ifdef SOFT_SIMULTE
        if(fMain->hanaART->IsHanaArtAvailable()==true)
        {
            return 0;
        }
        #endif
        int iLoadingCount=LastSet.iSCKARTInputCT;                               //Steven 20170327 (wei) : Fixed for ART count
        if(iLoadingCount<st.iInputCount)
        {
            st.bLdCntExdInputCnt=false;                                        //RogerYang 20250923 : 瑞薩FT-CT
            return 0;
        }
        else if(st.iInputCount==iLoadingCount)                                  //到了的時候要一顆一顆檢查下去
        {
            st.bLdCntExdInputCnt=false;                                        //RogerYang 20250923 : 瑞薩FT-CT
            return 1;
        }
        else                                                                    //need alarm
        {
            if(TestIF_File.bRENESAS_EnableFTCT==true)                          //RogerYang 20250923 : 瑞薩FT-CT
            {
                //若實際數量比設定數量多，做到底全部完畢之後跳出，供給/不供給視窗，顯示數量多xx枚。
                //return fMain->RENESAS_Server->DoNeedSupplyOrNot();
                st.bLdCntExdInputCnt=true;
                return 1;
            }
            else
            {
                return 2;
            }
        }
    }
    else if(IniConfig.bP57LoaderAutoCleanOutByInputCT &&                        //Sam 20250605 : Loader Count AutoCleanOut
            LastSet.iP57_InputCT!=0)
    {
        int iLoadingCount=LastSet.SendCT[0];
        if(iLoadingCount<LastSet.iP57_InputCT)
        {
            return 0;
        }
        else if(LastSet.iP57_InputCT==iLoadingCount)                            //到了的時候要一顆一顆檢查下去
        {
            return 1;
        }
        else                                                                    //need alarm
        {
            return 2;
        }
    }
    return -1;
}

// =============================================================================
//  4. CheckInArmNeedVariModeFIX -- golden TfSCKART::CheckInArmNeedVariModeFIX() (SCK_ART.cpp:995-1029)
// =============================================================================
void SckArt_CheckInArmNeedVariModeFIX(SckArtState &st)                          //Sam 20211118 : 整合 ART InOut Arm 一顆一顆放
{
    int iDevice=20;                                                             //Sam 20211207 : In/Out 都倒數 20 顆 一顆一顆放

    if(CosFunction.bUseSCKART &&
       USE_AUTO_RETEST==eartInstall &&
       IniConfig.bA10_AutoReTest &&
       TestIF_File.bSCKART_EnableART &&
       TestIF_File.bRENESAS_EnableFTCT==false)                                  //RogerYang 20251111 : 瑞薩FT-CT no need
    {
        int iLoadingCount=LastSet.iSCKARTInputCT;
        if(iLoadingCount<=st.iInputCount)
        {
            if(TestIF_File.bSCKART_LotDeviceCheck)
            {
                if(iLoadingCount>st.iInputCount-iDevice)                        //數量接近時，要改用Fix方式生產
                    ArmSpeed[InArm].bVariModeFIX=true;
                else
                    st.bBackUpInArmMode=false;
            }
        }
    }
    else if(IniConfig.bP57LoaderAutoCleanOutByInputCT &&                        //Sam 20250605 : Loader Count AutoCleanOut
            LastSet.iP57_InputCT!=0)
    {
        int iLoadingCount=LastSet.SendCT[0];
        if(iLoadingCount<=LastSet.iP57_InputCT)
        {
            if(iLoadingCount>LastSet.iP57_InputCT-iDevice)                      //數量接近時，要改用Fix方式生產
                ArmSpeed[InArm].bVariModeFIX=true;
            else
                st.bBackUpInArmMode=false;
        }
    }
}

// =============================================================================
//  5. CheckOutArmNeedVariModeFIX -- golden TfSCKART::CheckOutArmNeedVariModeFIX(int) (SCK_ART.cpp:1031-1070)
// =============================================================================
void SckArt_CheckOutArmNeedVariModeFIX(SckArtState &st, int iAuto)              //Sam 20211118 : 整合 ART InOut Arm 一顆一顆放
{
    int iDevice=20;                                                             //Sam 20211207 : Out 都倒數 20 顆 一顆一顆放
    int iTotalUnloadCount=0;
    if(CUSTOMER_CODE==CC_TERAPOWER)
    {
    }
    else
    {
        iDevice=30;
    }

    if(CosFunction.bUseSCKART &&
       USE_AUTO_RETEST==eartInstall &&
       IniConfig.bA10_AutoReTest &&
       TestIF_File.bSCKART_EnableART)
    {
        for(int i=0; i<10; i++)
        {
            iTotalUnloadCount+=W5SCKART_LS_BINCT(i);                            // golden :1050 LastSet.lSCKARTBinCT[i] -- TODO(W7-LastSet), see gate #3
        }

        if(Prod.iIsFailT6[iAuto]==1)                                            //Steven 20240105 : Prod.bIsPass --> Prod.iIsFailT6  //Fail IC 強制一顆一顆放
            ArmSpeed[OutArm].bVariModeFIX=true;
        else
            ArmSpeed[OutArm].bVariModeFIX=false;

        int iLoadingCount=LastSet.iSCKARTInputCT;
        if(st.iInputCount>=iLoadingCount)
        {
            if(TestIF_File.bSCKART_LotDeviceCheck)
            {
                if(st.iInputCount-iTotalUnloadCount<=iDevice)                   //數量接近時，要改用Fix方式生產
                    ArmSpeed[OutArm].bVariModeFIX=true;
                else
                    st.bBackUpOutArmMode=false;
            }
        }
    }
}

// =============================================================================
//  6. CheckNeedRT -- golden TfSCKART::CheckNeedRT() (SCK_ART.cpp:1072-1230)
// =============================================================================
void SckArt_CheckNeedRT(SckArtState &st)
{
    int iDevice=0;
    if(CUSTOMER_CODE==CC_TERAPOWER)                                             //Sam 20211105 : TPW 建勳要求改為300顆
        iDevice=300;
    else
        iDevice=20;

    int iRet=K_RETRY;

    if(fMain->hanaART->IsHanaArtAvailable()==true)                              //Steven 20250414 : HANA ART Function
    {
        int iHanaNeedToRt=W5SCKART_HANA_NEEDTORT();                            //0:Waiting command, 1:Need to RT, 2:Lot End -- TODO(W7-FormsFacade), see gate #1
        if(iHanaNeedToRt==0)
        {
            return;
        }
        else
        {
            st.iFTRTCount++;
            W5SCKART_ACCESSFILE(false, -1);                                    // golden :1093 AccessFile(false,-1) -- TODO(W7), see gate #2
            if(iHanaNeedToRt==2)                                                //Steven 20251219 : fixed for Hana ART
            {
                st.iNeedRT=0;
            }
            else
            {
                st.iNeedRT=1;
                if(st.iFTRTCount>TestIF_File.iSCKART_TryCnt)                    //Steven 20251020 : fixed for Hana ART
                {
                    st.iNeedRT=0;
                }
                else if(st.iFTRTCount==TestIF_File.iSCKART_TryCnt)              //最後一次要執行Final ART
                {
                    st.iNeedRT=2;
                }
            }
        }
    }
    else if(CosFunction.iAutoRetestTCPmode==2 &&                                //RogerYang 20251008 : 瑞薩FT-CT
            TestIF_File.bRENESAS_EnableFTCT==true)
    {
//        if(fMain->RENESAS_Server->iNeedToRT==0)  //收到"20"就會設定           // golden :1115-1120 -- verbatim (commented out in golden itself)
//            iNeedRT=0;
//        else
//            iNeedRT=1;

//        iFTRTCount++;
    }
    else
    {
        st.iFTRTCount++;

        if(st.iFTRTCount<=1 &&
           TestIF_File.bLowYieldForArtFT==true &&                              //Steven 20220208 : Add for ATK, ART low Yield
           TestIF_File.dLowYieldForArtFT<st.dCurrYield)
        {
            iRet=ShowErrorMessage("WAR07361", K_RETRY|K_TRAY_FEED, MMSystem, false, "CheckNeedRT");
        }

        if(iRet==K_TRAY_FEED)
        {
            st.iNeedRT=0;                                                      //Low Yield, 不用RT
        }
        else if(st.iFTRTCount>TestIF_File.iSCKART_TryCnt)                       //Steven 20250325 : fixed for ART
        {
            if(CUSTOMER_CODE==CC_TERAPOWER)
                st.iNeedRT=2;                                                  //Sam 20250702 : Final RT 後還會++ iNeedRT 狀態還需要維持"2"
            else
                st.iNeedRT=0;
        }
        else if(st.iFTRTCount==TestIF_File.iSCKART_TryCnt)                      //最後一次要執行Final ART
        {
            st.iNeedRT=2;
        }
        else if(st.iFTRTCount<TestIF_File.iSCKART_TryCnt &&                    //當次數還沒到
                st.dCurrYield<TestIF_File.dSCKART_Yield)                        //而且Yield還是小於設定
        {
            st.iNeedRT=1;
        }
        else if(st.dCurrYield>=TestIF_File.dSCKART_Yield &&                     //Yield已經到了
                st.iNeedRT!=2)                                                  //而且不是已經執行過Final ART, 要執行一次的Final ART
        {
            st.iNeedRT=2;
        }
        else
        {
            st.iNeedRT=0;                                                      //不用RT
        }
    }

    if(TestIF_File.bRENESAS_EnableFTCT==false)                                  //RogerYang 20251013 : RogerYang 瑞薩FT-CT 不在這裡變更
    {
        if(st.iFTRTCount==1 && st.iNeedRT==1)
            st.iCurrent93KARTStep=5;
        else if(st.iNeedRT==1)
            st.iCurrent93KARTStep=10;
    }

    if(st.iNeedRT!=0)
    {
        if(TestIF_File.bRENESAS_EnableFTCT==true)                              //RogerYang 20251015 : 瑞薩FTCT不能在這裡清零(20指令會把iSCKART_RTUnitCount設為零，重複再接收20會變成沒有RT)
        {
            if(st.iInputCount==0 &&                                            //沒有人員手動輸入的情況(30報表產出會清空)，且真的有RT數量
                W5SCKART_LS_RTUNITCOUNT!=0)                                     // golden :1177 LastSet.iSCKART_RTUnitCount -- TODO(W7-LastSet), see gate #3
            {
                st.iInputCount=W5SCKART_LS_RTUNITCOUNT;
                st.iInputCount+=(W5SCKART_LS_HDREJECT_ATFAIL+                   // golden :1180-1182 LastSet.iHdRejectATFail/ATPass/BT -- TODO(W7-LastSet), see gate #3   //Fail收納(＋JAM去除數)"＝投入數(Qty)
                                 W5SCKART_LS_HDREJECT_ATPASS+
                                 W5SCKART_LS_HDREJECT_BT);
            }
            W5SCKART_LS_RTUNITCOUNT=0;
            W5SCKART_ACCESSFILE(false, -1);                                    // golden :1185 -- TODO(W7), see gate #2
        }
        else
        {
            if(W5SCKART_LS_RTUNITCOUNT==0)
            {
                st.iNeedRT=0;
            }

            if(st.iTesterType==1)
            {
                if(IniConfig.bSPILFunction)                                    //JerryYang 20220923 : SPIL手動輸入reject數量(要重測的)
                {
                    st.iInputCount=W5SCKART_LS_RTUNITCOUNT+st.iManualRejectCnt;
                    st.iManualRejectCnt=0;
                }
                else
                {
                    st.iInputCount=W5SCKART_LS_RTUNITCOUNT;
                }
                W5SCKART_LS_RTUNITCOUNT=0;
                W5SCKART_ACCESSFILE(false, 1);                                  // golden :1206 -- TODO(W7), see gate #2
            }
        }

        if(W5SCKART_LS_RTUNITCOUNT<iDevice &&
           W5SCKART_LS_RTUNITCOUNT!=0)                                          //Steven 20170717 (wei) : RT的時候可能數量比較少,就要使用Fix模式
        {
            ArmSpeed[InArm].bVariModeFIX=true;
            if(CUSTOMER_CODE==CC_TERAPOWER)                                     //Sam 20211108 : TPW OutArm 也要一顆一顆放
                ArmSpeed[OutArm].bVariModeFIX=true;
        }
        else
        {
            ArmSpeed[InArm].bVariModeFIX=st.bBackUpInArmMode;
            if(CUSTOMER_CODE==CC_TERAPOWER)                                     //Sam 20211108 : TPW OutArm 也要一顆一顆放
                ArmSpeed[OutArm].bVariModeFIX=st.bBackUpOutArmMode;
        }
    }
    else
    {
        ArmSpeed[InArm].bVariModeFIX=st.bBackUpInArmMode;
        if(CUSTOMER_CODE==CC_TERAPOWER)                                         //Sam 20211108 : TPW OutArm 也要一顆一顆放
            ArmSpeed[OutArm].bVariModeFIX=st.bBackUpOutArmMode;
    }
}

// =============================================================================
//  7. DoAutoSocketOff -- golden TfSCKART::DoAutoSocketOff(bool) (SCK_ART.cpp:1232-1359)
// =============================================================================
void SckArt_DoAutoSocketOff(SckArtState &st, bool bAllSiteOn)
{
    int iSiteCount=0;                                                           //JerryYang 20220923 : 最小關site數量
    double dMaxYield=0, dYield[2][MAX_SOCKET_ROW][MAX_SOCKET_COL];
    double dTargetYield;

    if(TestIF_File.bSCKART_AutoSocketOff)
    {
        if(bAllSiteOn==false && st.iFTRTCount!=0)
        {
            if(TestIF_File.iSCKART_AutoSocketOffMode==0)                       //Auto Head
            {
                for(int i=0; i<TestSocket.iShtRow; i++)
                {
                    for(int j=0; j<TestSocket.iShtCol; j++)
                    {
                        // golden :1248/:1254 ArmData[k]->ArmSKET[i][j]->GetPCA() -- TODO(W7), see
                        // gate #4. Stand-in returns 0.0 for every site, so dMaxYield stays 0 and the
                        // dTargetYield>0 body below is naturally never entered (see gate #4 rationale).
                        dYield[0][i][j]=W5SCKART_ARMDATA_GETPCA(0,i,j);
                        if(dYield[0][i][j]>dMaxYield)
                        {
                            dMaxYield=dYield[0][i][j];
                        }

                        dYield[1][i][j]=W5SCKART_ARMDATA_GETPCA(1,i,j);
                        if(dYield[1][i][j]>dMaxYield)
                        {
                            dMaxYield=dYield[1][i][j];
                        }
                    }
                }

                dTargetYield=dMaxYield-TestIF_File.dSCKART_AutoSocketOffYield;

                if(dTargetYield>0)
                {
                    for(int k=0; k<2; k++)
                    {
                        for(int i=0; i<TestSocket.iShtRow; i++)
                        {
                            for(int j=0; j<TestSocket.iShtCol; j++)
                            {
                                if(dYield[k][i][j]<dTargetYield)
                                {
                                    LastSet.bUseTestSocket[k][i][j]=false;
                                }
                            }
                        }
                    }
                }
            }
            else if(TestIF_File.iSCKART_AutoSocketOffMode==1)                  //Auto Socket
            {
                for(int i=0; i<TestSocket.iShtRow; i++)
                {
                    for(int j=0; j<TestSocket.iShtCol; j++)
                    {
                        iSiteCount++;                                          //JerryYang 20220923 : 最小關site數量
                        dYield[0][i][j]=0;
                        // golden :1291-1294 ArmData[k]->ArmSKET[i][j]->GetPCA() -- TODO(W7), see gate #4.
                        if(LastSet.bUseTestSocket[0][i][j] && LastSet.bUseTestSocket[1][i][j])
                            dYield[0][i][j]=(W5SCKART_ARMDATA_GETPCA(0,i,j)+W5SCKART_ARMDATA_GETPCA(1,i,j))/2.0;
                        else if(LastSet.bUseTestSocket[0][i][j])
                            dYield[0][i][j]=W5SCKART_ARMDATA_GETPCA(0,i,j);
                        else
                            dYield[1][i][j]=W5SCKART_ARMDATA_GETPCA(1,i,j);

                        if(dYield[0][i][j]>dMaxYield)
                        {
                            dMaxYield=dYield[0][i][j];
                        }
                    }
                }

                dTargetYield=dMaxYield-TestIF_File.dSCKART_AutoSocketOffYield;
                if(dTargetYield>0)
                {
                    for(int i=0; i<TestSocket.iShtRow; i++)
                    {
                        for(int j=0; j<TestSocket.iShtCol; j++)
                        {
                            if(dYield[0][i][j]<dTargetYield)
                            {
                                iSiteCount--;                                   //JerryYang 20220923 : 最小關site數量
                                LastSet.bUseTestSocket[0][i][j]=false;
                                LastSet.bUseTestSocket[1][i][j]=false;
                            }
                        }
                    }
                }

                if(iSiteCount<TestIF_File.iSCKART_MinAutoCloseSite)             //JerryYang 20220923 : 最小關site數量
                {
                    ShowErrorMessage("WAR0724", K_RETRY, MMInterface);
                }
            }
            else                                                               //Manual
            {
            }
            fMain->ShowTestHeadComp(false);
        }
        else //if(iFTRTCount==0)                                                //Final RT後, 全開
        {
            for(int i=0; i<TestSocket.iShtRow; i++)
            {
                for(int j=0; j<TestSocket.iShtCol; j++)
                {
                    if(IniConfig.bI35UseThirdSiteControlByEngineer)             //Steven 20180829 : #P180828-ATK-H9-02 Auto Enable Site after finish ART even disable site by using "Eng Site" (White Color)
                    {
                        if(TestIF_File.iSiteMap[i][j]!=0)
                        {
                            // golden :1340-1343 LastSet.bUseTestSocketEE[k][i][j] -- TODO(W7-LastSet), see gate #3
                            if(W5SCKART_LS_USETESTSOCKETEE(0,i,j)==true)
                                LastSet.bUseTestSocket[0][i][j]=true;
                            if(W5SCKART_LS_USETESTSOCKETEE(1,i,j)==true)
                                LastSet.bUseTestSocket[1][i][j]=true;
                        }
                    }
                    else
                    {
                        if(TestIF_File.iSiteMap[i][j]!=0)
                        {
                            LastSet.bUseTestSocket[0][i][j]=true;
                            LastSet.bUseTestSocket[1][i][j]=true;
                        }
                    }
                }
            }
            fMain->ShowTestHeadComp(true);
        }
    }
}

// =============================================================================
//  8. DoChkInputCntAlarm -- golden TfSCKART::DoChkInputCntAlarm(bool) (SCK_ART.cpp:4359-4391)
// =============================================================================
bool SckArt_DoChkInputCntAlarm(const SckArtState &st, bool bExcess)             //RogerYang 20251224 : Rf360需求 lotcheck加入上下限設定
{
    if(CUSTOMER_CODE==CC_QUALCOMM)
    {
        int iLoadingCount=LastSet.iSCKARTInputCT;
        if(bExcess==true)  //數量超過
        {
            if(iLoadingCount<=st.iInputCount+TestIF_File.iSCKLdCntLimP)        //允許超過設定數量"前"不報警
            {
                return false;
            }
            else
            {
                return true;
            }
        }
        else               //數量不足
        {
            if(iLoadingCount>=st.iInputCount-TestIF_File.iSCKLdCntLimN)        //允許超過設定數量"後"不報警
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    }
    else
    {
        return true;
    }
}
