// =============================================================================
//  cSocket.h  --  per-socket / per-arm / per-lot pass-fail-bin counters
//
//  Faithful translation of golden cSocket.h (186 lines, BCB6, Big5/cp950 -- but
//  this header has no Chinese text of its own, only the English/romanized
//  engineer-name comments quoted below).
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2 (group "socket").
//
//  ROLE: declares the four value types cSocket.cpp defines and that the rest
//  of the tree reaches through the four extern globals at the bottom of this
//  file:
//    TMySocket    -- one physical socket's pass/fail/bin/IF-error counters
//                     (golden :9-66).
//    TArm         -- one arm's MAX_SOCKET_ROW x MAX_SOCKET_COL grid of
//                     TMySocket, plus the arm-level roll-up counters and the
//                     four TStringList mirrors (sPass/sFail/sTotal/iIFErr)
//                     SECS GEM reads per-site (golden :68-114).
//    TLotSummary  -- per-lot / per-bin / (optionally) per-inner-lot count
//                     accumulation used by SECS GEM lot summary and by the
//                     ATK/ART "by-lot" 2D-mapping cross-check (golden :116-142).
//    TEST_CATEGORY-- the whole-machine snapshot struct atester_ProcessCount.cpp
//                     and Automation/SCK_ART_Remainder.cpp read for By-Bin /
//                     By-Site / By-Head yield displays (golden :144-174).
//
//  WAVE SCOPE -- ACTIVE vs satisfied-by-shim: this header is a pure data-type
//  declaration file (no function bodies); every member function it declares
//  is DEFINED in cSocket.cpp (see that file's own WAVE SCOPE list for the full
//  63-function inventory). Nothing here is shimmed or gated.
//
//  VCL/Borland conversions:
//    * `_fastcall` on every method (golden :31-65/:89-113/:120-137) dropped --
//      this tree neutralizes it tree-wide; no method is `__property` or
//      `__published`.
//    * `TStringList*` members (sBinPassFail/sBinCT/sPass/sFail/sTotal/iIFErr)
//      keep golden's raw-pointer-owned-by-ctor/dtor shape verbatim (vclcompat
//      TStringList, not std::string) -- see cSocket.cpp's ctors/dtors.
//    * AnsiString (vclcompat), not std::string, for Name/FileName (golden :77/:89).
//
//  ***************************************************************************
//  [RESOLVED 20260820] INTEGRATION CONFLICT -- kept for history, no longer live
//  ***************************************************************************
//  AI(W906-StaleGates) 20260820: the integrate step this warning demanded has
//  HAPPENED -- SCK_ART_Remainder.cpp's stub definition was deleted (its :361
//  comment records the removal) and nm on libht9045_sm.a confirms
//  cSocket.cpp.obj owns the one real `TEST_CATEGORY TastCategory` while
//  SCK_ART_Remainder.cpp.obj only references it. The paragraph below is the
//  original warning, preserved as history; do not re-act on it.
//  ---------------------------------------------------------------------------
//  This header's `extern TEST_CATEGORY TastCategory;` (golden :177, mirrored
//  below) COLLIDES at link time with Automation/SCK_ART_Remainder.cpp:346's
//  PRE-EXISTING real definition:
//      `W5SckArtRem_TastCategoryStub TastCategory = {};`
//  That stub (and its `.UpdataCount()` no-op override, SCK_ART_Remainder.cpp:345)
//  was deliberately created BECAUSE cSocket.cpp/.h had no port yet -- its own
//  "Gate #16" comment (SCK_ART_Remainder.cpp:339) says so explicitly, and
//  atester_ProcessCount.cpp / Automation/SCK_ART.cpp / auto9045.cpp all read
//  the SAME global name `TastCategory` expecting whichever definition is
//  linked in. Two definitions of a global named `TastCategory` in different
//  translation units is an ODR violation the linker will reject the moment
//  both cSocket.cpp and Automation/SCK_ART_Remainder.cpp join the same
//  target. NOT fixed here -- Automation/SCK_ART_Remainder.cpp is a
//  pre-existing file this wave's write boundary forbids touching (hard rule
//  7). The integrate step must delete SCK_ART_Remainder.cpp's stub
//  definition (and its `W5SckArtRem_TastCategoryStub` type / the
//  `UpdataCount` no-op override) and let every caller bind to this file's
//  real `TEST_CATEGORY TastCategory` instead once cSocket.cpp/.h are added to
//  ht9045_sm's CMakeLists.txt sources.
//  No other extern below this banner collides: `LotSummary` has no real
//  definition anywhere in the current port tree (only inside a dead `#if 0`
//  block in cpublic.cpp and comments/macros in csystem.cpp) and
//  `ArmData`/`ArmDataLot`/`ArmHistory`/`ArmData_AutoClean`/
//  `OldControlBinCategory`/`NowControlBinCategory` are not defined anywhere
//  else either (grepped the whole tree, excluding build*/, 20260807).
//  ***************************************************************************
//
//  Big5: this header has no Chinese comments to preserve (checked cp950-decoded
//  golden text line by line). Final gate: ZERO U+FFFD (vacuously true).
// =============================================================================
// BCB6 ORIGINAL include block (mirrored as a comment for provenance):
//   //#include "MachineDefine.h"
//   #include "MachineType.h"                    //Steven 20130809 : OK
// =============================================================================
#ifndef cSocketH
#define cSocketH

#include "MachineType.h"   // TEST_MAX_BIN, MAX_SOCKET_ROW, MAX_SOCKET_COL, eTrayCount
//------------------------------------------------------------------------------
class TMySocket
{
    private:
    public:
        unsigned long Pass;
        unsigned long Fail;
        unsigned long Total;
        int SetBinPassFail[TEST_MAX_BIN];
        unsigned int iBinCT[TEST_MAX_BIN];
        unsigned int iIFErr;

        int SetByBinLowYieldPassFail[TEST_MAX_BIN];                             //Steven 20140828 : By Bin Yield Monitor
        int SetByBinArmYieldPassFail[TEST_MAX_BIN];                             //Steven 20140828 : By Bin Arm Yield Monitor
        int SetByBinSiteYieldPassFail[TEST_MAX_BIN];                            //Steven 20140828 : By Bin Site Yield Monitor
        unsigned long iByBinLowYieldPass;                                       //Steven 20140828 : By Bin Yield Monitor
        unsigned long iByBinArmYieldPass;                                       //Steven 20140828 : By Bin Arm Yield Monitor
        unsigned long iByBinSiteYieldPass;                                      //Steven 20140828 : By Bin Site Yield Monitor

        unsigned long BySitePass;                                               //kevin 20130710 by site yield record
        unsigned long BySiteFail;                                               //kevin 20130710 by site yield record
        unsigned long BySiteTotal;                                              //kevin 20130710 by site yield record alarm 後清除

        TMySocket();
        ~TMySocket();

        TStringList *sBinPassFail;
        TStringList *sBinCT;

        void ClearALLCT();
//        void SetbBinCodeStatus(int Bin, int Status);                          //set BinPassFail
        void SetbBinCodeStatus(int Bin, int Status, bool bByBinLowYieldPass, bool bByBinArmYieldPass, bool ByBinSiteYieldPass);//set BinPassFail
        void SetTesterBin(int Bin);                                             //Tester測試完成,將bin傳入socket
        virtual double GetPCA();                                                //==>%
        unsigned long GetPassCT();
        unsigned long GetFailCT();
        unsigned long GetTotal();
        unsigned long GetSelBinCT(int Bin);
        unsigned long GetSelTrayCT(int Tray);
        unsigned long GetIFError();

        unsigned long GetByBinLowYieldPassCT();                                 //Steven 20140828 : By Bin Yield Monitor
        unsigned long GetByBinArmYieldPassCT();                                 //Steven 20140828 : By Bin Arm Yield Monitor
        unsigned long GetByBinSiteYieldPassCT();                                //Steven 20140828 : By Bin Site Yield Monitor
        void SetByBinLowYieldPassCT(unsigned long Value);                       //Steven 20140828 : By Bin Yield Monitor
        void SetByBinArmYieldPassCT(unsigned long Value);                       //Steven 20140828 : By Bin Arm Yield Monitor
        void SetByBinSiteYieldPassCT(unsigned long Value);                      //Steven 20140828 : By Bin Site Yield Monitor

        double GetByBinLowYieldPassPCA();                                       //Steven 20140828 : By Bin Yield Monitor
        double GetByBinArmYieldPassPCA();                                       //Steven 20140828 : By Bin Arm Yield Monitor
        double GetByBinSiteYieldPassPCA();                                      //Steven 20140828 : By Bin Site Yield Monitor

        void SetPassCT(double Value);
        void SetFailCT(double Value);
        void SetBinCT(int Bin, double Value);
        void SetIFErr(double Value);                                            //Steven 20110801
        void ClearBySite();                                                     //kevin 20130710 by Site 計數清除
        double GetBySitePCA();                                                  //kevin 20130710 by sit計數
};
//------------------------------------------------------------------------------
class TArm
{
    private:
    public:
        unsigned long Pass;
        unsigned long Fail;
        unsigned long Total;
        int iSKETInArmCT;
        unsigned long iContactCT;
        AnsiString Name;
        bool bHasFile;

        unsigned long BySitePass;                                               //kevin 20130710 by site yield record
        unsigned long BySiteFail;                                               //kevin 20130710 by site yield record
        unsigned long BySiteTotal;                                              //kevin 20130710 by site yield record alarm 後清除

        TStringList *sPass;                                                     //Steven 20140509 : For Secs GEM
        TStringList *sFail;
        TStringList *sTotal;
        TStringList *iIFErr;

        TArm(AnsiString FileName);
        ~TArm();
        TMySocket *ArmSKET[MAX_SOCKET_ROW][MAX_SOCKET_COL];
        virtual double GetPCA();                                                //==>%
        unsigned long GetPassCT();
        unsigned long GetFailCT();
        unsigned long GetTotalCT();
        unsigned long GetSelBin(int Bin);                                       //Sam 20240131 : 取得此  Arm 的 Bin 數量
        void ClearALLCT();
        void ClearALLCT(int ROW, int COL);                                      //Steven 20140509 : For Secs GEM
        void SetArmSKTData(int ROW, int iCol, int Data);
        void InitContactCT();
        void SetContactCT(int CT);
        unsigned long GetContactCT();
        void SetArmBinCodeStatus(int ROW, int iCol, int BinValue, int Status, bool bByBinLowYieldPass, bool bByBinArmYieldPass, bool ByBinSiteYieldPass);
        void ReadFile();                                                        //Steven 20110801 : 改成初始化後讀檔
        void WriteFile();                                                       //Steven 20110801 : 改成初始化後讀檔
        void ClearBySite();                                                     //kevin 20130710 by Site 計數清除
        double GetBySitePCA();                                                  //kevin 20130710 by sit計數

        void SetPassCT(int ROW, int COL, double Value);                         //Steven 20140509 : For Secs GEM
        void SetFailCT(int ROW, int COL, double Value);                         //Steven 20140509 : For Secs GEM
        void SetBinCT(int ROW, int COL, int Bin, double Value);                 //Steven 20140509 : For Secs GEM
        void SetIFErr(int ROW, int COL, double Value);                          //Steven 20140509 : For Secs GEM
        double GetByBinLowYieldPCA();                                           //Steven 20141125
};
//------------------------------------------------------------------------------
class TLotSummary
{
    private:
    public:
        TLotSummary();
        ~TLotSummary();

        bool bIsRTBin[TEST_MAX_BIN];
        int iCountCategory[MAX_SOCKET_ROW*MAX_SOCKET_COL][TEST_MAX_BIN];        //[Row*Col][Category]  //Steven 20190816 : 改成Site number
        int iTotalCategory[TEST_MAX_BIN];
        int iLastTotalCategory[TEST_MAX_BIN];
        int iLoadTotal;
//        int iTotalSocket;

        void ClearAllData();
        void ClearRTData();
        void SetIsRTBin();
        void AddCount(int iSite, int iBin);                                     //Steven 20190726 : ATK ART Lot Count
        void AddByLotCount(int iSite, int iBin, AnsiString s2DID);
        void AddByLotLoadCount(AnsiString s2DID);
        void ReadFile();                                                        //Steven 20190726 : ATK ART Lot Count
        void WriteFile();                                                       //Steven 20190726 : ATK ART Lot Count

        int iByLotCountCategory[5][MAX_SOCKET_ROW*MAX_SOCKET_COL][TEST_MAX_BIN];//[Row*Col][Category]  //Steven 20190816 : 改成Site number
        int iByLotTotalCategory[5][TEST_MAX_BIN];
        int iByLotLoadCount[5];
};
//------------------------------------------------------------------------------
typedef struct
{
    int iCountCategory   [2][MAX_SOCKET_ROW][MAX_SOCKET_COL][TEST_MAX_BIN];     //[Arm][Row][Col][Category]
    int iCountHeadTotal  [2][MAX_SOCKET_ROW][MAX_SOCKET_COL];                   //[Arm][Row][Col]
    int iCountSocketTotal   [MAX_SOCKET_ROW][MAX_SOCKET_COL];                   //[Row][Col]
    int iCountPassHead   [2][MAX_SOCKET_ROW][MAX_SOCKET_COL];                   //[Arm][Row][Col]
    int iCountPassSocket    [MAX_SOCKET_ROW][MAX_SOCKET_COL];                   //[Row][Col]
    int iTotalCategory[TEST_MAX_BIN];

    int iBySiteCate     [MAX_SOCKET_ROW*MAX_SOCKET_COL][TEST_MAX_BIN];          //[SiteMap][Category]
    int iBySiteTotal    [MAX_SOCKET_ROW*MAX_SOCKET_COL];
    int iBySitePass     [MAX_SOCKET_ROW*MAX_SOCKET_COL];
    int iBySiteFail     [MAX_SOCKET_ROW*MAX_SOCKET_COL];
    double dBySiteCate  [MAX_SOCKET_ROW*MAX_SOCKET_COL][TEST_MAX_BIN];
    double dBySitePass  [MAX_SOCKET_ROW*MAX_SOCKET_COL];
    double dBySiteFail  [MAX_SOCKET_ROW*MAX_SOCKET_COL];

    int iUnloadCnt[eTrayCount];

    int iTotalSocket;
    int iPassSocket;
    int iFailSocket;
    int iRejectCount;

    double dPassYield;
    double dFailYield;

    void ClearCount();
    void UpdataCount(bool bCheckYield=false);
    void UpdataYield();
} TEST_CATEGORY;
//------------------------------------------------------------------------------
extern TLotSummary LotSummary;
extern TEST_CATEGORY TastCategory;
extern TEST_CATEGORY OldControlBinCategory;                                     //Sam 20200525 : Control Bin
extern TEST_CATEGORY NowControlBinCategory;                                     //Sam 20200525 : Control Bin
extern TArm *ArmData[3];
extern TArm *ArmDataLot[3];                                                     //Steven 20250603 : by lot summary
extern TArm *ArmHistory[3];
extern TArm *ArmData_AutoClean[3];                                              //ChungHung 20131225 add
//------------------------------------------------------------------------------
#endif
