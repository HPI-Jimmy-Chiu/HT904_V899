//---------------------------------------------------------------------------

#ifndef AMRH
#define AMRH
//---------------------------------------------------------------------------
class TTeraPowerAMR
{
    public:
    _fastcall TTeraPowerAMR::TTeraPowerAMR();
    void Initial();                                     //Initial Start or Tray Feed ﹍┮Τ戈

    void LoaderInCount();                               //Loader 秈絃璸计
    void UnloaderOutCount(int iAuto);                   //Unlaoder 絃璸计

    bool WaitAMRSupplyTray();                           //单 AMR 干絃ЧΘ
    bool CheckLoaderCount();                            //耞琌璶硄 AMR 干絃┪穐ǐ
    bool CheckUnloaderCount(int iAuto);                 //耞琌璶硄 AMR 干絃┪穐ǐ

    void LoaderCarryIn();                               //AMR Loader 干絃计秖仓
    void UnloaderCarryOut(int iAuto);                   //AMR Unloader 穐ǐ计秖睲
    void ClearTrackBinData(int iAuto);                  //AMR Unloader 穐ǐ Bin 计秖睲

    bool CheckTrayFeed();                               //絋粄 AMR 琌璶 Tray Feed

    AnsiString GetTrackBinData(int iAuto);              //眔 Bin 戈
    AnsiString GetTrackBinDataCateR(int iAuto);         //眔 Bin 戈 Retest CateR
    AnsiString GetNormalFailBin();                      //Spencerlin 20260202 : 眔 Auto1~3 ぃ琌 AutoRetest ┪ CateR  Fail Bin
    void SupplyCover(int iAuto);                        //耞 Unloader 琌璶干絃
    bool IsAutoReTestTray(int iAuto);                   //耞琌 ART Retest TrackPos

    void ARTReset();

    bool NeedAMRTransport();                              //耞诀惠璶 AMR 穐笲
};
extern class TTeraPowerAMR AMR;
#endif
