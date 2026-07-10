// =============================================================================
//  CCLink/MyCCLink.cpp  --  top-level CC-Link comm state machine (TMyCCLinkCtrl)
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/CCLink/MyCCLink.cpp
//  Translation wave: W5 (CCLink driver pair)
//  Translator: AI(W5-CCLink-Translate) 20260710
//
//  See MyCCLink.h for the full list of __published-property flattening /
//  fCCLink-form-coupling / TTimer-substitution notes. This .cpp preserves the
//  golden switch(Task) state machine (Timer1Timer) VERBATIM in shape; only the
//  call-site spellings change to match the flattened TMyCCLinkBoard API (see
//  MyCCLinkBoard.h) and the 3 fCCLink-form touches (see header notes + inline
//  comments below).
//
//    - `#pragma hdrstop` / `#pragma package(smart_init)` dropped (BCB-specific).
//    - `#include "MyCCLinkSensor.h"` (the golden TfCCLink FORM header) DROPPED
//      -- not needed once the fCCLink touches are threaded through as members
//      (see header notes).
//    - `#include "common.h"` kept: declares MySleep (dtor).
// =============================================================================
#include "MyCCLink.h"
#include <cstring>   // memset (DoSetMultiValue)

// AI(W5-CCLink-Translate) 20260710: MySleep is declared in golden common.h
// (common.h:261) but that declaration is currently GATED `#if 0 //
// TODO(wave-timing)` in the translated common.h (real body lives in
// acarry_shims.cpp, ht9045_sm -- a heavy dependency this small W5 comms
// driver should not require). Declared locally instead. See report.
extern void MySleep(DWORD dwMilliseconds);   // golden common.h:261

// AI(W5-CCLink-Translate) 20260710: golden declares MyDBIProcess in cMyDB.h (W3
// DEFERRED -- sqlite3 wrapper not translated yet). Forward-declared locally
// (not including the not-yet-translated cMyDB.h) matching the established
// "flag as a dependency, supply local test stub" pattern used elsewhere in
// this tree (e.g. test_config_loaders.cpp for MyDBIProcess/ShowMyMessage).
extern void MyDBIProcess(AnsiString S1, AnsiString S2);

//------------------------------------------------------------------------------
// constructor
//------------------------------------------------------------------------------
TMyCCLinkCtrl::TMyCCLinkCtrl(TComponent* Owner) : TComponent(Owner)
{
    // AI(W5-CCLink-Translate) 20260710: golden constructs CCLinkBoard with the GLOBAL
    // `fCCLink` pointer, not this ctor's own `Owner` parameter (see .h banner note for the
    // full rationale -- Owner IS ALWAYS fCCLink at the one real call site, so this is
    // behavior-equivalent and avoids depending on the undefined TfCCLink type here).
    CCLinkBoard=new TMyCCLinkBoard(Owner);
    iCCLinkTask=1;
    bReady=false;

    // AI(W5-CCLink-Translate) 20260710: golden creates+arms a real VCL TTimer (30ms interval,
    // OnTimer=Timer1Timer) here. No vclcompat::TTimer primitive exists yet in this tree (flagged
    // as a dependency, not invented in this unit -- see report). bTimerEnabled substitutes for
    // Timer1->Enabled; nothing here drives the 30ms cadence -- the integrate agent must either
    // add a vclcompat::TTimer shim or wire Timer1Timer() into a tick spine (e.g. W6.6 DoAllProcess).
    bTimerEnabled=true;
}
//------------------------------------------------------------------------------
// destructor
//------------------------------------------------------------------------------
TMyCCLinkCtrl::~TMyCCLinkCtrl()
{
    try
    {
        bTimerEnabled=false;   // golden: Timer1->Enabled=false;
        MySleep(100);
        if(CCLinkBoard!=NULL)
            delete CCLinkBoard;
    }
    catch(...)
    {
        MyDBIProcess("Exception", "~TMyCCLinkCtrl");
    }
}
//------------------------------------------------------------------------------
// AI(W5-CCLink-Translate) 20260710: golden extern decls (unused in this file --
// no T_HEX2ASCII/T_ASXII2HEX/T_HEX2ASCII_Mac/T_ASXII2HEX_Mac call site exists
// here; preserved verbatim per the golden "Steven 20100517: avoid #include
// cpublic.h" comment banner in MyCCLink.h). Harmless as plain extern decls.
extern const unsigned char T_HEX2ASCII[16];
extern const unsigned char T_ASXII2HEX[23];
extern unsigned char T_HEX2ASCII_Mac(unsigned char hex2ascii);
extern unsigned char T_ASXII2HEX_Mac(unsigned char ascii2hex);
//---------------------------------------------------------------------------
void TMyCCLinkCtrl::WriteCCLinkStatus(bool Value)
{
    iCCLinkStatus=Value;
    if(Value==true)
    {
        bHasErr=false;
        ErrMess="";
        iCCLinkTask=3;
    }
}
//---------------------------------------------------------------------------
void TMyCCLinkCtrl::DoSetValue(short sNu, short sSen, int Value)                //set a single sensor's threshold
{
    CCLinkBoard->iDoSetValueTask=1;
    CCLinkBoard->sSetNu=sNu;                                                    //the NU-CL1 to be configured
    CCLinkBoard->sSetSen=sSen;                                                  //the sensor to be configured
    CCLinkBoard->iSetValue=Value;                                               //the value to be set
    bSetValue=true;
}
//---------------------------------------------------------------------------
void TMyCCLinkCtrl::DoSetMultiValue(short sNu, short sSen, int *Value, int Count)    //set several sensors' thresholds
{
    if(bSetMultiValue==true)                                                    //jou 20221004 : avoid re-entering DoSetMultiValue
        return;

    memset(CCLinkBoard->iSetMultiValue, 0, sizeof(CCLinkBoard->iSetMultiValue));    //wei 20180622  CCLink Value Save File
    CCLinkBoard->iDoSetMultiValueTask=1;
    CCLinkBoard->sSetNu=sNu;                                                    //the NU-CL1 to be configured
    CCLinkBoard->sSetSen=sSen;                                                  //the sensor to be configured

    // AI(W5-CCLink-Translate) 20260710: thread Count into the board (substitutes for golden's
    // fCCLink->iAllSetCount read in TMyCCLinkBoard::DoSetMultiValue case 5 -- see
    // MyCCLinkBoard.h/.cpp notes). Golden never stores Count on CCLinkBoard; it relies on the
    // deferred TfCCLink form's iAllSetCount field always matching Count at the one real call
    // site (MyCCLinkSensor.cpp:776).
    CCLinkBoard->SetAllSetCount(Count);

    for(int i=0; i<Count; i++)
        CCLinkBoard->iSetMultiValue[i]=Value[i];                                //the values to be set
    bSetMultiValue=true;
}
//------------------------------------------------------------------------------
void TMyCCLinkCtrl::Timer1Timer(TObject * /*Sender*/)
{
    // AI(W5-CCLink-Translate) 20260710: substitutes for the VCL TTimer's own Enabled gate (a
    // real TTimer with Enabled==false simply never fires OnTimer) -- see .h note.
    if(bTimerEnabled==false)
        return;

    if(bInitialOK==false)                                                       //jou 2010-05-19 start : not yet initialized, cannot run
        return;

    if(bHasErr==true)
        return;

    int &Task=iCCLinkTask;
    CCLinkBoard->lPath=lPath;   // golden: CCLinkBoard->Path=lPath; (Path flattened to lPath -- see MyCCLinkBoard.h)
    switch(Task)
    {
        case 1:
            CCLinkBoard->iCCLinkBoardTask=1;
            bReady=false;
            Task=2;
        case 2:
            SysMess="Connecting CC-Link Board.";
            if(CCLinkBoard->DoConnectBoard(iAutoRetryCnt))                                   //initialize the connection
            {
                iNucl1No=CCLinkBoard->iConnectModule;   // golden: CCLinkBoard->ConnectModuleNo
                bHasErr=false;
                ErrMess="";
                Task=50;
            }

            if(CCLinkBoard->bHasErr)   // golden: CCLinkBoard->HasError
            {
                bHasErr=true;
                ErrMess=CCLinkBoard->ErrMess;   // golden: CCLinkBoard->ErrorMessage
            }
            break;
        case 3:
            SysMess="Resetting CC-Link Board.";
            CCLinkBoard->ResetBoard();
            CCLinkTimer.Set0_1SecAndOn(200);
            Task=4;
            iCCLinkStatus=false;
            break;
        case 4:
            if(CCLinkTimer.Off())
                Task=1;
            break;
        case 50:                                                                //lock all sensors' key buttons
            CCLinkBoard->SetSensorKeyLock(true);   // golden: CCLinkBoard->SensorKeyLock=true;
            Task=60;
            break;
        case 60:
            SysMess="Doing Sensor Key Lock.";
            if(CCLinkBoard->bKeyLock==true)   // golden: CCLinkBoard->SensorKeyLock==true
            {
                bHasErr=false;
                ErrMess="";
                CCLinkBoard->iReadSetValueTask=1;
                Task=70;
            }

            if(CCLinkBoard->bHasErr)   // golden: CCLinkBoard->HasError
            {
                bHasErr=true;
                ErrMess=CCLinkBoard->ErrMess;   // golden: CCLinkBoard->ErrorMessage
            }
            break;
        case 70:
            SysMess="Reading Set Values.";
            if(CCLinkBoard->DoReadSetValue())                                   //read the sensors' threshold values
            {
                bHasErr=false;
                ErrMess="";
                Task=100;
            }

            if(CCLinkBoard->bHasErr)   // golden: CCLinkBoard->HasError
            {
                bHasErr=true;
                ErrMess=CCLinkBoard->ErrMess;   // golden: CCLinkBoard->ErrorMessage
                Task=1;
            }
            break;
        case 100:                                                               //check system status (Wr0000==0)
            if(CCLinkBoard->DoCheckWr0000())                                    //read sensor data
            {
                if(CCLinkBoard->bHasErr)   // golden: CCLinkBoard->HasError
                {
                    bHasErr=true;
                    ErrMess=CCLinkBoard->ErrMess;   // golden: CCLinkBoard->ErrorMessage
                    Task=1;
                }
                else
                {
                    bHasErr=false;
                    ErrMess="";
                    Task=110;
                }
            }

            if(CCLinkBoard->bHasErr)   // golden: CCLinkBoard->HasError
            {
                bHasErr=true;
                ErrMess=CCLinkBoard->ErrMess;   // golden: CCLinkBoard->ErrorMessage
                Task=1;
            }
            break;
        case 110:
            bReady=true;
            if(bSetValue)
                Task=2000;
            else if(bSetMultiValue)
                Task=3000;
            else if(bNewFile)                                                   //wei 20180622 (Steven) : CCLink Value Save File
                // AI(W5-CCLink-Translate) 20260710: golden reads fCCLink->bNewFile (deferred
                // TfCCLink form field, golden MyCCLinkSensor.h:424); hosted here on
                // TMyCCLinkCtrl instead (see .h note). Nothing in this translated scope
                // currently sets bNewFile=true (that trigger lives in the deferred form,
                // golden MyCCLinkSensor.cpp:720).
                Task=4000;
            else
                Task=1000;
            break;
        case 1000:
            SysMess="Reading Current Values.";
            if(CCLinkBoard->DoReadCurrValue())                                  //read sensor data
            {
                bHasErr=false;
                ErrMess="";
                Task=1500;
            }

            if(CCLinkBoard->bHasErr)   // golden: CCLinkBoard->HasError
            {
                bHasErr=true;
                ErrMess=CCLinkBoard->ErrMess;   // golden: CCLinkBoard->ErrorMessage
                Task=1;
            }
            break;
        case 1500:
            SysMess="Reading On Off Status.";
            if(CCLinkBoard->DoReadOnOff())                                      //read sensor data
            {
                bHasErr=false;
                ErrMess="";
                Task=100;
            }

            if(CCLinkBoard->bHasErr)   // golden: CCLinkBoard->HasError
            {
                bHasErr=true;
                ErrMess=CCLinkBoard->ErrMess;   // golden: CCLinkBoard->ErrorMessage
                Task=1;
            }
            break;
        case 2000:
            SysMess="Setting Value.";
            if(CCLinkBoard->DoSetValue())
            {
                bHasErr=false;
                ErrMess="";
                bSetValue=false;
                Task=100;
            }

            if(CCLinkBoard->bHasErr)   // golden: CCLinkBoard->HasError
            {
                bHasErr=true;
                ErrMess=CCLinkBoard->ErrMess;   // golden: CCLinkBoard->ErrorMessage
                Task=1;
            }
            break;
        case 3000:
            SysMess="Setting Value.";
            if(CCLinkBoard->DoSetMultiValue())
            {
                bHasErr=false;
                ErrMess="";
                bSetMultiValue=false;
                Task=70;
            }

            if(CCLinkBoard->bHasErr)   // golden: CCLinkBoard->HasError
            {
                bHasErr=true;
                ErrMess=CCLinkBoard->ErrMess;   // golden: CCLinkBoard->ErrorMessage
                Task=1;
            }
            break;

        case 4000:                                                              //wei 20180622  CCLink Value Save File
            // AI(W5-CCLink-Translate) 20260710: golden calls fCCLink->SaveSetupFile() here -- a
            // VCL-widget-reading method on the not-yet-translated TfCCLink form (golden
            // MyCCLinkSensor.h:422; reads dozens of edIn1_x/edIn2_x threshold edits and tray
            // sensor grids and writes the CC-Link setup/DAT file). No plain-field substitute is
            // possible; gated below (TODO(W7-UI)). The cursor still advances so the state
            // machine's SHAPE is preserved -- only the actual file save is skipped offline.
#if 0   // TODO(W7-UI): TfCCLink::SaveSetupFile() (golden CCLink/MyCCLinkSensor.h:422)
            fCCLink->SaveSetupFile();
#endif
            bNewFile=false;
            Task=100;
            break;
    }
}
//------------------------------------------------------------------------------
