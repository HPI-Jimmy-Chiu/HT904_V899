//---------------------------------------------------------------------------
#ifndef ainarm_SearchPlacePlateH
#define ainarm_SearchPlacePlateH

#include "cmydef.h"
//---------------------------------------------------------------------------

extern bool DoPlaceToHotPlate_9045();
extern void DoPlaceToHPSwapData(int iSuckRow, int iSuckCol, int iPlate, int iPlateR, int iPlateC);                      //Steven 20170109 : 將放料到HP資料交換改成Function
extern void DoPlaceToHPBackupData(int iSuckRow, int iSuckCol, int iPlate, int iPlateR, int iPlateC);                    //Ifor 20170928 (Steven) : 將Auto Site Mapping 資料寫回到HP

extern int  iInArmToHPX;
extern int  iInArmToHPY;
extern int  iInXPToHP[X_PITCH_COUNT];
extern int  iZPosToHP[MAX_ARM_Row][MAX_ARM_Col];
extern bool bZFlgToHP[MAX_ARM_Row][MAX_ARM_Col];
extern bool bZFlgToHPUsage[MAX_ARM_Row][MAX_ARM_Col];
extern bool bZFlgToHPPick[MAX_ARM_Row][MAX_ARM_Col];
extern void ResetInToHPFlag();

extern bool bPlaceToHotplatePartOK;                                             //ChungHung 20120109 add

extern int  iPlaceHP;
extern int  iPickHP;
extern int  iTryPickHP;

extern int  GetInArmToHPOffset_9045(int iAction);
extern int  GetPlaceToHotPlateCol(int j);                                       //加熱盤位置
extern int  GetPlaceToHotPlateSuckCol(int j);                                   //吸嘴位置
extern int  GetHotPlateColStep(int iAction);
extern int  GetVariableXInHotPlateData(int iAction);
extern int  GetVariableYInHotPlateData();                                       //ChungHung 20131231 alter AutoYPitch
extern void GetHotPlateYHalfPos();

extern bool HotPlateYPitchCanPutAll();

extern void GetInArmZToHPPos(int iAction, int iCT);
extern bool CheckHasSpaceToPlace_9045();
extern bool ArmRow1NotICForPlaceToPlate();                                      //檢測第一排是否有IC return false:有IC true:沒有IC  //ChungHung 20120206 Hotplate check
extern bool Row2CanPutHP(bool bPlaceMode);
extern void ResetHotPlateSearchParameter();

extern bool MoveInArmXYToHotPlatePlace(int iAction, bool IncludeZ);

extern TQPF_Timer tHotPlateHangUp;
extern int iHPHangUpCount;
extern AnsiString sHPHangUpFunc;
extern void ClearHotPlateHangUp();                                              //Steven 20161102 : 針對加熱盤Hang Up進行紀錄
extern void DoHotPlateHangUp(AnsiString FuncName);                              //Steven 20161102 : 針對加熱盤Hang Up進行紀錄

class TMyHotPlatePlaceLog                                                       //Steven 20211110 : 記錄放料到加熱盤的位置
{
    public:
        __fastcall TMyHotPlatePlaceLog();
        void InitialPosition();
        void CheckPosition(int iXPos, int iYPos, int Vari, int YVari, int Vari2);
        void SetPosition();
        int iX;
        int iY;
        int iVari;
        int iYVari;
        int iVari2;

        int iTempX;
        int iTempY;
        int iTempVari;
        int iTempYVari;
        int iTempVari2;
        bool bIsSamePos;
};
extern TMyHotPlatePlaceLog HPPlaceLog;                                          //Steven 20211110 : 記錄放料到加熱盤的位置
#endif
