//---------------------------------------------------------------------------
#ifndef mytrayH
#define mytrayH

#define _MAX_COL_ITEM 30
#define _MAX_ROW_ITEM 70
#include "cmydef.h"
#include "MyProductionRecord.h"

//---------------------------------------------------------------------------
class TMyTray
{
    public :
        __fastcall TMyTray();
        __fastcall ~TMyTray();

        int  XItem;
        int  YItem;
        int  XBItem;
        int  YBItem;
        int  XBWidth;
        int  YBWidth;

        int  Data[_MAX_COL_ITEM][_MAX_ROW_ITEM];
        int  iWhichSite[_MAX_COL_ITEM][_MAX_ROW_ITEM];
        int  iNeedRotAng[_MAX_COL_ITEM][_MAX_ROW_ITEM];                         //預計旋轉的角度   //Steven 20170425 (wei) : Add rotate motor
        int  iCurrRotAng[_MAX_COL_ITEM][_MAX_ROW_ITEM];                         //目前產品的角度
        int  iWhichIndex[_MAX_COL_ITEM][_MAX_ROW_ITEM];                         //ChungHung 20150205 add for ATK
        int  iBinCode[_MAX_COL_ITEM][_MAX_ROW_ITEM];
        int  BufferData[_MAX_COL_ITEM][_MAX_ROW_ITEM];
        int  iTarget[_MAX_COL_ITEM][_MAX_ROW_ITEM];                             //Steven 20150915 : For TSMC 手動整盤功能
        int  iCleanCount[_MAX_COL_ITEM][_MAX_ROW_ITEM];
        int  iWhichAuto[_MAX_COL_ITEM][_MAX_ROW_ITEM];                          //JerryYang 20220909 : add magazine
        int  iBinData[_MAX_COL_ITEM][_MAX_ROW_ITEM];                            //JerryYang 20220909 : add magazine
        int  iAOIResult[_MAX_COL_ITEM][_MAX_ROW_ITEM];                          //Sam 20240325 : 新增 DamageTrayMapping 功能
        int  SiteMapData[_MAX_COL_ITEM][_MAX_ROW_ITEM];                         //Ifor 20210423 add: Tray Mapping Data

        bool b2DIDNG[_MAX_COL_ITEM][_MAX_ROW_ITEM];                             //Steven 20200611 : for Murata, 2DID NG不測試
        bool bFliped[_MAX_COL_ITEM][_MAX_ROW_ITEM];
        bool bTrayMapNGData[_MAX_COL_ITEM][_MAX_ROW_ITEM];                      //KenHsieh 20220923 : add Tray Map Throw IC Function

        AnsiString cCassetteID;
        AnsiString cCassetteLot;
        AnsiString cDeviceInf[_MAX_COL_ITEM][_MAX_ROW_ITEM];                    // 2012.06.01 Q_Q BarCode Mode
        AnsiString cSBin[_MAX_COL_ITEM][_MAX_ROW_ITEM];                         //Steven 20220120 : Amlogic需要收SBIN
        AnsiString cReDeviceInf[_MAX_COL_ITEM][_MAX_ROW_ITEM];                  // 2012.06.01 Q_Q BarCode Mode
        TMyProductionRecord *PordRec[_MAX_COL_ITEM][_MAX_ROW_ITEM];             //Steven 20221005 : Production Log減少記憶體使用量

        int  iCassetteCount[2];
        bool bMustToEmpty;                                                      //Sam 20230317 : 修正 P24 功能
        int  iNeedManualRemoved;                                                //Sam 20230317 : 修正 P24 功能  -1:不需要手動移除。0:搬到 Empty 後報警提示手動移除。1:搬到 Color 後報警提示手動移除

        void ClearData();

        void SetData(int data);
        bool SetBlockXYItem(int iBColItem, int iBRowItem);
        bool SetXYItem(int iColItem, int iRowItem);                             //Steven 20160614 : 設定Tray XY Item改用function加上保護

        bool HasIC();
        bool HasRealIC();
        bool HasCleanPad();                                                     //kevin 20150505 autoclean  判斷裡面有沒有CLEAN PAD
        bool HasOCRIC();                                                        //wei 20160606
        bool HasICCassette();                                                   //wei 20180702 MR
        bool HasDataIC(int DataType);                                           //wei 20180702 MR
        bool HasEmptyCassette();
        bool HasOnlyDataICAndNullIC(int DataType);                              //JimmyChiu 20220908 add Pickup Error Placement
        bool CleanPlate2HasIC();                                                //kevin 20120217 Autoclean 判斷是否有IC
        bool FullIC();

        int  HowManyIC();
        int  HowManyUpperHalfIC();                                              //Sam 20221101 : OneCycle 完成後顯示所有 Tray 各別的IC數量
        int  HowManyLowerHalfIC();                                              //Sam 20221101 : OneCycle 完成後顯示所有 Tray 各別的IC數量
        int  HowManyICInBuffer(int data);
        int  HowManyBinICInTray(int iBin);                                      //Sam 20240108 : 新增退 Tray 時顯示裡面有多少 Error Bin

        void SaveUnloaderInfo(AnsiString asFile);                               //ChungHung 20150205 add for ATK
        void ReadUnloaderInfo(AnsiString asFile);                               //ChungHung 20150205 add for ATK        
};
//------------------------------------------------------------------------------
#endif
