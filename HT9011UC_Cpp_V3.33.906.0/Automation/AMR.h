// =============================================================================
//  Automation/AMR.h  --  TeraPower AMR (Autonomous Mobile Robot) tray-count
//                         bridge  (W5 translation)
//
//  Faithful translation of golden Automation/AMR.h (36 lines, BCB6).
//  Golden: HT9011UC_Code_V3.33.906.0_20260618/Automation/AMR.h
//  Translation wave: W5 (Automation network unit)
//  Translator: AI(W5-AMR-Translate) 20260710
//
//  ROLE (golden AMR.cpp header comment context): TTeraPowerAMR tracks the
//  Loader / Auto1-3 tray counts and decides when the handler must notify the
//  external TeraPower AMR to supply a Loader tray or collect an Auto tray.
//  Pure counting + threshold decision logic over the global LastSet /
//  IniConfig / CosFunction / MOT[] / BinSelect[] state; the ONE UI touch in
//  the golden .cpp (fLotInfo->RefreshAMR()) is routed through FormsFacade
//  (see AMR.cpp file-head note + this unit's translate report).
//
//  VCL/Borland conversions:
//    * `#include "vclcompat/vcl_compat.h"` replaces the implicit <vcl.h>
//      AnsiString this header needs (AnsiString return types below).
//    * The golden in-class ctor declaration
//        `_fastcall TTeraPowerAMR::TTeraPowerAMR();`
//      is a Borland-only extension (a fully class-qualified name written
//      INSIDE the class body -- not legal in standard C++).  Translated as a
//      plain `TTeraPowerAMR();` -- `_fastcall`/`__fastcall` carries no ABI
//      meaning outside Borland and is dropped project-wide (established
//      convention, see KNOWLEDGE.md).
//    * `extern class TTeraPowerAMR AMR;` kept verbatim (valid standard C++;
//      the `class` elaborated-type-specifier on an extern decl is legal).
// =============================================================================
#ifndef AMRH
#define AMRH

#include "vclcompat/vcl_compat.h"   // AnsiString

class TTeraPowerAMR
{
public:
    TTeraPowerAMR();                                    //AI(W5-AMR-Translate) 20260710: golden `_fastcall TTeraPowerAMR::TTeraPowerAMR();` (Borland in-class qualified-ctor extension, not standard C++) -> plain ctor decl
    void Initial();                                     //Initial Start or Tray Feed 初始所有資料。

    void LoaderInCount();                               //Loader 進盤計數。
    void UnloaderOutCount(int iAuto);                   //Unlaoder 出盤計數。

    bool WaitAMRSupplyTray();                           //等待 AMR 補盤完成
    bool CheckLoaderCount();                            //判斷是否要通知 AMR 補盤或搬走。
    bool CheckUnloaderCount(int iAuto);                 //判斷是否要通知 AMR 補盤或搬走。

    void LoaderCarryIn();                               //AMR Loader 補盤數量累加。
    void UnloaderCarryOut(int iAuto);                   //AMR Unloader 搬走後數量清空。
    void ClearTrackBinData(int iAuto);                  //AMR Unloader 搬走後 Bin 數量清空。

    bool CheckTrayFeed();                               //確認 AMR 是否要 Tray Feed。

    AnsiString GetTrackBinData(int iAuto);              //取得 Bin 資料。
    AnsiString GetTrackBinDataCateR(int iAuto);         //取得 Bin 資料 Retest CateR。
    AnsiString GetNormalFailBin();                      //Spencerlin 20260202 : 取得 Auto1~3 不是 AutoRetest 或 CateR 的 Fail Bin
    void SupplyCover(int iAuto);                        //判斷 Unloader 是否要補盤。
    bool IsAutoReTestTray(int iAuto);                   //判斷是否為 ART Retest TrackPos

    void ARTReset();

    bool NeedAMRTransport();                              //判斷機台需要 AMR 搬運
};
extern class TTeraPowerAMR AMR;
#endif
