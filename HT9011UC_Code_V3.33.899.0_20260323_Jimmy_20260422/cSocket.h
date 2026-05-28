//------------------------------------------------------------------------------

#ifndef cSocketH
#define cSocketH

//#include "MachineDefine.h"
#include "MachineType.h"                                                        //Steven 20130809 : OK
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

        _fastcall TMySocket::TMySocket();
        _fastcall ~TMySocket();

        TStringList *sBinPassFail;
        TStringList *sBinCT;

        void _fastcall ClearALLCT();
//        void _fastcall SetbBinCodeStatus(int Bin, int Status);                //set BinPassFail
        void _fastcall SetbBinCodeStatus(int Bin, int Status, bool bByBinLowYieldPass, bool bByBinArmYieldPass, bool ByBinSiteYieldPass);//set BinPassFail
        void _fastcall SetTesterBin(int Bin);                                   //Tester測試完成,將bin傳入socket
        virtual double _fastcall GetPCA();                                      //==>%
        unsigned long _fastcall GetPassCT();
        unsigned long _fastcall GetFailCT();
        unsigned long _fastcall GetTotal();
        unsigned long _fastcall GetSelBinCT(int Bin);
        unsigned long _fastcall GetSelTrayCT(int Tray);
        unsigned long _fastcall GetIFError();

        unsigned long _fastcall GetByBinLowYieldPassCT();                       //Steven 20140828 : By Bin Yield Monitor
        unsigned long _fastcall GetByBinArmYieldPassCT();                       //Steven 20140828 : By Bin Arm Yield Monitor
        unsigned long _fastcall GetByBinSiteYieldPassCT();                      //Steven 20140828 : By Bin Site Yield Monitor
        void _fastcall SetByBinLowYieldPassCT(unsigned long Value);             //Steven 20140828 : By Bin Yield Monitor
        void _fastcall SetByBinArmYieldPassCT(unsigned long Value);             //Steven 20140828 : By Bin Arm Yield Monitor
        void _fastcall SetByBinSiteYieldPassCT(unsigned long Value);            //Steven 20140828 : By Bin Site Yield Monitor

        double _fastcall GetByBinLowYieldPassPCA();                             //Steven 20140828 : By Bin Yield Monitor
        double _fastcall GetByBinArmYieldPassPCA();                             //Steven 20140828 : By Bin Arm Yield Monitor
        double _fastcall GetByBinSiteYieldPassPCA();                            //Steven 20140828 : By Bin Site Yield Monitor

        void _fastcall SetPassCT(double Value);
        void _fastcall SetFailCT(double Value);
        void _fastcall SetBinCT(int Bin, double Value);
        void _fastcall SetIFErr(double Value);                                  //Steven 20110801
        void _fastcall ClearBySite();                                           //kevin 20130710 by Site 計數清除
        double _fastcall GetBySitePCA();                                        //kevin 20130710 by sit計數
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

        _fastcall TArm(AnsiString FileName);
        _fastcall ~TArm();
        TMySocket *ArmSKET[MAX_SOCKET_ROW][MAX_SOCKET_COL];
        virtual double _fastcall GetPCA();                                      //==>%
        unsigned long _fastcall GetPassCT();
        unsigned long _fastcall GetFailCT();
        unsigned long _fastcall GetTotalCT();
        unsigned long _fastcall GetSelBin(int Bin);                             //Sam 20240131 : 取得此  Arm 的 Bin 數量
        void _fastcall ClearALLCT();
        void _fastcall ClearALLCT(int ROW, int COL);                            //Steven 20140509 : For Secs GEM
        void _fastcall SetArmSKTData(int ROW, int iCol, int Data);
        void _fastcall InitContactCT();
        void _fastcall SetContactCT(int CT);
        unsigned long _fastcall GetContactCT();
        void _fastcall SetArmBinCodeStatus(int ROW, int iCol, int BinValue, int Status, bool bByBinLowYieldPass, bool bByBinArmYieldPass, bool ByBinSiteYieldPass);
        void _fastcall ReadFile();                                              //Steven 20110801 : 改成初始化後讀檔
        void _fastcall WriteFile();                                             //Steven 20110801 : 改成初始化後讀檔
        void _fastcall ClearBySite();                                           //kevin 20130710 by Site 計數清除
        double _fastcall GetBySitePCA();                                        //kevin 20130710 by sit計數

        void _fastcall SetPassCT(int ROW, int COL, double Value);               //Steven 20140509 : For Secs GEM
        void _fastcall SetFailCT(int ROW, int COL, double Value);               //Steven 20140509 : For Secs GEM
        void _fastcall SetBinCT(int ROW, int COL, int Bin, double Value);       //Steven 20140509 : For Secs GEM
        void _fastcall SetIFErr(int ROW, int COL, double Value);                //Steven 20140509 : For Secs GEM
        double _fastcall GetByBinLowYieldPCA();                                 //Steven 20141125
};
//------------------------------------------------------------------------------
class TLotSummary
{
    private:
    public:
        _fastcall TLotSummary();
        _fastcall ~TLotSummary();

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
        void _fastcall ReadFile();                                              //Steven 20190726 : ATK ART Lot Count
        void _fastcall WriteFile();                                             //Steven 20190726 : ATK ART Lot Count

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

