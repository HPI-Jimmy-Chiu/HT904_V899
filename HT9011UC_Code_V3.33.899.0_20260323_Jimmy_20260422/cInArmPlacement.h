//---------------------------------------------------------------------------

#ifndef cInArmPlacementH
#define cInArmPlacementH
//---------------------------------------------------------------------------
#include <StdCtrls.hpp>
#include <map>
#include "myTimer.h"
//---------------------------------------------------------------------------
class cInArmPlacement                                                           //JimmyChiu 20220908 add Pickup Error Placement
{
private:    // User declarations
    //for shuttle
    int iShuttleLine;
    int iWhichShuttle;
    int iSuckToRecycleFromShuttleTask;
    int iSuckFromShuttleToPlaceTask;
    int iSuckFromShuttleTask;
    int iPlaceSuckToPlacementTask;
    //for Loader
    int iSuckToRecycleFromLoaderTask;
    int iSuckFromLoaderToPlaceTask;
    int iSuckFromLoaderTask;
    //
    TQPF_Timer timeArmDownDelay;
    enum ShuttleRow
    {
        ShRow1=0,
        ShRow2
    };
public:     // User declarations
    cInArmPlacement();
    ~cInArmPlacement();
    void InitialDatas();
    //parameter
    AnsiString asErrorInArmPlacement;
    ////for shuttle
    void InitialShuttleDatas();
    int  DoCheckShuttleSensor();
    void GetShuttlePos(int iSht, int iRow, int iCol, int &iPosX, int &iPosY);
    void GetPlacementPos(int &iPosX, int &iPosY);
    bool MoveInArmXY(int iPosX, int iPosY);
    void DoInArmPlaceDown(bool bDown);
    void DoInArmPlaceSuck(bool bSuck);
    bool CheckInArmPlaceSuckHasIC();
    bool InArmPlacementEnable();
    //process
    bool DoSuckToRecycleFromShuttle(bool bIsFirst, AnsiString &asError);
    bool DoSuckFromShuttleToPlace(bool bIsFirst, int iSht, int iCol, AnsiString &asError);
    bool DoSuckFromShuttle(bool bIsFirst, int iSht, int iRow, int iCol, AnsiString &asError);
    bool DoPlaceSuckToPlacement(bool bIsFirst, AnsiString &asError);

    ////for Loader
    bool DoCheckLoaderHasErrorIC(int &iRow, int &iCol);
    void GetLoaderPos(int iRow, int iCol, int &iPosX, int &iPosY);

    //process
    bool DoSuckToRecycleFromLoader(bool bIsFirst, AnsiString &asError);
    bool DoSuckFromLoaderToPlace(bool bIsFirst, int iRow, int iCol, AnsiString &asError);
    bool DoSuckFromLoader(bool bIsFirst, int iRow, int iCol, AnsiString &asError);

};
//---------------------------------------------------------------------------
#endif
