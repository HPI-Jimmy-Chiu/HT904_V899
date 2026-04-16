#include "MachineDefine.h"
#pragma hdrstop

#include "mytray.h"

#include "cmydef.h"
//------------------------------------------------------------------------------
#pragma package(smart_init)
//------------------------------------------------------------------------------
__fastcall TMyTray::TMyTray()
{
    XItem=_MAX_COL_ITEM;
    YItem=_MAX_ROW_ITEM;
    XBItem=1;
    YBItem=1;
    bMustToEmpty=false;                                                         //Sam 20230317 : 修正 P24 功能
    iNeedManualRemoved=-1;
}
//------------------------------------------------------------------------------
__fastcall TMyTray::~TMyTray()
{
//Steven 20210304 : 避免解構失敗又沒辦法存Log
//    try
//    {
//        for(int iC=0; iC<_MAX_COL_ITEM; iC++)
//        {
//            for(int iR=0; iR<_MAX_ROW_ITEM; iR++)
//            {
//                  PordRec[iC][iR].DeleteProductionRecord();
//            }
//        }
//    }
//    catch(...)
//    {
//        MyDBIProcess("Exception", "~TMyTray");
//    }
}
//------------------------------------------------------------------------------
void TMyTray::ClearData()
{
    for(int iC=0; iC<XItem; iC++)
    {
        for(int iR=0; iR<YItem; iR++)
        {
            Data[iC][iR]          =NULL_IC;
            BufferData[iC][iR]    =NULL_IC;
            iTarget[iC][iR]       =0;                                           //Steven 20150915 : For TSMC 手動整盤功能
            cDeviceInf[iC][iR]    ="";                                          // 2012.06.01 Q_Q BarCode Mode
            cSBin[iC][iR]         ="";                                          //Steven 20220120 : Amlogic需要收SBIN
            b2DIDNG[iC][iR]       =false;                                       //Steven 20200611 : for Murata, 2DID NG不測試
            cReDeviceInf[iC][iR]  ="";                                          // 2012.06.01 Q_Q BarCode Mode
            cCassetteID         ="";
            cCassetteLot        ="";
            if(PordRec[iC][iR]!=NULL)                                           //Steven 20221005 : Production Log減少記憶體使用量
                PordRec[iC][iR]->InitialRecord();
            iCleanCount[iC][iR]   =0;
            bFliped[iC][iR]       =false;
            iBinData[iC][iR]      =-1;                                          //JerryYang 20220909 : add magazine
            iAOIResult[iC][iR]    =0;                                           //Sam 20240325 : 新增 DamageTrayMapping 功能
        }
    }
    iCassetteCount[0]     =0;
    iCassetteCount[1]     =0;
    ZeroMemory(iWhichSite, sizeof(iWhichSite));
    ZeroMemory(iWhichIndex, sizeof(iWhichIndex));
    ZeroMemory(iBinCode, sizeof(iBinCode));
    ZeroMemory(iNeedRotAng, sizeof(iNeedRotAng));                               //Steven 20170425 (wei) : Add rotate motor
    ZeroMemory(iCurrRotAng, sizeof(iCurrRotAng));
};
//------------------------------------------------------------------------------
struct UnloaderData
{
    int iWhichSite[_MAX_COL_ITEM][_MAX_ROW_ITEM];
    int iWhichIndex[_MAX_COL_ITEM][_MAX_ROW_ITEM];                              //ChungHung 20150205 add for ATK
    int iBinCode[_MAX_COL_ITEM][_MAX_ROW_ITEM];
};
//------------------------------------------------------------------------------
void TMyTray::SaveUnloaderInfo(AnsiString asFile)
{
    DWORD wtfz;
    HANDLE Fp;

    Fp=CreateFile(asFile.c_str(), FILE_SHARE_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if(Fp!=INVALID_HANDLE_VALUE)
    {
        WriteFile(Fp, &iWhichSite, sizeof(UnloaderData), &wtfz, NULL);
    }
    CloseHandle(Fp);
    return;
}
//------------------------------------------------------------------------------
void TMyTray::ReadUnloaderInfo(AnsiString asFile)
{
    DWORD rdfz;
    HANDLE Fp;
    Fp=CreateFile(asFile.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
    if(Fp!=INVALID_HANDLE_VALUE)
    {
        ReadFile(Fp, &iWhichSite, sizeof(UnloaderData), &rdfz, NULL);
    }
    CloseHandle(Fp);
    return;
}
//------------------------------------------------------------------------------
bool TMyTray::SetXYItem(int iColItem, int iRowItem)                             //Steven 20160614 : 設定Tray XY Item改用function加上保護
{
    bool bResult=true;
    if(iColItem>_MAX_COL_ITEM)
    {
        bResult=false;
        iColItem=_MAX_COL_ITEM;
    }

    if(iRowItem>_MAX_ROW_ITEM)
    {
        bResult=false;
        iRowItem=_MAX_ROW_ITEM;
    }

    if(IniConfig.bP06_LoaderUseCarrierTray)                                     //wei 20161123 使用CarrierTray需卡上限值
    {
        XItem=iColItem*XBItem;
        YItem=iRowItem*YBItem;

        if(XItem>_MAX_COL_ITEM)
        {
            bResult=false;
            XItem=_MAX_COL_ITEM;
        }

        if(YItem>_MAX_ROW_ITEM)
        {
            bResult=false;
            YItem=_MAX_ROW_ITEM;
        }
    }
    else
    {
        XItem=iColItem;
        YItem=iRowItem;
    }
    return bResult;
}
//------------------------------------------------------------------------------
bool TMyTray::SetBlockXYItem(int iColItem, int iRowItem)                        //Steven 20160614 : 設定Tray XY Item改用function加上保護
{
    bool bResult=true;
    if(iColItem>2)
    {
        bResult=false;
        iColItem=2;
    }

    if(iRowItem>5)
    {
        bResult=false;
        iRowItem=5;
    }

    if(iColItem!=1 || iRowItem!=1)
    {
        XBWidth=3;
        YBWidth=3;
    }
    else
    {
        XBWidth=1;
        YBWidth=1;
    }

    XBItem=iColItem;
    YBItem=iRowItem;
    return bResult;
}
//------------------------------------------------------------------------------
void TMyTray::SetData(int data)
{
    for(int iC=0; iC<XItem; iC++)
        for(int iR=0; iR<YItem; iR++)
            Data[iC][iR]=data;
};
//------------------------------------------------------------------------------
bool TMyTray::HasIC()
{
    for(int iC=0; iC<XItem; iC++)
        for(int iR=0; iR<YItem; iR++)
            if(Data[iC][iR])
                return true;
    return false;
}
//------------------------------------------------------------------------------
bool TMyTray::HasRealIC()
{
    for(int iC=0; iC<XItem; iC++)
        for(int iR=0; iR<YItem; iR++)
            if(Data[iC][iR] && Data[iC][iR]!=HAS_NULL_IC)
                return true;
    return false;
}
//------------------------------------------------------------------------------
bool TMyTray::FullIC()
{
    for(int iC=0; iC<XItem; iC++)
        for(int iR=0; iR<YItem; iR++)
            if(Data[iC][iR]==NULL_IC)
                return false;
    return true;
}
//------------------------------------------------------------------------------
int TMyTray::HowManyIC()
{
    int sum=0;
    for(int iC=0; iC<XItem; iC++)
        for(int iR=0; iR<YItem; iR++)
            if(Data[iC][iR]!=NULL_IC &&
               Data[iC][iR]!=HAS_NULL_IC)                                       //Steven 20190627 : 修改HowManyIC()計算方式
                sum++;
    return sum;
}
//------------------------------------------------------------------------------
int TMyTray::HowManyUpperHalfIC()                                               //Sam 20221101 : OneCycle 完成後顯示所有 Tray 各別的IC數量
{
    int sum=0;
    for(int iC=0; iC<XItem; iC++)
        for(int iR=0; iR<YItem/2; iR++)
            if(Data[iC][iR]!=NULL_IC && Data[iC][iR]!=HAS_NULL_IC)              //Steven 20190627 : 修改HowManyIC()計算方式
                sum++;
    return sum;
}
//------------------------------------------------------------------------------
int TMyTray::HowManyLowerHalfIC()                                               //Sam 20221101 : OneCycle 完成後顯示所有 Tray 各別的IC數量
{
    int sum=0;
    for(int iC=0; iC<XItem; iC++)
        for(int iR=YItem/2; iR<YItem; iR++)
            if(Data[iC][iR]!=NULL_IC &&
               Data[iC][iR]!=HAS_NULL_IC)                                       //Steven 20190627 : 修改HowManyIC()計算方式
                sum++;
    return sum;
}
//------------------------------------------------------------------------------
bool TMyTray::CleanPlate2HasIC()                                                //kevin 20130509 autoclean   有AUTCLEAN 但結批不判斷
{
    for(int iC=0; iC<XItem; iC++)
        for(int iR=4; iR<YItem; iR++)
            if(Data[iC][iR])
                return true;

    return false;
}
//------------------------------------------------------------------------------
bool TMyTray::HasCleanPad()                                                     //kevin 20150505 autoclean  判斷裡面有沒有CLEAN PAD
{
    for(int iC=0; iC<XItem; iC++)
        for(int iR=0; iR<YItem; iR++)
            if(Data[iC][iR]==HAS_CLEAN_IC ||
               Data[iC][iR]==CLEAN_FINISH_IC ||
               Data[iC][iR]==HAS_NULL_CLEAN_IC)                                 //Steven 20200424 : Add
                return true;

    return false;
}
//------------------------------------------------------------------------------
bool TMyTray::HasOCRIC()                                                        //wei 20160606
{
    for(int iC=0; iC<XItem; iC++)
        for(int iR=0; iR<YItem; iR++)
            if(Data[iC][iR]!=HAS_OCR_OK &&
               Data[iC][iR]!=NULL_IC &&
               Data[iC][iR]!=HAS_OCR_Err)
                return true;
    return false;
}
//------------------------------------------------------------------------------
bool TMyTray::HasICCassette()                                                   //wei 20180702 MR
{
    for(int iC=0; iC<XItem; iC++)
        for(int iR=0; iR<YItem; iR++)
            if(Data[iC][iR]==HAS_CASSETTE_DEVICE ||
               Data[iC][iR]==HAS_CASSETTE_TRAY)
                return true;
    return false;
}//-----------------------------------------------------------------------------
bool TMyTray::HasDataIC(int DataType)                                           //wei 20180702 MR
{
    for(int iC=0; iC<XItem; iC++)
        for(int iR=0; iR<YItem; iR++)
            if(Data[iC][iR]==DataType)
                return true;
    return false;
}
//------------------------------------------------------------------------------
bool TMyTray::HasEmptyCassette()                                                //wei 20180702 MR
{
    for(int iC=0; iC<XItem; iC++)
        for(int iR=0; iR<YItem; iR++)
            if(Data[iC][iR]==HAS_CASSETTE_EMPTY)
                return true;
    return false;
}
//------------------------------------------------------------------------------
int TMyTray::HowManyICInBuffer(int data)
{
    int sum=0;
    for(int iC=0; iC<XItem; iC++)
        for(int iR=0; iR<YItem; iR++)
            if(BufferData[iC][iR]==data)
                sum++;
    return sum;
}
//------------------------------------------------------------------------------
bool TMyTray::HasOnlyDataICAndNullIC(int DataType)                              //JimmyChiu 20220908 add Pickup Error Placement
{
    bool bHasDataType=false;
    for(int iC=0; iC<XItem; iC++)
        for(int iR=0; iR<YItem; iR++)
            if(Data[iC][iR]>0)
            {
                if(Data[iC][iR]==DataType)
                    bHasDataType=true;
                else
                    return false;
            }
    return bHasDataType;
}
//------------------------------------------------------------------------------
int TMyTray::HowManyBinICInTray(int iBin)                                       //Sam 20240108 : 新增退 Tray 時顯示裡面有多少 Error Bin
{
    int sum=0;
    for(int iC=0; iC<XItem; iC++)
        for(int iR=0; iR<YItem; iR++)
            if(iBinCode[iC][iR]==iBin)
                sum++;
    return sum;
}
//------------------------------------------------------------------------------
