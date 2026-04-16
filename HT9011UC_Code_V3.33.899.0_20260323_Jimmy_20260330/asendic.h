//---------------------------------------------------------------------------
#ifndef asendicH
#define asendicH
//---------------------------------------------------------------------------

#include "asendic_Loader.h"
#include "asendic_Loader_RT.h"
#include "asendic_Auto.h"
#include "asendic_Auto_RT.h"
#include "asendic_Auto2.h"
#include "asendic_Empty.h"
#include "asendic_Color.h"

extern int iTrayZLoadTrayToWaitTask;

extern bool CylinderUp(int CylinderName);
extern bool CylinderMiddle(int CylinderName);
extern bool CylinderLower(int CylinderName);
extern bool AutoCylinderUp(int Part, int CylinderName, int CylinderNameMid, bool bReset=false);     //kevin 20120718 修改輸入氣缸因代號位置不同共用程式)//Part 0=Auto1 1=Auto2 2=Auto3.....
extern bool AutoCylinderMiddle(int Part, int CylinderName, int CylinderNameMid, bool bReset=false);
extern bool AutoCylinderLower(int Part, int CylinderName, int CylinderNameMid, bool bReset=false);  //kevin 20120718 修改輸入氣缸因代號位置不同共用程式
extern void PushLoaderTrayInAverageTime(DWORD st);
extern void RecordAutoCleanOutStartEnd(bool bStart);                            //ChungHung 20130910 add for SCK
extern void NewDoAutoTrayEdgeCylinderLoop();
extern bool DoAutoTrayEdgeCylinderLoop(int iAuto);
extern void PushUnLoaderTrayInAverageTime(DWORD st, int pos);

extern void TrayMoveIn(bool bMove, int iAxis, int iPos=0);
extern void TrayMoveOut(bool bMove, int iAxis, int iPos=0);
extern void TrayCylinMoveIn(int iAxis);
extern void TrayCylinMoveOut(int iAxis);

extern int TrayMoveStatus(int iAxis,AnsiString sFun);                           //Sam 20210325 : 整合 tray move 狀態
void InitDoLoaderTrayFeedTask();                                                //kevin 20211110
bool DoLoaderTrayFeed();                                                        //kevin 20211110
extern bool bARTUnloaderUseOneCylin(int iPart);                                 //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
extern bool bARTUnloaderUseTwoCylin(int iPart);                                 //Sam 20220916 : 整合 ART Unload 上升汽缸判斷式
//==============================================================================
// 1999/08/15  處理送空IC TRAY到TRAY塔但不包括提供新TRAY
//==============================================================================
//void SendICTrayToReceiveTrayBuffer();
//==============================================================================
// 1999/08/15  處理供IC之TRAY移動
//==============================================================================
//void DoSendTrayMotor();
//==============================================================================
// 1999/08/15  處理供IC之TRAY供應,若在 Tray set中,則會call收tray function
//==============================================================================
//void DoSendEmptyTrayToBuild();
//==============================================================================
// 1999/08/15  處理供IC之TRAY 搜尋
//==============================================================================
//bool DoSendTraySort();
//void DoTubeLoaderToTubeBuffer();
//extern bool bLoaderTubePressHasTube;
//extern bool bLockUnloadKit[6];
//extern int iLoadTubeToBufferCount;
//extern int iTubeTrackCount[2];
//extern bool bUnloadAutoHasTube;
#endif
