//---------------------------------------------------------------------------

#ifndef asendic_LoaderH
#define asendic_LoaderH
//---------------------------------------------------------------------------

extern int LoadTask;
extern int iSupplyNewIC_From_LoaderCar;
extern int iTrayZLoadTrayToWaitTask;
extern int iLoadNewICTrayTask;

extern void InitLoadTask();
extern void Init_TrayY_TrayZ_Task();
extern void InitLoadNewICTrayTask();
extern void InitTrayZLoadTrayToWaitTask();
extern void InitSupplyNewIC_From_LoaderCarTask();

extern void DoLoad();
extern void Do_TrayY_TrayZ();
extern bool DoTrayZLoadTrayToWait();
extern int iLoaderTrackDetectICFloatingTask;
extern void InitLoaderTrackDetectICFloatingTask();  //Sam 20200316 : Loader Detect Tray
extern bool DoLoaderTrackDetectICFloating();
extern bool CheckLoaderICFloating(int iDir);        //Sam 20240108 : 整合 Loader 跳料檢查。
void CheckContinManualRemoveTray();  //Sam 20221027 : 整合 P24 功能
#endif
